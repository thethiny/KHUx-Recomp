// master::InitItem: rows of the master table #21.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct InitItem : Row {
    static constexpr uint32_t TABLE = 21;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab564;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab58c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08d92;                  // "28883147a7aa7a86184b3850b02d5ecad4827509"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t ID = 0;                       // int32
    static constexpr uint32_t CATEGORY = 4;                 // int32
    static constexpr uint32_t EQUIP_TYPE = 8;               // int32
    static constexpr uint32_t EQUIP_NO = 12;                // int32
    static constexpr uint32_t PARAM = 16;                   // int32
    static constexpr uint32_t ITEM_ID = 20;                 // int32
    static constexpr uint32_t SKILL_ID = 24;                // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfab5b8;          // _ZTVN6master8InitItemE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
