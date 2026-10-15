#include "RankingReward.hpp"
#include "ParseUtilC.hpp"

namespace master {

using RankingRewardFunctions = TableFunctions<RankingReward>;
RW_FUNCTION(0x78ad9c, "master::RankingReward::getRankingRewardIds", RankingRewardFunctions::getIds);
RW_FUNCTION(0x78adb8, "master::RankingReward::create", RankingRewardFunctions::create);
RW_FUNCTION(0x78b460, "master::RankingReward::getHash", RankingRewardFunctions::getHash);
RW_FUNCTION(0x78b54c, "master::RankingReward::getID", RankingRewardFunctions::getID);
RW_FUNCTION(0x78b598, "master::RankingReward::~RankingReward (deleting)", RankingRewardFunctions::destructDelete);

void RankingReward::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    constexpr uint32_t TYPE_KEY = 0xdfa938;         // "type"
    constexpr uint32_t ID_KEY = 0xdfa935;           // "id"
    constexpr uint32_t SKILL_TYPE_KEY = 0xe0869b;   // "skillType"
    constexpr uint32_t SKILL_ID_KEY = 0xdfb66a;     // "skillId"
    constexpr uint32_t SKILL_LV_KEY = 0xe093b1;     // "skillLv"
    constexpr uint32_t NUM_KEY = 0xe07d90;          // "num"
    constexpr uint32_t BUF = 0x38;                  // frame buffer shared by the arrays (stale entries kept)
    guest::Frame f(0x78);
    RowBuilder b(f);
    uint32_t row = 0;
    auto intMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isInt(v); };
    uint32_t id, startRank, endRank, validReward;
    if (intMember(0xe0938f, id) &&                  // "rankingRewardId"
        intMember(0xe0939f, startRank) &&           // "startRank"
        intMember(0xe093a9, endRank) &&             // "endRank"
        intMember(0xe08575, validReward) &&         // "validReward"
        isArray(member(json, TYPE_KEY)) && isArray(member(json, ID_KEY)) &&
        isArray(member(json, SKILL_TYPE_KEY)) && isArray(member(json, SKILL_ID_KEY)) &&
        isArray(member(json, SKILL_LV_KEY)) && isArray(member(json, NUM_KEY))) {
        b.create(SIZE, VTABLE);
        b.int32(RANKING_REWARD_ID, intOf(id), 0x34);
        b.int32(START_RANK, intOf(startRank), 0x30);
        b.int32(END_RANK, intOf(endRank), 0x2c);
        b.int32(VALID_REWARD, intOf(validReward), 0x28);
        if (b.int32Array(TYPE, member(json, TYPE_KEY), 10, BUF) &&
            b.int32Array(ID, member(json, ID_KEY), 10, BUF) &&
            b.int32Array(SKILL_TYPE, member(json, SKILL_TYPE_KEY), 10, BUF) &&
            b.int32Array(SKILL_ID, member(json, SKILL_ID_KEY), 10, BUF) &&
            b.int32Array(SKILL_LV, member(json, SKILL_LV_KEY), 10, BUF) &&
            b.int32Array(NUM, member(json, NUM_KEY), 10, BUF)) {
            row = b.obj();
        } else {
            b.destroy();
        }
    }
    makeShared(out, SHARED_PTR_CTOR, row);
}
RW_FUNCTION(0x78AF4C, "master::RankingReward::parse", RankingReward::parse);

}  // namespace master
