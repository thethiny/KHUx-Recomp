// StartDeckEditDialog: the "start quest" dialog of the adventure select (cocostudio StartDeckEdit2Scene_ver120.json):
// the keyblade deck pager (a BaseSlideView of the player's keyblades), the support character list
// (SupportCharacterScrollView), the quest info (raid boss / colosseum), the skip-ticket button and OK. OK sends the
// stage / colosseum / raid start request and changes to the battle scene.
#pragma once

#include "W4B1Common.hpp"

struct StartDeckEditDialog {
    // StageType (stageType): 0, 1 story / event, 2 colosseum, 3 raid boss (current), 4 raid from the raid list
    const uint8_t node[0x270];                // cocos2d::Node
    rw::Field<uint32_t> parent;               // +0x270 the node the dialog was opened on
    rw::Field<uint32_t> shadow;               // +0x274 modal shadow sprite
    rw::Field<uint32_t> root;                 // +0x278 the cocostudio scene
    rw::Field<uint8_t> closing;               // +0x27C
    rw::Field<uint8_t> started;               // +0x27D the quest start was requested
    const uint8_t pad_27E[2];
    const uint8_t fn_280[24];                 // +0x280 std::function (callable pointer at +0x290)
    const uint8_t onClose[24];                // +0x298 std::function<void(bool)>: the callback given to open()
    rw::Field<uint32_t> deckSlide;            // +0x2B0 BaseSlideView* of the keyblade decks
    rw::Field<uint32_t> leftUI;               // +0x2B4 LeftUI (pager arrow)
    rw::Field<uint32_t> rightUI;              // +0x2B8 RightUI
    rw::Field<uint8_t> notColosseum;          // +0x2BC stageType != 2
    rw::Field<uint8_t> supportOpen;           // +0x2BD the support list is shown
    const uint8_t pad_2BE[2];
    rw::Field<uint32_t> supportScroll;        // +0x2C0 SupportCharacterScrollView*
    rw::Field<float> field_2C4;               // +0x2C4 a cocos2d::Vec2
    rw::Field<float> field_2C8;               // +0x2C8
    rw::Field<uint32_t> avatarIconCache;      // +0x2CC AvatarIconCache*
    rw::Field<uint8_t> field_2D0;             // +0x2D0
    rw::Field<uint8_t> useSkipTicket;         // +0x2D1 passed to executeStageStart
    const uint8_t pad_2D2[2];
    rw::Field<int32_t> stageType;             // +0x2D4
    rw::Field<int32_t> stageIndex;            // +0x2D8 colosseum stage / raid list index
    guest::Vector campaigns;                  // +0x2DC vector<shared_ptr<const master::EvCampaign>>
    rw::Field<uint32_t> closeButton;          // +0x2E8 Button_Close
    const uint8_t pad_2EC[4];

    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x2F0;
    static constexpr uint32_t VTABLE = 0xF8AC38 + 8;    // _ZTV19StartDeckEditDialog + 8

    static Addr construct(Addr self);                    // C2
    static Addr destroy(Addr self);                      // D2
    static Addr create();
    static uint32_t init(Addr self);
    static void update(Addr self, float dt);
    static uint32_t open(Addr self, Addr parent, int32_t type, int32_t index, Addr callback);
    static void initLayout(Addr self);
    static void initInfo(Addr self);
    static void initDeckEdit(Addr self);
    static void updateDeckLayout(Addr self);
    static void openTutorial106(Addr self, Addr done);
    static void openTutorial107(Addr self, Addr done);
    static void openTutorial108(Addr self, Addr done);
    static void openTutorial112(Addr self, Addr done);
    static void updateDeckView(Addr self, Addr widget, int32_t deck, uint32_t flag);
    static void initSupport(Addr self);
    static void showSupportUser(Addr captures, int32_t index);   // sub_5765C4
    static void close(Addr self);
    static void supportScrollRunInAnimation(Addr self);
    static void supportScrollRunOutAnimation(Addr self);
    static void startAdventure(Addr self);
    static void startStory(Addr self);
    static void startColiseum(Addr self);
    static void startRaid(Addr self);
    static uint32_t isCostOverCurrentKeyblade(Addr self);
};
RW_LAYOUT(StartDeckEditDialog, parent, 0x270);
RW_LAYOUT(StartDeckEditDialog, closing, 0x27C);
RW_LAYOUT(StartDeckEditDialog, fn_280, 0x280);
RW_LAYOUT(StartDeckEditDialog, onClose, 0x298);
RW_LAYOUT(StartDeckEditDialog, deckSlide, 0x2B0);
RW_LAYOUT(StartDeckEditDialog, notColosseum, 0x2BC);
RW_LAYOUT(StartDeckEditDialog, supportScroll, 0x2C0);
RW_LAYOUT(StartDeckEditDialog, avatarIconCache, 0x2CC);
RW_LAYOUT(StartDeckEditDialog, stageType, 0x2D4);
RW_LAYOUT(StartDeckEditDialog, campaigns, 0x2DC);
RW_LAYOUT(StartDeckEditDialog, closeButton, 0x2E8);
RW_SIZE(StartDeckEditDialog, StartDeckEditDialog::SIZE);
