// std::deque as the NDK's libc++ 3.5 compiles it, for the instances over the game's element types. Layout (32-bit):
// the block map, a __split_buffer<T*> (first +0, begin +4, end +8, end of capacity +0xC), the index of the first
// element +0x10, the size +0x14. Blocks hold BS elements of S bytes (4096 bytes or a little less). The members
// below are libc++'s; the map's own push_back / push_front are the instance's out-of-line __split_buffer members
// where the compiled code calls them and libcxx::split's inlined copies where it inlined them.
#pragma once

#include "libcxx/SplitBuffer.hpp"

namespace libcxx::deque {

using rw::load;
using rw::save;

constexpr uint32_t MAP_FIRST = 0, MAP_BEGIN = 4, MAP_END = 8, MAP_CAP = 0xC, START = 0x10, SIZE = 0x14;

// An instance policy P provides: S, BS (element size, elements per block); the addresses of its map operations
// MapPushBack / MapPushFront (__split_buffer<T*, allocator<T*>>::push_back / push_front(T*&&)), BufPushBack /
// BufPushFront / BufDtor (the __split_buffer<T*, allocator<T*>&> ones the map is regrown through), MapDtor
// (~__split_buffer<T*, allocator<T*>>), Clear (its own clear()); destroy(element).

template <class P> inline uint32_t blockBytes() { return P::S * P::BS; }
template <class P> inline uint32_t mapSize(uint32_t d) { return (load<uint32_t>(d + MAP_END) - load<uint32_t>(d + MAP_BEGIN)) / 4; }

// the element at index i (from the first block), or 0 when the map is empty
template <class P> inline uint32_t slotAt(uint32_t d, uint32_t i) {
    const uint32_t begin = load<uint32_t>(d + MAP_BEGIN);
    if (load<uint32_t>(d + MAP_END) == begin) return 0;
    return load<uint32_t>(begin + i / P::BS * 4) + i % P::BS * P::S;
}

// __back_spare(): free slots after the last element
template <class P> inline uint32_t backSpare(uint32_t d) {
    const uint32_t n = mapSize<P>(d);
    const uint32_t capacity = n ? n * P::BS - 1 : 0;
    return capacity - load<uint32_t>(d + START) - load<uint32_t>(d + SIZE);
}

// __base::clear(): every element destroyed, the map trimmed to two blocks, the start recentred
template <class P> void clear(uint32_t d) {
    const uint32_t start = load<uint32_t>(d + START);
    uint32_t mp = load<uint32_t>(d + MAP_BEGIN) + start / P::BS * 4;
    uint32_t p = 0, e = 0;
    if (load<uint32_t>(d + MAP_END) != load<uint32_t>(d + MAP_BEGIN)) {
        const uint32_t last = start + load<uint32_t>(d + SIZE);
        e = load<uint32_t>(load<uint32_t>(d + MAP_BEGIN) + last / P::BS * 4) + last % P::BS * P::S;
        p = load<uint32_t>(mp) + start % P::BS * P::S;
    }
    while (p != e) {
        P::destroy(p);
        p += P::S;
        if (p - load<uint32_t>(mp) == blockBytes<P>()) {
            mp += 4;
            p = load<uint32_t>(mp);
        }
    }
    save<uint32_t>(d + SIZE, 0);
    while (mapSize<P>(d) > 2) {
        guest::deleteObject(load<uint32_t>(load<uint32_t>(d + MAP_BEGIN)));
        save<uint32_t>(d + MAP_BEGIN, load<uint32_t>(d + MAP_BEGIN) + 4);
    }
    const uint32_t n = mapSize<P>(d);
    if (n == 2) save<uint32_t>(d + START, P::BS);
    else if (n == 1) save<uint32_t>(d + START, P::BS / 2);
}

// ~__deque_base(): clear(), every block freed, then the map. Returns this.
template <class P> uint32_t destroyBase(uint32_t d) {
    rw::call(P::Clear, {d});
    const uint32_t end = load<uint32_t>(d + MAP_END);
    for (uint32_t i = load<uint32_t>(d + MAP_BEGIN); i != end; i += 4) guest::deleteObject(load<uint32_t>(i));
    rw::call(P::MapDtor, {d});
    return d;
}

// the map's push_back(pt), inlined (its own allocator; regrown through a temporary freed by BufDtor)
template <class P> void mapPushBackInline(uint32_t d, uint32_t pt) { split::pushBackValue<false, P::BufDtor>(d, pt); }

// __add_back_capacity(): a spare front block rotated to the back, else a new block added to the map, else the map
// regrown (twice the capacity) with the new block at its end
template <class P> void addBackCapacity(uint32_t d) {
    if (load<uint32_t>(d + START) >= P::BS) {
        save<uint32_t>(d + START, load<uint32_t>(d + START) - P::BS);
        const uint32_t pt = load<uint32_t>(load<uint32_t>(d + MAP_BEGIN));
        save<uint32_t>(d + MAP_BEGIN, load<uint32_t>(d + MAP_BEGIN) + 4);
        mapPushBackInline<P>(d, pt);
        return;
    }
    const uint32_t first = load<uint32_t>(d + MAP_FIRST), cap = load<uint32_t>(d + MAP_CAP);
    if (mapSize<P>(d) < (cap - first) / 4) {
        guest::Frame tmp(4);
        save<uint32_t>(tmp.addr(), guest::newObject(blockBytes<P>()));
        if (load<uint32_t>(d + MAP_CAP) != load<uint32_t>(d + MAP_END)) {
            rw::call(P::MapPushBack, {d, tmp.addr()});
        } else {
            rw::call(P::MapPushFront, {d, tmp.addr()});
            const uint32_t pt = load<uint32_t>(load<uint32_t>(d + MAP_BEGIN));
            save<uint32_t>(d + MAP_BEGIN, load<uint32_t>(d + MAP_BEGIN) + 4);
            mapPushBackInline<P>(d, pt);
        }
        return;
    }
    guest::Frame frame(24);   // the new block, then the new map (a __split_buffer<T*, allocator<T*>&>)
    const uint32_t hold = frame.addr(0), buf = frame.addr(4);
    save<uint32_t>(buf + split::ALLOC_REF, d + MAP_CAP);
    uint32_t c = (cap - first) / 4 * 2;
    if (!c) c = 1;
    const uint32_t size = mapSize<P>(d);
    const uint32_t bufFirst = guest::newObject(c * 4);
    save<uint32_t>(buf + split::FIRST, bufFirst);
    save<uint32_t>(buf + split::BEGIN, bufFirst + size * 4);
    save<uint32_t>(buf + split::END, bufFirst + size * 4);
    save<uint32_t>(buf + split::CAP, bufFirst + c * 4);
    save<uint32_t>(hold, guest::newObject(blockBytes<P>()));
    rw::call(P::BufPushBack, {buf, hold});
    for (uint32_t i = load<uint32_t>(d + MAP_END); i != load<uint32_t>(d + MAP_BEGIN);) {
        i -= 4;
        rw::call(P::BufPushFront, {buf, i});
    }
    for (uint32_t k = 0; k < 16; k += 4) {
        const uint32_t mine = load<uint32_t>(d + k);
        save<uint32_t>(d + k, load<uint32_t>(buf + k));
        save<uint32_t>(buf + k, mine);
    }
    rw::call(P::BufDtor, {buf});
}

// push_back(x): construct(slot, x) at the end (no element when the map is still empty)
template <class P, void (*Construct)(uint32_t, uint32_t)> void pushBack(uint32_t d, uint32_t x) {
    if (!backSpare<P>(d)) rw::call(P::AddBack, {d});
    if (const uint32_t slot = slotAt<P>(d, load<uint32_t>(d + START) + load<uint32_t>(d + SIZE))) Construct(slot, x);
    save<uint32_t>(d + SIZE, load<uint32_t>(d + SIZE) + 1);
}

// pop_front(): the first element destroyed; a block emptied twice over is freed
template <class P> void popFront(uint32_t d) {
    const uint32_t start = load<uint32_t>(d + START);
    P::destroy(load<uint32_t>(load<uint32_t>(d + MAP_BEGIN) + start / P::BS * 4) + start % P::BS * P::S);
    save<uint32_t>(d + SIZE, load<uint32_t>(d + SIZE) - 1);
    save<uint32_t>(d + START, load<uint32_t>(d + START) + 1);
    if (load<uint32_t>(d + START) >= 2 * P::BS) {
        guest::deleteObject(load<uint32_t>(load<uint32_t>(d + MAP_BEGIN)));
        save<uint32_t>(d + MAP_BEGIN, load<uint32_t>(d + MAP_BEGIN) + 4);
        save<uint32_t>(d + START, load<uint32_t>(d + START) - P::BS);
    }
}

// ------------------------------------------------------------------------------------------ iterators
// A __deque_iterator: {map entry, element}. operator+=(n), libc++'s (n may be negative)
template <class P> struct Iter {
    uint32_t m, p;
    void advance(int32_t n) {
        if (!n) return;
        n += int32_t(p - load<uint32_t>(m)) / int32_t(P::S);
        if (n > 0) {
            m += uint32_t(n / int32_t(P::BS)) * 4;
            p = load<uint32_t>(m) + uint32_t(n % int32_t(P::BS)) * P::S;
        } else {
            const int32_t z = int32_t(P::BS) - 1 - n;
            m -= uint32_t(z / int32_t(P::BS)) * 4;
            p = load<uint32_t>(m) + uint32_t(int32_t(P::BS) - 1 - z % int32_t(P::BS)) * P::S;
        }
    }
    // operator-(other): elements between (0 when the elements are the same)
    int32_t minus(const Iter &o) const {
        if (p == o.p) return 0;
        return int32_t(m - o.m) / 4 * int32_t(P::BS) + int32_t(p - load<uint32_t>(m)) / int32_t(P::S) -
               int32_t(o.p - load<uint32_t>(o.m)) / int32_t(P::S);
    }
};

template <class P> inline Iter<P> beginIter(uint32_t d) {
    const uint32_t start = load<uint32_t>(d + START);
    Iter<P> it{load<uint32_t>(d + MAP_BEGIN) + start / P::BS * 4, 0};
    if (load<uint32_t>(d + MAP_END) != load<uint32_t>(d + MAP_BEGIN)) it.p = load<uint32_t>(it.m) + start % P::BS * P::S;
    return it;
}

}  // namespace libcxx::deque
