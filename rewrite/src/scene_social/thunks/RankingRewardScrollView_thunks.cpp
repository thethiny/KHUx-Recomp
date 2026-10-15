// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// RankingRewardScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// RankingRewardScrollView::registItemTouchEvent $_4: __func::~__func (deleting)
static void t_5615C0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5615C0, "RankingRewardScrollView::registItemTouchEvent $_4 __func::~__func (deleting)", t_5615C0);

// RankingRewardScrollView::registItemTouchEvent $_4: __func::__clone()
static uint32_t t_5615C8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf58c7cu);
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
RW_FUNCTION(0x5615C8, "RankingRewardScrollView::registItemTouchEvent $_4 __func::__clone()", t_5615C8);

// RankingRewardScrollView::registItemTouchEvent $_4: __func::__clone(__base*)
static void t_5615FC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf58c7cu);
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
RW_FUNCTION(0x5615FC, "RankingRewardScrollView::registItemTouchEvent $_4 __func::__clone(__base*)", t_5615FC);

// RankingRewardScrollView::registItemTouchEvent $_4: __func::destroy
static void t_561628(uint32_t self) {
}
RW_FUNCTION(0x561628, "RankingRewardScrollView::registItemTouchEvent $_4 __func::destroy", t_561628);

// RankingRewardScrollView::registItemTouchEvent $_4: __func::destroy_deallocate
static void t_56162A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x56162A, "RankingRewardScrollView::registItemTouchEvent $_4 __func::destroy_deallocate", t_56162A);

// RankingRewardScrollView::registItemTouchEvent $_4: __func::target
static uint32_t t_561640(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe31200u ? self + 4 : 0; }
RW_FUNCTION(0x561640, "RankingRewardScrollView::registItemTouchEvent $_4 __func::target", t_561640);

// RankingRewardScrollView::registItemTouchEvent $_4: __func::target_type
static uint32_t t_561660() { return 0xf89e78u; }
RW_FUNCTION(0x561660, "RankingRewardScrollView::registItemTouchEvent $_4 __func::target_type", t_561660);

// RankingRewardScrollView::registItemTouchEvent $_5: __func::~__func (deleting)
static void t_561838(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x561838, "RankingRewardScrollView::registItemTouchEvent $_5 __func::~__func (deleting)", t_561838);

// RankingRewardScrollView::registItemTouchEvent $_5: __func::__clone()
static uint32_t t_561840(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf58ca8u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x561840, "RankingRewardScrollView::registItemTouchEvent $_5 __func::__clone()", t_561840);

// RankingRewardScrollView::registItemTouchEvent $_5: __func::__clone(__base*)
static void t_561870(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf58ca8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x561870, "RankingRewardScrollView::registItemTouchEvent $_5 __func::__clone(__base*)", t_561870);

// RankingRewardScrollView::registItemTouchEvent $_5: __func::destroy
static void t_561894(uint32_t self) {
}
RW_FUNCTION(0x561894, "RankingRewardScrollView::registItemTouchEvent $_5 __func::destroy", t_561894);

// RankingRewardScrollView::registItemTouchEvent $_5: __func::destroy_deallocate
static void t_561896(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x561896, "RankingRewardScrollView::registItemTouchEvent $_5 __func::destroy_deallocate", t_561896);

// RankingRewardScrollView::registItemTouchEvent $_5: __func::target
static uint32_t t_561904(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe312e0u ? self + 4 : 0; }
RW_FUNCTION(0x561904, "RankingRewardScrollView::registItemTouchEvent $_5 __func::target", t_561904);

// RankingRewardScrollView::registItemTouchEvent $_5: __func::target_type
static uint32_t t_561924() { return 0xf89e8cu; }
RW_FUNCTION(0x561924, "RankingRewardScrollView::registItemTouchEvent $_5 __func::target_type", t_561924);

// RankingRewardScrollView::registItemTouchEvent $_3: __func::~__func (deleting)
static void t_561938(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x561938, "RankingRewardScrollView::registItemTouchEvent $_3 __func::~__func (deleting)", t_561938);

// RankingRewardScrollView::registItemTouchEvent $_3: __func::__clone()
static uint32_t t_561940(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf58cd4u);
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
RW_FUNCTION(0x561940, "RankingRewardScrollView::registItemTouchEvent $_3 __func::__clone()", t_561940);

// RankingRewardScrollView::registItemTouchEvent $_3: __func::__clone(__base*)
static void t_561978(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf58cd4u);
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
RW_FUNCTION(0x561978, "RankingRewardScrollView::registItemTouchEvent $_3 __func::__clone(__base*)", t_561978);

// RankingRewardScrollView::registItemTouchEvent $_3: __func::destroy
static void t_5619A4(uint32_t self) {
}
RW_FUNCTION(0x5619A4, "RankingRewardScrollView::registItemTouchEvent $_3 __func::destroy", t_5619A4);

// RankingRewardScrollView::registItemTouchEvent $_3: __func::destroy_deallocate
static void t_5619A6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5619A6, "RankingRewardScrollView::registItemTouchEvent $_3 __func::destroy_deallocate", t_5619A6);

// RankingRewardScrollView::registItemTouchEvent $_3: __func::target
static uint32_t t_561AB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe313d0u ? self + 4 : 0; }
RW_FUNCTION(0x561AB8, "RankingRewardScrollView::registItemTouchEvent $_3 __func::target", t_561AB8);

// RankingRewardScrollView::registItemTouchEvent $_3: __func::target_type
static uint32_t t_561AD8() { return 0xf89ea0u; }
RW_FUNCTION(0x561AD8, "RankingRewardScrollView::registItemTouchEvent $_3 __func::target_type", t_561AD8);

// RankingRewardScrollView::registItemTouchEvent $_2: __func::~__func (deleting)
static void t_561AEC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x561AEC, "RankingRewardScrollView::registItemTouchEvent $_2 __func::~__func (deleting)", t_561AEC);

// RankingRewardScrollView::registItemTouchEvent $_2: __func::__clone()
static uint32_t t_561AF4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf58d00u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x561AF4, "RankingRewardScrollView::registItemTouchEvent $_2 __func::__clone()", t_561AF4);

// RankingRewardScrollView::registItemTouchEvent $_2: __func::__clone(__base*)
static void t_561B24(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf58d00u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x561B24, "RankingRewardScrollView::registItemTouchEvent $_2 __func::__clone(__base*)", t_561B24);

// RankingRewardScrollView::registItemTouchEvent $_2: __func::destroy
static void t_561B48(uint32_t self) {
}
RW_FUNCTION(0x561B48, "RankingRewardScrollView::registItemTouchEvent $_2 __func::destroy", t_561B48);

// RankingRewardScrollView::registItemTouchEvent $_2: __func::destroy_deallocate
static void t_561B4A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x561B4A, "RankingRewardScrollView::registItemTouchEvent $_2 __func::destroy_deallocate", t_561B4A);
