// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// AlbumScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// AlbumScrollView::registItemTouchEvent $_2: __func::~__func (deleting)
static void t_50C25C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50C25C, "AlbumScrollView::registItemTouchEvent $_2 __func::~__func (deleting)", t_50C25C);

// AlbumScrollView::registItemTouchEvent $_2: __func::__clone()
static uint32_t t_50C264(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf561b0u);
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
RW_FUNCTION(0x50C264, "AlbumScrollView::registItemTouchEvent $_2 __func::__clone()", t_50C264);

// AlbumScrollView::registItemTouchEvent $_2: __func::__clone(__base*)
static void t_50C298(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf561b0u);
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
RW_FUNCTION(0x50C298, "AlbumScrollView::registItemTouchEvent $_2 __func::__clone(__base*)", t_50C298);

// AlbumScrollView::registItemTouchEvent $_2: __func::destroy
static void t_50C2C4(uint32_t self) {
}
RW_FUNCTION(0x50C2C4, "AlbumScrollView::registItemTouchEvent $_2 __func::destroy", t_50C2C4);

// AlbumScrollView::registItemTouchEvent $_2: __func::destroy_deallocate
static void t_50C2C6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50C2C6, "AlbumScrollView::registItemTouchEvent $_2 __func::destroy_deallocate", t_50C2C6);

// AlbumScrollView::registItemTouchEvent $_2: __func::target
static uint32_t t_50C2DC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe222a0u ? self + 4 : 0; }
RW_FUNCTION(0x50C2DC, "AlbumScrollView::registItemTouchEvent $_2 __func::target", t_50C2DC);

// AlbumScrollView::registItemTouchEvent $_2: __func::target_type
static uint32_t t_50C2FC() { return 0xf83680u; }
RW_FUNCTION(0x50C2FC, "AlbumScrollView::registItemTouchEvent $_2 __func::target_type", t_50C2FC);

// AlbumScrollView::registItemTouchEvent $_3: __func::~__func (deleting)
static void t_50C610(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50C610, "AlbumScrollView::registItemTouchEvent $_3 __func::~__func (deleting)", t_50C610);

// AlbumScrollView::registItemTouchEvent $_3: __func::__clone()
static uint32_t t_50C618(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf561dcu);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x50C618, "AlbumScrollView::registItemTouchEvent $_3 __func::__clone()", t_50C618);

// AlbumScrollView::registItemTouchEvent $_3: __func::__clone(__base*)
static void t_50C648(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf561dcu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x50C648, "AlbumScrollView::registItemTouchEvent $_3 __func::__clone(__base*)", t_50C648);

// AlbumScrollView::registItemTouchEvent $_3: __func::destroy
static void t_50C66C(uint32_t self) {
}
RW_FUNCTION(0x50C66C, "AlbumScrollView::registItemTouchEvent $_3 __func::destroy", t_50C66C);

// AlbumScrollView::registItemTouchEvent $_3: __func::destroy_deallocate
static void t_50C66E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50C66E, "AlbumScrollView::registItemTouchEvent $_3 __func::destroy_deallocate", t_50C66E);

// AlbumScrollView::registItemTouchEvent $_3: __func::target
static uint32_t t_50C6D4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22370u ? self + 4 : 0; }
RW_FUNCTION(0x50C6D4, "AlbumScrollView::registItemTouchEvent $_3 __func::target", t_50C6D4);

// AlbumScrollView::registItemTouchEvent $_3: __func::target_type
static uint32_t t_50C6F4() { return 0xf83694u; }
RW_FUNCTION(0x50C6F4, "AlbumScrollView::registItemTouchEvent $_3 __func::target_type", t_50C6F4);

// AlbumScrollView::registItemTouchEvent $_1: __func::~__func (deleting)
static void t_50C708(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50C708, "AlbumScrollView::registItemTouchEvent $_1 __func::~__func (deleting)", t_50C708);

// AlbumScrollView::registItemTouchEvent $_1: __func::__clone()
static uint32_t t_50C710(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf56208u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x50C710, "AlbumScrollView::registItemTouchEvent $_1 __func::__clone()", t_50C710);

// AlbumScrollView::registItemTouchEvent $_1: __func::__clone(__base*)
static void t_50C748(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56208u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        uint32_t v6 = ld32(self + 24);
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x50C748, "AlbumScrollView::registItemTouchEvent $_1 __func::__clone(__base*)", t_50C748);

// AlbumScrollView::registItemTouchEvent $_1: __func::destroy
static void t_50C774(uint32_t self) {
}
RW_FUNCTION(0x50C774, "AlbumScrollView::registItemTouchEvent $_1 __func::destroy", t_50C774);

// AlbumScrollView::registItemTouchEvent $_1: __func::destroy_deallocate
static void t_50C776(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50C776, "AlbumScrollView::registItemTouchEvent $_1 __func::destroy_deallocate", t_50C776);

// AlbumScrollView::registItemTouchEvent $_1: __func::target
static uint32_t t_50C888(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22460u ? self + 4 : 0; }
RW_FUNCTION(0x50C888, "AlbumScrollView::registItemTouchEvent $_1 __func::target", t_50C888);

// AlbumScrollView::registItemTouchEvent $_1: __func::target_type
static uint32_t t_50C8A8() { return 0xf836a8u; }
RW_FUNCTION(0x50C8A8, "AlbumScrollView::registItemTouchEvent $_1 __func::target_type", t_50C8A8);

// AlbumScrollView::registItemTouchEvent $_0: __func::~__func (deleting)
static void t_50C8BC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50C8BC, "AlbumScrollView::registItemTouchEvent $_0 __func::~__func (deleting)", t_50C8BC);

// AlbumScrollView::registItemTouchEvent $_0: __func::__clone()
static uint32_t t_50C8C4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf56234u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x50C8C4, "AlbumScrollView::registItemTouchEvent $_0 __func::__clone()", t_50C8C4);

// AlbumScrollView::registItemTouchEvent $_0: __func::__clone(__base*)
static void t_50C8F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56234u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x50C8F4, "AlbumScrollView::registItemTouchEvent $_0 __func::__clone(__base*)", t_50C8F4);

// AlbumScrollView::registItemTouchEvent $_0: __func::destroy
static void t_50C918(uint32_t self) {
}
RW_FUNCTION(0x50C918, "AlbumScrollView::registItemTouchEvent $_0 __func::destroy", t_50C918);

// AlbumScrollView::registItemTouchEvent $_0: __func::destroy_deallocate
static void t_50C91A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50C91A, "AlbumScrollView::registItemTouchEvent $_0 __func::destroy_deallocate", t_50C91A);

// AlbumScrollView::registItemTouchEvent $_0: __func::target
static uint32_t t_50CA54(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22550u ? self + 4 : 0; }
RW_FUNCTION(0x50CA54, "AlbumScrollView::registItemTouchEvent $_0 __func::target", t_50CA54);

// AlbumScrollView::registItemTouchEvent $_0: __func::target_type
static uint32_t t_50CA74() { return 0xf836bcu; }
RW_FUNCTION(0x50CA74, "AlbumScrollView::registItemTouchEvent $_0 __func::target_type", t_50CA74);
