// SceneSplash: the boot splash layer. It shows the Square Enix logo, then the Disney logo (fade out / fade in,
// two seconds each; a tap skips the wait), then changes to the opening movie (SceneMovie).
#pragma once

#include "MainCommon.hpp"

struct SceneSplash {
    const uint8_t layer[0x288];                  // cocos2d::Layer
    rw::Field<uint32_t> logo;              // +0x288 ui::ImageView "SQEX_Logo" (its texture changes per logo)
    rw::Field<int32_t> state;              // +0x28C 0 show, 1 wait, 2 next logo, 3 done
    rw::Field<int32_t> logoIndex;          // +0x290 index into LOGO_FILES of the logo shown
    rw::Field<uint8_t> tapped;             // +0x294 the screen was touched: skip the current wait
    const uint8_t pad_295[3];

    static constexpr uint32_t SIZE = 0x298;
    static constexpr uint32_t VTABLE = 0xF9BCD0;       // _ZTV11SceneSplash + 8
    static constexpr uint32_t LOGO_FILES = 0xF643E0;   // const char*[]: the logo images after the first

    static uint32_t create();
    static uint32_t createScene();
    static SceneSplash *destroy(SceneSplash *self);
    static void destroyDelete(SceneSplash *self);
    static uint32_t init(SceneSplash *self);
    static void update(SceneSplash *self, float dt);

    // lambdas of init(): onEnter (start updates), onExit (stop updates), onTouchBegan (skip)
    static void onEnter(uint32_t func);
    static void onExit(uint32_t func);
    static uint32_t onTouchBegan(uint32_t func, uint32_t touch, uint32_t event);
};
RW_LAYOUT(SceneSplash, logo, 0x288);
RW_LAYOUT(SceneSplash, tapped, 0x294);
RW_SIZE(SceneSplash, SceneSplash::SIZE);
