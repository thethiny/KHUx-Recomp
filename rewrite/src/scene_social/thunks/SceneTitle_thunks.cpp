// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneTitle lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneTitle::init $_8: __func::~__func (deleting)
static void t_67EA74(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67EA74, "SceneTitle::init $_8 __func::~__func (deleting)", t_67EA74);

// SceneTitle::init $_8: __func::__clone()
static uint32_t t_67EA7C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf63ff4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x67EA7C, "SceneTitle::init $_8 __func::__clone()", t_67EA7C);

// SceneTitle::init $_8: __func::__clone(__base*)
static void t_67EAA8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63ff4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x67EAA8, "SceneTitle::init $_8 __func::__clone(__base*)", t_67EAA8);

// SceneTitle::init $_8: __func::destroy
static void t_67EAC8(uint32_t self) {
}
RW_FUNCTION(0x67EAC8, "SceneTitle::init $_8 __func::destroy", t_67EAC8);

// SceneTitle::init $_8: __func::destroy_deallocate
static void t_67EACA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67EACA, "SceneTitle::init $_8 __func::destroy_deallocate", t_67EACA);

// SceneTitle::init $_8: __func::target
static uint32_t t_67EAE0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6e550u ? self + 4 : 0; }
RW_FUNCTION(0x67EAE0, "SceneTitle::init $_8 __func::target", t_67EAE0);

// SceneTitle::init $_8: __func::target_type
static uint32_t t_67EB00() { return 0xf9bb08u; }
RW_FUNCTION(0x67EB00, "SceneTitle::init $_8 __func::target_type", t_67EB00);

// SceneTitle::init $_7: __func::~__func (deleting)
static void t_67EB14(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67EB14, "SceneTitle::init $_7 __func::~__func (deleting)", t_67EB14);

// SceneTitle::init $_7: __func::__clone()
static uint32_t t_67EB1C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64020u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x67EB1C, "SceneTitle::init $_7 __func::__clone()", t_67EB1C);

// SceneTitle::init $_7: __func::__clone(__base*)
static void t_67EB48(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64020u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x67EB48, "SceneTitle::init $_7 __func::__clone(__base*)", t_67EB48);

// SceneTitle::init $_7: __func::destroy
static void t_67EB68(uint32_t self) {
}
RW_FUNCTION(0x67EB68, "SceneTitle::init $_7 __func::destroy", t_67EB68);

// SceneTitle::init $_7: __func::destroy_deallocate
static void t_67EB6A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67EB6A, "SceneTitle::init $_7 __func::destroy_deallocate", t_67EB6A);

// SceneTitle::init $_7: __func::target
static uint32_t t_67ED7C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6e5e0u ? self + 4 : 0; }
RW_FUNCTION(0x67ED7C, "SceneTitle::init $_7 __func::target", t_67ED7C);

// SceneTitle::init $_7: __func::target_type
static uint32_t t_67ED9C() { return 0xf9bb1cu; }
RW_FUNCTION(0x67ED9C, "SceneTitle::init $_7 __func::target_type", t_67ED9C);

// SceneTitle::init $_7 UlvE1_: __func::~__func (deleting)
static void t_67EDB0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67EDB0, "SceneTitle::init $_7 UlvE1_ __func::~__func (deleting)", t_67EDB0);

// SceneTitle::init $_7 UlvE1_: __func::__clone()
static uint32_t t_67EDB8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6404cu);
    }
    return v1;
}
RW_FUNCTION(0x67EDB8, "SceneTitle::init $_7 UlvE1_ __func::__clone()", t_67EDB8);

// SceneTitle::init $_7 UlvE1_: __func::__clone(__base*)
static void t_67EDDC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6404cu);
    }
}
RW_FUNCTION(0x67EDDC, "SceneTitle::init $_7 UlvE1_ __func::__clone(__base*)", t_67EDDC);

// SceneTitle::init $_7 UlvE1_: __func::destroy
static void t_67EDF8(uint32_t self) {
}
RW_FUNCTION(0x67EDF8, "SceneTitle::init $_7 UlvE1_ __func::destroy", t_67EDF8);

// SceneTitle::init $_7 UlvE1_: __func::destroy_deallocate
static void t_67EDFA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67EDFA, "SceneTitle::init $_7 UlvE1_ __func::destroy_deallocate", t_67EDFA);

// SceneTitle::init $_7 UlvE1_: __func::target
static uint32_t t_67EEA0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6e680u ? self + 4 : 0; }
RW_FUNCTION(0x67EEA0, "SceneTitle::init $_7 UlvE1_ __func::target", t_67EEA0);

// SceneTitle::init $_7 UlvE1_: __func::target_type
static uint32_t t_67EEC0() { return 0xf9bb30u; }
RW_FUNCTION(0x67EEC0, "SceneTitle::init $_7 UlvE1_ __func::target_type", t_67EEC0);

// SceneTitle::init $_7 UlvE1_ UlvE_: __func::~__func (deleting)
static void t_67EED4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67EED4, "SceneTitle::init $_7 UlvE1_ UlvE_ __func::~__func (deleting)", t_67EED4);

// SceneTitle::init $_7 UlvE1_ UlvE_: __func::__clone()
static uint32_t t_67EEDC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64078u);
    }
    return v1;
}
RW_FUNCTION(0x67EEDC, "SceneTitle::init $_7 UlvE1_ UlvE_ __func::__clone()", t_67EEDC);
