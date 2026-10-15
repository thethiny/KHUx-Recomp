#include "RaidEnemy.hpp"
#include "ParseUtilB.hpp"

namespace master {

using RaidEnemyFunctions = TableFunctions<RaidEnemy>;
RW_FUNCTION(0x787728, "master::RaidEnemy::getRaidEnemyIds", RaidEnemyFunctions::getIds);
RW_FUNCTION(0x787744, "master::RaidEnemy::create", RaidEnemyFunctions::create);
RW_FUNCTION(0x788ed8, "master::RaidEnemy::getHash", RaidEnemyFunctions::getHash);
RW_FUNCTION(0x788fc4, "master::RaidEnemy::getID", RaidEnemyFunctions::getID);
RW_FUNCTION(0x789010, "master::RaidEnemy::~RaidEnemy (deleting)", RaidEnemyFunctions::destructDelete);

void RaidEnemy::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xdfb53c, Kind::Int, RAID_ENEMY_ID, 0x4, 0x118},               // "raidEnemyId"
        {0xdfade6, Kind::String, NAME, 0x81, 0x11c},                    // "name"
        {0xdfb52f, Kind::Int, ATTRIBUTE, 0x4, 0x114},                   // "attribute"
        {0xdfb52a, Kind::Int, RARE, 0x4, 0x110},                        // "rare"
        {0xe09130, Kind::Int, WEAKNESS, 0x4, 0x10c},                    // "weakness"
        {0xe07c76, Kind::Int, VALID_PARTS, 0x4, 0x108},                 // "validParts"
        {0xdfb4c9, Kind::IntArray, PARTS, 0x14, 0x11c},                 // "parts"
        {0xe088eb, Kind::Int, VALID_GROWTH, 0x4, 0x104},                // "validGrowth"
        {0xe088f7, Kind::IntArray, BASE_LV, 0x14, 0x11c},               // "baseLv"
        {0xdfb539, Kind::IntArray, HP, 0x14, 0x11c},                    // "hp"
        {0xdfb8dd, Kind::IntArray, ATTACK, 0x14, 0x11c},                // "attack"
        {0xdfb8d5, Kind::IntArray, DEFENSE, 0x14, 0x11c},               // "defense"
        {0xe09139, Kind::IntArray, EXP_ATTACK, 0x14, 0x11c},            // "expAttack"
        {0xe09143, Kind::IntArray, EXP_SUPPRESS, 0x14, 0x11c},          // "expSuppress"
        {0xe0914f, Kind::IntArray, MONEY_ATTACK, 0x14, 0x11c},          // "moneyAttack"
        {0xe0915b, Kind::IntArray, MONEY_SUPPRESS, 0x14, 0x11c},        // "moneySuppress"
        {0xe09169, Kind::IntArray, LUX_ATTACK, 0x14, 0x11c},            // "luxAttack"
        {0xe09173, Kind::IntArray, LUX_SUPPRESS, 0x14, 0x11c},          // "luxSuppress"
        {0xe0917f, Kind::IntArray, BURST_ATTACK, 0x14, 0x11c},          // "burstAttack"
        {0xe0918b, Kind::IntArray, BURST_SUPPRESS, 0x14, 0x11c},        // "burstSuppress"
        {0xe09199, Kind::IntArray, HP_ATTACK, 0x14, 0x11c},             // "hpAttack"
        {0xe091a2, Kind::IntArray, HP_SUPPRESS, 0x14, 0x11c},           // "hpSuppress"
        {0xe091ad, Kind::IntArray, GUILT_SUPPRESS, 0x14, 0x11c},        // "guiltSuppress"
        {0xe07d5b, Kind::IntArray, REWARD_ID, 0x14, 0x11c},             // "rewardId"
        {0xe091bb, Kind::IntArray, MVP_REWARD_ID, 0x14, 0x11c},         // "mvpRewardId"
        {0xe091c7, Kind::IntArray, DESTROY_ALL_REWARD_ID, 0x14, 0x11c}, // "destroyAllRewardId"
        {0xe091da, Kind::Int, DRAW_HEIGHT, 0x4, 0x100},                 // "drawHeight"
        {0xe08964, Kind::Int, VALID_SKILL, 0x4, 0xfc},                  // "validSkill"
        {0xdfb66a, Kind::IntArray, SKILL_ID, 0x10, 0x11c},              // "skillId"
        {0xe0896f, Kind::IntArray, SKILL_REQUIRE, 0x10, 0x11c},         // "skillRequire"
        {0xe0897c, Kind::IntArray, SKILL_REQUIRE_ARG, 0x10, 0x11c},     // "skillRequireArg"
        {0xe0898c, Kind::IntArray, SKILL_ODDS, 0x10, 0x11c},            // "skillOdds"
        {0xe08996, Kind::Int, PROTECT_POISON, 0x4, 0xf8},               // "protectPoison"
        {0xe089a4, Kind::Int, PROTECT_DEEP_POISON, 0x4, 0xf4},          // "protectDeepPoison"
        {0xe089b6, Kind::Int, PROTECT_SLEEP, 0x4, 0xf0},                // "protectSleep"
        {0xe089c3, Kind::Int, PROTECT_PARALYSIS, 0x4, 0xec},            // "protectParalysis"
        {0xe091e5, Kind::Int, PROTECT_CONFUSION, 0x4, 0xe8},            // "protectConfusion"
        {0xe089d4, Kind::Int, REGIST_POISON, 0x4, 0xe4},                // "registPoison"
        {0xe089e1, Kind::Int, REGIST_DEEP_POISON, 0x4, 0xe0},           // "registDeepPoison"
        {0xe089f2, Kind::Int, REGIST_SLEEP, 0x4, 0xdc},                 // "registSleep"
        {0xe089fe, Kind::Int, REGIST_PARALYSIS, 0x4, 0xd8},             // "registParalysis"
        {0xe091f6, Kind::Int, REGIST_CONFUSION, 0x4, 0xd4},             // "registConfusion"
        {0xe08069, Kind::Int, BUF_ATTACK, 0x4, 0xd0},                   // "bufAttack"
        {0xe0808e, Kind::Int, BUF_DEFENSE, 0x4, 0xcc},                  // "bufDefense"
        {0xe080b6, Kind::Int, BUF_ATTACK_POWER, 0x4, 0xc8},             // "bufAttackPower"
        {0xe080ea, Kind::Int, BUF_DEFENSE_POWER, 0x4, 0xc4},            // "bufDefensePower"
        {0xe08121, Kind::Int, BUF_ATTACK_SPEED, 0x4, 0xc0},             // "bufAttackSpeed"
        {0xe08155, Kind::Int, BUF_DEFENSE_SPEED, 0x4, 0xbc},            // "bufDefenseSpeed"
        {0xe0818c, Kind::Int, BUF_ATTACK_MAGIC, 0x4, 0xb8},             // "bufAttackMagic"
        {0xe081c0, Kind::Int, BUF_DEFENSE_MAGIC, 0x4, 0xb4},            // "bufDefenseMagic"
        {0xe08a0e, Kind::Int, BUF_TURN, 0x4, 0xb0},                     // "bufTurn"
        {0xe08a16, Kind::Int, BUF_COUNT, 0x4, 0xac},                    // "bufCount"
        {0xe08a1f, Kind::IntArray, STATUS_AILMENTS, 0x8, 0x11c},        // "statusAilments"
        {0xe08a2e, Kind::Int, STATUS_AILMENTS_TURN, 0x4, 0xa8},         // "statusAilmentsTurn"
        {0xe08a41, Kind::Int, STATUS_AILMENTS_COUNT, 0x4, 0xa4},        // "statusAilmentsCount"
        {0xe09206, Kind::Int, BGM_RAID, 0x4, 0xa0},                     // "bgmRaid"
    };
    parse_b::parseRow(out, json, {0x1b8, 0x2c4, PARSE_VTABLE, 0x788e85}, FIELDS);
}
RW_FUNCTION(0x7878d8, "master::RaidEnemy::parse", RaidEnemy::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
