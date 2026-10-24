// Helpers shared by the wave-3 group-6 rewrites (src/w3_6): the scene / social helpers (guest loads and stores,
// std::string / std::function layouts, cocos2d slots) plus a few more library entry points.
#pragma once

#include "../scene_social/SocialCommon.hpp"

namespace w36 {
using namespace social;

inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
inline void st16(uint32_t a, uint16_t v) { rw::save<uint16_t>(a, v); }

namespace wfn {
constexpr uint32_t StringBuffer_fset = 0x491501;         // StringBuffer::fset(this, fmt, ...)
constexpr uint32_t LWFNode_create = rwlib::cocos2d::LWFNode::create;            // cocos2d::LWFNode::create(const char*, void*)
constexpr uint32_t LWFNODE_LWF = 0x3E0;                  // cocos2d::LWFNode::_lwf (LWF::LWF*)
constexpr uint32_t LWF_ROOT_MOVIE = 0x18;                // LWF::LWF::rootMovie
constexpr uint32_t Movie_GotoAndPlay = rwlib::LWF::Movie::GotoAndPlay__i;         // LWF::Movie::GotoAndPlay(int)
constexpr uint32_t Movie_GotoAndStop = rwlib::LWF::Movie::GotoAndStop;         // LWF::Movie::GotoAndStop(int)
constexpr uint32_t MOVIE_TOTAL_FRAMES = 0x98;            // LWF::Movie: total frames
constexpr uint32_t MOVIE_CURRENT_FRAME = 0x9C;           // LWF::Movie: current frame (0-based)
constexpr uint32_t LWFUtil_preloadResources = 0x491361;  // (const std::string&)
constexpr uint32_t HandlerMap_insert_unique = 0x530869;  // map<string,function<void(Movie*)>>::__insert_unique(hint, pair)
constexpr uint32_t HandlerMap_destroy = 0x530811;        // map<...>::destroy(node)
constexpr uint32_t LWF_AddMovieEventHandler = rwlib::LWF::LWF::AddMovieEventHandler;  // (lwf, const string& name, const map<string,function>&)
constexpr uint32_t Node_scheduleUpdate = rwlib::cocos2d::Node::scheduleUpdate;
constexpr uint32_t Node_unscheduleUpdate = rwlib::cocos2d::Node::unscheduleUpdate;
constexpr uint32_t STRLEN = 0x40E08C;                    // strlen (import)
}  // namespace wfn

// A StringBuffer (1 KB of chars + length) on the guest stack, empty.
inline uint32_t newStringBuffer(Frame &f) {
    uint32_t buf = f.alloc(STRING_BUFFER_SIZE);
    st32(buf + 1024, 0);
    st8(buf, 0);
    return buf;
}

// lwf->AddMovieEventHandler(name, {{event, lambda}}): the handler map is built from one stack pair
// <const string, function<void(Movie*)>> (40 bytes: key at +0, function at +16) and destroyed after (the function,
// then the key). `event` is a short string (at most 10 chars).
inline void addMovieHandler(uint32_t lwf, uint32_t name, const char *event, uint32_t vtable,
                            std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t pair = f.alloc(40);
    uint32_t n = uint32_t(std::strlen(event));
    st8(pair, uint8_t(n * 2));
    for (uint32_t k = 0; k < n; k++) st8(pair + 1 + k, uint8_t(event[k]));
    st8(pair + 1 + n, 0);
    uint32_t fn = pair + 16;
    st32(fn, vtable);
    uint32_t k = 4;
    for (uint32_t c : captures) { st32(fn + k, c); k += 4; }
    st32(fn + 16, fn);
    uint32_t map = f.alloc(12);                     // std::map: begin node, root (end node's left), size
    st32(map + 4, 0);
    st32(map + 8, 0);
    st32(map, map + 4);
    call(wfn::HandlerMap_insert_unique, {map, map + 4, pair});
    call(wfn::LWF_AddMovieEventHandler, {lwf, name, map});
    call(wfn::HandlerMap_destroy, {map, ld32(map + 4)});
    functionDestroy(fn);
    strFree(pair);
}
// the same with the instance name a literal (std::string built before and freed after)
inline void addMovieHandler(uint32_t lwf, uint32_t nameLit, uint32_t nameLen, const char *event, uint32_t vtable,
                            std::initializer_list<uint32_t> captures) {
    withStr(nameLit, nameLen, [&](uint32_t name) { addMovieHandler(lwf, name, event, vtable, captures); });
}

// self->field = lambda: std::function<void()>::operator=(const function&) from an inline stack temporary
inline void assignLambda(uint32_t field, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    withLambda(vtable, captures, [&](uint32_t fo) { call(sfn::function_vv_assign, {field, fo}); });
}

}  // namespace w36
