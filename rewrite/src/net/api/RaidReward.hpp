// RaidReward / RaidReward151101: the rewards of finished raids ({"raidRewards": [...]}). The 151101 variant (a
// later server version) drops the discoverer / MVP users.
#pragma once

#include "Raid.hpp"

namespace hole::network::api {

// 32-bit ARM aligns 64-bit members to 8 but these records are laid out with 4-byte packing where it matters.
#pragma pack(push, 4)

// The part both reward records start with (148 bytes).
struct RaidRewardHead {
    rw::Field<uint64_t> raidId;               // +0
    rw::Field<int32_t> level;                 // +8
    rw::Field<int32_t> subdueId;              // +12
    rw::Field<int32_t> mvpId;                 // +16
    rw::Field<int32_t> partAllId;             // +20
    rw::Field<uint32_t> partCount;            // +24 min(parts.size, 6)
    RaidBase::Info::Part parts[6];            // +28 (every entry is copied: more than 6 overrun)
};
RW_LAYOUT(RaidRewardHead, parts, 28);
RW_SIZE(RaidRewardHead, 148);

struct RaidReward {
    struct Reward {
        RaidRewardHead head;                  // +0
        const uint8_t pad_148[4];
        RaidBase::User discoverer;            // +152 "discoverer"
        rw::Field<int32_t> mvpTotalDamage;    // +224
        const uint8_t pad_228[4];
        RaidBase::User mvp;                   // +232 "mvp"
        SharedPtr medals;                     // +304 UserMedal
        SharedPtr skills;                     // +312 UserSkill
        SharedPtr materials;                  // +320 UserMaterial

        // 1 when everything parsed and the material list is not null, else 0
        static uint32_t init(Reward *self, const JsonValue *json);
        static Reward *copyConstruct(Reward *self, const Reward *other);
    };

    StdVector rewards;                        // +0 std::vector<Reward>

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(RaidReward::Reward, discoverer, 152);
RW_LAYOUT(RaidReward::Reward, mvpTotalDamage, 224);
RW_LAYOUT(RaidReward::Reward, mvp, 232);
RW_LAYOUT(RaidReward::Reward, medals, 304);
RW_SIZE(RaidReward::Reward, 328);

struct RaidReward151101 {
    struct Reward {
        RaidRewardHead head;                  // +0
        rw::Field<int32_t> mvpTotalDamage;    // +148
        SharedPtr medals;                     // +152 UserMedal
        SharedPtr skills;                     // +160 UserSkill
        SharedPtr materials;                  // +168 UserMaterial

        static uint32_t init(Reward *self, const JsonValue *json);
    };

    StdVector rewards;                        // +0 std::vector<Reward>

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(RaidReward151101::Reward, medals, 152);
RW_SIZE(RaidReward151101::Reward, 176);

#pragma pack(pop)

}  // namespace hole::network::api
