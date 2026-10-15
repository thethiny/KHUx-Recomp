// SceneBattleBase: the common base of the quest and raid battle scenes (a cocos2d::Layer). It owns the stage
// managers (actor manager, field manager, StageUI), the touch / swipe state of the battle input, and the player
// side of a turn: invoking medal skills, HP and special gauge (CP) changes, and the per-turn buff / status ticks
// (the medal deck's turn callbacks set in init_UI).
#pragma once

#include "BattleUi.hpp"

struct SceneBattleBase {
    rw::Field<uint32_t> vtable;            // +0   cocos2d::Layer
    const uint8_t pad_004[644];                  // +4
    const uint8_t gesture[48];                   // +648 GestureRecognizer (swipe detection)
    rw::Field<uint32_t> stageUI;           // +696 StageUI*
    rw::Field<uint32_t> actorManager;      // +700 StageActorManager* (avatar actor at +32)
    rw::Field<uint32_t> fieldManager;      // +704 StageFieldManager*
    rw::Field<uint32_t> managers[3];       // +708 cocos2d::Vector<Ref*>: retains the three managers
    rw::Field<uint8_t> ready;              // +720 set by init: the menu button may open the menu
    rw::Field<uint8_t> menuOpen;           // +721 the battle menu is open (actors paused)
    rw::Field<uint8_t> menuAllowed;        // +722 (1 at start)
    const uint8_t pad_2D3[1];                    // +723
    rw::Field<int32_t> touchState;         // +724 1 = a touch began on the field
    rw::Field<float> touchTime;            // +728 0.5 when a touch begins
    rw::Field<uint8_t> touchIdle;          // +732 1 = no touch in progress
    rw::Field<uint8_t> tapped;             // +733 the last touch was a tap
    rw::Field<uint8_t> swiped;             // +734 the last touch became a swipe
    rw::Field<uint8_t> touchHeld;          // +735

    static constexpr uint32_t VTABLE = 0xF9680C;          // _ZTV15SceneBattleBase + 8
    // SceneBattleBase virtuals used by the turn callbacks (byte offsets into the vtable)
    static constexpr uint32_t VSLOT_ON_DEFENSE_TURN = 0x300;  // after the defence buffs tick
    static constexpr uint32_t VSLOT_ON_ATTACK_TURN = 0x304;   // after the attack buffs / statuses tick
    static constexpr uint32_t VSLOT_ON_SKILL = 0x308;         // (skillId): a medal skill was invoked

    static SceneBattleBase *construct(SceneBattleBase *self);
    static SceneBattleBase *destroy(SceneBattleBase *self);
    static void destroyDelete(SceneBattleBase *self);
    static uint32_t init(SceneBattleBase *self);
    static uint32_t init_UI(SceneBattleBase *self);
    static SceneBattleBase *onAttackButton(SceneBattleBase *self, int32_t type);
    static uint32_t onTouchBeganBattle(SceneBattleBase *self, uint32_t touch, uint32_t event);
    static void onTouchMovedBattle(SceneBattleBase *self, uint32_t touch, uint32_t event);
    static SceneBattleBase *onTouchEndedBattle(SceneBattleBase *self, uint32_t touch, uint32_t event);
    static SceneBattleBase *onTouchCancelledBattle(SceneBattleBase *self, uint32_t touch, uint32_t event);
    static uint32_t onSwipeBattle(SceneBattleBase *self, uint32_t gesture, uint32_t delta, uint32_t direction);
    static uint32_t enemy_count_end_enemy(SceneBattleBase *self, uint32_t enemy);
    static uint32_t enemy_count_end_raidBoss(SceneBattleBase *self, uint32_t boss);
    static uint32_t onMenuButton(SceneBattleBase *self, uint32_t playStyle);
    static uint32_t invokeAttackSkill(SceneBattleBase *self, uint32_t target);
    static uint32_t invokeSkill(SceneBattleBase *self, int32_t timing);
    static int32_t DamageSkill(SceneBattleBase *self);
    static uint32_t invoketSkill(bt::SharedPtr *out, SceneBattleBase *self, uint32_t medal, int32_t timing,
                                 uint32_t flag1, uint32_t flag2);
    static uint32_t setPlayerDamage(SceneBattleBase *self, int32_t damage);
    static uint32_t addPlayerHP(SceneBattleBase *self, int32_t amount);
    static uint32_t addPlayerCP(SceneBattleBase *self, int32_t amount);
    static uint32_t subPlayerCP(SceneBattleBase *self, uint32_t cost, uint32_t medal);
    static uint32_t isBurstEnoughCP(SceneBattleBase *self);
    static uint32_t firstSetPlayerCP(SceneBattleBase *self, int32_t amount, int32_t keybladeId);
};
RW_LAYOUT(SceneBattleBase, gesture, 648);
RW_LAYOUT(SceneBattleBase, stageUI, 696);
RW_LAYOUT(SceneBattleBase, managers, 708);
RW_LAYOUT(SceneBattleBase, ready, 720);
RW_LAYOUT(SceneBattleBase, touchState, 724);
RW_LAYOUT(SceneBattleBase, touchHeld, 735);
