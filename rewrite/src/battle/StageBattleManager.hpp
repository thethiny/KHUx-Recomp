// StageBattleManager: per-battle enemy-team logic of a quest stage: which enemies attack this turn (shuffled
// attacker list), team-wide checks (annihilated, calmed down, lowest HP rate), the approach movement of enemies
// on the field screen and the "select a target" mode (several approaching enemies wait for the player's choice).
#pragma once

#include "BattleCommon.hpp"

struct StageBattleManager {
    rw::Field<uint32_t> vtable;            // +0  cocos2d::Ref
    const uint8_t pad_04[16];                    // +4  Ref (refcount, script ids)
    rw::Field<int32_t> attackerCount;      // +20 attackers left this turn (not set by the constructor)
    bt::Vector attackers;                  // +24 cocos2d::Vector<EnemyActor*>
    rw::Field<uint32_t> selectEnemies;     // +36 cocos2d::Vector<EnemyActor*>* of the select-target mode
    rw::Field<uint32_t> selectedTarget;    // +40 EnemyActor* chosen (0 = still selecting)
    rw::Field<float> approachTimer;        // +44 seconds before approaching enemies may push each other

    static constexpr uint32_t VTABLE = 0xFA83BC;   // _ZTV18StageBattleManager + 8

    static StageBattleManager *create();
    static StageBattleManager *construct(StageBattleManager *self);
    static StageBattleManager *destruct(StageBattleManager *self);
    static void destructDelete(StageBattleManager *self);
    static uint32_t init(StageBattleManager *self);
    static uint32_t isEnemyTeamAnnihilation(StageBattleManager *self, bt::Vector *enemies);
    static uint32_t isEnemyTeamCalmDown(StageBattleManager *self, bt::Vector *enemies);
    static void setEnemyAttacker(StageBattleManager *self, bt::Vector *enemies);
    static uint32_t IsAllOverEnemyAttker(StageBattleManager *self, bt::Vector *enemies);
    static float MinHPRateEnemyTeam(StageBattleManager *self, bt::Vector *enemies);
    static uint32_t hasEnemyAttker(StageBattleManager *self);
    static uint32_t getEnemyAttcker(StageBattleManager *self);
    static void clearEnemyAttker(StageBattleManager *self);
    static void initApproachEnemyTeam(StageBattleManager *self, bt::Vector *enemies);
    static void approachEnemyTeam(StageBattleManager *self, float dt, uint32_t target, bt::Vector *enemies,
                                  uint32_t fieldManager);
    static uint32_t firstApproachIsMoveToFront(StageBattleManager *self, uint32_t avatar, uint32_t enemy);
    static void startSelectedTargetEnemy(StageBattleManager *self, bt::Vector *enemies);
    static uint32_t isSelectedTargetEnemy(StageBattleManager *self);
    static uint32_t selectedTargetEnemy(StageBattleManager *self, uint32_t enemy, uint32_t select);
    static void checkSubMission12(StageBattleManager *self, bt::Vector *enemies);
};
RW_LAYOUT(StageBattleManager, attackerCount, 20);
RW_LAYOUT(StageBattleManager, attackers, 24);
RW_LAYOUT(StageBattleManager, selectEnemies, 36);
RW_LAYOUT(StageBattleManager, approachTimer, 44);
RW_SIZE(StageBattleManager, 48);
