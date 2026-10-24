// MENU > Other: the small screens of the other menu. Settings (SceneUtil::openOptionPopup: BGM / SE sliders and
// the AP / Relief / Events on-off pairs, with the on / off label helpers sub_46E6C8 / sub_46E7D0), Serial Codes
// (OtherMenuDialog::openSerialCodePopup and its result / closed lambdas), Legal Information
// (OtherMenuDialog::openLegalPopup and its lambdas), Notices (the button lambda sub_5ACCEC, the list callback
// sub_5ACE24 and SceneUtil::openWebViewInfoPopup), Party (the button lambda sub_5AC6C0) and Help (the web view's
// shouldStartLoading JNI callback).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;               // FadeTo::create(float duration, uint8 opacity)
constexpr uint32_t Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                   // Size(float, float)
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t ScrollView_create = rwlib::cocos2d::ui::ScrollView::create, ScrollView_setInnerContainerSize = rwlib::cocos2d::ui::ScrollView::setInnerContainerSize;
constexpr uint32_t Slider_setPercent = rwlib::cocos2d::ui::Slider::setPercent, Slider_addEventListener = rwlib::cocos2d::ui::Slider::addEventListener;
constexpr uint32_t ImageButton_create = 0x531239;          // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetImageView = 0x531631;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;   // (button, std::function by value)
constexpr uint32_t ImageButton_setSelectEnabled = 0x5318F1, ImageButton_setIsSelected = 0x5318FD;
constexpr uint32_t WebView_create = rwlib::cocos2d::plugin::WebView::create, WebView_setScalesPageToFit = rwlib::cocos2d::plugin::WebView::setScalesPageToFit;
constexpr uint32_t WebView_loadUrl = rwlib::cocos2d::plugin::WebView::loadUrl, WebView_loadPostUrl = rwlib::cocos2d::plugin::WebView::loadPostUrl;
constexpr uint32_t WebViewImpl_shouldStartLoading = rwlib::cocos2d::plugin::WebViewImpl::shouldStartLoading;      // (index, const string&)
constexpr uint32_t FunctionWebView_swap = 0x4821B5;        // std::function<void(WebView*, string)>::swap
constexpr uint32_t FunctionNode_copy = 0x474D99;           // std::function<void(Node*)>(const function&)
constexpr uint32_t FunctionNode_move = 0x481F0D;           // std::function<void(Node*)>(function&&)
constexpr uint32_t FunctionVV_call = 0x470E69;             // std::function<void()>::operator()
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t String_notEqual = 0x47CBD5;             // (const string&, const char*): true when they differ
constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_getOptionState = 0x4EB16D;   // (shared_ptr out, app)
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_textFieldToUiText = 0x45C465;       // (TextField*, const char*, const char* font)
constexpr uint32_t SceneUtil_textFieldToEditBox = 0x45C0A1;
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;        // (Button*, Text*, bool, bool)
constexpr uint32_t SceneUtil_openWebViewInfoPopup = 0x47463D;    // (title, url, function<void(Node*)>, post)
constexpr uint32_t SM_getPopupLayer = 0x613E69, SM_getPopupLayer2 = 0x613E71;
constexpr uint32_t SM_openPopup = 0x61524D;                // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_setEnabledTouchEffect = 0x60CF65;
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1, SM_canFinishScene = 0x6214F5;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;        // (api, const function<void(bool)>&, bool)
constexpr uint32_t APIManager_executeSystemInformationList151203 = 0x4CDAC9;
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595;     // (function<void(bool)> by value)
constexpr uint32_t SideMenu_changeScene = 0x5AAB41;              // sub_5AAB40
constexpr uint32_t OptionOnLabel = 0x46E6C9, OptionOffLabel = 0x46E7D1;
// GOT: the popup fade time (&float), TXT_SERIAL_INPUT (&const char*), the __func vtable of a
// std::function<Layer*()> holding a function pointer, createSceneLayer<ScenePartyTop>
constexpr uint32_t GOT_POPUP_FADE_TIME = 0xFD65C0, GOT_TXT_SERIAL_INPUT = 0xFD66DC;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_PARTY_TOP = 0xFD6620;
}  // namespace g

constexpr uint32_t S_Empty = 0xDF8A1A;        // ""
constexpr uint32_t S_CloseButton = 0xDF8D2F;
constexpr uint32_t SM_FLAG_4CC = 0x4CC, SM_CALLBACK_548 = 0x548;
constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214, V_removeChild = 0x12C;
constexpr uint32_t V_setTouchEnabled = 0x29C, V_setEnabled = 0x298, V_getString = 0x150;

// ------------------------------------------------------------------------------------------------ settings
constexpr uint32_t S_OptionWin = 0xDF934F;    // "cocostudio/publish/OptionDialog_Win.json"
constexpr uint32_t S_OptionPanel = 0xDF937D;  // "cocostudio/publish/OptionDialog_Panel.json"
constexpr uint32_t S_Area = 0xDF9378, S_BGM = 0xDF93A8, S_TxtBGM = 0xDF93AC, S_Slider = 0xDF93B4;
constexpr uint32_t S_SE = 0xDF93BB, S_TxtSE = 0xDF93BE, S_AP = 0xDF93C5, S_TxtAP = 0xDF93C8;
constexpr uint32_t S_ButtonON = 0xDF93CF, S_ButtonOFF = 0xDF93DF, S_But04 = 0xDF93D9;
constexpr uint32_t S_Relief = 0xDF93EA, S_TxtRelief = 0xDF93F1, S_Events = 0xDF93FC, S_TxtEvents = 0xDF9403;
constexpr uint32_t S_TxtON = 0xDFA01F, S_TxtOFF = 0xDFA017;
constexpr int32_t TXT_BGM = 100500007, TXT_SE = 100500008, TXT_AP = 100500004, TXT_RELIEF = 100500005;
constexpr int32_t TXT_EVENTS = 100500006, TXT_ON = 100100032, TXT_OFF = 100100033;
constexpr uint32_t VT_BgmSlider = 0xF549D8, VT_SeSlider = 0xF549AC;   // {slider}
// on / off pairs (heap 0x18: +4 one button, +0xC the other, +0x10 shared_ptr<OptionState>)
constexpr uint32_t VT_ApOff = 0xF54980, VT_ApOn = 0xF54954, VT_ReliefOff = 0xF54928, VT_ReliefOn = 0xF548FC;
constexpr uint32_t VT_EventsOff = 0xF548D0, VT_EventsOn = 0xF548A4;
constexpr uint32_t VT_OptionClose = 0xF54878, VT_OptionCheck = 0xF5484C;
constexpr uint32_t VT_OptionResult = 0xF5479C;   // heap 0x38: the buttons, sliders, popup, shadow, layer
constexpr uint32_t API_OPTION = 0x31C;           // the user's option flags (AP, relief, events bytes)

// ------------------------------------------------------------------------------------------------ serial codes
constexpr uint32_t S_SerialJson = 0xE005F7;   // "cocostudio/publish/PopupNormal_SerialInput.json"
constexpr uint32_t S_TxtWording = 0xDF862F, S_TxtSerialInput = 0xE00627;
constexpr uint32_t S_FontSoGei = 0xDF9F4D, S_FontCalligraphy = 0xDF9F1F;
constexpr uint32_t S_ButtonSend = 0xE00637, S_ButtonList = 0xE00643, S_TxtSend = 0xE0064F, S_TxtList = 0xE00658;
constexpr int32_t TXT_WORDING = 104000002, TXT_SERIAL_INPUT = 104000010, TXT_SEND = 104000001, TXT_LIST = 104000008;
constexpr uint32_t VT_SerialSend = 0xF5BAB8;     // heap 0x18 {edit box, this, list, close, this}
constexpr uint32_t VT_SerialList = 0xF5BA60;     // heap 0x14 {this, this, list, close}
constexpr uint32_t VT_SerialClose = 0xF5BA34, VT_SerialCheck = 0xF5BA08;   // {}, {close}
constexpr uint32_t VT_SerialResult = 0xF5B9B0;   // heap 0x40: sub_5A6688
constexpr uint32_t VT_SerialOpened = 0xF5B984;
constexpr uint32_t VT_SerialClosed = 0xF5B9DC;   // heap 0x30 {layer, shadow, widget, done}: sub_5A69A8
// OtherMenuDialog members
constexpr uint32_t F_EDIT_FIELD = 0x270, F_SEND = 0x280, F_SEND_TEXT = 0x284, F_SERIAL_TEXT = 0x290;
constexpr uint32_t F_EDIT_NODE = 0x294, F_EDIT_LABEL = 0x298, F_CLOSING = 0x29C;

// ------------------------------------------------------------------------------------------------ legal
constexpr uint32_t S_LegalJson = 0xE005AF;    // "cocostudio/publish/Legal.json"
constexpr uint32_t S_UserAgreementButton = 0xE005CD, S_TxtLegal1 = 0xE005E1, S_LicenseButton = 0xE0054D;
constexpr uint32_t S_TxtLegal3 = 0xE005EC;
constexpr int32_t TXT_LEGAL1 = 104040001, TXT_LEGAL3 = 104040003;
constexpr uint32_t VT_LegalClose = 0xF5BC70, VT_LegalAgreement = 0xF5BC44, VT_LegalLicense = 0xF5BC18;
constexpr uint32_t VT_LegalOpened = 0xF5BBEC, VT_LegalCheck = 0xF5BBC0;
constexpr uint32_t VT_LegalResult = 0xF5BB68;    // heap 0x30 {close, shadow, widget, layer, done}: sub_5A76C4
constexpr uint32_t VT_LegalClosed = 0xF5BB94;    // heap 0x30 {layer, shadow, widget, done}: sub_5A79D4

// ------------------------------------------------------------------------------------------------ notices
constexpr uint32_t S_InfoJson = 0xDF9B2F;     // "cocostudio/publish/Information2_Base.json"
constexpr uint32_t S_TxtTitleLabel = 0xDF9554, S_ScrollArea = 0xDF9B59, S_webView = 0xDF99FD;
constexpr uint32_t S_ButtonClose = 0xDF8692;
constexpr uint32_t VT_WebViewLoad = 0xF543A8;    // the web view's should-start-loading callback {}
constexpr uint32_t VT_WebOpened = 0xF54350, VT_WebCheck = 0xF54324;   // {close}
constexpr uint32_t VT_WebResult = 0xF542CC;      // heap 0x38 {close, web view, shadow, popup, function, layer}
constexpr uint32_t VT_InfoList = 0xF5C324;       // sub_5ACCEC's request callback {}: sub_5ACE24
constexpr uint32_t VT_InfoError = 0xF5C350;
constexpr int32_t TXT_NOTICES = 100600018;
constexpr uint32_t API_INFORMATION = 0x1C0;      // -> {std::string* url}
constexpr uint32_t WEBVIEW_ON_LOAD = 0x368;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
float fadeTime() { return ldf(ld32(g::GOT_POPUP_FADE_TIME)); }
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
inline void setTouch(uint32_t node, uint32_t on) { vcall(node, V_setTouchEnabled, {node, on}); }
// a label with a shadow and a text-table text
void shadowText(uint32_t text, int32_t id) {
    SceneUtil::setShadow(text);
    setTextId(text, id);
}
// The shadow of a popup: fades in from 0 to its opacity over `fade` on `layer`
uint32_t addShadow(uint32_t layer, float fade) {
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(fade), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    return shadow;
}
// the popup on the shadow, anchored in its middle at the shadow's position
void placeOnShadow(uint32_t shadow, uint32_t widget) {
    vcall(shadow, node::addChild, {shadow, widget});
    Frame f;
    const uint32_t anchor = f.alloc(8);
    call(g::Vec2_ctorXY, {anchor, fbits(0.5f), fbits(0.5f)});
    vcall(widget, node::setAnchorPoint, {widget, anchor});
    call(g::Vec2_dtor, {anchor});
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
}
// a std::function at dst holding a new heap __func (vtable, size); the captures are stored by the caller
uint32_t heapLambda(uint32_t dst, uint32_t vtable, uint32_t size) {
    st32(dst + 16, 0);
    const uint32_t obj = call(fn::operator_new, {size});
    if (obj) st32(obj, vtable);
    st32(dst + 16, obj);
    return obj;
}
void runOpenAnimation(uint32_t widget, uint32_t vtable, std::initializer_list<uint32_t> captures, bool eased) {
    Frame f;
    const uint32_t fo = vtable ? newFunction(f, vtable, captures) : newEmptyFunction(f);
    uint32_t action = call(g::SceneUtil_createOpenPopupAnimation, {widget, fo});
    if (eased) action = call(g::EaseSineOut_create, {action});
    call(g::Node_runAction, {widget, action});
    functionDestroy(fo);
}
}  // namespace

struct OtherMenu {
    static void openOptionPopup();
    static void optionOnLabel(uint32_t node);
    static void optionOffLabel(uint32_t node);
    static void openSerialCodePopup(uint32_t self, uint32_t done);
    static void serialResult(uint32_t functor, uint32_t okPtr);
    static void popupClosed(uint32_t functor);
    static void openLegalPopup(uint32_t self, uint32_t done);
    static void legalResult(uint32_t functor, uint32_t okPtr);
    static void noticeTouched(uint32_t functor, uint32_t sender, uint32_t type);
    static void noticeListDone(uint32_t functor, uint32_t okPtr);
    static uint32_t openWebViewInfoPopup(uint32_t layer, uint32_t title, uint32_t url, uint32_t onClose, uint32_t post);
    static void partyTouched(uint32_t functor, uint32_t sender, uint32_t type);
    static uint32_t shouldStartLoading(uint32_t env, uint32_t clazz, uint32_t index, uint32_t url);
};

// ================================================================================================ settings
// One on / off pair under `group`: the ON and OFF image views each get an ImageButton ("But04"); each button's
// touch lambda holds the other button and the option state; the selected one shows the flag at `flag`.
struct OnOff {
    uint32_t on, off;
};
static OnOff optionPair(uint32_t group, uint32_t vtOff, uint32_t vtOn, uint32_t state, uint32_t flag) {
    OnOff r;
    const uint32_t onView = findNode(group, S_ButtonON, 9);
    call(g::OptionOnLabel, {onView});
    {
        Frame f;
        const uint32_t name = newStr(f, S_But04, 5);
        r.on = call(g::ImageButton_create, {name, 0});
        strFree(name);
    }
    vcall(onView, node::addChild, {onView, r.on});
    call(g::ImageButton_setTargetImageView, {r.on, onView});
    const uint32_t offView = findNode(group, S_ButtonOFF, 10);
    call(g::OptionOffLabel, {offView});
    {
        Frame f;
        const uint32_t name = newStr(f, S_But04, 5);
        r.off = call(g::ImageButton_create, {name, 0});
        strFree(name);
    }
    vcall(offView, node::addChild, {offView, r.off});
    call(g::ImageButton_setTargetImageView, {r.off, offView});
    for (const auto &[button, other, vt] : {std::tuple{r.off, r.on, vtOff}, std::tuple{r.on, r.off, vtOn}}) {
        const uint32_t sp = ld32(state), ctl = ld32(state + 4);
        addRefIf(ctl);
        Frame f;
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = heapLambda(fo, vt, 0x18);
        if (obj) {
            st32(obj + 4, other);
            st32(obj + 0xC, button);
            st32(obj + 0x10, sp);
            st32(obj + 0x14, ctl);
        }
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        if (!obj) releaseIf(ctl);
    }
    const uint32_t selected = ld8(ld32(state) + flag);
    call(g::ImageButton_setSelectEnabled, {r.on, 1});
    call(g::ImageButton_setIsSelected, {r.on, selected != 0 ? 1u : 0u});
    call(g::ImageButton_setSelectEnabled, {r.off, 1});
    call(g::ImageButton_setIsSelected, {r.off, selected == 0 ? 1u : 0u});
    return r;
}
static uint32_t optionSlider(uint32_t group, uint32_t vtable, float value) {
    const uint32_t slider = findNode(group, S_Slider, 6);
    call(g::Slider_setPercent, {slider, uint32_t(int32_t(value * 100.0f))});
    Frame f;
    const uint32_t fo = newFunction(f, vtable, {slider});
    call(g::Slider_addEventListener, {slider, fo});
    functionDestroy(fo);
    return slider;
}

void OtherMenu::openOptionPopup() {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = addShadow(layer, 0.1f);
    const uint32_t win = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_OptionWin});
    centerPopup(win);
    vcall(shadow, node::addChild, {shadow, win});
    const uint32_t area = findNode(win, S_Area, 4);
    const uint32_t scroll = call(g::ScrollView_create, {});
    vcall(scroll, node::setContentSize, {scroll, vcall(area, node::getContentSize, {area})});
    // the option state, refreshed from the user's flags
    Frame f;
    const uint32_t state = f.alloc(8);
    call(g::AppState_getOptionState, {state, call(g::AppState_getInstance, {})});
    {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t flags = ld32(api + API_OPTION);
        st8(ld32(state) + 8, ld8(flags));
        st8(ld32(state) + 10, ld8(ld32(api + API_OPTION) + 2));
        st8(ld32(state) + 9, ld8(ld32(api + API_OPTION) + 1));
    }
    const uint32_t panel = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_OptionPanel});
    vcall(scroll, node::addChild, {scroll, panel});
    call(g::ScrollView_setInnerContainerSize, {scroll, vcall(panel, node::getContentSize, {panel})});
    vcall(area, node::addChild, {area, scroll});

    const uint32_t bgm = findNode(panel, S_BGM, 3);
    shadowText(findNode(panel, S_TxtBGM, 7), TXT_BGM);
    const uint32_t bgmSlider = optionSlider(bgm, VT_BgmSlider, ldf(ld32(state)));
    const uint32_t se = findNode(panel, S_SE, 2);
    shadowText(findNode(panel, S_TxtSE, 6), TXT_SE);
    const uint32_t seSlider = optionSlider(se, VT_SeSlider, ldf(ld32(state) + 4));
    const uint32_t apGroup = findNode(panel, S_AP, 2);
    shadowText(findNode(apGroup, S_TxtAP, 6), TXT_AP);
    const OnOff ap = optionPair(apGroup, VT_ApOff, VT_ApOn, state, 8);
    const uint32_t reliefGroup = findNode(panel, S_Relief, 6);
    shadowText(findNode(reliefGroup, S_TxtRelief, 10), TXT_RELIEF);
    const OnOff relief = optionPair(reliefGroup, VT_ReliefOff, VT_ReliefOn, state, 9);
    const uint32_t eventsGroup = findNode(panel, S_Events, 6);
    shadowText(findNode(eventsGroup, S_TxtEvents, 10), TXT_EVENTS);
    const OnOff events = optionPair(eventsGroup, VT_EventsOff, VT_EventsOn, state, 10);

    const uint32_t close = findNode(win, S_CloseButton, 12);
    setVisible(close, 1);
    addTouch(close, VT_OptionClose, {});
    runOpenAnimation(win, 0, {}, false);
    {
        const uint32_t m = sceneManager();
        Frame t;
        const uint32_t check = newFunction(t, VT_OptionCheck, {});
        const uint32_t result = t.alloc(20);
        const uint32_t obj = heapLambda(result, VT_OptionResult, 0x38);
        if (obj) {
            const uint32_t caps[] = {close, close, ap.on, ap.off, relief.on, relief.off, events.on, events.off,
                                     bgmSlider, seSlider, win, shadow, layer};
            for (uint32_t k = 0; k < 13; k++) st32(obj + 4 + 4 * k, caps[k]);
        }
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(check);
    }
    releaseIf(ld32(state + 4));
}

// the "ON" / "OFF" label of an option button
void OtherMenu::optionOnLabel(uint32_t node) { shadowText(findNode(node, S_TxtON, 6), TXT_ON); }
void OtherMenu::optionOffLabel(uint32_t node) { shadowText(findNode(node, S_TxtOFF, 7), TXT_OFF); }

// ================================================================================================ serial codes
// The serial code popup: the input field becomes an edit box; Send (disabled until a code is typed), List and
// Close; the popup's result lambda closes it and then calls `done`.
void OtherMenu::openSerialCodePopup(uint32_t self, uint32_t done) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = addShadow(layer, 0.1f);
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_SerialJson});
    placeOnShadow(shadow, widget);
    titleLabel(widget, S_TxtWording, 0xB, TXT_WORDING);
    titleLabel(widget, S_TxtSerialInput, 0xF, TXT_SERIAL_INPUT);
    uint32_t field;
    {
        Frame f;
        const uint32_t name = newStr(f, ld32(ld32(g::GOT_TXT_SERIAL_INPUT)));
        field = SceneUtil::searchNodeByName(widget, name);
        strFree(name);
    }
    st32(self + F_SERIAL_TEXT, call(g::SceneUtil_textFieldToUiText, {field, S_Empty, S_FontSoGei}));
    st8(self + F_CLOSING, 0);
    uint32_t editBox;
    {
        Frame f;
        const uint32_t placeholder = f.alloc(12);
        vcall(field, V_getString, {placeholder, field});
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, fbits(400.0f), fbits(87.0f)});
        editBox = rw::call(g::SceneUtil_textFieldToEditBox, {self + F_EDIT_NODE, self + F_EDIT_LABEL, field, placeholder,
                                                             S_Empty, S_FontCalligraphy, size, self + F_EDIT_FIELD,
                                                             self + F_CLOSING, 0, 0});
        strFree(placeholder);
    }
    setVisible(editBox, 0);
    Frame f;
    // the popup closure {widget, layer, shadow, done}
    const uint32_t doneCopy = f.alloc(20);
    call(fn::function_vv_copy, {doneCopy, done});
    st32(self + F_SEND, findNode(widget, S_ButtonSend, 0xB));
    const uint32_t list = findNode(widget, S_ButtonList, 0xB);
    const uint32_t close = findNode(widget, S_CloseButton, 0xC);
    {
        const uint32_t send = ld32(self + F_SEND);
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = heapLambda(fo, VT_SerialSend, 0x18);
        if (obj) {
            st32(obj + 4, editBox);
            st32(obj + 8, self);
            st32(obj + 0xC, list);
            st32(obj + 0x10, close);
            st32(obj + 0x14, self);
        }
        call(fn::Widget_addTouchEventListener, {send, fo});
        functionDestroy(fo);
    }
    titleLabel(ld32(self + F_SEND), S_TxtSend, 8, TXT_SEND);
    st32(self + F_SEND_TEXT, findNode(ld32(self + F_SEND), S_TxtSend, 8));
    call(g::SceneUtil_setButtonEnabled, {ld32(self + F_SEND), ld32(self + F_SEND_TEXT), 0, 0});
    {
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = heapLambda(fo, VT_SerialList, 0x14);
        if (obj) {
            st32(obj + 4, self);
            st32(obj + 8, self);
            st32(obj + 0xC, list);
            st32(obj + 0x10, close);
        }
        call(fn::Widget_addTouchEventListener, {list, fo});
        functionDestroy(fo);
    }
    titleLabel(list, S_TxtList, 8, TXT_LIST);
    addTouch(close, VT_SerialClose, {});
    // the result lambda's closure {this, this, list, close, widget, layer, shadow, done} (copied twice on the way)
    const uint32_t closureDone = f.alloc(20);
    functionCopy(closureDone, doneCopy);
    {
        const uint32_t m = sceneManager();
        const uint32_t check = newFunction(f, VT_SerialCheck, {close});
        const uint32_t tmpDone = f.alloc(20);
        functionCopy(tmpDone, closureDone);
        const uint32_t result = f.alloc(20);
        st32(result + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x40});
        if (obj) {
            st32(obj, VT_SerialResult);
            st32(obj + 8, self);
            st32(obj + 0xC, self);
            st32(obj + 0x10, list);
            st32(obj + 0x14, close);
            st32(obj + 0x18, widget);
            st32(obj + 0x1C, layer);
            st32(obj + 0x20, shadow);
            call(fn::function_vv_move, {obj + 0x28, tmpDone});
        }
        st32(result + 16, obj);
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(tmpDone);
        functionDestroy(check);
    }
    runOpenAnimation(widget, VT_SerialOpened, {}, false);
    functionDestroy(closureDone);
    functionDestroy(doneCopy);
}

// The serial popup's result (false: cancelled: the cancel sound, buttons off): the shadow fades and the close
// animation removes the popup, then `done`.
void OtherMenu::serialResult(uint32_t functor, uint32_t okPtr) {
    if (!ld8(okPtr)) {
        const uint32_t self = ld32(functor + 8);
        call(g::AudioPlaySystemSECancel, {});
        st8(self + F_CLOSING, 1);
        setTouch(ld32(ld32(functor + 0xC) + F_SEND), 0);
        setTouch(ld32(functor + 0x10), 0);
        setTouch(ld32(functor + 0x14), 0);
    }
    const uint32_t widget = ld32(functor + 0x18), layer = ld32(functor + 0x1C), shadow = ld32(functor + 0x20);
    Frame f;
    const uint32_t done = f.alloc(20);
    call(fn::function_vv_copy, {done, functor + 0x28});
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapFunction(fo, VT_SerialClosed, 0x30, 0x18, fn::function_vv_move, done);
    if (obj) {
        st32(obj + 8, layer);
        st32(obj + 0xC, shadow);
        st32(obj + 0x10, widget);
    }
    call(g::Node_runAction, {widget, call(g::SceneUtil_createClosePopupAnimation, {widget, fo})});
    functionDestroy(fo);
    functionDestroy(done);
    const uint32_t sh = ld32(functor + 0x20);
    call(g::Node_runAction, {sh, call(g::FadeTo_create, {fbits(0.1f), 0})});
}

// A popup's close animation ended {layer, shadow, widget, done}: both removed from the layer, then done().
void OtherMenu::popupClosed(uint32_t functor) {
    vcall(ld32(functor + 8), V_removeChild, {ld32(functor + 8), ld32(functor + 0xC), 1});
    vcall(ld32(functor + 8), V_removeChild, {ld32(functor + 8), ld32(functor + 0x10), 1});
    if (ld32(functor + 0x28)) call(g::FunctionVV_call, {functor + 0x18});
}

// ================================================================================================ legal
// Legal Information: the user agreement and license buttons, Close; the result lambda closes and calls `done`.
void OtherMenu::openLegalPopup(uint32_t, uint32_t done) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    const uint32_t shadow = addShadow(layer, fadeTime());
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_LegalJson});
    setTouch(widget, 0);
    placeOnShadow(shadow, widget);
    const uint32_t close = findNode(widget, S_CloseButton, 0xC);
    Frame f;
    // the closure {close, shadow, widget, layer, done}
    const uint32_t doneCopy = f.alloc(20);
    call(fn::function_vv_copy, {doneCopy, done});
    setTouch(close, 1);
    addTouch(close, VT_LegalClose, {});
    {
        const uint32_t button = findNode(widget, S_UserAgreementButton, 0x13);
        addTouch(button, VT_LegalAgreement, {});
        shadowText(findNode(button, S_TxtLegal1, 0xA), TXT_LEGAL1);
    }
    {
        const uint32_t button = findNode(widget, S_LicenseButton, 0xD);
        addTouch(button, VT_LegalLicense, {});
        shadowText(findNode(button, S_TxtLegal3, 0xA), TXT_LEGAL3);
    }
    runOpenAnimation(widget, VT_LegalOpened, {}, true);
    const uint32_t m = sceneManager();
    const uint32_t check = newFunction(f, VT_LegalCheck, {close});
    const uint32_t tmpDone = f.alloc(20);
    functionCopy(tmpDone, doneCopy);
    const uint32_t result = f.alloc(20);
    const uint32_t obj = heapFunction(result, VT_LegalResult, 0x30, 0x18, fn::function_vv_move, tmpDone);
    if (obj) {
        st32(obj + 8, close);
        st32(obj + 0xC, shadow);
        st32(obj + 0x10, widget);
        st32(obj + 0x14, layer);
    }
    rw::call(g::SM_openPopup, {m, check, result, 1});
    functionDestroy(result);
    functionDestroy(tmpDone);
    functionDestroy(check);
    functionDestroy(doneCopy);
}

// The legal popup's result {close, shadow, widget, layer, done} (false: the cancel sound): close off, the shadow
// fades, the eased close animation removes the popup, then done.
void OtherMenu::legalResult(uint32_t functor, uint32_t okPtr) {
    if (!ld8(okPtr)) call(g::AudioPlaySystemSECancel, {});
    {
        const uint32_t close = ld32(functor + 8);
        vcall(close, V_setEnabled, {close, 0});
    }
    {
        const uint32_t shadow = ld32(functor + 0xC);
        call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(fadeTime()), 0})});
    }
    const uint32_t widget = ld32(functor + 0x10), layer = ld32(functor + 0x14), shadow = ld32(functor + 0xC);
    Frame f;
    const uint32_t done = f.alloc(20);
    call(fn::function_vv_copy, {done, functor + 0x18});
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapFunction(fo, VT_LegalClosed, 0x30, 0x18, fn::function_vv_move, done);
    if (obj) {
        st32(obj + 8, layer);
        st32(obj + 0xC, shadow);
        st32(obj + 0x10, widget);
    }
    const uint32_t action = call(g::SceneUtil_createClosePopupAnimation, {widget, fo});
    call(g::Node_runAction, {widget, call(g::EaseSineOut_create, {action})});
    functionDestroy(fo);
    functionDestroy(done);
}

// ================================================================================================ notices
// The notices button (touch ended): the decide sound, the loading indicator, the information list request.
void OtherMenu::noticeTouched(uint32_t, uint32_t, uint32_t type) {
    if (ld32(type) != 2) return;
    call(g::AudioPlaySystemSEDecide, {});
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_InfoList, {});
    call(g::APIManager_executeSystemInformationList151203, {api, fo});
    functionDestroy(fo);
}

// The list arrived: close the menu popup and show the notices page in a web view; on failure the error dialog.
void OtherMenu::noticeListDone(uint32_t, uint32_t okPtr) {
    const uint32_t ok = ld8(okPtr);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    Frame f;
    if (ok) {
        call(g::SM_closeCurrentPopup, {sceneManager(), 0});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t td = textData(f, TXT_NOTICES);
        const uint32_t title = newStr(f, textDataStr(td));
        const uint32_t url = ld32(ld32(api + API_INFORMATION));
        const uint32_t urlStr = newStr(f, (ld8(url) & 1) ? ld32(url + 8) : url + 1);
        const uint32_t onClose = newEmptyFunction(f);
        const uint32_t post = f.alloc(12);
        st8(post, 0);
        st8(post + 1, 0);
        call(g::SceneUtil_openWebViewInfoPopup, {title, urlStr, onClose, post});
        strFree(post);
        functionDestroy(onClose);
        strFree(urlStr);
        strFree(title);
        sharedRelease(td);
        return;
    }
    if (!ld8(call(fn::APIManager_getInstance, {}) + 0x14)) return;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t fo = newFunction(f, VT_InfoError, {});
    call(g::APIManager_openErrorDialog, {api, fo, 1});
    functionDestroy(fo);
}

// A web page in a popup on `layer`: the title, a web view filling the scroll area (posting `post` when it is
// not empty), Close; the result lambda closes it and calls onClose(node).
uint32_t OtherMenu::openWebViewInfoPopup(uint32_t layer, uint32_t title, uint32_t url, uint32_t onClose,
                                         uint32_t post) {
    call(g::AudioPlaySystemSEPopUp, {});
    call(g::SM_setEnabledTouchEffect, {sceneManager(), 0});
    const uint32_t shadow = addShadow(layer, 0.15f);
    const uint32_t popup = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_InfoJson});
    setTouch(popup, 0);
    placeOnShadow(shadow, popup);
    {
        const uint32_t label = findNode(popup, S_TxtTitleLabel, 15);
        SceneUtil::setShadow(label);
        setTextC(label, strData(title));
    }
    const uint32_t area = findNode(popup, S_ScrollArea, 11);
    const uint32_t web = call(g::WebView_create, {});
    Frame f;
    {
        const float w = ldf(vcall(area, node::getContentSize, {area}));
        const float h = ldf(vcall(area, node::getContentSize, {area}) + 4);
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, fbits(w), fbits(h)});
        vcall(web, node::setContentSize, {web, size});
    }
    {
        const float w = ldf(vcall(web, node::getContentSize, {web}));
        const float h = ldf(vcall(web, node::getContentSize, {web}) + 4);
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctorXY, {pos, fbits(w * 0.5f), fbits(h * 0.5f)});
        vcall(web, node::setPosition, {web, pos});
        call(g::Vec2_dtor, {pos});
    }
    setVisible(web, 0);
    {
        const uint32_t fo = newFunction(f, VT_WebViewLoad, {});
        call(g::FunctionWebView_swap, {fo, web + WEBVIEW_ON_LOAD});
        functionDestroy(fo);
    }
    call(g::WebView_setScalesPageToFit, {web, 1});
    if (call(g::String_notEqual, {post, S_Empty}))
        call(g::WebView_loadPostUrl, {web, url, post});
    else
        call(g::WebView_loadUrl, {web, url});
    {
        const uint32_t name = newStr(f, S_webView, 7);
        vcall(area, node::addChildZName, {area, web, 0, name});
        strFree(name);
    }
    const uint32_t close = findNode(popup, S_ButtonClose, 12);
    setTouch(close, 0);
    // the closure {close, web view, shadow, popup, onClose, layer}
    const uint32_t onCloseCopy = f.alloc(20);
    call(g::FunctionNode_copy, {onCloseCopy, onClose});
    runOpenAnimation(popup, VT_WebOpened, {close}, false);
    const uint32_t m = sceneManager();
    const uint32_t check = newFunction(f, VT_WebCheck, {close});
    const uint32_t tmp = f.alloc(20);
    functionCopy(tmp, onCloseCopy);
    const uint32_t result = f.alloc(20);
    const uint32_t obj = heapFunction(result, VT_WebResult, 0x38, 0x18, g::FunctionNode_move, tmp);
    if (obj) {
        st32(obj + 8, close);
        st32(obj + 0xC, web);
        st32(obj + 0x10, shadow);
        st32(obj + 0x14, popup);
        st32(obj + 0x30, layer);
    }
    rw::call(g::SM_openPopup, {m, check, result, 1});
    functionDestroy(result);
    functionDestroy(tmp);
    functionDestroy(check);
    functionDestroy(onCloseCopy);
    return 1;
}

// ================================================================================================ party
// The party button (touch ended): when the scene may finish, request the party top and change to it; the menu
// widget stops taking touches.
void OtherMenu::partyTouched(uint32_t functor, uint32_t, uint32_t type) {
    if (ld32(type) != 2) return;
    call(g::AudioPlaySystemSEDecide, {});
    if (call(g::SM_canFinishScene, {sceneManager()}) != 1) return;
    Frame f;
    {
        const uint32_t m = sceneManager();
        const uint32_t none = newEmptyFunction(f);
        st8(m + SM_FLAG_4CC, 1);
        call(g::FunctionVB_assign, {m + SM_CALLBACK_548, none});
        functionDestroy(none);
    }
    {
        const uint32_t none = newEmptyFunction(f);
        call(g::ScenePartyTop_callPartyTopAPI, {none});
        functionDestroy(none);
    }
    {
        const uint32_t layerFn = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(g::GOT_CREATE_PARTY_TOP)});
        call(g::SideMenu_changeScene, {layerFn});
        functionDestroy(layerFn);
    }
    setTouch(ld32(functor + 8), 0);
}

// ================================================================================================ help
// Cocos2dxWebViewHelper.shouldStartLoading(index, url) from Java: the web view decides on the URL.
uint32_t OtherMenu::shouldStartLoading(uint32_t env, uint32_t, uint32_t index, uint32_t url) {
    constexpr uint32_t JNI_GetStringUTFChars = 0x2A4, JNI_ReleaseStringUTFChars = 0x2A8;
    const uint32_t chars = vcall(env, JNI_GetStringUTFChars, {env, url, 0});
    Frame f;
    const uint32_t s = newStr(f, chars);
    vcall(env, JNI_ReleaseStringUTFChars, {env, url, chars});
    const uint32_t r = call(g::WebViewImpl_shouldStartLoading, {index, s});
    strFree(s);
    return r;
}

RW_FUNCTION(0x46D19C, "SceneUtil::openOptionPopup", OtherMenu::openOptionPopup);
RW_FUNCTION(0x46E6C8, "SceneUtil::openOptionPopup ON label (sub_46E6C8)", OtherMenu::optionOnLabel);
RW_FUNCTION(0x46E7D0, "SceneUtil::openOptionPopup OFF label (sub_46E7D0)", OtherMenu::optionOffLabel);
RW_FUNCTION(0x5A058C, "OtherMenuDialog::openSerialCodePopup", OtherMenu::openSerialCodePopup);
RW_FUNCTION(0x5A6688, "OtherMenuDialog serial popup result __func::operator()(bool) (sub_5A6688)",
            OtherMenu::serialResult);
RW_FUNCTION(0x5A69A8, "OtherMenuDialog serial popup closed __func::operator() (sub_5A69A8)", OtherMenu::popupClosed);
RW_FUNCTION(0x59FEAC, "OtherMenuDialog::openLegalPopup", OtherMenu::openLegalPopup);
RW_FUNCTION(0x5A76C4, "OtherMenuDialog legal popup result __func::operator()(bool) (sub_5A76C4)",
            OtherMenu::legalResult);
RW_FUNCTION(0x5A79D4, "OtherMenuDialog legal popup closed __func::operator() (sub_5A79D4)", OtherMenu::popupClosed);
RW_FUNCTION(0x5ACCEC, "side menu notices button lambda __func::operator() (sub_5ACCEC)", OtherMenu::noticeTouched);
RW_FUNCTION(0x5ACE24, "side menu notices list callback __func::operator()(bool) (sub_5ACE24)",
            OtherMenu::noticeListDone);
RW_FUNCTION(0x474700, "SceneUtil::openWebViewInfoPopup(Layer*, ...)", OtherMenu::openWebViewInfoPopup);
RW_FUNCTION(0x5AC6C0, "side menu party button lambda __func::operator() (sub_5AC6C0)", OtherMenu::partyTouched);
RW_FUNCTION(0x81ABF0, "Java_org_cocos2dx_lib_Cocos2dxWebViewHelper_shouldStartLoading", OtherMenu::shouldStartLoading);
