// StageUI: the quest screen's HUD owner (a cocos2d::Ref, not a node). It creates and keeps the HUD parts the
// battle scene adds to its layers: the medal deck, the carousel deck, the status field (score counters for rare
// items / items / munny / Lux), the attribute compatibility chart, the player status indicator, the Lux target,
// the heading LWF ("Battle start", "Clear"... banners) and the tutorial overlay widget. It also opens the in-quest
// dialogs (retire, continue, battle menu). Created by SceneBattleBase::init_UI; one instance per quest.
#pragma once

#include "BattleUi.hpp"

struct StageUI : bui::Ref {
    rw::Field<uint32_t> medalField;        // +20  MedalDeck* (retained)
    rw::Field<uint32_t> carouselDeck;      // +24  CarouselDeck* (retained)
    rw::Field<uint32_t> actionField;       // +28  ActionMap_ActionField widget (retained, unused in 1.0.1)
    rw::Field<uint32_t> statusField;       // +32  ActionMap_StatusField widget (retained)
    rw::Field<uint32_t> attributeField;    // +36  ActionMap_AttributeCompatibility widget (retained)
    rw::Field<uint32_t> playerStatusField; // +40  PlayerStatusIndicator* (retained)
    rw::Field<uint32_t> luxTargetField;    // +44  ActionMap_LuxNode widget (retained)
    rw::Field<uint32_t> luxBoard;          // +48  LuxBoard* (createHeading)
    rw::Field<uint32_t> rareItemScore;     // +52  status field node "ScoreField_RareItem"
    rw::Field<uint32_t> itemScore;         // +56  "ScoreField_Item"
    rw::Field<uint32_t> mannyScore;        // +60  "ScoreField_Manny"
    rw::Field<uint32_t> luxScore;          // +64  "ScoreField_Lux"
    rw::Field<uint32_t> itemLabel;         // +68  ui::Text "Item_Label"
    rw::Field<uint32_t> rareItemLabel;     // +72  ui::Text "RareItem_Label"
    rw::Field<uint32_t> mannyLabel;        // +76  ui::Text "Manny_Label"
    rw::Field<uint32_t> luxLabel;          // +80  ui::Text "Lux_Label"
    rw::Field<int32_t> shownItems;         // +84  value last written to itemLabel (updateDispUI)
    rw::Field<int32_t> shownRareItems;     // +88  rareItemLabel
    rw::Field<int32_t> shownManny;         // +92  mannyLabel
    rw::Field<int32_t> shownLux;           // +96  luxLabel
    rw::Field<uint32_t> heading;           // +100 LWFNodeGroup* of the heading banners (retained)
    rw::Field<float> headingX;             // +104 cocos2d::Vec2: the heading position (screen centre)
    rw::Field<float> headingY;             // +108
    rw::Field<uint8_t> battleTurn;         // +112 setBattleTurn
    const uint8_t pad_113[7];                    // +113
    const uint8_t tutorialCallback[20];          // +120 std::function (callable pointer at +136)
    rw::Field<uint32_t> field_8C;          // +140
    rw::Field<uint32_t> tutorialWidget;    // +144 StageUI::TutorialWidget* (retained)
    rw::Field<uint32_t> field_94;          // +148

    static constexpr uint32_t SIZE = 0x98;
    static constexpr uint32_t VTABLE = 0xFA883C;   // _ZTV7StageUI + 8

    // ---- StageUI.cpp: life cycle, HUD parts, headings, score display
    static StageUI *create();
    static StageUI *construct(StageUI *self);
    static StageUI *destroy(StageUI *self);
    static void destroyDelete(StageUI *self);
    static uint32_t init(StageUI *self);
    static uint32_t deleteTutorialWidget(StageUI *self);
    static uint32_t getMedalField(const StageUI *self);
    static uint32_t getCarouselDeck(const StageUI *self);
    static uint32_t getActionField(const StageUI *self);
    static uint32_t getStatusField(const StageUI *self);
    static uint32_t getAttributeField(const StageUI *self);
    static uint32_t getPlayerStatusField(const StageUI *self);
    static uint32_t getLuxTargetField(const StageUI *self);
    static uint32_t getLuxTarget(const StageUI *self);
    static uint32_t getRareTreasureTarget(const StageUI *self);
    static uint32_t getCommonTreasureTarget(const StageUI *self);
    static uint32_t createMedalField(StageUI *self);
    static uint32_t createCarouselDeck(StageUI *self);
    static uint32_t createActionField(StageUI *self);
    static uint32_t createStatusField(StageUI *self, uint32_t unused);
    static uint32_t createAttributeField(StageUI *self);
    static uint32_t createPlayerStatusField(StageUI *self, uint32_t a, uint32_t b, uint32_t style);
    static uint32_t createLuxTargetField(StageUI *self);
    static uint32_t shake_RareItem(StageUI *self);
    static uint32_t shake_Item(StageUI *self);
    static uint32_t shake_Manny(StageUI *self);
    static uint32_t shake_Lux(StageUI *self);
    static uint32_t createHeading(StageUI *self);
    static void showHeading(StageUI *self, int32_t heading);
    static void garbageCollectHeading(StageUI *self);
    static int32_t getLastShowHeading(const StageUI *self);
    static uint32_t isPlayingHeading(const StageUI *self);
    static void updateDispUI(StageUI *self);
    static void set_start_avatar_parameter(StageUI *self);
    static void setBattleTurn(StageUI *self, uint32_t turn);

    // ---- declarations owned by helper files (one .inc per helper, included here)
#include "StageUI_Menu.inc"
#include "StageUI_Lambdas.inc"
};
RW_LAYOUT(StageUI, medalField, 20);
RW_LAYOUT(StageUI, luxLabel, 80);
RW_LAYOUT(StageUI, heading, 100);
RW_LAYOUT(StageUI, battleTurn, 112);
RW_LAYOUT(StageUI, tutorialCallback, 120);
RW_LAYOUT(StageUI, tutorialWidget, 144);
RW_SIZE(StageUI, StageUI::SIZE);
