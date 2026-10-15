// UserTitle: the player's titles (name plates). The records live in a SafeBuffer (16 bytes each), and an
// unordered_map<int, int> (filled from pair<userTitleId, record index>) finds them.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserTitle : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7EF48;     // UserTitle vtable (+8)

    // One record (also the SafeBuffer layout: record k at data position 16 * k).
    struct Title {
        rw::Field<uint64_t> userTitleId;      // +0
        rw::Field<int32_t> titleId;           // +8 master title id
        const uint8_t pad_12[4];                    // never written by init (copied as it is)

        // {"userTitleId" (uint64), "titleId" (int)}: 1 when both are there, else 0 (fields set so far stay)
        static uint32_t init(Title *self, const JsonValue *json);
    };

    rw::Field<uint32_t> count;                // +32 records
    const uint8_t indexById[20];                    // +36 std::unordered_map<int, int>

    // response["userTitles"] (an array) -> *out; empty when absent, null when a record is incomplete
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    // a UserTitle holding the given records (make_shared)
    static uint32_t create(SharedPtr *out, const StdVector *titles);
    static UserTitle *destroy(UserTitle *self);          // ~UserTitle (D2)
    static uint32_t destroyDelete(UserTitle *self);      // ~UserTitle + operator delete (D0)
};
RW_LAYOUT(UserTitle::Title, titleId, 8);
RW_SIZE(UserTitle::Title, 16);
RW_LAYOUT(UserTitle, count, 32);
RW_LAYOUT(UserTitle, indexById, 36);
RW_SIZE(UserTitle, 56);

}  // namespace hole::network::api
