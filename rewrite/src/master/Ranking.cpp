#include "Ranking.hpp"
#include "ParseUtilA.hpp"

namespace master {

using RankingFunctions = TableFunctions<Ranking>;
RW_FUNCTION(0x7912b0, "master::Ranking::getRankingIds", RankingFunctions::getIds);
RW_FUNCTION(0x7912cc, "master::Ranking::create", RankingFunctions::create);
RW_FUNCTION(0x791884, "master::Ranking::getHash", RankingFunctions::getHash);
RW_FUNCTION(0x791970, "master::Ranking::getID", RankingFunctions::getID);
RW_FUNCTION(0x7919bc, "master::Ranking::~Ranking (deleting)", RankingFunctions::destructDelete);

void Ranking::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe08625, Kind::Int, RANKING_ID, 0x4, 0x40},                 // "rankingId"
        {0xdfb643, Kind::Int, CATEGORY, 0x4, 0x3c},                   // "category"
        {0xe09591, Kind::Date, START_DATETIME, 0x8, 0x44},            // "startDatetime"
        {0xdfaf72, Kind::Date, END_DATETIME, 0x8, 0x44},              // "endDatetime"
        {0xe0939f, Kind::Int, START_RANK, 0x4, 0x38},                 // "startRank"
        {0xe093a9, Kind::Int, END_RANK, 0x4, 0x34},                   // "endRank"
        {0xe09a14, Kind::IntArray, UNION_START_RANK, 0x14, 0x44},     // "unionStartRank"
        {0xe09a23, Kind::IntArray, UNION_END_RANK, 0x14, 0x44},       // "unionEndRank"
        {0xe08575, Kind::Int, VALID_REWARD, 0x4, 0x30},               // "validReward"
    };
    parse_a::parseRow(out, json, {0x70, 0x4c, PARSE_VTABLE, 0x791831}, FIELDS);
}
RW_FUNCTION(0x791460, "master::Ranking::parse", Ranking::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
