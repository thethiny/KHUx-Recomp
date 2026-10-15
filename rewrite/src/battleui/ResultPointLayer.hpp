// ResultPointLayer: a counting label of the battle result screens (Lux, munny, points...). Not a node: a
// 56-byte value embedded in BattleResultDialog (five of them). set() gives it a label, a start value, an amount to
// add and a duration; update() advances the time and redraws the label with the interpolated value, and once the
// time is over (or skipUpdate) the label shows the final value and the onDone callback runs.
#pragma once

#include "BattleUi.hpp"

struct ResultPointLayer {
    rw::Field<uint32_t> label;              // +0  cocos2d::ui::Text* (may be null)
    rw::Field<uint8_t> running;             // +4  counting started (set by the owner)
    rw::Field<uint8_t> finished;            // +5  reached the end
    const uint8_t pad_06[2];                      // +6
    rw::Field<uint32_t> baseLo;             // +8  value shown at the start (uint64)
    rw::Field<uint32_t> baseHi;             // +12
    rw::Field<uint32_t> addLo;              // +16 amount counted up (uint64)
    rw::Field<uint32_t> addHi;              // +20
    rw::Field<float> elapsed;               // +24 seconds counted so far
    rw::Field<float> duration;              // +28 seconds the count takes
    const uint8_t onDone[16];                     // +32 std::function<void()> run when the count ends
    rw::Field<uint32_t> onDone_f;           // +48 its callable pointer
    const uint8_t pad_34[4];                      // +52

    static constexpr uint32_t SIZE = 56;

    // advance by dt seconds (when running and not finished)
    static void update(ResultPointLayer *self, float dt, uint32_t withComma);
    // label, value to add (the base is reset to 0, then base += add), duration, callback
    static void set(ResultPointLayer *self, uint32_t label, uint64_t add, float duration, uint32_t onDone);
    // redraw the label with base + add * elapsed / duration
    static void updateLabel(ResultPointLayer *self, uint32_t withComma);
    // jump to the end: final value, finished, onDone()
    static void skipUpdate(ResultPointLayer *self, uint32_t withComma);
};
RW_LAYOUT(ResultPointLayer, baseLo, 8);
RW_LAYOUT(ResultPointLayer, elapsed, 24);
RW_LAYOUT(ResultPointLayer, onDone_f, 48);
RW_SIZE(ResultPointLayer, 56);
