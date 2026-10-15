// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the BattleResultScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// BattleResultScrollView::setDropImage $_8 (shared): __func::~__func
static uint32_t t_5154BA(uint32_t self) {
    return self;
}
RW_FUNCTION(0x5154BA, "BattleResultScrollView::setDropImage $_8 (shared) __func::~__func", t_5154BA);

// BattleResultScrollView::setDropImage $_8: __func::~__func (deleting)
static void t_5154BC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5154BC, "BattleResultScrollView::setDropImage $_8 __func::~__func (deleting)", t_5154BC);

// BattleResultScrollView::setDropImage $_8: __func::__clone()
static uint32_t t_5154C4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf565a4u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20 + 0);
        uint32_t v7 = ld32(self + 20 + 4);
        uint32_t v8 = ld32(self + 20 + 8);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
    }
    return v1;
}
RW_FUNCTION(0x5154C4, "BattleResultScrollView::setDropImage $_8 __func::__clone()", t_5154C4);

// BattleResultScrollView::setDropImage $_8: __func::__clone(__base*)
static void t_515500(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf565a4u);
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
        uint32_t v7 = ld32(self + 28);
        st32(p + 28, v7);
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x515500, "BattleResultScrollView::setDropImage $_8 __func::__clone(__base*)", t_515500);

// BattleResultScrollView::setDropImage $_8: __func::destroy
static void t_515530(uint32_t self) {
}
RW_FUNCTION(0x515530, "BattleResultScrollView::setDropImage $_8 __func::destroy", t_515530);

// BattleResultScrollView::setDropImage $_8: __func::destroy_deallocate
static void t_515532(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x515532, "BattleResultScrollView::setDropImage $_8 __func::destroy_deallocate", t_515532);

// BattleResultScrollView::setDropImage $_8: __func::target
static uint32_t t_51564C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23880u ? self + 4 : 0; }
RW_FUNCTION(0x51564C, "BattleResultScrollView::setDropImage $_8 __func::target", t_51564C);

// BattleResultScrollView::setDropImage $_8: __func::target_type
static uint32_t t_51566C() { return 0xf84314u; }
RW_FUNCTION(0x51566C, "BattleResultScrollView::setDropImage $_8 __func::target_type", t_51566C);

// BattleResultScrollView::pushBackItem $_6: __func::target
static uint32_t t_51582C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23940u ? self + 4 : 0; }
RW_FUNCTION(0x51582C, "BattleResultScrollView::pushBackItem $_6 __func::target", t_51582C);

// BattleResultScrollView::pushBackItem $_6: __func::target_type
static uint32_t t_51584C() { return 0xf84328u; }
RW_FUNCTION(0x51584C, "BattleResultScrollView::pushBackItem $_6 __func::target_type", t_51584C);

// BattleResultScrollView::pushFrontItem $_4: __func::target
static uint32_t t_515A0C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe239e0u ? self + 4 : 0; }
RW_FUNCTION(0x515A0C, "BattleResultScrollView::pushFrontItem $_4 __func::target", t_515A0C);

// BattleResultScrollView::pushFrontItem $_4: __func::target_type
static uint32_t t_515A2C() { return 0xf8433cu; }
RW_FUNCTION(0x515A2C, "BattleResultScrollView::pushFrontItem $_4 __func::target_type", t_515A2C);

// BattleResultScrollView::registItemTouchEvent $_2: __func::~__func (deleting)
static void t_515A40(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x515A40, "BattleResultScrollView::registItemTouchEvent $_2 __func::~__func (deleting)", t_515A40);

// BattleResultScrollView::registItemTouchEvent $_2: __func::__clone()
static uint32_t t_515A48(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf56628u);
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
RW_FUNCTION(0x515A48, "BattleResultScrollView::registItemTouchEvent $_2 __func::__clone()", t_515A48);

// BattleResultScrollView::registItemTouchEvent $_2: __func::__clone(__base*)
static void t_515A7C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56628u);
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
RW_FUNCTION(0x515A7C, "BattleResultScrollView::registItemTouchEvent $_2 __func::__clone(__base*)", t_515A7C);

// BattleResultScrollView::registItemTouchEvent $_2: __func::destroy
static void t_515AA8(uint32_t self) {
}
RW_FUNCTION(0x515AA8, "BattleResultScrollView::registItemTouchEvent $_2 __func::destroy", t_515AA8);

// BattleResultScrollView::registItemTouchEvent $_2: __func::destroy_deallocate
static void t_515AAA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x515AAA, "BattleResultScrollView::registItemTouchEvent $_2 __func::destroy_deallocate", t_515AAA);

// BattleResultScrollView::registItemTouchEvent $_2: __func::target
static uint32_t t_515AC0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23ac0u ? self + 4 : 0; }
RW_FUNCTION(0x515AC0, "BattleResultScrollView::registItemTouchEvent $_2 __func::target", t_515AC0);

// BattleResultScrollView::registItemTouchEvent $_2: __func::target_type
static uint32_t t_515AE0() { return 0xf84350u; }
RW_FUNCTION(0x515AE0, "BattleResultScrollView::registItemTouchEvent $_2 __func::target_type", t_515AE0);
