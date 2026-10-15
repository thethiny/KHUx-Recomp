#include "DrawMedalList.hpp"
#include "ParseUtilD.hpp"

namespace master {

using DrawMedalListFunctions = TableFunctions<DrawMedalList>;
RW_FUNCTION(0x77d6e4, "master::DrawMedalList::getDrawMedalListIds", DrawMedalListFunctions::getIds);
RW_FUNCTION(0x77d700, "master::DrawMedalList::create", DrawMedalListFunctions::create);
RW_FUNCTION(0x77db80, "master::DrawMedalList::getHash", DrawMedalListFunctions::getHash);
RW_FUNCTION(0x77dc6c, "master::DrawMedalList::getID", DrawMedalListFunctions::getID);
RW_FUNCTION(0x77dcb8, "master::DrawMedalList::~DrawMedalList (deleting)", DrawMedalListFunctions::destructDelete);

void DrawMedalList::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xe0867c, Kind::Int, DRAW_MEDAL_LIST_ID, 0x4, 0x4c},               // "drawMedalListId"
        {0xe0868c, Kind::Int, TABLE_ID, 0x4, 0x48},                         // "tableId"
        {0xdfb5bf, Kind::Int, MEDAL_ID, 0x4, 0x44},                         // "medalId"
        {0xe08694, Kind::Int, WEIGHT, 0x4, 0x40},                           // "weight"
        {0xe0869b, Kind::Int, SKILL_TYPE, 0x4, 0x3c},                       // "skillType"
        {0xdfb66a, Kind::Int, SKILL_ID, 0x4, 0x38},                         // "skillId"
        {0xdfb699, Kind::Int, BURST_UPPER_NUMBER, 0x4, 0x34},               // "burstUpperNumber"
        {0xe086a5, Kind::Int, IS_DELETED, 0x4, 0x30},                       // "isDeleted"
    };
    parse_d::parseRow(out, json, {0x68, 0x20, PARSE_VTABLE, 0x77db2d}, FIELDS);
}
RW_FUNCTION(0x77d894, "master::DrawMedalList::parse", DrawMedalList::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
