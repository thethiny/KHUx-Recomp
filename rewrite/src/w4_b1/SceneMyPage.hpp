// SceneMyPage: the "my page" world map (MyPageScene_ver112.json). A scrolling field with the party / union
// members' avatars walking between move points (balloons with their comments), the left / right menu buttons,
// the banner area, the solo / party / ranking modes of the right-hand panel, and the info icons (presents...).
// Multiple inheritance: a second vtable at +0x288. Most members are reached through offsets (see mypage::OFF_*).
#pragma once

#include "W4B1Common.hpp"

struct SceneMyPage {
    const uint8_t layer[0x210];               // cocos2d::Layer
    const uint8_t onEnterCallback[24];        // +0x210
    const uint8_t onExitCallback[24];         // +0x228
    const uint8_t pad_240[0x48];
    rw::Field<uint32_t> vtable2;              // +0x288
    const uint8_t pad_28C[4];
    rw::Field<uint32_t> root;                 // +0x290 the MyPageScene node
    rw::Field<uint32_t> centerWidget;         // +0x294 Widget in CenterUIBG (z 9999)
    rw::Field<uint32_t> panel6;               // +0x298 CenterUIBG/Panel_6
    rw::Field<uint32_t> fieldWidget;          // +0x29C the field (1590x896 at 0.72, replaces Image_DB1)
    rw::Field<uint32_t> fieldImage;           // +0x2A0 ImageView: the field's background
    rw::Field<uint32_t> fieldLayer;           // +0x2A4 the node the avatars walk on
    const uint8_t pad_2A8[4];
    rw::Field<uint32_t> rightSolo;            // +0x2AC RightUI_Solo
    rw::Field<uint32_t> jewelSolo;            // +0x2B0 JewelGet_Solo
    rw::Field<uint32_t> rightParty;           // +0x2B4 RightUI_Party
    rw::Field<uint32_t> centerParty;          // +0x2B8 CenterUI_Party
    const uint8_t pad_2BC[0x428 - 0x2BC];

    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x428;
    static constexpr uint32_t VTABLE_BASE = 0xF98F10;   // _ZTV11SceneMyPage
    static constexpr uint32_t VTABLE = VTABLE_BASE + 8;
    static constexpr uint32_t VTABLE2 = VTABLE_BASE + 0x318;

    static Addr construct(Addr self);          // C2
    static Addr destroy(Addr self);            // D2
    static Addr create();
    static Addr createScene();
    static void cancelShopHandle(Addr self);
    static uint32_t init(Addr self);
    static uint32_t assignTouchEnded(Addr function, Addr lambda);   // sub_63E3AC: function = init::$_11{this}
};
RW_LAYOUT(SceneMyPage, vtable2, 0x288);
RW_LAYOUT(SceneMyPage, root, 0x290);
RW_LAYOUT(SceneMyPage, fieldLayer, 0x2A4);
RW_LAYOUT(SceneMyPage, centerParty, 0x2B8);
RW_SIZE(SceneMyPage, SceneMyPage::SIZE);
