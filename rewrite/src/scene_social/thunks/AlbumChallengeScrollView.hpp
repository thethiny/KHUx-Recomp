// AlbumChallengeScrollView: the album challenge list (a BaseScrollView<int> over challenge ids). Each row shows the
// challenge text, its reward and a "receive" button that opens the reward popup (openAlbumChallengeGetRewardPopup).
// cocos2d::ui::Layout + IScrollProtocol (vtables at +0, +0x280, +0x350), then BaseScrollView<int>'s id vector.
#pragma once

#include "../../core/Guest.hpp"
#include "../../scene/SceneUi.hpp"

struct AlbumChallengeScrollView {
    using Addr = uint32_t;

    const uint8_t layout[0x4FC];               // cocos2d::ui::Layout (vtables at +0, +0x280, +0x350)
    const uint8_t scroll[0x628 - 0x4FC];       // IScrollProtocol (see scene_main/IScrollProtocol.hpp)
    guest::Vector itemIDs;                     // +0x628 BaseScrollView<int>: the challenge ids (0 = empty row)
    const uint8_t pad_634[4];
    const uint8_t callbacks[5][24];            // +0x638 five std::function members (unused by the code here)
    rw::Field<uint32_t> panel;                 // +0x6B0 retained row template (Album_Challenge_Panel_ver112.json)
    rw::Field<uint8_t> tapEnabled;             // +0x6B4 setEnabledTapItem
    rw::Field<uint8_t> field_6B5;              // +0x6B5
    const uint8_t pad_6B6[2];
    rw::Field<uint32_t> field_6B8;             // +0x6B8
    rw::Field<uint8_t> field_6BC;              // +0x6BC
    const uint8_t pad_6BD[3];
    rw::Field<uint32_t> field_6C0;             // +0x6C0
    const uint8_t pad_6C4[4];

    static constexpr uint32_t SIZE = 0x6C8;
    static constexpr uint32_t VTABLE = 0xF8E954;     // _ZTV24AlbumChallengeScrollView + 8
    static constexpr uint32_t VTABLE2 = 0xF8ECAC;    // +0x280
    static constexpr uint32_t VTABLE3 = 0xF8ECC0;    // +0x350

    static AlbumChallengeScrollView *destroy(AlbumChallengeScrollView *self);
    static void destroyDelete(AlbumChallengeScrollView *self);
    static Addr create();
    static AlbumChallengeScrollView *construct(AlbumChallengeScrollView *self);
    static uint32_t init(AlbumChallengeScrollView *self);
    static Addr getLayoutPanel(AlbumChallengeScrollView *self, int32_t row);
    static Addr createRow(AlbumChallengeScrollView *self, uint32_t index, int32_t column);
    static uint32_t changeRow(AlbumChallengeScrollView *self, Addr row, uint32_t index, int32_t column);
    static void registItemTouchEvent(AlbumChallengeScrollView *self, Addr widget, int32_t index);
    static AlbumChallengeScrollView *setEnabledTapItem(AlbumChallengeScrollView *self, uint32_t enabled);
    static void openAlbumChallengeGetRewardPopup(Addr unusedThis);
    static Addr createDrawMedal(Addr unusedThis, uint64_t userMedalId, int32_t rare);
    static Addr createRewardLayer(Addr unusedThis);
};
RW_LAYOUT(AlbumChallengeScrollView, itemIDs, 0x628);
RW_LAYOUT(AlbumChallengeScrollView, callbacks, 0x638);
RW_LAYOUT(AlbumChallengeScrollView, panel, 0x6B0);
RW_LAYOUT(AlbumChallengeScrollView, tapEnabled, 0x6B4);
RW_LAYOUT(AlbumChallengeScrollView, field_6C0, 0x6C0);
RW_SIZE(AlbumChallengeScrollView, AlbumChallengeScrollView::SIZE);
