// Raid responses: the boss (RaidBase::Info: id, level, timers, enemy parts), the players around it (discoverer, MVP,
// cooperators), and the shared pieces of the raid family (RaidReward, RaidStart / RaidClear include this header).
//
// The server builds these in handlers.py (raid list 107-111, raid start 112, raid clear 113).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

// ---------------------------------------------------------------------------------------------- raid family
namespace raid {
constexpr uint32_t Date_init = 0x4924F1;              // Date::init(const char*, bool)
constexpr uint32_t Coordinate_init = 0x49F6C5;        // UserAvatar::Coordinate::init(const GenericValue&)
constexpr uint32_t string_init = 0x47CD93;            // std::string::__init(const char*, size_t)
constexpr uint32_t add_shared = 0xC8DBB5;             // std::__shared_weak_count::__add_shared()
constexpr uint32_t release_shared = 0xC8DBD1;         // std::__shared_weak_count::__release_shared()
constexpr uint32_t aeabi_memset = 0x40E7A0;           // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t aeabi_memcpy = 0x40E164;           // __aeabi_memcpy(dst, src, n) (import)
constexpr uint32_t UserMedal_parse = 0x4A029D;        // UserMedal::parse(shared_ptr*, const GenericValue&)
constexpr uint32_t UserSkill_parse = 0x4A0781;        // UserSkill::parse
constexpr uint32_t UserMaterial_parse = 0x4A0A71;     // UserMaterial::parse

inline void releaseShared(uint32_t ctrl) { rw::call(release_shared, {ctrl}); }
inline void addShared(uint32_t ctrl) { rw::call(add_shared, {ctrl}); }

// `field = std::move(tmp)` as the parsers inline it: tmp is emptied, field takes its pointer and control block, the
// old control block is released (and then tmp's, which is always empty by then: the compiler's leftover check).
inline void moveShared(SharedPtr *field, SharedPtr *tmp) {
    uint32_t ptr = tmp->ptr, ctrl = tmp->ctrl;
    tmp->ptr = 0;
    tmp->ctrl = 0;
    field->ptr = ptr;
    uint32_t old = field->ctrl;
    field->ctrl = ctrl;
    if (old) {
        releaseShared(old);
        if (tmp->ctrl) releaseShared(tmp->ctrl);
    }
}

// Copy-construct a std::string member in place (long form: a fresh heap copy; short form: the three words).
inline void copyString(StdString *dst, const StdString *src) {
    if (src->isLong()) {
        rw::call(string_init, {rw::guest_addr(dst), src->w2, src->w1});
    } else {
        dst->w0 = src->w0;
        dst->w1 = src->w1;
        dst->w2 = src->w2;
    }
}
// ~std::string of a local: free the heap buffer of a long string.
inline void freeString(const StdString *s) {
    if (s->isLong()) guestDelete(s->w2);
}
// Copy n words from guest address src to guest address dst.
inline void copyWords(uint32_t dst, uint32_t src, uint32_t n) {
    for (uint32_t k = 0; k < n; k++) rw::save<uint32_t>(dst + 4 * k, rw::load<uint32_t>(src + 4 * k));
}
}  // namespace raid

// A Date (8 bytes, filled by Date::init from "YYYY-MM-DD hh:mm:ss"); byte 7 is a flag the owners clear first.
struct DateValue {
    const uint8_t bytes[8];
};
// UserAvatar::Coordinate (48 bytes; its own module fills it).
struct AvatarCoordinate {
    const uint8_t bytes[48];
};

struct RaidBase {
    // The raid boss.
    struct Info {
        // One enemy part ("parts" entries).
        struct Part {
            rw::Field<int32_t> raidEnemyId;   // +0
            rw::Field<int32_t> partsId;       // +4
            rw::Field<int32_t> hp;            // +8
            rw::Field<int32_t> attribute;     // +12
            rw::Field<int32_t> rare;          // +16

            // all five ints: 1, else 0 (fields set so far stay)
            static uint32_t init(Part *self, const JsonValue *json);
        };
        static constexpr uint32_t MAX_PARTS = 6;

        rw::Field<uint64_t> raidId;           // +0
        rw::Field<int32_t> level;             // +8
        rw::Field<int32_t> useAp;             // +12
        DateValue timeLeft;                   // +16
        rw::Field<uint8_t> feverFlag;         // +24
        const uint8_t pad_25[1];
        DateValue feverTime;                  // +26
        const uint8_t pad_34[2];
        rw::Field<int32_t> stageId;           // +36
        rw::Field<uint32_t> partCount;        // +40 min(parts.size, 6)
        Part parts[MAX_PARTS];                // +44 (the parser copies every entry: more than 6 overrun)

        static uint32_t init(Info *self, const JsonValue *json);
    };

    // A player shown with the raid (discoverer, MVP, cooperator).
    struct User {
        rw::Field<uint64_t> userId;           // +0
        StdString name;                       // +8
        rw::Field<int32_t> level;             // +20
        AvatarCoordinate avatar;              // +24 "userAvatar"

        // returns what UserAvatar::Coordinate::init returns once the other fields are in (0 before that)
        static uint32_t init(User *self, const JsonValue *json);
    };

    Info info;                                // +0  "raid"
    User discoverer;                          // +168 "discoverer"

    static uint32_t init(RaidBase *self, const JsonValue *json);
};
RW_LAYOUT(RaidBase::Info, timeLeft, 16);
RW_LAYOUT(RaidBase::Info, feverFlag, 24);
RW_LAYOUT(RaidBase::Info, feverTime, 26);
RW_LAYOUT(RaidBase::Info, stageId, 36);
RW_LAYOUT(RaidBase::Info, parts, 44);
RW_SIZE(RaidBase::Info::Part, 20);
RW_SIZE(RaidBase::Info, 168);
RW_LAYOUT(RaidBase::User, name, 8);
RW_LAYOUT(RaidBase::User, level, 20);
RW_LAYOUT(RaidBase::User, avatar, 24);
RW_SIZE(RaidBase::User, 72);
RW_LAYOUT(RaidBase, discoverer, 168);

// One raid (raid list / raid info): the boss, its discoverer and the MVP.
struct Raid {
    RaidBase::Info info;                      // +0   "raid"
    RaidBase::User discoverer;                // +168 "discoverer"
    rw::Field<int32_t> raidStatus;            // +240
    const uint8_t pad_244[4];
    RaidBase::User mvp;                       // +248 "mvp"

    // withUsers == 1: discoverer and mvp are required too
    static uint32_t init(Raid *self, const JsonValue *json, uint32_t withUsers);
    // response -> *out (Raid::init without the users; null when incomplete)
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(Raid, raidStatus, 240);
RW_LAYOUT(Raid, mvp, 248);
RW_SIZE(Raid, 0x140);

// Who fought a raid: the cooperators with their damage, the discoverer's and the MVP's user ids.
struct RaidMember {
    struct Cooperator {
        RaidBase::User user;                  // +0  "user"
        rw::Field<uint32_t> damage;           // +72 (unsigned)
        const uint8_t pad_76[4];

        static uint32_t init(Cooperator *self, const JsonValue *json);
    };

    StdVector cooperators;                    // +0  std::vector<Cooperator>
    const uint8_t pad_12[4];
    rw::Field<uint64_t> discovererUserId;     // +16
    rw::Field<uint64_t> mvpUserId;            // +24

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(RaidMember::Cooperator, damage, 72);
RW_SIZE(RaidMember::Cooperator, 80);
RW_LAYOUT(RaidMember, discovererUserId, 16);
RW_SIZE(RaidMember, 32);

}  // namespace hole::network::api
