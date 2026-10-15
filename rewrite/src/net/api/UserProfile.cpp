#include "UserProfile.hpp"

#include "Raid.hpp"
#include "User.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t USER_DATA = 0xDFA622;                  // the "userData" literal (User::parse's section)
constexpr uint32_t SHARED_PTR_CTOR = 0x4ABE39;            // std::shared_ptr<UserProfile>::shared_ptr(UserProfile*)
constexpr uint32_t USER_DETAIL_PARSE = 0x49EE1D;
constexpr uint32_t USER_POINT_PARSE = 0x49E8A9;
constexpr uint32_t USER_MEDAL_PARSE = 0x4A029D;
constexpr uint32_t USER_SKILL_PARSE = 0x4A0781;
constexpr uint32_t USER_AVATAR_PARSE = 0x49F82D;
constexpr uint32_t USER_PARSE = 0x49E449;
constexpr uint32_t U64_VECTOR_BASE_DTOR = 0x468499;

const char *const kRecordKeys[Record::COUNT] = {
    "login", "continueLogin", "getMoney", "getSpherePoint", "useMoney", "sellMedal", "sellMedalCount",
    "sellMaterial", "sellMaterialCount", "getKizunaPoint", "useKizunaPoint", "zukanMedal", "yamiSpeedZukan",
    "yamiPowerZukan", "yamiMagicZukan", "hikariSpeedZukan", "hikariPowerZukan", "hikariMagicZukan", "getMedal",
    "medalEnhanceSuccess", "medalEnhanceBigSuccess", "medalEnhanceGreatSuccess", "medalEvolveSuccess",
    "keybladeEnhance", "useMaterial", "kizunaDraw", "raiseDraw", "getStar1Medal", "getStar2Medal",
    "getStar3Medal", "getStar4Medal", "getStar5Medal", "getStar6Medal", "stageClear", "challengeClear",
    "dropEnemy", "dropMimic", "dropRareEnemy", "bossBattleJoinMe", "bossBattleJoinOther", "dropBoss", "getLux",
    "getLuxMax", "dropParts", "maxDamageOneTurn", "openTreasure", "getMaterial", "openAllSphere", "openGrid",
    "unionPersonalRankingHighest", "unionPartyRankingHighest", "totalPersonalRankingHighest",
    "colosseumRankingHighest",
};
}  // namespace

uint32_t Record::init(Record *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    for (uint32_t k = 0; k < COUNT; k++) {
        const JsonValue &v = (*json)[kRecordKeys[k]];
        if (!v.IsUint64()) return 0;
        self->values[k] = v.GetUint64();
    }
    return 1;
}
RW_FUNCTION(0x4AB04C, "hole::network::api::Record::init", Record::init);

uint32_t UserProfile::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x78: the list temporaries at +0x48 (user) / +0x40 / +0x38 / +0x30 / +0x28 / +0x20 (avatar),
    // the partyId local at +0x50
    GuestFrame frame(0x78);
    UserProfile *self = newObject<UserProfile>(sizeof(UserProfile));
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
    rw::call(guest::memset_, {rw::guest_addr(self), 0x30, 0});                // the six shared_ptrs
    rw::store8(p.leaderAppointDate + 7, 0);
    rw::store8(p.newcomerDate + 7, 0);
    p.chatEndpointUrl.w2 = 0;
    p.chatEndpointUrl.w1 = 0;
    p.chatEndpointUrl.w0 = 0;
    for (uint32_t off = 722; off >= 711; off--) rw::store8(reinterpret_cast<uint8_t *>(self) + off, 0);
    rw::store8(reinterpret_cast<uint8_t *>(self) + 723, 0);    // the Date flags and linkPlatformId, bytewise

    // field = X::parse(...) through the temporary at `local`; a null result stops the parse
    auto take = [&](SharedPtr *field, uint32_t local) {
        raid::moveShared(field, frame.at<SharedPtr>(local));
        return field->ptr != 0;
    };
    auto parseAll = [&]() -> bool {
        rw::call(USER_PARSE, {frame.addr(0x48), rw::guest_addr(response), 0, USER_DATA});
        if (!take(&self->user, 0x48)) return false;
        rw::call(USER_DETAIL_PARSE, {frame.addr(0x40), rw::guest_addr(response)});
        if (!take(&self->detail, 0x40)) return false;
        rw::call(USER_POINT_PARSE, {frame.addr(0x38), rw::guest_addr(response)});
        if (!take(&self->point, 0x38)) return false;
        const JsonValue &userData = (*response)["userData"];
        if (!userData.IsObject()) return false;
        const JsonValue &last = userData["lastActionDatetime"];
        if (!last.IsString()) return false;
        if (rw::call(guest::Date_init, {rw::guest_addr(self->lastActionDatetime), last.GetString(), 0}) != 1) return false;
        rw::call(USER_MEDAL_PARSE, {frame.addr(0x30), rw::guest_addr(response)});
        if (!take(&self->medals, 0x30)) return false;
        rw::call(USER_SKILL_PARSE, {frame.addr(0x28), rw::guest_addr(response)});
        if (!take(&self->skills, 0x28)) return false;
        rw::call(USER_AVATAR_PARSE, {frame.addr(0x20), rw::guest_addr(response)});
        if (!take(&self->avatar, 0x20)) return false;
        const JsonValue &keyblade = (*response)["userKeyblade"];
        if (!keyblade.IsObject() || UserKeyblade::Keyblade::init(&self->keyblade, &keyblade) != 1) return false;
        const JsonValue &record = (*response)["userRecord"];
        if (!record.IsObject() || Record::init(&self->record, &record) != 1) return false;
        const JsonValue &ranking = (*response)["userRanking"];
        if (!ranking.IsObject() || Ranking::init(&self->ranking, &ranking) != 1) return false;
        // the party only when the detail has a partyId
        SafeBuffer *detail = rw::host_ptr<SafeBuffer>(self->detail.ptr);
        SafeBuffer::read(detail, frame.at<uint8_t>(0x50), detail->data.get() + UserDetail::PARTY_ID, 8,
                         UserDetail::PARTY_ID);
        if ((rw::load<uint32_t>(frame.addr(0x50)) | rw::load<uint32_t>(frame.addr(0x54))) != 0) {
            if (Party::init(&self->party, &(*response)["party"]) != 1) return false;
        }
        const JsonValue &link = (*response)["linkPlatformId"];
        if (!link.IsString()) return false;
        self->linkPlatformId.assign(link.GetString());
        return true;
    };
    if (!parseAll()) {
        guestDelete(rw::guest_addr(destroy(self)));
        self = nullptr;
    }
    rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AB9A4, "hole::network::api::UserProfile::parse", UserProfile::parse);

UserProfile *UserProfile::destroy(UserProfile *self) {
    party::freeString(&self->linkPlatformId);
    party::freeString(&self->party.chatEndpointUrl);
    rw::call(U64_VECTOR_BASE_DTOR, {rw::guest_addr(&self->party.adminUserIds)});
    party::freeString(&self->party.message);
    party::freeString(&self->party.name);
    SharedPtr *lists[] = {&self->avatar, &self->skills, &self->medals, &self->point, &self->detail, &self->user};
    for (SharedPtr *s : lists)
        if (s->ctrl) raid::releaseShared(s->ctrl);
    return self;
}
RW_FUNCTION(0x4ABD94, "hole::network::api::UserProfile::~UserProfile", UserProfile::destroy);

}  // namespace hole::network::api
