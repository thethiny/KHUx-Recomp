// std::__split_buffer push_back / push_front as libc++ 3.5 writes them, for buffers of 4-byte elements (pointers):
// the block maps of deques and the out-of-line __split_buffer members. Layout: first +0, begin +4, end +8, end of
// capacity +0xC, then the allocator: empty and at +0xC (the buffer's own) or a reference at +0x10.
#pragma once

#include "libcxx/Libcxx.hpp"

namespace libcxx::split {

using rw::load;
using rw::save;

constexpr uint32_t FIRST = 0, BEGIN = 4, END = 8, CAP = 0xC, ALLOC_REF = 0x10;
constexpr uint32_t AEABI_MEMMOVE = 0x40EF50;   // __aeabi_memmove(dst, src, n) (import)

// The buffer is full at the end we push to and has no room to slide into: a new buffer twice the capacity (at
// least 1 element), the elements moved in at `front ? (c + 3) / 4 : c / 4`, the buffers swapped, the old one freed
// by the temporary's destructor (TmpDtor: ~__split_buffer<T, allocator<T>&>).
template <bool AllocRef, uint32_t TmpDtor> inline void reallocate(uint32_t sb, bool front) {
    guest::Frame frame(20);
    const uint32_t t = frame.addr();
    save<uint32_t>(t + ALLOC_REF, AllocRef ? load<uint32_t>(sb + ALLOC_REF) : sb + CAP);
    uint32_t c = uint32_t(int32_t(load<uint32_t>(sb + CAP) - load<uint32_t>(sb + FIRST)) >> 1);
    if (!c) c = 1;
    const uint32_t first = guest::newObject(c * 4);
    const uint32_t start = first + ((front ? c + 3 : c) & ~3u);
    save<uint32_t>(t + FIRST, first);
    save<uint32_t>(t + BEGIN, start);
    save<uint32_t>(t + END, start);
    save<uint32_t>(t + CAP, first + c * 4);
    // __construct_at_end(move_iterator(begin), move_iterator(end))
    const uint32_t last = load<uint32_t>(sb + END);
    for (uint32_t p = load<uint32_t>(sb + BEGIN); p != last; p += 4) {
        if (const uint32_t slot = load<uint32_t>(t + END)) save<uint32_t>(slot, load<uint32_t>(p));
        save<uint32_t>(t + END, load<uint32_t>(t + END) + 4);
    }
    for (uint32_t k = 0; k < 16; k += 4) {   // swap first, begin, end, end of capacity
        const uint32_t mine = load<uint32_t>(sb + k);
        save<uint32_t>(sb + k, load<uint32_t>(t + k));
        save<uint32_t>(t + k, mine);
    }
    rw::call(TmpDtor, {t});
}

// push_back(x): at a full end, the elements slide towards the front half of the free space there, or the buffer
// grows; then x is stored at the end. value() gives x, read after any regrow (x's address may point into memory
// the regrow moves)
template <bool AllocRef, uint32_t TmpDtor, class Value> inline void pushBackWith(uint32_t sb, Value value) {
    if (load<uint32_t>(sb + END) == load<uint32_t>(sb + CAP)) {
        const uint32_t first = load<uint32_t>(sb + FIRST), begin = load<uint32_t>(sb + BEGIN);
        if (begin > first) {
            const int32_t d = ((int32_t(begin - first) >> 2) + 1) / 2;
            const uint32_t n = load<uint32_t>(sb + END) - begin;
            rw::call(AEABI_MEMMOVE, {begin - uint32_t(d) * 4, begin, n});
            save<uint32_t>(sb + END, begin + ((n >> 2) - uint32_t(d)) * 4);
            save<uint32_t>(sb + BEGIN, load<uint32_t>(sb + BEGIN) - uint32_t(d) * 4);
        } else {
            reallocate<AllocRef, TmpDtor>(sb, false);
        }
    }
    if (const uint32_t slot = load<uint32_t>(sb + END)) save<uint32_t>(slot, value());
    save<uint32_t>(sb + END, load<uint32_t>(sb + END) + 4);
}

// push_front(x): the mirror image, sliding towards the back half of the free space
template <bool AllocRef, uint32_t TmpDtor, class Value> inline void pushFrontWith(uint32_t sb, Value value) {
    if (load<uint32_t>(sb + BEGIN) == load<uint32_t>(sb + FIRST)) {
        const uint32_t begin = load<uint32_t>(sb + BEGIN), end = load<uint32_t>(sb + END);
        const uint32_t cap = load<uint32_t>(sb + CAP);
        if (end < cap) {
            const int32_t d = ((int32_t(cap - end) >> 2) + 1) / 2;
            const uint32_t n = end - begin;
            const uint32_t dst = end + (uint32_t(d) - (n >> 2)) * 4;
            rw::call(AEABI_MEMMOVE, {dst, begin, n});
            save<uint32_t>(sb + BEGIN, dst);
            save<uint32_t>(sb + END, load<uint32_t>(sb + END) + uint32_t(d) * 4);
        } else {
            reallocate<AllocRef, TmpDtor>(sb, true);
        }
    }
    const uint32_t begin = load<uint32_t>(sb + BEGIN);
    save<uint32_t>(begin - 4, value());
    save<uint32_t>(sb + BEGIN, load<uint32_t>(sb + BEGIN) - 4);
}

// with x the element's address / the element itself
template <bool AllocRef, uint32_t TmpDtor> inline void pushBack(uint32_t sb, uint32_t x) {
    pushBackWith<AllocRef, TmpDtor>(sb, [&] { return load<uint32_t>(x); });
}
template <bool AllocRef, uint32_t TmpDtor> inline void pushFront(uint32_t sb, uint32_t x) {
    pushFrontWith<AllocRef, TmpDtor>(sb, [&] { return load<uint32_t>(x); });
}
template <bool AllocRef, uint32_t TmpDtor> inline void pushBackValue(uint32_t sb, uint32_t x) {
    pushBackWith<AllocRef, TmpDtor>(sb, [&] { return x; });
}

}  // namespace libcxx::split
