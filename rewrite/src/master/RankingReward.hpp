// master::RankingReward: rows of the master table #35.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct RankingReward : Row {
    static constexpr uint32_t TABLE = 35;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfabc24;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfabc4c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe093b9;                  // "2716ded18ab454982ae41126868901da75ab1358"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfabc78;                // _ZTVN6master13RankingRewardE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x78B40D;       // shared_ptr<const RankingReward>(RankingReward*)
    static constexpr uint32_t SIZE = 0x100;

    // data positions (JSON keys)
    static constexpr uint32_t RANKING_REWARD_ID = 0;    // int32
    static constexpr uint32_t START_RANK = 4;           // int32
    static constexpr uint32_t END_RANK = 8;             // int32
    static constexpr uint32_t VALID_REWARD = 12;        // int32
    static constexpr uint32_t TYPE = 16;                // int32[10]
    static constexpr uint32_t ID = 56;                  // int32[10]
    static constexpr uint32_t SKILL_TYPE = 96;          // int32[10]
    static constexpr uint32_t SKILL_ID = 136;           // int32[10]
    static constexpr uint32_t SKILL_LV = 176;           // int32[10]
    static constexpr uint32_t NUM = 216;                // int32[10]

    // shared_ptr<const RankingReward> from one JSON row (null when a field is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
