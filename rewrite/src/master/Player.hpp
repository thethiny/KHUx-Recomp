// master::Player: rows of the master table #29.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Player : Row {
    static constexpr uint32_t TABLE = 29;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab954;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab97c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09107;                  // "d6845aec95bfd372912c11a09f23519352183165"

    static constexpr uint32_t PARSE_VTABLE = 0xfab9a8;          // _ZTVN6master6PlayerE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t LV = 0;                       // int32
    static constexpr uint32_t NEED_EXP = 4;                 // int32
    static constexpr uint32_t LUX_MEDAL = 8;                // int32
    static constexpr uint32_t AP = 12;                      // int32
    static constexpr uint32_t COST = 16;                    // int32
    static constexpr uint32_t HP = 20;                      // int32
    static constexpr uint32_t REWARD_ID = 24;               // int32

    // parse(json): std::shared_ptr<const Player> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
