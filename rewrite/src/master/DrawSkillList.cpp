#include "DrawSkillList.hpp"
#include "ParseUtilA.hpp"

namespace master {

using DrawSkillListFunctions = TableFunctions<DrawSkillList>;
RW_FUNCTION(0x77ecc0, "master::DrawSkillList::getDrawSkillListIds", DrawSkillListFunctions::getIds);
RW_FUNCTION(0x77ecdc, "master::DrawSkillList::create", DrawSkillListFunctions::create);
RW_FUNCTION(0x77f09c, "master::DrawSkillList::getHash", DrawSkillListFunctions::getHash);
RW_FUNCTION(0x77f188, "master::DrawSkillList::getID", DrawSkillListFunctions::getID);
RW_FUNCTION(0x77f1d4, "master::DrawSkillList::~DrawSkillList (deleting)", DrawSkillListFunctions::destructDelete);

void DrawSkillList::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe087d9, Kind::Int, DRAW_SKILL_LIST_ID, 0x4, 0x2c},         // "drawSkillListId"
        {0xe0868c, Kind::Int, TABLE_ID, 0x4, 0x28},                   // "tableId"
        {0xdfb66a, Kind::Int, SKILL_ID, 0x4, 0x24},                   // "skillId"
        {0xe08694, Kind::Int, WEIGHT, 0x4, 0x20},                     // "weight"
        {0xe086a5, Kind::Int, IS_DELETED, 0x4, 0x1c},                 // "isDeleted"
    };
    parse_a::parseRow(out, json, {0x48, 0x14, PARSE_VTABLE, 0x77f049}, FIELDS);
}
RW_FUNCTION(0x77ee74, "master::DrawSkillList::parse", DrawSkillList::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
