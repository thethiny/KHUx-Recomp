// MissBoard: the std::function __func thunks of its lambdas (destructors, clone - copy the captures into a new /
// given block -, destroy, target / target_type typeinfo checks), generated from the lifted code by a scratch
// tool (gen_thunks.py, symbolic walk of each thunk) and checked against it. The lambda bodies are written by hand.
#include "../scene/SceneCommon.hpp"

namespace {
using namespace scene;
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }

// MissBoard::init $_5: __func::~__func (deleting)
static void init_L5_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D4278, "MissBoard::init $_5 __func::~__func (deleting)", init_L5_deletingDtor);

// MissBoard::init $_5: __func::__clone()
static uint32_t init_L5_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d3b4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5D4280, "MissBoard::init $_5 __func::__clone()", init_L5_clone);

// MissBoard::init $_5: __func::__clone(__base*)
static void init_L5_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d3b4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5D42AC, "MissBoard::init $_5 __func::__clone(__base*)", init_L5_cloneInto);

// MissBoard::init $_5: __func::destroy
static void init_L5_destroy(uint32_t self) {
}
RW_FUNCTION(0x5D42CC, "MissBoard::init $_5 __func::destroy", init_L5_destroy);

// MissBoard::init $_5: __func::destroy_deallocate
static void init_L5_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D42CE, "MissBoard::init $_5 __func::destroy_deallocate", init_L5_destroyDeallocate);

// MissBoard::init $_5: __func::target
static uint32_t init_L5_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4b4a0u ? self + 4 : 0; }
RW_FUNCTION(0x5D42D8, "MissBoard::init $_5 __func::target", init_L5_target);

// MissBoard::init $_5: __func::target_type
static uint32_t init_L5_targetType() { return 0xf90b74u; }
RW_FUNCTION(0x5D42F8, "MissBoard::init $_5 __func::target_type", init_L5_targetType);

// MissBoard::init $_4: __func::~__func (deleting)
static void init_L4_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D430C, "MissBoard::init $_4 __func::~__func (deleting)", init_L4_deletingDtor);

// MissBoard::init $_4: __func::__clone()
static uint32_t init_L4_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d3e0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5D4314, "MissBoard::init $_4 __func::__clone()", init_L4_clone);

// MissBoard::init $_4: __func::__clone(__base*)
static void init_L4_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d3e0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5D4340, "MissBoard::init $_4 __func::__clone(__base*)", init_L4_cloneInto);

// MissBoard::init $_4: __func::destroy
static void init_L4_destroy(uint32_t self) {
}
RW_FUNCTION(0x5D4360, "MissBoard::init $_4 __func::destroy", init_L4_destroy);

// MissBoard::init $_4: __func::destroy_deallocate
static void init_L4_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D4362, "MissBoard::init $_4 __func::destroy_deallocate", init_L4_destroyDeallocate);

// MissBoard::init $_4: __func::target
static uint32_t init_L4_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4b570u ? self + 4 : 0; }
RW_FUNCTION(0x5D43A8, "MissBoard::init $_4 __func::target", init_L4_target);

// MissBoard::init $_4: __func::target_type
static uint32_t init_L4_targetType() { return 0xf90b88u; }
RW_FUNCTION(0x5D43C8, "MissBoard::init $_4 __func::target_type", init_L4_targetType);

}  // namespace
