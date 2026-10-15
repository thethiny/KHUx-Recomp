// RaidStart (action 112: the raid boss, the players already fighting it, the logs, the request echo) and
// RaidClear (action 113: what a finished raid battle gave).
#pragma once

#include "Raid.hpp"

namespace hole::network::api {

struct RaidStart : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F490;
    static constexpr uint32_t INFO_SIZE = 0xA8;       // SafeBuffer data: a copy of RaidBase::Info

    // A player in the raid (players / vitalsLog / partsLog entries).
    struct Player {
        AvatarCoordinate avatar;              // +0  "userAvatar"
        rw::Field<int32_t> damage;            // +48
        StdString name;                       // +52
        rw::Field<int32_t> partsId;           // +64
        rw::Field<int32_t> keybladeId;        // +68

        static uint32_t init(Player *self, const JsonValue *json);
    };
    // The echo of the request ("startRaidData").
    struct RequestData {
        rw::Field<uint64_t> raidId;           // +0
        rw::Field<uint64_t> supportUserId;    // +8
        rw::Field<uint64_t> userKeybladeId;   // +16

        static uint32_t init(RequestData *self, const JsonValue *json);
    };

    StdVector players;                        // +0x20 std::vector<Player>
    const uint8_t pad_2c[4];
    RequestData request;                      // +0x30
    StdVector vitalsLog;                      // +0x48 std::vector<Player>
    StdVector partsLog;                       // +0x54 std::vector<Player>
    rw::Field<uint64_t> discovererUserId;     // +0x60
    StdVector campaigns;                      // +0x68 std::vector<int>
    const uint8_t pad_74[4];

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static RaidStart *destroy(RaidStart *self);              // D2
    static uint32_t destroyDelete(RaidStart *self);          // D0
};
RW_LAYOUT(RaidStart::Player, damage, 48);
RW_LAYOUT(RaidStart::Player, name, 52);
RW_SIZE(RaidStart::Player, 72);
RW_SIZE(RaidStart::RequestData, 24);
RW_LAYOUT(RaidStart, players, 0x20);
RW_LAYOUT(RaidStart, request, 0x30);
RW_LAYOUT(RaidStart, vitalsLog, 0x48);
RW_LAYOUT(RaidStart, partsLog, 0x54);
RW_LAYOUT(RaidStart, discovererUserId, 0x60);
RW_LAYOUT(RaidStart, campaigns, 0x68);
RW_SIZE(RaidStart, 0x78);

struct RaidClear {
    rw::Field<int32_t> raidTimeStatus;        // +0 1..5
    rw::Field<uint8_t> reliefFlag;            // +4
    const uint8_t pad_5[3];
    SharedPtr skills;                         // +8  UserSkill
    SharedPtr titles;                         // +16 UserTitle
    SharedPtr keyblades;                      // +24 UserKeyblade
    SharedPtr property[5];                    // +32 StageClear::Property (five shared_ptrs)

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static RaidClear *destroy(RaidClear *self);              // D2
};
RW_LAYOUT(RaidClear, skills, 8);
RW_LAYOUT(RaidClear, property, 32);
RW_SIZE(RaidClear, 0x48);

}  // namespace hole::network::api
