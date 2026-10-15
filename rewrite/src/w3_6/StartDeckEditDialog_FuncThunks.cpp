// The std::function __func thunks of the StartDeckEditDialog lambdas (generated from the lifted code by a scratch tool and
// checked against it): clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "Common.hpp"

using namespace w36;

// SceneUtil::openFBSharePhotoPopup  (shared): __func::~__func
static uint32_t t_48343C(uint32_t self) {
    return self;
}
RW_FUNCTION(0x48343C, "SceneUtil::openFBSharePhotoPopup  (shared) __func::~__func", t_48343C);

// StartDeckEditDialog::openCostOverDeckPopup $_37: __func::~__func
static uint32_t t_5787C8(uint32_t self) {
    st32(self, 0xf5956cu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x5787C8, "StartDeckEditDialog::openCostOverDeckPopup $_37 __func::~__func", t_5787C8);

// StartDeckEditDialog::openCostOverDeckPopup $_37: __func::~__func (deleting)
static void t_578804(uint32_t self) {
    st32(self, 0xf5956cu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x578804, "StartDeckEditDialog::openCostOverDeckPopup $_37 __func::~__func (deleting)", t_578804);

// StartDeckEditDialog::openCostOverDeckPopup $_37: __func::__clone()
static uint32_t t_578844(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf5956cu);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 12, v3);
        st32(v1 + 8, v2);
        uint32_t v4 = ld32(self + 16 + 0);
        uint32_t v5 = ld32(self + 16 + 4);
        uint32_t v6 = ld32(self + 16 + 8);
        st32(v1 + 16 + 0, v4);
        st32(v1 + 16 + 4, v5);
        st32(v1 + 16 + 8, v6);
        call(0x45c84d, {v1 + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x578844, "StartDeckEditDialog::openCostOverDeckPopup $_37 __func::__clone()", t_578844);

// StartDeckEditDialog::openCostOverDeckPopup $_37: __func::__clone(__base*)
static void t_5788A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5956cu);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 12, v2);
        st32(p + 8, v1);
        uint32_t v3 = ld32(self + 16 + 0);
        uint32_t v4 = ld32(self + 16 + 4);
        uint32_t v5 = ld32(self + 16 + 8);
        st32(p + 24, v5);
        st32(p + 20, v4);
        st32(p + 16, v3);
        call(0x45c84d, {p + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x5788A8, "StartDeckEditDialog::openCostOverDeckPopup $_37 __func::__clone(__base*)", t_5788A8);

// StartDeckEditDialog::openCostOverDeckPopup $_37: __func::destroy
static void t_5788F0(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x5788F0, "StartDeckEditDialog::openCostOverDeckPopup $_37 __func::destroy", t_5788F0);

// StartDeckEditDialog::openCostOverDeckPopup $_37: __func::destroy_deallocate
static void t_578910(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x578910, "StartDeckEditDialog::openCostOverDeckPopup $_37 __func::destroy_deallocate", t_578910);

// StartDeckEditDialog::openCostOverDeckPopup $_37: __func::target
static uint32_t t_57897C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe34900u ? self + 8 : 0; }
RW_FUNCTION(0x57897C, "StartDeckEditDialog::openCostOverDeckPopup $_37 __func::target", t_57897C);

// StartDeckEditDialog::openCostOverDeckPopup $_37: __func::target_type
static uint32_t t_5789A0() { return 0xf8aec8u; }
RW_FUNCTION(0x5789A0, "StartDeckEditDialog::openCostOverDeckPopup $_37 __func::target_type", t_5789A0);

// StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_: __func::~__func
static uint32_t t_578B50(uint32_t self) {
    st32(self, 0xf59598u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x578B50, "StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_ __func::~__func", t_578B50);

// StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_: __func::~__func (deleting)
static void t_578B8C(uint32_t self) {
    st32(self, 0xf59598u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x578B8C, "StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_ __func::~__func (deleting)", t_578B8C);

// StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_: __func::__clone()
static uint32_t t_578BCC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf59598u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x45c84d, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v5 = ld8(self + 48);
        st8(v1 + 48, v5);
    }
    return v1;
}
RW_FUNCTION(0x578BCC, "StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_ __func::__clone()", t_578BCC);

// StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_: __func::__clone(__base*)
static void t_578C2C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59598u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v4 = ld8(self + 48);
        st8(p + 48, v4);
    }
}
RW_FUNCTION(0x578C2C, "StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_ __func::__clone(__base*)", t_578C2C);

// StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_: __func::destroy
static void t_578C78(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x578C78, "StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_ __func::destroy", t_578C78);

// StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_: __func::destroy_deallocate
static void t_578C98(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x578C98, "StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_ __func::destroy_deallocate", t_578C98);

// StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_: __func::target
static uint32_t t_578CFC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe349e0u ? self + 8 : 0; }
RW_FUNCTION(0x578CFC, "StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_ __func::target", t_578CFC);

// StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_: __func::target_type
static uint32_t t_578D20() { return 0xf8aedcu; }
RW_FUNCTION(0x578D20, "StartDeckEditDialog::openCostOverDeckPopup $_35 UlvE_ __func::target_type", t_578D20);

// StartDeckEditDialog::openCostOverDeckPopup $_36: __func::~__func
static uint32_t t_578D34(uint32_t self) {
    st32(self, 0xf595c4u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x578D34, "StartDeckEditDialog::openCostOverDeckPopup $_36 __func::~__func", t_578D34);

// StartDeckEditDialog::openCostOverDeckPopup $_36: __func::~__func (deleting)
static void t_578D70(uint32_t self) {
    st32(self, 0xf595c4u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x578D70, "StartDeckEditDialog::openCostOverDeckPopup $_36 __func::~__func (deleting)", t_578D70);

// StartDeckEditDialog::openCostOverDeckPopup $_36: __func::__clone()
static uint32_t t_578DB0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf595c4u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 12, v3);
        st32(v1 + 8, v2);
        uint32_t v4 = ld32(self + 16 + 0);
        uint32_t v5 = ld32(self + 16 + 4);
        uint32_t v6 = ld32(self + 16 + 8);
        st32(v1 + 16 + 0, v4);
        st32(v1 + 16 + 4, v5);
        st32(v1 + 16 + 8, v6);
        call(0x45c84d, {v1 + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x578DB0, "StartDeckEditDialog::openCostOverDeckPopup $_36 __func::__clone()", t_578DB0);

// StartDeckEditDialog::openCostOverDeckPopup $_36: __func::__clone(__base*)
static void t_578E14(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf595c4u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 12, v2);
        st32(p + 8, v1);
        uint32_t v3 = ld32(self + 16 + 0);
        uint32_t v4 = ld32(self + 16 + 4);
        uint32_t v5 = ld32(self + 16 + 8);
        st32(p + 24, v5);
        st32(p + 20, v4);
        st32(p + 16, v3);
        call(0x45c84d, {p + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x578E14, "StartDeckEditDialog::openCostOverDeckPopup $_36 __func::__clone(__base*)", t_578E14);

// StartDeckEditDialog::openCostOverDeckPopup $_36: __func::destroy
static void t_578E5C(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x578E5C, "StartDeckEditDialog::openCostOverDeckPopup $_36 __func::destroy", t_578E5C);

// StartDeckEditDialog::openCostOverDeckPopup $_36: __func::destroy_deallocate
static void t_578E7C(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x578E7C, "StartDeckEditDialog::openCostOverDeckPopup $_36 __func::destroy_deallocate", t_578E7C);

// StartDeckEditDialog::openCostOverDeckPopup $_36: __func::target
static uint32_t t_578EE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe34af0u ? self + 8 : 0; }
RW_FUNCTION(0x578EE4, "StartDeckEditDialog::openCostOverDeckPopup $_36 __func::target", t_578EE4);

// StartDeckEditDialog::openCostOverDeckPopup $_36: __func::target_type
static uint32_t t_578F08() { return 0xf8aef0u; }
RW_FUNCTION(0x578F08, "StartDeckEditDialog::openCostOverDeckPopup $_36 __func::target_type", t_578F08);

// StartDeckEditDialog::openAbilityMaxPopup $_34: __func::~__func (deleting)
static void t_578F1C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x578F1C, "StartDeckEditDialog::openAbilityMaxPopup $_34 __func::~__func (deleting)", t_578F1C);

// StartDeckEditDialog::openAbilityMaxPopup $_34: __func::__clone()
static uint32_t t_578F24(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf595f0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x578F24, "StartDeckEditDialog::openAbilityMaxPopup $_34 __func::__clone()", t_578F24);

// StartDeckEditDialog::openAbilityMaxPopup $_34: __func::__clone(__base*)
static void t_578F50(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf595f0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x578F50, "StartDeckEditDialog::openAbilityMaxPopup $_34 __func::__clone(__base*)", t_578F50);

// StartDeckEditDialog::openAbilityMaxPopup $_34: __func::destroy
static void t_578F70(uint32_t self) {
}
RW_FUNCTION(0x578F70, "StartDeckEditDialog::openAbilityMaxPopup $_34 __func::destroy", t_578F70);

// StartDeckEditDialog::openAbilityMaxPopup $_34: __func::destroy_deallocate
static void t_578F72(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x578F72, "StartDeckEditDialog::openAbilityMaxPopup $_34 __func::destroy_deallocate", t_578F72);

// StartDeckEditDialog::openAbilityMaxPopup $_34: __func::target
static uint32_t t_578F84(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe34bc0u ? self + 4 : 0; }
RW_FUNCTION(0x578F84, "StartDeckEditDialog::openAbilityMaxPopup $_34 __func::target", t_578F84);

// StartDeckEditDialog::openAbilityMaxPopup $_34: __func::target_type
static uint32_t t_578FA4() { return 0xf8af04u; }
RW_FUNCTION(0x578FA4, "StartDeckEditDialog::openAbilityMaxPopup $_34 __func::target_type", t_578FA4);

// StartDeckEditDialog::openAbilityMaxPopup $_33: __func::~__func
static uint32_t t_578FB8(uint32_t self) {
    st32(self, 0xf5961cu);
    uint32_t v1 = ld32(self + 136);
    functionDestroy(self + 120);
    return self;
}
RW_FUNCTION(0x578FB8, "StartDeckEditDialog::openAbilityMaxPopup $_33 __func::~__func", t_578FB8);

// StartDeckEditDialog::openAbilityMaxPopup $_33: __func::~__func (deleting)
static void t_578FF8(uint32_t self) {
    st32(self, 0xf5961cu);
    uint32_t v1 = ld32(self + 136);
    functionDestroy(self + 120);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x578FF8, "StartDeckEditDialog::openAbilityMaxPopup $_33 __func::~__func (deleting)", t_578FF8);
