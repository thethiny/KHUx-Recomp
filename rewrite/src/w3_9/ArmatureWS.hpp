// ArmatureWS: a cocostudio::Armature with its own time scale (the animation advances by dt * timeScale), used for
// the battle UI's armature animations (CommonDirection).
#pragma once

#include "rw/rw.hpp"

struct ArmatureWS {
    rw::Field<uint32_t> vtable;          // +0   cocostudio::Armature (cocos2d::Node)
    const uint8_t armature_04[620];      // +4   Armature members
    rw::Field<uint32_t> blendVtable;     // +624 cocos2d::BlendProtocol base
    const uint8_t armature_274[80];      // +628 Armature members
    rw::Field<float> timeScale;          // +708 multiplier of update's dt (1.0 after init)

    static constexpr uint32_t VTABLE = 0xFA8FDC;        // _ZTV10ArmatureWS + 8
    static constexpr uint32_t BLEND_VTABLE = 0xFA92A8;  // _ZTV10ArmatureWS + 0x2D4 (BlendProtocol)

    static ArmatureWS *construct(ArmatureWS *self);
    static uint32_t destruct(ArmatureWS *self);
    static void destructDelete(ArmatureWS *self);
    static uint32_t init(ArmatureWS *self);
    static uint32_t initWithName(ArmatureWS *self, uint32_t name);
    static uint32_t initWithNameBone(ArmatureWS *self, uint32_t name, uint32_t parentBone);
    static ArmatureWS *create();
    static ArmatureWS *createWithName(uint32_t name);
    static ArmatureWS *createWithNameBone(uint32_t name, uint32_t parentBone);
    static void update(ArmatureWS *self, float dt);
    static void set_time_scale(ArmatureWS *self, float scale);
};
RW_LAYOUT(ArmatureWS, blendVtable, 624);
RW_LAYOUT(ArmatureWS, timeScale, 708);
RW_SIZE(ArmatureWS, 712);
