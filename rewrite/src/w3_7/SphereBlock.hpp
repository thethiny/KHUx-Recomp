// SphereBlock: one cell of a sphere board (a cocos2d::Node holding the cell's armature animation). A cell of kind
// 0 also shows an "incentive" panel (Sphere_Incentive.json: reward label and price) and listens to touches.
#pragma once

#include "core/Guest.hpp"

struct SphereBlock {
    rw::Field<uint32_t> vtable;              // +0x000
    const uint8_t node_[0x270 - 0x004];      // cocos2d::Node
    rw::Field<uint8_t> flag_270;             // +0x270 cleared by create()
    const uint8_t pad_271[3];
    rw::Field<int32_t> touchState;           // +0x274 cleared by init()
    rw::Field<uint8_t> flag_278;             // +0x278 cleared by create()
    const uint8_t pad_279[3];
    rw::Field<uint32_t> field_27C;           // +0x27C zeroed by create() (with the next five words)
    rw::Field<uint32_t> armature;            // +0x280 cocostudio::Armature* (retained)
    rw::Field<uint32_t> retained_284;        // +0x284 cocos2d::Ref* released by the destructor
    rw::Field<uint32_t> getMark;             // +0x288 the incentive panel's "Get" node (hidden at first)
    rw::Field<uint32_t> incentiveLabel;      // +0x28C ui::Text "Txt_Incentive_Label"
    rw::Field<uint32_t> priceLabel;          // +0x290 ui::Text "Txt_Price_Label"
    rw::Field<uint32_t> field_294;           // +0x294

    static constexpr uint32_t SIZE = 0x298;
    static constexpr uint32_t VTABLE = 0xFAC510;   // _ZTV11SphereBlock + 8

    // create(id, type, kind, price): a new autoreleased block, or null when init fails
    static uint32_t create(int32_t id, int32_t type, int32_t kind, int32_t price);
    static bool init(SphereBlock *self, int32_t id, int32_t type, int32_t kind, int32_t price);
    static uint32_t show(SphereBlock *self);
    static uint32_t hide(SphereBlock *self);
    static uint32_t setAnimNo(SphereBlock *self, uint32_t index);
    static SphereBlock *destruct(SphereBlock *self);
    static void destructDelete(SphereBlock *self);
};
RW_LAYOUT(SphereBlock, flag_270, 0x270);
RW_LAYOUT(SphereBlock, armature, 0x280);
RW_LAYOUT(SphereBlock, priceLabel, 0x290);
RW_SIZE(SphereBlock, SphereBlock::SIZE);
