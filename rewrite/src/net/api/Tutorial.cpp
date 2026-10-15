#include "Tutorial.hpp"

#include "User.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t INVITE_BONUS = 0xDFA7ED;               // the "inviteBonus" literal (User::parse's section)
constexpr uint32_t PROGRESS_SHARED_PTR_CTOR = 0x4AAC95;   // std::shared_ptr<TutorialProgress>::shared_ptr(...)
constexpr uint32_t STATUS_SHARED_PTR_CTOR = 0x4AADB5;     // std::shared_ptr<TutorialStatus>::shared_ptr(...)
constexpr uint32_t CLEAR_SHARED_PTR_CTOR = 0x4AAFBD;      // std::shared_ptr<TutorialClear>::shared_ptr(...)
constexpr uint32_t COORDINATE_INIT = 0x49F6C5;            // UserAvatar::Coordinate::init
constexpr uint32_t U64_VECTOR_BASE_DTOR = 0x468499;       // __vector_base<unsigned long long>::~__vector_base

// ~Party of a Party member or local (strings + the admin id vector)
void destroyParty(Party *p) {
    party::freeString(&p->chatEndpointUrl);
    rw::call(U64_VECTOR_BASE_DTOR, {rw::guest_addr(&p->adminUserIds)});
    party::freeString(&p->message);
    party::freeString(&p->name);
}
}  // namespace

// ---------------------------------------------------------------------------------------------- TutorialProgress
uint32_t TutorialProgress::parse(SharedPtr *out, const JsonValue *response) {
    TutorialProgress *self = newObject<TutorialProgress>(sizeof(TutorialProgress));
    self->name.w2 = 0;
    self->name.w1 = 0;
    self->name.w0 = 0;
    self->inviteCode.clear();
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &tutorial = (*response)["tutorial"];
        if (!tutorial.IsObject()) return false;
        const JsonValue &id = tutorial["userTutorialId"];
        if (!id.IsUint64()) return false;
        self->userTutorialId = id.GetUint64();
        const JsonValue &progression = tutorial["progression"];
        if (!progression.IsInt()) return false;
        self->progression = progression.GetInt();
        const JsonValue &name = tutorial["name"];
        if (!name.IsString()) return false;
        self->name.assign(name.GetString());
        const JsonValue &code = tutorial["inviteCode"];
        if (!code.IsString()) return false;
        self->inviteCode.assign(code.GetString());
        // later fields only exist once the tutorial got that far
        if (self->progression < 4) return true;
        const JsonValue &unionId = tutorial["unionId"];
        if (!unionId.IsInt()) return false;
        self->unionId = unionId.GetInt();
        if (self->progression < 5) return true;
        const JsonValue &avatar = tutorial["avatar"];
        if (!avatar.IsObject()) return false;
        return rw::call(COORDINATE_INIT, {rw::guest_addr(self->avatar), rw::guest_addr(&avatar)}) != 0;
    };
    if (!parseAll()) {
        party::freeString(&self->inviteCode);
        party::freeString(&self->name);
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(PROGRESS_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4AAAD8, "hole::network::api::TutorialProgress::parse", TutorialProgress::parse);

// ---------------------------------------------------------------------------------------------- TutorialStatus
uint32_t TutorialStatus::parse(SharedPtr *out, const JsonValue *response) {
    TutorialStatus *self = newObject<TutorialStatus>(sizeof(TutorialStatus));
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &phase = (*response)["phase"];
        if (phase.IsUint()) {
            self->phase = phase.GetUint();
            const JsonValue &popup = (*response)["popupFlag"];
            if (popup.IsUint64()) {
                self->popupFlag = popup.GetUint64();
                const JsonValue &finished = (*response)["isFinished"];
                if (finished.IsUint()) {
                    self->isFinished = uint8_t(finished.GetUint() != 0 ? 1 : 0);
                    ok = true;
                }
            }
        }
    }
    if (!ok) {
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(STATUS_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4AAD00, "hole::network::api::TutorialStatus::parse", TutorialStatus::parse);

// ---------------------------------------------------------------------------------------------- TutorialClear
TutorialClear::InviteBonus *TutorialClear::InviteBonus::construct(InviteBonus *self) {
    GuestFrame frame(0x98);                   // original frame; the temporary Party at +8
    Party &p = self->party;
    p.name.w2 = 0;
    p.name.w1 = 0;
    p.name.w0 = 0;
    p.message.w2 = 0;
    p.message.w1 = 0;
    p.message.w0 = 0;
    p.adminUserIds.begin = 0;
    p.adminUserIds.end = 0;
    p.adminUserIds.capEnd = 0;
    rw::store8(p.leaderAppointDate + 7, uint8_t(p.leaderAppointDate[7] & 0xFD));
    rw::store8(p.newcomerDate + 7, uint8_t(p.newcomerDate[7] & 0xFD));
    p.chatEndpointUrl.clear();
    self->user.reset();
    self->firstReward = 0;
    // party = Party(): a zeroed temporary moved in, then destroyed
    Party *tmp = frame.at<Party>(8);
    rw::call(guest::memset_, {frame.addr(8), sizeof(Party), 0});
    Party::moveAssign(&p, tmp);
    destroyParty(tmp);
    self->inviteStone = 0;
    return self;
}
RW_FUNCTION(0x4B31D8, "hole::network::api::TutorialClear::InviteBonus::InviteBonus()", TutorialClear::InviteBonus::construct);

uint32_t TutorialClear::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x30);                   // original frame; the User temporary at +0x10
    TutorialClear *self = newObject<TutorialClear>(sizeof(TutorialClear));
    InviteBonus::construct(&self->bonus);
    InviteBonus &b = self->bonus;
    const JsonValue &bonus = (*response)["inviteBonus"];
    if (bonus.IsNull()) {
        // no invite bonus: the default one
    } else {
        bool ok = false;
        if (bonus.IsObject()) {
            User::parse(frame.at<SharedPtr>(0x10), response, 0, INVITE_BONUS);
            SharedPtr *tmp = frame.at<SharedPtr>(0x10);
            uint32_t ptr = tmp->ptr, ctrl = tmp->ctrl;
            tmp->ptr = 0;
            tmp->ctrl = 0;
            b.user.ptr = ptr;
            uint32_t old = b.user.ctrl;
            b.user.ctrl = ctrl;
            if (old) {
                rw::call(party::release_shared, {old});
                if (tmp->ctrl) rw::call(party::release_shared, {tmp->ctrl});
            }
            if (b.user.ptr && Party::init(&b.party, &bonus["party"]) == 1) {
                const JsonValue &reward = bonus["firstReward"];
                if (reward.IsInt()) {
                    b.firstReward = uint8_t(reward.GetUint() != 0 ? 1 : 0);
                    const JsonValue &stone = bonus["inviteStone"];
                    if (stone.IsInt()) {
                        b.inviteStone = stone.GetInt();
                        ok = true;
                    }
                }
            }
        }
        if (!ok) {
            destroyParty(&b.party);
            if (b.user.ctrl) rw::call(party::release_shared, {b.user.ctrl});
            guestDelete(rw::guest_addr(self));
            self = nullptr;
        }
    }
    rw::call(CLEAR_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AAE04, "hole::network::api::TutorialClear::parse", TutorialClear::parse);

}  // namespace hole::network::api
