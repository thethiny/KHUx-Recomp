// DeckMedalView: one medal as shown in the deck editor and the medal lists (a cocos2d::ui::Widget holding a
// clone of the small or normal medal layout, the medal info panel and the "open info" panel). setData stores the
// medal and its context; updateLayout (not rewritten: too large) draws it. The status / fusion plates blink
// under DeckMedalViewManager::update (updateStatusAndMixedPlate).
#pragma once

#include "../scene/SceneUi.hpp"

struct DeckMedalView {
    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x3C0;

    rw::Field<uint32_t> vtable;             // +0x000 cocos2d::ui::Widget
    const uint8_t pad_004[0x27C];
    rw::Field<uint32_t> vtable2;            // +0x280
    const uint8_t pad_284[0xCC];
    rw::Field<int32_t> showType;            // +0x350 DeckMedalView::ShowType
    rw::Field<uint32_t> medal;              // +0x354 shared_ptr<MedalDataManager::UserMedalData>
    rw::Field<uint32_t> medalCtl;           // +0x358
    rw::Field<int32_t> field_35C;           // +0x35C setData int #1 (default -1)
    rw::Field<int32_t> field_360;           // +0x360 setData int #6 (default 0)
    rw::Field<int32_t> field_364;           // +0x364 setData int #2 (default 4)
    rw::Field<int32_t> field_368;           // +0x368 setData int #3 (default 3)
    rw::Field<int32_t> field_36C;           // +0x36C setData int #4 (default 10000)
    rw::Field<int32_t> magnification;       // +0x370 setData int #5 (default 10000; > 10000 shows Dummy_Mag2)
    rw::Field<uint8_t> field_374;           // +0x374
    rw::Field<uint8_t> typeMatchPlaying;    // +0x375 the type-match LWF is playing
    const uint8_t pad_376[2];
    rw::Field<uint32_t> skills;             // +0x378 vector<UserSkill::Skill>*
    rw::Field<uint32_t> detail;             // +0x37C shared_ptr<UserDetail> (owner of the medal)
    rw::Field<uint32_t> detailCtl;          // +0x380
    rw::Field<float> mixCircleX;            // +0x384 x of Dummy_Guilt_1/Area_MixCircle
    rw::Field<uint8_t> isSmall;               // +0x388 small layout
    rw::Field<uint8_t> isShowMixInfo;       // +0x389
    rw::Field<uint8_t> field_38A;           // +0x38A
    uint8_t pad_38B;
    rw::Field<uint8_t> noEffect;            // +0x38C
    const uint8_t pad_38D[3];
    rw::Field<uint32_t> medalInfoPanel;     // +0x390 clone of the medal info layout
    rw::Field<uint32_t> darklightMag;       // +0x394 Dummy_Mag2
    rw::Field<uint32_t> plate398;           // +0x398 nodes whose opacity blinks (set by updateLayout)
    rw::Field<uint32_t> plate39C;           // +0x39C
    rw::Field<uint32_t> plate3A0;           // +0x3A0
    rw::Field<uint32_t> plate3A4;           // +0x3A4
    rw::Field<uint32_t> plate3A8;           // +0x3A8
    const uint8_t pad_3AC[8];
    rw::Field<uint32_t> plate3B4;           // +0x3B4
    rw::Field<uint8_t> mixMode;             // +0x3B8 shown in the fusion material picker
    rw::Field<uint8_t> mixSelected;         // +0x3B9
    rw::Field<uint8_t> field_3BA;           // +0x3BA (1; cleared for normal-size list cells)
    const uint8_t pad_3BB[5];

    static Addr create(uint32_t smallLayout);
    static uint32_t init(DeckMedalView *self, uint32_t smallLayout);
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

    // updateLayout's LWF callback $_0 [layer?, node, view, type] and its fade-out lambda
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
RW_LAYOUT(DeckMedalView, plate3B4, 0x3B4);
RW_LAYOUT(DeckMedalView, mixMode, 0x3B8);
RW_LAYOUT(DeckMedalView, field_3BA, 0x3BA);
RW_SIZE(DeckMedalView, 0x3C0);
