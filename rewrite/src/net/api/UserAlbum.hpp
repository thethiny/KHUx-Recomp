// UserAlbum: the medal album (which medals the player has ever owned), and UserAlbumChallenge: the album
// challenges (collect-N-medals goals) with their progress.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

// response["userAlbum"] = {"medalIds": [int...], "allMedalCount": int}. The medal ids live in a SafeBuffer
// (4 bytes each).
struct UserAlbum : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F500;     // UserAlbum vtable (+8)
    static constexpr uint32_t RECORD = 4;            // one medal id

    rw::Field<int32_t> allMedalCount;                // +32 medals in the game (album size)
    rw::Field<uint32_t> count;                       // +36 medal ids in the buffer

    // response["userAlbum"] -> *out; empty when absent, null when a medal id or allMedalCount is bad
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static uint32_t destroyDelete(UserAlbum *self);  // ~UserAlbum + operator delete (D0)
};
RW_LAYOUT(UserAlbum, allMedalCount, 32);
RW_LAYOUT(UserAlbum, count, 36);
RW_SIZE(UserAlbum, 40);

// std::set<T, bool (*)(const T&, const T&)> (libc++ __tree): begin node, the end node (its left child = the root),
// size, comparator. Nodes: left +0, right +4, parent +8, is_black +12, value +16.
struct StdPtrCompareSet {
    rw::Field<uint32_t> beginNode;   // +0 leftmost node (or &root when empty)
    rw::Field<uint32_t> root;        // +4 end node's left child
    rw::Field<uint32_t> size;        // +8
    rw::Field<uint32_t> compare;     // +12 bool (*)(const T&, const T&)
};
RW_SIZE(StdPtrCompareSet, 16);

// response["userAlbumChallenge"] = [{"challengeId", "count", "status"}...] in a std::set ordered by challengeId.
struct UserAlbumChallenge {
    struct Challenge {
        rw::Field<uint32_t> challengeId;   // +0 (the set's key)
        rw::Field<uint32_t> count;         // +4 progress
        rw::Field<uint8_t> status;         // +8
        const uint8_t pad_9[3];

        // all three as unsigned ints: 1, else 0 (fields set so far stay)
        static uint32_t init(Challenge *self, const JsonValue *json);
    };

    StdPtrCompareSet challenges;           // +0 std::set<Challenge, bool (*)(const Challenge&, const Challenge&)>

    static UserAlbumChallenge *construct(UserAlbumChallenge *self);   // UserAlbumChallenge() (C2)
    // response["userAlbumChallenge"] -> *out; null when it is not an array of complete challenges
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    // replace (erase + insert) each challenge of `updated` by its challengeId
    static uint32_t updateChallenges(UserAlbumChallenge *self, const StdPtrCompareSet *updated);
};
RW_LAYOUT(UserAlbumChallenge::Challenge, status, 8);
RW_SIZE(UserAlbumChallenge::Challenge, 12);
RW_SIZE(UserAlbumChallenge, 16);

}  // namespace hole::network::api
