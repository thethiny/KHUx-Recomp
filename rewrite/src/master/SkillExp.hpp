// master::SkillExp: rows of the master table #39.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct SkillExp : Row {
    static constexpr uint32_t TABLE = 39;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfabdd4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfabdfc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09530;                  // "51d9b43c5835235d1f93c66eda1c75cf89daca2d"

    static constexpr uint32_t PARSE_VTABLE = 0xfabe28;          // _ZTVN6master8SkillExpE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t EXP_ID = 0;                   // int32
    static constexpr uint32_t VALUE = 4;                    // int32[3]

    // parse(json): std::shared_ptr<const SkillExp> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
