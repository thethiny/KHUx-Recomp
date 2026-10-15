// BattleResultScrollView: construction, the row widgets, touch handling, the animation settings and the
// ResutData copy constructor.
#include "BattleResultScrollView.hpp"

using namespace brsv;

namespace {
constexpr uint32_t S_GetItemJson = 0xDFD72A;       // "cocostudio/publish/BattleResult_GetItem.json"
constexpr uint32_t S_AnimJson = 0xDFD757;          // "cocostudio/publish/BattleResultAnimation.ExportJson"
constexpr uint32_t S_Anim = 0xDFD78B;              // "BattleResultAnimation"
constexpr uint32_t S_AnimNode = 0xDFD7A1;          // "BattleResultAnim"
constexpr uint32_t S_GetItem = 0xDFD7B2;           // "GetItem"
constexpr uint32_t S_ItemLabel = 0xDFD7BA;         // "Txt_GetItem_Label"
constexpr uint32_t S_ItemNumLabel = 0xDFD7CC;      // "Txt_GetItem_Num_Label"
constexpr uint32_t S_New = 0xDFD177;               // "New"
constexpr uint32_t S_UnderPanel = 0xDFD7E2;        // "UnderPanel"
constexpr uint32_t S_Wording = 0xDF862F;           // "Txt_Wording"
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;   // GOT slot: &cocos2d::Vec2::ANCHOR_MIDDLE
constexpr uint32_t TEXT_ITEM_COUNT = 103100054;    // TextData: the "x%d" count format

// lambdas of registItemTouchEvent (vtable + 8)
constexpr uint32_t VT_TouchBegan = 0xF566AC;       // $_0 [this, index]
constexpr uint32_t VT_TouchMoved = 0xF56680;       // $_1 [this, index, $_2]
constexpr uint32_t VT_TouchEnded = 0xF56628;       // $_2 [this, index, this, index]
constexpr uint32_t VT_TapDone = 0xF56654;          // $_3 (tap animation ended)

constexpr uint32_t LISTENER_ON_TOUCH_BEGAN = 0x50, LISTENER_ON_TOUCH_MOVED = 0x68;
constexpr uint32_t LISTENER_ON_TOUCH_ENDED = 0x80, LISTENER_ON_TOUCH_CANCELLED = 0x98;
}  // namespace

// ------------------------------------------------------------------------------------------------ helpers
namespace brsv {
void pushResult(Self *s, Addr data) {
    uint32_t end = s->results.end;
    if (end == s->results.cap) {
        call(g::ResutVector_pushSlow, {me(s) + 0x650, data});
    } else {
        uint32_t e = 0;
        if (end) {
            Self::copyResult(rw::host_ptr<ResutData>(end), rw::host_ptr<ResutData>(data));
            e = s->results.end;
        }
        s->results.end = e + Self::RESULT_SIZE;
    }
    if (ld32(data) == 3) s->hasRare = 1;
}

uint32_t fillPending(Self *s, Addr counter) {
    uint32_t begin = s->pending.begin, end = s->pending.end;
    uint32_t cur = begin;
    if (end != begin) {   // clear (end = begin, computed the way the inlined vector::clear does)
        cur = end + (0xFFFFFFFCu & ~((end - 4) - begin));
        s->pending.end = cur;
    }
    uint32_t k = 0;
    *rw::frame_ptr<uint32_t>(counter) = 0;
    uint32_t n = Self::resultCount(s);
    if (s->results.end == s->results.begin) return n;
    for (;;) {
        if (cur == s->pending.cap) {
            call(g::vector_int_pushSlowRef, {me(s) + 0x65C, counter});
        } else {
            uint32_t e = 0;
            if (cur) {
                st32(cur, k);
                e = s->pending.end;
                k = ld32(counter);
            }
            s->pending.end = e + 4;
        }
        k++;
        *rw::frame_ptr<uint32_t>(counter) = k;
        n = Self::resultCount(s);
        if (k >= n) return n;
        cur = s->pending.end;
    }
}

void playSE(uint32_t id) {
    Frame f;
    Addr layer = f.alloc(8);
    call(g::Audio_getLayer, {layer, call(g::Audio_getInstance, {}), 0});
    call(g::AudioLayer_playSE, {ld32(layer), id, 0, 0});
    sharedRelease(layer);
}
}  // namespace brsv

// ------------------------------------------------------------------------------------------------ life cycle
Self *Self::construct(Self *self) {
    Addr s = me(self);
    call(g::Layout_ctor, {s});
    // IScrollProtocol (inlined)
    self->deceleration = 0.1f;
    self->bounceDeceleration = 0.3f;
    self->itemCount = 0;
    self->field_508 = 0;
    self->layoutPanel = 0;
    self->bouncePixel = 200.0f;
    call(g::aeabi_memset, {s + 0x514, 0x24, 0});
    self->maxColumn = 1;
    self->scrollEnabled = 1;
    self->scrollType = 0;
    self->scrollBarEnabled = 0;
    self->targetPosition = 0.0f;
    rw::set<uint32_t>(self, 0x5D8, 0);   // the four std::function members: empty
    rw::set<uint32_t>(self, 0x5F0, 0);
    rw::set<uint32_t>(self, 0x608, 0);
    rw::set<uint32_t>(self, 0x620, 0);
    call(g::aeabi_memset, {s + 0x540, 0x12, 0});
    call(g::aeabi_memset, {s + 0x554, 0x12, 0});
    call(g::aeabi_memset, {s + 0x568, 0x25, 0});
    call(g::aeabi_memset, {s + 0x598, 0x2A, 0});
    // BaseScrollView<int>
    self->ids.begin = 0;
    self->ids.end = 0;
    self->ids.cap = 0;
    // BattleResultScrollView
    self->vtable = VTABLE_BASE + 8;
    self->vtable2 = VTABLE_BASE + 0x360;
    self->vtable3 = VTABLE_BASE + 0x374;
    self->onTapItem_f = 0;
    call(g::aeabi_memset, {s + 0x650, 0x18, 0});   // results, pending
    self->rareDropSpeed = 0.5f;
    self->addRightInTime = 0.2f;
    self->delayTime = 0.2f;
    self->listMoveTime = 0.2f;
    self->adding = 0;
    self->dropping = 0;
    self->hasRare = 0;
    self->enabledTapItem = 0;
    self->touching = 0;
    self->touchIndex = 0;
    self->tapping = 0;
    self->touchStartY = 0.0f;
    return self;
}
RW_FUNCTION(0x511DA0, "BattleResultScrollView::BattleResultScrollView()", BattleResultScrollView::construct);

Self *Self::destruct(Self *self) {
    Addr s = me(self);
    self->vtable = VTABLE_BASE + 8;
    self->vtable2 = VTABLE_BASE + 0x360;
    self->vtable3 = VTABLE_BASE + 0x374;
    uint32_t row = self->rowTemplate;
    if (row) vcall(row, slot::release, {row});
    self->rowTemplate = 0;
    call(g::vector_int_dtor, {s + 0x65C});
    call(g::ResutVector_dtor, {s + 0x650});
    functionDestroy(s + 0x638);
    // back to BaseScrollView<int>
    self->vtable = BASE_VTABLE + 8;
    self->vtable2 = BASE_VTABLE + 0x360;
    self->vtable3 = BASE_VTABLE + 0x374;
    call(g::vector_int_dtor, {s + 0x628});
    call(g::IScrollProtocol_dtor, {s});
    return self;
}
RW_FUNCTION(0x511BB4, "BattleResultScrollView::~BattleResultScrollView()", BattleResultScrollView::destruct);

void Self::destructDelete(Self *self) { call(fn::operator_delete, {me(destruct(self))}); }
RW_FUNCTION(0x511D18, "BattleResultScrollView::~BattleResultScrollView() (deleting)", BattleResultScrollView::destructDelete);

uint32_t Self::create() {
    Addr view = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<Self>(view));
    if (vcall(view, V_init, {view}) == 1) {
        call(g::Ref_autorelease, {view});
        return view;
    }
    vcall(view, slot::release, {view});
    return 0;
}
RW_FUNCTION(0x511D4C, "BattleResultScrollView::create()", BattleResultScrollView::create);

uint32_t Self::init(Self *self) {
    Addr s = me(self);
    self->itemWidth = 658.0f;
    self->itemHeight = 120.0f;
    call(g::IScrollProtocol_setEnabledScrollBar, {s, 1});
    call(g::IScrollProtocol_setScrollType, {s, 0});
    uint32_t row = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_GetItemJson});
    self->rowTemplate = row;
    call(g::Ref_retain, {row});
    uint32_t t = self->rowTemplate;
    vcall(t, V_setTouchEnabled, {t, 0});
    return call(g::BaseScrollView_int_init, {s});
}
RW_FUNCTION(0x511F44, "BattleResultScrollView::init()", BattleResultScrollView::init);

// ------------------------------------------------------------------------------------------------ rows
// A row: a plain Widget holding a clone of the template (moved right by half its width) and the result
// animation armature placed on the template's "GetItem" image, hidden.
uint32_t Self::getLayoutPanel(Self *self, int32_t /*unused*/) {
    uint32_t panel = call(g::Widget_create, {});
    uint32_t item = call(g::Widget_clone, {self->rowTemplate});
    uint32_t setPosition = ld32(ld32(item) + slot::setPosition);
    uint32_t t = self->rowTemplate;
    float width = ldf(vcall(t, node::getContentSize, {t}));
    {
        Frame f;
        Addr pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, fbits(width * 0.5f), 0});
        call(setPosition, {item, pos});
        call(g::Vec2_dtor, {pos});
    }
    vcall(panel, node::setAnchorPoint, {panel, ld32(GOT_ANCHOR_MIDDLE)});
    vcall(panel, node::addChild, {panel, item});
    uint32_t setContentSize = ld32(ld32(panel) + node::setContentSize);
    {
        Frame f;
        Addr size = f.alloc(8);
        call(g::Size_ctor, {size, fbits(float(self->itemWidth)), fbits(float(self->itemHeight))});
        call(setContentSize, {panel, size});
    }
    vcall(panel, node::setPositionX, {panel, fbits(float(self->itemWidth))});
    uint32_t adm = call(g::ArmatureDataManager_getInstance, {});
    withStr(S_AnimJson, 0x33, [&](Addr s) { call(g::ArmatureDataManager_addArmatureFileInfo, {adm, s}); });
    uint32_t armature = withStr(S_Anim, 0x15, [&](Addr s) { return call(g::Armature_create, {s}); });
    uint32_t setName = ld32(ld32(armature) + node::setName);
    withStr(S_AnimNode, 0x10, [&](Addr s) { call(setName, {armature, s}); });
    uint32_t image = search(item, S_GetItem, 7);
    vcall(item, node::addChild, {item, armature});
    uint32_t armSetPosition = ld32(ld32(armature) + slot::setPosition);
    call(armSetPosition, {armature, vcall(image, node::getPosition, {image})});
    vcall(armature, node::setVisible, {armature, 0});
    return panel;
}
RW_FUNCTION(0x511FC0, "BattleResultScrollView::getLayoutPanel(int)", BattleResultScrollView::getLayoutPanel);

uint32_t Self::createRow(Self *self, uint32_t index, uint32_t arg) {
    if (idCount(self) <= index) return call(g::throwOutOfRange, {});
    uint32_t panel = getLayoutPanel(self, 0x628);
    vcall(me(self), V_changeRow, {me(self), panel, index, arg});
    return panel;
}
RW_FUNCTION(0x512260, "BattleResultScrollView::createRow(int, int)", BattleResultScrollView::createRow);

// Fill a row with result ids[index]: image (keyblades at 30 %), name, count, "New" mark and the wording panel.
// Returns 1 when the row has no item image, else 0.
uint32_t Self::changeRow(Self *self, uint32_t widget, uint32_t index, uint32_t /*arg*/) {
    uint32_t idsBegin = self->ids.begin;
    if (idCount(self) <= index) return call(g::throwOutOfRange, {});
    uint32_t image = search(widget, S_GetItem, 7);
    if (!image) return 1;
    uint32_t slotAddr = idsBegin + 4 * index;   // &ids[index]
    auto data = [&]() { return resultAt(self, ld32(slotAddr)); };
    call(g::ImageView_loadTexture, {image, data() + 4, 0});
    float scale = ld32(data()) == 13 ? 0.3f : 1.0f;
    vcall(image, V_setScaleXY, {image, fbits(scale), fbits(scale)});
    uint32_t label = search(widget, S_ItemLabel, 0x11);
    call(g::SceneUtil_setShadow, {label});
    call(g::Text_setString, {label, data() + 0x10});
    uint32_t numLabel = search(widget, S_ItemNumLabel, 0x15);
    call(g::SceneUtil_setShadow, {numLabel});
    {
        Frame f;
        Addr text = f.alloc(8);
        call(g::TextData_get, {text, 0, TEXT_ITEM_COUNT});
        Addr buf = stringBuffer(f, {ld32(ld32(text) + 4), ld32(data() + 0x30)});
        uint32_t s = newStr(f, buf, call(g::strlen_, {buf}));
        call(g::Text_setString, {numLabel, s});
        strFree(s);
        sharedRelease(text);
    }
    uint32_t mark = search(widget, S_New, 3);
    vcall(mark, node::setVisible, {mark, ld8(data() + 0x20)});
    uint32_t under = search(widget, S_UnderPanel, 0xA);
    uint32_t wording = search(widget, S_Wording, 0xB);
    if (!ld8(data() + 0x34)) {
        vcall(under, node::setVisible, {under, 0});
        vcall(wording, node::setVisible, {wording, 0});
        return 0;
    }
    call(g::SceneUtil_setShadow, {wording});
    uint32_t w = data() + 0x38;
    uint32_t text = strData(w);
    {
        Frame f;
        uint32_t s = newStr(f, text, call(g::strlen_, {text}));
        call(g::Text_setString, {wording, s});
        strFree(s);
    }
    vcall(under, node::setVisible, {under, 1});
    vcall(under, node::setOpacity, {under, 255});
    vcall(wording, node::setVisible, {wording, 1});
    return 0;
}
RW_FUNCTION(0x5122B8, "BattleResultScrollView::changeRow(cocos2d::ui::Widget*, int, int)", BattleResultScrollView::changeRow);

// ------------------------------------------------------------------------------------------------ touch
// listener->onTouchMoved / onTouchEnded = $_2 [this, index, this, index] (outlined std::function assignment)
static void assignTouchLambda(uint32_t dst, uint32_t capture) {
    uint32_t c0 = ld32(capture), c1 = ld32(capture + 4), c2 = ld32(capture + 8), c3 = ld32(capture + 12);
    Frame f;
    Addr fn = f.alloc(24);
    st32(fn + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x14});
    if (heap) {
        st32(heap, VT_TouchEnded);
        st32(heap + 4, c0);
        st32(heap + 8, c1);
        st32(heap + 12, c2);
        st32(heap + 16, c3);
    }
    st32(fn + 16, heap);
    call(g::function_touchVoid_swap, {fn, dst});
    functionDestroy(fn);
}
RW_FUNCTION(0x51287C, "BattleResultScrollView::registItemTouchEvent assign $_2 (std::function operator=)", assignTouchLambda);

void Self::registItemTouchEvent(Self *self, uint32_t /*widget*/, int32_t index) {
    if (!self->enabledTapItem) return;
    Addr s = me(self);
    uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    withLambda(VT_TouchBegan, {s, uint32_t(index)},
               [&](Addr fn) { call(g::function_touchBool_swap, {fn, listener + LISTENER_ON_TOUCH_BEGAN}); });
    Frame f;
    Addr capture = f.alloc(16);   // $_2's captures, reused by $_1
    st32(capture, s);
    st32(capture + 4, uint32_t(index));
    st32(capture + 8, s);
    st32(capture + 12, uint32_t(index));
    {
        Frame g2;
        Addr fn = g2.alloc(24);
        st32(fn + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x1C});
        if (heap) {
            st32(heap, VT_TouchMoved);
            st32(heap + 4, s);
            st32(heap + 8, uint32_t(index));
            st32(heap + 12, s);
            st32(heap + 16, uint32_t(index));
            st32(heap + 20, s);
            st32(heap + 24, uint32_t(index));
        }
        st32(fn + 16, heap);
        call(g::function_touchVoid_swap, {fn, listener + LISTENER_ON_TOUCH_MOVED});
        functionDestroy(fn);
    }
    assignTouchLambda(listener + LISTENER_ON_TOUCH_ENDED, capture);
    assignTouchLambda(listener + LISTENER_ON_TOUCH_CANCELLED, capture);
    call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, s});
}
RW_FUNCTION(0x512734, "BattleResultScrollView::registItemTouchEvent(cocos2d::ui::Widget*, int)",
            BattleResultScrollView::registItemTouchEvent);

// $_2's body [view, index, ...]: a touch released near where it began on a filled cell plays the tap animation
// (whose end, $_3, reports the tap).
static void touchEnded(uint32_t capture, uint32_t touch) {
    Self *s = rw::host_ptr<Self>(ld32(capture));
    float rowHeight = s->itemHeight + ldf(ld32(GOT_ITEM_MARGIN));
    int32_t pos = f2i(s->position);
    int32_t rows = int32_t(call(g::divsi3, {uint32_t(pos), uint32_t(f2i(rowHeight))}));
    uint32_t index = ld32(capture + 4) + uint32_t(s->maxColumn) * uint32_t(rows) - 1;
    s->touchIndex = int32_t(index);
    if (Self::idCount(s) <= index || !s->touching) {
        s->touching = 0;
        return;
    }
    Frame f;
    Addr loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    float dy = std::fabs(ldf(loc + 4) - s->touchStartY);
    if (!(dy < 10.0f)) {
        call(g::Vec2_dtor, {loc});
        s->touching = 0;
        return;
    }
    uint32_t at = uint32_t(s->touchIndex);
    if (Self::idCount(s) <= at) {
        call(g::throwOutOfRange, {});
        return;
    }
    uint32_t id = ld32(s->ids.begin + 4 * at);
    call(g::Vec2_dtor, {loc});
    if (!id) {
        s->touching = 0;
        return;
    }
    s->tapping = 1;
    uint32_t cell = ld32(s->items.begin + 4 * ld32(capture + 4));
    vcall(cell, node::setLocalZOrder, {cell, 1});
    withLambda(VT_TapDone, {ld32(capture + 8), ld32(capture + 12)}, [&](Addr fn) {
        uint32_t anim = call(g::SceneUtil_createTapAnimation, {cell, fn});
        call(g::Node_runAction, {cell, anim});
    });
}
RW_FUNCTION(0x515AF4, "BattleResultScrollView::registItemTouchEvent $_2 body", touchEnded);

static void touchEndedCall(uint32_t func, const uint32_t *touch) { touchEnded(func + 4, *touch); }
RW_FUNCTION(0x515AB2, "BattleResultScrollView::registItemTouchEvent $_2 __func::operator()", touchEndedCall);

Self *Self::setEnabledTapItem(Self *self, uint32_t enabled) {
    self->enabledTapItem = uint8_t(enabled);
    return self;
}
RW_FUNCTION(0x512910, "BattleResultScrollView::setEnabledTapItem(bool)", BattleResultScrollView::setEnabledTapItem);

// ------------------------------------------------------------------------------------------------ settings
Self *Self::setAnimationTime(Self *self, float rareDrop, float addRightIn, float delay, float listMove) {
    self->rareDropSpeed = rareDrop;
    self->addRightInTime = addRightIn;
    self->delayTime = delay;
    self->listMoveTime = listMove;
    return self;
}
RW_FUNCTION(0x5133BC, "BattleResultScrollView::setAnimationTime(float, float, float, float)",
            BattleResultScrollView::setAnimationTime);

Self *Self::setRareDropSpeed(Self *self, float v) {
    self->rareDropSpeed = v;
    return self;
}
RW_FUNCTION(0x5133E0, "BattleResultScrollView::setRareDropSpeed(float)", BattleResultScrollView::setRareDropSpeed);

Self *Self::setAddRightInTime(Self *self, float v) {
    self->addRightInTime = v;
    return self;
}
RW_FUNCTION(0x5133E8, "BattleResultScrollView::setAddRightInTime(float)", BattleResultScrollView::setAddRightInTime);

Self *Self::setDelayTime(Self *self, float v) {
    self->delayTime = v;
    return self;
}
RW_FUNCTION(0x5133F4, "BattleResultScrollView::setDelayTime(float)", BattleResultScrollView::setDelayTime);

Self *Self::setListMoveTime(Self *self, float v) {
    self->listMoveTime = v;
    return self;
}
RW_FUNCTION(0x5133FC, "BattleResultScrollView::setListMoveTime(float)", BattleResultScrollView::setListMoveTime);

// ------------------------------------------------------------------------------------------------ ResutData
static void copyString(Addr dst, Addr src) {
    if (ld8(src) & 1) {
        call(g::string_init, {dst, ld32(src + 8), ld32(src + 4)});
    } else {
        st32(dst, ld32(src));
        st32(dst + 4, ld32(src + 4));
        st32(dst + 8, ld32(src + 8));
    }
}

ResutData *Self::copyResult(ResutData *self, const ResutData *other) {
    Addr d = rw::guest_addr(self), s = rw::guest_addr(other);
    self->type = other->type;
    copyString(d + 4, s + 4);
    self->type = other->type;
    copyString(d + 0x10, s + 0x10);
    for (uint32_t k = 0x1C; k <= 0x30; k += 4) st32(d + k, ld32(s + k));
    self->hasWording = other->hasWording;
    copyString(d + 0x38, s + 0x38);
    return self;
}
RW_FUNCTION(0x5153E6, "BattleResultScrollView::ResutData::ResutData(const ResutData&)", BattleResultScrollView::copyResult);
