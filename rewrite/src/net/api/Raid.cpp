#include "Raid.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t RAID_SHARED_PTR_CTOR = 0x4A2609;        // std::shared_ptr<Raid>::shared_ptr(Raid*)
constexpr uint32_t MEMBER_SHARED_PTR_CTOR = 0x4A28F5;      // std::shared_ptr<RaidMember>::shared_ptr(RaidMember*)
constexpr uint32_t COOPERATOR_VECTOR_DTOR = 0x4B69DB;      // ~__vector_base<RaidMember::Cooperator>
constexpr uint32_t COOPERATOR_PUSH_BACK_SLOW = 0x4B6A25;   // vector<Cooperator>::__push_back_slow_path(const&)

// an int member stored at self + offset; false when it is not an int
bool readInt(void *self, uint32_t offset, const JsonValue &obj, const char *key) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    rw::set<uint32_t>(self, offset, v.GetUint());
    return true;
}
}  // namespace

// ---------------------------------------------------------------------------------------------- RaidBase
uint32_t RaidBase::Info::Part::init(Part *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    if (!readInt(self, 0, *json, "raidEnemyId")) return 0;
    if (!readInt(self, 4, *json, "partsId")) return 0;
    if (!readInt(self, 8, *json, "hp")) return 0;
    if (!readInt(self, 12, *json, "attribute")) return 0;
    if (!readInt(self, 16, *json, "rare")) return 0;
    return 1;
}
RW_FUNCTION(0x4A204C, "hole::network::api::RaidBase::Info::Part::init", RaidBase::Info::Part::init);

uint32_t RaidBase::Info::init(Info *self, const JsonValue *json) {
    // original frame 0x48: the Part local at +0x1c
    GuestFrame frame(0x48);
    Part *part = frame.at<Part>(0x1C);

    if (!json->IsObject()) return 0;
    const JsonValue &raidId = (*json)["raidId"];
    if (!raidId.IsUint64()) return 0;
    self->raidId = raidId.GetUint64();
    if (!readInt(self, 8, *json, "level")) return 0;
    if (!readInt(self, 12, *json, "useAp")) return 0;
    const JsonValue &timeLeft = (*json)["timeLeft"];
    if (!timeLeft.IsString()) return 0;
    if (rw::call(raid::Date_init, {rw::guest_addr(&self->timeLeft), timeLeft.GetString(), 0}) != 1) return 0;
    const JsonValue &fever = (*json)["feverFlag"];
    if (!fever.IsInt()) return 0;
    self->feverFlag = uint8_t(fever.GetUint() != 0);
    const JsonValue &feverTime = (*json)["feverTime"];
    if (!feverTime.IsString()) return 0;
    if (rw::call(raid::Date_init, {rw::guest_addr(&self->feverTime), feverTime.GetString(), 0}) != 1) return 0;
    if (!readInt(self, 36, *json, "stageId")) return 0;

    const JsonValue &partsFirst = (*json)["parts"];
    if (!partsFirst.IsArray()) return 0;
    uint32_t count = partsFirst.Size();
    self->partCount = count < MAX_PARTS ? count : MAX_PARTS;

    const JsonValue &parts = (*json)["parts"];
    if (!parts.IsArray()) return 0;
    uint32_t n = parts.Size();
    // every entry is copied (no bound at 6: more entries overrun the record)
    uint32_t dst = rw::guest_addr(self) + 44;
    for (uint32_t k = 0; k < n; k++, dst += sizeof(Part)) {
        const JsonValue &entry = parts.At(k);
        if (!entry.IsObject()) return 0;
        if (Part::init(part, &entry) != 1) return 0;
        raid::copyWords(dst, rw::guest_addr(part), 5);
    }
    return 1;
}
RW_FUNCTION(0x4A2118, "hole::network::api::RaidBase::Info::init", RaidBase::Info::init);

uint32_t RaidBase::User::init(User *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &userId = (*json)["userId"];
    if (!userId.IsUint64()) return 0;
    self->userId = userId.GetUint64();
    const JsonValue &name = (*json)["name"];
    if (!name.IsString()) return 0;
    self->name.assign(name.GetString());
    if (!readInt(self, 20, *json, "level")) return 0;
    const JsonValue &avatar = (*json)["userAvatar"];
    if (!avatar.IsObject()) return 0;
    return rw::call(raid::Coordinate_init, {rw::guest_addr(&self->avatar), rw::guest_addr(&avatar)});
}
RW_FUNCTION(0x4A2350, "hole::network::api::RaidBase::User::init", RaidBase::User::init);

uint32_t RaidBase::init(RaidBase *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &raid = (*json)["raid"];
    if (!raid.IsObject()) return 0;
    if (Info::init(&self->info, &raid) != 1) return 0;
    const JsonValue &discoverer = (*json)["discoverer"];
    if (!discoverer.IsObject()) return 0;
    return User::init(&self->discoverer, &discoverer);
}
RW_FUNCTION(0x4A2424, "hole::network::api::RaidBase::init", RaidBase::init);

// ---------------------------------------------------------------------------------------------- Raid
uint32_t Raid::init(Raid *self, const JsonValue *json, uint32_t withUsers) {
    if (!json->IsObject()) return 0;
    if (!readInt(self, 240, *json, "raidStatus")) return 0;
    const JsonValue &raid = (*json)["raid"];
    if (!raid.IsObject()) return 0;
    if (RaidBase::Info::init(&self->info, &raid) != 1) return 0;
    if (withUsers != 1) return 1;
    const JsonValue &discoverer = (*json)["discoverer"];
    if (!discoverer.IsObject()) return 0;
    if (RaidBase::User::init(&self->discoverer, &discoverer) != 1) return 0;
    const JsonValue &mvp = (*json)["mvp"];
    if (!mvp.IsObject()) return 0;
    if (RaidBase::User::init(&self->mvp, &mvp) != 1) return 0;
    return 1;
}
RW_FUNCTION(0x4A249C, "hole::network::api::Raid::init", Raid::init);

uint32_t Raid::parse(SharedPtr *out, const JsonValue *response) {
    Raid *self = newObject<Raid>(sizeof(Raid));
    rw::store8(self->info.timeLeft.bytes + 7, 0);      // Date flag bytes
    rw::store8(self->info.feverTime.bytes + 7, 0);
    self->discoverer.name.w2 = 0;                       // empty strings (written last word first)
    self->discoverer.name.w1 = 0;
    self->discoverer.name.w0 = 0;
    self->mvp.name.w2 = 0;
    self->mvp.name.w1 = 0;
    self->mvp.name.w0 = 0;
    if (init(self, response, 0) == 0) {
        raid::freeString(&self->mvp.name);
        raid::freeString(&self->discoverer.name);
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(RAID_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4A2578, "hole::network::api::Raid::parse", Raid::parse);

// ---------------------------------------------------------------------------------------------- RaidMember
uint32_t RaidMember::Cooperator::init(Cooperator *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &user = (*json)["user"];
    if (!user.IsObject()) return 0;
    if (RaidBase::User::init(&self->user, &user) != 1) return 0;
    const JsonValue &damage = (*json)["damage"];
    if (!damage.IsUint()) return 0;
    self->damage = damage.GetUint();
    return 1;
}
RW_FUNCTION(0x4A267C, "hole::network::api::RaidMember::Cooperator::init", RaidMember::Cooperator::init);

uint32_t RaidMember::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0xa0: the Cooperator local at +0x30
    GuestFrame frame(0xA0);
    Cooperator *local = frame.at<Cooperator>(0x30);

    RaidMember *self = newObject<RaidMember>(sizeof(RaidMember));
    self->cooperators.begin = 0;
    self->cooperators.end = 0;
    self->cooperators.capEnd = 0;

    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &list = (*response)["cooperators"];
        if (list.IsArray()) {
            uint32_t n = list.Size();
            ok = true;
            for (uint32_t k = 0; k < n; k++) {
                const JsonValue &entry = list.At(k);
                if (!entry.IsObject()) { ok = false; break; }
                local->user.name.clear();
                if (!Cooperator::init(local, &entry)) {
                    raid::freeString(&local->user.name);
                    ok = false;
                    break;
                }
                uint32_t end = self->cooperators.end;
                if (end == self->cooperators.capEnd) {
                    rw::call(COOPERATOR_PUSH_BACK_SLOW, {rw::guest_addr(&self->cooperators), rw::guest_addr(local)});
                } else {
                    uint32_t newEnd = 0;
                    if (end != 0) {
                        // Cooperator copy: user id, name, then level + avatar + damage (14 words from +0x14)
                        Cooperator *dst = rw::host_ptr<Cooperator>(end);
                        raid::copyWords(end, rw::guest_addr(local), 2);
                        raid::copyString(&dst->user.name, &local->user.name);
                        raid::copyWords(end + 0x14, rw::guest_addr(local) + 0x14, 14);
                        newEnd = self->cooperators.end;
                    }
                    self->cooperators.end = newEnd + sizeof(Cooperator);
                }
                raid::freeString(&local->user.name);
            }
            if (ok) {
                const JsonValue &discoverer = (*response)["discovererUserId"];
                ok = false;
                if (discoverer.IsUint64()) {
                    self->discovererUserId = discoverer.GetUint64();
                    const JsonValue &mvp = (*response)["mvpUserId"];
                    if (mvp.IsUint64()) {
                        self->mvpUserId = mvp.GetUint64();
                        ok = true;
                    }
                }
            }
        }
    }
    if (!ok) {
        guestDelete(rw::call(COOPERATOR_VECTOR_DTOR, {rw::guest_addr(self)}));
        self = nullptr;
    }
    rw::call(MEMBER_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A26E8, "hole::network::api::RaidMember::parse", RaidMember::parse);

}  // namespace hole::network::api
