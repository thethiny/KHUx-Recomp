#include "GuiltProb.hpp"
#include "ParseUtilB.hpp"

namespace master {

using GuiltProbFunctions = TableFunctions<GuiltProb>;
RW_FUNCTION(0x791fa8, "master::GuiltProb::getIds", GuiltProbFunctions::getIds);
RW_FUNCTION(0x791fc4, "master::GuiltProb::create", GuiltProbFunctions::create);
RW_FUNCTION(0x792380, "master::GuiltProb::getHash", GuiltProbFunctions::getHash);
RW_FUNCTION(0x79246c, "master::GuiltProb::getID", GuiltProbFunctions::getID);
RW_FUNCTION(0x7924b8, "master::GuiltProb::~GuiltProb (deleting)", GuiltProbFunctions::destructDelete);

void GuiltProb::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xdfa935, Kind::Int, ID, 0x4, 0x2c},                           // "id"
        {0xe07ed2, Kind::Int, VALUE, 0x4, 0x28},                        // "value"
        {0xe09aac, Kind::Int, FACTOR, 0x4, 0x24},                       // "factor"
        {0xe08694, Kind::Int, WEIGHT, 0x4, 0x20},                       // "weight"
        {0xe09ab3, Kind::Int, IS_CAMPAIGN, 0x4, 0x1c},                  // "isCampaign"
    };
    parse_b::parseRow(out, json, {0x48, 0x14, PARSE_VTABLE, 0x79232d}, FIELDS);
}
RW_FUNCTION(0x792158, "master::GuiltProb::parse", GuiltProb::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
