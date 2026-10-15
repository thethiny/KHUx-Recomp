#include "SerialcodeReward.hpp"
#include "ParseUtilC.hpp"

namespace master {

using SerialcodeRewardFunctions = TableFunctions<SerialcodeReward>;
RW_FUNCTION(0x790050, "master::SerialcodeReward::getSerialcodeRewardIds", SerialcodeRewardFunctions::getIds);
RW_FUNCTION(0x79006c, "master::SerialcodeReward::create", SerialcodeRewardFunctions::create);
RW_FUNCTION(0x7907f0, "master::SerialcodeReward::getHash", SerialcodeRewardFunctions::getHash);
RW_FUNCTION(0x7908dc, "master::SerialcodeReward::getID", SerialcodeRewardFunctions::getID);
RW_FUNCTION(0x790928, "master::SerialcodeReward::~SerialcodeReward (deleting)", SerialcodeRewardFunctions::destructDelete);

void SerialcodeReward::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    constexpr uint32_t START_DATE_KEY = 0xe0879e;   // "startDate"
    constexpr uint32_t END_DATE_KEY = 0xe087a8;     // "endDate"
    constexpr uint32_t TYPE_KEY = 0xdfa938;         // "type"
    constexpr uint32_t ID_KEY = 0xdfa935;           // "id"
    constexpr uint32_t SKILL_TYPE_KEY = 0xe0869b;   // "skillType"
    constexpr uint32_t SKILL_ID_KEY = 0xdfb66a;     // "skillId"
    constexpr uint32_t SKILL_LV_KEY = 0xe093b1;     // "skillLv"
    constexpr uint32_t NUM_KEY = 0xe07d90;          // "num"
    constexpr uint32_t BUF = 0x3c;   // frame buffer shared by the name, the dates and the arrays (stale bytes kept)
    guest::Frame f(0x158);
    RowBuilder b(f);
    uint32_t row = 0;
    auto intMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isInt(v); };
    auto stringMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isString(v); };
    uint32_t id, name, itemId, validReward;
    if (intMember(0xe0992a, id) &&                  // "serialcodeRewardId"
        stringMember(0xdfade6, name) &&             // "name"
        isString(member(json, START_DATE_KEY)) &&
        isString(member(json, END_DATE_KEY)) &&
        intMember(0xe08d8b, itemId) &&              // "itemId"
        intMember(0xe08575, validReward) &&         // "validReward"
        isArray(member(json, TYPE_KEY)) && isArray(member(json, ID_KEY)) &&
        isArray(member(json, SKILL_TYPE_KEY)) && isArray(member(json, SKILL_ID_KEY)) &&
        isArray(member(json, SKILL_LV_KEY)) && isArray(member(json, NUM_KEY))) {
        b.create(SIZE, VTABLE);
        b.int32(SERIALCODE_REWARD_ID, intOf(id), 0x38);
        b.string(NAME, stringOf(name), 257, BUF);
        auto date = [&](uint32_t key, uint32_t pos) {
            uint32_t v = member(json, key);
            if (!isString(v) || b.parseDate(stringOf(v), BUF) == 0) return false;
            b.bytes(pos, BUF, 8);
            return true;
        };
        bool ok = date(START_DATE_KEY, START_DATE) && date(END_DATE_KEY, END_DATE);
        if (ok) {
            b.int32(ITEM_ID, intOf(itemId), 0x34);
            b.int32(VALID_REWARD, intOf(validReward), 0x30);
            ok = b.int32Array(TYPE, member(json, TYPE_KEY), 6, BUF) &&
                 b.int32Array(ID, member(json, ID_KEY), 6, BUF) &&
                 b.int32Array(SKILL_TYPE, member(json, SKILL_TYPE_KEY), 6, BUF) &&
                 b.int32Array(SKILL_ID, member(json, SKILL_ID_KEY), 6, BUF) &&
                 b.int32Array(SKILL_LV, member(json, SKILL_LV_KEY), 6, BUF) &&
                 b.int32Array(NUM, member(json, NUM_KEY), 6, BUF);
        }
        if (ok) {
            row = b.obj();
        } else {
            b.destroy();
        }
    }
    makeShared(out, SHARED_PTR_CTOR, row);
}
RW_FUNCTION(0x790200, "master::SerialcodeReward::parse", SerialcodeReward::parse);

}  // namespace master
