// CommonDirection: the battle "direction" layer shared by quests and raids (a cocos2d::Node, 0x388 bytes). It owns
// the on-screen effects that are not part of the field: the skill name panels that slide in when a medal skill
// triggers (six slots, queued), the material-drop panel, the target-select marker, the two boss direction markers
// (an arrow at the screen edge pointing to the boss / the transport gate, with a distance level 0..4), and a
// full-screen LayerColor used for black-outs and the heal / battle-start flashes.
// StageDirection (quests) and RaidDirection (raids) derive from it.
#pragma once

#include "BattleUi.hpp"

struct CommonDirection {
    const uint8_t node[0x210];                       // +0x000 cocos2d::Node
    const uint8_t startUpdate[24];                   // +0x210 std::function<void()>: [this]{ scheduleUpdate(); }
    const uint8_t stopUpdate[24];                    // +0x228 std::function<void()>: [this]{ unscheduleUpdate(); }
    const uint8_t pad_240[0x30];                     // +0x240
    rw::PtrField<uint8_t> panelLayer;          // +0x270 Node (retained): parent of the skill / material panels
    rw::PtrField<uint8_t> markerLayer;         // +0x274 Node (retained)
    rw::Field<int32_t> mode;                   // +0x278 2 = raid layout (marker layer at half scale)
    rw::Field<float> clock;                    // +0x27C seconds since the direction started (panel_update)
    // cocos2d::TTFConfig for the panel labels (+0x280 .. +0x2A4)
    const uint8_t fontFile[12];                      // +0x280 std::string
    rw::Field<int32_t> fontSize;               // +0x28C
    rw::Field<int32_t> glyphs;                 // +0x290
    rw::Field<uint32_t> customGlyphs;          // +0x294
    rw::Field<uint32_t> distanceField;         // +0x298 bool (written as a word by preloadResources)
    rw::Field<int32_t> outlineSize;            // +0x29C
    rw::Field<uint16_t> italicsBold;           // +0x2A0 two bools
    const uint8_t pad_2A2[2];                        // +0x2A2
    rw::PtrField<uint8_t> fadeLayer;           // +0x2A4 LayerColor: black-out / flashes (to_start)
    rw::PtrField<uint8_t> bossMarkerArrow;     // +0x2A8 ArmatureWS: distance animation (12 + level)
    rw::PtrField<uint8_t> bossMarker;          // +0x2AC ArmatureWS: rotated towards the target
    rw::PtrField<uint8_t> selectTarget;        // +0x2B0 Armature
    rw::PtrField<uint8_t> materialPanel;       // +0x2B4 Armature
    rw::PtrField<uint8_t> skillPanel[6];       // +0x2B8 Armature per slot
    rw::PtrField<uint8_t> skillPanelBack[6];   // +0x2D0 Armature per slot
    rw::Field<int32_t> panelSkillId[6];        // +0x2E8 skill shown in the slot
    rw::Field<int32_t> panelState[6];          // +0x300 -1 free, 1 in, 2 out, 3 moving up, 4 holding
    rw::Field<int32_t> panelOrder[6];          // +0x318 screen rows: the slot shown in each row, -1 = none
    rw::Field<float> panelHoldStart[6];        // +0x330 clock when the slot started holding
    rw::Field<int32_t> skillQueue[10];         // +0x348 skills waiting for a free slot
    rw::Field<int32_t> skillQueueCount;        // +0x370
    rw::Field<int32_t> materialId;             // +0x374 material shown in the panel, -1 = none
    rw::Field<uint8_t> flag_378;               // +0x378 cleared by init
    const uint8_t pad_379[3];
    rw::Field<int32_t> warningLevel;           // +0x37C boss marker distance level, -1 = none
    rw::Field<uint8_t> markerHidden;           // +0x380 the boss markers faded out (target close)
    rw::Field<uint8_t> warningOn;              // +0x381 calc_warning ran with the marker enabled last time
    const uint8_t pad_382[2];
    rw::Field<uint32_t> stageManager;          // +0x384 StageManager* (its +0x94 Vec2 is the screen centre)

    static constexpr uint32_t VTABLE = 0xFA8D64;   // _ZTV15CommonDirection + 8

    static uint32_t create();
    static CommonDirection *construct(CommonDirection *self);
    static CommonDirection *destruct(CommonDirection *self);
    static void destructDelete(CommonDirection *self);
    static uint32_t init(CommonDirection *self);
    static void preloadResources(CommonDirection *self);
    static void gene_boss_marker(CommonDirection *self, const uint32_t *marker, int32_t animation);
    static void req_select_target_disp(CommonDirection *self);
    static void req_select_target_hide(CommonDirection *self);
    static void req_materialPanel_in(CommonDirection *self, int32_t materialId);
    static void materialPanel_in(CommonDirection *self, int32_t materialId);
    static void req_skilPanel_in(CommonDirection *self, int32_t skillId);
    static void req_skilPanel_out(CommonDirection *self, int32_t skillId);
    static void skilPanel_out(CommonDirection *self, int32_t skillId);
    static void req_skilPanel_all(CommonDirection *self);
    static void skilPanel_out_all(CommonDirection *self);
    static void skilPanel_in(CommonDirection *self, int32_t skillId);
    static void skilPanel_out_common(CommonDirection *self, int32_t slot);
    static void skilPanel_hold(CommonDirection *self, int32_t slot, float seconds);
    static void exec_que(CommonDirection *self);
    static void panel_update(CommonDirection *self, float dt);
    static void hide_boss_marker(CommonDirection *self, const uint32_t *marker);
    static void show_boss_marker(CommonDirection *self, const uint32_t *marker);
    static void update(CommonDirection *self, float dt);
    static void to_start(CommonDirection *self);
    static uint32_t calc_warning(CommonDirection *self, uint32_t enemy, uint32_t avatar, uint32_t stageManager,
                                 uint32_t enabled);
    static void HideBossMarkerAll(CommonDirection *self);
    static void heal(CommonDirection *self);
    static void battle_start(CommonDirection *self);
    static uint32_t black_out(CommonDirection *self, uint32_t callFunc);
};
RW_LAYOUT(CommonDirection, panelLayer, 0x270);
RW_LAYOUT(CommonDirection, fontFile, 0x280);
RW_LAYOUT(CommonDirection, italicsBold, 0x2A0);
RW_LAYOUT(CommonDirection, fadeLayer, 0x2A4);
RW_LAYOUT(CommonDirection, skillPanel, 0x2B8);
RW_LAYOUT(CommonDirection, panelState, 0x300);
RW_LAYOUT(CommonDirection, panelOrder, 0x318);
RW_LAYOUT(CommonDirection, panelHoldStart, 0x330);
RW_LAYOUT(CommonDirection, skillQueue, 0x348);
RW_LAYOUT(CommonDirection, materialId, 0x374);
RW_LAYOUT(CommonDirection, warningLevel, 0x37C);
RW_LAYOUT(CommonDirection, stageManager, 0x384);
RW_SIZE(CommonDirection, 0x388);
