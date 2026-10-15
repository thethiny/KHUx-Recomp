#include "EnemyAttack.hpp"
#include "ParseUtilC.hpp"

namespace master {

using EnemyAttackFunctions = TableFunctions<EnemyAttack>;
RW_FUNCTION(0x780f34, "master::EnemyAttack::getEnemyAttackIds", EnemyAttackFunctions::getIds);
RW_FUNCTION(0x780f50, "master::EnemyAttack::create", EnemyAttackFunctions::create);
RW_FUNCTION(0x781c78, "master::EnemyAttack::getHash", EnemyAttackFunctions::getHash);
RW_FUNCTION(0x781d64, "master::EnemyAttack::getID", EnemyAttackFunctions::getID);
RW_FUNCTION(0x781db0, "master::EnemyAttack::~EnemyAttack (deleting)", EnemyAttackFunctions::destructDelete);

void EnemyAttack::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    // the int fields after the name, in lookup (and data) order; field k goes through the stack temp 0x14c - 4k
    struct IntField { uint32_t key, pos; };
    static constexpr IntField INT_FIELDS[] = {
        {0xe08a8c, ATTACK_MOTION},                  // "attackMotion"
        {0xe08a99, HIT_EFFECT},                     // "hitEffect"
        {0xe07f63, EFFECT_DIRECTION_FIX},           // "effectDirectionFix"
        {0xdfb361, DISPLAY},                        // "display"
        {0xe08aa3, CHOISE_ATTRIBUTE},               // "choiseAttribute"
        {0xe07f86, HIT_COUNT},                      // "hitCount"
        {0xe07e96, POWER},                          // "power"
        {0xe07f95, CRITICAL},                       // "critical"
        {0xe07f9e, CRITICAL_POWER},                 // "criticalPower"
        {0xe07e8f, TARGET},                         // "target"
        {0xe07e88, EFFECT},                         // "effect"
        {0xe08ab3, ADD_TARGET},                     // "addTarget"
        {0xe08058, ADD_TURN},                       // "addTurn"
        {0xe08060, ADD_COUNT},                      // "addCount"
        {0xe08069, BUF_ATTACK},                     // "bufAttack"
        {0xe0808e, BUF_DEFENSE},                    // "bufDefense"
        {0xe080b6, BUF_ATTACK_POWER},               // "bufAttackPower"
        {0xe080ea, BUF_DEFENSE_POWER},              // "bufDefensePower"
        {0xe08121, BUF_ATTACK_SPEED},               // "bufAttackSpeed"
        {0xe08155, BUF_DEFENSE_SPEED},              // "bufDefenseSpeed"
        {0xe0818c, BUF_ATTACK_MAGIC},               // "bufAttackMagic"
        {0xe081c0, BUF_DEFENSE_MAGIC},              // "bufDefenseMagic"
        {0xe08abd, RESET_ATTACK},                   // "resetAttack"
        {0xe08ac9, RESET_DEFENSE},                  // "resetDefense"
        {0xe08ad6, RESET_ATTACK_POWER},             // "resetAttackPower"
        {0xe08ae7, RESET_DEFENSE_POWER},            // "resetDefensePower"
        {0xe08af9, RESET_ATTACK_SPEED},             // "resetAttackSpeed"
        {0xe08b0a, RESET_DEFENSE_SPEED},            // "resetDefenseSpeed"
        {0xe08b1c, RESET_ATTACK_MAGIC},             // "resetAttackMagic"
        {0xe08b2d, RESET_DEFENSE_MAGIC},            // "resetDefenseMagic"
        {0xe0841e, HEAL},                           // "heal"
        {0xe08423, HEAL_SHAKE},                     // "healShake"
        {0xe08b3f, POISON},                         // "poison"
        {0xe08b46, DEEP_POISON},                    // "deepPoison"
        {0xe08b51, SLEEP},                          // "sleep"
        {0xe08b57, PARALYSIS},                      // "paralysis"
        {0xe0842d, CURE_POISON},                    // "curePoison"
        {0xe08438, CURE_SLEEP},                     // "cureSleep"
        {0xe08451, CURE_PARALYSIS},                 // "cureParalysis"
    };
    constexpr uint32_t COUNT = sizeof(INT_FIELDS) / sizeof(INT_FIELDS[0]);
    guest::Frame f(0x1b0);
    RowBuilder b(f);
    uint32_t id = member(json, 0xe08a7e);               // "enemyAttackId"
    if (isInt(id)) {
        uint32_t name = member(json, 0xdfade6);         // "name"
        bool ok = isString(name);
        uint32_t values[COUNT];
        for (uint32_t k = 0; ok && k < COUNT; k++) {
            values[k] = member(json, INT_FIELDS[k].key);
            ok = isInt(values[k]);
        }
        if (ok) {
            b.create(SIZE, VTABLE);
            b.int32(ENEMY_ATTACK_ID, intOf(id), 0x150);
            b.string(NAME, stringOf(name), 65, 0x154);
            for (uint32_t k = 0; k < COUNT; k++) b.int32(INT_FIELDS[k].pos, intOf(values[k]), 0x14c - 4 * k);
        }
    }
    makeShared(out, SHARED_PTR_CTOR, b.obj());
}
RW_FUNCTION(0x7810E4, "master::EnemyAttack::parse", EnemyAttack::parse);

}  // namespace master
