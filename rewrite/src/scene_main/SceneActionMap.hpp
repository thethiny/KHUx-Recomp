// SceneActionMap: the quest scene (a SceneBattleBase). The player's avatar walks a stage map (field mode), meets
// enemy platoons (battle mode, the shared SceneBattleBase turn logic), reads story talk windows (talk mode), and
// sees the stage title / interlude screens. The scene owns the stage managers (StageManager, StageBattleManager,
// StageExploreManager, StageDirection, CommonDirection, AimTarget) next to the base class's StageUI /
// StageActorManager / StageFieldManager. The current mode is StageManager::mode (StageManager +0x18):
// 1 battle, 2 field, 3 interlude, 4 stage title, 5 talk.
//
// The SceneBattleBase part is declared only as far as this scene touches it (see battleui/SceneBattleBase.hpp).
#pragma once

#include "MainCommon.hpp"

struct SceneActionMap {
    // ---- SceneBattleBase (cocos2d::Layer ...)
    rw::Field<uint32_t> vtable;                  // +0x000
    const uint8_t pad_004[0x210 - 0x004];
    const uint8_t onEnterFn[24];                 // +0x210 std::function<void()> (Layer's onEnter callback)
    const uint8_t onExitFn[24];                  // +0x228 std::function<void()> (Layer's onExit callback)
    const uint8_t pad_240[0x2B8 - 0x240];
    rw::Field<uint32_t> stageUI;                 // +0x2B8 StageUI*
    rw::Field<uint32_t> actorManager;            // +0x2BC StageActorManager* (avatar actor at +0x20)
    rw::Field<uint32_t> fieldManager;            // +0x2C0 StageFieldManager*
    rw::Field<uint32_t> managers[3];             // +0x2C4 cocos2d::Vector<Ref*>: retains the managers
    rw::Field<uint8_t> ready;                    // +0x2D0
    rw::Field<uint8_t> menuOpen;                 // +0x2D1 the battle menu is open: the scene does not update
    rw::Field<uint8_t> menuAllowed;              // +0x2D2
    const uint8_t pad_2D3[1];
    rw::Field<int32_t> touchState;               // +0x2D4
    const uint8_t pad_2D8[0x2E0 - 0x2D8];
    // ---- SceneActionMap
    rw::Field<int32_t> bgmIds[3];                // +0x2E0 audio resource ids from the stage table (+0x3F0..0x3F8)
    rw::Field<uint32_t> pad_2EC;
    const uint8_t gesture[0x30];                 // +0x2F0 GestureRecognizer (onSwipe std::function at +0x18)
    rw::Field<uint32_t> stageManager;            // +0x320 StageManager*
    rw::Field<uint32_t> battleManager;           // +0x324 StageBattleManager*
    rw::Field<uint32_t> exploreManager;          // +0x328 StageExploreManager*
    rw::Field<uint32_t> stageDirection;          // +0x32C StageDirection*
    rw::Field<uint32_t> commonDirection;         // +0x330 CommonDirection*
    rw::Field<uint32_t> aimTarget;               // +0x334 AimTarget*
    rw::Field<uint32_t> pad_338;
    rw::Field<int32_t> titleShown;               // +0x33C set to 1 when the stage title has finished
    const uint8_t pendingFn[0x18];               // +0x340 std::function<bool()> (polled by updateIntrludeMode...)
    rw::Field<float> cameraX;                    // +0x358 cocos2d::Vec2: the camera's map position
    rw::Field<float> cameraY;                    // +0x35C
    rw::Field<uint32_t> field_360;               // +0x360
    rw::Field<uint8_t> intrludeTapped;           // +0x364 a tap during the interlude (when the stage state allows)
    const uint8_t pad_365[3];
    rw::Field<uint32_t> avatar;                  // +0x368 AvatarActor* (the playable avatar)
    rw::Field<uint32_t> field_36C;               // +0x36C
    rw::Field<uint32_t> field_370;               // +0x370
    rw::Field<uint32_t> field_374;               // +0x374
    rw::Field<uint32_t> talkWindow;              // +0x378 cocos2d::Ref* (the talk window, released by the destructor)
    const uint8_t speechQueues[0x14];            // +0x37C unordered_map<SpeechDisp*, deque<string>>
    rw::Field<uint8_t> field_390;                // +0x390
    const uint8_t pad_391[3];
    rw::Field<uint32_t> cutInShader;             // +0x394 GuiltBurstCutInShader*

    static constexpr uint32_t VTABLE = 0xF96B94;          // _ZTV14SceneActionMap + 8
    static constexpr uint32_t SIZE = 0x398;

    static uint32_t createScene();
    static uint32_t createLayer();
    static uint32_t create();
    static SceneActionMap *construct(SceneActionMap *self);
    static SceneActionMap *destruct(SceneActionMap *self);
    static void destructDelete(SceneActionMap *self);
    static void update(SceneActionMap *self, float dt);
    static void updateLoadingMode(SceneActionMap *self, float dt);
    static void updateStageTitle(SceneActionMap *self, float dt);
    static uint32_t onTouchBegan(SceneActionMap *self, uint32_t touch, uint32_t event);
    static uint32_t onTouchBeganField(SceneActionMap *self, uint32_t touch, uint32_t event);
    static uint32_t onTouchBeganIntrlude(SceneActionMap *self, uint32_t touch, uint32_t event);
    static void onTouchMoved(SceneActionMap *self, uint32_t touch, uint32_t event);
    static void onTouchMovedField(SceneActionMap *self, uint32_t touch, uint32_t event);
    static void onTouchEnded(SceneActionMap *self, uint32_t touch, uint32_t event);
    static void onTouchEndedField(SceneActionMap *self, uint32_t touch, uint32_t event);
    static void onTouchCancelled(SceneActionMap *self, uint32_t touch, uint32_t event);
    static void onTouchCancelledField(SceneActionMap *self, uint32_t touch, uint32_t event);
    static void setNewMap(SceneActionMap *self, uint32_t avatar);
    // a cocos2d::Vec2 the camera code moves (passed by address)
    struct CameraVec {
        rw::Field<float> x;
        rw::Field<float> y;
    };
    static void updateCameraPosition(SceneActionMap *self, float rate, float dt);
    static void commonCameraPosition(SceneActionMap *self, CameraVec *from, CameraVec *to, float rate, float dt);
    static void updateTalkCameraPosition(SceneActionMap *self, float rate, float dt);
    static void updateMimicCameraPosition(SceneActionMap *self, float rate, float dt);
    static void toFieldModeSetup(SceneActionMap *self, uint32_t keepBgm);
    static void stageFinishTutorial(SceneActionMap *self, uint32_t done);
    static void onBurstButton(SceneActionMap *self);
    static void revivePlayer(SceneActionMap *self, int32_t hp);
    static void stageFinishNotification(SceneActionMap *self);
    static void updateAimTarget(uint32_t out, SceneActionMap *self);   // returns a Vec2 (sret)
    static void toLoadingModeSetup(SceneActionMap *self);
    static void toBattleModeSetup(SceneActionMap *self);
    static void subTalkText(SceneActionMap *self, uint32_t talk);
    static void toIntrludeModeSetup(SceneActionMap *self);
    static uint32_t isExecutionBurst(SceneActionMap *self);
    static void onSwipeField(SceneActionMap *self, uint32_t gesture, uint32_t delta, uint32_t direction);
    static void lockUI(SceneActionMap *self, uint32_t lock);
    static void executingTutorial1006(SceneActionMap *self);
    static void executingTutorial1010(SceneActionMap *self);
    static void enemy_state_update_turn_attack(SceneActionMap *self);
    static void enemy_state_update_turn_defense(SceneActionMap *self);
    static void req_skilPanel_in(SceneActionMap *self, int32_t skill);
    static void req_materialPanel_in(SceneActionMap *self, int32_t material);
    static void changedMapTutorial(SceneActionMap *self);
    static void setTalkText(SceneActionMap *self, int32_t id);
    static void updateTalkMode(SceneActionMap *self, float dt);
    static void updateIntrludeMode(SceneActionMap *self, float dt);
};
RW_LAYOUT(SceneActionMap, onEnterFn, 0x210);
RW_LAYOUT(SceneActionMap, stageUI, 0x2B8);
RW_LAYOUT(SceneActionMap, managers, 0x2C4);
RW_LAYOUT(SceneActionMap, menuOpen, 0x2D1);
RW_LAYOUT(SceneActionMap, bgmIds, 0x2E0);
RW_LAYOUT(SceneActionMap, gesture, 0x2F0);
RW_LAYOUT(SceneActionMap, stageManager, 0x320);
RW_LAYOUT(SceneActionMap, aimTarget, 0x334);
RW_LAYOUT(SceneActionMap, pendingFn, 0x340);
RW_LAYOUT(SceneActionMap, cameraX, 0x358);
RW_LAYOUT(SceneActionMap, intrludeTapped, 0x364);
RW_LAYOUT(SceneActionMap, avatar, 0x368);
RW_LAYOUT(SceneActionMap, talkWindow, 0x378);
RW_LAYOUT(SceneActionMap, speechQueues, 0x37C);
RW_LAYOUT(SceneActionMap, cutInShader, 0x394);
RW_SIZE(SceneActionMap, SceneActionMap::SIZE);
