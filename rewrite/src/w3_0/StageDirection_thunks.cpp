// StageDirection: the std::function __func thunks of its lambdas (destructors, clone - copy the captures into a new /
// given block -, destroy, target / target_type typeinfo checks), generated from the lifted code by a scratch
// tool (gen_thunks.py, symbolic walk of each thunk) and checked against it. The lambda bodies are written by hand.
#include "../scene/SceneCommon.hpp"

namespace {
using namespace scene;
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }

// StageDirection::all1turn_start $_16: __func::~__func (deleting)
static void all1turn_start_L16_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x769FA6, "StageDirection::all1turn_start $_16 __func::~__func (deleting)", all1turn_start_L16_deletingDtor);

// StageDirection::all1turn_start $_16: __func::__clone()
static uint32_t all1turn_start_L16_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e8b0u);
    }
    return v1;
}
RW_FUNCTION(0x769FB0, "StageDirection::all1turn_start $_16 __func::__clone()", all1turn_start_L16_clone);

// StageDirection::all1turn_start $_16: __func::__clone(__base*)
static void all1turn_start_L16_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e8b0u);
    }
}
RW_FUNCTION(0x769FD4, "StageDirection::all1turn_start $_16 __func::__clone(__base*)", all1turn_start_L16_cloneInto);

// StageDirection::all1turn_start $_16: __func::destroy
static void all1turn_start_L16_destroy(uint32_t self) {
}
RW_FUNCTION(0x769FF0, "StageDirection::all1turn_start $_16 __func::destroy", all1turn_start_L16_destroy);

// StageDirection::all1turn_start $_16: __func::destroy_deallocate
static void all1turn_start_L16_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x769FF2, "StageDirection::all1turn_start $_16 __func::destroy_deallocate", all1turn_start_L16_destroyDeallocate);

// StageDirection::all1turn_start $_16: __func::target
static uint32_t all1turn_start_L16_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea0d90u ? self + 4 : 0; }
RW_FUNCTION(0x76A080, "StageDirection::all1turn_start $_16 __func::target", all1turn_start_L16_target);

// StageDirection::all1turn_start $_16: __func::target_type
static uint32_t all1turn_start_L16_targetType() { return 0xfa9664u; }
RW_FUNCTION(0x76A0A0, "StageDirection::all1turn_start $_16 __func::target_type", all1turn_start_L16_targetType);

// StageDirection::cocos_lose_colosseum $_15: __func::~__func (deleting)
static void cocos_lose_colosseum_L15_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A0B4, "StageDirection::cocos_lose_colosseum $_15 __func::~__func (deleting)", cocos_lose_colosseum_L15_deletingDtor);

// StageDirection::cocos_lose_colosseum $_15: __func::__clone()
static uint32_t cocos_lose_colosseum_L15_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e8dcu);
    }
    return v1;
}
RW_FUNCTION(0x76A0BC, "StageDirection::cocos_lose_colosseum $_15 __func::__clone()", cocos_lose_colosseum_L15_clone);

// StageDirection::cocos_lose_colosseum $_15: __func::__clone(__base*)
static void cocos_lose_colosseum_L15_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e8dcu);
    }
}
RW_FUNCTION(0x76A0E0, "StageDirection::cocos_lose_colosseum $_15 __func::__clone(__base*)", cocos_lose_colosseum_L15_cloneInto);

// StageDirection::cocos_lose_colosseum $_15: __func::destroy
static void cocos_lose_colosseum_L15_destroy(uint32_t self) {
}
RW_FUNCTION(0x76A0FC, "StageDirection::cocos_lose_colosseum $_15 __func::destroy", cocos_lose_colosseum_L15_destroy);

// StageDirection::cocos_lose_colosseum $_15: __func::destroy_deallocate
static void cocos_lose_colosseum_L15_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A0FE, "StageDirection::cocos_lose_colosseum $_15 __func::destroy_deallocate", cocos_lose_colosseum_L15_destroyDeallocate);

// StageDirection::cocos_lose_colosseum $_15: __func::target
static uint32_t cocos_lose_colosseum_L15_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea0e50u ? self + 4 : 0; }
RW_FUNCTION(0x76A108, "StageDirection::cocos_lose_colosseum $_15 __func::target", cocos_lose_colosseum_L15_target);

// StageDirection::cocos_lose_colosseum $_15: __func::target_type
static uint32_t cocos_lose_colosseum_L15_targetType() { return 0xfa9678u; }
RW_FUNCTION(0x76A128, "StageDirection::cocos_lose_colosseum $_15 __func::target_type", cocos_lose_colosseum_L15_targetType);

// StageDirection::cocos_lose_colosseum $_14: __func::~__func (deleting)
static void cocos_lose_colosseum_L14_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A13C, "StageDirection::cocos_lose_colosseum $_14 __func::~__func (deleting)", cocos_lose_colosseum_L14_deletingDtor);

// StageDirection::cocos_lose_colosseum $_14: __func::__clone()
static uint32_t cocos_lose_colosseum_L14_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e908u);
    }
    return v1;
}
RW_FUNCTION(0x76A144, "StageDirection::cocos_lose_colosseum $_14 __func::__clone()", cocos_lose_colosseum_L14_clone);

// StageDirection::cocos_lose_colosseum $_14: __func::__clone(__base*)
static void cocos_lose_colosseum_L14_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e908u);
    }
}
RW_FUNCTION(0x76A168, "StageDirection::cocos_lose_colosseum $_14 __func::__clone(__base*)", cocos_lose_colosseum_L14_cloneInto);

// StageDirection::cocos_lose_colosseum $_14: __func::destroy
static void cocos_lose_colosseum_L14_destroy(uint32_t self) {
}
RW_FUNCTION(0x76A184, "StageDirection::cocos_lose_colosseum $_14 __func::destroy", cocos_lose_colosseum_L14_destroy);

// StageDirection::cocos_lose_colosseum $_14: __func::destroy_deallocate
static void cocos_lose_colosseum_L14_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A186, "StageDirection::cocos_lose_colosseum $_14 __func::destroy_deallocate", cocos_lose_colosseum_L14_destroyDeallocate);

// StageDirection::cocos_lose_colosseum $_14: __func::target
static uint32_t cocos_lose_colosseum_L14_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea0f20u ? self + 4 : 0; }
RW_FUNCTION(0x76A214, "StageDirection::cocos_lose_colosseum $_14 __func::target", cocos_lose_colosseum_L14_target);

// StageDirection::cocos_lose_colosseum $_14: __func::target_type
static uint32_t cocos_lose_colosseum_L14_targetType() { return 0xfa968cu; }
RW_FUNCTION(0x76A234, "StageDirection::cocos_lose_colosseum $_14 __func::target_type", cocos_lose_colosseum_L14_targetType);

// StageDirection::cocos_clear_colosseum $_13: __func::~__func (deleting)
static void cocos_clear_colosseum_L13_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A248, "StageDirection::cocos_clear_colosseum $_13 __func::~__func (deleting)", cocos_clear_colosseum_L13_deletingDtor);

// StageDirection::cocos_clear_colosseum $_13: __func::__clone()
static uint32_t cocos_clear_colosseum_L13_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e934u);
    }
    return v1;
}
RW_FUNCTION(0x76A250, "StageDirection::cocos_clear_colosseum $_13 __func::__clone()", cocos_clear_colosseum_L13_clone);

// StageDirection::cocos_clear_colosseum $_13: __func::__clone(__base*)
static void cocos_clear_colosseum_L13_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e934u);
    }
}
RW_FUNCTION(0x76A274, "StageDirection::cocos_clear_colosseum $_13 __func::__clone(__base*)", cocos_clear_colosseum_L13_cloneInto);

// StageDirection::cocos_clear_colosseum $_13: __func::destroy
static void cocos_clear_colosseum_L13_destroy(uint32_t self) {
}
RW_FUNCTION(0x76A290, "StageDirection::cocos_clear_colosseum $_13 __func::destroy", cocos_clear_colosseum_L13_destroy);

// StageDirection::cocos_clear_colosseum $_13: __func::destroy_deallocate
static void cocos_clear_colosseum_L13_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A292, "StageDirection::cocos_clear_colosseum $_13 __func::destroy_deallocate", cocos_clear_colosseum_L13_destroyDeallocate);

// StageDirection::cocos_clear_colosseum $_13: __func::target
static uint32_t cocos_clear_colosseum_L13_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea0ff0u ? self + 4 : 0; }
RW_FUNCTION(0x76A29C, "StageDirection::cocos_clear_colosseum $_13 __func::target", cocos_clear_colosseum_L13_target);

// StageDirection::cocos_clear_colosseum $_13: __func::target_type
static uint32_t cocos_clear_colosseum_L13_targetType() { return 0xfa96a0u; }
RW_FUNCTION(0x76A2BC, "StageDirection::cocos_clear_colosseum $_13 __func::target_type", cocos_clear_colosseum_L13_targetType);

// StageDirection::cocos_clear_colosseum $_12: __func::~__func (deleting)
static void cocos_clear_colosseum_L12_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A2D0, "StageDirection::cocos_clear_colosseum $_12 __func::~__func (deleting)", cocos_clear_colosseum_L12_deletingDtor);

// StageDirection::cocos_clear_colosseum $_12: __func::__clone()
static uint32_t cocos_clear_colosseum_L12_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e960u);
    }
    return v1;
}
RW_FUNCTION(0x76A2D8, "StageDirection::cocos_clear_colosseum $_12 __func::__clone()", cocos_clear_colosseum_L12_clone);

// StageDirection::cocos_clear_colosseum $_12: __func::__clone(__base*)
static void cocos_clear_colosseum_L12_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e960u);
    }
}
RW_FUNCTION(0x76A2FC, "StageDirection::cocos_clear_colosseum $_12 __func::__clone(__base*)", cocos_clear_colosseum_L12_cloneInto);

// StageDirection::cocos_clear_colosseum $_12: __func::destroy
static void cocos_clear_colosseum_L12_destroy(uint32_t self) {
}
RW_FUNCTION(0x76A318, "StageDirection::cocos_clear_colosseum $_12 __func::destroy", cocos_clear_colosseum_L12_destroy);

// StageDirection::cocos_clear_colosseum $_12: __func::destroy_deallocate
static void cocos_clear_colosseum_L12_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A31A, "StageDirection::cocos_clear_colosseum $_12 __func::destroy_deallocate", cocos_clear_colosseum_L12_destroyDeallocate);

// StageDirection::cocos_clear_colosseum $_12: __func::target
static uint32_t cocos_clear_colosseum_L12_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea10c0u ? self + 4 : 0; }
RW_FUNCTION(0x76A3A8, "StageDirection::cocos_clear_colosseum $_12 __func::target", cocos_clear_colosseum_L12_target);

// StageDirection::cocos_clear_colosseum $_12: __func::target_type
static uint32_t cocos_clear_colosseum_L12_targetType() { return 0xfa96b4u; }
RW_FUNCTION(0x76A3C8, "StageDirection::cocos_clear_colosseum $_12 __func::target_type", cocos_clear_colosseum_L12_targetType);

// StageDirection::cocos_finish $_11: __func::~__func (deleting)
static void cocos_finish_L11_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A3DC, "StageDirection::cocos_finish $_11 __func::~__func (deleting)", cocos_finish_L11_deletingDtor);

// StageDirection::cocos_finish $_11: __func::__clone()
static uint32_t cocos_finish_L11_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e98cu);
    }
    return v1;
}
RW_FUNCTION(0x76A3E4, "StageDirection::cocos_finish $_11 __func::__clone()", cocos_finish_L11_clone);

// StageDirection::cocos_finish $_11: __func::__clone(__base*)
static void cocos_finish_L11_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e98cu);
    }
}
RW_FUNCTION(0x76A408, "StageDirection::cocos_finish $_11 __func::__clone(__base*)", cocos_finish_L11_cloneInto);

// StageDirection::cocos_finish $_11: __func::destroy
static void cocos_finish_L11_destroy(uint32_t self) {
}
RW_FUNCTION(0x76A424, "StageDirection::cocos_finish $_11 __func::destroy", cocos_finish_L11_destroy);

// StageDirection::cocos_finish $_11: __func::destroy_deallocate
static void cocos_finish_L11_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A426, "StageDirection::cocos_finish $_11 __func::destroy_deallocate", cocos_finish_L11_destroyDeallocate);

// StageDirection::cocos_finish $_11: __func::target
static uint32_t cocos_finish_L11_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea1180u ? self + 4 : 0; }
RW_FUNCTION(0x76A430, "StageDirection::cocos_finish $_11 __func::target", cocos_finish_L11_target);

// StageDirection::cocos_finish $_11: __func::target_type
static uint32_t cocos_finish_L11_targetType() { return 0xfa96c8u; }
RW_FUNCTION(0x76A450, "StageDirection::cocos_finish $_11 __func::target_type", cocos_finish_L11_targetType);

// StageDirection::cocos_finish $_10: __func::~__func (deleting)
static void cocos_finish_L10_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A464, "StageDirection::cocos_finish $_10 __func::~__func (deleting)", cocos_finish_L10_deletingDtor);

// StageDirection::cocos_finish $_10: __func::__clone()
static uint32_t cocos_finish_L10_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e9b8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x76A46C, "StageDirection::cocos_finish $_10 __func::__clone()", cocos_finish_L10_clone);

// StageDirection::cocos_finish $_10: __func::__clone(__base*)
static void cocos_finish_L10_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e9b8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x76A498, "StageDirection::cocos_finish $_10 __func::__clone(__base*)", cocos_finish_L10_cloneInto);

// StageDirection::cocos_finish $_10: __func::destroy
static void cocos_finish_L10_destroy(uint32_t self) {
}
RW_FUNCTION(0x76A4B8, "StageDirection::cocos_finish $_10 __func::destroy", cocos_finish_L10_destroy);

// StageDirection::cocos_finish $_10: __func::destroy_deallocate
static void cocos_finish_L10_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A4BA, "StageDirection::cocos_finish $_10 __func::destroy_deallocate", cocos_finish_L10_destroyDeallocate);

// StageDirection::cocos_finish $_10: __func::target
static uint32_t cocos_finish_L10_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea1230u ? self + 4 : 0; }
RW_FUNCTION(0x76A4D0, "StageDirection::cocos_finish $_10 __func::target", cocos_finish_L10_target);

// StageDirection::cocos_finish $_10: __func::target_type
static uint32_t cocos_finish_L10_targetType() { return 0xfa96dcu; }
RW_FUNCTION(0x76A4F0, "StageDirection::cocos_finish $_10 __func::target_type", cocos_finish_L10_targetType);

// StageDirection::cocos_finish $_9: __func::~__func (deleting)
static void cocos_finish_L9_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A504, "StageDirection::cocos_finish $_9 __func::~__func (deleting)", cocos_finish_L9_deletingDtor);

// StageDirection::cocos_finish $_9: __func::__clone()
static uint32_t cocos_finish_L9_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e9e4u);
    }
    return v1;
}
RW_FUNCTION(0x76A50C, "StageDirection::cocos_finish $_9 __func::__clone()", cocos_finish_L9_clone);

// StageDirection::cocos_finish $_9: __func::__clone(__base*)
static void cocos_finish_L9_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e9e4u);
    }
}
RW_FUNCTION(0x76A530, "StageDirection::cocos_finish $_9 __func::__clone(__base*)", cocos_finish_L9_cloneInto);

// StageDirection::cocos_finish $_9: __func::destroy
static void cocos_finish_L9_destroy(uint32_t self) {
}
RW_FUNCTION(0x76A54C, "StageDirection::cocos_finish $_9 __func::destroy", cocos_finish_L9_destroy);

// StageDirection::cocos_finish $_9: __func::destroy_deallocate
static void cocos_finish_L9_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A54E, "StageDirection::cocos_finish $_9 __func::destroy_deallocate", cocos_finish_L9_destroyDeallocate);

// StageDirection::cocos_finish $_9: __func::target
static uint32_t cocos_finish_L9_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea12e0u ? self + 4 : 0; }
RW_FUNCTION(0x76A5DC, "StageDirection::cocos_finish $_9 __func::target", cocos_finish_L9_target);

// StageDirection::cocos_finish $_9: __func::target_type
static uint32_t cocos_finish_L9_targetType() { return 0xfa96f0u; }
RW_FUNCTION(0x76A5FC, "StageDirection::cocos_finish $_9 __func::target_type", cocos_finish_L9_targetType);

// StageDirection::cocos_start $_8: __func::~__func (deleting)
static void cocos_start_L8_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A610, "StageDirection::cocos_start $_8 __func::~__func (deleting)", cocos_start_L8_deletingDtor);

// StageDirection::cocos_start $_8: __func::__clone()
static uint32_t cocos_start_L8_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ea10u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x76A618, "StageDirection::cocos_start $_8 __func::__clone()", cocos_start_L8_clone);

// StageDirection::cocos_start $_8: __func::__clone(__base*)
static void cocos_start_L8_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ea10u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x76A644, "StageDirection::cocos_start $_8 __func::__clone(__base*)", cocos_start_L8_cloneInto);

// StageDirection::cocos_start $_8: __func::destroy
static void cocos_start_L8_destroy(uint32_t self) {
}
RW_FUNCTION(0x76A664, "StageDirection::cocos_start $_8 __func::destroy", cocos_start_L8_destroy);

// StageDirection::cocos_start $_8: __func::destroy_deallocate
static void cocos_start_L8_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76A666, "StageDirection::cocos_start $_8 __func::destroy_deallocate", cocos_start_L8_destroyDeallocate);

}  // namespace
