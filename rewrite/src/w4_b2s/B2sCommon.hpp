// Helpers shared by the w4_b2s rewrites: guest loads / logged stores by address, calls that keep the caller's
// frame exactly where the compiled code had it (call_exact, used together with guest::Frame of the original
// frame size), libc++ std::string / std::function pieces that the compiler inlines.
#pragma once

#include <cstring>

#include "../core/Guest.hpp"
#include "rw/rw.hpp"

namespace b2s {

using Addr = uint32_t;     // a guest address

inline uint32_t ld32(Addr a) { return rw::load<uint32_t>(a); }
inline uint16_t ld16(Addr a) { return rw::load<uint16_t>(a); }
inline uint8_t ld8(Addr a) { return rw::load<uint8_t>(a); }
inline float ldf(Addr a) { return rw::load<float>(a); }
inline void st32(Addr a, uint32_t v) { rw::save<uint32_t>(a, v); }
inline void st16(Addr a, uint16_t v) { rw::save<uint16_t>(a, v); }
inline void st8(Addr a, uint8_t v) { rw::save<uint8_t>(a, v); }

using rw::bitsf;
using rw::fbits;

// A guest call from the (exactly reproduced) frame: sp is not touched and arguments 5+ go to [sp...]
inline uint32_t cx(Addr fn, std::initializer_list<uint32_t> args) { return uint32_t(rw::call_exact(fn, args)); }
inline uint64_t cx64(Addr fn, std::initializer_list<uint32_t> args) { return rw::call_exact(fn, args); }
// virtual call through a byte offset into the vtable
inline uint32_t vcx(Addr obj, uint32_t offset, std::initializer_list<uint32_t> args) {
    return cx(ld32(ld32(obj) + offset), args);
}

// ---- guest functions
constexpr Addr OPERATOR_NEW = 0xC98EF1;
constexpr Addr OPERATOR_DELETE = 0xC98F9B;
constexpr Addr STRING_INIT = 0x47CD93;             // std::string::__init(out, const char*, size_t)
constexpr Addr SHARED_ADD_REF = 0xC8DBB5;          // __shared_weak_count::__add_shared
constexpr Addr SHARED_RELEASE = 0xC8DBD1;          // __shared_weak_count::__release_shared
constexpr Addr AEABI_MEMSET = 0x40E7A0;            // __aeabi_memset(dst, n, c)
constexpr Addr MEMCPY = 0x40E164;
constexpr Addr STRLEN = 0x40E08C;
constexpr Addr SAFEBUFFER_READ = 0x45A269;         // SafeBuffer::read(sb, out, src, n, index)
constexpr Addr SEARCH_NODE_BY_NAME = 0x45A9F5;     // SceneUtil::searchNodeByName(root, const std::string&)
constexpr Addr SCENEMANAGER_GETINSTANCE = 0x60C26D;
constexpr Addr SCENEMANAGER_GETPOPUPLAYER = 0x613E69;
constexpr Addr APIMANAGER_GETINSTANCE = 0x4BA8C9;
constexpr Addr APPSTATE_GETINSTANCE = 0x4E66DD;
constexpr Addr FADD = 0xCD6BD0;                    // __aeabi_fadd
constexpr Addr FMUL = 0xCD6E14;                    // __mulsf3
constexpr Addr REF_AUTORELEASE = rwlib::cocos2d::Ref::autorelease;
constexpr Addr RUN_ACTION = rwlib::cocos2d::Node::runAction;              // cocos2d::Node::runAction

// ---- cocos2d::Node virtual slots (byte offsets)
namespace slot {
constexpr uint32_t getLocalZOrder = 0x18;
constexpr uint32_t setPosition = 0x4C, getPosition = 0x54, getPositionX = 0x68, getPositionY = 0x70;
constexpr uint32_t setAnchorPoint = 0x9C;
constexpr uint32_t getContentSize = 0xAC;
constexpr uint32_t setVisible = 0xB0;
constexpr uint32_t addChild = 0xF0, addChildZ = 0xF4, addChildZName = 0xFC;
constexpr uint32_t removeChild = 0x12C;
constexpr uint32_t setOpacity = 0x214;
}  // namespace slot

// ---- std::string (12 bytes) at a fixed guest address
inline void strInit(Addr s, Addr text, uint32_t len) { cx(STRING_INIT, {s, text, len}); }
// the inlined destructor: a long string (bit 0 of byte 0) frees its heap buffer
inline void strFree(Addr s) {
    if (ld8(s) & 1) cx(OPERATOR_DELETE, {ld32(s + 8)});
}
// SceneUtil::searchNodeByName(root, literal): the temporary std::string lives at `tmp` in the caller's frame
inline Addr findNode(Addr root, Addr tmp, Addr literal, uint32_t len) {
    strInit(tmp, literal, len);
    Addr node = cx(SEARCH_NODE_BY_NAME, {root, tmp});
    strFree(tmp);
    return node;
}

// ---- std::function<...> (16 byte buffer + callable pointer at +16) destroyed the way the compiler inlines it
inline void fnDestroy(Addr f) {
    Addr p = ld32(f + 16);
    if (p == f) vcx(p, 0x10, {p});         // stored in the buffer: destroy()
    else if (p) vcx(p, 0x14, {p});         // on the heap: destroy_deallocate()
}

inline void sharedRelease(Addr ctrl) { if (ctrl) cx(SHARED_RELEASE, {ctrl}); }
inline void sharedAddRef(Addr ctrl) { if (ctrl) cx(SHARED_ADD_REF, {ctrl}); }

}  // namespace b2s
