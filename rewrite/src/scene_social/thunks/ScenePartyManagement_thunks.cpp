// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ScenePartyManagement lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ScenePartyManagement::backScene $_23: __func::~__func (deleting)
static void t_6E97FC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E97FC, "ScenePartyManagement::backScene $_23 __func::~__func (deleting)", t_6E97FC);

// ScenePartyManagement::backScene $_23: __func::__clone()
static uint32_t t_6E9804(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf691d0u);
    }
    return v1;
}
RW_FUNCTION(0x6E9804, "ScenePartyManagement::backScene $_23 __func::__clone()", t_6E9804);

// ScenePartyManagement::backScene $_23: __func::__clone(__base*)
static void t_6E9828(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf691d0u);
    }
}
RW_FUNCTION(0x6E9828, "ScenePartyManagement::backScene $_23 __func::__clone(__base*)", t_6E9828);

// ScenePartyManagement::backScene $_23: __func::destroy
static void t_6E9844(uint32_t self) {
}
RW_FUNCTION(0x6E9844, "ScenePartyManagement::backScene $_23 __func::destroy", t_6E9844);

// ScenePartyManagement::backScene $_23: __func::destroy_deallocate
static void t_6E9846(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9846, "ScenePartyManagement::backScene $_23 __func::destroy_deallocate", t_6E9846);

// ScenePartyManagement::backScene $_23: __func::target
static uint32_t t_6E9970(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86170u ? self + 4 : 0; }
RW_FUNCTION(0x6E9970, "ScenePartyManagement::backScene $_23 __func::target", t_6E9970);

// ScenePartyManagement::backScene $_23: __func::target_type
static uint32_t t_6E9990() { return 0xfa1aacu; }
RW_FUNCTION(0x6E9990, "ScenePartyManagement::backScene $_23 __func::target_type", t_6E9990);

// ScenePartyManagement::openPopUpApproveCheck : __func::~__func
static uint32_t t_6E99A4(uint32_t self) {
    st32(self, 0xf691fcu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x6E99A4, "ScenePartyManagement::openPopUpApproveCheck  __func::~__func", t_6E99A4);

// ScenePartyManagement::openPopUpApproveCheck : __func::~__func (deleting)
static void t_6E99E0(uint32_t self) {
    st32(self, 0xf691fcu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E99E0, "ScenePartyManagement::openPopUpApproveCheck  __func::~__func (deleting)", t_6E99E0);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone()
static uint32_t t_6E9A20(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf691fcu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 20, v5);
        st32(v1 + 16, v4);
        st32(v1 + 12, v3);
        st32(v1 + 8, v2);
        uint32_t v6 = ld32(self + 24);
        uint32_t v7 = ld32(self + 28);
        st32(v1 + 28, v7);
        st32(v1 + 24, v6);
        call(0x4683cd, {v1 + 32, self + 32});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6E9A20, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone()", t_6E9A20);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone(__base*)
static void t_6E9A84(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf691fcu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        uint32_t v5 = ld32(self + 24);
        uint32_t v6 = ld32(self + 28);
        st32(p + 28, v6);
        st32(p + 24, v5);
        call(0x4683cd, {p + 32, self + 32});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x6E9A84, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone(__base*)", t_6E9A84);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy
static void t_6E9ACC(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x6E9ACC, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy", t_6E9ACC);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy_deallocate
static void t_6E9AEC(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9AEC, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy_deallocate", t_6E9AEC);

// ScenePartyManagement::openPopUpApproveCheck : __func::target
static uint32_t t_6E9C58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86260u ? self + 8 : 0; }
RW_FUNCTION(0x6E9C58, "ScenePartyManagement::openPopUpApproveCheck  __func::target", t_6E9C58);

// ScenePartyManagement::openPopUpApproveCheck : __func::target_type
static uint32_t t_6E9C7C() { return 0xfa1ac0u; }
RW_FUNCTION(0x6E9C7C, "ScenePartyManagement::openPopUpApproveCheck  __func::target_type", t_6E9C7C);

// ScenePartyManagement::openPopUpApproveCheck : __func::~__func (deleting)
static void t_6E9D64(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9D64, "ScenePartyManagement::openPopUpApproveCheck  __func::~__func (deleting)", t_6E9D64);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone()
static uint32_t t_6E9D6C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf69228u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6E9D6C, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone()", t_6E9D6C);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone(__base*)
static void t_6E9D9C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69228u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E9D9C, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone(__base*)", t_6E9D9C);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy
static void t_6E9DC4(uint32_t self) {
}
RW_FUNCTION(0x6E9DC4, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy", t_6E9DC4);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy_deallocate
static void t_6E9DC6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9DC6, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy_deallocate", t_6E9DC6);

// ScenePartyManagement::openPopUpApproveCheck : __func::target
static uint32_t t_6E9DF8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe863d0u ? self + 4 : 0; }
RW_FUNCTION(0x6E9DF8, "ScenePartyManagement::openPopUpApproveCheck  __func::target", t_6E9DF8);

// ScenePartyManagement::openPopUpApproveCheck : __func::target_type
static uint32_t t_6E9E18() { return 0xfa1ad4u; }
RW_FUNCTION(0x6E9E18, "ScenePartyManagement::openPopUpApproveCheck  __func::target_type", t_6E9E18);

// ScenePartyManagement::openPopUpApproveCheck : __func::~__func (deleting)
static void t_6E9E2C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9E2C, "ScenePartyManagement::openPopUpApproveCheck  __func::~__func (deleting)", t_6E9E2C);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone()
static uint32_t t_6E9E34(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69254u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E9E34, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone()", t_6E9E34);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone(__base*)
static void t_6E9E60(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69254u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E9E60, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone(__base*)", t_6E9E60);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy
static void t_6E9E80(uint32_t self) {
}
RW_FUNCTION(0x6E9E80, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy", t_6E9E80);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy_deallocate
static void t_6E9E82(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9E82, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy_deallocate", t_6E9E82);

// ScenePartyManagement::openPopUpApproveCheck : __func::target
static uint32_t t_6E9E9C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86540u ? self + 4 : 0; }
RW_FUNCTION(0x6E9E9C, "ScenePartyManagement::openPopUpApproveCheck  __func::target", t_6E9E9C);

// ScenePartyManagement::openPopUpApproveCheck : __func::target_type
static uint32_t t_6E9EBC() { return 0xfa1ae8u; }
RW_FUNCTION(0x6E9EBC, "ScenePartyManagement::openPopUpApproveCheck  __func::target_type", t_6E9EBC);

// ScenePartyManagement::openPopUpApproveCheck : __func::~__func (deleting)
static void t_6E9ED0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9ED0, "ScenePartyManagement::openPopUpApproveCheck  __func::~__func (deleting)", t_6E9ED0);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone()
static uint32_t t_6E9ED8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69280u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E9ED8, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone()", t_6E9ED8);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone(__base*)
static void t_6E9F04(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69280u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E9F04, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone(__base*)", t_6E9F04);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy
static void t_6E9F24(uint32_t self) {
}
RW_FUNCTION(0x6E9F24, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy", t_6E9F24);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy_deallocate
static void t_6E9F26(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9F26, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy_deallocate", t_6E9F26);

// ScenePartyManagement::openPopUpApproveCheck : __func::target
static uint32_t t_6E9F38(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe866a0u ? self + 4 : 0; }
RW_FUNCTION(0x6E9F38, "ScenePartyManagement::openPopUpApproveCheck  __func::target", t_6E9F38);

// ScenePartyManagement::openPopUpApproveCheck : __func::target_type
static uint32_t t_6E9F58() { return 0xfa1afcu; }
RW_FUNCTION(0x6E9F58, "ScenePartyManagement::openPopUpApproveCheck  __func::target_type", t_6E9F58);

// ScenePartyManagement::openPopUpApproveCheck : __func::~__func (deleting)
static void t_6E9F6C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9F6C, "ScenePartyManagement::openPopUpApproveCheck  __func::~__func (deleting)", t_6E9F6C);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone()
static uint32_t t_6E9F74(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf692acu);
    }
    return v1;
}
RW_FUNCTION(0x6E9F74, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone()", t_6E9F74);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone(__base*)
static void t_6E9F98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf692acu);
    }
}
RW_FUNCTION(0x6E9F98, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone(__base*)", t_6E9F98);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy
static void t_6E9FB4(uint32_t self) {
}
RW_FUNCTION(0x6E9FB4, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy", t_6E9FB4);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy_deallocate
static void t_6E9FB6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E9FB6, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy_deallocate", t_6E9FB6);

// ScenePartyManagement::openPopUpApproveCheck : __func::target
static uint32_t t_6E9FD4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86830u ? self + 4 : 0; }
RW_FUNCTION(0x6E9FD4, "ScenePartyManagement::openPopUpApproveCheck  __func::target", t_6E9FD4);

// ScenePartyManagement::openPopUpApproveCheck : __func::target_type
static uint32_t t_6E9FF4() { return 0xfa1b10u; }
RW_FUNCTION(0x6E9FF4, "ScenePartyManagement::openPopUpApproveCheck  __func::target_type", t_6E9FF4);

// ScenePartyManagement::openPopUpApproveCheck : __func::~__func (deleting)
static void t_6EA008(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EA008, "ScenePartyManagement::openPopUpApproveCheck  __func::~__func (deleting)", t_6EA008);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone()
static uint32_t t_6EA010(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf692d8u);
    }
    return v1;
}
RW_FUNCTION(0x6EA010, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone()", t_6EA010);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone(__base*)
static void t_6EA034(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf692d8u);
    }
}
RW_FUNCTION(0x6EA034, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone(__base*)", t_6EA034);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy
static void t_6EA050(uint32_t self) {
}
RW_FUNCTION(0x6EA050, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy", t_6EA050);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy_deallocate
static void t_6EA052(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EA052, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy_deallocate", t_6EA052);

// ScenePartyManagement::openPopUpApproveCheck : __func::target
static uint32_t t_6EA070(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe869c0u ? self + 4 : 0; }
RW_FUNCTION(0x6EA070, "ScenePartyManagement::openPopUpApproveCheck  __func::target", t_6EA070);

// ScenePartyManagement::openPopUpApproveCheck : __func::target_type
static uint32_t t_6EA090() { return 0xfa1b24u; }
RW_FUNCTION(0x6EA090, "ScenePartyManagement::openPopUpApproveCheck  __func::target_type", t_6EA090);

// ScenePartyManagement::openPopUpApproveCheck : __func::~__func
static uint32_t t_6EA0A4(uint32_t self) {
    st32(self, 0xf69304u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x6EA0A4, "ScenePartyManagement::openPopUpApproveCheck  __func::~__func", t_6EA0A4);

// ScenePartyManagement::openPopUpApproveCheck : __func::~__func (deleting)
static void t_6EA0E0(uint32_t self) {
    st32(self, 0xf69304u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EA0E0, "ScenePartyManagement::openPopUpApproveCheck  __func::~__func (deleting)", t_6EA0E0);

// ScenePartyManagement::openPopUpApproveCheck : __func::__clone(__base*)
static void t_6EA190(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69304u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v4 = ld32(self + 8 + 0);
        uint32_t v5 = ld32(self + 8 + 4);
        uint32_t v6 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v4);
        st32(p + 8 + 4, v5);
        st32(p + 8 + 8, v6);
    }
}
RW_FUNCTION(0x6EA190, "ScenePartyManagement::openPopUpApproveCheck  __func::__clone(__base*)", t_6EA190);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy
static void t_6EA1E0(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x6EA1E0, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy", t_6EA1E0);

// ScenePartyManagement::openPopUpApproveCheck : __func::destroy_deallocate
static void t_6EA200(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EA200, "ScenePartyManagement::openPopUpApproveCheck  __func::destroy_deallocate", t_6EA200);

// ScenePartyManagement::openPopUpApproveCheck : __func::target
static uint32_t t_6EA24C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86b20u ? self + 8 : 0; }
RW_FUNCTION(0x6EA24C, "ScenePartyManagement::openPopUpApproveCheck  __func::target", t_6EA24C);

// ScenePartyManagement::openPopUpApproveCheck : __func::target_type
static uint32_t t_6EA270() { return 0xfa1b38u; }
RW_FUNCTION(0x6EA270, "ScenePartyManagement::openPopUpApproveCheck  __func::target_type", t_6EA270);

// ScenePartyManagement::openPopUpCheck : __func::~__func
static uint32_t t_6EA284(uint32_t self) {
    st32(self, 0xf69330u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6EA284, "ScenePartyManagement::openPopUpCheck  __func::~__func", t_6EA284);

// ScenePartyManagement::openPopUpCheck : __func::~__func (deleting)
static void t_6EA2DC(uint32_t self) {
    st32(self, 0xf69330u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EA2DC, "ScenePartyManagement::openPopUpCheck  __func::~__func (deleting)", t_6EA2DC);

// ScenePartyManagement::openPopUpCheck : __func::__clone(__base*)
static void t_6EA3C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69330u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        call(0x4683cd, {p + 32, self + 32});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x6EA3C4, "ScenePartyManagement::openPopUpCheck  __func::__clone(__base*)", t_6EA3C4);

// ScenePartyManagement::openPopUpCheck : __func::destroy
static void t_6EA438(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6EA438, "ScenePartyManagement::openPopUpCheck  __func::destroy", t_6EA438);

// ScenePartyManagement::openPopUpCheck : __func::destroy_deallocate
static void t_6EA478(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EA478, "ScenePartyManagement::openPopUpCheck  __func::destroy_deallocate", t_6EA478);

// ScenePartyManagement::openPopUpCheck : __func::target
static uint32_t t_6EA4E0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86c70u ? self + 8 : 0; }
RW_FUNCTION(0x6EA4E0, "ScenePartyManagement::openPopUpCheck  __func::target", t_6EA4E0);

// ScenePartyManagement::openPopUpCheck : __func::target_type
static uint32_t t_6EA500() { return 0xfa1b4cu; }
RW_FUNCTION(0x6EA500, "ScenePartyManagement::openPopUpCheck  __func::target_type", t_6EA500);

// ScenePartyManagement::runInEntryLayout $_16: __func::target
static uint32_t t_6EABB0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86d70u ? self + 4 : 0; }
RW_FUNCTION(0x6EABB0, "ScenePartyManagement::runInEntryLayout $_16 __func::target", t_6EABB0);

// ScenePartyManagement::runInEntryLayout $_16: __func::target_type
static uint32_t t_6EABD0() { return 0xfa1b60u; }
RW_FUNCTION(0x6EABD0, "ScenePartyManagement::runInEntryLayout $_16 __func::target_type", t_6EABD0);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_: __func::~__func
static uint32_t t_6EAC58(uint32_t self) {
    st32(self, 0xf69388u);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x6EAC58, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ __func::~__func", t_6EAC58);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_: __func::~__func (deleting)
static void t_6EAC88(uint32_t self) {
    st32(self, 0xf69388u);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EAC88, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ __func::~__func (deleting)", t_6EAC88);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_: __func::destroy
static void t_6EADD8(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x6EADD8, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ __func::destroy", t_6EADD8);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_: __func::destroy_deallocate
static void t_6EADEC(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EADEC, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ __func::destroy_deallocate", t_6EADEC);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_: __func::target
static uint32_t t_6EAEF8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86e30u ? self + 8 : 0; }
RW_FUNCTION(0x6EAEF8, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ __func::target", t_6EAEF8);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_: __func::target_type
static uint32_t t_6EAF1C() { return 0xfa1b74u; }
RW_FUNCTION(0x6EAF1C, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ __func::target_type", t_6EAF1C);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6EAF30(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EAF30, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ __func::~__func (deleting)", t_6EAF30);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6EAF38(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf693b4u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6EAF38, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ __func::__clone()", t_6EAF38);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_: __func::__clone(__base*)
static void t_6EAF6C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf693b4u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6EAF6C, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ __func::__clone(__base*)", t_6EAF6C);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_: __func::destroy
static void t_6EAF98(uint32_t self) {
}
RW_FUNCTION(0x6EAF98, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ __func::destroy", t_6EAF98);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6EAF9A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EAF9A, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ __func::destroy_deallocate", t_6EAF9A);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_: __func::target
static uint32_t t_6EB148(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86f00u ? self + 8 : 0; }
RW_FUNCTION(0x6EB148, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ __func::target", t_6EB148);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_: __func::target_type
static uint32_t t_6EB16C() { return 0xfa1b88u; }
RW_FUNCTION(0x6EB16C, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ __func::target_type", t_6EB16C);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_: __func::~__func (deleting)
static void t_6EB180(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EB180, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_ __func::~__func (deleting)", t_6EB180);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_: __func::__clone()
static uint32_t t_6EB188(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf693e0u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6EB188, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_ __func::__clone()", t_6EB188);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_: __func::__clone(__base*)
static void t_6EB1BC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf693e0u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6EB1BC, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_ __func::__clone(__base*)", t_6EB1BC);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_: __func::destroy
static void t_6EB1E8(uint32_t self) {
}
RW_FUNCTION(0x6EB1E8, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_ __func::destroy", t_6EB1E8);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_: __func::destroy_deallocate
static void t_6EB1EA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EB1EA, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_ __func::destroy_deallocate", t_6EB1EA);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_: __func::target
static uint32_t t_6EB2D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86fe0u ? self + 8 : 0; }
RW_FUNCTION(0x6EB2D8, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_ __func::target", t_6EB2D8);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_: __func::target_type
static uint32_t t_6EB2FC() { return 0xfa1b9cu; }
RW_FUNCTION(0x6EB2FC, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE0_ __func::target_type", t_6EB2FC);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6EB310(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EB310, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6EB310);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6EB318(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6940cu);
    }
    return v1;
}
RW_FUNCTION(0x6EB318, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6EB318);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6EB33C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6940cu);
    }
}
RW_FUNCTION(0x6EB33C, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6EB33C);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6EB358(uint32_t self) {
}
RW_FUNCTION(0x6EB358, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_ __func::destroy", t_6EB358);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6EB35A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EB35A, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6EB35A);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6EB364(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe870d0u ? self + 4 : 0; }
RW_FUNCTION(0x6EB364, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_ __func::target", t_6EB364);

// ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6EB384() { return 0xfa1bb0u; }
RW_FUNCTION(0x6EB384, "ScenePartyManagement::runInEntryLayout $_16 UlvE_ UlbE_ UlbE_ __func::target_type", t_6EB384);

// ScenePartyManagement::runInEntryLayout $_15: __func::target
static uint32_t t_6EBA1C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe871a0u ? self + 4 : 0; }
RW_FUNCTION(0x6EBA1C, "ScenePartyManagement::runInEntryLayout $_15 __func::target", t_6EBA1C);

// ScenePartyManagement::runInEntryLayout $_15: __func::target_type
static uint32_t t_6EBA3C() { return 0xfa1bc4u; }
RW_FUNCTION(0x6EBA3C, "ScenePartyManagement::runInEntryLayout $_15 __func::target_type", t_6EBA3C);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_: __func::~__func
static uint32_t t_6EBAC8(uint32_t self) {
    st32(self, 0xf69464u);
    uint32_t v1 = ld8(self + 32);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 40);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x6EBAC8, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ __func::~__func", t_6EBAC8);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_: __func::~__func (deleting)
static void t_6EBAF8(uint32_t self) {
    st32(self, 0xf69464u);
    uint32_t v1 = ld8(self + 32);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 40);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EBAF8, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ __func::~__func (deleting)", t_6EBAF8);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_: __func::destroy
static void t_6EBC40(uint32_t self) {
    uint32_t v1 = ld8(self + 32);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 40);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x6EBC40, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ __func::destroy", t_6EBC40);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_: __func::destroy_deallocate
static void t_6EBC54(uint32_t self) {
    uint32_t v1 = ld8(self + 32);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 40);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EBC54, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ __func::destroy_deallocate", t_6EBC54);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_: __func::target
static uint32_t t_6EBD28(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87260u ? self + 8 : 0; }
RW_FUNCTION(0x6EBD28, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ __func::target", t_6EBD28);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_: __func::target_type
static uint32_t t_6EBD4C() { return 0xfa1bd8u; }
RW_FUNCTION(0x6EBD4C, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ __func::target_type", t_6EBD4C);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6EBD60(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EBD60, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ __func::~__func (deleting)", t_6EBD60);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6EBD68(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69490u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6EBD68, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ __func::__clone()", t_6EBD68);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_: __func::__clone(__base*)
static void t_6EBD94(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69490u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6EBD94, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ __func::__clone(__base*)", t_6EBD94);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_: __func::destroy
static void t_6EBDB4(uint32_t self) {
}
RW_FUNCTION(0x6EBDB4, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ __func::destroy", t_6EBDB4);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6EBDB6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EBDB6, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ __func::destroy_deallocate", t_6EBDB6);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_: __func::target
static uint32_t t_6EBEB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87330u ? self + 4 : 0; }
RW_FUNCTION(0x6EBEB8, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ __func::target", t_6EBEB8);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_: __func::target_type
static uint32_t t_6EBED8() { return 0xfa1becu; }
RW_FUNCTION(0x6EBED8, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ __func::target_type", t_6EBED8);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_: __func::~__func (deleting)
static void t_6EBEEC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EBEEC, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_ __func::~__func (deleting)", t_6EBEEC);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_: __func::__clone()
static uint32_t t_6EBEF4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf694bcu);
    }
    return v1;
}
RW_FUNCTION(0x6EBEF4, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_ __func::__clone()", t_6EBEF4);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_: __func::__clone(__base*)
static void t_6EBF18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf694bcu);
    }
}
RW_FUNCTION(0x6EBF18, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_ __func::__clone(__base*)", t_6EBF18);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_: __func::destroy
static void t_6EBF34(uint32_t self) {
}
RW_FUNCTION(0x6EBF34, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_ __func::destroy", t_6EBF34);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_: __func::destroy_deallocate
static void t_6EBF36(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EBF36, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_ __func::destroy_deallocate", t_6EBF36);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_: __func::target
static uint32_t t_6EC060(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87410u ? self + 4 : 0; }
RW_FUNCTION(0x6EC060, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_ __func::target", t_6EC060);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_: __func::target_type
static uint32_t t_6EC080() { return 0xfa1c00u; }
RW_FUNCTION(0x6EC080, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlvE_ __func::target_type", t_6EC080);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6EC094(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EC094, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6EC094);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6EC09C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf694e8u);
    }
    return v1;
}
RW_FUNCTION(0x6EC09C, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6EC09C);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6EC0C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf694e8u);
    }
}
RW_FUNCTION(0x6EC0C0, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6EC0C0);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6EC0DC(uint32_t self) {
}
RW_FUNCTION(0x6EC0DC, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_ __func::destroy", t_6EC0DC);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6EC0DE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EC0DE, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6EC0DE);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6EC0E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87500u ? self + 4 : 0; }
RW_FUNCTION(0x6EC0E8, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_ __func::target", t_6EC0E8);

// ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6EC108() { return 0xfa1c14u; }
RW_FUNCTION(0x6EC108, "ScenePartyManagement::runInEntryLayout $_15 UlvE_ UlbE_ UlbE_ __func::target_type", t_6EC108);

// ScenePartyManagement::runInEntryLayout $_14: __func::target
static uint32_t t_6EC594(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe875d0u ? self + 4 : 0; }
RW_FUNCTION(0x6EC594, "ScenePartyManagement::runInEntryLayout $_14 __func::target", t_6EC594);

// ScenePartyManagement::runInEntryLayout $_14: __func::target_type
static uint32_t t_6EC5B4() { return 0xfa1c28u; }
RW_FUNCTION(0x6EC5B4, "ScenePartyManagement::runInEntryLayout $_14 __func::target_type", t_6EC5B4);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_: __func::~__func
static uint32_t t_6EC5C8(uint32_t self) {
    st32(self, 0xf69540u);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x6EC5C8, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ __func::~__func", t_6EC5C8);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_: __func::~__func (deleting)
static void t_6EC5F8(uint32_t self) {
    st32(self, 0xf69540u);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EC5F8, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ __func::~__func (deleting)", t_6EC5F8);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_: __func::destroy
static void t_6EC748(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x6EC748, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ __func::destroy", t_6EC748);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_: __func::destroy_deallocate
static void t_6EC75C(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EC75C, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ __func::destroy_deallocate", t_6EC75C);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_: __func::target
static uint32_t t_6EC868(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87690u ? self + 8 : 0; }
RW_FUNCTION(0x6EC868, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ __func::target", t_6EC868);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_: __func::target_type
static uint32_t t_6EC88C() { return 0xfa1c3cu; }
RW_FUNCTION(0x6EC88C, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ __func::target_type", t_6EC88C);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6EC8A0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EC8A0, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ __func::~__func (deleting)", t_6EC8A0);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6EC8A8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf6956cu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6EC8A8, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ __func::__clone()", t_6EC8A8);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_: __func::__clone(__base*)
static void t_6EC8DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6956cu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6EC8DC, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ __func::__clone(__base*)", t_6EC8DC);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_: __func::destroy
static void t_6EC908(uint32_t self) {
}
RW_FUNCTION(0x6EC908, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ __func::destroy", t_6EC908);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6EC90A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EC90A, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ __func::destroy_deallocate", t_6EC90A);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_: __func::target
static uint32_t t_6ECAB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87760u ? self + 8 : 0; }
RW_FUNCTION(0x6ECAB8, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ __func::target", t_6ECAB8);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_: __func::target_type
static uint32_t t_6ECADC() { return 0xfa1c50u; }
RW_FUNCTION(0x6ECADC, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ __func::target_type", t_6ECADC);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_: __func::~__func (deleting)
static void t_6ECAF0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ECAF0, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_ __func::~__func (deleting)", t_6ECAF0);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_: __func::__clone()
static uint32_t t_6ECAF8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf69598u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6ECAF8, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_ __func::__clone()", t_6ECAF8);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_: __func::__clone(__base*)
static void t_6ECB2C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69598u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6ECB2C, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_ __func::__clone(__base*)", t_6ECB2C);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_: __func::destroy
static void t_6ECB58(uint32_t self) {
}
RW_FUNCTION(0x6ECB58, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_ __func::destroy", t_6ECB58);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_: __func::destroy_deallocate
static void t_6ECB5A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ECB5A, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_ __func::destroy_deallocate", t_6ECB5A);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_: __func::target
static uint32_t t_6ECC48(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87840u ? self + 8 : 0; }
RW_FUNCTION(0x6ECC48, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_ __func::target", t_6ECC48);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_: __func::target_type
static uint32_t t_6ECC6C() { return 0xfa1c64u; }
RW_FUNCTION(0x6ECC6C, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE0_ __func::target_type", t_6ECC6C);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6ECC80(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ECC80, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6ECC80);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6ECC88(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf695c4u);
    }
    return v1;
}
RW_FUNCTION(0x6ECC88, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6ECC88);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6ECCAC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf695c4u);
    }
}
RW_FUNCTION(0x6ECCAC, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6ECCAC);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6ECCC8(uint32_t self) {
}
RW_FUNCTION(0x6ECCC8, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_ __func::destroy", t_6ECCC8);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6ECCCA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ECCCA, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6ECCCA);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6ECCD4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87930u ? self + 4 : 0; }
RW_FUNCTION(0x6ECCD4, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_ __func::target", t_6ECCD4);

// ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6ECCF4() { return 0xfa1c78u; }
RW_FUNCTION(0x6ECCF4, "ScenePartyManagement::runInEntryLayout $_14 UlvE_ UlbE_ UlbE_ __func::target_type", t_6ECCF4);

// ScenePartyManagement::runInJoinLayout $_13: __func::target
static uint32_t t_6ED3C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87a00u ? self + 4 : 0; }
RW_FUNCTION(0x6ED3C8, "ScenePartyManagement::runInJoinLayout $_13 __func::target", t_6ED3C8);

// ScenePartyManagement::runInJoinLayout $_13: __func::target_type
static uint32_t t_6ED3E8() { return 0xfa1c8cu; }
RW_FUNCTION(0x6ED3E8, "ScenePartyManagement::runInJoinLayout $_13 __func::target_type", t_6ED3E8);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_: __func::~__func
static uint32_t t_6ED470(uint32_t self) {
    st32(self, 0xf6961cu);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x6ED470, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ __func::~__func", t_6ED470);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_: __func::~__func (deleting)
static void t_6ED4A0(uint32_t self) {
    st32(self, 0xf6961cu);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ED4A0, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ __func::~__func (deleting)", t_6ED4A0);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_: __func::destroy
static void t_6ED5F0(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x6ED5F0, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ __func::destroy", t_6ED5F0);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_: __func::destroy_deallocate
static void t_6ED604(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ED604, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ __func::destroy_deallocate", t_6ED604);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_: __func::target
static uint32_t t_6ED710(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87ac0u ? self + 8 : 0; }
RW_FUNCTION(0x6ED710, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ __func::target", t_6ED710);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_: __func::target_type
static uint32_t t_6ED734() { return 0xfa1ca0u; }
RW_FUNCTION(0x6ED734, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ __func::target_type", t_6ED734);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6ED748(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ED748, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ __func::~__func (deleting)", t_6ED748);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6ED750(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf69648u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6ED750, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ __func::__clone()", t_6ED750);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_: __func::__clone(__base*)
static void t_6ED784(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69648u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6ED784, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ __func::__clone(__base*)", t_6ED784);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_: __func::destroy
static void t_6ED7B0(uint32_t self) {
}
RW_FUNCTION(0x6ED7B0, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ __func::destroy", t_6ED7B0);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6ED7B2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ED7B2, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ __func::destroy_deallocate", t_6ED7B2);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_: __func::target
static uint32_t t_6ED960(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87b80u ? self + 8 : 0; }
RW_FUNCTION(0x6ED960, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ __func::target", t_6ED960);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_: __func::target_type
static uint32_t t_6ED984() { return 0xfa1cb4u; }
RW_FUNCTION(0x6ED984, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ __func::target_type", t_6ED984);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_: __func::~__func (deleting)
static void t_6ED998(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ED998, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_ __func::~__func (deleting)", t_6ED998);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_: __func::__clone()
static uint32_t t_6ED9A0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf69674u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6ED9A0, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_ __func::__clone()", t_6ED9A0);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_: __func::__clone(__base*)
static void t_6ED9D4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69674u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6ED9D4, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_ __func::__clone(__base*)", t_6ED9D4);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_: __func::destroy
static void t_6EDA00(uint32_t self) {
}
RW_FUNCTION(0x6EDA00, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_ __func::destroy", t_6EDA00);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_: __func::destroy_deallocate
static void t_6EDA02(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EDA02, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_ __func::destroy_deallocate", t_6EDA02);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_: __func::target
static uint32_t t_6EDAF4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87c60u ? self + 8 : 0; }
RW_FUNCTION(0x6EDAF4, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_ __func::target", t_6EDAF4);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_: __func::target_type
static uint32_t t_6EDB18() { return 0xfa1cc8u; }
RW_FUNCTION(0x6EDB18, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE0_ __func::target_type", t_6EDB18);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6EDB2C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EDB2C, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6EDB2C);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6EDB34(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf696a0u);
    }
    return v1;
}
RW_FUNCTION(0x6EDB34, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6EDB34);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6EDB58(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf696a0u);
    }
}
RW_FUNCTION(0x6EDB58, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6EDB58);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6EDB74(uint32_t self) {
}
RW_FUNCTION(0x6EDB74, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_ __func::destroy", t_6EDB74);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6EDB76(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EDB76, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6EDB76);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6EDB80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87d50u ? self + 4 : 0; }
RW_FUNCTION(0x6EDB80, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_ __func::target", t_6EDB80);

// ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6EDBA0() { return 0xfa1cdcu; }
RW_FUNCTION(0x6EDBA0, "ScenePartyManagement::runInJoinLayout $_13 UlvE_ UlbE_ UlbE_ __func::target_type", t_6EDBA0);

// ScenePartyManagement::runInJoinLayout $_12: __func::target
static uint32_t t_6EE274(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87e20u ? self + 4 : 0; }
RW_FUNCTION(0x6EE274, "ScenePartyManagement::runInJoinLayout $_12 __func::target", t_6EE274);

// ScenePartyManagement::runInJoinLayout $_12: __func::target_type
static uint32_t t_6EE294() { return 0xfa1cf0u; }
RW_FUNCTION(0x6EE294, "ScenePartyManagement::runInJoinLayout $_12 __func::target_type", t_6EE294);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_: __func::~__func
static uint32_t t_6EE31C(uint32_t self) {
    st32(self, 0xf696f8u);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x6EE31C, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ __func::~__func", t_6EE31C);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_: __func::~__func (deleting)
static void t_6EE34C(uint32_t self) {
    st32(self, 0xf696f8u);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EE34C, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ __func::~__func (deleting)", t_6EE34C);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_: __func::destroy
static void t_6EE49C(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x6EE49C, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ __func::destroy", t_6EE49C);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_: __func::destroy_deallocate
static void t_6EE4B0(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EE4B0, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ __func::destroy_deallocate", t_6EE4B0);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_: __func::target
static uint32_t t_6EE5BC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87ee0u ? self + 8 : 0; }
RW_FUNCTION(0x6EE5BC, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ __func::target", t_6EE5BC);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_: __func::target_type
static uint32_t t_6EE5E0() { return 0xfa1d04u; }
RW_FUNCTION(0x6EE5E0, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ __func::target_type", t_6EE5E0);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6EE5F4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EE5F4, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ __func::~__func (deleting)", t_6EE5F4);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6EE5FC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf69724u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6EE5FC, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ __func::__clone()", t_6EE5FC);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_: __func::__clone(__base*)
static void t_6EE630(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69724u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6EE630, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ __func::__clone(__base*)", t_6EE630);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_: __func::destroy
static void t_6EE65C(uint32_t self) {
}
RW_FUNCTION(0x6EE65C, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ __func::destroy", t_6EE65C);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6EE65E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EE65E, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ __func::destroy_deallocate", t_6EE65E);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_: __func::target
static uint32_t t_6EE80C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe87fa0u ? self + 8 : 0; }
RW_FUNCTION(0x6EE80C, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ __func::target", t_6EE80C);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_: __func::target_type
static uint32_t t_6EE830() { return 0xfa1d18u; }
RW_FUNCTION(0x6EE830, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ __func::target_type", t_6EE830);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_: __func::~__func (deleting)
static void t_6EE844(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EE844, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_ __func::~__func (deleting)", t_6EE844);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_: __func::__clone()
static uint32_t t_6EE84C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf69750u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6EE84C, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_ __func::__clone()", t_6EE84C);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_: __func::__clone(__base*)
static void t_6EE880(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69750u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6EE880, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_ __func::__clone(__base*)", t_6EE880);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_: __func::destroy
static void t_6EE8AC(uint32_t self) {
}
RW_FUNCTION(0x6EE8AC, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_ __func::destroy", t_6EE8AC);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_: __func::destroy_deallocate
static void t_6EE8AE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EE8AE, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_ __func::destroy_deallocate", t_6EE8AE);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_: __func::target
static uint32_t t_6EE9A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88080u ? self + 8 : 0; }
RW_FUNCTION(0x6EE9A0, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_ __func::target", t_6EE9A0);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_: __func::target_type
static uint32_t t_6EE9C4() { return 0xfa1d2cu; }
RW_FUNCTION(0x6EE9C4, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE0_ __func::target_type", t_6EE9C4);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6EE9D8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EE9D8, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6EE9D8);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6EE9E0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6977cu);
    }
    return v1;
}
RW_FUNCTION(0x6EE9E0, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6EE9E0);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6EEA04(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6977cu);
    }
}
RW_FUNCTION(0x6EEA04, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6EEA04);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6EEA20(uint32_t self) {
}
RW_FUNCTION(0x6EEA20, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::destroy", t_6EEA20);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6EEA22(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EEA22, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6EEA22);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6EEA2C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88170u ? self + 4 : 0; }
RW_FUNCTION(0x6EEA2C, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::target", t_6EEA2C);

// ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6EEA4C() { return 0xfa1d40u; }
RW_FUNCTION(0x6EEA4C, "ScenePartyManagement::runInJoinLayout $_12 UlvE_ UlbE_ UlbE_ __func::target_type", t_6EEA4C);

// ScenePartyManagement::runInJoinLayout $_11: __func::target
static uint32_t t_6EEEDC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88240u ? self + 4 : 0; }
RW_FUNCTION(0x6EEEDC, "ScenePartyManagement::runInJoinLayout $_11 __func::target", t_6EEEDC);

// ScenePartyManagement::runInJoinLayout $_11: __func::target_type
static uint32_t t_6EEEFC() { return 0xfa1d54u; }
RW_FUNCTION(0x6EEEFC, "ScenePartyManagement::runInJoinLayout $_11 __func::target_type", t_6EEEFC);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_: __func::~__func
static uint32_t t_6EEF10(uint32_t self) {
    st32(self, 0xf697d4u);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x6EEF10, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ __func::~__func", t_6EEF10);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_: __func::~__func (deleting)
static void t_6EEF40(uint32_t self) {
    st32(self, 0xf697d4u);
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EEF40, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ __func::~__func (deleting)", t_6EEF40);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_: __func::destroy
static void t_6EF090(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x6EF090, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ __func::destroy", t_6EF090);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_: __func::destroy_deallocate
static void t_6EF0A4(uint32_t self) {
    uint32_t v1 = ld8(self + 40);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 48);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF0A4, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ __func::destroy_deallocate", t_6EF0A4);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_: __func::target
static uint32_t t_6EF1B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88300u ? self + 8 : 0; }
RW_FUNCTION(0x6EF1B0, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ __func::target", t_6EF1B0);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_: __func::target_type
static uint32_t t_6EF1D4() { return 0xfa1d68u; }
RW_FUNCTION(0x6EF1D4, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ __func::target_type", t_6EF1D4);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6EF1E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF1E8, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ __func::~__func (deleting)", t_6EF1E8);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6EF1F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf69800u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6EF1F0, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ __func::__clone()", t_6EF1F0);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_: __func::__clone(__base*)
static void t_6EF224(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69800u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6EF224, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ __func::__clone(__base*)", t_6EF224);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_: __func::destroy
static void t_6EF250(uint32_t self) {
}
RW_FUNCTION(0x6EF250, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ __func::destroy", t_6EF250);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6EF252(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF252, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ __func::destroy_deallocate", t_6EF252);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_: __func::target
static uint32_t t_6EF400(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe883c0u ? self + 8 : 0; }
RW_FUNCTION(0x6EF400, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ __func::target", t_6EF400);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_: __func::target_type
static uint32_t t_6EF424() { return 0xfa1d7cu; }
RW_FUNCTION(0x6EF424, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ __func::target_type", t_6EF424);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_: __func::~__func (deleting)
static void t_6EF438(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF438, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_ __func::~__func (deleting)", t_6EF438);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_: __func::__clone()
static uint32_t t_6EF440(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf6982cu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
    }
    return v1;
}
RW_FUNCTION(0x6EF440, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_ __func::__clone()", t_6EF440);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_: __func::__clone(__base*)
static void t_6EF474(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6982cu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 20);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        st32(p + 20, v4);
    }
}
RW_FUNCTION(0x6EF474, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_ __func::__clone(__base*)", t_6EF474);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_: __func::destroy
static void t_6EF4A0(uint32_t self) {
}
RW_FUNCTION(0x6EF4A0, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_ __func::destroy", t_6EF4A0);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_: __func::destroy_deallocate
static void t_6EF4A2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF4A2, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_ __func::destroy_deallocate", t_6EF4A2);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_: __func::target
static uint32_t t_6EF594(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe884a0u ? self + 8 : 0; }
RW_FUNCTION(0x6EF594, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_ __func::target", t_6EF594);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_: __func::target_type
static uint32_t t_6EF5B8() { return 0xfa1d90u; }
RW_FUNCTION(0x6EF5B8, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE0_ __func::target_type", t_6EF5B8);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6EF5CC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF5CC, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6EF5CC);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6EF5D4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69858u);
    }
    return v1;
}
RW_FUNCTION(0x6EF5D4, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6EF5D4);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6EF5F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69858u);
    }
}
RW_FUNCTION(0x6EF5F8, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6EF5F8);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6EF614(uint32_t self) {
}
RW_FUNCTION(0x6EF614, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_ __func::destroy", t_6EF614);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6EF616(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF616, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6EF616);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6EF620(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88590u ? self + 4 : 0; }
RW_FUNCTION(0x6EF620, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_ __func::target", t_6EF620);

// ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6EF640() { return 0xfa1da4u; }
RW_FUNCTION(0x6EF640, "ScenePartyManagement::runInJoinLayout $_11 UlvE_ UlbE_ UlbE_ __func::target_type", t_6EF640);

// ScenePartyManagement::runInJoinLayout $_10: __func::~__func (deleting)
static void t_6EF654(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF654, "ScenePartyManagement::runInJoinLayout $_10 __func::~__func (deleting)", t_6EF654);

// ScenePartyManagement::runInJoinLayout $_10: __func::__clone()
static uint32_t t_6EF65C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69884u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6EF65C, "ScenePartyManagement::runInJoinLayout $_10 __func::__clone()", t_6EF65C);

// ScenePartyManagement::runInJoinLayout $_10: __func::__clone(__base*)
static void t_6EF688(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69884u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6EF688, "ScenePartyManagement::runInJoinLayout $_10 __func::__clone(__base*)", t_6EF688);

// ScenePartyManagement::runInJoinLayout $_10: __func::destroy
static void t_6EF6A8(uint32_t self) {
}
RW_FUNCTION(0x6EF6A8, "ScenePartyManagement::runInJoinLayout $_10 __func::destroy", t_6EF6A8);

// ScenePartyManagement::runInJoinLayout $_10: __func::destroy_deallocate
static void t_6EF6AA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF6AA, "ScenePartyManagement::runInJoinLayout $_10 __func::destroy_deallocate", t_6EF6AA);

// ScenePartyManagement::runInJoinLayout $_10: __func::target
static uint32_t t_6EF7A8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88690u ? self + 4 : 0; }
RW_FUNCTION(0x6EF7A8, "ScenePartyManagement::runInJoinLayout $_10 __func::target", t_6EF7A8);

// ScenePartyManagement::runInJoinLayout $_10: __func::target_type
static uint32_t t_6EF7C8() { return 0xfa1db8u; }
RW_FUNCTION(0x6EF7C8, "ScenePartyManagement::runInJoinLayout $_10 __func::target_type", t_6EF7C8);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_: __func::~__func (deleting)
static void t_6EF7DC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF7DC, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ __func::~__func (deleting)", t_6EF7DC);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_: __func::__clone()
static uint32_t t_6EF7E4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf698b0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6EF7E4, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ __func::__clone()", t_6EF7E4);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_: __func::__clone(__base*)
static void t_6EF810(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf698b0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6EF810, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ __func::__clone(__base*)", t_6EF810);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_: __func::destroy
static void t_6EF830(uint32_t self) {
}
RW_FUNCTION(0x6EF830, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ __func::destroy", t_6EF830);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_: __func::destroy_deallocate
static void t_6EF832(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF832, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ __func::destroy_deallocate", t_6EF832);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_: __func::target
static uint32_t t_6EF974(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88770u ? self + 4 : 0; }
RW_FUNCTION(0x6EF974, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ __func::target", t_6EF974);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_: __func::target_type
static uint32_t t_6EF994() { return 0xfa1dccu; }
RW_FUNCTION(0x6EF994, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ __func::target_type", t_6EF994);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_: __func::~__func (deleting)
static void t_6EF9A8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF9A8, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ __func::~__func (deleting)", t_6EF9A8);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_: __func::__clone()
static uint32_t t_6EF9B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf698dcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6EF9B0, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ __func::__clone()", t_6EF9B0);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_: __func::__clone(__base*)
static void t_6EF9DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf698dcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6EF9DC, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ __func::__clone(__base*)", t_6EF9DC);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_: __func::destroy
static void t_6EF9FC(uint32_t self) {
}
RW_FUNCTION(0x6EF9FC, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ __func::destroy", t_6EF9FC);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_: __func::destroy_deallocate
static void t_6EF9FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EF9FE, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ __func::destroy_deallocate", t_6EF9FE);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_: __func::target
static uint32_t t_6EFAAC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88890u ? self + 4 : 0; }
RW_FUNCTION(0x6EFAAC, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ __func::target", t_6EFAAC);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_: __func::target_type
static uint32_t t_6EFACC() { return 0xfa1de0u; }
RW_FUNCTION(0x6EFACC, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ __func::target_type", t_6EFACC);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_: __func::~__func (deleting)
static void t_6EFAE0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EFAE0, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ __func::~__func (deleting)", t_6EFAE0);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_: __func::__clone()
static uint32_t t_6EFAE8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69908u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6EFAE8, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ __func::__clone()", t_6EFAE8);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_: __func::__clone(__base*)
static void t_6EFB14(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69908u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6EFB14, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ __func::__clone(__base*)", t_6EFB14);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_: __func::destroy
static void t_6EFB34(uint32_t self) {
}
RW_FUNCTION(0x6EFB34, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ __func::destroy", t_6EFB34);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_: __func::destroy_deallocate
static void t_6EFB36(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EFB36, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ __func::destroy_deallocate", t_6EFB36);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_: __func::target
static uint32_t t_6EFCC4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe889d0u ? self + 4 : 0; }
RW_FUNCTION(0x6EFCC4, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ __func::target", t_6EFCC4);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_: __func::target_type
static uint32_t t_6EFCE4() { return 0xfa1df4u; }
RW_FUNCTION(0x6EFCE4, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ __func::target_type", t_6EFCE4);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_: __func::~__func (deleting)
static void t_6EFCF8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EFCF8, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ __func::~__func (deleting)", t_6EFCF8);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_: __func::__clone()
static uint32_t t_6EFD00(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69934u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6EFD00, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ __func::__clone()", t_6EFD00);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_: __func::__clone(__base*)
static void t_6EFD2C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69934u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6EFD2C, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ __func::__clone(__base*)", t_6EFD2C);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_: __func::destroy
static void t_6EFD4C(uint32_t self) {
}
RW_FUNCTION(0x6EFD4C, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ __func::destroy", t_6EFD4C);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_: __func::destroy_deallocate
static void t_6EFD4E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EFD4E, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ __func::destroy_deallocate", t_6EFD4E);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_: __func::target
static uint32_t t_6EFE14(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88b30u ? self + 4 : 0; }
RW_FUNCTION(0x6EFE14, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ __func::target", t_6EFE14);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_: __func::target_type
static uint32_t t_6EFE34() { return 0xfa1e08u; }
RW_FUNCTION(0x6EFE34, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ __func::target_type", t_6EFE34);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_: __func::~__func (deleting)
static void t_6EFE48(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EFE48, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_ __func::~__func (deleting)", t_6EFE48);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_: __func::__clone()
static uint32_t t_6EFE50(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69960u);
    }
    return v1;
}
RW_FUNCTION(0x6EFE50, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_ __func::__clone()", t_6EFE50);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_: __func::__clone(__base*)
static void t_6EFE74(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69960u);
    }
}
RW_FUNCTION(0x6EFE74, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_ __func::__clone(__base*)", t_6EFE74);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_: __func::destroy
static void t_6EFE90(uint32_t self) {
}
RW_FUNCTION(0x6EFE90, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_ __func::destroy", t_6EFE90);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_: __func::destroy_deallocate
static void t_6EFE92(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EFE92, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_ __func::destroy_deallocate", t_6EFE92);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_: __func::target
static uint32_t t_6EFFBC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88cb0u ? self + 4 : 0; }
RW_FUNCTION(0x6EFFBC, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_ __func::target", t_6EFFBC);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_: __func::target_type
static uint32_t t_6EFFDC() { return 0xfa1e1cu; }
RW_FUNCTION(0x6EFFDC, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE0_ UlvE_ __func::target_type", t_6EFFDC);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6EFFF0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EFFF0, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6EFFF0);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6EFFF8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6998cu);
    }
    return v1;
}
RW_FUNCTION(0x6EFFF8, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_ __func::__clone()", t_6EFFF8);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6F001C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6998cu);
    }
}
RW_FUNCTION(0x6F001C, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6F001C);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6F0038(uint32_t self) {
}
RW_FUNCTION(0x6F0038, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_ __func::destroy", t_6F0038);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6F003A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F003A, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6F003A);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6F0064(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88e30u ? self + 4 : 0; }
RW_FUNCTION(0x6F0064, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_ __func::target", t_6F0064);

// ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6F0084() { return 0xfa1e30u; }
RW_FUNCTION(0x6F0084, "ScenePartyManagement::runInJoinLayout $_10 UlvE_ UlvE_ UlbE_ UlbE_ __func::target_type", t_6F0084);

// ScenePartyManagement::init $_9: __func::~__func (deleting)
static void t_6F0098(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F0098, "ScenePartyManagement::init $_9 __func::~__func (deleting)", t_6F0098);

// ScenePartyManagement::init $_9: __func::__clone()
static uint32_t t_6F00A0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf699b8u);
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
RW_FUNCTION(0x6F00A0, "ScenePartyManagement::init $_9 __func::__clone()", t_6F00A0);

// ScenePartyManagement::init $_9: __func::__clone(__base*)
static void t_6F00D4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf699b8u);
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
RW_FUNCTION(0x6F00D4, "ScenePartyManagement::init $_9 __func::__clone(__base*)", t_6F00D4);

// ScenePartyManagement::init $_9: __func::destroy
static void t_6F0100(uint32_t self) {
}
RW_FUNCTION(0x6F0100, "ScenePartyManagement::init $_9 __func::destroy", t_6F0100);

// ScenePartyManagement::init $_9: __func::destroy_deallocate
static void t_6F0102(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F0102, "ScenePartyManagement::init $_9 __func::destroy_deallocate", t_6F0102);

// ScenePartyManagement::init $_9: __func::target
static uint32_t t_6F0124(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe88f60u ? self + 4 : 0; }
RW_FUNCTION(0x6F0124, "ScenePartyManagement::init $_9 __func::target", t_6F0124);

// ScenePartyManagement::init $_9: __func::target_type
static uint32_t t_6F0144() { return 0xfa1e44u; }
RW_FUNCTION(0x6F0144, "ScenePartyManagement::init $_9 __func::target_type", t_6F0144);

// ScenePartyManagement::init $_8: __func::~__func (deleting)
static void t_6F0158(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F0158, "ScenePartyManagement::init $_8 __func::~__func (deleting)", t_6F0158);

// ScenePartyManagement::init $_8: __func::__clone()
static uint32_t t_6F0160(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf699e4u);
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
RW_FUNCTION(0x6F0160, "ScenePartyManagement::init $_8 __func::__clone()", t_6F0160);

// ScenePartyManagement::init $_8: __func::__clone(__base*)
static void t_6F0194(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf699e4u);
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
RW_FUNCTION(0x6F0194, "ScenePartyManagement::init $_8 __func::__clone(__base*)", t_6F0194);

// ScenePartyManagement::init $_8: __func::destroy
static void t_6F01C0(uint32_t self) {
}
RW_FUNCTION(0x6F01C0, "ScenePartyManagement::init $_8 __func::destroy", t_6F01C0);

// ScenePartyManagement::init $_8: __func::destroy_deallocate
static void t_6F01C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F01C2, "ScenePartyManagement::init $_8 __func::destroy_deallocate", t_6F01C2);

// ScenePartyManagement::init $_8: __func::target
static uint32_t t_6F01E4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe89020u ? self + 4 : 0; }
RW_FUNCTION(0x6F01E4, "ScenePartyManagement::init $_8 __func::target", t_6F01E4);

// ScenePartyManagement::init $_8: __func::target_type
static uint32_t t_6F0204() { return 0xfa1e58u; }
RW_FUNCTION(0x6F0204, "ScenePartyManagement::init $_8 __func::target_type", t_6F0204);

// ScenePartyManagement::init $_6: __func::~__func (deleting)
static void t_6F0218(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F0218, "ScenePartyManagement::init $_6 __func::~__func (deleting)", t_6F0218);

// ScenePartyManagement::init $_6: __func::__clone()
static uint32_t t_6F0220(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69a10u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F0220, "ScenePartyManagement::init $_6 __func::__clone()", t_6F0220);

// ScenePartyManagement::init $_6: __func::__clone(__base*)
static void t_6F024C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69a10u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F024C, "ScenePartyManagement::init $_6 __func::__clone(__base*)", t_6F024C);

// ScenePartyManagement::init $_6: __func::destroy
static void t_6F026C(uint32_t self) {
}
RW_FUNCTION(0x6F026C, "ScenePartyManagement::init $_6 __func::destroy", t_6F026C);

// ScenePartyManagement::init $_6: __func::destroy_deallocate
static void t_6F026E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F026E, "ScenePartyManagement::init $_6 __func::destroy_deallocate", t_6F026E);

// ScenePartyManagement::init $_6: __func::target
static uint32_t t_6F0348(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe890d0u ? self + 4 : 0; }
RW_FUNCTION(0x6F0348, "ScenePartyManagement::init $_6 __func::target", t_6F0348);

// ScenePartyManagement::init $_6: __func::target_type
static uint32_t t_6F0368() { return 0xfa1e6cu; }
RW_FUNCTION(0x6F0368, "ScenePartyManagement::init $_6 __func::target_type", t_6F0368);

// ScenePartyManagement::init $_5: __func::~__func (deleting)
static void t_6F037C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F037C, "ScenePartyManagement::init $_5 __func::~__func (deleting)", t_6F037C);

// ScenePartyManagement::init $_5: __func::__clone()
static uint32_t t_6F0384(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69a3cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F0384, "ScenePartyManagement::init $_5 __func::__clone()", t_6F0384);

// ScenePartyManagement::init $_5: __func::__clone(__base*)
static void t_6F03B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69a3cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F03B0, "ScenePartyManagement::init $_5 __func::__clone(__base*)", t_6F03B0);

// ScenePartyManagement::init $_5: __func::destroy
static void t_6F03D0(uint32_t self) {
}
RW_FUNCTION(0x6F03D0, "ScenePartyManagement::init $_5 __func::destroy", t_6F03D0);

// ScenePartyManagement::init $_5: __func::destroy_deallocate
static void t_6F03D2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F03D2, "ScenePartyManagement::init $_5 __func::destroy_deallocate", t_6F03D2);

// ScenePartyManagement::init $_5: __func::target
static uint32_t t_6F03F4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe89190u ? self + 4 : 0; }
RW_FUNCTION(0x6F03F4, "ScenePartyManagement::init $_5 __func::target", t_6F03F4);

// ScenePartyManagement::init $_5: __func::target_type
static uint32_t t_6F0414() { return 0xfa1e80u; }
RW_FUNCTION(0x6F0414, "ScenePartyManagement::init $_5 __func::target_type", t_6F0414);

// ScenePartyManagement::init $_4: __func::~__func (deleting)
static void t_6F0428(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F0428, "ScenePartyManagement::init $_4 __func::~__func (deleting)", t_6F0428);

// ScenePartyManagement::init $_4: __func::__clone()
static uint32_t t_6F0430(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69a68u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F0430, "ScenePartyManagement::init $_4 __func::__clone()", t_6F0430);

// ScenePartyManagement::init $_4: __func::__clone(__base*)
static void t_6F045C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69a68u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F045C, "ScenePartyManagement::init $_4 __func::__clone(__base*)", t_6F045C);

// ScenePartyManagement::init $_4: __func::destroy
static void t_6F047C(uint32_t self) {
}
RW_FUNCTION(0x6F047C, "ScenePartyManagement::init $_4 __func::destroy", t_6F047C);

// ScenePartyManagement::init $_4: __func::destroy_deallocate
static void t_6F047E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F047E, "ScenePartyManagement::init $_4 __func::destroy_deallocate", t_6F047E);

// ScenePartyManagement::init $_4: __func::target
static uint32_t t_6F0490(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe89220u ? self + 4 : 0; }
RW_FUNCTION(0x6F0490, "ScenePartyManagement::init $_4 __func::target", t_6F0490);

// ScenePartyManagement::init $_4: __func::target_type
static uint32_t t_6F04B0() { return 0xfa1e94u; }
RW_FUNCTION(0x6F04B0, "ScenePartyManagement::init $_4 __func::target_type", t_6F04B0);

// ScenePartyManagement::init $_3: __func::~__func (deleting)
static void t_6F04C4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F04C4, "ScenePartyManagement::init $_3 __func::~__func (deleting)", t_6F04C4);

// ScenePartyManagement::init $_3: __func::__clone()
static uint32_t t_6F04CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69a94u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F04CC, "ScenePartyManagement::init $_3 __func::__clone()", t_6F04CC);

// ScenePartyManagement::init $_3: __func::__clone(__base*)
static void t_6F04F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69a94u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F04F8, "ScenePartyManagement::init $_3 __func::__clone(__base*)", t_6F04F8);

// ScenePartyManagement::init $_3: __func::destroy
static void t_6F0518(uint32_t self) {
}
RW_FUNCTION(0x6F0518, "ScenePartyManagement::init $_3 __func::destroy", t_6F0518);

// ScenePartyManagement::init $_3: __func::destroy_deallocate
static void t_6F051A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F051A, "ScenePartyManagement::init $_3 __func::destroy_deallocate", t_6F051A);

// ScenePartyManagement::init $_3: __func::target
static uint32_t t_6F052C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe892b0u ? self + 4 : 0; }
RW_FUNCTION(0x6F052C, "ScenePartyManagement::init $_3 __func::target", t_6F052C);

// ScenePartyManagement::init $_3: __func::target_type
static uint32_t t_6F054C() { return 0xfa1ea8u; }
RW_FUNCTION(0x6F054C, "ScenePartyManagement::init $_3 __func::target_type", t_6F054C);

// ScenePartyManagement::init $_2: __func::~__func (deleting)
static void t_6F0560(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F0560, "ScenePartyManagement::init $_2 __func::~__func (deleting)", t_6F0560);

// ScenePartyManagement::init $_2: __func::__clone()
static uint32_t t_6F0568(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69ac0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F0568, "ScenePartyManagement::init $_2 __func::__clone()", t_6F0568);

// ScenePartyManagement::init $_2: __func::__clone(__base*)
static void t_6F0594(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69ac0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F0594, "ScenePartyManagement::init $_2 __func::__clone(__base*)", t_6F0594);

// ScenePartyManagement::init $_2: __func::destroy
static void t_6F05B4(uint32_t self) {
}
RW_FUNCTION(0x6F05B4, "ScenePartyManagement::init $_2 __func::destroy", t_6F05B4);

// ScenePartyManagement::init $_2: __func::destroy_deallocate
static void t_6F05B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F05B6, "ScenePartyManagement::init $_2 __func::destroy_deallocate", t_6F05B6);

// ScenePartyManagement::init $_2: __func::target
static uint32_t t_6F05C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe89340u ? self + 4 : 0; }
RW_FUNCTION(0x6F05C8, "ScenePartyManagement::init $_2 __func::target", t_6F05C8);

// ScenePartyManagement::init $_2: __func::target_type
static uint32_t t_6F05E8() { return 0xfa1ebcu; }
RW_FUNCTION(0x6F05E8, "ScenePartyManagement::init $_2 __func::target_type", t_6F05E8);

// ScenePartyManagement::callAPIs $_1: __func::~__func
static uint32_t t_6F05FC(uint32_t self) {
    st32(self, 0xf69aecu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6F05FC, "ScenePartyManagement::callAPIs $_1 __func::~__func", t_6F05FC);

// ScenePartyManagement::callAPIs $_1: __func::~__func (deleting)
static void t_6F0638(uint32_t self) {
    st32(self, 0xf69aecu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F0638, "ScenePartyManagement::callAPIs $_1 __func::~__func (deleting)", t_6F0638);

// ScenePartyManagement::callAPIs $_1: __func::__clone()
static uint32_t t_6F0678(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf69aecu);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6F0678, "ScenePartyManagement::callAPIs $_1 __func::__clone()", t_6F0678);

// ScenePartyManagement::callAPIs $_1: __func::__clone(__base*)
static void t_6F06C8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69aecu);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x6F06C8, "ScenePartyManagement::callAPIs $_1 __func::__clone(__base*)", t_6F06C8);

// ScenePartyManagement::callAPIs $_1: __func::destroy
static void t_6F06FC(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6F06FC, "ScenePartyManagement::callAPIs $_1 __func::destroy", t_6F06FC);

// ScenePartyManagement::callAPIs $_1: __func::destroy_deallocate
static void t_6F071C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F071C, "ScenePartyManagement::callAPIs $_1 __func::destroy_deallocate", t_6F071C);

// ScenePartyManagement::callAPIs $_1: __func::target
static uint32_t t_6F0854(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe893e0u ? self + 8 : 0; }
RW_FUNCTION(0x6F0854, "ScenePartyManagement::callAPIs $_1 __func::target", t_6F0854);

// ScenePartyManagement::callAPIs $_1: __func::target_type
static uint32_t t_6F0874() { return 0xfa1ed0u; }
RW_FUNCTION(0x6F0874, "ScenePartyManagement::callAPIs $_1 __func::target_type", t_6F0874);

// ScenePartyManagement::callAPIs $_0: __func::~__func
static uint32_t t_6F0888(uint32_t self) {
    st32(self, 0xf69b18u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6F0888, "ScenePartyManagement::callAPIs $_0 __func::~__func", t_6F0888);

// ScenePartyManagement::callAPIs $_0: __func::~__func (deleting)
static void t_6F08C4(uint32_t self) {
    st32(self, 0xf69b18u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F08C4, "ScenePartyManagement::callAPIs $_0 __func::~__func (deleting)", t_6F08C4);

// ScenePartyManagement::callAPIs $_0: __func::__clone()
static uint32_t t_6F0904(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf69b18u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6F0904, "ScenePartyManagement::callAPIs $_0 __func::__clone()", t_6F0904);

// ScenePartyManagement::callAPIs $_0: __func::__clone(__base*)
static void t_6F0954(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69b18u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x6F0954, "ScenePartyManagement::callAPIs $_0 __func::__clone(__base*)", t_6F0954);

// ScenePartyManagement::callAPIs $_0: __func::destroy
static void t_6F0988(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6F0988, "ScenePartyManagement::callAPIs $_0 __func::destroy", t_6F0988);

// ScenePartyManagement::callAPIs $_0: __func::destroy_deallocate
static void t_6F09A8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F09A8, "ScenePartyManagement::callAPIs $_0 __func::destroy_deallocate", t_6F09A8);

// ScenePartyManagement::callAPIs $_0: __func::target
static uint32_t t_6F09EC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe89490u ? self + 8 : 0; }
RW_FUNCTION(0x6F09EC, "ScenePartyManagement::callAPIs $_0 __func::target", t_6F09EC);

// ScenePartyManagement::callAPIs $_0: __func::target_type
static uint32_t t_6F0A0C() { return 0xfa1ee4u; }
RW_FUNCTION(0x6F0A0C, "ScenePartyManagement::callAPIs $_0 __func::target_type", t_6F0A0C);

// ---- further thunks attributed to ScenePartyManagement (wave 3)

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_: __func::~__func
static uint32_t t_6E2F64(uint32_t self) {
    st32(self, 0xf68ac4u);
    uint32_t v1 = ld8(self + 120);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 128);
        call(fn::operator_delete, {v2});
    }
    call(0x468499, {self + 88});   // _ZNSt3__113__vector_baseIyNS_9allocatorIyEEED2Ev
    uint32_t v3 = ld8(self + 60);
    if (v3 & 1) {
        uint32_t v4 = ld32(self + 68);
        call(fn::operator_delete, {v4});
    }
    uint32_t v5 = ld8(self + 40);
    if (v5 & 1) {
        uint32_t v6 = ld32(self + 48);
        call(fn::operator_delete, {v6});
    }
    return self;
}
RW_FUNCTION(0x6E2F64, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ __func::~__func", t_6E2F64);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6E2FBC(uint32_t self) {
    st32(self, 0xf68ac4u);
    uint32_t v1 = ld8(self + 120);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 128);
        call(fn::operator_delete, {v2});
    }
    call(0x468499, {self + 88});   // _ZNSt3__113__vector_baseIyNS_9allocatorIyEEED2Ev
    uint32_t v3 = ld8(self + 60);
    if (v3 & 1) {
        uint32_t v4 = ld32(self + 68);
        call(fn::operator_delete, {v4});
    }
    uint32_t v5 = ld8(self + 40);
    if (v5 & 1) {
        uint32_t v6 = ld32(self + 48);
        call(fn::operator_delete, {v6});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E2FBC, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ __func::~__func (deleting)", t_6E2FBC);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_: __func::destroy
static void t_6E30E4(uint32_t self) {
    uint32_t v1 = ld8(self + 120);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 128);
        call(fn::operator_delete, {v2});
    }
    call(0x468499, {self + 88});   // _ZNSt3__113__vector_baseIyNS_9allocatorIyEEED2Ev
    uint32_t v3 = ld8(self + 60);
    if (v3 & 1) {
        uint32_t v4 = ld32(self + 68);
        call(fn::operator_delete, {v4});
    }
    uint32_t v5 = ld8(self + 40);
    if (v5 & 1) {
        uint32_t v6 = ld32(self + 48);
        call(fn::operator_delete, {v6});
    }
}
RW_FUNCTION(0x6E30E4, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ __func::destroy", t_6E30E4);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6E3124(uint32_t self) {
    uint32_t v1 = ld8(self + 120);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 128);
        call(fn::operator_delete, {v2});
    }
    call(0x468499, {self + 88});   // _ZNSt3__113__vector_baseIyNS_9allocatorIyEEED2Ev
    uint32_t v3 = ld8(self + 60);
    if (v3 & 1) {
        uint32_t v4 = ld32(self + 68);
        call(fn::operator_delete, {v4});
    }
    uint32_t v5 = ld8(self + 40);
    if (v5 & 1) {
        uint32_t v6 = ld32(self + 48);
        call(fn::operator_delete, {v6});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3124, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ __func::destroy_deallocate", t_6E3124);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_: __func::~__func
static uint32_t t_6E3590(uint32_t self) {
    st32(self, 0xf68b1cu);
    uint32_t v1 = ld8(self + 120);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 128);
        call(fn::operator_delete, {v2});
    }
    call(0x468499, {self + 88});   // _ZNSt3__113__vector_baseIyNS_9allocatorIyEEED2Ev
    uint32_t v3 = ld8(self + 60);
    if (v3 & 1) {
        uint32_t v4 = ld32(self + 68);
        call(fn::operator_delete, {v4});
    }
    uint32_t v5 = ld8(self + 40);
    if (v5 & 1) {
        uint32_t v6 = ld32(self + 48);
        call(fn::operator_delete, {v6});
    }
    return self;
}
RW_FUNCTION(0x6E3590, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ __func::~__func", t_6E3590);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_: __func::~__func (deleting)
static void t_6E35E8(uint32_t self) {
    st32(self, 0xf68b1cu);
    uint32_t v1 = ld8(self + 120);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 128);
        call(fn::operator_delete, {v2});
    }
    call(0x468499, {self + 88});   // _ZNSt3__113__vector_baseIyNS_9allocatorIyEEED2Ev
    uint32_t v3 = ld8(self + 60);
    if (v3 & 1) {
        uint32_t v4 = ld32(self + 68);
        call(fn::operator_delete, {v4});
    }
    uint32_t v5 = ld8(self + 40);
    if (v5 & 1) {
        uint32_t v6 = ld32(self + 48);
        call(fn::operator_delete, {v6});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E35E8, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ __func::~__func (deleting)", t_6E35E8);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_: __func::destroy
static void t_6E3710(uint32_t self) {
    uint32_t v1 = ld8(self + 120);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 128);
        call(fn::operator_delete, {v2});
    }
    call(0x468499, {self + 88});   // _ZNSt3__113__vector_baseIyNS_9allocatorIyEEED2Ev
    uint32_t v3 = ld8(self + 60);
    if (v3 & 1) {
        uint32_t v4 = ld32(self + 68);
        call(fn::operator_delete, {v4});
    }
    uint32_t v5 = ld8(self + 40);
    if (v5 & 1) {
        uint32_t v6 = ld32(self + 48);
        call(fn::operator_delete, {v6});
    }
}
RW_FUNCTION(0x6E3710, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ __func::destroy", t_6E3710);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_: __func::destroy_deallocate
static void t_6E3750(uint32_t self) {
    uint32_t v1 = ld8(self + 120);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 128);
        call(fn::operator_delete, {v2});
    }
    call(0x468499, {self + 88});   // _ZNSt3__113__vector_baseIyNS_9allocatorIyEEED2Ev
    uint32_t v3 = ld8(self + 60);
    if (v3 & 1) {
        uint32_t v4 = ld32(self + 68);
        call(fn::operator_delete, {v4});
    }
    uint32_t v5 = ld8(self + 40);
    if (v5 & 1) {
        uint32_t v6 = ld32(self + 48);
        call(fn::operator_delete, {v6});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3750, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ __func::destroy_deallocate", t_6E3750);

// ScenePartyManagement::runInEntryLayout $_16: __func::~__func
static uint32_t t_6EA514(uint32_t self) {
    st32(self, 0xf6935cu);
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b3e5d, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEED2Ev
    return self;
}
RW_FUNCTION(0x6EA514, "ScenePartyManagement::runInEntryLayout $_16 __func::~__func", t_6EA514);

// ScenePartyManagement::runInEntryLayout $_16: __func::~__func (deleting)
static void t_6EA548(uint32_t self) {
    st32(self, 0xf6935cu);
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b3e5d, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EA548, "ScenePartyManagement::runInEntryLayout $_16 __func::~__func (deleting)", t_6EA548);

// ScenePartyManagement::runInEntryLayout $_16: __func::__clone()
static uint32_t t_6EA580(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf6935cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        call(0x5232d1, {v1 + 8, self + 8});   // _ZNSt3__16vectorIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEEC2ERKS8_
        uint32_t v3 = ld32(self + 4);
        st32(v1 + 4, v3);
        uint32_t v4 = ld32(self + 20);
        st32(v1 + 20, v4);
        uint32_t v5 = ld32(self + 24);
        st32(v1 + 24, v5);
        uint32_t m7 = v3;
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
            uint32_t v6 = ld32(self + 4);
            m7 = v6;
        }
        st32(v1 + 4, m7);
    }
    return v1;
}
RW_FUNCTION(0x6EA580, "ScenePartyManagement::runInEntryLayout $_16 __func::__clone()", t_6EA580);

// ScenePartyManagement::runInEntryLayout $_16: __func::__clone(__base*)
static void t_6EA5EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6935cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        call(0x5232d1, {p + 8, self + 8});   // _ZNSt3__16vectorIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEEC2ERKS8_
        uint32_t v2 = ld32(self + 4);
        st32(p + 4, v2);
        uint32_t v3 = ld32(self + 20);
        st32(p + 20, v3);
        uint32_t v4 = ld32(self + 24);
        st32(p + 24, v4);
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
        }
        uint32_t v5 = ld32(self + 4);
        st32(p + 4, v5);
    }
}
RW_FUNCTION(0x6EA5EC, "ScenePartyManagement::runInEntryLayout $_16 __func::__clone(__base*)", t_6EA5EC);

// ScenePartyManagement::runInEntryLayout $_16: __func::destroy
static void t_6EA640(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b3e5d, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEED2Ev
}
RW_FUNCTION(0x6EA640, "ScenePartyManagement::runInEntryLayout $_16 __func::destroy", t_6EA640);

// ScenePartyManagement::runInEntryLayout $_16: __func::destroy_deallocate
static void t_6EA65C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b3e5d, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EA65C, "ScenePartyManagement::runInEntryLayout $_16 __func::destroy_deallocate", t_6EA65C);

// ScenePartyManagement::runInEntryLayout $_15: __func::~__func
static uint32_t t_6EB398(uint32_t self) {
    st32(self, 0xf69438u);
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b3e5d, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEED2Ev
    return self;
}
RW_FUNCTION(0x6EB398, "ScenePartyManagement::runInEntryLayout $_15 __func::~__func", t_6EB398);

// ScenePartyManagement::runInEntryLayout $_15: __func::~__func (deleting)
static void t_6EB3CC(uint32_t self) {
    st32(self, 0xf69438u);
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b3e5d, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EB3CC, "ScenePartyManagement::runInEntryLayout $_15 __func::~__func (deleting)", t_6EB3CC);

// ScenePartyManagement::runInEntryLayout $_15: __func::__clone()
static uint32_t t_6EB404(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf69438u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        call(0x5232d1, {v1 + 8, self + 8});   // _ZNSt3__16vectorIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEEC2ERKS8_
        uint32_t v3 = ld32(self + 4);
        st32(v1 + 4, v3);
        uint32_t v4 = ld32(self + 20);
        st32(v1 + 20, v4);
        uint32_t v5 = ld32(self + 24);
        st32(v1 + 24, v5);
        uint32_t m7 = v3;
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
            uint32_t v6 = ld32(self + 4);
            m7 = v6;
        }
        st32(v1 + 4, m7);
    }
    return v1;
}
RW_FUNCTION(0x6EB404, "ScenePartyManagement::runInEntryLayout $_15 __func::__clone()", t_6EB404);

// ScenePartyManagement::runInEntryLayout $_15: __func::__clone(__base*)
static void t_6EB470(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69438u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        call(0x5232d1, {p + 8, self + 8});   // _ZNSt3__16vectorIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEEC2ERKS8_
        uint32_t v2 = ld32(self + 4);
        st32(p + 4, v2);
        uint32_t v3 = ld32(self + 20);
        st32(p + 20, v3);
        uint32_t v4 = ld32(self + 24);
        st32(p + 24, v4);
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
        }
        uint32_t v5 = ld32(self + 4);
        st32(p + 4, v5);
    }
}
RW_FUNCTION(0x6EB470, "ScenePartyManagement::runInEntryLayout $_15 __func::__clone(__base*)", t_6EB470);

// ScenePartyManagement::runInEntryLayout $_15: __func::destroy
static void t_6EB4C4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b3e5d, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEED2Ev
}
RW_FUNCTION(0x6EB4C4, "ScenePartyManagement::runInEntryLayout $_15 __func::destroy", t_6EB4C4);

// ScenePartyManagement::runInEntryLayout $_15: __func::destroy_deallocate
static void t_6EB4E0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b3e5d, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api15PartyInviteList5OfferENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EB4E0, "ScenePartyManagement::runInEntryLayout $_15 __func::destroy_deallocate", t_6EB4E0);

// ScenePartyManagement::runInEntryLayout $_14: __func::~__func
static uint32_t t_6EC11C(uint32_t self) {
    st32(self, 0xf69514u);
    call(0x4b4453, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEED2Ev
    return self;
}
RW_FUNCTION(0x6EC11C, "ScenePartyManagement::runInEntryLayout $_14 __func::~__func", t_6EC11C);

// ScenePartyManagement::runInEntryLayout $_14: __func::~__func (deleting)
static void t_6EC148(uint32_t self) {
    st32(self, 0xf69514u);
    call(0x4b4453, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EC148, "ScenePartyManagement::runInEntryLayout $_14 __func::~__func (deleting)", t_6EC148);

// ScenePartyManagement::runInEntryLayout $_14: __func::__clone()
static uint32_t t_6EC178(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf69514u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        call(0x523325, {v1 + 8, self + 8});   // _ZNSt3__16vectorIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEEC2ERKS8_
        uint32_t v3 = ld32(self + 4);
        st32(v1 + 4, v3);
    }
    return v1;
}
RW_FUNCTION(0x6EC178, "ScenePartyManagement::runInEntryLayout $_14 __func::__clone()", t_6EC178);

// ScenePartyManagement::runInEntryLayout $_14: __func::__clone(__base*)
static void t_6EC1D0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69514u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        call(0x523325, {p + 8, self + 8});   // _ZNSt3__16vectorIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEEC2ERKS8_
        uint32_t v2 = ld32(self + 4);
        st32(p + 4, v2);
    }
}
RW_FUNCTION(0x6EC1D0, "ScenePartyManagement::runInEntryLayout $_14 __func::__clone(__base*)", t_6EC1D0);

// ScenePartyManagement::runInEntryLayout $_14: __func::destroy
static void t_6EC210(uint32_t self) {
    call(0x4b4453, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEED2Ev
}
RW_FUNCTION(0x6EC210, "ScenePartyManagement::runInEntryLayout $_14 __func::destroy", t_6EC210);

// ScenePartyManagement::runInEntryLayout $_14: __func::destroy_deallocate
static void t_6EC21A(uint32_t self) {
    call(0x4b4453, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6EC21A, "ScenePartyManagement::runInEntryLayout $_14 __func::destroy_deallocate", t_6EC21A);

// ScenePartyManagement::runInJoinLayout $_13: __func::~__func
static uint32_t t_6ECD08(uint32_t self) {
    st32(self, 0xf695f0u);
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b4453, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEED2Ev
    return self;
}
RW_FUNCTION(0x6ECD08, "ScenePartyManagement::runInJoinLayout $_13 __func::~__func", t_6ECD08);

// ScenePartyManagement::runInJoinLayout $_13: __func::~__func (deleting)
static void t_6ECD3C(uint32_t self) {
    st32(self, 0xf695f0u);
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b4453, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ECD3C, "ScenePartyManagement::runInJoinLayout $_13 __func::~__func (deleting)", t_6ECD3C);

// ScenePartyManagement::runInJoinLayout $_13: __func::__clone()
static uint32_t t_6ECD74(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf695f0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        call(0x523325, {v1 + 8, self + 8});   // _ZNSt3__16vectorIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEEC2ERKS8_
        uint32_t v3 = ld32(self + 4);
        st32(v1 + 4, v3);
        uint32_t v4 = ld32(self + 20);
        st32(v1 + 20, v4);
        uint32_t v5 = ld32(self + 24);
        st32(v1 + 24, v5);
        uint32_t m7 = v3;
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
            uint32_t v6 = ld32(self + 4);
            m7 = v6;
        }
        st32(v1 + 4, m7);
    }
    return v1;
}
RW_FUNCTION(0x6ECD74, "ScenePartyManagement::runInJoinLayout $_13 __func::__clone()", t_6ECD74);

// ScenePartyManagement::runInJoinLayout $_13: __func::__clone(__base*)
static void t_6ECDE0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf695f0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        call(0x523325, {p + 8, self + 8});   // _ZNSt3__16vectorIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEEC2ERKS8_
        uint32_t v2 = ld32(self + 4);
        st32(p + 4, v2);
        uint32_t v3 = ld32(self + 20);
        st32(p + 20, v3);
        uint32_t v4 = ld32(self + 24);
        st32(p + 24, v4);
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
        }
        uint32_t v5 = ld32(self + 4);
        st32(p + 4, v5);
    }
}
RW_FUNCTION(0x6ECDE0, "ScenePartyManagement::runInJoinLayout $_13 __func::__clone(__base*)", t_6ECDE0);

// ScenePartyManagement::runInJoinLayout $_13: __func::destroy
static void t_6ECE34(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b4453, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEED2Ev
}
RW_FUNCTION(0x6ECE34, "ScenePartyManagement::runInJoinLayout $_13 __func::destroy", t_6ECE34);

// ScenePartyManagement::runInJoinLayout $_13: __func::destroy_deallocate
static void t_6ECE50(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(0x4b4453, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api16PartyApplication9ApplicantENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6ECE50, "ScenePartyManagement::runInJoinLayout $_13 __func::destroy_deallocate", t_6ECE50);
