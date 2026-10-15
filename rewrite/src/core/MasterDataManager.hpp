// MasterDataManager: the master data (49 tables). Each table is one archive file on disk (m/<n>.jpg): a key/value
// archive of rows (key = row ID as text, value = a serialized SafeBuffer) plus a "hash" entry (the table's schema
// hash) and its revision. TableFile indexes a file (row ID -> stream offset); rows are decoded on demand by the
// table classes (master::X::create) and kept in an LRU cache keyed by (table << 32 | id).
// Updating: the server's master list (update) queues the tables whose revision is newer (a deque of TableInfo),
// which are downloaded one by one (startNextDownload / parseResponse, a worker thread calls save).
#pragma once

#include "Guest.hpp"

struct MasterDataManager {
    static constexpr uint32_t TABLE_COUNT = 49;

    // One table file.
    struct TableFile {
        guest::String name;              // +0  the archive's first key (the table name)
        rw::Field<int32_t> revision;     // +12 the file's revision (first data entry, 4 bytes)
        guest::SharedPtr stream;         // +16 shared_ptr<bg::IInputStream> of the open file
        // +24 std::unordered_map<int, long>: row ID -> stream offset of the row's entry
        rw::Field<uint32_t> offsetBuckets;     // +24
        rw::Field<uint32_t> offsetBucketCount; // +28
        rw::Field<uint32_t> offsetFirst;       // +32 first node {next, hash?, key (+8), value (+12)}
        rw::Field<uint32_t> offsetCount;       // +36
        rw::Field<float> offsetMaxLoad;        // +40

        static uint32_t init(TableFile *self, uint32_t table, uint32_t path);
        static void clear(TableFile *self);
        static void getIDs(uint32_t outSet, const TableFile *self);
        static void getData(guest::SharedPtr *out, TableFile *self, int32_t id);
    };

    // One entry of the server's master list (queued for download).
    struct TableInfo {
        rw::Field<uint32_t> table;       // +0
        rw::Field<int32_t> revision;     // +4
        guest::String url;               // +8
        guest::SharedPtr cipher;         // +20 shared_ptr<Cipher> from the 32-byte key
        guest::String md5;               // +28

        static uint32_t parse(TableInfo *self, uint32_t json);
    };

    rw::Field<int32_t> masterRevision;   // +0
    rw::Field<uint32_t> status;          // +4 0 ok, 1 network error, 2 save failed, 3 bad response
    rw::Field<uint32_t> downloadTotal;   // +8 tables queued by the last update
    TableFile files[TABLE_COUNT];        // +12
    // +2168 std::deque<TableInfo> (map: first, begin, end, cap; start; size), 102 entries per block
    rw::Field<uint32_t> queueMapFirst;   // +2168
    rw::Field<uint32_t> queueMapBegin;   // +2172
    rw::Field<uint32_t> queueMapEnd;     // +2176
    rw::Field<uint32_t> queueMapCap;     // +2180
    rw::Field<uint32_t> queueStart;      // +2184
    rw::Field<uint32_t> queueSize;       // +2188
    // +2192 LRUCache<unsigned long long, shared_ptr<const SafeBuffer>> (layout: core/LRUCache.hpp)
    const uint8_t cache[0x8D0 - 0x890];
    rw::Field<uint8_t> downloading;      // +2256
    const uint8_t pad_8D1[3];
    guest::SharedPtr connection;         // +2260 the running request

    // offsets inside the cache
    static constexpr uint32_t CACHE = 2192;
    static constexpr uint32_t CACHE_LIST = 2212;      // std::list<unsigned long long> (recency)
    static constexpr uint32_t CACHE_CAPACITY = 2224;
    static constexpr uint32_t CACHE_USED = 2228;
    static constexpr uint32_t CACHE_SIZE_FN = 2232;   // std::function<unsigned(const shared_ptr<const SafeBuffer>&)>

    static constexpr uint32_t INSTANCE = 0x1004BB8;   // the singleton (function-local static)
    static constexpr uint32_t INSTANCE_GUARD = 0x10054A0;
    // per-table descriptors, 0x38 bytes each: +0 name (const char*), +8 std::function parse(json) -> row,
    // +0x20 std::function<const char*()> getHash
    static constexpr uint32_t TABLE_DESCRIPTORS = 0x1004100;
    static constexpr uint32_t DESCRIPTOR_SIZE = 0x38;

    static MasterDataManager *getInstance();
    static MasterDataManager *destruct(MasterDataManager *self);
    static MasterDataManager *construct(MasterDataManager *self);
    static uint32_t remove(MasterDataManager *self);
    static void find(guest::SharedPtr *out, MasterDataManager *self, uint32_t table, int32_t id);
    static void insert(MasterDataManager *self, uint32_t table, int32_t id, const guest::SharedPtr *row);
    static void getIDs(uint32_t outSet, const MasterDataManager *self, uint32_t table);
    static void getData(guest::SharedPtr *out, MasterDataManager *self, uint32_t table, int32_t id);
    static uint32_t reset(MasterDataManager *self);
    static uint32_t update(MasterDataManager *self, uint32_t document);
    static uint32_t startNextDownload(MasterDataManager *self);
    static uint8_t isDownloading(const MasterDataManager *self);
    static void finishDownload(MasterDataManager *self);
    static uint32_t getProgress(const MasterDataManager *self, const uint32_t *total);
    static void parseResponse(MasterDataManager *self, const guest::SharedPtr *result);
    static uint32_t save(MasterDataManager *self, const TableInfo *info, uint32_t rows);
};
RW_LAYOUT(MasterDataManager::TableFile, revision, 12);
RW_LAYOUT(MasterDataManager::TableFile, stream, 16);
RW_LAYOUT(MasterDataManager::TableFile, offsetBuckets, 24);
RW_LAYOUT(MasterDataManager::TableFile, offsetMaxLoad, 40);
RW_SIZE(MasterDataManager::TableFile, 44);
RW_LAYOUT(MasterDataManager::TableInfo, url, 8);
RW_LAYOUT(MasterDataManager::TableInfo, cipher, 20);
RW_LAYOUT(MasterDataManager::TableInfo, md5, 28);
RW_SIZE(MasterDataManager::TableInfo, 40);
RW_LAYOUT(MasterDataManager, files, 12);
RW_LAYOUT(MasterDataManager, queueMapFirst, 2168);
RW_LAYOUT(MasterDataManager, queueSize, 2188);
RW_LAYOUT(MasterDataManager, cache, 2192);
RW_LAYOUT(MasterDataManager, downloading, 2256);
RW_LAYOUT(MasterDataManager, connection, 2260);
RW_SIZE(MasterDataManager, 2268);
