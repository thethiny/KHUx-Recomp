// master::AvatarCombination: rows of the master table #1.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct AvatarCombination : Row {
    static constexpr uint32_t TABLE = 1;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaab44;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaab6c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe07de1;                  // "44267ff9a85c57c6f346fe73fe0d4f3c8aaa66a8"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0x10
    static constexpr uint32_t AVATAR_COMBINATION_ID = 0x0;  // int32
    static constexpr uint32_t COMBINATION_TYPE = 0x4;       // int32
    static constexpr uint32_t AVATAR_PARTS_ID = 0x8;        // int32
    static constexpr uint32_t STATUS = 0xc;                 // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfaab98;      // _ZTVN6master17AvatarCombinationE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
