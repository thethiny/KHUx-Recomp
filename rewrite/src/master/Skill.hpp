// master::Skill: rows of the master table #40.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Skill : Row {
    static constexpr uint32_t TABLE = 40;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfabd44;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfabd6c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09501;                  // "a95a1025697da5f0e67e771c544c3aed5cea0e2f"

    static constexpr uint32_t PARSE_VTABLE = 0xfabd98;          // _ZTVN6master5SkillE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t SKILL_ID = 0;                 // int32
    static constexpr uint32_t IMAGE_ID = 4;                 // int32
    static constexpr uint32_t THUMB_ID = 8;                 // int32
    static constexpr uint32_t NAME = 12;                    // char[65]
    static constexpr uint32_t DESCRIPTION = 77;             // char[129]
    static constexpr uint32_t RANK = 208;                   // int32
    static constexpr uint32_t MAX_LV = 212;                 // int32
    static constexpr uint32_t EXP_TYPE = 216;               // int32
    static constexpr uint32_t SORT_ID = 220;                // int32
    static constexpr uint32_t CATEGORY_EVOLVE = 224;        // int32
    static constexpr uint32_t INVOKE_TYPE = 228;            // int32
    static constexpr uint32_t INVOKE_PROB = 232;            // int32
    static constexpr uint32_t INVOKE_PROB_PER_LV = 236;     // int32
    static constexpr uint32_t INVOKE_SHOW_TIMING = 240;     // int32
    static constexpr uint32_t ATTACK_UP = 244;              // int32
    static constexpr uint32_t BURST_CANCEL = 248;           // int32
    static constexpr uint32_t DAMAGE_CUT = 252;             // int32
    static constexpr uint32_t DOKONJO = 256;                // int32
    static constexpr uint32_t COUNTER = 260;                // int32
    static constexpr uint32_t ADD_ACTION = 264;             // int32
    static constexpr uint32_t ADD_POISON = 268;             // int32
    static constexpr uint32_t ADD_DEEP_POISON = 272;        // int32
    static constexpr uint32_t ADD_SLEEP = 276;              // int32
    static constexpr uint32_t ADD_PARALYSIS = 280;          // int32
    static constexpr uint32_t CONDITION_TURN = 284;         // int32
    static constexpr uint32_t CONDITION_COUNT = 288;        // int32
    static constexpr uint32_t PRIZE_HP_UP = 292;            // int32
    static constexpr uint32_t PRIZE_BURST_UP = 296;         // int32
    static constexpr uint32_t LUX_UP = 300;                 // int32
    static constexpr uint32_t MONEY_UP = 304;               // int32
    static constexpr uint32_t RAID_UP = 308;                // int32

    // parse(json): std::shared_ptr<const Skill> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
