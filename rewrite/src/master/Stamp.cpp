#include "Stamp.hpp"
#include "ParseUtilA.hpp"

namespace master {

using StampFunctions = TableFunctions<Stamp>;
RW_FUNCTION(0x7919c8, "master::Stamp::getStampIds", StampFunctions::getIds);
RW_FUNCTION(0x7919e4, "master::Stamp::create", StampFunctions::create);
RW_FUNCTION(0x791e64, "master::Stamp::getHash", StampFunctions::getHash);
RW_FUNCTION(0x791f50, "master::Stamp::getID", StampFunctions::getID);
RW_FUNCTION(0x791f9c, "master::Stamp::~Stamp (deleting)", StampFunctions::destructDelete);

void Stamp::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe09a59, Kind::Int, STAMP_ID, 0x4, 0x4c},                   // "stampId"
        {0xdfb643, Kind::Int, CATEGORY, 0x4, 0x48},                   // "category"
        {0xe08f24, Kind::Int, IMAGE_ID, 0x4, 0x44},                   // "imageId"
        {0xe098f3, Kind::Int, REQUIRE, 0x4, 0x40},                    // "require"
        {0xdfae85, Kind::Int, SORT_ID, 0x4, 0x3c},                    // "sortId"
        {0xe09a61, Kind::Int, IMAGE_WIDTH, 0x4, 0x38},                // "imageWidth"
        {0xe09a6c, Kind::Int, IMAGE_HEIGHT, 0x4, 0x34},               // "imageHeight"
        {0xe09a78, Kind::Int, VALID_STAMP, 0x4, 0x30},                // "validStamp"
    };
    parse_a::parseRow(out, json, {0x68, 0x20, PARSE_VTABLE, 0x791e11}, FIELDS);
}
RW_FUNCTION(0x791b78, "master::Stamp::parse", Stamp::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
