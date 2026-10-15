// master::Keyblade: rows of the master table #22.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Keyblade : Row {
    static constexpr uint32_t TABLE = 22;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab5f4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab61c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08e58;                  // "080738c392bb3d670f3c6c01b0fb2fb6682c78f0"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfab648;                // _ZTVN6master8KeybladeE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x7848C5;       // shared_ptr<const Keyblade>(Keyblade*)
    static constexpr uint32_t SIZE = 0x164;

    // data positions (JSON keys)
    static constexpr uint32_t KEYBLADE_ID = 0;              // int32
    static constexpr uint32_t NAME = 4;                     // char[65]
    static constexpr uint32_t DESCRIPTION = 69;             // char[129]
    static constexpr uint32_t LV = 200;                     // int32
    static constexpr uint32_t DISPLAY_ID = 204;             // int32
    static constexpr uint32_t CATEGORY = 208;               // int32
    static constexpr uint32_t PASSIVE = 212;                // int32
    static constexpr uint32_t PASSIVE_POWER = 216;          // int32
    static constexpr uint32_t DRAW_COUNT = 220;             // int32
    static constexpr uint32_t MAX_BURST_GAUGE = 224;        // int32
    static constexpr uint32_t VALID_CONSIST = 228;          // int32
    static constexpr uint32_t ATTRIBUTE = 232;              // int32[5]
    static constexpr uint32_t DARKLIGHT = 252;              // int32[5]
    static constexpr uint32_t ACCORD = 272;                 // int32[5]
    static constexpr uint32_t DLACCORD = 292;               // int32[5]
    static constexpr uint32_t PARTNER_RATE = 312;           // int32
    static constexpr uint32_t EVOLVE_ID = 316;              // int32
    static constexpr uint32_t VALID_NEED_MATERIAL = 320;    // int32
    static constexpr uint32_t NEED_MATERIAL_ID = 324;       // int32[4]
    static constexpr uint32_t NEED_MATERIAL_NUM = 340;      // int32[4]

    // shared_ptr<const Keyblade> from one JSON row (null when a field is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
