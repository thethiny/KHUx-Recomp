#include "MypageBackground.hpp"
#include "ParseUtilD.hpp"

namespace master {

using MypageBackgroundFunctions = TableFunctions<MypageBackground>;
RW_FUNCTION(0x7924c4, "master::MypageBackground::getIds", MypageBackgroundFunctions::getIds);
RW_FUNCTION(0x7924e0, "master::MypageBackground::create", MypageBackgroundFunctions::create);
RW_FUNCTION(0x792c34, "master::MypageBackground::getHash", MypageBackgroundFunctions::getHash);
RW_FUNCTION(0x792d20, "master::MypageBackground::getID", MypageBackgroundFunctions::getID);
RW_FUNCTION(0x792d6c, "master::MypageBackground::~MypageBackground (deleting)", MypageBackgroundFunctions::destructDelete);

void MypageBackground::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xdfa935, Kind::Int, ID, 0x4, 0x4c},                               // "id"
        {0xdfa882, Kind::Int, BACKGROUND_ID, 0x4, 0x48},                    // "backgroundId"
        {0xdfae85, Kind::Int, SORT_ID, 0x4, 0x44},                          // "sortId"
        {0xe09ae7, Kind::Int, VALID_CHILD, 0x4, 0x40},                      // "validChild"
        {0xe08bb8, Kind::Date, START_TIME, 0x8, 0x50},                      // "startTime"
        {0xdfb479, Kind::Date, END_TIME, 0x8, 0x50},                        // "endTime"
        {0xe09af2, Kind::Int, VIEW_TYPE, 0x4, 0x3c},                        // "viewType"
        {0xe07c76, Kind::Int, VALID_PARTS, 0x4, 0x38},                      // "validParts"
        {0xdfb338, Kind::IntArray, PARTS_ID, 0x18, 0x50},                   // "partsId"
        {0xe09afb, Kind::IntArray, DATA_TYPE, 0x18, 0x50},                  // "dataType"
        {0xe07c81, Kind::IntArray, X_POSTION, 0x18, 0x50},                  // "xPostion"
        {0xe07c8a, Kind::IntArray, Y_POSTION, 0x18, 0x50},                  // "yPostion"
        {0xe09b04, Kind::IntArray, Z_SORT, 0x18, 0x50},                     // "zSort"
    };
    parse_d::parseRow(out, json, {0x80, 0xa0, PARSE_VTABLE, 0x792be1}, FIELDS);
}
RW_FUNCTION(0x792674, "master::MypageBackground::parse", MypageBackground::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
