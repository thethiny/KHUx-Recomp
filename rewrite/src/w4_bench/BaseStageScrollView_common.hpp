// Private helpers of the BaseStageScrollView rewrite (BaseStageScrollView*.cpp only).
#pragma once

#include <climits>

#include "BaseStageScrollView.hpp"

namespace bssv {

namespace g {
constexpr uint32_t Layout_ctor = 0x828671;                 // cocos2d::ui::Layout::Layout
constexpr uint32_t Layout_dtor = 0x8289C5;                 // cocos2d::ui::Layout::~Layout
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t MEMMOVE = 0x40EF50;                     // __aeabi_memmove
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;     // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t Widget_setContentSize = 0x825C75;       // ui::Widget::setContentSize(const Size&)
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t WidgetVector_dtor = 0x485A8F;           // cocos2d::Vector<ui::Widget*>::~Vector
constexpr uint32_t WidgetVector_clear = 0x485B45;
constexpr uint32_t WidgetVector_pushBack = 0x4F6B5D;
constexpr uint32_t WidgetVector_erase = 0x4F64B1;          // erase(iterator)
constexpr uint32_t UpdateDataVector_dtor = 0x59BD11;       // std::__vector_base<UpdateData>::~__vector_base
constexpr uint32_t UpdateData_pushBackSlow = 0x59DC55;     // vector<UpdateData>::__push_back_slow_path(UpdateData&&)
constexpr uint32_t FloatVector_dtor = 0x4F59FF;            // std::__vector_base<float>::~__vector_base
constexpr uint32_t IntVector_dtor = 0x478961;              // std::__vector_base<int>::~__vector_base
constexpr uint32_t IntVector_assign = 0x47D5BD;            // vector<int>::assign(int*, int*)
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Size_copy = 0x8DF02D;                   // Size::Size(const Size&)
constexpr uint32_t Vec2_ctor = 0x8E4B9D;                   // Vec2::Vec2(float, float)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor = 0x8DF18B;                   // Rect::Rect(x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Touch_getLocation = 0x900E57;           // (Vec2* out, Touch*)
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FunctionTouchBool_swap = 0x483E4D;      // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;      // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t FunctionVV_call = 0x470E69;             // std::function<void()>::operator()
constexpr uint32_t FunctionVI_call = 0x4F321D;             // std::function<void(int)>::operator()(int)
constexpr uint32_t FunctionVF_call = 0x4F8E89;             // std::function<void(float)>::operator()(float)
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;      // (shared_ptr<Layer>* out, Manager*, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;          // (Layer*, ResourceID, int, int)
constexpr uint32_t CEILF = 0x40EFBC, FLOORF = 0x415244;    // libm imports (float)
constexpr uint32_t SQRT = 0x415208, FABS = 0x415214;       // libm imports (double)
constexpr uint32_t DIVSI3 = 0xCD5E8D, MODSI3 = 0xCD6009;   // libgcc
}  // namespace g

// virtual slots (byte offsets into the vtable)
constexpr uint32_t V_setPosition = 0x4C;          // Node::setPosition(const Vec2&)
constexpr uint32_t V_getPosition = 0x54;          // Node::getPosition() -> const Vec2&
constexpr uint32_t V_getPositionX = 0x68;
constexpr uint32_t V_getPositionY = 0x70;
constexpr uint32_t V_getContentSize = 0xAC;       // -> const Size&
constexpr uint32_t V_setVisible = 0xB0;
constexpr uint32_t V_addChild = 0xF0;
constexpr uint32_t V_getParent = 0x11C;
constexpr uint32_t V_removeChild = 0x12C;         // (child, cleanup)
constexpr uint32_t V_setTouchEnabled = 0x31C;
constexpr uint32_t V_createRow = 0x33C;           // (itemID, int) -> ui::Widget*
constexpr uint32_t V_changeRow = 0x340;           // (widget, itemID, int) -> bool: true = replace the widget
constexpr uint32_t V_changeOverRow = 0x344;       // (widget, index): the row shows no item
constexpr uint32_t V_registItemTouchEvent = 0x348;  // (widget, row)
constexpr uint32_t V_updateScroll = 0x34C;        // (float delta)

constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;
// EventListenerTouchOneByOne std::function members
constexpr uint32_t LISTENER_ON_TOUCH_BEGAN = 0x50, LISTENER_ON_TOUCH_MOVED = 0x68, LISTENER_ON_TOUCH_ENDED = 0x80,
                   LISTENER_ON_TOUCH_CANCELLED = 0x98;
// __func vtables (+8) of init()'s lambdas
constexpr uint32_t VT_TouchBegan = 0xF5B354;   // $_0 [this]
constexpr uint32_t VT_TouchMoved = 0xF5B328;   // $_1 [this, this]
constexpr uint32_t VT_TouchEnded = 0xF5B2FC;   // $_2 [this, this]
constexpr uint32_t SE_SCROLL = 0x1E56B9EA;     // audio::ResourceID played when a drag ends

constexpr uint32_t SIGN = 0x80000000u;
using Self = BaseStageScrollView;

inline uint32_t me(const Self *s) { return rw::guest_addr(s); }
inline uint32_t fb(float f) { return rw::fbits(f); }
inline float neg(float v) { return rw::bitsf(rw::fbits(v) ^ SIGN); }   // eors with the sign bit
inline float absf(float v) { return rw::bitsf(rw::fbits(v) & 0x7FFFFFFFu); }
inline bool unord(float a, float b) { return a != a || b != b; }
inline uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
inline float ldf(uint32_t a) { return rw::load<float>(a); }
// libgcc __fixsfsi: truncate, saturate, NaN -> 0
inline int32_t f2i(float f) {
    if (f != f) return 0;
    if (f >= 2147483648.0f) return INT32_MAX;
    if (f < -2147483648.0f) return INT32_MIN;
    return int32_t(f);
}
// libgcc integer division (the guest helpers: division by zero stays theirs)
inline int32_t idiv(int32_t a, int32_t b) { return int32_t(rw::call(g::DIVSI3, {uint32_t(a), uint32_t(b)})); }
inline int32_t imod(int32_t a, int32_t b) { return int32_t(rw::call(g::MODSI3, {uint32_t(a), uint32_t(b)})); }
inline double callD(uint32_t fn, double v) {
    uint64_t u;
    std::memcpy(&u, &v, 8);
    uint64_t r = rw::call64(fn, {uint32_t(u), uint32_t(u >> 32)});
    double out;
    std::memcpy(&out, &r, 8);
    return out;
}
inline float callF(uint32_t fn, float v) { return rw::bitsf(rw::call(fn, {fb(v)})); }

// virtual call by byte offset
inline uint32_t vfn(uint32_t obj, uint32_t off) { return ld32(ld32(obj) + off); }
inline uint32_t vc(uint32_t obj, uint32_t off, std::initializer_list<uint32_t> a) { return rw::call(vfn(obj, off), a); }
// calls laid out like the compiled ones (inside a guest::Frame of the original size)
inline uint32_t cx(uint32_t fn, std::initializer_list<uint32_t> a) { return uint32_t(rw::call_exact(fn, a)); }
inline uint32_t vx(uint32_t obj, uint32_t off, std::initializer_list<uint32_t> a) { return cx(vfn(obj, off), a); }

// The inline libc++ ~function() of a std::function at f (callable pointer at +16): destroy() when it lives in the
// inline buffer, destroy_deallocate() when on the heap.
inline void destroyFunction(uint32_t f, bool exact) {
    uint32_t p = ld32(f + 16);
    if (p == f) {
        uint32_t fn = ld32(ld32(p) + 0x10);
        exact ? cx(fn, {p}) : rw::call(fn, {p});
    } else if (p) {
        uint32_t fn = ld32(ld32(p) + 0x14);
        exact ? cx(fn, {p}) : rw::call(fn, {p});
    }
}

}  // namespace bssv
