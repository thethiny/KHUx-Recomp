// AvatarInfoDialog popups (party member info): the OK / Cancel confirmations (expel, cancel, invitation, reject)
// and their request lambdas, the party offer / join request popups (comment field) and the sub name choice.
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
    constexpr uint32_t FadeTo_create = 0x86B3E1, Node_runAction = 0x894AED;
    constexpr uint32_t GUIReader_getInstance = 0x7BF1E5, GUIReader_widgetFromJsonFile = 0x7BF9FD;
    constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
    constexpr uint32_t Text_setString = 0x83A2DD;
    constexpr uint32_t VectorInt_pushBackSlow = 0x47D709, VectorInt_copy = 0x4AF707, VectorInt_dtor = 0x478961;
    constexpr uint32_t SafeBuffer_read = 0x45A269;
    constexpr uint32_t AvatarTitleScrollView_create = 0x510A45, BaseScrollViewInt_setItemIDs = 0x478929;
    constexpr uint32_t FunctionVI_copy = 0x539C99, FunctionVI_copy2 = 0x543045, FunctionVI_swap = 0x5428E1;
    constexpr uint32_t Widget_addTouchEventListener = 0x826CCF, SM_openPopup = 0x61524D;
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
    constexpr uint32_t FadeTo_create = 0x86B3E1, Node_runAction = 0x894AED;
    constexpr uint32_t GUIReader_getInstance = 0x7BF1E5, GUIReader_widgetFromJsonFile = 0x7BF9FD;
    constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17, Size_ctor = 0x8DF027;
    constexpr uint32_t Text_setString = 0x83A2DD, Text_getString = 0x83A33B, String_assign = 0x498A1D;
    constexpr uint32_t String_ctor = 0x48D811, SafeBuffer_read = 0x45A269, MasterMisc_create = 0x786D5D;
    constexpr uint32_t FunctionVV_swap = 0x4EE86D, Widget_addTouchEventListener = 0x826CCF;
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
}  // namespace

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
