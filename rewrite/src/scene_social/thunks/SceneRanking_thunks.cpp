// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneRanking lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneRanking::backScene $_21: __func::~__func
static uint32_t t_701118(uint32_t self) {
    st32(self, 0xf6a0f0u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x701118, "SceneRanking::backScene $_21 __func::~__func", t_701118);

// SceneRanking::backScene $_21: __func::~__func (deleting)
static void t_701154(uint32_t self) {
    st32(self, 0xf6a0f0u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701154, "SceneRanking::backScene $_21 __func::~__func (deleting)", t_701154);

// SceneRanking::backScene $_21: __func::destroy
static void t_70122C(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x70122C, "SceneRanking::backScene $_21 __func::destroy", t_70122C);

// SceneRanking::backScene $_21: __func::destroy_deallocate
static void t_70124C(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x70124C, "SceneRanking::backScene $_21 __func::destroy_deallocate", t_70124C);

// SceneRanking::backScene $_21: __func::target
static uint32_t t_7012B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8ac20u ? self + 8 : 0; }
RW_FUNCTION(0x7012B0, "SceneRanking::backScene $_21 __func::target", t_7012B0);

// SceneRanking::backScene $_21: __func::target_type
static uint32_t t_7012D0() { return 0xfa315cu; }
RW_FUNCTION(0x7012D0, "SceneRanking::backScene $_21 __func::target_type", t_7012D0);

// SceneRanking::openRankingRewardPopup $_20: __func::~__func (deleting)
static void t_7012E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7012E4, "SceneRanking::openRankingRewardPopup $_20 __func::~__func (deleting)", t_7012E4);

// SceneRanking::openRankingRewardPopup $_20: __func::__clone()
static uint32_t t_7012EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6a11cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7012EC, "SceneRanking::openRankingRewardPopup $_20 __func::__clone()", t_7012EC);

// SceneRanking::openRankingRewardPopup $_20: __func::__clone(__base*)
static void t_701318(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a11cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x701318, "SceneRanking::openRankingRewardPopup $_20 __func::__clone(__base*)", t_701318);

// SceneRanking::openRankingRewardPopup $_20: __func::destroy
static void t_701338(uint32_t self) {
}
RW_FUNCTION(0x701338, "SceneRanking::openRankingRewardPopup $_20 __func::destroy", t_701338);

// SceneRanking::openRankingRewardPopup $_20: __func::destroy_deallocate
static void t_70133A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x70133A, "SceneRanking::openRankingRewardPopup $_20 __func::destroy_deallocate", t_70133A);

// SceneRanking::openRankingRewardPopup $_20: __func::target
static uint32_t t_701450(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8acd0u ? self + 4 : 0; }
RW_FUNCTION(0x701450, "SceneRanking::openRankingRewardPopup $_20 __func::target", t_701450);

// SceneRanking::openRankingRewardPopup $_20: __func::target_type
static uint32_t t_701470() { return 0xfa3170u; }
RW_FUNCTION(0x701470, "SceneRanking::openRankingRewardPopup $_20 __func::target_type", t_701470);

// SceneRanking::openRankingRewardPopup $_19: __func::target
static uint32_t t_701844(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8add0u ? self + 8 : 0; }
RW_FUNCTION(0x701844, "SceneRanking::openRankingRewardPopup $_19 __func::target", t_701844);

// SceneRanking::openRankingRewardPopup $_19: __func::target_type
static uint32_t t_701868() { return 0xfa3184u; }
RW_FUNCTION(0x701868, "SceneRanking::openRankingRewardPopup $_19 __func::target_type", t_701868);

// SceneRanking::openRankingRewardPopup $_19 UlvE_: __func::target
static uint32_t t_701B64(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8aee0u ? self + 8 : 0; }
RW_FUNCTION(0x701B64, "SceneRanking::openRankingRewardPopup $_19 UlvE_ __func::target", t_701B64);

// SceneRanking::openRankingRewardPopup $_19 UlvE_: __func::target_type
static uint32_t t_701B88() { return 0xfa3198u; }
RW_FUNCTION(0x701B88, "SceneRanking::openRankingRewardPopup $_19 UlvE_ __func::target_type", t_701B88);

// SceneRanking::runOutAnimationMain $_17: __func::~__func
static uint32_t t_701BCC(uint32_t self) {
    st32(self, 0xf6a1a0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x701BCC, "SceneRanking::runOutAnimationMain $_17 __func::~__func", t_701BCC);

// SceneRanking::runOutAnimationMain $_17: __func::~__func (deleting)
static void t_701C08(uint32_t self) {
    st32(self, 0xf6a1a0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701C08, "SceneRanking::runOutAnimationMain $_17 __func::~__func (deleting)", t_701C08);

// SceneRanking::runOutAnimationMain $_17: __func::__clone()
static uint32_t t_701C48(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6a1a0u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x701C48, "SceneRanking::runOutAnimationMain $_17 __func::__clone()", t_701C48);

// SceneRanking::runOutAnimationMain $_17: __func::__clone(__base*)
static void t_701C98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a1a0u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x701C98, "SceneRanking::runOutAnimationMain $_17 __func::__clone(__base*)", t_701C98);

// SceneRanking::runOutAnimationMain $_17: __func::destroy
static void t_701CCC(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x701CCC, "SceneRanking::runOutAnimationMain $_17 __func::destroy", t_701CCC);

// SceneRanking::runOutAnimationMain $_17: __func::destroy_deallocate
static void t_701CEC(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701CEC, "SceneRanking::runOutAnimationMain $_17 __func::destroy_deallocate", t_701CEC);

// SceneRanking::runOutAnimationMain $_17: __func::target
static uint32_t t_701D24(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8aff0u ? self + 8 : 0; }
RW_FUNCTION(0x701D24, "SceneRanking::runOutAnimationMain $_17 __func::target", t_701D24);

// SceneRanking::runOutAnimationMain $_17: __func::target_type
static uint32_t t_701D44() { return 0xfa31acu; }
RW_FUNCTION(0x701D44, "SceneRanking::runOutAnimationMain $_17 __func::target_type", t_701D44);

// SceneRanking::runInAnimation $_16: __func::~__func (deleting)
static void t_701D58(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701D58, "SceneRanking::runInAnimation $_16 __func::~__func (deleting)", t_701D58);

// SceneRanking::runInAnimation $_16: __func::__clone()
static uint32_t t_701D60(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6a1ccu);
    }
    return v1;
}
RW_FUNCTION(0x701D60, "SceneRanking::runInAnimation $_16 __func::__clone()", t_701D60);

// SceneRanking::runInAnimation $_16: __func::__clone(__base*)
static void t_701D84(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a1ccu);
    }
}
RW_FUNCTION(0x701D84, "SceneRanking::runInAnimation $_16 __func::__clone(__base*)", t_701D84);

// SceneRanking::runInAnimation $_16: __func::destroy
static void t_701DA0(uint32_t self) {
}
RW_FUNCTION(0x701DA0, "SceneRanking::runInAnimation $_16 __func::destroy", t_701DA0);

// SceneRanking::runInAnimation $_16: __func::destroy_deallocate
static void t_701DA2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701DA2, "SceneRanking::runInAnimation $_16 __func::destroy_deallocate", t_701DA2);

// SceneRanking::runInAnimation $_16: __func::target
static uint32_t t_701E30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b0a0u ? self + 4 : 0; }
RW_FUNCTION(0x701E30, "SceneRanking::runInAnimation $_16 __func::target", t_701E30);

// SceneRanking::runInAnimation $_16: __func::target_type
static uint32_t t_701E50() { return 0xfa31c0u; }
RW_FUNCTION(0x701E50, "SceneRanking::runInAnimation $_16 __func::target_type", t_701E50);

// SceneRanking::changeScrollView $_15: __func::~__func (deleting)
static void t_701E64(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701E64, "SceneRanking::changeScrollView $_15 __func::~__func (deleting)", t_701E64);

// SceneRanking::changeScrollView $_15: __func::__clone()
static uint32_t t_701E6C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf6a1f8u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x701E6C, "SceneRanking::changeScrollView $_15 __func::__clone()", t_701E6C);

// SceneRanking::changeScrollView $_15: __func::__clone(__base*)
static void t_701E9C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a1f8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x701E9C, "SceneRanking::changeScrollView $_15 __func::__clone(__base*)", t_701E9C);

// SceneRanking::changeScrollView $_15: __func::destroy
static void t_701EC0(uint32_t self) {
}
RW_FUNCTION(0x701EC0, "SceneRanking::changeScrollView $_15 __func::destroy", t_701EC0);

// SceneRanking::changeScrollView $_15: __func::destroy_deallocate
static void t_701EC2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701EC2, "SceneRanking::changeScrollView $_15 __func::destroy_deallocate", t_701EC2);

// SceneRanking::changeScrollView $_15: __func::target
static uint32_t t_701ED4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b130u ? self + 4 : 0; }
RW_FUNCTION(0x701ED4, "SceneRanking::changeScrollView $_15 __func::target", t_701ED4);

// SceneRanking::changeScrollView $_15: __func::target_type
static uint32_t t_701EF4() { return 0xfa31d4u; }
RW_FUNCTION(0x701EF4, "SceneRanking::changeScrollView $_15 __func::target_type", t_701EF4);

// SceneRanking::init $_13: __func::~__func (deleting)
static void t_701F08(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701F08, "SceneRanking::init $_13 __func::~__func (deleting)", t_701F08);

// SceneRanking::init $_13: __func::__clone()
static uint32_t t_701F10(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6a224u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x701F10, "SceneRanking::init $_13 __func::__clone()", t_701F10);

// SceneRanking::init $_13: __func::__clone(__base*)
static void t_701F40(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a224u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x701F40, "SceneRanking::init $_13 __func::__clone(__base*)", t_701F40);

// SceneRanking::init $_13: __func::destroy
static void t_701F68(uint32_t self) {
}
RW_FUNCTION(0x701F68, "SceneRanking::init $_13 __func::destroy", t_701F68);

// SceneRanking::init $_13: __func::destroy_deallocate
static void t_701F6A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701F6A, "SceneRanking::init $_13 __func::destroy_deallocate", t_701F6A);

// SceneRanking::init $_13: __func::target
static uint32_t t_701FB4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b1e0u ? self + 4 : 0; }
RW_FUNCTION(0x701FB4, "SceneRanking::init $_13 __func::target", t_701FB4);

// SceneRanking::init $_13: __func::target_type
static uint32_t t_701FD4() { return 0xfa31e8u; }
RW_FUNCTION(0x701FD4, "SceneRanking::init $_13 __func::target_type", t_701FD4);

// SceneRanking::init $_12: __func::~__func (deleting)
static void t_701FE8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x701FE8, "SceneRanking::init $_12 __func::~__func (deleting)", t_701FE8);

// SceneRanking::init $_12: __func::__clone()
static uint32_t t_701FF0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6a250u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x701FF0, "SceneRanking::init $_12 __func::__clone()", t_701FF0);

// SceneRanking::init $_12: __func::__clone(__base*)
static void t_702020(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a250u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x702020, "SceneRanking::init $_12 __func::__clone(__base*)", t_702020);

// SceneRanking::init $_12: __func::destroy
static void t_702048(uint32_t self) {
}
RW_FUNCTION(0x702048, "SceneRanking::init $_12 __func::destroy", t_702048);

// SceneRanking::init $_12: __func::destroy_deallocate
static void t_70204A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x70204A, "SceneRanking::init $_12 __func::destroy_deallocate", t_70204A);

// SceneRanking::init $_12: __func::target
static uint32_t t_70208C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b280u ? self + 4 : 0; }
RW_FUNCTION(0x70208C, "SceneRanking::init $_12 __func::target", t_70208C);

// SceneRanking::init $_12: __func::target_type
static uint32_t t_7020AC() { return 0xfa31fcu; }
RW_FUNCTION(0x7020AC, "SceneRanking::init $_12 __func::target_type", t_7020AC);

// SceneRanking::init $_10: __func::~__func (deleting)
static void t_7020C0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7020C0, "SceneRanking::init $_10 __func::~__func (deleting)", t_7020C0);

// SceneRanking::init $_10: __func::__clone()
static uint32_t t_7020C8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6a27cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7020C8, "SceneRanking::init $_10 __func::__clone()", t_7020C8);

// SceneRanking::init $_10: __func::__clone(__base*)
static void t_7020F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a27cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7020F4, "SceneRanking::init $_10 __func::__clone(__base*)", t_7020F4);

// SceneRanking::init $_10: __func::destroy
static void t_702114(uint32_t self) {
}
RW_FUNCTION(0x702114, "SceneRanking::init $_10 __func::destroy", t_702114);

// SceneRanking::init $_10: __func::destroy_deallocate
static void t_702116(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702116, "SceneRanking::init $_10 __func::destroy_deallocate", t_702116);

// SceneRanking::init $_10: __func::target
static uint32_t t_7021CC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b320u ? self + 4 : 0; }
RW_FUNCTION(0x7021CC, "SceneRanking::init $_10 __func::target", t_7021CC);

// SceneRanking::init $_10: __func::target_type
static uint32_t t_7021EC() { return 0xfa3210u; }
RW_FUNCTION(0x7021EC, "SceneRanking::init $_10 __func::target_type", t_7021EC);

// SceneRanking::init $_10 UlvE_: __func::~__func (deleting)
static void t_702200(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702200, "SceneRanking::init $_10 UlvE_ __func::~__func (deleting)", t_702200);

// SceneRanking::init $_10 UlvE_: __func::__clone()
static uint32_t t_702208(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6a2a8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x702208, "SceneRanking::init $_10 UlvE_ __func::__clone()", t_702208);

// SceneRanking::init $_10 UlvE_: __func::__clone(__base*)
static void t_702234(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a2a8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x702234, "SceneRanking::init $_10 UlvE_ __func::__clone(__base*)", t_702234);

// SceneRanking::init $_10 UlvE_: __func::destroy
static void t_702254(uint32_t self) {
}
RW_FUNCTION(0x702254, "SceneRanking::init $_10 UlvE_ __func::destroy", t_702254);

// SceneRanking::init $_10 UlvE_: __func::destroy_deallocate
static void t_702256(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702256, "SceneRanking::init $_10 UlvE_ __func::destroy_deallocate", t_702256);

// SceneRanking::init $_10 UlvE_: __func::target
static uint32_t t_702324(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b3d0u ? self + 4 : 0; }
RW_FUNCTION(0x702324, "SceneRanking::init $_10 UlvE_ __func::target", t_702324);

// SceneRanking::init $_10 UlvE_: __func::target_type
static uint32_t t_702344() { return 0xfa3224u; }
RW_FUNCTION(0x702344, "SceneRanking::init $_10 UlvE_ __func::target_type", t_702344);

// SceneRanking::init $_10 UlvE_ UlvE_: __func::~__func (deleting)
static void t_702358(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702358, "SceneRanking::init $_10 UlvE_ UlvE_ __func::~__func (deleting)", t_702358);

// SceneRanking::init $_10 UlvE_ UlvE_: __func::__clone()
static uint32_t t_702360(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6a2d4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x702360, "SceneRanking::init $_10 UlvE_ UlvE_ __func::__clone()", t_702360);

// SceneRanking::init $_10 UlvE_ UlvE_: __func::__clone(__base*)
static void t_70238C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a2d4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x70238C, "SceneRanking::init $_10 UlvE_ UlvE_ __func::__clone(__base*)", t_70238C);

// SceneRanking::init $_10 UlvE_ UlvE_: __func::destroy
static void t_7023AC(uint32_t self) {
}
RW_FUNCTION(0x7023AC, "SceneRanking::init $_10 UlvE_ UlvE_ __func::destroy", t_7023AC);

// SceneRanking::init $_10 UlvE_ UlvE_: __func::destroy_deallocate
static void t_7023AE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7023AE, "SceneRanking::init $_10 UlvE_ UlvE_ __func::destroy_deallocate", t_7023AE);

// SceneRanking::init $_10 UlvE_ UlvE_: __func::target
static uint32_t t_7023D0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b4e0u ? self + 4 : 0; }
RW_FUNCTION(0x7023D0, "SceneRanking::init $_10 UlvE_ UlvE_ __func::target", t_7023D0);

// SceneRanking::init $_10 UlvE_ UlvE_: __func::target_type
static uint32_t t_7023F0() { return 0xfa3238u; }
RW_FUNCTION(0x7023F0, "SceneRanking::init $_10 UlvE_ UlvE_ __func::target_type", t_7023F0);

// SceneRanking::init $_9: __func::~__func (deleting)
static void t_70249C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x70249C, "SceneRanking::init $_9 __func::~__func (deleting)", t_70249C);

// SceneRanking::init $_9: __func::__clone()
static uint32_t t_7024A4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6a300u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7024A4, "SceneRanking::init $_9 __func::__clone()", t_7024A4);

// SceneRanking::init $_9: __func::__clone(__base*)
static void t_7024D0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a300u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7024D0, "SceneRanking::init $_9 __func::__clone(__base*)", t_7024D0);

// SceneRanking::init $_9: __func::destroy
static void t_7024F0(uint32_t self) {
}
RW_FUNCTION(0x7024F0, "SceneRanking::init $_9 __func::destroy", t_7024F0);

// SceneRanking::init $_9: __func::destroy_deallocate
static void t_7024F2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7024F2, "SceneRanking::init $_9 __func::destroy_deallocate", t_7024F2);

// SceneRanking::init $_9: __func::target
static uint32_t t_702514(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b5d0u ? self + 4 : 0; }
RW_FUNCTION(0x702514, "SceneRanking::init $_9 __func::target", t_702514);

// SceneRanking::init $_9: __func::target_type
static uint32_t t_702534() { return 0xfa324cu; }
RW_FUNCTION(0x702534, "SceneRanking::init $_9 __func::target_type", t_702534);

// SceneRanking::init $_8: __func::~__func (deleting)
static void t_702548(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702548, "SceneRanking::init $_8 __func::~__func (deleting)", t_702548);

// SceneRanking::init $_8: __func::__clone()
static uint32_t t_702550(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6a32cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x702550, "SceneRanking::init $_8 __func::__clone()", t_702550);

// SceneRanking::init $_8: __func::__clone(__base*)
static void t_70257C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a32cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x70257C, "SceneRanking::init $_8 __func::__clone(__base*)", t_70257C);

// SceneRanking::init $_8: __func::destroy
static void t_70259C(uint32_t self) {
}
RW_FUNCTION(0x70259C, "SceneRanking::init $_8 __func::destroy", t_70259C);

// SceneRanking::init $_8: __func::destroy_deallocate
static void t_70259E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x70259E, "SceneRanking::init $_8 __func::destroy_deallocate", t_70259E);

// SceneRanking::init $_8: __func::target
static uint32_t t_7025B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b650u ? self + 4 : 0; }
RW_FUNCTION(0x7025B0, "SceneRanking::init $_8 __func::target", t_7025B0);

// SceneRanking::init $_8: __func::target_type
static uint32_t t_7025D0() { return 0xfa3260u; }
RW_FUNCTION(0x7025D0, "SceneRanking::init $_8 __func::target_type", t_7025D0);

// SceneRanking::init $_7: __func::~__func (deleting)
static void t_7025E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7025E4, "SceneRanking::init $_7 __func::~__func (deleting)", t_7025E4);

// SceneRanking::init $_7: __func::__clone()
static uint32_t t_7025EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6a358u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7025EC, "SceneRanking::init $_7 __func::__clone()", t_7025EC);

// SceneRanking::init $_7: __func::__clone(__base*)
static void t_702618(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a358u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x702618, "SceneRanking::init $_7 __func::__clone(__base*)", t_702618);

// SceneRanking::init $_7: __func::destroy
static void t_702638(uint32_t self) {
}
RW_FUNCTION(0x702638, "SceneRanking::init $_7 __func::destroy", t_702638);

// SceneRanking::init $_7: __func::destroy_deallocate
static void t_70263A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x70263A, "SceneRanking::init $_7 __func::destroy_deallocate", t_70263A);

// SceneRanking::init $_7: __func::target
static uint32_t t_70264C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b6d0u ? self + 4 : 0; }
RW_FUNCTION(0x70264C, "SceneRanking::init $_7 __func::target", t_70264C);

// SceneRanking::init $_7: __func::target_type
static uint32_t t_70266C() { return 0xfa3274u; }
RW_FUNCTION(0x70266C, "SceneRanking::init $_7 __func::target_type", t_70266C);

// SceneRanking::init $_6: __func::~__func (deleting)
static void t_702680(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702680, "SceneRanking::init $_6 __func::~__func (deleting)", t_702680);

// SceneRanking::init $_6: __func::__clone()
static uint32_t t_702688(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6a384u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x702688, "SceneRanking::init $_6 __func::__clone()", t_702688);

// SceneRanking::init $_6: __func::__clone(__base*)
static void t_7026B4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a384u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7026B4, "SceneRanking::init $_6 __func::__clone(__base*)", t_7026B4);

// SceneRanking::init $_6: __func::destroy
static void t_7026D4(uint32_t self) {
}
RW_FUNCTION(0x7026D4, "SceneRanking::init $_6 __func::destroy", t_7026D4);

// SceneRanking::init $_6: __func::destroy_deallocate
static void t_7026D6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7026D6, "SceneRanking::init $_6 __func::destroy_deallocate", t_7026D6);

// SceneRanking::init $_6: __func::target
static uint32_t t_7026F8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b750u ? self + 4 : 0; }
RW_FUNCTION(0x7026F8, "SceneRanking::init $_6 __func::target", t_7026F8);

// SceneRanking::init $_6: __func::target_type
static uint32_t t_702718() { return 0xfa3288u; }
RW_FUNCTION(0x702718, "SceneRanking::init $_6 __func::target_type", t_702718);

// SceneRanking::callColosseumRankingAPI $_5: __func::~__func
static uint32_t t_70272C(uint32_t self) {
    st32(self, 0xf6a3b0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x70272C, "SceneRanking::callColosseumRankingAPI $_5 __func::~__func", t_70272C);

// SceneRanking::callColosseumRankingAPI $_5: __func::~__func (deleting)
static void t_702768(uint32_t self) {
    st32(self, 0xf6a3b0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702768, "SceneRanking::callColosseumRankingAPI $_5 __func::~__func (deleting)", t_702768);

// SceneRanking::callColosseumRankingAPI $_5: __func::__clone()
static uint32_t t_7027A8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6a3b0u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x7027A8, "SceneRanking::callColosseumRankingAPI $_5 __func::__clone()", t_7027A8);

// SceneRanking::callColosseumRankingAPI $_5: __func::__clone(__base*)
static void t_7027F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a3b0u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x7027F8, "SceneRanking::callColosseumRankingAPI $_5 __func::__clone(__base*)", t_7027F8);

// SceneRanking::callColosseumRankingAPI $_5: __func::destroy
static void t_70282C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x70282C, "SceneRanking::callColosseumRankingAPI $_5 __func::destroy", t_70282C);

// SceneRanking::callColosseumRankingAPI $_5: __func::destroy_deallocate
static void t_70284C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x70284C, "SceneRanking::callColosseumRankingAPI $_5 __func::destroy_deallocate", t_70284C);

// SceneRanking::callColosseumRankingAPI $_5: __func::target
static uint32_t t_702998(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b7f0u ? self + 8 : 0; }
RW_FUNCTION(0x702998, "SceneRanking::callColosseumRankingAPI $_5 __func::target", t_702998);

// SceneRanking::callColosseumRankingAPI $_5: __func::target_type
static uint32_t t_7029B8() { return 0xfa329cu; }
RW_FUNCTION(0x7029B8, "SceneRanking::callColosseumRankingAPI $_5 __func::target_type", t_7029B8);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_: __func::~__func
static uint32_t t_7029CC(uint32_t self) {
    st32(self, 0xf6a3dcu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x7029CC, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ __func::~__func", t_7029CC);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_: __func::~__func (deleting)
static void t_702A08(uint32_t self) {
    st32(self, 0xf6a3dcu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702A08, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ __func::~__func (deleting)", t_702A08);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_: __func::__clone()
static uint32_t t_702A48(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6a3dcu);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x702A48, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ __func::__clone()", t_702A48);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_: __func::__clone(__base*)
static void t_702A98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a3dcu);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x702A98, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ __func::__clone(__base*)", t_702A98);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_: __func::destroy
static void t_702ACC(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x702ACC, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ __func::destroy", t_702ACC);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_: __func::destroy_deallocate
static void t_702AEC(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702AEC, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ __func::destroy_deallocate", t_702AEC);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_: __func::target
static uint32_t t_702C38(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b8d0u ? self + 8 : 0; }
RW_FUNCTION(0x702C38, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ __func::target", t_702C38);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_: __func::target_type
static uint32_t t_702C58() { return 0xfa32b0u; }
RW_FUNCTION(0x702C58, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ __func::target_type", t_702C58);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_: __func::~__func
static uint32_t t_702C6C(uint32_t self) {
    st32(self, 0xf6a408u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x702C6C, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_ __func::~__func", t_702C6C);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_: __func::~__func (deleting)
static void t_702CA8(uint32_t self) {
    st32(self, 0xf6a408u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702CA8, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_ __func::~__func (deleting)", t_702CA8);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_: __func::__clone()
static uint32_t t_702CE8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6a408u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x702CE8, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_ __func::__clone()", t_702CE8);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_: __func::__clone(__base*)
static void t_702D38(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a408u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x702D38, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_ __func::__clone(__base*)", t_702D38);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_: __func::destroy
static void t_702D6C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x702D6C, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_ __func::destroy", t_702D6C);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_: __func::destroy_deallocate
static void t_702D8C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702D8C, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_ __func::destroy_deallocate", t_702D8C);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_: __func::target
static uint32_t t_702DCC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8b9c0u ? self + 8 : 0; }
RW_FUNCTION(0x702DCC, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_ __func::target", t_702DCC);

// SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_: __func::target_type
static uint32_t t_702DEC() { return 0xfa32c4u; }
RW_FUNCTION(0x702DEC, "SceneRanking::callColosseumRankingAPI $_5 UlbE_ UlbE_ __func::target_type", t_702DEC);

// SceneRanking::callPartyRankingAPI $_3: __func::~__func
static uint32_t t_702E00(uint32_t self) {
    st32(self, 0xf6a434u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x702E00, "SceneRanking::callPartyRankingAPI $_3 __func::~__func", t_702E00);

// SceneRanking::callPartyRankingAPI $_3: __func::~__func (deleting)
static void t_702E3C(uint32_t self) {
    st32(self, 0xf6a434u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702E3C, "SceneRanking::callPartyRankingAPI $_3 __func::~__func (deleting)", t_702E3C);

// SceneRanking::callPartyRankingAPI $_3: __func::__clone()
static uint32_t t_702E7C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6a434u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x702E7C, "SceneRanking::callPartyRankingAPI $_3 __func::__clone()", t_702E7C);

// SceneRanking::callPartyRankingAPI $_3: __func::__clone(__base*)
static void t_702ECC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6a434u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x702ECC, "SceneRanking::callPartyRankingAPI $_3 __func::__clone(__base*)", t_702ECC);

// SceneRanking::callPartyRankingAPI $_3: __func::destroy
static void t_702F00(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x702F00, "SceneRanking::callPartyRankingAPI $_3 __func::destroy", t_702F00);

// SceneRanking::callPartyRankingAPI $_3: __func::destroy_deallocate
static void t_702F20(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x702F20, "SceneRanking::callPartyRankingAPI $_3 __func::destroy_deallocate", t_702F20);

// SceneRanking::callPartyRankingAPI $_3: __func::target
static uint32_t t_70306C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8bab0u ? self + 8 : 0; }
RW_FUNCTION(0x70306C, "SceneRanking::callPartyRankingAPI $_3 __func::target", t_70306C);

// SceneRanking::callPartyRankingAPI $_3: __func::target_type
static uint32_t t_70308C() { return 0xfa32d8u; }
RW_FUNCTION(0x70308C, "SceneRanking::callPartyRankingAPI $_3 __func::target_type", t_70308C);

// SceneRanking::callPartyRankingAPI $_3 UlbE_: __func::~__func
static uint32_t t_7030A0(uint32_t self) {
    st32(self, 0xf6a460u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x7030A0, "SceneRanking::callPartyRankingAPI $_3 UlbE_ __func::~__func", t_7030A0);

// SceneRanking::callPartyRankingAPI $_3 UlbE_: __func::~__func (deleting)
static void t_7030DC(uint32_t self) {
    st32(self, 0xf6a460u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7030DC, "SceneRanking::callPartyRankingAPI $_3 UlbE_ __func::~__func (deleting)", t_7030DC);

// SceneRanking::callPartyRankingAPI $_3 UlbE_: __func::__clone()
static uint32_t t_70311C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6a460u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x70311C, "SceneRanking::callPartyRankingAPI $_3 UlbE_ __func::__clone()", t_70311C);
