#include "PartyApplication.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t PARTY_USER_LIST = 0xDFA7C5;            // the "partyUserList" literal
constexpr uint32_t APPLICATION_SHARED_PTR_CTOR = 0x4A8625;    // std::shared_ptr<PartyApplication>::shared_ptr(...)
constexpr uint32_t APPLICATION_LIST_SHARED_PTR_CTOR = 0x4A8A8D;
constexpr uint32_t APPLICANT_LIST_SHARED_PTR_CTOR = 0x4A8D21;
constexpr uint32_t INVITE_OFFER_SHARED_PTR_CTOR = 0x4A8DE1;
constexpr uint32_t INVITE_LIST_SHARED_PTR_CTOR = 0x4A91C5;
constexpr uint32_t OFFER_LIST_SHARED_PTR_CTOR = 0x4A95FD;
constexpr uint32_t APPLICANT_PUSH_SLOW = 0x4B449D;        // vector<Applicant>::__push_back_slow_path(const&)
constexpr uint32_t APPLICANT_VECTOR_BASE_DTOR = 0x4B4453;
constexpr uint32_t OFFER_PUSH_SLOW = 0x4B41D9;            // vector<Offer>::__push_back_slow_path(const&)
constexpr uint32_t OFFER_VECTOR_BASE_DTOR = 0x4B3E5D;
constexpr uint32_t PARTY_EX_PUSH_SLOW = 0x4B3EA9;         // vector<PartyEx>::__push_back_slow_path(const&)
constexpr uint32_t PARTY_EX_VECTOR_BASE_DTOR = 0x4B3DD1;
constexpr uint32_t U64_VECTOR_BASE_DTOR = 0x468499;       // __vector_base<unsigned long long>::~__vector_base
using PartyEx = PartyApplicationList::PartyEx;

bool readId(const JsonValue &obj, const char *key, rw::Field<uint64_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsUint64()) return false;
    field = v.GetUint64();
    return true;
}

// The shared body of Applicant::init / Offer::init.
uint32_t initRequest(PartyRequest *self, const JsonValue &obj, const char *dateKey, const char *idKey) {
    if (!readId(obj, "userId", self->userId)) return 0;
    if (!readId(obj, "partyId", self->partyId)) return 0;
    const JsonValue &message = obj["message"];
    if (!message.IsString()) return 0;
    self->message.assign(message.GetString());
    const JsonValue &status = obj["status"];
    if (!status.IsInt()) return 0;
    self->status = status.GetInt();
    const JsonValue &date = obj[dateKey];
    if (!date.IsString()) return 0;
    if (rw::call(guest::Date_init, {rw::guest_addr(self->date), date.GetString(), 0}) != 1) return 0;
    const JsonValue &agree = obj["agreeDate"];
    if (!agree.IsString()) return 0;
    if (rw::call(guest::Date_init, {rw::guest_addr(self->agreeDate), agree.GetString(), 0}) != 1) return 0;
    return readId(obj, idKey, self->requestId) ? 1 : 0;
}

// The request local before init: an empty message, the Date flag bits cleared.
void resetRequest(PartyRequest *local) {
    local->message.clear();
    rw::store8(local->date + 7, uint8_t(local->date[7] & 0xFD));
    rw::store8(local->agreeDate + 7, uint8_t(local->agreeDate[7] & 0xFD));
}

// requests: json[key] (an array of objects), through the local, appended to vec. A bad entry stops the list.
template <class Request>
bool readRequests(StdVector *vec, const JsonValue &json, const char *key, Request *local, uint32_t pushSlow,
                  uint32_t (*init)(Request *, const JsonValue *)) {
    const JsonValue &list = json[key];
    if (!list.IsArray()) return false;
    uint32_t n = list.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &entry = list.At(k);
        if (!entry.IsObject()) return false;
        resetRequest(local);
        if (!init(local, &entry)) {
            party::freeString(&local->message);
            return false;
        }
        uint32_t end = vec->end;
        if (end == vec->capEnd) {
            rw::call(pushSlow, {rw::guest_addr(vec), rw::guest_addr(local)});
        } else {
            uint32_t base = 0;
            if (end) {
                party::copyWords(end, rw::guest_addr(local), 4);                       // userId, partyId
                party::copyString(&rw::host_ptr<PartyRequest>(end)->message, &local->message);
                party::copyWords(end + 28, rw::guest_addr(local) + 28, 7);             // status .. requestId
                base = vec->end;
            }
            vec->end = base + sizeof(PartyRequest);
        }
        party::freeString(&local->message);
    }
    return true;
}

// The PartyEx local before init: empty strings and vector, the Date flag bits cleared.
void resetPartyEx(PartyEx *local) {
    Party &p = local->party;
    p.name.clear();
    p.message.clear();
    p.adminUserIds.begin = 0;
    p.adminUserIds.end = 0;
    p.adminUserIds.capEnd = 0;
    rw::store8(p.leaderAppointDate + 7, uint8_t(p.leaderAppointDate[7] & 0xFD));
    rw::store8(p.newcomerDate + 7, uint8_t(p.newcomerDate[7] & 0xFD));
    p.chatEndpointUrl.clear();
    const uint8_t *flags = local->leader.lastActionDatetime + 7;
    rw::store8(flags, uint8_t(*flags & 0xFD));
}
// The PartyEx local's destructor (the Party part)
void destroyPartyEx(PartyEx *local) {
    party::freeString(&local->party.chatEndpointUrl);
    rw::call(U64_VECTOR_BASE_DTOR, {rw::guest_addr(&local->party.adminUserIds)});
    party::freeString(&local->party.message);
    party::freeString(&local->party.name);
}

// parties: json["partyList"], through the local, appended to vec.
bool readParties(StdVector *vec, const JsonValue &json, PartyEx *local) {
    const JsonValue &list = json["partyList"];
    if (!list.IsArray()) return false;
    uint32_t n = list.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &entry = list.At(k);
        if (!entry.IsObject()) return false;
        resetPartyEx(local);
        if (!PartyEx::init(local, &entry)) {
            destroyPartyEx(local);
            return false;
        }
        uint32_t end = vec->end;
        if (end == vec->capEnd) {
            rw::call(PARTY_EX_PUSH_SLOW, {rw::guest_addr(vec), rw::guest_addr(local)});
        } else {
            if (end) {
                Party *copy = Party::copyConstruct(rw::host_ptr<Party>(end), &local->party);
                rw::call(guest::memcpy_, {rw::guest_addr(copy) + 0x70, rw::guest_addr(&local->leader), 0x128});
            }
            vec->end = vec->end + sizeof(PartyEx);
        }
        destroyPartyEx(local);
    }
    return true;
}

// supportList = StageSupportList::parse(json, "partyUserList") through the temporary at `tmp`
void readSupportList(SharedPtr *field, const JsonValue *json, const GuestFrame &frame, uint32_t tmp) {
    StageSupportList::parse(frame.at<SharedPtr>(tmp), json, PARTY_USER_LIST);
    SharedPtr *t = frame.at<SharedPtr>(tmp);
    uint32_t ptr = t->ptr, ctrl = t->ctrl;
    t->ptr = 0;
    t->ctrl = 0;
    field->ptr = ptr;
    uint32_t old = field->ctrl;
    field->ctrl = ctrl;
    if (old) {
        rw::call(party::release_shared, {old});
        if (t->ctrl) rw::call(party::release_shared, {t->ctrl});
    }
}
}  // namespace

// ---------------------------------------------------------------------------------------------- PartyApplication
uint32_t PartyApplication::Applicant::init(Applicant *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    return initRequest(self, *json, "applicationDate", "partyApplicantId");
}
RW_FUNCTION(0x4A83BC, "hole::network::api::PartyApplication::Applicant::init", PartyApplication::Applicant::init);

uint32_t PartyApplication::parse(SharedPtr *out, const JsonValue *response, uint32_t type) {
    PartyApplication *self = newObject<PartyApplication>(sizeof(PartyApplication));
    self->applicant.message.clear();
    rw::store8(self->applicant.date + 7, 0);
    rw::store8(self->applicant.agreeDate + 7, 0);
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        if (type != 0) {
            self->failedStatus = 0;
        } else {
            const JsonValue &failed = (*response)["failedStatus"];
            if (!failed.IsUint()) return false;
            self->failedStatus = failed.GetUint();
            if (self->failedStatus != 0) return true;          // a failed application: nothing more
        }
        const JsonValue &applicant = (*response)["partyApplicant"];
        if (!applicant.IsObject() || Applicant::init(&self->applicant, &applicant) != 1) return false;
        if (type != 0) {
            self->type = type;
            return true;
        }
        const JsonValue &reward = (*response)["firstReward"];
        if (!reward.IsInt()) return false;
        self->firstReward = uint8_t(reward.GetUint() != 0 ? 1 : 0);
        self->type = 0;
        return true;
    };
    if (!parseAll()) {
        party::freeString(&self->applicant.message);
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(APPLICATION_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4A8520, "hole::network::api::PartyApplication::parse", PartyApplication::parse);

// ---------------------------------------------------------------------------------------------- PartyApplicationList
uint32_t PartyApplicationList::PartyEx::init(PartyEx *self, const JsonValue *json) {
    const JsonValue &party = (*json)["party"];
    if (!party.IsObject() || Party::init(&self->party, &party) != 1) return 0;
    const JsonValue &leader = (*json)["partyLeader"];
    if (!leader.IsObject()) return 0;
    return StageSupportList::SupportUser::init(&self->leader, &leader);
}
RW_FUNCTION(0x4A8684, "hole::network::api::PartyApplicationList::PartyEx::init", PartyApplicationList::PartyEx::init);

uint32_t PartyApplicationList::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x240: the Applicant local at +0x50, the PartyEx local at +0x88
    GuestFrame frame(0x240);
    PartyApplicationList *self = newObject<PartyApplicationList>(sizeof(PartyApplicationList));
    rw::call(guest::memset_, {rw::guest_addr(self), sizeof(PartyApplicationList), 0});
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        if (!readRequests(&self->applicants, *response, "partyApplicants", frame.at<PartyApplication::Applicant>(0x50),
                          APPLICANT_PUSH_SLOW, &PartyApplication::Applicant::init))
            return false;
        return readParties(&self->parties, *response, frame.at<PartyEx>(0x88));
    };
    if (!parseAll()) {
        rw::call(PARTY_EX_VECTOR_BASE_DTOR, {rw::guest_addr(&self->parties)});
        // operator delete gets what the applicant vector's destructor returns (this)
        guestDelete(rw::call(APPLICANT_VECTOR_BASE_DTOR, {rw::guest_addr(&self->applicants)}));
        self = nullptr;
    }
    rw::call(APPLICATION_LIST_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A86F4, "hole::network::api::PartyApplicationList::parse", PartyApplicationList::parse);

// ---------------------------------------------------------------------------------------------- PartyApplicantList
uint32_t PartyApplicantList::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x98: the support list temporary at +0x38, the Applicant local at +0x40
    GuestFrame frame(0x98);
    PartyApplicantList *self = newObject<PartyApplicantList>(sizeof(PartyApplicantList));
    rw::call(guest::memset_, {rw::guest_addr(self), sizeof(PartyApplicantList), 0});
    bool ok = response->IsObject() &&
              readRequests(&self->applicants, *response, "partyApplicants", frame.at<PartyApplication::Applicant>(0x40),
                           APPLICANT_PUSH_SLOW, &PartyApplication::Applicant::init);
    if (ok) {
        readSupportList(&self->supportList, response, frame, 0x38);
    } else {
        if (self->supportList.ctrl) rw::call(party::release_shared, {self->supportList.ctrl});
        rw::call(APPLICANT_VECTOR_BASE_DTOR, {rw::guest_addr(&self->applicants)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(APPLICANT_LIST_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A8AF0, "hole::network::api::PartyApplicantList::parse", PartyApplicantList::parse);

// ---------------------------------------------------------------------------------------------- PartyInviteOffer
uint32_t PartyInviteOffer::parse(SharedPtr *out, const JsonValue *response) {
    PartyInviteOffer *self = newObject<PartyInviteOffer>(4);
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &failed = (*response)["failedStatus"];
        if (failed.IsUint()) {
            self->failedStatus = failed.GetUint();
            ok = true;
        }
    }
    if (!ok) {
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(INVITE_OFFER_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4A8D84, "hole::network::api::PartyInviteOffer::parse", PartyInviteOffer::parse);

// ---------------------------------------------------------------------------------------------- PartyInviteList
uint32_t PartyInviteList::Offer::init(Offer *self, const JsonValue *json) {
    return initRequest(self, *json, "offerDate", "partyOfferId");
}
RW_FUNCTION(0x4A8E30, "hole::network::api::PartyInviteList::Offer::init", PartyInviteList::Offer::init);

uint32_t PartyInviteList::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x98: the support list temporary at +0x38, the Offer local at +0x40
    GuestFrame frame(0x98);
    PartyInviteList *self = newObject<PartyInviteList>(sizeof(PartyInviteList));
    rw::call(guest::memset_, {rw::guest_addr(self), sizeof(PartyInviteList), 0});
    bool ok = response->IsObject() &&
              readRequests(&self->offers, *response, "partyOffers", frame.at<Offer>(0x40), OFFER_PUSH_SLOW,
                           &Offer::init);
    if (ok) {
        readSupportList(&self->supportList, response, frame, 0x38);
    } else {
        if (self->supportList.ctrl) rw::call(party::release_shared, {self->supportList.ctrl});
        rw::call(OFFER_VECTOR_BASE_DTOR, {rw::guest_addr(&self->offers)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(INVITE_LIST_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A8F94, "hole::network::api::PartyInviteList::parse", PartyInviteList::parse);

uint32_t PartyInviteList::setOffer(PartyInviteList *self, const Offer *offer) {
    uint32_t end = self->offers.end;
    for (uint32_t p = self->offers.begin; p != end; p += sizeof(Offer)) {
        Offer *o = rw::host_ptr<Offer>(p);
        if (o->partyId.get() != offer->partyId.get()) continue;
        o->status = offer->status.get();
        uint16_t h0 = rw::get<uint16_t>(offer, 40), h1 = rw::get<uint16_t>(offer, 42);
        uint16_t h2 = rw::get<uint16_t>(offer, 44), h3 = rw::get<uint16_t>(offer, 46);
        rw::set<uint16_t>(o, 46, h3);
        rw::set<uint16_t>(o, 44, h2);
        rw::set<uint16_t>(o, 42, h1);
        rw::set<uint16_t>(o, 40, h0);
        return h0;
    }
    return end;
}
RW_FUNCTION(0x4A9228, "hole::network::api::PartyInviteList::setOffer", PartyInviteList::setOffer);

// ---------------------------------------------------------------------------------------------- PartyOfferList
uint32_t PartyOfferList::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x240: the Offer local at +0x50, the PartyEx local at +0x88
    GuestFrame frame(0x240);
    PartyOfferList *self = newObject<PartyOfferList>(sizeof(PartyOfferList));
    rw::call(guest::memset_, {rw::guest_addr(self), sizeof(PartyOfferList), 0});
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        if (!readRequests(&self->offers, *response, "partyOffers", frame.at<PartyInviteList::Offer>(0x50),
                          OFFER_PUSH_SLOW, &PartyInviteList::Offer::init))
            return false;
        return readParties(&self->parties, *response, frame.at<PartyEx>(0x88));
    };
    if (!parseAll()) {
        rw::call(PARTY_EX_VECTOR_BASE_DTOR, {rw::guest_addr(&self->parties)});
        if (self->unused.ctrl) rw::call(party::release_shared, {self->unused.ctrl});
        rw::call(OFFER_VECTOR_BASE_DTOR, {rw::guest_addr(&self->offers)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(OFFER_LIST_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A9264, "hole::network::api::PartyOfferList::parse", PartyOfferList::parse);

uint32_t PartyOfferList::setParty(PartyOfferList *self, const Party *party) {
    uint32_t end = self->parties.end;
    for (uint32_t p = self->parties.begin; p != end; p += sizeof(PartyEx)) {
        Party *listed = rw::host_ptr<Party>(p);
        if (listed->partyId.get() != party->partyId.get()) continue;
        listed->memberCount = party->memberCount.get();
        uint16_t h0 = rw::get<uint16_t>(party, 84), h1 = rw::get<uint16_t>(party, 86);
        uint16_t h2 = rw::get<uint16_t>(party, 88), h3 = rw::get<uint16_t>(party, 90);
        rw::set<uint16_t>(listed, 90, h3);
        rw::set<uint16_t>(listed, 88, h2);
        rw::set<uint16_t>(listed, 86, h1);
        rw::set<uint16_t>(listed, 84, h0);
        return h0;
    }
    return end;
}
RW_FUNCTION(0x4A9668, "hole::network::api::PartyOfferList::setParty", PartyOfferList::setParty);

}  // namespace hole::network::api
