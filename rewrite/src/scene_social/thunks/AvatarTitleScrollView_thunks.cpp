// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// AvatarTitleScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// AvatarTitleScrollView::registItemTouchEvent $_2: __func::~__func (deleting)
static void t_511354(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x511354, "AvatarTitleScrollView::registItemTouchEvent $_2 __func::~__func (deleting)", t_511354);

// AvatarTitleScrollView::registItemTouchEvent $_2: __func::__clone()
static uint32_t t_51135C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf564f4u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x51135C, "AvatarTitleScrollView::registItemTouchEvent $_2 __func::__clone()", t_51135C);

// AvatarTitleScrollView::registItemTouchEvent $_2: __func::__clone(__base*)
static void t_51138C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf564f4u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x51138C, "AvatarTitleScrollView::registItemTouchEvent $_2 __func::__clone(__base*)", t_51138C);

// AvatarTitleScrollView::registItemTouchEvent $_2: __func::destroy
static void t_5113B4(uint32_t self) {
}
RW_FUNCTION(0x5113B4, "AvatarTitleScrollView::registItemTouchEvent $_2 __func::destroy", t_5113B4);

// AvatarTitleScrollView::registItemTouchEvent $_2: __func::destroy_deallocate
static void t_5113B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5113B6, "AvatarTitleScrollView::registItemTouchEvent $_2 __func::destroy_deallocate", t_5113B6);

// AvatarTitleScrollView::registItemTouchEvent $_2: __func::target
static uint32_t t_5113CC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23480u ? self + 4 : 0; }
RW_FUNCTION(0x5113CC, "AvatarTitleScrollView::registItemTouchEvent $_2 __func::target", t_5113CC);

// AvatarTitleScrollView::registItemTouchEvent $_2: __func::target_type
static uint32_t t_5113EC() { return 0xf83f2cu; }
RW_FUNCTION(0x5113EC, "AvatarTitleScrollView::registItemTouchEvent $_2 __func::target_type", t_5113EC);

// AvatarTitleScrollView::registItemTouchEvent $_3: __func::~__func (deleting)
static void t_511608(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x511608, "AvatarTitleScrollView::registItemTouchEvent $_3 __func::~__func (deleting)", t_511608);

// AvatarTitleScrollView::registItemTouchEvent $_3: __func::__clone()
static uint32_t t_511610(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf56520u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x511610, "AvatarTitleScrollView::registItemTouchEvent $_3 __func::__clone()", t_511610);

// AvatarTitleScrollView::registItemTouchEvent $_3: __func::__clone(__base*)
static void t_51163C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56520u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x51163C, "AvatarTitleScrollView::registItemTouchEvent $_3 __func::__clone(__base*)", t_51163C);

// AvatarTitleScrollView::registItemTouchEvent $_3: __func::destroy
static void t_51165C(uint32_t self) {
}
RW_FUNCTION(0x51165C, "AvatarTitleScrollView::registItemTouchEvent $_3 __func::destroy", t_51165C);

// AvatarTitleScrollView::registItemTouchEvent $_3: __func::destroy_deallocate
static void t_51165E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51165E, "AvatarTitleScrollView::registItemTouchEvent $_3 __func::destroy_deallocate", t_51165E);

// AvatarTitleScrollView::registItemTouchEvent $_3: __func::target
static uint32_t t_5116B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23550u ? self + 4 : 0; }
RW_FUNCTION(0x5116B0, "AvatarTitleScrollView::registItemTouchEvent $_3 __func::target", t_5116B0);

// AvatarTitleScrollView::registItemTouchEvent $_3: __func::target_type
static uint32_t t_5116D0() { return 0xf83f40u; }
RW_FUNCTION(0x5116D0, "AvatarTitleScrollView::registItemTouchEvent $_3 __func::target_type", t_5116D0);

// AvatarTitleScrollView::registItemTouchEvent $_1: __func::~__func (deleting)
static void t_5116E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5116E4, "AvatarTitleScrollView::registItemTouchEvent $_1 __func::~__func (deleting)", t_5116E4);

// AvatarTitleScrollView::registItemTouchEvent $_1: __func::__clone()
static uint32_t t_5116EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5654cu);
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
RW_FUNCTION(0x5116EC, "AvatarTitleScrollView::registItemTouchEvent $_1 __func::__clone()", t_5116EC);

// AvatarTitleScrollView::registItemTouchEvent $_1: __func::__clone(__base*)
static void t_511720(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5654cu);
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
RW_FUNCTION(0x511720, "AvatarTitleScrollView::registItemTouchEvent $_1 __func::__clone(__base*)", t_511720);

// AvatarTitleScrollView::registItemTouchEvent $_1: __func::destroy
static void t_511748(uint32_t self) {
}
RW_FUNCTION(0x511748, "AvatarTitleScrollView::registItemTouchEvent $_1 __func::destroy", t_511748);

// AvatarTitleScrollView::registItemTouchEvent $_1: __func::destroy_deallocate
static void t_51174A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51174A, "AvatarTitleScrollView::registItemTouchEvent $_1 __func::destroy_deallocate", t_51174A);
