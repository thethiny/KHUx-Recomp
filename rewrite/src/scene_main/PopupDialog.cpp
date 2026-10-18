// PopupDialog: the standard text popup (cocostudio PopupNormal_Text_34_*): a 4- or 8-line text, an OK button and
// optionally a Cancel button, opened over a parent node with a shadow and SceneManager's popup stack.
#include "scene/SceneCommon.hpp"

using namespace scene;

namespace {
// PopupDialog : cocos2d::Node (0x2B0 bytes)
constexpr uint32_t SIZE = 0x2B0;
constexpr uint32_t VT_POPUP_DIALOG = 0xF7FF54;
constexpr uint32_t P_ROOT = 0x270;           // the cocostudio widget
constexpr uint32_t P_PARENT = 0x274;         // set while open
constexpr uint32_t P_SHADOW = 0x278;         // the shadow sprite under the popup
constexpr uint32_t P_OK_BUTTON = 0x27C, P_OK_TEXT = 0x280, P_CANCEL_BUTTON = 0x284, P_CANCEL_TEXT = 0x288;
constexpr uint32_t P_BUTTON_TYPE = 0x28C;    // 0 OK, 1 OK + Cancel
constexpr uint32_t P_WINDOW_TYPE = 0x290;    // 0 four lines, 1 eight lines
constexpr uint32_t P_ON_CLOSE = 0x298;       // std::function<void(bool ok)>
constexpr uint32_t OK_CANCEL = 1, EIGHT_LINES = 1;
constexpr uint32_t TOUCH_ENDED = 2;

// lambdas (std::function vtables)
constexpr uint32_t VT_INIT_OK = 0xF559F4;       // init::$_0: OK tapped
constexpr uint32_t VT_INIT_CANCEL = 0xF559C8;   // init::$_1: Cancel tapped
constexpr uint32_t VT_OPEN_CAN_CLOSE = 0xF5599C;   // open::$_2: SceneManager asks whether it may close it
constexpr uint32_t VT_OPEN_CLOSED = 0xF55970;      // open::$_3: SceneManager closes it (ok / cancel)
constexpr uint32_t VT_CLOSE_DONE = 0xF55944;       // close::$_4: the close animation finished

constexpr uint32_t S_8LINE_OKCANCEL = 0xDFCEB7, S_8LINE_OK = 0xDFCE83;   // "cocostudio/publish/PopupNormal_Text_34_..."
constexpr uint32_t S_4LINE_OKCANCEL = 0xDFCE48, S_4LINE_OK = 0xDFCE13;
constexpr uint32_t S_8LINE_LABEL = 0xDFCEF2, S_4LINE_LABEL = 0xDF87A2;
constexpr uint32_t S_BUTTON_OK = 0xDF8688, S_TXT_OK = 0xDF869F, S_BUTTON_CLOSE = 0xDF8692, S_TXT_CANCEL = 0xDF86A6;
constexpr int32_t TEXT_OK = 100100001, TEXT_CANCEL = 100100002;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;   // &cocos2d::Vec2::ANCHOR_MIDDLE
constexpr float FADE_SECONDS = 0.1f;

constexpr uint32_t AEABI_MEMSET = 0x40E7A0;   // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t Node_ctor = 0x892531, Node_dtor = 0x8927ED, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5, GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5, SceneUtil_textToCustomTextView = 0x45BF15;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15, SceneUtil_getSideMargin = 0x45AD0D;
constexpr uint32_t SceneUtil_getSideHeightMargin = 0x45AF59, SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t SceneManager_getInstance = 0x60C26D, SceneManager_openPopup = 0x61524D;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t function_vb_assign = 0x490109;   // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t function_vb_call = 0x474139;     // std::function<void(bool)>::operator()(bool)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t Widget_setTouchEnabled = 0x29C;   // ui::Widget vtable byte offset

// the text label's name for the window type ("" for an unknown type)
uint32_t newLabelName(Frame &f, uint32_t self) {
    const uint32_t name = f.alloc(12);
    st32(name, 0);
    st32(name + 4, 0);
    st32(name + 8, 0);
    const uint32_t windowType = ld32(self + P_WINDOW_TYPE);
    if (windowType == EIGHT_LINES)
        call(fn::string_assign, {name, S_8LINE_LABEL, 11});
    else if (windowType == 0)
        call(fn::string_assign, {name, S_4LINE_LABEL, 11});
    return name;
}

uint32_t findNode(uint32_t parent, uint32_t name, uint32_t length) {
    Frame f;
    const uint32_t s = newStr(f, name, length);
    const uint32_t node = call(SceneUtil_searchNodeByName, {parent, s});
    strFree(s);
    return node;
}

// a button that calls `listener` (a lambda capturing the dialog) when touched
void listenTouches(uint32_t button, uint32_t listener, uint32_t self) {
    vcall(button, Widget_setTouchEnabled, {button, 1});
    Frame f;
    const uint32_t fo = newFunction(f, listener, {self});
    call(Widget_addTouchEventListener, {button, fo});
    functionDestroy(fo);
}

// a Text with the shadow style and a TextData text
void setUpLabel(uint32_t text, int32_t textId) {
    call(SceneUtil_setShadow, {text});
    Frame f;
    const uint32_t data = f.alloc(8);
    call(fn::TextData_get, {data, 0, uint32_t(textId)});
    const uint32_t s = f.alloc(12);
    strInitC(s, ld32(ld32(data) + 4));
    call(fn::Text_setString, {text, s});
    strFree(s);
    sharedRelease(data);
}
}  // namespace

namespace popup_dialog {

struct PopupDialog {
    static uint32_t create(uint32_t buttonType, uint32_t windowType) {
        const uint32_t self = call(fn::operator_new, {SIZE});
        call(AEABI_MEMSET, {self, SIZE, 0});
        call(Node_ctor, {self});
        st32(self, VT_POPUP_DIALOG);
        st32(self + 0x2A8, 0);   // the empty on-close std::function
        call(AEABI_MEMSET, {self + P_ROOT, 0x24, 0});
        init(self, buttonType, windowType);
        call(Ref_autorelease, {self});
        return self;
    }

    static uint32_t init(uint32_t self, uint32_t buttonType, uint32_t windowType) {
        st32(self + P_PARENT, 0);
        st32(self + P_BUTTON_TYPE, buttonType);
        st32(self + P_WINDOW_TYPE, windowType);
        uint32_t layout = 0;
        if (windowType == EIGHT_LINES) {
            if (buttonType == OK_CANCEL) layout = S_8LINE_OKCANCEL;
            else if (buttonType == 0) layout = S_8LINE_OK;
        } else if (windowType == 0) {
            if (buttonType == OK_CANCEL) layout = S_4LINE_OKCANCEL;
            else if (buttonType == 0) layout = S_4LINE_OK;
        }
        if (layout) {
            const uint32_t reader = call(GUIReader_getInstance, {});
            st32(self + P_ROOT, call(GUIReader_widgetFromJsonFile, {reader, layout}));
        }
        Frame f;
        const uint32_t label = newLabelName(f, self);
        const uint32_t root = ld32(self + P_ROOT);
        call(SceneUtil_textToCustomTextView, {call(SceneUtil_searchNodeByName, {root, label}), label});

        st32(self + P_OK_BUTTON, findNode(ld32(self + P_ROOT), S_BUTTON_OK, 9));
        listenTouches(ld32(self + P_OK_BUTTON), VT_INIT_OK, self);
        st32(self + P_OK_TEXT, findNode(ld32(self + P_OK_BUTTON), S_TXT_OK, 6));
        setUpLabel(ld32(self + P_OK_TEXT), TEXT_OK);
        if (buttonType == OK_CANCEL) {
            st32(self + P_CANCEL_BUTTON, findNode(ld32(self + P_ROOT), S_BUTTON_CLOSE, 12));
            listenTouches(ld32(self + P_CANCEL_BUTTON), VT_INIT_CANCEL, self);
            st32(self + P_CANCEL_TEXT, findNode(ld32(self + P_CANCEL_BUTTON), S_TXT_CANCEL, 10));
            setUpLabel(ld32(self + P_CANCEL_TEXT), TEXT_CANCEL);
        }

        // centred on the popup's own size, inside the side margins
        vcall(self, node::setAnchorPoint, {self, ld32(GOT_ANCHOR_MIDDLE)});
        const uint32_t contentSize = vcall(ld32(self + P_ROOT), node::getContentSize, {ld32(self + P_ROOT)});
        vcall(self, node::setContentSize, {self, contentSize});
        const float marginX = bitsf(call(SceneUtil_getSideMargin, {}));
        const float width = ldf(vcall(ld32(self + P_ROOT), node::getContentSize, {ld32(self + P_ROOT)}));
        vcall(self, node::setPositionX, {self, fbits(marginX + width * 0.5f)});
        const float marginY = bitsf(call(SceneUtil_getSideHeightMargin, {}));
        const float height = ldf(vcall(ld32(self + P_ROOT), node::getContentSize, {ld32(self + P_ROOT)}) + 4);
        vcall(self, node::setPositionY, {self, fbits(marginY + height * 0.5f)});
        vcall(self, node::addChild, {self, ld32(self + P_ROOT)});
        strFree(label);
        return 1;
    }

    // over `parent` with the text `xml` (CustomTextView markup); onClose(ok) runs when it closes
    static void open(uint32_t self, uint32_t parent, uint32_t xml, uint32_t onClose) {
        st32(self + P_PARENT, parent);
        call(function_vb_assign, {self + P_ON_CLOSE, onClose});
        Frame f;
        const uint32_t label = newLabelName(f, self);
        call(CustomTextView_setXml, {call(SceneUtil_searchNodeByName, {ld32(self + P_ROOT), label}), xml});

        // the shadow fades in from transparent to its own opacity
        st32(self + P_SHADOW, call(SceneUtil_createShadowSprite, {}));
        const uint32_t shadow = ld32(self + P_SHADOW);
        const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(FADE_SECONDS), opacity})});
        vcall(shadow, node::addChild, {shadow, self});
        vcall(parent, node::addChildZ, {parent, shadow, 100});

        const uint32_t done = newEmptyFunction(f);
        const uint32_t animation = call(SceneUtil_createOpenPopupAnimation, {self, done});
        call(fn::Node_runAction, {self, call(EaseSineOut_create, {animation})});
        functionDestroy(done);

        const uint32_t buttonType = ld32(self + P_BUTTON_TYPE);
        const uint32_t manager = call(SceneManager_getInstance, {});
        const uint32_t canClose = newFunction(f, VT_OPEN_CAN_CLOSE, {self});
        const uint32_t closed = newFunction(f, VT_OPEN_CLOSED, {self});
        call(SceneManager_openPopup, {manager, canClose, closed, buttonType == OK_CANCEL ? 1u : 0u});
        functionDestroy(closed);
        functionDestroy(canClose);
        strFree(label);
    }

    // animates out, calling the on-close callback first (once: only while open)
    static void close(uint32_t self, uint32_t ok) {
        if (!ld32(self + P_PARENT)) return;
        st32(self + P_PARENT, 0);
        if (ld32(self + P_ON_CLOSE + 16)) call(function_vb_call, {self + P_ON_CLOSE, ok});
        Frame f;
        const uint32_t done = newFunction(f, VT_CLOSE_DONE, {self});
        const uint32_t animation = call(SceneUtil_createClosePopupAnimation, {self, done});
        call(fn::Node_runAction, {self, call(EaseSineOut_create, {animation})});
        functionDestroy(done);
        const uint32_t shadow = ld32(self + P_SHADOW);
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(FADE_SECONDS), 0})});
    }

    static void destroyMembers(uint32_t self) {
        st32(self, VT_POPUP_DIALOG);
        functionDestroy(self + P_ON_CLOSE);
        call(Node_dtor, {self});
    }
    static uint32_t destruct(uint32_t self) {
        destroyMembers(self);
        return self;
    }
    static void destructDelete(uint32_t self) {
        destroyMembers(self);
        call(fn::operator_delete, {self});
    }

    static void setOkLabel(uint32_t self, uint32_t text) { setButtonText(ld32(self + P_OK_TEXT), text); }
    static void setCancelLabel(uint32_t self, uint32_t text) { setButtonText(ld32(self + P_CANCEL_TEXT), text); }
    static void setButtonText(uint32_t label, uint32_t text) {
        if (!label) return;
        Frame f;
        const uint32_t s = f.alloc(12);
        call(fn::string_init, {s, text, host_strlen(text)});
        call(fn::Text_setString, {label, s});
        strFree(s);
    }

    // init::$_0 / $_1: a touch on OK / Cancel; on release the popup on top closes (ok / not ok) while open
    static void onOkTouched(uint32_t functor, uint32_t, uint32_t type) { onButton(functor, type, 1); }
    static void onCancelTouched(uint32_t functor, uint32_t, uint32_t type) { onButton(functor, type, 0); }
    static void onButton(uint32_t functor, uint32_t type, uint32_t ok) {
        if (ld32(type) != TOUCH_ENDED || !ld32(ld32(functor + 4) + P_PARENT)) return;
        call(SceneManager_closeCurrentPopup, {call(SceneManager_getInstance, {}), ok});
    }

    // open::$_3: SceneManager closed the popup: the decide / cancel sound, then close(ok)
    static void onClosed(uint32_t functor, uint32_t okArg) {
        const uint32_t self = ld32(functor + 4);
        const uint8_t ok = ld8(okArg);
        call(ok ? AudioPlaySystemSEDecide : AudioPlaySystemSECancel, {});
        close(self, ok ? 1 : 0);
    }
};
}  // namespace popup_dialog

using popup_dialog::PopupDialog;
RW_FUNCTION(0x4F3C14, "PopupDialog::create", PopupDialog::create);
RW_FUNCTION(0x4F3CA4, "PopupDialog::init", PopupDialog::init);
RW_FUNCTION(0x4F420C, "PopupDialog::open", PopupDialog::open);
RW_FUNCTION(0x4F4464, "PopupDialog::close", PopupDialog::close);
RW_FUNCTION(0x4F4A14, "PopupDialog::~PopupDialog", PopupDialog::destruct);
RW_FUNCTION(0x4F4A60, "PopupDialog::~PopupDialog (deleting)", PopupDialog::destructDelete);
RW_FUNCTION(0x4F4568, "PopupDialog::setOkLabel", PopupDialog::setOkLabel);
RW_FUNCTION(0x4F4604, "PopupDialog::setCancelLabel", PopupDialog::setCancelLabel);
RW_FUNCTION(0x4F49BE, "PopupDialog::init::$_0 operator() (OK touched)", PopupDialog::onOkTouched);
RW_FUNCTION(0x4F490A, "PopupDialog::init::$_1 operator() (Cancel touched)", PopupDialog::onCancelTouched);
RW_FUNCTION(0x4F47A6, "PopupDialog::open::$_3 operator() (closed)", PopupDialog::onClosed);
