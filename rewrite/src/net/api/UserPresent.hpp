// UserPresent: the present box ({"userPresent": {"userPresentDatas": [...], "presentCount", "presentCountMax"}}),
// a std::map<userPresentId, Present>.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserPresent {
    struct Present {
        rw::Field<uint64_t> userPresentId;    // +0
        rw::Field<int32_t> resourceType;      // +8
        rw::Field<int32_t> resourceId;        // +12
        rw::Field<int32_t> number;            // +16
        const uint8_t limitDatetime[8];             // +20 Date
        StdString comment;                    // +28
        const uint8_t created[8];                   // +40 Date
        rw::Field<uint8_t> received;          // +48
        const uint8_t pad_49[7];

        // returns Date::init's result for "created" once everything else is in (0 before that)
        static uint32_t init(Present *self, const JsonValue *json);
    };

    // std::map<unsigned long long, Present>: begin node, end node (its left = the root), size
    rw::Field<uint32_t> beginNode;            // +0
    rw::Field<uint32_t> root;                 // +4 (the end node's left child)
    rw::Field<uint32_t> size;                 // +8
    rw::Field<int32_t> presentCount;          // +12
    rw::Field<int32_t> presentCountMax;       // +16

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    // mark a present received: 1 when the id is in the map, else 0
    static uint32_t setReceived(UserPresent *self, uint64_t userPresentId);
};
RW_LAYOUT(UserPresent::Present, comment, 28);
RW_LAYOUT(UserPresent::Present, created, 40);
RW_LAYOUT(UserPresent::Present, received, 48);
RW_SIZE(UserPresent::Present, 56);
RW_SIZE(UserPresent, 20);

}  // namespace hole::network::api
