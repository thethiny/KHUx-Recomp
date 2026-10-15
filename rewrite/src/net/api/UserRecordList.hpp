// UserRecordList: the shape shared by the "list of user-owned records" responses (UserAvatarParts, UserMedal,
// UserSkill, UserKeyblade; UserMaterial has the same shape). The records live in a SafeBuffer, one fixed-size
// record after the other, and an unordered_map<unsigned long long id, int index> finds a record by its user id
// (the first 8 bytes of every record).
//
// The parse / create / destructor bodies of those classes are the same code with a different record size,
// vtable, JSON key and record parser, so they are written once here (not registered: each class registers its
// own functions, which call these).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserRecordList : SafeBuffer {
    rw::Field<uint32_t> count;                // +32 records
    const uint8_t indexById[20];                    // +36 std::unordered_map<unsigned long long, int>
};
RW_LAYOUT(UserRecordList, count, 32);
RW_LAYOUT(UserRecordList, indexById, 36);
RW_SIZE(UserRecordList, 56);

namespace records {

constexpr uint32_t INDEX_MAP = 36;            // offset of the id -> index map
// std::unordered_map<unsigned long long, int>::__insert_unique(pair<ull, int>&&) / (pair<ull, unsigned>&&)
constexpr uint32_t MAP_INSERT_INT = 0x4B176D;
constexpr uint32_t MAP_INSERT_UINT = 0x4B1071;
constexpr uint32_t MAP_DTOR = 0x4B09C1;       // ~__hash_table<unsigned long long, int>
constexpr uint32_t ONE_F = 0x3F800000;        // 1.0f, the map's max_load_factor

// What differs between the record-list classes.
struct Kind {
    uint32_t vtable;            // the class vtable (+8)
    const char *key;            // the response member holding the array
    uint32_t recordSize;        // bytes per record (SafeBuffer stride)
    uint32_t sharedPtrCtor;     // std::shared_ptr<X>::shared_ptr(X*)
    uint32_t makeShared;        // std::shared_ptr<X>::make_shared<unsigned>(unsigned&&)
    uint32_t sizeInverse;       // inverse of recordSize / 8 mod 2^32 (create divides the vector size with it)
};

// Where the original parse keeps its locals (its stack frame size and the offsets from sp).
struct ParseFrame {
    uint32_t size;              // pushed registers + locals
    uint32_t record;            // the record being parsed
    uint32_t pair;              // the pair<ull, int> inserted into the map
    uint32_t insertResult;      // __insert_unique's returned pair<iterator, bool>
};

// new X: SafeBuffer(count * recordSize, true), the vtable, the count and an empty map.
inline UserRecordList *newList(uint32_t vtable, uint32_t count, uint32_t dataSize) {
    UserRecordList *self = newObject<UserRecordList>(sizeof(UserRecordList));
    constructSafeBuffer(self, dataSize);
    self->vtable = vtable;
    self->count = count;
    rw::set<uint32_t>(self, INDEX_MAP + 0, 0);       // buckets, bucket count, first node, size
    rw::set<uint32_t>(self, INDEX_MAP + 4, 0);
    rw::set<uint32_t>(self, INDEX_MAP + 8, 0);
    rw::set<uint32_t>(self, INDEX_MAP + 12, 0);
    rw::set<uint32_t>(self, INDEX_MAP + 16, ONE_F);
    return self;
}

// Copy the record at `record` (guest memory) into slot k and index it by its id (first 8 bytes).
inline void addRecord(UserRecordList *self, const GuestFrame &frame, const ParseFrame &at, uint32_t recordSize,
                      uint32_t k, uint32_t insertFn) {
    const uint8_t *record = frame.at<uint8_t>(at.record);
    writeBytes(self, k * recordSize, record, recordSize);
    IdIndexPair *pair = frame.at<IdIndexPair>(at.pair);
    pair->keyLo = rw::get<uint32_t>(record, 0);
    pair->keyHi = rw::get<uint32_t>(record, 4);
    pair->index = int32_t(k);
    rw::call(insertFn, {frame.addr(at.insertResult), rw::guest_addr(self) + INDEX_MAP, frame.addr(at.pair)});
}

// X::parse: response[key] (an array) -> *out. Not an object / no array: an empty pointer. A record that fails
// its init: the whole list is deleted and *out holds null.
template <class Record>
inline uint32_t parseList(SharedPtr *out, const JsonValue *response, const Kind &kind, const ParseFrame &at,
                          uint32_t (*init)(Record *, const JsonValue *)) {
    GuestFrame frame(at.size);
    UserRecordList *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &list = (*response)[kind.key];
        if (list.IsArray()) {
            uint32_t n = list.Size();
            self = newList(kind.vtable, n, n * kind.recordSize);
            uint32_t k = 0;
            for (; k < n; k++) {
                if (!init(frame.at<Record>(at.record), &list.At(k))) break;
                addRecord(self, frame, at, kind.recordSize, k, MAP_INSERT_INT);
            }
            if (k != n) {
                deleteVirtual(self);
                self = nullptr;
            }
        }
    }
    rw::call(kind.sharedPtrCtor, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}

// X::create: an X holding a copy of the given records (make_shared, then copy + index each one).
inline uint32_t createList(SharedPtr *out, const StdVector *records, const Kind &kind) {
    // original frame 0x50: the count at +0x14, the map pair at +0x18, the insert result at +0x2c
    GuestFrame frame(0x50);
    // records.size(): the byte span / 8 times the inverse of recordSize / 8 (an exact division)
    uint32_t count = uint32_t(int32_t(records->end - records->begin) >> 3) * kind.sizeInverse;
    rw::store32(frame.at<uint32_t>(0x14), count);
    rw::call(kind.makeShared, {rw::guest_addr(out), frame.addr(0x14)});
    uint32_t begin = records->begin;
    if (begin != records->end) {
        uint32_t bytes = records->end - begin;
        UserRecordList *self = rw::host_ptr<UserRecordList>(out->ptr);
        IdIndexPair *pair = frame.at<IdIndexPair>(0x18);
        uint32_t offset = 0;
        for (uint32_t k = 0;; k++) {
            writeBytes(self, offset, rw::host_ptr<uint8_t>(begin + offset), kind.recordSize);
            pair->keyLo = rw::load<uint32_t>(begin + offset);
            pair->keyHi = rw::load<uint32_t>(begin + offset + 4);
            pair->index = int32_t(k);
            rw::call(MAP_INSERT_UINT, {frame.addr(0x2C), rw::guest_addr(self) + INDEX_MAP, frame.addr(0x18)});
            offset += kind.recordSize;
            if (offset == bytes) break;
        }
    }
    return 0;   // stack-protector check
}

// ~X (D2): the map, then the SafeBuffer.
inline UserRecordList *destroy(UserRecordList *self, uint32_t vtable) {
    self->vtable = vtable;
    rw::call(MAP_DTOR, {rw::guest_addr(self) + INDEX_MAP});
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return self;
}

// ~X + operator delete (D0)
inline uint32_t destroyDelete(UserRecordList *self, uint32_t vtable) {
    destroy(self, vtable);
    return rw::call(guest::operator_delete, {rw::guest_addr(self)});
}

// Date::init / Date::calculateIntervalSince1970 for the "getDatetime" fields (the record keeps the seconds).
constexpr uint32_t DATE_CALCULATE_INTERVAL_SINCE_1970 = 0x492745;

// obj["getDatetime"] (a string) -> seconds since 1970 in *field; the Date local lives at `date` in the frame.
// false when the member is not a string or the date does not parse.
inline bool readDatetime(const JsonValue &obj, const GuestFrame &frame, uint32_t date, rw::Field<int32_t> &field) {
    rw::store8(frame.at<uint8_t>(date + 7), 0);          // Date flags byte cleared before init
    const JsonValue &v = obj["getDatetime"];
    if (!v.IsString()) return false;
    if (rw::call(guest::Date_init, {frame.addr(date), v.GetString(), 0}) != 1) return false;
    field = int32_t(rw::call(DATE_CALCULATE_INTERVAL_SINCE_1970, {frame.addr(date)}));
    return true;
}

}  // namespace records

}  // namespace hole::network::api
