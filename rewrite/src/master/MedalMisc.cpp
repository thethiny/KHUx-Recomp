#include "MedalMisc.hpp"
#include "ParseUtilA.hpp"

namespace master {

using MedalMiscFunctions = TableFunctions<MedalMisc>;
RW_FUNCTION(0x7868ac, "master::MedalMisc::getMedalMiscIds", MedalMiscFunctions::getIds);
RW_FUNCTION(0x7868c8, "master::MedalMisc::create", MedalMiscFunctions::create);
RW_FUNCTION(0x786bfc, "master::MedalMisc::getHash", MedalMiscFunctions::getHash);
RW_FUNCTION(0x786ce8, "master::MedalMisc::getID", MedalMiscFunctions::getID);
RW_FUNCTION(0x786d34, "master::MedalMisc::~MedalMisc (deleting)", MedalMiscFunctions::destructDelete);

void MedalMisc::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe09091, Kind::Int, MEDAL_MISC_ID, 0x4, 0x18},              // "medalMiscId"
        {0xe07ed2, Kind::IntArray, VALUE, 0xc, 0x1c},                 // "value"
    };
    parse_a::parseRow(out, json, {0x40, 0x10, PARSE_VTABLE, 0x786ba9}, FIELDS);
}
RW_FUNCTION(0x786a5c, "master::MedalMisc::parse", MedalMisc::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
