// libc++ templates instantiated over the game's types (std::vector<Avatar::Expression>, std::shared_ptr<...>,
// std::function<void(bool)>, ...). They are library code, but the compiler generated them for the game's own types,
// so no upstream library has them: a native build needs our rewrites of them. The helpers here are the inline pieces
// those instances share (member destructors, the vector's own bookkeeping), written as the 32-bit NDK libc++ does them.
#pragma once

#include "core/Guest.hpp"

namespace libcxx {

// ~basic_string: a long string (byte0 bit 0 set) frees its heap buffer (data pointer at +8).
inline void destroyString(uint32_t s) {
    if (rw::load<uint8_t>(s) & 1) guest::deleteObject(rw::load<uint32_t>(s + 8));
}

// ~shared_ptr: drops one owner of the control block (+4), if any.
inline void destroyShared(uint32_t p) {
    if (uint32_t ctrl = rw::load<uint32_t>(p + 4)) guest::release(ctrl);
}

// ~function: the callable (__f_ at +16) lives in the 16-byte buffer (destroy, slot 4) or on the heap
// (destroy_deallocate, slot 5).
inline void destroyFunction(uint32_t f) {
    uint32_t impl = rw::load<uint32_t>(f + 16);
    if (impl == f) rw::vcall(rw::host_ptr<uint8_t>(impl), 4, {impl});
    else if (impl) rw::vcall(rw::host_ptr<uint8_t>(impl), 5, {impl});
}

// __destruct_at_end(begin) of a vector of trivially destructible elements of Size bytes: one store of end, as
// libc++ computes it (end - ((end - begin) / Size) * Size, written as the compiler folds it).
template <uint32_t Size> inline void clearTrivial(guest::Vector *v) {
    uint32_t begin = v->begin, end = v->end;
    if (end != begin) v->end = end + ~((end - Size - begin) / Size) * Size;
}

// ~__vector_base of trivially destructible elements: forget them, free the buffer. Returns this.
template <uint32_t Size> inline uint32_t destroyVector(guest::Vector *v) {
    if (v->begin) {
        clearTrivial<Size>(v);
        guest::deleteObject(v->begin);
    }
    return rw::guest_addr(v);
}

// ~__vector_base of elements with a destructor: from the back, end steps down one element (stored each time), then
// the element at the new end is destroyed; then the buffer is freed. Returns this.
template <uint32_t Size, class Destroy> inline uint32_t destroyVector(guest::Vector *v, Destroy destroyElement) {
    if (v->begin) {
        while (v->end != v->begin) {
            uint32_t e = v->end - Size;
            v->end = e;
            destroyElement(e);
        }
        guest::deleteObject(v->begin);
    }
    return rw::guest_addr(v);
}

// std::make_shared<T>(args...): one allocation of `size` bytes - the control block __shared_ptr_emplace<T> (vtable,
// owners - 1, weak owners - 1) with the T after it at objOffset (12, or 16 when T needs 8-byte alignment). The T's
// constructor, inlined by the compiler, is construct(block, obj). The result gets the T and the block (a null
// block would give the T's offset as its address, as the compiled code does).
template <class Construct>
inline void makeShared(guest::SharedPtr *result, uint32_t size, uint32_t vtable, uint32_t objOffset, Construct construct) {
    uint32_t block = guest::newObject(size);
    if (block) {
        rw::save<uint32_t>(block + 4, 0);
        rw::save<uint32_t>(block + 8, 0);
        rw::save<uint32_t>(block, vtable);
        construct(block, block + objOffset);
    }
    result->obj = block + objOffset;
    result->ctrl = block;
}

// ---------------------------------------------------------------------------------------------- copies
constexpr uint32_t AEABI_MEMCPY = 0x40E164;          // __aeabi_memcpy(dest, src, n) (import)
constexpr uint32_t STRING_INIT = 0x47CD93;           // basic_string::__init(const char*, size_t) (sub_47CD92)
constexpr uint32_t THROW_LENGTH_ERROR = 0x47B66D;    // __vector_base_common<true>::__throw_length_error (sub_47B66C)

inline void copyBytes(uint32_t dst, uint32_t src, uint32_t n) { rw::call(AEABI_MEMCPY, {dst, src, n}); }
inline void copyWords(uint32_t dst, uint32_t src, uint32_t bytes) {
    for (uint32_t k = 0; k < bytes; k += 4) rw::save<uint32_t>(dst + k, rw::load<uint32_t>(src + k));
}
// one trivially copyable element of Size bytes, copied as the compiled code does: a byte, a halfword or words
template <uint32_t Size> inline void copyElement(uint32_t dst, uint32_t src) {
    if constexpr (Size == 1) rw::save<uint8_t>(dst, rw::load<uint8_t>(src));
    else if constexpr (Size == 2) rw::save<uint16_t>(dst, rw::load<uint16_t>(src));
    else copyWords(dst, src, Size);
}
// the 3 words of a moved std::string / std::vector: copied, then the source emptied
inline void moveTriple(uint32_t dst, uint32_t src) {
    copyWords(dst, src, 12);
    for (uint32_t k = 0; k < 12; k += 4) rw::save<uint32_t>(src + k, 0);
}
// basic_string(const basic_string&): a long string gets its own buffer, a short one is copied as it is
inline void copyString(uint32_t dst, uint32_t src) {
    if (rw::load<uint8_t>(src) & 1) rw::call(STRING_INIT, {dst, rw::load<uint32_t>(src + 8), rw::load<uint32_t>(src + 4)});
    else copyWords(dst, src, 12);
}
// shared_ptr(const shared_ptr&): one more owner
inline void copyShared(uint32_t dst, uint32_t src) {
    rw::save<uint32_t>(dst, rw::load<uint32_t>(src));
    const uint32_t ctrl = rw::load<uint32_t>(src + 4);
    rw::save<uint32_t>(dst + 4, ctrl);
    if (ctrl) guest::addRef(ctrl);
}
inline void moveShared(uint32_t dst, uint32_t src) {
    copyWords(dst, src, 8);
    rw::save<uint32_t>(src, 0);
    rw::save<uint32_t>(src + 4, 0);
}

// ---------------------------------------------------------------------------------------------- vector growth
// vector<T>::max_size() for Size-byte elements, and __recommend(need): twice the capacity, at least `need`, at most
// max_size (when the capacity is already at half of it or more)
template <uint32_t Size> constexpr uint32_t maxSize() { return 0xFFFFFFFFu / Size < 0x7FFFFFFFu ? 0xFFFFFFFFu / Size : 0x7FFFFFFFu; }
template <uint32_t Size> inline uint32_t recommend(guest::Vector *v, uint32_t need) {
    const uint32_t cap = (v->cap - v->begin) / Size;
    if (cap >= maxSize<Size>() / 2) return maxSize<Size>();
    return 2 * cap < need ? need : 2 * cap;
}

// Swap a vector's buffer for the one a __split_buffer on the stack holds (sb: first, begin, end, end of capacity,
// allocator&), its trivially relocatable elements memcpy'd in front of element `size` - the inlined
// __swap_out_circular_buffer - then the split buffer (now holding the old buffer) destroyed by splitBufferDtor.
template <uint32_t Size>
inline void swapOutRelocating(guest::Vector *v, uint32_t sb, uint32_t newFirst, uint32_t size, uint32_t newEnd,
                              uint32_t splitBufferDtor) {
    const uint32_t oldBegin = v->begin, oldEnd = v->end, bytes = oldEnd - oldBegin;
    const uint32_t newBegin = newFirst + (size - bytes / Size) * Size;
    copyBytes(newBegin, oldBegin, bytes);
    v->begin = newBegin;
    rw::save<uint32_t>(sb + 4, oldBegin);
    v->end = newEnd;
    rw::save<uint32_t>(sb + 8, oldEnd);
    const uint32_t oldCap = v->cap;
    v->cap = rw::load<uint32_t>(sb + 12);
    rw::save<uint32_t>(sb + 12, oldCap);
    rw::save<uint32_t>(sb, oldBegin);
    rw::call(splitBufferDtor, {sb});
}

// __push_back_slow_path for trivially relocatable elements: a __split_buffer of the recommended capacity on the
// stack, the new element copied in at its begin (copyIn), the old elements relocated in front of it, the old buffer
// freed by the split buffer's destructor (splitBufferDtor, ~__split_buffer<T, allocator<T>&>).
template <uint32_t Size, class CopyIn>
inline void pushBackRelocating(guest::Vector *v, uint32_t splitBufferDtor, CopyIn copyIn) {
    const uint32_t size = (v->end - v->begin) / Size;
    if (size + 1 > maxSize<Size>()) { rw::call(THROW_LENGTH_ERROR, {rw::guest_addr(v)}); return; }
    const uint32_t newCap = recommend<Size>(v, size + 1);
    const uint32_t buf = newCap ? guest::newObject(newCap * Size) : 0;
    const uint32_t slot = buf + size * Size;
    guest::Frame frame(20);
    const uint32_t sb = frame.addr();
    rw::save<uint32_t>(sb, buf);
    rw::save<uint32_t>(sb + 4, slot);
    rw::save<uint32_t>(sb + 8, slot);
    rw::save<uint32_t>(sb + 12, buf + newCap * Size);
    rw::save<uint32_t>(sb + 16, rw::guest_addr(v) + 8);
    if (slot) copyIn(slot);
    swapOutRelocating<Size>(v, sb, buf, size, slot + Size, splitBufferDtor);
}

// ... with the split buffer's destructor inlined (vector<char>): the old buffer freed in place
template <uint32_t Size, class CopyIn> inline void pushBackRelocatingFreeInline(guest::Vector *v, CopyIn copyIn) {
    const uint32_t size = (v->end - v->begin) / Size;
    if (size + 1 > maxSize<Size>()) { rw::call(THROW_LENGTH_ERROR, {rw::guest_addr(v)}); return; }
    const uint32_t newCap = recommend<Size>(v, size + 1);
    const uint32_t buf = newCap ? guest::newObject(newCap * Size) : 0;
    const uint32_t slot = buf + size * Size;
    if (slot) copyIn(slot);
    const uint32_t oldBegin = v->begin, bytes = v->end - oldBegin;
    const uint32_t newBegin = buf + (size - bytes / Size) * Size;
    copyBytes(newBegin, oldBegin, bytes);
    v->begin = newBegin;
    v->end = slot + Size;
    v->cap = buf + newCap * Size;
    if (oldBegin) guest::deleteObject(oldBegin);
}

// reserve(n) of trivially relocatable elements: a __split_buffer(n, size()) on the stack, swapped in
template <uint32_t Size> inline void reserveRelocating(guest::Vector *v, uint32_t n, uint32_t splitBufferDtor) {
    if ((v->cap - v->begin) / Size >= n) return;
    guest::Frame frame(20);
    const uint32_t sb = frame.addr();
    rw::save<uint32_t>(sb + 16, rw::guest_addr(v) + 8);
    rw::save<uint32_t>(sb + 12, 0);
    const uint32_t size = (v->end - v->begin) / Size;
    const uint32_t buf = n ? guest::newObject(n * Size) : 0;
    rw::save<uint32_t>(sb + 12, buf + n * Size);
    swapOutRelocating<Size>(v, sb, buf, size, buf + size * Size, splitBufferDtor);
}

// __push_back_slow_path for other elements: a __split_buffer on the stack (first, begin, end, end of capacity,
// allocator&) gets the new element (construct), then __swap_out_circular_buffer moves the old elements into it and
// swaps the buffers, and the split buffer's destructor destroys the moved-from elements and frees the old buffer.
template <uint32_t Size, class Construct>
inline void pushBackViaSplitBuffer(guest::Vector *v, uint32_t swapOut, uint32_t splitBufferDtor, Construct construct) {
    const uint32_t size = (v->end - v->begin) / Size;
    if (size + 1 > maxSize<Size>()) { rw::call(THROW_LENGTH_ERROR, {rw::guest_addr(v)}); return; }
    const uint32_t newCap = recommend<Size>(v, size + 1);
    const uint32_t buf = newCap ? guest::newObject(newCap * Size) : 0;
    const uint32_t slot = buf + size * Size;
    guest::Frame frame(20);
    uint32_t *sb = frame.at<uint32_t>(0);
    sb[0] = buf;
    sb[1] = slot;
    sb[2] = slot;
    sb[3] = buf + newCap * Size;
    sb[4] = rw::guest_addr(v) + 8;
    if (slot) construct(slot);
    sb[2] = slot + Size;
    rw::call(swapOut, {rw::guest_addr(v), frame.addr()});
    rw::call(splitBufferDtor, {frame.addr()});
}

}  // namespace libcxx
