// UserSphere: the player's sphere boards (the skill-tree boards bought with sphere points) from
// response["userSphere"]: {"userSphereDatas": [...], "notChargedSphereBoardIds": [...]}.
// The boards are kept in a std::map<sphereBoardId, Sphere>.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserSphere {
    // One board ("userSphereDatas" element).
    struct Sphere {
        rw::Field<int32_t> userSphereBoardId;   // +0
        rw::Field<int32_t> sphereBoardId;       // +4 master board id (the map key)
        rw::Field<uint8_t> isBuy;               // +8 bought (the JSON int != 0)
        const uint8_t pad_9[3];
        rw::Field<int32_t> status;              // +12
        rw::Field<uint32_t> getDatetime;        // +16 seconds since 1970 (Date::calculateIntervalSince1970)
        StdVector unlockMasuIds;                // +20 std::vector<int>: unlocked squares of the board

        // fill from one JSON board: 1 when every field is there, else 0 (fields set so far stay)
        static uint32_t init(Sphere *self, const JsonValue *json);
    };

    // std::map<int, Sphere> (libc++ __tree: begin node, root = end node's left child, size)
    rw::Field<uint32_t> mapBegin;               // +0
    rw::Field<uint32_t> mapRoot;                // +4 (the end node lives here: &mapRoot = end())
    rw::Field<uint32_t> mapSize;                // +8
    StdVector notChargedSphereBoardIds;         // +12 std::vector<int>

    // response["userSphere"] -> *out. skipNotCharged: do not read "notChargedSphereBoardIds".
    static uint32_t parse(SharedPtr *out, const JsonValue *response, bool skipNotCharged);
    // replace (erase + insert) the board with sphere.sphereBoardId
    static uint32_t setSphere(UserSphere *self, const Sphere *sphere);
    // erase the board with that sphereBoardId
    static uint32_t removeSphere(UserSphere *self, int32_t sphereBoardId);
};
RW_LAYOUT(UserSphere::Sphere, isBuy, 8);
RW_LAYOUT(UserSphere::Sphere, getDatetime, 16);
RW_LAYOUT(UserSphere::Sphere, unlockMasuIds, 20);
RW_SIZE(UserSphere::Sphere, 32);
RW_LAYOUT(UserSphere, notChargedSphereBoardIds, 12);
RW_SIZE(UserSphere, 24);

}  // namespace hole::network::api
