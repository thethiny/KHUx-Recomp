// std::list instances over the game's element types (libc++ 3.5). Layout: the end node {prev +0, next +4}, the size
// +8; a node: prev +0, next +4, the value +8.
#include "libcxx/Libcxx.hpp"

using libcxx::copyShared;
using libcxx::destroyShared;
using libcxx::destroyString;
using rw::load;
using rw::save;

namespace {

constexpr uint32_t PREV = 0, NEXT = 4, SIZE = 8, VALUE = 8;

void noValue(uint32_t) {}
void sharedValue(uint32_t n) { destroyShared(n + VALUE); }
void stringValue(uint32_t n) { destroyString(n + VALUE); }
void bankValue(uint32_t n) { destroyShared(n + VALUE + 4); }   // audio::Manager::Bank: an id, a shared_ptr

// __unlink_nodes(f, l): the chain f..l taken out of the list
void unlinkNodes(uint32_t f, uint32_t l) {
    save<uint32_t>(load<uint32_t>(f + PREV) + NEXT, load<uint32_t>(l + NEXT));
    save<uint32_t>(load<uint32_t>(l + NEXT) + PREV, load<uint32_t>(f + PREV));
}

// __list_imp::clear(): every node unlinked, destroyed and freed
template <void (*Destroy)(uint32_t)> void clear(uint32_t list) {
    if (!load<uint32_t>(list + SIZE)) return;
    uint32_t f = load<uint32_t>(list + NEXT);
    unlinkNodes(f, load<uint32_t>(list + PREV));
    save<uint32_t>(list + SIZE, 0);
    while (f != list) {
        const uint32_t next = load<uint32_t>(f + NEXT);
        Destroy(f);
        guest::deleteObject(f);
        f = next;
    }
}

// erase(pos) -> the next node
template <void (*Destroy)(uint32_t)> uint32_t eraseOne(uint32_t list, uint32_t p) {
    const uint32_t next = load<uint32_t>(p + NEXT);
    save<uint32_t>(load<uint32_t>(p + PREV) + NEXT, next);
    save<uint32_t>(load<uint32_t>(p + NEXT) + PREV, load<uint32_t>(p + PREV));
    save<uint32_t>(list + SIZE, load<uint32_t>(list + SIZE) - 1);
    Destroy(p);
    guest::deleteObject(p);
    return next;
}

// erase(f, l) -> l
template <void (*Destroy)(uint32_t)> uint32_t eraseRange(uint32_t list, uint32_t f, uint32_t l) {
    if (f == l) return l;
    unlinkNodes(f, load<uint32_t>(l + PREV));
    while (f != l) {
        const uint32_t next = load<uint32_t>(f + NEXT);
        save<uint32_t>(list + SIZE, load<uint32_t>(list + SIZE) - 1);
        Destroy(f);
        guest::deleteObject(f);
        f = next;
    }
    return l;
}

// remove(x): each run of nodes equal to x (compared by the first word: the pointer, or shared_ptr::get())
// erased with the instance's erase(f, l); the run is matched against the first match's value
template <uint32_t EraseRange> void remove(uint32_t list, uint32_t x) {
    uint32_t i = load<uint32_t>(list + NEXT);
    while (i != list) {
        const uint32_t v = load<uint32_t>(i + VALUE);
        if (v != load<uint32_t>(x)) {
            i = load<uint32_t>(i + NEXT);
            continue;
        }
        uint32_t j = load<uint32_t>(i + NEXT);
        while (j != list && load<uint32_t>(j + VALUE) == v) j = load<uint32_t>(j + NEXT);
        i = rw::call(EraseRange, {list, i, j});
    }
}

// push_back / push_front: a node of NodeSize bytes, its value built by construct(node, x)
template <uint32_t NodeSize, void (*Construct)(uint32_t, uint32_t)> void pushBack(uint32_t list, uint32_t x) {
    const uint32_t n = guest::newObject(NodeSize);
    Construct(n, x);
    save<uint32_t>(load<uint32_t>(list + PREV) + NEXT, n);
    save<uint32_t>(n + PREV, load<uint32_t>(list + PREV));
    save<uint32_t>(list + PREV, n);
    save<uint32_t>(n + NEXT, list);
    save<uint32_t>(list + SIZE, load<uint32_t>(list + SIZE) + 1);
}
template <uint32_t NodeSize, void (*Construct)(uint32_t, uint32_t)> void pushFront(uint32_t list, uint32_t x) {
    const uint32_t n = guest::newObject(NodeSize);
    Construct(n, x);
    const uint32_t first = load<uint32_t>(list + NEXT);
    save<uint32_t>(load<uint32_t>(first + PREV) + NEXT, n);
    save<uint32_t>(n + PREV, load<uint32_t>(first + PREV));
    save<uint32_t>(first + PREV, n);
    save<uint32_t>(n + NEXT, first);
    save<uint32_t>(list + SIZE, load<uint32_t>(list + SIZE) + 1);
}

void sharedCopy(uint32_t n, uint32_t x) { copyShared(n + VALUE, x); }
void bankCopy(uint32_t n, uint32_t x) {
    save<uint32_t>(n + VALUE, load<uint32_t>(x));
    copyShared(n + VALUE + 4, x + 4);
    save<uint32_t>(n + VALUE, load<uint32_t>(x));
}

}  // namespace

RW_FUNCTION(0x45A7FC, "std::list<SafeBuffer*>::remove", remove<0x45A83F>);
RW_FUNCTION(0x45A83E, "std::list<SafeBuffer*>::erase(first, last)", eraseRange<noValue>);
RW_FUNCTION(0x45A8B8, "std::__list_imp<SafeBuffer*>::clear", clear<noValue>);
RW_FUNCTION(0x4D8640, "std::list<shared_ptr<PurchaseManager::Transaction>>::push_front(const&)", (pushFront<0x10, sharedCopy>));
RW_FUNCTION(0x4D8FE0, "std::list<shared_ptr<PurchaseManager::Transaction>>::push_back(const&)", (pushBack<0x10, sharedCopy>));
RW_FUNCTION(0x4D867A, "std::list<shared_ptr<PurchaseManager::Transaction>>::remove", remove<0x4D9BC3>);
RW_FUNCTION(0x4D86BC, "std::list<shared_ptr<PurchaseManager::Transaction>>::erase(pos)", eraseOne<sharedValue>);
RW_FUNCTION(0x4D9BC2, "std::list<shared_ptr<PurchaseManager::Transaction>>::erase(first, last)", eraseRange<sharedValue>);
RW_FUNCTION(0x4D9210, "std::__list_imp<shared_ptr<PurchaseManager::Transaction>>::clear", clear<sharedValue>);
RW_FUNCTION(0x5BF060, "std::list<audio::Manager::Bank>::push_back(const&)", (pushBack<0x14, bankCopy>));
RW_FUNCTION(0x5BF58C, "std::list<audio::Manager::Bank>::erase(pos)", eraseOne<bankValue>);
RW_FUNCTION(0x5C0AC6, "std::__list_imp<audio::Manager::Bank>::clear", clear<bankValue>);
RW_FUNCTION(0x5CBD6C, "std::__list_imp<unsigned long long>::clear", clear<noValue>);
RW_FUNCTION(0x6F5886, "std::__list_imp<int>::clear", clear<noValue>);
RW_FUNCTION(0x7EF884, "std::__list_imp<cocos2d::LWFNode::ClipInfo>::clear", clear<noValue>);
RW_FUNCTION(0x7F0F1A, "std::__list_imp<string>::clear", clear<stringValue>);
RW_FUNCTION(0x7F0FCE, "std::__list_imp<unsigned int>::clear", clear<noValue>);
