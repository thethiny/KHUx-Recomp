// libgcc soft-float / 64-bit helpers of the game library (ARM EABI softfp): conversions, comparisons, 64-bit
// multiply, and libavutil-style rescale64. All are correctly rounded IEEE operations, so the host's arithmetic
// gives the same bits; the edge cases (NaN, overflow, negative inputs) follow the libgcc code.
#include "B2Common.hpp"

using namespace b2;

namespace softfloat {

static double asDouble(uint64_t bits) { double d; std::memcpy(&d, &bits, 8); return d; }
static uint64_t bitsOf(double d) { uint64_t u; std::memcpy(&u, &d, 8); return u; }

struct SoftFloat {
    static double floatunsidf(uint32_t v) { return double(v); }
    static double floatsidf(int32_t v) { return double(v); }
    static double floatdidf(int64_t v) { return double(v); }
    static float floatunsisf(uint32_t v) { return float(v); }
    static float floatsisf(int32_t v) { return float(v); }
    static float floatundisf(uint64_t v) { return float(v); }

    // __nedf2 / __nesf2: 0 when equal (+0 == -0), 1 when unordered, else -1 / 1 by order
    static int32_t nedf2(double a, double b) {
        if (std::isnan(a) || std::isnan(b)) return 1;
        if (a == b) return 0;
        return a < b ? -1 : 1;
    }
    static int32_t nesf2(float a, float b) {
        if (std::isnan(a) || std::isnan(b)) return 1;
        if (a == b) return 0;
        return a < b ? -1 : 1;
    }
    static int32_t unorddf2(double a, double b) { return (std::isnan(a) || std::isnan(b)) ? 1 : 0; }
    static int32_t unordsf2(float a, float b) { return (std::isnan(a) || std::isnan(b)) ? 1 : 0; }

    // truncating conversions: NaN -> 0, out of range saturates (unsigned: negative -> 0)
    static int32_t fixdfsi(double a) { return d2i(a); }
    static uint32_t fixunsdfsi(double a) {
        if (bitsOf(a) >> 63) return 0;  // any negative (also -0.5, -NaN)
        return d2u(a);
    }
    static int32_t fixsfsi(float a) { return f2i(a); }
    static uint32_t fixunssfsi(float a) {
        uint32_t b;
        std::memcpy(&b, &a, 4);
        if (b >> 31) return 0;
        return f2u(a);
    }

    // __fixunsdfdi: high word from a * 2^-32, low word from the remainder
    static uint64_t fixunsdfdi(double a) {
        uint32_t hi = fixunsdfsi(a * asDouble(0x3DF0000000000000ull));
        double rest = a - double(hi) * asDouble(0x41F0000000000000ull);
        uint32_t lo = fixunsdfsi(rest);
        return (uint64_t(hi) << 32) | lo;
    }
    static int64_t fixdfdi(double a) {
        if (a < 0.0) {
            uint64_t m = fixunsdfdi(asDouble(bitsOf(a) ^ 0x8000000000000000ull));
            return int64_t(0 - m);
        }
        return int64_t(fixunsdfdi(a));
    }
    static uint64_t fixunssfdi(float a) { return fixunsdfdi(double(a)); }

    static uint64_t muldi3(uint64_t a, uint64_t b) { return a * b; }

    // rescale64(a, b, c) = a * c / b by binary long division (a < b), 64 bits of fraction
    static int64_t rescale64(int64_t a, int64_t b, int64_t c) {
        if (a >= b) return c;
        if (a < 1) return 0;
        uint64_t x = uint64_t(a), r = 0;
        for (int k = 64;;) {
            x <<= 1;
            r <<= 1;
            if (--k == 0) break;
            if (int64_t(x) >= b) {
                r |= 1;
                x -= uint64_t(b);
            }
        }
        int64_t ret = 0;
        int64_t rs = int64_t(r);
        for (int k = 0; k < 64; k++) {
            if (rs & 1) ret = int64_t(uint64_t(ret) + uint64_t(c));
            rs >>= 1;
            ret >>= 1;
        }
        return ret;
    }
};
RW_FUNCTION(0xCD63A0, "__floatunsidf", SoftFloat::floatunsidf);
RW_FUNCTION(0xCD63C4, "__floatsidf", SoftFloat::floatsidf);
RW_FUNCTION(0xCD6440, "__floatdidf", SoftFloat::floatdidf);
RW_FUNCTION(0xCD6D60, "__floatunsisf", SoftFloat::floatunsisf);
RW_FUNCTION(0xCD6D68, "__floatsisf", SoftFloat::floatsisf);
RW_FUNCTION(0xCD6D88, "__floatundisf", SoftFloat::floatundisf);
RW_FUNCTION(0xCD6920, "__nedf2", SoftFloat::nedf2);
RW_FUNCTION(0xCD711C, "__nesf2", SoftFloat::nesf2);
RW_FUNCTION(0xCD6A3C, "__unorddf2", SoftFloat::unorddf2);
RW_FUNCTION(0xCD7208, "__unordsf2", SoftFloat::unordsf2);
RW_FUNCTION(0xCD6A74, "__fixdfsi", SoftFloat::fixdfsi);
RW_FUNCTION(0xCD6AD0, "__fixunsdfsi", SoftFloat::fixunsdfsi);
RW_FUNCTION(0xCD7240, "__fixsfsi", SoftFloat::fixsfsi);
RW_FUNCTION(0xCD729C, "__fixunssfsi", SoftFloat::fixunssfsi);
RW_FUNCTION(0xCD7470, "__fixunsdfdi", SoftFloat::fixunsdfdi);
RW_FUNCTION(0xCD73E8, "__fixdfdi", SoftFloat::fixdfdi);
RW_FUNCTION(0xCD7428, "__fixunssfdi", SoftFloat::fixunssfdi);
RW_FUNCTION(0xCD7388, "__muldi3", SoftFloat::muldi3);
RW_FUNCTION(0xCEFD60, "rescale64", SoftFloat::rescale64);

}  // namespace softfloat
