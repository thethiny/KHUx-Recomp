// StageExploreManager: the avatar's free movement on a quest's field screen: the dodge roll (a fixed 29-frame
// displacement curve along the roll direction) and the "slide along walls" movement (move as far as possible toward
// a target, else try the 8 neighbouring directions and keep the one closest to the wanted direction).
#pragma once

#include "../battle/BattleCommon.hpp"

struct StageExploreManager {
    rw::Field<uint32_t> vtable;          // +0  cocos2d::Ref
    const uint8_t pad_04[16];            // +4  Ref (refcount, script ids)
    rw::Field<int32_t> rolling;          // +20 1 while a dodge roll runs
    bt::Vec2 rollDirection;              // +24 roll direction (x picks the facing)
    rw::Field<int32_t> rollFrame;        // +32 next index into the roll curve (0..28)
    rw::Field<float> rollTime;           // +36 seconds since the roll started
    const uint8_t callback[20];          // +40 std::function (16-byte buffer, callable pointer at +56)
    const uint8_t pad_3C[4];             // +60 (operator new(64))

    static constexpr uint32_t VTABLE = 0xFA8434;     // _ZTV19StageExploreManager + 8
    static constexpr uint32_t ROLL_CURVE = 0xE9EBB0;  // float[29]: displacement per roll frame
    static constexpr uint32_t NEIGHBOURS = 0xE9EC24;  // int[8][2]: the 8 neighbouring offsets (x, y)

    static StageExploreManager *create();
    static StageExploreManager *construct(StageExploreManager *self);
    static StageExploreManager *destruct(StageExploreManager *self);
    static void destructDelete(StageExploreManager *self);
    static uint32_t init(StageExploreManager *self);
    static void startDodgeRoll(StageExploreManager *self, const bt::Vec2 *direction);
    static void stopDodgeRoll(StageExploreManager *self);
    static uint32_t actionDodgeRoll(StageExploreManager *self, float dt, uint32_t avatarActor);
    static uint32_t moveAsMuchAsPossible(StageExploreManager *self, uint32_t avatarActor, bt::Vec2 *target);
    static uint32_t searchMovablePosition(StageExploreManager *self, uint32_t avatarActor, bt::Vec2 *target,
                                          uint32_t fieldManager);
};
RW_LAYOUT(StageExploreManager, rolling, 20);
RW_LAYOUT(StageExploreManager, rollDirection, 24);
RW_LAYOUT(StageExploreManager, rollFrame, 32);
RW_LAYOUT(StageExploreManager, rollTime, 36);
RW_LAYOUT(StageExploreManager, callback, 40);
RW_SIZE(StageExploreManager, 64);
