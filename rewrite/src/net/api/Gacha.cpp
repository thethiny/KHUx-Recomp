#include "Gacha.hpp"

#include "Party.hpp"
#include "Raid.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t LIST_SHARED_PTR_CTOR = 0x4AA759;       // std::shared_ptr<GachaList>::shared_ptr(GachaList*)
constexpr uint32_t GACHA_PUSH_SLOW = 0x4B35B1;            // vector<Gacha>::__push_back_slow_path(const Gacha&)
constexpr uint32_t GACHA_VECTOR_BASE_DTOR = 0x4B353D;     // __vector_base<Gacha>::~__vector_base
constexpr uint32_t DRAW_SHARED_PTR_CTOR = 0x4AAA55;       // std::shared_ptr<GachaDraw>::shared_ptr(GachaDraw*)
constexpr uint32_t U64_PUSH_SLOW = 0x4B33F9;              // vector<unsigned long long>::__push_back_slow_path
constexpr uint32_t U64_VECTOR_BASE_DTOR = 0x468499;
constexpr uint32_t USER_MEDAL_PARSE = 0x4A029D;
constexpr uint32_t USER_SKILL_PARSE = 0x4A0781;
constexpr uint32_t USER_MATERIAL_PARSE = 0x4A0A71;

void destroyGacha(GachaList::Gacha *g) {
    party::freeString(&g->bannerUrl);
    party::freeString(&g->description);
    party::freeString(&g->name);
}
}  // namespace

// ---------------------------------------------------------------------------------------------- GachaList
uint32_t GachaList::Gacha::init(Gacha *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &valid = (*json)["validAdd"];
    if (!valid.IsInt()) return 0;
    self->validAdd = uint8_t(valid.GetUint() != 0 ? 1 : 0);
    static const char *const kInts[] = {"drawMedalTypeId", "drawType", "sortId", "payType", "price", "lotCount"};
    for (uint32_t k = 0; k < 6; k++) {
        const JsonValue &v = (*json)[kInts[k]];
        if (!v.IsInt()) return 0;
        rw::set<int32_t>(self, 4 + 4 * k, v.GetInt());
    }
    const JsonValue &dayLimited = (*json)["isDayLimited"];
    if (!dayLimited.IsUint()) return 0;
    self->isDayLimited = dayLimited.GetUint();
    const JsonValue &limited = (*json)["userLimitedCount"];
    if (!limited.IsUint()) return 0;
    self->userLimitedCount = limited.GetUint();
    struct Text { const char *key; StdString &field; };
    const Text texts[] = {{"name", self->name}, {"description", self->description}, {"bannerUrl", self->bannerUrl}};
    for (const Text &t : texts) {
        const JsonValue &v = (*json)[t.key];
        if (!v.IsString()) return 0;
        t.field.assign(v.GetString());
    }
    return 1;
}
RW_FUNCTION(0x4AA374, "hole::network::api::GachaList::Gacha::init", GachaList::Gacha::init);

GachaList::Gacha *GachaList::Gacha::copyConstruct(Gacha *self, const Gacha *other) {
    rw::call(guest::memcpy_, {rw::guest_addr(self), rw::guest_addr(other), 0x24});
    party::copyString(&self->name, &other->name);
    party::copyString(&self->description, &other->description);
    party::copyString(&self->bannerUrl, &other->bannerUrl);
    return self;
}
RW_FUNCTION(0x4B37E4, "hole::network::api::GachaList::Gacha::Gacha(const Gacha&)", GachaList::Gacha::copyConstruct);

uint32_t GachaList::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x90);                   // original frame; the Gacha local at +0x30
    Gacha *local = frame.at<Gacha>(0x30);
    GachaList *self = newObject<GachaList>(sizeof(GachaList));
    self->gachas.begin = 0;
    self->gachas.end = 0;
    self->gachas.capEnd = 0;
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &list = (*response)["gachaList"];
        if (!list.IsArray()) return false;
        uint32_t n = list.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = list.At(k);
            if (!entry.IsObject()) return false;
            rw::call(guest::memset_, {rw::guest_addr(&local->name), 0x24, 0});
            if (!Gacha::init(local, &entry)) {
                destroyGacha(local);
                return false;
            }
            uint32_t end = self->gachas.end;
            if (end == self->gachas.capEnd) {
                rw::call(GACHA_PUSH_SLOW, {rw::guest_addr(&self->gachas), rw::guest_addr(local)});
            } else {
                uint32_t base = 0;
                if (end) {
                    Gacha::copyConstruct(rw::host_ptr<Gacha>(end), local);
                    base = self->gachas.end;
                }
                self->gachas.end = base + sizeof(Gacha);
            }
            destroyGacha(local);
        }
        return true;
    };
    if (!parseAll()) {
        // operator delete gets what the vector destructor returns (this)
        guestDelete(rw::call(GACHA_VECTOR_BASE_DTOR, {rw::guest_addr(&self->gachas)}));
        self = nullptr;
    }
    rw::call(LIST_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AA594, "hole::network::api::GachaList::parse", GachaList::parse);

// ---------------------------------------------------------------------------------------------- GachaDraw
uint32_t GachaDraw::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x60: the list temporaries at +0x30 / +0x28 / +0x20, the id local at +0x38
    GuestFrame frame(0x60);
    GachaDraw *self = newObject<GachaDraw>(sizeof(GachaDraw));
    rw::call(guest::memset_, {rw::guest_addr(self), 0x18, 0});
    rw::call(guest::memset_, {rw::guest_addr(&self->userMedalIds), 0x18, 0});

    auto parseInto = [&](SharedPtr *field, uint32_t parseFn, uint32_t local) {
        rw::call(parseFn, {frame.addr(local), rw::guest_addr(response)});
        raid::moveShared(field, frame.at<SharedPtr>(local));
        return field->ptr != 0;
    };
    // a u64 array into vec through the local at +0x38
    auto readIds = [&](const JsonValue &array, StdVector *vec) -> bool {
        if (!array.IsArray()) return false;
        uint32_t n = array.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &v = array.At(k);
            if (!v.IsUint64()) return false;
            uint32_t lo = v.w0, hi = v.w1;
            rw::store32(frame.at<uint32_t>(0x38), lo);
            rw::store32(frame.at<uint32_t>(0x3C), hi);
            uint32_t end = vec->end;
            if (end >= vec->capEnd) {
                rw::call(U64_PUSH_SLOW, {rw::guest_addr(vec), frame.addr(0x38)});
            } else {
                uint32_t base = 0;
                if (end) {
                    rw::save<uint32_t>(end, lo);
                    rw::save<uint32_t>(end + 4, hi);
                    base = vec->end;
                }
                vec->end = base + 8;
            }
        }
        return true;
    };
    auto parseAll = [&]() -> bool {
        if (!parseInto(&self->medals, USER_MEDAL_PARSE, 0x30)) return false;
        if (!parseInto(&self->skills, USER_SKILL_PARSE, 0x28)) return false;
        if (!parseInto(&self->materials, USER_MATERIAL_PARSE, 0x20)) return false;
        const JsonValue &bonus = (*response)["gachaBonus"];
        if (!bonus.IsObject()) return false;
        const JsonValue &addType = bonus["addType"];
        if (!addType.IsInt()) return false;
        self->addType = addType.GetInt();
        if (!readIds(bonus["userMedalIds"], &self->userMedalIds)) return false;
        return readIds((*response)["firstGetUserMedalIds"], &self->firstGetUserMedalIds);
    };
    if (!parseAll()) {
        rw::call(U64_VECTOR_BASE_DTOR, {rw::guest_addr(&self->firstGetUserMedalIds)});
        rw::call(U64_VECTOR_BASE_DTOR, {rw::guest_addr(&self->userMedalIds)});
        if (self->materials.ctrl) raid::releaseShared(self->materials.ctrl);
        if (self->skills.ctrl) raid::releaseShared(self->skills.ctrl);
        if (self->medals.ctrl) raid::releaseShared(self->medals.ctrl);
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(DRAW_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AA7B0, "hole::network::api::GachaDraw::parse", GachaDraw::parse);

}  // namespace hole::network::api
