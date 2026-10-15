// Party applications and invitations: a player applying to a party (Applicant) or a party inviting a player
// (Offer) share one 56-byte record; PartyEx is a party with its leader.
//   PartyApplication      one application result
//   PartyApplicationList  the player's applications + the parties applied to
//   PartyApplicantList    the applicants of the player's party + their support records
//   PartyInviteOffer      an invitation result
//   PartyInviteList       the invitations a party sent + the invited players' support records
//   PartyOfferList        the invitations the player received + the inviting parties
#pragma once

#include "Party.hpp"

namespace hole::network::api {

// An application / invitation record.
struct PartyRequest {
    rw::Field<uint64_t> userId;               // +0
    rw::Field<uint64_t> partyId;              // +8
    StdString message;                        // +16
    rw::Field<int32_t> status;                // +28
    const uint8_t date[8];                          // +32 applicationDate / offerDate
    const uint8_t agreeDate[8];                     // +40
    rw::Field<uint64_t> requestId;            // +48 partyApplicantId / partyOfferId
};
RW_LAYOUT(PartyRequest, message, 16);
RW_LAYOUT(PartyRequest, date, 32);
RW_LAYOUT(PartyRequest, requestId, 48);
RW_SIZE(PartyRequest, 56);

struct PartyApplication {
    struct Applicant : PartyRequest {
        static uint32_t init(Applicant *self, const JsonValue *json);
    };

    rw::Field<uint32_t> type;                 // +0 the caller's type (0: an application response)
    const uint8_t pad_4[4];
    Applicant applicant;                      // +8  "partyApplicant"
    rw::Field<uint8_t> firstReward;           // +64 (type 0 only)
    const uint8_t pad_65[3];
    rw::Field<uint32_t> failedStatus;         // +68 (type 0: non-zero = the application failed, nothing else read)

    static uint32_t parse(SharedPtr *out, const JsonValue *response, uint32_t type);
};
RW_LAYOUT(PartyApplication, applicant, 8);
RW_LAYOUT(PartyApplication, firstReward, 64);
RW_SIZE(PartyApplication, 0x48);

struct PartyApplicationList {
    // A party and its leader.
    struct PartyEx {
        Party party;                                  // +0   "party"
        StageSupportList::SupportUser leader;         // +112 "partyLeader"

        // the members are looked up on json itself, which is not type-checked; returns SupportUser::init's result
        static uint32_t init(PartyEx *self, const JsonValue *json);
    };

    StdVector applicants;                     // +0  std::vector<Applicant> ("partyApplicants")
    StdVector parties;                        // +12 std::vector<PartyEx> ("partyList")

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(PartyApplicationList::PartyEx, leader, 112);
RW_SIZE(PartyApplicationList::PartyEx, 0x198);
RW_SIZE(PartyApplicationList, 24);

struct PartyApplicantList {
    StdVector applicants;                     // +0  std::vector<Applicant> ("partyApplicants")
    SharedPtr supportList;                    // +12 StageSupportList ("partyUserList")

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(PartyApplicantList, 20);

struct PartyInviteOffer {
    rw::Field<uint32_t> failedStatus;         // +0

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};

struct PartyInviteList {
    struct Offer : PartyRequest {
        // the members are looked up on json, which is not type-checked
        static uint32_t init(Offer *self, const JsonValue *json);
    };

    StdVector offers;                         // +0  std::vector<Offer> ("partyOffers")
    SharedPtr supportList;                    // +12 StageSupportList ("partyUserList")

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    // update the offer with the same partyId: status and agreeDate. Returns the end pointer when there is none,
    // else the first halfword of the copied agreeDate (what the original leaves in r0).
    static uint32_t setOffer(PartyInviteList *self, const Offer *offer);
};
RW_SIZE(PartyInviteList, 20);

struct PartyOfferList {
    StdVector offers;                         // +0  std::vector<Offer> ("partyOffers")
    SharedPtr unused;                         // +12
    StdVector parties;                        // +20 std::vector<PartyEx> ("partyList")

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    // update the listed party with the same partyId: memberCount and newcomerDate. Returns the end pointer when
    // there is none, else the first halfword of the copied newcomerDate.
    static uint32_t setParty(PartyOfferList *self, const Party *party);
};
RW_LAYOUT(PartyOfferList, parties, 20);
RW_SIZE(PartyOfferList, 0x20);

}  // namespace hole::network::api
