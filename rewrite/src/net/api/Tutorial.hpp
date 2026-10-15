// Tutorial responses: the tutorial progress (TutorialProgress), the tutorial phase (TutorialStatus) and the
// tutorial's end with an optional invite bonus (TutorialClear).
#pragma once

#include "Party.hpp"

namespace hole::network::api {

struct TutorialProgress {
    rw::Field<uint64_t> userTutorialId;       // +0
    rw::Field<int32_t> progression;           // +8
    StdString name;                           // +12
    rw::Field<int32_t> unionId;               // +24 (read from progression 4 on)
    const uint8_t avatar[48];                       // +28 UserAvatar::Coordinate (read from progression 5 on)
    StdString inviteCode;                     // +76

    static uint32_t parse(SharedPtr *out, const JsonValue *response);   // response["tutorial"]
};
RW_LAYOUT(TutorialProgress, name, 12);
RW_LAYOUT(TutorialProgress, avatar, 28);
RW_LAYOUT(TutorialProgress, inviteCode, 76);
RW_SIZE(TutorialProgress, 0x58);

struct TutorialStatus {
    rw::Field<uint32_t> phase;                // +0 (unsigned in the JSON)
    const uint8_t pad_4[4];
    rw::Field<uint64_t> popupFlag;            // +8
    rw::Field<uint8_t> isFinished;            // +16 (unsigned in the JSON)
    const uint8_t pad_17[7];

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(TutorialStatus, 0x18);

struct TutorialClear {
    // The invite bonus: the inviter, their party, the rewards. TutorialClear is just this.
    struct InviteBonus {
        SharedPtr user;                       // +0   User ("inviteBonus" section)
        Party party;                          // +8
        rw::Field<uint8_t> firstReward;       // +120
        const uint8_t pad_121[3];
        rw::Field<int32_t> inviteStone;       // +124

        static InviteBonus *construct(InviteBonus *self);
    };
    InviteBonus bonus;                        // +0

    // response["inviteBonus"]: absent (null) = no bonus, an object = parsed, anything else = null result
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(TutorialClear::InviteBonus, party, 8);
RW_LAYOUT(TutorialClear::InviteBonus, firstReward, 120);
RW_SIZE(TutorialClear::InviteBonus, 0x80);

}  // namespace hole::network::api
