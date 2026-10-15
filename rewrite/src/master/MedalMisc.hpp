// master::MedalMisc: rows of the master table #26.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct MedalMisc : Row {
    static constexpr uint32_t TABLE = 26;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab834;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab85c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe0909d;                  // "cd575ec8add3ddb7a6e1553e374002ecff1dd668"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t MEDAL_MISC_ID = 0;            // int32
    static constexpr uint32_t VALUE = 4;                    // int32[3]

    static constexpr uint32_t PARSE_VTABLE = 0xfab888;          // _ZTVN6master9MedalMiscE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
