// The std::function __func thunks of the StageUI lambdas (generated from the lifted code by a scratch tool and
// checked against it): clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "Common.hpp"

using namespace w36;

// StageUI::openBattleMenuPopup $_8: __func::~__func
static uint32_t t_761974(uint32_t self) {
    st32(self, 0xf6e300u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x761974, "StageUI::openBattleMenuPopup $_8 __func::~__func", t_761974);

// StageUI::openBattleMenuPopup $_8: __func::~__func (deleting)
static void t_7619B0(uint32_t self) {
    st32(self, 0xf6e300u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7619B0, "StageUI::openBattleMenuPopup $_8 __func::~__func (deleting)", t_7619B0);

// StageUI::openBattleMenuPopup $_8: __func::__clone(__base*)
static void t_761A60(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e300u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v4 = ld32(self + 8 + 0);
        uint32_t v5 = ld32(self + 8 + 4);
        uint32_t v6 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v4);
        st32(p + 8 + 4, v5);
        st32(p + 8 + 8, v6);
    }
}
RW_FUNCTION(0x761A60, "StageUI::openBattleMenuPopup $_8 __func::__clone(__base*)", t_761A60);

// StageUI::openBattleMenuPopup $_8: __func::destroy
static void t_761AB0(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x761AB0, "StageUI::openBattleMenuPopup $_8 __func::destroy", t_761AB0);

// StageUI::openBattleMenuPopup $_8: __func::destroy_deallocate
static void t_761AD0(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761AD0, "StageUI::openBattleMenuPopup $_8 __func::destroy_deallocate", t_761AD0);

// StageUI::openBattleMenuPopup $_8: __func::target
static uint32_t t_761BD8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9f390u ? self + 8 : 0; }
RW_FUNCTION(0x761BD8, "StageUI::openBattleMenuPopup $_8 __func::target", t_761BD8);

// StageUI::openBattleMenuPopup $_8: __func::target_type
static uint32_t t_761BFC() { return 0xfa8be4u; }
RW_FUNCTION(0x761BFC, "StageUI::openBattleMenuPopup $_8 __func::target_type", t_761BFC);

// StageUI::openBattleMenuPopup $_9 UlvE_: __func::~__func (deleting)
static void t_761C10(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761C10, "StageUI::openBattleMenuPopup $_9 UlvE_ __func::~__func (deleting)", t_761C10);

// StageUI::openBattleMenuPopup $_9 UlvE_: __func::__clone()
static uint32_t t_761C18(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6e32cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x761C18, "StageUI::openBattleMenuPopup $_9 UlvE_ __func::__clone()", t_761C18);

// StageUI::openBattleMenuPopup $_9 UlvE_: __func::__clone(__base*)
static void t_761C48(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e32cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x761C48, "StageUI::openBattleMenuPopup $_9 UlvE_ __func::__clone(__base*)", t_761C48);

// StageUI::openBattleMenuPopup $_9 UlvE_: __func::destroy
static void t_761C70(uint32_t self) {
}
RW_FUNCTION(0x761C70, "StageUI::openBattleMenuPopup $_9 UlvE_ __func::destroy", t_761C70);

// StageUI::openBattleMenuPopup $_9 UlvE_: __func::destroy_deallocate
static void t_761C72(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761C72, "StageUI::openBattleMenuPopup $_9 UlvE_ __func::destroy_deallocate", t_761C72);

// StageUI::openBattleMenuPopup $_9 UlvE_: __func::target
static uint32_t t_761CA4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9f490u ? self + 4 : 0; }
RW_FUNCTION(0x761CA4, "StageUI::openBattleMenuPopup $_9 UlvE_ __func::target", t_761CA4);

// StageUI::openBattleMenuPopup $_9 UlvE_: __func::target_type
static uint32_t t_761CC4() { return 0xfa8bf8u; }
RW_FUNCTION(0x761CC4, "StageUI::openBattleMenuPopup $_9 UlvE_ __func::target_type", t_761CC4);

// StageUI::openBattleMenuPopup $_7: __func::~__func (deleting)
static void t_761CD8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761CD8, "StageUI::openBattleMenuPopup $_7 __func::~__func (deleting)", t_761CD8);

// StageUI::openBattleMenuPopup $_7: __func::__clone()
static uint32_t t_761CE0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e358u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x761CE0, "StageUI::openBattleMenuPopup $_7 __func::__clone()", t_761CE0);

// StageUI::openBattleMenuPopup $_7: __func::__clone(__base*)
static void t_761D0C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e358u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x761D0C, "StageUI::openBattleMenuPopup $_7 __func::__clone(__base*)", t_761D0C);

// StageUI::openBattleMenuPopup $_7: __func::destroy
static void t_761D2C(uint32_t self) {
}
RW_FUNCTION(0x761D2C, "StageUI::openBattleMenuPopup $_7 __func::destroy", t_761D2C);

// StageUI::openBattleMenuPopup $_7: __func::destroy_deallocate
static void t_761D2E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761D2E, "StageUI::openBattleMenuPopup $_7 __func::destroy_deallocate", t_761D2E);

// StageUI::openBattleMenuPopup $_7: __func::target
static uint32_t t_761D3C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9f590u ? self + 4 : 0; }
RW_FUNCTION(0x761D3C, "StageUI::openBattleMenuPopup $_7 __func::target", t_761D3C);

// StageUI::openBattleMenuPopup $_7: __func::target_type
static uint32_t t_761D5C() { return 0xfa8c0cu; }
RW_FUNCTION(0x761D5C, "StageUI::openBattleMenuPopup $_7 __func::target_type", t_761D5C);

// StageUI::openBattleMenuPopup $_6: __func::~__func (deleting)
static void t_761D70(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761D70, "StageUI::openBattleMenuPopup $_6 __func::~__func (deleting)", t_761D70);

// StageUI::openBattleMenuPopup $_6: __func::__clone()
static uint32_t t_761D78(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e384u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x761D78, "StageUI::openBattleMenuPopup $_6 __func::__clone()", t_761D78);

// StageUI::openBattleMenuPopup $_6: __func::__clone(__base*)
static void t_761DA4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e384u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x761DA4, "StageUI::openBattleMenuPopup $_6 __func::__clone(__base*)", t_761DA4);

// StageUI::openBattleMenuPopup $_6: __func::destroy
static void t_761DC4(uint32_t self) {
}
RW_FUNCTION(0x761DC4, "StageUI::openBattleMenuPopup $_6 __func::destroy", t_761DC4);

// StageUI::openBattleMenuPopup $_6: __func::destroy_deallocate
static void t_761DC6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761DC6, "StageUI::openBattleMenuPopup $_6 __func::destroy_deallocate", t_761DC6);

// StageUI::openBattleMenuPopup $_6: __func::target
static uint32_t t_761E78(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9f6b0u ? self + 4 : 0; }
RW_FUNCTION(0x761E78, "StageUI::openBattleMenuPopup $_6 __func::target", t_761E78);

// StageUI::openBattleMenuPopup $_6: __func::target_type
static uint32_t t_761E98() { return 0xfa8c20u; }
RW_FUNCTION(0x761E98, "StageUI::openBattleMenuPopup $_6 __func::target_type", t_761E98);

// StageUI::openBattleMenuPopup $_6 UlbE_: __func::~__func (deleting)
static void t_761EAC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761EAC, "StageUI::openBattleMenuPopup $_6 UlbE_ __func::~__func (deleting)", t_761EAC);

// StageUI::openBattleMenuPopup $_6 UlbE_: __func::__clone()
static uint32_t t_761EB4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e3b0u);
    }
    return v1;
}
RW_FUNCTION(0x761EB4, "StageUI::openBattleMenuPopup $_6 UlbE_ __func::__clone()", t_761EB4);

// StageUI::openBattleMenuPopup $_6 UlbE_: __func::__clone(__base*)
static void t_761ED8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e3b0u);
    }
}
RW_FUNCTION(0x761ED8, "StageUI::openBattleMenuPopup $_6 UlbE_ __func::__clone(__base*)", t_761ED8);

// StageUI::openBattleMenuPopup $_6 UlbE_: __func::destroy
static void t_761EF4(uint32_t self) {
}
RW_FUNCTION(0x761EF4, "StageUI::openBattleMenuPopup $_6 UlbE_ __func::destroy", t_761EF4);

// StageUI::openBattleMenuPopup $_6 UlbE_: __func::destroy_deallocate
static void t_761EF6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761EF6, "StageUI::openBattleMenuPopup $_6 UlbE_ __func::destroy_deallocate", t_761EF6);

// StageUI::openBattleMenuPopup $_6 UlbE_: __func::target
static uint32_t t_761F14(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9f7e0u ? self + 4 : 0; }
RW_FUNCTION(0x761F14, "StageUI::openBattleMenuPopup $_6 UlbE_ __func::target", t_761F14);

// StageUI::openBattleMenuPopup $_6 UlbE_: __func::target_type
static uint32_t t_761F34() { return 0xfa8c34u; }
RW_FUNCTION(0x761F34, "StageUI::openBattleMenuPopup $_6 UlbE_ __func::target_type", t_761F34);

// StageUI::openBattleMenuPopup $_5: __func::~__func (deleting)
static void t_761F48(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761F48, "StageUI::openBattleMenuPopup $_5 __func::~__func (deleting)", t_761F48);

// StageUI::openBattleMenuPopup $_5: __func::__clone()
static uint32_t t_761F50(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e3dcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x761F50, "StageUI::openBattleMenuPopup $_5 __func::__clone()", t_761F50);

// StageUI::openBattleMenuPopup $_5: __func::__clone(__base*)
static void t_761F7C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e3dcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x761F7C, "StageUI::openBattleMenuPopup $_5 __func::__clone(__base*)", t_761F7C);

// StageUI::openBattleMenuPopup $_5: __func::destroy
static void t_761F9C(uint32_t self) {
}
RW_FUNCTION(0x761F9C, "StageUI::openBattleMenuPopup $_5 __func::destroy", t_761F9C);

// StageUI::openBattleMenuPopup $_5: __func::destroy_deallocate
static void t_761F9E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761F9E, "StageUI::openBattleMenuPopup $_5 __func::destroy_deallocate", t_761F9E);

// StageUI::openBattleMenuPopup $_5: __func::target
static uint32_t t_761FC4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9f940u ? self + 4 : 0; }
RW_FUNCTION(0x761FC4, "StageUI::openBattleMenuPopup $_5 __func::target", t_761FC4);

// StageUI::openBattleMenuPopup $_5: __func::target_type
static uint32_t t_761FE4() { return 0xfa8c48u; }
RW_FUNCTION(0x761FE4, "StageUI::openBattleMenuPopup $_5 __func::target_type", t_761FE4);

// StageUI::openBattleMenuPopup $_4: __func::~__func (deleting)
static void t_761FF8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x761FF8, "StageUI::openBattleMenuPopup $_4 __func::~__func (deleting)", t_761FF8);

// StageUI::openBattleMenuPopup $_4: __func::__clone()
static uint32_t t_762000(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e408u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x762000, "StageUI::openBattleMenuPopup $_4 __func::__clone()", t_762000);

// StageUI::openBattleMenuPopup $_4: __func::__clone(__base*)
static void t_76202C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e408u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x76202C, "StageUI::openBattleMenuPopup $_4 __func::__clone(__base*)", t_76202C);

// StageUI::openBattleMenuPopup $_4: __func::destroy
static void t_76204C(uint32_t self) {
}
RW_FUNCTION(0x76204C, "StageUI::openBattleMenuPopup $_4 __func::destroy", t_76204C);

// StageUI::openBattleMenuPopup $_4: __func::destroy_deallocate
static void t_76204E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76204E, "StageUI::openBattleMenuPopup $_4 __func::destroy_deallocate", t_76204E);

// StageUI::openBattleMenuPopup $_4: __func::target
static uint32_t t_762078(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9fa60u ? self + 4 : 0; }
RW_FUNCTION(0x762078, "StageUI::openBattleMenuPopup $_4 __func::target", t_762078);

// StageUI::openBattleMenuPopup $_4: __func::target_type
static uint32_t t_762098() { return 0xfa8c5cu; }
RW_FUNCTION(0x762098, "StageUI::openBattleMenuPopup $_4 __func::target_type", t_762098);

// StageUI::openContinueDialog $_3: __func::~__func
static uint32_t t_7620AC(uint32_t self) {
    st32(self, 0xf6e434u);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x7620AC, "StageUI::openContinueDialog $_3 __func::~__func", t_7620AC);

// StageUI::openContinueDialog $_3: __func::~__func (deleting)
static void t_762104(uint32_t self) {
    st32(self, 0xf6e434u);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x762104, "StageUI::openContinueDialog $_3 __func::~__func (deleting)", t_762104);

// StageUI::openContinueDialog $_3: __func::__clone(__base*)
static void t_7621F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e434u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v1 = ld32(self + 32);
        st32(p + 32, v1);
        call(0x4683cd, {p + 40, self + 40});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 32);
        st32(p + 32, v2);
    }
}
RW_FUNCTION(0x7621F4, "StageUI::openContinueDialog $_3 __func::__clone(__base*)", t_7621F4);

// StageUI::openContinueDialog $_3: __func::destroy
static void t_762270(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x762270, "StageUI::openContinueDialog $_3 __func::destroy", t_762270);

// StageUI::openContinueDialog $_3: __func::destroy_deallocate
static void t_7622B0(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7622B0, "StageUI::openContinueDialog $_3 __func::destroy_deallocate", t_7622B0);

// StageUI::openContinueDialog $_3: __func::target
static uint32_t t_7627C0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9fb30u ? self + 8 : 0; }
RW_FUNCTION(0x7627C0, "StageUI::openContinueDialog $_3 __func::target", t_7627C0);

// StageUI::openContinueDialog $_3: __func::target_type
static uint32_t t_7627E0() { return 0xfa8c70u; }
RW_FUNCTION(0x7627E0, "StageUI::openContinueDialog $_3 __func::target_type", t_7627E0);

// StageUI::openContinueDialog $_3 UlbE0_: __func::~__func
static uint32_t t_76284C(uint32_t self) {
    st32(self, 0xf6e460u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x76284C, "StageUI::openContinueDialog $_3 UlbE0_ __func::~__func", t_76284C);

// StageUI::openContinueDialog $_3 UlbE0_: __func::~__func (deleting)
static void t_762888(uint32_t self) {
    st32(self, 0xf6e460u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x762888, "StageUI::openContinueDialog $_3 UlbE0_ __func::~__func (deleting)", t_762888);

// StageUI::openContinueDialog $_3 UlbE0_: __func::__clone()
static uint32_t t_7628C8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf6e460u);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x7628C8, "StageUI::openContinueDialog $_3 UlbE0_ __func::__clone()", t_7628C8);

// StageUI::openContinueDialog $_3 UlbE0_: __func::__clone(__base*)
static void t_762920(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e460u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x762920, "StageUI::openContinueDialog $_3 UlbE0_ __func::__clone(__base*)", t_762920);

// StageUI::openContinueDialog $_3 UlbE0_: __func::destroy
static void t_762960(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x762960, "StageUI::openContinueDialog $_3 UlbE0_ __func::destroy", t_762960);

// StageUI::openContinueDialog $_3 UlbE0_: __func::destroy_deallocate
static void t_762980(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x762980, "StageUI::openContinueDialog $_3 UlbE0_ __func::destroy_deallocate", t_762980);

// StageUI::openContinueDialog $_3 UlbE0_: __func::target
static uint32_t t_7629C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9fbf0u ? self + 8 : 0; }
RW_FUNCTION(0x7629C8, "StageUI::openContinueDialog $_3 UlbE0_ __func::target", t_7629C8);

// StageUI::openContinueDialog $_3 UlbE0_: __func::target_type
static uint32_t t_7629EC() { return 0xfa8c84u; }
RW_FUNCTION(0x7629EC, "StageUI::openContinueDialog $_3 UlbE0_ __func::target_type", t_7629EC);

// StageUI::openContinueDialog $_3 UlbE_: __func::~__func
static uint32_t t_762A00(uint32_t self) {
    st32(self, 0xf6e48cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x762A00, "StageUI::openContinueDialog $_3 UlbE_ __func::~__func", t_762A00);

// StageUI::openContinueDialog $_3 UlbE_: __func::~__func (deleting)
static void t_762A3C(uint32_t self) {
    st32(self, 0xf6e48cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x762A3C, "StageUI::openContinueDialog $_3 UlbE_ __func::~__func (deleting)", t_762A3C);

// StageUI::openContinueDialog $_3 UlbE_: __func::__clone()
static uint32_t t_762A7C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf6e48cu);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x762A7C, "StageUI::openContinueDialog $_3 UlbE_ __func::__clone()", t_762A7C);

// StageUI::openContinueDialog $_3 UlbE_: __func::__clone(__base*)
static void t_762AD4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e48cu);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x762AD4, "StageUI::openContinueDialog $_3 UlbE_ __func::__clone(__base*)", t_762AD4);

// StageUI::openContinueDialog $_3 UlbE_: __func::destroy
static void t_762B14(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x762B14, "StageUI::openContinueDialog $_3 UlbE_ __func::destroy", t_762B14);

// StageUI::openContinueDialog $_3 UlbE_: __func::destroy_deallocate
static void t_762B34(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x762B34, "StageUI::openContinueDialog $_3 UlbE_ __func::destroy_deallocate", t_762B34);

// StageUI::openContinueDialog $_3 UlbE_: __func::target
static uint32_t t_762CB0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9fcc0u ? self + 8 : 0; }
RW_FUNCTION(0x762CB0, "StageUI::openContinueDialog $_3 UlbE_ __func::target", t_762CB0);

// StageUI::openContinueDialog $_3 UlbE_: __func::target_type
static uint32_t t_762CD4() { return 0xfa8c98u; }
RW_FUNCTION(0x762CD4, "StageUI::openContinueDialog $_3 UlbE_ __func::target_type", t_762CD4);

// StageUI::openContinueDialog $_3 UlbE_ UlvE_: __func::~__func
static uint32_t t_762CE8(uint32_t self) {
    st32(self, 0xf6e4b8u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x762CE8, "StageUI::openContinueDialog $_3 UlbE_ UlvE_ __func::~__func", t_762CE8);

// StageUI::openContinueDialog $_3 UlbE_ UlvE_: __func::~__func (deleting)
static void t_762D24(uint32_t self) {
    st32(self, 0xf6e4b8u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x762D24, "StageUI::openContinueDialog $_3 UlbE_ UlvE_ __func::~__func (deleting)", t_762D24);

// StageUI::openContinueDialog $_3 UlbE_ UlvE_: __func::__clone()
static uint32_t t_762D64(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf6e4b8u);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x762D64, "StageUI::openContinueDialog $_3 UlbE_ UlvE_ __func::__clone()", t_762D64);

// StageUI::openContinueDialog $_3 UlbE_ UlvE_: __func::__clone(__base*)
static void t_762DBC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e4b8u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x762DBC, "StageUI::openContinueDialog $_3 UlbE_ UlvE_ __func::__clone(__base*)", t_762DBC);

// StageUI::openContinueDialog $_3 UlbE_ UlvE_: __func::destroy
static void t_762DFC(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x762DFC, "StageUI::openContinueDialog $_3 UlbE_ UlvE_ __func::destroy", t_762DFC);

// StageUI::openContinueDialog $_3 UlbE_ UlvE_: __func::destroy_deallocate
static void t_762E1C(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x762E1C, "StageUI::openContinueDialog $_3 UlbE_ UlvE_ __func::destroy_deallocate", t_762E1C);

// StageUI::openContinueDialog $_3 UlbE_ UlvE_: __func::target
static uint32_t t_762E58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9fda0u ? self + 8 : 0; }
RW_FUNCTION(0x762E58, "StageUI::openContinueDialog $_3 UlbE_ UlvE_ __func::target", t_762E58);

// StageUI::openContinueDialog $_3 UlbE_ UlvE_: __func::target_type
static uint32_t t_762E7C() { return 0xfa8cacu; }
RW_FUNCTION(0x762E7C, "StageUI::openContinueDialog $_3 UlbE_ UlvE_ __func::target_type", t_762E7C);

// StageUI::openContinueDialog $_1: __func::~__func
static uint32_t t_762E90(uint32_t self) {
    st32(self, 0xf6e4e4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x762E90, "StageUI::openContinueDialog $_1 __func::~__func", t_762E90);

// StageUI::openContinueDialog $_1: __func::~__func (deleting)
static void t_762ECC(uint32_t self) {
    st32(self, 0xf6e4e4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x762ECC, "StageUI::openContinueDialog $_1 __func::~__func (deleting)", t_762ECC);

// StageUI::openContinueDialog $_1: __func::__clone()
static uint32_t t_762F0C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6e4e4u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x762F0C, "StageUI::openContinueDialog $_1 __func::__clone()", t_762F0C);

// StageUI::openContinueDialog $_1: __func::__clone(__base*)
static void t_762F5C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e4e4u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x762F5C, "StageUI::openContinueDialog $_1 __func::__clone(__base*)", t_762F5C);

// StageUI::openContinueDialog $_1: __func::destroy
static void t_762F90(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x762F90, "StageUI::openContinueDialog $_1 __func::destroy", t_762F90);

// StageUI::openContinueDialog $_1: __func::destroy_deallocate
static void t_762FB0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x762FB0, "StageUI::openContinueDialog $_1 __func::destroy_deallocate", t_762FB0);

// StageUI::openContinueDialog $_1: __func::target
static uint32_t t_7630A8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9fe70u ? self + 8 : 0; }
RW_FUNCTION(0x7630A8, "StageUI::openContinueDialog $_1 __func::target", t_7630A8);

// StageUI::openContinueDialog $_1: __func::target_type
static uint32_t t_7630C8() { return 0xfa8cc0u; }
RW_FUNCTION(0x7630C8, "StageUI::openContinueDialog $_1 __func::target_type", t_7630C8);

// StageUI::openContinueDialog $_1 UlbE_: __func::~__func (deleting)
static void t_7630DC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7630DC, "StageUI::openContinueDialog $_1 UlbE_ __func::~__func (deleting)", t_7630DC);

// StageUI::openContinueDialog $_1 UlbE_: __func::__clone()
static uint32_t t_7630E4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e510u);
    }
    return v1;
}
RW_FUNCTION(0x7630E4, "StageUI::openContinueDialog $_1 UlbE_ __func::__clone()", t_7630E4);

// StageUI::openContinueDialog $_1 UlbE_: __func::__clone(__base*)
static void t_763108(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e510u);
    }
}
RW_FUNCTION(0x763108, "StageUI::openContinueDialog $_1 UlbE_ __func::__clone(__base*)", t_763108);

// StageUI::openContinueDialog $_1 UlbE_: __func::destroy
static void t_763124(uint32_t self) {
}
RW_FUNCTION(0x763124, "StageUI::openContinueDialog $_1 UlbE_ __func::destroy", t_763124);

// StageUI::openContinueDialog $_1 UlbE_: __func::destroy_deallocate
static void t_763126(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x763126, "StageUI::openContinueDialog $_1 UlbE_ __func::destroy_deallocate", t_763126);

// StageUI::openContinueDialog $_1 UlbE_: __func::target
static uint32_t t_76313C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9ff30u ? self + 4 : 0; }
RW_FUNCTION(0x76313C, "StageUI::openContinueDialog $_1 UlbE_ __func::target", t_76313C);

// StageUI::openContinueDialog $_1 UlbE_: __func::target_type
static uint32_t t_76315C() { return 0xfa8cd4u; }
RW_FUNCTION(0x76315C, "StageUI::openContinueDialog $_1 UlbE_ __func::target_type", t_76315C);

// StageUI::openContinueDialog $_2: __func::~__func
static uint32_t t_763170(uint32_t self) {
    st32(self, 0xf6e53cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x763170, "StageUI::openContinueDialog $_2 __func::~__func", t_763170);

// StageUI::openContinueDialog $_2: __func::~__func (deleting)
static void t_7631AC(uint32_t self) {
    st32(self, 0xf6e53cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7631AC, "StageUI::openContinueDialog $_2 __func::~__func (deleting)", t_7631AC);

// StageUI::openContinueDialog $_2: __func::__clone()
static uint32_t t_7631EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6e53cu);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x7631EC, "StageUI::openContinueDialog $_2 __func::__clone()", t_7631EC);

// StageUI::openContinueDialog $_2: __func::__clone(__base*)
static void t_76323C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e53cu);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x76323C, "StageUI::openContinueDialog $_2 __func::__clone(__base*)", t_76323C);

// StageUI::openContinueDialog $_2: __func::destroy
static void t_763270(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x763270, "StageUI::openContinueDialog $_2 __func::destroy", t_763270);

// StageUI::openContinueDialog $_2: __func::destroy_deallocate
static void t_763290(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x763290, "StageUI::openContinueDialog $_2 __func::destroy_deallocate", t_763290);

// StageUI::openContinueDialog $_2: __func::target
static uint32_t t_7633B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9fff0u ? self + 8 : 0; }
RW_FUNCTION(0x7633B0, "StageUI::openContinueDialog $_2 __func::target", t_7633B0);

// StageUI::openContinueDialog $_2: __func::target_type
static uint32_t t_7633D0() { return 0xfa8ce8u; }
RW_FUNCTION(0x7633D0, "StageUI::openContinueDialog $_2 __func::target_type", t_7633D0);

// StageUI::openRetireDialog $_0: __func::~__func
static uint32_t t_7633E4(uint32_t self) {
    st32(self, 0xf6e568u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x7633E4, "StageUI::openRetireDialog $_0 __func::~__func", t_7633E4);

// StageUI::openRetireDialog $_0: __func::~__func (deleting)
static void t_763420(uint32_t self) {
    st32(self, 0xf6e568u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x763420, "StageUI::openRetireDialog $_0 __func::~__func (deleting)", t_763420);

// StageUI::openRetireDialog $_0: __func::__clone()
static uint32_t t_763460(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf6e568u);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x45c84d, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x763460, "StageUI::openRetireDialog $_0 __func::__clone()", t_763460);

// StageUI::openRetireDialog $_0: __func::__clone(__base*)
static void t_7634B8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e568u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x45c84d, {p + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x7634B8, "StageUI::openRetireDialog $_0 __func::__clone(__base*)", t_7634B8);

// StageUI::openRetireDialog $_0: __func::destroy
static void t_7634F8(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x7634F8, "StageUI::openRetireDialog $_0 __func::destroy", t_7634F8);

// StageUI::openRetireDialog $_0: __func::destroy_deallocate
static void t_763518(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x763518, "StageUI::openRetireDialog $_0 __func::destroy_deallocate", t_763518);

// StageUI::openRetireDialog $_0: __func::target
static uint32_t t_76362C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea00a0u ? self + 8 : 0; }
RW_FUNCTION(0x76362C, "StageUI::openRetireDialog $_0 __func::target", t_76362C);

// StageUI::openRetireDialog $_0: __func::target_type
static uint32_t t_763650() { return 0xfa8cfcu; }
RW_FUNCTION(0x763650, "StageUI::openRetireDialog $_0 __func::target_type", t_763650);

// StageUI::openRetireDialog $_0 UlbE_ UlbE_: __func::~__func (deleting)
static void t_7637E0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7637E0, "StageUI::openRetireDialog $_0 UlbE_ UlbE_ __func::~__func (deleting)", t_7637E0);

// StageUI::openRetireDialog $_0 UlbE_ UlbE_: __func::__clone()
static uint32_t t_7637E8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e594u);
    }
    return v1;
}
RW_FUNCTION(0x7637E8, "StageUI::openRetireDialog $_0 UlbE_ UlbE_ __func::__clone()", t_7637E8);

// StageUI::openRetireDialog $_0 UlbE_ UlbE_: __func::__clone(__base*)
static void t_76380C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e594u);
    }
}
RW_FUNCTION(0x76380C, "StageUI::openRetireDialog $_0 UlbE_ UlbE_ __func::__clone(__base*)", t_76380C);

// StageUI::openRetireDialog $_0 UlbE_ UlbE_: __func::destroy
static void t_763828(uint32_t self) {
}
RW_FUNCTION(0x763828, "StageUI::openRetireDialog $_0 UlbE_ UlbE_ __func::destroy", t_763828);

// StageUI::openRetireDialog $_0 UlbE_ UlbE_: __func::destroy_deallocate
static void t_76382A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x76382A, "StageUI::openRetireDialog $_0 UlbE_ UlbE_ __func::destroy_deallocate", t_76382A);

// StageUI::openRetireDialog $_0 UlbE_ UlbE_: __func::target
static uint32_t t_763834(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea0170u ? self + 4 : 0; }
RW_FUNCTION(0x763834, "StageUI::openRetireDialog $_0 UlbE_ UlbE_ __func::target", t_763834);

// StageUI::openRetireDialog $_0 UlbE_ UlbE_: __func::target_type
static uint32_t t_763854() { return 0xfa8d10u; }
RW_FUNCTION(0x763854, "StageUI::openRetireDialog $_0 UlbE_ UlbE_ __func::target_type", t_763854);

// StageUI::openRetireDialog $_0 UlbE_: __func::~__func (deleting)
static void t_763868(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x763868, "StageUI::openRetireDialog $_0 UlbE_ __func::~__func (deleting)", t_763868);

// StageUI::openRetireDialog $_0 UlbE_: __func::__clone()
static uint32_t t_763870(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6e5c0u);
    }
    return v1;
}
RW_FUNCTION(0x763870, "StageUI::openRetireDialog $_0 UlbE_ __func::__clone()", t_763870);

// StageUI::openRetireDialog $_0 UlbE_: __func::__clone(__base*)
static void t_763894(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6e5c0u);
    }
}
RW_FUNCTION(0x763894, "StageUI::openRetireDialog $_0 UlbE_ __func::__clone(__base*)", t_763894);

// StageUI::openRetireDialog $_0 UlbE_: __func::destroy
static void t_7638B0(uint32_t self) {
}
RW_FUNCTION(0x7638B0, "StageUI::openRetireDialog $_0 UlbE_ __func::destroy", t_7638B0);

// StageUI::openRetireDialog $_0 UlbE_: __func::destroy_deallocate
static void t_7638B2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x7638B2, "StageUI::openRetireDialog $_0 UlbE_ __func::destroy_deallocate", t_7638B2);

// StageUI::openRetireDialog $_0 UlbE_: __func::target
static uint32_t t_7638C4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xea0250u ? self + 4 : 0; }
RW_FUNCTION(0x7638C4, "StageUI::openRetireDialog $_0 UlbE_ __func::target", t_7638C4);

// StageUI::openRetireDialog $_0 UlbE_: __func::target_type
static uint32_t t_7638E4() { return 0xfa8d24u; }
RW_FUNCTION(0x7638E4, "StageUI::openRetireDialog $_0 UlbE_ __func::target_type", t_7638E4);
