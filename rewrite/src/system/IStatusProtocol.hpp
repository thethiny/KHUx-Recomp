// IStatusProtocol: condition (bad status) bookkeeping shared by StageState and the enemy parts. The implementer
// stores one StatusChange per ConditionType (0 poison, 1, 2, 3 poison-like; see elapseTurnStatusPoison) behind
// two virtuals; the protocol ticks and applies them.
#pragma once

#include "BuffStatus.hpp"

struct IStatusProtocol {
    rw::Field<uint32_t> vtable;   // +0

    static constexpr int32_t CONDITION_TYPES = 4;
    static constexpr uint32_t VSLOT_GET_CONDITION = 0;   // StatusChange getCondition(ConditionType) const (sret)
    static constexpr uint32_t VSLOT_SET_CONDITION = 1;   // void setCondition(ConditionType, const StatusChange&)

    // Merge a new condition into the current one (types 0 and 3 with no turns take the count as turns).
    static void addCondition_common(IStatusProtocol *self, int32_t type, int32_t turns, int32_t count);
    // One turn passed for conditions 1 and 2.
    static void elapseTurnStatus(IStatusProtocol *self);
    // One turn passed for the poison conditions 0 and 3.
    static void elapseTurnStatusPoison(IStatusProtocol *self);
    // One action passed for every condition.
    static void elapseCountStatus(IStatusProtocol *self);
    // Master data condition id (101..122) -> ConditionType (-1 = none); *flag = the id's variant bit.
    static int32_t masterId2ConditionType(IStatusProtocol *self, int32_t masterId, const uint8_t *flag);
    // Clear conditions 0 and 3.
    static void curePoison(IStatusProtocol *self);
};
