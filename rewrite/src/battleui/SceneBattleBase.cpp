// SceneBattleBase: construction, the battle input state, medal skill invocation, HP / CP and the turn callbacks.
#include "SceneBattleBase.hpp"
#include "StageUI.hpp"
#include "StageUI_Thunks.hpp"

using namespace scene;

namespace {
// cocos2d
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;
constexpr uint32_t Layer_dtor = rwlib::cocos2d::Layer::dtor_base;
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t RefVector_pushBack = 0x5E29A9;        // cocos2d::Vector<Ref*>::pushBack
constexpr uint32_t RefVector_clear = 0x5E44F1;
constexpr uint32_t RefVector_dtor = 0x5E2971;
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;         // std::function<void()>::operator=(const function&)
// game
constexpr uint32_t GestureRecognizer_ctor = 0x4F348D;    // (recognizer, cocos2d::Node* owner)
constexpr uint32_t GestureRecognizer_dtor = 0x4F3621;
constexpr uint32_t RaidBossHPIndicator_firstCall = 0x5E4F81;   // static
constexpr uint32_t RaidBossHPIndicator_finalCall = 0x5E50B9;   // static
constexpr uint32_t SceneManager_removeBackground = 0x611691;
constexpr uint32_t SceneManager_isOpenedPopup = 0x621519;
constexpr uint32_t StageActorManager_create = 0x5FA1C3;
constexpr uint32_t StageActorManager_pauseToAllActor = 0x752C8D;
constexpr uint32_t StageActorManager_resumeToAllActor = 0x752CD1;
constexpr uint32_t StageFieldManager_create = 0x5FA261;
constexpr uint32_t StageUI_openBattleMenuPopup = 0x760815;      // (ui, PlayStyle, const function<void(bool)>&)
constexpr uint32_t SceneUtil_createTopInAnimation = 0x45B03D;   // (node, float, float, function<void()> by value)
constexpr uint32_t SceneUtil_createRightInAnimation = 0x45B8BD;
constexpr uint32_t CarouselDeck_admission = 0x5D0875;
constexpr uint32_t CarouselDeck_setPossibleBurst = 0x5D0C11;   // (deck, bool, bool)
constexpr uint32_t MedalDeck_forEachMedal = 0x5E007D;          // (deck, const function<bool(const MedalInfo&)>&)
constexpr uint32_t MedalDeck_isValidCurrentBurst = 0x5E04E5;
constexpr uint32_t MedalDeck_getCurrentBurstGauge = 0x5E0565;
constexpr uint32_t PlayerStatusIndicator_setHP = 0x5E3119;
constexpr uint32_t PlayerStatusIndicator_setCP = 0x5E34D9;
constexpr uint32_t PlayerStatusIndicator_playEffectCostCancel = 0x5E3C65;
constexpr uint32_t StageState_addDamage = 0x4EEB89;
constexpr uint32_t StageState_addCP = 0x4EEC29;
constexpr uint32_t StageState_setCpMax = 0x4EF859;
constexpr uint32_t skill_invoke = 0x748EE9;     // stage::skill::invoke(sret shared_ptr, uint64 id, timing, bool, bool)
constexpr uint32_t IStatusProtocol_elapseTurnStatus = 0x4F1E01;
constexpr uint32_t IStatusProtocol_elapseCountStatus = 0x4F1F49;
constexpr uint32_t IBuffProtocol_elapseTurnAttackBuff = 0x4F211D;
constexpr uint32_t IBuffProtocol_elapseTurnDefenseBuff = 0x4F219D;
constexpr uint32_t IBuffProtocol_elapseCountAttackBuff = 0x4F221D;
constexpr uint32_t IBuffProtocol_elapseCountDefenseBuff = 0x4F229D;
constexpr uint32_t AvatarActor_UpdateStatus = 0x7403A9;
constexpr uint32_t EnemyActor_UpdateStatus = 0x74B441;
constexpr uint32_t RaidBossActor_UpdateStatus = 0x5F6BD9;

// lambda vtables (__func + 8)
constexpr uint32_t VT_InitUI0 = 0xF5E150;       // init_UI()::$_0  medal deck callback at +888
constexpr uint32_t VT_InitUI1 = 0xF5E124;       // init_UI()::$_1  +912
constexpr uint32_t VT_InitUI2 = 0xF5E0F8;       // init_UI()::$_2  +936
constexpr uint32_t VT_MenuClosed = 0xF5E0CC;    // onMenuButton(PlayStyle)::$_3
constexpr uint32_t VT_InvokeSkill = 0xF5E0A0;   // invokeSkill(InvokeShowTiming)::$_4

// MedalDeck fields
constexpr uint32_t DECK_ON_COUNT_TURN = 888;    // std::function<void()> callbacks the deck calls each turn
constexpr uint32_t DECK_ON_ATTACK_TURN = 912;
constexpr uint32_t DECK_ON_DEFENSE_TURN = 936;
constexpr uint32_t DECK_COUNT_A = 864;          // medal count = +864 + +868
constexpr uint32_t DECK_COUNT_B = 868;
constexpr uint32_t DECK_MEDALS = 632;           // MedalInfo[]: 40 bytes each, a SafeBuffer at +0 of the entry
constexpr uint32_t ACTOR_MANAGER_MEDAL_COUNT = 304;
constexpr uint32_t ACTOR_MANAGER_FIELD_MANAGER = 116;
constexpr uint32_t ACTOR_MANAGER_MEDAL_DECK = 40;
constexpr uint32_t ACTOR_MANAGER_AVATAR = 32;

// the avatar's state machine: Avatar +648 -> the motion owner whose state at +1316 is 47 while it cannot be touched
constexpr uint32_t AVATAR_FLASH = 648;
constexpr uint32_t FLASH_STATE = 1316;
constexpr uint32_t STATE_NO_TOUCH = 47;

// stage state data positions
constexpr uint32_t SS_SKILL_ID = 0x4234;        // last attack skill invoked
constexpr uint32_t SS_DAMAGE_RATE = 0x4238;     // lowest damage rate of the deck's damage skills (10000 = 100%)
constexpr uint32_t SS_TURN_RATE_ADD = 0x424C;
constexpr uint32_t SS_TURN_RATE = 0x4250;
constexpr uint32_t SS_SKILL_VALUE = 0x4244;

constexpr float TOUCH_TIME = 0.5f;

uint32_t stageState() { return ld32(call(fn::AppState_getInstance, {}) + 32); }
SafeBuffer *stageBuffer() { return rw::host_ptr<SafeBuffer>(stageState()); }

// MedalInfo: a SafeBuffer at +8 whose data holds the skill id (64 bits) at 8
uint64_t medalSkillId(uint32_t medal) {
    const SafeBuffer *sb = safe(medal + 8);
    return uint32_t(sb->get32(8)) | (uint64_t(uint32_t(sb->get32(12))) << 32);
}

// stage::skill::invoke into the shared_ptr at `out`; r1 is unused by the callee (the 64-bit id is in r2:r3)
void invoke(uint32_t out, uint32_t r1, uint64_t id, uint32_t timing, uint32_t flag1, uint32_t flag2) {
    call(skill_invoke, {out, r1, uint32_t(id), uint32_t(id >> 32), timing, flag1, flag2});
}

uint32_t avatarOf(const SceneBattleBase *self) { return ld32(self->actorManager + ACTOR_MANAGER_AVATAR); }

// a touch on the field begins (unless the avatar is in its untouchable state)
void beginTouch(SceneBattleBase *self) {
    if (ld32(ld32(avatarOf(self) + AVATAR_FLASH) + FLASH_STATE) == STATE_NO_TOUCH) return;
    self->tapped = 0;
    self->swiped = 0;
    self->touchTime = TOUCH_TIME;
    self->touchState = 1;
}

// a touch ends: it was a tap when it was still the plain touch begun on the field
void endTouch(SceneBattleBase *self) {
    uint8_t tap = 0;
    if (!self->touchHeld) tap = self->touchState == 1 ? 1 : 0;
    self->tapped = tap;
    self->touchHeld = 0;
    self->touchIdle = 1;
    self->touchState = 0;
}

void cancelTouch(SceneBattleBase *self) {
    self->tapped = 0;
    self->swiped = 0;
    self->touchTime = TOUCH_TIME;
    self->touchHeld = 0;
    self->touchIdle = 1;
    self->touchState = 0;
}

// run a top / right slide-in animation with an empty callback on a HUD part
void slideIn(uint32_t node, uint32_t animation) {
    vcall(node, node::setVisible, {node, 1});
    Frame f;
    uint32_t done = newEmptyFunction(f);
    uint32_t action = call(animation, {node, 0, 0, done});
    call(Node_runAction, {node, call(EaseSineOut_create, {action})});
    functionDestroy(done);
}

// deck->callback = [scene] {...}
void setDeckCallback(uint32_t deck, uint32_t offset, uint32_t vtable, uint32_t scene) {
    Frame f;
    uint32_t cb = newFunction(f, vtable, {scene});
    call(FunctionVV_assign, {deck + offset, cb});
    functionDestroy(cb);
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
SceneBattleBase *SceneBattleBase::construct(SceneBattleBase *self) {
    uint32_t a = rw::guest_addr(self);
    call(Layer_ctor, {a});
    self->vtable = VTABLE;
    call(GestureRecognizer_ctor, {a + 648, a});
    self->managers[0] = 0;
    self->managers[1] = 0;
    self->managers[2] = 0;
    self->menuOpen = 0;
    self->menuAllowed = 1;
    self->touchState = 0;
    rw::set<uint32_t>(self, 728, 0);          // touchTime = 0.0f
    self->touchHeld = 0;
    self->swiped = 0;
    self->tapped = 0;
    self->touchIdle = 1;
    call(RaidBossHPIndicator_firstCall, {708});   // static; r0 still holds the field offset
    return self;
}
RW_FUNCTION(0x5F9FF8, "SceneBattleBase::SceneBattleBase", SceneBattleBase::construct);

SceneBattleBase *SceneBattleBase::destroy(SceneBattleBase *self) {
    uint32_t a = rw::guest_addr(self);
    self->vtable = VTABLE;
    uint32_t r0 = call(RefVector_clear, {a + 708});
    call(RaidBossHPIndicator_finalCall, {r0});    // static
    call(RefVector_dtor, {a + 708});
    call(GestureRecognizer_dtor, {a + 648});
    call(Layer_dtor, {a});
    return self;
}
RW_FUNCTION(0x5FA0B4, "SceneBattleBase::~SceneBattleBase", SceneBattleBase::destroy);

void SceneBattleBase::destroyDelete(SceneBattleBase *self) {
    destroy(self);
    call(fn::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x5FA128, "SceneBattleBase::~SceneBattleBase (deleting)", SceneBattleBase::destroyDelete);

// Create the stage managers (kept alive by the managers vector), link them and create the medal deck.
uint32_t SceneBattleBase::init(SceneBattleBase *self) {
    uint32_t a = rw::guest_addr(self);
    if (call(Layer_init, {a}) != 1) return 0;
    self->ready = 1;
    call(SceneManager_removeBackground, {sceneManager()});
    self->actorManager = call(StageActorManager_create, {});
    self->stageUI = rw::guest_addr(StageUI::create());
    self->fieldManager = call(StageFieldManager_create, {});
    call(RefVector_pushBack, {a + 708, self->actorManager});
    call(RefVector_pushBack, {a + 708, self->stageUI});
    call(RefVector_pushBack, {a + 708, self->fieldManager});
    rw::save<uint32_t>(self->actorManager + ACTOR_MANAGER_FIELD_MANAGER, self->fieldManager);
    StageUI::createMedalField(rw::host_ptr<StageUI>(self->stageUI));
    uint32_t deck = StageUI::getMedalField(rw::host_ptr<StageUI>(self->stageUI));
    rw::save<uint32_t>(self->actorManager + ACTOR_MANAGER_MEDAL_DECK, deck);
    return 1;
}
RW_FUNCTION(0x5FA134, "SceneBattleBase::init", SceneBattleBase::init);

// Show the HUD (status field, attribute chart, player status slide in), let the carousel deck enter, hook the
// medal deck's turn callbacks and start the player status from the stage state.
uint32_t SceneBattleBase::init_UI(SceneBattleBase *self) {
    uint32_t a = rw::guest_addr(self);
    StageUI *ui = rw::host_ptr<StageUI>(self->stageUI);
    slideIn(StageUI::getStatusField(ui), SceneUtil_createTopInAnimation);
    ui = rw::host_ptr<StageUI>(self->stageUI);
    slideIn(StageUI::getAttributeField(ui), SceneUtil_createRightInAnimation);
    ui = rw::host_ptr<StageUI>(self->stageUI);
    slideIn(StageUI::getPlayerStatusField(ui), SceneUtil_createTopInAnimation);
    ui = rw::host_ptr<StageUI>(self->stageUI);
    call(CarouselDeck_admission, {StageUI::getCarouselDeck(ui)});
    ui = rw::host_ptr<StageUI>(self->stageUI);
    uint32_t deck = StageUI::getMedalField(ui);
    setDeckCallback(deck, DECK_ON_COUNT_TURN, VT_InitUI0, a);
    setDeckCallback(deck, DECK_ON_ATTACK_TURN, VT_InitUI1, a);
    setDeckCallback(deck, DECK_ON_DEFENSE_TURN, VT_InitUI2, a);
    uint32_t count = ld32(deck + DECK_COUNT_B) + ld32(deck + DECK_COUNT_A);
    rw::save<uint32_t>(self->actorManager + ACTOR_MANAGER_MEDAL_COUNT, count);
    StageUI::set_start_avatar_parameter(rw::host_ptr<StageUI>(self->stageUI));
    return 0;
}
RW_FUNCTION(0x5FA2B0, "SceneBattleBase::init_UI", SceneBattleBase::init_UI);

// ------------------------------------------------------------------------------------------- input
// TouchEventType: 0 began, 2 ended, 3 cancelled.
SceneBattleBase *SceneBattleBase::onAttackButton(SceneBattleBase *self, int32_t type) {
    if (type == 3) cancelTouch(self);
    else if (type == 2) endTouch(self);
    else if (type == 0) beginTouch(self);
    return self;
}
RW_FUNCTION(0x5FA580, "SceneBattleBase::onAttackButton", SceneBattleBase::onAttackButton);

uint32_t SceneBattleBase::onTouchBeganBattle(SceneBattleBase *self, uint32_t, uint32_t) {
    beginTouch(self);
    return 1;
}
RW_FUNCTION(0x5FA630, "SceneBattleBase::onTouchBeganBattle", SceneBattleBase::onTouchBeganBattle);

void SceneBattleBase::onTouchMovedBattle(SceneBattleBase *, uint32_t, uint32_t) {}
RW_FUNCTION(0x5FA674, "SceneBattleBase::onTouchMovedBattle", SceneBattleBase::onTouchMovedBattle);

SceneBattleBase *SceneBattleBase::onTouchEndedBattle(SceneBattleBase *self, uint32_t, uint32_t) {
    endTouch(self);
    return self;
}
RW_FUNCTION(0x5FA678, "SceneBattleBase::onTouchEndedBattle", SceneBattleBase::onTouchEndedBattle);

SceneBattleBase *SceneBattleBase::onTouchCancelledBattle(SceneBattleBase *self, uint32_t, uint32_t) {
    cancelTouch(self);
    return self;
}
RW_FUNCTION(0x5FA6B8, "SceneBattleBase::onTouchCancelledBattle", SceneBattleBase::onTouchCancelledBattle);

// A swipe turns the touch begun on the field into a swipe. Returns 0 then, else the scene (r0 untouched).
uint32_t SceneBattleBase::onSwipeBattle(SceneBattleBase *self, uint32_t, uint32_t, uint32_t) {
    if (self->touchState != 1) return rw::guest_addr(self);
    self->swiped = 1;
    self->touchState = 0;
    return 0;
}
RW_FUNCTION(0x5FA6EC, "SceneBattleBase::onSwipeBattle", SceneBattleBase::onSwipeBattle);

// ------------------------------------------------------------------------------------------- turn ticks
uint32_t SceneBattleBase::enemy_count_end_enemy(SceneBattleBase *self, uint32_t enemy) {
    call(IBuffProtocol_elapseCountAttackBuff, {enemy + 716});
    call(EnemyActor_UpdateStatus, {enemy});
    call(IBuffProtocol_elapseCountDefenseBuff, {stageState() + 36});
    return call(AvatarActor_UpdateStatus, {avatarOf(self)});
}
RW_FUNCTION(0x5FA708, "SceneBattleBase::enemy_count_end(EnemyActor*)", SceneBattleBase::enemy_count_end_enemy);

uint32_t SceneBattleBase::enemy_count_end_raidBoss(SceneBattleBase *self, uint32_t boss) {
    call(IBuffProtocol_elapseCountAttackBuff, {boss + 684});
    call(RaidBossActor_UpdateStatus, {boss});
    call(IBuffProtocol_elapseCountDefenseBuff, {stageState() + 36});
    return call(AvatarActor_UpdateStatus, {avatarOf(self)});
}
RW_FUNCTION(0x5FA73E, "SceneBattleBase::enemy_count_end(RaidBossActor*)",
            SceneBattleBase::enemy_count_end_raidBoss);

// The menu button: when no popup is open and the scene is ready, pause every actor and open the battle menu;
// closing it resumes them ($_3).
uint32_t SceneBattleBase::onMenuButton(SceneBattleBase *self, uint32_t playStyle) {
    if (call(SceneManager_isOpenedPopup, {sceneManager()}) != 0) return 0;
    if (!self->ready || !self->menuAllowed || self->menuOpen) return 0;
    self->menuOpen = 1;
    call(StageActorManager_pauseToAllActor, {self->actorManager});
    Frame f;
    uint32_t onClose = newFunction(f, VT_MenuClosed, {rw::guest_addr(self)});
    call(StageUI_openBattleMenuPopup, {self->stageUI, playStyle, onClose});
    functionDestroy(onClose);
    return 0;
}
RW_FUNCTION(0x5FA774, "SceneBattleBase::onMenuButton", SceneBattleBase::onMenuButton);

// ------------------------------------------------------------------------------------------- skills
// AttackTarget: +0 kind (2 / 3 select the invoke flags), +4 MedalInfo*, +8 shared_ptr<skill> (result).
// Invoke the attacking medal's skill; the skill id is remembered in the stage state.
uint32_t SceneBattleBase::invokeAttackSkill(SceneBattleBase *self, uint32_t target) {
    uint32_t skillId = 0;
    uint32_t medal = ld32(target + 4);
    if (medal) {
        uint64_t id = medalSkillId(medal);
        int32_t kind = int32_t(ld32(target));
        Frame f;
        uint32_t out = f.alloc(8);
        invoke(out, kind == 2, id, 1, kind == 2, kind == 3);
        uint32_t skill = ld32(out);
        if (skill) {
            skillId = uint32_t(safe32(skill, 0));
            vcall(rw::guest_addr(self), VSLOT_ON_SKILL, {rw::guest_addr(self), skillId});
        }
        // target->skill = result (copy assignment)
        uint32_t ctrl = ld32(out + 4);
        if (ctrl) call(fn::shared_add, {ctrl});
        rw::save<uint32_t>(target + 8, skill);
        uint32_t old = ld32(target + 12);
        rw::save<uint32_t>(target + 12, ctrl);
        if (old) call(fn::shared_release, {old});
        sharedRelease(out);
    }
    stageBuffer()->set32(SS_SKILL_ID, int32_t(skillId));
    return 0;
}
RW_FUNCTION(0x5FA858, "SceneBattleBase::invokeAttackSkill", SceneBattleBase::invokeAttackSkill);

// Invoke every medal's skills for a timing ($_4); timing 4 first clears the skill value.
uint32_t SceneBattleBase::invokeSkill(SceneBattleBase *self, int32_t timing) {
    if (timing == 4) stageBuffer()->set32(SS_SKILL_VALUE, 0);
    uint32_t deck = StageUI::getMedalField(rw::host_ptr<StageUI>(self->stageUI));
    Frame f;
    uint32_t each = newFunction(f, VT_InvokeSkill, {uint32_t(timing)});
    call(MedalDeck_forEachMedal, {deck, each});
    functionDestroy(each);
    return 0;
}
RW_FUNCTION(0x5FA974, "SceneBattleBase::invokeSkill", SceneBattleBase::invokeSkill);

// The deck's damage skills (timing 2): the lowest damage rate (skill +252, from 10000) goes to the stage state;
// returns the id of the skill that set it (0 when none is below 10000).
int32_t SceneBattleBase::DamageSkill(SceneBattleBase *self) {
    int32_t rate = 10000;
    int32_t best = 0;
    uint32_t deck = StageUI::getMedalField(rw::host_ptr<StageUI>(self->stageUI));
    int32_t count = int32_t(ld32(deck + DECK_COUNT_B) + ld32(deck + DECK_COUNT_A));
    for (int32_t k = 0; k < count; k++) {
        uint32_t entry = StageUI::getMedalField(rw::host_ptr<StageUI>(self->stageUI)) + DECK_MEDALS + 40u * k;
        const SafeBuffer *sb = safe(entry);
        uint64_t id = uint32_t(sb->get32(8)) | (uint64_t(uint32_t(sb->get32(12))) << 32);
        Frame f;
        uint32_t out = f.alloc(8);
        invoke(out, 0, id, 2, 0, 0);
        uint32_t skill = ld32(out);
        if (skill) {
            int32_t skillId = safe32(skill, 0);
            int32_t r = safe32(skill, 252);
            if (r < rate) {
                rate = r;
                best = skillId;
            }
        }
        sharedRelease(out);
    }
    stageBuffer()->set32(SS_DAMAGE_RATE, rate);
    return best;
}
RW_FUNCTION(0x5FAA40, "SceneBattleBase::DamageSkill", SceneBattleBase::DamageSkill);

// Invoke one medal's skill (sret shared_ptr); timing 1 records the skill id in the stage state.
uint32_t SceneBattleBase::invoketSkill(bt::SharedPtr *out, SceneBattleBase *self, uint32_t medal, int32_t timing,
                                       uint32_t flag1, uint32_t flag2) {
    out->obj = 0;
    out->ctrl = 0;
    uint32_t skillId = 0;
    if (medal) {
        uint64_t id = medalSkillId(medal);
        Frame f;
        uint32_t tmp = f.alloc(8);
        invoke(tmp, flag1, id, uint32_t(timing), flag1, flag2);
        out->obj = ld32(tmp);
        out->ctrl = ld32(tmp + 4);
        if (out->obj) {
            skillId = uint32_t(safe32(out->obj, 0));
            vcall(rw::guest_addr(self), VSLOT_ON_SKILL, {rw::guest_addr(self), skillId});
        }
    }
    if (timing == 1) stageBuffer()->set32(SS_SKILL_ID, int32_t(skillId));
    return 0;
}
RW_FUNCTION(0x5FAB68, "SceneBattleBase::invoketSkill", SceneBattleBase::invoketSkill);

// ------------------------------------------------------------------------------------------- HP / CP
uint32_t SceneBattleBase::setPlayerDamage(SceneBattleBase *self, int32_t damage) {
    uint32_t hp = call(StageState_addDamage, {stageState(), uint32_t(damage)});
    uint32_t indicator = StageUI::getPlayerStatusField(rw::host_ptr<StageUI>(self->stageUI));
    call(PlayerStatusIndicator_setHP, {indicator, hp});
    return hp == 0 ? 1 : 0;
}
RW_FUNCTION(0x5FAC60, "SceneBattleBase::setPlayerDamage", SceneBattleBase::setPlayerDamage);

// Heal (only while alive). Returns 1 when healed.
uint32_t SceneBattleBase::addPlayerHP(SceneBattleBase *self, int32_t amount) {
    if (safe32(stageState(), 8) == 0) return 0;
    uint32_t hp = call(StageState_addDamage, {stageState(), 0u - uint32_t(amount)});
    uint32_t indicator = StageUI::getPlayerStatusField(rw::host_ptr<StageUI>(self->stageUI));
    if (indicator) call(PlayerStatusIndicator_setHP, {indicator, hp});
    return 1;
}
RW_FUNCTION(0x5FAC9C, "SceneBattleBase::addPlayerHP", SceneBattleBase::addPlayerHP);

uint32_t SceneBattleBase::addPlayerCP(SceneBattleBase *self, int32_t amount) {
    uint32_t cp = call(StageState_addCP, {stageState(), uint32_t(amount)});
    uint32_t indicator = StageUI::getPlayerStatusField(rw::host_ptr<StageUI>(self->stageUI));
    if (indicator) call(PlayerStatusIndicator_setCP, {indicator, cp});
    uint32_t carousel = StageUI::getCarouselDeck(rw::host_ptr<StageUI>(self->stageUI));
    call(CarouselDeck_setPossibleBurst, {carousel, 1, 0});
    return 1;
}
RW_FUNCTION(0x5FAD2C, "SceneBattleBase::addPlayerCP", SceneBattleBase::addPlayerCP);

// Pay a special attack's cost: invoke the medal's cost skill (timing 5); a skill with +248 == 1 cancels the
// cost (effect only), otherwise the gauge drops by the cost. Returns 0 when the gauge is too low.
uint32_t SceneBattleBase::subPlayerCP(SceneBattleBase *self, uint32_t cost, uint32_t medal) {
    if (uint32_t(safe32(stageState(), 12)) < cost) return 0;
    Frame f;
    uint32_t out = f.alloc(8);
    invoketSkill(rw::frame_ptr<bt::SharedPtr>(out), self, medal, 5, 0, 1);
    uint32_t skill = ld32(out);
    if (skill && safe32(skill, 248) == 1) {
        uint32_t indicator = StageUI::getPlayerStatusField(rw::host_ptr<StageUI>(self->stageUI));
        if (indicator) call(PlayerStatusIndicator_playEffectCostCancel, {indicator});
    } else {
        uint32_t indicator = StageUI::getPlayerStatusField(rw::host_ptr<StageUI>(self->stageUI));
        uint32_t cp = call(StageState_addCP, {stageState(), 0u - cost});
        call(PlayerStatusIndicator_setCP, {indicator, cp});
    }
    sharedRelease(out);
    return 1;
}
RW_FUNCTION(0x5FAD78, "SceneBattleBase::subPlayerCP", SceneBattleBase::subPlayerCP);

// The current burst is usable and the gauge holds its cost (gauges x 10000, unsigned compare).
uint32_t SceneBattleBase::isBurstEnoughCP(SceneBattleBase *self) {
    uint32_t deck = StageUI::getMedalField(rw::host_ptr<StageUI>(self->stageUI));
    if (call(MedalDeck_isValidCurrentBurst, {deck}) != 1) return 0;
    uint32_t gauge = call(MedalDeck_getCurrentBurstGauge, {deck});
    uint32_t cp = uint32_t(safe32(stageState(), 12));
    return gauge * 10000u <= cp ? 1 : 0;
}
RW_FUNCTION(0x5FAE74, "SceneBattleBase::isBurstEnoughCP", SceneBattleBase::isBurstEnoughCP);

uint32_t SceneBattleBase::firstSetPlayerCP(SceneBattleBase *self, int32_t amount, int32_t keybladeId) {
    call(StageState_setCpMax, {stageState(), uint32_t(keybladeId)});
    return addPlayerCP(self, amount);
}
RW_FUNCTION(0x5FAF04, "SceneBattleBase::firstSetPlayerCP", SceneBattleBase::firstSetPlayerCP);

// ------------------------------------------------------------------------------------------- lambdas
namespace sbb {
// invokeSkill(timing)::$_4 (bool(const MedalInfo&)): invoke the medal's skill for the timing; at timing 4 the
// skill's value (+256) goes to the stage state. Always continues (1).
uint32_t invokeSkillEach(uint32_t self, uint32_t medal) {
    uint32_t timing = ld32(self + 4);
    uint64_t id = medalSkillId(medal);
    Frame f;
    uint32_t out = f.alloc(8);
    invoke(out, 0, id, timing, 0, 0);
    uint32_t skill = ld32(out);
    if (skill) {
        safe32(skill, 0);
        if (ld32(self + 4) == 4) {
            uint32_t state = stageState();
            int32_t value = safe32(ld32(out), 256);
            rw::host_ptr<SafeBuffer>(state)->set32(SS_SKILL_VALUE, value);
        }
    }
    sharedRelease(out);
    return 1;
}

// onMenuButton(PlayStyle)::$_3 (void(bool)): the battle menu closed: resume the actors.
void menuClosed(uint32_t self, uint32_t) {
    uint32_t scene = ld32(self + 4);
    call(StageActorManager_resumeToAllActor, {ld32(scene + 700)});
    rw::save<uint8_t>(scene + 721, 0);
}

// init_UI()::$_0: count turn: statuses count down, refresh the avatar.
void onCountTurn(uint32_t self) {
    uint32_t scene = ld32(self + 4);
    call(IStatusProtocol_elapseCountStatus, {stageState() + 32});
    call(AvatarActor_UpdateStatus, {ld32(ld32(scene + 700) + ACTOR_MANAGER_AVATAR)});
}

// init_UI()::$_1: attack turn: statuses and attack buffs tick, refresh the avatar, then the scene's hook.
void onAttackTurn(uint32_t self) {
    uint32_t scene = ld32(self + 4);
    call(IStatusProtocol_elapseTurnStatus, {stageState() + 32});
    call(IBuffProtocol_elapseTurnAttackBuff, {stageState() + 36});
    call(AvatarActor_UpdateStatus, {ld32(ld32(scene + 700) + ACTOR_MANAGER_AVATAR)});
    vcall(scene, SceneBattleBase::VSLOT_ON_ATTACK_TURN, {scene});
}

// init_UI()::$_2: defence turn: defence buffs tick, refresh the avatar, the scene's hook, then the turn rate
// accumulates (stage state 0x4250 += 0x424C).
void onDefenseTurn(uint32_t self) {
    uint32_t scene = ld32(self + 4);
    call(IBuffProtocol_elapseTurnDefenseBuff, {stageState() + 36});
    call(AvatarActor_UpdateStatus, {ld32(ld32(scene + 700) + ACTOR_MANAGER_AVATAR)});
    vcall(scene, SceneBattleBase::VSLOT_ON_DEFENSE_TURN, {scene});
    uint32_t state = stageState();
    int32_t rate = safe32(stageState(), SS_TURN_RATE);
    int32_t add = safe32(stageState(), SS_TURN_RATE_ADD);
    rw::host_ptr<SafeBuffer>(state)->set32(SS_TURN_RATE, int32_t(uint32_t(add) + uint32_t(rate)));
}

// lambda type names (target) and type_infos (target_type)
constexpr uint32_t NAME_InvokeSkill = 0xE4EC70, TI_InvokeSkill = 0xF96B34;
constexpr uint32_t NAME_MenuClosed = 0xE4ED40, TI_MenuClosed = 0xF96B48;
constexpr uint32_t NAME_InitUI2 = 0xE4EDF0, TI_InitUI2 = 0xF96B5C;
constexpr uint32_t NAME_InitUI1 = 0xE4EE80, TI_InitUI1 = 0xF96B70;
constexpr uint32_t NAME_InitUI0 = 0xE4EF10, TI_InitUI0 = 0xF96B84;
}  // namespace sbb

// invokeSkill(InvokeShowTiming)::$_4
RW_FUNCTION(0x5FAF2E, "SceneBattleBase::invokeSkill::$_4::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5FAF38, "SceneBattleBase::invokeSkill::$_4::__clone", thunk::clone1<VT_InvokeSkill>);
RW_FUNCTION(0x5FAF64, "SceneBattleBase::invokeSkill::$_4::__clone(dst)", thunk::cloneTo1<VT_InvokeSkill>);
RW_FUNCTION(0x5FAF84, "SceneBattleBase::invokeSkill::$_4::destroy", thunk::nop);
RW_FUNCTION(0x5FAF86, "SceneBattleBase::invokeSkill::$_4::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5FAF90, "SceneBattleBase::invokeSkill::$_4::operator()", sbb::invokeSkillEach);
RW_FUNCTION(0x5FB074, "SceneBattleBase::invokeSkill::$_4::target", thunk::target<sbb::NAME_InvokeSkill>);
RW_FUNCTION(0x5FB094, "SceneBattleBase::invokeSkill::$_4::target_type", thunk::targetType<sbb::TI_InvokeSkill>);

// onMenuButton(PlayStyle)::$_3
RW_FUNCTION(0x5FB0A8, "SceneBattleBase::onMenuButton::$_3::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5FB0B0, "SceneBattleBase::onMenuButton::$_3::__clone", thunk::clone1<VT_MenuClosed>);
RW_FUNCTION(0x5FB0DC, "SceneBattleBase::onMenuButton::$_3::__clone(dst)", thunk::cloneTo1<VT_MenuClosed>);
RW_FUNCTION(0x5FB0FC, "SceneBattleBase::onMenuButton::$_3::destroy", thunk::nop);
RW_FUNCTION(0x5FB0FE, "SceneBattleBase::onMenuButton::$_3::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5FB108, "SceneBattleBase::onMenuButton::$_3::operator()", sbb::menuClosed);
RW_FUNCTION(0x5FB124, "SceneBattleBase::onMenuButton::$_3::target", thunk::target<sbb::NAME_MenuClosed>);
RW_FUNCTION(0x5FB144, "SceneBattleBase::onMenuButton::$_3::target_type", thunk::targetType<sbb::TI_MenuClosed>);

// init_UI()::$_2
RW_FUNCTION(0x5FB158, "SceneBattleBase::init_UI::$_2::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5FB160, "SceneBattleBase::init_UI::$_2::__clone", thunk::clone1<VT_InitUI2>);
RW_FUNCTION(0x5FB18C, "SceneBattleBase::init_UI::$_2::__clone(dst)", thunk::cloneTo1<VT_InitUI2>);
RW_FUNCTION(0x5FB1AC, "SceneBattleBase::init_UI::$_2::destroy", thunk::nop);
RW_FUNCTION(0x5FB1AE, "SceneBattleBase::init_UI::$_2::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5FB1B8, "SceneBattleBase::init_UI::$_2::operator()", sbb::onDefenseTurn);
RW_FUNCTION(0x5FB280, "SceneBattleBase::init_UI::$_2::target", thunk::target<sbb::NAME_InitUI2>);
RW_FUNCTION(0x5FB2A0, "SceneBattleBase::init_UI::$_2::target_type", thunk::targetType<sbb::TI_InitUI2>);

// init_UI()::$_1
RW_FUNCTION(0x5FB2B4, "SceneBattleBase::init_UI::$_1::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5FB2BC, "SceneBattleBase::init_UI::$_1::__clone", thunk::clone1<VT_InitUI1>);
RW_FUNCTION(0x5FB2E8, "SceneBattleBase::init_UI::$_1::__clone(dst)", thunk::cloneTo1<VT_InitUI1>);
RW_FUNCTION(0x5FB308, "SceneBattleBase::init_UI::$_1::destroy", thunk::nop);
RW_FUNCTION(0x5FB30A, "SceneBattleBase::init_UI::$_1::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5FB312, "SceneBattleBase::init_UI::$_1::operator()", sbb::onAttackTurn);
RW_FUNCTION(0x5FB34C, "SceneBattleBase::init_UI::$_1::target", thunk::target<sbb::NAME_InitUI1>);
RW_FUNCTION(0x5FB36C, "SceneBattleBase::init_UI::$_1::target_type", thunk::targetType<sbb::TI_InitUI1>);

// init_UI()::$_0
RW_FUNCTION(0x5FB380, "SceneBattleBase::init_UI::$_0::~__func (deleting)", thunk::deleteThis);
RW_FUNCTION(0x5FB388, "SceneBattleBase::init_UI::$_0::__clone", thunk::clone1<VT_InitUI0>);
RW_FUNCTION(0x5FB3B4, "SceneBattleBase::init_UI::$_0::__clone(dst)", thunk::cloneTo1<VT_InitUI0>);
RW_FUNCTION(0x5FB3D4, "SceneBattleBase::init_UI::$_0::destroy", thunk::nop);
RW_FUNCTION(0x5FB3D6, "SceneBattleBase::init_UI::$_0::destroy_deallocate", thunk::deleteThis);
RW_FUNCTION(0x5FB3DE, "SceneBattleBase::init_UI::$_0::operator()", sbb::onCountTurn);
RW_FUNCTION(0x5FB3FC, "SceneBattleBase::init_UI::$_0::target", thunk::target<sbb::NAME_InitUI0>);
RW_FUNCTION(0x5FB41C, "SceneBattleBase::init_UI::$_0::target_type", thunk::targetType<sbb::TI_InitUI0>);
