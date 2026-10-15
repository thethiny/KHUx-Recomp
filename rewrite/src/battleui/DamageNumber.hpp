// DamageNumber: a damage / heal number popping out of an actor: a cocos2d::Node holding one sprite per digit
// (images per Type: normal, critical...), thrown up with a random offset, bounced, then faded out and removed.
// DisplayDamageNumber::disp_dmgNum (DisplayDamageNumber.hpp) creates and places one.
#pragma once

#include "BattleUi.hpp"

struct DamageNumber {
    rw::Field<uint32_t> vtable;            // +0   cocos2d::Node
    const uint8_t pad_004[620];                  // +4

    static constexpr uint32_t SIZE = 0x270;
    static constexpr uint32_t VTABLE = 0xF94CC4;   // _ZTV12DamageNumber + 8

    // type: 0..3 (digit image set); scale: 0 small (0.8), 1 normal (1.0)
    static DamageNumber *create(int32_t value, uint32_t type, uint32_t scale);
    static uint32_t init(DamageNumber *self, int32_t value, uint32_t type, uint32_t scale);
    static void destroyDelete(DamageNumber *self);
};
RW_SIZE(DamageNumber, DamageNumber::SIZE);
