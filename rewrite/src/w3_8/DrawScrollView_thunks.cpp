// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// DrawScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// DrawScrollView::openCheckbondsPopup $_20: __func::~__func
static uint32_t t_5B13B4(uint32_t self) {
    st32(self, 0xf5c3a8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5B13B4, "DrawScrollView::openCheckbondsPopup $_20 __func::~__func", t_5B13B4);

// DrawScrollView::openCheckbondsPopup $_20: __func::~__func (deleting)
static void t_5B13F0(uint32_t self) {
    st32(self, 0xf5c3a8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B13F0, "DrawScrollView::openCheckbondsPopup $_20 __func::~__func (deleting)", t_5B13F0);

// DrawScrollView::openCheckbondsPopup $_20: __func::__clone()
static uint32_t t_5B1430(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5c3a8u);
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
RW_FUNCTION(0x5B1430, "DrawScrollView::openCheckbondsPopup $_20 __func::__clone()", t_5B1430);

// DrawScrollView::openCheckbondsPopup $_20: __func::__clone(__base*)
static void t_5B148C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c3a8u);
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
RW_FUNCTION(0x5B148C, "DrawScrollView::openCheckbondsPopup $_20 __func::__clone(__base*)", t_5B148C);

// DrawScrollView::openCheckbondsPopup $_20: __func::destroy
static void t_5B14CC(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5B14CC, "DrawScrollView::openCheckbondsPopup $_20 __func::destroy", t_5B14CC);

// DrawScrollView::openCheckbondsPopup $_20: __func::destroy_deallocate
static void t_5B14EC(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B14EC, "DrawScrollView::openCheckbondsPopup $_20 __func::destroy_deallocate", t_5B14EC);

// DrawScrollView::openCheckbondsPopup $_20: __func::target
static uint32_t t_5B15B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe469a0u ? self + 8 : 0; }
RW_FUNCTION(0x5B15B0, "DrawScrollView::openCheckbondsPopup $_20 __func::target", t_5B15B0);

// DrawScrollView::openCheckbondsPopup $_20: __func::target_type
static uint32_t t_5B15D4() { return 0xf8df6cu; }
RW_FUNCTION(0x5B15D4, "DrawScrollView::openCheckbondsPopup $_20 __func::target_type", t_5B15D4);

// DrawScrollView::openCheckbondsPopup $_17 UlvE_: __func::~__func
static uint32_t t_5B1740(uint32_t self) {
    st32(self, 0xf5c3d4u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x5B1740, "DrawScrollView::openCheckbondsPopup $_17 UlvE_ __func::~__func", t_5B1740);

// DrawScrollView::openCheckbondsPopup $_17 UlvE_: __func::~__func (deleting)
static void t_5B177C(uint32_t self) {
    st32(self, 0xf5c3d4u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B177C, "DrawScrollView::openCheckbondsPopup $_17 UlvE_ __func::~__func (deleting)", t_5B177C);

// DrawScrollView::openCheckbondsPopup $_17 UlvE_: __func::__clone()
static uint32_t t_5B17BC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf5c3d4u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
        uint32_t v6 = ld8(self + 24);
        st8(v1 + 24, v6);
        call(0x4683cd, {v1 + 32, self + 32});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5B17BC, "DrawScrollView::openCheckbondsPopup $_17 UlvE_ __func::__clone()", t_5B17BC);

// DrawScrollView::openCheckbondsPopup $_17 UlvE_: __func::__clone(__base*)
static void t_5B181C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c3d4u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        uint32_t v5 = ld8(self + 24);
        st8(p + 24, v5);
        call(0x4683cd, {p + 32, self + 32});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5B181C, "DrawScrollView::openCheckbondsPopup $_17 UlvE_ __func::__clone(__base*)", t_5B181C);

// DrawScrollView::openCheckbondsPopup $_17 UlvE_: __func::destroy
static void t_5B1860(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x5B1860, "DrawScrollView::openCheckbondsPopup $_17 UlvE_ __func::destroy", t_5B1860);

// DrawScrollView::openCheckbondsPopup $_17 UlvE_: __func::destroy_deallocate
static void t_5B1880(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B1880, "DrawScrollView::openCheckbondsPopup $_17 UlvE_ __func::destroy_deallocate", t_5B1880);

// DrawScrollView::openCheckbondsPopup $_17 UlvE_: __func::target
static uint32_t t_5B18FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe46a80u ? self + 8 : 0; }
RW_FUNCTION(0x5B18FC, "DrawScrollView::openCheckbondsPopup $_17 UlvE_ __func::target", t_5B18FC);

// DrawScrollView::openCheckbondsPopup $_17 UlvE_: __func::target_type
static uint32_t t_5B1920() { return 0xf8df80u; }
RW_FUNCTION(0x5B1920, "DrawScrollView::openCheckbondsPopup $_17 UlvE_ __func::target_type", t_5B1920);

// DrawScrollView::openCheckbondsPopup $_19: __func::~__func
static uint32_t t_5B1934(uint32_t self) {
    st32(self, 0xf5c400u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5B1934, "DrawScrollView::openCheckbondsPopup $_19 __func::~__func", t_5B1934);

// DrawScrollView::openCheckbondsPopup $_19: __func::~__func (deleting)
static void t_5B1970(uint32_t self) {
    st32(self, 0xf5c400u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B1970, "DrawScrollView::openCheckbondsPopup $_19 __func::~__func (deleting)", t_5B1970);

// DrawScrollView::openCheckbondsPopup $_19: __func::__clone()
static uint32_t t_5B19B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5c400u);
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
RW_FUNCTION(0x5B19B0, "DrawScrollView::openCheckbondsPopup $_19 __func::__clone()", t_5B19B0);

// DrawScrollView::openCheckbondsPopup $_19: __func::__clone(__base*)
static void t_5B1A0C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c400u);
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
RW_FUNCTION(0x5B1A0C, "DrawScrollView::openCheckbondsPopup $_19 __func::__clone(__base*)", t_5B1A0C);

// DrawScrollView::openCheckbondsPopup $_19: __func::destroy
static void t_5B1A4C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5B1A4C, "DrawScrollView::openCheckbondsPopup $_19 __func::destroy", t_5B1A4C);

// DrawScrollView::openCheckbondsPopup $_19: __func::destroy_deallocate
static void t_5B1A6C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B1A6C, "DrawScrollView::openCheckbondsPopup $_19 __func::destroy_deallocate", t_5B1A6C);

// DrawScrollView::openCheckbondsPopup $_19: __func::target
static uint32_t t_5B1AB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe46b60u ? self + 8 : 0; }
RW_FUNCTION(0x5B1AB8, "DrawScrollView::openCheckbondsPopup $_19 __func::target", t_5B1AB8);

// DrawScrollView::openCheckbondsPopup $_19: __func::target_type
static uint32_t t_5B1ADC() { return 0xf8df94u; }
RW_FUNCTION(0x5B1ADC, "DrawScrollView::openCheckbondsPopup $_19 __func::target_type", t_5B1ADC);

// DrawScrollView::openCheckbondsPopup $_18: __func::~__func (deleting)
static void t_5B1AF0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B1AF0, "DrawScrollView::openCheckbondsPopup $_18 __func::~__func (deleting)", t_5B1AF0);

// DrawScrollView::openCheckbondsPopup $_18: __func::__clone()
static uint32_t t_5B1AF8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c42cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5B1AF8, "DrawScrollView::openCheckbondsPopup $_18 __func::__clone()", t_5B1AF8);

// DrawScrollView::openCheckbondsPopup $_18: __func::__clone(__base*)
static void t_5B1B24(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c42cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5B1B24, "DrawScrollView::openCheckbondsPopup $_18 __func::__clone(__base*)", t_5B1B24);

// DrawScrollView::openCheckbondsPopup $_18: __func::destroy
static void t_5B1B44(uint32_t self) {
}
RW_FUNCTION(0x5B1B44, "DrawScrollView::openCheckbondsPopup $_18 __func::destroy", t_5B1B44);

// DrawScrollView::openCheckbondsPopup $_18: __func::destroy_deallocate
static void t_5B1B46(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B1B46, "DrawScrollView::openCheckbondsPopup $_18 __func::destroy_deallocate", t_5B1B46);

// DrawScrollView::openCheckbondsPopup $_18: __func::target
static uint32_t t_5B1B58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe46c30u ? self + 4 : 0; }
RW_FUNCTION(0x5B1B58, "DrawScrollView::openCheckbondsPopup $_18 __func::target", t_5B1B58);

// DrawScrollView::openCheckbondsPopup $_18: __func::target_type
static uint32_t t_5B1B78() { return 0xf8dfa8u; }
RW_FUNCTION(0x5B1B78, "DrawScrollView::openCheckbondsPopup $_18 __func::target_type", t_5B1B78);

// DrawScrollView::openCheckbondsPopup $_16: __func::~__func (deleting)
static void t_5B1B8C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B1B8C, "DrawScrollView::openCheckbondsPopup $_16 __func::~__func (deleting)", t_5B1B8C);

// DrawScrollView::openCheckbondsPopup $_16: __func::__clone()
static uint32_t t_5B1B94(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c458u);
    }
    return v1;
}
RW_FUNCTION(0x5B1B94, "DrawScrollView::openCheckbondsPopup $_16 __func::__clone()", t_5B1B94);

// DrawScrollView::openCheckbondsPopup $_16: __func::__clone(__base*)
static void t_5B1BB8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c458u);
    }
}
RW_FUNCTION(0x5B1BB8, "DrawScrollView::openCheckbondsPopup $_16 __func::__clone(__base*)", t_5B1BB8);

// DrawScrollView::openCheckbondsPopup $_16: __func::destroy
static void t_5B1BD4(uint32_t self) {
}
RW_FUNCTION(0x5B1BD4, "DrawScrollView::openCheckbondsPopup $_16 __func::destroy", t_5B1BD4);

// DrawScrollView::openCheckbondsPopup $_16: __func::destroy_deallocate
static void t_5B1BD6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B1BD6, "DrawScrollView::openCheckbondsPopup $_16 __func::destroy_deallocate", t_5B1BD6);

// DrawScrollView::openCheckbondsPopup $_16: __func::target
static uint32_t t_5B1BF4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe46d30u ? self + 4 : 0; }
RW_FUNCTION(0x5B1BF4, "DrawScrollView::openCheckbondsPopup $_16 __func::target", t_5B1BF4);

// DrawScrollView::openCheckbondsPopup $_16: __func::target_type
static uint32_t t_5B1C14() { return 0xf8dfbcu; }
RW_FUNCTION(0x5B1C14, "DrawScrollView::openCheckbondsPopup $_16 __func::target_type", t_5B1C14);

// DrawScrollView::openCheckRaisePopup $_15: __func::~__func
static uint32_t t_5B1C28(uint32_t self) {
    st32(self, 0xf5c484u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5B1C28, "DrawScrollView::openCheckRaisePopup $_15 __func::~__func", t_5B1C28);

// DrawScrollView::openCheckRaisePopup $_15: __func::~__func (deleting)
static void t_5B1C64(uint32_t self) {
    st32(self, 0xf5c484u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B1C64, "DrawScrollView::openCheckRaisePopup $_15 __func::~__func (deleting)", t_5B1C64);

// DrawScrollView::openCheckRaisePopup $_15: __func::__clone()
static uint32_t t_5B1CA4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5c484u);
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
RW_FUNCTION(0x5B1CA4, "DrawScrollView::openCheckRaisePopup $_15 __func::__clone()", t_5B1CA4);

// DrawScrollView::openCheckRaisePopup $_15: __func::__clone(__base*)
static void t_5B1D00(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c484u);
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
RW_FUNCTION(0x5B1D00, "DrawScrollView::openCheckRaisePopup $_15 __func::__clone(__base*)", t_5B1D00);

// DrawScrollView::openCheckRaisePopup $_15: __func::destroy
static void t_5B1D40(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5B1D40, "DrawScrollView::openCheckRaisePopup $_15 __func::destroy", t_5B1D40);

// DrawScrollView::openCheckRaisePopup $_15: __func::destroy_deallocate
static void t_5B1D60(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5B1D60, "DrawScrollView::openCheckRaisePopup $_15 __func::destroy_deallocate", t_5B1D60);
