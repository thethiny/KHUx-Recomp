// master::Title: rows of the master table #46.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Title : Row {
    static constexpr uint32_t TABLE = 46;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfac0a4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfac0cc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09901;                  // "fabde2026cb6850c217285f78c36179062b48895"

    static constexpr uint32_t PARSE_VTABLE = 0xfac0f8;          // _ZTVN6master5TitleE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t TITLE_ID = 0;                 // int32
    static constexpr uint32_t CATEGORY = 4;                 // int32
    static constexpr uint32_t TITLE = 8;                    // char[65]
    static constexpr uint32_t DESCRIPTION = 73;             // char[257]
    static constexpr uint32_t REQUIRE = 332;                // int32
    static constexpr uint32_t VALUE = 336;                  // int32
    static constexpr uint32_t LABEL = 340;                  // int32

    // parse(json): std::shared_ptr<const Title> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
