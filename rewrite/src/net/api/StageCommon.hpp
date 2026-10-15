// Shared pieces of the stage-flow response parsers (Stage, StageEvent, StageSupportList, StageStart, StageClear):
// the library functions they call and the small libc++ idioms they inline (vector<int>::push_back, shared_ptr
// assignment / move).
#pragma once

#include "Common.hpp"

namespace hole::network::api::stage {

// ---------------------------------------------------------------------------------------------- guest functions
namespace fn {
constexpr uint32_t aeabi_memset = 0x40E7A0;        // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t memcpy_ = 0x40E164;             // memcpy (import)
constexpr uint32_t strncpy_ = 0x410E4C;            // strncpy (import)
constexpr uint32_t add_shared = 0xC8DBB5;          // std::__shared_weak_count::__add_shared
constexpr uint32_t release_shared = 0xC8DBD1;      // std::__shared_weak_count::__release_shared
constexpr uint32_t string_init = 0x47CD93;         // std::string::__init(const char*, size_t) (copy of a long string)
constexpr uint32_t vector_int_push_slow = 0x47D709;   // std::vector<int>::__push_back_slow_path<int>(int&&)
constexpr uint32_t vector_int_copy = 0x4AF707;        // std::vector<int>::vector(const vector&)
constexpr uint32_t vector_int_base_dtor = 0x478961;   // std::__vector_base<int>::~__vector_base
constexpr uint32_t date_init = 0x4924F1;              // Date::init(const char*, bool)
constexpr uint32_t map_insert_int = 0x4B176D;  // unordered_map<unsigned long long, int>::__insert_unique(pair<ull,int>&&)
constexpr uint32_t map_dtor = 0x4B09C1;        // ~__hash_table<unsigned long long, int>
}  // namespace fn

// ---------------------------------------------------------------------------------------------- helpers
inline uint32_t addr(const void *p) { return rw::guest_addr(p); }

// vector<int>::push_back(value) as inlined by the parsers: the value is first stored in a guest-stack local (whose
// address the slow path takes); in place when there is room (a null end pointer skips the store but still
// advances by 4), else __push_back_slow_path.
inline void pushInt(StdVector *vec, uint32_t local, uint32_t value) {
    rw::save<uint32_t>(local, value);
    uint32_t end = vec->end;
    if (end < vec->capEnd) {
        uint32_t base = 0;
        if (end) {
            rw::save<uint32_t>(end, value);
            base = vec->end;
        }
        vec->end = base + 4;
    } else {
        rw::call(fn::vector_int_push_slow, {addr(vec), local});
    }
}

// shared_ptr<T>& operator=(const shared_ptr<T>&) as inlined: add a reference to the new control block, store,
// then release the old control block.
inline void assignShared(SharedPtr *dst, uint32_t ptr, uint32_t ctrl) {
    if (ctrl) rw::call(fn::add_shared, {ctrl});
    dst->ptr = ptr;
    uint32_t old = dst->ctrl;
    dst->ctrl = ctrl;
    if (old) rw::call(fn::release_shared, {old});
}

// shared_ptr<T>& operator=(shared_ptr<T>&&) as inlined: the temporary is emptied first, then the old control
// block released (and the temporary's, which is null by then).
inline void moveShared(SharedPtr *dst, SharedPtr *tmp) {
    uint32_t ptr = tmp->ptr, ctrl = tmp->ctrl;
    tmp->ptr = 0;
    tmp->ctrl = 0;
    dst->ptr = ptr;
    uint32_t old = dst->ctrl;
    dst->ctrl = ctrl;
    if (old) {
        rw::call(fn::release_shared, {old});
        if (tmp->ctrl) rw::call(fn::release_shared, {tmp->ctrl});
    }
}

}  // namespace hole::network::api::stage
