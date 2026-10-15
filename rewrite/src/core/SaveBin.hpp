// SaveBin: the client's save-data container. A key -> blob store (std::unordered_map<std::string,
// std::shared_ptr<bg::Data>>): each typed value is serialized into a bg::MemoryStream and kept as a bg::Data
// blob under its key; serialize() packs every entry through bg::Archive into one blob, deserialize() reads them
// back.
#pragma once

#include "rw/rw.hpp"
#include "Guest.hpp"

// std::unordered_map<std::string, std::shared_ptr<bg::Data>> (32-bit libc++ __hash_table).
struct SaveBin {
    rw::Field<uint32_t> buckets;       // +0  bucket array
    rw::Field<uint32_t> bucketCount;   // +4
    rw::Field<uint32_t> firstNode;     // +8  __p1_ (before-begin node).__next_: the first entry
    rw::Field<uint32_t> size;          // +12
    rw::Field<float> maxLoadFactor;    // +16

    // A hash node of the map.
    struct Node {
        rw::Field<uint32_t> next;      // +0
        rw::Field<uint32_t> hash;      // +4
        guest::String key;             // +8
        guest::SharedPtr value;        // +20 shared_ptr<bg::Data>
    };

    // bg::Data: a byte blob.
    struct Data {
        rw::Field<uint32_t> vtable;    // +0
        rw::Field<uint32_t> bytes;     // +4 guest address of the bytes
        rw::Field<uint32_t> length;    // +8
    };

    // Typed values: serialized with bg::MemoryStream::write (sizeof(T) bytes) / read back with
    // IInputStream::read; get returns 1 when the key exists and exactly one element was read.
    template <class T> static void setValue(SaveBin *self, const char *key, const T *value);
    template <class T> static uint32_t getValue(const SaveBin *self, const char *key, const T *out);
    // Containers: serialized by bg::ioutil::_write / _read (C describes the container type).
    template <class C> static void setContainer(SaveBin *self, const char *key, uint32_t value);
    template <class C> static uint32_t getContainer(const SaveBin *self, const char *key, uint32_t out);

    // set(key, data): replaces the entry.
    static void set(SaveBin *self, const char *key, const guest::SharedPtr *data);
    static void remove(SaveBin *self, const char *key);
    // get(key) -> shared_ptr<bg::Data> (sret; empty when missing).
    static void get(guest::SharedPtr *out, const SaveBin *self, const char *key);
    static void clear(SaveBin *self);
    // serialize() -> shared_ptr<bg::Data> (sret).
    static void serialize(guest::SharedPtr *out, const SaveBin *self);
    // deserialize(blob): 1 = every entry read, 0 = a bad entry (the store is cleared).
    static uint32_t deserialize(SaveBin *self, const guest::SharedPtr *data);
};
RW_LAYOUT(SaveBin, firstNode, 8);
RW_LAYOUT(SaveBin, maxLoadFactor, 16);
RW_LAYOUT(SaveBin::Node, key, 8);
RW_LAYOUT(SaveBin::Node, value, 20);
RW_LAYOUT(SaveBin::Data, length, 8);
