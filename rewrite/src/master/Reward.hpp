// master::Reward: rows of the master table #36.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Reward : Row {
    static constexpr uint32_t TABLE = 36;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfabcb4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfabcdc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe093e2;                  // "78b7651b94eee005b27038082be3137a0d372008"

    static constexpr uint32_t PARSE_VTABLE = 0xfabd08;          // _ZTVN6master6RewardE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t REWARD_ID = 0;                // int32
    static constexpr uint32_t VALID_REWARD = 4;             // int32
    static constexpr uint32_t DISPLAY = 8;                  // int32[4]
    static constexpr uint32_t TYPE = 24;                    // int32[4]
    static constexpr uint32_t ID = 40;                      // int32[4]
    static constexpr uint32_t ASSIGN_SKILL_TYPE = 56;       // int32[4]
    static constexpr uint32_t ASSIGN_SKILL_ID = 72;         // int32[4]
    static constexpr uint32_t ASSIGN_SKILL_LV = 88;         // int32[4]
    static constexpr uint32_t NUM = 104;                    // int32[4]
    static constexpr uint32_t ODDS = 120;                   // int32[4]

    // parse(json): std::shared_ptr<const Reward> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
