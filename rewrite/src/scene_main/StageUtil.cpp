// StageUtil: quest helpers - the prizes an enemy / treasure drops (from the stage-start reply and the reward
// master), story lookups, raid globals and the colosseum symbol-enemy list read from the colosseum map files.
#include "core/Guest.hpp"
#include "core/SafeBuffer.hpp"

namespace {

namespace g {
constexpr uint32_t apiManager_getInstance = 0x4BA8C9;
constexpr uint32_t sceneManager_getInstance = 0x60C26D;
constexpr uint32_t reward_create = 0x78B5C1;          // master::Reward::create(int) (sret shared_ptr)
constexpr uint32_t stage_create = 0x78E4E9;           // master::Stage::create(int)
constexpr uint32_t raidSetting_create = 0x78A62D;     // master::RaidSetting::create(int)
constexpr uint32_t array4_at = 0x508555;              // std::array<int, 4>::at(size_t)
constexpr uint32_t treasureFn_swap = 0x764739;        // std::function<StageStart::Treasure(unsigned)>::swap
constexpr uint32_t treasureFn_call = 0x764081;        // std::function<StageStart::Treasure(unsigned)>::operator()
constexpr uint32_t story_dtor = 0x764255;             // hole::network::api::Stage::Story::~Story
constexpr uint32_t aeabi_memset = 0x40E7A0;           // __aeabi_memset(dst, n, c)
constexpr uint32_t fileUtils_getInstance = 0xA3F44D;
constexpr uint32_t stringBuffer_ctor = 0x49140D;      // StringBuffer::StringBuffer(const char* fmt, ...)
constexpr uint32_t strlen_ = 0x40E08C;
constexpr uint32_t string_init = 0x47CD93;            // std::string::__init(const char*, size_t)
constexpr uint32_t data_getBytes = 0x8ED90B;          // cocos2d::Data::getBytes() const
constexpr uint32_t data_clear = 0x8ED8B3;             // cocos2d::Data::clear()
constexpr uint32_t data_dtor = 0x8ED89B;              // cocos2d::Data::~Data()
constexpr uint32_t le16 = 0x75991D;                   // StageManager::getInteger16bitFromLittleEndianBytes(uint8_t*)
constexpr uint32_t le32 = 0x75992B;                   // StageManager::getInteger32bitFromLittleEndianBytes(uint8_t*)
constexpr uint32_t symbolList_pushBackSlow = 0x7645F5;   // vector<SymbolEnemyListInfo>::__push_back_slow_path
}  // namespace g

// APIManager members (byte offsets)
constexpr uint32_t API_STAGE_START = 0x13C;       // StageStart* (SafeBuffer): enemy drops and treasures
constexpr uint32_t API_RAID_STAGE_START = 0x260;  // raid StageStart
constexpr uint32_t API_STORY_LIST = 0x124;        // vector<Stage::Story>* (normal stories)
constexpr uint32_t API_EVENT_STORY_LIST = 0x12C;  // vector<event{vector<Stage::Story>, ...}>* (0x38-byte events)

// globals
constexpr uint32_t EMPTY_STORY = 0x10040D4;       // static Stage::Story returned when nothing matches
constexpr uint32_t EMPTY_STORY_GUARD = 0x10040D0;
constexpr uint32_t COLOSSEUM_DATA_FILE = 0xFEF1D8;      // StageManager::COLOSSEUM_DATA_FILE (const char*)
constexpr uint32_t COLOSSEUM_MAP_DATA_FILE = 0xFEF1DC;  // StageManager::COLOSSEUM_MAP_DATA_FILE

// master::Reward row (data positions): 4 entries of item type / id / count / flag
constexpr uint32_t REWARD_COUNT = 0x04, REWARD_FLAG = 0x08, REWARD_TYPE = 0x18, REWARD_ID = 0x28,
                   REWARD_AMOUNT = 0x68;

constexpr uint32_t TREASURE_FN_A = 0xF6E5EC;   // getTreasurePrizes $_1 (raid): __func vtable + 8
constexpr uint32_t TREASURE_FN_B = 0xF6E618;   // getTreasurePrizes $_0 (normal)

uint32_t ld(uint32_t a) { return rw::load<uint32_t>(a); }
const SafeBuffer *buf(uint32_t a) { return rw::host_ptr<SafeBuffer>(a); }
// SafeBuffer::read(dst, data + index, n, index)
void readAt(uint32_t sb, uint32_t dst, uint32_t index, uint32_t n) {
    const SafeBuffer *b = buf(sb);
    SafeBuffer::read(b, rw::host_ptr<uint8_t>(dst), b->data.get() + index, n, index);
}
uint32_t at(uint32_t array, uint32_t k) { return ld(rw::call(g::array4_at, {array, k})); }

// The prizes of reward `rewardId` the drop record allows: the original's frame (offsets from sp) is shared by
// getEnemyDropPrizes and getTreasurePrizes - reward shared_ptr +0x24, reward count +0x2C, flags +0x30, amounts
// +0x40, ids +0x50, types +0x60, the drop record {id, count, int types[4]} at +0x70.
uint32_t fillPrizes(uint32_t sp, uint32_t items, int32_t rewardId) {
    rw::call(g::reward_create, {sp + 0x24, uint32_t(rewardId)});
    uint32_t reward = sp + 0x24;
    readAt(ld(reward), sp + 0x60, REWARD_TYPE, 0x10);
    readAt(ld(reward), sp + 0x50, REWARD_ID, 0x10);
    readAt(ld(reward), sp + 0x40, REWARD_AMOUNT, 0x10);
    readAt(ld(reward), sp + 0x30, REWARD_FLAG, 0x10);
    readAt(ld(reward), sp + 0x2C, REWARD_COUNT, 4);
    uint32_t rewardCount = ld(sp + 0x2C), dropCount = ld(sp + 0x74);
    uint32_t n = dropCount < rewardCount ? dropCount : rewardCount;
    uint32_t found = 0;
    for (uint32_t k = 0; k < n; k++) {
        int32_t dropType = int32_t(at(sp + 0x78, k));
        int32_t type = int32_t(at(sp + 0x60, k));
        uint32_t id = at(sp + 0x50, k);
        uint32_t amount = at(sp + 0x40, k);
        uint32_t flag = rw::call(g::array4_at, {sp + 0x30, k});
        if (uint32_t(dropType - 2) <= 0x12 && dropType == type) {
            uint32_t item = items + k * 16;   // MapPrizes::Item {type, id, amount, bool rare}
            uint32_t f = ld(flag);
            rw::save<int32_t>(item, dropType);
            rw::save<uint32_t>(item + 4, id);
            rw::save<uint32_t>(item + 8, amount);
            found = 1;
            rw::save<uint8_t>(item + 12, f == 2 ? 1 : 0);
        }
    }
    if (ld(reward + 4)) guest::release(ld(reward + 4));
    return found & 1;
}

}  // namespace

namespace StageUtil {

// The drops of enemy `enemyId` (normal play only): its record in the stage-start reply against reward `rewardId`.
uint32_t getEnemyDropPrizes(uint32_t items, int32_t enemyId, int32_t rewardId, int32_t playStyle) {
    guest::Frame f(0x88);
    const uint32_t sp = f.addr();
    uint32_t api = rw::call(g::apiManager_getInstance, {});
    if (playStyle != 0) return 0;
    uint32_t stageStart = ld(api + API_STAGE_START);
    if (!stageStart) return 0;
    uint32_t count = ld(stageStart + 0x24);
    if (!count) return 0;
    uint32_t offset = 0;
    for (uint32_t k = 0;;) {
        uint32_t index = offset + ld(stageStart + 0x20) * 4;   // enemy drop records start after the header words
        readAt(stageStart, sp + 0x70, index, 0x18);
        k++;
        if (rw::load<int32_t>(sp + 0x70) == enemyId) break;
        offset += 0x18;
        if (k >= count) return 0;
        stageStart = ld(api + API_STAGE_START);
    }
    return fillPrizes(sp, items, rewardId);
}
RW_FUNCTION(0x763C1C, "StageUtil::getEnemyDropPrizes", getEnemyDropPrizes);

// The prizes of treasure `treasureId` (normal or raid play) against reward `rewardId`.
uint32_t getTreasurePrizes(uint32_t items, int32_t treasureId, int32_t rewardId, int32_t playStyle) {
    guest::Frame f(0xC0);
    const uint32_t sp = f.addr();
    const uint32_t fn = sp + 0x88, tmp = sp + 0xA0;   // std::function<Treasure(unsigned)>: the getter, a temporary
    rw::save<uint32_t>(fn + 16, 0);
    uint32_t count;
    if (playStyle == 1) {
        uint32_t api = rw::call(g::apiManager_getInstance, {});
        count = ld(ld(api + API_RAID_STAGE_START) + 0x20);
        rw::save<uint32_t>(tmp + 16, tmp);
        rw::save<uint32_t>(tmp, TREASURE_FN_A);
    } else if (playStyle == 0) {
        uint32_t api = rw::call(g::apiManager_getInstance, {});
        count = ld(ld(api + API_STAGE_START) + 0x28);
        rw::save<uint32_t>(tmp + 16, tmp);
        rw::save<uint32_t>(tmp, TREASURE_FN_B);
    } else {
        return 0;   // fn is empty: nothing to destroy
    }
    rw::call(g::treasureFn_swap, {tmp, fn});
    uint32_t p = ld(tmp + 16);
    if (p == tmp) rw::call(ld(ld(p) + 0x10), {p});        // destroy()
    else if (p) rw::call(ld(ld(p) + 0x14), {p});          // destroy_deallocate()
    if (!ld(fn + 16)) return 0;

    uint32_t result = 0;
    if (count) {
        for (uint32_t k = 0;;) {
            rw::call(g::treasureFn_call, {sp + 0x70, fn, k});
            k++;
            if (rw::load<int32_t>(sp + 0x70) == treasureId) {
                result = fillPrizes(sp, items, rewardId);
                break;
            }
            if (k >= count) break;
        }
    }
    p = ld(fn + 16);
    if (p == fn) rw::call(ld(ld(fn) + 0x10), {fn});
    else if (p) rw::call(ld(ld(p) + 0x14), {p});
    return result;
}
RW_FUNCTION(0x763DF8, "StageUtil::getTreasurePrizes", getTreasurePrizes);

// The story `storyId` of the normal (kind 1) or event (kind 2) story lists; an empty static Story when missing.
uint32_t findStory(int32_t storyId, int32_t kind) {
    if (guest::staticInitBegin(EMPTY_STORY_GUARD)) {
        rw::call(g::aeabi_memset, {EMPTY_STORY, 0x18, 0});
        rw::call(guest::CXA_ATEXIT, {g::story_dtor, EMPTY_STORY, guest::DSO_HANDLE});
        guest::staticInitDone(EMPTY_STORY_GUARD);
    }
    uint32_t api = rw::call(g::apiManager_getInstance, {});
    uint32_t api2 = rw::call(g::apiManager_getInstance, {});
    if (kind == 2) {
        uint32_t events = ld(api2 + API_EVENT_STORY_LIST);
        if (!events) return EMPTY_STORY;
        uint32_t end = ld(events + 4);
        for (uint32_t e = ld(events); e != end; e += 0x38) {
            uint32_t storiesEnd = ld(e + 4);
            for (uint32_t s = ld(e); s != storiesEnd; s += 0x18)
                if (rw::load<int32_t>(s) == storyId) return s;
        }
        return EMPTY_STORY;
    }
    if (kind != 1) return EMPTY_STORY;
    uint32_t stories = ld(api + API_STORY_LIST);
    if (!stories) return EMPTY_STORY;
    uint32_t s = ld(stories), end = ld(stories + 4);
    if (end == s) return EMPTY_STORY;
    uint32_t count = (uint32_t(int32_t(end - s) >> 3)) * 0xAAAAAAABu;   // (end - begin) / 24
    for (uint32_t k = 0;;) {
        k++;
        if (rw::load<int32_t>(s) == storyId) return s;
        s += 0x18;
        if (k >= count) return EMPTY_STORY;
    }
}
RW_FUNCTION(0x764100, "StageUtil::findStory", findStory);

// Copy the raid's identity and battle settings into SceneManager for the raid scenes.
void setupRaidGlobalVariables(uint32_t info) {
    guest::Frame f(0x30);
    const uint32_t sp = f.addr();
    const uint32_t setting = sp + 0x10, stage = sp + 0x18;   // shared_ptrs
    rw::call(g::stage_create, {stage, ld(info + 0x24)});
    readAt(ld(stage), sp + 0x20, 0x3EC, 4);                  // the stage's raid setting id
    rw::call(g::raidSetting_create, {setting, ld(sp + 0x20)});
    int32_t kind = rw::load<int32_t>(info + 0x3C);
    uint32_t value;
    if (kind == 2) {
        readAt(ld(setting), sp + 0x24, 0x10, 4);
        value = ld(sp + 0x24);
    } else {
        readAt(ld(setting), sp + 0x28, 4, 4);
        value = ld(sp + 0x28);
    }
    uint32_t sm = rw::call(g::sceneManager_getInstance, {});
    rw::save<uint8_t>(sm + 0x2EC, kind == 2 ? 1 : 0);
    rw::save<uint32_t>(sm + 0x2C8, 0);
    uint32_t w0 = ld(info), w1 = ld(info + 4);
    rw::save<uint32_t>(sm + 0x2D4, w1);
    rw::save<uint32_t>(sm + 0x2D0, w0);
    rw::save<uint32_t>(sm + 0x2E4, value);
    readAt(ld(stage), sp + 0x2C, 0x114, 4);
    rw::save<uint32_t>(sm + 0x2E8, ld(sp + 0x2C));
    if (ld(setting + 4)) guest::release(ld(setting + 4));
    if (ld(stage + 4)) guest::release(ld(stage + 4));
}
RW_FUNCTION(0x764268, "StageUtil::setupRaidGlobalVariables", setupRaidGlobalVariables);

// Append the symbol enemies of every map of colosseum `colosseumId` (read from the colosseum data files).
void getColosseumSymbolEnemyList(uint32_t list, int32_t colosseumId) {
    guest::Frame f(0x880);
    const uint32_t sp = f.addr();
    const uint32_t info = sp + 0x30;          // SymbolEnemyListInfo {map, x, y, enemy}
    const uint32_t mapName = sp + 0x40, mapData = sp + 0x4C, name = sp + 0x54, data = sp + 0x60;
    const uint32_t mapPath = sp + 0x68, path = sp + 0x46C;   // StringBuffers
    uint32_t files = rw::call(g::fileUtils_getInstance, {});
    uint32_t getData = ld(ld(files) + 0x10);  // FileUtils::getDataFromFile(const std::string&) (sret Data)
    rw::call(g::stringBuffer_ctor, {path, ld(ld(COLOSSEUM_DATA_FILE)), uint32_t(colosseumId)});
    rw::call(g::string_init, {name, path, rw::call(g::strlen_, {path})});
    rw::call(getData, {data, files, name});
    if (rw::load<uint8_t>(name) & 1) guest::deleteObject(ld(name + 8));
    int32_t maps = int32_t(rw::call(g::le16, {rw::call(g::data_getBytes, {data}) + 8}));
    for (int32_t i = 0; i < maps; i++) {
        uint32_t fu = rw::call(g::fileUtils_getInstance, {});
        uint32_t fmt = ld(ld(COLOSSEUM_MAP_DATA_FILE));
        uint32_t get = ld(ld(fu) + 0x10);
        rw::call(g::stringBuffer_ctor, {mapPath, fmt, uint32_t(colosseumId), uint32_t(i)});
        rw::call(g::string_init, {mapName, mapPath, rw::call(g::strlen_, {mapPath})});
        rw::call(get, {mapData, fu, mapName});
        if (rw::load<uint8_t>(mapName) & 1) guest::deleteObject(ld(mapName + 8));
        uint32_t bytes = rw::call(g::data_getBytes, {mapData});
        uint32_t header = rw::call(g::le32, {bytes + 0x24});
        uint32_t symbols = header + bytes;
        int32_t n = int32_t(rw::call(g::le32, {symbols + 0x34}));
        uint32_t enemies = header + uint32_t(n) * 0x44 + 0x3C;   // the enemy table follows the 0x44-byte symbols
        uint32_t sym = symbols + 0x3C;
        for (int32_t j = 0; j < n; j++, sym += 0x44) {
            uint32_t enemy = rw::call(g::le32, {sym});
            rw::save<int32_t>(info, i);
            uint32_t e = enemies + (enemy << 5) + bytes;
            rw::save<uint32_t>(info + 4, rw::call(g::le32, {e + 8}));
            rw::save<uint32_t>(info + 8, rw::call(g::le32, {e + 0xC}));
            rw::save<uint32_t>(info + 12, rw::call(g::le32, {e + 0x14}));
            uint32_t end = ld(list + 4);
            if (end == ld(list + 8)) {
                rw::call(g::symbolList_pushBackSlow, {list, info});
            } else {
                uint32_t next = 0;
                if (end) {
                    uint32_t a = ld(info), b = ld(info + 4), c = ld(info + 8), d = ld(info + 12);
                    rw::save<uint32_t>(end, a);
                    rw::save<uint32_t>(end + 4, b);
                    rw::save<uint32_t>(end + 8, c);
                    rw::save<uint32_t>(end + 12, d);
                    next = ld(list + 4);
                }
                rw::save<uint32_t>(list + 4, next + 0x10);
            }
        }
        rw::call(g::data_clear, {mapData});
        rw::call(g::data_dtor, {mapData});
    }
    rw::call(g::data_clear, {data});
    rw::call(g::data_dtor, {data});
}
RW_FUNCTION(0x764394, "StageUtil::getColosseumSymbolEnemyList", getColosseumSymbolEnemyList);

}  // namespace StageUtil
