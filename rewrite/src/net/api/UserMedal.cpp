#include "UserMedal.hpp"

namespace hole::network::api {

namespace {
const records::Kind kMedals = {
    UserMedal::VTABLE, "userMedals", sizeof(UserMedal::Medal),
    0x4A03E5,       // std::shared_ptr<UserMedal>::shared_ptr(UserMedal*)
    0x4B0E2D,       // std::shared_ptr<UserMedal>::make_shared<unsigned>(unsigned&&)
    0x38E38E39,     // 1/9: 72 = 8 * 9
};
const records::Kind kSkills = {
    UserSkill::VTABLE, "userSkills", sizeof(UserSkill::Skill),
    0x4A08C9,       // std::shared_ptr<UserSkill>::shared_ptr(UserSkill*)
    0x4B0C35,       // std::shared_ptr<UserSkill>::make_shared<unsigned>(unsigned&&)
    0xCCCCCCCD,     // 1/5: 40 = 8 * 5
};
const records::Kind kKeyblades = {
    UserKeyblade::VTABLE, "userKeyblades", sizeof(UserKeyblade::Keyblade),
    0x4A1085,       // std::shared_ptr<UserKeyblade>::shared_ptr(UserKeyblade*)
    0x4B0815,       // std::shared_ptr<UserKeyblade>::make_shared<unsigned>(unsigned&&)
    0xBA2E8BA3,     // 1/11: 88 = 8 * 11
};
constexpr uint32_t STONE_SHARED_PTR_CTOR = 0x49FFA5;   // std::shared_ptr<UserStone>::shared_ptr(UserStone*)

// obj[key] must be an int; stored in field
bool readInt(const JsonValue &obj, const char *key, rw::Field<int32_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    field = v.GetInt();
    return true;
}
bool readId(const JsonValue &obj, const char *key, rw::Field<uint64_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsUint64()) return false;
    field = v.GetUint64();
    return true;
}
}  // namespace

// ---------------------------------------------------------------------------------------------- UserMedal
uint32_t UserMedal::Medal::init(Medal *self, const JsonValue *json) {
    GuestFrame frame(0x38);                       // original frame; the Date local at +0x18
    if (!json->IsObject()) return 0;
    const JsonValue &obj = *json;
    if (!readId(obj, "userMedalId", self->userMedalId)) return 0;
    if (!readInt(obj, "medalId", self->medalId)) return 0;
    if (!readInt(obj, "level", self->level)) return 0;
    if (!readInt(obj, "exp", self->exp)) return 0;
    if (!readInt(obj, "attackUpperNumber", self->attackUpperNumber)) return 0;
    if (!readInt(obj, "defenseUpperNumber", self->defenseUpperNumber)) return 0;
    if (!readInt(obj, "burstUpperNumber", self->burstUpperNumber)) return 0;
    if (!readInt(obj, "lock", self->lock)) return 0;
    if (!readInt(obj, "upperCost", self->upperCost)) return 0;
    const JsonValue &guilt = obj["guiltFactor"];
    if (!guilt.IsUint()) return 0;
    self->guiltFactor = guilt.GetUint();

    const JsonValue &skills = obj["userSkills"];
    if (!skills.IsArray()) return 0;
    uint32_t n = skills.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &id = skills.At(k);
        if (!id.IsInt()) return 0;
        // sign-extended to 64 bits; no bounds check (original)
        rw::set<int64_t>(self, offsetof(Medal, userSkillIds) + 8 * k, int64_t(id.GetInt()));
    }
    const JsonValue &again = obj["userSkills"];
    if (!again.IsArray()) return 0;
    uint32_t count = again.Size();
    self->skillCount = count < 2 ? count : 2;
    return records::readDatetime(obj, frame, 0x18, self->getDatetime) ? 1 : 0;
}
RW_FUNCTION(0x49FFF8, "hole::network::api::UserMedal::Medal::init", UserMedal::Medal::init);

uint32_t UserMedal::parse(SharedPtr *out, const JsonValue *response) {
    return records::parseList(out, response, kMedals, {0xA0, 0x38, 0x20, 0x30}, &Medal::init);
}
RW_FUNCTION(0x4A029C, "hole::network::api::UserMedal::parse", UserMedal::parse);

uint32_t UserMedal::parseSimplex(SharedPtr *out, const JsonValue *response) {
    // one medal; "userMedal" itself is not type-checked (Medal::init checks it is an object)
    const records::ParseFrame at = {0x90, 0x28, 0x10, 0x20};
    GuestFrame frame(at.size);
    UserRecordList *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &medal = (*response)["userMedal"];
        self = records::newList(VTABLE, 1, sizeof(Medal));
        if (Medal::init(frame.at<Medal>(at.record), &medal)) {
            records::addRecord(self, frame, at, sizeof(Medal), 0, records::MAP_INSERT_INT);
        } else {
            deleteVirtual(self);
            self = nullptr;
        }
    }
    rw::call(kMedals.sharedPtrCtor, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A0438, "hole::network::api::UserMedal::parseSimplex", UserMedal::parseSimplex);

uint32_t UserMedal::create(SharedPtr *out, const StdVector *medals) {
    return records::createList(out, medals, kMedals);
}
RW_FUNCTION(0x4A0550, "hole::network::api::UserMedal::create", UserMedal::create);

UserMedal *UserMedal::destroy(UserMedal *self) {
    records::destroy(self, VTABLE);
    return self;
}
RW_FUNCTION(0x4B0F6C, "hole::network::api::UserMedal::~UserMedal", UserMedal::destroy);

uint32_t UserMedal::destroyDelete(UserMedal *self) { return records::destroyDelete(self, VTABLE); }
RW_FUNCTION(0x4B0FA0, "hole::network::api::UserMedal::~UserMedal (deleting)", UserMedal::destroyDelete);

// ---------------------------------------------------------------------------------------------- UserSkill
uint32_t UserSkill::Skill::init(Skill *self, const JsonValue *json) {
    GuestFrame frame(0x30);                       // original frame; the Date local at +0x10
    if (!json->IsObject()) return 0;
    const JsonValue &obj = *json;
    if (!readId(obj, "userSkillId", self->userSkillId)) return 0;
    if (!readInt(obj, "skillId", self->skillId)) return 0;
    if (!readInt(obj, "level", self->level)) return 0;
    if (!readInt(obj, "exp", self->exp)) return 0;
    if (!readId(obj, "userMedalId", self->userMedalId)) return 0;
    return records::readDatetime(obj, frame, 0x10, self->getDatetime) ? 1 : 0;
}
RW_FUNCTION(0x4A0618, "hole::network::api::UserSkill::Skill::init", UserSkill::Skill::init);

uint32_t UserSkill::parse(SharedPtr *out, const JsonValue *response) {
    return records::parseList(out, response, kSkills, {0x80, 0x20, 0x48, 0x5C}, &Skill::init);
}
RW_FUNCTION(0x4A0780, "hole::network::api::UserSkill::parse", UserSkill::parse);

uint32_t UserSkill::create(SharedPtr *out, const StdVector *skills) {
    return records::createList(out, skills, kSkills);
}
RW_FUNCTION(0x4A091C, "hole::network::api::UserSkill::create", UserSkill::create);

UserSkill *UserSkill::destroy(UserSkill *self) {
    records::destroy(self, VTABLE);
    return self;
}
RW_FUNCTION(0x4B0D74, "hole::network::api::UserSkill::~UserSkill", UserSkill::destroy);

uint32_t UserSkill::destroyDelete(UserSkill *self) { return records::destroyDelete(self, VTABLE); }
RW_FUNCTION(0x4B0DA8, "hole::network::api::UserSkill::~UserSkill (deleting)", UserSkill::destroyDelete);

// ---------------------------------------------------------------------------------------------- UserKeyblade
uint32_t UserKeyblade::Keyblade::init(Keyblade *self, const JsonValue *json) {
    GuestFrame frame(0x40);                       // original frame; the Date local at +0x20
    if (!json->IsObject()) return 0;
    const JsonValue &obj = *json;
    if (!readId(obj, "userKeybladeId", self->userKeybladeId)) return 0;
    if (!readInt(obj, "category", self->category)) return 0;
    if (!readInt(obj, "keybladeId", self->keybladeId)) return 0;
    const JsonValue &deck = obj["deckMedals"];
    if (!deck.IsArray()) return 0;
    uint32_t count = deck.Size();
    self->deckCount = count < 5 ? count : 5;
    const JsonValue &again = obj["deckMedals"];
    if (!again.IsArray()) return 0;
    uint32_t n = again.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &id = again.At(k);
        if (!id.IsUint64()) return 0;
        // no bounds check (original)
        rw::set<uint32_t>(self, offsetof(Keyblade, deckMedals) + 8 * k, id.w0);
        rw::set<uint32_t>(self, offsetof(Keyblade, deckMedals) + 8 * k + 4, id.w1);
    }
    if (!readInt(obj, "burst", self->burst)) return 0;
    if (!readInt(obj, "totalAttack", self->totalAttack)) return 0;
    if (!readInt(obj, "totalDefense", self->totalDefense)) return 0;
    const JsonValue &favorite = obj["isFavorite"];
    if (!favorite.IsInt()) return 0;
    self->isFavorite = uint8_t(favorite.GetUint() != 0 ? 1 : 0);
    return records::readDatetime(obj, frame, 0x20, self->getDatetime) ? 1 : 0;
}
RW_FUNCTION(0x4A0CBC, "hole::network::api::UserKeyblade::Keyblade::init", UserKeyblade::Keyblade::init);

uint32_t UserKeyblade::parse(SharedPtr *out, const JsonValue *response) {
    return records::parseList(out, response, kKeyblades, {0xB0, 0x38, 0x20, 0x30}, &Keyblade::init);
}
RW_FUNCTION(0x4A0F3C, "hole::network::api::UserKeyblade::parse", UserKeyblade::parse);

uint32_t UserKeyblade::create(SharedPtr *out, const StdVector *keyblades) {
    return records::createList(out, keyblades, kKeyblades);
}
RW_FUNCTION(0x4A10D8, "hole::network::api::UserKeyblade::create", UserKeyblade::create);

UserKeyblade *UserKeyblade::destroy(UserKeyblade *self) {
    records::destroy(self, VTABLE);
    return self;
}
RW_FUNCTION(0x4B0954, "hole::network::api::UserKeyblade::~UserKeyblade", UserKeyblade::destroy);

uint32_t UserKeyblade::destroyDelete(UserKeyblade *self) { return records::destroyDelete(self, VTABLE); }
RW_FUNCTION(0x4B0988, "hole::network::api::UserKeyblade::~UserKeyblade (deleting)", UserKeyblade::destroyDelete);

// ---------------------------------------------------------------------------------------------- UserStone
uint32_t UserStone::parse(SharedPtr *out, const JsonValue *response) {
    // the object is made before the response is looked at
    UserStone *self = newObject<UserStone>(32);
    constructSafeBuffer(self, 8);
    self->vtable = VTABLE;
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &stone = (*response)["userStone"];
        if (stone.IsObject()) {
            const JsonValue &free = stone["freeStone"];
            if (free.IsInt()) {
                writeU32(self, FREE_STONE, free.GetUint());
                const JsonValue &pay = stone["payStone"];
                if (pay.IsInt()) {
                    writeU32(self, PAY_STONE, pay.GetUint());
                    ok = true;
                }
            }
        }
    }
    if (!ok) {
        deleteVirtual(self);
        self = nullptr;
    }
    rw::call(STONE_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x49FE70, "hole::network::api::UserStone::parse", UserStone::parse);

uint32_t UserStone::destroyDelete(UserStone *self) { return deleteSafeBufferResponse(self); }
RW_FUNCTION(0x4B6D0C, "hole::network::api::UserStone::~UserStone (deleting)", UserStone::destroyDelete);

}  // namespace hole::network::api
