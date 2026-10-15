#include "EvGroupPattern.hpp"
#include "ParseUtilC.hpp"

namespace master {

using EvGroupPatternFunctions = TableFunctions<EvGroupPattern>;
RW_FUNCTION(0x782720, "master::EvGroupPattern::getPatternIds", EvGroupPatternFunctions::getIds);
RW_FUNCTION(0x78273c, "master::EvGroupPattern::create", EvGroupPatternFunctions::create);
RW_FUNCTION(0x782bcc, "master::EvGroupPattern::getHash", EvGroupPatternFunctions::getHash);
RW_FUNCTION(0x782cb8, "master::EvGroupPattern::getID", EvGroupPatternFunctions::getID);
RW_FUNCTION(0x782d04, "master::EvGroupPattern::~EvGroupPattern (deleting)", EvGroupPatternFunctions::destructDelete);

void EvGroupPattern::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    constexpr uint32_t START_TIME_KEY = 0xe08bb8;   // "startTime"
    constexpr uint32_t END_TIME_KEY = 0xdfb479;     // "endTime"
    constexpr uint32_t SHIFT_TIME_KEY = 0xe08c6f;   // "shiftTime"
    guest::Frame f(0x48);
    RowBuilder b(f);
    uint32_t row = 0;
    auto intMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isInt(v); };
    uint32_t id, groupDivide, rotate;
    if (intMember(0xe08c59, id) &&                  // "patternId"
        intMember(0xe08c63, groupDivide) &&         // "groupDivide"
        isString(member(json, START_TIME_KEY)) &&
        isString(member(json, END_TIME_KEY)) &&
        isString(member(json, SHIFT_TIME_KEY)) &&
        intMember(0xe08c79, rotate)) {              // "rotate"
        b.create(SIZE, VTABLE);
        b.int32(PATTERN_ID, intOf(id), 0x28);
        b.int32(GROUP_DIVIDE, intOf(groupDivide), 0x24);
        // each time: parsed into the Time at frame offset 0x2c (a bad one deletes the row)
        auto time = [&](uint32_t key, uint32_t pos) {
            uint32_t v = member(json, key);
            if (!isString(v) || b.parseTime(stringOf(v), 0x2c) == 0) return false;
            b.bytes(pos, 0x2c, 4);
            return true;
        };
        if (time(START_TIME_KEY, START_TIME) && time(END_TIME_KEY, END_TIME) && time(SHIFT_TIME_KEY, SHIFT_TIME)) {
            b.int32(ROTATE, intOf(rotate), 0x20);
            row = b.obj();
        } else {
            b.destroy();
        }
    }
    makeShared(out, SHARED_PTR_CTOR, row);
}
RW_FUNCTION(0x7828D0, "master::EvGroupPattern::parse", EvGroupPattern::parse);

}  // namespace master
