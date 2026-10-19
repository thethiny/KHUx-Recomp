// RankingScrollView: the list of SceneRanking. A BaseScrollView<int> over the ranking entries; the view type picks
// the row layout: 0 (Lux ranking) and 2 (colosseum ranking) use Ranking_MainPanel rows with the player's avatar
// icon (an AvatarIconCache), 1 (party ranking) uses Ranking_PartyPanel rows. setRankingData rebuilds the row pool
// when the ranking season changes and sizes the view for the number of entries (up to three entries the list is
// shrunk to fit and does not scroll).
#pragma once

#include "../w3_9/BaseScrollView.hpp"

struct RankingScrollView : BaseScrollView {
    const uint8_t pad_634[4];
    const uint8_t function_638[0x18];       // +0x638 std::function (callable pointer at +0x648)
    rw::Field<int32_t> viewType;            // +0x650 RankingScrollViewType
    rw::Field<int32_t> season;              // +0x654 RankingSeason of the shown data
    rw::Field<uint32_t> panel;              // +0x658 ui::Widget* row template (retained)
    rw::Field<uint32_t> avatarIcons;        // +0x65C AvatarIconCache* (Main panel types only)
    rw::Field<uint16_t> field_660;          // +0x660
    rw::Field<uint8_t> field_662;           // +0x662
    const uint8_t pad_663[1];
    rw::Field<uint32_t> field_664;          // +0x664
    rw::Field<uint8_t> field_668;           // +0x668
    const uint8_t pad_669[3];
    rw::Field<uint32_t> field_66C;          // +0x66C
    guest::Vector selectedIds;              // +0x670 std::vector<int>
    const uint8_t pad_67C[4];

    static constexpr uint32_t SIZE = 0x680;
    static constexpr uint32_t VTABLE = 0xF879C4;     // _ZTV17RankingScrollView + 8
    static constexpr uint32_t VTABLE2 = 0xF87D20;    // + 0x364 (at +0x280)
    static constexpr uint32_t VTABLE3 = 0xF87D34;    // + 0x378 (at +0x350)

    static Addr create(int32_t type);
    static RankingScrollView *construct(RankingScrollView *self);
    static RankingScrollView *destroy(RankingScrollView *self);
    static uint32_t init(RankingScrollView *self, int32_t type);
    static void changeRankingScrollViewType(RankingScrollView *self, int32_t type);
    static void setRankingData(RankingScrollView *self, int32_t season, Addr ids);   // const vector<int>* (by value)
};
RW_LAYOUT(RankingScrollView, function_638, 0x638);
RW_LAYOUT(RankingScrollView, viewType, 0x650);
RW_LAYOUT(RankingScrollView, panel, 0x658);
RW_LAYOUT(RankingScrollView, field_660, 0x660);
RW_LAYOUT(RankingScrollView, field_664, 0x664);
RW_LAYOUT(RankingScrollView, field_66C, 0x66C);
RW_LAYOUT(RankingScrollView, selectedIds, 0x670);
RW_SIZE(RankingScrollView, RankingScrollView::SIZE);
