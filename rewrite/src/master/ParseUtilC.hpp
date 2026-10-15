// Helpers for the master::X::parse(const rapidjson::Value&) functions: they read one JSON row and build the
// table's row object (a SafeBuffer holding the fields at fixed data positions), returned as shared_ptr<const X>.
#pragma once

#include "MasterTable.hpp"

namespace master::rowparse {

constexpr uint32_t DATE_INIT = 0x4924F1;   // Date::init(this, const char*, bool) -> 1 on success
constexpr uint32_t TIME_INIT = 0x492341;   // Time::init(this, const char*, bool) -> nonzero on success

// rapidjson::GenericValue (32-bit build, 16 bytes): payload at +0 (int value, string pointer or array elements),
// +4 array size, flags at +12 (kIntFlag 0x400, kStringFlag 0x100000, an array's flags are exactly kArrayType 4).
inline uint32_t member(uint32_t object, uint32_t key) { return rw::call(JSON_MEMBER, {object, key}); }  // operator[]
inline bool isInt(uint32_t v) { return rw::load<uint8_t>(v + 13) & 0x04; }
inline bool isString(uint32_t v) { return rw::load<uint8_t>(v + 14) & 0x10; }
inline bool isArray(uint32_t v) { return rw::load<uint32_t>(v + 12) == 4; }
inline int32_t intOf(uint32_t v) { return rw::load<int32_t>(v); }
inline uint32_t stringOf(uint32_t v) { return rw::load<uint32_t>(v); }
inline uint32_t arraySize(uint32_t v) { return rw::load<uint32_t>(v + 4); }
inline uint32_t element(uint32_t v, uint32_t k) { return rw::load<uint32_t>(v) + 16 * k; }

// The row under construction. Every field is encoded from a buffer in the parse function's stack frame (offsets
// as in the original frame: the original may copy stale bytes from there).
class RowBuilder {
public:
    explicit RowBuilder(const guest::Frame &frame) : f_(frame) {}

    // operator new(0x20) + SafeBuffer(size, true) (clock-seeded key) + the parsed class's vtable
    void create(uint32_t size, uint32_t vtable) {
        obj_ = guest::newObject(0x20);
        rw::call(0x459F21, {obj_, size, 1});   // SafeBuffer::SafeBuffer(unsigned, bool)
        rw::save<uint32_t>(obj_, vtable);
    }
    uint32_t obj() const { return obj_; }
    SafeBuffer *row() const { return rw::host_ptr<SafeBuffer>(obj_); }

    // Encode n bytes of the stack buffer at frame offset buf to data position pos.
    void bytes(uint32_t pos, uint32_t buf, uint32_t n) {
        SafeBuffer *r = row();
        SafeBuffer::write(r, r->data.get() + pos, f_.at<uint8_t>(buf), n, pos);
    }
    // A 32-bit value through the stack temp at frame offset temp.
    void int32(uint32_t pos, int32_t value, uint32_t temp) {
        rw::store32(f_.at<uint8_t>(temp), uint32_t(value));
        bytes(pos, temp, 4);
    }
    // char[n]: strncpy into the stack buffer, terminate at n - 1, encode all n bytes.
    void string(uint32_t pos, uint32_t src, uint32_t n, uint32_t buf) {
        rw::call(STRNCPY, {f_.addr(buf), src, n});
        rw::store8(f_.at<uint8_t>(buf + n - 1), 0);
        bytes(pos, buf, n);
    }
    // Copy the int elements of a JSON array to the stack buffer at frame offset buf, element k at buf + 4k (no
    // bound check, as in the original). false at the first element that is not an int.
    bool intArray(uint32_t array, uint32_t buf) {
        uint32_t n = arraySize(array);
        for (uint32_t k = 0; k < n; k++) {
            uint32_t e = element(array, k);
            if (!isInt(e)) return false;
            rw::store32(f_.at<uint8_t>(buf + 4 * k), uint32_t(intOf(e)));
        }
        return true;
    }
    // An int32[count] field from a JSON array through the stack buffer at buf (stale entries past the array's
    // size are encoded as they are). false when the value is not an array or an element is not an int.
    bool int32Array(uint32_t pos, uint32_t array, uint32_t count, uint32_t buf) {
        if (!isArray(array) || !intArray(array, buf)) return false;
        bytes(pos, buf, 4 * count);
        return true;
    }
    // Date (8 bytes) / Time (4 bytes) parsed into the stack buffer: the last byte is cleared first, then
    // Date::init / Time::init(buf, string, false). Returns the init result.
    uint32_t parseDate(uint32_t src, uint32_t buf) {
        rw::store8(f_.at<uint8_t>(buf + 7), 0);
        return rw::call(DATE_INIT, {f_.addr(buf), src, 0});
    }
    uint32_t parseTime(uint32_t src, uint32_t buf) {
        rw::store8(f_.at<uint8_t>(buf + 3), 0);
        return rw::call(TIME_INIT, {f_.addr(buf), src, 0});
    }
    // The half-built row is destroyed through its virtual deleting destructor (vtable slot 1).
    void destroy() { rw::vcall(rw::host_ptr<uint8_t>(obj_), 1, {obj_}); }

private:
    const guest::Frame &f_;
    uint32_t obj_ = 0;
};

// out = shared_ptr<const X>(row) through the table's own libc++ constructor (allocates the control block).
inline void makeShared(guest::SharedPtr *out, uint32_t ctor, uint32_t row) {
    rw::call(ctor, {rw::guest_addr(out), row, 0});
}

}  // namespace master::rowparse
