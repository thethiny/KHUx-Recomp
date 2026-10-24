// Lambdas of StartDeckEditDialog::openAbilityMaxPopup ($_29 .. $_33, $_38 and their nested lambdas) and
// StartDeckEditDialog::startColiseum ($_27, $_28) that owners.py attributes to SceneAdventureSelect (they sit
// between the two classes in the binary). The std::function __func slots were generated from the lifted code by
// a scratch tool (gen_thunks.py, the same one as src/scene_social/thunks) and mirror it exactly: clone (copy the
// captures into a new / given block), destroy (destroy the captured objects), target / target_type (typeinfo
// checks). The lambda bodies (operator()) follow, written by hand.
#include "SceneAdventureSelect.hpp"

using namespace scene;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
constexpr uint32_t AEABI_MEMCPY = 0x40E164;   // __aeabi_memcpy(dst, src, n) (import)
}  // namespace

// ------------------------------------------------------------------------------------------------ thunks
// StartDeckEditDialog::openAbilityMaxPopup $_33 (__func 0x90 bytes: 0x70 bytes of plain captures at +8, then a
// std::function<void()> at +0x78): __func::__clone()
static uint32_t t_57903C(uint32_t self) {
    uint32_t obj = call(fn::operator_new, {0x90});
    if (obj) {
        st32(obj, 0xf5961cu);
        call(AEABI_MEMCPY, {obj + 8, self + 8, 0x70});
        call(fn::function_vv_copy, {obj + 0x78, self + 0x78});
    }
    return obj;
}
RW_FUNCTION(0x57903C, "StartDeckEditDialog::openAbilityMaxPopup $_33 __func::__clone()", t_57903C);

// StartDeckEditDialog::openAbilityMaxPopup $_33: __func::__clone(__base*)
static void t_5790A4(uint32_t self, uint32_t p) {
    if (!p) return;
    st32(p, 0xf5961cu);
    call(AEABI_MEMCPY, {p + 8, self + 8, 0x70});
    call(fn::function_vv_copy, {p + 0x78, self + 0x78});
}
RW_FUNCTION(0x5790A4, "StartDeckEditDialog::openAbilityMaxPopup $_33 __func::__clone(__base*)", t_5790A4);

// StartDeckEditDialog::openAbilityMaxPopup $_33: __func::destroy
static void t_5790F0(uint32_t self) {
    uint32_t v1 = ld32(self + 136);
    functionDestroy(self + 120);
}
RW_FUNCTION(0x5790F0, "StartDeckEditDialog::openAbilityMaxPopup $_33 __func::destroy", t_5790F0);

// StartDeckEditDialog::openAbilityMaxPopup $_33: __func::destroy_deallocate
static void t_579112(uint32_t self) {
    uint32_t v1 = ld32(self + 136);
    functionDestroy(self + 120);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579112, "StartDeckEditDialog::openAbilityMaxPopup $_33 __func::destroy_deallocate", t_579112);

// StartDeckEditDialog::openAbilityMaxPopup $_33: __func::target
static uint32_t t_57944C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe34c90u ? self + 8 : 0; }
RW_FUNCTION(0x57944C, "StartDeckEditDialog::openAbilityMaxPopup $_33 __func::target", t_57944C);

// StartDeckEditDialog::openAbilityMaxPopup $_33: __func::target_type
static uint32_t t_579470() { return 0xf8af18u; }
RW_FUNCTION(0x579470, "StartDeckEditDialog::openAbilityMaxPopup $_33 __func::target_type", t_579470);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_: __func::~__func (deleting)
static void t_579484(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579484, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ __func::~__func (deleting)", t_579484);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_: __func::__clone()
static uint32_t t_57948C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59648u);
    }
    return v1;
}
RW_FUNCTION(0x57948C, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ __func::__clone()", t_57948C);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_: __func::__clone(__base*)
static void t_5794B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59648u);
    }
}
RW_FUNCTION(0x5794B0, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ __func::__clone(__base*)", t_5794B0);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_: __func::destroy
static void t_5794CC(uint32_t self) {
}
RW_FUNCTION(0x5794CC, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ __func::destroy", t_5794CC);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_: __func::destroy_deallocate
static void t_5794CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5794CE, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ __func::destroy_deallocate", t_5794CE);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_: __func::target
static uint32_t t_579578(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe34d70u ? self + 4 : 0; }
RW_FUNCTION(0x579578, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ __func::target", t_579578);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_: __func::target_type
static uint32_t t_579598() { return 0xf8af2cu; }
RW_FUNCTION(0x579598, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ __func::target_type", t_579598);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_: __func::~__func (deleting)
static void t_5795AC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5795AC, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_ __func::~__func (deleting)", t_5795AC);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_: __func::__clone()
static uint32_t t_5795B4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59674u);
    }
    return v1;
}
RW_FUNCTION(0x5795B4, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_ __func::__clone()", t_5795B4);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_: __func::__clone(__base*)
static void t_5795D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59674u);
    }
}
RW_FUNCTION(0x5795D8, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_ __func::__clone(__base*)", t_5795D8);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_: __func::destroy
static void t_5795F4(uint32_t self) {
}
RW_FUNCTION(0x5795F4, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_ __func::destroy", t_5795F4);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_: __func::destroy_deallocate
static void t_5795F6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5795F6, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_ __func::destroy_deallocate", t_5795F6);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_: __func::target
static uint32_t t_579600(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe34e70u ? self + 4 : 0; }
RW_FUNCTION(0x579600, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_ __func::target", t_579600);

// StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_: __func::target_type
static uint32_t t_579620() { return 0xf8af40u; }
RW_FUNCTION(0x579620, "StartDeckEditDialog::openAbilityMaxPopup $_33 UlbE_ UlvE_ __func::target_type", t_579620);

// StartDeckEditDialog::openAbilityMaxPopup $_31: __func::~__func
static uint32_t t_579634(uint32_t self) {
    st32(self, 0xf596a0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x579634, "StartDeckEditDialog::openAbilityMaxPopup $_31 __func::~__func", t_579634);

// StartDeckEditDialog::openAbilityMaxPopup $_31: __func::~__func (deleting)
static void t_579670(uint32_t self) {
    st32(self, 0xf596a0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579670, "StartDeckEditDialog::openAbilityMaxPopup $_31 __func::~__func (deleting)", t_579670);

// StartDeckEditDialog::openAbilityMaxPopup $_31: __func::__clone()
static uint32_t t_5796B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf596a0u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5796B0, "StartDeckEditDialog::openAbilityMaxPopup $_31 __func::__clone()", t_5796B0);

// StartDeckEditDialog::openAbilityMaxPopup $_31: __func::__clone(__base*)
static void t_579700(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf596a0u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x579700, "StartDeckEditDialog::openAbilityMaxPopup $_31 __func::__clone(__base*)", t_579700);

// StartDeckEditDialog::openAbilityMaxPopup $_31: __func::destroy
static void t_579734(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x579734, "StartDeckEditDialog::openAbilityMaxPopup $_31 __func::destroy", t_579734);

// StartDeckEditDialog::openAbilityMaxPopup $_31: __func::destroy_deallocate
static void t_579754(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579754, "StartDeckEditDialog::openAbilityMaxPopup $_31 __func::destroy_deallocate", t_579754);

// StartDeckEditDialog::openAbilityMaxPopup $_31: __func::target
static uint32_t t_579918(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe34f60u ? self + 8 : 0; }
RW_FUNCTION(0x579918, "StartDeckEditDialog::openAbilityMaxPopup $_31 __func::target", t_579918);

// StartDeckEditDialog::openAbilityMaxPopup $_31: __func::target_type
static uint32_t t_579938() { return 0xf8af54u; }
RW_FUNCTION(0x579938, "StartDeckEditDialog::openAbilityMaxPopup $_31 __func::target_type", t_579938);

// StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_: __func::~__func (deleting)
static void t_57994C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57994C, "StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_ __func::~__func (deleting)", t_57994C);

// StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_: __func::__clone()
static uint32_t t_579954(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf596ccu);
    }
    return v1;
}
RW_FUNCTION(0x579954, "StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_ __func::__clone()", t_579954);

// StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_: __func::__clone(__base*)
static void t_579978(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf596ccu);
    }
}
RW_FUNCTION(0x579978, "StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_ __func::__clone(__base*)", t_579978);

// StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_: __func::destroy
static void t_579994(uint32_t self) {
}
RW_FUNCTION(0x579994, "StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_ __func::destroy", t_579994);

// StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_: __func::destroy_deallocate
static void t_579996(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579996, "StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_ __func::destroy_deallocate", t_579996);

// StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_: __func::target
static uint32_t t_5799A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35040u ? self + 4 : 0; }
RW_FUNCTION(0x5799A0, "StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_ __func::target", t_5799A0);

// StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_: __func::target_type
static uint32_t t_5799C0() { return 0xf8af68u; }
RW_FUNCTION(0x5799C0, "StartDeckEditDialog::openAbilityMaxPopup $_31 UlbE_ __func::target_type", t_5799C0);

// StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_: __func::~__func (deleting)
static void t_5799D4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5799D4, "StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_ __func::~__func (deleting)", t_5799D4);

// StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_: __func::__clone()
static uint32_t t_5799DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf596f8u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x5799DC, "StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_ __func::__clone()", t_5799DC);

// StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_: __func::__clone(__base*)
static void t_579A0C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf596f8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x579A0C, "StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_ __func::__clone(__base*)", t_579A0C);

// StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_: __func::destroy
static void t_579A34(uint32_t self) {
}
RW_FUNCTION(0x579A34, "StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_ __func::destroy", t_579A34);

// StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_: __func::destroy_deallocate
static void t_579A36(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579A36, "StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_ __func::destroy_deallocate", t_579A36);

// StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_: __func::target
static uint32_t t_579A68(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35130u ? self + 4 : 0; }
RW_FUNCTION(0x579A68, "StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_ __func::target", t_579A68);

// StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_: __func::target_type
static uint32_t t_579A88() { return 0xf8af7cu; }
RW_FUNCTION(0x579A88, "StartDeckEditDialog::openAbilityMaxPopup $_38 UlvE_ __func::target_type", t_579A88);

// StartDeckEditDialog::openAbilityMaxPopup $_32: __func::~__func (deleting)
static void t_579A9C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579A9C, "StartDeckEditDialog::openAbilityMaxPopup $_32 __func::~__func (deleting)", t_579A9C);

// StartDeckEditDialog::openAbilityMaxPopup $_32: __func::__clone()
static uint32_t t_579AA4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf59724u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x579AA4, "StartDeckEditDialog::openAbilityMaxPopup $_32 __func::__clone()", t_579AA4);

// StartDeckEditDialog::openAbilityMaxPopup $_32: __func::__clone(__base*)
static void t_579AD4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59724u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x579AD4, "StartDeckEditDialog::openAbilityMaxPopup $_32 __func::__clone(__base*)", t_579AD4);

// StartDeckEditDialog::openAbilityMaxPopup $_32: __func::destroy
static void t_579AF8(uint32_t self) {
}
RW_FUNCTION(0x579AF8, "StartDeckEditDialog::openAbilityMaxPopup $_32 __func::destroy", t_579AF8);

// StartDeckEditDialog::openAbilityMaxPopup $_32: __func::destroy_deallocate
static void t_579AFA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579AFA, "StartDeckEditDialog::openAbilityMaxPopup $_32 __func::destroy_deallocate", t_579AFA);

// StartDeckEditDialog::openAbilityMaxPopup $_32: __func::target
static uint32_t t_579B20(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35210u ? self + 4 : 0; }
RW_FUNCTION(0x579B20, "StartDeckEditDialog::openAbilityMaxPopup $_32 __func::target", t_579B20);

// StartDeckEditDialog::openAbilityMaxPopup $_32: __func::target_type
static uint32_t t_579B40() { return 0xf8af90u; }
RW_FUNCTION(0x579B40, "StartDeckEditDialog::openAbilityMaxPopup $_32 __func::target_type", t_579B40);

// StartDeckEditDialog::openAbilityMaxPopup $_30: __func::~__func (deleting)
static void t_579B54(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579B54, "StartDeckEditDialog::openAbilityMaxPopup $_30 __func::~__func (deleting)", t_579B54);

// StartDeckEditDialog::openAbilityMaxPopup $_30: __func::__clone()
static uint32_t t_579B5C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59750u);
    }
    return v1;
}
RW_FUNCTION(0x579B5C, "StartDeckEditDialog::openAbilityMaxPopup $_30 __func::__clone()", t_579B5C);

// StartDeckEditDialog::openAbilityMaxPopup $_30: __func::__clone(__base*)
static void t_579B80(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59750u);
    }
}
RW_FUNCTION(0x579B80, "StartDeckEditDialog::openAbilityMaxPopup $_30 __func::__clone(__base*)", t_579B80);

// StartDeckEditDialog::openAbilityMaxPopup $_30: __func::destroy
static void t_579B9C(uint32_t self) {
}
RW_FUNCTION(0x579B9C, "StartDeckEditDialog::openAbilityMaxPopup $_30 __func::destroy", t_579B9C);

// StartDeckEditDialog::openAbilityMaxPopup $_30: __func::destroy_deallocate
static void t_579B9E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579B9E, "StartDeckEditDialog::openAbilityMaxPopup $_30 __func::destroy_deallocate", t_579B9E);

// StartDeckEditDialog::openAbilityMaxPopup $_30: __func::target
static uint32_t t_579BBC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35310u ? self + 4 : 0; }
RW_FUNCTION(0x579BBC, "StartDeckEditDialog::openAbilityMaxPopup $_30 __func::target", t_579BBC);

// StartDeckEditDialog::openAbilityMaxPopup $_30: __func::target_type
static uint32_t t_579BDC() { return 0xf8afa4u; }
RW_FUNCTION(0x579BDC, "StartDeckEditDialog::openAbilityMaxPopup $_30 __func::target_type", t_579BDC);

// StartDeckEditDialog::openAbilityMaxPopup $_29: __func::~__func (deleting)
static void t_579BF0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579BF0, "StartDeckEditDialog::openAbilityMaxPopup $_29 __func::~__func (deleting)", t_579BF0);

// StartDeckEditDialog::openAbilityMaxPopup $_29: __func::__clone()
static uint32_t t_579BF8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5977cu);
    }
    return v1;
}
RW_FUNCTION(0x579BF8, "StartDeckEditDialog::openAbilityMaxPopup $_29 __func::__clone()", t_579BF8);

// StartDeckEditDialog::openAbilityMaxPopup $_29: __func::__clone(__base*)
static void t_579C1C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5977cu);
    }
}
RW_FUNCTION(0x579C1C, "StartDeckEditDialog::openAbilityMaxPopup $_29 __func::__clone(__base*)", t_579C1C);

// StartDeckEditDialog::openAbilityMaxPopup $_29: __func::destroy
static void t_579C38(uint32_t self) {
}
RW_FUNCTION(0x579C38, "StartDeckEditDialog::openAbilityMaxPopup $_29 __func::destroy", t_579C38);

// StartDeckEditDialog::openAbilityMaxPopup $_29: __func::destroy_deallocate
static void t_579C3A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579C3A, "StartDeckEditDialog::openAbilityMaxPopup $_29 __func::destroy_deallocate", t_579C3A);

// StartDeckEditDialog::openAbilityMaxPopup $_29: __func::target
static uint32_t t_579C58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35410u ? self + 4 : 0; }
RW_FUNCTION(0x579C58, "StartDeckEditDialog::openAbilityMaxPopup $_29 __func::target", t_579C58);

// StartDeckEditDialog::openAbilityMaxPopup $_29: __func::target_type
static uint32_t t_579C78() { return 0xf8afb8u; }
RW_FUNCTION(0x579C78, "StartDeckEditDialog::openAbilityMaxPopup $_29 __func::target_type", t_579C78);

// StartDeckEditDialog::startColiseum $_28: __func::~__func (deleting)
static void t_579C8C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579C8C, "StartDeckEditDialog::startColiseum $_28 __func::~__func (deleting)", t_579C8C);

// StartDeckEditDialog::startColiseum $_28: __func::__clone()
static uint32_t t_579C94(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf597a8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x579C94, "StartDeckEditDialog::startColiseum $_28 __func::__clone()", t_579C94);

// StartDeckEditDialog::startColiseum $_28: __func::__clone(__base*)
static void t_579CC0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf597a8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x579CC0, "StartDeckEditDialog::startColiseum $_28 __func::__clone(__base*)", t_579CC0);

// StartDeckEditDialog::startColiseum $_28: __func::destroy
static void t_579CE0(uint32_t self) {
}
RW_FUNCTION(0x579CE0, "StartDeckEditDialog::startColiseum $_28 __func::destroy", t_579CE0);

// StartDeckEditDialog::startColiseum $_28: __func::destroy_deallocate
static void t_579CE2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579CE2, "StartDeckEditDialog::startColiseum $_28 __func::destroy_deallocate", t_579CE2);

// StartDeckEditDialog::startColiseum $_28: __func::target
static uint32_t t_579CF8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe354e0u ? self + 4 : 0; }
RW_FUNCTION(0x579CF8, "StartDeckEditDialog::startColiseum $_28 __func::target", t_579CF8);

// StartDeckEditDialog::startColiseum $_28: __func::target_type
static uint32_t t_579D18() { return 0xf8afccu; }
RW_FUNCTION(0x579D18, "StartDeckEditDialog::startColiseum $_28 __func::target_type", t_579D18);

// StartDeckEditDialog::startColiseum $_27: __func::~__func (deleting)
static void t_579D2C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579D2C, "StartDeckEditDialog::startColiseum $_27 __func::~__func (deleting)", t_579D2C);

// StartDeckEditDialog::startColiseum $_27: __func::__clone()
static uint32_t t_579D34(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf597d4u);
    }
    return v1;
}
RW_FUNCTION(0x579D34, "StartDeckEditDialog::startColiseum $_27 __func::__clone()", t_579D34);

// StartDeckEditDialog::startColiseum $_27: __func::__clone(__base*)
static void t_579D58(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf597d4u);
    }
}
RW_FUNCTION(0x579D58, "StartDeckEditDialog::startColiseum $_27 __func::__clone(__base*)", t_579D58);

// StartDeckEditDialog::startColiseum $_27: __func::destroy
static void t_579D74(uint32_t self) {
}
RW_FUNCTION(0x579D74, "StartDeckEditDialog::startColiseum $_27 __func::destroy", t_579D74);

// StartDeckEditDialog::startColiseum $_27: __func::destroy_deallocate
static void t_579D76(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x579D76, "StartDeckEditDialog::startColiseum $_27 __func::destroy_deallocate", t_579D76);

// StartDeckEditDialog::startColiseum $_27: __func::target
static uint32_t t_579E7C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35580u ? self + 4 : 0; }
RW_FUNCTION(0x579E7C, "StartDeckEditDialog::startColiseum $_27 __func::target", t_579E7C);

// StartDeckEditDialog::startColiseum $_27: __func::target_type
static uint32_t t_579E9C() { return 0xf8afe0u; }
RW_FUNCTION(0x579E9C, "StartDeckEditDialog::startColiseum $_27 __func::target_type", t_579E9C);

// ------------------------------------------------------------------------------------------------ lambda bodies
namespace {
constexpr uint32_t SafeBuffer_read = 0x45A269;              // (sb, dst, src, n, index)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;   // (api, const function<void(bool)>&, bool)
constexpr uint32_t APIManager_executeKeybladeBurstMax = 0x4C9DC9;   // (api, u64 id, const function<void(bool)>&)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;   // (mgr, bool)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;    // (mgr, function<Layer*()>*, bool, bool, FadeType)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;   // (node, std::function<void()>)
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;           // (string, function<void(bool)>, const char*)
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;     // (string, function<void(bool)>, ok, cancel)
constexpr uint32_t SceneAdventureSelect_callAdventureSelectAPI = 0x71D261;   // (function<void(bool)>)
constexpr uint32_t BuyMagicStoneDialog_open = 0x588CAD;     // (const function<void()>&, bool)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;                // (float duration, uint8 opacity)
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Widget_isTouchEnabled = rwlib::cocos2d::ui::Widget::isTouchEnabled;
constexpr uint32_t start_movie = 0x6C4A55;                  // Layer* start_movie(int, bool)

constexpr uint32_t POPUP_FADE_DURATION = 0xDEC3D0;          // a const float (0.15) the popups fade out with
constexpr uint32_t VT_LayerFactory = 0xF7E130;              // __func<Layer*(*)(), ...>
constexpr uint32_t CreateSceneLayer_AdventureSelect = 0x579EB1;   // createSceneLayer<SceneAdventureSelect>

// openAbilityMaxPopup lambdas (__func vtables)
constexpr uint32_t VT_abilityMax_33_inner = 0xF59648;       // $_33::{lambda(bool)#1}
constexpr uint32_t VT_abilityMax_33_inner2 = 0xF59674;      // $_33::{lambda(bool)#1}::{lambda()#1}
constexpr uint32_t VT_abilityMax_31 = 0xF596A0;             // $_31: [done] (bool)
constexpr uint32_t VT_abilityMax_31_inner = 0xF596CC;       // $_31::{lambda(bool)#1}
constexpr uint32_t VT_abilityMax_38_inner = 0xF596F8;       // $_38::{lambda()#1}: [parent, shade, popup]

constexpr uint32_t API_USER_POINT = 0xA4;                   // APIManager: SafeBuffer* (jewels: free at 0, paid at 4)
constexpr uint32_t API_ERROR_PENDING = 0x14;                // APIManager: byte, an error dialog is due
constexpr uint32_t SM_FLAG_30A = 0x30A;                     // SceneManager byte set after re-entering the scene
constexpr uint32_t TEXT_NOT_ENOUGH_JEWELS = 0x61D8D50;      // 102600016
constexpr uint32_t TEXT_ABILITY_MAXED = 0x623A7CD;          // 103000013
constexpr uint32_t WIDGET_SET_TOUCH_ENABLED = 0x29C;        // ui::Widget::setTouchEnabled(bool) (vtable offset)
constexpr uint32_t NODE_REMOVE_CHILD = 0x12C;               // Node::removeChild(child, cleanup)

uint32_t decodeWord(Frame &f, uint32_t sb, uint32_t index) {
    uint32_t dst = f.alloc(4);
    call(SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + index, 4, index});
    return ld32(dst);
}
}  // namespace

struct StartDeckEditDialogLambdas {
    // openAbilityMaxPopup $_33 (bool ok): the "raise the ability to max" popup closes (buttons off, shade fades,
    // close animation that removes it); on OK, pay with jewels when there are enough (API keyblade burst max,
    // then $_31), else the not-enough-jewels dialog (its inner lambda opens the shop).
    // captures: +8 / +0xC the two buttons, +0x10 the shade, +0x14 the popup, +0x18 its parent, +0x1C the price,
    // +0x20 the keyblade id (u64), +0x78 done (std::function<void()>)
    static void abilityMaxClosed(uint32_t functor, const uint8_t *okArg) {
        uint8_t ok = *okArg;
        uint32_t button1 = ld32(functor + 8);
        vcall(button1, WIDGET_SET_TOUCH_ENABLED, {button1, 0});
        uint32_t button2 = ld32(functor + 0xC);
        vcall(button2, WIDGET_SET_TOUCH_ENABLED, {button2, 0});
        uint32_t duration = ld32(POPUP_FADE_DURATION);
        uint32_t shade = ld32(functor + 0x10);
        call(Node_runAction, {shade, call(FadeTo_create, {duration, 0})});
        Frame f;
        uint32_t popup = ld32(functor + 0x14);
        uint32_t remove = newFunction(f, VT_abilityMax_38_inner,
                                      {ld32(functor + 0x18), ld32(functor + 0x10), popup});
        call(Node_runAction, {popup, call(SceneUtil_createClosePopupAnimation, {popup, remove})});
        functionDestroy(remove);
        if (!ok) {
            call(AudioPlaySystemSECancel, {});
            return;
        }
        call(AudioPlaySystemSEDecide, {});
        int32_t price = int32_t(ld32(functor + 0x1C));
        uint32_t free = decodeWord(f, ld32(call(APIManager_getInstance, {}) + API_USER_POINT), 0);
        uint32_t paid = decodeWord(f, ld32(call(APIManager_getInstance, {}) + API_USER_POINT), 4);
        if (price <= int32_t(paid + free)) {
            call(SceneManager_enableLoadingIndicator, {sceneManager(), 1});
            uint32_t api = call(APIManager_getInstance, {});
            uint32_t idLo = ld32(functor + 0x20), idHi = ld32(functor + 0x24);
            uint32_t done = f.alloc(24);
            uint32_t callback = f.alloc(24);
            call(fn::function_vv_copy, {done, functor + 0x78});
            st32(callback + 16, 0);
            uint32_t heap = call(fn::operator_new, {0x20});
            if (heap) {
                st32(heap, VT_abilityMax_31);
                functionCopy(heap + 8, done);     // the capture is copied (inline __clone), not moved
            }
            st32(callback + 16, heap);
            call(APIManager_executeKeybladeBurstMax, {api, 0, idLo, idHi, callback});
            functionDestroy(callback);
            functionDestroy(done);
        } else {
            uint32_t text = textData(f, int32_t(TEXT_NOT_ENOUGH_JEWELS));
            uint32_t message = newStr(f, textDataStr(text));
            uint32_t onClose = newFunction(f, VT_abilityMax_33_inner, {});
            call(SceneUtil_openOkCancelDialog4Line, {message, onClose, 0, 0});
            functionDestroy(onClose);
            strFree(message);
            sharedRelease(text);
        }
    }

    // $_33::{lambda(bool)#1}: OK on the not-enough-jewels dialog opens the jewel shop
    static void openShop(uint32_t functor, const uint8_t *ok) {
        (void)functor;
        if (!*ok) return;
        Frame f;
        uint32_t none = newFunction(f, VT_abilityMax_33_inner2, {});
        call(BuyMagicStoneDialog_open, {none, 0});
        functionDestroy(none);
    }

    // the empty lambdas (the shop callback of $_33, the error dialog callback of $_31)
    static void nothing(uint32_t functor) { (void)functor; }

    // openAbilityMaxPopup $_31 (bool success): the keyblade burst max API answered
    static void burstMaxDone(uint32_t functor, const uint8_t *successArg) {
        uint8_t success = *successArg;
        call(SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        Frame f;
        if (success) {
            call(SceneManager_updateHeader, {sceneManager()});
            call(fn::function_vv_call, {functor + 8});
            uint32_t probe = textData(f, int32_t(TEXT_ABILITY_MAXED));
            uint32_t entry = ld32(probe);
            sharedRelease(probe);
            if (!entry) return;
            uint32_t text = textData(f, int32_t(TEXT_ABILITY_MAXED));
            uint32_t message = newStr(f, textDataStr(text));
            uint32_t none = newEmptyFunction(f);
            call(SceneUtil_openOkDialog4Line, {message, none, 0});
            functionDestroy(none);
            strFree(message);
            sharedRelease(text);
        } else {
            if (!ld8(call(APIManager_getInstance, {}) + API_ERROR_PENDING)) return;
            uint32_t api = call(APIManager_getInstance, {});
            uint32_t onClose = newFunction(f, VT_abilityMax_31_inner, {});
            call(APIManager_openErrorDialog, {api, onClose, 1});
            functionDestroy(onClose);
        }
    }

    // $_38::{lambda()#1}: the closed popup and its shade are removed from the parent
    // captures: +4 parent, +8 shade, +0xC popup
    static void removePopup(uint32_t functor) {
        uint32_t parent = ld32(functor + 4);
        vcall(parent, NODE_REMOVE_CHILD, {parent, ld32(functor + 8), 1});
        parent = ld32(functor + 4);
        vcall(parent, NODE_REMOVE_CHILD, {parent, ld32(functor + 0xC), 1});
    }

    // $_32: the popup accepts a tap only when both buttons are touch enabled
    static uint32_t buttonsEnabled(uint32_t functor) {
        if (call(Widget_isTouchEnabled, {ld32(functor + 4)}) != 1) return 0;
        return call(Widget_isTouchEnabled, {ld32(functor + 8)});
    }

    // $_30 / $_29 (Ref*, TouchEventType): a tap released (type 2) closes the popup (cancel / OK)
    static void tapCancel(uint32_t functor, uint32_t sender, const int32_t *type) {
        (void)functor;
        (void)sender;
        if (*type == 2) call(SceneManager_closeCurrentPopup, {sceneManager(), 0});
    }
    static void tapOk(uint32_t functor, uint32_t sender, const int32_t *type) {
        (void)functor;
        (void)sender;
        if (*type == 2) call(SceneManager_closeCurrentPopup, {sceneManager(), 1});
    }

    // startColiseum $_28: the layer factory of the colosseum intro movie
    static uint32_t colosseumMovie(uint32_t functor) {
        (void)functor;
        return call(start_movie, {0, 0});
    }

    // startColiseum $_27 (bool): unless the request is retried, refresh the adventure data and re-enter the
    // adventure select scene
    static void reenterAdventureSelect(uint32_t functor, const uint8_t *arg) {
        (void)functor;
        (void)arg;
        if (call(APIManager_needsRetry, {call(APIManager_getInstance, {})})) return;
        Frame f;
        uint32_t none = newEmptyFunction(f);
        call(SceneAdventureSelect_callAdventureSelectAPI, {none});
        functionDestroy(none);
        uint32_t mgr = sceneManager();
        uint32_t factory = newFunction(f, VT_LayerFactory, {CreateSceneLayer_AdventureSelect});
        call(SceneManager_changeSceneLayer, {mgr, factory, 1, 1, 1});
        functionDestroy(factory);
        st8(sceneManager() + SM_FLAG_30A, 1);
    }
};

using SL = StartDeckEditDialogLambdas;
RW_FUNCTION(0x579140, "StartDeckEditDialog::openAbilityMaxPopup::$_33::operator()(bool)", SL::abilityMaxClosed);
RW_FUNCTION(0x5794D8, "StartDeckEditDialog::openAbilityMaxPopup::$_33::{lambda(bool)#1}::operator()", SL::openShop);
RW_FUNCTION(0x5795FE, "StartDeckEditDialog::openAbilityMaxPopup::$_33::{lambda(bool)#1}::{lambda()#1}::operator()", SL::nothing);
RW_FUNCTION(0x579780, "StartDeckEditDialog::openAbilityMaxPopup::$_31::operator()(bool)", SL::burstMaxDone);
RW_FUNCTION(0x57999E, "StartDeckEditDialog::openAbilityMaxPopup::$_31::{lambda(bool)#1}::operator()", SL::nothing);
RW_FUNCTION(0x579A3E, "StartDeckEditDialog::openAbilityMaxPopup::$_38::{lambda()#1}::operator()", SL::removePopup);
RW_FUNCTION(0x579B02, "StartDeckEditDialog::openAbilityMaxPopup::$_32::operator()", SL::buttonsEnabled);
RW_FUNCTION(0x579BA6, "StartDeckEditDialog::openAbilityMaxPopup::$_30::operator()", SL::tapCancel);
RW_FUNCTION(0x579C42, "StartDeckEditDialog::openAbilityMaxPopup::$_29::operator()", SL::tapOk);
RW_FUNCTION(0x579CEA, "StartDeckEditDialog::startColiseum::$_28::operator()", SL::colosseumMovie);
RW_FUNCTION(0x579D80, "StartDeckEditDialog::startColiseum::$_27::operator()(bool)", SL::reenterAdventureSelect);
