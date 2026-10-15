// master::BattleMisc: rows of the master table #4.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct BattleMisc : Row {
    static constexpr uint32_t TABLE = 4;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaacf4;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaad1c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe07ed8;                  // "da9d1a6bbd1427c1ef3908f3ebadf0fdc7763895"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfaad48;                // _ZTVN6master10BattleMiscE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x779D69;       // shared_ptr<const BattleMisc>(BattleMisc*)
    static constexpr uint32_t SIZE = 8;

    // data positions (JSON keys)
    static constexpr uint32_t BATTLE_MISC_ID = 0;   // int32
    static constexpr uint32_t VALUE = 4;            // int32

    // shared_ptr<const BattleMisc> from one JSON row (null when a field is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
