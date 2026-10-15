#include "Title.hpp"
#include "ParseUtilB.hpp"

namespace master {

using TitleFunctions = TableFunctions<Title>;
RW_FUNCTION(0x78fa70, "master::Title::getTitleIds", TitleFunctions::getIds);
RW_FUNCTION(0x78fa8c, "master::Title::create", TitleFunctions::create);
RW_FUNCTION(0x78ff0c, "master::Title::getHash", TitleFunctions::getHash);
RW_FUNCTION(0x78fff8, "master::Title::getID", TitleFunctions::getID);
RW_FUNCTION(0x790044, "master::Title::~Title (deleting)", TitleFunctions::destructDelete);

void Title::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xdfb5c7, Kind::Int, TITLE_ID, 0x4, 0x48},                     // "titleId"
        {0xdfb643, Kind::Int, CATEGORY, 0x4, 0x44},                     // "category"
        {0xe07ba4, Kind::String, TITLE, 0x41, 0x4c},                    // "title"
        {0xdfae44, Kind::String, DESCRIPTION, 0x101, 0x4c},             // "description"
        {0xe098f3, Kind::Int, REQUIRE, 0x4, 0x40},                      // "require"
        {0xe07ed2, Kind::Int, VALUE, 0x4, 0x3c},                        // "value"
        {0xe098fb, Kind::Int, LABEL, 0x4, 0x38},                        // "label"
    };
    parse_b::parseRow(out, json, {0x168, 0x158, PARSE_VTABLE, 0x78feb9}, FIELDS);
}
RW_FUNCTION(0x78fc20, "master::Title::parse", Title::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
