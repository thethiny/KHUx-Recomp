// Party (a union party record) and the small party responses: UserParty (the player's membership + party),
// PartyCreate, PartyLogin, Ranking (lux + rank).
#pragma once

#include "StageSupportList.hpp"

namespace hole::network::api {

// libc++ string helpers shared by the party family.
namespace party {
constexpr uint32_t string_init = 0x47CD93;          // std::string::__init(const char*, size_t) (copy of a long string)
constexpr uint32_t string_shrink = 0x48D705;        // std::string::reserve(0)
constexpr uint32_t release_shared = 0xC8DBD1;       // std::__shared_weak_count::__release_shared()

// copy-construct a string member in place (long: a fresh copy; short: the three words)
inline void copyString(StdString *dst, const StdString *src) {
    if (src->isLong()) {
        rw::call(string_init, {rw::guest_addr(dst), src->w2, src->w1});
    } else {
        dst->w0 = src->w0.get();
        dst->w1 = src->w1.get();
        dst->w2 = src->w2.get();
    }
}
// string move assignment as inlined: clear dst, shrink it, take src's three words, leave src empty
inline void moveString(StdString *dst, StdString *src) {
    if (dst->isLong()) {
        rw::store8(rw::host_ptr<uint8_t>(dst->w2), 0);
        dst->w1 = 0;
    } else {
        rw::store8(reinterpret_cast<uint8_t *>(dst) + 1, 0);
        rw::store8(reinterpret_cast<uint8_t *>(dst), 0);
    }
    rw::call(string_shrink, {rw::guest_addr(dst), 0});
    dst->w0 = src->w0.get();
    dst->w1 = src->w1.get();
    dst->w2 = src->w2.get();
    src->w0 = 0;
    src->w1 = 0;
    src->w2 = 0;
}
// ~std::string: free a long string's buffer
inline void freeString(const StdString *s) {
    if (s->isLong()) guestDelete(s->w2);
}
inline void copyWords(uint32_t dst, uint32_t src, uint32_t n) {
    for (uint32_t k = 0; k < n; k++) rw::save<uint32_t>(dst + 4 * k, rw::load<uint32_t>(src + 4 * k));
}
}  // namespace party

struct Party {
    rw::Field<uint64_t> partyId;              // +0
    rw::Field<int32_t> unionId;               // +8
    rw::Field<int32_t> rank;                  // +12
    StdString name;                           // +16
    rw::Field<int32_t> playStyle;             // +28
    rw::Field<int32_t> agreeType;             // +32
    StdString message;                        // +36
    rw::Field<int32_t> memberCount;           // +48
    const uint8_t pad_52[4];
    rw::Field<uint64_t> leaderUserId;         // +56
    StdVector adminUserIds;                   // +64 std::vector<unsigned long long>
    const uint8_t leaderAppointDate[8];             // +76 Date
    const uint8_t newcomerDate[8];                  // +84 Date
    rw::Field<uint32_t> chatId;               // +92 (unsigned in the JSON)
    StdString chatEndpointUrl;                // +96
    const uint8_t pad_108[4];

    static uint32_t init(Party *self, const JsonValue *json);
    static Party *copyConstruct(Party *self, const Party *other);   // Party(const Party&)
    static Party *moveAssign(Party *self, Party *other);            // operator=(Party&&)
};
RW_LAYOUT(Party, name, 16);
RW_LAYOUT(Party, message, 36);
RW_LAYOUT(Party, leaderUserId, 56);
RW_LAYOUT(Party, adminUserIds, 64);
RW_LAYOUT(Party, leaderAppointDate, 76);
RW_LAYOUT(Party, chatId, 92);
RW_LAYOUT(Party, chatEndpointUrl, 96);
RW_SIZE(Party, 0x70);

// The player's party membership (SafeBuffer) and the party itself.
struct UserParty : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F43C;
    static constexpr uint32_t USER_ID = 0;            // u64
    static constexpr uint32_t PARTY_ID = 8;           // u64 (0: no party; "party" is then not read)
    static constexpr uint32_t IS_JOIN = 16;           // bools (1 byte each)
    static constexpr uint32_t IS_LEADER = 17;
    static constexpr uint32_t IS_ADMIN = 18;
    static constexpr uint32_t FLAG_FIRST_REWARD = 19;
    static constexpr uint32_t JOIN_DATE = 20;         // Date

    Party party;                                      // +32

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static UserParty *destroy(UserParty *self);       // D2
    static uint32_t destroyDelete(UserParty *self);   // D0
};
RW_LAYOUT(UserParty, party, 32);
RW_SIZE(UserParty, 0x90);

struct PartyCreate {
    rw::Field<uint8_t> firstReward;           // +0

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};

struct PartyLogin {
    rw::Field<uint8_t> isAbsenceLeader;       // +0
    const uint8_t pad_1[7];
    StageSupportList::SupportUser partyLeader;   // +8

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(PartyLogin, partyLeader, 8);
RW_SIZE(PartyLogin, 0x130);

namespace party {
// A Party local before init (as the parsers inline it): empty strings and admin list, Date flag bit 1 cleared.
inline void resetLocal(Party *p) {
    p->name.clear();
    p->message.clear();
    p->adminUserIds.begin = 0;
    p->adminUserIds.end = 0;
    p->adminUserIds.capEnd = 0;
    rw::store8(p->leaderAppointDate + 7, uint8_t(p->leaderAppointDate[7] & 0xFD));
    rw::store8(p->newcomerDate + 7, uint8_t(p->newcomerDate[7] & 0xFD));
    p->chatEndpointUrl.clear();
}
// ~Party: chatEndpointUrl, the admin id vector, message, name
inline void destroy(Party *p) {
    freeString(&p->chatEndpointUrl);
    rw::call(0x468499, {rw::guest_addr(&p->adminUserIds)});   // __vector_base<unsigned long long>::~__vector_base
    freeString(&p->message);
    freeString(&p->name);
}
}  // namespace party

struct Ranking {
    rw::Field<uint64_t> lux;                  // +0
    rw::Field<int32_t> rank;                  // +8

    static uint32_t init(Ranking *self, const JsonValue *json);
};

}  // namespace hole::network::api
