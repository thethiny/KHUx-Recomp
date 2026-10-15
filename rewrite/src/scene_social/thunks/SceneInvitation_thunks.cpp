// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneInvitation lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneInvitation::init $_2: __func::~__func (deleting)
static void t_716790(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x716790, "SceneInvitation::init $_2 __func::~__func (deleting)", t_716790);

// SceneInvitation::init $_2: __func::__clone()
static uint32_t t_716798(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6b614u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x716798, "SceneInvitation::init $_2 __func::__clone()", t_716798);

// SceneInvitation::init $_2: __func::__clone(__base*)
static void t_7167C8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b614u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7167C8, "SceneInvitation::init $_2 __func::__clone(__base*)", t_7167C8);

// SceneInvitation::init $_2: __func::destroy
static void t_7167F0(uint32_t self) {
}
RW_FUNCTION(0x7167F0, "SceneInvitation::init $_2 __func::destroy", t_7167F0);

// SceneInvitation::init $_2: __func::destroy_deallocate
static void t_7167F2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7167F2, "SceneInvitation::init $_2 __func::destroy_deallocate", t_7167F2);

// SceneInvitation::init $_2: __func::target
static uint32_t t_71696C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe91410u ? self + 4 : 0; }
RW_FUNCTION(0x71696C, "SceneInvitation::init $_2 __func::target", t_71696C);

// SceneInvitation::init $_2: __func::target_type
static uint32_t t_71698C() { return 0xfa4e3cu; }
RW_FUNCTION(0x71698C, "SceneInvitation::init $_2 __func::target_type", t_71698C);

// SceneInvitation::init $_2 UlbE_: __func::~__func (deleting)
static void t_7169A0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7169A0, "SceneInvitation::init $_2 UlbE_ __func::~__func (deleting)", t_7169A0);

// SceneInvitation::init $_2 UlbE_: __func::__clone()
static uint32_t t_7169A8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf6b640u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7169A8, "SceneInvitation::init $_2 UlbE_ __func::__clone()", t_7169A8);

// SceneInvitation::init $_2 UlbE_: __func::__clone(__base*)
static void t_7169D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b640u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7169D8, "SceneInvitation::init $_2 UlbE_ __func::__clone(__base*)", t_7169D8);

// SceneInvitation::init $_2 UlbE_: __func::destroy
static void t_7169FC(uint32_t self) {
}
RW_FUNCTION(0x7169FC, "SceneInvitation::init $_2 UlbE_ __func::destroy", t_7169FC);

// SceneInvitation::init $_2 UlbE_: __func::destroy_deallocate
static void t_7169FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7169FE, "SceneInvitation::init $_2 UlbE_ __func::destroy_deallocate", t_7169FE);
