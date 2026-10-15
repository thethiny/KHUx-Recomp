// The std::function __func thunks of the lambdas attributed to Scale9SpriteR (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// AudioLWFNode::init $_2: __func::target
static uint32_t t_5EBE80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4ddf0u ? self + 4 : 0; }
RW_FUNCTION(0x5EBE80, "AudioLWFNode::init $_2 __func::target", t_5EBE80);

// AudioLWFNode::init $_2: __func::target_type
static uint32_t t_5EBEA0() { return 0xf952a0u; }
RW_FUNCTION(0x5EBEA0, "AudioLWFNode::init $_2 __func::target_type", t_5EBEA0);

// AudioLWFNode::init $_1: __func::~__func (deleting)
static void t_5EBEB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5EBEB4, "AudioLWFNode::init $_1 __func::~__func (deleting)", t_5EBEB4);

// AudioLWFNode::init $_1: __func::__clone()
static uint32_t t_5EBEBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5dd88u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5EBEBC, "AudioLWFNode::init $_1 __func::__clone()", t_5EBEBC);

// AudioLWFNode::init $_1: __func::__clone(__base*)
static void t_5EBEE8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5dd88u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5EBEE8, "AudioLWFNode::init $_1 __func::__clone(__base*)", t_5EBEE8);

// AudioLWFNode::init $_1: __func::destroy
static void t_5EBF08(uint32_t self) {
}
RW_FUNCTION(0x5EBF08, "AudioLWFNode::init $_1 __func::destroy", t_5EBF08);

// AudioLWFNode::init $_1: __func::destroy_deallocate
static void t_5EBF0A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5EBF0A, "AudioLWFNode::init $_1 __func::destroy_deallocate", t_5EBF0A);

// AudioLWFNode::init $_1: __func::target
static uint32_t t_5EBF3C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4de80u ? self + 4 : 0; }
RW_FUNCTION(0x5EBF3C, "AudioLWFNode::init $_1 __func::target", t_5EBF3C);

// AudioLWFNode::init $_1: __func::target_type
static uint32_t t_5EBF5C() { return 0xf952b4u; }
RW_FUNCTION(0x5EBF5C, "AudioLWFNode::init $_1 __func::target_type", t_5EBF5C);

// AudioLWFNode::init $_0: __func::~__func (deleting)
static void t_5EBF70(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5EBF70, "AudioLWFNode::init $_0 __func::~__func (deleting)", t_5EBF70);

// AudioLWFNode::init $_0: __func::__clone()
static uint32_t t_5EBF78(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ddb4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5EBF78, "AudioLWFNode::init $_0 __func::__clone()", t_5EBF78);

// AudioLWFNode::init $_0: __func::__clone(__base*)
static void t_5EBFA4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ddb4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5EBFA4, "AudioLWFNode::init $_0 __func::__clone(__base*)", t_5EBFA4);

// AudioLWFNode::init $_0: __func::destroy
static void t_5EBFC4(uint32_t self) {
}
RW_FUNCTION(0x5EBFC4, "AudioLWFNode::init $_0 __func::destroy", t_5EBFC4);

// AudioLWFNode::init $_0: __func::destroy_deallocate
static void t_5EBFC6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5EBFC6, "AudioLWFNode::init $_0 __func::destroy_deallocate", t_5EBFC6);

// AudioLWFNode::init $_0: __func::target
static uint32_t t_5EBFDC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4df10u ? self + 4 : 0; }
RW_FUNCTION(0x5EBFDC, "AudioLWFNode::init $_0 __func::target", t_5EBFDC);

// AudioLWFNode::init $_0: __func::target_type
static uint32_t t_5EBFFC() { return 0xf952c8u; }
RW_FUNCTION(0x5EBFFC, "AudioLWFNode::init $_0 __func::target_type", t_5EBFFC);
