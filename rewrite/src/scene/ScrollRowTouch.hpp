// The row touch lambdas the scroll views (IScrollProtocol + BaseScrollView<T>) register per row widget: touch began
// inside the row marks it and keeps the touch's y; moving out of the row, or the touch ending near where it began,
// plays the row's tap animation when the row's id is set. The views differ only in the offsets of their touch
// state and in the id width (RowTouch).
#pragma once

#include "SceneUi.hpp"

namespace scene {

// a view's row touch state
struct RowTouch {
    uint32_t inside;      // uint8: the touch began inside a row
    uint32_t index;       // int: the touched item index
    uint32_t running;     // uint8: a tap animation runs
    uint32_t touchY;      // float: the touch's y when it began
    uint32_t tapDoneVt;   // the tap animation's end lambda {2 captured words}
    uint32_t idBytes;     // sizeof(T) of BaseScrollView<T>::itemIDs (4 or 8)
};

namespace rowtouch {
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;   // (node, function<void()>)
constexpr uint32_t IScrollProtocol_containsPointInScrollArea = 0x4F7D21;   // (view, const Vec2&)
constexpr uint32_t VectorAt_throw = 0x47CCE1;
constexpr uint32_t GOT_ROW_SPACING = 0xFD659C;   // &IScrollProtocol::ITEM_MARGIN (float)
constexpr uint32_t F_ROWS = 0x51C, F_ROW_HEIGHT = 0x518, F_COLUMNS = 0x538, F_OFFSET = 0x540, F_IDS = 0x628;
constexpr uint32_t V_SET_LOCAL_Z = 0x0C;

inline uint32_t row(uint32_t view, uint32_t i) { return ld32(ld32(view + F_ROWS) + 4 * i); }
inline uint32_t idCount(uint32_t view, const RowTouch &t) {
    return (ld32(view + F_IDS + 4) - ld32(view + F_IDS)) / t.idBytes;
}
}  // namespace rowtouch

// is the touch inside the row's content rect (node space)
inline bool rowContainsTouch(uint32_t row, uint32_t touch) {
    using namespace rowtouch;
    Frame f;
    const uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(Touch_getLocation, {loc, touch});
    call(Node_convertToNodeSpace, {p, row, loc});
    call(Vec2_dtor, {loc});
    const uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
    const uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
    call(Rect_ctor, {rect, 0, 0, w, h});
    const bool in = call(Rect_containsPoint, {rect, p}) != 0;
    call(Vec2_dtor, {rect});
    call(Vec2_dtor, {p});
    return in;
}

// the tap {view, row index, 2 words for the end lambda}(touch): the touched index from the row and the scroll
// position; an id there and the touch still near its start: the row's tap animation
inline uint32_t rowTap(const RowTouch &t, uint32_t captures, uint32_t touch) {
    using namespace rowtouch;
    const uint32_t view = ld32(captures);
    const float rowStep = ldf(view + F_ROW_HEIGHT) + ldf(ld32(GOT_ROW_SPACING));
    const int32_t rows = f2i(ldf(view + F_OFFSET)) / f2i(rowStep);
    const uint32_t index = ld32(captures + 4) + ld32(view + F_COLUMNS) * uint32_t(rows) - 1;
    st32(view + t.index, index);
    if (idCount(view, t) > index && ld8(view + t.inside)) {
        Frame f;
        const uint32_t loc = f.alloc(8);
        call(Touch_getLocation, {loc, touch});
        if (std::fabs(ldf(loc + 4) - ldf(view + t.touchY)) < 10.0f) {
            const uint32_t at = ld32(view + t.index);
            if (idCount(view, t) <= at) call(VectorAt_throw, {view + F_IDS});
            const uint32_t entry = ld32(view + F_IDS) + t.idBytes * at;
            uint32_t id = ld32(entry);
            if (t.idBytes == 8) id |= ld32(entry + 4);
            call(Vec2_dtor, {loc});
            if (id) {
                st8(view + t.running, 1);
                const uint32_t r = row(view, ld32(captures + 4));
                vcall(r, V_SET_LOCAL_Z, {r, 1});
                Frame h;
                const uint32_t done = newFunction(h, t.tapDoneVt, {ld32(captures + 8), ld32(captures + 0xC)});
                const uint32_t action = call(SceneUtil_createTapAnimation, {r, done});
                call(fn::Node_runAction, {r, action});
                functionDestroy(done);
                return 0;
            }
        } else {
            call(Vec2_dtor, {loc});
        }
    }
    st8(view + t.inside, 0);
    return 0;
}

// touch moved {-, view, row index, tap captures (+0xC)}(touch*): leaving the row ends the tap (the original's
// tap function `tapFn`, called by address as the original does)
inline uint32_t rowTouchMoved(const RowTouch &t, uint32_t self, uint32_t touchp, uint32_t tapFn) {
    using namespace rowtouch;
    const uint32_t touch = ld32(touchp);
    const uint32_t view = ld32(self + 4);
    if (!ld8(view + t.inside)) return 0;
    const uint32_t r = row(view, ld32(self + 8));
    Frame f;
    const uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(Touch_getLocation, {loc, touch});
    call(Node_convertToNodeSpace, {p, r, loc});
    call(Vec2_dtor, {loc});
    const uint32_t w = ld32(vcall(r, node::getContentSize, {r}));
    const uint32_t h = ld32(vcall(r, node::getContentSize, {r}) + 4);
    call(Rect_ctor, {rect, 0, 0, w, h});
    if (!call(Rect_containsPoint, {rect, p})) {
        st8(view + t.inside, 0);
        call(tapFn, {self + 0xC, touch});
    }
    call(Vec2_dtor, {rect});
    call(Vec2_dtor, {p});
    return 0;
}

// touch began {-, view, row index}(touch*): not while a tap runs (and, when `scrollArea`, only inside the view's
// scroll area); the touch's y kept; inside the row: marked, 1
inline uint32_t rowTouchBegan(const RowTouch &t, uint32_t self, uint32_t touchp, bool scrollArea) {
    using namespace rowtouch;
    const uint32_t view = ld32(self + 4);
    if (ld8(view + t.running)) return 0;
    const uint32_t touch = ld32(touchp);
    if (scrollArea) {
        Frame f;
        const uint32_t loc = f.alloc(8);
        call(Touch_getLocation, {loc, touch});
        const uint32_t in = call(IScrollProtocol_containsPointInScrollArea, {view, loc});
        call(Vec2_dtor, {loc});
        if (!in) return 0;
    }
    {
        Frame f;
        const uint32_t loc = f.alloc(8);
        call(Touch_getLocation, {loc, touch});
        st32(view + t.touchY, ld32(loc + 4));
        call(Vec2_dtor, {loc});
    }
    if (!rowContainsTouch(row(view, ld32(self + 8)), touch)) return 0;
    st8(view + t.inside, 1);
    return 1;
}

}  // namespace scene
