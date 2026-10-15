// The std::function __func thunks of the lambdas attributed to BookAnimation (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// BookAnimation::leftAnimation $_1: __func::~__func
static uint32_t t_50B18C(uint32_t self) {
    st32(self, 0xf56158u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x50B18C, "BookAnimation::leftAnimation $_1 __func::~__func", t_50B18C);

// BookAnimation::leftAnimation $_1: __func::~__func (deleting)
static void t_50B1C8(uint32_t self) {
    st32(self, 0xf56158u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50B1C8, "BookAnimation::leftAnimation $_1 __func::~__func (deleting)", t_50B1C8);

// BookAnimation::leftAnimation $_1: __func::__clone()
static uint32_t t_50B208(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf56158u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x50B208, "BookAnimation::leftAnimation $_1 __func::__clone()", t_50B208);

// BookAnimation::leftAnimation $_1: __func::__clone(__base*)
static void t_50B264(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56158u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x50B264, "BookAnimation::leftAnimation $_1 __func::__clone(__base*)", t_50B264);

// BookAnimation::leftAnimation $_1: __func::destroy
static void t_50B2A4(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x50B2A4, "BookAnimation::leftAnimation $_1 __func::destroy", t_50B2A4);

// BookAnimation::leftAnimation $_1: __func::destroy_deallocate
static void t_50B2C4(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50B2C4, "BookAnimation::leftAnimation $_1 __func::destroy_deallocate", t_50B2C4);

// BookAnimation::leftAnimation $_1: __func::target
static uint32_t t_50B324(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe21fc0u ? self + 8 : 0; }
RW_FUNCTION(0x50B324, "BookAnimation::leftAnimation $_1 __func::target", t_50B324);

// BookAnimation::leftAnimation $_1: __func::target_type
static uint32_t t_50B348() { return 0xf832c0u; }
RW_FUNCTION(0x50B348, "BookAnimation::leftAnimation $_1 __func::target_type", t_50B348);

// BookAnimation::rightAnimation $_0: __func::~__func
static uint32_t t_50B35C(uint32_t self) {
    st32(self, 0xf56184u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x50B35C, "BookAnimation::rightAnimation $_0 __func::~__func", t_50B35C);

// BookAnimation::rightAnimation $_0: __func::~__func (deleting)
static void t_50B398(uint32_t self) {
    st32(self, 0xf56184u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50B398, "BookAnimation::rightAnimation $_0 __func::~__func (deleting)", t_50B398);

// BookAnimation::rightAnimation $_0: __func::__clone()
static uint32_t t_50B3D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf56184u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x50B3D8, "BookAnimation::rightAnimation $_0 __func::__clone()", t_50B3D8);
