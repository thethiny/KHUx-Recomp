// master::Stage: rows of the master table #44.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Stage : Row {
    static constexpr uint32_t TABLE = 44;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfac014;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfac03c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe098ca;                  // "9e94fb09a91bedbfc0838503ad7b3d2e7938113e"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0x3fc
    static constexpr uint32_t STAGE_ID = 0x0;               // int32
    static constexpr uint32_t ID = 0x4;                     // int32
    static constexpr uint32_t NAME = 0x8;                   // char[129]
    static constexpr uint32_t MAP_NAME = 0x89;              // char[129]
    static constexpr uint32_t USE_AP = 0x10c;               // int32
    static constexpr uint32_t CHAPTER_ID = 0x110;           // int32
    static constexpr uint32_t WORLD_ID = 0x114;             // int32
    static constexpr uint32_t THUMB_ID = 0x118;             // int32
    static constexpr uint32_t STAGE_KIND = 0x11c;           // int32
    static constexpr uint32_t SHOW_ICON = 0x120;            // int32
    static constexpr uint32_t VALID_BEFORE_DRAMA = 0x124;   // int32
    static constexpr uint32_t BEFORE_DRAMA_ID = 0x128;      // int32[5]
    static constexpr uint32_t BEFORE_DRAMA_TYPE = 0x13c;    // int32[5]
    static constexpr uint32_t VALID_AFTER_DRAMA = 0x150;    // int32
    static constexpr uint32_t AFTER_DRAMA_ID = 0x154;       // int32[5]
    static constexpr uint32_t AFTER_DRAMA_TYPE = 0x168;     // int32[5]
    static constexpr uint32_t VALID_CLEAR_GET_TITLE = 0x17c;// int32
    static constexpr uint32_t CLEAR_GET_TITLE = 0x180;      // int32
    static constexpr uint32_t VALID_CLEAR_GET_SPHERE = 0x184;// int32
    static constexpr uint32_t CLEAR_GET_SPHERE = 0x188;     // int32
    static constexpr uint32_t RESET_CLEAR_GET = 0x18c;      // int32
    static constexpr uint32_t VALID_CLEAR_GET_ITEM = 0x190; // int32
    static constexpr uint32_t CLEAR_GET_ITEM_TYPE = 0x194;  // int32[3]
    static constexpr uint32_t CLEAR_GET_ITEM_ID = 0x1a0;    // int32[3]
    static constexpr uint32_t CLEAR_GET_ASSIGN_SKILL_TYPE = 0x1ac;// int32[3]
    static constexpr uint32_t CLEAR_GET_ASSIGN_SKILL_ID = 0x1b8;// int32[3]
    static constexpr uint32_t CLEAR_GET_ASSIGN_SKILL_LV = 0x1c4;// int32[3]
    static constexpr uint32_t CLEAR_GET_ITEM_NUM = 0x1d0;   // int32[3]
    static constexpr uint32_t RESET_SUB_MISSION = 0x1dc;    // int32
    static constexpr uint32_t VALID_SUBMISSION = 0x1e0;     // int32
    static constexpr uint32_t SUBMISSION_REQUIRE = 0x1e4;   // int32[3]
    static constexpr uint32_t SUBMISSION_NAME = 0x1f0;      // char[3][129]
    static constexpr uint32_t SUBMISSION_DATA_TYPE = 0x374; // int32[3]
    static constexpr uint32_t SUBMISSION_ID_TYPE = 0x380;   // int32[3]
    static constexpr uint32_t SUBMISSION_ID = 0x38c;        // int32[3]
    static constexpr uint32_t SUBMISSION_NUM = 0x398;       // int32[3]
    static constexpr uint32_t SUBMISSION_REWARD_TYPE = 0x3a4;// int32[3]
    static constexpr uint32_t SUBMISSION_ITEM_ID = 0x3b0;   // int32[3]
    static constexpr uint32_t SUBMISSION_SKILL_TYPE = 0x3bc;// int32[3]
    static constexpr uint32_t SUBMISSION_SKILL_ID = 0x3c8;  // int32[3]
    static constexpr uint32_t SUBMISSION_SKILL_LV = 0x3d4;  // int32[3]
    static constexpr uint32_t SUBMISSION_ITEM_NUM = 0x3e0;  // int32[3]
    static constexpr uint32_t RAID_BOSS = 0x3ec;            // int32
    static constexpr uint32_t BGM_FIELD = 0x3f0;            // int32
    static constexpr uint32_t BGM_BATTLE = 0x3f4;           // int32
    static constexpr uint32_t BGM_BOSS = 0x3f8;             // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfac068;      // _ZTVN6master5StageE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
