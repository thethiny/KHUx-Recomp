// BaseStageScrollView: init() and the touch listener's lambdas (drag, fling, tap sound).
// The lambdas' std::function slots (clone, destroy, target...) are in src/scene_main/BaseStageScrollView_thunks.cpp.
#include "BaseStageScrollView_common.hpp"

using namespace bssv;

namespace {
// The drag distance from the touch start to `now` along the scroll direction (vertical: start - now negated; the
// horizontal one is not), damped by half past either end of the list `list` (the lambda's second capture).
float dragDistance(Self *list, float delta, bool vertical) {
    float mn = Self::getMinPosition(list);
    float mx = Self::getMaxPosition(list);
    float pos = vertical ? list->position - delta : delta + list->position;
    float d = vertical ? neg(delta) : delta;
    if (!(pos >= mn || unord(pos, mn))) return d + (pos - mn) * -0.5f;
    if (!(mx >= pos || unord(mx, pos))) return d + (pos - mx) * -0.5f;
    return d;
}

// Touch::getLocation into a frame Vec2; returns (x, y) and destroys the temporary.
struct Loc { float x, y; };
Loc location(uint32_t vec, uint32_t touch) {
    cx(g::Touch_getLocation, {vec, touch});
    Loc l{ldf(vec), ldf(vec + 4)};
    cx(g::Vec2_dtor, {vec});
    return l;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ init
uint32_t Self::init(Self *self) {
    uint32_t a = me(self);
    guest::Frame frame(20 + 0x34);
    uint32_t caps = frame.addr(8);          // {this, this}: the touch-ended lambda's captures
    uint32_t fn = frame.addr(0x10);         // a std::function on the stack (callable pointer at +16)
    vx(a, V_setTouchEnabled, {a, 1});
    self->touching = 0;
    self->position = 0.0f;
    self->index = 0;
    // itemIDs.clear()
    uint32_t end = self->itemIDs.end, begin = self->itemIDs.begin;
    if (end != begin) self->itemIDs.end = end + (~3u & ~((end - 4) - begin));
    self->minSpeed = 10.0f;
    self->speedDivisor = 32.0f;
    self->dragDivisor = 4.0f;
    self->bounceDuration = self->field_4FC;
    uint32_t dispatcher = ld32(cx(g::Director_getInstance, {}) + DIRECTOR_EVENT_DISPATCHER);
    *rw::frame_ptr<uint32_t>(frame.addr(0)) = dispatcher;
    uint32_t listener = cx(g::EventListenerTouchOneByOne_create, {});
    cx(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    // onTouchBegan = $_0 [this]
    *rw::frame_ptr<uint32_t>(fn + 16) = fn;
    *rw::frame_ptr<uint32_t>(fn) = VT_TouchBegan;
    *rw::frame_ptr<uint32_t>(fn + 4) = a;
    cx(g::FunctionTouchBool_swap, {fn, listener + LISTENER_ON_TOUCH_BEGAN});
    destroyFunction(fn, true);
    // onTouchMoved = $_1 [this, this]
    *rw::frame_ptr<uint32_t>(fn + 16) = fn;
    *rw::frame_ptr<uint32_t>(fn) = VT_TouchMoved;
    *rw::frame_ptr<uint32_t>(fn + 8) = a;
    *rw::frame_ptr<uint32_t>(fn + 4) = a;
    cx(g::FunctionTouchVoid_swap, {fn, listener + LISTENER_ON_TOUCH_MOVED});
    destroyFunction(fn, true);
    // onTouchEnded = onTouchCancelled = $_2 [this, this]
    *rw::frame_ptr<uint32_t>(caps) = a;
    *rw::frame_ptr<uint32_t>(caps + 4) = a;
    assignTouchEnded(listener + LISTENER_ON_TOUCH_ENDED, caps);
    assignTouchEnded(listener + LISTENER_ON_TOUCH_CANCELLED, caps);
    cx(g::EventDispatcher_addListenerWithSceneGraphPriority, {dispatcher, listener, a});
    cx(g::Node_scheduleUpdate, {a});
    return 1;
}
RW_FUNCTION(0x59BD94, "BaseStageScrollView::init", BaseStageScrollView::init);

// std::function<void(Touch*, Event*)>::operator=($_2&&): build the lambda in a stack std::function and swap it in.
void Self::assignTouchEnded(uint32_t function, uint32_t captures) {
    guest::Frame frame(20 + 0x2C);
    uint32_t fn = frame.addr(8);
    uint32_t c0 = ld32(captures), c1 = ld32(captures + 4);
    *rw::frame_ptr<uint32_t>(fn + 16) = fn;
    *rw::frame_ptr<uint32_t>(fn) = VT_TouchEnded;
    *rw::frame_ptr<uint32_t>(fn + 8) = c1;
    *rw::frame_ptr<uint32_t>(fn + 4) = c0;
    cx(g::FunctionTouchVoid_swap, {fn, function});
    destroyFunction(fn, true);
}
RW_FUNCTION(0x59BF24, "std::function<void(Touch*,Event*)>::operator=(BaseStageScrollView::init::$_2&&)",
            BaseStageScrollView::assignTouchEnded);

// ------------------------------------------------------------------------------------------------ lambdas
// $_0: a touch inside the list starts a drag (stops any coasting / bounce). Returns whether it is taken.
uint32_t Self::onTouchBeganLambda(uint32_t functor, uint32_t touchRef, uint32_t) {
    Self *self = rw::host_ptr<Self>(ld32(functor + 4));
    uint32_t a = me(self);
    guest::Frame frame(20 + 0x4C);
    if (self->field_5A0) return 0;
    uint32_t touch = ld32(touchRef);
    if (self->touchLocked || !self->scrollEnabled) return 0;
    if (int32_t(cx(g::SceneUtil_getNexTutorialphase, {})) < 995) return 0;
    uint32_t loc = frame.addr(0x38), local = frame.addr(0x40), rect = frame.addr(0x28);
    uint32_t parent = vx(a, V_getParent, {a});
    cx(g::Touch_getLocation, {loc, touch});
    cx(g::Node_convertToNodeSpace, {local, parent, loc});
    cx(g::Vec2_dtor, {loc});
    uint32_t x = vx(a, V_getPositionX, {a});
    uint32_t y = vx(a, V_getPositionY, {a});
    uint32_t w = ld32(vx(a, V_getContentSize, {a}));
    uint32_t h = ld32(vx(a, V_getContentSize, {a}) + 4);
    cx(g::Rect_ctor, {rect, x, y, w, h});      // h is the fifth argument, at [sp]
    uint32_t taken = 0;
    if (cx(g::Rect_containsPoint, {rect, local})) {
        self->motion.moving = 0;
        self->velocity = 0.0f;
        self->touching = 1;
        taken = 1;
        self->touchStartX = location(frame.addr(0x20), touch).x;
        self->touchStartY = location(frame.addr(0x18), touch).y;
        self->lastTouchX = self->touchStartX;
        self->lastTouchY = self->touchStartY;
        self->motion.bouncing = 0;
    }
    cx(g::Vec2_dtor, {rect});
    cx(g::Vec2_dtor, {local});
    return taken;
}
RW_FUNCTION(0x59E678, "BaseStageScrollView::init::$_0::operator()(Touch*, Event*)",
            BaseStageScrollView::onTouchBeganLambda);

// $_1: the finger moved: add the move to the velocity, and once it passed 20 pixels from the start drag the list
// (updateScroll by the damped distance, onDrag).
void Self::onTouchMovedLambda(uint32_t functor, uint32_t touchRef, uint32_t) {
    Self *self = rw::host_ptr<Self>(ld32(functor + 4));
    uint32_t a = me(self);
    guest::Frame frame(20 + 0x64);
    if (self->field_5A0) return;
    uint32_t touch = ld32(touchRef);
    if (self->touchLocked || !self->scrollEnabled) return;
    if (int32_t(cx(g::SceneUtil_getNexTutorialphase, {})) < 995) return;
    Self *list = rw::host_ptr<Self>(ld32(functor + 8));
    float delta;
    if (self->vertical()) {
        float now = location(frame.addr(0x58), touch).y;
        self->velocity = (now - self->lastTouchY) / self->dragDivisor + self->velocity;
        self->lastTouchY = location(frame.addr(0x50), touch).y;
        float start = self->touchStartY;
        cx(g::Touch_getLocation, {frame.addr(0x48), touch});
        delta = dragDistance(list, start - ldf(frame.addr(0x4C)), true);
        cx(g::Vec2_dtor, {frame.addr(0x48)});
        if (!self->dragged) {
            if (absf(delta) < 20.0f) return;
            self->dragged = 1;
            self->touchStartY = location(frame.addr(0x40), touch).y;
            self->lastTouchY = self->touchStartY;
            return;
        }
    } else {
        float now = location(frame.addr(0x38), touch).x;
        self->velocity = (now - self->lastTouchX) / self->dragDivisor + self->velocity;
        self->lastTouchX = location(frame.addr(0x30), touch).x;
        float start = self->touchStartX;
        cx(g::Touch_getLocation, {frame.addr(0x28), touch});
        delta = dragDistance(list, start - ldf(frame.addr(0x28)), false);
        cx(g::Vec2_dtor, {frame.addr(0x28)});
        if (!self->dragged) {
            if (absf(delta) < 20.0f) return;
            self->dragged = 1;
            self->touchStartX = location(frame.addr(0x20), touch).x;
            self->lastTouchX = self->touchStartX;
            return;
        }
    }
    vx(a, V_updateScroll, {a, fb(delta)});
    if (self->fn2.f) cx(g::FunctionVF_call, {a + 1328, fb(delta)});
}
RW_FUNCTION(0x59E1DC, "BaseStageScrollView::init::$_1::operator()(Touch*, Event*)",
            BaseStageScrollView::onTouchMovedLambda);

// $_2 (touch ended / cancelled): a drag scrolls by its last damped distance and starts coasting (moving); a move of
// 16 pixels or more from the start plays the scroll sound effect; onTouchEnded.
void Self::onTouchEndedLambda(uint32_t functor, uint32_t touchRef, uint32_t) {
    Self *self = rw::host_ptr<Self>(ld32(functor + 4));
    uint32_t a = me(self);
    guest::Frame frame(20 + 0x4C);
    uint32_t touch = ld32(touchRef);
    self->touching = 0;
    if (self->touchLocked || self->field_5A0 || !self->scrollEnabled) {
        if (self->fn3.f) cx(g::FunctionVV_call, {a + 1352});
        return;
    }
    if (self->dragged) {
        Self *list = rw::host_ptr<Self>(ld32(functor + 8));
        float delta;
        if (self->vertical()) {
            float start = self->touchStartY;
            cx(g::Touch_getLocation, {frame.addr(0x38), touch});
            delta = dragDistance(list, start - ldf(frame.addr(0x3C)), true);
            cx(g::Vec2_dtor, {frame.addr(0x38)});
        } else {
            float start = self->touchStartX;
            cx(g::Touch_getLocation, {frame.addr(0x30), touch});
            delta = dragDistance(list, start - ldf(frame.addr(0x30)), false);
            cx(g::Vec2_dtor, {frame.addr(0x30)});
        }
        self->position = delta + self->position;
        vx(a, V_updateScroll, {a, 0});
    }
    float startX = self->touchStartX;
    float nowX = location(frame.addr(0x28), touch).x;
    float startY = self->touchStartY;
    float nowY = location(frame.addr(0x20), touch).y;
    bool moved = absf(nowX - startX) >= 16.0f;
    if (!moved) {
        float dy = absf(nowY - startY);
        moved = !(dy < 16.0f || unord(dy, 16.0f));
    }
    if (moved) {
        uint32_t layer = frame.addr(0x40);       // std::shared_ptr<audio::Layer>
        uint32_t manager = cx(g::audio_Manager_getInstance, {});
        cx(g::audio_Manager_getLayer, {layer, manager, 0});
        cx(g::audio_Layer_playSE, {ld32(layer), SE_SCROLL, 0, 0});
        uint32_t ctrl = ld32(layer + 4);
        if (ctrl) cx(guest::SHARED_RELEASE, {ctrl});
    }
    self->motion.moving = 1;
    if (self->fn3.f) cx(g::FunctionVV_call, {a + 1352});
}
RW_FUNCTION(0x59DE00, "BaseStageScrollView::init::$_2::operator()(Touch*, Event*)",
            BaseStageScrollView::onTouchEndedLambda);
