// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the SceneTitle lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneTitle::init $_7 UlvE1_ UlvE_: __func::__clone(__base*)
static void t_67EF00(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64078u);
    }
}
RW_FUNCTION(0x67EF00, "SceneTitle::init $_7 UlvE1_ UlvE_ __func::__clone(__base*)", t_67EF00);

// SceneTitle::init $_7 UlvE1_ UlvE_: __func::destroy
static void t_67EF1C(uint32_t self) {
}
RW_FUNCTION(0x67EF1C, "SceneTitle::init $_7 UlvE1_ UlvE_ __func::destroy", t_67EF1C);

// SceneTitle::init $_7 UlvE1_ UlvE_: __func::destroy_deallocate
static void t_67EF1E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67EF1E, "SceneTitle::init $_7 UlvE1_ UlvE_ __func::destroy_deallocate", t_67EF1E);

// SceneTitle::init $_7 UlvE1_ UlvE_: __func::target
static uint32_t t_67F178(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6e770u ? self + 4 : 0; }
RW_FUNCTION(0x67F178, "SceneTitle::init $_7 UlvE1_ UlvE_ __func::target", t_67F178);

// SceneTitle::init $_7 UlvE1_ UlvE_: __func::target_type
static uint32_t t_67F198() { return 0xf9bb44u; }
RW_FUNCTION(0x67F198, "SceneTitle::init $_7 UlvE1_ UlvE_ __func::target_type", t_67F198);

// SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_: __func::__clone(__base*)
static void t_67F8FC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf640a4u);
    }
}
RW_FUNCTION(0x67F8FC, "SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_ __func::__clone(__base*)", t_67F8FC);

// SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_: __func::destroy
static void t_67F918(uint32_t self) {
}
RW_FUNCTION(0x67F918, "SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_ __func::destroy", t_67F918);

// SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_: __func::destroy_deallocate
static void t_67F91A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67F91A, "SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_ __func::destroy_deallocate", t_67F91A);

// SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_: __func::target
static uint32_t t_67F92C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6e870u ? self + 4 : 0; }
RW_FUNCTION(0x67F92C, "SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_ __func::target", t_67F92C);

// SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_: __func::target_type
static uint32_t t_67F94C() { return 0xf9bb58u; }
RW_FUNCTION(0x67F94C, "SceneTitle::init $_7 UlvE1_ UlvE_ UlvE_ __func::target_type", t_67F94C);

// SceneTitle::init $_7 UlvE0_: __func::~__func (deleting)
static void t_67F9B4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67F9B4, "SceneTitle::init $_7 UlvE0_ __func::~__func (deleting)", t_67F9B4);

// SceneTitle::init $_7 UlvE0_: __func::__clone()
static uint32_t t_67F9BC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf640d0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x67F9BC, "SceneTitle::init $_7 UlvE0_ __func::__clone()", t_67F9BC);

// SceneTitle::init $_7 UlvE0_: __func::__clone(__base*)
static void t_67F9E8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf640d0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x67F9E8, "SceneTitle::init $_7 UlvE0_ __func::__clone(__base*)", t_67F9E8);

// SceneTitle::init $_7 UlvE0_: __func::destroy
static void t_67FA08(uint32_t self) {
}
RW_FUNCTION(0x67FA08, "SceneTitle::init $_7 UlvE0_ __func::destroy", t_67FA08);

// SceneTitle::init $_7 UlvE0_: __func::destroy_deallocate
static void t_67FA0A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FA0A, "SceneTitle::init $_7 UlvE0_ __func::destroy_deallocate", t_67FA0A);

// SceneTitle::init $_7 UlvE0_: __func::target
static uint32_t t_67FA30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6e950u ? self + 4 : 0; }
RW_FUNCTION(0x67FA30, "SceneTitle::init $_7 UlvE0_ __func::target", t_67FA30);

// SceneTitle::init $_7 UlvE0_: __func::target_type
static uint32_t t_67FA50() { return 0xf9bb6cu; }
RW_FUNCTION(0x67FA50, "SceneTitle::init $_7 UlvE0_ __func::target_type", t_67FA50);

// SceneTitle::init $_7 UlvE_: __func::~__func (deleting)
static void t_67FA64(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FA64, "SceneTitle::init $_7 UlvE_ __func::~__func (deleting)", t_67FA64);

// SceneTitle::init $_7 UlvE_: __func::__clone()
static uint32_t t_67FA6C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf640fcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x67FA6C, "SceneTitle::init $_7 UlvE_ __func::__clone()", t_67FA6C);

// SceneTitle::init $_7 UlvE_: __func::__clone(__base*)
static void t_67FA98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf640fcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x67FA98, "SceneTitle::init $_7 UlvE_ __func::__clone(__base*)", t_67FA98);

// SceneTitle::init $_7 UlvE_: __func::destroy
static void t_67FAB8(uint32_t self) {
}
RW_FUNCTION(0x67FAB8, "SceneTitle::init $_7 UlvE_ __func::destroy", t_67FAB8);

// SceneTitle::init $_7 UlvE_: __func::destroy_deallocate
static void t_67FABA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FABA, "SceneTitle::init $_7 UlvE_ __func::destroy_deallocate", t_67FABA);

// SceneTitle::init $_7 UlvE_: __func::target
static uint32_t t_67FAFC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6ea20u ? self + 4 : 0; }
RW_FUNCTION(0x67FAFC, "SceneTitle::init $_7 UlvE_ __func::target", t_67FAFC);

// SceneTitle::init $_7 UlvE_: __func::target_type
static uint32_t t_67FB1C() { return 0xf9bb80u; }
RW_FUNCTION(0x67FB1C, "SceneTitle::init $_7 UlvE_ __func::target_type", t_67FB1C);

// SceneTitle::init $_6: __func::~__func (deleting)
static void t_67FB30(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FB30, "SceneTitle::init $_6 __func::~__func (deleting)", t_67FB30);

// SceneTitle::init $_6: __func::__clone()
static uint32_t t_67FB38(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64128u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x67FB38, "SceneTitle::init $_6 __func::__clone()", t_67FB38);

// SceneTitle::init $_6: __func::__clone(__base*)
static void t_67FB64(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64128u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x67FB64, "SceneTitle::init $_6 __func::__clone(__base*)", t_67FB64);

// SceneTitle::init $_6: __func::destroy
static void t_67FB84(uint32_t self) {
}
RW_FUNCTION(0x67FB84, "SceneTitle::init $_6 __func::destroy", t_67FB84);

// SceneTitle::init $_6: __func::destroy_deallocate
static void t_67FB86(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FB86, "SceneTitle::init $_6 __func::destroy_deallocate", t_67FB86);

// SceneTitle::init $_6: __func::target
static uint32_t t_67FB94(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6eae0u ? self + 4 : 0; }
RW_FUNCTION(0x67FB94, "SceneTitle::init $_6 __func::target", t_67FB94);

// SceneTitle::init $_6: __func::target_type
static uint32_t t_67FBB4() { return 0xf9bb94u; }
RW_FUNCTION(0x67FBB4, "SceneTitle::init $_6 __func::target_type", t_67FBB4);

// SceneTitle::init $_5: __func::~__func (deleting)
static void t_67FBC8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FBC8, "SceneTitle::init $_5 __func::~__func (deleting)", t_67FBC8);

// SceneTitle::init $_5: __func::__clone()
static uint32_t t_67FBD0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64154u);
    }
    return v1;
}
RW_FUNCTION(0x67FBD0, "SceneTitle::init $_5 __func::__clone()", t_67FBD0);

// SceneTitle::init $_5: __func::__clone(__base*)
static void t_67FBF4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64154u);
    }
}
RW_FUNCTION(0x67FBF4, "SceneTitle::init $_5 __func::__clone(__base*)", t_67FBF4);

// SceneTitle::init $_5: __func::destroy
static void t_67FC10(uint32_t self) {
}
RW_FUNCTION(0x67FC10, "SceneTitle::init $_5 __func::destroy", t_67FC10);

// SceneTitle::init $_5: __func::destroy_deallocate
static void t_67FC12(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FC12, "SceneTitle::init $_5 __func::destroy_deallocate", t_67FC12);

// SceneTitle::init $_5: __func::target
static uint32_t t_67FD4C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6eb80u ? self + 4 : 0; }
RW_FUNCTION(0x67FD4C, "SceneTitle::init $_5 __func::target", t_67FD4C);

// SceneTitle::init $_5: __func::target_type
static uint32_t t_67FD6C() { return 0xf9bba8u; }
RW_FUNCTION(0x67FD6C, "SceneTitle::init $_5 __func::target_type", t_67FD6C);

// SceneTitle::init $_5 UlbE_: __func::~__func (deleting)
static void t_67FD80(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FD80, "SceneTitle::init $_5 UlbE_ __func::~__func (deleting)", t_67FD80);

// SceneTitle::init $_5 UlbE_: __func::__clone()
static uint32_t t_67FD88(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64180u);
    }
    return v1;
}
RW_FUNCTION(0x67FD88, "SceneTitle::init $_5 UlbE_ __func::__clone()", t_67FD88);

// SceneTitle::init $_5 UlbE_: __func::__clone(__base*)
static void t_67FDAC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64180u);
    }
}
RW_FUNCTION(0x67FDAC, "SceneTitle::init $_5 UlbE_ __func::__clone(__base*)", t_67FDAC);

// SceneTitle::init $_5 UlbE_: __func::destroy
static void t_67FDC8(uint32_t self) {
}
RW_FUNCTION(0x67FDC8, "SceneTitle::init $_5 UlbE_ __func::destroy", t_67FDC8);

// SceneTitle::init $_5 UlbE_: __func::destroy_deallocate
static void t_67FDCA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FDCA, "SceneTitle::init $_5 UlbE_ __func::destroy_deallocate", t_67FDCA);

// SceneTitle::init $_5 UlbE_: __func::target
static uint32_t t_67FEC0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6ec30u ? self + 4 : 0; }
RW_FUNCTION(0x67FEC0, "SceneTitle::init $_5 UlbE_ __func::target", t_67FEC0);

// SceneTitle::init $_5 UlbE_: __func::target_type
static uint32_t t_67FEE0() { return 0xf9bbbcu; }
RW_FUNCTION(0x67FEE0, "SceneTitle::init $_5 UlbE_ __func::target_type", t_67FEE0);

// SceneTitle::init $_5 UlbE_ UlvE_: __func::~__func (deleting)
static void t_67FEF4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FEF4, "SceneTitle::init $_5 UlbE_ UlvE_ __func::~__func (deleting)", t_67FEF4);

// SceneTitle::init $_5 UlbE_ UlvE_: __func::__clone()
static uint32_t t_67FEFC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf641acu);
    }
    return v1;
}
RW_FUNCTION(0x67FEFC, "SceneTitle::init $_5 UlbE_ UlvE_ __func::__clone()", t_67FEFC);

// SceneTitle::init $_5 UlbE_ UlvE_: __func::__clone(__base*)
static void t_67FF20(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf641acu);
    }
}
RW_FUNCTION(0x67FF20, "SceneTitle::init $_5 UlbE_ UlvE_ __func::__clone(__base*)", t_67FF20);

// SceneTitle::init $_5 UlbE_ UlvE_: __func::destroy
static void t_67FF3C(uint32_t self) {
}
RW_FUNCTION(0x67FF3C, "SceneTitle::init $_5 UlbE_ UlvE_ __func::destroy", t_67FF3C);

// SceneTitle::init $_5 UlbE_ UlvE_: __func::destroy_deallocate
static void t_67FF3E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67FF3E, "SceneTitle::init $_5 UlbE_ UlvE_ __func::destroy_deallocate", t_67FF3E);

// SceneTitle::init $_5 UlbE_ UlvE_: __func::target
static uint32_t t_67FFEC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6ed40u ? self + 4 : 0; }
RW_FUNCTION(0x67FFEC, "SceneTitle::init $_5 UlbE_ UlvE_ __func::target", t_67FFEC);

// SceneTitle::init $_5 UlbE_ UlvE_: __func::target_type
static uint32_t t_68000C() { return 0xf9bbd0u; }
RW_FUNCTION(0x68000C, "SceneTitle::init $_5 UlbE_ UlvE_ __func::target_type", t_68000C);

// SceneTitle::init $_5 UlbE_ UlvE_ UlvE_: __func::~__func (deleting)
static void t_680020(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680020, "SceneTitle::init $_5 UlbE_ UlvE_ UlvE_ __func::~__func (deleting)", t_680020);

// SceneTitle::init $_5 UlbE_ UlvE_ UlvE_: __func::__clone()
static uint32_t t_680028(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf641d8u);
    }
    return v1;
}
RW_FUNCTION(0x680028, "SceneTitle::init $_5 UlbE_ UlvE_ UlvE_ __func::__clone()", t_680028);

// SceneTitle::init $_5 UlbE_ UlvE_ UlvE_: __func::__clone(__base*)
static void t_68004C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf641d8u);
    }
}
RW_FUNCTION(0x68004C, "SceneTitle::init $_5 UlbE_ UlvE_ UlvE_ __func::__clone(__base*)", t_68004C);

// SceneTitle::init $_5 UlbE_ UlvE_ UlvE_: __func::destroy
static void t_680068(uint32_t self) {
}
RW_FUNCTION(0x680068, "SceneTitle::init $_5 UlbE_ UlvE_ UlvE_ __func::destroy", t_680068);

// SceneTitle::init $_5 UlbE_ UlvE_ UlvE_: __func::destroy_deallocate
static void t_68006A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x68006A, "SceneTitle::init $_5 UlbE_ UlvE_ UlvE_ __func::destroy_deallocate", t_68006A);

// SceneTitle::init $_5 UlbE_ UlvE_ UlvE_: __func::target
static uint32_t t_680090(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6ee60u ? self + 4 : 0; }
RW_FUNCTION(0x680090, "SceneTitle::init $_5 UlbE_ UlvE_ UlvE_ __func::target", t_680090);

// SceneTitle::init $_5 UlbE_ UlvE_ UlvE_: __func::target_type
static uint32_t t_6800B0() { return 0xf9bbe4u; }
RW_FUNCTION(0x6800B0, "SceneTitle::init $_5 UlbE_ UlvE_ UlvE_ __func::target_type", t_6800B0);

// SceneTitle::init $_4: __func::~__func (deleting)
static void t_6800C4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6800C4, "SceneTitle::init $_4 __func::~__func (deleting)", t_6800C4);

// SceneTitle::init $_4: __func::__clone()
static uint32_t t_6800CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64204u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6800CC, "SceneTitle::init $_4 __func::__clone()", t_6800CC);

// SceneTitle::init $_4: __func::__clone(__base*)
static void t_6800F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64204u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6800F8, "SceneTitle::init $_4 __func::__clone(__base*)", t_6800F8);

// SceneTitle::init $_4: __func::destroy
static void t_680118(uint32_t self) {
}
RW_FUNCTION(0x680118, "SceneTitle::init $_4 __func::destroy", t_680118);

// SceneTitle::init $_4: __func::destroy_deallocate
static void t_68011A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x68011A, "SceneTitle::init $_4 __func::destroy_deallocate", t_68011A);

// SceneTitle::init $_4: __func::target
static uint32_t t_680138(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6ef20u ? self + 4 : 0; }
RW_FUNCTION(0x680138, "SceneTitle::init $_4 __func::target", t_680138);

// SceneTitle::init $_4: __func::target_type
static uint32_t t_680158() { return 0xf9bbf8u; }
RW_FUNCTION(0x680158, "SceneTitle::init $_4 __func::target_type", t_680158);

// SceneTitle::init $_3: __func::~__func (deleting)
static void t_68016C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x68016C, "SceneTitle::init $_3 __func::~__func (deleting)", t_68016C);

// SceneTitle::init $_3: __func::__clone()
static uint32_t t_680174(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64230u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x680174, "SceneTitle::init $_3 __func::__clone()", t_680174);

// SceneTitle::init $_3: __func::__clone(__base*)
static void t_6801A0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64230u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6801A0, "SceneTitle::init $_3 __func::__clone(__base*)", t_6801A0);

// SceneTitle::init $_3: __func::destroy
static void t_6801C0(uint32_t self) {
}
RW_FUNCTION(0x6801C0, "SceneTitle::init $_3 __func::destroy", t_6801C0);

// SceneTitle::init $_3: __func::destroy_deallocate
static void t_6801C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6801C2, "SceneTitle::init $_3 __func::destroy_deallocate", t_6801C2);

// SceneTitle::init $_3: __func::target
static uint32_t t_6802C0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6ef90u ? self + 4 : 0; }
RW_FUNCTION(0x6802C0, "SceneTitle::init $_3 __func::target", t_6802C0);

// SceneTitle::init $_3: __func::target_type
static uint32_t t_6802E0() { return 0xf9bc0cu; }
RW_FUNCTION(0x6802E0, "SceneTitle::init $_3 __func::target_type", t_6802E0);

// SceneTitle::init $_3 UlvE_: __func::~__func (deleting)
static void t_6802F4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6802F4, "SceneTitle::init $_3 UlvE_ __func::~__func (deleting)", t_6802F4);

// SceneTitle::init $_3 UlvE_: __func::__clone()
static uint32_t t_6802FC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6425cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6802FC, "SceneTitle::init $_3 UlvE_ __func::__clone()", t_6802FC);

// SceneTitle::init $_3 UlvE_: __func::__clone(__base*)
static void t_680328(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6425cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x680328, "SceneTitle::init $_3 UlvE_ __func::__clone(__base*)", t_680328);

// SceneTitle::init $_3 UlvE_: __func::destroy
static void t_680348(uint32_t self) {
}
RW_FUNCTION(0x680348, "SceneTitle::init $_3 UlvE_ __func::destroy", t_680348);

// SceneTitle::init $_3 UlvE_: __func::destroy_deallocate
static void t_68034A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x68034A, "SceneTitle::init $_3 UlvE_ __func::destroy_deallocate", t_68034A);
