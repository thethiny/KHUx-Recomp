// master::ColosseumStage: rows of the master table #9.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct ColosseumStage : Row {
    static constexpr uint32_t TABLE = 9;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaafc4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaafec;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08653;                  // "b0f77442a67258c2104df05e07818e3c54de61ac"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfab018;                // _ZTVN6master14ColosseumStageE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x77D54D;       // shared_ptr<const ColosseumStage>(ColosseumStage*)
    static constexpr uint32_t SIZE = 0x5c;

    // data positions (JSON keys)
    static constexpr uint32_t COLOSSEUM_STAGE_ID = 0;   // int32
    static constexpr uint32_t OPEN_DATE = 4;            // Date (8 bytes)
    static constexpr uint32_t CLOSE_DATE = 12;          // Date (8 bytes)
    static constexpr uint32_t RANKING_CLOSE_DATE = 20;  // Date (8 bytes)
    static constexpr uint32_t HEAD_COLOSSEUM_ID = 28;   // int32
    static constexpr uint32_t TERM_COLOSSEUM_ID = 32;   // int32
    static constexpr uint32_t RANKING_ID = 36;          // int32
    static constexpr uint32_t VALID_STAGE = 40;         // int32
    static constexpr uint32_t STAGE_ID = 44;            // int32[10]
    static constexpr uint32_t LOOP_STAGE_COUNT = 84;    // int32
    static constexpr uint32_t LV_PER_LOOP = 88;         // int32

    // shared_ptr<const ColosseumStage> from one JSON row (null when a field is missing, mistyped or a bad date)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
