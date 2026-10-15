// Small helpers shared by the w4_b2 rewrites (SQEX sound engine, bg framework, free functions, game pieces):
// raw guest loads / logged stores by address, GOT reads, softfp float bits, libgcc-style float -> int.
#pragma once

#include <cmath>
#include <cstring>

#include "../core/Guest.hpp"
#include "rw/rw.hpp"

namespace b2 {

inline uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
inline int32_t lds32(uint32_t a) { return rw::load<int32_t>(a); }
inline uint16_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
inline int16_t lds16(uint32_t a) { return rw::load<int16_t>(a); }
inline uint8_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }
inline int8_t lds8(uint32_t a) { return rw::load<int8_t>(a); }
inline float ldf(uint32_t a) { return rw::load<float>(a); }
inline void st32(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }
inline void st16(uint32_t a, uint16_t v) { rw::save<uint16_t>(a, v); }
inline void st8(uint32_t a, uint8_t v) { rw::save<uint8_t>(a, v); }
inline void stf(uint32_t a, float v) { rw::save<float>(a, v); }

using rw::bitsf;
using rw::fbits;

// A vtable / data address the code reads from the GOT (relocated at load time, so read at run time).
inline uint32_t got(uint32_t entry) { return ld32(entry); }

// virtual call through a byte offset into the vtable
inline uint32_t vcallo(uint32_t obj, uint32_t offset, std::initializer_list<uint32_t> args) {
    uint32_t fn = ld32(ld32(obj) + offset);
    return rw::call(fn, args);
}

// sign-extended byte result (the "sxtb r0, r0" epilogues)
inline int32_t sxtb(uint32_t v) { return int32_t(int8_t(uint8_t(v))); }

// float -> integer like libgcc / VFP vcvt (truncate, saturate, NaN -> 0)
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
inline int32_t d2i(double f) {
    if (std::isnan(f)) return 0;
    if (f >= 2147483648.0) return INT32_MAX;
    if (f <= -2147483649.0) return INT32_MIN;
    return int32_t(f);
}
inline uint32_t d2u(double f) {
    if (std::isnan(f) || f <= -1.0) return 0;
    if (f >= 4294967296.0) return UINT32_MAX;
    return uint32_t(f);
}

// __android_log_print (import)
constexpr uint32_t ANDROID_LOG = 0x413D38;
constexpr uint32_t MUTEX_LOCK = 0x4466C0;    // pthread_mutex_lock (import)
constexpr uint32_t MUTEX_UNLOCK = 0x4466E4;  // pthread_mutex_unlock (import)
inline uint32_t mutexLock(uint32_t m) { return rw::call(MUTEX_LOCK, {m}); }
inline uint32_t mutexUnlock(uint32_t m) { return rw::call(MUTEX_UNLOCK, {m}); }

}  // namespace b2
