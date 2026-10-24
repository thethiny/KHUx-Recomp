// BaseEndlessrollView: construction, the rows and their touch listeners (init), the wheel's motion (update) and the
// row layout (get_offset_move / setSub). See BaseEndlessrollView.hpp. The lambdas' __func thunks are in
// w3_0/BaseEndlessrollView_thunks.cpp; updateAddMove / updateMoveMaxCheck and the lambda bodies are not rewritten.
#include "BaseEndlessrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using Self = BaseEndlessrollView;

namespace {
namespace g {
constexpr uint32_t Node_ctor = rwlib::cocos2d::Node::ctor_base;
constexpr uint32_t Node_dtor = rwlib::cocos2d::Node::dtor_base;
constexpr uint32_t Node_scheduleUpdate = rwlib::cocos2d::Node::scheduleUpdate;
constexpr uint32_t Node_unscheduleUpdate = rwlib::cocos2d::Node::unscheduleUpdate;
constexpr uint32_t WidgetVector_dtor = 0x485A8F;            // cocos2d::Vector<ui::Widget*>::~Vector
constexpr uint32_t WidgetVector_clear = 0x485B45;
constexpr uint32_t WidgetVector_pushBack = 0x4F6B5D;
constexpr uint32_t Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;                    // Vec2(float, float)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t TouchBoolFunction_swap = 0x483E4D;       // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t TouchVoidFunction_swap = 0x4F3699;       // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t function_vf_call = 0x4F8E89;             // std::function<void(float)>::operator()
constexpr uint32_t function_vi_call = 0x4F321D;             // std::function<void(int)>::operator()
constexpr uint32_t libm_sin = 0x415A30;                     // sin(double) (import)
}  // namespace g

constexpr uint32_t S_SelectIconJson = 0xDFD831;   // "cocostudio/publish/SphereBoard_Select_Icon_ver120.json"
constexpr uint32_t S_Sprite = 0xDFD170;           // "Sprite"

constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;
// EventListenerTouchOneByOne callbacks
constexpr uint32_t ON_TOUCH_BEGAN = 0x50, ON_TOUCH_MOVED = 0x68, ON_TOUCH_ENDED = 0x80, ON_TOUCH_CANCELLED = 0x98;

// init's lambdas (__func vtables + 8)
constexpr uint32_t VT_RowBegan = 0xF56914;        // $_0 {this, row, sprite}: the front row
constexpr uint32_t VT_RowMoved = 0xF568E8;        // $_1 {this, row, sprite}
constexpr uint32_t VT_RowEnded = 0xF568BC;        // $_2 {this, row}
constexpr uint32_t VT_RowCancelled = 0xF56890;    // $_3 {this, row}
constexpr uint32_t VT_SideBegan = 0xF56864;       // $_4 {this, row, sprite}: the rows next to it
constexpr uint32_t VT_SideMoved = 0xF56838;       // $_5 {this}
constexpr uint32_t VT_SideEnded = 0xF5680C;       // $_6 {this, row index}
constexpr uint32_t VT_SideCancelled = 0xF567E0;   // $_7 {this}
constexpr uint32_t VT_WheelBegan = 0xF567B4;      // $_8 {this}: the wheel
constexpr uint32_t VT_WheelMoved = 0xF56788;      // $_9 {this}
constexpr uint32_t VT_WheelEnded = 0xF5675C;      // $_10 {this}
constexpr uint32_t VT_WheelCancelled = 0xF56730;  // $_11 {this}

constexpr float SNAP_TIME = 0.3f;
constexpr double PI = 3.14159265358979311600;     // 0x400921FB54442D18

double dbits(uint64_t u) { double d; std::memcpy(&d, &u, 8); return d; }
uint64_t bitsd(double d) { uint64_t u; std::memcpy(&u, &d, 8); return u; }

// sin through the guest's libm import (the host's may round the last bit differently)
double guestSin(double x) {
    const uint64_t b = bitsd(x);
    return dbits(rw::call64(g::libm_sin, {uint32_t(b), uint32_t(b >> 32)}));
}

// __divsi3 (the divisors here are item steps, never 0 in the game; the guest's division by zero returns 0)
int32_t idiv(int32_t a, int32_t b) { return b == 0 ? 0 : a / b; }

// listener->onTouchX = lambda (the std::function is built on the stack, swapped in, the old one destroyed)
void setTouchCallback(uint32_t listener, uint32_t field, uint32_t swap, uint32_t vtable,
                      std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t cb = newFunction(f, vtable, captures);
    call(swap, {cb, listener + field});
    functionDestroy(cb);
}

void addListener(uint32_t listener, uint32_t node) {
    uint32_t dispatcher = ld32(call(fn::Director_getInstance, {}) + DIRECTOR_EVENT_DISPATCHER);
    call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, node});
}

// One frame of the release's deceleration: speed -= speed / 16 + 0.1 toward 0, 0 once it falls inside (-1, 1).
float decelerate(float s) {
    if (s >= 1.0f) {
        float t = s - (s * 0.0625f + 0.1f);
        return t < 1.0f ? 0.0f : t;
    }
    float t = s - (s * 0.0625f + -0.1f);
    return t > -1.0f ? 0.0f : t;
}
// The angle a spin step moves by: the speed, at least 1 degree either way (a stopped wheel steps -1).
float spinStep(float s) {
    if (s > 0.0f) return s < 1.0f ? 1.0f : s;
    return s > -1.0f ? -1.0f : s;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self *Self::construct(Self *self) {
    const Addr me = rw::guest_addr(self);
    call(g::Node_ctor, {me});
    self->vtable = VTABLE;
    for (uint32_t fnAt : {0x270u, 0x288u, 0x2A0u, 0x2B8u, 0x2D0u, 0x2E8u, 0x300u}) st32(me + fnAt + 0x10, 0);
    self->field_344 = 0;
    self->locked = 0;
    self->field_320 = 0;
    self->index = 0;
    self->pos = 0.0f;
    for (uint32_t k = 0; k < 0x18; k += 4) st32(me + 0x328 + k, 0);    // template, rows, centre row, row vector
    self->itemCount = 1;
    self->moveForward = 0;
    self->autoMove = 0;
    self->field_350 = 45.0f;
    self->touching = 0;
    self->field_369 = 0;
    self->field_364 = 0;
    self->field_360 = 0;
    self->field_36A = 1;
    self->field_36B = 0;
    self->fast = 0;
    self->field_370 = 0;
    self->speed = 0.0f;
    self->field_378 = 0;
    self->field_38C = 0;
    self->snapping = 0;
    self->released = 0;
    for (uint32_t k = 0; k < 0x10; k += 4) st32(me + 0x394 + k, 0);    // half step, snap time / distance / start
    self->field_3A4 = 0;
    self->stepCount = 0;
    self->step = 0.0f;
    self->field_3AC = 0;
    self->field_3A8 = 0;
    return self;
}
RW_FUNCTION(0x6BE09C, "BaseEndlessrollView::BaseEndlessrollView", BaseEndlessrollView::construct);

Self *Self::destroy(Self *self) {
    const Addr me = rw::guest_addr(self);
    self->vtable = VTABLE;
    call(g::Node_unscheduleUpdate, {me});
    call(g::WidgetVector_dtor, {me + 0x334});
    for (uint32_t fnAt : {0x300u, 0x2E8u, 0x2D0u, 0x2B8u, 0x2A0u, 0x288u, 0x270u}) functionDestroy(me + fnAt);
    call(g::Node_dtor, {me});
    return self;
}
RW_FUNCTION(0x516510, "BaseEndlessrollView::~BaseEndlessrollView", BaseEndlessrollView::destroy);

// The board icon a row shows (a static helper: `this` is not used).
BaseEndlessrollView::Addr Self::createRow(Addr) {
    uint32_t reader = call(fn::GUIReader_getInstance, {});
    uint32_t widget = call(fn::GUIReader_widgetFromJsonFile, {reader, S_SelectIconJson});
    Frame f;
    uint32_t anchor = f.alloc(8);
    call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
    vcall(widget, node::setAnchorPoint, {widget, anchor});
    call(g::Vec2_dtor, {anchor});
    return widget;
}
RW_FUNCTION(0x516764, "BaseEndlessrollView::createRow", BaseEndlessrollView::createRow);

// Build the rows (rowCount clones of the template, each holding a board icon named "Sprite"), give the front row
// and the two on each side of it touch listeners, lay them out at angle 0 and listen to drags on the wheel.
uint32_t Self::init(Self *self, int32_t index) {
    const Addr me = rw::guest_addr(self);
    self->touching = 0;
    self->field_360 = 0;
    self->field_340 = 0;
    call(g::WidgetVector_clear, {me + 0x334});
    self->index = index;
    self->halfStep = self->step * 0.5f;
    if (!self->rowTemplate || !self->rowCount) return 0;
    if (self->rowCount >= 1) {
        for (int32_t i = 0; i < self->rowCount; i++) {
            uint32_t row = call(g::Widget_clone, {self->rowTemplate});
            uint32_t sprite = createRow(row);
            {
                Frame f;
                uint32_t name = newStr(f, S_Sprite, 6);
                vcall(sprite, node::setName, {sprite, name});
                strFree(name);
            }
            vcall(row, node::addChild, {row, sprite});
            vcall(me, node::addChild, {me, row});
            call(g::WidgetVector_pushBack, {me + 0x334, row});
            const int32_t center = self->centerRow;
            if (i == center) {
                uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
                call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
                setTouchCallback(listener, ON_TOUCH_BEGAN, g::TouchBoolFunction_swap, VT_RowBegan, {me, row, sprite});
                setTouchCallback(listener, ON_TOUCH_MOVED, g::TouchVoidFunction_swap, VT_RowMoved, {me, row, sprite});
                setTouchCallback(listener, ON_TOUCH_ENDED, g::TouchVoidFunction_swap, VT_RowEnded, {me, row});
                setTouchCallback(listener, ON_TOUCH_CANCELLED, g::TouchVoidFunction_swap, VT_RowCancelled, {me, row});
                addListener(listener, row);
            } else if (i < center + 3 && i > center - 3) {
                uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
                call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
                setTouchCallback(listener, ON_TOUCH_BEGAN, g::TouchBoolFunction_swap, VT_SideBegan, {me, row, sprite});
                setTouchCallback(listener, ON_TOUCH_MOVED, g::TouchVoidFunction_swap, VT_SideMoved, {me});
                setTouchCallback(listener, ON_TOUCH_ENDED, g::TouchVoidFunction_swap, VT_SideEnded, {me, uint32_t(i)});
                setTouchCallback(listener, ON_TOUCH_CANCELLED, g::TouchVoidFunction_swap, VT_SideCancelled, {me});
                addListener(listener, row);
            }
        }
        for (int32_t k = 0; k < self->rowCount; k++) vcall(me, V_SET_DISP_PARAM, {me, uint32_t(k), 0});
    }
    uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    setTouchCallback(listener, ON_TOUCH_BEGAN, g::TouchBoolFunction_swap, VT_WheelBegan, {me});
    setTouchCallback(listener, ON_TOUCH_MOVED, g::TouchVoidFunction_swap, VT_WheelMoved, {me});
    setTouchCallback(listener, ON_TOUCH_ENDED, g::TouchVoidFunction_swap, VT_WheelEnded, {me});
    setTouchCallback(listener, ON_TOUCH_CANCELLED, g::TouchVoidFunction_swap, VT_WheelCancelled, {me});
    addListener(listener, me);
    call(g::Node_scheduleUpdate, {me});
    return 1;
}
RW_FUNCTION(0x5167F4, "BaseEndlessrollView::init", BaseEndlessrollView::init);

// ------------------------------------------------------------------------------------------------ motion
// Per frame: auto move (arrow buttons, speed +/-4, stopping at the ends of a short list) or the released wheel's
// spin (pos += speed, decelerating; four steps a frame when fast), then the snap to the nearest item: when the
// wheel stops after a release the snap starts (index advanced by the whole steps, the rest eased away over 0.3 s
// along a sine), and when it ends onStop gets the item. onMove gets the speed every spin frame.
void Self::update(Self *self, float dt) {
    const Addr me = rw::guest_addr(self);
    Frame f;
    const uint32_t out = f.alloc(8);              // get_offset_move: offset (float) at +0, item steps at +4
    const auto addMove = [&] { vcall(me, V_UPDATE_ADD_MOVE, {me, 0}); };     // updateAddMove(0): re-lay the rows
    const auto moveMaxCheck = [&] { vcall(me, V_UPDATE_MOVE_MAX_CHECK, {me, 0, 0}); };
    const uint8_t autoMove = self->autoMove;
    float s;
    float off, old, p;
    int32_t n, idx;
    if (autoMove) {
        const uint8_t forward = self->moveForward;
        s = forward ? 4.0f : -4.0f;
        self->speed = s;
        n = self->itemCount;
        if (n >= 6) {
            if (!self->released) goto slowDown;
            goto spin;
        }
        idx = self->index;
        if (idx <= 0 && forward) {
            s = 0.0f;
            self->speed = 0.0f;
        }
        if (idx >= n - 1 && !forward) {
            self->speed = 0.0f;
            if (self->released) goto startSnap;
            goto snapStep;
        }
    } else {
        s = self->speed;
    }
    {
        const uint8_t released = self->released;
        if (s == 0.0f) {
            if (!released) goto snapStep;
            goto startSnap;
        }
        if (!released) goto slowDown;
    }

spin:
    p = spinStep(s) + self->pos;
    self->pos = p;
    if (self->fast && !autoMove) {
        for (int k = 0; k < 3; k++) {
            s = decelerate(s);
            self->speed = s;
            p = spinStep(s) + p;
        }
        self->pos = p;
    }
    n = self->itemCount;
    if (n <= 5) {
        vcall(me, V_GET_OFFSET_MOVE, {me, fbits(p), out + 4, out});
        const int32_t steps = int32_t(ld32(out + 4));
        if (steps != 0) {
            const int32_t v = self->index + steps;
            const bool atEnd = v <= 0 ? n >= 2 : (n >= 2 && v >= n - 1);
            if (atEnd) {
                self->speed = 0.0f;
                self->autoMove = 0;
                moveMaxCheck();
                addMove();
                return;
            }
        }
    }
    moveMaxCheck();
    addMove();
    s = self->speed;

slowDown:
    s = decelerate(s);
    self->speed = s;
    if (ld32(me + 0x2D0 + 0x10)) call(g::function_vf_call, {me + 0x2D0, fbits(s)});

snapStep:
    if (!self->snapping) return;
    if (self->snapTime < SNAP_TIME) {
        const auto tick = [&] {
            const float t = self->snapTime + dt;
            const float clamped = t >= SNAP_TIME ? SNAP_TIME : t;
            self->snapTime = clamped;
            const double start = double(float(self->snapStart));
            const double distance = double(float(self->snapDistance));
            const float angle = float(double((clamped / SNAP_TIME) * 90.0f) * PI / 180.0);
            self->pos = float(start + distance * guestSin(double(angle)));
        };
        tick();
        if (self->fast) {
            for (int k = 0; k < 3; k++) tick();
        }
        addMove();
        return;
    }
    p = self->snapStart + self->snapDistance;
    self->pos = p;
    if (self->fast) {
        vcall(me, V_GET_OFFSET_MOVE, {me, fbits(p), out + 4, out});
        const int32_t steps = int32_t(ld32(out + 4));
        if (steps != 0) {
            self->index = self->index + steps;
            self->pos = ldf(out);
        }
    }
    addMove();
    self->snapping = 0;
    self->fast = 0;
    self->snapTime = 0.0f;
    self->snapDistance = 0.0f;
    self->snapStart = 0.0f;
    if (ld32(me + 0x2E8 + 0x10)) {
        int32_t item = 0;
        const float at = self->pos;
        if (!(at > 0.0f)) {
            const float step = self->step;
            const int32_t divisor = step < 1.0f ? 1 : f2i(step);
            item = 1 - idiv(f2i(at), divisor);
        }
        call(g::function_vi_call, {me + 0x2E8, uint32_t(item)});
    }
    return;

startSnap:
    if (self->touching) goto snapStep;
    self->released = 0;
    self->snapping = 1;
    self->snapTime = 0.0f;
    old = self->pos;
    vcall(me, V_GET_OFFSET_MOVE, {me, fbits(old), out + 4, out});
    off = ldf(out);
    self->pos = off;
    idx = self->index + int32_t(ld32(out + 4));
    self->index = idx;
    n = self->itemCount;
    if (n >= 5) {
        if (idx >= n) {
            idx %= n;
            self->index = idx;
        } else if (idx < 0) {
            idx = n + idx;
            self->index = idx;
        }
    }
    {
        // a short list (2..5 items) does not wrap: at either end the wheel only eases back to the end item
        const bool shortList = uint32_t(n - 2) <= 3;
        if (idx <= 0 && shortList) {
            self->index = 0;
            if (old > 0.0f) {
                const float d = -0.0f - off;
                self->snapDistance = d;
                if (off == -0.0f) {
                    self->snapTime = SNAP_TIME;
                } else if (!(self->halfStep > d || std::isnan(float(self->halfStep)) || std::isnan(d))) {
                    self->snapDistance = 0.0f;
                    self->snapTime = SNAP_TIME;
                }
                self->snapStart = off;
                return;
            }
        } else if (shortList && idx >= n - 1) {
            self->index = n - 1;
            if (old < 0.0f) {
                const float d = -0.0f - off;
                self->snapDistance = d;
                if (off == -0.0f) {
                    self->snapTime = SNAP_TIME;
                } else if (!(off < self->halfStep || std::isnan(off) || std::isnan(float(self->halfStep)))) {
                    self->snapDistance = 0.0f;
                    self->snapTime = SNAP_TIME;
                }
                self->snapStart = off;
                return;
            }
        } else if (n < 2) {
            self->index = 0;
            self->snapDistance = bitsf(fbits(off) ^ 0x80000000u);
            self->snapStart = off;
            return;
        }
        // ease to the nearer item: forward / back a whole step past half a step, else back to 0
        const float half = self->halfStep;
        if (off > half) {
            self->snapDistance = self->step - off;
        } else if (off < bitsf(fbits(half) ^ 0x80000000u)) {
            self->snapDistance = (-0.0f - self->step) - off;
        } else if (!std::isnan(off) && !std::isnan(half)) {
            self->snapDistance = bitsf(fbits(off) ^ 0x80000000u);
        }
        self->snapStart = off;
    }
}
RW_FUNCTION(0x516D7C, "BaseEndlessrollView::update", BaseEndlessrollView::update);

float Self::degree2radian(Addr, float degree) { return float(double(degree) * PI / 180.0); }
RW_FUNCTION(0x5177A0, "BaseEndlessrollView::degree2radian", BaseEndlessrollView::degree2radian);

// Split a wheel angle into whole item steps (negated: the wheel turns against the list) and the angle left inside
// the step. A wrapping list (6+ items) first brings a negative angle back into one turn.
void Self::get_offset_move(Self *self, float angle, Addr stepOut, Addr offsetOut) {
    const float step = self->step;
    const int32_t steps = self->stepCount;
    if (step == 0.0f || steps == 0) {
        st32(stepOut, 0);
        st32(offsetOut, 0);
        return;
    }
    if (self->itemCount >= 6) {
        if (angle < 0.0f) {
            const float turn = float(steps) * step;
            do angle = angle + turn;
            while (angle < 0.0f);
        }
        const int32_t k = -idiv(f2i(angle), f2i(step));
        st32(stepOut, uint32_t(k));
        stf(offsetOut, angle + float(k) * self->step);
    } else {
        const int32_t k = -idiv(f2i(angle), f2i(step));
        st32(stepOut, uint32_t(k));
        stf(offsetOut, float(k) * self->step + angle);
    }
}
RW_FUNCTION(0x5177CC, "BaseEndlessrollView::get_offset_move", BaseEndlessrollView::get_offset_move);

// Lay the rows out for wheel angle `angle` (front row 4, then 5..7 and 3..1); with `refill` hide all rows and show
// / refill (changeRow) the ones that have an item (all of them on a wrapping list).
void Self::setSub(Self *self, uint32_t refill, float angle) {
    const Addr me = rw::guest_addr(self);
    Frame f;
    const uint32_t out = f.alloc(8);              // offset (float) at +0, item steps at +4
    vcall(me, V_GET_OFFSET_MOVE, {me, fbits(angle), out + 4, out});
    if (refill == 1) {
        for (int32_t k = 0; k < self->rowCount; k++) {
            uint32_t row = ld32(self->rows.begin + 4 * uint32_t(k));
            vcall(row, node::setVisible, {row, 0});
        }
    }
    const auto place = [&](int32_t i) {
        vcall(me, V_SET_DISP_PARAM, {me, uint32_t(i), ld32(out)});
        if (refill != 1) return;
        const int32_t item = self->index + int32_t(ld32(out + 4)) + i - 4;
        const int32_t n = self->itemCount;
        const uint32_t row = ld32(self->rows.begin + 4 * uint32_t(i));
        if (n > 5 || (item < n && item >= 0)) {
            int32_t m = item % n;
            if (m < 0) m += n;
            vcall(me, V_CHANGE_ROW, {me, row, uint32_t(m)});
            vcall(row, node::setVisible, {row, 1});
        } else {
            vcall(row, node::setVisible, {row, 0});
        }
    };
    for (int32_t i = 4; i != 8; i++) place(i);
    for (int32_t i = 3; i >= 1; i--) place(i);       // row 0 (and 8) is never placed
}
RW_FUNCTION(0x5178E8, "BaseEndlessrollView::setSub", BaseEndlessrollView::setSub);
