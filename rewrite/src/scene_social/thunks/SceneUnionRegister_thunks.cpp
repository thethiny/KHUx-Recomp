// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneUnionRegister lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneUnionRegister::initUnionButtons $_26: __func::~__func (deleting)
static void t_71A13E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A13E, "SceneUnionRegister::initUnionButtons $_26 __func::~__func (deleting)", t_71A13E);

// SceneUnionRegister::initUnionButtons $_26: __func::__clone()
static uint32_t t_71A148(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6b774u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x71A148, "SceneUnionRegister::initUnionButtons $_26 __func::__clone()", t_71A148);

// SceneUnionRegister::initUnionButtons $_26: __func::__clone(__base*)
static void t_71A178(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b774u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71A178, "SceneUnionRegister::initUnionButtons $_26 __func::__clone(__base*)", t_71A178);

// SceneUnionRegister::initUnionButtons $_26: __func::destroy
static void t_71A1A0(uint32_t self) {
}
RW_FUNCTION(0x71A1A0, "SceneUnionRegister::initUnionButtons $_26 __func::destroy", t_71A1A0);

// SceneUnionRegister::initUnionButtons $_26: __func::destroy_deallocate
static void t_71A1A2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A1A2, "SceneUnionRegister::initUnionButtons $_26 __func::destroy_deallocate", t_71A1A2);

// SceneUnionRegister::initUnionButtons $_26: __func::target
static uint32_t t_71A2C0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe919b0u ? self + 4 : 0; }
RW_FUNCTION(0x71A2C0, "SceneUnionRegister::initUnionButtons $_26 __func::target", t_71A2C0);

// SceneUnionRegister::initUnionButtons $_26: __func::target_type
static uint32_t t_71A2E0() { return 0xfa5564u; }
RW_FUNCTION(0x71A2E0, "SceneUnionRegister::initUnionButtons $_26 __func::target_type", t_71A2E0);

// SceneUnionRegister::initUnionButtons $_26 UlvE_: __func::~__func (deleting)
static void t_71A2F4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A2F4, "SceneUnionRegister::initUnionButtons $_26 UlvE_ __func::~__func (deleting)", t_71A2F4);

// SceneUnionRegister::initUnionButtons $_26 UlvE_: __func::__clone()
static uint32_t t_71A2FC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6b7a0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71A2FC, "SceneUnionRegister::initUnionButtons $_26 UlvE_ __func::__clone()", t_71A2FC);

// SceneUnionRegister::initUnionButtons $_26 UlvE_: __func::__clone(__base*)
static void t_71A328(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b7a0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71A328, "SceneUnionRegister::initUnionButtons $_26 UlvE_ __func::__clone(__base*)", t_71A328);

// SceneUnionRegister::initUnionButtons $_26 UlvE_: __func::destroy
static void t_71A348(uint32_t self) {
}
RW_FUNCTION(0x71A348, "SceneUnionRegister::initUnionButtons $_26 UlvE_ __func::destroy", t_71A348);

// SceneUnionRegister::initUnionButtons $_26 UlvE_: __func::destroy_deallocate
static void t_71A34A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A34A, "SceneUnionRegister::initUnionButtons $_26 UlvE_ __func::destroy_deallocate", t_71A34A);

// SceneUnionRegister::initUnionButtons $_26 UlvE_: __func::target
static uint32_t t_71A3E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe91a80u ? self + 4 : 0; }
RW_FUNCTION(0x71A3E8, "SceneUnionRegister::initUnionButtons $_26 UlvE_ __func::target", t_71A3E8);

// SceneUnionRegister::initUnionButtons $_26 UlvE_: __func::target_type
static uint32_t t_71A408() { return 0xfa5578u; }
RW_FUNCTION(0x71A408, "SceneUnionRegister::initUnionButtons $_26 UlvE_ __func::target_type", t_71A408);

// SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_: __func::~__func (deleting)
static void t_71A41C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A41C, "SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_ __func::~__func (deleting)", t_71A41C);

// SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_: __func::__clone()
static uint32_t t_71A424(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6b7ccu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71A424, "SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_ __func::__clone()", t_71A424);

// SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_: __func::__clone(__base*)
static void t_71A450(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b7ccu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71A450, "SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_ __func::__clone(__base*)", t_71A450);

// SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_: __func::destroy
static void t_71A470(uint32_t self) {
}
RW_FUNCTION(0x71A470, "SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_ __func::destroy", t_71A470);

// SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_: __func::destroy_deallocate
static void t_71A472(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A472, "SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_ __func::destroy_deallocate", t_71A472);

// SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_: __func::target
static uint32_t t_71A484(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe91b80u ? self + 4 : 0; }
RW_FUNCTION(0x71A484, "SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_ __func::target", t_71A484);

// SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_: __func::target_type
static uint32_t t_71A4A4() { return 0xfa558cu; }
RW_FUNCTION(0x71A4A4, "SceneUnionRegister::initUnionButtons $_26 UlvE_ UlbE_ __func::target_type", t_71A4A4);

// SceneUnionRegister::initUnionButtons $_25: __func::~__func (deleting)
static void t_71A4B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A4B8, "SceneUnionRegister::initUnionButtons $_25 __func::~__func (deleting)", t_71A4B8);

// SceneUnionRegister::initUnionButtons $_25: __func::__clone()
static uint32_t t_71A4C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6b7f8u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x71A4C0, "SceneUnionRegister::initUnionButtons $_25 __func::__clone()", t_71A4C0);

// SceneUnionRegister::initUnionButtons $_25: __func::__clone(__base*)
static void t_71A4F0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b7f8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71A4F0, "SceneUnionRegister::initUnionButtons $_25 __func::__clone(__base*)", t_71A4F0);

// SceneUnionRegister::initUnionButtons $_25: __func::destroy
static void t_71A518(uint32_t self) {
}
RW_FUNCTION(0x71A518, "SceneUnionRegister::initUnionButtons $_25 __func::destroy", t_71A518);

// SceneUnionRegister::initUnionButtons $_25: __func::destroy_deallocate
static void t_71A51A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A51A, "SceneUnionRegister::initUnionButtons $_25 __func::destroy_deallocate", t_71A51A);

// SceneUnionRegister::initUnionButtons $_25: __func::target
static uint32_t t_71A604(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe91c80u ? self + 4 : 0; }
RW_FUNCTION(0x71A604, "SceneUnionRegister::initUnionButtons $_25 __func::target", t_71A604);

// SceneUnionRegister::initUnionButtons $_25: __func::target_type
static uint32_t t_71A624() { return 0xfa55a0u; }
RW_FUNCTION(0x71A624, "SceneUnionRegister::initUnionButtons $_25 __func::target_type", t_71A624);

// SceneUnionRegister::initUnionButtons $_24: __func::~__func (deleting)
static void t_71A638(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A638, "SceneUnionRegister::initUnionButtons $_24 __func::~__func (deleting)", t_71A638);

// SceneUnionRegister::initUnionButtons $_24: __func::__clone()
static uint32_t t_71A640(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6b824u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x71A640, "SceneUnionRegister::initUnionButtons $_24 __func::__clone()", t_71A640);

// SceneUnionRegister::initUnionButtons $_24: __func::__clone(__base*)
static void t_71A670(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b824u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71A670, "SceneUnionRegister::initUnionButtons $_24 __func::__clone(__base*)", t_71A670);

// SceneUnionRegister::initUnionButtons $_24: __func::destroy
static void t_71A698(uint32_t self) {
}
RW_FUNCTION(0x71A698, "SceneUnionRegister::initUnionButtons $_24 __func::destroy", t_71A698);

// SceneUnionRegister::initUnionButtons $_24: __func::destroy_deallocate
static void t_71A69A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A69A, "SceneUnionRegister::initUnionButtons $_24 __func::destroy_deallocate", t_71A69A);

// SceneUnionRegister::initUnionButtons $_24: __func::target
static uint32_t t_71A790(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe91d50u ? self + 4 : 0; }
RW_FUNCTION(0x71A790, "SceneUnionRegister::initUnionButtons $_24 __func::target", t_71A790);

// SceneUnionRegister::initUnionButtons $_24: __func::target_type
static uint32_t t_71A7B0() { return 0xfa55b4u; }
RW_FUNCTION(0x71A7B0, "SceneUnionRegister::initUnionButtons $_24 __func::target_type", t_71A7B0);

// SceneUnionRegister::openPartyJewelPop $_23: __func::~__func
static uint32_t t_71A7C4(uint32_t self) {
    st32(self, 0xf6b850u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x71A7C4, "SceneUnionRegister::openPartyJewelPop $_23 __func::~__func", t_71A7C4);

// SceneUnionRegister::openPartyJewelPop $_23: __func::~__func (deleting)
static void t_71A800(uint32_t self) {
    st32(self, 0xf6b850u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A800, "SceneUnionRegister::openPartyJewelPop $_23 __func::~__func (deleting)", t_71A800);

// SceneUnionRegister::openPartyJewelPop $_23: __func::__clone()
static uint32_t t_71A840(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf6b850u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x45c84d, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x71A840, "SceneUnionRegister::openPartyJewelPop $_23 __func::__clone()", t_71A840);

// SceneUnionRegister::openPartyJewelPop $_23: __func::__clone(__base*)
static void t_71A89C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b850u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x71A89C, "SceneUnionRegister::openPartyJewelPop $_23 __func::__clone(__base*)", t_71A89C);

// SceneUnionRegister::openPartyJewelPop $_23: __func::destroy
static void t_71A8DC(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x71A8DC, "SceneUnionRegister::openPartyJewelPop $_23 __func::destroy", t_71A8DC);

// SceneUnionRegister::openPartyJewelPop $_23: __func::destroy_deallocate
static void t_71A8FC(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71A8FC, "SceneUnionRegister::openPartyJewelPop $_23 __func::destroy_deallocate", t_71A8FC);

// SceneUnionRegister::openPartyJewelPop $_23: __func::target
static uint32_t t_71AA20(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe91e40u ? self + 8 : 0; }
RW_FUNCTION(0x71AA20, "SceneUnionRegister::openPartyJewelPop $_23 __func::target", t_71AA20);

// SceneUnionRegister::openPartyJewelPop $_23: __func::target_type
static uint32_t t_71AA44() { return 0xfa55c8u; }
RW_FUNCTION(0x71AA44, "SceneUnionRegister::openPartyJewelPop $_23 __func::target_type", t_71AA44);

// SceneUnionRegister::openPartyJewelPop $_22 UlvE_: __func::~__func (deleting)
static void t_71AA58(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71AA58, "SceneUnionRegister::openPartyJewelPop $_22 UlvE_ __func::~__func (deleting)", t_71AA58);

// SceneUnionRegister::openPartyJewelPop $_22 UlvE_: __func::__clone()
static uint32_t t_71AA60(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6b87cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x71AA60, "SceneUnionRegister::openPartyJewelPop $_22 UlvE_ __func::__clone()", t_71AA60);

// SceneUnionRegister::openPartyJewelPop $_22 UlvE_: __func::__clone(__base*)
static void t_71AA90(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b87cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71AA90, "SceneUnionRegister::openPartyJewelPop $_22 UlvE_ __func::__clone(__base*)", t_71AA90);

// SceneUnionRegister::openPartyJewelPop $_22 UlvE_: __func::destroy
static void t_71AAB8(uint32_t self) {
}
RW_FUNCTION(0x71AAB8, "SceneUnionRegister::openPartyJewelPop $_22 UlvE_ __func::destroy", t_71AAB8);

// SceneUnionRegister::openPartyJewelPop $_22 UlvE_: __func::destroy_deallocate
static void t_71AABA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71AABA, "SceneUnionRegister::openPartyJewelPop $_22 UlvE_ __func::destroy_deallocate", t_71AABA);

// SceneUnionRegister::openPartyJewelPop $_22 UlvE_: __func::target
static uint32_t t_71AAEC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe91f20u ? self + 4 : 0; }
RW_FUNCTION(0x71AAEC, "SceneUnionRegister::openPartyJewelPop $_22 UlvE_ __func::target", t_71AAEC);

// SceneUnionRegister::openPartyJewelPop $_22 UlvE_: __func::target_type
static uint32_t t_71AB0C() { return 0xfa55dcu; }
RW_FUNCTION(0x71AB0C, "SceneUnionRegister::openPartyJewelPop $_22 UlvE_ __func::target_type", t_71AB0C);

// SceneUnionRegister::openInvitationJewelPop $_21: __func::~__func
static uint32_t t_71AB20(uint32_t self) {
    st32(self, 0xf6b8a8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x71AB20, "SceneUnionRegister::openInvitationJewelPop $_21 __func::~__func", t_71AB20);

// SceneUnionRegister::openInvitationJewelPop $_21: __func::~__func (deleting)
static void t_71AB5C(uint32_t self) {
    st32(self, 0xf6b8a8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71AB5C, "SceneUnionRegister::openInvitationJewelPop $_21 __func::~__func (deleting)", t_71AB5C);

// SceneUnionRegister::openInvitationJewelPop $_21: __func::__clone()
static uint32_t t_71AB9C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf6b8a8u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x45c84d, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x71AB9C, "SceneUnionRegister::openInvitationJewelPop $_21 __func::__clone()", t_71AB9C);

// SceneUnionRegister::openInvitationJewelPop $_21: __func::__clone(__base*)
static void t_71ABF8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b8a8u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x71ABF8, "SceneUnionRegister::openInvitationJewelPop $_21 __func::__clone(__base*)", t_71ABF8);

// SceneUnionRegister::openInvitationJewelPop $_21: __func::destroy
static void t_71AC38(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x71AC38, "SceneUnionRegister::openInvitationJewelPop $_21 __func::destroy", t_71AC38);

// SceneUnionRegister::openInvitationJewelPop $_21: __func::destroy_deallocate
static void t_71AC58(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71AC58, "SceneUnionRegister::openInvitationJewelPop $_21 __func::destroy_deallocate", t_71AC58);

// SceneUnionRegister::openInvitationJewelPop $_21: __func::target
static uint32_t t_71AD7C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92030u ? self + 8 : 0; }
RW_FUNCTION(0x71AD7C, "SceneUnionRegister::openInvitationJewelPop $_21 __func::target", t_71AD7C);

// SceneUnionRegister::openInvitationJewelPop $_21: __func::target_type
static uint32_t t_71ADA0() { return 0xfa55f0u; }
RW_FUNCTION(0x71ADA0, "SceneUnionRegister::openInvitationJewelPop $_21 __func::target_type", t_71ADA0);

// SceneUnionRegister::openInvitationJewelPop $_20 UlvE_: __func::~__func (deleting)
static void t_71ADB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71ADB4, "SceneUnionRegister::openInvitationJewelPop $_20 UlvE_ __func::~__func (deleting)", t_71ADB4);

// SceneUnionRegister::openInvitationJewelPop $_20 UlvE_: __func::__clone()
static uint32_t t_71ADBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6b8d4u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x71ADBC, "SceneUnionRegister::openInvitationJewelPop $_20 UlvE_ __func::__clone()", t_71ADBC);

// SceneUnionRegister::openInvitationJewelPop $_20 UlvE_: __func::__clone(__base*)
static void t_71ADEC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b8d4u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71ADEC, "SceneUnionRegister::openInvitationJewelPop $_20 UlvE_ __func::__clone(__base*)", t_71ADEC);

// SceneUnionRegister::openInvitationJewelPop $_20 UlvE_: __func::destroy
static void t_71AE14(uint32_t self) {
}
RW_FUNCTION(0x71AE14, "SceneUnionRegister::openInvitationJewelPop $_20 UlvE_ __func::destroy", t_71AE14);

// SceneUnionRegister::openInvitationJewelPop $_20 UlvE_: __func::destroy_deallocate
static void t_71AE16(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71AE16, "SceneUnionRegister::openInvitationJewelPop $_20 UlvE_ __func::destroy_deallocate", t_71AE16);

// SceneUnionRegister::openInvitationJewelPop $_20 UlvE_: __func::target
static uint32_t t_71AE48(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92110u ? self + 4 : 0; }
RW_FUNCTION(0x71AE48, "SceneUnionRegister::openInvitationJewelPop $_20 UlvE_ __func::target", t_71AE48);

// SceneUnionRegister::openInvitationJewelPop $_20 UlvE_: __func::target_type
static uint32_t t_71AE68() { return 0xfa5604u; }
RW_FUNCTION(0x71AE68, "SceneUnionRegister::openInvitationJewelPop $_20 UlvE_ __func::target_type", t_71AE68);

// SceneUnionRegister::openInvitationNotJoinPop $_19: __func::~__func
static uint32_t t_71AE7C(uint32_t self) {
    st32(self, 0xf6b900u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x71AE7C, "SceneUnionRegister::openInvitationNotJoinPop $_19 __func::~__func", t_71AE7C);

// SceneUnionRegister::openInvitationNotJoinPop $_19: __func::~__func (deleting)
static void t_71AEB8(uint32_t self) {
    st32(self, 0xf6b900u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71AEB8, "SceneUnionRegister::openInvitationNotJoinPop $_19 __func::~__func (deleting)", t_71AEB8);

// SceneUnionRegister::openInvitationNotJoinPop $_19: __func::__clone()
static uint32_t t_71AEF8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf6b900u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x45c84d, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x71AEF8, "SceneUnionRegister::openInvitationNotJoinPop $_19 __func::__clone()", t_71AEF8);

// SceneUnionRegister::openInvitationNotJoinPop $_19: __func::__clone(__base*)
static void t_71AF54(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b900u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x71AF54, "SceneUnionRegister::openInvitationNotJoinPop $_19 __func::__clone(__base*)", t_71AF54);

// SceneUnionRegister::openInvitationNotJoinPop $_19: __func::destroy
static void t_71AF94(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x71AF94, "SceneUnionRegister::openInvitationNotJoinPop $_19 __func::destroy", t_71AF94);

// SceneUnionRegister::openInvitationNotJoinPop $_19: __func::destroy_deallocate
static void t_71AFB4(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71AFB4, "SceneUnionRegister::openInvitationNotJoinPop $_19 __func::destroy_deallocate", t_71AFB4);

// SceneUnionRegister::openInvitationNotJoinPop $_19: __func::target
static uint32_t t_71B0D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92220u ? self + 8 : 0; }
RW_FUNCTION(0x71B0D8, "SceneUnionRegister::openInvitationNotJoinPop $_19 __func::target", t_71B0D8);

// SceneUnionRegister::openInvitationNotJoinPop $_19: __func::target_type
static uint32_t t_71B0FC() { return 0xfa5618u; }
RW_FUNCTION(0x71B0FC, "SceneUnionRegister::openInvitationNotJoinPop $_19 __func::target_type", t_71B0FC);

// SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_: __func::~__func (deleting)
static void t_71B110(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B110, "SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_ __func::~__func (deleting)", t_71B110);

// SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_: __func::__clone()
static uint32_t t_71B118(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6b92cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x71B118, "SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_ __func::__clone()", t_71B118);

// SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_: __func::__clone(__base*)
static void t_71B148(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b92cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71B148, "SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_ __func::__clone(__base*)", t_71B148);

// SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_: __func::destroy
static void t_71B170(uint32_t self) {
}
RW_FUNCTION(0x71B170, "SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_ __func::destroy", t_71B170);

// SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_: __func::destroy_deallocate
static void t_71B172(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B172, "SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_ __func::destroy_deallocate", t_71B172);

// SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_: __func::target
static uint32_t t_71B1A4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92300u ? self + 4 : 0; }
RW_FUNCTION(0x71B1A4, "SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_ __func::target", t_71B1A4);

// SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_: __func::target_type
static uint32_t t_71B1C4() { return 0xfa562cu; }
RW_FUNCTION(0x71B1C4, "SceneUnionRegister::openInvitationNotJoinPop $_18 UlvE_ __func::target_type", t_71B1C4);

// SceneUnionRegister::openInvitationResultPop $_17: __func::~__func
static uint32_t t_71B1D8(uint32_t self) {
    st32(self, 0xf6b958u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x71B1D8, "SceneUnionRegister::openInvitationResultPop $_17 __func::~__func", t_71B1D8);

// SceneUnionRegister::openInvitationResultPop $_17: __func::~__func (deleting)
static void t_71B214(uint32_t self) {
    st32(self, 0xf6b958u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B214, "SceneUnionRegister::openInvitationResultPop $_17 __func::~__func (deleting)", t_71B214);

// SceneUnionRegister::openInvitationResultPop $_17: __func::__clone()
static uint32_t t_71B254(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf6b958u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x45c84d, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x71B254, "SceneUnionRegister::openInvitationResultPop $_17 __func::__clone()", t_71B254);

// SceneUnionRegister::openInvitationResultPop $_17: __func::__clone(__base*)
static void t_71B2B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b958u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x71B2B0, "SceneUnionRegister::openInvitationResultPop $_17 __func::__clone(__base*)", t_71B2B0);

// SceneUnionRegister::openInvitationResultPop $_17: __func::destroy
static void t_71B2F0(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x71B2F0, "SceneUnionRegister::openInvitationResultPop $_17 __func::destroy", t_71B2F0);

// SceneUnionRegister::openInvitationResultPop $_17: __func::destroy_deallocate
static void t_71B310(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B310, "SceneUnionRegister::openInvitationResultPop $_17 __func::destroy_deallocate", t_71B310);

// SceneUnionRegister::openInvitationResultPop $_17: __func::target
static uint32_t t_71B434(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92410u ? self + 8 : 0; }
RW_FUNCTION(0x71B434, "SceneUnionRegister::openInvitationResultPop $_17 __func::target", t_71B434);

// SceneUnionRegister::openInvitationResultPop $_17: __func::target_type
static uint32_t t_71B458() { return 0xfa5640u; }
RW_FUNCTION(0x71B458, "SceneUnionRegister::openInvitationResultPop $_17 __func::target_type", t_71B458);

// SceneUnionRegister::openInvitationResultPop $_16 UlvE_: __func::~__func (deleting)
static void t_71B46C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B46C, "SceneUnionRegister::openInvitationResultPop $_16 UlvE_ __func::~__func (deleting)", t_71B46C);

// SceneUnionRegister::openInvitationResultPop $_16 UlvE_: __func::__clone()
static uint32_t t_71B474(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6b984u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x71B474, "SceneUnionRegister::openInvitationResultPop $_16 UlvE_ __func::__clone()", t_71B474);

// SceneUnionRegister::openInvitationResultPop $_16 UlvE_: __func::__clone(__base*)
static void t_71B4A4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b984u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71B4A4, "SceneUnionRegister::openInvitationResultPop $_16 UlvE_ __func::__clone(__base*)", t_71B4A4);

// SceneUnionRegister::openInvitationResultPop $_16 UlvE_: __func::destroy
static void t_71B4CC(uint32_t self) {
}
RW_FUNCTION(0x71B4CC, "SceneUnionRegister::openInvitationResultPop $_16 UlvE_ __func::destroy", t_71B4CC);

// SceneUnionRegister::openInvitationResultPop $_16 UlvE_: __func::destroy_deallocate
static void t_71B4CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B4CE, "SceneUnionRegister::openInvitationResultPop $_16 UlvE_ __func::destroy_deallocate", t_71B4CE);

// SceneUnionRegister::openInvitationResultPop $_16 UlvE_: __func::target
static uint32_t t_71B500(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe924f0u ? self + 4 : 0; }
RW_FUNCTION(0x71B500, "SceneUnionRegister::openInvitationResultPop $_16 UlvE_ __func::target", t_71B500);

// SceneUnionRegister::openInvitationResultPop $_16 UlvE_: __func::target_type
static uint32_t t_71B520() { return 0xfa5654u; }
RW_FUNCTION(0x71B520, "SceneUnionRegister::openInvitationResultPop $_16 UlvE_ __func::target_type", t_71B520);

// SceneUnionRegister::nextState $_13: __func::~__func (deleting)
static void t_71B534(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B534, "SceneUnionRegister::nextState $_13 __func::~__func (deleting)", t_71B534);

// SceneUnionRegister::nextState $_13: __func::__clone()
static uint32_t t_71B53C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf6b9b0u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71B53C, "SceneUnionRegister::nextState $_13 __func::__clone()", t_71B53C);

// SceneUnionRegister::nextState $_13: __func::__clone(__base*)
static void t_71B56C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b9b0u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71B56C, "SceneUnionRegister::nextState $_13 __func::__clone(__base*)", t_71B56C);

// SceneUnionRegister::nextState $_13: __func::destroy
static void t_71B590(uint32_t self) {
}
RW_FUNCTION(0x71B590, "SceneUnionRegister::nextState $_13 __func::destroy", t_71B590);

// SceneUnionRegister::nextState $_13: __func::destroy_deallocate
static void t_71B592(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B592, "SceneUnionRegister::nextState $_13 __func::destroy_deallocate", t_71B592);

// SceneUnionRegister::nextState $_13: __func::target
static uint32_t t_71B670(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe925b0u ? self + 4 : 0; }
RW_FUNCTION(0x71B670, "SceneUnionRegister::nextState $_13 __func::target", t_71B670);

// SceneUnionRegister::nextState $_13: __func::target_type
static uint32_t t_71B690() { return 0xfa5668u; }
RW_FUNCTION(0x71B690, "SceneUnionRegister::nextState $_13 __func::target_type", t_71B690);

// SceneUnionRegister::nextState $_14: __func::~__func (deleting)
static void t_71B6A4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B6A4, "SceneUnionRegister::nextState $_14 __func::~__func (deleting)", t_71B6A4);

// SceneUnionRegister::nextState $_14: __func::__clone()
static uint32_t t_71B6AC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6b9dcu);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71B6AC, "SceneUnionRegister::nextState $_14 __func::__clone()", t_71B6AC);

// SceneUnionRegister::nextState $_14: __func::__clone(__base*)
static void t_71B6D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6b9dcu);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x71B6D8, "SceneUnionRegister::nextState $_14 __func::__clone(__base*)", t_71B6D8);

// SceneUnionRegister::nextState $_14: __func::destroy
static void t_71B6F8(uint32_t self) {
}
RW_FUNCTION(0x71B6F8, "SceneUnionRegister::nextState $_14 __func::destroy", t_71B6F8);

// SceneUnionRegister::nextState $_14: __func::destroy_deallocate
static void t_71B6FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B6FA, "SceneUnionRegister::nextState $_14 __func::destroy_deallocate", t_71B6FA);

// SceneUnionRegister::nextState $_14: __func::target
static uint32_t t_71B7EC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe92640u ? self + 4 : 0; }
RW_FUNCTION(0x71B7EC, "SceneUnionRegister::nextState $_14 __func::target", t_71B7EC);

// SceneUnionRegister::nextState $_14: __func::target_type
static uint32_t t_71B80C() { return 0xfa567cu; }
RW_FUNCTION(0x71B80C, "SceneUnionRegister::nextState $_14 __func::target_type", t_71B80C);

// SceneUnionRegister::nextState $_14 UlbE_: __func::~__func (deleting)
static void t_71B820(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B820, "SceneUnionRegister::nextState $_14 UlbE_ __func::~__func (deleting)", t_71B820);

// SceneUnionRegister::nextState $_14 UlbE_: __func::__clone()
static uint32_t t_71B828(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ba08u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71B828, "SceneUnionRegister::nextState $_14 UlbE_ __func::__clone()", t_71B828);

// SceneUnionRegister::nextState $_14 UlbE_: __func::__clone(__base*)
static void t_71B854(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ba08u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x71B854, "SceneUnionRegister::nextState $_14 UlbE_ __func::__clone(__base*)", t_71B854);

// SceneUnionRegister::nextState $_14 UlbE_: __func::destroy
static void t_71B874(uint32_t self) {
}
RW_FUNCTION(0x71B874, "SceneUnionRegister::nextState $_14 UlbE_ __func::destroy", t_71B874);

// SceneUnionRegister::nextState $_14 UlbE_: __func::destroy_deallocate
static void t_71B876(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B876, "SceneUnionRegister::nextState $_14 UlbE_ __func::destroy_deallocate", t_71B876);

// SceneUnionRegister::nextState $_14 UlbE_: __func::target
static uint32_t t_71B88C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe926e0u ? self + 4 : 0; }
RW_FUNCTION(0x71B88C, "SceneUnionRegister::nextState $_14 UlbE_ __func::target", t_71B88C);

// SceneUnionRegister::nextState $_14 UlbE_: __func::target_type
static uint32_t t_71B8AC() { return 0xfa5690u; }
RW_FUNCTION(0x71B8AC, "SceneUnionRegister::nextState $_14 UlbE_ __func::target_type", t_71B8AC);

// SceneUnionRegister::nextState $_12: __func::~__func (deleting)
static void t_71B8C0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B8C0, "SceneUnionRegister::nextState $_12 __func::~__func (deleting)", t_71B8C0);

// SceneUnionRegister::nextState $_12: __func::__clone()
static uint32_t t_71B8C8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6ba34u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x71B8C8, "SceneUnionRegister::nextState $_12 __func::__clone()", t_71B8C8);

// SceneUnionRegister::nextState $_12: __func::__clone(__base*)
static void t_71B8F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6ba34u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x71B8F4, "SceneUnionRegister::nextState $_12 __func::__clone(__base*)", t_71B8F4);

// SceneUnionRegister::nextState $_12: __func::destroy
static void t_71B914(uint32_t self) {
}
RW_FUNCTION(0x71B914, "SceneUnionRegister::nextState $_12 __func::destroy", t_71B914);

// SceneUnionRegister::nextState $_12: __func::destroy_deallocate
static void t_71B916(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x71B916, "SceneUnionRegister::nextState $_12 __func::destroy_deallocate", t_71B916);
