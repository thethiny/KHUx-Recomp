// master::SerialcodeReward: rows of the master table #37.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct SerialcodeReward : Row {
    static constexpr uint32_t TABLE = 37;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfac134;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfac15c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe0993d;                  // "4fbfb5a403e8c28f231b42e55f05f521bba5c057"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfac188;                // _ZTVN6master16SerialcodeRewardE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x79079D;       // shared_ptr<const SerialcodeReward>(SerialcodeReward*)
    static constexpr uint32_t SIZE = 0x1b0;

    // data positions (JSON keys)
    static constexpr uint32_t SERIALCODE_REWARD_ID = 0; // int32
    static constexpr uint32_t NAME = 4;                 // char[257]
    static constexpr uint32_t START_DATE = 262;         // Date (8 bytes)
    static constexpr uint32_t END_DATE = 270;           // Date (8 bytes)
    static constexpr uint32_t ITEM_ID = 280;            // int32
    static constexpr uint32_t VALID_REWARD = 284;       // int32
    static constexpr uint32_t TYPE = 288;               // int32[6]
    static constexpr uint32_t ID = 312;                 // int32[6]
    static constexpr uint32_t SKILL_TYPE = 336;         // int32[6]
    static constexpr uint32_t SKILL_ID = 360;           // int32[6]
    static constexpr uint32_t SKILL_LV = 384;           // int32[6]
    static constexpr uint32_t NUM = 408;                // int32[6]

    // shared_ptr<const SerialcodeReward> from one JSON row (null when a field is missing, mistyped or a bad date)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
