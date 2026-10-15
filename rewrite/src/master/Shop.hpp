// master::Shop: rows of the master table #38.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Shop : Row {
    static constexpr uint32_t TABLE = 38;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfac1c4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfac1ec;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe099eb;                  // "492decb28cd8eac4d7f567ff771c5acb62f7c369"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t SHOP_ID = 0;                  // int32
    static constexpr uint32_t PRODUCT_ID = 4;               // char[129]
    static constexpr uint32_t SORT_ID = 136;                // int32
    static constexpr uint32_t TYPE = 140;                   // int32
    static constexpr uint32_t OS = 144;                     // int32
    static constexpr uint32_t PRICE = 148;                  // int32
    static constexpr uint32_t NUMBER = 152;                 // int32
    static constexpr uint32_t BONUS = 156;                  // int32
    static constexpr uint32_t EXPLANATION = 160;            // char[129]
    static constexpr uint32_t STAGE_SKIP_TICKET = 292;      // int32
    static constexpr uint32_t APPLE_STATUS = 296;           // int32
    static constexpr uint32_t BENEFIT_ID = 300;             // int32
    static constexpr uint32_t BENEFIT_BONUS_VALUE = 304;    // int32
    static constexpr uint32_t BENEFIT_RESET_WEEK = 308;     // char[129]
    static constexpr uint32_t BENEFIT_RESET_TIME = 437;     // Time (4 bytes)
    static constexpr uint32_t BENEFIT_EXPLANATION = 441;    // char[129]
    static constexpr uint32_t USE_BENEFIT_EXPLANATION = 570; // char[129]
    static constexpr uint32_t THUMB_ID = 700;               // int32
    static constexpr uint32_t STATUS = 704;                 // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfac218;          // _ZTVN6master4ShopE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
