// UserPoint: the player's currencies and stats (userData.userPoint of many responses), kept in a SafeBuffer.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserPoint : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F5A8;     // UserPoint vtable (+8)
    static constexpr uint32_t SIZE = 0x58;           // SafeBuffer data bytes

    // data positions
    static constexpr uint32_t MONEY = 0;                  // int
    static constexpr uint32_t LUX = 8;                    // int64
    static constexpr uint32_t TOTAL_LUX = 16;             // int64
    static constexpr uint32_t SPHERE_POINT = 24;
    static constexpr uint32_t KIZUNA_POINT = 28;
    static constexpr uint32_t RAID_POINT = 32;
    static constexpr uint32_t ATTACK = 36;
    static constexpr uint32_t DEFENSE = 40;
    static constexpr uint32_t BASE_HP = 44;
    static constexpr uint32_t HP = 48;
    static constexpr uint32_t AP = 52;
    static constexpr uint32_t MAX_HP = 56;
    static constexpr uint32_t MAX_AP = 60;
    static constexpr uint32_t LAST_AP_DATETIME = 64;      // Date (8 bytes)
    static constexpr uint32_t STAGE_SPHERE_POINT = 72;
    static constexpr uint32_t RAID_SPHERE_POINT = 76;
    static constexpr uint32_t COLOSSEUM_SPHERE_POINT = 80;
    static constexpr uint32_t STAGE_SKIP_TICKET = 84;

    // response["userData"]["userPoint"] -> *out (empty when absent or incomplete)
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(UserPoint, 32);

}  // namespace hole::network::api
