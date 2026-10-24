// The fixed shapes of a libc++ std::function __func<Lambda> vtable (vtable + 8 onwards): +0 ~__func (shared),
// +4 deleting destructor, +8 __clone() (heap copy), +0xC __clone(dst) (placement copy), +0x10 destroy(),
// +0x14 destroy_deallocate(), +0x18 operator(), +0x1C target(type_info), +0x20 target_type().
// A lambda capturing n words is a __func of 4 + 4n bytes: vtable, then the captures.
// Used by the battleui lead's class files (RaidBoss, PlayerStatusIndicator, SceneRaidList, RaidBossLwfDialog).
#pragma once

#include "BattleUi.hpp"

namespace bui {
namespace thunk {

// deleting destructor / destroy_deallocate: operator delete(this)
inline void deleteSelf(uint32_t self) { rw::call(guest::OPERATOR_DELETE, {self}); }

// __clone(): new __func (`size` bytes) with the same `words` captures (no copy when the allocation failed)
inline uint32_t cloneNew(uint32_t self, uint32_t vtable, uint32_t size, uint32_t words) {
    uint32_t p = rw::call(guest::OPERATOR_NEW, {size});
    if (p) {
        rw::save<uint32_t>(p, vtable);
        for (uint32_t k = 1; k <= words; k++) rw::save<uint32_t>(p + 4 * k, rw::load<uint32_t>(self + 4 * k));
    }
    return p;
}

// __clone(dst): placement copy (the compiled code checks dst for null)
inline void cloneInto(uint32_t self, uint32_t dst, uint32_t vtable, uint32_t words) {
    if (!dst) return;
    rw::save<uint32_t>(dst, vtable);
    for (uint32_t k = 1; k <= words; k++) rw::save<uint32_t>(dst + 4 * k, rw::load<uint32_t>(self + 4 * k));
}

// target(type_info): the functor (this + 4) when the type_info's name pointer is the lambda's, else null
inline uint32_t target(uint32_t self, uint32_t typeInfo, uint32_t typeName) {
    return rw::load<uint32_t>(typeInfo + 4) == typeName ? self + 4 : 0;
}

}  // namespace thunk

// ------------------------------------------------------------------------------------------- LWF helpers
namespace lwf {
constexpr uint32_t HandlerMap_insert_unique = 0x530869;  // map<string,function<void(Movie*)>>::__insert_unique(hint, pair)
constexpr uint32_t HandlerMap_destroy = 0x530811;        // map<...>::destroy(node)
constexpr uint32_t LWF_AddMovieEventHandler = rwlib::LWF::LWF::AddMovieEventHandler;  // (lwf, const string& name, const map<string,function>&)
constexpr uint32_t STRING_BUFFER_SIZE = 1028;            // StringBuffer: char[1024] + length

// A temporary StringBuffer on the guest stack (empty)
inline uint32_t newStringBuffer(scene::Frame &f) {
    uint32_t buf = f.alloc(STRING_BUFFER_SIZE);
    scene::st32(buf + 1024, 0);
    scene::st8(buf, 0);
    return buf;
}

// lwf->AddMovieEventHandler(name, {{"load", lambda}}): the handler map is built from one stack pair
// <const string, function<void(Movie*)>> (40 bytes: key at +0, function at +16) and destroyed after.
inline void addLoadHandler(uint32_t lwf, uint32_t name, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    using namespace scene;
    Frame f;
    uint32_t pair = f.alloc(40);
    st8(pair, 8);                                   // short string "load"
    st8(pair + 1, 'l'); st8(pair + 2, 'o'); st8(pair + 3, 'a'); st8(pair + 4, 'd'); st8(pair + 5, 0);
    uint32_t fn = pair + 16;
    st32(fn, vtable);
    uint32_t k = 4;
    for (uint32_t c : captures) { st32(fn + k, c); k += 4; }
    st32(fn + 16, fn);
    uint32_t map = f.alloc(12);                     // std::map: begin node, root (end node's left), size
    st32(map + 4, 0);
    st32(map + 8, 0);
    st32(map, map + 4);
    call(HandlerMap_insert_unique, {map, map + 4, pair});
    call(LWF_AddMovieEventHandler, {lwf, name, map});
    call(HandlerMap_destroy, {map, ld32(map + 4)});
    functionDestroy(fn);
    strFree(pair);
}
}  // namespace lwf
}  // namespace bui
