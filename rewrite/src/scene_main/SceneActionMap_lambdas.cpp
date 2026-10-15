// SceneActionMap: the lambda bodies (std::function operator()) of its callbacks. Each takes the __func object
// (vtable, then the captures: the scene at +4 unless noted); std::function forwards arguments by reference, so a
// value parameter arrives as a pointer.
#include "SceneActionMap.hpp"

using namespace smain;
using rw::guest_addr;

namespace {

namespace g {
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t SafeBuffer_read = 0x45A269;            // (buf, dst, src, size, offset)
constexpr uint32_t SafeBuffer_write = 0x459FA1;
constexpr uint32_t StageState_getCpMax = 0x4EF8F5;
constexpr uint32_t SceneBattleBase_addPlayerCP = 0x5FAD2D;
constexpr uint32_t SceneBattleBase_invokeSkill = 0x5FA975;
constexpr uint32_t SceneBattleBase_onMenuButton = 0x5FA775;
constexpr uint32_t SceneBattleBase_isBurstEnoughCP = 0x5FAE75;
constexpr uint32_t TutorialMisc_create = 0x777BB5;         // (shared_ptr out, id)
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t SceneUtil_CompleteTutorialPhase = 0x472A75;   // (function<void()>, bool, bool)
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;
constexpr uint32_t StageUI_getCarouselDeck = 0x75E661;
constexpr uint32_t StageUI_createTutorialWidget = 0x760EC9;
constexpr uint32_t StageUI_setTutorialWidgetOnSwipe = 0x761485;
constexpr uint32_t StageUI_deleteTutorialWidget = 0x75E5E1;
constexpr uint32_t StageUI_setBattleTurn = 0x761727;
constexpr uint32_t CarouselDeck_setTutorialAction = 0x5CF2D9;
constexpr uint32_t StageFieldManager_terminateCollectPrize = 0x7575F1;
constexpr uint32_t StageFieldManager_showMapNode = 0x755729;
constexpr uint32_t StageManager_execTransitionMap = 0x75C455;
constexpr uint32_t StageActorManager_showWanderingSymbolEnemy = 0x74F15D;
constexpr uint32_t StageExploreManager_startDodgeRoll = 0x75470D;
constexpr uint32_t StageDirection_all1turn_start = 0x769789;
constexpr uint32_t CommonDirection_HideBossMarkerAll = 0x766DAD;
constexpr uint32_t CommonDirection_heal = 0x766DF9;
constexpr uint32_t MedalDeck_turnEnd = 0x5DFF53;
constexpr uint32_t AvatarActor_setActionResurrection = 0x740879;
constexpr uint32_t Avatar_addBurst = 0x5C6289;
constexpr uint32_t MarkFader_fadeOut = 0x5D9D05;
constexpr uint32_t SpeechDisp_runSpeechSet = 0x492A4D;
constexpr uint32_t SpeechMap_at = 0x601971;               // unordered_map<SpeechDisp*, deque<string>>::operator[]
constexpr uint32_t SpeechMap_erase = 0x604EE1;            // __hash_table::__erase_unique(key)
constexpr uint32_t SpeechDeque_pop_front = 0x601DC1;
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;
constexpr uint32_t CallFunc_create = 0x866B39;
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t Sequence_create = 0x867999;
constexpr uint32_t ScaleTo_create = 0x86A8CD;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Rect_getMidX = 0x8DF259;
constexpr uint32_t Rect_getMidY = 0x8DF289;
constexpr uint32_t Vec2_ctor_xy = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Vec2_distance = 0x8E4EF5;
constexpr uint32_t Touch_getLocation = 0x900E57;
constexpr uint32_t EventDispatcher_pauseEventListenersForTarget = 0x8F2CCD;
constexpr uint32_t EventDispatcher_resumeEventListenersForTarget = 0x8F2D85;
constexpr uint32_t function_vv_copy = 0x4683CD;
constexpr uint32_t function_vv_move = 0x47F905;
constexpr uint32_t function_vv_call = 0x470E69;
constexpr uint32_t shared_release = 0xC8DBD1;
constexpr uint32_t BGM_BATTLE_WIN = 0x1DDCA740;           // audio::ResourceID
}  // namespace g

// __func vtables (+8)
constexpr uint32_t VT_battleStartTutorial_42_lambda = 0xF5E284;
constexpr uint32_t VT_battleStartTutorial_42 = 0xF5E2B0;
constexpr uint32_t VT_battleStartTutorial_43 = 0xF5E2DC;
constexpr uint32_t VT_battleStartTutorial_45_onSwipe = 0xF5E308;
constexpr uint32_t VT_battleStartTutorial_44 = 0xF5E334;
constexpr uint32_t VT_toLoadingModeSetup_40_lambda = 0xF5E3B8;
constexpr uint32_t VT_toLoadingModeSetup_40_lambda_lambda = 0xF5E3E4;
constexpr uint32_t VT_makeTalkWindow_39_lambda = 0xF5E43C;
constexpr uint32_t VT_stageFinishNotification_30_lambda = 0xF5E5F4;
constexpr uint32_t VT_updateBattleMode_26_lambda = 0xF5E6D0;

inline SceneActionMap *sceneOf(uint32_t func) { return rw::host_ptr<SceneActionMap>(ld32(func + 4)); }
inline uint32_t dispatcher() { return ld32(call(fn::Director_getInstance, {}) + 0x84); }

// a 4-byte field of a SafeBuffer-backed record
uint32_t readWord(uint32_t buf, uint32_t offset) {
    Frame f;
    uint32_t out = f.alloc(8);
    call(g::SafeBuffer_read, {buf, out, ld32(buf + 0xC) + offset, 4, offset});
    return ld32(out);
}
uint32_t stageStateWord(uint32_t offset) { return readWord(ld32(call(g::AppState_getInstance, {}) + 0x20), offset); }

// the tutorial misc value (TutorialMisc +0xC) compared with the number of enemies on the field
int32_t tutorialMiscValue(uint32_t id) {
    Frame f;
    uint32_t sp = f.alloc(8);
    call(g::TutorialMisc_create, {sp, id});
    int32_t v = int32_t(readWord(ld32(sp), 0xC));
    if (ld32(sp + 4)) call(g::shared_release, {ld32(sp + 4)});
    return v;
}
int32_t enemyCount(uint32_t self) {
    uint32_t actors = ld32(self + 0x2BC);
    return int32_t(ld32(actors + 0x50) - ld32(actors + 0x4C)) >> 2;
}

void resetBattleInput(uint32_t self) {
    rw::save<uint8_t>(self + 0x2DD, 0);
    rw::save<uint8_t>(self + 0x2DE, 0);
    rw::save<float>(self + 0x2D8, 0.5f);
    rw::save<uint8_t>(self + 0x2DF, 0);
    rw::save<uint8_t>(self + 0x2DC, 1);
    rw::save<uint32_t>(self + 0x2D4, 0);
}

// popup `id` whose callback is a heap __func (vtable vt) holding `words` captures (8 bytes apart, from caps) and a
// copy of the std::function at caps + 8 * words; returns the popup result's callable pointer (before it is freed)
uint32_t popupWithNext(uint32_t id, uint32_t vt, uint32_t caps, uint32_t words, uint32_t size) {
    Frame f;
    uint32_t tmp = f.alloc(8 * words + 0x18);
    for (uint32_t k = 0; k < words; k++) st32(tmp + 8 * k, ld32(caps + 8 * k));
    uint32_t fnCopy = tmp + 8 * words;
    call(g::function_vv_copy, {fnCopy, caps + 8 * words});
    uint32_t fn = f.alloc(0x18);
    st32(fn + 0x10, 0);
    uint32_t obj = call(fn::operator_new, {size});
    if (obj) {
        st32(obj, vt);
        for (uint32_t k = 0; k < words; k++) st32(obj + 8 + 8 * k, ld32(tmp + 8 * k));
        call(g::function_vv_move, {obj + 8 + 8 * words, fnCopy});
    }
    st32(fn + 0x10, obj);
    uint32_t out = f.alloc(0x18);
    call(g::SceneUtil_openTutorialPopup, {out, id, fn, 0, 0, 0, 0});
    uint32_t result = ld32(out + 0x10);
    functionDestroy(out);
    functionDestroy(fn);
    functionDestroy(fnCopy);
    return result;
}

}  // namespace

namespace SceneActionMapLambdas {

// executingTutorial1010()::$_50: the popup closed
void executingTutorial1010_50(uint32_t func) { rw::save<int32_t>(ld32(func + 4) + 0x33C, 1); }
// executingTutorial1006()::$_49 / changedMapTutorial()::$_48: the next tutorial step
void executingTutorial1006_49(uint32_t func) { SceneActionMap::executingTutorial1010(sceneOf(func)); }
void changedMapTutorial_48(uint32_t func) { SceneActionMap::executingTutorial1006(sceneOf(func)); }

// changedMapTutorial()::$_47 (const Rect& area, Touch*&): a tap within 50 of the area's centre counts as a field
// tap; then the scene stops taking touches
uint32_t changedMapTutorial_47(uint32_t func, uint32_t rect, uint32_t touchRef) {
    uint32_t touch = ld32(touchRef);
    uint32_t self = ld32(func + 4);
    float mx = rw::callf(g::Rect_getMidX, {rect});
    float my = rw::callf(g::Rect_getMidY, {rect});
    Frame f;
    uint32_t centre = f.alloc(8);
    call(g::Vec2_ctor_xy, {centre, fbits(mx), fbits(my)});
    uint32_t loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    float d = rw::callf(g::Vec2_distance, {centre, loc});
    call(g::Vec2_dtor, {loc});
    if (d < 50.0f) {
        SceneActionMap::onTouchBeganField(rw::host_ptr<SceneActionMap>(self), touch, 0);
        call(g::EventDispatcher_pauseEventListenersForTarget, {dispatcher(), self, 0});
    }
    call(g::Vec2_dtor, {centre});
    return 0;
}

// stageFinishTutorial()::$_46: run the stored callback (+8)
void stageFinishTutorial_46(uint32_t func) { call(g::function_vv_call, {func + 8}); }

// battleStartTutorial: a chain of tutorial popups. Each step's captures are (scene, ..., ..., callback) 8 bytes
// apart; a step whose condition does not hold goes straight to the next one.
void battleStartTutorial_42_body(uint32_t caps);
void battleStartTutorial_43_body(uint32_t caps);
void battleStartTutorial_44_body(uint32_t caps);

// $_45: popup 7 (unless the enemies outnumber TutorialMisc 10031), then the swipe tutorial widget
void battleStartTutorial_45(uint32_t func) {
    uint32_t self = ld32(func + 8);
    int32_t limit = tutorialMiscValue(0x272F);
    if (limit > enemyCount(self)) {
        battleStartTutorial_44_body(func + 0x10);
        return;
    }
    uint32_t shown = popupWithNext(7, VT_battleStartTutorial_44, func + 0x10, 3, 0x38);
    if (shown == 0) return;
    uint32_t widget = call(g::StageUI_createTutorialWidget, {ld32(self + 0x2B8), 0x66});
    if (widget == 0) return;
    {
        Frame f;
        uint32_t onSwipe = newFunction(f, VT_battleStartTutorial_45_onSwipe, {self});
        call(g::StageUI_setTutorialWidgetOnSwipe, {ld32(self + 0x2B8), onSwipe});
        functionDestroy(onSwipe);
    }
    call(g::EventDispatcher_pauseEventListenersForTarget, {dispatcher(), self, 0});
    vcall(self, node::addChildZ, {self, widget, 0x14});
}

// $_44: popup 9 (unless the enemies outnumber TutorialMisc 10101)
void battleStartTutorial_44_body(uint32_t caps) {
    uint32_t self = ld32(caps);
    int32_t limit = tutorialMiscValue(0x2775);
    if (limit > enemyCount(self)) {
        battleStartTutorial_43_body(caps + 8);
        return;
    }
    popupWithNext(9, VT_battleStartTutorial_43, caps + 8, 2, 0x30);
}

// $_43: popup 10 when the actor manager has a guard (+0x8C >= 1)
void battleStartTutorial_43_body(uint32_t caps) {
    uint32_t self = ld32(caps);
    if (rw::load<int32_t>(ld32(self + 0x2BC) + 0x8C) < 1) {
        battleStartTutorial_42_body(caps + 8);
        return;
    }
    popupWithNext(10, VT_battleStartTutorial_42, caps + 8, 1, 0x28);
}

// $_42: with enough CP, popup 5 then the burst tutorial on the carousel deck; else just the final callback
void battleStartTutorial_42_body(uint32_t caps) {
    uint32_t self = ld32(caps);
    uint32_t done = caps + 8;
    if (call(g::SceneBattleBase_isBurstEnoughCP, {self}) != 1) {
        call(g::function_vv_call, {done});
        return;
    }
    uint32_t shown;
    {
        Frame f;
        uint32_t cb = f.alloc(0x18);
        call(g::function_vv_copy, {cb, done});
        uint32_t out = f.alloc(0x18);
        call(g::SceneUtil_openTutorialPopup, {out, 5, cb, 0, 0, 0, 0});
        shown = ld32(out + 0x10);
        functionDestroy(out);
        functionDestroy(cb);
    }
    if (shown == 0) return;
    uint32_t deck = call(g::StageUI_getCarouselDeck, {ld32(self + 0x2B8)});
    {
        Frame f;
        uint32_t fn = newFunction(f, VT_battleStartTutorial_42_lambda, {self});
        call(g::CarouselDeck_setTutorialAction, {deck, fn});
        functionDestroy(fn);
    }
    call(g::EventDispatcher_pauseEventListenersForTarget, {dispatcher(), self, 0});
    resetBattleInput(self);
}

void battleStartTutorial_44(uint32_t func) { battleStartTutorial_44_body(func + 8); }
void battleStartTutorial_43(uint32_t func) { battleStartTutorial_43_body(func + 8); }
void battleStartTutorial_42(uint32_t func) { battleStartTutorial_42_body(func + 8); }

// $_42::lambda(): the burst tutorial action happened: the scene takes touches again
void battleStartTutorial_42_lambda(uint32_t func) {
    call(g::EventDispatcher_resumeEventListenersForTarget, {dispatcher(), ld32(func + 4), 0});
}

// $_45::lambda(GestureRecognizer*, const Vec2&, unsigned): the tutorial swipe: in battle it counts as a swipe
void battleStartTutorial_45_onSwipe(uint32_t func, uint32_t, uint32_t, uint32_t) {
    uint32_t self = ld32(func + 4);
    if (rw::load<int32_t>(ld32(self + 0x320) + 0x18) == 1) {
        rw::save<uint8_t>(self + 0x2DE, 1);
        rw::save<uint32_t>(self + 0x2D4, 0);
    }
    call(g::EventDispatcher_resumeEventListenersForTarget, {dispatcher(), self, 0});
    call(g::StageUI_deleteTutorialWidget, {ld32(self + 0x2B8)});
}

// toFieldModeSetup()::$_41 / update()::$_22: the popup closed, the scene runs again
void toFieldModeSetup_41(uint32_t func) { rw::save<uint8_t>(ld32(func + 4) + 0x2D1, 0); }
void update_22(uint32_t func) { rw::save<uint8_t>(ld32(func + 4) + 0x2D1, 0); }

// toLoadingModeSetup()::$_40 (scene, fader): the wipe covers the screen: build the next map, then after one
// frame fade the wipe out
void toLoadingModeSetup_40(uint32_t func) {
    uint32_t self = ld32(func + 4);
    SceneActionMap *s = rw::host_ptr<SceneActionMap>(self);
    call(g::StageFieldManager_terminateCollectPrize, {s->fieldManager, ld32(s->actorManager + 0x20)});
    call(g::StageManager_execTransitionMap, {s->stageManager});
    SceneActionMap::setNewMap(s, ld32(s->actorManager + 0x20));
    call(g::StageActorManager_showWanderingSymbolEnemy, {s->actorManager});
    call(g::StageFieldManager_showMapNode, {s->fieldManager});
    SceneActionMap::toFieldModeSetup(s, 0);
    call(g::CommonDirection_HideBossMarkerAll, {s->commonDirection});
    uint32_t dir = call(fn::Director_getInstance, {});
    double interval = rw::load<double>(dir + 0xA8);
    uint32_t delay = call(g::DelayTime_create, {fbits(float(interval))});
    Frame f;
    uint32_t fn = newFunction(f, VT_toLoadingModeSetup_40_lambda, {ld32(func + 8), self});
    uint32_t cf = call(g::CallFunc_create, {fn});
    uint32_t seq = call(g::Sequence_create, {delay, cf, 0});
    call(g::Node_runAction, {self, seq});
    functionDestroy(fn);
}

// $_40::lambda() (fader, scene): fade the wipe out
void toLoadingModeSetup_40_lambda(uint32_t func) {
    uint32_t fader = ld32(func + 4), self = ld32(func + 8);
    Frame f;
    uint32_t fn = newFunction(f, VT_toLoadingModeSetup_40_lambda_lambda, {self, fader});
    call(g::MarkFader_fadeOut, {fader, fn});
    functionDestroy(fn);
}

// $_40::lambda()::lambda() (scene, fader): the wipe is gone: remove it, mark the map shown, tutorial hook
void toLoadingModeSetup_40_lambda_lambda(uint32_t func) {
    uint32_t self = ld32(func + 4), fader = ld32(func + 8);
    vcall(self, node::removeChild, {self, fader, 1});
    rw::save<int32_t>(self + 0x33C, 1);
    call(0x601EED, {self});   // SceneActionMap::changedMapTutorial
}

// makeTalkWindow()::$_39: the talk window opens (scale-in), then its speech set runs
void makeTalkWindow_39(uint32_t func) {
    uint32_t self = ld32(func + 4);
    uint32_t layer = ld32(self + 0x36C);
    vcall(layer, node::addChildZTag, {layer, ld32(self + 0x378), 5, 5});
    uint32_t frame = ld32(self + 0x370);
    vcall(frame, node::setVisible, {frame, 1});
    frame = ld32(self + 0x370);
    vcall(frame, node::setScaleX, {frame, fbits(0.0f)});
    frame = ld32(self + 0x370);
    vcall(frame, node::setScaleY, {frame, fbits(1.0f)});
    uint32_t scale = call(g::ScaleTo_create, {0x3E088889, fbits(1.0f)});   // 0.1333 s
    frame = ld32(self + 0x370);
    uint32_t seq = call(g::Sequence_create, {scale, 0});
    call(g::Node_runAction, {frame, seq});
    uint32_t talk = ld32(self + 0x378);
    Frame f;
    uint32_t fn = newFunction(f, VT_makeTalkWindow_39_lambda, {self});
    call(g::SpeechDisp_runSpeechSet, {talk, call(g::CallFunc_create, {fn})});
    functionDestroy(fn);
    uint32_t speaker = ld32(ld32(self + 0x2C0) + 0xD4);
    st32(ld32(self + 0x320) + 0x1C, ld32(speaker + 0x2B4) == 1 ? 4 : 2);
}

namespace {
// the window's next-line cursor shows; the current line leaves its queue (the queue is dropped when empty)
void popSpeech(uint32_t self) {
    uint32_t cursor = ld32(self + 0x374);
    vcall(cursor, node::setVisible, {cursor, 1});
    uint32_t q = call(g::SpeechMap_at, {self + 0x37C, self + 0x378});
    call(g::SpeechDeque_pop_front, {q});
    if (ld32(q + 0x14) == 0) call(g::SpeechMap_erase, {self + 0x37C, self + 0x378});
}
}  // namespace

// $_39::lambda(): the speech set finished. A healing speaker (+0x2B4 == 1, once: +0x2C0, scene +0x390) heals and
// revives the player by the API's revive HP; talk state 5 (wait) or 3 (next)
void makeTalkWindow_39_lambda(uint32_t func) {
    uint32_t self = ld32(func + 4);
    popSpeech(self);
    uint32_t speaker = ld32(ld32(self + 0x2C0) + 0xD4);
    if (ld32(speaker + 0x2B4) != 1 || ld8(self + 0x390) != 0) {
        st32(ld32(self + 0x320) + 0x1C, 3);
        return;
    }
    if (ld32(speaker + 0x2C0) == 0) {
        st32(speaker + 0x2C0, 1);
        call(g::CommonDirection_heal, {ld32(self + 0x330)});
        uint32_t hp = readWord(ld32(call(g::APIManager_getInstance, {}) + 0x8C), 0x38);
        SceneActionMap::revivePlayer(rw::host_ptr<SceneActionMap>(self), int32_t(hp));
        st8(self + 0x390, 1);
    }
    st32(ld32(self + 0x320) + 0x1C, 5);
}

// onTouchEndedTalk()::$_38 / $_37: the cursor shows, the talk moves to state 5 / 3
void onTouchEndedTalk_38(uint32_t func) {
    uint32_t self = ld32(func + 4);
    uint32_t cursor = ld32(self + 0x374);
    vcall(cursor, node::setVisible, {cursor, 1});
    SceneActionMap::subTalkText(rw::host_ptr<SceneActionMap>(self), 5);
}
void onTouchEndedTalk_37(uint32_t func) {
    uint32_t self = ld32(func + 4);
    uint32_t cursor = ld32(self + 0x374);
    vcall(cursor, node::setVisible, {cursor, 1});
    SceneActionMap::subTalkText(rw::host_ptr<SceneActionMap>(self), 3);
}
// $_36: the line is consumed: state 5
void onTouchEndedTalk_36(uint32_t func) {
    uint32_t self = ld32(func + 4);
    popSpeech(self);
    st32(ld32(self + 0x320) + 0x1C, 5);
}

// updateTalkMode()::$_35: the talk window leaves the layer; $_34: talk state 8 (closed)
void updateTalkMode_35(uint32_t func) {
    uint32_t self = ld32(func + 4);
    uint32_t layer = ld32(self + 0x36C);
    vcall(layer, node::removeChild, {layer, ld32(self + 0x370), 1});
}
void updateTalkMode_34(uint32_t func) { st32(ld32(ld32(func + 4) + 0x320) + 0x1C, 8); }

// updateIntrludeMode()::$_33: back to the adventure select (its API call with no callback)
void updateIntrludeMode_33(uint32_t) {
    Frame f;
    uint32_t fn = newEmptyFunction(f);
    call(g::SceneAdventureSelect_callAdventureSelectAPI, {fn});
    functionDestroy(fn);
}
// $_32(bool&): when set, complete the tutorial phase
void updateIntrludeMode_32(uint32_t, uint32_t flag) {
    if (ld8(flag) == 0) return;
    Frame f;
    uint32_t fn = newEmptyFunction(f);
    call(g::SceneUtil_CompleteTutorialPhase, {fn, 0, 0});
    functionDestroy(fn);
}

// stageFinishNotification()::$_31, $_30::lambda(), updateBattleMode()::$_29: on to the interlude
void stageFinishNotification_31(uint32_t func) { SceneActionMap::toIntrludeModeSetup(sceneOf(func)); }
void stageFinishNotification_30_lambda(uint32_t func) { SceneActionMap::toIntrludeModeSetup(sceneOf(func)); }
void updateBattleMode_29(uint32_t func) { SceneActionMap::toIntrludeModeSetup(sceneOf(func)); }

// $_30: the "all in one turn" direction first
void stageFinishNotification_30(uint32_t func) {
    uint32_t self = ld32(func + 4);
    Frame f;
    uint32_t fn = newFunction(f, VT_stageFinishNotification_30_lambda, {self});
    uint32_t cf = call(g::CallFunc_create, {fn});
    functionDestroy(fn);
    call(g::StageDirection_all1turn_start, {ld32(self + 0x32C), cf});
}

// updateBattleMode()::$_28 (deck, scene, avatar actor): continue after a defeat: revive with the API's HP and
// full CP, count the continue (state +0x4264), the resurrection motion, battle step 29, the revive skills
void updateBattleMode_28(uint32_t func) {
    uint32_t self = ld32(func + 8);
    call(g::MedalDeck_turnEnd, {ld32(func + 4)});
    uint32_t hp = readWord(ld32(call(g::APIManager_getInstance, {}) + 0x8C), 0x38);
    SceneActionMap::revivePlayer(rw::host_ptr<SceneActionMap>(self), int32_t(hp));
    uint32_t cpMax = call(g::StageState_getCpMax, {ld32(call(g::AppState_getInstance, {}) + 0x20)});
    call(g::SceneBattleBase_addPlayerCP, {self, cpMax});
    uint32_t state = ld32(call(g::AppState_getInstance, {}) + 0x20);
    Frame f;
    uint32_t v = f.alloc(4);
    st32(v, stageStateWord(0x4264) + 1);
    call(g::SafeBuffer_write, {state, ld32(state + 0xC) + 0x4264, v, 4, 0x4264});
    call(g::AvatarActor_setActionResurrection, {ld32(func + 0xC)});
    rw::save<int32_t>(self + 0x338, 0x1D);
    call(g::SceneBattleBase_invokeSkill, {self, 4});
}

// $_27: battle step 24 when the stage state's +8 flag is set
void updateBattleMode_27(uint32_t func) {
    uint32_t self = ld32(func + 4);
    if (stageStateWord(8)) rw::save<int32_t>(self + 0x338, 0x18);
}

// $_26: after half a second on the avatar, $_26::lambda() ends the turn (step 6, battle turn off)
void updateBattleMode_26(uint32_t func) {
    uint32_t self = ld32(func + 4);
    uint32_t avatar = ld32(ld32(self + 0x2BC) + 0x20);
    uint32_t delay = call(g::DelayTime_create, {fbits(0.5f)});
    Frame f;
    uint32_t fn = newFunction(f, VT_updateBattleMode_26_lambda, {self});
    uint32_t cf = call(g::CallFunc_create, {fn});
    uint32_t seq = call(g::Sequence_create, {delay, cf, 0});
    call(g::Node_runAction, {avatar, seq});
    functionDestroy(fn);
}
void updateBattleMode_26_lambda(uint32_t func) {
    uint32_t self = ld32(func + 4);
    rw::save<int32_t>(self + 0x338, 6);
    call(g::StageUI_setBattleTurn, {ld32(self + 0x2B8), 0});
}

// $_25: the victory BGM; $_24: the stage is finished
void updateBattleMode_25(uint32_t) { call(g::AudioReplaceGameMainBGM, {g::BGM_BATTLE_WIN, 500}); }
void updateBattleMode_24(uint32_t func) { SceneActionMap::stageFinishNotification(sceneOf(func)); }

// $_23: battle step 9 with the swipe flags cleared
void updateBattleMode_23(uint32_t func) {
    uint32_t self = ld32(func + 4);
    rw::save<int32_t>(self + 0x338, 9);
    rw::save<uint8_t>(self + 0x2DD, 0);
    rw::save<uint8_t>(self + 0x2DE, 0);
    rw::save<float>(self + 0x2D8, 0.5f);
}

// init()::$_21: the back key opens the menu (when the deck's info window is closed); $_20 (Ref*, TouchEventType&):
// the menu button released
void init_21(uint32_t func) {
    uint32_t self = ld32(func + 4);
    uint32_t deck = call(g::StageUI_getCarouselDeck, {ld32(self + 0x2B8)});
    if (deck == 0 || ld32(deck + 0x410) != 0) return;
    call(g::SceneBattleBase_onMenuButton, {self, ld32(ld32(self + 0x320) + 0x14)});
}
void init_20(uint32_t func, uint32_t, uint32_t type) {
    if (ld32(type) != 2) return;
    uint32_t self = ld32(func + 4);
    call(g::SceneBattleBase_onMenuButton, {self, ld32(ld32(self + 0x320) + 0x14)});
}

namespace {
// a medal joined the deck: its burst (medal +0x4A8 set, burst at +0x4AC) goes to the actor's avatar
void addMedalBurst(uint32_t func, uint32_t medalRef, uint32_t actorOffset) {
    uint32_t self = ld32(func + 4);
    uint32_t medal = ld32(medalRef);
    if (readWord(medal, 0x4A8) == 0) return;
    uint32_t actor = ld32(ld32(self + 0x2BC) + actorOffset);
    uint32_t burst = readWord(ld32(medalRef), 0x4AC);
    call(g::Avatar_addBurst, {ld32(actor + 0x288), burst});
}
}  // namespace

// init()::$_19 / $_18 (const shared_ptr<Medal>&): the friend's / the player's medal
void init_19(uint32_t func, uint32_t medal) { addMedalBurst(func, medal, 0x24); }
void init_18(uint32_t func, uint32_t medal) { addMedalBurst(func, medal, 0x20); }
// init()::$_17 (Vec2&): a dodge roll towards the point
void init_17(uint32_t func, uint32_t point) {
    call(g::StageExploreManager_startDodgeRoll, {ld32(ld32(func + 4) + 0x328), point});
}

}  // namespace SceneActionMapLambdas

using namespace SceneActionMapLambdas;
RW_FUNCTION(0x6023D2, "SceneActionMap::executingTutorial1010::$_50 operator()", executingTutorial1010_50);
RW_FUNCTION(0x602472, "SceneActionMap::executingTutorial1006::$_49 operator()", executingTutorial1006_49);
RW_FUNCTION(0x60250E, "SceneActionMap::changedMapTutorial::$_48 operator()", changedMapTutorial_48);
RW_FUNCTION(0x6025B0, "SceneActionMap::changedMapTutorial::$_47 operator()", changedMapTutorial_47);
RW_FUNCTION(0x602820, "SceneActionMap::stageFinishTutorial::$_46 operator()", stageFinishTutorial_46);
RW_FUNCTION(0x6029F0, "SceneActionMap::battleStartTutorial::$_45 operator()", battleStartTutorial_45);
RW_FUNCTION(0x602C84, "SceneActionMap::battleStartTutorial::$_44 body", battleStartTutorial_44_body);
RW_FUNCTION(0x602E30, "SceneActionMap::battleStartTutorial::$_43 body", battleStartTutorial_43_body);
RW_FUNCTION(0x602F8C, "SceneActionMap::battleStartTutorial::$_42 body", battleStartTutorial_42_body);
RW_FUNCTION(0x603758, "SceneActionMap::battleStartTutorial::$_44 operator()", battleStartTutorial_44);
RW_FUNCTION(0x6034BC, "SceneActionMap::battleStartTutorial::$_43 operator()", battleStartTutorial_43);
RW_FUNCTION(0x603308, "SceneActionMap::battleStartTutorial::$_42 operator()", battleStartTutorial_42);
RW_FUNCTION(0x60315A, "SceneActionMap::battleStartTutorial::$_42::lambda() operator()", battleStartTutorial_42_lambda);
RW_FUNCTION(0x603564, "SceneActionMap::battleStartTutorial::$_45::lambda(GestureRecognizer*,...) operator()", battleStartTutorial_45_onSwipe);
RW_FUNCTION(0x6037FC, "SceneActionMap::toFieldModeSetup::$_41 operator()", toFieldModeSetup_41);
RW_FUNCTION(0x6038A8, "SceneActionMap::toLoadingModeSetup::$_40 operator()", toLoadingModeSetup_40);
RW_FUNCTION(0x603A70, "SceneActionMap::toLoadingModeSetup::$_40::lambda() operator()", toLoadingModeSetup_40_lambda);
RW_FUNCTION(0x603BAA, "SceneActionMap::toLoadingModeSetup::$_40::lambda()::lambda() operator()", toLoadingModeSetup_40_lambda_lambda);
RW_FUNCTION(0x604984, "SceneActionMap::makeTalkWindow::$_39 operator()", makeTalkWindow_39);
RW_FUNCTION(0x604B60, "SceneActionMap::makeTalkWindow::$_39::lambda() operator()", makeTalkWindow_39_lambda);
RW_FUNCTION(0x604CEE, "SceneActionMap::onTouchEndedTalk::$_38 operator()", onTouchEndedTalk_38);
RW_FUNCTION(0x604DA2, "SceneActionMap::onTouchEndedTalk::$_37 operator()", onTouchEndedTalk_37);
RW_FUNCTION(0x604E56, "SceneActionMap::onTouchEndedTalk::$_36 operator()", onTouchEndedTalk_36);
RW_FUNCTION(0x605192, "SceneActionMap::updateTalkMode::$_35 operator()", updateTalkMode_35);
RW_FUNCTION(0x605242, "SceneActionMap::updateTalkMode::$_34 operator()", updateTalkMode_34);
RW_FUNCTION(0x605338, "SceneActionMap::updateIntrludeMode::$_33 operator()", updateIntrludeMode_33);
RW_FUNCTION(0x6054D4, "SceneActionMap::updateIntrludeMode::$_32 operator()", updateIntrludeMode_32);
RW_FUNCTION(0x6055F6, "SceneActionMap::stageFinishNotification::$_31 operator()", stageFinishNotification_31);
RW_FUNCTION(0x605694, "SceneActionMap::stageFinishNotification::$_30 operator()", stageFinishNotification_30);
RW_FUNCTION(0x6057CE, "SceneActionMap::stageFinishNotification::$_30::lambda() operator()", stageFinishNotification_30_lambda);
RW_FUNCTION(0x60586A, "SceneActionMap::updateBattleMode::$_29 operator()", updateBattleMode_29);
RW_FUNCTION(0x605914, "SceneActionMap::updateBattleMode::$_28 operator()", updateBattleMode_28);
RW_FUNCTION(0x605A8C, "SceneActionMap::updateBattleMode::$_27 operator()", updateBattleMode_27);
RW_FUNCTION(0x605B84, "SceneActionMap::updateBattleMode::$_26 operator()", updateBattleMode_26);
RW_FUNCTION(0x605CDA, "SceneActionMap::updateBattleMode::$_26::lambda() operator()", updateBattleMode_26_lambda);
RW_FUNCTION(0x605D88, "SceneActionMap::updateBattleMode::$_25 operator()", updateBattleMode_25);
RW_FUNCTION(0x605E2E, "SceneActionMap::updateBattleMode::$_24 operator()", updateBattleMode_24);
RW_FUNCTION(0x605ECC, "SceneActionMap::updateBattleMode::$_23 operator()", updateBattleMode_23);
RW_FUNCTION(0x605F88, "SceneActionMap::update::$_22 operator()", update_22);
RW_FUNCTION(0x60602A, "SceneActionMap::init::$_21 operator()", init_21);
RW_FUNCTION(0x6060EA, "SceneActionMap::init::$_20 operator()", init_20);
RW_FUNCTION(0x60619C, "SceneActionMap::init::$_19 operator()", init_19);
RW_FUNCTION(0x6062B8, "SceneActionMap::init::$_18 operator()", init_18);
RW_FUNCTION(0x6063D6, "SceneActionMap::init::$_17 operator()", init_17);
