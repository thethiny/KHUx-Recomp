// MagicStoneScrollView: the Jewel packs of the Buy Jewels dialog (BuyMagicStoneDialog). A BaseScrollView<int>
// over master::Shop ids; every row is a ShopBuyItem_ItemBase card (pack image, Jewel count, bonus balloon, sale /
// skip ticket balloons, store price and a Buy button). Prices come from the store (iap::ProductInfo by product
// id, setProductInfoMap); the Buy button reports the shop id through a lambda of changeRow.
#pragma once

#include "../w3_9/BaseScrollView.hpp"

struct MagicStoneScrollView : BaseScrollView {
    const uint8_t pad_634[4];
    const uint8_t function_638[0x18];       // +0x638 std::function (callable pointer at +0x648)
    const uint8_t function_650[0x18];       // +0x650 std::function (callable pointer at +0x660)
    rw::Field<uint32_t> itemTemplate;       // +0x668 retained Widget holding the ItemBase card (cloned per row)
    rw::Field<uint8_t> itemTouchEnabled;    // +0x66C rows get a touch listener (registItemTouchEvent)
    rw::Field<uint8_t> field_66D;           // +0x66D
    const uint8_t pad_66E[2];
    rw::Field<uint32_t> field_670;          // +0x670
    rw::Field<uint8_t> field_674;           // +0x674
    const uint8_t pad_675[3];
    rw::Field<uint32_t> field_678;          // +0x678
    // +0x67C std::unordered_map<std::string, iap::ProductInfo>: the store's products by product id
    rw::Field<uint32_t> productBuckets;     // +0x67C
    rw::Field<uint32_t> productBucketCount; // +0x680
    rw::Field<uint32_t> productFirst;       // +0x684
    rw::Field<uint32_t> productSize;        // +0x688
    rw::Field<float> productMaxLoad;        // +0x68C

    static constexpr uint32_t SIZE = 0x690;
    static constexpr uint32_t VTABLE = 0xF80DD0;     // _ZTV20MagicStoneScrollView + 8
    static constexpr uint32_t VTABLE2 = 0xF81128;    // + 0x360 (at +0x280)
    static constexpr uint32_t VTABLE3 = 0xF8113C;    // + 0x374 (at +0x350)
    static constexpr uint32_t OFF_PRODUCTS = 0x67C;

    static Addr create();
    static MagicStoneScrollView *construct(MagicStoneScrollView *self);
    static MagicStoneScrollView *destroy(MagicStoneScrollView *self);
    static uint32_t init(MagicStoneScrollView *self);
    static Addr createRow(MagicStoneScrollView *self, uint32_t index, int32_t arg);
    static uint32_t changeRow(MagicStoneScrollView *self, Addr widget, uint32_t index, int32_t arg);
    static void registItemTouchEvent(MagicStoneScrollView *self, Addr widget, int32_t index);
    static void setProductInfoMap(MagicStoneScrollView *self, Addr products);   // const unordered_map* (may be 0)

    // std::function<void(Touch*, Event*)> = registItemTouchEvent's touch-ended lambda {this, index, this, index}
    static void assignTouchEnded(Addr function, Addr captures);
};
RW_LAYOUT(MagicStoneScrollView, function_638, 0x638);
RW_LAYOUT(MagicStoneScrollView, itemTemplate, 0x668);
RW_LAYOUT(MagicStoneScrollView, itemTouchEnabled, 0x66C);
RW_LAYOUT(MagicStoneScrollView, field_670, 0x670);
RW_LAYOUT(MagicStoneScrollView, productBuckets, 0x67C);
RW_LAYOUT(MagicStoneScrollView, productMaxLoad, 0x68C);
RW_SIZE(MagicStoneScrollView, MagicStoneScrollView::SIZE);
