#include "Misc.hpp"
#include "ParseUtilB.hpp"

namespace master {

using MiscFunctions = TableFunctions<Misc>;
RW_FUNCTION(0x786d40, "master::Misc::getMiscIds", MiscFunctions::getIds);
RW_FUNCTION(0x786d5c, "master::Misc::create", MiscFunctions::create);
RW_FUNCTION(0x787048, "master::Misc::getHash", MiscFunctions::getHash);
RW_FUNCTION(0x787134, "master::Misc::getID", MiscFunctions::getID);
RW_FUNCTION(0x787180, "master::Misc::~Misc (deleting)", MiscFunctions::destructDelete);

void Misc::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe090c6, Kind::Int, MISC_ID, 0x4, 0x14},                      // "miscId"
        {0xe07ed2, Kind::Int, VALUE, 0x4, 0x10},                        // "value"
    };
    parse_b::parseRow(out, json, {0x30, 0x8, PARSE_VTABLE, 0x786ff5}, FIELDS);
}
RW_FUNCTION(0x786ef0, "master::Misc::parse", Misc::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
