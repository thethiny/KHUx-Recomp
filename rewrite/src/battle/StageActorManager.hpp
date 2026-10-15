// StageActorManager: the actors of a quest stage. Builds the enemy platoons on the field map (symbol enemies,
// allied enemies), detects the encounter between the avatar and a symbol enemy (which team fights), keeps the
// player's attack queue (up to 6 AttackTargets: single medal attacks, whole attacks, bursts), applies the
// damage / bad status conditions of the player's attacks, the enemies' counter attacks and their hit effects,
// and runs the friend's covering fire (guilt burst) and the allies' attacks.
// Derived from BaseActorManager (cocos2d::Ref + the avatar list, the player and friend avatars, the medal deck).
#pragma once

#include "BattleCommon.hpp"

// std::function<R(A...)> (libc++, 32-bit): 16-byte in-place buffer, the callable pointer at +16 (points back
// into the buffer for small functors).
struct SamFunction {
    const uint8_t buf[16];
    rw::Field<uint32_t> f;
};
RW_SIZE(SamFunction, 20);

struct StageActorManager {
    // One queued attack of the player.
    struct AttackTarget {
        rw::Field<int32_t> type;           // +0  0 none, 1 single (one enemy), 2 whole team, 3 burst (guilt)
        rw::Field<uint32_t> medalInfo;     // +4  const MedalDeck::MedalInfo* (word 0 = the medal's SafeBuffer*)
        bt::SharedPtr skill;               // +8  std::shared_ptr of the skill row (SafeBuffer: conditions)
        rw::Field<uint32_t> enemy;         // +16 EnemyActor* target (single attacks)
        rw::Field<uint32_t> field_14;      // +20

        static AttackTarget *destruct(AttackTarget *self);
    };

    rw::Field<uint32_t> vtable;            // +0   cocos2d::Ref
    const uint8_t pad_04[16];                    // +4   Ref (refcount, script ids)
    bt::Vector avatars;                    // +20  BaseActorManager: AvatarActor* list (pause / resume)
    rw::Field<uint32_t> player;            // +32  AvatarActor* the player
    rw::Field<uint32_t> friendAvatar;      // +36  AvatarActor* the friend (covering fire), may be 0
    rw::Field<uint32_t> medalDeck;         // +40  MedalDeck*
    rw::Field<uint8_t> attackEntered;      // +44  an attack was queued this turn (not set by the constructor)
    const uint8_t pad_2d[3];
    rw::Field<int32_t> playStyle;          // +48  StageManager::PlayStyle (2 = raid)
    bt::Vector mapEnemies;                 // +52  cocos2d::Vector<EnemyActor*> symbol enemies of the field map
    bt::Vector allies;                     // +64  cocos2d::Vector<EnemyActor*> allied enemies (team flag 0x20)
    bt::Vector team;                       // +76  cocos2d::Vector<EnemyActor*> the encountered team (battle)
    bt::Vector mapNodes;                   // +88  cocos2d::Vector<MapNode*>
    rw::Field<uint8_t> friendCoveringFire; // +100 startFriendCoveringFire ran
    const uint8_t pad_65[3];
    bt::Vector crushed;                    // +104 std::vector<EnemyActor*> enemies killed this turn (money prizes)
    rw::Field<uint32_t> field;             // +116 StageFieldManager*
    rw::Field<int32_t> lastEncountTeam;    // +120 team index of the last encounter (-1 none)
    rw::Field<uint8_t> alliesJoin;         // +124 the encountered team calls the allies in
    const uint8_t pad_7d[3];
    rw::Field<int32_t> encountKind;        // +128 symbol kind of the encountered enemy (EnemyActor +756)
    rw::Field<uint32_t> tappedSymbol;      // +132 EnemyActor* symbol tapped on the map (forced encounter)
    rw::Field<int32_t> attrCount[3];       // +136 enemies of the team per attribute 1, 2, 3
    rw::Field<uint32_t> leaderSymbol;      // +148 EnemyActor* symbol enemy of the team flagged as leader
    rw::Field<uint32_t> direction;         // +152 CommonDirection* (+628 = its node: damage numbers, actors)
    rw::Field<uint32_t> scene;             // +156 SceneBattleBase*
    AttackTarget targets[6];               // +160 the attack queue
    rw::Field<int32_t> targetCapacity;     // +304 6
    rw::Field<int32_t> targetCount;        // +308 entries queued
    rw::Field<int32_t> targetExecuted;     // +312 entries already executed
    rw::Field<uint32_t> cutInShader;       // +316 GuiltBurstCutInShader*
    AttackTarget currentTarget;            // +320 the attack being executed
    SamFunction canTapEnemy;               // +344 std::function<bool()>
    const uint8_t pad_15c[4];
    SamFunction onTapEnemy;                // +368 std::function<void()>
    const uint8_t pad_174[4];
    rw::Field<int32_t> coveringFireState;  // +392 friend covering fire step (0 idle, 1..6)
    rw::Field<int32_t> coveringFireType;   // +396 CoveringFireType (1 = jump attack)
    SamFunction onEnemyAttack;             // +400 std::function<void(bool)>
    const uint8_t pad_1a4[4];
    rw::Field<uint32_t> jumpTarget;        // +424 EnemyActor* that needs a jump attack
    rw::Field<float> jumpOffset;           // +428 x distance kept from the jump target

    static constexpr uint32_t VTABLE = 0xFA82AC;      // _ZTV17StageActorManager + 8
    static constexpr uint32_t NULL_TARGET = 0x1003FE0; // StageActorManager::NullTarget

    static StageActorManager *create();
    static StageActorManager *construct(StageActorManager *self);
    static StageActorManager *destruct(StageActorManager *self);
    static void destructDelete(StageActorManager *self);
    static uint32_t init(StageActorManager *self);
    static void setupEnemyActorPlatoon(StageActorManager *self, uint32_t fieldManager, bt::Vector *teams);
    static void showWanderingSymbolEnemy(StageActorManager *self);
    static bool checkEncount(StageActorManager *self, uint32_t stageManager, uint32_t avatar);
    static uint32_t checkEncountOrder(StageActorManager *self, uint32_t stageManager);
    static uint32_t getLastEncountTeamUniqueID(StageActorManager *self, uint32_t stageManager);
    static uint32_t getLastEncountTeamInfo(StageActorManager *self, uint32_t stageManager);
    static uint32_t getCurrentEncountSymbolEnemyActor(StageActorManager *self);
    static void clearCurrentTarget(StageActorManager *self);
    static uint32_t entryWholeAttack(StageActorManager *self, uint32_t medalInfo);
    static uint32_t entryCurrentTarget(StageActorManager *self, const AttackTarget *target);
    static uint32_t hasAliveEnemyInTeam(StageActorManager *self);
    static uint32_t isAnnihilationMapEnemy(StageActorManager *self);
    static uint32_t hasActiveAlly(StageActorManager *self);
    static AttackTarget *executeAttackCurrentTarget(StageActorManager *self);
    static uint32_t choiceTargetAttributeWithLowerHp(StageActorManager *self, int32_t attribute);
    static void autoChoiceTarget(StageActorManager *self, uint32_t medalInfo, uint32_t burst);
    static void changeMode(StageActorManager *self, uint32_t mode);
    static void attackReadyAction(StageActorManager *self, uint32_t raidBoss);
    static void turnEndAction(StageActorManager *self, uint32_t money, uint32_t keepMarkers, uint32_t noMarker);
    static void enebleChoiceMarker(StageActorManager *self);
    static uint32_t battleFinishAction(StageActorManager *self, uint32_t bonus);
    static void update(StageActorManager *self, float dt);
    static void updateAvatar(StageActorManager *self, uint32_t avatar, const AttackTarget *target);
    static void updateEnemy(StageActorManager *self, uint32_t enemy, uint32_t avatar);
    static void updateEnemyEnemy(StageActorManager *self, uint32_t enemy, uint32_t target);
    static void deployHitEffect(StageActorManager *self, int32_t id, uint32_t pos, float offsetY, float dir);
    static void getEncountTargets(bt::Vector *out, StageActorManager *self, const AttackTarget *target,
                                  uint32_t aliveOnly);
    static void computeAttackPosition(bt::Vec2 *out, const StageActorManager *self, const AttackTarget *target,
                                      const float *scale);
    static bool isSingle(StageActorManager *self, const AttackTarget *target);
    static void EnemyDamage(StageActorManager *self, uint32_t enemy, bt::SharedPtr *burst, uint32_t bursted);
    static bool isRandom(StageActorManager *self, const AttackTarget *target);
    static void SelectRandom(StageActorManager *self);
    static void storeDamage(StageActorManager *self, uint32_t avatar, const AttackTarget *target);
    static void addCondition(StageActorManager *self, const AttackTarget *target, uint32_t enemy);
    static void reflectionCondition(StageActorManager *self, uint32_t avatar, const AttackTarget *target);
    static bool onTapBeganEnemyActor(StageActorManager *self, uint32_t enemy);
    static uint32_t onTapEndedEnemyActor(StageActorManager *self, uint32_t medalInfo, uint32_t enemy);
    static void startFriendCoveringFire(StageActorManager *self);
    static uint32_t isEndFriendSummons(StageActorManager *self);
    static void SetFriendBurst(StageActorManager *self);
    static void initFriendCoveringFireAction(StageActorManager *self, int32_t type);
    static uint32_t execFriendCoveringFireAction(StageActorManager *self, uint32_t player, uint32_t friendAv);
    static void doJumpUpAction(StageActorManager *self, uint32_t avatar);
    static void doJumpFallAction(StageActorManager *self, uint32_t avatar);
    static bool isFriendCoveringFireCameraTargetPlayer(StageActorManager *self);
    static uint32_t findNeedJumpAttackEnemy(StageActorManager *self, const AttackTarget *target);
    static bool isJumpAttackAction(StageActorManager *self, const AttackTarget *target, float offset);
    static void doJumpApprpachAction(StageActorManager *self, uint32_t avatar);
    static void startAllyAttack(StageActorManager *self, float dt);
    static bool isEndAllyAttack(StageActorManager *self);
    static void doAttackAllyActor(StageActorManager *self, uint32_t enemy, float dt);
    static uint32_t doDisengageAttackEnd(StageActorManager *self, uint32_t avatar, const AttackTarget *target);
    static void pauseToAllActor(StageActorManager *self);
    static void resumeToAllActor(StageActorManager *self);
};
RW_SIZE(StageActorManager::AttackTarget, 24);
RW_LAYOUT(StageActorManager::AttackTarget, skill, 8);
RW_LAYOUT(StageActorManager::AttackTarget, enemy, 16);
RW_LAYOUT(StageActorManager, avatars, 20);
RW_LAYOUT(StageActorManager, player, 32);
RW_LAYOUT(StageActorManager, medalDeck, 40);
RW_LAYOUT(StageActorManager, attackEntered, 44);
RW_LAYOUT(StageActorManager, playStyle, 48);
RW_LAYOUT(StageActorManager, mapEnemies, 52);
RW_LAYOUT(StageActorManager, allies, 64);
RW_LAYOUT(StageActorManager, team, 76);
RW_LAYOUT(StageActorManager, mapNodes, 88);
RW_LAYOUT(StageActorManager, friendCoveringFire, 100);
RW_LAYOUT(StageActorManager, crushed, 104);
RW_LAYOUT(StageActorManager, field, 116);
RW_LAYOUT(StageActorManager, lastEncountTeam, 120);
RW_LAYOUT(StageActorManager, alliesJoin, 124);
RW_LAYOUT(StageActorManager, tappedSymbol, 132);
RW_LAYOUT(StageActorManager, attrCount, 136);
RW_LAYOUT(StageActorManager, leaderSymbol, 148);
RW_LAYOUT(StageActorManager, direction, 152);
RW_LAYOUT(StageActorManager, scene, 156);
RW_LAYOUT(StageActorManager, targets, 160);
RW_LAYOUT(StageActorManager, targetCapacity, 304);
RW_LAYOUT(StageActorManager, cutInShader, 316);
RW_LAYOUT(StageActorManager, currentTarget, 320);
RW_LAYOUT(StageActorManager, canTapEnemy, 344);
RW_LAYOUT(StageActorManager, onTapEnemy, 368);
RW_LAYOUT(StageActorManager, coveringFireState, 392);
RW_LAYOUT(StageActorManager, onEnemyAttack, 400);
RW_LAYOUT(StageActorManager, jumpTarget, 424);
RW_SIZE(StageActorManager, 432);
