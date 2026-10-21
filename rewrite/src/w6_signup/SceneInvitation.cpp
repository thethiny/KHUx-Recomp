// SceneInvitation: the invitation code input of a new account (optional). The code field (a TextField turned into
// an edit box) and OK, which sends the code with the tutorial progress (phase 2) and goes on to the tutorial
// download.
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t Layer_init = 0x88A8A1, Layer_ctor = 0x88A7D1, Scene_create = 0x8A5F37, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t function_vv_assign = 0x4EB9CD;
constexpr uint32_t Size_ctor = 0x8DF027;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_executeTutorialProgress = 0x4C581D, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t EditBox_getText = 0x85520D;
constexpr uint32_t string_compare_cstr = 0x47CBD5, string_assign = 0x48D811;   // string::compare(char*), assign(p, n)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneInvitation_create = 0x67F619;
constexpr uint32_t GOT_VT_SceneInvitation = 0xFD68FC, GOT_TXTFIELD_NAME = 0xFD6984;   // &TXTFIELD_NAME (a const char*)
constexpr uint32_t GOT_TUTORIAL_DOWNLOAD_FACTORY = 0xFD6974;
constexpr uint32_t VT_OnEnter = 0xF6B6C4, VT_OnExit = 0xF6B698;   // {this}
constexpr uint32_t VT_OkTouch = 0xF6B614;                  // $_2 {this, OK button, code label}
constexpr uint32_t VT_Sent = 0xF6B640;                     // its API callback {this, OK button}
constexpr uint32_t VT_ErrorClosed = 0xF6B66C;              // {}
constexpr uint32_t S_Json = 0xE06B44, S_Wording1 = 0xDFF14C, S_Wording2 = 0xDF863B, S_TxtName = 0xDFDA10;
constexpr uint32_t S_ButtonOK = 0xDF8688, S_TxtOK = 0xDF869F, S_Font = 0xDF9F4D, S_Empty = 0xDF8A1A;
constexpr uint32_t V_release = 0x04, V_init = 0x248, V_setPositionX = 0x64, V_setVisible = 0xB0, V_addChild = 0xF0;
constexpr uint32_t V_getName = 0x150, V_getPlaceHolder = 0x150, V_setTouchEnabled = 0x29C, V_getString = 0x2AC;
constexpr uint32_t SIZE = 0x2A0;
// SceneInvitation
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228, F_DELEGATE = 0x288, F_ROOT = 0x28C;
constexpr uint32_t F_EDIT_LAYER = 0x290, F_EDIT_LABEL = 0x294, F_EDITING = 0x298, F_CODE_TEXT = 0x29C;
constexpr uint32_t F_CODE = 0x194;                         // std::string: the code typed
constexpr uint32_t API_RETRY_DIALOG = 0x14;
constexpr uint32_t PHASE_INVITATION = 2;

void assign(uint32_t member, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(function_vv_assign, {member, fo});
    functionDestroy(fo);
}
uint32_t shadowLabel(uint32_t parent, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(parent, name, len);
    SceneUtil::setShadow(text);
    setTextId(text, id);
    return text;
}
}  // namespace

namespace scene_invitation {

uint32_t init(uint32_t self) {
    if (call(Layer_init, {self}) != 1) return 0;
    assign(self + F_ON_ENTER, VT_OnEnter, self);
    assign(self + F_ON_EXIT, VT_OnExit, self);
    const uint32_t root = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Json});
    st32(self + F_ROOT, root);
    vcall(root, V_setPositionX, {root, call(SceneUtil_getSideMargin, {})});
    vcall(self, V_addChild, {self, ld32(self + F_ROOT)});
    shadowLabel(ld32(self + F_ROOT), S_Wording1, 0xC, 109130005);
    shadowLabel(ld32(self + F_ROOT), S_Wording2, 0xC, 109130002);
    shadowLabel(ld32(self + F_ROOT), S_TxtName, 8, 109130019);

    // the code field: a plain text showing the code, and the edit box over the field
    const uint32_t fieldName = ld32(ld32(GOT_TXTFIELD_NAME));
    const uint32_t field = findNode(ld32(self + F_ROOT), fieldName, host_strlen(fieldName));
    st32(self + F_CODE_TEXT, SceneUtil::textFieldToUiText(field, S_Empty, S_Font));
    st8(self + F_EDITING, 0);
    Frame f;
    const uint32_t placeholder = f.alloc(12);
    vcall(field, V_getPlaceHolder, {placeholder, field});
    const uint32_t size = f.alloc(8);
    call(Size_ctor, {size, fbits(400.0f), fbits(87.0f)});
    const uint32_t label = SceneUtil::textFieldToEditBox(self + F_EDIT_LAYER, self + F_EDIT_LABEL, field, placeholder,
                                                         S_Empty, S_Font, size, self + F_DELEGATE, self + F_EDITING,
                                                         0, 0);
    strFree(placeholder);
    vcall(label, V_setVisible, {label, 0});

    const uint32_t ok = findNode(ld32(self + F_ROOT), S_ButtonOK, 9);
    addTouch(ok, VT_OkTouch, {self, ok, label});
    shadowLabel(ok, S_TxtOK, 6, 100100001);
    return 1;
}
RW_FUNCTION(0x715F88, "SceneInvitation::init", scene_invitation::init);

uint32_t create(uint32_t) {
    const uint32_t self = call(fn::operator_new, {SIZE});
    call(Layer_ctor, {self});
    const uint32_t vt = ld32(GOT_VT_SceneInvitation);
    st32(self, vt + 8);
    st32(self + F_DELEGATE, vt + 0x320);
    st32(self + F_EDIT_LAYER, 0);
    st32(self + F_EDIT_LABEL, 0);
    st8(self + F_EDITING, 0);
    st32(self + F_CODE_TEXT, 0);
    if (vcall(self, V_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x67F618, "SceneInvitation::create", scene_invitation::create);

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneInvitation_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x715E94, "SceneInvitation::createScene", scene_invitation::createScene);

// the code field's edit box closed: the code kept and shown, then the shared edit-box ending
uint32_t editBoxEditingDidEnd(uint32_t self, uint32_t editBox) {
    Frame f;
    const uint32_t name = f.alloc(12);
    vcall(editBox, V_getName, {name, editBox});
    if (call(string_compare_cstr, {name, ld32(ld32(GOT_TXTFIELD_NAME))}) == 0) {
        const uint32_t text = call(EditBox_getText, {editBox});
        call(string_assign, {self + F_CODE, text, host_strlen(text)});
        const uint32_t shown = ld32(self + F_CODE_TEXT);
        if (shown) setTextC(shown, strData(self + F_CODE));
    }
    const uint32_t label = ld32(self + F_EDIT_LABEL);
    SceneUtil::editBoxEditingDidEnd(self + F_EDIT_LAYER, label, call(EditBox_getText, {editBox}));
    strFree(name);
    return 0;
}
RW_FUNCTION(0x716610, "SceneInvitation::editBoxEditingDidEnd", scene_invitation::editBoxEditingDidEnd);

// $_2: OK - the code (or none) sent with the tutorial progress
uint32_t okTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    const uint32_t self = ld32(functor + 4);
    call(AudioPlaySystemSEDecide, {});
    st8(self + F_EDITING, 0);
    vcall(ld32(functor + 8), V_setTouchEnabled, {ld32(functor + 8), 0});
    call(SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t ok = ld32(functor + 8);
    const uint32_t label = ld32(functor + 0xC);
    const uint32_t code = strData(vcall(label, V_getString, {label}));
    const uint8_t first = ld8(code);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t done = newFunction(f, VT_Sent, {self, ok});
    call(APIManager_executeTutorialProgress, {api, PHASE_INVITATION, 0, 0, 0, first ? code : 0, 0, 0, done});
    functionDestroy(done);
    return 0;
}
RW_FUNCTION(0x7167FC, "SceneInvitation::init::$_2 operator() (OK)", scene_invitation::okTouched);

// the code sent: on to the tutorial download, or the error dialog and OK back on (unless the request retries)
uint32_t codeSent(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    const uint32_t self = ld32(functor + 4);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        smain::changeSceneLayer(ld32(GOT_TUTORIAL_DOWNLOAD_FACTORY));
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
        st8(self + F_EDITING, 0);
        vcall(ld32(functor + 8), V_setTouchEnabled, {ld32(functor + 8), 1});
    }
    return 0;
}
RW_FUNCTION(0x716A08, "SceneInvitation::init::$_2 sent operator()", scene_invitation::codeSent);

}  // namespace scene_invitation
