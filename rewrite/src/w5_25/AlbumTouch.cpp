// Album (MENU > Other > Album): the cell touch lambdas of AlbumScrollView::registItemTouchEvent. Began marks a
// press that starts on the cell (and remembers the touch's y), moved cancels it once the finger leaves the cell,
// ended (sub_50C310, shared by moved and the ended / cancelled lambdas) checks the finger did not scroll more than
// 20 points, looks the medal up, flags whether the user owns it and plays the tap animation, whose lambda $_3
// opens the medal (AlbumScrollView's tap callback).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Touch_getLocation = 0x900E57;          // (Vec2 out, touch)
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;    // (Vec2 out, node, const Vec2&)
constexpr uint32_t Rect_ctor = 0x8DF18B;                  // (out, x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Node_stopAllActions = 0x893815;
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;   // (node, std::function<void()> by value)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;  // (sret, mgr, u64 user medal id, int, Medal)
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t fadd = 0xCD6BD0, fsub = 0xCD6BCC, fcmplt = 0xCD71B8, fixsfsi = 0xCD7240, divsi3 = 0xCD5E8D;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;             // vector::at (does not return)
}  // namespace g

constexpr uint32_t ITEM_MARGIN = 0xE1F834;     // IScrollProtocol::ITEM_MARGIN (float)

// AlbumScrollView fields
constexpr uint32_t F_ITEM_H = 0x518;           // float
constexpr uint32_t F_ROWS = 0x51C;             // vector<Widget*>: the cells on screen
constexpr uint32_t F_COLUMNS = 0x538;          // cells per line
constexpr uint32_t F_SCROLL_Y = 0x540;         // float
constexpr uint32_t F_ITEM_IDS = 0x628;         // vector<int>
constexpr uint32_t F_OWNED = 0x654;            // vector<int>: the user's album (medal ids)
constexpr uint32_t F_MEDALS = 0x660;           // vector<int>: medal ids in album order
constexpr uint32_t F_PRESSED = 0x66D;          // a press started on a cell
constexpr uint32_t F_TAPPED = 0x670;           // index of the tapped medal in F_MEDALS
constexpr uint32_t F_OWNED_TAP = 0x674;        // the tapped medal is in the user's album
constexpr uint32_t F_TOUCH_Y = 0x678;          // y where the press started

constexpr uint32_t VT_TapDone = 0xF561DC;      // tap animation lambda $_3 {this, index}
constexpr uint32_t MEDAL_SB = 0x58;            // UserMedalData: master::Medal SafeBuffer

uint32_t cell(uint32_t self, uint32_t index) { return ld32(ld32(self + F_ROWS) + 4 * index); }

// the touch, in the cell's space, is inside the cell's rectangle
bool touchInCell(uint32_t touch, uint32_t node) {
    Frame f;
    uint32_t loc = f.alloc(8), local = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {local, node, loc});
    call(g::Vec2_dtor, {loc});
    uint32_t w = ld32(vcall(node, node::getContentSize, {node}));
    uint32_t h = ld32(vcall(node, node::getContentSize, {node}) + 4);
    call(g::Rect_ctor, {rect, 0, 0, w, h});
    bool in = call(g::Rect_containsPoint, {rect, local}) != 0;
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {local});
    return in;
}

uint32_t vectorSize(uint32_t v) { return (ld32(v + 4) - ld32(v)) >> 2; }

struct AlbumTouch {
    // $_0 (began) {this, index}: claims the touch when it starts on the cell, unless a tap is being handled
    static uint32_t began(uint32_t functor, uint32_t touchRef) {
        uint32_t touch = ld32(touchRef);
        uint32_t self = ld32(functor + 4);
        if (ld8(self + F_OWNED_TAP)) return 0;
        {
            Frame f;
            uint32_t loc = f.alloc(8);
            call(g::Touch_getLocation, {loc, touch});
            st32(self + F_TOUCH_Y, ld32(loc + 4));
            call(g::Vec2_dtor, {loc});
        }
        if (!touchInCell(touch, cell(self, ld32(functor + 8)))) return 0;
        st8(self + F_PRESSED, 1);
        return 1;
    }

    // $_1 (moved) {this, index, this, index, this, index}: leaving the cell ends the press
    static void moved(uint32_t functor, uint32_t touchRef) {
        uint32_t touch = ld32(touchRef);
        uint32_t self = ld32(functor + 4);
        if (!ld8(self + F_PRESSED)) return;
        if (touchInCell(touch, cell(self, ld32(functor + 8)))) return;
        st8(self + F_PRESSED, 0);
        ended(functor + 0xC, touch);
    }

    // the tap (captures {this, index, this, index}): the medal under the finger, if the press did not scroll
    static void ended(uint32_t captures, uint32_t touch) {
        uint32_t self = ld32(captures);
        float lineH = bitsf(call(g::fadd, {ld32(self + F_ITEM_H), ld32(ITEM_MARGIN)}));
        int32_t scrolled = int32_t(call(g::fixsfsi, {ld32(self + F_SCROLL_Y)}));
        int32_t line = int32_t(call(g::divsi3, {uint32_t(scrolled), call(g::fixsfsi, {fbits(lineH)})}));
        uint32_t tapped = ld32(captures + 4) + uint32_t(line) * ld32(self + F_COLUMNS) - 1;
        st32(self + F_TAPPED, tapped);
        if (vectorSize(self + F_ITEM_IDS) <= tapped || !ld8(self + F_PRESSED)) {
            st8(self + F_PRESSED, 0);
            return;
        }
        {
            Frame f;
            uint32_t loc = f.alloc(8);
            call(g::Touch_getLocation, {loc, touch});
            uint32_t dy = call(g::fsub, {ld32(loc + 4), ld32(self + F_TOUCH_Y)}) & 0x7FFFFFFF;
            if (!call(g::fcmplt, {dy, fbits(20.0f)})) {
                call(g::Vec2_dtor, {loc});
                st8(self + F_PRESSED, 0);
                return;
            }
            if (vectorSize(self + F_MEDALS) <= ld32(self + F_TAPPED)) call(g::ThrowOutOfRange, {self + F_MEDALS});
            uint32_t medal = ld32(ld32(self + F_MEDALS) + 4 * ld32(self + F_TAPPED));
            call(g::Vec2_dtor, {loc});
            if (!medal) {
                st8(self + F_PRESSED, 0);
                return;
            }
        }
        if (vectorSize(self + F_MEDALS) <= ld32(self + F_TAPPED)) call(g::ThrowOutOfRange, {self + F_MEDALS});
        uint32_t medalId = ld32(ld32(self + F_MEDALS) + 4 * ld32(self + F_TAPPED));
        // the original frame: getMedalData takes an empty UserMedal::Medal by value at [sp + 8], whose bytes
        // 0x2C..0x30 the original never clears (stale stack)
        guest::Frame frame(0x100);
        uint32_t sp = frame.addr();
        uint32_t mgr = call(g::MedalDataManager_getInstance, {});
        uint32_t record = sp + 0x98;
        call(g::aeabi_memset, {record, 0x2C, 0});
        call(g::aeabi_memset, {record + 0x30, 0x18, 0});
        *rw::frame_ptr<uint32_t>(sp) = medalId;
        for (uint32_t k = 0; k < 0x48; k += 4) *rw::frame_ptr<uint32_t>(sp + 8 + k) = ld32(record + k);
        uint32_t data = sp + 0x6C;               // shared_ptr<UserMedalData>
        rw::call_exact(g::MedalDataManager_getMedalData, {data, mgr, 0, 0});
        // owned: the album holds the medal's master id (read from the medal's master row, position 0)
        for (uint32_t i = 0; ld32(self + F_OWNED + 4) != ld32(self + F_OWNED) && i < vectorSize(self + F_OWNED);) {
            uint32_t owned = ld32(ld32(self + F_OWNED) + 4 * i);
            uint32_t sb = ld32(ld32(data) + MEDAL_SB);
            *rw::frame_ptr<uint32_t>(sp) = 0;
            rw::call_exact(g::SafeBuffer_read, {sb, sp + 0x7C, ld32(sb + 0xC), 4});
            ++i;
            if (owned == ld32(sp + 0x7C)) {
                st8(self + F_OWNED_TAP, 1);
                break;
            }
        }
        uint32_t node = cell(self, ld32(captures + 4));
        vcall(node, node::setLocalZOrder, {node, 1});
        call(g::Node_stopAllActions, {node});
        vcall(node, node::setScale, {node, fbits(1.0f)});
        uint32_t done = sp + 0x80;               // std::function: $_3 {this, index} inline
        st32(done, VT_TapDone);
        st32(done + 4, ld32(captures + 8));
        st32(done + 8, ld32(captures + 0xC));
        st32(done + 16, done);
        call(fn::Node_runAction, {node, call(g::SceneUtil_createTapAnimation, {node, done})});
        functionDestroy(done);
        sharedRelease(data);
    }
};
}  // namespace

RW_FUNCTION(0x50C924, "AlbumScrollView::registItemTouchEvent $_0 (began) __func::operator() (sub_50C924)",
            AlbumTouch::began);
RW_FUNCTION(0x50C780, "AlbumScrollView::registItemTouchEvent $_1 (moved) __func::operator() (sub_50C780)",
            AlbumTouch::moved);
RW_FUNCTION(0x50C310, "AlbumScrollView::registItemTouchEvent tap end (sub_50C310)", AlbumTouch::ended);
