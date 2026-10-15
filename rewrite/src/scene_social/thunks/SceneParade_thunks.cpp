// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneParade lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneParade::init $_0: __func::~__func (deleting)
static void t_737C6A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x737C6A, "SceneParade::init $_0 __func::~__func (deleting)", t_737C6A);

// SceneParade::init $_0: __func::__clone()
static uint32_t t_737C74(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d55cu);
    }
    return v1;
}
RW_FUNCTION(0x737C74, "SceneParade::init $_0 __func::__clone()", t_737C74);

// SceneParade::init $_0: __func::__clone(__base*)
static void t_737C98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d55cu);
    }
}
RW_FUNCTION(0x737C98, "SceneParade::init $_0 __func::__clone(__base*)", t_737C98);

// SceneParade::init $_0: __func::destroy
static void t_737CB4(uint32_t self) {
}
RW_FUNCTION(0x737CB4, "SceneParade::init $_0 __func::destroy", t_737CB4);

// SceneParade::init $_0: __func::destroy_deallocate
static void t_737CB6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x737CB6, "SceneParade::init $_0 __func::destroy_deallocate", t_737CB6);

// SceneParade::init $_0: __func::target
static uint32_t t_737CC8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b230u ? self + 4 : 0; }
RW_FUNCTION(0x737CC8, "SceneParade::init $_0 __func::target", t_737CC8);

// SceneParade::init $_0: __func::target_type
static uint32_t t_737CE8() { return 0xfa6c90u; }
RW_FUNCTION(0x737CE8, "SceneParade::init $_0 __func::target_type", t_737CE8);
