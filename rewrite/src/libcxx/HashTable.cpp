// std::unordered_map / unordered_set instances over the game's (and the engine's) key and value types. The table
// algorithms are libcxx::hash::Table (HashTable.hpp); what differs per instance is written here: how a node is
// built (node size, the value's copy / move constructor, the key's hash), how a value is destroyed, and the
// operator[] / at / insert wrappers around them.
#include "libcxx/HashTable.hpp"

using namespace libcxx::hash;
using libcxx::copyString;
using libcxx::copyWords;
using libcxx::destroyShared;
using libcxx::destroyString;
using libcxx::moveShared;
using libcxx::moveTriple;
using rw::load;
using rw::save;

namespace {

constexpr uint32_t STRLEN = 0x40E08C;                 // strlen (import)
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;           // __aeabi_memset(dest, n, c) (import)
constexpr uint32_t STRING_ASSIGN = 0x498A1D;          // basic_string::operator=(const basic_string&) (sub_498A1C)
constexpr uint32_t PRODUCT_INFO_COPY = 0x4D9891;      // iap::ProductInfo::ProductInfo(const ProductInfo&)

// ------------------------------------------------------------------------------------------ value destructors
// (the value's members, last first; the key's own string last)
void none(uint32_t) {}
template <uint32_t Off> void sharedAt(uint32_t n) { destroyShared(n + Off); }
template <uint32_t Off> void stringAt(uint32_t n) { destroyString(n + Off); }
void stringKey(uint32_t n) { destroyString(n + 8); }
void stringKeyShared(uint32_t n) { destroyShared(n + 0x14); destroyString(n + 8); }
void stringKeyVec2(uint32_t n) { rw::call(rwlib::cocos2d::Vec2::dtor_base, {n + 0x14}); destroyString(n + 8); }
void stringKeyIntList(uint32_t n) { rw::call(0x6F5887, {n + 0x14}); destroyString(n + 8); }   // __list_imp<int>::clear
void productInfo(uint32_t n) {   // string key, iap::ProductInfo (three strings)
    destroyString(n + 0x2C);
    destroyString(n + 0x20);
    destroyString(n + 0x14);
    destroyString(n + 8);
}
void relativeData(uint32_t n) {  // string key, cocostudio::RelativeData (four vector<string>)
    for (uint32_t off : {0x38u, 0x2Cu, 0x20u, 0x14u}) rw::call(0x47363F, {n + off});   // __vector_base<string> dtor
    destroyString(n + 8);
}
void speechQueue(uint32_t n) { rw::call(0x601D95, {n + 0xC}); }   // __deque_base<string>::~__deque_base

template <void (*V)(uint32_t)> uint32_t destroyTable(uint32_t t) { return destroy(t, V); }
template <void (*V)(uint32_t)> void clearTable(uint32_t t) { clear(t, V); }
template <void (*V)(uint32_t)> void deallocateNodes(uint32_t, uint32_t np) { deallocate(np, V); }

// erase(it): the next node; the removed one destroyed through its holder
template <class Key, void (*V)(uint32_t)> uint32_t erase(uint32_t t, uint32_t it) {
    const uint32_t next = load<uint32_t>(it);
    guest::Frame holder(12);
    Table<Key>::remove(holder.addr(), t, it);
    dropHolder(holder.addr(), V);
    return next;
}

// ------------------------------------------------------------------------------------------ node construction
// A __node_holder {node, &allocator, value constructed}: the node allocated, the holder set with nothing built yet.
uint32_t newHeld(uint32_t holder, uint32_t t, uint32_t size) {
    const uint32_t n = guest::newObject(size);
    save<uint32_t>(holder, n);
    save<uint32_t>(holder + 4, t + 8);
    save<uint32_t>(holder + 8, 0);
    return n;
}
void constructed(uint32_t holder) { save<uint8_t>(holder + 8, 1); }
// a string key from a const char* (strlen + basic_string::__init)
void stringFromCStr(uint32_t s, uint32_t cstr) {
    const uint32_t len = rw::call(STRLEN, {cstr});
    rw::call(libcxx::STRING_INIT, {s, cstr, len});
}

// unordered_set<unsigned long long>: the key
void constructU64(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x10);
    const uint32_t lo = load<uint32_t>(x), hi = load<uint32_t>(x + 4);
    save<uint32_t>(n + 8, lo);
    save<uint32_t>(n + 0xC, hi);
    constructed(holder);
    const uint32_t h = U64Key::hash(n + 8);
    save<uint32_t>(n, 0);
    save<uint32_t>(n + 4, h);
}
// map<unsigned long long, int> from pair<u64, unsigned / int>
void constructU64Int(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x18);
    const uint32_t lo = load<uint32_t>(x), hi = load<uint32_t>(x + 4);
    save<uint32_t>(n + 0xC, hi);
    save<uint32_t>(n + 8, lo);
    save<uint32_t>(n + 0x10, load<uint32_t>(x + 8));
    constructed(holder);
    const uint32_t h = U64Key::hash(n + 8);
    save<uint32_t>(n, 0);
    save<uint32_t>(n + 4, h);
}
// map<unsigned long long, shared_ptr<T>> from pair<u64, shared_ptr<T>>&&
void constructU64Shared(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x18);
    const uint32_t lo = load<uint32_t>(x), hi = load<uint32_t>(x + 4);
    save<uint32_t>(n + 0xC, hi);
    save<uint32_t>(n + 8, lo);
    moveShared(n + 0x10, x + 8);
    constructed(holder);
    const uint32_t h = U64Key::hash(n + 8);
    save<uint32_t>(n, 0);
    save<uint32_t>(n + 4, h);
}
// map<unsigned, string> from pair<unsigned, const char*>
void constructUintString(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x18);
    save<uint32_t>(n + 8, load<uint32_t>(x));
    stringFromCStr(n + 0xC, load<uint32_t>(x + 4));
    constructed(holder);
    save<uint32_t>(n + 4, load<uint32_t>(n + 8));
    save<uint32_t>(n, 0);
}
// map<string, iap::ProductInfo> from const pair<const string, ProductInfo>&
void constructProductInfo(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x38);
    copyString(n + 8, x);
    rw::call(PRODUCT_INFO_COPY, {n + 0x14, x + 0xC});
    constructed(holder);
    save<uint32_t>(n + 4, StringKey::hash(n + 8));
    save<uint32_t>(n, 0);
}
// ... from pair<string, ProductInfo>&& (four strings moved)
void constructProductInfoMove(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x38);
    for (uint32_t k = 0; k < 0x30; k += 0xC) moveTriple(n + 8 + k, x + k);
    constructed(holder);
    const uint32_t h = StringKey::hash(n + 8);
    save<uint32_t>(n, 0);
    save<uint32_t>(n + 4, h);
}
// map<string, shared_ptr<bg::Data>> from pair<const char*, shared_ptr<bg::Data>>&&
void constructStringData(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x1C);
    stringFromCStr(n + 8, load<uint32_t>(x));
    moveShared(n + 0x14, x + 4);
    constructed(holder);
    save<uint32_t>(n + 4, StringKey::hash(n + 8));
    save<uint32_t>(n, 0);
}
// map<string, int> from pair<const char*, int>
void constructStringInt(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x18);
    stringFromCStr(n + 8, load<uint32_t>(x));
    save<uint32_t>(n + 0x14, load<uint32_t>(x + 4));
    constructed(holder);
    save<uint32_t>(n + 4, StringKey::hash(n + 8));
    save<uint32_t>(n, 0);
}
// map<string, shared_ptr<LWFTextRendererContext>> from pair<string, shared_ptr<...>>&&
void constructStringContext(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x1C);
    moveTriple(n + 8, x);
    moveShared(n + 0x14, x + 0xC);
    constructed(holder);
    const uint32_t h = StringKey::hash(n + 8);
    save<uint32_t>(n, 0);
    save<uint32_t>(n + 4, h);
}
// ... __construct_node_with_key(key): the key copied, the value empty, both marked constructed (no hash yet)
void constructContextWithKey(uint32_t holder, uint32_t t, uint32_t key) {
    const uint32_t n = guest::newObject(0x1C);
    copyString(n + 8, key);
    save<uint32_t>(n + 0x14, 0);
    save<uint32_t>(n + 0x18, 0);
    save<uint32_t>(holder, n);
    save<uint32_t>(holder + 8, 0x101);
    save<uint32_t>(holder + 4, t + 8);
}
// map<SpeechDisp*, deque<string>> from pair<SpeechDisp*, deque<string>>&&
void constructSpeechQueue(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x24);
    save<uint32_t>(n + 8, load<uint32_t>(x));
    copyWords(n + 0xC, x + 4, 0x10);              // the block map (a __split_buffer)
    for (uint32_t off : {0x10u, 0xCu, 8u, 4u}) save<uint32_t>(x + off, 0);
    save<uint32_t>(n + 0x1C, load<uint32_t>(x + 0x14));   // start, size
    save<uint32_t>(n + 0x20, load<uint32_t>(x + 0x18));
    save<uint32_t>(x + 0x14, 0);
    save<uint32_t>(x + 0x18, 0);
    constructed(holder);
    const uint32_t h = PtrKey::hash(n + 8);
    save<uint32_t>(n, 0);
    save<uint32_t>(n + 4, h);
}
// map<const char*, T> from pair<const char*, T> (T one word)
void constructCStrWord(uint32_t holder, uint32_t t, uint32_t x) {
    const uint32_t n = newHeld(holder, t, 0x10);
    save<uint32_t>(n + 8, load<uint32_t>(x));
    save<uint32_t>(n + 0xC, load<uint32_t>(x + 4));
    constructed(holder);
    save<uint32_t>(n + 4, CStrKey::hash(n + 8));
    save<uint32_t>(n, 0);
}
// unordered_set<string>: __construct_node(x, hash)
void constructStringSet(uint32_t holder, uint32_t t, uint32_t x, uint32_t h) {
    const uint32_t n = guest::newObject(0x14);
    copyString(n + 8, x);
    save<uint32_t>(n, 0);
    save<uint32_t>(n + 4, h);
    save<uint32_t>(holder, n);
    save<uint32_t>(holder + 4, t + 8);
    save<uint32_t>(holder + 8, 1);
}

// ------------------------------------------------------------------------------------------ inserts
// __insert_unique(value) through __construct_node: the holder keeps the node when an equal key was already there
template <class Key, void (*Construct)(uint32_t, uint32_t, uint32_t), void (*V)(uint32_t)>
void insertHeld(uint32_t result, uint32_t t, uint32_t x) {
    guest::Frame holder(12);
    Construct(holder.addr(), t, x);
    const uint32_t nd = load<uint32_t>(holder.addr());
    Table<Key>::nodeInsertUnique(result, t, nd);
    const bool inserted = load<uint8_t>(result + 4);
    *holder.at<uint32_t>(0) = 0;
    if (!inserted && nd) {
        if (load<uint8_t>(holder.addr() + 8)) V(nd);
        guest::deleteObject(nd);
    }
}

// __insert_unique(value) with the node built inline (Fill: value, hash, next)
template <class Key, uint32_t Size, void (*Fill)(uint32_t, uint32_t), void (*V)(uint32_t)>
void insertInline(uint32_t result, uint32_t t, uint32_t x) {
    const uint32_t nd = guest::newObject(Size);
    Fill(nd, x);
    Table<Key>::nodeInsertUnique(result, t, nd);
    if (nd && !load<uint8_t>(result + 4)) {
        V(nd);
        guest::deleteObject(nd);
    }
}
// pair<int, word>
void fillIntWord(uint32_t n, uint32_t x) {
    const uint32_t key = load<uint32_t>(x);
    save<uint32_t>(n + 8, key);
    save<uint32_t>(n + 0xC, load<uint32_t>(x + 4));
    save<uint32_t>(n + 4, key);
    save<uint32_t>(n, 0);
}
// pair<unsigned long long, word> into a map<int, int>: the low word is the key, the value at +8
void fillIntFromU64Pair(uint32_t n, uint32_t x) {
    const uint32_t key = load<uint32_t>(x);
    save<uint32_t>(n + 8, key);
    save<uint32_t>(n + 0xC, load<uint32_t>(x + 8));
    save<uint32_t>(n + 4, key);
    save<uint32_t>(n, 0);
}
// pair<audio::ResourceID, audio::Manager::Bank>&& (the Bank's shared_ptr moved)
void fillBank(uint32_t n, uint32_t x) {
    const uint32_t key = load<uint32_t>(x);
    save<uint32_t>(n + 8, key);
    const uint32_t b = load<uint32_t>(x + 4);
    moveShared(n + 0x10, x + 8);
    save<uint32_t>(n + 0xC, b);
    save<uint32_t>(n + 4, key);
    save<uint32_t>(n, 0);
}
// pair<UserLink::PlatformType, UserLink::Info>&& (Info: a byte and a string, moved)
void fillPlatformInfo(uint32_t n, uint32_t x) {
    const uint8_t key = load<uint8_t>(x);
    save<uint8_t>(n + 8, key);
    const uint8_t b = load<uint8_t>(x + 4);
    moveTriple(n + 0x10, x + 8);
    save<uint8_t>(n + 0xC, b);
    save<uint32_t>(n + 4, key);
    save<uint32_t>(n, 0);
}
// pair<unsigned, shared_ptr<T>>&&
void fillUintShared(uint32_t n, uint32_t x) {
    const uint32_t key = load<uint32_t>(x);
    save<uint32_t>(n + 8, key);
    moveShared(n + 0xC, x + 4);
    save<uint32_t>(n + 4, key);
    save<uint32_t>(n, 0);
}

// unordered_set<T>::__insert_unique(const T&) with the node built inline (one-word key, or u64)
void insertWordSetImpl(uint32_t result, uint32_t t, uint32_t x, bool ptr) {
    auto make = [&](uint32_t h) {
        const uint32_t n = guest::newObject(0xC);
        save<uint32_t>(n, 0);
        save<uint32_t>(n + 4, h);
        save<uint32_t>(n + 8, load<uint32_t>(x));
        return n;
    };
    if (ptr) Table<PtrKey>::insertUniqueSet(result, t, x, make);
    else Table<WordKey>::insertUniqueSet(result, t, x, make);
}
void insertIntSet(uint32_t result, uint32_t t, uint32_t x) { insertWordSetImpl(result, t, x, false); }
void insertPtrSet(uint32_t result, uint32_t t, uint32_t x) { insertWordSetImpl(result, t, x, true); }
void insertU64Set(uint32_t result, uint32_t t, uint32_t x) {
    Table<U64Key>::insertUniqueSet(result, t, x, [&](uint32_t h) {
        const uint32_t n = guest::newObject(0x10);
        const uint32_t lo = load<uint32_t>(x), hi = load<uint32_t>(x + 4);
        save<uint32_t>(n, 0);
        save<uint32_t>(n + 4, h);
        save<uint32_t>(n + 8, lo);
        save<uint32_t>(n + 0xC, hi);
        return n;
    });
}
void insertStringSet(uint32_t result, uint32_t t, uint32_t x) {
    guest::Frame holder(12);
    Table<StringKey>::insertUniqueSet(result, t, x, [&](uint32_t h) {
        constructStringSet(holder.addr(), t, x, h);
        return load<uint32_t>(holder.addr());
    });
    *holder.at<uint32_t>(0) = 0;   // released
}

// ------------------------------------------------------------------------------------------ operator[] / at
// operator[](key): the mapped value, a value-initialised node inserted first when the key is new
template <class Key, uint32_t Size, uint32_t MappedOffset, void (*Fill)(uint32_t, uint32_t)>
uint32_t subscript(uint32_t t, uint32_t key) {
    uint32_t nd = Table<Key>::find(t, key);
    if (!nd) {
        const uint32_t n = guest::newObject(Size);
        Fill(n, key);
        guest::Frame result(8);
        Table<Key>::nodeInsertUnique(result.addr(), t, n);
        nd = load<uint32_t>(result.addr());
    }
    return nd + MappedOffset;
}
void keyWordZero(uint32_t n, uint32_t key) {        // one-word key, one-word value 0
    save<uint32_t>(n + 8, load<uint32_t>(key));
    save<uint32_t>(n + 0xC, 0);
}
void keyWordSharedZero(uint32_t n, uint32_t key) {  // one-word key, empty shared_ptr
    save<uint32_t>(n + 8, load<uint32_t>(key));
    save<uint32_t>(n + 0xC, 0);
    save<uint32_t>(n + 0x10, 0);
}
void keyU64SharedZero(uint32_t n, uint32_t key) {   // u64 key, empty shared_ptr
    save<uint32_t>(n + 8, load<uint32_t>(key));
    save<uint32_t>(n + 0xC, load<uint32_t>(key + 4));
    save<uint32_t>(n + 0x10, 0);
    save<uint32_t>(n + 0x14, 0);
}
void keyPlatformInfoZero(uint32_t n, uint32_t key) {
    save<uint8_t>(n + 8, load<uint8_t>(key));
    for (uint32_t off : {0xCu, 0x10u, 0x14u, 0x18u}) save<uint32_t>(n + off, 0);
}
void keySpeechQueueZero(uint32_t n, uint32_t key) {
    save<uint32_t>(n + 8, load<uint32_t>(key));
    rw::call(AEABI_MEMSET, {n + 0xC, 0x18, 0});
}

// map<string, shared_ptr<LWFTextRendererContext>>::operator[](const string&): through __construct_node_with_key
uint32_t subscriptStringContext(uint32_t t, uint32_t key) {
    uint32_t nd = Table<StringKey>::find(t, key);
    if (!nd) {
        guest::Frame frame(0x14);
        const uint32_t holder = frame.addr(8), result = frame.addr(0);
        constructContextWithKey(holder, t, key);
        Table<StringKey>::nodeInsertUnique(result, t, load<uint32_t>(holder));
        *frame.at<uint32_t>(8) = 0;   // released
        nd = load<uint32_t>(result);
    }
    return nd + 0x14;
}

// at(key): the mapped value, or std::out_of_range
template <uint32_t Find> uint32_t at(uint32_t t, uint32_t key) {
    const uint32_t nd = rw::call(Find, {t, key});
    if (nd) return nd + 0xC;
    const uint32_t e = rw::call(0xC98131, {8});                  // __cxa_allocate_exception
    rw::call(0xC8E86D, {e, 0xDFF291});                           // logic_error(e, "unordered_map::at: key not found")
    save<uint32_t>(e, load<uint32_t>(0xFD6094) + 8);             // vtable for std::out_of_range
    rw::call(0xC983D5, {e, load<uint32_t>(0xFD6098), load<uint32_t>(0xFD609C)});   // __cxa_throw
    return 0;
}

// ------------------------------------------------------------------------------------------ the rest
// unordered_set<unsigned long long>(const unordered_set&)
uint32_t copyU64Set(uint32_t t, uint32_t u) {
    for (uint32_t off : {0xCu, 8u, 4u, 0u}) save<uint32_t>(t + off, 0);
    save<uint32_t>(t + MAX_LOAD, load<uint32_t>(u + MAX_LOAD));
    Table<U64Key>::rehash(t, load<uint32_t>(u + BUCKET_COUNT));
    guest::Frame result(8);
    for (uint32_t n = load<uint32_t>(u + FIRST); n; n = load<uint32_t>(n)) insertU64Set(result.addr(), t, n + 8);
    return t;
}

// __hash_table(__hash_table&&) (unordered_set<unsigned long long>)
uint32_t moveU64Table(uint32_t t, uint32_t u) {
    const uint32_t buckets = load<uint32_t>(u);
    save<uint32_t>(u, 0);
    const uint32_t bc = load<uint32_t>(u + 4);
    save<uint32_t>(u + 4, 0);
    save<uint32_t>(t, buckets);
    save<uint32_t>(t + 4, bc);
    save<uint32_t>(t + FIRST, load<uint32_t>(u + FIRST));
    const uint32_t size = load<uint32_t>(u + SIZE);
    save<uint32_t>(t + SIZE, size);
    save<uint32_t>(t + MAX_LOAD, load<uint32_t>(u + MAX_LOAD));
    if (size) {
        save<uint32_t>(buckets + constrain(load<uint32_t>(load<uint32_t>(t + FIRST) + 4), bc) * 4, t + FIRST);
        save<uint32_t>(u + FIRST, 0);
        save<uint32_t>(u + SIZE, 0);
    }
    return t;
}

// map<string, iap::ProductInfo>: __insert_multi(const value&), __assign_multi(first, last)
uint32_t insertMultiProductInfo(uint32_t t, uint32_t x) {
    guest::Frame holder(12);
    constructProductInfo(holder.addr(), t, x);
    const uint32_t nd = load<uint32_t>(holder.addr());
    return Table<StringKey>::nodeInsertMulti(t, nd);
}
void assignMultiProductInfo(uint32_t t, uint32_t first, uint32_t last) {
    const uint32_t bc = load<uint32_t>(t + BUCKET_COUNT);
    if (bc) {
        for (uint32_t i = 0; i < bc; i++) save<uint32_t>(load<uint32_t>(t + BUCKETS) + i * 4, 0);
        save<uint32_t>(t + SIZE, 0);
        uint32_t cache = load<uint32_t>(t + FIRST);
        save<uint32_t>(t + FIRST, 0);
        for (; cache && first != last; first = load<uint32_t>(first)) {
            for (uint32_t off : {8u, 0x14u, 0x20u, 0x2Cu}) rw::call(STRING_ASSIGN, {cache + off, first + off});
            const uint32_t next = load<uint32_t>(cache);
            Table<StringKey>::nodeInsertMulti(t, cache);
            cache = next;
        }
        deallocate(cache, productInfo);
    }
    for (; first != last; first = load<uint32_t>(first)) insertMultiProductInfo(t, first + 8);
}

// the hash functors themselves
uint32_t murmurFunctor(uint32_t, uint32_t data, uint32_t len) { return murmur2(data, len); }
uint32_t stringHashFunctor(uint32_t, uint32_t s) { return stringHash(s); }

}  // namespace

RW_FUNCTION(0x48286C, "std::__murmur2_or_cityhash<unsigned int, 32>::operator()", murmurFunctor);
RW_FUNCTION(0x4D9838, "std::hash<string>::operator()", stringHashFunctor);

// ---------------------------------------------------------------------------------------------- value-specific
RW_FUNCTION(0x4740A8, "std::unordered_set<unsigned long long>::unordered_set(const&)", copyU64Set);
RW_FUNCTION(0x6A5034, "std::unordered_set<unsigned long long>::__hash_table(&&)", moveU64Table);
RW_FUNCTION(0x482648, "std::unordered_set<unsigned long long>::__insert_unique(const&)", insertU64Set);
RW_FUNCTION(0x4D6E1C, "std::unordered_set<unsigned long long>::__insert_unique(&&)", (insertHeld<U64Key, constructU64, none>));
RW_FUNCTION(0x4D6EB0, "std::unordered_set<unsigned long long>::__construct_node", constructU64);
RW_FUNCTION(0x5C002E, "std::unordered_set<int>::__insert_unique", insertIntSet);
RW_FUNCTION(0x6F4F94, "std::unordered_set<SpeechDisp*>::__insert_unique", insertPtrSet);
RW_FUNCTION(0x6F42CC, "std::unordered_set<SpeechDisp*>::erase", (erase<PtrKey, none>));
RW_FUNCTION(0x6F62D6, "std::unordered_set<SpeechDisp*>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x4DA504, "std::unordered_set<string>::__insert_unique", insertStringSet);
RW_FUNCTION(0x4DA7C0, "std::unordered_set<string>::__construct_node", constructStringSet);

RW_FUNCTION(0x4942D2, "std::unordered_map<int, int>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x4B0238, "std::unordered_map<int, int>::__insert_unique<pair<u64, unsigned>>", (insertInline<WordKey, 0x10, fillIntFromU64Pair, none>));
RW_FUNCTION(0x4B0498, "std::unordered_map<int, int>::__insert_unique<pair<u64, int>>", (insertInline<WordKey, 0x10, fillIntFromU64Pair, none>));
RW_FUNCTION(0x4B18D0, "std::unordered_map<int, int>::__insert_unique<pair<int, int>>", (insertInline<WordKey, 0x10, fillIntWord, none>));
RW_FUNCTION(0x5F7114, "std::unordered_map<int, BattleMarker*>::__insert_unique", (insertInline<WordKey, 0x10, fillIntWord, none>));
RW_FUNCTION(0x5F7AA0, "std::unordered_map<int, BattleMarker*>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x75CF18, "std::unordered_map<int, MapFieldLayer*>::__insert_unique", (insertInline<WordKey, 0x10, fillIntWord, none>));
RW_FUNCTION(0x755428, "std::unordered_map<int, MapFieldLayer*>::at", at<0x758BC1>);
RW_FUNCTION(0x75CC08, "std::unordered_map<int, MapFieldLayer*>::clear", clearTable<none>);
RW_FUNCTION(0x60805C, "std::unordered_map<int, MapFieldLayer*>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x75D32C, "std::unordered_map<int, vector<MapJumpInfo*>*>::__insert_unique", (insertInline<WordKey, 0x10, fillIntWord, none>));
RW_FUNCTION(0x756DB4, "std::unordered_map<int, vector<MapJumpInfo*>*>::at", at<0x75848F>);
RW_FUNCTION(0x75D2EA, "std::unordered_map<int, vector<MapJumpInfo*>*>::clear", clearTable<none>);
RW_FUNCTION(0x60802E, "std::unordered_map<int, vector<MapJumpInfo*>*>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x76F6B8, "std::unordered_map<int, long>::__insert_unique", (insertInline<WordKey, 0x10, fillIntWord, none>));
RW_FUNCTION(0x76F200, "std::unordered_map<int, long>::clear", clearTable<none>);
RW_FUNCTION(0x772430, "std::unordered_map<int, long>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x5C1B30, "std::unordered_map<int, audio::ResourceID>::__insert_unique", (insertInline<WordKey, 0x10, fillIntWord, none>));
RW_FUNCTION(0x5C20C8, "std::unordered_map<int, audio::ResourceID>::erase", (erase<WordKey, none>));
RW_FUNCTION(0x5C2246, "std::unordered_map<int, audio::ResourceID>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x5C053A, "std::unordered_map<int, audio::Manager::Bank>::__insert_unique", (insertInline<WordKey, 0x18, fillBank, sharedAt<0x10>>));
RW_FUNCTION(0x5C03D0, "std::unordered_map<int, audio::Manager::Bank>::erase", (erase<WordKey, sharedAt<0x10>>));
RW_FUNCTION(0x5C0A70, "std::unordered_map<int, audio::Manager::Bank>::__deallocate", deallocateNodes<sharedAt<0x10>>);
RW_FUNCTION(0x8193C8, "std::unordered_map<int, plugin::WebViewImpl*>::operator[]", (subscript<WordKey, 0x10, 0xC, keyWordZero>));
RW_FUNCTION(0x843BC4, "std::unordered_map<int, experimental::ui::VideoPlayer*>::operator[]", (subscript<WordKey, 0x10, 0xC, keyWordZero>));

RW_FUNCTION(0x4B4EF8, "std::unordered_map<unsigned int, unsigned int>::operator[]", (subscript<WordKey, 0x10, 0xC, keyWordZero>));
RW_FUNCTION(0x4EE2A4, "std::unordered_map<unsigned int, unsigned int>::clear", clearTable<none>);
RW_FUNCTION(0x4B4EC8, "std::unordered_map<unsigned int, unsigned int>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x4DFC5C, "std::unordered_map<unsigned int, string>::__insert_unique", (insertHeld<WordKey, constructUintString, stringAt<0xC>>));
RW_FUNCTION(0x4DFD1C, "std::unordered_map<unsigned int, string>::__construct_node", constructUintString);
RW_FUNCTION(0x4DF714, "std::unordered_map<unsigned int, string>::erase", (erase<WordKey, stringAt<0xC>>));
RW_FUNCTION(0x7FA08C, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::__insert_unique", (insertInline<WordKey, 0x14, fillUintShared, sharedAt<0xC>>));
RW_FUNCTION(0x7FA45C, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::erase", (erase<WordKey, sharedAt<0xC>>));
RW_FUNCTION(0x7F1062, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::clear", clearTable<sharedAt<0xC>>);
RW_FUNCTION(0x7F109A, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::__deallocate", deallocateNodes<sharedAt<0xC>>);
RW_FUNCTION(0x7F9FA4, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::operator[]", (subscript<WordKey, 0x14, 0xC, keyWordSharedZero>));
RW_FUNCTION(0x918BF4, "std::unordered_map<unsigned int, shared_ptr<Texture2D>>::__insert_unique", (insertInline<WordKey, 0x14, fillUintShared, sharedAt<0xC>>));
RW_FUNCTION(0x918FC4, "std::unordered_map<unsigned int, shared_ptr<Texture2D>>::erase", (erase<WordKey, sharedAt<0xC>>));
RW_FUNCTION(0x7F9974, "std::unordered_map<unsigned int, shared_ptr<unordered_map<string, Value>>>::__insert_unique", (insertInline<WordKey, 0x14, fillUintShared, sharedAt<0xC>>));
RW_FUNCTION(0x7F96B4, "std::unordered_map<unsigned int, shared_ptr<unordered_map<string, Value>>>::erase", (erase<WordKey, sharedAt<0xC>>));
RW_FUNCTION(0x7F1002, "std::unordered_map<unsigned int, shared_ptr<unordered_map<string, Value>>>::clear", clearTable<sharedAt<0xC>>);
RW_FUNCTION(0x7F103A, "std::unordered_map<unsigned int, shared_ptr<unordered_map<string, Value>>>::__deallocate", deallocateNodes<sharedAt<0xC>>);

RW_FUNCTION(0x4B1070, "std::unordered_map<unsigned long long, int>::__insert_unique<pair<u64, unsigned>>", (insertHeld<U64Key, constructU64Int, none>));
RW_FUNCTION(0x4B176C, "std::unordered_map<unsigned long long, int>::__insert_unique<pair<u64, int>>", (insertHeld<U64Key, constructU64Int, none>));
RW_FUNCTION(0x4B1104, "std::unordered_map<unsigned long long, int>::__construct_node<pair<u64, unsigned>>", constructU64Int);
RW_FUNCTION(0x4B1800, "std::unordered_map<unsigned long long, int>::__construct_node<pair<u64, int>>", constructU64Int);
RW_FUNCTION(0x4B09C0, "std::unordered_map<unsigned long long, int>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x5CAE38, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::__insert_unique", (insertHeld<U64Key, constructU64Shared, sharedAt<0x10>>));
RW_FUNCTION(0x5CAEF0, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::__construct_node", constructU64Shared);
RW_FUNCTION(0x5CB368, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::erase", (erase<U64Key, sharedAt<0x10>>));
RW_FUNCTION(0x5CBD44, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::__deallocate", deallocateNodes<sharedAt<0x10>>);
RW_FUNCTION(0x5CACD0, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::operator[]", (subscript<U64Key, 0x18, 0x10, keyU64SharedZero>));
RW_FUNCTION(0x76E7D0, "std::unordered_map<unsigned long long, shared_ptr<const SafeBuffer>>::__insert_unique", (insertHeld<U64Key, constructU64Shared, sharedAt<0x10>>));
RW_FUNCTION(0x76E888, "std::unordered_map<unsigned long long, shared_ptr<const SafeBuffer>>::__construct_node", constructU64Shared);
RW_FUNCTION(0x76ED00, "std::unordered_map<unsigned long long, shared_ptr<const SafeBuffer>>::erase", (erase<U64Key, sharedAt<0x10>>));
RW_FUNCTION(0x76EF44, "std::unordered_map<unsigned long long, shared_ptr<const SafeBuffer>>::clear", clearTable<sharedAt<0x10>>);
RW_FUNCTION(0x76EF7C, "std::unordered_map<unsigned long long, shared_ptr<const SafeBuffer>>::__deallocate", deallocateNodes<sharedAt<0x10>>);
RW_FUNCTION(0x76E668, "std::unordered_map<unsigned long long, shared_ptr<const SafeBuffer>>::operator[]", (subscript<U64Key, 0x18, 0x10, keyU64SharedZero>));
RW_FUNCTION(0x77245E, "std::unordered_map<unsigned long long, shared_ptr<const SafeBuffer>>::~__hash_table", destroyTable<sharedAt<0x10>>);

RW_FUNCTION(0x4AE09A, "std::unordered_map<UserLink::PlatformType, UserLink::Info>::__insert_unique", (insertInline<ByteKey, 0x1C, fillPlatformInfo, stringAt<0x10>>));
RW_FUNCTION(0x4C4FE0, "std::unordered_map<UserLink::PlatformType, UserLink::Info>::operator[]", (subscript<ByteKey, 0x1C, 0xC, keyPlatformInfoZero>));

RW_FUNCTION(0x603C08, "std::unordered_map<SpeechDisp*, deque<string>>::__insert_unique", (insertHeld<PtrKey, constructSpeechQueue, speechQueue>));
RW_FUNCTION(0x603CC0, "std::unordered_map<SpeechDisp*, deque<string>>::__construct_node", constructSpeechQueue);
RW_FUNCTION(0x604FC8, "std::unordered_map<SpeechDisp*, deque<string>>::erase", (erase<PtrKey, speechQueue>));
RW_FUNCTION(0x605284, "std::unordered_map<SpeechDisp*, deque<string>>::clear", clearTable<speechQueue>);
RW_FUNCTION(0x6052BC, "std::unordered_map<SpeechDisp*, deque<string>>::__deallocate", deallocateNodes<speechQueue>);
RW_FUNCTION(0x608096, "std::unordered_map<SpeechDisp*, deque<string>>::~__hash_table", destroyTable<speechQueue>);
RW_FUNCTION(0x601970, "std::unordered_map<SpeechDisp*, deque<string>>::operator[]", (subscript<PtrKey, 0x24, 0xC, keySpeechQueueZero>));

RW_FUNCTION(0x5C96BC, "std::unordered_map<const char*, Avatar::Expression>::__insert_unique", (insertHeld<CStrKey, constructCStrWord, none>));
RW_FUNCTION(0x5C9750, "std::unordered_map<const char*, Avatar::Expression>::__construct_node", constructCStrWord);
RW_FUNCTION(0x5C9D84, "std::unordered_map<const char*, Avatar::Expression>::~__hash_table", destroyTable<none>);
RW_FUNCTION(0x8DDA90, "std::unordered_map<const char*, int>::__insert_unique", (insertHeld<CStrKey, constructCStrWord, none>));
RW_FUNCTION(0x8DDB08, "std::unordered_map<const char*, int>::__construct_node", constructCStrWord);
RW_FUNCTION(0x8DDF4A, "std::unordered_map<const char*, int>::clear", clearTable<none>);

RW_FUNCTION(0x4D930C, "std::unordered_map<string, iap::ProductInfo>::__assign_multi", assignMultiProductInfo);
RW_FUNCTION(0x4D9404, "std::unordered_map<string, iap::ProductInfo>::__node_insert_multi", Table<StringKey>::nodeInsertMulti);
RW_FUNCTION(0x4D96C0, "std::unordered_map<string, iap::ProductInfo>::__insert_multi", insertMultiProductInfo);
RW_FUNCTION(0x4D9774, "std::unordered_map<string, iap::ProductInfo>::__construct_node(const&) (multi)", constructProductInfo);
RW_FUNCTION(0x4FBAB0, "std::unordered_map<string, iap::ProductInfo>::__construct_node(const&)", constructProductInfo);
RW_FUNCTION(0x4E2018, "std::unordered_map<string, iap::ProductInfo>::__construct_node(&&)", constructProductInfoMove);
RW_FUNCTION(0x4E1F08, "std::unordered_map<string, iap::ProductInfo>::__insert_unique(&&)", (insertHeld<StringKey, constructProductInfoMove, productInfo>));
RW_FUNCTION(0x4FB9A0, "std::unordered_map<string, iap::ProductInfo>::__insert_unique(const&)", (insertHeld<StringKey, constructProductInfo, productInfo>));
RW_FUNCTION(0x4D966C, "std::unordered_map<string, iap::ProductInfo>::__deallocate", deallocateNodes<productInfo>);
RW_FUNCTION(0x4FBB72, "std::unordered_map<string, iap::ProductInfo>::clear", clearTable<productInfo>);
RW_FUNCTION(0x4DABC6, "std::unordered_map<string, iap::ProductInfo>::~__hash_table", destroyTable<productInfo>);
RW_FUNCTION(0x4F16A4, "std::unordered_map<string, shared_ptr<bg::Data>>::__insert_unique", (insertHeld<StringKey, constructStringData, stringKeyShared>));
RW_FUNCTION(0x4F1778, "std::unordered_map<string, shared_ptr<bg::Data>>::__construct_node", constructStringData);
RW_FUNCTION(0x4DAB90, "std::unordered_map<string, shared_ptr<bg::Data>>::__deallocate", deallocateNodes<stringKeyShared>);
RW_FUNCTION(0x4F12DC, "std::unordered_map<string, shared_ptr<bg::Data>>::clear", clearTable<stringKeyShared>);
RW_FUNCTION(0x5C23B8, "std::unordered_map<string, int>::__insert_unique", (insertHeld<StringKey, constructStringInt, stringKey>));
RW_FUNCTION(0x5C2478, "std::unordered_map<string, int>::__construct_node", constructStringInt);
RW_FUNCTION(0x5C2ACA, "std::unordered_map<string, int>::__deallocate", deallocateNodes<stringKey>);
RW_FUNCTION(0x5C2A92, "std::unordered_map<string, int>::clear", clearTable<stringKey>);
RW_FUNCTION(0x5C2AF6, "std::unordered_map<string, int>::~__hash_table", destroyTable<stringKey>);
RW_FUNCTION(0x6F62AA, "std::unordered_map<string, SpeechDisp*>::__deallocate", deallocateNodes<stringKey>);
RW_FUNCTION(0x5D844E, "std::unordered_map<string, Vec2>::__deallocate", deallocateNodes<stringKeyVec2>);
RW_FUNCTION(0x6F6254, "std::unordered_map<string, list<int>>::__deallocate", deallocateNodes<stringKeyIntList>);
RW_FUNCTION(0x7AFBA6, "std::unordered_map<string, cocostudio::RelativeData>::__deallocate", deallocateNodes<relativeData>);
RW_FUNCTION(0x7AFB6E, "std::unordered_map<string, cocostudio::RelativeData>::clear", clearTable<relativeData>);
RW_FUNCTION(0x7F1290, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::__insert_unique", (insertHeld<StringKey, constructStringContext, stringKeyShared>));
RW_FUNCTION(0x7F1328, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::__construct_node", constructStringContext);
RW_FUNCTION(0x7F1A90, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::__construct_node_with_key", constructContextWithKey);
RW_FUNCTION(0x7F0F98, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::__deallocate", deallocateNodes<stringKeyShared>);
RW_FUNCTION(0x7F0F60, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::clear", clearTable<stringKeyShared>);
RW_FUNCTION(0x7F181C, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::erase", (erase<StringKey, stringKeyShared>));
RW_FUNCTION(0x7F10C4, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::operator[]", subscriptStringContext);

// ---------------------------------------------------------------------------------------------- generic (Table<Key>)
RW_FUNCTION(0x603D60, "std::unordered_map<SpeechDisp*, deque<string>>::__node_insert_unique", Table<PtrKey>::nodeInsertUnique);
RW_FUNCTION(0x604018, "std::unordered_map<SpeechDisp*, deque<string>>::__rehash", Table<PtrKey>::rehashTo);
RW_FUNCTION(0x604F00, "std::unordered_map<SpeechDisp*, deque<string>>::find", Table<PtrKey>::find);
RW_FUNCTION(0x603F40, "std::unordered_map<SpeechDisp*, deque<string>>::rehash", Table<PtrKey>::rehash);
RW_FUNCTION(0x60503C, "std::unordered_map<SpeechDisp*, deque<string>>::remove", Table<PtrKey>::remove);
RW_FUNCTION(0x4AE130, "std::unordered_map<UserLink::PlatformType, UserLink::Info>::__node_insert_unique", Table<ByteKey>::nodeInsertUnique);
RW_FUNCTION(0x4AE39C, "std::unordered_map<UserLink::PlatformType, UserLink::Info>::__rehash", Table<ByteKey>::rehashTo);
RW_FUNCTION(0x47ABFA, "std::unordered_map<UserLink::PlatformType, UserLink::Info>::find", Table<ByteKey>::find);
RW_FUNCTION(0x4D526C, "std::unordered_map<UserLink::PlatformType, UserLink::Info>::find (2)", Table<ByteKey>::find);
RW_FUNCTION(0x4AE2C4, "std::unordered_map<UserLink::PlatformType, UserLink::Info>::rehash", Table<ByteKey>::rehash);
RW_FUNCTION(0x5C97A4, "std::unordered_map<char const*, Avatar::Expression>::__node_insert_unique", Table<CStrKey>::nodeInsertUnique);
RW_FUNCTION(0x5C9A34, "std::unordered_map<char const*, Avatar::Expression>::__rehash", Table<CStrKey>::rehashTo);
RW_FUNCTION(0x5C914C, "std::unordered_map<char const*, Avatar::Expression>::find", Table<CStrKey>::find);
RW_FUNCTION(0x5C995C, "std::unordered_map<char const*, Avatar::Expression>::rehash", Table<CStrKey>::rehash);
RW_FUNCTION(0x8DDB5C, "std::unordered_map<char const*, int>::__node_insert_unique", Table<CStrKey>::nodeInsertUnique);
RW_FUNCTION(0x8DDDEC, "std::unordered_map<char const*, int>::__rehash", Table<CStrKey>::rehashTo);
RW_FUNCTION(0x8DE18C, "std::unordered_map<char const*, int>::find", Table<CStrKey>::find);
RW_FUNCTION(0x8DE358, "std::unordered_map<char const*, int>::find (2)", Table<CStrKey>::find);
RW_FUNCTION(0x8DDD14, "std::unordered_map<char const*, int>::rehash", Table<CStrKey>::rehash);
RW_FUNCTION(0x5F717A, "std::unordered_map<int, BattleMarker*>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x5F73E4, "std::unordered_map<int, BattleMarker*>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x5F1250, "std::unordered_map<int, BattleMarker*>::find", Table<WordKey>::find);
RW_FUNCTION(0x5F730C, "std::unordered_map<int, BattleMarker*>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0xA428A4, "std::unordered_map<int, EventKeyboard::KeyCode>::find", Table<WordKey>::find);
RW_FUNCTION(0x75CF7E, "std::unordered_map<int, MapFieldLayer*>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x75D1E8, "std::unordered_map<int, MapFieldLayer*>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x758BC0, "std::unordered_map<int, MapFieldLayer*>::find", Table<WordKey>::find);
RW_FUNCTION(0x75CC4A, "std::unordered_map<int, MapFieldLayer*>::find (2)", Table<WordKey>::find);
RW_FUNCTION(0x75D110, "std::unordered_map<int, MapFieldLayer*>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x5C05C0, "std::unordered_map<int, audio::Manager::Bank>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x5C082C, "std::unordered_map<int, audio::Manager::Bank>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x5C092E, "std::unordered_map<int, audio::Manager::Bank>::find", Table<WordKey>::find);
RW_FUNCTION(0x5C0754, "std::unordered_map<int, audio::Manager::Bank>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x5C0444, "std::unordered_map<int, audio::Manager::Bank>::remove", Table<WordKey>::remove);
RW_FUNCTION(0x5C1B96, "std::unordered_map<int, audio::ResourceID>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x5C1E00, "std::unordered_map<int, audio::ResourceID>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x5C1D28, "std::unordered_map<int, audio::ResourceID>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x5C2124, "std::unordered_map<int, audio::ResourceID>::remove", Table<WordKey>::remove);
RW_FUNCTION(0x844726, "std::unordered_map<int, experimental::ui::VideoPlayer*>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x844990, "std::unordered_map<int, experimental::ui::VideoPlayer*>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x844550, "std::unordered_map<int, experimental::ui::VideoPlayer*>::find", Table<WordKey>::find);
RW_FUNCTION(0x8448B8, "std::unordered_map<int, experimental::ui::VideoPlayer*>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x741788, "std::unordered_map<int, float>::find", Table<WordKey>::find);
RW_FUNCTION(0x494364, "std::unordered_map<int, int>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x4945D0, "std::unordered_map<int, int>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x494250, "std::unordered_map<int, int>::find", Table<WordKey>::find);
RW_FUNCTION(0x497A20, "std::unordered_map<int, int>::find (2)", Table<WordKey>::find);
RW_FUNCTION(0x4944F8, "std::unordered_map<int, int>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x76F242, "std::unordered_map<int, long>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x76F4AC, "std::unordered_map<int, long>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x76F5AE, "std::unordered_map<int, long>::find", Table<WordKey>::find);
RW_FUNCTION(0x76F3D4, "std::unordered_map<int, long>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x81A7AA, "std::unordered_map<int, plugin::WebViewImpl*>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x81AA14, "std::unordered_map<int, plugin::WebViewImpl*>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x81A5B4, "std::unordered_map<int, plugin::WebViewImpl*>::find", Table<WordKey>::find);
RW_FUNCTION(0x81A93C, "std::unordered_map<int, plugin::WebViewImpl*>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x75D392, "std::unordered_map<int, vector<MapJumpInfo*>*>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x75D5FC, "std::unordered_map<int, vector<MapJumpInfo*>*>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x75848E, "std::unordered_map<int, vector<MapJumpInfo*>*>::find", Table<WordKey>::find);
RW_FUNCTION(0x758510, "std::unordered_map<int, vector<MapJumpInfo*>*>::find (2)", Table<WordKey>::find);
RW_FUNCTION(0x75D524, "std::unordered_map<int, vector<MapJumpInfo*>*>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x6F537E, "std::unordered_map<string, SpeechDisp*>::find", Table<StringKey>::find);
RW_FUNCTION(0x5D78F8, "std::unordered_map<string, Vec2>::find", Table<StringKey>::find);
RW_FUNCTION(0x4E20A4, "std::unordered_map<string, iap::ProductInfo>::__node_insert_unique", Table<StringKey>::nodeInsertUnique);
RW_FUNCTION(0x4D9A14, "std::unordered_map<string, iap::ProductInfo>::__rehash", Table<StringKey>::rehashTo);
RW_FUNCTION(0x4DA18E, "std::unordered_map<string, iap::ProductInfo>::find", Table<StringKey>::find);
RW_FUNCTION(0x4D993C, "std::unordered_map<string, iap::ProductInfo>::rehash", Table<StringKey>::rehash);
RW_FUNCTION(0x5C2504, "std::unordered_map<string, int>::__node_insert_unique", Table<StringKey>::nodeInsertUnique);
RW_FUNCTION(0x5C27E8, "std::unordered_map<string, int>::__rehash", Table<StringKey>::rehashTo);
RW_FUNCTION(0x5C2996, "std::unordered_map<string, int>::find", Table<StringKey>::find);
RW_FUNCTION(0x5C2710, "std::unordered_map<string, int>::rehash", Table<StringKey>::rehash);
RW_FUNCTION(0x7F1388, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::__node_insert_unique", Table<StringKey>::nodeInsertUnique);
RW_FUNCTION(0x7F166C, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::__rehash", Table<StringKey>::rehashTo);
RW_FUNCTION(0x7F1992, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::find", Table<StringKey>::find);
RW_FUNCTION(0x7F1594, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::rehash", Table<StringKey>::rehash);
RW_FUNCTION(0x7F189C, "std::unordered_map<string, shared_ptr<LWFTextRendererContext>>::remove", Table<StringKey>::remove);
RW_FUNCTION(0x4F180E, "std::unordered_map<string, shared_ptr<bg::Data>>::__node_insert_unique", Table<StringKey>::nodeInsertUnique);
RW_FUNCTION(0x4F1AF4, "std::unordered_map<string, shared_ptr<bg::Data>>::__rehash", Table<StringKey>::rehashTo);
RW_FUNCTION(0x4F15A6, "std::unordered_map<string, shared_ptr<bg::Data>>::find", Table<StringKey>::find);
RW_FUNCTION(0x4F1A1C, "std::unordered_map<string, shared_ptr<bg::Data>>::rehash", Table<StringKey>::rehash);
RW_FUNCTION(0x7FA0EC, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x7FA358, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x7FA5C6, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::find", Table<WordKey>::find);
RW_FUNCTION(0x7FA280, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x7FA4D0, "std::unordered_map<unsigned int, shared_ptr<LWF::Data>>::remove", Table<WordKey>::remove);
RW_FUNCTION(0x918C54, "std::unordered_map<unsigned int, shared_ptr<Texture2D>>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x918EC0, "std::unordered_map<unsigned int, shared_ptr<Texture2D>>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x91912E, "std::unordered_map<unsigned int, shared_ptr<Texture2D>>::find", Table<WordKey>::find);
RW_FUNCTION(0x918DE8, "std::unordered_map<unsigned int, shared_ptr<Texture2D>>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x919038, "std::unordered_map<unsigned int, shared_ptr<Texture2D>>::remove", Table<WordKey>::remove);
RW_FUNCTION(0x7F99D4, "std::unordered_map<unsigned int, shared_ptr<unordered_map<string, Value, hash<string>, equal_to<string>>>>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x7F9C40, "std::unordered_map<unsigned int, shared_ptr<unordered_map<string, Value, hash<string>, equal_to<string>>>>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x7F981E, "std::unordered_map<unsigned int, shared_ptr<unordered_map<string, Value, hash<string>, equal_to<string>>>>::find", Table<WordKey>::find);
RW_FUNCTION(0x7F9B68, "std::unordered_map<unsigned int, shared_ptr<unordered_map<string, Value, hash<string>, equal_to<string>>>>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x7F9728, "std::unordered_map<unsigned int, shared_ptr<unordered_map<string, Value, hash<string>, equal_to<string>>>>::remove", Table<WordKey>::remove);
RW_FUNCTION(0x4DFD8E, "std::unordered_map<unsigned int, string>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x4DFFF8, "std::unordered_map<unsigned int, string>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x4DF690, "std::unordered_map<unsigned int, string>::find", Table<WordKey>::find);
RW_FUNCTION(0x4DFF20, "std::unordered_map<unsigned int, string>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x4DF78C, "std::unordered_map<unsigned int, string>::remove", Table<WordKey>::remove);
RW_FUNCTION(0x4B4F84, "std::unordered_map<unsigned int, unsigned int>::__node_insert_unique", Table<WordKey>::nodeInsertUnique);
RW_FUNCTION(0x4B51F0, "std::unordered_map<unsigned int, unsigned int>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x4B52F2, "std::unordered_map<unsigned int, unsigned int>::find", Table<WordKey>::find);
RW_FUNCTION(0x4B5118, "std::unordered_map<unsigned int, unsigned int>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x4B1188, "std::unordered_map<unsigned long long, int>::__node_insert_unique", Table<U64Key>::nodeInsertUnique);
RW_FUNCTION(0x4B1458, "std::unordered_map<unsigned long long, int>::__rehash", Table<U64Key>::rehashTo);
RW_FUNCTION(0x490274, "std::unordered_map<unsigned long long, int>::find", Table<U64Key>::find);
RW_FUNCTION(0x4B1380, "std::unordered_map<unsigned long long, int>::rehash", Table<U64Key>::rehash);
RW_FUNCTION(0x5CAF7C, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::__node_insert_unique", Table<U64Key>::nodeInsertUnique);
RW_FUNCTION(0x5CB24C, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::__rehash", Table<U64Key>::rehashTo);
RW_FUNCTION(0x5CB4D4, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::find", Table<U64Key>::find);
RW_FUNCTION(0x5CB174, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::rehash", Table<U64Key>::rehash);
RW_FUNCTION(0x5CB3DC, "std::unordered_map<unsigned long long, shared_ptr<AvatarIcon>>::remove", Table<U64Key>::remove);
RW_FUNCTION(0x76E914, "std::unordered_map<unsigned long long, shared_ptr<SafeBuffer const>>::__node_insert_unique", Table<U64Key>::nodeInsertUnique);
RW_FUNCTION(0x76EBE4, "std::unordered_map<unsigned long long, shared_ptr<SafeBuffer const>>::__rehash", Table<U64Key>::rehashTo);
RW_FUNCTION(0x76EE6C, "std::unordered_map<unsigned long long, shared_ptr<SafeBuffer const>>::find", Table<U64Key>::find);
RW_FUNCTION(0x76EB0C, "std::unordered_map<unsigned long long, shared_ptr<SafeBuffer const>>::rehash", Table<U64Key>::rehash);
RW_FUNCTION(0x76ED74, "std::unordered_map<unsigned long long, shared_ptr<SafeBuffer const>>::remove", Table<U64Key>::remove);
RW_FUNCTION(0x6F527C, "std::unordered_set<SpeechDisp*>::__rehash", Table<PtrKey>::rehashTo);
RW_FUNCTION(0x6F452C, "std::unordered_set<SpeechDisp*>::find", Table<PtrKey>::find);
RW_FUNCTION(0x6F51A4, "std::unordered_set<SpeechDisp*>::rehash", Table<PtrKey>::rehash);
RW_FUNCTION(0x6F4328, "std::unordered_set<SpeechDisp*>::remove", Table<PtrKey>::remove);
RW_FUNCTION(0x5C02CC, "std::unordered_set<int>::__rehash", Table<WordKey>::rehashTo);
RW_FUNCTION(0x5BFE58, "std::unordered_set<int>::find", Table<WordKey>::find);
RW_FUNCTION(0x5C01F4, "std::unordered_set<int>::rehash", Table<WordKey>::rehash);
RW_FUNCTION(0x4DA8FC, "std::unordered_set<string>::__rehash", Table<StringKey>::rehashTo);
RW_FUNCTION(0x4DA824, "std::unordered_set<string>::rehash", Table<StringKey>::rehash);
RW_FUNCTION(0x4D6F30, "std::unordered_set<unsigned long long>::__node_insert_unique", Table<U64Key>::nodeInsertUnique);
RW_FUNCTION(0x4828FC, "std::unordered_set<unsigned long long>::__rehash", Table<U64Key>::rehashTo);
RW_FUNCTION(0x4D4AC0, "std::unordered_set<unsigned long long>::find", Table<U64Key>::find);
RW_FUNCTION(0x4D5460, "std::unordered_set<unsigned long long>::find (2)", Table<U64Key>::find);
RW_FUNCTION(0x482540, "std::unordered_set<unsigned long long>::rehash", Table<U64Key>::rehash);
