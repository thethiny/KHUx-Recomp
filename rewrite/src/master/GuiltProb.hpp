// master::GuiltProb: rows of the master table #20.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct GuiltProb : Row {
    static constexpr uint32_t TABLE = 20;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfac374;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfac39c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09abe;                  // "a69f85954ec8a39e90b7f39dee70997f55fa4cee"

    static constexpr uint32_t PARSE_VTABLE = 0xfac3c8;          // _ZTVN6master9GuiltProbE + 8 (the object parse() builds)

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t ID = 0;                       // int32
    static constexpr uint32_t VALUE = 4;                    // int32
    static constexpr uint32_t FACTOR = 8;                   // int32
    static constexpr uint32_t WEIGHT = 12;                  // int32
    static constexpr uint32_t IS_CAMPAIGN = 16;             // int32

    // parse(json): std::shared_ptr<const GuiltProb> from one JSON row (empty when a member is missing / mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
