// BattleResultScrollView: adding the items one by one. pushFrontItem / pushBackItem slide the visible rows by one
// item ($_4 / $_6 run when each row arrives); when the last row arrived, addFrontItem / addBackItem insert the new
// item, slide it in from the right and show its image (setDropImage: treasures first play the reward armature,
// whose end, $_8, shows the name and count).
#include "BattleResultScrollView.hpp"

using namespace brsv;

namespace {
constexpr uint32_t SE_ITEM_IN = 0x1E56B97C;        // an item slides in
constexpr uint32_t SE_REWARD = 0x1E56BAEE;         // the treasure reward armature
constexpr uint32_t S_GetItem = 0xDFD7B2;           // "GetItem"
constexpr uint32_t S_ItemLabel = 0xDFD7BA;         // "Txt_GetItem_Label"
constexpr uint32_t S_ItemNumLabel = 0xDFD7CC;      // "Txt_GetItem_Num_Label"
constexpr uint32_t S_AnimNode = 0xDFD7A1;          // "BattleResultAnim"
constexpr uint32_t S_RewardAnime = 0xDFD7ED;       // "Result_Reward_Anime"
constexpr uint32_t S_Layer1 = 0xDFD808, S_Layer2 = 0xDFD801;
constexpr uint32_t S_TreasureFmt = 0xDFD80F;       // "img/prizes/Prize_Treasure%02d.png"
constexpr uint32_t S_CountFmt = 0xDFD670;          // the "x%d" count format
constexpr uint32_t S_Empty = 0xDF8A1A;             // ""

constexpr uint32_t VT_BackRowArrived = 0xF565D0;   // pushBackItem $_6 (vtable + 8)
constexpr uint32_t VT_FrontRowArrived = 0xF565FC;  // pushFrontItem $_4
constexpr uint32_t VT_BackAdded = 0xF566D8;        // pushBackItem $_5 (the added item arrived)
constexpr uint32_t VT_FrontAdded = 0xF56704;       // pushFrontItem $_3
constexpr uint32_t VT_DropEnded = 0xF565A4;        // setDropImage $_8
constexpr uint32_t ROW_LAMBDA_SIZE = 0x20;         // [row, Vec2 position, k, count, view, index]

float getItemPixel(Self *s) { return rw::callf(g::IScrollProtocol_getItemPixel, {me(s)}); }

// node->setPosition(Vec2(x, y)) through the vtable function fetched first
void setPositionXY(uint32_t node, float x, float y) {
    uint32_t setPosition = ld32(ld32(node) + slot::setPosition);
    Frame f;
    Addr v = f.alloc(8);
    call(g::Vec2_ctor, {v, fbits(x), fbits(y)});
    call(setPosition, {node, v});
    call(g::Vec2_dtor, {v});
}

// The rows that move: min(itemCount, rowCount / 2 + 1) as the code computes it in float.
int32_t movingRows(Self *s, uint32_t n) {
    float lim = s->rowCount * 0.5f + 1.0f;
    if (!(float(n) > lim)) lim = float(int32_t(n));
    return f2i(lim);
}

// Slide rows 1..count by one item (dir = -1: up, +1: down); each row's arrival runs a heap lambda
// [row, its old position, k, count, view, index] (vtable `vt`).
void slideRows(Self *s, int32_t index, float dir, uint32_t vt) {
    uint32_t n = Self::idCount(s);
    int32_t count = movingRows(s, n);
    if (count < 1) return;
    for (int32_t k = 1;; k++) {
        uint32_t row = ld32(s->items.begin + 4 * uint32_t(k));
        Frame f;
        Addr pos = f.alloc(8);
        call(g::Vec2_copy, {pos, vcall(row, node::getPosition, {row})});
        setPositionXY(row, ldf(pos), ldf(pos + 4));
        float y = ldf(pos + 4);
        float duration = s->listMoveTime;
        float x = ldf(pos);
        float pixel = getItemPixel(s);
        Addr target = f.alloc(8);
        call(g::Vec2_ctor, {target, fbits(x), fbits(dir < 0 ? y - pixel : y + pixel)});
        uint32_t move = call(g::MoveTo_create, {fbits(duration), target});
        // the lambda's captures, built on the stack, then copied into the heap __func
        Addr cap = f.alloc(0x1C);
        st32(cap, row);
        call(g::Vec2_copy, {cap + 4, pos});
        st32(cap + 0xC, uint32_t(k));
        st32(cap + 0x10, uint32_t(count));
        st32(cap + 0x14, me(s));
        st32(cap + 0x18, uint32_t(index));
        Addr fn = f.alloc(24);
        st32(fn + 16, 0);
        uint32_t heap = call(fn::operator_new, {ROW_LAMBDA_SIZE});
        if (heap) {
            st32(heap, vt);
            st32(heap + 4, row);
            call(g::Vec2_copy, {heap + 8, cap + 4});
            st32(heap + 0x10, uint32_t(k));
            st32(heap + 0x14, uint32_t(count));
            st32(heap + 0x18, me(s));
            st32(heap + 0x1C, uint32_t(index));
        }
        st32(fn + 16, heap);
        uint32_t then = call(g::CallFunc_create, {fn});
        uint32_t seq = call(g::Sequence_create, {move, then, 0});
        call(g::Node_runAction, {row, seq});
        functionDestroy(fn);
        call(g::Vec2_dtor, {cap + 4});
        call(g::Vec2_dtor, {target});
        call(g::Vec2_dtor, {pos});
        if (k >= count) return;
    }
}

// Slide `row` in from the right (an item width) back to its position, then the "added" lambda [view]; and its
// image.
void slideIn(Self *s, uint32_t row, int32_t index, uint32_t addedVt) {
    Frame f;
    Addr pos = f.alloc(8);
    call(g::Vec2_copy, {pos, vcall(row, node::getPosition, {row})});
    setPositionXY(row, ldf(pos) + s->itemWidth, ldf(pos + 4));
    float duration = s->addRightInTime;
    Addr target = f.alloc(8);
    call(g::Vec2_ctor, {target, ld32(pos), ld32(pos + 4)});
    uint32_t move = call(g::MoveTo_create, {fbits(duration), target});
    uint32_t delay = call(g::DelayTime_create, {fbits(float(s->delayTime))});
    withLambda(addedVt, {me(s)}, [&](Addr fn) {
        uint32_t then = call(g::CallFunc_create, {fn});
        uint32_t seq = call(g::Sequence_create, {move, delay, then, 0});
        call(g::Node_runAction, {row, seq});
    });
    call(g::Vec2_dtor, {target});
    Self::setDropImage(s, row, index);
    call(g::Vec2_dtor, {pos});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ front
// (pushFrontItem $_3's work) insert the item at the front of the list and slide its row in. a = {view, index}.
static void addFrontItem(uint32_t a) {
    Self *s = rw::host_ptr<Self>(ld32(a));
    playSE(SE_ITEM_IN);
    call(g::vector_int_insert, {me(s) + 0x628, s->ids.begin, a + 4});
    call(g::IScrollProtocol_setItemIDSize, {me(s), Self::idCount(s)});
    call(g::IScrollProtocol_updateLayout, {me(s), 0});
    if (s->scrollBarEnabled) call(g::ScrollBar_updateLayout, {s->scrollBar});
    uint32_t row = ld32(s->items.begin + 4);
    slideIn(s, row, int32_t(ld32(a + 4)), VT_FrontAdded);
}
RW_FUNCTION(0x512BAC, "BattleResultScrollView::pushFrontItem add the item (outlined)", addFrontItem);

void Self::pushFrontItem(Self *self, int32_t index) {
    Frame f;
    Addr a = f.alloc(8);
    st32(a, me(self));
    st32(a + 4, uint32_t(index));
    if (idCount(self) == 0) {
        addFrontItem(a);
        return;
    }
    slideRows(self, index, -1.0f, VT_FrontRowArrived);
}
RW_FUNCTION(0x512918, "BattleResultScrollView::pushFrontItem(int)", BattleResultScrollView::pushFrontItem);

// ------------------------------------------------------------------------------------------------ back
// (pushBackItem $_5's work) append the item, scroll to the end and slide its row in. a = {view, index}.
static void addBackItem(uint32_t a) {
    Self *s = rw::host_ptr<Self>(ld32(a));
    playSE(SE_ITEM_IN);
    uint32_t end = s->ids.end;
    if (end == s->ids.cap) {
        call(g::vector_int_pushSlowRef, {me(s) + 0x628, a + 4});
    } else {
        uint32_t e = 0;
        if (end) {
            st32(end, ld32(a + 4));
            e = s->ids.end;
        }
        s->ids.end = e + 4;
    }
    call(g::IScrollProtocol_setItemIDSize, {me(s), Self::idCount(s)});
    call(g::IScrollProtocol_updateLayout, {me(s), 0});
    if (s->scrollBarEnabled) call(g::ScrollBar_updateLayout, {s->scrollBar});
    uint32_t bar = s->scrollBar;
    call(g::ScrollBar_setScrollPosition, {bar, call(g::IScrollProtocol_getMaxPosition, {me(s)})});
    float maxPos = rw::callf(g::IScrollProtocol_getMaxPosition, {me(s)});
    float pixel = 0.0f;
    if (Self::idCount(s) >= 3) pixel = getItemPixel(s);
    s->position = maxPos + pixel;
    uint32_t n = uint32_t(int32_t(s->ids.end - s->ids.begin) >> 2);
    float lim = s->rowCount * 0.5f + 1.0f;
    if (!(float(n) > lim)) lim = float(int32_t(n));
    uint32_t rowsBegin = s->items.begin;
    uint32_t row = ld32(rowsBegin + 4 * uint32_t(f2i(lim)));
    slideIn(s, row, int32_t(ld32(a + 4)), VT_BackAdded);
}
RW_FUNCTION(0x513044, "BattleResultScrollView::pushBackItem add the item (outlined)", addBackItem);

void Self::pushBackItem(Self *self, int32_t index) {
    Frame f;
    Addr a = f.alloc(8);
    st32(a, me(self));
    st32(a + 4, uint32_t(index));
    if (idCount(self) <= 2) {
        addBackItem(a);
        return;
    }
    slideRows(self, index, 1.0f, VT_BackRowArrived);
}
RW_FUNCTION(0x512DAC, "BattleResultScrollView::pushBackItem(int)", BattleResultScrollView::pushBackItem);

// ------------------------------------------------------------------------------------------------ the row lambdas
// $_4 / $_6 [row, position, k, count, view, index]: the row arrived: put it back to its old position (the list
// layout moved underneath); after the last row, add the item.
static void frontRowArrived(uint32_t func) {
    uint32_t row = ld32(func + 4);
    setPositionXY(row, ldf(func + 8), ldf(func + 12));
    if (ld32(func + 0x10) == ld32(func + 0x14)) addFrontItem(func + 0x18);
}
RW_FUNCTION(0x515980, "BattleResultScrollView::pushFrontItem $_4 operator()", frontRowArrived);

static void backRowArrived(uint32_t func) {
    uint32_t row = ld32(func + 4);
    setPositionXY(row, ldf(func + 8), ldf(func + 12));
    if (ld32(func + 0x10) == ld32(func + 0x14)) addBackItem(func + 0x18);
}
RW_FUNCTION(0x5157A0, "BattleResultScrollView::pushBackItem $_6 operator()", backRowArrived);

namespace {
// __func slots of the row lambdas (they hold a cocos2d::Vec2 at +8: copied / destroyed with its constructors)
template <uint32_t VT> struct RowLambda {
    static void copyCaptures(uint32_t dst, uint32_t src) {
        st32(dst, VT);
        st32(dst + 4, ld32(src + 4));
        call(g::Vec2_copy, {dst + 8, src + 8});
        for (uint32_t k = 0x10; k < 0x20; k += 4) st32(dst + k, ld32(src + k));
    }
    static uint32_t destructor(uint32_t self) {
        st32(self, VT);
        call(g::Vec2_dtor, {self + 8});
        return self;
    }
    static void deletingDestructor(uint32_t self) {
        destructor(self);
        call(fn::operator_delete, {self});
    }
    static uint32_t clone(uint32_t self) {
        uint32_t copy = call(fn::operator_new, {ROW_LAMBDA_SIZE});
        if (copy) copyCaptures(copy, self);
        return copy;
    }
    static void cloneInto(uint32_t self, uint32_t dst) {
        if (dst) copyCaptures(dst, self);
    }
    static void destroy(uint32_t self) { call(g::Vec2_dtor, {self + 8}); }
    static void destroyDeallocate(uint32_t self) {
        call(g::Vec2_dtor, {self + 8});
        call(fn::operator_delete, {self});
    }
};
using BackRow = RowLambda<VT_BackRowArrived>;
using FrontRow = RowLambda<VT_FrontRowArrived>;
}  // namespace

RW_FUNCTION(0x515680, "BattleResultScrollView::pushBackItem $_6 __func::~__func", BackRow::destructor);
RW_FUNCTION(0x5156AC, "BattleResultScrollView::pushBackItem $_6 __func::~__func (deleting)", BackRow::deletingDestructor);
RW_FUNCTION(0x5156DC, "BattleResultScrollView::pushBackItem $_6 __func::__clone()", BackRow::clone);
RW_FUNCTION(0x51573C, "BattleResultScrollView::pushBackItem $_6 __func::__clone(__base*)", BackRow::cloneInto);
RW_FUNCTION(0x515780, "BattleResultScrollView::pushBackItem $_6 __func::destroy", BackRow::destroy);
RW_FUNCTION(0x51578A, "BattleResultScrollView::pushBackItem $_6 __func::destroy_deallocate", BackRow::destroyDeallocate);
RW_FUNCTION(0x515860, "BattleResultScrollView::pushFrontItem $_4 __func::~__func", FrontRow::destructor);
RW_FUNCTION(0x51588C, "BattleResultScrollView::pushFrontItem $_4 __func::~__func (deleting)", FrontRow::deletingDestructor);
RW_FUNCTION(0x5158BC, "BattleResultScrollView::pushFrontItem $_4 __func::__clone()", FrontRow::clone);
RW_FUNCTION(0x51591C, "BattleResultScrollView::pushFrontItem $_4 __func::__clone(__base*)", FrontRow::cloneInto);
RW_FUNCTION(0x515960, "BattleResultScrollView::pushFrontItem $_4 __func::destroy", FrontRow::destroy);
RW_FUNCTION(0x51596A, "BattleResultScrollView::pushFrontItem $_4 __func::destroy_deallocate", FrontRow::destroyDeallocate);

// ------------------------------------------------------------------------------------------------ update
// Add the next pending item when none is moving: returns 1 once every item is in the list (and then keeps a
// back-filled list scrolled to its end).
uint32_t Self::updateResultAnimetion(Self *self, float /*dt*/, uint32_t front) {
    uint32_t n = idCount(self);
    uint32_t pendingBegin = self->pending.begin;
    if (n < ((self->pending.end - pendingBegin) >> 2)) {
        if (self->adding || self->dropping) return 0;
        self->adding = 1;
        int32_t index = int32_t(ld32(pendingBegin + 4 * n));
        if (front == 1) pushFrontItem(self, index);
        else pushBackItem(self, index);
        return 0;
    }
    if (n >= 3 && (front ^ 1) == 1) self->position = rw::callf(g::IScrollProtocol_getMaxPosition, {me(self)});
    return 1;
}
RW_FUNCTION(0x513334, "BattleResultScrollView::updateResultAnimetion(float, bool)", BattleResultScrollView::updateResultAnimetion);

// ------------------------------------------------------------------------------------------------ drop image
// A treasure item plays the reward armature over its row (image and treasure box as skins of bones "Layer2" /
// "Layer1"); the name stays empty and the count shows the treasure count until the armature ends ($_8).
void Self::setDropImage(Self *self, uint32_t widget, int32_t index) {
    uint32_t off = RESULT_SIZE * uint32_t(index);
    auto data = [&]() { return self->results.begin + off; };
    if (!ld8(data() + 0x21)) return;
    uint32_t label = search(widget, S_ItemLabel, 0x11);
    withStr(S_Empty, 0, [&](Addr s) { call(g::Text_setString, {label, s}); });
    uint32_t numLabel = search(widget, S_ItemNumLabel, 0x15);
    {
        Frame f;
        Addr buf = stringBuffer(f, {S_CountFmt, ld32(data() + 0x28)});
        uint32_t s = newStr(f, buf, call(g::strlen_, {buf}));
        call(g::Text_setString, {numLabel, s});
        strFree(s);
    }
    uint32_t image = search(widget, S_GetItem, 7);
    vcall(image, node::setVisible, {image, 0});
    uint32_t anim = search(widget, S_AnimNode, 0x10);
    vcall(anim, node::setVisible, {anim, 1});
    uint32_t animation = vcall(anim, V_getAnimation, {anim});
    vcall(animation, V_setSpeedScale, {animation, fbits(float(self->rareDropSpeed))});
    animation = vcall(anim, V_getAnimation, {anim});
    uint32_t play = ld32(ld32(animation) + V_play);
    withStr(S_RewardAnime, 0x13, [&](Addr s) { call(play, {animation, s, 0xFFFFFFFFu, 0xFFFFFFFFu}); });
    playSE(SE_REWARD);
    animation = vcall(anim, V_getAnimation, {anim});
    {
        Frame f;
        Addr fn = f.alloc(24);
        st32(fn + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x20});
        if (heap) {
            st32(heap, VT_DropEnded);
            st32(heap + 4, label);
            st32(heap + 8, me(self));
            st32(heap + 12, uint32_t(index));
            st32(heap + 16, numLabel);
            st32(heap + 20, image);
            st32(heap + 24, anim);
        }
        st32(fn + 16, heap);
        call(g::ArmatureAnimation_setMovementEventCallFunc, {animation, fn});
        functionDestroy(fn);
    }
    self->dropping = 1;
    uint32_t getBone = ld32(ld32(anim) + V_getBone);
    uint32_t bone2 = withStr(S_Layer2, 6, [&](Addr s) { return call(getBone, {anim, s}); });
    uint32_t skin = call(g::Skin_create, {data() + 4});
    uint32_t setScale = ld32(ld32(skin) + V_setScaleXY);
    float scale = ld32(data()) == 13 ? 0.3f : 1.0f;
    call(setScale, {skin, fbits(scale), fbits(scale)});
    call(g::Bone_removeDisplay, {bone2, 0});
    call(g::Bone_addDisplay, {bone2, skin, 0xFFFFFFFFu});
    getBone = ld32(ld32(anim) + V_getBone);
    uint32_t bone1 = withStr(S_Layer1, 6, [&](Addr s) { return call(getBone, {anim, s}); });
    uint32_t box;
    {
        Frame f;
        Addr buf = stringBuffer(f, {S_TreasureFmt, ld32(data() + 0x2C)});
        uint32_t s = newStr(f, buf, call(g::strlen_, {buf}));
        box = call(g::Skin_create, {s});
        strFree(s);
    }
    call(g::Bone_removeDisplay, {bone1, 0});
    call(g::Bone_addDisplay, {bone1, box, 0xFFFFFFFFu});
}
RW_FUNCTION(0x513408, "BattleResultScrollView::setDropImage(cocos2d::ui::Widget*, int)", BattleResultScrollView::setDropImage);

// setDropImage $_8 [label, view, index, numLabel, image, anim]: the reward armature finished (COMPLETE): show
// the name and the amount, the item image again, hide the armature.
static void dropEnded(uint32_t func, const uint32_t * /*armature*/, const int32_t *type, uint32_t /*movementId*/) {
    if (*type != 1) return;
    Self *s = rw::host_ptr<Self>(ld32(func + 8));
    uint32_t off = Self::RESULT_SIZE * ld32(func + 12);
    call(g::Text_setString, {ld32(func + 4), s->results.begin + off + 0x10});
    uint32_t numLabel = ld32(func + 16);
    {
        Frame f;
        Addr buf = stringBuffer(f, {S_CountFmt, ld32(s->results.begin + Self::RESULT_SIZE * ld32(func + 12) + 0x30)});
        uint32_t str = newStr(f, buf, call(g::strlen_, {buf}));
        call(g::Text_setString, {numLabel, str});
        strFree(str);
    }
    uint32_t image = ld32(func + 20);
    vcall(image, node::setVisible, {image, 1});
    uint32_t anim = ld32(func + 24);
    vcall(anim, node::setVisible, {anim, 0});
    s->dropping = 0;
}
RW_FUNCTION(0x51553C, "BattleResultScrollView::setDropImage $_8 operator()", dropEnded);
