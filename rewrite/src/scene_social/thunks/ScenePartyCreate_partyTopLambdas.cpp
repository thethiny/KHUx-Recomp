// Lambda bodies of ScenePartyTop (runInEntryLayout / runInJoinLayout) that sit next to ScenePartyCreate::create in
// the binary and are attributed to ScenePartyCreate (most of them open the party creation scene). Their
// std::function thunks are in ScenePartyTop_thunks.cpp.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
namespace g {
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;   // (mgr, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;    // (mgr, float seconds)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;      // (api, const function<void(bool)>&, bool)
constexpr uint32_t APIManager_executePartyLeaderChange = 0x4D008D;   // (api, u64 userId, const function<void(bool)>&)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5; // (anim, std::function<void()> by value)
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;     // (const string&, function<void(bool)> by value, label)
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t CallFunc_create = 0x866B39;
constexpr uint32_t Sequence_create = 0x867999;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t function_vv_call = 0x470E69;
constexpr uint32_t SafeBuffer_read = 0x45A269;
}  // namespace g

constexpr uint32_t createSceneLayer_ScenePartyCreate = 0x657DE9;   // createSceneLayer<ScenePartyCreate>()
constexpr uint32_t VT_LayerFactory = 0xF7E130;      // __func<Layer*(*)(), ..., Layer*()> + 8
constexpr uint32_t VT_join20Done = 0xF621B4;        // runInJoinLayout $_20::{lambda()#1} {top}
constexpr uint32_t VT_join18Done = 0xF62264;        // runInJoinLayout $_18::{lambda()#1} {top}
constexpr uint32_t VT_join26Done = 0xF6220C;        // runInJoinLayout $_26::{lambda()#1}: open ScenePartyCreate
constexpr uint32_t VT_join16Answer = 0xF622E8;      // $_16::{lambda(bool)#1} {top, member shared_ptr}
constexpr uint32_t VT_join16Changed = 0xF62314;     // $_16::{lambda(bool)#1}::{lambda(bool)#1} {top}
constexpr uint32_t VT_join16Done = 0xF62340;        // the "leader changed" dialog closed {top}
constexpr uint32_t VT_join16Error = 0xF62398;       // the API error dialog closed
constexpr uint32_t VT_join16Out = 0xF6236C;         // after the out animation

constexpr uint32_t TOUCH_ENDED = 2;
constexpr uint32_t SM_NO_TAP_TIME = 0x300;          // SceneManager: float, the scene change duration
constexpr uint32_t TOP_IN_OUT = 0x288;              // ScenePartyTop's IInOutAnimation base
constexpr uint32_t TOP_ON_CHANGE = 0x298;           // ScenePartyTop: std::function<void()> run after a change
constexpr int32_t TEXT_LEADER_CHANGE_CONFIRM = 103820001;
constexpr int32_t TEXT_LEADER_CHANGED = 103820002;

// SceneManager::changeSceneLayer(createSceneLayer<ScenePartyCreate>, true, true, fade 1)
void openPartyCreate() {
    Addr mgr = sceneManager();
    withLambda(VT_LayerFactory, {createSceneLayer_ScenePartyCreate},
               [&](Addr factory) { call(g::SceneManager_changeSceneLayer, {mgr, factory, 1, 1, 1}); });
}

// a button release: block touches for the scene change time, then run `vtable`'s lambda {node} on `node`
void delayedOnRelease(Addr node, uint32_t vtable) {
    call(g::AudioPlaySystemSEDecide, {});
    Addr mgr = sceneManager();
    call(g::SceneManager_openNoTapScreen, {mgr, ld32(sceneManager() + SM_NO_TAP_TIME)});
    Addr delay = call(g::DelayTime_create, {ld32(sceneManager() + SM_NO_TAP_TIME)});
    withLambda(vtable, {node}, [&](Addr fo) {
        Addr callFunc = call(g::CallFunc_create, {fo});
        call(g::Node_runAction, {node, call(g::Sequence_create, {delay, callFunc, 0})});
    });
}

void runOut(Addr top, uint32_t vtable) {
    withLambda(vtable, {}, [&](Addr fo) { call(g::IInOutAnimation_runOutAnimation, {top + TOP_IN_OUT, fo}); });
}
}  // namespace

struct PartyTopLambdas {
    // runInEntryLayout $_21::{lambda()#1}, runInJoinLayout $_26::{lambda()#1}: open the party creation scene
    static void openCreate(Addr) { openPartyCreate(); }
    // runInJoinLayout $_20 / $_18 {top}(touch): released - run the next step after the scene change time
    static void join20Touch(Addr func, const uint32_t *, const uint32_t *type) {
        if (*type != TOUCH_ENDED) return;
        delayedOnRelease(ld32(func + 4), VT_join20Done);
    }
    static void join18Touch(Addr func, const uint32_t *, const uint32_t *type) {
        if (*type != TOUCH_ENDED) return;
        delayedOnRelease(ld32(func + 4), VT_join18Done);
    }
    // $_20 / $_18::{lambda()#1} {top}: the top's change callback
    static void runOnChange(Addr func) { call(g::function_vv_call, {ld32(func + 4) + TOP_ON_CHANGE}); }
    // $_19 / $_17 {top}(touch): released - out animation, then the party creation scene
    static void createTouch(Addr func, const uint32_t *, const uint32_t *type) {
        if (*type != TOUCH_ENDED) return;
        call(g::AudioPlaySystemSEDecide, {});
        runOut(ld32(func + 4), VT_join26Done);
    }
    // $_16 {top, member}: confirm the leader change (ok / cancel dialog)
    static void confirmLeaderChange(Addr func) {
        Addr top = ld32(func + 4);
        Frame f;
        Addr text = textData(f, TEXT_LEADER_CHANGE_CONFIRM);
        Addr s = newStr(f, textDataStr(text));
        Addr member = ld32(func + 8), ctrl = ld32(func + 12);
        if (ctrl) guest::addRef(ctrl);
        withLambda(VT_join16Answer, {top, member, ctrl},
                   [&](Addr fo) { call(g::SceneUtil_openOkCancelDialog4Line, {s, fo, 0, 0}); });
        strFree(s);
        sharedRelease(text);
    }
    // $_16::{lambda(bool)#1} {top, member}(ok): ask the server to make the member the leader
    static void leaderChangeAnswer(Addr func, const uint32_t *okArg) {
        if (!uint8_t(*okArg)) return;
        Addr top = ld32(func + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        Addr api = call(g::APIManager_getInstance, {});
        Frame f;
        Addr id = f.alloc(8);
        Addr sb = ld32(func + 8);
        call(g::SafeBuffer_read, {sb, id, ld32(sb + 0xC), 8, 0});
        uint32_t lo = ld32(id), hi = ld32(id + 4);
        withLambda(VT_join16Changed, {top},
                   [&](Addr fo) { call(g::APIManager_executePartyLeaderChange, {api, id, lo, hi, fo}); });
    }
    // $_16::...{lambda(bool)#1} {top}(ok): the leader changed - tell the player; or the API error dialog
    static void leaderChanged(Addr func, const uint32_t *okArg) {
        uint8_t ok = uint8_t(*okArg);
        Addr top = ld32(func + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (ok) {
            Frame f;
            Addr text = textData(f, TEXT_LEADER_CHANGED);
            Addr s = newStr(f, textDataStr(text));
            withLambda(VT_join16Done, {top}, [&](Addr fo) { call(g::SceneUtil_openOkDialog4Line, {s, fo, 0}); });
            strFree(s);
            sharedRelease(text);
            return;
        }
        if (!ld8(call(g::APIManager_getInstance, {}) + 0x14)) return;
        Addr api = call(g::APIManager_getInstance, {});
        withLambda(VT_join16Error, {}, [&](Addr fo) { call(g::APIManager_openErrorDialog, {api, fo, 1}); });
    }
    // the "leader changed" dialog closed {top}: out animation
    static void leaderChangedClosed(Addr func, const uint32_t *) { runOut(ld32(func + 4), VT_join16Out); }
};

RW_FUNCTION(0x657D08, "ScenePartyTop::runInEntryLayout $_21 UlvE_ __func::operator()", PartyTopLambdas::openCreate);
RW_FUNCTION(0x657EF0, "ScenePartyTop::runInJoinLayout $_20 __func::operator()", PartyTopLambdas::join20Touch);
RW_FUNCTION(0x658076, "ScenePartyTop::runInJoinLayout $_20 UlvE_ __func::operator()", PartyTopLambdas::runOnChange);
RW_FUNCTION(0x65811C, "ScenePartyTop::runInJoinLayout $_19 __func::operator()", PartyTopLambdas::createTouch);
RW_FUNCTION(0x658250, "ScenePartyTop::runInJoinLayout $_26 UlvE_ __func::operator()", PartyTopLambdas::openCreate);
RW_FUNCTION(0x658390, "ScenePartyTop::runInJoinLayout $_18 __func::operator()", PartyTopLambdas::join18Touch);
RW_FUNCTION(0x658516, "ScenePartyTop::runInJoinLayout $_18 UlvE_ __func::operator()", PartyTopLambdas::runOnChange);
RW_FUNCTION(0x6585BC, "ScenePartyTop::runInJoinLayout $_17 __func::operator()", PartyTopLambdas::createTouch);
RW_FUNCTION(0x6587A4, "ScenePartyTop::runInJoinLayout $_16 __func::operator()", PartyTopLambdas::confirmLeaderChange);
RW_FUNCTION(0x6589FC, "ScenePartyTop::runInJoinLayout $_16 UlbE_ __func::operator()", PartyTopLambdas::leaderChangeAnswer);
RW_FUNCTION(0x658B64, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ __func::operator()", PartyTopLambdas::leaderChanged);
RW_FUNCTION(0x658D70, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ __func::operator()", PartyTopLambdas::leaderChangedClosed);
