// SupportCharacterScrollView: the row touch lambdas of registItemTouchEvent (batch 3, named from their __func
// vtables' target_type). `functor` is the __func object: vtable at +0, the view at +4, the row index at +8.
#include "SceneUi.hpp"

using namespace scene;

namespace {

constexpr uint32_t SCV_ROWS = 0x51C;                          // the row widgets (array)
constexpr uint32_t SCV_ROW_HEIGHT = 0x518, SCV_SCROLL = 0x540;  // floats: row height (plus a spacing), scroll position
constexpr uint32_t SCV_USER_IDS = 0x628;                      // std::vector<uint64> of the listed users
constexpr uint32_t SCV_ON_OTHER = 0x638, SCV_ON_SELF = 0x650;    // std::function<void(int)>
constexpr uint32_t SCV_PRESSED = 0x66C, SCV_SELECTED = 0x670, SCV_BUSY = 0x674, SCV_TOUCH_Y = 0x678;
constexpr uint32_t SCV_IN_RIGHT = 0x67C, SCV_HOLD = 0x680, SCV_USER = 0x688;   // SupportUser copy (0x124 bytes)
constexpr uint32_t SCV_SELF_ID = 0x7B0;                       // uint64
constexpr uint32_t API_SUPPORT_LIST = 0x134;                  // shared_ptr<StageSupportList> (SafeBuffer of users)
constexpr uint32_t USER_SIZE = 0x128;

constexpr uint32_t SupportIndex_find = 0x490275;              // unordered_map<uint64, int>::find (const)
constexpr uint32_t SafeBuffer_read = 0x45A269;                // (buffer, dst, src, n, index)
constexpr uint32_t FunctionVI_call = 0x4F321D;                // std::function<void(int)>::operator()(int)
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5;   // (view, bool)
constexpr uint32_t Touch_getLocation = 0x900E57;              // (Vec2* out, Touch*)
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;        // (Vec2* out, node, const Vec2&)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor = 0x8DF18B;                      // (Rect*, x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;
constexpr uint32_t DIVSI3 = 0xCD5E8D;
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t SupportCancel_body = 0x50495D;             // sub_50495C(captures, touch): the press is cancelled
constexpr uint32_t GOT_ROW_SPACING = 0xFD659C;                // &a float added to the row height
constexpr uint32_t S_MAIN_PANEL = 0xDFD1FC, S_MEDAL = 0xDFD05F;

constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;         // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;   // (node, std::function<void()>)
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t VT_touchLambda = 0xF55E40;                 // [3 words]
constexpr uint32_t VT_tapDone = 0xF55E6C;                     // [word]
constexpr uint32_t S_SUPPORT_PANEL = 0xDF8F8B;
constexpr uint32_t SCV_PRESS_Y = 0x678;

uint32_t userCount(uint32_t self) {
    return uint32_t(int32_t(ld32(self + SCV_USER_IDS + 4) - ld32(self + SCV_USER_IDS)) >> 3);
}

// the user record of the listed user `index`, from the support list (SafeBuffer::read into `dst`)
void readSupportUser(uint32_t sb, uint32_t self, uint32_t index, uint32_t dst, uint32_t idSlot) {
    const uint32_t begin = ld32(self + SCV_USER_IDS);
    st32(idSlot, ld32(begin + index * 8));
    st32(idSlot + 4, ld32(begin + index * 8 + 4));
    const uint32_t node = call(SupportIndex_find, {sb + 0x24, idSlot});
    const uint32_t offset = node ? ld32(node + 0x10) * USER_SIZE : 0xFFFFFED8u;
    call(SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + offset, USER_SIZE, offset});
}

// is the touch inside `node`'s content rect? `local` receives the point in node space (destroyed by the caller)
bool touchIn(uint32_t node, uint32_t touch, uint32_t local) {
    Frame f;
    const uint32_t at = f.alloc(8);
    call(Touch_getLocation, {at, touch});
    call(Node_convertToNodeSpace, {local, node, at});
    call(Vec2_dtor, {at});
    const float w = ldf(vcall(node, node::getContentSize, {node}));
    const float h = ldf(vcall(node, node::getContentSize, {node}) + 4);
    const uint32_t rect = f.alloc(16);
    call(Rect_ctor, {rect, 0, 0, fbits(w), fbits(h)});
    const bool in = call(Rect_containsPoint, {rect, local}) != 0;
    call(Vec2_dtor, {rect});
    return in;
}

int32_t sdiv(int32_t a, int32_t b) {
    if (b == 0) return int32_t(call(DIVSI3, {uint32_t(a), 0}));
    if (b == -1) return int32_t(0u - uint32_t(a));
    return a / b;
}

}  // namespace

struct SupportCharacterScrollViewBatch3 {
    // sub_504820(function<void(Touch*, Event*)>* target, words[3]): target = lambda [words] (by swap)
    static void setTouchLambda(uint32_t target, uint32_t words) {
        Frame f;
        const uint32_t fo = newFunction(f, VT_touchLambda, {ld32(words), ld32(words + 4), ld32(words + 8)});
        call(FunctionTouchVoid_swap, {fo, target});
        functionDestroy(fo);
    }

    // sub_50495C(captures [view, row, word], touch): released without moving (less than 20 points): the row is
    // busy and its support panel plays the tap animation
    static void tapped(uint32_t captures, uint32_t touch) {
        const uint32_t self = ld32(captures);
        auto clear = [&]() {
            st8(self + SCV_PRESSED, 0);
            st8(self + SCV_IN_RIGHT, 0);
        };
        if (!ld8(self + SCV_PRESSED)) return clear();
        Frame f;
        const uint32_t at = f.alloc(8);
        call(Touch_getLocation, {at, touch});
        const float dy = ldf(at + 4) - ldf(self + SCV_PRESS_Y);
        if (!(bitsf(fbits(dy) & 0x7FFFFFFFu) < 20.0f)) {
            call(Vec2_dtor, {at});
            return clear();
        }
        const uint32_t index = ld32(self + SCV_SELECTED);
        if (!(userCount(self) > index)) call(THROW_OUT_OF_RANGE, {self + SCV_USER_IDS});
        const uint32_t lo = ld32(ld32(self + SCV_USER_IDS) + index * 8), hi = ld32(ld32(self + SCV_USER_IDS) + index * 8 + 4);
        call(Vec2_dtor, {at});
        if ((hi | lo) == 0) return clear();
        st8(self + SCV_BUSY, 1);
        const uint32_t row = ld32(ld32(self + SCV_ROWS) + ld32(captures + 4) * 4);
        const uint32_t panel = findNode(row, S_SUPPORT_PANEL, 13);
        if (!panel) return;
        const uint32_t done = newFunction(f, VT_tapDone, {ld32(captures + 8)});
        call(Node_runAction, {panel, call(SceneUtil_createTapAnimation, {panel, done})});
        functionDestroy(done);
    }

    // sub_50491A: a touch lambda's operator()(Touch*, Event*) - the captures follow the vtable; the body is void
    static void tappedSlot(uint32_t functor, uint32_t touchPtr) { tapped(functor + 4, ld32(touchPtr)); }

    // $_4 [this]: released - the self / other callback for the selected row, then the layout
    static void released(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        if (ld8(self + SCV_PRESSED)) {
            const uint32_t api = call(fn::APIManager_getInstance, {});
            const uint32_t ctrl = ld32(api + API_SUPPORT_LIST + 4), sb = ld32(api + API_SUPPORT_LIST);
            if (ctrl) call(fn::shared_add, {ctrl});
            const uint32_t index = ld32(self + SCV_SELECTED);
            if (!(userCount(self) > index)) call(THROW_OUT_OF_RANGE, {self + SCV_USER_IDS});
            Frame f;
            const uint32_t id = f.alloc(8), user = f.alloc(USER_SIZE);
            readSupportUser(sb, self, index, user, id);
            const bool mine = ((ld32(user + 4) ^ ld32(self + SCV_SELF_ID + 4)) |
                               (ld32(user) ^ ld32(self + SCV_SELF_ID))) == 0;
            if (mine) {
                if (ld32(self + SCV_ON_SELF + 16)) call(FunctionVI_call, {self + SCV_ON_SELF, ld32(self + SCV_SELECTED)});
            } else if (ld32(self + SCV_ON_OTHER + 16)) {
                call(FunctionVI_call, {self + SCV_ON_OTHER, ld32(self + SCV_SELECTED)});
            }
            st32(self + SCV_SELF_ID, ld32(self + SCV_USER));
            st32(self + SCV_SELF_ID + 4, ld32(self + SCV_USER + 4));
            call(IScrollProtocol_updateLayout, {self, 0});
            if (ctrl) call(fn::shared_release, {ctrl});
        }
        st8(self + SCV_BUSY, 0);
        st8(self + SCV_PRESSED, 0);
        st8(self + SCV_IN_RIGHT, 0);
    }

    // $_2 [this, row, ...](Touch*, Event*): moved off the row's main panel - the press is cancelled
    static void moved(uint32_t functor, uint32_t touchPtr) {
        const uint32_t touch = ld32(touchPtr);
        const uint32_t self = ld32(functor + 4);
        if (!ld8(self + SCV_PRESSED)) return;
        const uint32_t row = ld32(ld32(self + SCV_ROWS) + ld32(functor + 8) * 4);
        const uint32_t panel = findNode(row, S_MAIN_PANEL, 9);
        if (!panel) return;
        Frame f;
        const uint32_t local = f.alloc(8);
        if (!touchIn(panel, touch, local)) {
            st8(self + SCV_PRESSED, 0);
            st8(self + SCV_IN_RIGHT, 0);
            call(SupportCancel_body, {functor + 0xC, touch});
        }
        call(Vec2_dtor, {local});
    }

    // $_1 [this, row, ...](Touch*, Event*) -> bool: a press on the row (not on its medal) selects its user: the
    // record is copied into the view, and whether the press is on the right part of the panel is kept
    static uint32_t pressed(uint32_t functor, uint32_t touchPtr) {
        const uint32_t touch = ld32(touchPtr);
        const uint32_t self = ld32(functor + 4);
        if (ld8(self + SCV_BUSY)) return 0;
        {
            Frame f;
            const uint32_t at = f.alloc(8);
            call(Touch_getLocation, {at, touch});
            st32(self + SCV_TOUCH_Y, ld32(at + 4));
            call(Vec2_dtor, {at});
        }
        const uint32_t row = ld32(ld32(self + SCV_ROWS) + ld32(functor + 8) * 4);
        const uint32_t panel = findNode(row, S_MAIN_PANEL, 9);
        if (!panel) return 0;
        Frame f;
        const uint32_t local = f.alloc(8);
        uint32_t result = 0;
        if (touchIn(panel, touch, local)) {
            const uint32_t medal = findNode(row, S_MEDAL, 5);
            const uint32_t medalLocal = f.alloc(8);
            if (!touchIn(medal, touch, medalLocal)) {
                const float rowHeight = ldf(self + SCV_ROW_HEIGHT) + ldf(ld32(GOT_ROW_SPACING));
                const int32_t scroll = f2i(ldf(self + SCV_SCROLL));
                const int32_t rows = sdiv(scroll, f2i(rowHeight));
                uint32_t index = uint32_t(rows) + ld32(functor + 8) - 1;
                st32(self + SCV_SELECTED, index);
                if (!(userCount(self) > index)) st32(self + SCV_SELECTED, userCount(self) - 1);
                const uint32_t api = call(fn::APIManager_getInstance, {});
                const uint32_t ctrl = ld32(api + API_SUPPORT_LIST + 4), sb = ld32(api + API_SUPPORT_LIST);
                if (ctrl) call(fn::shared_add, {ctrl});
                index = ld32(self + SCV_SELECTED);
                if (!(userCount(self) > index)) call(THROW_OUT_OF_RANGE, {self + SCV_USER_IDS});
                const uint32_t id = f.alloc(8), user = f.alloc(USER_SIZE);
                readSupportUser(sb, self, index, user, id);
                call(MEMCPY, {self + SCV_USER, user, 0x124});
                const float x = ldf(vcall(panel, node::getContentSize, {panel})) + -150.0f;
                const float w = ldf(vcall(panel, node::getContentSize, {panel}));
                const float h = ldf(vcall(panel, node::getContentSize, {panel}) + 4);
                const uint32_t right = f.alloc(16);
                call(Rect_ctor, {right, fbits(x), 0, fbits(w), fbits(h)});
                st8(self + SCV_IN_RIGHT, uint8_t(call(Rect_containsPoint, {right, local})));
                st8(self + SCV_PRESSED, 1);
                st32(self + SCV_HOLD, 0);
                call(Vec2_dtor, {right});
                if (ctrl) call(fn::shared_release, {ctrl});
                result = 1;
            }
            call(Vec2_dtor, {medalLocal});
        }
        call(Vec2_dtor, {local});
        return result;
    }
};

using L = SupportCharacterScrollViewBatch3;
RW_FUNCTION(0x504B78, "SupportCharacterScrollView::registItemTouchEvent::$_4::operator()", L::released);
RW_FUNCTION(0x504DA4, "SupportCharacterScrollView::registItemTouchEvent::$_2::operator()", L::moved);
RW_FUNCTION(0x504FB0, "SupportCharacterScrollView::registItemTouchEvent::$_1::operator()", L::pressed);
RW_FUNCTION(0x504820, "SupportCharacterScrollView (sub_504820): set a touch lambda", L::setTouchLambda);
RW_FUNCTION(0x50495C, "SupportCharacterScrollView (sub_50495C): tapped", L::tapped);
RW_FUNCTION(0x50491A, "SupportCharacterScrollView (sub_50491A): the tapped lambda operator()", L::tappedSlot);
