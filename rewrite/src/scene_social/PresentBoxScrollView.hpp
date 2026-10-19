// PresentBoxScrollView: the Presents list (MyPage: Presents). A BaseScrollView<unsigned long long> over present ids;
// each row is a clone of PresentBOX_Panel (gift image, name and count, received / expiry dates, detail text and a
// Receive button whose lambda carries a copy of the present). The present data comes from the user's present map
// (APIManager +0x104).
#pragma once

#include "../w3_9/BaseScrollView.hpp"

struct PresentBoxScrollView : BaseScrollView {
    const uint8_t pad_634[4];
    const uint8_t function_638[0x18];       // +0x638 std::function x5 (callable pointer at +0x10 of each)
    const uint8_t function_650[0x18];       // +0x650
    const uint8_t function_668[0x18];       // +0x668
    const uint8_t function_680[0x18];       // +0x680
    const uint8_t function_698[0x18];       // +0x698
    rw::Field<uint32_t> panel;              // +0x6B0 the row template (retained; not set by the constructor)
    rw::Field<uint8_t> touchEnabled;        // +0x6B4 rows get touch listeners
    rw::Field<uint8_t> field_6B5;           // +0x6B5
    const uint8_t pad_6B6[2];
    rw::Field<uint32_t> field_6B8;          // +0x6B8
    rw::Field<uint8_t> field_6BC;           // +0x6BC
    const uint8_t pad_6BD[3];
    rw::Field<uint32_t> field_6C0;          // +0x6C0
    const uint8_t pad_6C4[4];

    static constexpr uint32_t SIZE = 0x6C8;
    static constexpr uint32_t VTABLE = 0xF836CC;     // _ZTV20PresentBoxScrollView + 8
    static constexpr uint32_t VTABLE2 = 0xF83A24;    // + 0x360 (at +0x280)
    static constexpr uint32_t VTABLE3 = 0xF83A38;    // + 0x374 (at +0x350)

    static Addr create();
    static PresentBoxScrollView *construct(PresentBoxScrollView *self);
    static PresentBoxScrollView *destroy(PresentBoxScrollView *self);
    static uint32_t init(PresentBoxScrollView *self);
    static Addr getLayoutPanel(PresentBoxScrollView *self);
    static Addr createRow(PresentBoxScrollView *self, int32_t index, int32_t arg);
    static uint32_t changeRow(PresentBoxScrollView *self, Addr row, int32_t index, int32_t arg);
    static void registItemTouchEvent(PresentBoxScrollView *self, Addr row, int32_t index);
    static Addr cloneReceiveLambda(Addr functor);    // changeRow's lambda: __func::__clone()
};
RW_LAYOUT(PresentBoxScrollView, function_638, 0x638);
RW_LAYOUT(PresentBoxScrollView, panel, 0x6B0);
RW_LAYOUT(PresentBoxScrollView, touchEnabled, 0x6B4);
RW_LAYOUT(PresentBoxScrollView, field_6B8, 0x6B8);
RW_LAYOUT(PresentBoxScrollView, field_6C0, 0x6C0);
RW_SIZE(PresentBoxScrollView, PresentBoxScrollView::SIZE);
