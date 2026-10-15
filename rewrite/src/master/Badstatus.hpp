// master::Badstatus: rows of the master table #3.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Badstatus : Row {
    static constexpr uint32_t TABLE = 3;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaac64;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaac8c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe07e9c;                  // "6e3e4fcfd48bf19d291ab6f55dbf414f83a998c7"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0x5c
    static constexpr uint32_t BADSTATUS_ID = 0x0;           // int32
    static constexpr uint32_t NAME = 0x4;                   // char[65]
    static constexpr uint32_t ICON_ID = 0x48;               // int32
    static constexpr uint32_t EFFECT = 0x4c;                // int32
    static constexpr uint32_t TARGET = 0x50;                // int32
    static constexpr uint32_t POWER = 0x54;                 // int32[2]

    static constexpr uint32_t PARSE_VTABLE = 0xfaacb8;      // _ZTVN6master9BadstatusE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
