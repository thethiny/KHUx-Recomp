#include "ColosseumStage.hpp"
#include "ParseUtilC.hpp"

namespace master {

using ColosseumStageFunctions = TableFunctions<ColosseumStage>;
RW_FUNCTION(0x77cf78, "master::ColosseumStage::getColosseumStageIds", ColosseumStageFunctions::getIds);
RW_FUNCTION(0x77cf94, "master::ColosseumStage::create", ColosseumStageFunctions::create);
RW_FUNCTION(0x77d5a0, "master::ColosseumStage::getHash", ColosseumStageFunctions::getHash);
RW_FUNCTION(0x77d68c, "master::ColosseumStage::getID", ColosseumStageFunctions::getID);
RW_FUNCTION(0x77d6d8, "master::ColosseumStage::~ColosseumStage (deleting)", ColosseumStageFunctions::destructDelete);

void ColosseumStage::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    constexpr uint32_t OPEN_DATE_KEY = 0xe085e1;            // "openDate"
    constexpr uint32_t CLOSE_DATE_KEY = 0xe085ea;           // "closeDate"
    constexpr uint32_t RANKING_CLOSE_DATE_KEY = 0xe085f4;   // "rankingCloseDate"
    constexpr uint32_t STAGE_ID_KEY = 0xdfaef5;             // "stageId"
    constexpr uint32_t BUF = 0x50;   // frame buffer shared by the dates and the stageId array (stale bytes kept)
    guest::Frame f(0x90);
    RowBuilder b(f);
    uint32_t row = 0;
    auto intMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isInt(v); };
    uint32_t id, head, term, rankingId, validStage, loopStageCount, lvPerLoop;
    if (intMember(0xdfaf54, id) &&                          // "colosseumStageId"
        isString(member(json, OPEN_DATE_KEY)) &&
        isString(member(json, CLOSE_DATE_KEY)) &&
        isString(member(json, RANKING_CLOSE_DATE_KEY)) &&
        intMember(0xe08605, head) &&                        // "headColosseumId"
        intMember(0xe08615, term) &&                        // "termColosseumId"
        intMember(0xe08625, rankingId) &&                   // "rankingId"
        intMember(0xe0862f, validStage) &&                  // "validStage"
        isArray(member(json, STAGE_ID_KEY)) &&
        intMember(0xe0863a, loopStageCount) &&              // "loopStageCount"
        intMember(0xe08649, lvPerLoop)) {                   // "lvPerLoop"
        b.create(SIZE, VTABLE);
        b.int32(COLOSSEUM_STAGE_ID, intOf(id), 0x4c);
        auto date = [&](uint32_t key, uint32_t pos) {
            uint32_t v = member(json, key);
            if (!isString(v) || b.parseDate(stringOf(v), BUF) == 0) return false;
            b.bytes(pos, BUF, 8);
            return true;
        };
        bool ok = date(OPEN_DATE_KEY, OPEN_DATE) && date(CLOSE_DATE_KEY, CLOSE_DATE) &&
                  date(RANKING_CLOSE_DATE_KEY, RANKING_CLOSE_DATE);
        if (ok) {
            b.int32(HEAD_COLOSSEUM_ID, intOf(head), 0x48);
            b.int32(TERM_COLOSSEUM_ID, intOf(term), 0x44);
            b.int32(RANKING_ID, intOf(rankingId), 0x40);
            b.int32(VALID_STAGE, intOf(validStage), 0x3c);
            uint32_t stageIds = member(json, STAGE_ID_KEY);
            ok = isArray(stageIds) && b.intArray(stageIds, BUF);
        }
        if (ok) {
            b.bytes(STAGE_ID, BUF, 40);
            b.int32(LOOP_STAGE_COUNT, intOf(loopStageCount), 0x38);
            b.int32(LV_PER_LOOP, intOf(lvPerLoop), 0x34);
            row = b.obj();
        } else {
            b.destroy();
        }
    }
    makeShared(out, SHARED_PTR_CTOR, row);
}
RW_FUNCTION(0x77D128, "master::ColosseumStage::parse", ColosseumStage::parse);

}  // namespace master
