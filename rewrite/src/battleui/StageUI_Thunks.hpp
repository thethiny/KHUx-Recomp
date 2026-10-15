// The fixed shapes of libc++ std::function thunks (std::__function::__func<Lambda, allocator, Sig>) as the
// compiler emitted them for the lambdas of StageUI, SceneBattleBase, LuxNumber, DamageNumber, BattleMarker...
// Vtable layout (byte offsets): +0x00 ~__func, +0x04 deleting ~__func, +0x08 __clone() (heap copy),
// +0x0C __clone(dst) (copy into dst), +0x10 destroy() (lambda destructor, in place), +0x14 destroy_deallocate()
// (destroy and free), +0x18 operator(), +0x1C target(type_info), +0x20 target_type().
// The lambda's captures follow the vtable pointer (+4...). Each thunk is a template over the constants the
// compiler baked in (the __func vtable, the lambda's type name, its type_info) and is registered at its address.
#pragma once

#include "BattleUi.hpp"

namespace thunk {

// deleting destructor / destroy_deallocate of a lambda with trivially destructible captures
inline void deleteThis(uint32_t self) { rw::call(guest::OPERATOR_DELETE, {self}); }
// destroy() of a lambda with trivially destructible captures
inline void nop(uint32_t) {}

// __clone(): a heap copy of a lambda capturing `words` 32-bit values (null when the allocation failed)
template <uint32_t VT, uint32_t WORDS> uint32_t clone(uint32_t self) {
    uint32_t p = rw::call(guest::OPERATOR_NEW, {4 + 4 * WORDS});
    if (p) {
        rw::save<uint32_t>(p, VT);
        for (uint32_t k = 1; k <= WORDS; k++) rw::save<uint32_t>(p + 4 * k, rw::load<uint32_t>(self + 4 * k));
    }
    return p;
}

// __clone(dst): copy into dst (when not null)
template <uint32_t VT, uint32_t WORDS> void cloneTo(uint32_t self, uint32_t dst) {
    if (!dst) return;
    rw::save<uint32_t>(dst, VT);
    for (uint32_t k = 1; k <= WORDS; k++) rw::save<uint32_t>(dst + 4 * k, rw::load<uint32_t>(self + 4 * k));
}

// one- and two-word captures (separate names: RW_FUNCTION cannot take a template-id with a comma)
template <uint32_t VT> uint32_t clone1(uint32_t self) { return clone<VT, 1>(self); }
template <uint32_t VT> uint32_t clone2(uint32_t self) { return clone<VT, 2>(self); }
template <uint32_t VT> void cloneTo1(uint32_t self, uint32_t dst) { cloneTo<VT, 1>(self, dst); }
template <uint32_t VT> void cloneTo2(uint32_t self, uint32_t dst) { cloneTo<VT, 2>(self, dst); }

// a lambda without captures (the __func is still 8 bytes; only the vtable is written)
template <uint32_t VT> uint32_t cloneEmpty(uint32_t) {
    uint32_t p = rw::call(guest::OPERATOR_NEW, {8});
    if (p) rw::save<uint32_t>(p, VT);
    return p;
}
template <uint32_t VT> void cloneToEmpty(uint32_t, uint32_t dst) {
    if (dst) rw::save<uint32_t>(dst, VT);
}

// target(type_info): the stored lambda when the type's name pointer is this lambda's
template <uint32_t NAME> uint32_t target(uint32_t self, uint32_t typeInfo) {
    return rw::load<uint32_t>(typeInfo + 4) == NAME ? self + 4 : 0;
}

// target_type(): the lambda's type_info
template <uint32_t TYPEINFO> uint32_t targetType() { return TYPEINFO; }

}  // namespace thunk
