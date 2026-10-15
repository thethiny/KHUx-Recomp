#include "Badstatus.hpp"
#include "ParseUtilD.hpp"

namespace master {

using BadstatusFunctions = TableFunctions<Badstatus>;
RW_FUNCTION(0x779508, "master::Badstatus::getBadstatusIds", BadstatusFunctions::getIds);
RW_FUNCTION(0x779524, "master::Badstatus::create", BadstatusFunctions::create);
RW_FUNCTION(0x779970, "master::Badstatus::getHash", BadstatusFunctions::getHash);
RW_FUNCTION(0x779a5c, "master::Badstatus::getID", BadstatusFunctions::getID);
RW_FUNCTION(0x779aa8, "master::Badstatus::~Badstatus (deleting)", BadstatusFunctions::destructDelete);

void Badstatus::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xe07e75, Kind::Int, BADSTATUS_ID, 0x4, 0x30},                     // "badstatusId"
        {0xdfade6, Kind::String, NAME, 0x41, 0x34},                         // "name"
        {0xe07e81, Kind::Int, ICON_ID, 0x4, 0x2c},                          // "iconId"
        {0xe07e88, Kind::Int, EFFECT, 0x4, 0x28},                           // "effect"
        {0xe07e8f, Kind::Int, TARGET, 0x4, 0x24},                           // "target"
        {0xe07e96, Kind::IntArray, POWER, 0x8, 0x34},                       // "power"
    };
    parse_d::parseRow(out, json, {0x90, 0x5c, PARSE_VTABLE, 0x77991d}, FIELDS);
}
RW_FUNCTION(0x7796b8, "master::Badstatus::parse", Badstatus::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
