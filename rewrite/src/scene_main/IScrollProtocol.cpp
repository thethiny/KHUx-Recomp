// IScrollProtocol: the recycling scroll list behind every list in the game (see IScrollProtocol.hpp).
#include "IScrollProtocol.hpp"

#include <cmath>

using namespace scene;
using Self = IScrollProtocol;

namespace {
namespace g {
constexpr uint32_t Layout_dtor = 0x8289C5;                 // cocos2d::ui::Layout::~Layout
constexpr uint32_t Widget_setContentSize = 0x825C75;       // ui::Widget::setContentSize(const Size&)
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t WidgetVector_dtor = 0x485A8F;           // cocos2d::Vector<ui::Widget*>::~Vector
constexpr uint32_t WidgetVector_clear = 0x485B45;
constexpr uint32_t WidgetVector_pushBack = 0x4F6B5D;
constexpr uint32_t WidgetVector_erase = 0x4F64B1;          // erase(iterator)
constexpr uint32_t UpdateDataVector_dtor = 0x4F59D1;       // std::__vector_base<UpdateData>::~__vector_base
constexpr uint32_t UpdateData_pushBackSlow = 0x4F82C1;     // vector<UpdateData>::__push_back_slow_path(UpdateData&&)
constexpr uint32_t FloatVector_dtor = 0x4F59FF;            // std::__vector_base<float>::~__vector_base
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Node_stopAllActions = 0x893815;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;     // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t Touch_getLocation = 0x900E57;           // (Vec2* out, Touch*)
constexpr uint32_t Size_copy = 0x8DF02D;                   // Size::Size(const Size&)
constexpr uint32_t Size_ctor = 0x8DF027;                   // Size::Size(float, float)
constexpr uint32_t Vec2_ctor = 0x8E4B9D;                   // Vec2::Vec2(float, float)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor = 0x8DF18B;                   // Rect::Rect(x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t DelayTime_create = 0x86852D;            // (float)
constexpr uint32_t FadeTo_create = 0x86B3E1;               // (float, uint8)
constexpr uint32_t Sequence_create = 0x867999;             // (action, ..., nullptr)
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FunctionTouchBool_swap = 0x483E4D;      // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;      // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVV_call = 0x470E69;             // std::function<void()>::operator()
constexpr uint32_t FunctionVI_call = 0x4F321D;             // std::function<void(int)>::operator()(int)
constexpr uint32_t FunctionVF_call = 0x4F8E89;             // std::function<void(float)>::operator()(float)
constexpr uint32_t ScrollBar_create = 0x4F5025;            // ScrollBar::create(IScrollProtocol*)
constexpr uint32_t ScrollBar_setBackgroundVisible = 0x4F57C1;
constexpr uint32_t ScrollBar_updateLayout = 0x4F53F9;
constexpr uint32_t ScrollBar_setScrollPosition = 0x4F56A5;  // (float)
constexpr uint32_t SceneUtil_setCascadeOpacityEnabled = 0x45B16D;
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;          // std::vector::at failure (never returns)
constexpr uint32_t MEMMOVE = 0x40EF50;                     // __aeabi_memmove
constexpr uint32_t CEILF = 0x40EFBC, FLOORF = 0x415244;    // libm imports
constexpr uint32_t SQRT = 0x415208, FABS = 0x415214;       // (double)
constexpr uint32_t DIVSI3 = 0xCD5E8D, MODSI3 = 0xCD6009;
constexpr uint32_t ANCHOR_MIDDLE = 0xFEF5C0;               // cocos2d::Vec2::ANCHOR_MIDDLE
}  // namespace g

// __func vtables (+8) of init()'s lambdas
constexpr uint32_t VT_TouchBegan = 0xF55B28;   // $_0 [this]
constexpr uint32_t VT_TouchMoved = 0xF55AFC;   // $_1 [this, this]
constexpr uint32_t VT_TouchEnded = 0xF55AD0;   // $_2 [this, this]
constexpr uint32_t VT_OnEnter = 0xF55AA4;      // $_3 [this]
constexpr uint32_t VT_OnExit = 0xF55A78;       // $_4 [this]

constexpr uint32_t SIGN = 0x80000000u;
constexpr float NEG_HALF = -0.5f;

inline float neg(float v) { return rw::bitsf(rw::fbits(v) ^ SIGN); }   // eors with the sign bit
inline uint32_t fb(float f) { return rw::fbits(f); }
inline bool unord(float a, float b) { return a != a || b != b; }
// libgcc integer division (the guest helpers: division by zero stays theirs)
inline int32_t idiv(int32_t a, int32_t b) { return int32_t(call(g::DIVSI3, {uint32_t(a), uint32_t(b)})); }
inline int32_t imod(int32_t a, int32_t b) { return int32_t(call(g::MODSI3, {uint32_t(a), uint32_t(b)})); }
inline float i2f(int32_t v) { return float(v); }
inline double callD(uint32_t fn, double v) {
    uint64_t u;
    std::memcpy(&u, &v, 8);
    uint64_t r = rw::call64(fn, {uint32_t(u), uint32_t(u >> 32)});
    double out;
    std::memcpy(&out, &r, 8);
    return out;
}

inline float contentWidth(uint32_t node) { return ldf(vcall(node, Self::V_getContentSize, {node})); }
inline float contentHeight(uint32_t node) { return ldf(vcall(node, Self::V_getContentSize, {node}) + 4); }
inline uint32_t widgetAt(const IScrollProtocol *s, uint32_t k) { return ld32(s->items.begin + 4 * k); }
inline int32_t vecCount(const guest::Vector &v, uint32_t shift) {
    return int32_t(uint32_t(v.end) - uint32_t(v.begin)) >> shift;
}
// The inline std::function<void()> destructor of a member
inline void destroyMemberFunction(uint32_t f) { functionDestroy(f); }
inline bool hasFunction(const IScrollProtocol *s, uint32_t off) { return ld32(rw::guest_addr(s) + off + 16) != 0; }

// Position the scroll bar along the edge: horizontal lists put it at y 0, vertical ones at the right edge.
void placeScrollBar(IScrollProtocol *self, int32_t type, uint32_t bar) {
    uint32_t me = self->self();
    if (type) {
        vcall(bar, 0x6C, {bar, 0});                                  // setPositionY(0)
    } else {
        uint32_t setX = ld32(ld32(bar) + 0x64);                      // setPositionX
        float w = contentWidth(me);
        float bw = contentWidth(self->scrollBar);
        call(setX, {bar, fb(w - bw)});
    }
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self::Addr Self::destroy(IScrollProtocol *self) {
    uint32_t me = self->self();
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    if (self->layoutPanel) call(g::Ref_release, {self->layoutPanel});
    call(g::Node_unscheduleUpdate, {me});
    destroyMemberFunction(me + OFF_ON_TOUCH_ENDED);
    destroyMemberFunction(me + OFF_ON_DRAG);
    destroyMemberFunction(me + OFF_ON_SCROLL_END);
    destroyMemberFunction(me + OFF_ON_INDEX_CHANGED);
    call(g::WidgetVector_dtor, {me + 0x5B4});
    call(g::WidgetVector_dtor, {me + 0x5A8});
    call(g::UpdateDataVector_dtor, {me + 0x59C});
    call(g::FloatVector_dtor, {me + 0x580});
    call(g::WidgetVector_dtor, {me + 0x51C});
    return call(g::Layout_dtor, {me});
}
RW_FUNCTION(0x4F57E0, "IScrollProtocol::~IScrollProtocol()", IScrollProtocol::destroy);

void Self::destroyDelete(IScrollProtocol *self) {
    uint32_t p = destroy(self);
    call(fn::operator_delete, {p});
}
RW_FUNCTION(0x4F5A48, "IScrollProtocol::~IScrollProtocol() (deleting)", IScrollProtocol::destroyDelete);

// Touch handling (a one-by-one listener that does not swallow) plus onEnter / onExit callbacks that start and
// stop the per-frame update.
uint32_t Self::init(IScrollProtocol *self) {
    uint32_t me = self->self();
    vcall(me, V_setTouchEnabled, {me, 1});
    self->touching = 0;
    self->position = 0.0f;
    self->index = 0;
    self->field_52C = 10.0f;
    self->field_530 = 32.0f;
    self->field_534 = 4.0f;
    uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_TouchBegan, {me});
        call(g::FunctionTouchBool_swap, {fo, listener + 0x50});
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_TouchMoved, {me, me});
        call(g::FunctionTouchVoid_swap, {fo, listener + 0x68});
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t caps = f.alloc(8);
        st32(caps, me);
        st32(caps + 4, me);
        assignTouchEnded(listener + 0x80, caps);   // onTouchEnded
        assignTouchEnded(listener + 0x98, caps);   // onTouchCancelled
    }
    call(g::EventDispatcher_addListenerWithSceneGraphPriority, {dispatcher, listener, me});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_OnEnter, {me});
        call(g::FunctionVV_assign, {me + 0x210, fo});
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_OnExit, {me});
        call(g::FunctionVV_assign, {me + 0x228, fo});
        functionDestroy(fo);
    }
    return 1;
}
RW_FUNCTION(0x4F5A7C, "IScrollProtocol::init()", IScrollProtocol::init);

// std::function<void(Touch*, Event*)> = $_2 (the captures [this, this] copied from `captures`)
void Self::assignTouchEnded(Addr function, Addr captures) {
    Frame f;
    uint32_t fo = f.alloc(20);
    st32(fo + 16, fo);
    st32(fo, VT_TouchEnded);
    st32(fo + 8, ld32(captures + 4));
    st32(fo + 4, ld32(captures));
    call(g::FunctionTouchVoid_swap, {fo, function});
    functionDestroy(fo);
}
RW_FUNCTION(0x4F5C80, "std::function<void(Touch*,Event*)>::operator=(IScrollProtocol::init()::$_2)",
            IScrollProtocol::assignTouchEnded);

// $_4: onExit -> stop the per-frame update
void Self::onExitLambda(Addr functor) { call(g::Node_unscheduleUpdate, {ld32(functor + 4)}); }
RW_FUNCTION(0x4F859E, "IScrollProtocol::init()::$_4 __func::operator()", IScrollProtocol::onExitLambda);

// $_3: onEnter -> start it
void Self::onEnterLambda(Addr functor) { call(g::Node_scheduleUpdate, {ld32(functor + 4)}); }
RW_FUNCTION(0x4F863A, "IScrollProtocol::init()::$_3 __func::operator()", IScrollProtocol::onEnterLambda);

// $_0: a touch inside the list stops it and starts a possible drag; the scroll bar fades in.
uint32_t Self::onTouchBeganLambda(Addr functor, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    uint32_t me = ld32(functor + 4);
    IScrollProtocol *self = rw::host_ptr<IScrollProtocol>(me);
    if (!self->scrollEnabled) return 0;
    Frame f;
    uint32_t parent = vcall(me, V_getParent, {me});
    uint32_t loc = f.alloc(8), local = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {local, parent, loc});
    call(g::Vec2_dtor, {loc});
    float x = rw::bitsf(vcall(me, 0x68, {me}));                 // getPositionX
    float y = rw::bitsf(vcall(me, 0x70, {me}));                 // getPositionY
    float w = contentWidth(me);
    float h = contentHeight(me);
    call(g::Rect_ctor, {rect, fb(x), fb(y), fb(w), fb(h)});
    uint32_t began = 0;
    if (call(g::Rect_containsPoint, {rect, local})) {
        self->moving = 0;
        self->velocity = 0.0f;
        self->dragged = 0;
        began = 1;
        self->touching = 1;
        uint32_t p = f.alloc(8);
        call(g::Touch_getLocation, {p, touch});
        self->touchStartX = ldf(p);
        call(g::Vec2_dtor, {p});
        call(g::Touch_getLocation, {p, touch});
        self->touchStartY = ldf(p + 4);
        call(g::Vec2_dtor, {p});
        self->lastTouchX = self->touchStartX;
        self->lastTouchY = self->touchStartY;
        self->moveDelta = 0.0f;
        if (self->scrollBarEnabled) {
            call(g::Node_stopAllActions, {self->scrollBar});
            uint32_t bar = self->scrollBar;
            call(g::Node_runAction, {bar, call(g::FadeTo_create, {fb(0.15f), 0xFF})});
        }
        self->bouncing = 0;
    }
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {local});
    return began;
}
RW_FUNCTION(0x4F8F68, "IScrollProtocol::init()::$_0 __func::operator()", IScrollProtocol::onTouchBeganLambda);

// $_1: the finger moved. moveDelta accumulates the move while it keeps its direction; once the drag passed 20
// pixels the list follows the finger (the part past either end at half speed) and onDrag is told.
void Self::onTouchMovedLambda(Addr functor, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    uint32_t me = ld32(functor + 4);
    IScrollProtocol *self = rw::host_ptr<IScrollProtocol>(me);
    IScrollProtocol *limits = rw::host_ptr<IScrollProtocol>(ld32(functor + 8));
    if (!self->scrollEnabled) return;
    Frame f;
    uint32_t p = f.alloc(8);
    float delta;
    if (self->scrollType) {
        call(g::Touch_getLocation, {p, touch});
        float last = self->lastTouchX;
        float x = ldf(p);
        call(g::Vec2_dtor, {p});
        float d = x - last;
        float md = self->moveDelta;
        float acc = 0.0f;
        if (!(d * md < 0.0f)) acc = d + md;
        self->moveDelta = acc;
        call(g::Touch_getLocation, {p, touch});
        self->lastTouchX = ldf(p);
        call(g::Vec2_dtor, {p});
        float start = self->touchStartX;
        call(g::Touch_getLocation, {p, touch});
        float diff = start - ldf(p);
        float mx = getMaxPosition(limits);
        float v = diff + limits->position;
        if (v < 0.0f) delta = diff + v * NEG_HALF;
        else if (mx < v) delta = diff + (v - mx) * NEG_HALF;
        else delta = diff;
        call(g::Vec2_dtor, {p});
        if (self->dragged) {
            vcall(me, V_updateScroll, {me, fb(delta)});
        } else {
            if (std::fabs(delta) < 20.0f) return;
            self->dragged = 1;
            call(g::Touch_getLocation, {p, touch});
            self->touchStartX = ldf(p);
            call(g::Vec2_dtor, {p});
            self->lastTouchX = self->touchStartX;
            return;
        }
    } else {
        call(g::Touch_getLocation, {p, touch});
        float last = self->lastTouchY;
        float y = ldf(p + 4);
        call(g::Vec2_dtor, {p});
        float d = y - last;
        float md = self->moveDelta;
        float acc = 0.0f;
        if (!(d * md < 0.0f)) acc = d + md;
        self->moveDelta = acc;
        call(g::Touch_getLocation, {p, touch});
        self->lastTouchY = ldf(p + 4);
        call(g::Vec2_dtor, {p});
        float start = self->touchStartY;
        call(g::Touch_getLocation, {p, touch});
        float diff = start - ldf(p + 4);
        float negDiff = neg(diff);
        float mx = getMaxPosition(limits);
        float v = limits->position - diff;
        if (v < 0.0f) delta = negDiff + v * NEG_HALF;
        else if (mx < v) delta = negDiff + (v - mx) * NEG_HALF;
        else delta = negDiff;
        call(g::Vec2_dtor, {p});
        if (self->dragged) {
            vcall(me, V_updateScroll, {me, fb(delta)});
        } else {
            if (std::fabs(delta) < 20.0f) return;
            self->dragged = 1;
            call(g::Touch_getLocation, {p, touch});
            self->touchStartY = ldf(p + 4);
            call(g::Vec2_dtor, {p});
            self->lastTouchY = self->touchStartY;
            return;
        }
    }
    if (hasFunction(self, OFF_ON_DRAG)) call(g::FunctionVF_call, {me + OFF_ON_DRAG, fb(delta)});
}
RW_FUNCTION(0x4F8A60, "IScrollProtocol::init()::$_1 __func::operator()", IScrollProtocol::onTouchMovedLambda);

// $_2: the finger was lifted (or the touch cancelled): a drag applies its last step, then the list coasts with
// the accumulated move (at most 80 px per frame); the scroll bar fades out after a second.
void Self::onTouchEndedLambda(Addr functor, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    uint32_t me = ld32(functor + 4);
    IScrollProtocol *self = rw::host_ptr<IScrollProtocol>(me);
    if (!self->scrollEnabled) {
        if (hasFunction(self, OFF_ON_TOUCH_ENDED)) call(g::FunctionVV_call, {me + OFF_ON_TOUCH_ENDED});
        return;
    }
    if (self->dragged) {
        IScrollProtocol *limits = rw::host_ptr<IScrollProtocol>(ld32(functor + 8));
        Frame f;
        uint32_t p = f.alloc(8);
        float step;
        if (self->scrollType) {
            float start = self->touchStartX;
            call(g::Touch_getLocation, {p, touch});
            float d = start - ldf(p);
            float mx = getMaxPosition(limits);
            float v = d + limits->position;
            if (v < 0.0f) step = d + v * NEG_HALF;
            else if (mx < v) step = d + (v - mx) * NEG_HALF;
            else step = d;
        } else {
            float start = self->touchStartY;
            call(g::Touch_getLocation, {p, touch});
            float e = start - ldf(p + 4);
            float negE = neg(e);
            float mx = getMaxPosition(limits);
            float v = limits->position - e;
            if (v < 0.0f) step = negE + v * NEG_HALF;
            else if (mx < v) step = negE + (v - mx) * NEG_HALF;
            else step = negE;
        }
        call(g::Vec2_dtor, {p});
        self->position = step + self->position;
        vcall(me, V_updateScroll, {me, 0});
    }
    self->moving = 1;
    self->touching = 0;
    float md = self->moveDelta;
    self->velocity = md > 80.0f ? 80.0f : md;
    if (self->scrollBarEnabled) {
        call(g::Node_stopAllActions, {self->scrollBar});
        uint32_t bar = self->scrollBar;
        uint32_t delay = call(g::DelayTime_create, {fb(1.0f)});
        uint32_t fade = call(g::FadeTo_create, {fb(0.15f), 0});
        call(g::Node_runAction, {bar, call(g::Sequence_create, {delay, fade, 0})});
    }
    if (hasFunction(self, OFF_ON_TOUCH_ENDED)) call(g::FunctionVV_call, {me + OFF_ON_TOUCH_ENDED});
}
RW_FUNCTION(0x4F86E0, "IScrollProtocol::init()::$_2 __func::operator()", IScrollProtocol::onTouchEndedLambda);

// ------------------------------------------------------------------------------------------------ geometry
// The largest scroll position: the content length minus the view (at least 0) plus maxOffset, but never below
// the minimum position.
float Self::getMaxPosition(IScrollProtocol *self) {
    uint32_t me = self->self();
    int32_t type = self->scrollType;
    float item = type ? self->itemWidth : self->itemHeight;
    int32_t columns = self->maxColumn;
    int32_t count = self->itemCount;
    uint32_t size = vcall(me, V_getContentSize, {me});
    double itemD = double(item + 0.0f);
    float rows = rw::bitsf(call(g::CEILF, {fb(i2f(count) / i2f(columns))}));
    double total = itemD * double(rows);
    float view = ldf(type ? size : size + 4);
    float content = float(total - double(view));
    if (self->variableSize) {
        float sum = 0.0f;
        uint32_t p = self->itemSizes.begin;
        if (uint32_t(self->itemSizes.end) != p) {
            uint32_t n = uint32_t(vecCount(self->itemSizes, 2));
            uint32_t k = 0;
            do {
                sum = sum + ldf(p);
                p += 4;
                k++;
            } while (k < n);
        }
        content = sum - contentHeight(me);
    }
    float clamped = content < 0.0f ? 0.0f : content;
    float hi = self->maxOffset + clamped;
    float lo = 0.0f;
    if (self->scrollType == 0) lo = neg(self->itemHeight + 0.0f);
    lo = lo + self->minOffset;
    return hi < lo ? lo : hi;
}
RW_FUNCTION(0x4F6510, "IScrollProtocol::getMaxPosition()", IScrollProtocol::getMaxPosition);

// How far the list may be pulled past its start: half the first item with variable sizes.
float Self::getStartBouncePixel(IScrollProtocol *self) {
    float v = self->bouncePixel;
    if (self->variableSize && uint32_t(self->itemSizes.end) != uint32_t(self->itemSizes.begin))
        v = ldf(self->itemSizes.begin) * 0.5f;
    return v;
}
RW_FUNCTION(0x4F66BC, "IScrollProtocol::getStartBouncePixel()", IScrollProtocol::getStartBouncePixel);

// ... and past its end: half the last item with variable sizes.
float Self::getBouncePixel(IScrollProtocol *self) {
    float v = self->bouncePixel;
    if (self->variableSize) {
        uint32_t n = uint32_t(vecCount(self->itemSizes, 2));
        if (n) {
            uint32_t last = n - 1;
            if (last >= n) {
                call(g::THROW_OUT_OF_RANGE, {rw::guest_addr(&self->itemSizes)});
                return v;
            }
            v = ldf(self->itemSizes.begin + 4 * last) * 0.5f;
        }
    }
    return v;
}
RW_FUNCTION(0x4F66F4, "IScrollProtocol::getBouncePixel()", IScrollProtocol::getBouncePixel);

float Self::getItemPixel(IScrollProtocol *self) { return self->itemPixel(); }
RW_FUNCTION(0x4F6750, "IScrollProtocol::getItemPixel()", IScrollProtocol::getItemPixel);

float Self::getMinPosition(IScrollProtocol *self) {
    float lo = 0.0f;
    if (self->scrollType == 0) lo = neg(self->itemHeight + 0.0f);
    return lo + self->minOffset;
}
RW_FUNCTION(0x4F7168, "IScrollProtocol::getMinPosition()", IScrollProtocol::getMinPosition);

// A new size rebuilds the row pool: the old row widgets are removed, then (view / item + 3) rows times the
// column count fresh widgets are created, placed in their grid cells and given their touch handlers.
void Self::setContentSize(IScrollProtocol *self, Addr size) {
    uint32_t me = self->self();
    Frame f;
    uint32_t old = f.alloc(8);
    call(g::Size_copy, {old, vcall(me, V_getContentSize, {me})});
    call(g::Widget_setContentSize, {me, size});
    if (ldf(old) == ldf(size) && ldf(old + 4) == ldf(size + 4)) return;
    if (int32_t(uint32_t(self->items.end) - uint32_t(self->items.begin)) >= 1) {
        int32_t k = 0;
        do {
            uint32_t w = widgetAt(self, uint32_t(k));
            if (vcall(w, V_getParent, {w})) vcall(me, V_removeChild, {me, w, 1});
            k++;
        } while (k < vecCount(self->items, 2));
    }
    call(g::WidgetVector_clear, {me + 0x51C});
    call(g::WidgetVector_clear, {me + 0x5A8});
    call(g::WidgetVector_clear, {me + 0x5B4});
    int32_t type = self->scrollType;
    float item = (type ? self->itemWidth : self->itemHeight) + 0.0f;
    float view = ldf(type ? size : size + 4);
    float fl = rw::bitsf(call(g::FLOORF, {fb(view / item)}));
    float rows = float((double(fl) + 3.0) * double(self->maxColumn));
    self->rowCount = rows;
    if (!(rows <= 0.0f) && rows == rows) {
        float half = item * 0.5f;
        uint32_t v = f.alloc(8);
        int32_t k = 0;
        do {
            uint32_t w = call(g::Widget_create, {});
            int32_t columns = self->maxColumn;
            int32_t col = imod(k - 1, columns) + 1;
            int32_t row = idiv(k - 1, columns);
            float x, y;
            if (self->scrollType) {
                float h = self->itemHeight;
                y = i2f(col) * h + h * 0.5f;
                x = item * i2f(row);
                if (columns == 1) y = rw::bitsf(vcall(w, 0x70, {w})) + self->offsetY;   // getPositionY
            } else {
                float wd = self->itemWidth;
                float t = i2f(col) * wd + wd * NEG_HALF;
                float xa = self->offsetX + t;
                y = ((view - item * i2f(row)) - half) + self->offsetY;
                if (columns == 1) x = rw::bitsf(vcall(w, 0x68, {w})) + self->offsetX;   // getPositionX
                else x = xa;
            }
            uint32_t setPos = ld32(ld32(w) + 0x4C);
            call(g::Vec2_ctor, {v, fb(x), fb(y)});
            call(setPos, {w, v});
            call(g::Vec2_dtor, {v});
            call(g::WidgetVector_pushBack, {me + 0x51C, w});
            vcall(me, V_registItemTouchEvent, {me, w, uint32_t(k)});
            k++;
        } while (i2f(k) < self->rowCount);
    }
    if (self->scrollBarEnabled) {
        call(g::ScrollBar_updateLayout, {self->scrollBar});
        placeScrollBar(self, self->scrollType, self->scrollBar);
    }
}
RW_FUNCTION(0x4F6778, "IScrollProtocol::setContentSize(const Size&)", IScrollProtocol::setContentSize);

void Self::registItemTouchEvent(IScrollProtocol *, Addr, int32_t) {}
RW_FUNCTION(0x4F6BC4, "IScrollProtocol::registItemTouchEvent(Widget*, int)", IScrollProtocol::registItemTouchEvent);

// A new item count: the first row is clamped to what can still be scrolled to, then the rows are refreshed
// (a variable-size list with no items just hides every row widget).
void Self::setItemIDSize(IScrollProtocol *self, int32_t count) {
    uint32_t me = self->self();
    self->itemCount = count;
    float mx = getMaxPosition(self);
    float item = self->scrollType ? self->itemWidth : self->itemHeight;
    int32_t px = f2i(item + 0.0f);
    int32_t last = self->maxColumn * idiv(f2i(mx), px);
    uint8_t variable = self->variableSize;
    if (variable) {
        last = vecCount(self->itemSizes, 2);
        if (self->itemCount == 0) {
            float rows = self->rowCount;
            if (rows <= 0.0f || rows != rows) return;
            int32_t k = 0;
            do {
                uint32_t w = widgetAt(self, uint32_t(k));
                vcall(w, V_setVisible, {w, 0});
                k++;
            } while (i2f(k) < self->rowCount);
            return;
        }
    }
    if (last < self->index) {
        self->index = last;
        self->position = getMaxPosition(self);
        variable = self->variableSize;
    }
    if (!variable) updateLayout(self, 0);
    if (self->scrollBarEnabled) call(g::ScrollBar_updateLayout, {self->scrollBar});
}
RW_FUNCTION(0x4F6BC8, "IScrollProtocol::setItemIDSize(int)", IScrollProtocol::setItemIDSize);

// Tell every row widget which item it shows (now, or queued for update() when `deferred`).
void Self::updateLayout(IScrollProtocol *self, uint32_t deferred) {
    uint32_t me = self->self();
    float item = self->scrollType ? self->itemWidth : self->itemHeight;
    int32_t px = f2i(item + 0.0f);
    float pos = self->position;
    int32_t first = self->maxColumn * idiv(f2i(pos), px);
    if (self->variableSize) {
        uint32_t end = self->itemSizes.end, begin = self->itemSizes.begin;
        if (end == begin) {
            first = 0;
        } else {
            uint32_t p = begin;
            int32_t count = 0;
            float sum = 0.0f;
            for (;;) {
                if (!(pos > sum || unord(pos, sum))) {
                    // pos <= sum: the row before the one that starts past the position
                    bool back = count > 0 && pos < sum;
                    first = back ? count - 1 : count;
                    break;
                }
                sum = sum + ldf(p);
                p += 4;
                count++;
                if (!(uint32_t(count) < uint32_t(int32_t(end - begin) >> 2))) {
                    first = count;
                    break;
                }
            }
        }
    }
    float rows = self->rowCount;
    if (rows <= 0.0f || rows != rows) return;
    Frame f;
    uint32_t data = f.alloc(8);
    int32_t k = 0;
    do {
        if (deferred == 1) {
            st32(data, uint32_t(k));
            st32(data + 4, uint32_t(first));
            uint32_t end = self->updates.end;
            if (end >= uint32_t(self->updates.cap)) {
                call(g::UpdateData_pushBackSlow, {me + 0x59C, data});
            } else {
                uint32_t n = 0;
                if (end) {
                    st32(end, ld32(data));
                    st32(end + 4, ld32(data + 4));
                    n = self->updates.end;
                }
                self->updates.end = n + 8;
            }
        } else {
            vcall(me, V_updateRow, {me, uint32_t(k), uint32_t(first)});
        }
        k++;
    } while (i2f(k) < self->rowCount);
}
RW_FUNCTION(0x4F6CF4, "IScrollProtocol::updateLayout(bool)", IScrollProtocol::updateLayout);

// Row `row` shows item first + row - 1 (1-based first + row). Out of range rows are hidden; a widget the
// subclass wants replaced (isReplaceItem) is swapped for a fresh createItem() at the same position (the old one
// is removed in update()).
void Self::updateRow(IScrollProtocol *self, int32_t row, int32_t first) {
    uint32_t me = self->self();
    int32_t n = first + row;
    int32_t item = n - 1;
    uint32_t w = widgetAt(self, uint32_t(row));
    if (n < 1 || n > self->itemCount) {
        vcall(w, V_setVisible, {w, 0});
        return;
    }
    uint32_t shown = w;
    if (vcall(me, V_isReplaceItem, {me, w, uint32_t(item), 0}) == 1) {
        if (row == 0) return;
        uint32_t nw = vcall(me, V_createItem, {me, uint32_t(item), 0});
        call(g::WidgetVector_pushBack, {me + 0x5A8, nw});
        call(g::WidgetVector_pushBack, {me + 0x5B4, w});
        call(g::Ref_release, {ld32(self->items.begin + 4 * uint32_t(row))});
        st32(self->items.begin + 4 * uint32_t(row), nw);
        call(g::Ref_retain, {nw});
        uint32_t setPos = ld32(ld32(nw) + 0x4C);
        call(setPos, {nw, vcall(w, 0x54, {w})});   // setPosition(old->getPosition())
        vcall(me, V_registItemTouchEvent, {me, nw, uint32_t(row)});
        shown = nw;
    }
    vcall(shown, V_setVisible, {shown, 1});
}
RW_FUNCTION(0x4F6EE0, "IScrollProtocol::updateRow(int, int)", IScrollProtocol::updateRow);

void Self::setEnabledScrollBar(IScrollProtocol *self, uint32_t enabled) {
    uint32_t me = self->self();
    if (uint32_t(self->scrollBarEnabled) == enabled) return;
    self->scrollBarEnabled = uint8_t(enabled);
    if (enabled) {
        self->scrollBar = call(g::ScrollBar_create, {me});
        call(g::ScrollBar_setBackgroundVisible, {self->scrollBar, 0});
        vcall(self->scrollBar, V_setOpacity, {self->scrollBar, 0});
        int32_t type = self->scrollType;
        placeScrollBar(self, type, self->scrollBar);
        call(g::SceneUtil_setCascadeOpacityEnabled, {self->scrollBar, 1});
        vcall(me, V_addChildZ, {me, self->scrollBar, 0x7FFFFFFF});
    } else {
        vcall(me, V_removeChild, {me, self->scrollBar, 1});
        self->scrollBar = 0;
    }
}
RW_FUNCTION(0x4F6FE8, "IScrollProtocol::setEnabledScrollBar(bool)", IScrollProtocol::setEnabledScrollBar);

void Self::setScrollType(IScrollProtocol *self, int32_t type) {
    self->scrollType = type;
    uint32_t bar = self->scrollBar;
    if (!bar) return;
    if (self->scrollBarEnabled) placeScrollBar(self, type, bar);
    call(g::ScrollBar_updateLayout, {self->scrollBar});
}
RW_FUNCTION(0x4F70B4, "IScrollProtocol::setScrollType(ScrollType)", IScrollProtocol::setScrollType);

void Self::setVisibleWidgets(IScrollProtocol *self, uint32_t visible) {
    if (int32_t(uint32_t(self->items.end) - uint32_t(self->items.begin)) < 1) return;
    int32_t k = 0;
    do {
        uint32_t w = widgetAt(self, uint32_t(k));
        vcall(w, V_setVisible, {w, visible});
        k++;
    } while (k < vecCount(self->items, 2));
}
RW_FUNCTION(0x4F711C, "IScrollProtocol::setVisibleWidgets(bool)", IScrollProtocol::setVisibleWidgets);

IScrollProtocol *Self::setMaxColumn(IScrollProtocol *self, int32_t columns) {
    self->maxColumn = columns;
    return self;
}
RW_FUNCTION(0x4F719C, "IScrollProtocol::setMaxColumn(int)", IScrollProtocol::setMaxColumn);

// Scroll to item `index` (its row; a variable-size list sums the sizes before it and keeps the view inside).
void Self::setIndex(IScrollProtocol *self, int32_t index) {
    uint32_t me = self->self();
    float item = self->itemPixel();
    int32_t row = idiv(index, self->maxColumn);
    self->position = i2f(row) * item;
    float mx = getMaxPosition(self);
    float pos = self->position;
    if (!(mx >= pos || unord(mx, pos))) self->position = getMaxPosition(self);
    self->index = row;
    if (self->variableSize) {
        self->index = -2;
        self->position = 0.0f;
        uint32_t p = self->itemSizes.begin;
        int32_t n = vecCount(self->itemSizes, 2);
        int32_t lim = n < index ? n : index;
        float sum = 0.0f;
        if (lim >= 1) {
            int32_t k = 0;
            do {
                if (uint32_t(n) <= uint32_t(k)) {
                    call(g::THROW_OUT_OF_RANGE, {rw::guest_addr(&self->itemSizes)});
                    return;
                }
                sum = ldf(p) + sum;
                self->position = sum;
                p += 4;
                k++;
            } while (k < lim);
        }
        float h = contentHeight(me);
        if (sum < h || unord(sum, h)) self->position = 0.0f;
        else self->position = self->position - contentHeight(me);
    }
    vcall(me, V_updateScroll, {me, 0});
}
RW_FUNCTION(0x4F71A4, "IScrollProtocol::setIndex(int)", IScrollProtocol::setIndex);

void Self::setIndexPrev(IScrollProtocol *self, int32_t index, int32_t prev) {
    uint32_t me = self->self();
    float item = self->itemPixel();
    self->position = i2f(idiv(index, self->maxColumn)) * item;
    float mx = getMaxPosition(self);
    float pos = self->position;
    if (!(mx >= pos || unord(mx, pos))) self->position = getMaxPosition(self);
    self->index = prev;
    vcall(me, V_updateScroll, {me, 0});
}
RW_FUNCTION(0x4F731C, "IScrollProtocol::setIndexPrev(int, int)", IScrollProtocol::setIndexPrev);

IScrollProtocol *Self::setEnabledScroll(IScrollProtocol *self, uint32_t enabled) {
    self->scrollEnabled = uint8_t(enabled);
    self->moving = 0;
    self->velocity = 0.0f;
    self->dragged = 0;
    return self;
}
RW_FUNCTION(0x4F73C8, "IScrollProtocol::setEnabledScroll(bool)", IScrollProtocol::setEnabledScroll);

IScrollProtocol *Self::resetScroll(IScrollProtocol *self) {
    self->moving = 0;
    self->velocity = 0.0f;
    self->dragged = 0;
    return self;
}
RW_FUNCTION(0x4F73EC, "IScrollProtocol::resetScroll()", IScrollProtocol::resetScroll);

// ------------------------------------------------------------------------------------------------ scrolling
// Move by `delta` (clamped to the bounce range), work out the first visible row and the pixel remainder, and
// lay the row widgets out from there; rows are re-assigned (updateRow) when the first row changed.
void Self::updateScroll(IScrollProtocol *self, float delta) {
    uint32_t me = self->self();
    if (!self->scrollEnabled || !self->itemCount) return;
    float item = self->itemPixel();
    uint8_t variable = 0;
    float mx = getMaxPosition(self);
    float next = self->position + delta;
    float bp = getBouncePixel(self);
    float pos;
    if (self->variableSize) {
        uint32_t size = vcall(me, V_getContentSize, {me});
        float start = self->bouncePixel;
        float h = ldf(size + 4);
        variable = self->variableSize;
        if (variable && uint32_t(self->itemSizes.end) != uint32_t(self->itemSizes.begin))
            start = ldf(self->itemSizes.begin) * 0.5f;
        float lo = neg(start);
        if (!(next >= lo || unord(next, lo))) {
            self->position = lo;
            pos = lo;
        } else {
            float hi = mx + (bp + h);
            if (!(hi >= next || unord(hi, next))) {
                self->position = hi;
                pos = hi;
            } else {
                pos = next;
            }
        }
    } else {
        float lo = neg(bp);
        if (!(next >= lo || unord(next, lo))) {
            self->position = lo;
            pos = lo;
        } else {
            float hi = mx + bp;
            if (!(hi >= next || unord(hi, next))) {
                self->position = hi;
                pos = hi;
            } else {
                pos = next;
            }
        }
    }
    int32_t ipos = f2i(pos);
    int32_t ipx = f2i(item);
    int32_t first = self->maxColumn * idiv(ipos, ipx);
    int32_t rem = imod(ipos, ipx);
    if (variable) {
        // the item containing the position and the pixels into it
        uint32_t begin = self->itemSizes.begin, end = self->itemSizes.end;
        first = 0;
        float remaining = pos;
        uint32_t sizesAddr = rw::guest_addr(&self->itemSizes);
        if (pos >= 0.0f) {
            uint32_t n = uint32_t(int32_t(end - begin) >> 2);
            if (n != 0) {
                uint32_t p = begin;
                uint32_t k = 0;
                float sum = 0.0f;
                for (;;) {
                    float size = ldf(p);
                    float nextSum = sum + size;
                    if (!(sum > pos || unord(sum, pos)) && !(pos >= nextSum || unord(pos, nextSum))) {
                        // sum <= pos < nextSum: inside item k
                        if (!(n > k)) {
                            call(g::THROW_OUT_OF_RANGE, {sizesAddr});
                            return;
                        }
                        rem = imod(f2i(remaining), f2i(size));
                        item = size;
                        first = int32_t(k);
                        break;
                    }
                    remaining = remaining - size;
                    p += 4;
                    k++;
                    sum = nextSum;
                    if (!(k < n)) {
                        first = int32_t(k);
                        break;
                    }
                }
            }
        }
        int32_t n = int32_t(int32_t(end - begin) >> 2);
        if (!(first < n)) {
            uint32_t last = uint32_t(n) - 1;
            if (!(last < uint32_t(n))) {
                call(g::THROW_OUT_OF_RANGE, {sizesAddr});
                return;
            }
            first = int32_t(last);
            float size = ldf(begin + 4 * last);
            rem = imod(f2i(remaining + size), f2i(size));
        }
        self->offsetY = 0.0f;
    }
    float clampPos = pos;
    int32_t oldIndex = self->index;
    self->index = first;
    if (oldIndex != first && hasFunction(self, OFF_ON_INDEX_CHANGED))
        call(g::FunctionVI_call, {me + OFF_ON_INDEX_CHANGED, uint32_t(first + 1)});
    float rows = self->rowCount;
    if (!(rows <= 0.0f || unord(rows, 0.0f))) {
        int32_t firstClamped = first < 0 ? 0 : first;
        float halfItem = item * 0.5f;
        float remF = i2f(rem);
        float xa = 0.0f;
        int32_t k = 0;
        Frame f;
        uint32_t v = f.alloc(8);
        do {
            int32_t columns = self->maxColumn;
            int32_t col = imod(k - 1, columns) + 1;
            uint32_t w = widgetAt(self, uint32_t(k));
            int32_t row = idiv(k - 1, columns);
            float x, y;
            if (self->scrollType) {
                float h = self->itemHeight;
                y = i2f(col) * h + h * 0.5f;
                x = item * i2f(row) - remF;
                if (columns == 1) y = rw::bitsf(vcall(w, 0x70, {w}));   // getPositionY
            } else {
                float wd = self->itemWidth;
                float t = i2f(col) * wd + wd * NEG_HALF;
                xa = self->offsetX + t;
                uint32_t size = vcall(me, V_getContentSize, {me});
                float mp = item * i2f(row);
                y = remF + (self->offsetY + ((ldf(size + 4) - mp) - halfItem));
                if (first >= 0 && self->variableSize) {
                    y = (remF + contentHeight(me)) + self->offsetY;
                    uint32_t sizes = self->itemSizes.begin;
                    int32_t n = vecCount(self->itemSizes, 2);
                    uint32_t sizesAddr = rw::guest_addr(&self->itemSizes);
                    if (firstClamped < n) {
                        if (!(uint32_t(n) > uint32_t(firstClamped))) {
                            call(g::THROW_OUT_OF_RANGE, {sizesAddr});
                            return;
                        }
                        y = y + ldf(sizes + 4 * uint32_t(firstClamped)) * NEG_HALF;
                    }
                    if (k >= 1) {
                        if (firstClamped < n) {
                            if (!(uint32_t(n) > uint32_t(firstClamped))) {
                                call(g::THROW_OUT_OF_RANGE, {sizesAddr});
                                return;
                            }
                            y = y + ldf(sizes + 4 * uint32_t(firstClamped)) * NEG_HALF;
                        }
                        int32_t stop = k + firstClamped;
                        for (int32_t j = firstClamped + 1; j < stop; j++) {
                            if (j >= n) continue;
                            if (!(uint32_t(n) > uint32_t(j))) {
                                call(g::THROW_OUT_OF_RANGE, {sizesAddr});
                                return;
                            }
                            y = y - ldf(sizes + 4 * uint32_t(j));
                        }
                        if (stop < n) {
                            if (!(uint32_t(n) > uint32_t(stop))) {
                                call(g::THROW_OUT_OF_RANGE, {sizesAddr});
                                return;
                            }
                            y = y + ldf(sizes + 4 * uint32_t(stop)) * 0.5f;
                        } else if (stop == n) {
                            uint32_t last = uint32_t(n) - 1;
                            if (!(last < uint32_t(n))) {
                                call(g::THROW_OUT_OF_RANGE, {sizesAddr});
                                return;
                            }
                            y = y + ldf(sizes + 4 * last) * 0.5f;
                        }
                    }
                }
                if (self->maxColumn == 1) x = rw::bitsf(vcall(w, 0x68, {w}));   // getPositionX
                else x = xa;
            }
            uint32_t setPos = ld32(ld32(w) + 0x4C);
            call(g::Vec2_ctor, {v, fb(x), fb(y)});
            call(setPos, {w, v});
            call(g::Vec2_dtor, {v});
            if (oldIndex != first) vcall(me, V_updateRow, {me, uint32_t(k), uint32_t(first)});
            k++;
        } while (i2f(k) < self->rowCount);
    }
    if (self->scrollBarEnabled) call(g::ScrollBar_setScrollPosition, {self->scrollBar, fb(clampPos)});
}
RW_FUNCTION(0x4F7408, "IScrollProtocol::updateScroll(float)", IScrollProtocol::updateScroll);

// The item (variable sizes) or first item of the row at the scroll position; -1 before the start, -2 past the end.
int32_t Self::getScrollIndex(IScrollProtocol *self) {
    float pos = self->position;
    if (self->variableSize) {
        if (pos < 0.0f) return -1;
        if (pos > getMaxPosition(self)) return -2;
        float p2 = self->position;
        int32_t k = 0;
        uint32_t end = self->itemSizes.end, begin = self->itemSizes.begin;
        if (!(p2 < 0.0f || unord(p2, 0.0f)) && end != begin) {
            uint32_t p = begin;
            float sum = 0.0f;
            for (;;) {
                bool past = sum > p2 || unord(sum, p2);
                sum = sum + ldf(p);
                if (!past && p2 < sum) break;
                p += 4;
                k++;
                if (!(uint32_t(k) < uint32_t(int32_t(end - begin) >> 2))) break;
            }
        }
        int32_t n = int32_t(end - begin) >> 2;
        return k < n ? k : n - 1;
    }
    if (pos < 0.0f) return -1;
    if (pos > getMaxPosition(self)) return -2;
    float item = self->scrollType ? self->itemWidth : self->itemHeight;
    getMaxPosition(self);
    float p = self->position;
    getBouncePixel(self);
    int32_t px = f2i(item + 0.0f);
    return self->maxColumn * idiv(f2i(p), px);
}
RW_FUNCTION(0x4F7B78, "IScrollProtocol::getScrollIndex()", IScrollProtocol::getScrollIndex);

uint32_t Self::containsPointInScrollArea(IScrollProtocol *self, Addr point) {
    uint32_t me = self->self();
    Frame f;
    uint32_t rect = f.alloc(16), local = f.alloc(8);
    float w = contentWidth(me);
    float h = contentHeight(me);
    call(g::Rect_ctor, {rect, 0, 0, fb(w), fb(h)});
    call(g::Node_convertToNodeSpace, {local, me, point});
    uint32_t inside = call(g::Rect_containsPoint, {rect, local});
    call(g::Vec2_dtor, {local});
    call(g::Vec2_dtor, {rect});
    return inside;
}
RW_FUNCTION(0x4F7D20, "IScrollProtocol::containsPointInScrollArea(const Vec2&)", IScrollProtocol::containsPointInScrollArea);

// Item number of row widget `local` (1-based) at the current position, 0-based.
int32_t Self::getItemIndex(IScrollProtocol *self, int32_t local) {
    float item = self->scrollType ? self->itemWidth : self->itemHeight;
    int32_t px = f2i(item + 0.0f);
    int32_t first = self->maxColumn * idiv(f2i(self->position), px);
    return int32_t(uint32_t(local) + uint32_t(first) - 1u);
}
RW_FUNCTION(0x4F7DE0, "IScrollProtocol::getItemIndex(int)", IScrollProtocol::getItemIndex);

// The row widget under a point (world space), or -1.
int32_t Self::getLocalItemIndex(IScrollProtocol *self, Addr point) {
    int32_t found = -1;
    if (int32_t(uint32_t(self->items.end) - uint32_t(self->items.begin)) < 1) return found;
    Frame f;
    uint32_t local = f.alloc(8), rect = f.alloc(16);
    int32_t prev = -1;
    int32_t k = 0;
    for (;;) {
        uint32_t w = widgetAt(self, uint32_t(k));
        call(g::Node_convertToNodeSpace, {local, w, point});
        float ww = contentWidth(w);
        float wh = contentHeight(w);
        call(g::Rect_ctor, {rect, 0, 0, fb(ww), fb(wh)});
        uint32_t inside = call(g::Rect_containsPoint, {rect, local});
        call(g::Vec2_dtor, {rect});
        call(g::Vec2_dtor, {local});
        found = inside ? k : prev;
        k++;
        if (inside) break;
        prev = found;
        if (!(k < vecCount(self->items, 2))) break;
    }
    return found;
}
RW_FUNCTION(0x4F7E38, "IScrollProtocol::getLocalItemIndex(const Vec2&)", IScrollProtocol::getLocalItemIndex);

// A row widget made from the template panel: a centred copy inside an item-sized widget.
Self::Addr Self::getLayoutPanel(IScrollProtocol *self) {
    if (!self->layoutPanel) return 0;
    uint32_t w = call(g::Widget_create, {});
    uint32_t panel = call(g::Widget_clone, {self->layoutPanel});
    vcall(w, 0x9C, {w, g::ANCHOR_MIDDLE});                  // setAnchorPoint
    vcall(w, 0xF0, {w, panel});                             // addChild
    uint32_t setSize = ld32(ld32(w) + 0xA8);                // setContentSize
    Frame f;
    uint32_t size = f.alloc(8);
    call(g::Size_ctor, {size, fb(self->itemWidth), fb(self->itemHeight)});
    call(setSize, {w, size});
    vcall(w, 0x64, {w, fb(self->itemWidth * 0.5f)});        // setPositionX
    return w;
}
RW_FUNCTION(0x4F7F40, "IScrollProtocol::getLayoutPanel()", IScrollProtocol::getLayoutPanel);

// Start a bounce that centres the item `step` items away from the current one; false when that item is
// outside the list (the target is clamped).
uint32_t Self::migrationBounceCenterPosition(IScrollProtocol *self, int32_t step) {
    int32_t type = self->scrollType;
    float lo = 0.0f;
    if (type == 0) lo = neg(self->itemHeight + 0.0f);
    int32_t count = self->itemCount;
    float pos = self->position;
    float item = type ? self->itemWidth : self->itemHeight;
    float minOff = self->minOffset;
    self->bouncing = 1;
    self->centering = 1;
    float base = lo + minOff;
    float itemPx = item + 0.0f;
    float t = (pos - base) + itemPx * 0.5f;
    int32_t target = idiv(f2i(t), f2i(itemPx)) + step;
    int32_t last = count - 1;
    int32_t clamped = target < last ? target : last;
    float fc = i2f(clamped);
    float rows = clamped < 0 ? 0.0f : fc;
    float dest = base + itemPx * rows;
    self->targetPosition = dest;
    uint32_t inside = (target < last ? 1u : 0u) & (target < 0 ? 0u : 1u);
    if (type == 1) {
        float mx = getMaxPosition(self);
        dest = self->targetPosition;
        if (!(dest <= mx || unord(dest, mx))) {
            self->targetPosition = mx;
            dest = mx;
        }
    }
    self->bounceDistance = self->position - dest;
    self->bounceTime = 0.0f;
    return inside;
}
RW_FUNCTION(0x4F801C, "IScrollProtocol::migrationBounceCenterPosition(int)", IScrollProtocol::migrationBounceCenterPosition);

uint32_t Self::migrationLeft(IScrollProtocol *self, int32_t step) {
    return migrationBounceCenterPosition(self, step >= 1 ? int32_t(0u - uint32_t(step)) : -1);
}
RW_FUNCTION(0x4F81B8, "IScrollProtocol::migrationLeft(int)", IScrollProtocol::migrationLeft);

uint32_t Self::migrationRight(IScrollProtocol *self, int32_t step) {
    return migrationBounceCenterPosition(self, step >= 1 ? step : 1);
}
RW_FUNCTION(0x4F81D2, "IScrollProtocol::migrationRight(int)", IScrollProtocol::migrationRight);

// Clamp a position into the bounce range (and scroll there).
void Self::checkScrollInterval(IScrollProtocol *self, const float *pos) {
    float lo0 = 0.0f;
    if (self->scrollType == 0) lo0 = neg(self->itemHeight + 0.0f);
    float minOff = self->minOffset;
    float mx = getMaxPosition(self);
    float lo = lo0 + minOff;
    float bp = getBouncePixel(self);
    lo = lo - bp;
    float v = *pos;
    if (!(v >= lo || unord(v, lo))) {
        rw::save<float>(rw::guest_addr(pos), lo);
        self->position = lo;
        return;
    }
    float hi = mx + bp;
    if (!(hi >= v || unord(hi, v))) {
        rw::save<float>(rw::guest_addr(pos), hi);
        self->position = hi;
    }
}
RW_FUNCTION(0x4F81E8, "IScrollProtocol::checkScrollInterval(float&)", IScrollProtocol::checkScrollInterval);

// Per frame: apply queued row updates and widget replacements, animate a bounce back into range, and let a
// released list coast (velocity decays by `deceleration`, faster past either end) until it stops - then report
// the item it stopped on (onScrollEnd) and start the bounce if it ended out of range.
void Self::updatePending(IScrollProtocol *self) {
    uint32_t me = self->self();
    // ---- queued row updates (updateLayout(true)): only rows still showing the same first row are refreshed
    uint32_t begin = self->updates.begin, end = self->updates.end;
    if (end != begin) {
        if (self->updateOnePerFrame) {
            if (self->index == int32_t(ld32(begin + 4))) {
                vcall(me, V_updateRow, {me, ld32(begin)});
                end = self->updates.end;
                begin = self->updates.begin;
            }
            uint32_t tail = end - (begin + 8);
            call(g::MEMMOVE, {begin, begin + 8, tail});
            uint32_t newEnd = begin + (tail & ~7u);
            uint32_t cur = self->updates.end;
            if (cur != newEnd) self->updates.end = cur + (~7u & ~((cur - 8) - newEnd));
        } else {
            uint32_t k = 0;
            do {
                if (self->index == int32_t(ld32(begin + 8 * k + 4))) {
                    vcall(me, V_updateRow, {me, ld32(begin + 8 * k)});
                    begin = self->updates.begin;
                    end = self->updates.end;
                }
                k++;
            } while (k < uint32_t(int32_t(end - begin) >> 3));
            if (end != begin) self->updates.end = end + (~7u & ~((end - 8) - begin));
        }
    }
    // ---- widget replacements (updateRow): add the new widgets, remove the old ones
    if (int32_t(uint32_t(self->pendingNew.end) - uint32_t(self->pendingNew.begin)) >= 1) {
        if (self->updateOnePerFrame) {
            uint32_t old = ld32(self->pendingOld.begin);
            vcall(me, V_addChild, {me, ld32(self->pendingNew.begin)});
            vcall(me, V_removeChild, {me, old, 1});
            call(g::WidgetVector_erase, {me + 0x5A8, self->pendingNew.begin});
            call(g::WidgetVector_erase, {me + 0x5B4, self->pendingOld.begin});
        } else {
            int32_t k = 0;
            do {
                uint32_t old = ld32(self->pendingOld.begin + 4 * uint32_t(k));
                vcall(me, V_addChild, {me, ld32(self->pendingNew.begin + 4 * uint32_t(k))});
                vcall(me, V_removeChild, {me, old, 1});
                k++;
            } while (k < vecCount(self->pendingNew, 2));
            call(g::WidgetVector_clear, {me + 0x5A8});
            call(g::WidgetVector_clear, {me + 0x5B4});
        }
    }
}

void Self::update(IScrollProtocol *self, float dt) {
    uint32_t me = self->self();
    updatePending(self);
    // ---- bounce: position eases back by distance * (-sqrt(t / 0.3)) until it reaches the end
    float mx = getMaxPosition(self);
    if (self->bouncing) {
        self->bounceTime = self->bounceTime + dt;
        float ratio = self->bounceTime / 0.3f;
        float dist = self->bounceDistance;
        double s = callD(g::FABS, callD(g::SQRT, double(ratio)));
        bool minusInf = ratio == rw::bitsf(0xFF800000u);
        double pw = minusInf ? rw::bitsf(0x7F800000u) : s;
        double move = double(-0.0f - dist) * pw;
        float pos = self->position;
        if (!(pos >= 0.0f || unord(pos, 0.0f))) {
            double u = move - double(pos - dist);
            self->position = float(double(pos) + u);
            double c = double(-0.0f - dist);
            if (!(c > move || c != c || move != move)) {
                self->bouncing = 0;
                self->position = 0.0f;
            }
        } else {
            double b = double(pos - (mx + dist));
            double u = b - move;
            self->position = float(double(pos) - u);
            double v = -0.0 - move;
            double dd = double(dist);
            if (!(dd > v || dd != dd || v != v)) {
                self->bouncing = 0;
                self->position = mx;
            }
        }
        vcall(me, V_updateScroll, {me, 0});
    }
    // ---- a finger on the list: the accumulated move fades
    if (self->touching) {
        self->moveDelta = self->moveDelta * 0.5f;
        return;
    }
    float v = self->velocity;
    if (!self->dragged || v == 0.0f) {
        // stopped
        self->dragged = 0;
        if (!self->moving) return;
        self->moving = 0;
        if (hasFunction(self, OFF_ON_SCROLL_END)) {
            float pos = self->position;
            int32_t item = 0;
            if (!(pos > 0.0f)) {
                float px = self->scrollType ? self->itemWidth : self->itemHeight;
                int32_t ipx = f2i(px + 0.0f);
                item = int32_t(1u - uint32_t(idiv(f2i(pos), ipx)));
            }
            call(g::FunctionVI_call, {me + OFF_ON_SCROLL_END, uint32_t(item)});
        }
        float pos = self->position;
        if (!(pos >= 0.0f || unord(pos, 0.0f))) {
            self->bouncing = 1;
            self->bounceDistance = pos;
        } else {
            if (mx >= pos || unord(mx, pos)) return;
            self->bouncing = 1;
            self->bounceDistance = pos - mx;
        }
        self->bounceTime = 0.0f;
        return;
    }
    // ---- coasting
    float dec = v * self->deceleration;
    bool stop;
    if (!(v <= 0.0f || unord(v, 0.0f))) {
        // moving forward (position grows)
        float pos = self->position;
        if (!(mx >= pos || unord(mx, pos))) dec = (pos - mx) * self->bounceDeceleration;
        float nv = v - dec;
        self->velocity = nv;
        if (self->variableSize) {
            if (nv < 0.0f) {
                stop = true;
            } else {
                float start = self->bouncePixel;
                if (uint32_t(self->itemSizes.end) != uint32_t(self->itemSizes.begin))
                    start = ldf(self->itemSizes.begin) * 0.5f;
                float lo = neg(start);
                stop = !(pos > lo || unord(pos, lo));
            }
        } else {
            if (nv < 0.0f) {
                stop = true;
            } else {
                float lo = neg(getBouncePixel(self));
                stop = !(pos > lo || unord(pos, lo));
            }
        }
    } else {
        float pos = self->position;
        if (!(pos >= 0.0f || unord(pos, 0.0f))) dec = pos * self->bounceDeceleration;
        float nv = v - dec;
        self->velocity = nv;
        if (nv > 0.0f) {
            stop = true;
        } else {
            float hi = mx + getBouncePixel(self);
            float now = self->position;
            stop = !(hi > now || unord(hi, now));
        }
    }
    if (stop) self->velocity = 0.0f;
    float nv = self->velocity;
    float a = std::fabs(nv);
    if (!(a >= 1.0f || unord(a, 1.0f))) {
        nv = 0.0f;
        self->velocity = 0.0f;
    }
    if (!self->moving) return;
    if (nv == 0.0f) return;
    if (self->scrollType) self->position = self->position - nv;
    else self->position = nv + self->position;
    vcall(me, V_updateScroll, {me, 0});
}
RW_FUNCTION(0x4F5CFC, "IScrollProtocol::update(float)", IScrollProtocol::update);
