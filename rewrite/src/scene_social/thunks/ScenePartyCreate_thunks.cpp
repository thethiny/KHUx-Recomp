// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ScenePartyCreate lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ScenePartyCreate::openPopUpCreateParty $_13: __func::~__func (deleting)
static void t_6D42C4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D42C4, "ScenePartyCreate::openPopUpCreateParty $_13 __func::~__func (deleting)", t_6D42C4);

// ScenePartyCreate::openPopUpCreateParty $_13: __func::__clone()
static uint32_t t_6D42CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf67b20u);
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
RW_FUNCTION(0x6D42CC, "ScenePartyCreate::openPopUpCreateParty $_13 __func::__clone()", t_6D42CC);

// ScenePartyCreate::openPopUpCreateParty $_13: __func::__clone(__base*)
static void t_6D4300(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67b20u);
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
RW_FUNCTION(0x6D4300, "ScenePartyCreate::openPopUpCreateParty $_13 __func::__clone(__base*)", t_6D4300);

// ScenePartyCreate::openPopUpCreateParty $_13: __func::destroy
static void t_6D432C(uint32_t self) {
}
RW_FUNCTION(0x6D432C, "ScenePartyCreate::openPopUpCreateParty $_13 __func::destroy", t_6D432C);

// ScenePartyCreate::openPopUpCreateParty $_13: __func::destroy_deallocate
static void t_6D432E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D432E, "ScenePartyCreate::openPopUpCreateParty $_13 __func::destroy_deallocate", t_6D432E);

// ScenePartyCreate::openPopUpCreateParty $_13: __func::target
static uint32_t t_6D43A4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f750u ? self + 4 : 0; }
RW_FUNCTION(0x6D43A4, "ScenePartyCreate::openPopUpCreateParty $_13 __func::target", t_6D43A4);

// ScenePartyCreate::openPopUpCreateParty $_13: __func::target_type
static uint32_t t_6D43C4() { return 0xfa0974u; }
RW_FUNCTION(0x6D43C4, "ScenePartyCreate::openPopUpCreateParty $_13 __func::target_type", t_6D43C4);

// ScenePartyCreate::openPopUpCreateParty $_12: __func::~__func (deleting)
static void t_6D43D8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D43D8, "ScenePartyCreate::openPopUpCreateParty $_12 __func::~__func (deleting)", t_6D43D8);

// ScenePartyCreate::openPopUpCreateParty $_12: __func::__clone()
static uint32_t t_6D43E0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf67b4cu);
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
RW_FUNCTION(0x6D43E0, "ScenePartyCreate::openPopUpCreateParty $_12 __func::__clone()", t_6D43E0);

// ScenePartyCreate::openPopUpCreateParty $_12: __func::__clone(__base*)
static void t_6D4414(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67b4cu);
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
RW_FUNCTION(0x6D4414, "ScenePartyCreate::openPopUpCreateParty $_12 __func::__clone(__base*)", t_6D4414);

// ScenePartyCreate::openPopUpCreateParty $_12: __func::destroy
static void t_6D4440(uint32_t self) {
}
RW_FUNCTION(0x6D4440, "ScenePartyCreate::openPopUpCreateParty $_12 __func::destroy", t_6D4440);

// ScenePartyCreate::openPopUpCreateParty $_12: __func::destroy_deallocate
static void t_6D4442(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4442, "ScenePartyCreate::openPopUpCreateParty $_12 __func::destroy_deallocate", t_6D4442);

// ScenePartyCreate::openPopUpCreateParty $_12: __func::target
static uint32_t t_6D44BC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f830u ? self + 4 : 0; }
RW_FUNCTION(0x6D44BC, "ScenePartyCreate::openPopUpCreateParty $_12 __func::target", t_6D44BC);

// ScenePartyCreate::openPopUpCreateParty $_12: __func::target_type
static uint32_t t_6D44DC() { return 0xfa0988u; }
RW_FUNCTION(0x6D44DC, "ScenePartyCreate::openPopUpCreateParty $_12 __func::target_type", t_6D44DC);

// ScenePartyCreate::openPopUpCreateParty $_11: __func::~__func (deleting)
static void t_6D44F0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D44F0, "ScenePartyCreate::openPopUpCreateParty $_11 __func::~__func (deleting)", t_6D44F0);

// ScenePartyCreate::openPopUpCreateParty $_11: __func::__clone()
static uint32_t t_6D44F8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf67b78u);
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
RW_FUNCTION(0x6D44F8, "ScenePartyCreate::openPopUpCreateParty $_11 __func::__clone()", t_6D44F8);

// ScenePartyCreate::openPopUpCreateParty $_11: __func::__clone(__base*)
static void t_6D452C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67b78u);
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
RW_FUNCTION(0x6D452C, "ScenePartyCreate::openPopUpCreateParty $_11 __func::__clone(__base*)", t_6D452C);

// ScenePartyCreate::openPopUpCreateParty $_11: __func::destroy
static void t_6D4558(uint32_t self) {
}
RW_FUNCTION(0x6D4558, "ScenePartyCreate::openPopUpCreateParty $_11 __func::destroy", t_6D4558);

// ScenePartyCreate::openPopUpCreateParty $_11: __func::destroy_deallocate
static void t_6D455A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D455A, "ScenePartyCreate::openPopUpCreateParty $_11 __func::destroy_deallocate", t_6D455A);

// ScenePartyCreate::openPopUpCreateParty $_11: __func::target
static uint32_t t_6D45D4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f910u ? self + 4 : 0; }
RW_FUNCTION(0x6D45D4, "ScenePartyCreate::openPopUpCreateParty $_11 __func::target", t_6D45D4);

// ScenePartyCreate::openPopUpCreateParty $_11: __func::target_type
static uint32_t t_6D45F4() { return 0xfa099cu; }
RW_FUNCTION(0x6D45F4, "ScenePartyCreate::openPopUpCreateParty $_11 __func::target_type", t_6D45F4);

// ScenePartyCreate::openPopUpCreateParty $_9: __func::~__func (deleting)
static void t_6D4608(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4608, "ScenePartyCreate::openPopUpCreateParty $_9 __func::~__func (deleting)", t_6D4608);

// ScenePartyCreate::openPopUpCreateParty $_9: __func::__clone()
static uint32_t t_6D4610(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf67ba4u);
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
RW_FUNCTION(0x6D4610, "ScenePartyCreate::openPopUpCreateParty $_9 __func::__clone()", t_6D4610);

// ScenePartyCreate::openPopUpCreateParty $_9: __func::__clone(__base*)
static void t_6D4644(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67ba4u);
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
RW_FUNCTION(0x6D4644, "ScenePartyCreate::openPopUpCreateParty $_9 __func::__clone(__base*)", t_6D4644);

// ScenePartyCreate::openPopUpCreateParty $_9: __func::destroy
static void t_6D4670(uint32_t self) {
}
RW_FUNCTION(0x6D4670, "ScenePartyCreate::openPopUpCreateParty $_9 __func::destroy", t_6D4670);

// ScenePartyCreate::openPopUpCreateParty $_9: __func::destroy_deallocate
static void t_6D4672(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4672, "ScenePartyCreate::openPopUpCreateParty $_9 __func::destroy_deallocate", t_6D4672);

// ScenePartyCreate::openPopUpCreateParty $_9: __func::target
static uint32_t t_6D4694(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7f9f0u ? self + 4 : 0; }
RW_FUNCTION(0x6D4694, "ScenePartyCreate::openPopUpCreateParty $_9 __func::target", t_6D4694);

// ScenePartyCreate::openPopUpCreateParty $_9: __func::target_type
static uint32_t t_6D46B4() { return 0xfa09b0u; }
RW_FUNCTION(0x6D46B4, "ScenePartyCreate::openPopUpCreateParty $_9 __func::target_type", t_6D46B4);

// ScenePartyCreate::openPopUpCreateParty $_8: __func::~__func (deleting)
static void t_6D46C8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D46C8, "ScenePartyCreate::openPopUpCreateParty $_8 __func::~__func (deleting)", t_6D46C8);

// ScenePartyCreate::openPopUpCreateParty $_8: __func::__clone()
static uint32_t t_6D46D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf67bd0u);
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
RW_FUNCTION(0x6D46D0, "ScenePartyCreate::openPopUpCreateParty $_8 __func::__clone()", t_6D46D0);

// ScenePartyCreate::openPopUpCreateParty $_8: __func::__clone(__base*)
static void t_6D4704(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67bd0u);
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
RW_FUNCTION(0x6D4704, "ScenePartyCreate::openPopUpCreateParty $_8 __func::__clone(__base*)", t_6D4704);

// ScenePartyCreate::openPopUpCreateParty $_8: __func::destroy
static void t_6D4730(uint32_t self) {
}
RW_FUNCTION(0x6D4730, "ScenePartyCreate::openPopUpCreateParty $_8 __func::destroy", t_6D4730);

// ScenePartyCreate::openPopUpCreateParty $_8: __func::destroy_deallocate
static void t_6D4732(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4732, "ScenePartyCreate::openPopUpCreateParty $_8 __func::destroy_deallocate", t_6D4732);

// ScenePartyCreate::openPopUpCreateParty $_8: __func::target
static uint32_t t_6D4754(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7fad0u ? self + 4 : 0; }
RW_FUNCTION(0x6D4754, "ScenePartyCreate::openPopUpCreateParty $_8 __func::target", t_6D4754);

// ScenePartyCreate::openPopUpCreateParty $_8: __func::target_type
static uint32_t t_6D4774() { return 0xfa09c4u; }
RW_FUNCTION(0x6D4774, "ScenePartyCreate::openPopUpCreateParty $_8 __func::target_type", t_6D4774);

// ScenePartyCreate::openPopUpCreateParty $_7: __func::~__func (deleting)
static void t_6D4788(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4788, "ScenePartyCreate::openPopUpCreateParty $_7 __func::~__func (deleting)", t_6D4788);

// ScenePartyCreate::openPopUpCreateParty $_7: __func::__clone()
static uint32_t t_6D4790(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf67bfcu);
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
RW_FUNCTION(0x6D4790, "ScenePartyCreate::openPopUpCreateParty $_7 __func::__clone()", t_6D4790);

// ScenePartyCreate::openPopUpCreateParty $_7: __func::__clone(__base*)
static void t_6D47C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67bfcu);
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
RW_FUNCTION(0x6D47C4, "ScenePartyCreate::openPopUpCreateParty $_7 __func::__clone(__base*)", t_6D47C4);

// ScenePartyCreate::openPopUpCreateParty $_7: __func::destroy
static void t_6D47F0(uint32_t self) {
}
RW_FUNCTION(0x6D47F0, "ScenePartyCreate::openPopUpCreateParty $_7 __func::destroy", t_6D47F0);

// ScenePartyCreate::openPopUpCreateParty $_7: __func::destroy_deallocate
static void t_6D47F2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D47F2, "ScenePartyCreate::openPopUpCreateParty $_7 __func::destroy_deallocate", t_6D47F2);

// ScenePartyCreate::openPopUpCreateParty $_7: __func::target
static uint32_t t_6D4814(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7fbb0u ? self + 4 : 0; }
RW_FUNCTION(0x6D4814, "ScenePartyCreate::openPopUpCreateParty $_7 __func::target", t_6D4814);

// ScenePartyCreate::openPopUpCreateParty $_7: __func::target_type
static uint32_t t_6D4834() { return 0xfa09d8u; }
RW_FUNCTION(0x6D4834, "ScenePartyCreate::openPopUpCreateParty $_7 __func::target_type", t_6D4834);

// ScenePartyCreate::openPopUpCreateParty $_5: __func::~__func (deleting)
static void t_6D4848(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4848, "ScenePartyCreate::openPopUpCreateParty $_5 __func::~__func (deleting)", t_6D4848);

// ScenePartyCreate::openPopUpCreateParty $_5: __func::__clone()
static uint32_t t_6D4850(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf67c28u);
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
RW_FUNCTION(0x6D4850, "ScenePartyCreate::openPopUpCreateParty $_5 __func::__clone()", t_6D4850);

// ScenePartyCreate::openPopUpCreateParty $_5: __func::__clone(__base*)
static void t_6D4884(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67c28u);
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
RW_FUNCTION(0x6D4884, "ScenePartyCreate::openPopUpCreateParty $_5 __func::__clone(__base*)", t_6D4884);

// ScenePartyCreate::openPopUpCreateParty $_5: __func::destroy
static void t_6D48AC(uint32_t self) {
}
RW_FUNCTION(0x6D48AC, "ScenePartyCreate::openPopUpCreateParty $_5 __func::destroy", t_6D48AC);

// ScenePartyCreate::openPopUpCreateParty $_5: __func::destroy_deallocate
static void t_6D48AE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D48AE, "ScenePartyCreate::openPopUpCreateParty $_5 __func::destroy_deallocate", t_6D48AE);

// ScenePartyCreate::openPopUpCreateParty $_5: __func::target
static uint32_t t_6D48DC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7fc60u ? self + 4 : 0; }
RW_FUNCTION(0x6D48DC, "ScenePartyCreate::openPopUpCreateParty $_5 __func::target", t_6D48DC);

// ScenePartyCreate::openPopUpCreateParty $_5: __func::target_type
static uint32_t t_6D48FC() { return 0xfa09ecu; }
RW_FUNCTION(0x6D48FC, "ScenePartyCreate::openPopUpCreateParty $_5 __func::target_type", t_6D48FC);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_: __func::~__func (deleting)
static void t_6D4A44(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4A44, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ __func::~__func (deleting)", t_6D4A44);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_: __func::__clone()
static uint32_t t_6D4A4C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf67c54u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6D4A4C, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ __func::__clone()", t_6D4A4C);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_: __func::__clone(__base*)
static void t_6D4A7C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67c54u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D4A7C, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ __func::__clone(__base*)", t_6D4A7C);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_: __func::destroy
static void t_6D4AA4(uint32_t self) {
}
RW_FUNCTION(0x6D4AA4, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ __func::destroy", t_6D4AA4);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_: __func::destroy_deallocate
static void t_6D4AA6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4AA6, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ __func::destroy_deallocate", t_6D4AA6);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_: __func::target
static uint32_t t_6D4B54(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7fd20u ? self + 4 : 0; }
RW_FUNCTION(0x6D4B54, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ __func::target", t_6D4B54);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_: __func::target_type
static uint32_t t_6D4B74() { return 0xfa0a00u; }
RW_FUNCTION(0x6D4B74, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ __func::target_type", t_6D4B74);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_: __func::~__func (deleting)
static void t_6D4B88(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4B88, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_ __func::~__func (deleting)", t_6D4B88);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_: __func::__clone()
static uint32_t t_6D4B90(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf67c80u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D4B90, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_ __func::__clone()", t_6D4B90);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_: __func::__clone(__base*)
static void t_6D4BC0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67c80u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D4BC0, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_ __func::__clone(__base*)", t_6D4BC0);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_: __func::destroy
static void t_6D4BE4(uint32_t self) {
}
RW_FUNCTION(0x6D4BE4, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_ __func::destroy", t_6D4BE4);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_: __func::destroy_deallocate
static void t_6D4BE6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4BE6, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_ __func::destroy_deallocate", t_6D4BE6);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_: __func::target
static uint32_t t_6D4D2C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7fe00u ? self + 4 : 0; }
RW_FUNCTION(0x6D4D2C, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_ __func::target", t_6D4D2C);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_: __func::target_type
static uint32_t t_6D4D4C() { return 0xfa0a14u; }
RW_FUNCTION(0x6D4D4C, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE0_ UlvE_ __func::target_type", t_6D4D4C);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE_: __func::~__func (deleting)
static void t_6D4D60(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4D60, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE_ __func::~__func (deleting)", t_6D4D60);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE_: __func::__clone()
static uint32_t t_6D4D68(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67cacu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D4D68, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE_ __func::__clone()", t_6D4D68);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE_: __func::__clone(__base*)
static void t_6D4D94(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67cacu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D4D94, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE_ __func::__clone(__base*)", t_6D4D94);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE_: __func::destroy
static void t_6D4DB4(uint32_t self) {
}
RW_FUNCTION(0x6D4DB4, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE_ __func::destroy", t_6D4DB4);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE_: __func::destroy_deallocate
static void t_6D4DB6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4DB6, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE_ __func::destroy_deallocate", t_6D4DB6);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE_: __func::target
static uint32_t t_6D4E74(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7fed0u ? self + 4 : 0; }
RW_FUNCTION(0x6D4E74, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE_ __func::target", t_6D4E74);

// ScenePartyCreate::openPopUpCreateParty $_14 UlvE_: __func::target_type
static uint32_t t_6D4E94() { return 0xfa0a28u; }
RW_FUNCTION(0x6D4E94, "ScenePartyCreate::openPopUpCreateParty $_14 UlvE_ __func::target_type", t_6D4E94);

// ScenePartyCreate::openPopUpCreateParty $_4: __func::~__func (deleting)
static void t_6D4EA8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4EA8, "ScenePartyCreate::openPopUpCreateParty $_4 __func::~__func (deleting)", t_6D4EA8);

// ScenePartyCreate::openPopUpCreateParty $_4: __func::__clone()
static uint32_t t_6D4EB0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67cd8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D4EB0, "ScenePartyCreate::openPopUpCreateParty $_4 __func::__clone()", t_6D4EB0);

// ScenePartyCreate::openPopUpCreateParty $_4: __func::__clone(__base*)
static void t_6D4EDC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67cd8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D4EDC, "ScenePartyCreate::openPopUpCreateParty $_4 __func::__clone(__base*)", t_6D4EDC);

// ScenePartyCreate::openPopUpCreateParty $_4: __func::destroy
static void t_6D4EFC(uint32_t self) {
}
RW_FUNCTION(0x6D4EFC, "ScenePartyCreate::openPopUpCreateParty $_4 __func::destroy", t_6D4EFC);

// ScenePartyCreate::openPopUpCreateParty $_4: __func::destroy_deallocate
static void t_6D4EFE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4EFE, "ScenePartyCreate::openPopUpCreateParty $_4 __func::destroy_deallocate", t_6D4EFE);

// ScenePartyCreate::openPopUpCreateParty $_4: __func::target
static uint32_t t_6D4F10(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe7ff80u ? self + 4 : 0; }
RW_FUNCTION(0x6D4F10, "ScenePartyCreate::openPopUpCreateParty $_4 __func::target", t_6D4F10);

// ScenePartyCreate::openPopUpCreateParty $_4: __func::target_type
static uint32_t t_6D4F30() { return 0xfa0a3cu; }
RW_FUNCTION(0x6D4F30, "ScenePartyCreate::openPopUpCreateParty $_4 __func::target_type", t_6D4F30);

// ScenePartyCreate::openPopUpCreateParty $_3: __func::~__func (deleting)
static void t_6D4F44(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4F44, "ScenePartyCreate::openPopUpCreateParty $_3 __func::~__func (deleting)", t_6D4F44);

// ScenePartyCreate::openPopUpCreateParty $_3: __func::__clone()
static uint32_t t_6D4F4C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67d04u);
    }
    return v1;
}
RW_FUNCTION(0x6D4F4C, "ScenePartyCreate::openPopUpCreateParty $_3 __func::__clone()", t_6D4F4C);

// ScenePartyCreate::openPopUpCreateParty $_3: __func::__clone(__base*)
static void t_6D4F70(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67d04u);
    }
}
RW_FUNCTION(0x6D4F70, "ScenePartyCreate::openPopUpCreateParty $_3 __func::__clone(__base*)", t_6D4F70);

// ScenePartyCreate::openPopUpCreateParty $_3: __func::destroy
static void t_6D4F8C(uint32_t self) {
}
RW_FUNCTION(0x6D4F8C, "ScenePartyCreate::openPopUpCreateParty $_3 __func::destroy", t_6D4F8C);

// ScenePartyCreate::openPopUpCreateParty $_3: __func::destroy_deallocate
static void t_6D4F8E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4F8E, "ScenePartyCreate::openPopUpCreateParty $_3 __func::destroy_deallocate", t_6D4F8E);

// ScenePartyCreate::openPopUpCreateParty $_3: __func::target
static uint32_t t_6D4FAC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80060u ? self + 4 : 0; }
RW_FUNCTION(0x6D4FAC, "ScenePartyCreate::openPopUpCreateParty $_3 __func::target", t_6D4FAC);

// ScenePartyCreate::openPopUpCreateParty $_3: __func::target_type
static uint32_t t_6D4FCC() { return 0xfa0a50u; }
RW_FUNCTION(0x6D4FCC, "ScenePartyCreate::openPopUpCreateParty $_3 __func::target_type", t_6D4FCC);

// ScenePartyCreate::openPopUpCreateParty $_2: __func::~__func (deleting)
static void t_6D4FE0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D4FE0, "ScenePartyCreate::openPopUpCreateParty $_2 __func::~__func (deleting)", t_6D4FE0);

// ScenePartyCreate::openPopUpCreateParty $_2: __func::__clone()
static uint32_t t_6D4FE8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf67d30u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20 + 0);
        uint32_t v7 = ld32(self + 20 + 4);
        uint32_t v8 = ld32(self + 20 + 8);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
    }
    return v1;
}
RW_FUNCTION(0x6D4FE8, "ScenePartyCreate::openPopUpCreateParty $_2 __func::__clone()", t_6D4FE8);

// ScenePartyCreate::openPopUpCreateParty $_2: __func::__clone(__base*)
static void t_6D5024(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67d30u);
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
        uint32_t v7 = ld32(self + 28);
        st32(p + 28, v7);
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6D5024, "ScenePartyCreate::openPopUpCreateParty $_2 __func::__clone(__base*)", t_6D5024);

// ScenePartyCreate::openPopUpCreateParty $_2: __func::destroy
static void t_6D5054(uint32_t self) {
}
RW_FUNCTION(0x6D5054, "ScenePartyCreate::openPopUpCreateParty $_2 __func::destroy", t_6D5054);

// ScenePartyCreate::openPopUpCreateParty $_2: __func::destroy_deallocate
static void t_6D5056(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D5056, "ScenePartyCreate::openPopUpCreateParty $_2 __func::destroy_deallocate", t_6D5056);

// ScenePartyCreate::openPopUpCreateParty $_2: __func::target
static uint32_t t_6D5624(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80140u ? self + 4 : 0; }
RW_FUNCTION(0x6D5624, "ScenePartyCreate::openPopUpCreateParty $_2 __func::target", t_6D5624);

// ScenePartyCreate::openPopUpCreateParty $_2: __func::target_type
static uint32_t t_6D5644() { return 0xfa0a64u; }
RW_FUNCTION(0x6D5644, "ScenePartyCreate::openPopUpCreateParty $_2 __func::target_type", t_6D5644);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_: __func::~__func (deleting)
static void t_6D56B0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D56B0, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ __func::~__func (deleting)", t_6D56B0);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_: __func::__clone()
static uint32_t t_6D56B8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf67d5cu);
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
RW_FUNCTION(0x6D56B8, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ __func::__clone()", t_6D56B8);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_: __func::__clone(__base*)
static void t_6D56EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67d5cu);
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
RW_FUNCTION(0x6D56EC, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ __func::__clone(__base*)", t_6D56EC);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_: __func::destroy
static void t_6D5714(uint32_t self) {
}
RW_FUNCTION(0x6D5714, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ __func::destroy", t_6D5714);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_: __func::destroy_deallocate
static void t_6D5716(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D5716, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ __func::destroy_deallocate", t_6D5716);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_: __func::target
static uint32_t t_6D5828(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80220u ? self + 4 : 0; }
RW_FUNCTION(0x6D5828, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ __func::target", t_6D5828);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_: __func::target_type
static uint32_t t_6D5848() { return 0xfa0a78u; }
RW_FUNCTION(0x6D5848, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ __func::target_type", t_6D5848);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_: __func::~__func (deleting)
static void t_6D585C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D585C, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_ __func::~__func (deleting)", t_6D585C);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_: __func::__clone()
static uint32_t t_6D5864(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67d88u);
    }
    return v1;
}
RW_FUNCTION(0x6D5864, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_ __func::__clone()", t_6D5864);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_: __func::__clone(__base*)
static void t_6D5888(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67d88u);
    }
}
RW_FUNCTION(0x6D5888, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_ __func::__clone(__base*)", t_6D5888);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_: __func::destroy
static void t_6D58A4(uint32_t self) {
}
RW_FUNCTION(0x6D58A4, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_ __func::destroy", t_6D58A4);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_: __func::destroy_deallocate
static void t_6D58A6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D58A6, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_ __func::destroy_deallocate", t_6D58A6);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_: __func::target
static uint32_t t_6D58B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80340u ? self + 4 : 0; }
RW_FUNCTION(0x6D58B0, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_ __func::target", t_6D58B0);

// ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_: __func::target_type
static uint32_t t_6D58D0() { return 0xfa0a8cu; }
RW_FUNCTION(0x6D58D0, "ScenePartyCreate::openPopUpCreateParty $_2 UlbE_ UlbE_ __func::target_type", t_6D58D0);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_: __func::~__func (deleting)
static void t_6D58E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D58E4, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_ __func::~__func (deleting)", t_6D58E4);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_: __func::__clone()
static uint32_t t_6D58EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67db4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D58EC, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_ __func::__clone()", t_6D58EC);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_: __func::__clone(__base*)
static void t_6D5918(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67db4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D5918, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_ __func::__clone(__base*)", t_6D5918);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_: __func::destroy
static void t_6D5938(uint32_t self) {
}
RW_FUNCTION(0x6D5938, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_ __func::destroy", t_6D5938);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_: __func::destroy_deallocate
static void t_6D593A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D593A, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_ __func::destroy_deallocate", t_6D593A);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_: __func::target
static uint32_t t_6D5A30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80470u ? self + 4 : 0; }
RW_FUNCTION(0x6D5A30, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_ __func::target", t_6D5A30);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_: __func::target_type
static uint32_t t_6D5A50() { return 0xfa0aa0u; }
RW_FUNCTION(0x6D5A50, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE0_ __func::target_type", t_6D5A50);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE_: __func::~__func (deleting)
static void t_6D5A64(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D5A64, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE_ __func::~__func (deleting)", t_6D5A64);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE_: __func::__clone()
static uint32_t t_6D5A6C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67de0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D5A6C, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE_ __func::__clone()", t_6D5A6C);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE_: __func::__clone(__base*)
static void t_6D5A98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67de0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D5A98, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE_ __func::__clone(__base*)", t_6D5A98);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE_: __func::destroy
static void t_6D5AB8(uint32_t self) {
}
RW_FUNCTION(0x6D5AB8, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE_ __func::destroy", t_6D5AB8);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE_: __func::destroy_deallocate
static void t_6D5ABA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D5ABA, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE_ __func::destroy_deallocate", t_6D5ABA);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE_: __func::target
static uint32_t t_6D5BB0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80580u ? self + 4 : 0; }
RW_FUNCTION(0x6D5BB0, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE_ __func::target", t_6D5BB0);

// ScenePartyCreate::openPopUpCreateParty $_2 UlvE_: __func::target_type
static uint32_t t_6D5BD0() { return 0xfa0ab4u; }
RW_FUNCTION(0x6D5BD0, "ScenePartyCreate::openPopUpCreateParty $_2 UlvE_ __func::target_type", t_6D5BD0);

// ScenePartyCreate::init $_1: __func::~__func (deleting)
static void t_6D5BE4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D5BE4, "ScenePartyCreate::init $_1 __func::~__func (deleting)", t_6D5BE4);

// ScenePartyCreate::init $_1: __func::__clone()
static uint32_t t_6D5BEC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67e0cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D5BEC, "ScenePartyCreate::init $_1 __func::__clone()", t_6D5BEC);

// ScenePartyCreate::init $_1: __func::__clone(__base*)
static void t_6D5C18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67e0cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D5C18, "ScenePartyCreate::init $_1 __func::__clone(__base*)", t_6D5C18);

// ScenePartyCreate::init $_1: __func::destroy
static void t_6D5C38(uint32_t self) {
}
RW_FUNCTION(0x6D5C38, "ScenePartyCreate::init $_1 __func::destroy", t_6D5C38);

// ScenePartyCreate::init $_1: __func::destroy_deallocate
static void t_6D5C3A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D5C3A, "ScenePartyCreate::init $_1 __func::destroy_deallocate", t_6D5C3A);

// ScenePartyCreate::init $_1: __func::target
static uint32_t t_6D5C4C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80650u ? self + 4 : 0; }
RW_FUNCTION(0x6D5C4C, "ScenePartyCreate::init $_1 __func::target", t_6D5C4C);

// ScenePartyCreate::init $_1: __func::target_type
static uint32_t t_6D5C6C() { return 0xfa0ac8u; }
RW_FUNCTION(0x6D5C6C, "ScenePartyCreate::init $_1 __func::target_type", t_6D5C6C);

// ScenePartyCreate::init $_0: __func::~__func (deleting)
static void t_6D5C80(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D5C80, "ScenePartyCreate::init $_0 __func::~__func (deleting)", t_6D5C80);

// ScenePartyCreate::init $_0: __func::__clone()
static uint32_t t_6D5C88(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67e38u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6D5C88, "ScenePartyCreate::init $_0 __func::__clone()", t_6D5C88);

// ScenePartyCreate::init $_0: __func::__clone(__base*)
static void t_6D5CB4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67e38u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6D5CB4, "ScenePartyCreate::init $_0 __func::__clone(__base*)", t_6D5CB4);

// ScenePartyCreate::init $_0: __func::destroy
static void t_6D5CD4(uint32_t self) {
}
RW_FUNCTION(0x6D5CD4, "ScenePartyCreate::init $_0 __func::destroy", t_6D5CD4);

// ScenePartyCreate::init $_0: __func::destroy_deallocate
static void t_6D5CD6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6D5CD6, "ScenePartyCreate::init $_0 __func::destroy_deallocate", t_6D5CD6);

// ScenePartyCreate::init $_0: __func::target
static uint32_t t_6D5CE8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe806e0u ? self + 4 : 0; }
RW_FUNCTION(0x6D5CE8, "ScenePartyCreate::init $_0 __func::target", t_6D5CE8);

// ScenePartyCreate::init $_0: __func::target_type
static uint32_t t_6D5D08() { return 0xfa0adcu; }
RW_FUNCTION(0x6D5D08, "ScenePartyCreate::init $_0 __func::target_type", t_6D5D08);
