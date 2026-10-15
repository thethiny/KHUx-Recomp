// SceneLight::cmd_move: the show / hide / slide / jump actions of a slot sprite. Each action, when the command
// waits for input (wait < 0), ends with a lambda {this} that lets the sequence go on ($_11 .. $_21).
// The original's outlined tails (sub_6C427E .. sub_6C4918) share this function's frame and return through its
// epilogue; they are written inline here.
#include "SceneLight_lib.hpp"

using namespace slight;

namespace {
float moveX(uint32_t k) { return float(rw::load<int16_t>(T_MOVE_X + k * 2)); }

// run `action` on `node`: alone, or followed by lambda `vtable` {this} when the command waits for input
void runTail(uint32_t self, uint32_t node, uint32_t action, uint32_t vtable) {
    if (waitNotNegative(self)) runAlone(node, action);
    else runWithDone(node, action, vtable, self);
}

uint32_t moveTo(float ms, float x, float y) {
    return withVec2(x, y, [&](uint32_t v) { return call(wl::MoveTo_create, {fb(ms), v}); });
}
}  // namespace

// move: 1001..1005 show at position 1..5, 1009 / 1029 fade in, 1011..1016 / 1020..1025 open the speech window
// (type 0..5), 1019 close it, 1031..1035 slide in from the right, 1041..1045 from the left, 1050 / 1051 slide out
// right / left, 1060 wait, 3001 / 3002 jump forward / back
void SceneLight::cmdMove(Addr self, int32_t slot, int32_t move, int32_t ms) {
    const uint32_t sp = sprite(self, slot);
    Frame f;
    const Size2 vs = visibleSize(f);
    const uint32_t size = f.alloc(8);
    call(Size_copy, {size, vcall(sp, VS_getContentSize, {sp})});
    Self *s = S(self);
    // y of a picture standing on the bottom of the 640-high area (+ 160)
    auto standY = [&]() { return ldf(size + 4) * 0.5f + ((vs.h * 0.5f + -320.0f) + 160.0f); };

    if (move > 3000) {
        if (uint32_t(move - 3001) >= 2) return;
        // a jump of 96 px forward (3001) / back (3002), toward the side the picture faces
        Frame g;
        uint32_t px = g.alloc(4), py = g.alloc(4);
        vcall(sp, VS_getPositionXY, {sp, px, py});
        uint32_t back = move == 3001 ? 0 : 1;
        uint32_t flipped = ld8(flipAddr(self, slot)) ? 1 : 0;
        float x = ldf(px) + ((back ^ flipped) ? 96.0f : -96.0f);
        float y = ldf(size + 4) * 0.5f + (((vs.h * 0.5f + -320.0f) + 160.0f));
        uint32_t jump = withVec2(x, y, [&](uint32_t v) {
            return call(JumpTo_create, {fb(msToSec(ms)), v, fb(40.0f), 1});
        });
        uint32_t act = call(EaseSineOut_create, {jump});
        runTail(self, sp, act, VT_move_15);
        return;
    }
    const uint32_t k = uint32_t(move - 1001);
    if (k > 0x3B) return;

    if (k <= 4) {
        // 1001..1005: show the picture at position k
        setVisible(sp, 1);
        uint32_t act;
        if (slot == SLOT_BG) {
            // the background fades in under the black sprite fading out
            float x = vs.w * 0.5f + moveX(k);
            float y = ldf(size + 4) * 0.5f + ((((vs.h * 0.5f + -320.0f) + 160.0f) + 40.0f));
            vcall(sp, VS_setPositionXY, {sp, fb(x), fb(y)});
            setOpacity(sp, 0xFF);
            float dur = msToSec(ms);
            act = call(FadeTo_create, {fb(dur), 0xFF});
            uint32_t black = sprite(self, SLOT_BLACK);
            setVisible(black, 1);
            setOpacity(black, 0xFF);
            uint32_t out = call(FadeTo_create, {fb(dur), 0});
            runWithDone(black, out, VT_move_16, self);
        } else {
            float x = vs.w * 0.5f + moveX(k);
            float base = vs.h * 0.5f + -320.0f;
            if (slot == SLOT_PICTURE) {
                float y = base + ldf(size + 4) * 0.5f;
                vcall(sp, VS_setPositionXY, {sp, fb(x), fb(y)});
                setOpacity(sp, 0xFF);
                vcall(sp, VS_scaleX, {sp, fb(71.0f)});
                act = call(FadeTo_create, {fb(msToSec(ms)), 0xFF});
            } else {
                float y = (base + 160.0f) + ldf(size + 4) * 0.5f;
                vcall(sp, VS_setPositionXY, {sp, fb(x), fb(y)});
                setOpacity(sp, 0);
                act = call(FadeTo_create, {fb(0.25f), 0xFF});
                if (s->avatarIcon && s->avatarSlot == slot) {
                    setOpacity(sp, 0xFF);
                    call(wl::Node_runAction, {s->avatarIcon, act});
                }
            }
        }
        runTail(self, sp, act, VT_move_17);
        return;
    }
    if (k == 8 || k == 28) {
        // 1009 / 1029 (also hiding the window tail and the cursor): fade in
        if (k == 28) {
            setVisible(sprite(self, SLOT_HIGE), 0);
            setVisible(sprite(self, SLOT_CURSOR), 0);
        }
        setVisible(sp, 1);
        uint32_t act;
        if (slot == SLOT_PICTURE) {
            act = call(FadeTo_create, {fb(msToSec(ms)), 0xFF});
        } else if (slot == SLOT_BG) {
            float dur = msToSec(ms);
            act = call(FadeTo_create, {fb(dur), 0xFF});
            uint32_t black = sprite(self, SLOT_BLACK);
            setOpacity(black, 0);
            setVisible(black, 1);
            runAlone(black, call(FadeTo_create, {fb(dur), 0xFF}));
        } else {
            act = call(FadeTo_create, {fb(0.25f), 0xFF});
            if (s->avatarIcon && s->avatarSlot == slot) call(wl::Node_runAction, {s->avatarIcon, act});
        }
        runTail(self, sp, act, VT_move_18);
        return;
    }
    if ((k >= 10 && k <= 15) || (k >= 19 && k <= 24)) {
        // 1011..1016: the speech window of type 0..5 pops up (scale), 1020..1025: fades in (with a sound)
        const bool pop = k <= 15;
        if (pop) s->textShown = 0;
        else call(AudioPlaySystemSEPopUp, {});
        setVisible(sprite(self, SLOT_CURSOR), 0);
        int32_t type = pop ? move - 1011 : move - 1020;
        s->windowType = type;
        bindTexture(self, SLOT_WINDOW, type ? 99999 : 99996, 0);
        call(Size_assign, {size, vcall(sp, VS_getContentSize, {sp})});
        setHige(self);
        vcall(sp, VS_setPositionX, {sp, fb(vs.w * 0.5f)});
        float y = vs.h * 0.5f;
        if (s->windowType) y = ((y + -320.0f) + ldf(size + 4) * 0.5f) + 18.0f;
        vcall(sp, VS_setPositionY, {sp, fb(y)});
        uint32_t act;
        if (pop) {
            vcall(sp, VS_setScale, {sp, 0});
            setOpacity(sp, 0xFF);
            setVisible(sp, 1);
            act = call(EaseBackOut_create, {call(ScaleTo_create, {fb(msToSec(ms)), fb(1.0f)})});
        } else {
            setVisible(sp, 1);
            setOpacity(sp, 0);
            vcall(sp, VS_setScale, {sp, fb(1.0f)});
            act = call(FadeTo_create, {fb(msToSec(ms)), 0xFF});
        }
        runTail(self, sp, act, pop ? VT_move_19 : VT_move_21);
        return;
    }
    if (k == 18) {
        // 1019: the window closes (shrinks and fades), the text goes
        setVisible(sprite(self, SLOT_HIGE), 0);
        setVisible(sprite(self, SLOT_CURSOR), 0);
        removeText(self);
        float dur = msToSec(ms);
        uint32_t shrink = call(EaseBackIn_create, {call(ScaleTo_create, {fb(dur), 0})});
        uint32_t fade = call(EaseExponentialIn_create, {call(FadeTo_create, {fb(dur), 0})});
        uint32_t act = call(Spawn_createWithTwoActions, {shrink, fade});
        runTail(self, sp, act, VT_move_20);
        return;
    }
    if ((k >= 30 && k <= 34) || (k >= 40 && k <= 44)) {
        // 1031..1035 / 1041..1045: slide in from the right / left to position 1..5
        const bool right = k <= 34;
        float startX = right ? vs.w * 1.5f : vs.w * 0.5f;
        float y0 = ldf(size + 4) * 0.5f + ((vs.h * 0.5f + -320.0f) + 160.0f);
        if (!right) startX = -startX;
        vcall(sp, VS_setPositionXY, {sp, fb(startX), fb(y0)});
        setVisible(sp, 1);
        float x = vs.w * 0.5f + moveX(right ? k - 30 : k - 40);
        float y = standY();
        uint32_t act = call(EaseOut_create, {moveTo(msToSec(ms), x, y), fb(10.0f)});
        runTail(self, sp, act, right ? VT_move_12 : VT_move_11);
        return;
    }
    if (k == 49 || k == 50) {
        // 1050 / 1051: slide out to the right / left
        float x = k == 49 ? vs.w * 1.5f : -(vs.w * 0.5f);
        float y = standY();
        uint32_t act = moveTo(msToSec(ms), x, y);
        setVisible(sp, 1);
        runTail(self, sp, act, k == 49 ? VT_move_14 : VT_move_13);
        return;
    }
    if (k == 59) {
        // 1060: wait
        s->wait = msToSec(ms);
        return;
    }
}

RW_FUNCTION(0x6C3764, "SceneLight::cmd_move", SceneLight::cmdMove);
