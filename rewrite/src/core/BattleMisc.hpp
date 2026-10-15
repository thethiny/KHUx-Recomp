// BattleMisc: the battle formula singleton (not master::BattleMisc, the table of constants it reads).
// It owns the damage formulas of the player's attacks (medals, bursts) against enemies and raid bosses, the
// enemies' / raid bosses' attacks against the player, the attribute matching coefficient (weak / resist), the
// critical roll and the random spread, and the buff corrections those formulas apply.
//
// Buff corrections: every master::Buff row has a kind (data 0x50, 1..8 -> BuffType = (kind-1) & 1,
// BuffTarget = (kind-1) >> 1), a level (data 4) and a value (data 0x54, 10000 = 100%). The constructor indexes
// the rows per (type, target) in unordered_map<level, buffId>; a correction is the row of the level the actor's
// IBuffProtocol reports (buff_level), value / 10000, or 0 when that level has no row.
//
// The live instance is a function-local static at 0x1003EC0 (guard 0x1003F64).
#pragma once

#include "core/Guest.hpp"
#include "core/SafeBuffer.hpp"

struct BattleMisc {
    // std::unordered_map<int, int> (libc++, 32-bit __hash_table): bucket array, bucket count, first node
    // (before-begin's next), size, max_load_factor. Nodes: next +0, hash +4, key +8, value +12.
    struct IntMap {
        rw::Field<uint32_t> buckets;        // +0
        rw::Field<uint32_t> bucketCount;    // +4
        rw::Field<uint32_t> first;          // +8
        rw::Field<uint32_t> size;           // +12
        rw::Field<float> maxLoadFactor;     // +16
    };

    // buffLevels[type][target]: buff level -> master::Buff id (type 0 = attack, 1 = defense; target 0 = all,
    // 1..3 = an attribute)
    IntMap buffLevels[2][4];                // +0
    // result flags of the last formula (std::bitset<3>): bit 0 critical, bit 1 weak (attribute advantage),
    // bit 2 resist (attribute disadvantage); cleared at the start of the attack formulas
    rw::Field<uint32_t> flags;              // +160

    static constexpr uint32_t FLAG_CRITICAL = 1;
    static constexpr uint32_t FLAG_WEAK = 2;
    static constexpr uint32_t FLAG_RESIST = 4;

    static constexpr uint32_t INSTANCE = 0x1003EC0;
    static constexpr uint32_t INSTANCE_GUARD = 0x1003F64;

    static BattleMisc *getInstance();
    static BattleMisc *construct(BattleMisc *self);
    static BattleMisc *destruct(BattleMisc *self);

    // master::Buff row -> BuffType / BuffTarget / the kind itself (0 when the kind is not 1..8)
    static uint32_t get_buff_kinds_type(BattleMisc *self, int32_t buffId);
    static uint32_t get_buff_kinds_target(BattleMisc *self, int32_t buffId);
    static int32_t get_buff_kinds(BattleMisc *self, int32_t buffId);

    // buff corrections (value / 10000) of the player, an enemy, a raid boss (part idx; 0 = the boss itself)
    static float getPlayerBuffCorrection(BattleMisc *self, int32_t type, int32_t target);
    static float getEnemyBuffCorrection(BattleMisc *self, int32_t type, int32_t target, const uint8_t *enemy);
    static float getRaidBuffCorrection(BattleMisc *self, int32_t type, int32_t target, const uint8_t *raid,
                                       int32_t idx);
    // the same for one attribute (1..3; anything else: 0)
    static float getPlayerBuffCorrectionAttack(BattleMisc *self, int32_t attr);
    static float getPlayerBuffCorrectionDefense(BattleMisc *self, int32_t attr);
    static float getEnemyBuffCorrectionAttack(BattleMisc *self, int32_t attr, const uint8_t *enemy);
    static float getEnemyBuffCorrectionDefense(BattleMisc *self, int32_t attr, const uint8_t *enemy);
    static float getRaidBuffCorrectionAttack(BattleMisc *self, int32_t attr, const uint8_t *raid);
    static float getRaidBuffCorrectionDefense(BattleMisc *self, int32_t attr, const uint8_t *raid, int32_t idx);

    // player attacks
    static int32_t calculatePlayerAttack(BattleMisc *self, const uint8_t *target, const uint8_t *enemy);
    static int32_t calculatePlayerAttackRaid(BattleMisc *self, const uint8_t *target, const uint8_t *raid, int32_t idx);
    static int32_t calculatePlayerAttack_sub(BattleMisc *self, const uint8_t *target, int32_t defense, float defenseBuff,
                                             uint32_t enemyCondition0or3, uint32_t enemyCondition1,
                                             uint32_t enemyCondition2, int32_t attr, int32_t enemyAttr,
                                             float attrDefenseBuff, uint32_t outCoefficient, uint32_t outMatching);
    // enemy / raid boss attacks
    static int32_t calculateEnemyAttack(BattleMisc *self, const uint8_t *enemy, int32_t defense);
    static int32_t calculateRaidAttack(BattleMisc *self, const uint8_t *raid, const uint8_t *medalDeck, guest::SharedPtr *attack);
    static int32_t calculateEnemyAttack_sub(BattleMisc *self, int32_t attr, int32_t defense, int32_t attack,
                                            float attackBuff, float power, float critical, int32_t divisor,
                                            float attrAttackBuff);

    // side effects of an attack before its damage: a burst's buffs / debuffs and buff steal (returns the
    // out-of-line part's result, sub_74344C; 0 for medal attacks, which reset the burst chain counter), an enemy
    // attack's conditions, heal, cures and buffs on its targets
    static int32_t preparePlayerAttackEnemy(BattleMisc *self, const uint8_t *target, const uint8_t *actorManager);
    static void prepareEnemyAttack(BattleMisc *self, const uint8_t *enemy, const uint8_t *avatar, const uint8_t *actorManager);

    static float calcMatchingCoefficient(BattleMisc *self, int32_t attackerAttr, int32_t defenderAttr);
    static float get_criticalCoefficient(BattleMisc *self, float rate);
    // max(1, trunc(random[1 - spread, 1 + spread] * (base * multiplier / divisor)))
    static int32_t normalizeAttack(BattleMisc *self, float base, float multiplier, int32_t divisor);
};
RW_LAYOUT(BattleMisc, buffLevels, 0);
RW_LAYOUT(BattleMisc, flags, 160);
RW_SIZE(BattleMisc::IntMap, 20);
RW_SIZE(BattleMisc, 164);
