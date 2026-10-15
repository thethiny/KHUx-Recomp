// master::RaidReward: rows of the master table #32.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct RaidReward : Row {
    static constexpr uint32_t TABLE = 32;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfabb04;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfabb2c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe0929d;                  // "740531a343738802628d39f276d51d40a5b271c0"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t RAID_REWARD_ID = 0;           // int32
    static constexpr uint32_t ODDS_UP_PER_LV = 4;           // int32
    static constexpr uint32_t POINT_UP_PER_LV = 8;          // int32
    static constexpr uint32_t VALID_REWARD = 12;            // int32
    static constexpr uint32_t TYPE = 16;                    // int32[4]
    static constexpr uint32_t ID = 32;                      // int32[4]
    static constexpr uint32_t ASSIGN_SKILL_TYPE = 48;       // int32[4]
    static constexpr uint32_t ASSIGN_SKILL_ID = 64;         // int32[4]
    static constexpr uint32_t ASSIGN_SKILL_LV = 80;         // int32[4]
    static constexpr uint32_t NUM = 96;                     // int32[4]
    static constexpr uint32_t ODDS = 112;                   // int32[4]

    static constexpr uint32_t PARSE_VTABLE = 0xfabb58;          // _ZTVN6master10RaidRewardE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
