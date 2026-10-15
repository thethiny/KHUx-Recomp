#include "SphereMasu.hpp"
#include "ParseUtilA.hpp"

namespace master {

using SphereMasuFunctions = TableFunctions<SphereMasu>;
RW_FUNCTION(0x78dcec, "master::SphereMasu::getMasuIds", SphereMasuFunctions::getIds);
RW_FUNCTION(0x78dd08, "master::SphereMasu::create", SphereMasuFunctions::create);
RW_FUNCTION(0x78e388, "master::SphereMasu::getHash", SphereMasuFunctions::getHash);
RW_FUNCTION(0x78e474, "master::SphereMasu::getID", SphereMasuFunctions::getID);
RW_FUNCTION(0x78e4c0, "master::SphereMasu::~SphereMasu (deleting)", SphereMasuFunctions::destructDelete);

void SphereMasu::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe09579, Kind::Int, MASU_ID, 0x4, 0x20},                    // "masuId"
        {0xdfb29f, Kind::Int, SPHERE_BOARD_ID, 0x4, 0x1c},            // "sphereBoardId"
        {0xdfb8fa, Kind::IntArray, SPHERE_POINT, 0x5c, 0x24},         // "spherePoint"
        {0xe09614, Kind::IntArray, REWARD_SELECT, 0x5c, 0x24},        // "rewardSelect"
        {0xe09621, Kind::IntArray, MEDAL_CATEGORY, 0x5c, 0x24},       // "medalCategory"
        {0xe07d5b, Kind::IntArray, REWARD_ID, 0x5c, 0x24},            // "rewardId"
        {0xe0962f, Kind::IntArray, REWARD_EXPAND, 0x5c, 0x24},        // "rewardExpand"
        {0xe0963c, Kind::IntArray, REWARD_EXPAND_ID, 0x5c, 0x24},     // "rewardExpandId"
        {0xe0964b, Kind::IntArray, REWARD_VALUE, 0x5c, 0x24},         // "rewardValue"
    };
    parse_a::parseRow(out, json, {0x98, 0x28c, PARSE_VTABLE, 0x78e335}, FIELDS);
}
RW_FUNCTION(0x78de9c, "master::SphereMasu::parse", SphereMasu::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
