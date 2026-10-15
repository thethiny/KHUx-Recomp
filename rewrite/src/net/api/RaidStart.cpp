#include "RaidStart.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t START_SHARED_PTR_CTOR = 0x4A5855;   // std::shared_ptr<RaidStart>::shared_ptr(RaidStart*)
constexpr uint32_t PLAYER_PUSH_SLOW = 0x4B53F5;        // std::vector<RaidStart::Player>::__push_back_slow_path
constexpr uint32_t PLAYER_VECTOR_BASE_DTOR = 0x4B572D; // std::__vector_base<RaidStart::Player>::~__vector_base
constexpr uint32_t INT_PUSH_SLOW = 0x47D709;           // std::vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t INT_VECTOR_BASE_DTOR = 0x478961;    // std::__vector_base<int>::~__vector_base
constexpr uint32_t CLEAR_SHARED_PTR_CTOR = 0x4A5A95;   // std::shared_ptr<RaidClear>::shared_ptr(RaidClear*)
constexpr uint32_t USER_TITLE_PARSE = 0x4A1689;        // UserTitle::parse
constexpr uint32_t USER_KEYBLADE_PARSE = 0x4A0F3D;     // UserKeyblade::parse
constexpr uint32_t PROPERTY_INIT = 0x4A4E15;           // StageClear::Property::init()

bool readInt(const JsonValue &obj, const char *key, rw::Field<int32_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    field = v.GetInt();
    return true;
}
}  // namespace

// ---------------------------------------------------------------------------------------------- RaidStart
uint32_t RaidStart::Player::init(Player *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &avatar = (*json)["userAvatar"];
    if (!avatar.IsObject()) return 0;
    if (rw::call(raid::Coordinate_init, {rw::guest_addr(&self->avatar), rw::guest_addr(&avatar)}) != 1) return 0;
    if (!readInt(*json, "damage", self->damage)) return 0;
    const JsonValue &name = (*json)["name"];
    if (!name.IsString()) return 0;
    self->name.assign(name.GetString());
    if (!readInt(*json, "partsId", self->partsId)) return 0;
    if (!readInt(*json, "keybladeId", self->keybladeId)) return 0;
    return 1;
}
RW_FUNCTION(0x4A51B4, "hole::network::api::RaidStart::Player::init", RaidStart::Player::init);

uint32_t RaidStart::RequestData::init(RequestData *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    struct IdField { const char *key; rw::Field<uint64_t> &field; };
    const IdField ids[] = {{"raidId", self->raidId}, {"supportUserId", self->supportUserId},
                           {"userKeybladeId", self->userKeybladeId}};
    for (const IdField &f : ids) {
        const JsonValue &v = (*json)[f.key];
        if (!v.IsUint64()) return 0;
        f.field = v.GetUint64();
    }
    return 1;
}
RW_FUNCTION(0x4A52A8, "hole::network::api::RaidStart::RequestData::init", RaidStart::RequestData::init);

uint32_t RaidStart::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x1e8: the Info local at +0x48 (later the campaign int), Player locals at +0xf4 (players),
    // +0x13c (vitalsLog), +0x184 (partsLog)
    GuestFrame frame(0x1E8);
    constexpr uint32_t LOCAL_INFO = 0x48;

    RaidStart *self = newObject<RaidStart>(sizeof(RaidStart));
    constructSafeBuffer(self, INFO_SIZE);
    self->vtable = VTABLE;
    self->players.begin = 0;
    self->players.end = 0;
    self->players.capEnd = 0;
    self->campaigns.begin = 0;
    self->campaigns.end = 0;
    self->campaigns.capEnd = 0;
    rw::call(guest::memset_, {rw::guest_addr(&self->vitalsLog), 0x18, 0});   // vitalsLog + partsLog

    // a list of players into `list`, parsed through the Player local at `localOffset`
    auto readPlayers = [&](const char *key, StdVector *list, uint32_t localOffset) -> bool {
        const JsonValue &array = (*response)[key];
        if (!array.IsArray()) return false;
        uint32_t n = array.Size();
        Player *local = frame.at<Player>(localOffset);
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = array.At(k);
            if (!entry.IsObject()) return false;
            local->name.clear();
            if (!Player::init(local, &entry)) {
                raid::freeString(&local->name);
                return false;
            }
            uint32_t end = list->end;
            if (end == list->capEnd) {
                rw::call(PLAYER_PUSH_SLOW, {rw::guest_addr(list), rw::guest_addr(local)});
            } else {
                uint32_t base = 0;
                if (end) {
                    // Player copy: avatar + damage, the name, partsId + keybladeId
                    raid::copyWords(end, rw::guest_addr(local), 13);
                    raid::copyString(&rw::host_ptr<Player>(end)->name, &local->name);
                    raid::copyWords(end + 64, rw::guest_addr(local) + 64, 2);
                    base = list->end;
                }
                list->end = base + sizeof(Player);
            }
            raid::freeString(&local->name);
        }
        return true;
    };

    auto parseAll = [&]() -> bool {
        const JsonValue &raid = (*response)["raid"];
        if (!raid.IsObject()) return false;
        RaidBase::Info *info = frame.at<RaidBase::Info>(LOCAL_INFO);
        rw::store8(info->timeLeft.bytes + 7, 0);              // Date flag bytes
        rw::store8(info->feverTime.bytes + 7, 0);
        if (RaidBase::Info::init(info, &raid) == 0) return false;
        writeBytes(self, 0, info, INFO_SIZE);
        if (!readPlayers("players", &self->players, 0xF4)) return false;
        const JsonValue &request = (*response)["startRaidData"];
        if (!request.IsObject() || RequestData::init(&self->request, &request) != 1) return false;
        if (!readPlayers("vitalsLog", &self->vitalsLog, 0x13C)) return false;
        if (!readPlayers("partsLog", &self->partsLog, 0x184)) return false;
        const JsonValue &discoverer = (*response)["discovererUserId"];
        if (!discoverer.IsUint64()) return false;
        self->discovererUserId = discoverer.GetUint64();
        const JsonValue &campaigns = (*response)["campaigns"];
        if (!campaigns.IsArray()) return false;
        uint32_t n = campaigns.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &v = campaigns.At(k);
            if (!v.IsInt()) return false;
            uint32_t id = v.GetUint();
            rw::store32(frame.at<uint32_t>(LOCAL_INFO), id);
            uint32_t end = self->campaigns.end;
            if (end >= self->campaigns.capEnd) {
                rw::call(INT_PUSH_SLOW, {rw::guest_addr(&self->campaigns), frame.addr(LOCAL_INFO)});
            } else {
                uint32_t base = 0;
                if (end) {
                    rw::save<uint32_t>(end, id);
                    base = self->campaigns.end;
                }
                self->campaigns.end = base + 4;
            }
        }
        return true;
    };

    if (!parseAll()) {
        deleteVirtual(self);
        self = nullptr;
    }
    rw::call(START_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A533C, "hole::network::api::RaidStart::parse", RaidStart::parse);

RaidStart *RaidStart::destroy(RaidStart *self) {
    self->vtable = VTABLE;
    rw::call(INT_VECTOR_BASE_DTOR, {rw::guest_addr(&self->campaigns)});
    rw::call(PLAYER_VECTOR_BASE_DTOR, {rw::guest_addr(&self->partsLog)});
    rw::call(PLAYER_VECTOR_BASE_DTOR, {rw::guest_addr(&self->vitalsLog)});
    rw::call(PLAYER_VECTOR_BASE_DTOR, {rw::guest_addr(&self->players)});
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x4B5680, "hole::network::api::RaidStart::~RaidStart", RaidStart::destroy);

uint32_t RaidStart::destroyDelete(RaidStart *self) {
    destroy(self);
    return rw::call(guest::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x4B56D4, "hole::network::api::RaidStart::~RaidStart (deleting)", RaidStart::destroyDelete);

// ---------------------------------------------------------------------------------------------- RaidClear
uint32_t RaidClear::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x48: the parse results at +0x28 (skills), +0x20 (titles), +0x18 (keyblades)
    GuestFrame frame(0x48);
    RaidClear *self = nullptr;
    if (response->IsObject()) {
        self = newObject<RaidClear>(sizeof(RaidClear));
        rw::call(guest::memset_, {rw::guest_addr(self) + 8, 0x40, 0});
        auto parseAll = [&]() -> bool {
            const JsonValue &status = (*response)["raidTimeStatus"];
            if (!status.IsInt() || status.GetUint() - 1 > 4) return false;
            self->raidTimeStatus = status.GetInt();
            const JsonValue &relief = (*response)["reliefFlag"];
            if (!relief.IsInt()) return false;
            self->reliefFlag = uint8_t(relief.GetUint() != 0 ? 1 : 0);
            rw::call(raid::UserSkill_parse, {frame.addr(0x28), rw::guest_addr(response)});
            raid::moveShared(&self->skills, frame.at<SharedPtr>(0x28));
            if (!self->skills.ptr) return false;
            rw::call(USER_TITLE_PARSE, {frame.addr(0x20), rw::guest_addr(response)});
            raid::moveShared(&self->titles, frame.at<SharedPtr>(0x20));
            if (!self->titles.ptr) return false;
            rw::call(USER_KEYBLADE_PARSE, {frame.addr(0x18), rw::guest_addr(response)});
            raid::moveShared(&self->keyblades, frame.at<SharedPtr>(0x18));
            return self->keyblades.ptr != 0;
        };
        if (parseAll()) {
            rw::call(PROPERTY_INIT, {rw::guest_addr(self->property)});
        } else {
            guestDelete(rw::guest_addr(destroy(self)));
            self = nullptr;
        }
    }
    rw::call(CLEAR_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A58A8, "hole::network::api::RaidClear::parse", RaidClear::parse);

RaidClear *RaidClear::destroy(RaidClear *self) {
    // the eight shared_ptrs from the last: release each control block
    for (int k = 7; k >= 0; k--) {
        uint32_t ctrl = rw::get<uint32_t>(self, 12 + 8 * uint32_t(k));
        if (ctrl) raid::releaseShared(ctrl);
    }
    return self;
}
RW_FUNCTION(0x4A5A38, "hole::network::api::RaidClear::~RaidClear", RaidClear::destroy);

}  // namespace hole::network::api
