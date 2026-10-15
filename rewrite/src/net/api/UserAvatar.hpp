// UserAvatar, UserAvatarAll, UserAvatarParts: the player's avatar (Union χ character customisation).
//   UserAvatar       the current coordinate (userAvatar: one Coordinate in a SafeBuffer)
//   UserAvatarAll    every saved coordinate (userAvatars: Coordinates, indexed by myCoordinateNo)
//   UserAvatarParts  the owned avatar parts (userAvatarParts: Parts, indexed by userAvatarPartsId)
#pragma once

#include "UserRecordList.hpp"

namespace hole::network::api {

struct UserAvatar : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F554;     // UserAvatar vtable (+8)

    // One avatar coordinate (outfit); also the SafeBuffer layout (UserAvatar: data position 0).
    struct Coordinate {
        rw::Field<int32_t> myCoordinateNo;        // +0 slot number
        rw::Field<int32_t> gender;                // +4
        rw::Field<int32_t> hairPartsId;           // +8 avatarParts master ids
        rw::Field<int32_t> hairColorPartsId;      // +12
        rw::Field<int32_t> facePartsId;           // +16
        rw::Field<int32_t> bodyPartsId;           // +20
        rw::Field<int32_t> skinPartsId;           // +24
        rw::Field<int32_t> accessoriesPartsIds[5];   // +28 (unused slots 0)

        // the seven ints, then "accessoriesPartsIds" (an int array): 1 when all are there, else 0 (fields set so
        // far stay). The array is not bounds-checked: more than five ids run over the record.
        static uint32_t init(Coordinate *self, const JsonValue *json);
    };

    // response["userAvatar"] (an object) -> *out; null when the response or the coordinate is incomplete
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static uint32_t destroyDelete(UserAvatar *self);           // ~UserAvatar + operator delete (D0)
};
RW_LAYOUT(UserAvatar::Coordinate, accessoriesPartsIds, 28);
RW_SIZE(UserAvatar::Coordinate, 48);
RW_SIZE(UserAvatar, 32);

struct UserAvatarAll : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F538;     // UserAvatarAll vtable (+8)

    rw::Field<uint32_t> count;                // +32 coordinates (48 bytes each in the SafeBuffer)
    const uint8_t indexByNo[20];                    // +36 std::unordered_map<int myCoordinateNo, int index>

    // response["userAvatars"] (an array of Coordinates) -> *out; empty when absent, null when one is incomplete
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static UserAvatarAll *destroy(UserAvatarAll *self);        // ~UserAvatarAll (D2)
    static uint32_t destroyDelete(UserAvatarAll *self);        // ~UserAvatarAll + operator delete (D0)
};
RW_LAYOUT(UserAvatarAll, count, 32);
RW_LAYOUT(UserAvatarAll, indexByNo, 36);
RW_SIZE(UserAvatarAll, 56);

struct UserAvatarParts : UserRecordList {
    static constexpr uint32_t VTABLE = 0xF7F1DC;     // UserAvatarParts vtable (+8)

    // One owned part (also the SafeBuffer layout: record k at data position 24 * k).
    struct Part {
        rw::Field<uint64_t> userAvatarPartsId;    // +0
        rw::Field<int32_t> partsType;             // +8
        rw::Field<int32_t> avatarPartsId;         // +12 avatarParts master id
        rw::Field<int32_t> getDatetime;           // +16 seconds since 1970
        const uint8_t pad_20[4];

        // {"userAvatarPartsId", "partsType", "avatarPartsId", "getDatetime"}: 1 when all are there, else 0
        static uint32_t init(Part *self, const JsonValue *json);
    };

    // response["userAvatarParts"] (an array) -> *out; empty when absent, null when a part is incomplete
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    // a UserAvatarParts holding the given parts (make_shared)
    static uint32_t create(SharedPtr *out, const StdVector *parts);
    static UserAvatarParts *destroy(UserAvatarParts *self);    // ~UserAvatarParts (D2)
    static uint32_t destroyDelete(UserAvatarParts *self);      // ~UserAvatarParts + operator delete (D0)
};
RW_LAYOUT(UserAvatarParts::Part, getDatetime, 16);
RW_SIZE(UserAvatarParts::Part, 24);
RW_SIZE(UserAvatarParts, 56);

}  // namespace hole::network::api
