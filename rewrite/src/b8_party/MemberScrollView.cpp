// Batch #8: MemberScrollView's row touch lambdas (the party member list). A row tap: touch began inside the row
// marks it (+0x6F9) and keeps the touch's y (+0x704); moving out of the row, or a tap ending, plays the tap
// animation of the row under the touch (when it holds a member); the member button opens the member after a short
// no-tap screen.
//
// MemberScrollView (BaseScrollView<unsigned long long>): +0x51C row widgets, +0x518 row height, +0x538 columns,
// +0x540 the scroll offset, +0x628 vector<ull> member ids, +0x6B0 std::function (on tap), +0x6E0 the tapped id,
// +0x6F9 touch inside a row, +0x6FC the tapped index, +0x700 tap running, +0x704 the touch's y.
#include "../scene/ScrollRowTouch.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t FunctionTouch_swap = 0x4F3699;   // function<void(Touch*, Event*)>::swap
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_openNoTapScreen = 0x61FB25;
}  // namespace g

constexpr uint32_t VT_TouchLambda = 0xF56CDC;      // {4 words} (heap 0x14)
constexpr uint32_t VT_TapDone = 0xF56D08;          // the tap animation's end {2 words}
constexpr uint32_t VT_OpenMember = 0xF56E10;       // {view}
constexpr uint32_t F_ON_TAP = 0x6B0, F_TAPPED_ID = 0x6E0, F_INSIDE = 0x6F9, F_INDEX = 0x6FC, F_RUNNING = 0x700;
constexpr uint32_t F_TOUCH_Y = 0x704;
constexpr uint32_t SM_NO_TAP_TIME = 0x300;
constexpr uint32_t V_IS_VISIBLE = 0xB4;

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

constexpr RowTouch ROW_TOUCH = {F_INSIDE, F_INDEX, F_RUNNING, F_TOUCH_Y, VT_TapDone, 8};

uint32_t tapRow(uint32_t captures, uint32_t touch) { return rowTap(ROW_TOUCH, captures, touch); }
uint32_t touchMoved(uint32_t self, uint32_t touchp) { return rowTouchMoved(ROW_TOUCH, self, touchp, 0x523631); }
uint32_t touchBegan(uint32_t self, uint32_t touchp) { return rowTouchBegan(ROW_TOUCH, self, touchp, false); }

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
