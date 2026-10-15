// master::SphereMasu: rows of the master table #43.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct SphereMasu : Row {
    static constexpr uint32_t TABLE = 43;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfabf84;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfabfac;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09657;                  // "bac8e4c3cfee37776ae658d60e7c177f0c8d766f"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t MASU_ID = 0;                  // int32
    static constexpr uint32_t SPHERE_BOARD_ID = 4;          // int32
    static constexpr uint32_t SPHERE_POINT = 8;             // int32[23]
    static constexpr uint32_t REWARD_SELECT = 100;          // int32[23]
    static constexpr uint32_t MEDAL_CATEGORY = 192;         // int32[23]
    static constexpr uint32_t REWARD_ID = 284;              // int32[23]
    static constexpr uint32_t REWARD_EXPAND = 376;          // int32[23]
    static constexpr uint32_t REWARD_EXPAND_ID = 468;       // int32[23]
    static constexpr uint32_t REWARD_VALUE = 560;           // int32[23]

    static constexpr uint32_t PARSE_VTABLE = 0xfabfd8;          // _ZTVN6master10SphereMasuE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
