#include "EvCampaign.hpp"
#include "ParseUtilB.hpp"

namespace master {

using EvCampaignFunctions = TableFunctions<EvCampaign>;
RW_FUNCTION(0x781dbc, "master::EvCampaign::getCampaignIds", EvCampaignFunctions::getIds);
RW_FUNCTION(0x781dd8, "master::EvCampaign::create", EvCampaignFunctions::create);
RW_FUNCTION(0x7825dc, "master::EvCampaign::getHash", EvCampaignFunctions::getHash);
RW_FUNCTION(0x7826c8, "master::EvCampaign::getID", EvCampaignFunctions::getID);
RW_FUNCTION(0x782714, "master::EvCampaign::~EvCampaign (deleting)", EvCampaignFunctions::destructDelete);

void EvCampaign::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe08b8a, Kind::Int, CAMPAIGN_ID, 0x4, 0x90},                  // "campaignId"
        {0xdfa938, Kind::Int, TYPE, 0x4, 0x8c},                         // "type"
        {0xe07e96, Kind::Int, POWER, 0x4, 0x88},                        // "power"
        {0xe08b95, Kind::String, DISPLAY_TEXT, 0x101, 0x94},            // "displayText"
        {0xe08ba1, Kind::Int, STAGE_TYPE, 0x4, 0x84},                   // "stageType"
        {0xe08bab, Kind::Int, VALID_STAGE_ID, 0x4, 0x80},               // "validStageId"
        {0xdfaef5, Kind::IntArray, STAGE_ID, 0x14, 0x94},               // "stageId"
        {0xe08bb8, Kind::Date, START_TIME, 0x8, 0x94},                  // "startTime"
        {0xdfb479, Kind::Date, END_TIME, 0x8, 0x94},                    // "endTime"
        {0xe08bc2, Kind::Int, WEEK_MONDAY, 0x4, 0x7c},                  // "weekMonday"
        {0xe08bcd, Kind::Int, WEEK_TUESDAY, 0x4, 0x78},                 // "weekTuesday"
        {0xe08bd9, Kind::Int, WEEK_WEDNESDAY, 0x4, 0x74},               // "weekWednesday"
        {0xe08be7, Kind::Int, WEEK_THURSDAY, 0x4, 0x70},                // "weekThursday"
        {0xe08bf4, Kind::Int, WEEK_FRIDAY, 0x4, 0x6c},                  // "weekFriday"
        {0xe08bff, Kind::Int, WEEK_SATURDAY, 0x4, 0x68},                // "weekSaturday"
        {0xe08c0c, Kind::Int, WEEK_SUNDAY, 0x4, 0x64},                  // "weekSunday"
        {0xe08c17, Kind::Int, GROUP_PATTERN, 0x4, 0x60},                // "groupPattern"
        {0xe08c24, Kind::Int, VALID_MINUTE, 0x4, 0x5c},                 // "validMinute"
    };
    parse_b::parseRow(out, json, {0x1b0, 0x160, PARSE_VTABLE, 0x782589}, FIELDS);
}
RW_FUNCTION(0x781f6c, "master::EvCampaign::parse", EvCampaign::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
