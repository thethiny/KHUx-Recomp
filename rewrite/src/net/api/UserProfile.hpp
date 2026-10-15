// UserProfile: another player's profile page (their user, detail, points, medals, skills, avatar, keyblade,
// play record, ranking and party), and Record (the play statistics: 53 unsigned 64-bit counters).
#pragma once

#include "Party.hpp"
#include "UserMedal.hpp"

namespace hole::network::api {

struct Record {
    static constexpr uint32_t COUNT = 53;
    // login, continueLogin, getMoney, ... colosseumRankingHighest (see Record.cpp for the order)
    rw::Field<uint64_t> values[COUNT];        // +0

    // every counter must be an unsigned 64-bit number: 1, else 0 (counters set so far stay)
    static uint32_t init(Record *self, const JsonValue *json);
};
RW_SIZE(Record, 424);

struct UserProfile {
    SharedPtr user;                           // +0   User ("userData" section)
    SharedPtr detail;                         // +8   UserDetail
    SharedPtr point;                          // +16  UserPoint
    SharedPtr medals;                         // +24  UserMedal
    SharedPtr skills;                         // +32  UserSkill
    SharedPtr avatar;                         // +40  UserAvatar
    UserKeyblade::Keyblade keyblade;          // +48  "userKeyblade"
    Record record;                            // +136 "userRecord"
    Ranking ranking;                          // +560 "userRanking"
    const uint8_t pad_576[16];
    Party party;                              // +592 "party" (only when the detail has a partyId)
    const uint8_t lastActionDatetime[8];            // +704 userData.lastActionDatetime (Date)
    StdString linkPlatformId;                 // +712

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static UserProfile *destroy(UserProfile *self);   // D2
};
RW_LAYOUT(UserProfile, keyblade, 48);
RW_LAYOUT(UserProfile, record, 136);
RW_LAYOUT(UserProfile, ranking, 560);
RW_LAYOUT(UserProfile, party, 592);
RW_LAYOUT(UserProfile, lastActionDatetime, 704);
RW_LAYOUT(UserProfile, linkPlatformId, 712);
RW_SIZE(UserProfile, 0x2D8);

}  // namespace hole::network::api
