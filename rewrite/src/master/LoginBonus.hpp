// master::LoginBonus: rows of the master table #23.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct LoginBonus : Row {
    static constexpr uint32_t TABLE = 23;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab684;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab6ac;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08ecd;                  // "100c80349a0a7239818d501c253eb75bd9a247b5"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0x938
    static constexpr uint32_t LOGIN_BONUS_ID = 0x0;         // int32
    static constexpr uint32_t BONUS_TYPE = 0x4;             // int32
    static constexpr uint32_t TITLE = 0x8;                  // char[257]
    static constexpr uint32_t DESCRIPTION = 0x109;          // char[1025]
    static constexpr uint32_t COMMENT = 0x50a;              // char[1025]
    static constexpr uint32_t DAY = 0x90c;                  // int32
    static constexpr uint32_t START_DATE = 0x910;           // Date (8 bytes)
    static constexpr uint32_t END_DATE = 0x918;             // Date (8 bytes)
    static constexpr uint32_t REWARD_TYPE = 0x920;          // int32
    static constexpr uint32_t REWARD_ID = 0x924;            // int32
    static constexpr uint32_t REWARD_ASSIGN_SKILL_TYPE = 0x928;// int32
    static constexpr uint32_t REWARD_ASSIGN_SKILL_ID = 0x92c;// int32
    static constexpr uint32_t REWARD_ASSIGN_SKILL_LV = 0x930;// int32
    static constexpr uint32_t REWARD_NUM = 0x934;           // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfab6d8;      // _ZTVN6master10LoginBonusE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
