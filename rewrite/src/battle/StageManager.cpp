#include "StageManager.hpp"

#include <initializer_list>

namespace {

using rw::call;

// ------------------------------------------------------------------------------------------- guest functions
// (as the lifted code calls them: Thumb bit set, PLT stubs for imports)
namespace g {
constexpr uint32_t operator_new = 0xC98EF1;          // operator new(unsigned)
constexpr uint32_t operator_delete = 0xC98F9B;       // operator delete(void*)
constexpr uint32_t new_array = 0xC98F6F;             // operator new[](unsigned)
constexpr uint32_t delete_array = 0xC98FB3;          // operator delete[](void*)
constexpr uint32_t release_shared = 0xC8DBD1;        // std::__shared_weak_count::__release_shared()
constexpr uint32_t memset_ = 0x40E7A0;               // __aeabi_memset(dest, n, c)
constexpr uint32_t memcpy_ = 0x40E164;               // __aeabi_memcpy(dest, src, n)
constexpr uint32_t strlen_ = 0x40E08C;               // strlen
constexpr uint32_t divsi3 = 0xCD5E8D;                // __divsi3
constexpr uint32_t string_init = 0x47CD93;           // std::string::__init(const char*, size_t) (sub_47CD92)
constexpr uint32_t StringBuffer_ctor = 0x49140D;     // StringBuffer::StringBuffer(const char *fmt, ...)
constexpr uint32_t Ref_ctor = 0x8FD025;              // cocos2d::Ref::Ref()
constexpr uint32_t Ref_dtor = 0x8FD059;              // cocos2d::Ref::~Ref()
constexpr uint32_t Ref_autorelease = 0x8FD0D5;       // cocos2d::Ref::autorelease()
constexpr uint32_t Ref_retain = 0x8FD0B5;            // cocos2d::Ref::retain()
constexpr uint32_t Ref_release = 0x8FD0BD;           // cocos2d::Ref::release()
constexpr uint32_t Vec2_ctor = 0x8E4B95;             // cocos2d::Vec2::Vec2()
constexpr uint32_t Vec2_ctor2 = 0x8E4BB7;            // cocos2d::Vec2::Vec2(const Vec2 &from, const Vec2 &to)
constexpr uint32_t Vec2_dtor = 0x8E4C17;             // cocos2d::Vec2::~Vec2()
constexpr uint32_t Vec2_distance = 0x8E4EF5;         // cocos2d::Vec2::distance(const Vec2&) const
constexpr uint32_t Size_ctor = 0x8DF01F;             // cocos2d::Size::Size()
constexpr uint32_t Rect_ctor = 0x8DF171;             // cocos2d::Rect::Rect()
constexpr uint32_t Data_copy = 0x8ED841;             // cocos2d::Data::Data(const Data&)
constexpr uint32_t Data_dtor = 0x8ED89B;             // cocos2d::Data::~Data()
constexpr uint32_t Data_isNull = 0x8ED8EF;           // cocos2d::Data::isNull() const
constexpr uint32_t Data_clear = 0x8ED8B3;            // cocos2d::Data::clear()
constexpr uint32_t Data_getBytes = 0x8ED90B;         // cocos2d::Data::getBytes() const
constexpr uint32_t Data_getSize = 0x8ED90F;          // cocos2d::Data::getSize() const
constexpr uint32_t Data_assign = 0x8ED8C7;           // cocos2d::Data::operator=(const Data&)
constexpr uint32_t Data_moveAssign = 0x8ED8DF;       // cocos2d::Data::operator=(Data&&)
constexpr uint32_t Data_Null = 0x1006984;            // cocos2d::Data::Null
constexpr uint32_t FileUtils_getInstance = 0xA3F44D; // cocos2d::FileUtils::getInstance()
constexpr uint32_t VSLOT_GET_DATA_FROM_FILE = 4;     // FileUtils::getDataFromFile(const std::string&) (sret)
constexpr uint32_t FadeOut_create = 0x86B25D;        // cocos2d::FadeOut::create(float)
constexpr uint32_t FadeIn_create = 0x86B115;         // cocos2d::FadeIn::create(float)
constexpr uint32_t Hide_create = 0x8665E9;           // cocos2d::Hide::create()
constexpr uint32_t Show_create = 0x866591;           // cocos2d::Show::create()
constexpr uint32_t Sequence_create = 0x867999;       // cocos2d::Sequence::create(FiniteTimeAction*, ...)
constexpr uint32_t Node_runAction = 0x894AED;        // cocos2d::Node::runAction(Action*)
constexpr uint32_t SafeBuffer_ctor = 0x459F21;       // SafeBuffer::SafeBuffer(unsigned, bool)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;       // SafeBuffer::~SafeBuffer()
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t Misc_create = 0x786D5D;           // master::Misc::create(int) (sret shared_ptr)
constexpr uint32_t Stage_create = 0x78E4E9;          // master::Stage::create(int)
constexpr uint32_t ColosseumStage_create = 0x77CF95; // master::ColosseumStage::create(int)
constexpr uint32_t Colosseum_create = 0x77C4E9;      // master::Colosseum::create(int)
constexpr uint32_t getCurrentKeyblade = 0x4976FD;    // AvatarUtil::getCurrentKeyblade() (sret)
constexpr uint32_t forEachMedal = 0x5E007D;          // MedalDeck::forEachMedal(const std::function<bool(const MedalInfo&)>&)
// StageState
constexpr uint32_t setSubMission04 = 0x4EFA55;
constexpr uint32_t setSubMission12 = 0x4EFB45;
constexpr uint32_t setSubMission14 = 0x4EFCC5;
constexpr uint32_t setSubMission15 = 0x4EFE45;
constexpr uint32_t setSubMission19 = 0x4EFFD1;
constexpr uint32_t getSubMission04 = 0x4EF9F5;
constexpr uint32_t getSubMission12 = 0x4EFAE5;
constexpr uint32_t getSubMission14 = 0x4EFC65;
constexpr uint32_t getSubMission15 = 0x4EFDE5;
constexpr uint32_t getSubMission19 = 0x4EFF71;
constexpr uint32_t getHasTreasureCount = 0x4EF7E1;
constexpr uint32_t getHasMaterialCount = 0x4EF74D;
constexpr uint32_t getBadStatusCount = 0x4F0061;
constexpr uint32_t addClearMission = 0x4EF495;
// map objects
constexpr uint32_t MapTreasure_firstCall = 0x5DF521, MapTreasure_finalCall = 0x5DF5C5;
constexpr uint32_t MapMaterial_firstCall = 0x5DBABD, MapMaterial_finalCall = 0x5DBB61;
constexpr uint32_t MapPrizes_firstCall = 0x5DE939, MapPrizes_finalCall = 0x5DEA99;
constexpr uint32_t LuxBoard_firstCall = 0x5D3BDD, LuxBoard_finalCall = 0x5D3CA9;
constexpr uint32_t MissBoard_firstCall = 0x5D406D, MissBoard_finalCall = 0x5D4139;
constexpr uint32_t MapFieldLayer_create = 0x5DA985;         // MapFieldLayer::create(const Info*)
constexpr uint32_t MapFieldLayer_addLayerMapNode = 0x5DB031;
constexpr uint32_t MapPlacementObject_create = 0x5DC7F5;    // MapPlacementObject::create(const Info*)
// libc++ containers of the map objects
constexpr uint32_t vecbase_MapNode_dtor = 0x5DB1CD;
constexpr uint32_t vecbase_MapPlacementObject_dtor = 0x4960B7;
constexpr uint32_t vecbase_Npc_dtor = 0x607F5D;
constexpr uint32_t vecbase_Material_dtor = 0x607F87;
constexpr uint32_t vecbase_Treasure_dtor = 0x607FB1;
constexpr uint32_t vecbase_Transfer_dtor = 0x607FDB;
constexpr uint32_t vecbase_EnemyTeam_dtor = 0x608005;
constexpr uint32_t vecbase_MapJumpInfo_dtor = 0x75C1FF;
constexpr uint32_t vec_MapJumpInfo_ctor = 0x75C1A9;         // vector<MapJumpInfo*>::vector(size_t)
constexpr uint32_t reserve_EnemyTeam = 0x75BC01;
constexpr uint32_t reserve_Transfer = 0x75BE95;
constexpr uint32_t reserve_Treasure = 0x75BF55;
constexpr uint32_t reserve_Material = 0x75C015;
constexpr uint32_t reserve_Npc = 0x75C0E9;
constexpr uint32_t push_EnemyTeam = 0x75DC19;                // __push_back_slow_path
constexpr uint32_t push_Transfer = 0x75DADD;
constexpr uint32_t push_Treasure = 0x75D9A1;
constexpr uint32_t push_Material = 0x75D865;
constexpr uint32_t push_Npc = 0x75D729;
constexpr uint32_t push_MapPlacementObject = 0x75CCCD;
constexpr uint32_t push_MapNode = 0x75CE09;
constexpr uint32_t hash_FieldLayer_dtor = 0x60805D;
constexpr uint32_t hash_FieldLayer_insert = 0x75CF19;       // __insert_unique(pair<int, MapFieldLayer*>&&) (sret)
constexpr uint32_t hash_FieldLayer_find = 0x75CC4B;
constexpr uint32_t hash_FieldLayer_at = 0x755429;           // unordered_map::at(const int&)
constexpr uint32_t hash_FieldLayer_clear = 0x75CC09;
constexpr uint32_t hash_Jump_dtor = 0x60802F;
constexpr uint32_t hash_Jump_insert = 0x75D32D;
constexpr uint32_t hash_Jump_clear = 0x75D2EB;
}  // namespace g

// ------------------------------------------------------------------------------------------- data
constexpr uint32_t STORY_STAGE_DATA_FILE = 0xFEF1D0;        // const char* "stage/mappoi_stg%05d.bin"
constexpr uint32_t STORY_STAGE_MAP_DATA_FILE = 0xFEF1D4;    // "stage/mappoi_stg%05d_%02d.bin"
constexpr uint32_t COLOSSEUM_DATA_FILE = 0xFEF1D8;          // "stage/cl_stg%08d.bin"
constexpr uint32_t COLOSSEUM_MAP_DATA_FILE = 0xFEF1DC;
constexpr uint32_t DIRECTION_TABLE = 0xE4DBA0;              // int[4] {1, -1, 1, -1} for file directions 1..4
constexpr uint32_t TRIANGLE_EDGES = 0xE9EF00;               // int[2][3][2]: corner pairs of the 2 triangles
constexpr uint32_t STR_CLS = 0xE0748F;                      // "map/%s/%s_cls.bin"
constexpr uint32_t STR_CHP = 0xE0747D;                      // "map/%s/%s_chp.bin"
constexpr uint32_t STR_JMP = 0xE0746B;                      // "map/%s/%s_jmp.bin"
constexpr uint32_t STR_INFO = 0xE07458;                     // "map/%s/%s_info.bin"
constexpr uint32_t STR_PNG = 0xE07447;                      // "map/%s/%s_%s.png"
constexpr uint32_t STR_LWF = 0xE07435;                      // "lwf/map/%s/%s.lwf"
constexpr uint32_t STR_BASE1 = 0xE0742F;                    // "base1"
constexpr uint32_t FUNCTOR_COUNT_ATTRIBUTE = 0xF6E26C;      // vtables of the forEachMedal lambdas
constexpr uint32_t FUNCTOR_HIGHER_RARITY = 0xF6E240;
constexpr uint32_t FUNCTOR_FULL_SET = 0xF6E214;
constexpr uint32_t FLOAT_ONE = 0x3F800000;

// ------------------------------------------------------------------------------------------- helpers
// The original function's stack frame, at exactly the original addresses (pushed registers + locals, no
// realignment): locals handed to guest functions live at their original sp offsets, so pointers to them
// stored into heap objects (std::function captures) and uninitialised leftovers match the original.
class Frame {
public:
    explicit Frame(uint32_t size) : saved_(rw::cpu->r[13]) {
        base_ = saved_ - size;
        rw::cpu->r[13] = base_;
    }
    ~Frame() { rw::cpu->r[13] = saved_; }
    Frame(const Frame &) = delete;
    Frame &operator=(const Frame &) = delete;
    uint32_t addr(uint32_t off) const { return base_ + off; }
    uint32_t word(uint32_t off) const { return rw::load<uint32_t>(base_ + off); }
    void setWord(uint32_t off, uint32_t v) const { rw::save<uint32_t>(base_ + off, v); }
    void setByte(uint32_t off, uint8_t v) const { rw::save<uint8_t>(base_ + off, v); }
    float f32(uint32_t off) const { return rw::load<float>(base_ + off); }
    void setF32(uint32_t off, float v) const { rw::save<float>(base_ + off, v); }
    uint8_t *host(uint32_t off) const { return rw::frame_ptr<uint8_t>(base_ + off); }

private:
    uint32_t saved_, base_;
};

inline uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
inline uint8_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }
inline uint16_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
inline void st32(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }
inline void st16(uint32_t a, uint16_t v) { rw::save<uint16_t>(a, v); }
inline void stf(uint32_t a, float v) { rw::save<float>(a, v); }
// little-endian values of the data files, read byte by byte like the original
inline uint32_t le32(uint32_t a) {
    return uint32_t(ld8(a)) | uint32_t(ld8(a + 1)) << 8 | uint32_t(ld8(a + 2)) << 16 | uint32_t(ld8(a + 3)) << 24;
}
inline uint32_t le16(uint32_t a) { return uint32_t(ld8(a)) | uint32_t(ld8(a + 1)) << 8; }
inline int32_t les16(uint32_t a) { return int16_t(uint16_t(le16(a))); }
inline float lefloat(uint32_t a) { return float(int32_t(le32(a))); }   // __floatsisf
inline float bitsToFloat(uint32_t u) { return bt::fromBits(u); }

template <class T> inline uint32_t ga(const T *p) { return rw::guest_addr(p); }

inline uint32_t vtableSlot(uint32_t obj, uint32_t slot) { return ld32(ld32(obj) + 4 * slot); }

// The StageState of the running quest (AppState::getInstance()->stageState).
inline uint32_t stageState() { return ld32(call(g::AppState_getInstance, {}) + 0x20); }
inline int32_t stageStateValue(uint32_t index) { return rw::host_ptr<SafeBuffer>(stageState())->get32(index); }

// new[] size of `count` elements of `elem` bytes plus the 8-byte cookie: __muldi3 on (count, 0) x (elem, sign)
// and the overflow checks, -1 when it overflows.
uint32_t arraySize(int32_t count, uint32_t elem) {
    uint64_t a = uint32_t(count);
    uint64_t b = (uint64_t(uint32_t(count >> 31)) << 32) | elem;
    uint64_t prod = a * b;
    uint32_t lo = uint32_t(prod), hi = uint32_t(prod >> 32);
    uint32_t total = lo + 8;
    return (total < lo || hi != 0) ? 0xFFFFFFFFu : total;
}

// vector<T*>::clear() of trivially destructible pointers: end = begin, as the inlined code computes it.
void vectorClear(uint32_t vec) {
    uint32_t begin = ld32(vec), end = ld32(vec + 4);
    if (begin != end) st32(vec + 4, end + (~(end - 4 - begin) & 0xFFFFFFFCu));
}

// vector<T*>::push_back(value) as inlined: the fast path when end != cap (or end < cap), else
// __push_back_slow_path(vec, slot) with `slot` (a guest local) already holding the value.
void pushBack(uint32_t vec, uint32_t value, uint32_t slot, uint32_t slowPath, bool lessThanCap) {
    uint32_t end = ld32(vec + 4), cap = ld32(vec + 8);
    bool fast = lessThanCap ? end < cap : end != cap;
    if (fast) {
        uint32_t base = 0;
        if (end) {
            st32(end, value);
            base = ld32(vec + 4);
        }
        st32(vec + 4, base + 4);
    } else {
        call(slowPath, {vec, slot});
    }
}

// direction 1..4 of the data files -> -1 / 1 (through the table), else 0
inline uint32_t directionFromTable(uint32_t direction) {
    uint32_t k = direction - 1;
    return k <= 3 ? ld32(DIRECTION_TABLE + 4 * k) : 0;
}
// direction 1..4 -> 16-bit 1 / 0xFFFF (the inlined convertDirectionID), else 0
inline uint16_t direction16(uint32_t direction) {
    uint32_t k = direction - 1;
    if (k > 3) return 0;
    return (k & 1) ? 0xFFFF : 1;
}

// The std::function the forEachMedal callers build: after the call, destroy it (the inlined ~function).
void destroyFunction(const Frame &f, uint32_t bufOff, uint32_t fOff) {
    uint32_t fn = f.word(fOff);
    if (fn == f.addr(bufOff)) {
        call(ld32(f.word(bufOff) + 0x10), {f.addr(bufOff)});           // __base::destroy()
    } else if (fn) {
        call(ld32(ld32(fn) + 0x14), {fn});                              // __base::destroy_deallocate()
    }
}

// cocos2d::FileUtils::getInstance()->getDataFromFile(std::string(StringBuffer(fmt, ...))) up to the call; the
// caller frees the long string afterwards (the original's order relative to the Data operations varies).
void readDataFile(uint32_t fu, uint32_t getData, uint32_t sb, uint32_t str, uint32_t data) {
    uint32_t len = call(g::strlen_, {sb});
    call(g::string_init, {str, sb, len});
    call(getData, {data, fu, str});
}
inline void freeLongString(uint32_t str) {
    if (ld8(str) & 1) call(g::operator_delete, {ld32(str + 8)});
}

}  // namespace

using SM = StageManager;
using SMI = StageManager::StageMapInfo;

// ============================================================================================ lifetime
uint32_t StageManager::create() {
    uint32_t p = call(g::operator_new, {0x170});
    StageManager *sm = rw::host_ptr<StageManager>(p);
    construct(sm);
    if (rw::vcall(sm, 2, {p}) == 1) {                     // init()
        call(g::Ref_autorelease, {p});
        return p;
    }
    rw::vcall(sm, 1, {p});                                // deleting destructor
    return 0;
}
RW_FUNCTION(0x5FC7E0, "StageManager::create", StageManager::create);

StageManager *StageManager::construct(StageManager *self) {
    uint32_t s = ga(self);
    call(g::Ref_ctor, {s});
    self->vtable = VTABLE;
    self->mode = 0;
    call(g::SafeBuffer_ctor, {s + 0x40, 4, 1});           // key from the clock: not verifiable
    self->param.vtable = StageInfoParam::VTABLE;
    StageMapInfo::construct(&self->map);
    self->prevMapId = -1;
    call(g::Vec2_ctor, {s + 0x14C});
    self->stageDataFile = 0;
    self->mapDataFile = 0;
    return self;
}
RW_FUNCTION(0x607D3C, "StageManager::StageManager", StageManager::construct);

SMI *StageManager::StageMapInfo::construct(StageMapInfo *self) {
    uint32_t s = ga(self);
    call(g::Data_copy, {s + 0x14, g::Data_Null});
    call(g::memset_, {s + 0x30, 0x54, 0});
    call(g::Data_copy, {s + 0x84, g::Data_Null});
    call(g::Data_copy, {s + 0x8C, g::Data_Null});
    self->jumps.size = 0;
    self->jumps.first = 0;
    self->jumps.bucketCount = 0;
    self->jumps.buckets = 0;
    rw::set<uint32_t>(self, 0xA4, FLOAT_ONE);             // max load factor 1.0f
    self->fieldLayers.size = 0;
    self->fieldLayers.first = 0;
    self->fieldLayers.bucketCount = 0;
    self->fieldLayers.buckets = 0;
    rw::set<uint32_t>(self, 0xC0, FLOAT_ONE);
    call(g::memset_, {s + 0xC4, 0x24, 0});
    return self;
}
RW_FUNCTION(0x607E60, "StageManager::StageMapInfo::StageMapInfo", StageManager::StageMapInfo::construct);

void StageManager::StageInfoParam::destructDelete(StageInfoParam *self) {
    uint32_t r = call(g::SafeBuffer_dtor, {ga(self)});
    call(g::operator_delete, {r});
}
RW_FUNCTION(0x60808A, "StageManager::StageInfo::Param::~Param (deleting)", StageManager::StageInfoParam::destructDelete);

StageManager *StageManager::destruct(StageManager *self) {
    uint32_t s = ga(self);
    self->vtable = VTABLE;
    call(g::MapTreasure_finalCall, {});
    call(g::MapMaterial_finalCall, {});
    call(g::MapPrizes_finalCall, {});
    call(g::LuxBoard_finalCall, {});
    call(g::MissBoard_finalCall, {});
    clearMapInfo(self);
    call(g::Vec2_dtor, {s + 0x14C});
    call(g::vecbase_MapNode_dtor, {s + 0x13C});
    call(g::vecbase_MapPlacementObject_dtor, {s + 0x130});
    call(g::vecbase_MapPlacementObject_dtor, {s + 0x124});
    call(g::hash_FieldLayer_dtor, {s + 0x110});
    call(g::hash_Jump_dtor, {s + 0xF4});
    call(g::Data_dtor, {s + 0xEC});
    call(g::Data_dtor, {s + 0xE4});
    call(g::vecbase_Npc_dtor, {s + 0xC0});
    call(g::vecbase_Material_dtor, {s + 0xB4});
    call(g::vecbase_Treasure_dtor, {s + 0xA8});
    call(g::vecbase_Transfer_dtor, {s + 0x9C});
    call(g::vecbase_EnemyTeam_dtor, {s + 0x90});
    call(g::Data_dtor, {s + 0x74});
    call(g::SafeBuffer_dtor, {s + 0x40});
    call(g::Ref_dtor, {s});
    return self;
}
RW_FUNCTION(0x758C44, "StageManager::~StageManager", StageManager::destruct);

void StageManager::destructDelete(StageManager *self) {
    StageManager *r = destruct(self);
    call(g::operator_delete, {ga(r)});
}
RW_FUNCTION(0x758E5C, "StageManager::~StageManager (deleting)", StageManager::destructDelete);

uint32_t StageManager::init(StageManager *self) {
    call(g::MapTreasure_firstCall, {});
    call(g::MapMaterial_firstCall, {});
    call(g::MapPrizes_firstCall, {});
    call(g::LuxBoard_firstCall, {});
    call(g::MissBoard_firstCall, {});
    getMasterMiscValue(self);
    uint32_t scene = ld32(call(g::SceneManager_getInstance, {}) + 0x28C);
    if (scene - 1 < 3 || scene == 7) {
        self->mode = 0;
        self->stageDataFile = ld32(STORY_STAGE_DATA_FILE);
        self->mapDataFile = ld32(STORY_STAGE_MAP_DATA_FILE);
    } else if (scene == 5) {
        self->mode = 1;
        self->stageDataFile = ld32(COLOSSEUM_DATA_FILE);
        self->mapDataFile = ld32(COLOSSEUM_MAP_DATA_FILE);
    }
    uint32_t api = call(g::APIManager_getInstance, {});
    int32_t stage = rw::host_ptr<SafeBuffer>(api + 0x33C)->get32(4);      // the started stage id
    stageFirstSetup(self, stage);
    if (self->mode == 0) setupSubMission(self);
    self->state = 2;
    return 1;
}
RW_FUNCTION(0x758E68, "StageManager::init", StageManager::init);

// ============================================================================================ stage / map loading
void StageManager::getMasterMiscValue(StageManager *self) {
    Frame f(0x48);
    static constexpr struct { uint32_t id, sret, field; } kMisc[3] = {{107, 0x1C, 0x164}, {108, 0x14, 0x168},
                                                                     {109, 0x0C, 0x16C}};
    for (const auto &m : kMisc) {
        call(g::Misc_create, {f.addr(m.sret), m.id});
        int32_t v = bt::sbGet32(f.word(m.sret), 4);
        rw::set<float>(self, m.field, float(v));
        bt::releaseShared(f.word(m.sret + 4));
    }
}
RW_FUNCTION(0x758F6C, "StageManager::getMasterMiscValue", StageManager::getMasterMiscValue);

void StageManager::stageFirstSetup(StageManager *self, int32_t stageId) {
    Frame f(0x450);
    const uint32_t str = f.addr(0x20), data = f.addr(0x2C), sb = f.addr(0x34);
    uint32_t fu = call(g::FileUtils_getInstance, {});
    uint32_t getData = vtableSlot(fu, g::VSLOT_GET_DATA_FROM_FILE);
    call(g::StringBuffer_ctor, {sb, self->stageDataFile, uint32_t(stageId)});
    readDataFile(fu, getData, sb, str, data);
    freeLongString(str);
    self->stageId = stageId;
    uint32_t b = call(g::Data_getBytes, {data});
    self->mapTotalNumber = les16(b + 0x08);
    self->nextMapId = les16(b + 0x0A);
    self->startPos.x = lefloat(b + 0x0C);
    self->startPos.y = lefloat(b + 0x10);
    self->startDirection = int32_t(directionFromTable(le32(b + 0x14)));
    self->bossEnemyId = int32_t(le32(b + 0x18));
    self->enemyPlatoonCount = int32_t(le32(b + 0x20));
    self->field_30 = int32_t(le32(b + 0x24));
    self->field_34 = int32_t(le32(b + 0x34));
    self->treasureCount = int32_t(le32(b + 0x38));
    self->materialCount = int32_t(le32(b + 0x3C));
    int32_t mode = self->mode;
    if (mode == 0) {
        uint32_t api = call(g::APIManager_getInstance, {});
        uint32_t randomSymbols = ld32(ld32(api + 0x13C) + 0x20);
        self->enemyPlatoonCount = int32_t(uint32_t(self->enemyPlatoonCount.get()) - randomSymbols);
        self->param.set32(0, 0);
    } else if (mode == 1) {
        int32_t level = getColosseumRiseLevel(rw::host_ptr<StageManager>(1));   // called with this = 1
        self->param.set32(0, level);
    }
    setNextMapInfo(self);
    call(g::Data_dtor, {data});
}
RW_FUNCTION(0x759068, "StageManager::stageFirstSetup", StageManager::stageFirstSetup);

int32_t StageManager::getColosseumRiseLevel(StageManager * /*self*/) {
    Frame f(0x50);
    uint32_t api1 = call(g::APIManager_getInstance, {});
    uint32_t api2 = call(g::APIManager_getInstance, {});
    uint32_t stagePtr = ld32(api1 + 0x24C);
    int32_t result = 0;
    if (stagePtr == 0) return result;
    uint32_t colosseum = ld32(api2 + 0x260);
    if (colosseum == 0) return result;
    call(g::ColosseumStage_create, {f.addr(0x20), ld32(stagePtr)});
    uint32_t row = f.word(0x20);
    int32_t first = bt::sbGet32(row, 0x1C);
    int32_t divisor = bt::sbGet32(row, 0x54);
    int32_t multiplier = bt::sbGet32(row, 0x58);
    uint32_t sum = 0;
    if (int32_t(ld32(colosseum + 0x28)) >= 2) {
        int32_t k = 0;
        do {
            call(g::Colosseum_create, {f.addr(0x18), uint32_t(first + k)});
            sum += uint32_t(bt::sbGet32(f.word(0x18), 0x10));
            bt::releaseShared(f.word(0x1C));
            k++;
        } while (k < int32_t(ld32(colosseum + 0x28) - 1));
    }
    uint32_t q = call(g::divsi3, {ld32(colosseum + 0x2C) - 1, uint32_t(divisor)});
    result = int32_t(uint32_t(multiplier) * q + sum);
    bt::releaseShared(f.word(0x24));
    return result;
}
RW_FUNCTION(0x759974, "StageManager::getColosseumRiseLevel", StageManager::getColosseumRiseLevel);

void StageManager::setNextMapInfo(StageManager *self) {
    Frame f(0xC80);
    uint32_t s = ga(self);
    int32_t mapId = self->nextMapId;
    int32_t stageId = self->stageId;
    clearMapInfo(self);
    // the map file
    const uint32_t str = f.addr(0x44), data = f.addr(0x50), sb = f.addr(0x864);
    uint32_t fu = call(g::FileUtils_getInstance, {});
    uint32_t getData = vtableSlot(fu, g::VSLOT_GET_DATA_FROM_FILE);
    call(g::StringBuffer_ctor, {sb, self->mapDataFile, uint32_t(stageId), uint32_t(mapId)});
    readDataFile(fu, getData, sb, str, data);
    freeLongString(str);
    self->map.mapId = mapId;
    uint32_t b = call(g::Data_getBytes, {data});
    const uint32_t name = s + 0x64;
    call(g::memcpy_, {name, b + 0x0C, 0x10});
    self->map.field_A8 = les16(b + 0x1C);
    call(g::Data_assign, {s + 0x74, data});
    self->map.enemyOffset = le32(b + 0x24) + 0x34;
    self->map.transferOffset = le32(b + 0x20) + 0x34;
    self->map.treasureOffset = le32(b + 0x28) + 0x34;
    self->map.materialOffset = le32(b + 0x2C) + 0x34;
    self->map.npcOffset = le32(b + 0x30) + 0x34;
    int32_t levelOffset = self->param.get32(0);
    createMapEnemyInfo(self, &self->map, levelOffset);
    createMapTransferInfo(self, &self->map);
    createMapTreasureInfo(self, &self->map);
    createMapMaterialInfo(self, &self->map);
    createMapNpcInfo(self, &self->map);
    createMapJumpInfo(self, &self->map);
    createMapFieldLayer(self, &self->map);
    // map/<name>/<name>_cls.bin
    {
        const uint32_t clsStr = f.addr(0x30), clsData = f.addr(0x3C), clsSb = f.addr(0x460);
        uint32_t fu2 = call(g::FileUtils_getInstance, {});
        uint32_t getData2 = vtableSlot(fu2, g::VSLOT_GET_DATA_FROM_FILE);
        call(g::StringBuffer_ctor, {clsSb, STR_CLS, name, name});
        readDataFile(fu2, getData2, clsSb, clsStr, clsData);
        call(g::Data_moveAssign, {s + 0xE4, clsData});
        call(g::Data_dtor, {clsData});
        freeLongString(clsStr);
    }
    // map/<name>/<name>_chp.bin
    {
        const uint32_t chpStr = f.addr(0x1C), chpData = f.addr(0x28), chpSb = f.addr(0x5C);
        uint32_t fu3 = call(g::FileUtils_getInstance, {});
        uint32_t getData3 = vtableSlot(fu3, g::VSLOT_GET_DATA_FROM_FILE);
        call(g::StringBuffer_ctor, {chpSb, STR_CHP, name, name});
        readDataFile(fu3, getData3, chpSb, chpStr, chpData);
        call(g::Data_moveAssign, {s + 0xEC, chpData});
        call(g::Data_dtor, {chpData});
        freeLongString(chpStr);
    }
    call(g::Data_dtor, {data});
}
RW_FUNCTION(0x759AB8, "StageManager::setNextMapInfo", StageManager::setNextMapInfo);

void StageManager::clearMapInfo(StageManager *self) {
    uint32_t s = ga(self);
    if (call(g::Data_isNull, {s + 0x74}) == 0) call(g::Data_clear, {s + 0x74});
    SMI *info = &self->map;
    deleteMapEnemyInfo(self, info);
    deleteMapTransferInfo(self, info);
    deleteMapTreasureInfo(self, info);
    deleteMapMaterialInfo(self, info);
    deleteMapNpcInfo(self, info);
    deleteMapJumpInfo(self, info);
    deleteMapFieldLayer(self, info);
    if (call(g::Data_isNull, {s + 0xE4}) == 0) call(g::Data_clear, {s + 0xE4});
    if (call(g::Data_isNull, {s + 0xEC}) == 0) call(g::Data_clear, {s + 0xEC});
}
RW_FUNCTION(0x758DCC, "StageManager::clearMapInfo", StageManager::clearMapInfo);

void StageManager::execTransitionMap(StageManager *self) {
    self->prevMapId = self->map.mapId.get();
    setNextMapInfo(self);
}
RW_FUNCTION(0x75C454, "StageManager::execTransitionMap", StageManager::execTransitionMap);

// Enemy section: count (4), symbol count (4 bytes, little-endian), `count` team records of 68 bytes, then the
// symbol records of 32 bytes. Teams: 68-byte MapEnemyTeamInfo (+0 symbol pointer, +0x1C direction, +0x1E flags,
// +0x20 four encounter-area corners, +0x40 corner count); symbols: 32 bytes (+8 level + offset, +0x18 position).
void StageManager::createMapEnemyInfo(StageManager *self, StageMapInfo *info, int32_t levelOffset) {
    Frame f(0x70);
    const uint32_t slot = f.addr(0x54);
    uint32_t i = ga(info);
    deleteMapEnemyInfo(self, info);
    uint32_t bytes = call(g::Data_getBytes, {i + 0x14});
    uint32_t off = info->enemyOffset;
    uint32_t p = bytes + off;
    int32_t teams = int32_t(le32(p));
    uint32_t b7 = ld8(p + 7), b6 = ld8(p + 6), b4 = ld8(p + 4), b5 = ld8(p + 5);
    call(g::reserve_EnemyTeam, {i + 0x30, uint32_t(teams)});
    uint32_t teamBytes = uint32_t(teams) * 0x44;
    uint32_t arr = call(g::new_array, {arraySize(teams, 0x44)});
    st32(arr, 0x44);
    st32(arr + 4, uint32_t(teams));
    uint32_t elems = arr + 8;
    int32_t symbols = int32_t((b5 << 8 | b4) | b6 << 16 | b7 << 24);
    if (teams != 0) {
        uint32_t end = elems + teamBytes;
        uint32_t r1 = arr + 0x20, e = elems;
        do {
            st32(e + 8, 0);
            uint32_t v = r1;
            for (int k = 0; k < 4; k++) v = call(g::Vec2_ctor, {v + 8});     // encounter area corners
            r1 += 0x44;
            e += 0x44;
        } while (e != end);
    }
    info->teamArray = elems;
    // the symbols
    uint32_t arr2 = call(g::new_array, {arraySize(symbols, 0x20)});
    st32(arr2, 0x20);
    st32(arr2 + 4, uint32_t(symbols));
    uint32_t elems2 = arr2 + 8;
    if (symbols != 0) {
        uint32_t total = uint32_t(symbols) << 5;
        for (uint32_t k = 0; k != total; k += 0x20) call(g::Vec2_ctor, {arr2 + k + 0x20});
        info->symbolArray = elems2;
        if (symbols >= 1) {
            uint32_t base = bytes + (off + teamBytes);
            uint32_t cur = elems2;
            int32_t left = symbols - 1;
            for (uint32_t r5 = 0;; r5 += 0x20) {
                uint32_t r4 = base + r5;
                uint32_t e = cur + r5;
                st32(e + 0x00, le32(r4 + 0x10));
                st32(e + 0x04, le32(r4 + 0x14));
                st32(e + 0x0C, le32(r4 + 0x18));
                st32(e + 0x08, le32(r4 + 0x1C) + uint32_t(levelOffset));
                st32(e + 0x10, le32(r4 + 0x20));
                st32(e + 0x14, le32(r4 + 0x24));
                stf(e + 0x18, lefloat(r4 + 0x08));
                stf(e + 0x1C, lefloat(r4 + 0x0C));
                if (left == 0) break;
                left--;
                cur = info->symbolArray;
            }
        }
    } else {
        info->symbolArray = elems2;
    }
    // the teams
    if (teams >= 1) {
        int32_t left = teams;
        uint32_t r7 = 0;
        do {
            uint32_t e = info->teamArray + r7;
            f.setWord(0x54, e);
            uint32_t r2 = p + r7;
            st32(e + 0x00, info->symbolArray + (le32(r2 + 0x08) << 5));
            st32(e + 0x04, le32(r2 + 0x0C));
            st32(e + 0x0C, le32(r2 + 0x14));
            st32(e + 0x08, le32(r2 + 0x18));
            st32(e + 0x10, le32(r2 + 0x1C));
            st32(e + 0x14, le32(r2 + 0x20));
            st32(e + 0x18, le32(r2 + 0x24));
            uint32_t dir = le32(r2 + 0x28);
            st16(e + 0x1C, direction16(dir));
            uint32_t fixing = (dir - 3 < 2) ? 1 : 0;
            st16(e + 0x1E, uint16_t((ld16(e + 0x1E) & 0xFFFE) | fixing));
            int32_t corners = setupEncountArea(rw::host_ptr<StageManager>(fixing), rw::host_ptr<bt::Vec2>(e + 0x20),
                                               rw::host_ptr<const uint8_t>(r2 + 0x2C));
            st32(e + 0x40, uint32_t(corners));
            if (hasRandomEnemySymbol(self, ld32(e + 0x10)) == 0)
                pushBack(i + 0x30, e, slot, g::push_EnemyTeam, false);
            r7 += 0x44;
        } while (--left != 0);
    }
}
RW_FUNCTION(0x759E38, "StageManager::createMapEnemyInfo", StageManager::createMapEnemyInfo);

// Transfer gates: count, then 36-byte records -> MapTransferInfo (36 bytes: +0 position, +8, +0xC, +0x10,
// +0x14 destination position, +0x1C destination map, +0x20 direction).
void StageManager::createMapTransferInfo(StageManager *self, StageMapInfo *info) {
    Frame f(0x48);
    const uint32_t slot = f.addr(0x2C);
    uint32_t i = ga(info);
    deleteMapTransferInfo(self, info);
    uint32_t p = call(g::Data_getBytes, {i + 0x14}) + info->transferOffset;
    int32_t n = int32_t(le32(p));
    call(g::reserve_Transfer, {i + 0x3C, uint32_t(n)});
    uint32_t arr = call(g::new_array, {arraySize(n, 0x24)});
    st32(arr, 0x24);
    st32(arr + 4, uint32_t(n));
    uint32_t elems = arr + 8;
    if (n == 0) {
        info->transferArray = elems;
        return;
    }
    uint32_t total = 0x24 * uint32_t(n);
    uint32_t k = 0;
    do {
        call(g::Vec2_ctor, {arr + k + 8});
        call(g::Vec2_ctor, {arr + k + 0x1C});
        k += 0x24;
    } while (total != k);
    info->transferArray = elems;
    if (n < 1) return;
    uint32_t cur = elems, off = 0;
    int32_t left = n;
    while (true) {
        uint32_t e = cur + off;
        f.setWord(0x2C, e);
        uint32_t r = p + off;
        stf(e + 0x00, lefloat(r + 0x04));
        stf(e + 0x04, lefloat(r + 0x08));
        st32(e + 0x08, le32(r + 0x0C));
        st32(e + 0x0C, le32(r + 0x10));
        st32(e + 0x10, le32(r + 0x14));
        stf(e + 0x14, lefloat(r + 0x18));
        stf(e + 0x18, lefloat(r + 0x1C));
        st32(e + 0x1C, le32(r + 0x20));
        st32(e + 0x20, directionFromTable(le32(r + 0x24)));
        pushBack(i + 0x3C, e, slot, g::push_Transfer, false);
        off += 0x24;
        if (--left == 0) break;
        cur = info->transferArray;
    }
}
RW_FUNCTION(0x75A334, "StageManager::createMapTransferInfo", StageManager::createMapTransferInfo);

// Treasures: count, then 20-byte records -> MapTreasure::Info (24 bytes: +0 position, +8, +0xC, +0x10).
void StageManager::createMapTreasureInfo(StageManager *self, StageMapInfo *info) {
    Frame f(0x38);
    const uint32_t slot = f.addr(0x1C);
    uint32_t i = ga(info);
    deleteMapTreasureInfo(self, info);
    uint32_t p = call(g::Data_getBytes, {i + 0x14}) + info->treasureOffset;
    int32_t n = int32_t(le32(p));
    call(g::reserve_Treasure, {i + 0x48, uint32_t(n)});
    uint32_t arr = call(g::new_array, {arraySize(n, 0x18)});
    st32(arr, 0x18);
    st32(arr + 4, uint32_t(n));
    uint32_t elems = arr + 8;
    if (n == 0) {
        info->treasureArray = elems;
        return;
    }
    uint32_t total = 0x18 * uint32_t(n);
    uint32_t k = 0;
    do {
        call(g::Vec2_ctor, {arr + k + 8});
        k += 0x18;
    } while (total != k);
    info->treasureArray = elems;
    if (n < 1) return;
    uint32_t cur = elems, off = 0, r = p + 4;
    int32_t left = n;
    while (true) {
        uint32_t e = cur + off;
        f.setWord(0x1C, e);
        stf(e + 0x00, lefloat(r + 0x00));
        stf(e + 0x04, lefloat(r + 0x04));
        st32(e + 0x08, le32(r + 0x0C));
        st32(e + 0x0C, le32(r + 0x08));
        st32(e + 0x10, le32(r + 0x10));
        pushBack(i + 0x48, e, slot, g::push_Treasure, false);
        r += 0x14;
        off += 0x18;
        if (--left == 0) break;
        cur = info->treasureArray;
    }
}
RW_FUNCTION(0x75A5E0, "StageManager::createMapTreasureInfo", StageManager::createMapTreasureInfo);

// Materials: count, then 16-byte records -> MapMaterial::Info (20 bytes: +0 position, +8, +0xC, +0x10 the
// unique id (mapId, index, kind 6) as createUniqueIdInStage builds it).
void StageManager::createMapMaterialInfo(StageManager *self, StageMapInfo *info) {
    Frame f(0x40);
    const uint32_t slot = f.addr(0x24);
    uint32_t i = ga(info);
    deleteMapMaterialInfo(self, info);
    uint32_t p = call(g::Data_getBytes, {i + 0x14}) + info->materialOffset;
    int32_t n = int32_t(le32(p));
    call(g::reserve_Material, {i + 0x54, uint32_t(n)});
    uint32_t arr = call(g::new_array, {arraySize(n, 0x14)});
    st32(arr, 0x14);
    st32(arr + 4, uint32_t(n));
    uint32_t elems = arr + 8;
    if (n == 0) {
        info->materialArray = elems;
        return;
    }
    uint32_t total = 0x14 * uint32_t(n);
    uint32_t k = 0;
    do {
        call(g::Vec2_ctor, {arr + k + 8});
        k += 0x14;
    } while (total != k);
    info->materialArray = elems;
    if (n < 1) return;
    uint32_t cur = elems, off = 0, unique = 0x400;
    int32_t idx = 0;
    while (true) {
        uint32_t e = cur + off;
        f.setWord(0x24, e);
        uint32_t r = p + (uint32_t(idx) << 4);
        stf(e + 0x00, lefloat(r + 0x04));
        stf(e + 0x04, lefloat(r + 0x08));
        st32(e + 0x08, le32(r + 0x0C));
        st32(e + 0x0C, le32(r + 0x10));
        st32(e + 0x10, ((uint32_t(info->mapId.get()) << 5) + 0x20) | unique | 6);
        pushBack(i + 0x54, e, slot, g::push_Material, false);
        unique += 0x400;
        off += 0x14;
        idx++;
        if (-n == -idx) break;
        cur = info->materialArray;
    }
}
RW_FUNCTION(0x75A808, "StageManager::createMapMaterialInfo", StageManager::createMapMaterialInfo);

// NPCs: count, then 32-byte records -> MapNonPlayableCharacter::Info (36 bytes: +0 position, +8, +0xC 16 bits,
// +0xE direction (16 bits), +0x10, +0x14, +0x18 unique id (kind 11), +0x1C, +0x20 16 bits, +0x22 flags).
void StageManager::createMapNpcInfo(StageManager *self, StageMapInfo *info) {
    Frame f(0x48);
    const uint32_t slot = f.addr(0x2C);
    uint32_t i = ga(info);
    deleteMapNpcInfo(self, info);
    uint32_t p = call(g::Data_getBytes, {i + 0x14}) + info->npcOffset;
    int32_t n = int32_t(le32(p));
    call(g::reserve_Npc, {i + 0x60, uint32_t(n)});
    uint32_t arr = call(g::new_array, {arraySize(n, 0x24)});
    st32(arr, 0x24);
    st32(arr + 4, uint32_t(n));
    uint32_t elems = arr + 8;
    if (n == 0) {
        info->npcArray = elems;
        return;
    }
    uint32_t total = 0x24 * uint32_t(n);
    uint32_t k = 0;
    do {
        call(g::Vec2_ctor, {arr + k + 8});
        k += 0x24;
    } while (total != k);
    info->npcArray = elems;
    if (n < 1) return;
    uint32_t cur = elems, off = 0, unique = 0x400;
    int32_t idx = 0;
    while (true) {
        uint32_t e = cur + off;
        f.setWord(0x2C, e);
        uint32_t r = p + (uint32_t(idx) << 5);
        stf(e + 0x00, lefloat(r + 0x04));
        stf(e + 0x04, lefloat(r + 0x08));
        st32(e + 0x08, le32(r + 0x0C));
        st16(e + 0x0C, uint16_t(le16(r + 0x10)));
        int32_t dir = les16(r + 0x14);
        st16(e + 0x0E, direction16(uint32_t(dir)));
        uint32_t fixing = (uint32_t(dir) - 3 < 2) ? 1 : 0;
        st16(e + 0x22, uint16_t((ld16(e + 0x22) & 0xFFFE) | fixing));
        st16(e + 0x20, uint16_t(le16(r + 0x16)));
        st32(e + 0x10, le32(r + 0x18));
        st32(e + 0x14, le32(r + 0x1C));
        st32(e + 0x1C, le32(r + 0x20));
        st32(e + 0x18, ((uint32_t(info->mapId.get()) << 5) + 0x20) | unique | 0xB);
        st16(e + 0x22, uint16_t(ld16(e + 0x22) & 0xFFFD));
        pushBack(i + 0x60, e, slot, g::push_Npc, false);
        unique += 0x400;
        off += 0x24;
        idx++;
        if (-n == -idx) break;
        cur = info->npcArray;
    }
}
RW_FUNCTION(0x75A9FC, "StageManager::createMapNpcInfo", StageManager::createMapNpcInfo);

// map/<name>/<name>_jmp.bin: a 16-byte header, then groups {key, count, count x 20-byte MapJumpInfo records}.
// MapJumpInfo (20 bytes, new): +0 from position, +8 to position, +0x10.
void StageManager::createMapJumpInfo(StageManager * /*self*/, StageMapInfo *info) {
    Frame f(0x480);
    const uint32_t pair = f.addr(0x38), str = f.addr(0x40), data = f.addr(0x4C), ret = f.addr(0x54),
                   sb = f.addr(0x5C);
    uint32_t i = ga(info);
    uint32_t fu = call(g::FileUtils_getInstance, {});
    uint32_t getData = vtableSlot(fu, g::VSLOT_GET_DATA_FROM_FILE);
    call(g::StringBuffer_ctor, {sb, STR_JMP, i + 4, i + 4});
    readDataFile(fu, getData, sb, str, data);
    freeLongString(str);
    int32_t size = int32_t(call(g::Data_getSize, {data}));
    uint32_t bytes = call(g::Data_getBytes, {data});
    if (size >= 0x11) {
        uint32_t off = 0x10;
        uint32_t next;
        do {
            uint32_t q = bytes + off;
            int32_t count = int32_t(le32(q + 4));
            uint32_t key = le32(q);
            uint32_t vec = call(g::operator_new, {0xC});
            next = off + 8;
            call(g::vec_MapJumpInfo_ctor, {vec, uint32_t(count)});
            if (count >= 1) {
                uint32_t r4 = 8;
                for (int32_t k = 0; k < count; k++) {
                    uint32_t o = call(g::operator_new, {0x14});
                    call(g::memset_, {o, 0x14, 0});
                    call(g::Vec2_ctor, {o});
                    call(g::Vec2_ctor, {o + 8});
                    uint32_t r = q + 5 * (4 * uint32_t(k));
                    stf(o + 0x00, lefloat(r + 0x08));
                    stf(o + 0x04, lefloat(r + 0x0C));
                    stf(o + 0x08, lefloat(r + 0x10));
                    stf(o + 0x0C, lefloat(r + 0x14));
                    st32(o + 0x10, le32(r + 0x18));
                    st32(ld32(vec) + 4 * uint32_t(k), o);
                    r4 += 0x14;
                }
                next = off + r4;
            }
            f.setWord(0x38, key);
            f.setWord(0x3C, vec);
            call(g::hash_Jump_insert, {ret, i + 0x94, pair});
            off = next;
        } while (int32_t(next) < size);
    }
    call(g::Data_clear, {data});
    call(g::Data_dtor, {data});
}
RW_FUNCTION(0x75ACA8, "StageManager::createMapJumpInfo", StageManager::createMapJumpInfo);

// map/<name>/<name>_info.bin: header (+8 16-bit "has base" word, +0xA layer count, +0xC png placement count,
// +0xE lwf placement count, +0x10 base name, +0x20 base size), then 60-byte layer records, 52-byte png
// placement records and 52-byte lwf placement records. Builds the MapFieldLayers (id -> layer, "base1" = 0)
// and the MapPlacementObjects.
//   MapFieldLayer::Info (stack): +0 Size, +8 Rect, +0x18 name, +0x1C name 2, +0x20 id, +0x24 bool
//   MapPlacementObject::Info (stack): +0 Vec2, +8 float, +0xC file name, +0x10 bool
void StageManager::createMapFieldLayer(StageManager *self, StageMapInfo *info) {
    Frame f(0xD48);
    uint32_t i = ga(info);
    deleteMapFieldLayer(self, info);
    const uint32_t str = f.addr(0x88), data = f.addr(0x94), sb = f.addr(0x92C);
    uint32_t fu = call(g::FileUtils_getInstance, {});
    uint32_t getData = vtableSlot(fu, g::VSLOT_GET_DATA_FROM_FILE);
    call(g::StringBuffer_ctor, {sb, STR_INFO, i + 4, i + 4});
    readDataFile(fu, getData, sb, str, data);
    freeLongString(str);
    uint32_t b = call(g::Data_getBytes, {data});
    uint8_t noBase = le16(b + 8) != 0 ? 0 : 1;
    uint32_t b10 = ld8(b + 0xA), b11 = ld8(b + 0xB), b12 = ld8(b + 0xC), b13 = ld8(b + 0xD), b14 = ld8(b + 0xE),
             b15 = ld8(b + 0xF);
    info->noBaseFlag = noBase;
    const uint32_t lwfCount = b15 << 8 | b14;
    const uint32_t pngCount = b13 << 8 | b12;
    const uint32_t layerCount = b11 << 8 | b10;
    const uint32_t layers = i + 0xB0;
    // the base layer (id 0)
    call(g::Size_ctor, {f.addr(0xEC)});
    call(g::Rect_ctor, {f.addr(0xF4)});
    float w = lefloat(b + 0x20);
    f.setF32(0xEC, w);
    float h = lefloat(b + 0x24);
    f.setF32(0xF0, h);
    f.setWord(0xF4, 0);
    f.setWord(0xF8, 0);
    f.setF32(0xFC, w);
    f.setF32(0x100, h);
    f.setWord(0x104, b + 0x10);
    f.setWord(0x108, STR_BASE1);
    f.setWord(0x10C, 0);
    uint32_t base = call(g::MapFieldLayer_create, {f.addr(0xEC)});
    call(g::Ref_retain, {base});
    f.setWord(0x80, f.word(0x10C));
    f.setWord(0x84, base);
    call(g::hash_FieldLayer_insert, {f.addr(0x114), layers, f.addr(0x80)});
    // the layers
    if (layerCount != 0) {
        uint32_t r4 = b + 0x63;
        int32_t k = 0;
        do {
            call(g::Size_ctor, {f.addr(0xC4)});
            call(g::Rect_ctor, {f.addr(0xCC)});
            f.setF32(0xC4, lefloat(r4 - 27));
            f.setF32(0xC8, lefloat(r4 - 23));
            f.setF32(0xCC, lefloat(r4 - 19));
            f.setF32(0xD0, lefloat(r4 - 15));
            f.setF32(0xD4, lefloat(r4 - 11));
            f.setF32(0xD8, lefloat(r4 - 7));
            f.setWord(0xDC, r4 - 0x3B);
            f.setWord(0xE0, r4 - 0x2B);
            f.setWord(0xE4, uint32_t(les16(r4 - 3)));
            f.setByte(0xE8, le16(r4 - 1) == 1 ? 1 : 0);
            uint32_t layer = call(g::MapFieldLayer_create, {f.addr(0xC4)});
            call(g::Ref_retain, {layer});
            f.setWord(0x78, f.word(0xE4));
            f.setWord(0x7C, layer);
            call(g::hash_FieldLayer_insert, {f.addr(0x11C), layers, f.addr(0x78)});
            if (ld8(f.addr(0xE8)) != 0) {
                f.setWord(0x74, layer);
                pushBack(i + 0xDC, layer, f.addr(0x74), g::push_MapNode, true);
            }
            call(g::Vec2_dtor, {f.addr(0xCC)});
            r4 += 0x3C;
            k++;
        } while (k < int32_t(layerCount));
    }
    // png placements, then lwf placements: identical except for the file name format and the stack slots
    struct Kind {
        uint32_t count, info, name, nameFmtArgs, slotObj, slotKey, slotNode;
    };
    const Kind kinds[2] = {{pngCount, 0xB0, 0x528, 3, 0x70, 0x6C, 0x68}, {lwfCount, 0x9C, 0x124, 2, 0x64, 0x60, 0x5C}};
    uint32_t r5 = 0x3C * layerCount + b + 0x59;
    for (int kind = 0; kind < 2; kind++) {
        const Kind &kd = kinds[kind];
        if (kind == 1) r5 = (0x34 * pngCount + 0x3C * layerCount) + b + 0x59;
        if (kd.count == 0) continue;
        int32_t k = 0;
        do {
            uint32_t nm = r5 - 0x31;
            call(g::Vec2_ctor, {f.addr(kd.info)});
            f.setF32(kd.info + 0x0, lefloat(r5 - 17));
            f.setF32(kd.info + 0x4, lefloat(r5 - 13));
            f.setF32(kd.info + 0x8, lefloat(r5 - 9));
            if (kd.nameFmtArgs == 3) {
                f.setWord(0, r5 - 0x21);
                call(g::StringBuffer_ctor, {f.addr(kd.name), STR_PNG, nm, nm, r5 - 0x21});
            } else {
                call(g::StringBuffer_ctor, {f.addr(kd.name), STR_LWF, nm, nm});
            }
            f.setWord(kd.info + 0xC, f.addr(kd.name));
            f.setByte(kd.info + 0x10, le16(r5 - 1) == 1 ? 1 : 0);
            uint32_t obj = call(g::MapPlacementObject_create, {f.addr(kd.info)});
            f.setWord(kd.slotObj, obj);
            call(g::Ref_retain, {obj});
            pushBack(i + 0xC4, obj, f.addr(kd.slotObj), g::push_MapPlacementObject, false);
            int32_t layerId = les16(r5 - 5);
            f.setWord(kd.slotKey, uint32_t(layerId));
            uint32_t slotRaw = le16(r5 - 3);
            int32_t slotIndex = int16_t(uint16_t(slotRaw));
            if (call(g::hash_FieldLayer_find, {layers, f.addr(kd.slotKey)}) != 0) {
                if ((uint32_t(layerId) | slotRaw) != 0) {
                    uint32_t valuePtr = call(g::hash_FieldLayer_at, {layers, f.addr(kd.slotKey)});
                    call(g::MapFieldLayer_addLayerMapNode, {ld32(valuePtr), obj, uint32_t(slotIndex)});
                } else {
                    pushBack(i + 0xD0, obj, f.addr(kd.slotObj), g::push_MapPlacementObject, false);
                }
            }
            if (ld8(f.addr(kd.info + 0x10)) != 0) {
                f.setWord(kd.slotNode, obj);
                pushBack(i + 0xDC, obj, f.addr(kd.slotNode), g::push_MapNode, true);
            }
            call(g::Vec2_dtor, {f.addr(kd.info)});
            r5 += 0x34;
            k++;
        } while (k < int32_t(kd.count));
    }
    call(g::Data_clear, {data});
    call(g::Vec2_dtor, {f.addr(0xF4)});
    call(g::Data_dtor, {data});
}
RW_FUNCTION(0x75AF20, "StageManager::createMapFieldLayer", StageManager::createMapFieldLayer);

// ---------------------------------------------------------------------------------------------- deletes
void StageManager::deleteMapEnemyInfo(StageManager * /*self*/, StageMapInfo *info) {
    uint32_t i = ga(info);
    vectorClear(i + 0x30);
    uint32_t teams = info->teamArray;
    if (teams) {
        uint32_t n = ld32(teams - 4);
        if (n) {
            uint32_t e = teams + 0x44 * n;
            do {
                for (int32_t k = -12; k >= -36; k -= 8) call(g::Vec2_dtor, {e + uint32_t(k)});
                e -= 0x44;
            } while (e != teams);
        }
        call(g::delete_array, {teams - 8});
        info->teamArray = 0;
    }
    uint32_t symbols = info->symbolArray;
    if (symbols) {
        uint32_t n = ld32(symbols - 4);
        uint32_t arr = symbols - 8;
        if (n) {
            uint32_t k = n << 5;
            do {
                call(g::Vec2_dtor, {arr + k});
                k -= 0x20;
            } while (k != 0);
        }
        call(g::delete_array, {arr});
        info->symbolArray = 0;
    }
}
RW_FUNCTION(0x75B7C8, "StageManager::deleteMapEnemyInfo", StageManager::deleteMapEnemyInfo);

void StageManager::deleteMapTransferInfo(StageManager * /*self*/, StageMapInfo *info) {
    uint32_t i = ga(info);
    vectorClear(i + 0x3C);
    uint32_t a = info->transferArray;
    if (a) {
        uint32_t n = ld32(a - 4);
        uint32_t arr = a - 8;
        if (n) {
            uint32_t k = uint32_t(-36) * n;
            uint32_t v = a + 0x24 * n - 0x10;
            do {
                call(g::Vec2_dtor, {v});
                call(g::Vec2_dtor, {v - 0x14});
                k += 0x24;
                v -= 0x24;
            } while (k != 0);
        }
        call(g::delete_array, {arr});
        info->transferArray = 0;
    }
}
RW_FUNCTION(0x75B860, "StageManager::deleteMapTransferInfo", StageManager::deleteMapTransferInfo);

namespace {
// The single-Vec2 arrays (treasures, materials, NPCs): destroy each element's Vec2 from the last, delete[].
void deleteVec2Array(rw::Field<uint32_t> &array, uint32_t elem) {
    uint32_t a = array;
    if (!a) return;
    uint32_t n = ld32(a - 4);
    uint32_t arr = a - 8;
    if (n) {
        uint32_t k = elem * n;
        uint32_t base = a - elem;
        do {
            call(g::Vec2_dtor, {base + k});
            k -= elem;
        } while (k != 0);
    }
    call(g::delete_array, {arr});
    array = 0;
}
}  // namespace

void StageManager::deleteMapTreasureInfo(StageManager * /*self*/, StageMapInfo *info) {
    vectorClear(ga(info) + 0x48);
    deleteVec2Array(info->treasureArray, 0x18);
}
RW_FUNCTION(0x75B8CC, "StageManager::deleteMapTreasureInfo", StageManager::deleteMapTreasureInfo);

void StageManager::deleteMapMaterialInfo(StageManager * /*self*/, StageMapInfo *info) {
    vectorClear(ga(info) + 0x54);
    deleteVec2Array(info->materialArray, 0x14);
}
RW_FUNCTION(0x75B922, "StageManager::deleteMapMaterialInfo", StageManager::deleteMapMaterialInfo);

void StageManager::deleteMapNpcInfo(StageManager * /*self*/, StageMapInfo *info) {
    vectorClear(ga(info) + 0x60);
    deleteVec2Array(info->npcArray, 0x24);
}
RW_FUNCTION(0x75B978, "StageManager::deleteMapNpcInfo", StageManager::deleteMapNpcInfo);

void StageManager::deleteMapJumpInfo(StageManager * /*self*/, StageMapInfo *info) {
    uint32_t i = ga(info);
    for (uint32_t node = info->jumps.first; node; node = ld32(node)) {
        uint32_t vec = ld32(node + 0xC);
        uint32_t it = ld32(vec), end = ld32(vec + 4);
        if (it != end) {
            do {
                uint32_t o = ld32(it);
                if (o) {
                    call(g::Vec2_dtor, {o + 8});
                    call(g::Vec2_dtor, {o});
                    call(g::operator_delete, {o});
                }
                it += 4;
            } while (end != it);
            vectorClear(vec);
        }
        uint32_t r = call(g::vecbase_MapJumpInfo_dtor, {vec});
        call(g::operator_delete, {r});
    }
    call(g::hash_Jump_clear, {i + 0x94});
}
RW_FUNCTION(0x75B9D2, "StageManager::deleteMapJumpInfo", StageManager::deleteMapJumpInfo);

void StageManager::deleteMapFieldLayer(StageManager * /*self*/, StageMapInfo *info) {
    uint32_t i = ga(info);
    for (uint32_t node = info->fieldLayers.first; node; node = ld32(node)) {
        uint32_t layer = ld32(node + 0xC);
        if (layer) call(g::Ref_release, {layer});
    }
    call(g::hash_FieldLayer_clear, {i + 0xB0});
    uint32_t end = ld32(i + 0xC8), it = ld32(i + 0xC4);
    if (it != end) {
        do {
            uint32_t o = ld32(it);
            if (o) call(g::Ref_release, {o});
            it += 4;
        } while (end != it);
        vectorClear(i + 0xC4);
    }
    vectorClear(i + 0xD0);
    vectorClear(i + 0xDC);
}
RW_FUNCTION(0x75BA46, "StageManager::deleteMapFieldLayer", StageManager::deleteMapFieldLayer);

// The encounter area: up to 4 corners (8 bytes each, unsigned x / y); duplicates are dropped. Returns the
// number of distinct corners written to area[] (the rest stays 0).
int32_t StageManager::setupEncountArea(StageManager * /*self*/, bt::Vec2 *area, const uint8_t *bytes) {
    uint32_t a = ga(area), b = ga(bytes);
    uint32_t pts[4][2];
    for (uint32_t k = 0; k != 32; k += 8) {
        pts[k / 8][0] = le32(b + k);
        pts[k / 8][1] = le32(b + k + 4);
        st32(a + k, 0);
        st32(a + k + 4, 0);
    }
    int32_t count = 0;
    for (int32_t k = 0; k != 4; k++) {
        bool dup = false;
        for (int32_t j = k - 1; j >= 0; j--)
            if (pts[k][0] == pts[j][0] && pts[k][1] == pts[j][1]) dup = true;
        if (dup) continue;
        uint32_t dst = a + 8 * uint32_t(count);
        stf(dst, float(pts[k][0]));                       // __floatunsisf
        stf(dst + 4, float(pts[k][1]));
        count++;
    }
    return count;
}
RW_FUNCTION(0x75BCD0, "StageManager::setupEncountArea", StageManager::setupEncountArea);

// Story quests: is this symbol id one of the server's random enemy symbols (APIManager list at +0x13C)?
uint32_t StageManager::hasRandomEnemySymbol(StageManager *self, uint32_t symbolId) {
    if (self->mode != 0) return 0;
    uint32_t list = call(g::APIManager_getInstance, {}) + 0x13C;
    uint32_t sb = ld32(list);
    if (!sb) return 0;
    uint32_t n = ld32(sb + 0x20);
    if (!n) return 0;
    uint32_t index = 0;
    for (uint32_t k = 0;;) {
        int32_t v = bt::sbGet32(sb, index);
        k++;
        if (uint32_t(v) == symbolId) return 1;
        index += 4;
        if (k >= n) return 0;
        sb = ld32(list);
    }
}
RW_FUNCTION(0x75BDF4, "StageManager::hasRandomEnemySymbol", StageManager::hasRandomEnemySymbol);

// ============================================================================================ file helpers
int32_t StageManager::getInteger16bitFromLittleEndianBytes(const uint8_t *bytes) { return les16(ga(bytes)); }
RW_FUNCTION(0x75991C, "StageManager::getInteger16bitFromLittleEndianBytes",
            StageManager::getInteger16bitFromLittleEndianBytes);

int32_t StageManager::getInteger32bitFromLittleEndianBytes(const uint8_t *bytes) { return int32_t(le32(ga(bytes))); }
RW_FUNCTION(0x75992A, "StageManager::getInteger32bitFromLittleEndianBytes",
            StageManager::getInteger32bitFromLittleEndianBytes);

int32_t StageManager::convertDirectionID(StageManager * /*self*/, int32_t direction) {
    return int16_t(direction16(uint32_t(direction)));
}
RW_FUNCTION(0x759940, "StageManager::convertDirectionID", StageManager::convertDirectionID);

uint32_t StageManager::convertIsFixing(StageManager * /*self*/, int32_t direction) {
    return uint32_t(direction) - 3 < 2 ? 1 : 0;
}
RW_FUNCTION(0x75BCC0, "StageManager::convertIsFixing", StageManager::convertIsFixing);

int32_t StageManager::createUniqueIdInStage(StageManager * /*self*/, int32_t mapId, int32_t index, int32_t kind) {
    uint32_t v = ((uint32_t(index) << 10) + 0x400) | ((uint32_t(mapId) << 5) + 0x20);
    return int32_t((uint32_t(kind) + 1) | v);
}
RW_FUNCTION(0x75C0D4, "StageManager::createUniqueIdInStage", StageManager::createUniqueIdInStage);

// ============================================================================================ queries
// World of the current map from the first two letters of its name.
int32_t StageManager::getWorldCode(StageManager *self) {
    uint32_t code = le16(ga(self) + 0x64);
    if (code == 0x4741) return 4;   // "AG"
    if (code == 0x4244) return 1;   // "DB"
    if (code == 0x5744) return 2;   // "DW"
    if (code == 0x434F) return 5;   // "OC"
    if (code == 0x4C57) return 3;   // "WL"
    if (code == 0x4342) return 6;   // "BC"
    return 1;
}
RW_FUNCTION(0x75BAF4, "StageManager::getWorldCode", StageManager::getWorldCode);

uint32_t StageManager::getMapID(StageManager *self) { return ga(self) + 0x64; }
RW_FUNCTION(0x75BBD0, "StageManager::getMapID", StageManager::getMapID);

int32_t StageManager::getMapTotalNumber(StageManager *self) { return self->mapTotalNumber; }
RW_FUNCTION(0x75BBD4, "StageManager::getMapTotalNumber", StageManager::getMapTotalNumber);

uint32_t StageManager::getMapStartInfo(StageManager *self) { return ga(self) + 0x14C; }
RW_FUNCTION(0x75BBD8, "StageManager::getMapStartInfo", StageManager::getMapStartInfo);

int32_t StageManager::getStageBossEnemyID(StageManager *self) { return self->bossEnemyId; }
RW_FUNCTION(0x75BBE0, "StageManager::getStageBossEnemyID", StageManager::getStageBossEnemyID);

int32_t StageManager::getStageEnemyPlatoonCount(StageManager *self) { return self->enemyPlatoonCount; }
RW_FUNCTION(0x75BBE4, "StageManager::getStageEnemyPlatoonCount", StageManager::getStageEnemyPlatoonCount);

uint32_t StageManager::getAvatarActorStylePlayable(StageManager * /*self*/, uint32_t style) { return style; }
RW_FUNCTION(0x75BBE8, "StageManager::getAvatarActorStylePlayable", StageManager::getAvatarActorStylePlayable);

uint32_t StageManager::getAvatarActorStyleFriend(StageManager *self, uint32_t style) {
    return self->mode != 0 ? 0 : style;
}
RW_FUNCTION(0x75BBEE, "StageManager::getAvatarActorStyleFriend", StageManager::getAvatarActorStyleFriend);

uint32_t StageManager::getEnemyPlatoonInfo(StageManager *self) { return ga(self) + 0x90; }
RW_FUNCTION(0x75C228, "StageManager::getEnemyPlatoonInfo", StageManager::getEnemyPlatoonInfo);

uint32_t StageManager::getEnemyTeamInfo(StageManager *self, int32_t index) {
    if (index < 0) return 0;
    uint32_t begin = self->map.enemyTeams.begin;
    uint32_t count = uint32_t(int32_t(self->map.enemyTeams.end.get() - begin) >> 2);
    if (count <= uint32_t(index)) return 0;
    return ld32(begin + 4 * uint32_t(index));
}
RW_FUNCTION(0x75C22C, "StageManager::getEnemyTeamInfo", StageManager::getEnemyTeamInfo);

int32_t StageManager::getEnemyPlatoonCount(StageManager *self) {
    return int32_t(self->map.enemyTeams.end.get() - self->map.enemyTeams.begin.get()) >> 2;
}
RW_FUNCTION(0x75C250, "StageManager::getEnemyPlatoonCount", StageManager::getEnemyPlatoonCount);

// Is pos inside team `index`'s encounter area (one triangle for 3 corners, two for 4)? An edge test per
// triangle edge: cross(edge, pos - corner) >= 0.
uint32_t StageManager::isInsideTeamEncountArea(StageManager *self, uint32_t pos, int32_t index) {
    Frame f(0x60);
    if (index < 0) return 0;
    uint32_t begin = self->map.enemyTeams.begin;
    uint32_t count = uint32_t(int32_t(self->map.enemyTeams.end.get() - begin) >> 2);
    if (count <= uint32_t(index)) return 0;
    uint32_t team = ld32(begin + 4 * uint32_t(index));
    if (!team) return 0;
    uint32_t corners = ld32(team + 0x40);
    if (corners < 3) return 0;
    int32_t triangles = corners > 3 ? 2 : 1;
    uint32_t edges = TRIANGLE_EDGES + 4;
    uint32_t any = 0;
    for (int32_t t = 0; t < triangles; t++) {
        uint32_t inside = 1;
        uint32_t e = edges;
        for (int32_t k = 0; k < 3; k++) {
            uint32_t from = team + 0x20 + (ld32(e - 4) << 3);
            uint32_t to = team + 0x20 + (ld32(e) << 3);
            call(g::Vec2_ctor2, {f.addr(0x40), from, to});
            call(g::Vec2_ctor2, {f.addr(0x38), from, pos});
            float ey = f.f32(0x44);
            float dy = f.f32(0x3C);
            float dx = f.f32(0x38);
            float ex = f.f32(0x40);
            call(g::Vec2_dtor, {f.addr(0x38)});
            call(g::Vec2_dtor, {f.addr(0x40)});
            float m1 = ex * dy;
            float m2 = ey * dx;
            float cross = m1 - m2;
            inside &= (cross >= 0.0f) ? 1u : 0u;
            e += 8;
        }
        any = (any & 0xFF) | inside;
        edges += 0x18;
    }
    return any != 0 ? 1 : 0;
}
RW_FUNCTION(0x75C260, "StageManager::isInsideTeamEncountArea", StageManager::isInsideTeamEncountArea);

// The first transfer gate within 96 of pos: its destination becomes the next map start; returns 1.
uint32_t StageManager::checkMapTransportGate(StageManager *self, uint32_t pos) {
    uint32_t end = self->map.transfers.end, it = self->map.transfers.begin;
    if (it == end) return 0;
    while (true) {
        uint32_t gate = ld32(it);
        float d = bitsToFloat(call(g::Vec2_distance, {pos, gate}));
        it += 4;
        if (d < 96.0f) {
            uint32_t x = ld32(gate + 0x14), y = ld32(gate + 0x18), map = ld32(gate + 0x1C), dir = ld32(gate + 0x20);
            rw::set<uint32_t>(self, 0x158, dir);
            rw::set<uint32_t>(self, 0x154, map);
            rw::set<uint32_t>(self, 0x150, y);
            rw::set<uint32_t>(self, 0x14C, x);
            return 1;
        }
        if (end == it) return 0;
    }
}
RW_FUNCTION(0x75C3D0, "StageManager::checkMapTransportGate", StageManager::checkMapTransportGate);

// The transfer gate with the smallest distance(pos) + gate[+0xC]; 0 when there is none.
uint32_t StageManager::MarkingMapTransportGate(StageManager *self, uint32_t pos) {
    uint32_t end = self->map.transfers.end, it = self->map.transfers.begin;
    float best = bitsToFloat(0x501502F9);                 // 1e10
    uint32_t prev = 0, chosen = 0;
    if (it == end) return 0;
    do {
        chosen = ld32(it);
        float d = bitsToFloat(call(g::Vec2_distance, {pos, chosen})) + float(ld32(chosen + 0xC));
        float next = d < best ? d : best;
        if (!(d < best)) chosen = prev;
        it += 4;
        best = next;
        prev = chosen;
    } while (end != it);
    return chosen;
}
RW_FUNCTION(0x75C464, "StageManager::MarkingMapTransportGate", StageManager::MarkingMapTransportGate);

// Fade the mode nodes whose mode (virtual slot 45) equals `mode`: mode 1 fades them out and hides them, any
// other mode shows them and fades them in.
void StageManager::changeMode(StageManager *self, uint32_t mode) {
    uint32_t end = self->map.modeNodes.end;
    for (uint32_t it = self->map.modeNodes.begin; end != it; it += 4) {
        uint32_t node = ld32(it);
        if (rw::vcall(rw::host_ptr<void>(node), 45, {node}) != mode) continue;
        uint32_t first, second;
        if (mode == 1) {
            first = call(g::FadeOut_create, {FLOAT_ONE});
            second = call(g::Hide_create, {});
        } else {
            first = call(g::Show_create, {});
            second = call(g::FadeIn_create, {FLOAT_ONE});
        }
        uint32_t seq = call(g::Sequence_create, {first, second, 0});
        call(g::Node_runAction, {node, seq});
    }
}
RW_FUNCTION(0x75C4EC, "StageManager::changeMode", StageManager::changeMode);

// ============================================================================================ sub missions
// Stage row (master::Stage): +0x1E0 sub mission count, +0x1E4 int[3] kinds, +0x38C int[3] parameters,
// +0x398 int[3] target values. Kinds 4/12/14/15/19 keep a counter in the StageState that is set up here.
void StageManager::setupSubMission(StageManager *self) {
    Frame f(0x48);
    call(g::Stage_create, {f.addr(0x0C), uint32_t(self->stageId.get())});
    SafeBuffer *row = rw::host_ptr<SafeBuffer>(f.word(0x0C));
    int32_t count = row->get32(0x1E0);
    SafeBuffer::read(row, f.host(0x24), row->data.get() + 0x1E4, 0xC, 0x1E4);
    SafeBuffer::read(row, f.host(0x18), row->data.get() + 0x38C, 0xC, 0x38C);
    for (int32_t k = 0; k < count; k++) {
        int32_t kind = int32_t(f.word(0x24 + 4 * uint32_t(k)));
        uint32_t param = f.word(0x18 + 4 * uint32_t(k));
        uint32_t fn = 0;
        if (kind > 11) {
            switch (uint32_t(kind - 12)) {
            case 0: fn = g::setSubMission12; break;
            case 2: fn = g::setSubMission14; break;
            case 3: fn = g::setSubMission15; break;
            case 7: fn = g::setSubMission19; break;
            default: break;
            }
        } else if (kind == 4) {
            fn = g::setSubMission04;
        }
        if (fn) call(fn, {stageState(), uint32_t(k), param});
    }
    bt::releaseShared(f.word(0x10));
}
RW_FUNCTION(0x7592F8, "StageManager::setupSubMission", StageManager::setupSubMission);

void StageManager::finish(StageManager *self, uint32_t deck) {
    if (self->mode == 0) decisionSubMission(self, deck);
}
RW_FUNCTION(0x75945C, "StageManager::finish", StageManager::finish);

// End of quest: test every sub mission of the stage and record the cleared ones (StageState::addClearMission
// with the 1-based mission number). v = target value, p = parameter.
void StageManager::decisionSubMission(StageManager *self, uint32_t deck) {
    Frame f(0x100);
    call(g::Stage_create, {f.addr(0x24), uint32_t(self->stageId.get())});
    SafeBuffer *row = rw::host_ptr<SafeBuffer>(f.word(0x24));
    int32_t count = row->get32(0x1E0);
    SafeBuffer::read(row, f.host(0x7C), row->data.get() + 0x1E4, 0xC, 0x1E4);   // kinds
    SafeBuffer::read(row, f.host(0x70), row->data.get() + 0x38C, 0xC, 0x38C);   // parameters
    SafeBuffer::read(row, f.host(0x64), row->data.get() + 0x398, 0xC, 0x398);   // target values
    for (int32_t k = 0; k < count; k++) {
        uint32_t kind = f.word(0x7C + 4 * uint32_t(k));
        uint32_t v = f.word(0x64 + 4 * uint32_t(k));
        uint32_t p = f.word(0x70 + 4 * uint32_t(k));
        bool hit = false;
        switch (kind) {
        case 2: {   // field_30 - field_34 == StageState[0x20]
            uint32_t a = uint32_t(self->field_30.get()) - uint32_t(self->field_34.get());
            hit = a == uint32_t(stageStateValue(0x20));
            break;
        }
        case 3: {   // v <= StageState[0x20] + [0x24] + [0x28]
            uint32_t x = uint32_t(stageStateValue(0x20));
            uint32_t y = uint32_t(stageStateValue(0x24));
            uint32_t z = uint32_t(stageStateValue(0x28));
            hit = v <= (y + x) + z;
            break;
        }
        case 4: hit = int32_t(v) <= int32_t(call(g::getSubMission04, {stageState(), uint32_t(k)})); break;
        case 5: hit = v <= uint32_t(stageStateValue(0x4258)); break;
        case 6: hit = v >= uint32_t(stageStateValue(0x4258)); break;
        case 7: hit = v >= uint32_t(stageStateValue(0x4254)); break;
        case 8: hit = v >= uint32_t(stageStateValue(0x4260)); break;
        case 9: hit = v >= uint32_t(stageStateValue(0x4268)); break;
        case 10: hit = stageStateValue(0x4264) == 0; break;
        case 11: hit = isNoBadStatus(self) == 1; break;
        case 12: {
            int32_t c = int32_t(call(g::getSubMission12, {stageState(), uint32_t(k)}));
            if (int32_t(v) < c) break;
            hit = int32_t(call(g::getSubMission12, {stageState(), uint32_t(k)})) > 0;
            break;
        }
        case 13: hit = v <= uint32_t(stageStateValue(0x2C)); break;
        case 14: hit = int32_t(call(g::getSubMission14, {stageState(), uint32_t(k)})) > 0; break;
        case 15: hit = int32_t(v) <= int32_t(call(g::getSubMission15, {stageState(), uint32_t(k)})); break;
        case 16: {  // bad status of the parameter's kind ('e' 0, 'o' 1, 'y' 2, 'g' 3, else 4) was inflicted
            uint32_t ss = stageState();
            uint32_t type;
            if (int32_t(p) > 0x78) type = p == 0x79 ? 2 : 4;
            else if (p == 0x65) type = 0;
            else if (p == 0x67) type = 3;
            else if (p == 0x6F) type = 1;
            else type = 4;
            hit = call(g::getBadStatusCount, {ss, type}) != 0;
            break;
        }
        case 17: {
            int32_t want = self->treasureCount;
            hit = uint32_t(want) == call(g::getHasTreasureCount, {stageState()});
            break;
        }
        case 18: {
            int32_t want = self->materialCount;
            hit = uint32_t(want) == call(g::getHasMaterialCount, {stageState()});
            break;
        }
        case 19: hit = int32_t(v) <= int32_t(call(g::getSubMission19, {stageState(), uint32_t(k)})); break;
        case 20: hit = call(g::getHasTreasureCount, {stageState()}) == 0; break;
        case 21: hit = countOnDeckMedalAttribute(self, deck, 0, 1, 1) == 0; break;
        case 22: hit = countOnDeckMedalAttribute(self, deck, 1, 0, 1) == 0; break;
        case 23: hit = countOnDeckMedalAttribute(self, deck, 1, 1, 0) == 0; break;
        case 24: hit = int32_t(ld32(deck + 0x360)) <= int32_t(v); break;
        case 25: {  // the equipped keyblade's kind (id / 1000) is the parameter's
            call(g::getCurrentKeyblade, {f.addr(0x88)});
            int32_t mine = int32_t(f.word(0x94)) / 1000;
            hit = uint32_t(mine) == p / 1000u;
            break;
        }
        case 26: hit = getMostHigherRarity(self, deck) <= int32_t(v); break;
        case 28: hit = stageStateValue(0x45BC) == 0; break;
        case 29: hit = v <= uint32_t(stageStateValue(0x10)); break;
        case 30: hit = isFullSetTargetRarity(self, deck, int32_t(v)) == 1; break;
        default: break;
        }
        if (hit) call(g::addClearMission, {stageState(), uint32_t(k) + 1});
    }
    bt::releaseShared(f.word(0x28));
}
RW_FUNCTION(0x75946C, "StageManager::decisionSubMission", StageManager::decisionSubMission);

uint32_t StageManager::isNoBadStatus(StageManager * /*self*/) {
    uint32_t sum = call(g::getBadStatusCount, {stageState(), 0});
    sum += call(g::getBadStatusCount, {stageState(), 1});
    sum += call(g::getBadStatusCount, {stageState(), 2});
    uint32_t last = call(g::getBadStatusCount, {stageState(), 3});
    return sum + last == 0 ? 1 : 0;
}
RW_FUNCTION(0x75C570, "StageManager::isNoBadStatus", StageManager::isNoBadStatus);

// Count the deck's medals per attribute (forEachMedal with a lambda capturing the counters) and sum the
// selected ones.
int32_t StageManager::countOnDeckMedalAttribute(StageManager * /*self*/, uint32_t deck, uint32_t attr1,
                                                uint32_t attr2, uint32_t attr3) {
    Frame f(0x50);
    f.setWord(0x0C, 0);
    f.setWord(0x10, 0);
    f.setWord(0x14, 0);
    if (deck) {
        f.setWord(0x08, ld32(deck + 0x360));
        f.setWord(0x28, 0);
        uint32_t fn = call(g::operator_new, {0x14});
        if (fn) {
            st32(fn + 0x00, FUNCTOR_COUNT_ATTRIBUTE);
            st32(fn + 0x04, f.addr(0x08));
            st32(fn + 0x08, f.addr(0x14));
            st32(fn + 0x0C, f.addr(0x10));
            st32(fn + 0x10, f.addr(0x0C));
        }
        f.setWord(0x28, fn);
        call(g::forEachMedal, {deck, f.addr(0x18)});
        destroyFunction(f, 0x18, 0x28);
    }
    uint32_t r4 = attr1 != 0 ? f.word(0x14) : 0;
    uint32_t r1 = attr2 != 0 ? r4 + f.word(0x10) : r4;
    uint32_t r0 = attr3 != 0 ? r1 + f.word(0x0C) : r1;
    return int32_t(r0);
}
RW_FUNCTION(0x75C5C0, "StageManager::countOnDeckMedalAttribute", StageManager::countOnDeckMedalAttribute);

int32_t StageManager::getOnDeckMedalCount(StageManager * /*self*/, uint32_t deck) { return int32_t(ld32(deck + 0x360)); }
RW_FUNCTION(0x75C6B4, "StageManager::getOnDeckMedalCount", StageManager::getOnDeckMedalCount);

uint32_t StageManager::isEqualKeyBleadKind(StageManager * /*self*/, uint32_t keybladeId) {
    Frame f(0x80);
    call(g::getCurrentKeyblade, {f.addr(0x08)});
    int32_t mine = int32_t(f.word(0x14)) / 1000;
    return uint32_t(mine) == keybladeId / 1000u ? 1 : 0;
}
RW_FUNCTION(0x75C6BC, "StageManager::isEqualKeyBleadKind", StageManager::isEqualKeyBleadKind);

int32_t StageManager::getMostHigherRarity(StageManager * /*self*/, uint32_t deck) {
    Frame f(0x40);
    f.setWord(0x0C, 0);
    if (deck) {
        f.setWord(0x08, ld32(deck + 0x360));
        f.setWord(0x20, f.addr(0x10));
        f.setWord(0x10, FUNCTOR_HIGHER_RARITY);
        st32((f.addr(0x10) | 4) + 4, f.addr(0x0C));      // as the original computes the capture address
        f.setWord(0x14, f.addr(0x08));
        call(g::forEachMedal, {deck, f.addr(0x10)});
        destroyFunction(f, 0x10, 0x20);
    }
    return int32_t(f.word(0x0C));
}
RW_FUNCTION(0x75C730, "StageManager::getMostHigherRarity", StageManager::getMostHigherRarity);

uint32_t StageManager::isFullSetTargetRarity(StageManager * /*self*/, uint32_t deck, int32_t rarity) {
    Frame f(0x40);
    f.setWord(0x08, 0);
    f.setWord(0x0C, uint32_t(rarity));
    if (deck) {
        f.setWord(0x04, ld32(deck + 0x360));
        f.setWord(0x20, f.addr(0x10));
        f.setWord(0x10, FUNCTOR_FULL_SET);
        f.setWord(0x14, f.addr(0x04));
        f.setWord(0x18, f.addr(0x0C));
        f.setWord(0x1C, f.addr(0x08));
        call(g::forEachMedal, {deck, f.addr(0x10)});
        destroyFunction(f, 0x10, 0x20);
    }
    return int32_t(f.word(0x08)) > 4 ? 1 : 0;
}
RW_FUNCTION(0x75C7EC, "StageManager::isFullSetTargetRarity", StageManager::isFullSetTargetRarity);
