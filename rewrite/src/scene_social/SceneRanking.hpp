// SceneRanking: the Rankings screen (MyPage: Lux emblem > Rankings, Party > Rankings). Three categories
// (SceneManager::SelectRanking: 4 Lux, 1 party, 2 colosseum) and two periods (1 the current one, 2 the previous
// one), switched with the two CenterUI_Switch buttons. The list is a RankingScrollView over the API's ranking
// data; the title area shows the period dates, the player's own rank sits above the list. A ranking result the
// server has not delivered yet opens the reward popup (openRankingRewardPopup).
#pragma once

#include "rw/rw.hpp"

struct SceneRanking {
    using Addr = uint32_t;

    rw::Field<uint32_t> vtable;              // +0x000 cocos2d::Layer
    const uint8_t pad_004[0x284];
    rw::Field<uint32_t> vtable2;             // +0x288 IInOutAnimation
    rw::Field<int32_t> backTo;               // +0x28C where Back returns: 1 party top, 2 adventure select, else MyPage
    rw::Field<uint32_t> root;                // +0x290 the scene file's root node
    rw::Field<uint32_t> scrollView;          // +0x294 RankingScrollView*
    rw::Field<uint32_t> txtRank;             // +0x298 the player's own rank: label
    rw::Field<uint32_t> txtRankLabel;        // +0x29C the player's own rank: "%d%s"
    rw::Field<float> scrollX;                // +0x2A0 Vec2: the list's home position
    rw::Field<float> scrollY;                // +0x2A4
    rw::Field<int32_t> period;               // +0x2A8 1 current, 2 previous (0 before the first switch)
    rw::Field<int32_t> select;               // +0x2AC SceneManager::SelectRanking shown
    rw::Field<uint32_t> centerUI;            // +0x2B0
    rw::Field<uint32_t> centerUIButton;      // +0x2B4
    rw::Field<uint32_t> field_2B8;           // +0x2B8
    rw::Field<uint32_t> shadeBottom;         // +0x2BC
    rw::Field<uint32_t> leftUI;              // +0x2C0
    rw::Field<uint32_t> preparation;         // +0x2C4 Ranking_preparation layout ("being tallied")
    rw::Field<float> preparationX;           // +0x2C8 Vec2: its home position
    rw::Field<float> preparationY;           // +0x2CC
    rw::Field<uint8_t> changing;             // +0x2D0 a list change is running
    rw::Field<uint8_t> canBack;              // +0x2D1
    const uint8_t pad_2D2[6];

    static constexpr uint32_t SIZE = 0x2D8;
    static constexpr uint32_t VTABLE = 0xFA2E08;     // _ZTV12SceneRanking + 8
    static constexpr uint32_t VTABLE2 = 0xFA3118;    // + 0x318 (IInOutAnimation at +0x288)

    static Addr create();
    static Addr createScene();
    static SceneRanking *construct(SceneRanking *self);
    static SceneRanking *destroy(SceneRanking *self);
    static void destroyDelete(SceneRanking *self);
    static uint32_t init(SceneRanking *self);
    static void initScene(SceneRanking *self, int32_t select);
    static void initRankingTitle(SceneRanking *self);
    static void initMyRanking(SceneRanking *self);
    static void initRankingScrollView(SceneRanking *self);
    static void changeTickerMessage(SceneRanking *self, int32_t select);
    static void changeScrollView(SceneRanking *self, uint32_t animate);
    static void runInAnimation(SceneRanking *self);
    static void runOutAnimationMain(SceneRanking *self, Addr done);
    static void backScene(SceneRanking *self);

    // init's lambdas, called directly with their captures {this, text}
    static void selectPeriod(Addr captures, int32_t period, uint32_t animate);                // $_11
    static void selectCategory(Addr captures, int32_t select, uint32_t callApi, int32_t arg);  // $_14
    // changeScrollView $_15 {this, preparation text}: show the list of the chosen category and period
    static void showRanking(Addr captures);
    // backScene $_21 {bool fade, std::function<Layer*()>}: __clone() and operator()
    static Addr backCloneLambda(Addr functor);
    static void backLambda(Addr functor);
};
RW_LAYOUT(SceneRanking, vtable2, 0x288);
RW_LAYOUT(SceneRanking, backTo, 0x28C);
RW_LAYOUT(SceneRanking, scrollX, 0x2A0);
RW_LAYOUT(SceneRanking, period, 0x2A8);
RW_LAYOUT(SceneRanking, centerUI, 0x2B0);
RW_LAYOUT(SceneRanking, preparation, 0x2C4);
RW_LAYOUT(SceneRanking, changing, 0x2D0);
RW_SIZE(SceneRanking, SceneRanking::SIZE);
