#include "RaidReward.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t REWARD_SHARED_PTR_CTOR = 0x4A2F1D;     // std::shared_ptr<RaidReward>::shared_ptr(RaidReward*)
constexpr uint32_t REWARD_VECTOR_BASE_DTOR = 0x4B63A5;    // std::__vector_base<RaidReward::Reward>::~__vector_base
constexpr uint32_t REWARD_PUSH_SLOW = 0x4B6431;           // std::vector<RaidReward::Reward>::__push_back_slow_path
constexpr uint32_t R151101_SHARED_PTR_CTOR = 0x4A345D;    // std::shared_ptr<RaidReward151101>::shared_ptr(...)
constexpr uint32_t R151101_VECTOR_BASE_DTOR = 0x4B6119;   // std::__vector_base<RaidReward151101::Reward>::~...
constexpr uint32_t R151101_PUSH_SLOW = 0x4B6175;          // std::vector<RaidReward151101::Reward>::__push_back_slow_path

// Reward::init frames (both 0x60): the Part local at +0x34, the parse results at +0x2c / +0x24 / +0x1c
constexpr uint32_t INIT_FRAME = 0x60;
constexpr uint32_t LOCAL_PART = 0x34, LOCAL_MEDALS = 0x2C, LOCAL_SKILLS = 0x24, LOCAL_MATERIALS = 0x1C;

bool readInt(const JsonValue &obj, const char *key, rw::Field<int32_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    field = v.GetInt();
    return true;
}

// raidId, level, subdueId, mvpId, partAllId, parts (both reward kinds)
bool readHead(RaidRewardHead *self, const JsonValue &obj, const GuestFrame &frame) {
    const JsonValue &id = obj["raidId"];
    if (!id.IsUint64()) return false;
    self->raidId = id.GetUint64();
    if (!readInt(obj, "level", self->level)) return false;
    if (!readInt(obj, "subdueId", self->subdueId)) return false;
    if (!readInt(obj, "mvpId", self->mvpId)) return false;
    if (!readInt(obj, "partAllId", self->partAllId)) return false;
    const JsonValue &parts = obj["parts"];
    if (!parts.IsArray()) return false;
    uint32_t count = parts.Size();
    self->partCount = count < 6 ? count : 6;
    const JsonValue &again = obj["parts"];
    if (!again.IsArray()) return false;
    uint32_t n = again.Size();
    RaidBase::Info::Part *part = frame.at<RaidBase::Info::Part>(LOCAL_PART);
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &entry = again.At(k);
        if (!entry.IsObject()) return false;
        if (RaidBase::Info::Part::init(part, &entry) != 1) return false;
        // no bounds check (original)
        raid::copyWords(rw::guest_addr(self->parts) + 20 * k, rw::guest_addr(part), 5);
    }
    return true;
}

// medals / skills / materials = X::parse(json), each moved into the record; a null one stops the parse
bool readLists(SharedPtr *medals, SharedPtr *skills, SharedPtr *materials, const JsonValue &obj,
               const GuestFrame &frame) {
    rw::call(raid::UserMedal_parse, {frame.addr(LOCAL_MEDALS), rw::guest_addr(&obj)});
    raid::moveShared(medals, frame.at<SharedPtr>(LOCAL_MEDALS));
    if (!medals->ptr) return false;
    rw::call(raid::UserSkill_parse, {frame.addr(LOCAL_SKILLS), rw::guest_addr(&obj)});
    raid::moveShared(skills, frame.at<SharedPtr>(LOCAL_SKILLS));
    if (!skills->ptr) return false;
    rw::call(raid::UserMaterial_parse, {frame.addr(LOCAL_MATERIALS), rw::guest_addr(&obj)});
    raid::moveShared(materials, frame.at<SharedPtr>(LOCAL_MATERIALS));
    return materials->ptr != 0;
}

// ~shared_ptr of a local: release the control block when there is one
void releaseLocal(const SharedPtr &p) {
    if (p.ctrl) raid::releaseShared(p.ctrl);
}
// shared_ptr copy into fresh memory: pointer, control block, add a reference
void copySharedInto(SharedPtr *dst, const SharedPtr &src) {
    dst->ptr = src.ptr.get();
    uint32_t ctrl = src.ctrl;
    dst->ctrl = ctrl;
    if (ctrl) raid::addShared(ctrl);
}
}  // namespace

// ---------------------------------------------------------------------------------------------- RaidReward
uint32_t RaidReward::Reward::init(Reward *self, const JsonValue *json) {
    GuestFrame frame(INIT_FRAME);
    if (!json->IsObject()) return 0;
    const JsonValue &obj = *json;
    if (!readHead(&self->head, obj, frame)) return 0;
    const JsonValue &discoverer = obj["discoverer"];
    if (!discoverer.IsObject() || RaidBase::User::init(&self->discoverer, &discoverer) != 1) return 0;
    if (!readInt(obj, "mvpTotalDamage", self->mvpTotalDamage)) return 0;
    const JsonValue &mvp = obj["mvp"];
    if (!mvp.IsObject() || RaidBase::User::init(&self->mvp, &mvp) != 1) return 0;
    return readLists(&self->medals, &self->skills, &self->materials, obj, frame) ? 1 : 0;
}
RW_FUNCTION(0x4A294C, "hole::network::api::RaidReward::Reward::init", RaidReward::Reward::init);

RaidReward::Reward *RaidReward::Reward::copyConstruct(Reward *self, const Reward *other) {
    uint32_t dst = rw::guest_addr(self), src = rw::guest_addr(other);
    rw::call(guest::memcpy_, {dst, src, 148});                         // the head
    raid::copyWords(dst + 152, src + 152, 2);                          // discoverer.userId
    raid::copyString(&self->discoverer.name, &other->discoverer.name);
    raid::copyWords(dst + 172, src + 172, 14);                         // discoverer level + avatar, mvpTotalDamage
    raid::copyWords(dst + 232, src + 232, 2);                          // mvp.userId
    raid::copyString(&self->mvp.name, &other->mvp.name);
    raid::copyWords(dst + 252, src + 252, 13);                         // mvp level + avatar
    copySharedInto(&self->medals, other->medals);
    copySharedInto(&self->skills, other->skills);
    copySharedInto(&self->materials, other->materials);
    return self;
}
RW_FUNCTION(0x4B67DC, "hole::network::api::RaidReward::Reward::Reward(const Reward&)",
            RaidReward::Reward::copyConstruct);

uint32_t RaidReward::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x1A8);                    // original frame; the Reward local at +0x40
    Reward *local = frame.at<Reward>(0x40);

    RaidReward *self = newObject<RaidReward>(sizeof(RaidReward));
    self->rewards.begin = 0;
    self->rewards.end = 0;
    self->rewards.capEnd = 0;

    // the local's destructor: the three lists, then the two names
    auto destroyLocal = [&]() {
        releaseLocal(local->materials);
        releaseLocal(local->skills);
        releaseLocal(local->medals);
        raid::freeString(&local->mvp.name);
        raid::freeString(&local->discoverer.name);
    };
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &list = (*response)["raidRewards"];
        if (!list.IsArray()) return false;
        uint32_t n = list.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = list.At(k);
            if (!entry.IsObject()) return false;
            local->discoverer.name.clear();
            local->mvp.name.clear();
            rw::call(guest::memset_, {rw::guest_addr(&local->medals), 0x18, 0});
            if (!Reward::init(local, &entry)) {
                destroyLocal();
                return false;
            }
            uint32_t end = self->rewards.end;
            if (end == self->rewards.capEnd) {
                rw::call(REWARD_PUSH_SLOW, {rw::guest_addr(&self->rewards), rw::guest_addr(local)});
            } else {
                uint32_t base = 0;
                if (end) {
                    Reward::copyConstruct(rw::host_ptr<Reward>(end), local);
                    base = self->rewards.end;
                }
                self->rewards.end = base + sizeof(Reward);
            }
            destroyLocal();
        }
        return true;
    };
    if (!parseAll()) {
        guestDelete(rw::call(REWARD_VECTOR_BASE_DTOR, {rw::guest_addr(self)}));
        self = nullptr;
    }
    rw::call(REWARD_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A2CE0, "hole::network::api::RaidReward::parse", RaidReward::parse);

// ---------------------------------------------------------------------------------------------- RaidReward151101
uint32_t RaidReward151101::Reward::init(Reward *self, const JsonValue *json) {
    GuestFrame frame(INIT_FRAME);
    if (!json->IsObject()) return 0;
    const JsonValue &obj = *json;
    if (!readHead(&self->head, obj, frame)) return 0;
    if (!readInt(obj, "mvpTotalDamage", self->mvpTotalDamage)) return 0;
    return readLists(&self->medals, &self->skills, &self->materials, obj, frame) ? 1 : 0;
}
RW_FUNCTION(0x4A2F74, "hole::network::api::RaidReward151101::Reward::init", RaidReward151101::Reward::init);

uint32_t RaidReward151101::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x108);                    // original frame; the Reward local at +0x38
    Reward *local = frame.at<Reward>(0x38);

    RaidReward151101 *self = newObject<RaidReward151101>(sizeof(RaidReward151101));
    self->rewards.begin = 0;
    self->rewards.end = 0;
    self->rewards.capEnd = 0;

    auto destroyLocal = [&]() {
        releaseLocal(local->materials);
        releaseLocal(local->skills);
        releaseLocal(local->medals);
    };
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &list = (*response)["raidRewards"];
        if (!list.IsArray()) return false;
        uint32_t n = list.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = list.At(k);
            if (!entry.IsObject()) return false;
            rw::call(guest::memset_, {rw::guest_addr(&local->medals), 0x18, 0});
            if (!Reward::init(local, &entry)) {
                destroyLocal();
                return false;
            }
            uint32_t end = self->rewards.end;
            if (end == self->rewards.capEnd) {
                rw::call(R151101_PUSH_SLOW, {rw::guest_addr(&self->rewards), rw::guest_addr(local)});
            } else {
                if (end) {
                    // inlined copy constructor: the plain part, then the three shared_ptrs
                    Reward *dst = rw::host_ptr<Reward>(end);
                    rw::call(guest::memcpy_, {end, rw::guest_addr(local), 0x98});
                    copySharedInto(&dst->medals, local->medals);
                    copySharedInto(&dst->skills, local->skills);
                    copySharedInto(&dst->materials, local->materials);
                }
                self->rewards.end = self->rewards.end + sizeof(Reward);
            }
            destroyLocal();
        }
        return true;
    };
    if (!parseAll()) {
        guestDelete(rw::call(R151101_VECTOR_BASE_DTOR, {rw::guest_addr(self)}));
        self = nullptr;
    }
    rw::call(R151101_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A325C, "hole::network::api::RaidReward151101::parse", RaidReward151101::parse);

}  // namespace hole::network::api
