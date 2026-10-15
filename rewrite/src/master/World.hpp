// master::World: rows of the master table #48.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct World : Row {
    static constexpr uint32_t TABLE = 48;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaa988;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaa9b0;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe07c93;                  // "1f08ee2b84d47e9951076a85128160553f2c43ed"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0x3b4
    static constexpr uint32_t WORLD_ID = 0x0;               // int32
    static constexpr uint32_t WORLD_NAME = 0x4;             // char[129]
    static constexpr uint32_t RAID_BACKGROUND = 0x85;       // char[129]
    static constexpr uint32_t INSTANCE_LWF = 0x106;         // char[129]
    static constexpr uint32_t INSTANCE = 0x187;             // char[129]
    static constexpr uint32_t X_POS = 0x208;                // int32
    static constexpr uint32_t Y_POS = 0x20c;                // int32
    static constexpr uint32_t RATE = 0x210;                 // int32
    static constexpr uint32_t VALID_PARTS = 0x214;          // int32
    static constexpr uint32_t PARTS_ID = 0x218;             // char[3][129]
    static constexpr uint32_t X_POSTION = 0x39c;            // int32[3]
    static constexpr uint32_t Y_POSTION = 0x3a8;            // int32[3]

    static constexpr uint32_t PARSE_VTABLE = 0xfaa9e8;      // _ZTVN6master5WorldE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
