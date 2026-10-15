// master::DrawMedalList: rows of the master table #10.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct DrawMedalList : Row {
    static constexpr uint32_t TABLE = 10;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab054;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab07c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe086af;                  // "ba92dd9143c7b8e543caaa9fb4b7f1e0c1726c57"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0x20
    static constexpr uint32_t DRAW_MEDAL_LIST_ID = 0x0;     // int32
    static constexpr uint32_t TABLE_ID = 0x4;               // int32
    static constexpr uint32_t MEDAL_ID = 0x8;               // int32
    static constexpr uint32_t WEIGHT = 0xc;                 // int32
    static constexpr uint32_t SKILL_TYPE = 0x10;            // int32
    static constexpr uint32_t SKILL_ID = 0x14;              // int32
    static constexpr uint32_t BURST_UPPER_NUMBER = 0x18;    // int32
    static constexpr uint32_t IS_DELETED = 0x1c;            // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfab0a8;      // _ZTVN6master13DrawMedalListE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
