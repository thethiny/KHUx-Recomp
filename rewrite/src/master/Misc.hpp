// master::Misc: rows of the master table #27.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Misc : Row {
    static constexpr uint32_t TABLE = 27;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab8c4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab8ec;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe090cd;                  // "a257704e4a7e81ffa5233a697bf8133e0b78c5b8"

    static constexpr uint32_t PARSE_VTABLE = 0xfab918;          // _ZTVN6master4MiscE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t MISC_ID = 0;                  // int32
    static constexpr uint32_t VALUE = 4;                    // int32

    // parse(json): std::shared_ptr<const Misc> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
