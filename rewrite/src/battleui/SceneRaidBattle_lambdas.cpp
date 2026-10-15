// SceneRaidBattle: the scene's lambdas. Each is a libc++ std::function __func object: its operator() (the lambda
// body) and the fixed thunks (deleting destructor, __clone(), __clone(dst), destroy(), destroy_deallocate(),
// target(type_info), target_type()). Every lambda captures `this` (the scene) except the movie factory of the
// tutorial ending, which captures the scene, the movie id and its flag.
#include "SceneRaidBattle.hpp"

using namespace scene;

namespace {

constexpr uint32_t start_movie = 0x6C4A55;                        // Layer* start_movie(int id, bool)
constexpr uint32_t RaidBossLwfDialog_create = 0x59734D;
constexpr uint32_t RaidBossLwfDialog_open = 0x5973ED;             // (dialog, parent, type, int, int, function)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneBattleBase_onSwipeBattle = 0x5FA6ED;      // (scene, GestureRecognizer*, const Vec2&, dir)
constexpr uint32_t SceneBattleBase_onTouchBeganBattle = 0x5FA631;
constexpr uint32_t SceneBattleBase_onTouchMovedBattle = 0x5FA675;
constexpr uint32_t SceneBattleBase_onTouchEndedBattle = 0x5FA679;
constexpr uint32_t SceneBattleBase_onTouchCancelledBattle = 0x5FA6B9;
constexpr uint32_t SceneBattleBase_addPlayerHP = 0x5FAC9D;
constexpr uint32_t SceneBattleBase_addPlayerCP = 0x5FAD2D;
constexpr uint32_t SceneBattleBase_isBurstEnoughCP = 0x5FAE75;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Avatar_addBurst = 0x5C6289;
constexpr uint32_t RaidBossActor_marker_mode_chamge = 0x5F3745;
constexpr uint32_t StageActorManager_attackReadyAction = 0x74FCED;
constexpr uint32_t RaidBattle_moveCamera = 0x5EF751;
constexpr uint32_t RaidDirection_raid_fever = 0x5F7E69;
constexpr uint32_t RaidDirection_raid_NoCover = 0x5F8545;
constexpr uint32_t RaidDirection_raid_Cover = 0x5F8605;

// __func vtables (+8)
constexpr uint32_t VT_update_20_inner = 0xF5EBBC;
constexpr uint32_t VT_update_20_movie = 0xF5EBE8;
constexpr uint32_t VT_setRaidStage_11_inner = 0xF5EDA0;

constexpr uint32_t BOSS_WHOLE_POINT = 0x390, BOSS_PART_TARGET = 0x3A0, BOSS_SPREAD = 0x398;
constexpr uint32_t BATTLE_TOUCHING = 0x324;          // RaidBattle: bool, a touch is down
constexpr uint32_t BATTLE_FEVER = 0x31C;
constexpr uint32_t ACTORS_PLAYER = 0x20, ACTORS_FRIEND = 0x24, ACTOR_AVATAR = 0x288;
constexpr uint32_t RDIR_COVER_A = 0x298, RDIR_COVER_B = 0x29C, RDIR_COVER_C = 0x280, RDIR_COVER_D = 0x284;
constexpr uint32_t MEDAL_BURST_FLAG = 0x4A8, MEDAL_BURST = 0x4AC;   // master::Medal rows
constexpr uint32_t DIRECTOR_SCHEDULER = 0x60, SCHEDULER_TIME_SCALE = 0x14;
constexpr uint32_t MOVIE_LWF_TYPE = 2, MOVIE_LWF_ID = 1016;

SceneRaidBattle *sceneOf(uint32_t functor) { return rw::host_ptr<SceneRaidBattle>(ld32(functor + 4)); }
uint32_t timeScaleAddr() { return ld32(director() + DIRECTOR_SCHEDULER) + SCHEDULER_TIME_SCALE; }

// ------------------------------------------------------------------------------------------------ thunks
// The std::function plumbing of a lambda whose captures are `words` 32-bit values (the functor is the vtable
// followed by the captures). NAME = the address of the lambda's type_info name, TI = its type_info.
template <uint32_t VT, uint32_t WORDS, uint32_t NAME, uint32_t TI>
struct Func {
    // ~__func() deleting / destroy_deallocate(): free the heap functor
    static void destroyDelete(uint32_t self) { call(fn::operator_delete, {self}); }
    static void copyCaptures(uint32_t dst, uint32_t src) {
        for (uint32_t k = 1; k <= WORDS; k++) st32(dst + 4 * k, ld32(src + 4 * k));
    }
    // __clone(): a heap copy
    static uint32_t clone(uint32_t self) {
        uint32_t p = call(fn::operator_new, {4 + 4 * WORDS});
        if (p) {
            st32(p, VT);
            copyCaptures(p, self);
        }
        return p;
    }
    // __clone(dst): copy into a std::function's inline buffer
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (!dst) return;
        st32(dst, VT);
        copyCaptures(dst, self);
    }
    // destroy(): the captures are trivial
    static void destroy(uint32_t self) { (void)self; }
    // target(const type_info&): the lambda when the type matches
    static uint32_t target(uint32_t self, uint32_t typeInfo) { return ld32(typeInfo + 4) == NAME ? self + 4 : 0; }
    static uint32_t targetType() { return TI; }
};

// (each lambda registers its thunks below: ~__func, __clone(), __clone(dst), destroy, destroy_deallocate,
// target, target_type)

// ------------------------------------------------------------------------------------------------ update()

// $_20: the tutorial raid's black-out ended: the raid boss LWF dialog, then the movie.
void update20(uint32_t functor) {
    uint32_t a = ld32(functor + 4);
    uint32_t dialog = call(RaidBossLwfDialog_create, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    Frame f;
    uint32_t done = newFunction(f, VT_update_20_inner, {a});
    call(RaidBossLwfDialog_open, {dialog, layer, MOVIE_LWF_TYPE, MOVIE_LWF_ID, 1, done});
    functionDestroy(done);
}
using Update20 = Func<0xF5EB90, 1, 0xE51A70, 0xF97A84>;
RW_FUNCTION(0x60ABF8, "SceneRaidBattle::update::$_20::~__func", Update20::destroyDelete);
RW_FUNCTION(0x60AC00, "SceneRaidBattle::update::$_20::__clone()", Update20::clone);
RW_FUNCTION(0x60AC2C, "SceneRaidBattle::update::$_20::__clone(__base*)", Update20::cloneInto);
RW_FUNCTION(0x60AC4C, "SceneRaidBattle::update::$_20::destroy", Update20::destroy);
RW_FUNCTION(0x60AC4E, "SceneRaidBattle::update::$_20::destroy_deallocate", Update20::destroyDelete);
RW_FUNCTION(0x60AD14, "SceneRaidBattle::update::$_20::target", Update20::target);
RW_FUNCTION(0x60AD34, "SceneRaidBattle::update::$_20::target_type", Update20::targetType);
RW_FUNCTION(0x60AC58, "SceneRaidBattle::update::$_20::operator()", update20);

// $_20's dialog closed: stop the scene and change to the next movie (the id counts up).
void update20Closed(uint32_t functor) {
    SceneRaidBattle *self = sceneOf(functor);
    uint32_t a = rw::guest_addr(self);
    call(Node_unscheduleUpdate, {a});
    self->movieId = self->movieId + 1;
    int32_t movie = self->movieId;
    uint32_t flag = self->movieFlag;
    uint32_t mgr = sceneManager();
    Frame f;
    uint32_t factory = newFunction(f, VT_update_20_movie, {a, uint32_t(movie), flag});
    call(SceneManager_changeSceneLayer, {mgr, factory, 1, 1, 1});
    functionDestroy(factory);
}
using Update20Closed = Func<0xF5EBBC, 1, 0xE51B10, 0xF97A98>;
RW_FUNCTION(0x60AD48, "SceneRaidBattle::update::$_20::{lambda()}::~__func", Update20Closed::destroyDelete);
RW_FUNCTION(0x60AD50, "SceneRaidBattle::update::$_20::{lambda()}::__clone()", Update20Closed::clone);
RW_FUNCTION(0x60AD7C, "SceneRaidBattle::update::$_20::{lambda()}::__clone(__base*)", Update20Closed::cloneInto);
RW_FUNCTION(0x60AD9C, "SceneRaidBattle::update::$_20::{lambda()}::destroy", Update20Closed::destroy);
RW_FUNCTION(0x60AD9E, "SceneRaidBattle::update::$_20::{lambda()}::destroy_deallocate", Update20Closed::destroyDelete);
RW_FUNCTION(0x60AE68, "SceneRaidBattle::update::$_20::{lambda()}::target", Update20Closed::target);
RW_FUNCTION(0x60AE88, "SceneRaidBattle::update::$_20::{lambda()}::target_type", Update20Closed::targetType);
RW_FUNCTION(0x60ADA8, "SceneRaidBattle::update::$_20::{lambda()}::operator()", update20Closed);

// the layer factory: the movie scene
uint32_t update20Movie(uint32_t functor) {
    return call(start_movie, {ld32(functor + 8), ld8(functor + 12)});
}
using Update20Movie = Func<0xF5EBE8, 3, 0xE51BD0, 0xF97AAC>;
RW_FUNCTION(0x60AE9C, "SceneRaidBattle::update::$_20::{lambda()}::{lambda()}::~__func", Update20Movie::destroyDelete);
RW_FUNCTION(0x60AEA4, "SceneRaidBattle::update::$_20::{lambda()}::{lambda()}::__clone()", Update20Movie::clone);
RW_FUNCTION(0x60AED4, "SceneRaidBattle::update::$_20::{lambda()}::{lambda()}::__clone(__base*)", Update20Movie::cloneInto);
RW_FUNCTION(0x60AEFC, "SceneRaidBattle::update::$_20::{lambda()}::{lambda()}::destroy", Update20Movie::destroy);
RW_FUNCTION(0x60AEFE, "SceneRaidBattle::update::$_20::{lambda()}::{lambda()}::destroy_deallocate", Update20Movie::destroyDelete);
RW_FUNCTION(0x60AF14, "SceneRaidBattle::update::$_20::{lambda()}::{lambda()}::target", Update20Movie::target);
RW_FUNCTION(0x60AF34, "SceneRaidBattle::update::$_20::{lambda()}::{lambda()}::target_type", Update20Movie::targetType);
RW_FUNCTION(0x60AF06, "SceneRaidBattle::update::$_20::{lambda()}::{lambda()}::operator()", update20Movie);

// $_19: the tutorial popup closed: the battle UI as in HEADING.
void update19(uint32_t functor) {
    SceneRaidBattle *self = sceneOf(functor);
    call(RaidBossActor_marker_mode_chamge, {self->boss});
    self->markerActive = 1;
    call(StageActorManager_attackReadyAction, {self->actorManager, self->boss});
    SceneRaidBattle::RaidBattle_Start(self);
    self->waitFrames = 60;
    self->battleState = SceneRaidBattle::START_WAIT;
}
using Update19 = Func<0xF5EC14, 1, 0xE51C70, 0xF97AC0>;
RW_FUNCTION(0x60AF48, "SceneRaidBattle::update::$_19::~__func", Update19::destroyDelete);
RW_FUNCTION(0x60AF50, "SceneRaidBattle::update::$_19::__clone()", Update19::clone);
RW_FUNCTION(0x60AF7C, "SceneRaidBattle::update::$_19::__clone(__base*)", Update19::cloneInto);
RW_FUNCTION(0x60AF9C, "SceneRaidBattle::update::$_19::destroy", Update19::destroy);
RW_FUNCTION(0x60AF9E, "SceneRaidBattle::update::$_19::destroy_deallocate", Update19::destroyDelete);
RW_FUNCTION(0x60AFE8, "SceneRaidBattle::update::$_19::target", Update19::target);
RW_FUNCTION(0x60B008, "SceneRaidBattle::update::$_19::target_type", Update19::targetType);
RW_FUNCTION(0x60AFA8, "SceneRaidBattle::update::$_19::operator()", update19);

// ------------------------------------------------------------------------------------------------ loadUIComplete()

// $_18: a swipe: the base handling, then (at the input step) a whole attack on every part.
void swipe(uint32_t functor, uint32_t recognizerRef, uint32_t pos, uint32_t directionRef) {
    SceneRaidBattle *self = sceneOf(functor);
    uint32_t a = rw::guest_addr(self);
    call(SceneBattleBase_onSwipeBattle, {a, ld32(recognizerRef), pos, ld32(directionRef)});
    if (self->battleState != SceneRaidBattle::PLAYER_INPUT) return;
    st32(self->boss + BOSS_SPREAD, 0xFFFFFFFFu);
    st32(self->boss + BOSS_SPREAD + 4, 0xFFFFFFFFu);
    for (uint32_t k = 0; k < 6; k++) st8(self->boss + k + BOSS_PART_TARGET, 1);
    SceneRaidBattle::execRaidAttack(self, self->boss + BOSS_WHOLE_POINT, 1, 0);
}
using Swipe = Func<0xF5EC40, 1, 0xE51D30, 0xF97AD4>;
RW_FUNCTION(0x60B166, "SceneRaidBattle::loadUIComplete::$_18::~__func", Swipe::destroyDelete);
RW_FUNCTION(0x60B170, "SceneRaidBattle::loadUIComplete::$_18::__clone()", Swipe::clone);
RW_FUNCTION(0x60B19C, "SceneRaidBattle::loadUIComplete::$_18::__clone(__base*)", Swipe::cloneInto);
RW_FUNCTION(0x60B1BC, "SceneRaidBattle::loadUIComplete::$_18::destroy", Swipe::destroy);
RW_FUNCTION(0x60B1BE, "SceneRaidBattle::loadUIComplete::$_18::destroy_deallocate", Swipe::destroyDelete);
RW_FUNCTION(0x60B228, "SceneRaidBattle::loadUIComplete::$_18::target", Swipe::target);
RW_FUNCTION(0x60B248, "SceneRaidBattle::loadUIComplete::$_18::target_type", Swipe::targetType);
RW_FUNCTION(0x60B1C6, "SceneRaidBattle::loadUIComplete::$_18::operator()", swipe);

// $_17: touch ended
void touchEnded(uint32_t functor, uint32_t touchRef, uint32_t eventRef) {
    SceneRaidBattle *self = sceneOf(functor);
    uint32_t a = rw::guest_addr(self);
    st8(self->raidBattle + BATTLE_TOUCHING, 0);
    call(SceneBattleBase_onTouchEndedBattle, {a, ld32(touchRef), ld32(eventRef)});
}
using TouchEnded = Func<0xF5EC6C, 1, 0xE51DE0, 0xF97AE8>;
RW_FUNCTION(0x60B25C, "SceneRaidBattle::loadUIComplete::$_17::~__func", TouchEnded::destroyDelete);
RW_FUNCTION(0x60B264, "SceneRaidBattle::loadUIComplete::$_17::__clone()", TouchEnded::clone);
RW_FUNCTION(0x60B290, "SceneRaidBattle::loadUIComplete::$_17::__clone(__base*)", TouchEnded::cloneInto);
RW_FUNCTION(0x60B2B0, "SceneRaidBattle::loadUIComplete::$_17::destroy", TouchEnded::destroy);
RW_FUNCTION(0x60B2B2, "SceneRaidBattle::loadUIComplete::$_17::destroy_deallocate", TouchEnded::destroyDelete);
RW_FUNCTION(0x60B2D8, "SceneRaidBattle::loadUIComplete::$_17::target", TouchEnded::target);
RW_FUNCTION(0x60B2F8, "SceneRaidBattle::loadUIComplete::$_17::target_type", TouchEnded::targetType);
RW_FUNCTION(0x60B2BA, "SceneRaidBattle::loadUIComplete::$_17::operator()", touchEnded);

// $_16: touch cancelled
void touchCancelled(uint32_t functor, uint32_t touchRef, uint32_t eventRef) {
    call(SceneBattleBase_onTouchCancelledBattle, {ld32(functor + 4), ld32(touchRef), ld32(eventRef)});
}
using TouchCancelled = Func<0xF5EC98, 1, 0xE51E90, 0xF97AFC>;
RW_FUNCTION(0x60B30C, "SceneRaidBattle::loadUIComplete::$_16::~__func", TouchCancelled::destroyDelete);
RW_FUNCTION(0x60B314, "SceneRaidBattle::loadUIComplete::$_16::__clone()", TouchCancelled::clone);
RW_FUNCTION(0x60B340, "SceneRaidBattle::loadUIComplete::$_16::__clone(__base*)", TouchCancelled::cloneInto);
RW_FUNCTION(0x60B360, "SceneRaidBattle::loadUIComplete::$_16::destroy", TouchCancelled::destroy);
RW_FUNCTION(0x60B362, "SceneRaidBattle::loadUIComplete::$_16::destroy_deallocate", TouchCancelled::destroyDelete);
RW_FUNCTION(0x60B378, "SceneRaidBattle::loadUIComplete::$_16::target", TouchCancelled::target);
RW_FUNCTION(0x60B398, "SceneRaidBattle::loadUIComplete::$_16::target_type", TouchCancelled::targetType);
RW_FUNCTION(0x60B36A, "SceneRaidBattle::loadUIComplete::$_16::operator()", touchCancelled);

// $_15: touch moved
void touchMoved(uint32_t functor, uint32_t touchRef, uint32_t eventRef) {
    call(SceneBattleBase_onTouchMovedBattle, {ld32(functor + 4), ld32(touchRef), ld32(eventRef)});
}
using TouchMoved = Func<0xF5ECC4, 1, 0xE51F40, 0xF97B10>;
RW_FUNCTION(0x60B3AC, "SceneRaidBattle::loadUIComplete::$_15::~__func", TouchMoved::destroyDelete);
RW_FUNCTION(0x60B3B4, "SceneRaidBattle::loadUIComplete::$_15::__clone()", TouchMoved::clone);
RW_FUNCTION(0x60B3E0, "SceneRaidBattle::loadUIComplete::$_15::__clone(__base*)", TouchMoved::cloneInto);
RW_FUNCTION(0x60B400, "SceneRaidBattle::loadUIComplete::$_15::destroy", TouchMoved::destroy);
RW_FUNCTION(0x60B402, "SceneRaidBattle::loadUIComplete::$_15::destroy_deallocate", TouchMoved::destroyDelete);
RW_FUNCTION(0x60B418, "SceneRaidBattle::loadUIComplete::$_15::target", TouchMoved::target);
RW_FUNCTION(0x60B438, "SceneRaidBattle::loadUIComplete::$_15::target_type", TouchMoved::targetType);
RW_FUNCTION(0x60B40A, "SceneRaidBattle::loadUIComplete::$_15::operator()", touchMoved);

// $_14: touch began (always taken)
bool touchBegan(uint32_t functor, uint32_t touchRef, uint32_t eventRef) {
    SceneRaidBattle *self = sceneOf(functor);
    uint32_t a = rw::guest_addr(self);
    st8(self->raidBattle + BATTLE_TOUCHING, 1);
    call(SceneBattleBase_onTouchBeganBattle, {a, ld32(touchRef), ld32(eventRef)});
    return true;
}
using TouchBegan = Func<0xF5ECF0, 1, 0xE51FF0, 0xF97B24>;
RW_FUNCTION(0x60B44C, "SceneRaidBattle::loadUIComplete::$_14::~__func", TouchBegan::destroyDelete);
RW_FUNCTION(0x60B454, "SceneRaidBattle::loadUIComplete::$_14::__clone()", TouchBegan::clone);
RW_FUNCTION(0x60B480, "SceneRaidBattle::loadUIComplete::$_14::__clone(__base*)", TouchBegan::cloneInto);
RW_FUNCTION(0x60B4A0, "SceneRaidBattle::loadUIComplete::$_14::destroy", TouchBegan::destroy);
RW_FUNCTION(0x60B4A2, "SceneRaidBattle::loadUIComplete::$_14::destroy_deallocate", TouchBegan::destroyDelete);
RW_FUNCTION(0x60B4CC, "SceneRaidBattle::loadUIComplete::$_14::target", TouchBegan::target);
RW_FUNCTION(0x60B4EC, "SceneRaidBattle::loadUIComplete::$_14::target_type", TouchBegan::targetType);
RW_FUNCTION(0x60B4AA, "SceneRaidBattle::loadUIComplete::$_14::operator()", touchBegan);

// ------------------------------------------------------------------------------------------------ setRaidStage()

// $_13: the avatar gains CP
uint32_t addCP(uint32_t functor, uint32_t amountRef) {
    return call(SceneBattleBase_addPlayerCP, {ld32(functor + 4), ld32(amountRef)});
}
using AddCP = Func<0xF5ED1C, 1, 0xE52090, 0xF97B38>;
RW_FUNCTION(0x60B500, "SceneRaidBattle::setRaidStage::$_13::~__func", AddCP::destroyDelete);
RW_FUNCTION(0x60B508, "SceneRaidBattle::setRaidStage::$_13::__clone()", AddCP::clone);
RW_FUNCTION(0x60B534, "SceneRaidBattle::setRaidStage::$_13::__clone(__base*)", AddCP::cloneInto);
RW_FUNCTION(0x60B554, "SceneRaidBattle::setRaidStage::$_13::destroy", AddCP::destroy);
RW_FUNCTION(0x60B556, "SceneRaidBattle::setRaidStage::$_13::destroy_deallocate", AddCP::destroyDelete);
RW_FUNCTION(0x60B56C, "SceneRaidBattle::setRaidStage::$_13::target", AddCP::target);
RW_FUNCTION(0x60B58C, "SceneRaidBattle::setRaidStage::$_13::target_type", AddCP::targetType);
RW_FUNCTION(0x60B55E, "SceneRaidBattle::setRaidStage::$_13::operator()", addCP);

// $_12: the avatar gains HP
uint32_t addHP(uint32_t functor, uint32_t amountRef) {
    return call(SceneBattleBase_addPlayerHP, {ld32(functor + 4), ld32(amountRef)});
}
using AddHP = Func<0xF5ED48, 1, 0xE52130, 0xF97B4C>;
RW_FUNCTION(0x60B5A0, "SceneRaidBattle::setRaidStage::$_12::~__func", AddHP::destroyDelete);
RW_FUNCTION(0x60B5A8, "SceneRaidBattle::setRaidStage::$_12::__clone()", AddHP::clone);
RW_FUNCTION(0x60B5D4, "SceneRaidBattle::setRaidStage::$_12::__clone(__base*)", AddHP::cloneInto);
RW_FUNCTION(0x60B5F4, "SceneRaidBattle::setRaidStage::$_12::destroy", AddHP::destroy);
RW_FUNCTION(0x60B5F6, "SceneRaidBattle::setRaidStage::$_12::destroy_deallocate", AddHP::destroyDelete);
RW_FUNCTION(0x60B60C, "SceneRaidBattle::setRaidStage::$_12::target", AddHP::target);
RW_FUNCTION(0x60B62C, "SceneRaidBattle::setRaidStage::$_12::target_type", AddHP::targetType);
RW_FUNCTION(0x60B5FE, "SceneRaidBattle::setRaidStage::$_12::operator()", addHP);

// $_11: the raid cover direction (none when its two counter pairs agree), then the fever level
void raidCover(uint32_t functor) {
    uint32_t a = ld32(functor + 4);
    SceneRaidBattle *self = rw::host_ptr<SceneRaidBattle>(a);
    uint32_t rd = self->raidDirection;
    if (ld32(rd + RDIR_COVER_B) == ld32(rd + RDIR_COVER_A) && ld32(rd + RDIR_COVER_D) == ld32(rd + RDIR_COVER_C)) {
        call(RaidDirection_raid_NoCover, {rd, 0});
        return;
    }
    Frame f;
    uint32_t then = newFunction(f, VT_setRaidStage_11_inner, {a});
    uint32_t action = call(fn::CallFunc_create, {then});
    functionDestroy(then);
    call(RaidDirection_raid_Cover, {self->raidDirection, action});
}
using RaidCover = Func<0xF5ED74, 1, 0xE521D0, 0xF97B60>;
RW_FUNCTION(0x60B640, "SceneRaidBattle::setRaidStage::$_11::~__func", RaidCover::destroyDelete);
RW_FUNCTION(0x60B648, "SceneRaidBattle::setRaidStage::$_11::__clone()", RaidCover::clone);
RW_FUNCTION(0x60B674, "SceneRaidBattle::setRaidStage::$_11::__clone(__base*)", RaidCover::cloneInto);
RW_FUNCTION(0x60B694, "SceneRaidBattle::setRaidStage::$_11::destroy", RaidCover::destroy);
RW_FUNCTION(0x60B696, "SceneRaidBattle::setRaidStage::$_11::destroy_deallocate", RaidCover::destroyDelete);
RW_FUNCTION(0x60B780, "SceneRaidBattle::setRaidStage::$_11::target", RaidCover::target);
RW_FUNCTION(0x60B7A0, "SceneRaidBattle::setRaidStage::$_11::target_type", RaidCover::targetType);
RW_FUNCTION(0x60B6A0, "SceneRaidBattle::setRaidStage::$_11::operator()", raidCover);

void raidCoverDone(uint32_t functor) {
    SceneRaidBattle *self = sceneOf(functor);
    call(RaidDirection_raid_fever, {self->raidDirection, ld32(self->raidBattle + BATTLE_FEVER)});
}
using RaidCoverDone = Func<0xF5EDA0, 1, 0xE52270, 0xF97B74>;
RW_FUNCTION(0x60B7B4, "SceneRaidBattle::setRaidStage::$_11::{lambda()}::~__func", RaidCoverDone::destroyDelete);
RW_FUNCTION(0x60B7BC, "SceneRaidBattle::setRaidStage::$_11::{lambda()}::__clone()", RaidCoverDone::clone);
RW_FUNCTION(0x60B7E8, "SceneRaidBattle::setRaidStage::$_11::{lambda()}::__clone(__base*)", RaidCoverDone::cloneInto);
RW_FUNCTION(0x60B808, "SceneRaidBattle::setRaidStage::$_11::{lambda()}::destroy", RaidCoverDone::destroy);
RW_FUNCTION(0x60B80A, "SceneRaidBattle::setRaidStage::$_11::{lambda()}::destroy_deallocate", RaidCoverDone::destroyDelete);
RW_FUNCTION(0x60B830, "SceneRaidBattle::setRaidStage::$_11::{lambda()}::target", RaidCoverDone::target);
RW_FUNCTION(0x60B850, "SceneRaidBattle::setRaidStage::$_11::{lambda()}::target_type", RaidCoverDone::targetType);
RW_FUNCTION(0x60B812, "SceneRaidBattle::setRaidStage::$_11::{lambda()}::operator()", raidCoverDone);

// $_10: the camera back to the centre, zoomed out
void cameraCentre(uint32_t functor) {
    SceneRaidBattle *self = sceneOf(functor);
    Frame f;
    uint32_t centre = newVec2(f, 0.0f, 0.0f);
    call(RaidBattle_moveCamera, {self->raidBattle, fbits(1.0f), centre, fbits(0.5f), 1});
    call(Vec2_dtor, {centre});
}
using CameraCentre = Func<0xF5EDCC, 1, 0xE52320, 0xF97B88>;
RW_FUNCTION(0x60B864, "SceneRaidBattle::setRaidStage::$_10::~__func", CameraCentre::destroyDelete);
RW_FUNCTION(0x60B86C, "SceneRaidBattle::setRaidStage::$_10::__clone()", CameraCentre::clone);
RW_FUNCTION(0x60B898, "SceneRaidBattle::setRaidStage::$_10::__clone(__base*)", CameraCentre::cloneInto);
RW_FUNCTION(0x60B8B8, "SceneRaidBattle::setRaidStage::$_10::destroy", CameraCentre::destroy);
RW_FUNCTION(0x60B8BA, "SceneRaidBattle::setRaidStage::$_10::destroy_deallocate", CameraCentre::destroyDelete);
RW_FUNCTION(0x60B944, "SceneRaidBattle::setRaidStage::$_10::target", CameraCentre::target);
RW_FUNCTION(0x60B964, "SceneRaidBattle::setRaidStage::$_10::target_type", CameraCentre::targetType);
RW_FUNCTION(0x60B8C4, "SceneRaidBattle::setRaidStage::$_10::operator()", cameraCentre);

// $_9 / $_8: a medal was used: an avatar's burst gauge grows by the medal's burst value (when it has one)
void medalBurst(uint32_t functor, uint32_t medalRef, uint32_t actorSlot) {
    SceneRaidBattle *self = sceneOf(functor);
    uint32_t medal = ld32(medalRef);                           // shared_ptr<const master::Medal>
    if (bt::sbGet32(medal, MEDAL_BURST_FLAG) == 0) return;
    uint32_t actor = ld32(self->actorManager + actorSlot);
    int32_t burst = bt::sbGet32(ld32(medalRef), MEDAL_BURST);
    call(Avatar_addBurst, {ld32(actor + ACTOR_AVATAR), uint32_t(burst)});
}
void friendMedalUsed(uint32_t functor, uint32_t medalRef) { medalBurst(functor, medalRef, ACTORS_FRIEND); }
using FriendMedalUsed = Func<0xF5EDF8, 1, 0xE523E0, 0xF97B9C>;
RW_FUNCTION(0x60B978, "SceneRaidBattle::setRaidStage::$_9::~__func", FriendMedalUsed::destroyDelete);
RW_FUNCTION(0x60B980, "SceneRaidBattle::setRaidStage::$_9::__clone()", FriendMedalUsed::clone);
RW_FUNCTION(0x60B9AC, "SceneRaidBattle::setRaidStage::$_9::__clone(__base*)", FriendMedalUsed::cloneInto);
RW_FUNCTION(0x60B9CC, "SceneRaidBattle::setRaidStage::$_9::destroy", FriendMedalUsed::destroy);
RW_FUNCTION(0x60B9CE, "SceneRaidBattle::setRaidStage::$_9::destroy_deallocate", FriendMedalUsed::destroyDelete);
RW_FUNCTION(0x60BA60, "SceneRaidBattle::setRaidStage::$_9::target", FriendMedalUsed::target);
RW_FUNCTION(0x60BA80, "SceneRaidBattle::setRaidStage::$_9::target_type", FriendMedalUsed::targetType);
RW_FUNCTION(0x60B9D8, "SceneRaidBattle::setRaidStage::$_9::operator()", friendMedalUsed);

void medalUsed(uint32_t functor, uint32_t medalRef) { medalBurst(functor, medalRef, ACTORS_PLAYER); }
using MedalUsed = Func<0xF5EE24, 1, 0xE524A0, 0xF97BB0>;
RW_FUNCTION(0x60BA94, "SceneRaidBattle::setRaidStage::$_8::~__func", MedalUsed::destroyDelete);
RW_FUNCTION(0x60BA9C, "SceneRaidBattle::setRaidStage::$_8::__clone()", MedalUsed::clone);
RW_FUNCTION(0x60BAC8, "SceneRaidBattle::setRaidStage::$_8::__clone(__base*)", MedalUsed::cloneInto);
RW_FUNCTION(0x60BAE8, "SceneRaidBattle::setRaidStage::$_8::destroy", MedalUsed::destroy);
RW_FUNCTION(0x60BAEA, "SceneRaidBattle::setRaidStage::$_8::destroy_deallocate", MedalUsed::destroyDelete);
RW_FUNCTION(0x60BB7C, "SceneRaidBattle::setRaidStage::$_8::target", MedalUsed::target);
RW_FUNCTION(0x60BB9C, "SceneRaidBattle::setRaidStage::$_8::target_type", MedalUsed::targetType);
RW_FUNCTION(0x60BAF4, "SceneRaidBattle::setRaidStage::$_8::operator()", medalUsed);

// $_7: enough CP for the burst
uint32_t burstEnough(uint32_t functor) { return call(SceneBattleBase_isBurstEnoughCP, {ld32(functor + 4)}); }
using BurstEnough = Func<0xF5EE50, 1, 0xE52540, 0xF97BC4>;
RW_FUNCTION(0x60BBB0, "SceneRaidBattle::setRaidStage::$_7::~__func", BurstEnough::destroyDelete);
RW_FUNCTION(0x60BBB8, "SceneRaidBattle::setRaidStage::$_7::__clone()", BurstEnough::clone);
RW_FUNCTION(0x60BBE4, "SceneRaidBattle::setRaidStage::$_7::__clone(__base*)", BurstEnough::cloneInto);
RW_FUNCTION(0x60BC04, "SceneRaidBattle::setRaidStage::$_7::destroy", BurstEnough::destroy);
RW_FUNCTION(0x60BC06, "SceneRaidBattle::setRaidStage::$_7::destroy_deallocate", BurstEnough::destroyDelete);
RW_FUNCTION(0x60BC18, "SceneRaidBattle::setRaidStage::$_7::target", BurstEnough::target);
RW_FUNCTION(0x60BC38, "SceneRaidBattle::setRaidStage::$_7::target_type", BurstEnough::targetType);
RW_FUNCTION(0x60BC0E, "SceneRaidBattle::setRaidStage::$_7::operator()", burstEnough);

// $_6: the deck takes input
uint32_t deckEnabled(uint32_t functor) { return sceneOf(functor)->deckEnabled; }
using DeckEnabled = Func<0xF5EE7C, 1, 0xE525E0, 0xF97BD8>;
RW_FUNCTION(0x60BC4C, "SceneRaidBattle::setRaidStage::$_6::~__func", DeckEnabled::destroyDelete);
RW_FUNCTION(0x60BC54, "SceneRaidBattle::setRaidStage::$_6::__clone()", DeckEnabled::clone);
RW_FUNCTION(0x60BC80, "SceneRaidBattle::setRaidStage::$_6::__clone(__base*)", DeckEnabled::cloneInto);
RW_FUNCTION(0x60BCA0, "SceneRaidBattle::setRaidStage::$_6::destroy", DeckEnabled::destroy);
RW_FUNCTION(0x60BCA2, "SceneRaidBattle::setRaidStage::$_6::destroy_deallocate", DeckEnabled::destroyDelete);
RW_FUNCTION(0x60BCB8, "SceneRaidBattle::setRaidStage::$_6::target", DeckEnabled::target);
RW_FUNCTION(0x60BCD8, "SceneRaidBattle::setRaidStage::$_6::target_type", DeckEnabled::targetType);
RW_FUNCTION(0x60BCAC, "SceneRaidBattle::setRaidStage::$_6::operator()", deckEnabled);

// $_5: the burst button
bool burstButton(uint32_t functor) {
    SceneRaidBattle::onBurstButton(sceneOf(functor));
    return true;
}
using BurstButton = Func<0xF5EEA8, 1, 0xE52680, 0xF97BEC>;
RW_FUNCTION(0x60BCEC, "SceneRaidBattle::setRaidStage::$_5::~__func", BurstButton::destroyDelete);
RW_FUNCTION(0x60BCF4, "SceneRaidBattle::setRaidStage::$_5::__clone()", BurstButton::clone);
RW_FUNCTION(0x60BD20, "SceneRaidBattle::setRaidStage::$_5::__clone(__base*)", BurstButton::cloneInto);
RW_FUNCTION(0x60BD40, "SceneRaidBattle::setRaidStage::$_5::destroy", BurstButton::destroy);
RW_FUNCTION(0x60BD42, "SceneRaidBattle::setRaidStage::$_5::destroy_deallocate", BurstButton::destroyDelete);
RW_FUNCTION(0x60BD58, "SceneRaidBattle::setRaidStage::$_5::target", BurstButton::target);
RW_FUNCTION(0x60BD78, "SceneRaidBattle::setRaidStage::$_5::target_type", BurstButton::targetType);
RW_FUNCTION(0x60BD4A, "SceneRaidBattle::setRaidStage::$_5::operator()", burstButton);

// ------------------------------------------------------------------------------------------------ to_slow()

// the slow motion steps: the scheduler's time scale as a fraction of the saved one
void slowStep(uint32_t functor, float factor, bool divide) {
    SceneRaidBattle *self = sceneOf(functor);
    uint32_t dir = director();
    float saved = self->savedTimeScale;
    float scale = divide ? saved / factor : saved * factor;
    rw::save<float>(ld32(dir + DIRECTOR_SCHEDULER) + SCHEDULER_TIME_SCALE, scale);
}
void slow4(uint32_t functor) { slowStep(functor, 0.125f, false); }
void slow3(uint32_t functor) { slowStep(functor, 30.0f, true); }
void slow2(uint32_t functor) { slowStep(functor, 0.25f, false); }
using Slow4 = Func<0xF5EED4, 1, 0xE52710, 0xF97C00>;
RW_FUNCTION(0x60BD8C, "SceneRaidBattle::to_slow::$_4::~__func", Slow4::destroyDelete);
RW_FUNCTION(0x60BD94, "SceneRaidBattle::to_slow::$_4::__clone()", Slow4::clone);
RW_FUNCTION(0x60BDC0, "SceneRaidBattle::to_slow::$_4::__clone(__base*)", Slow4::cloneInto);
RW_FUNCTION(0x60BDE0, "SceneRaidBattle::to_slow::$_4::destroy", Slow4::destroy);
RW_FUNCTION(0x60BDE2, "SceneRaidBattle::to_slow::$_4::destroy_deallocate", Slow4::destroyDelete);
RW_FUNCTION(0x60BE0C, "SceneRaidBattle::to_slow::$_4::target", Slow4::target);
RW_FUNCTION(0x60BE2C, "SceneRaidBattle::to_slow::$_4::target_type", Slow4::targetType);
RW_FUNCTION(0x60BDEA, "SceneRaidBattle::to_slow::$_4::operator()", slow4);
using Slow3 = Func<0xF5EF00, 1, 0xE527A0, 0xF97C14>;
RW_FUNCTION(0x60BE40, "SceneRaidBattle::to_slow::$_3::~__func", Slow3::destroyDelete);
RW_FUNCTION(0x60BE48, "SceneRaidBattle::to_slow::$_3::__clone()", Slow3::clone);
RW_FUNCTION(0x60BE74, "SceneRaidBattle::to_slow::$_3::__clone(__base*)", Slow3::cloneInto);
RW_FUNCTION(0x60BE94, "SceneRaidBattle::to_slow::$_3::destroy", Slow3::destroy);
RW_FUNCTION(0x60BE96, "SceneRaidBattle::to_slow::$_3::destroy_deallocate", Slow3::destroyDelete);
RW_FUNCTION(0x60BEC4, "SceneRaidBattle::to_slow::$_3::target", Slow3::target);
RW_FUNCTION(0x60BEE4, "SceneRaidBattle::to_slow::$_3::target_type", Slow3::targetType);
RW_FUNCTION(0x60BEA0, "SceneRaidBattle::to_slow::$_3::operator()", slow3);
using Slow2 = Func<0xF5EF2C, 1, 0xE52830, 0xF97C28>;
RW_FUNCTION(0x60BEF8, "SceneRaidBattle::to_slow::$_2::~__func", Slow2::destroyDelete);
RW_FUNCTION(0x60BF00, "SceneRaidBattle::to_slow::$_2::__clone()", Slow2::clone);
RW_FUNCTION(0x60BF2C, "SceneRaidBattle::to_slow::$_2::__clone(__base*)", Slow2::cloneInto);
RW_FUNCTION(0x60BF4C, "SceneRaidBattle::to_slow::$_2::destroy", Slow2::destroy);
RW_FUNCTION(0x60BF4E, "SceneRaidBattle::to_slow::$_2::destroy_deallocate", Slow2::destroyDelete);
RW_FUNCTION(0x60BF78, "SceneRaidBattle::to_slow::$_2::target", Slow2::target);
RW_FUNCTION(0x60BF98, "SceneRaidBattle::to_slow::$_2::target_type", Slow2::targetType);
RW_FUNCTION(0x60BF56, "SceneRaidBattle::to_slow::$_2::operator()", slow2);

// ------------------------------------------------------------------------------------------------ init()

// $_1: the scene ends: the time scale back, no more updates
void sceneEnd(uint32_t functor) {
    SceneRaidBattle *self = sceneOf(functor);
    uint32_t dir = director();
    rw::save<float>(ld32(dir + DIRECTOR_SCHEDULER) + SCHEDULER_TIME_SCALE, self->savedTimeScale);
    call(Node_unscheduleUpdate, {rw::guest_addr(self)});
}
using SceneEnd = Func<0xF5EF58, 1, 0xE528C0, 0xF97C3C>;
RW_FUNCTION(0x60BFAC, "SceneRaidBattle::init::$_1::~__func", SceneEnd::destroyDelete);
RW_FUNCTION(0x60BFB4, "SceneRaidBattle::init::$_1::__clone()", SceneEnd::clone);
RW_FUNCTION(0x60BFE0, "SceneRaidBattle::init::$_1::__clone(__base*)", SceneEnd::cloneInto);
RW_FUNCTION(0x60C000, "SceneRaidBattle::init::$_1::destroy", SceneEnd::destroy);
RW_FUNCTION(0x60C002, "SceneRaidBattle::init::$_1::destroy_deallocate", SceneEnd::destroyDelete);
RW_FUNCTION(0x60C028, "SceneRaidBattle::init::$_1::target", SceneEnd::target);
RW_FUNCTION(0x60C048, "SceneRaidBattle::init::$_1::target_type", SceneEnd::targetType);
RW_FUNCTION(0x60C00A, "SceneRaidBattle::init::$_1::operator()", sceneEnd);

// $_0: the scene starts: updates on, the current time scale kept
void sceneStart(uint32_t functor) {
    SceneRaidBattle *self = sceneOf(functor);
    call(Node_scheduleUpdate, {rw::guest_addr(self)});
    self->savedTimeScale = rw::load<float>(timeScaleAddr());
}
using SceneStart = Func<0xF5EF84, 1, 0xE52940, 0xF97C50>;
RW_FUNCTION(0x60C05C, "SceneRaidBattle::init::$_0::~__func", SceneStart::destroyDelete);
RW_FUNCTION(0x60C064, "SceneRaidBattle::init::$_0::__clone()", SceneStart::clone);
RW_FUNCTION(0x60C090, "SceneRaidBattle::init::$_0::__clone(__base*)", SceneStart::cloneInto);
RW_FUNCTION(0x60C0B0, "SceneRaidBattle::init::$_0::destroy", SceneStart::destroy);
RW_FUNCTION(0x60C0B2, "SceneRaidBattle::init::$_0::destroy_deallocate", SceneStart::destroyDelete);
RW_FUNCTION(0x60C0D8, "SceneRaidBattle::init::$_0::target", SceneStart::target);
RW_FUNCTION(0x60C0F8, "SceneRaidBattle::init::$_0::target_type", SceneStart::targetType);
RW_FUNCTION(0x60C0BA, "SceneRaidBattle::init::$_0::operator()", sceneStart);

}  // namespace
