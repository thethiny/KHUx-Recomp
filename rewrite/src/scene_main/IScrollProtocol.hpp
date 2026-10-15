// IScrollProtocol: the game's own scroll list (a cocos2d::ui::Layout). It keeps a fixed pool of row widgets
// (enough rows to cover the view plus three) and recycles them while scrolling: each frame the row widgets are
// repositioned from the scroll position, and when the first visible row changes every widget is told which item
// it now shows (updateRow). Rows can be laid out vertically or horizontally (scrollType), in maxColumn columns,
// with equal item sizes or a list of per-item sizes (variableSize). Touch handling gives drag, fling with
// deceleration, bounce at both ends and an optional ScrollBar.
// Multiple inheritance from Layout: vtables at +0x000, +0x280 and +0x350. The constructor is inlined into the
// derived classes (see BaseScrollView<int>::BaseScrollView for the field defaults).
#pragma once

#include "MainCommon.hpp"

struct IScrollProtocol {
    rw::Field<uint32_t> vtable;              // +0x000
    const uint8_t pad_004[0x27C];
    rw::Field<uint32_t> vtable2;             // +0x280
    const uint8_t pad_284[0xCC];
    rw::Field<uint32_t> vtable3;             // +0x350
    const uint8_t pad_354[0x4FC - 0x354];
    rw::Field<float> deceleration;           // +0x4FC share of the velocity lost per frame (0.1)
    rw::Field<float> bounceDeceleration;     // +0x500 overscroll damping (0.3)
    rw::Field<int32_t> itemCount;            // +0x504 number of items (setItemIDSize)
    rw::Field<uint32_t> field_508;           // +0x508
    rw::Field<uint32_t> layoutPanel;         // +0x50C ui::Widget* template row (retained)
    rw::Field<float> bouncePixel;            // +0x510 how far the list may be pulled past its ends (200)
    rw::Field<float> itemWidth;              // +0x514
    rw::Field<float> itemHeight;             // +0x518
    guest::Vector items;                     // +0x51C cocos2d::Vector<ui::Widget*>: the row widgets
    rw::Field<float> rowCount;               // +0x528 number of row widgets in the pool
    rw::Field<float> field_52C;              // +0x52C (10)
    rw::Field<float> field_530;              // +0x530 (32)
    rw::Field<float> field_534;              // +0x534 (4)
    rw::Field<int32_t> maxColumn;            // +0x538 items per row
    rw::Field<uint8_t> scrollEnabled;        // +0x53C
    const uint8_t pad_53D[3];
    rw::Field<float> position;               // +0x540 scroll position in pixels (0 = first item)
    rw::Field<int32_t> index;                // +0x544 first visible row
    rw::Field<float> touchStartY;            // +0x548 where the drag started
    rw::Field<float> touchStartX;            // +0x54C
    rw::Field<uint8_t> touching;             // +0x550 a finger is on the list
    rw::Field<uint8_t> dragged;              // +0x551 moved past the 20 pixel threshold
    const uint8_t pad_552[2];
    rw::Field<float> lastTouchY;             // +0x554 previous touch position
    rw::Field<float> lastTouchX;             // +0x558
    rw::Field<float> velocity;               // +0x55C
    rw::Field<float> moveDelta;              // +0x560 accumulated move in one direction (halved each touching frame)
    rw::Field<uint8_t> moving;               // +0x564 scrolling (until it stops: then the bounce check)
    rw::Field<uint8_t> bouncing;             // +0x565 animating back into range
    const uint8_t pad_566[2];
    rw::Field<float> bounceTime;             // +0x568
    rw::Field<float> bounceDistance;         // +0x56C
    rw::Field<float> offsetX;                // +0x570 row widget offset
    rw::Field<float> offsetY;                // +0x574
    rw::Field<float> minOffset;              // +0x578 added to the minimum position
    rw::Field<float> maxOffset;              // +0x57C added to the maximum position
    guest::Vector itemSizes;                 // +0x580 std::vector<float>: per-item sizes (variableSize)
    rw::Field<uint8_t> variableSize;         // +0x58C
    const uint8_t pad_58D[3];
    rw::Field<int32_t> scrollType;           // +0x590 IScrollProtocol::ScrollType: 0 vertical, 1 horizontal
    rw::Field<uint8_t> scrollBarEnabled;     // +0x594
    const uint8_t pad_595[3];
    rw::Field<uint32_t> scrollBar;           // +0x598 ScrollBar*
    guest::Vector updates;                   // +0x59C std::vector<UpdateData{row, index}>: rows to refresh
    guest::Vector pendingNew;                // +0x5A8 cocos2d::Vector<ui::Widget*> replacement widgets
    guest::Vector pendingOld;                // +0x5B4 cocos2d::Vector<ui::Widget*> widgets they replace
    rw::Field<uint8_t> updateOnePerFrame;    // +0x5C0 refresh one pending row per frame
    rw::Field<uint8_t> centering;            // +0x5C1 migrating to a centred item
    const uint8_t pad_5C2[2];
    rw::Field<float> targetPosition;         // +0x5C4
    const uint8_t onIndexChanged[24];        // +0x5C8 std::function<void(int)>: first visible row changed (1-based)
    const uint8_t onScrollEnd[24];           // +0x5E0 std::function<void(int)>: scrolling stopped (item index)
    const uint8_t onDrag[24];                // +0x5F8 std::function<void(float)>: dragged by a distance
    const uint8_t onTouchEnded[24];          // +0x610 std::function<void()>: the finger was lifted

    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x628;
    static constexpr uint32_t VTABLE = 0xF80708;         // _ZTV15IScrollProtocol + 8
    static constexpr uint32_t VTABLE2 = 0xF80A60;
    static constexpr uint32_t VTABLE3 = 0xF80A74;
    // std::function members (callable pointer at +16)
    static constexpr uint32_t OFF_ON_INDEX_CHANGED = 0x5C8, OFF_ON_SCROLL_END = 0x5E0, OFF_ON_DRAG = 0x5F8,
                              OFF_ON_TOUCH_ENDED = 0x610;

    // virtual slots (byte offsets) used by the scroll code
    static constexpr uint32_t V_getContentSize = 0xAC;
    static constexpr uint32_t V_setVisible = 0xB0;
    static constexpr uint32_t V_addChild = 0xF0;
    static constexpr uint32_t V_addChildZ = 0xF4;
    static constexpr uint32_t V_getParent = 0x11C;
    static constexpr uint32_t V_removeChild = 0x12C;
    static constexpr uint32_t V_setOpacity = 0x214;
    static constexpr uint32_t V_setTouchEnabled = 0x31C;
    static constexpr uint32_t V_createItem = 0x33C;        // (index, bool) -> ui::Widget*: a widget for an item
    static constexpr uint32_t V_isReplaceItem = 0x340;     // (widget, index, bool) -> bool: replace the widget
    static constexpr uint32_t V_registItemTouchEvent = 0x344;
    static constexpr uint32_t V_updateScroll = 0x348;
    static constexpr uint32_t V_updateRow = 0x34C;

    Addr self() const { return rw::guest_addr(this); }
    // item extent along the scroll direction (the + 0.0f is the code's own)
    float itemPixel() const { return (scrollType ? itemWidth : itemHeight) + 0.0f; }

    static Addr destroy(IScrollProtocol *self);
    static void destroyDelete(IScrollProtocol *self);
    static uint32_t init(IScrollProtocol *self);
    static void update(IScrollProtocol *self, float dt);
    // update()'s first step (inlined in every update override): queued row updates and widget replacements
    static void updatePending(IScrollProtocol *self);
    static float getMaxPosition(IScrollProtocol *self);
    static float getStartBouncePixel(IScrollProtocol *self);
    static float getBouncePixel(IScrollProtocol *self);
    static float getItemPixel(IScrollProtocol *self);
    static void setContentSize(IScrollProtocol *self, Addr size);
    static void registItemTouchEvent(IScrollProtocol *self, Addr widget, int32_t index);
    static void setItemIDSize(IScrollProtocol *self, int32_t count);
    static void updateLayout(IScrollProtocol *self, uint32_t deferred);
    static void updateRow(IScrollProtocol *self, int32_t row, int32_t first);
    static void setEnabledScrollBar(IScrollProtocol *self, uint32_t enabled);
    static void setScrollType(IScrollProtocol *self, int32_t type);
    static void setVisibleWidgets(IScrollProtocol *self, uint32_t visible);
    static float getMinPosition(IScrollProtocol *self);
    static IScrollProtocol *setMaxColumn(IScrollProtocol *self, int32_t columns);
    static void setIndex(IScrollProtocol *self, int32_t index);
    static void setIndexPrev(IScrollProtocol *self, int32_t index, int32_t prev);
    static IScrollProtocol *setEnabledScroll(IScrollProtocol *self, uint32_t enabled);
    static IScrollProtocol *resetScroll(IScrollProtocol *self);
    static void updateScroll(IScrollProtocol *self, float delta);
    static int32_t getScrollIndex(IScrollProtocol *self);
    static uint32_t containsPointInScrollArea(IScrollProtocol *self, Addr point);
    static int32_t getItemIndex(IScrollProtocol *self, int32_t local);
    static int32_t getLocalItemIndex(IScrollProtocol *self, Addr point);
    static Addr getLayoutPanel(IScrollProtocol *self);
    static uint32_t migrationBounceCenterPosition(IScrollProtocol *self, int32_t step);
    static uint32_t migrationLeft(IScrollProtocol *self, int32_t step);
    static uint32_t migrationRight(IScrollProtocol *self, int32_t step);
    static void checkScrollInterval(IScrollProtocol *self, const float *pos);

    // init()'s helper and lambdas (functor = the __func object: vtable, captures at +4 / +8)
    static void assignTouchEnded(Addr function, Addr captures);
    static void onExitLambda(Addr functor);                          // $_4
    static void onEnterLambda(Addr functor);                         // $_3
    static void onTouchEndedLambda(Addr functor, Addr touch, Addr event);   // $_2 [this, this]
    static void onTouchMovedLambda(Addr functor, Addr touch, Addr event);   // $_1 [this, this]
    static uint32_t onTouchBeganLambda(Addr functor, Addr touch, Addr event);  // $_0 [this]
};
RW_LAYOUT(IScrollProtocol, vtable2, 0x280);
RW_LAYOUT(IScrollProtocol, vtable3, 0x350);
RW_LAYOUT(IScrollProtocol, deceleration, 0x4FC);
RW_LAYOUT(IScrollProtocol, items, 0x51C);
RW_LAYOUT(IScrollProtocol, rowCount, 0x528);
RW_LAYOUT(IScrollProtocol, maxColumn, 0x538);
RW_LAYOUT(IScrollProtocol, position, 0x540);
RW_LAYOUT(IScrollProtocol, touching, 0x550);
RW_LAYOUT(IScrollProtocol, velocity, 0x55C);
RW_LAYOUT(IScrollProtocol, moving, 0x564);
RW_LAYOUT(IScrollProtocol, bounceTime, 0x568);
RW_LAYOUT(IScrollProtocol, itemSizes, 0x580);
RW_LAYOUT(IScrollProtocol, scrollType, 0x590);
RW_LAYOUT(IScrollProtocol, scrollBar, 0x598);
RW_LAYOUT(IScrollProtocol, updates, 0x59C);
RW_LAYOUT(IScrollProtocol, pendingOld, 0x5B4);
RW_LAYOUT(IScrollProtocol, updateOnePerFrame, 0x5C0);
RW_LAYOUT(IScrollProtocol, targetPosition, 0x5C4);
RW_LAYOUT(IScrollProtocol, onIndexChanged, 0x5C8);
RW_LAYOUT(IScrollProtocol, onTouchEnded, 0x610);
RW_SIZE(IScrollProtocol, IScrollProtocol::SIZE);
