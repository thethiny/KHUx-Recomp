// SQEX sound driver (Square Enix "sqexsdlib"): layouts of the objects the w4_b2 rewrites touch.
#pragma once

#include "B2Common.hpp"

namespace sqex {

using rw::Field;

// A value that moves linearly from `from` to `to` over `duration` (ms) - fades of volume, pitch, pan.
struct DynamicValue {
    Field<float> from;        // +0x00
    Field<float> to;          // +0x04
    Field<int32_t> elapsed;   // +0x08
    Field<int32_t> duration;  // +0x0C
    Field<uint32_t> flags;    // +0x10 bit0: moving
};
RW_SIZE(DynamicValue, 20);

// SQEX::SoundImpl (OnMemorySound / StreamingSound derive from it)
struct SoundImpl {
    Field<uint32_t> vtable;        // +0x00
    Field<uint32_t> voice;         // +0x04 Voice* (playing core)
    Field<int32_t> id;             // +0x08 handle: slot | serial << 16
    DynamicValue fadeVolume;       // +0x0C
    DynamicValue playFade;         // +0x20 0 -> 1 on play, -> 0 on stop
    DynamicValue pauseFade;        // +0x34
    DynamicValue volume;           // +0x48
    DynamicValue pitch;            // +0x5C
    DynamicValue pan;              // +0x70
    DynamicValue categoryVolume;   // +0x84
    DynamicValue categoryPitch;    // +0x98
    Field<int32_t> state;          // +0xAC 0 not ready, 1 ready, 2 play waiting, 3 playing, 4 stopping, 5 finished
    Field<int32_t> pauseState;     // +0xB0 1 pausing, 2 paused
    Field<uint8_t> flagB4;         // +0xB4
    Field<uint8_t> flagB5;         // +0xB5
    const uint8_t pad_B6[2];
    Field<uint32_t> playTime;      // +0xB8 accumulated ms
    Field<float> randomVolume;     // +0xBC
    Field<float> randomPitch;      // +0xC0
    Field<uint8_t> flagC4;         // +0xC4
    const uint8_t pad_C5[1];
    Field<uint16_t> externalId;    // +0xC6
    Field<uint32_t> seekTime;      // +0xC8
    Field<float> attrVolume;       // +0xCC
    Field<uint8_t> layoutEnabled;  // +0xD0
    const uint8_t pad_D1[3];
    Field<float> position[4];      // +0xD4
    Field<float> innerRange;       // +0xE4
    Field<float> outerRange;       // +0xE8
    Field<float> layoutVolume;     // +0xEC
    Field<float> layoutPan;        // +0xF0
    Field<uint32_t> interiorFactor;// +0xF4
    Field<uint8_t> noLayout;       // +0xF8
    const uint8_t pad_F9[3];
};
RW_LAYOUT(SoundImpl, state, 0xAC);
RW_LAYOUT(SoundImpl, noLayout, 0xF8);
RW_SIZE(SoundImpl, 0xFC);

// SoundManager's slot: a node of the sound pool (free list / used list), 24 bytes
struct SoundSlot {
    Field<uint32_t> vtable;  // +0x00
    Field<uint32_t> prev;    // +0x04
    Field<uint32_t> next;    // +0x08
    Field<int32_t> index;    // +0x0C
    Field<uint32_t> flags;   // +0x10 bit0: in use
    Field<uint32_t> sound;   // +0x14 SoundImpl*
};
RW_SIZE(SoundSlot, 24);

// SoundManager's static state at 0x100E5F0
struct SoundManagerState {
    Field<int32_t> capacity;   // +0x00
    Field<int32_t> count;      // +0x04
    Field<uint32_t> slots;     // +0x08 SoundSlot[capacity] (block header: elem size, count at -8 / -4)
    Field<uint32_t> freeHead;  // +0x0C
    Field<uint32_t> usedHead;  // +0x10
    Field<uint32_t> usedTail;  // +0x14
    const uint8_t mutex[4];    // +0x18 pthread_mutex_t
    Field<uint16_t> serial;    // +0x1C
};

constexpr uint32_t SOUND_MANAGER = 0x100E5F0;
constexpr uint32_t SOUND_MUTEX = SOUND_MANAGER + 0x18;
constexpr uint32_t ACTION_MANAGER = 0x100E5D8;   // nodes, free, head, tail, capacity, mutex (+0x14)
constexpr uint32_t ACTION_MUTEX = ACTION_MANAGER + 0x14;
constexpr uint32_t LOG_TAG = 0xEECB88;           // "sqexsdlib"

constexpr uint32_t MEMORY_MALLOC = 0xCE43B4;     // SQEX::Memory::Malloc(size, align)
constexpr uint32_t MEMORY_FREE = 0xCE43C0;       // SQEX::Memory::Free(p)

inline SoundManagerState &soundManager() { return *rw::host_ptr<SoundManagerState>(SOUND_MANAGER); }
inline uint32_t malloc_(uint32_t size, uint32_t align) { return rw::call(MEMORY_MALLOC, {size, align}); }
inline void free_(uint32_t p) { rw::call(MEMORY_FREE, {p}); }
inline void log(uint32_t level, uint32_t fmt) { rw::call(b2::ANDROID_LOG, {level, LOG_TAG, fmt}); }
inline void log(uint32_t level, uint32_t fmt, uint32_t a) { rw::call(b2::ANDROID_LOG, {level, LOG_TAG, fmt, a}); }

}  // namespace sqex
