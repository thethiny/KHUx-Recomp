// std::function plumbing for the lambdas of the system module (libc++ std::__function::__func<Lambda, Alloc,
// R(Args...)>). The __func vtable is
//   [0] ~__func   [1] ~__func + operator delete   [2] __clone() (heap copy)   [3] __clone(__base* p) (copy into p)
//   [4] destroy() [5] destroy_deallocate()         [6] operator()(Args...)     [7] target(type_info&)  [8] target_type()
// and the object is the vtable followed by the lambda's captures. These templates are the thunks of lambdas whose
// captures are plain words; each is registered at its own address. Void thunks compare no return register.
#pragma once

#include "SystemCommon.hpp"

namespace sysfn {

// [1] / [5]: operator delete(this)
inline void deleteThis(uint32_t self) { guest::deleteObject(self); }
// [0] / [4]: nothing to destroy
inline void nothing() {}

// [2] __clone(): operator new(SIZE), the vtable, then the captured words at the byte offsets OFFS... (copied in
// the order given).
template <uint32_t VTABLE, uint32_t SIZE, uint32_t... OFFS> uint32_t clone(uint32_t self) {
    uint32_t p = guest::newObject(SIZE);
    if (p) {
        rw::save<uint32_t>(p, VTABLE);
        ((rw::save<uint32_t>(p + OFFS, rw::load<uint32_t>(self + OFFS))), ...);
    }
    return p;
}
// [3] __clone(__base* dst): the vtable and the captured words into dst (when dst is set).
template <uint32_t VTABLE, uint32_t... OFFS> void cloneInto(uint32_t self, uint32_t dst) {
    if (!dst) return;
    rw::save<uint32_t>(dst, VTABLE);
    ((rw::save<uint32_t>(dst + OFFS, rw::load<uint32_t>(self + OFFS))), ...);
}
// [7] target(const type_info& ti): the lambda (this + 4) when ti's name pointer is the lambda's type name.
template <uint32_t TYPE_NAME> uint32_t target(uint32_t self, uint32_t typeInfo) {
    return rw::load<uint32_t>(typeInfo + 4) == TYPE_NAME ? self + 4 : 0;
}
// [8] target_type(): the lambda's typeinfo
template <uint32_t TYPE_INFO> uint32_t targetType() { return TYPE_INFO; }

// A std::function on the guest stack holding a small lambda in place (buffer = __func: vtable + captures; the
// callable pointer at +16 = the buffer). `at` must have 20 bytes.
inline void makeLocal(uint32_t at, uint32_t vtable, std::initializer_list<uint32_t> captures = {}) {
    rw::save<uint32_t>(at, vtable);
    uint32_t k = 4;
    for (uint32_t c : captures) { rw::save<uint32_t>(at + k, c); k += 4; }
    rw::save<uint32_t>(at + 16, at);
}
// The inlined ~std::function: destroy() in place, or destroy_deallocate() of a heap functor.
inline void destroy(uint32_t at) {
    uint32_t f = rw::load<uint32_t>(at + 16);
    if (f == at) rw::call(rw::load<uint32_t>(rw::load<uint32_t>(at) + 16), {at});
    else if (f) rw::call(rw::load<uint32_t>(rw::load<uint32_t>(f) + 20), {f});
}

}  // namespace sysfn
