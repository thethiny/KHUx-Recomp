// SceneNameRegister: the player name of a new account - the name popup (PopupRegister::openNameRegisterPopup) over
// a gradient background, then the avatar creation.
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t Layer_ctor = 0x88A7D1, Scene_create = 0x8A5F37, Ref_autorelease = 0x8FD0D5;
constexpr uint32_t function_vv_assign = 0x4EB9CD;
constexpr uint32_t SceneManager_hideHeader = 0x61154D, SceneManager_changeBackground = 0x6128ED;
constexpr uint32_t SceneManager_removeBackground = 0x611691;
constexpr uint32_t PopupRegister_openNameRegisterPopup = 0x56549D;   // (function<void()> by value, scene)
constexpr uint32_t SceneUtil_editBoxEditingDidEnd = 0x45C61D;        // (layerRef, label, text)
constexpr uint32_t EditBox_getText = 0x85520D;
constexpr uint32_t SceneNameRegister_create = 0x67F6AD;
constexpr uint32_t GOT_VT_SceneNameRegister = 0xFD6900, GOT_AVATAR_EDIT_FACTORY = 0xFD662C;
constexpr uint32_t VT_OnEnter = 0xF6B748, VT_OnExit = 0xF6B71C;   // {this}
constexpr uint32_t VT_Registered = 0xF6B6F0;               // {}
constexpr uint32_t S_Background = 0xE067A6;                 // "img/ui/Bg_Circle_Gradation.png"
constexpr uint32_t V_release = 0x04, V_init = 0x248, V_addChild = 0xF0;
constexpr uint32_t SIZE = 0x2A0;
// SceneNameRegister
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228, F_DELEGATE = 0x288;
constexpr uint32_t F_EDIT_LAYER = 0x290, F_EDIT_LABEL = 0x294, F_EDITING = 0x298;

void assign(uint32_t member, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(function_vv_assign, {member, fo});
    functionDestroy(fo);
}
}  // namespace

namespace scene_name_register {

uint32_t init(uint32_t self) {
    assign(self + F_ON_ENTER, VT_OnEnter, self);
    assign(self + F_ON_EXIT, VT_OnExit, self);
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SceneManager_hideHeader, {sm, none});
        functionDestroy(none);
    }
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t s = newStr(f, S_Background, 0x1E);
        call(SceneManager_changeBackground, {sm, 1, s});
        strFree(s);
    }
    Frame f;
    const uint32_t done = newFunction(f, VT_Registered, {});
    call(PopupRegister_openNameRegisterPopup, {done, self});
    functionDestroy(done);
    return 1;
}
RW_FUNCTION(0x716E40, "SceneNameRegister::init", scene_name_register::init);

uint32_t create(uint32_t) {
    const uint32_t self = call(fn::operator_new, {SIZE});
    call(Layer_ctor, {self});
    const uint32_t vt = ld32(GOT_VT_SceneNameRegister);
    st32(self, vt + 8);
    st32(self + F_DELEGATE, vt + 0x320);
    st32(self + F_EDIT_LAYER, 0);
    st32(self + F_EDIT_LABEL, 0);
    st8(self + F_EDITING, 0);
    if (vcall(self, V_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x67F6AC, "SceneNameRegister::create", scene_name_register::create);

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneNameRegister_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x716D4C, "SceneNameRegister::createScene", scene_name_register::createScene);

// the name edit box closed: the shared ending (the label shows the text)
uint32_t editBoxEditingDidEnd(uint32_t self, uint32_t editBox) {
    const uint32_t label = ld32(self + F_EDIT_LABEL);
    return call(SceneUtil_editBoxEditingDidEnd, {self + F_EDIT_LAYER, label, call(EditBox_getText, {editBox})});
}
RW_FUNCTION(0x716FFA, "SceneNameRegister::editBoxEditingDidEnd", scene_name_register::editBoxEditingDidEnd);

// the same through the EditBoxDelegate base (this + 0x288)
uint32_t editBoxEditingDidEndThunk(uint32_t delegate, uint32_t editBox) {
    const uint32_t label = ld32(delegate + (F_EDIT_LABEL - F_DELEGATE));
    return call(SceneUtil_editBoxEditingDidEnd,
                {delegate + (F_EDIT_LAYER - F_DELEGATE), label, call(EditBox_getText, {editBox})});
}
RW_FUNCTION(0x717022, "SceneNameRegister::editBoxEditingDidEnd (thunk)", scene_name_register::editBoxEditingDidEndThunk);

// the name registered: the background off, then the avatar creation (no fade)
uint32_t registered(uint32_t) {
    call(SceneManager_removeBackground, {sceneManager()});
    smain::changeSceneLayer(ld32(GOT_AVATAR_EDIT_FACTORY), 0);
    return 0;
}
RW_FUNCTION(0x7170A8, "SceneNameRegister::init popup done (to the avatar)", scene_name_register::registered);

}  // namespace scene_name_register
