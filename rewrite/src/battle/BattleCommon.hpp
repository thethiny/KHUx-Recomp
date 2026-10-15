// Shared helpers for the battle module (StageState, StageManager, the stage actor / field / battle managers,
// MapPrizes, AimTarget, BattleUtil): guest stack scratch space, libc++ / cocos2d layouts, ARM soft-float
// conversions and the guest addresses of library functions the module calls often.
#pragma once

#include "rw/rw.hpp"
#include "../core/SafeBuffer.hpp"

#include <cmath>

namespace bt {

// ------------------------------------------------------------------------------------------- guest stack
// Scratch bytes on the guest stack, for objects the original keeps in its frame and passes to guest functions
// (an sret shared_ptr, a Vec2 by reference, a temporary std::string). The block lives below the caller's sp,
// inside the stack window the verifier ignores; sp is restored when the frame goes out of scope.
class GuestFrame {
public:
    explicit GuestFrame(uint32_t bytes) : saved_(rw::cpu->r[13]) {
        base_ = (saved_ - bytes) & ~7u;
        rw::cpu->r[13] = base_;
    }
    ~GuestFrame() { rw::cpu->r[13] = saved_; }
    GuestFrame(const GuestFrame &) = delete;
    GuestFrame &operator=(const GuestFrame &) = delete;

    uint32_t addr(uint32_t offset = 0) const { return base_ + offset; }
    template <class T> T *ptr(uint32_t offset = 0) const { return rw::frame_ptr<T>(base_ + offset); }
    uint32_t word(uint32_t offset) const { return rw::load<uint32_t>(base_ + offset); }
    void setWord(uint32_t offset, uint32_t v) const { rw::save<uint32_t>(base_ + offset, v); }

private:
    uint32_t saved_;
    uint32_t base_;
};

// ------------------------------------------------------------------------------------------- float helpers
// The library is built soft-float (libgcc __addsf3, __aeabi_fcmp*, ...): IEEE single precision with
// round-to-nearest, which host float arithmetic reproduces. Floats travel as raw bits in core registers.
inline uint32_t bits(float f) { uint32_t u; std::memcpy(&u, &f, 4); return u; }
inline float fromBits(uint32_t u) { float f; std::memcpy(&f, &u, 4); return f; }
inline uint64_t bits(double d) { uint64_t u; std::memcpy(&u, &d, 8); return u; }
inline double dfromBits(uint64_t u) { double d; std::memcpy(&d, &u, 8); return d; }

// __aeabi_f2iz / __fixsfsi: truncate toward zero, saturating; NaN -> 0.
inline int32_t f2iz(float f) {
    if (std::isnan(f)) return 0;
    if (f >= 2147483648.0f) return INT32_MAX;
    if (f <= -2147483648.0f) return INT32_MIN;
    return int32_t(f);
}
// __aeabi_f2uiz / __fixunssfsi: truncate, negative -> 0, saturating; NaN -> 0.
inline uint32_t f2uiz(float f) {
    if (std::isnan(f) || f <= 0.0f) return 0;
    if (f >= 4294967296.0f) return UINT32_MAX;
    return uint32_t(f);
}
// __aeabi_d2iz / __fixdfsi
inline int32_t d2iz(double d) {
    if (std::isnan(d)) return 0;
    if (d >= 2147483648.0) return INT32_MAX;
    if (d <= -2147483649.0) return INT32_MIN;
    return int32_t(d);
}
inline uint32_t d2uiz(double d) {
    if (std::isnan(d) || d <= 0.0) return 0;
    if (d >= 4294967296.0) return UINT32_MAX;
    return uint32_t(d);
}

// ------------------------------------------------------------------------------------------- layouts
// cocos2d::Vec2 / Size: two floats.
struct Vec2 {
    rw::Field<float> x;
    rw::Field<float> y;
};
RW_SIZE(Vec2, 8);

// std::shared_ptr<T> (libc++, 32-bit): object pointer, control block (std::__shared_weak_count).
struct SharedPtr {
    rw::Field<uint32_t> obj;
    rw::Field<uint32_t> ctrl;
};
RW_SIZE(SharedPtr, 8);

// std::vector<T> (libc++): begin, end, end of capacity. cocos2d::Vector<T*> holds one at +0.
struct Vector {
    rw::Field<uint32_t> begin;
    rw::Field<uint32_t> end;
    rw::Field<uint32_t> cap;
    uint32_t count(uint32_t elem = 4) const { return (end.get() - begin.get()) / elem; }
    uint32_t at(uint32_t k) const { return rw::load<uint32_t>(begin.get() + 4 * k); }
};
RW_SIZE(Vector, 12);

// std::string (libc++, 32-bit, 12 bytes): byte0 bit 0 clear = short form (size = byte0 >> 1, chars at +1);
// set = long form (capacity | 1 at +0, size at +4, data pointer at +8).
struct String {
    const uint8_t raw[12];
    bool isLong() const { return (raw[0] & 1) != 0; }
    uint32_t size() const { return isLong() ? rw::get<uint32_t>(raw, 4) : uint32_t(raw[0] >> 1); }
    uint32_t dataAddr() const { return isLong() ? rw::get<uint32_t>(raw, 8) : rw::guest_addr(raw + 1); }
};
RW_SIZE(String, 12);

// ------------------------------------------------------------------------------------------- library calls
// Guest addresses as the lifted code calls them (Thumb bit set for Thumb code; PLT stubs for imports).
namespace fn {
constexpr uint32_t operator_new = 0xC98EF1;           // operator new(unsigned)
constexpr uint32_t operator_delete = 0xC98F9B;        // operator delete(void*)
constexpr uint32_t release_shared = 0xC8DBD1;        // std::__shared_weak_count::__release_shared()
constexpr uint32_t ref_autorelease = 0x8FD0D5;       // cocos2d::Ref::autorelease()
constexpr uint32_t ref_dtor = 0x8FD059;              // cocos2d::Ref::~Ref()
constexpr uint32_t safebuffer_dtor = 0x45A13D;       // SafeBuffer::~SafeBuffer()
}  // namespace fn

// Drop a shared_ptr's reference (the inlined `if (ctrl) ctrl->__release_shared()`).
inline void releaseShared(uint32_t ctrl) {
    if (ctrl) rw::call(fn::release_shared, {ctrl});
}

// A 32-bit value at data position `index` of a SafeBuffer-backed object at guest address `obj`.
inline int32_t sbGet32(uint32_t obj, uint32_t index) {
    return rw::host_ptr<SafeBuffer>(obj)->get32(index);
}

}  // namespace bt
