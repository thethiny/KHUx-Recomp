// ChatScrollView::ChatMessage (0x40 bytes): the ids, a flag, the poster's name, raw info, the text and a second
// flag; its copy / move constructors and destructor as libc++ inlines them (the chat popups pass it by value).
#pragma once

#include "SceneCommon.hpp"

namespace scene {
namespace chatmsg {

constexpr uint32_t MSG_HEAD = 0x00;       // 16 bytes (ids)
constexpr uint32_t MSG_FLAG = 0x10;       // uint8
constexpr uint32_t MSG_NAME = 0x14;       // std::string
constexpr uint32_t MSG_INFO = 0x20;       // 12 bytes, copied raw
constexpr uint32_t MSG_TEXT = 0x2C;       // std::string
constexpr uint32_t MSG_FLAG2 = 0x38;      // uint8
constexpr uint32_t MSG_SIZE = 0x40;

inline void msgCopy(uint32_t dst, uint32_t src) {
    copyWords(dst + MSG_HEAD, src + MSG_HEAD, 16);
    st8(dst + MSG_FLAG, ld8(src + MSG_FLAG));
    strCopy(dst + MSG_NAME, src + MSG_NAME);
    copyWords(dst + MSG_INFO, src + MSG_INFO, 12);
    strCopy(dst + MSG_TEXT, src + MSG_TEXT);
    st8(dst + MSG_FLAG2, ld8(src + MSG_FLAG2));
}
inline void msgMove(uint32_t dst, uint32_t src) {
    copyWords(dst + MSG_HEAD, src + MSG_HEAD, 16);
    st8(dst + MSG_FLAG, ld8(src + MSG_FLAG));
    strMove(dst + MSG_NAME, src + MSG_NAME);
    copyWords(dst + MSG_INFO, src + MSG_INFO, 12);
    strMove(dst + MSG_TEXT, src + MSG_TEXT);
    st8(dst + MSG_FLAG2, ld8(src + MSG_FLAG2));
}
inline void msgDestroy(uint32_t m) {
    strFree(m + MSG_TEXT);
    strFree(m + MSG_NAME);
}

}  // namespace chatmsg
}  // namespace scene
