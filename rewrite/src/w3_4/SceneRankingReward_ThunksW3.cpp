// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the SceneRankingReward lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneRankingReward::init $_9: __func::__clone(__base*)
static void t_711230(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6aedcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x711230, "SceneRankingReward::init $_9 __func::__clone(__base*)", t_711230);

// SceneRankingReward::init $_9: __func::destroy
static void t_711250(uint32_t self) {
}
RW_FUNCTION(0x711250, "SceneRankingReward::init $_9 __func::destroy", t_711250);

// SceneRankingReward::init $_9: __func::destroy_deallocate
static void t_711252(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711252, "SceneRankingReward::init $_9 __func::destroy_deallocate", t_711252);

// SceneRankingReward::init $_9: __func::target
static uint32_t t_711274(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8ebc0u ? self + 4 : 0; }
RW_FUNCTION(0x711274, "SceneRankingReward::init $_9 __func::target", t_711274);

// SceneRankingReward::init $_9: __func::target_type
static uint32_t t_711294() { return 0xfa3e50u; }
RW_FUNCTION(0x711294, "SceneRankingReward::init $_9 __func::target_type", t_711294);

// SceneRankingReward::init $_8: __func::~__func (deleting)
static void t_7112A8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7112A8, "SceneRankingReward::init $_8 __func::~__func (deleting)", t_7112A8);

// SceneRankingReward::init $_8: __func::__clone()
static uint32_t t_7112B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6af08u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7112B0, "SceneRankingReward::init $_8 __func::__clone()", t_7112B0);

// SceneRankingReward::init $_8: __func::__clone(__base*)
static void t_7112DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6af08u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7112DC, "SceneRankingReward::init $_8 __func::__clone(__base*)", t_7112DC);

// SceneRankingReward::init $_8: __func::destroy
static void t_7112FC(uint32_t self) {
}
RW_FUNCTION(0x7112FC, "SceneRankingReward::init $_8 __func::destroy", t_7112FC);

// SceneRankingReward::init $_8: __func::destroy_deallocate
static void t_7112FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7112FE, "SceneRankingReward::init $_8 __func::destroy_deallocate", t_7112FE);

// SceneRankingReward::init $_8: __func::target
static uint32_t t_711320(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8ec80u ? self + 4 : 0; }
RW_FUNCTION(0x711320, "SceneRankingReward::init $_8 __func::target", t_711320);

// SceneRankingReward::init $_8: __func::target_type
static uint32_t t_711340() { return 0xfa3e64u; }
RW_FUNCTION(0x711340, "SceneRankingReward::init $_8 __func::target_type", t_711340);

// SceneRankingReward::init $_7: __func::~__func (deleting)
static void t_711354(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711354, "SceneRankingReward::init $_7 __func::~__func (deleting)", t_711354);

// SceneRankingReward::init $_7: __func::__clone()
static uint32_t t_71135C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6af34u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71135C, "SceneRankingReward::init $_7 __func::__clone()", t_71135C);

// SceneRankingReward::init $_7: __func::__clone(__base*)
static void t_711388(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6af34u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x711388, "SceneRankingReward::init $_7 __func::__clone(__base*)", t_711388);

// SceneRankingReward::init $_7: __func::destroy
static void t_7113A8(uint32_t self) {
}
RW_FUNCTION(0x7113A8, "SceneRankingReward::init $_7 __func::destroy", t_7113A8);

// SceneRankingReward::init $_7: __func::destroy_deallocate
static void t_7113AA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7113AA, "SceneRankingReward::init $_7 __func::destroy_deallocate", t_7113AA);

// SceneRankingReward::init $_7: __func::target
static uint32_t t_71148C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8ed10u ? self + 4 : 0; }
RW_FUNCTION(0x71148C, "SceneRankingReward::init $_7 __func::target", t_71148C);

// SceneRankingReward::init $_7: __func::target_type
static uint32_t t_7114AC() { return 0xfa3e78u; }
RW_FUNCTION(0x7114AC, "SceneRankingReward::init $_7 __func::target_type", t_7114AC);

// SceneRankingReward::init $_6: __func::~__func (deleting)
static void t_7114C0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7114C0, "SceneRankingReward::init $_6 __func::~__func (deleting)", t_7114C0);

// SceneRankingReward::init $_6: __func::__clone()
static uint32_t t_7114C8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6af60u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x7114C8, "SceneRankingReward::init $_6 __func::__clone()", t_7114C8);

// SceneRankingReward::init $_6: __func::__clone(__base*)
static void t_7114F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6af60u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7114F4, "SceneRankingReward::init $_6 __func::__clone(__base*)", t_7114F4);

// SceneRankingReward::init $_6: __func::destroy
static void t_711514(uint32_t self) {
}
RW_FUNCTION(0x711514, "SceneRankingReward::init $_6 __func::destroy", t_711514);

// SceneRankingReward::init $_6: __func::destroy_deallocate
static void t_711516(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711516, "SceneRankingReward::init $_6 __func::destroy_deallocate", t_711516);

// SceneRankingReward::init $_6: __func::target
static uint32_t t_71163C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8eda0u ? self + 4 : 0; }
RW_FUNCTION(0x71163C, "SceneRankingReward::init $_6 __func::target", t_71163C);

// SceneRankingReward::init $_6: __func::target_type
static uint32_t t_71165C() { return 0xfa3e8cu; }
RW_FUNCTION(0x71165C, "SceneRankingReward::init $_6 __func::target_type", t_71165C);

// SceneRankingReward::init $_5: __func::~__func (deleting)
static void t_711670(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711670, "SceneRankingReward::init $_5 __func::~__func (deleting)", t_711670);

// SceneRankingReward::init $_5: __func::__clone()
static uint32_t t_711678(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6af8cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x711678, "SceneRankingReward::init $_5 __func::__clone()", t_711678);

// SceneRankingReward::init $_5: __func::__clone(__base*)
static void t_7116A4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6af8cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x7116A4, "SceneRankingReward::init $_5 __func::__clone(__base*)", t_7116A4);

// SceneRankingReward::init $_5: __func::destroy
static void t_7116C4(uint32_t self) {
}
RW_FUNCTION(0x7116C4, "SceneRankingReward::init $_5 __func::destroy", t_7116C4);

// SceneRankingReward::init $_5: __func::destroy_deallocate
static void t_7116C6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7116C6, "SceneRankingReward::init $_5 __func::destroy_deallocate", t_7116C6);

// SceneRankingReward::init $_5: __func::target
static uint32_t t_711990(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8ee40u ? self + 4 : 0; }
RW_FUNCTION(0x711990, "SceneRankingReward::init $_5 __func::target", t_711990);

// SceneRankingReward::init $_5: __func::target_type
static uint32_t t_7119B0() { return 0xfa3ea0u; }
RW_FUNCTION(0x7119B0, "SceneRankingReward::init $_5 __func::target_type", t_7119B0);

// SceneRankingReward::init $_4: __func::~__func (deleting)
static void t_7119C4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7119C4, "SceneRankingReward::init $_4 __func::~__func (deleting)", t_7119C4);

// SceneRankingReward::init $_4: __func::__clone()
static uint32_t t_7119CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6afb8u);
    }
    return v1;
}
RW_FUNCTION(0x7119CC, "SceneRankingReward::init $_4 __func::__clone()", t_7119CC);

// SceneRankingReward::init $_4: __func::__clone(__base*)
static void t_7119F0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6afb8u);
    }
}
RW_FUNCTION(0x7119F0, "SceneRankingReward::init $_4 __func::__clone(__base*)", t_7119F0);

// SceneRankingReward::init $_4: __func::destroy
static void t_711A0C(uint32_t self) {
}
RW_FUNCTION(0x711A0C, "SceneRankingReward::init $_4 __func::destroy", t_711A0C);

// SceneRankingReward::init $_4: __func::destroy_deallocate
static void t_711A0E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711A0E, "SceneRankingReward::init $_4 __func::destroy_deallocate", t_711A0E);

// SceneRankingReward::init $_4: __func::target
static uint32_t t_711A20(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8eee0u ? self + 4 : 0; }
RW_FUNCTION(0x711A20, "SceneRankingReward::init $_4 __func::target", t_711A20);

// SceneRankingReward::init $_4: __func::target_type
static uint32_t t_711A40() { return 0xfa3eb4u; }
RW_FUNCTION(0x711A40, "SceneRankingReward::init $_4 __func::target_type", t_711A40);

// SceneRankingReward::init $_3: __func::~__func (deleting)
static void t_711A54(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711A54, "SceneRankingReward::init $_3 __func::~__func (deleting)", t_711A54);

// SceneRankingReward::init $_3: __func::__clone()
static uint32_t t_711A5C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6afe4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x711A5C, "SceneRankingReward::init $_3 __func::__clone()", t_711A5C);

// SceneRankingReward::init $_3: __func::__clone(__base*)
static void t_711A88(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6afe4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x711A88, "SceneRankingReward::init $_3 __func::__clone(__base*)", t_711A88);

// SceneRankingReward::init $_3: __func::destroy
static void t_711AA8(uint32_t self) {
}
RW_FUNCTION(0x711AA8, "SceneRankingReward::init $_3 __func::destroy", t_711AA8);

// SceneRankingReward::init $_3: __func::destroy_deallocate
static void t_711AAA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711AAA, "SceneRankingReward::init $_3 __func::destroy_deallocate", t_711AAA);

// SceneRankingReward::init $_3: __func::target
static uint32_t t_711ACC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8efa0u ? self + 4 : 0; }
RW_FUNCTION(0x711ACC, "SceneRankingReward::init $_3 __func::target", t_711ACC);

// SceneRankingReward::init $_3: __func::target_type
static uint32_t t_711AEC() { return 0xfa3ec8u; }
RW_FUNCTION(0x711AEC, "SceneRankingReward::init $_3 __func::target_type", t_711AEC);

// SceneRankingReward::init $_2: __func::~__func (deleting)
static void t_711B00(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711B00, "SceneRankingReward::init $_2 __func::~__func (deleting)", t_711B00);

// SceneRankingReward::init $_2: __func::__clone()
static uint32_t t_711B08(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6b010u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x711B08, "SceneRankingReward::init $_2 __func::__clone()", t_711B08);

// SceneRankingReward::init $_2: __func::__clone(__base*)
static void t_711B34(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b010u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x711B34, "SceneRankingReward::init $_2 __func::__clone(__base*)", t_711B34);

// SceneRankingReward::init $_2: __func::destroy
static void t_711B54(uint32_t self) {
}
RW_FUNCTION(0x711B54, "SceneRankingReward::init $_2 __func::destroy", t_711B54);

// SceneRankingReward::init $_2: __func::destroy_deallocate
static void t_711B56(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711B56, "SceneRankingReward::init $_2 __func::destroy_deallocate", t_711B56);

// SceneRankingReward::init $_2: __func::target
static uint32_t t_711B68(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8f030u ? self + 4 : 0; }
RW_FUNCTION(0x711B68, "SceneRankingReward::init $_2 __func::target", t_711B68);

// SceneRankingReward::init $_2: __func::target_type
static uint32_t t_711B88() { return 0xfa3edcu; }
RW_FUNCTION(0x711B88, "SceneRankingReward::init $_2 __func::target_type", t_711B88);

// SceneRankingReward::init $_1: __func::~__func (deleting)
static void t_711B9C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711B9C, "SceneRankingReward::init $_1 __func::~__func (deleting)", t_711B9C);

// SceneRankingReward::init $_1: __func::__clone()
static uint32_t t_711BA4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6b03cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x711BA4, "SceneRankingReward::init $_1 __func::__clone()", t_711BA4);

// SceneRankingReward::init $_1: __func::__clone(__base*)
static void t_711BD0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b03cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x711BD0, "SceneRankingReward::init $_1 __func::__clone(__base*)", t_711BD0);

// SceneRankingReward::init $_1: __func::destroy
static void t_711BF0(uint32_t self) {
}
RW_FUNCTION(0x711BF0, "SceneRankingReward::init $_1 __func::destroy", t_711BF0);

// SceneRankingReward::init $_1: __func::destroy_deallocate
static void t_711BF2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711BF2, "SceneRankingReward::init $_1 __func::destroy_deallocate", t_711BF2);

// SceneRankingReward::init $_1: __func::target
static uint32_t t_711C04(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8f0c0u ? self + 4 : 0; }
RW_FUNCTION(0x711C04, "SceneRankingReward::init $_1 __func::target", t_711C04);

// SceneRankingReward::init $_1: __func::target_type
static uint32_t t_711C24() { return 0xfa3ef0u; }
RW_FUNCTION(0x711C24, "SceneRankingReward::init $_1 __func::target_type", t_711C24);

// SceneRankingReward::init $_0: __func::~__func (deleting)
static void t_711C38(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711C38, "SceneRankingReward::init $_0 __func::~__func (deleting)", t_711C38);

// SceneRankingReward::init $_0: __func::__clone()
static uint32_t t_711C40(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6b068u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x711C40, "SceneRankingReward::init $_0 __func::__clone()", t_711C40);

// SceneRankingReward::init $_0: __func::__clone(__base*)
static void t_711C6C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b068u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x711C6C, "SceneRankingReward::init $_0 __func::__clone(__base*)", t_711C6C);

// SceneRankingReward::init $_0: __func::destroy
static void t_711C8C(uint32_t self) {
}
RW_FUNCTION(0x711C8C, "SceneRankingReward::init $_0 __func::destroy", t_711C8C);

// SceneRankingReward::init $_0: __func::destroy_deallocate
static void t_711C8E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x711C8E, "SceneRankingReward::init $_0 __func::destroy_deallocate", t_711C8E);

// SceneRankingReward::init $_0: __func::target
static uint32_t t_711CA0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8f150u ? self + 4 : 0; }
RW_FUNCTION(0x711CA0, "SceneRankingReward::init $_0 __func::target", t_711CA0);

// SceneRankingReward::init $_0: __func::target_type
static uint32_t t_711CC0() { return 0xfa3f04u; }
RW_FUNCTION(0x711CC0, "SceneRankingReward::init $_0 __func::target_type", t_711CC0);
