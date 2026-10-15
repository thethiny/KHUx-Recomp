// master::DrawSkillType: rows of the master table #13.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct DrawSkillType : Row {
    static constexpr uint32_t TABLE = 13;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab204;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab22c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe0884f;                  // "36bcf33508e1b8c8d13a48847d6e7767c5cea0a7"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0x20
    static constexpr uint32_t DRAW_SKILL_TYPE_ID = 0x0;     // int32
    static constexpr uint32_t TABLE_ID = 0x4;               // int32
    static constexpr uint32_t LOT_RARE3 = 0x8;              // int32
    static constexpr uint32_t LOT_RARE2 = 0xc;              // int32
    static constexpr uint32_t LOT_RARE1 = 0x10;             // int32
    static constexpr uint32_t LOT_NON = 0x14;               // int32
    static constexpr uint32_t LOT_RESERVE = 0x18;           // int32
    static constexpr uint32_t IS_DELETED = 0x1c;            // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfab258;      // _ZTVN6master13DrawSkillTypeE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
