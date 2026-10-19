// DrawScrollView: the gacha list of the Shop (SceneShopTop). A BaseScrollView<int> of 923 x 148 rows (one per gacha,
// item ids 0..n-1) cloned from ShopDraw_3Plate_Plate; init flattens APIManager's gacha list (grouped by shop) into
// gachas (map<int, GachaData>, keyed by row). A row shows the shop banner (downloaded through DownloadManager and
// kept in textures), the friend points and the draw button(s) whose lambdas carry the gacha.
#pragma once

#include "../w3_9/BaseScrollView.hpp"

struct DrawScrollView : BaseScrollView {
    // A gacha of the list (0x40 bytes): three strings and the draw parameters.
    //   +0x00 std::string, +0x0C std::string, +0x18 id, +0x1C cost, +0x20 draws, +0x24 std::string banner URL,
    //   +0x30 type (2: friend points), +0x34, +0x38 limited, +0x3C draws left
    struct GachaData {
        static void construct(Addr self, Addr other);   // copy constructor
    };

    const uint8_t pad_634[4];
    const uint8_t function_638[0x18];       // +0x638 std::function (callable pointer at +0x648)
    rw::Field<uint8_t> field_650;           // +0x650
    rw::Field<uint8_t> field_651;           // +0x651
    const uint8_t pad_652[2];
    rw::Field<uint32_t> gachasBegin;        // +0x654 std::map<int, GachaData>
    rw::Field<uint32_t> gachasRoot;         // +0x658
    rw::Field<uint32_t> gachasSize;         // +0x65C
    rw::Field<uint32_t> texturesBegin;      // +0x660 std::map<int, Texture2D*> (retained) of the banners
    rw::Field<uint32_t> texturesRoot;       // +0x664
    rw::Field<uint32_t> texturesSize;       // +0x668
    rw::Field<uint32_t> handlesBegin;       // +0x66C std::map<int, unsigned>: DownloadManager request per row
    rw::Field<uint32_t> handlesRoot;        // +0x670
    rw::Field<uint32_t> handlesSize;        // +0x674
    rw::Field<uint8_t> field_678;           // +0x678
    const uint8_t pad_679[3];
    rw::Field<uint32_t> field_67C;          // +0x67C
    rw::Field<uint32_t> field_680;          // +0x680
    rw::Field<uint32_t> field_684;          // +0x684
    rw::Field<uint16_t> field_688;          // +0x688
    const uint8_t pad_68A[2];
    rw::Field<uint32_t> field_68C;          // +0x68C
    rw::Field<uint8_t> field_690;           // +0x690
    const uint8_t pad_691[3];
    guest::Vector raids;                    // +0x694 std::vector<hole::network::api::RaidBase>

    static constexpr uint32_t SIZE = 0x6A0;
    static constexpr uint32_t VTABLE = 0xF8DBD0;
    static constexpr uint32_t VTABLE2 = 0xF8DF28;
    static constexpr uint32_t VTABLE3 = 0xF8DF3C;

    static Addr create();
    static DrawScrollView *construct(DrawScrollView *self);
    static DrawScrollView *destroy(DrawScrollView *self);
    static uint32_t init(DrawScrollView *self);
    static void cancelShopHandle(DrawScrollView *self);
    static void update(DrawScrollView *self, float dt);
    static Addr createRow(DrawScrollView *self, int32_t index, int32_t arg);
    static uint32_t changeRow(DrawScrollView *self, Addr row, int32_t index, int32_t arg);
    static void updateRow(DrawScrollView *self, int32_t row, int32_t first);
    static void updateScroll(DrawScrollView *self, float delta);
    static void registItemTouchEvent(DrawScrollView *self, Addr row, int32_t index);
    static void setDrawFrame(DrawScrollView *self, Addr row, int32_t index);
    static void initShopTopGachaButton(DrawScrollView *self, Addr plate, int32_t index, Addr gacha);
    static uint32_t loadShopBanner(DrawScrollView *self, Addr image, int32_t index, Addr url);
    static void assignTouchCancelled(Addr function, Addr captures);   // sub_5AF210
};
RW_LAYOUT(DrawScrollView, function_638, 0x638);
RW_LAYOUT(DrawScrollView, field_650, 0x650);
RW_LAYOUT(DrawScrollView, gachasBegin, 0x654);
RW_LAYOUT(DrawScrollView, texturesBegin, 0x660);
RW_LAYOUT(DrawScrollView, handlesBegin, 0x66C);
RW_LAYOUT(DrawScrollView, field_678, 0x678);
RW_LAYOUT(DrawScrollView, field_688, 0x688);
RW_LAYOUT(DrawScrollView, field_68C, 0x68C);
RW_LAYOUT(DrawScrollView, raids, 0x694);
RW_SIZE(DrawScrollView, DrawScrollView::SIZE);
