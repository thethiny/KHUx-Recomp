// BaseEndlessrollView: a carousel of rows (cocos2d::Node). Rows are clones of a template widget laid out by the
// subclass (Set_disp_param) at `pos` degrees around a wheel; dragging moves `pos`, a release keeps the wheel
// spinning with `speed` (decelerating), then it snaps to the nearest item (snapTime over 0.3 s, a sine ease).
// get_offset_move folds `pos` into an item step and an offset, setSub re-lays the rows and refills the ones that
// changed item (changeRow). Lists of fewer than six items stop at both ends instead of wrapping.
//
// Used by SphereScrollView (the Avatar Board list). All guest objects are 32-bit guest addresses.
#pragma once

#include "rw/rw.hpp"
#include "../core/Guest.hpp"

struct BaseEndlessrollView {
    using Addr = uint32_t;

    rw::Field<uint32_t> vtable;              // +0x000 cocos2d::Node
    const uint8_t pad_004[0x26C];
    const uint8_t onFunction270[0x18];       // +0x270 std::function (callable pointer at +0x10)
    const uint8_t onFunction288[0x18];       // +0x288
    const uint8_t onScroll[0x18];            // +0x2A0 std::function<void(float)>
    const uint8_t onFunction2B8[0x18];       // +0x2B8
    const uint8_t onMove[0x18];              // +0x2D0 std::function<void(float)>: the wheel moved (speed)
    const uint8_t onStop[0x18];              // +0x2E8 std::function<void(int)>: the wheel stopped on an item
    const uint8_t onTapRow[0x18];            // +0x300 std::function<void(int, ui::Widget*)>
    rw::Field<float> pos;                    // +0x318 wheel angle inside the current item step
    rw::Field<int32_t> index;                // +0x31C item at the centre row
    rw::Field<int32_t> field_320;            // +0x320
    rw::Field<uint8_t> locked;               // +0x324 no more scrolling
    const uint8_t pad_325[3];
    rw::Field<uint32_t> rowTemplate;         // +0x328 ui::Widget* the rows are cloned from
    rw::Field<int32_t> rowCount;             // +0x32C rows on the wheel
    rw::Field<int32_t> centerRow;            // +0x330 the row at the front
    guest::Vector rows;                      // +0x334 cocos2d::Vector<ui::Widget*>
    rw::Field<int32_t> field_340;            // +0x340
    rw::Field<uint8_t> field_344;            // +0x344
    const uint8_t pad_345[3];
    rw::Field<int32_t> itemCount;            // +0x348 items (1 until data is set)
    rw::Field<uint8_t> moveForward;          // +0x34C auto move direction
    rw::Field<uint8_t> autoMove;             // +0x34D moving on its own (arrow buttons)
    const uint8_t pad_34E[2];
    rw::Field<float> field_350;              // +0x350 45
    const uint8_t pad_354[0xC];
    rw::Field<int32_t> field_360;            // +0x360
    rw::Field<int32_t> field_364;            // +0x364
    rw::Field<uint8_t> touching;             // +0x368 a finger is on the wheel
    rw::Field<uint8_t> field_369;            // +0x369
    rw::Field<uint8_t> field_36A;            // +0x36A
    rw::Field<uint8_t> field_36B;            // +0x36B
    rw::Field<uint8_t> fast;                 // +0x36C four update steps per frame
    const uint8_t pad_36D[3];
    rw::Field<int32_t> field_370;            // +0x370
    rw::Field<float> speed;                  // +0x374 degrees per update step
    rw::Field<int32_t> field_378;            // +0x378
    const uint8_t pad_37C[0x10];
    rw::Field<int32_t> field_38C;            // +0x38C
    rw::Field<uint8_t> released;             // +0x390 the finger left: snap when the wheel stops
    rw::Field<uint8_t> snapping;             // +0x391
    const uint8_t pad_392[2];
    rw::Field<float> halfStep;               // +0x394 step / 2
    rw::Field<float> snapTime;               // +0x398
    rw::Field<float> snapDistance;           // +0x39C
    rw::Field<float> snapStart;              // +0x3A0
    rw::Field<uint8_t> field_3A4;            // +0x3A4
    const uint8_t pad_3A5[3];
    rw::Field<int32_t> field_3A8;            // +0x3A8
    rw::Field<int32_t> field_3AC;            // +0x3AC
    rw::Field<float> step;                   // +0x3B0 degrees between two items
    rw::Field<int32_t> stepCount;            // +0x3B4 items the wheel wraps over

    static constexpr uint32_t SIZE = 0x3B8;
    static constexpr uint32_t VTABLE = 0xF843C4;     // _ZTV19BaseEndlessrollView + 8

    // virtual slots (byte offsets in the vtable)
    static constexpr uint32_t V_GET_OFFSET_MOVE = 0x274;
    static constexpr uint32_t V_CHANGE_ROW = 0x278;
    static constexpr uint32_t V_SET_DISP_PARAM = 0x27C;
    static constexpr uint32_t V_SET_SUB = 0x280;
    static constexpr uint32_t V_UPDATE_ADD_MOVE = 0x284;
    static constexpr uint32_t V_UPDATE_MOVE_MAX_CHECK = 0x288;

    static BaseEndlessrollView *construct(BaseEndlessrollView *self);
    static BaseEndlessrollView *destroy(BaseEndlessrollView *self);
    static Addr createRow(Addr self);
    static uint32_t init(BaseEndlessrollView *self, int32_t index);
    static void update(BaseEndlessrollView *self, float dt);
    static float degree2radian(Addr self, float degree);
    static void get_offset_move(BaseEndlessrollView *self, float angle, Addr stepOut, Addr offsetOut);
    static void setSub(BaseEndlessrollView *self, uint32_t refill, float angle);
};
RW_LAYOUT(BaseEndlessrollView, onFunction270, 0x270);
RW_LAYOUT(BaseEndlessrollView, onMove, 0x2D0);
RW_LAYOUT(BaseEndlessrollView, onStop, 0x2E8);
RW_LAYOUT(BaseEndlessrollView, onTapRow, 0x300);
RW_LAYOUT(BaseEndlessrollView, pos, 0x318);
RW_LAYOUT(BaseEndlessrollView, rowTemplate, 0x328);
RW_LAYOUT(BaseEndlessrollView, rows, 0x334);
RW_LAYOUT(BaseEndlessrollView, itemCount, 0x348);
RW_LAYOUT(BaseEndlessrollView, field_350, 0x350);
RW_LAYOUT(BaseEndlessrollView, field_360, 0x360);
RW_LAYOUT(BaseEndlessrollView, touching, 0x368);
RW_LAYOUT(BaseEndlessrollView, fast, 0x36C);
RW_LAYOUT(BaseEndlessrollView, speed, 0x374);
RW_LAYOUT(BaseEndlessrollView, field_38C, 0x38C);
RW_LAYOUT(BaseEndlessrollView, released, 0x390);
RW_LAYOUT(BaseEndlessrollView, halfStep, 0x394);
RW_LAYOUT(BaseEndlessrollView, snapStart, 0x3A0);
RW_LAYOUT(BaseEndlessrollView, field_3A8, 0x3A8);
RW_LAYOUT(BaseEndlessrollView, step, 0x3B0);
RW_SIZE(BaseEndlessrollView, BaseEndlessrollView::SIZE);
