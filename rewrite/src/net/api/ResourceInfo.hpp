// hole::network::ResourceInfo: the resource list of the download step (an array of {"data": [...], "index": [...]}
// entries, numbered from baseId). The two file lists are parsed by helpers outside hole::network.
#pragma once

#include "Common.hpp"

namespace hole::network {

struct ResourceInfo {
    // One entry: its id and its two file lists (vectors of 28-byte file records).
    struct Entry {
        rw::Field<int32_t> id;                // +0
        api::StdVector data;                  // +4
        api::StdVector index;                 // +16
    };

    const uint8_t head[8];                          // +0
    rw::Field<int32_t> baseId;                // +8  id of the first entry
    rw::Field<int32_t> lastId;                // +12 id of the last entry (set by init)
    api::StdVector entries;                   // +16 std::vector<Entry>

    // json: the entries array. Appends every entry (std::vector push_back, reallocation inlined); 1 on success,
    // 0 on a malformed entry (the entries parsed so far stay).
    static uint32_t init(ResourceInfo *self, const api::JsonValue *json);
    // push every data file record of every entry (pointers) onto the manager's work deque (+0x30)
    static uint32_t acceptForDataFile(const ResourceInfo *self, uint32_t manager);
    // the same with the index file records of the last entry
    static uint32_t acceptForIndexFile(const ResourceInfo *self, uint32_t manager);
};
RW_SIZE(ResourceInfo::Entry, 28);
RW_LAYOUT(ResourceInfo, entries, 16);

}  // namespace hole::network
