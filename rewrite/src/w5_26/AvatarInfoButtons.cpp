// AvatarInfoDialog::createLayout's four action buttons (A..D) on the Player Information page. A touch locks the five
// buttons (A..D, invitation) and opens a sub-popup with a "closed" callback that unlocks them again: $_72 (refresh
// the header and the profile, relayout the slide view) on the player's own page, $_73 for another player's (party
// leader actions). Own page: A = sub name, B = comment (after the popup delay; the payment info first when the API
// has none), C = play style. Another player's: B = leader change, C = sub-leader appoint / dismiss.
#include "../scene_menu/AvatarInfoDialog.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;   // (manager, bool)
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;          // (manager, float seconds)
constexpr uint32_t BaseSlideView_updateLayout = 0x525EB5;            // (view, bool)
constexpr uint32_t FunctionVB_copy = 0x45C84D;                       // std::function<void(bool)>(const&)
constexpr uint32_t FunctionVB_swap = 0x490179;
constexpr uint32_t APIManager_executePaymentInfo = 0x4CE74D;         // (api, const function<void(bool)>&)
constexpr uint32_t openPopUpSubName = 0x53506D;                      // (dialog, function<void(bool)> by value)
constexpr uint32_t openPopUpComment = 0x536481;
constexpr uint32_t openPopUpPlayStyle = 0x5376D1;
constexpr uint32_t openPopUpLeaderChange = 0x539CCD;
constexpr uint32_t openPopUpSubLeaderAppoint = 0x539F35;
constexpr uint32_t openPopUpSubLeaderDismiss = 0x53A18D;
}  // namespace g

constexpr uint32_t VSLOT_setTouchEnabled = 0x29C;      // ui::Widget
constexpr uint32_t TOUCH_ENDED = 2;
constexpr uint32_t D_OTHER_USER = 0x2C8;               // AvatarInfoDialog: another player's page
constexpr uint32_t D_SUB_LEADER = 0x2CA;               // that player is a sub leader (C dismisses)
constexpr uint32_t D_PROFILE = 0x310;                  // shared_ptr<SafeBuffer> the profile shown (+0x314 count)
constexpr uint32_t D_ON_POPUP_CLOSED = 0x480;          // std::function<void(bool)> for the comment popup
constexpr uint32_t API_USER_DETAIL = 0x94;             // shared_ptr<SafeBuffer> (+0x98 count)
constexpr uint32_t API_PAYMENT = 0x1D4;                // shared_ptr<SafeBuffer> payment info (+0x1D8 count)
constexpr uint32_t SM_POPUP_DELAY = 0x300;             // SceneManager: float, the popup open animation time
constexpr uint32_t VT_OWN_CLOSED = 0xF58804;           // $_72 {buttons A..D, invitation, dialog, view}
constexpr uint32_t VT_OTHER_CLOSED = 0xF5864C;         // $_73 (same captures)
constexpr uint32_t VT_OPEN_COMMENT = 0xF5885C;         // $_3's lambda() {dialog}
constexpr uint32_t VT_PAYMENT_DONE = 0xF58888;         // that lambda's lambda(bool) {dialog}

// a std::function<void(bool)> at `out` holding a heap "closed" lambda (`vt`, 0x20 bytes) with 7 captured words
uint32_t closedCallback(uint32_t vt, uint32_t out, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e,
                        uint32_t f, uint32_t g) {
    st32(out + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x20});
    if (obj) {
        st32(obj, vt);
        st32(obj + 4, a);
        st32(obj + 8, b);
        st32(obj + 0xC, c);
        st32(obj + 0x10, d);
        st32(obj + 0x14, e);
        st32(obj + 0x18, f);
        st32(obj + 0x1C, g);
    }
    st32(out + 16, obj);
    return out;
}
uint32_t ownClosed(uint32_t out, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g) {
    return closedCallback(VT_OWN_CLOSED, out, a, b, c, d, e, f, g);
}
uint32_t otherClosed(uint32_t out, uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e, uint32_t f, uint32_t g) {
    return closedCallback(VT_OTHER_CLOSED, out, a, b, c, d, e, f, g);
}

void setButtonsTouch(uint32_t buttons, uint32_t enabled) {
    for (uint32_t k = 0; k < 5; k++) {
        const uint32_t b = ld32(buttons + 4 * k);
        vcall(b, VSLOT_setTouchEnabled, {b, enabled});
    }
}

// opens `popup` with a "closed" callback built from the 7 captured words at `caps`
void openWith(uint32_t popup, uint32_t dialog, bool other, uint32_t caps) {
    Frame f;
    uint32_t cb = f.alloc(20);
    uint32_t w[7];
    for (uint32_t k = 0; k < 7; k++) w[k] = ld32(caps + 4 * k);
    (other ? otherClosed : ownClosed)(cb, w[0], w[1], w[2], w[3], w[4], w[5], w[6]);
    call(popup, {dialog, cb});
    functionDestroy(cb);
}

struct AvatarInfoButtons {
    // $_72 / $_73 operator()(bool): unlock the buttons, refresh the header, take the new profile, relayout
    static void ownClosedCall(uint32_t functor) {
        const uint32_t dialog = ld32(functor + 0x18);
        setButtonsTouch(functor + 4, 1);
        call(g::SceneManager_updateHeader, {sceneManager()});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t count = ld32(api + API_USER_DETAIL + 4), profile = ld32(api + API_USER_DETAIL);
        if (count) call(fn::shared_add, {count});
        st32(dialog + D_PROFILE, profile);
        const uint32_t old = ld32(dialog + D_PROFILE + 4);
        st32(dialog + D_PROFILE + 4, count);
        if (old) call(fn::shared_release, {old});
        call(g::BaseSlideView_updateLayout, {ld32(functor + 0x1C), 1});
    }

    // $_2 (button A) {+4 dialog, +8 buttons, +0x1C $_72's captures}: the sub name popup (own page only)
    static void buttonA(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
        const uint32_t dialog = ld32(functor + 4);
        if (ld32(typeRef) != TOUCH_ENDED || ld8(dialog + D_OTHER_USER)) return;
        setButtonsTouch(functor + 8, 0);
        call(g::AudioPlaySystemSEDecide, {});
        openWith(g::openPopUpSubName, dialog, false, functor + 0x1C);
    }

    // $_3 (button B) {+4 buttons, +0x18 dialog, +0x1C $_72's, +0x38 $_73's captures}: leader change, or the
    // comment popup after the open delay (the "closed" callback parked in the dialog)
    static void buttonB(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        const uint32_t dialog = ld32(functor + 0x18);
        setButtonsTouch(functor + 4, 0);
        if (ld8(dialog + D_OTHER_USER)) {
            openWith(g::openPopUpLeaderChange, dialog, true, functor + 0x38);
            return;
        }
        call(g::AudioPlaySystemSEDecide, {});
        {
            Frame f;
            uint32_t cb = f.alloc(20);
            const uint32_t c = functor + 0x1C;
            ownClosed(cb, ld32(c), ld32(c + 4), ld32(c + 8), ld32(c + 0xC), ld32(c + 0x10), ld32(c + 0x14),
                      ld32(c + 0x18));
            call(g::FunctionVB_swap, {cb, dialog + D_ON_POPUP_CLOSED});
            functionDestroy(cb);
        }
        const float delay = ldf(sceneManager() + SM_POPUP_DELAY);
        call(g::SceneManager_openNoTapScreen, {sceneManager(), fbits(delay)});
        const uint32_t wait = call(fn::DelayTime_create, {fbits(ldf(sceneManager() + SM_POPUP_DELAY))});
        Frame f;
        uint32_t then = newFunction(f, VT_OPEN_COMMENT, {dialog});
        const uint32_t done = call(fn::CallFunc_create, {then});
        call(fn::Node_runAction, {dialog, call(fn::Sequence_create, {wait, done, 0})});
        functionDestroy(then);
    }

    // $_4 (button C) {as $_3}: sub-leader dismiss / appoint (another player's page), else the play style popup
    static void buttonC(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        const uint32_t dialog = ld32(functor + 0x18);
        setButtonsTouch(functor + 4, 0);
        if (!ld8(dialog + D_OTHER_USER)) {
            openWith(g::openPopUpPlayStyle, dialog, false, functor + 0x1C);
            return;
        }
        openWith(ld8(dialog + D_SUB_LEADER) ? g::openPopUpSubLeaderDismiss : g::openPopUpSubLeaderAppoint, dialog,
                 true, functor + 0x38);
    }

    // $_3's lambda() {dialog}: the comment popup with the parked callback, or first the payment info (the comment
    // needs it) whose callback opens it
    static void openComment(uint32_t functor) {
        const uint32_t dialog = ld32(functor + 4);
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t count = ld32(api + API_PAYMENT + 4), payment = ld32(api + API_PAYMENT);
        if (count) call(fn::shared_add, {count});
        Frame f;
        if (payment) {
            uint32_t cb = f.alloc(20);
            call(g::FunctionVB_copy, {cb, dialog + D_ON_POPUP_CLOSED});
            call(g::openPopUpComment, {dialog, cb});
            functionDestroy(cb);
        } else {
            call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
            const uint32_t api2 = call(fn::APIManager_getInstance, {});
            uint32_t cb = newFunction(f, VT_PAYMENT_DONE, {dialog});
            call(g::APIManager_executePaymentInfo, {api2, cb});
            functionDestroy(cb);
        }
        if (count) call(fn::shared_release, {count});
    }
};
}  // namespace

RW_FUNCTION(0x54CAD4, "AvatarInfoDialog::createLayout::$_2 operator() (button A)", AvatarInfoButtons::buttonA);
RW_FUNCTION(0x54C460, "AvatarInfoDialog::createLayout::$_3 operator() (button B)", AvatarInfoButtons::buttonB);
RW_FUNCTION(0x54C070, "AvatarInfoDialog::createLayout::$_4 operator() (button C)", AvatarInfoButtons::buttonC);
RW_FUNCTION(0x54C326, "AvatarInfoDialog::createLayout::$_72 operator()", AvatarInfoButtons::ownClosedCall);
RW_FUNCTION(0x54C6D8, "AvatarInfoDialog::createLayout::$_3::lambda() operator()", AvatarInfoButtons::openComment);
RW_FUNCTION(0x54C244, "sub_54C244 (createLayout $_72 std::function)", ownClosed);
RW_FUNCTION(0x54AE30, "sub_54AE30 (createLayout $_73 std::function)", otherClosed);
