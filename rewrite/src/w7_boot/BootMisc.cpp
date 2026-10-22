// Small pieces grouped with the boot batch: SceneMovie (createScene, the out animation, SceneManager's deferred
// layer change), FlashMovieView's count popup lambdas, LWFButton (an LWF movie used as a button: "char" plays "on"
// while pressed and "off" on release), the concentration-line shader's parameters and the particle system that
// kills particles outside its own rectangle.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Scene_create = 0x8A5F37, SceneMovie_create = 0x638CE1;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99, SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t function_vv_copy = 0x4683CD, function_vv_copy2 = 0x47F905;
constexpr uint32_t SceneUtil_createLeftOutAnimation = 0x45B735, EaseSineOut_create = 0x860D09;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t IScrollProtocol_setEnabledScroll = 0x4F73C9;
constexpr uint32_t StringBuffer_ctor = 0x49140D, Text_setString = 0x83A2DD, S_FmtCount = 0xDFF1AD;   // "%d%s"
constexpr uint32_t Node_ctor = 0x892531, aeabi_memclr = 0x40E7A0, LWFButton_init = 0x4F4B2D;
constexpr uint32_t Ref_autorelease = 0x8FD0D5, LWFNode_create = 0x7EDF31, Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5, EventListener_setSwallowTouches = 0x8FAC99;
constexpr uint32_t function_bte_swap = 0x483E4D, function_vte_swap = 0x4F3699;
constexpr uint32_t EventDispatcher_addWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t LWF_SearchMovieInstance = 0x7FE1B9, LWF_Movie_GotoAndPlay = 0x815D91, function_vv_call = 0x470E69;
constexpr uint32_t Node_getGLProgramState = 0x89360F, GLProgram_updateUniforms = 0x90A9F5;
constexpr uint32_t Size_copy = 0x8DF02D, GLProgram_getUniformLocationForName = 0x90BCA5;
constexpr uint32_t GLProgram_setUniform1f = 0x90BE81, GLProgram_setUniform2f = 0x90BEE1;
constexpr uint32_t S_time = 0xDFA257, S_fade = 0xDFA25C, S_resolution = 0xDFA261, S_basePos = 0xDFA2AF;
constexpr uint32_t ParticleSystem_update = 0x89FF29, Rect_ctor = 0x8DF18B, Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Vec2_dtor_ = 0x8E4C17;
constexpr uint32_t GOT_LWFButton_vtable = 0xFD657C;
constexpr uint32_t VT_OutDone = 0xF60A28;                  // runOutAnimation's lambda {function<void()>} (heap 0x20)
constexpr uint32_t VT_CountCanClose = 0xF59044, VT_CountClosed = 0xF59018;
constexpr uint32_t VT_PickCanClose = 0xF590F4, VT_PickClosed = 0xF590C8;
constexpr uint32_t VT_Pick2CanClose = 0xF591D0, VT_Pick2Closed = 0xF591A4;
constexpr uint32_t VT_TouchBegan = 0xF55A4C, VT_TouchEnded = 0xF55A20;
constexpr uint32_t V_addChild = 0xF0, V_setVisible = 0xB0, V_setTouchEnabled = 0x29C;
constexpr uint32_t V_getPositionX = 0x68, V_getPositionY = 0x70;
// LWFButton: +0x270 pressed, +0x274 the LWFNode (its LWF at +0x3E0), +0x278 the click callback (ptr +0x288)
constexpr uint32_t F_PRESSED = 0x270, F_NODE = 0x274, F_CLICK = 0x278, LWFNODE_LWF = 0x3E0;
constexpr uint32_t SM_LAYER_FACTORY = 0x370;

float f32(uint32_t bits) { float f; std::memcpy(&f, &bits, 4); return f; }
// a short std::string (inline: length * 2, the characters, NUL)
uint32_t shortStr(Frame &f, const char *text) {
    const uint32_t s = f.alloc(12);
    const uint32_t n = uint32_t(std::strlen(text));
    st8(s, 2 * n);
    for (uint32_t k = 0; k <= n; k++) st8(s + 1 + k, uint8_t(text[k]));
    return s;
}
void setVisible(uint32_t n, uint32_t on) { vcall(n, V_setVisible, {n, on}); }
uint32_t setTouch(uint32_t n, uint32_t on) { return vcall(n, V_setTouchEnabled, {n, on}); }
// the movie "char" of a button's LWF played from `label`
void playChar(uint32_t button, const char *label) {
    const uint32_t lwf = ld32(ld32(button + F_NODE) + LWFNODE_LWF);
    Frame f;
    const uint32_t name = shortStr(f, "char");
    const uint32_t movie = call(LWF_SearchMovieInstance, {lwf, name, 0, 0x68});
    const uint32_t l = shortStr(f, label);
    call(LWF_Movie_GotoAndPlay, {movie, l});
}
}  // namespace

namespace boot_misc {

// SceneManager's deferred layer change: the factory it kept at +0x370 used and emptied
uint32_t deferredChange(uint32_t captures) {
    const uint32_t sm = ld32(captures + 4);
    call(SceneManager_changeSceneLayer, {sm, sm + SM_LAYER_FACTORY, 1, 1, 1});
    functionDestroy(sm + SM_LAYER_FACTORY);
    st32(sm + SM_LAYER_FACTORY + 16, 0);
    return 0;
}
RW_FUNCTION(0x638C42, "SceneManager deferred changeSceneLayer lambda (sub_638C42)", boot_misc::deferredChange);

uint32_t movieCreateScene(uint32_t) {
    const uint32_t scene = call(Scene_create, {});
    const uint32_t layer = call(SceneMovie_create, {scene});
    vcall(scene, V_addChild, {scene, layer});
    return scene;
}
RW_FUNCTION(0x638CBC, "SceneMovie::createScene", boot_misc::movieCreateScene);

uint32_t movieRunOutAnimation(uint32_t self, uint32_t done) {
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vv_copy, {copy, done});
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = call(fn::operator_new, {0x20});
    if (obj) {
        st32(obj, VT_OutDone);
        call(function_vv_copy2, {obj + 8, copy});
    }
    st32(fo + 16, obj);
    const uint32_t out = call(SceneUtil_createLeftOutAnimation, {self, 0, 0, fo});
    call(fn::Node_runAction, {self, call(EaseSineOut_create, {out})});
    functionDestroy(fo);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x6390E8, "SceneMovie::runOutAnimation", boot_misc::movieRunOutAnimation);

// FlashMovieView's count popup {button, label, view a, view b, panel, ...}: "n / m" from the views' +0x650, the
// panel shown, a popup opened (closing hides it again)
uint32_t countTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    setVisible(ld32(self + 4), 0);
    const uint32_t label = ld32(self + 8);
    {
        Frame f;
        const uint32_t td = textData(f, 109140007);
        const uint32_t buf = f.alloc(0x404);
        call(StringBuffer_ctor, {buf, textDataStr(td), ld32(ld32(self + 12) + 0x650), ld32(ld32(self + 16) + 0x650)});
        const uint32_t s = newStr(f, buf);
        call(Text_setString, {label, s});
        strFree(s);
        sharedRelease(td);
    }
    setVisible(ld32(self + 20), 1);
    const uint32_t panel = ld32(self + 20), extra = ld32(self + 24), button = ld32(self + 4);
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t canClose = newFunction(f, VT_CountCanClose, {extra});
    const uint32_t closed = newFunction(f, VT_CountClosed, {panel, button});
    call(fn::SceneManager_openPopup, {sm, canClose, closed, 1});
    functionDestroy(closed);
    functionDestroy(canClose);
    return 0;
}
RW_FUNCTION(0x567124, "FlashMovieView count popup operator() (sub_567124)", boot_misc::countTouched);

uint32_t countClosed(uint32_t self, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    if (ok) return ok;
    call(AudioPlaySystemSECancel, {});
    setVisible(ld32(self + 4), 0);
    return vcall(ld32(self + 8), V_setVisible, {ld32(self + 8), 1});
}
RW_FUNCTION(0x56737A, "FlashMovieView count popup closed operator() (sub_56737A)", boot_misc::countClosed);

// the pick popup {panel, scroll, cover, b1, b2, b3, label}: n chosen - the popup closed, "n<unit>" shown
uint32_t pickChosenText(uint32_t self, uint32_t valueRef, int32_t unit) {
    const uint32_t value = ld32(valueRef);
    call(AudioPlaySystemSEDecide, {});
    setVisible(ld32(self + 4), 0);
    call(IScrollProtocol_setEnabledScroll, {ld32(self + 8), 0});
    setVisible(ld32(self + 12), 0);
    setTouch(ld32(self + 16), 1);
    setTouch(ld32(self + 20), 1);
    setTouch(ld32(self + 24), 1);
    call(SceneManager_closeCurrentPopup, {sceneManager(), 1});
    const uint32_t label = ld32(self + 28);
    Frame f;
    const uint32_t td = textData(f, unit);
    const uint32_t buf = f.alloc(0x404);
    call(StringBuffer_ctor, {buf, S_FmtCount, value, textDataStr(td)});
    const uint32_t s = newStr(f, buf);
    call(Text_setString, {label, s});
    strFree(s);
    sharedRelease(td);
    return 0;
}
uint32_t pickChosen(uint32_t self, uint32_t valueRef) { return pickChosenText(self, valueRef, 109140004); }
RW_FUNCTION(0x5674F4, "FlashMovieView pick popup chosen operator() (sub_5674F4)", boot_misc::pickChosen);
uint32_t pickChosen2(uint32_t self, uint32_t valueRef) { return pickChosenText(self, valueRef, 109140003); }
RW_FUNCTION(0x567B8C, "FlashMovieView second pick popup chosen operator() (sub_567B8C)", boot_misc::pickChosen2);

uint32_t pickOpen(uint32_t self, uint32_t typeRef, uint32_t vtCanClose, uint32_t vtClosed) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEPopUp, {});
    setVisible(ld32(self + 4), 1);
    call(IScrollProtocol_setEnabledScroll, {ld32(self + 8), 1});
    setVisible(ld32(self + 12), 1);
    setTouch(ld32(self + 16), 0);
    setTouch(ld32(self + 20), 0);
    setTouch(ld32(self + 24), 0);
    uint32_t w[7];
    for (uint32_t k = 0; k < 7; k++) w[k] = ld32(self + 4 + 4 * k);
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t canClose = newFunction(f, vtCanClose, {w[6]});
    const uint32_t closed = f.alloc(20);
    const uint32_t obj = call(fn::operator_new, {0x1C});
    if (obj) {
        st32(obj, vtClosed);
        for (uint32_t k = 0; k < 6; k++) st32(obj + 4 + 4 * k, w[k]);
    }
    st32(closed + 16, obj);
    call(fn::SceneManager_openPopup, {sm, canClose, closed, 1});
    functionDestroy(closed);
    functionDestroy(canClose);
    return 0;
}
uint32_t pickTouched(uint32_t self, uint32_t, uint32_t typeRef) {
    return pickOpen(self, typeRef, VT_PickCanClose, VT_PickClosed);
}
RW_FUNCTION(0x5676F8, "FlashMovieView pick popup operator() (sub_5676F8)", boot_misc::pickTouched);
uint32_t pickTouched2(uint32_t self, uint32_t, uint32_t typeRef) {
    return pickOpen(self, typeRef, VT_Pick2CanClose, VT_Pick2Closed);
}
RW_FUNCTION(0x567D90, "FlashMovieView second pick popup operator() (sub_567D90)", boot_misc::pickTouched2);

uint32_t pickClosed(uint32_t self, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    if (ok) return ok;
    call(AudioPlaySystemSECancel, {});
    setVisible(ld32(self + 4), 0);
    call(IScrollProtocol_setEnabledScroll, {ld32(self + 8), 0});
    setVisible(ld32(self + 12), 0);
    setTouch(ld32(self + 16), 1);
    setTouch(ld32(self + 20), 1);
    return setTouch(ld32(self + 24), 1);
}
RW_FUNCTION(0x56793E, "FlashMovieView pick popup closed operator() (sub_56793E)", boot_misc::pickClosed);
RW_FUNCTION(0x567FD6, "FlashMovieView second pick popup closed operator() (sub_567FD6)", boot_misc::pickClosed);

uint32_t lwfButtonCreate(uint32_t path) {
    const uint32_t self = call(fn::operator_new, {0x290});
    call(aeabi_memclr, {self, 0x290, 0});
    call(Node_ctor, {self});
    st32(self, ld32(GOT_LWFButton_vtable) + 8);
    st32(self + F_CLICK + 16, 0);
    call(LWFButton_init, {self, path});
    call(Ref_autorelease, {self});
    return self;
}
RW_FUNCTION(0x4F4AB0, "LWFButton::create", boot_misc::lwfButtonCreate);

uint32_t lwfButtonInit(uint32_t self, uint32_t path) {
    st8(self + F_PRESSED, 0);
    functionDestroy(self + F_CLICK);
    st32(self + F_CLICK + 16, 0);
    st32(self + F_NODE, call(LWFNode_create, {path, 0}));
    const uint32_t dispatcher = ld32(call(Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(EventListenerTouchOneByOne_create, {dispatcher});
    call(EventListener_setSwallowTouches, {listener, 1});
    {
        Frame f;
        const uint32_t fo = newFunction(f, VT_TouchBegan, {self});
        call(function_bte_swap, {fo, listener + 0x50});
        functionDestroy(fo);
    }
    {
        Frame f;
        const uint32_t fo = newFunction(f, VT_TouchEnded, {self});
        call(function_vte_swap, {fo, listener + 0x80});
        functionDestroy(fo);
    }
    call(EventDispatcher_addWithSceneGraphPriority, {dispatcher, listener, ld32(self + F_NODE)});
    vcall(self, V_addChild, {self, ld32(self + F_NODE)});
    return 1;
}
RW_FUNCTION(0x4F4B2C, "LWFButton::init", boot_misc::lwfButtonInit);

uint32_t lwfButtonEnded(uint32_t captures) {
    const uint32_t button = ld32(captures + 4);
    if (ld8(button + F_PRESSED)) {
        playChar(button, "off");
        if (ld32(button + F_CLICK + 16)) call(function_vv_call, {button + F_CLICK});
    }
    return 0;
}
RW_FUNCTION(0x4F4CE4, "LWFButton::init touch ended lambda (sub_4F4CE4)", boot_misc::lwfButtonEnded);

uint32_t lwfButtonBegan(uint32_t captures) {
    const uint32_t button = ld32(captures + 4);
    st8(button + F_PRESSED, 1);
    playChar(button, "on");
    return 1;
}
RW_FUNCTION(0x4F4E74, "LWFButton::init touch began lambda (sub_4F4E74)", boot_misc::lwfButtonBegan);

// the concentration lines: time, fade, the frame size and a fixed centre (-0.7, 0.7)
uint32_t setConcentrationShaderParam(uint32_t node, uint32_t time, uint32_t fade) {
    const uint32_t program = ld32(call(Node_getGLProgramState, {node}) + 0x70);
    call(GLProgram_updateUniforms, {program});
    const uint32_t view = ld32(call(Director_getInstance, {}) + 0x9C);
    Frame f;
    const uint32_t size = f.alloc(8);
    call(Size_copy, {size, vcall(view, 0x1C, {view})});
    call(GLProgram_setUniform1f, {program, call(GLProgram_getUniformLocationForName, {program, S_time}), time});
    call(GLProgram_setUniform1f, {program, call(GLProgram_getUniformLocationForName, {program, S_fade}), fade});
    call(GLProgram_setUniform2f,
         {program, call(GLProgram_getUniformLocationForName, {program, S_resolution}), ld32(size), ld32(size + 4)});
    call(GLProgram_setUniform2f,
         {program, call(GLProgram_getUniformLocationForName, {program, S_basePos}), fbits(-0.7f), fbits(0.7f)});
    return 0;
}
RW_FUNCTION(0x495170, "shader::set_concentration_shader_param", boot_misc::setConcentrationShaderParam);

// a ParticleSystemQuad that kills (size 0) every live particle outside the rectangle centred on its position plus
// the offset +0x438 / +0x43C, of size +0x440 / +0x444
uint32_t clippedParticlesUpdate(uint32_t self, uint32_t dt) {
    call(ParticleSystem_update, {self, dt});
    const float w = f32(ld32(self + 0x440)), h = f32(ld32(self + 0x444));
    const float x = (f32(vcall(self, V_getPositionX, {self})) + f32(ld32(self + 0x438))) - w * 0.5f;
    const float y = (f32(vcall(self, V_getPositionY, {self})) + f32(ld32(self + 0x43C))) - h * 0.5f;
    Frame f;
    const uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, fbits(x), fbits(y), ld32(self + 0x440), ld32(self + 0x444)});
    for (int32_t i = 0; i < int32_t(ld32(self + 0x2F0)); i++) {
        const uint32_t p = ld32(self + 0x2C4) + 104 * uint32_t(i) + 64;
        if (f32(ld32(p)) > 0.0f && !call(Rect_containsPoint, {rect, p - 0x40})) st32(p, 0);
    }
    call(Vec2_dtor_, {rect});
    return 0;
}
RW_FUNCTION(0x5ED990, "ParticleSystemQuad clipped update (sub_5ED990)", boot_misc::clippedParticlesUpdate);

}  // namespace boot_misc
