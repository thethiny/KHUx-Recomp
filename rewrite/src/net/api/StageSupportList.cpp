#include "StageSupportList.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t SHARED_PTR_CTOR = 0x4A46E9;        // std::shared_ptr<StageSupportList>::shared_ptr(...)
constexpr uint32_t USER_MEDAL_PARSE_SIMPLEX = 0x4A0439;   // UserMedal::parseSimplex
constexpr uint32_t USER_SKILL_PARSE = 0x4A0781;       // UserSkill::parse
constexpr uint32_t COORDINATE_INIT = 0x49F6C5;        // UserAvatar::Coordinate::init
constexpr uint32_t RECORD = sizeof(StageSupportList::SupportUser);
constexpr uint32_t MEDAL = 72, SKILL = 40;

// The member-name literals SupportUser::init passes to rapidjson's operator[] (.rodata).
constexpr uint32_t KEY_SUPPORT_USER_ID = 0xDFB318, KEY_LEVEL = 0xDFB43D, KEY_USER_NAME = 0xDFB0A1,
                   KEY_TITLE_LEFT_ID = 0xDFB431, KEY_TITLE_RIGHT_ID = 0xDFB424, KEY_TITLE_PLATE_ID = 0xDFB417,
                   KEY_ADD_KIZUNA_POINT = 0xDFB408, KEY_KEYBLADE_ID = 0xDFB32D, KEY_IS_PARTY = 0xDFB400,
                   KEY_IS_GUILT = 0xDFB3F8, KEY_USER_AVATAR = 0xDFB347, KEY_LAST_ACTION_DATETIME = 0xDFAA8A,
                   KEY_EARN_LUX_RANK = 0xDFB3EC;
constexpr uint32_t GOT_OFFSET = 0xFFFFCBF8;   // the literal-pool word r4 holds during the first lookups

// SupportUser::init: the original copies stack bytes it never initialised into the record (UserMedal::Medal's
// pad_44 and unused userSkillIds, built by parseSimplex in the frame below init, where rapidjson's lookups left
// their saved registers). Those bytes differ with any other frame layout; they are never read. The verifier lists
// this function in accepted.txt for that reason.

// obj[key] (key: a guest literal), called like the original: at sp, no realignment
const JsonValue &lookup(const JsonValue &obj, uint32_t key) {
    return *rw::host_ptr<JsonValue>(uint32_t(rw::call_exact(guest::JsonValue_member, {rw::guest_addr(&obj), key})));
}
bool readInt(const JsonValue &obj, uint32_t key, rw::Field<int32_t> &field) {
    const JsonValue &v = lookup(obj, key);
    if (!v.IsInt()) return false;
    field = v.GetInt();
    return true;
}
}  // namespace

uint32_t StageSupportList::SupportUser::init(SupportUser *self, const JsonValue *json) {
    // original frame 0xd0: the skill buffer at +0x30, the skill list at +0x58, the medal list at +0x60, the medal
    // buffer at +0x68
    GuestFrame frame(0xD0);
    const uint32_t jsonAddr = rw::guest_addr(json);
    if (!json->IsObject()) return 0;
    const JsonValue &obj = *json;
    const JsonValue &id = lookup(obj, KEY_SUPPORT_USER_ID);
    if (!id.IsUint64()) return 0;
    self->supportUserId = id.GetUint64();
    if (!readInt(obj, KEY_LEVEL, self->level)) return 0;
    const JsonValue &name = lookup(obj, KEY_USER_NAME);
    if (!name.IsString()) return 0;
    rw::call_exact(guest::strncpy_, {rw::guest_addr(self->userName), name.GetString(), 0x21});
    rw::store8(self->userName + 32, 0);
    if (!readInt(obj, KEY_TITLE_LEFT_ID, self->titleLeftId)) return 0;
    if (!readInt(obj, KEY_TITLE_RIGHT_ID, self->titleRightId)) return 0;
    if (!readInt(obj, KEY_TITLE_PLATE_ID, self->titlePlateId)) return 0;
    if (!readInt(obj, KEY_ADD_KIZUNA_POINT, self->addKizunaPoint)) return 0;
    if (!readInt(obj, KEY_KEYBLADE_ID, self->keybladeId)) return 0;
    const JsonValue &party = lookup(obj, KEY_IS_PARTY);
    if (!party.IsInt()) return 0;
    self->isParty = uint8_t(party.GetUint() != 0 ? 1 : 0);
    const JsonValue &guilt = lookup(obj, KEY_IS_GUILT);
    if (!guilt.IsInt()) return 0;
    self->isGuilt = uint8_t(guilt.GetUint() != 0 ? 1 : 0);

    // the medal: a one-record UserMedal, decoded out of its SafeBuffer
    SharedPtr *medals = frame.at<SharedPtr>(0x60);
    rw::call_exact(USER_MEDAL_PARSE_SIMPLEX, {frame.addr(0x60), jsonAddr});
    uint32_t result = 0;
    if (medals->ptr) {
        SafeBuffer *medalBuf = rw::host_ptr<SafeBuffer>(medals->ptr);
        SafeBuffer::read(medalBuf, frame.at<uint8_t>(0x68), medalBuf->data.get(), MEDAL, 0);
        rw::call_exact(guest::memcpy_, {rw::guest_addr(self->medal), frame.addr(0x68), MEDAL});

        // the skills: every record of the UserSkill list
        SharedPtr *skills = frame.at<SharedPtr>(0x58);
        rw::call_exact(USER_SKILL_PARSE, {frame.addr(0x58), jsonAddr});
        if (skills->ptr) {
            SafeBuffer *skillBuf = rw::host_ptr<SafeBuffer>(skills->ptr);
            self->skillCount = rw::get<uint32_t>(skillBuf, 32);
            if (self->skillCount != 0) {
                uint32_t offset = 0;
                for (uint32_t k = 0;; k++) {
                    SafeBuffer::read(skillBuf, frame.at<uint8_t>(0x30), skillBuf->data.get() + offset, SKILL, offset);
                    for (uint32_t w = 0; w < SKILL; w += 4)     // copied word by word (inlined)
                        rw::save<uint32_t>(rw::guest_addr(self->skills) + offset + w, rw::load<uint32_t>(frame.addr(0x30) + w));
                    offset += SKILL;
                    if (k + 1 >= self->skillCount) break;
                    skillBuf = rw::host_ptr<SafeBuffer>(skills->ptr);
                }
            }
            const JsonValue &avatar = lookup(obj, KEY_USER_AVATAR);
            bool avatarOk = false;
            if (avatar.IsObject()) {
                avatarOk = uint32_t(rw::call_exact(COORDINATE_INIT,
                                                   {rw::guest_addr(self->avatar), rw::guest_addr(&avatar)})) == 1;
            }
            if (avatarOk) {
                const JsonValue &last = lookup(obj, KEY_LAST_ACTION_DATETIME);
                if (last.IsString() &&
                    uint32_t(rw::call_exact(guest::Date_init, {rw::guest_addr(self->lastActionDatetime),
                                                              last.GetString(), 0})) == 1) {
                    const JsonValue &rank = lookup(obj, KEY_EARN_LUX_RANK);
                    result = 1;
                    if (rank.IsInt()) self->earnLuxRank = rank.GetInt();
                }
            }
        }
        if (skills->ctrl) rw::call_exact(guest::shared_release, {skills->ctrl});
    }
    if (medals->ctrl) rw::call_exact(guest::shared_release, {medals->ctrl});
    return result;
}
RW_FUNCTION(0x4A41CC, "hole::network::api::StageSupportList::SupportUser::init", StageSupportList::SupportUser::init);

uint32_t StageSupportList::parse(SharedPtr *out, const JsonValue *json, uint32_t key) {
    // original frame 0x190: the map pair at +0x30, the insert result at +0x40, the SupportUser local at +0x48
    const records::ParseFrame at = {0x190, 0x48, 0x30, 0x40};
    GuestFrame frame(at.size);
    SupportUser *local = frame.at<SupportUser>(at.record);
    UserRecordList *self = nullptr;
    if (json->IsObject()) {
        const JsonValue &list = json->member(key);
        if (list.IsArray()) {
            uint32_t n = list.Size();
            self = records::newList(VTABLE, n, n * RECORD);
            uint32_t k = 0;
            for (; k < n; k++) {
                // the Date flag bit 1 is cleared before each init; on the first pass the compiler's register
                // still holds the byte offset 0x11f, so the byte becomes 0x11f & 0xfd = 0x1d (original quirk)
                const uint8_t *flags = local->lastActionDatetime + 7;
                rw::store8(flags, k == 0 ? uint8_t(0x11F & 0xFD) : uint8_t(*flags & 0xFD));
                if (!SupportUser::init(local, &list.At(k))) break;
                records::addRecord(self, frame, at, RECORD, k, records::MAP_INSERT_INT);
            }
            if (k != n) {
                deleteVirtual(self);
                self = nullptr;
            }
        }
    }
    rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A457C, "hole::network::api::StageSupportList::parse", StageSupportList::parse);

StageSupportList *StageSupportList::destroy(StageSupportList *self) {
    records::destroy(self, VTABLE);
    return self;
}
RW_FUNCTION(0x4B4A94, "hole::network::api::StageSupportList::~StageSupportList", StageSupportList::destroy);

uint32_t StageSupportList::destroyDelete(StageSupportList *self) { return records::destroyDelete(self, VTABLE); }
RW_FUNCTION(0x4B4AC8, "hole::network::api::StageSupportList::~StageSupportList (deleting)",
            StageSupportList::destroyDelete);

}  // namespace hole::network::api
