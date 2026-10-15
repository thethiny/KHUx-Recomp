// master::EvStage: rows of the master table #19.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct EvStage : Row {
    static constexpr uint32_t TABLE = 19;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab4d4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab4fc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08d4a;                  // "44a3fc65a4a66636e3e762f094e06af98822c153"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t EVENT_STAGE_ID = 0;           // int32
    static constexpr uint32_t VALID_STAGE_ID = 4;           // int32
    static constexpr uint32_t STAGE_ID = 8;                 // int32[5]
    static constexpr uint32_t VALID_STAGE_WIDTH_ID = 28;    // int32
    static constexpr uint32_t STAGE_START_ID = 32;          // int32
    static constexpr uint32_t STAGE_END_ID = 36;            // int32
    static constexpr uint32_t EVENT_CATEGORY = 40;          // int32
    static constexpr uint32_t EVENT_TYPE = 44;              // int32
    static constexpr uint32_t BANNER_URL = 48;              // char[513]
    static constexpr uint32_t MYPAGE_BANNER = 564;          // int32
    static constexpr uint32_t START_TIME = 568;             // Date (8 bytes)
    static constexpr uint32_t END_TIME = 576;               // Date (8 bytes)
    static constexpr uint32_t BOARD_END_TIME = 584;         // Date (8 bytes)
    static constexpr uint32_t WEEK_MONDAY = 592;            // int32
    static constexpr uint32_t WEEK_TUESDAY = 596;           // int32
    static constexpr uint32_t WEEK_WEDNESDAY = 600;         // int32
    static constexpr uint32_t WEEK_THURSDAY = 604;          // int32
    static constexpr uint32_t WEEK_FRIDAY = 608;            // int32
    static constexpr uint32_t WEEK_SATURDAY = 612;          // int32
    static constexpr uint32_t WEEK_SUNDAY = 616;            // int32
    static constexpr uint32_t BENEFIT_ID = 620;             // int32
    static constexpr uint32_t BENEFIT_MONDAY = 624;         // int32
    static constexpr uint32_t BENEFIT_TUESDAY = 628;        // int32
    static constexpr uint32_t BENEFIT_WEDNESDAY = 632;      // int32
    static constexpr uint32_t BENEFIT_THURSDAY = 636;       // int32
    static constexpr uint32_t BENEFIT_FRIDAY = 640;         // int32
    static constexpr uint32_t BENEFIT_SATURDAY = 644;       // int32
    static constexpr uint32_t BENEFIT_SUNDAY = 648;         // int32
    static constexpr uint32_t GROUP_PATTERN = 652;          // int32
    static constexpr uint32_t VALID_MINUTE = 656;           // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfab528;          // _ZTVN6master7EvStageE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
