// SceneShopTop: the shop's top screen ("ShopDraw_3Plate"): the draw (gacha) list in a DrawScrollView and the
// bottom buttons - jewels (magic stones, with the discount offer), sell materials, buy one / three items. Opening a
// draw leaves for SceneDrawRaise, selling for SceneSellOutMaterialItem, back returns to My Page.
//
// The scene is a cocos2d::Layer with a second base, IInOutAnimation, at +0x288 (runInAnimation /
// runOutAnimationMain are reached through its thunks, this - 0x288). All guest objects are 32-bit guest addresses.
#pragma once

#include "core/Guest.hpp"
#include "scene/SceneUi.hpp"

struct SceneShopTop {
    const uint8_t layer[0x288];               // cocos2d::Layer (Node std::function members at +0x210 / +0x228)
    rw::Field<uint32_t> inOutVtable;          // +0x288 IInOutAnimation base
    rw::Field<uint32_t> root;                 // +0x28C node holding the cocostudio layout ("ShopDraw_All")
    rw::Field<uint32_t> field_290;            // +0x290 (zeroed by create, unused here)
    rw::Field<uint8_t> checkDuplicateMedals;  // +0x294 the in-animation end checks for the fusion tutorial
    rw::Field<uint8_t> canTouch;              // +0x295 the bottom buttons react
    const uint8_t pad_296[2];
    rw::Field<uint32_t> scrollView;           // +0x298 DrawScrollView* (the draw list)
    rw::Field<uint8_t> canBack;               // +0x29C the back button / key works
    const uint8_t pad_29D[3];

    static constexpr uint32_t SIZE = 0x2A0;
    static constexpr uint32_t VTABLE_START = 0xF9CA40;            // _ZTV12SceneShopTop
    static constexpr uint32_t VTABLE = VTABLE_START + 8;
    static constexpr uint32_t INOUT_VTABLE = VTABLE_START + 0x318;
    static constexpr uint32_t INOUT_OFFSET = 0x288;               // IInOutAnimation base (thunks: this - 0x288)

    // DrawScrollView members the scene touches
    static constexpr uint32_t SCROLL_ON_OPEN_DRAW = 0x638;        // std::function<void()>: a draw was chosen
    static constexpr uint32_t SCROLL_BUSY = 0x650;                // byte: a dialog is open over the list
    static constexpr uint32_t SCROLL_TAPPED_JEWEL = 0x651;        // byte
    static constexpr uint32_t IMAGEBUTTON_ENABLED = 0x272;        // ImageButton byte: reacts to taps

    using Addr = uint32_t;

    static Addr create();
    static Addr createScene();
    static void callShopAPI(Addr callback);             // std::function<void(bool)>* (by value, caller-owned)
    static Addr destroy(Addr self);                     // ~SceneShopTop (D2)
    static Addr destroyThunk(Addr inOut);               // non-virtual thunk from the IInOutAnimation base (D1)
    static void destroyDelete(Addr self);               // deleting destructor (D0)
    static void destroyDeleteThunk(Addr inOut);
    static uint32_t init(Addr self);
    static void initShopTop(Addr self, Addr widget);
    static void initShopTopBottomButton(Addr self, Addr widget);
    static void update(Addr self, float dt);
    static void runInAnimation(Addr self);
    static void runInAnimationThunk(Addr inOut);
    static void runOutAnimationMain(Addr self, Addr done);   // done: std::function<void()>* (caller-owned)
    static void runOutAnimationMainThunk(Addr inOut, Addr done);
    static void backScene(Addr self);

    // lambda bodies (std::function operator(); fn = the __func object, captures from +4)
    static void onGachaListResult(Addr fn, Addr ok);                 // callShopAPI $_0 (std::function at +0x10)
    static void onEnterScene(Addr fn);                               // init $_1
    static void onExitScene(Addr fn);                                // init $_2
    static void onBackKey(Addr fn);                                  // init $_3
    static uint32_t onTouchBegan(Addr fn, Addr touch, Addr event);   // init $_4: swallow every touch
    static void onInAnimationDone(Addr fn);                          // runInAnimation $_5
    static void enableTouch(Addr fn);                                // runInAnimation $_5 inner #2
    static void onOpenDraw(Addr fn);                                 // initShopTop $_6
    static void openDrawRaise(Addr fn);                              // initShopTop $_6 inner
    static void onTapJewelDummy(Addr fn, Addr ref, Addr type);       // initShopTopBottomButton $_7
    static void onTapJewel(Addr fn, Addr ref, Addr type);            // $_8
    static void refreshJewelButtonCall(Addr fn);                     // $_9 operator()
    static void refreshJewelButton(Addr captures);                   // $_9 body: [Button_Jewel, jewel button, this]
    static void onTapSell(Addr fn, Addr ref, Addr type);             // $_10
    static void openSellOut(Addr fn);                                // $_10 inner
    static void onTapBuy1(Addr fn, Addr ref, Addr type);             // $_11
    static void onTapBuy3(Addr fn, Addr ref, Addr type);             // $_12
    static void onMedalMaxUpClosed(Addr fn, Addr cancel, Addr buy);  // $_12 inner (bool*, bool*)
    static void onDiscountClosed(Addr fn);                           // $_14
    static void backToMyPage(Addr fn);                               // backScene $_13
};
RW_LAYOUT(SceneShopTop, inOutVtable, 0x288);
RW_LAYOUT(SceneShopTop, root, 0x28C);
RW_LAYOUT(SceneShopTop, field_290, 0x290);
RW_LAYOUT(SceneShopTop, checkDuplicateMedals, 0x294);
RW_LAYOUT(SceneShopTop, canTouch, 0x295);
RW_LAYOUT(SceneShopTop, scrollView, 0x298);
RW_LAYOUT(SceneShopTop, canBack, 0x29C);
RW_SIZE(SceneShopTop, SceneShopTop::SIZE);

// The __func vtables (+8) of the scene's lambdas.
namespace shop_top {
constexpr uint32_t VT_GachaListResult = 0xF65050;     // callShopAPI $_0 [std::function<void(bool)> at +0x10]
constexpr uint32_t VT_EnterScene = 0xF65024;          // init $_1 [this]
constexpr uint32_t VT_ExitScene = 0xF64FF8;           // init $_2 [this]
constexpr uint32_t VT_BackKey = 0xF64FCC;             // init $_3 [this]
constexpr uint32_t VT_TouchBegan = 0xF64FA0;          // init $_4 []
constexpr uint32_t VT_InAnimationDone = 0xF64F48;     // runInAnimation $_5 [this]
constexpr uint32_t VT_EnableTouch = 0xF64F74;         // $_5 inner #2 [this]
constexpr uint32_t VT_OpenDraw = 0xF64EF0;            // initShopTop $_6 [this]
constexpr uint32_t VT_OpenDrawRaise = 0xF64F1C;       // $_6 inner []
constexpr uint32_t VT_TapJewelDummy = 0xF64EC4;       // $_7 [this]
constexpr uint32_t VT_TapJewel = 0xF64E40;            // $_8 [this, this, Button_Jewel, jewel button, this] (heap)
constexpr uint32_t VT_RefreshJewel = 0xF64E6C;        // $_9 [Button_Jewel, jewel button, this]
constexpr uint32_t VT_TapSell = 0xF64DE8;             // $_10 [this]
constexpr uint32_t VT_OpenSellOut = 0xF64E14;         // $_10 inner []
constexpr uint32_t VT_TapBuy1 = 0xF64DBC;             // $_11 [this]
constexpr uint32_t VT_TapBuy3 = 0xF64D64;             // $_12 [this]
constexpr uint32_t VT_MedalMaxUpClosed = 0xF64D90;    // $_12 inner []
constexpr uint32_t VT_DiscountClosed = 0xF64E98;      // $_14 [this]
constexpr uint32_t VT_BackToMyPage = 0xF64D38;        // backScene $_13 []
}  // namespace shop_top
