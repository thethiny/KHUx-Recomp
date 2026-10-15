#include "Skill.hpp"
#include "ParseUtilB.hpp"

namespace master {

using SkillFunctions = TableFunctions<Skill>;
RW_FUNCTION(0x78bdcc, "master::Skill::getSkillIds", SkillFunctions::getIds);
RW_FUNCTION(0x78bde8, "master::Skill::create", SkillFunctions::create);
RW_FUNCTION(0x78c92c, "master::Skill::getHash", SkillFunctions::getHash);
RW_FUNCTION(0x78ca18, "master::Skill::getID", SkillFunctions::getID);
RW_FUNCTION(0x78ca64, "master::Skill::~Skill (deleting)", SkillFunctions::destructDelete);

void Skill::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xdfb66a, Kind::Int, SKILL_ID, 0x4, 0x110},                    // "skillId"
        {0xe08f24, Kind::Int, IMAGE_ID, 0x4, 0x10c},                    // "imageId"
        {0xe08f2c, Kind::Int, THUMB_ID, 0x4, 0x108},                    // "thumbId"
        {0xdfade6, Kind::String, NAME, 0x41, 0x114},                    // "name"
        {0xdfae44, Kind::String, DESCRIPTION, 0x81, 0x114},             // "description"
        {0xdfaa03, Kind::Int, RANK, 0x4, 0x104},                        // "rank"
        {0xe08f66, Kind::Int, MAX_LV, 0x4, 0x100},                      // "maxLv"
        {0xe08f6c, Kind::Int, EXP_TYPE, 0x4, 0xfc},                     // "expType"
        {0xdfae85, Kind::Int, SORT_ID, 0x4, 0xf8},                      // "sortId"
        {0xe0940b, Kind::Int, CATEGORY_EVOLVE, 0x4, 0xf4},              // "categoryEvolve"
        {0xe0941a, Kind::Int, INVOKE_TYPE, 0x4, 0xf0},                  // "invokeType"
        {0xe09425, Kind::Int, INVOKE_PROB, 0x4, 0xec},                  // "invokeProb"
        {0xe09430, Kind::Int, INVOKE_PROB_PER_LV, 0x4, 0xe8},           // "invokeProbPerLv"
        {0xe09440, Kind::Int, INVOKE_SHOW_TIMING, 0x4, 0xe4},           // "invokeShowTiming"
        {0xe09451, Kind::Int, ATTACK_UP, 0x4, 0xe0},                    // "attackUp"
        {0xe0945a, Kind::Int, BURST_CANCEL, 0x4, 0xdc},                 // "burstCancel"
        {0xe09466, Kind::Int, DAMAGE_CUT, 0x4, 0xd8},                   // "damageCut"
        {0xe09470, Kind::Int, DOKONJO, 0x4, 0xd4},                      // "dokonjo"
        {0xe09478, Kind::Int, COUNTER, 0x4, 0xd0},                      // "counter"
        {0xe09480, Kind::Int, ADD_ACTION, 0x4, 0xcc},                   // "addAction"
        {0xe0948a, Kind::Int, ADD_POISON, 0x4, 0xc8},                   // "addPoison"
        {0xe09494, Kind::Int, ADD_DEEP_POISON, 0x4, 0xc4},              // "addDeepPoison"
        {0xe094a2, Kind::Int, ADD_SLEEP, 0x4, 0xc0},                    // "addSleep"
        {0xe094ab, Kind::Int, ADD_PARALYSIS, 0x4, 0xbc},                // "addParalysis"
        {0xe094b8, Kind::Int, CONDITION_TURN, 0x4, 0xb8},               // "conditionTurn"
        {0xe094c6, Kind::Int, CONDITION_COUNT, 0x4, 0xb4},              // "conditionCount"
        {0xe094d5, Kind::Int, PRIZE_HP_UP, 0x4, 0xb0},                  // "prizeHpUp"
        {0xe094df, Kind::Int, PRIZE_BURST_UP, 0x4, 0xac},               // "prizeBurstUp"
        {0xe094ec, Kind::Int, LUX_UP, 0x4, 0xa8},                       // "luxUp"
        {0xe094f2, Kind::Int, MONEY_UP, 0x4, 0xa4},                     // "moneyUp"
        {0xe094fa, Kind::Int, RAID_UP, 0x4, 0xa0},                      // "raidUp"
    };
    parse_b::parseRow(out, json, {0x1b0, 0x138, PARSE_VTABLE, 0x78c8d9}, FIELDS);
}
RW_FUNCTION(0x78bf7c, "master::Skill::parse", Skill::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
