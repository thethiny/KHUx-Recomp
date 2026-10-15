#include "TutorialMisc.hpp"
#include "ParseUtilC.hpp"

namespace master {

using TutorialMiscFunctions = TableFunctions<TutorialMisc>;
RW_FUNCTION(0x777b98, "master::TutorialMisc::getTutorialIds", TutorialMiscFunctions::getIds);
RW_FUNCTION(0x777bb4, "master::TutorialMisc::create", TutorialMiscFunctions::create);
RW_FUNCTION(0x778034, "master::TutorialMisc::getHash", TutorialMiscFunctions::getHash);
RW_FUNCTION(0x778120, "master::TutorialMisc::getID", TutorialMiscFunctions::getID);
RW_FUNCTION(0x77816c, "master::TutorialMisc::~TutorialMisc (deleting)", TutorialMiscFunctions::destructDelete);

void TutorialMisc::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    guest::Frame f(0x68);
    RowBuilder b(f);
    auto intMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isInt(v); };
    uint32_t id, startClearQuest, startMiscId, num1, num2, setClearQuest, setMiscId, valid;
    if (intMember(0xe07cbc, id) &&                  // "tutorialId"
        intMember(0xe07cc7, startClearQuest) &&     // "startClearQuest"
        intMember(0xe07cd7, startMiscId) &&         // "startMiscId"
        intMember(0xe07ce3, num1) &&                // "num1"
        intMember(0xe07ce8, num2) &&                // "num2"
        intMember(0xe07ced, setClearQuest) &&       // "setClearQuest"
        intMember(0xe07cfb, setMiscId) &&           // "setMiscId"
        intMember(0xe07d05, valid)) {               // "valid"
        b.create(SIZE, VTABLE);
        b.int32(TUTORIAL_ID, intOf(id), 0x4c);
        b.int32(START_CLEAR_QUEST, intOf(startClearQuest), 0x48);
        b.int32(START_MISC_ID, intOf(startMiscId), 0x44);
        b.int32(NUM1, intOf(num1), 0x40);
        b.int32(NUM2, intOf(num2), 0x3c);
        b.int32(SET_CLEAR_QUEST, intOf(setClearQuest), 0x38);
        b.int32(SET_MISC_ID, intOf(setMiscId), 0x34);
        b.int32(VALID, intOf(valid), 0x30);
    }
    makeShared(out, SHARED_PTR_CTOR, b.obj());
}
RW_FUNCTION(0x777D48, "master::TutorialMisc::parse", TutorialMisc::parse);

}  // namespace master
