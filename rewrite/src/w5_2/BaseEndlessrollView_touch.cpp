// BaseEndlessrollView (the Avatar Board list wheel): moving the wheel (updateAddMove, the end stops of short lists
// in updateMoveMaxCheck) and the touch lambdas of the wheel (began / moved / ended) and of one row (began / ended).
// Field names follow BaseEndlessrollView.hpp; offsets are written out here (the lambdas get the view as a capture).
#include "../scene/SceneCommon.hpp"

#include <cmath>

using namespace scene;

namespace {
namespace a {
constexpr uint32_t Touch_getLocation = 0x900E57;          // (Vec2* out, Touch*)
constexpr uint32_t Vec2_ctor = 0x8E4B95, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Size_copy = 0x8DF02D, Size_assign = 0x8DF041;
constexpr uint32_t Rect_ctor = 0x8DF18B;                  // (out, x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;    // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t FunctionVF_call = 0x4F8E89;            // function<void(float)>::operator()
constexpr uint32_t FunctionVIW_call = 0x519061;           // function<void(int, ui::Widget*)>::operator()
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
}  // namespace a

// BaseEndlessrollView fields
constexpr uint32_t F_ON_TOUCH_BEGAN = 0x288, F_ON_SCROLL = 0x2A0, F_ON_RELEASE = 0x2B8, F_ON_TAP_ROW = 0x300;
constexpr uint32_t F_POS = 0x318, F_INDEX = 0x31C, F_LOCKED = 0x324, F_ITEM = 0x340, F_USE_PARENT = 0x344;
constexpr uint32_t F_ITEM_COUNT = 0x348, F_START_X = 0x364, F_TOUCHING = 0x368, F_DRAGGING = 0x369;
constexpr uint32_t F_ROW_PRESSABLE = 0x36A, F_ROW_PRESSED = 0x36B, F_FAST = 0x36C, F_LAST_X = 0x370;
constexpr uint32_t F_SPEED = 0x374, F_SLOW_MOVES = 0x378, F_DELTAS = 0x37C, F_DELTA_COUNT = 0x38C;
constexpr uint32_t F_RELEASED = 0x390, F_SNAPPING = 0x391, F_SNAP_TIME = 0x398, F_SNAP_DISTANCE = 0x39C;
constexpr uint32_t F_SNAP_START = 0x3A0, F_ROW_TOUCHED = 0x3A4, F_ROW_TOUCH_X = 0x3A8, F_DRAG = 0x3AC;
constexpr uint32_t F_STEP = 0x3B0;
// virtual slots
constexpr uint32_t V_GET_OFFSET_MOVE = 0x274, V_SET_SUB = 0x280, V_UPDATE_ADD_MOVE = 0x284;
constexpr uint32_t V_UPDATE_MOVE_MAX_CHECK = 0x288;


inline float fl(uint32_t a) { return ldf(a); }

// the touch's x
uint32_t touchX(uint32_t touch) {
    Frame f;
    const uint32_t loc = f.alloc(8);
    call(a::Touch_getLocation, {loc, touch});
    const uint32_t x = ld32(loc);
    call(a::Vec2_dtor, {loc});
    return x;
}
}  // namespace

struct BaseEndlessrollViewMove {
    static void updateAddMove(uint32_t self, uint32_t dt);
    static void updateMoveMaxCheck(uint32_t self, uint32_t dt, uint32_t touch);
    static uint32_t touchBegan(uint32_t lambda, uint32_t touchRef);
    static void touchMoved(uint32_t lambda, uint32_t touchRef);
    static void touchEnded(uint32_t lambda, uint32_t touchRef);
    static uint32_t rowTouchBegan(uint32_t lambda, uint32_t touchRef);
    static void rowTouchEnded(uint32_t lambda);
};

void BaseEndlessrollViewMove::updateAddMove(uint32_t self, uint32_t dt) {
    const float pos = fl(self + F_POS) + bitsf(dt);
    Frame f;
    const uint32_t item = f.alloc(4), offset = f.alloc(4);
    vcall(self, V_GET_OFFSET_MOVE, {self, fbits(pos), item, offset});
    const uint32_t old = ld32(self + F_ITEM);
    st32(self + F_ITEM, ld32(item));
    vcall(self, V_SET_SUB, {self, old != ld32(item) ? 1u : 0u, fbits(pos)});
}

// lists of fewer than six items stop at both ends: past an end the wheel is put back (when dragged) or its speed
// capped at 2 (when spinning)
void BaseEndlessrollViewMove::updateMoveMaxCheck(uint32_t self, uint32_t dt, uint32_t touch) {
    const float pos = fl(self + F_POS) + bitsf(dt);
    Frame f;
    const uint32_t itemOut = f.alloc(4), offset = f.alloc(4);
    vcall(self, V_GET_OFFSET_MOVE, {self, fbits(pos), itemOut, offset});
    const int32_t item = int32_t(ld32(itemOut));
    const int32_t at = item + int32_t(ld32(self + F_INDEX));
    float placed;
    if (at <= 0 && pos >= fl(self + F_STEP) * 0.5f) {
        // the first item, dragged past it
        const float step = fl(self + F_STEP), half = step * 0.5f;
        if (!touch) {
            if (int32_t(ld32(self + F_ITEM_COUNT)) <= 5 && fl(self + F_SPEED) > 2.0f) stf(self + F_SPEED, 2.0f);
            return;
        }
        if (int32_t(ld32(self + F_ITEM_COUNT)) > 5) return;
        int32_t k = item;
        if (at < 0) {
            st32(self + F_INDEX, 0);
            if (item < 0) {
                st32(itemOut, 0);
                k = 0;
            }
        }
        placed = half - step * float(k);
    } else {
        const int32_t n = int32_t(ld32(self + F_ITEM_COUNT));
        if (at < n - 1) return;
        const float step = fl(self + F_STEP), half = step * 0.5f;
        if (!(pos <= -0.0f - half)) return;
        // the last item, dragged past it
        if (!touch) {
            if (n <= 5 && fl(self + F_SPEED) < -2.0f) stf(self + F_SPEED, -2.0f);
            return;
        }
        if (n > 5) return;
        int32_t k = item;
        if (at > n - 1) {
            st32(self + F_INDEX, uint32_t(n - 1));
            if (item >= 1) {
                k = 0;
                st32(itemOut, 0);
            }
        }
        placed = (-0.0f - step * float(k)) - half;
    }
    stf(self + F_POS, placed);
    st32(self + F_START_X, touchX(touch));
    const uint32_t x = ld32(self + F_START_X);
    st32(self + F_LAST_X, x);
    Frame g;
    const uint32_t loc = g.alloc(8);
    call(a::Touch_getLocation, {loc, touch});
    stf(self + F_DRAG, (bitsf(x) - fl(loc)) * -0.125f);
    call(a::Vec2_dtor, {loc});
    st32(self + F_SPEED, 0);
    st32(self + F_SLOW_MOVES, ld32(self + F_SLOW_MOVES) + 1);
}

// ------------------------------------------------------------------------------------------------ wheel touches
// the lambdas capture the view at +4; the touch arrives by reference (Touch*&)
uint32_t BaseEndlessrollViewMove::touchBegan(uint32_t lambda, uint32_t touchRef) {
    const uint32_t v = ld32(lambda + 4);
    if (ld8(v + F_LOCKED) || ld8(v + F_FAST) || int32_t(ld32(v + F_ITEM_COUNT)) < 2) return 0;
    const uint32_t touch = ld32(touchRef);
    st8(v + F_SNAPPING, 0);
    st32(v + F_SNAP_TIME, 0);
    st32(v + F_SNAP_DISTANCE, 0);
    st32(v + F_SNAP_START, 0);
    st8(v + F_RELEASED, 0);
    st32(v + F_SPEED, 0);
    st8(v + F_DRAGGING, 0);
    st8(v + F_TOUCHING, 1);
    st32(v + F_START_X, touchX(touch));
    st32(v + F_LAST_X, ld32(v + F_START_X));
    st32(v + F_SNAP_TIME, 0);
    st8(v + F_SNAPPING, 0);
    if (ld32(v + F_ON_TOUCH_BEGAN + 0x10)) call(fn::function_vv_call, {v + F_ON_TOUCH_BEGAN});
    return 1;
}

void BaseEndlessrollViewMove::touchMoved(uint32_t lambda, uint32_t touchRef) {
    const uint32_t v = ld32(lambda + 4);
    if (ld8(v + F_FAST) || int32_t(ld32(v + F_ITEM_COUNT)) < 2) return;
    const uint32_t touch = ld32(touchRef);
    const uint32_t x = touchX(touch);
    const uint32_t lastX = ld32(v + F_LAST_X);
    st32(v + F_LAST_X, touchX(touch));
    // the last four moves (for the release speed)
    const int32_t n = int32_t(ld32(v + F_DELTA_COUNT)) + 1;
    st32(v + F_DELTA_COUNT, uint32_t(n));
    const float delta = bitsf(x) - bitsf(lastX);
    stf(v + F_DELTAS + 4 * uint32_t(n % 4), delta);
    if (std::fabs(delta) > 16.0f) {
        stf(v + F_SPEED, delta * 0.125f + fl(v + F_SPEED));
        st32(v + F_SLOW_MOVES, 0);
    } else {
        st32(v + F_SPEED, 0);
        st32(v + F_SLOW_MOVES, ld32(v + F_SLOW_MOVES) + 1);
    }
    if (!ld8(v + F_DRAGGING)) {
        const float startX = fl(v + F_START_X);
        if (std::fabs(startX - bitsf(touchX(touch))) >= 20.0f) {
            st8(v + F_DRAGGING, 1);
            st32(v + F_START_X, touchX(touch));
        }
    }
    const float startX = fl(v + F_START_X);
    stf(v + F_DRAG, (startX - bitsf(touchX(touch))) * -0.125f);
    vcall(v, V_UPDATE_MOVE_MAX_CHECK, {v, ld32(v + F_DRAG), touch});
    if (ld8(v + F_DRAGGING)) vcall(v, V_UPDATE_ADD_MOVE, {v, ld32(v + F_DRAG)});
    if (ld32(v + F_ON_SCROLL + 0x10)) call(a::FunctionVF_call, {v + F_ON_SCROLL, ld32(v + F_DRAG)});
}

// release: the last drag applied, the wheel keeps the average of the last four moves as its speed (unless it was
// already faster in that direction, or the finger rested)
void BaseEndlessrollViewMove::touchEnded(uint32_t lambda, uint32_t touchRef) {
    const uint32_t v = ld32(lambda + 4);
    if (ld8(v + F_FAST) || int32_t(ld32(v + F_ITEM_COUNT)) < 2) return;
    const uint32_t touch = ld32(touchRef);
    if (ld8(v + F_DRAGGING)) {
        const float startX = fl(v + F_START_X);
        const float d = (startX - bitsf(touchX(touch))) * -0.125f;
        stf(v + F_POS, fl(v + F_POS) + d);
        vcall(v, V_UPDATE_MOVE_MAX_CHECK, {v, 0, 0});
        vcall(v, V_UPDATE_ADD_MOVE, {v, 0});
    }
    st32(v + F_DRAG, 0);
    st8(v + F_RELEASED, 1);
    st8(v + F_TOUCHING, 0);
    st8(v + F_DRAGGING, 0);
    st8(v + F_FAST, 0);
    if (int32_t(ld32(v + F_SLOW_MOVES)) >= 2) {
        st32(v + F_SPEED, 0);
    } else {
        const float avg = (((fl(v + F_DELTAS) + fl(v + F_DELTAS + 4)) + fl(v + F_DELTAS + 8)) + fl(v + F_DELTAS + 12)) *
                          0.03125f;
        const float speed = fl(v + F_SPEED);
        const bool keep = (avg <= 0.0f || std::isnan(avg)) ? avg <= speed : avg >= speed;
        if (!keep && !std::isnan(avg) && !std::isnan(speed)) stf(v + F_SPEED, avg);
    }
    if (ld32(v + F_ON_RELEASE + 0x10)) call(fn::function_vv_call, {v + F_ON_RELEASE});
}

// ------------------------------------------------------------------------------------------------ row touches
// captures: the view (+4), the row widget (+8), the row's parent (+0xC: used when the view's flag at +0x344 is set)
uint32_t BaseEndlessrollViewMove::rowTouchBegan(uint32_t lambda, uint32_t touchRef) {
    const uint32_t v = ld32(lambda + 4);
    if (ld8(v + F_LOCKED)) return 0;
    const uint32_t touch = ld32(touchRef);
    const uint32_t row = ld32(lambda + 8);
    if (vcall(row, node::isVisible, {row}) != 1) return 0;
    if (ld8(v + F_ROW_PRESSED) || ld8(v + F_FAST)) return 0;
    const uint32_t released = ld8(v + F_RELEASED);
    st8(v + F_ROW_PRESSABLE, uint8_t(released ^ 1));
    if (released == 1) return 0;
    Frame f;
    const uint32_t loc = f.alloc(8), size = f.alloc(8), local = f.alloc(8), rect = f.alloc(16), tmp = f.alloc(8);
    call(a::Touch_getLocation, {loc, touch});
    call(a::Size_copy, {size, vcall(ld32(lambda + 8), node::getContentSize, {ld32(lambda + 8)})});
    call(a::Vec2_ctor, {local});
    if (ld8(v + F_USE_PARENT))
        call(a::Size_assign, {size, vcall(ld32(lambda + 0xC), node::getContentSize, {ld32(lambda + 0xC)})});
    rw::call(a::Rect_ctor, {rect, 0, 0, ld32(size), ld32(size + 4)});
    call(a::Node_convertToNodeSpace, {tmp, ld32(lambda + (ld8(v + F_USE_PARENT) ? 0xC : 8)), loc});
    st32(local, ld32(tmp));
    st32(local + 4, ld32(tmp + 4));
    call(a::Vec2_dtor, {tmp});
    uint32_t touched = 0;
    if (call(a::Rect_containsPoint, {rect, local}) == 1) {
        st32(v + F_ROW_TOUCH_X, touchX(touch));
        st8(v + F_ROW_TOUCHED, 1);
        touched = 1;
    }
    call(a::Vec2_dtor, {rect});
    call(a::Vec2_dtor, {local});
    call(a::Vec2_dtor, {loc});
    return touched;
}

// a tap on a row (not a drag): its "New" badge goes away and the view's row callback gets (item + 1, row)
void BaseEndlessrollViewMove::rowTouchEnded(uint32_t lambda) {
    const uint32_t v = ld32(lambda + 4);
    const uint32_t released = ld8(v + F_RELEASED);
    st8(v + F_ROW_PRESSABLE, uint8_t(released ^ 1));
    if (released == 1 || !ld8(v + F_ROW_TOUCHED) || !ld8(v + F_ROW_PRESSED)) return;
    st8(v + F_ROW_PRESSED, 0);
    if (!ld32(v + F_ON_TAP_ROW + 0x10) || int32_t(ld32(v + F_ITEM_COUNT)) < 1) return;
    Frame f;
    const uint32_t s = f.alloc(12);              // a short std::string "New" (bytes past the terminator unset)
    st8(s, 6);
    st8(s + 1, 'N');
    st8(s + 2, 'e');
    st8(s + 3, 'w');
    st8(s + 4, 0);
    const uint32_t badge = call(a::SceneUtil_searchNodeByName, {ld32(lambda + 8), s});
    if (badge) vcall(badge, node::setVisible, {badge, 0});
    call(a::FunctionVIW_call, {v + F_ON_TAP_ROW, ld32(v + F_ITEM) + 1, ld32(lambda + 8)});
}

RW_FUNCTION(0x517AD8, "BaseEndlessrollView::updateAddMove", BaseEndlessrollViewMove::updateAddMove);
RW_FUNCTION(0x517B64, "BaseEndlessrollView::updateMoveMaxCheck", BaseEndlessrollViewMove::updateMoveMaxCheck);
RW_FUNCTION(0x5186B4, "BaseEndlessrollView touch began lambda (sub_5186B4)", BaseEndlessrollViewMove::touchBegan);
RW_FUNCTION(0x5183F4, "BaseEndlessrollView touch moved lambda (sub_5183F4)", BaseEndlessrollViewMove::touchMoved);
RW_FUNCTION(0x5181C4, "BaseEndlessrollView touch ended lambda (sub_5181C4)", BaseEndlessrollViewMove::touchEnded);
RW_FUNCTION(0x518B64, "BaseEndlessrollView row touch began lambda (sub_518B64)", BaseEndlessrollViewMove::rowTouchBegan);
RW_FUNCTION(0x518F20, "BaseEndlessrollView row touch ended lambda (sub_518F20)", BaseEndlessrollViewMove::rowTouchEnded);
