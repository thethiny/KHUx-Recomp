// SceneNotice: create / init / the notice popups (see SceneNotice.hpp).
#include "SceneNotice.hpp"

using namespace social;

namespace {
namespace g {
constexpr uint32_t Node_create = 0x89372D;                   // cocos2d::Node::create()
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t FadeTo_create = 0x86B3E1;                 // (float duration, uint8 opacity)
constexpr uint32_t Vec2_ctor = 0x8E4B9D;                     // (Vec2*, float, float)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Size_ctor = 0x8DF027;                     // (Size*, float, float)
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;  // (reader, const char*)
constexpr uint32_t Widget_isTouchEnabled = 0x826665;
constexpr uint32_t WebView_create = 0x8191B5;                // cocos2d::plugin::WebView::create()
constexpr uint32_t WebView_loadUrl = 0x819221;               // (webView, const std::string&)
constexpr uint32_t WebView_setScalesPageToFit = 0x8192AD;    // (webView, bool)
constexpr uint32_t strlen_ = 0x40E08C;                       // strlen (import)
constexpr uint32_t vector_throw_out_of_range = 0x47CCE1;     // std::__vector_base_common<true>::__throw_out_of_range
// libc++ std::function
constexpr uint32_t function_vv_call = 0x470E69;              // function<void()>::operator()
constexpr uint32_t function_vv_copy = 0x4683CD;              // function<void()>::function(const function&)
constexpr uint32_t function_vv_move = 0x47F905;              // function<void()>::function(function&&)
constexpr uint32_t function_vv_assign = 0x4EB9CD;            // function<void()>::operator=(const function&)
constexpr uint32_t function_vb_assign = 0x490109;            // function<void(bool)>::operator=(const function&)
constexpr uint32_t function_node_copy = 0x474D99;            // function<void(Node*)>::function(const function&)
constexpr uint32_t function_node_move = 0x481F0D;            // function<void(Node*)>::function(function&&)
constexpr uint32_t function_node_call = 0x481E8D;            // function<void(Node*)>::operator()(Node*)
constexpr uint32_t function_webview_swap = 0x4821B5;         // function<void(WebView*, std::string)>::swap
// game
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D;       // float
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;           // (ui::Text*)
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;   // (node, std::function<void()> by value)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;  // (node, std::function<void()> by value)
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;      // (std::function<void(bool)> by value)
constexpr uint32_t SceneUtil_moveToMyPageOrResumeStage = 0x46F161;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SceneManager_getPopupLayer2 = 0x613E71;
constexpr uint32_t SceneManager_openPopup = 0x61524D;        // (mgr, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;  // (mgr, bool)
constexpr uint32_t SceneManager_setEnabledTouchEffect = 0x60CF65;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;  // (mgr, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t APIManager_executeSystemInformationList151203 = 0x4CDAC9;  // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_executeLoginBonus = 0x4C5699;                   // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_executeRankingParadeGet = 0x4D3181;             // (api, const function<void(bool)>&)
constexpr uint32_t SafeBuffer_read = 0x45A269;               // (sb, dst, src, n, index)
}  // namespace g

// GOT slots (read at run time: the loader fills them)
constexpr uint32_t GOT_IN_OUT_TIME = 0xFD65C0;               // &SceneUtil::IN_OUT_TIME (float)
constexpr uint32_t GOT_FUNC_LAYER_FACTORY_VTABLE = 0xFD6074; // &_ZTV __func<Layer*(*)(), ..., Layer*()> (vptr + 8)
constexpr uint32_t GOT_CREATE_LOGIN_BONUS = 0xFD6990;        // &createSceneLayer<SceneLoginBonus>
constexpr uint32_t GOT_CREATE_UNION_RANKING = 0xFD695C;      // &createSceneLayer<SceneUnionRanking>

// strings
constexpr uint32_t S_Information2_Base = 0xDF9B2F;   // "cocostudio/publish/Information2_Base.json"
constexpr uint32_t S_Txt_Title_Label = 0xDF9554;
constexpr uint32_t S_Scroll_Area = 0xDF9B59;
constexpr uint32_t S_Button_Close = 0xDF8692;

constexpr int32_t TEXT_NOTICE_TITLE = 100600018;     // the popup title
constexpr int32_t TUTORIAL_PHASE_DONE = 994;         // getNexTutorialphase() > 994: the tutorial is over

// Node / Widget virtual slots (byte offsets) not in SceneCommon's node:: list
constexpr uint32_t V_release = 0x04;                 // deleting destructor
constexpr uint32_t V_initThroughVtableSymbol = 0x250;  // _ZTV11SceneNotice + 0x250: SceneNotice::init
constexpr uint32_t V_setTouchEnabled = 0x29C;        // ui::Widget::setTouchEnabled(bool)
constexpr uint32_t V_setVisible = 0xB0;
constexpr uint32_t V_setContentSize = 0xA8;
constexpr uint32_t V_getContentSize = 0xAC;
constexpr uint32_t V_setAnchorPoint = 0x9C;
constexpr uint32_t V_setPosition = 0x4C;
constexpr uint32_t V_getPosition = 0x54;
constexpr uint32_t V_getOpacity = 0x20C;
constexpr uint32_t V_setOpacity = 0x214;
constexpr uint32_t V_addChild = 0xF0;
constexpr uint32_t V_removeChild = 0x12C;            // (child, cleanup)
constexpr uint32_t WEBVIEW_ON_LOADED = 0x368;        // WebView + 0x368: std::function<void(WebView*, std::string)>
constexpr uint32_t APIMANAGER_NOTICE_URLS = 0x1C0;   // APIManager + 0x1C0: std::vector<std::string>* notice urls
constexpr uint32_t APIMANAGER_USER = 0x4C;           // APIManager + 0x4C: shared_ptr<SafeBuffer> user flags
constexpr uint32_t APIMANAGER_UNION_RANKING = 0x2EC; // APIManager + 0x2EC: a vector* (union ranking results)
constexpr uint32_t SCENEMANAGER_LOADING_BLOCKED = 0x4CC;  // byte
constexpr uint32_t SCENEMANAGER_ON_RESUME = 0x548;        // std::function<void(bool)>

constexpr uint32_t TOUCH_ENDED = 2;                  // ui::Widget::TouchEventType::ENDED
constexpr uint32_t HALF = 0x3F000000;                // 0.5f

inline SceneNotice *notice(Addr p) { return rw::host_ptr<SceneNotice>(p); }
inline uint32_t vptr() { return ld32(SceneNotice::GOT_VTABLE) + 8; }

// a std::string on the guest stack from a C string (std::string(const char*): strlen, then __init)
Addr newCStr(Frame &f, Addr text) {
    Addr s = f.alloc(12);
    strInit(s, text, call(g::strlen_, {text}));
    return s;
}
inline Addr strCStr(Addr s) { return (ld8(s) & 1) ? ld32(s + 8) : s + 1; }

// The inlined copy constructor of a std::function (any signature): `dst` gets a clone of `src`'s callable.
void copyFunctionInline(Addr dst, Addr src) {
    Addr p = ld32(src + 16);
    if (!p) {
        st32(dst + 16, 0);
    } else if (p == src) {
        st32(dst + 16, dst);
        vcall(p, 0x0C, {p, dst});   // __clone(dst)
    } else {
        st32(dst + 16, vcall(p, 0x08, {p}));   // __clone()
    }
}

// SceneManager::changeSceneLayer(mgr, function<Layer*()>{factory}, true, true, 1)
void changeSceneLayerTo(uint32_t gotFactory) {
    Addr mgr = call(sfn::SceneManager_getInstance, {});
    Frame f;
    Addr fn = newFunction(f, ld32(GOT_FUNC_LAYER_FACTORY_VTABLE) + 8, {ld32(gotFactory)});
    call(g::SceneManager_changeSceneLayer, {mgr, fn, 1, 1, 1});
    functionDestroy(fn);
}

// SceneManager: loadingBlocked = 1, onResume = an empty function
void blockLoadingAndClearResume() {
    Addr mgr = call(sfn::SceneManager_getInstance, {});
    Frame f;
    Addr empty = newEmptyFunction(f);
    rw::save<uint8_t>(mgr + SCENEMANAGER_LOADING_BLOCKED, 1);
    call(g::function_vb_assign, {mgr + SCENEMANAGER_ON_RESUME, empty});
    functionDestroy(empty);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
SceneNotice::Addr SceneNotice::create() {
    Addr self = call(fn::operator_new, {SIZE});
    call(sfn::Layer_ctor, {self});
    Addr vtableSymbol = ld32(GOT_VTABLE);
    st32(self, vtableSymbol + 8);
    SceneNotice *n = notice(self);
    n->root = 0;
    n->webView = 0;
    n->shownCount = 0;
    // init() through the vtable symbol (the compiler knew the dynamic type)
    if (call(ld32(vtableSymbol + V_initThroughVtableSymbol), {self}) == 1) {
        call(sfn::Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x67F540, "SceneNotice::create", SceneNotice::create);

SceneNotice::Addr SceneNotice::createScene() {
    Addr scene = call(sfn::Scene_create, {});
    Addr layer = create();
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x733AB4, "SceneNotice::createScene", SceneNotice::createScene);

uint32_t SceneNotice::destroy(SceneNotice *self) {
    st32(rw::guest_addr(self), vptr());
    return call(sfn::Layer_dtor, {rw::guest_addr(self)});
}
RW_FUNCTION(0x733B6C, "SceneNotice::~SceneNotice", SceneNotice::destroy);

void SceneNotice::destroyDelete(SceneNotice *self) {
    st32(rw::guest_addr(self), vptr());
    Addr p = call(sfn::Layer_dtor, {rw::guest_addr(self)});
    call(fn::operator_delete, {p});
}
RW_FUNCTION(0x733B8C, "SceneNotice::~SceneNotice (deleting)", SceneNotice::destroyDelete);

uint32_t SceneNotice::init(SceneNotice *self) {
    Addr me = rw::guest_addr(self);
    if (call(sfn::Layer_init, {me}) != 1) return 0;
    withLambda(SceneNoticeVt::InitOnEnter1, {me}, [&](Addr f) { call(g::function_vv_assign, {me + 0x210, f}); });
    withLambda(SceneNoticeVt::InitOnExit2, {me}, [&](Addr f) { call(g::function_vv_assign, {me + 0x228, f}); });

    Addr root = call(g::Node_create, {});
    self->root = root;
    Addr setPositionX = ld32(ld32(root) + node::setPositionX);
    call(setPositionX, {root, call(g::SceneUtil_getSideMargin, {})});
    vcall(me, V_addChild, {me, self->root});

    if (int32_t(call(g::SceneUtil_getNexTutorialphase, {})) > TUTORIAL_PHASE_DONE) {
        // show the notices; $_4 runs after the last one
        withLambda(SceneNoticeVt::InitAllShown4, {me}, [&](Addr f) { openNextPage(self, f); });
    } else {
        // during the tutorial: skip them ($_3 right away)
        withLambda(SceneNoticeVt::InitSkip3, {}, [&](Addr f) { call(g::function_vv_call, {f}); });
    }
    return 1;
}
RW_FUNCTION(0x733BB0, "SceneNotice::init", SceneNotice::init);

// the callback argument is not used: the notice list request always finishes with $_0
void SceneNotice::callPrepareAPI(SceneNotice *, Addr) {
    Addr api = call(sfn::APIManager_getInstance, {});
    withLambda(SceneNoticeVt::PrepareDone0, {}, [&](Addr f) {
        call(g::APIManager_executeSystemInformationList151203, {api, f});
    });
}
RW_FUNCTION(0x733AD8, "SceneNotice::callPrepareAPI", SceneNotice::callPrepareAPI);

void SceneNotice::runOutAnimation(SceneNotice *, Addr done) { call(g::function_vv_call, {done}); }
RW_FUNCTION(0x733D78, "SceneNotice::runOutAnimation", SceneNotice::runOutAnimation);

void SceneNotice::update(SceneNotice *, float) {}
RW_FUNCTION(0x734044, "SceneNotice::update", SceneNotice::update);

void SceneNotice::runInAnimation(SceneNotice *) {}
RW_FUNCTION(0x734046, "SceneNotice::runInAnimation", SceneNotice::runInAnimation);

// ------------------------------------------------------------------------------------------------ notices
// Open the next notice (the urls are shown from the last to the first); when all were shown, call `done`.
void SceneNotice::openNextPage(SceneNotice *self, Addr done) {
    Addr me = rw::guest_addr(self);
    Frame f;
    Addr urls = ld32(call(sfn::APIManager_getInstance, {}) + APIMANAGER_NOTICE_URLS);
    uint32_t begin = ld32(urls), end = ld32(urls + 4);
    Addr doneCopy = f.alloc(20);
    call(g::function_vv_copy, {doneCopy, done});
    uint32_t count = uint32_t(int32_t(end - begin) >> 2) * 0xAAAAAAABu;   // / sizeof(std::string)
    if (self->shownCount >= int32_t(count)) {
        if (ld32(done + 16)) call(g::function_vv_call, {done});
    } else {
        Addr text = f.alloc(8);
        call(sfn::TextData_get, {text, 0, uint32_t(TEXT_NOTICE_TITLE)});
        Addr title = newCStr(f, ld32(ld32(text) + 4));
        uint32_t size = uint32_t(int32_t(ld32(urls + 4) - ld32(urls)) >> 2) * 0xAAAAAAABu;
        uint32_t index = count - uint32_t(self->shownCount.get()) - 1;
        if (size <= index) {   // urls.at(index) out of range: throws
            call(g::vector_throw_out_of_range, {urls});
            return;
        }
        Addr url = newCStr(f, strCStr(ld32(urls) + index * 12));

        // $_9 {this, done}: on close, open the next notice
        Addr lambda = f.alloc(0x1C);   // this at +0, std::function<void()> at +8
        Addr lambdaDone = lambda + 8;
        st32(lambda, me);
        copyFunctionInline(lambdaDone, doneCopy);
        st32(lambda, me);
        Addr onClosed = f.alloc(20);
        st32(onClosed + 16, 0);
        Addr func = call(fn::operator_new, {0x28});
        if (func) {
            st32(func, SceneNoticeVt::OpenNextPage9);
            st32(func + 8, ld32(lambda));
            call(g::function_vv_move, {func + 16, lambdaDone});
            st32(func + 8, ld32(lambda));
        }
        st32(onClosed + 16, func);
        openWebViewInfoPopup(self, title, url, onClosed);
        functionDestroy(onClosed);
        functionDestroy(lambdaDone);
        strFree(url);
        strFree(title);
        sharedRelease(text);
        self->shownCount = int32_t(uint32_t(self->shownCount.get()) + 1);
    }
    functionDestroy(doneCopy);
}
RW_FUNCTION(0x733D84, "SceneNotice::openNextPage", SceneNotice::openNextPage);

// The notice popup: a fading shadow on popup layer 2 holding Information2_Base.json with the title; when the
// open animation ends, $_6 puts a WebView with `url` into the scroll area. Button_Close closes the popup ($_5);
// $_8 then fades it out, calls onClosed(panel) and removes it.
void SceneNotice::openWebViewInfoPopup(SceneNotice *self, Addr title, Addr url, Addr onClosed) {
    Addr me = rw::guest_addr(self);
    Frame f;
    call(g::AudioPlaySystemSEPopUp, {});
    Addr popupLayer = call(g::SceneManager_getPopupLayer2, {call(sfn::SceneManager_getInstance, {})});
    Addr shadow = call(g::SceneUtil_createShadowSprite, {});
    uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    Addr fade = call(g::FadeTo_create, {ld32(ld32(GOT_IN_OUT_TIME)), opacity});
    call(sfn::Node_runAction, {shadow, fade});
    vcall(popupLayer, V_addChild, {popupLayer, shadow});

    Addr panel = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_Information2_Base});
    vcall(panel, V_setTouchEnabled, {panel, 0});
    vcall(shadow, V_addChild, {shadow, panel});
    Addr setAnchorPoint = ld32(ld32(panel) + V_setAnchorPoint);
    Addr anchor = f.alloc(8);
    call(g::Vec2_ctor, {anchor, HALF, HALF});
    call(setAnchorPoint, {panel, anchor});
    call(g::Vec2_dtor, {anchor});
    Addr setPosition = ld32(ld32(panel) + V_setPosition);
    call(setPosition, {panel, vcall(shadow, V_getPosition, {shadow})});

    Addr titleLabel = search(panel, S_Txt_Title_Label, 15);
    call(g::SceneUtil_setShadow, {titleLabel});
    {
        Frame t;
        Addr s = newCStr(t, strCStr(title));
        call(fn::Text_setString, {titleLabel, s});
        strFree(s);
    }
    Addr scrollArea = search(panel, S_Scroll_Area, 11);
    Addr closeButton = search(panel, S_Button_Close, 12);
    vcall(closeButton, V_setTouchEnabled, {closeButton, 0});

    // $_8's captures {shadow, panel, onClosed, popupLayer}
    Addr closed = f.alloc(0x24);
    Addr closedFn = closed + 8;
    st32(closed, shadow);
    st32(closed + 4, panel);
    call(g::function_node_copy, {closedFn, onClosed});
    st32(closed + 0x20, popupLayer);

    vcall(closeButton, V_setTouchEnabled, {closeButton, 1});
    withLambda(SceneNoticeVt::CloseTouch5, {}, [&](Addr fn5) {
        call(sfn::Widget_addTouchEventListener, {closeButton, fn5});
    });

    // $_6 {this, scrollArea, url}: after the open animation, show the page
    Addr opened = f.alloc(0x14);
    st32(opened, me);
    st32(opened + 4, scrollArea);
    Addr urlCopy = opened + 8;
    if (ld8(url) & 1) {
        strInit(urlCopy, ld32(url + 8), ld32(url + 4));
    } else {
        st32(urlCopy, ld32(url));
        st32(urlCopy + 4, ld32(url + 4));
        st32(urlCopy + 8, ld32(url + 8));
    }
    Addr openedFn = f.alloc(20);
    st32(openedFn + 16, 0);
    Addr func6 = call(fn::operator_new, {0x18});
    if (func6) {
        st32(func6, SceneNoticeVt::PopupOpened6);
        st32(func6 + 8, ld32(opened + 4));
        st32(func6 + 4, ld32(opened));
        if (ld8(urlCopy) & 1) {
            strInit(func6 + 12, ld32(opened + 0x10), ld32(opened + 0xC));
        } else {
            st32(func6 + 12, ld32(urlCopy));
            st32(func6 + 16, ld32(urlCopy + 4));
            st32(func6 + 20, ld32(urlCopy + 8));
        }
    }
    st32(openedFn + 16, func6);
    Addr openAnim = call(g::SceneUtil_createOpenPopupAnimation, {panel, openedFn});
    call(sfn::Node_runAction, {panel, openAnim});
    functionDestroy(openedFn);
    strFree(urlCopy);

    Addr mgr = call(sfn::SceneManager_getInstance, {});
    // $_7 {closeButton}: the popup may close while the button is enabled
    Addr canClose = newFunction(f, SceneNoticeVt::PopupCanClose7, {closeButton});
    // $_8 copy {shadow, panel, onClosed, popupLayer}, then moved into its __func
    Addr closedCopy = f.alloc(0x24);
    Addr closedCopyFn = closedCopy + 8;
    st32(closedCopy, ld32(closed));
    st32(closedCopy + 4, ld32(closed + 4));
    copyFunctionInline(closedCopyFn, closedFn);
    st32(closedCopy + 0x20, ld32(closed + 0x20));
    Addr closedArg = f.alloc(20);
    st32(closedArg + 16, 0);
    Addr func8 = call(fn::operator_new, {0x30});
    if (func8) {
        st32(func8, SceneNoticeVt::PopupClosed8);
        st32(func8 + 8, ld32(closedCopy));
        st32(func8 + 12, ld32(closedCopy + 4));
        call(g::function_node_move, {func8 + 16, closedCopyFn});
        st32(func8 + 40, ld32(closedCopy + 0x20));
    }
    st32(closedArg + 16, func8);
    call(g::SceneManager_openPopup, {mgr, canClose, closedArg, 1});
    functionDestroy(closedArg);
    functionDestroy(closedCopyFn);
    functionDestroy(canClose);
    functionDestroy(closedFn);
}
RW_FUNCTION(0x734048, "SceneNotice::openWebViewInfoPopup", SceneNotice::openWebViewInfoPopup);

// ------------------------------------------------------------------------------------------------ lambda bodies
// (operator() of each __func: the __func object first, then pointers to the call's arguments)
struct SceneNoticeLambdas {
    // openNextPage $_9 {this +8, done +16}(Node*): open the next notice
    static void openNext(Addr func, Addr) {
        Frame f;
        SceneNotice *self = notice(ld32(func + 8));
        Addr done = f.alloc(20);
        call(g::function_vv_copy, {done, func + 16});
        SceneNotice::openNextPage(self, done);
        functionDestroy(done);
    }

    // openWebViewInfoPopup $_8 {shadow +8, panel +12, onClosed +16, popupLayer +40}(bool ok): the popup is
    // closing - fade the shadow, play the close animation on the panel, then $_8::{lambda()#1}
    static void popupClosed(Addr func, Addr ok) {
        if (ld8(ok) == 0) call(g::AudioPlaySystemSECancel, {});
        uint32_t duration = ld32(ld32(GOT_IN_OUT_TIME));
        Addr shadow = ld32(func + 8);
        call(sfn::Node_runAction, {shadow, call(g::FadeTo_create, {duration, 0})});
        Addr panel = ld32(func + 12);
        Frame f;
        // $_8::{lambda()#1}'s captures: onClosed +0, panel +0x18, popupLayer +0x1C, shadow +0x20
        Addr remove = f.alloc(0x24);
        call(g::function_node_copy, {remove, func + 16});
        st32(remove + 0x18, ld32(func + 12));
        st32(remove + 0x1C, ld32(func + 40));
        st32(remove + 0x20, ld32(func + 8));
        Addr removeFn = f.alloc(20);
        st32(removeFn + 16, 0);
        Addr heap = call(fn::operator_new, {0x30});
        if (heap) {
            st32(heap, SceneNoticeVt::PopupRemove8);
            call(g::function_node_move, {heap + 8, remove});
            st32(heap + 0x20, ld32(remove + 0x18));
            st32(heap + 0x24, ld32(remove + 0x1C));
            st32(heap + 0x28, ld32(remove + 0x20));
        }
        st32(removeFn + 16, heap);
        Addr anim = call(g::SceneUtil_createClosePopupAnimation, {panel, removeFn});
        call(sfn::Node_runAction, {panel, anim});
        functionDestroy(removeFn);
        functionDestroy(remove);
    }

    // $_8::{lambda()#1} {onClosed +8, panel +0x20, popupLayer +0x24, shadow +0x28}: onClosed(panel), then remove
    // the shadow and the panel from the popup layer
    static void popupRemove(Addr func) {
        if (ld32(func + 0x18)) call(g::function_node_call, {func + 8, ld32(func + 0x20)});
        Addr layer = ld32(func + 0x24);
        vcall(layer, V_removeChild, {layer, ld32(func + 0x28), 1});
        layer = ld32(func + 0x24);
        vcall(layer, V_removeChild, {layer, ld32(func + 0x20), 1});
    }

    // $_7 {closeButton +4}() -> bool: the popup may close while the close button is touch enabled
    static uint32_t popupCanClose(Addr func) { return call(g::Widget_isTouchEnabled, {ld32(func + 4)}); }

    // $_6 {this +4, scrollArea +8, url +12}(): a hidden WebView sized and centred on the scroll area loads the
    // url; it becomes visible once loaded ($_6::{lambda#1})
    static void popupOpened(Addr func) {
        Frame f;
        SceneNotice *self = notice(ld32(func + 4));
        Addr wv = call(g::WebView_create, {});
        self->webView = wv;
        vcall(wv, V_setVisible, {wv, 0});

        wv = self->webView;
        Addr setContentSize = ld32(ld32(wv) + V_setContentSize);
        Addr scroll = ld32(func + 8);
        uint32_t width = ld32(vcall(scroll, V_getContentSize, {scroll}));
        scroll = ld32(func + 8);
        uint32_t height = ld32(vcall(scroll, V_getContentSize, {scroll}) + 4);
        Addr size = f.alloc(8);
        call(g::Size_ctor, {size, width, height});
        call(setContentSize, {wv, size});

        wv = self->webView;
        Addr setPosition = ld32(ld32(wv) + V_setPosition);
        float w = rw::bitsf(ld32(vcall(wv, V_getContentSize, {wv})));
        Addr wv2 = self->webView;
        Addr sz = vcall(wv2, V_getContentSize, {wv2});
        float x = w * 0.5f;
        float y = rw::bitsf(ld32(sz + 4)) * 0.5f;
        Addr pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, rw::fbits(x), rw::fbits(y)});
        call(setPosition, {wv, pos});
        call(g::Vec2_dtor, {pos});

        call(g::WebView_setScalesPageToFit, {self->webView, 1});
        call(g::WebView_loadUrl, {self->webView, func + 12});
        scroll = ld32(func + 8);
        vcall(scroll, V_addChild, {scroll, self->webView});
        Addr onLoaded = newFunction(f, SceneNoticeVt::WebViewLoaded6, {});
        call(g::function_webview_swap, {onLoaded, self->webView + WEBVIEW_ON_LOADED});
        functionDestroy(onLoaded);
    }

    // $_6::{lambda(WebView*, std::string)#1}: the page loaded - show the WebView (the string is taken by value:
    // moved out of the argument and freed)
    static void webViewLoaded(Addr, Addr webViewArg, Addr str) {
        uint32_t data = ld32(str + 8);
        uint8_t flags = ld8(str);
        Addr wv = ld32(webViewArg);
        st32(str, 0);
        st32(str + 4, 0);
        st32(str + 8, 0);
        vcall(wv, V_setVisible, {wv, 1});
        if (flags & 1) call(fn::operator_delete, {data});
    }

    // $_5 (Button_Close touch): on release, close the popup
    static void closeTouch(Addr, Addr, Addr type) {
        if (ld32(type) != TOUCH_ENDED) return;
        call(g::SceneManager_closeCurrentPopup, {call(sfn::SceneManager_getInstance, {}), 0});
    }

    // init $_4 {this}(): all notices shown - run $_4::{lambda()#1}
    static void allShown(Addr) {
        withLambda(SceneNoticeVt::InitAllShown4Next, {}, [&](Addr f) { call(g::function_vv_call, {f}); });
    }

    // init $_4::{lambda()#1}: leave the notice scene. With the user's login bonus flag (byte 2 of the user
    // SafeBuffer): request the login bonus and go to SceneLoginBonus; else to SceneUnionRanking when there are
    // union ranking results, else to my page (or the interrupted stage).
    static void leaveNotices() {
        Frame f;
        Addr api = call(sfn::APIManager_getInstance, {});
        Addr user = ld32(api + APIMANAGER_USER);
        Addr ctrl = ld32(api + APIMANAGER_USER + 4);
        if (ctrl) call(fn::shared_add, {ctrl});
        Addr flag = f.alloc(4);
        call(g::SafeBuffer_read, {user, flag, ld32(user + 12) + 2, 1, 2});
        if (ld8(flag)) {
            blockLoadingAndClearResume();
            Addr api2 = call(sfn::APIManager_getInstance, {});
            withEmptyFunction([&](Addr empty) { call(g::APIManager_executeLoginBonus, {api2, empty}); });
            changeSceneLayerTo(GOT_CREATE_LOGIN_BONUS);
        } else {
            Addr results = ld32(call(sfn::APIManager_getInstance, {}) + APIMANAGER_UNION_RANKING);
            if (ld32(results + 4) == ld32(results)) call(g::SceneUtil_moveToMyPageOrResumeStage, {});
            else changeSceneLayerTo(GOT_CREATE_UNION_RANKING);
        }
        if (ctrl) call(fn::shared_release, {ctrl});
    }

    // init $_3(): during the tutorial - prepare the my page requests, then move on
    static void skipNotices(Addr) {
        withLambda(SceneNoticeVt::InitSkip3Done, {}, [&](Addr f) { call(g::SceneUtil_callPrepareMypageAPIs, {f}); });
    }

    // init $_3's innermost lambda(bool)
    static void skipNoticesDone(Addr, Addr) { call(g::SceneUtil_moveToMyPageOrResumeStage, {}); }

    // init $_2 {this} (onExit)
    static void onExit(Addr func) {
        call(g::Node_unscheduleUpdate, {ld32(func + 4)});
        call(g::SceneManager_setEnabledTouchEffect, {call(sfn::SceneManager_getInstance, {}), 1});
    }

    // init $_1 {this} (onEnter)
    static void onEnter(Addr func) {
        call(g::Node_scheduleUpdate, {ld32(func + 4)});
        call(g::SceneManager_setEnabledTouchEffect, {call(sfn::SceneManager_getInstance, {}), 0});
    }

    // callPrepareAPI $_0(bool ok): on success, request the ranking parade (loading indicator blocked, no
    // resume callback)
    static void prepareDone(Addr, Addr ok) {
        if (ld8(ok) == 0) return;
        blockLoadingAndClearResume();
        Addr api = call(sfn::APIManager_getInstance, {});
        withEmptyFunction([&](Addr empty) { call(g::APIManager_executeRankingParadeGet, {api, empty}); });
    }
};
RW_FUNCTION(0x73479C, "SceneNotice::openNextPage $_9 operator()", SceneNoticeLambdas::openNext);
RW_FUNCTION(0x7349D4, "SceneNotice::openWebViewInfoPopup $_8 operator()", SceneNoticeLambdas::popupClosed);
RW_FUNCTION(0x734CD0, "SceneNotice::openWebViewInfoPopup $_8::{lambda()#1} operator()", SceneNoticeLambdas::popupRemove);
RW_FUNCTION(0x734D9E, "SceneNotice::openWebViewInfoPopup $_7 operator()", SceneNoticeLambdas::popupCanClose);
RW_FUNCTION(0x734F30, "SceneNotice::openWebViewInfoPopup $_6 operator()", SceneNoticeLambdas::popupOpened);
RW_FUNCTION(0x735136, "SceneNotice::openWebViewInfoPopup $_6::{lambda(WebView*, string)#1} operator()",
            SceneNoticeLambdas::webViewLoaded);
RW_FUNCTION(0x735206, "SceneNotice::openWebViewInfoPopup $_5 operator()", SceneNoticeLambdas::closeTouch);
RW_FUNCTION(0x7352B0, "SceneNotice::init $_4 operator()", SceneNoticeLambdas::allShown);
RW_FUNCTION(0x7353C8, "SceneNotice::init $_4::{lambda()#1} operator()", SceneNoticeLambdas::leaveNotices);
RW_FUNCTION(0x735640, "SceneNotice::init $_3 operator()", SceneNoticeLambdas::skipNotices);
RW_FUNCTION(0x735756, "SceneNotice::init $_3 lambda(bool) operator()", SceneNoticeLambdas::skipNoticesDone);
RW_FUNCTION(0x7357F2, "SceneNotice::init $_2 operator()", SceneNoticeLambdas::onExit);
RW_FUNCTION(0x73589A, "SceneNotice::init $_1 operator()", SceneNoticeLambdas::onEnter);
RW_FUNCTION(0x735938, "SceneNotice::callPrepareAPI $_0 operator()", SceneNoticeLambdas::prepareDone);
