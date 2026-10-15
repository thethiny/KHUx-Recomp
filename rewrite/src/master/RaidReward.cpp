#include "RaidReward.hpp"
#include "ParseUtilA.hpp"

namespace master {

using RaidRewardFunctions = TableFunctions<RaidReward>;
RW_FUNCTION(0x789ddc, "master::RaidReward::getRaidRewardIds", RaidRewardFunctions::getIds);
RW_FUNCTION(0x789df8, "master::RaidReward::create", RaidRewardFunctions::create);
RW_FUNCTION(0x78a4cc, "master::RaidReward::getHash", RaidRewardFunctions::getHash);
RW_FUNCTION(0x78a5b8, "master::RaidReward::getID", RaidRewardFunctions::getID);
RW_FUNCTION(0x78a604, "master::RaidReward::~RaidReward (deleting)", RaidRewardFunctions::destructDelete);

void RaidReward::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe09272, Kind::Int, RAID_REWARD_ID, 0x4, 0x3c},             // "raidRewardId"
        {0xe0927f, Kind::Int, ODDS_UP_PER_LV, 0x4, 0x38},             // "oddsUpPerLv"
        {0xe0928b, Kind::Int, POINT_UP_PER_LV, 0x4, 0x34},            // "pointUpPerLv"
        {0xe08575, Kind::Int, VALID_REWARD, 0x4, 0x30},               // "validReward"
        {0xdfa938, Kind::IntArray, TYPE, 0x10, 0x40},                 // "type"
        {0xdfa935, Kind::IntArray, ID, 0x10, 0x40},                   // "id"
        {0xe07d64, Kind::IntArray, ASSIGN_SKILL_TYPE, 0x10, 0x40},    // "assignSkillType"
        {0xe07d74, Kind::IntArray, ASSIGN_SKILL_ID, 0x10, 0x40},      // "assignSkillId"
        {0xe07d82, Kind::IntArray, ASSIGN_SKILL_LV, 0x10, 0x40},      // "assignSkillLv"
        {0xe07d90, Kind::IntArray, NUM, 0x10, 0x40},                  // "num"
        {0xe09298, Kind::IntArray, ODDS, 0x10, 0x40},                 // "odds"
    };
    parse_a::parseRow(out, json, {0x68, 0x80, PARSE_VTABLE, 0x78a479}, FIELDS);
}
RW_FUNCTION(0x789f8c, "master::RaidReward::parse", RaidReward::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
