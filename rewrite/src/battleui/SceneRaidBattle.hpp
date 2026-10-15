// SceneRaidBattle: the raid boss battle scene (a SceneBattleBase). One boss (RaidBossActor, made of up to 6
// parts) on a camera node that RaidBattle pans and zooms; the player's avatar and the friend avatar attack it with
// the medal deck. The scene runs a small turn state machine in update() (battleState): intro, heading, player turn,
// player attacks (RaidBattle), enemy turn, the raid-clear request and the move to the battle result. A tutorial
// variant (isTutorial) plays a scripted fight with no server request and ends with a movie.
//
// The SceneBattleBase part of the layout is declared here as far as this scene touches it (that class is
// rewritten separately).
#pragma once

#include "BattleUi.hpp"

struct SceneRaidBattle {
    // ---- SceneBattleBase (cocos2d::Layer ... )
    rw::Field<uint32_t> vtable;                  // +0x000
    const uint8_t pad_004[0x210 - 0x004];
    const uint8_t onStart[20];                         // +0x210 std::function<void()> (base: run when the scene starts)
    rw::Field<uint32_t> pad_224;
    const uint8_t onFinish[20];                        // +0x228 std::function<void()> (base: run when the scene ends)
    const uint8_t pad_23C[0x2B8 - 0x23C];
    rw::Field<uint32_t> stageUI;                 // +0x2B8 StageUI*
    rw::Field<uint32_t> actorManager;            // +0x2BC StageActorManager*
    rw::Field<uint32_t> fieldManager;            // +0x2C0 StageFieldManager*
    rw::Field<uint32_t> retained[3];             // +0x2C4 cocos2d::Vector<Ref*>: objects kept alive by the scene
    rw::Field<uint32_t> pad_2D0;
    rw::Field<int32_t> triggerMode;              // +0x2D4 1: the attack trigger timer runs
    rw::Field<float> triggerTimer;               // +0x2D8 seconds left before the automatic attack trigger
    rw::Field<uint8_t> triggerRunning;           // +0x2DC
    rw::Field<uint8_t> triggerFired;             // +0x2DD the timer ran out: battleTrgUI on the next update
    rw::Field<uint8_t> field_2DE;                // +0x2DE
    rw::Field<uint8_t> field_2DF;                // +0x2DF
    rw::Field<int32_t> battleState;              // +0x2E0 the turn state machine (see update)
    rw::Field<int32_t> waitFrames;               // +0x2E4 frames to wait in the current state
    const uint8_t pad_2E8[0x2F8 - 0x2E8];
    // ---- SceneRaidBattle
    rw::Field<float> cameraX;                    // +0x2F8 cocos2d::Vec2: centre of the camera node
    rw::Field<float> cameraY;                    // +0x2FC
    rw::Field<uint32_t> camera;                  // +0x300 cocos2d::Node* holding the boss, avatars, background
    rw::Field<uint32_t> raidBattle;              // +0x304 RaidBattle*
    rw::Field<uint32_t> boss;                    // +0x308 RaidBossActor*
    rw::Field<uint32_t> raidDirection;           // +0x30C RaidDirection*
    rw::Field<uint32_t> commonDirection;         // +0x310 CommonDirection*
    rw::Field<uint32_t> pad_314;
    const uint8_t gesture[0x348 - 0x318];              // +0x318 GestureRecognizer (onSwipe std::function at +0x18)
    rw::Field<uint32_t> cutInShader;             // +0x348 GuiltBurstCutInShader*
    rw::Field<uint8_t> partyRescue;              // +0x34C cleared by the raid info's "joined" flag / the tutorial
    rw::Field<uint8_t> markerActive;             // +0x34D the boss target markers take taps
    rw::Field<uint8_t> deckEnabled;              // +0x34E the medal deck takes input (a CarouselDeck predicate)
    rw::Field<uint8_t> isTutorial;               // +0x34F scripted tutorial raid
    rw::Field<int32_t> movieId;                  // +0x350 tutorial: the movie played at the end (incremented)
    rw::Field<uint8_t> movieFlag;                // +0x354 tutorial: start_movie's flag
    const uint8_t pad_355[3];
    rw::Field<float> savedTimeScale;             // +0x358 the scheduler's time scale before the slow motion
    rw::Field<uint32_t> pad_35C;

    // battleState values
    enum State : int32_t {
        INTRO = 3, BOSS_APPEAR = 4, HEADING = 6, START_WAIT = 7, ENEMY_DONE = 8, PLAYER_TURN = 9,
        PLAYER_POISON = 10, ENEMY_TURN = 11, PLAYER_INPUT = 12, PLAYER_ATTACK = 14, CLEAR = 15, LOSE = 16,
        RESULT_WAIT = 17, TUTORIAL = 19, CONTINUE = 20,
    };

    static constexpr uint32_t VTABLE = 0xF9775C;          // _ZTV15SceneRaidBattle + 8
    static constexpr uint32_t SIZE = 0x360;

    static uint32_t create();
    static SceneRaidBattle *construct(SceneRaidBattle *self);
    static uint32_t createScene();
    static SceneRaidBattle *destruct(SceneRaidBattle *self);
    static void destructDelete(SceneRaidBattle *self);
    static bool init(SceneRaidBattle *self);
    static void to_slow(SceneRaidBattle *self);
    static void RaidBattle_Start(SceneRaidBattle *self);
    static void loadUIComplete(SceneRaidBattle *self);
    static void setRaidStage(SceneRaidBattle *self, int32_t raidId, int32_t mapType, int32_t unused,
                             uint32_t tutorial, int32_t movieId, uint32_t movieFlag);
    static void loadMapComplete(SceneRaidBattle *self, int32_t mapType);
    static void onBurstButton(SceneRaidBattle *self);
    static void execRaidAttackFriend(SceneRaidBattle *self, uint32_t target, uint32_t burst, uint32_t whole);
    static void execRaidAttack(SceneRaidBattle *self, uint32_t target, uint32_t burst, uint32_t whole);
    static void SelectRandom(SceneRaidBattle *self, uint32_t showMarkers);
    static uint32_t getCameraPosition(SceneRaidBattle *self);
    static void alterCamera(SceneRaidBattle *self, uint32_t action);
    static void battleTrgUI(SceneRaidBattle *self);
    static void update(SceneRaidBattle *self, float dt);
    static void endEnemyTurn(SceneRaidBattle *self);
    static void startEnemyTurn(SceneRaidBattle *self);
    static bool executeRaidBattle(SceneRaidBattle *self, uint32_t done);
    static void removeEnemyMarker(SceneRaidBattle *self);
    static void endPlayerTurn(SceneRaidBattle *self);
    static bool endPlayerCount(SceneRaidBattle *self);
    static void setPlayerDamage(SceneRaidBattle *self, int32_t damage);
    static int32_t getEnemyHp(SceneRaidBattle *self);
    static bool advanceBattleSequence(SceneRaidBattle *self);
    static void enemy_state_update_turn_attack(SceneRaidBattle *self);
    static void enemy_state_update_turn_defense(SceneRaidBattle *self);
    static void req_skilPanel_in(SceneRaidBattle *self, int32_t skillId);
    static void req_materialPanel_in(SceneRaidBattle *self, int32_t materialId);
};
RW_LAYOUT(SceneRaidBattle, onStart, 0x210);
RW_LAYOUT(SceneRaidBattle, onFinish, 0x228);
RW_LAYOUT(SceneRaidBattle, stageUI, 0x2B8);
RW_LAYOUT(SceneRaidBattle, retained, 0x2C4);
RW_LAYOUT(SceneRaidBattle, triggerMode, 0x2D4);
RW_LAYOUT(SceneRaidBattle, battleState, 0x2E0);
RW_LAYOUT(SceneRaidBattle, cameraX, 0x2F8);
RW_LAYOUT(SceneRaidBattle, camera, 0x300);
RW_LAYOUT(SceneRaidBattle, commonDirection, 0x310);
RW_LAYOUT(SceneRaidBattle, gesture, 0x318);
RW_LAYOUT(SceneRaidBattle, cutInShader, 0x348);
RW_LAYOUT(SceneRaidBattle, isTutorial, 0x34F);
RW_LAYOUT(SceneRaidBattle, movieId, 0x350);
RW_LAYOUT(SceneRaidBattle, savedTimeScale, 0x358);
RW_SIZE(SceneRaidBattle, 0x360);
