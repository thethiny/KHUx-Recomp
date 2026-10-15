// UserMedalEnhance / UserMedalEvolve: the results of the medal enhance (fusion) and evolve actions: the medal
// before and after, plus for enhance the effect, the skills that were overwritten and whether that succeeded.
// The medals are UserMedal::Medal records (72 bytes, filled by UserMedal::Medal::init), the skills
// UserSkill::Skill records (40 bytes, UserSkill::Skill::init).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

constexpr uint32_t USER_MEDAL_RECORD = 72;     // sizeof(UserMedal::Medal)
constexpr uint32_t USER_SKILL_RECORD = 40;     // sizeof(UserSkill::Skill)

struct UserMedalEnhance {
    const uint8_t beforeMedal[USER_MEDAL_RECORD];     // +0    UserMedal::Medal
    const uint8_t afterMedal[USER_MEDAL_RECORD];      // +72   UserMedal::Medal
    rw::Field<int32_t> enhanceEffect;           // +144
    StdVector overwriteSkills;                  // +148  std::vector<UserSkill::Skill>
    rw::Field<uint8_t> successOverwrite;        // +160  bool
    const uint8_t pad_161[7];

    // {"beforeEnhanceUserMedal", "afterEnhanceUserMedal", "enhanceEffect", "overwriteSkills",
    //  "successOverwrite"} -> *out; empty when the response is not an object, null when anything is bad
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(UserMedalEnhance, afterMedal, 72);
RW_LAYOUT(UserMedalEnhance, enhanceEffect, 144);
RW_LAYOUT(UserMedalEnhance, overwriteSkills, 148);
RW_LAYOUT(UserMedalEnhance, successOverwrite, 160);
RW_SIZE(UserMedalEnhance, 168);

struct UserMedalEvolve {
    const uint8_t beforeMedal[USER_MEDAL_RECORD];     // +0    UserMedal::Medal
    const uint8_t afterMedal[USER_MEDAL_RECORD];      // +72   UserMedal::Medal
    const uint8_t pad_144[8];                         // not touched by parse

    // {"beforeEvolveUserMedal", "afterEvolveUserMedal"} -> *out; null when either is bad
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(UserMedalEvolve, afterMedal, 72);
RW_SIZE(UserMedalEvolve, 152);

}  // namespace hole::network::api
