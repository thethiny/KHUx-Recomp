#include "BuffStatus.hpp"

// ------------------------------------------------------------------------------------------------ BuffChange
BuffChange *BuffChange::set(BuffChange *self, int32_t type, uint32_t target, int32_t value, int32_t turns,
                            int32_t count) {
    uint32_t flags = 0;
    self->value = value;
    self->flags = 0;
    if (turns) {
        self->remain = turns;
        flags = TURN_BIT;
        self->flags = flags;
    } else if (count) {
        self->remain = count;
    }
    if (type == 1) flags |= TYPE_BIT;
    self->flags = (target & TARGET_MASK) | flags;
    return self;
}
RW_FUNCTION(0x4F2048, "BuffChange::set", BuffChange::set);

uint32_t BuffChange::elapseTurn(BuffChange *self) {
    int32_t remain = self->remain;
    if (remain && (self->flags & TURN_BIT)) {
        self->remain = int32_t(uint32_t(remain) - 1);
        return 1;
    }
    return 0;
}
RW_FUNCTION(0x4F207A, "BuffChange::elapseTurn", BuffChange::elapseTurn);

uint32_t BuffChange::elapseCount(BuffChange *self) {
    int32_t remain = self->remain;
    if (remain && !(self->flags & TURN_BIT)) {
        self->remain = int32_t(uint32_t(remain) - 1);
        return 1;
    }
    return 0;
}
RW_FUNCTION(0x4F2098, "BuffChange::elapseCount", BuffChange::elapseCount);

BuffChange *BuffChange::clear(BuffChange *self) {
    self->remain = 0;
    self->value = 0;
    self->flags = 0;
    return self;
}
RW_FUNCTION(0x4F20B6, "BuffChange::clear", BuffChange::clear);

uint32_t BuffChange::check_type_target(const BuffChange *self, int32_t type, int32_t target) {
    if (!self->remain || !self->value) return 0;
    uint32_t flags = self->flags;
    if ((flags & TARGET_MASK) != uint32_t(target)) return 0;
    return (flags & TYPE_BIT) == uint32_t(type) * 4 ? 1 : 0;
}
RW_FUNCTION(0x4F20C0, "BuffChange::check_type_target", BuffChange::check_type_target);

uint32_t BuffChange::isTarget(const BuffChange *self, int32_t target) {
    return (self->flags & TARGET_MASK) == uint32_t(target) ? 1 : 0;
}
RW_FUNCTION(0x4F20E6, "BuffChange::isTarget", BuffChange::isTarget);

uint32_t BuffChange::isType(const BuffChange *self, int32_t type) {
    return (self->flags & TYPE_BIT) == uint32_t(type) * 4 ? 1 : 0;
}
RW_FUNCTION(0x4F20FA, "BuffChange::isType", BuffChange::isType);

uint32_t BuffChange::isTurn(const BuffChange *self) { return (self->flags & TURN_BIT) >> 3; }
RW_FUNCTION(0x4F2110, "BuffChange::isTurn", BuffChange::isTurn);

// ------------------------------------------------------------------------------------------------ StatusChange
StatusChange *StatusChange::set(StatusChange *self, int32_t id, int32_t turns, int32_t count) {
    int32_t curTurns = self->turns;
    if (curTurns | turns) {
        self->id = id;
        self->turns = curTurns >= turns ? curTurns : turns;
        self->count = 0;
    } else {
        int32_t curCount = self->count;
        if (curCount | count) {
            self->id = id;
            self->count = curCount >= count ? curCount : count;
        }
    }
    return self;
}
RW_FUNCTION(0x4F1CA2, "StatusChange::set", StatusChange::set);

uint32_t StatusChange::elapseTurn(StatusChange *self) {
    int32_t turns = self->turns;
    if (!turns) return 0;
    turns = int32_t(uint32_t(turns) - 1);
    self->turns = turns;
    if (!turns) self->id = 0;
    return 1;
}
RW_FUNCTION(0x4F1CDC, "StatusChange::elapseTurn", StatusChange::elapseTurn);

uint32_t StatusChange::elapseCount(StatusChange *self) {
    int32_t count = self->count;
    if (!count) return 0;
    count = int32_t(uint32_t(count) - 1);
    self->count = count;
    if (!count) self->id = 0;
    return 1;
}
RW_FUNCTION(0x4F1CF8, "StatusChange::elapseCount", StatusChange::elapseCount);

StatusChange *StatusChange::clear(StatusChange *self) {
    self->turns = 0;
    self->count = 0;
    self->id = 0;
    return self;
}
RW_FUNCTION(0x4F1D14, "StatusChange::clear", StatusChange::clear);
