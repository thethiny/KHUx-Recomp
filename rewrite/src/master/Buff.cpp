#include "Buff.hpp"
#include "ParseUtilD.hpp"

namespace master {

using BuffFunctions = TableFunctions<Buff>;
RW_FUNCTION(0x779f00, "master::Buff::getBuffIds", BuffFunctions::getIds);
RW_FUNCTION(0x779f1c, "master::Buff::create", BuffFunctions::create);
RW_FUNCTION(0x77a330, "master::Buff::getHash", BuffFunctions::getHash);
RW_FUNCTION(0x77a41c, "master::Buff::getID", BuffFunctions::getID);
RW_FUNCTION(0x77a468, "master::Buff::~Buff (deleting)", BuffFunctions::destructDelete);

void Buff::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xe07f01, Kind::Int, BUFF_ID, 0x4, 0x38},                          // "buffId"
        {0xe07f08, Kind::Int, GRADE, 0x4, 0x34},                            // "grade"
        {0xdfade6, Kind::String, NAME, 0x41, 0x3c},                         // "name"
        {0xe07e81, Kind::Int, ICON_ID, 0x4, 0x30},                          // "iconId"
        {0xe07e88, Kind::Int, EFFECT, 0x4, 0x2c},                           // "effect"
        {0xe07f0e, Kind::Int, ADD_POWER, 0x4, 0x28},                        // "addPower"
    };
    parse_d::parseRow(out, json, {0x98, 0x58, PARSE_VTABLE, 0x77a2dd}, FIELDS);
}
RW_FUNCTION(0x77a0b0, "master::Buff::parse", Buff::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
