#include "Colosseum.hpp"
#include "ParseUtilD.hpp"

namespace master {

using ColosseumFunctions = TableFunctions<Colosseum>;
RW_FUNCTION(0x77c4cc, "master::Colosseum::getColosseumIds", ColosseumFunctions::getIds);
RW_FUNCTION(0x77c4e8, "master::Colosseum::create", ColosseumFunctions::create);
RW_FUNCTION(0x77ce34, "master::Colosseum::getHash", ColosseumFunctions::getHash);
RW_FUNCTION(0x77cf20, "master::Colosseum::getID", ColosseumFunctions::getID);
RW_FUNCTION(0x77cf6c, "master::Colosseum::~Colosseum (deleting)", ColosseumFunctions::destructDelete);

void Colosseum::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xe084bb, Kind::Int, COLOSSEUM_ID, 0x4, 0x88},                     // "colosseumId"
        {0xdfaf54, Kind::Int, COLOSSEUM_STAGE_ID, 0x4, 0x84},               // "colosseumStageId"
        {0xe084c7, Kind::Int, ROUND, 0x4, 0x80},                            // "round"
        {0xdfb8cb, Kind::Int, AP, 0x4, 0x7c},                               // "ap"
        {0xe084cd, Kind::Int, ARRIVAL_STAGE_COUNT, 0x4, 0x78},              // "arrivalStageCount"
        {0xe084df, Kind::String, RANK_ID, 0x41, 0x8c},                      // "rankId"
        {0xe084e6, Kind::Int, EXCELLENT_UNLOCK_COUNT, 0x4, 0x74},           // "excellentUnlockCount"
        {0xe084fb, Kind::Int, VALID_STAGE_REWARD, 0x4, 0x70},               // "validStageReward"
        {0xe0850c, Kind::Int, STAGE_REWARD_TYPE, 0x4, 0x6c},                // "stageRewardType"
        {0xe0851c, Kind::Int, STAGE_REWARD_ID, 0x4, 0x68},                  // "stageRewardId"
        {0xe0852a, Kind::Int, STAGE_REWARD_ASSIGN_TYPE, 0x4, 0x64},         // "stageRewardAssignType"
        {0xe08540, Kind::Int, STAGE_REWARD_SKILL_ID, 0x4, 0x60},            // "stageRewardSkillId"
        {0xe08553, Kind::Int, STAGE_REWARD_SKILL_LV, 0x4, 0x5c},            // "stageRewardSkillLv"
        {0xe08566, Kind::Int, STAGE_REWARD_NUM, 0x4, 0x58},                 // "stageRewardNum"
        {0xe08575, Kind::Int, VALID_REWARD, 0x4, 0x54},                     // "validReward"
        {0xe07d50, Kind::IntArray, REWARD_TYPE, 0x10, 0x8c},                // "rewardType"
        {0xe07d5b, Kind::IntArray, REWARD_ID, 0x10, 0x8c},                  // "rewardId"
        {0xe08581, Kind::IntArray, REWARD_ASSIGN_TYPE, 0x10, 0x8c},         // "rewardAssignType"
        {0xe08592, Kind::IntArray, REWARD_SKILL_ID, 0x10, 0x8c},            // "rewardSkillId"
        {0xe085a0, Kind::IntArray, REWARD_SKILL_LV, 0x10, 0x8c},            // "rewardSkillLv"
        {0xe085ae, Kind::IntArray, REWARD_NUM, 0x10, 0x8c},                 // "rewardNum"
    };
    parse_d::parseRow(out, json, {0xe8, 0xdc, PARSE_VTABLE, 0x77cde1}, FIELDS);
}
RW_FUNCTION(0x77c67c, "master::Colosseum::parse", Colosseum::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
