// DeckMedalView: one medal as shown in the deck editor and the medal lists (a cocos2d::ui::Widget holding a
// clone of the small or normal medal layout, the medal info panel and the "open info" panel). setData stores the
// medal and the deck slot it sits in; updateLayout (DeckMedalView_layout.cpp) draws it. The status / fusion plates
// blink under DeckMedalViewManager::update (updateStatusAndMixedPlate).
//
// A deck slot has an attribute (power / speed / magic, 4 = none) and a direction (upright / reversed, 3 = any);
// a medal matching them gets the slot's multipliers, shown as "×1.50"-style labels and the type-match effect.
#pragma once

#include "../scene/SceneUi.hpp"

struct DeckMedalView {
    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x3C0;

    rw::Field<uint32_t> vtable;             // +0x000 cocos2d::ui::Widget
    const uint8_t pad_004[0x27C];
    rw::Field<uint32_t> vtable2;            // +0x280
    const uint8_t pad_284[0xCC];
    rw::Field<int32_t> showType;            // +0x350 DeckMedalView::ShowType: which value the info row shows
    rw::Field<uint32_t> medal;              // +0x354 shared_ptr<MedalDataManager::UserMedalData>
    rw::Field<uint32_t> medalCtl;           // +0x358
    rw::Field<int32_t> slotIndex;           // +0x35C setData int #1: deck slot (-1: not in a deck; default -1)
    rw::Field<int32_t> slotOpenRank;        // +0x360 setData int #6: rank that opens a locked slot (default 0)
    rw::Field<int32_t> slotAttribute;       // +0x364 setData int #2: 1-3, 4 = none (default 4)
    rw::Field<int32_t> slotDirection;       // +0x368 setData int #3: 1 upright, 2 reversed, 3 any (default 3)
    rw::Field<int32_t> attributeBonus;      // +0x36C setData int #4: x10000 (default 10000)
    rw::Field<int32_t> magnification;       // +0x370 setData int #5: direction bonus x10000 (> 10000 shows Dummy_Mag2)
    rw::Field<uint8_t> typeMatched;         // +0x374 the medal's attribute matches the slot (set by updateLayout)
    rw::Field<uint8_t> typeMatchPlaying;    // +0x375 the type-match LWF is playing
    const uint8_t pad_376[2];
    rw::Field<uint32_t> skills;             // +0x378 vector<UserSkill::Skill>*
    rw::Field<uint32_t> detail;             // +0x37C shared_ptr<UserDetail> (owner of the medal)
    rw::Field<uint32_t> detailCtl;          // +0x380
    rw::Field<float> mixCircleX;            // +0x384 x of Dummy_Guilt_1/Area_MixCircle
    rw::Field<uint8_t> isSmall;             // +0x388 small layout
    rw::Field<uint8_t> isShowMixInfo;       // +0x389
    rw::Field<uint8_t> field_38A;           // +0x38A the status plates blink
    rw::Field<uint8_t> showCoinPanel;       // +0x38B sell-price view: show the sphere point panel of maxed bursts
    rw::Field<uint8_t> noEffect;            // +0x38C
    const uint8_t pad_38D[3];
    rw::Field<uint32_t> medalInfoPanel;     // +0x390 clone of the medal info layout
    rw::Field<uint32_t> darklightMag;       // +0x394 Dummy_Mag2
    // the nodes whose opacity blinks (set by updateLayout): the type-match bonus, the shown value, the fusion row
    rw::Field<uint32_t> bonusIcon;          // +0x398 Icon_3 / Icon_4
    rw::Field<uint32_t> bonusPanel;         // +0x39C Panel_S / Panel_XL
    rw::Field<uint32_t> statusIcon;         // +0x3A0 Icon_1 / Star_Area / Dummy_Guilt_1
    rw::Field<uint32_t> statusPanel;        // +0x3A4 Panel_S / Panel_M
    rw::Field<uint32_t> mixIcon;            // +0x3A8 Icon_2
    rw::Field<uint32_t> mixPanel;           // +0x3AC Panel_S / Panel_M
    rw::Field<uint32_t> largePanel;         // +0x3B0 Panel_L
    rw::Field<uint32_t> coinPanel;          // +0x3B4 avatarCoinPanel
    rw::Field<uint8_t> mixMode;             // +0x3B8 shown in the fusion material picker
    rw::Field<uint8_t> mixSelected;         // +0x3B9
    rw::Field<uint8_t> field_3BA;           // +0x3BA (1; cleared for normal-size list cells)
    const uint8_t pad_3BB[5];

    static Addr create(uint32_t smallLayout);
    static uint32_t init(DeckMedalView *self, uint32_t smallLayout);
    static void updateLayout(DeckMedalView *self, uint32_t playEffect);
    static void updateStatusAndMixedPlate(DeckMedalView *self, uint32_t statusOpacity, uint32_t mixOpacity);
    static void setData(DeckMedalView *self, Addr medal, int32_t a1, int32_t a2, int32_t a3, int32_t a4, int32_t a5,
                        uint32_t relayout, int32_t a6, Addr skills, Addr detail);
    static void setIsShowDarklightMagnification(DeckMedalView *self, uint32_t show);
    static void showTypeMatchEffect(DeckMedalView *self, int32_t type);
    static void changeShowType(DeckMedalView *self, int32_t type);
    static DeckMedalView *destruct(DeckMedalView *self);
    static void destructDelete(DeckMedalView *self);
    static uint32_t destructThunk(uint32_t base2);          // via the +0x280 base
    static void destructDeleteThunk(uint32_t base2);

    // updateLayout's LWF callback $_0 [LWF node, medal image, view, type] and its fade-out lambda
    static void onTypeMatchMovie(Addr functor, Addr movieRef);
    static void onTypeMatchFaded(Addr functor);
};
RW_LAYOUT(DeckMedalView, vtable2, 0x280);
RW_LAYOUT(DeckMedalView, showType, 0x350);
RW_LAYOUT(DeckMedalView, magnification, 0x370);
RW_LAYOUT(DeckMedalView, skills, 0x378);
RW_LAYOUT(DeckMedalView, mixCircleX, 0x384);
RW_LAYOUT(DeckMedalView, isSmall, 0x388);
RW_LAYOUT(DeckMedalView, noEffect, 0x38C);
RW_LAYOUT(DeckMedalView, medalInfoPanel, 0x390);
RW_LAYOUT(DeckMedalView, bonusIcon, 0x398);
RW_LAYOUT(DeckMedalView, mixPanel, 0x3AC);
RW_LAYOUT(DeckMedalView, coinPanel, 0x3B4);
RW_LAYOUT(DeckMedalView, mixMode, 0x3B8);
RW_LAYOUT(DeckMedalView, field_3BA, 0x3BA);
RW_SIZE(DeckMedalView, 0x3C0);
