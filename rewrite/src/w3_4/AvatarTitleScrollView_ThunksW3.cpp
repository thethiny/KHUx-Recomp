// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the AvatarTitleScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// AvatarTitleScrollView::registItemTouchEvent $_1: __func::target
static uint32_t t_5118C0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23640u ? self + 4 : 0; }
RW_FUNCTION(0x5118C0, "AvatarTitleScrollView::registItemTouchEvent $_1 __func::target", t_5118C0);

// AvatarTitleScrollView::registItemTouchEvent $_1: __func::target_type
static uint32_t t_5118E0() { return 0xf83f54u; }
RW_FUNCTION(0x5118E0, "AvatarTitleScrollView::registItemTouchEvent $_1 __func::target_type", t_5118E0);

// AvatarTitleScrollView::registItemTouchEvent $_0: __func::~__func (deleting)
static void t_5118F4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5118F4, "AvatarTitleScrollView::registItemTouchEvent $_0 __func::~__func (deleting)", t_5118F4);

// AvatarTitleScrollView::registItemTouchEvent $_0: __func::__clone()
static uint32_t t_5118FC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf56578u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5118FC, "AvatarTitleScrollView::registItemTouchEvent $_0 __func::__clone()", t_5118FC);

// AvatarTitleScrollView::registItemTouchEvent $_0: __func::__clone(__base*)
static void t_51192C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56578u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x51192C, "AvatarTitleScrollView::registItemTouchEvent $_0 __func::__clone(__base*)", t_51192C);

// AvatarTitleScrollView::registItemTouchEvent $_0: __func::destroy
static void t_511950(uint32_t self) {
}
RW_FUNCTION(0x511950, "AvatarTitleScrollView::registItemTouchEvent $_0 __func::destroy", t_511950);

// AvatarTitleScrollView::registItemTouchEvent $_0: __func::destroy_deallocate
static void t_511952(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x511952, "AvatarTitleScrollView::registItemTouchEvent $_0 __func::destroy_deallocate", t_511952);

// AvatarTitleScrollView::registItemTouchEvent $_0: __func::target
static uint32_t t_511B80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23730u ? self + 4 : 0; }
RW_FUNCTION(0x511B80, "AvatarTitleScrollView::registItemTouchEvent $_0 __func::target", t_511B80);

// AvatarTitleScrollView::registItemTouchEvent $_0: __func::target_type
static uint32_t t_511BA0() { return 0xf83f68u; }
RW_FUNCTION(0x511BA0, "AvatarTitleScrollView::registItemTouchEvent $_0 __func::target_type", t_511BA0);
