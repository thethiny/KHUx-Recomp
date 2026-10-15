#include "EvStage.hpp"
#include "ParseUtilA.hpp"

namespace master {

using EvStageFunctions = TableFunctions<EvStage>;
RW_FUNCTION(0x782d10, "master::EvStage::getEventStageIds", EvStageFunctions::getIds);
RW_FUNCTION(0x782d2c, "master::EvStage::create", EvStageFunctions::create);
RW_FUNCTION(0x7838a0, "master::EvStage::getHash", EvStageFunctions::getHash);
RW_FUNCTION(0x78398c, "master::EvStage::getID", EvStageFunctions::getID);
RW_FUNCTION(0x7839d8, "master::EvStage::~EvStage (deleting)", EvStageFunctions::destructDelete);

void EvStage::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xdfb494, Kind::Int, EVENT_STAGE_ID, 0x4, 0xe8},             // "eventStageId"
        {0xe08bab, Kind::Int, VALID_STAGE_ID, 0x4, 0xe4},             // "validStageId"
        {0xdfaef5, Kind::IntArray, STAGE_ID, 0x14, 0xec},             // "stageId"
        {0xe08ca9, Kind::Int, VALID_STAGE_WIDTH_ID, 0x4, 0xe0},       // "validStageWidthId"
        {0xe08cbb, Kind::Int, STAGE_START_ID, 0x4, 0xdc},             // "stageStartId"
        {0xe08cc8, Kind::Int, STAGE_END_ID, 0x4, 0xd8},               // "stageEndId"
        {0xdfbc4f, Kind::Int, EVENT_CATEGORY, 0x4, 0xd4},             // "eventCategory"
        {0xdfb481, Kind::Int, EVENT_TYPE, 0x4, 0xd0},                 // "eventType"
        {0xdfae3a, Kind::String, BANNER_URL, 0x201, 0xec},            // "bannerUrl"
        {0xe0873d, Kind::Int, MYPAGE_BANNER, 0x4, 0xcc},              // "mypageBanner"
        {0xe08bb8, Kind::Date, START_TIME, 0x8, 0xec},                // "startTime"
        {0xdfb479, Kind::Date, END_TIME, 0x8, 0xec},                  // "endTime"
        {0xe08cd3, Kind::Date, BOARD_END_TIME, 0x8, 0xec},            // "boardEndTime"
        {0xe08bc2, Kind::Int, WEEK_MONDAY, 0x4, 0xc8},                // "weekMonday"
        {0xe08bcd, Kind::Int, WEEK_TUESDAY, 0x4, 0xc4},               // "weekTuesday"
        {0xe08bd9, Kind::Int, WEEK_WEDNESDAY, 0x4, 0xc0},             // "weekWednesday"
        {0xe08be7, Kind::Int, WEEK_THURSDAY, 0x4, 0xbc},              // "weekThursday"
        {0xe08bf4, Kind::Int, WEEK_FRIDAY, 0x4, 0xb8},                // "weekFriday"
        {0xe08bff, Kind::Int, WEEK_SATURDAY, 0x4, 0xb4},              // "weekSaturday"
        {0xe08c0c, Kind::Int, WEEK_SUNDAY, 0x4, 0xb0},                // "weekSunday"
        {0xdfaf7e, Kind::Int, BENEFIT_ID, 0x4, 0xac},                 // "benefitId"
        {0xe08ce0, Kind::Int, BENEFIT_MONDAY, 0x4, 0xa8},             // "benefitMonday"
        {0xe08cee, Kind::Int, BENEFIT_TUESDAY, 0x4, 0xa4},            // "benefitTuesday"
        {0xe08cfd, Kind::Int, BENEFIT_WEDNESDAY, 0x4, 0xa0},          // "benefitWednesday"
        {0xe08d0e, Kind::Int, BENEFIT_THURSDAY, 0x4, 0x9c},           // "benefitThursday"
        {0xe08d1e, Kind::Int, BENEFIT_FRIDAY, 0x4, 0x98},             // "benefitFriday"
        {0xe08d2c, Kind::Int, BENEFIT_SATURDAY, 0x4, 0x94},           // "benefitSaturday"
        {0xe08d3c, Kind::Int, BENEFIT_SUNDAY, 0x4, 0x90},             // "benefitSunday"
        {0xe08c17, Kind::Int, GROUP_PATTERN, 0x4, 0x8c},              // "groupPattern"
        {0xe08c24, Kind::Int, VALID_MINUTE, 0x4, 0x88},               // "validMinute"
    };
    parse_a::parseRow(out, json, {0x308, 0x294, PARSE_VTABLE, 0x78384d}, FIELDS);
}
RW_FUNCTION(0x782ec0, "master::EvStage::parse", EvStage::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
