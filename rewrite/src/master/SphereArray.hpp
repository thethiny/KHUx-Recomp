// master::SphereArray: rows of the master table #41.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct SphereArray : Row {
    static constexpr uint32_t TABLE = 41;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfabef4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfabf1c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe095eb;                  // "92dcd05ff1c15798bb79ff3f3f14216798882563"

    static constexpr uint32_t PARSE_VTABLE = 0xfabf48;          // _ZTVN6master11SphereArrayE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t ARRY_ID = 0;                  // int32
    static constexpr uint32_t SP_COL4 = 4;                  // int32[9]
    static constexpr uint32_t SP_COL3 = 40;                 // int32[9]
    static constexpr uint32_t SP_COL2 = 76;                 // int32[9]
    static constexpr uint32_t SP_COL1 = 112;                // int32[9]
    static constexpr uint32_t SP_COL0 = 148;                // int32[9]

    // parse(json): std::shared_ptr<const SphereArray> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
