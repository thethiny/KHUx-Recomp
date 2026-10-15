// master::Buff: rows of the master table #5.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Buff : Row {
    static constexpr uint32_t TABLE = 5;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaad84;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaadac;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe07f17;                  // "4c050d4b9ce53f0a3e6196be3034191489cec5da"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0x58
    static constexpr uint32_t BUFF_ID = 0x0;                // int32
    static constexpr uint32_t GRADE = 0x4;                  // int32
    static constexpr uint32_t NAME = 0x8;                   // char[65]
    static constexpr uint32_t ICON_ID = 0x4c;               // int32
    static constexpr uint32_t EFFECT = 0x50;                // int32
    static constexpr uint32_t ADD_POWER = 0x54;             // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfaadd8;      // _ZTVN6master4BuffE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
