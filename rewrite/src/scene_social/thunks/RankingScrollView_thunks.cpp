// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// RankingScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// RankingScrollView::initAvatarLayer $_1: __func::~__func (deleting)
static void t_552E8C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x552E8C, "RankingScrollView::initAvatarLayer $_1 __func::~__func (deleting)", t_552E8C);

// RankingScrollView::initAvatarLayer $_1: __func::__clone()
static uint32_t t_552E94(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf58a6cu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x552E94, "RankingScrollView::initAvatarLayer $_1 __func::__clone()", t_552E94);

// RankingScrollView::initAvatarLayer $_1: __func::__clone(__base*)
static void t_552EC8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf58a6cu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x552EC8, "RankingScrollView::initAvatarLayer $_1 __func::__clone(__base*)", t_552EC8);

// RankingScrollView::initAvatarLayer $_1: __func::destroy
static void t_552EF4(uint32_t self) {
}
RW_FUNCTION(0x552EF4, "RankingScrollView::initAvatarLayer $_1 __func::destroy", t_552EF4);

// RankingScrollView::initAvatarLayer $_1: __func::destroy_deallocate
static void t_552EF6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x552EF6, "RankingScrollView::initAvatarLayer $_1 __func::destroy_deallocate", t_552EF6);
