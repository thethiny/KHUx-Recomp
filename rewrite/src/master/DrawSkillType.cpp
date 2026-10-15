#include "DrawSkillType.hpp"
#include "ParseUtilD.hpp"

namespace master {

using DrawSkillTypeFunctions = TableFunctions<DrawSkillType>;
RW_FUNCTION(0x77f1e0, "master::DrawSkillType::getDrawSkillTypeIds", DrawSkillTypeFunctions::getIds);
RW_FUNCTION(0x77f1fc, "master::DrawSkillType::create", DrawSkillTypeFunctions::create);
RW_FUNCTION(0x77f67c, "master::DrawSkillType::getHash", DrawSkillTypeFunctions::getHash);
RW_FUNCTION(0x77f768, "master::DrawSkillType::getID", DrawSkillTypeFunctions::getID);
RW_FUNCTION(0x77f7b4, "master::DrawSkillType::~DrawSkillType (deleting)", DrawSkillTypeFunctions::destructDelete);

void DrawSkillType::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xe08812, Kind::Int, DRAW_SKILL_TYPE_ID, 0x4, 0x4c},               // "drawSkillTypeId"
        {0xe0868c, Kind::Int, TABLE_ID, 0x4, 0x48},                         // "tableId"
        {0xe08822, Kind::Int, LOT_RARE3, 0x4, 0x44},                        // "lotRare3"
        {0xe0882b, Kind::Int, LOT_RARE2, 0x4, 0x40},                        // "lotRare2"
        {0xe08834, Kind::Int, LOT_RARE1, 0x4, 0x3c},                        // "lotRare1"
        {0xe0883d, Kind::Int, LOT_NON, 0x4, 0x38},                          // "lotNon"
        {0xe08844, Kind::Int, LOT_RESERVE, 0x4, 0x34},                      // "lotReserve"
        {0xe086a5, Kind::Int, IS_DELETED, 0x4, 0x30},                       // "isDeleted"
    };
    parse_d::parseRow(out, json, {0x68, 0x20, PARSE_VTABLE, 0x77f629}, FIELDS);
}
RW_FUNCTION(0x77f390, "master::DrawSkillType::parse", DrawSkillType::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
