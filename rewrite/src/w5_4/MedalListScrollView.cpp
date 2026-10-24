// MedalListScrollView: the grid of Equipment > Medal List (a BaseScrollView<unsigned long long> of medal ids).
// Each cell is a DeckMedalView ("medalView") with the MedalSelectAnimation armature ("selectView") over it; a
// map<id, bool> remembers which cells show the selection. A long press (update) reports the pressed medal.
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

// vtables (through the GOT, as the code loads them): +0 = vt + 8, +0x280 = vt + 0x360, +0x350 = vt + 0x374
constexpr uint32_t GOT_VT_MedalListScrollView = 0xFD66D4, GOT_VT_BaseScrollView_ull = 0xFD65A4;
constexpr uint32_t VT2_OFFSET = 0x360, VT3_OFFSET = 0x374;
constexpr uint32_t SIZE = 0x6B0;
// fields
constexpr uint32_t F_ROWS = 0x51C;            // IScrollProtocol's row widgets (begin, end)
constexpr uint32_t F_ITEM_W = 0x514, F_ITEM_H = 0x518;
constexpr uint32_t F_ITEM_IDS = 0x628;        // BaseScrollView<ull> item ids
constexpr uint32_t F_ON_TAP = 0x638;          // std::function<void(int)>
constexpr uint32_t F_ON_LONG_PRESS = 0x650;   // std::function<void(ull)>
constexpr uint32_t F_SELECTED = 0x668;        // std::map<ull, bool>: the cells showing the selection
constexpr uint32_t F_SHOW_TYPE = 0x678;       // DeckMedalView::ShowType
constexpr uint32_t F_DISABLED = 0x67C;        // std::vector<ull>: medals that cannot be sold (isDisableMedal)
constexpr uint32_t F_TOUCH = 0x688;           // cells take touches
constexpr uint32_t F_PRESSING = 0x689;        // a cell is held
constexpr uint32_t F_PRESS_DONE = 0x698;
constexpr uint32_t F_PRESS_TIME = 0x69C;      // float seconds held
constexpr uint32_t F_PRESS_INDEX = 0x6A0;
// lambda vtables (+8)
constexpr uint32_t VT_TouchBegan = 0xF5B2D0;  // registItemTouchEvent $_0 {this, index}
constexpr uint32_t VT_TouchMoved = 0xF5B2A4;  // $_1 {this, index, this, index, this, index} (heap 0x1C)
constexpr uint32_t VT_TouchEnded = 0xF5B24C;  // $_2 {this, index, this, index} (heap 0x14)
// strings
constexpr uint32_t S_Medal = 0xDFD05F, S_medalView = 0xDFD055, S_selectView = 0xDFD0BA;
constexpr uint32_t S_AnimJson = 0xE0043E;     // "cocostudio/publish/MedalSelectAnimation.ExportJson"
constexpr uint32_t S_Anim = 0xE00471;         // "MedalSelectAnimation"
constexpr uint32_t S_AnimSelect = 0xE00486;   // "Medal_Select01"
// APIManager: shared_ptr<UserData> (SafeBuffer: 0x58 = the support medal id)
constexpr uint32_t API_USER_DATA = 0x114;

constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_changeRow = 0x340;
constexpr uint32_t V_getAnimation = 0x298, V_animationPlay = 0x68, V_setBright = 0x238, V_setTouchEnabled = 0x29C;

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
}  // namespace

struct MedalListScrollView {
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
    static uint32_t cloneTouchBeganInto(uint32_t functor, uint32_t dst);
    static uint32_t setMedalIDs(uint32_t self, uint32_t ids);
    static uint32_t isDisableMedal(uint32_t self, uint32_t medal);
    static void setSelectedMedal(uint32_t self, uint64_t id, uint32_t selected);
};

// ------------------------------------------------------------------------------------------ lifetime
uint32_t MedalListScrollView::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    construct(me);
    if (vcall(me, V_init, {me}) == 1) {
        call(a::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t MedalListScrollView::construct(uint32_t s) {
    auto *base = rw::host_ptr<BaseScrollView>(s);
    call(a::Layout_ctor, {s});
    BaseScrollView::initScrollDefaults(base);
    base->itemIDs.begin = 0;
    base->itemIDs.end = 0;
    base->itemIDs.cap = 0;
    const uint32_t vt = ld32(GOT_VT_MedalListScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + VT2_OFFSET);
    st32(s + 0x350, vt + VT3_OFFSET);
    st32(s + F_ON_TAP + 0x10, 0);
    st32(s + F_ON_LONG_PRESS + 0x10, 0);
    st32(s + F_SELECTED + 4, 0);           // empty map: root, size, begin = &root
    st32(s + F_SELECTED + 8, 0);
    st32(s + F_SELECTED, s + F_SELECTED + 4);
    st8(s + 0x674, 0);
    st32(s + F_SHOW_TYPE, 4);
    st32(s + 0x68C, 0);
    st8(s + 0x690, 0);
    st32(s + 0x694, 0);
    st8(s + F_PRESS_DONE, 0);
    st32(s + F_PRESS_TIME, 0);
    st32(s + F_PRESS_INDEX, 0);
    st8(s + F_TOUCH, 0);
    st8(s + F_PRESSING, 0);
    st32(s + F_DISABLED + 8, 0);
    st32(s + F_DISABLED + 4, 0);
    st32(s + F_DISABLED, 0);
    call(a::Vec2_ctor, {s + 0x6A4});
    return s;
}

uint32_t MedalListScrollView::destruct(uint32_t s) {
    const uint32_t vt = ld32(GOT_VT_MedalListScrollView);
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
    call(a::Vec2_dtor, {s + 0x6A4});
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

// Cells 134x158, seven columns (the scene sets them), with the scroll bar.
uint32_t MedalListScrollView::init(uint32_t s) {
    stf(s + 0x570, 24.0f);
    stf(s + 0x574, -64.0f);
    stf(s + 0x57C, 64.0f);
    stf(s + F_ITEM_W, 134.0f);
    stf(s + F_ITEM_H, 158.0f);
    call(a::IScrollProtocol_setEnabledScrollBar, {s, 1});
    return call(a::BaseScrollViewULL_init, {s});
}

// After the scroll update: a cell held for 0.3 s reports its medal (on-long-press) once.
uint32_t MedalListScrollView::update(uint32_t s, float dt) {
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
// A cell: an empty widget of the cell size with the selection armature ("selectView", scale 0.32, over the medal)
// and a DeckMedalView ("medalView", registered with the DeckMedalViewManager). The id is not used.
uint32_t MedalListScrollView::getLayoutPanel(uint32_t s, uint32_t, uint32_t, uint32_t) {
    const uint32_t panel = call(a::Widget_create, {});
    {
        const uint32_t mgr = call(a::ArmatureDataManager_getInstance, {});
        Frame f;
        const uint32_t json = newStr(f, S_AnimJson, 0x32);
        call(a::ArmatureDataManager_addArmatureFileInfo, {mgr, json});
        strFree(json);
    }
    uint32_t armature;
    {
        Frame f;
        const uint32_t name = newStr(f, S_Anim, 0x14);
        armature = call(a::Armature_create, {name});
        strFree(name);
    }
    {
        const uint32_t anim = vcall(armature, V_getAnimation, {armature});
        Frame f;
        const uint32_t name = newStr(f, S_AnimSelect, 0xE);
        vcall(anim, V_animationPlay, {anim, name, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(name);
    }
    vcall(armature, node::setScale, {armature, fbits(0.32f)});
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

uint32_t MedalListScrollView::createRow(uint32_t s, uint32_t index, uint32_t arg) {
    const uint32_t count = uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 3);
    if (count <= index) {
        call(a::ThrowOutOfRange, {s + F_ITEM_IDS});
        return 0;
    }
    const uint32_t panel = getLayoutPanel(s, 0, 0, 0);
    vcall(s, V_changeRow, {s, panel, index, arg});
    return panel;
}

// Fill a cell with item `index`: the medal (show type of the list), white, and the selection mark of its id.
uint32_t MedalListScrollView::changeRow(uint32_t s, uint32_t widget, uint32_t index, uint32_t) {
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
    vcall(medalNode, V_setBright, {medalNode, 0});
    {
        const uint32_t white = f.alloc(4);
        call(a::Color3B_ctor, {white, 255, 255, 255});
        vcall(medalNode, node::setColor, {medalNode, white});
    }
    const uint32_t select = findNode(widget, S_selectView, 0xA);
    const uint32_t shown = call(a::MapULLBool_index, {s + F_SELECTED, id});
    vcall(select, node::setVisible, {select, ld8(shown)});
    releaseIf(ld32(medal + 4));
    return 0;
}

// Touch listener for a cell: began / moved / ended / cancelled lambdas capturing (this, index).
uint32_t MedalListScrollView::registItemTouchEvent(uint32_t s, uint32_t widget, uint32_t index) {
    if (!ld8(s + F_TOUCH)) return 0;
    const uint32_t dispatcher = ld32(call(a::Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(a::EventListenerTouchOneByOne_create, {});
    call(a::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    const uint32_t began = newFunction(f, VT_TouchBegan, {s, index});
    call(a::FunctionBTE_swap, {began, listener + 0x50});
    functionDestroy(began);
    const uint32_t captures = f.alloc(16);
    st32(captures, s);
    st32(captures + 4, index);
    st32(captures + 8, s);
    st32(captures + 12, index);
    const uint32_t moved = f.alloc(20);
    st32(moved + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x1C});
    if (obj) {
        st32(obj, VT_TouchMoved);
        for (uint32_t w = 4; w < 0x1C; w += 8) {
            st32(obj + w, s);
            st32(obj + w + 4, index);
        }
    }
    st32(moved + 16, obj);
    call(a::FunctionVTE_swap, {moved, listener + 0x68});
    functionDestroy(moved);
    assignTouchEnded(listener + 0x80, captures);
    assignTouchEnded(listener + 0x98, captures);
    call(a::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, widget});
    return 0;
}

// std::function<void(Touch*, Event*)>::operator=($_2&&): a heap __func with the four captures, swapped in
uint32_t MedalListScrollView::assignTouchEnded(uint32_t dst, uint32_t lambda) {
    Frame f;
    const uint32_t tmp = f.alloc(20);
    st32(tmp + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x14});
    if (obj) {
        st32(obj, VT_TouchEnded);
        for (uint32_t w = 0; w < 16; w += 4) st32(obj + 4 + w, ld32(lambda + w));
    }
    st32(tmp + 16, obj);
    call(a::FunctionVTE_swap, {tmp, dst});
    functionDestroy(tmp);
    return 0;
}

// registItemTouchEvent $_0 __func::__clone(__base*): placement copy {this, index}
uint32_t MedalListScrollView::cloneTouchBeganInto(uint32_t functor, uint32_t dst) {
    if (!dst) return functor;
    st32(dst, VT_TouchBegan);
    const uint32_t me = ld32(functor + 4), index = ld32(functor + 8);
    st32(dst + 8, index);
    st32(dst + 4, me);
    return index;
}

// ------------------------------------------------------------------------------------------ medal ids
// The ids to show: each gets a selection entry (false) if it has none, and the ones that cannot be sold
// (isDisableMedal) are remembered; then BaseScrollView<ull>::setItemIDs.
uint32_t MedalListScrollView::setMedalIDs(uint32_t s, uint32_t ids) {
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
            const uint64_t k = (uint64_t(hi) << 32) | lo;
            bool found = false;
            for (uint32_t node = ld32(s + F_SELECTED + 4); node;) {
                const uint64_t nk = (uint64_t(ld32(node + 0x14)) << 32) | ld32(node + 0x10);
                if (k < nk) {
                    node = ld32(node);
                } else if (nk < k) {
                    node = ld32(node + 4);
                } else {
                    found = true;
                    break;
                }
            }
            if (!found) st8(call(a::MapULLBool_index, {s + F_SELECTED, key}), 0);
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
            if (disabled == 1) {
                const uint32_t data = ld32(medal);
                const uint32_t end = ld32(s + F_DISABLED + 4);
                if (end == ld32(s + F_DISABLED + 8)) {
                    call(a::VectorULL_pushBackSlow, {s + F_DISABLED, data});
                } else {
                    uint32_t n = 0;
                    if (end) {
                        st32(end, ld32(data));
                        st32(end + 4, ld32(data + 4));
                        n = ld32(s + F_DISABLED + 4);
                    }
                    st32(s + F_DISABLED + 4, n + 8);
                }
            }
            releaseIf(ld32(medal + 4));
            i++;
            if (i >= uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 3)) break;
        }
    }
    call(a::BaseScrollViewULL_setItemIDs, {s, ids});
    releaseIf(ctl);
    return 0;
}

// Select / deselect medal `id` (only one with a selection entry): a favourite, the user's support medal (user
// data +0x58) or a locked medal (+0x26) is never selected; otherwise the decide SE and `selected`. Then the
// unsellable list is rebuilt from the item ids and the layout refreshed.
void MedalListScrollView::setSelectedMedal(uint32_t s, uint64_t id, uint32_t selected) {
    bool found = false;
    for (uint32_t node = ld32(s + F_SELECTED + 4); node;) {
        const uint64_t nk = (uint64_t(ld32(node + 0x14)) << 32) | ld32(node + 0x10);
        if (id < nk) {
            node = ld32(node);
        } else if (nk < id) {
            node = ld32(node + 4);
        } else {
            found = true;
            break;
        }
    }
    if (!found) return;
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
    const uint32_t userData = ld32(api + API_USER_DATA), ctl = ld32(api + API_USER_DATA + 4);
    addRefIf(ctl);
    const uint32_t support = safeBytes(f, userData, 0x58, 8);
    const uint64_t supportId = (uint64_t(ld32(support + 4)) << 32) | ld32(support);
    if (!favourite && id != supportId && !ld8(ld32(medal) + 0x26)) {
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
        if (disabled == 1) {
            const uint32_t data = ld32(item);
            const uint32_t last = ld32(s + F_DISABLED + 4);
            if (last == ld32(s + F_DISABLED + 8)) {
                call(a::VectorULL_pushBackSlow, {s + F_DISABLED, data});
            } else {
                uint32_t n = 0;
                if (last) {
                    st32(last, ld32(data));
                    st32(last + 4, ld32(data + 4));
                    n = ld32(s + F_DISABLED + 4);
                }
                st32(s + F_DISABLED + 4, n + 8);
            }
        }
        releaseIf(ld32(item + 4));
    }
    call(a::IScrollProtocol_updateLayout, {s, 0});
    releaseIf(ctl);
    releaseIf(ld32(medal + 4));
}

// A medal that cannot be sold: a favourite, the user's support medal, or a locked one.
uint32_t MedalListScrollView::isDisableMedal(uint32_t, uint32_t medal) {
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
    const uint32_t lo = ld32(data), hi = ld32(data + 4);
    Frame f;
    const uint32_t support = f.alloc(8);
    rw::call(a::SafeBuffer_read, {userData, support, ld32(userData + 0xC) + 0x58, 8, 0x58});
    uint32_t disabled = 1;
    if (!favourite) {
        if (((ld32(support) ^ lo) | (ld32(support + 4) ^ hi)) != 0) disabled = ld8(ld32(medal) + 0x26) != 0 ? 1 : 0;
    }
    releaseIf(ctl);
    return disabled;
}

RW_FUNCTION(0x599F24, "MedalListScrollView::create", MedalListScrollView::create);
RW_FUNCTION(0x599F78, "MedalListScrollView::MedalListScrollView", MedalListScrollView::construct);
RW_FUNCTION(0x599C68, "MedalListScrollView::~MedalListScrollView", MedalListScrollView::destruct);
RW_FUNCTION(0x59A1E8, "MedalListScrollView::init", MedalListScrollView::init);
RW_FUNCTION(0x59A244, "MedalListScrollView::update", MedalListScrollView::update);
RW_FUNCTION(0x59A2F8, "MedalListScrollView::getLayoutPanel", MedalListScrollView::getLayoutPanel);
RW_FUNCTION(0x59A608, "MedalListScrollView::createRow", MedalListScrollView::createRow);
RW_FUNCTION(0x59A660, "MedalListScrollView::changeRow", MedalListScrollView::changeRow);
RW_FUNCTION(0x59A924, "MedalListScrollView::registItemTouchEvent", MedalListScrollView::registItemTouchEvent);
RW_FUNCTION(0x59AA7C, "MedalListScrollView::registItemTouchEvent $_2 std::function::operator=(&&) (sub_59AA7C)",
            MedalListScrollView::assignTouchEnded);
RW_FUNCTION(0x59B8C8, "MedalListScrollView::registItemTouchEvent $_0 __func::__clone(__base*) (sub_59B8C8)",
            MedalListScrollView::cloneTouchBeganInto);
RW_FUNCTION(0x59AB18, "MedalListScrollView::setMedalIDs", MedalListScrollView::setMedalIDs);
RW_FUNCTION(0x59AD3C, "MedalListScrollView::isDisableMedal", MedalListScrollView::isDisableMedal);
RW_FUNCTION(0x59AE3C, "MedalListScrollView::setSelectedMedal(unsigned long long, bool)", MedalListScrollView::setSelectedMedal);
