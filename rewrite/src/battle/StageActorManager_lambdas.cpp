// StageActorManager: the std::function plumbing of its lambdas (libc++ __func slots: deleting destructor, clone, clone into a
// buffer, destroy, destroy_deallocate, target, target_type), generated from the binary's __func vtables (size,
// captured bytes, type_info, type name and target offset read from each slot's instructions), plus the lambda bodies.
#include "../scene_main/FunctionThunks.hpp"

namespace StageActorManager_w3 {

// StageActorManager::doDisengageAttackEnd::$_10
using doDisengageAttackEnd_10 = thunk::Lambda<0xF6DEE8, 8, 0xFA82DC, 0xE9DE00, 4, thunk::cap(4, 4)>;
// StageActorManager::doDisengageAttackEnd::$_9
using doDisengageAttackEnd_9 = thunk::Lambda<0xF6DF14, 8, 0xFA82F0, 0xE9DEF0, 4, thunk::cap(4, 4)>;
// StageActorManager::startAllyAttack::$_8
using startAllyAttack_8 = thunk::Lambda<0xF6DF40, 16, 0xFA8304, 0xE9DFC0, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;
// StageActorManager::doJumpFallAction::$_7
using doJumpFallAction_7 = thunk::Lambda<0xF6DF6C, 8, 0xFA8318, 0xE9E060, 4, thunk::cap(4, 4)>;
// StageActorManager::doJumpFallAction::$_6
using doJumpFallAction_6 = thunk::Lambda<0xF6DF98, 8, 0xFA832C, 0xE9E110, 4, thunk::cap(4, 4)>;
// StageActorManager::doJumpUpAction::$_5
using doJumpUpAction_5 = thunk::Lambda<0xF6DFC4, 8, 0xFA8340, 0xE9E1C0, 4, thunk::cap(4, 4)>;
// StageActorManager::doJumpApprpachAction::$_4
using doJumpApprpachAction_4 = thunk::Lambda<0xF6DFF0, 16, 0xFA8354, 0xE9E280, 4, thunk::cap(4, 4), thunk::cap(8, 4), thunk::cap(12, 4)>;

}  // namespace StageActorManager_w3

RW_FUNCTION(0x752D14, "StageActorManager::doDisengageAttackEnd::$_10 ~__func() (deleting)", StageActorManager_w3::doDisengageAttackEnd_10::deleteThis);
RW_FUNCTION(0x752D1C, "StageActorManager::doDisengageAttackEnd::$_10 __clone()", StageActorManager_w3::doDisengageAttackEnd_10::clone);
RW_FUNCTION(0x752D48, "StageActorManager::doDisengageAttackEnd::$_10 __clone(__base*)", StageActorManager_w3::doDisengageAttackEnd_10::cloneInto);
RW_FUNCTION(0x752D68, "StageActorManager::doDisengageAttackEnd::$_10 destroy()", StageActorManager_w3::doDisengageAttackEnd_10::destroy);
RW_FUNCTION(0x752D6A, "StageActorManager::doDisengageAttackEnd::$_10 destroy_deallocate()", StageActorManager_w3::doDisengageAttackEnd_10::deleteThis);
RW_FUNCTION(0x752D84, "StageActorManager::doDisengageAttackEnd::$_10 target(const type_info&)", StageActorManager_w3::doDisengageAttackEnd_10::target);
RW_FUNCTION(0x752DA4, "StageActorManager::doDisengageAttackEnd::$_10 target_type()", StageActorManager_w3::doDisengageAttackEnd_10::targetType);
RW_FUNCTION(0x752DB8, "StageActorManager::doDisengageAttackEnd::$_9 ~__func() (deleting)", StageActorManager_w3::doDisengageAttackEnd_9::deleteThis);
RW_FUNCTION(0x752DC0, "StageActorManager::doDisengageAttackEnd::$_9 __clone()", StageActorManager_w3::doDisengageAttackEnd_9::clone);
RW_FUNCTION(0x752DEC, "StageActorManager::doDisengageAttackEnd::$_9 __clone(__base*)", StageActorManager_w3::doDisengageAttackEnd_9::cloneInto);
RW_FUNCTION(0x752E0C, "StageActorManager::doDisengageAttackEnd::$_9 destroy()", StageActorManager_w3::doDisengageAttackEnd_9::destroy);
RW_FUNCTION(0x752E0E, "StageActorManager::doDisengageAttackEnd::$_9 destroy_deallocate()", StageActorManager_w3::doDisengageAttackEnd_9::deleteThis);
RW_FUNCTION(0x752E28, "StageActorManager::doDisengageAttackEnd::$_9 target(const type_info&)", StageActorManager_w3::doDisengageAttackEnd_9::target);
RW_FUNCTION(0x752E48, "StageActorManager::doDisengageAttackEnd::$_9 target_type()", StageActorManager_w3::doDisengageAttackEnd_9::targetType);
RW_FUNCTION(0x752E5C, "StageActorManager::startAllyAttack::$_8 ~__func() (deleting)", StageActorManager_w3::startAllyAttack_8::deleteThis);
RW_FUNCTION(0x752E64, "StageActorManager::startAllyAttack::$_8 __clone()", StageActorManager_w3::startAllyAttack_8::clone);
RW_FUNCTION(0x752E94, "StageActorManager::startAllyAttack::$_8 __clone(__base*)", StageActorManager_w3::startAllyAttack_8::cloneInto);
RW_FUNCTION(0x752EBC, "StageActorManager::startAllyAttack::$_8 destroy()", StageActorManager_w3::startAllyAttack_8::destroy);
RW_FUNCTION(0x752EBE, "StageActorManager::startAllyAttack::$_8 destroy_deallocate()", StageActorManager_w3::startAllyAttack_8::deleteThis);
RW_FUNCTION(0x752ED8, "StageActorManager::startAllyAttack::$_8 target(const type_info&)", StageActorManager_w3::startAllyAttack_8::target);
RW_FUNCTION(0x752EF8, "StageActorManager::startAllyAttack::$_8 target_type()", StageActorManager_w3::startAllyAttack_8::targetType);
RW_FUNCTION(0x752F0C, "StageActorManager::doJumpFallAction::$_7 ~__func() (deleting)", StageActorManager_w3::doJumpFallAction_7::deleteThis);
RW_FUNCTION(0x752F14, "StageActorManager::doJumpFallAction::$_7 __clone()", StageActorManager_w3::doJumpFallAction_7::clone);
RW_FUNCTION(0x752F40, "StageActorManager::doJumpFallAction::$_7 __clone(__base*)", StageActorManager_w3::doJumpFallAction_7::cloneInto);
RW_FUNCTION(0x752F60, "StageActorManager::doJumpFallAction::$_7 destroy()", StageActorManager_w3::doJumpFallAction_7::destroy);
RW_FUNCTION(0x752F62, "StageActorManager::doJumpFallAction::$_7 destroy_deallocate()", StageActorManager_w3::doJumpFallAction_7::deleteThis);
RW_FUNCTION(0x752F88, "StageActorManager::doJumpFallAction::$_7 target(const type_info&)", StageActorManager_w3::doJumpFallAction_7::target);
RW_FUNCTION(0x752FA8, "StageActorManager::doJumpFallAction::$_7 target_type()", StageActorManager_w3::doJumpFallAction_7::targetType);
RW_FUNCTION(0x752FBC, "StageActorManager::doJumpFallAction::$_6 ~__func() (deleting)", StageActorManager_w3::doJumpFallAction_6::deleteThis);
RW_FUNCTION(0x752FC4, "StageActorManager::doJumpFallAction::$_6 __clone()", StageActorManager_w3::doJumpFallAction_6::clone);
RW_FUNCTION(0x752FF0, "StageActorManager::doJumpFallAction::$_6 __clone(__base*)", StageActorManager_w3::doJumpFallAction_6::cloneInto);
RW_FUNCTION(0x753010, "StageActorManager::doJumpFallAction::$_6 destroy()", StageActorManager_w3::doJumpFallAction_6::destroy);
RW_FUNCTION(0x753012, "StageActorManager::doJumpFallAction::$_6 destroy_deallocate()", StageActorManager_w3::doJumpFallAction_6::deleteThis);
RW_FUNCTION(0x75302C, "StageActorManager::doJumpFallAction::$_6 target(const type_info&)", StageActorManager_w3::doJumpFallAction_6::target);
RW_FUNCTION(0x75304C, "StageActorManager::doJumpFallAction::$_6 target_type()", StageActorManager_w3::doJumpFallAction_6::targetType);
RW_FUNCTION(0x753060, "StageActorManager::doJumpUpAction::$_5 ~__func() (deleting)", StageActorManager_w3::doJumpUpAction_5::deleteThis);
RW_FUNCTION(0x753068, "StageActorManager::doJumpUpAction::$_5 __clone()", StageActorManager_w3::doJumpUpAction_5::clone);
RW_FUNCTION(0x753094, "StageActorManager::doJumpUpAction::$_5 __clone(__base*)", StageActorManager_w3::doJumpUpAction_5::cloneInto);
RW_FUNCTION(0x7530B4, "StageActorManager::doJumpUpAction::$_5 destroy()", StageActorManager_w3::doJumpUpAction_5::destroy);
RW_FUNCTION(0x7530B6, "StageActorManager::doJumpUpAction::$_5 destroy_deallocate()", StageActorManager_w3::doJumpUpAction_5::deleteThis);
RW_FUNCTION(0x7530D0, "StageActorManager::doJumpUpAction::$_5 target(const type_info&)", StageActorManager_w3::doJumpUpAction_5::target);
RW_FUNCTION(0x7530F0, "StageActorManager::doJumpUpAction::$_5 target_type()", StageActorManager_w3::doJumpUpAction_5::targetType);
RW_FUNCTION(0x753104, "StageActorManager::doJumpApprpachAction::$_4 ~__func() (deleting)", StageActorManager_w3::doJumpApprpachAction_4::deleteThis);
RW_FUNCTION(0x75310C, "StageActorManager::doJumpApprpachAction::$_4 __clone()", StageActorManager_w3::doJumpApprpachAction_4::clone);
RW_FUNCTION(0x75313C, "StageActorManager::doJumpApprpachAction::$_4 __clone(__base*)", StageActorManager_w3::doJumpApprpachAction_4::cloneInto);
RW_FUNCTION(0x753164, "StageActorManager::doJumpApprpachAction::$_4 destroy()", StageActorManager_w3::doJumpApprpachAction_4::destroy);
RW_FUNCTION(0x753166, "StageActorManager::doJumpApprpachAction::$_4 destroy_deallocate()", StageActorManager_w3::doJumpApprpachAction_4::deleteThis);
RW_FUNCTION(0x7531AC, "StageActorManager::doJumpApprpachAction::$_4 target(const type_info&)", StageActorManager_w3::doJumpApprpachAction_4::target);
RW_FUNCTION(0x7531CC, "StageActorManager::doJumpApprpachAction::$_4 target_type()", StageActorManager_w3::doJumpApprpachAction_4::targetType);

// ------------------------------------------------------------------------------------------------ lambda bodies
namespace StageActorManager_w3 {
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;      // Avatar::changeMotion(Motion)
constexpr uint32_t MapNode_lockMapPosZ = 0x5DA575;      // MapNode::lockMapPosZ(float z)
constexpr uint32_t MapNode_unlockMapPosZ = 0x5DA381;
constexpr uint32_t StageActorManager_doAttackAllyActor = 0x75296D;   // (EnemyActor*, float)
constexpr uint32_t ACTOR_AVATAR = 0x288;                // AvatarActor / EnemyActor: its Avatar (the animated body)
constexpr uint32_t V_ACTOR_0x28 = 0x28;                 // Node virtual slot 0x28 (setScaleX)

inline uint32_t cap(uint32_t lambda, uint32_t off) { return rw::load<uint32_t>(lambda + off); }
inline void motion(uint32_t actor, uint32_t m) { rw::call(Avatar_changeMotion, {rw::load<uint32_t>(actor + ACTOR_AVATAR), m}); }

// doDisengageAttackEnd::$_10 {actor}: back to motion 11
void disengageEndMotion11(uint32_t lambda) { motion(cap(lambda, 4), 0xB); }
// doDisengageAttackEnd::$_9 {actor}: motion 7
void disengageEndMotion7(uint32_t lambda) { motion(cap(lambda, 4), 7); }
// startAllyAttack::$_8 {manager, enemy, delay}: the ally's attack lands
void allyAttack(uint32_t lambda) {
    rw::call(StageActorManager_doAttackAllyActor, {cap(lambda, 4), cap(lambda, 8), cap(lambda, 12)});
}
// doJumpFallAction::$_7 {actor}: landed - motion 11, release the map z lock
void jumpFallLanded(uint32_t lambda) {
    motion(cap(lambda, 4), 0xB);
    rw::call(MapNode_unlockMapPosZ, {cap(lambda, 4)});
}
// doJumpFallAction::$_6 {actor}: motion 18 (falling)
void jumpFallMotion(uint32_t lambda) { motion(cap(lambda, 4), 0x12); }
// doJumpUpAction::$_5 {actor}: motion 5 (jump up)
void jumpUpMotion(uint32_t lambda) { motion(cap(lambda, 4), 5); }
// doJumpApprpachAction::$_4 {actor, value, target}: slot 0x28 with the captured value, lock the actor's map z just
// above the target (target z + 10), motion 8
void jumpApproach(uint32_t lambda) {
    uint32_t actor = cap(lambda, 4);
    rw::vcall(rw::host_ptr<uint32_t>(actor), V_ACTOR_0x28 / 4, {actor, cap(lambda, 8)});
    float z = rw::load<float>(cap(lambda, 12) + 0x274) + 10.0f;
    rw::call(MapNode_lockMapPosZ, {cap(lambda, 4), rw::fbits(z)});
    motion(cap(lambda, 4), 8);
}
}  // namespace StageActorManager_w3

RW_FUNCTION(0x752D72, "StageActorManager::doDisengageAttackEnd::$_10 operator()", StageActorManager_w3::disengageEndMotion11);
RW_FUNCTION(0x752E16, "StageActorManager::doDisengageAttackEnd::$_9 operator()", StageActorManager_w3::disengageEndMotion7);
RW_FUNCTION(0x752EC6, "StageActorManager::startAllyAttack::$_8 operator()", StageActorManager_w3::allyAttack);
RW_FUNCTION(0x752F6A, "StageActorManager::doJumpFallAction::$_7 operator()", StageActorManager_w3::jumpFallLanded);
RW_FUNCTION(0x75301A, "StageActorManager::doJumpFallAction::$_6 operator()", StageActorManager_w3::jumpFallMotion);
RW_FUNCTION(0x7530BE, "StageActorManager::doJumpUpAction::$_5 operator()", StageActorManager_w3::jumpUpMotion);
RW_FUNCTION(0x753170, "StageActorManager::doJumpApprpachAction::$_4 operator()", StageActorManager_w3::jumpApproach);
