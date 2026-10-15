// master::TutorialMisc: rows of the master table #47.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct TutorialMisc : Row {
    static constexpr uint32_t TABLE = 47;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaaa24;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaaa4c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe07d0b;                  // "6c42247a0ed61e03b79346245126639a7f2c260f"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfaaa78;                // _ZTVN6master12TutorialMiscE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x777FE1;       // shared_ptr<const TutorialMisc>(TutorialMisc*)
    static constexpr uint32_t SIZE = 0x20;

    // data positions (JSON keys)
    static constexpr uint32_t TUTORIAL_ID = 0;          // int32
    static constexpr uint32_t START_CLEAR_QUEST = 4;    // int32
    static constexpr uint32_t START_MISC_ID = 8;        // int32
    static constexpr uint32_t NUM1 = 12;                // int32
    static constexpr uint32_t NUM2 = 16;                // int32
    static constexpr uint32_t SET_CLEAR_QUEST = 20;     // int32
    static constexpr uint32_t SET_MISC_ID = 24;         // int32
    static constexpr uint32_t VALID = 28;               // int32

    // shared_ptr<const TutorialMisc> from one JSON row (null when a field is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
