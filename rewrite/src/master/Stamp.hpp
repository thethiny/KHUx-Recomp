// master::Stamp: rows of the master table #45.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Stamp : Row {
    static constexpr uint32_t TABLE = 45;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfac2e4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfac30c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09a83;                  // "25fd55c2f55d941096506434e1c032581afb1241"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t STAMP_ID = 0;                 // int32
    static constexpr uint32_t CATEGORY = 4;                 // int32
    static constexpr uint32_t IMAGE_ID = 8;                 // int32
    static constexpr uint32_t REQUIRE = 12;                 // int32
    static constexpr uint32_t SORT_ID = 16;                 // int32
    static constexpr uint32_t IMAGE_WIDTH = 20;             // int32
    static constexpr uint32_t IMAGE_HEIGHT = 24;            // int32
    static constexpr uint32_t VALID_STAMP = 28;             // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfac338;          // _ZTVN6master5StampE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
