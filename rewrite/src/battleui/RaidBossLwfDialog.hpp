// RaidBossLwfDialog: the raid boss demo shown full screen over a modal shadow (the boss appearing, escaping or
// being put down): an AudioLWFNode in a FlashMovieView, plus the cocostudio "RaidDemobtn_1" layout whose centre
// button skips to the last frame. Tapping after the movie ended closes it and runs the callback.
#pragma once

#include "BattleUi.hpp"

struct RaidBossLwfDialog {
    rw::Field<uint32_t> vtable;             // +0 cocos2d::Node
    const uint8_t pad_004[620];                   // +4 cocos2d::Node
    rw::Field<uint32_t> parent;             // +624 the node the dialog was opened on
    rw::Field<uint32_t> shadow;             // +628 SceneUtil::createShadowSprite (holds the dialog)
    rw::Field<uint32_t> field_278;          // +632 zeroed by create
    rw::Field<uint32_t> layout;             // +636 RaidDemobtn_1 widget
    rw::Field<uint32_t> movieView;          // +640 FlashMovieView playing the demo
    rw::Field<uint32_t> field_284;          // +644
    const uint8_t function_288[16];               // +648 std::function<...> (unused here)
    rw::Field<uint32_t> function_288_f;     // +664 its callable pointer
    const uint8_t pad_29C[4];                     // +668
    const uint8_t onClose[16];                    // +672 std::function<void()> run when the dialog closed
    rw::Field<uint32_t> onClose_f;          // +688 its callable pointer
    const uint8_t pad_2B4[4];                     // +692

    static constexpr uint32_t SIZE = 0x2B8;
    static constexpr uint32_t VTABLE = 0xF8C3C4;     // _ZTV17RaidBossLwfDialog + 8

    static RaidBossLwfDialog *create();
    static uint32_t init(RaidBossLwfDialog *self);
    static void open(RaidBossLwfDialog *self, uint32_t parent, int32_t type, int32_t stageId, int32_t demoId,
                     uint32_t onClose);
    static void settingRaidLwf(uint32_t unusedThis, uint32_t lwfNode, int32_t bgType);
    static RaidBossLwfDialog *destroy(RaidBossLwfDialog *self);
    static void deletingDestroy(RaidBossLwfDialog *self);
};
RW_LAYOUT(RaidBossLwfDialog, parent, 624);
RW_LAYOUT(RaidBossLwfDialog, movieView, 640);
RW_LAYOUT(RaidBossLwfDialog, function_288_f, 664);
RW_LAYOUT(RaidBossLwfDialog, onClose, 672);
RW_SIZE(RaidBossLwfDialog, 0x2B8);
