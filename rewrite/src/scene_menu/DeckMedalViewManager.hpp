// DeckMedalViewManager: the singleton that knows every live DeckMedalView. It caches the cocostudio layouts the
// views clone (small / normal medal panel, the info panel and the "open info" panel) and drives the blinking of
// the status / fusion plates: a 2 s show, 1 s cross-fade, 2 s show, 1 s cross-fade back cycle whose two
// opacities are pushed to every view each frame.
#pragma once

#include "../scene/SceneUi.hpp"

struct DeckMedalViewManager {
    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x24;
    static constexpr uint32_t INSTANCE = 0x1003BC0;    // static DeckMedalViewManager* (getInstance)

    rw::Field<int32_t> state;            // +0x00 blink phase 0-3
    rw::Field<float> timer;              // +0x04 seconds in the phase
    rw::Field<uint32_t> viewsBegin;      // +0x08 cocos2d::Vector<DeckMedalView*> (std::vector)
    rw::Field<uint32_t> viewsEnd;        // +0x0C
    rw::Field<uint32_t> viewsCap;        // +0x10
    rw::Field<uint32_t> smallPanel;      // +0x14 DeckEdit_SmallMedal.json (retained)
    rw::Field<uint32_t> normalPanel;     // +0x18 DeckEdit_RevolverMedal_ver120.json
    rw::Field<uint32_t> medalInfoPanel;  // +0x1C DeckEdit_Deck_MedalInfo_ver120.json
    rw::Field<uint32_t> openInfoPanel;   // +0x20 DeckEdit_OpenInfo.json

    static Addr getInstance();
    static DeckMedalViewManager *construct(DeckMedalViewManager *self);
    static DeckMedalViewManager *destruct(DeckMedalViewManager *self);
    static void update(DeckMedalViewManager *self, float dt);
    static void addDeckMedalView(Addr self, Addr view);
    static void removeDeckMedalView(Addr self, Addr view);
    static void updateDeckMedalViews(DeckMedalViewManager *self);
    static Addr getSmallMedalPanel(DeckMedalViewManager *self);
    static Addr getNormalMedalPanel(DeckMedalViewManager *self);
    static Addr getMedalInfoPanel(DeckMedalViewManager *self);
    static Addr getOpenInfoPanel(DeckMedalViewManager *self);
};
RW_LAYOUT(DeckMedalViewManager, viewsBegin, 0x08);
RW_LAYOUT(DeckMedalViewManager, smallPanel, 0x14);
RW_LAYOUT(DeckMedalViewManager, openInfoPanel, 0x20);
RW_SIZE(DeckMedalViewManager, 0x24);
