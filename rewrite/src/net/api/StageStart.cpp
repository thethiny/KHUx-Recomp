#include "StageStart.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t START_SHARED_PTR_CTOR = 0x4A4D35;  // std::shared_ptr<StageStart>::shared_ptr(StageStart*)
constexpr uint32_t CLEAR_SHARED_PTR_CTOR = 0x4A5161;  // std::shared_ptr<StageClear>::shared_ptr(StageClear*)
constexpr uint32_t API_MANAGER_GET_INSTANCE = 0x4BA8C9;   // APIManager::getInstance()
constexpr uint32_t USER_MATERIAL_PARSE = 0x4A0A71;    // UserMaterial::parse
constexpr uint32_t USER_MEDAL_PARSE = 0x4A029D;       // UserMedal::parse
constexpr uint32_t USER_SKILL_PARSE = 0x4A0781;       // UserSkill::parse
constexpr uint32_t USER_TITLE_PARSE = 0x4A1689;       // UserTitle::parse
constexpr uint32_t USER_KEYBLADE_PARSE = 0x4A0F3D;    // UserKeyblade::parse

bool readInt(const JsonValue &obj, const char *key, rw::Field<int32_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    field = v.GetInt();
    return true;
}

// {idKey, "dropItemTypeIds"} (EnemyDropItem / Treasure)
uint32_t initDropList(StageStart::DropList *self, const JsonValue *json, const char *idKey) {
    if (!json->IsObject()) return 0;
    if (!readInt(*json, idKey, self->id)) return 0;
    const JsonValue &ids = (*json)["dropItemTypeIds"];
    if (!ids.IsArray()) return 0;
    uint32_t count = ids.Size();
    self->count = count < 4 ? count : 4;
    const JsonValue &again = (*json)["dropItemTypeIds"];
    if (!again.IsArray()) return 0;
    uint32_t n = again.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &v = again.At(k);
        if (!v.IsInt()) return 0;
        rw::set<int32_t>(self, 8 + 4 * k, v.GetInt());          // no bounds check (original)
    }
    return 1;
}

// field = X::parse(json) through the temporary at `local`
void parseInto(SharedPtr *field, uint32_t parseFn, const GuestFrame &frame, uint32_t local, const JsonValue *json) {
    rw::call(parseFn, {frame.addr(local), rw::guest_addr(json)});
    stage::moveShared(field, frame.at<SharedPtr>(local));
}
}  // namespace

// ---------------------------------------------------------------------------------------------- StageStart
uint32_t StageStart::RandomEnemy::init(RandomEnemy *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    return readInt(*json, "uniqueSymbolEnemyId", self->uniqueSymbolEnemyId) ? 1 : 0;
}
RW_FUNCTION(0x4A473C, "hole::network::api::StageStart::RandomEnemy::init", StageStart::RandomEnemy::init);

uint32_t StageStart::EnemyDropItem::init(EnemyDropItem *self, const JsonValue *json) {
    return initDropList(self, json, "uniqueEnemyId");
}
RW_FUNCTION(0x4A477C, "hole::network::api::StageStart::EnemyDropItem::init", StageStart::EnemyDropItem::init);

uint32_t StageStart::Treasure::init(Treasure *self, const JsonValue *json) {
    return initDropList(self, json, "uniqueTreasureId");
}
RW_FUNCTION(0x4A4830, "hole::network::api::StageStart::Treasure::init", StageStart::Treasure::init);

uint32_t StageStart::RequestData::init(RequestData *self, const JsonValue *json) {
    GuestFrame frame(0x38);                   // original frame; the pushed int local at +0x1c
    if (!json->IsObject()) return 0;
    if (!readInt(*json, "stageId", self->stageId)) return 0;
    const JsonValue &support = (*json)["supportUserId"];
    if (!support.IsUint64()) return 0;
    self->supportUserId = support.GetUint64();
    const JsonValue &keyblade = (*json)["userKeybladeId"];
    if (!keyblade.IsUint64()) return 0;
    self->userKeybladeId = keyblade.GetUint64();
    const JsonValue &missions = (*json)["clearMissionIds"];
    if (!missions.IsArray()) return 0;
    uint32_t n = missions.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &v = missions.At(k);
        if (!v.IsInt()) return 0;
        stage::pushInt(&self->clearMissionIds, frame.addr(0x1C), v.GetUint());
    }
    const JsonValue &skip = (*json)["stageSkip"];
    if (!skip.IsUint()) return 0;
    self->stageSkip = uint8_t(skip.GetUint());
    return 1;
}
RW_FUNCTION(0x4A48E4, "hole::network::api::StageStart::RequestData::init", StageStart::RequestData::init);

uint32_t StageStart::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x80: RandomEnemy local at +0x30, the campaign int at +0x34, Treasure at +0x38,
    // EnemyDropItem at +0x50
    GuestFrame frame(0x80);
    StageStart *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &randoms = (*response)["userRandomEnemies"];
        if (randoms.IsArray()) {
            const JsonValue &drops = (*response)["userEnemyDropItems"];
            if (drops.IsArray()) {
                const JsonValue &treasures = (*response)["userTreasures"];
                if (treasures.IsArray()) {
                    uint32_t dropCount = drops.Size();
                    uint32_t treasureCount = treasures.Size();
                    uint32_t randomCount = randoms.Size();
                    self = newObject<StageStart>(sizeof(StageStart));
                    constructSafeBuffer(self, 24 * (treasureCount + dropCount) + 4 * randomCount);
                    self->vtable = VTABLE;
                    self->randomEnemyCount = randomCount;
                    self->enemyDropCount = dropCount;
                    self->treasureCount = treasureCount;
                    rw::call(guest::memset_, {rw::guest_addr(&self->request.clearMissionIds), 0x18, 0});

                    auto parseAll = [&]() -> bool {
                        RandomEnemy *random = frame.at<RandomEnemy>(0x30);
                        for (uint32_t k = 0; k < randomCount; k++) {
                            if (!RandomEnemy::init(random, &randoms.At(k))) return false;
                            writeBytes(self, 4 * k, random, 4);
                        }
                        EnemyDropItem *drop = frame.at<EnemyDropItem>(0x50);
                        for (uint32_t k = 0; k < dropCount; k++) {
                            if (EnemyDropItem::init(drop, &drops.At(k)) != 1) return false;
                            writeBytes(self, 24 * k + 4 * self->randomEnemyCount, drop, 24);
                        }
                        Treasure *treasure = frame.at<Treasure>(0x38);
                        for (uint32_t k = 0; k < treasureCount; k++) {
                            if (Treasure::init(treasure, &treasures.At(k)) != 1) return false;
                            writeBytes(self, 24 * (k + self->enemyDropCount) + 4 * self->randomEnemyCount, treasure, 24);
                        }
                        const JsonValue &request = (*response)["startStageData"];
                        if (!request.IsObject() || RequestData::init(&self->request, &request) != 1) return false;
                        const JsonValue &campaigns = (*response)["campaigns"];
                        if (!campaigns.IsArray()) return false;
                        uint32_t n = campaigns.Size();
                        for (uint32_t k = 0; k < n; k++) {
                            const JsonValue &v = campaigns.At(k);
                            if (!v.IsInt()) return false;
                            stage::pushInt(&self->campaigns, frame.addr(0x34), v.GetUint());
                        }
                        return true;
                    };
                    if (!parseAll()) {
                        deleteVirtual(self);
                        self = nullptr;
                    }
                }
            }
        }
    }
    rw::call(START_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A4A60, "hole::network::api::StageStart::parse", StageStart::parse);

StageStart *StageStart::destroy(StageStart *self) {
    self->vtable = VTABLE;
    rw::call(stage::fn::vector_int_base_dtor, {rw::guest_addr(&self->campaigns)});
    rw::call(stage::fn::vector_int_base_dtor, {rw::guest_addr(&self->request.clearMissionIds)});
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x4B581C, "hole::network::api::StageStart::~StageStart", StageStart::destroy);

uint32_t StageStart::destroyDelete(StageStart *self) {
    destroy(self);
    return rw::call(guest::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x4B585C, "hole::network::api::StageStart::~StageStart (deleting)", StageStart::destroyDelete);

// ---------------------------------------------------------------------------------------------- StageClear
uint32_t StageClear::Reward::init(Reward *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    if (!readInt(*json, "rewardkind", self->rewardkind)) return 0;
    const JsonValue &medal = (*json)["userMedalId"];
    if (!medal.IsUint64()) return 0;
    self->userMedalId = medal.GetUint64();
    if (!readInt(*json, "display", self->display)) return 0;
    return 1;
}
RW_FUNCTION(0x4A4D88, "hole::network::api::StageClear::Reward::init", StageClear::Reward::init);

uint32_t StageClear::Property::init(Property *self) {
    // copy (shared_ptr operator=) five of APIManager's list pointers
    static constexpr uint32_t kSources[5] = {0x8C, 0x94, 0xDC, 0xA4, 0xEC};
    const uint8_t *api = rw::host_ptr<uint8_t>(rw::call(API_MANAGER_GET_INSTANCE, {}));
    for (uint32_t k = 0; k < 5; k++) {
        uint32_t ctrl = rw::get<uint32_t>(api, kSources[k] + 4);
        uint32_t ptr = rw::get<uint32_t>(api, kSources[k]);
        stage::assignShared(&self->lists[k], ptr, ctrl);
    }
    return 1;
}
RW_FUNCTION(0x4A4E14, "hole::network::api::StageClear::Property::init", StageClear::Property::init);

StageClear::Property *StageClear::Property::assign(Property *self, const Property *other) {
    for (uint32_t k = 0; k < 5; k++) stage::assignShared(&self->lists[k], other->lists[k].ptr, other->lists[k].ctrl);
    return self;
}
RW_FUNCTION(0x683DB8, "hole::network::api::StageClear::Property::operator=", StageClear::Property::assign);

uint32_t StageClear::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x88: the list temporaries at +0x48 / +0x40 / +0x38 / +0x30 / +0x28, the Reward local at
    // +0x50, the flag byte at +0x6b
    GuestFrame frame(0x88);
    StageClear *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &rewards = (*response)["stageRewardUserMedalIds"];
        if (rewards.IsArray()) {
            uint32_t n = rewards.Size();
            self = newObject<StageClear>(sizeof(StageClear));
            constructSafeBuffer(self, (24 * n) | 1);
            self->vtable = VTABLE;
            self->count = n;
            rw::call(guest::memset_, {rw::guest_addr(&self->materials), 0x58, 0});
            auto parseAll = [&]() -> bool {
                Reward *reward = frame.at<Reward>(0x50);
                for (uint32_t k = 0; k < n; k++) {
                    if (!Reward::init(reward, &rewards.At(k))) return false;
                    writeBytes(self, 24 * k, reward, 24);
                }
                const JsonValue &first = (*response)["firstClearFlag"];
                if (!first.IsInt()) return false;
                uint8_t *flag = frame.at<uint8_t>(0x6B);
                rw::store8(flag, uint8_t(first.GetUint() != 0 ? 1 : 0));
                writeBytes(self, 0, flag, 1);                         // over reward 0's first byte (original)
                parseInto(&self->materials, USER_MATERIAL_PARSE, frame, 0x48, response);
                if (!self->materials.ptr) return false;
                parseInto(&self->medals, USER_MEDAL_PARSE, frame, 0x40, response);
                if (!self->medals.ptr) return false;
                parseInto(&self->skills, USER_SKILL_PARSE, frame, 0x38, response);
                if (!self->skills.ptr) return false;
                parseInto(&self->titles, USER_TITLE_PARSE, frame, 0x30, response);
                if (!self->titles.ptr) return false;
                parseInto(&self->keyblades, USER_KEYBLADE_PARSE, frame, 0x28, response);
                return self->keyblades.ptr != 0;
            };
            if (parseAll()) {
                Property::init(&self->property);
            } else {
                deleteVirtual(self);
                self = nullptr;
            }
        }
    }
    rw::call(CLEAR_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A4ED4, "hole::network::api::StageClear::parse", StageClear::parse);

StageClear *StageClear::destroy(StageClear *self) {
    self->vtable = VTABLE;
    // the eleven shared_ptrs from the last: release each control block
    for (uint32_t off = 0x78; off >= 0x28; off -= 8) {
        uint32_t ctrl = rw::get<uint32_t>(self, off);
        if (ctrl) rw::call(stage::fn::release_shared, {ctrl});
    }
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x4B5778, "hole::network::api::StageClear::~StageClear", StageClear::destroy);

uint32_t StageClear::destroyDelete(StageClear *self) {
    return rw::call(guest::operator_delete, {rw::guest_addr(destroy(self))});
}
RW_FUNCTION(0x4B5810, "hole::network::api::StageClear::~StageClear (deleting)", StageClear::destroyDelete);

}  // namespace hole::network::api
