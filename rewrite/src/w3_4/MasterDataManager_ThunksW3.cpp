// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the MasterDataManager lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// MasterDataManager $_0: __func::~__func
static uint32_t t_76EFA4(uint32_t self) {
    return self;
}
RW_FUNCTION(0x76EFA4, "MasterDataManager $_0 __func::~__func", t_76EFA4);

// MasterDataManager $_0: __func::~__func (deleting)
static void t_76EFA6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76EFA6, "MasterDataManager $_0 __func::~__func (deleting)", t_76EFA6);

// MasterDataManager $_0: __func::__clone()
static uint32_t t_76EFB0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ebf4u);
    }
    return v1;
}
RW_FUNCTION(0x76EFB0, "MasterDataManager $_0 __func::__clone()", t_76EFB0);

// MasterDataManager $_0: __func::__clone(__base*)
static void t_76EFD4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ebf4u);
    }
}
RW_FUNCTION(0x76EFD4, "MasterDataManager $_0 __func::__clone(__base*)", t_76EFD4);

// MasterDataManager $_0: __func::destroy
static void t_76EFF0(uint32_t self) {
}
RW_FUNCTION(0x76EFF0, "MasterDataManager $_0 __func::destroy", t_76EFF0);

// MasterDataManager $_0: __func::destroy_deallocate
static void t_76EFF2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76EFF2, "MasterDataManager $_0 __func::destroy_deallocate", t_76EFF2);

// MasterDataManager $_0: __func::target
static uint32_t t_76F000(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea1e00u ? self + 4 : 0; }
RW_FUNCTION(0x76F000, "MasterDataManager $_0 __func::target", t_76F000);

// MasterDataManager $_0: __func::target_type
static uint32_t t_76F020() { return 0xfa98f0u; }
RW_FUNCTION(0x76F020, "MasterDataManager $_0 __func::target_type", t_76F020);
