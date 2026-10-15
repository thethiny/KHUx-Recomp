// master::Sphere: rows of the master table #42.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Sphere : Row {
    static constexpr uint32_t TABLE = 42;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfabe64;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfabe8c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe0959f;                  // "15680bf2ea3f4d9ee20faefacb449709799fc66f"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0xb8
    static constexpr uint32_t SPHERE_BOARD_ID = 0x0;        // int32
    static constexpr uint32_t BOARD_NAME = 0x4;             // char[129]
    static constexpr uint32_t KIND_ID = 0x88;               // int32
    static constexpr uint32_t BOARD_ID = 0x8c;              // int32
    static constexpr uint32_t ARRY_ID = 0x90;               // int32
    static constexpr uint32_t MASU_ID = 0x94;               // int32
    static constexpr uint32_t TYPE = 0x98;                  // int32
    static constexpr uint32_t OPEN_ID = 0x9c;               // int32
    static constexpr uint32_t OPEN_PARAM = 0xa0;            // int32
    static constexpr uint32_t PRICE = 0xa4;                 // int32
    static constexpr uint32_t START_DATETIME = 0xa8;        // Date (8 bytes)
    static constexpr uint32_t END_DATETIME = 0xb0;          // Date (8 bytes)

    static constexpr uint32_t PARSE_VTABLE = 0xfabeb8;      // _ZTVN6master6SphereE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
