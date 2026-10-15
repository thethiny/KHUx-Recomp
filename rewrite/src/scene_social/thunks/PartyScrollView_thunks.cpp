// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// PartyScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// PartyScrollView::registItemTouchEvent $_8: __func::~__func (deleting)
static void t_51F0E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F0E8, "PartyScrollView::registItemTouchEvent $_8 __func::~__func (deleting)", t_51F0E8);

// PartyScrollView::registItemTouchEvent $_8: __func::__clone()
static uint32_t t_51F0F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf56a48u);
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
RW_FUNCTION(0x51F0F0, "PartyScrollView::registItemTouchEvent $_8 __func::__clone()", t_51F0F0);

// PartyScrollView::registItemTouchEvent $_8: __func::__clone(__base*)
static void t_51F124(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56a48u);
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
RW_FUNCTION(0x51F124, "PartyScrollView::registItemTouchEvent $_8 __func::__clone(__base*)", t_51F124);

// PartyScrollView::registItemTouchEvent $_8: __func::destroy
static void t_51F150(uint32_t self) {
}
RW_FUNCTION(0x51F150, "PartyScrollView::registItemTouchEvent $_8 __func::destroy", t_51F150);

// PartyScrollView::registItemTouchEvent $_8: __func::destroy_deallocate
static void t_51F152(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F152, "PartyScrollView::registItemTouchEvent $_8 __func::destroy_deallocate", t_51F152);

// PartyScrollView::registItemTouchEvent $_8: __func::target
static uint32_t t_51F168(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe24d70u ? self + 4 : 0; }
RW_FUNCTION(0x51F168, "PartyScrollView::registItemTouchEvent $_8 __func::target", t_51F168);

// PartyScrollView::registItemTouchEvent $_8: __func::target_type
static uint32_t t_51F188() { return 0xf8550cu; }
RW_FUNCTION(0x51F188, "PartyScrollView::registItemTouchEvent $_8 __func::target_type", t_51F188);

// PartyScrollView::registItemTouchEvent $_9: __func::~__func (deleting)
static void t_51F370(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F370, "PartyScrollView::registItemTouchEvent $_9 __func::~__func (deleting)", t_51F370);

// PartyScrollView::registItemTouchEvent $_9: __func::__clone()
static uint32_t t_51F378(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf56a74u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x51F378, "PartyScrollView::registItemTouchEvent $_9 __func::__clone()", t_51F378);

// PartyScrollView::registItemTouchEvent $_9: __func::__clone(__base*)
static void t_51F3A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56a74u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x51F3A8, "PartyScrollView::registItemTouchEvent $_9 __func::__clone(__base*)", t_51F3A8);

// PartyScrollView::registItemTouchEvent $_9: __func::destroy
static void t_51F3CC(uint32_t self) {
}
RW_FUNCTION(0x51F3CC, "PartyScrollView::registItemTouchEvent $_9 __func::destroy", t_51F3CC);

// PartyScrollView::registItemTouchEvent $_9: __func::destroy_deallocate
static void t_51F3CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F3CE, "PartyScrollView::registItemTouchEvent $_9 __func::destroy_deallocate", t_51F3CE);

// PartyScrollView::registItemTouchEvent $_9: __func::target
static uint32_t t_51F42C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe24e40u ? self + 4 : 0; }
RW_FUNCTION(0x51F42C, "PartyScrollView::registItemTouchEvent $_9 __func::target", t_51F42C);

// PartyScrollView::registItemTouchEvent $_9: __func::target_type
static uint32_t t_51F44C() { return 0xf85520u; }
RW_FUNCTION(0x51F44C, "PartyScrollView::registItemTouchEvent $_9 __func::target_type", t_51F44C);

// PartyScrollView::registItemTouchEvent $_7: __func::~__func (deleting)
static void t_51F460(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F460, "PartyScrollView::registItemTouchEvent $_7 __func::~__func (deleting)", t_51F460);

// PartyScrollView::registItemTouchEvent $_7: __func::__clone()
static uint32_t t_51F468(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf56aa0u);
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
RW_FUNCTION(0x51F468, "PartyScrollView::registItemTouchEvent $_7 __func::__clone()", t_51F468);

// PartyScrollView::registItemTouchEvent $_7: __func::__clone(__base*)
static void t_51F4A0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56aa0u);
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
RW_FUNCTION(0x51F4A0, "PartyScrollView::registItemTouchEvent $_7 __func::__clone(__base*)", t_51F4A0);

// PartyScrollView::registItemTouchEvent $_7: __func::destroy
static void t_51F4CC(uint32_t self) {
}
RW_FUNCTION(0x51F4CC, "PartyScrollView::registItemTouchEvent $_7 __func::destroy", t_51F4CC);

// PartyScrollView::registItemTouchEvent $_7: __func::destroy_deallocate
static void t_51F4CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F4CE, "PartyScrollView::registItemTouchEvent $_7 __func::destroy_deallocate", t_51F4CE);

// PartyScrollView::registItemTouchEvent $_7: __func::target
static uint32_t t_51F5E0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe24f30u ? self + 4 : 0; }
RW_FUNCTION(0x51F5E0, "PartyScrollView::registItemTouchEvent $_7 __func::target", t_51F5E0);

// PartyScrollView::registItemTouchEvent $_7: __func::target_type
static uint32_t t_51F600() { return 0xf85534u; }
RW_FUNCTION(0x51F600, "PartyScrollView::registItemTouchEvent $_7 __func::target_type", t_51F600);

// PartyScrollView::registItemTouchEvent $_6: __func::~__func (deleting)
static void t_51F614(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F614, "PartyScrollView::registItemTouchEvent $_6 __func::~__func (deleting)", t_51F614);

// PartyScrollView::registItemTouchEvent $_6: __func::__clone()
static uint32_t t_51F61C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf56accu);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x51F61C, "PartyScrollView::registItemTouchEvent $_6 __func::__clone()", t_51F61C);

// PartyScrollView::registItemTouchEvent $_6: __func::__clone(__base*)
static void t_51F64C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56accu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x51F64C, "PartyScrollView::registItemTouchEvent $_6 __func::__clone(__base*)", t_51F64C);

// PartyScrollView::registItemTouchEvent $_6: __func::destroy
static void t_51F670(uint32_t self) {
}
RW_FUNCTION(0x51F670, "PartyScrollView::registItemTouchEvent $_6 __func::destroy", t_51F670);

// PartyScrollView::registItemTouchEvent $_6: __func::destroy_deallocate
static void t_51F672(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F672, "PartyScrollView::registItemTouchEvent $_6 __func::destroy_deallocate", t_51F672);

// PartyScrollView::registItemTouchEvent $_6: __func::target
static uint32_t t_51F7A4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe25020u ? self + 4 : 0; }
RW_FUNCTION(0x51F7A4, "PartyScrollView::registItemTouchEvent $_6 __func::target", t_51F7A4);

// PartyScrollView::registItemTouchEvent $_6: __func::target_type
static uint32_t t_51F7C4() { return 0xf85548u; }
RW_FUNCTION(0x51F7C4, "PartyScrollView::registItemTouchEvent $_6 __func::target_type", t_51F7C4);

// PartyScrollView::changeRow $_5: __func::~__func (deleting)
static void t_51F7D8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F7D8, "PartyScrollView::changeRow $_5 __func::~__func (deleting)", t_51F7D8);

// PartyScrollView::changeRow $_5: __func::__clone()
static uint32_t t_51F7E0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf56af8u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x51F7E0, "PartyScrollView::changeRow $_5 __func::__clone()", t_51F7E0);

// PartyScrollView::changeRow $_5: __func::__clone(__base*)
static void t_51F810(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56af8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x51F810, "PartyScrollView::changeRow $_5 __func::__clone(__base*)", t_51F810);

// PartyScrollView::changeRow $_5: __func::destroy
static void t_51F838(uint32_t self) {
}
RW_FUNCTION(0x51F838, "PartyScrollView::changeRow $_5 __func::destroy", t_51F838);

// PartyScrollView::changeRow $_5: __func::destroy_deallocate
static void t_51F83A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F83A, "PartyScrollView::changeRow $_5 __func::destroy_deallocate", t_51F83A);

// PartyScrollView::changeRow $_5: __func::target
static uint32_t t_51F954(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe25100u ? self + 4 : 0; }
RW_FUNCTION(0x51F954, "PartyScrollView::changeRow $_5 __func::target", t_51F954);

// PartyScrollView::changeRow $_5: __func::target_type
static uint32_t t_51F974() { return 0xf8555cu; }
RW_FUNCTION(0x51F974, "PartyScrollView::changeRow $_5 __func::target_type", t_51F974);

// PartyScrollView::changeRow $_5 UlvE_: __func::~__func (deleting)
static void t_51F988(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F988, "PartyScrollView::changeRow $_5 UlvE_ __func::~__func (deleting)", t_51F988);

// PartyScrollView::changeRow $_5 UlvE_: __func::__clone()
static uint32_t t_51F990(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf56b24u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x51F990, "PartyScrollView::changeRow $_5 UlvE_ __func::__clone()", t_51F990);

// PartyScrollView::changeRow $_5 UlvE_: __func::__clone(__base*)
static void t_51F9BC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56b24u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x51F9BC, "PartyScrollView::changeRow $_5 UlvE_ __func::__clone(__base*)", t_51F9BC);

// PartyScrollView::changeRow $_5 UlvE_: __func::destroy
static void t_51F9DC(uint32_t self) {
}
RW_FUNCTION(0x51F9DC, "PartyScrollView::changeRow $_5 UlvE_ __func::destroy", t_51F9DC);

// PartyScrollView::changeRow $_5 UlvE_: __func::destroy_deallocate
static void t_51F9DE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51F9DE, "PartyScrollView::changeRow $_5 UlvE_ __func::destroy_deallocate", t_51F9DE);

// PartyScrollView::changeRow $_5 UlvE_: __func::target
static uint32_t t_51F9FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe251e0u ? self + 4 : 0; }
RW_FUNCTION(0x51F9FC, "PartyScrollView::changeRow $_5 UlvE_ __func::target", t_51F9FC);

// PartyScrollView::changeRow $_5 UlvE_: __func::target_type
static uint32_t t_51FA1C() { return 0xf85570u; }
RW_FUNCTION(0x51FA1C, "PartyScrollView::changeRow $_5 UlvE_ __func::target_type", t_51FA1C);

// PartyScrollView::changeRow $_4: __func::~__func (deleting)
static void t_51FA30(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51FA30, "PartyScrollView::changeRow $_4 __func::~__func (deleting)", t_51FA30);

// PartyScrollView::changeRow $_4: __func::__clone()
static uint32_t t_51FA38(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf56b50u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x51FA38, "PartyScrollView::changeRow $_4 __func::__clone()", t_51FA38);

// PartyScrollView::changeRow $_4: __func::__clone(__base*)
static void t_51FA68(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf56b50u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x51FA68, "PartyScrollView::changeRow $_4 __func::__clone(__base*)", t_51FA68);

// PartyScrollView::changeRow $_4: __func::destroy
static void t_51FA90(uint32_t self) {
}
RW_FUNCTION(0x51FA90, "PartyScrollView::changeRow $_4 __func::destroy", t_51FA90);

// PartyScrollView::changeRow $_4: __func::destroy_deallocate
static void t_51FA92(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51FA92, "PartyScrollView::changeRow $_4 __func::destroy_deallocate", t_51FA92);

// PartyScrollView::changeRow $_4: __func::target
static uint32_t t_51FBAC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe252e0u ? self + 4 : 0; }
RW_FUNCTION(0x51FBAC, "PartyScrollView::changeRow $_4 __func::target", t_51FBAC);

// PartyScrollView::changeRow $_4: __func::target_type
static uint32_t t_51FBCC() { return 0xf85584u; }
RW_FUNCTION(0x51FBCC, "PartyScrollView::changeRow $_4 __func::target_type", t_51FBCC);

// PartyScrollView::changeRow $_4 UlvE_: __func::~__func (deleting)
static void t_51FBE0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x51FBE0, "PartyScrollView::changeRow $_4 UlvE_ __func::~__func (deleting)", t_51FBE0);

// PartyScrollView::changeRow $_4 UlvE_: __func::__clone()
static uint32_t t_51FBE8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf56b7cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x51FBE8, "PartyScrollView::changeRow $_4 UlvE_ __func::__clone()", t_51FBE8);
