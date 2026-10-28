// AvatarInfoDialog: the Player Information dialog (a player's avatar, titles, records, party actions), opened from
// MyPage's Other menu (the player) or from a party member / ranking entry (another player). open asks the server
// for the profile (/user/profile, executeUserProfileGet); its callback builds the page (createLayout).
#pragma once

#include "../scene/SceneUi.hpp"

// Helpers shared by createLayout and the slide view's page updater (AvatarInfoDialog_pages.cpp)
namespace aid {
constexpr uint32_t SafeBuffer_read = 0x45A269;               // (sb, dst, src, len, offset)
constexpr uint32_t ImageButton_create = 0x531239;            // (string, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;     // (Node*, float)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t S_Empty = 0xDF8A1A;                       // ""

// SafeBuffer::read of len bytes at offset into dst
inline void sbRead(uint32_t sb, uint32_t dst, uint32_t offset, uint32_t len) {
    rw::call(SafeBuffer_read, {sb, dst, scene::ld32(sb + 0xC) + offset, len, offset});
}

// the scene manager, API manager and dialog state the page lambdas share
constexpr uint32_t SM_POPUP_DELAY = 0x300;                   // SceneManager: float, the popup open animation time
constexpr uint32_t SM_LOCKED = 0x309;                        // SceneManager: uint8
constexpr uint32_t SM_PAYMENT_WAIT = 0x4CC, SM_ON_PAYMENT = 0x548;   // uint8, std::function<void(bool)>
constexpr uint32_t API_SHOW_ERROR = 0x14;                    // APIManager: uint8, the error dialog is wanted
constexpr uint32_t API_PAYMENT = 0x1D4;                      // APIManager: shared_ptr<SafeBuffer> payment info
constexpr uint32_t D_ON_POPUP_CLOSED = 0x480;                // AvatarInfoDialog: std::function<void(bool)>
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;   // (sm, bool)
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;          // (sm, float seconds)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;            // (api, function<void()>, bool)
constexpr uint32_t APIManager_executePaymentInfo = 0x4CE74D;         // (api, const function<void(bool)>&)
constexpr uint32_t FunctionVB_assign = 0x490109;                     // std::function<void(bool)>::operator=

// the payment info from the server first (the popups need the player's age): the scene manager's payment
// callback emptied, the loading indicator on, then /payment/info with the lambda {dialog} (`vt`) as its callback
inline void requestPaymentInfo(uint32_t dialog, uint32_t vt) {
    const uint32_t sm = scene::sceneManager();
    scene::st8(sm + SM_PAYMENT_WAIT, 1);
    scene::Frame f;
    const uint32_t none = scene::newEmptyFunction(f);
    rw::call(FunctionVB_assign, {sm + SM_ON_PAYMENT, none});
    scene::functionDestroy(none);
    rw::call(SceneManager_enableLoadingIndicator, {scene::sceneManager(), 1});
    const uint32_t api = rw::call(APIManager_getInstance, {});
    const uint32_t cb = scene::newFunction(f, vt, {dialog});
    rw::call(APIManager_executePaymentInfo, {api, cb});
    scene::functionDestroy(cb);
}

// the API error dialog with a capture-less callback (`vt`), when the API manager wants one
inline void apiErrorDialog(uint32_t vt) {
    if (!scene::ld8(rw::call(APIManager_getInstance, {}) + API_SHOW_ERROR)) return;
    const uint32_t api = rw::call(APIManager_getInstance, {});
    scene::Frame f;
    const uint32_t cb = scene::newFunction(f, vt, {});
    rw::call(APIManager_openErrorDialog, {api, cb, 1});
    scene::functionDestroy(cb);
}

// after the popup open time (taps blocked meanwhile): the lambda {dialog} (`vt`), through the dialog's actions
inline void afterPopupDelay(uint32_t dialog, uint32_t vt) {
    const uint32_t sm = scene::sceneManager();
    rw::call(SceneManager_openNoTapScreen, {sm, scene::ld32(scene::sceneManager() + SM_POPUP_DELAY)});
    const uint32_t wait = rw::call(scene::fn::DelayTime_create, {scene::ld32(scene::sceneManager() + SM_POPUP_DELAY)});
    scene::Frame f;
    const uint32_t then = scene::newFunction(f, vt, {dialog});
    const uint32_t done = rw::call(scene::fn::CallFunc_create, {then});
    rw::call(scene::fn::Node_runAction, {dialog, rw::call(scene::fn::Sequence_create, {wait, done, 0})});
    scene::functionDestroy(then);
}
}  // namespace aid

struct AvatarInfoDialog {
    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x4C8;

    rw::Field<uint32_t> vtable;             // +0x000 cocos2d::Node
    const uint8_t pad_004[0x26C];
    rw::Field<uint32_t> vtable2;            // +0x270 cocos2d::extension::EditBoxDelegate (name / message fields)
    rw::Field<uint32_t> root;               // +0x274 the AvatarInfoScene_A layout
    rw::Field<uint32_t> parent;             // +0x278 set by open (0: closed)
    rw::Field<uint32_t> shadow;             // +0x27C
    const uint8_t onClose[0x18];            // +0x280 std::function<void(bool)>
    const uint8_t skills[0xC];              // +0x298 std::vector<UserSkill::Skill>
    rw::Field<uint8_t> field_2A4;           // +0x2A4
    const uint8_t pad_2A5[3];
    const uint8_t canClose[0x18];           // +0x2A8 std::function<bool()>
    rw::Field<uint32_t> userIdLo;           // +0x2C0 the player shown (another player only)
    rw::Field<uint32_t> userIdHi;           // +0x2C4
    rw::Field<uint8_t> isOtherUser;         // +0x2C8
    rw::Field<uint8_t> viewerFlag;          // +0x2C9 the viewer's setting byte 0x11 (UserSetting)
    rw::Field<uint8_t> field_2CA;           // +0x2CA
    const uint8_t pad_2CB[0x18D];
    const uint8_t pendingPopup[0x18];       // +0x458 std::function<void(bool)> of an open sub-popup
    const uint8_t pad_470[0x54];
    rw::Field<uint32_t> imageDownloader;    // +0x4C4 ImageDownloader* (owned)

    static Addr create();
    static AvatarInfoDialog *construct(AvatarInfoDialog *self);
    static uint32_t init(AvatarInfoDialog *self);
    static AvatarInfoDialog *destruct(AvatarInfoDialog *self);
    static uint32_t open(AvatarInfoDialog *self, Addr parent, uint32_t idLo, uint32_t idHi,
                         Addr onClose, uint32_t flag, Addr canClose);
    static void close(AvatarInfoDialog *self);
    static void setTouchEnabledUnderUIButtons(AvatarInfoDialog *self, uint32_t enabled);
};
RW_LAYOUT(AvatarInfoDialog, vtable2, 0x270);
RW_LAYOUT(AvatarInfoDialog, shadow, 0x27C);
RW_LAYOUT(AvatarInfoDialog, field_2A4, 0x2A4);
RW_LAYOUT(AvatarInfoDialog, userIdLo, 0x2C0);
RW_LAYOUT(AvatarInfoDialog, field_2CA, 0x2CA);
RW_LAYOUT(AvatarInfoDialog, imageDownloader, 0x4C4);
RW_SIZE(AvatarInfoDialog, 0x4C8);
