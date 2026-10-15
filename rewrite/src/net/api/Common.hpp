// Shared pieces of the hole::network::api response parsers: the rapidjson value view, guest-stack locals, and
// the library types (std::string, std::vector, std::shared_ptr) the parsers fill in.
//
// The parsers read the server's JSON (handlers.py builds it) with rapidjson and store what they find either in
// plain fields of a response object or in a SafeBuffer (the client's obfuscated value store). They return the
// response through a std::shared_ptr (the hidden result pointer, first argument). The runtime compares r0 of
// every rewrite (no void returns), so each rewrite returns what the original leaves in r0: 0 for the functions
// with a stack protector (guard - saved guard), otherwise the last callee's result.
#pragma once

#include <cstring>

#include "rw/rw.hpp"
#include "core/SafeBuffer.hpp"

namespace hole::network::api {

// ---------------------------------------------------------------------------------------------- guest functions
// Library and other-module functions the parsers call (Thumb bit included, as the lifted code calls them).
namespace guest {
constexpr uint32_t operator_new = 0xC98EF1;       // operator new(size_t)
constexpr uint32_t operator_delete = 0xC98F9B;    // operator delete(void*)
constexpr uint32_t JsonValue_member = 0x4B1BB5;   // rapidjson::GenericValue::operator[](const char*)
constexpr uint32_t SafeBuffer_ctor = 0x459F21;    // SafeBuffer::SafeBuffer(unsigned size, bool)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;    // SafeBuffer::~SafeBuffer()
constexpr uint32_t string_assign = 0x48D811;      // std::string::assign(const char*, size_t)
constexpr uint32_t Date_init = 0x4924F1;          // Date::init(const char*, bool)
constexpr uint32_t strlen_ = 0x40E08C;            // strlen (import)
constexpr uint32_t strncpy_ = 0x410E4C;           // strncpy (import)
constexpr uint32_t memcpy_ = 0x40E164;            // __aeabi_memcpy (import)
constexpr uint32_t memset_ = 0x40E7A0;            // __aeabi_memset(dest, n, c) (import)
constexpr uint32_t shared_add = 0xC8DBB5;         // std::__shared_weak_count::__add_shared()
constexpr uint32_t shared_release = 0xC8DBD1;     // std::__shared_weak_count::__release_shared()
constexpr uint32_t string_init = 0x47CD93;        // std::string::__init(const char*, size_t)
}  // namespace guest

// ---------------------------------------------------------------------------------------------- guest stack
// Locals whose address is handed to guest functions must live in guest memory. A GuestFrame reserves the
// original function's stack frame (pushed registers + locals, so callees run at the same stack addresses and
// uninitialised bytes hold the same leftovers as in the original) and gives the locals at their original
// offsets from the frame's stack pointer. Stores into it go through rw::store8 / rw::store32 / rw::set.
class GuestFrame {
public:
    explicit GuestFrame(uint32_t size) : saved_(rw::cpu->r[13]) { rw::cpu->r[13] = saved_ - size; }
    ~GuestFrame() { rw::cpu->r[13] = saved_; }
    GuestFrame(const GuestFrame &) = delete;
    GuestFrame &operator=(const GuestFrame &) = delete;

    uint32_t sp() const { return rw::cpu->r[13]; }
    uint32_t addr(uint32_t offset) const { return sp() + offset; }               // guest address of a local
    template <class T> T *at(uint32_t offset) const { return rw::frame_ptr<T>(sp() + offset); }

private:
    uint32_t saved_;
};

// A C string copied onto the guest stack for the duration of a call (JSON member names: rapidjson only compares
// the characters, so a copy behaves like the original literal).
class GuestCString {
public:
    explicit GuestCString(const char *s)
        : frame_((uint32_t(std::strlen(s)) + 1 + 7) & ~7u) {
        uint8_t *p = frame_.at<uint8_t>(0);
        for (size_t k = 0;; k++) {
            rw::store8(p + k, uint8_t(s[k]));
            if (!s[k]) break;
        }
    }
    uint32_t addr() const { return frame_.sp(); }

private:
    GuestFrame frame_;
};

// ---------------------------------------------------------------------------------------------- rapidjson
// rapidjson::GenericValue as bundled with cocos2d-x 3.x (16 bytes): 12 bytes of payload, then the flags word.
// Payload: int / uint = word 0; int64 / uint64 / double = words 0-1; string = {const char *str, length, hash};
// object = {Member *members (32 bytes each: name value, value value), size, capacity}; array = {GenericValue
// *elements, size, capacity}.
struct JsonValue {
    rw::Field<uint32_t> w0;        // +0
    rw::Field<uint32_t> w1;        // +4
    rw::Field<uint32_t> w2;        // +8
    rw::Field<uint32_t> flags;     // +12 type (low byte) | kind flags

    // types (flags & 0xFF) and kind flags
    static constexpr uint32_t kNull = 0, kFalse = 1, kTrue = 2, kObject = 3, kArray = 4, kString = 5, kNumber = 6;
    static constexpr uint32_t kBoolFlag = 0x100, kNumberFlag = 0x200, kIntFlag = 0x400, kUintFlag = 0x800,
                              kInt64Flag = 0x1000, kUint64Flag = 0x2000, kDoubleFlag = 0x4000,
                              kStringFlag = 0x100000;
    static constexpr uint32_t kTrueFlag = kTrue | kBoolFlag, kFalseFlag = kFalse | kBoolFlag;

    // rapidjson's Is*(): the object / array / null / true tests compare the whole flags word
    bool IsNull() const { return flags == kNull; }
    bool IsObject() const { return flags == kObject; }
    bool IsArray() const { return flags == kArray; }
    bool IsTrue() const { return flags == kTrueFlag; }
    bool IsFalse() const { return flags == kFalseFlag; }
    bool IsBool() const { return (flags & kBoolFlag) != 0; }
    bool IsNumber() const { return (flags & kNumberFlag) != 0; }
    bool IsInt() const { return (flags & kIntFlag) != 0; }
    bool IsUint() const { return (flags & kUintFlag) != 0; }
    bool IsInt64() const { return (flags & kInt64Flag) != 0; }
    bool IsUint64() const { return (flags & kUint64Flag) != 0; }
    bool IsDouble() const { return (flags & kDoubleFlag) != 0; }
    bool IsString() const { return (flags & kStringFlag) != 0; }

    int32_t GetInt() const { return int32_t(w0.get()); }
    uint32_t GetUint() const { return w0; }
    uint64_t GetUint64() const { return uint64_t(w0.get()) | (uint64_t(w1.get()) << 32); }
    int64_t GetInt64() const { return int64_t(GetUint64()); }
    double GetDouble() const { double d; uint64_t u = GetUint64(); std::memcpy(&d, &u, 8); return d; }
    bool GetBool() const { return flags == kTrueFlag; }
    uint32_t GetString() const { return w0; }                         // guest address of the characters
    const char *GetStringHost() const { return rw::host_ptr<char>(w0); }
    uint32_t GetStringLength() const { return w1; }

    // arrays / objects
    uint32_t Size() const { return w1; }                              // element / member count
    JsonValue *Elements() const { return rw::host_ptr<JsonValue>(w0); }
    JsonValue &At(uint32_t k) const { return Elements()[k]; }        // array element k (no bounds check)

    // object member by name: rapidjson's lookup (a missing name gives its static null value)
    JsonValue &operator[](const char *name) const {
        GuestCString key(name);
        return *rw::host_ptr<JsonValue>(rw::call(guest::JsonValue_member, {rw::guest_addr(this), key.addr()}));
    }
    // the same with a name that is already a guest string (a key passed in by the caller, or a .rodata literal)
    JsonValue &member(uint32_t guestName) const {
        return *rw::host_ptr<JsonValue>(rw::call(guest::JsonValue_member, {rw::guest_addr(this), guestName}));
    }
};
RW_SIZE(JsonValue, 16);

// ---------------------------------------------------------------------------------------------- libc++ types
// std::string (libc++, 32-bit): short form when byte 0 bit 0 is clear (size = byte0 >> 1, chars at +1), else
// long form (capacity | 1 at +0, size at +4, data pointer at +8).
struct StdString {
    rw::Field<uint32_t> w0;        // +0
    rw::Field<uint32_t> w1;        // +4
    rw::Field<uint32_t> w2;        // +8

    bool isLong() const { return (w0 & 1) != 0; }
    uint32_t size() const { return isLong() ? w1.get() : (w0 & 0xFF) >> 1; }
    uint32_t data() const { return isLong() ? w2.get() : rw::guest_addr(this) + 1; }   // guest address
    // an empty string (what the zero-filled constructor makes)
    void clear() { w0 = 0; w1 = 0; w2 = 0; }
    // std::string::assign(const char *s, size_t n) with s a guest address
    void assign(uint32_t s, uint32_t n) { rw::call(guest::string_assign, {rw::guest_addr(this), s, n}); }
    // assign a NUL-terminated guest string (the parsers do strlen + assign)
    void assign(uint32_t s) { assign(s, uint32_t(std::strlen(rw::host_ptr<char>(s)))); }
};
RW_SIZE(StdString, 12);

// std::vector<T> (libc++): begin, end, end of capacity.
struct StdVector {
    rw::Field<uint32_t> begin;     // +0
    rw::Field<uint32_t> end;       // +4
    rw::Field<uint32_t> capEnd;    // +8
};
RW_SIZE(StdVector, 12);

// std::shared_ptr<T>: the object, then its control block.
struct SharedPtr {
    rw::Field<uint32_t> ptr;       // +0
    rw::Field<uint32_t> ctrl;      // +4
    void reset() { ptr = 0; ctrl = 0; }   // what the parsers store for "no result"
};
RW_SIZE(SharedPtr, 8);

// The pair<unsigned long long, int> the parsers insert into their id -> record index unordered_maps (16 bytes
// with padding; built in a guest-stack local, then std::__hash_table::__insert_unique).
struct IdIndexPair {
    rw::Field<uint32_t> keyLo;      // +0
    rw::Field<uint32_t> keyHi;      // +4
    rw::Field<int32_t> index;       // +8
};

// ---------------------------------------------------------------------------------------------- helpers
inline uint32_t guestNew(uint32_t size) { return rw::call(guest::operator_new, {size}); }
inline void guestDelete(uint32_t p) { rw::call(guest::operator_delete, {p}); }
template <class T> inline T *newObject(uint32_t size) { return rw::host_ptr<T>(guestNew(size)); }

// SafeBuffer::SafeBuffer(size, true): construct the buffer part of a response object in place.
inline void constructSafeBuffer(SafeBuffer *buf, uint32_t size, bool flag = true) {
    rw::call(guest::SafeBuffer_ctor, {rw::arg(buf), size, flag ? 1u : 0u});
}
// The deleting destructor (D0) of the SafeBuffer-only responses (UserPoint, User, Login...): ~SafeBuffer, then
// operator delete on what the destructor returned (this). Returns operator delete's r0.
inline uint32_t deleteSafeBufferResponse(SafeBuffer *self) {
    uint32_t p = rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return rw::call(guest::operator_delete, {p});
}
// Virtual deleting destructor (vtable slot 1) of a polymorphic guest object.
inline void deleteVirtual(void *obj) { rw::vcall(obj, 1, {rw::guest_addr(obj)}); }

// SafeBuffer stores of plain values at a data position (the parsers copy through a local, then write).
inline void writeU32(SafeBuffer *buf, uint32_t index, uint32_t v) {
    SafeBuffer::write(buf, buf->data.get() + index, reinterpret_cast<const uint8_t *>(&v), 4, index);
}
inline void writeU64(SafeBuffer *buf, uint32_t index, uint64_t v) {
    SafeBuffer::write(buf, buf->data.get() + index, reinterpret_cast<const uint8_t *>(&v), 8, index);
}
inline void writeBytes(SafeBuffer *buf, uint32_t index, const void *src, uint32_t n) {
    SafeBuffer::write(buf, buf->data.get() + index, static_cast<const uint8_t *>(src), n, index);
}

}  // namespace hole::network::api
