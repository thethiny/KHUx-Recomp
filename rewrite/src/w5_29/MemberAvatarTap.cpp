// MemberScrollView::changeRow $_0: a row's avatar. With a select callback set (the member list) the row's user is
// kept at +0x6E0 and the callback runs after the no-tap delay; otherwise (e.g. the recruit search) the user's
// avatar info popup opens after it, from the id kept at +0x6E8.
// Also SceneManager::openPartyNewPopup's re-show helper (sub_620D10).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_openNoTapScreen = 0x61FB25;          // (sm, float seconds)
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create, CallFunc_create = rwlib::cocos2d::CallFunc::create, Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
}  // namespace g

constexpr uint32_t SM_NO_TAP_TIME = 0x300;     // SceneManager: float, the no-tap screen's seconds
constexpr uint32_t LIST_ON_SELECT = 0x650;     // MemberScrollView: std::function<void(ull)> (callable at +0x660)
constexpr uint32_t F_SELECTED = 0x6E0;         // the user handed to the select callback
constexpr uint32_t F_AVATAR_USER = 0x6E8;      // the user whose avatar info opens
constexpr uint32_t F_TYPE = 0x6F0;
constexpr uint32_t VT_Select = 0xF57020;       // $_0::lambda()#1 {view}
constexpr uint32_t VT_AvatarInfo = 0xF56FF4;   // $_0::lambda()#2 {view}
constexpr uint32_t V_setScale = 0x40;

// the no-tap screen, then `vt` {view} after its delay
void afterNoTap(uint32_t view, uint32_t vt) {
    const uint32_t sm = sceneManager();
    call(g::SM_openNoTapScreen, {sm, ld32(sceneManager() + SM_NO_TAP_TIME)});
    const uint32_t delay = call(g::DelayTime_create, {ld32(sceneManager() + SM_NO_TAP_TIME)});
    Frame f;
    const uint32_t then = newFunction(f, vt, {view});
    const uint32_t cf = call(g::CallFunc_create, {then});
    call(g::Node_runAction, {view, call(g::Sequence_create, {delay, cf, 0})});
    functionDestroy(then);
}

// $_0 {-, avatar, view, selected user, avatar user}
void avatarTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t view = ld32(functor + 12);
    const uint32_t avatar = ld32(functor + 8);
    if (vcall(avatar, node::isVisible, {avatar}) != 1) return;
    call(g::AudioPlaySystemSEDecide, {});
    if (ld32(view + LIST_ON_SELECT + 0x10)) {
        st32(view + F_SELECTED + 4, ld32(functor + 20));
        st32(view + F_SELECTED, ld32(functor + 16));
        afterNoTap(view, VT_Select);
    } else {
        st32(view + F_AVATAR_USER + 4, ld32(functor + 28));
        st32(view + F_AVATAR_USER, ld32(functor + 24));
        afterNoTap(view, VT_AvatarInfo);
    }
}

// $_0::lambda()#2 {view}: the avatar info popup (the member list type: with the profile buttons)
void openAvatarInfo(uint32_t functor) {
    const uint32_t view = ld32(functor + 4);
    Frame f;
    const uint32_t callback = newEmptyFunction(f), canClose = newEmptyFunction(f);
    SceneUtil::openAvatarInfoPopup(uint64_t(ld32(view + F_AVATAR_USER + 4)) << 32 | ld32(view + F_AVATAR_USER),
                                   ld32(view + F_TYPE) == 0 ? 1u : 0u, callback, canClose);
    functionDestroy(canClose);
    functionDestroy(callback);
}

// sub_620D10 {popup, shadow, opacity}: openPartyNewPopup shows its popup again (sound, scale 1, the shadow fading
// in, the open animation)
void showPartyNewPopup(uint32_t captures) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t popup = ld32(captures);
    vcall(popup, node::setVisible, {popup, 1});
    vcall(popup, V_setScale, {popup, fbits(1.0f)});
    const uint32_t shadow = ld32(captures + 4);
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), ld8(captures + 8)})});
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    call(fn::Node_runAction, {ld32(captures), SceneUtil::createOpenPopupAnimation(ld32(captures), none)});
    functionDestroy(none);
}
}  // namespace

RW_FUNCTION(0x5250B8, "MemberScrollView::changeRow::$_0 operator() (avatar)", avatarTouched);
RW_FUNCTION(0x525320, "MemberScrollView::changeRow::$_0::lambda()#2 operator() (avatar info)", openAvatarInfo);
RW_FUNCTION(0x620D10, "SceneManager::openPartyNewPopup show popup helper (sub_620D10)", showPartyNewPopup);
