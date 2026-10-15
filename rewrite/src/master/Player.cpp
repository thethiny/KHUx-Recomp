#include "Player.hpp"
#include "ParseUtilB.hpp"

namespace master {

using PlayerFunctions = TableFunctions<Player>;
RW_FUNCTION(0x78718c, "master::Player::getLvs", PlayerFunctions::getIds);
RW_FUNCTION(0x7871a8, "master::Player::create", PlayerFunctions::create);
RW_FUNCTION(0x7875e4, "master::Player::getHash", PlayerFunctions::getHash);
RW_FUNCTION(0x7876d0, "master::Player::getID", PlayerFunctions::getID);
RW_FUNCTION(0x78771c, "master::Player::~Player (deleting)", PlayerFunctions::destructDelete);

void Player::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe08dbb, Kind::Int, LV, 0x4, 0x44},                           // "lv"
        {0xe090f6, Kind::Int, NEED_EXP, 0x4, 0x40},                     // "needExp"
        {0xe090fe, Kind::Int, LUX_MEDAL, 0x4, 0x3c},                    // "luxMedal"
        {0xdfb8cb, Kind::Int, AP, 0x4, 0x38},                           // "ap"
        {0xe08f74, Kind::Int, COST, 0x4, 0x34},                         // "cost"
        {0xdfb539, Kind::Int, HP, 0x4, 0x30},                           // "hp"
        {0xe07d5b, Kind::Int, REWARD_ID, 0x4, 0x2c},                    // "rewardId"
    };
    parse_b::parseRow(out, json, {0x60, 0x1c, PARSE_VTABLE, 0x787591}, FIELDS);
}
RW_FUNCTION(0x78733c, "master::Player::parse", Player::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
