// The filter popups' shared steps (SceneUtil::openAvatarBoardFilterPopup / openAdventureSelectFilterPopup): their
// state is a row of bool flags, +0 "the filter is on"; a check box flips one flag, "on" is recomputed over the
// popup's flags (in the popup's own order) and Release is lit to match; Release clears every flag; the result
// lambda turns the controls off, saves or puts the snapshot back, and closes the popup.
#pragma once

#include "SceneUi.hpp"

#include <initializer_list>

namespace scene {
namespace filter {

using Flags = std::initializer_list<uint32_t>;
constexpr uint32_t ON = 0;
constexpr uint32_t TOUCH_ENDED = 2, V_SET_TOUCH_ENABLED = 0x29C;
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright;
constexpr uint32_t CheckBox_setSelectedState = rwlib::cocos2d::ui::CheckBox::setSelectedState;
constexpr uint32_t SceneUtil_setTextLook = 0x4744F5;   // sub_4744F4(text, on, 0): the label's lit / greyed look
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t AppState_save = 0x4E8DC9;

// whether any of `flags` is set (tested in that order)
inline bool any(uint32_t state, Flags flags) {
    for (uint32_t i : flags)
        if (ld8(state + i)) return true;
    return false;
}
// Release lit and touchable (or not), its label to match
inline void lightRelease(uint32_t on, uint32_t release, uint32_t text) {
    call(Widget_setBright, {release, on});
    vcall(release, V_SET_TOUCH_ENABLED, {release, on});
    call(SceneUtil_setTextLook, {text, on, 0});
}
// "on" (in `onState`) recomputed from `state`'s flags, Release lit to match
inline void update(uint32_t onState, uint32_t state, Flags flags, uint32_t release, uint32_t text) {
    st8(onState + ON, any(state, flags));
    lightRelease(ld8(onState + ON) ? 1 : 0, release, text);
}
inline void select(uint32_t box, uint32_t state, uint32_t i) {
    call(CheckBox_setSelectedState, {box, uint32_t(ld8(state + i) != 0)});
}
// a check box {state, -, state, -, release, text}(sender, type): flag `i` flipped, "on" and Release updated
inline void boxTouched(uint32_t f, uint32_t typeRef, uint32_t i, Flags flags) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    call(fn::AudioPlaySystemSEDecide, {});
    const uint32_t state = ld32(f + 4);
    st8(state + i, !ld8(state + i));
    update(ld32(f + 4), ld32(f + 0xC), flags, ld32(f + 0x14), ld32(f + 0x18));
}
// Release {state, -, release, text, box, state, -, boxes...}(sender, type): with the filter on, the flags cleared
// (in `cleared` order), Release dimmed, each box (`boxes`: the first at +0x14, the rest from +0x20) unticked
inline void releaseTouched(uint32_t f, uint32_t typeRef, Flags cleared, Flags boxes) {
    if (ld32(typeRef) != TOUCH_ENDED || !ld8(ld32(f + 4) + ON)) return;
    call(fn::AudioPlaySystemSEDecide, {});
    for (uint32_t i : cleared) st8(ld32(f + 4) + i, 0);
    lightRelease(ld8(ld32(f + 4) + ON) != 0, ld32(f + 0xC), ld32(f + 0x10));
    uint32_t box = f + 0x14;
    for (uint32_t i : boxes) {
        select(ld32(box), ld32(f + 0x18), i);
        box = box == f + 0x14 ? f + 0x20 : box + 4;
    }
}
// the result lambda's start: its controls (`n` from +8) turned off
inline void controlsOff(uint32_t f, uint32_t n) {
    for (uint32_t k = 0; k < n; k++) vcall(ld32(f + 8 + 4 * k), V_SET_TOUCH_ENABLED, {ld32(f + 8 + 4 * k), 0});
}
// Close: the cancel sound and the snapshot at `saved` put back (byte k into flag `flags[k]`)
inline void restore(uint32_t state, uint32_t saved, Flags flags) {
    call(fn::AudioPlaySystemSECancel, {});
    uint32_t k = 0;
    for (uint32_t i : flags) st8(ld32(state) + i, ld8(saved + k++));
}
// the popup closing: the shadow fades, then the popup shrinks (its end `endVt` {layer, shadow, popup} removes them)
inline void close(uint32_t shadow, uint32_t popup, uint32_t layer, uint32_t endVt) {
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.15f), 0})});
    Frame fr;
    const uint32_t end = newFunction(fr, endVt, {layer, shadow, popup});
    call(fn::Node_runAction, {popup, call(SceneUtil_createClosePopupAnimation, {popup, end})});
    functionDestroy(end);
}

}  // namespace filter
}  // namespace scene
