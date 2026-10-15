// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// PresentBoxScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// PresentBoxScrollView::popUpAllGetPresent $_6: __func::~__func
static uint32_t t_50E778(uint32_t self) {
    st32(self, 0xf56260u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x50E778, "PresentBoxScrollView::popUpAllGetPresent $_6 __func::~__func", t_50E778);

// PresentBoxScrollView::popUpAllGetPresent $_6: __func::~__func (deleting)
static void t_50E7B4(uint32_t self) {
    st32(self, 0xf56260u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50E7B4, "PresentBoxScrollView::popUpAllGetPresent $_6 __func::~__func (deleting)", t_50E7B4);

// PresentBoxScrollView::popUpAllGetPresent $_6: __func::__clone()
static uint32_t t_50E7F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf56260u);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x50E7F4, "PresentBoxScrollView::popUpAllGetPresent $_6 __func::__clone()", t_50E7F4);

// PresentBoxScrollView::popUpAllGetPresent $_6: __func::__clone(__base*)
static void t_50E84C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56260u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x50E84C, "PresentBoxScrollView::popUpAllGetPresent $_6 __func::__clone(__base*)", t_50E84C);

// PresentBoxScrollView::popUpAllGetPresent $_6: __func::destroy
static void t_50E88C(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x50E88C, "PresentBoxScrollView::popUpAllGetPresent $_6 __func::destroy", t_50E88C);

// PresentBoxScrollView::popUpAllGetPresent $_6: __func::destroy_deallocate
static void t_50E8AC(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50E8AC, "PresentBoxScrollView::popUpAllGetPresent $_6 __func::destroy_deallocate", t_50E8AC);

// PresentBoxScrollView::popUpAllGetPresent $_6: __func::target
static uint32_t t_50EA64(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22640u ? self + 8 : 0; }
RW_FUNCTION(0x50EA64, "PresentBoxScrollView::popUpAllGetPresent $_6 __func::target", t_50EA64);

// PresentBoxScrollView::popUpAllGetPresent $_6: __func::target_type
static uint32_t t_50EA88() { return 0xf83a68u; }
RW_FUNCTION(0x50EA88, "PresentBoxScrollView::popUpAllGetPresent $_6 __func::target_type", t_50EA88);

// PresentBoxScrollView::popUpAllGetPresent $_5: __func::~__func
static uint32_t t_50EA9C(uint32_t self) {
    st32(self, 0xf5628cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x50EA9C, "PresentBoxScrollView::popUpAllGetPresent $_5 __func::~__func", t_50EA9C);

// PresentBoxScrollView::popUpAllGetPresent $_5: __func::~__func (deleting)
static void t_50EAD8(uint32_t self) {
    st32(self, 0xf5628cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50EAD8, "PresentBoxScrollView::popUpAllGetPresent $_5 __func::~__func (deleting)", t_50EAD8);

// PresentBoxScrollView::popUpAllGetPresent $_5: __func::__clone()
static uint32_t t_50EB18(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5628cu);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x50EB18, "PresentBoxScrollView::popUpAllGetPresent $_5 __func::__clone()", t_50EB18);

// PresentBoxScrollView::popUpAllGetPresent $_5: __func::__clone(__base*)
static void t_50EB70(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5628cu);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x50EB70, "PresentBoxScrollView::popUpAllGetPresent $_5 __func::__clone(__base*)", t_50EB70);

// PresentBoxScrollView::popUpAllGetPresent $_5: __func::destroy
static void t_50EBB0(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x50EBB0, "PresentBoxScrollView::popUpAllGetPresent $_5 __func::destroy", t_50EBB0);

// PresentBoxScrollView::popUpAllGetPresent $_5: __func::destroy_deallocate
static void t_50EBD0(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50EBD0, "PresentBoxScrollView::popUpAllGetPresent $_5 __func::destroy_deallocate", t_50EBD0);

// PresentBoxScrollView::popUpAllGetPresent $_5: __func::target
static uint32_t t_50F03C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22710u ? self + 8 : 0; }
RW_FUNCTION(0x50F03C, "PresentBoxScrollView::popUpAllGetPresent $_5 __func::target", t_50F03C);

// PresentBoxScrollView::popUpAllGetPresent $_5: __func::target_type
static uint32_t t_50F060() { return 0xf83a7cu; }
RW_FUNCTION(0x50F060, "PresentBoxScrollView::popUpAllGetPresent $_5 __func::target_type", t_50F060);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_: __func::~__func
static uint32_t t_50F074(uint32_t self) {
    st32(self, 0xf562b8u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x50F074, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_ __func::~__func", t_50F074);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_: __func::~__func (deleting)
static void t_50F0B0(uint32_t self) {
    st32(self, 0xf562b8u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F0B0, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_ __func::~__func (deleting)", t_50F0B0);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_: __func::__clone()
static uint32_t t_50F0F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf562b8u);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x50F0F0, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_ __func::__clone()", t_50F0F0);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_: __func::__clone(__base*)
static void t_50F148(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf562b8u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x50F148, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_ __func::__clone(__base*)", t_50F148);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_: __func::destroy
static void t_50F188(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x50F188, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_ __func::destroy", t_50F188);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_: __func::destroy_deallocate
static void t_50F1A8(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F1A8, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_ __func::destroy_deallocate", t_50F1A8);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_: __func::target
static uint32_t t_50F248(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe227f0u ? self + 8 : 0; }
RW_FUNCTION(0x50F248, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_ __func::target", t_50F248);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_: __func::target_type
static uint32_t t_50F26C() { return 0xf83a90u; }
RW_FUNCTION(0x50F26C, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_ __func::target_type", t_50F26C);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_: __func::~__func
static uint32_t t_50F280(uint32_t self) {
    st32(self, 0xf562e4u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x50F280, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_ __func::~__func", t_50F280);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_: __func::~__func (deleting)
static void t_50F2BC(uint32_t self) {
    st32(self, 0xf562e4u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F2BC, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_ __func::~__func (deleting)", t_50F2BC);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_: __func::__clone()
static uint32_t t_50F2FC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf562e4u);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x50F2FC, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_ __func::__clone()", t_50F2FC);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_: __func::__clone(__base*)
static void t_50F354(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf562e4u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x50F354, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_ __func::__clone(__base*)", t_50F354);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_: __func::destroy
static void t_50F394(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x50F394, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_ __func::destroy", t_50F394);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_: __func::destroy_deallocate
static void t_50F3B4(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F3B4, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_ __func::destroy_deallocate", t_50F3B4);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_: __func::target
static uint32_t t_50F414(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe228e0u ? self + 8 : 0; }
RW_FUNCTION(0x50F414, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_ __func::target", t_50F414);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_: __func::target_type
static uint32_t t_50F438() { return 0xf83aa4u; }
RW_FUNCTION(0x50F438, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_ __func::target_type", t_50F438);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_: __func::~__func (deleting)
static void t_50F44C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F44C, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_ __func::~__func (deleting)", t_50F44C);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_: __func::__clone()
static uint32_t t_50F454(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf56310u);
    }
    return v1;
}
RW_FUNCTION(0x50F454, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_ __func::__clone()", t_50F454);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_: __func::__clone(__base*)
static void t_50F478(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56310u);
    }
}
RW_FUNCTION(0x50F478, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_ __func::__clone(__base*)", t_50F478);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_: __func::destroy
static void t_50F494(uint32_t self) {
}
RW_FUNCTION(0x50F494, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_ __func::destroy", t_50F494);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_: __func::destroy_deallocate
static void t_50F496(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F496, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_ __func::destroy_deallocate", t_50F496);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_: __func::target
static uint32_t t_50F4A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe229d0u ? self + 4 : 0; }
RW_FUNCTION(0x50F4A0, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_ __func::target", t_50F4A0);

// PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_: __func::target_type
static uint32_t t_50F4C0() { return 0xf83ab8u; }
RW_FUNCTION(0x50F4C0, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE_ __func::target_type", t_50F4C0);

// PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_: __func::~__func (deleting)
static void t_50F4D4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F4D4, "PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_ __func::~__func (deleting)", t_50F4D4);

// PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_: __func::__clone()
static uint32_t t_50F4DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5633cu);
    }
    return v1;
}
RW_FUNCTION(0x50F4DC, "PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_ __func::__clone()", t_50F4DC);

// PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_: __func::__clone(__base*)
static void t_50F500(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5633cu);
    }
}
RW_FUNCTION(0x50F500, "PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_ __func::__clone(__base*)", t_50F500);

// PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_: __func::destroy
static void t_50F51C(uint32_t self) {
}
RW_FUNCTION(0x50F51C, "PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_ __func::destroy", t_50F51C);

// PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_: __func::destroy_deallocate
static void t_50F51E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F51E, "PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_ __func::destroy_deallocate", t_50F51E);

// PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_: __func::target
static uint32_t t_50F528(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22ac0u ? self + 4 : 0; }
RW_FUNCTION(0x50F528, "PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_ __func::target", t_50F528);

// PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_: __func::target_type
static uint32_t t_50F548() { return 0xf83accu; }
RW_FUNCTION(0x50F548, "PresentBoxScrollView::popUpAllGetPresent $_6 UlbE_ __func::target_type", t_50F548);

// PresentBoxScrollView::popUpGetPresent $_4: __func::~__func
static uint32_t t_50F55C(uint32_t self) {
    st32(self, 0xf56368u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x50F55C, "PresentBoxScrollView::popUpGetPresent $_4 __func::~__func", t_50F55C);

// PresentBoxScrollView::popUpGetPresent $_4: __func::~__func (deleting)
static void t_50F598(uint32_t self) {
    st32(self, 0xf56368u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F598, "PresentBoxScrollView::popUpGetPresent $_4 __func::~__func (deleting)", t_50F598);

// PresentBoxScrollView::popUpGetPresent $_4: __func::__clone()
static uint32_t t_50F5D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf56368u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x50F5D8, "PresentBoxScrollView::popUpGetPresent $_4 __func::__clone()", t_50F5D8);

// PresentBoxScrollView::popUpGetPresent $_4: __func::__clone(__base*)
static void t_50F634(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56368u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x50F634, "PresentBoxScrollView::popUpGetPresent $_4 __func::__clone(__base*)", t_50F634);

// PresentBoxScrollView::popUpGetPresent $_4: __func::destroy
static void t_50F674(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x50F674, "PresentBoxScrollView::popUpGetPresent $_4 __func::destroy", t_50F674);

// PresentBoxScrollView::popUpGetPresent $_4: __func::destroy_deallocate
static void t_50F694(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F694, "PresentBoxScrollView::popUpGetPresent $_4 __func::destroy_deallocate", t_50F694);

// PresentBoxScrollView::popUpGetPresent $_4: __func::target
static uint32_t t_50F708(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22ba0u ? self + 8 : 0; }
RW_FUNCTION(0x50F708, "PresentBoxScrollView::popUpGetPresent $_4 __func::target", t_50F708);

// PresentBoxScrollView::popUpGetPresent $_4: __func::target_type
static uint32_t t_50F72C() { return 0xf83ae0u; }
RW_FUNCTION(0x50F72C, "PresentBoxScrollView::popUpGetPresent $_4 __func::target_type", t_50F72C);

// PresentBoxScrollView::registItemTouchEvent $_3: __func::~__func (deleting)
static void t_50F740(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F740, "PresentBoxScrollView::registItemTouchEvent $_3 __func::~__func (deleting)", t_50F740);

// PresentBoxScrollView::registItemTouchEvent $_3: __func::__clone()
static uint32_t t_50F748(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf56394u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 16, v5);
        st32(v1 + 12, v4);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x50F748, "PresentBoxScrollView::registItemTouchEvent $_3 __func::__clone()", t_50F748);

// PresentBoxScrollView::registItemTouchEvent $_3: __func::__clone(__base*)
static void t_50F77C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56394u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x50F77C, "PresentBoxScrollView::registItemTouchEvent $_3 __func::__clone(__base*)", t_50F77C);

// PresentBoxScrollView::registItemTouchEvent $_3: __func::destroy
static void t_50F7A8(uint32_t self) {
}
RW_FUNCTION(0x50F7A8, "PresentBoxScrollView::registItemTouchEvent $_3 __func::destroy", t_50F7A8);

// PresentBoxScrollView::registItemTouchEvent $_3: __func::destroy_deallocate
static void t_50F7AA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F7AA, "PresentBoxScrollView::registItemTouchEvent $_3 __func::destroy_deallocate", t_50F7AA);

// PresentBoxScrollView::registItemTouchEvent $_3: __func::target
static uint32_t t_50F7C0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22c90u ? self + 4 : 0; }
RW_FUNCTION(0x50F7C0, "PresentBoxScrollView::registItemTouchEvent $_3 __func::target", t_50F7C0);

// PresentBoxScrollView::registItemTouchEvent $_3: __func::target_type
static uint32_t t_50F7E0() { return 0xf83af4u; }
RW_FUNCTION(0x50F7E0, "PresentBoxScrollView::registItemTouchEvent $_3 __func::target_type", t_50F7E0);

// PresentBoxScrollView::registItemTouchEvent $_7: __func::~__func (deleting)
static void t_50F9D0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50F9D0, "PresentBoxScrollView::registItemTouchEvent $_7 __func::~__func (deleting)", t_50F9D0);

// PresentBoxScrollView::registItemTouchEvent $_7: __func::__clone()
static uint32_t t_50F9D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf563c0u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x50F9D8, "PresentBoxScrollView::registItemTouchEvent $_7 __func::__clone()", t_50F9D8);

// PresentBoxScrollView::registItemTouchEvent $_7: __func::__clone(__base*)
static void t_50FA08(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf563c0u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x50FA08, "PresentBoxScrollView::registItemTouchEvent $_7 __func::__clone(__base*)", t_50FA08);

// PresentBoxScrollView::registItemTouchEvent $_7: __func::destroy
static void t_50FA2C(uint32_t self) {
}
RW_FUNCTION(0x50FA2C, "PresentBoxScrollView::registItemTouchEvent $_7 __func::destroy", t_50FA2C);

// PresentBoxScrollView::registItemTouchEvent $_7: __func::destroy_deallocate
static void t_50FA2E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50FA2E, "PresentBoxScrollView::registItemTouchEvent $_7 __func::destroy_deallocate", t_50FA2E);

// PresentBoxScrollView::registItemTouchEvent $_7: __func::target
static uint32_t t_50FA8C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22d60u ? self + 4 : 0; }
RW_FUNCTION(0x50FA8C, "PresentBoxScrollView::registItemTouchEvent $_7 __func::target", t_50FA8C);

// PresentBoxScrollView::registItemTouchEvent $_7: __func::target_type
static uint32_t t_50FAAC() { return 0xf83b08u; }
RW_FUNCTION(0x50FAAC, "PresentBoxScrollView::registItemTouchEvent $_7 __func::target_type", t_50FAAC);

// PresentBoxScrollView::registItemTouchEvent $_2: __func::~__func (deleting)
static void t_50FAC0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50FAC0, "PresentBoxScrollView::registItemTouchEvent $_2 __func::~__func (deleting)", t_50FAC0);

// PresentBoxScrollView::registItemTouchEvent $_2: __func::__clone()
static uint32_t t_50FAC8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf563ecu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x50FAC8, "PresentBoxScrollView::registItemTouchEvent $_2 __func::__clone()", t_50FAC8);

// PresentBoxScrollView::registItemTouchEvent $_2: __func::__clone(__base*)
static void t_50FB00(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf563ecu);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        uint32_t v6 = ld32(self + 24);
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x50FB00, "PresentBoxScrollView::registItemTouchEvent $_2 __func::__clone(__base*)", t_50FB00);

// PresentBoxScrollView::registItemTouchEvent $_2: __func::destroy
static void t_50FB2C(uint32_t self) {
}
RW_FUNCTION(0x50FB2C, "PresentBoxScrollView::registItemTouchEvent $_2 __func::destroy", t_50FB2C);

// PresentBoxScrollView::registItemTouchEvent $_2: __func::destroy_deallocate
static void t_50FB2E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50FB2E, "PresentBoxScrollView::registItemTouchEvent $_2 __func::destroy_deallocate", t_50FB2E);

// PresentBoxScrollView::registItemTouchEvent $_2: __func::target
static uint32_t t_50FC5C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22e50u ? self + 4 : 0; }
RW_FUNCTION(0x50FC5C, "PresentBoxScrollView::registItemTouchEvent $_2 __func::target", t_50FC5C);

// PresentBoxScrollView::registItemTouchEvent $_2: __func::target_type
static uint32_t t_50FC7C() { return 0xf83b1cu; }
RW_FUNCTION(0x50FC7C, "PresentBoxScrollView::registItemTouchEvent $_2 __func::target_type", t_50FC7C);

// PresentBoxScrollView::registItemTouchEvent $_1: __func::~__func (deleting)
static void t_50FC90(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50FC90, "PresentBoxScrollView::registItemTouchEvent $_1 __func::~__func (deleting)", t_50FC90);

// PresentBoxScrollView::registItemTouchEvent $_1: __func::__clone()
static uint32_t t_50FC98(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf56418u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x50FC98, "PresentBoxScrollView::registItemTouchEvent $_1 __func::__clone()", t_50FC98);

// PresentBoxScrollView::registItemTouchEvent $_1: __func::__clone(__base*)
static void t_50FCC8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56418u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x50FCC8, "PresentBoxScrollView::registItemTouchEvent $_1 __func::__clone(__base*)", t_50FCC8);

// PresentBoxScrollView::registItemTouchEvent $_1: __func::destroy
static void t_50FCEC(uint32_t self) {
}
RW_FUNCTION(0x50FCEC, "PresentBoxScrollView::registItemTouchEvent $_1 __func::destroy", t_50FCEC);

// PresentBoxScrollView::registItemTouchEvent $_1: __func::destroy_deallocate
static void t_50FCEE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50FCEE, "PresentBoxScrollView::registItemTouchEvent $_1 __func::destroy_deallocate", t_50FCEE);

// PresentBoxScrollView::registItemTouchEvent $_1: __func::target
static uint32_t t_50FE40(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe22f40u ? self + 4 : 0; }
RW_FUNCTION(0x50FE40, "PresentBoxScrollView::registItemTouchEvent $_1 __func::target", t_50FE40);

// PresentBoxScrollView::registItemTouchEvent $_1: __func::target_type
static uint32_t t_50FE60() { return 0xf83b30u; }
RW_FUNCTION(0x50FE60, "PresentBoxScrollView::registItemTouchEvent $_1 __func::target_type", t_50FE60);

// PresentBoxScrollView::changeRow $_0: __func::~__func
static uint32_t t_50FE74(uint32_t self) {
    st32(self, 0xf56444u);
    uint32_t v1 = ld8(self + 44);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 52);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x50FE74, "PresentBoxScrollView::changeRow $_0 __func::~__func", t_50FE74);

// PresentBoxScrollView::changeRow $_0: __func::~__func (deleting)
static void t_50FEA4(uint32_t self) {
    st32(self, 0xf56444u);
    uint32_t v1 = ld8(self + 44);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 52);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x50FEA4, "PresentBoxScrollView::changeRow $_0 __func::~__func (deleting)", t_50FEA4);

// PresentBoxScrollView::changeRow $_0: __func::destroy
static void t_510010(uint32_t self) {
    uint32_t v1 = ld8(self + 44);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 52);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x510010, "PresentBoxScrollView::changeRow $_0 __func::destroy", t_510010);

// PresentBoxScrollView::changeRow $_0: __func::destroy_deallocate
static void t_510024(uint32_t self) {
    uint32_t v1 = ld8(self + 44);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 52);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x510024, "PresentBoxScrollView::changeRow $_0 __func::destroy_deallocate", t_510024);

// PresentBoxScrollView::changeRow $_0: __func::target
static uint32_t t_5101F4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23030u ? self + 8 : 0; }
RW_FUNCTION(0x5101F4, "PresentBoxScrollView::changeRow $_0 __func::target", t_5101F4);

// PresentBoxScrollView::changeRow $_0: __func::target_type
static uint32_t t_510218() { return 0xf83b44u; }
RW_FUNCTION(0x510218, "PresentBoxScrollView::changeRow $_0 __func::target_type", t_510218);

// PresentBoxScrollView::changeRow $_0 UlbE0_: __func::~__func (deleting)
static void t_510270(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x510270, "PresentBoxScrollView::changeRow $_0 UlbE0_ __func::~__func (deleting)", t_510270);

// PresentBoxScrollView::changeRow $_0 UlbE0_: __func::__clone()
static uint32_t t_510278(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf56470u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x510278, "PresentBoxScrollView::changeRow $_0 UlbE0_ __func::__clone()", t_510278);

// PresentBoxScrollView::changeRow $_0 UlbE0_: __func::__clone(__base*)
static void t_5102AC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56470u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x5102AC, "PresentBoxScrollView::changeRow $_0 UlbE0_ __func::__clone(__base*)", t_5102AC);

// PresentBoxScrollView::changeRow $_0 UlbE0_: __func::destroy
static void t_5102D8(uint32_t self) {
}
RW_FUNCTION(0x5102D8, "PresentBoxScrollView::changeRow $_0 UlbE0_ __func::destroy", t_5102D8);

// PresentBoxScrollView::changeRow $_0 UlbE0_: __func::destroy_deallocate
static void t_5102DA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5102DA, "PresentBoxScrollView::changeRow $_0 UlbE0_ __func::destroy_deallocate", t_5102DA);

// PresentBoxScrollView::changeRow $_0 UlbE0_: __func::target
static uint32_t t_5103F0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23110u ? self + 8 : 0; }
RW_FUNCTION(0x5103F0, "PresentBoxScrollView::changeRow $_0 UlbE0_ __func::target", t_5103F0);

// PresentBoxScrollView::changeRow $_0 UlbE0_: __func::target_type
static uint32_t t_510414() { return 0xf83b58u; }
RW_FUNCTION(0x510414, "PresentBoxScrollView::changeRow $_0 UlbE0_ __func::target_type", t_510414);

// PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_: __func::~__func (deleting)
static void t_510428(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x510428, "PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_ __func::~__func (deleting)", t_510428);

// PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_: __func::__clone()
static uint32_t t_510430(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5649cu);
    }
    return v1;
}
RW_FUNCTION(0x510430, "PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_ __func::__clone()", t_510430);

// PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_: __func::__clone(__base*)
static void t_510454(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5649cu);
    }
}
RW_FUNCTION(0x510454, "PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_ __func::__clone(__base*)", t_510454);

// PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_: __func::destroy
static void t_510470(uint32_t self) {
}
RW_FUNCTION(0x510470, "PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_ __func::destroy", t_510470);

// PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_: __func::destroy_deallocate
static void t_510472(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x510472, "PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_ __func::destroy_deallocate", t_510472);

// PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_: __func::target
static uint32_t t_51047C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23230u ? self + 4 : 0; }
RW_FUNCTION(0x51047C, "PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_ __func::target", t_51047C);

// PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_: __func::target_type
static uint32_t t_51049C() { return 0xf83b6cu; }
RW_FUNCTION(0x51049C, "PresentBoxScrollView::changeRow $_0 UlbE0_ UlbE_ __func::target_type", t_51049C);

// PresentBoxScrollView::changeRow $_0 UlbE_: __func::~__func (deleting)
static void t_5104B0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5104B0, "PresentBoxScrollView::changeRow $_0 UlbE_ __func::~__func (deleting)", t_5104B0);

// PresentBoxScrollView::changeRow $_0 UlbE_: __func::__clone()
static uint32_t t_5104B8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf564c8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5104B8, "PresentBoxScrollView::changeRow $_0 UlbE_ __func::__clone()", t_5104B8);

// PresentBoxScrollView::changeRow $_0 UlbE_: __func::__clone(__base*)
static void t_5104E4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf564c8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5104E4, "PresentBoxScrollView::changeRow $_0 UlbE_ __func::__clone(__base*)", t_5104E4);

// PresentBoxScrollView::changeRow $_0 UlbE_: __func::destroy
static void t_510504(uint32_t self) {
}
RW_FUNCTION(0x510504, "PresentBoxScrollView::changeRow $_0 UlbE_ __func::destroy", t_510504);

// PresentBoxScrollView::changeRow $_0 UlbE_: __func::destroy_deallocate
static void t_510506(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x510506, "PresentBoxScrollView::changeRow $_0 UlbE_ __func::destroy_deallocate", t_510506);

// PresentBoxScrollView::changeRow $_0 UlbE_: __func::target
static uint32_t t_510530(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe23350u ? self + 4 : 0; }
RW_FUNCTION(0x510530, "PresentBoxScrollView::changeRow $_0 UlbE_ __func::target", t_510530);

// PresentBoxScrollView::changeRow $_0 UlbE_: __func::target_type
static uint32_t t_510550() { return 0xf83b80u; }
RW_FUNCTION(0x510550, "PresentBoxScrollView::changeRow $_0 UlbE_ __func::target_type", t_510550);
