#include "IBuffProtocol.hpp"

namespace {

// A BuffChange in the guest stack frame (the sret target of getBuff / the argument of setBuff).
struct StackBuff {
    uint32_t addr;
    BuffChange *operator->() const { return rw::frame_ptr<BuffChange>(addr); }
};

void getBuff(IBuffProtocol *self, StackBuff out, int32_t slot) {
    rw::vcall(self, IBuffProtocol::VSLOT_GET_BUFF, {out.addr, rw::guest_addr(self), uint32_t(slot)});
}
void setBuff(IBuffProtocol *self, int32_t slot, StackBuff in) {
    rw::vcall(self, IBuffProtocol::VSLOT_SET_BUFF, {rw::guest_addr(self), uint32_t(slot), in.addr});
}

// The four elapse loops: buffs of `defense` type (bit 2) lose one when their TURN bit equals `turn`.
void elapse(IBuffProtocol *self, bool defense, bool turn) {
    guest::Frame f(16);
    StackBuff b{f.addr()};
    for (int32_t i = 0; i < IBuffProtocol::SLOTS; i++) {
        getBuff(self, b, i);
        uint32_t flags = b->flags;
        if (bool(flags & BuffChange::TYPE_BIT) != defense) continue;
        if (bool(flags & BuffChange::TURN_BIT) == turn && b->remain != 0)
            b->remain = int32_t(uint32_t(int32_t(b->remain)) - 1);
        setBuff(self, i, b);
    }
}

}  // namespace

void IBuffProtocol::elapseTurnAttackBuff(IBuffProtocol *self) { elapse(self, false, true); }
RW_FUNCTION(0x4F211C, "IBuffProtocol::elapseTurnAttackBuff", IBuffProtocol::elapseTurnAttackBuff);

void IBuffProtocol::elapseTurnDefenseBuff(IBuffProtocol *self) { elapse(self, true, true); }
RW_FUNCTION(0x4F219C, "IBuffProtocol::elapseTurnDefenseBuff", IBuffProtocol::elapseTurnDefenseBuff);

void IBuffProtocol::elapseCountAttackBuff(IBuffProtocol *self) { elapse(self, false, false); }
RW_FUNCTION(0x4F221C, "IBuffProtocol::elapseCountAttackBuff", IBuffProtocol::elapseCountAttackBuff);

void IBuffProtocol::elapseCountDefenseBuff(IBuffProtocol *self) { elapse(self, true, false); }
RW_FUNCTION(0x4F229C, "IBuffProtocol::elapseCountDefenseBuff", IBuffProtocol::elapseCountDefenseBuff);

int32_t IBuffProtocol::buff_level_main(IBuffProtocol *self, int32_t type, int32_t target) {
    guest::Frame f(16);
    StackBuff b{f.addr()};
    uint32_t sum = 0;
    for (int32_t i = 0; i < SLOTS; i++) {
        getBuff(self, b, i);
        if (b->remain == 0 || b->value == 0) continue;
        uint32_t flags = b->flags;
        if ((flags & BuffChange::TYPE_BIT) == uint32_t(type) * 4 &&
            (flags & BuffChange::TARGET_MASK) == uint32_t(target))
            sum += uint32_t(int32_t(b->value));
    }
    return int32_t(sum);
}
RW_FUNCTION(0x4F231C, "IBuffProtocol::buff_level_main", IBuffProtocol::buff_level_main);

int32_t IBuffProtocol::buff_level(IBuffProtocol *self, int32_t type, int32_t target) {
    int32_t level = buff_level_main(self, type, target);
    int32_t maxLevel = rw::load<int32_t>(MAX_LEVEL);
    if (maxLevel >= level) maxLevel = level;
    int32_t minLevel = rw::load<int32_t>(MIN_LEVEL);
    return maxLevel >= minLevel ? maxLevel : minLevel;
}
RW_FUNCTION(0x4F23CC, "IBuffProtocol::buff_level", IBuffProtocol::buff_level);

int32_t IBuffProtocol::buff_add_make_tempbuff(IBuffProtocol *self, const uint8_t *out, int32_t type,
                                              int32_t target) {
    guest::Frame f(16);
    StackBuff b{f.addr()};
    uint32_t sum = 0;
    for (int32_t i = 0; i < SLOTS; i++) {
        getBuff(self, b, i);
        const uint8_t *dst = out + 12 * i;
        int32_t remain = b->remain, value = b->value;
        uint32_t flags = b->flags;
        if (remain && value && (flags & BuffChange::TARGET_MASK) == uint32_t(target) &&
            (flags & BuffChange::TYPE_BIT) == uint32_t(type) * 4) {
            rw::store32(dst, uint32_t(remain));
            rw::store32(dst + 4, uint32_t(value));
            rw::store32(dst + 8, flags);
            sum += uint32_t(value);
        } else {
            rw::store32(dst, 0);
            rw::store32(dst + 4, 0);
            rw::store32(dst + 8, 0);
        }
    }
    return int32_t(sum);
}
RW_FUNCTION(0x4F2404, "IBuffProtocol::buff_add_make_tempbuff", IBuffProtocol::buff_add_make_tempbuff);

int32_t IBuffProtocol::buff_add_find_minCount(IBuffProtocol *, const uint8_t *buffs) {
    auto remainOf = [&](int32_t i) { return rw::get<uint32_t>(buffs, 12 * uint32_t(i)); };
    int32_t best = -1;
    bool bestIsCount = false;   // the best so far is an action-count buff (those are cancelled first)
    for (int32_t i = 0; i < SLOTS; i++) {
        const uint8_t *b = buffs + 12 * i;
        if (rw::get<uint32_t>(b, 4) == 0) continue;   // no value
        bool turn = (b[8] & BuffChange::TURN_BIT) != 0;
        if (turn) {
            if (best == -1 || bestIsCount) {
                if (best == -1) best = i;
            } else {
                if (rw::get<uint32_t>(b, 0) < remainOf(best)) best = i;
                bestIsCount = false;
            }
        } else {
            if (best == -1 || !bestIsCount) {
                best = i;
            } else if (rw::get<uint32_t>(b, 0) < remainOf(best)) {
                best = i;
            }
            bestIsCount = true;
        }
    }
    return best;
}
RW_FUNCTION(0x4F24B0, "IBuffProtocol::buff_add_find_minCount", IBuffProtocol::buff_add_find_minCount);

void IBuffProtocol::buff_add(IBuffProtocol *self, int32_t value, int32_t turns, int32_t count, int32_t type,
                             int32_t target) {
    buff_add_main(self, value, turns, count, type, target);
    int32_t level = buff_level_main(self, type, target);
    // over +-3: add a one-action counter-buff bringing the level back to the limit
    if (level >= 4)
        buff_add_main(self, int32_t(3u - uint32_t(level)), 0, 1, type, target);
    else if (level <= -4)
        buff_add_main(self, int32_t(uint32_t(-3) - uint32_t(level)), 0, 1, type, target);
}
RW_FUNCTION(0x4F256E, "IBuffProtocol::buff_add", IBuffProtocol::buff_add);

void IBuffProtocol::buff_add_main(IBuffProtocol *self, int32_t value, int32_t turns, int32_t count, int32_t type,
                                  int32_t target) {
    // the original's frame: A at +0x34, B at +0x40, the probe at +0x4C, the 64 temp buffs at +0x58
    guest::Frame f(0x360);
    StackBuff a{f.addr(0x34)}, b{f.addr(0x40)}, probe{f.addr(0x4C)};
    const uint32_t temp = f.addr(0x58);
    for (uint32_t k = 0; k < 64 * 12; k += 4) rw::store32(rw::frame_ptr<uint8_t>(temp + k), 0);
    int32_t total = buff_add_make_tempbuff(self, rw::host_ptr<uint8_t>(temp), type, target);

    // an opposite-sign buff cancels the existing ones first
    bool cancel = total != 0 && ((value < 0 && total > 0) || (value >= 1 && total < 0));
    if (cancel) {
        while (value != 0) {
            int32_t idx = buff_add_find_minCount(self, rw::host_ptr<uint8_t>(temp));
            if (idx < 0) break;   // nothing left to cancel: add the rest as a new buff
            const uint32_t entry = temp + 12 * uint32_t(idx);
            int32_t entryValue = rw::load<int32_t>(entry + 4);
            getBuff(self, b, idx);
            int32_t rest;
            bool partial;
            if (value < 0) partial = entryValue > int32_t(0u - uint32_t(value));
            else partial = value >= 1 && value < int32_t(0u - uint32_t(entryValue));
            if (partial) {
                // the existing buff absorbs the whole value
                rw::save<int32_t>(entry + 4, int32_t(uint32_t(entryValue) + uint32_t(value)));
                b->value = int32_t(uint32_t(int32_t(b->value)) + uint32_t(value));
                rest = 0;
            } else {
                // the existing buff is used up
                rw::save<uint32_t>(entry + 8, 0);
                rw::save<uint32_t>(entry + 4, 0);
                rw::save<uint32_t>(entry, 0);
                b->remain = 0;
                b->value = 0;
                b->flags = 0;
                rest = int32_t(uint32_t(entryValue) + uint32_t(value));
            }
            setBuff(self, idx, b);
            value = rest;
        }
        if (value == 0) return;
    }
    if (value == 0) return;

    // a new buff in the first empty slot
    int32_t slot = 0;
    for (;; slot++) {
        if (slot >= SLOTS) return;
        getBuff(self, probe, slot);
        if (probe->remain == 0) break;
    }
    getBuff(self, a, slot);
    uint32_t flags = 0;
    a->flags = 0;
    a->value = value;
    if (turns) {
        a->remain = turns;
        flags = BuffChange::TURN_BIT;
        a->flags = flags;
    } else if (count) {
        a->remain = count;
    }
    if (type == 1) {
        flags |= BuffChange::TYPE_BIT;
        a->flags = flags;
    }
    a->flags = (uint32_t(target) & BuffChange::TARGET_MASK) | flags;
    setBuff(self, slot, a);
}
RW_FUNCTION(0x4F25CC, "IBuffProtocol::buff_add_main", IBuffProtocol::buff_add_main);

int32_t IBuffProtocol::getBlankBuff(IBuffProtocol *self) {
    guest::Frame f(16);
    StackBuff b{f.addr()};
    for (int32_t i = 0; i < SLOTS; i++) {
        getBuff(self, b, i);
        if (b->remain == 0) return i;
    }
    return -1;
}
RW_FUNCTION(0x4F2780, "IBuffProtocol::getBlankBuff", IBuffProtocol::getBlankBuff);

void IBuffProtocol::buff_clear(IBuffProtocol *self, int32_t type, int32_t target) {
    guest::Frame f(16);
    StackBuff b{f.addr()};
    for (int32_t i = 0; i < SLOTS; i++) {
        getBuff(self, b, i);
        if (b->remain == 0 || b->value == 0) continue;
        uint32_t flags = b->flags;
        if ((flags & BuffChange::TARGET_MASK) != uint32_t(target) ||
            (flags & BuffChange::TYPE_BIT) != uint32_t(type) * 4)
            continue;
        b->remain = 0;
        b->value = 0;
        b->flags = 0;
        setBuff(self, i, b);
    }
}
RW_FUNCTION(0x4F27E8, "IBuffProtocol::buff_clear", IBuffProtocol::buff_clear);

void IBuffProtocol::buff_clear_all(IBuffProtocol *self) {
    guest::Frame f(16);
    StackBuff b{f.addr()};
    for (int32_t i = 0; i < SLOTS; i++) {
        getBuff(self, b, i);
        b->remain = 0;
        b->value = 0;
        b->flags = 0;
        setBuff(self, i, b);
    }
}
RW_FUNCTION(0x4F2880, "IBuffProtocol::buff_clear_all", IBuffProtocol::buff_clear_all);
