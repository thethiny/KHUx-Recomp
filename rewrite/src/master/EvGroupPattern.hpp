// master::EvGroupPattern: rows of the master table #17.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct EvGroupPattern : Row {
    static constexpr uint32_t TABLE = 17;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab444;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab46c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08c80;                  // "7d14a28305742503fa9def457979963d5a45c444"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfab498;                // _ZTVN6master14EvGroupPatternE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x782B79;       // shared_ptr<const EvGroupPattern>(EvGroupPattern*)
    static constexpr uint32_t SIZE = 0x18;

    // data positions (JSON keys)
    static constexpr uint32_t PATTERN_ID = 0;       // int32
    static constexpr uint32_t GROUP_DIVIDE = 4;     // int32
    static constexpr uint32_t START_TIME = 8;       // Time (4 bytes)
    static constexpr uint32_t END_TIME = 12;        // Time (4 bytes)
    static constexpr uint32_t SHIFT_TIME = 16;      // Time (4 bytes)
    static constexpr uint32_t ROTATE = 20;          // int32

    // shared_ptr<const EvGroupPattern> from one JSON row (null when a field is missing, mistyped or a bad time)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
