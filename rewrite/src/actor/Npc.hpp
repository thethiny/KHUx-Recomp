// Npc: the LWF character of a town NPC (lwf/character/npc/<id>/wait/wait.lwf), a FlashNode with one motion. The
// first load records the "icon" point of the movie (where the talk icon is drawn).
#pragma once

#include "Enemy.hpp"   // flashlwf:: helpers

struct Npc : FlashNode {
    rw::Field<int32_t> npcId;              // +1308 the LWF folder
    rw::Field<int32_t> motion;             // +1312 (1 before the first load)
    rw::Field<uint8_t> firstLoad;          // +1316 the next wait load records the icon point
    const uint8_t pad_525[3];                    // +1317
    bt::Vec2 field_528;                    // +1320
    bt::Vec2 iconPoint;                    // +1328 "icon" movie position (y flipped)

    static constexpr uint32_t VTABLE = 0xF8FB50;   // _ZTV3Npc + 8
    static constexpr uint32_t SIZE = 0x538;

    static Npc *create(int32_t npcId);
    static Npc *construct(Npc *self, int32_t npcId);
    static void changeMotion(Npc *self, int32_t motion);
    static void preloadResources(const Npc *self);
    static Npc *destruct(Npc *self);
    static void destructDelete(Npc *self);
    static void motionEndLambda(uint32_t lambda);
};
RW_LAYOUT(Npc, npcId, 1308);
RW_LAYOUT(Npc, firstLoad, 1316);
RW_LAYOUT(Npc, iconPoint, 1328);
RW_SIZE(Npc, 0x538);
