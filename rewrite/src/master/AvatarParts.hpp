// master::AvatarParts: rows of the master table #2.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct AvatarParts : Row {
    static constexpr uint32_t TABLE = 2;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaabd4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaabfc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe07e4c;                  // "969abc93609b4c1e59593b9d4d703af4e5cbae95"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0xc0
    static constexpr uint32_t AVATAR_PARTS_ID = 0x0;        // int32
    static constexpr uint32_t NAME = 0x4;                   // char[129]
    static constexpr uint32_t PARTS_TYPE = 0x88;            // int32
    static constexpr uint32_t GENDER = 0x8c;                // int32
    static constexpr uint32_t COMBINATION_TYPE = 0x90;      // int32
    static constexpr uint32_t COMBINATION_FLAG = 0x94;      // int32
    static constexpr uint32_t POSITION = 0x98;              // int32
    static constexpr uint32_t SET_KIND = 0x9c;              // int32
    static constexpr uint32_t FIXED_FLAG = 0xa0;            // int32
    static constexpr uint32_t VALID_SET_CLOTH = 0xa4;       // int32
    static constexpr uint32_t SET_CLOTH = 0xa8;             // int32[5]
    static constexpr uint32_t STATUS = 0xbc;                // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfaac28;      // _ZTVN6master11AvatarPartsE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
