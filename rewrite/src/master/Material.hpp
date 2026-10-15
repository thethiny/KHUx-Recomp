// master::Material: rows of the master table #24.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Material : Row {
    static constexpr uint32_t TABLE = 24;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab714;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab73c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08efb;                  // "569d08d3bf1c2001ece49654969dbdf334d374bc"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfab768;                // _ZTVN6master8MaterialE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x7856BD;       // shared_ptr<const Material>(Material*)
    static constexpr uint32_t SIZE = 0xd4;

    // data positions (JSON keys)
    static constexpr uint32_t MATERIAL_ID = 0;      // int32
    static constexpr uint32_t CATEGORY = 4;         // int32
    static constexpr uint32_t SORT_ID = 8;          // int32
    static constexpr uint32_t NAME = 12;            // char[65]
    static constexpr uint32_t DESCRIPTION = 77;     // char[129]
    static constexpr uint32_t SELL = 208;           // int32

    // shared_ptr<const Material> from one JSON row (null when a field is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
