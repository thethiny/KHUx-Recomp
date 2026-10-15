// master::Medal: rows of the master table #25.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Medal : Row {
    static constexpr uint32_t TABLE = 25;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab7a4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab7cc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09068;                  // "0c99210f96d0bbd44d4befd7ce0a3ad886fd496e"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t MEDAL_ID = 0;                 // int32
    static constexpr uint32_t IMAGE_ID = 4;                 // int32
    static constexpr uint32_t THUMB_ID = 8;                 // int32
    static constexpr uint32_t SORT_ID = 12;                 // int32
    static constexpr uint32_t NAME = 16;                    // char[65]
    static constexpr uint32_t FLAVOR = 81;                  // char[513]
    static constexpr uint32_t ADVANTAGE = 594;              // char[513]
    static constexpr uint32_t TYPE = 1108;                  // int32
    static constexpr uint32_t ATTRIBUTE = 1112;             // int32
    static constexpr uint32_t DARKLIGHT = 1116;             // int32
    static constexpr uint32_t VALID_SOURCE = 1120;          // int32
    static constexpr uint32_t SOURCE = 1124;                // int32[3]
    static constexpr uint32_t RARE = 1136;                  // int32
    static constexpr uint32_t SKILL_SLOT = 1140;            // int32
    static constexpr uint32_t GUILT_VALUE = 1144;           // int32
    static constexpr uint32_t MAX_LV = 1148;                // int32
    static constexpr uint32_t EXP_TYPE = 1152;              // int32
    static constexpr uint32_t COST = 1156;                  // int32
    static constexpr uint32_t MIN_COST = 1160;              // int32
    static constexpr uint32_t GROWTH_TYPE = 1164;           // int32
    static constexpr uint32_t ATTACK = 1168;                // int32
    static constexpr uint32_t MAX_ATTACK = 1172;            // int32
    static constexpr uint32_t ADD_MAX_ATTACK = 1176;        // int32
    static constexpr uint32_t DEFENSE = 1180;               // int32
    static constexpr uint32_t MAX_DEFENSE = 1184;           // int32
    static constexpr uint32_t ADD_MAX_DEFENSE = 1188;       // int32
    static constexpr uint32_t VALID_BURST = 1192;           // int32
    static constexpr uint32_t BURST_ID = 1196;              // int32
    static constexpr uint32_t BURST_ENHANCE_CATEGORY = 1200; // int32[2]
    static constexpr uint32_t GROUP_ID = 1208;              // int32
    static constexpr uint32_t SELL_SPHERE_POINT = 1212;     // int32
    static constexpr uint32_t MATERIAL_EXP = 1216;          // int32
    static constexpr uint32_t SELL = 1220;                  // int32
    static constexpr uint32_t SELL_PER_LV = 1224;           // int32
    static constexpr uint32_t ENHANCE_ATTACK = 1228;        // int32
    static constexpr uint32_t ENHANCE_DEFENSE = 1232;       // int32
    static constexpr uint32_t ENHANCE_COST = 1236;          // int32
    static constexpr uint32_t VALID_EVOLVE = 1240;          // int32
    static constexpr uint32_t EVOLVE_ID = 1244;             // int32
    static constexpr uint32_t EVOLVE_MONEY = 1248;          // int32
    static constexpr uint32_t VALID_EVOLVE_NEED = 1252;     // int32
    static constexpr uint32_t EVOLVE_NEED_ID = 1256;        // int32[5]

    static constexpr uint32_t PARSE_VTABLE = 0xfab7f8;          // _ZTVN6master5MedalE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
