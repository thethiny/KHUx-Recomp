// Shared by the list views of batch #9 (ColosseumScrollView, RankingScrollView, RankingRewardScrollView): each is a
// BaseScrollView<int> whose copies of these functions differ only in field offsets and the lambdas' vtables.
// - the selected ids (a sorted std::vector<int>): setSelectItem toggles one, searchSelect looks one up and
//   updateSelectItems shows each row's "Panel_Eff" highlight;
// - the rows' own touch listener: began (remember the y, press inside the row), moved (leaving the row ends the
//   press), ended / cancelled (a tap that moved under 10 pixels plays the tap animation, then selects the item).
#pragma once

#include "../scene/SceneUi.hpp"

#include <cmath>

namespace rowtouch {
using namespace scene;

namespace g {
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t VectorInt_copy = 0x4AF707, VectorInt_dtor = 0x478961, VectorInt_pushSlow = 0x4D6CC1;
constexpr uint32_t VectorInt_assign = 0x47D5BD, SortInt = 0x4EC65D;
constexpr uint32_t FunctionVTE_swap = 0x4F3699;
constexpr uint32_t Rect_ctor = 0x8DF18B, Rect_containsPoint = 0x8DF2A9, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Node_runAction = 0x894AED, Node_convertToNodeSpace = 0x89599D, Touch_getLocation = 0x900E57;
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;
constexpr uint32_t aeabi_fixsfsi = 0xCD7240, aeabi_idiv = 0xCD5E8D;
}  // namespace g

constexpr uint32_t GOT_ROW_SPACING = 0xFD659C;            // &float added to the row height
// IScrollProtocol / BaseScrollView<int>
constexpr uint32_t F_ITEM_H = 0x518, F_ROWS = 0x51C, F_ROW_COUNT = 0x528, F_COLUMNS = 0x538, F_SCROLL_Y = 0x540;
constexpr uint32_t F_FIRST = 0x544, F_IDS = 0x628;
constexpr uint32_t V_setLocalZOrder = 0x0C, V_setVisible = 0xB0;
constexpr uint32_t S_PanelEff = 0xDFE94F;                 // "Panel_Eff"

// per view: where its fields are and which guest functions / lambdas it uses
struct View {
    uint32_t selected;          // std::vector<int> of selected ids
    uint32_t pressed;           // bool: a press inside a row is running
    uint32_t pressIndex;        // the pressed item (index into the ids)
    uint32_t tapped;            // bool: the tap animation runs
    uint32_t pressY;            // float: where the press began
    uint32_t vtEnded;           // __func vtable of the ended / cancelled lambda (heap, 0x14)
    uint32_t vtTapDone;         // __func vtable of the tap animation's done lambda
    uint32_t setSelectItem, updateSelectItems, touchEndedBody;   // guest addresses (Thumb bit set)
};

inline uint32_t idCount(uint32_t s) { return (ld32(s + F_IDS + 4) - ld32(s + F_IDS)) >> 2; }

// lower_bound over the sorted ids
inline uint32_t lowerBound(uint32_t it, uint32_t end, uint32_t id) {
    for (int32_t len = int32_t(end - it) >> 2; len;) {
        const int32_t half = len / 2;
        if (int32_t(ld32(it + 4 * half)) < int32_t(id)) {
            it += 4 * half + 4;
            len = len - 1 - half;
        } else {
            len = half;
        }
    }
    return it;
}

// add the id when missing (kept sorted), else rebuild the list without it; then the highlights. The original
// clears the list first (the end pointer back to the start; the vector keeps its buffer) and copies the view's ids
// into a local it never uses - so a second call only ever holds the one id.
inline void setSelectItem(const View &v, uint32_t s, uint32_t id) {
    Frame f;
    const uint32_t value = f.alloc(4);
    st32(value, id);
    const uint32_t vec = s + v.selected;
    if (ld32(vec + 4) != ld32(vec)) {
        const uint32_t end = ld32(vec + 4);
        st32(vec + 4, end + (~(end - 4 - ld32(vec)) & 0xFFFFFFFCu));
    }
    const uint32_t copy = f.alloc(12);
    call(g::VectorInt_copy, {copy, s + F_IDS});
    const uint32_t less = f.alloc(4);
    call(g::SortInt, {ld32(vec), ld32(vec + 4), less});
    const uint32_t begin = ld32(vec), end = ld32(vec + 4);
    const uint32_t it = lowerBound(begin, end, id);
    if (it == end || int32_t(ld32(it)) > int32_t(id)) {
        if (end == ld32(vec + 8)) {
            call(g::VectorInt_pushSlow, {vec, value});
        } else {
            if (end) st32(end, id);
            st32(vec + 4, (end ? ld32(vec + 4) : 0) + 4);
        }
        call(g::SortInt, {ld32(vec), ld32(vec + 4), less});
    } else {
        const uint32_t tmp = f.alloc(12);
        st32(tmp, 0);
        st32(tmp + 4, 0);
        st32(tmp + 8, 0);
        uint32_t p = begin;
        if (p != end) {
            do {
                if (ld32(p) != id) {
                    const uint32_t te = ld32(tmp + 4);
                    if (te == ld32(tmp + 8)) {
                        call(g::VectorInt_pushSlow, {tmp, p});
                    } else {
                        if (te) st32(te, ld32(p));
                        st32(tmp + 4, (te ? ld32(tmp + 4) : 0) + 4);
                    }
                }
                p += 4;
            } while (p != ld32(vec + 4));
            if (ld32(vec) != p) st32(vec + 4, p - ((p - ld32(vec) - 4) & ~3u) - 4);
        }
        call(g::VectorInt_assign, {vec, ld32(tmp), ld32(tmp + 4)});
        call(g::VectorInt_dtor, {tmp});
    }
    call(v.updateSelectItems, {s});
    call(g::VectorInt_dtor, {copy});
}

inline uint32_t searchSelect(const View &v, uint32_t s, uint32_t id) {
    const uint32_t end = ld32(s + v.selected + 4);
    const uint32_t it = lowerBound(ld32(s + v.selected), end, id);
    if (it == end) return 0;
    return int32_t(ld32(it)) <= int32_t(id) ? 1 : 0;
}

// each shown row's "Panel_Eff": on when the row's id is selected
inline void updateSelectItems(const View &v, uint32_t s) {
    if (ld32(s + F_IDS + 4) == ld32(s + F_IDS)) return;
    if (!(ldf(s + F_ROW_COUNT) > 0.0f)) return;
    int32_t k = 0;
    do {
        const uint32_t begin = ld32(s + v.selected), end = ld32(s + v.selected + 4);
        uint32_t on = 0;
        if (end != begin) {
            const uint32_t id = ld32(uint32_t(k + int32_t(ld32(s + F_FIRST))) * 4 + ld32(s + F_IDS) - 4);
            const uint32_t n = (end - begin) >> 2;
            uint32_t j = 0;
            do {
                const uint32_t x = ld32(begin + 4 * j++);
                on = 1;
                if (x == id) break;
                on = 0;
            } while (j < n);
        }
        const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * uint32_t(k));
        const uint32_t eff = findNode(row, S_PanelEff, 9);
        if (eff) vcall(eff, V_setVisible, {eff, on});
        k++;
    } while (float(k) < ldf(s + F_ROW_COUNT));
}

// function<void(Touch*, Event*)> at dst = the ended / cancelled lambda {this, index, this, index} (heap)
inline void assignEnded(const View &v, uint32_t dst, uint32_t caps) {
    Frame f;
    const uint32_t tmp = newEmptyFunction(f);
    const uint32_t obj = call(fn::operator_new, {0x14});
    if (obj) {
        st32(obj, v.vtEnded);
        for (uint32_t w = 0; w < 16; w += 4) st32(obj + 4 + w, ld32(caps + w));
    }
    st32(tmp + 16, obj);
    call(g::FunctionVTE_swap, {tmp, dst});
    functionDestroy(tmp);
}

// the end of a press {this, index, this, index}: the item under the press; a tap (moved under 10 pixels) raises
// the row and plays the tap animation, anything else ends the press
inline void touchEndedBody(const View &v, uint32_t caps, uint32_t touch) {
    const uint32_t s = ld32(caps);
    const float step = ldf(s + F_ITEM_H) + ldf(ld32(GOT_ROW_SPACING));
    const uint32_t scrolled = call(g::aeabi_fixsfsi, {ld32(s + F_SCROLL_Y)});
    const uint32_t rowStep = call(g::aeabi_fixsfsi, {fbits(step)});
    const uint32_t rows = call(g::aeabi_idiv, {scrolled, rowStep});
    const uint32_t item = ld32(caps + 4) + ld32(s + F_COLUMNS) * rows - 1;
    st32(s + v.pressIndex, item);
    if (!(idCount(s) > item) || !ld8(s + v.pressed)) {
        st8(s + v.pressed, 0);
        return;
    }
    Frame f;
    const uint32_t loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    const float pressY = ldf(s + v.pressY), y = ldf(loc + 4);
    call(g::Vec2_dtor, {loc});
    if (!(std::fabs(y - pressY) < 10.0f)) {
        st8(s + v.pressed, 0);
        return;
    }
    st8(s + v.tapped, 1);
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(caps + 4));
    vcall(row, V_setLocalZOrder, {row, 1});
    const uint32_t fo = newFunction(f, v.vtTapDone, {ld32(caps + 8), ld32(caps + 12)});
    call(g::Node_runAction, {row, call(g::SceneUtil_createTapAnimation, {row, fo})});
    functionDestroy(fo);
}

// the tap animation done {this, index}: the row back down, the pressed item selected
inline void tapDone(const View &v, uint32_t fo) {
    const uint32_t s = ld32(fo + 4);
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(fo + 8));
    vcall(row, V_setLocalZOrder, {row, 0});
    const uint32_t at = ld32(s + v.pressIndex);
    if (idCount(s) <= at) call(g::ThrowOutOfRange, {s + F_IDS});
    call(v.setSelectItem, {s, ld32(ld32(s + F_IDS) + 4 * at)});
    st8(s + v.tapped, 0);
    st8(s + v.pressed, 0);
}

inline bool insideRow(uint32_t s, uint32_t index, uint32_t touch) {
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * index);
    Frame f;
    const uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, row, loc});
    call(g::Vec2_dtor, {loc});
    const uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
    const uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
    call(g::Rect_ctor, {rect, 0, 0, w, h});
    const bool in = call(g::Rect_containsPoint, {rect, p}) != 0;
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {p});
    return in;
}

// moved {this, index, then the ended captures}: leaving the row ends the press
inline void touchMoved(const View &v, uint32_t fo, uint32_t touchRef) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t s = ld32(fo + 4);
    if (!ld8(s + v.pressed)) return;
    if (!insideRow(s, ld32(fo + 8), touch)) {
        st8(s + v.pressed, 0);
        call(v.touchEndedBody, {fo + 0xC, touch});
    }
}

// began {this, index}: remember the y; a touch inside the row starts a press
inline uint32_t touchBegan(const View &v, uint32_t fo, uint32_t touchRef) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t s = ld32(fo + 4);
    if (ld8(s + v.tapped)) return 0;
    {
        Frame f;
        const uint32_t loc = f.alloc(8);
        call(g::Touch_getLocation, {loc, touch});
        st32(s + v.pressY, ld32(loc + 4));
        call(g::Vec2_dtor, {loc});
    }
    if (insideRow(s, ld32(fo + 8), touch)) {
        st8(s + v.pressed, 1);
        return 1;
    }
    return 0;
}

}  // namespace rowtouch
