// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SellMedalScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SellMedalScrollView::registItemTouchEvent $_2: __func::~__func (deleting)
static void t_50051C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50051C, "SellMedalScrollView::registItemTouchEvent $_2 __func::~__func (deleting)", t_50051C);

// SellMedalScrollView::registItemTouchEvent $_2: __func::__clone()
static uint32_t t_500524(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf55d0cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x500524, "SellMedalScrollView::registItemTouchEvent $_2 __func::__clone()", t_500524);

// SellMedalScrollView::registItemTouchEvent $_2: __func::__clone(__base*)
static void t_500558(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55d0cu);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x500558, "SellMedalScrollView::registItemTouchEvent $_2 __func::__clone(__base*)", t_500558);

// SellMedalScrollView::registItemTouchEvent $_2: __func::destroy
static void t_500580(uint32_t self) {
}
RW_FUNCTION(0x500580, "SellMedalScrollView::registItemTouchEvent $_2 __func::destroy", t_500580);

// SellMedalScrollView::registItemTouchEvent $_2: __func::destroy_deallocate
static void t_500582(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x500582, "SellMedalScrollView::registItemTouchEvent $_2 __func::destroy_deallocate", t_500582);

// SellMedalScrollView::registItemTouchEvent $_2: __func::target
static uint32_t t_50074C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe20660u ? self + 4 : 0; }
RW_FUNCTION(0x50074C, "SellMedalScrollView::registItemTouchEvent $_2 __func::target", t_50074C);

// SellMedalScrollView::registItemTouchEvent $_2: __func::target_type
static uint32_t t_50076C() { return 0xf82094u; }
RW_FUNCTION(0x50076C, "SellMedalScrollView::registItemTouchEvent $_2 __func::target_type", t_50076C);

// SellMedalScrollView::registItemTouchEvent $_2 UlvE_: __func::~__func (deleting)
static void t_500780(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x500780, "SellMedalScrollView::registItemTouchEvent $_2 UlvE_ __func::~__func (deleting)", t_500780);

// SellMedalScrollView::registItemTouchEvent $_2 UlvE_: __func::__clone()
static uint32_t t_500788(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf55d38u);
    }
    return v1;
}
RW_FUNCTION(0x500788, "SellMedalScrollView::registItemTouchEvent $_2 UlvE_ __func::__clone()", t_500788);

// SellMedalScrollView::registItemTouchEvent $_2 UlvE_: __func::__clone(__base*)
static void t_5007AC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55d38u);
    }
}
RW_FUNCTION(0x5007AC, "SellMedalScrollView::registItemTouchEvent $_2 UlvE_ __func::__clone(__base*)", t_5007AC);

// SellMedalScrollView::registItemTouchEvent $_2 UlvE_: __func::destroy
static void t_5007C8(uint32_t self) {
}
RW_FUNCTION(0x5007C8, "SellMedalScrollView::registItemTouchEvent $_2 UlvE_ __func::destroy", t_5007C8);

// SellMedalScrollView::registItemTouchEvent $_2 UlvE_: __func::destroy_deallocate
static void t_5007CA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5007CA, "SellMedalScrollView::registItemTouchEvent $_2 UlvE_ __func::destroy_deallocate", t_5007CA);

// SellMedalScrollView::registItemTouchEvent $_2 UlvE_: __func::target
static uint32_t t_5007D4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe20760u ? self + 4 : 0; }
RW_FUNCTION(0x5007D4, "SellMedalScrollView::registItemTouchEvent $_2 UlvE_ __func::target", t_5007D4);

// SellMedalScrollView::registItemTouchEvent $_2 UlvE_: __func::target_type
static uint32_t t_5007F4() { return 0xf820a8u; }
RW_FUNCTION(0x5007F4, "SellMedalScrollView::registItemTouchEvent $_2 UlvE_ __func::target_type", t_5007F4);

// SellMedalScrollView::registItemTouchEvent $_1: __func::~__func (deleting)
static void t_500808(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x500808, "SellMedalScrollView::registItemTouchEvent $_1 __func::~__func (deleting)", t_500808);

// SellMedalScrollView::registItemTouchEvent $_1: __func::__clone()
static uint32_t t_500810(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf55d64u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x500810, "SellMedalScrollView::registItemTouchEvent $_1 __func::__clone()", t_500810);

// SellMedalScrollView::registItemTouchEvent $_1: __func::__clone(__base*)
static void t_500840(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55d64u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x500840, "SellMedalScrollView::registItemTouchEvent $_1 __func::__clone(__base*)", t_500840);

// SellMedalScrollView::registItemTouchEvent $_1: __func::destroy
static void t_500864(uint32_t self) {
}
RW_FUNCTION(0x500864, "SellMedalScrollView::registItemTouchEvent $_1 __func::destroy", t_500864);

// SellMedalScrollView::registItemTouchEvent $_1: __func::destroy_deallocate
static void t_500866(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x500866, "SellMedalScrollView::registItemTouchEvent $_1 __func::destroy_deallocate", t_500866);
