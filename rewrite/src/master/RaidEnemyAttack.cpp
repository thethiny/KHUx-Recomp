#include "RaidEnemyAttack.hpp"
#include "ParseUtilD.hpp"

namespace master {

using RaidEnemyAttackFunctions = TableFunctions<RaidEnemyAttack>;
RW_FUNCTION(0x78901c, "master::RaidEnemyAttack::getRaidEnemyAttackIds", RaidEnemyAttackFunctions::getIds);
RW_FUNCTION(0x789038, "master::RaidEnemyAttack::create", RaidEnemyAttackFunctions::create);
RW_FUNCTION(0x789c98, "master::RaidEnemyAttack::getHash", RaidEnemyAttackFunctions::getHash);
RW_FUNCTION(0x789d84, "master::RaidEnemyAttack::getID", RaidEnemyAttackFunctions::getID);
RW_FUNCTION(0x789dd0, "master::RaidEnemyAttack::~RaidEnemyAttack (deleting)", RaidEnemyAttackFunctions::destructDelete);

void RaidEnemyAttack::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xe09237, Kind::Int, RAID_ENEMY_ATTACK_ID, 0x4, 0x138},            // "raidEnemyAttackId"
        {0xdfade6, Kind::String, NAME, 0x41, 0x13c},                        // "name"
        {0xdfb361, Kind::Int, DISPLAY, 0x4, 0x134},                         // "display"
        {0xe08aa3, Kind::Int, CHOISE_ATTRIBUTE, 0x4, 0x130},                // "choiseAttribute"
        {0xe07f86, Kind::Int, HIT_COUNT, 0x4, 0x12c},                       // "hitCount"
        {0xe07e96, Kind::Int, POWER, 0x4, 0x128},                           // "power"
        {0xe07f95, Kind::Int, CRITICAL, 0x4, 0x124},                        // "critical"
        {0xe07f9e, Kind::Int, CRITICAL_POWER, 0x4, 0x120},                  // "criticalPower"
        {0xe07e8f, Kind::Int, TARGET, 0x4, 0x11c},                          // "target"
        {0xe07e88, Kind::Int, EFFECT, 0x4, 0x118},                          // "effect"
        {0xe08ab3, Kind::Int, ADD_TARGET, 0x4, 0x114},                      // "addTarget"
        {0xe08058, Kind::Int, ADD_TURN, 0x4, 0x110},                        // "addTurn"
        {0xe08060, Kind::Int, ADD_COUNT, 0x4, 0x10c},                       // "addCount"
        {0xe08069, Kind::Int, BUF_ATTACK, 0x4, 0x108},                      // "bufAttack"
        {0xe0808e, Kind::Int, BUF_DEFENSE, 0x4, 0x104},                     // "bufDefense"
        {0xe080b6, Kind::Int, BUF_ATTACK_POWER, 0x4, 0x100},                // "bufAttackPower"
        {0xe080ea, Kind::Int, BUF_DEFENSE_POWER, 0x4, 0xfc},                // "bufDefensePower"
        {0xe08121, Kind::Int, BUF_ATTACK_SPEED, 0x4, 0xf8},                 // "bufAttackSpeed"
        {0xe08155, Kind::Int, BUF_DEFENSE_SPEED, 0x4, 0xf4},                // "bufDefenseSpeed"
        {0xe0818c, Kind::Int, BUF_ATTACK_MAGIC, 0x4, 0xf0},                 // "bufAttackMagic"
        {0xe081c0, Kind::Int, BUF_DEFENSE_MAGIC, 0x4, 0xec},                // "bufDefenseMagic"
        {0xe08abd, Kind::Int, RESET_ATTACK, 0x4, 0xe8},                     // "resetAttack"
        {0xe08ac9, Kind::Int, RESET_DEFENSE, 0x4, 0xe4},                    // "resetDefense"
        {0xe08ad6, Kind::Int, RESET_ATTACK_POWER, 0x4, 0xe0},               // "resetAttackPower"
        {0xe08ae7, Kind::Int, RESET_DEFENSE_POWER, 0x4, 0xdc},              // "resetDefensePower"
        {0xe08af9, Kind::Int, RESET_ATTACK_SPEED, 0x4, 0xd8},               // "resetAttackSpeed"
        {0xe08b0a, Kind::Int, RESET_DEFENSE_SPEED, 0x4, 0xd4},              // "resetDefenseSpeed"
        {0xe08b1c, Kind::Int, RESET_ATTACK_MAGIC, 0x4, 0xd0},               // "resetAttackMagic"
        {0xe08b2d, Kind::Int, RESET_DEFENSE_MAGIC, 0x4, 0xcc},              // "resetDefenseMagic"
        {0xe0841e, Kind::Int, HEAL, 0x4, 0xc8},                             // "heal"
        {0xe08423, Kind::Int, HEAL_SHAKE, 0x4, 0xc4},                       // "healShake"
        {0xe08b3f, Kind::Int, POISON, 0x4, 0xc0},                           // "poison"
        {0xe08b46, Kind::Int, DEEP_POISON, 0x4, 0xbc},                      // "deepPoison"
        {0xe08b51, Kind::Int, SLEEP, 0x4, 0xb8},                            // "sleep"
        {0xe08b57, Kind::Int, PARALYSIS, 0x4, 0xb4},                        // "paralysis"
        {0xe0842d, Kind::Int, CURE_POISON, 0x4, 0xb0},                      // "curePoison"
        {0xe08438, Kind::Int, CURE_SLEEP, 0x4, 0xac},                       // "cureSleep"
        {0xe08451, Kind::Int, CURE_PARALYSIS, 0x4, 0xa8},                   // "cureParalysis"
    };
    parse_d::parseRow(out, json, {0x198, 0xd8, PARSE_VTABLE, 0x789c45}, FIELDS);
}
RW_FUNCTION(0x7891cc, "master::RaidEnemyAttack::parse", RaidEnemyAttack::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
