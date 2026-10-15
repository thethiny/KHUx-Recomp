// BaseStageScrollView::update: queued row updates and widget replacements, the bounce animation and coasting.
#include "BaseStageScrollView_common.hpp"

using namespace bssv;

namespace {

// Queued row updates (updateLayout(true)): only rows still showing the same first item are refreshed.
void applyQueuedRows(Self *self) {
    uint32_t begin = self->updateData.begin;
    uint32_t end = self->updateData.end;
    if (end == begin) return;
    if (self->updateOnePerFrame) {
        if (self->index == int32_t(ld32(begin + 4))) {
            Self::updateRow(self, int32_t(ld32(begin)), int32_t(ld32(begin + 4)));
            end = self->updateData.end;
            begin = self->updateData.begin;
        }
        uint32_t tail = end - (begin + 8);
        rw::call(g::MEMMOVE, {begin, begin + 8, tail});
        uint32_t newEnd = begin + (tail & ~7u);
        uint32_t cur = self->updateData.end;
        if (cur != newEnd) self->updateData.end = cur + (~7u & ~((cur - 8) - newEnd));
    } else {
        uint32_t k = 0;
        do {
            int32_t first = int32_t(ld32(begin + 8 * k + 4));
            if (self->index == first) {
                Self::updateRow(self, int32_t(ld32(begin + 8 * k)), first);
                begin = self->updateData.begin;
                end = self->updateData.end;
            }
            k++;
        } while (k < uint32_t(int32_t(end - begin) >> 3));
        if (end != begin) self->updateData.end = end + (~7u & ~((end - 8) - begin));
    }
}

// Replacement widgets made by updateRow: add the new widgets, remove the old ones.
void applyReplacements(Self *self) {
    uint32_t a = me(self);
    uint32_t begin = self->widgetsA.begin;
    if (int32_t(uint32_t(self->widgetsA.end) - begin) < 1) return;
    if (self->updateOnePerFrame) {
        uint32_t old = ld32(self->widgetsB.begin);
        vc(a, V_addChild, {a, ld32(begin)});
        vc(a, V_removeChild, {a, old, 1});
        rw::call(g::WidgetVector_erase, {a + 1568, self->widgetsA.begin});
        rw::call(g::WidgetVector_erase, {a + 1580, self->widgetsB.begin});
    } else {
        int32_t k = 0;
        do {
            uint32_t old = ld32(self->widgetsB.begin + uint32_t(k) * 4);
            vc(a, V_addChild, {a, ld32(begin + uint32_t(k) * 4)});
            vc(a, V_removeChild, {a, old, 1});
            begin = self->widgetsA.begin;
            k++;
        } while (k < (int32_t(uint32_t(self->widgetsA.end) - begin) >> 2));
        rw::call(g::WidgetVector_clear, {a + 1568});
        rw::call(g::WidgetVector_clear, {a + 1580});
    }
}

}  // namespace

// Per frame: apply queued row updates and replacements, animate a bounce towards targetPosition, and let a released
// list coast (deceleration grows with the speed and is four times stronger past either end) until it stops - then
// report the item it stopped on (onScrollEnd) and bounce back into range (centred lists: snap to the nearest item).
void Self::update(Self *self, float dt) {
    uint32_t a = me(self);
    applyQueuedRows(self);
    applyReplacements(self);
    float mn = getMinPosition(self);
    float mx = getMaxPosition(self);

    // ---- bounce: position eases by distance * sqrt(t / duration) until it reaches the target
    if (self->motion.bouncing) {
        float t = self->bounceTime + dt;
        self->bounceTime = t;
        float ratio = t / (self->bounceDuration + self->bounceExtra);
        float dist = self->bounceDistance;
        double s = callD(g::FABS, callD(g::SQRT, double(ratio)));
        double pw = ratio == rw::bitsf(0xFF800000u) ? rw::bitsf(0x7F800000u) : s;   // pow(-inf, 0.5) = +inf
        double move = double(-0.0f - dist) * pw;
        if (self->vertical()) {
            float pos = self->position;
            if (!(pos >= mn || unord(pos, mn))) {
                float d = -0.0f - dist;
                double u = move - double(pos + (d - mn));
                self->position = float(double(pos) + u);
                double dd = double(d);
                if (!(dd > move || dd != dd || move != move)) {
                    self->motion.bouncing = 0;
                    self->position = mn;
                }
            } else if (!(pos <= mx || unord(pos, mx))) {
                double u = double(pos - (mx + dist)) - move;
                self->position = float(double(pos) - u);
                double v = -0.0 - move;
                double dd = double(dist);
                if (!(dd > v || dd != dd || v != v)) {
                    self->motion.bouncing = 0;
                    self->position = mx;
                }
            }
        } else {
            if (!(move <= 0.0 || move != move)) {
                float d = -0.0f - dist;
                float target = self->targetPosition;
                float pos = self->position;
                double u = move - double(pos + (d - target));
                self->position = float(double(pos) + u);
                double dd = double(d);
                if (!(dd > move || dd != dd || move != move)) {
                    self->motion.bouncing = 0;
                    self->position = target;
                }
            } else {
                float target = self->targetPosition;
                float pos = self->position;
                double u = double(pos - (dist + target)) - move;
                self->position = float(double(pos) - u);
                double v = -0.0 - move;
                double dd = double(dist);
                if (!(dd > v || dd != dd || v != v)) {
                    self->motion.bouncing = 0;
                    self->position = target;
                }
            }
        }
        vc(a, V_updateScroll, {a, 0});
        if (!self->motion.bouncing && self->fn4.f) {
            rw::call(g::FunctionVV_call, {a + 1376});
            self->bounceExtra = 0.0f;
        }
    }

    // ---- coasting
    float v = self->velocity;
    if (self->dragged && !(v == 0.0f)) {
        bool backward = v <= 0.0f || unord(v, 0.0f);
        float q = v / self->speedDivisor;
        float pos = self->position;
        float nv;
        float floor;
        if (!backward) {
            bool inside = mx >= pos || unord(mx, pos);
            float dec = q + 1.0f;
            if (!inside) dec = dec * 4.0f;
            nv = v - dec;
            self->velocity = nv;
            floor = self->minSpeed;
            uint32_t keep = (nv >= floor || unord(nv, floor)) ? 1u : 0u;
            float lo = mn + -200.0f;
            keep &= (pos > lo || unord(pos, lo)) ? 1u : 0u;
            if (!keep) nv = 0.0f;
        } else {
            bool inside = pos >= mn || unord(pos, mn);
            float dec = q + -1.0f;
            if (!inside) dec = dec * 4.0f;
            nv = v - dec;
            self->velocity = nv;
            floor = self->minSpeed;
            float negFloor = -0.0f - floor;
            uint32_t keep = (nv <= negFloor || unord(nv, negFloor)) ? 1u : 0u;
            float hi = mx + 200.0f;
            keep &= (hi > pos || unord(hi, pos)) ? 1u : 0u;
            if (!keep) nv = 0.0f;
        }
        self->velocity = nv;
        if (!self->motion.moving) return;
        if (nv == 0.0f) return;
        float step;
        if (!(nv <= 0.0f || unord(nv, 0.0f))) {
            step = nv < floor ? floor : nv;          // at least minSpeed
        } else {
            float lim = neg(floor);
            step = nv > lim ? lim : nv;              // at least minSpeed backwards
        }
        if (self->vertical()) self->position = step + pos;
        else self->position = pos - step;
        vc(a, V_updateScroll, {a, 0});
        return;
    }

    // ---- stopped
    uint8_t touching = self->touching;
    if (!touching) self->dragged = 0;
    if (!self->motion.moving || touching) return;
    self->motion.moving = 0;
    if (self->fn1.f) {
        float pos = self->position;
        int32_t item = 0;
        if (!(pos > 0.0f)) {
            int32_t type = self->scrollType;
            int32_t ipos = f2i(pos);
            float px = ((type | 2) == 2 ? self->itemHeight : self->itemWidth) + 0.0f;
            item = int32_t(1u - uint32_t(idiv(ipos, f2i(px))));
        }
        rw::call(g::FunctionVI_call, {a + 1304, uint32_t(item)});
    }
    if (self->motion.bouncing) return;
    if ((self->scrollType & ~1) == 2) {
        migrationBounceCenterPosition(self, 0);
        return;
    }
    float pos = self->position;
    float end;
    if (!(pos >= mn || unord(pos, mn))) {
        end = mn;
    } else {
        if (mx >= pos || unord(mx, pos)) return;
        end = mx;
    }
    self->motion.bouncing = 1;
    self->bounceDistance = pos - end;
    self->bounceTime = 0.0f;
    self->targetPosition = end;
}
RW_FUNCTION(0x59BFA0, "BaseStageScrollView::update(float)", BaseStageScrollView::update);
