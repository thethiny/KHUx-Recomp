// Batch #8: MemberScrollView's row touch lambdas (the party member list). A row tap: touch began inside the row
// marks it (+0x6F9) and keeps the touch's y (+0x704); moving out of the row, or a tap ending, plays the tap
// animation of the row under the touch (when it holds a member); the member button opens the member after a short
// no-tap screen.
//
// MemberScrollView (BaseScrollView<unsigned long long>): +0x51C row widgets, +0x518 row height, +0x538 columns,
// +0x540 the scroll offset, +0x628 vector<ull> member ids, +0x6B0 std::function (on tap), +0x6E0 the tapped id,
// +0x6F9 touch inside a row, +0x6FC the tapped index, +0x700 tap running, +0x704 the touch's y.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t FunctionTouch_swap = 0x4F3699;   // function<void(Touch*, Event*)>::swap
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation, Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;
constexpr uint32_t VectorAt_throw = 0x47CCE1;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_openNoTapScreen = 0x61FB25;
constexpr uint32_t GOT_ROW_SPACING = 0xFD659C;
}  // namespace g

constexpr uint32_t VT_TouchLambda = 0xF56CDC;      // {4 words} (heap 0x14)
constexpr uint32_t VT_TapDone = 0xF56D08;          // the tap animation's end {2 words}
constexpr uint32_t VT_OpenMember = 0xF56E10;       // {view}
constexpr uint32_t F_ROWS = 0x51C, F_ROW_HEIGHT = 0x518, F_COLUMNS = 0x538, F_OFFSET = 0x540, F_IDS = 0x628;
constexpr uint32_t F_ON_TAP = 0x6B0, F_TAPPED_ID = 0x6E0, F_INSIDE = 0x6F9, F_INDEX = 0x6FC, F_RUNNING = 0x700;
constexpr uint32_t F_TOUCH_Y = 0x704;
constexpr uint32_t SM_NO_TAP_TIME = 0x300;
constexpr uint32_t V_IS_VISIBLE = 0xB4, V_SET_LOCAL_Z = 0x0C;

// function<void(Touch*, Event*)> = lambda{4 words}: a heap __func swapped in, the old target destroyed
uint32_t assignTouch(uint32_t fn, uint32_t lambda) {
    Frame f;
    const uint32_t tmp = f.alloc(20);
    st32(tmp + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x14});
    if (heap) {
        st32(heap, VT_TouchLambda);
        for (uint32_t k = 0; k < 0x10; k += 4) st32(heap + 4 + k, ld32(lambda + k));
    }
    st32(tmp + 16, heap);
    call(g::FunctionTouch_swap, {tmp, fn});
    functionDestroy(tmp);
    return 0;
}

// the tap {view, row, 2 words}(touch): the touched index from the row and the scroll position; a member there and
// the touch still near its start: the row's tap animation
uint32_t tapRow(uint32_t captures, uint32_t touch) {
    const uint32_t view = ld32(captures);
    const float rowStep = ldf(view + F_ROW_HEIGHT) + ldf(ld32(g::GOT_ROW_SPACING));
    const int32_t rows = f2i(ldf(view + F_OFFSET)) / f2i(rowStep);
    const uint32_t index = ld32(captures + 4) + ld32(view + F_COLUMNS) * uint32_t(rows) - 1;
    st32(view + F_INDEX, index);
    const uint32_t count = (ld32(view + F_IDS + 4) - ld32(view + F_IDS)) >> 3;
    if (count > index && ld8(view + F_INSIDE)) {
        Frame f;
        const uint32_t loc = f.alloc(8);
        call(g::Touch_getLocation, {loc, touch});
        if (std::fabs(ldf(loc + 4) - ldf(view + F_TOUCH_Y)) < 10.0f) {
            const uint32_t at = ld32(view + F_INDEX);
            if ((ld32(view + F_IDS + 4) - ld32(view + F_IDS)) >> 3 <= at) call(g::VectorAt_throw, {view + F_IDS});
            const uint32_t entry = ld32(view + F_IDS) + 8 * at;
            const uint32_t lo = ld32(entry), hi = ld32(entry + 4);
            call(g::Vec2_dtor, {loc});
            if (lo | hi) {
                st8(view + F_RUNNING, 1);
                const uint32_t row = ld32(ld32(view + F_ROWS) + 4 * ld32(captures + 4));
                vcall(row, V_SET_LOCAL_Z, {row, 1});
                Frame h;
                const uint32_t done = newFunction(h, VT_TapDone, {ld32(captures + 8), ld32(captures + 0xC)});
                const uint32_t action = call(g::SceneUtil_createTapAnimation, {row, done});
                call(fn::Node_runAction, {row, action});
                functionDestroy(done);
                return 0;
            }
        } else {
            call(g::Vec2_dtor, {loc});
        }
    }
    st8(view + F_INSIDE, 0);
    return 0;
}

// is the touch inside row `index`'s content rect (node space)
bool inside(uint32_t row, uint32_t touch) {
    Frame f;
    const uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, row, loc});
    call(g::Vec2_dtor, {loc});
    const uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
    const uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
    call(g::Rect_ctor, {rect, 0, 0, w, h});
    const bool in = call(g::Rect_containsPoint, {rect, p}) != 0;
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {p});
    return in;
}

// touch moved {-, view, row, tap captures (+0xC)}(touch*): leaving the row ends the tap
uint32_t touchMoved(uint32_t self, uint32_t touchp) {
    const uint32_t touch = ld32(touchp);
    const uint32_t view = ld32(self + 4);
    if (!ld8(view + F_INSIDE)) return 0;
    const uint32_t row = ld32(ld32(view + F_ROWS) + 4 * ld32(self + 8));
    Frame f;
    const uint32_t loc = f.alloc(8), p = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {p, row, loc});
    call(g::Vec2_dtor, {loc});
    const uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
    const uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
    call(g::Rect_ctor, {rect, 0, 0, w, h});
    if (!call(g::Rect_containsPoint, {rect, p})) {
        st8(view + F_INSIDE, 0);
        tapRow(self + 0xC, touch);
    }
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {p});
    return 0;
}

// touch began {-, view, row}(touch*): the touch's y kept; inside the row: marked, 1
uint32_t touchBegan(uint32_t self, uint32_t touchp) {
    const uint32_t view = ld32(self + 4);
    if (ld8(view + F_RUNNING)) return 0;
    const uint32_t touch = ld32(touchp);
    {
        Frame f;
        const uint32_t loc = f.alloc(8);
        call(g::Touch_getLocation, {loc, touch});
        st32(view + F_TOUCH_Y, ld32(loc + 4));
        call(g::Vec2_dtor, {loc});
    }
    const uint32_t row = ld32(ld32(view + F_ROWS) + 4 * ld32(self + 8));
    if (!inside(row, touch)) return 0;
    st8(view + F_INSIDE, 1);
    return 1;
}

// the member button {-, -, button, view, member id}(sender, type): both visible and a tap callback set: the id
// kept, a no-tap screen, then the member opened after the same delay
uint32_t memberButton(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    const uint32_t button = ld32(self + 8), view = ld32(self + 0xC);
    if (vcall(button, V_IS_VISIBLE, {button}) != 1) return 0;
    if (vcall(view, V_IS_VISIBLE, {view}) != 1) return 0;
    call(g::AudioPlaySystemSEDecide, {});
    if (!ld32(view + F_ON_TAP + 0x10)) return 0;
    st32(view + F_TAPPED_ID + 4, ld32(self + 0x14));
    st32(view + F_TAPPED_ID, ld32(self + 0x10));
    const uint32_t sm = sceneManager();
    call(g::SM_openNoTapScreen, {sm, ld32(sceneManager() + SM_NO_TAP_TIME)});
    const uint32_t delay = call(fn::DelayTime_create, {ld32(sceneManager() + SM_NO_TAP_TIME)});
    Frame f;
    const uint32_t fo = newFunction(f, VT_OpenMember, {view});
    const uint32_t then = call(fn::CallFunc_create, {fo});
    call(fn::Node_runAction, {view, call(fn::Sequence_create, {delay, then, 0})});
    functionDestroy(fo);
    return 0;
}
}  // namespace

RW_FUNCTION(0x5234C0, "MemberScrollView touch lambda assign (sub_5234C0)", assignTouch);
RW_FUNCTION(0x523630, "MemberScrollView row tap (sub_523630)", tapRow);
RW_FUNCTION(0x523974, "MemberScrollView touch moved (sub_523974)", touchMoved);
RW_FUNCTION(0x523B18, "MemberScrollView touch began (sub_523B18)", touchBegan);
RW_FUNCTION(0x523F9C, "MemberScrollView member button (sub_523F9C)", memberButton);
