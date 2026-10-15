#include "PartyNotice.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t SHARED_PTR_CTOR = 0x4A7D7D;        // std::shared_ptr<PartyNotice>::shared_ptr(PartyNotice*)
constexpr uint32_t NEWCOMER_PUSH_SLOW = 0x4B4B5D;     // vector<NewcomerUser>::__push_back_slow_path(const&)
constexpr uint32_t NEWCOMER_VECTOR_BASE_DTOR = 0x4B4B01;  // __vector_base<NewcomerUser>::~__vector_base
}  // namespace

uint32_t PartyNotice::NewcomerUser::init(NewcomerUser *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &name = (*json)["userName"];
    if (!name.IsString()) return 0;
    self->userName.assign(name.GetString());
    const JsonValue &id = (*json)["userId"];
    if (!id.IsUint64()) return 0;
    self->userId = id.GetUint64();
    const JsonValue &join = (*json)["joinDate"];
    if (!join.IsString()) return 0;
    if (rw::call(guest::Date_init, {rw::guest_addr(self->joinDate), join.GetString(), 0}) != 1) return 0;
    const JsonValue &message = (*json)["message"];
    if (!message.IsString()) return 0;
    self->message.assign(message.GetString());
    return 1;
}
RW_FUNCTION(0x4A79D4, "hole::network::api::PartyNotice::NewcomerUser::init", PartyNotice::NewcomerUser::init);

uint32_t PartyNotice::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x78);                   // original frame; the NewcomerUser local at +0x28
    NewcomerUser *local = frame.at<NewcomerUser>(0x28);

    PartyNotice *self = newObject<PartyNotice>(sizeof(PartyNotice));
    self->newcomerUsers.begin = 0;
    self->newcomerUsers.end = 0;
    self->newcomerUsers.capEnd = 0;
    auto destroyLocal = [&]() {
        party::freeString(&local->message);
        party::freeString(&local->userName);
    };
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        static const char *const kFlags[] = {"hasNewOffer", "isApply", "hasNewApplicant", "isNewLeader"};
        for (uint32_t k = 0; k < 4; k++) {
            const JsonValue &v = (*response)[kFlags[k]];
            if (!v.IsInt()) return false;
            rw::store8(reinterpret_cast<uint8_t *>(self) + k, uint8_t(v.GetUint() != 0 ? 1 : 0));
        }
        const JsonValue &users = (*response)["newcomerUsers"];
        if (!users.IsArray()) return false;
        uint32_t n = users.Size();
        StdVector *list = &self->newcomerUsers;
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = users.At(k);
            if (!entry.IsObject()) return false;
            local->userName.clear();
            rw::store8(local->joinDate + 7, uint8_t(local->joinDate[7] & 0xFD));
            local->message.clear();
            if (!NewcomerUser::init(local, &entry)) {
                destroyLocal();
                return false;
            }
            uint32_t end = list->end;
            if (end == list->capEnd) {
                rw::call(NEWCOMER_PUSH_SLOW, {rw::guest_addr(list), rw::guest_addr(local)});
            } else {
                if (end) {
                    NewcomerUser *dst = rw::host_ptr<NewcomerUser>(end);
                    party::copyString(&dst->userName, &local->userName);
                    party::copyWords(end + 16, rw::guest_addr(local) + 16, 4);   // userId, joinDate
                    party::copyString(&dst->message, &local->message);
                }
                list->end = list->end + sizeof(NewcomerUser);
            }
            destroyLocal();
        }
        return true;
    };
    if (!parseAll()) {
        rw::call(NEWCOMER_VECTOR_BASE_DTOR, {rw::guest_addr(&self->newcomerUsers)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A7AC8, "hole::network::api::PartyNotice::parse", PartyNotice::parse);

}  // namespace hole::network::api
