// master::EvResource: rows of the master table #18.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct EvResource : Row {
    static constexpr uint32_t TABLE = 18;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfac494;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfac4bc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09b41;                  // "e9f5af4bcf736c24b1719bc37c4d8889e4e31fd9"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfac4e8;                // _ZTVN6master10EvResourceE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x7930F1;       // shared_ptr<const EvResource>(EvResource*)
    static constexpr uint32_t SIZE = 0x14;

    // data positions (JSON keys)
    static constexpr uint32_t RESOURCE_ID = 0;      // int32
    static constexpr uint32_t PRIORITY = 4;         // int32
    static constexpr uint32_t VALID_RESOURCE = 8;   // int32
    static constexpr uint32_t END_TIME = 12;        // Date (8 bytes)

    // shared_ptr<const EvResource> from one JSON row (null when a field is missing, mistyped or a bad date)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
