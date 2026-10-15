#include "SphereArray.hpp"
#include "ParseUtilB.hpp"

namespace master {

using SphereArrayFunctions = TableFunctions<SphereArray>;
RW_FUNCTION(0x78d658, "master::SphereArray::getArryIds", SphereArrayFunctions::getIds);
RW_FUNCTION(0x78d674, "master::SphereArray::create", SphereArrayFunctions::create);
RW_FUNCTION(0x78dba8, "master::SphereArray::getHash", SphereArrayFunctions::getHash);
RW_FUNCTION(0x78dc94, "master::SphereArray::getID", SphereArrayFunctions::getID);
RW_FUNCTION(0x78dce0, "master::SphereArray::~SphereArray (deleting)", SphereArrayFunctions::destructDelete);

void SphereArray::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe09572, Kind::Int, ARRY_ID, 0x4, 0x18},                      // "arryId"
        {0xe095c8, Kind::IntArray, SP_COL4, 0x24, 0x1c},                // "spCol4"
        {0xe095cf, Kind::IntArray, SP_COL3, 0x24, 0x1c},                // "spCol3"
        {0xe095d6, Kind::IntArray, SP_COL2, 0x24, 0x1c},                // "spCol2"
        {0xe095dd, Kind::IntArray, SP_COL1, 0x24, 0x1c},                // "spCol1"
        {0xe095e4, Kind::IntArray, SP_COL0, 0x24, 0x1c},                // "spCol0"
    };
    parse_b::parseRow(out, json, {0x58, 0xb8, PARSE_VTABLE, 0x78db55}, FIELDS);
}
RW_FUNCTION(0x78d808, "master::SphereArray::parse", SphereArray::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
