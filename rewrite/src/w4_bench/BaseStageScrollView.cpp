// BaseStageScrollView: construction, geometry (min / max scroll positions), row recycling and scrolling.
// The per-frame update and the touch handling are in BaseStageScrollView_update.cpp / _touch.cpp.
#include "BaseStageScrollView_common.hpp"

using namespace bssv;

// ------------------------------------------------------------------------------------------------ life cycle
Self *Self::construct(Self *self) {
    uint32_t a = me(self);
    rw::call(g::Layout_ctor, {a});
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    self->field_4FC = 0.3f;
    // the five std::function members start empty
    self->fn0.f = 0;
    self->fn1.f = 0;
    self->fn2.f = 0;
    self->fn3.f = 0;
    self->fn4.f = 0;
    self->field_5A1 = 0;
    self->field_5A0 = 0;
    rw::call(g::aeabi_memset, {a + 1404, 0x20, 0});   // item size, itemIDs, widgets (+1404..+1435)
    rw::call(g::aeabi_memset, {a + 1444, 0x14, 0});   // speeds and durations (+1444..+1463)
    self->maxColumn = 1;
    self->scrollEnabled = 1;
    self->touchLocked = 0;
    rw::call(g::aeabi_memset, {a + 1472, 0x12, 0});   // position .. touching / dragged (+1472..+1489)
    self->field_5E0 = 0;                               // moving, bouncing
    self->velocity = 0.0f;
    self->lastTouchX = 0.0f;
    self->lastTouchY = 0.0f;
    rw::call(g::aeabi_memset, {a + 1508, 0x29, 0});   // bounce state .. floats, variableSize (+1508..+1548)
    rw::call(g::aeabi_memset, {a + 1552, 0x29, 0});   // scrollType .. updateOnePerFrame (+1552..+1592)
    return self;
}
RW_FUNCTION(0x55DAE4, "BaseStageScrollView::BaseStageScrollView()", BaseStageScrollView::construct);

Self *Self::destroy(Self *self) {
    uint32_t a = me(self);
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    rw::call(g::Node_unscheduleUpdate, {a});
    rw::call(g::WidgetVector_dtor, {a + 1580});
    rw::call(g::WidgetVector_dtor, {a + 1568});
    rw::call(g::UpdateDataVector_dtor, {a + 1556});
    rw::call(g::FloatVector_dtor, {a + 1536});
    rw::call(g::WidgetVector_dtor, {a + 1424});
    rw::call(g::IntVector_dtor, {a + 1412});
    destroyFunction(a + 1376, false);
    destroyFunction(a + 1352, false);
    destroyFunction(a + 1328, false);
    destroyFunction(a + 1304, false);
    destroyFunction(a + 1280, false);
    rw::call(g::Layout_dtor, {a});
    return self;
}
RW_FUNCTION(0x59BADC, "BaseStageScrollView::~BaseStageScrollView() [base]", BaseStageScrollView::destroy);

// ------------------------------------------------------------------------------------------------ geometry
// The scroll position of the first item: minus one item, or (centred lists) so the first item sits in the middle
// of the view; plus minOffset.
float Self::getMinPosition(Self *self) {
    int32_t type = self->scrollType;
    float pos = neg(((type | 2) == 2 ? self->itemHeight : self->itemWidth) + 0.0f);
    if ((type & ~1) == 2) {
        uint32_t size = vc(me(self), V_getContentSize, {me(self)});
        float extent = ldf(type == 2 ? size + 4 : size);
        float px = self->itemPixelRaw() + 0.0f;
        pos = pos + (extent - px) * -0.5f;
    }
    return pos + self->minOffset;
}
RW_FUNCTION(0x59CB14, "BaseStageScrollView::getMinPosition", BaseStageScrollView::getMinPosition);

// The scroll position of the end: item extent * (rows - 1) minus the view extent (centred lists: so the last item
// sits in the middle), plus maxOffset; never below the minimum.
float Self::getMaxPosition(Self *self) {
    uint32_t a = me(self);
    bool vert = (self->scrollType | 2) == 2;
    int32_t bytes = int32_t(uint32_t(self->itemIDs.end) - uint32_t(self->itemIDs.begin));
    int32_t cols = self->maxColumn;
    float px = vert ? self->itemHeight : self->itemWidth;
    uint32_t size = vc(a, V_getContentSize, {a});
    double pxd = double(px + 0.0f);
    float rows = callF(g::CEILF, float(uint32_t(bytes >> 2)) / float(cols));
    double span = pxd * ((double(rows) + -2.0) + 1.0);
    float pos = float(span - double(ldf(vert ? size + 4 : size)));
    int32_t type = self->scrollType;
    if ((type & ~1) == 2) {
        uint32_t size2 = vc(a, V_getContentSize, {a});
        float extent = ldf(type == 2 ? size2 + 4 : size2);
        float px2 = self->itemPixelRaw() + 0.0f;
        pos = pos + (extent - px2) * 0.5f;
    }
    pos = pos + self->maxOffset;
    float mn = getMinPosition(self);
    return pos < mn ? mn : pos;
}
RW_FUNCTION(0x59CBC0, "BaseStageScrollView::getMaxPosition", BaseStageScrollView::getMaxPosition);

float Self::getItemPixel(Self *self) { return self->itemPixelRaw() + 0.0f; }
RW_FUNCTION(0x59CDDC, "BaseStageScrollView::getItemPixel", BaseStageScrollView::getItemPixel);

// The view extent along the scroll direction.
float Self::getScrollIntervalPixel(Self *self) {
    bool vert = self->vertical();
    uint32_t size = vc(me(self), V_getContentSize, {me(self)});
    return ldf(vert ? size + 4 : size);
}
RW_FUNCTION(0x59D578, "BaseStageScrollView::getScrollIntervalPixel", BaseStageScrollView::getScrollIntervalPixel);

uint32_t Self::isScroll(Self *self) {
    if (self->touching || self->dragged || self->motion.moving || self->motion.bouncing) return 1;
    return 0;
}
RW_FUNCTION(0x59DC18, "BaseStageScrollView::isScroll", BaseStageScrollView::isScroll);

// Keep a scroll position within 200 pixels past either end (both *pos and the position are clamped).
void Self::checkScrollInterval(Self *self, const float *pos) {
    float mn = getMinPosition(self);
    float mx = getMaxPosition(self);
    float lo = mn + -200.0f;
    float p = *pos;
    if (!(p >= lo || unord(p, lo))) {
        rw::set<float>(pos, 0, lo);
        self->position = lo;
        return;
    }
    float hi = mx + 200.0f;
    if (!(hi >= p || unord(hi, p))) {
        rw::set<float>(pos, 0, hi);
        self->position = hi;
    }
}
RW_FUNCTION(0x59D9F0, "BaseStageScrollView::checkScrollInterval(float&)", BaseStageScrollView::checkScrollInterval);

// ------------------------------------------------------------------------------------------------ migrations
// Start a bounce to the item `step` items after the one nearest the current position (clamped to the list).
// Returns whether that item exists.
uint32_t Self::migrationBounceCenterPosition(Self *self, int32_t step) {
    float mn = getMinPosition(self);
    int32_t type = self->scrollType;
    float px = (type | 2) == 2 ? self->itemHeight : self->itemWidth;
    uint32_t begin = self->itemIDs.begin, end = self->itemIDs.end;
    float pos = self->position;
    self->motion.bouncing = 1;
    float pxf = px + 0.0f;
    float rel = pos - mn;
    int32_t nearest = idiv(f2i(rel + pxf * 0.5f), f2i(pxf));
    int32_t item = int32_t(uint32_t(nearest) + uint32_t(step));
    int32_t last = (int32_t(end - begin) >> 2) - 1;
    int32_t clamped = item < last ? item : last;
    float f = float(clamped);
    float target = mn + pxf * (clamped < 0 ? 0.0f : f);
    self->targetPosition = target;
    uint32_t exists = (item < last ? 1u : 0u) & (item >= 0 ? 1u : 0u);
    if (type == 1) {
        float mx = getMaxPosition(self);
        target = self->targetPosition;
        if (!(target <= mx || unord(target, mx))) {
            self->targetPosition = mx;
            target = mx;
        }
    }
    self->bounceDistance = self->position - target;
    self->bounceTime = 0.0f;
    return exists;
}
RW_FUNCTION(0x59CE04, "BaseStageScrollView::migrationBounceCenterPosition", BaseStageScrollView::migrationBounceCenterPosition);

// Start a bounce to the last item; the bounce gets 0.1 s more per item to travel (total at most 1 s). The
// original returns the new bounceExtra when it was capped, else the comparison result (1).
uint32_t Self::migrationMaxPosition(Self *self) {
    float mn = getMinPosition(self);
    float px = self->itemPixelRaw();
    float pos = self->position;
    int32_t count = int32_t(uint32_t(self->itemIDs.end) - uint32_t(self->itemIDs.begin)) >> 2;
    self->motion.bouncing = 1;
    float pxf = px + 0.0f;
    int32_t last = count - 1;
    float target = mn + pxf * float(last);
    self->targetPosition = target;
    self->bounceDistance = pos - target;
    self->bounceTime = 0.0f;
    float rel = pos - mn;
    int32_t nearest = idiv(f2i(rel + pxf * 0.5f), f2i(pxf));
    float extra = float(int32_t(uint32_t(last) - uint32_t(nearest))) * 0.1f;
    self->bounceExtra = extra;
    float duration = self->bounceDuration;
    float total = duration + extra;
    if (!(total <= 1.0f || unord(total, 1.0f))) {
        float capped = 1.0f - duration;
        self->bounceExtra = capped;
        return fb(capped);
    }
    return 1;
}
RW_FUNCTION(0x59DAC8, "BaseStageScrollView::migrationMaxPosition", BaseStageScrollView::migrationMaxPosition);

// ------------------------------------------------------------------------------------------------ rows
// Row `row` now shows item first + row - 1 (row 0 is the spare row above the view): ask the derived class whether
// the widget must be replaced (changeRow); a replacement is created now and swapped in by update(). Rows past the
// list are handed to changeOverRow.
void Self::updateRow(Self *self, int32_t row, int32_t first) {
    uint32_t a = me(self);
    int32_t pos = int32_t(uint32_t(first) + uint32_t(row));
    uint32_t idx = uint32_t(pos) - 1;
    uint32_t old = ld32(self->widgets.begin + uint32_t(row) * 4);
    uint32_t ids = self->itemIDs.begin;
    if (pos < 1 || idx >= uint32_t(int32_t(uint32_t(self->itemIDs.end) - ids) >> 2)) {
        vc(a, V_changeOverRow, {a, old, idx});
        return;
    }
    uint32_t changeRow = vfn(a, V_changeRow);
    uint32_t widget = old;
    if (rw::call(changeRow, {a, old, ld32(ids + idx * 4), 0}) == 1) {
        if (row == 0) return;
        widget = vc(a, V_createRow, {a, ld32(self->itemIDs.begin + idx * 4), 0});
        rw::call(g::WidgetVector_pushBack, {a + 1568, widget});
        rw::call(g::WidgetVector_pushBack, {a + 1580, old});
        rw::call(g::Ref_release, {ld32(self->widgets.begin + uint32_t(row) * 4)});
        rw::save<uint32_t>(self->widgets.begin + uint32_t(row) * 4, widget);
        rw::call(g::Ref_retain, {widget});
        uint32_t setPosition = vfn(widget, V_setPosition);
        rw::call(setPosition, {widget, vc(old, V_getPosition, {old})});
        vc(a, V_registItemTouchEvent, {a, widget, uint32_t(row)});
    }
    vc(widget, V_setVisible, {widget, 1});
}
RW_FUNCTION(0x59C9F4, "BaseStageScrollView::updateRow", BaseStageScrollView::updateRow);

// Refresh every row for the current position: now (updateRow) or queued for update() when deferred.
void Self::updateLayout(Self *self, uint32_t deferred) {
    uint32_t a = me(self);
    float px = self->itemPixelRaw() + 0.0f;
    int32_t ipos = f2i(self->position);
    int32_t first = int32_t(uint32_t(self->maxColumn) * uint32_t(idiv(ipos, f2i(px))));
    float rows = self->rowCount;
    if (rows <= 0.0f || unord(rows, 0.0f)) return;
    guest::Frame frame(20 + 0x34);
    uint32_t local = frame.addr(0x20);     // UpdateData{row, first}
    int32_t row = 0;
    do {
        if (deferred == 1) {
            *rw::frame_ptr<int32_t>(local) = row;
            *rw::frame_ptr<int32_t>(local + 4) = first;
            uint32_t end = self->updateData.end;
            if (end < uint32_t(self->updateData.cap)) {
                uint32_t next = 0;
                if (end) {
                    rw::save<int32_t>(end, row);
                    rw::save<int32_t>(end + 4, first);
                    next = self->updateData.end;
                }
                self->updateData.end = next + 8;
            } else {
                cx(g::UpdateData_pushBackSlow, {a + 1556, local});
            }
        } else {
            updateRow(self, row, first);
        }
        row++;
    } while (float(row) < self->rowCount);
}
RW_FUNCTION(0x59D3F4, "BaseStageScrollView::updateLayout", BaseStageScrollView::updateLayout);

// Show or hide the pending replacement widgets.
void Self::setVisibleWidgets(Self *self, uint32_t visible) {
    uint32_t begin = self->widgetsA.begin;
    if (int32_t(uint32_t(self->widgetsA.end) - begin) < 1) return;
    int32_t k = 0;
    do {
        uint32_t w = ld32(begin + uint32_t(k) * 4);
        vc(w, V_setVisible, {w, visible});
        begin = self->widgetsA.begin;
        k++;
    } while (k < (int32_t(uint32_t(self->widgetsA.end) - begin) >> 2));
}
RW_FUNCTION(0x59D530, "BaseStageScrollView::setVisibleWidgets", BaseStageScrollView::setVisibleWidgets);

void Self::setItemIDs(Self *self, uint32_t ids) {
    uint32_t a = me(self);
    if (a + 1412 != ids) rw::call(g::IntVector_assign, {a + 1412, ld32(ids), ld32(ids + 4)});
    float mx = getMaxPosition(self);
    int32_t type = self->scrollType;
    int32_t imx = f2i(mx);
    float px = ((type | 2) == 2 ? self->itemHeight : self->itemWidth) + 0.0f;
    int32_t last = int32_t(uint32_t(self->maxColumn) * uint32_t(idiv(imx, f2i(px))));
    if (last < self->index) {
        self->index = last;
        self->position = getMaxPosition(self);
    }
    if (self->variableSize == 0) updateLayout(self, 0);
}
RW_FUNCTION(0x59D350, "BaseStageScrollView::setItemIDs(std::vector<int>)", BaseStageScrollView::setItemIDs);

void Self::setIndex(Self *self, int32_t index) {
    uint32_t a = me(self);
    int32_t row = idiv(index, self->maxColumn);
    float frow = float(row);
    float mn = getMinPosition(self);
    float px = self->itemPixelRaw() + 0.0f;
    self->position = mn + frow * px;
    self->index = row;
    vc(a, V_updateScroll, {a, 0});
}
RW_FUNCTION(0x59D5A8, "BaseStageScrollView::setIndex", BaseStageScrollView::setIndex);

// Rebuild the row pool for a new size: remove the old widgets, create rowCount = (floor(extent / px) + 5) *
// maxColumn new ones and place them (vertical: x by column, y down from the top; horizontal: x by row).
void Self::setContentSize(Self *self, uint32_t size) {
    uint32_t a = me(self);
    guest::Frame frame(20 + 0x64);
    uint32_t oldSize = frame.addr(0x58);
    uint32_t vec = frame.addr(0x50);
    cx(g::Size_copy, {oldSize, vx(a, V_getContentSize, {a})});
    cx(g::Widget_setContentSize, {a, size});
    if (ldf(oldSize) == ldf(size) && ldf(oldSize + 4) == ldf(size + 4)) return;
    // drop the old pool
    uint32_t begin = self->widgets.begin;
    if (int32_t(uint32_t(self->widgets.end) - begin) >= 1) {
        int32_t k = 0;
        do {
            uint32_t w = ld32(begin + uint32_t(k) * 4);
            if (vx(w, V_getParent, {w})) vx(a, V_removeChild, {a, w, 1});
            begin = self->widgets.begin;
            k++;
        } while (k < (int32_t(uint32_t(self->widgets.end) - begin) >> 2));
    }
    cx(g::WidgetVector_clear, {a + 1424});
    cx(g::WidgetVector_clear, {a + 1568});
    cx(g::WidgetVector_clear, {a + 1580});
    bool vert = (self->scrollType | 2) == 2;
    float px = (vert ? self->itemHeight : self->itemWidth) + 0.0f;
    float extent = ldf(vert ? size + 4 : size);
    double rows = double(callF(g::FLOORF, extent / px)) + 5.0;
    self->rowCount = float(rows * double(self->maxColumn));
    float count = self->rowCount;
    if (count <= 0.0f || unord(count, 0.0f)) return;
    float half = px * 0.5f;
    int32_t k = 0;
    do {
        uint32_t w = cx(g::Widget_create, {});
        int32_t slot = int32_t(uint32_t(k) - 2);
        int32_t cols = self->maxColumn;
        int32_t col = imod(slot, cols);
        bool v = (self->scrollType | 2) == 2;
        int32_t line = idiv(slot, cols);
        float x, y;
        if (v) {
            float wdt = self->itemWidth;
            x = self->offsetX + (float(col + 1) * wdt + wdt * -0.5f);
            y = ((extent - px * float(line)) - half) + self->offsetY;
            if (cols == 1) x = rw::bitsf(vx(w, V_getPositionX, {w})) + self->offsetX;
        } else {
            float hgt = self->itemHeight;
            y = float(col + 1) * hgt + hgt * 0.5f;
            x = px * float(line);
            if (cols == 1) y = rw::bitsf(vx(w, V_getPositionY, {w})) + self->offsetY;
        }
        uint32_t setPosition = vfn(w, V_setPosition);
        cx(g::Vec2_ctor, {vec, fb(x), fb(y)});
        cx(setPosition, {w, vec});
        cx(g::Vec2_dtor, {vec});
        cx(g::WidgetVector_pushBack, {a + 1424, w});
        vx(a, V_registItemTouchEvent, {a, w, uint32_t(k)});
        k++;
    } while (float(k) < self->rowCount);
}
RW_FUNCTION(0x59CF8C, "BaseStageScrollView::setContentSize", BaseStageScrollView::setContentSize);

// Scroll by delta: clamp, work out the first visible item (onIndexChanged during a touch) and place the rows; when
// the first item changed every row is refreshed (updateRow).
void Self::updateScroll(Self *self, float delta) {
    uint32_t a = me(self);
    if (!self->scrollEnabled) return;
    if (uint32_t(self->itemIDs.end) == uint32_t(self->itemIDs.begin)) return;
    guest::Frame frame(20 + 0x6C);
    uint32_t newPos = frame.addr(0x64);
    uint32_t vec = frame.addr(0x5C);
    float px = self->itemPixelRaw();
    getMinPosition(self);                  // results unused
    getMaxPosition(self);
    *rw::frame_ptr<float>(newPos) = self->position + delta;
    checkScrollInterval(self, rw::host_ptr<float>(newPos));
    float pxf = px + 0.0f;
    int32_t ipos = f2i(*rw::host_ptr<float>(newPos));
    int32_t ipx = f2i(pxf);
    int32_t first = int32_t(uint32_t(self->maxColumn) * uint32_t(idiv(ipos, ipx)));
    int32_t oldFirst = self->index;
    self->index = first;
    int32_t rem = imod(ipos, ipx);
    if (oldFirst != first && (self->touching || self->dragged) && self->fn0.f)
        cx(g::FunctionVI_call, {a + 1280, uint32_t(first + 1)});
    float rows = self->rowCount;
    if (rows <= 0.0f || unord(rows, 0.0f)) return;
    float half = pxf * 0.5f;
    float frem = float(rem);
    int32_t row = 0;
    do {
        int32_t slot = int32_t(uint32_t(row) - 2);
        int32_t cols = self->maxColumn;
        int32_t col = imod(slot, cols);
        bool v = (self->scrollType | 2) == 2;
        uint32_t w = ld32(self->widgets.begin + uint32_t(row) * 4);
        int32_t line = idiv(slot, cols);
        float x, y;
        if (v) {
            float wdt = self->itemWidth;
            x = self->offsetX + (float(col + 1) * wdt + wdt * -0.5f);
            uint32_t size = vx(a, V_getContentSize, {a});
            y = frem + (self->offsetY + ((ldf(size + 4) - pxf * float(line)) - half));
            if (self->maxColumn == 1) x = rw::bitsf(vx(w, V_getPositionX, {w}));
        } else {
            float hgt = self->itemHeight;
            y = float(col + 1) * hgt + hgt * 0.5f;
            x = (pxf * float(line) - frem) + self->offsetX;
            if (cols == 1) y = rw::bitsf(vx(w, V_getPositionY, {w}));
        }
        uint32_t setPosition = vfn(w, V_setPosition);
        cx(g::Vec2_ctor, {vec, fb(x), fb(y)});
        cx(setPosition, {w, vec});
        cx(g::Vec2_dtor, {vec});
        if (oldFirst != first) updateRow(self, row, first);
        row++;
    } while (float(row) < self->rowCount);
}
RW_FUNCTION(0x59D66C, "BaseStageScrollView::updateScroll", BaseStageScrollView::updateScroll);
