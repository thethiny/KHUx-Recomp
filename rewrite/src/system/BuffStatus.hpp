// BuffChange / StatusChange: the 12-byte records a battle participant keeps per active buff and per condition
// (poison, stun, ...). Both count down either per turn or per action ("count"), and are stored inside a
// SafeBuffer (StageState) or a PartsProperty, so the protocols copy them in and out by value.
#pragma once

#include "rw/rw.hpp"
#include "core/Guest.hpp"

// A buff: remaining duration, strength and packed kind bits.
struct BuffChange {
    rw::Field<int32_t> remain;   // +0 turns (flag TURN) or actions left; 0 = no buff
    rw::Field<int32_t> value;    // +4 strength
    rw::Field<uint32_t> flags;   // +8 bits 0-1: BuffTarget, bit 2: BuffType (1 = defense), bit 3: TURN

    static constexpr uint32_t TARGET_MASK = 3;
    static constexpr uint32_t TYPE_BIT = 4;
    static constexpr uint32_t TURN_BIT = 8;

    // set(type, target, value, turns, count): a buff lasting `turns` turns, else `count` actions.
    static BuffChange *set(BuffChange *self, int32_t type, uint32_t target, int32_t value, int32_t turns,
                           int32_t count);
    // One turn passed: a turn buff loses one (returns 1 when it did).
    static uint32_t elapseTurn(BuffChange *self);
    // One action passed: an action-count buff loses one (returns 1 when it did).
    static uint32_t elapseCount(BuffChange *self);
    static BuffChange *clear(BuffChange *self);
    // Active and of this type and target.
    static uint32_t check_type_target(const BuffChange *self, int32_t type, int32_t target);
    static uint32_t isTarget(const BuffChange *self, int32_t target);
    static uint32_t isType(const BuffChange *self, int32_t type);
    static uint32_t isTurn(const BuffChange *self);
};
RW_SIZE(BuffChange, 12);

// A condition: remaining turns, remaining actions and the condition's id (0 when it ran out).
struct StatusChange {
    rw::Field<int32_t> turns;    // +0
    rw::Field<int32_t> count;    // +4
    rw::Field<int32_t> id;       // +8

    // set(id, turns, count): an existing condition keeps the longer duration.
    static StatusChange *set(StatusChange *self, int32_t id, int32_t turns, int32_t count);
    static uint32_t elapseTurn(StatusChange *self);
    static uint32_t elapseCount(StatusChange *self);
    static StatusChange *clear(StatusChange *self);
};
RW_SIZE(StatusChange, 12);
