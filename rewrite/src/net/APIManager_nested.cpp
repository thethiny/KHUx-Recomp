// APIManager requests with nested JSON parameters (arrays of objects): medal skills, keyblade decks, avatar
// coordinates, raid / colosseum results and the tutorial progress steps.
#include "APIManager.hpp"

using namespace apim;

namespace {

constexpr uint32_t SAFEBUFFER_READ = 0x45A269;
constexpr uint32_t STRINGBUFFER_CTOR = 0x49140D;
constexpr uint32_t APPSTATE_STAGE = 0x20;                 // AppState: shared_ptr<StageState>

constexpr uint32_t S_ACCESSORIES_PARTS_IDS = 0xDFB70C;    // "accessoriesPartsIds"
constexpr uint32_t S_BIRTHDAY = 0xDFB224;                 // "birthday"
constexpr uint32_t S_BIRTHDAY_FORMAT = 0xDFBDD5;          // "%04d-%02d-01 00:00:00"
constexpr uint32_t S_BODY_PARTS_ID = 0xDFB72C;            // "bodyPartsId"
constexpr uint32_t S_BURST = 0xDFB632;                    // "burst"
constexpr uint32_t S_DAMAGE = 0xDFB340;                   // "damage"
constexpr uint32_t S_DEFAULT_FLAG = 0xDFBE62;             // "defaultFlag"
constexpr uint32_t S_ENEMY_DEAD_NUMBER = 0xDFBF4C;        // "enemyDeadNumber"
constexpr uint32_t S_EXP = 0xDFB666;                      // "exp"
constexpr uint32_t S_FACE_PARTS_ID = 0xDFB738;            // "facePartsId"
constexpr uint32_t S_GENDER = 0xDFB761;                   // "gender"
constexpr uint32_t S_GET_POINT = 0xDFBF17;                // "getPoint"
constexpr uint32_t S_HAIR_COLOR_PARTS_ID = 0xDFB744;      // "hairColorPartsId"
constexpr uint32_t S_HAIR_PARTS_ID = 0xDFB755;            // "hairPartsId"
constexpr uint32_t S_HP = 0xDFB539;                       // "hp"
constexpr uint32_t S_INVITE_CODE = 0xDFADDB;              // "inviteCode"
constexpr uint32_t S_IS_FAVORITE = 0xDFB60E;              // "isFavorite"
constexpr uint32_t S_IS_SKIP = 0xDFC017;                  // "isSkip"
constexpr uint32_t S_LUX = 0xDFA9FF;                      // "lux"
constexpr uint32_t S_MAXIMUM_DAMAGE = 0xDFBF80;           // "maximumDamage"
constexpr uint32_t S_MIMIC_DEAD_NUMBER = 0xDFBF5C;        // "mimicDeadNumber"
constexpr uint32_t S_MONEY = 0xDFB90F;                    // "money"
constexpr uint32_t S_MY_COORDINATE_NO = 0xDFB768;         // "myCoordinateNo"
constexpr uint32_t S_NAME = 0xDFADE6;                     // "name"
constexpr uint32_t S_PARTS = 0xDFB4C9;                    // "parts"
constexpr uint32_t S_PARTS_ID = 0xDFB338;                 // "partsId"
constexpr uint32_t S_PROGRESSION = 0xDFADEB;              // "progression"
constexpr uint32_t S_RAID_ENEMY_ID = 0xDFB53C;            // "raidEnemyId"
constexpr uint32_t S_RANK = 0xDFAA03;                     // "rank"
constexpr uint32_t S_SKIN_PARTS_ID = 0xDFB720;            // "skinPartsId"
constexpr uint32_t S_STAGE_COUNT = 0xDFAEBE;              // "stageCount"
constexpr uint32_t S_STAGE_ID = 0xDFAEF5;                 // "stageId"
constexpr uint32_t S_UNION_ID = 0xDFAA08;                 // "unionId"
constexpr uint32_t S_UPDATE_AVATAR_DATA = 0xDFBDEB;       // "updateAvatarData"
constexpr uint32_t S_USER_KEYBLADE_ID = 0xDFAEE6;         // "userKeybladeId"
constexpr uint32_t S_USER_KEYBLADES = 0xDFA6A4;           // "userKeyblades"
constexpr uint32_t S_USER_MEDAL_ID = 0xDFB369;            // "userMedalId"
constexpr uint32_t S_USER_MEDAL_IDS = 0xDFAE1B;           // "userMedalIds"
constexpr uint32_t S_USER_MEDALS = 0xDFA676;              // "userMedals"
constexpr uint32_t S_USER_SKILL_IDS = 0xDFBE85;           // "userSkillIds"

bool begin(const Frame &F, APIManager *self, uint32_t cb, uint32_t docSlot, uint32_t &doc) {
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    doc = newParams(F, docSlot);
    return true;
}

void setObject(uint32_t doc) {
    if (doc) {
        put32(doc + 0, 0);
        put32(doc + 4, 0);
        put32(doc + 8, 0);
        put32(doc + 12, JSON_OBJECT);
    }
}

// An avatar coordinate (UserAvatar::Coordinate, 0x30 bytes: number, gender, hair, hair color, face, body, skin,
// 5 accessories) as the "updateAvatarData" object at objSlot: the non-zero accessories in an array (arraySlot,
// values at valueSlot), then the fields.
void addCoordinateFields(const Frame &F, uint32_t obj, uint32_t coord, uint32_t alloc, uint32_t arraySlot,
                         uint32_t valueSlot, bool withNumber, int32_t defaultNo) {
    if (withNumber) {
        addI32(F, obj, S_MY_COORDINATE_NO, load32(coord + 0), alloc);
        addI32(F, obj, S_GENDER, load32(coord + 4), alloc);
        addI32(F, obj, S_DEFAULT_FLAG, int32_t(load32(coord)) == defaultNo ? 1 : 0, alloc);
    } else {
        addI32(F, obj, S_GENDER, load32(coord + 4), alloc);
    }
    addI32(F, obj, S_HAIR_PARTS_ID, load32(coord + 0x08), alloc);
    addI32(F, obj, S_HAIR_COLOR_PARTS_ID, load32(coord + 0x0C), alloc);
    addI32(F, obj, S_FACE_PARTS_ID, load32(coord + 0x10), alloc);
    addI32(F, obj, S_BODY_PARTS_ID, load32(coord + 0x14), alloc);
    addI32(F, obj, S_SKIN_PARTS_ID, load32(coord + 0x18), alloc);
    setNameValue(F, valueSlot, S_ACCESSORIES_PARTS_IDS, 19);
    addMemberValue(F, obj, valueSlot, arraySlot, alloc);
}
// the accessories array of a coordinate (non-zero ids at +0x1C..+0x2C)
void fillAccessories(const Frame &F, uint32_t coord, uint32_t alloc, uint32_t arraySlot, uint32_t valueSlot) {
    setEmptyArray(F, arraySlot);
    for (uint32_t off = 0x1C; off != 0x30; off += 4) {
        int32_t id = int32_t(load32(coord + off));
        if (id == 0) continue;
        setIntValue(F, valueSlot, id);
        F.call(PUSH_BACK_VALUE, {F.at(arraySlot), F.at(valueSlot), alloc});
    }
}

}  // namespace

// medal/skill/update: {"userMedals": [{"userMedalId", "userSkillIds": [..]}]} from a std::map<u64, vector<u64>>
bool APIManager::executeMedalSkillUpdate(APIManager *self, uint32_t medalSkills, uint32_t cb) {
    Frame F(0xB8);
    uint32_t doc;
    if (!begin(F, self, cb, 0x38, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    const uint32_t medal = 0x40, skills = 0x50, medals = 0x60, name = 0x70;
    setEmptyArray(F, medals);
    for (uint32_t node = load32(medalSkills); node != medalSkills + 4; node = treeNext(node)) {
        setEmptyArray(F, skills);
        for (uint32_t p = load32(node + 0x18), end = load32(node + 0x1C); p != end; p += 8)
            F.call(PUSH_BACK_U64, {F.at(skills), 0, load32(p), load32(p + 4), alloc});
        setObjectAt(F, medal);
        addU64(F, F.at(medal), S_USER_MEDAL_ID, uint64_t(load32(node + 0x10)) | (uint64_t(load32(node + 0x14)) << 32),
               alloc);
        setNameValue(F, name, S_USER_SKILL_IDS, 12);
        addMemberValue(F, F.at(medal), name, skills, alloc);
        F.call(PUSH_BACK_VALUE, {F.at(medals), F.at(medal), alloc});
    }
    setNameValue(F, name, S_USER_MEDALS, 10);
    addMemberValue(F, doc, name, medals, alloc);
    return sendWithParams(F, self, 46, cb, 1, {0x38, 0x80, 0x30});
}
RW_FUNCTION_NOVERIFY(0x4C95F8, "APIManager::executeMedalSkillUpdate", APIManager::executeMedalSkillUpdate);

// user/keyblade/update: {"userKeyblades": [{"userKeybladeId", "userMedalIds": [..], "isFavorite"}]}
bool APIManager::executeUserKeybladeUpdate(APIManager *self, uint32_t keyblades, uint32_t cb) {
    Frame F(0xC0);
    uint32_t doc;
    if (!begin(F, self, cb, 0x40, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    const uint32_t keyblade = 0x48, medals = 0x58, list = 0x68, name = 0x78;
    setEmptyArray(F, list);
    for (uint32_t kb = load32(keyblades), end = load32(keyblades + 4); kb != end; kb += 0x58) {
        setEmptyArray(F, medals);
        for (uint32_t j = 0; j < load32(kb + 0x10); j++)          // medal count; ids at +0x18
            F.call(PUSH_BACK_U64, {F.at(medals), 0, load32(kb + 0x18 + 8 * j), load32(kb + 0x1C + 8 * j), alloc});
        setObjectAt(F, keyblade);
        addU64(F, F.at(keyblade), S_USER_KEYBLADE_ID, uint64_t(load32(kb)) | (uint64_t(load32(kb + 4)) << 32), alloc);
        setNameValue(F, name, S_USER_MEDAL_IDS, 12);
        addMemberValue(F, F.at(keyblade), name, medals, alloc);
        addI32(F, F.at(keyblade), S_IS_FAVORITE, load8(kb + 0x4C), alloc);
        F.call(PUSH_BACK_VALUE, {F.at(list), F.at(keyblade), alloc});
    }
    setObject(doc);                                // SetObject() again
    setNameValue(F, name, S_USER_KEYBLADES, 13);
    addMemberValue(F, doc, name, list, alloc);
    return sendWithParams(F, self, 43, cb, 1, {0x40, 0x88, 0x38});
}
RW_FUNCTION_NOVERIFY(0x4C8ED4, "APIManager::executeUserKeybladeUpdate", APIManager::executeUserKeybladeUpdate);

// user/avatar/update: {"updateAvatarData": [coordinate objects]}; defaultNo marks the default coordinate
bool APIManager::executeUserAvatarUpdate(APIManager *self, uint32_t coordinates, int32_t defaultNo, uint32_t cb) {
    Frame F(0xB8);
    uint32_t doc;
    if (!begin(F, self, cb, 0x38, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    const uint32_t coordinate = 0x40, accessories = 0x50, list = 0x60, value = 0x70;
    setEmptyArray(F, list);
    for (uint32_t c = load32(coordinates), end = load32(coordinates + 4); c != end; c += 0x30) {
        fillAccessories(F, c, alloc, accessories, value);
        setObjectAt(F, coordinate);
        addCoordinateFields(F, F.at(coordinate), c, alloc, accessories, value, true, defaultNo);
        F.call(PUSH_BACK_VALUE, {F.at(list), F.at(coordinate), alloc});
    }
    setObject(doc);
    setNameValue(F, value, S_UPDATE_AVATAR_DATA, 16);
    addMemberValue(F, doc, value, list, alloc);
    return sendWithParams(F, self, 41, cb, 0, {0x38, 0x80, 0x30});
}
RW_FUNCTION_NOVERIFY(0x4C8B54, "APIManager::executeUserAvatarUpdate", APIManager::executeUserAvatarUpdate);

// tutorial/progress: {"progression"} plus the step's data: 1 birthday "YYYY-MM-01 00:00:00", 2 invite code,
// 4 name, 5 avatar, 6 union
bool APIManager::executeTutorialProgress(APIManager *self, int32_t progression, int32_t year, int32_t month,
                                         uint32_t name, uint32_t inviteCode, int32_t unionId, uint32_t coordinate,
                                         uint32_t cb) {
    Frame F(0x4D8);
    uint32_t doc;
    if (!begin(F, self, cb, 0x38, doc)) return false;
    uint32_t alloc = allocatorOf(doc);
    addI32(F, doc, S_PROGRESSION, uint32_t(progression), alloc);
    const uint32_t nameSlot = 0x90;
    switch (progression) {
    case 1:
        if (year && month) {
            F.call(STRINGBUFFER_CTOR, {F.at(0xB8), S_BIRTHDAY_FORMAT, uint32_t(year), uint32_t(month)});
            addString(F, doc, S_BIRTHDAY, 8, F.at(0xB8), alloc, 0x80, nameSlot);
        }
        break;
    case 2:
        if (inviteCode) addString(F, doc, S_INVITE_CODE, 10, inviteCode, alloc, 0x70, nameSlot);
        break;
    case 4:
        if (name) addString(F, doc, S_NAME, 4, name, alloc, 0x60, nameSlot);
        break;
    case 5:
        if (coordinate) {
            fillAccessories(F, coordinate, alloc, 0x50, nameSlot);
            setObjectAt(F, 0x40);
            addCoordinateFields(F, F.at(0x40), coordinate, alloc, 0x50, nameSlot, false, 0);
            setNameValue(F, nameSlot, S_UPDATE_AVATAR_DATA, 16);
            addMemberValue(F, doc, nameSlot, 0x40, alloc);
        }
        break;
    case 3:
        break;
    default:
        if (progression == 6 && unionId != 0) addI32(F, doc, S_UNION_ID, uint32_t(unionId), alloc);
        break;
    }
    return sendWithParams(F, self, 62, cb, 0, {0x38, 0xA0, 0x30});
}
RW_FUNCTION_NOVERIFY(0x4C581C, "APIManager::executeTutorialProgress", APIManager::executeTutorialProgress);

// colosseum/clear: the battle results of the running StageState plus the colosseum start reply (+0x260:
// keyblade id at +0x38, rank / stage count / stage id at +0x28..+0x30)
bool APIManager::executeColosseumClear(APIManager *self, uint32_t cb) {
    Frame F(0x90);
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});
    const uint32_t stage = load32(app + APPSTATE_STAGE);
    const uint32_t stageCtrl = load32(app + APPSTATE_STAGE + 4);
    if (stageCtrl) addShared(F, stageCtrl);
    if (!stage) {
        releaseIfSet(F, stageCtrl);
        return false;
    }
    uint32_t doc = newParams(F, 0x38);
    uint32_t alloc = allocatorOf(doc);
    addI8(F, doc, S_IS_SKIP, uint32_t(int32_t(int8_t(sbByte(stage, 1)))), alloc);
    addU32(F, doc, S_ENEMY_DEAD_NUMBER, sbWord(stage, 0x20), alloc);
    addU32(F, doc, S_MIMIC_DEAD_NUMBER, sbWord(stage, 0x24), alloc);
    addU32(F, doc, S_MAXIMUM_DAMAGE, sbWord(stage, 0x2C), alloc);
    uint32_t colosseum = self->colosseumStart.ptr;
    addU64(F, doc, S_USER_KEYBLADE_ID, uint64_t(load32(colosseum + 0x38)) | (uint64_t(load32(colosseum + 0x3C)) << 32),
           alloc);
    uint32_t app2 = F.call(APPSTATE_GET_INSTANCE, {});
    addU32(F, doc, S_BURST, sbWord(load32(app2 + APPSTATE_STAGE), 0xC), alloc);
    addU32(F, doc, S_LUX, sbWord(stage, 0x10), alloc);
    colosseum = self->colosseumStart.ptr;
    addI32(F, doc, S_RANK, load32(colosseum + 0x28), alloc);
    addI32(F, doc, S_STAGE_COUNT, load32(colosseum + 0x2C), alloc);
    addI32(F, doc, S_STAGE_ID, load32(colosseum + 0x30), alloc);
    sendWithParams(F, self, 118, cb, 1, {0x38, 0x58, 0x30});
    releaseIfSet(F, stageCtrl);
    return true;
}
RW_FUNCTION_NOVERIFY(0x4D1954, "APIManager::executeColosseumClear", APIManager::executeColosseumClear);

// raid/clear: {"parts": [{raidEnemyId, partsId, hp, damage}] for the first `count` RaidParts (SafeBuffers of
// 0x24 bytes), "getPoint": {exp, money, lux}, "maximumDamage", "burst"}
bool APIManager::executeRaidClear(APIManager *self, int32_t count, uint32_t parts, uint32_t cb) {
    Frame F(0xE8);
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});
    const uint32_t stage = load32(app + APPSTATE_STAGE);
    const uint32_t stageCtrl = load32(app + APPSTATE_STAGE + 4);
    if (stageCtrl) addShared(F, stageCtrl);
    if (!stage) {
        releaseIfSet(F, stageCtrl);
        return false;
    }
    uint32_t doc = newParams(F, 0x44);
    uint32_t alloc = allocatorOf(doc);
    const uint32_t part = 0x80, list = 0x90, name = 0xA0, points = 0x70;
    setEmptyArray(F, list);
    for (int32_t i = 0; i < count; i++) {
        uint32_t p = parts + uint32_t(i) * 0x24;
        setObjectAt(F, part);
        addI32(F, F.at(part), S_RAID_ENEMY_ID, sbWord(p, 0), alloc);
        addI32(F, F.at(part), S_PARTS_ID, sbWord(p, 4), alloc);
        addI32(F, F.at(part), S_HP, sbWord(p, 8), alloc);
        addI32(F, F.at(part), S_DAMAGE, sbWord(p, 12), alloc);
        F.call(PUSH_BACK_VALUE, {F.at(list), F.at(part), alloc});
    }
    setNameValue(F, name, S_PARTS, 5);
    addMemberValue(F, doc, name, list, alloc);
    setObjectAt(F, points);
    addU32(F, F.at(points), S_EXP, sbWord(stage, 0x18), alloc);
    addU32(F, F.at(points), S_MONEY, sbWord(stage, 0x1C), alloc);
    addU32(F, F.at(points), S_LUX, sbWord(stage, 0x10), alloc);
    setNameValue(F, name, S_GET_POINT, 8);
    addMemberValue(F, doc, name, points, alloc);
    addU32(F, doc, S_MAXIMUM_DAMAGE, sbWord(stage, 0x2C), alloc);
    uint32_t app2 = F.call(APPSTATE_GET_INSTANCE, {});
    addU32(F, doc, S_BURST, sbWord(load32(app2 + APPSTATE_STAGE), 0xC), alloc);
    sendWithParams(F, self, 113, cb, 1, {0x44, 0xB0, 0x3C});
    releaseIfSet(F, stageCtrl);
    return true;
}
RW_FUNCTION_NOVERIFY(0x4CC768, "APIManager::executeRaidClear", APIManager::executeRaidClear);
