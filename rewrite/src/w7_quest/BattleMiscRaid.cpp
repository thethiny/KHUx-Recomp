// BattleMisc, the rest: a burst's effects on the player (sub_74344C, the outlined end of preparePlayerAttackEnemy /
// preparePlayerAttackRaid: buff levels by burst kind, buffs given and cleared, a heal, cures) and the raid version
// of the player's attack preparation (the burst chain, buffs on the hit part / the boss / every part, taking over
// the hit part's or the boss's buffs). The parts' take-over clears the player's type-0 buffs but copies back only
// the type-1 ones (as the original does).
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t SafeBuffer_read = 0x45A269, SafeBuffer_write = 0x459FA1;
constexpr uint32_t master_Burst_create = 0x77A491, master_RaidEnemy_create = 0x787745;
constexpr uint32_t StageState_setBuffChange = 0x4F02DD, IBuffProtocol_buff_level = 0x4F23CD;
constexpr uint32_t IBuffProtocol_buff_add = 0x4F256F, IBuffProtocol_buff_clear = 0x4F27E9;
constexpr uint32_t BuffChange_check_type_target = 0x4F20C1, IStatusProtocol_curePoison = 0x74030D;
constexpr uint32_t StatusChange_clear = 0x4F1D15, Random_getInstance = 0x492055, Random_generateUniformFloat = 0x492261;
constexpr uint32_t RaidBossProperty_getBuff = 0x5F65CF, PartsProperty_getBuff = 0x5F66CF;
constexpr uint32_t BattleMisc_burstOnPlayer = 0x74344D;
constexpr uint32_t GOT_BUFF_MIN_LEVEL = 0xFD6574;
constexpr uint32_t master_RaidEnemyAttack_create = 0x789039, Random_generateUniformInt = 0x492225;
constexpr uint32_t RaidBoss_getCurrentHP = 0x5F2ECD, RaidBoss_getMaxHP = 0x5F3B05, RaidBoss_setCurrentHP = 0x5F3FAD;
constexpr uint32_t RaidBoss_updateDamageAll = 0x5F2F2D;
constexpr uint32_t buffVector_pushBackSlow = 0x748C71, buffVector_destruct = 0x748C1B;
constexpr uint32_t statusVector_pushBackSlow = 0x748DAD, statusVector_destruct = 0x748C45;
// master::RaidEnemy +0x220: the attack ids (4). master::RaidEnemyAttack: +0x68 who (2 the player, 3 the boss, 4 every
// living part, 5 the part with the lowest HP ratio), +0x6C / +0x70 buff turns / param, adds at 0x74 + 8g + 4t,
// clears at 0x94 + 8g + 4t, heal 0xB4 (+ spread 0xB8), condition chances 0xBC (0), 0xC0 (3), 0xC4 (1), 0xC8 (2),
// cures 0xCC (poison), 0xD0 (1), 0xD4 (2)
constexpr uint32_t RE_ATTACKS = 0x220, RA_TARGET = 0x68, RA_TURNS = 0x6C, RA_PARAM = 0x70, RA_ADD = 0x74;
constexpr uint32_t RA_CLEAR = 0x94, RA_HEAL = 0xB4, RA_SPREAD = 0xB8, RA_COND0 = 0xBC, RA_COND3 = 0xC0;
constexpr uint32_t RA_COND1 = 0xC4, RA_COND2 = 0xC8, RA_CURE_POISON = 0xCC, RA_CURE1 = 0xD0, RA_CURE2 = 0xD4;
constexpr uint32_t BOSS_STATUS = 0x2A8, PART_BUFFS = 0x2D0, AVATAR_HIT_FLAG = 0x314;
// StageState: +0x20 the status protocol (IStatusProtocol), +0x24 the buff protocol, +0x425C bursts in a row
constexpr uint32_t STAGE_STATUS = 0x20, STAGE_BUFFS = 0x24, STAGE_BURST_CHAIN = 0x425C;
constexpr uint32_t STAGE_ADD_BUFF = 0x18;          // StageState virtual: add a BuffChange
// AttackTarget: +0 the kind (0..2 medal attacks, 3 burst), +4 the medal (shared_ptr*: SafeBuffer, +0x4AC burst)
constexpr uint32_t TARGET_MEDAL = 4, MEDAL_BURST_ID = 0x4AC;
// master::Burst
constexpr uint32_t BURST_KIND = 0x1C0, BURST_TURNS = 0x1F4, BURST_PARAM = 0x1F8;
constexpr uint32_t BURST_ADD_ENEMY = 0x200, BURST_ADD_TEAM = 0x204, BURST_ADD_SELF = 0x1FC;
constexpr uint32_t BURST_CLEAR_ENEMY = 0x260, BURST_CLEAR_TEAM = 0x264, BURST_CLEAR_SELF = 0x25C;
constexpr uint32_t BURST_STEAL_ALL = 0x2BC, BURST_STEAL_ATTRIBUTE = 0x2C0;
constexpr uint32_t BURST_HEAL = 0x2C4, BURST_HEAL_SPREAD = 0x2C8, BURST_CURE_POISON = 0x2CC;
constexpr uint32_t BURST_CURE1 = 0x2D0, BURST_CURE2 = 0x2D8;
// RaidBossActor: +0x288 its Property, +0x2AC the body's buff protocol, parts' at +0x2AC + 0x24 * n (their
// Property at +0x2B0 + 0x24 * n), +0x8A4 the raid enemy id. master::RaidEnemy: +0x90 the part slot, +0x94 the
// parts, +0x98 their raid enemy ids (5)
constexpr uint32_t BOSS_PROPERTY = 0x288, BOSS_BUFFS = 0x2AC, PART_STRIDE = 0x24, PART_PROPERTY = 0x2B0;
constexpr uint32_t BOSS_RAID_ENEMY = 0x8A4, RE_SLOT = 0x90, RE_PARTS = 0x94, RE_PART_IDS = 0x98;

uint32_t sbGet(uint32_t sb, uint32_t index, uint32_t n = 4) {
    Frame f;
    const uint32_t v = f.alloc(n < 4 ? 4 : n);
    call(SafeBuffer_read, {sb, v, ld32(sb + 0xC) + index, n, index});
    return ld32(v);
}
uint32_t stage() { return ld32(call(fn::AppState_getInstance, {}) + 0x20); }
uint32_t field(uint32_t base, uint32_t t, uint32_t g) { return base + 0xC * t + 0x18 * g; }
void buffAdd(uint32_t p, uint32_t v, uint32_t turns, uint32_t param, uint32_t t, uint32_t g) {
    call(IBuffProtocol_buff_add, {p, v, turns, param, t, g});
}
void buffClear(uint32_t p, uint32_t t, uint32_t g) { call(IBuffProtocol_buff_clear, {p, t, g}); }

// adds of (t = 0..1, g = 0..3) from `addBase`, then clears from `clearBase`, on `protocol`; `types` 1: type 1 only
void buffsOn(uint32_t burst, uint32_t protocol, uint32_t addBase, uint32_t clearBase, uint32_t turns, uint32_t param,
             uint32_t firstType) {
    for (uint32_t t = firstType; t < 2; t++)
        for (uint32_t g = 0; g < 4; g++)
            if (const uint32_t v = sbGet(burst, field(addBase, t, g))) buffAdd(protocol, v, turns, param, t, g);
    for (uint32_t t = firstType; t < 2; t++)
        for (uint32_t g = 0; g < 4; g++)
            if (sbGet(burst, field(clearBase, t, g))) buffClear(protocol, t, g);
}

// the player's buffs of (t, g) replaced by the source's 64 slots of that kind (getBuff(slot, source, k))
void copyBuffs(uint32_t getBuff, uint32_t source, uint32_t t, uint32_t g, uint32_t slot) {
    for (uint32_t k = 0; k < 64; k++) {
        call(getBuff, {slot, source, k});
        if (call(BuffChange_check_type_target, {slot, t, g}) == 1) {
            const uint32_t st = stage();
            vcall(st, STAGE_ADD_BUFF, {st, slot});
        }
    }
}

// the level change of a burst kind (8..19): min(level - minLevel, -value) for (type, target)
void levelChange(uint32_t t, uint32_t g, uint32_t value) {
    const int32_t level = int32_t(call(IBuffProtocol_buff_level, {stage() + STAGE_BUFFS, t, g})) -
                          int32_t(ld32(ld32(GOT_BUFF_MIN_LEVEL)));
    const int32_t cap = -int32_t(value);
    call(StageState_setBuffChange, {stage(), t, g, uint32_t(level >= cap ? cap : level)});
}

void clearCondition(uint32_t protocol, uint32_t type, uint32_t slot) {
    const uint32_t vt = ld32(protocol);
    const uint32_t setCondition = ld32(vt + 4);
    call(ld32(vt), {slot, protocol, type});
    call(setCondition, {protocol, type, call(StatusChange_clear, {slot})});
}
int32_t randomPercent() {
    return int32_t(call(Random_generateUniformInt, {call(Random_getInstance, {}), 1, 10000, 0}));
}
void pushBack(uint32_t vec, uint32_t slot, uint32_t value, uint32_t slowPath) {
    st32(slot, value);
    const uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        if (end) st32(end, value);
        st32(vec + 4, end + 4);
    } else {
        call(slowPath, {vec, slot});
    }
}
// boss part `idx` (its slot - 1; 0 the body) set to (ratio + random[0, spread]) of its max HP, when the attack heals
void healPart(uint32_t attack, uint32_t boss, uint32_t idx) {
    const float ratio = float(int32_t(sbGet(attack, RA_HEAL))) / 10000.0f;
    const float spread = float(int32_t(sbGet(attack, RA_SPREAD))) / 10000.0f;
    if (!(ratio > 0.0f) && (spread <= 0.0f || spread != spread)) return;
    const float r = bitsf(call(Random_generateUniformFloat, {call(Random_getInstance, {}), 0, fbits(spread), 0}));
    const uint32_t max = call(RaidBoss_getMaxHP, {boss, idx});
    call(RaidBoss_setCurrentHP, {boss, idx, uint32_t(int32_t((ratio + r) * float(max)))});
}
// the attack type-1 buffs on boss part `idx`
void partBuffs(uint32_t attack, uint32_t boss, uint32_t idx, uint32_t turns, uint32_t param) {
    const uint32_t p = boss + PART_BUFFS + PART_STRIDE * idx;
    for (uint32_t g = 0; g < 4; g++)
        if (const uint32_t v = sbGet(attack, RA_ADD + 8 * g + 4)) buffAdd(p, v, turns, param, 1, g);
    for (uint32_t g = 0; g < 4; g++)
        if (sbGet(attack, RA_CLEAR + 8 * g + 4)) buffClear(p, 1, g);
}
}  // namespace

namespace battle_misc_raid {

// a burst's effects on the player (returns 1 when it healed)
uint32_t burstOnPlayer(uint32_t target) {
    Frame f;
    const uint32_t medal = ld32(target + TARGET_MEDAL);
    const uint32_t burstRef = masterRow(f, master_Burst_create, sbGet(ld32(medal), MEDAL_BURST_ID));
    sbGet(medal + 8, 0x10);
    const uint32_t burst = ld32(burstRef);
    const uint32_t turns = sbGet(burst, BURST_TURNS), param = sbGet(burst, BURST_PARAM);
    const uint32_t status = stage() ? stage() + STAGE_STATUS : 0;
    const uint32_t buffs = stage() ? stage() + STAGE_BUFFS : 0;
    const uint32_t kind = sbGet(burst, BURST_KIND);
    if (kind >= 8 && kind <= 15) {
        const uint32_t t = (kind - 8) / 4, g = (kind - 8) % 4;
        levelChange(t, g, sbGet(burst, field(BURST_ADD_SELF, t, g)));
    } else if (kind >= 16 && kind <= 19) {
        const uint32_t g = kind - 16;
        const uint32_t v0 = sbGet(burst, field(BURST_ADD_SELF, 0, g)), v1 = sbGet(burst, field(BURST_ADD_SELF, 1, g));
        levelChange(0, g, v0);
        levelChange(1, g, v1);
    }
    for (uint32_t t = 0; t < 2; t++)
        for (uint32_t g = 0; g < 4; g++)
            if (const uint32_t v = sbGet(burst, field(BURST_ADD_SELF, t, g))) buffAdd(buffs, v, turns, param, t, g);
    for (uint32_t t = 0; t < 2; t++)
        for (uint32_t g = 0; g < 4; g++)
            if (sbGet(burst, field(BURST_CLEAR_SELF, t, g))) buffClear(buffs, t, g);
    const float heal = float(int32_t(sbGet(burst, BURST_HEAL))) / 10000.0f;
    const float spread = float(int32_t(sbGet(burst, BURST_HEAL_SPREAD))) / 10000.0f;
    uint32_t healed = 0;
    if (heal > 0.0f || !(spread <= 0.0f)) {
        const float r = bitsf(call(Random_generateUniformFloat, {call(Random_getInstance, {}), 0, fbits(spread), 0}));
        const uint32_t vt = ld32(status);
        const uint32_t setHp = ld32(vt + 0x10);
        const int32_t max = int32_t(call(ld32(vt + 0xC), {status}));
        call(setHp, {status, uint32_t(int32_t((heal + r) * float(max)))});
        healed = 1;
    }
    if (sbGet(burst, BURST_CURE_POISON)) call(IStatusProtocol_curePoison, {status});
    const uint32_t slot = f.alloc(16);
    if (sbGet(burst, BURST_CURE1)) clearCondition(status, 1, slot);
    if (sbGet(burst, BURST_CURE2)) clearCondition(status, 2, slot);
    sharedRelease(burstRef);
    return healed;
}
RW_FUNCTION(0x74344C, "BattleMisc burst effects on the player (sub_74344C)", battle_misc_raid::burstOnPlayer);

// the player's attack on raid boss part `part` (0 the body): the burst chain, the burst's buffs, then its effects
// on the player
uint32_t preparePlayerAttackRaid(uint32_t, uint32_t target, uint32_t boss, uint32_t part) {
    const uint32_t kind = ld32(target);
    if (kind != 3 && kind <= 2) {
        Frame f;
        const uint32_t zero = f.alloc(4);
        st32(zero, 0);
        const uint32_t st = stage();
        call(SafeBuffer_write, {st, ld32(st + 0xC) + STAGE_BURST_CHAIN, zero, 4, STAGE_BURST_CHAIN});
        return 0;
    }
    if (kind == 3) {
        const uint32_t st = stage();
        Frame f;
        const uint32_t n = f.alloc(4);
        st32(n, sbGet(stage(), STAGE_BURST_CHAIN) + 1);
        call(SafeBuffer_write, {st, ld32(st + 0xC) + STAGE_BURST_CHAIN, n, 4, STAGE_BURST_CHAIN});
    }
    Frame f;
    const uint32_t raidRef = masterRow(f, master_RaidEnemy_create, ld32(boss + BOSS_RAID_ENEMY));
    const uint32_t medal = ld32(target + TARGET_MEDAL);
    const uint32_t burstRef = masterRow(f, master_Burst_create, sbGet(ld32(medal), MEDAL_BURST_ID));
    const uint32_t burst = ld32(burstRef);
    const uint32_t turns = sbGet(burst, BURST_TURNS), param = sbGet(burst, BURST_PARAM);
    const uint32_t raid = ld32(raidRef);
    const uint32_t body = boss + BOSS_BUFFS;
    if (int32_t(part) >= 1) {
        const uint32_t ids = f.alloc(0x14);
        call(SafeBuffer_read, {raid, ids, ld32(raid + 0xC) + RE_PART_IDS, 0x14, RE_PART_IDS});
        Frame g;
        const uint32_t partRef = masterRow(g, master_RaidEnemy_create, ld32(ids + 4 * (part - 1)));
        const uint32_t slotIndex = sbGet(ld32(partRef), RE_SLOT);
        buffsOn(burst, body + PART_STRIDE * slotIndex, BURST_ADD_ENEMY, BURST_CLEAR_ENEMY, turns, param, 1);
        sharedRelease(partRef);
    } else if (part == 0) {
        buffsOn(burst, body, BURST_ADD_ENEMY, BURST_CLEAR_ENEMY, turns, param, 0);
    }
    buffsOn(burst, body, BURST_ADD_TEAM, BURST_CLEAR_TEAM, turns, param, 0);
    const int32_t parts = int32_t(sbGet(raid, RE_PARTS));
    for (int32_t k = 0; k < parts; k++) {
        const uint32_t ids = f.alloc(0x14);
        call(SafeBuffer_read, {raid, ids, ld32(raid + 0xC) + RE_PART_IDS, 0x14, RE_PART_IDS});
        Frame g;
        const uint32_t partRef = masterRow(g, master_RaidEnemy_create, ld32(ids + 4 * uint32_t(k)));
        const uint32_t slotIndex = sbGet(ld32(partRef), RE_SLOT);
        buffsOn(burst, body + PART_STRIDE * slotIndex, BURST_ADD_TEAM, BURST_CLEAR_TEAM, turns, param, 1);
        sharedRelease(partRef);
    }
    const uint32_t slot = f.alloc(16);
    if (part) {
        const uint32_t source = boss + PART_STRIDE * part + PART_PROPERTY;
        if (sbGet(burst, BURST_STEAL_ALL)) {
            buffClear(stage() + STAGE_BUFFS, 0, 0);
            buffClear(stage() + STAGE_BUFFS, 1, 0);
            copyBuffs(PartsProperty_getBuff, source, 1, 0, slot);
        }
        if (sbGet(burst, BURST_STEAL_ATTRIBUTE)) {
            buffClear(stage() + STAGE_BUFFS, 0, 1);
            buffClear(stage() + STAGE_BUFFS, 0, 2);
            buffClear(stage() + STAGE_BUFFS, 0, 3);
            for (uint32_t g = 1; g < 4; g++) {
                buffClear(stage() + STAGE_BUFFS, 1, g);
                copyBuffs(PartsProperty_getBuff, source, 1, g, slot);
            }
        }
    } else {
        const uint32_t source = boss + BOSS_PROPERTY;
        if (sbGet(burst, BURST_STEAL_ALL)) {
            for (uint32_t t = 0; t < 2; t++) {
                buffClear(stage() + STAGE_BUFFS, t, 0);
                copyBuffs(RaidBossProperty_getBuff, source, t, 0, slot);
            }
        }
        if (sbGet(burst, BURST_STEAL_ATTRIBUTE)) {
            for (uint32_t t = 0; t < 2; t++)
                for (uint32_t g = 1; g < 4; g++) {
                    buffClear(stage() + STAGE_BUFFS, t, g);
                    copyBuffs(RaidBossProperty_getBuff, source, t, g, slot);
                }
        }
    }
    sharedRelease(burstRef);
    sharedRelease(raidRef);
    return call(BattleMisc_burstOnPlayer, {target});
}
RW_FUNCTION(0x7441F4, "BattleMisc::preparePlayerAttackRaid", battle_misc_raid::preparePlayerAttackRaid);


// the raid boss attack `index` on the player (`avatar`): conditions, heal, cures and buffs on its targets
uint32_t prepareRaidAttack(uint32_t, uint32_t boss, uint32_t avatar, uint32_t index) {
    Frame f;
    const uint32_t buffs = f.alloc(12), statuses = f.alloc(12), slot = f.alloc(4);
    st32(buffs, 0); st32(buffs + 4, 0); st32(buffs + 8, 0);
    st32(statuses, 0); st32(statuses + 4, 0); st32(statuses + 8, 0);
    const uint32_t raidRef = masterRow(f, master_RaidEnemy_create, ld32(boss + BOSS_RAID_ENEMY));
    const uint32_t raid = ld32(raidRef);
    const uint32_t ids = f.alloc(16);
    call(SafeBuffer_read, {raid, ids, ld32(raid + 0xC) + RE_ATTACKS, 0x10, RE_ATTACKS});
    const uint32_t attackRef = masterRow(f, master_RaidEnemyAttack_create, ld32(ids + 4 * index));
    const uint32_t attack = ld32(attackRef);
    const int32_t parts = int32_t(sbGet(raid, RE_PARTS));
    const uint32_t turns = sbGet(attack, RA_TURNS), param = sbGet(attack, RA_PARAM);
    const uint32_t kind = sbGet(attack, RA_TARGET);
    auto finish = [&]() {
        sharedRelease(attackRef);
        sharedRelease(raidRef);
        call(buffVector_destruct, {buffs});
        call(statusVector_destruct, {statuses});
        return 0u;                                      // r0: the stack guard check (guard - saved = 0)
    };
    auto partRow = [&](Frame &g, int32_t k) {
        const uint32_t list = g.alloc(0x14);
        call(SafeBuffer_read, {raid, list, ld32(raid + 0xC) + RE_PART_IDS, 0x14, RE_PART_IDS});
        return masterRow(g, master_RaidEnemy_create, ld32(list + 4 * uint32_t(k)));
    };
    if (kind - 2 > 3) return finish();
    if (kind == 5) {
        if (parts < 1) return finish();
        float best = 3.40282347e+38f;
        int32_t chosen = -1;
        for (int32_t k = 0; k < parts; k++) {
            Frame g;
            const uint32_t partRef = partRow(g, k);
            const uint32_t idx = sbGet(ld32(partRef), RE_SLOT) - 1;
            if (call(RaidBoss_getCurrentHP, {boss, idx})) {
                const uint32_t cur = call(RaidBoss_getCurrentHP, {boss, idx});
                const uint32_t max = call(RaidBoss_getMaxHP, {boss, idx});
                const float ratio = float(cur / max);           // integer division, as the original
                if (!(best <= ratio)) {
                    chosen = int32_t(idx);
                    best = ratio;
                }
            }
            sharedRelease(partRef);
        }
        if (chosen < 0) return finish();
        partBuffs(attack, boss, uint32_t(chosen), turns, param);
        healPart(attack, boss, uint32_t(chosen));
        return finish();
    }
    if (kind == 2) {
        const uint32_t st = stage();
        pushBack(statuses, slot, st ? st + STAGE_STATUS : 0, statusVector_pushBackSlow);
        const uint32_t st2 = stage();
        pushBack(buffs, slot, st2 ? st2 + STAGE_BUFFS : 0, buffVector_pushBackSlow);
    } else {
        pushBack(statuses, slot, boss + BOSS_STATUS, statusVector_pushBackSlow);
        pushBack(buffs, slot, boss + BOSS_BUFFS, buffVector_pushBackSlow);
    }
    if (kind == 4) {
        // the team: every living part (the lists above stay unused)
        for (int32_t k = 0; k < parts; k++) {
            Frame g;
            const uint32_t partRef = partRow(g, k);
            const uint32_t idx = sbGet(ld32(partRef), RE_SLOT) - 1;
            if (call(RaidBoss_getCurrentHP, {boss, idx})) {
                partBuffs(attack, boss, idx, turns, param);
                healPart(attack, boss, idx);
            }
            sharedRelease(partRef);
        }
        return finish();
    }
    if (ld32(statuses) != ld32(statuses + 4)) {
        int32_t condTurns = int32_t(turns), condParam = int32_t(param);
        if (!condTurns) { condTurns = condParam; condParam = 0; }
        for (uint32_t it = ld32(statuses); it != ld32(statuses + 4); it += 4) {
            const uint32_t p = ld32(it);
            int32_t a = int32_t(turns), b = int32_t(param);
            if (int32_t(sbGet(attack, RA_COND0)) >= randomPercent()) {
                vcall(p, 0x8, {p, 0, uint32_t(condTurns), uint32_t(condParam)});
                a = condTurns;
                b = condParam;
            }
            if (int32_t(sbGet(attack, RA_COND3)) >= randomPercent()) {
                if (!a) { a = b; b = 0; }
                vcall(p, 0x8, {p, 3, uint32_t(a), uint32_t(b)});
            }
            if (int32_t(sbGet(attack, RA_COND1)) >= randomPercent()) {
                vcall(p, 0x8, {p, 1, uint32_t(a), uint32_t(b)});
                st8(avatar + AVATAR_HIT_FLAG, 1);
            }
            if (int32_t(sbGet(attack, RA_COND2)) >= randomPercent()) vcall(p, 0x8, {p, 2, uint32_t(a), uint32_t(b)});
        }
        healPart(attack, boss, 0);
        if (sbGet(attack, RA_CURE_POISON))
            for (uint32_t it = ld32(statuses); it != ld32(statuses + 4); it += 4) call(IStatusProtocol_curePoison, {ld32(it)});
        const uint32_t change = f.alloc(16);
        if (sbGet(attack, RA_CURE1))
            for (uint32_t it = ld32(statuses); it != ld32(statuses + 4); it += 4) clearCondition(ld32(it), 1, change);
        if (sbGet(attack, RA_CURE2))
            for (uint32_t it = ld32(statuses); it != ld32(statuses + 4); it += 4) clearCondition(ld32(it), 2, change);
    }
    if (ld32(buffs) != ld32(buffs + 4)) {
        for (uint32_t t = 0; t < 2; t++)
            for (uint32_t g = 0; g < 4; g++)
                if (const uint32_t v = sbGet(attack, RA_ADD + 8 * g + 4 * t))
                    for (uint32_t it = ld32(buffs); it != ld32(buffs + 4); it += 4) buffAdd(ld32(it), v, turns, param, t, g);
        for (uint32_t t = 0; t < 2; t++)
            for (uint32_t g = 0; g < 4; g++)
                if (sbGet(attack, RA_CLEAR + 8 * g + 4 * t))
                    for (uint32_t it = ld32(buffs); it != ld32(buffs + 4); it += 4) buffClear(ld32(it), t, g);
    }
    call(RaidBoss_updateDamageAll, {boss});
    return finish();
}
RW_FUNCTION(0x74612C, "BattleMisc::prepareRaidAttack", battle_misc_raid::prepareRaidAttack);

}  // namespace battle_misc_raid
