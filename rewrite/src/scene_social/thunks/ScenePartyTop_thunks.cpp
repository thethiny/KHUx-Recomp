// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ScenePartyTop lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ScenePartyTop::backScene $_25: __func::~__func (deleting)
static void t_655D88(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x655D88, "ScenePartyTop::backScene $_25 __func::~__func (deleting)", t_655D88);

// ScenePartyTop::backScene $_25: __func::__clone()
static uint32_t t_655D90(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf61ec8u);
    }
    return v1;
}
RW_FUNCTION(0x655D90, "ScenePartyTop::backScene $_25 __func::__clone()", t_655D90);

// ScenePartyTop::backScene $_25: __func::__clone(__base*)
static void t_655DB4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf61ec8u);
    }
}
RW_FUNCTION(0x655DB4, "ScenePartyTop::backScene $_25 __func::__clone(__base*)", t_655DB4);

// ScenePartyTop::backScene $_25: __func::destroy
static void t_655DD0(uint32_t self) {
}
RW_FUNCTION(0x655DD0, "ScenePartyTop::backScene $_25 __func::destroy", t_655DD0);

// ScenePartyTop::backScene $_25: __func::destroy_deallocate
static void t_655DD2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x655DD2, "ScenePartyTop::backScene $_25 __func::destroy_deallocate", t_655DD2);

// ScenePartyTop::backScene $_25: __func::target
static uint32_t t_655EFC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe60e70u ? self + 4 : 0; }
RW_FUNCTION(0x655EFC, "ScenePartyTop::backScene $_25 __func::target", t_655EFC);

// ScenePartyTop::backScene $_25: __func::target_type
static uint32_t t_655F1C() { return 0xf99ebcu; }
RW_FUNCTION(0x655F1C, "ScenePartyTop::backScene $_25 __func::target_type", t_655F1C);

// ScenePartyTop::openPopupPartyInvite $_24: __func::~__func (deleting)
static void t_655F30(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x655F30, "ScenePartyTop::openPopupPartyInvite $_24 __func::~__func (deleting)", t_655F30);

// ScenePartyTop::openPopupPartyInvite $_24: __func::__clone()
static uint32_t t_655F38(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf61ef4u);
    }
    return v1;
}
RW_FUNCTION(0x655F38, "ScenePartyTop::openPopupPartyInvite $_24 __func::__clone()", t_655F38);

// ScenePartyTop::openPopupPartyInvite $_24: __func::__clone(__base*)
static void t_655F5C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf61ef4u);
    }
}
RW_FUNCTION(0x655F5C, "ScenePartyTop::openPopupPartyInvite $_24 __func::__clone(__base*)", t_655F5C);

// ScenePartyTop::openPopupPartyInvite $_24: __func::destroy
static void t_655F78(uint32_t self) {
}
RW_FUNCTION(0x655F78, "ScenePartyTop::openPopupPartyInvite $_24 __func::destroy", t_655F78);

// ScenePartyTop::openPopupPartyInvite $_24: __func::destroy_deallocate
static void t_655F7A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x655F7A, "ScenePartyTop::openPopupPartyInvite $_24 __func::destroy_deallocate", t_655F7A);

// ScenePartyTop::openPopupPartyInvite $_24: __func::target
static uint32_t t_656A58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe60f10u ? self + 4 : 0; }
RW_FUNCTION(0x656A58, "ScenePartyTop::openPopupPartyInvite $_24 __func::target", t_656A58);

// ScenePartyTop::openPopupPartyInvite $_24: __func::target_type
static uint32_t t_656A78() { return 0xf99ed0u; }
RW_FUNCTION(0x656A78, "ScenePartyTop::openPopupPartyInvite $_24 __func::target_type", t_656A78);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE0_: __func::~__func (deleting)
static void t_656A8C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656A8C, "ScenePartyTop::openPopupPartyInvite $_24 UlbE0_ __func::~__func (deleting)", t_656A8C);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE0_: __func::__clone()
static uint32_t t_656A94(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf61f20u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x656A94, "ScenePartyTop::openPopupPartyInvite $_24 UlbE0_ __func::__clone()", t_656A94);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE0_: __func::__clone(__base*)
static void t_656AC4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf61f20u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x656AC4, "ScenePartyTop::openPopupPartyInvite $_24 UlbE0_ __func::__clone(__base*)", t_656AC4);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE0_: __func::destroy
static void t_656AEC(uint32_t self) {
}
RW_FUNCTION(0x656AEC, "ScenePartyTop::openPopupPartyInvite $_24 UlbE0_ __func::destroy", t_656AEC);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE0_: __func::destroy_deallocate
static void t_656AEE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656AEE, "ScenePartyTop::openPopupPartyInvite $_24 UlbE0_ __func::destroy_deallocate", t_656AEE);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE0_: __func::target
static uint32_t t_656B10(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe60fc0u ? self + 4 : 0; }
RW_FUNCTION(0x656B10, "ScenePartyTop::openPopupPartyInvite $_24 UlbE0_ __func::target", t_656B10);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE0_: __func::target_type
static uint32_t t_656B30() { return 0xf99ee4u; }
RW_FUNCTION(0x656B30, "ScenePartyTop::openPopupPartyInvite $_24 UlbE0_ __func::target_type", t_656B30);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_: __func::~__func (deleting)
static void t_656C18(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656C18, "ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_ __func::~__func (deleting)", t_656C18);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_: __func::__clone()
static uint32_t t_656C20(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf61f4cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x656C20, "ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_ __func::__clone()", t_656C20);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_: __func::__clone(__base*)
static void t_656C50(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf61f4cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x656C50, "ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_ __func::__clone(__base*)", t_656C50);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_: __func::destroy
static void t_656C78(uint32_t self) {
}
RW_FUNCTION(0x656C78, "ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_ __func::destroy", t_656C78);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_: __func::destroy_deallocate
static void t_656C7A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656C7A, "ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_ __func::destroy_deallocate", t_656C7A);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_: __func::target
static uint32_t t_656CAC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61080u ? self + 4 : 0; }
RW_FUNCTION(0x656CAC, "ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_ __func::target", t_656CAC);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_: __func::target_type
static uint32_t t_656CCC() { return 0xf99ef8u; }
RW_FUNCTION(0x656CCC, "ScenePartyTop::openPopupPartyInvite $_24 UlvE_ UlvE_ __func::target_type", t_656CCC);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE0_: __func::~__func (deleting)
static void t_656CE0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656CE0, "ScenePartyTop::openPopupPartyInvite $_24 UlvE0_ __func::~__func (deleting)", t_656CE0);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE0_: __func::__clone()
static uint32_t t_656CE8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf61f78u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x656CE8, "ScenePartyTop::openPopupPartyInvite $_24 UlvE0_ __func::__clone()", t_656CE8);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE0_: __func::__clone(__base*)
static void t_656D14(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf61f78u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x656D14, "ScenePartyTop::openPopupPartyInvite $_24 UlvE0_ __func::__clone(__base*)", t_656D14);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE0_: __func::destroy
static void t_656D34(uint32_t self) {
}
RW_FUNCTION(0x656D34, "ScenePartyTop::openPopupPartyInvite $_24 UlvE0_ __func::destroy", t_656D34);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE0_: __func::destroy_deallocate
static void t_656D36(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656D36, "ScenePartyTop::openPopupPartyInvite $_24 UlvE0_ __func::destroy_deallocate", t_656D36);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE0_: __func::target
static uint32_t t_656D48(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61150u ? self + 4 : 0; }
RW_FUNCTION(0x656D48, "ScenePartyTop::openPopupPartyInvite $_24 UlvE0_ __func::target", t_656D48);

// ScenePartyTop::openPopupPartyInvite $_24 UlvE0_: __func::target_type
static uint32_t t_656D68() { return 0xf99f0cu; }
RW_FUNCTION(0x656D68, "ScenePartyTop::openPopupPartyInvite $_24 UlvE0_ __func::target_type", t_656D68);

// ScenePartyTop::openPopupPartyInvite $_24: __func::~__func (deleting)
static void t_656D7C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656D7C, "ScenePartyTop::openPopupPartyInvite $_24 __func::~__func (deleting)", t_656D7C);

// ScenePartyTop::openPopupPartyInvite $_24: __func::__clone()
static uint32_t t_656D84(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf61fa4u);
    }
    return v1;
}
RW_FUNCTION(0x656D84, "ScenePartyTop::openPopupPartyInvite $_24 __func::__clone()", t_656D84);

// ScenePartyTop::openPopupPartyInvite $_24: __func::__clone(__base*)
static void t_656DA8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf61fa4u);
    }
}
RW_FUNCTION(0x656DA8, "ScenePartyTop::openPopupPartyInvite $_24 __func::__clone(__base*)", t_656DA8);

// ScenePartyTop::openPopupPartyInvite $_24: __func::destroy
static void t_656DC4(uint32_t self) {
}
RW_FUNCTION(0x656DC4, "ScenePartyTop::openPopupPartyInvite $_24 __func::destroy", t_656DC4);

// ScenePartyTop::openPopupPartyInvite $_24: __func::destroy_deallocate
static void t_656DC6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656DC6, "ScenePartyTop::openPopupPartyInvite $_24 __func::destroy_deallocate", t_656DC6);

// ScenePartyTop::openPopupPartyInvite $_24: __func::target
static uint32_t t_656DE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61240u ? self + 4 : 0; }
RW_FUNCTION(0x656DE4, "ScenePartyTop::openPopupPartyInvite $_24 __func::target", t_656DE4);

// ScenePartyTop::openPopupPartyInvite $_24: __func::target_type
static uint32_t t_656E04() { return 0xf99f20u; }
RW_FUNCTION(0x656E04, "ScenePartyTop::openPopupPartyInvite $_24 __func::target_type", t_656E04);

// ScenePartyTop::openPopupPartyInvite $_24: __func::~__func
static uint32_t t_656E18(uint32_t self) {
    st32(self, 0xf61fd0u);
    uint32_t v1 = ld8(self + 4);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 12);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x656E18, "ScenePartyTop::openPopupPartyInvite $_24 __func::~__func", t_656E18);

// ScenePartyTop::openPopupPartyInvite $_24: __func::~__func (deleting)
static void t_656E48(uint32_t self) {
    st32(self, 0xf61fd0u);
    uint32_t v1 = ld8(self + 4);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 12);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656E48, "ScenePartyTop::openPopupPartyInvite $_24 __func::~__func (deleting)", t_656E48);

// ScenePartyTop::openPopupPartyInvite $_24: __func::destroy
static void t_656F3C(uint32_t self) {
    uint32_t v1 = ld8(self + 4);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 12);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x656F3C, "ScenePartyTop::openPopupPartyInvite $_24 __func::destroy", t_656F3C);

// ScenePartyTop::openPopupPartyInvite $_24: __func::destroy_deallocate
static void t_656F4E(uint32_t self) {
    uint32_t v1 = ld8(self + 4);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 12);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x656F4E, "ScenePartyTop::openPopupPartyInvite $_24 __func::destroy_deallocate", t_656F4E);

// ScenePartyTop::openPopupPartyInvite $_24: __func::target
static uint32_t t_657110(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61360u ? self + 4 : 0; }
RW_FUNCTION(0x657110, "ScenePartyTop::openPopupPartyInvite $_24 __func::target", t_657110);

// ScenePartyTop::openPopupPartyInvite $_24: __func::target_type
static uint32_t t_657130() { return 0xf99f34u; }
RW_FUNCTION(0x657130, "ScenePartyTop::openPopupPartyInvite $_24 __func::target_type", t_657130);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE_: __func::~__func (deleting)
static void t_657144(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657144, "ScenePartyTop::openPopupPartyInvite $_24 UlbE_ __func::~__func (deleting)", t_657144);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE_: __func::__clone()
static uint32_t t_65714C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf61ffcu);
    }
    return v1;
}
RW_FUNCTION(0x65714C, "ScenePartyTop::openPopupPartyInvite $_24 UlbE_ __func::__clone()", t_65714C);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE_: __func::__clone(__base*)
static void t_657170(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf61ffcu);
    }
}
RW_FUNCTION(0x657170, "ScenePartyTop::openPopupPartyInvite $_24 UlbE_ __func::__clone(__base*)", t_657170);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE_: __func::destroy
static void t_65718C(uint32_t self) {
}
RW_FUNCTION(0x65718C, "ScenePartyTop::openPopupPartyInvite $_24 UlbE_ __func::destroy", t_65718C);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE_: __func::destroy_deallocate
static void t_65718E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65718E, "ScenePartyTop::openPopupPartyInvite $_24 UlbE_ __func::destroy_deallocate", t_65718E);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE_: __func::target
static uint32_t t_657198(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61450u ? self + 4 : 0; }
RW_FUNCTION(0x657198, "ScenePartyTop::openPopupPartyInvite $_24 UlbE_ __func::target", t_657198);

// ScenePartyTop::openPopupPartyInvite $_24 UlbE_: __func::target_type
static uint32_t t_6571B8() { return 0xf99f48u; }
RW_FUNCTION(0x6571B8, "ScenePartyTop::openPopupPartyInvite $_24 UlbE_ __func::target_type", t_6571B8);

// ScenePartyTop::runInEntryLayout $_23: __func::~__func (deleting)
static void t_6571CC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6571CC, "ScenePartyTop::runInEntryLayout $_23 __func::~__func (deleting)", t_6571CC);

// ScenePartyTop::runInEntryLayout $_23: __func::__clone()
static uint32_t t_6571D4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62028u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6571D4, "ScenePartyTop::runInEntryLayout $_23 __func::__clone()", t_6571D4);

// ScenePartyTop::runInEntryLayout $_23: __func::__clone(__base*)
static void t_657200(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62028u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x657200, "ScenePartyTop::runInEntryLayout $_23 __func::__clone(__base*)", t_657200);

// ScenePartyTop::runInEntryLayout $_23: __func::destroy
static void t_657220(uint32_t self) {
}
RW_FUNCTION(0x657220, "ScenePartyTop::runInEntryLayout $_23 __func::destroy", t_657220);

// ScenePartyTop::runInEntryLayout $_23: __func::destroy_deallocate
static void t_657222(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657222, "ScenePartyTop::runInEntryLayout $_23 __func::destroy_deallocate", t_657222);

// ScenePartyTop::runInEntryLayout $_23: __func::target
static uint32_t t_6572D4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61520u ? self + 4 : 0; }
RW_FUNCTION(0x6572D4, "ScenePartyTop::runInEntryLayout $_23 __func::target", t_6572D4);

// ScenePartyTop::runInEntryLayout $_23: __func::target_type
static uint32_t t_6572F4() { return 0xf99f5cu; }
RW_FUNCTION(0x6572F4, "ScenePartyTop::runInEntryLayout $_23 __func::target_type", t_6572F4);

// ScenePartyTop::runInEntryLayout $_23 UlvE_: __func::~__func (deleting)
static void t_657308(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657308, "ScenePartyTop::runInEntryLayout $_23 UlvE_ __func::~__func (deleting)", t_657308);

// ScenePartyTop::runInEntryLayout $_23 UlvE_: __func::__clone()
static uint32_t t_657310(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62054u);
    }
    return v1;
}
RW_FUNCTION(0x657310, "ScenePartyTop::runInEntryLayout $_23 UlvE_ __func::__clone()", t_657310);

// ScenePartyTop::runInEntryLayout $_23 UlvE_: __func::__clone(__base*)
static void t_657334(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62054u);
    }
}
RW_FUNCTION(0x657334, "ScenePartyTop::runInEntryLayout $_23 UlvE_ __func::__clone(__base*)", t_657334);

// ScenePartyTop::runInEntryLayout $_23 UlvE_: __func::destroy
static void t_657350(uint32_t self) {
}
RW_FUNCTION(0x657350, "ScenePartyTop::runInEntryLayout $_23 UlvE_ __func::destroy", t_657350);

// ScenePartyTop::runInEntryLayout $_23 UlvE_: __func::destroy_deallocate
static void t_657352(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657352, "ScenePartyTop::runInEntryLayout $_23 UlvE_ __func::destroy_deallocate", t_657352);

// ScenePartyTop::runInEntryLayout $_23 UlvE_: __func::target
static uint32_t t_6573F0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe615f0u ? self + 4 : 0; }
RW_FUNCTION(0x6573F0, "ScenePartyTop::runInEntryLayout $_23 UlvE_ __func::target", t_6573F0);

// ScenePartyTop::runInEntryLayout $_23 UlvE_: __func::target_type
static uint32_t t_657410() { return 0xf99f70u; }
RW_FUNCTION(0x657410, "ScenePartyTop::runInEntryLayout $_23 UlvE_ __func::target_type", t_657410);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_: __func::~__func (deleting)
static void t_657424(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657424, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ __func::~__func (deleting)", t_657424);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_: __func::__clone()
static uint32_t t_65742C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62080u);
    }
    return v1;
}
RW_FUNCTION(0x65742C, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ __func::__clone()", t_65742C);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_: __func::__clone(__base*)
static void t_657450(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62080u);
    }
}
RW_FUNCTION(0x657450, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ __func::__clone(__base*)", t_657450);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_: __func::destroy
static void t_65746C(uint32_t self) {
}
RW_FUNCTION(0x65746C, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ __func::destroy", t_65746C);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_: __func::destroy_deallocate
static void t_65746E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65746E, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ __func::destroy_deallocate", t_65746E);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_: __func::target
static uint32_t t_6575F8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61710u ? self + 4 : 0; }
RW_FUNCTION(0x6575F8, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ __func::target", t_6575F8);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_: __func::target_type
static uint32_t t_657618() { return 0xf99f84u; }
RW_FUNCTION(0x657618, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ __func::target_type", t_657618);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6576EC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6576EC, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6576EC);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6576F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf620acu);
    }
    return v1;
}
RW_FUNCTION(0x6576F4, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6576F4);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_657718(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf620acu);
    }
}
RW_FUNCTION(0x657718, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_657718);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_657734(uint32_t self) {
}
RW_FUNCTION(0x657734, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_ __func::destroy", t_657734);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_657736(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657736, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_657736);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_65774C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61850u ? self + 4 : 0; }
RW_FUNCTION(0x65774C, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_ __func::target", t_65774C);

// ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_65776C() { return 0xf99f98u; }
RW_FUNCTION(0x65776C, "ScenePartyTop::runInEntryLayout $_23 UlvE_ UlbE_ UlbE_ __func::target_type", t_65776C);

// ScenePartyTop::runInEntryLayout $_22: __func::~__func (deleting)
static void t_657780(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657780, "ScenePartyTop::runInEntryLayout $_22 __func::~__func (deleting)", t_657780);

// ScenePartyTop::runInEntryLayout $_22: __func::__clone()
static uint32_t t_657788(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf620d8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x657788, "ScenePartyTop::runInEntryLayout $_22 __func::__clone()", t_657788);

// ScenePartyTop::runInEntryLayout $_22: __func::__clone(__base*)
static void t_6577B4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf620d8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6577B4, "ScenePartyTop::runInEntryLayout $_22 __func::__clone(__base*)", t_6577B4);

// ScenePartyTop::runInEntryLayout $_22: __func::destroy
static void t_6577D4(uint32_t self) {
}
RW_FUNCTION(0x6577D4, "ScenePartyTop::runInEntryLayout $_22 __func::destroy", t_6577D4);

// ScenePartyTop::runInEntryLayout $_22: __func::destroy_deallocate
static void t_6577D6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6577D6, "ScenePartyTop::runInEntryLayout $_22 __func::destroy_deallocate", t_6577D6);

// ScenePartyTop::runInEntryLayout $_22: __func::target
static uint32_t t_657888(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61960u ? self + 4 : 0; }
RW_FUNCTION(0x657888, "ScenePartyTop::runInEntryLayout $_22 __func::target", t_657888);

// ScenePartyTop::runInEntryLayout $_22: __func::target_type
static uint32_t t_6578A8() { return 0xf99facu; }
RW_FUNCTION(0x6578A8, "ScenePartyTop::runInEntryLayout $_22 __func::target_type", t_6578A8);

// ScenePartyTop::runInEntryLayout $_22 UlvE_: __func::~__func (deleting)
static void t_6578BC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6578BC, "ScenePartyTop::runInEntryLayout $_22 UlvE_ __func::~__func (deleting)", t_6578BC);

// ScenePartyTop::runInEntryLayout $_22 UlvE_: __func::__clone()
static uint32_t t_6578C4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62104u);
    }
    return v1;
}
RW_FUNCTION(0x6578C4, "ScenePartyTop::runInEntryLayout $_22 UlvE_ __func::__clone()", t_6578C4);

// ScenePartyTop::runInEntryLayout $_22 UlvE_: __func::__clone(__base*)
static void t_6578E8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62104u);
    }
}
RW_FUNCTION(0x6578E8, "ScenePartyTop::runInEntryLayout $_22 UlvE_ __func::__clone(__base*)", t_6578E8);

// ScenePartyTop::runInEntryLayout $_22 UlvE_: __func::destroy
static void t_657904(uint32_t self) {
}
RW_FUNCTION(0x657904, "ScenePartyTop::runInEntryLayout $_22 UlvE_ __func::destroy", t_657904);

// ScenePartyTop::runInEntryLayout $_22 UlvE_: __func::destroy_deallocate
static void t_657906(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657906, "ScenePartyTop::runInEntryLayout $_22 UlvE_ __func::destroy_deallocate", t_657906);

// ScenePartyTop::runInEntryLayout $_22 UlvE_: __func::target
static uint32_t t_657A44(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61a30u ? self + 4 : 0; }
RW_FUNCTION(0x657A44, "ScenePartyTop::runInEntryLayout $_22 UlvE_ __func::target", t_657A44);

// ScenePartyTop::runInEntryLayout $_22 UlvE_: __func::target_type
static uint32_t t_657A64() { return 0xf99fc0u; }
RW_FUNCTION(0x657A64, "ScenePartyTop::runInEntryLayout $_22 UlvE_ __func::target_type", t_657A64);

// ScenePartyTop::runInEntryLayout $_21: __func::~__func (deleting)
static void t_657B78(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657B78, "ScenePartyTop::runInEntryLayout $_21 __func::~__func (deleting)", t_657B78);

// ScenePartyTop::runInEntryLayout $_21: __func::__clone()
static uint32_t t_657B80(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62130u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x657B80, "ScenePartyTop::runInEntryLayout $_21 __func::__clone()", t_657B80);

// ScenePartyTop::runInEntryLayout $_21: __func::__clone(__base*)
static void t_657BAC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62130u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x657BAC, "ScenePartyTop::runInEntryLayout $_21 __func::__clone(__base*)", t_657BAC);

// ScenePartyTop::runInEntryLayout $_21: __func::destroy
static void t_657BCC(uint32_t self) {
}
RW_FUNCTION(0x657BCC, "ScenePartyTop::runInEntryLayout $_21 __func::destroy", t_657BCC);

// ScenePartyTop::runInEntryLayout $_21: __func::destroy_deallocate
static void t_657BCE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657BCE, "ScenePartyTop::runInEntryLayout $_21 __func::destroy_deallocate", t_657BCE);

// ScenePartyTop::runInEntryLayout $_21: __func::target
static uint32_t t_657C80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61b30u ? self + 4 : 0; }
RW_FUNCTION(0x657C80, "ScenePartyTop::runInEntryLayout $_21 __func::target", t_657C80);

// ScenePartyTop::runInEntryLayout $_21: __func::target_type
static uint32_t t_657CA0() { return 0xf99fd4u; }
RW_FUNCTION(0x657CA0, "ScenePartyTop::runInEntryLayout $_21 __func::target_type", t_657CA0);

// ScenePartyTop::runInEntryLayout $_21 UlvE_: __func::~__func (deleting)
static void t_657CB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657CB4, "ScenePartyTop::runInEntryLayout $_21 UlvE_ __func::~__func (deleting)", t_657CB4);

// ScenePartyTop::runInEntryLayout $_21 UlvE_: __func::__clone()
static uint32_t t_657CBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6215cu);
    }
    return v1;
}
RW_FUNCTION(0x657CBC, "ScenePartyTop::runInEntryLayout $_21 UlvE_ __func::__clone()", t_657CBC);

// ScenePartyTop::runInEntryLayout $_21 UlvE_: __func::__clone(__base*)
static void t_657CE0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6215cu);
    }
}
RW_FUNCTION(0x657CE0, "ScenePartyTop::runInEntryLayout $_21 UlvE_ __func::__clone(__base*)", t_657CE0);

// ScenePartyTop::runInEntryLayout $_21 UlvE_: __func::destroy
static void t_657CFC(uint32_t self) {
}
RW_FUNCTION(0x657CFC, "ScenePartyTop::runInEntryLayout $_21 UlvE_ __func::destroy", t_657CFC);

// ScenePartyTop::runInEntryLayout $_21 UlvE_: __func::destroy_deallocate
static void t_657CFE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657CFE, "ScenePartyTop::runInEntryLayout $_21 UlvE_ __func::destroy_deallocate", t_657CFE);

// ScenePartyTop::runInEntryLayout $_21 UlvE_: __func::target
static uint32_t t_657DB4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61c00u ? self + 4 : 0; }
RW_FUNCTION(0x657DB4, "ScenePartyTop::runInEntryLayout $_21 UlvE_ __func::target", t_657DB4);

// ScenePartyTop::runInEntryLayout $_21 UlvE_: __func::target_type
static uint32_t t_657DD4() { return 0xf99fe8u; }
RW_FUNCTION(0x657DD4, "ScenePartyTop::runInEntryLayout $_21 UlvE_ __func::target_type", t_657DD4);

// ScenePartyTop::runInJoinLayout $_20: __func::~__func (deleting)
static void t_657E90(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657E90, "ScenePartyTop::runInJoinLayout $_20 __func::~__func (deleting)", t_657E90);

// ScenePartyTop::runInJoinLayout $_20: __func::__clone()
static uint32_t t_657E98(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62188u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x657E98, "ScenePartyTop::runInJoinLayout $_20 __func::__clone()", t_657E98);

// ScenePartyTop::runInJoinLayout $_20: __func::__clone(__base*)
static void t_657EC4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62188u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x657EC4, "ScenePartyTop::runInJoinLayout $_20 __func::__clone(__base*)", t_657EC4);

// ScenePartyTop::runInJoinLayout $_20: __func::destroy
static void t_657EE4(uint32_t self) {
}
RW_FUNCTION(0x657EE4, "ScenePartyTop::runInJoinLayout $_20 __func::destroy", t_657EE4);

// ScenePartyTop::runInJoinLayout $_20: __func::destroy_deallocate
static void t_657EE6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x657EE6, "ScenePartyTop::runInJoinLayout $_20 __func::destroy_deallocate", t_657EE6);

// ScenePartyTop::runInJoinLayout $_20: __func::target
static uint32_t t_657FE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61d00u ? self + 4 : 0; }
RW_FUNCTION(0x657FE4, "ScenePartyTop::runInJoinLayout $_20 __func::target", t_657FE4);

// ScenePartyTop::runInJoinLayout $_20: __func::target_type
static uint32_t t_658004() { return 0xf99ffcu; }
RW_FUNCTION(0x658004, "ScenePartyTop::runInJoinLayout $_20 __func::target_type", t_658004);

// ScenePartyTop::runInJoinLayout $_20 UlvE_: __func::~__func (deleting)
static void t_658018(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658018, "ScenePartyTop::runInJoinLayout $_20 UlvE_ __func::~__func (deleting)", t_658018);

// ScenePartyTop::runInJoinLayout $_20 UlvE_: __func::__clone()
static uint32_t t_658020(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf621b4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x658020, "ScenePartyTop::runInJoinLayout $_20 UlvE_ __func::__clone()", t_658020);

// ScenePartyTop::runInJoinLayout $_20 UlvE_: __func::__clone(__base*)
static void t_65804C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf621b4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65804C, "ScenePartyTop::runInJoinLayout $_20 UlvE_ __func::__clone(__base*)", t_65804C);

// ScenePartyTop::runInJoinLayout $_20 UlvE_: __func::destroy
static void t_65806C(uint32_t self) {
}
RW_FUNCTION(0x65806C, "ScenePartyTop::runInJoinLayout $_20 UlvE_ __func::destroy", t_65806C);

// ScenePartyTop::runInJoinLayout $_20 UlvE_: __func::destroy_deallocate
static void t_65806E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65806E, "ScenePartyTop::runInJoinLayout $_20 UlvE_ __func::destroy_deallocate", t_65806E);

// ScenePartyTop::runInJoinLayout $_20 UlvE_: __func::target
static uint32_t t_658088(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61dd0u ? self + 4 : 0; }
RW_FUNCTION(0x658088, "ScenePartyTop::runInJoinLayout $_20 UlvE_ __func::target", t_658088);

// ScenePartyTop::runInJoinLayout $_20 UlvE_: __func::target_type
static uint32_t t_6580A8() { return 0xf9a010u; }
RW_FUNCTION(0x6580A8, "ScenePartyTop::runInJoinLayout $_20 UlvE_ __func::target_type", t_6580A8);

// ScenePartyTop::runInJoinLayout $_19: __func::~__func (deleting)
static void t_6580BC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6580BC, "ScenePartyTop::runInJoinLayout $_19 __func::~__func (deleting)", t_6580BC);

// ScenePartyTop::runInJoinLayout $_19: __func::__clone()
static uint32_t t_6580C4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf621e0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6580C4, "ScenePartyTop::runInJoinLayout $_19 __func::__clone()", t_6580C4);

// ScenePartyTop::runInJoinLayout $_19: __func::__clone(__base*)
static void t_6580F0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf621e0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6580F0, "ScenePartyTop::runInJoinLayout $_19 __func::__clone(__base*)", t_6580F0);

// ScenePartyTop::runInJoinLayout $_19: __func::destroy
static void t_658110(uint32_t self) {
}
RW_FUNCTION(0x658110, "ScenePartyTop::runInJoinLayout $_19 __func::destroy", t_658110);

// ScenePartyTop::runInJoinLayout $_19: __func::destroy_deallocate
static void t_658112(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658112, "ScenePartyTop::runInJoinLayout $_19 __func::destroy_deallocate", t_658112);

// ScenePartyTop::runInJoinLayout $_19: __func::target
static uint32_t t_6581C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61ed0u ? self + 4 : 0; }
RW_FUNCTION(0x6581C8, "ScenePartyTop::runInJoinLayout $_19 __func::target", t_6581C8);

// ScenePartyTop::runInJoinLayout $_19: __func::target_type
static uint32_t t_6581E8() { return 0xf9a024u; }
RW_FUNCTION(0x6581E8, "ScenePartyTop::runInJoinLayout $_19 __func::target_type", t_6581E8);

// ScenePartyTop::runInJoinLayout $_26 UlvE_: __func::~__func (deleting)
static void t_6581FC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6581FC, "ScenePartyTop::runInJoinLayout $_26 UlvE_ __func::~__func (deleting)", t_6581FC);

// ScenePartyTop::runInJoinLayout $_26 UlvE_: __func::__clone()
static uint32_t t_658204(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6220cu);
    }
    return v1;
}
RW_FUNCTION(0x658204, "ScenePartyTop::runInJoinLayout $_26 UlvE_ __func::__clone()", t_658204);

// ScenePartyTop::runInJoinLayout $_26 UlvE_: __func::__clone(__base*)
static void t_658228(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6220cu);
    }
}
RW_FUNCTION(0x658228, "ScenePartyTop::runInJoinLayout $_26 UlvE_ __func::__clone(__base*)", t_658228);

// ScenePartyTop::runInJoinLayout $_26 UlvE_: __func::destroy
static void t_658244(uint32_t self) {
}
RW_FUNCTION(0x658244, "ScenePartyTop::runInJoinLayout $_26 UlvE_ __func::destroy", t_658244);

// ScenePartyTop::runInJoinLayout $_26 UlvE_: __func::destroy_deallocate
static void t_658246(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658246, "ScenePartyTop::runInJoinLayout $_26 UlvE_ __func::destroy_deallocate", t_658246);

// ScenePartyTop::runInJoinLayout $_26 UlvE_: __func::target
static uint32_t t_6582FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe61f70u ? self + 4 : 0; }
RW_FUNCTION(0x6582FC, "ScenePartyTop::runInJoinLayout $_26 UlvE_ __func::target", t_6582FC);

// ScenePartyTop::runInJoinLayout $_26 UlvE_: __func::target_type
static uint32_t t_65831C() { return 0xf9a038u; }
RW_FUNCTION(0x65831C, "ScenePartyTop::runInJoinLayout $_26 UlvE_ __func::target_type", t_65831C);

// ScenePartyTop::runInJoinLayout $_18: __func::~__func (deleting)
static void t_658330(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658330, "ScenePartyTop::runInJoinLayout $_18 __func::~__func (deleting)", t_658330);

// ScenePartyTop::runInJoinLayout $_18: __func::__clone()
static uint32_t t_658338(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62238u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x658338, "ScenePartyTop::runInJoinLayout $_18 __func::__clone()", t_658338);

// ScenePartyTop::runInJoinLayout $_18: __func::__clone(__base*)
static void t_658364(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62238u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x658364, "ScenePartyTop::runInJoinLayout $_18 __func::__clone(__base*)", t_658364);

// ScenePartyTop::runInJoinLayout $_18: __func::destroy
static void t_658384(uint32_t self) {
}
RW_FUNCTION(0x658384, "ScenePartyTop::runInJoinLayout $_18 __func::destroy", t_658384);

// ScenePartyTop::runInJoinLayout $_18: __func::destroy_deallocate
static void t_658386(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658386, "ScenePartyTop::runInJoinLayout $_18 __func::destroy_deallocate", t_658386);

// ScenePartyTop::runInJoinLayout $_18: __func::target
static uint32_t t_658484(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62040u ? self + 4 : 0; }
RW_FUNCTION(0x658484, "ScenePartyTop::runInJoinLayout $_18 __func::target", t_658484);

// ScenePartyTop::runInJoinLayout $_18: __func::target_type
static uint32_t t_6584A4() { return 0xf9a04cu; }
RW_FUNCTION(0x6584A4, "ScenePartyTop::runInJoinLayout $_18 __func::target_type", t_6584A4);

// ScenePartyTop::runInJoinLayout $_18 UlvE_: __func::~__func (deleting)
static void t_6584B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6584B8, "ScenePartyTop::runInJoinLayout $_18 UlvE_ __func::~__func (deleting)", t_6584B8);

// ScenePartyTop::runInJoinLayout $_18 UlvE_: __func::__clone()
static uint32_t t_6584C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62264u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6584C0, "ScenePartyTop::runInJoinLayout $_18 UlvE_ __func::__clone()", t_6584C0);

// ScenePartyTop::runInJoinLayout $_18 UlvE_: __func::__clone(__base*)
static void t_6584EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62264u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6584EC, "ScenePartyTop::runInJoinLayout $_18 UlvE_ __func::__clone(__base*)", t_6584EC);

// ScenePartyTop::runInJoinLayout $_18 UlvE_: __func::destroy
static void t_65850C(uint32_t self) {
}
RW_FUNCTION(0x65850C, "ScenePartyTop::runInJoinLayout $_18 UlvE_ __func::destroy", t_65850C);

// ScenePartyTop::runInJoinLayout $_18 UlvE_: __func::destroy_deallocate
static void t_65850E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65850E, "ScenePartyTop::runInJoinLayout $_18 UlvE_ __func::destroy_deallocate", t_65850E);

// ScenePartyTop::runInJoinLayout $_18 UlvE_: __func::target
static uint32_t t_658528(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62110u ? self + 4 : 0; }
RW_FUNCTION(0x658528, "ScenePartyTop::runInJoinLayout $_18 UlvE_ __func::target", t_658528);

// ScenePartyTop::runInJoinLayout $_18 UlvE_: __func::target_type
static uint32_t t_658548() { return 0xf9a060u; }
RW_FUNCTION(0x658548, "ScenePartyTop::runInJoinLayout $_18 UlvE_ __func::target_type", t_658548);

// ScenePartyTop::runInJoinLayout $_17: __func::~__func (deleting)
static void t_65855C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65855C, "ScenePartyTop::runInJoinLayout $_17 __func::~__func (deleting)", t_65855C);

// ScenePartyTop::runInJoinLayout $_17: __func::__clone()
static uint32_t t_658564(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62290u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x658564, "ScenePartyTop::runInJoinLayout $_17 __func::__clone()", t_658564);

// ScenePartyTop::runInJoinLayout $_17: __func::__clone(__base*)
static void t_658590(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62290u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x658590, "ScenePartyTop::runInJoinLayout $_17 __func::__clone(__base*)", t_658590);

// ScenePartyTop::runInJoinLayout $_17: __func::destroy
static void t_6585B0(uint32_t self) {
}
RW_FUNCTION(0x6585B0, "ScenePartyTop::runInJoinLayout $_17 __func::destroy", t_6585B0);

// ScenePartyTop::runInJoinLayout $_17: __func::destroy_deallocate
static void t_6585B2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6585B2, "ScenePartyTop::runInJoinLayout $_17 __func::destroy_deallocate", t_6585B2);

// ScenePartyTop::runInJoinLayout $_17: __func::target
static uint32_t t_658668(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62210u ? self + 4 : 0; }
RW_FUNCTION(0x658668, "ScenePartyTop::runInJoinLayout $_17 __func::target", t_658668);

// ScenePartyTop::runInJoinLayout $_17: __func::target_type
static uint32_t t_658688() { return 0xf9a074u; }
RW_FUNCTION(0x658688, "ScenePartyTop::runInJoinLayout $_17 __func::target_type", t_658688);

// ScenePartyTop::runInJoinLayout $_16: __func::~__func
static uint32_t t_65869C(uint32_t self) {
    st32(self, 0xf622bcu);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x65869C, "ScenePartyTop::runInJoinLayout $_16 __func::~__func", t_65869C);

// ScenePartyTop::runInJoinLayout $_16: __func::~__func (deleting)
static void t_6586C8(uint32_t self) {
    st32(self, 0xf622bcu);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6586C8, "ScenePartyTop::runInJoinLayout $_16 __func::~__func (deleting)", t_6586C8);

// ScenePartyTop::runInJoinLayout $_16: __func::__clone()
static uint32_t t_6586F8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf622bcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t m6 = v2;
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
            uint32_t v5 = ld32(self + 4);
            m6 = v5;
        }
        st32(v1 + 4, m6);
    }
    return v1;
}
RW_FUNCTION(0x6586F8, "ScenePartyTop::runInJoinLayout $_16 __func::__clone()", t_6586F8);

// ScenePartyTop::runInJoinLayout $_16: __func::__clone(__base*)
static void t_658740(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf622bcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 4);
        st32(p + 4, v4);
    }
}
RW_FUNCTION(0x658740, "ScenePartyTop::runInJoinLayout $_16 __func::__clone(__base*)", t_658740);

// ScenePartyTop::runInJoinLayout $_16: __func::destroy
static void t_65877C(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x65877C, "ScenePartyTop::runInJoinLayout $_16 __func::destroy", t_65877C);

// ScenePartyTop::runInJoinLayout $_16: __func::destroy_deallocate
static void t_65878A(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65878A, "ScenePartyTop::runInJoinLayout $_16 __func::destroy_deallocate", t_65878A);

// ScenePartyTop::runInJoinLayout $_16: __func::target
static uint32_t t_6588C0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe622a0u ? self + 4 : 0; }
RW_FUNCTION(0x6588C0, "ScenePartyTop::runInJoinLayout $_16 __func::target", t_6588C0);

// ScenePartyTop::runInJoinLayout $_16: __func::target_type
static uint32_t t_6588E0() { return 0xf9a088u; }
RW_FUNCTION(0x6588E0, "ScenePartyTop::runInJoinLayout $_16 __func::target_type", t_6588E0);

// ScenePartyTop::runInJoinLayout $_16 UlbE_: __func::~__func
static uint32_t t_6588F4(uint32_t self) {
    st32(self, 0xf622e8u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6588F4, "ScenePartyTop::runInJoinLayout $_16 UlbE_ __func::~__func", t_6588F4);

// ScenePartyTop::runInJoinLayout $_16 UlbE_: __func::~__func (deleting)
static void t_658920(uint32_t self) {
    st32(self, 0xf622e8u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658920, "ScenePartyTop::runInJoinLayout $_16 UlbE_ __func::~__func (deleting)", t_658920);

// ScenePartyTop::runInJoinLayout $_16 UlbE_: __func::__clone()
static uint32_t t_658950(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf622e8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t m6 = v2;
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
            uint32_t v5 = ld32(self + 4);
            m6 = v5;
        }
        st32(v1 + 4, m6);
    }
    return v1;
}
RW_FUNCTION(0x658950, "ScenePartyTop::runInJoinLayout $_16 UlbE_ __func::__clone()", t_658950);

// ScenePartyTop::runInJoinLayout $_16 UlbE_: __func::__clone(__base*)
static void t_658998(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf622e8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 4);
        st32(p + 4, v4);
    }
}
RW_FUNCTION(0x658998, "ScenePartyTop::runInJoinLayout $_16 UlbE_ __func::__clone(__base*)", t_658998);

// ScenePartyTop::runInJoinLayout $_16 UlbE_: __func::destroy
static void t_6589D4(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x6589D4, "ScenePartyTop::runInJoinLayout $_16 UlbE_ __func::destroy", t_6589D4);

// ScenePartyTop::runInJoinLayout $_16 UlbE_: __func::destroy_deallocate
static void t_6589E2(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6589E2, "ScenePartyTop::runInJoinLayout $_16 UlbE_ __func::destroy_deallocate", t_6589E2);

// ScenePartyTop::runInJoinLayout $_16 UlbE_: __func::target
static uint32_t t_658AD0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62340u ? self + 4 : 0; }
RW_FUNCTION(0x658AD0, "ScenePartyTop::runInJoinLayout $_16 UlbE_ __func::target", t_658AD0);

// ScenePartyTop::runInJoinLayout $_16 UlbE_: __func::target_type
static uint32_t t_658AF0() { return 0xf9a09cu; }
RW_FUNCTION(0x658AF0, "ScenePartyTop::runInJoinLayout $_16 UlbE_ __func::target_type", t_658AF0);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_: __func::~__func (deleting)
static void t_658B04(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658B04, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ __func::~__func (deleting)", t_658B04);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_: __func::__clone()
static uint32_t t_658B0C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62314u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x658B0C, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ __func::__clone()", t_658B0C);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_: __func::__clone(__base*)
static void t_658B38(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62314u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x658B38, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ __func::__clone(__base*)", t_658B38);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_: __func::destroy
static void t_658B58(uint32_t self) {
}
RW_FUNCTION(0x658B58, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ __func::destroy", t_658B58);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_: __func::destroy_deallocate
static void t_658B5A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658B5A, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ __func::destroy_deallocate", t_658B5A);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_: __func::target
static uint32_t t_658CDC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62400u ? self + 4 : 0; }
RW_FUNCTION(0x658CDC, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ __func::target", t_658CDC);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_: __func::target_type
static uint32_t t_658CFC() { return 0xf9a0b0u; }
RW_FUNCTION(0x658CFC, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ __func::target_type", t_658CFC);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_: __func::~__func (deleting)
static void t_658D10(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658D10, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ __func::~__func (deleting)", t_658D10);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_: __func::__clone()
static uint32_t t_658D18(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62340u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x658D18, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ __func::__clone()", t_658D18);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_: __func::__clone(__base*)
static void t_658D44(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62340u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x658D44, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ __func::__clone(__base*)", t_658D44);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_: __func::destroy
static void t_658D64(uint32_t self) {
}
RW_FUNCTION(0x658D64, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ __func::destroy", t_658D64);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_: __func::destroy_deallocate
static void t_658D66(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658D66, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ __func::destroy_deallocate", t_658D66);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_: __func::target
static uint32_t t_658E08(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe624e0u ? self + 4 : 0; }
RW_FUNCTION(0x658E08, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ __func::target", t_658E08);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_: __func::target_type
static uint32_t t_658E28() { return 0xf9a0c4u; }
RW_FUNCTION(0x658E28, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ __func::target_type", t_658E28);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_: __func::~__func (deleting)
static void t_658E3C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658E3C, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_ __func::~__func (deleting)", t_658E3C);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_: __func::__clone()
static uint32_t t_658E44(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6236cu);
    }
    return v1;
}
RW_FUNCTION(0x658E44, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_ __func::__clone()", t_658E44);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_: __func::__clone(__base*)
static void t_658E68(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6236cu);
    }
}
RW_FUNCTION(0x658E68, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_ __func::__clone(__base*)", t_658E68);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_: __func::destroy
static void t_658E84(uint32_t self) {
}
RW_FUNCTION(0x658E84, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_ __func::destroy", t_658E84);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_: __func::destroy_deallocate
static void t_658E86(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658E86, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_ __func::destroy_deallocate", t_658E86);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_: __func::target
static uint32_t t_658FB0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe625e0u ? self + 4 : 0; }
RW_FUNCTION(0x658FB0, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_ __func::target", t_658FB0);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_: __func::target_type
static uint32_t t_658FD0() { return 0xf9a0d8u; }
RW_FUNCTION(0x658FD0, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE0_ UlvE_ __func::target_type", t_658FD0);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_658FE4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x658FE4, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_ __func::~__func (deleting)", t_658FE4);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_658FEC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62398u);
    }
    return v1;
}
RW_FUNCTION(0x658FEC, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_ __func::__clone()", t_658FEC);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_659010(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62398u);
    }
}
RW_FUNCTION(0x659010, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_ __func::__clone(__base*)", t_659010);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_: __func::destroy
static void t_65902C(uint32_t self) {
}
RW_FUNCTION(0x65902C, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_ __func::destroy", t_65902C);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_65902E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65902E, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_ __func::destroy_deallocate", t_65902E);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_: __func::target
static uint32_t t_659038(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe626d0u ? self + 4 : 0; }
RW_FUNCTION(0x659038, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_ __func::target", t_659038);

// ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_659058() { return 0xf9a0ecu; }
RW_FUNCTION(0x659058, "ScenePartyTop::runInJoinLayout $_16 UlbE_ UlbE_ UlbE_ __func::target_type", t_659058);

// ScenePartyTop::runInJoinLayout $_15: __func::~__func (deleting)
static void t_65906C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65906C, "ScenePartyTop::runInJoinLayout $_15 __func::~__func (deleting)", t_65906C);

// ScenePartyTop::runInJoinLayout $_15: __func::__clone()
static uint32_t t_659074(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf623c4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x659074, "ScenePartyTop::runInJoinLayout $_15 __func::__clone()", t_659074);

// ScenePartyTop::runInJoinLayout $_15: __func::__clone(__base*)
static void t_6590A0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf623c4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6590A0, "ScenePartyTop::runInJoinLayout $_15 __func::__clone(__base*)", t_6590A0);

// ScenePartyTop::runInJoinLayout $_15: __func::destroy
static void t_6590C0(uint32_t self) {
}
RW_FUNCTION(0x6590C0, "ScenePartyTop::runInJoinLayout $_15 __func::destroy", t_6590C0);

// ScenePartyTop::runInJoinLayout $_15: __func::destroy_deallocate
static void t_6590C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6590C2, "ScenePartyTop::runInJoinLayout $_15 __func::destroy_deallocate", t_6590C2);

// ScenePartyTop::runInJoinLayout $_15: __func::target
static uint32_t t_6590DC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe627c0u ? self + 4 : 0; }
RW_FUNCTION(0x6590DC, "ScenePartyTop::runInJoinLayout $_15 __func::target", t_6590DC);

// ScenePartyTop::runInJoinLayout $_15: __func::target_type
static uint32_t t_6590FC() { return 0xf9a100u; }
RW_FUNCTION(0x6590FC, "ScenePartyTop::runInJoinLayout $_15 __func::target_type", t_6590FC);

// ScenePartyTop::runInJoinLayout $_14: __func::~__func (deleting)
static void t_659110(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659110, "ScenePartyTop::runInJoinLayout $_14 __func::~__func (deleting)", t_659110);

// ScenePartyTop::runInJoinLayout $_14: __func::__clone()
static uint32_t t_659118(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf623f0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x659118, "ScenePartyTop::runInJoinLayout $_14 __func::__clone()", t_659118);

// ScenePartyTop::runInJoinLayout $_14: __func::__clone(__base*)
static void t_659144(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf623f0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x659144, "ScenePartyTop::runInJoinLayout $_14 __func::__clone(__base*)", t_659144);

// ScenePartyTop::runInJoinLayout $_14: __func::destroy
static void t_659164(uint32_t self) {
}
RW_FUNCTION(0x659164, "ScenePartyTop::runInJoinLayout $_14 __func::destroy", t_659164);

// ScenePartyTop::runInJoinLayout $_14: __func::destroy_deallocate
static void t_659166(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659166, "ScenePartyTop::runInJoinLayout $_14 __func::destroy_deallocate", t_659166);

// ScenePartyTop::runInJoinLayout $_14: __func::target
static uint32_t t_659218(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62880u ? self + 4 : 0; }
RW_FUNCTION(0x659218, "ScenePartyTop::runInJoinLayout $_14 __func::target", t_659218);

// ScenePartyTop::runInJoinLayout $_14: __func::target_type
static uint32_t t_659238() { return 0xf9a114u; }
RW_FUNCTION(0x659238, "ScenePartyTop::runInJoinLayout $_14 __func::target_type", t_659238);

// ScenePartyTop::runInJoinLayout $_14 UlvE_: __func::~__func (deleting)
static void t_65924C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65924C, "ScenePartyTop::runInJoinLayout $_14 UlvE_ __func::~__func (deleting)", t_65924C);

// ScenePartyTop::runInJoinLayout $_14 UlvE_: __func::__clone()
static uint32_t t_659254(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6241cu);
    }
    return v1;
}
RW_FUNCTION(0x659254, "ScenePartyTop::runInJoinLayout $_14 UlvE_ __func::__clone()", t_659254);

// ScenePartyTop::runInJoinLayout $_14 UlvE_: __func::__clone(__base*)
static void t_659278(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6241cu);
    }
}
RW_FUNCTION(0x659278, "ScenePartyTop::runInJoinLayout $_14 UlvE_ __func::__clone(__base*)", t_659278);

// ScenePartyTop::runInJoinLayout $_14 UlvE_: __func::destroy
static void t_659294(uint32_t self) {
}
RW_FUNCTION(0x659294, "ScenePartyTop::runInJoinLayout $_14 UlvE_ __func::destroy", t_659294);

// ScenePartyTop::runInJoinLayout $_14 UlvE_: __func::destroy_deallocate
static void t_659296(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659296, "ScenePartyTop::runInJoinLayout $_14 UlvE_ __func::destroy_deallocate", t_659296);

// ScenePartyTop::runInJoinLayout $_14 UlvE_: __func::target
static uint32_t t_6593B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62950u ? self + 4 : 0; }
RW_FUNCTION(0x6593B0, "ScenePartyTop::runInJoinLayout $_14 UlvE_ __func::target", t_6593B0);

// ScenePartyTop::runInJoinLayout $_14 UlvE_: __func::target_type
static uint32_t t_6593D0() { return 0xf9a128u; }
RW_FUNCTION(0x6593D0, "ScenePartyTop::runInJoinLayout $_14 UlvE_ __func::target_type", t_6593D0);

// ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_: __func::~__func (deleting)
static void t_6593E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6593E4, "ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_ __func::~__func (deleting)", t_6593E4);

// ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_: __func::__clone()
static uint32_t t_6593EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62448u);
    }
    return v1;
}
RW_FUNCTION(0x6593EC, "ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_ __func::__clone()", t_6593EC);

// ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_: __func::__clone(__base*)
static void t_659410(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62448u);
    }
}
RW_FUNCTION(0x659410, "ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_ __func::__clone(__base*)", t_659410);

// ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_: __func::destroy
static void t_65942C(uint32_t self) {
}
RW_FUNCTION(0x65942C, "ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_ __func::destroy", t_65942C);

// ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_: __func::destroy_deallocate
static void t_65942E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65942E, "ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_ __func::destroy_deallocate", t_65942E);

// ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_: __func::target
static uint32_t t_65944C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62a80u ? self + 4 : 0; }
RW_FUNCTION(0x65944C, "ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_ __func::target", t_65944C);

// ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_: __func::target_type
static uint32_t t_65946C() { return 0xf9a13cu; }
RW_FUNCTION(0x65946C, "ScenePartyTop::runInJoinLayout $_14 UlvE_ UlvE_ __func::target_type", t_65946C);

// ScenePartyTop::runInJoinLayout $_13: __func::~__func (deleting)
static void t_659480(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659480, "ScenePartyTop::runInJoinLayout $_13 __func::~__func (deleting)", t_659480);

// ScenePartyTop::runInJoinLayout $_13: __func::__clone()
static uint32_t t_659488(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf62474u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x659488, "ScenePartyTop::runInJoinLayout $_13 __func::__clone()", t_659488);

// ScenePartyTop::runInJoinLayout $_13: __func::__clone(__base*)
static void t_6594B8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62474u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6594B8, "ScenePartyTop::runInJoinLayout $_13 __func::__clone(__base*)", t_6594B8);

// ScenePartyTop::runInJoinLayout $_13: __func::destroy
static void t_6594DC(uint32_t self) {
}
RW_FUNCTION(0x6594DC, "ScenePartyTop::runInJoinLayout $_13 __func::destroy", t_6594DC);

// ScenePartyTop::runInJoinLayout $_13: __func::destroy_deallocate
static void t_6594DE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6594DE, "ScenePartyTop::runInJoinLayout $_13 __func::destroy_deallocate", t_6594DE);

// ScenePartyTop::runInJoinLayout $_13: __func::target
static uint32_t t_65959C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62b90u ? self + 4 : 0; }
RW_FUNCTION(0x65959C, "ScenePartyTop::runInJoinLayout $_13 __func::target", t_65959C);

// ScenePartyTop::runInJoinLayout $_13: __func::target_type
static uint32_t t_6595BC() { return 0xf9a150u; }
RW_FUNCTION(0x6595BC, "ScenePartyTop::runInJoinLayout $_13 __func::target_type", t_6595BC);

// ScenePartyTop::runInJoinLayout $_13 UlvE_: __func::~__func (deleting)
static void t_6595D0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6595D0, "ScenePartyTop::runInJoinLayout $_13 UlvE_ __func::~__func (deleting)", t_6595D0);

// ScenePartyTop::runInJoinLayout $_13 UlvE_: __func::__clone()
static uint32_t t_6595D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf624a0u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6595D8, "ScenePartyTop::runInJoinLayout $_13 UlvE_ __func::__clone()", t_6595D8);

// ScenePartyTop::runInJoinLayout $_13 UlvE_: __func::__clone(__base*)
static void t_659604(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf624a0u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x659604, "ScenePartyTop::runInJoinLayout $_13 UlvE_ __func::__clone(__base*)", t_659604);

// ScenePartyTop::runInJoinLayout $_13 UlvE_: __func::destroy
static void t_659624(uint32_t self) {
}
RW_FUNCTION(0x659624, "ScenePartyTop::runInJoinLayout $_13 UlvE_ __func::destroy", t_659624);

// ScenePartyTop::runInJoinLayout $_13 UlvE_: __func::destroy_deallocate
static void t_659626(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659626, "ScenePartyTop::runInJoinLayout $_13 UlvE_ __func::destroy_deallocate", t_659626);

// ScenePartyTop::runInJoinLayout $_13 UlvE_: __func::target
static uint32_t t_6597B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62c60u ? self + 4 : 0; }
RW_FUNCTION(0x6597B0, "ScenePartyTop::runInJoinLayout $_13 UlvE_ __func::target", t_6597B0);

// ScenePartyTop::runInJoinLayout $_13 UlvE_: __func::target_type
static uint32_t t_6597D0() { return 0xf9a164u; }
RW_FUNCTION(0x6597D0, "ScenePartyTop::runInJoinLayout $_13 UlvE_ __func::target_type", t_6597D0);

// ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6597E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6597E4, "ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_ __func::~__func (deleting)", t_6597E4);

// ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6597EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf624ccu);
    }
    return v1;
}
RW_FUNCTION(0x6597EC, "ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_ __func::__clone()", t_6597EC);

// ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_: __func::__clone(__base*)
static void t_659810(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf624ccu);
    }
}
RW_FUNCTION(0x659810, "ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_ __func::__clone(__base*)", t_659810);

// ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_: __func::destroy
static void t_65982C(uint32_t self) {
}
RW_FUNCTION(0x65982C, "ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_ __func::destroy", t_65982C);

// ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_: __func::destroy_deallocate
static void t_65982E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65982E, "ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_ __func::destroy_deallocate", t_65982E);

// ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_: __func::target
static uint32_t t_659924(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62d80u ? self + 4 : 0; }
RW_FUNCTION(0x659924, "ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_ __func::target", t_659924);

// ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_: __func::target_type
static uint32_t t_659944() { return 0xf9a178u; }
RW_FUNCTION(0x659944, "ScenePartyTop::runInJoinLayout $_13 UlvE_ UlbE_ __func::target_type", t_659944);

// ScenePartyTop::runInJoinLayout $_12: __func::~__func (deleting)
static void t_659958(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659958, "ScenePartyTop::runInJoinLayout $_12 __func::~__func (deleting)", t_659958);

// ScenePartyTop::runInJoinLayout $_12: __func::__clone()
static uint32_t t_659960(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf624f8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x659960, "ScenePartyTop::runInJoinLayout $_12 __func::__clone()", t_659960);

// ScenePartyTop::runInJoinLayout $_12: __func::__clone(__base*)
static void t_65998C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf624f8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65998C, "ScenePartyTop::runInJoinLayout $_12 __func::__clone(__base*)", t_65998C);

// ScenePartyTop::runInJoinLayout $_12: __func::destroy
static void t_6599AC(uint32_t self) {
}
RW_FUNCTION(0x6599AC, "ScenePartyTop::runInJoinLayout $_12 __func::destroy", t_6599AC);

// ScenePartyTop::runInJoinLayout $_12: __func::destroy_deallocate
static void t_6599AE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6599AE, "ScenePartyTop::runInJoinLayout $_12 __func::destroy_deallocate", t_6599AE);

// ScenePartyTop::runInJoinLayout $_12: __func::target
static uint32_t t_659A60(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62e90u ? self + 4 : 0; }
RW_FUNCTION(0x659A60, "ScenePartyTop::runInJoinLayout $_12 __func::target", t_659A60);

// ScenePartyTop::runInJoinLayout $_12: __func::target_type
static uint32_t t_659A80() { return 0xf9a18cu; }
RW_FUNCTION(0x659A80, "ScenePartyTop::runInJoinLayout $_12 __func::target_type", t_659A80);

// ScenePartyTop::runInJoinLayout $_12 UlvE_: __func::~__func (deleting)
static void t_659A94(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659A94, "ScenePartyTop::runInJoinLayout $_12 UlvE_ __func::~__func (deleting)", t_659A94);

// ScenePartyTop::runInJoinLayout $_12 UlvE_: __func::__clone()
static uint32_t t_659A9C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62524u);
    }
    return v1;
}
RW_FUNCTION(0x659A9C, "ScenePartyTop::runInJoinLayout $_12 UlvE_ __func::__clone()", t_659A9C);

// ScenePartyTop::runInJoinLayout $_12 UlvE_: __func::__clone(__base*)
static void t_659AC0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62524u);
    }
}
RW_FUNCTION(0x659AC0, "ScenePartyTop::runInJoinLayout $_12 UlvE_ __func::__clone(__base*)", t_659AC0);

// ScenePartyTop::runInJoinLayout $_12 UlvE_: __func::destroy
static void t_659ADC(uint32_t self) {
}
RW_FUNCTION(0x659ADC, "ScenePartyTop::runInJoinLayout $_12 UlvE_ __func::destroy", t_659ADC);

// ScenePartyTop::runInJoinLayout $_12 UlvE_: __func::destroy_deallocate
static void t_659ADE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659ADE, "ScenePartyTop::runInJoinLayout $_12 UlvE_ __func::destroy_deallocate", t_659ADE);

// ScenePartyTop::runInJoinLayout $_12 UlvE_: __func::target
static uint32_t t_659BD8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe62f60u ? self + 4 : 0; }
RW_FUNCTION(0x659BD8, "ScenePartyTop::runInJoinLayout $_12 UlvE_ __func::target", t_659BD8);

// ScenePartyTop::runInJoinLayout $_12 UlvE_: __func::target_type
static uint32_t t_659BF8() { return 0xf9a1a0u; }
RW_FUNCTION(0x659BF8, "ScenePartyTop::runInJoinLayout $_12 UlvE_ __func::target_type", t_659BF8);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_: __func::~__func (deleting)
static void t_659C0C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659C0C, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ __func::~__func (deleting)", t_659C0C);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_: __func::__clone()
static uint32_t t_659C14(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62550u);
    }
    return v1;
}
RW_FUNCTION(0x659C14, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ __func::__clone()", t_659C14);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_: __func::__clone(__base*)
static void t_659C38(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62550u);
    }
}
RW_FUNCTION(0x659C38, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ __func::__clone(__base*)", t_659C38);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_: __func::destroy
static void t_659C54(uint32_t self) {
}
RW_FUNCTION(0x659C54, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ __func::destroy", t_659C54);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_: __func::destroy_deallocate
static void t_659C56(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659C56, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ __func::destroy_deallocate", t_659C56);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_: __func::target
static uint32_t t_659E30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe63080u ? self + 4 : 0; }
RW_FUNCTION(0x659E30, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ __func::target", t_659E30);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_: __func::target_type
static uint32_t t_659E50() { return 0xf9a1b4u; }
RW_FUNCTION(0x659E50, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ __func::target_type", t_659E50);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_659EF0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659EF0, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_659EF0);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_659EF8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6257cu);
    }
    return v1;
}
RW_FUNCTION(0x659EF8, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::__clone()", t_659EF8);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_659F1C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6257cu);
    }
}
RW_FUNCTION(0x659F1C, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_659F1C);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_659F38(uint32_t self) {
}
RW_FUNCTION(0x659F38, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::destroy", t_659F38);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_659F3A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659F3A, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_659F3A);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_659F50(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe631c0u ? self + 4 : 0; }
RW_FUNCTION(0x659F50, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::target", t_659F50);

// ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_659F70() { return 0xf9a1c8u; }
RW_FUNCTION(0x659F70, "ScenePartyTop::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::target_type", t_659F70);

// ScenePartyTop::runInJoinLayout $_11: __func::~__func (deleting)
static void t_659F84(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659F84, "ScenePartyTop::runInJoinLayout $_11 __func::~__func (deleting)", t_659F84);

// ScenePartyTop::runInJoinLayout $_11: __func::__clone()
static uint32_t t_659F8C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf625a8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x659F8C, "ScenePartyTop::runInJoinLayout $_11 __func::__clone()", t_659F8C);

// ScenePartyTop::runInJoinLayout $_11: __func::__clone(__base*)
static void t_659FB8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf625a8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x659FB8, "ScenePartyTop::runInJoinLayout $_11 __func::__clone(__base*)", t_659FB8);

// ScenePartyTop::runInJoinLayout $_11: __func::destroy
static void t_659FD8(uint32_t self) {
}
RW_FUNCTION(0x659FD8, "ScenePartyTop::runInJoinLayout $_11 __func::destroy", t_659FD8);

// ScenePartyTop::runInJoinLayout $_11: __func::destroy_deallocate
static void t_659FDA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x659FDA, "ScenePartyTop::runInJoinLayout $_11 __func::destroy_deallocate", t_659FDA);

// ScenePartyTop::runInJoinLayout $_11: __func::target
static uint32_t t_65A08C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe632d0u ? self + 4 : 0; }
RW_FUNCTION(0x65A08C, "ScenePartyTop::runInJoinLayout $_11 __func::target", t_65A08C);

// ScenePartyTop::runInJoinLayout $_11: __func::target_type
static uint32_t t_65A0AC() { return 0xf9a1dcu; }
RW_FUNCTION(0x65A0AC, "ScenePartyTop::runInJoinLayout $_11 __func::target_type", t_65A0AC);

// ScenePartyTop::runInJoinLayout $_11 UlvE_: __func::~__func (deleting)
static void t_65A0C0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A0C0, "ScenePartyTop::runInJoinLayout $_11 UlvE_ __func::~__func (deleting)", t_65A0C0);

// ScenePartyTop::runInJoinLayout $_11 UlvE_: __func::__clone()
static uint32_t t_65A0C8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf625d4u);
    }
    return v1;
}
RW_FUNCTION(0x65A0C8, "ScenePartyTop::runInJoinLayout $_11 UlvE_ __func::__clone()", t_65A0C8);

// ScenePartyTop::runInJoinLayout $_11 UlvE_: __func::__clone(__base*)
static void t_65A0EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf625d4u);
    }
}
RW_FUNCTION(0x65A0EC, "ScenePartyTop::runInJoinLayout $_11 UlvE_ __func::__clone(__base*)", t_65A0EC);

// ScenePartyTop::runInJoinLayout $_11 UlvE_: __func::destroy
static void t_65A108(uint32_t self) {
}
RW_FUNCTION(0x65A108, "ScenePartyTop::runInJoinLayout $_11 UlvE_ __func::destroy", t_65A108);

// ScenePartyTop::runInJoinLayout $_11 UlvE_: __func::destroy_deallocate
static void t_65A10A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A10A, "ScenePartyTop::runInJoinLayout $_11 UlvE_ __func::destroy_deallocate", t_65A10A);

// ScenePartyTop::runInJoinLayout $_11 UlvE_: __func::target
static uint32_t t_65A250(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe633a0u ? self + 4 : 0; }
RW_FUNCTION(0x65A250, "ScenePartyTop::runInJoinLayout $_11 UlvE_ __func::target", t_65A250);

// ScenePartyTop::runInJoinLayout $_11 UlvE_: __func::target_type
static uint32_t t_65A270() { return 0xf9a1f0u; }
RW_FUNCTION(0x65A270, "ScenePartyTop::runInJoinLayout $_11 UlvE_ __func::target_type", t_65A270);

// ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_: __func::~__func (deleting)
static void t_65A284(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A284, "ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_ __func::~__func (deleting)", t_65A284);

// ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_: __func::__clone()
static uint32_t t_65A28C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62600u);
    }
    return v1;
}
RW_FUNCTION(0x65A28C, "ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_ __func::__clone()", t_65A28C);

// ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_: __func::__clone(__base*)
static void t_65A2B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62600u);
    }
}
RW_FUNCTION(0x65A2B0, "ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_ __func::__clone(__base*)", t_65A2B0);

// ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_: __func::destroy
static void t_65A2CC(uint32_t self) {
}
RW_FUNCTION(0x65A2CC, "ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_ __func::destroy", t_65A2CC);

// ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_: __func::destroy_deallocate
static void t_65A2CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A2CE, "ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_ __func::destroy_deallocate", t_65A2CE);

// ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_: __func::target
static uint32_t t_65A3C4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe634c0u ? self + 4 : 0; }
RW_FUNCTION(0x65A3C4, "ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_ __func::target", t_65A3C4);

// ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_: __func::target_type
static uint32_t t_65A3E4() { return 0xf9a204u; }
RW_FUNCTION(0x65A3E4, "ScenePartyTop::runInJoinLayout $_11 UlvE_ UlbE_ __func::target_type", t_65A3E4);

// ScenePartyTop::init $_9: __func::~__func (deleting)
static void t_65A3F8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A3F8, "ScenePartyTop::init $_9 __func::~__func (deleting)", t_65A3F8);

// ScenePartyTop::init $_9: __func::__clone()
static uint32_t t_65A400(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6262cu);
    }
    return v1;
}
RW_FUNCTION(0x65A400, "ScenePartyTop::init $_9 __func::__clone()", t_65A400);

// ScenePartyTop::init $_9: __func::__clone(__base*)
static void t_65A424(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6262cu);
    }
}
RW_FUNCTION(0x65A424, "ScenePartyTop::init $_9 __func::__clone(__base*)", t_65A424);

// ScenePartyTop::init $_9: __func::destroy
static void t_65A440(uint32_t self) {
}
RW_FUNCTION(0x65A440, "ScenePartyTop::init $_9 __func::destroy", t_65A440);

// ScenePartyTop::init $_9: __func::destroy_deallocate
static void t_65A442(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A442, "ScenePartyTop::init $_9 __func::destroy_deallocate", t_65A442);

// ScenePartyTop::init $_9: __func::target
static uint32_t t_65A44C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe635a0u ? self + 4 : 0; }
RW_FUNCTION(0x65A44C, "ScenePartyTop::init $_9 __func::target", t_65A44C);

// ScenePartyTop::init $_9: __func::target_type
static uint32_t t_65A46C() { return 0xf9a218u; }
RW_FUNCTION(0x65A46C, "ScenePartyTop::init $_9 __func::target_type", t_65A46C);

// ScenePartyTop::init $_8: __func::~__func (deleting)
static void t_65A480(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A480, "ScenePartyTop::init $_8 __func::~__func (deleting)", t_65A480);

// ScenePartyTop::init $_8: __func::__clone()
static uint32_t t_65A488(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62658u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x65A488, "ScenePartyTop::init $_8 __func::__clone()", t_65A488);

// ScenePartyTop::init $_8: __func::__clone(__base*)
static void t_65A4B4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62658u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65A4B4, "ScenePartyTop::init $_8 __func::__clone(__base*)", t_65A4B4);

// ScenePartyTop::init $_8: __func::destroy
static void t_65A4D4(uint32_t self) {
}
RW_FUNCTION(0x65A4D4, "ScenePartyTop::init $_8 __func::destroy", t_65A4D4);

// ScenePartyTop::init $_8: __func::destroy_deallocate
static void t_65A4D6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A4D6, "ScenePartyTop::init $_8 __func::destroy_deallocate", t_65A4D6);

// ScenePartyTop::init $_8: __func::target
static uint32_t t_65A4E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe63620u ? self + 4 : 0; }
RW_FUNCTION(0x65A4E8, "ScenePartyTop::init $_8 __func::target", t_65A4E8);

// ScenePartyTop::init $_8: __func::target_type
static uint32_t t_65A508() { return 0xf9a22cu; }
RW_FUNCTION(0x65A508, "ScenePartyTop::init $_8 __func::target_type", t_65A508);

// ScenePartyTop::init $_7: __func::~__func (deleting)
static void t_65A51C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A51C, "ScenePartyTop::init $_7 __func::~__func (deleting)", t_65A51C);

// ScenePartyTop::init $_7: __func::__clone()
static uint32_t t_65A524(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62684u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x65A524, "ScenePartyTop::init $_7 __func::__clone()", t_65A524);

// ScenePartyTop::init $_7: __func::__clone(__base*)
static void t_65A550(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62684u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65A550, "ScenePartyTop::init $_7 __func::__clone(__base*)", t_65A550);

// ScenePartyTop::init $_7: __func::destroy
static void t_65A570(uint32_t self) {
}
RW_FUNCTION(0x65A570, "ScenePartyTop::init $_7 __func::destroy", t_65A570);

// ScenePartyTop::init $_7: __func::destroy_deallocate
static void t_65A572(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A572, "ScenePartyTop::init $_7 __func::destroy_deallocate", t_65A572);

// ScenePartyTop::init $_7: __func::target
static uint32_t t_65A584(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe636a0u ? self + 4 : 0; }
RW_FUNCTION(0x65A584, "ScenePartyTop::init $_7 __func::target", t_65A584);

// ScenePartyTop::init $_7: __func::target_type
static uint32_t t_65A5A4() { return 0xf9a240u; }
RW_FUNCTION(0x65A5A4, "ScenePartyTop::init $_7 __func::target_type", t_65A5A4);

// ScenePartyTop::init $_6: __func::~__func (deleting)
static void t_65A5B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A5B8, "ScenePartyTop::init $_6 __func::~__func (deleting)", t_65A5B8);

// ScenePartyTop::init $_6: __func::__clone()
static uint32_t t_65A5C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf626b0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x65A5C0, "ScenePartyTop::init $_6 __func::__clone()", t_65A5C0);

// ScenePartyTop::init $_6: __func::__clone(__base*)
static void t_65A5EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf626b0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65A5EC, "ScenePartyTop::init $_6 __func::__clone(__base*)", t_65A5EC);

// ScenePartyTop::init $_6: __func::destroy
static void t_65A60C(uint32_t self) {
}
RW_FUNCTION(0x65A60C, "ScenePartyTop::init $_6 __func::destroy", t_65A60C);

// ScenePartyTop::init $_6: __func::destroy_deallocate
static void t_65A60E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A60E, "ScenePartyTop::init $_6 __func::destroy_deallocate", t_65A60E);

// ScenePartyTop::init $_6: __func::target
static uint32_t t_65A620(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe63720u ? self + 4 : 0; }
RW_FUNCTION(0x65A620, "ScenePartyTop::init $_6 __func::target", t_65A620);

// ScenePartyTop::init $_6: __func::target_type
static uint32_t t_65A640() { return 0xf9a254u; }
RW_FUNCTION(0x65A640, "ScenePartyTop::init $_6 __func::target_type", t_65A640);

// ScenePartyTop::callPartyTopAPI $_5: __func::~__func
static uint32_t t_65A654(uint32_t self) {
    st32(self, 0xf626dcu);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    return self;
}
RW_FUNCTION(0x65A654, "ScenePartyTop::callPartyTopAPI $_5 __func::~__func", t_65A654);

// ScenePartyTop::callPartyTopAPI $_5: __func::~__func (deleting)
static void t_65A690(uint32_t self) {
    st32(self, 0xf626dcu);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A690, "ScenePartyTop::callPartyTopAPI $_5 __func::~__func (deleting)", t_65A690);

// ScenePartyTop::callPartyTopAPI $_5: __func::__clone()
static uint32_t t_65A6D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {64});
    if (v1) {
        st32(v1, 0xf626dcu);
        call(0x45c84d, {v1 + 40, self + 40});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x65A6D0, "ScenePartyTop::callPartyTopAPI $_5 __func::__clone()", t_65A6D0);

// ScenePartyTop::callPartyTopAPI $_5: __func::__clone(__base*)
static void t_65A720(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf626dcu);
        call(0x45c84d, {p + 40, self + 40});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x65A720, "ScenePartyTop::callPartyTopAPI $_5 __func::__clone(__base*)", t_65A720);

// ScenePartyTop::callPartyTopAPI $_5: __func::destroy
static void t_65A754(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
}
RW_FUNCTION(0x65A754, "ScenePartyTop::callPartyTopAPI $_5 __func::destroy", t_65A754);

// ScenePartyTop::callPartyTopAPI $_5: __func::destroy_deallocate
static void t_65A774(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65A774, "ScenePartyTop::callPartyTopAPI $_5 __func::destroy_deallocate", t_65A774);

// ScenePartyTop::callPartyTopAPI $_5: __func::target
static uint32_t t_65A7B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe637b0u ? self + 8 : 0; }
RW_FUNCTION(0x65A7B4, "ScenePartyTop::callPartyTopAPI $_5 __func::target", t_65A7B4);

// ScenePartyTop::callPartyTopAPI $_5: __func::target_type
static uint32_t t_65A7D4() { return 0xf9a268u; }
RW_FUNCTION(0x65A7D4, "ScenePartyTop::callPartyTopAPI $_5 __func::target_type", t_65A7D4);

// ScenePartyTop::callPartyTopAPI $_2 UlbE_: __func::~__func
static uint32_t t_65A90C(uint32_t self) {
    st32(self, 0xf62708u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x65A90C, "ScenePartyTop::callPartyTopAPI $_2 UlbE_ __func::~__func", t_65A90C);
