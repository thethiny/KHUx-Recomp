// The guilt-burst cut-in shader effect: shader:: helpers (GL programs from bg::vfs fragment shaders, uniforms)
// and GuiltBurstCutInShader (two render textures and the timed shader / tint animation).
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"

namespace w4b2 {
using namespace scene;

constexpr uint32_t PROGRAM_CACHE = rwlib::cocos2d::GLProgramCache::getInstance;        // GLProgramCache::getInstance
constexpr uint32_t GET_PROGRAM = rwlib::cocos2d::GLProgramCache::getGLProgram;          // GLProgramCache::getGLProgram(name)
constexpr uint32_t ADD_PROGRAM = rwlib::cocos2d::GLProgramCache::addGLProgram;          // GLProgramCache::addGLProgram(program, name)
constexpr uint32_t STATE_FOR_PROGRAM = rwlib::cocos2d::GLProgramState::getOrCreateWithGLProgram;    // GLProgramState::getOrCreateWithGLProgram
constexpr uint32_t SET_STATE = rwlib::cocos2d::Node::setGLProgramState;            // Node::setGLProgramState
constexpr uint32_t UNIFORM_LOC = rwlib::cocos2d::GLProgram::getUniformLocationForName;          // GLProgram::getUniformLocationForName
constexpr uint32_t UNIFORM_1F = rwlib::cocos2d::GLProgram::setUniformLocationWith1f, UNIFORM_2F = rwlib::cocos2d::GLProgram::setUniformLocationWith2f, UNIFORM_3F = rwlib::cocos2d::GLProgram::setUniformLocationWith3f;
constexpr uint32_t UPDATE_UNIFORMS = rwlib::cocos2d::GLProgram::updateUniforms;

// ============================================================================================ shader
struct Shader {
    static uint32_t cachedProgram(Frame &f, uint32_t nameVar) {
        uint32_t cache = call(PROGRAM_CACHE, {});
        uint32_t s = f.alloc(12);
        strInitC(s, ld32(nameVar));
        uint32_t p = call(GET_PROGRAM, {cache, s});
        strFree(s);
        return p;
    }
    // a new program from the shared vertex shader and `frag` (read from the vfs), cached under the name
    static void buildProgram(Frame &f, uint32_t node, uint32_t nameVar, uint32_t frag) {
        uint32_t p = call(fn::operator_new, {0x88});
        call(rwlib::cocos2d::GLProgram::ctor_base, {p});  // GLProgram::GLProgram
        uint32_t vs = ld32(ld32(0xFD6120));
        uint32_t data = f.alloc(8);
        call(0x8DD46D, {data, frag});  // bg::vfs::readData
        call(rwlib::cocos2d::GLProgram::initWithByteArrays, {p, vs, ld32(ld32(data) + 4)});  // initWithByteArrays
        if (ld32(data + 4) != 0) call(fn::shared_release, {ld32(data + 4)});
        call(rwlib::cocos2d::Ref::autorelease, {p});  // autorelease
        call(SET_STATE, {node, call(STATE_FOR_PROGRAM, {p})});
        call(rwlib::cocos2d::GLProgram::link, {p});           // link
        call(UPDATE_UNIFORMS, {p});
        uint32_t cache = call(PROGRAM_CACHE, {});
        uint32_t s = f.alloc(12);
        strInitC(s, ld32(nameVar));
        call(ADD_PROGRAM, {cache, p, s});
        strFree(s);
    }
    static uint32_t setGuiltShader(uint32_t node) {
        Frame f;
        uint32_t nameVar = ld32(0xFD611C);  // &shader::SHADER_GUILT
        uint32_t prog = cachedProgram(f, nameVar);
        uint32_t again = cachedProgram(f, ld32(0xFD611C));
        if (again != 0) call(SET_STATE, {node, call(STATE_FOR_PROGRAM, {prog})});
        else buildProgram(f, node, ld32(0xFD611C), 0xDFA22B);  // "shader/guilt.frag"
        return 0;
    }
    static uint32_t setConcentrationShader(uint32_t node) {
        Frame f;
        uint32_t prog = cachedProgram(f, ld32(0xFD6124));  // &shader::SHADER_CONCENT
        if (prog != 0) call(SET_STATE, {node, call(STATE_FOR_PROGRAM, {prog})});
        else buildProgram(f, node, ld32(0xFD6124), 0xDFA23D);  // "shader/concentration.frag"
        return 0;
    }

    static void set1f(uint32_t prog, uint32_t name, uint32_t v) {
        call(UNIFORM_1F, {prog, call(UNIFORM_LOC, {prog, name}), v});
    }
    static void set3f(uint32_t prog, uint32_t a, uint32_t b, uint32_t c) {
        uint32_t loc = call(UNIFORM_LOC, {prog, 0xDFA284});  // "bgColor"
        call(UNIFORM_3F, {prog, loc, a, b, c});
    }
    // time, fade, resolution, param7, param6 = 12, colorBase and the per-type colour / parameters
    static uint32_t setGuiltShaderParam(uint32_t node, float time, float fade, float param7, int32_t type,
                                        float param5) {
        Frame f;
        uint32_t prog = ld32(call(rwlib::cocos2d::Node::getGLProgramState, {node}) + 0x70);  // getGLProgramState()->getGLProgram()
        call(UPDATE_UNIFORMS, {prog});
        uint32_t view = ld32(call(fn::Director_getInstance, {}) + 0x9C);
        uint32_t size = f.alloc(8);
        call(rwlib::cocos2d::Size::ctor_base__RKS0_, {size, vcall(view, 0x1C, {view})});  // Size of the frame size
        set1f(prog, 0xDFA257, fbits(time));
        set1f(prog, 0xDFA25C, fbits(fade));
        call(UNIFORM_2F, {prog, call(UNIFORM_LOC, {prog, 0xDFA261}), ld32(size), ld32(size + 4)});  // resolution
        set1f(prog, 0xDFA26C, fbits(param7));
        set1f(prog, 0xDFA273, 0x41400000);  // param6 = 12
        uint32_t loc = call(UNIFORM_LOC, {prog, 0xDFA27A});  // colorBase
        float base = fade / bitsf(0x3F666666);
        base = base * 0.5f;
        base = base * bitsf(0x3FE66666);
        call(UNIFORM_1F, {prog, loc, fbits(base)});
        const uint32_t P1 = 0xDFA28C, P2 = 0xDFA293, P3 = 0xDFA29A, P4 = 0xDFA2A1, P5 = 0xDFA2A8;
        switch (type) {
        case 1:
            set3f(prog, 0x3E99999A, 0x3DCCCCCD, 0x3F333333);  // 0.3, 0.1, 0.7
            set1f(prog, P1, 0x40000000);
            set1f(prog, P2, 0x40400000);
            set1f(prog, P3, 0x3DCCCCCD);
            set1f(prog, P4, 0x3ECCCCCD);
            break;
        case 2:
            set3f(prog, 0x3F000000, 0x3DCCCCCD, 0x3F19999A);  // 0.5, 0.1, 0.6
            set1f(prog, P1, 0x40000000);
            set1f(prog, P2, 0x40A00000);
            set1f(prog, P3, 0x3DCCCCCD);
            set1f(prog, P4, 0x3ECCCCCD);
            break;
        case 3:
            set3f(prog, 0x3F4CCCCD, 0x3DCCCCCD, 0x3E99999A);  // 0.8, 0.1, 0.3
            set1f(prog, P1, 0x40400000);
            set1f(prog, P2, 0x40A00000);
            set1f(prog, P3, 0x3DCCCCCD);
            set1f(prog, P4, 0x3ECCCCCD);
            break;
        case 4:
            set3f(prog, 0x3F666666, 0x3ECCCCCD, 0x3DCCCCCD);  // 0.9, 0.4, 0.1
            set1f(prog, P1, 0x40800000);
            set1f(prog, P2, 0x40A00000);
            set1f(prog, P3, 0x3DCCCCCD);
            set1f(prog, P4, 0x3ECCCCCD);
            break;
        case 5:
            set3f(prog, 0x3F4CCCCD, 0x3E4CCCCD, 0x3DCCCCCD);  // 0.8, 0.2, 0.1
            set1f(prog, P1, 0x40A00000);
            set1f(prog, P2, 0x40A00000);
            set1f(prog, P3, 0x3E4CCCCD);
            set1f(prog, P4, 0x3F000000);
            break;
        default:
            return 0;
        }
        set1f(prog, P5, fbits(param5));
        return 0;
    }
};
RW_FUNCTION(0x4948EC, "shader::set_guilt_shader", Shader::setGuiltShader);
RW_FUNCTION(0x494B14, "shader::set_concentration_shader", Shader::setConcentrationShader);
RW_FUNCTION(0x494CC8, "shader::set_guilt_shader_param", Shader::setGuiltShaderParam);

// ============================================================================================ GuiltBurstCutInShader
// (a cocos2d::Node) render textures +0x270 (guilt) / +0x274 (concentration), stage manager +0x278, field
// manager +0x27C, time +0x288, fade +0x28C, concentration +0x290, param7 +0x294, tint +0x298, field tint +0x29C,
// param5 +0x2A0, state +0x2A4, timer +0x2A8, running +0x2AC, initialising +0x2AD, type +0x2B0, +0x2B4,
// tinted nodes vector +0x2B8
struct GuiltBurstCutInShader {
    static constexpr uint32_t SET_PARAM = 0x494CC9, SET_CONCENT_PARAM = 0x495171, TINT = 0x495ED9;
    static constexpr uint32_t COLOR3B = rwlib::cocos2d::Color3B::ctor_base;

    static uint32_t create() {
        uint32_t obj = call(fn::operator_new, {0x2C8});
        call(rwlib::cocos2d::Node::ctor_base, {obj});  // Node::Node
        uint32_t vt = ld32(0xFD6128);
        st32(obj, vt + 8);
        call(0x40E7A0, {obj + 0x270, 0x20, 0});  // __aeabi_memset
        stf(obj + 0x290, 1.0f);
        st32(obj + 0x294, 0x3C23D70A);  // 0.01
        stf(obj + 0x298, 1.0f);
        stf(obj + 0x29C, 1.0f);
        stf(obj + 0x2A0, 12.0f);
        st32(obj + 0x2A4, 0);
        st32(obj + 0x2A8, 0);
        st8(obj + 0x2AC, 0);
        st8(obj + 0x2AD, 1);
        st32(obj + 0x2B0, 1);
        st8(obj + 0x2B4, 0);
        st32(obj + 0x2B8, 0);
        st32(obj + 0x2BC, 0);
        st32(obj + 0x2C0, 0);
        if (call(ld32(vt + 0x250), {obj}) == 1) {  // init
            call(rwlib::cocos2d::Ref::autorelease, {obj});
            return obj;
        }
        vcall(obj, 4, {obj});
        return 0;
    }
    static void destroyDelete(uint32_t self) {
        st32(self, ld32(0xFD6128) + 8);
        call(0x495295, {self + 0x2B8});  // vector dtor
        call(rwlib::cocos2d::Node::dtor_base, {self});
        call(fn::operator_delete, {self});
    }
    static uint32_t makeTexture(Frame &f, uint32_t size) {
        int32_t w = f2i(ldf(size)), h = f2i(ldf(size + 4));
        uint32_t rt = call(rwlib::cocos2d::RenderTexture::create__ii, {uint32_t(w), uint32_t(h)});  // RenderTexture::create
        return rt;
    }
    static void setupTexture(Frame &f, uint32_t self, uint32_t field, uint32_t size) {
        uint32_t rt = ld32(self + field);
        vcall(rt, 0x274, {rt, 0, 0, 0, 0});  // beginWithClear(0, 0, 0, 0)
        rt = ld32(self + field);
        vcall(rt, 0x280, {rt});              // end
        rt = ld32(self + field);
        uint32_t setPosition = ld32(ld32(rt) + 0x4C);
        float x = ldf(size) * 0.5f;
        float y = ldf(size + 4) * 0.5f;
        uint32_t pos = f.alloc(8);
        call(rwlib::cocos2d::Vec2::ctor_base__ff, {pos, fbits(x), fbits(y)});
        call(setPosition, {rt, pos});
        call(rwlib::cocos2d::Vec2::dtor_base, {pos});
    }
    static uint32_t init(uint32_t self) {
        Frame f;
        if (call(rwlib::cocos2d::Node::init, {self}) != 1) return 0;  // Node::init
        vcall(self, 0x11C, {self});  // getParent (result unused)
        uint32_t view = ld32(call(fn::Director_getInstance, {}) + 0x9C);
        uint32_t size = f.alloc(8);
        call(rwlib::cocos2d::Size::ctor_base__RKS0_, {size, vcall(view, 0x34, {view})});  // Size of the design resolution
        st32(self + 0x270, makeTexture(f, size));
        setupTexture(f, self, 0x270, size);
        st32(self + 0x274, makeTexture(f, size));
        setupTexture(f, self, 0x274, size);
        st8(self + 0x2AC, 0);
        st8(self + 0x2AD, 1);
        st32(self + 0x2A4, 0);
        st32(self + 0x28C, 0x3A83126F);  // 0.001
        call(0x4948ED, {ld32(ld32(self + 0x270) + 0x2E4)});  // shader::set_guilt_shader(sprite)
        float t = ldf(self + 0x288) * 12.0f;
        call(SET_PARAM, {ld32(ld32(self + 0x270) + 0x2E4), fbits(t), ld32(self + 0x28C), ld32(self + 0x294),
                         ld32(self + 0x2B0), 0x41400000});
        call(0x494B15, {ld32(ld32(self + 0x274) + 0x2E4)});  // shader::set_concentration_shader
        call(SET_PARAM, {ld32(ld32(self + 0x274) + 0x2E4), ld32(self + 0x288), ld32(self + 0x28C),
                         ld32(self + 0x294), ld32(self + 0x2B0), 0x41400000});
        return 1;
    }
    static uint32_t addShaderParent(uint32_t self, uint32_t stage, uint32_t field) {
        if (vcall(self, 0x11C, {self}) == 0) return 0;
        uint32_t parent = vcall(self, 0x11C, {self});
        vcall(parent, 0xF4, {parent, ld32(self + 0x270), 4});  // addChild(guilt, 4)
        vcall(parent, 0xF4, {parent, ld32(self + 0x274), 3});  // addChild(concentration, 3)
        st8(self + 0x2AC, 0);
        st32(self + 0x27C, field);
        if (stage != 0) {
            st32(self + 0x278, stage);
            return 0x278;
        }
        st8(self + 0x2B4, 0);
        uint32_t endAddr = self + 0x2BC;
        uint32_t end = ld32(endAddr), begin = ld32(self + 0x2B8);
        if (end != begin) st32(endAddr, end + (~(end - 4 - begin) & ~3u));  // clear()
        return endAddr;
    }
    static uint32_t updateInit(uint32_t self, float) {
        uint32_t flag = self + 0x2AD;
        if (ld8(flag) == 0) return flag;
        int32_t st = rw::load<int32_t>(self + 0x2A4);
        if (st == 1) {
            st8(flag, 0);
            uint32_t rt = ld32(self + 0x274);
            vcall(rt, 0xB0, {rt, 0});
            rt = ld32(self + 0x270);
            return vcall(rt, 0xB0, {rt, 0});
        }
        if (st != 0) return flag;
        call(SET_PARAM, {ld32(ld32(self + 0x270) + 0x2E4), ld32(self + 0x288), ld32(self + 0x28C),
                         ld32(self + 0x294), ld32(self + 0x2B0), 0x41400000});
        call(SET_PARAM, {ld32(ld32(self + 0x274) + 0x2E4), ld32(self + 0x288), ld32(self + 0x290),
                         ld32(self + 0x294), 0, 0x41400000});
        st32(self + 0x2A4, 1);
        return 1;
    }

    // grey level for setColor from a 0..1 tint
    static uint32_t grey(Frame &f, float tint) {
        uint32_t g = f2u(tint * 255.0f) & 0xFF;
        uint32_t c = f.alloc(4);
        call(COLOR3B, {c, g, g, g});
        return c;
    }
    static void tintActors(Frame &f, uint32_t self, uint32_t vecCopy, uint32_t source) {
        uint32_t v = f.alloc(12);
        call(0x495E75, {v, source});  // vector copy
        for (uint32_t i = 0; i < uint32_t(int32_t(ld32(v + 4) - ld32(v)) >> 2); i++) {
            uint32_t actor = ld32(ld32(v) + i * 4);
            uint32_t node = ld32(actor + 0x288);
            if (node != 0) {
                uint32_t setColor = ld32(ld32(node) + 0x22C);
                call(setColor, {node, grey(f, ldf(self + 0x298))});
            }
            if (ld32(actor + 0x28C) != 0) call(TINT, {self, ld32(actor + 0x28C), ld32(self + 0x298)});
        }
        (void)vecCopy;
        call(0x4960B7, {v});  // vector dtor
    }

    static uint32_t update(uint32_t self, float dt) {
        Frame f;
        if (ld8(self + 0x2AC) == 0) return 0;
        auto addTimer = [&]() {
            float t = ldf(self + 0x2A8) + dt;
            stf(self + 0x2A8, t);
            return t;
        };
        auto setVisible = [&](uint32_t field, uint32_t v) {
            uint32_t rt = ld32(self + field);
            vcall(rt, 0xB0, {rt, v});
        };
        switch (rw::load<int32_t>(self + 0x2A4)) {
        case 2: {
            float t = addTimer();
            if (t >= bitsf(0x3E6147AE)) {  // 0.22
                st32(self + 0x2A8, 0);
                st32(self + 0x2A4, 3);
                setVisible(0x270, 1);
                stf(self + 0x2A0, 12.0f);
            }
            break;
        }
        case 3: {
            float t = addTimer();
            float r = t / bitsf(0x3E883127);  // 0.266
            stf(self + 0x294, r * 0.2f);
            stf(self + 0x28C, r * 0.8f);
            stf(self + 0x298, r * -0.5f + 1.0f);
            stf(self + 0x29C, r * bitsf(0xBE99999A) + 1.0f);
            if (t >= bitsf(0x3E883127)) {
                st32(self + 0x28C, 0x3F4CCCCD);
                st32(self + 0x2A4, 4);
                st32(self + 0x294, 0x3E4CCCCD);
                st32(self + 0x2A8, 0);
                stf(self + 0x298, 0.5f);
                st32(self + 0x29C, 0x3F333333);
                stf(self + 0x2A0, 12.0f);
            }
            break;
        }
        case 4: {
            float t = addTimer();
            if (t >= bitsf(0x3F1EB852)) {  // 0.62
                st32(self + 0x2A8, 0);
                st32(self + 0x2A4, 5);
                stf(self + 0x298, 0.5f);
                st32(self + 0x29C, 0x3F333333);
            }
            break;
        }
        case 5: {
            float t = addTimer();
            float q = t / bitsf(0x3E99999A);  // 0.3
            stf(self + 0x294, bitsf(0x3E4CCCCD) - q * bitsf(0x3E4CCCCD));
            stf(self + 0x28C, bitsf(0x3F4CCCCD) - q * bitsf(0x3F4CCCCD));
            if (t >= bitsf(0x3E99999A)) {
                st32(self + 0x28C, 0);
                st32(self + 0x2A4, 7);
                st32(self + 0x294, 0x3C23D70A);
                st32(self + 0x2A8, 0);
                setVisible(0x270, 0);
                stf(self + 0x298, 0.5f);
                st32(self + 0x29C, 0x3F333333);
            }
            break;
        }
        case 8: {
            float t = addTimer();
            float q = t / bitsf(0x3ECCCCCD);  // 0.4
            stf(self + 0x294, bitsf(0x3E4CCCCD) - q * bitsf(0x3E4CCCCD));
            stf(self + 0x28C, bitsf(0x3F4CCCCD) - q * bitsf(0x3F4CCCCD));
            if (t >= bitsf(0x3ECCCCCD)) {
                setVisible(0x270, 0);
                st32(self + 0x2A4, 9);
                st32(self + 0x294, 0x3C23D70A);
                st32(self + 0x2A8, 0);
                st32(self + 0x290, 0);
                stf(self + 0x298, 0.5f);
                st32(self + 0x29C, 0x3F333333);
                setVisible(0x274, 1);
            }
            break;
        }
        case 9:
            setVisible(0x274, 1);
            break;
        case 10: {
            float t = addTimer();
            float q = t / bitsf(0x3E4CCCCD);  // 0.2
            stf(self + 0x290, 1.0f - q);
            stf(self + 0x298, q * 0.5f + 0.5f);
            stf(self + 0x29C, q * bitsf(0x3E99999A) + bitsf(0x3F333333));
            if (t >= bitsf(0x3E4CCCCD)) {
                st32(self + 0x290, 0);
                st32(self + 0x2A8, 0);
                stf(self + 0x298, 1.0f);
                stf(self + 0x29C, 1.0f);
                st32(self + 0x2A4, 11);
                setVisible(0x274, 0);
                st8(self + 0x2AC, 0);
            }
            break;
        }
        default:
            break;
        }
        float tm = ldf(self + 0x288) + dt;
        if (tm >= 360.0f) tm = tm + -360.0f;
        stf(self + 0x288, tm);
        uint32_t rt = ld32(self + 0x270);
        if (vcall(rt, 0xB4, {rt}) == 1) {  // isVisible
            float v = ldf(self + 0x288) * 12.0f;
            call(SET_PARAM, {ld32(ld32(self + 0x270) + 0x2E4), fbits(v), ld32(self + 0x28C), ld32(self + 0x294),
                             ld32(self + 0x2B0), ld32(self + 0x2A0)});
        }
        rt = ld32(self + 0x274);
        if (vcall(rt, 0xB4, {rt}) == 1) {
            float v = ldf(self + 0x288) * 10.0f;
            call(SET_CONCENT_PARAM, {ld32(ld32(self + 0x274) + 0x2E4), fbits(v), ld32(self + 0x290)});
        }
        uint32_t stage = ld32(self + 0x278);
        if (stage != 0) {
            for (uint32_t n = ld32(stage + 0x118); n != 0; n = ld32(n)) {  // the stage's field objects
                uint32_t obj = ld32(ld32(ld32(n + 0xC) + 0x28C) + 0x274);
                uint32_t setColor = ld32(ld32(obj) + 0x22C);
                call(setColor, {obj, grey(f, ldf(self + 0x298))});
            }
            tintActors(f, self, 0, ld32(self + 0x278) + 0x124);  // players
            tintActors(f, self, 0, ld32(self + 0x278) + 0x130);  // enemies
            uint32_t field = ld32(self + 0x27C);
            if (field != 0) {
                uint32_t v = f.alloc(12);
                call(0x496039, {v, field + 0x6C});  // vector copy
                if (int32_t(ld32(v + 4) - ld32(v)) >= 1) {
                    for (int32_t i = 0; i < int32_t(ld32(v + 4) - ld32(v)) >> 2; i++) {
                        uint32_t e = ld32(ld32(ld32(v) + uint32_t(i) * 4) + 0x2B0);
                        if (e != 0) call(TINT, {self, ld32(e + 0x29C), ld32(self + 0x29C)});
                    }
                }
                call(0x49608B, {v});
            }
        } else {
            uint32_t begin = ld32(self + 0x2B8);
            if (ld32(self + 0x2BC) != begin) {
                for (uint32_t i = 0; i < uint32_t(int32_t(ld32(self + 0x2BC) - ld32(self + 0x2B8)) >> 2); i++)
                    call(TINT, {self, ld32(ld32(self + 0x2B8) + i * 4), ld32(self + 0x298)});
            }
        }
        return 0;
    }
};
RW_FUNCTION(0x5FFC50, "GuiltBurstCutInShader::create", GuiltBurstCutInShader::create);
RW_FUNCTION(0x4952C0, "GuiltBurstCutInShader::~GuiltBurstCutInShader (deleting)", GuiltBurstCutInShader::destroyDelete);
RW_FUNCTION(0x4952F8, "GuiltBurstCutInShader::init", GuiltBurstCutInShader::init);
RW_FUNCTION(0x495560, "GuiltBurstCutInShader::addShaderParent", GuiltBurstCutInShader::addShaderParent);
RW_FUNCTION(0x4960E0, "GuiltBurstCutInShader::updateInit", GuiltBurstCutInShader::updateInit);
RW_FUNCTION(0x495604, "GuiltBurstCutInShader::update", GuiltBurstCutInShader::update);

}  // namespace w4b2
