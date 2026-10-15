// master::EvCampaign: rows of the master table #16.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct EvCampaign : Row {
    static constexpr uint32_t TABLE = 16;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab3b4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab3dc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08c30;                  // "16c788257487c4932ff038a5fee12192b57b1d35"

    static constexpr uint32_t PARSE_VTABLE = 0xfab408;          // _ZTVN6master10EvCampaignE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t CAMPAIGN_ID = 0;              // int32
    static constexpr uint32_t TYPE = 4;                     // int32
    static constexpr uint32_t POWER = 8;                    // int32
    static constexpr uint32_t DISPLAY_TEXT = 12;            // char[257]
    static constexpr uint32_t STAGE_TYPE = 272;             // int32
    static constexpr uint32_t VALID_STAGE_ID = 276;         // int32
    static constexpr uint32_t STAGE_ID = 280;               // int32[5]
    static constexpr uint32_t START_TIME = 300;             // Date (8 bytes)
    static constexpr uint32_t END_TIME = 308;               // Date (8 bytes)
    static constexpr uint32_t WEEK_MONDAY = 316;            // int32
    static constexpr uint32_t WEEK_TUESDAY = 320;           // int32
    static constexpr uint32_t WEEK_WEDNESDAY = 324;         // int32
    static constexpr uint32_t WEEK_THURSDAY = 328;          // int32
    static constexpr uint32_t WEEK_FRIDAY = 332;            // int32
    static constexpr uint32_t WEEK_SATURDAY = 336;          // int32
    static constexpr uint32_t WEEK_SUNDAY = 340;            // int32
    static constexpr uint32_t GROUP_PATTERN = 344;          // int32
    static constexpr uint32_t VALID_MINUTE = 348;           // int32

    // parse(json): std::shared_ptr<const EvCampaign> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
