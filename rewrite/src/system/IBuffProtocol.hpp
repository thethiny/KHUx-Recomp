// IBuffProtocol: the buff bookkeeping shared by everything that can be buffed (StageState for the player,
// PartsProperty for enemy parts). The implementer stores 64 BuffChange slots and exposes them through three
// virtuals; the protocol adds / cancels / levels buffs on top of them.
#pragma once

#include "BuffStatus.hpp"

struct IBuffProtocol {
    rw::Field<uint32_t> vtable;   // +0

    static constexpr int32_t SLOTS = 64;
    // virtual slots
    static constexpr uint32_t VSLOT_GET_BUFF = 0;   // BuffChange getBuff(int slot) const (sret)
    static constexpr uint32_t VSLOT_ADD_BUFF = 1;   // void addBuff(const BuffChange&)
    static constexpr uint32_t VSLOT_SET_BUFF = 2;   // void setBuff(int slot, const BuffChange&)

    static constexpr uint32_t MIN_LEVEL = 0xFEF030;   // BuffChange::minLevel (int)
    static constexpr uint32_t MAX_LEVEL = 0xFEF034;   // BuffChange::maxLevel (int)

    // One turn passed: turn-limited attack (type 0) / defense (type 1) buffs lose a turn.
    static void elapseTurnAttackBuff(IBuffProtocol *self);
    static void elapseTurnDefenseBuff(IBuffProtocol *self);
    // One action passed: action-limited attack / defense buffs lose one.
    static void elapseCountAttackBuff(IBuffProtocol *self);
    static void elapseCountDefenseBuff(IBuffProtocol *self);
    // Sum of the active buffs' values of a type and target.
    static int32_t buff_level_main(IBuffProtocol *self, int32_t type, int32_t target);
    // buff_level_main clamped to [minLevel, maxLevel].
    static int32_t buff_level(IBuffProtocol *self, int32_t type, int32_t target);
    // Copy every slot into out[64]: the matching buffs as they are, the others zeroed. Returns their value sum.
    static int32_t buff_add_make_tempbuff(IBuffProtocol *self, const uint8_t *out, int32_t type, int32_t target);
    // The slot to cancel first (action-count buffs before turn buffs, the fewest remaining first); -1 = none.
    static int32_t buff_add_find_minCount(IBuffProtocol *self, const uint8_t *buffs);
    // Add a buff; the level is then kept within -3..3 by adding a counter-buff.
    static void buff_add(IBuffProtocol *self, int32_t value, int32_t turns, int32_t count, int32_t type,
                         int32_t target);
    // Add a buff: an opposite-sign buff first cancels existing ones of the same type and target.
    static void buff_add_main(IBuffProtocol *self, int32_t value, int32_t turns, int32_t count, int32_t type,
                              int32_t target);
    // The first empty slot, or -1.
    static int32_t getBlankBuff(IBuffProtocol *self);
    static void buff_clear(IBuffProtocol *self, int32_t type, int32_t target);
    static void buff_clear_all(IBuffProtocol *self);
};
