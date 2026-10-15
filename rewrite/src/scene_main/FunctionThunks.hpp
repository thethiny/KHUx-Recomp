// The std::function plumbing of the scene lambdas. Every lambda stored in a std::function becomes a libc++
// __function::__func<Lambda, allocator, Signature> object: a vtable followed by the lambda's captures. Its vtable
// slots are ~__func (complete / deleting), __clone() (heap copy), __clone(dst) (copy into a std::function's inline
// buffer), destroy(), destroy_deallocate(), operator() (the lambda body), target(type_info) and target_type().
//
// Lambda<...> implements the mechanical slots for a lambda whose captures are plain values (pointers, ints, bools):
// a capture is described by cap(offset, size) (byte offset inside the __func object, size 1, 2 or 4), and only
// those bytes are copied - the compiler copies exactly the captured fields, padding stays untouched.
// LambdaFn<...> is the same for a lambda that also captures std::function objects (copied with the copy
// constructor of their signature, destroyed inline the way libc++ does).
#pragma once

#include "../core/Guest.hpp"

namespace thunk {

constexpr uint32_t cap(uint32_t offset, uint32_t size) { return offset << 8 | size; }

// Copy the captured bytes `caps` from one __func object to another.
template <uint32_t... CAPS> inline void copyCaptures(uint32_t dst, uint32_t src) {
    const uint32_t caps[] = {CAPS..., 0};
    for (uint32_t k = 0; caps[k]; k++) {
        uint32_t off = caps[k] >> 8, size = caps[k] & 0xFF;
        if (size == 4) rw::save<uint32_t>(dst + off, rw::load<uint32_t>(src + off));
        else if (size == 2) rw::save<uint16_t>(dst + off, rw::load<uint16_t>(src + off));
        else rw::save<uint8_t>(dst + off, rw::load<uint8_t>(src + off));
    }
}

// The inline libc++ ~function(): the callable lives in the inline buffer (destroy()) or on the heap
// (destroy_deallocate()), or there is none.
inline void destroyFunction(uint32_t f) {
    uint32_t p = rw::load<uint32_t>(f + 16);
    if (p == f) rw::call(rw::load<uint32_t>(rw::load<uint32_t>(p) + 0x10), {p});
    else if (p) rw::call(rw::load<uint32_t>(rw::load<uint32_t>(p) + 0x14), {p});
}

// VTABLE: the value stored at +0 (vtable + 8); SIZE: sizeof(__func); TYPEINFO: the lambda's type_info (returned by
// target_type); TYPENAME: its name pointer (target() compares type_info names by address); TARGET: offset of the
// lambda inside the __func (4, or 8 when the lambda needs 8-byte alignment).
template <uint32_t VTABLE, uint32_t SIZE, uint32_t TYPEINFO, uint32_t TYPENAME, uint32_t TARGET, uint32_t... CAPS>
struct Lambda {
    // __clone(): a heap copy (operator new; nothing is stored when it returns null)
    static uint32_t clone(uint32_t self) {
        uint32_t obj = guest::newObject(SIZE);
        if (obj) {
            rw::save<uint32_t>(obj, VTABLE);
            copyCaptures<CAPS...>(obj, self);
        }
        return obj;
    }
    // __clone(dst): placement copy into a std::function's buffer
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (!dst) return;
        rw::save<uint32_t>(dst, VTABLE);
        copyCaptures<CAPS...>(dst, self);
    }
    // ~__func(): the captures need no destruction
    static uint32_t destructor(uint32_t self) { return self; }
    // deleting ~__func() / destroy_deallocate(): free the heap object
    static void deleteThis(uint32_t self) { guest::deleteObject(self); }
    // destroy(): nothing to destroy
    static void destroy(uint32_t) {}
    // target(type): the lambda when the type matches
    static uint32_t target(uint32_t self, uint32_t type) {
        return rw::load<uint32_t>(type + 4) == TYPENAME ? self + TARGET : 0;
    }
    static uint32_t targetType(uint32_t) { return TYPEINFO; }
};

// A lambda capturing one std::function at FN (copied with FN_COPY, the copy constructor of its signature) plus
// plain values CAPS.
template <uint32_t VTABLE, uint32_t SIZE, uint32_t TYPEINFO, uint32_t TYPENAME, uint32_t TARGET, uint32_t FN,
          uint32_t FN_COPY, uint32_t... CAPS>
struct LambdaFn {
    static uint32_t clone(uint32_t self) {
        uint32_t obj = guest::newObject(SIZE);
        if (obj) {
            rw::save<uint32_t>(obj, VTABLE);
            copyCaptures<CAPS...>(obj, self);
            rw::call(FN_COPY, {obj + FN, self + FN});
        }
        return obj;
    }
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (!dst) return;
        rw::save<uint32_t>(dst, VTABLE);
        copyCaptures<CAPS...>(dst, self);
        rw::call(FN_COPY, {dst + FN, self + FN});
    }
    // ~__func(): resets the vtable, then destroys the captured std::function
    static uint32_t destructor(uint32_t self) {
        rw::save<uint32_t>(self, VTABLE);
        destroyFunction(self + FN);
        return self;
    }
    static void destructorDelete(uint32_t self) {
        destructor(self);
        guest::deleteObject(self);
    }
    static void destroy(uint32_t self) { destroyFunction(self + FN); }
    static void destroyDeallocate(uint32_t self) {
        destroyFunction(self + FN);
        guest::deleteObject(self);
    }
    static uint32_t target(uint32_t self, uint32_t type) {
        return rw::load<uint32_t>(type + 4) == TYPENAME ? self + TARGET : 0;
    }
    static uint32_t targetType(uint32_t) { return TYPEINFO; }
};

}  // namespace thunk
