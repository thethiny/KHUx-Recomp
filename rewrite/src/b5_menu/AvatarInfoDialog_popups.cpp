// AvatarInfoDialog popups (party member info): the OK / Cancel confirmations (expel, cancel, invitation, reject)
// and their request lambdas, the party offer / join request popups (comment field) and the sub name choice;
// batch #8 adds the popups' lambdas (answers, OK / Cancel / result / close, requests, __func thunks).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t FunctionVB_copy = 0x45C84D, FunctionVB_move = 0x47BC69;
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;   // (msg, function<void(bool)>, int, int)
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;
constexpr uint32_t SM_getInstance = 0x60C26D, SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_executePartyApplicantReject = 0x4CFA9D;   // (api, u64 user, function)
constexpr uint32_t APIManager_executePartyInvitationCancel = 0x4D0591;
constexpr uint32_t shared_release = 0xC8DBD1;
}  // namespace g

constexpr uint32_t F_USER = 0x2C0;   // the shown user's id (u64)

void releaseIf(uint32_t c) {
    if (c) call(g::shared_release, {c});
}

// the OK / Cancel confirmation of `textId`; the answer goes to $_ {self, done} (heap, vtable `vt`)
void confirm(uint32_t self, uint32_t done, int32_t textId, uint32_t vt) {
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t a = f.alloc(20);
    call(g::FunctionVB_copy, {a, done});
    const uint32_t text = f.alloc(8);
    call(g::TextData_get, {text, 0, uint32_t(textId)});
    const uint32_t msg = newStr(f, ld32(ld32(text) + 4));
    const uint32_t b = f.alloc(20);
    functionCopy(b, a);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, vt);
        st32(obj + 8, self);
        call(g::FunctionVB_move, {obj + 0x10, b});
        st32(obj + 8, self);
    }
    st32(fo + 16, obj);
    call(g::SceneUtil_openOkCancelDialog4Line, {msg, fo, 0, 0});
    functionDestroy(fo);
    functionDestroy(b);
    strFree(msg);
    releaseIf(ld32(text + 4));
    functionDestroy(a);
}
void openPopUpCancel(uint32_t self, uint32_t done) { confirm(self, done, 103850002, 0xF575A0); }
void openPopUpInvitation(uint32_t self, uint32_t done) { confirm(self, done, 103850004, 0xF5751C); }
void openPopUpReject(uint32_t self, uint32_t done) { confirm(self, done, 103850009, 0xF57498); }

// expel: the same confirmation, its lambda {self, done, done} (heap 0x40)
void openPopUpMemberExpel(uint32_t self, uint32_t done) {
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t a = f.alloc(20);
    call(g::FunctionVB_copy, {a, done});
    const uint32_t text = f.alloc(8);
    call(g::TextData_get, {text, 0, 103840005});
    const uint32_t msg = newStr(f, ld32(ld32(text) + 4));
    const uint32_t b = f.alloc(20);
    functionCopy(b, a);
    const uint32_t c = f.alloc(20);
    call(g::FunctionVB_copy, {c, done});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x40});
    if (obj) {
        st32(obj, 0xF57AF4);
        st32(obj + 8, self);
        call(g::FunctionVB_move, {obj + 0x10, b});
        st32(obj + 8, self);
        call(g::FunctionVB_move, {obj + 0x28, c});
        st32(obj + 8, self);
    }
    st32(fo + 16, obj);
    call(g::SceneUtil_openOkCancelDialog4Line, {msg, fo, 0, 0});
    functionDestroy(fo);
    functionDestroy(c);
    functionDestroy(b);
    strFree(msg);
    releaseIf(ld32(text + 4));
    functionDestroy(a);
}

// sub_53CA44 / sub_53D088, the reject / invitation answer {self, done}: on OK the request for the shown user
// ($_ {done}, heap vtable `vt`)
void sendForUser(uint32_t functor, uint32_t okRef, uint32_t request, uint32_t vt) {
    if (!ld8(okRef)) return;
    const uint32_t self = ld32(functor + 8);
    call(g::SM_enableLoadingIndicator, {call(g::SM_getInstance, {}), 1});
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t hi = ld32(self + F_USER + 4), lo = ld32(self + F_USER);
    Frame f;
    const uint32_t a = f.alloc(20);
    call(g::FunctionVB_copy, {a, functor + 0x10});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x20});
    if (obj) {
        st32(obj, vt);
        call(g::FunctionVB_move, {obj + 8, a});
    }
    st32(fo + 16, obj);
    call(request, {api, 0, lo, hi, fo});
    functionDestroy(fo);
    functionDestroy(a);
}
void rejectAnswered(uint32_t functor, uint32_t okRef) {
    sendForUser(functor, okRef, g::APIManager_executePartyApplicantReject, 0xF574C4);
}
void invitationAnswered(uint32_t functor, uint32_t okRef) {
    sendForUser(functor, okRef, g::APIManager_executePartyInvitationCancel, 0xF57548);
}

// sub_53CCF8 / sub_53D33C, the request's callback {done}: the "done" message (OK calls done) or the error dialog
void requestDone(uint32_t functor, uint32_t okRef, int32_t textId, uint32_t vtError) {
    const uint8_t ok = ld8(okRef);
    call(g::SM_enableLoadingIndicator, {call(g::SM_getInstance, {}), 0});
    if (ok) {
        Frame f;
        const uint32_t text = f.alloc(8);
        call(g::TextData_get, {text, 0, uint32_t(textId)});
        const uint32_t msg = newStr(f, ld32(ld32(text) + 4));
        const uint32_t c = f.alloc(20);
        call(g::FunctionVB_copy, {c, functor + 8});
        call(g::SceneUtil_openOkDialog4Line, {msg, c, 0});
        functionDestroy(c);
        strFree(msg);
        releaseIf(ld32(text + 4));
        return;
    }
    if (!ld8(call(g::APIManager_getInstance, {}) + 0x14)) return;
    const uint32_t api = call(g::APIManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, vtError, {});
    call(g::APIManager_openErrorDialog, {api, fo, 1});
    functionDestroy(fo);
}
void rejectDone(uint32_t functor, uint32_t okRef) { requestDone(functor, okRef, 103850010, 0xF574F0); }
void invitationDone(uint32_t functor, uint32_t okRef) { requestDone(functor, okRef, 103850008, 0xF57574); }
// AvatarInfoDialog::openPopUpSelectSubName(self, mode, ?, done): the title (sub name) choice: AvatarTitle_Select
// placed by the side margins, its heading by mode, the list of the player's titles (0 = none, then the user's
// title rows) in an AvatarTitleScrollView whose choice goes to $_ {widget, shadow, layer, done, ids}; Close
// ($_ {}); the result lambda {close, view, widget, shadow, layer}
void openPopUpSelectSubName(uint32_t, uint32_t mode, uint32_t, uint32_t done) {
    constexpr uint32_t SM_getPopupLayer2 = 0x613E71;
    constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_setShadow = 0x45BD15;
    constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
    constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
    constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create, Node_runAction = rwlib::cocos2d::Node::runAction;
    constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
    constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
    constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
    constexpr uint32_t VectorInt_pushBackSlow = 0x47D709, VectorInt_copy = 0x4AF707, VectorInt_dtor = 0x478961;
    constexpr uint32_t SafeBuffer_read = 0x45A269;
    constexpr uint32_t AvatarTitleScrollView_create = 0x510A45, BaseScrollViewInt_setItemIDs = 0x478929;
    constexpr uint32_t FunctionVI_copy = 0x539C99, FunctionVI_copy2 = 0x543045, FunctionVI_swap = 0x5428E1;
    constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener, SM_openPopup = 0x61524D;
    constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;

    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(SM_getPopupLayer2, {call(g::SM_getInstance, {})});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, 0x20C, {shadow});
    vcall(shadow, 0x214, {shadow, 0});
    call(Node_runAction, {shadow, call(FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t widget = call(GUIReader_widgetFromJsonFile, {call(GUIReader_getInstance, {}), 0xDFE5A3});
    vcall(widget, node::setAnchorPoint, {widget, ld32(GOT_ANCHOR_MIDDLE)});
    {
        const uint32_t setPos = ld32(ld32(widget) + node::setPosition);
        const float x0 = bitsf(vcall(widget, 0x68, {widget}));
        const float w = ldf(vcall(widget, 0xAC, {widget}));
        const float margin = bitsf(call(SceneUtil_getSideMargin, {}));
        const float y0 = bitsf(vcall(widget, 0x70, {widget}));
        const float h = ldf(vcall(widget, 0xAC, {widget}) + 4);
        const float x = margin + (x0 + w * 0.5f);
        const float y = y0 + h * 0.5f;
        const float hm = bitsf(call(SceneUtil_getSideHeightMargin, {}));
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(Vec2_ctor, {pos, fbits(x), fbits(hm + y)});
        call(setPos, {widget, pos});
        call(Vec2_dtor, {pos});
    }
    vcall(shadow, node::addChild, {shadow, widget});
    {
        const uint32_t heading = findNode(widget, 0xDFE5CE, 16);         // "Txt_Select_Label"
        call(SceneUtil_setShadow, {heading});
        Frame f;
        const uint32_t text = f.alloc(8);
        call(g::TextData_get, {text, 0, mode ? 102200008u : 102200007u});
        const uint32_t s = newStr(f, ld32(ld32(text) + 4));
        call(Text_setString, {heading, s});
        strFree(s);
        releaseIf(ld32(text + 4));
    }
    Frame f;
    const uint32_t ids = f.alloc(12), item = f.alloc(4), raw = f.alloc(16);
    st32(item, 0);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    call(VectorInt_pushBackSlow, {ids, item});
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + 0x110), titles = ld32(api + 0x10C);
    if (ctrl) call(0xC8DBB5, {ctrl});
    for (uint32_t i = 0, off = 0; i < ld32(titles + 0x20); i++, off += 16) {
        call(SafeBuffer_read, {titles, raw, ld32(titles + 0xC) + off, 16, off});
        st32(item, ld32(raw + 8));
        const uint32_t end = ld32(ids + 4);
        if (end >= ld32(ids + 8)) {
            call(VectorInt_pushBackSlow, {ids, item});
        } else {
            if (end) st32(end, ld32(item));
            st32(ids + 4, (end ? end : 0) + 4);
        }
    }
    const uint32_t area = findNode(widget, 0xDF9B59, 11);               // "Scroll_Area"
    const uint32_t view = call(AvatarTitleScrollView_create, {});
    vcall(view, 0xA8, {view, vcall(area, 0xAC, {area})});
    call(BaseScrollViewInt_setItemIDs, {view, ids});
    {
        // the choice lambda {widget, shadow, layer, done, ids}, built from two closure copies
        Frame t;
        const uint32_t a = t.alloc(20), v1 = t.alloc(12), b = t.alloc(20), v2 = t.alloc(12);
        call(FunctionVI_copy, {a, done});
        call(VectorInt_copy, {v1, ids});
        call(FunctionVI_copy2, {b, a});
        for (uint32_t k = 0; k < 12; k += 4) {
            st32(v2 + k, ld32(v1 + k));
            st32(v1 + k, 0);
        }
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x40});
        if (obj) {
            st32(obj, 0xF57DB4);
            st32(obj + 8, widget);
            st32(obj + 0xC, shadow);
            st32(obj + 0x10, layer);
            call(FunctionVI_copy2, {obj + 0x18, b});
            for (uint32_t k = 0; k < 12; k += 4) {
                st32(obj + 0x30 + k, ld32(v2 + k));
                st32(v2 + k, 0);
            }
        }
        st32(fo + 16, obj);
        call(FunctionVI_swap, {fo, view + 0x638});
        functionDestroy(fo);
        call(VectorInt_dtor, {v2});
        functionDestroy(b);
        call(VectorInt_dtor, {v1});
        functionDestroy(a);
    }
    const uint32_t close = findNode(widget, 0xDF8D2F, 12);              // "Close_Button"
    {
        Frame t;
        const uint32_t fo = newFunction(t, 0xF57D88, {});
        call(Widget_addTouchEventListener, {close, fo});
        functionDestroy(fo);
    }
    {
        const uint32_t m = call(g::SM_getInstance, {});
        Frame t;
        const uint32_t check = newFunction(t, 0xF57D5C, {close});
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x18});
        if (obj) {
            st32(obj, 0xF57D04);
            st32(obj + 4, close);
            st32(obj + 8, view);
            st32(obj + 0xC, widget);
            st32(obj + 0x10, shadow);
            st32(obj + 0x14, layer);
        }
        st32(result + 16, obj);
        rw::call(SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(check);
    }
    vcall(area, node::addChild, {area, view});
    {
        Frame t;
        const uint32_t fo = newEmptyFunction(t);
        call(Node_runAction, {widget, call(SceneUtil_createOpenPopupAnimation, {widget, fo})});
        functionDestroy(fo);
    }
    releaseIf(ctrl);
    call(VectorInt_dtor, {ids});
}
// AvatarInfoDialog::openPopUpOffer / openPopUpJoinRequest(done): the party message popup (party rank > 12: the
// join request layout with a comment field and its length limit; else the general one with a fixed comment kept
// at +0x470): OK ($_ {edit box, party, self}) sends, Cancel ($_ {ok, cancel}); the result lambda {self, shadow,
// widget, layer, done(, self)} closes it. The join request's closure also carries self.
struct PartyPopup {
    int32_t text2Line, textComment, textCommentDefault;
    uint32_t vtSwap, vtOk, vtCancel, vtCheck, vtResult;
    bool join;
};
uint32_t partyMessagePopup(uint32_t self, uint32_t done, const PartyPopup &p) {
    constexpr uint32_t SM_getPopupLayer2 = 0x613E71, SM_openPopup = 0x61524D;
    constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_setShadow = 0x45BD15;
    constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
    constexpr uint32_t SceneUtil_textFieldToUiText = 0x45C465, SceneUtil_textFieldToEditBox = 0x45C0A1;
    constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create, Node_runAction = rwlib::cocos2d::Node::runAction;
    constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
    constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base, Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
    constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString, Text_getString = rwlib::cocos2d::ui::Text::getString, String_assign = 0x498A1D;
    constexpr uint32_t String_ctor = 0x48D811, SafeBuffer_read = 0x45A269, MasterMisc_create = 0x786D5D;
    constexpr uint32_t FunctionVV_swap = 0x4EE86D, Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
    constexpr uint32_t S_CALLIGRAPHY = 0xDF9F1F;

    auto readInt = [](uint32_t sb, uint32_t offset, uint32_t dst) {
        call(0x45A269, {sb, dst, ld32(sb + 0xC) + offset, 4, offset});   // SafeBuffer::read
        return int32_t(ld32(dst));
    };
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(SM_getPopupLayer2, {call(g::SM_getInstance, {})});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, 0x20C, {shadow});
    vcall(shadow, 0x214, {shadow, 0});
    call(Node_runAction, {shadow, call(FadeTo_create, {fbits(0.15f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t partyCtrl = ld32(api + 0x1D8), party = ld32(api + 0x1D4);
    if (partyCtrl) call(0xC8DBB5, {partyCtrl});
    Frame f;
    const uint32_t word = f.alloc(4), path = f.alloc(12);
    st32(path, 0);
    st32(path + 4, 0);
    st32(path + 8, 0);
    if (party && readInt(party, 0, word) > 12) call(String_ctor, {path, 0xDFE611, 0x35});   // ..._JoinRequest.json
    else call(String_ctor, {path, 0xDFE5DF, 0x31});                                       // ..._General.json
    const uint32_t reader = call(GUIReader_getInstance, {});
    const uint32_t widget = call(GUIReader_widgetFromJsonFile, {reader, strData(path)});
    vcall(shadow, node::addChild, {shadow, widget});
    {
        Frame t;
        const uint32_t anchor = t.alloc(8);
        call(Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(widget, node::setAnchorPoint, {widget, anchor});
        call(Vec2_dtor, {anchor});
    }
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
    titleLabel(widget, 0xDFE647, 11, p.text2Line);                      // "2Line_Label"
    titleLabel(widget, 0xDF9EF7, 11, p.textComment);                    // "Txt_Comment"
    if (!party || readInt(party, 0, word) <= 12) {
        const uint32_t label = findNode(widget, 0xDFE1CC, 17);          // "Txt_Comment_Label"
        {
            Frame t;
            const uint32_t text = t.alloc(8);
            call(g::TextData_get, {text, 0, uint32_t(p.textCommentDefault)});
            const uint32_t s = newStr(t, ld32(ld32(text) + 4));
            call(Text_setString, {label, s});
            strFree(s);
            releaseIf(ld32(text + 4));
        }
        call(String_assign, {self + 0x470, call(Text_getString, {label})});
    }
    const uint32_t field = findNode(widget, 0xDF9F0D, 17);              // "TextField_Comment"
    const uint32_t limit = findNode(widget, 0xDFE418, 10);              // "Txt_Limit2"
    if (limit && readInt(party, 0, word) >= 13) {
        call(SceneUtil_setShadow, {limit});
        Frame t;
        const uint32_t text = t.alloc(8), misc = t.alloc(8);
        call(g::TextData_get, {text, 0, 100100037});
        const uint32_t fmt = ld32(ld32(text) + 4);
        call(MasterMisc_create, {misc, 808});
        const uint32_t n = uint32_t(readInt(ld32(misc), 4, word));
        const uint32_t s = newStr(t, stringBuffer(t, {fmt, n}));
        call(Text_setString, {limit, s});
        strFree(s);
        releaseIf(ld32(misc + 4));
        releaseIf(ld32(text + 4));
    }
    uint32_t edit = 0;
    if (field) {
        vcall(field, 0x11C, {field});
        {
            Frame t;
            const uint32_t text = t.alloc(8);
            call(g::TextData_get, {text, 0, uint32_t(p.textCommentDefault)});
            st32(self + 0x450, call(SceneUtil_textFieldToUiText, {field, ld32(ld32(text) + 4), S_CALLIGRAPHY}));
            releaseIf(ld32(text + 4));
        }
        st8(self + 0x4C0, 0);
        Frame t;
        const uint32_t placeholder = t.alloc(12);
        vcall(field, 0x150, {placeholder, field});
        const uint32_t text = t.alloc(8), size = t.alloc(8), misc = t.alloc(8);
        call(g::TextData_get, {text, 0, uint32_t(p.textCommentDefault)});
        call(Size_ctor, {size, fbits(400.0f), fbits(136.0f)});
        call(MasterMisc_create, {misc, 808});
        const uint32_t maxLen = uint32_t(readInt(ld32(misc), 4, word));
        edit = rw::call(SceneUtil_textFieldToEditBox, {self + 0x4B8, self + 0x4BC, field, placeholder,
                                                       ld32(ld32(text) + 4), S_CALLIGRAPHY, size, self + 0x270,
                                                       self + 0x4C0, 0, maxLen});
        releaseIf(ld32(misc + 4));
        releaseIf(ld32(text + 4));
        strFree(placeholder);
        vcall(edit, node::setVisible, {edit, 0});
    }
    // the closure {widget nodes, done(, self)}
    const uint32_t a = f.alloc(20);
    call(g::FunctionVB_copy, {a, done});
    {
        Frame t;
        const uint32_t fo = newFunction(t, p.vtSwap, {self});
        call(FunctionVV_swap, {fo, self + 0x498});
        functionDestroy(fo);
    }
    titleLabel(widget, 0xDF869F, 6, 100100001);                         // "Txt_OK"
    const uint32_t ok = findNode(widget, 0xDF8688, 9);                  // "Button_OK"
    {
        if (partyCtrl) call(0xC8DBB5, {partyCtrl});
        Frame t;
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x14});
        uint32_t left = partyCtrl;
        if (obj) {
            st32(obj, p.vtOk);
            st32(obj + 4, edit);
            st32(obj + 8, party);
            st32(obj + 0xC, partyCtrl);
            st32(obj + 0x10, self);
            left = 0;
        }
        st32(fo + 16, obj);
        call(Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        releaseIf(left);
    }
    titleLabel(widget, 0xDF86A6, 10, 100100002);                        // "Txt_Cancel"
    const uint32_t cancel = findNode(widget, 0xDF8692, 12);             // "Button_Close"
    {
        Frame t;
        const uint32_t fo = newFunction(t, p.vtCancel, {ok, cancel});
        call(Widget_addTouchEventListener, {cancel, fo});
        functionDestroy(fo);
    }
    const uint32_t b = f.alloc(20);
    functionCopy(b, a);
    {
        const uint32_t m = call(g::SM_getInstance, {});
        Frame t;
        const uint32_t check = newFunction(t, p.vtCheck, {cancel});
        const uint32_t c = t.alloc(20);
        functionCopy(c, b);
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t obj = call(fn::operator_new, {p.join ? 0x40u : 0x38u});
        if (obj) {
            st32(obj, p.vtResult);
            st32(obj + 8, self);
            st32(obj + 0x10, shadow);
            st32(obj + 0x14, widget);
            st32(obj + 0x18, layer);
            call(g::FunctionVB_move, {obj + 0x20, c});
            if (p.join) st32(obj + 0x38, self);
            st32(obj + 8, self);
        }
        st32(result + 16, obj);
        rw::call(SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(c);
        functionDestroy(check);
    }
    {
        Frame t;
        const uint32_t fo = newEmptyFunction(t);
        call(Node_runAction, {widget, call(SceneUtil_createOpenPopupAnimation, {widget, fo})});
        functionDestroy(fo);
    }
    functionDestroy(b);
    functionDestroy(a);
    strFree(path);
    releaseIf(partyCtrl);
    return p.join ? 0x6BCu : 0x6A4u;   // r0 at the return: the frame size (add sp, r0)
}
uint32_t openPopUpOffer(uint32_t self, uint32_t done) {
    static constexpr PartyPopup P{103830013, 103800044, 103830014, 0xF57A18, 0xF5793C, 0xF57910, 0xF578E4, 0xF5788C, false};
    return partyMessagePopup(self, done, P);
}
uint32_t openPopUpJoinRequest(uint32_t self, uint32_t done) {
    static constexpr PartyPopup P{103810002, 103800044, 103810025, 0xF577B0, 0xF576D4, 0xF576A8, 0xF5767C, 0xF57624, true};
    return partyMessagePopup(self, done, P);
}

// ------------------------------------------------------------------------------------------------ batch #8
// The party popups' lambdas: the cancel / expel answers and their requests, the join request / offer popups' OK,
// Cancel, result and close lambdas, their requests and answers, the __func thunks of the two-function lambdas, and
// the sub name choice's result.
constexpr uint32_t APIManager_executePartyMemberExpel = 0x4D0591;      // (api, -, u64 user, function)
constexpr uint32_t APIManager_executePartyApplication = 0x4CF2C9;      // (api, -, u64 party, comment, function)
constexpr uint32_t APIManager_executePartyInviteOffer = 0x4D09A5;      // (api, -, u64 user, comment, function)
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t FunctionVB_call = 0x474139, FunctionVB_assign = 0x490109;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1, SM_openNoTapScreen = 0x61FB25, SM_openPopupAllClear = 0x6217DD;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61, SceneUtil_splitSpaceCheck = 0x47898D;
constexpr uint32_t SceneUtil_openPartyInfoJoinAlertPopup = 0x478E1D;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t AvatarInfoDialog_close = 0x534F51, AvatarTitleScrollView_setEnabledTapItem = 0x511349;
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595;
constexpr uint32_t String_copyAssign = 0x498A1D;
constexpr uint32_t GOT_SHADOW_FADE = 0xFD65C0, GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_PARTY_TOP = 0xFD6620;
constexpr uint32_t F_SENDING = 0x4C0, F_COMMENT = 0x470, F_PARTY_INFO = 0x310;
constexpr uint32_t SM_LOCKED = 0x309, SM_NO_TAP_TIME = 0x300;
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C, V_REMOVE_CHILD = 0x12C, V_EDIT_GET_TEXT = 0x2AC;

uint32_t api() { return call(g::APIManager_getInstance, {}); }
uint32_t sm() { return call(g::SM_getInstance, {}); }

// the cancel confirmation's answer {self, done}: /party/member/expel for the shown user, then "cancelled"
void cancelAnswered(uint32_t functor, uint32_t okRef) {
    sendForUser(functor, okRef, APIManager_executePartyMemberExpel, 0xF575CC);
}
void cancelDone(uint32_t functor, uint32_t okRef) { requestDone(functor, okRef, 103850003, 0xF575F8); }

// the expel confirmation's answer {self, done, cancelled}: Cancel runs `cancelled(false)`; OK: the request
void expelAnswered(uint32_t functor, uint32_t okRef) {
    if (!ld8(okRef)) {
        call(FunctionVB_call, {functor + 0x28, 0});
        return;
    }
    sendForUser(functor, okRef, APIManager_executePartyMemberExpel, 0xF57B20);
}
// its request's callback {done}: "expelled" (OK: done), or done(false) unless retried and the error dialog
void expelDone(uint32_t functor, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    call(g::SM_enableLoadingIndicator, {sm(), 0});
    st8(sm() + SM_LOCKED, 1);
    if (ok) {
        Frame f;
        const uint32_t text = f.alloc(8);
        call(g::TextData_get, {text, 0, 103840006});
        const uint32_t msg = newStr(f, ld32(ld32(text) + 4));
        const uint32_t c = f.alloc(20);
        call(g::FunctionVB_copy, {c, functor + 8});
        call(g::SceneUtil_openOkDialog4Line, {msg, c, 0});
        functionDestroy(c);
        strFree(msg);
        releaseIf(ld32(text + 4));
        return;
    }
    if (!ld8(api() + 0x14)) return;
    if (!call(APIManager_needsRetry, {api()}) && ld32(functor + 0x18)) call(FunctionVB_call, {functor + 8, 0});
    const uint32_t a = api();
    Frame f;
    const uint32_t fo = newFunction(f, 0xF57B4C, {});
    call(g::APIManager_openErrorDialog, {a, fo, 1});
    functionDestroy(fo);
}

// the party message popup's result {self, -, shadow, widget, layer, done(, self)}(bool sent): cancelled: not sending;
// the shadow fades, the eased close animation then the close lambda {layer, shadow, widget, done, sent(, self)}
void popupResult(uint32_t functor, uint32_t okRef, uint32_t vt, bool join) {
    const uint8_t ok = ld8(okRef);
    if (!ok) {
        const uint32_t self = ld32(functor + 8);
        call(AudioPlaySystemSECancel, {});
        st8(self + F_SENDING, 1);
    }
    const uint32_t self2 = join ? ld32(functor + 0x38) : 0;
    const uint32_t shadow = ld32(functor + 0x10);
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {ld32(ld32(GOT_SHADOW_FADE)), 0})});
    const uint32_t widget = ld32(functor + 0x14);
    Frame f;
    const uint32_t done = f.alloc(20);
    call(g::FunctionVB_copy, {done, functor + 0x20});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x38});
    if (obj) {
        st32(obj, vt);
        st32(obj + 8, ld32(functor + 0x18));
        st32(obj + 0xC, ld32(functor + 0x10));
        st32(obj + 0x10, widget);
        call(g::FunctionVB_move, {obj + 0x18, done});
        if (join) {
            st32(obj + 0x30, ok);
            st32(obj + 0x34, self2);
        } else {
            st8(obj + 0x30, ok);
        }
    }
    st32(fo + 16, obj);
    const uint32_t anim = call(SceneUtil_createClosePopupAnimation, {widget, fo});
    call(fn::Node_runAction, {widget, call(EaseSineOut_create, {anim})});
    functionDestroy(fo);
    functionDestroy(done);
}
void joinResult(uint32_t functor, uint32_t okRef) { popupResult(functor, okRef, 0xF57650, true); }
void offerResult(uint32_t functor, uint32_t okRef) { popupResult(functor, okRef, 0xF578B8, false); }

// the join request popup closed {layer, shadow, widget, done, sent, self}: both removed, done(sent); sent and now
// in a party (the party's join type 3): the dialog closes and the party top reloads
void joinClosed(uint32_t functor) {
    const uint32_t self = ld32(functor + 0x34);
    const uint32_t layer = ld32(functor + 8);
    vcall(layer, V_REMOVE_CHILD, {layer, ld32(functor + 0xC), 1});
    vcall(ld32(functor + 8), V_REMOVE_CHILD, {ld32(functor + 8), ld32(functor + 0x10), 1});
    call(FunctionVB_call, {functor + 0x18, ld8(functor + 0x30)});
    if (!ld8(functor + 0x30)) return;
    const uint32_t party = ld32(api() + 0x210);
    if (!party || ld32(party + 0x44)) return;
    const uint32_t info = ld32(api() + 0x1E4);
    if (!info || ld32(info + 0x20) != 3) return;
    call(AvatarInfoDialog_close, {self});
    call(SM_openPopupAllClear, {sm()});
    const uint32_t m = sm();
    st8(m + 0x4CC, 1);
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(FunctionVB_assign, {m + 0x548, none});
        functionDestroy(none);
    }
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(ScenePartyTop_callPartyTopAPI, {none});
        functionDestroy(none);
    }
    const uint32_t m2 = sm();
    Frame f;
    const uint32_t fo = newFunction(f, ld32(GOT_FUNC_LAYER_VT) + 8, {ld32(GOT_CREATE_PARTY_TOP)});
    call(SM_changeSceneLayer, {m2, fo, 1, 1, 1});
    functionDestroy(fo);
}

// Cancel {OK, Cancel}(sender, type): the popup closed, both buttons off
uint32_t cancelTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    if (type != 2) return type;
    call(SM_closeCurrentPopup, {sm(), 0});
    vcall(ld32(functor + 4), V_SET_TOUCH_ENABLED, {ld32(functor + 4), 0});
    return vcall(ld32(functor + 8), V_SET_TOUCH_ENABLED, {ld32(functor + 8), 0});
}

// a delayed lambda {self} on self
void delayedOn(uint32_t self, uint32_t vt) {
    const uint32_t m = sm();
    call(SM_openNoTapScreen, {m, ld32(sm() + SM_NO_TAP_TIME)});
    const uint32_t delay = call(fn::DelayTime_create, {ld32(sm() + SM_NO_TAP_TIME)});
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    const uint32_t then = call(fn::CallFunc_create, {fo});
    call(fn::Node_runAction, {self, call(fn::Sequence_create, {delay, then, 0})});
    functionDestroy(fo);
}
// the user level (party data word 0)
int32_t level(uint32_t party) {
    Frame f;
    const uint32_t v = f.alloc(4);
    call(0x45A269, {party, v, ld32(party + 0xC), 4, 0});
    return int32_t(ld32(v));
}
// OK {edit box, party, party ctrl, self}(sender, type): from level 13 the comment must not be empty (else the
// "empty" lambda) and pass the space check (else the NG message); then sending: the comment kept, the request after
// the popup delay
struct OkLambdas {
    uint32_t vtEmpty, vtSend, vtNg;
};
void okTouched(uint32_t functor, uint32_t typeRef, const OkLambdas &l) {
    if (ld32(typeRef) != 2) return;
    const uint32_t self = ld32(functor + 0x10);
    call(g::AudioPlaySystemSEDecide, {});
    const uint32_t edit = ld32(functor + 4), party = ld32(functor + 8);
    if (edit && party && level(party) >= 13) {
        const uint32_t text = vcall(edit, V_EDIT_GET_TEXT, {edit});
        if (!strSize(text)) {
            delayedOn(self, l.vtEmpty);
            return;
        }
        if (!call(SceneUtil_splitSpaceCheck, {text})) {
            st8(sm() + SM_LOCKED, 1);
            Frame f;
            const uint32_t td = f.alloc(8);
            call(g::TextData_get, {td, 0, 103810020});
            const uint32_t msg = newStr(f, ld32(ld32(td) + 4));
            const uint32_t fo = newFunction(f, l.vtNg, {});
            call(g::SceneUtil_openOkDialog4Line, {msg, fo, 0});
            functionDestroy(fo);
            strFree(msg);
            releaseIf(ld32(td + 4));
            return;
        }
    }
    st8(self + F_SENDING, 1);
    if (const uint32_t p = ld32(functor + 8); p && level(p) >= 13)
        call(String_copyAssign, {self + F_COMMENT, vcall(ld32(functor + 4), V_EDIT_GET_TEXT, {ld32(functor + 4)})});
    delayedOn(self, l.vtSend);
}
void joinOk(uint32_t functor, uint32_t, uint32_t typeRef) { okTouched(functor, typeRef, {0xF57758, 0xF57700, 0xF5772C}); }
void offerOk(uint32_t functor, uint32_t, uint32_t typeRef) { okTouched(functor, typeRef, {0xF579C0, 0xF57968, 0xF57994}); }

// the empty comment's delayed lambda: the NG message
void emptyComment(uint32_t vt) {
    st8(sm() + SM_LOCKED, 1);
    Frame f;
    const uint32_t td = f.alloc(8);
    call(g::TextData_get, {td, 0, 103810020});
    const uint32_t msg = newStr(f, ld32(ld32(td) + 4));
    const uint32_t fo = newFunction(f, vt, {});
    call(g::SceneUtil_openOkDialog4Line, {msg, fo, 0});
    functionDestroy(fo);
    strFree(msg);
    releaseIf(ld32(td + 4));
}
void joinEmpty(uint32_t) { emptyComment(0xF57784); }
void offerEmpty(uint32_t) { emptyComment(0xF579EC); }

// the join request's send {self, ?}: /party/application for the shown party with the comment (the answer lambda
// gets the lambda's second word, never set by its creator)
void joinSend(uint32_t functor) {
    const uint32_t self = ld32(functor + 4);
    call(g::SM_enableLoadingIndicator, {sm(), 1});
    const uint32_t a = api();
    const uint32_t info = ld32(self + F_PARTY_INFO);
    Frame f;
    const uint32_t id = f.alloc(8);
    call(0x45A269, {info, id, ld32(info + 0xC) + 0x28, 8, 0x28});
    const uint32_t comment = strData(self + F_COMMENT);
    const uint32_t other = ld32(functor + 8);
    const uint32_t fo = newFunction(f, 0xF577DC, {other});
    call(APIManager_executePartyApplication, {a, other, ld32(id), ld32(id + 4), comment, fo});
    functionDestroy(fo);
}
// the offer's send {self, ?}: /party/invite/offer for the shown user with the comment
void offerSend(uint32_t functor) {
    const uint32_t self = ld32(functor + 4);
    call(g::SM_enableLoadingIndicator, {sm(), 1});
    const uint32_t a = api();
    const uint32_t hi = ld32(self + F_USER + 4), lo = ld32(self + F_USER);
    const uint32_t comment = strData(self + F_COMMENT);
    const uint32_t other = ld32(functor + 8);
    Frame f;
    const uint32_t fo = newFunction(f, 0xF57A44, {other});
    call(APIManager_executePartyInviteOffer, {a, comment, lo, hi, comment, fo});
    functionDestroy(fo);
}

// their answers {self}(bool ok): failed: the error dialog, not sending unless retried; done: the join alert when
// the party / invitee is taken, else "applied" / "invited"
struct Answer {
    uint32_t api, vtJoined, vtDone, vtError;
    bool joinAlert;
    int32_t text;
};
void answered(uint32_t functor, uint32_t okRef, const Answer &a) {
    const uint8_t ok = ld8(okRef);
    const uint32_t self = ld32(functor + 4);
    call(g::SM_enableLoadingIndicator, {sm(), 0});
    if (!ok) {
        st8(sm() + SM_LOCKED, 1);
        if (ld8(api() + 0x14)) {
            const uint32_t x = api();
            Frame f;
            const uint32_t fo = newFunction(f, a.vtError, {});
            call(g::APIManager_openErrorDialog, {x, fo, 1});
            functionDestroy(fo);
        }
        if (!call(APIManager_needsRetry, {api()})) st8(self + F_SENDING, 0);
        return;
    }
    const uint32_t list = ld32(api() + a.api);
    if (!(a.joinAlert ? ld32(list + 0x44) : ld32(list))) {
        api();
        st8(sm() + SM_LOCKED, 1);
        Frame f;
        const uint32_t td = f.alloc(8);
        call(g::TextData_get, {td, 0, uint32_t(a.text)});
        const uint32_t msg = newStr(f, ld32(ld32(td) + 4));
        const uint32_t fo = newFunction(f, a.vtDone, {});
        call(g::SceneUtil_openOkDialog4Line, {msg, fo, 0});
        functionDestroy(fo);
        strFree(msg);
        releaseIf(ld32(td + 4));
        return;
    }
    Frame f;
    const uint32_t fo = newFunction(f, a.vtJoined, {});
    call(SceneUtil_openPartyInfoJoinAlertPopup, {a.joinAlert ? 1u : 0u, fo});
    functionDestroy(fo);
}
void joinAnswered(uint32_t functor, uint32_t okRef) {
    answered(functor, okRef, {0x210, 0xF57808, 0xF57834, 0xF57860, true, 103810003});
}
void offerAnswered(uint32_t functor, uint32_t okRef) {
    answered(functor, okRef, {0x228, 0xF57A70, 0xF57A9C, 0xF57AC8, false, 103830015});
}

// __func thunks of the lambdas holding two std::function<void(bool)> (at +0x10 and `second`)
uint32_t destroyFn(uint32_t fo) {
    const uint32_t p = ld32(fo + 16);
    if (p == fo) return vcall(p, 0x10, {p});
    if (p) return vcall(p, 0x14, {p});
    return 0;
}
uint32_t twoFnDestroy(uint32_t self, uint32_t second) {
    destroyFn(self + second);
    return destroyFn(self + 0x10);
}
uint32_t twoFnDtor(uint32_t self, uint32_t vt, uint32_t second) {
    st32(self, vt);
    twoFnDestroy(self, second);
    return self;
}
uint32_t expelDtor(uint32_t s) { return twoFnDtor(s, 0xF57AF4, 0x28); }
void expelDelete(uint32_t s) { expelDtor(s); call(fn::operator_delete, {s}); }
uint32_t expelDestroy(uint32_t s) { return twoFnDestroy(s, 0x28); }
void expelDestroyDelete(uint32_t s) { expelDestroy(s); call(fn::operator_delete, {s}); }
uint32_t b78Dtor(uint32_t s) { return twoFnDtor(s, 0xF57B78, 0x30); }
void b78Delete(uint32_t s) { b78Dtor(s); call(fn::operator_delete, {s}); }
uint32_t b78Destroy(uint32_t s) { return twoFnDestroy(s, 0x30); }
uint32_t bfcDtor(uint32_t s) { return twoFnDtor(s, 0xF57BFC, 0x30); }
void bfcDelete(uint32_t s) { bfcDtor(s); call(fn::operator_delete, {s}); }
uint32_t bfcDestroy(uint32_t s) { return twoFnDestroy(s, 0x30); }
uint32_t c80Dtor(uint32_t s) { return twoFnDtor(s, 0xF57C80, 0x28); }
void c80Delete(uint32_t s) { c80Dtor(s); call(fn::operator_delete, {s}); }
uint32_t c80Destroy(uint32_t s) { return twoFnDestroy(s, 0x28); }

// the title list's choice {-, widget, shadow, layer, -, done, ids}(title id): when the list is the current popup it
// closes: it shrinks, then a heap lambda {shadow, widget, layer, -, done, ids, id} ends the choice
void subNameChosen(uint32_t functor, uint32_t idRef) {
    constexpr uint32_t SM_closeCurrentPopup = 0x6216A1;            // (sm, bool) -> bool
    constexpr uint32_t FunctionVI_copy = 0x539C99, FunctionVI_move = 0x543045;
    constexpr uint32_t VectorInt_copy = 0x4AF707, VectorInt_dtor = 0x478961;
    constexpr uint32_t VT_Chosen = 0xF57DE0;
    const uint32_t id = ld32(idRef);
    if (call(SM_closeCurrentPopup, {call(g::SM_getInstance, {}), 1}) != 1) return;
    const uint32_t widget = ld32(functor + 8);
    Frame f;
    const uint32_t copy = f.alloc(0x38);   // {shadow, widget, layer, -, done (+0x10), ids (+0x28), id (+0x34)}
    st32(copy, ld32(functor + 0xC));
    st32(copy + 4, widget);
    st32(copy + 8, ld32(functor + 0x10));
    call(FunctionVI_copy, {copy + 0x10, functor + 0x18});
    call(VectorInt_copy, {copy + 0x28, functor + 0x30});
    st32(copy + 0x34, id);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x40});
    if (obj) {
        st32(obj, VT_Chosen);
        for (uint32_t k = 0; k < 12; k += 4) st32(obj + 8 + k, ld32(copy + k));
        call(FunctionVI_move, {obj + 0x18, copy + 0x10});
        for (uint32_t k = 0; k < 12; k += 4) st32(obj + 0x30 + k, ld32(copy + 0x28 + k));
        for (uint32_t k = 0; k < 12; k += 4) st32(copy + 0x28 + k, 0);
        st32(obj + 0x3C, ld32(copy + 0x34));
    }
    st32(fo + 16, obj);
    call(fn::Node_runAction, {widget, SceneUtil::createClosePopupAnimation(widget, fo)});
    functionDestroy(fo);
    call(VectorInt_dtor, {copy + 0x28});
    functionDestroy(copy + 0x10);
}

// the sub name choice's result {Close, title list, widget, 2 words}(bool chosen): taps off; cancelled: the close
// animation (its end {word, widget, word})
void subNameResult(uint32_t functor, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    vcall(ld32(functor + 4), V_SET_TOUCH_ENABLED, {ld32(functor + 4), 0});
    call(AvatarTitleScrollView_setEnabledTapItem, {ld32(functor + 8), 0});
    if (ok) return;
    call(AudioPlaySystemSECancel, {});
    const uint32_t widget = ld32(functor + 0xC);
    Frame f;
    const uint32_t fo = newFunction(f, 0xF57D30, {ld32(functor + 0x10), widget, ld32(functor + 0x14)});
    call(fn::Node_runAction, {widget, call(SceneUtil_createClosePopupAnimation, {widget, fo})});
    functionDestroy(fo);
}
}  // namespace

RW_FUNCTION(0x53D6CC, "AvatarInfoDialog cancel answered (sub_53D6CC)", cancelAnswered);
RW_FUNCTION(0x53D980, "AvatarInfoDialog cancel done (sub_53D980)", cancelDone);
RW_FUNCTION(0x5408D8, "AvatarInfoDialog expel answered (sub_5408D8)", expelAnswered);
RW_FUNCTION(0x540B9C, "AvatarInfoDialog expel done (sub_540B9C)", expelDone);
RW_FUNCTION(0x53DD30, "AvatarInfoDialog join request popup result (sub_53DD30)", joinResult);
RW_FUNCTION(0x53F410, "AvatarInfoDialog offer popup result (sub_53F410)", offerResult);
RW_FUNCTION(0x53E134, "AvatarInfoDialog join request popup closed (sub_53E134)", joinClosed);
RW_FUNCTION(0x53E4C2, "AvatarInfoDialog join request Cancel (sub_53E4C2)", cancelTouched);
RW_FUNCTION(0x53F8B2, "AvatarInfoDialog offer Cancel (sub_53F8B2)", cancelTouched);
RW_FUNCTION(0x53E638, "AvatarInfoDialog join request OK (sub_53E638)", joinOk);
RW_FUNCTION(0x53FA28, "AvatarInfoDialog offer OK (sub_53FA28)", offerOk);
RW_FUNCTION(0x53EAD4, "AvatarInfoDialog join request empty comment (sub_53EAD4)", joinEmpty);
RW_FUNCTION(0x53FEC4, "AvatarInfoDialog offer empty comment (sub_53FEC4)", offerEmpty);
RW_FUNCTION(0x53ED10, "AvatarInfoDialog join request send (sub_53ED10)", joinSend);
RW_FUNCTION(0x540100, "AvatarInfoDialog offer send (sub_540100)", offerSend);
RW_FUNCTION(0x53EE90, "AvatarInfoDialog join request answered (sub_53EE90)", joinAnswered);
RW_FUNCTION(0x540280, "AvatarInfoDialog offer answered (sub_540280)", offerAnswered);
RW_FUNCTION(0x540684, "AvatarInfoDialog expel lambda __func::~__func (sub_540684)", expelDtor);
RW_FUNCTION(0x5406DC, "AvatarInfoDialog expel lambda __func::~__func (deleting) (sub_5406DC)", expelDelete);
RW_FUNCTION(0x540854, "AvatarInfoDialog expel lambda __func::destroy (sub_540854)", expelDestroy);
RW_FUNCTION(0x540890, "AvatarInfoDialog expel lambda __func::destroy_deallocate (sub_540890)", expelDestroyDelete);
RW_FUNCTION(0x540E1C, "AvatarInfoDialog lambda F57B78 __func::~__func (sub_540E1C)", b78Dtor);
RW_FUNCTION(0x540E74, "AvatarInfoDialog lambda F57B78 __func::~__func (deleting) (sub_540E74)", b78Delete);
RW_FUNCTION(0x540FF4, "AvatarInfoDialog lambda F57B78 __func::destroy (sub_540FF4)", b78Destroy);
RW_FUNCTION(0x5415F4, "AvatarInfoDialog lambda F57BFC __func::~__func (sub_5415F4)", bfcDtor);
RW_FUNCTION(0x54164C, "AvatarInfoDialog lambda F57BFC __func::~__func (deleting) (sub_54164C)", bfcDelete);
RW_FUNCTION(0x5417CC, "AvatarInfoDialog lambda F57BFC __func::destroy (sub_5417CC)", bfcDestroy);
RW_FUNCTION(0x541DC8, "AvatarInfoDialog lambda F57C80 __func::~__func (sub_541DC8)", c80Dtor);
RW_FUNCTION(0x541E20, "AvatarInfoDialog lambda F57C80 __func::~__func (deleting) (sub_541E20)", c80Delete);
RW_FUNCTION(0x541F98, "AvatarInfoDialog lambda F57C80 __func::destroy (sub_541F98)", c80Destroy);
RW_FUNCTION(0x5425D0, "AvatarInfoDialog sub name choice result (sub_5425D0)", subNameResult);
RW_FUNCTION(0x542BE4, "AvatarInfoDialog title list choice (sub_542BE4)", subNameChosen);

RW_FUNCTION(0x53A640, "AvatarInfoDialog::openPopUpOffer(function<void(bool)>)", openPopUpOffer);
RW_FUNCTION(0x53B38C, "AvatarInfoDialog::openPopUpJoinRequest(function<void(bool)>)", openPopUpJoinRequest);
RW_FUNCTION(0x539514, "AvatarInfoDialog::openPopUpSelectSubName(int, int, function<void(int)>)", openPopUpSelectSubName);
RW_FUNCTION(0x53C0F0, "AvatarInfoDialog::openPopUpCancel(function<void(bool)>)", openPopUpCancel);
RW_FUNCTION(0x53C2E4, "AvatarInfoDialog::openPopUpInvitation(function<void(bool)>)", openPopUpInvitation);
RW_FUNCTION(0x53C4D8, "AvatarInfoDialog::openPopUpReject(function<void(bool)>)", openPopUpReject);
RW_FUNCTION(0x53A3E4, "AvatarInfoDialog::openPopUpMemberExpel(function<void(bool)>)", openPopUpMemberExpel);
RW_FUNCTION(0x53CA44, "AvatarInfoDialog reject answered (sub_53CA44)", rejectAnswered);
RW_FUNCTION(0x53D088, "AvatarInfoDialog invitation answered (sub_53D088)", invitationAnswered);
RW_FUNCTION(0x53CCF8, "AvatarInfoDialog reject done (sub_53CCF8)", rejectDone);
RW_FUNCTION(0x53D33C, "AvatarInfoDialog invitation done (sub_53D33C)", invitationDone);
