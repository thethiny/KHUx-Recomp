// The engine shared by the master::X::parse functions of RaidEnemy, DrawMedalType, Skill, EvCampaign, Reward,
// RaidSetting, SphereArray, Title, Player, GuiltProb, SkillExp and Misc: one JSON row (a rapidjson object) -> a
// new SafeBuffer row, returned as std::shared_ptr<const X>.
//
// All of them are the same compiled pattern over a list of fields:
//   1. look every member up in field order and check its type (int / string / array); at the first mismatch the
//      result is an empty shared_ptr (nothing allocated);
//   2. operator new(0x20) + SafeBuffer(rowSize, true), set the table's vtable, then encode each field at its data
//      position from a stack temporary with SafeBuffer::write. Int and String fields reuse the step-1 lookup;
//      IntArray and Date fields look the member up again and check it again; a failure there destroys the
//      half-built row (virtual deleting destructor) and the result is empty;
//   3. the result is std::shared_ptr<const X>(row).
// The stack temporaries are at the original frame offsets, so the bytes the original copies without initialising
// them (the tail of a short array, the padding of a Date) are the same stale stack bytes.
#pragma once

#include <vector>

#include "MasterTable.hpp"

namespace master::parse_b {

constexpr uint32_t DATE_INIT = 0x4924F1;   // Date::init(Date*, const char*, bool) -> bool

// rapidjson::GenericValue (16 bytes): +0 payload (int value / string pointer / array elements), +4 array size,
// +12 flags (exactly 4 = array; bit 0x400 = int; bit 0x100000 = string).
struct Json {
    uint32_t addr = 0;
    static Json member(uint32_t object, uint32_t key) { return {rw::call(JSON_MEMBER, {object, key})}; }
    bool isInt() const { return rw::load<uint8_t>(addr + 13) & 4; }
    bool isString() const { return rw::load<uint8_t>(addr + 14) & 0x10; }
    bool isArray() const { return rw::load<uint32_t>(addr + 12) == 4; }
    uint32_t raw() const { return rw::load<uint32_t>(addr); }        // int value / string pointer
    uint32_t size() const { return rw::load<uint32_t>(addr + 4); }   // array length
    Json at(uint32_t k) const { return {raw() + 16 * k}; }           // array element
};

enum class Kind : uint8_t {
    Int,        // int32 (rapidjson int)
    String,     // char[size]: strncpy(size), last byte forced to 0
    IntArray,   // int32[size / 4]: the elements present are copied (no bound check); the rest is stale stack
    Date,       // Date (8 bytes) parsed from a string: byte 7 cleared, then Date::init(buf, str, false)
};

struct Field {
    uint32_t key;    // guest address of the JSON key literal
    Kind kind;
    uint32_t pos;    // data position in the row
    uint32_t size;   // bytes encoded
    uint32_t buf;    // frame offset of the stack temporary the original encodes from
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

inline bool checkType(Kind kind, Json v) {
    switch (kind) {
    case Kind::Int: return v.isInt();
    case Kind::IntArray: return v.isArray();
    default: return v.isString();   // String, Date
    }
}

inline void parseRow(guest::SharedPtr *out, uint32_t json, const Spec &spec, const Field *fields, size_t count) {
    guest::Frame f(spec.frameSize);
    uint32_t result = rw::guest_addr(out);
    // 1. every member present with the right type
    std::vector<Json> values(count);
    for (size_t k = 0; k < count; k++) {
        values[k] = Json::member(json, fields[k].key);
        if (!checkType(fields[k].kind, values[k])) {
            rw::call(spec.sharedCtor, {result, 0, 0});
            return;
        }
    }
    // 2. the row
    uint32_t obj = guest::newObject(0x20);
    auto *row = rw::host_ptr<SafeBuffer>(obj);
    SafeBuffer::construct(row, spec.rowSize, 1);
    row->vtable = spec.vtable;
    for (size_t k = 0; k < count; k++) {
        const Field &field = fields[k];
        uint8_t *buf = f.at<uint8_t>(field.buf);
        bool ok = true;
        switch (field.kind) {
        case Kind::Int:
            rw::store32(buf, values[k].raw());
            break;
        case Kind::String:
            rw::call(STRNCPY, {f.addr(field.buf), values[k].raw(), field.size});
            rw::store8(buf + field.size - 1, 0);
            break;
        case Kind::IntArray: {
            Json array = Json::member(json, field.key);
            ok = array.isArray() && copyIntArray(f, field.buf, array);
            break;
        }
        case Kind::Date: {
            Json text = Json::member(json, field.key);
            if (!text.isString()) { ok = false; break; }
            rw::store8(buf + 7, 0);
            ok = rw::call(DATE_INIT, {f.addr(field.buf), text.raw(), 0}) != 0;
            break;
        }
        }
        if (!ok) {
            rw::vcall(row, 1, {obj});   // virtual ~X() (deleting)
            rw::call(spec.sharedCtor, {result, 0, 0});
            return;
        }
        SafeBuffer::write(row, row->data.get() + field.pos, buf, field.size, field.pos);
    }
    // 3. the result
    rw::call(spec.sharedCtor, {result, obj, 0});
}

template <size_t N>
inline void parseRow(guest::SharedPtr *out, uint32_t json, const Spec &spec, const Field (&fields)[N]) {
    parseRow(out, json, spec, fields, N);
}

}  // namespace master::parse_b
