// APIManager: quest (stage) requests: start, clear, retire, continue.
#include "APIManager.hpp"

using namespace apim;

namespace {

constexpr uint32_t SAFEBUFFER_READ = 0x45A269;        // SafeBuffer::read(dst, src, n, index)
constexpr uint32_t INT_MAP_FIND = 0x494251;           // std::unordered_map<int,int>::find (__hash_table::find<int>)
constexpr uint32_t INT_MAP_INSERT = 0x4B18D1;         // __hash_table::__insert_unique<std::pair<int,int>>
constexpr uint32_t INT_MAP_DTOR = 0x4942D3;           // __hash_table<int,int>::~__hash_table

constexpr uint32_t APPSTATE_STAGE = 0x20;             // AppState: shared_ptr<StageState> of the running quest

// StageState data positions (the quest's results, see battle/StageState.hpp)
constexpr uint32_t ST_BURST = 0x0C;
constexpr uint32_t ST_LUX = 0x10;
constexpr uint32_t ST_RAID_POINT = 0x14;
constexpr uint32_t ST_EXP = 0x18;
constexpr uint32_t ST_MONEY = 0x1C;
constexpr uint32_t ST_ENEMY_DEAD = 0x20;
constexpr uint32_t ST_MIMIC_DEAD = 0x24;
constexpr uint32_t ST_RARE_ENEMY_DEAD = 0x28;
constexpr uint32_t ST_MAXIMUM_DAMAGE = 0x2C;
constexpr uint32_t ST_MATERIALS = 0x38;               // 1000 x {materialId, ?}, -1 terminated
constexpr uint32_t ST_ENEMY_DROPS = 0x1F78;           // 1000 x {itemId, ?}
constexpr uint32_t ST_TREASURES = 0x3EB8;             // 100 x {treasureId, ?}
constexpr uint32_t ST_CLEAR_MISSIONS = 0x4228;        // 3 mission ids

constexpr uint32_t S_STAGE_ID = 0xDFAEF5;             // "stageId"
constexpr uint32_t S_SUPPORT_USER_ID = 0xDFB318;      // "supportUserId"
constexpr uint32_t S_USER_KEYBLADE_ID = 0xDFAEE6;     // "userKeybladeId"
constexpr uint32_t S_STAGE_SKIP = 0xDFB38F;           // "stageSkip"
constexpr uint32_t S_EXP = 0xDFB666;                  // "exp"
constexpr uint32_t S_RAID_POINT = 0xDFB8E4;           // "raidPoint"
constexpr uint32_t S_MONEY = 0xDFB90F;                // "money"
constexpr uint32_t S_LUX = 0xDFA9FF;                  // "lux"
constexpr uint32_t S_GET_POINT = 0xDFBF17;            // "getPoint"
constexpr uint32_t S_MATERIAL_ID = 0xDFB64C;          // "materialId"
constexpr uint32_t S_NUMBER = 0xDFB260;               // "number"
constexpr uint32_t S_GET_MATERIALS = 0xDFBF20;        // "getMaterials"
constexpr uint32_t S_GET_ENEMY_DROP_ITEMS = 0xDFBF2D; // "getEnemyDropItems"
constexpr uint32_t S_GET_TREASURES = 0xDFBF3F;        // "getTreasures"
constexpr uint32_t S_CLEAR_MISSION_IDS = 0xDFB399;    // "clearMissionIds"
constexpr uint32_t S_ENEMY_DEAD_NUMBER = 0xDFBF4C;    // "enemyDeadNumber"
constexpr uint32_t S_MIMIC_DEAD_NUMBER = 0xDFBF5C;    // "mimicDeadNumber"
constexpr uint32_t S_RARE_ENEMY_DEAD_NUMBER = 0xDFBF6C; // "rareEnemyDeadNumber"
constexpr uint32_t S_MAXIMUM_DAMAGE = 0xDFBF80;       // "maximumDamage"
constexpr uint32_t S_BURST = 0xDFB632;                // "burst"

// GenericValue(unsigned) at the frame slot
void setUintValue(const Frame &F, uint32_t slot, uint32_t v) {
    F.setWord(slot + 12, JSON_UINT_HIGH_BIT);
    F.setWord(slot + 4, 0);
    F.setWord(slot + 0, v);
    if (int32_t(v) >= 0) F.setWord(slot + 12, JSON_INT_POSITIVE);
}

}  // namespace

// stage/start: {"stageId", "supportUserId", "userKeybladeId", "stageSkip": 1 when skipping}
bool APIManager::executeStageStart(APIManager *self, int32_t stageId, uint64_t supportUserId, uint64_t userKeybladeId,
                                   uint32_t skip, uint32_t cb) {
    Frame F(0x80);
    if (refuseRequest(F, self, cb)) return false;
    self->allowWhileBusy = 0;
    uint32_t doc = newParams(F, 0x40);
    uint32_t alloc = allocatorOf(doc);
    addI32(F, doc, S_STAGE_ID, uint32_t(stageId), alloc);
    addU64(F, doc, S_SUPPORT_USER_ID, supportUserId, alloc);
    addU64(F, doc, S_USER_KEYBLADE_ID, userKeybladeId, alloc);
    if (skip == 1) addI32(F, doc, S_STAGE_SKIP, 1, alloc);
    self->retryable = 0;
    uint32_t ctrl = keepParams(F, self, 0x40);
    sendRequest(F, self, 103, cb, 0x48, 0x38);
    releaseIfSet(F, ctrl);
    return true;
}
RW_FUNCTION_NOVERIFY(0x4CB22C, "APIManager::executeStageStart", APIManager::executeStageStart);

// stage/clear: the results of the running quest (AppState's StageState): points, collected materials (counted
// per material id), enemy drops, treasures, cleared missions, kill counts, maximum damage and burst.
bool APIManager::executeStageClear(APIManager *self, uint32_t cb) {
    Frame F(0x42F0);
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
    const uint32_t stageData = load32(stage + 12);
    uint32_t doc = newParams(F, 0x6C);
    uint32_t alloc = allocatorOf(doc);

    // "getPoint": {exp, raidPoint, money, lux}
    setObjectAt(F, 0xE8);
    addU32(F, F.at(0xE8), S_EXP, sbWord(stage, ST_EXP), alloc);
    addU32(F, F.at(0xE8), S_RAID_POINT, sbWord(stage, ST_RAID_POINT), alloc);
    addU32(F, F.at(0xE8), S_MONEY, sbWord(stage, ST_MONEY), alloc);
    addU32(F, F.at(0xE8), S_LUX, sbWord(stage, ST_LUX), alloc);
    setNameValue(F, 0x2394, S_GET_POINT, 8);
    addMemberValue(F, doc, 0x2394, 0xE8, alloc);

    // "getMaterials": [{materialId, number}] - the collected materials counted in an unordered_map<int, int>
    F.call(SAFEBUFFER_READ, {stage, F.at(0x2394), stageData + ST_MATERIALS, 8000, ST_MATERIALS});
    const uint32_t counts = F.at(0x58);
    F.setWord(0x58, 0);
    F.setWord(0x5C, 0);
    F.setWord(0x60, 0);
    F.setWord(0x64, 0);
    F.setWord(0x68, 0x3F800000);                               // max_load_factor 1.0f
    for (uint32_t k = 0; k < 1000; k++) {
        uint32_t entry = F.at(0x2394 + 8 * k);
        int32_t materialId = int32_t(load32(entry));
        if (materialId == -1) break;
        uint32_t node = F.call(INT_MAP_FIND, {counts, entry});
        if (node) {
            rw::save<uint32_t>(node + 12, load32(node + 12) + 1);
        } else {
            F.setWord(0x50, uint32_t(materialId));
            F.setWord(0x54, 1);
            F.call(INT_MAP_INSERT, {F.at(0xF8), counts, F.at(0x50)});
        }
    }
    setEmptyArray(F, 0xD8);
    for (uint32_t node = F.word(0x60); node; node = load32(node)) {
        setObjectAt(F, 0xC8);
        addI32(F, F.at(0xC8), S_MATERIAL_ID, load32(node + 8), alloc);
        addI32(F, F.at(0xC8), S_NUMBER, load32(node + 12), alloc);
        F.call(PUSH_BACK_VALUE, {F.at(0xD8), F.at(0xC8), alloc});
    }
    setNameValue(F, 0x454, S_GET_MATERIALS, 12);
    addMemberValue(F, doc, 0x454, 0xD8, alloc);

    // "getEnemyDropItems" / "getTreasures": the ids, -1 terminated
    auto idArray = [&](uint32_t arraySlot, uint32_t bufferSlot, uint32_t index, uint32_t count) {
        setEmptyArray(F, arraySlot);
        F.call(SAFEBUFFER_READ, {stage, F.at(bufferSlot), stageData + index, count * 8, index});
        for (uint32_t k = 0; k < count; k++) {
            int32_t id = int32_t(F.word(bufferSlot + 8 * k));
            if (id == -1) break;
            setIntValue(F, 0x100, id);
            F.call(PUSH_BACK_VALUE, {F.at(arraySlot), F.at(0x100), alloc});
        }
    };
    idArray(0xB8, 0x454, ST_ENEMY_DROPS, 1000);
    setNameValue(F, 0x134, S_GET_ENEMY_DROP_ITEMS, 17);
    addMemberValue(F, doc, 0x134, 0xB8, alloc);
    idArray(0xA8, 0x134, ST_TREASURES, 100);
    setNameValue(F, 0x100, S_GET_TREASURES, 12);
    addMemberValue(F, doc, 0x100, 0xA8, alloc);

    // "clearMissionIds": up to 3 (unsigned values)
    setEmptyArray(F, 0x98);
    F.call(SAFEBUFFER_READ, {stage, F.at(0x128), stageData + ST_CLEAR_MISSIONS, 12, ST_CLEAR_MISSIONS});
    for (uint32_t k = 0; k < 3; k++) {
        uint32_t id = F.word(0x128 + 4 * k);
        if (id == 0xFFFFFFFFu) break;
        setUintValue(F, 0x100, id);
        F.call(PUSH_BACK_VALUE, {F.at(0x98), F.at(0x100), alloc});
    }
    setNameValue(F, 0x100, S_CLEAR_MISSION_IDS, 15);
    addMemberValue(F, doc, 0x100, 0x98, alloc);

    addU32(F, doc, S_ENEMY_DEAD_NUMBER, sbWord(stage, ST_ENEMY_DEAD), alloc);
    addU32(F, doc, S_MIMIC_DEAD_NUMBER, sbWord(stage, ST_MIMIC_DEAD), alloc);
    addU32(F, doc, S_RARE_ENEMY_DEAD_NUMBER, sbWord(stage, ST_RARE_ENEMY_DEAD), alloc);
    addU32(F, doc, S_MAXIMUM_DAMAGE, sbWord(stage, ST_MAXIMUM_DAMAGE), alloc);
    uint32_t app2 = F.call(APPSTATE_GET_INSTANCE, {});
    addU32(F, doc, S_BURST, sbWord(load32(app2 + APPSTATE_STAGE), ST_BURST), alloc);

    self->retryable = 1;
    uint32_t ctrl = F.word(0x70);
    assignShared(F, self->params, doc, ctrl);
    sendRequest(F, self, 106, cb, 0x110, 0x48);
    F.call(INT_MAP_DTOR, {counts});
    releaseIfSet(F, ctrl);
    releaseIfSet(F, stageCtrl);
    return true;
}
RW_FUNCTION_NOVERIFY(0x4CB770, "APIManager::executeStageClear", APIManager::executeStageClear);
