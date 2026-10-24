// Helpers shared by the system module (utilities, sound, downloads, UI widgets): variadic argument blocks,
// soft-float conversions, guest string / std::function helpers and frequently used library entry points.
#pragma once

#include <climits>
#include <cmath>
#include <cstring>

#include "rw/rw.hpp"
#include "core/Guest.hpp"

namespace sys {

// ------------------------------------------------------------------------------------------------ varargs
// A variadic function's va_list: the original spills the variadic register arguments (r<first>..r3) just below
// its entry sp, so they continue into the stack arguments, and passes that address as the va_list. The block
// is placed exactly at entry sp - 4 * (4 - first) (no realignment) so it joins the caller's stack arguments.
// Construct it first thing in the function body (the registers are still the entry values).
class VaSpill {
public:
    explicit VaSpill(int first) : saved_(rw::cpu->r[13]) {
        uint32_t words = first < 4 ? uint32_t(4 - first) : 0u;
        va_ = saved_ - 4 * words;
        for (uint32_t k = 0; k < words; k++)
            std::memcpy(rw::frame_ptr<uint8_t>(va_ + 4 * k), &rw::cpu->r[first + int(k)], 4);
        rw::cpu->r[13] = va_ & ~7u;
    }
    ~VaSpill() { rw::cpu->r[13] = saved_; }
    VaSpill(const VaSpill &) = delete;
    VaSpill &operator=(const VaSpill &) = delete;
    uint32_t va() const { return va_; }

private:
    uint32_t saved_, va_;
};

// The original function's frame at exactly entry sp - size (no realignment): for locals whose unwritten bytes
// the callee copies out (stale stack contents then come from the same addresses as in the original).
class ExactFrame {
public:
    explicit ExactFrame(uint32_t size) : saved_(rw::cpu->r[13]) { rw::cpu->r[13] = base_ = saved_ - size; }
    ~ExactFrame() { rw::cpu->r[13] = saved_; }
    ExactFrame(const ExactFrame &) = delete;
    ExactFrame &operator=(const ExactFrame &) = delete;
    uint32_t addr(uint32_t offset = 0) const { return base_ + offset; }

private:
    uint32_t saved_, base_;
};

// ------------------------------------------------------------------------------------------------ floats
// libgcc conversions: truncate toward zero, saturate, NaN -> 0.
inline int32_t f2i(float f) {
    if (std::isnan(f)) return 0;
    if (f >= 2147483648.0f) return INT32_MAX;
    if (f <= -2147483648.0f) return INT32_MIN;
    return int32_t(f);
}
inline uint32_t f2u(float f) {
    if (std::isnan(f) || f <= 0.0f) return 0;
    if (f >= 4294967296.0f) return UINT32_MAX;
    return uint32_t(f);
}
inline int32_t d2i(double d) {
    if (std::isnan(d)) return 0;
    if (d >= 2147483648.0) return INT32_MAX;
    if (d <= -2147483649.0) return INT32_MIN;
    return int32_t(d);
}
inline uint32_t fb(float f) { return rw::fbits(f); }
inline float bf(uint32_t u) { return rw::bitsf(u); }
inline uint64_t db(double d) { uint64_t u; std::memcpy(&u, &d, 8); return u; }
inline double bd(uint64_t u) { double d; std::memcpy(&d, &u, 8); return d; }

// ------------------------------------------------------------------------------------------------ memory
inline uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
inline uint16_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
inline uint8_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }
inline float ldf(uint32_t a) { return rw::load<float>(a); }
inline void st32(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }
inline void st16(uint32_t a, uint16_t v) { rw::save<uint16_t>(a, v); }
inline void st8(uint32_t a, uint8_t v) { rw::save<uint8_t>(a, v); }
inline void stf(uint32_t a, float v) { rw::save<float>(a, v); }
template <class T> inline uint32_t A(const T *p) { return rw::guest_addr(p); }

// virtual call through a byte offset into the vtable (as the code indexes it)
inline uint32_t vcallOff(uint32_t obj, uint32_t offset, std::initializer_list<uint32_t> args) {
    return rw::call(ld32(ld32(obj) + offset), args);
}

// ------------------------------------------------------------------------------------------------ library
namespace lib {
constexpr uint32_t MALLOC = 0x40FF58;         // malloc (import)
constexpr uint32_t FREE = 0x40FF64;           // free (import) - check before use
constexpr uint32_t VSNPRINTF = rwlib::sub_CA3B24;      // sub_CA3B24: vsnprintf(buf, size, fmt, va_list)
constexpr uint32_t STACK_CHK_FAIL = 0x40DD44; // __stack_chk_fail
}  // namespace lib

}  // namespace sys
