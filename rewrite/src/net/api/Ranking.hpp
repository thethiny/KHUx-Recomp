// Ranking responses: the Lux ranking (players), the party ranking, the colosseum ranking, the union ranking
// (unions + their top players) and the ranking rewards.
#pragma once

#include "Colosseum.hpp"
#include "Party.hpp"
#include "UserMedal.hpp"

namespace hole::network::api {

// The three lists every ranked player carries (User / UserDetail / UserAvatar, parsed from the entry itself).
struct RankedUser {
    SharedPtr user;                           // +0  User (the entry's "userData" section)
    SharedPtr detail;                         // +8  UserDetail
    SharedPtr avatar;                         // +16 UserAvatar
};
RW_SIZE(RankedUser, 24);

struct LuxRanking {
    struct User : RankedUser {
        rw::Field<int32_t> lux;               // +24
        rw::Field<int32_t> rank;              // +28

        static uint32_t init(User *self, const JsonValue *json);
        static User *assign(User *self, const User *other);      // operator=(const User&)
    };

    rw::Field<int32_t> rankingStatus;         // +0
    const uint8_t rankingTermStart[8];              // +4  Date
    const uint8_t rankingTermEnd[8];                // +12 Date
    StdVector users;                          // +20 std::vector<User> ("rankingDatas")
    rw::Field<int32_t> lastRank;              // +32

    // json[key] (key: a guest C string)
    static uint32_t parse(SharedPtr *out, const JsonValue *json, uint32_t key);
};
RW_SIZE(LuxRanking::User, 32);
RW_LAYOUT(LuxRanking, users, 20);
RW_SIZE(LuxRanking, 0x24);

struct PartyRanking {
    struct Info {
        Party party;                          // +0   "party"
        rw::Field<int32_t> rank;              // +112
        rw::Field<int32_t> lux;               // +116

        static uint32_t init(Info *self, const JsonValue *json);
    };

    rw::Field<int32_t> rankingStatus;         // +0
    StdVector parties;                        // +4  std::vector<Info> ("rankingDatas")
    rw::Field<int32_t> lastRank;              // +16
    const uint8_t updateTime[8];                    // +20 Date
    const uint8_t nextUpdateTime[8];                // +28 Date

    static uint32_t parse(SharedPtr *out, const JsonValue *json, uint32_t key);
};
RW_SIZE(PartyRanking::Info, 0x78);
RW_SIZE(PartyRanking, 0x24);

struct ColosseumRanking {
    struct User : RankedUser {
        Colosseum::UserState state;           // +24 "userColosseum"
        const uint8_t arrivalTime[8];               // +36 Date
        rw::Field<int32_t> rank;              // +44

        static uint32_t init(User *self, const JsonValue *json);
        static User *assign(User *self, const User *other);      // operator=(const User&)
    };

    rw::Field<int32_t> rankingStatus;         // +0
    const uint8_t rankingTermStart[8];              // +4  Date
    const uint8_t rankingTermEnd[8];                // +12 Date
    StdVector users;                          // +20 std::vector<User> ("rankingDatas")

    static uint32_t parse(SharedPtr *out, const JsonValue *json, uint32_t key);
};
RW_LAYOUT(ColosseumRanking::User, arrivalTime, 36);
RW_SIZE(ColosseumRanking::User, 48);
RW_SIZE(ColosseumRanking, 0x20);

struct UnionRanking {
    struct Info {
        rw::Field<int32_t> unionId;           // +0
        rw::Field<int32_t> rank;              // +4
        rw::Field<uint64_t> lux;              // +8

        static uint32_t init(Info *self, const JsonValue *json);
    };
    struct Ranker {
        SharedPtr user;                       // +0  User
        SharedPtr avatar;                     // +8  UserAvatar
        UserKeyblade::Keyblade keyblade;      // +16 "userKeyblade"
        rw::Field<int32_t> rank;              // +104
        rw::Field<int32_t> lux;               // +108

        static uint32_t init(Ranker *self, const JsonValue *json);
    };

    StdVector unions;                         // +0  std::vector<Info> ("rankingUnionResult")
    StdVector rankers;                        // +12 std::vector<Ranker> ("rankingUnionTopRanker")

    // response["unionParade"] -> *out (made with make_shared: the object sits in its control block)
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(UnionRanking::Info, 16);
RW_LAYOUT(UnionRanking::Ranker, keyblade, 16);
RW_SIZE(UnionRanking::Ranker, 0x70);
RW_SIZE(UnionRanking, 24);

struct RankingReward {
    struct Result {
        rw::Field<int32_t> lastRankingId;     // +0
        rw::Field<int32_t> lastRank;          // +4
        rw::Field<int32_t> lastUnionRank;     // +8

        static uint32_t init(Result *self, const JsonValue *json);
    };

    StdVector results;                        // +0  std::vector<Result> ("lastRankingResults")
    StdVector rankingRewardIds;               // +12 std::vector<int>
    StdVector openRankingIds;                 // +24 std::vector<int>

    // response -> *out (made with make_shared)
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(RankingReward::Result, 12);
RW_SIZE(RankingReward, 36);

}  // namespace hole::network::api
