// RaidBossActor: damage to the boss's parts and the part accessors.
#include "RaidBossActor_common.hpp"

using namespace rba;

namespace {
// DisplayDamageNumber::disp_dmgNum(damage, node, Vec2 position, -64, flags..., 600, scale) for a part's attack
void showDamageNumber(const RaidBossActor *self, int32_t damage, uint32_t node, uint32_t marker, int32_t part,
                      float scale, uint32_t posAddr) {
    uint32_t p = rw::vcall(rw::host_ptr<void>(marker), node_slot::GET_POSITION, {marker});
    rw::call(fn::Vec2_copy, {posAddr, p});
    uint32_t flags = self->parts[part].attackFlags;
    rw::call(fn::DisplayDamageNumber_disp_dmgNum,
             {uint32_t(damage), node, posAddr, rw::fbits(-64.0f), (flags & 2) >> 1, (flags & 4) >> 2, 0, flags & 1,
              600, rw::fbits(scale)});
    rw::call(fn::Vec2_dtor, {posAddr});
}
}  // namespace

// Damage one part (pair<const int, BattleMarker*> of the markers map): the HP left goes to the part's marker (the
// body: the status indicator). Then, when the part is down and its "Break" movie not yet enabled, enable it and
// make the marker hide completely; returns 1 when that happened to a part other than the body.
uint32_t RaidBossActor::setEnemyPartDamage(RaidBossActor *self, const uint32_t *entry, uint32_t damage,
                                           const guest::SharedPtr *burst) {
    uint32_t a = addr(self);
    uint32_t e = addr(entry);
    if (damage) {
        Frame f(0x30);
        int32_t part = ldi(e);
        uint32_t ctrl;
        if (part) {
            uint32_t marker = ld32(e + 4);
            uint32_t copy = f.addr(0x14);
            rw::save<uint32_t>(copy, burst->obj);
            ctrl = burst->ctrl;
            rw::save<uint32_t>(copy + 4, ctrl);
            addRef(ctrl);
            uint32_t hp = rw::call(self_fn::addDamage, {a, uint32_t(part), damage, copy});
            rw::call(fn::BattleMarker_setHP, {marker, hp});
        } else {
            uint32_t indicator = self->statusIndicator;
            uint32_t copy = f.addr(0x1C);
            rw::save<uint32_t>(copy, burst->obj);
            ctrl = burst->ctrl;
            rw::save<uint32_t>(copy + 4, ctrl);
            addRef(ctrl);
            uint32_t hp = rw::call(self_fn::addDamage, {a, 0, damage, copy});
            rw::call(fn::RaidBossStatusIndicator_setRaidBossHP, {indicator, hp});
        }
        releaseShared(ctrl);
    }
    int32_t part = ldi(e);
    if (ld8(self->boss.addr + uint32_t(part) + BOSS_POINT_ENABLED)) return 0;
    if (partGet(self, part, RaidParts::HP)) return 0;
    rw::save<uint8_t>(self->boss.addr + uint32_t(ldi(e)) + BOSS_POINT_ENABLED, 1);
    rw::save<uint8_t>(ld32(e + 4) + MARKER_RING_ONLY, 0);
    return ldi(e) != 0 ? 1 : 0;
}
RW_FUNCTION(0x5F2BD0, "RaidBossActor::setEnemyPartDamage", RaidBossActor::setEnemyPartDamage);

// Apply damage to a part: the HP goes down (not below 0), the dealt and total damage go up, the stage state notes
// the attack; a crushed part drops its crushing prizes, otherwise the attack prizes (scaled by the burst's rate).
// Returns the HP left.
uint32_t RaidBossActor::addDamage(RaidBossActor *self, int32_t part, uint32_t damage, const guest::SharedPtr *burst) {
    uint32_t a = addr(self);
    uint32_t p = partAddr(self, part);
    uint32_t hp = getu(p, RaidParts::HP);
    if (!hp) return 0;
    uint32_t dealt = hp > damage ? damage : hp;
    uint32_t left = hp - dealt;
    set32(p, RaidParts::HP, int32_t(left));
    uint32_t dealtSoFar = getu(p, RaidParts::DAMAGE);
    uint32_t total = getu(p, RaidParts::TOTAL_ATTACK_DAMAGE);
    set32(p, RaidParts::DAMAGE, int32_t(dealtSoFar + dealt));
    set32(p, RaidParts::TOTAL_ATTACK_DAMAGE, int32_t(total + damage));
    rw::call(fn::StageState_checkMaxAttack, {ld32(rw::call(fn::AppState_getInstance, {}) + APPSTATE_STAGE_STATE), damage});
    if (hp == dealt) {
        rw::call(self_fn::addCrushingPrizes, {a, uint32_t(part)});
        rw::call(self_fn::addCrushingPrizesLux, {a, uint32_t(part), hp});
        return 0;
    }
    uint32_t rate = 1;
    if (burst->obj) rate = getu(burst->obj, 356);    // master::Burst data +356
    rw::call(self_fn::addAttackPrizes, {a, uint32_t(part), rate});
    rw::call(self_fn::addAttackPrizesLux, {a, uint32_t(part), dealt});
    return left;
}
RW_FUNCTION(0x5F2D20, "RaidBossActor::addDamage", RaidBossActor::addDamage);

// Every marker shows its part's HP again (the body: the status indicator).
void RaidBossActor::updateDamageAll(RaidBossActor *self) {
    for (uint32_t node = self->markerFirst; node; node = ld32(node)) {
        int32_t part = ldi(node + 8);
        if (part) {
            uint32_t marker = ld32(node + 12);
            rw::call(fn::BattleMarker_setHP, {marker, getu(partAddr(self, part), RaidParts::HP)});
        } else {
            uint32_t indicator = self->statusIndicator;
            rw::call(fn::RaidBossStatusIndicator_setRaidBossHP, {indicator, getu(partAddr(self, 0), RaidParts::HP)});
        }
    }
}
RW_FUNCTION(0x5F2F2C, "RaidBossActor::updateDamageAll", RaidBossActor::updateDamageAll);

// Apply the current attack (each part's ATTACK_DAMAGE): to the targeted part, or to every part hit; shows the
// damage numbers (larger for a critical hit). Returns how many parts went down (targeted: setEnemyPartDamage's
// result); hitting the body directly also refreshes the boss's bad status display.
uint32_t RaidBossActor::setEnemyDamage(RaidBossActor *self, const guest::SharedPtr *burst, uint32_t critical) {
    uint32_t a = addr(self);
    Frame f(0xB8);   // the original's frame: the click map copy's buckets point at the map object
    const uint32_t clickMap = f.addr(0x5C), position = f.addr(0x70);
    uint32_t p = rw::vcall(self, node_slot::GET_POSITION, {a});
    rw::call(fn::Vec2_copy, {position, p});
    uint32_t points = rw::call(fn::RaidBoss_getClickPoints, {self->boss.addr});
    rw::call(fn::clickMap_copy, {clickMap, points});
    uint32_t result = 0;
    if (self->targetPart >= 0) {
        uint32_t node = rw::call(fn::markers_find, {a + MARKERS, a + 920});
        if (node) {
            uint32_t entry = node + 8;
            int32_t key = ldi(entry);
            int32_t hp = partGet(self, key, RaidParts::HP);
            int32_t damage = partGet(self, ldi(entry), RaidParts::ATTACK_DAMAGE);
            uint32_t copy = f.addr(0x4C);
            rw::save<uint32_t>(copy, burst->obj);
            uint32_t ctrl = burst->ctrl;
            rw::save<uint32_t>(copy + 4, ctrl);
            addRef(ctrl);
            result = setEnemyPartDamage(self, rw::host_ptr<uint32_t>(entry), uint32_t(damage),
                                        rw::host_ptr<guest::SharedPtr>(copy));
            releaseShared(ctrl);
            if (hp) {
                int32_t shown = partGet(self, ldi(entry), RaidParts::ATTACK_DAMAGE);
                uint32_t fieldNode = self->fieldNode;
                int32_t part = ldi(node + 8);
                showDamageNumber(self, shown, fieldNode, ld32(node + 12), part, critical ? 1.2f : 1.0f,
                                 f.addr(0x78));
            }
            if (!ldi(entry)) rw::call(fn::FlashNode_reflection_status, {self->boss.addr, 0});
        }
    } else {
        uint32_t node = self->markerFirst;
        if (node) {
            float scale = critical ? 1.2f : 1.0f;
            for (; node; node = ld32(node)) {
                uint32_t entry = node + 8;
                int32_t key = ldi(entry);
                if (!self->partHit[key]) continue;
                int32_t hp = partGet(self, key, RaidParts::HP);
                int32_t damage = partGet(self, ldi(entry), RaidParts::ATTACK_DAMAGE);
                uint32_t copy = f.addr(0x54);
                rw::save<uint32_t>(copy, burst->obj);
                uint32_t ctrl = burst->ctrl;
                rw::save<uint32_t>(copy + 4, ctrl);
                addRef(ctrl);
                uint32_t down = setEnemyPartDamage(self, rw::host_ptr<uint32_t>(entry), uint32_t(damage),
                                                   rw::host_ptr<guest::SharedPtr>(copy));
                releaseShared(ctrl);
                if (down) result++;
                if (hp) {
                    int32_t shown = partGet(self, ldi(entry), RaidParts::ATTACK_DAMAGE);
                    int32_t part = ldi(node + 8);
                    uint32_t fieldNode = self->fieldNode;
                    showDamageNumber(self, shown, fieldNode, ld32(node + 12), part, scale, f.addr(0x8C));
                }
            }
        }
    }
    rw::call(fn::clickMap_dtor, {clickMap});
    rw::call(fn::Vec2_dtor, {position});
    return result;
}
RW_FUNCTION(0x5F2FE0, "RaidBossActor::setEnemyDamage", RaidBossActor::setEnemyDamage);

// ------------------------------------------------------------------------------------------------ part values
int32_t RaidBossActor::getCurrentHP(const RaidBossActor *self, int32_t part) { return partGet(self, part, RaidParts::HP); }
RW_FUNCTION(0x5F2ECC, "RaidBossActor::getCurrentHP", RaidBossActor::getCurrentHP);

int32_t RaidBossActor::getAttackDamage(const RaidBossActor *self, int32_t part) {
    return partGet(self, part, RaidParts::ATTACK_DAMAGE);
}
RW_FUNCTION(0x5F3430, "RaidBossActor::getAttackDamage", RaidBossActor::getAttackDamage);

uint32_t RaidBossActor::getAttackDamageFlags(const RaidBossActor *self, int32_t part) {
    return partAddr(self, part) + 32;
}
RW_FUNCTION(0x5F3490, "RaidBossActor::getAttackDamageFlags", RaidBossActor::getAttackDamageFlags);

int32_t RaidBossActor::getType(const RaidBossActor *self, int32_t part) { return partGet(self, part, RaidParts::TYPE); }
RW_FUNCTION(0x5F3AA4, "RaidBossActor::getType", RaidBossActor::getType);

int32_t RaidBossActor::getMaxHP(const RaidBossActor *self, int32_t part) { return partGet(self, part, RaidParts::MAX_HP); }
RW_FUNCTION(0x5F3B04, "RaidBossActor::getMaxHP", RaidBossActor::getMaxHP);

void RaidBossActor::setCurrentHP(const RaidBossActor *self, int32_t part, int32_t hp) {
    partSet(self, part, RaidParts::HP, hp);
}
RW_FUNCTION(0x5F3FAC, "RaidBossActor::setCurrentHP", RaidBossActor::setCurrentHP);

void RaidBossActor::setTotalAttackDamage(const RaidBossActor *self, int32_t part, int32_t damage) {
    partSet(self, part, RaidParts::TOTAL_ATTACK_DAMAGE, damage);
}
RW_FUNCTION(0x5F4578, "RaidBossActor::setTotalAttackDamage", RaidBossActor::setTotalAttackDamage);

int32_t RaidBossActor::getTotalAttackDamage(const RaidBossActor *self, int32_t part) {
    return partGet(self, part, RaidParts::TOTAL_ATTACK_DAMAGE);
}
RW_FUNCTION(0x5F48B4, "RaidBossActor::getTotalAttackDamage", RaidBossActor::getTotalAttackDamage);

int32_t RaidBossActor::getCurrentDamage(const RaidBossActor *self, int32_t part) {
    return partGet(self, part, RaidParts::DAMAGE);
}
RW_FUNCTION(0x5F4C5C, "RaidBossActor::getCurrentDamage", RaidBossActor::getCurrentDamage);

int32_t RaidBossActor::getDefense(const RaidBossActor *self, int32_t part) {
    return partGet(self, part, RaidParts::DEFENSE);
}
RW_FUNCTION(0x5F4CBC, "RaidBossActor::getDefense", RaidBossActor::getDefense);

void RaidBossActor::setCurrentDamage(const RaidBossActor *self, int32_t part, int32_t damage) {
    partSet(self, part, RaidParts::DAMAGE, damage);
}
RW_FUNCTION(0x5F4D1C, "RaidBossActor::setCurrentDamage", RaidBossActor::setCurrentDamage);

// The damage of the attack being shown, and its flags (null: all clear).
void RaidBossActor::setAttackDamage(const RaidBossActor *self, int32_t part, int32_t damage, const uint32_t *flags) {
    uint32_t p = partAddr(self, part);
    set32(p, RaidParts::ATTACK_DAMAGE, damage);
    if (flags) {
        rw::save<uint32_t>(p + 32, *flags);
    } else {
        Frame f(0x20);
        uint32_t it = f.addr(0x10);              // __bit_iterator{word pointer, bit 0}
        rw::save<uint32_t>(it, p + 32);
        rw::save<uint32_t>(it + 4, 0);
        rw::call(fn::bitset3_fill_false, {it, 3});
    }
}
RW_FUNCTION(0x5F4D7C, "RaidBossActor::setAttackDamage", RaidBossActor::setAttackDamage);

// (reads the body's attack whatever the part)
int32_t RaidBossActor::getAttack(const RaidBossActor *self, int32_t) { return partGet(self, 0, RaidParts::ATTACK); }
RW_FUNCTION(0x5F4DFC, "RaidBossActor::getAttack", RaidBossActor::getAttack);

#define RBA_PART_GETTER(ADDR, NAME, FIELD)                                                                    \
    int32_t RaidBossActor::NAME(const RaidBossActor *self, int32_t part) { return partGet(self, part, FIELD); } \
    RW_FUNCTION(ADDR, "RaidBossActor::" #NAME, RaidBossActor::NAME);
RBA_PART_GETTER(0x5F4E54, getExpAttack, RaidParts::EXP_ATTACK)
RBA_PART_GETTER(0x5F4EB4, getExpSuppress, RaidParts::EXP_SUPPRESS)
RBA_PART_GETTER(0x5F4F10, getMoneyAttack, RaidParts::MONEY_ATTACK)
RBA_PART_GETTER(0x5F4F70, getMoneySuppress, RaidParts::MONEY_SUPPRESS)
RBA_PART_GETTER(0x5F4FD0, getLuxAttack, RaidParts::LUX_ATTACK)
RBA_PART_GETTER(0x5F5030, getLuxSuppress, RaidParts::LUX_SUPPRESS)
RBA_PART_GETTER(0x5F5090, getBurstAttack, RaidParts::BURST_ATTACK)
RBA_PART_GETTER(0x5F50F0, getBurstSuppress, RaidParts::BURST_SUPPRESS)
RBA_PART_GETTER(0x5F5150, getHpAttack, RaidParts::HP_ATTACK)
RBA_PART_GETTER(0x5F51B0, getHpSuppress, RaidParts::HP_SUPPRESS)
RBA_PART_GETTER(0x5F5210, getGuiltSuppress, RaidParts::GUILT_SUPPRESS)
RBA_PART_GETTER(0x5F5270, getRewardId, RaidParts::REWARD_ID)
RBA_PART_GETTER(0x5F52D0, getMvpRewardId, RaidParts::MVP_REWARD_ID)
RBA_PART_GETTER(0x5F5330, getDestroyAllRewardId, RaidParts::DESTROY_ALL_REWARD_ID)
#undef RBA_PART_GETTER

// master::RaidEnemy bad status protection / resistance values (the shared_ptr is the caller's)
#define RBA_ENEMY_GETTER(ADDR, NAME, FIELD)                                                                   \
    int32_t RaidBossActor::NAME(const RaidBossActor *, const guest::SharedPtr *enemy) {                       \
        return get32(enemy->obj, FIELD);                                                                      \
    }                                                                                                         \
    RW_FUNCTION(ADDR, "RaidBossActor::" #NAME, RaidBossActor::NAME);
RBA_ENEMY_GETTER(0x5F5390, get_protectPoison, RE_PROTECT_POISON)
RBA_ENEMY_GETTER(0x5F53E4, get_protectDeepPoison, RE_PROTECT_DEEP_POISON)
RBA_ENEMY_GETTER(0x5F5438, get_protectSleep, RE_PROTECT_SLEEP)
RBA_ENEMY_GETTER(0x5F548C, get_protectParalysis, RE_PROTECT_PARALYSIS)
RBA_ENEMY_GETTER(0x5F54E0, get_protectConfusion, RE_PROTECT_CONFUSION)
RBA_ENEMY_GETTER(0x5F5534, get_registPoison, RE_REGIST_POISON)
RBA_ENEMY_GETTER(0x5F5588, get_registDeepPoison, RE_REGIST_DEEP_POISON)
RBA_ENEMY_GETTER(0x5F55DC, get_registSleep, RE_REGIST_SLEEP)
RBA_ENEMY_GETTER(0x5F5630, get_registParalysis, RE_REGIST_PARALYSIS)
RBA_ENEMY_GETTER(0x5F5684, get_registConfusion, RE_REGIST_CONFUSION)
#undef RBA_ENEMY_GETTER
