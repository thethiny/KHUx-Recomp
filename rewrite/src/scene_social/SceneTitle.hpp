// SceneTitle: the title screen layer (title animation, logo, "touch screen" prompt, data restore button, the
// version label). Tapping anywhere starts the game (touch listener lambdas); the login request is sent from init.
#pragma once

#include "SocialCommon.hpp"

struct SceneTitle {
    const uint8_t layer[0x288];                   // cocos2d::Layer (its onEnter / onExit std::functions at 0x210 / 0x228)
    rw::Field<uint32_t> root;               // +0x288 TitleScene.json node
    rw::Field<uint32_t> rightUI;            // +0x28C
    rw::Field<uint32_t> gameStart;          // +0x290 "GameStart" group
    rw::Field<uint32_t> restoreButton;      // +0x294 "Button_Restore"
    rw::Field<uint32_t> leftUI;             // +0x298
    rw::Field<uint32_t> titleAnm;           // +0x29C "TitleAnm": parent of both armatures
    rw::Field<uint32_t> titleLogo;          // +0x2A0
    rw::Field<uint32_t> titleArmature;      // +0x2A4 TitleAnimation armature (movement 0: TitleAnime_in)
    rw::Field<uint32_t> logoArmature;       // +0x2A8 TitleAnimation armature (movement 1: TitleAnime_Logo)
    rw::Field<uint32_t> blackLayer;         // +0x2AC LayerColor (black, z 10)
    rw::Field<uint8_t> field_2B0;           // +0x2B0 1 at creation
    rw::Field<uint8_t> uiShown;             // +0x2B1 the UI slid in once the title animation stopped
    const uint8_t pad_2B2[2];
    rw::Field<int32_t> lastFrame;           // +0x2B4 frame count - 1 of the last movement looked up
    rw::Field<uint8_t> field_2B8;           // +0x2B8
    const uint8_t pad_2B9[7];

    static constexpr uint32_t SIZE = 0x2C0;
    static constexpr uint32_t VTABLE = 0xF9B7F0;   // _ZTV10SceneTitle + 8

    static uint32_t create();
    static uint32_t createScene();
    static uint32_t destroy(SceneTitle *self);
    static void destroyDelete(SceneTitle *self);
    static uint32_t init(SceneTitle *self);
    static void runInAnimation(SceneTitle *self);
    static void update(SceneTitle *self, float dt);
    static void runOutAnimation(SceneTitle *self, uint32_t done);
};
RW_LAYOUT(SceneTitle, root, 0x288);
RW_LAYOUT(SceneTitle, blackLayer, 0x2AC);
RW_LAYOUT(SceneTitle, lastFrame, 0x2B4);
RW_SIZE(SceneTitle, SceneTitle::SIZE);
