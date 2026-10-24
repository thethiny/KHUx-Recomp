// SceneAgreement: the terms of use shown on a first start (a WebView with the agreement page from the server),
// Disagree (a confirmation popup; back to the title) and Agree (to the Facebook take-over screen when the login
// response asks for it, else to the birthday input).
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t Layer_init = rwlib::cocos2d::Layer::init;
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base;
constexpr uint32_t Scene_create = rwlib::cocos2d::Scene::create;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t function_vv_assign = 0x4EB9CD;          // std::function<void()>::operator=(const function&)
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr uint32_t SceneManager_openPopup = 0x61524D;      // (sm, function<bool()>&, function<void(bool)>&, bool)
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base, Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t fmul = 0xCD6E14;                        // __aeabi_fmul
constexpr uint32_t WebView_create = rwlib::cocos2d::plugin::WebView::create, WebView_setScalesPageToFit = rwlib::cocos2d::plugin::WebView::setScalesPageToFit;
constexpr uint32_t WebView_loadUrl = rwlib::cocos2d::plugin::WebView::loadUrl, WebView_stopLoading = rwlib::cocos2d::plugin::WebView::stopLoading;
constexpr uint32_t function_webview_swap = 0x4821B5;       // std::function<void(WebView*, string)>::swap
constexpr uint32_t SceneAgreement_runInAnimation = 0x71576D, SceneAgreement_create = 0x67F7F5;
constexpr uint32_t SceneBirthRegister_changeSceneLayer = 0x711CD5;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t GOT_TITLE_FACTORY = 0xFD68B4, GOT_TAKEOVER_FACTORY = 0xFD6980;
constexpr uint32_t VT_SceneAgreement = 0xFA4720;
constexpr uint32_t VT_OnEnter = 0xF6B5E8, VT_OnExit = 0xF6B5BC, VT_BackKey = 0xF6B590;   // $_0 / $_1 / $_2 {this}
constexpr uint32_t VT_PageEvent = 0xF6B564;                // $_3 {}: the WebView's callback
constexpr uint32_t VT_DisagreeTouch = 0xF6B538;            // $_4 {}
constexpr uint32_t VT_CanClose = 0xF6B50C;                 // $_5 {Disagree button}
constexpr uint32_t VT_Closed = 0xF6B4E0;                   // $_6 {}
constexpr uint32_t VT_AgreeTouch = 0xF6B4B4;               // $_7 {this}
constexpr uint32_t S_Json = 0xE06AEE, S_ScrollArea = 0xDF9B59, S_TxtTitle = 0xDF9554, S_ButtonDisagree = 0xE06B10;
constexpr uint32_t S_TxtDisagree = 0xE06B20, S_ButtonAgree = 0xE06B2D, S_TxtAgree = 0xE06B3A;
constexpr int32_t TXT_TITLE = 109110001, TXT_DISAGREE = 109110004, TXT_AGREE = 109110005;
constexpr uint32_t V_release = 0x04, V_init = 0x248, V_setPosition = 0x4C, V_setContentSize = 0xA8, V_getContentSize = 0xAC;
constexpr uint32_t V_setVisible = 0xB0, V_addChild = 0xF0, V_webViewClose = 0x124;
// SceneAgreement
constexpr uint32_t SIZE = 0x290, F_ON_ENTER = 0x210, F_ON_EXIT = 0x228, F_ROOT = 0x288, F_WEBVIEW = 0x28C;
// APIManager: the login / system data with the agreement URL (a std::string at +0x3C); the user data (+0x44,
// whose flags byte +0x34 bit 0x10 asks for the Facebook take-over)
constexpr uint32_t API_SYSTEM = 0x1A8, SYSTEM_AGREEMENT_URL = 0x3C, API_USER = 0x44, USER_FLAGS = 0x34;
constexpr uint32_t FLAG_TAKEOVER = 0x10;
constexpr uint32_t WEBVIEW_PAGE_CALLBACK = 0x368;
constexpr uint32_t TOUCH_ENDED = 2;

void assign(uint32_t member, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(function_vv_assign, {member, fo});
    functionDestroy(fo);
}

// the text under `name` of `node`: shadowed, from the text table
void labelText(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(node, name, len);
    SceneUtil::setShadow(text);
    setTextId(text, id);
}
}  // namespace

namespace scene_agreement {

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
    Frame f;
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Json});
    st32(self + F_ROOT, root);
    {
        const uint32_t x = call(SceneUtil_getSideMargin, {});
        const uint32_t y = call(SceneUtil_getSideHeightMargin, {x});
        Frame g;
        const uint32_t pos = g.alloc(8);
        call(Vec2_ctor, {pos, x, y});
        vcall(root, V_setPosition, {root, pos});
        call(Vec2_dtor, {pos});
    }
    vcall(self, V_addChild, {self, ld32(self + F_ROOT)});

    // the agreement page in a WebView filling the scroll area
    const uint32_t area = findNode(ld32(self + F_ROOT), S_ScrollArea, 0xB);
    const uint32_t web = call(WebView_create, {0xA3});
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
        const uint32_t w = ld32(vcall(view, V_getContentSize, {view}));
        const uint32_t h = ld32(vcall(ld32(self + F_WEBVIEW), V_getContentSize, {ld32(self + F_WEBVIEW)}) + 4);
        const uint32_t halfW = call(fmul, {w, 0x3F000000});
        const uint32_t halfH = call(fmul, {h, 0x3F000000});
        Frame g;
        const uint32_t pos = g.alloc(8);
        call(Vec2_ctor, {pos, halfW, halfH});
        vcall(view, V_setPosition, {view, pos});
        call(Vec2_dtor, {pos});
    }
    call(WebView_setScalesPageToFit, {ld32(self + F_WEBVIEW), 1});
    vcall(ld32(self + F_WEBVIEW), V_setVisible, {ld32(self + F_WEBVIEW), 0});
    vcall(area, V_addChild, {area, ld32(self + F_WEBVIEW)});
    {
        Frame g;
        const uint32_t cb = newFunction(g, VT_PageEvent, {});
        call(function_webview_swap, {cb, ld32(self + F_WEBVIEW) + WEBVIEW_PAGE_CALLBACK});
        functionDestroy(cb);
    }
    // the URL (a copy of the login data's string, kept to the end)
    const uint32_t src = ld32(call(fn::APIManager_getInstance, {}) + API_SYSTEM) + SYSTEM_AGREEMENT_URL;
    const uint32_t url = f.alloc(12);
    if (strIsLong(src)) {
        strInit(url, ld32(src + 8), ld32(src + 4));
    } else {
        st32(url, ld32(src));
        st32(url + 4, ld32(src + 4));
        st32(url + 8, ld32(src + 8));
    }
    {
        Frame g;
        const uint32_t s = newStr(g, strData(url));
        call(WebView_loadUrl, {ld32(self + F_WEBVIEW), s});
        strFree(s);
    }
    labelText(ld32(self + F_ROOT), S_TxtTitle, 0xF, TXT_TITLE);

    // Disagree: its touch, and the popup that opens over it
    const uint32_t disagree = findNode(ld32(self + F_ROOT), S_ButtonDisagree, 0xF);
    addTouch(disagree, VT_DisagreeTouch, {});
    {
        const uint32_t sm = sceneManager();
        Frame g;
        const uint32_t canClose = newFunction(g, VT_CanClose, {disagree});
        const uint32_t closed = newFunction(g, VT_Closed, {});
        call(SceneManager_openPopup, {sm, canClose, closed, 1});
        functionDestroy(closed);
        functionDestroy(canClose);
    }
    labelText(disagree, S_TxtDisagree, 0xC, TXT_DISAGREE);
    const uint32_t agree = findNode(ld32(self + F_ROOT), S_ButtonAgree, 0xC);
    addTouch(agree, VT_AgreeTouch, {self});
    labelText(agree, S_TxtAgree, 9, TXT_AGREE);
    call(SceneAgreement_runInAnimation, {self});
    strFree(url);
    return 1;
}
RW_FUNCTION(0x714EEC, "SceneAgreement::init", scene_agreement::init);

uint32_t create(uint32_t) {
    const uint32_t self = call(fn::operator_new, {SIZE});
    call(Layer_ctor, {self});
    st32(self, VT_SceneAgreement);
    st32(self + F_WEBVIEW, 0);
    if (vcall(self, V_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x67F7F4, "SceneAgreement::create", scene_agreement::create);

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneAgreement_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x714E84, "SceneAgreement::createScene", scene_agreement::createScene);

// $_6: the Disagree popup closed - "no" goes back to the title
uint32_t disagreeClosed(uint32_t, uint32_t okRef) {
    if (ld8(okRef)) return 0;
    call(AudioPlaySystemSECancel, {});
    smain::changeSceneLayer(ld32(GOT_TITLE_FACTORY));
    return 0;
}
RW_FUNCTION(0x7159AC, "SceneAgreement::init::$_6 operator() (disagree closed)", scene_agreement::disagreeClosed);

// $_7: Agree - the page stops and closes, then the take-over screen (Facebook data on the server) or the birthday
uint32_t agreeTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return 0;
    const uint32_t scene = ld32(self + 4);
    call(AudioPlaySystemSEDecide, {});
    call(WebView_stopLoading, {ld32(scene + F_WEBVIEW)});
    vcall(ld32(scene + F_WEBVIEW), V_webViewClose, {ld32(scene + F_WEBVIEW)});
    call(SceneManager_closeCurrentPopup, {sceneManager(), 1});
    call(fn::APIManager_getInstance, {});
    const uint32_t user = ld32(call(fn::APIManager_getInstance, {}) + API_USER);
    if (user && (ld8(user + USER_FLAGS) & FLAG_TAKEOVER)) {
        smain::changeSceneLayer(ld32(GOT_TAKEOVER_FACTORY));
    } else {
        call(SceneBirthRegister_changeSceneLayer, {user ? uint32_t(ld8(user + USER_FLAGS)) : 0u});
    }
    return 0;
}
RW_FUNCTION(0x71581C, "SceneAgreement::init::$_7 operator() (agree)", scene_agreement::agreeTouched);

// $_3 (WebView*, std::string): the page loaded - the WebView shown; the URL string is taken (moved) and freed
void pageLoaded(uint32_t, uint32_t webRef, uint32_t url) {
    const uint32_t data = ld32(url + 8);
    const uint8_t head = ld8(url);
    const uint32_t web = ld32(webRef);
    st32(url, 0);
    st32(url + 4, 0);
    st32(url + 8, 0);
    vcall(web, V_setVisible, {web, 1});
    if (head & 1) call(fn::operator_delete, {data});
}
RW_FUNCTION(0x715C26, "SceneAgreement::init::$_3 operator() (page loaded)", scene_agreement::pageLoaded);

}  // namespace scene_agreement
