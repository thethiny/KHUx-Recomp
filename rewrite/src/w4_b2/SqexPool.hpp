// SQEX handle pools (SoundManager's sounds, SdSoundSystem's banks): a growable array of 24-byte slots
// {vtable, prev, next, index, flags (bit0: in use), object} kept on a free list and a used list, with the pool
// state {capacity, count, slots, free head, used head, used tail} at a fixed address. The compiled code inlines
// the same list operations in every function that touches a pool; these helpers reproduce them exactly.
#pragma once

#include "Sqex.hpp"

namespace sqex::pool {

using namespace b2;

// pool state offsets
constexpr uint32_t CAP = 0, COUNT = 4, SLOTS = 8, FREE = 0xC, USED = 0x10, TAIL = 0x14;

// Moves slot `index` from the used list back to the free list, when it is a valid slot in use.
inline bool release(uint32_t base, int32_t index) {
    if (index < 0 || index >= lds32(base + CAP) || ld32(base + SLOTS) == 0) return false;
    uint32_t node = ld32(base + SLOTS) + uint32_t(index) * 24u;
    if ((ld8(node + 0x10) & 1) == 0) return false;
    uint32_t prev = ld32(node + 4), next = ld32(node + 8);
    if (prev == 0) {
        st32(base + USED, next);
        if (next == 0) st32(base + TAIL, 0);
        else st32(next + 4, 0);
    } else {
        st32(prev + 8, next);
        uint32_t n = ld32(node + 8);
        if (n == 0) {
            st32(base + TAIL, prev);
            st32(prev + 8, 0);
        } else {
            st32(n + 4, prev);
        }
    }
    st32(node + 4, 0);
    st8(node + 0x10, uint8_t(ld8(node + 0x10) & ~1u));
    uint32_t free = ld32(base + FREE);
    st32(node + 8, free);
    if (free != 0) st32(free + 4, node);
    st32(base + FREE, node);
    st32(base + COUNT, ld32(base + COUNT) - 1);
    return true;
}

// Fills a fresh slot block: every slot unused, index -1.
inline void initSlots(uint32_t block, uint32_t count, uint32_t vtableGot) {
    uint32_t vtable = got(vtableGot) + 8;
    for (uint32_t k = 0; k < count; k++) {
        uint32_t e = block + 8 + k * 24;
        st32(e, vtable);
        st8(e + 0x10, uint8_t(ld8(e + 0x10) & ~1u));
        st32(e + 4, 0);
        st32(e + 8, 0);
        st32(e + 0xC, 0xFFFFFFFF);
        st32(e + 0x10, 0);
        st32(e + 0x14, 0);
    }
}

// Destroys the slot array (each slot's destructor, from the last one down) and frees its block.
inline void destroySlots(uint32_t base) {
    uint32_t slots = ld32(base + SLOTS);
    uint32_t p = slots + ld32(slots - 4) * 24;
    while (ld32(base + SLOTS) != p) {
        p -= 24;
        rw::call(ld32(ld32(p)), {p});
    }
    free_(ld32(base + SLOTS) - 8);
}

// Chains slots from..to-1 through prev / next with their indices; the last keeps next = 0.
inline void chain(uint32_t arr, int32_t from, int32_t to) {
    uint32_t first = arr + uint32_t(from) * 24;
    st32(first + 0xC, uint32_t(from));
    st32(first + 4, 0);
    uint32_t prev = first;
    for (int32_t k = from + 1; k < to; k++) {
        uint32_t e = arr + uint32_t(k) * 24;
        st32(e + 4, prev);
        st32(e + 0xC, uint32_t(k));
        if (prev != 0) st32(prev + 8, e);
        prev = e;
    }
}

// (Re)creates the pool with 32 free slots.
inline void initialize(uint32_t base, uint32_t vtableGot) {
    if (ld32(base + SLOTS) != 0) destroySlots(base);
    st32(base + SLOTS, 0);
    st32(base + FREE, 0);
    st32(base + USED, 0);
    st32(base + TAIL, 0);
    st32(base + COUNT, 0);
    st32(base + CAP, 32);
    uint32_t block = malloc_(0x308, 4);
    st32(block + 4, 32);
    st32(block, 24);
    initSlots(block, 32, vtableGot);
    uint32_t first = block + 8;
    int32_t cap = lds32(base + CAP);
    st32(base + FREE, first);
    st32(base + SLOTS, first);
    if (cap > 0) chain(first, 0, cap);
    st32(base + USED, 0);
    st32(base + TAIL, 0);
    st32(base + COUNT, 0);
}

// Doubles the pool, carrying the used list over to the new array; returns the new free head.
inline uint32_t grow(uint32_t base, uint32_t vtableGot) {
    int32_t oldCap = lds32(base + CAP);
    uint32_t newCap = uint32_t(oldCap) * 2;
    uint32_t size = newCap <= 0x5500000u ? (uint32_t(oldCap) * 6u) * 8u + 8u : 0xFFFFFFFFu;
    uint32_t block = malloc_(size, 4);
    st32(block + 4, newCap);
    st32(block, 24);
    if (newCap != 0) initSlots(block, newCap, vtableGot);
    uint32_t arr = block + 8;
    if (arr == 0) return ld32(base + FREE);
    uint32_t oldUsedHead = ld32(base + USED);
    for (uint32_t n = oldUsedHead; n != 0; n = ld32(n + 8)) {
        int32_t idx = lds32(n + 0xC);
        uint32_t prev = ld32(n + 4);
        uint8_t used = ld8(n + 0x10);
        uint32_t object = ld32(n + 0x14);
        uint32_t e = arr + uint32_t(idx) * 24;
        st32(e + 0xC, uint32_t(idx));
        uint32_t newPrev = prev != 0 ? arr + ld32(prev + 0xC) * 24 : 0;
        uint8_t f = ld8(e + 0x10);
        st32(e + 0x14, object);
        st8(e + 0x10, uint8_t((f & ~1u) | (used & 1u)));
        if (prev != 0) {
            st32(e + 4, newPrev);
            st32(newPrev + 8, e);
        } else {
            st32(e + 4, 0);
        }
    }
    int32_t cap = lds32(base + CAP);
    uint32_t freeHead = arr + uint32_t(cap) * 24;
    if (int32_t(newCap) > cap) chain(arr, cap, int32_t(newCap));  // the new slots form the free list
    st32(base + FREE, freeHead);
    if (oldUsedHead != 0) st32(base + USED, arr + ld32(oldUsedHead + 0xC) * 24);
    uint32_t tail = ld32(base + TAIL);
    if (tail != 0) st32(base + TAIL, arr + ld32(tail + 0xC) * 24);
    st32(base + CAP, newCap);
    if (ld32(base + SLOTS) != 0) {
        destroySlots(base);
        freeHead = ld32(base + FREE);
    }
    st32(base + SLOTS, arr);
    return freeHead;
}

// Takes free slot `node`: off the free list, appended to the used list, marked in use, holding `object`.
inline void take(uint32_t base, uint32_t node, uint32_t object) {
    uint32_t next = ld32(node + 8);
    uint32_t usedHead = ld32(base + USED);
    st32(base + FREE, next);
    if (usedHead == 0) {
        st32(base + TAIL, node);
        st32(base + USED, node);
        st32(node + 4, 0);
        st32(node + 8, 0);
    } else {
        uint32_t tail = ld32(base + TAIL);
        st32(node + 8, 0);
        st32(node + 4, tail);
        st32(tail + 8, node);
        st32(base + TAIL, node);
    }
    uint8_t f = ld8(node + 0x10);
    uint32_t count = ld32(base + COUNT);
    st32(node + 0x14, object);
    st8(node + 0x10, uint8_t(f | 1));
    st32(base + COUNT, count + 1);
}

// Next serial number for a handle (never 0), stored back.
inline uint32_t nextSerial(uint32_t serialAddr) {
    uint32_t serial = ld16(serialAddr);
    do serial = (serial + 1) & 0xFFFF; while (serial == 0);
    return serial;
}

}  // namespace sqex::pool
