// AvatarInfoDialog::openPopUpComment: the name + message popup of the Player Information page (button B). The name
// and the message (copied to +0x418 / +0x424 for editing) get edit boxes limited by misc 804 / 805; a player under
// 13 (payment info age) gets the U13 layout: a fixed message label instead of the message box. Also the edit
// boxes' end (editBoxEditingDidEnd) and the payment info callback that opens the popup.
#include "../scene_menu/AvatarInfoDialog.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t FunctionVB_copy = 0x45C84D;              // std::function<void(bool)>(const&)
constexpr uint32_t FunctionVB_move = 0x47BC69;              // std::function<void(bool)>(&&)
constexpr uint32_t SceneManager_openPopup = 0x61524D;       // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Size_ctor = 0x8DF027;                    // (size, float w, float h)
constexpr uint32_t String_assignCopy = 0x498A1D;            // std::string::operator=(const string&)
constexpr uint32_t Misc_create = 0x786D5D;                  // master::Misc::create(shared_ptr out, id)
constexpr uint32_t SafeBuffer_read = 0x45A269;              // (sb, dst, src, len, offset)
constexpr uint32_t StringBuffer_ctor = 0x49140D;            // (buf, fmt, ...)
constexpr uint32_t SceneUtil_settingTitleLabelText = 0x46E8D9;   // (root, string name, string text)
constexpr uint32_t SceneUtil_textFieldToEditBox = 0x45C0A1;
constexpr uint32_t SceneUtil_textFieldToUiText = 0x45C465;  // (field, const char* text, const char* font)
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t String_compare = 0x47CBD5;              // std::string::compare(const char*): 0 = equal
constexpr uint32_t String_assignN = 0x48D811;              // std::string::assign(const char*, size_t)
constexpr uint32_t EditBox_getText = 0x85520D;
constexpr uint32_t SceneUtil_editBoxEditingDidEnd = 0x45C61D;   // (EditBox** box, Label* label, const char* text)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;  // (api, const function<void(bool)>&, bool)
constexpr uint32_t openPopUpComment = 0x536481;
constexpr uint32_t STRLEN = 0x40E08C;
}  // namespace g

constexpr uint32_t S_Comment = 0xDFE39D, S_CommentU13 = 0xDFE3C8, S_TxtName = 0xDFDA10, S_FontSoGei = 0xDF9F4D,
                   S_TxtLimit1 = 0xDFE3F7, S_TxtComment = 0xDF9EF7, S_TxtCommentLabelU13 = 0xDFE402,
                   S_FontCalligraphy = 0xDF9F1F, S_TxtLimit2 = 0xDFE418, S_ButtonClose = 0xDF8692,
                   S_ButtonOk = 0xDF9564, S_TxtOK = 0xDF869F, S_TxtCancel = 0xDF86A6;
constexpr uint32_t G_TXT_NAME_LABEL = 0xFD6614;       // GOT: &TXT_NAME_LABEL (const char*)
constexpr uint32_t G_TXT_COMMENT_LABEL = 0xFD6618;    // GOT: &TXT_COMMENT_LABEL
constexpr uint32_t G_TXT_COMMENT_INPUT = 0xFD661C;    // GOT: &TXT_COMMENT_INPUT
constexpr uint32_t G_SHADOW_FADE = 0xFD65C0;          // GOT: the shadow fade-in time (float)
constexpr int32_t TXT_NAME = 102300001, TXT_COMMENT = 102300002, TXT_LIMIT = 100100037, TXT_OK = 100100001,
                  TXT_CANCEL = 100100002;
constexpr uint32_t MISC_NAME_LIMIT = 804, MISC_COMMENT_LIMIT = 805;
constexpr int32_t ADULT_AGE = 13;

constexpr uint32_t D_EDIT_DELEGATE = 0x270;           // AvatarInfoDialog: EditBoxDelegate base
constexpr uint32_t D_NAME = 0x2CC, D_COMMENT = 0x2D8; // std::string the player's name / message
constexpr uint32_t D_EDIT_NAME = 0x418, D_EDIT_COMMENT = 0x424;   // their copies being edited
constexpr uint32_t D_COMMENT_TEXT = 0x450;            // ui::Text over the message field
constexpr uint32_t D_ON_POPUP_CLOSED = 0x480;         // std::function<void(bool)> parked for the comment popup
constexpr uint32_t D_EDIT_BOX = 0x4B8, D_EDIT_LABEL = 0x4BC, D_EDIT_CLOSING = 0x4C0;
constexpr uint32_t API_USER_DETAIL_COUNT = 0x78;      // APIManager: the user detail shared_ptr's count
constexpr uint32_t API_PAYMENT = 0x1D4;               // shared_ptr<SafeBuffer> payment info (age at +0)

constexpr uint32_t V_SET_VISIBLE = 0xB0, V_GET_OPACITY = 0x20C, V_SET_OPACITY = 0x214, V_ADD_CHILD = 0xF0;
constexpr uint32_t V_SET_ANCHOR = 0x9C, V_SET_POSITION = 0x4C, V_GET_X = 0x68, V_GET_Y = 0x70, V_GET_SIZE = 0xAC;
constexpr uint32_t V_FIELD_HIDE = 0x11C;              // TextField vslot 0x11C (called before the text replaces it)
constexpr uint32_t V_GET_PLACEHOLDER = 0x150;         // TextField::getPlaceHolder (string out)
constexpr uint32_t V_BOX_NAME = 0x150;                // the EditBox's vslot 0x150: the name it was given (string out)

constexpr uint32_t VT_OkTouch = 0xF5817C;             // Button_Ok {payment shared_ptr, dialog, close, ok} (heap 0x18)
constexpr uint32_t VT_CancelTouch = 0xF58150;         // Button_Close (no captures)
constexpr uint32_t VT_CanClose = 0xF58124;            // openPopup's function<bool()> {close}
constexpr uint32_t VT_Closed = 0xF580CC;              // openPopup's result (heap 0x48)
constexpr uint32_t VT_PaymentError = 0xF588B4;        // createLayout $_3 payment info's error dialog callback
constexpr uint32_t API_OFFLINE = 0x14;                // APIManager: a request failed (byte)

// Closed lambda layouts (stack): {close, ok, view, layer, shadow, -, onClosed} and
// {dialog, ok, close, -, that lambda}
constexpr uint32_t C_FN = 0x18, C_SIZE = 0x30;
constexpr uint32_t R_CLOSER = 0x10, R_SIZE = 0x40;


int32_t readMisc(uint32_t id) {
    Frame f;
    const uint32_t row = f.alloc(8);
    call(g::Misc_create, {row, id});
    const uint32_t sb = ld32(row);
    const uint32_t v = f.alloc(4);
    call(g::SafeBuffer_read, {sb, v, ld32(sb + 12) + 4, 4, 4});
    sharedRelease(row);
    return int32_t(ld32(v));
}

// "Txt_Limit": the limit text (TextData format with the length)
void limitLabel(uint32_t view, uint32_t name, int32_t limit) {
    Frame f;
    const uint32_t n = newStr(f, name, 0xA);
    const uint32_t fmt = textData(f, TXT_LIMIT);
    const uint32_t buf = f.alloc(STRING_BUFFER_SIZE);
    call(g::StringBuffer_ctor, {buf, textDataStr(fmt), uint32_t(limit)});
    const uint32_t text = newStr(f, buf);
    call(g::SceneUtil_settingTitleLabelText, {view, n, text});
    strFree(text);
    sharedRelease(fmt);
    strFree(n);
}

uint32_t editBox(uint32_t dialog, uint32_t field, uint32_t text, uint32_t font, float h, int32_t limit) {
    Frame f;
    const uint32_t placeholder = f.alloc(12);
    vcall(field, V_GET_PLACEHOLDER, {placeholder, field});
    const uint32_t size = f.alloc(8);
    call(g::Size_ctor, {size, fbits(400.0f), fbits(h)});
    const uint32_t box = rw::call(g::SceneUtil_textFieldToEditBox,
                                  {dialog + D_EDIT_BOX, dialog + D_EDIT_LABEL, field, placeholder, text, font, size,
                                   dialog + D_EDIT_DELEGATE, dialog + D_EDIT_CLOSING, 0, uint32_t(limit)});
    strFree(placeholder);
    return box;
}

struct AvatarInfoComment {
    static void open(uint32_t dialog, uint32_t onClosed) {
        call(fn::AudioPlaySystemSEPopUp, {});
        call(g::String_assignCopy, {dialog + D_EDIT_NAME, dialog + D_NAME});
        call(g::String_assignCopy, {dialog + D_EDIT_COMMENT, dialog + D_COMMENT});
        uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t paymentCount = ld32(api + API_PAYMENT + 4), payment = ld32(api + API_PAYMENT);
        if (paymentCount) call(fn::shared_add, {paymentCount});
        const uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
        const uint32_t shadow = SceneUtil::createShadowSprite();
        const uint32_t opacity = vcall(shadow, V_GET_OPACITY, {shadow});
        vcall(shadow, V_SET_OPACITY, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {ld32(ld32(G_SHADOW_FADE)), opacity})});
        vcall(layer, V_ADD_CHILD, {layer, shadow});
        Frame f;
        const uint32_t age = f.alloc(4);
        uint32_t json = S_Comment;
        if (payment) {
            call(g::SafeBuffer_read, {payment, age, ld32(payment + 12), 4, 0});
            json = int32_t(ld32(age)) >= ADULT_AGE ? S_Comment : S_CommentU13;
        }
        const uint32_t view = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
        vcall(view, V_SET_ANCHOR, {view, VEC2_ANCHOR_MIDDLE});
        {
            const float px = bitsf(vcall(view, V_GET_X, {view}));
            const float w = ldf(vcall(view, V_GET_SIZE, {view}));
            const float x = SceneUtil::getSideMargin() + (px + w * 0.5f);
            const float py = bitsf(vcall(view, V_GET_Y, {view}));
            const float h = ldf(vcall(view, V_GET_SIZE, {view}) + 4);
            const float y = SceneUtil::getSideHeightMargin() + (py + h * 0.5f);
            const uint32_t pos = f.alloc(8);
            call(g::Vec2_ctor, {pos, fbits(x), fbits(y)});
            vcall(view, V_SET_POSITION, {view, pos});
            call(g::Vec2_dtor, {pos});
        }
        vcall(shadow, V_ADD_CHILD, {shadow, view});
        api = call(fn::APIManager_getInstance, {});
        const uint32_t detailCount = ld32(api + API_USER_DETAIL_COUNT);
        if (detailCount) call(fn::shared_add, {detailCount});

        {
            const uint32_t name = findNode(view, S_TxtName, 8);
            setTextId(name, TXT_NAME);
            SceneUtil::setShadow(name);
        }
        const uint32_t nameField = findNode(view, ld32(ld32(G_TXT_NAME_LABEL)),
                                            call(g::STRLEN, {ld32(ld32(G_TXT_NAME_LABEL))}));
        const int32_t nameLimit = readMisc(MISC_NAME_LIMIT);
        st8(dialog + D_EDIT_CLOSING, 0);
        editBox(dialog, nameField, strData(dialog + D_EDIT_NAME), S_FontSoGei, 87.0f, nameLimit);
        limitLabel(view, S_TxtLimit1, nameLimit);
        {
            const uint32_t text = findNode(view, S_TxtComment, 0xB);
            setTextId(text, TXT_COMMENT);
            SceneUtil::setShadow(text);
        }
        bool child = false;
        if (payment) {
            call(g::SafeBuffer_read, {payment, age, ld32(payment + 12), 4, 0});
            child = int32_t(ld32(age)) < ADULT_AGE;
        }
        if (child) {
            const uint32_t label = findNode(view, S_TxtCommentLabelU13, 0x15);
            call(fn::Text_setString, {label, dialog + D_EDIT_COMMENT});
            SceneUtil::setShadow(label);
        } else {
            const uint32_t field = findNode(view, ld32(ld32(G_TXT_COMMENT_LABEL)),
                                            call(g::STRLEN, {ld32(ld32(G_TXT_COMMENT_LABEL))}));
            const int32_t limit = readMisc(MISC_COMMENT_LIMIT);
            vcall(field, V_FIELD_HIDE, {field});
            st32(dialog + D_COMMENT_TEXT,
                 call(g::SceneUtil_textFieldToUiText, {field, strData(dialog + D_EDIT_COMMENT), S_FontCalligraphy}));
            st8(dialog + D_EDIT_CLOSING, 0);
            const uint32_t box = editBox(dialog, field, strData(dialog + D_EDIT_COMMENT), S_FontCalligraphy, 136.0f,
                                         limit);
            vcall(box, V_SET_VISIBLE, {box, 0});
            limitLabel(view, S_TxtLimit2, limit);
        }
        const uint32_t close = findNode(view, S_ButtonClose, 0xC);
        const uint32_t ok = findNode(view, S_ButtonOk, 9);
        const uint32_t closer = f.alloc(C_SIZE);
        st32(closer, close);
        st32(closer + 4, ok);
        st32(closer + 8, view);
        st32(closer + 0xC, layer);
        st32(closer + 0x10, shadow);
        call(g::FunctionVB_copy, {closer + C_FN, onClosed});
        {
            const uint32_t text = findNode(view, S_TxtOK, 6);
            setTextId(text, TXT_OK);
            SceneUtil::setShadow(text);
        }
        {
            if (paymentCount) call(fn::shared_add, {paymentCount});
            Frame t;
            const uint32_t fo = t.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x18});
            uint32_t leftover = paymentCount;
            if (obj) {
                st32(obj, VT_OkTouch);
                st32(obj + 4, payment);
                st32(obj + 8, paymentCount);
                st32(obj + 0xC, dialog);
                st32(obj + 0x10, close);
                st32(obj + 0x14, ok);
                leftover = 0;
            }
            st32(fo + 16, obj);
            call(fn::Widget_addTouchEventListener, {ok, fo});
            functionDestroy(fo);
            if (leftover) call(fn::shared_release, {leftover});
        }
        {
            const uint32_t text = findNode(view, S_TxtCancel, 0xA);
            setTextId(text, TXT_CANCEL);
            SceneUtil::setShadow(text);
        }
        addTouch(close, VT_CancelTouch, {});

        const uint32_t result = f.alloc(R_SIZE);
        st32(result, dialog);
        st32(result + 4, ok);
        st32(result + 8, close);
        for (uint32_t k = 0; k < 5; k++) st32(result + R_CLOSER + 4 * k, ld32(closer + 4 * k));
        functionCopy(result + R_CLOSER + C_FN, closer + C_FN);
        {
            const uint32_t sm = sceneManager();
            Frame t;
            const uint32_t canClose = newFunction(t, VT_CanClose, {close});
            const uint32_t copy = t.alloc(R_SIZE);   // the by-value copy of the result lambda
            for (uint32_t k = 0; k < 3; k++) st32(copy + 4 * k, ld32(result + 4 * k));
            for (uint32_t k = 0; k < 5; k++) st32(copy + R_CLOSER + 4 * k, ld32(result + R_CLOSER + 4 * k));
            functionCopy(copy + R_CLOSER + C_FN, result + R_CLOSER + C_FN);
            const uint32_t closed = t.alloc(20);
            st32(closed + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x48});
            if (obj) {
                st32(obj, VT_Closed);
                for (uint32_t k = 0; k < 3; k++) st32(obj + 8 + 4 * k, ld32(copy + 4 * k));
                for (uint32_t k = 0; k < 5; k++) st32(obj + 0x18 + 4 * k, ld32(copy + R_CLOSER + 4 * k));
                call(g::FunctionVB_move, {obj + 0x30, copy + R_CLOSER + C_FN});
            }
            st32(closed + 16, obj);
            call(g::SceneManager_openPopup, {sm, canClose, closed, 1});
            functionDestroy(closed);
            functionDestroy(copy + R_CLOSER + C_FN);
            functionDestroy(canClose);
        }
        {
            Frame t;
            const uint32_t none = newEmptyFunction(t);
            const uint32_t action = SceneUtil::createOpenPopupAnimation(view, none);
            call(fn::Node_runAction, {view, call(g::EaseSineOut_create, {action})});
            functionDestroy(none);
        }
        functionDestroy(result + R_CLOSER + C_FN);
        functionDestroy(closer + C_FN);
        if (detailCount) call(fn::shared_release, {detailCount});
        if (paymentCount) call(fn::shared_release, {paymentCount});
    }

    // EditBoxDelegate::editBoxEditingDidEnd: the box's text into the name / message being edited (the message
    // also shown in its ui::Text), then SceneUtil's common end (the box hidden, its label back)
    static void editingDidEnd(uint32_t dialog, uint32_t box) {
        Frame f;
        const uint32_t name = f.alloc(12);
        vcall(box, V_BOX_NAME, {name, box});
        if (!call(g::String_compare, {name, ld32(ld32(G_TXT_NAME_LABEL))})) {
            const uint32_t text = call(g::EditBox_getText, {box});
            call(g::String_assignN, {dialog + D_EDIT_NAME, text, call(g::STRLEN, {text})});
        } else if (!call(g::String_compare, {name, ld32(ld32(G_TXT_COMMENT_LABEL))})) {
            const uint32_t text = call(g::EditBox_getText, {box});
            call(g::String_assignN, {dialog + D_EDIT_COMMENT, text, call(g::STRLEN, {text})});
            if (const uint32_t label = ld32(dialog + D_COMMENT_TEXT)) {
                const uint32_t s = newStr(f, strData(dialog + D_EDIT_COMMENT));
                call(fn::Text_setString, {label, s});
                strFree(s);
            }
        } else if (!call(g::String_compare, {name, ld32(ld32(G_TXT_COMMENT_INPUT))})) {
            const uint32_t text = call(g::EditBox_getText, {box});
            const uint32_t typed = newStr(f, text);
            if (const uint32_t label = ld32(dialog + D_COMMENT_TEXT)) {
                const uint32_t s = newStr(f, strData(typed));
                call(fn::Text_setString, {label, s});
                strFree(s);
            }
            strFree(typed);
        }
        const uint32_t editLabel = ld32(dialog + D_EDIT_LABEL);
        call(g::SceneUtil_editBoxEditingDidEnd, {dialog + D_EDIT_BOX, editLabel, call(g::EditBox_getText, {box})});
        strFree(name);
    }

    // createLayout $_3's lambda()'s lambda(bool ok) {dialog}: the payment info arrived - the comment popup with
    // the parked callback, or the error dialog
    static void paymentInfoDone(uint32_t functor, uint32_t okRef) {
        const uint32_t ok = ld8(okRef), dialog = ld32(functor + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        Frame f;
        if (ok) {
            const uint32_t cb = f.alloc(20);
            call(g::FunctionVB_copy, {cb, dialog + D_ON_POPUP_CLOSED});
            call(g::openPopUpComment, {dialog, cb});
            functionDestroy(cb);
            return;
        }
        if (!ld8(call(fn::APIManager_getInstance, {}) + API_OFFLINE)) return;
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t cb = newFunction(f, VT_PaymentError, {});
        call(g::APIManager_openErrorDialog, {api, cb, 1});
        functionDestroy(cb);
    }
};
}  // namespace

RW_FUNCTION(0x536480, "AvatarInfoDialog::openPopUpComment(function<void(bool)>)", AvatarInfoComment::open);
RW_FUNCTION(0x53743C, "AvatarInfoDialog::editBoxEditingDidEnd(EditBox*)", AvatarInfoComment::editingDidEnd);
RW_FUNCTION(0x54C888, "AvatarInfoDialog::createLayout::$_3::lambda()::lambda(bool) operator() (payment info)",
            AvatarInfoComment::paymentInfoDone);
