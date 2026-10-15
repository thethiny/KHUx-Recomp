// Random: the game's random number generator, a SafeBuffer-protected LCG state (the libc rand() constants), seeded
// from the microsecond clock. A caller may pass its own seed word instead of the shared state.
#pragma once

#include "core/SafeBuffer.hpp"

struct Random : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7E2B4;          // _ZTV6Random + 8
    static constexpr uint32_t INSTANCE = 0x1003414;       // the singleton (function-local static)
    static constexpr uint32_t INSTANCE_GUARD = 0x1003410;
    static constexpr uint32_t MAX = 0x7FFFFFFF;

    static Random *getInstance();
    static Random *construct(Random *self);
    static uint32_t getMaxInt(Random *self);
    // Next value in 0..MAX: steps *seed when given, else the shared state.
    static uint32_t generate(Random *self, const uint32_t *seed);
    // Uniform integer in [lo, hi].
    static int32_t generateUniformInt(Random *self, int32_t lo, int32_t hi, const uint32_t *seed);
    // Uniform float in [lo, hi).
    static float generateUniformFloat(Random *self, float lo, float hi, const uint32_t *seed);
    static void deletingDestroy(Random *self);
};
RW_SIZE(Random, 32);
