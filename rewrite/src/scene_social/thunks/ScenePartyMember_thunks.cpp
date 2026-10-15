// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ScenePartyMember lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ScenePartyMember::backScene $_6: __func::~__func (deleting)
static void t_6F1B28(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F1B28, "ScenePartyMember::backScene $_6 __func::~__func (deleting)", t_6F1B28);

// ScenePartyMember::backScene $_6: __func::__clone()
static uint32_t t_6F1B30(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69b44u);
    }
    return v1;
}
RW_FUNCTION(0x6F1B30, "ScenePartyMember::backScene $_6 __func::__clone()", t_6F1B30);

// ScenePartyMember::backScene $_6: __func::__clone(__base*)
static void t_6F1B54(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69b44u);
    }
}
RW_FUNCTION(0x6F1B54, "ScenePartyMember::backScene $_6 __func::__clone(__base*)", t_6F1B54);

// ScenePartyMember::backScene $_6: __func::destroy
static void t_6F1B70(uint32_t self) {
}
RW_FUNCTION(0x6F1B70, "ScenePartyMember::backScene $_6 __func::destroy", t_6F1B70);

// ScenePartyMember::backScene $_6: __func::destroy_deallocate
static void t_6F1B72(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F1B72, "ScenePartyMember::backScene $_6 __func::destroy_deallocate", t_6F1B72);

// ScenePartyMember::backScene $_6: __func::target
static uint32_t t_6F1C9C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe89550u ? self + 4 : 0; }
RW_FUNCTION(0x6F1C9C, "ScenePartyMember::backScene $_6 __func::target", t_6F1C9C);

// ScenePartyMember::backScene $_6: __func::target_type
static uint32_t t_6F1CBC() { return 0xfa224cu; }
RW_FUNCTION(0x6F1CBC, "ScenePartyMember::backScene $_6 __func::target_type", t_6F1CBC);

// ScenePartyMember::initMemberLayer $_4: __func::~__func (deleting)
static void t_6F1CD0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F1CD0, "ScenePartyMember::initMemberLayer $_4 __func::~__func (deleting)", t_6F1CD0);

// ScenePartyMember::initMemberLayer $_4: __func::__clone()
static uint32_t t_6F1CD8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69b70u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F1CD8, "ScenePartyMember::initMemberLayer $_4 __func::__clone()", t_6F1CD8);

// ScenePartyMember::initMemberLayer $_4: __func::__clone(__base*)
static void t_6F1D04(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69b70u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F1D04, "ScenePartyMember::initMemberLayer $_4 __func::__clone(__base*)", t_6F1D04);

// ScenePartyMember::initMemberLayer $_4: __func::destroy
static void t_6F1D24(uint32_t self) {
}
RW_FUNCTION(0x6F1D24, "ScenePartyMember::initMemberLayer $_4 __func::destroy", t_6F1D24);

// ScenePartyMember::initMemberLayer $_4: __func::destroy_deallocate
static void t_6F1D26(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F1D26, "ScenePartyMember::initMemberLayer $_4 __func::destroy_deallocate", t_6F1D26);

// ScenePartyMember::initMemberLayer $_4: __func::target
static uint32_t t_6F1EC0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe895f0u ? self + 4 : 0; }
RW_FUNCTION(0x6F1EC0, "ScenePartyMember::initMemberLayer $_4 __func::target", t_6F1EC0);

// ScenePartyMember::initMemberLayer $_4: __func::target_type
static uint32_t t_6F1EE0() { return 0xfa2260u; }
RW_FUNCTION(0x6F1EE0, "ScenePartyMember::initMemberLayer $_4 __func::target_type", t_6F1EE0);

// ScenePartyMember::initMemberLayer $_4 UlbE_: __func::~__func (deleting)
static void t_6F1EF4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F1EF4, "ScenePartyMember::initMemberLayer $_4 UlbE_ __func::~__func (deleting)", t_6F1EF4);

// ScenePartyMember::initMemberLayer $_4 UlbE_: __func::__clone()
static uint32_t t_6F1EFC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69b9cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F1EFC, "ScenePartyMember::initMemberLayer $_4 UlbE_ __func::__clone()", t_6F1EFC);

// ScenePartyMember::initMemberLayer $_4 UlbE_: __func::__clone(__base*)
static void t_6F1F28(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69b9cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F1F28, "ScenePartyMember::initMemberLayer $_4 UlbE_ __func::__clone(__base*)", t_6F1F28);

// ScenePartyMember::initMemberLayer $_4 UlbE_: __func::destroy
static void t_6F1F48(uint32_t self) {
}
RW_FUNCTION(0x6F1F48, "ScenePartyMember::initMemberLayer $_4 UlbE_ __func::destroy", t_6F1F48);

// ScenePartyMember::initMemberLayer $_4 UlbE_: __func::destroy_deallocate
static void t_6F1F4A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F1F4A, "ScenePartyMember::initMemberLayer $_4 UlbE_ __func::destroy_deallocate", t_6F1F4A);
