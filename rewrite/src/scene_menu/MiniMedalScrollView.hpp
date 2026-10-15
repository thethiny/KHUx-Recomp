// MiniMedalScrollView: the medal grid of the deck editor's medal picker (and the fusion material picker). A
// BaseScrollView<unsigned long long> (IScrollProtocol, itself a cocos2d::ui::Layout) whose items are medal ids;
// every cell holds a DeckMedalView. Medals can be tapped (onTapItem) or dragged out of the list onto a target
// (onDropItem / getDropTarget); some ids are shown greyed out (disabled / already used / without effect).
#pragma once

#include "../scene/SceneUi.hpp"

// std::vector<unsigned long long>
struct MedalIdVector {
    rw::Field<uint32_t> begin;   // +0
    rw::Field<uint32_t> end;     // +4
    rw::Field<uint32_t> cap;     // +8
};
RW_SIZE(MedalIdVector, 12);

struct MiniMedalScrollView {
    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x700;

    // ---- cocos2d::ui::Layout / IScrollProtocol
    rw::Field<uint32_t> vtable;              // +0x000
    const uint8_t pad_004[0x27C];
    rw::Field<uint32_t> vtable2;             // +0x280
    const uint8_t pad_284[0xCC];
    rw::Field<uint32_t> vtable3;             // +0x350
    const uint8_t pad_354[0x1A8];
    rw::Field<float> field_4FC;              // +0x4FC 0.1
    rw::Field<float> field_500;              // +0x500 0.3
    rw::Field<uint32_t> field_504;           // +0x504
    rw::Field<uint32_t> field_508;           // +0x508
    rw::Field<uint32_t> field_50C;           // +0x50C
    rw::Field<float> field_510;              // +0x510 200.0
    rw::Field<float> itemWidth;              // +0x514 cell width (120)
    rw::Field<float> itemHeight;             // +0x518 cell height (164)
    rw::Field<uint32_t> rowsBegin;           // +0x51C std::vector<ui::Widget*> row widgets
    rw::Field<uint32_t> rowsEnd;             // +0x520
    const uint8_t pad_524[0x14];
    rw::Field<int32_t> maxColumn;            // +0x538 cells per row
    const uint8_t pad_53C[4];
    rw::Field<float> scrollOffset;           // +0x540 scrolled distance (pixels)
    const uint8_t pad_544[0x2C];
    rw::Field<float> field_570;              // +0x570
    const uint8_t pad_574[8];
    rw::Field<float> field_57C;              // +0x57C
    const uint8_t pad_580[0xA8];
    // ---- BaseScrollView<unsigned long long>
    MedalIdVector itemIds;                   // +0x628 the medal ids shown
    const uint8_t pad_634[4];
    // ---- MiniMedalScrollView
    const uint8_t onTapItem[20];                   // +0x638 std::function<void(unsigned long long, int)>
    const uint8_t pad_64C[4];
    const uint8_t onDropItem[20];                  // +0x650 std::function<void(int, unsigned long long)>
    const uint8_t pad_664[4];
    const uint8_t getDropTarget[20];               // +0x668 std::function<int(unsigned long long, Vec2, float*)>
    const uint8_t pad_67C[4];
    rw::Field<uint32_t> field_680;           // +0x680
    rw::Field<uint8_t> field_684;            // +0x684
    rw::Field<uint8_t> dropAnimating;        // +0x685 the dragged panel is flying back / onto its target
    rw::Field<uint8_t> dragging;             // +0x686 a medal panel is being dragged
    rw::Field<uint8_t> touching;             // +0x687 a touch began on an item (tap or drag start)
    rw::Field<uint32_t> field_688;           // +0x688
    rw::Field<float> touchStartX;            // +0x68C location where the touch began
    rw::Field<float> touchStartY;            // +0x690
    rw::Field<float> dragStartX;             // +0x694 popup-layer position of the dragged panel at drag start
    rw::Field<float> dragStartY;             // +0x698
    rw::Field<int32_t> touchIndex;           // +0x69C item index of the touch
    rw::Field<uint32_t> hiddenItem;          // +0x6A0 the cell hidden while its copy is dragged
    rw::Field<uint32_t> dragPanel;           // +0x6A4 the dragged copy (on the popup layer)
    const uint8_t targetRect[16];                  // +0x6A8 cocos2d::Rect
    MedalIdVector burstUpIds;                // +0x6B8
    MedalIdVector disabledIds;               // +0x6C4 ids that cannot be picked (isDisabledMedal)
    MedalIdVector disableMedalIds;           // +0x6D0 ids greyed out by setDisableMedalID
    MedalIdVector usedIds;                   // +0x6DC ids dragged out (in use)
    MedalIdVector noEffectIds;               // +0x6E8 ids marked "no effect"
    rw::Field<int32_t> showType;             // +0x6F4 DeckMedalView::ShowType
    rw::Field<uint8_t> mixMode;              // +0x6F8 fusion material picker
    rw::Field<uint8_t> isShowMixInfo;        // +0x6F9
    rw::Field<uint8_t> enabledTapItem;       // +0x6FA
    const uint8_t pad_6FB[5];

    static constexpr uint32_t OFF_ITEM_IDS = 0x628, OFF_DISABLED = 0x6C4, OFF_DISABLE_MEDAL = 0x6D0,
                              OFF_USED = 0x6DC, OFF_NO_EFFECT = 0x6E8, OFF_BURST_UP = 0x6B8;

    // ---- construction / destruction
    static Addr create();
    static MiniMedalScrollView *construct(MiniMedalScrollView *self);
    static MiniMedalScrollView *destruct(MiniMedalScrollView *self);
    static void destructDelete(MiniMedalScrollView *self);
    static uint32_t destructThunk640(uint32_t base2);        // ~MiniMedalScrollView via the +0x280 base
    static uint32_t destructThunk848(uint32_t base3);        // via the +0x350 base
    static void destructDeleteThunk640(uint32_t base2);
    static void destructDeleteThunk848(uint32_t base3);
    static bool init(MiniMedalScrollView *self);
    static void update(MiniMedalScrollView *self, float dt);

    // ---- cells
    static Addr getLayoutPanel(MiniMedalScrollView *self, uint64_t id, uint32_t smallLayout);
    static Addr createRow(MiniMedalScrollView *self, int32_t index, int32_t arg);
    static uint32_t changeRow(MiniMedalScrollView *self, Addr widget, uint32_t index, int32_t arg);
    static void registItemTouchEvent(MiniMedalScrollView *self, Addr widget, int32_t index);

    // ---- medal state
    static bool isDisabledMedal(MiniMedalScrollView *self, Addr medal);          // shared_ptr<UserMedalData>*
    static void updateDisabledItemIDs(MiniMedalScrollView *self);
    static void setItemIDs(MiniMedalScrollView *self, Addr ids);                 // const vector<ull>*
    static void setTargetRect(MiniMedalScrollView *self, Addr rect);
    static void setBurstUpMedalIDs(MiniMedalScrollView *self, Addr ids);
    static void setDisableMedalID(MiniMedalScrollView *self, uint64_t id, uint32_t disable, uint32_t relayout);
    static void changeShowType(MiniMedalScrollView *self, int32_t type);
    static void setIsShowMixInfo(MiniMedalScrollView *self, uint32_t show);
    static bool canSelectMaterialMedal(MiniMedalScrollView *self, Addr medal);
    static void setEnabledTapItem(MiniMedalScrollView *self, uint32_t enabled);
    static void clearNoEffectMedalIDs(MiniMedalScrollView *self);
    static void addNoEffectMedalID(MiniMedalScrollView *self, uint64_t id);
    static void clearDisableMedals(MiniMedalScrollView *self);

    // ---- touch handling (registItemTouchEvent's lambdas, MiniMedalScrollView_touch.cpp)
    // std::function<void(Touch*,Event*)>::operator=($_2): store the touch-ended lambda
    static void assignTouchEnded(Addr function, Addr lambda);
    // $_0 onTouchBegan [scroll, index]
    static bool onTouchBegan(Addr functor, Addr touch, Addr event);
    // $_1 onTouchMoved [scroll, index, widget]
    static void onTouchMoved(Addr functor, Addr touch, Addr event);
    // $_2 onTouchEnded / onTouchCancelled [scroll, index]
    static void onTouchEnded(Addr functor, Addr touch, Addr event);
    // $_2's CallFunc lambda [scroll, moved]: the drop animation finished
    static void onDropFinished(Addr functor);
    // that lambda's CallFunc lambda [scroll]: remove the dragged panel
    static void onDragPanelFaded(Addr functor);
};
RW_LAYOUT(MiniMedalScrollView, vtable2, 0x280);
RW_LAYOUT(MiniMedalScrollView, vtable3, 0x350);
RW_LAYOUT(MiniMedalScrollView, field_4FC, 0x4FC);
RW_LAYOUT(MiniMedalScrollView, itemWidth, 0x514);
RW_LAYOUT(MiniMedalScrollView, rowsBegin, 0x51C);
RW_LAYOUT(MiniMedalScrollView, maxColumn, 0x538);
RW_LAYOUT(MiniMedalScrollView, scrollOffset, 0x540);
RW_LAYOUT(MiniMedalScrollView, field_570, 0x570);
RW_LAYOUT(MiniMedalScrollView, field_57C, 0x57C);
RW_LAYOUT(MiniMedalScrollView, itemIds, 0x628);
RW_LAYOUT(MiniMedalScrollView, onTapItem, 0x638);
RW_LAYOUT(MiniMedalScrollView, onDropItem, 0x650);
RW_LAYOUT(MiniMedalScrollView, getDropTarget, 0x668);
RW_LAYOUT(MiniMedalScrollView, field_680, 0x680);
RW_LAYOUT(MiniMedalScrollView, touching, 0x687);
RW_LAYOUT(MiniMedalScrollView, touchStartX, 0x68C);
RW_LAYOUT(MiniMedalScrollView, touchIndex, 0x69C);
RW_LAYOUT(MiniMedalScrollView, dragPanel, 0x6A4);
RW_LAYOUT(MiniMedalScrollView, targetRect, 0x6A8);
RW_LAYOUT(MiniMedalScrollView, burstUpIds, 0x6B8);
RW_LAYOUT(MiniMedalScrollView, disabledIds, 0x6C4);
RW_LAYOUT(MiniMedalScrollView, noEffectIds, 0x6E8);
RW_LAYOUT(MiniMedalScrollView, showType, 0x6F4);
RW_LAYOUT(MiniMedalScrollView, enabledTapItem, 0x6FA);
RW_SIZE(MiniMedalScrollView, 0x700);

// Helpers shared by the MiniMedalScrollView files (inline: no link-time symbols).
namespace mini_medal {
using namespace scene;

// std::vector<unsigned long long> helpers, as libc++ inlines them
// find: the element equal to id, or end
inline uint32_t vecFind(const MedalIdVector &v, uint32_t lo, uint32_t hi) {
    uint32_t p = v.begin, e = v.end;
    for (; p != e; p += 8)
        if (ld32(p) == lo && ld32(p + 4) == hi) break;
    return p;
}
// clear(): end = begin, computed like the compiled code
inline void vecClear(MedalIdVector &v) {
    uint32_t e = v.end, b = v.begin;
    if (e != b) v.end = e + (~7u & ~(e - 8 - b));
}
// push_back(id): construct in place when there is room, else __push_back_slow_path(&value)
constexpr uint32_t VECTOR_ULL_PUSH_BACK_SLOW = 0x4ECB6D;
inline void vecPushBack(MedalIdVector &v, uint32_t lo, uint32_t hi, uint32_t valueAddr) {
    uint32_t e = v.end;
    if (e != v.cap) {
        uint32_t n = 0;
        if (e) {
            st32(e, lo);
            st32(e + 4, hi);
            n = v.end;
        }
        v.end = n + 8;
    } else {
        call(VECTOR_ULL_PUSH_BACK_SLOW, {rw::guest_addr(&v), valueAddr});
    }
}
// erase(it) as compiled: memmove the tail down, then move end
constexpr uint32_t MEMMOVE = 0x40EF50;   // __aeabi_memmove(dst, src, n)
inline void vecErase(MedalIdVector &v, uint32_t it) {
    uint32_t b = v.begin, e = v.end;
    uint32_t off = it - b;
    uint32_t pos = b + (off & ~7u);
    uint32_t tail = e - (pos + 8);
    call(MEMMOVE, {pos, pos + 8, tail});
    uint32_t newEnd = b + (((tail >> 3) + (off >> 3)) << 3);
    uint32_t cur = v.end;
    if (cur != newEnd) v.end = cur + (~7u & ~(cur - 8 - newEnd));
}
}  // namespace mini_medal
