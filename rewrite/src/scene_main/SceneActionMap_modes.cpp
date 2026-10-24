// SceneActionMap: mode changes (battle / loading / interlude), the end-of-stage notification, the player's revival,
// the aim target, the tutorial hooks and the enemy turn ticks.
#include "SceneActionMap.hpp"

using namespace smain;
using rw::guest_addr;

namespace {

namespace g {
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t SafeBuffer_read = 0x45A269;            // (buf, dst, src, size, offset)
constexpr uint32_t SafeBuffer_write = 0x459FA1;           // (buf, dst, src, size, offset)
constexpr uint32_t StageState_addDamage = 0x4EEB89;
constexpr uint32_t StageActorManager_attackReadyAction = 0x74FCED;
constexpr uint32_t StageActorManager_getLastEncountTeamInfo = 0x74F5B1;
constexpr uint32_t StageActorManager_clearCurrentTarget = 0x74F5F5;
constexpr uint32_t StageActorManager_changeMode = 0x74FCBD;
constexpr uint32_t BaseActorManager_isPlayerAttackSkipStatus = 0x74E8AD;
constexpr uint32_t StageFieldManager_getScreenPosToMapPos = 0x756559;   // (Vec2* out, manager, const Vec2&)
constexpr uint32_t StageFieldManager_changeMode = 0x7576D3;
constexpr uint32_t StageFieldManager_getCurrentEventMapTreasure = 0x757275;
constexpr uint32_t StageFieldManager_isProcessedPrizes = 0x757289;
constexpr uint32_t StageManager_changeMode = 0x75C4ED;
constexpr uint32_t StageManager_getWorldCode = 0x75BAF5;
constexpr uint32_t StageBattleManager_clearEnemyAttker = 0x753BBF;
constexpr uint32_t StageDirection_cocos_clear_colosseum = 0x7693A5;
constexpr uint32_t StageDirection_cocos_finish = 0x7691BD;
constexpr uint32_t StageDirection_to_end = 0x769EF5;
constexpr uint32_t CommonDirection_req_skilPanel_in = 0x765A19;
constexpr uint32_t CommonDirection_req_materialPanel_in = 0x765491;
constexpr uint32_t StageUI_getMedalField = 0x75E65D;
constexpr uint32_t StageUI_getCarouselDeck = 0x75E661;
constexpr uint32_t StageUI_getPlayerStatusField = 0x75E671;
constexpr uint32_t StageUI_deleteTutorialWidget = 0x75E5E1;
constexpr uint32_t MedalDeck_setBattleState = 0x5DFEAB;
constexpr uint32_t CarouselDeck_setBattleState = 0x5D1407;
constexpr uint32_t CarouselDeck_reflashDeck = 0x5D0BC5;
constexpr uint32_t CarouselDeck_disableButton = 0x5D13E1;
constexpr uint32_t PlayerStatusIndicator_setHP = 0x5E3119;
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;
constexpr uint32_t AvatarActor_ClearBuffDebuff = 0x7401B9;
constexpr uint32_t AvatarActor_hasPrize = 0x741159;
constexpr uint32_t AimTarget_reset = 0x73F5B5;
constexpr uint32_t AimMarker_show = 0x73FBA1;
constexpr uint32_t EnemyActor_UpdateStatus = 0x74B441;
constexpr uint32_t IStatusProtocol_elapseTurnStatus = 0x4F1E01;
constexpr uint32_t IBuffProtocol_elapseTurnAttackBuff = 0x4F211D;
constexpr uint32_t IBuffProtocol_elapseTurnDefenseBuff = 0x4F219D;
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;
constexpr uint32_t MarkFader_create = 0x5D9931;            // (const std::string& image)
constexpr uint32_t MarkFader_fadeIn = 0x5D9B49;            // (fader, std::function<void()>)
constexpr uint32_t StringBuffer_ctor = 0x49140D;           // StringBuffer(buf, fmt, ...)
constexpr uint32_t strlen_ = 0x40E08C;
constexpr uint32_t string_init = 0x47CD93;
constexpr uint32_t Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Vec2_ZERO = 0x1006950;                  // cocos2d::Vec2::ZERO
constexpr uint32_t function_vv_assign = 0x4EB9CD;          // std::function<void()>::operator=(const&)
constexpr uint32_t function_b_call = 0x51022D;             // std::function<bool()>::operator()
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t EventDispatcher_resumeEventListenersForTarget = rwlib::cocos2d::EventDispatcher::resumeEventListenersForTarget;
constexpr uint32_t SpeechMap_at = 0x601971;                // unordered_map<SpeechDisp*, deque<string>>::operator[]
constexpr uint32_t SpeechDeque_pop_front = 0x601DC1;
constexpr uint32_t WIPE_FORMAT = 0xE02E61;                 // "img/wipe/wipe_%d.png"
constexpr uint32_t BGM_STAGE_CLEAR = 0x1DDCB6F1;           // audio::ResourceID of the stage clear jingle
}  // namespace g

constexpr uint32_t VT_stageFinishNotification_31 = 0xF5E59C;  // stage finished (normal / colosseum)
constexpr uint32_t VT_stageFinishNotification_30 = 0xF5E5C8;  // stage finished after a colosseum flag
constexpr uint32_t VT_toLoadingModeSetup_40 = 0xF5E38C;       // the wipe faded in: (self, fader)
constexpr uint32_t VT_executingTutorial1006_49 = 0xF5E1A8;
constexpr uint32_t VT_executingTutorial1010_50 = 0xF5E17C;

// one 4-byte field of the stage state (AppState +0x20, a SafeBuffer)
uint32_t stageStateWord(uint32_t offset) {
    Frame f;
    uint32_t out = f.alloc(8);
    uint32_t state = ld32(call(g::AppState_getInstance, {}) + 0x20);
    call(g::SafeBuffer_read, {state, out, ld32(state + 0xC) + offset, 4, offset});
    return ld32(out);
}

// the battle input state of SceneBattleBase, reset when the scene changes mode
void resetBattleInput(SceneActionMap *self) {
    rw::set<uint32_t>(self, 0x338, 0);         // battle step
    rw::set<uint8_t>(self, 0x2DD, 0);
    rw::set<uint8_t>(self, 0x2DE, 0);
    rw::set<float>(self, 0x2D8, 0.5f);
    rw::set<uint8_t>(self, 0x2DF, 0);
    rw::set<uint8_t>(self, 0x2DC, 1);
    self->touchState = 0;
}

}  // namespace

// Revive after a continue: back to the battle (or field) deck state, add the continue count (state +0x424C) to
// the total (+0x4250), heal by `hp`, refresh the HP gauge and drop the enemies' pending attacks.
void SceneActionMap::revivePlayer(SceneActionMap *self, int32_t hp) {
    uint32_t avatar = ld32(self->actorManager + 0x20);
    call(g::StageActorManager_attackReadyAction, {self->actorManager, 0});
    uint32_t deck = call(g::StageUI_getMedalField, {self->stageUI});
    uint32_t battle = rw::load<int32_t>(self->stageManager + 0x18) == 1 ? 1 : 0;
    call(g::MedalDeck_setBattleState, {deck, battle});
    if (call(g::CarouselDeck_reflashDeck, {call(g::StageUI_getCarouselDeck, {self->stageUI})}) == 1) {
        uint32_t state = ld32(call(g::AppState_getInstance, {}) + 0x20);
        Frame f;
        uint32_t sum = f.alloc(4);
        uint32_t total = stageStateWord(0x4250);
        uint32_t count = stageStateWord(0x424C);
        st32(sum, count + total);
        call(g::SafeBuffer_write, {state, ld32(state + 0xC) + 0x4250, sum, 4, 0x4250});
    }
    st8(avatar + 0x295, 0);
    call(g::StageState_addDamage, {ld32(call(g::AppState_getInstance, {}) + 0x20), 0u - uint32_t(hp)});
    uint32_t status = call(g::StageUI_getPlayerStatusField, {self->stageUI});
    call(g::PlayerStatusIndicator_setHP, {status, stageStateWord(8)});
    call(g::StageBattleManager_clearEnemyAttker, {self->battleManager});
    self->menuAllowed = 1;
}

// The stage is over: the avatar's win motion (unless the last team info says otherwise), the clear BGM, then the
// finish direction (colosseum or normal) which calls back into the scene.
void SceneActionMap::stageFinishNotification(SceneActionMap *self) {
    uint32_t a = guest_addr(self);
    uint32_t avatar = ld32(self->actorManager + 0x20);
    uint32_t info = call(g::StageActorManager_getLastEncountTeamInfo, {self->actorManager, self->stageManager});
    if ((info & 0x10) == 0) call(g::Avatar_changeMotion, {ld32(avatar + 0x288), 0x28});
    if ((info & 2) == 0) call(g::AudioReplaceGameMainBGM, {g::BGM_STAGE_CLEAR, 500});
    Frame f;
    uint32_t flag = f.alloc(4);
    uint32_t state = ld32(call(g::AppState_getInstance, {}) + 0x20);
    call(g::SafeBuffer_read, {state, flag, ld32(state + 0xC) + 1, 1, 1});
    uint32_t action;
    bool colosseum;
    if (ld8(flag) == 1) {
        uint32_t fn = newFunction(f, VT_stageFinishNotification_30, {a});
        action = call(g::CallFunc_create, {fn});
        functionDestroy(fn);
        colosseum = true;
    } else {
        uint32_t fn = newFunction(f, VT_stageFinishNotification_31, {a});
        action = call(g::CallFunc_create, {fn});
        functionDestroy(fn);
        colosseum = ld32(self->stageManager + 0x14) != 0;
    }
    if (colosseum) call(g::StageDirection_cocos_clear_colosseum, {self->stageDirection, action});
    else call(g::StageDirection_cocos_finish, {self->stageDirection, action});
    rw::set<int32_t>(self, 0x338, 0x20);
}

// Where the aim marker points (map coordinates): the touch point while aiming (AimTarget +0x25), else the marker's
// current position; the marker is shown there.
void SceneActionMap::updateAimTarget(uint32_t out, SceneActionMap *self) {
    call(g::Vec2_copy, {out, g::Vec2_ZERO});
    uint32_t aim = self->aimTarget;
    if (ld8(aim + 0x25)) {
        Frame f;
        uint32_t p = f.alloc(8);
        call(g::StageFieldManager_getScreenPosToMapPos, {p, self->fieldManager, aim + 0x1C});
        st32(out, ld32(p));
        st32(out + 4, ld32(p + 4));
        call(g::Vec2_dtor, {p});
        uint32_t x = ld32(out), y = ld32(out + 4);
        uint32_t marker = ld32(self->aimTarget + 0x34);
        st32(marker + 0x274, y);
        st32(marker + 0x270, x);
        call(g::AimMarker_show, {ld32(self->aimTarget + 0x34)});
    } else {
        uint32_t marker = ld32(aim + 0x34);
        uint32_t x = ld32(marker + 0x270), y = ld32(marker + 0x274);
        st32(out, x);
        st32(out + 4, y);
        call(g::AimMarker_show, {ld32(aim + 0x34)});
    }
}

// Moving to another map: the world's wipe image fades in over everything, then (lambda $_40) the next map loads.
void SceneActionMap::toLoadingModeSetup(SceneActionMap *self) {
    uint32_t a = guest_addr(self);
    self->titleShown = 0;
    uint32_t world = call(g::StageManager_getWorldCode, {self->stageManager});
    guest::Frame frame(0x444);
    uint32_t buf = frame.addr(0x38);
    call(g::StringBuffer_ctor, {buf, g::WIPE_FORMAT, world});
    uint32_t len = call(g::strlen_, {buf});
    uint32_t name = frame.addr(0x14);
    call(g::string_init, {name, buf, len});
    uint32_t fader = call(g::MarkFader_create, {name});
    strFree(name);
    uint32_t fn = frame.addr(0x20);
    st32(fn, VT_toLoadingModeSetup_40);
    st32(fn + 4, a);
    st32(fn + 8, fader);
    st32(fn + 0x10, fn);
    call(g::MarkFader_fadeIn, {fader, fn});
    functionDestroy(fn);
    vcall(a, node::addChildZ, {a, fader, 0x7FFFFFFF});
    st32(self->stageManager + 0x18, 0);
}

// An encounter starts a battle (mode 1): remember where the avatar stood, battle stance, battle decks.
void SceneActionMap::toBattleModeSetup(SceneActionMap *self) {
    st32(self->stageManager + 0x18, 1);
    uint32_t actors = self->actorManager;
    uint32_t avatar = ld32(actors + 0x20);
    uint32_t x = ld32(avatar + 0x270);
    rw::set<uint32_t>(self, 0x35C, ld32(avatar + 0x274));
    rw::set<uint32_t>(self, 0x358, x);
    call(g::Avatar_changeMotion, {ld32(ld32(actors + 0x20) + 0x288), 11});
    call(g::MedalDeck_setBattleState, {call(g::StageUI_getMedalField, {self->stageUI}), 1});
    call(g::CarouselDeck_setBattleState, {call(g::StageUI_getCarouselDeck, {self->stageUI}), 1});
    call(g::AimTarget_reset, {self->aimTarget});
    call(g::StageActorManager_clearCurrentTarget, {self->actorManager});
    resetBattleInput(self);
    call(g::StageManager_changeMode, {self->stageManager, 1});
    call(g::StageActorManager_changeMode, {self->actorManager, 1});
    call(g::StageFieldManager_changeMode, {self->fieldManager, 1});
}

// a talk line was consumed: the stage's current talk, and the next queued line of the talk window
void SceneActionMap::subTalkText(SceneActionMap *self, uint32_t talk) {
    uint32_t a = guest_addr(self);
    st32(self->stageManager + 0x1C, talk);
    uint32_t queue = call(g::SpeechMap_at, {a + 0x37C, a + 0x378});
    if (ld32(queue + 0x14)) call(g::SpeechDeque_pop_front, {queue});
}

// The interlude (mode 3) between maps: the field state without the BGM change, then a 3 second wait (quests;
// raids only when the pending check says so) with the deck disabled.
void SceneActionMap::toIntrludeModeSetup(SceneActionMap *self) {
    call(g::AvatarActor_ClearBuffDebuff, {ld32(self->actorManager + 0x20)});
    toFieldModeSetup(self, 1);
    st32(self->stageManager + 0x18, 3);
    call(g::StageDirection_to_end, {self->stageDirection});
    self->field_360 = 0;
    uint32_t stage = self->stageManager;
    rw::set<float>(self, 0x360, ld32(stage + 0x14) == 0 ? 3.0f : 0.0f);
    if (ld32(stage + 0x14) == 1 && call(g::function_b_call, {guest_addr(self) + 0x340}) == 1)
        rw::set<float>(self, 0x360, 3.0f);
    call(g::CarouselDeck_disableButton, {call(g::StageUI_getCarouselDeck, {self->stageUI})});
    self->menuAllowed = 0;
}

// a burst was chosen: battle step 5, or 8 when the player's attack is skipped (status effect)
uint32_t SceneActionMap::isExecutionBurst(SceneActionMap *self) {
    rw::set<int32_t>(self, 0x338, 5);
    if (call(g::BaseActorManager_isPlayerAttackSkipStatus, {self->actorManager, 1}) == 1) {
        rw::set<int32_t>(self, 0x338, 8);
        return 0;
    }
    return 1;
}

void SceneActionMap::onSwipeField(SceneActionMap *, uint32_t, uint32_t, uint32_t) {}

// lock / unlock the battle UI: the menu flag and the carousel deck's buttons (deck +0x2BC)
void SceneActionMap::lockUI(SceneActionMap *self, uint32_t lock) {
    self->menuAllowed = uint8_t(lock ^ 1);
    uint32_t deck = call(g::StageUI_getCarouselDeck, {self->stageUI});
    if (deck == 0) return;
    uint32_t buttons = ld32(deck + 0x2BC);
    if (buttons) vcall(buttons, 0x29C, {buttons, self->menuAllowed});
}

// tutorial stage 1006: once the event treasure is reached, drop the tutorial widget and hook the explore manager's
// callback (lambda $_49)
void SceneActionMap::executingTutorial1006(SceneActionMap *self) {
    if (call(g::StageFieldManager_getCurrentEventMapTreasure, {self->fieldManager}) == 0) return;
    call(g::StageUI_deleteTutorialWidget, {self->stageUI});
    Frame f;
    uint32_t explore = self->exploreManager;
    uint32_t fn = newFunction(f, VT_executingTutorial1006_49, {guest_addr(self)});
    call(g::function_vv_assign, {explore + 0x28, fn});
    functionDestroy(fn);
}

// tutorial stage 1010: when the prizes are done and the avatar holds none, tutorial popup 43, unlock the input and
// clear the explore manager's callback
void SceneActionMap::executingTutorial1010(SceneActionMap *self) {
    if (call(g::StageFieldManager_isProcessedPrizes, {self->fieldManager})) return;
    if (call(g::AvatarActor_hasPrize, {ld32(self->actorManager + 0x20)})) return;
    uint32_t a = guest_addr(self);
    {
        Frame f;
        uint32_t popup = f.alloc(0x18);
        uint32_t fn = newFunction(f, VT_executingTutorial1010_50, {a});
        call(g::SceneUtil_openTutorialPopup, {popup, 0x2B, fn, 0, 0, 0, 0});
        functionDestroy(popup);
        functionDestroy(fn);
    }
    uint32_t dispatcher = ld32(call(fn::Director_getInstance, {}) + 0x84);
    call(g::EventDispatcher_resumeEventListenersForTarget, {dispatcher, a, 0});
    lockUI(self, 0);
    Frame f;
    uint32_t explore = self->exploreManager;
    uint32_t empty = newEmptyFunction(f);
    call(g::function_vv_assign, {explore + 0x28, empty});
    functionDestroy(empty);
}

// enemy turn ticks: status and attack buffs (attack turn) / defence buffs (defence turn) of every enemy
void SceneActionMap::enemy_state_update_turn_attack(SceneActionMap *self) {
    uint32_t actors = self->actorManager;
    uint32_t it = ld32(actors + 0x4C), end = ld32(actors + 0x50);
    for (; it != end; it += 4) {
        call(g::IStatusProtocol_elapseTurnStatus, {ld32(it) + 0x2C8});
        call(g::IBuffProtocol_elapseTurnAttackBuff, {ld32(it) + 0x2CC});
        call(g::EnemyActor_UpdateStatus, {ld32(it)});
    }
}

void SceneActionMap::enemy_state_update_turn_defense(SceneActionMap *self) {
    uint32_t actors = self->actorManager;
    uint32_t it = ld32(actors + 0x4C), end = ld32(actors + 0x50);
    for (; it != end; it += 4) {
        call(g::IBuffProtocol_elapseTurnDefenseBuff, {ld32(it) + 0x2CC});
        call(g::EnemyActor_UpdateStatus, {ld32(it)});
    }
}

void SceneActionMap::req_skilPanel_in(SceneActionMap *self, int32_t skill) {
    call(g::CommonDirection_req_skilPanel_in, {self->commonDirection, uint32_t(skill)});
}

void SceneActionMap::req_materialPanel_in(SceneActionMap *self, int32_t material) {
    call(g::CommonDirection_req_materialPanel_in, {self->commonDirection, uint32_t(material)});
}

RW_FUNCTION(0x600DA4, "SceneActionMap::revivePlayer", SceneActionMap::revivePlayer);
RW_FUNCTION(0x600EF0, "SceneActionMap::stageFinishNotification", SceneActionMap::stageFinishNotification);
RW_FUNCTION(0x601074, "SceneActionMap::updateAimTarget", SceneActionMap::updateAimTarget);
RW_FUNCTION(0x601140, "SceneActionMap::toLoadingModeSetup", SceneActionMap::toLoadingModeSetup);
RW_FUNCTION(0x601290, "SceneActionMap::toBattleModeSetup", SceneActionMap::toBattleModeSetup);
RW_FUNCTION(0x601BF4, "SceneActionMap::subTalkText", SceneActionMap::subTalkText);
RW_FUNCTION(0x601E3C, "SceneActionMap::toIntrludeModeSetup", SceneActionMap::toIntrludeModeSetup);
RW_FUNCTION(0x601EBC, "SceneActionMap::isExecutionBurst", SceneActionMap::isExecutionBurst);
RW_FUNCTION(0x601EE8, "SceneActionMap::onSwipeField", SceneActionMap::onSwipeField);
RW_FUNCTION(0x6020CC, "SceneActionMap::lockUI", SceneActionMap::lockUI);
RW_FUNCTION(0x602104, "SceneActionMap::executingTutorial1006", SceneActionMap::executingTutorial1006);
RW_FUNCTION(0x6021C0, "SceneActionMap::executingTutorial1010", SceneActionMap::executingTutorial1010);
RW_FUNCTION(0x6022F8, "SceneActionMap::enemy_state_update_turn_attack", SceneActionMap::enemy_state_update_turn_attack);
RW_FUNCTION(0x60232E, "SceneActionMap::enemy_state_update_turn_defense", SceneActionMap::enemy_state_update_turn_defense);
RW_FUNCTION(0x602358, "SceneActionMap::req_skilPanel_in", SceneActionMap::req_skilPanel_in);
RW_FUNCTION(0x602366, "SceneActionMap::req_materialPanel_in", SceneActionMap::req_materialPanel_in);
