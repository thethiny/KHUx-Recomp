// LuxNumber: the "+N Lux" counter that pops up when Lux is collected in a quest: a cocos2d::Node holding the
// cocostudio armature "ActionMapLuxNumAnim" whose 5 digit bones ("LuxNum1".."LuxNum5") show the number.
#pragma once

#include "BattleUi.hpp"

struct LuxNumber {
    rw::Field<uint32_t> vtable;            // +0   cocos2d::Node
    const uint8_t pad_004[620];                  // +4
    rw::Field<uint32_t> armature;          // +624 cocostudio::Armature*
    const uint8_t pad_274[4];                    // +628

    static constexpr uint32_t SIZE = 0x278;
    static constexpr uint32_t VTABLE = 0xF90BE8;   // _ZTV9LuxNumber + 8

    static LuxNumber *destroy(LuxNumber *self);
    static void destroyDelete(LuxNumber *self);
    static uint32_t init(LuxNumber *self, uint32_t value, uint32_t style);
    static LuxNumber *create(uint32_t value, uint32_t style);
    static uint32_t play(LuxNumber *self);
    static uint32_t isFinish(LuxNumber *self);
    static uint32_t firstCall();
    static void finalCall();
};
RW_LAYOUT(LuxNumber, armature, 624);
RW_SIZE(LuxNumber, LuxNumber::SIZE);
