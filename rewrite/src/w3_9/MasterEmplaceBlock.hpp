// The control block master::X::create() allocates with make_shared: std::__shared_ptr_emplace<X::create(int)::impl>
// (a __shared_weak_count followed by the row, a SafeBuffer whose vtable is the impl's). Every master table has the
// same five compiler-generated functions for it:
//   ~__shared_ptr_emplace()            restore the block vtable, destroy the row, ~__shared_weak_count (empty)
//   ~__shared_ptr_emplace() (deleting) the same + operator delete
//   __on_zero_shared()                 the last owner went away: the row's virtual destructor (vtable slot 0)
//   __on_zero_shared_weak()            the last weak reference went away: operator delete of the block
//   impl::~impl() (deleting)           SafeBuffer destructor + operator delete
// EmplaceBlockFunctions<T> implements them once for a table T (T::CREATE_BLOCK_VTABLE).
#pragma once

#include "core/Guest.hpp"
#include "core/SafeBuffer.hpp"

namespace master {

constexpr uint32_t SAFEBUFFER_DTOR = 0x45A13D;            // SafeBuffer::~SafeBuffer() (returns this)
constexpr uint32_t SHARED_WEAK_COUNT_DTOR = 0xC8DB69;     // std::__shared_weak_count::~__shared_weak_count (nullsub_4)

struct EmplaceBlock {
    rw::Field<uint32_t> vtable;       // +0
    rw::Field<uint32_t> sharedOwners; // +4  owners - 1
    rw::Field<uint32_t> weakOwners;   // +8  weak references - 1
    SafeBuffer row;                   // +12 the emplaced create(int)::impl
};
RW_LAYOUT(EmplaceBlock, row, 12);
RW_SIZE(EmplaceBlock, 44);

template <class T>
struct EmplaceBlockFunctions {
    static EmplaceBlock *destruct(EmplaceBlock *self) {
        self->vtable = T::CREATE_BLOCK_VTABLE;
        rw::call(SAFEBUFFER_DTOR, {rw::guest_addr(&self->row)});
        rw::call(SHARED_WEAK_COUNT_DTOR, {rw::guest_addr(self)});
        return self;
    }
    static void destructDelete(EmplaceBlock *self) {
        self->vtable = T::CREATE_BLOCK_VTABLE;
        rw::call(SAFEBUFFER_DTOR, {rw::guest_addr(&self->row)});
        rw::call(SHARED_WEAK_COUNT_DTOR, {rw::guest_addr(self)});
        guest::deleteObject(rw::guest_addr(self));
    }
    // the row's virtual destructor (slot 0: the complete-object destructor); r0 is whatever it returned
    static uint32_t onZeroShared(EmplaceBlock *self) {
        return rw::vcall(&self->row, 0, {rw::guest_addr(&self->row)});
    }
    static void onZeroSharedWeak(EmplaceBlock *self) { guest::deleteObject(rw::guest_addr(self)); }
    // create(int)::impl deleting destructor: the pointer deleted is the one ~SafeBuffer returned
    static void rowDestructDelete(SafeBuffer *self) {
        guest::deleteObject(rw::call(SAFEBUFFER_DTOR, {rw::guest_addr(self)}));
    }
};

}  // namespace master
