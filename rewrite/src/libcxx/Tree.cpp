// std::map / std::set instances over the game's (and the engine's) key and value types. The tree algorithms are
// libcxx::tree (Tree.hpp); what differs per instance is written here: the node size, how a value is built (copied,
// moved, value-initialised) and destroyed, and which instance members each one calls.
#include "libcxx/Tree.hpp"

using namespace libcxx::tree;
using libcxx::copyBytes;
using libcxx::copyString;
using libcxx::copyWords;
using libcxx::destroyFunction;
using libcxx::destroyShared;
using libcxx::destroyString;
using libcxx::moveShared;
using libcxx::moveTriple;
using rw::load;
using rw::save;

namespace {

constexpr uint32_t AEABI_MEMSET = 0x40E7A0;           // __aeabi_memset(dest, n, c) (import)
constexpr uint32_t STRING_ASSIGN = 0x498A1D;          // basic_string::operator=(const basic_string&) (sub_498A1C)
constexpr uint32_t VectorInt_dtor = 0x478961;         // __vector_base<int>::~__vector_base
constexpr uint32_t GachaData_copy = 0x5AE2CD;         // DrawScrollView::GachaData::GachaData(const GachaData&)
constexpr uint32_t FunctionMovie_copy = 0x530E0F;     // std::function<void(LWF::Movie*)>::function(const function&)

void memclear(uint32_t dest, uint32_t n) { rw::call(AEABI_MEMSET, {dest, n, 0}); }

// ------------------------------------------------------------------------------------------ values
// keys and plain values copied word by word: construct(nd, v) of Bytes bytes
template <uint32_t Bytes> void words(uint32_t nd, uint32_t v) { copyWords(nd + VALUE, v, Bytes); }

// value destructors (members last first, the key's string last)
template <uint32_t Off> void stringAt(uint32_t n) { destroyString(n + Off); }
template <uint32_t Off> void sharedAt(uint32_t n) { destroyShared(n + Off); }
void twoStrings(uint32_t n) {   // map<string, string>
    destroyString(n + 0x1C);
    destroyString(n + 0x10);
}
void stringFunction(uint32_t n) {   // map<string, function<...>>: the function at 0x20 (__f_ at 0x30)
    destroyFunction(n + 0x20);
    destroyString(n + 0x10);
}

// map<unsigned long long, UserPresent::Present>: key 0x10, words 0x18-0x33, a string at 0x34, 9 bytes at 0x40
void presentMove(uint32_t nd, uint32_t v) {
    copyWords(nd + 0x10, v, 8);
    copyWords(nd + 0x18, v + 8, 16);
    copyWords(nd + 0x28, v + 0x18, 12);
    moveTriple(nd + 0x34, v + 0x24);
    copyBytes(nd + 0x40, v + 0x30, 9);
}
void presentCopy(uint32_t nd, uint32_t v) {
    copyWords(nd + 0x18, v + 8, 16);
    copyWords(nd + 0x28, v + 0x18, 12);
    copyString(nd + 0x34, v + 0x24);
    copyBytes(nd + 0x40, v + 0x30, 9);
    copyWords(nd + 0x10, v, 8);
}
void presentInit(uint32_t nd, uint32_t k) {   // operator[]: the key, a zeroed Present
    copyWords(nd + 0x10, k, 8);
    memclear(nd + 0x18, 0x38);
}

// map<int, UserSphere::Sphere>: key, words 0x14-0x27, a vector<int> at 0x28
void sphereDestroy(uint32_t n) { rw::call(VectorInt_dtor, {n + 0x28}); }
void sphereMove(uint32_t nd, uint32_t v) {
    copyWords(nd + 0x10, v, 0x18);
    moveTriple(nd + 0x28, v + 0x18);
}

// map<int, DrawScrollView::GachaData>: strings at 0x14, 0x20, 0x38; words at 0x2C and 0x44
void gachaDestroy(uint32_t n) {
    destroyString(n + 0x38);
    destroyString(n + 0x20);
    destroyString(n + 0x14);
}
void gachaInit(uint32_t nd, uint32_t k) {
    save<uint32_t>(nd + 0x10, load<uint32_t>(k));
    memclear(nd + 0x14, 0x40);
}
void gachaCopy(uint32_t nd, uint32_t v) {
    save<uint32_t>(nd + 0x10, load<uint32_t>(v));
    rw::call(GachaData_copy, {nd + 0x14, v + 4});
    save<uint32_t>(nd + 0x10, load<uint32_t>(v));
}
void gachaAssign(uint32_t nd, uint32_t from) {   // the value of node `from` assigned over nd's
    save<uint32_t>(nd + 0x10, load<uint32_t>(from + 0x10));
    rw::call(STRING_ASSIGN, {nd + 0x14, from + 0x14});
    rw::call(STRING_ASSIGN, {nd + 0x20, from + 0x20});
    copyWords(nd + 0x2C, from + 0x2C, 12);
    rw::call(STRING_ASSIGN, {nd + 0x38, from + 0x38});
    copyWords(nd + 0x44, from + 0x44, 16);
}

// map<int, DrawScrollView::ShopData>: a map<int, GachaData> at 0x14
void shopDestroy(uint32_t n) { rw::call(0x5B387D, {n + 0x14, load<uint32_t>(n + 0x18)}); }   // its destroy(root)
void shopInit(uint32_t nd, uint32_t k) {
    save<uint32_t>(nd + 0x10, load<uint32_t>(k));
    save<uint32_t>(nd + 0x14, 0);
    save<uint32_t>(nd + 0x18, 0);
    save<uint32_t>(nd + 0x1C, 0);
    save<uint32_t>(nd + 0x14, nd + 0x18);
}

// map<int, RaidSubdueScrollView::SubdueData>: a string at 0x24
void subdueInit(uint32_t nd, uint32_t k) {
    save<uint32_t>(nd + 0x10, load<uint32_t>(k));
    memclear(nd + 0x14, 0x50);
}

// map<int, shared_ptr<audio::Layer>>: moved in
void layerMove(uint32_t nd, uint32_t v) {
    save<uint32_t>(nd + 0x10, load<uint32_t>(v));
    moveShared(nd + 0x14, v + 4);
}

// map<string, function<void(bool, const char*)>>::operator[](string&&): the key moved in, an empty function
void boolFunctionInit(uint32_t nd, uint32_t k) {
    moveTriple(nd + 0x10, k);
    save<uint32_t>(nd + 0x30, 0);
}
// map<string, function<void(LWF::Movie*)>>: key and function copied
void movieFunctionCopy(uint32_t nd, uint32_t v) {
    copyString(nd + 0x10, v);
    rw::call(FunctionMovie_copy, {nd + 0x20, v + 0x10});
}

// mapped values value-initialised by map::__construct_node_with_key
void boolInit(uint32_t nd) { save<uint8_t>(nd + 0x1C, 0); }
void movieEventHandlersInit(uint32_t nd) {   // LWF::MovieEventHandlers: empty flag set, six empty vectors
    save<uint8_t>(nd + 0x1C, 1);
    for (uint32_t p = nd + 0x20; p != nd + 0x68; p += 12) {
        save<uint32_t>(p, 0);
        save<uint32_t>(p + 4, 0);
        save<uint32_t>(p + 8, 0);
    }
}
void sharedInit(uint32_t nd) {
    save<uint32_t>(nd + 0x1C, 0);
    save<uint32_t>(nd + 0x20, 0);
}

void timezoneAssign(uint32_t nd, uint32_t from) { save<uint32_t>(nd + 0x10, load<uint32_t>(from + 0x10)); }

// ------------------------------------------------------------------------------------------ multi inserts
// set<APIManager::Timezone>::__insert_multi(v): the slot first, then the node
uint32_t timezoneInsertMulti(uint32_t t, uint32_t v) {
    uint32_t parentNode;
    const uint32_t child = Search<IntKey>::findLeafHigh(t, v, parentNode);
    const uint32_t nd = guest::newObject(0x14);
    save<uint32_t>(nd + 0x10, load<uint32_t>(v));
    rw::call(0x54CE09, {t, parentNode, child, nd});
    return nd;
}

// multimap<int, int>::__insert_multi(v): the node first, then the slot
uint32_t intIntInsertMulti(uint32_t t, uint32_t v) {
    const uint32_t nd = guest::newObject(0x18);
    copyWords(nd + 0x10, v, 8);
    uint32_t parentNode;
    const uint32_t child = Search<IntKey>::findLeafHigh(t, v, parentNode);
    rw::call(0x5BF923, {t, parentNode, child, nd});
    return nd;
}

// map<int, GachaData>::__insert_multi(v): the slot, then the node through __construct_node and its holder
uint32_t gachaInsertMulti(uint32_t t, uint32_t v) {
    uint32_t parentNode;
    const uint32_t child = Search<IntKey>::findLeafHigh(t, v, parentNode);
    guest::Frame holder(12);
    rw::call(0x5B4249, {holder.addr(), t, v});
    const uint32_t nd = load<uint32_t>(holder.addr());
    rw::call(0x5B3DD1, {t, parentNode, child, nd});
    save<uint32_t>(holder.addr(), 0);
    return nd;
}

// std::__lower_bound<__less<Timezone>&>(first, last, value): binary search over set iterators (std::advance)
uint32_t timezoneLowerBound(uint32_t first, uint32_t last, uint32_t value) {
    int32_t len = 0;
    for (uint32_t it = first; it != last; it = treeNext(it)) len++;
    while (len != 0) {
        const int32_t half = len / 2;
        uint32_t m = first;
        if (half >= 0) {
            for (int32_t k = 0; k < half; k++) m = treeNext(m);
        } else {
            for (int32_t k = half; k < 0; k++) m = treePrev(m);
        }
        if (int32_t(load<uint32_t>(m + VALUE)) < int32_t(load<uint32_t>(value))) {
            first = treeNext(m);
            len -= half + 1;
        } else {
            len = half;
        }
    }
    return first;
}

}  // namespace

// ---------------------------------------------------------------------------------------------- red-black core
RW_FUNCTION(0x493F5A, "std::__tree_balance_after_insert", balanceAfterInsert);
RW_FUNCTION(0x493FFA, "std::__tree_left_rotate", leftRotate);
RW_FUNCTION(0x49401E, "std::__tree_right_rotate", rightRotate);
RW_FUNCTION(0x4AF826, "std::__tree_remove", treeRemove);

// ---------------------------------------------------------------------------------------------- set<int>
RW_FUNCTION(0x48D9B4, "std::set<int>::destroy", (destroy<0x48D9B5, noValue>));
RW_FUNCTION(0x497AA2, "std::set<int>::find", Search<IntKey>::find);
RW_FUNCTION(0x6BCA04, "std::set<int>::__insert_unique(hint, const int&)", (insertUniqueHintSearchFirst<0x14, words<4>, 0x6BCA7D, 0x6BCB59>));
RW_FUNCTION(0x6BCA7C, "std::set<int>::__find_equal(hint)", Search<IntKey>::findEqualHint<0x6BCB89>);
RW_FUNCTION(0x6BCB58, "std::set<int>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x6BCB88, "std::set<int>::__find_equal", Search<IntKey>::findEqual);
RW_FUNCTION(0x6BCBD4, "std::set<int>::__insert_unique(int&&)", (insertUniqueNodeFirst<0x14, words<4>, 0x6BCC19, noValue>));
RW_FUNCTION(0x6BCC18, "std::set<int>::__node_insert_unique", (nodeInsertUnique<0x6BCB89, 0x6BCB59>));
RW_FUNCTION(0x76F630, "std::set<int>::__insert_unique(const int&)", (insertUniqueSearchFirst<0x14, words<4>, 0x6BCB89, 0x6BCB59>));

// ---------------------------------------------------------------------------------------------- set<unsigned>
RW_FUNCTION(0x4F09F8, "std::set<unsigned>::find", Search<UintKey>::find);
RW_FUNCTION(0x4F0A3C, "std::set<unsigned>::__insert_unique(const unsigned&)", (insertUniqueSearchFirst<0x14, words<4>, 0x4F0AC5, 0x4F0B11>));
RW_FUNCTION(0x4F0AC4, "std::set<unsigned>::__find_equal", Search<UintKey>::findEqual);
RW_FUNCTION(0x4F0B10, "std::set<unsigned>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x4F0B40, "std::set<unsigned>::destroy", (destroy<0x4F0B41, noValue>));

// ---------------------------------------------------------------------------------------------- set<void*>
RW_FUNCTION(0x5EDDE4, "std::set<void*>::destroy", (destroy<0x5EDDE5, noValue>));
RW_FUNCTION(0x5EDE10, "std::set<void*>::__move_assign", (moveAssign<0x5EDDE5, false>));

// ---------------------------------------------------------------------------------------------- set<UserAlbumChallenge::Challenge, bool(*)(const&, const&)>
RW_FUNCTION(0x4B04FE, "std::set<UserAlbumChallenge::Challenge>::__erase_unique", (eraseUnique<0x4B0521, 0x4B0589>));
RW_FUNCTION(0x4B0520, "std::set<UserAlbumChallenge::Challenge>::find", Search<FnKey>::find);
RW_FUNCTION(0x4B0588, "std::set<UserAlbumChallenge::Challenge>::erase", erase<noValue>);
RW_FUNCTION(0x4B0630, "std::set<UserAlbumChallenge::Challenge>::__insert_unique(const&)", (insertUniqueSearchFirst<0x1C, words<12>, 0x4B06BD, 0x4B0733>));
RW_FUNCTION(0x4B06BC, "std::set<UserAlbumChallenge::Challenge>::__find_equal", Search<FnKey>::findEqual);
RW_FUNCTION(0x4B0732, "std::set<UserAlbumChallenge::Challenge>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x4B07B0, "std::set<UserAlbumChallenge::Challenge>::__move_assign", (moveAssign<0x4B07E7, true>));
RW_FUNCTION(0x4B07E6, "std::set<UserAlbumChallenge::Challenge>::destroy", (destroy<0x4B07E7, noValue>));

// ---------------------------------------------------------------------------------------------- set<APIManager::Timezone>
RW_FUNCTION(0x54B0D0, "std::__lower_bound<__less<APIManager::Timezone>&, set iterator>", timezoneLowerBound);
RW_FUNCTION(0x54CC0C, "std::set<APIManager::Timezone>::__assign_multi", (assignMulti<0x54CCE5, 0x54CD2D, 0x54CD73, 0x54CD9F, timezoneAssign>));
RW_FUNCTION(0x54CCE4, "std::set<APIManager::Timezone>::__detach", detach);
RW_FUNCTION(0x54CD2C, "std::set<APIManager::Timezone>::__node_insert_multi", (nodeInsertMulti<IntKey, 0x54CE09>));
RW_FUNCTION(0x54CD72, "std::set<APIManager::Timezone>::destroy", (destroy<0x54CD73, noValue>));
RW_FUNCTION(0x54CD9E, "std::set<APIManager::Timezone>::__insert_multi(const&)", timezoneInsertMulti);
RW_FUNCTION(0x54CE08, "std::set<APIManager::Timezone>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x54CE38, "std::set<APIManager::Timezone>::__insert_unique(&&)", (insertUniqueNodeFirst<0x14, words<4>, 0x54CE7D, noValue>));
RW_FUNCTION(0x54CE7C, "std::set<APIManager::Timezone>::__node_insert_unique", (nodeInsertUnique<0x54CEF5, 0x54CE09>));
RW_FUNCTION(0x54CEF4, "std::set<APIManager::Timezone>::__find_equal", Search<IntKey>::findEqual);

// ---------------------------------------------------------------------------------------------- map<unsigned long long, UserPresent::Present>
RW_FUNCTION(0x4AF444, "std::map<unsigned long long, UserPresent::Present>::find", Search<U64Key>::find);
RW_FUNCTION(0x4AF538, "std::map<unsigned long long, UserPresent::Present>::__insert_unique(pair&&)", (insertUniqueNodeFirst<0x50, presentMove, 0x4AF5C9, stringAt<0x34>>));
RW_FUNCTION(0x4AF5C8, "std::map<unsigned long long, UserPresent::Present>::__node_insert_unique", (nodeInsertUnique<0x4AF641, 0x4AF6D7>));
RW_FUNCTION(0x4AF640, "std::map<unsigned long long, UserPresent::Present>::__find_equal", Search<U64Key>::findEqual);
RW_FUNCTION(0x4AF6D6, "std::map<unsigned long long, UserPresent::Present>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x4B5380, "std::map<unsigned long long, UserPresent::Present>::destroy", (destroy<0x4B5381, stringAt<0x34>>));
RW_FUNCTION(0x50DC74, "std::map<unsigned long long, UserPresent::Present>::operator[]", (subscript<0x510565, 0x4AF6D7, 0x50, 0x18, presentInit>));
RW_FUNCTION(0x510564, "std::map<unsigned long long, UserPresent::Present>::__find_equal_key", Search<U64Key>::findEqual);
RW_FUNCTION(0x5105FC, "std::map<unsigned long long, UserPresent::Present>::__insert_unique(hint, const pair&)", (insertUniqueHintHeld<0x510695, 0x510755, stringAt<0x34>>));
RW_FUNCTION(0x510694, "std::map<unsigned long long, UserPresent::Present>::__construct_node", (constructNode<0x50, presentCopy>));
RW_FUNCTION(0x510754, "std::map<unsigned long long, UserPresent::Present>::__node_insert_unique(hint)", (nodeInsertUniqueHint<0x5107C5, 0x4AF6D7>));
RW_FUNCTION(0x5107C4, "std::map<unsigned long long, UserPresent::Present>::__find_equal(hint)", Search<U64Key>::findEqualHint<0x4AF641>);

// ---------------------------------------------------------------------------------------------- map<unsigned long long, bool>
RW_FUNCTION(0x5004F0, "std::map<unsigned long long, bool>::destroy", (destroy<0x5004F1, noValue>));

// ---------------------------------------------------------------------------------------------- map<int, UserSphere::Sphere>
RW_FUNCTION(0x4AF768, "std::map<int, UserSphere::Sphere>::__erase_unique", (eraseUnique<0x4AF78B, 0x4AF7CF>));
RW_FUNCTION(0x4AF78A, "std::map<int, UserSphere::Sphere>::find", Search<IntKey>::find);
RW_FUNCTION(0x503478, "std::map<int, UserSphere::Sphere>::find const", Search<IntKey>::find);
RW_FUNCTION(0x4AF7CE, "std::map<int, UserSphere::Sphere>::erase", erase<sphereDestroy>);
RW_FUNCTION(0x4AFA78, "std::map<int, UserSphere::Sphere>::__insert_unique(pair&&)", (insertUniqueNodeFirst<0x34, sphereMove, 0x4AFAED, sphereDestroy>));
RW_FUNCTION(0x4AFAEC, "std::map<int, UserSphere::Sphere>::__node_insert_unique", (nodeInsertUnique<0x4AFB65, 0x4AFBB1>));
RW_FUNCTION(0x4AFB64, "std::map<int, UserSphere::Sphere>::__find_equal", Search<IntKey>::findEqual);
RW_FUNCTION(0x4AFBB0, "std::map<int, UserSphere::Sphere>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x4B53BC, "std::map<int, UserSphere::Sphere>::destroy", (destroy<0x4B53BD, sphereDestroy>));

// ---------------------------------------------------------------------------------------------- map<unsigned, TextFormat Tag*>
RW_FUNCTION(0x494194, "std::map<unsigned, Tag*>::destroy (TextFormat)", (destroy<0x494195, noValue>));

// ---------------------------------------------------------------------------------------------- map<int, pointer / int>
RW_FUNCTION(0x588C02, "std::map<int, Texture2D*>::__find_equal_key", Search<IntKey>::findEqual);
RW_FUNCTION(0x588C4E, "std::map<int, Texture2D*>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x588C7E, "std::map<int, Texture2D*>::destroy", (destroy<0x588C7F, noValue>));
RW_FUNCTION(0x652510, "std::map<int, Image*>::destroy", (destroy<0x652511, noValue>));
RW_FUNCTION(0x6C6334, "std::map<int, Label*>::destroy", (destroy<0x6C6335, noValue>));
RW_FUNCTION(0x5B42A6, "std::map<int, unsigned>::destroy", (destroy<0x5B42A7, noValue>));
RW_FUNCTION(0x5BF8C6, "std::multimap<int, int>::__insert_multi(const pair&)", intIntInsertMulti);
RW_FUNCTION(0x5BF922, "std::multimap<int, int>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x5BFE0C, "std::multimap<int, int>::destroy", (destroy<0x5BFE0D, noValue>));
RW_FUNCTION(0x6B5C24, "std::multimap<int, int>::find", Search<IntKey>::find);
RW_FUNCTION(0x8153E0, "std::map<int, shared_ptr<LWF::LWFContainer>>::find", Search<IntKey>::find);

// ---------------------------------------------------------------------------------------------- map<int, ActionManager*> / map<int, Scheduler*>
RW_FUNCTION(0x8F10F0, "std::map<int, ActionManager*>::__node_insert_unique", (nodeInsertUnique<0x8F1169, 0x8F11B5>));
RW_FUNCTION(0x8F1168, "std::map<int, ActionManager*>::__find_equal", Search<IntKey>::findEqual);
RW_FUNCTION(0x8F11B4, "std::map<int, ActionManager*>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x8F11E4, "std::map<int, ActionManager*>::__find_equal_key", Search<IntKey>::findEqual);
RW_FUNCTION(0x8F1272, "std::map<int, ActionManager*>::erase", erase<noValue>);
RW_FUNCTION(0x8F12C0, "std::map<int, ActionManager*>::find", Search<IntKey>::find);
RW_FUNCTION(0x8F143C, "std::map<int, ActionManager*>::__insert_unique(pair&&)", (insertUniqueNodeFirst<0x18, words<8>, 0x8F10F1, noValue>));
RW_FUNCTION(0x8F1230, "std::map<int, Scheduler*>::__find_equal_key const", Search<IntKey>::findEqualValue);
RW_FUNCTION(0x8F1304, "std::map<int, Scheduler*>::erase", erase<noValue>);
RW_FUNCTION(0x8F1352, "std::map<int, Scheduler*>::find", Search<IntKey>::find);
RW_FUNCTION(0x8F1484, "std::map<int, Scheduler*>::__insert_unique(pair&&)", (insertUniqueNodeFirst<0x18, words<8>, 0x8F14CD, noValue>));
RW_FUNCTION(0x8F14CC, "std::map<int, Scheduler*>::__node_insert_unique", (nodeInsertUnique<0x8F1545, 0x8F1591>));
RW_FUNCTION(0x8F1544, "std::map<int, Scheduler*>::__find_equal", Search<IntKey>::findEqual);
RW_FUNCTION(0x8F1590, "std::map<int, Scheduler*>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x8F1608, "std::map<int, Scheduler*>::__find_equal_key", Search<IntKey>::findEqual);

// ---------------------------------------------------------------------------------------------- map<int, audio::Manager::StartParameter> / map<int, shared_ptr<audio::Layer>>
RW_FUNCTION(0x5BF952, "std::map<int, audio::Manager::StartParameter>::__insert_unique(pair&&)", (insertUniqueNodeFirst<0x20, words<16>, 0x5BF9A1, noValue>));
RW_FUNCTION(0x5BF9A0, "std::map<int, audio::Manager::StartParameter>::__node_insert_unique", (nodeInsertUnique<0x5BFA19, 0x5BFA65>));
RW_FUNCTION(0x5BFA18, "std::map<int, audio::Manager::StartParameter>::__find_equal", Search<IntKey>::findEqual);
RW_FUNCTION(0x5BFA64, "std::map<int, audio::Manager::StartParameter>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x5BFA94, "std::map<int, audio::Manager::StartParameter>::erase", erase<noValue>);
RW_FUNCTION(0x5BFAE2, "std::map<int, audio::Manager::StartParameter>::find", Search<IntKey>::find);
RW_FUNCTION(0x5C0B3E, "std::map<int, audio::Manager::StartParameter>::destroy", (destroy<0x5C0B3F, noValue>));
RW_FUNCTION(0x5BFB26, "std::map<int, shared_ptr<audio::Layer>>::find", Search<IntKey>::find);
RW_FUNCTION(0x5BFB6A, "std::map<int, shared_ptr<audio::Layer>>::__erase_unique", (eraseUnique<0x5BFB27, 0x5BFB8D>));
RW_FUNCTION(0x5BFB8C, "std::map<int, shared_ptr<audio::Layer>>::erase", erase<sharedAt<0x14>>);
RW_FUNCTION(0x5BFBE4, "std::map<int, shared_ptr<audio::Layer>>::__insert_unique(pair&&)", (insertUniqueNodeFirst<0x1C, layerMove, 0x5BFC41, sharedAt<0x14>>));
RW_FUNCTION(0x5BFC40, "std::map<int, shared_ptr<audio::Layer>>::__node_insert_unique", (nodeInsertUnique<0x5BFCB9, 0x5BFD05>));
RW_FUNCTION(0x5BFCB8, "std::map<int, shared_ptr<audio::Layer>>::__find_equal", Search<IntKey>::findEqual);
RW_FUNCTION(0x5BFD04, "std::map<int, shared_ptr<audio::Layer>>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x5C0B08, "std::map<int, shared_ptr<audio::Layer>>::destroy", (destroy<0x5C0B09, sharedAt<0x14>>));

// ---------------------------------------------------------------------------------------------- map<int, DrawScrollView::GachaData / ShopData>, map<int, SubdueData>
RW_FUNCTION(0x5AE244, "std::map<int, DrawScrollView::GachaData>::operator[]", (subscript<0x5B3D85, 0x5B3DD1, 0x54, 0x14, gachaInit>));
RW_FUNCTION(0x5B387C, "std::map<int, DrawScrollView::GachaData>::destroy", (destroy<0x5B387D, gachaDestroy>));
RW_FUNCTION(0x5B3D84, "std::map<int, DrawScrollView::GachaData>::__find_equal_key", Search<IntKey>::findEqual);
RW_FUNCTION(0x5B3DD0, "std::map<int, DrawScrollView::GachaData>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x5B3FC8, "std::map<int, DrawScrollView::GachaData>::__assign_multi", (assignMulti<0x5B411D, 0x5B4165, 0x5B387D, 0x5B41AD, gachaAssign>));
RW_FUNCTION(0x5B411C, "std::map<int, DrawScrollView::GachaData>::__detach", detach);
RW_FUNCTION(0x5B4164, "std::map<int, DrawScrollView::GachaData>::__node_insert_multi", (nodeInsertMulti<IntKey, 0x5B3DD1>));
RW_FUNCTION(0x5B41AC, "std::map<int, DrawScrollView::GachaData>::__insert_multi(const pair&)", gachaInsertMulti);
RW_FUNCTION(0x5B4248, "std::map<int, DrawScrollView::GachaData>::__construct_node", (constructNode<0x54, gachaCopy>));
RW_FUNCTION(0x5AEC68, "std::map<int, DrawScrollView::ShopData>::operator[]", (subscript<0x5B38D3, 0x5B391F, 0x20, 0x14, shopInit>));
RW_FUNCTION(0x5B38D2, "std::map<int, DrawScrollView::ShopData>::__find_equal_key", Search<IntKey>::findEqual);
RW_FUNCTION(0x5B391E, "std::map<int, DrawScrollView::ShopData>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x5B394E, "std::map<int, DrawScrollView::ShopData>::find", Search<IntKey>::find);
RW_FUNCTION(0x5B3992, "std::map<int, DrawScrollView::ShopData>::destroy", (destroy<0x5B3993, shopDestroy>));
RW_FUNCTION(0x5B5720, "std::map<int, RaidSubdueScrollView::SubdueData>::operator[]", (subscript<0x5BAB4D, 0x5BAB99, 0x64, 0x14, subdueInit>));
RW_FUNCTION(0x5BAB10, "std::map<int, RaidSubdueScrollView::SubdueData>::destroy", (destroy<0x5BAB11, stringAt<0x24>>));
RW_FUNCTION(0x5BAB4C, "std::map<int, RaidSubdueScrollView::SubdueData>::__find_equal_key", Search<IntKey>::findEqual);
RW_FUNCTION(0x5BAB98, "std::map<int, RaidSubdueScrollView::SubdueData>::__insert_node_at", insertNodeAt);

// ---------------------------------------------------------------------------------------------- string-keyed maps
RW_FUNCTION(0x530810, "std::map<string, function<void(LWF::Movie*)>>::destroy", (destroy<0x530811, stringFunction>));
RW_FUNCTION(0x530868, "std::map<string, function<void(LWF::Movie*)>>::__insert_unique(hint, const pair&)", (insertUniqueHintHeld<0x53091D, 0x5309BD, stringFunction>));
RW_FUNCTION(0x53091C, "std::map<string, function<void(LWF::Movie*)>>::__construct_node", (constructNode<0x38, movieFunctionCopy>));
RW_FUNCTION(0x5309BC, "std::map<string, function<void(LWF::Movie*)>>::__node_insert_unique(hint)", (nodeInsertUniqueHint<0x530A2D, 0x530CC1>));
RW_FUNCTION(0x530A2C, "std::map<string, function<void(LWF::Movie*)>>::__find_equal(hint)", Search<StringKey>::findEqualHint<0x530CF1>);
RW_FUNCTION(0x530CC0, "std::map<string, function<void(LWF::Movie*)>>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x530CF0, "std::map<string, function<void(LWF::Movie*)>>::__find_equal", Search<StringKey>::findEqual);
RW_FUNCTION(0x5BDB8C, "std::map<string, function<void(bool, const char*)>>::operator[](string&&)", (subscript<0x5BE431, 0x5BE54F, 0x38, 0x20, boolFunctionInit>));
RW_FUNCTION(0x5BE1E4, "std::map<string, function<void(bool, const char*)>>::find", Search<StringKey>::findVia<0x5BE269>);
RW_FUNCTION(0x5BE268, "std::map<string, function<void(bool, const char*)>>::__lower_bound", Search<StringKey>::lowerBound);
RW_FUNCTION(0x5BE430, "std::map<string, function<void(bool, const char*)>>::__find_equal_key", Search<StringKey>::findEqual);
RW_FUNCTION(0x5BE54E, "std::map<string, function<void(bool, const char*)>>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x5BECA6, "std::map<string, function<void(bool, const char*)>>::destroy", (destroy<0x5BECA7, stringFunction>));
RW_FUNCTION(0x5BE57E, "std::map<string, unsigned>::destroy", (destroy<0x5BE57F, stringAt<0x10>>));
RW_FUNCTION(0x5BEB54, "std::map<string, Image*>::find", Search<StringKey>::findVia<0x5BEBD9>);
RW_FUNCTION(0x5BEBD8, "std::map<string, Image*>::__lower_bound", Search<StringKey>::lowerBound);
RW_FUNCTION(0x5BEC6C, "std::map<string, Image*>::destroy", (destroy<0x5BEC6D, stringAt<0x10>>));
RW_FUNCTION(0x621D6C, "std::map<string, string>::find", Search<StringKey>::findVia<0x621DF1>);
RW_FUNCTION(0x621DF0, "std::map<string, string>::__lower_bound", Search<StringKey>::lowerBound);
RW_FUNCTION(0x622034, "std::map<string, string>::destroy", (destroy<0x622035, twoStrings>));
RW_FUNCTION(0x7EC624, "std::map<string, bool>::operator[]", (subscriptWithKey<0x7ECCF7, 0x7ECE15, 0x7ECE59, 0x1C>));
RW_FUNCTION(0x7ECCF6, "std::map<string, bool>::__find_equal_key", Search<StringKey>::findEqual);
RW_FUNCTION(0x7ECE14, "std::map<string, bool>::__construct_node_with_key", (constructNodeWithKey<0x20, boolInit>));
RW_FUNCTION(0x7ECE58, "std::map<string, bool>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x808820, "std::map<string, LWF::MovieEventHandlers>::operator[]", (subscriptWithKey<0x8097D9, 0x8098F9, 0x809959, 0x1C>));
RW_FUNCTION(0x8097D8, "std::map<string, LWF::MovieEventHandlers>::__find_equal_key", Search<StringKey>::findEqual);
RW_FUNCTION(0x8098F8, "std::map<string, LWF::MovieEventHandlers>::__construct_node_with_key", (constructNodeWithKey<0x68, movieEventHandlersInit>));
RW_FUNCTION(0x809958, "std::map<string, LWF::MovieEventHandlers>::__insert_node_at", insertNodeAt);
RW_FUNCTION(0x809988, "std::map<string, LWF::MovieEventHandlers>::find", Search<StringKey>::findVia<0x809A0D>);
RW_FUNCTION(0x809A0C, "std::map<string, LWF::MovieEventHandlers>::__lower_bound", Search<StringKey>::lowerBound);
RW_FUNCTION(0x813356, "std::map<string, shared_ptr<LWF::LWFContainer>>::find", Search<StringKey>::findVia<0x8133DB>);
RW_FUNCTION(0x8133DA, "std::map<string, shared_ptr<LWF::LWFContainer>>::__lower_bound", Search<StringKey>::lowerBound);
RW_FUNCTION(0x814B50, "std::map<string, shared_ptr<LWF::LWFContainer>>::operator[]", (subscriptWithKey<0x814F07, 0x815025, 0x81506D, 0x1C>));
RW_FUNCTION(0x814F06, "std::map<string, shared_ptr<LWF::LWFContainer>>::__find_equal_key", Search<StringKey>::findEqual);
RW_FUNCTION(0x815024, "std::map<string, shared_ptr<LWF::LWFContainer>>::__construct_node_with_key", (constructNodeWithKey<0x24, sharedInit>));
RW_FUNCTION(0x81506C, "std::map<string, shared_ptr<LWF::LWFContainer>>::__insert_node_at", insertNodeAt);
