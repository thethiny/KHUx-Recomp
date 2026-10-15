// RaidBossStatusIndicator: the raid boss's large HP gauge (name, level, HP number, up to 18 box cells). It is a
// RaidBossHPIndicator with the raid layout ("RaidHpBox_Large") and an HP number that counts along with the gauge:
// a plain Node (`numberNode`) whose scale is animated from the old HP to the new one, read back every frame by
// update() to print the number.
#pragma once

#include "RaidBossHPIndicator.hpp"

struct RaidBossStatusIndicator : RaidBossHPIndicator {
    // field_32C (base): the HP number last animated to, as a float
    rw::Field<uint32_t> field_330;          // +0x330
    rw::Field<uint32_t> numberTarget;       // +0x334 HP the number animates to
    rw::Field<uint32_t> numberNode;         // +0x338 cocos2d::Node whose scale is the shown number
    rw::Field<uint32_t> field_33C;          // +0x33C

    static constexpr uint32_t SIZE = 0x340;
    static constexpr uint32_t VTABLE = 0xF946E0;   // _ZTV23RaidBossStatusIndicator + 8

    static RaidBossHPIndicator *destroy(RaidBossHPIndicator *self);     // the shared (folded) destructor body
    static void destroyDelete(RaidBossStatusIndicator *self);
    static RaidBossStatusIndicator *create(uint32_t name, uint32_t hp, uint32_t hpMax, int32_t level, int32_t type,
                                           uint32_t hpPerBox);
    static int32_t init(RaidBossStatusIndicator *self, uint32_t name, uint32_t hp, uint32_t hpMax, int32_t level,
                        int32_t type, uint32_t hpPerBox);
    static void NumSync(RaidBossStatusIndicator *self, uint32_t node, float scale);
    static void update(RaidBossStatusIndicator *self, float dt);
    static uint32_t setRaidBossHP(RaidBossStatusIndicator *self, uint32_t hp);
    static void setRaidBossHPDownDir(RaidBossStatusIndicator *self);
    static void setRaidBossHPDirEnd(RaidBossStatusIndicator *self);
    static void setRaidHPUpDir(RaidBossStatusIndicator *self);
    static void setRaidBossHPDownJustDir(RaidBossStatusIndicator *self);
    static void setRaidHPUpJustDir(RaidBossStatusIndicator *self);
    static void setRaidHPUpDirEnd(RaidBossStatusIndicator *self);
};
RW_LAYOUT(RaidBossStatusIndicator, field_330, 0x330);
RW_LAYOUT(RaidBossStatusIndicator, numberTarget, 0x334);
RW_LAYOUT(RaidBossStatusIndicator, numberNode, 0x338);
RW_SIZE(RaidBossStatusIndicator, 0x340);
