// SceneTakeOver: shown after the terms' Agree when the login's user data asks for a take-over (flags +0x34 bit
// 0x10). "Take over" opens the take-over web page (the system data's URL at +0x18, formatted with the device UUID
// and 2) in an info popup with a WebView - unless the user flags +0x35 bit 2 is set, then the button gets an empty
// handler; "Intro" (start anew) logs out of Facebook first when logged in, else goes to the birthday input.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base, Layer_init = rwlib::cocos2d::Layer::init, Scene_create = rwlib::cocos2d::Scene::create, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t SceneTakeOver_create = 0x71244D, SceneTakeOver_init = 0x712501;
constexpr uint32_t SceneTakeOver_openWebViewInfoPopup = 0x712CED;
constexpr uint32_t function_vv_assign = 0x4EB9CD, SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr uint32_t SceneManager_setEnabledTouchEffect = 0x60CF65, SceneManager_openPopup = 0x61524D;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61, SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_getUUIDString = 0x4EA8E1;
constexpr uint32_t StringBuffer_ctor = 0x49140D, Text_setString = rwlib::cocos2d::ui::Text::setString, libc_memcpy = 0x40E164;
constexpr uint32_t WebView_create = rwlib::cocos2d::plugin::WebView::create, WebView_setScalesPageToFit = rwlib::cocos2d::plugin::WebView::setScalesPageToFit, WebView_loadUrl = rwlib::cocos2d::plugin::WebView::loadUrl;
constexpr uint32_t WebView_stopLoading = rwlib::cocos2d::plugin::WebView::stopLoading, function_webview_swap = 0x4821B5;
constexpr uint32_t function_node_copy = 0x474D99, function_node_copy2 = 0x481F0D;   // function<void(Node*)>
constexpr uint32_t FacebookManager_getInstance = 0x4E5585, FacebookManager_logout = 0x4E5751;
constexpr uint32_t SceneBirthRegister_changeSceneLayer = 0x711CD5;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t Vec2_ctor_ = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor_ = rwlib::cocos2d::Vec2::dtor_base, Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t GOT_SceneTakeOver_vtable = 0xFD697C, GOT_POPUP_FADE = 0xFD65C0;
constexpr uint32_t S_Json = 0xE06A87, S_InfoJson = 0xDF9B2F, S_TxtTitle = 0xDFF142, S_TxtWording = 0xDF862F;
constexpr uint32_t S_ButtonTakeover = 0xE06ABA, S_TxtTakeover = 0xE06ACA, S_ButtonIntro = 0xE06AD7;
constexpr uint32_t S_TxtIntro = 0xE06AE4, S_TxtTitleLabel = 0xDF9554, S_ScrollArea = 0xDF9B59;
constexpr uint32_t S_ButtonClose = 0xDF8692;
constexpr uint32_t VT_OnEnter = 0xF6B488, VT_OnExit = 0xF6B45C, VT_BackKey = 0xF6B430;   // {this}
constexpr uint32_t VT_TakeoverPage = 0xF6B2A4;            // {this, the formatted URL (0x404 bytes)} (heap 0x410)
constexpr uint32_t VT_TakeoverNone = 0xF6B2FC;            // {byte 0}
constexpr uint32_t VT_IntroTouch = 0xF6B24C, VT_InfoPopupClosed = 0xF6B2D0;
constexpr uint32_t VT_FacebookLoggedOut = 0xF6B278, VT_PageEvent = 0xF6B220;
constexpr uint32_t VT_OpenDone = 0xF6B1C8;                // {this, URL string, Close} (heap 0x18)
constexpr uint32_t VT_CloseTouch = 0xF6B1F4, VT_CanClose = 0xF6B19C;
constexpr uint32_t VT_Closed = 0xF6B144;                  // {Close, this, shadow, root, function, layer} (heap 0x38)
constexpr uint32_t VT_CloseDone = 0xF6B170;               // {function, root, layer, shadow} (heap 0x30)
constexpr uint32_t V_release = 4, V_addChild = 0xF0, V_setPosition = 0x4C, V_getPosition = 0x54;
constexpr uint32_t V_setPositionX = 0x64, V_setPositionY = 0x6C, V_setAnchorPoint = 0x9C;
constexpr uint32_t V_setContentSize = 0xA8, V_getContentSize = 0xAC, V_setVisible = 0xB0;
constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214, V_setTouchEnabled = 0x29C;
// SceneTakeOver: +0x210 / +0x228 the enter / exit callbacks, +0x288 the layout, +0x28C the info popup's WebView
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228, F_ROOT = 0x288, F_WEBVIEW = 0x28C;
constexpr uint32_t API_SYSTEM = 0x1A8, SYSTEM_TAKEOVER_URL = 0x18, API_USER = 0x44, USER_FLAGS2 = 0x35;
constexpr uint32_t WEBVIEW_PAGE_CALLBACK = 0x368;

float f32(uint32_t bits) { float f; std::memcpy(&f, &bits, 4); return f; }
void assign(uint32_t member, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(function_vv_assign, {member, fo});
    functionDestroy(fo);
}
void labelText(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(node, name, len);
    SceneUtil::setShadow(text);
    setTextId(text, id);
}
void strCopy(uint32_t dst, uint32_t src) {
    if (strIsLong(src)) strInit(dst, ld32(src + 8), ld32(src + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
}
}  // namespace

namespace scene_takeover {

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneTakeOver_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x712428, "SceneTakeOver::createScene", scene_takeover::createScene);

uint32_t create(uint32_t) {
    const uint32_t self = call(fn::operator_new, {0x290});
    call(Layer_ctor, {self});
    st32(self, ld32(GOT_SceneTakeOver_vtable) + 8);
    st32(self + F_WEBVIEW, 0);
    if (call(SceneTakeOver_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x71244C, "SceneTakeOver::create", scene_takeover::create);

uint32_t init(uint32_t self) {
    if (call(Layer_init, {self}) != 1) return 0;
    assign(self + F_ON_ENTER, VT_OnEnter, self);
    assign(self + F_ON_EXIT, VT_OnExit, self);
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t fo = newFunction(f, VT_BackKey, {self});
        call(SceneManager_setOnBackKeyCallback, {sm, fo});
        functionDestroy(fo);
    }
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Json});
    st32(self + F_ROOT, root);
    vcall(root, V_setPositionX, {root, call(SceneUtil_getSideMargin, {ld32(root)})});
    {
        const uint32_t r = ld32(self + F_ROOT);
        vcall(r, V_setPositionY, {r, call(SceneUtil_getSideHeightMargin, {ld32(r)})});
    }
    vcall(self, V_addChild, {self, ld32(self + F_ROOT)});
    labelText(ld32(self + F_ROOT), S_TxtTitle, 9, 109120001);
    labelText(ld32(self + F_ROOT), S_TxtWording, 0xB, 109120002);
    // the take-over page: the system data's URL with the device UUID and 2
    Frame f;
    const uint32_t url = f.alloc(12);
    strCopy(url, ld32(call(fn::APIManager_getInstance, {}) + API_SYSTEM) + SYSTEM_TAKEOVER_URL);
    const uint32_t page = f.alloc(0x404);
    {
        Frame g;
        const uint32_t uuid = g.alloc(12);
        call(AppState_getUUIDString, {uuid, call(AppState_getInstance, {})});
        call(StringBuffer_ctor, {page, strData(url), strData(uuid), 2});
        strFree(uuid);
    }
    const uint32_t takeover = findNode(ld32(self + F_ROOT), S_ButtonTakeover, 0xF);
    const uint32_t user = ld32(call(fn::APIManager_getInstance, {}) + API_USER);
    if (user && (ld8(user + USER_FLAGS2) & 2)) {
        Frame g;
        const uint32_t fo = g.alloc(20);
        st32(fo, VT_TakeoverNone);
        st8(fo + 4, 0);
        st32(fo + 16, fo);
        call(fn::Widget_addTouchEventListener, {takeover, fo});
        functionDestroy(fo);
    } else {
        Frame g;
        const uint32_t copy = g.alloc(0x404);
        call(libc_memcpy, {copy, page, 0x404});
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = call(fn::operator_new, {0x410});
        if (obj) {
            st32(obj, VT_TakeoverPage);
            st32(obj + 4, self);
            call(libc_memcpy, {obj + 8, copy, 0x404});
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {takeover, fo});
        functionDestroy(fo);
    }
    labelText(takeover, S_TxtTakeover, 0xC, 109120003);
    const uint32_t intro = findNode(ld32(self + F_ROOT), S_ButtonIntro, 0xC);
    addTouch(intro, VT_IntroTouch, {});
    labelText(intro, S_TxtIntro, 9, 109100002);
    strFree(url);
    return 1;
}
RW_FUNCTION(0x712500, "SceneTakeOver::init", scene_takeover::init);

// the info popup: `title`, a WebView in the scroll area that loads `url` once the popup is open, Close; closing
// calls `closed(node)`. Only one at a time (+0x28C); 1 when opened.
uint32_t openWebViewInfoPopup(uint32_t self, uint32_t layer, uint32_t title, uint32_t url, uint32_t closed) {
    if (ld32(self + F_WEBVIEW)) return 0;
    call(SceneManager_setEnabledTouchEffect, {sceneManager(), 0});
    call(AudioPlaySystemSEPopUp, {});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {0x83});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), opacity})});
    vcall(layer, V_addChild, {layer, shadow});
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_InfoJson});
    vcall(root, V_setTouchEnabled, {root, 0});
    vcall(shadow, V_addChild, {shadow, root});
    {
        Frame g;
        const uint32_t anchor = g.alloc(8);
        call(Vec2_ctor_, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(root, V_setAnchorPoint, {root, anchor});
        call(Vec2_dtor_, {anchor});
    }
    vcall(root, V_setPosition, {root, vcall(shadow, V_getPosition, {shadow})});
    {
        const uint32_t label = findNode(root, S_TxtTitleLabel, 0xF);
        SceneUtil::setShadow(label);
        Frame g;
        const uint32_t s = newStr(g, strData(title));
        call(Text_setString, {label, s});
        strFree(s);
    }
    const uint32_t area = findNode(root, S_ScrollArea, 0xB);
    const uint32_t web = call(WebView_create, {});
    st32(self + F_WEBVIEW, web);
    {
        const uint32_t w = ld32(vcall(area, V_getContentSize, {area}));
        const uint32_t h = ld32(vcall(area, V_getContentSize, {area}) + 4);
        Frame g;
        const uint32_t size = g.alloc(8);
        call(Size_ctor, {size, w, h});
        vcall(web, V_setContentSize, {web, size});
    }
    {
        const uint32_t view = ld32(self + F_WEBVIEW);
        const float w = f32(ld32(vcall(view, V_getContentSize, {view})));
        const float h = f32(ld32(vcall(view, V_getContentSize, {view}) + 4));
        Frame g;
        const uint32_t pos = g.alloc(8);
        call(Vec2_ctor_, {pos, fbits(w * 0.5f), fbits(h * 0.5f)});
        vcall(view, V_setPosition, {view, pos});
        call(Vec2_dtor_, {pos});
    }
    vcall(ld32(self + F_WEBVIEW), V_setVisible, {ld32(self + F_WEBVIEW), 0});
    call(WebView_setScalesPageToFit, {ld32(self + F_WEBVIEW), 1});
    {
        Frame g;
        const uint32_t cb = newFunction(g, VT_PageEvent, {});
        call(function_webview_swap, {cb, ld32(self + F_WEBVIEW) + WEBVIEW_PAGE_CALLBACK});
        functionDestroy(cb);
    }
    vcall(area, V_addChild, {area, ld32(self + F_WEBVIEW)});
    const uint32_t close = findNode(root, S_ButtonClose, 0xC);
    vcall(close, V_setTouchEnabled, {close, 0});
    Frame f;
    const uint32_t fn = f.alloc(20);
    call(function_node_copy, {fn, closed});
    {
        // the open animation, then the page loads {this, URL, Close}
        const uint32_t tmp = f.alloc(12);
        strCopy(tmp, url);
        Frame g;
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = call(fn::operator_new, {0x18});
        if (obj) {
            st32(obj, VT_OpenDone);
            st32(obj + 4, self);
            strCopy(obj + 8, tmp);
            st32(obj + 0x14, close);
        }
        st32(fo + 16, obj);
        const uint32_t open = call(SceneUtil_createOpenPopupAnimation, {root, fo});
        call(fn::Node_runAction, {root, open});
        functionDestroy(fo);
        strFree(tmp);
    }
    {
        const uint32_t sm = sceneManager();
        Frame g;
        const uint32_t canClose = newFunction(g, VT_CanClose, {close});
        const uint32_t copy = g.alloc(20);
        functionCopy(copy, fn);
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = call(fn::operator_new, {0x38});
        if (obj) {
            st32(obj, VT_Closed);
            const uint32_t words[4] = {close, self, shadow, root};
            for (uint32_t k = 0; k < 4; k++) st32(obj + 8 + 4 * k, words[k]);
            call(function_node_copy2, {obj + 0x18, copy});
            st32(obj + 0x30, layer);
        }
        st32(fo + 16, obj);
        call(SceneManager_openPopup, {sm, canClose, fo, 1});
        functionDestroy(fo);
        functionDestroy(copy);
        functionDestroy(canClose);
    }
    functionDestroy(fn);
    return 1;
}
RW_FUNCTION(0x712CEC, "SceneTakeOver::openWebViewInfoPopup", scene_takeover::openWebViewInfoPopup);

// the popup closed {Close, scene, shadow, root, function<void(Node*)> +0x18, layer +0x30}: Close off, the page
// stopped, the shadow fading out and the popup closing (then the callback)
uint32_t popupClosed(uint32_t self, uint32_t okRef) {
    const uint32_t scene = ld32(self + 12);
    if (!ld8(okRef)) call(AudioPlaySystemSECancel, {});
    vcall(ld32(self + 8), V_setTouchEnabled, {ld32(self + 8), 0});
    call(WebView_stopLoading, {ld32(scene + F_WEBVIEW)});
    const uint32_t shadow = ld32(self + 16);
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {ld32(ld32(GOT_POPUP_FADE)), 0})});
    const uint32_t root = ld32(self + 20);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_node_copy, {copy, self + 24});
    const uint32_t layer = ld32(self + 48);
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = call(fn::operator_new, {0x30});
    if (obj) {
        st32(obj, VT_CloseDone);
        call(function_node_copy2, {obj + 8, copy});
        st32(obj + 0x20, root);
        st32(obj + 0x24, layer);
        st32(obj + 0x28, shadow);
    }
    st32(fo + 16, obj);
    call(fn::Node_runAction, {root, call(SceneUtil_createClosePopupAnimation, {root, fo})});
    functionDestroy(fo);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x713538, "SceneTakeOver::openWebViewInfoPopup closed operator() (sub_713538)", scene_takeover::popupClosed);

// the popup is open {scene, URL, Close}: the page loads, Close works
uint32_t popupOpened(uint32_t self) {
    call(WebView_loadUrl, {ld32(ld32(self + 4) + F_WEBVIEW), self + 8});
    const uint32_t close = ld32(self + 20);
    vcall(close, V_setTouchEnabled, {close, 1});
    addTouch(ld32(self + 20), VT_CloseTouch, {});
    return 0;
}
RW_FUNCTION(0x713AC8, "SceneTakeOver::openWebViewInfoPopup opened operator() (sub_713AC8)", scene_takeover::popupOpened);

// Intro: a Facebook login is logged out first (its callback goes on), else the birthday input
uint32_t introTouched(uint32_t, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    const uint32_t r = call(SceneUtil_isLoggedInFacebook, {});
    if (r == 1) {
        const uint32_t fb = call(FacebookManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_FacebookLoggedOut, {});
        call(FacebookManager_logout, {fb, fo});
        functionDestroy(fo);
    } else {
        call(SceneBirthRegister_changeSceneLayer, {r});
    }
    return 0;
}
RW_FUNCTION(0x713D74, "SceneTakeOver::init Intro operator() (sub_713D74)", scene_takeover::introTouched);

// the take-over button lambda {this, URL} copied (__clone)
uint32_t takeoverClone(uint32_t self) {
    const uint32_t c = call(fn::operator_new, {0x410});
    if (c) {
        st32(c, VT_TakeoverPage);
        call(libc_memcpy, {c + 4, self + 4, 0x40C});
    }
    return c;
}
RW_FUNCTION(0x713F28, "SceneTakeOver::init Takeover lambda __clone() (sub_713F28)", scene_takeover::takeoverClone);

// Take over {this, URL}: the info popup with the take-over page
uint32_t takeoverTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    const uint32_t scene = ld32(self + 4);
    call(AudioPlaySystemSEDecide, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    Frame f;
    const uint32_t td = textData(f, 109120001);
    const uint32_t title = newStr(f, textDataStr(td));
    const uint32_t url = newStr(f, self + 8);
    const uint32_t fo = newFunction(f, VT_InfoPopupClosed, {});
    call(SceneTakeOver_openWebViewInfoPopup, {scene, layer, title, url, fo});
    functionDestroy(fo);
    strFree(url);
    strFree(title);
    sharedRelease(td);
    return 0;
}
RW_FUNCTION(0x713FA4, "SceneTakeOver::init Takeover operator() (sub_713FA4)", scene_takeover::takeoverTouched);

}  // namespace scene_takeover
