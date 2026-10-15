// ScenePresentBox: the present box scene (a cocos2d::Layer). init() asks the server for the presents
// (APIManager::executeUserPresentGet); the PresentBOXScene layout then shows them in a PresentBoxScrollView
// (openPresentBoxDialogMain also opens it as a popup from other scenes, see openPresentBoxDialog).
#pragma once

#include "../../core/Guest.hpp"
#include "../../scene/SceneUi.hpp"

struct ScenePresentBox {
    using Addr = uint32_t;

    const uint8_t layer[0x210];                // cocos2d::Layer
    const uint8_t onEnterCallback[24];         // +0x210 std::function<void()> (scheduleUpdate)
    const uint8_t onExitCallback[24];          // +0x228 std::function<void()> (unscheduleUpdate)
    const uint8_t pad_240[0x288 - 0x240];
    rw::Field<uint32_t> field_288;             // +0x288
    rw::Field<uint32_t> sceneNode;             // +0x28C the PresentBOXScene layout
    rw::Field<uint32_t> field_290;             // +0x290
    rw::Field<uint32_t> field_294;             // +0x294
    const uint8_t pad_298[8];

    static constexpr uint32_t SIZE = 0x2A0;
    static constexpr uint32_t VTABLE = 0xFA01A8;   // _ZTV15ScenePresentBox + 8

    static Addr createScene();
    static Addr create();
    static ScenePresentBox *destroy(ScenePresentBox *self);
    static void destroyDelete(ScenePresentBox *self);
    static uint32_t init(ScenePresentBox *self);
    static void update(ScenePresentBox *self, float dt);
    static void runInAnimation(ScenePresentBox *self);
    static void runOutAnimation(ScenePresentBox *self, Addr done);
    // both static: `done` is the std::function<void()> passed by value (run when the popup has closed)
    static void openPresentBoxDialog(Addr done);
    static void openPresentBoxDialogMain(Addr done);
};
RW_LAYOUT(ScenePresentBox, onEnterCallback, 0x210);
RW_LAYOUT(ScenePresentBox, sceneNode, 0x28C);
RW_SIZE(ScenePresentBox, ScenePresentBox::SIZE);
