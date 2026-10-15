// DeckMedalView: creation, data, the blinking plates, the type-match effect and updateLayout's LWF callbacks.
// DeckMedalView::updateLayout itself (0x5276D8, 6976 bytes, ~5900 lifted lines with its outlined chunks) is not
// rewritten.
#include "DeckMedalView.hpp"

#include "DeckMedalViewManager.hpp"

using namespace scene;

namespace {
constexpr uint32_t GOT_VT_DeckMedalView = 0xFD65FC;       // -> _ZTV13DeckMedalView (0xF86314)
constexpr uint32_t VT2_OFFSET = 0x32C;                    // vtable of the +0x280 base

constexpr uint32_t Memset = 0x40E7A0;                     // __aeabi_memset(dst, n, c)
constexpr uint32_t Widget_ctor = 0x8253D1;
constexpr uint32_t Widget_dtor = 0x825601;
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t LWFNode_create = 0x7EDF31;             // (const char* path, void*)
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t DeckMedalView_updateLayout = 0x5276D9;

constexpr uint32_t APIManager_userDetail = 0x94;         // shared_ptr<UserDetail>: the player

constexpr uint32_t VSLOT_setTouchEnabled = 0x29C;         // ui::Widget

constexpr uint32_t STR_Area_MedalInfo = 0xDFDD54;
constexpr uint32_t STR_Dummy_Mag2 = 0xDFDD63;
constexpr uint32_t STR_Dummy_Guilt_1 = 0xDFDD6E;
constexpr uint32_t STR_Area_MixCircle = 0xDF8B8A;
constexpr uint32_t STR_Area_OpenInfo = 0xDFDD7C;
constexpr uint32_t STR_TypeMatchEffect = 0xDFE0FE;
constexpr uint32_t STR_TYpe = 0xDFDDA7;                   // "TYpe" (sic)
constexpr uint32_t STR_TypeMatchLwf = 0xDFE10E;           // "lwf/medal/medaldeck/MedalDeck_Type%d/MedalDeck_Type%d.lwf"

// updateLayout's lambdas: __func vtables (+8), type names, typeinfos
constexpr uint32_t VT_TypeMatchMovie = 0xF57388 + 8;      // $_0 void(LWF::Movie*): [node, parent, view, type]
constexpr uint32_t VT_TypeMatchFaded = 0xF573B4 + 8;      // $_0::operator()::{lambda()#1}: [parent, node, view, type]
constexpr uint32_t NAME_TypeMatchMovie = 0xE28470, TI_TypeMatchMovie = 0xF862E8;
constexpr uint32_t NAME_TypeMatchFaded = 0xE28520, TI_TypeMatchFaded = 0xF862FC;

inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }
inline void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
inline void setOpacity(uint32_t n, uint32_t v) { vcall(n, node::setOpacity, {n, v}); }

// std::function __func thunks of the 20-byte lambdas (vtable + four captured words)
uint32_t cloneFunc(uint32_t vt, uint32_t self) {
    uint32_t p = call(fn::operator_new, {0x14});
    if (p) {
        st32(p, vt);
        for (uint32_t k = 0x10; k >= 4; k -= 4) st32(p + k, ld32(self + k));
    }
    return p;
}
void cloneInto(uint32_t vt, uint32_t self, uint32_t dst) {
    if (!dst) return;
    st32(dst, vt);
    for (uint32_t k = 0x10; k >= 4; k -= 4) st32(dst + k, ld32(self + k));
}
uint32_t target(uint32_t name, uint32_t self, uint32_t typeInfo) { return ld32(typeInfo + 4) == name ? self + 4 : 0; }

void TypeMatchMovie_deleting(uint32_t self) { call(fn::operator_delete, {self}); }
uint32_t TypeMatchMovie_clone(uint32_t self) { return cloneFunc(VT_TypeMatchMovie, self); }
void TypeMatchMovie_cloneInto(uint32_t self, uint32_t dst) { cloneInto(VT_TypeMatchMovie, self, dst); }
void TypeMatchMovie_destroy(uint32_t) {}
void TypeMatchMovie_destroyDeallocate(uint32_t self) { call(fn::operator_delete, {self}); }
uint32_t TypeMatchMovie_target(uint32_t self, uint32_t ti) { return target(NAME_TypeMatchMovie, self, ti); }
uint32_t TypeMatchMovie_targetType() { return TI_TypeMatchMovie; }
void TypeMatchFaded_deleting(uint32_t self) { call(fn::operator_delete, {self}); }
uint32_t TypeMatchFaded_clone(uint32_t self) { return cloneFunc(VT_TypeMatchFaded, self); }
void TypeMatchFaded_cloneInto(uint32_t self, uint32_t dst) { cloneInto(VT_TypeMatchFaded, self, dst); }
void TypeMatchFaded_destroy(uint32_t) {}
void TypeMatchFaded_destroyDeallocate(uint32_t self) { call(fn::operator_delete, {self}); }
uint32_t TypeMatchFaded_target(uint32_t self, uint32_t ti) { return target(NAME_TypeMatchFaded, self, ti); }
uint32_t TypeMatchFaded_targetType() { return TI_TypeMatchFaded; }
}  // namespace

RW_FUNCTION(0x530E40, "DeckMedalView::updateLayout::$_0 __func::~__func (deleting)", TypeMatchMovie_deleting);
RW_FUNCTION(0x530E48, "DeckMedalView::updateLayout::$_0 __func::__clone()", TypeMatchMovie_clone);
RW_FUNCTION(0x530E7C, "DeckMedalView::updateLayout::$_0 __func::__clone(__base*)", TypeMatchMovie_cloneInto);
RW_FUNCTION(0x530EA8, "DeckMedalView::updateLayout::$_0 __func::destroy (nullsub_418)", TypeMatchMovie_destroy);
RW_FUNCTION(0x530EAA, "DeckMedalView::updateLayout::$_0 __func::destroy_deallocate", TypeMatchMovie_destroyDeallocate);
RW_FUNCTION(0x530F94, "DeckMedalView::updateLayout::$_0 __func::target", TypeMatchMovie_target);
RW_FUNCTION(0x530FB4, "DeckMedalView::updateLayout::$_0 __func::target_type", TypeMatchMovie_targetType);
RW_FUNCTION(0x530FC8, "DeckMedalView::updateLayout::$_0::{lambda()#1} __func::~__func (deleting)", TypeMatchFaded_deleting);
RW_FUNCTION(0x530FD0, "DeckMedalView::updateLayout::$_0::{lambda()#1} __func::__clone()", TypeMatchFaded_clone);
RW_FUNCTION(0x531004, "DeckMedalView::updateLayout::$_0::{lambda()#1} __func::__clone(__base*)", TypeMatchFaded_cloneInto);
RW_FUNCTION(0x531030, "DeckMedalView::updateLayout::$_0::{lambda()#1} __func::destroy (nullsub_419)", TypeMatchFaded_destroy);
RW_FUNCTION(0x531032, "DeckMedalView::updateLayout::$_0::{lambda()#1} __func::destroy_deallocate", TypeMatchFaded_destroyDeallocate);
RW_FUNCTION(0x531068, "DeckMedalView::updateLayout::$_0::{lambda()#1} __func::target", TypeMatchFaded_target);
RW_FUNCTION(0x531088, "DeckMedalView::updateLayout::$_0::{lambda()#1} __func::target_type", TypeMatchFaded_targetType);

// ------------------------------------------------------------------------------------------ lifetime
DeckMedalView::Addr DeckMedalView::create(uint32_t smallLayout) {
    uint32_t obj = call(fn::operator_new, {SIZE});
    call(Memset, {obj, SIZE, 0});
    call(Widget_ctor, {obj});
    DeckMedalView *self = rw::host_ptr<DeckMedalView>(obj);
    uint32_t vt = ld32(GOT_VT_DeckMedalView);
    self->vtable = vt + 8;
    self->vtable2 = vt + VT2_OFFSET;
    self->showType = 0;
    self->medal = 0;
    self->medalCtl = 0;
    self->field_35C = -1;
    self->field_360 = 0;
    self->field_364 = 4;
    self->field_368 = 3;
    self->field_36C = 10000;
    self->magnification = 10000;
    self->typeMatchPlaying = 0;
    self->field_374 = 0;
    call(Memset, {obj + 0x378, 0x15, 0});
    call(Memset, {obj + 0x390, 0x2A, 0});
    self->field_3BA = 1;
    init(self, smallLayout);
    call(Ref_autorelease, {obj});
    return obj;
}
RW_FUNCTION(0x52BA70, "DeckMedalView::create", DeckMedalView::create);

// Clone the medal layout (small or normal), put the medal info panel into its Area_MedalInfo and the "open info"
// panel on top, and take the layout's size.
uint32_t DeckMedalView::init(DeckMedalView *self, uint32_t smallLayout) {
    uint32_t s = rw::guest_addr(self);
    self->isSmall = uint8_t(smallLayout);
    DeckMedalViewManager *mgr = rw::host_ptr<DeckMedalViewManager>(DeckMedalViewManager::getInstance());
    uint32_t panel = smallLayout == 1 ? DeckMedalViewManager::getSmallMedalPanel(mgr)
                                      : DeckMedalViewManager::getNormalMedalPanel(mgr);
    uint32_t base = call(Widget_clone, {panel});
    vcall(base, VSLOT_setTouchEnabled, {base, 0});
    mgr = rw::host_ptr<DeckMedalViewManager>(DeckMedalViewManager::getInstance());
    uint32_t info = call(Widget_clone, {DeckMedalViewManager::getMedalInfoPanel(mgr)});
    self->medalInfoPanel = info;
    vcall(info, VSLOT_setTouchEnabled, {info, 0});
    uint32_t area = findNode(base, STR_Area_MedalInfo, 14);
    vcall(area, node::setLocalZOrder, {area, 2});
    vcall(area, node::addChild, {area, self->medalInfoPanel});
    self->darklightMag = findNode(base, STR_Dummy_Mag2, 10);
    uint32_t guilt = findNode(base, STR_Dummy_Guilt_1, 13);
    uint32_t mix = findNode(guilt, STR_Area_MixCircle, 14);
    rw::set<uint32_t>(self, 0x384, vcall(mix, node::getPositionX, {mix}));
    mgr = rw::host_ptr<DeckMedalViewManager>(DeckMedalViewManager::getInstance());
    uint32_t open = call(Widget_clone, {DeckMedalViewManager::getOpenInfoPanel(mgr)});
    vcall(open, VSLOT_setTouchEnabled, {open, 0});
    {
        Frame f;
        uint32_t name = newStr(f, STR_Area_OpenInfo, 13);
        vcall(base, node::addChildZName, {base, open, 2, name});
        strFree(name);
    }
    vcall(s, node::setContentSize, {s, vcall(base, node::getContentSize, {base})});
    vcall(s, node::addChild, {s, base});
    return 1;
}
RW_FUNCTION(0x52BB70, "DeckMedalView::init", DeckMedalView::init);

// ------------------------------------------------------------------------------------------ plates
// The blink step pushed by DeckMedalViewManager::update: statusOpacity / mixOpacity go to the status and fusion
// plates (which ones exist depends on the layout and the medal); plates that do not blink are fully shown.
void DeckMedalView::updateStatusAndMixedPlate(DeckMedalView *self, uint32_t statusOpacity, uint32_t mixOpacity) {
    if (self->isSmall) {
        if (self->isShowMixInfo && self->plate3A8) {
            setOpacity(self->plate3A8, mixOpacity);
            setOpacity(self->plate3A0, statusOpacity);
            if (self->plate3B4) setOpacity(self->plate3B4, statusOpacity);
            return;
        }
        if (!self->plate3A0) return;
        setOpacity(self->plate3A0, 0xFF);
        if (self->plate3B4) setOpacity(self->plate3B4, 0xFF);
        return;
    }
    uint8_t has38A = self->field_38A;
    if (!self->field_374) {
        if (!has38A || !self->plate3A0) return;
        setOpacity(self->plate3A0, 0xFF);
        if (self->plate3A4) setOpacity(self->plate3A4, 0xFF);
        if (self->plate3B4) setOpacity(self->plate3B4, 0xFF);
        return;
    }
    if (!has38A || !self->plate3A0) {
        if (self->plate398) setOpacity(self->plate398, 0xFF);
        if (self->plate39C) setOpacity(self->plate39C, 0xFF);
        return;
    }
    // while the type-match effect plays, the status plates are hidden and the fusion plates fully shown
    bool playing = self->typeMatchPlaying != 0;
    uint32_t mix = playing ? 0xFF : mixOpacity;
    uint32_t status = playing ? 0 : statusOpacity;
    if (self->plate398) setOpacity(self->plate398, mix);
    if (self->plate39C) setOpacity(self->plate39C, mix);
    setOpacity(self->plate3A0, status);
    if (self->plate3A4) setOpacity(self->plate3A4, status);
    if (self->plate3B4) setOpacity(self->plate3B4, status);
}
RW_FUNCTION(0x52737C, "DeckMedalView::updateStatusAndMixedPlate", DeckMedalView::updateStatusAndMixedPlate);

// ------------------------------------------------------------------------------------------ data
// Keep the medal and its context and redraw; without an owner the player (APIManager's user detail) is used.
void DeckMedalView::setData(DeckMedalView *self, Addr medal, int32_t a1, int32_t a2, int32_t a3, int32_t a4,
                            int32_t a5, uint32_t relayout, int32_t a6, Addr skills, Addr detail) {
    uint32_t obj = ld32(medal), ctl = ld32(medal + 4);
    if (ctl) call(fn::shared_add, {ctl});
    self->medal = obj;
    uint32_t old = self->medalCtl;
    self->medalCtl = ctl;
    releaseIf(old);
    self->field_35C = a1;
    self->field_364 = a2;
    self->field_368 = a3;
    self->field_36C = a4;
    self->magnification = a5;
    self->field_360 = a6;
    self->skills = skills;
    uint32_t dObj = ld32(detail), dCtl = ld32(detail + 4);
    if (dCtl) call(fn::shared_add, {dCtl});
    self->detail = dObj;
    old = self->detailCtl;
    self->detailCtl = dCtl;
    releaseIf(old);
    if (self->detail == 0) {
        uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t pCtl = ld32(api + APIManager_userDetail + 4), pObj = ld32(api + APIManager_userDetail);
        if (pCtl) call(fn::shared_add, {pCtl});
        self->detail = pObj;
        old = self->detailCtl;
        self->detailCtl = pCtl;
        releaseIf(old);
    }
    call(DeckMedalView_updateLayout, {rw::guest_addr(self), relayout});
}
RW_FUNCTION(0x52BEE4, "DeckMedalView::setData", DeckMedalView::setData);

void DeckMedalView::setIsShowDarklightMagnification(DeckMedalView *self, uint32_t show) {
    uint32_t n = self->darklightMag;
    if (n && self->magnification >= 10001) vcall(n, node::setVisible, {n, show});
}
RW_FUNCTION(0x52BFDC, "DeckMedalView::setIsShowDarklightMagnification", DeckMedalView::setIsShowDarklightMagnification);

// Play the medal-type match LWF over the "TYpe" icon (once: skipped while one is attached).
void DeckMedalView::showTypeMatchEffect(DeckMedalView *self, int32_t type) {
    uint32_t s = rw::guest_addr(self);
    if (findNode(s, STR_TypeMatchEffect, 15)) return;
    uint32_t icon = findNode(s, STR_TYpe, 4);
    Frame f;
    uint32_t path = stringBuffer(f, {STR_TypeMatchLwf, uint32_t(type), uint32_t(type)});
    uint32_t lwf = call(LWFNode_create, {path, 0});
    uint32_t setPosition = ld32(ld32(lwf) + node::setPosition);
    float width = ldf(vcall(icon, node::getContentSize, {icon}));
    uint32_t size = vcall(icon, node::getContentSize, {icon});
    float x = width * 0.5f;
    float y = ldf(size + 4) * 0.5f;
    uint32_t pos = f.alloc(8);
    call(Vec2_ctorXY, {pos, rw::fbits(x), rw::fbits(y)});
    call(setPosition, {lwf, pos});
    call(Vec2_dtor, {pos});
    uint32_t name = newStr(f, STR_TypeMatchEffect, 15);
    vcall(icon, node::addChildZName, {icon, lwf, 0, name});
    strFree(name);
}
RW_FUNCTION(0x52C008, "DeckMedalView::showTypeMatchEffect", DeckMedalView::showTypeMatchEffect);

void DeckMedalView::changeShowType(DeckMedalView *self, int32_t type) { self->showType = type; }
RW_FUNCTION(0x52C1EC, "DeckMedalView::changeShowType", DeckMedalView::changeShowType);

// ------------------------------------------------------------------------------------------ destruction
DeckMedalView *DeckMedalView::destruct(DeckMedalView *self) {
    uint32_t vt = ld32(GOT_VT_DeckMedalView);
    self->vtable = vt + 8;
    self->vtable2 = vt + VT2_OFFSET;
    releaseIf(self->detailCtl);
    releaseIf(self->medalCtl);
    call(Widget_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x5310D4, "DeckMedalView::~DeckMedalView", DeckMedalView::destruct);

void DeckMedalView::destructDelete(DeckMedalView *self) {
    destruct(self);
    call(fn::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x53112C, "DeckMedalView::~DeckMedalView (deleting)", DeckMedalView::destructDelete);

uint32_t DeckMedalView::destructThunk(uint32_t base2) {
    DeckMedalView *self = rw::host_ptr<DeckMedalView>(base2 - 0x280);
    uint32_t vt = ld32(GOT_VT_DeckMedalView);
    self->vtable = vt + 8;
    self->vtable2 = vt + VT2_OFFSET;
    releaseIf(self->detailCtl);
    releaseIf(self->medalCtl);
    return call(Widget_dtor, {base2 - 0x280});
}
RW_FUNCTION(0x531188, "non-virtual thunk to DeckMedalView::~DeckMedalView (+640)", DeckMedalView::destructThunk);

void DeckMedalView::destructDeleteThunk(uint32_t base2) {
    destructThunk(base2);
    call(fn::operator_delete, {base2 - 0x280});
}
RW_FUNCTION(0x5311DC, "non-virtual thunk to DeckMedalView::~DeckMedalView (+640, deleting)",
            DeckMedalView::destructDeleteThunk);

// ------------------------------------------------------------------------------------------ updateLayout lambdas
// The type-match LWF reached its last frame: fade it out, then (onTypeMatchFaded) remove it and play again.
void DeckMedalView::onTypeMatchMovie(Addr functor, Addr movieRef) {
    uint32_t movie = ld32(movieRef);
    if (ld32(movie + 0x9C) != ld32(movie + 0x98)) return;
    uint32_t fading = ld32(functor + 4), parent = ld32(functor + 8);
    uint32_t view = ld32(functor + 12), type = ld32(functor + 16);
    Frame f;
    uint32_t done = f.alloc(20);
    fst32(done + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x14});
    if (heap) {
        st32(heap, VT_TypeMatchFaded);
        st32(heap + 4, parent);
        st32(heap + 8, fading);
        st32(heap + 12, view);
        st32(heap + 16, type);
    }
    fst32(done + 16, heap);
    call(fn::Node_runAction, {fading, SceneUtil::createFadeOutAnimation(fading, done)});
    functionDestroy(done);
}
RW_FUNCTION(0x530EB4, "DeckMedalView::updateLayout::$_0::operator()", DeckMedalView::onTypeMatchMovie);

void DeckMedalView::onTypeMatchFaded(Addr functor) {
    uint32_t parent = ld32(functor + 4), child = ld32(functor + 8);
    DeckMedalView *view = rw::host_ptr<DeckMedalView>(ld32(functor + 12));
    vcall(parent, node::removeChild, {parent, child, 1});
    view->typeMatchPlaying = 0;
    showTypeMatchEffect(view, int32_t(ld32(functor + 16)));
}
RW_FUNCTION(0x53103C, "DeckMedalView::updateLayout::$_0::operator()::{lambda()#1}::operator()",
            DeckMedalView::onTypeMatchFaded);
