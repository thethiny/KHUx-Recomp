// PrizeParam (a SafeBuffer row of a map prize) and, attributed to it by position, the std::function thunks of
// RaidBossActor::make_marker(SceneRaidBattle*, cocos2d::Node*, float)::$_0: the touch callback of a boss part's
// target marker (choose the part the player attacks).
#include "battleui/SceneRaidBattle.hpp"
#include "core/Guest.hpp"
#include "core/SafeBuffer.hpp"

namespace w3_7_prize {

namespace {
constexpr uint32_t SAFEBUFFER_DTOR = 0x45A13D;           // SafeBuffer::~SafeBuffer() (returns this)
constexpr uint32_t SAFEBUFFER_READ = 0x45A269;           // SafeBuffer::read(dst, src, n, index)
constexpr uint32_t STAGEUI_GET_MEDAL_FIELD = 0x75E65D;   // StageUI::getMedalField()
constexpr uint32_t MEDALDECK_GET_CURRENT_MEDAL_INFO = 0x5DFF6B;  // MedalDeck::getCurrentMedalInfo() -> shared_ptr<SafeBuffer>*
constexpr uint32_t BURST_CREATE = 0x77A491;              // master::Burst::create(out shared_ptr, int id)
constexpr uint32_t VEC2_COPY = 0x8E4C03;                 // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t VEC2_SUBTRACT = 0x8E530F;             // cocos2d::Vec2::subtract(const Vec2&)
constexpr uint32_t VEC2_DTOR = 0x8E4C17;                 // cocos2d::Vec2::~Vec2()
constexpr uint32_t EXEC_RAID_ATTACK = 0x6099E5;          // SceneRaidBattle::execRaidAttack(target, burst, whole)
constexpr uint32_t NODE_GET_POSITION_SLOT = 21;          // cocos2d::Node::getPosition() const (vtable +0x54)

constexpr uint32_t MARKER_FUNC_VTABLE = 0xF5DE90;        // __func<make_marker $_0, void(Node*)> + 8
constexpr uint32_t MARKER_LAMBDA_NAME = 0xE4E2E0;        // typeid($_0).name()
constexpr uint32_t MARKER_LAMBDA_TYPEINFO = 0xF96470;    // typeinfo of $_0

// RaidBossActor fields the callback touches
constexpr uint32_t BOSS_TARGET_PART = 0x398;      // +920 int: the part the next attack aims at
constexpr uint32_t BOSS_SELECTED_PART = 0x39C;    // +924 int: the marker tapped (< 0: none yet)
constexpr uint32_t BOSS_PART_TARGETED = 0x3A0;    // +928 bool[6]: the part was chosen as a target
constexpr uint32_t BOSS_PART_STATUS = 0x3B4;      // +948 SafeBuffer[6] (36 bytes apart): per-part state
constexpr uint32_t BOSS_PART_STATUS_STRIDE = 36;
constexpr uint32_t PART_ALIVE = 8;                // data position: int, the part can still be hit

// medal data positions (the current deck medal) and the burst table
constexpr uint32_t MEDAL_HAS_BURST = 0x4A8;       // int: the medal's burst gauge is full
constexpr uint32_t MEDAL_BURST_ID = 0x4AC;        // int: master::Burst id
constexpr uint32_t BURST_TARGET_KIND = 0x160;     // int: 1 = a single target the player picks

// RaidBattle: the point the player's attack goes to (relative to the boss)
constexpr uint32_t RAIDBATTLE_TARGET_X = 0x294;   // +660
constexpr uint32_t RAIDBATTLE_TARGET_Y = 0x298;   // +664

// The captures of $_0: { RaidBossActor* boss; int part; SceneRaidBattle* scene; } after the __func vtable.
struct MarkerFunc {
    rw::Field<uint32_t> vtable;   // +0
    rw::Field<uint32_t> boss;     // +4
    rw::Field<int32_t> part;      // +8
    rw::Field<uint32_t> scene;    // +12
};
RW_SIZE(MarkerFunc, 16);

uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
int32_t readInt(uint32_t buffer, const uint32_t out, uint32_t position) {
    rw::call(SAFEBUFFER_READ, {buffer, out, ld32(buffer + 12) + position, 4, position});
    return rw::load<int32_t>(out);
}
}  // namespace

// ~PrizeParam() (deleting): SafeBuffer destructor + operator delete of what it returned
static void PrizeParam_destructDelete(SafeBuffer *self) {
    guest::deleteObject(rw::call(SAFEBUFFER_DTOR, {rw::guest_addr(self)}));
}
RW_FUNCTION(0x5F7ACE, "PrizeParam::~PrizeParam (deleting)", PrizeParam_destructDelete);

// ------------------------------------------------------------------------------------------------ marker $_0
// operator()(Node* marker): when no part is selected yet and the tapped part is still alive, select it - during
// the attack phase with a single-target burst medal, aim the burst at it; during input, attack it right away.
static void MarkerTouch_invoke(MarkerFunc *self, const uint32_t *markerArg) {
    guest::Frame f(0x3C);
    const uint32_t vec = f.addr(0x18), burst = f.addr(0x24), value = f.addr(0x2C), value2 = f.addr(0x30);
    const uint32_t boss = self->boss;
    if (rw::load<int32_t>(boss + BOSS_SELECTED_PART) >= 0) return;
    const uint32_t marker = *markerArg;
    const uint32_t partStatus = boss + uint32_t(self->part) * BOSS_PART_STATUS_STRIDE + BOSS_PART_STATUS;
    if (!readInt(partStatus, value, PART_ALIVE)) return;
    SceneRaidBattle *scene = rw::host_ptr<SceneRaidBattle>(self->scene);
    int32_t state = scene->battleState;
    if (state == SceneRaidBattle::PLAYER_ATTACK) {
        rw::set<int32_t>(rw::host_ptr<uint8_t>(boss), BOSS_SELECTED_PART, self->part);
        uint32_t field = rw::call(STAGEUI_GET_MEDAL_FIELD, {rw::host_ptr<SceneRaidBattle>(self->scene)->stageUI});
        uint32_t medalInfo = rw::call(MEDALDECK_GET_CURRENT_MEDAL_INFO, {field});   // shared_ptr<SafeBuffer>*
        if (!readInt(ld32(medalInfo), value, MEDAL_HAS_BURST)) return;
        int32_t burstId = readInt(ld32(medalInfo), value2, MEDAL_BURST_ID);
        rw::call(BURST_CREATE, {burst, uint32_t(burstId)});
        if (readInt(ld32(burst), value2, BURST_TARGET_KIND) == 1) {
            rw::set<int32_t>(rw::host_ptr<uint8_t>(boss), BOSS_TARGET_PART, self->part);
            rw::store8(rw::host_ptr<uint8_t>(boss + uint32_t(self->part) + BOSS_PART_TARGETED), 1);
            uint32_t raidBattle = rw::host_ptr<SceneRaidBattle>(self->scene)->raidBattle;
            uint32_t markerPos = rw::vcall(rw::host_ptr<uint8_t>(marker), NODE_GET_POSITION_SLOT, {marker});
            uint32_t bossPos = rw::vcall(rw::host_ptr<uint8_t>(boss), NODE_GET_POSITION_SLOT, {boss});
            rw::call(VEC2_COPY, {vec, markerPos});
            rw::call(VEC2_SUBTRACT, {vec, bossPos});
            uint32_t x = ld32(vec), y = ld32(vec + 4);
            rw::save<uint32_t>(raidBattle + RAIDBATTLE_TARGET_Y, y);
            rw::save<uint32_t>(raidBattle + RAIDBATTLE_TARGET_X, x);
            rw::call(VEC2_DTOR, {vec});
        }
        uint32_t ctrl = ld32(burst + 4);
        if (ctrl) guest::release(ctrl);
    } else if (state == SceneRaidBattle::PLAYER_INPUT) {
        rw::set<int32_t>(rw::host_ptr<uint8_t>(boss), BOSS_SELECTED_PART, self->part);
        rw::set<int32_t>(rw::host_ptr<uint8_t>(boss), BOSS_TARGET_PART, self->part);
        rw::store8(rw::host_ptr<uint8_t>(boss + uint32_t(self->part) + BOSS_PART_TARGETED), 1);
        uint32_t sceneAddr = self->scene;
        uint32_t markerPos = rw::vcall(rw::host_ptr<uint8_t>(marker), NODE_GET_POSITION_SLOT, {marker});
        rw::call(EXEC_RAID_ATTACK, {sceneAddr, markerPos, 0, 0});
    }
}
RW_FUNCTION(0x5F7650, "RaidBossActor::make_marker $_0 __func::operator()", MarkerTouch_invoke);

// ~__func (deleting): the captures are trivial
static void MarkerFunc_destructDelete(uint32_t self) { guest::deleteObject(self); }
RW_FUNCTION(0x5F75E4, "RaidBossActor::make_marker $_0 __func::~__func (deleting)", MarkerFunc_destructDelete);

static uint32_t MarkerFunc_clone(const MarkerFunc *self) {
    uint32_t p = guest::newObject(sizeof(MarkerFunc));
    if (p) {
        MarkerFunc *copy = rw::host_ptr<MarkerFunc>(p);
        copy->vtable = MARKER_FUNC_VTABLE;
        uint32_t boss = self->boss, part = self->part, scene = self->scene;
        copy->boss = boss;
        copy->part = int32_t(part);
        copy->scene = scene;
    }
    return p;
}
RW_FUNCTION(0x5F75EC, "RaidBossActor::make_marker $_0 __func::__clone()", MarkerFunc_clone);

// __clone(__base* p): r0 ends as the scene capture when p is set (a leftover of the copy), else self
static uint32_t MarkerFunc_cloneInto(const MarkerFunc *self, MarkerFunc *p) {
    if (!p) return rw::guest_addr(self);
    p->vtable = MARKER_FUNC_VTABLE;
    uint32_t boss = self->boss, part = self->part, scene = self->scene;
    p->scene = scene;
    p->part = int32_t(part);
    p->boss = boss;
    return scene;
}
RW_FUNCTION(0x5F761C, "RaidBossActor::make_marker $_0 __func::__clone(__base*)", MarkerFunc_cloneInto);

static void MarkerFunc_destroy() {}
RW_FUNCTION(0x5F7644, "RaidBossActor::make_marker $_0 __func::destroy (nullsub_99)", MarkerFunc_destroy);

static void MarkerFunc_destroyDeallocate(uint32_t self) { guest::deleteObject(self); }
RW_FUNCTION(0x5F7646, "RaidBossActor::make_marker $_0 __func::destroy_deallocate", MarkerFunc_destroyDeallocate);

// target(const type_info&): the lambda when the type matches (type_info compared by name pointer)
static uint32_t MarkerFunc_target(const MarkerFunc *self, const uint32_t *typeInfo) {
    return rw::get<uint32_t>(typeInfo, 4) == MARKER_LAMBDA_NAME ? rw::guest_addr(self) + 4 : 0;
}
RW_FUNCTION(0x5F7854, "RaidBossActor::make_marker $_0 __func::target", MarkerFunc_target);

static uint32_t MarkerFunc_targetType() { return MARKER_LAMBDA_TYPEINFO; }
RW_FUNCTION(0x5F7874, "RaidBossActor::make_marker $_0 __func::target_type", MarkerFunc_targetType);

static void PrizeParam_nullsub100() {}
RW_FUNCTION(0x5F7888, "nullsub_100 (empty function after the $_0 thunks)", PrizeParam_nullsub100);

}  // namespace w3_7_prize
