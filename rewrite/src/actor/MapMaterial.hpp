// MapMaterial: a dig spot on the quest map ("ActionMapExcavateAnim"). It holds one material id and how many the
// player gets; when the player walks over it (StageFieldManager::checkMaterial) popup() erupts one MapPrizes burst
// of a single material per unit, spread over 64 px left of the spot.
#pragma once

#include "MapNode.hpp"
#include "core/SafeBuffer.hpp"

struct MapMaterial : MapNode {
    // What the spot gives (the argument of create / init).
    struct Info {
        bt::Vec2 position;                 // +0  map position
        rw::Field<int32_t> materialId;     // +8
        rw::Field<uint32_t> prizeCount;    // +12 number of prize bursts (one material each); 0 = no spot
        rw::Field<int32_t> count;          // +16 materials (kept in the property)
    };

    // MapMaterial::Property: SafeBuffer of 8 bytes: material id (0), count (4).
    struct Property : SafeBuffer {
        static constexpr uint32_t VTABLE = 0xF9271C;   // _ZTVN11MapMaterial8PropertyE + 8
        static constexpr uint32_t MATERIAL_ID = 0;
        static constexpr uint32_t COUNT = 4;
        static void destructDelete(Property *self);
    };

    Property property;                     // +648
    rw::Field<uint32_t> armature;          // +680 cocostudio::Armature* ActionMapExcavateAnim
    bt::Vector prizes;                     // +684 cocos2d::Vector<MapPrizes*> one burst per prize
    rw::Field<uint8_t> opened;             // +696 popup() ran (StageFieldManager CHEST_OPENED 0x2B8)
    const uint8_t pad_2b9[7];                    // +697

    static constexpr uint32_t VTABLE = 0xF92494;   // _ZTV11MapMaterial + 8
    static constexpr uint32_t SIZE = 0x2C0;
    static constexpr uint32_t VSLOT_INIT = 156;    // virtual init(const Info*, cocos2d::Node*) (+0x270)
    static constexpr int32_t NODE_TAG = 5;         // node kind on the field: material

    static MapMaterial *construct(MapMaterial *self);
    static MapMaterial *destruct(MapMaterial *self);
    static void destructDelete(MapMaterial *self);
    static uint32_t init(MapMaterial *self, const Info *info, uint32_t parent);
    static MapMaterial *create(const Info *info, uint32_t parent);
    static void firstCall();
    static void finalCall();
    static uint32_t popup(MapMaterial *self);
};
RW_LAYOUT(MapMaterial::Info, materialId, 8);
RW_LAYOUT(MapMaterial::Info, count, 16);
RW_LAYOUT(MapMaterial, property, 648);
RW_LAYOUT(MapMaterial, armature, 680);
RW_LAYOUT(MapMaterial, prizes, 684);
RW_LAYOUT(MapMaterial, opened, 696);
RW_SIZE(MapMaterial, 0x2C0);
