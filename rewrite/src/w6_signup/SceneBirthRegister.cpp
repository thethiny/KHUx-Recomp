// SceneBirthRegister: the birthday input of a new account (misc 907 off skips it), then the invitation code (when
// the login data allows one) or the tutorial download.
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base, Scene_create = rwlib::cocos2d::Scene::create, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t function_vv_assign = 0x4EB9CD;
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B, SceneManager_hideHeader = 0x61154D;
constexpr uint32_t SceneManager_changeBackground = 0x6128ED, SceneManager_removeBackground = 0x611691;
constexpr uint32_t PopupRegister_openBirthRegisterPopup = 0x563345;   // (function<void()> by value)
constexpr uint32_t MapUintUint_at = 0x5681A5;               // unordered_map<unsigned, unsigned>::at
constexpr uint32_t SceneBirthRegister_create = 0x711EF1, SceneBirthRegister_init = 0x711F99;
constexpr uint32_t SceneBirthRegister_next = 0x711DB5;
constexpr uint32_t GOT_BIRTH_FACTORY = 0xFD696C, GOT_INVITATION_FACTORY = 0xFD6970;
constexpr uint32_t GOT_TUTORIAL_DOWNLOAD_FACTORY = 0xFD6974, GOT_BIRTH_VTABLE = 0xFD6978;
constexpr uint32_t VT_OnEnter = 0xF6B118, VT_OnExit = 0xF6B0EC, VT_BackKey = 0xF6B0C0;   // {this}
constexpr uint32_t VT_Registered = 0xF6B094;               // the popup's done callback {}
constexpr uint32_t S_Background = 0xE03AF2;                 // "img/ui/Equip_DeckBG_01.png"
constexpr uint32_t V_release = 0x04, V_addChild = 0xF0;
constexpr uint32_t SIZE = 0x290, F_ON_ENTER = 0x210, F_ON_EXIT = 0x228;
// APIManager: the misc values (unordered_map<id, value>) and the user data (+0x44, flags word +0x34: bit 0 =
// an invitation code may be entered)
constexpr uint32_t API_MISC = 0x1B0, API_USER = 0x44, USER_FLAGS = 0x34, FLAG_INVITATION = 1;
constexpr uint32_t MISC_BIRTH_REGISTER = 907;

void assign(uint32_t member, uint32_t vt, uint32_t self) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {self});
    call(function_vv_assign, {member, fo});
    functionDestroy(fo);
}
}  // namespace

namespace scene_birth_register {

// after the birthday: the invitation code screen when the user data allows one, else the tutorial download
uint32_t next() {
    call(fn::APIManager_getInstance, {});
    const uint32_t user = ld32(call(fn::APIManager_getInstance, {}) + API_USER);
    const bool invitation = user && (ld32(user + USER_FLAGS) & FLAG_INVITATION);
    smain::changeSceneLayer(ld32(invitation ? GOT_INVITATION_FACTORY : GOT_TUTORIAL_DOWNLOAD_FACTORY));
    return 0;
}
RW_FUNCTION(0x711DB4, "SceneBirthRegister: next scene (invitation / tutorial download)", scene_birth_register::next);

// the birthday screen when misc 907 asks for it, else straight on
uint32_t changeSceneLayer(uint32_t) {
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, MISC_BIRTH_REGISTER);
    if (ld32(call(MapUintUint_at, {ld32(call(fn::APIManager_getInstance, {}) + API_MISC), key}))) {
        call(SceneBirthRegister_next, {});
    } else {
        smain::changeSceneLayer(ld32(GOT_BIRTH_FACTORY));
    }
    return 0;
}
RW_FUNCTION(0x711CD4, "SceneBirthRegister::changeSceneLayer", scene_birth_register::changeSceneLayer);

uint32_t init(uint32_t self) {
    assign(self + F_ON_ENTER, VT_OnEnter, self);
    assign(self + F_ON_EXIT, VT_OnExit, self);
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t fo = newFunction(f, VT_BackKey, {self});
        call(SceneManager_setOnBackKeyCallback, {sm, fo});
        functionDestroy(fo);
    }
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
        const uint32_t s = newStr(f, S_Background, 0x1A);
        call(SceneManager_changeBackground, {sm, 0, s});
        strFree(s);
    }
    {
        Frame f;
        const uint32_t done = newFunction(f, VT_Registered, {});
        call(PopupRegister_openBirthRegisterPopup, {done});
        functionDestroy(done);
    }
    return 1;
}
RW_FUNCTION(0x711F98, "SceneBirthRegister::init", scene_birth_register::init);

uint32_t create(uint32_t) {
    const uint32_t self = call(fn::operator_new, {SIZE});
    call(Layer_ctor, {self});
    st32(self, ld32(GOT_BIRTH_VTABLE) + 8);
    if (call(SceneBirthRegister_init, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_release, {self});
    return 0;
}
RW_FUNCTION(0x711EF0, "SceneBirthRegister::create", scene_birth_register::create);

uint32_t createScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneBirthRegister_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x711ECC, "SceneBirthRegister::createScene", scene_birth_register::createScene);

// the popup's done callback: the background off, then the next scene
uint32_t registered(uint32_t) {
    call(SceneManager_removeBackground, {sceneManager()});
    return call(SceneBirthRegister_next, {});
}
RW_FUNCTION(0x7121FA, "SceneBirthRegister::init popup done (sub_7121FA)", scene_birth_register::registered);

}  // namespace scene_birth_register
