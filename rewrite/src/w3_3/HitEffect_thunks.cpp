// The std::function __func thunks of the lambdas attributed to HitEffect (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ShowEffect $_1: __func::target_type
static uint32_t t_5D8F6C() { return 0xf919f4u; }
RW_FUNCTION(0x5D8F6C, "ShowEffect $_1 __func::target_type", t_5D8F6C);

// ShowEffect $_0: __func::~__func (deleting)
static void t_5D8F80(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8F80, "ShowEffect $_0 __func::~__func (deleting)", t_5D8F80);

// ShowEffect $_0: __func::__clone()
static uint32_t t_5D8F88(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d81cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5D8F88, "ShowEffect $_0 __func::__clone()", t_5D8F88);

// ShowEffect $_0: __func::__clone(__base*)
static void t_5D8FB4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d81cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5D8FB4, "ShowEffect $_0 __func::__clone(__base*)", t_5D8FB4);

// ShowEffect $_0: __func::destroy
static void t_5D8FD4(uint32_t self) {
}
RW_FUNCTION(0x5D8FD4, "ShowEffect $_0 __func::destroy", t_5D8FD4);

// ShowEffect $_0: __func::destroy_deallocate
static void t_5D8FD6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8FD6, "ShowEffect $_0 __func::destroy_deallocate", t_5D8FD6);

// ShowEffect $_0: __func::target
static uint32_t t_5D91F0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4c640u ? self + 4 : 0; }
RW_FUNCTION(0x5D91F0, "ShowEffect $_0 __func::target", t_5D91F0);

// ShowEffect $_0: __func::target_type
static uint32_t t_5D9210() { return 0xf91a08u; }
RW_FUNCTION(0x5D9210, "ShowEffect $_0 __func::target_type", t_5D9210);

// HitEffect::deploy $_0: __func::~__func
static uint32_t t_5D96FC(uint32_t self) {
    st32(self, 0xf5d848u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x5D96FC, "HitEffect::deploy $_0 __func::~__func", t_5D96FC);

// HitEffect::deploy $_0: __func::~__func (deleting)
static void t_5D9738(uint32_t self) {
    st32(self, 0xf5d848u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D9738, "HitEffect::deploy $_0 __func::~__func (deleting)", t_5D9738);
