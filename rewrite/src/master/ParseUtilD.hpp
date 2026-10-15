// The engine shared by the master::X::parse functions of Stage, RaidEnemyAttack, Colosseum, MypageBackground,
// LoginBonus, World, AlbumChallenge, Sphere, AvatarParts, DrawMedalList, DrawSkillType, Badstatus, Buff and
// AvatarCombination: one JSON row (rapidjson object) -> a new SafeBuffer row.
//
// Every one of these parse functions is the same compiled pattern over a list of fields:
//   1. look up every member in field order and check its type (int / string / array); on the first mismatch the
//      result is an empty shared_ptr (nothing allocated);
//   2. operator new(0x20) + SafeBuffer(rowSize, true), set the table's vtable, then encode each field at its data
//      position through a stack temporary (SafeBuffer::write). Int and String fields reuse the step-1 lookup;
//      Date / IntArray / StringArray fields look the member up again and check it again (and every array
//      element); a failure there destroys the half-built row (virtual deleting destructor) and the result is
//      empty;
//   3. the result is std::shared_ptr<const X>(row).
// Stack temporaries live at the original frame offsets, so bytes the original copies without initialising (the
// tail of a short array, the bytes after a Date) are the same stale bytes.
#pragma once

#include <vector>

#include "MasterTable.hpp"

namespace master::parse_d {

constexpr uint32_t DATE_INIT = 0x4924F1;   // Date::init(Date*, const char*, bool)

// rapidjson::GenericValue (16 bytes): +0 payload (int value / string pointer / array elements), +4 array size,
// +12 flags (exactly 4 = array; bit 0x400 = int; bit 0x100000 = string).
struct Json {
    uint32_t addr;
    static Json member(uint32_t object, uint32_t key) { return {rw::call(JSON_MEMBER, {object, key})}; }
    bool isInt() const { return rw::load<uint8_t>(addr + 13) & 4; }
    bool isString() const { return rw::load<uint8_t>(addr + 14) & 0x10; }
    bool isArray() const { return rw::load<uint32_t>(addr + 12) == 4; }
    uint32_t raw() const { return rw::load<uint32_t>(addr); }          // int value / string pointer
    uint32_t size() const { return rw::load<uint32_t>(addr + 4); }     // array length
    Json at(uint32_t k) const { return {raw() + 16 * k}; }             // array element
};

enum class Kind : uint8_t {
    Int,           // int32 (rapidjson int)
    String,        // char[size]: strncpy(size), last byte forced to 0
    Date,          // Date (8 bytes) parsed from a string: byte 7 cleared, then Date::init(buf, str, false)
    IntArray,      // int32[size / 4]: the elements present are copied; the rest of the buffer is whatever it held
    StringArray,   // char[size / stride][stride]: each element strncpy(stride) + its last byte forced to 0; the
                   // strings not present are whatever the buffer held
};

struct Field {
    uint32_t key;         // guest address of the JSON key literal
    Kind kind;
    uint32_t pos;         // data position in the row
    uint32_t size;        // bytes encoded
    uint32_t buf;         // frame offset of the stack temporary the original encodes from
    uint32_t stride = 0;  // StringArray: bytes per string
};

struct Spec {
    uint32_t frameSize;    // the original's stack frame (pushes + locals)
    uint32_t rowSize;      // SafeBuffer size
    uint32_t vtable;       // vtable of the parsed object (_ZTVN6master<X>E + 8)
    uint32_t sharedCtor;   // std::shared_ptr<const X>::shared_ptr<X>(X*) as the original calls it
};

// Copy the int elements of an array into the stack buffer; false at the first element that is not an int (the
// elements before it are already copied).
inline bool copyIntArray(const guest::Frame &f, uint32_t buf, Json array) {
    uint32_t count = array.size();
    for (uint32_t k = 0; k < count; k++) {
        Json element = array.at(k);
        if (!element.isInt()) return false;
        rw::store32(f.at<uint8_t>(buf + 4 * k), element.raw());
    }
    return true;
}

// Copy the string elements of an array into consecutive stride-byte slots; false at the first element that is not
// a string.
inline bool copyStringArray(const guest::Frame &f, uint32_t buf, uint32_t stride, Json array) {
    uint32_t count = array.size();
    for (uint32_t k = 0; k < count; k++) {
        Json element = array.at(k);
        if (!element.isString()) return false;
        rw::call(STRNCPY, {f.addr(buf + stride * k), element.raw(), stride});
        rw::store8(f.at<uint8_t>(buf + stride * k + stride - 1), 0);
    }
    return true;
}

inline bool typeMatches(Kind kind, Json v) {
    switch (kind) {
    case Kind::Int: return v.isInt();
    case Kind::String:
    case Kind::Date: return v.isString();
    case Kind::IntArray:
    case Kind::StringArray: return v.isArray();
    }
    return false;
}

inline void parseRow(guest::SharedPtr *out, uint32_t json, const Spec &spec, const Field *fields, size_t count) {
    guest::Frame f(spec.frameSize);
    // 1. every member present with the right type
    std::vector<Json> values(count);
    for (size_t k = 0; k < count; k++) {
        Json v = Json::member(json, fields[k].key);
        if (!typeMatches(fields[k].kind, v)) {
            rw::call(spec.sharedCtor, {rw::guest_addr(out), 0, 0});
            return;
        }
        values[k] = v;
    }
    // 2. the row
    uint32_t obj = guest::newObject(0x20);
    auto *row = rw::host_ptr<SafeBuffer>(obj);
    SafeBuffer::construct(row, spec.rowSize, 1);
    row->vtable = spec.vtable;
    for (size_t k = 0; k < count; k++) {
        const Field &field = fields[k];
        bool ok = true;
        switch (field.kind) {
        case Kind::Int:
            rw::store32(f.at<uint8_t>(field.buf), values[k].raw());
            break;
        case Kind::String:
            rw::call(STRNCPY, {f.addr(field.buf), values[k].raw(), field.size});
            rw::store8(f.at<uint8_t>(field.buf + field.size - 1), 0);
            break;
        case Kind::Date: {
            Json text = Json::member(json, field.key);
            if (!text.isString()) { ok = false; break; }
            rw::store8(f.at<uint8_t>(field.buf + 7), 0);
            ok = rw::call(DATE_INIT, {f.addr(field.buf), text.raw(), 0}) != 0;
            break;
        }
        case Kind::IntArray: {
            Json array = Json::member(json, field.key);
            ok = array.isArray() && copyIntArray(f, field.buf, array);
            break;
        }
        case Kind::StringArray: {
            Json array = Json::member(json, field.key);
            ok = array.isArray() && copyStringArray(f, field.buf, field.stride, array);
            break;
        }
        }
        if (!ok) {
            rw::vcall(row, 1, {obj});   // virtual ~X() (deleting)
            rw::call(spec.sharedCtor, {rw::guest_addr(out), 0, 0});
            return;
        }
        SafeBuffer::write(row, row->data.get() + field.pos, f.at<uint8_t>(field.buf), field.size, field.pos);
    }
    // 3. the result
    rw::call(spec.sharedCtor, {rw::guest_addr(out), obj, 0});
}

template <size_t N>
inline void parseRow(guest::SharedPtr *out, uint32_t json, const Spec &spec, const Field (&fields)[N]) {
    parseRow(out, json, spec, fields, N);
}

}  // namespace master::parse_d
