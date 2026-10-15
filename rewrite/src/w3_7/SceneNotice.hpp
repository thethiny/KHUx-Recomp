// SceneNotice: the notice ("information") screen shown after login. It walks the list of notice URLs the server
// sent (APIManager + 0x1C0, a std::vector<std::string>) from the last one to the first, opening each in a popup
// (Information2_Base.json) with a WebView inside its scroll area; closing a popup opens the next one, and after
// the last one the game moves on (to the login bonus, the union ranking or my page - init's $_4 lambda).
// A cocos2d::Layer with three fields of its own.
#pragma once

#include "../scene_social/SocialCommon.hpp"

struct SceneNotice {
    using Addr = uint32_t;

    const uint8_t layer[0x288];        // cocos2d::Layer (its onEnter / onExit std::functions at +0x210 / +0x228)
    rw::Field<uint32_t> root;          // +0x288 cocos2d::Node shifted right by the side margin
    rw::Field<uint32_t> webView;       // +0x28C cocos2d::plugin::WebView of the open notice popup
    rw::Field<int32_t> shownCount;     // +0x290 notices opened so far (the next one is url[count - shown - 1])
    const uint8_t pad_294[4];

    static constexpr uint32_t SIZE = 0x298;
    static constexpr uint32_t GOT_VTABLE = 0xFD68F8;   // GOT slot: &_ZTV11SceneNotice (the vptr is that + 8)

    static Addr create();
    static Addr createScene();
    static uint32_t destroy(SceneNotice *self);
    static void destroyDelete(SceneNotice *self);
    static uint32_t init(SceneNotice *self);
    static void callPrepareAPI(SceneNotice *self, Addr callback);
    static void runOutAnimation(SceneNotice *self, Addr done);
    static void openNextPage(SceneNotice *self, Addr done);
    static void update(SceneNotice *self, float dt);
    static void runInAnimation(SceneNotice *self);
    static void openWebViewInfoPopup(SceneNotice *self, Addr title, Addr url, Addr onClosed);
};
RW_LAYOUT(SceneNotice, root, 0x288);
RW_LAYOUT(SceneNotice, webView, 0x28C);
RW_LAYOUT(SceneNotice, shownCount, 0x290);
RW_SIZE(SceneNotice, SceneNotice::SIZE);

// The std::function __func vtables (+8) of SceneNotice's lambdas.
namespace SceneNoticeVt {
constexpr uint32_t OpenNextPage9 = 0xF6D2F4;      // openNextPage $_9 {this, function<void()> done}: void(Node*)
constexpr uint32_t PopupClosed8 = 0xF6D320;       // openWebViewInfoPopup $_8 {shadow, panel, done, popupLayer}: void(bool)
constexpr uint32_t PopupRemove8 = 0xF6D34C;       // $_8::{lambda()#1} {done, panel, popupLayer, shadow}: void()
constexpr uint32_t PopupCanClose7 = 0xF6D378;     // $_7 {closeButton}: bool()
constexpr uint32_t PopupOpened6 = 0xF6D3A4;       // $_6 {this, scrollArea, url}: void()
constexpr uint32_t WebViewLoaded6 = 0xF6D3D0;     // $_6::{lambda(WebView*, std::string)#1}
constexpr uint32_t CloseTouch5 = 0xF6D3FC;        // $_5: Button_Close touch
constexpr uint32_t InitAllShown4 = 0xF6D428;      // init $_4 {this}: void() (after the last notice)
constexpr uint32_t InitAllShown4Next = 0xF6D454;  // init $_4::{lambda()#1}
constexpr uint32_t InitSkip3 = 0xF6D480;          // init $_3: void() (tutorial not finished)
constexpr uint32_t InitSkip3Done = 0xF6D4AC;      // init $_3 ...::{lambda(bool)#1}
constexpr uint32_t InitOnExit2 = 0xF6D4D8;        // init $_2 {this}: onExit
constexpr uint32_t InitOnEnter1 = 0xF6D504;       // init $_1 {this}: onEnter
constexpr uint32_t PrepareDone0 = 0xF6D530;       // callPrepareAPI $_0: void(bool)
}  // namespace SceneNoticeVt
