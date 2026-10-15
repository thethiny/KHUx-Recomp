// Generic bodies of the std::function __func thunks (libc++ 32-bit) for the lambda shapes the menu scenes use.
// A __func is [vtable][lambda captures...]; the captures start at +4, or at +8 when the lambda holds a
// std::function (8-aligned). The thunks of one lambda are the 9 vtable slots: ~__func, deleting ~__func,
// __clone() (heap copy), __clone(dst), destroy(), destroy_deallocate(), operator(), target(), target_type().
// The bodies below are parametrised by the vtable value stored in the object (the vtable symbol + 8).
#pragma once

#include "rw/rw.hpp"
#include "core/Guest.hpp"

namespace scene_menu {

constexpr uint32_t FUNCTION_VB_COPY = 0x45C84D;   // std::function<void(bool)>(const function&)
constexpr uint32_t FUNCTION_VV_COPY = 0x4683CD;   // std::function<void()>(const function&)

// ~std::function at fo (__f at +16): destroy() in place, destroy_deallocate() on the heap
inline void destroyFunctionAt(uint32_t fo) {
    uint32_t p = rw::load<uint32_t>(fo + 16);
    if (p == fo) rw::vcall(rw::host_ptr<uint8_t>(p), 4, {p});
    else if (p) rw::vcall(rw::host_ptr<uint8_t>(p), 5, {p});
}
inline void releaseAt(uint32_t ctrlAddr) {
    uint32_t c = rw::load<uint32_t>(ctrlAddr);
    if (c) guest::release(c);
}

// target(type_info): the lambda (at self + lambdaOffset) when the type_info's name pointer is the lambda's
inline uint32_t funcTargetAt(uint32_t self, uint32_t typeInfo, uint32_t typeName, uint32_t lambdaOffset) {
    return rw::load<uint32_t>(typeInfo + 4) == typeName ? self + lambdaOffset : 0;
}

// Lambda [w0, w1, w2, w3, shared_ptr, std::function<void(bool)>] in a 0x38-byte __func:
// words at +8..+0x14, shared_ptr at +0x18, function at +0x20.
template <uint32_t VTABLE> struct Words4SharedFnFunc {
    static constexpr uint32_t SIZE = 0x38, SP = 0x18, FN = 0x20;
    static void copyInto(uint32_t self, uint32_t dst) {
        rw::save<uint32_t>(dst, VTABLE);
        for (uint32_t k = 8; k < SP; k += 4) rw::save<uint32_t>(dst + k, rw::load<uint32_t>(self + k));
        rw::save<uint32_t>(dst + SP, rw::load<uint32_t>(self + SP));
        uint32_t ctrl = rw::load<uint32_t>(self + SP + 4);
        rw::save<uint32_t>(dst + SP + 4, ctrl);
        if (ctrl) guest::addRef(ctrl);
        rw::call(FUNCTION_VB_COPY, {dst + FN, self + FN});
    }
    static void destroyMembers(uint32_t self) {
        destroyFunctionAt(self + FN);
        releaseAt(self + SP + 4);
    }
    static uint32_t destroyFunc(uint32_t self) {
        rw::save<uint32_t>(self, VTABLE);
        destroyMembers(self);
        return self;
    }
    static void deleteFunc(uint32_t self) {
        rw::save<uint32_t>(self, VTABLE);
        destroyMembers(self);
        guest::deleteObject(self);
    }
    static uint32_t clone(uint32_t self) {
        uint32_t copy = guest::newObject(SIZE);
        if (copy) copyInto(self, copy);
        return copy;
    }
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (dst) copyInto(self, dst);
    }
    static void destroy(uint32_t self) { destroyMembers(self); }
    static void destroyDeallocate(uint32_t self) {
        destroyMembers(self);
        guest::deleteObject(self);
    }
};

}  // namespace scene_menu
