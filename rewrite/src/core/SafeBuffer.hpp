// SafeBuffer: the client's obfuscated value store. Game state that cheaters would edit (HP, CP, points, master
// data rows) lives in a SafeBuffer: every byte is XORed with a keyed LCG stream, and the data is hashed per
// 256-byte page so tampering can be detected (dirty pages are re-hashed).
#pragma once

#include "rw/rw.hpp"
#include "Guest.hpp"

struct SafeBuffer {
    rw::Field<uint32_t> vtable;            // +0
    rw::Field<uint32_t> key;               // +4  random seed of the XOR stream (initialize)
    rw::Field<uint32_t> size;              // +8  bytes of data
    rw::PtrField<uint8_t> data;            // +12 the obfuscated bytes
    rw::Field<uint32_t> pageCount;         // +16 (size + 255) / 256
    rw::PtrField<uint32_t> pageHashes;     // +20 one hash per page
    rw::PtrField<uint8_t> pageDirty;       // +24 one flag per page: written since its hash was taken
    rw::Field<uint32_t> hashState;         // +28

    static constexpr uint32_t PAGE_SHIFT = 8;

    // The XOR byte for data position `index` (an LCG step over index + key; bits 16-23 of the result).
    static uint8_t mask(uint32_t key, uint32_t index) { return uint8_t((214013u * (index + key) + 2531011u) >> 16); }

    // Decode n bytes from src (inside data) to dst; index = position of src in the data.
    static const SafeBuffer *read(const SafeBuffer *self, const uint8_t *dst, const uint8_t *src, uint32_t n, uint32_t index);
    // Encode n bytes from src into dst (inside data) and mark the pages it touched dirty.
    static SafeBuffer *write(SafeBuffer *self, const uint8_t *dst, const uint8_t *src, uint32_t n, uint32_t index);
    // Take over other's storage (move constructor / assignment); other keeps nothing to free.
    static uint32_t move(SafeBuffer *self, SafeBuffer *other);

    static constexpr uint32_t VTABLE = 0xF7CF38;   // _ZTV10SafeBuffer + 8
    static constexpr uint32_t PAGE_SIZE = 1u << PAGE_SHIFT;

    // Allocate `size` bytes of data plus the page hash tables (the key is a fresh random seed).
    static void initialize(SafeBuffer *self, uint32_t size);
    // Allocate the page hash tables for the current size (used after the data was adopted from elsewhere).
    static uint32_t createHashes(SafeBuffer *self);
    // Free data and hash tables.
    static void deinitialize(SafeBuffer *self);
    // SafeBuffer(size, registerForUpdate): zero-filled buffer; registered buffers are re-hashed page by page
    // every frame (the global update list).
    static SafeBuffer *construct(SafeBuffer *self, uint32_t size, uint32_t registerForUpdate);
    static SafeBuffer *destruct(SafeBuffer *self);
    static void destructDelete(SafeBuffer *self);
    // Re-key a copy of other's data into a freshly initialised buffer.
    static void copy(SafeBuffer *self, const SafeBuffer *other);
    static SafeBuffer *copyConstruct(SafeBuffer *self, const SafeBuffer *other);
    static SafeBuffer *copyAssign(SafeBuffer *self, const SafeBuffer *other);
    static SafeBuffer *moveConstruct(SafeBuffer *self, SafeBuffer *other);
    static SafeBuffer *moveAssign(SafeBuffer *self, SafeBuffer *other);
    // Hash the page at hashState (the page cursor): a dirty page takes the new hash, a clean page must match its
    // stored hash (false = tampered). *wrapped (optional) = the cursor wrapped to page 0.
    static bool update(SafeBuffer *self, const uint8_t *wrapped);
    // SafeBuffer(const shared_ptr<bg::Data>&): load key, size and the raw data from a serialized blob.
    static SafeBuffer *constructFromData(SafeBuffer *self, const guest::SharedPtr *data);
    // shared_ptr<bg::Data> with key, size and the raw (still obfuscated) data.
    static void serialize(guest::SharedPtr *out, const SafeBuffer *self);
    // Encode zeros over the whole buffer.
    static void zerofill(SafeBuffer *self);

    // Typed helpers for rewritten callers: a 32-bit value at data position `index`.
    int32_t get32(uint32_t index) const;
    void set32(uint32_t index, int32_t value);
};
RW_LAYOUT(SafeBuffer, key, 4);
RW_LAYOUT(SafeBuffer, data, 12);
RW_LAYOUT(SafeBuffer, pageDirty, 24);
RW_SIZE(SafeBuffer, 32);
