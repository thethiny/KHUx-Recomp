// SceneSphereBoardSelect: the std::function __func thunks of its lambdas (destructors, clone - copy the captures into a new /
// given block -, destroy, target / target_type typeinfo checks), generated from the lifted code by a scratch
// tool (gen_thunks.py, symbolic walk of each thunk) and checked against it. The lambda bodies are written by hand.
#include "../scene/SceneCommon.hpp"

namespace {
using namespace scene;
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }

// SceneSphereBoardSelect::backScene $_15: __func::~__func (deleting)
static void backScene_L15_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BB7D0, "SceneSphereBoardSelect::backScene $_15 __func::~__func (deleting)", backScene_L15_deletingDtor);

// SceneSphereBoardSelect::backScene $_15: __func::__clone()
static uint32_t backScene_L15_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf665a0u);
    }
    return v1;
}
RW_FUNCTION(0x6BB7D8, "SceneSphereBoardSelect::backScene $_15 __func::__clone()", backScene_L15_clone);

// SceneSphereBoardSelect::backScene $_15: __func::__clone(__base*)
static void backScene_L15_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf665a0u);
    }
}
RW_FUNCTION(0x6BB7FC, "SceneSphereBoardSelect::backScene $_15 __func::__clone(__base*)", backScene_L15_cloneInto);

// SceneSphereBoardSelect::backScene $_15: __func::destroy
static void backScene_L15_destroy(uint32_t self) {
}
RW_FUNCTION(0x6BB818, "SceneSphereBoardSelect::backScene $_15 __func::destroy", backScene_L15_destroy);

// SceneSphereBoardSelect::backScene $_15: __func::destroy_deallocate
static void backScene_L15_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BB81A, "SceneSphereBoardSelect::backScene $_15 __func::destroy_deallocate", backScene_L15_destroyDeallocate);

// SceneSphereBoardSelect::backScene $_15: __func::target
static uint32_t backScene_L15_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe79d90u ? self + 4 : 0; }
RW_FUNCTION(0x6BB8D0, "SceneSphereBoardSelect::backScene $_15 __func::target", backScene_L15_target);

// SceneSphereBoardSelect::backScene $_15: __func::target_type
static uint32_t backScene_L15_targetType() { return 0xf9e64cu; }
RW_FUNCTION(0x6BB8F0, "SceneSphereBoardSelect::backScene $_15 __func::target_type", backScene_L15_targetType);

// SceneSphereBoardSelect::backScene $_14: __func::~__func (deleting)
static void backScene_L14_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BB904, "SceneSphereBoardSelect::backScene $_14 __func::~__func (deleting)", backScene_L14_deletingDtor);

// SceneSphereBoardSelect::backScene $_14: __func::__clone()
static uint32_t backScene_L14_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf665ccu);
    }
    return v1;
}
RW_FUNCTION(0x6BB90C, "SceneSphereBoardSelect::backScene $_14 __func::__clone()", backScene_L14_clone);

// SceneSphereBoardSelect::backScene $_14: __func::__clone(__base*)
static void backScene_L14_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf665ccu);
    }
}
RW_FUNCTION(0x6BB930, "SceneSphereBoardSelect::backScene $_14 __func::__clone(__base*)", backScene_L14_cloneInto);

// SceneSphereBoardSelect::backScene $_14: __func::destroy
static void backScene_L14_destroy(uint32_t self) {
}
RW_FUNCTION(0x6BB94C, "SceneSphereBoardSelect::backScene $_14 __func::destroy", backScene_L14_destroy);

// SceneSphereBoardSelect::backScene $_14: __func::destroy_deallocate
static void backScene_L14_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BB94E, "SceneSphereBoardSelect::backScene $_14 __func::destroy_deallocate", backScene_L14_destroyDeallocate);

// SceneSphereBoardSelect::backScene $_14: __func::target
static uint32_t backScene_L14_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe79e30u ? self + 4 : 0; }
RW_FUNCTION(0x6BBA78, "SceneSphereBoardSelect::backScene $_14 __func::target", backScene_L14_target);

// SceneSphereBoardSelect::backScene $_14: __func::target_type
static uint32_t backScene_L14_targetType() { return 0xf9e660u; }
RW_FUNCTION(0x6BBA98, "SceneSphereBoardSelect::backScene $_14 __func::target_type", backScene_L14_targetType);

// SceneSphereBoardSelect::runOutAnimationMain $_11: __func::~__func
static uint32_t runOutAnimationMain_L11_dtor(uint32_t self) {
    st32(self, 0xf665f8u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6BBAAC, "SceneSphereBoardSelect::runOutAnimationMain $_11 __func::~__func", runOutAnimationMain_L11_dtor);

// SceneSphereBoardSelect::runOutAnimationMain $_11: __func::~__func (deleting)
static void runOutAnimationMain_L11_deletingDtor(uint32_t self) {
    st32(self, 0xf665f8u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBAE8, "SceneSphereBoardSelect::runOutAnimationMain $_11 __func::~__func (deleting)", runOutAnimationMain_L11_deletingDtor);

// SceneSphereBoardSelect::runOutAnimationMain $_11: __func::__clone()
static uint32_t runOutAnimationMain_L11_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf665f8u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6BBB28, "SceneSphereBoardSelect::runOutAnimationMain $_11 __func::__clone()", runOutAnimationMain_L11_clone);

// SceneSphereBoardSelect::runOutAnimationMain $_11: __func::__clone(__base*)
static void runOutAnimationMain_L11_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf665f8u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x6BBB78, "SceneSphereBoardSelect::runOutAnimationMain $_11 __func::__clone(__base*)", runOutAnimationMain_L11_cloneInto);

// SceneSphereBoardSelect::runOutAnimationMain $_11: __func::destroy
static void runOutAnimationMain_L11_destroy(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6BBBAC, "SceneSphereBoardSelect::runOutAnimationMain $_11 __func::destroy", runOutAnimationMain_L11_destroy);

// SceneSphereBoardSelect::runOutAnimationMain $_11: __func::destroy_deallocate
static void runOutAnimationMain_L11_destroyDeallocate(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBBCC, "SceneSphereBoardSelect::runOutAnimationMain $_11 __func::destroy_deallocate", runOutAnimationMain_L11_destroyDeallocate);

// SceneSphereBoardSelect::runOutAnimationMain $_11: __func::target
static uint32_t runOutAnimationMain_L11_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe79ee0u ? self + 8 : 0; }
RW_FUNCTION(0x6BBC08, "SceneSphereBoardSelect::runOutAnimationMain $_11 __func::target", runOutAnimationMain_L11_target);

// SceneSphereBoardSelect::runOutAnimationMain $_11: __func::target_type
static uint32_t runOutAnimationMain_L11_targetType() { return 0xf9e674u; }
RW_FUNCTION(0x6BBC28, "SceneSphereBoardSelect::runOutAnimationMain $_11 __func::target_type", runOutAnimationMain_L11_targetType);

// SceneSphereBoardSelect::initSphereBoardList $_10: __func::~__func (deleting)
static void initSphereBoardList_L10_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBC3C, "SceneSphereBoardSelect::initSphereBoardList $_10 __func::~__func (deleting)", initSphereBoardList_L10_deletingDtor);

// SceneSphereBoardSelect::initSphereBoardList $_10: __func::__clone()
static uint32_t initSphereBoardList_L10_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf66624u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6BBC44, "SceneSphereBoardSelect::initSphereBoardList $_10 __func::__clone()", initSphereBoardList_L10_clone);

// SceneSphereBoardSelect::initSphereBoardList $_10: __func::__clone(__base*)
static void initSphereBoardList_L10_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf66624u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6BBC74, "SceneSphereBoardSelect::initSphereBoardList $_10 __func::__clone(__base*)", initSphereBoardList_L10_cloneInto);

// SceneSphereBoardSelect::initSphereBoardList $_10: __func::destroy
static void initSphereBoardList_L10_destroy(uint32_t self) {
}
RW_FUNCTION(0x6BBC98, "SceneSphereBoardSelect::initSphereBoardList $_10 __func::destroy", initSphereBoardList_L10_destroy);

// SceneSphereBoardSelect::initSphereBoardList $_10: __func::destroy_deallocate
static void initSphereBoardList_L10_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBC9A, "SceneSphereBoardSelect::initSphereBoardList $_10 __func::destroy_deallocate", initSphereBoardList_L10_destroyDeallocate);

// SceneSphereBoardSelect::initSphereBoardList $_10: __func::target
static uint32_t initSphereBoardList_L10_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe79fd0u ? self + 4 : 0; }
RW_FUNCTION(0x6BBCE8, "SceneSphereBoardSelect::initSphereBoardList $_10 __func::target", initSphereBoardList_L10_target);

// SceneSphereBoardSelect::initSphereBoardList $_10: __func::target_type
static uint32_t initSphereBoardList_L10_targetType() { return 0xf9e688u; }
RW_FUNCTION(0x6BBD08, "SceneSphereBoardSelect::initSphereBoardList $_10 __func::target_type", initSphereBoardList_L10_targetType);

// SceneSphereBoardSelect::initSphereBoardList $_9: __func::~__func (deleting)
static void initSphereBoardList_L9_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBD1C, "SceneSphereBoardSelect::initSphereBoardList $_9 __func::~__func (deleting)", initSphereBoardList_L9_deletingDtor);

// SceneSphereBoardSelect::initSphereBoardList $_9: __func::__clone()
static uint32_t initSphereBoardList_L9_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf66650u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6BBD24, "SceneSphereBoardSelect::initSphereBoardList $_9 __func::__clone()", initSphereBoardList_L9_clone);

// SceneSphereBoardSelect::initSphereBoardList $_9: __func::__clone(__base*)
static void initSphereBoardList_L9_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf66650u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6BBD54, "SceneSphereBoardSelect::initSphereBoardList $_9 __func::__clone(__base*)", initSphereBoardList_L9_cloneInto);

// SceneSphereBoardSelect::initSphereBoardList $_9: __func::destroy
static void initSphereBoardList_L9_destroy(uint32_t self) {
}
RW_FUNCTION(0x6BBD78, "SceneSphereBoardSelect::initSphereBoardList $_9 __func::destroy", initSphereBoardList_L9_destroy);

// SceneSphereBoardSelect::initSphereBoardList $_9: __func::destroy_deallocate
static void initSphereBoardList_L9_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBD7A, "SceneSphereBoardSelect::initSphereBoardList $_9 __func::destroy_deallocate", initSphereBoardList_L9_destroyDeallocate);

// SceneSphereBoardSelect::initSphereBoardList $_9: __func::target
static uint32_t initSphereBoardList_L9_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7a0b0u ? self + 4 : 0; }
RW_FUNCTION(0x6BBDC8, "SceneSphereBoardSelect::initSphereBoardList $_9 __func::target", initSphereBoardList_L9_target);

// SceneSphereBoardSelect::initSphereBoardList $_9: __func::target_type
static uint32_t initSphereBoardList_L9_targetType() { return 0xf9e69cu; }
RW_FUNCTION(0x6BBDE8, "SceneSphereBoardSelect::initSphereBoardList $_9 __func::target_type", initSphereBoardList_L9_targetType);

// SceneSphereBoardSelect::initSphereBoardList $_8: __func::~__func (deleting)
static void initSphereBoardList_L8_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBDFC, "SceneSphereBoardSelect::initSphereBoardList $_8 __func::~__func (deleting)", initSphereBoardList_L8_deletingDtor);

// SceneSphereBoardSelect::initSphereBoardList $_8: __func::__clone()
static uint32_t initSphereBoardList_L8_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6667cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6BBE04, "SceneSphereBoardSelect::initSphereBoardList $_8 __func::__clone()", initSphereBoardList_L8_clone);

// SceneSphereBoardSelect::initSphereBoardList $_8: __func::__clone(__base*)
static void initSphereBoardList_L8_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6667cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6BBE30, "SceneSphereBoardSelect::initSphereBoardList $_8 __func::__clone(__base*)", initSphereBoardList_L8_cloneInto);

// SceneSphereBoardSelect::initSphereBoardList $_8: __func::destroy
static void initSphereBoardList_L8_destroy(uint32_t self) {
}
RW_FUNCTION(0x6BBE50, "SceneSphereBoardSelect::initSphereBoardList $_8 __func::destroy", initSphereBoardList_L8_destroy);

// SceneSphereBoardSelect::initSphereBoardList $_8: __func::destroy_deallocate
static void initSphereBoardList_L8_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBE52, "SceneSphereBoardSelect::initSphereBoardList $_8 __func::destroy_deallocate", initSphereBoardList_L8_destroyDeallocate);

// SceneSphereBoardSelect::initSphereBoardList $_8: __func::target
static uint32_t initSphereBoardList_L8_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7a160u ? self + 4 : 0; }
RW_FUNCTION(0x6BBE64, "SceneSphereBoardSelect::initSphereBoardList $_8 __func::target", initSphereBoardList_L8_target);

// SceneSphereBoardSelect::initSphereBoardList $_8: __func::target_type
static uint32_t initSphereBoardList_L8_targetType() { return 0xf9e6b0u; }
RW_FUNCTION(0x6BBE84, "SceneSphereBoardSelect::initSphereBoardList $_8 __func::target_type", initSphereBoardList_L8_targetType);

// SceneSphereBoardSelect::initSphereBoardList $_7: __func::~__func (deleting)
static void initSphereBoardList_L7_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBE98, "SceneSphereBoardSelect::initSphereBoardList $_7 __func::~__func (deleting)", initSphereBoardList_L7_deletingDtor);

// SceneSphereBoardSelect::initSphereBoardList $_7: __func::__clone()
static uint32_t initSphereBoardList_L7_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf666a8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6BBEA0, "SceneSphereBoardSelect::initSphereBoardList $_7 __func::__clone()", initSphereBoardList_L7_clone);

// SceneSphereBoardSelect::initSphereBoardList $_7: __func::__clone(__base*)
static void initSphereBoardList_L7_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf666a8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6BBECC, "SceneSphereBoardSelect::initSphereBoardList $_7 __func::__clone(__base*)", initSphereBoardList_L7_cloneInto);

// SceneSphereBoardSelect::initSphereBoardList $_7: __func::destroy
static void initSphereBoardList_L7_destroy(uint32_t self) {
}
RW_FUNCTION(0x6BBEEC, "SceneSphereBoardSelect::initSphereBoardList $_7 __func::destroy", initSphereBoardList_L7_destroy);

// SceneSphereBoardSelect::initSphereBoardList $_7: __func::destroy_deallocate
static void initSphereBoardList_L7_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BBEEE, "SceneSphereBoardSelect::initSphereBoardList $_7 __func::destroy_deallocate", initSphereBoardList_L7_destroyDeallocate);

// SceneSphereBoardSelect::initSphereBoardList $_7: __func::target
static uint32_t initSphereBoardList_L7_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7a210u ? self + 4 : 0; }
RW_FUNCTION(0x6BBF10, "SceneSphereBoardSelect::initSphereBoardList $_7 __func::target", initSphereBoardList_L7_target);

// SceneSphereBoardSelect::initSphereBoardList $_7: __func::target_type
static uint32_t initSphereBoardList_L7_targetType() { return 0xf9e6c4u; }
RW_FUNCTION(0x6BBF30, "SceneSphereBoardSelect::initSphereBoardList $_7 __func::target_type", initSphereBoardList_L7_targetType);

// SceneSphereBoardSelect::initSphereBoardList $_6: __func::~__func
static uint32_t initSphereBoardList_L6_dtor(uint32_t self) {
    st32(self, 0xf666d4u);
    call(0x478961, {self + 16});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6BC040, "SceneSphereBoardSelect::initSphereBoardList $_6 __func::~__func", initSphereBoardList_L6_dtor);

// SceneSphereBoardSelect::initSphereBoardList $_6: __func::~__func (deleting)
static void initSphereBoardList_L6_deletingDtor(uint32_t self) {
    st32(self, 0xf666d4u);
    call(0x478961, {self + 16});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BC074, "SceneSphereBoardSelect::initSphereBoardList $_6 __func::~__func (deleting)", initSphereBoardList_L6_deletingDtor);

// SceneSphereBoardSelect::initSphereBoardList $_6: __func::__clone()
static uint32_t initSphereBoardList_L6_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf666d4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t m6 = v2;
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
            uint32_t v5 = ld32(self + 4);
            m6 = v5;
        }
        st32(v1 + 4, m6);
        call(0x4af707, {v1 + 16, self + 16});   // _ZNSt3__16vectorIiNS_9allocatorIiEEEC2ERKS3_
        uint32_t v7 = ld32(self + 4);
        st32(v1 + 4, v7);
    }
    return v1;
}
RW_FUNCTION(0x6BC0AC, "SceneSphereBoardSelect::initSphereBoardList $_6 __func::__clone()", initSphereBoardList_L6_clone);

// SceneSphereBoardSelect::initSphereBoardList $_6: __func::__clone(__base*)
static void initSphereBoardList_L6_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf666d4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 4);
        st32(p + 4, v4);
        call(0x4af707, {p + 16, self + 16});   // _ZNSt3__16vectorIiNS_9allocatorIiEEEC2ERKS3_
        uint32_t v5 = ld32(self + 4);
        st32(p + 4, v5);
    }
}
RW_FUNCTION(0x6BC128, "SceneSphereBoardSelect::initSphereBoardList $_6 __func::__clone(__base*)", initSphereBoardList_L6_cloneInto);

// SceneSphereBoardSelect::initSphereBoardList $_6: __func::destroy
static void initSphereBoardList_L6_destroy(uint32_t self) {
    call(0x478961, {self + 16});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x6BC190, "SceneSphereBoardSelect::initSphereBoardList $_6 __func::destroy", initSphereBoardList_L6_destroy);

// SceneSphereBoardSelect::initSphereBoardList $_6: __func::destroy_deallocate
static void initSphereBoardList_L6_destroyDeallocate(uint32_t self) {
    call(0x478961, {self + 16});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BC1A8, "SceneSphereBoardSelect::initSphereBoardList $_6 __func::destroy_deallocate", initSphereBoardList_L6_destroyDeallocate);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_: __func::~__func (deleting)
static void openBoardBuyDialog_L12_Uv0_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BD78C, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_ __func::~__func (deleting)", openBoardBuyDialog_L12_Uv0_deletingDtor);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_: __func::__clone()
static uint32_t openBoardBuyDialog_L12_Uv0_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf66910u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6BD794, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_ __func::__clone()", openBoardBuyDialog_L12_Uv0_clone);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_: __func::__clone(__base*)
static void openBoardBuyDialog_L12_Uv0_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf66910u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6BD7C0, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_ __func::__clone(__base*)", openBoardBuyDialog_L12_Uv0_cloneInto);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_: __func::destroy
static void openBoardBuyDialog_L12_Uv0_destroy(uint32_t self) {
}
RW_FUNCTION(0x6BD7E0, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_ __func::destroy", openBoardBuyDialog_L12_Uv0_destroy);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_: __func::destroy_deallocate
static void openBoardBuyDialog_L12_Uv0_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BD7E2, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_ __func::destroy_deallocate", openBoardBuyDialog_L12_Uv0_destroyDeallocate);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_: __func::target
static uint32_t openBoardBuyDialog_L12_Uv0_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7ad80u ? self + 4 : 0; }
RW_FUNCTION(0x6BD7F4, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_ __func::target", openBoardBuyDialog_L12_Uv0_target);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_: __func::target_type
static uint32_t openBoardBuyDialog_L12_Uv0_targetType() { return 0xf9e7dcu; }
RW_FUNCTION(0x6BD814, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlvE0_ __func::target_type", openBoardBuyDialog_L12_Uv0_targetType);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::~__func (deleting)
static void openBoardBuyDialog_L12_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BD828, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::~__func (deleting)", openBoardBuyDialog_L12_deletingDtor);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::__clone()
static uint32_t openBoardBuyDialog_L12_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6693cu);
    }
    return v1;
}
RW_FUNCTION(0x6BD830, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::__clone()", openBoardBuyDialog_L12_clone);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::__clone(__base*)
static void openBoardBuyDialog_L12_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6693cu);
    }
}
RW_FUNCTION(0x6BD854, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::__clone(__base*)", openBoardBuyDialog_L12_cloneInto);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::destroy
static void openBoardBuyDialog_L12_destroy(uint32_t self) {
}
RW_FUNCTION(0x6BD870, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::destroy", openBoardBuyDialog_L12_destroy);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::destroy_deallocate
static void openBoardBuyDialog_L12_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BD872, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::destroy_deallocate", openBoardBuyDialog_L12_destroyDeallocate);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::target
static uint32_t openBoardBuyDialog_L12_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7aea0u ? self + 4 : 0; }
RW_FUNCTION(0x6BD890, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::target", openBoardBuyDialog_L12_target);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::target_type
static uint32_t openBoardBuyDialog_L12_targetType() { return 0xf9e7f0u; }
RW_FUNCTION(0x6BD8B0, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::target_type", openBoardBuyDialog_L12_targetType);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::~__func
static uint32_t openBoardBuyDialog_L12_dtor(uint32_t self) {
    st32(self, 0xf66968u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x6BD8C4, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::~__func", openBoardBuyDialog_L12_dtor);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::~__func (deleting)
static void openBoardBuyDialog_L12_deletingDtor_2(uint32_t self) {
    st32(self, 0xf66968u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BD900, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::~__func (deleting)", openBoardBuyDialog_L12_deletingDtor_2);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::__clone()
static uint32_t openBoardBuyDialog_L12_clone_2(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf66968u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x45cd55, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
        uint32_t v5 = ld32(self + 48);
        st32(v1 + 48, v5);
    }
    return v1;
}
RW_FUNCTION(0x6BD940, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::__clone()", openBoardBuyDialog_L12_clone_2);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::__clone(__base*)
static void openBoardBuyDialog_L12_cloneInto_2(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf66968u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        call(0x45cd55, {p + 24, self + 24});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
        uint32_t v4 = ld32(self + 48);
        st32(p + 48, v4);
    }
}
RW_FUNCTION(0x6BD9A0, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::__clone(__base*)", openBoardBuyDialog_L12_cloneInto_2);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::destroy
static void openBoardBuyDialog_L12_destroy_2(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x6BD9E8, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::destroy", openBoardBuyDialog_L12_destroy_2);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::destroy_deallocate
static void openBoardBuyDialog_L12_destroyDeallocate_2(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BDA08, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::destroy_deallocate", openBoardBuyDialog_L12_destroyDeallocate_2);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::target
static uint32_t openBoardBuyDialog_L12_target_2(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7aff0u ? self + 8 : 0; }
RW_FUNCTION(0x6BDC18, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::target", openBoardBuyDialog_L12_target_2);

// SceneSphereBoardSelect::openBoardBuyDialog $_12: __func::target_type
static uint32_t openBoardBuyDialog_L12_targetType_2() { return 0xf9e804u; }
RW_FUNCTION(0x6BDC3C, "SceneSphereBoardSelect::openBoardBuyDialog $_12 __func::target_type", openBoardBuyDialog_L12_targetType_2);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_: __func::~__func
static uint32_t openBoardBuyDialog_L12_Ub_dtor(uint32_t self) {
    st32(self, 0xf66994u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x6BDC50, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ __func::~__func", openBoardBuyDialog_L12_Ub_dtor);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_: __func::~__func (deleting)
static void openBoardBuyDialog_L12_Ub_deletingDtor(uint32_t self) {
    st32(self, 0xf66994u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BDC8C, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ __func::~__func (deleting)", openBoardBuyDialog_L12_Ub_deletingDtor);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_: __func::__clone(__base*)
static void openBoardBuyDialog_L12_Ub_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf66994u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        call(0x45cd55, {p + 24, self + 24});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
        uint32_t v4 = ld32(self + 8 + 0);
        uint32_t v5 = ld32(self + 8 + 4);
        uint32_t v6 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v4);
        st32(p + 8 + 4, v5);
        st32(p + 8 + 8, v6);
    }
}
RW_FUNCTION(0x6BDD3C, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ __func::__clone(__base*)", openBoardBuyDialog_L12_Ub_cloneInto);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_: __func::destroy
static void openBoardBuyDialog_L12_Ub_destroy(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x6BDD8C, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ __func::destroy", openBoardBuyDialog_L12_Ub_destroy);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_: __func::destroy_deallocate
static void openBoardBuyDialog_L12_Ub_destroyDeallocate(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BDDAC, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ __func::destroy_deallocate", openBoardBuyDialog_L12_Ub_destroyDeallocate);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_: __func::target
static uint32_t openBoardBuyDialog_L12_Ub_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7b150u ? self + 8 : 0; }
RW_FUNCTION(0x6BDFD0, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ __func::target", openBoardBuyDialog_L12_Ub_target);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_: __func::target_type
static uint32_t openBoardBuyDialog_L12_Ub_targetType() { return 0xf9e818u; }
RW_FUNCTION(0x6BDFF4, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ __func::target_type", openBoardBuyDialog_L12_Ub_targetType);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_: __func::~__func (deleting)
static void openBoardBuyDialog_L12_Ub_Ub_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BE008, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_ __func::~__func (deleting)", openBoardBuyDialog_L12_Ub_Ub_deletingDtor);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_: __func::__clone()
static uint32_t openBoardBuyDialog_L12_Ub_Ub_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf669c0u);
    }
    return v1;
}
RW_FUNCTION(0x6BE010, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_ __func::__clone()", openBoardBuyDialog_L12_Ub_Ub_clone);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_: __func::__clone(__base*)
static void openBoardBuyDialog_L12_Ub_Ub_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf669c0u);
    }
}
RW_FUNCTION(0x6BE034, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_ __func::__clone(__base*)", openBoardBuyDialog_L12_Ub_Ub_cloneInto);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_: __func::destroy
static void openBoardBuyDialog_L12_Ub_Ub_destroy(uint32_t self) {
}
RW_FUNCTION(0x6BE050, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_ __func::destroy", openBoardBuyDialog_L12_Ub_Ub_destroy);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_: __func::destroy_deallocate
static void openBoardBuyDialog_L12_Ub_Ub_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6BE052, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_ __func::destroy_deallocate", openBoardBuyDialog_L12_Ub_Ub_destroyDeallocate);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_: __func::target
static uint32_t openBoardBuyDialog_L12_Ub_Ub_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7b2d0u ? self + 4 : 0; }
RW_FUNCTION(0x6BE068, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_ __func::target", openBoardBuyDialog_L12_Ub_Ub_target);

// SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_: __func::target_type
static uint32_t openBoardBuyDialog_L12_Ub_Ub_targetType() { return 0xf9e82cu; }
RW_FUNCTION(0x6BE088, "SceneSphereBoardSelect::openBoardBuyDialog $_12 UlbE_ UlbE_ __func::target_type", openBoardBuyDialog_L12_Ub_Ub_targetType);

}  // namespace
