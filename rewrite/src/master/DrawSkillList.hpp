// master::DrawSkillList: rows of the master table #12.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct DrawSkillList : Row {
    static constexpr uint32_t TABLE = 12;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab174;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab19c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe087e9;                  // "5ff5faaa278012d8ccb9f919464f3cb1de361efc"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t DRAW_SKILL_LIST_ID = 0;       // int32
    static constexpr uint32_t TABLE_ID = 4;                 // int32
    static constexpr uint32_t SKILL_ID = 8;                 // int32
    static constexpr uint32_t WEIGHT = 12;                  // int32
    static constexpr uint32_t IS_DELETED = 16;              // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfab1c8;          // _ZTVN6master13DrawSkillListE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
