// StageStart (action 103 /stage/start: the quest's random enemies, enemy drops, treasures, the request echo and the
// running campaigns) and StageClear (action 104: the rewards and the lists the clear changed).
#pragma once

#include "StageCommon.hpp"

namespace hole::network::api {

struct StageStart : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F4C8;
    // SafeBuffer data: randomEnemyCount RandomEnemy records (4 bytes), then enemyDropCount EnemyDropItem records and
    // treasureCount Treasure records (24 bytes each)

    struct RandomEnemy {
        rw::Field<int32_t> uniqueSymbolEnemyId;   // +0
        static uint32_t init(RandomEnemy *self, const JsonValue *json);
    };
    // EnemyDropItem and Treasure share their layout: an id, then up to 4 drop item type ids.
    struct DropList {
        rw::Field<int32_t> id;                    // +0 uniqueEnemyId / uniqueTreasureId
        rw::Field<uint32_t> count;                // +4 min(dropItemTypeIds.size, 4)
        rw::Field<int32_t> dropItemTypeIds[4];    // +8 (every entry is copied: more than 4 overrun)
    };
    struct EnemyDropItem : DropList {
        static uint32_t init(EnemyDropItem *self, const JsonValue *json);
    };
    struct Treasure : DropList {
        static uint32_t init(Treasure *self, const JsonValue *json);
    };
    // The echo of the request ("startStageData").
    struct RequestData {
        rw::Field<int32_t> stageId;               // +0
        const uint8_t pad_4[4];
        rw::Field<uint64_t> supportUserId;        // +8
        rw::Field<uint64_t> userKeybladeId;       // +16
        rw::Field<uint8_t> stageSkip;             // +24 (unsigned in the JSON)
        const uint8_t pad_25[3];
        StdVector clearMissionIds;                // +28 std::vector<int>

        static uint32_t init(RequestData *self, const JsonValue *json);
    };

    rw::Field<uint32_t> randomEnemyCount;         // +32
    rw::Field<uint32_t> enemyDropCount;           // +36
    rw::Field<uint32_t> treasureCount;            // +40
    const uint8_t pad_44[4];
    RequestData request;                          // +48
    StdVector campaigns;                          // +88 std::vector<int>
    const uint8_t pad_100[4];

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static StageStart *destroy(StageStart *self);               // D2
    static uint32_t destroyDelete(StageStart *self);            // D0
};
RW_SIZE(StageStart::DropList, 24);
RW_LAYOUT(StageStart::RequestData, stageSkip, 24);
RW_LAYOUT(StageStart::RequestData, clearMissionIds, 28);
RW_SIZE(StageStart::RequestData, 40);
RW_LAYOUT(StageStart, request, 48);
RW_LAYOUT(StageStart, campaigns, 88);
RW_SIZE(StageStart, 0x68);

struct StageClear : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F4AC;
    // SafeBuffer data: count Reward records (24 bytes), plus one byte; firstClearFlag is written at position 0,
    // over the first byte of reward 0 (original quirk)

    struct Reward {
        rw::Field<int32_t> rewardkind;            // +0
        const uint8_t pad_4[4];
        rw::Field<uint64_t> userMedalId;          // +8
        rw::Field<int32_t> display;               // +16
        const uint8_t pad_20[4];

        static uint32_t init(Reward *self, const JsonValue *json);
    };
    // The player's lists as APIManager holds them, captured at clear time (five shared_ptrs).
    struct Property {
        SharedPtr lists[5];                       // copies of APIManager +0x8c, +0x94, +0xdc, +0xa4, +0xec

        static uint32_t init(Property *self);
        static Property *assign(Property *self, const Property *other);   // operator=(const Property&)
    };

    rw::Field<uint32_t> count;                    // +32
    SharedPtr materials;                          // +36 UserMaterial
    SharedPtr medals;                             // +44 UserMedal
    SharedPtr skills;                             // +52 UserSkill
    SharedPtr titles;                             // +60 UserTitle
    SharedPtr keyblades;                          // +68 UserKeyblade
    SharedPtr unused;                             // +76
    Property property;                            // +84

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static StageClear *destroy(StageClear *self);               // D2
    static uint32_t destroyDelete(StageClear *self);            // D0
};
RW_SIZE(StageClear::Reward, 24);
RW_SIZE(StageClear::Property, 40);
RW_LAYOUT(StageClear, materials, 36);
RW_LAYOUT(StageClear, property, 84);
RW_SIZE(StageClear, 0x7C);

}  // namespace hole::network::api
