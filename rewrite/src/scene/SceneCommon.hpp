// Helpers shared by the scene rewrites (SceneUtil, SceneManager): guest stack temporaries, the libc++ objects the
// scene code builds on the stack (std::string, std::function, std::shared_ptr), cocos2d virtual slots and the
// addresses of the library functions the scene code calls.
#pragma once

#include <climits>
#include <cmath>

#include "rw/rw.hpp"

namespace scene {

// ------------------------------------------------------------------------------------------------ values
// softfp: a float travels in a core register as its bit pattern
inline uint32_t fbits(float f) { uint32_t u; std::memcpy(&u, &f, 4); return u; }
inline float bitsf(uint32_t u) { float f; std::memcpy(&f, &u, 4); return f; }

// float -> integer like the guest's soft-float helpers (libgcc __fixsfsi / __fixunssfsi): truncate, saturate out
// of range, NaN -> 0
inline int32_t f2i(float f) {
    if (f != f) return 0;
    if (f >= 2147483648.0f) return INT32_MAX;
    if (f < -2147483648.0f) return INT32_MIN;
    return int32_t(f);
}
inline uint32_t f2u(float f) {
    if (f != f) return 0;
    if (f < 1.0f) return 0;
    if (f >= 4294967296.0f) return 0xFFFFFFFFu;
    return uint32_t(f);
}

inline uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
inline uint8_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }
inline float ldf(uint32_t a) { return rw::load<float>(a); }
inline void st32(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }
inline void st8(uint32_t a, uint8_t v) { rw::save<uint8_t>(a, v); }
inline void stf(uint32_t a, float v) { rw::save<float>(a, v); }

inline uint32_t host_strlen(uint32_t s) { return uint32_t(std::strlen(rw::host_ptr<const char>(s))); }

// ------------------------------------------------------------------------------------------------ guest stack
// Temporaries passed to guest functions by address (strings, Vec2, std::function...) live on the guest stack,
// below the caller's sp, like the original's locals. Allocations are released when the Frame goes away.
class Frame {
public:
    Frame() : saved_(rw::cpu->r[13]) {}
    ~Frame() { rw::cpu->r[13] = saved_; }
    Frame(const Frame &) = delete;
    Frame &operator=(const Frame &) = delete;
    // n bytes, 8-aligned (contents are whatever was on the stack)
    uint32_t alloc(uint32_t n) {
        uint32_t sp = (rw::cpu->r[13] - n) & ~7u;
        rw::cpu->r[13] = sp;
        return sp;
    }
    // exactly n bytes below the current sp: the original function's whole frame, when the rewrite must keep its
    // locals (and the frames of its callees) at the original addresses - the original reads stale stack bytes
    uint32_t allocExact(uint32_t n) {
        rw::cpu->r[13] -= n;
        return rw::cpu->r[13];
    }

private:
    uint32_t saved_;
};

// ------------------------------------------------------------------------------------------------ guest calls
using rw::call;

// virtual call through a byte offset into the vtable (the offsets as the code uses them)
inline uint32_t vcall(uint32_t obj, uint32_t offset, std::initializer_list<uint32_t> args) {
    return rw::call(ld32(ld32(obj) + offset), args);
}

// Guest functions (Thumb bit set where the lifted calls use it).
namespace fn {
// libc++ / runtime
constexpr uint32_t operator_new = 0xC98EF1;          // operator new(size_t)
constexpr uint32_t operator_delete = 0xC98F9B;       // operator delete(void*)
constexpr uint32_t string_init = 0x47CD93;           // std::string::__init(const char*, size_t)
constexpr uint32_t string_append = 0x47AC89;         // std::string::append(const char*, size_t)
constexpr uint32_t string_assign = 0x48D811;         // std::string::assign(const char*, size_t)
constexpr uint32_t string_substr_init = 0x47CD4D;    // std::string(const string&, pos, n)
constexpr uint32_t string_reserve = 0x48D705;        // std::string::reserve(size_t)
constexpr uint32_t shared_add = 0xC8DBB5;            // __shared_count::__add_shared
constexpr uint32_t shared_release = 0xC8DBD1;        // __shared_weak_count::__release_shared
constexpr uint32_t function_vv_copy = 0x4683CD;      // std::function<void()>::function(const function&)
constexpr uint32_t function_vv_move = 0x47F905;      // std::function<void()>::function(function&&)
constexpr uint32_t function_vv_call = 0x470E69;      // std::function<void()>::operator()
constexpr uint32_t function_vb_copy = 0x45C84D;      // std::function<void(bool)>::function(const function&)
constexpr uint32_t function_vb_move = 0x47BC69;      // std::function<void(bool)>::function(function&&)
constexpr uint32_t function_vbb_move = 0x48B1E9;     // std::function<void(bool,bool)>::function(function&&)
// cocos2d
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;    // (Size* out, Director*)
constexpr uint32_t Director_getVisibleOrigin = rwlib::cocos2d::Director::getVisibleOrigin;  // (Vec2* out, Director*)
constexpr uint32_t Vec2_ctor_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t MoveTo_create = rwlib::cocos2d::MoveTo::create;              // (float duration, const Vec2&)
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;              // (float duration, uint8 opacity)
constexpr uint32_t Spawn_create = rwlib::cocos2d::Spawn::create;               // (FiniteTimeAction*, ..., nullptr)
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;            // (FiniteTimeAction*, ..., nullptr)
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;           // (float)
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;            // (const std::function<void()>&)
constexpr uint32_t ScaleTo_create = rwlib::cocos2d::ScaleTo::create__ff;             // (float duration, float scale)
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;             // ui::Text::setString(const std::string&)
constexpr uint32_t TextData_get = 0x4928AD;               // TextData::get(shared_ptr out, Type, int id)
constexpr uint32_t AppGetGrameFrameHeight = 0x799D21;
}  // namespace fn

// cocos2d::Node virtual slots (byte offsets into the vtable)
namespace node {
constexpr uint32_t setLocalZOrder = 0x0C;
constexpr uint32_t setScaleX = 0x28, getScaleX = 0x2C, setScaleY = 0x30, getScaleY = 0x34;
constexpr uint32_t setScale = 0x40, getScale = 0x44;
constexpr uint32_t setPosition = 0x4C, getPosition = 0x54, setPositionXY = 0x5C;
constexpr uint32_t setPositionX = 0x64, getPositionX = 0x68, setPositionY = 0x6C, getPositionY = 0x70;
constexpr uint32_t setAnchorPoint = 0x9C, getAnchorPoint = 0xA0;
constexpr uint32_t setContentSize = 0xA8, getContentSize = 0xAC;
constexpr uint32_t setVisible = 0xB0, isVisible = 0xB4;
constexpr uint32_t setRotation = 0xB8;
constexpr uint32_t addChild = 0xF0, addChildZ = 0xF4, addChildZTag = 0xF8, addChildZName = 0xFC;
constexpr uint32_t getChildByTag = 0x100, getChildByName = 0x104, getChildren = 0x10C, getChildrenCount = 0x114;
constexpr uint32_t getParent = 0x11C;
constexpr uint32_t removeFromParent = 0x124, removeFromParentAndCleanup = 0x128, removeChild = 0x12C;
constexpr uint32_t removeAllChildren = 0x138;
constexpr uint32_t getTag = 0x148, setTag = 0x14C, getName = 0x150, setName = 0x154;
constexpr uint32_t getOpacity = 0x20C, setOpacity = 0x214, setCascadeOpacityEnabled = 0x220;
constexpr uint32_t getColor = 0x224, setColor = 0x22C, setCascadeColorEnabled = 0x238;
}  // namespace node

// ------------------------------------------------------------------------------------------------ cocos2d values
struct Vec2 { float x, y; };
struct Size { float width, height; };

inline uint32_t newVec2(Frame &f, float x, float y) {
    uint32_t v = f.alloc(8);
    stf(v, x);
    stf(v + 4, y);
    return v;
}
inline Vec2 readVec2(uint32_t a) { return Vec2{ldf(a), ldf(a + 4)}; }

inline uint32_t director() { return call(fn::Director_getInstance, {}); }
inline Size visibleSize() {
    Frame f;
    uint32_t out = f.alloc(8);
    call(fn::Director_getVisibleSize, {out, director()});
    return Size{ldf(out), ldf(out + 4)};
}
inline Vec2 visibleOrigin() {
    Frame f;
    uint32_t out = f.alloc(8);
    call(fn::Director_getVisibleOrigin, {out, director()});
    return Vec2{ldf(out), ldf(out + 4)};
}

// ------------------------------------------------------------------------------------------------ std::string
// 12 bytes: short form (byte0 even): size = byte0 >> 1, chars at +1; long form: cap|1, size, data pointer.
inline bool strIsLong(uint32_t s) { return (ld8(s) & 1) != 0; }
inline uint32_t strSize(uint32_t s) { return strIsLong(s) ? ld32(s + 4) : uint32_t(ld8(s) >> 1); }
inline uint32_t strData(uint32_t s) { return strIsLong(s) ? ld32(s + 8) : s + 1; }
// construct s from n chars at text (guest address)
inline void strInit(uint32_t s, uint32_t text, uint32_t n) { call(fn::string_init, {s, text, n}); }
inline void strInitC(uint32_t s, uint32_t text) { strInit(s, text, host_strlen(text)); }
// the inline destructor: free the heap block of a long string
inline void strFree(uint32_t s) {
    if (strIsLong(s)) call(fn::operator_delete, {ld32(s + 8)});
}
// out = std::move(tmp), as libc++ inlines it: clear out, shrink it (reserve(0) frees a long buffer), take tmp's
// three words and leave tmp empty
inline void strMoveAssign(uint32_t out, uint32_t tmp) {
    if (strIsLong(out)) {
        st8(ld32(out + 8), 0);
        st32(out + 4, 0);
    } else {
        st8(out + 1, 0);
        st8(out, 0);
    }
    call(fn::string_reserve, {out, 0});
    st32(out, ld32(tmp));
    st32(out + 4, ld32(tmp + 4));
    st32(out + 8, ld32(tmp + 8));
    st32(tmp, 0);
    st32(tmp + 4, 0);
    st32(tmp + 8, 0);
}
// a temporary std::string on the guest stack built from a C string
inline uint32_t newStr(Frame &f, uint32_t text) {
    uint32_t s = f.alloc(12);
    strInitC(s, text);
    return s;
}
inline uint32_t newStr(Frame &f, uint32_t text, uint32_t n) {
    uint32_t s = f.alloc(12);
    strInit(s, text, n);
    return s;
}

// ------------------------------------------------------------------------------------------------ std::function
// 16-byte inline buffer, then the callable pointer at +16 (== the object itself when stored inline).
// __base vtable: +0x08 __clone(), +0x0C __clone(dst), +0x10 destroy, +0x14 destroy_deallocate, +0x18 operator().
inline void functionDestroy(uint32_t f) {
    uint32_t p = ld32(f + 16);
    if (p == f) vcall(p, 0x10, {p});
    else if (p) vcall(p, 0x14, {p});
}
// a std::function holding a lambda stored inline: vtable (of __func<lambda>) and up to 3 captured words
inline uint32_t newFunction(Frame &f, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    uint32_t fo = f.alloc(20);
    st32(fo, vtable);
    uint32_t k = 4;
    for (uint32_t c : captures) { st32(fo + k, c); k += 4; }
    st32(fo + 16, fo);
    return fo;
}
inline uint32_t newEmptyFunction(Frame &f) {
    uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    return fo;
}

// ------------------------------------------------------------------------------------------------ shared_ptr
// object pointer, control block pointer
inline void sharedRelease(uint32_t sp) {
    uint32_t cb = ld32(sp + 4);
    if (cb) call(fn::shared_release, {cb});
}

}  // namespace scene
