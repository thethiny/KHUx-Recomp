// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneRankingReward lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneRankingReward::backScene $_14: __func::~__func (deleting)
static void t_710740(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710740, "SceneRankingReward::backScene $_14 __func::~__func (deleting)", t_710740);

// SceneRankingReward::backScene $_14: __func::__clone()
static uint32_t t_710748(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ad50u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x710748, "SceneRankingReward::backScene $_14 __func::__clone()", t_710748);

// SceneRankingReward::backScene $_14: __func::__clone(__base*)
static void t_710774(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ad50u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x710774, "SceneRankingReward::backScene $_14 __func::__clone(__base*)", t_710774);

// SceneRankingReward::backScene $_14: __func::destroy
static void t_710794(uint32_t self) {
}
RW_FUNCTION(0x710794, "SceneRankingReward::backScene $_14 __func::destroy", t_710794);

// SceneRankingReward::backScene $_14: __func::destroy_deallocate
static void t_710796(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710796, "SceneRankingReward::backScene $_14 __func::destroy_deallocate", t_710796);

// SceneRankingReward::backScene $_14: __func::target
static uint32_t t_7109FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8e4d0u ? self + 4 : 0; }
RW_FUNCTION(0x7109FC, "SceneRankingReward::backScene $_14 __func::target", t_7109FC);

// SceneRankingReward::backScene $_14: __func::target_type
static uint32_t t_710A1C() { return 0xfa3d9cu; }
RW_FUNCTION(0x710A1C, "SceneRankingReward::backScene $_14 __func::target_type", t_710A1C);

// SceneRankingReward::backScene $_14 UlvE0_: __func::~__func (deleting)
static void t_710A30(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710A30, "SceneRankingReward::backScene $_14 UlvE0_ __func::~__func (deleting)", t_710A30);

// SceneRankingReward::backScene $_14 UlvE0_: __func::__clone()
static uint32_t t_710A38(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ad7cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x710A38, "SceneRankingReward::backScene $_14 UlvE0_ __func::__clone()", t_710A38);

// SceneRankingReward::backScene $_14 UlvE0_: __func::__clone(__base*)
static void t_710A64(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ad7cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x710A64, "SceneRankingReward::backScene $_14 UlvE0_ __func::__clone(__base*)", t_710A64);

// SceneRankingReward::backScene $_14 UlvE0_: __func::destroy
static void t_710A84(uint32_t self) {
}
RW_FUNCTION(0x710A84, "SceneRankingReward::backScene $_14 UlvE0_ __func::destroy", t_710A84);

// SceneRankingReward::backScene $_14 UlvE0_: __func::destroy_deallocate
static void t_710A86(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710A86, "SceneRankingReward::backScene $_14 UlvE0_ __func::destroy_deallocate", t_710A86);

// SceneRankingReward::backScene $_14 UlvE0_: __func::target
static uint32_t t_710AA8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8e580u ? self + 4 : 0; }
RW_FUNCTION(0x710AA8, "SceneRankingReward::backScene $_14 UlvE0_ __func::target", t_710AA8);

// SceneRankingReward::backScene $_14 UlvE0_: __func::target_type
static uint32_t t_710AC8() { return 0xfa3db0u; }
RW_FUNCTION(0x710AC8, "SceneRankingReward::backScene $_14 UlvE0_ __func::target_type", t_710AC8);

// SceneRankingReward::backScene $_14 UlbE_: __func::~__func (deleting)
static void t_710ADC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710ADC, "SceneRankingReward::backScene $_14 UlbE_ __func::~__func (deleting)", t_710ADC);

// SceneRankingReward::backScene $_14 UlbE_: __func::__clone()
static uint32_t t_710AE4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ada8u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x710AE4, "SceneRankingReward::backScene $_14 UlbE_ __func::__clone()", t_710AE4);

// SceneRankingReward::backScene $_14 UlbE_: __func::__clone(__base*)
static void t_710B10(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ada8u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x710B10, "SceneRankingReward::backScene $_14 UlbE_ __func::__clone(__base*)", t_710B10);

// SceneRankingReward::backScene $_14 UlbE_: __func::destroy
static void t_710B30(uint32_t self) {
}
RW_FUNCTION(0x710B30, "SceneRankingReward::backScene $_14 UlbE_ __func::destroy", t_710B30);

// SceneRankingReward::backScene $_14 UlbE_: __func::destroy_deallocate
static void t_710B32(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710B32, "SceneRankingReward::backScene $_14 UlbE_ __func::destroy_deallocate", t_710B32);

// SceneRankingReward::backScene $_14 UlbE_: __func::target
static uint32_t t_710BE0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8e630u ? self + 4 : 0; }
RW_FUNCTION(0x710BE0, "SceneRankingReward::backScene $_14 UlbE_ __func::target", t_710BE0);

// SceneRankingReward::backScene $_14 UlbE_: __func::target_type
static uint32_t t_710C00() { return 0xfa3dc4u; }
RW_FUNCTION(0x710C00, "SceneRankingReward::backScene $_14 UlbE_ __func::target_type", t_710C00);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_: __func::~__func (deleting)
static void t_710C14(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710C14, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ __func::~__func (deleting)", t_710C14);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_: __func::__clone()
static uint32_t t_710C1C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6add4u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x710C1C, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ __func::__clone()", t_710C1C);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_: __func::__clone(__base*)
static void t_710C48(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6add4u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x710C48, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ __func::__clone(__base*)", t_710C48);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_: __func::destroy
static void t_710C68(uint32_t self) {
}
RW_FUNCTION(0x710C68, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ __func::destroy", t_710C68);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_: __func::destroy_deallocate
static void t_710C6A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710C6A, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ __func::destroy_deallocate", t_710C6A);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_: __func::target
static uint32_t t_710D18(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8e6f0u ? self + 4 : 0; }
RW_FUNCTION(0x710D18, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ __func::target", t_710D18);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_: __func::target_type
static uint32_t t_710D38() { return 0xfa3dd8u; }
RW_FUNCTION(0x710D38, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ __func::target_type", t_710D38);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_710D4C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710D4C, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_ __func::~__func (deleting)", t_710D4C);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_710D54(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ae00u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x710D54, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_ __func::__clone()", t_710D54);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_710D80(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ae00u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x710D80, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_ __func::__clone(__base*)", t_710D80);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_: __func::destroy
static void t_710DA0(uint32_t self) {
}
RW_FUNCTION(0x710DA0, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_ __func::destroy", t_710DA0);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_710DA2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710DA2, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_ __func::destroy_deallocate", t_710DA2);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_: __func::target
static uint32_t t_710DAC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8e7d0u ? self + 4 : 0; }
RW_FUNCTION(0x710DAC, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_ __func::target", t_710DAC);

// SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_710DCC() { return 0xfa3decu; }
RW_FUNCTION(0x710DCC, "SceneRankingReward::backScene $_14 UlbE_ UlbE_ UlbE_ __func::target_type", t_710DCC);

// SceneRankingReward::runOutAnimationMain $_13: __func::~__func
static uint32_t t_710DE0(uint32_t self) {
    st32(self, 0xf6ae2cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x710DE0, "SceneRankingReward::runOutAnimationMain $_13 __func::~__func", t_710DE0);

// SceneRankingReward::runOutAnimationMain $_13: __func::~__func (deleting)
static void t_710E1C(uint32_t self) {
    st32(self, 0xf6ae2cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710E1C, "SceneRankingReward::runOutAnimationMain $_13 __func::~__func (deleting)", t_710E1C);

// SceneRankingReward::runOutAnimationMain $_13: __func::__clone()
static uint32_t t_710E5C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6ae2cu);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x710E5C, "SceneRankingReward::runOutAnimationMain $_13 __func::__clone()", t_710E5C);

// SceneRankingReward::runOutAnimationMain $_13: __func::__clone(__base*)
static void t_710EAC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ae2cu);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x710EAC, "SceneRankingReward::runOutAnimationMain $_13 __func::__clone(__base*)", t_710EAC);

// SceneRankingReward::runOutAnimationMain $_13: __func::destroy
static void t_710EE0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x710EE0, "SceneRankingReward::runOutAnimationMain $_13 __func::destroy", t_710EE0);

// SceneRankingReward::runOutAnimationMain $_13: __func::destroy_deallocate
static void t_710F00(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710F00, "SceneRankingReward::runOutAnimationMain $_13 __func::destroy_deallocate", t_710F00);

// SceneRankingReward::runOutAnimationMain $_13: __func::target
static uint32_t t_710F38(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8e8a0u ? self + 8 : 0; }
RW_FUNCTION(0x710F38, "SceneRankingReward::runOutAnimationMain $_13 __func::target", t_710F38);

// SceneRankingReward::runOutAnimationMain $_13: __func::target_type
static uint32_t t_710F58() { return 0xfa3e00u; }
RW_FUNCTION(0x710F58, "SceneRankingReward::runOutAnimationMain $_13 __func::target_type", t_710F58);

// SceneRankingReward::runInAnimation $_12: __func::~__func (deleting)
static void t_710F6C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710F6C, "SceneRankingReward::runInAnimation $_12 __func::~__func (deleting)", t_710F6C);

// SceneRankingReward::runInAnimation $_12: __func::__clone()
static uint32_t t_710F74(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ae58u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x710F74, "SceneRankingReward::runInAnimation $_12 __func::__clone()", t_710F74);

// SceneRankingReward::runInAnimation $_12: __func::__clone(__base*)
static void t_710FA0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ae58u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x710FA0, "SceneRankingReward::runInAnimation $_12 __func::__clone(__base*)", t_710FA0);

// SceneRankingReward::runInAnimation $_12: __func::destroy
static void t_710FC0(uint32_t self) {
}
RW_FUNCTION(0x710FC0, "SceneRankingReward::runInAnimation $_12 __func::destroy", t_710FC0);

// SceneRankingReward::runInAnimation $_12: __func::destroy_deallocate
static void t_710FC2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x710FC2, "SceneRankingReward::runInAnimation $_12 __func::destroy_deallocate", t_710FC2);

// SceneRankingReward::runInAnimation $_12: __func::target
static uint32_t t_710FE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8e960u ? self + 4 : 0; }
RW_FUNCTION(0x710FE4, "SceneRankingReward::runInAnimation $_12 __func::target", t_710FE4);

// SceneRankingReward::runInAnimation $_12: __func::target_type
static uint32_t t_711004() { return 0xfa3e14u; }
RW_FUNCTION(0x711004, "SceneRankingReward::runInAnimation $_12 __func::target_type", t_711004);

// SceneRankingReward::initRankingScrollVew $_11: __func::~__func (deleting)
static void t_711018(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711018, "SceneRankingReward::initRankingScrollVew $_11 __func::~__func (deleting)", t_711018);

// SceneRankingReward::initRankingScrollVew $_11: __func::__clone()
static uint32_t t_711020(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ae84u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x711020, "SceneRankingReward::initRankingScrollVew $_11 __func::__clone()", t_711020);

// SceneRankingReward::initRankingScrollVew $_11: __func::__clone(__base*)
static void t_71104C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ae84u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71104C, "SceneRankingReward::initRankingScrollVew $_11 __func::__clone(__base*)", t_71104C);

// SceneRankingReward::initRankingScrollVew $_11: __func::destroy
static void t_71106C(uint32_t self) {
}
RW_FUNCTION(0x71106C, "SceneRankingReward::initRankingScrollVew $_11 __func::destroy", t_71106C);

// SceneRankingReward::initRankingScrollVew $_11: __func::destroy_deallocate
static void t_71106E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71106E, "SceneRankingReward::initRankingScrollVew $_11 __func::destroy_deallocate", t_71106E);

// SceneRankingReward::initRankingScrollVew $_11: __func::target
static uint32_t t_711098(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8ea10u ? self + 4 : 0; }
RW_FUNCTION(0x711098, "SceneRankingReward::initRankingScrollVew $_11 __func::target", t_711098);

// SceneRankingReward::initRankingScrollVew $_11: __func::target_type
static uint32_t t_7110B8() { return 0xfa3e28u; }
RW_FUNCTION(0x7110B8, "SceneRankingReward::initRankingScrollVew $_11 __func::target_type", t_7110B8);

// SceneRankingReward::initRankingScrollVew $_10: __func::~__func (deleting)
static void t_7110CC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7110CC, "SceneRankingReward::initRankingScrollVew $_10 __func::~__func (deleting)", t_7110CC);

// SceneRankingReward::initRankingScrollVew $_10: __func::__clone()
static uint32_t t_7110D4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6aeb0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7110D4, "SceneRankingReward::initRankingScrollVew $_10 __func::__clone()", t_7110D4);

// SceneRankingReward::initRankingScrollVew $_10: __func::__clone(__base*)
static void t_711100(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6aeb0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x711100, "SceneRankingReward::initRankingScrollVew $_10 __func::__clone(__base*)", t_711100);

// SceneRankingReward::initRankingScrollVew $_10: __func::destroy
static void t_711120(uint32_t self) {
}
RW_FUNCTION(0x711120, "SceneRankingReward::initRankingScrollVew $_10 __func::destroy", t_711120);

// SceneRankingReward::initRankingScrollVew $_10: __func::destroy_deallocate
static void t_711122(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711122, "SceneRankingReward::initRankingScrollVew $_10 __func::destroy_deallocate", t_711122);

// SceneRankingReward::initRankingScrollVew $_10: __func::target
static uint32_t t_7111C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8eae0u ? self + 4 : 0; }
RW_FUNCTION(0x7111C8, "SceneRankingReward::initRankingScrollVew $_10 __func::target", t_7111C8);

// SceneRankingReward::initRankingScrollVew $_10: __func::target_type
static uint32_t t_7111E8() { return 0xfa3e3cu; }
RW_FUNCTION(0x7111E8, "SceneRankingReward::initRankingScrollVew $_10 __func::target_type", t_7111E8);

// SceneRankingReward::init $_9: __func::~__func (deleting)
static void t_7111FC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7111FC, "SceneRankingReward::init $_9 __func::~__func (deleting)", t_7111FC);

// SceneRankingReward::init $_9: __func::__clone()
static uint32_t t_711204(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6aedcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x711204, "SceneRankingReward::init $_9 __func::__clone()", t_711204);
