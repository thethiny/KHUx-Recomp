// MiniMedalScrollView: the touch handling of a cell (registItemTouchEvent's lambdas) and their std::function
// __func thunks. A touch on a cell either taps the medal (onTapItem) or, once it moved sideways far enough,
// lifts a copy of the cell onto the popup layer and drags it; on release the copy flies to the drop target
// (getDropTarget / onDropItem) or back to where it came from, then fades out.
#include "MiniMedalScrollView.hpp"

#include "DeckMedalView.hpp"
#include "DeckMedalViewManager.hpp"

using namespace scene;
using namespace mini_medal;

namespace {
// __func vtables (+8 = address point) and the lambdas' type names / typeinfos
constexpr uint32_t VT_TouchEnded = 0xF55C28 + 8;     // $_2: [scroll, index]
constexpr uint32_t VT_DropFinished = 0xF55C54 + 8;   // $_2::operator()::{lambda()#1}: [scroll, moved]
constexpr uint32_t VT_PanelFaded = 0xF55C80 + 8;     // ...::{lambda()#1}::operator()::{lambda()#1}: [scroll]
constexpr uint32_t VT_TouchMoved = 0xF55CAC + 8;     // $_1: [scroll, index, widget]
constexpr uint32_t VT_TouchBegan = 0xF55CD8 + 8;     // $_0: [scroll, index]
constexpr uint32_t NAME_TouchEnded = 0xE200F0, TI_TouchEnded = 0xF81900;
constexpr uint32_t NAME_DropFinished = 0xE201F0, TI_DropFinished = 0xF81914;
constexpr uint32_t NAME_PanelFaded = 0xE20310, TI_PanelFaded = 0xF81928;
constexpr uint32_t NAME_TouchMoved = 0xE20430, TI_TouchMoved = 0xF8193C;
constexpr uint32_t NAME_TouchBegan = 0xE20520, TI_TouchBegan = 0xF81950;

constexpr uint32_t FunctionTouchVoid_swap = 0x4F3699;   // std::function<void(Touch*,Event*)>::swap
constexpr uint32_t FunctionTapItem_call = 0x4FDB21;      // std::function<void(ull,int)>::operator()
constexpr uint32_t FunctionDropTarget_call = 0x4FDBA9;   // std::function<int(ull,Vec2,float*)>::operator()
constexpr uint32_t FunctionDropItem_call = 0x4FDC31;     // std::function<void(int,ull)>::operator()
constexpr uint32_t Touch_getLocation = 0x900E57;         // (Vec2* out, Touch*)
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;   // (Vec2* out, Node*, const Vec2&)
constexpr uint32_t Node_convertToWorldSpace = 0x895A39;
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_ctor4 = 0x8DF18B;                // Rect(x, y, w, h)
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t FadeOut_create = 0x86B25D;
constexpr uint32_t NewArray = 0xC98F6F, DeleteArray = 0xC98FB3;
constexpr uint32_t DivSI3 = 0xCD5E8D;                    // __divsi3
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t IScrollProtocol_setEnabledScroll = 0x4F73C9;
constexpr uint32_t IScrollProtocol_getItemPixel = 0x4F6751;
constexpr uint32_t IScrollProtocol_containsPointInScrollArea = 0x4F7D21;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t DeckMedalView_setData = 0x52BEE5;

constexpr uint32_t STR_medalView = 0xDFD055;
constexpr uint32_t MEDAL_RECORD_SIZE = 0x48;
constexpr uint32_t VSLOT_isVisible = 0xB4;

inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }
inline void fstf(uint32_t a, float v) { fst32(a, rw::fbits(v)); }
inline MiniMedalScrollView *scrollOf(uint32_t functor) { return rw::host_ptr<MiniMedalScrollView>(ld32(functor + 4)); }
inline uint32_t itemCount(const MiniMedalScrollView *s) {
    return uint32_t(int32_t(s->itemIds.end - s->itemIds.begin) >> 3);
}
// the item under a cell: column `index` of the first visible row
inline int32_t itemNumber(MiniMedalScrollView *s, int32_t index) {
    int32_t offset = f2i(s->scrollOffset);
    int32_t pixel = f2i(rw::callf(IScrollProtocol_getItemPixel, {rw::guest_addr(s)}));
    int32_t row = int32_t(call(DivSI3, {uint32_t(offset), uint32_t(pixel)}));
    return int32_t(uint32_t(s->maxColumn) * uint32_t(row) + uint32_t(index));
}
inline uint32_t popupLayer() { return call(fn::SceneManager_getPopupLayer, {call(fn::SceneManager_getInstance, {})}); }
inline void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// no scrolling during tutorial phases 220 / 700
inline uint32_t scrollAllowed() {
    if (SceneUtil::getNexTutorialphase() == 220) return 0;
    return SceneUtil::getNexTutorialphase() == 700 ? 0 : 1;
}

// ---- std::function __func thunks (vtable slots 1-8); the captured words follow the vtable pointer
uint32_t cloneFunc(uint32_t vt, uint32_t size, uint32_t self) {
    uint32_t p = call(fn::operator_new, {size});
    if (p) {
        st32(p, vt);
        for (uint32_t k = 4; k < size; k += 4) st32(p + k, ld32(self + k));
    }
    return p;
}
void cloneInto(uint32_t vt, uint32_t size, uint32_t self, uint32_t dst) {
    if (!dst) return;
    st32(dst, vt);
    for (uint32_t k = 4; k < size; k += 4) st32(dst + k, ld32(self + k));
}
uint32_t target(uint32_t name, uint32_t self, uint32_t typeInfo) { return ld32(typeInfo + 4) == name ? self + 4 : 0; }
void deleteFunc(uint32_t self) { call(fn::operator_delete, {self}); }
void destroyNothing(uint32_t) {}

// $_2 (touch ended / cancelled)
void TouchEnded_deleting(uint32_t self) { deleteFunc(self); }
uint32_t TouchEnded_clone(uint32_t self) { return cloneFunc(VT_TouchEnded, 0xC, self); }
void TouchEnded_cloneInto(uint32_t self, uint32_t dst) { cloneInto(VT_TouchEnded, 0xC, self, dst); }
void TouchEnded_destroy(uint32_t self) { destroyNothing(self); }
void TouchEnded_destroyDeallocate(uint32_t self) { deleteFunc(self); }
uint32_t TouchEnded_target(uint32_t self, uint32_t ti) { return target(NAME_TouchEnded, self, ti); }
uint32_t TouchEnded_targetType() { return TI_TouchEnded; }
// $_2's lambda (drop animation finished)
void DropFinished_deleting(uint32_t self) { deleteFunc(self); }
uint32_t DropFinished_clone(uint32_t self) { return cloneFunc(VT_DropFinished, 0xC, self); }
void DropFinished_cloneInto(uint32_t self, uint32_t dst) { cloneInto(VT_DropFinished, 0xC, self, dst); }
void DropFinished_destroy(uint32_t self) { destroyNothing(self); }
void DropFinished_destroyDeallocate(uint32_t self) { deleteFunc(self); }
uint32_t DropFinished_target(uint32_t self, uint32_t ti) { return target(NAME_DropFinished, self, ti); }
uint32_t DropFinished_targetType() { return TI_DropFinished; }
// the panel fade-out's lambda
void PanelFaded_deleting(uint32_t self) { deleteFunc(self); }
uint32_t PanelFaded_clone(uint32_t self) { return cloneFunc(VT_PanelFaded, 0x8, self); }
void PanelFaded_cloneInto(uint32_t self, uint32_t dst) { cloneInto(VT_PanelFaded, 0x8, self, dst); }
void PanelFaded_destroy(uint32_t self) { destroyNothing(self); }
void PanelFaded_destroyDeallocate(uint32_t self) { deleteFunc(self); }
uint32_t PanelFaded_target(uint32_t self, uint32_t ti) { return target(NAME_PanelFaded, self, ti); }
uint32_t PanelFaded_targetType() { return TI_PanelFaded; }
// $_1 (touch moved)
void TouchMoved_deleting(uint32_t self) { deleteFunc(self); }
uint32_t TouchMoved_clone(uint32_t self) { return cloneFunc(VT_TouchMoved, 0x10, self); }
void TouchMoved_cloneInto(uint32_t self, uint32_t dst) { cloneInto(VT_TouchMoved, 0x10, self, dst); }
void TouchMoved_destroy(uint32_t self) { destroyNothing(self); }
void TouchMoved_destroyDeallocate(uint32_t self) { deleteFunc(self); }
uint32_t TouchMoved_target(uint32_t self, uint32_t ti) { return target(NAME_TouchMoved, self, ti); }
uint32_t TouchMoved_targetType() { return TI_TouchMoved; }
// $_0 (touch began)
void TouchBegan_deleting(uint32_t self) { deleteFunc(self); }
uint32_t TouchBegan_clone(uint32_t self) { return cloneFunc(VT_TouchBegan, 0xC, self); }
void TouchBegan_cloneInto(uint32_t self, uint32_t dst) { cloneInto(VT_TouchBegan, 0xC, self, dst); }
void TouchBegan_destroy(uint32_t self) { destroyNothing(self); }
void TouchBegan_destroyDeallocate(uint32_t self) { deleteFunc(self); }
uint32_t TouchBegan_target(uint32_t self, uint32_t ti) { return target(NAME_TouchBegan, self, ti); }
uint32_t TouchBegan_targetType() { return TI_TouchBegan; }

// unreachable outlined vector::at() throws placed before MiniMedalScrollView::create
void throwStubA(uint32_t, uint32_t vec) { call(ThrowOutOfRange, {vec}); }
void throwStubB(uint32_t vec) { call(ThrowOutOfRange, {vec}); }
}  // namespace

RW_FUNCTION(0x4FD750, "MiniMedalScrollView::registItemTouchEvent::$_2 __func::~__func (deleting)", TouchEnded_deleting);
RW_FUNCTION(0x4FD758, "MiniMedalScrollView::registItemTouchEvent::$_2 __func::__clone()", TouchEnded_clone);
RW_FUNCTION(0x4FD788, "MiniMedalScrollView::registItemTouchEvent::$_2 __func::__clone(__base*)", TouchEnded_cloneInto);
RW_FUNCTION(0x4FD7AC, "MiniMedalScrollView::registItemTouchEvent::$_2 __func::destroy (nullsub_292)", TouchEnded_destroy);
RW_FUNCTION(0x4FD7AE, "MiniMedalScrollView::registItemTouchEvent::$_2 __func::destroy_deallocate", TouchEnded_destroyDeallocate);
RW_FUNCTION(0x4FDAEC, "MiniMedalScrollView::registItemTouchEvent::$_2 __func::target", TouchEnded_target);
RW_FUNCTION(0x4FDB0C, "MiniMedalScrollView::registItemTouchEvent::$_2 __func::target_type", TouchEnded_targetType);
RW_FUNCTION(0x4FDCB4, "MiniMedalScrollView::onDropFinished __func::~__func (deleting)", DropFinished_deleting);
RW_FUNCTION(0x4FDCBC, "MiniMedalScrollView::onDropFinished __func::__clone()", DropFinished_clone);
RW_FUNCTION(0x4FDCEC, "MiniMedalScrollView::onDropFinished __func::__clone(__base*)", DropFinished_cloneInto);
RW_FUNCTION(0x4FDD10, "MiniMedalScrollView::onDropFinished __func::destroy (nullsub_293)", DropFinished_destroy);
RW_FUNCTION(0x4FDD12, "MiniMedalScrollView::onDropFinished __func::destroy_deallocate", DropFinished_destroyDeallocate);
RW_FUNCTION(0x4FDF28, "MiniMedalScrollView::onDropFinished __func::target", DropFinished_target);
RW_FUNCTION(0x4FDF48, "MiniMedalScrollView::onDropFinished __func::target_type", DropFinished_targetType);
RW_FUNCTION(0x4FDF5C, "MiniMedalScrollView::onDragPanelFaded __func::~__func (deleting)", PanelFaded_deleting);
RW_FUNCTION(0x4FDF64, "MiniMedalScrollView::onDragPanelFaded __func::__clone()", PanelFaded_clone);
RW_FUNCTION(0x4FDF90, "MiniMedalScrollView::onDragPanelFaded __func::__clone(__base*)", PanelFaded_cloneInto);
RW_FUNCTION(0x4FDFB0, "MiniMedalScrollView::onDragPanelFaded __func::destroy (nullsub_294)", PanelFaded_destroy);
RW_FUNCTION(0x4FDFB2, "MiniMedalScrollView::onDragPanelFaded __func::destroy_deallocate", PanelFaded_destroyDeallocate);
RW_FUNCTION(0x4FE09C, "MiniMedalScrollView::onDragPanelFaded __func::target", PanelFaded_target);
RW_FUNCTION(0x4FE0BC, "MiniMedalScrollView::onDragPanelFaded __func::target_type", PanelFaded_targetType);
RW_FUNCTION(0x4FE0D0, "MiniMedalScrollView::registItemTouchEvent::$_1 __func::~__func (deleting)", TouchMoved_deleting);
RW_FUNCTION(0x4FE0D8, "MiniMedalScrollView::registItemTouchEvent::$_1 __func::__clone()", TouchMoved_clone);
RW_FUNCTION(0x4FE108, "MiniMedalScrollView::registItemTouchEvent::$_1 __func::__clone(__base*)", TouchMoved_cloneInto);
RW_FUNCTION(0x4FE130, "MiniMedalScrollView::registItemTouchEvent::$_1 __func::destroy (nullsub_295)", TouchMoved_destroy);
RW_FUNCTION(0x4FE132, "MiniMedalScrollView::registItemTouchEvent::$_1 __func::destroy_deallocate", TouchMoved_destroyDeallocate);
RW_FUNCTION(0x4FE868, "MiniMedalScrollView::registItemTouchEvent::$_1 __func::target", TouchMoved_target);
RW_FUNCTION(0x4FE888, "MiniMedalScrollView::registItemTouchEvent::$_1 __func::target_type", TouchMoved_targetType);
RW_FUNCTION(0x4FE89C, "MiniMedalScrollView::registItemTouchEvent::$_0 __func::~__func (deleting)", TouchBegan_deleting);
RW_FUNCTION(0x4FE8A4, "MiniMedalScrollView::registItemTouchEvent::$_0 __func::__clone()", TouchBegan_clone);
RW_FUNCTION(0x4FE8D4, "MiniMedalScrollView::registItemTouchEvent::$_0 __func::__clone(__base*)", TouchBegan_cloneInto);
RW_FUNCTION(0x4FE8F8, "MiniMedalScrollView::registItemTouchEvent::$_0 __func::destroy (nullsub_296)", TouchBegan_destroy);
RW_FUNCTION(0x4FE8FA, "MiniMedalScrollView::registItemTouchEvent::$_0 __func::destroy_deallocate", TouchBegan_destroyDeallocate);
RW_FUNCTION(0x4FEB04, "MiniMedalScrollView::registItemTouchEvent::$_0 __func::target", TouchBegan_target);
RW_FUNCTION(0x4FEB24, "MiniMedalScrollView::registItemTouchEvent::$_0 __func::target_type", TouchBegan_targetType);
RW_FUNCTION(0x664B10, "MiniMedalScrollView throw stub (sub_664B10)", throwStubA);
RW_FUNCTION(0x664B14, "MiniMedalScrollView throw stub (sub_664B14)", throwStubB);

// ------------------------------------------------------------------------------------------ lambdas
void MiniMedalScrollView::assignTouchEnded(Addr function, Addr lambda) {
    Frame f;
    uint32_t tmp = f.alloc(20);
    fst32(tmp + 16, tmp);
    fst32(tmp, VT_TouchEnded);
    fst32(tmp + 8, ld32(lambda + 4));
    fst32(tmp + 4, ld32(lambda));
    call(FunctionTouchVoid_swap, {tmp, function});
    functionDestroy(tmp);
}
RW_FUNCTION(0x4FD380, "std::function<void(Touch*,Event*)>::operator=(MiniMedalScrollView::registItemTouchEvent::$_2)",
            MiniMedalScrollView::assignTouchEnded);

// A touch on a visible, idle list counts when it is inside the scroll area and the cell; it remembers the item
// and where the touch began.
bool MiniMedalScrollView::onTouchBegan(Addr functor, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    MiniMedalScrollView *self = scrollOf(functor);
    uint32_t s = rw::guest_addr(self);
    if (vcall(s, VSLOT_isVisible, {s}) != 1) return false;
    if (self->dragging) return false;
    if (!self->enabledTapItem) return false;
    Frame f;
    {
        uint32_t loc = f.alloc(8);
        call(Touch_getLocation, {loc, touch});
        uint32_t inside = call(IScrollProtocol_containsPointInScrollArea, {s, loc});
        call(Vec2_dtor, {loc});
        if (!inside) return false;
    }
    int32_t index = int32_t(ld32(functor + 8));
    if (SceneUtil::getNexTutorialphase() <= 0x3E2 && index > 1) return false;
    uint32_t row = ld32(self->rowsBegin + 4 * uint32_t(index));
    float width = ldf(vcall(row, node::getContentSize, {row}));
    float height = ldf(vcall(row, node::getContentSize, {row}) + 4);
    uint32_t rect = f.alloc(16);
    call(Rect_ctor4, {rect, 0, 0, rw::fbits(width), rw::fbits(height)});
    uint32_t loc = f.alloc(8), local = f.alloc(8);
    call(Touch_getLocation, {loc, touch});
    call(Node_convertToNodeSpace, {local, row, loc});
    call(Vec2_dtor, {loc});
    bool began = false;
    if (call(Rect_containsPoint, {rect, local}) == 1) {
        int32_t number = itemNumber(self, index);
        if (number >= 1 && uint32_t(number - 1) < itemCount(self)) {
            self->touchIndex = number - 1;
            uint32_t start = f.alloc(8);
            call(Touch_getLocation, {start, touch});
            self->touchStartX = ldf(start);
            self->touchStartY = ldf(start + 4);
            call(Vec2_dtor, {start});
            began = true;
            self->touching = 1;
            self->field_688 = 0;
        }
    }
    call(Vec2_dtor, {local});
    call(Vec2_dtor, {rect});
    return began;
}
RW_FUNCTION(0x4FE904, "MiniMedalScrollView::registItemTouchEvent::$_0::operator()", MiniMedalScrollView::onTouchBegan);

// Drag the lifted copy along; while not dragging yet, a sideways move beyond 30 (design) pixels lifts a pickable
// medal out of the list, a vertical move beyond 20 pixels cancels the tap (the list scrolls instead).
void MiniMedalScrollView::onTouchMoved(Addr functor, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    MiniMedalScrollView *self = scrollOf(functor);
    uint32_t s = rw::guest_addr(self);
    Frame fr;
    const uint32_t sp = fr.allocExact(0x180);
    const uint32_t detail = sp + 0x94, medal = sp + 0x9C, dragPos = sp + 0xB0, world = sp + 0xBC;
    const uint32_t local = sp + 0xC4, l4 = sp + 0xE0, l3 = sp + 0xE8, l2 = sp + 0xF0, l1 = sp + 0xF8;
    const uint32_t pos = sp + 0x100, l0 = sp + 0x108, nodePos = sp + 0x110, record = sp + 0x118;

    call(Touch_getLocation, {l0, touch});
    call(Node_convertToNodeSpace, {nodePos, s, l0});
    call(Vec2_dtor, {l0});
    uint32_t panel = self->dragPanel;
    if (panel) {
        float startX = self->dragStartX;
        uint32_t setPosition = ld32(ld32(panel) + node::setPosition);
        call(Touch_getLocation, {l1, touch});
        float touchX = self->touchStartX, startY = self->dragStartY;
        call(Touch_getLocation, {l2, touch});
        float x = (startX + ldf(l1)) - touchX;
        float y = (startY + ldf(l2 + 4)) - self->touchStartY;
        call(Vec2_ctorXY, {pos, rw::fbits(x), rw::fbits(y)});
        call(setPosition, {panel, pos});
        call(Vec2_dtor, {pos});
        call(Vec2_dtor, {l2});
        call(Vec2_dtor, {l1});
    }
    if (!self->touching || self->dragging) {
        call(Vec2_dtor, {nodePos});
        return;
    }
    call(Touch_getLocation, {l3, touch});
    float startX = self->touchStartX;
    float x3 = ldf(l3);
    call(Vec2_dtor, {l3});
    float dx = x3 - startX;
    call(Touch_getLocation, {l4, touch});
    float startY = self->touchStartY;
    float y4 = ldf(l4 + 4);
    call(Vec2_dtor, {l4});
    float dy = y4 - startY;
    int32_t number = itemNumber(self, int32_t(ld32(functor + 8)));
    // a medal that is disabled, greyed out, in use or without effect cannot be lifted
    bool blocked = false;
    if (number >= 1 && uint32_t(number - 1) < itemCount(self)) {
        uint32_t at = self->itemIds.begin + 8 * uint32_t(number - 1);
        uint32_t lo = ld32(at), hi = ld32(at + 4);
        blocked = vecFind(self->disabledIds, lo, hi) != self->disabledIds.end ||
                  vecFind(self->disableMedalIds, lo, hi) != self->disableMedalIds.end ||
                  vecFind(self->usedIds, lo, hi) != self->usedIds.end ||
                  vecFind(self->noEffectIds, lo, hi) != self->noEffectIds.end;
    }
    float scale = visibleSize().width / 960.0f;
    float limitX = scale * 30.0f;
    float adx = bitsf(rw::fbits(dx) & 0x7FFFFFFFu);
    bool shortX = (limitX > adx) || limitX != limitX || adx != adx;
    if (blocked || shortX) {
        float limitY = scale * 20.0f;
        float ady = bitsf(rw::fbits(dy) & 0x7FFFFFFFu);
        bool shortY = (limitY > ady) || limitY != limitY || ady != ady;
        if (!shortY) self->touching = 0;
        call(Vec2_dtor, {nodePos});
        return;
    }

    // lift the medal: a copy of the cell follows the finger on the popup layer
    call(IScrollProtocol_setEnabledScroll, {s, 0});
    if (self->dragPanel) {
        uint32_t view = findNode(self->dragPanel, STR_medalView, 9);
        DeckMedalViewManager::removeDeckMedalView(DeckMedalViewManager::getInstance(), view);
        uint32_t layer = popupLayer();
        vcall(layer, node::removeChild, {layer, self->dragPanel, 1});
        self->dragPanel = 0;
    }
    if (number >= 1 && uint32_t(number - 1) < itemCount(self)) {
        self->touchIndex = number - 1;
        uint32_t widget = ld32(functor + 12);
        self->hiddenItem = widget;
        uint32_t layer = popupLayer();
        uint32_t parent = vcall(widget, node::getParent, {widget});
        uint32_t position = vcall(widget, node::getPosition, {widget});
        call(Node_convertToWorldSpace, {world, parent, position});
        call(Node_convertToNodeSpace, {local, layer, world});
        call(Vec2_dtor, {world});
        float px = ldf(local) + -32.0f;
        float py = ldf(local + 4) + -14.0f;
        call(Vec2_ctorXY, {dragPos, rw::fbits(px), rw::fbits(py)});
        self->dragStartX = ldf(dragPos);
        self->dragStartY = ldf(dragPos + 4);
        call(Vec2_dtor, {dragPos});
        uint32_t at = self->itemIds.begin + 8 * uint32_t(self->touchIndex);
        uint32_t lo = ld32(at), hi = ld32(at + 4);
        uint32_t copy = getLayoutPanel(self, 0, 0);
        self->dragPanel = copy;
        uint32_t view = findNode(copy, STR_medalView, 9);
        uint32_t mgr = call(MedalDataManager_getInstance, {});
        fst32(sp, 0);
        for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4)
            fst32(sp + 8 + k, k == 0x2C ? ld32(record + 0x2C) : 0);
        rw::call_exact(MedalDataManager_getMedalData, {medal, mgr, lo, hi});
        fst32(detail, 0);
        fst32(detail + 4, 0);
        fst32(sp + 0x18, detail);
        fst32(sp + 0x14, 0);
        fst32(sp + 0x10, 0);
        fst32(sp + 0x0C, 0);
        fst32(sp + 0x08, 10000);
        fst32(sp + 0x04, 10000);
        fst32(sp + 0x00, 1);
        rw::call_exact(DeckMedalView_setData, {view, medal, 0xFFFFFFFFu, 4});
        releaseIf(ld32(detail + 4));
        releaseIf(ld32(medal + 4));
        uint32_t p = self->dragPanel;
        vcall(p, node::setOpacity, {p, 200});
        p = self->dragPanel;
        vcall(p, node::setPosition, {p, s + 0x694});
        p = self->dragPanel;
        vcall(p, node::setScale, {p, rw::fbits(1.2f)});
        vcall(layer, node::addChild, {layer, self->dragPanel});
        uint32_t idAt = self->itemIds.begin + 8 * uint32_t(self->touchIndex);
        vecPushBack(self->usedIds, ld32(idAt), ld32(idAt + 4), idAt);
        call(IScrollProtocol_updateLayout, {s, 0});
        call(Vec2_dtor, {local});
    }
    self->dragging = 1;
    call(Vec2_dtor, {nodePos});
}
RW_FUNCTION(0x4FE13C, "MiniMedalScrollView::registItemTouchEvent::$_1::operator()", MiniMedalScrollView::onTouchMoved);

// Release: a tap calls onTapItem(id, item); a drag asks getDropTarget where the medal lands (-1: nowhere) and
// flies the copy there (onDropItem) or back into the list, then onDropFinished.
void MiniMedalScrollView::onTouchEnded(Addr functor, Addr touchRef, Addr) {
    uint32_t touch = ld32(touchRef);
    MiniMedalScrollView *self = scrollOf(functor);
    uint32_t s = rw::guest_addr(self);
    Frame fr;
    const uint32_t sp = fr.allocExact(0x78);
    const uint32_t popupPos = sp + 0x20, converted = sp + 0x28, loc = sp + 0x30, target = sp + 0x38;
    const uint32_t done = sp + 0x40;

    if (self->touching) {
        self->touching = 0;
        if (!self->dragging) {
            int32_t number = itemNumber(self, int32_t(ld32(functor + 8)));
            if (number >= 1 && uint32_t(number - 1) < itemCount(self) && rw::get<uint32_t>(self, 0x648)) {
                uint32_t at = self->itemIds.begin + 8 * uint32_t(number - 1);
                rw::call_exact(FunctionTapItem_call, {s + 0x638, at + 8, ld32(at), ld32(at + 4), uint32_t(number - 1)});
            }
        }
    }
    if (!self->dragging) return;
    if (self->dropAnimating) return;
    call(fn::Vec2_ctor_copy, {target, s + 0x694});
    uint32_t point = call(NewArray, {8});
    bool moved = false;
    uint32_t slot = 0xFFFFFFFFu;
    if (rw::get<uint32_t>(self, 0x678)) {
        if (!(itemCount(self) > uint32_t(self->touchIndex))) {
            call(ThrowOutOfRange, {s + OFF_ITEM_IDS});
            return;
        }
        uint32_t at = self->itemIds.begin + 8 * uint32_t(self->touchIndex);
        uint32_t lo = ld32(at), hi = ld32(at + 4);
        call(Touch_getLocation, {loc, touch});
        uint32_t r = uint32_t(rw::call_exact(FunctionDropTarget_call, {s + 0x668, loc, lo, hi, loc, point}));
        call(Vec2_dtor, {loc});
        if (r != 0xFFFFFFFFu) {
            uint32_t layer = popupLayer();
            call(Vec2_ctorXY, {popupPos, ld32(point), ld32(point + 4)});
            call(Node_convertToNodeSpace, {converted, layer, popupPos});
            fst32(target, ld32(converted));
            fst32(target + 4, ld32(converted + 4));
            call(Vec2_dtor, {converted});
            call(Vec2_dtor, {popupPos});
            fstf(target, ldf(target) + -32.0f);
            fstf(target + 4, ldf(target + 4) + -14.0f);
            moved = true;
            slot = r;
        }
    }
    call(DeleteArray, {point});
    self->dropAnimating = 1;
    uint32_t action = call(fn::MoveTo_create, {rw::fbits(0.1f), target});
    if (moved) {
        if (rw::get<uint32_t>(self, 0x660)) {
            if (!(itemCount(self) > uint32_t(self->touchIndex))) {
                call(ThrowOutOfRange, {s + OFF_ITEM_IDS});
                return;
            }
            uint32_t at = self->itemIds.begin + 8 * uint32_t(self->touchIndex);
            call(FunctionDropItem_call, {s + 0x650, slot, ld32(at), ld32(at + 4)});
        }
    } else {
        action = call(fn::Spawn_create, {action, call(fn::ScaleTo_create, {rw::fbits(0.1f), rw::fbits(1.0f)}), 0});
    }
    uint32_t panel = self->dragPanel;
    fst32(done + 16, done);
    fst32(done, VT_DropFinished);
    fst32(done + 8, moved ? 1 : 0);
    fst32(done + 4, s);
    uint32_t callFunc = call(fn::CallFunc_create, {done});
    call(fn::Node_runAction, {panel, call(fn::Sequence_create, {action, callFunc, 0})});
    functionDestroy(done);
    call(Vec2_dtor, {target});
}
RW_FUNCTION(0x4FD7B8, "MiniMedalScrollView::registItemTouchEvent::$_2::operator()", MiniMedalScrollView::onTouchEnded);

// The copy arrived: fade it out, give the medal back to the list (or, in the fusion picker, keep it greyed out
// when it was dropped on a slot), show the original cell again and re-enable scrolling.
void MiniMedalScrollView::onDropFinished(Addr functor) {
    MiniMedalScrollView *self = scrollOf(functor);
    uint32_t s = rw::guest_addr(self);
    uint32_t panel = self->dragPanel;
    uint32_t fade = call(FadeOut_create, {0x3D83126Fu});    // 0.064 s
    {
        Frame f;
        uint32_t done = f.alloc(20);
        fst32(done + 16, done);
        fst32(done, VT_PanelFaded);
        fst32(done + 4, s);
        uint32_t callFunc = call(fn::CallFunc_create, {done});
        call(fn::Node_runAction, {panel, call(fn::Sequence_create, {fade, callFunc, 0})});
        functionDestroy(done);
    }
    uint32_t at = self->itemIds.begin + 8 * uint32_t(self->touchIndex);
    uint32_t found = vecFind(self->usedIds, ld32(at), ld32(at + 4));
    if (found != self->usedIds.end) vecErase(self->usedIds, found);
    if (self->mixMode && ld8(functor + 8)) {
        uint32_t idAt = self->itemIds.begin + 8 * uint32_t(self->touchIndex);
        vecPushBack(self->disableMedalIds, ld32(idAt), ld32(idAt + 4), idAt);
    }
    uint32_t hidden = self->hiddenItem;
    vcall(hidden, node::setVisible, {hidden, 1});
    self->hiddenItem = 0;
    self->dragging = 0;
    self->dropAnimating = 0;
    call(IScrollProtocol_setEnabledScroll, {s, scrollAllowed()});
}
RW_FUNCTION(0x4FDD1C, "MiniMedalScrollView::registItemTouchEvent::$_2::operator()::{lambda()#1}::operator()",
            MiniMedalScrollView::onDropFinished);

// The copy faded out: remove it from the popup layer and the view manager, redraw the list.
void MiniMedalScrollView::onDragPanelFaded(Addr functor) {
    MiniMedalScrollView *self = scrollOf(functor);
    uint32_t layer = popupLayer();
    vcall(layer, node::removeChild, {layer, self->dragPanel, 1});
    uint32_t view = findNode(self->dragPanel, STR_medalView, 9);
    DeckMedalViewManager::removeDeckMedalView(DeckMedalViewManager::getInstance(), view);
    self->dragPanel = 0;
    call(IScrollProtocol_updateLayout, {rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4FDFBC, "MiniMedalScrollView::registItemTouchEvent::$_2::operator()::{lambda()#1}::operator()::{lambda()#1}::operator()",
            MiniMedalScrollView::onDragPanelFaded);
