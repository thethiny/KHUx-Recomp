// UserStamp (the chat stamps the player owns: a SafeBuffer of stamp ids) and UserChat (the chat account: tokens,
// endpoint, the stamps).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserStamp : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F3B0;

    rw::Field<int32_t> count;                 // +32 stamp ids (4 bytes each in the SafeBuffer)

    static uint32_t parse(SharedPtr *out, const JsonValue *json);     // json["stampIds"]
    static uint32_t destroyDelete(UserStamp *self);                  // D0
};
RW_SIZE(UserStamp, 0x24);

struct UserChat {
    rw::Field<uint64_t> userId;               // +0
    StdString authToken;                      // +8   response["authToken"]
    StdString userToken;                      // +20  response["userToken"]
    StdString tagName;                        // +32
    StdString userEndpointUrl;                // +44
    SharedPtr stamps;                         // +56  UserStamp (from userData.userChat)

    static uint32_t parse(SharedPtr *out, const JsonValue *response);   // response["userData"]["userChat"]
};
RW_LAYOUT(UserChat, authToken, 8);
RW_LAYOUT(UserChat, stamps, 56);
RW_SIZE(UserChat, 0x40);

}  // namespace hole::network::api
