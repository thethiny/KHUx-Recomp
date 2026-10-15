#include "SkillExp.hpp"
#include "ParseUtilB.hpp"

namespace master {

using SkillExpFunctions = TableFunctions<SkillExp>;
RW_FUNCTION(0x78ca70, "master::SkillExp::getExpIds", SkillExpFunctions::getIds);
RW_FUNCTION(0x78ca8c, "master::SkillExp::create", SkillExpFunctions::create);
RW_FUNCTION(0x78cdc0, "master::SkillExp::getHash", SkillExpFunctions::getHash);
RW_FUNCTION(0x78ceac, "master::SkillExp::getID", SkillExpFunctions::getID);
RW_FUNCTION(0x78cef8, "master::SkillExp::~SkillExp (deleting)", SkillExpFunctions::destructDelete);

void SkillExp::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_b::Kind;
    static constexpr parse_b::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe0952a, Kind::Int, EXP_ID, 0x4, 0x18},                       // "expId"
        {0xe07ed2, Kind::IntArray, VALUE, 0xc, 0x1c},                   // "value"
    };
    parse_b::parseRow(out, json, {0x40, 0x10, PARSE_VTABLE, 0x78cd6d}, FIELDS);
}
RW_FUNCTION(0x78cc20, "master::SkillExp::parse", SkillExp::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
