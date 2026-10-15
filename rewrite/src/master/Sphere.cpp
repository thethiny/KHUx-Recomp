#include "Sphere.hpp"
#include "ParseUtilD.hpp"

namespace master {

using SphereFunctions = TableFunctions<Sphere>;
RW_FUNCTION(0x78cf04, "master::Sphere::getSphereBoardIds", SphereFunctions::getIds);
RW_FUNCTION(0x78cf20, "master::Sphere::create", SphereFunctions::create);
RW_FUNCTION(0x78d514, "master::Sphere::getHash", SphereFunctions::getHash);
RW_FUNCTION(0x78d600, "master::Sphere::getID", SphereFunctions::getID);
RW_FUNCTION(0x78d64c, "master::Sphere::~Sphere (deleting)", SphereFunctions::destructDelete);

void Sphere::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xdfb29f, Kind::Int, SPHERE_BOARD_ID, 0x4, 0x68},                  // "sphereBoardId"
        {0xe09559, Kind::String, BOARD_NAME, 0x81, 0x6c},                   // "boardName"
        {0xe09563, Kind::Int, KIND_ID, 0x4, 0x64},                          // "kindId"
        {0xe0956a, Kind::Int, BOARD_ID, 0x4, 0x60},                         // "boardId"
        {0xe09572, Kind::Int, ARRY_ID, 0x4, 0x5c},                          // "arryId"
        {0xe09579, Kind::Int, MASU_ID, 0x4, 0x58},                          // "masuId"
        {0xdfa938, Kind::Int, TYPE, 0x4, 0x54},                             // "type"
        {0xe09580, Kind::Int, OPEN_ID, 0x4, 0x50},                          // "openId"
        {0xe09587, Kind::Int, OPEN_PARAM, 0x4, 0x4c},                       // "openParam"
        {0xdfae77, Kind::Int, PRICE, 0x4, 0x48},                            // "price"
        {0xe09591, Kind::Date, START_DATETIME, 0x8, 0x6c},                  // "startDatetime"
        {0xdfaf72, Kind::Date, END_DATETIME, 0x8, 0x6c},                    // "endDatetime"
    };
    parse_d::parseRow(out, json, {0x108, 0xb8, PARSE_VTABLE, 0x78d4c1}, FIELDS);
}
RW_FUNCTION(0x78d0b4, "master::Sphere::parse", Sphere::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
