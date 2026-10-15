// master::Ranking: rows of the master table #34.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Ranking : Row {
    static constexpr uint32_t TABLE = 34;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfac254;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfac27c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09a30;                  // "eb066bf942d32afbb46e008d786ec67e11ae5108"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t RANKING_ID = 0;               // int32
    static constexpr uint32_t CATEGORY = 4;                 // int32
    static constexpr uint32_t START_DATETIME = 8;           // Date (8 bytes)
    static constexpr uint32_t END_DATETIME = 16;            // Date (8 bytes)
    static constexpr uint32_t START_RANK = 24;              // int32
    static constexpr uint32_t END_RANK = 28;                // int32
    static constexpr uint32_t UNION_START_RANK = 32;        // int32[5]
    static constexpr uint32_t UNION_END_RANK = 52;          // int32[5]
    static constexpr uint32_t VALID_REWARD = 72;            // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfac2a8;          // _ZTVN6master7RankingE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
