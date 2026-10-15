// BaseStageScrollView: a cocos2d::ui::Layout that scrolls a column (or row) of stage rows, the base of
// AdvStageScrollView (the quest / event / raid / colosseum stage lists). Like IScrollProtocol it keeps a fixed pool
// of row widgets (enough to cover the view plus five rows) and recycles them while scrolling: each frame the widgets
// are placed from the scroll position, and when the first visible row changes every widget is told which item it
// now shows (updateRow). Touch handling (init's lambdas) gives drag, fling with deceleration and bounce at both
// ends. Secondary vtables at +640 and +848. Layout fields end at +1276; this class's fields span +1276..+1616
// (AdvStageScrollView's own start at +1616).
//
// scrollType (+1552): 0 vertical, 1 horizontal, 2 vertical with centring, 3 horizontal with centring. The code
// tests (type | 2) == 2 for "vertical" (rows stacked downwards, item extent = itemHeight) and (type & ~1) == 2 for
// "centred" (the first / last item may sit in the middle of the view; a stop snaps to the nearest item).
//
// OWNERSHIP: offsets and the total size (1616) are fixed - AdvStageScrollView.hpp derives from this struct.
// Members may be renamed/split (keep existing names working) only by the BaseStageScrollView agent. The original
// member names (field_57C, field_5B8, field_5D4...) stay as union aliases of the descriptive names.
#pragma once

#include "../core/Guest.hpp"
#include "../actor/FlashNode.hpp"  // StdFunction

struct BaseStageScrollView {
    // +1504 / +1505 (stored together as a halfword by the constructor)
    struct Motion {
        rw::Field<uint8_t> moving;           // +1504 released and coasting (set by touch end, cleared when stopped)
        rw::Field<uint8_t> bouncing;         // +1505 animating back into range / to the target position
    };

    rw::Field<uint32_t> vtable;              // +0     (_ZTV19BaseStageScrollView + 8 = 0xF8CDC0)
    const uint8_t pad_004[636];              // +4     cocos2d::ui::Layout
    rw::Field<uint32_t> vtable2;             // +640   (0xF8D118)
    const uint8_t pad_284[204];              // +644
    rw::Field<uint32_t> vtable3;             // +848   (0xF8D12C)
    const uint8_t pad_354[424];              // +852   ... Layout ends at +1276
    rw::Field<float> field_4FC;              // +1276  default bounce duration (0.3 s), copied to bounceDuration by init
    StdFunction fn0;                         // +1280  onIndexChanged: function<void(int)> first item index + 1, when
                                             //        the first visible row changes during a touch
    const uint8_t pad_514[4];                // +1300
    StdFunction fn1;                         // +1304  onScrollEnd: function<void(int)> item it stopped on (1 - pos/px)
    const uint8_t pad_52C[4];                // +1324
    StdFunction fn2;                         // +1328  onDrag: function<void(float)> dragged distance
    const uint8_t pad_544[4];                // +1348
    StdFunction fn3;                         // +1352  onTouchEnded: function<void()> the finger was lifted
    const uint8_t pad_55C[4];                // +1372
    StdFunction fn4;                         // +1376  onBounceEnd: function<void()> a bounce / migration finished
    const uint8_t pad_574[8];                // +1396
    union {
        rw::Field<uint32_t> field_57C;       // +1404
        rw::Field<float> itemWidth;          // +1404  item extent along a horizontal list
    };
    union {
        rw::Field<uint32_t> field_580;       // +1408
        rw::Field<float> itemHeight;         // +1408  item extent along a vertical list
    };
    guest::Vector itemIDs;                   // +1412  std::vector<int> the items (stage ids)
    guest::Vector widgets;                   // +1424  cocos2d::Vector<ui::Widget*> the row widget pool
    rw::Field<float> rowCount;               // +1436  number of row widgets in the pool
    rw::Field<uint8_t> field_5A0;            // +1440  touches ignored (checked first by the touch lambdas)
    rw::Field<uint8_t> field_5A1;            // +1441
    const uint8_t pad_5A2[2];                // +1442
    rw::Field<float> minSpeed;               // +1444  coasting speed floor / stop threshold (10, set by init)
    rw::Field<float> speedDivisor;           // +1448  velocity / 32 is added to the per-frame deceleration
    rw::Field<float> dragDivisor;            // +1452  touch move / 4 is added to the velocity
    rw::Field<float> bounceDuration;         // +1456  seconds (field_4FC)
    rw::Field<float> bounceExtra;            // +1460  extra seconds for migrationMaxPosition (0.1 per item)
    union {
        rw::Field<int32_t> field_5B8;        // +1464  1 in the constructor
        rw::Field<int32_t> maxColumn;        // +1464  items per row
    };
    union {
        rw::Field<uint8_t> field_5BC;        // +1468
        rw::Field<uint8_t> touchLocked;      // +1468  touches ignored
    };
    union {
        rw::Field<uint8_t> field_5BD;        // +1469  1 in the constructor
        rw::Field<uint8_t> scrollEnabled;    // +1469
    };
    const uint8_t pad_5BE[2];                // +1470
    rw::Field<float> position;               // +1472  scroll position in pixels (0 = first item)
    rw::Field<int32_t> index;                // +1476  first visible item (row * maxColumn)
    rw::Field<float> touchStartY;            // +1480  where the drag started
    rw::Field<float> touchStartX;            // +1484
    rw::Field<uint8_t> touching;             // +1488  a finger is on the list
    rw::Field<uint8_t> dragged;              // +1489  moved past the 20 pixel threshold
    const uint8_t pad_5D2[2];                // +1490
    union {
        rw::Field<uint32_t> field_5D4;       // +1492
        rw::Field<float> lastTouchY;         // +1492  previous touch position
    };
    union {
        rw::Field<uint32_t> field_5D8;       // +1496
        rw::Field<float> lastTouchX;         // +1496
    };
    union {
        rw::Field<uint32_t> field_5DC;       // +1500
        rw::Field<float> velocity;           // +1500
    };
    union {
        rw::Field<uint16_t> field_5E0;       // +1504
        Motion motion;                       // +1504  moving / bouncing
    };
    const uint8_t pad_5E2[2];                // +1506
    rw::Field<float> bounceTime;             // +1508  seconds into the bounce
    rw::Field<float> bounceDistance;         // +1512  distance still to cover when the bounce started
    rw::Field<float> targetPosition;         // +1516  where the bounce ends
    rw::Field<float> offsetX;                // +1520  row widget offset
    rw::Field<float> offsetY;                // +1524
    rw::Field<float> minOffset;              // +1528  added to the minimum position
    rw::Field<float> maxOffset;              // +1532  added to the maximum position
    guest::Vector floats;                    // +1536  std::vector<float> (per-item sizes; unused by the base)
    rw::Field<uint8_t> variableSize;         // +1548  setItemIDs leaves the layout to the derived class
    const uint8_t pad_60D[3];                // +1549
    rw::Field<int32_t> scrollType;           // +1552  0 / 2 vertical, 1 / 3 horizontal (2 / 3 centred)
    guest::Vector updateData;                // +1556  std::vector<UpdateData{row, first}> rows to refresh
    guest::Vector widgetsA;                  // +1568  cocos2d::Vector<ui::Widget*> replacement widgets (to add)
    guest::Vector widgetsB;                  // +1580  cocos2d::Vector<ui::Widget*> widgets they replace (to remove)
    rw::Field<uint8_t> updateOnePerFrame;    // +1592  apply one queued row update / replacement per frame
    const uint8_t pad_tail[23];              // +1593..+1616

    static constexpr uint32_t VTABLE = 0xF8CDC0, VTABLE2 = 0xF8D118, VTABLE3 = 0xF8D12C;

    // item extent along the scroll direction, without the code's "+ 0.0f"
    float itemPixelRaw() const { return (scrollType | 2) == 2 ? itemHeight : itemWidth; }
    bool vertical() const { return (scrollType | 2) == 2; }
    bool centred() const { return (scrollType & ~1) == 2; }

    static BaseStageScrollView *construct(BaseStageScrollView *self);
    static BaseStageScrollView *destroy(BaseStageScrollView *self);
    static uint32_t init(BaseStageScrollView *self);
    static void update(BaseStageScrollView *self, float dt);
    static void updateRow(BaseStageScrollView *self, int32_t row, int32_t first);
    static float getMinPosition(BaseStageScrollView *self);
    static float getMaxPosition(BaseStageScrollView *self);
    static float getItemPixel(BaseStageScrollView *self);
    static uint32_t migrationBounceCenterPosition(BaseStageScrollView *self, int32_t step);
    static void setContentSize(BaseStageScrollView *self, uint32_t size);
    static void setItemIDs(BaseStageScrollView *self, uint32_t ids);
    static void updateLayout(BaseStageScrollView *self, uint32_t deferred);
    static void setVisibleWidgets(BaseStageScrollView *self, uint32_t visible);
    static float getScrollIntervalPixel(BaseStageScrollView *self);
    static void setIndex(BaseStageScrollView *self, int32_t index);
    static void updateScroll(BaseStageScrollView *self, float delta);
    static void checkScrollInterval(BaseStageScrollView *self, const float *pos);
    static uint32_t migrationMaxPosition(BaseStageScrollView *self);
    static uint32_t isScroll(BaseStageScrollView *self);

    // init()'s lambdas (functor = the __func object: vtable, captures this at +4 / +8)
    static void assignTouchEnded(uint32_t function, uint32_t captures);
    static uint32_t onTouchBeganLambda(uint32_t functor, uint32_t touchRef, uint32_t eventRef);   // $_0 [this]
    static void onTouchMovedLambda(uint32_t functor, uint32_t touchRef, uint32_t eventRef);       // $_1 [this, this]
    static void onTouchEndedLambda(uint32_t functor, uint32_t touchRef, uint32_t eventRef);       // $_2 [this, this]
};
RW_LAYOUT(BaseStageScrollView, updateData, 1556);
RW_LAYOUT(BaseStageScrollView, widgetsB, 1580);
RW_LAYOUT(BaseStageScrollView, vtable2, 640);
RW_LAYOUT(BaseStageScrollView, vtable3, 848);
RW_LAYOUT(BaseStageScrollView, field_4FC, 1276);
RW_LAYOUT(BaseStageScrollView, fn0, 1280);
RW_LAYOUT(BaseStageScrollView, fn4, 1376);
RW_LAYOUT(BaseStageScrollView, field_57C, 1404);
RW_LAYOUT(BaseStageScrollView, itemHeight, 1408);
RW_LAYOUT(BaseStageScrollView, itemIDs, 1412);
RW_LAYOUT(BaseStageScrollView, widgets, 1424);
RW_LAYOUT(BaseStageScrollView, rowCount, 1436);
RW_LAYOUT(BaseStageScrollView, minSpeed, 1444);
RW_LAYOUT(BaseStageScrollView, bounceExtra, 1460);
RW_LAYOUT(BaseStageScrollView, field_5B8, 1464);
RW_LAYOUT(BaseStageScrollView, scrollEnabled, 1469);
RW_LAYOUT(BaseStageScrollView, position, 1472);
RW_LAYOUT(BaseStageScrollView, touching, 1488);
RW_LAYOUT(BaseStageScrollView, field_5D4, 1492);
RW_LAYOUT(BaseStageScrollView, velocity, 1500);
RW_LAYOUT(BaseStageScrollView, field_5E0, 1504);
RW_LAYOUT(BaseStageScrollView, bounceTime, 1508);
RW_LAYOUT(BaseStageScrollView, maxOffset, 1532);
RW_LAYOUT(BaseStageScrollView, floats, 1536);
RW_LAYOUT(BaseStageScrollView, variableSize, 1548);
RW_LAYOUT(BaseStageScrollView, scrollType, 1552);
RW_LAYOUT(BaseStageScrollView, updateOnePerFrame, 1592);
RW_SIZE(BaseStageScrollView, 1616);
