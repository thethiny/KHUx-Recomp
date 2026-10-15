// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the MaterialScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// MaterialScrollView::registItemTouchEvent $_1: __func::target
static uint32_t t_502198(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe20b60u ? self + 4 : 0; }
RW_FUNCTION(0x502198, "MaterialScrollView::registItemTouchEvent $_1 __func::target", t_502198);

// MaterialScrollView::registItemTouchEvent $_1: __func::target_type
static uint32_t t_5021B8() { return 0xf82490u; }
RW_FUNCTION(0x5021B8, "MaterialScrollView::registItemTouchEvent $_1 __func::target_type", t_5021B8);

// MaterialScrollView::registItemTouchEvent $_0: __func::~__func (deleting)
static void t_5021CC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5021CC, "MaterialScrollView::registItemTouchEvent $_0 __func::~__func (deleting)", t_5021CC);

// MaterialScrollView::registItemTouchEvent $_0: __func::__clone()
static uint32_t t_5021D4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf55e14u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5021D4, "MaterialScrollView::registItemTouchEvent $_0 __func::__clone()", t_5021D4);

// MaterialScrollView::registItemTouchEvent $_0: __func::__clone(__base*)
static void t_502204(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55e14u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x502204, "MaterialScrollView::registItemTouchEvent $_0 __func::__clone(__base*)", t_502204);

// MaterialScrollView::registItemTouchEvent $_0: __func::destroy
static void t_502228(uint32_t self) {
}
RW_FUNCTION(0x502228, "MaterialScrollView::registItemTouchEvent $_0 __func::destroy", t_502228);

// MaterialScrollView::registItemTouchEvent $_0: __func::destroy_deallocate
static void t_50222A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50222A, "MaterialScrollView::registItemTouchEvent $_0 __func::destroy_deallocate", t_50222A);

// MaterialScrollView::registItemTouchEvent $_0: __func::target
static uint32_t t_5023E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe20c50u ? self + 4 : 0; }
RW_FUNCTION(0x5023E8, "MaterialScrollView::registItemTouchEvent $_0 __func::target", t_5023E8);

// MaterialScrollView::registItemTouchEvent $_0: __func::target_type
static uint32_t t_502408() { return 0xf824a4u; }
RW_FUNCTION(0x502408, "MaterialScrollView::registItemTouchEvent $_0 __func::target_type", t_502408);
