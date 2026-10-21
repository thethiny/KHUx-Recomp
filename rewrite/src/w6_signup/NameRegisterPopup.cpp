// PopupRegister::openNameRegisterPopup: the player name popup of a new account (the name field as an edit box
// limited to misc 804 characters) and its OK: an empty name or one with spaces only gets an error dialog, else the
// name goes out with the tutorial progress (phase 4); once sent the popup closes and the done callback runs.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_getSideMargin = 0x45AD0D;
constexpr uint32_t SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_settingTitleLabelStr = 0x46E8D9;     // (node, const string& name, const string& text)
constexpr uint32_t SceneUtil_splitSpaceCheck = 0x47898D;           // (const string&): not only spaces
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;         // (const string&, function<void(bool)>, okLabel)
constexpr uint32_t EaseSineOut_create = 0x860D09, FadeTo_create = 0x86B3E1, Node_runAction = 0x894AED;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17, Size_ctor = 0x8DF027;
constexpr uint32_t Label_setAlignment = 0x884CB9;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_executeTutorialProgress = 0x4C581D, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t function_vv_copy = 0x4683CD, function_vv_copy2 = 0x47F905, function_vv_call = 0x470E69;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054, GOT_CLOSE_FADE = 0xFD65C0;   // &Vec2::ANCHOR_MIDDLE, &float
constexpr uint32_t VT_OkTouch = 0xF58E60;                  // $_10 {label, scene, OK, root, layer, shadow, done} (0x40)
constexpr uint32_t VT_Sent = 0xF58E8C;                     // $_11 {scene, OK, root, layer, shadow, done} (0x38)
constexpr uint32_t VT_Closed = 0xF58EB8;                   // $_11's close-animation end {layer, shadow, root}
constexpr uint32_t VT_ErrorClosed = 0xF58EE4;              // {}
constexpr uint32_t S_Json = 0xDFF259, S_Wording1 = 0xDFF14C, S_TxtName = 0xDFDA10, S_TxtOK = 0xDF869F;
constexpr uint32_t S_TextFieldName = 0xDFF282, S_Font = 0xDF9F4D, S_Empty = 0xDF8A1A, S_TxtLimit1 = 0xDFE3F7;
constexpr uint32_t S_ButtonOK = 0xDF8688;
constexpr uint32_t V_setPosition = 0x4C, V_getPositionX = 0x68, V_getPositionY = 0x70, V_setAnchorPoint = 0x9C;
constexpr uint32_t V_getContentSize = 0xAC, V_addChild = 0xF0, V_getPlaceHolder = 0x150, V_getOpacity = 0x20C;
constexpr uint32_t V_setOpacity = 0x214, V_setTouchEnabled = 0x29C, V_getString = 0x2AC;
constexpr uint32_t LABEL_V_ALIGNMENT = 0x3E0;
constexpr uint32_t MISC_NAME_LENGTH = 804;
constexpr uint32_t API_RETRY_DIALOG = 0x14;
constexpr uint32_t PHASE_NAME = 4;
// SceneNameRegister
constexpr uint32_t F_DELEGATE = 0x288, F_EDIT_LAYER = 0x290, F_EDIT_LABEL = 0x294, F_EDITING = 0x298;

uint32_t label(uint32_t root, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(root, name, len);
    SceneUtil::setShadow(text);
    setTextId(text, id);
    return text;
}

// the "invalid name" dialog (OK only)
void nameError() {
    Frame f;
    const uint32_t td = textData(f, 109130006);
    const uint32_t s = newStr(f, textDataStr(td));
    const uint32_t none = newEmptyFunction(f);
    call(SceneUtil_openOkDialog4Line, {s, none, 0});
    functionDestroy(none);
    strFree(s);
    sharedRelease(td);
}
}  // namespace

namespace popup_register {

uint32_t openNameRegisterPopup(uint32_t done, uint32_t scene) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {0x20C});
    vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    vcall(layer, V_addChild, {layer, shadow});
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Json});
    vcall(root, V_setAnchorPoint, {root, ld32(GOT_ANCHOR_MIDDLE)});
    {
        // centred: the side margins plus the node's position and half its size
        const float posX = bitsf(vcall(root, V_getPositionX, {root}));
        const float w = ldf(vcall(root, V_getContentSize, {root}));
        const float margin = bitsf(call(SceneUtil_getSideMargin, {}));
        const float posY = bitsf(vcall(root, V_getPositionY, {root}));
        const float h = ldf(vcall(root, V_getContentSize, {root}) + 4);
        const float x = margin + (posX + w * 0.5f);
        const float y0 = posY + h * 0.5f;
        const float hm = bitsf(call(SceneUtil_getSideHeightMargin, {fbits(y0)}));
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(Vec2_ctor, {pos, fbits(x), fbits(hm + y0)});
        vcall(root, V_setPosition, {root, pos});
        call(Vec2_dtor, {pos});
    }
    vcall(layer, V_addChild, {layer, root});
    label(root, S_Wording1, 0xC, 109130020);
    label(root, S_TxtName, 8, 109130017);
    label(root, S_TxtOK, 6, 100100001);

    const uint32_t field = findNode(root, S_TextFieldName, 0xE);
    uint32_t maxLength;
    {
        Frame f;
        const uint32_t row = masterRow(f, fn::Misc_create, MISC_NAME_LENGTH);
        const uint32_t sb = ld32(row);
        const uint32_t out = f.alloc(4);
        call(SafeBuffer_read, {sb, out, ld32(sb + 0xC) + 4, 4, 4});
        maxLength = ld32(out);
        sharedRelease(row);
    }
    st8(scene + F_EDITING, 0);
    Frame f;
    uint32_t editLabel;
    {
        const uint32_t placeholder = f.alloc(12);
        vcall(field, V_getPlaceHolder, {placeholder, field});
        const uint32_t size = f.alloc(8);
        call(Size_ctor, {size, fbits(400.0f), fbits(87.0f)});
        editLabel = SceneUtil::textFieldToEditBox(scene + F_EDIT_LAYER, scene + F_EDIT_LABEL, field, placeholder,
                                                  S_Empty, S_Font, size, scene + F_DELEGATE, scene + F_EDITING, 0,
                                                  int32_t(maxLength));
        strFree(placeholder);
    }
    call(Label_setAlignment, {editLabel, 1, ld32(editLabel + LABEL_V_ALIGNMENT)});
    {
        Frame g;
        const uint32_t name = newStr(g, S_TxtLimit1, 0xA);
        const uint32_t td = textData(g, 100100037);
        const uint32_t value = newStr(g, stringBuffer(g, {textDataStr(td), maxLength}));
        call(SceneUtil_settingTitleLabelStr, {root, name, value});
        strFree(value);
        sharedRelease(td);
        strFree(name);
    }
    const uint32_t ok = findNode(root, S_ButtonOK, 9);

    // OK: {label, scene, OK, root, layer, shadow, done}
    const uint32_t doneCopy = f.alloc(20);
    call(function_vv_copy, {doneCopy, done});
    {
        Frame g;
        const uint32_t copy = g.alloc(20);
        functionCopy(copy, doneCopy);
        const uint32_t fo = g.alloc(20);
        const uint32_t obj = call(fn::operator_new, {0x40});
        if (obj) {
            st32(obj, VT_OkTouch);
            st32(obj + 8, editLabel);
            st32(obj + 0xC, scene);
            st32(obj + 0x10, ok);
            st32(obj + 0x18, root);
            st32(obj + 0x1C, layer);
            st32(obj + 0x20, shadow);
            call(function_vv_copy2, {obj + 0x28, copy});
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        functionDestroy(copy);
    }
    {
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        const uint32_t open = call(SceneUtil_createOpenPopupAnimation, {root, none});
        call(Node_runAction, {root, call(EaseSineOut_create, {open})});
        functionDestroy(none);
    }
    functionDestroy(doneCopy);
    return 0;
}
RW_FUNCTION(0x56549C, "PopupRegister::openNameRegisterPopup", popup_register::openNameRegisterPopup);

// $_10: OK - the name checked (not empty, not only spaces), the field closed, the name sent (phase 4)
uint32_t nameOk(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    const uint32_t editLabel = ld32(self + 8);
    const uint32_t name = vcall(editLabel, V_getString, {editLabel});
    if (strSize(name) == 0 || !call(SceneUtil_splitSpaceCheck, {name})) {
        nameError();
        return 0;
    }
    st8(ld32(self + 0xC) + F_EDITING, 1);
    vcall(ld32(self + 0x10), V_setTouchEnabled, {ld32(self + 0x10), 0});
    const uint32_t scene = ld32(self + 0xC), ok = ld32(self + 0x10);
    const uint32_t root = ld32(self + 0x18), layer = ld32(self + 0x1C), shadow = ld32(self + 0x20);
    Frame f;
    const uint32_t doneCopy = f.alloc(20);
    call(function_vv_copy, {doneCopy, self + 0x28});
    call(SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t text = strData(name);
    const uint32_t copy = f.alloc(20);
    functionCopy(copy, doneCopy);
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = call(fn::operator_new, {0x38});
    if (obj) {
        st32(obj, VT_Sent);
        st32(obj + 8, scene);
        st32(obj + 0xC, ok);
        st32(obj + 0x10, root);
        st32(obj + 0x14, layer);
        st32(obj + 0x18, shadow);
        call(function_vv_copy2, {obj + 0x20, copy});
    }
    st32(fo + 16, obj);
    call(APIManager_executeTutorialProgress, {api, PHASE_NAME, 0, 0, text, 0, 0, 0, fo});
    functionDestroy(fo);
    functionDestroy(copy);
    functionDestroy(doneCopy);
    return 0;
}
RW_FUNCTION(0x565E38, "PopupRegister::openNameRegisterPopup::$_10 operator() (OK)", popup_register::nameOk);

uint32_t sentClone(uint32_t self) {
    const uint32_t c = call(fn::operator_new, {0x38});
    if (c) {
        st32(c, VT_Sent);
        st32(c + 8, ld32(self + 8));
        st32(c + 0xC, ld32(self + 0xC));
        for (uint32_t k = 0x10; k < 0x1C; k += 4) st32(c + k, ld32(self + k));
        call(function_vv_copy, {c + 0x20, self + 0x20});
    }
    return c;
}
RW_FUNCTION(0x5662CC, "PopupRegister::openNameRegisterPopup::$_11 __clone()", popup_register::sentClone);

uint32_t sentCloneInto(uint32_t self, uint32_t p) {
    if (!p) return self;
    st32(p, VT_Sent);
    st32(p + 8, ld32(self + 8));
    st32(p + 0xC, ld32(self + 0xC));
    for (uint32_t k = 0x10; k < 0x1C; k += 4) st32(p + k, ld32(self + k));
    return call(function_vv_copy, {p + 0x20, self + 0x20});
}
RW_FUNCTION(0x566330, "PopupRegister::openNameRegisterPopup::$_11 __clone(p)", popup_register::sentCloneInto);

// $_11: the name sent - the popup closes and the done callback runs, or the error dialog and OK back on
uint32_t nameSent(uint32_t self, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        const uint32_t root = ld32(self + 0x10);
        {
            Frame f;
            const uint32_t fo = newFunction(f, VT_Closed, {ld32(self + 0x14), ld32(self + 0x18), root});
            call(Node_runAction, {root, call(SceneUtil_createClosePopupAnimation, {root, fo})});
            functionDestroy(fo);
        }
        const uint32_t shadow = ld32(self + 0x18);
        call(Node_runAction, {shadow, call(FadeTo_create, {ld32(ld32(GOT_CLOSE_FADE)), 0})});
        if (ld32(self + 0x30)) call(function_vv_call, {self + 0x20});
        return 0;
    }
    if (ld8(call(fn::APIManager_getInstance, {}) + API_RETRY_DIALOG)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_ErrorClosed, {});
        call(APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }
    if (!call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) {
        st8(ld32(self + 8) + F_EDITING, 0);
        vcall(ld32(self + 0xC), V_setTouchEnabled, {ld32(self + 0xC), 1});
    }
    return 0;
}
RW_FUNCTION(0x5663C4, "PopupRegister::openNameRegisterPopup::$_11 operator() (sent)", popup_register::nameSent);

}  // namespace popup_register
