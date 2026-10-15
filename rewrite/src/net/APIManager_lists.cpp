// APIManager requests whose parameters include arrays (id sets, timezones, strings) or values read from the
// game state (the running quest's burst gauge, a purchase transaction).
#include "APIManager.hpp"

using namespace apim;

namespace {

constexpr uint32_t SAFEBUFFER_READ = 0x45A269;
constexpr uint32_t STRINGBUFFER_CTOR = 0x49140D;          // StringBuffer::StringBuffer(const char* fmt, ...)
constexpr uint32_t STRINGBUFFER_FSET = 0x491501;          // StringBuffer::fset(const char* fmt, ...)
constexpr uint32_t RESOURCE_DATA_MANAGER_GET_INSTANCE = 0x7728AD;
constexpr uint32_t RESOURCE_DATA_MANAGER_RESET = 0x77376D;
constexpr uint32_t APPSTATE_STAGE = 0x20;                 // AppState: shared_ptr<StageState> (burst gauge at 0xC)

constexpr uint32_t S_ATTACHMENT_MEDAL_IDS = 0xDFBEC9;     // "attachmentMedalIds"
constexpr uint32_t S_AGREE_TYPE = 0xDFB12E;               // "agreeType"
constexpr uint32_t S_BASE_USER_MEDAL_ID = 0xDFBE92;       // "baseUserMedalId"
constexpr uint32_t S_BASE_USER_SKILL_ID = 0xDFBEDC;       // "baseUserSkillId"
constexpr uint32_t S_BRIDGE_TRANS_ID = 0xDFAF88;          // "bridgeTransId"
constexpr uint32_t S_BURST = 0xDFB632;                    // "burst"
constexpr uint32_t S_COMMENT = 0xDFB036;                  // "comment"
constexpr uint32_t S_COMPONENT_USER_MEDAL_IDS = 0xDFBEA2; // "componentUserMedalIds"
constexpr uint32_t S_COMPONENT_USER_SKILL_IDS = 0xDFBEEC; // "componentUserSkillIds"
constexpr uint32_t S_DATA_SIGNATURE = 0xDFBFED;           // "dataSignature"
constexpr uint32_t S_EARN_LUX_RANK = 0xDFB3EC;            // "earnLuxRank"
constexpr uint32_t S_IS_FINISHED = 0xDFADB9;              // "isFinished"
constexpr uint32_t S_IS_LOCKS = 0xDFBE6E;                 // "isLocks"
constexpr uint32_t S_IS_OVERWRITE_SKILL = 0xDFBEB8;       // "isOverwriteSkill"
constexpr uint32_t S_LEVEL = 0xDFB43D;                    // "level"
constexpr uint32_t S_MESSAGE = 0xDFAFCD;                  // "message"
constexpr uint32_t S_MONTH = 0xDFBFBD;                    // "month"
constexpr uint32_t S_NAME = 0xDFADE6;                     // "name"
constexpr uint32_t S_PHASE = 0xDFADCE;                    // "phase"
constexpr uint32_t S_PLATFORM_IDS = 0xDFC03D;             // "platformIds"
constexpr uint32_t S_PLATFORM_TYPE = 0xDFA83B;            // "platformType"
constexpr uint32_t S_PLAY_FREQUENTLY = 0xDFB82D;          // "playFrequently"
constexpr uint32_t S_PLAY_STYLE = 0xDFB138;               // "playStyle"
constexpr uint32_t S_PLAY_TIMEZONES = 0xDFB83C;           // "playTimezones"
constexpr uint32_t S_POPUP_FLAG = 0xDFADC4;               // "popupFlag"
constexpr uint32_t S_PRICE_LOCALE = 0xDFBFD4;             // "priceLocale"
constexpr uint32_t S_PRODUCT_ID = 0xDFBFCA;               // "productId"
constexpr uint32_t S_PURCHASE_DATA = 0xDFBFE0;            // "purchaseData"
constexpr uint32_t S_RECEIVE_PRESENT_IDS = 0xDFBF9C;      // "receivePresentIds"
constexpr uint32_t S_RESOURCE_IDS = 0xDFBF0B;             // "resourceIds"
constexpr uint32_t S_USER_MEDAL_IDS = 0xDFAE1B;           // "userMedalIds"
constexpr uint32_t S_USER_SKILL_IDS = 0xDFBE85;           // "userSkillIds"
constexpr uint32_t S_YEAR = 0xDFBFB3;                     // "year"
constexpr uint32_t S_FORMAT_04D = 0xDFBFAE;               // "%04d"
constexpr uint32_t S_FORMAT_02D = 0xDFBFB8;               // "%02d"


bool begin(const Frame &F, APIManager *self, uint32_t cb, uint32_t docSlot, uint32_t &doc) {
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    doc = newParams(F, docSlot);
    return true;
}

// "name": [the ids of an unordered_set<unsigned long long>]
void addIdSet(const Frame &F, uint32_t obj, uint32_t name, uint32_t nameLength, uint32_t set, uint32_t alloc,
              uint32_t arraySlot, uint32_t nameSlot) {
    setEmptyArray(F, arraySlot);
    for (uint32_t node = load32(set + 8); node; node = load32(node))
        F.call(PUSH_BACK_U64, {F.at(arraySlot), 0, load32(node + 8), load32(node + 12), alloc});
    setNameValue(F, nameSlot, name, nameLength);
    addMemberValue(F, obj, nameSlot, arraySlot, alloc);
}

// "playTimezones": [the ints of a std::set<Timezone>] (values built at valueSlot, which is then the name slot)
void addTimezones(const Frame &F, uint32_t obj, uint32_t set, uint32_t alloc, uint32_t arraySlot, uint32_t valueSlot) {
    setEmptyArray(F, arraySlot);
    const uint32_t end = set + 4;                              // the end node (the set's own __end_node)
    for (uint32_t node = load32(set); node != end; node = treeNext(node)) {
        setIntValue(F, valueSlot, int32_t(load32(node + 0x10)));
        F.call(PUSH_BACK_VALUE, {F.at(arraySlot), F.at(valueSlot), alloc});
    }
    setNameValue(F, valueSlot, S_PLAY_TIMEZONES, 13);
    addMemberValue(F, obj, valueSlot, arraySlot, alloc);
}

}  // namespace

// ---------------------------------------------------------------------------------------------- id sets
namespace {
bool sellRequest(APIManager *self, int32_t action, uint32_t name, uint32_t nameLength, uint32_t ids, uint32_t cb) {
    Frame F(0x88);
    uint32_t doc;
    if (!begin(F, self, cb, 0x28, doc)) return false;
    addIdSet(F, doc, name, nameLength, ids, allocatorOf(doc), 0x30, 0x40);
    return sendWithParams(F, self, action, cb, 1, {0x28, 0x50, 0x20});
}
}  // namespace

bool APIManager::executeUserMedalSell(APIManager *self, uint32_t userMedalIds, uint32_t cb) {
    return sellRequest(self, 49, S_USER_MEDAL_IDS, 12, userMedalIds, cb);
}
RW_FUNCTION_NOVERIFY(0x4C9A28, "APIManager::executeUserMedalSell", APIManager::executeUserMedalSell);

bool APIManager::executeSkillSell(APIManager *self, uint32_t userSkillIds, uint32_t cb) {
    return sellRequest(self, 50, S_USER_SKILL_IDS, 12, userSkillIds, cb);
}
RW_FUNCTION_NOVERIFY(0x4C9BF8, "APIManager::executeSkillSell", APIManager::executeSkillSell);

bool APIManager::executeUserPresentReceive(APIManager *self, uint32_t presentIds, uint32_t cb) {
    Frame F(0x90);
    uint32_t doc;
    if (!begin(F, self, cb, 0x30, doc)) return false;
    addIdSet(F, doc, S_RECEIVE_PRESENT_IDS, 17, presentIds, allocatorOf(doc), 0x38, 0x48);
    return sendWithParams(F, self, 47, cb, 0, {0x30, 0x58, 0x28});
}
RW_FUNCTION_NOVERIFY(0x4CD080, "APIManager::executeUserPresentReceive", APIManager::executeUserPresentReceive);

namespace {
// {"<base>": id, "<components>": [ids]} (+ "isOverwriteSkill" for the medal enhance)
bool mergeRequest(APIManager *self, int32_t action, uint32_t baseName, uint64_t baseId, uint32_t listName,
                  uint32_t listLength, uint32_t ids, const uint32_t *overwrite, uint32_t cb) {
    Frame F(0x90);
    uint32_t doc;
    if (!begin(F, self, cb, 0x30, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addU64(F, doc, baseName, baseId, alloc);
    addIdSet(F, doc, listName, listLength, ids, alloc, 0x38, 0x48);
    if (overwrite) addI32(F, doc, S_IS_OVERWRITE_SKILL, *overwrite, alloc);
    return sendWithParams(F, self, action, cb, 1, {0x30, 0x58, 0x28});
}
}  // namespace

bool APIManager::executeUserMedalEnhance(APIManager *self, uint64_t baseUserMedalId, uint32_t componentIds,
                                         uint32_t overwriteSkill, uint32_t cb) {
    return mergeRequest(self, 53, S_BASE_USER_MEDAL_ID, baseUserMedalId, S_COMPONENT_USER_MEDAL_IDS, 21,
                        componentIds, &overwriteSkill, cb);
}
RW_FUNCTION_NOVERIFY(0x4CA118, "APIManager::executeUserMedalEnhance", APIManager::executeUserMedalEnhance);

bool APIManager::executeUserMedalEvolve(APIManager *self, uint64_t baseUserMedalId, uint32_t attachmentIds, uint32_t cb) {
    return mergeRequest(self, 54, S_BASE_USER_MEDAL_ID, baseUserMedalId, S_ATTACHMENT_MEDAL_IDS, 18, attachmentIds,
                        nullptr, cb);
}
RW_FUNCTION_NOVERIFY(0x4CA330, "APIManager::executeUserMedalEvolve", APIManager::executeUserMedalEvolve);

bool APIManager::executeSkillEvolve(APIManager *self, uint64_t baseUserSkillId, uint32_t componentIds, uint32_t cb) {
    return mergeRequest(self, 55, S_BASE_USER_SKILL_ID, baseUserSkillId, S_COMPONENT_USER_SKILL_IDS, 21, componentIds,
                        nullptr, cb);
}
RW_FUNCTION_NOVERIFY(0x4CA528, "APIManager::executeSkillEvolve", APIManager::executeSkillEvolve);

// user/medal/lock: {"userMedalIds": [locked..., unlocked...], "isLocks": [1..., 0...]} (both sets by value)
bool APIManager::executeUserMedalLock(APIManager *self, uint32_t locked, uint32_t unlocked, uint32_t cb) {
    Frame F(0xA0);
    uint32_t doc;
    if (!begin(F, self, cb, 0x30, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    const uint32_t flags = 0x38, ids = 0x48, value = 0x58;
    setEmptyArray(F, flags);
    setEmptyArray(F, ids);
    auto add = [&](uint32_t set, uint32_t isLock) {
        for (uint32_t node = load32(set + 8); node; node = load32(node)) {
            F.call(PUSH_BACK_U64, {F.at(ids), 0, load32(node + 8), load32(node + 12), alloc});
            F.setWord(value + 4, 0);
            F.setWord(value + 0, isLock);
            F.setWord(value + 12, JSON_INT_POSITIVE);
            F.call(PUSH_BACK_VALUE, {F.at(flags), F.at(value), alloc});
        }
    };
    add(locked, 1);
    add(unlocked, 0);
    if (doc) {                                     // SetObject() again
        put32(doc + 0, 0);
        put32(doc + 4, 0);
        put32(doc + 8, 0);
        put32(doc + 12, JSON_OBJECT);
    }
    setNameValue(F, value, S_USER_MEDAL_IDS, 12);
    addMemberValue(F, doc, value, ids, alloc);
    setNameValue(F, value, S_IS_LOCKS, 7);
    addMemberValue(F, doc, value, flags, alloc);
    return sendWithParams(F, self, 44, cb, 1, {0x30, 0x68, 0x28});
}
RW_FUNCTION_NOVERIFY(0x4C91F4, "APIManager::executeUserMedalLock", APIManager::executeUserMedalLock);

// system/resource/ev: {"resourceIds": [the event resource ids]} (the vector by value); resets the resource
// manager first
bool APIManager::executeSystemResourceEv(APIManager *self, uint32_t resourceIds, uint32_t cb) {
    Frame F(0x90);
    F.call(RESOURCE_DATA_MANAGER_RESET, {F.call(RESOURCE_DATA_MANAGER_GET_INSTANCE, {})});
    uint32_t doc;
    if (!begin(F, self, cb, 0x30, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    const uint32_t array = 0x38, value = 0x48;
    setEmptyArray(F, array);
    for (uint32_t p = load32(resourceIds), end = load32(resourceIds + 4); p != end; p += 4) {
        uint32_t id = load32(p);
        F.setWord(value + 12, JSON_UINT_HIGH_BIT);   // GenericValue(unsigned)
        F.setWord(value + 4, 0);
        F.setWord(value + 0, id);
        if (int32_t(id) >= 0) F.setWord(value + 12, JSON_INT_POSITIVE);
        F.call(PUSH_BACK_VALUE, {F.at(array), F.at(value), alloc});
    }
    setNameValue(F, value, S_RESOURCE_IDS, 11);
    addMemberValue(F, doc, value, array, alloc);
    return sendWithParams(F, self, 27, cb, 1, {0x30, 0x58, 0x28});
}
RW_FUNCTION_NOVERIFY(0x4CAA78, "APIManager::executeSystemResourceEv", APIManager::executeSystemResourceEv);

bool APIManager::executeLinkMembersGet(APIManager *self, uint32_t platformIds, uint32_t platformType, uint32_t cb) {
    Frame F(0xA0);
    uint32_t doc;
    if (!begin(F, self, cb, 0x30, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    const uint32_t value = 0x38, array = 0x48, name = 0x58;
    setEmptyArray(F, array);
    for (uint32_t s = load32(platformIds), end = load32(platformIds + 4); s != end; s += 12) {   // vector<string>
        F.call(VALUE_FROM_STRING, {F.at(value), cstr(s), alloc});
        F.call(PUSH_BACK_VALUE, {F.at(array), F.at(value), alloc});
    }
    setNameValue(F, name, S_PLATFORM_IDS, 11);
    addMemberValue(F, doc, name, array, alloc);
    addU8(F, doc, S_PLATFORM_TYPE, platformType, alloc);
    return sendWithParams(F, self, 139, cb, 0, {0x30, 0x68, 0x28});
}
RW_FUNCTION_NOVERIFY(0x4D3FB0, "APIManager::executeLinkMembersGet", APIManager::executeLinkMembersGet);

// ---------------------------------------------------------------------------------------------- play style
bool APIManager::executeUserPlaystyleUpdate(APIManager *self, uint32_t timezones, int32_t frequency, uint32_t cb) {
    Frame F(0x88);
    uint32_t doc;
    if (!begin(F, self, cb, 0x28, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addTimezones(F, doc, timezones, alloc, 0x30, 0x40);
    addI32(F, doc, S_PLAY_FREQUENTLY, uint32_t(frequency), alloc);
    return sendWithParams(F, self, 38, cb, 1, {0x28, 0x50, 0x20});
}
RW_FUNCTION_NOVERIFY(0x4C8740, "APIManager::executeUserPlaystyleUpdate", APIManager::executeUserPlaystyleUpdate);

bool APIManager::executePartyInviteSearch(APIManager *self, int32_t level, uint32_t timezones, int32_t frequency,
                                          int32_t luxRank, uint32_t cb) {
    Frame F(0x90);
    uint32_t doc;
    if (!begin(F, self, cb, 0x30, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addI32(F, doc, S_LEVEL, uint32_t(level), alloc);
    addTimezones(F, doc, timezones, alloc, 0x38, 0x48);
    addI32(F, doc, S_PLAY_FREQUENTLY, uint32_t(frequency), alloc);
    addI32(F, doc, S_EARN_LUX_RANK, uint32_t(luxRank), alloc);
    return sendWithParams(F, self, 92, cb, 0, {0x30, 0x58, 0x28});
}
RW_FUNCTION_NOVERIFY(0x4D073C, "APIManager::executePartyInviteSearch", APIManager::executePartyInviteSearch);

// ---------------------------------------------------------------------------------------------- strings
bool APIManager::executeUserUpdate(APIManager *self, uint32_t name, uint32_t comment, uint32_t cb) {
    Frame F(0xA0);
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    if (!name || !load8(name)) {                   // no name: nothing to send
        F.call(0x4BAEB1, {addr(self), 0});         // APIManager::finishConnection(false)
        return false;
    }
    uint32_t doc = newParams(F, 0x30);
    uint32_t alloc = allocatorOf(doc);
    addString(F, doc, S_NAME, 4, name, alloc, 0x48, 0x58);
    addString(F, doc, S_COMMENT, 7, comment, alloc, 0x38, 0x58);
    return sendWithParams(F, self, 36, cb, 0, {0x30, 0x68, 0x28});
}
RW_FUNCTION_NOVERIFY(0x4C8360, "APIManager::executeUserUpdate", APIManager::executeUserUpdate);

namespace {
// party/create, party/update: {"name", "message", "playStyle", "agreeType"}
bool partyRequest(APIManager *self, int32_t action, uint32_t name, uint32_t message, int32_t playStyle,
                  int32_t agreeType, uint32_t cb) {
    Frame F(0xA8);
    uint32_t doc;
    if (!begin(F, self, cb, 0x38, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addString(F, doc, S_NAME, 4, name, alloc, 0x50, 0x60);
    addString(F, doc, S_MESSAGE, 7, message, alloc, 0x40, 0x60);
    addI32(F, doc, S_PLAY_STYLE, uint32_t(playStyle), alloc);
    addI32(F, doc, S_AGREE_TYPE, uint32_t(agreeType), alloc);
    return sendWithParams(F, self, action, cb, 0, {0x38, 0x70, 0x30});
}
}  // namespace

bool APIManager::executePartyCreate(APIManager *self, uint32_t name, uint32_t message, int32_t playStyle,
                                    int32_t agreeType, uint32_t cb) {
    return partyRequest(self, 74, name, message, playStyle, agreeType, cb);
}
RW_FUNCTION_NOVERIFY(0x4CE890, "APIManager::executePartyCreate", APIManager::executePartyCreate);

bool APIManager::executePartyUpdate(APIManager *self, uint32_t name, uint32_t message, int32_t playStyle,
                                    int32_t agreeType, uint32_t cb) {
    return partyRequest(self, 75, name, message, playStyle, agreeType, cb);
}
RW_FUNCTION_NOVERIFY(0x4CEAD0, "APIManager::executePartyUpdate", APIManager::executePartyUpdate);

bool APIManager::executePurchasePrepare(APIManager *self, uint32_t productId, uint32_t priceLocale, uint32_t cb) {
    Frame F(0xA0);
    uint32_t doc;
    if (!begin(F, self, cb, 0x30, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addString(F, doc, S_PRODUCT_ID, 9, productId, alloc, 0x48, 0x58);
    addString(F, doc, S_PRICE_LOCALE, 11, priceLocale, alloc, 0x38, 0x58);
    return sendWithParams(F, self, 72, cb, 0, {0x30, 0x68, 0x28});
}
RW_FUNCTION_NOVERIFY(0x4CFC48, "APIManager::executePurchasePrepare", APIManager::executePurchasePrepare);

// purchase/finish: a PurchaseManager::Transaction {u32 bridge id, ..., std::string purchase data at +0x2C,
// std::string signature at +0x38}
bool APIManager::executePurchaseFinish(APIManager *self, uint32_t transaction, uint32_t cb) {
    Frame F(0xA0);
    uint32_t doc;
    if (!begin(F, self, cb, 0x30, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addU32(F, doc, S_BRIDGE_TRANS_ID, load32(transaction), alloc);
    addString(F, doc, S_PURCHASE_DATA, 12, cstr(transaction + 0x2C), alloc, 0x48, 0x58);
    addString(F, doc, S_DATA_SIGNATURE, 13, cstr(transaction + 0x38), alloc, 0x38, 0x58);
    return sendWithParams(F, self, 73, cb, 1, {0x30, 0x68, 0x28});
}
RW_FUNCTION_NOVERIFY(0x4CFE40, "APIManager::executePurchaseFinish", APIManager::executePurchaseFinish);

// user/birthday/create: {"year": "YYYY", "month": "MM"} (strings)
bool APIManager::executeUserBirthdayCreate(APIManager *self, int32_t year, int32_t month, uint32_t cb) {
    Frame F(0x4A0);
    uint32_t doc;
    if (!begin(F, self, cb, 0x30, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    const uint32_t text = F.at(0x80);              // a StringBuffer
    F.call(STRINGBUFFER_CTOR, {text, S_FORMAT_04D, uint32_t(year)});
    addString(F, doc, S_YEAR, 4, text, alloc, 0x48, 0x58);
    F.call(STRINGBUFFER_FSET, {text, S_FORMAT_02D, uint32_t(month)});
    addString(F, doc, S_MONTH, 5, text, alloc, 0x38, 0x58);
    return sendWithParams(F, self, 40, cb, 1, {0x30, 0x68, 0x28});
}
RW_FUNCTION_NOVERIFY(0x4CD398, "APIManager::executeUserBirthdayCreate", APIManager::executeUserBirthdayCreate);

// ---------------------------------------------------------------------------------------------- tutorial / retire
// tutorial/status/update: only the values that differ from the cached TutorialStatus (+0x64: u32 phase,
// u64 popup flags at +8, bool finished at +0x10)
bool APIManager::executeTutorialStatusUpdate(APIManager *self, uint32_t phase, uint64_t popupFlag, uint32_t finished,
                                             uint32_t cb) {
    Frame F(0x68);
    uint32_t doc;
    if (!begin(F, self, cb, 0x28, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    const uint32_t status = self->tutorialStatus.ptr;
    if (load32(status) != phase) addU32(F, doc, S_PHASE, phase, alloc);
    uint32_t cached = self->tutorialStatus.ptr;
    uint64_t cachedFlag = uint64_t(load32(cached + 8)) | (uint64_t(load32(cached + 12)) << 32);
    if (cachedFlag != popupFlag) addU64(F, doc, S_POPUP_FLAG, popupFlag, alloc);
    cached = self->tutorialStatus.ptr;
    uint32_t cachedFinished = load8(cached + 0x10) ? 1u : 0u;
    if ((cachedFinished ^ finished) != 0) addI32(F, doc, S_IS_FINISHED, finished, alloc);
    return sendWithParams(F, self, 65, cb, 1, {0x28, 0x30, 0x20});
}
RW_FUNCTION_NOVERIFY(0x4C5DCC, "APIManager::executeTutorialStatusUpdate", APIManager::executeTutorialStatusUpdate);

namespace {
// stage/retire, colosseum/retire: {"burst": the running quest's burst gauge}
bool retireRequest(APIManager *self, int32_t action, uint32_t cb) {
    Frame F(0x70);
    uint32_t doc;
    if (!begin(F, self, cb, 0x2C, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});
    uint32_t stage = load32(app + APPSTATE_STAGE);
    F.call(SAFEBUFFER_READ, {stage, F.at(0x34), load32(stage + 12) + 0xC, 4, 0xC});
    addU32(F, doc, S_BURST, F.word(0x34), alloc);
    return sendWithParams(F, self, action, cb, 1, {0x2C, 0x38, 0x24});
}
}  // namespace

bool APIManager::executeStageRetire(APIManager *self, uint32_t cb) { return retireRequest(self, 104, cb); }
RW_FUNCTION_NOVERIFY(0x4CB43C, "APIManager::executeStageRetire", APIManager::executeStageRetire);

bool APIManager::executeColosseumRetire(APIManager *self, uint32_t cb) { return retireRequest(self, 117, cb); }
RW_FUNCTION_NOVERIFY(0x4D17A4, "APIManager::executeColosseumRetire", APIManager::executeColosseumRetire);
