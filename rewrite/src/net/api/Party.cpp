#include "Party.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t U64_VECTOR_PUSH_SLOW = 0x4B33F9;   // vector<unsigned long long>::__push_back_slow_path(ull&&)
constexpr uint32_t U64_VECTOR_COPY = 0x468431;        // vector<unsigned long long>::vector(const vector&)
constexpr uint32_t U64_VECTOR_DEALLOCATE = 0x4B33C5;  // vector<unsigned long long>::deallocate()
constexpr uint32_t U64_VECTOR_BASE_DTOR = 0x468499;   // __vector_base<unsigned long long>::~__vector_base
constexpr uint32_t USER_PARTY_SHARED_PTR_CTOR = 0x4A78C5;   // std::shared_ptr<UserParty>::shared_ptr(...)
constexpr uint32_t CREATE_SHARED_PTR_CTOR = 0x4A7985;       // std::shared_ptr<PartyCreate>::shared_ptr(...)
constexpr uint32_t LOGIN_SHARED_PTR_CTOR = 0x4A7E79;        // std::shared_ptr<PartyLogin>::shared_ptr(...)

bool readInt(const JsonValue &obj, const char *key, rw::Field<int32_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    field = v.GetInt();
    return true;
}
bool readString(const JsonValue &obj, const char *key, StdString &field) {
    const JsonValue &v = obj[key];
    if (!v.IsString()) return false;
    field.assign(v.GetString());
    return true;
}
}  // namespace

// ---------------------------------------------------------------------------------------------- Party
uint32_t Party::init(Party *self, const JsonValue *json) {
    GuestFrame frame(0x50);                   // original frame; the pushed id local at +0x28
    if (!json->IsObject()) return 0;
    const JsonValue &obj = *json;
    const JsonValue &id = obj["partyId"];
    if (!id.IsUint64()) return 0;
    self->partyId = id.GetUint64();
    if (!readInt(obj, "unionId", self->unionId)) return 0;
    if (!readInt(obj, "rank", self->rank)) return 0;
    if (!readString(obj, "name", self->name)) return 0;
    if (!readInt(obj, "playStyle", self->playStyle)) return 0;
    if (!readInt(obj, "agreeType", self->agreeType)) return 0;
    if (!readString(obj, "message", self->message)) return 0;
    if (!readInt(obj, "memberCount", self->memberCount)) return 0;
    const JsonValue &leader = obj["leaderUserId"];
    if (!leader.IsUint64()) return 0;
    self->leaderUserId = leader.GetUint64();
    const JsonValue &admins = obj["adminUserIds"];
    if (!admins.IsArray()) return 0;
    uint32_t n = admins.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &v = admins.At(k);
        if (!v.IsUint64()) return 0;
        uint32_t lo = v.w0, hi = v.w1;
        rw::store32(frame.at<uint32_t>(0x2C), hi);
        rw::store32(frame.at<uint32_t>(0x28), lo);
        StdVector *list = &self->adminUserIds;
        uint32_t end = list->end;
        if (end >= list->capEnd) {
            rw::call(U64_VECTOR_PUSH_SLOW, {rw::guest_addr(list), frame.addr(0x28)});
        } else {
            uint32_t base = 0;
            if (end) {
                rw::save<uint32_t>(end, lo);
                rw::save<uint32_t>(end + 4, hi);
                base = list->end;
            }
            list->end = base + 8;
        }
    }
    const JsonValue &appoint = obj["leaderAppointDate"];
    if (!appoint.IsString()) return 0;
    if (rw::call(guest::Date_init, {rw::guest_addr(self->leaderAppointDate), appoint.GetString(), 0}) != 1) return 0;
    const JsonValue &newcomer = obj["newcomerDate"];
    if (!newcomer.IsString()) return 0;
    if (rw::call(guest::Date_init, {rw::guest_addr(self->newcomerDate), newcomer.GetString(), 0}) != 1) return 0;
    const JsonValue &chat = obj["chatId"];
    if (!chat.IsUint()) return 0;
    self->chatId = chat.GetUint();
    return readString(obj, "chatEndpointUrl", self->chatEndpointUrl) ? 1 : 0;
}
RW_FUNCTION(0x4A723C, "hole::network::api::Party::init", Party::init);

Party *Party::copyConstruct(Party *self, const Party *other) {
    uint32_t dst = rw::guest_addr(self), src = rw::guest_addr(other);
    party::copyWords(dst, src, 4);                                 // partyId, unionId, rank
    party::copyString(&self->name, &other->name);
    party::copyWords(dst + 28, src + 28, 2);                       // playStyle, agreeType
    party::copyString(&self->message, &other->message);
    party::copyWords(dst + 48, src + 48, 4);                       // memberCount, leaderUserId
    rw::call(U64_VECTOR_COPY, {dst + 64, src + 64});
    rw::call(guest::memcpy_, {dst + 76, src + 76, 0x14});          // the two Dates, chatId
    party::copyString(&self->chatEndpointUrl, &other->chatEndpointUrl);
    return self;
}
RW_FUNCTION(0x4B2E62, "hole::network::api::Party::Party(const Party&)", Party::copyConstruct);

Party *Party::moveAssign(Party *self, Party *other) {
    uint32_t dst = rw::guest_addr(self), src = rw::guest_addr(other);
    party::copyWords(dst, src, 4);
    party::moveString(&self->name, &other->name);
    party::copyWords(dst + 28, src + 28, 2);
    party::moveString(&self->message, &other->message);
    party::copyWords(dst + 48, src + 48, 4);
    rw::call(U64_VECTOR_DEALLOCATE, {dst + 64});
    self->adminUserIds.begin = other->adminUserIds.begin.get();
    self->adminUserIds.end = other->adminUserIds.end.get();
    self->adminUserIds.capEnd = other->adminUserIds.capEnd.get();
    other->adminUserIds.begin = 0;
    other->adminUserIds.end = 0;
    other->adminUserIds.capEnd = 0;
    rw::call(guest::memcpy_, {dst + 76, src + 76, 0x14});
    party::moveString(&self->chatEndpointUrl, &other->chatEndpointUrl);
    return self;
}
RW_FUNCTION(0x4B32A8, "hole::network::api::Party::operator=(Party&&)", Party::moveAssign);

// ---------------------------------------------------------------------------------------------- UserParty
uint32_t UserParty::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x58);                   // original frame; the 8-byte staging local at +0x30
    uint8_t *local = frame.at<uint8_t>(0x30);

    UserParty *self = newObject<UserParty>(sizeof(UserParty));
    constructSafeBuffer(self, 0x20);
    self->vtable = VTABLE;
    self->party.name.clear();
    self->party.message.clear();
    self->party.adminUserIds.begin = 0;
    self->party.adminUserIds.end = 0;
    self->party.adminUserIds.capEnd = 0;
    rw::store8(self->party.leaderAppointDate + 7, uint8_t(self->party.leaderAppointDate[7] & 0xFD));
    rw::store8(self->party.newcomerDate + 7, uint8_t(self->party.newcomerDate[7] & 0xFD));
    self->party.chatEndpointUrl.clear();

    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &user = (*response)["userParty"];
        if (!user.IsObject()) return false;
        static const char *const kIds[] = {"userId", "partyId"};
        for (uint32_t k = 0; k < 2; k++) {
            const JsonValue &v = user[kIds[k]];
            if (!v.IsUint64()) return false;
            rw::store32(local, v.w0);
            rw::store32(local + 4, v.w1);
            writeBytes(self, 8 * k, local, 8);
        }
        static const char *const kFlags[] = {"isJoin", "isLeader", "isAdmin", "flagFirstReward"};
        for (uint32_t k = 0; k < 4; k++) {
            const JsonValue &v = user[kFlags[k]];
            if (!v.IsInt()) return false;
            rw::store8(local, uint8_t(v.GetUint() != 0 ? 1 : 0));
            writeBytes(self, IS_JOIN + k, local, 1);
        }
        const JsonValue &join = user["joinDate"];
        if (!join.IsString()) return false;
        rw::store8(local + 7, 0);
        if (rw::call(guest::Date_init, {frame.addr(0x30), join.GetString(), 0}) == 0) return false;
        writeBytes(self, JOIN_DATE, local, 8);
        // a party is only read when the player is in one
        SafeBuffer::read(self, local, self->data.get() + PARTY_ID, 8, PARTY_ID);
        if ((rw::get<uint32_t>(local, 0) | rw::get<uint32_t>(local, 4)) == 0) return true;
        const JsonValue &party = (*response)["party"];
        if (!party.IsObject()) return false;
        return Party::init(&self->party, &party) != 0;
    };
    if (!parseAll()) {
        deleteVirtual(self);
        self = nullptr;
    }
    rw::call(USER_PARTY_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A7584, "hole::network::api::UserParty::parse", UserParty::parse);

UserParty *UserParty::destroy(UserParty *self) {
    self->vtable = VTABLE;
    party::freeString(&self->party.chatEndpointUrl);
    rw::call(U64_VECTOR_BASE_DTOR, {rw::guest_addr(&self->party.adminUserIds)});
    party::freeString(&self->party.message);
    party::freeString(&self->party.name);
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x4B4DF0, "hole::network::api::UserParty::~UserParty", UserParty::destroy);

uint32_t UserParty::destroyDelete(UserParty *self) {
    destroy(self);
    return rw::call(guest::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x4B4E54, "hole::network::api::UserParty::~UserParty (deleting)", UserParty::destroyDelete);

// ---------------------------------------------------------------------------------------------- small responses
uint32_t PartyCreate::parse(SharedPtr *out, const JsonValue *response) {
    PartyCreate *self = newObject<PartyCreate>(1);
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &reward = (*response)["firstReward"];
        if (reward.IsInt()) {
            self->firstReward = uint8_t(reward.GetUint() != 0 ? 1 : 0);
            ok = true;
        }
    }
    if (!ok) {
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(CREATE_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4A7918, "hole::network::api::PartyCreate::parse", PartyCreate::parse);

uint32_t PartyLogin::parse(SharedPtr *out, const JsonValue *response) {
    PartyLogin *self = newObject<PartyLogin>(sizeof(PartyLogin));
    rw::store8(self->partyLeader.lastActionDatetime + 7, 0);       // the leader's Date flags
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &absence = (*response)["isAbsenceLeader"];
        if (absence.IsInt()) {
            self->isAbsenceLeader = uint8_t(absence.GetUint() != 0 ? 1 : 0);
            const JsonValue &leader = (*response)["partyLeader"];
            ok = leader.IsObject() && StageSupportList::SupportUser::init(&self->partyLeader, &leader) != 0;
        }
    }
    if (!ok) {
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(LOGIN_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4A7DD4, "hole::network::api::PartyLogin::parse", PartyLogin::parse);

uint32_t Ranking::init(Ranking *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &lux = (*json)["lux"];
    if (!lux.IsUint64()) return 0;
    self->lux = lux.GetUint64();
    return readInt(*json, "rank", self->rank) ? 1 : 0;
}
RW_FUNCTION(0x4A7FD8, "hole::network::api::Ranking::init", Ranking::init);

}  // namespace hole::network::api
