// master::Colosseum: rows of the master table #8.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Colosseum : Row {
    static constexpr uint32_t TABLE = 8;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaaf34;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaaf5c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe085b8;                  // "f201b60aa65790ad863b4cc7af282cc319ac666f"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0xdc
    static constexpr uint32_t COLOSSEUM_ID = 0x0;           // int32
    static constexpr uint32_t COLOSSEUM_STAGE_ID = 0x4;     // int32
    static constexpr uint32_t ROUND = 0x8;                  // int32
    static constexpr uint32_t AP = 0xc;                     // int32
    static constexpr uint32_t ARRIVAL_STAGE_COUNT = 0x10;   // int32
    static constexpr uint32_t RANK_ID = 0x14;               // char[65]
    static constexpr uint32_t EXCELLENT_UNLOCK_COUNT = 0x58;// int32
    static constexpr uint32_t VALID_STAGE_REWARD = 0x5c;    // int32
    static constexpr uint32_t STAGE_REWARD_TYPE = 0x60;     // int32
    static constexpr uint32_t STAGE_REWARD_ID = 0x64;       // int32
    static constexpr uint32_t STAGE_REWARD_ASSIGN_TYPE = 0x68;// int32
    static constexpr uint32_t STAGE_REWARD_SKILL_ID = 0x6c; // int32
    static constexpr uint32_t STAGE_REWARD_SKILL_LV = 0x70; // int32
    static constexpr uint32_t STAGE_REWARD_NUM = 0x74;      // int32
    static constexpr uint32_t VALID_REWARD = 0x78;          // int32
    static constexpr uint32_t REWARD_TYPE = 0x7c;           // int32[4]
    static constexpr uint32_t REWARD_ID = 0x8c;             // int32[4]
    static constexpr uint32_t REWARD_ASSIGN_TYPE = 0x9c;    // int32[4]
    static constexpr uint32_t REWARD_SKILL_ID = 0xac;       // int32[4]
    static constexpr uint32_t REWARD_SKILL_LV = 0xbc;       // int32[4]
    static constexpr uint32_t REWARD_NUM = 0xcc;            // int32[4]

    static constexpr uint32_t PARSE_VTABLE = 0xfaaf88;      // _ZTVN6master9ColosseumE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
