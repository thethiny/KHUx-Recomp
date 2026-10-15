#include "UserStamp.hpp"

#include "Party.hpp"
#include "Raid.hpp"
#include "StageCommon.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t STAMP_SHARED_PTR_CTOR = 0x4AD389;   // std::shared_ptr<UserStamp>::shared_ptr(UserStamp*)
constexpr uint32_t CHAT_SHARED_PTR_CTOR = 0x4AD635;    // std::shared_ptr<UserChat>::shared_ptr(UserChat*)
}  // namespace

uint32_t UserStamp::parse(SharedPtr *out, const JsonValue *json) {
    UserStamp *self = nullptr;
    if (json->IsObject()) {
        const JsonValue &ids = (*json)["stampIds"];
        if (ids.IsArray()) {
            int32_t n = int32_t(ids.Size());
            self = newObject<UserStamp>(sizeof(UserStamp));
            constructSafeBuffer(self, uint32_t(n) * 4);
            self->vtable = VTABLE;
            self->count = n;
            int32_t k = 0;
            for (; k < n; k++) {                     // signed compares (original)
                const JsonValue &v = ids.At(uint32_t(k));
                if (!v.IsInt()) break;
                writeU32(self, 4 * uint32_t(k), v.GetUint());
            }
            if (k != n) {
                deleteVirtual(self);
                self = nullptr;
            }
        }
    }
    rw::call(STAMP_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AD26C, "hole::network::api::UserStamp::parse", UserStamp::parse);

uint32_t UserStamp::destroyDelete(UserStamp *self) { return deleteSafeBufferResponse(self); }
RW_FUNCTION(0x4B234A, "hole::network::api::UserStamp::~UserStamp (deleting)", UserStamp::destroyDelete);

uint32_t UserChat::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x40);                   // original frame; the UserStamp temporary at +0x20
    UserChat *self = newObject<UserChat>(sizeof(UserChat));
    rw::call(guest::memset_, {rw::guest_addr(&self->authToken), 0x38, 0});
    auto readText = [](const JsonValue &obj, const char *key, StdString &field) {
        const JsonValue &v = obj[key];
        if (!v.IsString()) return false;
        field.assign(v.GetString());
        return true;
    };
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &userData = (*response)["userData"];
        if (!userData.IsObject()) return false;
        const JsonValue &chat = userData["userChat"];
        if (!chat.IsObject()) return false;
        const JsonValue &id = chat["userId"];
        if (!id.IsUint64()) return false;
        self->userId = id.GetUint64();
        if (!readText(chat, "tagName", self->tagName)) return false;
        if (!readText(chat, "userEndpointUrl", self->userEndpointUrl)) return false;
        if (!readText(*response, "authToken", self->authToken)) return false;
        if (!readText(*response, "userToken", self->userToken)) return false;
        // stamps = UserStamp::parse(userChat) (a copy, then the temporary's destructor)
        UserStamp::parse(frame.at<SharedPtr>(0x20), &chat);
        SharedPtr *tmp = frame.at<SharedPtr>(0x20);
        if (!tmp->ptr) {
            if (tmp->ctrl) raid::releaseShared(tmp->ctrl);
            return false;
        }
        stage::assignShared(&self->stamps, tmp->ptr, tmp->ctrl);
        if (tmp->ctrl) raid::releaseShared(tmp->ctrl);
        return true;
    };
    if (!parseAll()) {
        if (self->stamps.ctrl) raid::releaseShared(self->stamps.ctrl);
        party::freeString(&self->userEndpointUrl);
        party::freeString(&self->tagName);
        party::freeString(&self->userToken);
        party::freeString(&self->authToken);
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(CHAT_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AD3DC, "hole::network::api::UserChat::parse", UserChat::parse);

}  // namespace hole::network::api
