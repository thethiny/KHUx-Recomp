// Lambdas of SceneUnionRegister (nextState $_12 / $_15, openPopUpbeLongToUnion $_8 .. $_11 / $_27,
// runOutAnimation $_7, runInAnimation $_6, startTutorialStage $_5, callTutorialStageStartAPI $_4, init $_0 .. $_3
// and their nested lambdas) that owners.py attributes to SceneAdventureSelect (they sit right before it in the
// binary; src/scene_social/thunks/SceneUnionRegister_thunks.cpp has the others). The std::function __func slots
// were generated from the lifted code by a scratch tool (gen_thunks.py, the same one as src/scene_social/thunks)
// and mirror it exactly: clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). The lambda bodies (operator()) follow, written by hand.
#include "SceneAdventureSelect.hpp"

using namespace scene;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ------------------------------------------------------------------------------------------------ thunks
// SceneUnionRegister::nextState $_12: __func::target
static uint32_t t_71BA14(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92780u ? self + 4 : 0; }
RW_FUNCTION(0x71BA14, "SceneUnionRegister::nextState $_12 __func::target", t_71BA14);

// SceneUnionRegister::nextState $_12: __func::target_type
static uint32_t t_71BA34() { return 0xfa56a4u; }
RW_FUNCTION(0x71BA34, "SceneUnionRegister::nextState $_12 __func::target_type", t_71BA34);

// SceneUnionRegister::nextState $_12 UlbE0_: __func::~__func (deleting)
static void t_71BA48(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BA48, "SceneUnionRegister::nextState $_12 UlbE0_ __func::~__func (deleting)", t_71BA48);

// SceneUnionRegister::nextState $_12 UlbE0_: __func::__clone()
static uint32_t t_71BA50(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ba60u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71BA50, "SceneUnionRegister::nextState $_12 UlbE0_ __func::__clone()", t_71BA50);

// SceneUnionRegister::nextState $_12 UlbE0_: __func::__clone(__base*)
static void t_71BA7C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ba60u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71BA7C, "SceneUnionRegister::nextState $_12 UlbE0_ __func::__clone(__base*)", t_71BA7C);

// SceneUnionRegister::nextState $_12 UlbE0_: __func::destroy
static void t_71BA9C(uint32_t self) {
}
RW_FUNCTION(0x71BA9C, "SceneUnionRegister::nextState $_12 UlbE0_ __func::destroy", t_71BA9C);

// SceneUnionRegister::nextState $_12 UlbE0_: __func::destroy_deallocate
static void t_71BA9E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BA9E, "SceneUnionRegister::nextState $_12 UlbE0_ __func::destroy_deallocate", t_71BA9E);

// SceneUnionRegister::nextState $_12 UlbE0_: __func::target
static uint32_t t_71BAB0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92820u ? self + 4 : 0; }
RW_FUNCTION(0x71BAB0, "SceneUnionRegister::nextState $_12 UlbE0_ __func::target", t_71BAB0);

// SceneUnionRegister::nextState $_12 UlbE0_: __func::target_type
static uint32_t t_71BAD0() { return 0xfa56b8u; }
RW_FUNCTION(0x71BAD0, "SceneUnionRegister::nextState $_12 UlbE0_ __func::target_type", t_71BAD0);

// SceneUnionRegister::nextState $_12 UlbE_: __func::~__func (deleting)
static void t_71BAE4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BAE4, "SceneUnionRegister::nextState $_12 UlbE_ __func::~__func (deleting)", t_71BAE4);

// SceneUnionRegister::nextState $_12 UlbE_: __func::__clone()
static uint32_t t_71BAEC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ba8cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71BAEC, "SceneUnionRegister::nextState $_12 UlbE_ __func::__clone()", t_71BAEC);

// SceneUnionRegister::nextState $_12 UlbE_: __func::__clone(__base*)
static void t_71BB18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ba8cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71BB18, "SceneUnionRegister::nextState $_12 UlbE_ __func::__clone(__base*)", t_71BB18);

// SceneUnionRegister::nextState $_12 UlbE_: __func::destroy
static void t_71BB38(uint32_t self) {
}
RW_FUNCTION(0x71BB38, "SceneUnionRegister::nextState $_12 UlbE_ __func::destroy", t_71BB38);

// SceneUnionRegister::nextState $_12 UlbE_: __func::destroy_deallocate
static void t_71BB3A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BB3A, "SceneUnionRegister::nextState $_12 UlbE_ __func::destroy_deallocate", t_71BB3A);

// SceneUnionRegister::nextState $_12 UlbE_: __func::target
static uint32_t t_71BBD8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe928d0u ? self + 4 : 0; }
RW_FUNCTION(0x71BBD8, "SceneUnionRegister::nextState $_12 UlbE_ __func::target", t_71BBD8);

// SceneUnionRegister::nextState $_12 UlbE_: __func::target_type
static uint32_t t_71BBF8() { return 0xfa56ccu; }
RW_FUNCTION(0x71BBF8, "SceneUnionRegister::nextState $_12 UlbE_ __func::target_type", t_71BBF8);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_: __func::~__func (deleting)
static void t_71BC0C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BC0C, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ __func::~__func (deleting)", t_71BC0C);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_: __func::__clone()
static uint32_t t_71BC14(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bab8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71BC14, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ __func::__clone()", t_71BC14);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_: __func::__clone(__base*)
static void t_71BC40(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bab8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71BC40, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ __func::__clone(__base*)", t_71BC40);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_: __func::destroy
static void t_71BC60(uint32_t self) {
}
RW_FUNCTION(0x71BC60, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ __func::destroy", t_71BC60);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_: __func::destroy_deallocate
static void t_71BC62(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BC62, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ __func::destroy_deallocate", t_71BC62);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_: __func::target
static uint32_t t_71BD00(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92990u ? self + 4 : 0; }
RW_FUNCTION(0x71BD00, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ __func::target", t_71BD00);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_: __func::target_type
static uint32_t t_71BD20() { return 0xfa56e0u; }
RW_FUNCTION(0x71BD20, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ __func::target_type", t_71BD20);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_71BD34(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BD34, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_ __func::~__func (deleting)", t_71BD34);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_71BD3C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bae4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71BD3C, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_ __func::__clone()", t_71BD3C);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_71BD68(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bae4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71BD68, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_ __func::__clone(__base*)", t_71BD68);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_: __func::destroy
static void t_71BD88(uint32_t self) {
}
RW_FUNCTION(0x71BD88, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_ __func::destroy", t_71BD88);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_71BD8A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BD8A, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_ __func::destroy_deallocate", t_71BD8A);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_: __func::target
static uint32_t t_71BD9C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92a70u ? self + 4 : 0; }
RW_FUNCTION(0x71BD9C, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_ __func::target", t_71BD9C);

// SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_71BDBC() { return 0xfa56f4u; }
RW_FUNCTION(0x71BDBC, "SceneUnionRegister::nextState $_12 UlbE_ UlbE_ UlbE_ __func::target_type", t_71BDBC);

// SceneUnionRegister::openPopUpbeLongToUnion $_11: __func::~__func (deleting)
static void t_71BDD0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BDD0, "SceneUnionRegister::openPopUpbeLongToUnion $_11 __func::~__func (deleting)", t_71BDD0);

// SceneUnionRegister::openPopUpbeLongToUnion $_11: __func::__clone()
static uint32_t t_71BDD8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6bb10u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x71BDD8, "SceneUnionRegister::openPopUpbeLongToUnion $_11 __func::__clone()", t_71BDD8);

// SceneUnionRegister::openPopUpbeLongToUnion $_11: __func::__clone(__base*)
static void t_71BE08(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bb10u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71BE08, "SceneUnionRegister::openPopUpbeLongToUnion $_11 __func::__clone(__base*)", t_71BE08);

// SceneUnionRegister::openPopUpbeLongToUnion $_11: __func::destroy
static void t_71BE30(uint32_t self) {
}
RW_FUNCTION(0x71BE30, "SceneUnionRegister::openPopUpbeLongToUnion $_11 __func::destroy", t_71BE30);

// SceneUnionRegister::openPopUpbeLongToUnion $_11: __func::destroy_deallocate
static void t_71BE32(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BE32, "SceneUnionRegister::openPopUpbeLongToUnion $_11 __func::destroy_deallocate", t_71BE32);

// SceneUnionRegister::openPopUpbeLongToUnion $_11: __func::target
static uint32_t t_71BF34(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92b40u ? self + 4 : 0; }
RW_FUNCTION(0x71BF34, "SceneUnionRegister::openPopUpbeLongToUnion $_11 __func::target", t_71BF34);

// SceneUnionRegister::openPopUpbeLongToUnion $_11: __func::target_type
static uint32_t t_71BF54() { return 0xfa5708u; }
RW_FUNCTION(0x71BF54, "SceneUnionRegister::openPopUpbeLongToUnion $_11 __func::target_type", t_71BF54);

// SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_: __func::~__func (deleting)
static void t_71BF68(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BF68, "SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_ __func::~__func (deleting)", t_71BF68);

// SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_: __func::__clone()
static uint32_t t_71BF70(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6bb3cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x71BF70, "SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_ __func::__clone()", t_71BF70);

// SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_: __func::__clone(__base*)
static void t_71BFA0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bb3cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71BFA0, "SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_ __func::__clone(__base*)", t_71BFA0);

// SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_: __func::destroy
static void t_71BFC8(uint32_t self) {
}
RW_FUNCTION(0x71BFC8, "SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_ __func::destroy", t_71BFC8);

// SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_: __func::destroy_deallocate
static void t_71BFCA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71BFCA, "SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_ __func::destroy_deallocate", t_71BFCA);

// SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_: __func::target
static uint32_t t_71BFFC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92c20u ? self + 4 : 0; }
RW_FUNCTION(0x71BFFC, "SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_ __func::target", t_71BFFC);

// SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_: __func::target_type
static uint32_t t_71C01C() { return 0xfa571cu; }
RW_FUNCTION(0x71C01C, "SceneUnionRegister::openPopUpbeLongToUnion $_27 UlvE_ __func::target_type", t_71C01C);

// SceneUnionRegister::openPopUpbeLongToUnion $_10: __func::~__func (deleting)
static void t_71C030(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C030, "SceneUnionRegister::openPopUpbeLongToUnion $_10 __func::~__func (deleting)", t_71C030);

// SceneUnionRegister::openPopUpbeLongToUnion $_10: __func::__clone()
static uint32_t t_71C038(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bb68u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71C038, "SceneUnionRegister::openPopUpbeLongToUnion $_10 __func::__clone()", t_71C038);

// SceneUnionRegister::openPopUpbeLongToUnion $_10: __func::__clone(__base*)
static void t_71C064(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bb68u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71C064, "SceneUnionRegister::openPopUpbeLongToUnion $_10 __func::__clone(__base*)", t_71C064);

// SceneUnionRegister::openPopUpbeLongToUnion $_10: __func::destroy
static void t_71C084(uint32_t self) {
}
RW_FUNCTION(0x71C084, "SceneUnionRegister::openPopUpbeLongToUnion $_10 __func::destroy", t_71C084);

// SceneUnionRegister::openPopUpbeLongToUnion $_10: __func::destroy_deallocate
static void t_71C086(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C086, "SceneUnionRegister::openPopUpbeLongToUnion $_10 __func::destroy_deallocate", t_71C086);

// SceneUnionRegister::openPopUpbeLongToUnion $_10: __func::target
static uint32_t t_71C098(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92d00u ? self + 4 : 0; }
RW_FUNCTION(0x71C098, "SceneUnionRegister::openPopUpbeLongToUnion $_10 __func::target", t_71C098);

// SceneUnionRegister::openPopUpbeLongToUnion $_10: __func::target_type
static uint32_t t_71C0B8() { return 0xfa5730u; }
RW_FUNCTION(0x71C0B8, "SceneUnionRegister::openPopUpbeLongToUnion $_10 __func::target_type", t_71C0B8);

// SceneUnionRegister::openPopUpbeLongToUnion $_9: __func::~__func (deleting)
static void t_71C0CC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C0CC, "SceneUnionRegister::openPopUpbeLongToUnion $_9 __func::~__func (deleting)", t_71C0CC);

// SceneUnionRegister::openPopUpbeLongToUnion $_9: __func::__clone()
static uint32_t t_71C0D4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf6bb94u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71C0D4, "SceneUnionRegister::openPopUpbeLongToUnion $_9 __func::__clone()", t_71C0D4);

// SceneUnionRegister::openPopUpbeLongToUnion $_9: __func::__clone(__base*)
static void t_71C104(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bb94u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71C104, "SceneUnionRegister::openPopUpbeLongToUnion $_9 __func::__clone(__base*)", t_71C104);

// SceneUnionRegister::openPopUpbeLongToUnion $_9: __func::destroy
static void t_71C128(uint32_t self) {
}
RW_FUNCTION(0x71C128, "SceneUnionRegister::openPopUpbeLongToUnion $_9 __func::destroy", t_71C128);

// SceneUnionRegister::openPopUpbeLongToUnion $_9: __func::destroy_deallocate
static void t_71C12A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C12A, "SceneUnionRegister::openPopUpbeLongToUnion $_9 __func::destroy_deallocate", t_71C12A);

// SceneUnionRegister::openPopUpbeLongToUnion $_9: __func::target
static uint32_t t_71C16C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92e00u ? self + 4 : 0; }
RW_FUNCTION(0x71C16C, "SceneUnionRegister::openPopUpbeLongToUnion $_9 __func::target", t_71C16C);

// SceneUnionRegister::openPopUpbeLongToUnion $_9: __func::target_type
static uint32_t t_71C18C() { return 0xfa5744u; }
RW_FUNCTION(0x71C18C, "SceneUnionRegister::openPopUpbeLongToUnion $_9 __func::target_type", t_71C18C);

// SceneUnionRegister::openPopUpbeLongToUnion $_8: __func::~__func
static uint32_t t_71C1A0(uint32_t self) {
    st32(self, 0xf6bbc0u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x71C1A0, "SceneUnionRegister::openPopUpbeLongToUnion $_8 __func::~__func", t_71C1A0);

// SceneUnionRegister::openPopUpbeLongToUnion $_8: __func::~__func (deleting)
static void t_71C1DC(uint32_t self) {
    st32(self, 0xf6bbc0u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C1DC, "SceneUnionRegister::openPopUpbeLongToUnion $_8 __func::~__func (deleting)", t_71C1DC);

// SceneUnionRegister::openPopUpbeLongToUnion $_8: __func::__clone()
static uint32_t t_71C21C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf6bbc0u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 20, v5);
        st32(v1 + 16, v4);
        st32(v1 + 12, v3);
        st32(v1 + 8, v2);
        uint32_t v6 = ld32(self + 24);
        uint32_t v7 = ld32(self + 28);
        st32(v1 + 28, v7);
        st32(v1 + 24, v6);
        call(0x45c84d, {v1 + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x71C21C, "SceneUnionRegister::openPopUpbeLongToUnion $_8 __func::__clone()", t_71C21C);

// SceneUnionRegister::openPopUpbeLongToUnion $_8: __func::__clone(__base*)
static void t_71C280(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bbc0u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        uint32_t v5 = ld32(self + 24);
        uint32_t v6 = ld32(self + 28);
        st32(p + 28, v6);
        st32(p + 24, v5);
        call(0x45c84d, {p + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x71C280, "SceneUnionRegister::openPopUpbeLongToUnion $_8 __func::__clone(__base*)", t_71C280);

// SceneUnionRegister::openPopUpbeLongToUnion $_8: __func::destroy
static void t_71C2C8(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x71C2C8, "SceneUnionRegister::openPopUpbeLongToUnion $_8 __func::destroy", t_71C2C8);

// SceneUnionRegister::openPopUpbeLongToUnion $_8: __func::destroy_deallocate
static void t_71C2E8(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C2E8, "SceneUnionRegister::openPopUpbeLongToUnion $_8 __func::destroy_deallocate", t_71C2E8);

// SceneUnionRegister::openPopUpbeLongToUnion $_8: __func::target
static uint32_t t_71C4D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92f00u ? self + 8 : 0; }
RW_FUNCTION(0x71C4D8, "SceneUnionRegister::openPopUpbeLongToUnion $_8 __func::target", t_71C4D8);

// SceneUnionRegister::openPopUpbeLongToUnion $_8: __func::target_type
static uint32_t t_71C4FC() { return 0xfa5758u; }
RW_FUNCTION(0x71C4FC, "SceneUnionRegister::openPopUpbeLongToUnion $_8 __func::target_type", t_71C4FC);

// SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_: __func::~__func
static uint32_t t_71C510(uint32_t self) {
    st32(self, 0xf6bbecu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x71C510, "SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_ __func::~__func", t_71C510);

// SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_: __func::~__func (deleting)
static void t_71C54C(uint32_t self) {
    st32(self, 0xf6bbecu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C54C, "SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_ __func::~__func (deleting)", t_71C54C);

// SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_: __func::__clone()
static uint32_t t_71C58C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6bbecu);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x71C58C, "SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_ __func::__clone()", t_71C58C);

// SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_: __func::__clone(__base*)
static void t_71C5DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bbecu);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x71C5DC, "SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_ __func::__clone(__base*)", t_71C5DC);

// SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_: __func::destroy
static void t_71C610(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x71C610, "SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_ __func::destroy", t_71C610);

// SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_: __func::destroy_deallocate
static void t_71C630(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C630, "SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_ __func::destroy_deallocate", t_71C630);

// SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_: __func::target
static uint32_t t_71C668(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe93010u ? self + 8 : 0; }
RW_FUNCTION(0x71C668, "SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_ __func::target", t_71C668);

// SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_: __func::target_type
static uint32_t t_71C688() { return 0xfa576cu; }
RW_FUNCTION(0x71C688, "SceneUnionRegister::openPopUpbeLongToUnion $_8 UlvE_ __func::target_type", t_71C688);

// SceneUnionRegister::runOutAnimation $_7: __func::~__func
static uint32_t t_71C69C(uint32_t self) {
    st32(self, 0xf6bc18u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x71C69C, "SceneUnionRegister::runOutAnimation $_7 __func::~__func", t_71C69C);

// SceneUnionRegister::runOutAnimation $_7: __func::~__func (deleting)
static void t_71C6D8(uint32_t self) {
    st32(self, 0xf6bc18u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C6D8, "SceneUnionRegister::runOutAnimation $_7 __func::~__func (deleting)", t_71C6D8);

// SceneUnionRegister::runOutAnimation $_7: __func::__clone()
static uint32_t t_71C718(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6bc18u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x71C718, "SceneUnionRegister::runOutAnimation $_7 __func::__clone()", t_71C718);

// SceneUnionRegister::runOutAnimation $_7: __func::__clone(__base*)
static void t_71C768(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bc18u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x71C768, "SceneUnionRegister::runOutAnimation $_7 __func::__clone(__base*)", t_71C768);

// SceneUnionRegister::runOutAnimation $_7: __func::destroy
static void t_71C79C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x71C79C, "SceneUnionRegister::runOutAnimation $_7 __func::destroy", t_71C79C);

// SceneUnionRegister::runOutAnimation $_7: __func::destroy_deallocate
static void t_71C7BC(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C7BC, "SceneUnionRegister::runOutAnimation $_7 __func::destroy_deallocate", t_71C7BC);

// SceneUnionRegister::runOutAnimation $_7: __func::target
static uint32_t t_71C7F4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe93120u ? self + 8 : 0; }
RW_FUNCTION(0x71C7F4, "SceneUnionRegister::runOutAnimation $_7 __func::target", t_71C7F4);

// SceneUnionRegister::runOutAnimation $_7: __func::target_type
static uint32_t t_71C814() { return 0xfa5780u; }
RW_FUNCTION(0x71C814, "SceneUnionRegister::runOutAnimation $_7 __func::target_type", t_71C814);

// SceneUnionRegister::runInAnimation $_6: __func::~__func (deleting)
static void t_71C828(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C828, "SceneUnionRegister::runInAnimation $_6 __func::~__func (deleting)", t_71C828);

// SceneUnionRegister::runInAnimation $_6: __func::__clone()
static uint32_t t_71C830(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bc44u);
    }
    return v1;
}
RW_FUNCTION(0x71C830, "SceneUnionRegister::runInAnimation $_6 __func::__clone()", t_71C830);

// SceneUnionRegister::runInAnimation $_6: __func::__clone(__base*)
static void t_71C854(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bc44u);
    }
}
RW_FUNCTION(0x71C854, "SceneUnionRegister::runInAnimation $_6 __func::__clone(__base*)", t_71C854);

// SceneUnionRegister::runInAnimation $_6: __func::destroy
static void t_71C870(uint32_t self) {
}
RW_FUNCTION(0x71C870, "SceneUnionRegister::runInAnimation $_6 __func::destroy", t_71C870);

// SceneUnionRegister::runInAnimation $_6: __func::destroy_deallocate
static void t_71C872(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C872, "SceneUnionRegister::runInAnimation $_6 __func::destroy_deallocate", t_71C872);

// SceneUnionRegister::runInAnimation $_6: __func::target
static uint32_t t_71C87C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe931e0u ? self + 4 : 0; }
RW_FUNCTION(0x71C87C, "SceneUnionRegister::runInAnimation $_6 __func::target", t_71C87C);

// SceneUnionRegister::runInAnimation $_6: __func::target_type
static uint32_t t_71C89C() { return 0xfa5794u; }
RW_FUNCTION(0x71C89C, "SceneUnionRegister::runInAnimation $_6 __func::target_type", t_71C89C);

// SceneUnionRegister::startTutorialStage $_5: __func::~__func (deleting)
static void t_71C8B0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C8B0, "SceneUnionRegister::startTutorialStage $_5 __func::~__func (deleting)", t_71C8B0);

// SceneUnionRegister::startTutorialStage $_5: __func::__clone()
static uint32_t t_71C8B8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bc70u);
    }
    return v1;
}
RW_FUNCTION(0x71C8B8, "SceneUnionRegister::startTutorialStage $_5 __func::__clone()", t_71C8B8);

// SceneUnionRegister::startTutorialStage $_5: __func::__clone(__base*)
static void t_71C8DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bc70u);
    }
}
RW_FUNCTION(0x71C8DC, "SceneUnionRegister::startTutorialStage $_5 __func::__clone(__base*)", t_71C8DC);

// SceneUnionRegister::startTutorialStage $_5: __func::destroy
static void t_71C8F8(uint32_t self) {
}
RW_FUNCTION(0x71C8F8, "SceneUnionRegister::startTutorialStage $_5 __func::destroy", t_71C8F8);

// SceneUnionRegister::startTutorialStage $_5: __func::destroy_deallocate
static void t_71C8FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C8FA, "SceneUnionRegister::startTutorialStage $_5 __func::destroy_deallocate", t_71C8FA);

// SceneUnionRegister::startTutorialStage $_5: __func::target
static uint32_t t_71C910(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe93290u ? self + 4 : 0; }
RW_FUNCTION(0x71C910, "SceneUnionRegister::startTutorialStage $_5 __func::target", t_71C910);

// SceneUnionRegister::startTutorialStage $_5: __func::target_type
static uint32_t t_71C930() { return 0xfa57a8u; }
RW_FUNCTION(0x71C930, "SceneUnionRegister::startTutorialStage $_5 __func::target_type", t_71C930);

// SceneUnionRegister::callTutorialStageStartAPI $_4: __func::~__func (deleting)
static void t_71C944(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C944, "SceneUnionRegister::callTutorialStageStartAPI $_4 __func::~__func (deleting)", t_71C944);

// SceneUnionRegister::callTutorialStageStartAPI $_4: __func::__clone()
static uint32_t t_71C94C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bc9cu);
    }
    return v1;
}
RW_FUNCTION(0x71C94C, "SceneUnionRegister::callTutorialStageStartAPI $_4 __func::__clone()", t_71C94C);

// SceneUnionRegister::callTutorialStageStartAPI $_4: __func::__clone(__base*)
static void t_71C970(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bc9cu);
    }
}
RW_FUNCTION(0x71C970, "SceneUnionRegister::callTutorialStageStartAPI $_4 __func::__clone(__base*)", t_71C970);

// SceneUnionRegister::callTutorialStageStartAPI $_4: __func::destroy
static void t_71C98C(uint32_t self) {
}
RW_FUNCTION(0x71C98C, "SceneUnionRegister::callTutorialStageStartAPI $_4 __func::destroy", t_71C98C);

// SceneUnionRegister::callTutorialStageStartAPI $_4: __func::destroy_deallocate
static void t_71C98E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71C98E, "SceneUnionRegister::callTutorialStageStartAPI $_4 __func::destroy_deallocate", t_71C98E);

// SceneUnionRegister::callTutorialStageStartAPI $_4: __func::target
static uint32_t t_71CA38(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe93340u ? self + 4 : 0; }
RW_FUNCTION(0x71CA38, "SceneUnionRegister::callTutorialStageStartAPI $_4 __func::target", t_71CA38);

// SceneUnionRegister::callTutorialStageStartAPI $_4: __func::target_type
static uint32_t t_71CA58() { return 0xfa57bcu; }
RW_FUNCTION(0x71CA58, "SceneUnionRegister::callTutorialStageStartAPI $_4 __func::target_type", t_71CA58);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_: __func::~__func (deleting)
static void t_71CA6C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71CA6C, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ __func::~__func (deleting)", t_71CA6C);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_: __func::__clone()
static uint32_t t_71CA74(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bcc8u);
    }
    return v1;
}
RW_FUNCTION(0x71CA74, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ __func::__clone()", t_71CA74);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_: __func::__clone(__base*)
static void t_71CA98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bcc8u);
    }
}
RW_FUNCTION(0x71CA98, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ __func::__clone(__base*)", t_71CA98);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_: __func::destroy
static void t_71CAB4(uint32_t self) {
}
RW_FUNCTION(0x71CAB4, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ __func::destroy", t_71CAB4);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_: __func::destroy_deallocate
static void t_71CAB6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71CAB6, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ __func::destroy_deallocate", t_71CAB6);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_: __func::target
static uint32_t t_71CC20(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe93400u ? self + 4 : 0; }
RW_FUNCTION(0x71CC20, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ __func::target", t_71CC20);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_: __func::target_type
static uint32_t t_71CC40() { return 0xfa57d0u; }
RW_FUNCTION(0x71CC40, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ __func::target_type", t_71CC40);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_: __func::~__func (deleting)
static void t_71CC54(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71CC54, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_ __func::~__func (deleting)", t_71CC54);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_: __func::__clone()
static uint32_t t_71CC5C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bcf4u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71CC5C, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_ __func::__clone()", t_71CC5C);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_: __func::__clone(__base*)
static void t_71CC88(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bcf4u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x71CC88, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_ __func::__clone(__base*)", t_71CC88);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_: __func::destroy
static void t_71CCA8(uint32_t self) {
}
RW_FUNCTION(0x71CCA8, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_ __func::destroy", t_71CCA8);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_: __func::destroy_deallocate
static void t_71CCAA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71CCAA, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_ __func::destroy_deallocate", t_71CCAA);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_: __func::target
static uint32_t t_71CD88(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe934e0u ? self + 4 : 0; }
RW_FUNCTION(0x71CD88, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_ __func::target", t_71CD88);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_: __func::target_type
static uint32_t t_71CDA8() { return 0xfa57e4u; }
RW_FUNCTION(0x71CDA8, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE_ __func::target_type", t_71CDA8);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_: __func::~__func (deleting)
static void t_71CDBC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71CDBC, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_ __func::~__func (deleting)", t_71CDBC);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_: __func::__clone()
static uint32_t t_71CDC4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bd20u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71CDC4, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_ __func::__clone()", t_71CDC4);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_: __func::__clone(__base*)
static void t_71CDF0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bd20u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x71CDF0, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_ __func::__clone(__base*)", t_71CDF0);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_: __func::destroy
static void t_71CE10(uint32_t self) {
}
RW_FUNCTION(0x71CE10, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_ __func::destroy", t_71CE10);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_: __func::destroy_deallocate
static void t_71CE12(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71CE12, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_ __func::destroy_deallocate", t_71CE12);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_: __func::target
static uint32_t t_71CF30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe935d0u ? self + 4 : 0; }
RW_FUNCTION(0x71CF30, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_ __func::target", t_71CF30);

// SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_: __func::target_type
static uint32_t t_71CF50() { return 0xfa57f8u; }
RW_FUNCTION(0x71CF50, "SceneUnionRegister::callTutorialStageStartAPI $_4 UlbE_ UlbE0_ __func::target_type", t_71CF50);

// SceneUnionRegister::init $_3: __func::~__func (deleting)
static void t_71CF64(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71CF64, "SceneUnionRegister::init $_3 __func::~__func (deleting)", t_71CF64);

// SceneUnionRegister::init $_3: __func::__clone()
static uint32_t t_71CF6C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bd4cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71CF6C, "SceneUnionRegister::init $_3 __func::__clone()", t_71CF6C);

// SceneUnionRegister::init $_3: __func::__clone(__base*)
static void t_71CF98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bd4cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71CF98, "SceneUnionRegister::init $_3 __func::__clone(__base*)", t_71CF98);

// SceneUnionRegister::init $_3: __func::destroy
static void t_71CFB8(uint32_t self) {
}
RW_FUNCTION(0x71CFB8, "SceneUnionRegister::init $_3 __func::destroy", t_71CFB8);

// SceneUnionRegister::init $_3: __func::destroy_deallocate
static void t_71CFBA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71CFBA, "SceneUnionRegister::init $_3 __func::destroy_deallocate", t_71CFBA);

// SceneUnionRegister::init $_3: __func::target
static uint32_t t_71CFCC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe93690u ? self + 4 : 0; }
RW_FUNCTION(0x71CFCC, "SceneUnionRegister::init $_3 __func::target", t_71CFCC);

// SceneUnionRegister::init $_3: __func::target_type
static uint32_t t_71CFEC() { return 0xfa580cu; }
RW_FUNCTION(0x71CFEC, "SceneUnionRegister::init $_3 __func::target_type", t_71CFEC);

// SceneUnionRegister::init $_2: __func::~__func (deleting)
static void t_71D000(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71D000, "SceneUnionRegister::init $_2 __func::~__func (deleting)", t_71D000);

// SceneUnionRegister::init $_2: __func::__clone()
static uint32_t t_71D008(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bd78u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71D008, "SceneUnionRegister::init $_2 __func::__clone()", t_71D008);

// SceneUnionRegister::init $_2: __func::__clone(__base*)
static void t_71D034(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bd78u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71D034, "SceneUnionRegister::init $_2 __func::__clone(__base*)", t_71D034);

// SceneUnionRegister::init $_2: __func::destroy
static void t_71D054(uint32_t self) {
}
RW_FUNCTION(0x71D054, "SceneUnionRegister::init $_2 __func::destroy", t_71D054);

// SceneUnionRegister::init $_2: __func::destroy_deallocate
static void t_71D056(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71D056, "SceneUnionRegister::init $_2 __func::destroy_deallocate", t_71D056);

// SceneUnionRegister::init $_2: __func::target
static uint32_t t_71D060(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe93720u ? self + 4 : 0; }
RW_FUNCTION(0x71D060, "SceneUnionRegister::init $_2 __func::target", t_71D060);

// SceneUnionRegister::init $_2: __func::target_type
static uint32_t t_71D080() { return 0xfa5820u; }
RW_FUNCTION(0x71D080, "SceneUnionRegister::init $_2 __func::target_type", t_71D080);

// SceneUnionRegister::init $_1: __func::~__func (deleting)
static void t_71D094(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71D094, "SceneUnionRegister::init $_1 __func::~__func (deleting)", t_71D094);

// SceneUnionRegister::init $_1: __func::__clone()
static uint32_t t_71D09C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bda4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71D09C, "SceneUnionRegister::init $_1 __func::__clone()", t_71D09C);

// SceneUnionRegister::init $_1: __func::__clone(__base*)
static void t_71D0C8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bda4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71D0C8, "SceneUnionRegister::init $_1 __func::__clone(__base*)", t_71D0C8);

// SceneUnionRegister::init $_1: __func::destroy
static void t_71D0E8(uint32_t self) {
}
RW_FUNCTION(0x71D0E8, "SceneUnionRegister::init $_1 __func::destroy", t_71D0E8);

// SceneUnionRegister::init $_1: __func::destroy_deallocate
static void t_71D0EA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71D0EA, "SceneUnionRegister::init $_1 __func::destroy_deallocate", t_71D0EA);

// SceneUnionRegister::init $_1: __func::target
static uint32_t t_71D0FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe937b0u ? self + 4 : 0; }
RW_FUNCTION(0x71D0FC, "SceneUnionRegister::init $_1 __func::target", t_71D0FC);

// SceneUnionRegister::init $_1: __func::target_type
static uint32_t t_71D11C() { return 0xfa5834u; }
RW_FUNCTION(0x71D11C, "SceneUnionRegister::init $_1 __func::target_type", t_71D11C);

// SceneUnionRegister::init $_0: __func::~__func (deleting)
static void t_71D130(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71D130, "SceneUnionRegister::init $_0 __func::~__func (deleting)", t_71D130);

// SceneUnionRegister::init $_0: __func::__clone()
static uint32_t t_71D138(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bdd0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71D138, "SceneUnionRegister::init $_0 __func::__clone()", t_71D138);

// SceneUnionRegister::init $_0: __func::__clone(__base*)
static void t_71D164(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bdd0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71D164, "SceneUnionRegister::init $_0 __func::__clone(__base*)", t_71D164);

// SceneUnionRegister::init $_0: __func::destroy
static void t_71D184(uint32_t self) {
}
RW_FUNCTION(0x71D184, "SceneUnionRegister::init $_0 __func::destroy", t_71D184);

// SceneUnionRegister::init $_0: __func::destroy_deallocate
static void t_71D186(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71D186, "SceneUnionRegister::init $_0 __func::destroy_deallocate", t_71D186);

// SceneUnionRegister::init $_0: __func::target
static uint32_t t_71D198(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe93840u ? self + 4 : 0; }
RW_FUNCTION(0x71D198, "SceneUnionRegister::init $_0 __func::target", t_71D198);

// SceneUnionRegister::init $_0: __func::target_type
static uint32_t t_71D1B8() { return 0xfa5848u; }
RW_FUNCTION(0x71D1B8, "SceneUnionRegister::init $_0 __func::target_type", t_71D1B8);

// SceneUnionRegister::nextState $_15 UlbE_: __func::~__func (deleting)
static void t_71D1CC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71D1CC, "SceneUnionRegister::nextState $_15 UlbE_ __func::~__func (deleting)", t_71D1CC);

// SceneUnionRegister::nextState $_15 UlbE_: __func::__clone()
static uint32_t t_71D1D4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6bdfcu);
    }
    return v1;
}
RW_FUNCTION(0x71D1D4, "SceneUnionRegister::nextState $_15 UlbE_ __func::__clone()", t_71D1D4);

// SceneUnionRegister::nextState $_15 UlbE_: __func::__clone(__base*)
static void t_71D1F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6bdfcu);
    }
}
RW_FUNCTION(0x71D1F8, "SceneUnionRegister::nextState $_15 UlbE_ __func::__clone(__base*)", t_71D1F8);

// SceneUnionRegister::nextState $_15 UlbE_: __func::destroy
static void t_71D214(uint32_t self) {
}
RW_FUNCTION(0x71D214, "SceneUnionRegister::nextState $_15 UlbE_ __func::destroy", t_71D214);

// SceneUnionRegister::nextState $_15 UlbE_: __func::destroy_deallocate
static void t_71D216(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71D216, "SceneUnionRegister::nextState $_15 UlbE_ __func::destroy_deallocate", t_71D216);

// SceneUnionRegister::nextState $_15 UlbE_: __func::target
static uint32_t t_71D22C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe938e0u ? self + 4 : 0; }
RW_FUNCTION(0x71D22C, "SceneUnionRegister::nextState $_15 UlbE_ __func::target", t_71D22C);

// SceneUnionRegister::nextState $_15 UlbE_: __func::target_type
static uint32_t t_71D24C() { return 0xfa585cu; }
RW_FUNCTION(0x71D24C, "SceneUnionRegister::nextState $_15 UlbE_ __func::target_type", t_71D24C);

// ------------------------------------------------------------------------------------------------ lambda bodies
namespace {
constexpr uint32_t SafeBuffer_read = 0x45A269;              // (sb, dst, src, n, index)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_executeRaidGet = 0x4CBEC5;    // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_executeStageStart = 0x4CB22D; // (api, int stage, u64, u64, bool, const function&)
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_setIsStartTutorialStage = 0x4EACFF;
constexpr uint32_t AppState_resetClearTutorialPopupIDs = 0x4EB8B5;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;   // (mgr, bool)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;   // (node, std::function<void()>)
constexpr uint32_t SceneUtil_getStageResumptionStatus = 0x46F101;
constexpr uint32_t SceneUtil_updateTutorialPhase = 0x472BA1;         // (phase, function<void()>, bool, bool)
constexpr uint32_t FunctionVB_assign = 0x490109;            // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionVB_call = 0x474139;              // std::function<void(bool)>::operator()(bool)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t FadeTo_create = 0x86B3E1;                // (float duration, uint8 opacity)
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Widget_isTouchEnabled = 0x826665;
constexpr uint32_t start_movie = 0x6C4A55;                  // Layer* start_movie(int, bool)

constexpr uint32_t SUR_nextState = 0x7176ED;
constexpr uint32_t SUR_callTutorialStageStartAPI = 0x71792D;
constexpr uint32_t SUR_runOutAnimation = 0x717BB1;          // (scene, std::function<void()>)
constexpr uint32_t SUR_openInvitationJewelPop = 0x719371;   // (scene, std::function<void(bool)>)
constexpr uint32_t SUR_openPartyJewelPop = 0x7198D5;        // (scene, std::function<void(bool)>)

constexpr uint32_t POPUP_FADE_DURATION = 0xDEC3D0;          // a const float (0.15) the popups fade out with

// __func vtables of the lambdas the bodies create
constexpr uint32_t VT_nextState_12_a = 0xF6BAB8;            // $_12::{lambda(bool)#1}::{lambda(bool)#1}: [scene]
constexpr uint32_t VT_nextState_12_b = 0xF6BAE4;            // ... ::{lambda(bool)#1}: [scene]
constexpr uint32_t VT_belongToUnion_27_inner = 0xF6BB3C;    // $_27::{lambda()#1}: [parent, child, child]
constexpr uint32_t VT_belongToUnion_8_inner = 0xF6BBEC;     // $_8::{lambda()#1}: [function<void(bool)> at +8]
constexpr uint32_t VT_tutorialStart_4_a = 0xF6BCC8;         // $_4::{lambda(bool)#1}
constexpr uint32_t VT_tutorialStart_4_b = 0xF6BCF4;         // $_4::...::{lambda(bool)#1}: [bool]
constexpr uint32_t VT_tutorialStart_4_c = 0xF6BD20;         // $_4::...::{lambda(bool)#2}: [bool]

constexpr uint32_t API_USER_INFO = 0x64;                    // APIManager: user info (byte +0x10: tutorial flag)
constexpr uint32_t API_TUTORIAL_STAGE = 0x124;              // APIManager: record whose +0xC is the tutorial stage id
constexpr uint32_t API_CAMPAIGN_STAGES = 0xEC;              // APIManager: SafeBuffer* (88-byte records)
constexpr uint32_t SM_LOADING_BLOCKED = 0x4CC;              // SceneManager byte
constexpr uint32_t SM_ON_FINISH_LOADING = 0x548;            // SceneManager std::function<void(bool)>
constexpr uint32_t WIDGET_SET_TOUCH_ENABLED = 0x29C;        // ui::Widget::setTouchEnabled(bool) (vtable offset)
constexpr uint32_t NODE_REMOVE_CHILD = 0x12C;               // Node::removeChild(child, cleanup)
constexpr uint32_t PHASE_UNION_DONE = 995, PHASE_UNION_FIRST = 50;

// an inline std::function holding a lambda with one captured bool
uint32_t newFunctionBool(Frame &f, uint32_t vtable, uint8_t value) {
    uint32_t fo = f.alloc(20);
    st32(fo, vtable);
    st8(fo + 4, value);
    st32(fo + 16, fo);
    return fo;
}

// the popup close: its close animation (removing it through `remover` captures) and the shade fading out
void closePopup(uint32_t popup, uint32_t remover, std::initializer_list<uint32_t> removerCaptures) {
    Frame f;
    uint32_t fo = newFunction(f, remover, removerCaptures);
    call(Node_runAction, {popup, call(SceneUtil_createClosePopupAnimation, {popup, fo})});
    functionDestroy(fo);
}

}  // namespace

struct SceneUnionRegisterLambdas {
    // nextState $_12::{lambda(bool)#2}, ...{lambda(bool)#1}^3, init $_3: continue the registration flow
    static void nextState(uint32_t functor) { call(SUR_nextState, {ld32(functor + 4)}); }

    // nextState $_12::{lambda(bool)#1}: the invitation jewel popup, then the party jewel popup
    static void openInvitationJewelPop(uint32_t functor) {
        uint32_t scene = ld32(functor + 4);
        Frame f;
        uint32_t next = newFunction(f, VT_nextState_12_a, {scene});
        call(SUR_openInvitationJewelPop, {scene, next});
        functionDestroy(next);
    }
    static void openPartyJewelPop(uint32_t functor) {
        uint32_t scene = ld32(functor + 4);
        Frame f;
        uint32_t next = newFunction(f, VT_nextState_12_b, {scene});
        call(SUR_openPartyJewelPop, {scene, next});
        functionDestroy(next);
    }

    // openPopUpbeLongToUnion $_11 (bool ok): cancelled - close the popup
    // captures: +4 the popup, +8 its parent, +0xC the shade
    static void belongToUnionClosed(uint32_t functor, const uint8_t *ok) {
        if (*ok) return;
        call(AudioPlaySystemSECancel, {});
        uint32_t popup = ld32(functor + 4);
        closePopup(popup, VT_belongToUnion_27_inner, {ld32(functor + 8), ld32(functor + 0xC), popup});
        uint32_t duration = ld32(POPUP_FADE_DURATION);
        uint32_t shade = ld32(functor + 0xC);
        call(Node_runAction, {shade, call(FadeTo_create, {duration, 0})});
    }

    // openPopUpbeLongToUnion $_27::{lambda()#1}: remove the popup and its shade from the parent
    // captures: +4 parent, +8 / +0xC the children
    static void removePopup(uint32_t functor) {
        uint32_t parent = ld32(functor + 4);
        vcall(parent, NODE_REMOVE_CHILD, {parent, ld32(functor + 8), 1});
        parent = ld32(functor + 4);
        vcall(parent, NODE_REMOVE_CHILD, {parent, ld32(functor + 0xC), 1});
    }

    // openPopUpbeLongToUnion $_10: the popup takes taps while its button is touch enabled
    static uint32_t popupTouchable(uint32_t functor) { return call(Widget_isTouchEnabled, {ld32(functor + 4)}); }

    // openPopUpbeLongToUnion $_9 (Ref*, TouchEventType): cancel released - close the popup, buttons off
    static void tapCancel(uint32_t functor, uint32_t sender, const int32_t *type) {
        (void)sender;
        if (*type != 2) return;
        call(SceneManager_closeCurrentPopup, {sceneManager(), 0});
        uint32_t button1 = ld32(functor + 4);
        vcall(button1, WIDGET_SET_TOUCH_ENABLED, {button1, 0});
        uint32_t button2 = ld32(functor + 8);
        vcall(button2, WIDGET_SET_TOUCH_ENABLED, {button2, 0});
    }

    // openPopUpbeLongToUnion $_8 (Ref*, TouchEventType): OK released - close the popup, then leave the scene
    // with the captured callback(true) at the end of the out animation.
    // captures: +8 / +0xC buttons, +0x10 the popup, +0x14 its parent, +0x18 the shade, +0x1C the scene,
    // +0x20 the callback (std::function<void(bool)>)
    static void tapOk(uint32_t functor, uint32_t sender, const int32_t *type) {
        (void)sender;
        if (*type != 2) return;
        uint32_t scene = ld32(functor + 0x1C);
        call(AudioPlaySystemSEDecide, {});
        call(SceneManager_closeCurrentPopup, {sceneManager(), 1});
        uint32_t button1 = ld32(functor + 8);
        vcall(button1, WIDGET_SET_TOUCH_ENABLED, {button1, 0});
        uint32_t button2 = ld32(functor + 0xC);
        vcall(button2, WIDGET_SET_TOUCH_ENABLED, {button2, 0});
        uint32_t popup = ld32(functor + 0x10);
        closePopup(popup, VT_belongToUnion_27_inner, {ld32(functor + 0x14), ld32(functor + 0x18), popup});
        uint32_t duration = ld32(POPUP_FADE_DURATION);
        uint32_t shade = ld32(functor + 0x18);
        call(Node_runAction, {shade, call(FadeTo_create, {duration, 0})});
        Frame f;
        uint32_t copy = f.alloc(24);
        uint32_t done = f.alloc(24);
        call(fn::function_vb_copy, {copy, functor + 0x20});
        heapFunction(done, VT_belongToUnion_8_inner, 0x20, 8, fn::function_vb_move, copy);
        call(SUR_runOutAnimation, {scene, done});
        functionDestroy(done);
        functionDestroy(copy);
    }

    // openPopUpbeLongToUnion $_8::{lambda()#1}: callback(true)
    static void callbackTrue(uint32_t functor) { call(FunctionVB_call, {functor + 8, 1}); }

    // runOutAnimation $_7: the captured `done`
    static void runDone(uint32_t functor) { call(fn::function_vv_call, {functor + 8}); }

    // runInAnimation $_6, init $_2: nothing
    static void nothing(uint32_t functor) { (void)functor; }

    // startTutorialStage $_5: the layer factory of the tutorial movie
    static uint32_t tutorialMovie(uint32_t functor) {
        (void)functor;
        return call(start_movie, {0, 0});
    }

    // callTutorialStageStartAPI $_4 (bool ok): fetch the raid state next
    static void tutorialStageStart(uint32_t functor, const uint8_t *ok) {
        (void)functor;
        if (!*ok) return;
        uint32_t api = call(APIManager_getInstance, {});
        Frame f;
        uint32_t next = newFunction(f, VT_tutorialStart_4_a, {});
        call(APIManager_executeRaidGet, {api, next});
        functionDestroy(next);
    }

    // callTutorialStageStartAPI $_4::{lambda(bool)#1} (bool ok): start the tutorial stage
    static void tutorialRaidGot(uint32_t functor, const uint8_t *ok) {
        (void)functor;
        if (!*ok) return;
        uint32_t api = call(APIManager_getInstance, {});
        Frame f;
        uint32_t record = f.alloc(0x58);
        uint32_t sb = ld32(call(APIManager_getInstance, {}) + API_CAMPAIGN_STAGES);
        call(SafeBuffer_read, {sb, record, ld32(sb + 0xC), 0x58, 0});
        int32_t status = int32_t(call(SceneUtil_getStageResumptionStatus, {}));
        uint32_t mgr = sceneManager();
        uint32_t onLoaded = newFunctionBool(f, VT_tutorialStart_4_c, 1);
        st8(mgr + SM_LOADING_BLOCKED, 1);
        call(FunctionVB_assign, {mgr + SM_ON_FINISH_LOADING, onLoaded});
        functionDestroy(onLoaded);
        startStage(api, record, status, 1);
        call(AppState_setIsStartTutorialStage, {call(AppState_getInstance, {}), 1});
    }

    // callTutorialStageStartAPI $_4::...::{lambda(bool)#1} [bool first]: the stage started - tutorial phase 995
    // when the user already belongs to a union (first time: phase 50)
    static void tutorialPhase(uint32_t functor, const uint8_t *arg) {
        (void)arg;
        if (!ld8(functor + 4)) return;
        uint32_t info = ld32(call(APIManager_getInstance, {}) + API_USER_INFO);
        Frame f;
        uint32_t none = newEmptyFunction(f);
        if (info && ld8(info + 0x10)) call(SceneUtil_updateTutorialPhase, {PHASE_UNION_DONE, none, 1, 0});
        else call(SceneUtil_updateTutorialPhase, {PHASE_UNION_FIRST, none, 0, 0});
        functionDestroy(none);
        call(AppState_resetClearTutorialPopupIDs, {call(AppState_getInstance, {})});
    }

    // callTutorialStageStartAPI $_4::...::{lambda(bool)#2} [bool flag]: after loading - start the stage again
    // unless the request is being retried
    static void tutorialRetry(uint32_t functor, const uint8_t *arg) {
        (void)arg;
        if (call(APIManager_needsRetry, {call(APIManager_getInstance, {})})) return;
        uint32_t api = call(APIManager_getInstance, {});
        Frame f;
        uint32_t record = f.alloc(0x58);
        uint32_t sb = ld32(call(APIManager_getInstance, {}) + API_CAMPAIGN_STAGES);
        call(SafeBuffer_read, {sb, record, ld32(sb + 0xC), 0x58, 0});
        int32_t status = int32_t(call(SceneUtil_getStageResumptionStatus, {}));
        startStage(api, record, status, ld8(functor + 4));
    }

    // init $_1 / $_0: stop / start the scene's update
    static void unscheduleUpdate(uint32_t functor) { call(Node_unscheduleUpdate, {ld32(functor + 4)}); }
    static void scheduleUpdate(uint32_t functor) { call(Node_scheduleUpdate, {ld32(functor + 4)}); }

    // nextState $_15::{lambda(bool)#1} (bool ok): start the tutorial stage
    // (callTutorialStageStartAPI gets the functor in r0, as the original passes it on unchanged)
    static void startTutorial(uint32_t functor, const uint8_t *ok) {
        if (*ok) call(SUR_callTutorialStageStartAPI, {functor});
    }

private:
    // APIManager::executeStageStart(stage, 0, record id (u64), false, {lambda(bool) [flag]}): the tutorial stage
    // id and the campaign record's first two words unless the resumption status is 1 (then all zero)
    static void startStage(uint32_t api, uint32_t record, int32_t status, uint8_t flag) {
        uint32_t api2 = call(APIManager_getInstance, {});
        uint32_t stage = 0, lo = 0, hi = 0;
        if (status != 1) {
            stage = ld32(ld32(api + API_TUTORIAL_STAGE) + 0xC);
            lo = ld32(record);
            hi = ld32(record + 4);
        }
        Frame f;
        uint32_t done = newFunctionBool(f, VT_tutorialStart_4_b, flag);
        call(APIManager_executeStageStart, {api2, stage, 0, 0, lo, hi, 0, done});
        functionDestroy(done);
    }
};

using UL = SceneUnionRegisterLambdas;
RW_FUNCTION(0x71BAA6, "SceneUnionRegister::nextState::$_12::{lambda(bool)#2}::operator()", UL::nextState);
RW_FUNCTION(0x71BB44, "SceneUnionRegister::nextState::$_12::{lambda(bool)#1}::operator()", UL::openInvitationJewelPop);
RW_FUNCTION(0x71BC6C, "SceneUnionRegister::nextState::$_12::{lambda(bool)#1}::{lambda(bool)#1}::operator()", UL::openPartyJewelPop);
RW_FUNCTION(0x71BD92, "SceneUnionRegister::nextState::$_12::{lambda(bool)#1}^3::operator()", UL::nextState);
RW_FUNCTION(0x71BE3C, "SceneUnionRegister::openPopUpbeLongToUnion::$_11::operator()(bool)", UL::belongToUnionClosed);
RW_FUNCTION(0x71BFD2, "SceneUnionRegister::openPopUpbeLongToUnion::$_27::{lambda()#1}::operator()", UL::removePopup);
RW_FUNCTION(0x71C08E, "SceneUnionRegister::openPopUpbeLongToUnion::$_10::operator()", UL::popupTouchable);
RW_FUNCTION(0x71C132, "SceneUnionRegister::openPopUpbeLongToUnion::$_9::operator()", UL::tapCancel);
RW_FUNCTION(0x71C314, "SceneUnionRegister::openPopUpbeLongToUnion::$_8::operator()", UL::tapOk);
RW_FUNCTION(0x71C65C, "SceneUnionRegister::openPopUpbeLongToUnion::$_8::{lambda()#1}::operator()", UL::callbackTrue);
RW_FUNCTION(0x71C7E8, "SceneUnionRegister::runOutAnimation::$_7::operator()", UL::runDone);
RW_FUNCTION(0x71C87A, "SceneUnionRegister::runInAnimation::$_6::operator()", UL::nothing);
RW_FUNCTION(0x71C902, "SceneUnionRegister::startTutorialStage::$_5::operator()", UL::tutorialMovie);
RW_FUNCTION(0x71C998, "SceneUnionRegister::callTutorialStageStartAPI::$_4::operator()(bool)", UL::tutorialStageStart);
RW_FUNCTION(0x71CAC0, "SceneUnionRegister::callTutorialStageStartAPI::$_4::{lambda(bool)#1}::operator()", UL::tutorialRaidGot);
RW_FUNCTION(0x71CCB4, "SceneUnionRegister::callTutorialStageStartAPI::$_4::{lambda(bool)#1}^2::operator()", UL::tutorialPhase);
RW_FUNCTION(0x71CE1C, "SceneUnionRegister::callTutorialStageStartAPI::$_4::{lambda(bool)#1}::{lambda(bool)#2}::operator()", UL::tutorialRetry);
RW_FUNCTION(0x71CFC2, "SceneUnionRegister::init::$_3::operator()", UL::nextState);
RW_FUNCTION(0x71D05E, "SceneUnionRegister::init::$_2::operator()", UL::nothing);
RW_FUNCTION(0x71D0F2, "SceneUnionRegister::init::$_1::operator()", UL::unscheduleUpdate);
RW_FUNCTION(0x71D18E, "SceneUnionRegister::init::$_0::operator()", UL::scheduleUpdate);
RW_FUNCTION(0x71D21E, "SceneUnionRegister::nextState::$_15::{lambda(bool)#1}::operator()", UL::startTutorial);
