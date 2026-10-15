// UserSupport: the support (friend / helper) player picked for a quest (response["supportUser"]), kept in a
// SafeBuffer.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserSupport : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F4E4;     // UserSupport vtable (+8)
    static constexpr uint32_t SIZE = 0x70;           // SafeBuffer data bytes

    // data positions
    static constexpr uint32_t SUPPORT_USER_ID = 0;         // uint64
    static constexpr uint32_t LEVEL = 8;                   // int
    static constexpr uint32_t PARTY_ID = 16;               // uint64
    static constexpr uint32_t USER_NAME = 24;              // char[33], NUL-terminated
    static constexpr uint32_t USER_NAME_SIZE = 33;
    static constexpr uint32_t TITLE_LEFT_ID = 60;          // int
    static constexpr uint32_t TITLE_RIGHT_ID = 64;         // int
    static constexpr uint32_t TITLE_PLATE_ID = 68;         // int
    static constexpr uint32_t USER_KEYBLADE_ID = 72;       // uint64
    static constexpr uint32_t KEYBLADE_ID = 80;            // int
    static constexpr uint32_t USER_MEDAL_ID = 88;          // uint64
    static constexpr uint32_t MEDAL_ID = 96;               // int
    static constexpr uint32_t LAST_ACTION_DATETIME = 100;  // Date (8 bytes)

    // response["supportUser"] -> *out; empty pointer (shared_ptr(nullptr)) when absent or incomplete
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static uint32_t destroyDelete(UserSupport *self);   // ~UserSupport + operator delete (D0)
};
RW_SIZE(UserSupport, 32);

}  // namespace hole::network::api
