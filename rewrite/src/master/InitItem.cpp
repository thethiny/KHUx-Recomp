#include "InitItem.hpp"
#include "ParseUtilA.hpp"

namespace master {

using InitItemFunctions = TableFunctions<InitItem>;
RW_FUNCTION(0x7839e4, "master::InitItem::getIds", InitItemFunctions::getIds);
RW_FUNCTION(0x783a00, "master::InitItem::create", InitItemFunctions::create);
RW_FUNCTION(0x783e3c, "master::InitItem::getHash", InitItemFunctions::getHash);
RW_FUNCTION(0x783f28, "master::InitItem::getID", InitItemFunctions::getID);
RW_FUNCTION(0x783f74, "master::InitItem::~InitItem (deleting)", InitItemFunctions::destructDelete);

void InitItem::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xdfa935, Kind::Int, ID, 0x4, 0x44},                         // "id"
        {0xdfb643, Kind::Int, CATEGORY, 0x4, 0x40},                   // "category"
        {0xe08d73, Kind::Int, EQUIP_TYPE, 0x4, 0x3c},                 // "equipType"
        {0xe08d7d, Kind::Int, EQUIP_NO, 0x4, 0x38},                   // "equipNo"
        {0xe08d85, Kind::Int, PARAM, 0x4, 0x34},                      // "param"
        {0xe08d8b, Kind::Int, ITEM_ID, 0x4, 0x30},                    // "itemId"
        {0xdfb66a, Kind::Int, SKILL_ID, 0x4, 0x2c},                   // "skillId"
    };
    parse_a::parseRow(out, json, {0x60, 0x1c, PARSE_VTABLE, 0x783de9}, FIELDS);
}
RW_FUNCTION(0x783b94, "master::InitItem::parse", InitItem::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
