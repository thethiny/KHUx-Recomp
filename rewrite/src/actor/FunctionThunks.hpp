// The std::function plumbing the compiler generates for every lambda stored in a std::function (libc++
// std::__function::__func<Lambda, Alloc, R(Args...)>). Its vtable is
//   [0] ~__func (shared empty function)   [1] ~__func + operator delete    [2] __clone() (new copy)
//   [3] __clone(__base* p) (copy into p)  [4] destroy() (empty)            [5] destroy_deallocate() (delete)
//   [6] operator()(Args...) (the lambda)  [7] target(const type_info&)     [8] target_type()
// and the object is the vtable followed by the lambda's captures. These templates reproduce the thunks for
// lambdas with no captures (8-byte object, one unused word) or with captured words; each thunk is registered at
// its own address (RW_FUNCTION with the template instantiated on the lambda's __func vtable / type name).
#pragma once

#include "MapNode.hpp"

namespace fnthunk {

// [1] / [5]: operator delete(this)
inline void deleteThis(uint32_t self) { rw::call(bt::fn::operator_delete, {self}); }
// [4] destroy() of a trivially destructible lambda
inline void nothing() {}

// [2] __clone() for an empty lambda: new __func (8 bytes), vtable only.
template <uint32_t VTABLE> uint32_t clone0() {
    uint32_t p = rw::call(bt::fn::operator_new, {8});
    if (p) rw::save<uint32_t>(p, VTABLE);
    return p;
}
// [2] __clone() for a lambda capturing N words (object = 4 + 4 * N bytes, rounded to 8 by the compiler: SIZE).
template <uint32_t VTABLE, uint32_t SIZE, uint32_t N> uint32_t cloneWords(uint32_t self) {
    uint32_t p = rw::call(bt::fn::operator_new, {SIZE});
    if (p) {
        rw::save<uint32_t>(p, VTABLE);
        for (uint32_t k = 1; k <= N; k++) rw::save<uint32_t>(p + 4 * k, rw::load<uint32_t>(self + 4 * k));
    }
    return p;
}
// [3] __clone(__base* dst) for an empty lambda. r0 is the vtable when dst is set, else this (untouched).
template <uint32_t VTABLE> uint32_t cloneInto0(uint32_t self, uint32_t dst) {
    if (!dst) return self;
    rw::save<uint32_t>(dst, VTABLE);
    return VTABLE;
}
// [3] __clone(__base* dst) for a lambda capturing one word. r0 is the copied word when dst is set.
template <uint32_t VTABLE> uint32_t cloneInto1(uint32_t self, uint32_t dst) {
    if (!dst) return self;
    rw::save<uint32_t>(dst, VTABLE);
    uint32_t w = rw::load<uint32_t>(self + 4);
    rw::save<uint32_t>(dst + 4, w);
    return w;
}
// [3] __clone(__base* dst) for a lambda capturing N words. r0 is the last copied word when dst is set.
template <uint32_t VTABLE, uint32_t N> uint32_t cloneIntoWords(uint32_t self, uint32_t dst) {
    if (!dst) return self;
    rw::save<uint32_t>(dst, VTABLE);
    uint32_t w = 0;
    for (uint32_t k = 1; k <= N; k++) {
        w = rw::load<uint32_t>(self + 4 * k);
        rw::save<uint32_t>(dst + 4 * k, w);
    }
    return w;
}
// [7] target(const type_info& ti): the lambda (this + 4) when ti's name pointer is the lambda's type name.
template <uint32_t TYPE_NAME> uint32_t target(uint32_t self, uint32_t typeInfo) {
    return rw::load<uint32_t>(typeInfo + 4) == TYPE_NAME ? self + 4 : 0;
}
// [8] target_type(): the lambda's typeinfo
template <uint32_t TYPE_INFO> uint32_t targetType() { return TYPE_INFO; }

// A std::function temporary on the guest stack holding a small lambda in place: buffer = __func (vtable +
// captures), callable pointer (+16) = the buffer. `at` must have 20 bytes.
inline void makeLocal(uint32_t at, uint32_t vtable, std::initializer_list<uint32_t> captures = {}) {
    rw::save<uint32_t>(at, vtable);
    uint32_t k = 4;
    for (uint32_t c : captures) { rw::save<uint32_t>(at + k, c); k += 4; }
    rw::save<uint32_t>(at + 16, at);
}
// The inlined std::function destructor: destroy() in place, or destroy_deallocate() of a heap functor.
inline void destroy(uint32_t at) {
    uint32_t f = rw::load<uint32_t>(at + 16);
    if (f == at) rw::call(rw::load<uint32_t>(rw::load<uint32_t>(at) + 16), {at});
    else if (f) rw::call(rw::load<uint32_t>(rw::load<uint32_t>(f) + 20), {f});
}

}  // namespace fnthunk
