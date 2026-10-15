// MiniMedalScrollView: construction, cells and the medal id lists (the touch lambdas are in
// MiniMedalScrollView_touch.cpp).
#include "MiniMedalScrollView.hpp"

#include "DeckMedalView.hpp"
#include "DeckMedalViewManager.hpp"

using namespace scene;
using namespace mini_medal;

namespace {
// vtables (through the GOT, as the code loads them)
constexpr uint32_t GOT_VT_MiniMedalScrollView = 0xFD65A0;
constexpr uint32_t GOT_VT_BaseScrollView_ull = 0xFD65A4;
constexpr uint32_t VT2_OFFSET = 0x360, VT3_OFFSET = 0x374;   // the +0x280 / +0x350 bases' vtables

// std::function __func vtables of registItemTouchEvent's lambdas (+8 = address point)
constexpr uint32_t VT_TouchBegan = 0xF55CD8 + 8;   // $_0 bool(Touch*, Event*)
constexpr uint32_t VT_TouchMoved = 0xF55CAC + 8;   // $_1 void(Touch*, Event*)

// library / other classes
constexpr uint32_t Layout_ctor = 0x828671;
constexpr uint32_t Vec2_ctor = 0x8E4B95;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor = 0x8DF171;
constexpr uint32_t Rect_assign = 0x8DF1C1;
constexpr uint32_t Size_ctor = 0x8DF027;            // Size(float, float)
constexpr uint32_t Memset = 0x40E7A0;               // __aeabi_memset(dst, n, c)
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Color3B_ctor = 0x9062E7;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;      // std::__vector_base_common::__throw_out_of_range
constexpr uint32_t VectorULL_dtor = 0x468499;       // __vector_base<unsigned long long>::~__vector_base
constexpr uint32_t VectorULL_assign = 0x4D7461;     // vector<ull>::assign(first, last)
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1;
constexpr uint32_t IScrollProtocol_update = 0x4F5CFD;
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t IScrollProtocol_setScrollType = 0x4F70B5;
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t IScrollProtocol_setEnabledScroll = 0x4F73C9;
constexpr uint32_t BaseScrollView_init = 0x4FC969;
constexpr uint32_t BaseScrollView_setItemIDs = 0x4FD231;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, int, UserMedal::Medal)
constexpr uint32_t MedalDataManager_isFavoriteMedal = 0x76B249;
constexpr uint32_t DeckMedalView_setData = 0x52BEE5;
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t FunctionTouchBool_swap = 0x483E4D;   // std::function<bool(Touch*,Event*)>::swap
constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;   // std::function<void(Touch*,Event*)>::swap
constexpr uint32_t EventDispatcher_addSceneGraph = 0x8F3649;

// APIManager: shared_ptr<UserData> (SafeBuffer: 0x58 = the leader / support medal id)
constexpr uint32_t APIManager_userData = 0x114;
constexpr uint32_t USERDATA_SUPPORT_MEDAL = 0x58;
// MedalDataManager::UserMedalData: +0 id (u64), +0x26 locked flag, +0x58 shared_ptr<master::Medal>
constexpr uint32_t UserMedalData_locked = 0x26;
constexpr uint32_t UserMedalData_row = 0x58;
constexpr uint32_t MEDAL_MATERIAL_FLAG = 0x454;     // master::Medal: 1 = can be a fusion material
constexpr uint32_t MEDAL_RECORD_SIZE = 0x48;        // hole::network::api::UserMedal::Medal

// widget virtual slots
constexpr uint32_t VSLOT_init = 0x248;
constexpr uint32_t VSLOT_deletingDtor = 0x04;
constexpr uint32_t VSLOT_changeRow = 0x340;
constexpr uint32_t VSLOT_setBright = 0x238;        // ui::Widget (enabled look)

constexpr uint32_t STR_medalView = 0xDFD055;         // "medalView"
constexpr uint32_t STR_Medal = 0xDFD05F;             // "Medal"

// stack-frame scratch (not compared by the verifier)
inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }

// The zero-initialised UserMedal::Medal argument of getMedalData, copied to the outgoing arguments at
// sp + 8 (the word at +0x2C is never initialised: it keeps the stack bytes of the original's local copy).
void buildMedalArgs(uint32_t sp, uint32_t localCopy) {
    fst32(sp, 0);
    for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4)
        fst32(sp + 8 + k, k == 0x2C ? ld32(localCopy + 0x2C) : 0);
}

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
}  // namespace

// ------------------------------------------------------------------------------------------ lifetime
MiniMedalScrollView::Addr MiniMedalScrollView::create() {
    uint32_t obj = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<MiniMedalScrollView>(obj));
    if (vcall(obj, VSLOT_init, {obj}) == 1) {
        call(Ref_autorelease, {obj});
        return obj;
    }
    vcall(obj, VSLOT_deletingDtor, {obj});
    return 0;
}
RW_FUNCTION(0x664D50, "MiniMedalScrollView::create", MiniMedalScrollView::create);

MiniMedalScrollView *MiniMedalScrollView::construct(MiniMedalScrollView *self) {
    uint32_t s = rw::guest_addr(self);
    call(Layout_ctor, {s});
    // IScrollProtocol
    self->field_4FC = 0.1f;
    self->field_500 = 0.3f;
    self->field_504 = 0;
    self->field_508 = 0;
    self->field_50C = 0;
    self->field_510 = 200.0f;
    call(Memset, {s + 0x514, 0x24, 0});
    self->maxColumn = 1;
    rw::set<uint8_t>(self, 0x53C, 1);
    rw::set<uint32_t>(self, 0x590, 0);
    rw::set<uint8_t>(self, 0x594, 0);
    rw::set<uint32_t>(self, 0x5C4, 0);
    rw::set<uint32_t>(self, 0x5D8, 0);
    rw::set<uint32_t>(self, 0x5F0, 0);
    rw::set<uint32_t>(self, 0x608, 0);
    rw::set<uint32_t>(self, 0x620, 0);
    call(Memset, {s + 0x540, 0x12, 0});
    call(Memset, {s + 0x554, 0x12, 0});
    call(Memset, {s + 0x568, 0x25, 0});
    call(Memset, {s + 0x598, 0x2A, 0});
    // BaseScrollView<unsigned long long>
    self->itemIds.begin = 0;
    self->itemIds.end = 0;
    self->itemIds.cap = 0;
    // MiniMedalScrollView
    uint32_t vt = ld32(GOT_VT_MiniMedalScrollView);
    self->vtable = vt + 8;
    self->vtable2 = vt + VT2_OFFSET;
    self->vtable3 = vt + VT3_OFFSET;
    rw::set<uint32_t>(self, 0x648, 0);    // the three std::functions are empty
    rw::set<uint32_t>(self, 0x660, 0);
    rw::set<uint32_t>(self, 0x678, 0);
    self->field_688 = 0;
    rw::set<uint32_t>(self, 0x684, 0);
    self->field_680 = 0;
    call(Vec2_ctor, {s + 0x68C});
    call(Vec2_ctor, {s + 0x694});
    self->touchIndex = 0;
    self->hiddenItem = 0;
    self->dragPanel = 0;
    call(Rect_ctor, {s + 0x6A8});
    call(Memset, {s + OFF_BURST_UP, 0x43, 0});
    return self;
}
RW_FUNCTION(0x67CFD4, "MiniMedalScrollView::MiniMedalScrollView", MiniMedalScrollView::construct);

MiniMedalScrollView *MiniMedalScrollView::destruct(MiniMedalScrollView *self) {
    uint32_t s = rw::guest_addr(self);
    uint32_t vt = ld32(GOT_VT_MiniMedalScrollView);
    self->vtable = vt + 8;
    self->vtable2 = vt + VT2_OFFSET;
    self->vtable3 = vt + VT3_OFFSET;
    // unregister the cells' medal views
    uint32_t rows = self->rowsBegin;
    if (int32_t(self->rowsEnd - rows) >= 1) {
        for (int32_t k = 0;;) {
            uint32_t row = ld32(rows + 4 * uint32_t(k));
            uint32_t view = findNode(row, STR_medalView, 9);
            if (view) DeckMedalViewManager::removeDeckMedalView(DeckMedalViewManager::getInstance(), view);
            rows = self->rowsBegin;
            k++;
            if (k >= int32_t(self->rowsEnd - rows) >> 2) break;
        }
    }
    call(VectorULL_dtor, {s + OFF_NO_EFFECT});
    call(VectorULL_dtor, {s + OFF_USED});
    call(VectorULL_dtor, {s + OFF_DISABLE_MEDAL});
    call(VectorULL_dtor, {s + OFF_DISABLED});
    call(VectorULL_dtor, {s + OFF_BURST_UP});
    call(Vec2_dtor, {s + 0x6A8});          // targetRect.origin
    call(Vec2_dtor, {s + 0x694});
    call(Vec2_dtor, {s + 0x68C});
    functionDestroy(s + 0x668);
    functionDestroy(s + 0x650);
    functionDestroy(s + 0x638);
    // ~BaseScrollView<unsigned long long>
    uint32_t bvt = ld32(GOT_VT_BaseScrollView_ull);
    self->vtable = bvt + 8;
    self->vtable2 = bvt + VT2_OFFSET;
    self->vtable3 = bvt + VT3_OFFSET;
    call(VectorULL_dtor, {s + OFF_ITEM_IDS});
    call(IScrollProtocol_dtor, {s});
    return self;
}
RW_FUNCTION(0x4FC56C, "MiniMedalScrollView::~MiniMedalScrollView", MiniMedalScrollView::destruct);

void MiniMedalScrollView::destructDelete(MiniMedalScrollView *self) {
    call(fn::operator_delete, {rw::guest_addr(destruct(self))});
}
RW_FUNCTION(0x4FC898, "MiniMedalScrollView::~MiniMedalScrollView (deleting)", MiniMedalScrollView::destructDelete);

uint32_t MiniMedalScrollView::destructThunk640(uint32_t base2) {
    return rw::guest_addr(destruct(rw::host_ptr<MiniMedalScrollView>(base2 - 0x280)));
}
RW_FUNCTION(0x4FC878, "non-virtual thunk to MiniMedalScrollView::~MiniMedalScrollView (+640)",
            MiniMedalScrollView::destructThunk640);

uint32_t MiniMedalScrollView::destructThunk848(uint32_t base3) {
    return rw::guest_addr(destruct(rw::host_ptr<MiniMedalScrollView>(base3 - 0x350)));
}
RW_FUNCTION(0x4FC888, "non-virtual thunk to MiniMedalScrollView::~MiniMedalScrollView (+848)",
            MiniMedalScrollView::destructThunk848);

void MiniMedalScrollView::destructDeleteThunk640(uint32_t base2) {
    call(fn::operator_delete, {destructThunk640(base2)});
}
RW_FUNCTION(0x4FC8A4, "non-virtual thunk to MiniMedalScrollView::~MiniMedalScrollView (+640, deleting)",
            MiniMedalScrollView::destructDeleteThunk640);

void MiniMedalScrollView::destructDeleteThunk848(uint32_t base3) {
    call(fn::operator_delete, {destructThunk848(base3)});
}
RW_FUNCTION(0x4FC8B8, "non-virtual thunk to MiniMedalScrollView::~MiniMedalScrollView (+848, deleting)",
            MiniMedalScrollView::destructDeleteThunk848);

// Cell size 120x164, three columns; no scroll bar (and no scrolling) during tutorial phases 220 / 700.
bool MiniMedalScrollView::init(MiniMedalScrollView *self) {
    uint32_t s = rw::guest_addr(self);
    self->itemWidth = 120.0f;
    self->itemHeight = 164.0f;
    self->field_57C = 60.0f;
    self->field_570 = 16.0f;
    self->enabledTapItem = 1;
    call(IScrollProtocol_setScrollType, {s, 0});
    if (SceneUtil::getNexTutorialphase() == 220 || SceneUtil::getNexTutorialphase() == 700) {
        call(IScrollProtocol_setEnabledScrollBar, {s, 0});
        call(IScrollProtocol_setEnabledScroll, {s, 0});
    } else {
        call(IScrollProtocol_setEnabledScrollBar, {s, 1});
    }
    call(IScrollProtocol_setMaxColumn, {s, 3});
    return call(BaseScrollView_init, {s}) != 0;
}
RW_FUNCTION(0x4FC8CC, "MiniMedalScrollView::init", MiniMedalScrollView::init);

void MiniMedalScrollView::update(MiniMedalScrollView *self, float dt) {
    call(IScrollProtocol_update, {rw::guest_addr(self), rw::fbits(dt)});
}
RW_FUNCTION(0x4FC998, "MiniMedalScrollView::update", MiniMedalScrollView::update);

// ------------------------------------------------------------------------------------------ cells
// A cell: an empty widget of the cell size holding a DeckMedalView named "medalView" (registered with the
// DeckMedalViewManager for the blinking status plates). The id is not used.
MiniMedalScrollView::Addr MiniMedalScrollView::getLayoutPanel(MiniMedalScrollView *self, uint64_t, uint32_t smallLayout) {
    uint32_t panel = call(Widget_create, {});
    uint32_t view = DeckMedalView::create(smallLayout);
    if (smallLayout == 0) rw::store8(rw::host_ptr<uint8_t>(view + 0x3BA), 0);
    vcall(panel, node::setAnchorPoint, {panel, VEC2_ANCHOR_MIDDLE});
    {
        Frame f;
        uint32_t name = newStr(f, STR_medalView, 9);
        vcall(panel, node::addChildZName, {panel, view, 0, name});
        strFree(name);
    }
    {
        Frame f;
        uint32_t size = f.alloc(8);
        call(Size_ctor, {size, rw::fbits(self->itemWidth), rw::fbits(self->itemHeight)});
        vcall(panel, node::setContentSize, {panel, size});
    }
    vcall(panel, node::setPositionX, {panel, rw::fbits(self->itemWidth * 0.5f)});
    DeckMedalViewManager::addDeckMedalView(DeckMedalViewManager::getInstance(), view);
    return panel;
}
RW_FUNCTION(0x4FC9A0, "MiniMedalScrollView::getLayoutPanel", MiniMedalScrollView::getLayoutPanel);

MiniMedalScrollView::Addr MiniMedalScrollView::createRow(MiniMedalScrollView *self, int32_t index, int32_t arg) {
    uint32_t s = rw::guest_addr(self);
    uint32_t count = uint32_t(int32_t(self->itemIds.end - self->itemIds.begin) >> 3);
    if (count <= uint32_t(index)) {
        call(ThrowOutOfRange, {s + OFF_ITEM_IDS});
        return 0;
    }
    uint32_t panel = getLayoutPanel(self, 0, 1);
    vcall(s, VSLOT_changeRow, {s, panel, uint32_t(index), uint32_t(arg)});
    return panel;
}
RW_FUNCTION(0x4FCAD4, "MiniMedalScrollView::createRow", MiniMedalScrollView::createRow);

// Fill a cell with item `index`: the medal, its flags (used / no effect) and a grey tint when it cannot be picked.
uint32_t MiniMedalScrollView::changeRow(MiniMedalScrollView *self, Addr widget, uint32_t index, int32_t) {
    uint32_t s = rw::guest_addr(self);
    // the original frame (getMedalData / setData take stack arguments; the medal record has a stale word)
    Frame fr;
    const uint32_t sp = fr.allocExact(0x128);
    const uint32_t color1 = sp + 0x90, color2 = sp + 0x94, detail = sp + 0x98, medal = sp + 0xA0;
    const uint32_t record = sp + 0xC0;

    uint32_t begin = self->itemIds.begin;
    if (!(uint32_t(int32_t(self->itemIds.end - begin) >> 3) > index)) {
        call(ThrowOutOfRange, {s + OFF_ITEM_IDS});
        return 0;
    }
    uint32_t medalNode = findNode(widget, STR_Medal, 5);
    if (!medalNode) return 1;
    uint32_t view = findNode(medalNode, STR_medalView, 9);
    DeckMedalView *dv = rw::host_ptr<DeckMedalView>(view);
    dv->mixMode = self->mixMode;
    dv->isShowMixInfo = self->isShowMixInfo;
    DeckMedalView::changeShowType(dv, self->showType);

    uint32_t idAddr = begin + 8 * index;
    uint32_t lo = ld32(idAddr), hi = ld32(idAddr + 4);
    bool inDisabled = vecFind(self->disabledIds, lo, hi) != self->disabledIds.end;
    bool inDisable = vecFind(self->disableMedalIds, lo, hi) != self->disableMedalIds.end;
    bool inUsed = vecFind(self->usedIds, lo, hi) != self->usedIds.end;
    bool inNoEffect = vecFind(self->noEffectIds, lo, hi) != self->noEffectIds.end;
    if (self->mixMode) dv->mixSelected = inDisable ? 1 : 0;
    dv->noEffect = (inNoEffect && !inDisable && !inUsed) ? 1 : 0;

    uint32_t mgr = call(MedalDataManager_getInstance, {});
    lo = ld32(idAddr);
    hi = ld32(idAddr + 4);
    buildMedalArgs(sp, record);
    rw::call_exact(MedalDataManager_getMedalData, {medal, mgr, lo, hi});
    fst32(detail, 0);
    fst32(detail + 4, 0);
    fst32(sp + 0x00, 1);
    fst32(sp + 0x04, 10000);
    fst32(sp + 0x08, 10000);
    fst32(sp + 0x0C, 0);
    fst32(sp + 0x10, 0);
    fst32(sp + 0x14, 0);
    fst32(sp + 0x18, detail);
    rw::call_exact(DeckMedalView_setData, {view, medal, 0xFFFFFFFFu, 4});
    releaseIf(ld32(detail + 4));
    releaseIf(ld32(medal + 4));

    if (!inDisabled && !inDisable && !inNoEffect) {
        vcall(medalNode, VSLOT_setBright, {medalNode, 0});
        call(Color3B_ctor, {color1, 255, 255, 255});
        vcall(medalNode, node::setColor, {medalNode, color1});
    } else {
        vcall(medalNode, VSLOT_setBright, {medalNode, 1});
        call(Color3B_ctor, {color2, 100, 100, 100});
        vcall(medalNode, node::setColor, {medalNode, color2});
    }
    return 0;
}
RW_FUNCTION(0x4FCB30, "MiniMedalScrollView::changeRow", MiniMedalScrollView::changeRow);

// Touch listener for a cell: began / moved / ended / cancelled lambdas capturing (this, index[, widget]).
void MiniMedalScrollView::registItemTouchEvent(MiniMedalScrollView *self, Addr widget, int32_t index) {
    uint32_t s = rw::guest_addr(self);
    Frame fr;
    const uint32_t sp = fr.allocExact(0x50);
    const uint32_t lambda = sp + 0x10, fnTmp = sp + 0x18;
    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});

    fst32(fnTmp + 16, fnTmp);
    fst32(fnTmp, VT_TouchBegan);
    fst32(fnTmp + 8, uint32_t(index));
    fst32(fnTmp + 4, s);
    call(FunctionTouchBool_swap, {fnTmp, listener + 0x50});     // onTouchBegan
    functionDestroy(fnTmp);

    fst32(fnTmp + 16, fnTmp);
    fst32(fnTmp, VT_TouchMoved);
    fst32(fnTmp + 4, s);
    fst32(fnTmp + 8, uint32_t(index));
    fst32(fnTmp + 12, widget);
    call(FunctionTouchVoid_swap, {fnTmp, listener + 0x68});     // onTouchMoved
    functionDestroy(fnTmp);

    fst32(lambda, s);
    fst32(lambda + 4, uint32_t(index));
    assignTouchEnded(listener + 0x80, lambda);                  // onTouchEnded
    assignTouchEnded(listener + 0x98, lambda);                  // onTouchCancelled
    uint32_t dispatcher = ld32(call(fn::Director_getInstance, {}) + 0x84);
    call(EventDispatcher_addSceneGraph, {dispatcher, listener, widget});
}
RW_FUNCTION(0x4FD268, "MiniMedalScrollView::registItemTouchEvent", MiniMedalScrollView::registItemTouchEvent);

// ------------------------------------------------------------------------------------------ medal lists
// A medal that cannot be picked: in the fusion picker, a medal whose master row forbids it as material;
// otherwise a favourite, the user's support medal, or a locked medal.
bool MiniMedalScrollView::isDisabledMedal(MiniMedalScrollView *self, Addr medal) {
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t udCtl = ld32(api + APIManager_userData + 4);
    uint32_t userData = ld32(api + APIManager_userData);
    if (udCtl) call(fn::shared_add, {udCtl});
    bool disabled;
    if (self->mixMode) {
        uint32_t row = ld32(ld32(medal) + UserMedalData_row);
        disabled = safe32(row, MEDAL_MATERIAL_FLAG) != 1;
    } else {
        uint32_t mgr = call(MedalDataManager_getInstance, {});
        Frame f;
        uint32_t copy = f.alloc(8);
        fst32(copy, ld32(medal));
        uint32_t ctl = ld32(medal + 4);
        fst32(copy + 4, ctl);
        if (ctl) call(fn::shared_add, {ctl});
        uint32_t favourite = call(MedalDataManager_isFavoriteMedal, {mgr, copy});
        releaseIf(ld32(copy + 4));
        uint32_t data = ld32(medal);
        uint32_t lo = ld32(data), hi = ld32(data + 4);
        uint32_t supportLo = uint32_t(safe32(userData, USERDATA_SUPPORT_MEDAL));
        uint32_t supportHi = uint32_t(safe32(userData, USERDATA_SUPPORT_MEDAL + 4));
        if (favourite) disabled = true;
        else if (lo == supportLo && hi == supportHi) disabled = true;
        else disabled = ld8(ld32(medal) + UserMedalData_locked) != 0;
    }
    releaseIf(udCtl);
    return disabled;
}
RW_FUNCTION(0x4FCF14, "MiniMedalScrollView::isDisabledMedal", MiniMedalScrollView::isDisabledMedal);

void MiniMedalScrollView::updateDisabledItemIDs(MiniMedalScrollView *self) {
    Frame fr;
    const uint32_t sp = fr.allocExact(0x108);
    const uint32_t copy = sp + 0x88, got = sp + 0x90, idTmp = sp + 0x98, record = sp + 0xA0;
    vecClear(self->disabledIds);
    if (self->itemIds.end == self->itemIds.begin) return;
    for (uint32_t i = 0;; i++) {
        uint32_t at = self->itemIds.begin + 8 * i;
        uint32_t lo = ld32(at), hi = ld32(at + 4);
        fst32(idTmp, lo);
        fst32(idTmp + 4, hi);
        uint32_t mgr = call(MedalDataManager_getInstance, {});
        buildMedalArgs(sp, record);
        rw::call_exact(MedalDataManager_getMedalData, {got, mgr, lo, hi});
        fst32(copy, ld32(got));
        uint32_t ctl = ld32(got + 4);
        fst32(copy + 4, ctl);
        if (ctl) call(fn::shared_add, {ctl});
        bool disabled = isDisabledMedal(self, copy);
        releaseIf(ctl);
        if (disabled) vecPushBack(self->disabledIds, lo, hi, idTmp);
        releaseIf(ld32(got + 4));
        if (!(i + 1 < uint32_t(int32_t(self->itemIds.end - self->itemIds.begin) >> 3))) break;
    }
}
RW_FUNCTION(0x4FD058, "MiniMedalScrollView::updateDisabledItemIDs", MiniMedalScrollView::updateDisabledItemIDs);

void MiniMedalScrollView::setItemIDs(MiniMedalScrollView *self, Addr ids) {
    uint32_t s = rw::guest_addr(self);
    if (s + OFF_ITEM_IDS != ids) call(VectorULL_assign, {s + OFF_ITEM_IDS, ld32(ids), ld32(ids + 4)});
    updateDisabledItemIDs(self);
    call(BaseScrollView_setItemIDs, {s, ids});
}
RW_FUNCTION(0x4FD1FC, "MiniMedalScrollView::setItemIDs", MiniMedalScrollView::setItemIDs);

void MiniMedalScrollView::setTargetRect(MiniMedalScrollView *self, Addr rect) {
    call(Rect_assign, {rw::guest_addr(self) + 0x6A8, rect});
}
RW_FUNCTION(0x4FD3FC, "MiniMedalScrollView::setTargetRect", MiniMedalScrollView::setTargetRect);

void MiniMedalScrollView::setBurstUpMedalIDs(MiniMedalScrollView *self, Addr ids) {
    uint32_t v = rw::guest_addr(self) + OFF_BURST_UP;
    if (v != ids) call(VectorULL_assign, {v, ld32(ids), ld32(ids + 4)});
}
RW_FUNCTION(0x4FD40A, "MiniMedalScrollView::setBurstUpMedalIDs", MiniMedalScrollView::setBurstUpMedalIDs);

// Grey a medal out (disable) or back in; relayout redraws the cells.
void MiniMedalScrollView::setDisableMedalID(MiniMedalScrollView *self, uint64_t id, uint32_t disable,
                                            uint32_t relayout) {
    Frame f;
    const uint32_t idTmp = f.alloc(8);
    const uint32_t lo = uint32_t(id), hi = uint32_t(id >> 32);
    fst32(idTmp, lo);
    fst32(idTmp + 4, hi);
    uint32_t end = self->disableMedalIds.end;
    uint32_t found = vecFind(self->disableMedalIds, lo, hi);
    if (disable == 1) {
        if (found == end) vecPushBack(self->disableMedalIds, lo, hi, idTmp);
    } else if (found != end) {
        vecErase(self->disableMedalIds, found);
    }
    if (relayout == 1) call(IScrollProtocol_updateLayout, {rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4FD424, "MiniMedalScrollView::setDisableMedalID", MiniMedalScrollView::setDisableMedalID);

void MiniMedalScrollView::changeShowType(MiniMedalScrollView *self, int32_t type) {
    self->showType = type;
    call(IScrollProtocol_updateLayout, {rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4FD530, "MiniMedalScrollView::changeShowType", MiniMedalScrollView::changeShowType);

void MiniMedalScrollView::setIsShowMixInfo(MiniMedalScrollView *self, uint32_t show) {
    self->isShowMixInfo = uint8_t(show);
    call(IScrollProtocol_updateLayout, {rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4FD544, "MiniMedalScrollView::setIsShowMixInfo", MiniMedalScrollView::setIsShowMixInfo);

// A fusion material can be picked when it is not disabled, not greyed out, not in use and has an effect.
bool MiniMedalScrollView::canSelectMaterialMedal(MiniMedalScrollView *self, Addr medal) {
    bool disabled;
    {
        Frame f;
        uint32_t copy = f.alloc(8);
        fst32(copy, ld32(medal));
        uint32_t ctl = ld32(medal + 4);
        fst32(copy + 4, ctl);
        if (ctl) call(fn::shared_add, {ctl});
        disabled = isDisabledMedal(self, copy);
        releaseIf(ctl);
    }
    if (disabled) return false;
    uint32_t data = ld32(medal);
    uint32_t lo = ld32(data), hi = ld32(data + 4);
    bool inDisable = vecFind(self->disableMedalIds, lo, hi) != self->disableMedalIds.end;
    bool inUsed = vecFind(self->usedIds, lo, hi) != self->usedIds.end;
    bool inNoEffect = vecFind(self->noEffectIds, lo, hi) != self->noEffectIds.end;
    return !inDisable && !inUsed && !inNoEffect;
}
RW_FUNCTION(0x4FD558, "MiniMedalScrollView::canSelectMaterialMedal", MiniMedalScrollView::canSelectMaterialMedal);

void MiniMedalScrollView::setEnabledTapItem(MiniMedalScrollView *self, uint32_t enabled) {
    self->enabledTapItem = uint8_t(enabled);
}
RW_FUNCTION(0x4FD6AC, "MiniMedalScrollView::setEnabledTapItem", MiniMedalScrollView::setEnabledTapItem);

void MiniMedalScrollView::clearNoEffectMedalIDs(MiniMedalScrollView *self) { vecClear(self->noEffectIds); }
RW_FUNCTION(0x4FD6B8, "MiniMedalScrollView::clearNoEffectMedalIDs", MiniMedalScrollView::clearNoEffectMedalIDs);

void MiniMedalScrollView::addNoEffectMedalID(MiniMedalScrollView *self, uint64_t id) {
    Frame f;
    const uint32_t idTmp = f.alloc(8);
    const uint32_t lo = uint32_t(id), hi = uint32_t(id >> 32);
    fst32(idTmp, lo);
    fst32(idTmp + 4, hi);
    vecPushBack(self->noEffectIds, lo, hi, idTmp);
}
RW_FUNCTION(0x4FD6E0, "MiniMedalScrollView::addNoEffectMedalID", MiniMedalScrollView::addNoEffectMedalID);

void MiniMedalScrollView::clearDisableMedals(MiniMedalScrollView *self) {
    vecClear(self->disabledIds);
    vecClear(self->disableMedalIds);
}
RW_FUNCTION(0x6654E8, "MiniMedalScrollView::clearDisableMedals", MiniMedalScrollView::clearDisableMedals);
