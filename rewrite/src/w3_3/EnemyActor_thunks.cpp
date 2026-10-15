// The std::function __func thunks of the lambdas attributed to EnemyActor (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// EnemyActor::setupTouchRect $_6: __func::~__func (deleting)
static void t_74DCA8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x74DCA8, "EnemyActor::setupTouchRect $_6 __func::~__func (deleting)", t_74DCA8);

// EnemyActor::setupTouchRect $_6: __func::__clone()
static uint32_t t_74DCB0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ddb4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x74DCB0, "EnemyActor::setupTouchRect $_6 __func::__clone()", t_74DCB0);

// EnemyActor::setupTouchRect $_6: __func::__clone(__base*)
static void t_74DCDC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ddb4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x74DCDC, "EnemyActor::setupTouchRect $_6 __func::__clone(__base*)", t_74DCDC);

// EnemyActor::setupTouchRect $_6: __func::destroy
static void t_74DCFC(uint32_t self) {
}
RW_FUNCTION(0x74DCFC, "EnemyActor::setupTouchRect $_6 __func::destroy", t_74DCFC);

// EnemyActor::setupTouchRect $_6: __func::destroy_deallocate
static void t_74DCFE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x74DCFE, "EnemyActor::setupTouchRect $_6 __func::destroy_deallocate", t_74DCFE);

// EnemyActor::setupTouchRect $_6: __func::target
static uint32_t t_74DD60(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9d8b0u ? self + 4 : 0; }
RW_FUNCTION(0x74DD60, "EnemyActor::setupTouchRect $_6 __func::target", t_74DD60);

// EnemyActor::setupTouchRect $_6: __func::target_type
static uint32_t t_74DD80() { return 0xfa8210u; }
RW_FUNCTION(0x74DD80, "EnemyActor::setupTouchRect $_6 __func::target_type", t_74DD80);

// EnemyActor::setupTouchRect $_5: __func::~__func (deleting)
static void t_74DD94(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x74DD94, "EnemyActor::setupTouchRect $_5 __func::~__func (deleting)", t_74DD94);

// EnemyActor::setupTouchRect $_5: __func::__clone()
static uint32_t t_74DD9C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6dde0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x74DD9C, "EnemyActor::setupTouchRect $_5 __func::__clone()", t_74DD9C);

// EnemyActor::setupTouchRect $_5: __func::__clone(__base*)
static void t_74DDC8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6dde0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x74DDC8, "EnemyActor::setupTouchRect $_5 __func::__clone(__base*)", t_74DDC8);

// EnemyActor::setupTouchRect $_5: __func::destroy
static void t_74DDE8(uint32_t self) {
}
RW_FUNCTION(0x74DDE8, "EnemyActor::setupTouchRect $_5 __func::destroy", t_74DDE8);

// EnemyActor::setupTouchRect $_5: __func::destroy_deallocate
static void t_74DDEA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x74DDEA, "EnemyActor::setupTouchRect $_5 __func::destroy_deallocate", t_74DDEA);

// EnemyActor::setupTouchRect $_5: __func::target
static uint32_t t_74DEA8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9d960u ? self + 4 : 0; }
RW_FUNCTION(0x74DEA8, "EnemyActor::setupTouchRect $_5 __func::target", t_74DEA8);

// EnemyActor::setupTouchRect $_5: __func::target_type
static uint32_t t_74DEC8() { return 0xfa8224u; }
RW_FUNCTION(0x74DEC8, "EnemyActor::setupTouchRect $_5 __func::target_type", t_74DEC8);

// EnemyActor::setupTouchRect $_4: __func::~__func (deleting)
static void t_74DF5C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x74DF5C, "EnemyActor::setupTouchRect $_4 __func::~__func (deleting)", t_74DF5C);

// EnemyActor::setupTouchRect $_4: __func::__clone()
static uint32_t t_74DF64(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6de0cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x74DF64, "EnemyActor::setupTouchRect $_4 __func::__clone()", t_74DF64);

// EnemyActor::setupTouchRect $_4: __func::__clone(__base*)
static void t_74DF90(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6de0cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x74DF90, "EnemyActor::setupTouchRect $_4 __func::__clone(__base*)", t_74DF90);

// EnemyActor::setupTouchRect $_4: __func::destroy
static void t_74DFB0(uint32_t self) {
}
RW_FUNCTION(0x74DFB0, "EnemyActor::setupTouchRect $_4 __func::destroy", t_74DFB0);

// EnemyActor::setupTouchRect $_4: __func::destroy_deallocate
static void t_74DFB2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x74DFB2, "EnemyActor::setupTouchRect $_4 __func::destroy_deallocate", t_74DFB2);

// EnemyActor::setupTouchRect $_4: __func::target
static uint32_t t_74E0A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9da10u ? self + 4 : 0; }
RW_FUNCTION(0x74E0A0, "EnemyActor::setupTouchRect $_4 __func::target", t_74E0A0);

// EnemyActor::setupTouchRect $_4: __func::target_type
static uint32_t t_74E0C0() { return 0xfa8238u; }
RW_FUNCTION(0x74E0C0, "EnemyActor::setupTouchRect $_4 __func::target_type", t_74E0C0);

// EnemyActor::setupTouchRect $_3: __func::~__func (deleting)
static void t_74E0D4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x74E0D4, "EnemyActor::setupTouchRect $_3 __func::~__func (deleting)", t_74E0D4);
