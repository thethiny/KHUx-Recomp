// BaseScrollView<int> / BaseScrollView<unsigned long long>: IScrollProtocol with an item id list.
#include "BaseScrollView.hpp"

using namespace scene;

namespace {
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;               // cocos2d::ui::Layout::Layout()
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;                    // __aeabi_memset(dst, n, c)
constexpr uint32_t VectorInt_assign = 0x47D5BD;          // std::vector<int>::assign(int*, int*)
constexpr uint32_t VectorUll_assign = 0x4D7461;          // std::vector<unsigned long long>::assign(ull*, ull*)
constexpr uint32_t VectorBaseInt_dtor = 0x478961;        // std::__vector_base<int>::~__vector_base
constexpr uint32_t VectorBaseUll_dtor = 0x468499;        // std::__vector_base<unsigned long long>::~__vector_base

// vector.clear() as libc++ inlines it for elements of `size` bytes
void clearIds(guest::Vector &v, uint32_t size) {
    uint32_t e = v.end, b = v.begin;
    if (e != b) v.end = e + (~(size - 1) & ~(e - size - b));
}
}  // namespace

// The inlined IScrollProtocol constructor: the scroll defaults (0.1 deceleration, 0.3 bounce damping, 200 px
// bounce range, one column, scrolling on), everything else zero.
void BaseScrollView::initScrollDefaults(BaseScrollView *self) {
    uint32_t me = self->self();
    self->deceleration = 0.1f;
    self->bounceDeceleration = 0.3f;
    self->itemCount = 0;
    self->field_508 = 0;
    self->layoutPanel = 0;
    self->bouncePixel = 200.0f;
    call(AEABI_MEMSET, {me + 0x514, 0x24, 0});
    self->maxColumn = 1;
    self->scrollEnabled = 1;
    self->scrollType = 0;
    self->scrollBarEnabled = 0;
    self->targetPosition = 0.0f;
    rw::set<uint32_t>(self, OFF_ON_INDEX_CHANGED + 16, 0);
    rw::set<uint32_t>(self, OFF_ON_SCROLL_END + 16, 0);
    rw::set<uint32_t>(self, OFF_ON_DRAG + 16, 0);
    rw::set<uint32_t>(self, OFF_ON_TOUCH_ENDED + 16, 0);
    call(AEABI_MEMSET, {me + 0x540, 0x12, 0});
    call(AEABI_MEMSET, {me + 0x554, 0x12, 0});
    call(AEABI_MEMSET, {me + 0x568, 0x25, 0});
    call(AEABI_MEMSET, {me + 0x598, 0x2A, 0});
}

BaseScrollView *BaseScrollView::constructInt(BaseScrollView *self) {
    call(Layout_ctor, {self->self()});
    initScrollDefaults(self);
    self->vtable = VTABLE_INT;
    self->vtable2 = VTABLE2_INT;
    self->vtable3 = VTABLE3_INT;
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    return self;
}
RW_FUNCTION(0x55DBF4, "BaseScrollView<int>::BaseScrollView()", BaseScrollView::constructInt);

uint32_t BaseScrollView::initInt(BaseScrollView *self) {
    clearIds(self->itemIDs, 4);
    return IScrollProtocol::init(self);
}
RW_FUNCTION(0x4FA8A0, "BaseScrollView<int>::init()", BaseScrollView::initInt);

uint32_t BaseScrollView::initUll(BaseScrollView *self) {
    clearIds(self->itemIDs, 8);
    return IScrollProtocol::init(self);
}
RW_FUNCTION(0x4FC968, "BaseScrollView<unsigned long long>::init()", BaseScrollView::initUll);

void BaseScrollView::setItemIDsInt(BaseScrollView *self, Addr ids) {
    uint32_t mine = rw::guest_addr(&self->itemIDs);
    if (mine != ids) call(VectorInt_assign, {mine, ld32(ids), ld32(ids + 4)});
    IScrollProtocol::setItemIDSize(self, int32_t(uint32_t(self->itemIDs.end) - uint32_t(self->itemIDs.begin)) >> 2);
}
RW_FUNCTION(0x478928, "BaseScrollView<int>::setItemIDs(const vector<int>&)", BaseScrollView::setItemIDsInt);

void BaseScrollView::setItemIDsUll(BaseScrollView *self, Addr ids) {
    uint32_t mine = rw::guest_addr(&self->itemIDs);
    if (mine != ids) call(VectorUll_assign, {mine, ld32(ids), ld32(ids + 4)});
    IScrollProtocol::setItemIDSize(self, int32_t(uint32_t(self->itemIDs.end) - uint32_t(self->itemIDs.begin)) >> 3);
}
RW_FUNCTION(0x4FD230, "BaseScrollView<unsigned long long>::setItemIDs(const vector<ull>&)",
            BaseScrollView::setItemIDsUll);

BaseScrollView *BaseScrollView::destroyInt(BaseScrollView *self) {
    self->vtable = VTABLE_INT;
    self->vtable2 = VTABLE2_INT;
    self->vtable3 = VTABLE3_INT;
    call(VectorBaseInt_dtor, {rw::guest_addr(&self->itemIDs)});
    IScrollProtocol::destroy(self);
    return self;
}
RW_FUNCTION(0x4FC390, "BaseScrollView<int>::~BaseScrollView()", BaseScrollView::destroyInt);

void BaseScrollView::destroyDeleteInt(BaseScrollView *self) {
    destroyInt(self);
    call(fn::operator_delete, {self->self()});
}
RW_FUNCTION(0x4FC3E0, "BaseScrollView<int>::~BaseScrollView() (deleting)", BaseScrollView::destroyDeleteInt);

BaseScrollView *BaseScrollView::destroyUll(BaseScrollView *self) {
    self->vtable = VTABLE_ULL;
    self->vtable2 = VTABLE2_ULL;
    self->vtable3 = VTABLE3_ULL;
    call(VectorBaseUll_dtor, {rw::guest_addr(&self->itemIDs)});
    IScrollProtocol::destroy(self);
    return self;
}
RW_FUNCTION(0x4FEB38, "BaseScrollView<unsigned long long>::~BaseScrollView()", BaseScrollView::destroyUll);

void BaseScrollView::destroyDeleteUll(BaseScrollView *self) {
    destroyUll(self);
    call(fn::operator_delete, {self->self()});
}
RW_FUNCTION(0x4FEB88, "BaseScrollView<unsigned long long>::~BaseScrollView() (deleting)",
            BaseScrollView::destroyDeleteUll);
