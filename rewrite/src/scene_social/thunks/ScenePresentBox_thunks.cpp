// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ScenePresentBox lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ScenePresentBox::openPresentBoxDialogMain $_13: __func::~__func
static uint32_t t_6D09A0(uint32_t self) {
    st32(self, 0xf6788cu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x6D09A0, "ScenePresentBox::openPresentBoxDialogMain $_13 __func::~__func", t_6D09A0);

// ScenePresentBox::openPresentBoxDialogMain $_13: __func::~__func (deleting)
static void t_6D09DC(uint32_t self) {
    st32(self, 0xf6788cu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D09DC, "ScenePresentBox::openPresentBoxDialogMain $_13 __func::~__func (deleting)", t_6D09DC);

// ScenePresentBox::openPresentBoxDialogMain $_13: __func::__clone()
static uint32_t t_6D0A1C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf6788cu);
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
RW_FUNCTION(0x6D0A1C, "ScenePresentBox::openPresentBoxDialogMain $_13 __func::__clone()", t_6D0A1C);

// ScenePresentBox::openPresentBoxDialogMain $_13: __func::__clone(__base*)
static void t_6D0A78(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6788cu);
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
RW_FUNCTION(0x6D0A78, "ScenePresentBox::openPresentBoxDialogMain $_13 __func::__clone(__base*)", t_6D0A78);

// ScenePresentBox::openPresentBoxDialogMain $_13: __func::destroy
static void t_6D0AB8(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x6D0AB8, "ScenePresentBox::openPresentBoxDialogMain $_13 __func::destroy", t_6D0AB8);

// ScenePresentBox::openPresentBoxDialogMain $_13: __func::destroy_deallocate
static void t_6D0AD8(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D0AD8, "ScenePresentBox::openPresentBoxDialogMain $_13 __func::destroy_deallocate", t_6D0AD8);

// ScenePresentBox::openPresentBoxDialogMain $_13: __func::target
static uint32_t t_6D0C44(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7eb60u ? self + 8 : 0; }
RW_FUNCTION(0x6D0C44, "ScenePresentBox::openPresentBoxDialogMain $_13 __func::target", t_6D0C44);

// ScenePresentBox::openPresentBoxDialogMain $_13: __func::target_type
static uint32_t t_6D0C68() { return 0xfa04c0u; }
RW_FUNCTION(0x6D0C68, "ScenePresentBox::openPresentBoxDialogMain $_13 __func::target_type", t_6D0C68);

// ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_: __func::~__func
static uint32_t t_6D0C7C(uint32_t self) {
    st32(self, 0xf678b8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x6D0C7C, "ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_ __func::~__func", t_6D0C7C);

// ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_: __func::~__func (deleting)
static void t_6D0CB8(uint32_t self) {
    st32(self, 0xf678b8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D0CB8, "ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_ __func::~__func (deleting)", t_6D0CB8);

// ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_: __func::__clone()
static uint32_t t_6D0CF8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf678b8u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v6 = ld8(self + 48);
        st8(v1 + 48, v6);
    }
    return v1;
}
RW_FUNCTION(0x6D0CF8, "ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_ __func::__clone()", t_6D0CF8);

// ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_: __func::__clone(__base*)
static void t_6D0D5C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf678b8u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v5 = ld8(self + 48);
        st8(p + 48, v5);
    }
}
RW_FUNCTION(0x6D0D5C, "ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_ __func::__clone(__base*)", t_6D0D5C);

// ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_: __func::destroy
static void t_6D0DA8(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x6D0DA8, "ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_ __func::destroy", t_6D0DA8);

// ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_: __func::destroy_deallocate
static void t_6D0DC8(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D0DC8, "ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_ __func::destroy_deallocate", t_6D0DC8);

// ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_: __func::target
static uint32_t t_6D0F98(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7ec40u ? self + 8 : 0; }
RW_FUNCTION(0x6D0F98, "ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_ __func::target", t_6D0F98);

// ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_: __func::target_type
static uint32_t t_6D0FBC() { return 0xfa04d4u; }
RW_FUNCTION(0x6D0FBC, "ScenePresentBox::openPresentBoxDialogMain $_8 UlvE_ __func::target_type", t_6D0FBC);

// ScenePresentBox::openPresentBoxDialogMain $_12: __func::~__func (deleting)
static void t_6D0FD0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D0FD0, "ScenePresentBox::openPresentBoxDialogMain $_12 __func::~__func (deleting)", t_6D0FD0);

// ScenePresentBox::openPresentBoxDialogMain $_12: __func::__clone()
static uint32_t t_6D0FD8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf678e4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D0FD8, "ScenePresentBox::openPresentBoxDialogMain $_12 __func::__clone()", t_6D0FD8);

// ScenePresentBox::openPresentBoxDialogMain $_12: __func::__clone(__base*)
static void t_6D1004(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf678e4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D1004, "ScenePresentBox::openPresentBoxDialogMain $_12 __func::__clone(__base*)", t_6D1004);

// ScenePresentBox::openPresentBoxDialogMain $_12: __func::destroy
static void t_6D1024(uint32_t self) {
}
RW_FUNCTION(0x6D1024, "ScenePresentBox::openPresentBoxDialogMain $_12 __func::destroy", t_6D1024);

// ScenePresentBox::openPresentBoxDialogMain $_12: __func::destroy_deallocate
static void t_6D1026(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1026, "ScenePresentBox::openPresentBoxDialogMain $_12 __func::destroy_deallocate", t_6D1026);

// ScenePresentBox::openPresentBoxDialogMain $_12: __func::target
static uint32_t t_6D1038(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7ed20u ? self + 4 : 0; }
RW_FUNCTION(0x6D1038, "ScenePresentBox::openPresentBoxDialogMain $_12 __func::target", t_6D1038);

// ScenePresentBox::openPresentBoxDialogMain $_12: __func::target_type
static uint32_t t_6D1058() { return 0xfa04e8u; }
RW_FUNCTION(0x6D1058, "ScenePresentBox::openPresentBoxDialogMain $_12 __func::target_type", t_6D1058);

// ScenePresentBox::openPresentBoxDialogMain $_11: __func::~__func (deleting)
static void t_6D106C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D106C, "ScenePresentBox::openPresentBoxDialogMain $_11 __func::~__func (deleting)", t_6D106C);

// ScenePresentBox::openPresentBoxDialogMain $_11: __func::__clone()
static uint32_t t_6D1074(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67910u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D1074, "ScenePresentBox::openPresentBoxDialogMain $_11 __func::__clone()", t_6D1074);

// ScenePresentBox::openPresentBoxDialogMain $_11: __func::__clone(__base*)
static void t_6D10A0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67910u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D10A0, "ScenePresentBox::openPresentBoxDialogMain $_11 __func::__clone(__base*)", t_6D10A0);

// ScenePresentBox::openPresentBoxDialogMain $_11: __func::destroy
static void t_6D10C0(uint32_t self) {
}
RW_FUNCTION(0x6D10C0, "ScenePresentBox::openPresentBoxDialogMain $_11 __func::destroy", t_6D10C0);

// ScenePresentBox::openPresentBoxDialogMain $_11: __func::destroy_deallocate
static void t_6D10C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D10C2, "ScenePresentBox::openPresentBoxDialogMain $_11 __func::destroy_deallocate", t_6D10C2);

// ScenePresentBox::openPresentBoxDialogMain $_11: __func::target
static uint32_t t_6D10FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7ee20u ? self + 4 : 0; }
RW_FUNCTION(0x6D10FC, "ScenePresentBox::openPresentBoxDialogMain $_11 __func::target", t_6D10FC);

// ScenePresentBox::openPresentBoxDialogMain $_11: __func::target_type
static uint32_t t_6D111C() { return 0xfa04fcu; }
RW_FUNCTION(0x6D111C, "ScenePresentBox::openPresentBoxDialogMain $_11 __func::target_type", t_6D111C);

// ScenePresentBox::openPresentBoxDialogMain $_10: __func::~__func (deleting)
static void t_6D1130(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1130, "ScenePresentBox::openPresentBoxDialogMain $_10 __func::~__func (deleting)", t_6D1130);

// ScenePresentBox::openPresentBoxDialogMain $_10: __func::__clone()
static uint32_t t_6D1138(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6793cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D1138, "ScenePresentBox::openPresentBoxDialogMain $_10 __func::__clone()", t_6D1138);

// ScenePresentBox::openPresentBoxDialogMain $_10: __func::__clone(__base*)
static void t_6D1164(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6793cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D1164, "ScenePresentBox::openPresentBoxDialogMain $_10 __func::__clone(__base*)", t_6D1164);

// ScenePresentBox::openPresentBoxDialogMain $_10: __func::destroy
static void t_6D1184(uint32_t self) {
}
RW_FUNCTION(0x6D1184, "ScenePresentBox::openPresentBoxDialogMain $_10 __func::destroy", t_6D1184);

// ScenePresentBox::openPresentBoxDialogMain $_10: __func::destroy_deallocate
static void t_6D1186(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1186, "ScenePresentBox::openPresentBoxDialogMain $_10 __func::destroy_deallocate", t_6D1186);

// ScenePresentBox::openPresentBoxDialogMain $_10: __func::target
static uint32_t t_6D11B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7eef0u ? self + 4 : 0; }
RW_FUNCTION(0x6D11B0, "ScenePresentBox::openPresentBoxDialogMain $_10 __func::target", t_6D11B0);

// ScenePresentBox::openPresentBoxDialogMain $_10: __func::target_type
static uint32_t t_6D11D0() { return 0xfa0510u; }
RW_FUNCTION(0x6D11D0, "ScenePresentBox::openPresentBoxDialogMain $_10 __func::target_type", t_6D11D0);

// ScenePresentBox::openPresentBoxDialogMain $_9: __func::~__func (deleting)
static void t_6D11E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D11E4, "ScenePresentBox::openPresentBoxDialogMain $_9 __func::~__func (deleting)", t_6D11E4);

// ScenePresentBox::openPresentBoxDialogMain $_9: __func::__clone()
static uint32_t t_6D11EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67968u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D11EC, "ScenePresentBox::openPresentBoxDialogMain $_9 __func::__clone()", t_6D11EC);

// ScenePresentBox::openPresentBoxDialogMain $_9: __func::__clone(__base*)
static void t_6D1218(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67968u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D1218, "ScenePresentBox::openPresentBoxDialogMain $_9 __func::__clone(__base*)", t_6D1218);

// ScenePresentBox::openPresentBoxDialogMain $_9: __func::destroy
static void t_6D1238(uint32_t self) {
}
RW_FUNCTION(0x6D1238, "ScenePresentBox::openPresentBoxDialogMain $_9 __func::destroy", t_6D1238);

// ScenePresentBox::openPresentBoxDialogMain $_9: __func::destroy_deallocate
static void t_6D123A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D123A, "ScenePresentBox::openPresentBoxDialogMain $_9 __func::destroy_deallocate", t_6D123A);

// ScenePresentBox::openPresentBoxDialogMain $_9: __func::target
static uint32_t t_6D124C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7efc0u ? self + 4 : 0; }
RW_FUNCTION(0x6D124C, "ScenePresentBox::openPresentBoxDialogMain $_9 __func::target", t_6D124C);

// ScenePresentBox::openPresentBoxDialogMain $_9: __func::target_type
static uint32_t t_6D126C() { return 0xfa0524u; }
RW_FUNCTION(0x6D126C, "ScenePresentBox::openPresentBoxDialogMain $_9 __func::target_type", t_6D126C);

// ScenePresentBox::openPresentBoxDialogMain $_7: __func::~__func (deleting)
static void t_6D1280(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1280, "ScenePresentBox::openPresentBoxDialogMain $_7 __func::~__func (deleting)", t_6D1280);

// ScenePresentBox::openPresentBoxDialogMain $_7: __func::__clone()
static uint32_t t_6D1288(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf67994u);
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
RW_FUNCTION(0x6D1288, "ScenePresentBox::openPresentBoxDialogMain $_7 __func::__clone()", t_6D1288);

// ScenePresentBox::openPresentBoxDialogMain $_7: __func::__clone(__base*)
static void t_6D12BC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67994u);
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
RW_FUNCTION(0x6D12BC, "ScenePresentBox::openPresentBoxDialogMain $_7 __func::__clone(__base*)", t_6D12BC);

// ScenePresentBox::openPresentBoxDialogMain $_7: __func::destroy
static void t_6D12E8(uint32_t self) {
}
RW_FUNCTION(0x6D12E8, "ScenePresentBox::openPresentBoxDialogMain $_7 __func::destroy", t_6D12E8);

// ScenePresentBox::openPresentBoxDialogMain $_7: __func::destroy_deallocate
static void t_6D12EA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D12EA, "ScenePresentBox::openPresentBoxDialogMain $_7 __func::destroy_deallocate", t_6D12EA);

// ScenePresentBox::openPresentBoxDialogMain $_7: __func::target
static uint32_t t_6D13B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f0c0u ? self + 4 : 0; }
RW_FUNCTION(0x6D13B4, "ScenePresentBox::openPresentBoxDialogMain $_7 __func::target", t_6D13B4);

// ScenePresentBox::openPresentBoxDialogMain $_7: __func::target_type
static uint32_t t_6D13D4() { return 0xfa0538u; }
RW_FUNCTION(0x6D13D4, "ScenePresentBox::openPresentBoxDialogMain $_7 __func::target_type", t_6D13D4);

// ScenePresentBox::openPresentBoxDialogMain $_6: __func::~__func (deleting)
static void t_6D13E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D13E8, "ScenePresentBox::openPresentBoxDialogMain $_6 __func::~__func (deleting)", t_6D13E8);

// ScenePresentBox::openPresentBoxDialogMain $_6: __func::__clone()
static uint32_t t_6D13F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf679c0u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D13F0, "ScenePresentBox::openPresentBoxDialogMain $_6 __func::__clone()", t_6D13F0);

// ScenePresentBox::openPresentBoxDialogMain $_6: __func::__clone(__base*)
static void t_6D1420(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf679c0u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D1420, "ScenePresentBox::openPresentBoxDialogMain $_6 __func::__clone(__base*)", t_6D1420);

// ScenePresentBox::openPresentBoxDialogMain $_6: __func::destroy
static void t_6D1444(uint32_t self) {
}
RW_FUNCTION(0x6D1444, "ScenePresentBox::openPresentBoxDialogMain $_6 __func::destroy", t_6D1444);

// ScenePresentBox::openPresentBoxDialogMain $_6: __func::destroy_deallocate
static void t_6D1446(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1446, "ScenePresentBox::openPresentBoxDialogMain $_6 __func::destroy_deallocate", t_6D1446);

// ScenePresentBox::openPresentBoxDialogMain $_6: __func::target
static uint32_t t_6D157C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f190u ? self + 4 : 0; }
RW_FUNCTION(0x6D157C, "ScenePresentBox::openPresentBoxDialogMain $_6 __func::target", t_6D157C);

// ScenePresentBox::openPresentBoxDialogMain $_6: __func::target_type
static uint32_t t_6D159C() { return 0xfa054cu; }
RW_FUNCTION(0x6D159C, "ScenePresentBox::openPresentBoxDialogMain $_6 __func::target_type", t_6D159C);

// ScenePresentBox::openPresentBoxDialogMain $_5: __func::~__func (deleting)
static void t_6D15B0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D15B0, "ScenePresentBox::openPresentBoxDialogMain $_5 __func::~__func (deleting)", t_6D15B0);

// ScenePresentBox::openPresentBoxDialogMain $_5: __func::__clone()
static uint32_t t_6D15B8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf679ecu);
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
RW_FUNCTION(0x6D15B8, "ScenePresentBox::openPresentBoxDialogMain $_5 __func::__clone()", t_6D15B8);

// ScenePresentBox::openPresentBoxDialogMain $_5: __func::__clone(__base*)
static void t_6D15EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf679ecu);
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
RW_FUNCTION(0x6D15EC, "ScenePresentBox::openPresentBoxDialogMain $_5 __func::__clone(__base*)", t_6D15EC);

// ScenePresentBox::openPresentBoxDialogMain $_5: __func::destroy
static void t_6D1618(uint32_t self) {
}
RW_FUNCTION(0x6D1618, "ScenePresentBox::openPresentBoxDialogMain $_5 __func::destroy", t_6D1618);

// ScenePresentBox::openPresentBoxDialogMain $_5: __func::destroy_deallocate
static void t_6D161A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D161A, "ScenePresentBox::openPresentBoxDialogMain $_5 __func::destroy_deallocate", t_6D161A);

// ScenePresentBox::openPresentBoxDialogMain $_5: __func::target
static uint32_t t_6D1710(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f260u ? self + 4 : 0; }
RW_FUNCTION(0x6D1710, "ScenePresentBox::openPresentBoxDialogMain $_5 __func::target", t_6D1710);

// ScenePresentBox::openPresentBoxDialogMain $_5: __func::target_type
static uint32_t t_6D1730() { return 0xfa0560u; }
RW_FUNCTION(0x6D1730, "ScenePresentBox::openPresentBoxDialogMain $_5 __func::target_type", t_6D1730);

// ScenePresentBox::openPresentBoxDialog $_3: __func::~__func
static uint32_t t_6D1744(uint32_t self) {
    st32(self, 0xf67a18u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6D1744, "ScenePresentBox::openPresentBoxDialog $_3 __func::~__func", t_6D1744);

// ScenePresentBox::openPresentBoxDialog $_3: __func::~__func (deleting)
static void t_6D1780(uint32_t self) {
    st32(self, 0xf67a18u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1780, "ScenePresentBox::openPresentBoxDialog $_3 __func::~__func (deleting)", t_6D1780);

// ScenePresentBox::openPresentBoxDialog $_3: __func::__clone()
static uint32_t t_6D17C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf67a18u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6D17C0, "ScenePresentBox::openPresentBoxDialog $_3 __func::__clone()", t_6D17C0);

// ScenePresentBox::openPresentBoxDialog $_3: __func::__clone(__base*)
static void t_6D1810(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67a18u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x6D1810, "ScenePresentBox::openPresentBoxDialog $_3 __func::__clone(__base*)", t_6D1810);

// ScenePresentBox::openPresentBoxDialog $_3: __func::destroy
static void t_6D1844(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6D1844, "ScenePresentBox::openPresentBoxDialog $_3 __func::destroy", t_6D1844);

// ScenePresentBox::openPresentBoxDialog $_3: __func::destroy_deallocate
static void t_6D1864(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1864, "ScenePresentBox::openPresentBoxDialog $_3 __func::destroy_deallocate", t_6D1864);

// ScenePresentBox::openPresentBoxDialog $_3: __func::target
static uint32_t t_6D1978(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f330u ? self + 8 : 0; }
RW_FUNCTION(0x6D1978, "ScenePresentBox::openPresentBoxDialog $_3 __func::target", t_6D1978);

// ScenePresentBox::openPresentBoxDialog $_3: __func::target_type
static uint32_t t_6D1998() { return 0xfa0574u; }
RW_FUNCTION(0x6D1998, "ScenePresentBox::openPresentBoxDialog $_3 __func::target_type", t_6D1998);

// ScenePresentBox::openPresentBoxDialog $_3 UlbE_: __func::~__func (deleting)
static void t_6D19AC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D19AC, "ScenePresentBox::openPresentBoxDialog $_3 UlbE_ __func::~__func (deleting)", t_6D19AC);

// ScenePresentBox::openPresentBoxDialog $_3 UlbE_: __func::__clone()
static uint32_t t_6D19B4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67a44u);
    }
    return v1;
}
RW_FUNCTION(0x6D19B4, "ScenePresentBox::openPresentBoxDialog $_3 UlbE_ __func::__clone()", t_6D19B4);

// ScenePresentBox::openPresentBoxDialog $_3 UlbE_: __func::__clone(__base*)
static void t_6D19D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67a44u);
    }
}
RW_FUNCTION(0x6D19D8, "ScenePresentBox::openPresentBoxDialog $_3 UlbE_ __func::__clone(__base*)", t_6D19D8);

// ScenePresentBox::openPresentBoxDialog $_3 UlbE_: __func::destroy
static void t_6D19F4(uint32_t self) {
}
RW_FUNCTION(0x6D19F4, "ScenePresentBox::openPresentBoxDialog $_3 UlbE_ __func::destroy", t_6D19F4);

// ScenePresentBox::openPresentBoxDialog $_3 UlbE_: __func::destroy_deallocate
static void t_6D19F6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D19F6, "ScenePresentBox::openPresentBoxDialog $_3 UlbE_ __func::destroy_deallocate", t_6D19F6);

// ScenePresentBox::openPresentBoxDialog $_3 UlbE_: __func::target
static uint32_t t_6D1A0C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f410u ? self + 4 : 0; }
RW_FUNCTION(0x6D1A0C, "ScenePresentBox::openPresentBoxDialog $_3 UlbE_ __func::target", t_6D1A0C);

// ScenePresentBox::openPresentBoxDialog $_3 UlbE_: __func::target_type
static uint32_t t_6D1A2C() { return 0xfa0588u; }
RW_FUNCTION(0x6D1A2C, "ScenePresentBox::openPresentBoxDialog $_3 UlbE_ __func::target_type", t_6D1A2C);

// ScenePresentBox::init $_2: __func::~__func (deleting)
static void t_6D1A40(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1A40, "ScenePresentBox::init $_2 __func::~__func (deleting)", t_6D1A40);

// ScenePresentBox::init $_2: __func::__clone()
static uint32_t t_6D1A48(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67a70u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D1A48, "ScenePresentBox::init $_2 __func::__clone()", t_6D1A48);

// ScenePresentBox::init $_2: __func::__clone(__base*)
static void t_6D1A74(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67a70u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D1A74, "ScenePresentBox::init $_2 __func::__clone(__base*)", t_6D1A74);

// ScenePresentBox::init $_2: __func::destroy
static void t_6D1A94(uint32_t self) {
}
RW_FUNCTION(0x6D1A94, "ScenePresentBox::init $_2 __func::destroy", t_6D1A94);

// ScenePresentBox::init $_2: __func::destroy_deallocate
static void t_6D1A96(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1A96, "ScenePresentBox::init $_2 __func::destroy_deallocate", t_6D1A96);

// ScenePresentBox::init $_2: __func::target
static uint32_t t_6D1CC8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f4d0u ? self + 4 : 0; }
RW_FUNCTION(0x6D1CC8, "ScenePresentBox::init $_2 __func::target", t_6D1CC8);

// ScenePresentBox::init $_2: __func::target_type
static uint32_t t_6D1CE8() { return 0xfa059cu; }
RW_FUNCTION(0x6D1CE8, "ScenePresentBox::init $_2 __func::target_type", t_6D1CE8);

// ScenePresentBox::init $_2 UlbE_: __func::~__func (deleting)
static void t_6D1CFC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1CFC, "ScenePresentBox::init $_2 UlbE_ __func::~__func (deleting)", t_6D1CFC);

// ScenePresentBox::init $_2 UlbE_: __func::__clone()
static uint32_t t_6D1D04(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67a9cu);
    }
    return v1;
}
RW_FUNCTION(0x6D1D04, "ScenePresentBox::init $_2 UlbE_ __func::__clone()", t_6D1D04);

// ScenePresentBox::init $_2 UlbE_: __func::__clone(__base*)
static void t_6D1D28(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67a9cu);
    }
}
RW_FUNCTION(0x6D1D28, "ScenePresentBox::init $_2 UlbE_ __func::__clone(__base*)", t_6D1D28);

// ScenePresentBox::init $_2 UlbE_: __func::destroy
static void t_6D1D44(uint32_t self) {
}
RW_FUNCTION(0x6D1D44, "ScenePresentBox::init $_2 UlbE_ __func::destroy", t_6D1D44);

// ScenePresentBox::init $_2 UlbE_: __func::destroy_deallocate
static void t_6D1D46(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1D46, "ScenePresentBox::init $_2 UlbE_ __func::destroy_deallocate", t_6D1D46);

// ScenePresentBox::init $_2 UlbE_: __func::target
static uint32_t t_6D1D5C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f560u ? self + 4 : 0; }
RW_FUNCTION(0x6D1D5C, "ScenePresentBox::init $_2 UlbE_ __func::target", t_6D1D5C);

// ScenePresentBox::init $_2 UlbE_: __func::target_type
static uint32_t t_6D1D7C() { return 0xfa05b0u; }
RW_FUNCTION(0x6D1D7C, "ScenePresentBox::init $_2 UlbE_ __func::target_type", t_6D1D7C);

// ScenePresentBox::init $_1: __func::~__func (deleting)
static void t_6D1D90(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1D90, "ScenePresentBox::init $_1 __func::~__func (deleting)", t_6D1D90);

// ScenePresentBox::init $_1: __func::__clone()
static uint32_t t_6D1D98(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67ac8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D1D98, "ScenePresentBox::init $_1 __func::__clone()", t_6D1D98);

// ScenePresentBox::init $_1: __func::__clone(__base*)
static void t_6D1DC4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67ac8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D1DC4, "ScenePresentBox::init $_1 __func::__clone(__base*)", t_6D1DC4);

// ScenePresentBox::init $_1: __func::destroy
static void t_6D1DE4(uint32_t self) {
}
RW_FUNCTION(0x6D1DE4, "ScenePresentBox::init $_1 __func::destroy", t_6D1DE4);

// ScenePresentBox::init $_1: __func::destroy_deallocate
static void t_6D1DE6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1DE6, "ScenePresentBox::init $_1 __func::destroy_deallocate", t_6D1DE6);

// ScenePresentBox::init $_1: __func::target
static uint32_t t_6D1DF8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f5f0u ? self + 4 : 0; }
RW_FUNCTION(0x6D1DF8, "ScenePresentBox::init $_1 __func::target", t_6D1DF8);

// ScenePresentBox::init $_1: __func::target_type
static uint32_t t_6D1E18() { return 0xfa05c4u; }
RW_FUNCTION(0x6D1E18, "ScenePresentBox::init $_1 __func::target_type", t_6D1E18);

// ScenePresentBox::init $_0: __func::~__func (deleting)
static void t_6D1E2C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1E2C, "ScenePresentBox::init $_0 __func::~__func (deleting)", t_6D1E2C);

// ScenePresentBox::init $_0: __func::__clone()
static uint32_t t_6D1E34(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67af4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D1E34, "ScenePresentBox::init $_0 __func::__clone()", t_6D1E34);

// ScenePresentBox::init $_0: __func::__clone(__base*)
static void t_6D1E60(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67af4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D1E60, "ScenePresentBox::init $_0 __func::__clone(__base*)", t_6D1E60);

// ScenePresentBox::init $_0: __func::destroy
static void t_6D1E80(uint32_t self) {
}
RW_FUNCTION(0x6D1E80, "ScenePresentBox::init $_0 __func::destroy", t_6D1E80);

// ScenePresentBox::init $_0: __func::destroy_deallocate
static void t_6D1E82(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D1E82, "ScenePresentBox::init $_0 __func::destroy_deallocate", t_6D1E82);

// ScenePresentBox::init $_0: __func::target
static uint32_t t_6D1E94(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f670u ? self + 4 : 0; }
RW_FUNCTION(0x6D1E94, "ScenePresentBox::init $_0 __func::target", t_6D1E94);

// ScenePresentBox::init $_0: __func::target_type
static uint32_t t_6D1EB4() { return 0xfa05d8u; }
RW_FUNCTION(0x6D1EB4, "ScenePresentBox::init $_0 __func::target_type", t_6D1EB4);
