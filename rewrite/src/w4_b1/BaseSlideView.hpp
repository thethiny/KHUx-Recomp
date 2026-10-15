// BaseSlideView: a horizontal pager (a cocos2d::ui::Layout). It shows one item at a time from a list of item ids;
// three page widgets (current, next on the right, previous on the left) sit in a container layer that is dragged
// with the finger and slid one screen width left / right (0.15 s MoveTo) to change page. Page widgets are made by
// the createItem callback (std::function<Widget*(Widget*, int id, bool)>, which may reuse the widget passed in);
// after a slide, update() refreshes the pages queued in `updates` / `updateIndexes`.
// Multiple inheritance from Layout: vtables at +0x000, +0x280 and +0x350 (IScrollProtocol-like bases).
#pragma once

#include "W4B1Common.hpp"

struct BaseSlideView {
    // UpdateTarget: which page widget a queued update refreshes
    enum UpdateTarget : int32_t { CURRENT = 0, PREV = 1, NEXT = 2 };

    rw::Field<uint32_t> vtable;              // +0x000
    const uint8_t pad_004[0x27C];
    rw::Field<uint32_t> vtable2;             // +0x280
    const uint8_t pad_284[0xCC];
    rw::Field<uint32_t> vtable3;             // +0x350
    const uint8_t pad_354[0x500 - 0x354];
    const uint8_t fn_500[24];                // +0x500 std::function (callable pointer at +0x510)
    const uint8_t createItem[24];            // +0x518 std::function<Widget*(Widget*, int, bool)>
    const uint8_t onSlideStart[24];          // +0x530 std::function<void()>: a page slide starts
    const uint8_t onMove[24];                // +0x548 std::function<void(float)>: dragged (distance)
    const uint8_t onTouchEnd[24];            // +0x560 std::function<void()>: the finger was lifted
    guest::Vector itemIDs;                   // +0x578 std::vector<int>
    rw::Field<uint32_t> container;           // +0x584 cocos2d::Layer* holding the page widgets
    rw::Field<uint32_t> currentNode;         // +0x588 ui::Widget*
    rw::Field<uint32_t> prevNode;            // +0x58C ui::Widget* (one screen to the left)
    rw::Field<uint32_t> nextNode;            // +0x590 ui::Widget* (one screen to the right)
    guest::Vector updates;                   // +0x594 std::vector<UpdateTarget>
    guest::Vector updateIndexes;             // +0x5A0 std::vector<int>: the item id each queued update shows
    rw::Field<int32_t> index;                // +0x5AC current item index
    rw::Field<float> baseX;                  // +0x5B0 the container's x when the touch began
    rw::Field<float> touchStartX;            // +0x5B4
    rw::Field<float> touchTime;              // +0x5B8 seconds the finger is down (counted up to 0.3)
    rw::Field<float> screenWidth;            // +0x5BC
    rw::Field<uint8_t> touching;             // +0x5C0
    rw::Field<uint8_t> animating;            // +0x5C1 a slide / bounce runs
    rw::Field<uint8_t> slideEnabled;         // +0x5C2 (more than one item)
    const uint8_t pad_5C3[1];
    rw::Field<uint32_t> listener;            // +0x5C4 EventListenerTouchOneByOne*
    rw::Field<uint8_t> updated;              // +0x5C8 update() refreshed a side page
    rw::Field<uint8_t> dragging;             // +0x5C9 moved past 20 pixels
    const uint8_t pad_5CA[6];

    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x5D0;
    static constexpr uint32_t VTABLE_BASE = 0xF85B58;   // _ZTV13BaseSlideView
    static constexpr uint32_t VTABLE = VTABLE_BASE + 8;
    static constexpr uint32_t VTABLE2 = VTABLE_BASE + 0x34C;
    static constexpr uint32_t VTABLE3 = VTABLE_BASE + 0x360;

    static Addr create();
    static Addr destroy(Addr self);                       // D2
    static uint32_t init(Addr self);
    static void update(Addr self, float dt);
    static void moveNextToPrevNode(Addr self);
    static void movePrevToNextNode(Addr self);
    static void updateLayout(Addr self, uint32_t queueSides);
    static void setItemIDs(Addr self, Addr ids, uint32_t queueSides);
    static void setSlideEnabled(Addr self, uint32_t enabled);
    static void onCompleteAnimation(Addr self, uint32_t delayed);
    static uint32_t changeNextPage(Addr self);
    static void runRightSlide(Addr self);
    static uint32_t changePreviousPage(Addr self);
    static void runLeftSlide(Addr self);
    static float getMoveX(Addr self, float x);

    // lambdas
    static void assignTouchEndLambda(Addr target, Addr captures);        // sub_525A20
    static uint32_t onTouchBegan(Addr fn, Addr touchRef, Addr eventRef);  // init::$_0
    static void onTouchMoved(Addr fn, Addr touchRef, Addr eventRef);      // init::$_1
    static void onTouchEnded(Addr fn, Addr touchRef, Addr eventRef);      // init::$_2
    static void onCompleteDelayed(Addr fn);                               // onCompleteAnimation::$_3
    static void onBounceDone(Addr fn);                                    // init::$_6
};
RW_LAYOUT(BaseSlideView, vtable2, 0x280);
RW_LAYOUT(BaseSlideView, vtable3, 0x350);
RW_LAYOUT(BaseSlideView, createItem, 0x518);
RW_LAYOUT(BaseSlideView, itemIDs, 0x578);
RW_LAYOUT(BaseSlideView, container, 0x584);
RW_LAYOUT(BaseSlideView, updates, 0x594);
RW_LAYOUT(BaseSlideView, index, 0x5AC);
RW_LAYOUT(BaseSlideView, screenWidth, 0x5BC);
RW_LAYOUT(BaseSlideView, listener, 0x5C4);
RW_LAYOUT(BaseSlideView, dragging, 0x5C9);
RW_SIZE(BaseSlideView, BaseSlideView::SIZE);
