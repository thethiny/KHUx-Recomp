// A lambda's by-value captures as libc++ copies, moves and destroys them inline: words, a halfword, std::strings,
// chat messages, shared_ptrs and std::functions at fixed offsets. The requests and dialogs that hand their captures
// on to an answer lambda copy them to the stack, move them into the heap __func and destroy the copies after.
#pragma once

#include "ChatMessage.hpp"
#include "SceneCommon.hpp"

#include <initializer_list>
#include <iterator>

namespace scene {
namespace caps {

enum class Cap : uint8_t { Words, Half, Str, Msg, Shared, Fn };
struct Field {
    Cap kind;
    uint32_t off, bytes = 0;
    uint32_t copyFn = fn::function_vv_copy, moveFn = fn::function_vv_move;   // Fn: its signature's constructors
};
using Layout = std::initializer_list<Field>;

inline void copy(Layout layout, uint32_t dst, uint32_t src) {
    for (const Field &c : layout) {
        const uint32_t d = dst + c.off, s = src + c.off;
        switch (c.kind) {
        case Cap::Words: copyWords(d, s, c.bytes); break;
        case Cap::Half: rw::save<uint16_t>(d, rw::load<uint16_t>(s)); break;
        case Cap::Str: strCopy(d, s); break;
        case Cap::Msg: chatmsg::msgCopy(d, s); break;
        case Cap::Shared:
            st32(d, ld32(s));
            st32(d + 4, ld32(s + 4));
            if (ld32(d + 4)) call(fn::shared_add, {ld32(d + 4)});
            break;
        case Cap::Fn: call(c.copyFn, {d, s}); break;
        }
    }
}
inline void move(Layout layout, uint32_t dst, uint32_t src) {
    for (const Field &c : layout) {
        const uint32_t d = dst + c.off, s = src + c.off;
        switch (c.kind) {
        case Cap::Words: copyWords(d, s, c.bytes); break;
        case Cap::Half: rw::save<uint16_t>(d, rw::load<uint16_t>(s)); break;
        case Cap::Str: strMove(d, s); break;
        case Cap::Msg: chatmsg::msgMove(d, s); break;
        case Cap::Shared:
            st32(d, ld32(s));
            st32(d + 4, ld32(s + 4));
            st32(s, 0);
            st32(s + 4, 0);
            break;
        case Cap::Fn: call(c.moveFn, {d, s}); break;
        }
    }
}
inline void destroy(Layout layout, uint32_t p) {
    for (auto it = std::rbegin(layout); it != std::rend(layout); ++it) {
        const uint32_t a = p + it->off;
        switch (it->kind) {
        case Cap::Str: strFree(a); break;
        case Cap::Msg: chatmsg::msgDestroy(a); break;
        case Cap::Shared:
            if (ld32(a + 4)) call(fn::shared_release, {ld32(a + 4)});
            break;
        case Cap::Fn: functionDestroy(a); break;
        default: break;
        }
    }
}
// the captures at `src` copied to the stack and moved into a heap __func `vt` (at +8, `size` bytes), handed to
// `send(answer)`; the answer and the copies destroyed after. Returns what `send` returned.
template <class Send>
uint32_t sendWith(Layout layout, uint32_t size, uint32_t vt, uint32_t src, Send send) {
    Frame fr;
    const uint32_t tmp = fr.alloc(size);
    copy(layout, tmp, src);
    const uint32_t answer = fr.alloc(24);
    st32(answer + 16, 0);
    const uint32_t h = call(fn::operator_new, {8 + size});
    if (h) {
        st32(h, vt);
        move(layout, h + 8, tmp);
    }
    st32(answer + 16, h);
    const uint32_t r = send(answer);
    functionDestroy(answer);
    destroy(layout, tmp);
    return r;
}

}  // namespace caps
}  // namespace scene
