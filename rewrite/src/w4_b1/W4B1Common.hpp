// Shared by the w4_b1 rewrites (SceneMyPage, SceneLight, StartDeckEditDialog, DeckRevolverView, BaseSlideView):
// the library entry points they call, on top of scene_main/MainCommon.hpp.
#pragma once

#include "../scene_main/MainCommon.hpp"

namespace w4b1 {

using namespace smain;

namespace wl {
// runtime / libc++
constexpr uint32_t NEW_ARRAY = 0xC98F6F;                 // operator new[]
constexpr uint32_t DELETE_ARRAY = 0xC98FB3;              // operator delete[]
constexpr uint32_t MEMSET = 0x40E7A0;                    // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t MEMCPY = 0x40E164;                    // __aeabi_memcpy(dst, src, n) (import)
constexpr uint32_t MEMMOVE = 0x40EF50;                   // __aeabi_memmove(dst, src, n) (import)
constexpr uint32_t VECTOR_AT_THROW = 0x47CCE1;           // sub_47CCE0: vector::at out of range (never returns)
constexpr uint32_t IntVector_baseDtor = 0x478961;        // std::__vector_base<int>::~__vector_base
constexpr uint32_t IntVector_assign = 0x47D5BD;          // std::vector<int>::assign(int*, int*)
constexpr uint32_t IntVector_pushBackSlow = 0x4D6CC1;    // std::vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t FunctionVV_call = 0x470E69;           // std::function<void()>::operator()
constexpr uint32_t FunctionVF_call = 0x4F8E89;           // std::function<void(float)>::operator()(float)
constexpr uint32_t FunctionTouchB_swap = 0x483E4D;       // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionTouchV_swap = 0x4F3699;       // std::function<void(Touch*, Event*)>::swap
// cocos2d
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v;                 // Vec2::Vec2() (0, 0)
constexpr uint32_t Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff;               // Vec2::Vec2(float, float)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;                 // Vec2::~Vec2()
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;               // ui::Layout::Layout()
constexpr uint32_t Layout_dtor = rwlib::cocos2d::ui::Layout::dtor_base;               // ui::Layout::~Layout()
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;          // (float)
constexpr uint32_t MoveTo_create = rwlib::cocos2d::MoveTo::create;             // (float, const Vec2&)
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;           // (const std::function<void()>&)
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;           // (FiniteTimeAction*, ..., nullptr)
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
}  // namespace wl

// softfp bit casts (short names: scene:: and rw:: both declare fbits / bitsf)
inline uint32_t fb(float f) { return scene::fbits(f); }
inline float bf(uint32_t u) { return scene::bitsf(u); }

// libc++ std::vector<T> (4-byte elements) seen through its guest address
inline uint32_t vecBegin(uint32_t v) { return ld32(v); }
inline uint32_t vecEnd(uint32_t v) { return ld32(v + 4); }
inline uint32_t vecSize4(uint32_t v) { return uint32_t(int32_t(vecEnd(v) - vecBegin(v)) >> 2); }

// vector<T>::erase(begin()) for 4-byte elements, as libc++ inlines it: memmove the tail down, then pull the end
// pointer back (stored only when it changes). Returns the new end.
inline uint32_t vecEraseFront4(uint32_t v) {
    uint32_t b = vecBegin(v), e = vecEnd(v);
    uint32_t n = e - (b + 4);
    call(wl::MEMMOVE, {b, b + 4, n});
    uint32_t newEnd = b + (n & ~3u);
    if (ld32(v + 4) != newEnd) st32(v + 4, newEnd);
    return newEnd;
}

// vector<T>::clear() for 4-byte elements, as inlined: the end pointer is stored only when it changes
inline void vecClear4(uint32_t v) {
    uint32_t b = vecBegin(v), e = vecEnd(v);
    if (e != b) st32(v + 4, b);
}

// Sequence::create(a, b, nullptr) then node->runAction(seq)
inline void runSequence2(uint32_t node, uint32_t a, uint32_t b) {
    uint32_t seq = call(wl::Sequence_create, {a, b, 0});
    call(wl::Node_runAction, {node, seq});
}

// CallFunc::create(inline lambda {vtable, captures}); the temporary std::function is destroyed after
inline uint32_t callFunc(uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fo = newFunction(f, vtable, captures);
    uint32_t cf = call(wl::CallFunc_create, {fo});
    functionDestroy(fo);
    return cf;
}

}  // namespace w4b1
