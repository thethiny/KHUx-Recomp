// PartyMemberList: the members of a party. A StageSupportList (one SupportUser record per member, indexed by user
// id) plus a vector of per-member details (keyblade, lux ranking, comment...).
#pragma once

#include "Party.hpp"
#include "UserMedal.hpp"

namespace hole::network::api {

struct PartyMemberList : StageSupportList {
    static constexpr uint32_t VTABLE = 0xF7F404;

    struct Detail {
        UserKeyblade::Keyblade keyblade;      // +0   "userKeyblade"
        Ranking ranking;                      // +88  "userRanking"
        const uint8_t pad_104[16];
        rw::Field<int32_t> lastStageId;       // +120
        StdString comment;                    // +124
        StdString linkPlatformId;             // +136
        const uint8_t pad_148[4];

        static uint32_t init(Detail *self, const JsonValue *json);
    };

    rw::Field<uint8_t> complete;              // +56 1 when every detail parsed (the list is kept either way)
    const uint8_t pad_57[3];
    StdVector details;                        // +60 std::vector<Detail>

    // json[key] (key: a guest C string, null = "partyUserList"): the members (both passes read the same array)
    static uint32_t parse(SharedPtr *out, const JsonValue *json, uint32_t key);
    static PartyMemberList *destroy(PartyMemberList *self);       // D2
    static uint32_t destroyDelete(PartyMemberList *self);         // D0
};
RW_LAYOUT(PartyMemberList::Detail, ranking, 88);
RW_LAYOUT(PartyMemberList::Detail, lastStageId, 120);
RW_LAYOUT(PartyMemberList::Detail, comment, 124);
RW_LAYOUT(PartyMemberList::Detail, linkPlatformId, 136);
RW_SIZE(PartyMemberList::Detail, 152);
RW_LAYOUT(PartyMemberList, complete, 56);
RW_LAYOUT(PartyMemberList, details, 60);
RW_SIZE(PartyMemberList, 0x48);

}  // namespace hole::network::api
