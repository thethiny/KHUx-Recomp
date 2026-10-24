// OtherMenuDialog popups not written with the rest of the Other menu (w5_12/OtherMenu.cpp): the serial code list
// (with the campaign request), the user code (account transfer) popups, the movie popup and the Facebook
// login / logout button, and their lambdas.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;
constexpr uint32_t Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t SM_getPopupLayer2 = 0x613E71;
constexpr uint32_t SM_openPopup = 0x61524D;                // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;  // (api, const function<void(bool)>&, bool)
constexpr uint32_t APIManager_executeSystemSerialcodeCampaignGet = 0x4D1F29;
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t SerialCodeScrollView_create = 0x555B95;
constexpr uint32_t BaseScrollViewInt_setItemIDs = 0x478929;
constexpr uint32_t VectorInt_pushBackSlow = 0x4D6CC1;
constexpr uint32_t VectorInt_dtor = 0x478961;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1, SM_setEnabledTouchEffect = 0x60CF65;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t SceneUtil_textFieldToEditBox = 0x45C0A1, SceneUtil_setButtonEnabled = 0x468625;
constexpr uint32_t SceneUtil_splitSpaceCheck = 0x47898D, SceneUtil_openOkDialog4Line = 0x45C775;
constexpr uint32_t SceneUtil_openOkCancelDialog8Line = 0x45CB15, SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_executeCampaignPincodeCreate = 0x4D3839;
constexpr uint32_t APIManager_executeCampaignPincodeUpdate = 0x4D397D;    // (api, const char*, function)
constexpr uint32_t APIManager_executeCampaignPincodeStatusUpdate = 0x4D3C85;
constexpr uint32_t APIManager_executeSystemSerialcode = 0x4D1D65;         // (api, const char*, function)
constexpr uint32_t String_notEqual = 0x47CBD5;
constexpr uint32_t AudioPushGameMainBGM = 0x5C0FE5;
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t GOT_POPUP_FADE_TIME = 0xFD65C0;
constexpr uint32_t VideoPlayer_create = rwlib::cocos2d::experimental::ui::VideoPlayer::create;
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create, CallFunc_create = rwlib::cocos2d::CallFunc::create, Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t FunctionVB_copy = 0x45C84D, FunctionVB_move = 0x47BC69;
constexpr uint32_t FacebookManager_getInstance = 0x4E5585, FacebookManager_login = 0x4E5747;
}  // namespace g

constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214, V_setTouchEnabled = 0x29C;
constexpr uint32_t V_setContentSize = 0xA8, V_getContentSize = 0xAC;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// a popup's shadow: faded in from 0 to its opacity over 0.1 s on `layer`
uint32_t addShadow(uint32_t layer) {
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    return shadow;
}
// the popup on the shadow, anchored in its middle at the shadow's position
void placeOnShadow(uint32_t shadow, uint32_t widget) {
    vcall(shadow, node::addChild, {shadow, widget});
    {
        Frame f;
        const uint32_t anchor = f.alloc(8);
        call(g::Vec2_ctorXY, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(widget, node::setAnchorPoint, {widget, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
}
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }

// ================================================================================================ serial code list
constexpr uint32_t S_SerialWinJson = 0xE00661;   // "cocostudio/publish/Information2_SerialWin.json"
constexpr uint32_t S_TxtTitleLabel = 0xDF9554, S_TxtDescription = 0xE00690, S_ScrollArea = 0xDF9B59;
constexpr uint32_t S_ButtonClose = 0xDF8692;
constexpr int32_t TXT_LIST_TITLE = 104000008, TXT_LIST_EMPTY = 104000011;
constexpr uint32_t VT_ListLoaded = 0xF5B92C;     // {description, area, view}: sub_5A6240
constexpr uint32_t VT_ListError = 0xF5B958;
constexpr uint32_t VT_ListClose = 0xF5B900;
constexpr uint32_t VT_ListCheck = 0xF5B8D4;      // {close}
constexpr uint32_t VT_ListResult = 0xF5B87C;     // heap 0x30 {widget, layer, shadow, done}: sub_5A5D30
constexpr uint32_t VT_ListClosed = 0xF5B8A8;     // heap 0x30 {layer, shadow, widget, done}
constexpr uint32_t VT_ListOpened = 0xF5B850;
constexpr uint32_t API_SERIAL_CAMPAIGNS = 0x270; // std::vector<int>: the serialcodeReward ids

// The list of serial code campaigns: the description ("no campaigns") hidden, the list in "Scroll_Area" filled
// by the campaign request (sub_5A6240), Close; the popup's result lambda closes it and calls `done`.
void openSerialCodeListPopup(uint32_t, uint32_t done) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = addShadow(layer);
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_SerialWinJson});
    placeOnShadow(shadow, widget);
    Frame f;
    const uint32_t doneCopy = f.alloc(20);   // the closure {widget, layer, shadow, done}
    call(fn::function_vv_copy, {doneCopy, done});
    titleLabel(widget, S_TxtTitleLabel, 15, TXT_LIST_TITLE);
    {
        Frame t;
        const uint32_t text = t.alloc(8);
        call(g::TextData_get, {text, 0, uint32_t(TXT_LIST_EMPTY)});
        const uint32_t entry = ld32(text);
        releaseIf(ld32(text + 4));
        if (entry) titleLabel(widget, S_TxtDescription, 15, TXT_LIST_EMPTY);
    }
    const uint32_t description = findNode(widget, S_TxtDescription, 15);
    setVisible(description, 0);
    const uint32_t area = findNode(widget, S_ScrollArea, 11);
    setVisible(area, 1);
    st8(sceneManager() + 0x308, 1);
    const uint32_t view = call(g::SerialCodeScrollView_create, {});
    vcall(view, V_setContentSize, {view, vcall(area, V_getContentSize, {area})});
    vcall(area, node::addChild, {area, view});
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    {
        const uint32_t api = call(g::APIManager_getInstance, {});
        Frame t;
        const uint32_t fo = newFunction(t, VT_ListLoaded, {description, area, view});
        call(g::APIManager_executeSystemSerialcodeCampaignGet, {api, fo});
        functionDestroy(fo);
    }
    const uint32_t close = findNode(widget, S_ButtonClose, 12);
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_ListClose, {});
        call(g::Widget_addTouchEventListener, {close, fo});
        functionDestroy(fo);
    }
    const uint32_t closureDone = f.alloc(20);
    functionCopy(closureDone, doneCopy);
    {
        const uint32_t m = sceneManager();
        Frame t;
        const uint32_t check = newFunction(t, VT_ListCheck, {close});
        const uint32_t tmpDone = t.alloc(20);
        functionCopy(tmpDone, closureDone);
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x30});
        if (obj) {
            st32(obj, VT_ListResult);
            st32(obj + 8, widget);
            st32(obj + 0xC, layer);
            st32(obj + 0x10, shadow);
            call(fn::function_vv_move, {obj + 0x18, tmpDone});
        }
        st32(result + 16, obj);
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(tmpDone);
        functionDestroy(check);
    }
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_ListOpened, {});
        const uint32_t anim = call(g::EaseSineOut_create, {call(g::SceneUtil_createOpenPopupAnimation, {widget, fo})});
        call(g::Node_runAction, {widget, anim});
        functionDestroy(fo);
    }
    functionDestroy(closureDone);
    functionDestroy(doneCopy);
}

// sub_5A6240, the campaign request's callback {description, area, view}: the loading indicator off; on success
// the list gets the campaign ids, or the description shows when there are none; a failure shows the error dialog
// when the request asks for one
void serialListLoaded(uint32_t functor, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    const uint32_t api = call(g::APIManager_getInstance, {});
    if (!ok) {
        if (!ld8(api + 0x14)) return;
        const uint32_t api2 = call(g::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_ListError, {});
        call(g::APIManager_openErrorDialog, {api2, fo, 1});
        functionDestroy(fo);
        return;
    }
    const uint32_t ids = api + API_SERIAL_CAMPAIGNS;
    if (ld32(ids + 4) == ld32(ids)) {
        const uint32_t description = ld32(functor + 4), area = ld32(functor + 8);
        setVisible(description, 1);
        setVisible(area, 0);
        vcall(ld32(functor + 8), V_setTouchEnabled, {ld32(functor + 8), 0});
        return;
    }
    Frame f;
    const uint32_t vec = f.alloc(12);
    st32(vec, 0);
    st32(vec + 4, 0);
    st32(vec + 8, 0);
    for (uint32_t i = 1, off = 0;; i++, off += 4) {
        const uint32_t item = ld32(ids) + off;
        const uint32_t end = ld32(vec + 4);
        if (end == ld32(vec + 8)) {
            call(g::VectorInt_pushBackSlow, {vec, item});
        } else {
            if (end) st32(end, ld32(item));
            st32(vec + 4, (end ? end : 0) + 4);
        }
        if (i >= (ld32(ids + 4) - ld32(ids)) >> 2) break;
    }
    call(g::BaseScrollViewInt_setItemIDs, {ld32(functor + 0xC), vec});
    call(g::VectorInt_dtor, {vec});
}

// sub_5A5D30, the list popup's result lambda {.., widget, layer, shadow, done}: when closed by the player, the
// cancel sound, the popup flag cleared, the close animation ($_ {layer, shadow, widget, done} at its end) and the
// shadow faded out
void serialListResult(uint32_t functor, uint32_t okRef) {
    if (ld8(okRef)) return;
    call(g::AudioPlaySystemSECancel, {});
    st8(sceneManager() + 0x308, 0);
    const uint32_t widget = ld32(functor + 8), layer = ld32(functor + 0xC), shadow = ld32(functor + 0x10);
    Frame f;
    const uint32_t doneCopy = f.alloc(20);
    call(fn::function_vv_copy, {doneCopy, functor + 0x18});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x30});
    if (obj) {
        st32(obj, VT_ListClosed);
        st32(obj + 8, layer);
        st32(obj + 0xC, shadow);
        st32(obj + 0x10, widget);
        call(fn::function_vv_move, {obj + 0x18, doneCopy});
    }
    st32(fo + 16, obj);
    const uint32_t anim = call(g::EaseSineOut_create, {call(g::SceneUtil_createClosePopupAnimation, {widget, fo})});
    call(g::Node_runAction, {widget, anim});
    functionDestroy(fo);
    functionDestroy(doneCopy);
    const uint32_t s = ld32(functor + 0x10);
    call(g::Node_runAction, {s, call(g::FadeTo_create, {fbits(0.1f), 0})});
}
}  // namespace

RW_FUNCTION(0x5A1248, "OtherMenuDialog::openSerialCodeListPopup(function<void()>)", openSerialCodeListPopup);
RW_FUNCTION(0x5A6240, "OtherMenuDialog serial code list loaded (sub_5A6240)", serialListLoaded);
RW_FUNCTION(0x5A5D30, "OtherMenuDialog serial code list result (sub_5A5D30)", serialListResult);

// ================================================================================================ shared steps
namespace {
// the popup's SceneManager::openPopup step: the closure {widget, layer, shadow, done(, self)} is copied, the
// check lambda {close} and the result lambda (heap: the three nodes at +8, the done function moved in at +0x18,
// self at +0x30 when `withSelf`) go to openPopup; the open animation follows ($_ vtOpened); the closure copy is
// destroyed last (the caller destroys its own `doneCopy`)
void openWithResult(uint32_t widget, uint32_t layer, uint32_t shadow, uint32_t doneCopy, uint32_t close,
                    uint32_t vtCheck, uint32_t vtResult, uint32_t vtOpened, bool eased, bool withSelf = false,
                    uint32_t self = 0) {
    Frame f;
    const uint32_t closure = f.alloc(20);
    functionCopy(closure, doneCopy);
    const uint32_t m = sceneManager();
    {
        Frame t;
        const uint32_t check = newFunction(t, vtCheck, {close});
        const uint32_t tmp = t.alloc(20);
        functionCopy(tmp, closure);
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t obj = call(fn::operator_new, {withSelf ? 0x38u : 0x30u});
        if (obj) {
            st32(obj, vtResult);
            st32(obj + 8, widget);
            st32(obj + 0xC, layer);
            st32(obj + 0x10, shadow);
            call(fn::function_vv_move, {obj + 0x18, tmp});
            if (withSelf) st32(obj + 0x30, self);
        }
        st32(result + 16, obj);
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(tmp);
        functionDestroy(check);
    }
    {
        Frame t;
        const uint32_t fo = newFunction(t, vtOpened, {});
        uint32_t anim = call(g::SceneUtil_createOpenPopupAnimation, {widget, fo});
        if (eased) anim = call(g::EaseSineOut_create, {anim});
        call(g::Node_runAction, {widget, anim});
        functionDestroy(fo);
    }
    functionDestroy(closure);
}

// the popup closes itself: the close animation ($_ vtClosed {layer, shadow, widget, done} at its end) and the
// shadow faded out; `nodes` holds {widget, layer, shadow} and the done function at nodes + 0x10
void closeWithAnimation(uint32_t nodes, uint32_t vtClosed, bool eased) {
    const uint32_t widget = ld32(nodes), layer = ld32(nodes + 4), shadow = ld32(nodes + 8);
    Frame f;
    const uint32_t doneCopy = f.alloc(20);
    call(fn::function_vv_copy, {doneCopy, nodes + 0x10});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x30});
    if (obj) {
        st32(obj, vtClosed);
        st32(obj + 8, layer);
        st32(obj + 0xC, shadow);
        st32(obj + 0x10, widget);
        call(fn::function_vv_move, {obj + 0x18, doneCopy});
    }
    st32(fo + 16, obj);
    uint32_t anim = call(g::SceneUtil_createClosePopupAnimation, {widget, fo});
    if (eased) anim = call(g::EaseSineOut_create, {anim});
    call(g::Node_runAction, {widget, anim});
    functionDestroy(fo);
    functionDestroy(doneCopy);
    const uint32_t s = ld32(nodes + 8);
    call(g::Node_runAction, {s, call(g::FadeTo_create, {fbits(0.1f), 0})});
}

inline void addTouch(uint32_t widget, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    const uint32_t fo = newFunction(f, vtable, captures);
    call(g::Widget_addTouchEventListener, {widget, fo});
    functionDestroy(fo);
}

// ================================================================================================ user codes
constexpr uint32_t S_TxtHeading = 0xE006CA, S_TxtWording = 0xDF862F;
constexpr uint32_t S_TopJson = 0xE006A0, S_IssueJson = 0xE00713, S_Input1Json = 0xE0074E, S_Input2Json = 0xE007A2;
constexpr uint32_t S_ButtonIssue = 0xE006D6, S_TxtIssue = 0xE006E3, S_ButtonInput = 0xE006ED, S_TxtInput = 0xE006FA;
constexpr uint32_t S_CloseButton0 = 0xE00704, S_CloseButton = 0xDF8D2F, S_TxtIssueCode = 0xE0073F;
constexpr uint32_t S_ButtonCertification = 0xE0077B, S_TxtCertification = 0xE00790;
constexpr uint32_t S_ButtonOK = 0xDF8688, S_TxtOK = 0xDF869F;
constexpr uint32_t S_FontSoGei = 0xDF9F4D, S_Empty = 0xDF8A1A;
constexpr uint32_t GOT_TXT_CODE_INPUT = 0xFD66E0;
constexpr uint32_t API_USER_CODE = 0x360;        // std::string: the issued code
// OtherMenuDialog members (the code input)
constexpr uint32_t F_EDIT_FIELD = 0x270, F_CERT_BUTTON = 0x288, F_CERT_TEXT = 0x28C, F_CODE_TEXT = 0x290;
constexpr uint32_t F_EDIT_NODE = 0x294, F_EDIT_LABEL = 0x298, F_BUSY = 0x29C;

uint32_t popupBase(uint32_t json, uint32_t &layer, uint32_t &shadow) {
    call(g::AudioPlaySystemSEPopUp, {});
    layer = call(g::SM_getPopupLayer2, {sceneManager()});
    shadow = addShadow(layer);
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), json});
    placeOnShadow(shadow, widget);
    return widget;
}

// The account transfer top popup: Issue (sub_5A5858), Input (sub_5A5728) and Close; the result lambda
// (sub_5A5228) closes it and calls `done`.
void openUserCodePopup(uint32_t self, uint32_t done) {
    uint32_t layer, shadow;
    const uint32_t widget = popupBase(S_TopJson, layer, shadow);
    titleLabel(widget, S_TxtHeading, 11, 104700001);
    titleLabel(widget, S_TxtWording, 11, 104700002);
    Frame f;
    const uint32_t doneCopy = f.alloc(20);
    call(fn::function_vv_copy, {doneCopy, done});
    const uint32_t issue = findNode(widget, S_ButtonIssue, 12);
    addTouch(issue, 0xF5B7CC, {self});
    titleLabel(issue, S_TxtIssue, 9, 104700003);
    const uint32_t input = findNode(widget, S_ButtonInput, 12);
    addTouch(input, 0xF5B7A0, {self});
    titleLabel(input, S_TxtInput, 9, 104700004);
    const uint32_t close = findNode(widget, S_CloseButton0, 14);
    addTouch(close, 0xF5B774, {});
    openWithResult(widget, layer, shadow, doneCopy, close, 0xF5B748, 0xF5B6F0, 0xF5B6C4, false);
    functionDestroy(doneCopy);
}

// The issued code popup: the code (APIManager +0x360) and Close; the result lambda (sub_5A4BA8) closes it.
void openIssueUserCodePopup(uint32_t, uint32_t done) {
    uint32_t layer, shadow;
    const uint32_t widget = popupBase(S_IssueJson, layer, shadow);
    titleLabel(widget, S_TxtHeading, 11, 104700005);
    titleLabel(widget, S_TxtWording, 11, 104700006);
    Frame f;
    const uint32_t doneCopy = f.alloc(20);
    call(fn::function_vv_copy, {doneCopy, done});
    const uint32_t close = findNode(widget, S_CloseButton, 12);
    addTouch(close, 0xF5B698, {});
    const uint32_t code = findNode(widget, S_TxtIssueCode, 14);
    SceneUtil::setShadow(code);
    call(fn::Text_setString, {code, call(g::APIManager_getInstance, {}) + API_USER_CODE});
    openWithResult(widget, layer, shadow, doneCopy, close, 0xF5B66C, 0xF5B614, 0xF5B5E8, false);
    functionDestroy(doneCopy);
}

// The code input popup: the input field becomes a Text (shown) plus an edit box (hidden); Certification is
// disabled until a code is typed (sub_5A45F4 sends it); the result lambda (sub_5A40DC) closes it.
void openInputUserCodePopup(uint32_t self, uint32_t done) {
    uint32_t layer, shadow;
    const uint32_t widget = popupBase(S_Input1Json, layer, shadow);
    titleLabel(widget, S_TxtHeading, 11, 104700007);
    titleLabel(widget, S_TxtWording, 11, 104700008);
    Frame f;
    const uint32_t doneCopy = f.alloc(20);
    call(fn::function_vv_copy, {doneCopy, done});
    const uint32_t close = findNode(widget, S_CloseButton, 12);
    addTouch(close, 0xF5B5BC, {});
    st32(self + F_CERT_BUTTON, findNode(widget, S_ButtonCertification, 20));
    titleLabel(ld32(self + F_CERT_BUTTON), S_TxtCertification, 17, 104700009);
    uint32_t field;
    {
        Frame t;
        const uint32_t name = newStr(t, ld32(ld32(GOT_TXT_CODE_INPUT)));
        field = SceneUtil::searchNodeByName(widget, name);
        strFree(name);
    }
    // the field's text: a ui::Text in its place (same position, anchor, font size and area), empty
    const uint32_t parent = vcall(field, 0x11C, {field});
    const uint32_t text = call(rwlib::cocos2d::ui::Text::create, {});                       // ui::Text::create()
    st32(self + F_CODE_TEXT, text);
    vcall(text, node::setPosition, {text, vcall(field, node::getPosition, {field})});
    {
        const uint32_t t = ld32(self + F_CODE_TEXT);
        vcall(t, node::setAnchorPoint, {t, vcall(field, 0xA0, {field})});
    }
    {
        const uint32_t t = ld32(self + F_CODE_TEXT);
        Frame t2;
        const uint32_t font = newStr(t2, S_FontSoGei, 0x22);
        call(rwlib::cocos2d::ui::Text::setFontName, {t, font});                                  // Text::setFontName
        strFree(font);
    }
    {
        const uint32_t t = ld32(self + F_CODE_TEXT);
        call(rwlib::cocos2d::ui::Text::setFontSize, {t, call(rwlib::cocos2d::ui::TextField::getFontSize, {field})});               // setFontSize(TextField::getFontSize)
    }
    {
        const uint32_t t = ld32(self + F_CODE_TEXT);
        call(rwlib::cocos2d::ui::Text::setTextAreaSize, {t, vcall(field, V_getContentSize, {field})});   // setTextAreaSize
    }
    {
        const uint32_t t = ld32(self + F_CODE_TEXT);
        const uint32_t setSize = ld32(ld32(t) + V_setContentSize);
        call(setSize, {t, vcall(field, V_getContentSize, {field})});
    }
    {
        const uint32_t t = ld32(self + F_CODE_TEXT);
        vcall(t, 0x2D4, {t, 0});
    }
    {
        const uint32_t t = ld32(self + F_CODE_TEXT);
        Frame t2;
        const uint32_t empty = newStr(t2, S_Empty, 0);
        call(fn::Text_setString, {t, empty});
        strFree(empty);
    }
    {
        const uint32_t add = ld32(ld32(parent) + 0xF4);
        const uint32_t t = ld32(self + F_CODE_TEXT);
        call(add, {parent, t, vcall(field, 0x18, {field})});
    }
    st8(self + F_BUSY, 0);
    uint32_t editBox;
    {
        Frame t;
        const uint32_t placeholder = t.alloc(12);
        vcall(field, 0x150, {placeholder, field});
        const uint32_t size = t.alloc(8);
        call(g::Size_ctor, {size, fbits(400.0f), fbits(100.0f)});
        editBox = rw::call(g::SceneUtil_textFieldToEditBox, {self + F_EDIT_NODE, self + F_EDIT_LABEL, field, placeholder,
                                                             S_Empty, S_FontSoGei, size, self + F_EDIT_FIELD,
                                                             self + F_BUSY, 0, 0});
        strFree(placeholder);
    }
    setVisible(editBox, 0);
    st32(self + F_CERT_TEXT, findNode(ld32(self + F_CERT_BUTTON), S_TxtCertification, 17));
    call(g::SceneUtil_setButtonEnabled, {ld32(self + F_CERT_BUTTON), ld32(self + F_CERT_TEXT), 0, 0});
    addTouch(ld32(self + F_CERT_BUTTON), 0xF5B538, {self, editBox});
    openWithResult(widget, layer, shadow, doneCopy, close, 0xF5B50C, 0xF5B4B4, 0xF5B488, false, true, self);
    functionDestroy(doneCopy);
}

// The code accepted popup: OK closes it (sub_5A3B88).
void openInputSuccessUserCodePopup(uint32_t, uint32_t done) {
    uint32_t layer, shadow;
    const uint32_t widget = popupBase(S_Input2Json, layer, shadow);
    titleLabel(widget, S_TxtHeading, 11, 104700007);
    titleLabel(widget, S_TxtWording, 11, 104700010);
    Frame f;
    const uint32_t doneCopy = f.alloc(20);
    call(fn::function_vv_copy, {doneCopy, done});
    const uint32_t ok = findNode(widget, S_ButtonOK, 9);
    titleLabel(ok, S_TxtOK, 6, 100100001);
    {
        Frame t;
        const uint32_t closure = t.alloc(20);
        functionCopy(closure, doneCopy);
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x30});
        if (obj) {
            st32(obj, 0xF5B430);
            st32(obj + 8, widget);
            st32(obj + 0xC, layer);
            st32(obj + 0x10, shadow);
            call(fn::function_vv_move, {obj + 0x18, closure});
        }
        st32(fo + 16, obj);
        call(g::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        functionDestroy(closure);
    }
    {
        Frame t;
        const uint32_t fo = newFunction(t, 0xF5B404, {});
        call(g::Node_runAction, {widget, call(g::SceneUtil_createOpenPopupAnimation, {widget, fo})});
        functionDestroy(fo);
    }
    functionDestroy(doneCopy);
}

// sub_5A5858, Issue {self}: the pincode create request ($_ sub_5A59A0)
void issueTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t self = ld32(functor + 4);
    call(g::AudioPlaySystemSEDecide, {});
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(g::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, 0xF5B7F8, {self});
    call(g::APIManager_executeCampaignPincodeCreate, {api, fo});
    functionDestroy(fo);
}
// sub_5A59A0, the pincode create callback: on success the top popup closes and the issued code popup opens (its
// `this` is closeCurrentPopup's return value); a failure shows the error dialog when asked
void pincodeCreated(uint32_t, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        const uint32_t r = call(g::SM_closeCurrentPopup, {sceneManager(), 1});
        Frame f;
        const uint32_t fo = newEmptyFunction(f);
        call(0x5A1FF1, {r, fo});                                     // openIssueUserCodePopup
        functionDestroy(fo);
        return;
    }
    if (!ld8(call(g::APIManager_getInstance, {}) + 0x14)) return;
    const uint32_t api = call(g::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, 0xF5B824, {});
    call(g::APIManager_openErrorDialog, {api, fo, 1});
    functionDestroy(fo);
}
// sub_5A5728, Input {self}: the top popup closes and the code input popup opens
void inputTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t self = ld32(functor + 4);
    call(g::SM_closeCurrentPopup, {sceneManager(), 1});
    Frame f;
    const uint32_t fo = newEmptyFunction(f);
    call(0x5A2571, {self, fo});                                      // openInputUserCodePopup
    functionDestroy(fo);
}
// sub_5A5228, the top popup's result: the decide or cancel sound, then it closes
void userCodeTopResult(uint32_t functor, uint32_t okRef) {
    call(ld8(okRef) ? g::AudioPlaySystemSEDecide : g::AudioPlaySystemSECancel, {});
    closeWithAnimation(functor + 8, 0xF5B71C, false);
}
// sub_5A4BA8, the issued code popup's result: closed by the player
void issueResult(uint32_t functor, uint32_t okRef) {
    if (ld8(okRef)) return;
    call(g::AudioPlaySystemSECancel, {});
    closeWithAnimation(functor + 8, 0xF5B640, false);
}
// sub_5A41D0: the code input popup closes ({widget, layer, shadow, done} at captures)
void inputClose(uint32_t captures) { closeWithAnimation(captures, 0xF5B4E0, false); }
// sub_5A40DC, the code input popup's result {.., nodes, done, self}: accepted - it closes and the success popup
// opens; closed by the player - the cancel sound and it closes; either way the dialog is busy again
void inputResult(uint32_t functor, uint32_t okRef) {
    const uint32_t self = ld32(functor + 0x30);
    if (ld8(okRef)) {
        inputClose(functor + 8);
        Frame f;
        const uint32_t fo = newEmptyFunction(f);
        call(0x5A2ED9, {0, fo});                                     // openInputSuccessUserCodePopup
        functionDestroy(fo);
        st8(self + F_BUSY, 1);
        return;
    }
    call(g::AudioPlaySystemSECancel, {});
    st8(self + F_BUSY, 1);
    inputClose(functor + 8);
}
// sub_5A45F4, Certification {self, edit box}: the typed code is sent (pincode update, $_ sub_5A4770)
void certificationTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t self = ld32(functor + 4);
    call(g::AudioPlaySystemSEDecide, {});
    st8(self + F_BUSY, 1);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t box = ld32(functor + 8);
    const uint32_t code = vcall(box, 0x2AC, {box});
    Frame f;
    const uint32_t fo = newFunction(f, 0xF5B564, {self});
    call(g::APIManager_executeCampaignPincodeUpdate, {api, strData(code), fo});
    functionDestroy(fo);
}
// sub_5A4770, the pincode update callback {self}: success closes the input popup; a failure shows the error
// dialog when asked and, unless the request will be retried, frees the dialog
void pincodeUpdated(uint32_t functor, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    const uint32_t self = ld32(functor + 4);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        call(g::SM_closeCurrentPopup, {sceneManager(), 1});
        return;
    }
    if (ld8(call(g::APIManager_getInstance, {}) + 0x14)) {
        const uint32_t api = call(g::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, 0xF5B590, {});
        call(g::APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }
    if (!call(g::APIManager_needsRetry, {call(g::APIManager_getInstance, {})})) st8(self + F_BUSY, 0);
}
// sub_5A3B88, the success popup's OK: the decide sound, then it closes
void inputSuccessOk(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    call(g::AudioPlaySystemSEDecide, {});
    closeWithAnimation(functor + 8, 0xF5B45C, false);
}

// ================================================================================================ serial buttons
constexpr uint32_t F_SEND = 0x280;
// sub_5A6BC8, the serial popup's List {self, dialog, list, close}: the buttons off and the list popup opened
// ($_ sub_5A6D72 turns them back on when it closes)
void serialListTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t self = ld32(functor + 4);
    call(g::AudioPlaySystemSEDecide, {});
    st8(self + F_BUSY, 1);
    const uint32_t send = ld32(ld32(functor + 8) + F_SEND);
    vcall(send, V_setTouchEnabled, {send, 0});
    vcall(ld32(functor + 0xC), V_setTouchEnabled, {ld32(functor + 0xC), 0});
    vcall(ld32(functor + 0x10), V_setTouchEnabled, {ld32(functor + 0x10), 0});
    const uint32_t dialog = ld32(functor + 8), list = ld32(functor + 0xC), close = ld32(functor + 0x10);
    Frame f;
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x14});
    if (obj) {
        st32(obj, 0xF5BA8C);
        st32(obj + 4, self);
        st32(obj + 8, dialog);
        st32(obj + 0xC, list);
        st32(obj + 0x10, close);
    }
    st32(fo + 16, obj);
    call(0x5A1249, {obj, fo});                                       // openSerialCodeListPopup
    functionDestroy(fo);
}
// sub_5A6D72: the serial popup's buttons back on
void serialListClosed(uint32_t functor) {
    st8(ld32(functor + 4) + F_BUSY, 0);
    const uint32_t send = ld32(ld32(functor + 8) + F_SEND);
    vcall(send, V_setTouchEnabled, {send, 1});
    vcall(ld32(functor + 0xC), V_setTouchEnabled, {ld32(functor + 0xC), 1});
    vcall(ld32(functor + 0x10), V_setTouchEnabled, {ld32(functor + 0x10), 1});
}
// sub_5A6E54, Send {edit box, dialog, list, close, self}: a typed code that is empty or passes splitSpaceCheck
// is sent (the buttons off, the serial code request $_ {self, dialog, list, close}); "" or a failed check shows
// the invalid code dialog
void serialSendTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t owner = ld32(functor + 0x14);
    call(g::AudioPlaySystemSEDecide, {});
    const uint32_t box = ld32(functor + 4);
    const uint32_t code = vcall(box, 0x2AC, {box});
    bool send = false;
    if (call(g::String_notEqual, {code, S_Empty})) send = strSize(code) == 0 || call(g::SceneUtil_splitSpaceCheck, {code}) != 0;
    if (!send) {
        Frame f;
        const uint32_t text = f.alloc(8);
        call(g::TextData_get, {text, 0, 104000002});
        const uint32_t msg = newStr(f, ld32(ld32(text) + 4));
        const uint32_t fo = newEmptyFunction(f);
        call(g::SceneUtil_openOkDialog4Line, {msg, fo, 0});
        functionDestroy(fo);
        strFree(msg);
        releaseIf(ld32(text + 4));
        return;
    }
    const uint32_t sendButton = ld32(ld32(functor + 8) + F_SEND);
    vcall(sendButton, V_setTouchEnabled, {sendButton, 0});
    vcall(ld32(functor + 0xC), V_setTouchEnabled, {ld32(functor + 0xC), 0});
    vcall(ld32(functor + 0x10), V_setTouchEnabled, {ld32(functor + 0x10), 0});
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t cstr = strData(code);
    const uint32_t dialog = ld32(functor + 8), list = ld32(functor + 0xC), close = ld32(functor + 0x10);
    Frame f;
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x14});
    if (obj) {
        st32(obj, 0xF5BAE4);
        st32(obj + 4, owner);
        st32(obj + 8, dialog);
        st32(obj + 0xC, list);
        st32(obj + 0x10, close);
    }
    st32(fo + 16, obj);
    call(g::APIManager_executeSystemSerialcode, {api, cstr, fo});
    functionDestroy(fo);
}

// ================================================================================================ movie, Facebook
// sub_5A3680, the movie popup's start {player}: the main BGM pushed away and tutorial_movie.mp4 played
void moviePlay(uint32_t functor) {
    call(g::AudioPushGameMainBGM, {0, 500});
    const uint32_t player = ld32(functor + 4);
    const uint32_t setFile = ld32(ld32(player) + 0x31C);
    Frame f;
    const uint32_t name = f.alloc(12);
    const uint32_t buf = call(fn::operator_new, {0x20});
    st32(name + 8, buf);
    st32(name, 0x21);
    st32(name + 4, 0x12);
    call(g::MEMCPY, {buf, 0xE007CF, 0x12});                          // "tutorial_movie.mp4"
    st8(buf + 0x12, 0);
    call(setFile, {player, name});
    strFree(name);
    const uint32_t p = ld32(functor + 4);
    vcall(p, 0x32C, {p});                                            // play
}

// The tutorial movie on popup layer 2: a 960 x 540 VideoPlayer centred over a shadow, touch-closable ($_ {player,
// shadow}), its event listener ($_ {player, shadow, player}), started half a second later (sub_5A3680)
void openMoviePopup(uint32_t) {
    call(g::SM_setEnabledTouchEffect, {sceneManager(), 0});
    const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {ld32(ld32(g::GOT_POPUP_FADE_TIME)), opacity})});
    vcall(layer, 0xF4, {layer, shadow, 0});
    const uint32_t player = call(g::VideoPlayer_create, {});
    Frame f;
    const uint32_t visible = f.alloc(8);
    call(g::Director_getVisibleSize, {visible, call(fn::Director_getInstance, {})});
    {
        Frame t;
        const uint32_t size = t.alloc(8);
        call(g::Size_ctor, {size, fbits(960.0f), fbits(540.0f)});
        vcall(player, V_setContentSize, {player, size});
    }
    {
        const float x = ldf(visible) * 0.5f, y = ldf(visible + 4) * 0.5f;
        const uint32_t setPos = ld32(ld32(player) + node::setPosition);
        Frame t;
        const uint32_t pos = t.alloc(8);
        call(g::Vec2_ctorXY, {pos, fbits(x), fbits(y)});
        call(setPos, {player, pos});
        call(g::Vec2_dtor, {pos});
    }
    vcall(player, V_setTouchEnabled, {player, 0});
    addTouch(player, 0xF5B3D8, {player, shadow});
    {
        const uint32_t addListener = ld32(ld32(player) + 0x358);
        Frame t;
        const uint32_t fo = newFunction(t, 0xF5B3AC, {player, shadow, player});
        call(addListener, {player, fo});
        functionDestroy(fo);
    }
    vcall(layer, 0xF4, {layer, player, 1});
    {
        const uint32_t delay = call(g::DelayTime_create, {fbits(0.5f)});
        Frame t;
        const uint32_t fo = newFunction(t, 0xF5B380, {player});
        const uint32_t done = call(g::CallFunc_create, {fo});
        call(g::Node_runAction, {layer, call(g::Sequence_create, {delay, done, 0})});
        functionDestroy(fo);
    }
}

// Facebook: logged in - confirm the logout (OK / Cancel, $_ {done, self}); logged out - log in ($_ {done, self})
void openFacebookLoginLogout(uint32_t self, uint32_t done) {
    Frame f;
    if (call(g::SceneUtil_isLoggedInFacebook, {}) == 1) {
        const uint32_t doneCopy = f.alloc(20);
        call(g::FunctionVB_copy, {doneCopy, done});
        const uint32_t title = f.alloc(8);
        call(g::TextData_get, {title, 0, 104900007});
        const uint32_t msg = newStr(f, ld32(ld32(title) + 4));
        const uint32_t tmp = f.alloc(20);
        functionCopy(tmp, doneCopy);
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x28});
        if (obj) {
            st32(obj, 0xF5BDD0);
            call(g::FunctionVB_move, {obj + 8, tmp});
            st32(obj + 0x20, self);
        }
        st32(fo + 16, obj);
        const uint32_t body = f.alloc(8);
        call(g::TextData_get, {body, 0, 104900008});
        call(g::SceneUtil_openOkCancelDialog8Line, {msg, fo, ld32(ld32(body) + 4), 0});
        releaseIf(ld32(body + 4));
        functionDestroy(fo);
        functionDestroy(tmp);
        strFree(msg);
        releaseIf(ld32(title + 4));
        functionDestroy(doneCopy);
        return;
    }
    const uint32_t doneCopy = f.alloc(20);
    call(g::FunctionVB_copy, {doneCopy, done});
    const uint32_t manager = call(g::FacebookManager_getInstance, {});
    const uint32_t tmp = f.alloc(20);
    functionCopy(tmp, doneCopy);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, 0xF5BCC8);
        call(g::FunctionVB_move, {obj + 8, tmp});
        st32(obj + 0x20, self);
    }
    st32(fo + 16, obj);
    call(g::FacebookManager_login, {manager, 0, fo});
    functionDestroy(fo);
    functionDestroy(tmp);
    functionDestroy(doneCopy);
}

// sub_59F970: a button's "Txt_FaceBook" label: shadow and a text-table text
void facebookLabel(uint32_t button, int32_t id) {
    const uint32_t label = findNode(button, 0xE00808, 12);
    SceneUtil::setShadow(label);
    Frame f;
    const uint32_t text = f.alloc(8);
    call(g::TextData_get, {text, 0, uint32_t(id)});
    const uint32_t s = newStr(f, ld32(ld32(text) + 4));
    call(fn::Text_setString, {label, s});
    strFree(s);
    releaseIf(ld32(text + 4));
}

// the Facebook button's label: log out when logged in, else log in
void updateFacebookButton(uint32_t self) {
    const uint32_t button = findNode(ld32(self + 0x274), 0xE005A0, 14);   // "FaceBookButton"
    if (!button) return;
    facebookLabel(button, call(g::SceneUtil_isLoggedInFacebook, {}) == 1 ? 104900004 : 104900003);
}

// the user code popup after a pincode status update ($_ {self})
void priOpenUserCodePopup(uint32_t self) {
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(g::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, 0xF5BEAC, {self});
    call(g::APIManager_executeCampaignPincodeStatusUpdate, {api, fo});
    functionDestroy(fo);
}
}  // namespace

RW_FUNCTION(0x5A18D8, "OtherMenuDialog::openUserCodePopup(function<void()>)", openUserCodePopup);
RW_FUNCTION(0x5A1FF0, "OtherMenuDialog::openIssueUserCodePopup(function<void()>)", openIssueUserCodePopup);
RW_FUNCTION(0x5A2570, "OtherMenuDialog::openInputUserCodePopup(function<void()>)", openInputUserCodePopup);
RW_FUNCTION(0x5A2ED8, "OtherMenuDialog::openInputSuccessUserCodePopup(function<void()>)", openInputSuccessUserCodePopup);
RW_FUNCTION(0x5A32FC, "OtherMenuDialog::openMoviePopup()", openMoviePopup);
RW_FUNCTION(0x59FA78, "OtherMenuDialog::openFacebookLoginLogout(function<void(bool)>)", openFacebookLoginLogout);
RW_FUNCTION(0x59F818, "OtherMenuDialog::updateFacebookButton()", updateFacebookButton);
RW_FUNCTION(0x59F8CC, "OtherMenuDialog::priOpenUserCodePopup()", priOpenUserCodePopup);
RW_FUNCTION(0x59F970, "OtherMenuDialog Facebook button label (sub_59F970)", facebookLabel);
RW_FUNCTION(0x5A5858, "OtherMenuDialog user code Issue button (sub_5A5858)", issueTouched);
RW_FUNCTION(0x5A59A0, "OtherMenuDialog pincode create callback (sub_5A59A0)", pincodeCreated);
RW_FUNCTION(0x5A5728, "OtherMenuDialog user code Input button (sub_5A5728)", inputTouched);
RW_FUNCTION(0x5A5228, "OtherMenuDialog user code top result (sub_5A5228)", userCodeTopResult);
RW_FUNCTION(0x5A4BA8, "OtherMenuDialog issued code result (sub_5A4BA8)", issueResult);
RW_FUNCTION(0x5A41D0, "OtherMenuDialog code input close (sub_5A41D0)", inputClose);
RW_FUNCTION(0x5A40DC, "OtherMenuDialog code input result (sub_5A40DC)", inputResult);
RW_FUNCTION(0x5A45F4, "OtherMenuDialog Certification button (sub_5A45F4)", certificationTouched);
RW_FUNCTION(0x5A4770, "OtherMenuDialog pincode update callback (sub_5A4770)", pincodeUpdated);
RW_FUNCTION(0x5A3B88, "OtherMenuDialog code accepted OK (sub_5A3B88)", inputSuccessOk);
RW_FUNCTION(0x5A6BC8, "OtherMenuDialog serial popup List button (sub_5A6BC8)", serialListTouched);
RW_FUNCTION(0x5A6D72, "OtherMenuDialog serial list closed (sub_5A6D72)", serialListClosed);
RW_FUNCTION(0x5A6E54, "OtherMenuDialog serial popup Send button (sub_5A6E54)", serialSendTouched);
RW_FUNCTION(0x5A3680, "OtherMenuDialog movie start (sub_5A3680)", moviePlay);
