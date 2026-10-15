#include "AlbumChallenge.hpp"
#include "ParseUtilD.hpp"

namespace master {

using AlbumChallengeFunctions = TableFunctions<AlbumChallenge>;
RW_FUNCTION(0x778178, "master::AlbumChallenge::getChallengeIds", AlbumChallengeFunctions::getIds);
RW_FUNCTION(0x778194, "master::AlbumChallenge::create", AlbumChallengeFunctions::create);
RW_FUNCTION(0x7787b4, "master::AlbumChallenge::getHash", AlbumChallengeFunctions::getHash);
RW_FUNCTION(0x7788a0, "master::AlbumChallenge::getID", AlbumChallengeFunctions::getID);
RW_FUNCTION(0x7788ec, "master::AlbumChallenge::~AlbumChallenge (deleting)", AlbumChallengeFunctions::destructDelete);

void AlbumChallenge::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xdfb5f4, Kind::Int, CHALLENGE_ID, 0x4, 0x78},                     // "challengeId"
        {0xdfae85, Kind::Int, SORT_ID, 0x4, 0x74},                          // "sortId"
        {0xe07d05, Kind::Int, VALID, 0x4, 0x70},                            // "valid"
        {0xe07d34, Kind::String, SUBMISSION_NAME, 0x101, 0x7c},             // "submissionName"
        {0xdfa938, Kind::Int, TYPE, 0x4, 0x6c},                             // "type"
        {0xdfb52f, Kind::Int, ATTRIBUTE, 0x4, 0x68},                        // "attribute"
        {0xe07d43, Kind::Int, TARGET_NUMBER, 0x4, 0x64},                    // "targetNumber"
        {0xe07d50, Kind::Int, REWARD_TYPE, 0x4, 0x60},                      // "rewardType"
        {0xe07d5b, Kind::Int, REWARD_ID, 0x4, 0x5c},                        // "rewardId"
        {0xe07d64, Kind::Int, ASSIGN_SKILL_TYPE, 0x4, 0x58},                // "assignSkillType"
        {0xe07d74, Kind::Int, ASSIGN_SKILL_ID, 0x4, 0x54},                  // "assignSkillId"
        {0xe07d82, Kind::Int, ASSIGN_SKILL_LV, 0x4, 0x50},                  // "assignSkillLv"
        {0xe07d90, Kind::Int, NUM, 0x4, 0x4c},                              // "num"
    };
    parse_d::parseRow(out, json, {0x198, 0x134, PARSE_VTABLE, 0x778761}, FIELDS);
}
RW_FUNCTION(0x778348, "master::AlbumChallenge::parse", AlbumChallenge::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
