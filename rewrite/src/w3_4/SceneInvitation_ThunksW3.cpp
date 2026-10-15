// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the SceneInvitation lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneInvitation::init $_2 UlbE_: __func::target
static uint32_t t_716B58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe914c0u ? self + 4 : 0; }
RW_FUNCTION(0x716B58, "SceneInvitation::init $_2 UlbE_ __func::target", t_716B58);

// SceneInvitation::init $_2 UlbE_: __func::target_type
static uint32_t t_716B78() { return 0xfa4e50u; }
RW_FUNCTION(0x716B78, "SceneInvitation::init $_2 UlbE_ __func::target_type", t_716B78);

// SceneInvitation::init $_2 UlbE_ UlbE_: __func::~__func (deleting)
static void t_716B8C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x716B8C, "SceneInvitation::init $_2 UlbE_ UlbE_ __func::~__func (deleting)", t_716B8C);

// SceneInvitation::init $_2 UlbE_ UlbE_: __func::__clone()
static uint32_t t_716B94(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6b66cu);
    }
    return v1;
}
RW_FUNCTION(0x716B94, "SceneInvitation::init $_2 UlbE_ UlbE_ __func::__clone()", t_716B94);

// SceneInvitation::init $_2 UlbE_ UlbE_: __func::__clone(__base*)
static void t_716BB8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b66cu);
    }
}
RW_FUNCTION(0x716BB8, "SceneInvitation::init $_2 UlbE_ UlbE_ __func::__clone(__base*)", t_716BB8);

// SceneInvitation::init $_2 UlbE_ UlbE_: __func::destroy
static void t_716BD4(uint32_t self) {
}
RW_FUNCTION(0x716BD4, "SceneInvitation::init $_2 UlbE_ UlbE_ __func::destroy", t_716BD4);

// SceneInvitation::init $_2 UlbE_ UlbE_: __func::destroy_deallocate
static void t_716BD6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x716BD6, "SceneInvitation::init $_2 UlbE_ UlbE_ __func::destroy_deallocate", t_716BD6);

// SceneInvitation::init $_2 UlbE_ UlbE_: __func::target
static uint32_t t_716BE0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe915c0u ? self + 4 : 0; }
RW_FUNCTION(0x716BE0, "SceneInvitation::init $_2 UlbE_ UlbE_ __func::target", t_716BE0);

// SceneInvitation::init $_2 UlbE_ UlbE_: __func::target_type
static uint32_t t_716C00() { return 0xfa4e64u; }
RW_FUNCTION(0x716C00, "SceneInvitation::init $_2 UlbE_ UlbE_ __func::target_type", t_716C00);

// SceneInvitation::init $_1: __func::~__func (deleting)
static void t_716C14(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x716C14, "SceneInvitation::init $_1 __func::~__func (deleting)", t_716C14);

// SceneInvitation::init $_1: __func::__clone()
static uint32_t t_716C1C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6b698u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x716C1C, "SceneInvitation::init $_1 __func::__clone()", t_716C1C);

// SceneInvitation::init $_1: __func::__clone(__base*)
static void t_716C48(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b698u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x716C48, "SceneInvitation::init $_1 __func::__clone(__base*)", t_716C48);

// SceneInvitation::init $_1: __func::destroy
static void t_716C68(uint32_t self) {
}
RW_FUNCTION(0x716C68, "SceneInvitation::init $_1 __func::destroy", t_716C68);

// SceneInvitation::init $_1: __func::destroy_deallocate
static void t_716C6A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x716C6A, "SceneInvitation::init $_1 __func::destroy_deallocate", t_716C6A);

// SceneInvitation::init $_1: __func::target
static uint32_t t_716C7C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe91690u ? self + 4 : 0; }
RW_FUNCTION(0x716C7C, "SceneInvitation::init $_1 __func::target", t_716C7C);

// SceneInvitation::init $_1: __func::target_type
static uint32_t t_716C9C() { return 0xfa4e78u; }
RW_FUNCTION(0x716C9C, "SceneInvitation::init $_1 __func::target_type", t_716C9C);

// SceneInvitation::init $_0: __func::~__func (deleting)
static void t_716CB0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x716CB0, "SceneInvitation::init $_0 __func::~__func (deleting)", t_716CB0);

// SceneInvitation::init $_0: __func::__clone()
static uint32_t t_716CB8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6b6c4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x716CB8, "SceneInvitation::init $_0 __func::__clone()", t_716CB8);

// SceneInvitation::init $_0: __func::__clone(__base*)
static void t_716CE4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b6c4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x716CE4, "SceneInvitation::init $_0 __func::__clone(__base*)", t_716CE4);

// SceneInvitation::init $_0: __func::destroy
static void t_716D04(uint32_t self) {
}
RW_FUNCTION(0x716D04, "SceneInvitation::init $_0 __func::destroy", t_716D04);

// SceneInvitation::init $_0: __func::destroy_deallocate
static void t_716D06(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x716D06, "SceneInvitation::init $_0 __func::destroy_deallocate", t_716D06);

// SceneInvitation::init $_0: __func::target
static uint32_t t_716D18(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe91710u ? self + 4 : 0; }
RW_FUNCTION(0x716D18, "SceneInvitation::init $_0 __func::target", t_716D18);

// SceneInvitation::init $_0: __func::target_type
static uint32_t t_716D38() { return 0xfa4e8cu; }
RW_FUNCTION(0x716D38, "SceneInvitation::init $_0 __func::target_type", t_716D38);
