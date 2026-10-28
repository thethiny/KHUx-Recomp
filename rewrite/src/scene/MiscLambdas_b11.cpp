// Lambda bodies of several owners left after the game rewrite (found when libsplit gave unnamed functions that call
// game code back to the game, 2026-10-29): SceneEvolveMedal, StageUtil, RaidBattle, SphereUI.
#include "SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneUtil_openMedalInfoPopup = 0x466EF5;
// (shared_ptr<UserMedalData>, bool, bool, bool, function<void(MedalInfoButtonType, bool)>, vector<UserSkill>*,
//  function<void()>, bool), the by-value arguments passed by address
constexpr uint32_t SceneUtil_clearMedalNewMark = 0x46C4B5;
constexpr uint32_t FunctionVB_call = 0x474139;
constexpr uint32_t SafeBuffer_read = 0x45A269;               // (sb, dst, src, n, index)
constexpr uint32_t RaidBattle_set_week_position = 0x5EE21D;
constexpr uint32_t CommonDirection_req_select_target_hide = 0x76547D;
constexpr uint32_t SphereUI_openPopUpIncentiveGet = 0x797FB9;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;    // (api, const function<void()>&, bool)
}  // namespace g

constexpr uint32_t VT_EvolveInfoButton = 0xF66498;   // SceneEvolveMedal::init $_5's medal info buttons callback
constexpr uint32_t VT_IncentiveError = 0xF6EEC0;     // SphereUI::sphereUpdateIncentive $_0's error dialog callback
constexpr uint32_t EVOLVE_MEDAL = 0x2A0;             // SceneEvolveMedal: shared_ptr<UserMedalData>
constexpr uint32_t API_USER_HEADER = 0x94;           // APIManager: SafeBuffer, byte 0x54 = the medal popup flag
constexpr uint32_t API_TREASURES = 0x260;            // APIManager: SafeBuffer of 24-byte treasure prizes
constexpr uint32_t TREASURE_SIZE = 0x18;
constexpr uint32_t API_SHOW_ERROR = 0x14;
constexpr uint32_t RB_UI = 0x278, RB_DIRECTION = 0x288;     // RaidBattle: StageUI*, CommonDirection*
constexpr uint32_t UI_SELECTED_TARGET = 0x39C;              // StageUI: int, < 0 none
constexpr uint32_t TOUCH_ENDED = 2;
}  // namespace

// SceneEvolveMedal::init $_5 {scene}(sender, type): the medal's info popup (read-only buttons)
static void evolveMedalInfo(uint32_t functor, uint32_t, uint32_t typep) {
    if (ld32(typep) != TOUCH_ENDED) return;
    const uint32_t scene = ld32(functor + 4);
    call(fn::AudioPlaySystemSEDecide, {});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t medal = f.alloc(8);
    st32(medal, ld32(scene + EVOLVE_MEDAL));
    st32(medal + 4, ld32(scene + EVOLVE_MEDAL + 4));
    if (ld32(medal + 4)) call(fn::shared_add, {ld32(medal + 4)});
    const uint8_t flag = ld8(safeBytes(f, ld32(api + API_USER_HEADER), 0x54, 1));
    const uint32_t buttons = newFunction(f, VT_EvolveInfoButton, {});
    const uint32_t closed = newEmptyFunction(f);
    call(g::SceneUtil_openMedalInfoPopup, {medal, flag ? 1u : 0u, 0, 1, buttons, 0, closed, 0});
    functionDestroy(closed);
    functionDestroy(buttons);
    if (ld32(medal + 4)) call(fn::shared_release, {ld32(medal + 4)});
}
RW_FUNCTION(0x6B5E6C, "SceneEvolveMedal::init $_5::operator() (medal info)", evolveMedalInfo);

// SceneEvolveMedal::init $_3 [](function<void(bool)> done): the medals' new marks cleared, then done(true)
static void evolveMedalLeave(uint32_t, uint32_t donePtr) {
    Frame f;
    const uint32_t done = f.alloc(24);
    call(fn::function_vb_move, {done, donePtr});
    call(g::SceneUtil_clearMedalNewMark, {});
    call(g::FunctionVB_call, {done, 1});
    functionDestroy(done);
}
RW_FUNCTION(0x6B6178, "SceneEvolveMedal::init $_3::operator() (leave)", evolveMedalLeave);

// StageUtil::getTreasurePrizes $_1 (sret prize, functor, const int& index): the index-th treasure prize record
static void treasurePrize(uint32_t out, uint32_t, uint32_t indexRef) {
    const uint32_t offset = ld32(indexRef) * TREASURE_SIZE;
    const uint32_t sb = ld32(call(fn::APIManager_getInstance, {}) + API_TREASURES);
    call(g::SafeBuffer_read, {sb, out, ld32(sb + 0xC) + offset, TREASURE_SIZE, offset});
}
RW_FUNCTION(0x76488A, "StageUtil::getTreasurePrizes $_1::operator()", treasurePrize);

// RaidBattle::update $_0 {battle}: no target selected -> the week position and the target selection hidden
static void raidBattleNoTarget(uint32_t functor) {
    const uint32_t battle = ld32(functor + 4);
    if (int32_t(ld32(ld32(battle + RB_UI) + UI_SELECTED_TARGET)) >= 0) return;
    call(g::RaidBattle_set_week_position, {battle});
    call(g::CommonDirection_req_select_target_hide, {ld32(battle + RB_DIRECTION)});
}
RW_FUNCTION(0x5F148A, "RaidBattle::update $_0::operator()", raidBattleNoTarget);

// SphereUI::sphereUpdateIncentive $_0 {ui}(ok): the incentive popup, or the API error dialog
static void sphereIncentiveDone(uint32_t functor, uint32_t okp) {
    const uint8_t ok = ld8(okp);
    const uint32_t ui = ld32(functor + 4);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        call(g::SphereUI_openPopUpIncentiveGet, {ui});
        return;
    }
    if (!ld8(call(fn::APIManager_getInstance, {}) + API_SHOW_ERROR)) return;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_IncentiveError, {});
    call(g::APIManager_openErrorDialog, {api, fo, 1});
    functionDestroy(fo);
}
RW_FUNCTION(0x7999A0, "SphereUI::sphereUpdateIncentive $_0::operator()", sphereIncentiveDone);

// ------------------------------------------------------------------------------------------------ more owners
namespace {
namespace g2 {
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t PresentBoxScrollView_popUpGetPresent = 0x50DEE9;   // (view, u64 presentId, function<void()> by value)
constexpr uint32_t SceneManager_openUnBlockPostPopup = 0x61E705;      // (mgr, ChatBlock by value, function<void()> by value)
constexpr uint32_t EnemyActor_isDied = 0x6008ED;
constexpr uint32_t EnemyActor_actionMarker = 0x74A65D;                // (actor, TapAction)
constexpr uint32_t FunctionActorBool_call = 0x74DEDD;                 // std::function<bool(EnemyActor*)>::operator()
constexpr uint32_t FunctionActorVoid_call = 0x74E2E5;                 // std::function<void(EnemyActor*, bool)>::operator()
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
}  // namespace g2

constexpr uint32_t VT_PresentError = 0xF5649C;       // PresentBoxScrollView::changeRow $_0's error dialog callback
constexpr uint32_t PRESENT_ON_GET = 0x680;           // PresentBoxScrollView: std::function<void()>
constexpr uint32_t CHAT_BLOCK_ENABLED = 0x53C;       // ChatBlockListScrollView: byte, rows take taps
constexpr uint32_t CHAT_BLOCK_ON_UNBLOCK = 0x638;    // ChatBlockListScrollView: std::function<void()>
// EnemyActor
constexpr uint32_t EA_TOUCHED = 0x350;               // byte: the touch began on the enemy
constexpr uint32_t EA_TOUCH_RECT = 0x340;            // cocos2d::Rect (node space)
constexpr uint32_t EA_TAP_DISABLED = 0x28A, EA_TAP_PENDING = 0x28E;   // bytes
constexpr uint32_t EA_ON_TAP = 0x388;                // std::function<void(EnemyActor*, bool)>
constexpr uint32_t EA_CAN_TAP = 0x358;               // std::function<bool(EnemyActor*)>
constexpr uint32_t EA_MARKER = 0x3B8, EA_MARKER_EFFECT = 0x3BC;       // nodes
constexpr uint32_t V_GET_ANIMATION = 0x298, V_ANIMATION_STOP = 0x0C;
constexpr uint32_t TAP_ACTION_MARK = 1;
}  // namespace

// PresentBoxScrollView::changeRow $_0's lambda(bool) #2 {-, view (+8), -, present id (+0x10)}(ok): the present
// received -> the header updated and the present popup; failed -> the API error dialog
static void presentReceived(uint32_t functor, uint32_t okp) {
    const uint8_t ok = ld8(okp);
    const uint32_t view = ld32(functor + 8);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        call(g2::SceneManager_updateHeader, {sceneManager()});
        const uint32_t idLo = ld32(functor + 0x10), idHi = ld32(functor + 0x14);
        Frame f;
        const uint32_t onGet = f.alloc(24);
        call(fn::function_vv_copy, {onGet, view + PRESENT_ON_GET});
        call(g2::PresentBoxScrollView_popUpGetPresent, {view, 0, idLo, idHi, onGet});
        functionDestroy(onGet);
        return;
    }
    if (!ld8(call(fn::APIManager_getInstance, {}) + API_SHOW_ERROR)) return;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_PresentError, {});
    call(g::APIManager_openErrorDialog, {api, fo, 1});
    functionDestroy(fo);
}
RW_FUNCTION(0x5102E4, "PresentBoxScrollView::changeRow $_0 lambda(bool) #2::operator()", presentReceived);

// ChatBlockListScrollView::changeRow $_2 {view, ChatBlock (+8: 12 bytes, u16 +0x14, name +0x18)}(sender, type):
// a blocked user's row -> the unblock popup (with the view's refresh callback)
static void chatUnblockTouch(uint32_t functor, uint32_t, uint32_t typep) {
    const uint32_t view = ld32(functor + 4);
    if (ld32(typep) != 2 || !ld8(view + CHAT_BLOCK_ENABLED)) return;
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t block = f.alloc(0x1C);
    st32(block, ld32(functor + 8));
    st32(block + 4, ld32(functor + 0xC));
    st32(block + 8, ld32(functor + 0x10));
    rw::save<uint16_t>(block + 0xC, rw::load<uint16_t>(functor + 0x14));
    strCopy(block + 0x10, functor + 0x18);
    const uint32_t refresh = f.alloc(24);
    call(fn::function_vv_copy, {refresh, view + CHAT_BLOCK_ON_UNBLOCK});
    call(g2::SceneManager_openUnBlockPostPopup, {sm, block, refresh});
    functionDestroy(refresh);
    strFree(block + 0x10);
}
RW_FUNCTION(0x55D0E4, "ChatBlockListScrollView::changeRow $_2::operator()", chatUnblockTouch);

// EnemyActor::setupTouchRectEff $_3 {actor}(Touch*, Event*) -> bool: a touch beginning in a living, visible,
// tappable enemy's touch rect. With a tap callback: a pending tap is delivered. Without: the marker's effect
// stops and hides; the enemy is marked (actionMarker when its marker shows) when the tap check allows it.
static uint32_t enemyTouchBegan(uint32_t functor, uint32_t touchp) {
    const uint32_t touch = ld32(touchp);
    const uint32_t actor = ld32(functor + 4);
    st8(actor + EA_TOUCHED, 0);
    if (call(g2::EnemyActor_isDied, {actor})) return 0;
    if (vcall(actor, node::isVisible, {actor}) != 1) return 0;
    if (ld8(actor + EA_TAP_DISABLED)) return 0;
    Frame f;
    const uint32_t loc = f.alloc(8), p = f.alloc(8);
    call(g2::Touch_getLocation, {loc, touch});
    call(g2::Node_convertToNodeSpace, {p, actor, loc});
    call(g2::Vec2_dtor, {loc});
    if (call(g2::Rect_containsPoint, {actor + EA_TOUCH_RECT, p}) == 1) {
        if (ld32(actor + EA_ON_TAP + 16)) {
            if (ld8(actor + EA_TAP_PENDING)) {
                st8(actor + EA_TAP_PENDING, 0);
                call(g2::FunctionActorVoid_call, {actor + EA_ON_TAP, actor, 1});
            }
        } else {
            const uint32_t marker = ld32(actor + EA_MARKER);
            if (vcall(marker, node::isVisible, {marker}) == 1) {
                const uint32_t effect = ld32(actor + EA_MARKER_EFFECT);
                const uint32_t anim = vcall(effect, V_GET_ANIMATION, {effect});
                vcall(anim, V_ANIMATION_STOP, {anim});
                vcall(ld32(actor + EA_MARKER_EFFECT), node::setVisible, {ld32(actor + EA_MARKER_EFFECT), 0});
            }
            if (ld32(actor + EA_CAN_TAP + 16) && call(g2::FunctionActorBool_call, {actor + EA_CAN_TAP, actor}) == 1) {
                const uint32_t m = ld32(actor + EA_MARKER);
                if (vcall(m, node::isVisible, {m}) == 1) call(g2::EnemyActor_actionMarker, {actor, TAP_ACTION_MARK});
                st8(actor + EA_TOUCHED, 1);
            }
        }
    }
    const uint8_t touched = ld8(actor + EA_TOUCHED);
    call(g2::Vec2_dtor, {p});
    return touched ? 1 : 0;
}
RW_FUNCTION(0x74E134, "EnemyActor::setupTouchRectEff $_3::operator() (touch began)", enemyTouchBegan);
