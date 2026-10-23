// RaidBattle: one attack exchange of a raid battle (a cocos2d::Node, 0x330 bytes). Field offsets in
// raid_battle; the functions take the guest address of the object.
#pragma once

#include "BattleUi.hpp"

namespace raid_battle {
using scene::Frame;
using scene::call;
using scene::ld32;
using scene::ldf;
using scene::st32;
using scene::vcall;

constexpr uint32_t SIZE = 0x330;
constexpr uint32_t SCENE = 624;          // SceneRaidBattle*
constexpr uint32_t AVATAR = 628;         // AvatarActor* attacking
constexpr uint32_t BOSS = 632;           // RaidBossActor*
constexpr uint32_t CAMERA_NODE = 636;
constexpr uint32_t DIRECTION = 644;      // RaidDirection*
constexpr uint32_t FIELD_OWNER = 648;    // its +628 is the node damage numbers go to
constexpr uint32_t STATE = 652;          // the update state machine
constexpr uint32_t OFFSET = 660;         // Vec2: target minus boss position
constexpr uint32_t SIDE = 668;           // 1 / 2: the target is near / far from the boss's height
constexpr uint32_t F_676 = 676;
constexpr uint32_t SKILL = 680;          // the boss skill in use
constexpr uint32_t COOLDOWN = 684;       // int[4]: turns until a cool-down skill is usable
constexpr uint32_t SKILL_USED = 700;     // uint8[4]: once-only skills spent
constexpr uint32_t AVATAR_POS = 708;     // Vec2: where the attacker started
constexpr uint32_t F_716 = 716, F_720 = 720, F_724 = 724, F_732 = 732, F_736 = 736, F_740 = 740, F_748 = 748;
constexpr uint32_t TARGET_POS = 756;     // Vec2
constexpr uint32_t BATTLE_SEQUENCE = 764;   // uint8: SceneRaidBattle::advanceBattleSequence
constexpr uint32_t TARGET_TYPE = 772;    // StageActorManager::AttackTarget: type (1 chain, 2 plain, 3 burst)
constexpr uint32_t MEDAL_INFO = 776;     // AttackTarget: MedalDeck::MedalInfo*
constexpr uint32_t FEVER = 796;          // damage dealt so far: the fever level
constexpr uint32_t SHADER = 808;         // GuiltBurstCutInShader*
}  // namespace raid_battle

struct RaidBattle {
    static uint32_t construct(uint32_t self);
    static uint32_t create();
    static void start(uint32_t self, uint32_t attacker, uint32_t boss, uint32_t camera, uint32_t scene,
                      uint32_t target, uint32_t type);
    static void startFriend(uint32_t self, uint32_t attacker, uint32_t boss, uint32_t camera, uint32_t scene,
                            uint32_t target, uint32_t type);
    static void set_week_position(uint32_t self);
    static uint32_t ChainAttack(uint32_t self);
    static void update(uint32_t self, float dt);
    static void moveCamera(uint32_t self, float seconds, uint32_t target, float scale, uint32_t withField);
    static void avatarAttack(uint32_t self);
    static void RaidDamage(uint32_t self);
    static void reflectionCondition(uint32_t self);
    static void go_back(uint32_t self);
    static void resetAttackDamage(uint32_t self);
    static void enemyAttack(uint32_t self);
    static void addCondition(uint32_t self, uint32_t target, uint32_t boss);
    static void RaidBossEndSequence(uint32_t self);
    static uint32_t use_skill_check(uint32_t self, int32_t i, float teamRate);
    static void use_skill(uint32_t self, int32_t i);
    static uint32_t setRaidEnemyPoisonEffect(uint32_t self);
};
