// PlayerStatusIndicator: the player's status in a quest (bottom-left of the action map and battles): the avatar
// icon inside a circular HP gauge (CircularGauge; long HP bars continue in a straight LinearGauge to the side),
// the HP number (white, or red with an alert sound under 20 %), the defense number, the special-attack (burst)
// gauge with its number and "MAX" animations. A cocos2d::Node; all parts are kept in `parts`.
//
// The anonymous-namespace classes of the translation unit used here:
//   Arc            a ring of three sprites (full, right half, quarter) lit by rate (RaidBossHPIndicator_Lambdas)
//   CircularGauge  two Arcs (04 = shown HP, 03 = HP after the drop animation) clipped by a mask
//   LinearGauge    the straight continuation: two sprites (04 / 03) scaled along x
//   AnimationNode  a Node whose setScaleX calls a std::function<void(float)> (drives the HP drop animation)
//   Vibrate        an ActionInterval shaking a node (the hit reaction)
#pragma once

#include "../battleui/BattleUi.hpp"

struct PlayerStatusIndicator {
    const uint8_t node[0x270];             // cocos2d::Node
    rw::Field<uint32_t> hp;                // +624 HP shown
    rw::Field<uint32_t> hpMax;             // +628
    rw::Field<uint32_t> hpAlert;           // +632 hpMax * 0.2: below it the HP number turns red
    rw::Field<uint32_t> cp;                // +636 burst gauge (10000 per point)
    rw::Field<uint32_t> cpMax;             // +640
    rw::Field<float> gaugeLength;          // +644 total HP bar length (circle + side), in pixels
    rw::Field<float> circleShare;          // +648 the circle's share of the bar (1 = no side gauge)
    rw::Field<uint32_t> parts;             // +652 cocos2d::Vector<Ref*> (see Part)
    rw::Field<uint32_t> partsEnd;          // +656
    rw::Field<uint32_t> partsCap;          // +660
    rw::Field<uint32_t> alertSound;        // +664 audio handle of the HP alert loop (0 = none)
    const uint8_t pad_29C[4];

    // indices into parts
    enum Part : uint32_t {
        ROOT = 0,            // container node (child of this)
        HP_ANIMATION = 1,    // AnimationNode: its scale is the HP rate shown
        HP_NUM = 2,          // TextAtlas
        HP_RED_NUM = 3,      // TextAtlas (alert)
        DEF_NUM = 4,         // "Medal_Deck_Label" TextAtlas
        HP_ALERT = 5,        // Armature "HPAlert"
        BURST_CHARGE = 6,    // Armature at "Buerst_MAX" (Burst_Charge)
        BURST_USE = 7,       // Armature (Burst_Use, placed after the burst number)
        BURST_MAX = 8,       // Armature "Burst_MAX"
        AVATAR_ICON = 9,     // AvatarIcon
        VIBRATE = 10,        // Vibrate action (hit)
        BURST_NUM = 11,      // "Avatar_Burst_Num" TextAtlas
        COST_CANCEL = 12,    // Armature (Skill_CostCancel)
        BURST_GAUGE = 13,    // "Burst_Gauge" LoadingBar
        CIRCLE_GAUGE = 14,   // CircularGauge
        SIDE_GAUGE = 15,     // LinearGauge (long HP bars only)
    };
    uint32_t part(uint32_t k) const { return rw::load<uint32_t>(parts + 4 * k); }

    static constexpr uint32_t SIZE = 0x2A0;
    static constexpr uint32_t VTABLE = 0xF93764;   // _ZTV21PlayerStatusIndicator + 8

    static PlayerStatusIndicator *create(uint32_t hpMax, uint32_t cpMax, uint32_t style);
    static uint32_t init(PlayerStatusIndicator *self, uint32_t hpMax, uint32_t cpMax, uint32_t style);
    static PlayerStatusIndicator *destruct(PlayerStatusIndicator *self);
    static void destructDelete(PlayerStatusIndicator *self);
    static void stopAlert(PlayerStatusIndicator *self);
    static uint32_t renderGaugeTexture(PlayerStatusIndicator *self, float x, float angle, float sideLength);
    static void setHP(PlayerStatusIndicator *self, uint32_t hp);
    static void setCP(PlayerStatusIndicator *self, uint32_t cp);
    static uint32_t calculateFractions(PlayerStatusIndicator *self, float rate, const float *circle,
                                       const float *side);
    static void setupHPText(PlayerStatusIndicator *self, uint32_t hp);
    static void setDef(PlayerStatusIndicator *self, uint32_t def);
    static void playEffectCostCancel(PlayerStatusIndicator *self);
    static uint32_t renderMaskTexture(PlayerStatusIndicator *self, int32_t size, float angle);
};
RW_LAYOUT(PlayerStatusIndicator, hp, 624);
RW_LAYOUT(PlayerStatusIndicator, circleShare, 648);
RW_LAYOUT(PlayerStatusIndicator, parts, 652);
RW_LAYOUT(PlayerStatusIndicator, alertSound, 664);
RW_SIZE(PlayerStatusIndicator, 0x2A0);

namespace psi {

// (anonymous)::CircularGauge, a Node of 0x290 bytes
namespace circle {
constexpr uint32_t SIZE = 0x290;
constexpr uint32_t VTABLE = 0xF941D8;
constexpr uint32_t SCALE = 624;        // float 1.0
constexpr uint32_t MASK_ROTATION = 628;  // float 180: the mask sprite's rotation
constexpr uint32_t ANGLE = 632;        // float -270
constexpr uint32_t SHARE = 636;        // float: share of the full ring the HP uses (Arc rate at full HP)
constexpr uint32_t RATE_04 = 640;      // float: rate shown by arc04 (-1 = never set)
constexpr uint32_t RATE_03 = 644;      // float: rate shown by arc03
constexpr uint32_t ARC_04 = 648;       // Arc*
constexpr uint32_t ARC_03 = 652;       // Arc*
}  // namespace circle

// (anonymous)::LinearGauge, a Node of 0x2A8 bytes
namespace side {
constexpr uint32_t SIZE = 0x2A8;
constexpr uint32_t VTABLE = 0xF939FC;
constexpr uint32_t SHARE = 624;        // float: side part of the bar (relative)
constexpr uint32_t RATE_04 = 628;      // float: rate shown by sprite04 (-1 = never set)
constexpr uint32_t RATE_03 = 632;      // float
constexpr uint32_t WIDTH_04 = 640;     // float: sprite widths (texture width - 2)
constexpr uint32_t WIDTH_03 = 644;
constexpr uint32_t LENGTH = 648;       // float: roundf(SHARE * 251)
constexpr uint32_t LENGTH_HALF = 652;  // float: LENGTH + 0.5
constexpr uint32_t SPRITE_04 = 656;    // Sprite*
constexpr uint32_t SPRITE_03 = 660;    // Sprite*
constexpr uint32_t SPRITES = 664;      // cocos2d::Vector<Sprite*>
}  // namespace side

// (anonymous)::AnimationNode (0x288 bytes): std::function<void(float)> at +624 (callable pointer +640)
namespace anim {
constexpr uint32_t SIZE = 0x288;
constexpr uint32_t VTABLE = 0xF93CD0;
constexpr uint32_t FUNCTION = 624;
constexpr uint32_t FUNCTION_PTR = 640;
}  // namespace anim

// (anonymous)::Arc (0x280 bytes)
namespace arc {
constexpr uint32_t SIZE = 0x280;
constexpr uint32_t VTABLE = 0xF93F54;
constexpr uint32_t SPRITES = 628;      // cocos2d::Vector<Sprite*>
uint32_t create(uint32_t path);        // sub_5E4C98
}  // namespace arc

// (anonymous)::Vibrate, a cocos2d::ActionInterval (0x44 bytes)
namespace vibrate {
constexpr uint32_t SIZE = 0x44;
constexpr uint32_t VTABLE = 0xF93C80;
constexpr uint32_t CLONABLE_VTABLE = 0xF93CB0;  // at +0x14
uint32_t construct(uint32_t self);                         // sub_5E47EC
uint32_t create(float duration, float strength, uint32_t offset);  // sub_5E2A10
}  // namespace vibrate

}  // namespace psi
