// SceneAdventureSelect: the quest selection screen (story chapters, events, colosseum, raid). A cocos2d::Layer
// with a second base, IInOutAnimation, at +0x288 (its runInAnimation / runOutAnimationMain are reached through the
// thunks at this - 0x288). The cocostudio scene "AdventureTop2Scene_ver130.json" is held in `root`; its parts are
// AdventurePanel records (a widget plus its original position, so the in / out animations can slide it back).
//
// Lambda bodies (operator() of the $_N lambdas) live in SceneAdventureSelect_lambdas.cpp, the std::function slots
// in SceneAdventureSelect_thunks.cpp. All guest objects are 32-bit guest addresses.
#pragma once

#include "../core/Guest.hpp"
#include "../scene/SceneUi.hpp"

struct SceneAdventureSelect {
    // AdventurePanel (12 bytes): the widget's position when it was found (a cocos2d::Vec2) and the widget.
    // AdventurePanel::searchWidget(panel, root, name) fills it; the run*Animation helpers slide it from / to `pos`.
    struct Panel {
        rw::Field<float> x;                     // +0 original position
        rw::Field<float> y;                     // +4
        rw::Field<uint32_t> node;               // +8 cocos2d::ui::Widget*
    };

    // AdventureType (field adventureType): 0 story top, 1..4 event kinds, 5 colosseum, 6 story stage list,
    // 7 adventure select (as SceneAdventureSelect::init derives it from SceneManager +0x28C)
    const uint8_t layer[0x288];                  // cocos2d::Layer (Node std::function members at +0x210 / +0x228)
    rw::Field<uint32_t> inOutVtable;             // +0x288 IInOutAnimation base
    rw::Field<int32_t> adventureType;            // +0x28C the shown AdventureType
    rw::Field<uint32_t> txtFilter;               // +0x290 RightUI/Txt_Filter (ui::Text)
    rw::Field<uint8_t> raidInfoActive;           // +0x294 a raid boss is up: update() refreshes its time left
    const uint8_t pad_295[3];
    rw::Field<uint32_t> field_298;               // +0x298
    rw::Field<uint8_t> flag_29C;                 // +0x29C
    rw::Field<uint8_t> flag_29D;                 // +0x29D
    rw::Field<uint8_t> flag_29E;                 // +0x29E
    rw::Field<uint8_t> flag_29F;                 // +0x29F copied from SceneManager in init()
    rw::Field<uint8_t> flag_2A0;                 // +0x2A0
    const uint8_t pad_2A1[3];
    rw::Field<uint32_t> root;                    // +0x2A4 the cocostudio scene node
    rw::Field<uint32_t> advButton;               // +0x2A8 Adv_Button
    rw::Field<uint32_t> advPanelArea;            // +0x2AC Adv_Panel_Area
    rw::Field<uint32_t> shadeLeft;               // +0x2B0 Shade_Left
    rw::Field<uint32_t> shadeRight;              // +0x2B4 Shade_Right
    rw::Field<uint32_t> buttonFilter;            // +0x2B8 RightUI_Button_Filter
    Panel announce;                              // +0x2BC Announce
    rw::Field<uint32_t> field_2C8;               // +0x2C8
    rw::Field<uint32_t> leftUiBack;              // +0x2CC LeftUI_Buck (the back button's panel)
    rw::Field<float> leftUiBackX;                // +0x2D0 its original position
    rw::Field<float> leftUiBackY;                // +0x2D4
    rw::Field<uint8_t> scrollMoving;             // +0x2D8 update(): an arrow scroll of the stage list is running
    rw::Field<uint8_t> scrollLeft;               // +0x2D9 ... towards the left (else right)
    rw::Field<uint8_t> scrollInfoOut;            // +0x2DA ... the info panel was already slid out
    const uint8_t pad_2DB[1];
    rw::Field<float> scrollTime;                 // +0x2DC ... seconds held (moves after 0.3 s)
    rw::Field<int32_t> scrollSpeed;              // +0x2E0 ... migration step (grows up to 5)
    rw::Field<uint8_t> flag_2E4;                 // +0x2E4
    const uint8_t pad_2E5[3];
    Panel cursor;                                // +0x2E8 Cursor
    Panel cursorArrow;                           // +0x2F4 Cursor_Arrow
    rw::Field<uint32_t> stageScrollView;         // +0x300 AdvStageScrollView* (BaseStageScrollView)
    rw::Field<uint32_t> leftArrow;               // +0x304 LeftUI/Left_arrow
    rw::Field<uint32_t> rightArrow;              // +0x308 RightUI/Right_arrow
    rw::Field<float> field_30C;                  // +0x30C a cocos2d::Vec2
    rw::Field<float> field_310;                  // +0x310
    rw::Field<float> leftArrowX;                 // +0x314 Left_arrow's original position
    rw::Field<float> leftArrowY;                 // +0x318
    rw::Field<float> rightArrowX;                // +0x31C Right_arrow's original position
    rw::Field<float> rightArrowY;                // +0x320
    rw::Field<uint8_t> flag_324;                 // +0x324
    rw::Field<uint8_t> flag_325;                 // +0x325 (1 after construction)
    const uint8_t pad_326[2];
    rw::Field<int32_t> eventIndex;               // +0x328 index into APIManager's StageEvent::Event list
    rw::Field<uint32_t> field_32C;               // +0x32C
    Panel eventSelect;                           // +0x330 Event_Select
    Panel eventSelectLeftUI;                     // +0x33C Event_Select_LeftUI
    rw::Field<uint8_t> eventTimerOn;             // +0x348 updateEventTimeChack refreshes Txt_Limit
    const uint8_t pad_349[3];
    guest::Vector bannerNames;                   // +0x34C std::vector<std::string>
    guest::Vector bannerDates;                   // +0x358 std::vector<Date>
    rw::Field<uint32_t> field_364;               // +0x364
    rw::Field<uint32_t> field_368;               // +0x368
    rw::Field<float> eventTimer;                 // +0x36C seconds until Txt_Limit is refreshed
    rw::Field<uint32_t> deferment;               // +0x370 "deferment" node
    Panel advSelect;                             // +0x374 Adv_Select
    Panel story;                                 // +0x380 Story
    Panel event;                                 // +0x38C Event
    Panel colosseum;                             // +0x398 Colosseum
    Panel raid;                                  // +0x3A4 Raid
    Panel panel_3B0;                             // +0x3B0 (slide out to the right)
    Panel panel_3BC;                             // +0x3BC
    Panel panel_3C8;                             // +0x3C8
    Panel panel_3D4;                             // +0x3D4
    Panel filterButton;                          // +0x3E0 Button_Filter (RightUI)
    Panel filterText;                            // +0x3EC Txt_Filter (RightUI)
    rw::Field<uint32_t> cursorAnimAP;            // +0x3F8 cocostudio::Armature "Cursor_Anim_AP"
    rw::Field<uint8_t> flag_3FC;                 // +0x3FC
    rw::Field<uint8_t> flag_3FD;                 // +0x3FD
    const uint8_t pad_3FE[2];

    static constexpr uint32_t SIZE = 0x400;
    static constexpr uint32_t VTABLE_START = 0xFA5870;           // _ZTV20SceneAdventureSelect
    static constexpr uint32_t VTABLE = VTABLE_START + 8;
    static constexpr uint32_t INOUT_VTABLE = VTABLE_START + 0x318;
    static constexpr uint32_t INOUT_OFFSET = 0x288;               // IInOutAnimation base (thunks: this - 0x288)

    using Addr = uint32_t;

    // ---- named functions (SceneAdventureSelect.cpp and parts)
    static Addr create();
    static Addr construct(Addr self);                              // C2
    static void callAdventureSelectAPI(Addr callback);             // static; callback: std::function<void(bool)>*
    static Addr createScene();
    static Addr destroy(Addr self);                                // D2
    static void destroyDelete(Addr self);                          // D0
    static uint32_t init(Addr self);
    static void changeOutAnimation(Addr self, Addr done);
    static void initStageRaidChack(Addr self, Addr done);
    static void openTutorial31(Addr self, Addr done);
    static void update(Addr self, float dt);
    static void updateStageRaidChack(Addr self, float dt);
    static void updateEventTimeChack(Addr self, float dt);
    static void infoOutAnimation(Addr self);
    static void changeAdventure(Addr self, int32_t type, uint32_t flag);
    static void initAdvScrollView(Addr self, int32_t type);
    static void initStory(Addr self);
    static void initEventBanner(Addr self, uint32_t flag);
    static void initEvent(Addr self);
    static void initAdvSelect(Addr self);
    static void storyInAnimation(Addr self, Addr done);
    static void changeStageInfo(Addr self);
    static void eventBannerDisps(Addr self);
    static void eventBannerInAnimation(Addr self);
    static void eventInAnimation(Addr self);
    static void initColosseum(Addr self);
    static void colosseumInAnimation(Addr self);
    static void openTutorial25(Addr self, Addr done);
    static void setStageScrollViewItemsStory(Addr self);
    static void setStageScrollViewItemsEvent(Addr self);
    static void setStageScrollViewItemsColosseum(Addr self);
    static void initSelectStage(Addr self, int32_t type);
    static void scrollRunOutAnimation(Addr self, Addr done);
    static void backButtonRunOutAnimation(Addr self, Addr done);
    static void backButtonRunInAnimation(Addr self);
    static void scrollRunInAnimation(Addr self, Addr done);
    static void colosseumOpenAnimation(Addr self, Addr done);
    static void raidBossAnim(Addr self, Addr done);
    static void raidBossEscapeAnim(Addr self, Addr done);
    static void raidBossWinAnim(Addr self, Addr done);
    static void raidBossRewardPopup(Addr self, Addr done);
    static uint32_t openColosseumCheck(Addr self);
    static void openPopupStartDeckEdit(Addr self);
    static void checkApStory(Addr self);
    static void checkAp(Addr self, int32_t ap, Addr done);
    static void checkApEvent(Addr self);
    static void checkApColosseum(Addr self);
    static uint32_t isScroll(Addr self);
    static void openTutorial13(Addr self, Addr done);
    static void openTutorial33(Addr self, Addr done);
    static void openTutorial34(Addr self, Addr done);
    static void openTutorial104(Addr self, Addr done);
    static void openTutorial105(Addr self, Addr done);
    static void backScene(Addr self);
    static void runInAnimation(Addr self);
    static void runOutAnimationMain(Addr self, Addr done);
};
RW_LAYOUT(SceneAdventureSelect, inOutVtable, 0x288);
RW_LAYOUT(SceneAdventureSelect, adventureType, 0x28C);
RW_LAYOUT(SceneAdventureSelect, raidInfoActive, 0x294);
RW_LAYOUT(SceneAdventureSelect, flag_29F, 0x29F);
RW_LAYOUT(SceneAdventureSelect, root, 0x2A4);
RW_LAYOUT(SceneAdventureSelect, announce, 0x2BC);
RW_LAYOUT(SceneAdventureSelect, leftUiBack, 0x2CC);
RW_LAYOUT(SceneAdventureSelect, scrollMoving, 0x2D8);
RW_LAYOUT(SceneAdventureSelect, scrollTime, 0x2DC);
RW_LAYOUT(SceneAdventureSelect, cursor, 0x2E8);
RW_LAYOUT(SceneAdventureSelect, stageScrollView, 0x300);
RW_LAYOUT(SceneAdventureSelect, leftArrowX, 0x314);
RW_LAYOUT(SceneAdventureSelect, flag_324, 0x324);
RW_LAYOUT(SceneAdventureSelect, eventIndex, 0x328);
RW_LAYOUT(SceneAdventureSelect, eventSelect, 0x330);
RW_LAYOUT(SceneAdventureSelect, eventTimerOn, 0x348);
RW_LAYOUT(SceneAdventureSelect, bannerNames, 0x34C);
RW_LAYOUT(SceneAdventureSelect, eventTimer, 0x36C);
RW_LAYOUT(SceneAdventureSelect, deferment, 0x370);
RW_LAYOUT(SceneAdventureSelect, advSelect, 0x374);
RW_LAYOUT(SceneAdventureSelect, raid, 0x3A4);
RW_LAYOUT(SceneAdventureSelect, filterButton, 0x3E0);
RW_LAYOUT(SceneAdventureSelect, cursorAnimAP, 0x3F8);
RW_SIZE(SceneAdventureSelect, SceneAdventureSelect::SIZE);
