#include "Enemy.hpp"
#include "ParseUtilC.hpp"

namespace master {

using EnemyFunctions = TableFunctions<Enemy>;
RW_FUNCTION(0x77f7c0, "master::Enemy::getEnemyIds", EnemyFunctions::getIds);
RW_FUNCTION(0x77f7dc, "master::Enemy::create", EnemyFunctions::create);
RW_FUNCTION(0x780df0, "master::Enemy::getHash", EnemyFunctions::getHash);
RW_FUNCTION(0x780edc, "master::Enemy::getID", EnemyFunctions::getID);
RW_FUNCTION(0x780f28, "master::Enemy::~Enemy (deleting)", EnemyFunctions::destructDelete);

void Enemy::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    // the fields after the strings, in lookup (and data) order: an int (count 0) or an int32[count] from an array.
    // The ints go through the stack temps 0x18c, 0x188, ... in order; the arrays through the shared buffer.
    struct Field { uint32_t key, pos, count; };
    static constexpr Field FIELDS[] = {
        {0xe08887, DISPLAY_ID, 0},                  // "displayId"
        {0xe08891, SHOW, 0},                        // "show"
        {0xe08896, SHOW_SWF, 0},                    // "showSwf"
        {0xe0889e, SHOW_FRAME, 0},                  // "showFrame"
        {0xe088a8, HIDE_FRAME, 0},                  // "hideFrame"
        {0xe088b2, MOVE_SPEED, 0},                  // "moveSpeed"
        {0xe088bc, HIT_RADIUS, 0},                  // "hitRadius"
        {0xe088c6, TAP_RADIUS, 0},                  // "tapRadius"
        {0xe088d0, WIDTH, 0},                       // "width"
        {0xe088d6, HEIGHT, 0},                      // "height"
        {0xe088dd, SP_BATTLE, 0},                   // "spBattle"
        {0xe088e6, KIND, 0},                        // "kind"
        {0xdfb52f, ATTRIBUTE, 0},                   // "attribute"
        {0xe088eb, VALID_GROWTH, 0},                // "validGrowth"
        {0xe088f7, BASE_LV, 5},                     // "baseLv"
        {0xdfb539, HP, 5},                          // "hp"
        {0xdfb8dd, ATTACK, 5},                      // "attack"
        {0xdfb8d5, DEFENSE, 5},                     // "defense"
        {0xdfb666, EXP, 5},                         // "exp"
        {0xdfb90f, MONEY, 5},                       // "money"
        {0xdfb8e4, RAID_POINT, 5},                  // "raidPoint"
        {0xdfa9ff, LUX, 5},                         // "lux"
        {0xe088fe, ATTACK_CP, 0},                   // "attackCp"
        {0xe08907, SUPPRESS_CP, 0},                 // "suppressCp"
        {0xe08912, ATTACK_HP, 0},                   // "attackHp"
        {0xe0891b, SUPPRESS_HP, 0},                 // "suppressHp"
        {0xe08926, SUPPRESS_GUILT, 0},              // "suppressGuilt"
        {0xe08934, BATTLE_PATTERN, 0},              // "battlePattern"
        {0xe08942, DISPLAY_HEIGHT, 0},              // "displayHeight"
        {0xe08950, RUNAWAY, 0},                     // "runaway"
        {0xe08958, NEVER_ATTACK, 0},                // "neverAttack"
        {0xe08964, VALID_SKILL, 0},                 // "validSkill"
        {0xdfb66a, SKILL_ID, 4},                    // "skillId"
        {0xe0896f, SKILL_REQUIRE, 4},               // "skillRequire"
        {0xe0897c, SKILL_REQUIRE_ARG, 4},           // "skillRequireArg"
        {0xe0898c, SKILL_ODDS, 4},                  // "skillOdds"
        {0xe08996, PROTECT_POISON, 0},              // "protectPoison"
        {0xe089a4, PROTECT_DEEP_POISON, 0},         // "protectDeepPoison"
        {0xe089b6, PROTECT_SLEEP, 0},               // "protectSleep"
        {0xe089c3, PROTECT_PARALYSIS, 0},           // "protectParalysis"
        {0xe089d4, REGIST_POISON, 0},               // "registPoison"
        {0xe089e1, REGIST_DEEP_POISON, 0},          // "registDeepPoison"
        {0xe089f2, REGIST_SLEEP, 0},                // "registSleep"
        {0xe089fe, REGIST_PARALYSIS, 0},            // "registParalysis"
        {0xe08069, BUF_ATTACK, 0},                  // "bufAttack"
        {0xe0808e, BUF_DEFENSE, 0},                 // "bufDefense"
        {0xe080b6, BUF_ATTACK_POWER, 0},            // "bufAttackPower"
        {0xe080ea, BUF_DEFENSE_POWER, 0},           // "bufDefensePower"
        {0xe08121, BUF_ATTACK_SPEED, 0},            // "bufAttackSpeed"
        {0xe08155, BUF_DEFENSE_SPEED, 0},           // "bufDefenseSpeed"
        {0xe0818c, BUF_ATTACK_MAGIC, 0},            // "bufAttackMagic"
        {0xe081c0, BUF_DEFENSE_MAGIC, 0},           // "bufDefenseMagic"
        {0xe08a0e, BUF_TURN, 0},                    // "bufTurn"
        {0xe08a16, BUF_COUNT, 0},                   // "bufCount"
        {0xe08a1f, STATUS_AILMENTS, 2},             // "statusAilments"
        {0xe08a2e, STATUS_AILMENTS_TURN, 0},        // "statusAilmentsTurn"
        {0xe08a41, STATUS_AILMENTS_COUNT, 0},       // "statusAilmentsCount"
    };
    constexpr uint32_t COUNT = sizeof(FIELDS) / sizeof(FIELDS[0]);
    constexpr uint32_t BUF = 0x194;  // frame buffer shared by the strings and the arrays (stale bytes kept)
    guest::Frame f(0x3b0);
    RowBuilder b(f);
    uint32_t row = 0;
    uint32_t id = member(json, 0xe08878);               // "enemyId"
    if (isInt(id)) {
        uint32_t name = 0, flavor = 0;
        bool ok = isString(name = member(json, 0xdfade6)) &&      // "name"
                  isString(flavor = member(json, 0xe08880));       // "flavor"
        uint32_t values[COUNT];
        for (uint32_t k = 0; ok && k < COUNT; k++) {
            values[k] = member(json, FIELDS[k].key);
            ok = FIELDS[k].count ? isArray(values[k]) : isInt(values[k]);
        }
        if (ok) {
            b.create(SIZE, VTABLE);
            b.int32(ENEMY_ID, intOf(id), 0x190);
            b.string(NAME, stringOf(name), 65, BUF);
            b.string(FLAVOR, stringOf(flavor), 513, BUF);
            uint32_t temp = 0x18c;
            for (uint32_t k = 0; ok && k < COUNT; k++) {
                if (FIELDS[k].count == 0) {
                    b.int32(FIELDS[k].pos, intOf(values[k]), temp);
                    temp -= 4;
                } else {   // the array is looked up again
                    ok = b.int32Array(FIELDS[k].pos, member(json, FIELDS[k].key), FIELDS[k].count, BUF);
                }
            }
            if (ok) {
                row = b.obj();
            } else {
                b.destroy();
            }
        }
    }
    makeShared(out, SHARED_PTR_CTOR, row);
}
RW_FUNCTION(0x77F970, "master::Enemy::parse", Enemy::parse);

}  // namespace master
