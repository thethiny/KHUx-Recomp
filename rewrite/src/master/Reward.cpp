#include "Reward.hpp"
#include "ParseUtilB.hpp"

namespace master {

using RewardFunctions = TableFunctions<Reward>;
RW_FUNCTION(0x78b5a4, "master::Reward::getRewardIds", RewardFunctions::getIds);
RW_FUNCTION(0x78b5c0, "master::Reward::create", RewardFunctions::create);
RW_FUNCTION(0x78bc88, "master::Reward::getHash", RewardFunctions::getHash);
RW_FUNCTION(0x78bd74, "master::Reward::getID", RewardFunctions::getID);
RW_FUNCTION(0x78bdc0, "master::Reward::~Reward (deleting)", RewardFunctions::destructDelete);

void Reward::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe07d5b, Kind::Int, REWARD_ID, 0x4, 0x24},                    // "rewardId"
        {0xe08575, Kind::Int, VALID_REWARD, 0x4, 0x20},                 // "validReward"
        {0xdfb361, Kind::IntArray, DISPLAY, 0x10, 0x28},                // "display"
        {0xdfa938, Kind::IntArray, TYPE, 0x10, 0x28},                   // "type"
        {0xdfa935, Kind::IntArray, ID, 0x10, 0x28},                     // "id"
        {0xe07d64, Kind::IntArray, ASSIGN_SKILL_TYPE, 0x10, 0x28},      // "assignSkillType"
        {0xe07d74, Kind::IntArray, ASSIGN_SKILL_ID, 0x10, 0x28},        // "assignSkillId"
        {0xe07d82, Kind::IntArray, ASSIGN_SKILL_LV, 0x10, 0x28},        // "assignSkillLv"
        {0xe07d90, Kind::IntArray, NUM, 0x10, 0x28},                    // "num"
        {0xe09298, Kind::IntArray, ODDS, 0x10, 0x28},                   // "odds"
    };
    parse_b::parseRow(out, json, {0x50, 0x88, PARSE_VTABLE, 0x78bc35}, FIELDS);
}
RW_FUNCTION(0x78b754, "master::Reward::parse", Reward::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
