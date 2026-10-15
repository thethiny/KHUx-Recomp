// BaseEndlessrollView: the std::function __func thunks of its lambdas (destructors, clone - copy the captures into a new /
// given block -, destroy, target / target_type typeinfo checks), generated from the lifted code by a scratch
// tool (gen_thunks.py, symbolic walk of each thunk) and checked against it. The lambda bodies are written by hand.
#include "../scene/SceneCommon.hpp"

namespace {
using namespace scene;
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }

// BaseEndlessrollView::init $_11: __func::~__func (deleting)
static void init_L11_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x517F34, "BaseEndlessrollView::init $_11 __func::~__func (deleting)", init_L11_deletingDtor);

// BaseEndlessrollView::init $_11: __func::__clone()
static uint32_t init_L11_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf56730u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x517F3C, "BaseEndlessrollView::init $_11 __func::__clone()", init_L11_clone);

// BaseEndlessrollView::init $_11: __func::__clone(__base*)
static void init_L11_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56730u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x517F68, "BaseEndlessrollView::init $_11 __func::__clone(__base*)", init_L11_cloneInto);

// BaseEndlessrollView::init $_11: __func::destroy
static void init_L11_destroy(uint32_t self) {
}
RW_FUNCTION(0x517F88, "BaseEndlessrollView::init $_11 __func::destroy", init_L11_destroy);

// BaseEndlessrollView::init $_11: __func::destroy_deallocate
static void init_L11_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x517F8A, "BaseEndlessrollView::init $_11 __func::destroy_deallocate", init_L11_destroyDeallocate);

// BaseEndlessrollView::init $_11: __func::target
static uint32_t init_L11_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe24000u ? self + 4 : 0; }
RW_FUNCTION(0x518130, "BaseEndlessrollView::init $_11 __func::target", init_L11_target);

// BaseEndlessrollView::init $_11: __func::target_type
static uint32_t init_L11_targetType() { return 0xf84668u; }
RW_FUNCTION(0x518150, "BaseEndlessrollView::init $_11 __func::target_type", init_L11_targetType);

// BaseEndlessrollView::init $_10: __func::~__func (deleting)
static void init_L10_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x518164, "BaseEndlessrollView::init $_10 __func::~__func (deleting)", init_L10_deletingDtor);

// BaseEndlessrollView::init $_10: __func::__clone()
static uint32_t init_L10_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5675cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x51816C, "BaseEndlessrollView::init $_10 __func::__clone()", init_L10_clone);

// BaseEndlessrollView::init $_10: __func::__clone(__base*)
static void init_L10_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5675cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x518198, "BaseEndlessrollView::init $_10 __func::__clone(__base*)", init_L10_cloneInto);

// BaseEndlessrollView::init $_10: __func::destroy
static void init_L10_destroy(uint32_t self) {
}
RW_FUNCTION(0x5181B8, "BaseEndlessrollView::init $_10 __func::destroy", init_L10_destroy);

// BaseEndlessrollView::init $_10: __func::destroy_deallocate
static void init_L10_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5181BA, "BaseEndlessrollView::init $_10 __func::destroy_deallocate", init_L10_destroyDeallocate);

// BaseEndlessrollView::init $_10: __func::target
static uint32_t init_L10_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe240b0u ? self + 4 : 0; }
RW_FUNCTION(0x518360, "BaseEndlessrollView::init $_10 __func::target", init_L10_target);

// BaseEndlessrollView::init $_10: __func::target_type
static uint32_t init_L10_targetType() { return 0xf8467cu; }
RW_FUNCTION(0x518380, "BaseEndlessrollView::init $_10 __func::target_type", init_L10_targetType);

// BaseEndlessrollView::init $_9: __func::~__func (deleting)
static void init_L9_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x518394, "BaseEndlessrollView::init $_9 __func::~__func (deleting)", init_L9_deletingDtor);

// BaseEndlessrollView::init $_9: __func::__clone()
static uint32_t init_L9_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf56788u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x51839C, "BaseEndlessrollView::init $_9 __func::__clone()", init_L9_clone);

// BaseEndlessrollView::init $_9: __func::__clone(__base*)
static void init_L9_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56788u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5183C8, "BaseEndlessrollView::init $_9 __func::__clone(__base*)", init_L9_cloneInto);

// BaseEndlessrollView::init $_9: __func::destroy
static void init_L9_destroy(uint32_t self) {
}
RW_FUNCTION(0x5183E8, "BaseEndlessrollView::init $_9 __func::destroy", init_L9_destroy);

// BaseEndlessrollView::init $_9: __func::destroy_deallocate
static void init_L9_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5183EA, "BaseEndlessrollView::init $_9 __func::destroy_deallocate", init_L9_destroyDeallocate);

// BaseEndlessrollView::init $_9: __func::target
static uint32_t init_L9_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe24160u ? self + 4 : 0; }
RW_FUNCTION(0x518620, "BaseEndlessrollView::init $_9 __func::target", init_L9_target);

// BaseEndlessrollView::init $_9: __func::target_type
static uint32_t init_L9_targetType() { return 0xf84690u; }
RW_FUNCTION(0x518640, "BaseEndlessrollView::init $_9 __func::target_type", init_L9_targetType);

// BaseEndlessrollView::init $_8: __func::~__func (deleting)
static void init_L8_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x518654, "BaseEndlessrollView::init $_8 __func::~__func (deleting)", init_L8_deletingDtor);

// BaseEndlessrollView::init $_8: __func::__clone()
static uint32_t init_L8_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf567b4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x51865C, "BaseEndlessrollView::init $_8 __func::__clone()", init_L8_clone);

// BaseEndlessrollView::init $_8: __func::__clone(__base*)
static void init_L8_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf567b4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x518688, "BaseEndlessrollView::init $_8 __func::__clone(__base*)", init_L8_cloneInto);

// BaseEndlessrollView::init $_8: __func::destroy
static void init_L8_destroy(uint32_t self) {
}
RW_FUNCTION(0x5186A8, "BaseEndlessrollView::init $_8 __func::destroy", init_L8_destroy);

// BaseEndlessrollView::init $_8: __func::destroy_deallocate
static void init_L8_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5186AA, "BaseEndlessrollView::init $_8 __func::destroy_deallocate", init_L8_destroyDeallocate);

// BaseEndlessrollView::init $_8: __func::target
static uint32_t init_L8_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe24210u ? self + 4 : 0; }
RW_FUNCTION(0x518794, "BaseEndlessrollView::init $_8 __func::target", init_L8_target);

// BaseEndlessrollView::init $_8: __func::target_type
static uint32_t init_L8_targetType() { return 0xf846a4u; }
RW_FUNCTION(0x5187B4, "BaseEndlessrollView::init $_8 __func::target_type", init_L8_targetType);

// BaseEndlessrollView::init $_7: __func::~__func (deleting)
static void init_L7_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5187C8, "BaseEndlessrollView::init $_7 __func::~__func (deleting)", init_L7_deletingDtor);

// BaseEndlessrollView::init $_7: __func::__clone()
static uint32_t init_L7_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf567e0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5187D0, "BaseEndlessrollView::init $_7 __func::__clone()", init_L7_clone);

// BaseEndlessrollView::init $_7: __func::__clone(__base*)
static void init_L7_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf567e0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5187FC, "BaseEndlessrollView::init $_7 __func::__clone(__base*)", init_L7_cloneInto);

// BaseEndlessrollView::init $_7: __func::destroy
static void init_L7_destroy(uint32_t self) {
}
RW_FUNCTION(0x51881C, "BaseEndlessrollView::init $_7 __func::destroy", init_L7_destroy);

// BaseEndlessrollView::init $_7: __func::destroy_deallocate
static void init_L7_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51881E, "BaseEndlessrollView::init $_7 __func::destroy_deallocate", init_L7_destroyDeallocate);

// BaseEndlessrollView::init $_7: __func::target
static uint32_t init_L7_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe242c0u ? self + 4 : 0; }
RW_FUNCTION(0x518834, "BaseEndlessrollView::init $_7 __func::target", init_L7_target);

// BaseEndlessrollView::init $_7: __func::target_type
static uint32_t init_L7_targetType() { return 0xf846b8u; }
RW_FUNCTION(0x518854, "BaseEndlessrollView::init $_7 __func::target_type", init_L7_targetType);

// BaseEndlessrollView::init $_6: __func::~__func (deleting)
static void init_L6_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x518868, "BaseEndlessrollView::init $_6 __func::~__func (deleting)", init_L6_deletingDtor);

// BaseEndlessrollView::init $_6: __func::__clone()
static uint32_t init_L6_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5680cu);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x518870, "BaseEndlessrollView::init $_6 __func::__clone()", init_L6_clone);

// BaseEndlessrollView::init $_6: __func::__clone(__base*)
static void init_L6_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5680cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5188A0, "BaseEndlessrollView::init $_6 __func::__clone(__base*)", init_L6_cloneInto);

// BaseEndlessrollView::init $_6: __func::destroy
static void init_L6_destroy(uint32_t self) {
}
RW_FUNCTION(0x5188C4, "BaseEndlessrollView::init $_6 __func::destroy", init_L6_destroy);

// BaseEndlessrollView::init $_6: __func::destroy_deallocate
static void init_L6_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5188C6, "BaseEndlessrollView::init $_6 __func::destroy_deallocate", init_L6_destroyDeallocate);

// BaseEndlessrollView::init $_6: __func::target
static uint32_t init_L6_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe24370u ? self + 4 : 0; }
RW_FUNCTION(0x518984, "BaseEndlessrollView::init $_6 __func::target", init_L6_target);

// BaseEndlessrollView::init $_6: __func::target_type
static uint32_t init_L6_targetType() { return 0xf846ccu; }
RW_FUNCTION(0x5189A4, "BaseEndlessrollView::init $_6 __func::target_type", init_L6_targetType);

// BaseEndlessrollView::init $_5: __func::~__func (deleting)
static void init_L5_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5189B8, "BaseEndlessrollView::init $_5 __func::~__func (deleting)", init_L5_deletingDtor);

// BaseEndlessrollView::init $_5: __func::__clone()
static uint32_t init_L5_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf56838u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5189C0, "BaseEndlessrollView::init $_5 __func::__clone()", init_L5_clone);

// BaseEndlessrollView::init $_5: __func::__clone(__base*)
static void init_L5_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56838u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5189EC, "BaseEndlessrollView::init $_5 __func::__clone(__base*)", init_L5_cloneInto);

// BaseEndlessrollView::init $_5: __func::destroy
static void init_L5_destroy(uint32_t self) {
}
RW_FUNCTION(0x518A0C, "BaseEndlessrollView::init $_5 __func::destroy", init_L5_destroy);

// BaseEndlessrollView::init $_5: __func::destroy_deallocate
static void init_L5_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x518A0E, "BaseEndlessrollView::init $_5 __func::destroy_deallocate", init_L5_destroyDeallocate);

}  // namespace
