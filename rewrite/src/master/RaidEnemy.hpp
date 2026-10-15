// master::RaidEnemy: rows of the master table #31.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct RaidEnemy : Row {
    static constexpr uint32_t TABLE = 31;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab9e4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaba0c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe0920e;                  // "bbbad38dd75b71502982afefd62394e35b23fff4"

    static constexpr uint32_t PARSE_VTABLE = 0xfaba38;          // _ZTVN6master9RaidEnemyE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t RAID_ENEMY_ID = 0;            // int32
    static constexpr uint32_t NAME = 4;                     // char[129]
    static constexpr uint32_t ATTRIBUTE = 136;              // int32
    static constexpr uint32_t RARE = 140;                   // int32
    static constexpr uint32_t WEAKNESS = 144;               // int32
    static constexpr uint32_t VALID_PARTS = 148;            // int32
    static constexpr uint32_t PARTS = 152;                  // int32[5]
    static constexpr uint32_t VALID_GROWTH = 172;           // int32
    static constexpr uint32_t BASE_LV = 176;                // int32[5]
    static constexpr uint32_t HP = 196;                     // int32[5]
    static constexpr uint32_t ATTACK = 216;                 // int32[5]
    static constexpr uint32_t DEFENSE = 236;                // int32[5]
    static constexpr uint32_t EXP_ATTACK = 256;             // int32[5]
    static constexpr uint32_t EXP_SUPPRESS = 276;           // int32[5]
    static constexpr uint32_t MONEY_ATTACK = 296;           // int32[5]
    static constexpr uint32_t MONEY_SUPPRESS = 316;         // int32[5]
    static constexpr uint32_t LUX_ATTACK = 336;             // int32[5]
    static constexpr uint32_t LUX_SUPPRESS = 356;           // int32[5]
    static constexpr uint32_t BURST_ATTACK = 376;           // int32[5]
    static constexpr uint32_t BURST_SUPPRESS = 396;         // int32[5]
    static constexpr uint32_t HP_ATTACK = 416;              // int32[5]
    static constexpr uint32_t HP_SUPPRESS = 436;            // int32[5]
    static constexpr uint32_t GUILT_SUPPRESS = 456;         // int32[5]
    static constexpr uint32_t REWARD_ID = 476;              // int32[5]
    static constexpr uint32_t MVP_REWARD_ID = 496;          // int32[5]
    static constexpr uint32_t DESTROY_ALL_REWARD_ID = 516;  // int32[5]
    static constexpr uint32_t DRAW_HEIGHT = 536;            // int32
    static constexpr uint32_t VALID_SKILL = 540;            // int32
    static constexpr uint32_t SKILL_ID = 544;               // int32[4]
    static constexpr uint32_t SKILL_REQUIRE = 560;          // int32[4]
    static constexpr uint32_t SKILL_REQUIRE_ARG = 576;      // int32[4]
    static constexpr uint32_t SKILL_ODDS = 592;             // int32[4]
    static constexpr uint32_t PROTECT_POISON = 608;         // int32
    static constexpr uint32_t PROTECT_DEEP_POISON = 612;    // int32
    static constexpr uint32_t PROTECT_SLEEP = 616;          // int32
    static constexpr uint32_t PROTECT_PARALYSIS = 620;      // int32
    static constexpr uint32_t PROTECT_CONFUSION = 624;      // int32
    static constexpr uint32_t REGIST_POISON = 628;          // int32
    static constexpr uint32_t REGIST_DEEP_POISON = 632;     // int32
    static constexpr uint32_t REGIST_SLEEP = 636;           // int32
    static constexpr uint32_t REGIST_PARALYSIS = 640;       // int32
    static constexpr uint32_t REGIST_CONFUSION = 644;       // int32
    static constexpr uint32_t BUF_ATTACK = 648;             // int32
    static constexpr uint32_t BUF_DEFENSE = 652;            // int32
    static constexpr uint32_t BUF_ATTACK_POWER = 656;       // int32
    static constexpr uint32_t BUF_DEFENSE_POWER = 660;      // int32
    static constexpr uint32_t BUF_ATTACK_SPEED = 664;       // int32
    static constexpr uint32_t BUF_DEFENSE_SPEED = 668;      // int32
    static constexpr uint32_t BUF_ATTACK_MAGIC = 672;       // int32
    static constexpr uint32_t BUF_DEFENSE_MAGIC = 676;      // int32
    static constexpr uint32_t BUF_TURN = 680;               // int32
    static constexpr uint32_t BUF_COUNT = 684;              // int32
    static constexpr uint32_t STATUS_AILMENTS = 688;        // int32[2]
    static constexpr uint32_t STATUS_AILMENTS_TURN = 696;   // int32
    static constexpr uint32_t STATUS_AILMENTS_COUNT = 700;  // int32
    static constexpr uint32_t BGM_RAID = 704;               // int32

    // parse(json): std::shared_ptr<const RaidEnemy> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
