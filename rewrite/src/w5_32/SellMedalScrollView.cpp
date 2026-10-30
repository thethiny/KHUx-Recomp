// SellMedalScrollView: the grid of Medal List > Sell Medals (a BaseScrollView<unsigned long long> of medal ids),
// MedalListScrollView's sibling: cells 126x168 with the Cursor_Anim_MedalSell armature ("selectView") over a
// DeckMedalView ("medalView"); a map<id, bool> holds the selection. Medals that cannot be sold (a favourite, the
// support medal, a locked one, or the id the scene excludes - setMedalIDs' second argument) are greyed out and never
// selected. A long press (update) reports the pressed medal.
#include "../w3_9/BaseScrollView.hpp"
#include "../scene/SceneUi.hpp"
#include "../scene_menu/DeckMedalView.hpp"
#include "../scene_menu/DeckMedalViewManager.hpp"

#include <cmath>

using namespace scene;

namespace {
namespace a {
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                     // Size(float, float)
constexpr uint32_t Color3B_ctor = rwlib::cocos2d::Color3B::ctor_base;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create;
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance, ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;              // vector::at failure (does not return)
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t IScrollProtocol_getItemIndex = 0x4F7DE1, IScrollProtocol_getLocalItemIndex = 0x4F7E39;
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;
constexpr uint32_t FunctionVI_call = 0x4F321D;               // std::function<void(int)>::operator()
constexpr uint32_t SellMedalScrollView_setSelectedMedal = 0x500081;   // (this, -, u64 id, bool on the stack)
constexpr uint32_t VectorULL_dtor = 0x468499;
constexpr uint32_t VectorULL_pushBackSlow = 0x4ECB6D;       // vector<ull>::__push_back_slow_path(const ull&)
constexpr uint32_t MapULLBool_destroy = 0x5004F1;           // __tree<ull, bool>::destroy(node)
constexpr uint32_t MapULLBool_index = 0x4FFAC5;             // map<ull, bool>::operator[](const ull&)
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1, IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t IScrollProtocol_update = 0x4F5CFD;
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t BaseScrollViewULL_init = 0x4FC969, BaseScrollViewULL_setItemIDs = 0x4FD231;
constexpr uint32_t FunctionVUll_call = 0x4FF3C9;            // std::function<void(ull)>::operator()(ull)
constexpr uint32_t FunctionBTE_swap = 0x483E4D;             // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionVTE_swap = 0x4F3699;             // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create, EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, int, UserMedal::Medal by value)
constexpr uint32_t MedalDataManager_isFavoriteMedal = 0x76B249;
constexpr uint32_t DeckMedalView_setData = 0x52BEE5, DeckMedalView_changeShowType = 0x52C1ED;
constexpr uint32_t SafeBuffer_read = 0x45A269;
}  // namespace a

// vtables (through the GOT): +0 = vt + 8, +0x280 = vt + 0x360, +0x350 = vt + 0x374
constexpr uint32_t GOT_VT_SellMedalScrollView = 0xFD65A8, GOT_VT_BaseScrollView_ull = 0xFD65A4;
constexpr uint32_t VT2_OFFSET = 0x360, VT3_OFFSET = 0x374;
constexpr uint32_t SIZE = 0x6C0;
// fields
constexpr uint32_t F_ROWS = 0x51C;            // IScrollProtocol's row widgets (begin, end)
constexpr uint32_t F_ITEM_W = 0x514, F_ITEM_H = 0x518;
constexpr uint32_t F_ITEM_IDS = 0x628;        // BaseScrollView<ull> item ids
constexpr uint32_t F_ON_TAP = 0x638;          // std::function<void(int)>
constexpr uint32_t F_ON_LONG_PRESS = 0x650;   // std::function<void(ull)>
constexpr uint32_t F_SELECTED = 0x668;        // std::map<ull, bool>
constexpr uint32_t F_SHOW_TYPE = 0x678;       // DeckMedalView::ShowType
constexpr uint32_t F_DISABLED = 0x67C;        // std::vector<ull>: medals that cannot be sold
constexpr uint32_t F_EXCLUDED = 0x688;        // ull: a medal id the scene never lets be sold
constexpr uint32_t F_TOUCH = 0x690;           // cells take touches
constexpr uint32_t F_PRESSING = 0x691;        // a cell is held
constexpr uint32_t F_PRESS_DONE = 0x69C;      // the press moved (or became a long press): no tap
constexpr uint32_t F_PRESS_TIME = 0x6A0;      // float seconds held
constexpr uint32_t F_PRESS_INDEX = 0x6A4;
constexpr uint32_t F_VEC2 = 0x6A8;
// lambda vtables (+8)
constexpr uint32_t VT_TouchBegan = 0xF55D90;  // registItemTouchEvent $_0 {this, index}
constexpr uint32_t VT_TouchMoved = 0xF55D64;  // $_1 {this, this}
constexpr uint32_t VT_TouchEnded = 0xF55D0C;  // $_2 {this, index, this, index, this} (heap 0x18)
constexpr uint32_t VT_TapDone = 0xF55D38;     // $_2's tap animation end (no captures)
// the touch state (the BaseScrollView<ull> part ends at 0x690)
constexpr uint32_t F_TAP_INDEX = 0x694, F_BEGIN_X = 0x698;
constexpr uint32_t F_BEGIN_ITEM = 0x6A4, F_BEGIN_LOC = 0x6A8;   // Vec2
constexpr uint32_t F_SWIPE_OK = 0x6B0;        // a sideways swipe may still select
constexpr uint32_t F_SWIPE_LAST = 0x6B4;      // the item the swipe was over last
constexpr uint32_t F_TOUCH_FREE = 0x6B8;      // cleared by a press, set by its end
constexpr uint32_t F_SWIPE_FIRST = 0x6B9, F_SWIPE_FROM = 0x6BA;   // the swipe's first step / the state it toggles
constexpr uint32_t F_MAX_COLUMN = 0x538;      // IScrollProtocol
constexpr float MOVED = 20.0f, SWIPE_OFF_Y = 35.0f, SWIPE_X = 60.0f;
// strings
constexpr uint32_t S_Medal = 0xDFD05F, S_medalView = 0xDFD055, S_selectView = 0xDFD0BA;
constexpr uint32_t S_AnimJson = 0xDFD065;     // "cocostudio/publish/Cursor_Anim_MedalSell.ExportJson"
constexpr uint32_t S_Anim = 0xDFD099;         // "Cursor_Anim_MedalSell"
constexpr uint32_t S_AnimPlay = 0xDFD0AF;     // "Animation1"
// APIManager: shared_ptr<UserData> (SafeBuffer: 0x58 = the support medal id)
constexpr uint32_t API_USER_DATA = 0x114;

constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_changeRow = 0x340;
constexpr uint32_t V_getAnimation = 0x298, V_animationPlay = 0x68, V_setBright = 0x238;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
// MedalDataManager::getMedalData(out, manager, id, 0, UserMedal::Medal{}) (the record's unset word passed as 0)
uint32_t medalData(Frame &f, uint32_t manager, uint32_t lo, uint32_t hi) {
    const uint32_t out = f.alloc(8);
    call(a::MedalDataManager_getMedalData, {out, manager, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0});
    return out;
}
uint64_t u64(uint32_t at) { return (uint64_t(ld32(at + 4)) << 32) | ld32(at); }
// the map has an entry for `key` (std::map::find, inline)
bool hasEntry(uint32_t s, uint64_t key) {
    for (uint32_t node = ld32(s + F_SELECTED + 4); node;) {
        const uint64_t nk = u64(node + 0x10);
        if (key < nk) node = ld32(node);
        else if (nk < key) node = ld32(node + 4);
        else return true;
    }
    return false;
}
// push the medal's id (a vector<ull>::push_back, inline) when it cannot be sold
void pushDisabled(uint32_t s, uint32_t data) {
    const uint32_t end = ld32(s + F_DISABLED + 4);
    if (end == ld32(s + F_DISABLED + 8)) {
        call(a::VectorULL_pushBackSlow, {s + F_DISABLED, data});
        return;
    }
    uint32_t n = 0;
    if (end) {
        st32(end, ld32(data));
        st32(end + 4, ld32(data + 4));
        n = ld32(s + F_DISABLED + 4);
    }
    st32(s + F_DISABLED + 4, n + 8);
}
}  // namespace

struct SellMedalScrollView {
    static uint32_t create();
    static uint32_t construct(uint32_t self);
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static uint32_t update(uint32_t self, float dt);
    static uint32_t getLayoutPanel(uint32_t self, uint32_t, uint32_t, uint32_t);
    static uint32_t createRow(uint32_t self, uint32_t index, uint32_t arg);
    static uint32_t changeRow(uint32_t self, uint32_t widget, uint32_t index, uint32_t arg);
    static uint32_t registItemTouchEvent(uint32_t self, uint32_t widget, uint32_t index);
    static uint32_t assignTouchEnded(uint32_t dst, uint32_t lambda);
    static uint32_t setMedalIDs(uint32_t self, uint32_t ids, uint32_t excludedLo, uint32_t excludedHi);
    static uint32_t isDisableMedal(uint32_t self, uint32_t medal);
    static void setSelectedMedal(uint32_t self, uint64_t id, uint32_t selected);
    static uint32_t getSelectedMedals(uint32_t out, uint32_t self);
    static uint32_t touchBegan(uint32_t functor, uint32_t touchRef, uint32_t);
    static void touchMoved(uint32_t functor, uint32_t touchRef, uint32_t);
    static void touchEnded(uint32_t functor, uint32_t touchRef, uint32_t);
};

namespace {
// selected[id] (std::map<ull, bool>::operator[], inserting false)
uint8_t selectedOf(uint32_t s, uint32_t lo, uint32_t hi) {
    Frame f;
    const uint32_t key = f.alloc(8);
    st32(key, lo);
    st32(key + 4, hi);
    return ld8(call(a::MapULLBool_index, {s + F_SELECTED, key}));
}
// the medal toggled (SellMedalScrollView::setSelectedMedal(id, !selected[id])), then the list's tap callback with
// its tap index
void toggle(uint32_t s, uint32_t lo, uint32_t hi) {
    const uint32_t on = selectedOf(s, lo, hi) ? 0u : 1u;
    {
        Frame f;
        const uint32_t args = f.alloc(8);
        st32(args, on);
        rw::call_exact(a::SellMedalScrollView_setSelectedMedal, {s, 0, lo, hi});
    }
    if (ld32(s + F_ON_TAP + 16)) call(a::FunctionVI_call, {s + F_ON_TAP, ld32(s + F_TAP_INDEX)});
}
uint32_t touchLocation(Frame &f, uint32_t touch) {
    const uint32_t loc = f.alloc(8);
    call(a::Touch_getLocation, {loc, touch});
    return loc;
}
}  // namespace

// $_0 (touch began) {this, row}: taps on, a press inside the row's cell starts: the press state, the touch
// position, the item under it (also the swipe's start)
uint32_t SellMedalScrollView::touchBegan(uint32_t f, uint32_t touchRef, uint32_t) {
    const uint32_t s = ld32(f + 4);
    if (!ld8(s + F_TOUCH) || !ld8(s + F_TOUCH_FREE)) return 0;
    const uint32_t touch = ld32(touchRef);
    Frame fr;
    {
        const uint32_t loc = touchLocation(fr, touch);
        st32(s + F_BEGIN_X, ld32(loc));
        call(a::Vec2_dtor, {loc});
    }
    const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(f + 8));
    const uint32_t local = fr.alloc(8), rect = fr.alloc(16);
    {
        const uint32_t loc = touchLocation(fr, touch);
        call(a::Node_convertToNodeSpace, {local, row, loc});
        call(a::Vec2_dtor, {loc});
    }
    const uint32_t w = ld32(vcall(row, node::getContentSize, {row}));
    const uint32_t h = ld32(vcall(row, node::getContentSize, {row}) + 4);
    call(a::Rect_ctor, {rect, 0, 0, w, h});
    uint32_t pressed = 0;
    if (call(a::Rect_containsPoint, {rect, local})) {
        st8(s + F_TOUCH_FREE, 0);
        st32(s + F_PRESS_TIME, 0);
        pressed = 1;
        st8(s + F_PRESSING, 1);
        const uint32_t loc = touchLocation(fr, touch);
        st32(s + F_BEGIN_LOC + 4, ld32(loc + 4));
        st32(s + F_BEGIN_LOC, ld32(loc));
        call(a::Vec2_dtor, {loc});
        st8(s + F_PRESS_DONE, 0);
        const uint32_t item = call(a::IScrollProtocol_getItemIndex, {s, ld32(f + 8)});
        st32(s + F_BEGIN_ITEM, item);
        st8(s + F_SWIPE_OK, 1);
        st32(s + F_SWIPE_LAST, item);
        st8(s + F_SWIPE_FROM, 0);
        st8(s + F_SWIPE_FIRST, 1);
    }
    call(a::Vec2_dtor, {rect});
    call(a::Vec2_dtor, {local});
    return pressed;
}

// $_1 (touch moved) {this, this}: a move of MOVED marks the press moved (no tap); a vertical move of SWIPE_OFF_Y
// ends the swipe; a sideways swipe of SWIPE_X over the items toggles each item passed whose selection matches the
// one the swipe started on (the span capped to one item when it reaches the column count)
void SellMedalScrollView::touchMoved(uint32_t f, uint32_t touchRef, uint32_t) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t s = ld32(f + 4);
    if (!ld8(s + F_PRESSING)) return;
    Frame fr;
    float swipeX, moveX, moveY;
    {
        const uint32_t loc = touchLocation(fr, touch);
        swipeX = std::fabs(ldf(loc) - ldf(s + F_BEGIN_X));
        call(a::Vec2_dtor, {loc});
    }
    {
        const uint32_t loc = touchLocation(fr, touch);
        moveX = std::fabs(ldf(loc) - ldf(s + F_BEGIN_LOC));
        call(a::Vec2_dtor, {loc});
    }
    {
        const uint32_t loc = touchLocation(fr, touch);
        moveY = std::fabs(ldf(loc + 4) - ldf(s + F_BEGIN_LOC + 4));
        call(a::Vec2_dtor, {loc});
    }
    if (moveY >= MOVED || moveX >= MOVED) st8(s + F_PRESS_DONE, 1);
    if (moveY >= SWIPE_OFF_Y) st8(s + F_SWIPE_OK, 0);
    if (!(swipeX >= SWIPE_X) || !ld8(s + F_SWIPE_OK)) return;
    int32_t local;
    {
        const uint32_t loc = touchLocation(fr, touch);
        local = int32_t(call(a::IScrollProtocol_getLocalItemIndex, {s, loc}));
        call(a::Vec2_dtor, {loc});
    }
    if (local < 0) return;
    const int32_t item = int32_t(call(a::IScrollProtocol_getItemIndex, {s, uint32_t(local)}));
    const int32_t last = int32_t(ld32(s + F_SWIPE_LAST));
    if (last == item) return;
    int32_t from = item, n = 1;
    if (last == -1) {
        n = item >= 0 ? 1 : 0;
    } else if (item == -1) {
        from = last;
    } else {
        n = std::abs(last - item) + 1;
        if (last < item) from = last;
    }
    if (n >= 1 && ld8(s + F_SWIPE_FIRST)) {
        st8(s + F_SWIPE_FIRST, 0);
        const uint32_t id = ld32(s + F_ITEM_IDS) + 8 * uint32_t(from);
        st8(s + F_SWIPE_FROM, selectedOf(s, ld32(id), ld32(id + 4)));
    }
    if (!(int32_t(ld32(s + F_MAX_COLUMN)) > n)) n = 1;
    for (int32_t k = 0; k < n; k++) {
        const uint32_t id = ld32(s + F_ITEM_IDS) + 8 * uint32_t(from + k);
        const uint32_t lo = ld32(id), hi = ld32(id + 4);
        bool matches = selectedOf(s, lo, hi) && ld8(s + F_SWIPE_FROM);
        if (!matches) matches = !selectedOf(s, lo, hi) && !ld8(s + F_SWIPE_FROM);
        if (matches) toggle(ld32(f + 8), lo, hi);
    }
    st32(s + F_SWIPE_LAST, uint32_t(item));
}

// $_2 (touch ended) {this, row, this, row, this}: touches free again; a press that did not move on a medal taps
// it: the row bounces (front while it does) and the medal's selection toggles; the press state cleared
void SellMedalScrollView::touchEnded(uint32_t f, uint32_t, uint32_t) {
    const uint32_t s = ld32(f + 4);
    st8(s + F_TOUCH_FREE, 1);
    const uint32_t item = call(a::IScrollProtocol_getItemIndex, {s, ld32(f + 8)});
    st32(s + F_TAP_INDEX, item);
    const uint32_t ids = ld32(s + F_ITEM_IDS);
    if ((ld32(s + F_ITEM_IDS + 4) - ids) >> 3 <= item || !ld8(s + F_PRESSING) || ld8(s + F_PRESS_DONE) ||
        !(ld32(ids + 8 * item) | ld32(ids + 8 * item + 4))) {
        st8(s + F_PRESSING, 0);
        return;
    }
    {
        const uint32_t row = ld32(ld32(s + F_ROWS) + 4 * ld32(f + 8));
        vcall(row, node::setLocalZOrder, {row, 1});
        vcall(row, node::setScale, {row, fbits(1.0f)});
        Frame fr;
        const uint32_t done = newFunction(fr, VT_TapDone, {});
        call(fn::Node_runAction, {row, call(a::SceneUtil_createTapAnimation, {row, done})});
        functionDestroy(done);
    }
    const uint32_t s2 = ld32(f + 0xC);
    const uint32_t row2 = ld32(ld32(s2 + F_ROWS) + 4 * ld32(f + 0x10));
    vcall(row2, node::setLocalZOrder, {row2, 0});
    if (ld8(s2 + F_PRESSING)) {
        const uint32_t id = ld32(s2 + F_ITEM_IDS) + 8 * ld32(s2 + F_TAP_INDEX);
        toggle(ld32(f + 0x14), ld32(id), ld32(id + 4));
    }
    st8(s2 + F_PRESSING, 0);
    st8(s2 + F_PRESS_DONE, 0);
    st8(s2 + F_SWIPE_OK, 0);
}

// ------------------------------------------------------------------------------------------ lifetime
uint32_t SellMedalScrollView::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    construct(me);
    if (vcall(me, V_init, {me}) == 1) {
        call(a::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t SellMedalScrollView::construct(uint32_t s) {
    auto *base = rw::host_ptr<BaseScrollView>(s);
    call(a::Layout_ctor, {s});
    BaseScrollView::initScrollDefaults(base);
    base->itemIDs.begin = 0;
    base->itemIDs.end = 0;
    base->itemIDs.cap = 0;
    const uint32_t vt = ld32(GOT_VT_SellMedalScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    st32(s + F_ON_TAP + 0x10, 0);
    st32(s + F_ON_LONG_PRESS + 0x10, 0);
    st32(s + F_SELECTED + 4, 0);           // empty map: root, size, begin = &root
    st32(s + F_SELECTED + 8, 0);
    st32(s + F_SELECTED, s + F_SELECTED + 4);
    st8(s + 0x674, 0);
    st32(s + F_SHOW_TYPE, 1);
    st32(s + 0x694, 0);
    st32(s + 0x698, 0);
    st8(s + F_PRESS_DONE, 0);
    st32(s + F_PRESS_TIME, 0);
    st32(s + F_PRESS_INDEX, 0);
    for (uint32_t k = 0; k < 0x14; k += 4) st32(s + F_DISABLED + k, 0);   // the vector, the excluded id
    st8(s + F_TOUCH, 0);
    st8(s + F_PRESSING, 0);
    call(a::Vec2_ctor, {s + F_VEC2});
    st8(s + 0x6B0, 0);
    st32(s + 0x6B4, 0xFFFFFFFFu);
    st8(s + 0x6B9, 0);
    st8(s + 0x6B8, 0);
    st8(s + 0x6BA, 0);
    return s;
}

uint32_t SellMedalScrollView::destruct(uint32_t s) {
    const uint32_t vt = ld32(GOT_VT_SellMedalScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    // unregister the cells' medal views
    uint32_t rows = ld32(s + F_ROWS);
    if (int32_t(ld32(s + F_ROWS + 4) - rows) >= 1) {
        for (int32_t k = 0;;) {
            const uint32_t view = findNode(ld32(rows + 4 * uint32_t(k)), S_medalView, 9);
            if (view) DeckMedalViewManager::removeDeckMedalView(DeckMedalViewManager::getInstance(), view);
            rows = ld32(s + F_ROWS);
            k++;
            if (k >= int32_t(ld32(s + F_ROWS + 4) - rows) >> 2) break;
        }
    }
    call(a::Vec2_dtor, {s + F_VEC2});
    call(a::VectorULL_dtor, {s + F_DISABLED});
    call(a::MapULLBool_destroy, {s + F_SELECTED, ld32(s + F_SELECTED + 4)});
    functionDestroy(s + F_ON_LONG_PRESS);
    functionDestroy(s + F_ON_TAP);
    const uint32_t bvt = ld32(GOT_VT_BaseScrollView_ull);
    st32(s, bvt + 8);
    st32(s + 0x280, bvt + VT2_OFFSET);
    st32(s + 0x350, bvt + VT3_OFFSET);
    call(a::VectorULL_dtor, {s + F_ITEM_IDS});
    call(a::IScrollProtocol_dtor, {s});
    return s;
}

// Cells 126x168, with the scroll bar.
uint32_t SellMedalScrollView::init(uint32_t s) {
    stf(s + 0x570, 39.0f);
    stf(s + 0x574, -70.0f);
    stf(s + 0x57C, 190.0f);
    stf(s + F_ITEM_W, 126.0f);
    stf(s + F_ITEM_H, 168.0f);
    call(a::IScrollProtocol_setEnabledScrollBar, {s, 1});
    st8(s + 0x6B8, 1);
    return call(a::BaseScrollViewULL_init, {s});
}

// After the scroll update: a cell held for 0.3 s reports its medal (on-long-press) once.
uint32_t SellMedalScrollView::update(uint32_t s, float dt) {
    call(a::IScrollProtocol_update, {s, fbits(dt)});
    if (!ld8(s + F_PRESSING)) return 0;
    if (ld8(s + F_PRESS_DONE)) return ld8(s + F_PRESS_DONE);
    const float held = ldf(s + F_PRESS_TIME);
    if (!(held >= 0.3f) && !std::isnan(held)) {
        const float t = held + dt;
        stf(s + F_PRESS_TIME, t);
        return fbits(t);
    }
    const int32_t index = int32_t(ld32(s + F_PRESS_INDEX));
    if (index < 0) return F_PRESS_INDEX;
    const uint32_t begin = ld32(s + F_ITEM_IDS);
    const uint32_t count = uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - begin) >> 3);
    if (uint32_t(index) >= count) return count;
    if (ld32(s + F_ON_LONG_PRESS + 0x10)) {
        const uint32_t id = begin + 8 * uint32_t(index);
        rw::call(a::FunctionVUll_call, {s + F_ON_LONG_PRESS, id + 8, ld32(id), ld32(id + 4)});
    }
    st8(s + F_PRESSING, 0);
    return 0;
}

// ------------------------------------------------------------------------------------------ cells
// A cell: an empty widget of the cell size with the selection armature ("selectView") over a DeckMedalView
// ("medalView", registered with the DeckMedalViewManager). The id is not used.
uint32_t SellMedalScrollView::getLayoutPanel(uint32_t s, uint32_t, uint32_t, uint32_t) {
    const uint32_t panel = call(a::Widget_create, {});
    {
        const uint32_t mgr = call(a::ArmatureDataManager_getInstance, {});
        Frame f;
        const uint32_t json = newStr(f, S_AnimJson, 0x33);
        call(a::ArmatureDataManager_addArmatureFileInfo, {mgr, json});
        strFree(json);
    }
    uint32_t armature;
    {
        Frame f;
        const uint32_t name = newStr(f, S_Anim, 0x15);
        armature = call(a::Armature_create, {name});
        strFree(name);
    }
    {
        const uint32_t anim = vcall(armature, V_getAnimation, {armature});
        Frame f;
        const uint32_t name = newStr(f, S_AnimPlay, 0xA);
        vcall(anim, V_animationPlay, {anim, name, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(name);
    }
    {
        Frame f;
        const uint32_t name = newStr(f, S_selectView, 0xA);
        vcall(panel, node::addChildZName, {panel, armature, 0, name});
        strFree(name);
    }
    const uint32_t view = DeckMedalView::create(1);
    vcall(panel, node::setAnchorPoint, {panel, VEC2_ANCHOR_MIDDLE});
    {
        Frame f;
        const uint32_t name = newStr(f, S_medalView, 9);
        vcall(panel, node::addChildZName, {panel, view, 0, name});
        strFree(name);
    }
    {
        Frame f;
        const uint32_t size = f.alloc(8);
        call(a::Size_ctor, {size, ld32(s + F_ITEM_W), ld32(s + F_ITEM_H)});
        vcall(panel, node::setContentSize, {panel, size});
    }
    vcall(panel, node::setPositionX, {panel, fbits(ldf(s + F_ITEM_W) * 0.5f)});
    DeckMedalViewManager::addDeckMedalView(DeckMedalViewManager::getInstance(), view);
    const uint32_t medal = findNode(view, S_Medal, 5);
    vcall(armature, node::setPosition, {armature, vcall(medal, node::getPosition, {medal})});
    return panel;
}

uint32_t SellMedalScrollView::createRow(uint32_t s, uint32_t index, uint32_t arg) {
    const uint32_t count = uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 3);
    if (count <= index) {
        call(a::ThrowOutOfRange, {s + F_ITEM_IDS});
        return 0;
    }
    const uint32_t panel = getLayoutPanel(s, 0, 0, 0);
    vcall(s, V_changeRow, {s, panel, index, arg});
    return panel;
}

// Fill a cell with item `index`: the medal (show type of the list), greyed out when it cannot be sold, and the
// selection mark of its id.
uint32_t SellMedalScrollView::changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t) {
    const uint32_t begin = ld32(s + F_ITEM_IDS);
    if (!(uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - begin) >> 3) > index)) {
        call(a::ThrowOutOfRange, {s + F_ITEM_IDS});
        return 0;
    }
    const uint32_t id = begin + 8 * index;
    const uint32_t medalNode = findNode(widget, S_Medal, 5);
    if (!medalNode) return 1;
    const uint32_t mgr = call(a::MedalDataManager_getInstance, {});
    Frame f;
    const uint32_t medal = medalData(f, mgr, ld32(id), ld32(id + 4));
    const uint32_t view = findNode(widget, S_medalView, 9);
    st8(view + 0x38B, 1);
    st8(view + 0x389, 1);
    call(a::DeckMedalView_changeShowType, {view, ld32(s + F_SHOW_TYPE)});
    // setData(shared_ptr copy, -1, 4, 1, 10000, 10000, false, 0, nullptr, shared_ptr<UserDetail>{})
    const uint32_t copy = f.alloc(8), detail = f.alloc(8);
    st32(copy, ld32(medal));
    st32(copy + 4, ld32(medal + 4));
    addRefIf(ld32(copy + 4));
    st32(detail, 0);
    st32(detail + 4, 0);
    call(a::DeckMedalView_setData, {view, copy, 0xFFFFFFFFu, 4, 1, 10000, 10000, 0, 0, 0, detail});
    releaseIf(ld32(detail + 4));
    releaseIf(ld32(copy + 4));
    // unsellable: lit and grey; otherwise unlit and white
    uint32_t at = ld32(s + F_DISABLED);
    const uint32_t end = ld32(s + F_DISABLED + 4);
    while (at != end && u64(at) != u64(id)) at += 8;
    const bool disabled = at != end;
    vcall(medalNode, V_setBright, {medalNode, disabled ? 1u : 0u});
    {
        const uint32_t color = f.alloc(4);
        const uint32_t c = disabled ? 100 : 255;
        call(a::Color3B_ctor, {color, c, c, c});
        vcall(medalNode, node::setColor, {medalNode, color});
    }
    const uint32_t select = findNode(widget, S_selectView, 0xA);
    const uint32_t shown = call(a::MapULLBool_index, {s + F_SELECTED, id});
    vcall(select, node::setVisible, {select, ld8(shown)});
    releaseIf(ld32(medal + 4));
    return 0;
}

// Touch listener for a cell: began {this, index}, moved {this, this}, ended / cancelled {this, index, this, index,
// this}.
uint32_t SellMedalScrollView::registItemTouchEvent(uint32_t s, uint32_t widget, uint32_t index) {
    if (!ld8(s + F_TOUCH)) return 0;
    const uint32_t dispatcher = ld32(call(a::Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(a::EventListenerTouchOneByOne_create, {});
    call(a::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    const uint32_t began = newFunction(f, VT_TouchBegan, {s, index});
    call(a::FunctionBTE_swap, {began, listener + 0x50});
    functionDestroy(began);
    const uint32_t captures = f.alloc(20);
    st32(captures, s);
    st32(captures + 4, index);
    st32(captures + 8, s);
    st32(captures + 12, index);
    st32(captures + 16, s);
    const uint32_t moved = newFunction(f, VT_TouchMoved, {s, s});
    call(a::FunctionVTE_swap, {moved, listener + 0x68});
    functionDestroy(moved);
    assignTouchEnded(listener + 0x80, captures);
    assignTouchEnded(listener + 0x98, captures);
    call(a::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, widget});
    return 0;
}

// sub_4FFC80: std::function<void(Touch*, Event*)> = $_2 (a heap __func with the five captures, swapped in)
uint32_t SellMedalScrollView::assignTouchEnded(uint32_t dst, uint32_t lambda) {
    Frame f;
    const uint32_t tmp = f.alloc(20);
    st32(tmp + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x18});
    if (obj) {
        st32(obj, VT_TouchEnded);
        for (uint32_t w = 0; w < 20; w += 4) st32(obj + 4 + w, ld32(lambda + w));
    }
    st32(tmp + 16, obj);
    call(a::FunctionVTE_swap, {tmp, dst});
    functionDestroy(tmp);
    return 0;
}

// ------------------------------------------------------------------------------------------ medal ids
// The ids to show and the one never to sell: each id gets a selection entry (false) if it has none, and the ones
// that cannot be sold are remembered; then BaseScrollView<ull>::setItemIDs.
uint32_t SellMedalScrollView::setMedalIDs(uint32_t s, uint32_t ids, uint32_t excludedLo, uint32_t excludedHi) {
    st32(s + F_EXCLUDED + 4, excludedHi);
    st32(s + F_EXCLUDED, excludedLo);
    const uint32_t ctl = ld32(call(fn::APIManager_getInstance, {}) + API_USER_DATA + 4);
    addRefIf(ctl);
    if (ld32(ids + 4) != ld32(ids)) {
        Frame f;
        const uint32_t key = f.alloc(8);
        for (uint32_t i = 0;;) {
            const uint32_t at = ld32(ids) + 8 * i;
            const uint32_t lo = ld32(at), hi = ld32(at + 4);
            st32(key, lo);
            st32(key + 4, hi);
            if (!hasEntry(s, (uint64_t(hi) << 32) | lo)) st8(call(a::MapULLBool_index, {s + F_SELECTED, key}), 0);
            const uint32_t mgr = call(a::MedalDataManager_getInstance, {});
            Frame m;
            const uint32_t medal = medalData(m, mgr, lo, hi);
            const uint32_t copy = m.alloc(8);
            st32(copy, ld32(medal));
            const uint32_t copyCtl = ld32(medal + 4);
            st32(copy + 4, copyCtl);
            addRefIf(copyCtl);
            const uint32_t disabled = isDisableMedal(s, copy);
            releaseIf(copyCtl);
            if (disabled == 1) pushDisabled(s, ld32(medal));
            releaseIf(ld32(medal + 4));
            i++;
            if (i >= uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 3)) break;
        }
    }
    call(a::BaseScrollViewULL_setItemIDs, {s, ids});
    releaseIf(ctl);
    return 0;
}

// Select / deselect medal `id` (only one with a selection entry): a favourite, the excluded id, the user's
// support medal (user data +0x58) or a locked medal (+0x26) is never selected; otherwise the decide SE and
// `selected`. Then the unsellable list is rebuilt from the item ids and the layout refreshed.
void SellMedalScrollView::setSelectedMedal(uint32_t s, uint64_t id, uint32_t selected) {
    if (!hasEntry(s, id)) return;
    Frame f;
    const uint32_t key = f.alloc(8);
    st32(key, uint32_t(id));
    st32(key + 4, uint32_t(id >> 32));
    const uint32_t medal = medalData(f, call(a::MedalDataManager_getInstance, {}), uint32_t(id), uint32_t(id >> 32));
    const uint32_t mgr = call(a::MedalDataManager_getInstance, {});
    const uint32_t copy = f.alloc(8);
    st32(copy, ld32(medal));
    st32(copy + 4, ld32(medal + 4));
    addRefIf(ld32(copy + 4));
    const uint32_t favourite = call(a::MedalDataManager_isFavoriteMedal, {mgr, copy});
    releaseIf(ld32(copy + 4));
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t userData = ld32(api + API_USER_DATA);
    const uint32_t support = f.alloc(8);
    rw::call(a::SafeBuffer_read, {userData, support, ld32(userData + 0xC) + 0x58, 8, 0x58});
    if (!favourite && u64(s + F_EXCLUDED) != id && u64(support) != id && !ld8(ld32(medal) + 0x26)) {
        call(a::AudioPlaySystemSEDecide, {});
        st8(call(a::MapULLBool_index, {s + F_SELECTED, key}), uint8_t(selected));
    } else {
        st8(call(a::MapULLBool_index, {s + F_SELECTED, key}), 0);
    }
    // the unsellable medals of the list, again
    const uint32_t begin = ld32(s + F_DISABLED), end = ld32(s + F_DISABLED + 4);
    if (end != begin) st32(s + F_DISABLED + 4, end + (~(end - 8 - begin) & ~7u));
    const uint32_t idsEnd = ld32(s + F_ITEM_IDS + 4);
    for (uint32_t at = ld32(s + F_ITEM_IDS); at != idsEnd; at += 8) {
        Frame m;
        const uint32_t item = medalData(m, call(a::MedalDataManager_getInstance, {}), ld32(at), ld32(at + 4));
        const uint32_t itemCopy = m.alloc(8);
        st32(itemCopy, ld32(item));
        const uint32_t copyCtl = ld32(item + 4);
        st32(itemCopy + 4, copyCtl);
        addRefIf(copyCtl);
        const uint32_t disabled = isDisableMedal(s, itemCopy);
        releaseIf(copyCtl);
        if (disabled == 1) pushDisabled(s, ld32(item));
        releaseIf(ld32(item + 4));
    }
    call(a::IScrollProtocol_updateLayout, {s, 0});
    releaseIf(ld32(medal + 4));
}

// A medal that cannot be sold: a favourite, the excluded id, the user's support medal, or a locked one.
uint32_t SellMedalScrollView::isDisableMedal(uint32_t s, uint32_t medal) {
    const uint32_t mgr = call(a::MedalDataManager_getInstance, {});
    uint32_t favourite;
    {
        Frame f;
        const uint32_t copy = f.alloc(8);
        st32(copy, ld32(medal));
        st32(copy + 4, ld32(medal + 4));
        addRefIf(ld32(copy + 4));
        favourite = call(a::MedalDataManager_isFavoriteMedal, {mgr, copy});
        releaseIf(ld32(copy + 4));
    }
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctl = ld32(api + API_USER_DATA + 4), userData = ld32(api + API_USER_DATA);
    addRefIf(ctl);
    const uint32_t data = ld32(medal);
    const uint64_t id = u64(data);
    Frame f;
    const uint32_t support = f.alloc(8);
    rw::call(a::SafeBuffer_read, {userData, support, ld32(userData + 0xC) + 0x58, 8, 0x58});
    const uint32_t disabled =
        (favourite || u64(s + F_EXCLUDED) == u64(ld32(medal)) || u64(support) == id || ld8(ld32(medal) + 0x26)) ? 1 : 0;
    releaseIf(ctl);
    return disabled;
}

// The selected medals (the ids whose map entry is true, in list order), into the vector `out`.
uint32_t SellMedalScrollView::getSelectedMedals(uint32_t out, uint32_t s) {
    st32(out, 0);
    st32(out + 4, 0);
    st32(out + 8, 0);
    uint32_t result = ld32(s + F_ITEM_IDS);
    if (ld32(s + F_ITEM_IDS + 4) == result) return result;
    for (uint32_t i = 0;;) {
        if (ld8(call(a::MapULLBool_index, {s + F_SELECTED, result + 8 * i}))) {
            if (uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 3) <= i)
                call(a::ThrowOutOfRange, {s + F_ITEM_IDS});
            const uint32_t at = ld32(s + F_ITEM_IDS) + 8 * i;
            const uint32_t end = ld32(out + 4);
            if (end == ld32(out + 8)) {
                call(a::VectorULL_pushBackSlow, {out, at});
            } else {
                uint32_t n = 0;
                if (end) {
                    st32(end, ld32(at));
                    st32(end + 4, ld32(at + 4));
                    n = ld32(out + 4);
                }
                st32(out + 4, n + 8);
            }
        }
        result = ld32(s + F_ITEM_IDS);
        i++;
        if (i >= uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - result) >> 3)) return result;
    }
}

// Every selection entry dropped, then each listed id gets a fresh one (false).
static uint32_t clearSelectedMedals(uint32_t s) {
    call(a::MapULLBool_destroy, {s + F_SELECTED, ld32(s + F_SELECTED + 4)});
    st32(s + F_SELECTED + 8, 0);
    st32(s + F_SELECTED, s + F_SELECTED + 4);
    st32(s + F_SELECTED + 4, 0);
    uint32_t result = ld32(s + F_ITEM_IDS);
    if (ld32(s + F_ITEM_IDS + 4) == result) return result;
    for (uint32_t i = 0;;) {
        st8(call(a::MapULLBool_index, {s + F_SELECTED, result + 8 * i}), 0);
        result = ld32(s + F_ITEM_IDS);
        i++;
        if (i >= uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - result) >> 3)) return result;
    }
}

RW_FUNCTION(0x500464, "SellMedalScrollView::clearSelectedMedals", clearSelectedMedals);
RW_FUNCTION(0x4FEFD0, "SellMedalScrollView::create", SellMedalScrollView::create);
RW_FUNCTION(0x4FF024, "SellMedalScrollView::SellMedalScrollView", SellMedalScrollView::construct);
RW_FUNCTION(0x4FED14, "SellMedalScrollView::~SellMedalScrollView", SellMedalScrollView::destruct);
RW_FUNCTION(0x4FF2A8, "SellMedalScrollView::init", SellMedalScrollView::init);
RW_FUNCTION(0x4FF310, "SellMedalScrollView::update", SellMedalScrollView::update);
RW_FUNCTION(0x4FF448, "SellMedalScrollView::getLayoutPanel", SellMedalScrollView::getLayoutPanel);
RW_FUNCTION(0x4FF748, "SellMedalScrollView::createRow", SellMedalScrollView::createRow);
RW_FUNCTION(0x4FF7A0, "SellMedalScrollView::changeRow", SellMedalScrollView::changeRow);
RW_FUNCTION(0x4FFB44, "SellMedalScrollView::registItemTouchEvent", SellMedalScrollView::registItemTouchEvent);
RW_FUNCTION(0x4FFC80, "SellMedalScrollView::registItemTouchEvent $_2 std::function::operator=(&&) (sub_4FFC80)",
            SellMedalScrollView::assignTouchEnded);
RW_FUNCTION(0x4FFD24, "SellMedalScrollView::setMedalIDs", SellMedalScrollView::setMedalIDs);
RW_FUNCTION(0x4FFF58, "SellMedalScrollView::isDisableMedal", SellMedalScrollView::isDisableMedal);
RW_FUNCTION(0x500080, "SellMedalScrollView::setSelectedMedal(unsigned long long, bool)", SellMedalScrollView::setSelectedMedal);
RW_FUNCTION(0x5003B4, "SellMedalScrollView::getSelectedMedals", SellMedalScrollView::getSelectedMedals);
RW_FUNCTION(0x500C44, "SellMedalScrollView::registItemTouchEvent $_0::operator() (touch began)", SellMedalScrollView::touchBegan);
RW_FUNCTION(0x500870, "SellMedalScrollView::registItemTouchEvent $_1::operator() (touch moved)", SellMedalScrollView::touchMoved);
RW_FUNCTION(0x50058C, "SellMedalScrollView::registItemTouchEvent $_2::operator() (touch ended)", SellMedalScrollView::touchEnded);
