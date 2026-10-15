// master::RaidSetting: rows of the master table #33.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct RaidSetting : Row {
    static constexpr uint32_t TABLE = 33;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfabb94;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfabbbc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09366;                  // "8a97473a1405c15d79082a958c797f2e896e2b3f"

    static constexpr uint32_t PARSE_VTABLE = 0xfabbe8;          // _ZTVN6master11RaidSettingE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t RAID_SETTING_ID = 0;          // int32
    static constexpr uint32_t BOSS_ID = 4;                  // int32
    static constexpr uint32_t BOSS_UP_TIME = 8;             // int32
    static constexpr uint32_t USE_AP = 12;                  // int32
    static constexpr uint32_t RARE_BOSS_ID = 16;            // int32
    static constexpr uint32_t RARE_BOSS_UP_TIME = 20;       // int32
    static constexpr uint32_t RARE_USE_AP = 24;             // int32
    static constexpr uint32_t RARE_UP_TYPE = 28;            // int32
    static constexpr uint32_t RARE_UP_TYPE_ARG = 32;        // int32
    static constexpr uint32_t RARE_UP_TRIGGER_BOSS_ID = 36; // int32
    static constexpr uint32_t VALID_UNLOCK_RARE = 40;       // int32
    static constexpr uint32_t UNLOCK_RARE_BOSS_ID = 44;     // int32[3]
    static constexpr uint32_t UNLOCK_RARE_NUM = 56;         // int32[3]

    // parse(json): std::shared_ptr<const RaidSetting> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
