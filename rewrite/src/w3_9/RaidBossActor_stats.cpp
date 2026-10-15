// RaidBossActor: HP from the raid start response, the buffs / conditions from the master data, damage totals.
#include "RaidBossActor_common.hpp"

using namespace rba;

namespace {
constexpr uint32_t UDIVSI3 = 0x40E518;     // __udivsi3 (PLT), called like the original (division by zero included)
// RaidStart (a SafeBuffer of 0xA8 bytes): part count at 40, then 20-byte part records from 44 (HP at +8)
constexpr uint32_t RAIDSTART_PART_COUNT = 40;
constexpr uint32_t RAIDSTART_PART_HP = 52;
constexpr uint32_t RAIDSTART_PART_SIZE = 20;

// A shared_ptr<const master::RaidEnemy> on the guest stack: create(id) ... release.
uint32_t createEnemy(uint32_t at, int32_t id) {
    rw::call(fn::master_RaidEnemy_create, {at, uint32_t(id)});
    return at;
}
}  // namespace

// Each part's HP from the raid start response (the body first, then the parts listed by the RaidEnemy); a part
// with HP left shows its "Break" movie no more; the total attack damage restarts at 0.
void RaidBossActor::set_strat_hp(RaidBossActor *self) {
    uint32_t a = addr(self);
    Frame f(0x90);
    uint32_t enemy = createEnemy(f.addr(0x58), self->raidEnemyId);
    int32_t validParts = get32(ld32(enemy), RE_VALID_PARTS);
    int32_t weakness = get32(ld32(enemy), RE_WEAKNESS);
    uint32_t copy = f.addr(0x50);
    rw::save<uint32_t>(copy, ld32(enemy));
    rw::save<uint32_t>(copy + 4, ld32(enemy + 4));
    uint32_t body = a + 948 + 36u * uint32_t(weakness - 1);
    addRef(ld32(enemy + 4));
    int32_t level = self->level;
    int32_t id = self->raidEnemyId;
    uint32_t hp = rw::call(self_fn::get_api_hp, {a + 2216, 0});
    rw::call(RaidParts::SET_MASTER_PARAM, {body, copy, uint32_t(level), uint32_t(id), hp});
    releaseShared(ld32(copy + 4));
    set32(body, RaidParts::TOTAL_ATTACK_DAMAGE, 0);
    for (int32_t k = 0; k < validParts; k++) {
        int32_t partId = get32(ld32(enemy), RE_PARTS + 4u * uint32_t(k));
        uint32_t pe = createEnemy(f.addr(0x48), partId);
        int32_t w = get32(ld32(pe), RE_WEAKNESS);
        uint32_t partHp = rw::call(self_fn::get_api_hp, {ld32(pe), uint32_t(k + 1)});   // (`this` unused)
        uint32_t pcopy = f.addr(0x40);
        rw::save<uint32_t>(pcopy, ld32(pe));
        rw::save<uint32_t>(pcopy + 4, ld32(pe + 4));
        uint32_t index = uint32_t(w) - 1;
        uint32_t part = a + 948 + 36u * index;
        addRef(ld32(pe + 4));
        rw::call(RaidParts::SET_MASTER_PARAM, {part, pcopy, uint32_t(self->level.get()), uint32_t(partId), partHp});
        releaseShared(ld32(pcopy + 4));
        if (partHp) rw::save<uint8_t>(self->boss.addr + BOSS_POINT_ENABLED + index, 0);
        set32(part, RaidParts::TOTAL_ATTACK_DAMAGE, 0);
        releaseShared(ld32(pe + 4));
    }
    releaseShared(ld32(enemy + 4));
}
RW_FUNCTION(0x5F400C, "RaidBossActor::set_strat_hp", RaidBossActor::set_strat_hp);

// The boss's buffs from its RaidEnemy row (BuffInfoDispParam by value: 8 buff values, turns, count, two
// condition types (-1 = none), condition turns, condition count).
void RaidBossActor::set_buff_param(RaidBossActor *self, int32_t attack, int32_t defense, int32_t attackPower,
                                   int32_t defensePower, int32_t attackSpeed, int32_t defenseSpeed,
                                   int32_t attackMagic, int32_t defenseMagic, int32_t turns, int32_t count,
                                   int32_t condition0, int32_t condition1, int32_t conditionTurns,
                                   int32_t conditionCount) {
    uint32_t a = addr(self);
    if (uint32_t(turns) + uint32_t(count)) {
        const int32_t values[8] = {attack, defense, attackPower, defensePower,
                                   attackSpeed, defenseSpeed, attackMagic, defenseMagic};
        for (uint32_t k = 0; k < 8; k++)   // (attack, defense) per target 0..3
            rw::call(fn::IBuffProtocol_buff_add,
                     {a + 684, uint32_t(values[k]), uint32_t(turns), uint32_t(count), k & 1, k >> 1});
    }
    if (uint32_t(conditionTurns) + uint32_t(conditionCount)) {
        if (condition0 != -1)
            rw::call(self_fn::raidAddCondition, {a, uint32_t(condition0), uint32_t(conditionTurns), uint32_t(conditionCount)});
        if (condition1 != -1)
            rw::call(self_fn::raidAddCondition, {a, uint32_t(condition1), uint32_t(conditionTurns), uint32_t(conditionCount)});
    }
}
RW_FUNCTION(0x5F4260, "RaidBossActor::set_buff_param", RaidBossActor::set_buff_param);

// The same for one part's buffs (the conditions are ignored).
void RaidBossActor::set_buff_param_parts(RaidBossActor *self, int32_t attack, int32_t defense, int32_t attackPower,
                                         int32_t defensePower, int32_t attackSpeed, int32_t defenseSpeed,
                                         int32_t attackMagic, int32_t defenseMagic, int32_t turns, int32_t count,
                                         int32_t, int32_t, int32_t, int32_t, int32_t part) {
    if (!(uint32_t(turns) + uint32_t(count))) return;
    uint32_t buffs = addr(self) + 720 + 36u * uint32_t(part);
    const int32_t values[8] = {attack, defense, attackPower, defensePower,
                               attackSpeed, defenseSpeed, attackMagic, defenseMagic};
    for (uint32_t k = 0; k < 8; k++)
        rw::call(fn::IBuffProtocol_buff_add, {buffs, uint32_t(values[k]), uint32_t(turns), uint32_t(count), k & 1, k >> 1});
}
RW_FUNCTION(0x5F43B2, "RaidBossActor::set_buff_param_parts", RaidBossActor::set_buff_param_parts);

// HP of part `index` in the raid start response (APIManager's RaidStart), 0 without one. `this` is not used.
uint32_t RaidBossActor::get_api_hp(uint32_t, int32_t index) {
    uint32_t api = rw::call(fn::APIManager_getInstance, {});
    uint32_t ctrl = ld32(api + APIMANAGER_RAID_START + 4);
    uint32_t start = ld32(api + APIMANAGER_RAID_START);
    addRef(ctrl);
    uint32_t hp = 0;
    // (the original decodes the record block into its frame; indexes past the 6 records read other frame bytes)
    if (start) hp = getu(start, RAIDSTART_PART_HP + RAIDSTART_PART_SIZE * uint32_t(index));
    releaseShared(ctrl);
    return hp;
}
RW_FUNCTION(0x5F44BC, "RaidBossActor::get_api_hp", RaidBossActor::get_api_hp);

// HP the raid team took off the boss since the raid started (start HP - HP now, over the body and the parts).
int32_t RaidBossActor::calc_total_real_damage(RaidBossActor *self) {
    Frame f(0x70);
    uint32_t enemy = createEnemy(f.addr(0x30), self->raidEnemyId);
    int32_t validParts = get32(ld32(enemy), RE_VALID_PARTS);
    int32_t w = get32(ld32(enemy), RE_WEAKNESS);
    uint32_t start = rw::call(self_fn::get_api_hp, {ld32(enemy), uint32_t(w - 1)});   // (`this` unused)
    uint32_t total = start - getu(partAddr(self, w - 1), RaidParts::HP);
    for (int32_t k = 0; k < validParts; k++) {
        int32_t id = get32(ld32(enemy), RE_PARTS + 4u * uint32_t(k));
        uint32_t pe = createEnemy(f.addr(0x28), id);
        int32_t pw = get32(ld32(pe), RE_WEAKNESS);
        uint32_t partStart = rw::call(self_fn::get_api_hp, {ld32(pe), uint32_t(pw - 1)});
        total = partStart + total - getu(partAddr(self, pw - 1), RaidParts::HP);
        releaseShared(ld32(pe + 4));
    }
    releaseShared(ld32(enemy + 4));
    return int32_t(total);
}
RW_FUNCTION(0x5F45D8, "RaidBossActor::calc_total_real_damage", RaidBossActor::calc_total_real_damage);

// Total attack damage over the body and the parts.
int32_t RaidBossActor::calc_total_attack_damage(RaidBossActor *self) {
    Frame f(0x60);
    uint32_t enemy = createEnemy(f.addr(0x28), self->raidEnemyId);
    int32_t validParts = get32(ld32(enemy), RE_VALID_PARTS);
    int32_t w = get32(ld32(enemy), RE_WEAKNESS);
    uint32_t total = getu(partAddr(self, w - 1), RaidParts::TOTAL_ATTACK_DAMAGE);
    for (int32_t k = 0; k < validParts; k++) {
        int32_t id = get32(ld32(enemy), RE_PARTS + 4u * uint32_t(k));
        uint32_t pe = createEnemy(f.addr(0x20), id);
        int32_t pw = get32(ld32(pe), RE_WEAKNESS);
        total += getu(partAddr(self, pw - 1), RaidParts::TOTAL_ATTACK_DAMAGE);
        releaseShared(ld32(pe + 4));
    }
    releaseShared(ld32(enemy + 4));
    return int32_t(total);
}
RW_FUNCTION(0x5F4760, "RaidBossActor::calc_total_attack_damage", RaidBossActor::calc_total_attack_damage);

// Parts that still have HP in the raid start response show their "Break" movie no more.
void RaidBossActor::api_reflect(RaidBossActor *self) {
    uint32_t api = rw::call(fn::APIManager_getInstance, {});
    uint32_t ctrl = ld32(api + APIMANAGER_RAID_START + 4);
    uint32_t start = ld32(api + APIMANAGER_RAID_START);
    addRef(ctrl);
    if (start) {
        Frame f(0x18);
        uint32_t players = f.addr(0x4);              // a copy of the response's player list (unused)
        rw::call(fn::PlayerVector_copy, {players, start + RAIDSTART_PLAYERS});
        for (uint32_t k = 0; k < getu(start, RAIDSTART_PART_COUNT); k++) {
            if (getu(start, RAIDSTART_PART_HP + RAIDSTART_PART_SIZE * k))
                rw::save<uint8_t>(self->boss.addr + BOSS_POINT_ENABLED + k, 0);
        }
        rw::call(fn::PlayerVector_dtor, {players});
    }
    releaseShared(ctrl);
}
RW_FUNCTION(0x5F4914, "RaidBossActor::api_reflect", RaidBossActor::api_reflect);

// The lowest HP / max HP among the body and the live parts (integer division, as a float: 0 or 1).
float RaidBossActor::MinHPRateRaidTeam(RaidBossActor *self) {
    Frame f(0x80);
    uint32_t enemy = createEnemy(f.addr(0x3C), self->raidEnemyId);
    int32_t validParts = get32(ld32(enemy), RE_VALID_PARTS);
    int32_t w = get32(ld32(enemy), RE_WEAKNESS);
    uint32_t body = partAddr(self, w - 1);
    uint32_t hp = getu(body, RaidParts::HP);
    uint32_t maxHp = getu(body, RaidParts::MAX_HP);
    float lowest = float(rw::call(UDIVSI3, {hp, maxHp}));
    for (int32_t k = 0; k < validParts; k++) {
        int32_t id = get32(ld32(enemy), RE_PARTS + 4u * uint32_t(k));
        uint32_t pe = createEnemy(f.addr(0x34), id);
        int32_t pw = get32(ld32(pe), RE_WEAKNESS);
        uint32_t part = partAddr(self, pw - 1);
        if (getu(part, RaidParts::HP)) {
            uint32_t partHp = getu(part, RaidParts::HP);
            uint32_t partMax = getu(part, RaidParts::MAX_HP);
            float rate = float(rw::call(UDIVSI3, {partHp, partMax}));
            if (rate < lowest) lowest = rate;
        }
        releaseShared(ld32(pe + 4));
    }
    releaseShared(ld32(enemy + 4));
    return lowest;
}
RW_FUNCTION(0x5F4A58, "RaidBossActor::MinHPRateRaidTeam", RaidBossActor::MinHPRateRaidTeam);

// Seconds until the next day boundary of the raid's end date (1..86400).
int32_t RaidBossActor::getRemainingTime(const RaidBossActor *self) {
    Frame f(0x18);
    uint32_t date = f.addr(8);
    rw::save<uint8_t>(date + 7, 0);
    SafeBuffer::read(&self->property, rw::host_ptr<uint8_t>(date), self->property.data.get() + Property::END_DATE, 8,
                     Property::END_DATE);
    int32_t m = int32_t(rw::call(fn::Date_calculateIntervalSinceNow, {date})) % 86400;
    uint32_t top = m > 0 ? uint32_t(m) + 86399 : 86399u;
    return int32_t(top - (top - uint32_t(m)) % 86400u);
}
RW_FUNCTION(0x5F56D8, "RaidBossActor::getRemainingTime", RaidBossActor::getRemainingTime);

int32_t RaidBossActor::getNumberOfParts(const RaidBossActor *self) {
    return get32(addr(&self->property), Property::PART_COUNT);
}
RW_FUNCTION(0x5F576C, "RaidBossActor::getNumberOfParts", RaidBossActor::getNumberOfParts);
