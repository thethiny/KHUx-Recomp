// StageSupportList: the support (friend / helper) players offered for a quest, one 296-byte SupportUser record per
// entry in a SafeBuffer, indexed by supportUserId (the UserRecordList shape). PartyMemberList derives from it.
#pragma once

#include "UserRecordList.hpp"

namespace hole::network::api {

struct StageSupportList : UserRecordList {
    static constexpr uint32_t VTABLE = 0xF7F42C;

    struct SupportUser {
        rw::Field<uint64_t> supportUserId;    // +0
        rw::Field<int32_t> level;             // +8
        const uint8_t userName[33];                 // +12 (strncpy, then +44 = 0)
        const uint8_t pad_45[3];
        rw::Field<int32_t> titleLeftId;       // +48
        rw::Field<int32_t> titleRightId;      // +52
        rw::Field<int32_t> titlePlateId;      // +56
        rw::Field<int32_t> addKizunaPoint;    // +60
        rw::Field<int32_t> keybladeId;        // +64
        rw::Field<uint8_t> isParty;           // +68
        rw::Field<uint8_t> isGuilt;           // +69
        const uint8_t pad_70[2];
        const uint8_t medal[72];                    // +72  UserMedal::Medal (from "userMedal")
        rw::Field<uint32_t> skillCount;       // +144
        const uint8_t pad_148[4];
        uint8_t skills[2][40];                // +152 UserSkill::Skill (every skill is copied: more than 2 overrun)
        const uint8_t avatar[48];                   // +232 UserAvatar::Coordinate
        const uint8_t lastActionDatetime[8];        // +280 Date
        rw::Field<int32_t> earnLuxRank;       // +288 (optional: not required for success)
        const uint8_t pad_292[4];

        // 1 once lastActionDatetime parsed (earnLuxRank is read when present), else 0
        static uint32_t init(SupportUser *self, const JsonValue *json);
    };

    // json[key] (an array; key is a guest C string) -> *out
    static uint32_t parse(SharedPtr *out, const JsonValue *json, uint32_t key);
    static StageSupportList *destroy(StageSupportList *self);      // D2
    static uint32_t destroyDelete(StageSupportList *self);         // D0
};
RW_LAYOUT(StageSupportList::SupportUser, titleLeftId, 48);
RW_LAYOUT(StageSupportList::SupportUser, isParty, 68);
RW_LAYOUT(StageSupportList::SupportUser, medal, 72);
RW_LAYOUT(StageSupportList::SupportUser, skillCount, 144);
RW_LAYOUT(StageSupportList::SupportUser, skills, 152);
RW_LAYOUT(StageSupportList::SupportUser, avatar, 232);
RW_LAYOUT(StageSupportList::SupportUser, lastActionDatetime, 280);
RW_LAYOUT(StageSupportList::SupportUser, earnLuxRank, 288);
RW_SIZE(StageSupportList::SupportUser, 296);
RW_SIZE(StageSupportList, 56);

}  // namespace hole::network::api
