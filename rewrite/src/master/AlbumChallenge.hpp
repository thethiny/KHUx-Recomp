// master::AlbumChallenge: rows of the master table #0.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct AlbumChallenge : Row {
    static constexpr uint32_t TABLE = 0;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaaab4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaaadc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe07d94;                  // "f03a62eb16a617fa2d26a97cd585bba76bd2fd70"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0x134
    static constexpr uint32_t CHALLENGE_ID = 0x0;           // int32
    static constexpr uint32_t SORT_ID = 0x4;                // int32
    static constexpr uint32_t VALID = 0x8;                  // int32
    static constexpr uint32_t SUBMISSION_NAME = 0xc;        // char[257]
    static constexpr uint32_t TYPE = 0x110;                 // int32
    static constexpr uint32_t ATTRIBUTE = 0x114;            // int32
    static constexpr uint32_t TARGET_NUMBER = 0x118;        // int32
    static constexpr uint32_t REWARD_TYPE = 0x11c;          // int32
    static constexpr uint32_t REWARD_ID = 0x120;            // int32
    static constexpr uint32_t ASSIGN_SKILL_TYPE = 0x124;    // int32
    static constexpr uint32_t ASSIGN_SKILL_ID = 0x128;      // int32
    static constexpr uint32_t ASSIGN_SKILL_LV = 0x12c;      // int32
    static constexpr uint32_t NUM = 0x130;                  // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfaab08;      // _ZTVN6master14AlbumChallengeE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
