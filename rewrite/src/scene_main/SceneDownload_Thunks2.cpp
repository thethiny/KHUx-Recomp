// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the SceneDownload lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneDownload::finalize $_3 UlvE_: __func::~__func (deleting)
static void t_6C888C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6C888C, "SceneDownload::finalize $_3 UlvE_ __func::~__func (deleting)", t_6C888C);

// SceneDownload::finalize $_3 UlvE_: __func::__clone()
static uint32_t t_6C8894(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf671d8u);
    }
    return v1;
}
RW_FUNCTION(0x6C8894, "SceneDownload::finalize $_3 UlvE_ __func::__clone()", t_6C8894);

// SceneDownload::finalize $_3 UlvE_: __func::__clone(__base*)
static void t_6C88B8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf671d8u);
    }
}
RW_FUNCTION(0x6C88B8, "SceneDownload::finalize $_3 UlvE_ __func::__clone(__base*)", t_6C88B8);

// SceneDownload::finalize $_3 UlvE_: __func::destroy
static void t_6C88D4(uint32_t self) {
}
RW_FUNCTION(0x6C88D4, "SceneDownload::finalize $_3 UlvE_ __func::destroy", t_6C88D4);

// SceneDownload::finalize $_3 UlvE_: __func::destroy_deallocate
static void t_6C88D6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6C88D6, "SceneDownload::finalize $_3 UlvE_ __func::destroy_deallocate", t_6C88D6);

// SceneDownload::finalize $_3 UlvE_: __func::target
static uint32_t t_6C89B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7cdb0u ? self + 4 : 0; }
RW_FUNCTION(0x6C89B8, "SceneDownload::finalize $_3 UlvE_ __func::target", t_6C89B8);

// SceneDownload::finalize $_3 UlvE_: __func::target_type
static uint32_t t_6C89D8() { return 0xf9f83cu; }
RW_FUNCTION(0x6C89D8, "SceneDownload::finalize $_3 UlvE_ __func::target_type", t_6C89D8);

// SceneDownload::finalize $_3 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6C89EC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6C89EC, "SceneDownload::finalize $_3 UlvE_ UlbE_ __func::~__func (deleting)", t_6C89EC);

// SceneDownload::finalize $_3 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6C89F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67204u);
    }
    return v1;
}
RW_FUNCTION(0x6C89F4, "SceneDownload::finalize $_3 UlvE_ UlbE_ __func::__clone()", t_6C89F4);

// SceneDownload::finalize $_3 UlvE_ UlbE_: __func::__clone(__base*)
static void t_6C8A18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67204u);
    }
}
RW_FUNCTION(0x6C8A18, "SceneDownload::finalize $_3 UlvE_ UlbE_ __func::__clone(__base*)", t_6C8A18);

// SceneDownload::finalize $_3 UlvE_ UlbE_: __func::destroy
static void t_6C8A34(uint32_t self) {
}
RW_FUNCTION(0x6C8A34, "SceneDownload::finalize $_3 UlvE_ UlbE_ __func::destroy", t_6C8A34);

// SceneDownload::finalize $_3 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6C8A36(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6C8A36, "SceneDownload::finalize $_3 UlvE_ UlbE_ __func::destroy_deallocate", t_6C8A36);
