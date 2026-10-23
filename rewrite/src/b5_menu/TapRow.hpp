// The touch-ended body that several BaseScrollView lists share (HelpScrollView sub_51C314, PresentBoxScrollView
// sub_50F7F4): captures {view, row, view, row}. The pressed item is the row plus the first visible item (scroll
// position / item height, in columns); a release within 10 pixels of where the touch went down on an item with a
// non-zero id plays the row's tap animation (whose end callback, {view, row}, acts on the item); anything else
// ends the press.
#pragma once

#include "../w3_9/BaseScrollView.hpp"

#include <cmath>

namespace tap_row {
struct Layout {
    uint32_t pressedItem;   // int: the pressed item
    uint32_t pressed;       // byte: a row is pressed
    uint32_t tapped;        // byte: the tap animation runs
    uint32_t touchY;        // float: where the touch went down
    uint32_t idSize;        // 4 (BaseScrollView<int>) or 8 (<unsigned long long>)
    uint32_t vtTapDone;     // the tap animation's end lambda (__func vtable + 8)
};

inline void touchEnded(const Layout &l, uint32_t captures, uint32_t touch) {
    using namespace scene;
    constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1, DIVSI3 = 0xCD5E8D;
    constexpr uint32_t Touch_getLocation = 0x900E57, Vec2_dtor = 0x8E4C17;
    constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9, Node_runAction = 0x894AED;
    constexpr uint32_t V_retain = 0x0C;

    uint32_t me = ld32(captures);
    auto *self = rw::host_ptr<BaseScrollView>(me);
    auto count = [&] { return (self->itemIDs.end - self->itemIDs.begin) / l.idSize; };
    float item = self->itemHeight + 0.0f;
    int32_t first = int32_t(call(DIVSI3, {uint32_t(f2i(self->position)), uint32_t(f2i(item))}));
    uint32_t pressed = ld32(captures + 4) + uint32_t(self->maxColumn * first) - 1;
    st32(me + l.pressedItem, pressed);
    bool tap = false;
    if (pressed < count() && ld8(me + l.pressed)) {
        Frame f;
        uint32_t loc = f.alloc(8);
        call(Touch_getLocation, {loc, touch});
        if (std::fabs(ldf(loc + 4) - ldf(me + l.touchY)) < 10.0f) {
            uint32_t at = ld32(me + l.pressedItem);
            if (count() <= at) call(THROW_OUT_OF_RANGE, {});
            uint32_t id = self->itemIDs.begin + l.idSize * at;
            bool nonZero = l.idSize == 8 ? (ld32(id) | ld32(id + 4)) != 0 : ld32(id) != 0;
            call(Vec2_dtor, {loc});
            tap = nonZero;
        } else {
            call(Vec2_dtor, {loc});
        }
    }
    if (!tap) {
        st8(me + l.pressed, 0);
        return;
    }
    st8(me + l.tapped, 1);
    uint32_t widget = ld32(self->items.begin + 4 * ld32(captures + 4));
    vcall(widget, V_retain, {widget});
    Frame f;
    uint32_t fo = newFunction(f, l.vtTapDone, {ld32(captures + 8), ld32(captures + 12)});
    uint32_t anim = call(SceneUtil_createTapAnimation, {widget, fo});
    call(Node_runAction, {widget, anim});
    functionDestroy(fo);
}

// a std::function holding a heap-stored lambda: vtable and its captured words (the callable is the heap object)
inline uint32_t newHeapFunction(scene::Frame &f, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    using namespace scene;
    uint32_t fo = f.alloc(20);
    uint32_t obj = call(fn::operator_new, {uint32_t(4 + 4 * captures.size())});
    if (obj) {
        st32(obj, vtable);
        uint32_t k = 4;
        for (uint32_t c : captures) { st32(obj + k, c); k += 4; }
    }
    st32(fo + 16, obj);
    return fo;
}

// the touch-ended / cancelled slot setter (HelpScrollView sub_51C1C0, PresentBoxScrollView sub_50DE48): the slot
// gets the {view, row, view, row} lambda
inline void assignTouchEnded(uint32_t vtable, uint32_t slot, uint32_t captures) {
    using namespace scene;
    constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;
    Frame f;
    uint32_t fo = newHeapFunction(f, vtable, {ld32(captures), ld32(captures + 4), ld32(captures + 8), ld32(captures + 12)});
    call(FunctionTouchVoid_swap, {fo, slot});
    functionDestroy(fo);
}
}  // namespace tap_row
