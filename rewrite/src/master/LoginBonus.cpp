#include "LoginBonus.hpp"
#include "ParseUtilD.hpp"

namespace master {

using LoginBonusFunctions = TableFunctions<LoginBonus>;
RW_FUNCTION(0x784a5c, "master::LoginBonus::getLoginBonusIds", LoginBonusFunctions::getIds);
RW_FUNCTION(0x784a78, "master::LoginBonus::create", LoginBonusFunctions::create);
RW_FUNCTION(0x785184, "master::LoginBonus::getHash", LoginBonusFunctions::getHash);
RW_FUNCTION(0x785270, "master::LoginBonus::getID", LoginBonusFunctions::getID);
RW_FUNCTION(0x7852bc, "master::LoginBonus::~LoginBonus (deleting)", LoginBonusFunctions::destructDelete);

void LoginBonus::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xdfb993, Kind::Int, LOGIN_BONUS_ID, 0x4, 0x70},                   // "loginBonusId"
        {0xe08e81, Kind::Int, BONUS_TYPE, 0x4, 0x6c},                       // "bonusType"
        {0xe07ba4, Kind::String, TITLE, 0x101, 0x74},                       // "title"
        {0xdfae44, Kind::String, DESCRIPTION, 0x401, 0x74},                 // "description"
        {0xdfb036, Kind::String, COMMENT, 0x401, 0x74},                     // "comment"
        {0xe08e8b, Kind::Int, DAY, 0x4, 0x68},                              // "day"
        {0xe0879e, Kind::Date, START_DATE, 0x8, 0x74},                      // "startDate"
        {0xe087a8, Kind::Date, END_DATE, 0x8, 0x74},                        // "endDate"
        {0xe07d50, Kind::Int, REWARD_TYPE, 0x4, 0x64},                      // "rewardType"
        {0xe07d5b, Kind::Int, REWARD_ID, 0x4, 0x60},                        // "rewardId"
        {0xe08e8f, Kind::Int, REWARD_ASSIGN_SKILL_TYPE, 0x4, 0x5c},         // "rewardAssignSkillType"
        {0xe08ea5, Kind::Int, REWARD_ASSIGN_SKILL_ID, 0x4, 0x58},           // "rewardAssignSkillId"
        {0xe08eb9, Kind::Int, REWARD_ASSIGN_SKILL_LV, 0x4, 0x54},           // "rewardAssignSkillLv"
        {0xe085ae, Kind::Int, REWARD_NUM, 0x4, 0x50},                       // "rewardNum"
    };
    parse_d::parseRow(out, json, {0x490, 0x938, PARSE_VTABLE, 0x785131}, FIELDS);
}
RW_FUNCTION(0x784c0c, "master::LoginBonus::parse", LoginBonus::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
