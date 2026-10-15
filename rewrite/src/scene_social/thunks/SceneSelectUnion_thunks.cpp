// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneSelectUnion lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneSelectUnion::backScene $_16: __func::~__func (deleting)
static void t_6CABB8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CABB8, "SceneSelectUnion::backScene $_16 __func::~__func (deleting)", t_6CABB8);

// SceneSelectUnion::backScene $_16: __func::__clone()
static uint32_t t_6CABC0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf672e0u);
    }
    return v1;
}
RW_FUNCTION(0x6CABC0, "SceneSelectUnion::backScene $_16 __func::__clone()", t_6CABC0);

// SceneSelectUnion::backScene $_16: __func::__clone(__base*)
static void t_6CABE4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf672e0u);
    }
}
RW_FUNCTION(0x6CABE4, "SceneSelectUnion::backScene $_16 __func::__clone(__base*)", t_6CABE4);

// SceneSelectUnion::backScene $_16: __func::destroy
static void t_6CAC00(uint32_t self) {
}
RW_FUNCTION(0x6CAC00, "SceneSelectUnion::backScene $_16 __func::destroy", t_6CAC00);

// SceneSelectUnion::backScene $_16: __func::destroy_deallocate
static void t_6CAC02(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CAC02, "SceneSelectUnion::backScene $_16 __func::destroy_deallocate", t_6CAC02);

// SceneSelectUnion::backScene $_16: __func::target
static uint32_t t_6CAD2C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d160u ? self + 4 : 0; }
RW_FUNCTION(0x6CAD2C, "SceneSelectUnion::backScene $_16 __func::target", t_6CAD2C);

// SceneSelectUnion::backScene $_16: __func::target_type
static uint32_t t_6CAD4C() { return 0xf9fbc8u; }
RW_FUNCTION(0x6CAD4C, "SceneSelectUnion::backScene $_16 __func::target_type", t_6CAD4C);

// SceneSelectUnion::openPopUpUnion $_15: __func::~__func (deleting)
static void t_6CAD60(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CAD60, "SceneSelectUnion::openPopUpUnion $_15 __func::~__func (deleting)", t_6CAD60);

// SceneSelectUnion::openPopUpUnion $_15: __func::__clone()
static uint32_t t_6CAD68(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6730cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6CAD68, "SceneSelectUnion::openPopUpUnion $_15 __func::__clone()", t_6CAD68);

// SceneSelectUnion::openPopUpUnion $_15: __func::__clone(__base*)
static void t_6CAD98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6730cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CAD98, "SceneSelectUnion::openPopUpUnion $_15 __func::__clone(__base*)", t_6CAD98);

// SceneSelectUnion::openPopUpUnion $_15: __func::destroy
static void t_6CADC0(uint32_t self) {
}
RW_FUNCTION(0x6CADC0, "SceneSelectUnion::openPopUpUnion $_15 __func::destroy", t_6CADC0);

// SceneSelectUnion::openPopUpUnion $_15: __func::destroy_deallocate
static void t_6CADC2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CADC2, "SceneSelectUnion::openPopUpUnion $_15 __func::destroy_deallocate", t_6CADC2);

// SceneSelectUnion::openPopUpUnion $_15: __func::target
static uint32_t t_6CAEB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d200u ? self + 4 : 0; }
RW_FUNCTION(0x6CAEB8, "SceneSelectUnion::openPopUpUnion $_15 __func::target", t_6CAEB8);

// SceneSelectUnion::openPopUpUnion $_15: __func::target_type
static uint32_t t_6CAED8() { return 0xf9fbdcu; }
RW_FUNCTION(0x6CAED8, "SceneSelectUnion::openPopUpUnion $_15 __func::target_type", t_6CAED8);

// SceneSelectUnion::openPopUpUnion $_17 UlvE_: __func::~__func (deleting)
static void t_6CAEEC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CAEEC, "SceneSelectUnion::openPopUpUnion $_17 UlvE_ __func::~__func (deleting)", t_6CAEEC);

// SceneSelectUnion::openPopUpUnion $_17 UlvE_: __func::__clone()
static uint32_t t_6CAEF4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf67338u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6CAEF4, "SceneSelectUnion::openPopUpUnion $_17 UlvE_ __func::__clone()", t_6CAEF4);

// SceneSelectUnion::openPopUpUnion $_17 UlvE_: __func::__clone(__base*)
static void t_6CAF24(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67338u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CAF24, "SceneSelectUnion::openPopUpUnion $_17 UlvE_ __func::__clone(__base*)", t_6CAF24);

// SceneSelectUnion::openPopUpUnion $_17 UlvE_: __func::destroy
static void t_6CAF4C(uint32_t self) {
}
RW_FUNCTION(0x6CAF4C, "SceneSelectUnion::openPopUpUnion $_17 UlvE_ __func::destroy", t_6CAF4C);

// SceneSelectUnion::openPopUpUnion $_17 UlvE_: __func::destroy_deallocate
static void t_6CAF4E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CAF4E, "SceneSelectUnion::openPopUpUnion $_17 UlvE_ __func::destroy_deallocate", t_6CAF4E);

// SceneSelectUnion::openPopUpUnion $_17 UlvE_: __func::target
static uint32_t t_6CAF80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d2a0u ? self + 4 : 0; }
RW_FUNCTION(0x6CAF80, "SceneSelectUnion::openPopUpUnion $_17 UlvE_ __func::target", t_6CAF80);

// SceneSelectUnion::openPopUpUnion $_17 UlvE_: __func::target_type
static uint32_t t_6CAFA0() { return 0xf9fbf0u; }
RW_FUNCTION(0x6CAFA0, "SceneSelectUnion::openPopUpUnion $_17 UlvE_ __func::target_type", t_6CAFA0);

// SceneSelectUnion::openPopUpUnion $_14: __func::~__func (deleting)
static void t_6CAFB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CAFB4, "SceneSelectUnion::openPopUpUnion $_14 __func::~__func (deleting)", t_6CAFB4);

// SceneSelectUnion::openPopUpUnion $_14: __func::__clone()
static uint32_t t_6CAFBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67364u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CAFBC, "SceneSelectUnion::openPopUpUnion $_14 __func::__clone()", t_6CAFBC);

// SceneSelectUnion::openPopUpUnion $_14: __func::__clone(__base*)
static void t_6CAFE8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67364u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CAFE8, "SceneSelectUnion::openPopUpUnion $_14 __func::__clone(__base*)", t_6CAFE8);

// SceneSelectUnion::openPopUpUnion $_14: __func::destroy
static void t_6CB008(uint32_t self) {
}
RW_FUNCTION(0x6CB008, "SceneSelectUnion::openPopUpUnion $_14 __func::destroy", t_6CB008);

// SceneSelectUnion::openPopUpUnion $_14: __func::destroy_deallocate
static void t_6CB00A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB00A, "SceneSelectUnion::openPopUpUnion $_14 __func::destroy_deallocate", t_6CB00A);

// SceneSelectUnion::openPopUpUnion $_14: __func::target
static uint32_t t_6CB01C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d350u ? self + 4 : 0; }
RW_FUNCTION(0x6CB01C, "SceneSelectUnion::openPopUpUnion $_14 __func::target", t_6CB01C);

// SceneSelectUnion::openPopUpUnion $_14: __func::target_type
static uint32_t t_6CB03C() { return 0xf9fc04u; }
RW_FUNCTION(0x6CB03C, "SceneSelectUnion::openPopUpUnion $_14 __func::target_type", t_6CB03C);

// SceneSelectUnion::openPopUpUnion $_13: __func::~__func (deleting)
static void t_6CB050(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB050, "SceneSelectUnion::openPopUpUnion $_13 __func::~__func (deleting)", t_6CB050);

// SceneSelectUnion::openPopUpUnion $_13: __func::__clone()
static uint32_t t_6CB058(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf67390u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CB058, "SceneSelectUnion::openPopUpUnion $_13 __func::__clone()", t_6CB058);

// SceneSelectUnion::openPopUpUnion $_13: __func::__clone(__base*)
static void t_6CB088(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67390u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CB088, "SceneSelectUnion::openPopUpUnion $_13 __func::__clone(__base*)", t_6CB088);

// SceneSelectUnion::openPopUpUnion $_13: __func::destroy
static void t_6CB0AC(uint32_t self) {
}
RW_FUNCTION(0x6CB0AC, "SceneSelectUnion::openPopUpUnion $_13 __func::destroy", t_6CB0AC);

// SceneSelectUnion::openPopUpUnion $_13: __func::destroy_deallocate
static void t_6CB0AE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB0AE, "SceneSelectUnion::openPopUpUnion $_13 __func::destroy_deallocate", t_6CB0AE);

// SceneSelectUnion::openPopUpUnion $_13: __func::target
static uint32_t t_6CB0F0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d410u ? self + 4 : 0; }
RW_FUNCTION(0x6CB0F0, "SceneSelectUnion::openPopUpUnion $_13 __func::target", t_6CB0F0);

// SceneSelectUnion::openPopUpUnion $_13: __func::target_type
static uint32_t t_6CB110() { return 0xf9fc18u; }
RW_FUNCTION(0x6CB110, "SceneSelectUnion::openPopUpUnion $_13 __func::target_type", t_6CB110);

// SceneSelectUnion::openPopUpUnion $_12: __func::~__func (deleting)
static void t_6CB124(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB124, "SceneSelectUnion::openPopUpUnion $_12 __func::~__func (deleting)", t_6CB124);

// SceneSelectUnion::openPopUpUnion $_12: __func::__clone()
static uint32_t t_6CB12C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf673bcu);
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
RW_FUNCTION(0x6CB12C, "SceneSelectUnion::openPopUpUnion $_12 __func::__clone()", t_6CB12C);

// SceneSelectUnion::openPopUpUnion $_12: __func::__clone(__base*)
static void t_6CB160(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf673bcu);
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
RW_FUNCTION(0x6CB160, "SceneSelectUnion::openPopUpUnion $_12 __func::__clone(__base*)", t_6CB160);

// SceneSelectUnion::openPopUpUnion $_12: __func::destroy
static void t_6CB18C(uint32_t self) {
}
RW_FUNCTION(0x6CB18C, "SceneSelectUnion::openPopUpUnion $_12 __func::destroy", t_6CB18C);

// SceneSelectUnion::openPopUpUnion $_12: __func::destroy_deallocate
static void t_6CB18E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB18E, "SceneSelectUnion::openPopUpUnion $_12 __func::destroy_deallocate", t_6CB18E);

// SceneSelectUnion::openPopUpUnion $_12: __func::target
static uint32_t t_6CB450(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d4d0u ? self + 4 : 0; }
RW_FUNCTION(0x6CB450, "SceneSelectUnion::openPopUpUnion $_12 __func::target", t_6CB450);

// SceneSelectUnion::openPopUpUnion $_12: __func::target_type
static uint32_t t_6CB470() { return 0xf9fc2cu; }
RW_FUNCTION(0x6CB470, "SceneSelectUnion::openPopUpUnion $_12 __func::target_type", t_6CB470);

// SceneSelectUnion::openPopUpUnion $_12 UlbE0_: __func::~__func (deleting)
static void t_6CB484(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB484, "SceneSelectUnion::openPopUpUnion $_12 UlbE0_ __func::~__func (deleting)", t_6CB484);

// SceneSelectUnion::openPopUpUnion $_12 UlbE0_: __func::__clone()
static uint32_t t_6CB48C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf673e8u);
    }
    return v1;
}
RW_FUNCTION(0x6CB48C, "SceneSelectUnion::openPopUpUnion $_12 UlbE0_ __func::__clone()", t_6CB48C);

// SceneSelectUnion::openPopUpUnion $_12 UlbE0_: __func::__clone(__base*)
static void t_6CB4B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf673e8u);
    }
}
RW_FUNCTION(0x6CB4B0, "SceneSelectUnion::openPopUpUnion $_12 UlbE0_ __func::__clone(__base*)", t_6CB4B0);

// SceneSelectUnion::openPopUpUnion $_12 UlbE0_: __func::destroy
static void t_6CB4CC(uint32_t self) {
}
RW_FUNCTION(0x6CB4CC, "SceneSelectUnion::openPopUpUnion $_12 UlbE0_ __func::destroy", t_6CB4CC);

// SceneSelectUnion::openPopUpUnion $_12 UlbE0_: __func::destroy_deallocate
static void t_6CB4CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB4CE, "SceneSelectUnion::openPopUpUnion $_12 UlbE0_ __func::destroy_deallocate", t_6CB4CE);

// SceneSelectUnion::openPopUpUnion $_12 UlbE0_: __func::target
static uint32_t t_6CB564(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d5a0u ? self + 4 : 0; }
RW_FUNCTION(0x6CB564, "SceneSelectUnion::openPopUpUnion $_12 UlbE0_ __func::target", t_6CB564);

// SceneSelectUnion::openPopUpUnion $_12 UlbE0_: __func::target_type
static uint32_t t_6CB584() { return 0xf9fc40u; }
RW_FUNCTION(0x6CB584, "SceneSelectUnion::openPopUpUnion $_12 UlbE0_ __func::target_type", t_6CB584);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_: __func::~__func (deleting)
static void t_6CB598(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB598, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ __func::~__func (deleting)", t_6CB598);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_: __func::__clone()
static uint32_t t_6CB5A0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67414u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CB5A0, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ __func::__clone()", t_6CB5A0);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_: __func::__clone(__base*)
static void t_6CB5CC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67414u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CB5CC, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ __func::__clone(__base*)", t_6CB5CC);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_: __func::destroy
static void t_6CB5EC(uint32_t self) {
}
RW_FUNCTION(0x6CB5EC, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ __func::destroy", t_6CB5EC);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_: __func::destroy_deallocate
static void t_6CB5EE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB5EE, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ __func::destroy_deallocate", t_6CB5EE);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_: __func::target
static uint32_t t_6CB770(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d6b0u ? self + 4 : 0; }
RW_FUNCTION(0x6CB770, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ __func::target", t_6CB770);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_: __func::target_type
static uint32_t t_6CB790() { return 0xf9fc54u; }
RW_FUNCTION(0x6CB790, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ __func::target_type", t_6CB790);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_: __func::~__func (deleting)
static void t_6CB7A4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB7A4, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ __func::~__func (deleting)", t_6CB7A4);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_: __func::__clone()
static uint32_t t_6CB7AC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67440u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CB7AC, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ __func::__clone()", t_6CB7AC);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_: __func::__clone(__base*)
static void t_6CB7D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67440u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CB7D8, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ __func::__clone(__base*)", t_6CB7D8);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_: __func::destroy
static void t_6CB7F8(uint32_t self) {
}
RW_FUNCTION(0x6CB7F8, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ __func::destroy", t_6CB7F8);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_: __func::destroy_deallocate
static void t_6CB7FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB7FA, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ __func::destroy_deallocate", t_6CB7FA);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_: __func::target
static uint32_t t_6CB894(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d7e0u ? self + 4 : 0; }
RW_FUNCTION(0x6CB894, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ __func::target", t_6CB894);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_: __func::target_type
static uint32_t t_6CB8B4() { return 0xf9fc68u; }
RW_FUNCTION(0x6CB8B4, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ __func::target_type", t_6CB8B4);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_: __func::~__func (deleting)
static void t_6CB8C8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB8C8, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_ __func::~__func (deleting)", t_6CB8C8);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_: __func::__clone()
static uint32_t t_6CB8D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6746cu);
    }
    return v1;
}
RW_FUNCTION(0x6CB8D0, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_ __func::__clone()", t_6CB8D0);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_: __func::__clone(__base*)
static void t_6CB8F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6746cu);
    }
}
RW_FUNCTION(0x6CB8F4, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_ __func::__clone(__base*)", t_6CB8F4);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_: __func::destroy
static void t_6CB910(uint32_t self) {
}
RW_FUNCTION(0x6CB910, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_ __func::destroy", t_6CB910);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_: __func::destroy_deallocate
static void t_6CB912(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CB912, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_ __func::destroy_deallocate", t_6CB912);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_: __func::target
static uint32_t t_6CB9FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7d940u ? self + 4 : 0; }
RW_FUNCTION(0x6CB9FC, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_ __func::target", t_6CB9FC);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_: __func::target_type
static uint32_t t_6CBA1C() { return 0xf9fc7cu; }
RW_FUNCTION(0x6CBA1C, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlvE_ UlbE_ UlvE_ __func::target_type", t_6CBA1C);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_: __func::~__func (deleting)
static void t_6CBA30(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CBA30, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ __func::~__func (deleting)", t_6CBA30);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_: __func::__clone()
static uint32_t t_6CBA38(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67498u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CBA38, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ __func::__clone()", t_6CBA38);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_: __func::__clone(__base*)
static void t_6CBA64(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67498u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CBA64, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ __func::__clone(__base*)", t_6CBA64);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_: __func::destroy
static void t_6CBA84(uint32_t self) {
}
RW_FUNCTION(0x6CBA84, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ __func::destroy", t_6CBA84);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_: __func::destroy_deallocate
static void t_6CBA86(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CBA86, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ __func::destroy_deallocate", t_6CBA86);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_: __func::target
static uint32_t t_6CBB38(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7da80u ? self + 4 : 0; }
RW_FUNCTION(0x6CBB38, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ __func::target", t_6CBB38);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_: __func::target_type
static uint32_t t_6CBB58() { return 0xf9fc90u; }
RW_FUNCTION(0x6CBB58, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ __func::target_type", t_6CBB58);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_: __func::~__func (deleting)
static void t_6CBB6C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CBB6C, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_ __func::~__func (deleting)", t_6CBB6C);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_: __func::__clone()
static uint32_t t_6CBB74(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf674c4u);
    }
    return v1;
}
RW_FUNCTION(0x6CBB74, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_ __func::__clone()", t_6CBB74);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_: __func::__clone(__base*)
static void t_6CBB98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf674c4u);
    }
}
RW_FUNCTION(0x6CBB98, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_ __func::__clone(__base*)", t_6CBB98);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_: __func::destroy
static void t_6CBBB4(uint32_t self) {
}
RW_FUNCTION(0x6CBBB4, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_ __func::destroy", t_6CBBB4);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_: __func::destroy_deallocate
static void t_6CBBB6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CBBB6, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_ __func::destroy_deallocate", t_6CBBB6);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_: __func::target
static uint32_t t_6CBCA0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7dbc0u ? self + 4 : 0; }
RW_FUNCTION(0x6CBCA0, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_ __func::target", t_6CBCA0);

// SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_: __func::target_type
static uint32_t t_6CBCC0() { return 0xf9fca4u; }
RW_FUNCTION(0x6CBCC0, "SceneSelectUnion::openPopUpUnion $_12 UlbE_ UlbE_ UlvE_ __func::target_type", t_6CBCC0);

// SceneSelectUnion::openPopUpDescriptionUnion $_11: __func::~__func
static uint32_t t_6CBCD4(uint32_t self) {
    st32(self, 0xf674f0u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x6CBCD4, "SceneSelectUnion::openPopUpDescriptionUnion $_11 __func::~__func", t_6CBCD4);

// SceneSelectUnion::openPopUpDescriptionUnion $_11: __func::~__func (deleting)
static void t_6CBD10(uint32_t self) {
    st32(self, 0xf674f0u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CBD10, "SceneSelectUnion::openPopUpDescriptionUnion $_11 __func::~__func (deleting)", t_6CBD10);

// SceneSelectUnion::openPopUpDescriptionUnion $_11: __func::__clone()
static uint32_t t_6CBD50(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf674f0u);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x45c84d, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v3 = ld32(self + 40);
        uint32_t v4 = ld32(self + 44);
        st32(v1 + 40, v3);
        st32(v1 + 44, v4);
    }
    return v1;
}
RW_FUNCTION(0x6CBD50, "SceneSelectUnion::openPopUpDescriptionUnion $_11 __func::__clone()", t_6CBD50);

// SceneSelectUnion::openPopUpDescriptionUnion $_11: __func::__clone(__base*)
static void t_6CBDAC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf674f0u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x45c84d, {p + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 40);
        uint32_t v3 = ld32(self + 44);
        st32(p + 40, v2);
        st32(p + 44, v3);
    }
}
RW_FUNCTION(0x6CBDAC, "SceneSelectUnion::openPopUpDescriptionUnion $_11 __func::__clone(__base*)", t_6CBDAC);

// SceneSelectUnion::openPopUpDescriptionUnion $_11: __func::destroy
static void t_6CBDF0(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x6CBDF0, "SceneSelectUnion::openPopUpDescriptionUnion $_11 __func::destroy", t_6CBDF0);

// SceneSelectUnion::openPopUpDescriptionUnion $_11: __func::destroy_deallocate
static void t_6CBE10(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CBE10, "SceneSelectUnion::openPopUpDescriptionUnion $_11 __func::destroy_deallocate", t_6CBE10);

// SceneSelectUnion::openPopUpDescriptionUnion $_11: __func::target
static uint32_t t_6CBE64(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7dcd0u ? self + 8 : 0; }
RW_FUNCTION(0x6CBE64, "SceneSelectUnion::openPopUpDescriptionUnion $_11 __func::target", t_6CBE64);

// SceneSelectUnion::openPopUpDescriptionUnion $_11: __func::target_type
static uint32_t t_6CBE88() { return 0xf9fcb8u; }
RW_FUNCTION(0x6CBE88, "SceneSelectUnion::openPopUpDescriptionUnion $_11 __func::target_type", t_6CBE88);

// SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_: __func::~__func
static uint32_t t_6CC010(uint32_t self) {
    st32(self, 0xf6751cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6CC010, "SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_ __func::~__func", t_6CC010);

// SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_: __func::~__func (deleting)
static void t_6CC04C(uint32_t self) {
    st32(self, 0xf6751cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC04C, "SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_ __func::~__func (deleting)", t_6CC04C);

// SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_: __func::destroy
static void t_6CC13C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6CC13C, "SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_ __func::destroy", t_6CC13C);

// SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_: __func::destroy_deallocate
static void t_6CC15C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC15C, "SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_ __func::destroy_deallocate", t_6CC15C);

// SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_: __func::target
static uint32_t t_6CC1C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7ddb0u ? self + 8 : 0; }
RW_FUNCTION(0x6CC1C8, "SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_ __func::target", t_6CC1C8);

// SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_: __func::target_type
static uint32_t t_6CC1E8() { return 0xf9fcccu; }
RW_FUNCTION(0x6CC1E8, "SceneSelectUnion::openPopUpDescriptionUnion $_9 UlvE_ __func::target_type", t_6CC1E8);

// SceneSelectUnion::openPopUpDescriptionUnion $_10: __func::~__func (deleting)
static void t_6CC1FC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC1FC, "SceneSelectUnion::openPopUpDescriptionUnion $_10 __func::~__func (deleting)", t_6CC1FC);

// SceneSelectUnion::openPopUpDescriptionUnion $_10: __func::__clone()
static uint32_t t_6CC204(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67548u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CC204, "SceneSelectUnion::openPopUpDescriptionUnion $_10 __func::__clone()", t_6CC204);

// SceneSelectUnion::openPopUpDescriptionUnion $_10: __func::__clone(__base*)
static void t_6CC230(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67548u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CC230, "SceneSelectUnion::openPopUpDescriptionUnion $_10 __func::__clone(__base*)", t_6CC230);

// SceneSelectUnion::openPopUpDescriptionUnion $_10: __func::destroy
static void t_6CC250(uint32_t self) {
}
RW_FUNCTION(0x6CC250, "SceneSelectUnion::openPopUpDescriptionUnion $_10 __func::destroy", t_6CC250);

// SceneSelectUnion::openPopUpDescriptionUnion $_10: __func::destroy_deallocate
static void t_6CC252(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC252, "SceneSelectUnion::openPopUpDescriptionUnion $_10 __func::destroy_deallocate", t_6CC252);

// SceneSelectUnion::openPopUpDescriptionUnion $_10: __func::target
static uint32_t t_6CC264(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7de90u ? self + 4 : 0; }
RW_FUNCTION(0x6CC264, "SceneSelectUnion::openPopUpDescriptionUnion $_10 __func::target", t_6CC264);

// SceneSelectUnion::openPopUpDescriptionUnion $_10: __func::target_type
static uint32_t t_6CC284() { return 0xf9fce0u; }
RW_FUNCTION(0x6CC284, "SceneSelectUnion::openPopUpDescriptionUnion $_10 __func::target_type", t_6CC284);

// SceneSelectUnion::openPopUpDescriptionUnion $_8: __func::~__func (deleting)
static void t_6CC298(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC298, "SceneSelectUnion::openPopUpDescriptionUnion $_8 __func::~__func (deleting)", t_6CC298);

// SceneSelectUnion::openPopUpDescriptionUnion $_8: __func::__clone()
static uint32_t t_6CC2A0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67574u);
    }
    return v1;
}
RW_FUNCTION(0x6CC2A0, "SceneSelectUnion::openPopUpDescriptionUnion $_8 __func::__clone()", t_6CC2A0);

// SceneSelectUnion::openPopUpDescriptionUnion $_8: __func::__clone(__base*)
static void t_6CC2C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67574u);
    }
}
RW_FUNCTION(0x6CC2C4, "SceneSelectUnion::openPopUpDescriptionUnion $_8 __func::__clone(__base*)", t_6CC2C4);

// SceneSelectUnion::openPopUpDescriptionUnion $_8: __func::destroy
static void t_6CC2E0(uint32_t self) {
}
RW_FUNCTION(0x6CC2E0, "SceneSelectUnion::openPopUpDescriptionUnion $_8 __func::destroy", t_6CC2E0);

// SceneSelectUnion::openPopUpDescriptionUnion $_8: __func::destroy_deallocate
static void t_6CC2E2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC2E2, "SceneSelectUnion::openPopUpDescriptionUnion $_8 __func::destroy_deallocate", t_6CC2E2);

// SceneSelectUnion::openPopUpDescriptionUnion $_8: __func::target
static uint32_t t_6CC300(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7df90u ? self + 4 : 0; }
RW_FUNCTION(0x6CC300, "SceneSelectUnion::openPopUpDescriptionUnion $_8 __func::target", t_6CC300);

// SceneSelectUnion::openPopUpDescriptionUnion $_8: __func::target_type
static uint32_t t_6CC320() { return 0xf9fcf4u; }
RW_FUNCTION(0x6CC320, "SceneSelectUnion::openPopUpDescriptionUnion $_8 __func::target_type", t_6CC320);

// SceneSelectUnion::openPopUpDescriptionUnion $_7: __func::~__func (deleting)
static void t_6CC334(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC334, "SceneSelectUnion::openPopUpDescriptionUnion $_7 __func::~__func (deleting)", t_6CC334);

// SceneSelectUnion::openPopUpDescriptionUnion $_7: __func::__clone()
static uint32_t t_6CC33C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf675a0u);
    }
    return v1;
}
RW_FUNCTION(0x6CC33C, "SceneSelectUnion::openPopUpDescriptionUnion $_7 __func::__clone()", t_6CC33C);

// SceneSelectUnion::openPopUpDescriptionUnion $_7: __func::__clone(__base*)
static void t_6CC360(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf675a0u);
    }
}
RW_FUNCTION(0x6CC360, "SceneSelectUnion::openPopUpDescriptionUnion $_7 __func::__clone(__base*)", t_6CC360);

// SceneSelectUnion::openPopUpDescriptionUnion $_7: __func::destroy
static void t_6CC37C(uint32_t self) {
}
RW_FUNCTION(0x6CC37C, "SceneSelectUnion::openPopUpDescriptionUnion $_7 __func::destroy", t_6CC37C);

// SceneSelectUnion::openPopUpDescriptionUnion $_7: __func::destroy_deallocate
static void t_6CC37E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC37E, "SceneSelectUnion::openPopUpDescriptionUnion $_7 __func::destroy_deallocate", t_6CC37E);

// SceneSelectUnion::openPopUpDescriptionUnion $_7: __func::target
static uint32_t t_6CC39C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e090u ? self + 4 : 0; }
RW_FUNCTION(0x6CC39C, "SceneSelectUnion::openPopUpDescriptionUnion $_7 __func::target", t_6CC39C);

// SceneSelectUnion::openPopUpDescriptionUnion $_7: __func::target_type
static uint32_t t_6CC3BC() { return 0xf9fd08u; }
RW_FUNCTION(0x6CC3BC, "SceneSelectUnion::openPopUpDescriptionUnion $_7 __func::target_type", t_6CC3BC);

// SceneSelectUnion::runOutAnimation $_6: __func::~__func
static uint32_t t_6CC3D0(uint32_t self) {
    st32(self, 0xf675ccu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6CC3D0, "SceneSelectUnion::runOutAnimation $_6 __func::~__func", t_6CC3D0);

// SceneSelectUnion::runOutAnimation $_6: __func::~__func (deleting)
static void t_6CC40C(uint32_t self) {
    st32(self, 0xf675ccu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC40C, "SceneSelectUnion::runOutAnimation $_6 __func::~__func (deleting)", t_6CC40C);

// SceneSelectUnion::runOutAnimation $_6: __func::__clone()
static uint32_t t_6CC44C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf675ccu);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6CC44C, "SceneSelectUnion::runOutAnimation $_6 __func::__clone()", t_6CC44C);

// SceneSelectUnion::runOutAnimation $_6: __func::__clone(__base*)
static void t_6CC49C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf675ccu);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x6CC49C, "SceneSelectUnion::runOutAnimation $_6 __func::__clone(__base*)", t_6CC49C);

// SceneSelectUnion::runOutAnimation $_6: __func::destroy
static void t_6CC4D0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6CC4D0, "SceneSelectUnion::runOutAnimation $_6 __func::destroy", t_6CC4D0);

// SceneSelectUnion::runOutAnimation $_6: __func::destroy_deallocate
static void t_6CC4F0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC4F0, "SceneSelectUnion::runOutAnimation $_6 __func::destroy_deallocate", t_6CC4F0);

// SceneSelectUnion::runOutAnimation $_6: __func::target
static uint32_t t_6CC528(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e160u ? self + 8 : 0; }
RW_FUNCTION(0x6CC528, "SceneSelectUnion::runOutAnimation $_6 __func::target", t_6CC528);

// SceneSelectUnion::runOutAnimation $_6: __func::target_type
static uint32_t t_6CC548() { return 0xf9fd1cu; }
RW_FUNCTION(0x6CC548, "SceneSelectUnion::runOutAnimation $_6 __func::target_type", t_6CC548);

// SceneSelectUnion::init $_5: __func::~__func (deleting)
static void t_6CC55C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC55C, "SceneSelectUnion::init $_5 __func::~__func (deleting)", t_6CC55C);

// SceneSelectUnion::init $_5: __func::__clone()
static uint32_t t_6CC564(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf675f8u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6CC564, "SceneSelectUnion::init $_5 __func::__clone()", t_6CC564);

// SceneSelectUnion::init $_5: __func::__clone(__base*)
static void t_6CC594(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf675f8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CC594, "SceneSelectUnion::init $_5 __func::__clone(__base*)", t_6CC594);

// SceneSelectUnion::init $_5: __func::destroy
static void t_6CC5BC(uint32_t self) {
}
RW_FUNCTION(0x6CC5BC, "SceneSelectUnion::init $_5 __func::destroy", t_6CC5BC);

// SceneSelectUnion::init $_5: __func::destroy_deallocate
static void t_6CC5BE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC5BE, "SceneSelectUnion::init $_5 __func::destroy_deallocate", t_6CC5BE);

// SceneSelectUnion::init $_5: __func::target
static uint32_t t_6CC6DC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e230u ? self + 4 : 0; }
RW_FUNCTION(0x6CC6DC, "SceneSelectUnion::init $_5 __func::target", t_6CC6DC);

// SceneSelectUnion::init $_5: __func::target_type
static uint32_t t_6CC6FC() { return 0xf9fd30u; }
RW_FUNCTION(0x6CC6FC, "SceneSelectUnion::init $_5 __func::target_type", t_6CC6FC);

// SceneSelectUnion::init $_5 UlvE_: __func::~__func (deleting)
static void t_6CC710(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC710, "SceneSelectUnion::init $_5 UlvE_ __func::~__func (deleting)", t_6CC710);

// SceneSelectUnion::init $_5 UlvE_: __func::__clone()
static uint32_t t_6CC718(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67624u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CC718, "SceneSelectUnion::init $_5 UlvE_ __func::__clone()", t_6CC718);

// SceneSelectUnion::init $_5 UlvE_: __func::__clone(__base*)
static void t_6CC744(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67624u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CC744, "SceneSelectUnion::init $_5 UlvE_ __func::__clone(__base*)", t_6CC744);

// SceneSelectUnion::init $_5 UlvE_: __func::destroy
static void t_6CC764(uint32_t self) {
}
RW_FUNCTION(0x6CC764, "SceneSelectUnion::init $_5 UlvE_ __func::destroy", t_6CC764);

// SceneSelectUnion::init $_5 UlvE_: __func::destroy_deallocate
static void t_6CC766(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC766, "SceneSelectUnion::init $_5 UlvE_ __func::destroy_deallocate", t_6CC766);

// SceneSelectUnion::init $_5 UlvE_: __func::target
static uint32_t t_6CC778(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e2e0u ? self + 4 : 0; }
RW_FUNCTION(0x6CC778, "SceneSelectUnion::init $_5 UlvE_ __func::target", t_6CC778);

// SceneSelectUnion::init $_5 UlvE_: __func::target_type
static uint32_t t_6CC798() { return 0xf9fd44u; }
RW_FUNCTION(0x6CC798, "SceneSelectUnion::init $_5 UlvE_ __func::target_type", t_6CC798);

// SceneSelectUnion::init $_4: __func::~__func (deleting)
static void t_6CC7AC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC7AC, "SceneSelectUnion::init $_4 __func::~__func (deleting)", t_6CC7AC);

// SceneSelectUnion::init $_4: __func::__clone()
static uint32_t t_6CC7B4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf67650u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6CC7B4, "SceneSelectUnion::init $_4 __func::__clone()", t_6CC7B4);

// SceneSelectUnion::init $_4: __func::__clone(__base*)
static void t_6CC7E4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67650u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CC7E4, "SceneSelectUnion::init $_4 __func::__clone(__base*)", t_6CC7E4);

// SceneSelectUnion::init $_4: __func::destroy
static void t_6CC80C(uint32_t self) {
}
RW_FUNCTION(0x6CC80C, "SceneSelectUnion::init $_4 __func::destroy", t_6CC80C);

// SceneSelectUnion::init $_4: __func::destroy_deallocate
static void t_6CC80E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC80E, "SceneSelectUnion::init $_4 __func::destroy_deallocate", t_6CC80E);

// SceneSelectUnion::init $_4: __func::target
static uint32_t t_6CC8F8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e3b0u ? self + 4 : 0; }
RW_FUNCTION(0x6CC8F8, "SceneSelectUnion::init $_4 __func::target", t_6CC8F8);

// SceneSelectUnion::init $_4: __func::target_type
static uint32_t t_6CC918() { return 0xf9fd58u; }
RW_FUNCTION(0x6CC918, "SceneSelectUnion::init $_4 __func::target_type", t_6CC918);

// SceneSelectUnion::init $_3: __func::~__func (deleting)
static void t_6CC92C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC92C, "SceneSelectUnion::init $_3 __func::~__func (deleting)", t_6CC92C);

// SceneSelectUnion::init $_3: __func::__clone()
static uint32_t t_6CC934(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf6767cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6CC934, "SceneSelectUnion::init $_3 __func::__clone()", t_6CC934);

// SceneSelectUnion::init $_3: __func::__clone(__base*)
static void t_6CC968(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6767cu);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6CC968, "SceneSelectUnion::init $_3 __func::__clone(__base*)", t_6CC968);

// SceneSelectUnion::init $_3: __func::destroy
static void t_6CC990(uint32_t self) {
}
RW_FUNCTION(0x6CC990, "SceneSelectUnion::init $_3 __func::destroy", t_6CC990);

// SceneSelectUnion::init $_3: __func::destroy_deallocate
static void t_6CC992(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CC992, "SceneSelectUnion::init $_3 __func::destroy_deallocate", t_6CC992);

// SceneSelectUnion::init $_3: __func::target
static uint32_t t_6CCA94(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e460u ? self + 4 : 0; }
RW_FUNCTION(0x6CCA94, "SceneSelectUnion::init $_3 __func::target", t_6CCA94);

// SceneSelectUnion::init $_3: __func::target_type
static uint32_t t_6CCAB4() { return 0xf9fd6cu; }
RW_FUNCTION(0x6CCAB4, "SceneSelectUnion::init $_3 __func::target_type", t_6CCAB4);

// SceneSelectUnion::init $_2: __func::~__func (deleting)
static void t_6CCAC8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CCAC8, "SceneSelectUnion::init $_2 __func::~__func (deleting)", t_6CCAC8);

// SceneSelectUnion::init $_2: __func::__clone()
static uint32_t t_6CCAD0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf676a8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CCAD0, "SceneSelectUnion::init $_2 __func::__clone()", t_6CCAD0);

// SceneSelectUnion::init $_2: __func::__clone(__base*)
static void t_6CCAFC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf676a8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CCAFC, "SceneSelectUnion::init $_2 __func::__clone(__base*)", t_6CCAFC);

// SceneSelectUnion::init $_2: __func::destroy
static void t_6CCB1C(uint32_t self) {
}
RW_FUNCTION(0x6CCB1C, "SceneSelectUnion::init $_2 __func::destroy", t_6CCB1C);

// SceneSelectUnion::init $_2: __func::destroy_deallocate
static void t_6CCB1E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CCB1E, "SceneSelectUnion::init $_2 __func::destroy_deallocate", t_6CCB1E);

// SceneSelectUnion::init $_2: __func::target
static uint32_t t_6CCB30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e4f0u ? self + 4 : 0; }
RW_FUNCTION(0x6CCB30, "SceneSelectUnion::init $_2 __func::target", t_6CCB30);

// SceneSelectUnion::init $_2: __func::target_type
static uint32_t t_6CCB50() { return 0xf9fd80u; }
RW_FUNCTION(0x6CCB50, "SceneSelectUnion::init $_2 __func::target_type", t_6CCB50);

// SceneSelectUnion::init $_1: __func::~__func (deleting)
static void t_6CCB64(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CCB64, "SceneSelectUnion::init $_1 __func::~__func (deleting)", t_6CCB64);

// SceneSelectUnion::init $_1: __func::__clone()
static uint32_t t_6CCB6C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf676d4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CCB6C, "SceneSelectUnion::init $_1 __func::__clone()", t_6CCB6C);

// SceneSelectUnion::init $_1: __func::__clone(__base*)
static void t_6CCB98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf676d4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CCB98, "SceneSelectUnion::init $_1 __func::__clone(__base*)", t_6CCB98);

// SceneSelectUnion::init $_1: __func::destroy
static void t_6CCBB8(uint32_t self) {
}
RW_FUNCTION(0x6CCBB8, "SceneSelectUnion::init $_1 __func::destroy", t_6CCBB8);

// SceneSelectUnion::init $_1: __func::destroy_deallocate
static void t_6CCBBA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CCBBA, "SceneSelectUnion::init $_1 __func::destroy_deallocate", t_6CCBBA);

// SceneSelectUnion::init $_1: __func::target
static uint32_t t_6CCBCC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e580u ? self + 4 : 0; }
RW_FUNCTION(0x6CCBCC, "SceneSelectUnion::init $_1 __func::target", t_6CCBCC);

// SceneSelectUnion::init $_1: __func::target_type
static uint32_t t_6CCBEC() { return 0xf9fd94u; }
RW_FUNCTION(0x6CCBEC, "SceneSelectUnion::init $_1 __func::target_type", t_6CCBEC);

// SceneSelectUnion::init $_0: __func::~__func (deleting)
static void t_6CCC00(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CCC00, "SceneSelectUnion::init $_0 __func::~__func (deleting)", t_6CCC00);

// SceneSelectUnion::init $_0: __func::__clone()
static uint32_t t_6CCC08(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67700u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6CCC08, "SceneSelectUnion::init $_0 __func::__clone()", t_6CCC08);

// SceneSelectUnion::init $_0: __func::__clone(__base*)
static void t_6CCC34(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67700u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6CCC34, "SceneSelectUnion::init $_0 __func::__clone(__base*)", t_6CCC34);

// SceneSelectUnion::init $_0: __func::destroy
static void t_6CCC54(uint32_t self) {
}
RW_FUNCTION(0x6CCC54, "SceneSelectUnion::init $_0 __func::destroy", t_6CCC54);

// SceneSelectUnion::init $_0: __func::destroy_deallocate
static void t_6CCC56(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6CCC56, "SceneSelectUnion::init $_0 __func::destroy_deallocate", t_6CCC56);

// SceneSelectUnion::init $_0: __func::target
static uint32_t t_6CCC68(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7e610u ? self + 4 : 0; }
RW_FUNCTION(0x6CCC68, "SceneSelectUnion::init $_0 __func::target", t_6CCC68);

// SceneSelectUnion::init $_0: __func::target_type
static uint32_t t_6CCC88() { return 0xf9fda8u; }
RW_FUNCTION(0x6CCC88, "SceneSelectUnion::init $_0 __func::target_type", t_6CCC88);

// ---- further thunks attributed to SceneSelectUnion (wave 3)

// OtherMenuDialog::open $_66 UlvE_: __func::__clone()
static uint32_t t_5AAD14(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5bfb4u);
        call(0x5aac65, {v1 + 8, self + 8});   // _ZNSt3__18functionIFPN7cocos2d5LayerEvEEC2ERKS5_
    }
    return v1;
}
RW_FUNCTION(0x5AAD14, "OtherMenuDialog::open $_66 UlvE_ __func::__clone()", t_5AAD14);

// OtherMenuDialog::open $_66 UlvE_: __func::__clone(__base*)
static void t_5AAD64(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bfb4u);
        call(0x5aac65, {p + 8, self + 8});   // _ZNSt3__18functionIFPN7cocos2d5LayerEvEEC2ERKS5_
    }
}
RW_FUNCTION(0x5AAD64, "OtherMenuDialog::open $_66 UlvE_ __func::__clone(__base*)", t_5AAD64);

// SceneDownload::finalize $_3 UlvE_ UlbE_: __func::target
static uint32_t t_6C8B24(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7ce50u ? self + 4 : 0; }
RW_FUNCTION(0x6C8B24, "SceneDownload::finalize $_3 UlvE_ UlbE_ __func::target", t_6C8B24);

// SceneDownload::finalize $_3 UlvE_ UlbE_: __func::target_type
static uint32_t t_6C8B44() { return 0xf9f850u; }
RW_FUNCTION(0x6C8B44, "SceneDownload::finalize $_3 UlvE_ UlbE_ __func::target_type", t_6C8B44);

// SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6C8B58(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6C8B58, "SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6C8B58);

// SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6C8B60(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67230u);
    }
    return v1;
}
RW_FUNCTION(0x6C8B60, "SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6C8B60);

// SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6C8B84(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67230u);
    }
}
RW_FUNCTION(0x6C8B84, "SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6C8B84);

// SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6C8BA0(uint32_t self) {
}
RW_FUNCTION(0x6C8BA0, "SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_ __func::destroy", t_6C8BA0);

// SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6C8BA2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6C8BA2, "SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6C8BA2);

// SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6C8BAC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7cf10u ? self + 4 : 0; }
RW_FUNCTION(0x6C8BAC, "SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_ __func::target", t_6C8BAC);

// SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6C8BCC() { return 0xf9f864u; }
RW_FUNCTION(0x6C8BCC, "SceneDownload::finalize $_3 UlvE_ UlbE_ UlbE_ __func::target_type", t_6C8BCC);
