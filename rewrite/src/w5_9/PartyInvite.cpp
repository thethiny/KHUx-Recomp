// Party > Invitation Code: ScenePartyTop::openPopupPartyInvite asks for the user's invite code; its callback
// (sub_655F84) opens PopupNormal_Invitation_Code (the code, the reward wording, Copy and Close) on a fading shadow;
// the popup's result / close-animation lambdas (sub_656AF6, sub_656B44, sub_656C82) and the copy button lambda's
// clone (sub_656E7C).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;               // FadeTo::create(float duration, uint8 opacity)
constexpr uint32_t Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t Misc_create = 0x786D5D;                 // master::Misc::create(shared_ptr out, id)
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SM_getPopupLayer = 0x613E69, SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SM_openPopup = 0x61524D;                // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;        // (api, const function<void(bool)>&, bool)
constexpr uint32_t APIManager_executeSystemInvitecode = 0x4D206D;
constexpr uint32_t GOT_POPUP_FADE_TIME = 0xFD65C0;         // &float (0.15 s)
}  // namespace g

constexpr uint32_t VT_InviteCode = 0xF61EF4;   // openPopupPartyInvite {}: sub_655F84
constexpr uint32_t VT_InviteError = 0xF61FFC;  // error dialog {}
constexpr uint32_t VT_CopyButton = 0xF61FD0;   // {string code, shadow, widget, layer} (heap 0x1C)
constexpr uint32_t VT_CloseButton = 0xF61FA4;  // {}
constexpr uint32_t VT_PopupCheck = 0xF61F78;   // function<bool()> {close button}
constexpr uint32_t VT_PopupResult = 0xF61F20;  // function<void(bool)> {shadow, widget, layer}: sub_656AF6
constexpr uint32_t VT_Closed = 0xF61F4C;       // {layer, shadow, widget}: sub_656C82
constexpr uint32_t S_PopupJson = 0xE03CEF;     // "cocostudio/publish/PopupNormal_Invitation_Code.json"
constexpr uint32_t S_TxtWording1 = 0xDFF14C, S_TxtWording2 = 0xDF863B, S_TxtWording3 = 0xE0014B;
constexpr uint32_t S_TxtWording4 = 0xE00158, S_TxtCommentLabel = 0xDFE1CC;
constexpr uint32_t S_ButtonCopy = 0xE03D23, S_CloseButton = 0xDF8D2F;
constexpr int32_t TXT_WORDING1 = 104010001, TXT_WORDING2 = 104010003, TXT_WORDING3 = 104010004;
constexpr int32_t TXT_WORDING4 = 104010007, TXT_COPY = 104010005;
constexpr uint32_t MISC_INVITE_REWARD = 115;
constexpr uint32_t API_INVITE = 0x27C;         // shared_ptr: {string code, +0xC, +0x10}
constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214, V_removeChild = 0x12C;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
float fadeTime() { return ldf(ld32(g::GOT_POPUP_FADE_TIME)); }
void setTextFromId(uint32_t text, int32_t id) {
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t s = newStr(f, textDataStr(td));
    call(g::Text_setString, {text, s});
    strFree(s);
    sharedRelease(td);
}
// "Txt_Copy" (no such literal in the binary: the original builds the short string in place)
uint32_t findTxtCopy(uint32_t root) {
    Frame f;
    const uint32_t s = f.alloc(12);
    static const char name[] = "Txt_Copy";
    st8(s, uint8_t(2 * (sizeof name - 1)));
    for (uint32_t k = 0; k < sizeof name; k++) st8(s + 1 + k, uint8_t(name[k]));
    return SceneUtil::searchNodeByName(root, s);
}
// dst = a copy of the std::string at src
void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) {
        strInit(dst, ld32(src + 8), ld32(src + 4));
    } else {
        st32(dst, ld32(src));
        st32(dst + 4, ld32(src + 4));
        st32(dst + 8, ld32(src + 8));
    }
}
}  // namespace

struct PartyInvite {
    static void openPopupPartyInvite(uint32_t self);
    static void inviteCodeDone(uint32_t functor, uint32_t okPtr);
    static void popupResult(uint32_t functor, uint32_t okPtr);
    static void closePopup(uint32_t closure);
    static void removePopup(uint32_t functor);
    static uint32_t cloneCopyButton(uint32_t functor);
};

void PartyInvite::openPopupPartyInvite(uint32_t) {
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_InviteCode, {});
    call(g::APIManager_executeSystemInvitecode, {api, fo});
    functionDestroy(fo);
}

// The invite code arrived: the popup; on failure the error dialog.
void PartyInvite::inviteCodeDone(uint32_t, uint32_t okPtr) {
    const uint32_t ok = ld8(okPtr);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    const uint32_t a = call(fn::APIManager_getInstance, {});
    Frame f;
    if (!ok) {
        if (!ld8(a + 0x14)) return;
        const uint32_t a2 = call(fn::APIManager_getInstance, {});
        const uint32_t fo = newFunction(f, VT_InviteError, {});
        call(g::APIManager_openErrorDialog, {a2, fo, 1});
        functionDestroy(fo);
        return;
    }
    // the invite data (each read holds the shared_ptr for a moment)
    uint32_t invite, number, count;
    {
        const uint32_t ctl = ld32(a + API_INVITE + 4);
        invite = ld32(a + API_INVITE);
        if (ctl) {
            addRefIf(ctl);
            releaseIf(ctl);
        }
    }
    {
        const uint32_t a2 = call(fn::APIManager_getInstance, {});
        const uint32_t ctl = ld32(a2 + API_INVITE + 4), p = ld32(a2 + API_INVITE);
        addRefIf(ctl);
        number = ld32(p + 0xC);
        releaseIf(ctl);
    }
    {
        const uint32_t a3 = call(fn::APIManager_getInstance, {});
        const uint32_t ctl = ld32(a3 + API_INVITE + 4), p = ld32(a3 + API_INVITE);
        addRefIf(ctl);
        count = ld32(p + 0x10);
        releaseIf(ctl);
    }
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow}) & 0xFF;
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(fadeTime()), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_PopupJson});
    vcall(shadow, node::addChild, {shadow, widget});
    {
        const uint32_t anchor = f.alloc(8);
        call(g::Vec2_ctorXY, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(widget, node::setAnchorPoint, {widget, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
    {
        const uint32_t text = findNode(widget, S_TxtWording1, 0xC);
        Frame t;
        const uint32_t td = textData(t, TXT_WORDING1);
        const uint32_t fmt = textDataStr(td);
        const uint32_t row = t.alloc(8);
        call(g::Misc_create, {row, MISC_INVITE_REWARD});
        const uint32_t v = t.alloc(4);
        const uint32_t sb = ld32(row);
        rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + 4, 4, 4});
        const uint32_t buf = stringBuffer(t, {fmt, ld32(v)});
        setTextC(text, buf);
        releaseIf(ld32(row + 4));
        releaseIf(ld32(td + 4));
        SceneUtil::setShadow(text);
    }
    {
        const uint32_t text = findNode(widget, S_TxtWording2, 0xC);
        setTextFromId(text, TXT_WORDING2);
        SceneUtil::setShadow(text);
    }
    {
        const uint32_t text = findNode(widget, S_TxtWording3, 0xC);
        Frame t;
        const uint32_t td = textData(t, TXT_WORDING3);
        const uint32_t buf = stringBuffer(t, {textDataStr(td), number, number + count});
        setTextC(text, buf);
        sharedRelease(td);
        SceneUtil::setShadow(text);
    }
    {
        const uint32_t text = findNode(widget, S_TxtWording4, 0xC);
        setTextFromId(text, TXT_WORDING4);
        SceneUtil::setShadow(text);
    }
    {
        const uint32_t label = findNode(widget, S_TxtCommentLabel, 0x11);
        setTextC(label, strData(invite));
        SceneUtil::setShadow(label);
    }
    {
        const uint32_t text = findTxtCopy(widget);
        setTextFromId(text, TXT_COPY);
        SceneUtil::setShadow(text);
    }
    {
        const uint32_t button = findNode(widget, S_ButtonCopy, 0xB);
        Frame t;
        const uint32_t code = t.alloc(12);
        copyString(code, invite);
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x1C});
        if (obj) {
            st32(obj, VT_CopyButton);
            copyString(obj + 4, code);
            st32(obj + 0x10, shadow);
            st32(obj + 0x14, widget);
            st32(obj + 0x18, layer);
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {button, fo});
        functionDestroy(fo);
        strFree(code);
    }
    const uint32_t close = findNode(widget, S_CloseButton, 0xC);
    addTouch(close, VT_CloseButton, {});
    {
        const uint32_t m = sceneManager();
        Frame t;
        const uint32_t check = newFunction(t, VT_PopupCheck, {close});
        const uint32_t result = newFunction(t, VT_PopupResult, {shadow, widget, layer});
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(check);
    }
    {
        Frame t;
        const uint32_t none = newEmptyFunction(t);
        const uint32_t action = call(g::SceneUtil_createOpenPopupAnimation, {widget, none});
        call(g::Node_runAction, {widget, call(g::EaseSineOut_create, {action})});
        functionDestroy(none);
    }
}

// the popup's result (false: cancelled): cancel sound, then close
void PartyInvite::popupResult(uint32_t functor, uint32_t okPtr) {
    if (ld8(okPtr)) return;
    call(g::AudioPlaySystemSECancel, {});
    closePopup(functor + 4);
}

// sub_656B44 (closure {shadow, widget, layer}): the shadow fades, the popup's close animation removes both
void PartyInvite::closePopup(uint32_t closure) {
    const uint32_t shadow = ld32(closure);
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(fadeTime()), 0})});
    const uint32_t widget = ld32(closure + 4);
    Frame f;
    const uint32_t fo = newFunction(f, VT_Closed, {ld32(closure + 8), ld32(closure), widget});
    const uint32_t action = call(g::SceneUtil_createClosePopupAnimation, {widget, fo});
    call(g::Node_runAction, {widget, call(g::EaseSineOut_create, {action})});
    functionDestroy(fo);
}

// the close animation's end {layer, shadow, widget}: both removed from the layer
void PartyInvite::removePopup(uint32_t functor) {
    const uint32_t layer = ld32(functor + 4);
    vcall(layer, V_removeChild, {layer, ld32(functor + 8), 1});
    vcall(ld32(functor + 4), V_removeChild, {ld32(functor + 4), ld32(functor + 12), 1});
}

// __func::__clone() of the copy button lambda {string, shadow, widget, layer}
uint32_t PartyInvite::cloneCopyButton(uint32_t functor) {
    const uint32_t obj = call(fn::operator_new, {0x1C});
    if (obj) {
        st32(obj, VT_CopyButton);
        copyString(obj + 4, functor + 4);
        st32(obj + 0x10, ld32(functor + 0x10));
        st32(obj + 0x14, ld32(functor + 0x14));
        st32(obj + 0x18, ld32(functor + 0x18));
    }
    return obj;
}

RW_FUNCTION(0x655C40, "ScenePartyTop::openPopupPartyInvite", PartyInvite::openPopupPartyInvite);
RW_FUNCTION(0x655F84, "ScenePartyTop::openPopupPartyInvite callback __func::operator() (sub_655F84)", PartyInvite::inviteCodeDone);
RW_FUNCTION(0x656AF6, "ScenePartyTop invite popup result __func::operator()(bool) (sub_656AF6)", PartyInvite::popupResult);
RW_FUNCTION(0x656B44, "ScenePartyTop invite popup close (sub_656B44)", PartyInvite::closePopup);
RW_FUNCTION(0x656C82, "ScenePartyTop invite popup closed __func::operator() (sub_656C82)", PartyInvite::removePopup);
RW_FUNCTION(0x656E7C, "ScenePartyTop invite copy button lambda __func::__clone() (sub_656E7C)", PartyInvite::cloneCopyButton);
