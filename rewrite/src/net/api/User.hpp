// User (response[section]["user"]), UserDetail (userData.userDetail) and StageResumption
// (userData.stageResumption): SafeBuffer-backed records.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

// The player's (or another user's) public record.
struct User : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F5C4;
    static constexpr uint32_t SIZE = 0x168;
    static constexpr uint32_t USER_ID = 0;                  // u64
    static constexpr uint32_t NATIVE_USER_ID = 8;           // u64
    static constexpr uint32_t PLATFORM_ID = 16;
    static constexpr uint32_t USER_NAME = 20;               // char[33]
    static constexpr uint32_t GENDER = 56;
    static constexpr uint32_t COMMENT = 60;                 // char[257]
    static constexpr uint32_t DEVICE_TYPE = 320;
    static constexpr uint32_t CONTINUE_LOGIN_COUNT = 324;
    static constexpr uint32_t IS_FLEEZE = 328;              // bool (1 byte)
    static constexpr uint32_t FLEEZED_DATETIME = 330;       // Date (8 bytes)
    static constexpr uint32_t NATIVE_TAG_NAME = 338;        // char[15]

    // json[section]["user"] -> *out. section is a guest C string; withTag == 1 also reads "nativeTagName".
    static uint32_t parse(SharedPtr *out, const JsonValue *json, uint32_t withTag, uint32_t section);
    static uint32_t destroyDelete(User *self);              // D0
};
RW_SIZE(User, 32);

struct UserDetail : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F58C;
    static constexpr uint32_t SIZE = 0x70;
    // ints at 0..28: level, exp, luxRank, luxGetRatio, titleLeftId, titleRightId, titlePlateId, maxDeckCost
    static constexpr uint32_t PLAY_TIMEZONE_COUNT = 32;     // min(count, 6)
    static constexpr uint32_t PLAY_FREQUENTLY = 36;
    static constexpr uint32_t PARTY_ID = 40;                // u64
    static constexpr uint32_t UNION_ID = 48;
    // ints at 52..80: maxMedal, mvpCount, equipCoordinateNo, lastClearStageId, lastPlay*SphereBoardId x4
    static constexpr uint32_t IS_GUILT = 84;                // bool (1 byte)
    static constexpr uint32_t PLAY_TIMEZONES = 88;          // int[6]

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static uint32_t destroyDelete(UserDetail *self);        // D0
};
RW_SIZE(UserDetail, 32);

// Where an interrupted quest can be resumed.
struct StageResumption : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F570;
    static constexpr uint32_t SIZE = 0x18;
    static constexpr uint32_t RESUMPTION_STATUS = 0;        // 0..3
    static constexpr uint32_t STAGE_ID = 4;
    static constexpr uint32_t RAID_ID = 8;                  // u64
    static constexpr uint32_t COLOSSEUM_STAGE_ID = 16;

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static uint32_t destroyDelete(StageResumption *self);   // D0
};
RW_SIZE(StageResumption, 32);

}  // namespace hole::network::api
