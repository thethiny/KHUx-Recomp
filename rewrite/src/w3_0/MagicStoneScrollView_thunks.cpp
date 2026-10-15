// MagicStoneScrollView: the std::function __func thunks of its lambdas (destructors, clone - copy the captures into a new /
// given block -, destroy, target / target_type typeinfo checks), generated from the lifted code by a scratch
// tool (gen_thunks.py, symbolic walk of each thunk) and checked against it. The lambda bodies are written by hand.
#include "../scene/SceneCommon.hpp"

namespace {
using namespace scene;
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }

// MagicStoneScrollView::registItemTouchEvent $_3: __func::~__func (deleting)
static void registItemTouchEvent_L3_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4FBBAA, "MagicStoneScrollView::registItemTouchEvent $_3 __func::~__func (deleting)", registItemTouchEvent_L3_deletingDtor);

// MagicStoneScrollView::registItemTouchEvent $_3: __func::__clone()
static uint32_t registItemTouchEvent_L3_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf55b54u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 16, v5);
        st32(v1 + 12, v4);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x4FBBB4, "MagicStoneScrollView::registItemTouchEvent $_3 __func::__clone()", registItemTouchEvent_L3_clone);

// MagicStoneScrollView::registItemTouchEvent $_3: __func::__clone(__base*)
static void registItemTouchEvent_L3_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55b54u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x4FBBE8, "MagicStoneScrollView::registItemTouchEvent $_3 __func::__clone(__base*)", registItemTouchEvent_L3_cloneInto);

// MagicStoneScrollView::registItemTouchEvent $_3: __func::destroy
static void registItemTouchEvent_L3_destroy(uint32_t self) {
}
RW_FUNCTION(0x4FBC14, "MagicStoneScrollView::registItemTouchEvent $_3 __func::destroy", registItemTouchEvent_L3_destroy);

// MagicStoneScrollView::registItemTouchEvent $_3: __func::destroy_deallocate
static void registItemTouchEvent_L3_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4FBC16, "MagicStoneScrollView::registItemTouchEvent $_3 __func::destroy_deallocate", registItemTouchEvent_L3_destroyDeallocate);

// MagicStoneScrollView::registItemTouchEvent $_3: __func::target
static uint32_t registItemTouchEvent_L3_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1fc20u ? self + 4 : 0; }
RW_FUNCTION(0x4FBC2C, "MagicStoneScrollView::registItemTouchEvent $_3 __func::target", registItemTouchEvent_L3_target);

// MagicStoneScrollView::registItemTouchEvent $_3: __func::target_type
static uint32_t registItemTouchEvent_L3_targetType() { return 0xf8116cu; }
RW_FUNCTION(0x4FBC4C, "MagicStoneScrollView::registItemTouchEvent $_3 __func::target_type", registItemTouchEvent_L3_targetType);

// MagicStoneScrollView::registItemTouchEvent $_4: __func::~__func (deleting)
static void registItemTouchEvent_L4_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4FBE34, "MagicStoneScrollView::registItemTouchEvent $_4 __func::~__func (deleting)", registItemTouchEvent_L4_deletingDtor);

// MagicStoneScrollView::registItemTouchEvent $_4: __func::__clone()
static uint32_t registItemTouchEvent_L4_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf55b80u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x4FBE3C, "MagicStoneScrollView::registItemTouchEvent $_4 __func::__clone()", registItemTouchEvent_L4_clone);

}  // namespace
