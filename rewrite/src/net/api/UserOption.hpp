// UserOption (the notification settings) and UserLink (the linked platform accounts: an unordered_map from the
// platform type to its id).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserOption {
    rw::Field<uint8_t> isNoticeAp;            // +0
    rw::Field<uint8_t> isNoticeHelp;          // +1
    rw::Field<uint8_t> isNoticeEvent;         // +2

    static uint32_t parse(SharedPtr *out, const JsonValue *response);   // response["userOption"]
};
RW_SIZE(UserOption, 3);

struct UserLink {
    struct Info {
        rw::Field<uint8_t> platformType;      // +0 (unsigned in the JSON)
        const uint8_t pad_1[3];
        StdString linkPlatformId;             // +4

        static uint32_t init(Info *self, const JsonValue *json);
    };

    const uint8_t byPlatform[20];                   // +0 std::unordered_map<PlatformType, Info>

    // response["linkInfos"] -> *out (make_shared; every entry inserted by its platform type)
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(UserLink::Info, 16);
RW_SIZE(UserLink, 20);

}  // namespace hole::network::api
