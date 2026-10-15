#include "Colosseum.hpp"

#include "Raid.hpp"
#include "StageStart.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t COLOSSEUM_SHARED_PTR_CTOR = 0x4A9CFD;   // std::shared_ptr<Colosseum>::shared_ptr(Colosseum*)
constexpr uint32_t STAGE_PUSH_SLOW = 0x4B3AC5;             // vector<Colosseum::Stage>::__push_back_slow_path
constexpr uint32_t STAGE_VECTOR_BASE_DTOR = 0x4B3A95;      // __vector_base<Colosseum::Stage>::~__vector_base
constexpr uint32_t START_SHARED_PTR_CTOR = 0x4A9FC9;       // std::shared_ptr<ColosseumStart>::shared_ptr(...)
constexpr uint32_t INT_PUSH_SLOW = 0x47D709;               // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t INT_VECTOR_BASE_DTOR = 0x478961;        // __vector_base<int>::~__vector_base
constexpr uint32_t CLEAR_SHARED_PTR_CTOR = 0x4AA31D;       // std::shared_ptr<ColosseumClear>::shared_ptr(...)
constexpr uint32_t CLEAR_PUSH_SLOW = 0x4B38E5;             // vector<ClearColosseum>::__push_back_slow_path
constexpr uint32_t CLEAR_VECTOR_BASE_DTOR = 0x4B38B5;      // __vector_base<ClearColosseum>::~__vector_base
constexpr uint32_t USER_MATERIAL_PARSE = 0x4A0A71;
constexpr uint32_t USER_MEDAL_PARSE = 0x4A029D;
constexpr uint32_t USER_SKILL_PARSE = 0x4A0781;
constexpr uint32_t USER_TITLE_PARSE = 0x4A1689;
constexpr uint32_t USER_KEYBLADE_PARSE = 0x4A0F3D;
constexpr uint32_t PROPERTY_INIT = 0x4A4E15;               // StageClear::Property::init()

// a run of int members, all required, in order; stops at the first missing one
bool readInts(const JsonValue &obj, std::initializer_list<const char *> keys, rw::Field<int32_t> *fields) {
    for (const char *key : keys) {
        const JsonValue &v = obj[key];
        if (!v.IsInt()) return false;
        *fields++ = v.GetInt();
    }
    return true;
}
}  // namespace

// ---------------------------------------------------------------------------------------------- Colosseum
uint32_t Colosseum::Info::init(Info *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    return readInts(*json, {"colosseumStageId", "status"}, &self->colosseumStageId) ? 1 : 0;
}
RW_FUNCTION(0x4A99A0, "hole::network::api::Colosseum::Info::init", Colosseum::Info::init);

uint32_t Colosseum::Stage::init(Stage *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    return readInts(*json, {"rank", "stageCount", "stageId", "useAp"}, &self->rank) ? 1 : 0;
}
RW_FUNCTION(0x4A9A04, "hole::network::api::Colosseum::Stage::init", Colosseum::Stage::init);

uint32_t Colosseum::UserState::init(UserState *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    return readInts(*json, {"userRank", "userStageCount", "userStageTotalCount"}, &self->userRank) ? 1 : 0;
}
RW_FUNCTION(0x4A9AAC, "hole::network::api::Colosseum::UserState::init", Colosseum::UserState::init);

uint32_t Colosseum::parse(SharedPtr *out, const JsonValue *response, uint32_t infoOnly) {
    GuestFrame frame(0x50);                   // original frame; the Stage local at +0x28
    Stage *local = frame.at<Stage>(0x28);
    Colosseum *self = newObject<Colosseum>(sizeof(Colosseum));
    self->stages.begin = 0;
    self->stages.end = 0;
    self->stages.capEnd = 0;
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &info = (*response)["colosseumInfo"];
        if (!info.IsObject() || Info::init(&self->info, &info) != 1) return false;
        if (infoOnly == 0) {
            const JsonValue &stages = (*response)["colosseumStages"];
            if (!stages.IsArray()) return false;
            uint32_t n = stages.Size();
            for (uint32_t k = 0; k < n; k++) {
                const JsonValue &entry = stages.At(k);
                if (!entry.IsObject() || Stage::init(local, &entry) != 1) return false;
                uint32_t end = self->stages.end;
                if (end == self->stages.capEnd) {
                    rw::call(STAGE_PUSH_SLOW, {rw::guest_addr(&self->stages), rw::guest_addr(local)});
                } else {
                    uint32_t base = 0;
                    if (end) {
                        for (uint32_t w = 0; w < 16; w += 4) rw::save<uint32_t>(end + w, rw::get<uint32_t>(local, w));
                        base = self->stages.end;
                    }
                    self->stages.end = base + sizeof(Stage);
                }
            }
        }
        const JsonValue &user = (*response)["userColosseum"];
        if (!user.IsObject()) return false;
        return UserState::init(&self->user, &user) != 0;
    };
    if (!parseAll()) {
        rw::call(STAGE_VECTOR_BASE_DTOR, {rw::guest_addr(&self->stages)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(COLOSSEUM_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A9B34, "hole::network::api::Colosseum::parse", Colosseum::parse);

// ---------------------------------------------------------------------------------------------- ColosseumStart
uint32_t ColosseumStart::RequestData::init(RequestData *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    if (!readInts(*json, {"rank", "stageCount", "stageId"}, &self->rank)) return 0;
    const JsonValue &keyblade = (*json)["userKeybladeId"];
    if (!keyblade.IsUint64()) return 0;
    self->userKeybladeId = keyblade.GetUint64();
    return 1;
}
RW_FUNCTION(0x4A9D58, "hole::network::api::ColosseumStart::RequestData::init", ColosseumStart::RequestData::init);

uint32_t ColosseumStart::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x58);                   // original frame; the campaign int at +0x24, the Treasure at +0x28
    ColosseumStart *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &treasures = (*response)["userTreasures"];
        if (treasures.IsArray()) {
            uint32_t n = treasures.Size();
            self = newObject<ColosseumStart>(sizeof(ColosseumStart));
            constructSafeBuffer(self, TREASURE * n);
            self->vtable = VTABLE;
            self->treasureCount = n;
            self->campaigns.begin = 0;
            self->campaigns.end = 0;
            self->campaigns.capEnd = 0;
            auto parseAll = [&]() -> bool {
                StageStart::Treasure *treasure = frame.at<StageStart::Treasure>(0x28);
                for (uint32_t k = 0; k < n; k++) {
                    if (!StageStart::Treasure::init(treasure, &treasures.At(k))) return false;
                    writeBytes(self, TREASURE * k, treasure, TREASURE);
                }
                const JsonValue &request = (*response)["startColosseumData"];
                if (!request.IsObject() || RequestData::init(&self->request, &request) != 1) return false;
                const JsonValue &campaigns = (*response)["campaigns"];
                if (!campaigns.IsArray()) return false;
                uint32_t m = campaigns.Size();
                for (uint32_t k = 0; k < m; k++) {
                    const JsonValue &v = campaigns.At(k);
                    if (!v.IsInt()) return false;
                    uint32_t id = v.GetUint();
                    rw::store32(frame.at<uint32_t>(0x24), id);
                    uint32_t end = self->campaigns.end;
                    if (end >= self->campaigns.capEnd) {
                        rw::call(INT_PUSH_SLOW, {rw::guest_addr(&self->campaigns), frame.addr(0x24)});
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
        }
    }
    rw::call(START_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A9E08, "hole::network::api::ColosseumStart::parse", ColosseumStart::parse);

ColosseumStart *ColosseumStart::destroy(ColosseumStart *self) {
    self->vtable = VTABLE;
    rw::call(INT_VECTOR_BASE_DTOR, {rw::guest_addr(&self->campaigns)});
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x4B3A28, "hole::network::api::ColosseumStart::~ColosseumStart", ColosseumStart::destroy);

uint32_t ColosseumStart::destroyDelete(ColosseumStart *self) {
    destroy(self);
    return rw::call(guest::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x4B3A5C, "hole::network::api::ColosseumStart::~ColosseumStart (deleting)", ColosseumStart::destroyDelete);

// ---------------------------------------------------------------------------------------------- ColosseumClear
uint32_t ColosseumClear::ClearColosseum::init(ClearColosseum *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    return readInts(*json, {"rank", "stageCount"}, &self->rank) ? 1 : 0;
}
RW_FUNCTION(0x4AA01C, "hole::network::api::ColosseumClear::ClearColosseum::init", ColosseumClear::ClearColosseum::init);

uint32_t ColosseumClear::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x70: the list temporaries at +0x40 / +0x38 / +0x30 / +0x28 / +0x20, the ClearColosseum
    // local at +0x48
    GuestFrame frame(0x70);
    ColosseumClear *self = nullptr;
    if (response->IsObject()) {
        self = newObject<ColosseumClear>(sizeof(ColosseumClear));
        rw::call(guest::memset_, {rw::guest_addr(self), sizeof(ColosseumClear), 0});
        auto parseInto = [&](SharedPtr *field, uint32_t parseFn, uint32_t local) {
            rw::call(parseFn, {frame.addr(local), rw::guest_addr(response)});
            raid::moveShared(field, frame.at<SharedPtr>(local));
            return field->ptr != 0;
        };
        auto parseAll = [&]() -> bool {
            const JsonValue &clears = (*response)["clearColosseums"];
            if (!clears.IsArray()) return false;
            uint32_t n = clears.Size();
            ClearColosseum *local = frame.at<ClearColosseum>(0x48);
            for (uint32_t k = 0; k < n; k++) {
                const JsonValue &entry = clears.At(k);
                if (!entry.IsObject() || ClearColosseum::init(local, &entry) != 1) return false;
                uint32_t end = self->clears.end;
                if (end == self->clears.capEnd) {
                    rw::call(CLEAR_PUSH_SLOW, {rw::guest_addr(&self->clears), rw::guest_addr(local)});
                } else {
                    uint32_t base = 0;
                    if (end) {
                        rw::save<uint32_t>(end, local->rank.raw);
                        rw::save<uint32_t>(end + 4, local->stageCount.raw);
                        base = self->clears.end;
                    }
                    self->clears.end = base + sizeof(ClearColosseum);
                }
            }
            return parseInto(&self->userMaterial, USER_MATERIAL_PARSE, 0x40) &&
                   parseInto(&self->userMedal, USER_MEDAL_PARSE, 0x38) &&
                   parseInto(&self->userSkill, USER_SKILL_PARSE, 0x30) &&
                   parseInto(&self->userTitle, USER_TITLE_PARSE, 0x28) &&
                   parseInto(&self->userKeyblade, USER_KEYBLADE_PARSE, 0x20);
        };
        if (parseAll()) {
            rw::call(PROPERTY_INIT, {rw::guest_addr(self->stageClearProperty)});
        } else {
            guestDelete(rw::guest_addr(destroy(self)));
            self = nullptr;
        }
    }
    rw::call(CLEAR_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AA080, "hole::network::api::ColosseumClear::parse", ColosseumClear::parse);

ColosseumClear *ColosseumClear::destroy(ColosseumClear *self) {
    // the ten shared_ptrs from the last: release each control block, then the vector
    for (uint32_t off = 0x58; off >= 0x10; off -= 8) {
        uint32_t ctrl = rw::get<uint32_t>(self, off);
        if (ctrl) raid::releaseShared(ctrl);
    }
    rw::call(CLEAR_VECTOR_BASE_DTOR, {rw::guest_addr(&self->clears)});
    return self;
}
RW_FUNCTION(0x4AA2A4, "hole::network::api::ColosseumClear::~ColosseumClear", ColosseumClear::destroy);

}  // namespace hole::network::api
