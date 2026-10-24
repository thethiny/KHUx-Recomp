// BaseSlideView: life cycle, paging, touch lambdas.
#include "BaseSlideView.hpp"

using namespace w4b1;
using Self = BaseSlideView;

namespace {
constexpr uint32_t VT_init_0 = 0xF57154;                 // init::$_0 (onTouchBegan)
constexpr uint32_t VT_init_1 = 0xF57128;                 // init::$_1 (onTouchMoved)
constexpr uint32_t VT_init_2 = 0xF570D0;                 // init::$_2 (onTouchEnded / onTouchCancelled)
constexpr uint32_t VT_onCompleteAnimation_3 = 0xF570A4;
constexpr uint32_t VT_runLeftSlide_4 = 0xF57078;
constexpr uint32_t VT_runRightSlide_5 = 0xF5704C;
constexpr uint32_t VT_init_6 = 0xF570FC;

constexpr uint32_t FunctionWidget_call = 0x525D59;       // std::function<Widget*(Widget*, int, bool)>::operator()
constexpr uint32_t UpdateTargetVector_baseDtor = 0x525831;
constexpr uint32_t UpdateTargetVector_pushBackSlow = 0x526819;
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;
constexpr uint32_t Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Node_scheduleUpdate = rwlib::cocos2d::Node::scheduleUpdate;
constexpr uint32_t Node_unscheduleUpdate = rwlib::cocos2d::Node::unscheduleUpdate;
constexpr uint32_t Layer_create = rwlib::cocos2d::Layer::create;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t ListenerTouch_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t ListenerTouch_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addSceneGraph = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;

constexpr uint32_t LISTENER_ON_TOUCH_BEGAN = 0x50;
constexpr uint32_t LISTENER_ON_TOUCH_MOVED = 0x68;
constexpr uint32_t LISTENER_ON_TOUCH_ENDED = 0x80;
constexpr uint32_t LISTENER_ON_TOUCH_CANCELLED = 0x98;

inline Self *S(uint32_t a) { return rw::host_ptr<Self>(a); }
inline uint32_t A(const uint8_t *p) { return rw::guest_addr(p); }

constexpr uint32_t SIGN = 0x80000000u;
inline float negf(float f) { return bf(fb(f) ^ SIGN); }

// The drag distance clamped to one screen width (the original's getMoveX, inlined in the touch lambdas)
float clampMove(float d, float width) {
    float ad = bf(fb(d) & 0x7FFFFFFFu);
    if (width >= ad || width != width || ad != ad) return d;
    if (d > 0.0f) return width;
    return negf(width);
}

// libc++ push_back of a 4-byte value when there is room (the null check of placement new included)
inline void pushBackInPlace(uint32_t vec, uint32_t value) {
    uint32_t e = ld32(vec + 4);
    uint32_t v = 0;
    if (e) {
        st32(e, value);
        v = ld32(vec + 4);
    }
    st32(vec + 4, v + 4);
}

// widget = createItem(widget, id, flag); add it to the container when it has no parent yet
uint32_t makePage(uint32_t self, uint32_t nodeField, uint32_t id, uint32_t flag) {
    Self *s = S(self);
    uint32_t node = call(FunctionWidget_call, {A(s->createItem), ld32(nodeField), id, flag});
    st32(nodeField, node);
    return vcall(node, node::getParent, {node});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
BaseSlideView::Addr BaseSlideView::create() {
    uint32_t self = call(fn::operator_new, {SIZE});
    call(wl::Layout_ctor, {self});
    Self *s = S(self);
    s->vtable = VTABLE;
    s->vtable2 = VTABLE2;
    s->vtable3 = VTABLE3;
    rw::set<uint32_t>(s, 0x510, 0);          // the std::function members start empty
    rw::set<uint32_t>(s, 0x528, 0);
    rw::set<uint32_t>(s, 0x540, 0);
    rw::set<uint32_t>(s, 0x558, 0);
    rw::set<uint32_t>(s, 0x570, 0);
    call(wl::MEMSET, {self + 0x578, 0x4A, 0});   // itemIDs .. touching
    s->slideEnabled = 1;
    s->listener = 0;
    s->dragging = 0;
    s->updated = 0;
    // init() through the vtable entry the compiler read statically
    if (call(ld32(VTABLE_BASE + 0x250), {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, vslot::release, {self});
    return 0;
}
RW_FUNCTION(0x534E78, "BaseSlideView::create", BaseSlideView::create);

BaseSlideView::Addr BaseSlideView::destroy(Addr self) {
    Self *s = S(self);
    s->vtable = VTABLE;
    s->vtable2 = VTABLE2;
    s->vtable3 = VTABLE3;
    call(Node_unscheduleUpdate, {self});
    call(wl::IntVector_baseDtor, {self + 0x5A0});
    call(UpdateTargetVector_baseDtor, {self + 0x594});
    call(wl::IntVector_baseDtor, {self + 0x578});
    functionDestroy(A(s->onTouchEnd));
    functionDestroy(A(s->onMove));
    functionDestroy(A(s->onSlideStart));
    functionDestroy(A(s->createItem));
    functionDestroy(A(s->fn_500));
    call(wl::Layout_dtor, {self});
    return self;
}
RW_FUNCTION(0x525638, "BaseSlideView::~BaseSlideView", BaseSlideView::destroy);

// Touch handling: began / moved through $_0 / $_1, ended and cancelled both through $_2.
uint32_t BaseSlideView::init(Addr self) {
    Self *s = S(self);
    s->touching = 0;
    s->index = 0;
    vecClear4(self + 0x578);
    {
        Frame f;
        uint32_t size = f.alloc(8);
        call(Director_getVisibleSize, {size, director()});
        s->screenWidth = ldf(size);
    }
    uint32_t layer = call(Layer_create, {});
    s->container = layer;
    vcall(self, node::addChild, {self, layer});
    uint32_t dispatcher = ld32(director() + smain::lib::Director_eventDispatcher);
    s->listener = call(ListenerTouch_create, {});
    call(ListenerTouch_setSwallowTouches, {s->listener, 0});
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_init_0, {self});
        call(wl::FunctionTouchB_swap, {fo, s->listener + LISTENER_ON_TOUCH_BEGAN});
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t fo = newFunction(f, VT_init_1, {self});
        call(wl::FunctionTouchV_swap, {fo, s->listener + LISTENER_ON_TOUCH_MOVED});
        functionDestroy(fo);
    }
    {
        Frame f;
        uint32_t caps = f.alloc(8);
        st32(caps, self);
        st32(caps + 4, self);
        assignTouchEndLambda(s->listener + LISTENER_ON_TOUCH_ENDED, caps);
        assignTouchEndLambda(s->listener + LISTENER_ON_TOUCH_CANCELLED, caps);
    }
    call(EventDispatcher_addSceneGraph, {dispatcher, s->listener, self});
    call(Node_scheduleUpdate, {self});
    return 1;
}
RW_FUNCTION(0x5258B0, "BaseSlideView::init", BaseSlideView::init);

// target = $_2 {captures[0], captures[1]} (function(lambda).swap(target))
void BaseSlideView::assignTouchEndLambda(Addr target, Addr captures) {
    Frame f;
    uint32_t fo = newFunction(f, VT_init_2, {ld32(captures), ld32(captures + 4)});
    call(wl::FunctionTouchV_swap, {fo, target});
    functionDestroy(fo);
}
RW_FUNCTION(0x525A20, "BaseSlideView::init::assignTouchEndLambda (sub_525A20)", BaseSlideView::assignTouchEndLambda);

// ------------------------------------------------------------------------------------------------ update
// Counts the touch time, then works through the queued page refreshes (one per entry of `updates`).
void BaseSlideView::update(Addr self, float dt) {
    Self *s = S(self);
    if (s->touching) {
        float t = s->touchTime;
        if (t < 0.3f) s->touchTime = t + dt;
    }
    uint32_t updates = self + 0x594, indexes = self + 0x5A0, ids = self + 0x578;
    if (ld32(updates + 4) == ld32(updates)) return;
    for (;;) {
        uint32_t ib = ld32(indexes);
        if (ld32(indexes + 4) == ib) {
            call(wl::VECTOR_AT_THROW, {indexes});
            return;
        }
        int32_t type = int32_t(ld32(ld32(updates)));
        uint32_t k = ld32(ib);
        if (type == PREV || type == NEXT) {
            if (s->updated && vecSize4(ids) <= 2) {
                if (type == PREV) moveNextToPrevNode(self);
                else movePrevToNextNode(self);
            }
            if (!(vecSize4(ids) > k)) {
                call(wl::VECTOR_AT_THROW, {ids});
                return;
            }
            uint32_t field = type == PREV ? self + 0x58C : self + 0x590;
            if (makePage(self, field, ld32(vecBegin(ids) + k * 4), 0) == 0) {
                uint32_t node = ld32(field);
                float x = bf(vcall(node, node::getPositionX, {node}));
                float w = s->screenWidth;
                float nx = type == PREV ? x - w : x + w;
                vcall(node, node::setPositionX, {node, fb(nx)});
                uint32_t c = s->container;
                vcall(c, node::addChild, {c, ld32(field)});
                call(Ref_release, {ld32(field)});
            }
            s->updated = 1;
        } else if (type == CURRENT) {
            if (!(vecSize4(ids) > k)) {
                call(wl::VECTOR_AT_THROW, {ids});
                return;
            }
            if (makePage(self, self + 0x588, ld32(vecBegin(ids) + k * 4), 1) == 0) {
                uint32_t c = s->container;
                vcall(c, node::addChild, {c, s->currentNode});
                call(Ref_release, {s->currentNode});
            }
        }
        vecEraseFront4(indexes);
        uint32_t end = vecEraseFront4(updates);
        if (end == ld32(updates)) return;
    }
}
RW_FUNCTION(0x525A9C, "BaseSlideView::update", BaseSlideView::update);

// The right page widget becomes the left one (two screens to the left)
void BaseSlideView::moveNextToPrevNode(Addr self) {
    Self *s = S(self);
    uint32_t node = s->nextNode;
    if (!node) return;
    s->prevNode = node;
    float x = bf(vcall(node, node::getPositionX, {node}));
    vcall(node, node::setPositionX, {node, fb(x + s->screenWidth * -2.0f)});
    s->nextNode = 0;
}
RW_FUNCTION(0x525DE4, "BaseSlideView::moveNextToPrevNode", BaseSlideView::moveNextToPrevNode);

// The left page widget becomes the right one (two screens to the right)
void BaseSlideView::movePrevToNextNode(Addr self) {
    Self *s = S(self);
    uint32_t node = s->prevNode;
    if (!node) return;
    s->nextNode = node;
    float x = bf(vcall(node, node::getPositionX, {node}));
    float w = s->screenWidth;
    vcall(node, node::setPositionX, {node, fb(x + (w + w))});
    s->prevNode = 0;
}
RW_FUNCTION(0x525E44, "BaseSlideView::movePrevToNextNode", BaseSlideView::movePrevToNextNode);

// Builds the current page; with queueSides, queues the previous and next pages for update() (wrapping around).
void BaseSlideView::updateLayout(Addr self, uint32_t queueSides) {
    Self *s = S(self);
    uint32_t ids = self + 0x578;
    uint32_t idx = uint32_t(s->index.get());
    if (!(vecSize4(ids) > idx)) return;
    if (makePage(self, self + 0x588, ld32(vecBegin(ids) + idx * 4), 1) == 0) {
        uint32_t c = s->container;
        vcall(c, node::addChild, {c, s->currentNode});
        call(Ref_release, {s->currentNode});
    }
    if (queueSides != 1) return;
    int32_t cur = s->index;
    if (cur <= 0) cur = int32_t(vecSize4(ids));
    uint32_t prev = uint32_t(cur - 1);
    Frame f;
    uint32_t tmp = f.alloc(8);           // the UpdateTarget temporaries (NEXT at +0, PREV at +4)
    uint32_t updates = self + 0x594, indexes = self + 0x5A0;
    st32(tmp + 4, PREV);
    if (ld32(updates + 4) < ld32(updates + 8)) pushBackInPlace(updates, PREV);
    else call(UpdateTargetVector_pushBackSlow, {updates, tmp + 4});
    if (!(vecSize4(ids) > prev)) {
        call(wl::VECTOR_AT_THROW, {ids});
        return;
    }
    uint32_t item = vecBegin(ids) + prev * 4;
    if (ld32(indexes + 4) != ld32(indexes + 8)) pushBackInPlace(indexes, ld32(item));
    else call(wl::IntVector_pushBackSlow, {indexes, item});
    uint32_t next = uint32_t(s->index.get()) + 1;
    st32(tmp, NEXT);
    if (!(vecSize4(ids) > next)) next = 0;
    if (ld32(updates + 4) < ld32(updates + 8)) pushBackInPlace(updates, NEXT);
    else call(UpdateTargetVector_pushBackSlow, {updates, tmp});
    if (!(vecSize4(ids) > next)) {
        call(wl::VECTOR_AT_THROW, {ids});
        return;
    }
    item = vecBegin(ids) + next * 4;
    if (ld32(indexes + 4) != ld32(indexes + 8)) pushBackInPlace(indexes, ld32(item));
    else call(wl::IntVector_pushBackSlow, {indexes, item});
}
RW_FUNCTION(0x525EB4, "BaseSlideView::updateLayout", BaseSlideView::updateLayout);

void BaseSlideView::setItemIDs(Addr self, Addr ids, uint32_t queueSides) {
    Self *s = S(self);
    if (self + 0x578 != ids) call(wl::IntVector_assign, {self + 0x578, ld32(ids), ld32(ids + 4)});
    uint8_t enabled = 0;
    if (vecSize4(ids) >= 2) enabled = vecSize4(self + 0x578) > 1 ? 1 : 0;
    s->slideEnabled = enabled;
    updateLayout(self, queueSides);
}
RW_FUNCTION(0x5260AC, "BaseSlideView::setItemIDs", BaseSlideView::setItemIDs);

void BaseSlideView::setSlideEnabled(Addr self, uint32_t enabled) {
    Self *s = S(self);
    uint8_t v = 0;
    if (enabled == 1) v = vecSize4(self + 0x578) > 1 ? 1 : 0;
    s->slideEnabled = v;
}
RW_FUNCTION(0x526110, "BaseSlideView::setSlideEnabled", BaseSlideView::setSlideEnabled);

// ------------------------------------------------------------------------------------------------ paging
void BaseSlideView::onCompleteAnimation(Addr self, uint32_t delayed) {
    Self *s = S(self);
    if (delayed == 1) {
        uint32_t delay = call(wl::DelayTime_create, {fb(0.032f)});
        Frame f;
        uint32_t fo = newFunction(f, VT_onCompleteAnimation_3, {self});
        uint32_t cf = call(wl::CallFunc_create, {fo});
        runSequence2(self, delay, cf);
        functionDestroy(fo);
        return;
    }
    s->animating = 0;
    uint32_t c = s->container;
    vcall(c, node::setPositionX, {c, fb(s->baseX)});
}
RW_FUNCTION(0x526154, "BaseSlideView::onCompleteAnimation", BaseSlideView::onCompleteAnimation);

uint32_t BaseSlideView::changeNextPage(Addr self) {
    if (S(self)->animating) return 0;
    if (vecSize4(self + 0x578) <= 2) movePrevToNextNode(self);
    runRightSlide(self);
    return 1;
}
RW_FUNCTION(0x526244, "BaseSlideView::changeNextPage", BaseSlideView::changeNextPage);

uint32_t BaseSlideView::changePreviousPage(Addr self) {
    if (S(self)->animating) return 0;
    if (vecSize4(self + 0x578) <= 2) moveNextToPrevNode(self);
    runLeftSlide(self);
    return 1;
}
RW_FUNCTION(0x5263EC, "BaseSlideView::changePreviousPage", BaseSlideView::changePreviousPage);

// Slide the container one screen to the left (to the next item), then $_5
void BaseSlideView::runRightSlide(Addr self) {
    Self *s = S(self);
    if (rw::get<uint32_t>(s, 0x540)) call(wl::FunctionVV_call, {A(s->onSlideStart)});
    s->animating = 1;
    uint32_t idx = uint32_t(s->index.get()) + 1;
    s->index = int32_t(idx);
    if (!(vecSize4(self + 0x578) > idx)) s->index = 0;
    float x = s->baseX - s->screenWidth;
    uint32_t c = s->container;
    float y = bf(vcall(c, node::getPositionY, {c}));
    Frame f;
    uint32_t pos = f.alloc(8);
    call(wl::Vec2_ctorXY, {pos, fb(x), fb(y)});
    uint32_t move = call(wl::MoveTo_create, {fb(0.15f), pos});
    uint32_t fo = newFunction(f, VT_runRightSlide_5, {self});
    uint32_t cf = call(wl::CallFunc_create, {fo});
    runSequence2(c, move, cf);
    functionDestroy(fo);
    call(wl::Vec2_dtor, {pos});
}
RW_FUNCTION(0x526284, "BaseSlideView::runRightSlide", BaseSlideView::runRightSlide);

// Slide the container one screen to the right (to the previous item), then $_4
void BaseSlideView::runLeftSlide(Addr self) {
    Self *s = S(self);
    if (rw::get<uint32_t>(s, 0x540)) call(wl::FunctionVV_call, {A(s->onSlideStart)});
    s->animating = 1;
    int32_t idx = s->index;
    s->index = int32_t(uint32_t(idx) - 1);
    if (idx <= 0) s->index = int32_t(vecSize4(self + 0x578) - 1);
    float x = s->baseX + s->screenWidth;
    uint32_t c = s->container;
    float y = bf(vcall(c, node::getPositionY, {c}));
    Frame f;
    uint32_t pos = f.alloc(8);
    call(wl::Vec2_ctorXY, {pos, fb(x), fb(y)});
    uint32_t move = call(wl::MoveTo_create, {fb(0.15f), pos});
    uint32_t fo = newFunction(f, VT_runLeftSlide_4, {self});
    uint32_t cf = call(wl::CallFunc_create, {fo});
    runSequence2(c, move, cf);
    functionDestroy(fo);
    call(wl::Vec2_dtor, {pos});
}
RW_FUNCTION(0x52642C, "BaseSlideView::runLeftSlide", BaseSlideView::runLeftSlide);

float BaseSlideView::getMoveX(Addr self, float x) {
    Self *s = S(self);
    return clampMove(x - s->touchStartX, s->screenWidth);
}
RW_FUNCTION(0x5265A0, "BaseSlideView::getMoveX", BaseSlideView::getMoveX);

// ------------------------------------------------------------------------------------------------ lambdas
// $_3: the delayed end of a slide - rebuild the pages, put the container back
void BaseSlideView::onCompleteDelayed(Addr fn) {
    uint32_t self = ld32(fn + 4);
    updateLayout(self, 1);
    Self *s = S(self);
    s->animating = 0;
    uint32_t c = s->container;
    vcall(c, node::setPositionX, {c, fb(s->baseX)});
}
RW_FUNCTION(0x5267B4, "BaseSlideView::onCompleteAnimation::$_3::operator()", BaseSlideView::onCompleteDelayed);

// $_6: a bounce back ended
void BaseSlideView::onBounceDone(Addr fn) {
    Self *s = S(ld32(fn + 4));
    s->animating = 0;
    uint32_t c = s->container;
    vcall(c, node::setPositionX, {c, fb(s->baseX)});
}
RW_FUNCTION(0x526D08, "BaseSlideView::init::$_6::operator()", BaseSlideView::onBounceDone);

// $_0: a touch inside the view starts a drag (not while sliding)
uint32_t BaseSlideView::onTouchBegan(Addr fn, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    if (!s->slideEnabled || s->animating) return 0;
    uint32_t result = 0;
    Frame f;
    uint32_t loc = f.alloc(8);
    uint32_t p = f.alloc(8);
    uint32_t rect = f.alloc(16);
    call(Touch_getLocation, {loc, touch});
    call(Node_convertToNodeSpace, {p, self, loc});
    call(wl::Vec2_dtor, {loc});
    uint32_t w = ld32(vcall(self, node::getContentSize, {self}));
    uint32_t h = ld32(vcall(self, node::getContentSize, {self}) + 4);
    call(Rect_ctor, {rect, 0, 0, w, h});
    if (call(Rect_containsPoint, {rect, p})) {
        result = 1;
        s->touching = 1;
        s->touchTime = 0.0f;
        uint32_t loc2 = f.alloc(8);
        call(Touch_getLocation, {loc2, touch});
        rw::set<uint32_t>(s, 0x5B4, ld32(loc2));
        call(wl::Vec2_dtor, {loc2});
        uint32_t c = s->container;
        rw::set<uint32_t>(s, 0x5B0, vcall(c, node::getPositionX, {c}));
        s->dragging = 0;
    }
    call(wl::Vec2_dtor, {rect});
    call(wl::Vec2_dtor, {p});
    return result;
}
RW_FUNCTION(0x526FC8, "BaseSlideView::init::$_0::operator()", BaseSlideView::onTouchBegan);

// $_1: drag the container (after 20 pixels), keeping a page on the side the finger moves to
void BaseSlideView::onTouchMoved(Addr fn, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    float v;
    {
        Frame f;
        uint32_t loc = f.alloc(8);
        call(Touch_getLocation, {loc, touch});
        float d = ldf(loc) - s->touchStartX;
        v = clampMove(d, s->screenWidth);
        call(wl::Vec2_dtor, {loc});
    }
    if (!s->dragging) {
        if (bf(fb(v) & 0x7FFFFFFFu) < 20.0f) return;
        s->dragging = 1;
    }
    if (!s->slideEnabled) return;
    if (vecSize4(self + 0x578) <= 2) {
        if (v <= 0.0f || v != v) movePrevToNextNode(self);
        else moveNextToPrevNode(self);
    }
    uint32_t c = s->container;
    vcall(c, node::setPositionX, {c, fb(v + s->baseX)});
    if (rw::get<uint32_t>(s, 0x558)) call(wl::FunctionVF_call, {A(s->onMove), fb(v)});
}
RW_FUNCTION(0x526DC0, "BaseSlideView::init::$_1::operator()", BaseSlideView::onTouchMoved);

// $_2: the finger is lifted - slide to the next / previous item when dragged past 30% of the width (10% for a
// quick flick under 0.3 s), else bounce back
void BaseSlideView::onTouchEnded(Addr fn, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    uint32_t self = ld32(fn + 4);
    Self *s = S(self);
    s->touching = 0;
    s->animating = 1;
    float v;
    {
        Frame f;
        uint32_t loc = f.alloc(8);
        call(Touch_getLocation, {loc, touch});
        float d = ldf(loc) - s->touchStartX;
        v = clampMove(d, s->screenWidth);
        call(wl::Vec2_dtor, {loc});
    }
    float w = s->screenWidth;
    float small = w * 0.1f, large = w * 0.3f;
    auto unord = [](float a, float b) { return a != a || b != b; };
    bool left;    // runLeftSlide
    if (large <= v) {
        left = true;
    } else {
        float t = s->touchTime;
        if (!(small > v || unord(small, v)) && !(t > 0.3f || unord(t, 0.3f))) {
            left = true;
        } else {
            left = false;
            float nl = negf(large), ns = negf(small);
            bool right;
            if (v <= nl) right = true;
            else if (v > ns || unord(v, ns)) right = false;
            else right = !(t > 0.3f || unord(t, 0.3f));
            if (right) {
                runRightSlide(self);
            } else {
                float x = s->baseX;
                uint32_t c = s->container;
                float y = bf(vcall(c, node::getPositionY, {c}));
                Frame f;
                uint32_t pos = f.alloc(8);
                call(wl::Vec2_ctorXY, {pos, fb(x), fb(y)});
                uint32_t move = call(wl::MoveTo_create, {fb(0.15f), pos});
                uint32_t fo = newFunction(f, VT_init_6, {ld32(fn + 8)});
                uint32_t cf = call(wl::CallFunc_create, {fo});
                runSequence2(c, move, cf);
                functionDestroy(fo);
                call(wl::Vec2_dtor, {pos});
            }
        }
    }
    if (left) runLeftSlide(self);
    if (rw::get<uint32_t>(s, 0x570)) call(wl::FunctionVV_call, {A(s->onTouchEnd)});
}
RW_FUNCTION(0x5269BC, "BaseSlideView::init::$_2::operator()", BaseSlideView::onTouchEnded);
