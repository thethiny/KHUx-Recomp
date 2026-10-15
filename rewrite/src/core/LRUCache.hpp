// LRUCache<K, V>: a size-bounded cache. Values (shared_ptrs) live in an unordered_map<K, V>; a std::list<K>
// keeps the keys by recency (front = most recently used, back = least). Every value has a cost given by a
// std::function<unsigned(const V&)>; inserting adds the cost to `used` and evicts from the back while
// used > capacity. Without a cost function the cache is a plain map (no recency list, no eviction).
//
// Known instances (all V are shared_ptrs):
//   K = unsigned long long: shared_ptr<AvatarIcon>, shared_ptr<const SafeBuffer> (MasterDataManager + 2192,
//       capacity 0x40000)
//   K = unsigned: shared_ptr<LWF::Data>, shared_ptr<unordered_map<string, cocos2d::Value>>,
//       shared_ptr<cocos2d::Texture2D>
//   K = std::string: shared_ptr<cocos2d::LWFTextRendererContext>
#pragma once

#include "rw/rw.hpp"
#include "Guest.hpp"

// The cache object (60 bytes, the same for every instance).
struct LRUCache {
    // std::unordered_map<K, V>
    rw::Field<uint32_t> buckets;        // +0
    rw::Field<uint32_t> bucketCount;    // +4
    rw::Field<uint32_t> firstNode;      // +8
    rw::Field<uint32_t> mapSize;        // +12
    rw::Field<float> maxLoadFactor;     // +16
    // std::list<K>: the end node (prev = back = least recently used, next = front) and the size
    rw::Field<uint32_t> listBack;       // +20
    rw::Field<uint32_t> listFront;      // +24
    rw::Field<uint32_t> listSize;       // +28
    rw::Field<uint32_t> capacity;       // +32
    rw::Field<uint32_t> used;           // +36 sum of the costs of the cached values
    const uint8_t costFn[16];                 // +40 std::function<unsigned(const V&)> buffer
    rw::Field<uint32_t> costFnCallable; // +56 its __f_ (0 = no cost function)

    static constexpr uint32_t LIST_END = 20;   // offset of the list's end node

    // A recency-list node: prev, next, then the key (8-aligned for 64-bit keys).
    struct ListNode {
        rw::Field<uint32_t> prev;   // +0
        rw::Field<uint32_t> next;   // +4
        // +8 key
    };
};
RW_LAYOUT(LRUCache, listBack, 20);
RW_LAYOUT(LRUCache, capacity, 32);
RW_LAYOUT(LRUCache, used, 36);
RW_LAYOUT(LRUCache, costFnCallable, 56);
RW_SIZE(LRUCache, 60);

// Map hash node: next +0, hash +4, key +8, value (shared_ptr) after the key:
//   unsigned key: value at +12; unsigned long long key: +16; std::string key: +20.
