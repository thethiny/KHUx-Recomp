// PartyNotice: the party notification flags and the users who joined recently.
#pragma once

#include "Party.hpp"

namespace hole::network::api {

struct PartyNotice {
    struct NewcomerUser {
        StdString userName;                   // +0
        const uint8_t pad_12[4];
        rw::Field<uint64_t> userId;           // +16
        const uint8_t joinDate[8];                  // +24 Date
        StdString message;                    // +32
        const uint8_t pad_44[4];

        static uint32_t init(NewcomerUser *self, const JsonValue *json);
    };

    rw::Field<uint8_t> hasNewOffer;           // +0
    rw::Field<uint8_t> isApply;               // +1
    rw::Field<uint8_t> hasNewApplicant;       // +2
    rw::Field<uint8_t> isNewLeader;           // +3
    StdVector newcomerUsers;                  // +4 std::vector<NewcomerUser>

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(PartyNotice::NewcomerUser, userId, 16);
RW_LAYOUT(PartyNotice::NewcomerUser, message, 32);
RW_SIZE(PartyNotice::NewcomerUser, 48);
RW_SIZE(PartyNotice, 16);

}  // namespace hole::network::api
