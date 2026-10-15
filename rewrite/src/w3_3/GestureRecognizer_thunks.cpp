// The std::function __func thunks of the lambdas attributed to GestureRecognizer (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// GestureRecognizer $_3 (shared): __func::~__func
static uint32_t t_4F3794(uint32_t self) {
    return self;
}
RW_FUNCTION(0x4F3794, "GestureRecognizer $_3 (shared) __func::~__func", t_4F3794);

// GestureRecognizer $_3: __func::~__func (deleting)
static void t_4F3796(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4F3796, "GestureRecognizer $_3 __func::~__func (deleting)", t_4F3796);

// GestureRecognizer $_3: __func::__clone()
static uint32_t t_4F37A0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf55894u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x4F37A0, "GestureRecognizer $_3 __func::__clone()", t_4F37A0);

// GestureRecognizer $_3: __func::__clone(__base*)
static void t_4F37CC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55894u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x4F37CC, "GestureRecognizer $_3 __func::__clone(__base*)", t_4F37CC);

// GestureRecognizer $_3: __func::destroy
static void t_4F37EC(uint32_t self) {
}
RW_FUNCTION(0x4F37EC, "GestureRecognizer $_3 __func::destroy", t_4F37EC);

// GestureRecognizer $_3: __func::destroy_deallocate
static void t_4F37EE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4F37EE, "GestureRecognizer $_3 __func::destroy_deallocate", t_4F37EE);

// GestureRecognizer $_3: __func::target
static uint32_t t_4F3800(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1efa0u ? self + 4 : 0; }
RW_FUNCTION(0x4F3800, "GestureRecognizer $_3 __func::target", t_4F3800);

// GestureRecognizer $_3: __func::target_type
static uint32_t t_4F3820() { return 0xf7fea4u; }
RW_FUNCTION(0x4F3820, "GestureRecognizer $_3 __func::target_type", t_4F3820);

// GestureRecognizer $_2: __func::~__func (deleting)
static void t_4F3834(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4F3834, "GestureRecognizer $_2 __func::~__func (deleting)", t_4F3834);

// GestureRecognizer $_2: __func::__clone()
static uint32_t t_4F383C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf558c0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x4F383C, "GestureRecognizer $_2 __func::__clone()", t_4F383C);

// GestureRecognizer $_2: __func::__clone(__base*)
static void t_4F3868(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf558c0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x4F3868, "GestureRecognizer $_2 __func::__clone(__base*)", t_4F3868);

// GestureRecognizer $_2: __func::destroy
static void t_4F3888(uint32_t self) {
}
RW_FUNCTION(0x4F3888, "GestureRecognizer $_2 __func::destroy", t_4F3888);

// GestureRecognizer $_2: __func::destroy_deallocate
static void t_4F388A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4F388A, "GestureRecognizer $_2 __func::destroy_deallocate", t_4F388A);

// GestureRecognizer $_2: __func::target
static uint32_t t_4F389C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1f050u ? self + 4 : 0; }
RW_FUNCTION(0x4F389C, "GestureRecognizer $_2 __func::target", t_4F389C);

// GestureRecognizer $_2: __func::target_type
static uint32_t t_4F38BC() { return 0xf7feb8u; }
RW_FUNCTION(0x4F38BC, "GestureRecognizer $_2 __func::target_type", t_4F38BC);

// GestureRecognizer $_1: __func::~__func (deleting)
static void t_4F38D0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4F38D0, "GestureRecognizer $_1 __func::~__func (deleting)", t_4F38D0);

// GestureRecognizer $_1: __func::__clone()
static uint32_t t_4F38D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf558ecu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x4F38D8, "GestureRecognizer $_1 __func::__clone()", t_4F38D8);

// GestureRecognizer $_1: __func::__clone(__base*)
static void t_4F3904(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf558ecu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x4F3904, "GestureRecognizer $_1 __func::__clone(__base*)", t_4F3904);
