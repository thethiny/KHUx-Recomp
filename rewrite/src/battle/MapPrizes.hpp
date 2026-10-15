// MapPrizes: the burst of prizes (items, munny / coin piles, Lux) that erupts from a defeated enemy or a treasure
// on the quest map. Prizes are MapPrize nodes attached to the bones of an "orbit" armature (ActionMapPrizeAnim /
// ActionMapLuxAnim), which plays the eruption; a LuxNumber shows the Lux amount.
#pragma once

#include "BattleCommon.hpp"

// One prize slot of MapPrizes::Info (MapPrize::Item).
struct MapPrizeItem {
    rw::Field<int32_t> type;               // +0  item type: 0/1 none, 4 munny, 8 / 9 coins, 11 Lux, other <= 20 items
    rw::Field<int32_t> id;                 // +4
    rw::Field<int32_t> count;              // +8
    rw::Field<uint8_t> rare;               // +12 rare drop (treasure box shows the rare animation)
    const uint8_t pad_0d[3];
};
RW_SIZE(MapPrizeItem, 16);

struct MapPrizes {
    // What drops: map position, up to four items, where it comes from.
    struct Info {
        bt::Vec2 position;                 // +0
        MapPrizeItem items[4];             // +8
        rw::Field<int32_t> from;           // +72 FromGeneration: 1 enemy, 3 material (all-material orbit), ...
        rw::Field<uint32_t> luxStyle;      // +76 LuxNumber::Style

        // Info(Vec2 position, FromGeneration from)
        static Info *construct(Info *self, const bt::Vec2 *position, int32_t from);
    };

    rw::Field<uint32_t> vtable;            // +0  MapNode (cocos2d::Node)
    const uint8_t pad_004[620];
    bt::Vec2 position;                     // +624 MapNode map position
    const uint8_t pad_270[16];
    rw::Field<uint32_t> orbitArmature;     // +648 items / material orbit (cocostudio::Armature*)
    rw::Field<uint32_t> luxArmature;       // +652 Lux orbit
    rw::Field<uint32_t> treasureArmature;  // +656 treasure box (Treasure_Takara)
    bt::Vector orbitPrizes;                // +660 cocos2d::Vector<MapPrize*> on orbitArmature
    bt::Vector luxPrizes;                  // +672 on luxArmature
    bt::Vector treasurePrizes;             // +684 dropped directly (the treasure box)
    rw::Field<uint32_t> luxNumber;         // +696 LuxNumber*
    rw::Field<uint32_t> parent;            // +700 node the LuxNumber is added to (0 = this)
    rw::Field<uint32_t> luxAmount;         // +704
    const uint8_t pad_2c4[4];

    static constexpr uint32_t VTABLE = 0xF92F34;   // _ZTV9MapPrizes + 8
    static constexpr uint32_t SIZE = 0x2C8;

    static MapPrizes *destruct(MapPrizes *self);
    static void destructDelete(MapPrizes *self);
    static uint32_t init(MapPrizes *self, Info *info, uint32_t parent);
    static void initMaterialPrizes(MapPrizes *self, Info *info);
    static void initNormalPrizes(MapPrizes *self, Info *info);
    static void shuffleArray(MapPrizes *self, const int32_t *array, int32_t n);
    static void initLuxPrizes(MapPrizes *self, MapPrizeItem *item, const bt::Vec2 *position, uint32_t style);
    static uint32_t createArmature(MapPrizes *self, const char *file, const char *movement);
    static uint32_t playArmature(MapPrizes *self, uint32_t armature);
    static uint32_t isFinishPlayArmature(MapPrizes *self, uint32_t armature);
    static uint32_t eruptionStartArmature(MapPrizes *self, uint32_t armature, bt::Vector *prizes);
    static uint32_t eruptionStartArmatureAt(MapPrizes *self, uint32_t armature, bt::Vector *prizes,
                                            const bt::Vec2 *position);
    static MapPrizes *create(Info *info, uint32_t parent);
    static void firstCall();
    static void finalCall();
    static uint32_t eruption(MapPrizes *self);
    static uint32_t eruptionAt(MapPrizes *self, const bt::Vec2 *position, const bt::Vec2 *luxPosition);
    static uint32_t isEruption(MapPrizes *self);
    static uint32_t isFinishEruption(MapPrizes *self);
};
RW_LAYOUT(MapPrizes::Info, items, 8);
RW_LAYOUT(MapPrizes::Info, from, 72);
RW_SIZE(MapPrizes::Info, 80);
RW_LAYOUT(MapPrizes, position, 624);
RW_LAYOUT(MapPrizes, orbitArmature, 648);
RW_LAYOUT(MapPrizes, orbitPrizes, 660);
RW_LAYOUT(MapPrizes, treasurePrizes, 684);
RW_LAYOUT(MapPrizes, luxNumber, 696);
RW_LAYOUT(MapPrizes, luxAmount, 704);
RW_SIZE(MapPrizes, 0x2C8);
