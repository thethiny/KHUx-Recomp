// The std::function plumbing (libc++ __func slots) of the lambdas created by StageBattleManager and, next to them
// in the binary, by StageActorManager (deployHitEffect, setupEnemyActorPlatoon: attributed to this module by
// address; StageActorManager.cpp does not register them). Each thunk was checked against its instructions: every
// __func is the vtable plus the captured words, slot 0 (~__func) is shared, slots 1 / 5 free the object, slot 4
// does nothing. The lambda bodies (operator()) are written out below.
#include "StageBattleManager.hpp"

#include "../scene_main/FunctionThunks.hpp"

namespace {
namespace g {
constexpr uint32_t ref_retain = 0x8FD0B5;                  // cocos2d::Ref::retain()
constexpr uint32_t ref_autorelease = 0x8FD0D5;             // cocos2d::Ref::autorelease()
constexpr uint32_t field_removeMapNodeToField = 0x7558E5;  // StageFieldManager::removeMapNodeToField(MapNode*)
constexpr uint32_t field_isCollisionSafe = 0x7569B5;       // StageFieldManager::isCollisionSafe(float, float)
constexpr uint32_t medalDeck_getCurrentMedalInfo = 0x5DFF6B;  // MedalDeck::getCurrentMedalInfo()
constexpr uint32_t actors_onTapBeganEnemyActor = 0x751D61;  // StageActorManager::onTapBeganEnemyActor(EnemyActor*)
constexpr uint32_t actors_onTapEndedEnemyActor = 0x751DDD;  // StageActorManager::onTapEndedEnemyActor(const MedalInfo*, EnemyActor*)
constexpr uint32_t avatar_changeMotion = 0x5C63C5;         // Avatar::changeMotion(Avatar::Motion)

// StageActorManager fields the lambdas read
constexpr uint32_t ACTORS_FIELD_MANAGER = 0x74;  // StageFieldManager*
constexpr uint32_t ACTORS_MEDAL_DECK = 0x28;     // MedalDeck*
// AvatarActor: its Avatar (the drawn character)
constexpr uint32_t AVATAR_ACTOR_AVATAR = 0x288;

// cocos2d::Node virtual slots
constexpr uint32_t VSLOT_SET_SCALE_X = 10;
constexpr uint32_t VSLOT_GET_SCALE_X = 11;

// Avatar::Motion values used by firstApproachIsMoveToFront
constexpr uint32_t MOTION_11 = 11;
constexpr uint32_t MOTION_24 = 24;
}  // namespace g

// __func types: thunk::Lambda<stored vtable, sizeof(__func), typeinfo, type name, lambda offset, captures...>
using DeployHitEffect3 = thunk::Lambda<0xF6E01C, 12, 0xFA8368, 0xE9E340, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
using Platoon2 = thunk::Lambda<0xF6E048, 8, 0xFA837C, 0xE9E450, 4, thunk::cap(4, 4)>;
using Platoon1 = thunk::Lambda<0xF6E074, 8, 0xFA8390, 0xE9E5A0, 4, thunk::cap(4, 4)>;
using Platoon0 = thunk::Lambda<0xF6E0A0, 8, 0xFA83A4, 0xE9E6E0, 4, thunk::cap(4, 4)>;
using SelectTarget3 = thunk::Lambda<0xF6E0CC, 8, 0xFA83E0, 0xE9E850, 4, thunk::cap(4, 4)>;
using MoveToFront2 = thunk::Lambda<0xF6E0F8, 8, 0xFA83F4, 0xE9E940, 4, thunk::cap(4, 4)>;
using MoveToFront1 = thunk::Lambda<0xF6E124, 8, 0xFA8408, 0xE9EA30, 4, thunk::cap(4, 4)>;
using MoveToFront0 = thunk::Lambda<0xF6E150, 8, 0xFA841C, 0xE9EB20, 4, thunk::cap(4, 4)>;

// The captured word of a one- or two-capture lambda (self = the __func object).
uint32_t capture(uint32_t self, uint32_t k = 0) { return rw::load<uint32_t>(self + 4 + 4 * k); }
}  // namespace

namespace StageBattleManager_lambdas {

// StageActorManager::deployHitEffect $_3 [node, manager]: when the hit effect ends, take the node off the field
// (kept alive across the removal, then handed to the autorelease pool).
void deployHitEffectDone(uint32_t self) {
    uint32_t node = capture(self, 0);
    uint32_t manager = capture(self, 1);
    rw::call(g::ref_retain, {node});
    rw::call(g::field_removeMapNodeToField, {rw::load<uint32_t>(manager + g::ACTORS_FIELD_MANAGER), capture(self, 0)});
    rw::call(g::ref_autorelease, {capture(self, 0)});
}

// StageActorManager::setupEnemyActorPlatoon $_2 [manager]: an enemy was released after a tap: attack it with the
// current medal.
uint32_t platoonTapEnded(uint32_t self, const uint32_t *enemy) {
    uint32_t target = *enemy;
    uint32_t manager = capture(self);
    uint32_t medal = rw::call(g::medalDeck_getCurrentMedalInfo, {rw::load<uint32_t>(manager + g::ACTORS_MEDAL_DECK)});
    return rw::call(g::actors_onTapEndedEnemyActor, {manager, medal, target});
}

// StageActorManager::setupEnemyActorPlatoon $_1 [manager]: an enemy was touched.
uint32_t platoonTapBegan(uint32_t self, const uint32_t *enemy) {
    return rw::call(g::actors_onTapBeganEnemyActor, {capture(self), *enemy});
}

// StageActorManager::setupEnemyActorPlatoon $_0 [manager]: may an enemy stand at (x, y)?
uint32_t platoonCollisionSafe(uint32_t self, const uint32_t *x, const uint32_t *y) {
    uint32_t yb = *y, xb = *x;
    return rw::call(g::field_isCollisionSafe, {rw::load<uint32_t>(capture(self) + g::ACTORS_FIELD_MANAGER), xb, yb});
}

// StageBattleManager::startSelectedTargetEnemy $_3 [manager]: an enemy waiting for the player's choice was
// tapped (select = chosen) or dismissed.
void selectWaitDone(uint32_t self, const uint32_t *enemy, const uint8_t *select) {
    StageBattleManager *manager = rw::host_ptr<StageBattleManager>(capture(self));
    uint32_t chosen = *select;
    StageBattleManager::selectedTargetEnemy(manager, *enemy, chosen);
}

// StageBattleManager::firstApproachIsMoveToFront $_2 [avatar actor]: turn the actor around (scaleX = -scaleX).
void moveToFrontFlip(uint32_t self) {
    uint32_t actor = capture(self);
    const void *node = rw::host_ptr<uint32_t>(actor);
    uint32_t vtable = rw::load<uint32_t>(actor);
    uint32_t setScaleX = rw::load<uint32_t>(vtable + 4 * g::VSLOT_SET_SCALE_X);
    uint32_t scaleX = rw::vcall(node, g::VSLOT_GET_SCALE_X, {actor});
    rw::call(setScaleX, {actor, scaleX ^ 0x80000000u});
}

// $_1 [avatar actor]: play motion 11 on its avatar.
void moveToFrontMotion11(uint32_t self) {
    rw::call(g::avatar_changeMotion, {rw::load<uint32_t>(capture(self) + g::AVATAR_ACTOR_AVATAR), g::MOTION_11});
}

// $_0 [avatar actor]: play motion 24 on its avatar.
void moveToFrontMotion24(uint32_t self) {
    rw::call(g::avatar_changeMotion, {rw::load<uint32_t>(capture(self) + g::AVATAR_ACTOR_AVATAR), g::MOTION_24});
}

}  // namespace StageBattleManager_lambdas

namespace L = StageBattleManager_lambdas;

// StageActorManager::deployHitEffect $_3 (slot 0 is the shared ~__func at 0x48343C)
RW_FUNCTION(0x75331C, "StageActorManager::deployHitEffect $_3 __func::~__func (deleting)", DeployHitEffect3::deleteThis);
RW_FUNCTION(0x753324, "StageActorManager::deployHitEffect $_3 __func::__clone()", DeployHitEffect3::clone);
RW_FUNCTION(0x753354, "StageActorManager::deployHitEffect $_3 __func::__clone(__base*)", DeployHitEffect3::cloneInto);
RW_FUNCTION(0x753378, "StageActorManager::deployHitEffect $_3 __func::destroy", DeployHitEffect3::destroy);
RW_FUNCTION(0x75337A, "StageActorManager::deployHitEffect $_3 __func::destroy_deallocate", DeployHitEffect3::deleteThis);
RW_FUNCTION(0x753382, "StageActorManager::deployHitEffect $_3", L::deployHitEffectDone);
RW_FUNCTION(0x7533A0, "StageActorManager::deployHitEffect $_3 __func::target", DeployHitEffect3::target);
RW_FUNCTION(0x7533C0, "StageActorManager::deployHitEffect $_3 __func::target_type", DeployHitEffect3::targetType);

// StageActorManager::setupEnemyActorPlatoon $_2 (slot 0 is the shared ~__func at 0x753484)
RW_FUNCTION(0x7533D4, "StageActorManager::setupEnemyActorPlatoon $_2 __func::~__func (deleting)", Platoon2::deleteThis);
RW_FUNCTION(0x7533DC, "StageActorManager::setupEnemyActorPlatoon $_2 __func::__clone()", Platoon2::clone);
RW_FUNCTION(0x753408, "StageActorManager::setupEnemyActorPlatoon $_2 __func::__clone(__base*)", Platoon2::cloneInto);
RW_FUNCTION(0x753428, "StageActorManager::setupEnemyActorPlatoon $_2 __func::destroy", Platoon2::destroy);
RW_FUNCTION(0x75342A, "StageActorManager::setupEnemyActorPlatoon $_2 __func::destroy_deallocate", Platoon2::deleteThis);
RW_FUNCTION(0x753432, "StageActorManager::setupEnemyActorPlatoon $_2", L::platoonTapEnded);
RW_FUNCTION(0x753450, "StageActorManager::setupEnemyActorPlatoon $_2 __func::target", Platoon2::target);
RW_FUNCTION(0x753470, "StageActorManager::setupEnemyActorPlatoon $_2 __func::target_type", Platoon2::targetType);

// StageActorManager::setupEnemyActorPlatoon $_1
RW_FUNCTION(0x753484, "std::__function::__func<lambda>::~__func (shared, empty)", Platoon1::destructor);
RW_FUNCTION(0x753486, "StageActorManager::setupEnemyActorPlatoon $_1 __func::~__func (deleting)", Platoon1::deleteThis);
RW_FUNCTION(0x753490, "StageActorManager::setupEnemyActorPlatoon $_1 __func::__clone()", Platoon1::clone);
RW_FUNCTION(0x7534BC, "StageActorManager::setupEnemyActorPlatoon $_1 __func::__clone(__base*)", Platoon1::cloneInto);
RW_FUNCTION(0x7534DC, "StageActorManager::setupEnemyActorPlatoon $_1 __func::destroy", Platoon1::destroy);
RW_FUNCTION(0x7534DE, "StageActorManager::setupEnemyActorPlatoon $_1 __func::destroy_deallocate", Platoon1::deleteThis);
RW_FUNCTION(0x7534E6, "StageActorManager::setupEnemyActorPlatoon $_1", L::platoonTapBegan);
RW_FUNCTION(0x7534F4, "StageActorManager::setupEnemyActorPlatoon $_1 __func::target", Platoon1::target);
RW_FUNCTION(0x753514, "StageActorManager::setupEnemyActorPlatoon $_1 __func::target_type", Platoon1::targetType);

// StageActorManager::setupEnemyActorPlatoon $_0 (slot 0 is the shared ~__func at 0x6071A8)
RW_FUNCTION(0x753528, "StageActorManager::setupEnemyActorPlatoon $_0 __func::~__func (deleting)", Platoon0::deleteThis);
RW_FUNCTION(0x753530, "StageActorManager::setupEnemyActorPlatoon $_0 __func::__clone()", Platoon0::clone);
RW_FUNCTION(0x75355C, "StageActorManager::setupEnemyActorPlatoon $_0 __func::__clone(__base*)", Platoon0::cloneInto);
RW_FUNCTION(0x75357C, "StageActorManager::setupEnemyActorPlatoon $_0 __func::destroy", Platoon0::destroy);
RW_FUNCTION(0x75357E, "StageActorManager::setupEnemyActorPlatoon $_0 __func::destroy_deallocate", Platoon0::deleteThis);
RW_FUNCTION(0x753586, "StageActorManager::setupEnemyActorPlatoon $_0", L::platoonCollisionSafe);
RW_FUNCTION(0x753598, "StageActorManager::setupEnemyActorPlatoon $_0 __func::target", Platoon0::target);
RW_FUNCTION(0x7535B8, "StageActorManager::setupEnemyActorPlatoon $_0 __func::target_type", Platoon0::targetType);

// StageBattleManager::startSelectedTargetEnemy $_3
RW_FUNCTION(0x754410, "std::__function::__func<lambda>::~__func (shared, empty) #2", SelectTarget3::destructor);
RW_FUNCTION(0x754412, "StageBattleManager::startSelectedTargetEnemy $_3 __func::~__func (deleting)", SelectTarget3::deleteThis);
RW_FUNCTION(0x75441C, "StageBattleManager::startSelectedTargetEnemy $_3 __func::__clone()", SelectTarget3::clone);
RW_FUNCTION(0x754448, "StageBattleManager::startSelectedTargetEnemy $_3 __func::__clone(__base*)", SelectTarget3::cloneInto);
RW_FUNCTION(0x754468, "StageBattleManager::startSelectedTargetEnemy $_3 __func::destroy", SelectTarget3::destroy);
RW_FUNCTION(0x75446A, "StageBattleManager::startSelectedTargetEnemy $_3 __func::destroy_deallocate", SelectTarget3::deleteThis);
RW_FUNCTION(0x754472, "StageBattleManager::startSelectedTargetEnemy $_3", L::selectWaitDone);
RW_FUNCTION(0x754480, "StageBattleManager::startSelectedTargetEnemy $_3 __func::target", SelectTarget3::target);
RW_FUNCTION(0x7544A0, "StageBattleManager::startSelectedTargetEnemy $_3 __func::target_type", SelectTarget3::targetType);

// StageBattleManager::firstApproachIsMoveToFront $_2 / $_1 / $_0 (slot 0 is the shared ~__func at 0x48343C)
RW_FUNCTION(0x7544B4, "StageBattleManager::firstApproachIsMoveToFront $_2 __func::~__func (deleting)", MoveToFront2::deleteThis);
RW_FUNCTION(0x7544BC, "StageBattleManager::firstApproachIsMoveToFront $_2 __func::__clone()", MoveToFront2::clone);
RW_FUNCTION(0x7544E8, "StageBattleManager::firstApproachIsMoveToFront $_2 __func::__clone(__base*)", MoveToFront2::cloneInto);
RW_FUNCTION(0x754508, "StageBattleManager::firstApproachIsMoveToFront $_2 __func::destroy", MoveToFront2::destroy);
RW_FUNCTION(0x75450A, "StageBattleManager::firstApproachIsMoveToFront $_2 __func::destroy_deallocate", MoveToFront2::deleteThis);
RW_FUNCTION(0x754512, "StageBattleManager::firstApproachIsMoveToFront $_2", L::moveToFrontFlip);
RW_FUNCTION(0x754530, "StageBattleManager::firstApproachIsMoveToFront $_2 __func::target", MoveToFront2::target);
RW_FUNCTION(0x754550, "StageBattleManager::firstApproachIsMoveToFront $_2 __func::target_type", MoveToFront2::targetType);

RW_FUNCTION(0x754564, "StageBattleManager::firstApproachIsMoveToFront $_1 __func::~__func (deleting)", MoveToFront1::deleteThis);
RW_FUNCTION(0x75456C, "StageBattleManager::firstApproachIsMoveToFront $_1 __func::__clone()", MoveToFront1::clone);
RW_FUNCTION(0x754598, "StageBattleManager::firstApproachIsMoveToFront $_1 __func::__clone(__base*)", MoveToFront1::cloneInto);
RW_FUNCTION(0x7545B8, "StageBattleManager::firstApproachIsMoveToFront $_1 __func::destroy", MoveToFront1::destroy);
RW_FUNCTION(0x7545BA, "StageBattleManager::firstApproachIsMoveToFront $_1 __func::destroy_deallocate", MoveToFront1::deleteThis);
RW_FUNCTION(0x7545C2, "StageBattleManager::firstApproachIsMoveToFront $_1", L::moveToFrontMotion11);
RW_FUNCTION(0x7545D4, "StageBattleManager::firstApproachIsMoveToFront $_1 __func::target", MoveToFront1::target);
RW_FUNCTION(0x7545F4, "StageBattleManager::firstApproachIsMoveToFront $_1 __func::target_type", MoveToFront1::targetType);

RW_FUNCTION(0x754608, "StageBattleManager::firstApproachIsMoveToFront $_0 __func::~__func (deleting)", MoveToFront0::deleteThis);
RW_FUNCTION(0x754610, "StageBattleManager::firstApproachIsMoveToFront $_0 __func::__clone()", MoveToFront0::clone);
RW_FUNCTION(0x75463C, "StageBattleManager::firstApproachIsMoveToFront $_0 __func::__clone(__base*)", MoveToFront0::cloneInto);
RW_FUNCTION(0x75465C, "StageBattleManager::firstApproachIsMoveToFront $_0 __func::destroy", MoveToFront0::destroy);
RW_FUNCTION(0x75465E, "StageBattleManager::firstApproachIsMoveToFront $_0 __func::destroy_deallocate", MoveToFront0::deleteThis);
RW_FUNCTION(0x754666, "StageBattleManager::firstApproachIsMoveToFront $_0", L::moveToFrontMotion24);
RW_FUNCTION(0x754678, "StageBattleManager::firstApproachIsMoveToFront $_0 __func::target", MoveToFront0::target);
RW_FUNCTION(0x754698, "StageBattleManager::firstApproachIsMoveToFront $_0 __func::target_type", MoveToFront0::targetType);
