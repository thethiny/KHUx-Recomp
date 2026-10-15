#include "World.hpp"
#include "ParseUtilD.hpp"

namespace master {

using WorldFunctions = TableFunctions<World>;
RW_FUNCTION(0x777350, "master::World::getWorldIds", WorldFunctions::getIds);
RW_FUNCTION(0x77736c, "master::World::create", WorldFunctions::create);
RW_FUNCTION(0x777a54, "master::World::getHash", WorldFunctions::getHash);
RW_FUNCTION(0x777b40, "master::World::getID", WorldFunctions::getID);
RW_FUNCTION(0x777b8c, "master::World::~World (deleting)", WorldFunctions::destructDelete);

void World::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xe07c36, Kind::Int, WORLD_ID, 0x4, 0x58},                         // "worldId"
        {0xe07c3e, Kind::String, WORLD_NAME, 0x81, 0x5c},                   // "worldName"
        {0xe07c48, Kind::String, RAID_BACKGROUND, 0x81, 0x5c},              // "raidBackground"
        {0xe07c57, Kind::String, INSTANCE_LWF, 0x81, 0x5c},                 // "instanceLwf"
        {0xe07c63, Kind::String, INSTANCE, 0x81, 0x5c},                     // "instance"
        {0xe07c6c, Kind::Int, X_POS, 0x4, 0x54},                            // "xPos"
        {0xe07c71, Kind::Int, Y_POS, 0x4, 0x50},                            // "yPos"
        {0xdfcdd6, Kind::Int, RATE, 0x4, 0x4c},                             // "rate"
        {0xe07c76, Kind::Int, VALID_PARTS, 0x4, 0x48},                      // "validParts"
        {0xdfb338, Kind::StringArray, PARTS_ID, 0x183, 0x5c, 0x81},         // "partsId"
        {0xe07c81, Kind::IntArray, X_POSTION, 0xc, 0x5c},                   // "xPostion"
        {0xe07c8a, Kind::IntArray, Y_POSTION, 0xc, 0x5c},                   // "yPostion"
    };
    parse_d::parseRow(out, json, {0x1f8, 0x3b4, PARSE_VTABLE, 0x777a01}, FIELDS);
}
RW_FUNCTION(0x777500, "master::World::parse", World::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
