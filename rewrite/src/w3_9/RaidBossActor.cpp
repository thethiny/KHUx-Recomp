// RaidBossActor: life cycle (create, constructor, init, destructor).
#include "RaidBossActor_common.hpp"

using namespace rba;

// ------------------------------------------------------------------------------------------------ life cycle
RaidBossActor *RaidBossActor::create(int32_t raidEnemyId, int32_t level) {
    uint32_t p = guest::newObject(SIZE);
    rw::call(self_fn::construct, {p});
    if (rw::call(self_fn::init, {p, uint32_t(raidEnemyId), uint32_t(level)}) != 1) return nullptr;  // (leaks p)
    rw::call(fn::Ref_autorelease, {p});
    return rw::host_ptr<RaidBossActor>(p);
}
RW_FUNCTION(0x5F1EC8, "RaidBossActor::create(int, int)", RaidBossActor::create);

RaidBossActor *RaidBossActor::construct(RaidBossActor *self) {
    uint32_t a = addr(self);
    rw::call(fn::MapNode_construct, {a});
    self->vtable = VTABLE;
    uint32_t p = rw::call(fn::SafeBuffer_construct, {a + 648, Property::SIZE, 1});
    rw::save<uint32_t>(p, Property::VTABLE);
    self->property.statusVtable = Property::STATUS_VTABLE;
    self->property.buffVtable = Property::BUFF_VTABLE;
    for (uint32_t k = 0; k < 6; k++) {
        uint32_t q = rw::call(fn::SafeBuffer_construct, {a + 688 + 36 * k, 0x300, 1});
        rw::save<uint32_t>(q, 0xF96380);          // PartsProperty::Property vtable
        rw::save<uint32_t>(q + 32, 0xF9639C);     // its IBuffProtocol vtable
    }
    rw::call(fn::Vec2_ctor, {a + 904});
    rw::call(fn::Vec2_ctor, {a + 912});
    self->statusIndicator = 0;
    self->fieldNode = 0;
    self->statusBits = 0;
    for (uint32_t k = 0; k < 6; k++) {
        uint32_t q = rw::call(fn::SafeBuffer_construct, {a + 948 + 36 * k, 0x60, 1});
        rw::save<uint32_t>(q, RaidParts::VTABLE);
        rw::save<uint32_t>(q + 32, 0);
    }
    for (uint32_t k = 0; k < 11; k++) {          // the records' std::string members (at +12 of each)
        rw::save<uint32_t>(a + 1176 + 72 * k, 0);
        rw::save<uint32_t>(a + 1176 + 72 * k + 4, 0);
        rw::save<uint32_t>(a + 1176 + 72 * k + 8, 0);
    }
    rw::call(fn::aeabi_memset, {a + 1960, 0x1C, 0});   // dropItem, the two cocos2d::Vectors
    for (uint32_t k = 0; k < 6; k++) {
        rw::call(fn::SafeBuffer_construct, {a + 1988 + 32 * k, 8, 1});
        rw::save<uint32_t>(a + 1988 + 32 * k, PrizeParam::VTABLE);
    }
    rw::call(fn::aeabi_memset, {a + 2180, 0x18, 0});   // skill, the markers map
    self->markerMaxLoad = 1.0f;
    self->field_8AC = 0;
    return self;
}
RW_FUNCTION(0x5F1F20, "RaidBossActor::RaidBossActor()", RaidBossActor::construct);

RaidBossActor *RaidBossActor::destruct(RaidBossActor *self) {
    uint32_t a = addr(self);
    self->vtable = VTABLE;
    if (self->statusIndicator) rw::call(fn::Ref_release, {self->statusIndicator});
    rw::call(fn::markers_dtor, {a + 2188});
    releaseShared(self->skill.ctrl);
    for (int32_t k = 5; k >= 0; k--) rw::call(fn::SafeBuffer_destruct, {a + 1988 + 32 * uint32_t(k)});
    rw::call(fn::VectorMapPrizes_dtor, {a + 1976});
    rw::call(fn::VectorMapPrize_dtor, {a + 1964});
    for (int32_t k = 10; k >= 0; k--) {
        uint32_t s = a + 1176 + 72 * uint32_t(k);
        if (ld8(s) & 1) guest::deleteObject(ld32(s + 8));
    }
    for (int32_t k = 5; k >= 0; k--) rw::call(fn::SafeBuffer_destruct, {a + 948 + 36 * uint32_t(k)});
    rw::call(fn::Vec2_dtor, {a + 912});
    rw::call(fn::Vec2_dtor, {a + 904});
    for (int32_t k = 5; k >= 0; k--) rw::call(fn::SafeBuffer_destruct, {a + 688 + 36 * uint32_t(k)});
    rw::call(fn::SafeBuffer_destruct, {a + 648});
    rw::call(fn::MapNode_destruct, {a});
    return self;
}
RW_FUNCTION(0x5F2A1C, "RaidBossActor::~RaidBossActor()", RaidBossActor::destruct);

void RaidBossActor::destructDelete(RaidBossActor *self) {
    guest::deleteObject(rw::call(self_fn::destruct, {addr(self)}));
}
RW_FUNCTION(0x5F2BC4, "RaidBossActor::~RaidBossActor() (deleting)", RaidBossActor::destructDelete);

// ------------------------------------------------------------------------------------------------ init
// raidEnemyId / level both 0: a demo boss (raid enemy 1, level 1) whose HP comes from the master data; otherwise
// the HP of each part comes from the raid start response (set_strat_hp).
uint32_t RaidBossActor::init(RaidBossActor *self, int32_t raidEnemyId, int32_t level) {
    uint32_t a = addr(self);
    bool fromApi = (uint32_t(raidEnemyId) | uint32_t(level)) != 0;
    self->level = fromApi ? level : 1;
    self->raidEnemyId = fromApi ? raidEnemyId : 1;
    uint32_t boss = rw::call(fn::RaidBoss_create, {uint32_t(self->raidEnemyId.get() % 10000)});
    self->boss = rw::Ptr<RaidBoss>{boss};
    if (!boss) return 0;
    rw::vcall(self, node_slot::ADD_CHILD, {a, boss});
    rw::call(fn::SafeBuffer_zerofill, {a + 648});

    Frame f(0x648);                              // the original frame (push 20 + 0x634)
    const uint32_t emptySkill = f.addr(0xF0);    // shared_ptr<const master::Skill>() for setPrizeParam
    for (uint32_t k = 0; k < 6; k++) {
        uint32_t buffs = a + 720 + 36 * k;      // the part's IBuffProtocol
        for (uint32_t target = 0; target < 4; target++) {
            rw::call(fn::IBuffProtocol_buff_clear, {buffs, 0, target});
            rw::call(fn::IBuffProtocol_buff_clear, {buffs, 1, target});
        }
        rw::save<uint32_t>(emptySkill, 0);
        rw::save<uint32_t>(emptySkill + 4, 0);
        rw::call(self_fn::setPrizeParam, {a, k, rw::fbits(1.0f), rw::fbits(1.0f), emptySkill});
        set32(a + 948 + 36 * k, RaidParts::HP, 0);
    }

    // the stage state's raid counters start at 0 (the shared_ptr is held to the end)
    uint32_t app = rw::call(fn::AppState_getInstance, {});
    uint32_t stage = ld32(app + APPSTATE_STAGE_STATE);
    uint32_t stageCtrl = ld32(app + APPSTATE_STAGE_STATE + 4);
    addRef(stageCtrl);
    set32(stage, STAGESTATE_RAID_44, 0);
    set32(ld32(rw::call(fn::AppState_getInstance, {}) + APPSTATE_STAGE_STATE), STAGESTATE_RAID_48, 0);

    const uint32_t enemy = f.addr(0xE0);         // shared_ptr<const master::RaidEnemy>
    rw::call(fn::master_RaidEnemy_create, {enemy, uint32_t(self->raidEnemyId.get())});
    // setName(name): StringBuffer(name) -> std::string
    {
        uint32_t setName = ld32(self->vtable + 340);
        uint32_t nameBuf = f.addr(0x1A8), text = f.addr(0x22C), str = f.addr(0xD4);
        uint32_t row = ld32(enemy);
        SafeBuffer::read(sb(row), rw::host_ptr<uint8_t>(nameBuf), sb(row)->data.get() + RE_NAME, 0x81, RE_NAME);
        rw::call(fn::StringBuffer_ctor, {text, nameBuf});
        uint32_t len = rw::call(fn::strlen_, {text});
        rw::call(fn::string_init, {str, text, len});
        rw::call(setName, {a, str});
        if (ld8(str) & 1) guest::deleteObject(ld32(str + 8));
    }
    rw::call(fn::AudioReplaceGameMainBGM, {getu(ld32(enemy), RE_BGM_RAID), 500});
    int32_t validParts = get32(ld32(enemy), RE_VALID_PARTS);
    set32(a + 648, Property::PART_COUNT, int32_t(uint32_t(validParts) + 1));
    float height = float(get32(ld32(enemy), RE_DRAW_HEIGHT)) / 10000.0f;
    self->boss->scale = height;
    rw::vcall(self, node_slot::SET_SCALE, {a, rw::fbits(height)});

    if (fromApi) {
        rw::call(self_fn::set_strat_hp, {a});
    } else {
        // demo: the body at level hp 450, the first two parts at 0 and 300
        uint32_t copy = f.addr(0xC8);
        rw::save<uint32_t>(copy, ld32(enemy));
        rw::save<uint32_t>(copy + 4, ld32(enemy + 4));
        addRef(ld32(enemy + 4));
        rw::call(RaidParts::SET_MASTER_PARAM, {a + 948, copy, uint32_t(self->level.get()),
                                               uint32_t(self->raidEnemyId.get()), 450});
        releaseShared(ld32(copy + 4));
        self->boss->pointEnabled[0] = 0;

        int32_t part0 = get32(ld32(enemy), RE_PARTS);
        const uint32_t e0 = f.addr(0xC0);
        rw::call(fn::master_RaidEnemy_create, {e0, uint32_t(part0)});
        int32_t w0 = get32(ld32(e0), RE_WEAKNESS);
        copy = f.addr(0xB8);
        rw::save<uint32_t>(copy, ld32(enemy));
        rw::save<uint32_t>(copy + 4, ld32(enemy + 4));
        uint32_t partA = a + 912 + 36u * uint32_t(w0);
        addRef(ld32(enemy + 4));
        rw::call(RaidParts::SET_MASTER_PARAM, {partA, copy, uint32_t(self->level.get()), uint32_t(part0), 0});
        releaseShared(ld32(copy + 4));

        int32_t part1 = get32(ld32(enemy), RE_PARTS + 4);
        const uint32_t e1 = f.addr(0xB0);
        rw::call(fn::master_RaidEnemy_create, {e1, uint32_t(part1)});
        int32_t w1 = get32(ld32(e1), RE_WEAKNESS);
        copy = f.addr(0xA8);
        rw::save<uint32_t>(copy, ld32(enemy));
        rw::save<uint32_t>(copy + 4, ld32(enemy + 4));
        uint32_t index1 = uint32_t(w1) - 1;
        uint32_t partB = a + 948 + 36u * index1;
        addRef(ld32(enemy + 4));
        rw::call(RaidParts::SET_MASTER_PARAM, {partB, copy, uint32_t(self->level.get()), uint32_t(part1), 300});
        releaseShared(ld32(copy + 4));
        rw::save<uint8_t>(self->boss.addr + BOSS_POINT_ENABLED + index1, 0);
        releaseShared(ld32(e1 + 4));
        releaseShared(ld32(e0 + 4));
    }

    // the boss's buffs and bad status
    uint32_t row = ld32(enemy);
    int32_t bufAttack = get32(row, RE_BUF_ATTACK);
    int32_t bufDefense = get32(row, RE_BUF_DEFENSE);
    int32_t bufAttackPower = get32(row, RE_BUF_ATTACK_POWER);
    int32_t bufDefensePower = get32(row, RE_BUF_DEFENSE_POWER);
    int32_t bufAttackSpeed = get32(row, RE_BUF_ATTACK_SPEED);
    int32_t bufDefenseSpeed = get32(row, RE_BUF_DEFENSE_SPEED);
    int32_t bufAttackMagic = get32(row, RE_BUF_ATTACK_MAGIC);
    int32_t bufDefenseMagic = get32(row, RE_BUF_DEFENSE_MAGIC);
    int32_t bufTurn = get32(row, RE_BUF_TURN);
    int32_t bufCount = get32(row, RE_BUF_COUNT);
    int32_t ailment0 = get32(row, RE_STATUS_AILMENTS);
    int32_t ailment1 = get32(ld32(enemy), RE_STATUS_AILMENTS + 4);
    int32_t condition0 = -1, condition1 = -1;
    const uint32_t flag = f.addr(0xA7);
    if (ailment0)
        condition0 = int32_t(rw::call(fn::IStatusProtocol_masterId2ConditionType, {a + 680, uint32_t(ailment0), flag}));
    if (ailment1)
        condition1 = int32_t(rw::call(fn::IStatusProtocol_masterId2ConditionType, {a + 680, uint32_t(ailment1), flag}));
    int32_t ailmentTurn = get32(ld32(enemy), RE_STATUS_AILMENTS_TURN);
    int32_t ailmentCount = get32(ld32(enemy), RE_STATUS_AILMENTS_COUNT);
    set_buff_param(self, bufAttack, bufDefense, bufAttackPower, bufDefensePower, bufAttackSpeed, bufDefenseSpeed,
                   bufAttackMagic, bufDefenseMagic, bufTurn, bufCount, condition0, condition1, ailmentTurn,
                   ailmentCount);

    // each part's own buffs: the defense-side buff values, turns and count of its RaidEnemy row (the two
    // condition slots 1 / 2 are not used for parts)
    for (int32_t k = 0; k < validParts; k++) {
        int32_t id = get32(ld32(enemy), RE_PARTS + 4u * uint32_t(k));
        const uint32_t pe = f.addr(0x9C);
        rw::call(fn::master_RaidEnemy_create, {pe, uint32_t(id)});
        uint32_t prow = ld32(pe);
        int32_t weakness = get32(prow, RE_WEAKNESS);
        int32_t pDefense = get32(ld32(pe), RE_BUF_DEFENSE);
        int32_t pDefensePower = get32(ld32(pe), RE_BUF_DEFENSE_POWER);
        int32_t pDefenseSpeed = get32(ld32(pe), RE_BUF_DEFENSE_SPEED);
        int32_t pDefenseMagic = get32(ld32(pe), RE_BUF_DEFENSE_MAGIC);
        int32_t pTurn = get32(ld32(pe), RE_BUF_TURN);
        int32_t pCount = get32(ld32(pe), RE_BUF_COUNT);
        set_buff_param_parts(self, 0, pDefense, 0, pDefensePower, 0, pDefenseSpeed, 0, pDefenseMagic, pTurn, pCount,
                             1, 2, 0, 0, weakness - 1);
        releaseShared(ld32(pe + 4));
    }
    releaseShared(ld32(enemy + 4));
    releaseShared(stageCtrl);
    return 1;
}
RW_FUNCTION(0x5F21CC, "RaidBossActor::init(int, int)", RaidBossActor::init);
