#include "Shop.hpp"
#include "ParseUtilA.hpp"

namespace master {

using ShopFunctions = TableFunctions<Shop>;
RW_FUNCTION(0x790934, "master::Shop::getShopIds", ShopFunctions::getIds);
RW_FUNCTION(0x790950, "master::Shop::create", ShopFunctions::create);
RW_FUNCTION(0x79116c, "master::Shop::getHash", ShopFunctions::getHash);
RW_FUNCTION(0x791258, "master::Shop::getID", ShopFunctions::getID);
RW_FUNCTION(0x7912a4, "master::Shop::~Shop (deleting)", ShopFunctions::destructDelete);

void Shop::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe09966, Kind::Int, SHOP_ID, 0x4, 0x98},                    // "shopId"
        {0xdfbfca, Kind::String, PRODUCT_ID, 0x81, 0x9c},             // "productId"
        {0xdfae85, Kind::Int, SORT_ID, 0x4, 0x94},                    // "sortId"
        {0xdfa938, Kind::Int, TYPE, 0x4, 0x90},                       // "type"
        {0xe0996d, Kind::Int, OS, 0x4, 0x8c},                         // "os"
        {0xdfae77, Kind::Int, PRICE, 0x4, 0x88},                      // "price"
        {0xdfb260, Kind::Int, NUMBER, 0x4, 0x84},                     // "number"
        {0xe09970, Kind::Int, BONUS, 0x4, 0x80},                      // "bonus"
        {0xe09976, Kind::String, EXPLANATION, 0x81, 0x9c},            // "explanation"
        {0xdfb86a, Kind::Int, STAGE_SKIP_TICKET, 0x4, 0x7c},          // "stageSkipTicket"
        {0xe09982, Kind::Int, APPLE_STATUS, 0x4, 0x78},               // "appleStatus"
        {0xdfaf7e, Kind::Int, BENEFIT_ID, 0x4, 0x74},                 // "benefitId"
        {0xe0998e, Kind::Int, BENEFIT_BONUS_VALUE, 0x4, 0x70},        // "benefitBonusValue"
        {0xe099a0, Kind::String, BENEFIT_RESET_WEEK, 0x81, 0x9c},     // "benefitResetWeek"
        {0xe099b1, Kind::Time, BENEFIT_RESET_TIME, 0x4, 0x9c},        // "benefitResetTime"
        {0xe099c2, Kind::String, BENEFIT_EXPLANATION, 0x81, 0x9c},    // "benefitExplanation"
        {0xe099d5, Kind::String, USE_BENEFIT_EXPLANATION, 0x81, 0x9c}, // "useBenefitExplanation"
        {0xe08f2c, Kind::Int, THUMB_ID, 0x4, 0x6c},                   // "thumbId"
        {0xdfaf4d, Kind::Int, STATUS, 0x4, 0x68},                     // "status"
    };
    parse_a::parseRow(out, json, {0x138, 0x2c4, PARSE_VTABLE, 0x791119}, FIELDS);
}
RW_FUNCTION(0x790ae4, "master::Shop::parse", Shop::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
