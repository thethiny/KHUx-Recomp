// MissBoard: the "MISS" popup of the battle screen. A cocos2d::Node holding one cocostudio armature
// (ActionMapLuxNumAnim, movement "SkillMiss"); disp_miss creates one over a node, plays it once and the movement
// event lambda hides it when the movement completes.
#pragma once

#include "../scene/SceneCommon.hpp"

struct MissBoard {
    rw::Field<uint32_t> vtable;          // +0x000
    const uint8_t nodeBody[0x26C];       // +0x004 cocos2d::Node
    rw::Field<uint32_t> armature;        // +0x270 cocostudio::Armature "ActionMapLuxNumAnim"
    rw::Field<float> pos[2];             // +0x274 cocos2d::Vec2 (constructed, unused here)
    rw::Field<float> uiPos[2];           // +0x27C cocos2d::Vec2: world position (getUIPos)
    const uint8_t pad_284[4];            // +0x284

    static constexpr uint32_t SIZE = 0x288;
    static constexpr uint32_t VTABLE = 0xF908E0;   // _ZTV9MissBoard + 8

    static uint32_t create();
    static MissBoard *construct(MissBoard *self);
    static MissBoard *destruct(MissBoard *self);
    static void destructDelete(MissBoard *self);
    static bool init(MissBoard *self);
    static void firstCall();
    static void finalCall();
    static void play(MissBoard *self, uint32_t unused);
    static bool isFinish(MissBoard *self);
    static uint32_t getUIPos(MissBoard *self);
    static void setCompleteVisible(MissBoard *self, uint32_t armature);
    static void disp_miss(uint32_t parent, uint32_t pos, float yOffset, float scale);
};
RW_LAYOUT(MissBoard, armature, 0x270);
RW_LAYOUT(MissBoard, pos, 0x274);
RW_LAYOUT(MissBoard, uiPos, 0x27C);
RW_SIZE(MissBoard, 0x288);
