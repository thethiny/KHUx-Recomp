// SceneRaidList: the raid boss list scene (AdvRaidListScene_ver130): a RaidBossScrollView listing the raids the
// server sent (APIManager raid list); tapping one checks medal slots / AP and opens StartDeckEditDialog for it.
#pragma once

#include "BattleUi.hpp"

struct SceneRaidList {
    rw::Field<uint32_t> vtable;             // +0 cocos2d::Layer
    const uint8_t pad_004[524];                   // +4
    const uint8_t onEnterHook[24];                // +528 Node std::function<void()> (scheduleUpdate)
    const uint8_t onExitHook[24];                 // +552 Node std::function<void()> (unscheduleUpdate)
    const uint8_t pad_240[72];                    // +576 rest of cocos2d::Layer
    rw::Field<uint32_t> root;               // +648 the scene file's root node
    rw::Field<uint32_t> scrollView;         // +652 RaidBossScrollView*
    rw::Field<uint8_t> leaving;             // +656 set by runOutAnimation (taps ignored)
    rw::Field<uint8_t> canGoBack;           // +657 backScene allowed
    const uint8_t pad_292[6];                     // +658

    static constexpr uint32_t SIZE = 0x298;
    static constexpr uint32_t VTABLE = 0xF9F154;     // _ZTV13SceneRaidList + 8

    static SceneRaidList *create();
    static uint32_t createScene();
    static SceneRaidList *destroy(SceneRaidList *self);
    static void deletingDestroy(SceneRaidList *self);
    static uint32_t init(SceneRaidList *self);
    static void runInAnimation(SceneRaidList *self);
    static void update(SceneRaidList *self, float dt);
    static void runOutAnimation(SceneRaidList *self, uint32_t done);
    static void backScene(SceneRaidList *self);
};
RW_LAYOUT(SceneRaidList, onEnterHook, 528);
RW_LAYOUT(SceneRaidList, root, 648);
RW_LAYOUT(SceneRaidList, leaving, 656);
RW_SIZE(SceneRaidList, 0x298);
