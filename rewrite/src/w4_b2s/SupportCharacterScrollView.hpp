// SupportCharacterScrollView: the scrolling list of support (partner) characters of the deck edit screen. A
// cocos2d::ui::Layout + IScrollProtocol (three vtables: +0x000, +0x280 and +0x350). The layout is declared as far
// as the rewritten functions touch it; everything else is padding.
#pragma once

#include "../core/Guest.hpp"
#include "rw/rw.hpp"

struct SupportCharacterScrollViewLayout {
    rw::Field<uint32_t> vtable;   // +0x000
    const uint8_t pad_004[0x27C];
    rw::Field<uint32_t> vtable2;   // +0x280
    const uint8_t pad_284[0xCC];
    rw::Field<uint32_t> vtable3;   // +0x350
    const uint8_t pad_354[0x1A8];
    rw::Field<float> f4FC;   // +0x4FC 0.1f
    rw::Field<float> f500;   // +0x500 0.3f
    rw::Field<uint32_t> f504;   // +0x504
    rw::Field<uint32_t> f508;   // +0x508
    rw::Field<uint32_t> f50C;   // +0x50C
    rw::Field<float> f510;   // +0x510 200.0f
    rw::Field<float> rowWidth;   // +0x514
    rw::Field<float> rowHeight;   // +0x518
    const uint8_t pad_51C[0x1C];
    rw::Field<uint32_t> f538;   // +0x538 1
    rw::Field<uint8_t> f53C;   // +0x53C 1
    const uint8_t pad_53D[0x33];
    rw::Field<float> halfRowWidth;   // +0x570
    rw::Field<float> f574;   // +0x574 -10.0f
    const uint8_t pad_578[0x18];
    rw::Field<uint32_t> f590;   // +0x590
    rw::Field<uint8_t> f594;   // +0x594
    const uint8_t pad_595[0x2F];
    rw::Field<uint32_t> f5C4;   // +0x5C4
    const uint8_t pad_5C8[0x10];
    rw::Field<uint32_t> f5D8;   // +0x5D8
    const uint8_t pad_5DC[0x14];
    rw::Field<uint32_t> f5F0;   // +0x5F0
    const uint8_t pad_5F4[0x14];
    rw::Field<uint32_t> f608;   // +0x608
    const uint8_t pad_60C[0x14];
    rw::Field<uint32_t> f620;   // +0x620
    const uint8_t pad_624[0x4];
    guest::Vector userIds;   // +0x628 std::vector<uint64_t> of the users shown (one per row)
    const uint8_t pad_634[0x4];
    const uint8_t onRowFunctionA[20];   // std::function slot (callable pointer at +0x648)
    const uint8_t pad_64C[0x4];
    const uint8_t onRowFunctionB[20];   // std::function slot (callable pointer at +0x660)
    const uint8_t pad_664[0x4];
    rw::PtrField<uint32_t> itemTemplate;   // +0x668 row template widget (cloned for every row)
    rw::Field<uint8_t> f66C;   // +0x66C
    const uint8_t pad_66D[0x3];
    rw::Field<uint32_t> f670;   // +0x670
    rw::Field<uint8_t> f674;   // +0x674
    const uint8_t pad_675[0x3];
    rw::Field<uint32_t> f678;   // +0x678
    rw::Field<uint8_t> f67C;   // +0x67C
    const uint8_t pad_67D[0x3];
    rw::Field<uint32_t> f680;   // +0x680
    rw::Field<uint8_t> f684;   // +0x684
    const uint8_t pad_685[0x122];
    rw::Field<uint8_t> flags7A7;   // +0x7A7 bit 1 cleared by the constructor
    const uint8_t pad_7A8[0x8];
    rw::Field<uint32_t> selfUserIdLo;   // +0x7B0 own user id, low word
    rw::Field<uint32_t> selfUserIdHi;   // +0x7B4 own user id, high word
    rw::Field<uint32_t> f7B8;   // +0x7B8
    rw::PtrField<uint32_t> avatarIconCache;   // +0x7BC AvatarIconCache*
};
RW_LAYOUT(SupportCharacterScrollViewLayout, vtable, 0x0);
RW_LAYOUT(SupportCharacterScrollViewLayout, vtable2, 0x280);
RW_LAYOUT(SupportCharacterScrollViewLayout, vtable3, 0x350);
RW_LAYOUT(SupportCharacterScrollViewLayout, f4FC, 0x4FC);
RW_LAYOUT(SupportCharacterScrollViewLayout, f500, 0x500);
RW_LAYOUT(SupportCharacterScrollViewLayout, f504, 0x504);
RW_LAYOUT(SupportCharacterScrollViewLayout, f508, 0x508);
RW_LAYOUT(SupportCharacterScrollViewLayout, f50C, 0x50C);
RW_LAYOUT(SupportCharacterScrollViewLayout, f510, 0x510);
RW_LAYOUT(SupportCharacterScrollViewLayout, rowWidth, 0x514);
RW_LAYOUT(SupportCharacterScrollViewLayout, rowHeight, 0x518);
RW_LAYOUT(SupportCharacterScrollViewLayout, f538, 0x538);
RW_LAYOUT(SupportCharacterScrollViewLayout, f53C, 0x53C);
RW_LAYOUT(SupportCharacterScrollViewLayout, halfRowWidth, 0x570);
RW_LAYOUT(SupportCharacterScrollViewLayout, f574, 0x574);
RW_LAYOUT(SupportCharacterScrollViewLayout, f590, 0x590);
RW_LAYOUT(SupportCharacterScrollViewLayout, f594, 0x594);
RW_LAYOUT(SupportCharacterScrollViewLayout, f5C4, 0x5C4);
RW_LAYOUT(SupportCharacterScrollViewLayout, f5D8, 0x5D8);
RW_LAYOUT(SupportCharacterScrollViewLayout, f5F0, 0x5F0);
RW_LAYOUT(SupportCharacterScrollViewLayout, f608, 0x608);
RW_LAYOUT(SupportCharacterScrollViewLayout, f620, 0x620);
RW_LAYOUT(SupportCharacterScrollViewLayout, userIds, 0x628);
RW_LAYOUT(SupportCharacterScrollViewLayout, onRowFunctionA, 0x638);
RW_LAYOUT(SupportCharacterScrollViewLayout, onRowFunctionB, 0x650);
RW_LAYOUT(SupportCharacterScrollViewLayout, itemTemplate, 0x668);
RW_LAYOUT(SupportCharacterScrollViewLayout, f66C, 0x66C);
RW_LAYOUT(SupportCharacterScrollViewLayout, f670, 0x670);
RW_LAYOUT(SupportCharacterScrollViewLayout, f674, 0x674);
RW_LAYOUT(SupportCharacterScrollViewLayout, f678, 0x678);
RW_LAYOUT(SupportCharacterScrollViewLayout, f67C, 0x67C);
RW_LAYOUT(SupportCharacterScrollViewLayout, f680, 0x680);
RW_LAYOUT(SupportCharacterScrollViewLayout, f684, 0x684);
RW_LAYOUT(SupportCharacterScrollViewLayout, flags7A7, 0x7A7);
RW_LAYOUT(SupportCharacterScrollViewLayout, selfUserIdLo, 0x7B0);
RW_LAYOUT(SupportCharacterScrollViewLayout, selfUserIdHi, 0x7B4);
RW_LAYOUT(SupportCharacterScrollViewLayout, f7B8, 0x7B8);
RW_LAYOUT(SupportCharacterScrollViewLayout, avatarIconCache, 0x7BC);
RW_SIZE(SupportCharacterScrollViewLayout, 0x7C0);

struct SupportCharacterScrollView : SupportCharacterScrollViewLayout {
    // new(0x7C0) + constructor + virtual init (+0x248); autoreleased on success, deleted (virtual +4) on failure
    static SupportCharacterScrollView *create();
    static SupportCharacterScrollView *construct(SupportCharacterScrollView *self);   // C2
    static SupportCharacterScrollView *destruct(SupportCharacterScrollView *self);    // D2
    static uint32_t init(SupportCharacterScrollView *self);
    // clone the row template, start its cursor animation and let the virtual +0x340 fill it with row `index`
    static uint32_t createRow(SupportCharacterScrollView *self, uint32_t index, uint32_t arg);
    // refill an existing row widget for the user at `index` (partner panel, medal, skill icon, mix circle)
    static uint32_t changeRow(SupportCharacterScrollView *self, uint32_t row, uint32_t index, uint32_t unused);
    // a touch listener on the scroll view that dispatches the row touches
    static void registItemTouchEvent(SupportCharacterScrollView *self, uint32_t widget, int32_t index);
};
