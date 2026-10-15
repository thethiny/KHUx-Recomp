#include "Medal.hpp"
#include "ParseUtilA.hpp"

namespace master {

using MedalFunctions = TableFunctions<Medal>;
RW_FUNCTION(0x785854, "master::Medal::getMedalIds", MedalFunctions::getIds);
RW_FUNCTION(0x785870, "master::Medal::create", MedalFunctions::create);
RW_FUNCTION(0x786768, "master::Medal::getHash", MedalFunctions::getHash);
RW_FUNCTION(0x786854, "master::Medal::getID", MedalFunctions::getID);
RW_FUNCTION(0x7868a0, "master::Medal::~Medal (deleting)", MedalFunctions::destructDelete);

void Medal::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xdfb5bf, Kind::Int, MEDAL_ID, 0x4, 0x150},                  // "medalId"
        {0xe08f24, Kind::Int, IMAGE_ID, 0x4, 0x14c},                  // "imageId"
        {0xe08f2c, Kind::Int, THUMB_ID, 0x4, 0x148},                  // "thumbId"
        {0xdfae85, Kind::Int, SORT_ID, 0x4, 0x144},                   // "sortId"
        {0xdfade6, Kind::String, NAME, 0x41, 0x154},                  // "name"
        {0xe08880, Kind::String, FLAVOR, 0x201, 0x154},               // "flavor"
        {0xe08f34, Kind::String, ADVANTAGE, 0x201, 0x154},            // "advantage"
        {0xdfa938, Kind::Int, TYPE, 0x4, 0x140},                      // "type"
        {0xdfb52f, Kind::Int, ATTRIBUTE, 0x4, 0x13c},                 // "attribute"
        {0xe08df8, Kind::Int, DARKLIGHT, 0x4, 0x138},                 // "darklight"
        {0xe08f3e, Kind::Int, VALID_SOURCE, 0x4, 0x134},              // "validSource"
        {0xe08f4a, Kind::IntArray, SOURCE, 0xc, 0x154},               // "source"
        {0xdfb52a, Kind::Int, RARE, 0x4, 0x130},                      // "rare"
        {0xe08f51, Kind::Int, SKILL_SLOT, 0x4, 0x12c},                // "skillSlot"
        {0xe08f5b, Kind::Int, GUILT_VALUE, 0x4, 0x128},               // "guiltValue"
        {0xe08f66, Kind::Int, MAX_LV, 0x4, 0x124},                    // "maxLv"
        {0xe08f6c, Kind::Int, EXP_TYPE, 0x4, 0x120},                  // "expType"
        {0xe08f74, Kind::Int, COST, 0x4, 0x11c},                      // "cost"
        {0xe08f79, Kind::Int, MIN_COST, 0x4, 0x118},                  // "minCost"
        {0xe08f81, Kind::Int, GROWTH_TYPE, 0x4, 0x114},               // "growthType"
        {0xdfb8dd, Kind::Int, ATTACK, 0x4, 0x110},                    // "attack"
        {0xe08f8c, Kind::Int, MAX_ATTACK, 0x4, 0x10c},                // "maxAttack"
        {0xe08f96, Kind::Int, ADD_MAX_ATTACK, 0x4, 0x108},            // "addMaxAttack"
        {0xdfb8d5, Kind::Int, DEFENSE, 0x4, 0x104},                   // "defense"
        {0xe08fa3, Kind::Int, MAX_DEFENSE, 0x4, 0x100},               // "maxDefense"
        {0xe08fae, Kind::Int, ADD_MAX_DEFENSE, 0x4, 0xfc},            // "addMaxDefense"
        {0xe08fbc, Kind::Int, VALID_BURST, 0x4, 0xf8},                // "validBurst"
        {0xe07f40, Kind::Int, BURST_ID, 0x4, 0xf4},                   // "burstId"
        {0xe08fc7, Kind::IntArray, BURST_ENHANCE_CATEGORY, 0x8, 0x154}, // "burstEnhanceCategory"
        {0xe08fdc, Kind::Int, GROUP_ID, 0x4, 0xf0},                   // "groupId"
        {0xe08fe4, Kind::Int, SELL_SPHERE_POINT, 0x4, 0xec},          // "sellSpherePoint"
        {0xe08ff4, Kind::Int, MATERIAL_EXP, 0x4, 0xe8},               // "materialExp"
        {0xe08ef6, Kind::Int, SELL, 0x4, 0xe4},                       // "sell"
        {0xe09000, Kind::Int, SELL_PER_LV, 0x4, 0xe0},                // "sellPerLv"
        {0xe0900a, Kind::Int, ENHANCE_ATTACK, 0x4, 0xdc},             // "enhanceAttack"
        {0xe09018, Kind::Int, ENHANCE_DEFENSE, 0x4, 0xd8},            // "enhanceDefense"
        {0xe09027, Kind::Int, ENHANCE_COST, 0x4, 0xd4},               // "enhanceCost"
        {0xe09033, Kind::Int, VALID_EVOLVE, 0x4, 0xd0},               // "validEvolve"
        {0xe08e1e, Kind::Int, EVOLVE_ID, 0x4, 0xcc},                  // "evolveId"
        {0xe0903f, Kind::Int, EVOLVE_MONEY, 0x4, 0xc8},               // "evolveMoney"
        {0xe0904b, Kind::Int, VALID_EVOLVE_NEED, 0x4, 0xc4},          // "validEvolveNeed"
        {0xe0905b, Kind::IntArray, EVOLVE_NEED_ID, 0x14, 0x154},      // "evolveNeedId"
    };
    parse_a::parseRow(out, json, {0x370, 0x4fc, PARSE_VTABLE, 0x786715}, FIELDS);
}
RW_FUNCTION(0x785a04, "master::Medal::parse", Medal::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
