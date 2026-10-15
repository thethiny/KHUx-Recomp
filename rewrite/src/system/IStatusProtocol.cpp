#include "IStatusProtocol.hpp"

namespace {

StatusChange *stackStatus(uint32_t addr) { return rw::frame_ptr<StatusChange>(addr); }

void getCondition(IStatusProtocol *self, uint32_t out, int32_t type) {
    rw::vcall(self, IStatusProtocol::VSLOT_GET_CONDITION, {out, rw::guest_addr(self), uint32_t(type)});
}
void setCondition(IStatusProtocol *self, int32_t type, uint32_t in) {
    rw::vcall(self, IStatusProtocol::VSLOT_SET_CONDITION, {rw::guest_addr(self), uint32_t(type), in});
}

// One turn off condition `type` (its id is cleared when it runs out); unchanged conditions are not written back.
void elapseTurn(IStatusProtocol *self, int32_t type, uint32_t buf) {
    getCondition(self, buf, type);
    StatusChange *s = stackStatus(buf);
    int32_t turns = s->turns;
    if (!turns) return;
    turns = int32_t(uint32_t(turns) - 1);
    s->turns = turns;
    if (!turns) s->id = 0;
    setCondition(self, type, buf);
}

}  // namespace

void IStatusProtocol::addCondition_common(IStatusProtocol *self, int32_t type, int32_t turns, int32_t count) {
    guest::Frame f(16);
    const uint32_t buf = f.addr();
    getCondition(self, buf, type);
    StatusChange *cur = stackStatus(buf);
    // the poison types without turns count their duration in turns
    if ((type == 0 || type == 3) && turns == 0) {
        turns = count;
        count = 0;
    }
    int32_t curTurns = cur->turns;
    if (curTurns | turns) {
        cur->id = 1;
        cur->turns = curTurns >= turns ? curTurns : turns;
        cur->count = 0;
    } else {
        int32_t curCount = cur->count;
        if (curCount | count) {
            cur->id = 1;
            cur->count = curCount >= count ? curCount : count;
        }
    }
    setCondition(self, type, buf);
}
RW_FUNCTION(0x4F1D20, "IStatusProtocol::addCondition_common", IStatusProtocol::addCondition_common);

void IStatusProtocol::elapseTurnStatus(IStatusProtocol *self) {
    guest::Frame f(24);
    elapseTurn(self, 1, f.addr(0));
    elapseTurn(self, 2, f.addr(12));
}
RW_FUNCTION(0x4F1E00, "IStatusProtocol::elapseTurnStatus", IStatusProtocol::elapseTurnStatus);

void IStatusProtocol::elapseTurnStatusPoison(IStatusProtocol *self) {
    guest::Frame f(24);
    elapseTurn(self, 0, f.addr(0));
    elapseTurn(self, 3, f.addr(12));
}
RW_FUNCTION(0x4F1EA4, "IStatusProtocol::elapseTurnStatusPoison", IStatusProtocol::elapseTurnStatusPoison);

void IStatusProtocol::elapseCountStatus(IStatusProtocol *self) {
    guest::Frame f(16);
    const uint32_t buf = f.addr();
    for (int32_t type = 0; type < CONDITION_TYPES; type++) {
        getCondition(self, buf, type);
        StatusChange *s = stackStatus(buf);
        int32_t count = s->count;
        if (!count) continue;
        count = int32_t(uint32_t(count) - 1);
        s->count = count;
        if (!count) s->id = 0;
        setCondition(self, type, buf);
    }
}
RW_FUNCTION(0x4F1F48, "IStatusProtocol::elapseCountStatus", IStatusProtocol::elapseCountStatus);

int32_t IStatusProtocol::masterId2ConditionType(IStatusProtocol *, int32_t masterId, const uint8_t *flag) {
    switch (masterId) {
    case 101: rw::store8(flag, 0); return 0;
    case 102: rw::store8(flag, 1); return 0;
    case 103: rw::store8(flag, 0); return 3;
    case 104: rw::store8(flag, 1); return 3;
    case 111: rw::store8(flag, 0); return 1;
    case 112: rw::store8(flag, 1); return 1;
    case 121: rw::store8(flag, 0); return 2;
    case 122: rw::store8(flag, 1); return 2;
    default: return -1;
    }
}
RW_FUNCTION(0x4F1FC0, "IStatusProtocol::masterId2ConditionType", IStatusProtocol::masterId2ConditionType);

void IStatusProtocol::curePoison(IStatusProtocol *self) {
    guest::Frame f(24);
    for (int32_t type : {0, 3}) {
        const uint32_t buf = f.addr(type == 0 ? 12 : 0);
        getCondition(self, buf, type);
        StatusChange::clear(stackStatus(buf));
        setCondition(self, type, buf);
    }
}
RW_FUNCTION(0x74030C, "IStatusProtocol::curePoison", IStatusProtocol::curePoison);
