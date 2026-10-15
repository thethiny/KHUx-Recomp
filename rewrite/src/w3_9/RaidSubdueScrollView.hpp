// RaidSubdueScrollView: the horizontal strip of raid boss cards in the raid list dialog (one card per subdued /
// discovered boss: "found by", "MVP", "MVP 2", "entrant" or a rare-boss icon card, with the damage dealt, the
// player's name and their avatar icon). A BaseScrollView<int> whose item ids are the card numbers; the card data
// (SubdueData, 80 bytes each) lives in a std::map<int, SubdueData> keyed by card number. Cards are centred one
// at a time (migrationLeft / migrationRight bounce to the next card) and the list can auto-advance.
#pragma once

#include "BaseScrollView.hpp"

struct RaidSubdueScrollView : BaseScrollView {
    rw::Field<float> bounceDurationDefault;  // +0x634 (0.2) copied to bounceDuration by init
    rw::Field<float> autoScrollInterval;     // +0x638 (0.3) seconds between automatic steps
    rw::Field<uint8_t> tapAnimating;         // +0x63C touches are ignored while set
    const uint8_t pad_63D[3];
    rw::Field<float> autoScrollTimer;        // +0x640
    rw::Field<uint8_t> autoScrollLeft;       // +0x644 step direction of the automatic scroll
    rw::Field<uint8_t> autoScrolling;        // +0x645 automatic scroll running (cleared at either end)
    const uint8_t pad_646[2];
    guest::SharedPtr iconCache;              // +0x648 std::shared_ptr<AvatarIconCache>: the players' avatar icons
    rw::Field<uint32_t> subdueBegin;         // +0x650 std::map<int, SubdueData>: begin node
    rw::Field<uint32_t> subdueRoot;          // +0x654 end node (its left = root)
    rw::Field<uint32_t> subdueSize;          // +0x658
    guest::Vector actors;                    // +0x65C std::vector<AvatarActor*>
    rw::Field<float> bounceDuration;         // +0x668 duration of a centring bounce
    rw::Field<float> bounceExtra;            // +0x66C added to it (reset when a bounce ends)
    rw::Field<uint8_t> itemTouching;         // +0x670 a touch began on a card
    const uint8_t pad_671[3];
    rw::Field<int32_t> touchedItem;          // +0x674 the card it began on
    rw::Field<uint32_t> field_678;           // +0x678
    const uint8_t pad_67C[4];

    static constexpr uint32_t SIZE = 0x680;
    static constexpr uint32_t VTABLE = 0xF8E148, VTABLE2 = 0xF8E4A0, VTABLE3 = 0xF8E4B4;
    static constexpr uint32_t OFF_SUBDUE = 0x650;    // the std::map

    // SubdueData (80 bytes): +0 boss id, +4 count, +8 rare flag (byte), +12 damage, +16 std::string player name,
    // +28 card type (1 found, 2 MVP, 3 MVP 2, 4 rare boss icon, else entrant), +32 the avatar coordinate.
    static constexpr uint32_t SUBDUE_SIZE = 80;

    static Addr create();
    static RaidSubdueScrollView *construct(RaidSubdueScrollView *self);
    static RaidSubdueScrollView *destroy(RaidSubdueScrollView *self);
    static void destroyDelete(RaidSubdueScrollView *self);
    static uint32_t init(RaidSubdueScrollView *self);
    static void update(RaidSubdueScrollView *self, float dt);
    static void updateScroll(RaidSubdueScrollView *self, float delta);
    static uint32_t reflectionList(RaidSubdueScrollView *self, Addr list);
    static Addr createRow(RaidSubdueScrollView *self, int32_t index, int32_t arg);
    static uint32_t changeRow(RaidSubdueScrollView *self, Addr widget, int32_t index, int32_t arg);
    static void setDrawFrame(RaidSubdueScrollView *self, Addr widget, int32_t index);
    static void registItemTouchEvent(RaidSubdueScrollView *self, Addr widget, int32_t index);
    static void reSetWidthAndOffsetX(RaidSubdueScrollView *self, int32_t columns);

    // registItemTouchEvent's lambdas (RaidSubdueScrollView_lambdas.cpp)
    static void assignTouchEnded(Addr function, Addr captures);   // std::function<void(Touch*,Event*)> = $_2
    static void onTouchMoved(Addr functor, Addr touch, Addr event);   // $_1 {view, Rect}
    static void onTouchEnded(Addr functor, Addr touch, Addr event);   // $_2 {view}
};
RW_LAYOUT(RaidSubdueScrollView, bounceDurationDefault, 0x634);
RW_LAYOUT(RaidSubdueScrollView, tapAnimating, 0x63C);
RW_LAYOUT(RaidSubdueScrollView, autoScrollTimer, 0x640);
RW_LAYOUT(RaidSubdueScrollView, autoScrolling, 0x645);
RW_LAYOUT(RaidSubdueScrollView, iconCache, 0x648);
RW_LAYOUT(RaidSubdueScrollView, subdueBegin, 0x650);
RW_LAYOUT(RaidSubdueScrollView, actors, 0x65C);
RW_LAYOUT(RaidSubdueScrollView, bounceDuration, 0x668);
RW_LAYOUT(RaidSubdueScrollView, itemTouching, 0x670);
RW_LAYOUT(RaidSubdueScrollView, touchedItem, 0x674);
RW_SIZE(RaidSubdueScrollView, RaidSubdueScrollView::SIZE);
