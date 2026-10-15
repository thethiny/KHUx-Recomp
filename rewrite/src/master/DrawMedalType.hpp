// master::DrawMedalType: rows of the master table #11.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct DrawMedalType : Row {
    static constexpr uint32_t TABLE = 11;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab0e4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab10c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe087b0;                  // "387e8f3dcfd28f2aaad78e6f5832f700b1c8a427"

    static constexpr uint32_t PARSE_VTABLE = 0xfab138;          // _ZTVN6master13DrawMedalTypeE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t DRAW_MEDAL_TYPE_ID = 0;       // int32
    static constexpr uint32_t DRAW_TYPE = 4;                // int32
    static constexpr uint32_t SORT_ID = 8;                  // int32
    static constexpr uint32_t NAME = 12;                    // char[129]
    static constexpr uint32_t DESCRIPTION = 141;            // char[257]
    static constexpr uint32_t PAY_TYPE = 400;               // int32
    static constexpr uint32_t PRICE = 404;                  // int32
    static constexpr uint32_t TABLE_ID = 408;               // int32
    static constexpr uint32_t VALID_LOT = 412;              // int32
    static constexpr uint32_t LOT_COUNT = 416;              // int32[3]
    static constexpr uint32_t LOT_PSR = 428;                // int32[3]
    static constexpr uint32_t LOT_SR = 440;                 // int32[3]
    static constexpr uint32_t LOT_PR = 452;                 // int32[3]
    static constexpr uint32_t LOT_R = 464;                  // int32[3]
    static constexpr uint32_t LOT_PN = 476;                 // int32[3]
    static constexpr uint32_t LOT_N = 488;                  // int32[3]
    static constexpr uint32_t TABLE_IDS = 500;              // int32[3]
    static constexpr uint32_t VALID_ADD = 512;              // int32
    static constexpr uint32_t ADD_TYPE = 516;               // int32
    static constexpr uint32_t ADD_ID = 520;                 // int32
    static constexpr uint32_t ADD_SKILL_TYPE = 524;         // int32
    static constexpr uint32_t ADD_SKILL_ID = 528;           // int32
    static constexpr uint32_t ADD_SKILL_LV = 532;           // int32
    static constexpr uint32_t ADD_NUM = 536;                // int32
    static constexpr uint32_t BANNER_URL = 540;             // char[513]
    static constexpr uint32_t MYPAGE_BANNER = 1056;         // int32
    static constexpr uint32_t DAY_COUNT = 1060;             // int32
    static constexpr uint32_t WARRANT_NUMBER_OF_TIME = 1064;// int32
    static constexpr uint32_t WARRANT_MEDAL_LIST_IDS = 1068;// int32[1]
    static constexpr uint32_t WARRANT_MEDAL_REPEAT = 1072;  // int32
    static constexpr uint32_t WARRANT_RESET_MEDAL_LIST_IDS = 1076;// int32[10]
    static constexpr uint32_t START_DATE = 1116;            // Date (8 bytes)
    static constexpr uint32_t END_DATE = 1124;              // Date (8 bytes)
    static constexpr uint32_t IS_DELETED = 1132;            // int32

    // parse(json): std::shared_ptr<const DrawMedalType> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
