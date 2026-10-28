// std::__hash_table (unordered_map / unordered_set) as the NDK's libc++ compiles it, for the instances over the game's
// key and value types. Layout (32-bit): bucket array +0, bucket count +4, first node (the before-begin node's next)
// +8, size +0xC, max_load_factor (float) +0x10. A node: next +0, hash +4, value +8 (key first).
// The algorithms below are libc++'s (find, __node_insert_unique with its rehash, rehash / __rehash, remove, erase,
// clear, the destructor), parameterised by the key's hash and equality and the value's destructor. Float steps
// (load factor) are IEEE single precision as the soft-float helpers compute them; ceilf is the libm import.
#pragma once

#include <cmath>

#include "libcxx/Libcxx.hpp"

namespace libcxx::hash {

using rw::load;
using rw::save;

constexpr uint32_t BUCKETS = 0, BUCKET_COUNT = 4, FIRST = 8, SIZE = 0xC, MAX_LOAD = 0x10;
constexpr uint32_t NEXT_PRIME = 0xC609F9;   // std::__next_prime(size_t)
constexpr uint32_t CEILF = 0x40EFBC;        // ceilf (import)
constexpr uint32_t MEMCMP = 0x40EF2C;       // memcmp (import)
constexpr uint32_t STRCMP = 0x410234;       // strcmp (import)

inline float bitsToFloat(uint32_t u) { return rw::bitsf(u); }
// __fixunssfsi: truncate; negative and NaN give 0, too big saturates
inline uint32_t floatToUint(float f) {
    if (!(f > 0.0f)) return 0;
    if (f >= 4294967296.0f) return 0xFFFFFFFFu;
    return uint32_t(f);
}
inline float ceilfImport(float f) { return rw::bitsf(rw::call(CEILF, {rw::fbits(f)})); }

// bucket of a hash: a mask for power-of-two counts, else the remainder
inline uint32_t constrain(uint32_t h, uint32_t bc) { return (bc & (bc - 1)) ? h % bc : h & (bc - 1); }
inline bool isPower2(uint32_t bc) { return bc > 2 && !(bc & (bc - 1)); }
// __next_pow2 as compiled (no n < 2 test): 1 << (32 - clz(n - 1)), an ARM register shift (>= 32 gives 0)
inline uint32_t nextPow2(uint32_t n) {
    uint32_t v = n - 1, clz = 0;
    for (uint32_t bit = 0x80000000u; bit && !(v & bit); bit >>= 1) clz++;
    const uint32_t s = 32 - clz;
    return s >= 32 ? 0 : 1u << s;
}

// ------------------------------------------------------------------------------------------ hashes
// std::__murmur2_or_cityhash<unsigned, 32>::operator()(const void*, size_t): MurmurHash2
inline uint32_t murmur2(uint32_t data, uint32_t len) {
    constexpr uint32_t m = 0x5BD1E995;
    uint32_t h = len;
    for (; len >= 4; data += 4, len -= 4) {
        uint32_t k = load<uint8_t>(data) | (load<uint8_t>(data + 1) << 8) | (load<uint8_t>(data + 2) << 16) |
                     (uint32_t(load<uint8_t>(data + 3)) << 24);
        k *= m;
        k ^= k >> 24;
        k *= m;
        h *= m;
        h ^= k;
    }
    switch (len) {
    case 3: h ^= uint32_t(load<uint8_t>(data + 2)) << 16; [[fallthrough]];
    case 2: h ^= uint32_t(load<uint8_t>(data + 1)) << 8; [[fallthrough]];
    case 1: h ^= load<uint8_t>(data); h *= m;
    }
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return h;
}
// std::hash<std::string>: murmur2 over the characters
inline uint32_t stringHash(uint32_t s) {
    const bool isLong = load<uint8_t>(s) & 1;
    const uint32_t size = isLong ? load<uint32_t>(s + 4) : uint32_t(load<uint8_t>(s) >> 1);
    const uint32_t data = isLong ? load<uint32_t>(s + 8) : s + 1;
    return murmur2(data, size);
}
// operator==(const string&, const string&): sizes, then memcmp (long) or a byte loop (short)
inline bool stringEqual(uint32_t a, uint32_t b) {
    const bool aLong = load<uint8_t>(a) & 1, bLong = load<uint8_t>(b) & 1;
    const uint32_t size = aLong ? load<uint32_t>(a + 4) : uint32_t(load<uint8_t>(a) >> 1);
    const uint32_t bSize = bLong ? load<uint32_t>(b + 4) : uint32_t(load<uint8_t>(b) >> 1);
    if (size != bSize) return false;
    const uint32_t ad = aLong ? load<uint32_t>(a + 8) : a + 1, bd = bLong ? load<uint32_t>(b + 8) : b + 1;
    if (aLong) return rw::call(MEMCMP, {ad, bd, size}) == 0;
    for (uint32_t k = 0; k < size; k++)
        if (load<uint8_t>(ad + k) != load<uint8_t>(bd + k)) return false;
    return true;
}

// ------------------------------------------------------------------------------------------ key kinds
// hash(key address) and equal(key in a node, other key) - the argument order of the library's key_eq
struct WordKey {   // int, unsigned, enums: hash<T> is the value
    static uint32_t hash(uint32_t k) { return load<uint32_t>(k); }
    static bool equal(uint32_t a, uint32_t b) { return load<uint32_t>(a) == load<uint32_t>(b); }
};
struct ByteKey {   // a one-byte enum (UserLink::PlatformType, hashed by its own Hasher as the value)
    static uint32_t hash(uint32_t k) { return load<uint8_t>(k); }
    static bool equal(uint32_t a, uint32_t b) { return load<uint8_t>(a) == load<uint8_t>(b); }
};
struct U64Key {    // unsigned long long: murmur2 over the 8 bytes
    static uint32_t hash(uint32_t k) { return murmur2(k, 8); }
    static bool equal(uint32_t a, uint32_t b) {
        return load<uint32_t>(a) == load<uint32_t>(b) && load<uint32_t>(a + 4) == load<uint32_t>(b + 4);
    }
};
struct PtrKey {     // pointers (hash<T*>): murmur2 over the 4 bytes
    static uint32_t hash(uint32_t k) { return murmur2(k, 4); }
    static bool equal(uint32_t a, uint32_t b) { return load<uint32_t>(a) == load<uint32_t>(b); }
};
struct StringKey {
    static uint32_t hash(uint32_t k) { return stringHash(k); }
    static bool equal(uint32_t a, uint32_t b) { return stringEqual(a, b); }
};
struct CStrKey {   // const char* with the game's CharHash (djb2: 5381, * 33 + c) and strcmp equality
    static uint32_t hash(uint32_t k) {
        uint32_t h = 5381;
        for (uint32_t p = load<uint32_t>(k); uint8_t c = load<uint8_t>(p); p++) h = h * 33 + c;
        return h;
    }
    static bool equal(uint32_t a, uint32_t b) { return rw::call(STRCMP, {load<uint32_t>(a), load<uint32_t>(b)}) == 0; }
};

// ------------------------------------------------------------------------------------------ algorithms
// Key: a key kind above. DestroyValue(node): the node's value destructor (members, last first), not the node.
template <class Key> struct Table {
    // find(key): the node, or 0
    static uint32_t find(uint32_t t, uint32_t key) {
        const uint32_t h = Key::hash(key);
        const uint32_t bc = load<uint32_t>(t + BUCKET_COUNT);
        if (bc) {
            const uint32_t chash = constrain(h, bc);
            uint32_t nd = load<uint32_t>(load<uint32_t>(t + BUCKETS) + chash * 4);
            if (nd) {
                for (nd = load<uint32_t>(nd); nd && constrain(load<uint32_t>(nd + 4), bc) == chash; nd = load<uint32_t>(nd))
                    if (Key::equal(nd + 8, key)) return nd;
            }
        }
        return 0;
    }

    // __rehash(nbc): a new bucket array, the chain regrouped (equal keys stay together)
    static void rehashTo(uint32_t t, uint32_t nbc) {
        const uint32_t buckets = nbc ? guest::newObject(nbc * 4) : 0;
        const uint32_t old = load<uint32_t>(t + BUCKETS);
        save<uint32_t>(t + BUCKETS, buckets);
        if (old) guest::deleteObject(old);
        save<uint32_t>(t + BUCKET_COUNT, nbc);
        if (!nbc) return;
        for (uint32_t i = 0; i < nbc; i++) save<uint32_t>(load<uint32_t>(t + BUCKETS) + i * 4, 0);
        uint32_t pp = t + FIRST, cp = load<uint32_t>(pp);
        if (!cp) return;
        uint32_t chash = constrain(load<uint32_t>(cp + 4), nbc);
        save<uint32_t>(load<uint32_t>(t + BUCKETS) + chash * 4, pp);
        uint32_t phash = chash;
        for (pp = cp, cp = load<uint32_t>(cp); cp; cp = load<uint32_t>(pp)) {
            chash = constrain(load<uint32_t>(cp + 4), nbc);
            if (chash == phash) {
                pp = cp;
                continue;
            }
            const uint32_t slot = load<uint32_t>(t + BUCKETS) + chash * 4;
            if (!load<uint32_t>(slot)) {
                save<uint32_t>(slot, pp);
                pp = cp;
                phash = chash;
            } else {
                uint32_t np = cp;
                while (load<uint32_t>(np) && Key::equal(cp + 8, load<uint32_t>(np) + 8)) np = load<uint32_t>(np);
                save<uint32_t>(pp, load<uint32_t>(np));
                save<uint32_t>(np, load<uint32_t>(load<uint32_t>(slot)));
                save<uint32_t>(load<uint32_t>(slot), cp);
            }
        }
    }

    // rehash(n)
    static void rehash(uint32_t t, uint32_t n) {
        if (n == 1) n = 2;
        else if (n & (n - 1)) n = rw::call(NEXT_PRIME, {n});
        const uint32_t bc = load<uint32_t>(t + BUCKET_COUNT);
        if (n > bc) {
            rehashTo(t, n);
        } else if (n < bc) {
            const bool pow2 = isPower2(bc);
            const float need = ceilfImport(float(load<uint32_t>(t + SIZE)) / bitsToFloat(load<uint32_t>(t + MAX_LOAD)));
            const uint32_t m = floatToUint(need);
            const uint32_t fit = pow2 ? nextPow2(m) : rw::call(NEXT_PRIME, {m});
            if (fit > n) n = fit;
            if (n < bc) rehashTo(t, n);
        }
    }

    // __node_insert_unique(nd) -> pair<iterator, bool> in `result`
    static void nodeInsertUnique(uint32_t result, uint32_t t, uint32_t nd) {
        save<uint32_t>(nd + 4, Key::hash(nd + 8));
        uint32_t bc = load<uint32_t>(t + BUCKET_COUNT);
        uint32_t chash = 0;
        if (bc) {
            chash = constrain(load<uint32_t>(nd + 4), bc);
            uint32_t p = load<uint32_t>(load<uint32_t>(t + BUCKETS) + chash * 4);
            if (p) {
                for (p = load<uint32_t>(p); p && constrain(load<uint32_t>(p + 4), bc) == chash; p = load<uint32_t>(p)) {
                    if (Key::equal(p + 8, nd + 8)) {
                        save<uint32_t>(result, p);
                        save<uint8_t>(result + 4, 0);
                        return;
                    }
                }
            }
        }
        if (growFor(t, bc)) {
            bc = load<uint32_t>(t + BUCKET_COUNT);
            chash = constrain(load<uint32_t>(nd + 4), bc);
        }
        link(t, nd, chash, bc);
        save<uint32_t>(t + SIZE, load<uint32_t>(t + SIZE) + 1);
        save<uint32_t>(result, nd);
        save<uint8_t>(result + 4, 1);
    }

    // the growth check of an insert: rehash when the table would pass its load factor (or has no buckets)
    static bool growFor(uint32_t t, uint32_t bc) {
        const float mlf = bitsToFloat(load<uint32_t>(t + MAX_LOAD));
        const float limit = float(bc) * mlf;
        const float size1 = float(load<uint32_t>(t + SIZE) + 1);
        if (bc != 0 && !(size1 > limit)) return false;
        const uint32_t grow = 2 * bc + (isPower2(bc) ? 0 : 1);
        const uint32_t need = floatToUint(ceilfImport(size1 / mlf));
        rehash(t, grow < need ? need : grow);
        return true;
    }

    // link a node into its bucket (after the bucket's predecessor, or at the front of the chain)
    static void link(uint32_t t, uint32_t nd, uint32_t chash, uint32_t bc) {
        const uint32_t slot = load<uint32_t>(t + BUCKETS) + chash * 4;
        const uint32_t pn = load<uint32_t>(slot);
        if (!pn) {
            save<uint32_t>(nd, load<uint32_t>(t + FIRST));
            save<uint32_t>(t + FIRST, nd);
            save<uint32_t>(load<uint32_t>(t + BUCKETS) + chash * 4, t + FIRST);
            if (const uint32_t next = load<uint32_t>(nd))
                save<uint32_t>(load<uint32_t>(t + BUCKETS) + constrain(load<uint32_t>(next + 4), bc) * 4, nd);
        } else {
            save<uint32_t>(nd, load<uint32_t>(pn));
            save<uint32_t>(pn, nd);
        }
    }

    // __insert_unique(const value&) of a set: search first, build the node only when the key is new
    // (makeNode(hash) -> the node, its next / hash / value set)
    template <class MakeNode> static void insertUniqueSet(uint32_t result, uint32_t t, uint32_t key, MakeNode makeNode) {
        const uint32_t h = Key::hash(key);
        uint32_t bc = load<uint32_t>(t + BUCKET_COUNT);
        uint32_t chash = 0;
        if (bc) {
            chash = constrain(h, bc);
            uint32_t p = load<uint32_t>(load<uint32_t>(t + BUCKETS) + chash * 4);
            if (p) {
                for (p = load<uint32_t>(p); p && constrain(load<uint32_t>(p + 4), bc) == chash; p = load<uint32_t>(p)) {
                    if (Key::equal(p + 8, key)) {
                        save<uint32_t>(result, p);
                        save<uint8_t>(result + 4, 0);
                        return;
                    }
                }
            }
        }
        const uint32_t nd = makeNode(h);
        if (growFor(t, bc)) {
            bc = load<uint32_t>(t + BUCKET_COUNT);
            chash = constrain(h, bc);
        }
        link(t, nd, chash, bc);
        save<uint32_t>(t + SIZE, load<uint32_t>(t + SIZE) + 1);
        save<uint32_t>(result, nd);
        save<uint8_t>(result + 4, 1);
    }

    // __node_insert_multi(cp): after the last node with an equal key in its bucket, else at the bucket's front
    static uint32_t nodeInsertMulti(uint32_t t, uint32_t cp) {
        save<uint32_t>(cp + 4, Key::hash(cp + 8));
        uint32_t bc = load<uint32_t>(t + BUCKET_COUNT);
        if (growFor(t, bc)) bc = load<uint32_t>(t + BUCKET_COUNT);
        const uint32_t chash = constrain(load<uint32_t>(cp + 4), bc);
        const uint32_t slot = load<uint32_t>(t + BUCKETS) + chash * 4;
        uint32_t pn = load<uint32_t>(slot);
        if (!pn) {
            link(t, cp, chash, bc);
        } else {
            bool found = false;
            for (; load<uint32_t>(pn) && constrain(load<uint32_t>(load<uint32_t>(pn) + 4), bc) == chash; pn = load<uint32_t>(pn)) {
                const uint32_t next = load<uint32_t>(pn);
                const bool same = load<uint32_t>(next + 4) == load<uint32_t>(cp + 4) && Key::equal(next + 8, cp + 8);
                if (found != same) {
                    if (!found) found = true;
                    else break;
                }
            }
            save<uint32_t>(cp, load<uint32_t>(pn));
            save<uint32_t>(pn, cp);
            if (const uint32_t next = load<uint32_t>(cp)) {
                const uint32_t nhash = constrain(load<uint32_t>(next + 4), bc);
                if (nhash != chash) save<uint32_t>(load<uint32_t>(t + BUCKETS) + nhash * 4, cp);
            }
        }
        save<uint32_t>(t + SIZE, load<uint32_t>(t + SIZE) + 1);
        return cp;
    }

    // remove(it) -> the node, unlinked, in a __node_holder {node, &allocator, value constructed} at `holder`
    static void remove(uint32_t holder, uint32_t t, uint32_t cn) {
        const uint32_t bc = load<uint32_t>(t + BUCKET_COUNT);
        const uint32_t chash = constrain(load<uint32_t>(cn + 4), bc);
        const uint32_t slot = load<uint32_t>(t + BUCKETS) + chash * 4;
        uint32_t pn = load<uint32_t>(slot);
        while (load<uint32_t>(pn) != cn) pn = load<uint32_t>(pn);
        if (pn == t + FIRST || constrain(load<uint32_t>(pn + 4), bc) != chash) {
            const uint32_t next = load<uint32_t>(cn);
            if (!next || constrain(load<uint32_t>(next + 4), bc) != chash) save<uint32_t>(slot, 0);
        }
        if (const uint32_t next = load<uint32_t>(cn)) {
            const uint32_t nhash = constrain(load<uint32_t>(next + 4), bc);
            if (nhash != chash) save<uint32_t>(load<uint32_t>(t + BUCKETS) + nhash * 4, pn);
        }
        save<uint32_t>(pn, load<uint32_t>(cn));
        save<uint32_t>(cn, 0);
        save<uint32_t>(t + SIZE, load<uint32_t>(t + SIZE) - 1);
        save<uint32_t>(holder, cn);
        save<uint32_t>(holder + 4, t + 8);
        save<uint32_t>(holder + 8, 1);
    }
};

// __deallocate(node chain): each node's value destroyed, the node freed
template <class DestroyValue> inline void deallocate(uint32_t np, DestroyValue destroyValue) {
    while (np) {
        const uint32_t next = load<uint32_t>(np);
        destroyValue(np);
        guest::deleteObject(np);
        np = next;
    }
}

// ~__hash_table: the nodes, then the bucket array (unique_ptr reset: null stored first). Returns this.
template <class DestroyValue> inline uint32_t destroy(uint32_t t, DestroyValue destroyValue) {
    deallocate(load<uint32_t>(t + FIRST), destroyValue);
    const uint32_t buckets = load<uint32_t>(t + BUCKETS);
    save<uint32_t>(t + BUCKETS, 0);
    if (buckets) guest::deleteObject(buckets);
    return t;
}

// clear()
template <class DestroyValue> inline void clear(uint32_t t, DestroyValue destroyValue) {
    if (!load<uint32_t>(t + SIZE)) return;
    deallocate(load<uint32_t>(t + FIRST), destroyValue);
    save<uint32_t>(t + FIRST, 0);
    const uint32_t bc = load<uint32_t>(t + BUCKET_COUNT);
    for (uint32_t i = 0; i < bc; i++) save<uint32_t>(load<uint32_t>(t + BUCKETS) + i * 4, 0);
    save<uint32_t>(t + SIZE, 0);
}

// a __node_holder going out of scope (unique_ptr<node, __hash_node_destructor>): {node, &alloc, constructed}
template <class DestroyValue> inline void dropHolder(uint32_t holder, DestroyValue destroyValue) {
    const uint32_t nd = load<uint32_t>(holder);
    save<uint32_t>(holder, 0);
    if (!nd) return;
    if (load<uint8_t>(holder + 8)) destroyValue(nd);
    guest::deleteObject(nd);
}

inline void noValue(uint32_t) {}

}  // namespace libcxx::hash
