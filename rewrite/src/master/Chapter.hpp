// master::Chapter: rows of the master table #7.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Chapter : Row {
    static constexpr uint32_t TABLE = 7;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaaea4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaaecc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08492;                  // "53c45ed4fbbf7c85bf6650918629c9a69e9629d2"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t CHAPTER_ID = 0;               // int32
    static constexpr uint32_t NAME = 4;                     // char[257]

    static constexpr uint32_t PARSE_VTABLE = 0xfaaef8;          // _ZTVN6master7ChapterE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
