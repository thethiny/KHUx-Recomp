// master::MypageBackground: rows of the master table #28.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct MypageBackground : Row {
    static constexpr uint32_t TABLE = 28;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfac404;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfac42c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09b0a;                  // "2d1aa44433dfde6347f61b02b644bd9052899083"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0xa0
    static constexpr uint32_t ID = 0x0;                     // int32
    static constexpr uint32_t BACKGROUND_ID = 0x4;          // int32
    static constexpr uint32_t SORT_ID = 0x8;                // int32
    static constexpr uint32_t VALID_CHILD = 0xc;            // int32
    static constexpr uint32_t START_TIME = 0x10;            // Date (8 bytes)
    static constexpr uint32_t END_TIME = 0x18;              // Date (8 bytes)
    static constexpr uint32_t VIEW_TYPE = 0x20;             // int32
    static constexpr uint32_t VALID_PARTS = 0x24;           // int32
    static constexpr uint32_t PARTS_ID = 0x28;              // int32[6]
    static constexpr uint32_t DATA_TYPE = 0x40;             // int32[6]
    static constexpr uint32_t X_POSTION = 0x58;             // int32[6]
    static constexpr uint32_t Y_POSTION = 0x70;             // int32[6]
    static constexpr uint32_t Z_SORT = 0x88;                // int32[6]

    static constexpr uint32_t PARSE_VTABLE = 0xfac458;      // _ZTVN6master16MypageBackgroundE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
