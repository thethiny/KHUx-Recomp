// BattleMarker: the tappable target marker over a raid boss / enemy in a raid battle: two cocostudio
// "ActMapTapAnim" armatures (the marker ring and the "select" caption), an optional RaidBossHPIndicator, and a
// touch listener that plays the press / select animations and calls onTap when the marker is released over it.
#pragma once

#include "BattleUi.hpp"

struct BattleMarker {
    rw::Field<uint32_t> vtable;            // +0   cocos2d::Node
    const uint8_t pad_004[620];                  // +4
    rw::Field<uint32_t> ring;              // +624 cocostudio::Armature (animation index color * 4 + state)
    rw::Field<uint32_t> caption;           // +628 cocostudio::Armature (animation 8: the "select" text)
    rw::Field<uint32_t> hpIndicator;       // +632 RaidBossHPIndicator* (child at (0, 54))
    rw::Field<uint8_t> pressed;            // +636 a touch began on the marker
    const uint8_t pad_27D[3];                    // +637
    rw::Field<int32_t> color;              // +640 Color: 0 for raid boss markers, 1 otherwise (showEff sets it)
    rw::Field<uint32_t> scene;             // +644 SceneRaidBattle* (set_scene)
    rw::Field<uint8_t> ringOnly;           // +648 hide() hides only the ring
    const uint8_t pad_289[7];                    // +649
    const uint8_t onTap[20];                     // +656 std::function<void(BattleMarker*)> (callable pointer at +672)
    const uint8_t pad_2A4[4];                    // +676

    static constexpr uint32_t SIZE = 0x2A8;
    static constexpr uint32_t VTABLE = 0xF91158;   // _ZTV12BattleMarker + 8
    // SceneRaidBattle fields read by the touch handlers
    static constexpr uint32_t SCENE_INPUT_ENABLED = 0x34D;    // uint8
    static constexpr uint32_t SCENE_BATTLE = 0x308;           // -> object whose int at +0x39C is < 0 when idle

    static BattleMarker *create(uint32_t type, uint32_t hp);
    static BattleMarker *construct(BattleMarker *self, uint32_t type, uint32_t hp);
    static void set_scene(BattleMarker *self, uint32_t scene);
    static uint32_t show(BattleMarker *self);
    static uint32_t show_wait(BattleMarker *self);
    static uint32_t show_select(BattleMarker *self);
    static uint32_t disp_select_string(BattleMarker *self);
    static uint32_t show_selected(BattleMarker *self);
    static uint32_t show_selected_random(BattleMarker *self);
    static uint32_t showEff(BattleMarker *self, float x, float y, int32_t color);
    static uint32_t hide(BattleMarker *self);
    static void setHP(BattleMarker *self, uint32_t hp);
    static BattleMarker *destroy(BattleMarker *self);
    static void destroyDelete(BattleMarker *self);
};
RW_LAYOUT(BattleMarker, ring, 624);
RW_LAYOUT(BattleMarker, pressed, 636);
RW_LAYOUT(BattleMarker, scene, 644);
RW_LAYOUT(BattleMarker, ringOnly, 648);
RW_LAYOUT(BattleMarker, onTap, 656);
RW_SIZE(BattleMarker, BattleMarker::SIZE);
