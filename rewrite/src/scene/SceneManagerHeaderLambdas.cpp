// SceneManager: the header buttons' touch lambdas (createHeader $_9 .. $_13), the notice URL opener (update $_8)
// and moveToTitleForcibly's scene change ($_6). A header button acts on touch ended once the footer menu takes
// touches (+0x588) and the tutorial is over: Raid (the raid list scene), Chat (the chat popup), the avatar (the
// player's own info popup), Jewels (the Buy Jewels dialog); the AP gauge shows its tip while touched.
#include "SceneUi.hpp"
#include "../actor/FunctionThunks.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneManager_isOpenedPopupOnly = 0x62158D;
constexpr uint32_t SceneManager_closeGlobalMenu = 0x611BD5;
constexpr uint32_t SceneManager_openChatPopup = 0x613E9D;
constexpr uint32_t APIManager_executeRaidList = 0x4CC009;       // (api, const function<void(bool)>&)
constexpr uint32_t SceneUtil_openAvatarInfoPopup = 0x46A229;   // (u64 userId, bool, function<void(bool)>, function<bool()>)
constexpr uint32_t BuyMagicStoneDialog_open = 0x588CAD;        // (const function<void()>&, bool)
constexpr uint32_t FunctionBool_call = 0x51022D;               // std::function<bool()>::operator()
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t FadeIn_create = 0x86B115, FadeOut_create = 0x86B25D;   // (float)
constexpr uint32_t Application_getInstance = 0xA3E7D5, Application_openURL = 0xA3EAC9;   // (app, const char*)
constexpr uint32_t GOT_CREATE_RAID_LIST = 0xFD66A8, GOT_CREATE_TITLE = 0xFD68B4;   // &createSceneLayer<X>
}  // namespace g

constexpr uint32_t VT_JewelsClosed = 0xF60794;       // $_10's BuyMagicStoneDialog::open callback {button}
constexpr uint32_t VT_RaidButton = 0xF606B8;         // $_13 {this, button}
constexpr uint32_t VT_ChatButton = 0xF606E4;         // $_12 {this}
constexpr uint32_t VT_AvatarButton = 0xF60710;       // $_11 {this}
constexpr uint32_t VT_AvatarCanClose = 0xF6073C;     // $_11's openAvatarInfoPopup function<bool()> {this}
constexpr uint32_t SM_MENU_READY = 0x588;            // byte: the footer menu (and the header) take touches
constexpr uint32_t SM_MENU_SHOWN = 0x368;            // byte: the footer menu is open
constexpr uint32_t SM_CHAT_BLOCKED = 0x470;          // byte
constexpr uint32_t SM_CAN_OPEN_CHAT = 0x518;         // std::function<bool()>
constexpr uint32_t SM_CAN_CHANGE = 0x4D0;            // std::function<bool()>
constexpr uint32_t SM_TITLE_FORCED = 0x31F;          // byte
constexpr uint32_t APP_TITLE_FORCED = 0xE5;          // AppState: byte
constexpr uint32_t API_NOTICE_URL = 0x1A8;           // APIManager: shared_ptr<...> with the URL string at +0x6C
constexpr uint32_t NOTICE_URL = 0x6C;
constexpr uint32_t V_IS_VISIBLE = 0xB4, V_SET_TOUCH_ENABLED = 0x29C;
constexpr uint32_t TOUCH_BEGAN = 0, TOUCH_MOVED = 1, TOUCH_ENDED = 2;
constexpr uint32_t TUTORIAL_PHASE_DONE = 995;
constexpr float TIP_FADE = 0.3f, TIP_SHOWN = 3.0f;

bool tutorialDone() { return int32_t(call(g::SceneUtil_getNexTutorialphase, {})) >= int32_t(TUTORIAL_PHASE_DONE); }
// the header takes touches, the touch ended and the tutorial is over (the original asks the phase first)
bool headerTapped(uint32_t self, uint32_t typep) {
    if (!ld8(self + SM_MENU_READY)) return false;
    const uint32_t type = ld32(typep);
    const bool done = tutorialDone();
    return type == TOUCH_ENDED && done;
}
}  // namespace

// $_13 {this, button}: Raid (the button shown, no popup open) -> the footer menu closed, the header off, the raid
// list (its API with no callback first), faded in
static void raidButton(uint32_t functor, uint32_t, uint32_t typep) {
    const uint32_t self = ld32(functor + 4);
    if (!headerTapped(self, typep)) return;
    const uint32_t button = ld32(functor + 8);
    if (vcall(button, V_IS_VISIBLE, {button}) != 1) return;
    if (call(g::SceneManager_isOpenedPopupOnly, {self})) return;
    call(g::AudioPlaySystemSEDecide, {});
    if (ld8(self + SM_MENU_SHOWN)) call(g::SceneManager_closeGlobalMenu, {self});
    const uint32_t sm = sceneManager();
    call(g::SceneUtil_getNexTutorialphase, {});
    st8(sm + SM_MENU_READY, 0);
    apiBusyDropPayment();
    {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(g::APIManager_executeRaidList, {api, none});
        functionDestroy(none);
    }
    changeSceneLayerTo(g::GOT_CREATE_RAID_LIST, 1, 1, 1);
}

// $_12 {this}: Chat -> the chat popup (unless blocked, or the manager's chat check says no)
static void chatButton(uint32_t functor, uint32_t, uint32_t typep) {
    const uint32_t self = ld32(functor + 4);
    if (!headerTapped(self, typep)) return;
    call(g::AudioPlaySystemSEDecide, {});
    if (ld8(self + SM_CHAT_BLOCKED)) return;
    if (ld32(self + SM_CAN_OPEN_CHAT + 16) && call(g::FunctionBool_call, {self + SM_CAN_OPEN_CHAT}) != 1) return;
    call(g::SceneManager_openChatPopup, {self});
}

// $_11 {this}: the avatar -> the player's own info popup (user 0, closable while the scene may change)
static void avatarButton(uint32_t functor, uint32_t, uint32_t typep) {
    const uint32_t self = ld32(functor + 4);
    if (!headerTapped(self, typep)) return;
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t closed = newEmptyFunction(f);
    const uint32_t canClose = newFunction(f, VT_AvatarCanClose, {self});
    call(g::SceneUtil_openAvatarInfoPopup, {0, 0, 1, closed, canClose});
    functionDestroy(canClose);
    functionDestroy(closed);
}

// $_11's function<bool()> {this}: the manager's "may the scene change" check, true without one
static uint32_t avatarCanClose(uint32_t functor) {
    const uint32_t self = ld32(functor + 4);
    if (!ld32(self + SM_CAN_CHANGE + 16)) return 1;
    return call(g::FunctionBool_call, {self + SM_CAN_CHANGE}) == 1 ? 1 : 0;
}

// $_10 {this, button}: Jewels -> the button off until the Buy Jewels dialog closes
static void jewelsButton(uint32_t functor, uint32_t, uint32_t typep) {
    if (!headerTapped(ld32(functor + 4), typep)) return;
    const uint32_t button = ld32(functor + 8);
    vcall(button, V_SET_TOUCH_ENABLED, {button, 0});
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t closed = newFunction(f, VT_JewelsClosed, {ld32(functor + 8)});
    call(g::BuyMagicStoneDialog_open, {closed, 0});
    functionDestroy(closed);
}

// $_9 {this, tip}: the AP gauge -> its tip fades in on touch began and out 3 s after the touch ends
static void apTip(uint32_t functor, uint32_t, uint32_t typep) {
    if (!ld8(ld32(functor + 4) + SM_MENU_READY)) return;
    const uint32_t type = ld32(typep);
    if (!tutorialDone() || type == TOUCH_MOVED) return;
    uint32_t action;
    if (type == TOUCH_BEGAN) {
        call(g::Node_stopAllActions, {ld32(functor + 8)});
        action = call(g::FadeIn_create, {fbits(TIP_FADE)});
    } else if ((type & ~1u) == TOUCH_ENDED) {   // ended or cancelled
        const uint32_t delay = call(fn::DelayTime_create, {fbits(TIP_SHOWN)});
        action = call(fn::Sequence_create, {delay, call(g::FadeOut_create, {fbits(TIP_FADE)}), 0});
    } else {
        return;
    }
    call(fn::Node_runAction, {ld32(functor + 8), action});
}

// update $_8: the notice's URL opened in the browser
static void openNoticeUrl(uint32_t) {
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + API_NOTICE_URL + 4);
    const uint32_t obj = ld32(api + API_NOTICE_URL);
    if (ctrl) call(fn::shared_add, {ctrl});
    if (obj) {
        Frame f;
        const uint32_t url = f.alloc(12);
        strCopy(url, obj + NOTICE_URL);
        call(g::Application_openURL, {call(g::Application_getInstance, {}), strData(url)});
        strFree(url);
    }
    if (ctrl) call(fn::shared_release, {ctrl});
}

// moveToTitleForcibly $_6 {callback +8, this +0x20}: the callback, the title forced (manager and app state; the
// tutorial popup 0 closed meanwhile), then the title scene faded in
static void toTitle(uint32_t functor) {
    const uint32_t self = ld32(functor + 0x20);
    if (ld32(functor + 0x18)) call(fn::function_vv_call, {functor + 8});
    st8(self + SM_TITLE_FORCED, 1);
    st8(call(fn::AppState_getInstance, {}) + APP_TITLE_FORCED, 1);
    execTutorialCloseIfSet(0);
    st8(call(fn::AppState_getInstance, {}) + APP_TITLE_FORCED, 0);
    changeSceneLayerTo(g::GOT_CREATE_TITLE, 1, 1, 1);
}

RW_FUNCTION(0x637198, "SceneManager::createHeader::$_13::operator() (Raid)", raidButton);
RW_FUNCTION(0x6373D0, "SceneManager::createHeader::$_12::operator() (Chat)", chatButton);
RW_FUNCTION(0x6374B8, "SceneManager::createHeader::$_11::operator() (avatar)", avatarButton);
RW_FUNCTION(0x637646, "SceneManager::createHeader::$_11 can-close lambda::operator()", avatarCanClose);
RW_FUNCTION(0x637708, "SceneManager::createHeader::$_10::operator() (Jewels)", jewelsButton);
RW_FUNCTION(0x63793C, "SceneManager::createHeader::$_9::operator() (AP tip)", apTip);
RW_FUNCTION(0x637A40, "SceneManager::update::$_8::operator() (notice URL)", openNoticeUrl);
RW_FUNCTION(0x637D74, "SceneManager::moveToTitleForcibly::$_6::operator()", toTitle);
RW_FUNCTION(0x637138, "SceneManager::createHeader::$_13 __clone()", (fnthunk::cloneWords<VT_RaidButton, 0xC, 2>));
RW_FUNCTION(0x637378, "SceneManager::createHeader::$_12 __clone()", (fnthunk::cloneWords<VT_ChatButton, 8, 1>));
RW_FUNCTION(0x637460, "SceneManager::createHeader::$_11 __clone()", (fnthunk::cloneWords<VT_AvatarButton, 8, 1>));
RW_FUNCTION(0x6375F0, "SceneManager::createHeader::$_11 can-close lambda __clone()",
            (fnthunk::cloneWords<VT_AvatarCanClose, 8, 1>));
