// MapPrize: one collectible on the quest map - an item / munny / Lux icon, or a material - with its contents (a
// MapPrize::Item kept in a SafeBuffer "Gift"). Prizes ride the bones of a MapPrizes eruption armature, then break
// away onto the map and fly into the player (vortex) or to a HUD target (attract); onAbsorptFinish() hides the prize
// and runs the caller's callback.
#pragma once

#include "FunctionThunks.hpp"
#include "FlashNode.hpp"   // StdFunction
#include "battle/MapPrizes.hpp"   // MapPrizeItem (= MapPrize::Item)
#include "core/SafeBuffer.hpp"

struct MapPrize : MapNode {
    using Item = MapPrizeItem;

    // MapPrize::Gift: SafeBuffer of 16 bytes holding the Item (type, id, count, rare).
    struct Gift : SafeBuffer {
        static constexpr uint32_t VTABLE = 0xF92F18;   // _ZTVN8MapPrize4GiftE + 8
        static void destructDelete(Gift *self);
    };

    // MapPrize::ID: the icon (index into the sprite file table, 0..11; 10 / 11 drawn additive).
    static constexpr uint32_t ID_MAX = 11;

    Gift gift;                             // +648
    rw::Field<uint32_t> bone;              // +680 cocostudio::Bone* the prize rides (0 = broken away)
    rw::Field<uint32_t> sprite;            // +684 cocos2d::Sprite* the icon
    rw::Field<uint8_t> ready;              // +688 may break away (StageFieldManager PRIZE_READY)
    rw::Field<uint8_t> absorbed;           // +689 the fly-in finished
    rw::Field<uint8_t> material;           // +690 built by init(Vec2, materialId)
    const uint8_t pad_2b3[5];                    // +691
    StdFunction onAbsorbed;                // +696 std::function<void(MapPrize*)>
    const uint8_t pad_2cc[4];                    // +716

    static constexpr uint32_t VTABLE = 0xF92C64;   // _ZTV8MapPrize + 8
    static constexpr uint32_t SIZE = 0x2D0;
    static constexpr uint32_t VSLOT_INIT_ID = 156;        // virtual init(Vec2, ID, float)
    static constexpr uint32_t VSLOT_INIT_MATERIAL = 157;  // virtual init(Vec2, int)
    static constexpr int32_t NODE_TAG = 3;                // node kind on the field: prize

    static MapPrize *construct(MapPrize *self);
    static MapPrize *destruct(MapPrize *self);
    static void destructDelete(MapPrize *self);
    static uint32_t initId(MapPrize *self, const bt::Vec2 *position, uint32_t id, float scale);
    static MapPrize *createId(const bt::Vec2 *position, uint32_t id, float scale);
    static uint32_t initMaterial(MapPrize *self, const bt::Vec2 *position, int32_t materialId);
    static MapPrize *createMaterial(const bt::Vec2 *position, int32_t materialId);
    static void setItem(MapPrize *self, const Item *item, uint32_t withoutCount);
    static uint32_t getItem(MapPrize *self, Item *out, uint32_t clear);
    static void setAbsorptionVortex(MapPrize *self, const bt::Vec2 *from, uint32_t onAbsorbed);
    static void setAbsorptionAttract(MapPrize *self, const bt::Vec2 *from, uint32_t onAbsorbed, uint32_t spin);
    static uint32_t setBone(MapPrize *self, uint32_t bone);
    static uint32_t breakAwayArmature(MapPrize *self);
    static void onAbsorptFinish(MapPrize *self);

    // CallFunc lambda of setAbsorptionVortex ($_0) / setAbsorptionAttract ($_1): captures this
    static void absorbedLambda(uint32_t lambda);
};
RW_LAYOUT(MapPrize, gift, 648);
RW_LAYOUT(MapPrize, bone, 680);
RW_LAYOUT(MapPrize, sprite, 684);
RW_LAYOUT(MapPrize, ready, 688);
RW_LAYOUT(MapPrize, material, 690);
RW_LAYOUT(MapPrize, onAbsorbed, 696);
RW_SIZE(MapPrize, 0x2D0);
