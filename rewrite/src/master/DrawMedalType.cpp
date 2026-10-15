#include "DrawMedalType.hpp"
#include "ParseUtilB.hpp"

namespace master {

using DrawMedalTypeFunctions = TableFunctions<DrawMedalType>;
RW_FUNCTION(0x77dcc4, "master::DrawMedalType::getDrawMedalTypeIds", DrawMedalTypeFunctions::getIds);
RW_FUNCTION(0x77dce0, "master::DrawMedalType::create", DrawMedalTypeFunctions::create);
RW_FUNCTION(0x77eb7c, "master::DrawMedalType::getHash", DrawMedalTypeFunctions::getHash);
RW_FUNCTION(0x77ec68, "master::DrawMedalType::getID", DrawMedalTypeFunctions::getID);
RW_FUNCTION(0x77ecb4, "master::DrawMedalType::~DrawMedalType (deleting)", DrawMedalTypeFunctions::destructDelete);

void DrawMedalType::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xdfae95, Kind::Int, DRAW_MEDAL_TYPE_ID, 0x4, 0xc0},           // "drawMedalTypeId"
        {0xdfae8c, Kind::Int, DRAW_TYPE, 0x4, 0xbc},                    // "drawType"
        {0xdfae85, Kind::Int, SORT_ID, 0x4, 0xb8},                      // "sortId"
        {0xdfade6, Kind::String, NAME, 0x81, 0xc4},                     // "name"
        {0xdfae44, Kind::String, DESCRIPTION, 0x101, 0xc4},             // "description"
        {0xdfae7d, Kind::Int, PAY_TYPE, 0x4, 0xb4},                     // "payType"
        {0xdfae77, Kind::Int, PRICE, 0x4, 0xb0},                        // "price"
        {0xe0868c, Kind::Int, TABLE_ID, 0x4, 0xac},                     // "tableId"
        {0xe086d8, Kind::Int, VALID_LOT, 0x4, 0xa8},                    // "validLot"
        {0xdfae6e, Kind::IntArray, LOT_COUNT, 0xc, 0xc4},               // "lotCount"
        {0xe086e1, Kind::IntArray, LOT_PSR, 0xc, 0xc4},                 // "lotPsr"
        {0xe086e8, Kind::IntArray, LOT_SR, 0xc, 0xc4},                  // "lotSr"
        {0xe086ee, Kind::IntArray, LOT_PR, 0xc, 0xc4},                  // "lotPr"
        {0xe086f4, Kind::IntArray, LOT_R, 0xc, 0xc4},                   // "lotR"
        {0xe086f9, Kind::IntArray, LOT_PN, 0xc, 0xc4},                  // "lotPn"
        {0xe086ff, Kind::IntArray, LOT_N, 0xc, 0xc4},                   // "lotN"
        {0xe08704, Kind::IntArray, TABLE_IDS, 0xc, 0xc4},               // "tableIds"
        {0xdfaea5, Kind::Int, VALID_ADD, 0x4, 0xa4},                    // "validAdd"
        {0xdfae28, Kind::Int, ADD_TYPE, 0x4, 0xa0},                     // "addType"
        {0xe0870d, Kind::Int, ADD_ID, 0x4, 0x9c},                       // "addId"
        {0xe08713, Kind::Int, ADD_SKILL_TYPE, 0x4, 0x98},               // "addSkillType"
        {0xe08720, Kind::Int, ADD_SKILL_ID, 0x4, 0x94},                 // "addSkillId"
        {0xe0872b, Kind::Int, ADD_SKILL_LV, 0x4, 0x90},                 // "addSkillLv"
        {0xe08736, Kind::Int, ADD_NUM, 0x4, 0x8c},                      // "addNum"
        {0xdfae3a, Kind::String, BANNER_URL, 0x201, 0xc4},              // "bannerUrl"
        {0xe0873d, Kind::Int, MYPAGE_BANNER, 0x4, 0x88},                // "mypageBanner"
        {0xdfb98a, Kind::Int, DAY_COUNT, 0x4, 0x84},                    // "dayCount"
        {0xe0874a, Kind::Int, WARRANT_NUMBER_OF_TIME, 0x4, 0x80},       // "warrantNumberOfTime"
        {0xe0875e, Kind::IntArray, WARRANT_MEDAL_LIST_IDS, 0x4, 0xc4},  // "warrantMedalListIds"
        {0xe08772, Kind::Int, WARRANT_MEDAL_REPEAT, 0x4, 0x7c},         // "warrantMedalRepeat"
        {0xe08785, Kind::IntArray, WARRANT_RESET_MEDAL_LIST_IDS, 0x28, 0xc4}, // "warrantResetMedalListIds"
        {0xe0879e, Kind::Date, START_DATE, 0x8, 0xc4},                  // "startDate"
        {0xe087a8, Kind::Date, END_DATE, 0x8, 0xc4},                    // "endDate"
        {0xe086a5, Kind::Int, IS_DELETED, 0x4, 0x78},                   // "isDeleted"
    };
    parse_b::parseRow(out, json, {0x2e0, 0x470, PARSE_VTABLE, 0x77eb29}, FIELDS);
}
RW_FUNCTION(0x77de74, "master::DrawMedalType::parse", DrawMedalType::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
