// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneUnionRanking lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneUnionRanking::init $_7: __func::~__func (deleting)
static void t_7389FC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7389FC, "SceneUnionRanking::init $_7 __func::~__func (deleting)", t_7389FC);

// SceneUnionRanking::init $_7: __func::__clone()
static uint32_t t_738A04(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6d588u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x738A04, "SceneUnionRanking::init $_7 __func::__clone()", t_738A04);

// SceneUnionRanking::init $_7: __func::__clone(__base*)
static void t_738A34(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d588u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x738A34, "SceneUnionRanking::init $_7 __func::__clone(__base*)", t_738A34);

// SceneUnionRanking::init $_7: __func::destroy
static void t_738A5C(uint32_t self) {
}
RW_FUNCTION(0x738A5C, "SceneUnionRanking::init $_7 __func::destroy", t_738A5C);

// SceneUnionRanking::init $_7: __func::destroy_deallocate
static void t_738A5E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x738A5E, "SceneUnionRanking::init $_7 __func::destroy_deallocate", t_738A5E);

// SceneUnionRanking::init $_7: __func::target
static uint32_t t_738B58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b2d0u ? self + 4 : 0; }
RW_FUNCTION(0x738B58, "SceneUnionRanking::init $_7 __func::target", t_738B58);

// SceneUnionRanking::init $_7: __func::target_type
static uint32_t t_738B78() { return 0xfa6fb8u; }
RW_FUNCTION(0x738B78, "SceneUnionRanking::init $_7 __func::target_type", t_738B78);

// SceneUnionRanking::init $_7 UlvE_: __func::~__func (deleting)
static void t_738B8C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x738B8C, "SceneUnionRanking::init $_7 UlvE_ __func::~__func (deleting)", t_738B8C);

// SceneUnionRanking::init $_7 UlvE_: __func::__clone()
static uint32_t t_738B94(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6d5b4u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x738B94, "SceneUnionRanking::init $_7 UlvE_ __func::__clone()", t_738B94);

// SceneUnionRanking::init $_7 UlvE_: __func::__clone(__base*)
static void t_738BC4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d5b4u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x738BC4, "SceneUnionRanking::init $_7 UlvE_ __func::__clone(__base*)", t_738BC4);

// SceneUnionRanking::init $_7 UlvE_: __func::destroy
static void t_738BEC(uint32_t self) {
}
RW_FUNCTION(0x738BEC, "SceneUnionRanking::init $_7 UlvE_ __func::destroy", t_738BEC);

// SceneUnionRanking::init $_7 UlvE_: __func::destroy_deallocate
static void t_738BEE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x738BEE, "SceneUnionRanking::init $_7 UlvE_ __func::destroy_deallocate", t_738BEE);

// SceneUnionRanking::init $_7 UlvE_: __func::target
static uint32_t t_738EB4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b370u ? self + 4 : 0; }
RW_FUNCTION(0x738EB4, "SceneUnionRanking::init $_7 UlvE_ __func::target", t_738EB4);

// SceneUnionRanking::init $_7 UlvE_: __func::target_type
static uint32_t t_738ED4() { return 0xfa6fccu; }
RW_FUNCTION(0x738ED4, "SceneUnionRanking::init $_7 UlvE_ __func::target_type", t_738ED4);

// SceneUnionRanking::init $_7 UlvE_: __func::~__func (deleting)
static void t_738EE8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x738EE8, "SceneUnionRanking::init $_7 UlvE_ __func::~__func (deleting)", t_738EE8);

// SceneUnionRanking::init $_7 UlvE_: __func::__clone()
static uint32_t t_738EF0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf6d5e0u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x738EF0, "SceneUnionRanking::init $_7 UlvE_ __func::__clone()", t_738EF0);

// SceneUnionRanking::init $_7 UlvE_: __func::__clone(__base*)
static void t_738F20(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d5e0u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x738F20, "SceneUnionRanking::init $_7 UlvE_ __func::__clone(__base*)", t_738F20);

// SceneUnionRanking::init $_7 UlvE_: __func::destroy
static void t_738F44(uint32_t self) {
}
RW_FUNCTION(0x738F44, "SceneUnionRanking::init $_7 UlvE_ __func::destroy", t_738F44);

// SceneUnionRanking::init $_7 UlvE_: __func::destroy_deallocate
static void t_738F46(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x738F46, "SceneUnionRanking::init $_7 UlvE_ __func::destroy_deallocate", t_738F46);

// SceneUnionRanking::init $_7 UlvE_: __func::target
static uint32_t t_739024(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b450u ? self + 4 : 0; }
RW_FUNCTION(0x739024, "SceneUnionRanking::init $_7 UlvE_ __func::target", t_739024);

// SceneUnionRanking::init $_7 UlvE_: __func::target_type
static uint32_t t_739044() { return 0xfa6fe0u; }
RW_FUNCTION(0x739044, "SceneUnionRanking::init $_7 UlvE_ __func::target_type", t_739044);

// SceneUnionRanking::init $_6: __func::~__func (deleting)
static void t_7391B0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7391B0, "SceneUnionRanking::init $_6 __func::~__func (deleting)", t_7391B0);

// SceneUnionRanking::init $_6: __func::__clone()
static uint32_t t_7391B8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d60cu);
    }
    return v1;
}
RW_FUNCTION(0x7391B8, "SceneUnionRanking::init $_6 __func::__clone()", t_7391B8);

// SceneUnionRanking::init $_6: __func::__clone(__base*)
static void t_7391DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d60cu);
    }
}
RW_FUNCTION(0x7391DC, "SceneUnionRanking::init $_6 __func::__clone(__base*)", t_7391DC);

// SceneUnionRanking::init $_6: __func::destroy
static void t_7391F8(uint32_t self) {
}
RW_FUNCTION(0x7391F8, "SceneUnionRanking::init $_6 __func::destroy", t_7391F8);

// SceneUnionRanking::init $_6: __func::destroy_deallocate
static void t_7391FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7391FA, "SceneUnionRanking::init $_6 __func::destroy_deallocate", t_7391FA);

// SceneUnionRanking::init $_6: __func::target
static uint32_t t_7392E4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b520u ? self + 4 : 0; }
RW_FUNCTION(0x7392E4, "SceneUnionRanking::init $_6 __func::target", t_7392E4);

// SceneUnionRanking::init $_6: __func::target_type
static uint32_t t_739304() { return 0xfa6ff4u; }
RW_FUNCTION(0x739304, "SceneUnionRanking::init $_6 __func::target_type", t_739304);

// SceneUnionRanking::init $_5: __func::~__func (deleting)
static void t_739318(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x739318, "SceneUnionRanking::init $_5 __func::~__func (deleting)", t_739318);

// SceneUnionRanking::init $_5: __func::__clone()
static uint32_t t_739320(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d638u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x739320, "SceneUnionRanking::init $_5 __func::__clone()", t_739320);

// SceneUnionRanking::init $_5: __func::__clone(__base*)
static void t_73934C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d638u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x73934C, "SceneUnionRanking::init $_5 __func::__clone(__base*)", t_73934C);

// SceneUnionRanking::init $_5: __func::destroy
static void t_73936C(uint32_t self) {
}
RW_FUNCTION(0x73936C, "SceneUnionRanking::init $_5 __func::destroy", t_73936C);

// SceneUnionRanking::init $_5: __func::destroy_deallocate
static void t_73936E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73936E, "SceneUnionRanking::init $_5 __func::destroy_deallocate", t_73936E);

// SceneUnionRanking::init $_5: __func::target
static uint32_t t_739380(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b5b0u ? self + 4 : 0; }
RW_FUNCTION(0x739380, "SceneUnionRanking::init $_5 __func::target", t_739380);

// SceneUnionRanking::init $_5: __func::target_type
static uint32_t t_7393A0() { return 0xfa7008u; }
RW_FUNCTION(0x7393A0, "SceneUnionRanking::init $_5 __func::target_type", t_7393A0);

// SceneUnionRanking::init $_4: __func::~__func (deleting)
static void t_7393B4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7393B4, "SceneUnionRanking::init $_4 __func::~__func (deleting)", t_7393B4);

// SceneUnionRanking::init $_4: __func::__clone()
static uint32_t t_7393BC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d664u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7393BC, "SceneUnionRanking::init $_4 __func::__clone()", t_7393BC);

// SceneUnionRanking::init $_4: __func::__clone(__base*)
static void t_7393E8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d664u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7393E8, "SceneUnionRanking::init $_4 __func::__clone(__base*)", t_7393E8);

// SceneUnionRanking::init $_4: __func::destroy
static void t_739408(uint32_t self) {
}
RW_FUNCTION(0x739408, "SceneUnionRanking::init $_4 __func::destroy", t_739408);

// SceneUnionRanking::init $_4: __func::destroy_deallocate
static void t_73940A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73940A, "SceneUnionRanking::init $_4 __func::destroy_deallocate", t_73940A);

// SceneUnionRanking::init $_4: __func::target
static uint32_t t_73941C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b640u ? self + 4 : 0; }
RW_FUNCTION(0x73941C, "SceneUnionRanking::init $_4 __func::target", t_73941C);

// SceneUnionRanking::init $_4: __func::target_type
static uint32_t t_73943C() { return 0xfa701cu; }
RW_FUNCTION(0x73943C, "SceneUnionRanking::init $_4 __func::target_type", t_73943C);

// SceneUnionRanking::settingUnionRankingLwf : __func::~__func (deleting)
static void t_739450(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x739450, "SceneUnionRanking::settingUnionRankingLwf  __func::~__func (deleting)", t_739450);

// SceneUnionRanking::settingUnionRankingLwf : __func::__clone()
static uint32_t t_739458(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf6d690u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x739458, "SceneUnionRanking::settingUnionRankingLwf  __func::__clone()", t_739458);

// SceneUnionRanking::settingUnionRankingLwf : __func::__clone(__base*)
static void t_739488(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d690u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x739488, "SceneUnionRanking::settingUnionRankingLwf  __func::__clone(__base*)", t_739488);

// SceneUnionRanking::settingUnionRankingLwf : __func::destroy
static void t_7394AC(uint32_t self) {
}
RW_FUNCTION(0x7394AC, "SceneUnionRanking::settingUnionRankingLwf  __func::destroy", t_7394AC);

// SceneUnionRanking::settingUnionRankingLwf : __func::destroy_deallocate
static void t_7394AE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7394AE, "SceneUnionRanking::settingUnionRankingLwf  __func::destroy_deallocate", t_7394AE);

// SceneUnionRanking::settingUnionRankingLwf : __func::target
static uint32_t t_7394EC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b720u ? self + 4 : 0; }
RW_FUNCTION(0x7394EC, "SceneUnionRanking::settingUnionRankingLwf  __func::target", t_7394EC);

// SceneUnionRanking::settingUnionRankingLwf : __func::target_type
static uint32_t t_73950C() { return 0xfa7030u; }
RW_FUNCTION(0x73950C, "SceneUnionRanking::settingUnionRankingLwf  __func::target_type", t_73950C);

// SceneUnionRanking::settingUnionRankingLwf : __func::~__func (deleting)
static void t_739520(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x739520, "SceneUnionRanking::settingUnionRankingLwf  __func::~__func (deleting)", t_739520);

// SceneUnionRanking::settingUnionRankingLwf : __func::__clone()
static uint32_t t_739528(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf6d6bcu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        st32(v1 + 8 + 12, v5);
        uint32_t v6 = ld32(self + 24 + 0);
        uint32_t v7 = ld32(self + 24 + 4);
        uint32_t v8 = ld32(self + 24 + 8);
        uint32_t v9 = ld32(self + 24 + 12);
        st32(v1 + 24 + 0, v6);
        st32(v1 + 24 + 4, v7);
        st32(v1 + 24 + 8, v8);
        st32(v1 + 24 + 12, v9);
    }
    return v1;
}
RW_FUNCTION(0x739528, "SceneUnionRanking::settingUnionRankingLwf  __func::__clone()", t_739528);

// SceneUnionRanking::settingUnionRankingLwf : __func::__clone(__base*)
static void t_739564(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d6bcu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        uint32_t v5 = ld32(self + 24 + 0);
        uint32_t v6 = ld32(self + 24 + 4);
        uint32_t v7 = ld32(self + 24 + 8);
        uint32_t v8 = ld32(self + 36);
        st32(p + 36, v8);
        st32(p + 24 + 0, v5);
        st32(p + 24 + 4, v6);
        st32(p + 24 + 8, v7);
    }
}
RW_FUNCTION(0x739564, "SceneUnionRanking::settingUnionRankingLwf  __func::__clone(__base*)", t_739564);

// SceneUnionRanking::settingUnionRankingLwf : __func::destroy
static void t_73959C(uint32_t self) {
}
RW_FUNCTION(0x73959C, "SceneUnionRanking::settingUnionRankingLwf  __func::destroy", t_73959C);

// SceneUnionRanking::settingUnionRankingLwf : __func::destroy_deallocate
static void t_73959E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73959E, "SceneUnionRanking::settingUnionRankingLwf  __func::destroy_deallocate", t_73959E);

// SceneUnionRanking::settingUnionRankingLwf : __func::target
static uint32_t t_739738(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b850u ? self + 8 : 0; }
RW_FUNCTION(0x739738, "SceneUnionRanking::settingUnionRankingLwf  __func::target", t_739738);

// SceneUnionRanking::settingUnionRankingLwf : __func::target_type
static uint32_t t_73975C() { return 0xfa7044u; }
RW_FUNCTION(0x73975C, "SceneUnionRanking::settingUnionRankingLwf  __func::target_type", t_73975C);

// SceneUnionRanking::settingUnionRankingLwf : __func::~__func (deleting)
static void t_739770(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x739770, "SceneUnionRanking::settingUnionRankingLwf  __func::~__func (deleting)", t_739770);

// SceneUnionRanking::settingUnionRankingLwf : __func::__clone()
static uint32_t t_739778(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf6d6e8u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x739778, "SceneUnionRanking::settingUnionRankingLwf  __func::__clone()", t_739778);

// SceneUnionRanking::settingUnionRankingLwf : __func::__clone(__base*)
static void t_7397A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d6e8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7397A8, "SceneUnionRanking::settingUnionRankingLwf  __func::__clone(__base*)", t_7397A8);

// SceneUnionRanking::settingUnionRankingLwf : __func::destroy
static void t_7397CC(uint32_t self) {
}
RW_FUNCTION(0x7397CC, "SceneUnionRanking::settingUnionRankingLwf  __func::destroy", t_7397CC);

// SceneUnionRanking::settingUnionRankingLwf : __func::destroy_deallocate
static void t_7397CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7397CE, "SceneUnionRanking::settingUnionRankingLwf  __func::destroy_deallocate", t_7397CE);

// SceneUnionRanking::settingUnionRankingLwf : __func::target
static uint32_t t_7398B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9b980u ? self + 4 : 0; }
RW_FUNCTION(0x7398B8, "SceneUnionRanking::settingUnionRankingLwf  __func::target", t_7398B8);

// SceneUnionRanking::settingUnionRankingLwf : __func::target_type
static uint32_t t_7398D8() { return 0xfa7058u; }
RW_FUNCTION(0x7398D8, "SceneUnionRanking::settingUnionRankingLwf  __func::target_type", t_7398D8);
