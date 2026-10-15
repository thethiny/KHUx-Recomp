// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ScenePartyRequest lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ScenePartyRequest::backScene $_84: __func::~__func (deleting)
static void t_6DD374(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD374, "ScenePartyRequest::backScene $_84 __func::~__func (deleting)", t_6DD374);

// ScenePartyRequest::backScene $_84: __func::__clone()
static uint32_t t_6DD37C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67e64u);
    }
    return v1;
}
RW_FUNCTION(0x6DD37C, "ScenePartyRequest::backScene $_84 __func::__clone()", t_6DD37C);

// ScenePartyRequest::backScene $_84: __func::__clone(__base*)
static void t_6DD3A0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67e64u);
    }
}
RW_FUNCTION(0x6DD3A0, "ScenePartyRequest::backScene $_84 __func::__clone(__base*)", t_6DD3A0);

// ScenePartyRequest::backScene $_84: __func::destroy
static void t_6DD3BC(uint32_t self) {
}
RW_FUNCTION(0x6DD3BC, "ScenePartyRequest::backScene $_84 __func::destroy", t_6DD3BC);

// ScenePartyRequest::backScene $_84: __func::destroy_deallocate
static void t_6DD3BE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD3BE, "ScenePartyRequest::backScene $_84 __func::destroy_deallocate", t_6DD3BE);

// ScenePartyRequest::backScene $_84: __func::target
static uint32_t t_6DD4E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80790u ? self + 4 : 0; }
RW_FUNCTION(0x6DD4E8, "ScenePartyRequest::backScene $_84 __func::target", t_6DD4E8);

// ScenePartyRequest::backScene $_84: __func::target_type
static uint32_t t_6DD508() { return 0xfa0e84u; }
RW_FUNCTION(0x6DD508, "ScenePartyRequest::backScene $_84 __func::target_type", t_6DD508);

// ScenePartyRequest::openPopUp $_83: __func::~__func
static uint32_t t_6DD51C(uint32_t self) {
    st32(self, 0xf67e90u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x6DD51C, "ScenePartyRequest::openPopUp $_83 __func::~__func", t_6DD51C);

// ScenePartyRequest::openPopUp $_83: __func::~__func (deleting)
static void t_6DD558(uint32_t self) {
    st32(self, 0xf67e90u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD558, "ScenePartyRequest::openPopUp $_83 __func::~__func (deleting)", t_6DD558);

// ScenePartyRequest::openPopUp $_83: __func::__clone()
static uint32_t t_6DD598(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf67e90u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 20, v5);
        st32(v1 + 16, v4);
        st32(v1 + 12, v3);
        st32(v1 + 8, v2);
        uint32_t v6 = ld32(self + 24);
        st32(v1 + 24, v6);
        call(0x4683cd, {v1 + 32, self + 32});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6DD598, "ScenePartyRequest::openPopUp $_83 __func::__clone()", t_6DD598);

// ScenePartyRequest::openPopUp $_83: __func::__clone(__base*)
static void t_6DD5F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67e90u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        uint32_t v5 = ld32(self + 24);
        st32(p + 24, v5);
        call(0x4683cd, {p + 32, self + 32});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x6DD5F8, "ScenePartyRequest::openPopUp $_83 __func::__clone(__base*)", t_6DD5F8);

// ScenePartyRequest::openPopUp $_83: __func::destroy
static void t_6DD63C(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x6DD63C, "ScenePartyRequest::openPopUp $_83 __func::destroy", t_6DD63C);

// ScenePartyRequest::openPopUp $_83: __func::destroy_deallocate
static void t_6DD65C(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD65C, "ScenePartyRequest::openPopUp $_83 __func::destroy_deallocate", t_6DD65C);

// ScenePartyRequest::openPopUp $_83: __func::target
static uint32_t t_6DD6D4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80840u ? self + 8 : 0; }
RW_FUNCTION(0x6DD6D4, "ScenePartyRequest::openPopUp $_83 __func::target", t_6DD6D4);

// ScenePartyRequest::openPopUp $_83: __func::target_type
static uint32_t t_6DD6F8() { return 0xfa0e98u; }
RW_FUNCTION(0x6DD6F8, "ScenePartyRequest::openPopUp $_83 __func::target_type", t_6DD6F8);

// ScenePartyRequest::openPopUp $_88 UlvE_: __func::~__func (deleting)
static void t_6DD7E0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD7E0, "ScenePartyRequest::openPopUp $_88 UlvE_ __func::~__func (deleting)", t_6DD7E0);

// ScenePartyRequest::openPopUp $_88 UlvE_: __func::__clone()
static uint32_t t_6DD7E8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf67ebcu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6DD7E8, "ScenePartyRequest::openPopUp $_88 UlvE_ __func::__clone()", t_6DD7E8);

// ScenePartyRequest::openPopUp $_88 UlvE_: __func::__clone(__base*)
static void t_6DD818(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67ebcu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DD818, "ScenePartyRequest::openPopUp $_88 UlvE_ __func::__clone(__base*)", t_6DD818);

// ScenePartyRequest::openPopUp $_88 UlvE_: __func::destroy
static void t_6DD840(uint32_t self) {
}
RW_FUNCTION(0x6DD840, "ScenePartyRequest::openPopUp $_88 UlvE_ __func::destroy", t_6DD840);

// ScenePartyRequest::openPopUp $_88 UlvE_: __func::destroy_deallocate
static void t_6DD842(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD842, "ScenePartyRequest::openPopUp $_88 UlvE_ __func::destroy_deallocate", t_6DD842);

// ScenePartyRequest::openPopUp $_88 UlvE_: __func::target
static uint32_t t_6DD874(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80910u ? self + 4 : 0; }
RW_FUNCTION(0x6DD874, "ScenePartyRequest::openPopUp $_88 UlvE_ __func::target", t_6DD874);

// ScenePartyRequest::openPopUp $_88 UlvE_: __func::target_type
static uint32_t t_6DD894() { return 0xfa0eacu; }
RW_FUNCTION(0x6DD894, "ScenePartyRequest::openPopUp $_88 UlvE_ __func::target_type", t_6DD894);

// ScenePartyRequest::openPopUp $_82: __func::~__func (deleting)
static void t_6DD8A8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD8A8, "ScenePartyRequest::openPopUp $_82 __func::~__func (deleting)", t_6DD8A8);

// ScenePartyRequest::openPopUp $_82: __func::__clone()
static uint32_t t_6DD8B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67ee8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6DD8B0, "ScenePartyRequest::openPopUp $_82 __func::__clone()", t_6DD8B0);

// ScenePartyRequest::openPopUp $_82: __func::__clone(__base*)
static void t_6DD8DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67ee8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DD8DC, "ScenePartyRequest::openPopUp $_82 __func::__clone(__base*)", t_6DD8DC);

// ScenePartyRequest::openPopUp $_82: __func::destroy
static void t_6DD8FC(uint32_t self) {
}
RW_FUNCTION(0x6DD8FC, "ScenePartyRequest::openPopUp $_82 __func::destroy", t_6DD8FC);

// ScenePartyRequest::openPopUp $_82: __func::destroy_deallocate
static void t_6DD8FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD8FE, "ScenePartyRequest::openPopUp $_82 __func::destroy_deallocate", t_6DD8FE);

// ScenePartyRequest::openPopUp $_82: __func::target
static uint32_t t_6DD910(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe809f0u ? self + 4 : 0; }
RW_FUNCTION(0x6DD910, "ScenePartyRequest::openPopUp $_82 __func::target", t_6DD910);

// ScenePartyRequest::openPopUp $_82: __func::target_type
static uint32_t t_6DD930() { return 0xfa0ec0u; }
RW_FUNCTION(0x6DD930, "ScenePartyRequest::openPopUp $_82 __func::target_type", t_6DD930);

// ScenePartyRequest::openPopUp $_81: __func::~__func (deleting)
static void t_6DD944(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD944, "ScenePartyRequest::openPopUp $_81 __func::~__func (deleting)", t_6DD944);

// ScenePartyRequest::openPopUp $_81: __func::__clone()
static uint32_t t_6DD94C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67f14u);
    }
    return v1;
}
RW_FUNCTION(0x6DD94C, "ScenePartyRequest::openPopUp $_81 __func::__clone()", t_6DD94C);

// ScenePartyRequest::openPopUp $_81: __func::__clone(__base*)
static void t_6DD970(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67f14u);
    }
}
RW_FUNCTION(0x6DD970, "ScenePartyRequest::openPopUp $_81 __func::__clone(__base*)", t_6DD970);

// ScenePartyRequest::openPopUp $_81: __func::destroy
static void t_6DD98C(uint32_t self) {
}
RW_FUNCTION(0x6DD98C, "ScenePartyRequest::openPopUp $_81 __func::destroy", t_6DD98C);

// ScenePartyRequest::openPopUp $_81: __func::destroy_deallocate
static void t_6DD98E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD98E, "ScenePartyRequest::openPopUp $_81 __func::destroy_deallocate", t_6DD98E);

// ScenePartyRequest::openPopUp $_81: __func::target
static uint32_t t_6DD9AC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80af0u ? self + 4 : 0; }
RW_FUNCTION(0x6DD9AC, "ScenePartyRequest::openPopUp $_81 __func::target", t_6DD9AC);

// ScenePartyRequest::openPopUp $_81: __func::target_type
static uint32_t t_6DD9CC() { return 0xfa0ed4u; }
RW_FUNCTION(0x6DD9CC, "ScenePartyRequest::openPopUp $_81 __func::target_type", t_6DD9CC);

// ScenePartyRequest::openPopUp $_80: __func::~__func (deleting)
static void t_6DD9E0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DD9E0, "ScenePartyRequest::openPopUp $_80 __func::~__func (deleting)", t_6DD9E0);

// ScenePartyRequest::openPopUp $_80: __func::__clone()
static uint32_t t_6DD9E8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf67f40u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6DD9E8, "ScenePartyRequest::openPopUp $_80 __func::__clone()", t_6DD9E8);

// ScenePartyRequest::openPopUp $_80: __func::__clone(__base*)
static void t_6DDA18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67f40u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DDA18, "ScenePartyRequest::openPopUp $_80 __func::__clone(__base*)", t_6DDA18);

// ScenePartyRequest::openPopUp $_80: __func::destroy
static void t_6DDA40(uint32_t self) {
}
RW_FUNCTION(0x6DDA40, "ScenePartyRequest::openPopUp $_80 __func::destroy", t_6DDA40);

// ScenePartyRequest::openPopUp $_80: __func::destroy_deallocate
static void t_6DDA42(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DDA42, "ScenePartyRequest::openPopUp $_80 __func::destroy_deallocate", t_6DDA42);

// ScenePartyRequest::openPopUp $_80: __func::target
static uint32_t t_6DDB60(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80bf0u ? self + 4 : 0; }
RW_FUNCTION(0x6DDB60, "ScenePartyRequest::openPopUp $_80 __func::target", t_6DDB60);

// ScenePartyRequest::openPopUp $_80: __func::target_type
static uint32_t t_6DDB80() { return 0xfa0ee8u; }
RW_FUNCTION(0x6DDB80, "ScenePartyRequest::openPopUp $_80 __func::target_type", t_6DDB80);

// ScenePartyRequest::openPopUp $_80 UlvE_: __func::~__func (deleting)
static void t_6DDB94(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DDB94, "ScenePartyRequest::openPopUp $_80 UlvE_ __func::~__func (deleting)", t_6DDB94);

// ScenePartyRequest::openPopUp $_80 UlvE_: __func::__clone()
static uint32_t t_6DDB9C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf67f6cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6DDB9C, "ScenePartyRequest::openPopUp $_80 UlvE_ __func::__clone()", t_6DDB9C);

// ScenePartyRequest::openPopUp $_80 UlvE_: __func::__clone(__base*)
static void t_6DDBC8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67f6cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DDBC8, "ScenePartyRequest::openPopUp $_80 UlvE_ __func::__clone(__base*)", t_6DDBC8);

// ScenePartyRequest::openPopUp $_80 UlvE_: __func::destroy
static void t_6DDBE8(uint32_t self) {
}
RW_FUNCTION(0x6DDBE8, "ScenePartyRequest::openPopUp $_80 UlvE_ __func::destroy", t_6DDBE8);

// ScenePartyRequest::openPopUp $_80 UlvE_: __func::destroy_deallocate
static void t_6DDBEA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DDBEA, "ScenePartyRequest::openPopUp $_80 UlvE_ __func::destroy_deallocate", t_6DDBEA);

// ScenePartyRequest::openPopUp $_80 UlvE_: __func::target
static uint32_t t_6DDC04(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80cf0u ? self + 4 : 0; }
RW_FUNCTION(0x6DDC04, "ScenePartyRequest::openPopUp $_80 UlvE_ __func::target", t_6DDC04);

// ScenePartyRequest::openPopUp $_80 UlvE_: __func::target_type
static uint32_t t_6DDC24() { return 0xfa0efcu; }
RW_FUNCTION(0x6DDC24, "ScenePartyRequest::openPopUp $_80 UlvE_ __func::target_type", t_6DDC24);

// ScenePartyRequest::openPopUp $_79: __func::~__func
static uint32_t t_6DDC38(uint32_t self) {
    st32(self, 0xf67f98u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x6DDC38, "ScenePartyRequest::openPopUp $_79 __func::~__func", t_6DDC38);

// ScenePartyRequest::openPopUp $_79: __func::~__func (deleting)
static void t_6DDC74(uint32_t self) {
    st32(self, 0xf67f98u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DDC74, "ScenePartyRequest::openPopUp $_79 __func::~__func (deleting)", t_6DDC74);

// ScenePartyRequest::openPopUp $_79: __func::__clone()
static uint32_t t_6DDCB4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf67f98u);
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
RW_FUNCTION(0x6DDCB4, "ScenePartyRequest::openPopUp $_79 __func::__clone()", t_6DDCB4);

// ScenePartyRequest::openPopUp $_79: __func::__clone(__base*)
static void t_6DDD10(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67f98u);
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
RW_FUNCTION(0x6DDD10, "ScenePartyRequest::openPopUp $_79 __func::__clone(__base*)", t_6DDD10);

// ScenePartyRequest::openPopUp $_79: __func::destroy
static void t_6DDD50(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x6DDD50, "ScenePartyRequest::openPopUp $_79 __func::destroy", t_6DDD50);

// ScenePartyRequest::openPopUp $_79: __func::destroy_deallocate
static void t_6DDD70(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DDD70, "ScenePartyRequest::openPopUp $_79 __func::destroy_deallocate", t_6DDD70);

// ScenePartyRequest::openPopUp $_79: __func::target
static uint32_t t_6DDDE8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80df0u ? self + 8 : 0; }
RW_FUNCTION(0x6DDDE8, "ScenePartyRequest::openPopUp $_79 __func::target", t_6DDDE8);

// ScenePartyRequest::openPopUp $_79: __func::target_type
static uint32_t t_6DDE0C() { return 0xfa0f10u; }
RW_FUNCTION(0x6DDE0C, "ScenePartyRequest::openPopUp $_79 __func::target_type", t_6DDE0C);

// ScenePartyRequest::openPopUpRequest $_78: __func::~__func
static uint32_t t_6DDE20(uint32_t self) {
    st32(self, 0xf67fc4u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x6DDE20, "ScenePartyRequest::openPopUpRequest $_78 __func::~__func", t_6DDE20);

// ScenePartyRequest::openPopUpRequest $_78: __func::~__func (deleting)
static void t_6DDE5C(uint32_t self) {
    st32(self, 0xf67fc4u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DDE5C, "ScenePartyRequest::openPopUpRequest $_78 __func::~__func (deleting)", t_6DDE5C);

// ScenePartyRequest::openPopUpRequest $_78: __func::__clone()
static uint32_t t_6DDE9C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf67fc4u);
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
RW_FUNCTION(0x6DDE9C, "ScenePartyRequest::openPopUpRequest $_78 __func::__clone()", t_6DDE9C);

// ScenePartyRequest::openPopUpRequest $_78: __func::__clone(__base*)
static void t_6DDF00(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67fc4u);
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
RW_FUNCTION(0x6DDF00, "ScenePartyRequest::openPopUpRequest $_78 __func::__clone(__base*)", t_6DDF00);

// ScenePartyRequest::openPopUpRequest $_78: __func::destroy
static void t_6DDF48(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x6DDF48, "ScenePartyRequest::openPopUpRequest $_78 __func::destroy", t_6DDF48);

// ScenePartyRequest::openPopUpRequest $_78: __func::destroy_deallocate
static void t_6DDF68(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DDF68, "ScenePartyRequest::openPopUpRequest $_78 __func::destroy_deallocate", t_6DDF68);

// ScenePartyRequest::openPopUpRequest $_78: __func::target
static uint32_t t_6DDFCC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80ec0u ? self + 8 : 0; }
RW_FUNCTION(0x6DDFCC, "ScenePartyRequest::openPopUpRequest $_78 __func::target", t_6DDFCC);

// ScenePartyRequest::openPopUpRequest $_78: __func::target_type
static uint32_t t_6DDFF0() { return 0xfa0f24u; }
RW_FUNCTION(0x6DDFF0, "ScenePartyRequest::openPopUpRequest $_78 __func::target_type", t_6DDFF0);

// ScenePartyRequest::openPopUpRequest $_87 UlvE_: __func::~__func (deleting)
static void t_6DE100(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE100, "ScenePartyRequest::openPopUpRequest $_87 UlvE_ __func::~__func (deleting)", t_6DE100);

// ScenePartyRequest::openPopUpRequest $_87 UlvE_: __func::__clone()
static uint32_t t_6DE108(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf67ff0u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6DE108, "ScenePartyRequest::openPopUpRequest $_87 UlvE_ __func::__clone()", t_6DE108);

// ScenePartyRequest::openPopUpRequest $_87 UlvE_: __func::__clone(__base*)
static void t_6DE138(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf67ff0u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DE138, "ScenePartyRequest::openPopUpRequest $_87 UlvE_ __func::__clone(__base*)", t_6DE138);

// ScenePartyRequest::openPopUpRequest $_87 UlvE_: __func::destroy
static void t_6DE160(uint32_t self) {
}
RW_FUNCTION(0x6DE160, "ScenePartyRequest::openPopUpRequest $_87 UlvE_ __func::destroy", t_6DE160);

// ScenePartyRequest::openPopUpRequest $_87 UlvE_: __func::destroy_deallocate
static void t_6DE162(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE162, "ScenePartyRequest::openPopUpRequest $_87 UlvE_ __func::destroy_deallocate", t_6DE162);

// ScenePartyRequest::openPopUpRequest $_87 UlvE_: __func::target
static uint32_t t_6DE194(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe80fa0u ? self + 4 : 0; }
RW_FUNCTION(0x6DE194, "ScenePartyRequest::openPopUpRequest $_87 UlvE_ __func::target", t_6DE194);

// ScenePartyRequest::openPopUpRequest $_87 UlvE_: __func::target_type
static uint32_t t_6DE1B4() { return 0xfa0f38u; }
RW_FUNCTION(0x6DE1B4, "ScenePartyRequest::openPopUpRequest $_87 UlvE_ __func::target_type", t_6DE1B4);

// ScenePartyRequest::openPopUpRequest $_77: __func::~__func (deleting)
static void t_6DE1C8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE1C8, "ScenePartyRequest::openPopUpRequest $_77 __func::~__func (deleting)", t_6DE1C8);

// ScenePartyRequest::openPopUpRequest $_77: __func::__clone()
static uint32_t t_6DE1D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6801cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6DE1D0, "ScenePartyRequest::openPopUpRequest $_77 __func::__clone()", t_6DE1D0);

// ScenePartyRequest::openPopUpRequest $_77: __func::__clone(__base*)
static void t_6DE1FC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6801cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DE1FC, "ScenePartyRequest::openPopUpRequest $_77 __func::__clone(__base*)", t_6DE1FC);

// ScenePartyRequest::openPopUpRequest $_77: __func::destroy
static void t_6DE21C(uint32_t self) {
}
RW_FUNCTION(0x6DE21C, "ScenePartyRequest::openPopUpRequest $_77 __func::destroy", t_6DE21C);

// ScenePartyRequest::openPopUpRequest $_77: __func::destroy_deallocate
static void t_6DE21E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE21E, "ScenePartyRequest::openPopUpRequest $_77 __func::destroy_deallocate", t_6DE21E);

// ScenePartyRequest::openPopUpRequest $_77: __func::target
static uint32_t t_6DE230(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81080u ? self + 4 : 0; }
RW_FUNCTION(0x6DE230, "ScenePartyRequest::openPopUpRequest $_77 __func::target", t_6DE230);

// ScenePartyRequest::openPopUpRequest $_77: __func::target_type
static uint32_t t_6DE250() { return 0xfa0f4cu; }
RW_FUNCTION(0x6DE250, "ScenePartyRequest::openPopUpRequest $_77 __func::target_type", t_6DE250);

// ScenePartyRequest::openPopUpRequest $_76: __func::~__func (deleting)
static void t_6DE264(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE264, "ScenePartyRequest::openPopUpRequest $_76 __func::~__func (deleting)", t_6DE264);

// ScenePartyRequest::openPopUpRequest $_76: __func::__clone()
static uint32_t t_6DE26C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68048u);
    }
    return v1;
}
RW_FUNCTION(0x6DE26C, "ScenePartyRequest::openPopUpRequest $_76 __func::__clone()", t_6DE26C);

// ScenePartyRequest::openPopUpRequest $_76: __func::__clone(__base*)
static void t_6DE290(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68048u);
    }
}
RW_FUNCTION(0x6DE290, "ScenePartyRequest::openPopUpRequest $_76 __func::__clone(__base*)", t_6DE290);

// ScenePartyRequest::openPopUpRequest $_76: __func::destroy
static void t_6DE2AC(uint32_t self) {
}
RW_FUNCTION(0x6DE2AC, "ScenePartyRequest::openPopUpRequest $_76 __func::destroy", t_6DE2AC);

// ScenePartyRequest::openPopUpRequest $_76: __func::destroy_deallocate
static void t_6DE2AE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE2AE, "ScenePartyRequest::openPopUpRequest $_76 __func::destroy_deallocate", t_6DE2AE);

// ScenePartyRequest::openPopUpRequest $_76: __func::target
static uint32_t t_6DE2CC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81180u ? self + 4 : 0; }
RW_FUNCTION(0x6DE2CC, "ScenePartyRequest::openPopUpRequest $_76 __func::target", t_6DE2CC);

// ScenePartyRequest::openPopUpRequest $_76: __func::target_type
static uint32_t t_6DE2EC() { return 0xfa0f60u; }
RW_FUNCTION(0x6DE2EC, "ScenePartyRequest::openPopUpRequest $_76 __func::target_type", t_6DE2EC);

// ScenePartyRequest::openPopUpRequest $_75: __func::~__func
static uint32_t t_6DE300(uint32_t self) {
    st32(self, 0xf68074u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6DE300, "ScenePartyRequest::openPopUpRequest $_75 __func::~__func", t_6DE300);

// ScenePartyRequest::openPopUpRequest $_75: __func::~__func (deleting)
static void t_6DE32C(uint32_t self) {
    st32(self, 0xf68074u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE32C, "ScenePartyRequest::openPopUpRequest $_75 __func::~__func (deleting)", t_6DE32C);

// ScenePartyRequest::openPopUpRequest $_75: __func::__clone()
static uint32_t t_6DE35C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf68074u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
        }
        uint32_t v5 = ld32(self + 16 + 0);
        uint32_t v6 = ld32(self + 16 + 4);
        uint32_t v7 = ld32(self + 16 + 8);
        st32(v1 + 16 + 0, v5);
        st32(v1 + 16 + 4, v6);
        st32(v1 + 16 + 8, v7);
    }
    return v1;
}
RW_FUNCTION(0x6DE35C, "ScenePartyRequest::openPopUpRequest $_75 __func::__clone()", t_6DE35C);

// ScenePartyRequest::openPopUpRequest $_75: __func::__clone(__base*)
static void t_6DE3AC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68074u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 16 + 0);
        uint32_t v5 = ld32(self + 16 + 4);
        uint32_t v6 = ld32(self + 16 + 8);
        st32(p + 16 + 0, v4);
        st32(p + 16 + 4, v5);
        st32(p + 16 + 8, v6);
    }
}
RW_FUNCTION(0x6DE3AC, "ScenePartyRequest::openPopUpRequest $_75 __func::__clone(__base*)", t_6DE3AC);

// ScenePartyRequest::openPopUpRequest $_75: __func::destroy
static void t_6DE3F0(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x6DE3F0, "ScenePartyRequest::openPopUpRequest $_75 __func::destroy", t_6DE3F0);

// ScenePartyRequest::openPopUpRequest $_75: __func::destroy_deallocate
static void t_6DE3FE(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE3FE, "ScenePartyRequest::openPopUpRequest $_75 __func::destroy_deallocate", t_6DE3FE);

// ScenePartyRequest::openPopUpRequest $_75: __func::target
static uint32_t t_6DE720(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81280u ? self + 4 : 0; }
RW_FUNCTION(0x6DE720, "ScenePartyRequest::openPopUpRequest $_75 __func::target", t_6DE720);

// ScenePartyRequest::openPopUpRequest $_75: __func::target_type
static uint32_t t_6DE740() { return 0xfa0f74u; }
RW_FUNCTION(0x6DE740, "ScenePartyRequest::openPopUpRequest $_75 __func::target_type", t_6DE740);

// ScenePartyRequest::openPopUpRequest $_75 UlvE0_: __func::~__func (deleting)
static void t_6DE754(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE754, "ScenePartyRequest::openPopUpRequest $_75 UlvE0_ __func::~__func (deleting)", t_6DE754);

// ScenePartyRequest::openPopUpRequest $_75 UlvE0_: __func::__clone()
static uint32_t t_6DE75C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf680a0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6DE75C, "ScenePartyRequest::openPopUpRequest $_75 UlvE0_ __func::__clone()", t_6DE75C);

// ScenePartyRequest::openPopUpRequest $_75 UlvE0_: __func::__clone(__base*)
static void t_6DE788(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf680a0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DE788, "ScenePartyRequest::openPopUpRequest $_75 UlvE0_ __func::__clone(__base*)", t_6DE788);

// ScenePartyRequest::openPopUpRequest $_75 UlvE0_: __func::destroy
static void t_6DE7A8(uint32_t self) {
}
RW_FUNCTION(0x6DE7A8, "ScenePartyRequest::openPopUpRequest $_75 UlvE0_ __func::destroy", t_6DE7A8);

// ScenePartyRequest::openPopUpRequest $_75 UlvE0_: __func::destroy_deallocate
static void t_6DE7AA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE7AA, "ScenePartyRequest::openPopUpRequest $_75 UlvE0_ __func::destroy_deallocate", t_6DE7AA);

// ScenePartyRequest::openPopUpRequest $_75 UlvE0_: __func::target
static uint32_t t_6DE7C4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81390u ? self + 4 : 0; }
RW_FUNCTION(0x6DE7C4, "ScenePartyRequest::openPopUpRequest $_75 UlvE0_ __func::target", t_6DE7C4);

// ScenePartyRequest::openPopUpRequest $_75 UlvE0_: __func::target_type
static uint32_t t_6DE7E4() { return 0xfa0f88u; }
RW_FUNCTION(0x6DE7E4, "ScenePartyRequest::openPopUpRequest $_75 UlvE0_ __func::target_type", t_6DE7E4);

// ScenePartyRequest::openPopUpRequest $_75 UlbE_: __func::~__func (deleting)
static void t_6DE7F8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE7F8, "ScenePartyRequest::openPopUpRequest $_75 UlbE_ __func::~__func (deleting)", t_6DE7F8);

// ScenePartyRequest::openPopUpRequest $_75 UlbE_: __func::__clone()
static uint32_t t_6DE800(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf680ccu);
    }
    return v1;
}
RW_FUNCTION(0x6DE800, "ScenePartyRequest::openPopUpRequest $_75 UlbE_ __func::__clone()", t_6DE800);

// ScenePartyRequest::openPopUpRequest $_75 UlbE_: __func::__clone(__base*)
static void t_6DE824(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf680ccu);
    }
}
RW_FUNCTION(0x6DE824, "ScenePartyRequest::openPopUpRequest $_75 UlbE_ __func::__clone(__base*)", t_6DE824);

// ScenePartyRequest::openPopUpRequest $_75 UlbE_: __func::destroy
static void t_6DE840(uint32_t self) {
}
RW_FUNCTION(0x6DE840, "ScenePartyRequest::openPopUpRequest $_75 UlbE_ __func::destroy", t_6DE840);

// ScenePartyRequest::openPopUpRequest $_75 UlbE_: __func::destroy_deallocate
static void t_6DE842(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE842, "ScenePartyRequest::openPopUpRequest $_75 UlbE_ __func::destroy_deallocate", t_6DE842);

// ScenePartyRequest::openPopUpRequest $_75 UlbE_: __func::target
static uint32_t t_6DE860(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe814e0u ? self + 4 : 0; }
RW_FUNCTION(0x6DE860, "ScenePartyRequest::openPopUpRequest $_75 UlbE_ __func::target", t_6DE860);

// ScenePartyRequest::openPopUpRequest $_75 UlbE_: __func::target_type
static uint32_t t_6DE880() { return 0xfa0f9cu; }
RW_FUNCTION(0x6DE880, "ScenePartyRequest::openPopUpRequest $_75 UlbE_ __func::target_type", t_6DE880);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_: __func::~__func (deleting)
static void t_6DE894(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE894, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ __func::~__func (deleting)", t_6DE894);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_: __func::__clone()
static uint32_t t_6DE89C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf680f8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6DE89C, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ __func::__clone()", t_6DE89C);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_: __func::__clone(__base*)
static void t_6DE8C8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf680f8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DE8C8, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ __func::__clone(__base*)", t_6DE8C8);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_: __func::destroy
static void t_6DE8E8(uint32_t self) {
}
RW_FUNCTION(0x6DE8E8, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ __func::destroy", t_6DE8E8);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_: __func::destroy_deallocate
static void t_6DE8EA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DE8EA, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ __func::destroy_deallocate", t_6DE8EA);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_: __func::target
static uint32_t t_6DE9F8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81630u ? self + 4 : 0; }
RW_FUNCTION(0x6DE9F8, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ __func::target", t_6DE9F8);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_: __func::target_type
static uint32_t t_6DEA18() { return 0xfa0fb0u; }
RW_FUNCTION(0x6DEA18, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ __func::target_type", t_6DEA18);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6DEA2C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DEA2C, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_ __func::~__func (deleting)", t_6DEA2C);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6DEA34(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68124u);
    }
    return v1;
}
RW_FUNCTION(0x6DEA34, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_ __func::__clone()", t_6DEA34);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_: __func::__clone(__base*)
static void t_6DEA58(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68124u);
    }
}
RW_FUNCTION(0x6DEA58, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_ __func::__clone(__base*)", t_6DEA58);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_: __func::destroy
static void t_6DEA74(uint32_t self) {
}
RW_FUNCTION(0x6DEA74, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_ __func::destroy", t_6DEA74);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6DEA76(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DEA76, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_ __func::destroy_deallocate", t_6DEA76);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_: __func::target
static uint32_t t_6DEA94(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81780u ? self + 4 : 0; }
RW_FUNCTION(0x6DEA94, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_ __func::target", t_6DEA94);

// ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_: __func::target_type
static uint32_t t_6DEAB4() { return 0xfa0fc4u; }
RW_FUNCTION(0x6DEAB4, "ScenePartyRequest::openPopUpRequest $_75 UlvE_ UlbE_ __func::target_type", t_6DEAB4);

// ScenePartyRequest::openPopUpRequest $_74: __func::~__func
static uint32_t t_6DEAC8(uint32_t self) {
    st32(self, 0xf68150u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x6DEAC8, "ScenePartyRequest::openPopUpRequest $_74 __func::~__func", t_6DEAC8);

// ScenePartyRequest::openPopUpRequest $_74: __func::~__func (deleting)
static void t_6DEB04(uint32_t self) {
    st32(self, 0xf68150u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DEB04, "ScenePartyRequest::openPopUpRequest $_74 __func::~__func (deleting)", t_6DEB04);

// ScenePartyRequest::openPopUpRequest $_74: __func::__clone()
static uint32_t t_6DEB44(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf68150u);
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
RW_FUNCTION(0x6DEB44, "ScenePartyRequest::openPopUpRequest $_74 __func::__clone()", t_6DEB44);

// ScenePartyRequest::openPopUpRequest $_74: __func::__clone(__base*)
static void t_6DEBA8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68150u);
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
RW_FUNCTION(0x6DEBA8, "ScenePartyRequest::openPopUpRequest $_74 __func::__clone(__base*)", t_6DEBA8);

// ScenePartyRequest::openPopUpRequest $_74: __func::destroy
static void t_6DEBF0(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x6DEBF0, "ScenePartyRequest::openPopUpRequest $_74 __func::destroy", t_6DEBF0);

// ScenePartyRequest::openPopUpRequest $_74: __func::destroy_deallocate
static void t_6DEC10(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DEC10, "ScenePartyRequest::openPopUpRequest $_74 __func::destroy_deallocate", t_6DEC10);

// ScenePartyRequest::openPopUpRequest $_74: __func::target
static uint32_t t_6DEC7C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81890u ? self + 8 : 0; }
RW_FUNCTION(0x6DEC7C, "ScenePartyRequest::openPopUpRequest $_74 __func::target", t_6DEC7C);

// ScenePartyRequest::openPopUpRequest $_74: __func::target_type
static uint32_t t_6DECA0() { return 0xfa0fd8u; }
RW_FUNCTION(0x6DECA0, "ScenePartyRequest::openPopUpRequest $_74 __func::target_type", t_6DECA0);

// ScenePartyRequest::openPopUpSearchID $_73: __func::~__func (deleting)
static void t_6DECB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DECB4, "ScenePartyRequest::openPopUpSearchID $_73 __func::~__func (deleting)", t_6DECB4);

// ScenePartyRequest::openPopUpSearchID $_73: __func::__clone()
static uint32_t t_6DECBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf6817cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6DECBC, "ScenePartyRequest::openPopUpSearchID $_73 __func::__clone()", t_6DECBC);

// ScenePartyRequest::openPopUpSearchID $_73: __func::__clone(__base*)
static void t_6DECF4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6817cu);
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
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6DECF4, "ScenePartyRequest::openPopUpSearchID $_73 __func::__clone(__base*)", t_6DECF4);

// ScenePartyRequest::openPopUpSearchID $_73: __func::destroy
static void t_6DED20(uint32_t self) {
}
RW_FUNCTION(0x6DED20, "ScenePartyRequest::openPopUpSearchID $_73 __func::destroy", t_6DED20);

// ScenePartyRequest::openPopUpSearchID $_73: __func::destroy_deallocate
static void t_6DED22(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DED22, "ScenePartyRequest::openPopUpSearchID $_73 __func::destroy_deallocate", t_6DED22);

// ScenePartyRequest::openPopUpSearchID $_73: __func::target
static uint32_t t_6DED70(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81950u ? self + 4 : 0; }
RW_FUNCTION(0x6DED70, "ScenePartyRequest::openPopUpSearchID $_73 __func::target", t_6DED70);

// ScenePartyRequest::openPopUpSearchID $_73: __func::target_type
static uint32_t t_6DED90() { return 0xfa0fecu; }
RW_FUNCTION(0x6DED90, "ScenePartyRequest::openPopUpSearchID $_73 __func::target_type", t_6DED90);

// ScenePartyRequest::openPopUpSearchID $_86 UlvE_: __func::~__func (deleting)
static void t_6DEE78(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DEE78, "ScenePartyRequest::openPopUpSearchID $_86 UlvE_ __func::~__func (deleting)", t_6DEE78);

// ScenePartyRequest::openPopUpSearchID $_86 UlvE_: __func::__clone()
static uint32_t t_6DEE80(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf681a8u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6DEE80, "ScenePartyRequest::openPopUpSearchID $_86 UlvE_ __func::__clone()", t_6DEE80);

// ScenePartyRequest::openPopUpSearchID $_86 UlvE_: __func::__clone(__base*)
static void t_6DEEB0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf681a8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DEEB0, "ScenePartyRequest::openPopUpSearchID $_86 UlvE_ __func::__clone(__base*)", t_6DEEB0);

// ScenePartyRequest::openPopUpSearchID $_86 UlvE_: __func::destroy
static void t_6DEED8(uint32_t self) {
}
RW_FUNCTION(0x6DEED8, "ScenePartyRequest::openPopUpSearchID $_86 UlvE_ __func::destroy", t_6DEED8);

// ScenePartyRequest::openPopUpSearchID $_86 UlvE_: __func::destroy_deallocate
static void t_6DEEDA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DEEDA, "ScenePartyRequest::openPopUpSearchID $_86 UlvE_ __func::destroy_deallocate", t_6DEEDA);

// ScenePartyRequest::openPopUpSearchID $_86 UlvE_: __func::target
static uint32_t t_6DEF0C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81a10u ? self + 4 : 0; }
RW_FUNCTION(0x6DEF0C, "ScenePartyRequest::openPopUpSearchID $_86 UlvE_ __func::target", t_6DEF0C);

// ScenePartyRequest::openPopUpSearchID $_86 UlvE_: __func::target_type
static uint32_t t_6DEF2C() { return 0xfa1000u; }
RW_FUNCTION(0x6DEF2C, "ScenePartyRequest::openPopUpSearchID $_86 UlvE_ __func::target_type", t_6DEF2C);

// ScenePartyRequest::openPopUpSearchID $_72: __func::~__func (deleting)
static void t_6DEF40(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DEF40, "ScenePartyRequest::openPopUpSearchID $_72 __func::~__func (deleting)", t_6DEF40);

// ScenePartyRequest::openPopUpSearchID $_72: __func::__clone()
static uint32_t t_6DEF48(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf681d4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6DEF48, "ScenePartyRequest::openPopUpSearchID $_72 __func::__clone()", t_6DEF48);

// ScenePartyRequest::openPopUpSearchID $_72: __func::__clone(__base*)
static void t_6DEF74(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf681d4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DEF74, "ScenePartyRequest::openPopUpSearchID $_72 __func::__clone(__base*)", t_6DEF74);

// ScenePartyRequest::openPopUpSearchID $_72: __func::destroy
static void t_6DEF94(uint32_t self) {
}
RW_FUNCTION(0x6DEF94, "ScenePartyRequest::openPopUpSearchID $_72 __func::destroy", t_6DEF94);

// ScenePartyRequest::openPopUpSearchID $_72: __func::destroy_deallocate
static void t_6DEF96(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DEF96, "ScenePartyRequest::openPopUpSearchID $_72 __func::destroy_deallocate", t_6DEF96);

// ScenePartyRequest::openPopUpSearchID $_72: __func::target
static uint32_t t_6DEFA8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81ac0u ? self + 4 : 0; }
RW_FUNCTION(0x6DEFA8, "ScenePartyRequest::openPopUpSearchID $_72 __func::target", t_6DEFA8);

// ScenePartyRequest::openPopUpSearchID $_72: __func::target_type
static uint32_t t_6DEFC8() { return 0xfa1014u; }
RW_FUNCTION(0x6DEFC8, "ScenePartyRequest::openPopUpSearchID $_72 __func::target_type", t_6DEFC8);

// ScenePartyRequest::openPopUpSearchID $_71: __func::~__func (deleting)
static void t_6DEFDC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DEFDC, "ScenePartyRequest::openPopUpSearchID $_71 __func::~__func (deleting)", t_6DEFDC);

// ScenePartyRequest::openPopUpSearchID $_71: __func::__clone()
static uint32_t t_6DEFE4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68200u);
    }
    return v1;
}
RW_FUNCTION(0x6DEFE4, "ScenePartyRequest::openPopUpSearchID $_71 __func::__clone()", t_6DEFE4);

// ScenePartyRequest::openPopUpSearchID $_71: __func::__clone(__base*)
static void t_6DF008(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68200u);
    }
}
RW_FUNCTION(0x6DF008, "ScenePartyRequest::openPopUpSearchID $_71 __func::__clone(__base*)", t_6DF008);

// ScenePartyRequest::openPopUpSearchID $_71: __func::destroy
static void t_6DF024(uint32_t self) {
}
RW_FUNCTION(0x6DF024, "ScenePartyRequest::openPopUpSearchID $_71 __func::destroy", t_6DF024);

// ScenePartyRequest::openPopUpSearchID $_71: __func::destroy_deallocate
static void t_6DF026(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF026, "ScenePartyRequest::openPopUpSearchID $_71 __func::destroy_deallocate", t_6DF026);

// ScenePartyRequest::openPopUpSearchID $_71: __func::target
static uint32_t t_6DF044(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81ba0u ? self + 4 : 0; }
RW_FUNCTION(0x6DF044, "ScenePartyRequest::openPopUpSearchID $_71 __func::target", t_6DF044);

// ScenePartyRequest::openPopUpSearchID $_71: __func::target_type
static uint32_t t_6DF064() { return 0xfa1028u; }
RW_FUNCTION(0x6DF064, "ScenePartyRequest::openPopUpSearchID $_71 __func::target_type", t_6DF064);

// ScenePartyRequest::openPopUpSearchID $_70: __func::~__func (deleting)
static void t_6DF078(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF078, "ScenePartyRequest::openPopUpSearchID $_70 __func::~__func (deleting)", t_6DF078);

// ScenePartyRequest::openPopUpSearchID $_70: __func::__clone()
static uint32_t t_6DF080(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6822cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6DF080, "ScenePartyRequest::openPopUpSearchID $_70 __func::__clone()", t_6DF080);

// ScenePartyRequest::openPopUpSearchID $_70: __func::__clone(__base*)
static void t_6DF0B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6822cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DF0B0, "ScenePartyRequest::openPopUpSearchID $_70 __func::__clone(__base*)", t_6DF0B0);

// ScenePartyRequest::openPopUpSearchID $_70: __func::destroy
static void t_6DF0D8(uint32_t self) {
}
RW_FUNCTION(0x6DF0D8, "ScenePartyRequest::openPopUpSearchID $_70 __func::destroy", t_6DF0D8);

// ScenePartyRequest::openPopUpSearchID $_70: __func::destroy_deallocate
static void t_6DF0DA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF0DA, "ScenePartyRequest::openPopUpSearchID $_70 __func::destroy_deallocate", t_6DF0DA);

// ScenePartyRequest::openPopUpSearchID $_70: __func::target
static uint32_t t_6DF200(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81c80u ? self + 4 : 0; }
RW_FUNCTION(0x6DF200, "ScenePartyRequest::openPopUpSearchID $_70 __func::target", t_6DF200);

// ScenePartyRequest::openPopUpSearchID $_70: __func::target_type
static uint32_t t_6DF220() { return 0xfa103cu; }
RW_FUNCTION(0x6DF220, "ScenePartyRequest::openPopUpSearchID $_70 __func::target_type", t_6DF220);

// ScenePartyRequest::openPopUpSearchID $_70 UlvE_: __func::~__func (deleting)
static void t_6DF234(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF234, "ScenePartyRequest::openPopUpSearchID $_70 UlvE_ __func::~__func (deleting)", t_6DF234);

// ScenePartyRequest::openPopUpSearchID $_70 UlvE_: __func::__clone()
static uint32_t t_6DF23C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68258u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6DF23C, "ScenePartyRequest::openPopUpSearchID $_70 UlvE_ __func::__clone()", t_6DF23C);

// ScenePartyRequest::openPopUpSearchID $_70 UlvE_: __func::__clone(__base*)
static void t_6DF268(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68258u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DF268, "ScenePartyRequest::openPopUpSearchID $_70 UlvE_ __func::__clone(__base*)", t_6DF268);

// ScenePartyRequest::openPopUpSearchID $_70 UlvE_: __func::destroy
static void t_6DF288(uint32_t self) {
}
RW_FUNCTION(0x6DF288, "ScenePartyRequest::openPopUpSearchID $_70 UlvE_ __func::destroy", t_6DF288);

// ScenePartyRequest::openPopUpSearchID $_70 UlvE_: __func::destroy_deallocate
static void t_6DF28A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF28A, "ScenePartyRequest::openPopUpSearchID $_70 UlvE_ __func::destroy_deallocate", t_6DF28A);

// ScenePartyRequest::openPopUpSearchID $_70 UlvE_: __func::target
static uint32_t t_6DF2A4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81d60u ? self + 4 : 0; }
RW_FUNCTION(0x6DF2A4, "ScenePartyRequest::openPopUpSearchID $_70 UlvE_ __func::target", t_6DF2A4);

// ScenePartyRequest::openPopUpSearchID $_70 UlvE_: __func::target_type
static uint32_t t_6DF2C4() { return 0xfa1050u; }
RW_FUNCTION(0x6DF2C4, "ScenePartyRequest::openPopUpSearchID $_70 UlvE_ __func::target_type", t_6DF2C4);

// ScenePartyRequest::openPopUpSearchID $_69: __func::~__func (deleting)
static void t_6DF2D8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF2D8, "ScenePartyRequest::openPopUpSearchID $_69 __func::~__func (deleting)", t_6DF2D8);

// ScenePartyRequest::openPopUpSearchID $_69: __func::__clone()
static uint32_t t_6DF2E0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf68284u);
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
        uint32_t v9 = ld32(self + 20 + 12);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
        st32(v1 + 20 + 12, v9);
    }
    return v1;
}
RW_FUNCTION(0x6DF2E0, "ScenePartyRequest::openPopUpSearchID $_69 __func::__clone()", t_6DF2E0);

// ScenePartyRequest::openPopUpSearchID $_69: __func::__clone(__base*)
static void t_6DF318(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68284u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20 + 0);
        uint32_t v6 = ld32(self + 20 + 4);
        uint32_t v7 = ld32(self + 20 + 8);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
    }
}
RW_FUNCTION(0x6DF318, "ScenePartyRequest::openPopUpSearchID $_69 __func::__clone(__base*)", t_6DF318);

// ScenePartyRequest::openPopUpSearchID $_69: __func::destroy
static void t_6DF34C(uint32_t self) {
}
RW_FUNCTION(0x6DF34C, "ScenePartyRequest::openPopUpSearchID $_69 __func::destroy", t_6DF34C);

// ScenePartyRequest::openPopUpSearchID $_69: __func::destroy_deallocate
static void t_6DF34E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF34E, "ScenePartyRequest::openPopUpSearchID $_69 __func::destroy_deallocate", t_6DF34E);

// ScenePartyRequest::openPopUpSearchID $_69: __func::target
static uint32_t t_6DF848(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81e40u ? self + 4 : 0; }
RW_FUNCTION(0x6DF848, "ScenePartyRequest::openPopUpSearchID $_69 __func::target", t_6DF848);

// ScenePartyRequest::openPopUpSearchID $_69: __func::target_type
static uint32_t t_6DF868() { return 0xfa1064u; }
RW_FUNCTION(0x6DF868, "ScenePartyRequest::openPopUpSearchID $_69 __func::target_type", t_6DF868);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE1_: __func::~__func (deleting)
static void t_6DF87C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF87C, "ScenePartyRequest::openPopUpSearchID $_69 UlbE1_ __func::~__func (deleting)", t_6DF87C);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE1_: __func::__clone()
static uint32_t t_6DF884(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf682b0u);
    }
    return v1;
}
RW_FUNCTION(0x6DF884, "ScenePartyRequest::openPopUpSearchID $_69 UlbE1_ __func::__clone()", t_6DF884);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE1_: __func::__clone(__base*)
static void t_6DF8A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf682b0u);
    }
}
RW_FUNCTION(0x6DF8A8, "ScenePartyRequest::openPopUpSearchID $_69 UlbE1_ __func::__clone(__base*)", t_6DF8A8);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE1_: __func::destroy
static void t_6DF8C4(uint32_t self) {
}
RW_FUNCTION(0x6DF8C4, "ScenePartyRequest::openPopUpSearchID $_69 UlbE1_ __func::destroy", t_6DF8C4);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE1_: __func::destroy_deallocate
static void t_6DF8C6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF8C6, "ScenePartyRequest::openPopUpSearchID $_69 UlbE1_ __func::destroy_deallocate", t_6DF8C6);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE1_: __func::target
static uint32_t t_6DF8E4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81f00u ? self + 4 : 0; }
RW_FUNCTION(0x6DF8E4, "ScenePartyRequest::openPopUpSearchID $_69 UlbE1_ __func::target", t_6DF8E4);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE1_: __func::target_type
static uint32_t t_6DF904() { return 0xfa1078u; }
RW_FUNCTION(0x6DF904, "ScenePartyRequest::openPopUpSearchID $_69 UlbE1_ __func::target_type", t_6DF904);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE0_: __func::~__func (deleting)
static void t_6DF918(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF918, "ScenePartyRequest::openPopUpSearchID $_69 UlbE0_ __func::~__func (deleting)", t_6DF918);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE0_: __func::__clone()
static uint32_t t_6DF920(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf682dcu);
    }
    return v1;
}
RW_FUNCTION(0x6DF920, "ScenePartyRequest::openPopUpSearchID $_69 UlbE0_ __func::__clone()", t_6DF920);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE0_: __func::__clone(__base*)
static void t_6DF944(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf682dcu);
    }
}
RW_FUNCTION(0x6DF944, "ScenePartyRequest::openPopUpSearchID $_69 UlbE0_ __func::__clone(__base*)", t_6DF944);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE0_: __func::destroy
static void t_6DF960(uint32_t self) {
}
RW_FUNCTION(0x6DF960, "ScenePartyRequest::openPopUpSearchID $_69 UlbE0_ __func::destroy", t_6DF960);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE0_: __func::destroy_deallocate
static void t_6DF962(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF962, "ScenePartyRequest::openPopUpSearchID $_69 UlbE0_ __func::destroy_deallocate", t_6DF962);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE0_: __func::target
static uint32_t t_6DF980(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe81fc0u ? self + 4 : 0; }
RW_FUNCTION(0x6DF980, "ScenePartyRequest::openPopUpSearchID $_69 UlbE0_ __func::target", t_6DF980);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE0_: __func::target_type
static uint32_t t_6DF9A0() { return 0xfa108cu; }
RW_FUNCTION(0x6DF9A0, "ScenePartyRequest::openPopUpSearchID $_69 UlbE0_ __func::target_type", t_6DF9A0);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE_: __func::~__func (deleting)
static void t_6DF9B4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF9B4, "ScenePartyRequest::openPopUpSearchID $_69 UlbE_ __func::~__func (deleting)", t_6DF9B4);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE_: __func::__clone()
static uint32_t t_6DF9BC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68308u);
    }
    return v1;
}
RW_FUNCTION(0x6DF9BC, "ScenePartyRequest::openPopUpSearchID $_69 UlbE_ __func::__clone()", t_6DF9BC);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE_: __func::__clone(__base*)
static void t_6DF9E0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68308u);
    }
}
RW_FUNCTION(0x6DF9E0, "ScenePartyRequest::openPopUpSearchID $_69 UlbE_ __func::__clone(__base*)", t_6DF9E0);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE_: __func::destroy
static void t_6DF9FC(uint32_t self) {
}
RW_FUNCTION(0x6DF9FC, "ScenePartyRequest::openPopUpSearchID $_69 UlbE_ __func::destroy", t_6DF9FC);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE_: __func::destroy_deallocate
static void t_6DF9FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DF9FE, "ScenePartyRequest::openPopUpSearchID $_69 UlbE_ __func::destroy_deallocate", t_6DF9FE);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE_: __func::target
static uint32_t t_6DFA1C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82080u ? self + 4 : 0; }
RW_FUNCTION(0x6DFA1C, "ScenePartyRequest::openPopUpSearchID $_69 UlbE_ __func::target", t_6DFA1C);

// ScenePartyRequest::openPopUpSearchID $_69 UlbE_: __func::target_type
static uint32_t t_6DFA3C() { return 0xfa10a0u; }
RW_FUNCTION(0x6DFA3C, "ScenePartyRequest::openPopUpSearchID $_69 UlbE_ __func::target_type", t_6DFA3C);

// ScenePartyRequest::openPopUpLux $_68: __func::~__func (deleting)
static void t_6DFA50(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFA50, "ScenePartyRequest::openPopUpLux $_68 __func::~__func (deleting)", t_6DFA50);

// ScenePartyRequest::openPopUpLux $_68: __func::__clone()
static uint32_t t_6DFA58(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf68334u);
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
RW_FUNCTION(0x6DFA58, "ScenePartyRequest::openPopUpLux $_68 __func::__clone()", t_6DFA58);

// ScenePartyRequest::openPopUpLux $_68: __func::__clone(__base*)
static void t_6DFA94(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68334u);
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
RW_FUNCTION(0x6DFA94, "ScenePartyRequest::openPopUpLux $_68 __func::__clone(__base*)", t_6DFA94);

// ScenePartyRequest::openPopUpLux $_68: __func::destroy
static void t_6DFAC4(uint32_t self) {
}
RW_FUNCTION(0x6DFAC4, "ScenePartyRequest::openPopUpLux $_68 __func::destroy", t_6DFAC4);

// ScenePartyRequest::openPopUpLux $_68: __func::destroy_deallocate
static void t_6DFAC6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFAC6, "ScenePartyRequest::openPopUpLux $_68 __func::destroy_deallocate", t_6DFAC6);

// ScenePartyRequest::openPopUpLux $_68: __func::target
static uint32_t t_6DFAE8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82150u ? self + 4 : 0; }
RW_FUNCTION(0x6DFAE8, "ScenePartyRequest::openPopUpLux $_68 __func::target", t_6DFAE8);

// ScenePartyRequest::openPopUpLux $_68: __func::target_type
static uint32_t t_6DFB08() { return 0xfa10b4u; }
RW_FUNCTION(0x6DFB08, "ScenePartyRequest::openPopUpLux $_68 __func::target_type", t_6DFB08);

// ScenePartyRequest::openPopUpLux $_67: __func::~__func (deleting)
static void t_6DFB1C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFB1C, "ScenePartyRequest::openPopUpLux $_67 __func::~__func (deleting)", t_6DFB1C);

// ScenePartyRequest::openPopUpLux $_67: __func::__clone()
static uint32_t t_6DFB24(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf68360u);
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
RW_FUNCTION(0x6DFB24, "ScenePartyRequest::openPopUpLux $_67 __func::__clone()", t_6DFB24);

// ScenePartyRequest::openPopUpLux $_67: __func::__clone(__base*)
static void t_6DFB60(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68360u);
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
RW_FUNCTION(0x6DFB60, "ScenePartyRequest::openPopUpLux $_67 __func::__clone(__base*)", t_6DFB60);

// ScenePartyRequest::openPopUpLux $_67: __func::destroy
static void t_6DFB90(uint32_t self) {
}
RW_FUNCTION(0x6DFB90, "ScenePartyRequest::openPopUpLux $_67 __func::destroy", t_6DFB90);

// ScenePartyRequest::openPopUpLux $_67: __func::destroy_deallocate
static void t_6DFB92(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFB92, "ScenePartyRequest::openPopUpLux $_67 __func::destroy_deallocate", t_6DFB92);

// ScenePartyRequest::openPopUpLux $_67: __func::target
static uint32_t t_6DFBB4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82210u ? self + 4 : 0; }
RW_FUNCTION(0x6DFBB4, "ScenePartyRequest::openPopUpLux $_67 __func::target", t_6DFBB4);

// ScenePartyRequest::openPopUpLux $_67: __func::target_type
static uint32_t t_6DFBD4() { return 0xfa10c8u; }
RW_FUNCTION(0x6DFBD4, "ScenePartyRequest::openPopUpLux $_67 __func::target_type", t_6DFBD4);

// ScenePartyRequest::openPopUpLux $_66: __func::~__func (deleting)
static void t_6DFBE8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFBE8, "ScenePartyRequest::openPopUpLux $_66 __func::~__func (deleting)", t_6DFBE8);

// ScenePartyRequest::openPopUpLux $_66: __func::__clone()
static uint32_t t_6DFBF0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6838cu);
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
RW_FUNCTION(0x6DFBF0, "ScenePartyRequest::openPopUpLux $_66 __func::__clone()", t_6DFBF0);

// ScenePartyRequest::openPopUpLux $_66: __func::__clone(__base*)
static void t_6DFC2C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6838cu);
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
RW_FUNCTION(0x6DFC2C, "ScenePartyRequest::openPopUpLux $_66 __func::__clone(__base*)", t_6DFC2C);

// ScenePartyRequest::openPopUpLux $_66: __func::destroy
static void t_6DFC5C(uint32_t self) {
}
RW_FUNCTION(0x6DFC5C, "ScenePartyRequest::openPopUpLux $_66 __func::destroy", t_6DFC5C);

// ScenePartyRequest::openPopUpLux $_66: __func::destroy_deallocate
static void t_6DFC5E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFC5E, "ScenePartyRequest::openPopUpLux $_66 __func::destroy_deallocate", t_6DFC5E);

// ScenePartyRequest::openPopUpLux $_66: __func::target
static uint32_t t_6DFC80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe822d0u ? self + 4 : 0; }
RW_FUNCTION(0x6DFC80, "ScenePartyRequest::openPopUpLux $_66 __func::target", t_6DFC80);

// ScenePartyRequest::openPopUpLux $_66: __func::target_type
static uint32_t t_6DFCA0() { return 0xfa10dcu; }
RW_FUNCTION(0x6DFCA0, "ScenePartyRequest::openPopUpLux $_66 __func::target_type", t_6DFCA0);

// ScenePartyRequest::openPopUpLux $_65: __func::~__func (deleting)
static void t_6DFCB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFCB4, "ScenePartyRequest::openPopUpLux $_65 __func::~__func (deleting)", t_6DFCB4);

// ScenePartyRequest::openPopUpLux $_65: __func::__clone()
static uint32_t t_6DFCBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf683b8u);
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
RW_FUNCTION(0x6DFCBC, "ScenePartyRequest::openPopUpLux $_65 __func::__clone()", t_6DFCBC);

// ScenePartyRequest::openPopUpLux $_65: __func::__clone(__base*)
static void t_6DFCF8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf683b8u);
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
RW_FUNCTION(0x6DFCF8, "ScenePartyRequest::openPopUpLux $_65 __func::__clone(__base*)", t_6DFCF8);

// ScenePartyRequest::openPopUpLux $_65: __func::destroy
static void t_6DFD28(uint32_t self) {
}
RW_FUNCTION(0x6DFD28, "ScenePartyRequest::openPopUpLux $_65 __func::destroy", t_6DFD28);

// ScenePartyRequest::openPopUpLux $_65: __func::destroy_deallocate
static void t_6DFD2A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFD2A, "ScenePartyRequest::openPopUpLux $_65 __func::destroy_deallocate", t_6DFD2A);

// ScenePartyRequest::openPopUpLux $_65: __func::target
static uint32_t t_6DFD4C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82390u ? self + 4 : 0; }
RW_FUNCTION(0x6DFD4C, "ScenePartyRequest::openPopUpLux $_65 __func::target", t_6DFD4C);

// ScenePartyRequest::openPopUpLux $_65: __func::target_type
static uint32_t t_6DFD6C() { return 0xfa10f0u; }
RW_FUNCTION(0x6DFD6C, "ScenePartyRequest::openPopUpLux $_65 __func::target_type", t_6DFD6C);

// ScenePartyRequest::openPopUpLux $_64: __func::~__func (deleting)
static void t_6DFD80(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFD80, "ScenePartyRequest::openPopUpLux $_64 __func::~__func (deleting)", t_6DFD80);

// ScenePartyRequest::openPopUpLux $_64: __func::__clone()
static uint32_t t_6DFD88(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf683e4u);
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
RW_FUNCTION(0x6DFD88, "ScenePartyRequest::openPopUpLux $_64 __func::__clone()", t_6DFD88);

// ScenePartyRequest::openPopUpLux $_64: __func::__clone(__base*)
static void t_6DFDC4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf683e4u);
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
RW_FUNCTION(0x6DFDC4, "ScenePartyRequest::openPopUpLux $_64 __func::__clone(__base*)", t_6DFDC4);

// ScenePartyRequest::openPopUpLux $_64: __func::destroy
static void t_6DFDF4(uint32_t self) {
}
RW_FUNCTION(0x6DFDF4, "ScenePartyRequest::openPopUpLux $_64 __func::destroy", t_6DFDF4);

// ScenePartyRequest::openPopUpLux $_64: __func::destroy_deallocate
static void t_6DFDF6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFDF6, "ScenePartyRequest::openPopUpLux $_64 __func::destroy_deallocate", t_6DFDF6);

// ScenePartyRequest::openPopUpLux $_64: __func::target
static uint32_t t_6DFE18(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82450u ? self + 4 : 0; }
RW_FUNCTION(0x6DFE18, "ScenePartyRequest::openPopUpLux $_64 __func::target", t_6DFE18);

// ScenePartyRequest::openPopUpLux $_64: __func::target_type
static uint32_t t_6DFE38() { return 0xfa1104u; }
RW_FUNCTION(0x6DFE38, "ScenePartyRequest::openPopUpLux $_64 __func::target_type", t_6DFE38);

// ScenePartyRequest::openPopUpLux $_63: __func::~__func (deleting)
static void t_6DFE4C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFE4C, "ScenePartyRequest::openPopUpLux $_63 __func::~__func (deleting)", t_6DFE4C);

// ScenePartyRequest::openPopUpLux $_63: __func::__clone()
static uint32_t t_6DFE54(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf68410u);
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
RW_FUNCTION(0x6DFE54, "ScenePartyRequest::openPopUpLux $_63 __func::__clone()", t_6DFE54);

// ScenePartyRequest::openPopUpLux $_63: __func::__clone(__base*)
static void t_6DFE90(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68410u);
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
RW_FUNCTION(0x6DFE90, "ScenePartyRequest::openPopUpLux $_63 __func::__clone(__base*)", t_6DFE90);

// ScenePartyRequest::openPopUpLux $_63: __func::destroy
static void t_6DFEC0(uint32_t self) {
}
RW_FUNCTION(0x6DFEC0, "ScenePartyRequest::openPopUpLux $_63 __func::destroy", t_6DFEC0);

// ScenePartyRequest::openPopUpLux $_63: __func::destroy_deallocate
static void t_6DFEC2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFEC2, "ScenePartyRequest::openPopUpLux $_63 __func::destroy_deallocate", t_6DFEC2);

// ScenePartyRequest::openPopUpLux $_63: __func::target
static uint32_t t_6DFEE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82510u ? self + 4 : 0; }
RW_FUNCTION(0x6DFEE4, "ScenePartyRequest::openPopUpLux $_63 __func::target", t_6DFEE4);

// ScenePartyRequest::openPopUpLux $_63: __func::target_type
static uint32_t t_6DFF04() { return 0xfa1118u; }
RW_FUNCTION(0x6DFF04, "ScenePartyRequest::openPopUpLux $_63 __func::target_type", t_6DFF04);

// ScenePartyRequest::openPopUpLux $_61: __func::~__func (deleting)
static void t_6DFF18(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFF18, "ScenePartyRequest::openPopUpLux $_61 __func::~__func (deleting)", t_6DFF18);

// ScenePartyRequest::openPopUpLux $_61: __func::__clone()
static uint32_t t_6DFF20(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6843cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6DFF20, "ScenePartyRequest::openPopUpLux $_61 __func::__clone()", t_6DFF20);

// ScenePartyRequest::openPopUpLux $_61: __func::__clone(__base*)
static void t_6DFF4C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6843cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6DFF4C, "ScenePartyRequest::openPopUpLux $_61 __func::__clone(__base*)", t_6DFF4C);

// ScenePartyRequest::openPopUpLux $_61: __func::destroy
static void t_6DFF6C(uint32_t self) {
}
RW_FUNCTION(0x6DFF6C, "ScenePartyRequest::openPopUpLux $_61 __func::destroy", t_6DFF6C);

// ScenePartyRequest::openPopUpLux $_61: __func::destroy_deallocate
static void t_6DFF6E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6DFF6E, "ScenePartyRequest::openPopUpLux $_61 __func::destroy_deallocate", t_6DFF6E);

// ScenePartyRequest::openPopUpLux $_61: __func::target
static uint32_t t_6E01A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe825a0u ? self + 4 : 0; }
RW_FUNCTION(0x6E01A0, "ScenePartyRequest::openPopUpLux $_61 __func::target", t_6E01A0);

// ScenePartyRequest::openPopUpLux $_61: __func::target_type
static uint32_t t_6E01C0() { return 0xfa112cu; }
RW_FUNCTION(0x6E01C0, "ScenePartyRequest::openPopUpLux $_61 __func::target_type", t_6E01C0);

// ScenePartyRequest::openPopUpLevel $_60: __func::~__func (deleting)
static void t_6E01D4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E01D4, "ScenePartyRequest::openPopUpLevel $_60 __func::~__func (deleting)", t_6E01D4);

// ScenePartyRequest::openPopUpLevel $_60: __func::__clone()
static uint32_t t_6E01DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf68468u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6E01DC, "ScenePartyRequest::openPopUpLevel $_60 __func::__clone()", t_6E01DC);

// ScenePartyRequest::openPopUpLevel $_60: __func::__clone(__base*)
static void t_6E0214(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68468u);
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
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6E0214, "ScenePartyRequest::openPopUpLevel $_60 __func::__clone(__base*)", t_6E0214);

// ScenePartyRequest::openPopUpLevel $_60: __func::destroy
static void t_6E0240(uint32_t self) {
}
RW_FUNCTION(0x6E0240, "ScenePartyRequest::openPopUpLevel $_60 __func::destroy", t_6E0240);

// ScenePartyRequest::openPopUpLevel $_60: __func::destroy_deallocate
static void t_6E0242(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0242, "ScenePartyRequest::openPopUpLevel $_60 __func::destroy_deallocate", t_6E0242);

// ScenePartyRequest::openPopUpLevel $_60: __func::target
static uint32_t t_6E0264(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82660u ? self + 4 : 0; }
RW_FUNCTION(0x6E0264, "ScenePartyRequest::openPopUpLevel $_60 __func::target", t_6E0264);

// ScenePartyRequest::openPopUpLevel $_60: __func::target_type
static uint32_t t_6E0284() { return 0xfa1140u; }
RW_FUNCTION(0x6E0284, "ScenePartyRequest::openPopUpLevel $_60 __func::target_type", t_6E0284);

// ScenePartyRequest::openPopUpLevel $_59: __func::~__func (deleting)
static void t_6E0298(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0298, "ScenePartyRequest::openPopUpLevel $_59 __func::~__func (deleting)", t_6E0298);

// ScenePartyRequest::openPopUpLevel $_59: __func::__clone()
static uint32_t t_6E02A0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf68494u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6E02A0, "ScenePartyRequest::openPopUpLevel $_59 __func::__clone()", t_6E02A0);

// ScenePartyRequest::openPopUpLevel $_59: __func::__clone(__base*)
static void t_6E02D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68494u);
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
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6E02D8, "ScenePartyRequest::openPopUpLevel $_59 __func::__clone(__base*)", t_6E02D8);

// ScenePartyRequest::openPopUpLevel $_59: __func::destroy
static void t_6E0304(uint32_t self) {
}
RW_FUNCTION(0x6E0304, "ScenePartyRequest::openPopUpLevel $_59 __func::destroy", t_6E0304);

// ScenePartyRequest::openPopUpLevel $_59: __func::destroy_deallocate
static void t_6E0306(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0306, "ScenePartyRequest::openPopUpLevel $_59 __func::destroy_deallocate", t_6E0306);

// ScenePartyRequest::openPopUpLevel $_59: __func::target
static uint32_t t_6E0328(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82720u ? self + 4 : 0; }
RW_FUNCTION(0x6E0328, "ScenePartyRequest::openPopUpLevel $_59 __func::target", t_6E0328);

// ScenePartyRequest::openPopUpLevel $_59: __func::target_type
static uint32_t t_6E0348() { return 0xfa1154u; }
RW_FUNCTION(0x6E0348, "ScenePartyRequest::openPopUpLevel $_59 __func::target_type", t_6E0348);

// ScenePartyRequest::openPopUpLevel $_58: __func::~__func (deleting)
static void t_6E035C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E035C, "ScenePartyRequest::openPopUpLevel $_58 __func::~__func (deleting)", t_6E035C);

// ScenePartyRequest::openPopUpLevel $_58: __func::__clone()
static uint32_t t_6E0364(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf684c0u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6E0364, "ScenePartyRequest::openPopUpLevel $_58 __func::__clone()", t_6E0364);

// ScenePartyRequest::openPopUpLevel $_58: __func::__clone(__base*)
static void t_6E039C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf684c0u);
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
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6E039C, "ScenePartyRequest::openPopUpLevel $_58 __func::__clone(__base*)", t_6E039C);

// ScenePartyRequest::openPopUpLevel $_58: __func::destroy
static void t_6E03C8(uint32_t self) {
}
RW_FUNCTION(0x6E03C8, "ScenePartyRequest::openPopUpLevel $_58 __func::destroy", t_6E03C8);

// ScenePartyRequest::openPopUpLevel $_58: __func::destroy_deallocate
static void t_6E03CA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E03CA, "ScenePartyRequest::openPopUpLevel $_58 __func::destroy_deallocate", t_6E03CA);

// ScenePartyRequest::openPopUpLevel $_58: __func::target
static uint32_t t_6E03EC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe827e0u ? self + 4 : 0; }
RW_FUNCTION(0x6E03EC, "ScenePartyRequest::openPopUpLevel $_58 __func::target", t_6E03EC);

// ScenePartyRequest::openPopUpLevel $_58: __func::target_type
static uint32_t t_6E040C() { return 0xfa1168u; }
RW_FUNCTION(0x6E040C, "ScenePartyRequest::openPopUpLevel $_58 __func::target_type", t_6E040C);

// ScenePartyRequest::openPopUpLevel $_57: __func::~__func (deleting)
static void t_6E0420(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0420, "ScenePartyRequest::openPopUpLevel $_57 __func::~__func (deleting)", t_6E0420);

// ScenePartyRequest::openPopUpLevel $_57: __func::__clone()
static uint32_t t_6E0428(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf684ecu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6E0428, "ScenePartyRequest::openPopUpLevel $_57 __func::__clone()", t_6E0428);

// ScenePartyRequest::openPopUpLevel $_57: __func::__clone(__base*)
static void t_6E0460(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf684ecu);
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
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6E0460, "ScenePartyRequest::openPopUpLevel $_57 __func::__clone(__base*)", t_6E0460);

// ScenePartyRequest::openPopUpLevel $_57: __func::destroy
static void t_6E048C(uint32_t self) {
}
RW_FUNCTION(0x6E048C, "ScenePartyRequest::openPopUpLevel $_57 __func::destroy", t_6E048C);

// ScenePartyRequest::openPopUpLevel $_57: __func::destroy_deallocate
static void t_6E048E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E048E, "ScenePartyRequest::openPopUpLevel $_57 __func::destroy_deallocate", t_6E048E);

// ScenePartyRequest::openPopUpLevel $_57: __func::target
static uint32_t t_6E04B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe828a0u ? self + 4 : 0; }
RW_FUNCTION(0x6E04B0, "ScenePartyRequest::openPopUpLevel $_57 __func::target", t_6E04B0);

// ScenePartyRequest::openPopUpLevel $_57: __func::target_type
static uint32_t t_6E04D0() { return 0xfa117cu; }
RW_FUNCTION(0x6E04D0, "ScenePartyRequest::openPopUpLevel $_57 __func::target_type", t_6E04D0);

// ScenePartyRequest::openPopUpLevel $_56: __func::~__func (deleting)
static void t_6E04E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E04E4, "ScenePartyRequest::openPopUpLevel $_56 __func::~__func (deleting)", t_6E04E4);

// ScenePartyRequest::openPopUpLevel $_56: __func::__clone()
static uint32_t t_6E04EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf68518u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6E04EC, "ScenePartyRequest::openPopUpLevel $_56 __func::__clone()", t_6E04EC);

// ScenePartyRequest::openPopUpLevel $_56: __func::__clone(__base*)
static void t_6E0524(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68518u);
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
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6E0524, "ScenePartyRequest::openPopUpLevel $_56 __func::__clone(__base*)", t_6E0524);

// ScenePartyRequest::openPopUpLevel $_56: __func::destroy
static void t_6E0550(uint32_t self) {
}
RW_FUNCTION(0x6E0550, "ScenePartyRequest::openPopUpLevel $_56 __func::destroy", t_6E0550);

// ScenePartyRequest::openPopUpLevel $_56: __func::destroy_deallocate
static void t_6E0552(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0552, "ScenePartyRequest::openPopUpLevel $_56 __func::destroy_deallocate", t_6E0552);

// ScenePartyRequest::openPopUpLevel $_56: __func::target
static uint32_t t_6E0574(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82960u ? self + 4 : 0; }
RW_FUNCTION(0x6E0574, "ScenePartyRequest::openPopUpLevel $_56 __func::target", t_6E0574);

// ScenePartyRequest::openPopUpLevel $_56: __func::target_type
static uint32_t t_6E0594() { return 0xfa1190u; }
RW_FUNCTION(0x6E0594, "ScenePartyRequest::openPopUpLevel $_56 __func::target_type", t_6E0594);

// ScenePartyRequest::openPopUpLevel $_54: __func::~__func (deleting)
static void t_6E05A8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E05A8, "ScenePartyRequest::openPopUpLevel $_54 __func::~__func (deleting)", t_6E05A8);

// ScenePartyRequest::openPopUpLevel $_54: __func::__clone()
static uint32_t t_6E05B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68544u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E05B0, "ScenePartyRequest::openPopUpLevel $_54 __func::__clone()", t_6E05B0);

// ScenePartyRequest::openPopUpLevel $_54: __func::__clone(__base*)
static void t_6E05DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68544u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E05DC, "ScenePartyRequest::openPopUpLevel $_54 __func::__clone(__base*)", t_6E05DC);

// ScenePartyRequest::openPopUpLevel $_54: __func::destroy
static void t_6E05FC(uint32_t self) {
}
RW_FUNCTION(0x6E05FC, "ScenePartyRequest::openPopUpLevel $_54 __func::destroy", t_6E05FC);

// ScenePartyRequest::openPopUpLevel $_54: __func::destroy_deallocate
static void t_6E05FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E05FE, "ScenePartyRequest::openPopUpLevel $_54 __func::destroy_deallocate", t_6E05FE);

// ScenePartyRequest::openPopUpLevel $_54: __func::target
static uint32_t t_6E081C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82a00u ? self + 4 : 0; }
RW_FUNCTION(0x6E081C, "ScenePartyRequest::openPopUpLevel $_54 __func::target", t_6E081C);

// ScenePartyRequest::openPopUpLevel $_54: __func::target_type
static uint32_t t_6E083C() { return 0xfa11a4u; }
RW_FUNCTION(0x6E083C, "ScenePartyRequest::openPopUpLevel $_54 __func::target_type", t_6E083C);

// ScenePartyRequest::openPopUpTimeZone $_53: __func::~__func (deleting)
static void t_6E0850(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0850, "ScenePartyRequest::openPopUpTimeZone $_53 __func::~__func (deleting)", t_6E0850);

// ScenePartyRequest::openPopUpTimeZone $_53: __func::__clone()
static uint32_t t_6E0858(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf68570u);
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
        uint32_t v9 = ld32(self + 20 + 12);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
        st32(v1 + 20 + 12, v9);
    }
    return v1;
}
RW_FUNCTION(0x6E0858, "ScenePartyRequest::openPopUpTimeZone $_53 __func::__clone()", t_6E0858);

// ScenePartyRequest::openPopUpTimeZone $_53: __func::__clone(__base*)
static void t_6E0890(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68570u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20 + 0);
        uint32_t v6 = ld32(self + 20 + 4);
        uint32_t v7 = ld32(self + 20 + 8);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
    }
}
RW_FUNCTION(0x6E0890, "ScenePartyRequest::openPopUpTimeZone $_53 __func::__clone(__base*)", t_6E0890);

// ScenePartyRequest::openPopUpTimeZone $_53: __func::destroy
static void t_6E08C4(uint32_t self) {
}
RW_FUNCTION(0x6E08C4, "ScenePartyRequest::openPopUpTimeZone $_53 __func::destroy", t_6E08C4);

// ScenePartyRequest::openPopUpTimeZone $_53: __func::destroy_deallocate
static void t_6E08C6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E08C6, "ScenePartyRequest::openPopUpTimeZone $_53 __func::destroy_deallocate", t_6E08C6);

// ScenePartyRequest::openPopUpTimeZone $_53: __func::target
static uint32_t t_6E08E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82ad0u ? self + 4 : 0; }
RW_FUNCTION(0x6E08E8, "ScenePartyRequest::openPopUpTimeZone $_53 __func::target", t_6E08E8);

// ScenePartyRequest::openPopUpTimeZone $_53: __func::target_type
static uint32_t t_6E0908() { return 0xfa11b8u; }
RW_FUNCTION(0x6E0908, "ScenePartyRequest::openPopUpTimeZone $_53 __func::target_type", t_6E0908);

// ScenePartyRequest::openPopUpTimeZone $_52: __func::~__func (deleting)
static void t_6E091C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E091C, "ScenePartyRequest::openPopUpTimeZone $_52 __func::~__func (deleting)", t_6E091C);

// ScenePartyRequest::openPopUpTimeZone $_52: __func::__clone()
static uint32_t t_6E0924(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf6859cu);
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
        uint32_t v9 = ld32(self + 20 + 12);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
        st32(v1 + 20 + 12, v9);
    }
    return v1;
}
RW_FUNCTION(0x6E0924, "ScenePartyRequest::openPopUpTimeZone $_52 __func::__clone()", t_6E0924);

// ScenePartyRequest::openPopUpTimeZone $_52: __func::__clone(__base*)
static void t_6E095C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6859cu);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20 + 0);
        uint32_t v6 = ld32(self + 20 + 4);
        uint32_t v7 = ld32(self + 20 + 8);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
    }
}
RW_FUNCTION(0x6E095C, "ScenePartyRequest::openPopUpTimeZone $_52 __func::__clone(__base*)", t_6E095C);

// ScenePartyRequest::openPopUpTimeZone $_52: __func::destroy
static void t_6E0990(uint32_t self) {
}
RW_FUNCTION(0x6E0990, "ScenePartyRequest::openPopUpTimeZone $_52 __func::destroy", t_6E0990);

// ScenePartyRequest::openPopUpTimeZone $_52: __func::destroy_deallocate
static void t_6E0992(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0992, "ScenePartyRequest::openPopUpTimeZone $_52 __func::destroy_deallocate", t_6E0992);

// ScenePartyRequest::openPopUpTimeZone $_52: __func::target
static uint32_t t_6E09B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82bb0u ? self + 4 : 0; }
RW_FUNCTION(0x6E09B4, "ScenePartyRequest::openPopUpTimeZone $_52 __func::target", t_6E09B4);

// ScenePartyRequest::openPopUpTimeZone $_52: __func::target_type
static uint32_t t_6E09D4() { return 0xfa11ccu; }
RW_FUNCTION(0x6E09D4, "ScenePartyRequest::openPopUpTimeZone $_52 __func::target_type", t_6E09D4);

// ScenePartyRequest::openPopUpTimeZone $_51: __func::~__func (deleting)
static void t_6E09E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E09E8, "ScenePartyRequest::openPopUpTimeZone $_51 __func::~__func (deleting)", t_6E09E8);

// ScenePartyRequest::openPopUpTimeZone $_51: __func::__clone()
static uint32_t t_6E09F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf685c8u);
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
        uint32_t v9 = ld32(self + 20 + 12);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
        st32(v1 + 20 + 12, v9);
    }
    return v1;
}
RW_FUNCTION(0x6E09F0, "ScenePartyRequest::openPopUpTimeZone $_51 __func::__clone()", t_6E09F0);

// ScenePartyRequest::openPopUpTimeZone $_51: __func::__clone(__base*)
static void t_6E0A28(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf685c8u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20 + 0);
        uint32_t v6 = ld32(self + 20 + 4);
        uint32_t v7 = ld32(self + 20 + 8);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
    }
}
RW_FUNCTION(0x6E0A28, "ScenePartyRequest::openPopUpTimeZone $_51 __func::__clone(__base*)", t_6E0A28);

// ScenePartyRequest::openPopUpTimeZone $_51: __func::destroy
static void t_6E0A5C(uint32_t self) {
}
RW_FUNCTION(0x6E0A5C, "ScenePartyRequest::openPopUpTimeZone $_51 __func::destroy", t_6E0A5C);

// ScenePartyRequest::openPopUpTimeZone $_51: __func::destroy_deallocate
static void t_6E0A5E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0A5E, "ScenePartyRequest::openPopUpTimeZone $_51 __func::destroy_deallocate", t_6E0A5E);

// ScenePartyRequest::openPopUpTimeZone $_51: __func::target
static uint32_t t_6E0A80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82c90u ? self + 4 : 0; }
RW_FUNCTION(0x6E0A80, "ScenePartyRequest::openPopUpTimeZone $_51 __func::target", t_6E0A80);

// ScenePartyRequest::openPopUpTimeZone $_51: __func::target_type
static uint32_t t_6E0AA0() { return 0xfa11e0u; }
RW_FUNCTION(0x6E0AA0, "ScenePartyRequest::openPopUpTimeZone $_51 __func::target_type", t_6E0AA0);

// ScenePartyRequest::openPopUpTimeZone $_50: __func::~__func (deleting)
static void t_6E0AB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0AB4, "ScenePartyRequest::openPopUpTimeZone $_50 __func::~__func (deleting)", t_6E0AB4);

// ScenePartyRequest::openPopUpTimeZone $_50: __func::__clone()
static uint32_t t_6E0ABC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf685f4u);
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
        uint32_t v9 = ld32(self + 20 + 12);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
        st32(v1 + 20 + 12, v9);
    }
    return v1;
}
RW_FUNCTION(0x6E0ABC, "ScenePartyRequest::openPopUpTimeZone $_50 __func::__clone()", t_6E0ABC);

// ScenePartyRequest::openPopUpTimeZone $_50: __func::__clone(__base*)
static void t_6E0AF4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf685f4u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20 + 0);
        uint32_t v6 = ld32(self + 20 + 4);
        uint32_t v7 = ld32(self + 20 + 8);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
    }
}
RW_FUNCTION(0x6E0AF4, "ScenePartyRequest::openPopUpTimeZone $_50 __func::__clone(__base*)", t_6E0AF4);

// ScenePartyRequest::openPopUpTimeZone $_50: __func::destroy
static void t_6E0B28(uint32_t self) {
}
RW_FUNCTION(0x6E0B28, "ScenePartyRequest::openPopUpTimeZone $_50 __func::destroy", t_6E0B28);

// ScenePartyRequest::openPopUpTimeZone $_50: __func::destroy_deallocate
static void t_6E0B2A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0B2A, "ScenePartyRequest::openPopUpTimeZone $_50 __func::destroy_deallocate", t_6E0B2A);

// ScenePartyRequest::openPopUpTimeZone $_50: __func::target
static uint32_t t_6E0B4C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82d70u ? self + 4 : 0; }
RW_FUNCTION(0x6E0B4C, "ScenePartyRequest::openPopUpTimeZone $_50 __func::target", t_6E0B4C);

// ScenePartyRequest::openPopUpTimeZone $_50: __func::target_type
static uint32_t t_6E0B6C() { return 0xfa11f4u; }
RW_FUNCTION(0x6E0B6C, "ScenePartyRequest::openPopUpTimeZone $_50 __func::target_type", t_6E0B6C);

// ScenePartyRequest::openPopUpTimeZone $_49: __func::~__func (deleting)
static void t_6E0B80(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0B80, "ScenePartyRequest::openPopUpTimeZone $_49 __func::~__func (deleting)", t_6E0B80);

// ScenePartyRequest::openPopUpTimeZone $_49: __func::__clone()
static uint32_t t_6E0B88(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf68620u);
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
        uint32_t v9 = ld32(self + 20 + 12);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
        st32(v1 + 20 + 12, v9);
    }
    return v1;
}
RW_FUNCTION(0x6E0B88, "ScenePartyRequest::openPopUpTimeZone $_49 __func::__clone()", t_6E0B88);

// ScenePartyRequest::openPopUpTimeZone $_49: __func::__clone(__base*)
static void t_6E0BC0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68620u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20 + 0);
        uint32_t v6 = ld32(self + 20 + 4);
        uint32_t v7 = ld32(self + 20 + 8);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
    }
}
RW_FUNCTION(0x6E0BC0, "ScenePartyRequest::openPopUpTimeZone $_49 __func::__clone(__base*)", t_6E0BC0);

// ScenePartyRequest::openPopUpTimeZone $_49: __func::destroy
static void t_6E0BF4(uint32_t self) {
}
RW_FUNCTION(0x6E0BF4, "ScenePartyRequest::openPopUpTimeZone $_49 __func::destroy", t_6E0BF4);

// ScenePartyRequest::openPopUpTimeZone $_49: __func::destroy_deallocate
static void t_6E0BF6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0BF6, "ScenePartyRequest::openPopUpTimeZone $_49 __func::destroy_deallocate", t_6E0BF6);

// ScenePartyRequest::openPopUpTimeZone $_49: __func::target
static uint32_t t_6E0C18(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82e50u ? self + 4 : 0; }
RW_FUNCTION(0x6E0C18, "ScenePartyRequest::openPopUpTimeZone $_49 __func::target", t_6E0C18);

// ScenePartyRequest::openPopUpTimeZone $_49: __func::target_type
static uint32_t t_6E0C38() { return 0xfa1208u; }
RW_FUNCTION(0x6E0C38, "ScenePartyRequest::openPopUpTimeZone $_49 __func::target_type", t_6E0C38);

// ScenePartyRequest::openPopUpTimeZone $_48: __func::~__func (deleting)
static void t_6E0C4C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0C4C, "ScenePartyRequest::openPopUpTimeZone $_48 __func::~__func (deleting)", t_6E0C4C);

// ScenePartyRequest::openPopUpTimeZone $_48: __func::__clone()
static uint32_t t_6E0C54(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf6864cu);
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
        uint32_t v9 = ld32(self + 20 + 12);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
        st32(v1 + 20 + 12, v9);
    }
    return v1;
}
RW_FUNCTION(0x6E0C54, "ScenePartyRequest::openPopUpTimeZone $_48 __func::__clone()", t_6E0C54);

// ScenePartyRequest::openPopUpTimeZone $_48: __func::__clone(__base*)
static void t_6E0C8C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6864cu);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20 + 0);
        uint32_t v6 = ld32(self + 20 + 4);
        uint32_t v7 = ld32(self + 20 + 8);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
    }
}
RW_FUNCTION(0x6E0C8C, "ScenePartyRequest::openPopUpTimeZone $_48 __func::__clone(__base*)", t_6E0C8C);

// ScenePartyRequest::openPopUpTimeZone $_48: __func::destroy
static void t_6E0CC0(uint32_t self) {
}
RW_FUNCTION(0x6E0CC0, "ScenePartyRequest::openPopUpTimeZone $_48 __func::destroy", t_6E0CC0);

// ScenePartyRequest::openPopUpTimeZone $_48: __func::destroy_deallocate
static void t_6E0CC2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0CC2, "ScenePartyRequest::openPopUpTimeZone $_48 __func::destroy_deallocate", t_6E0CC2);

// ScenePartyRequest::openPopUpTimeZone $_48: __func::target
static uint32_t t_6E0CE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe82f30u ? self + 4 : 0; }
RW_FUNCTION(0x6E0CE4, "ScenePartyRequest::openPopUpTimeZone $_48 __func::target", t_6E0CE4);

// ScenePartyRequest::openPopUpTimeZone $_48: __func::target_type
static uint32_t t_6E0D04() { return 0xfa121cu; }
RW_FUNCTION(0x6E0D04, "ScenePartyRequest::openPopUpTimeZone $_48 __func::target_type", t_6E0D04);

// ScenePartyRequest::openPopUpTimeZone $_47: __func::~__func (deleting)
static void t_6E0D18(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0D18, "ScenePartyRequest::openPopUpTimeZone $_47 __func::~__func (deleting)", t_6E0D18);

// ScenePartyRequest::openPopUpTimeZone $_47: __func::__clone()
static uint32_t t_6E0D20(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf68678u);
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
        uint32_t v9 = ld32(self + 20 + 12);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
        st32(v1 + 20 + 12, v9);
    }
    return v1;
}
RW_FUNCTION(0x6E0D20, "ScenePartyRequest::openPopUpTimeZone $_47 __func::__clone()", t_6E0D20);

// ScenePartyRequest::openPopUpTimeZone $_47: __func::__clone(__base*)
static void t_6E0D58(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68678u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20 + 0);
        uint32_t v6 = ld32(self + 20 + 4);
        uint32_t v7 = ld32(self + 20 + 8);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
    }
}
RW_FUNCTION(0x6E0D58, "ScenePartyRequest::openPopUpTimeZone $_47 __func::__clone(__base*)", t_6E0D58);

// ScenePartyRequest::openPopUpTimeZone $_47: __func::destroy
static void t_6E0D8C(uint32_t self) {
}
RW_FUNCTION(0x6E0D8C, "ScenePartyRequest::openPopUpTimeZone $_47 __func::destroy", t_6E0D8C);

// ScenePartyRequest::openPopUpTimeZone $_47: __func::destroy_deallocate
static void t_6E0D8E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0D8E, "ScenePartyRequest::openPopUpTimeZone $_47 __func::destroy_deallocate", t_6E0D8E);

// ScenePartyRequest::openPopUpTimeZone $_47: __func::target
static uint32_t t_6E0DB0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83010u ? self + 4 : 0; }
RW_FUNCTION(0x6E0DB0, "ScenePartyRequest::openPopUpTimeZone $_47 __func::target", t_6E0DB0);

// ScenePartyRequest::openPopUpTimeZone $_47: __func::target_type
static uint32_t t_6E0DD0() { return 0xfa1230u; }
RW_FUNCTION(0x6E0DD0, "ScenePartyRequest::openPopUpTimeZone $_47 __func::target_type", t_6E0DD0);

// ScenePartyRequest::openPopUpTimeZone $_45: __func::~__func (deleting)
static void t_6E0DE4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0DE4, "ScenePartyRequest::openPopUpTimeZone $_45 __func::~__func (deleting)", t_6E0DE4);

// ScenePartyRequest::openPopUpTimeZone $_45: __func::__clone()
static uint32_t t_6E0DEC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf686a4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E0DEC, "ScenePartyRequest::openPopUpTimeZone $_45 __func::__clone()", t_6E0DEC);

// ScenePartyRequest::openPopUpTimeZone $_45: __func::__clone(__base*)
static void t_6E0E18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf686a4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E0E18, "ScenePartyRequest::openPopUpTimeZone $_45 __func::__clone(__base*)", t_6E0E18);

// ScenePartyRequest::openPopUpTimeZone $_45: __func::destroy
static void t_6E0E38(uint32_t self) {
}
RW_FUNCTION(0x6E0E38, "ScenePartyRequest::openPopUpTimeZone $_45 __func::destroy", t_6E0E38);

// ScenePartyRequest::openPopUpTimeZone $_45: __func::destroy_deallocate
static void t_6E0E3A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E0E3A, "ScenePartyRequest::openPopUpTimeZone $_45 __func::destroy_deallocate", t_6E0E3A);

// ScenePartyRequest::openPopUpTimeZone $_45: __func::target
static uint32_t t_6E1084(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe830c0u ? self + 4 : 0; }
RW_FUNCTION(0x6E1084, "ScenePartyRequest::openPopUpTimeZone $_45 __func::target", t_6E1084);

// ScenePartyRequest::openPopUpTimeZone $_45: __func::target_type
static uint32_t t_6E10A4() { return 0xfa1244u; }
RW_FUNCTION(0x6E10A4, "ScenePartyRequest::openPopUpTimeZone $_45 __func::target_type", t_6E10A4);

// ScenePartyRequest::openPopUpMember $_44: __func::~__func (deleting)
static void t_6E10B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E10B8, "ScenePartyRequest::openPopUpMember $_44 __func::~__func (deleting)", t_6E10B8);

// ScenePartyRequest::openPopUpMember $_44: __func::__clone()
static uint32_t t_6E10C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf686d0u);
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
RW_FUNCTION(0x6E10C0, "ScenePartyRequest::openPopUpMember $_44 __func::__clone()", t_6E10C0);

// ScenePartyRequest::openPopUpMember $_44: __func::__clone(__base*)
static void t_6E10F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf686d0u);
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
RW_FUNCTION(0x6E10F4, "ScenePartyRequest::openPopUpMember $_44 __func::__clone(__base*)", t_6E10F4);

// ScenePartyRequest::openPopUpMember $_44: __func::destroy
static void t_6E111C(uint32_t self) {
}
RW_FUNCTION(0x6E111C, "ScenePartyRequest::openPopUpMember $_44 __func::destroy", t_6E111C);

// ScenePartyRequest::openPopUpMember $_44: __func::destroy_deallocate
static void t_6E111E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E111E, "ScenePartyRequest::openPopUpMember $_44 __func::destroy_deallocate", t_6E111E);

// ScenePartyRequest::openPopUpMember $_44: __func::target
static uint32_t t_6E1140(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83190u ? self + 4 : 0; }
RW_FUNCTION(0x6E1140, "ScenePartyRequest::openPopUpMember $_44 __func::target", t_6E1140);

// ScenePartyRequest::openPopUpMember $_44: __func::target_type
static uint32_t t_6E1160() { return 0xfa1258u; }
RW_FUNCTION(0x6E1160, "ScenePartyRequest::openPopUpMember $_44 __func::target_type", t_6E1160);

// ScenePartyRequest::openPopUpMember $_43: __func::~__func (deleting)
static void t_6E1174(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1174, "ScenePartyRequest::openPopUpMember $_43 __func::~__func (deleting)", t_6E1174);

// ScenePartyRequest::openPopUpMember $_43: __func::__clone()
static uint32_t t_6E117C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf686fcu);
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
RW_FUNCTION(0x6E117C, "ScenePartyRequest::openPopUpMember $_43 __func::__clone()", t_6E117C);

// ScenePartyRequest::openPopUpMember $_43: __func::__clone(__base*)
static void t_6E11B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf686fcu);
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
RW_FUNCTION(0x6E11B0, "ScenePartyRequest::openPopUpMember $_43 __func::__clone(__base*)", t_6E11B0);

// ScenePartyRequest::openPopUpMember $_43: __func::destroy
static void t_6E11D8(uint32_t self) {
}
RW_FUNCTION(0x6E11D8, "ScenePartyRequest::openPopUpMember $_43 __func::destroy", t_6E11D8);

// ScenePartyRequest::openPopUpMember $_43: __func::destroy_deallocate
static void t_6E11DA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E11DA, "ScenePartyRequest::openPopUpMember $_43 __func::destroy_deallocate", t_6E11DA);

// ScenePartyRequest::openPopUpMember $_43: __func::target
static uint32_t t_6E11FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83250u ? self + 4 : 0; }
RW_FUNCTION(0x6E11FC, "ScenePartyRequest::openPopUpMember $_43 __func::target", t_6E11FC);

// ScenePartyRequest::openPopUpMember $_43: __func::target_type
static uint32_t t_6E121C() { return 0xfa126cu; }
RW_FUNCTION(0x6E121C, "ScenePartyRequest::openPopUpMember $_43 __func::target_type", t_6E121C);

// ScenePartyRequest::openPopUpMember $_42: __func::~__func (deleting)
static void t_6E1230(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1230, "ScenePartyRequest::openPopUpMember $_42 __func::~__func (deleting)", t_6E1230);

// ScenePartyRequest::openPopUpMember $_42: __func::__clone()
static uint32_t t_6E1238(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf68728u);
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
RW_FUNCTION(0x6E1238, "ScenePartyRequest::openPopUpMember $_42 __func::__clone()", t_6E1238);

// ScenePartyRequest::openPopUpMember $_42: __func::__clone(__base*)
static void t_6E126C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68728u);
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
RW_FUNCTION(0x6E126C, "ScenePartyRequest::openPopUpMember $_42 __func::__clone(__base*)", t_6E126C);

// ScenePartyRequest::openPopUpMember $_42: __func::destroy
static void t_6E1294(uint32_t self) {
}
RW_FUNCTION(0x6E1294, "ScenePartyRequest::openPopUpMember $_42 __func::destroy", t_6E1294);

// ScenePartyRequest::openPopUpMember $_42: __func::destroy_deallocate
static void t_6E1296(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1296, "ScenePartyRequest::openPopUpMember $_42 __func::destroy_deallocate", t_6E1296);

// ScenePartyRequest::openPopUpMember $_42: __func::target
static uint32_t t_6E12B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83310u ? self + 4 : 0; }
RW_FUNCTION(0x6E12B8, "ScenePartyRequest::openPopUpMember $_42 __func::target", t_6E12B8);

// ScenePartyRequest::openPopUpMember $_42: __func::target_type
static uint32_t t_6E12D8() { return 0xfa1280u; }
RW_FUNCTION(0x6E12D8, "ScenePartyRequest::openPopUpMember $_42 __func::target_type", t_6E12D8);

// ScenePartyRequest::openPopUpMember $_41: __func::~__func (deleting)
static void t_6E12EC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E12EC, "ScenePartyRequest::openPopUpMember $_41 __func::~__func (deleting)", t_6E12EC);

// ScenePartyRequest::openPopUpMember $_41: __func::__clone()
static uint32_t t_6E12F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf68754u);
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
RW_FUNCTION(0x6E12F4, "ScenePartyRequest::openPopUpMember $_41 __func::__clone()", t_6E12F4);

// ScenePartyRequest::openPopUpMember $_41: __func::__clone(__base*)
static void t_6E1328(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68754u);
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
RW_FUNCTION(0x6E1328, "ScenePartyRequest::openPopUpMember $_41 __func::__clone(__base*)", t_6E1328);

// ScenePartyRequest::openPopUpMember $_41: __func::destroy
static void t_6E1350(uint32_t self) {
}
RW_FUNCTION(0x6E1350, "ScenePartyRequest::openPopUpMember $_41 __func::destroy", t_6E1350);

// ScenePartyRequest::openPopUpMember $_41: __func::destroy_deallocate
static void t_6E1352(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1352, "ScenePartyRequest::openPopUpMember $_41 __func::destroy_deallocate", t_6E1352);

// ScenePartyRequest::openPopUpMember $_41: __func::target
static uint32_t t_6E1374(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe833d0u ? self + 4 : 0; }
RW_FUNCTION(0x6E1374, "ScenePartyRequest::openPopUpMember $_41 __func::target", t_6E1374);

// ScenePartyRequest::openPopUpMember $_41: __func::target_type
static uint32_t t_6E1394() { return 0xfa1294u; }
RW_FUNCTION(0x6E1394, "ScenePartyRequest::openPopUpMember $_41 __func::target_type", t_6E1394);

// ScenePartyRequest::openPopUpMember $_39: __func::~__func (deleting)
static void t_6E13A8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E13A8, "ScenePartyRequest::openPopUpMember $_39 __func::~__func (deleting)", t_6E13A8);

// ScenePartyRequest::openPopUpMember $_39: __func::__clone()
static uint32_t t_6E13B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68780u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E13B0, "ScenePartyRequest::openPopUpMember $_39 __func::__clone()", t_6E13B0);

// ScenePartyRequest::openPopUpMember $_39: __func::__clone(__base*)
static void t_6E13DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68780u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E13DC, "ScenePartyRequest::openPopUpMember $_39 __func::__clone(__base*)", t_6E13DC);

// ScenePartyRequest::openPopUpMember $_39: __func::destroy
static void t_6E13FC(uint32_t self) {
}
RW_FUNCTION(0x6E13FC, "ScenePartyRequest::openPopUpMember $_39 __func::destroy", t_6E13FC);

// ScenePartyRequest::openPopUpMember $_39: __func::destroy_deallocate
static void t_6E13FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E13FE, "ScenePartyRequest::openPopUpMember $_39 __func::destroy_deallocate", t_6E13FE);

// ScenePartyRequest::openPopUpMember $_39: __func::target
static uint32_t t_6E15F4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83470u ? self + 4 : 0; }
RW_FUNCTION(0x6E15F4, "ScenePartyRequest::openPopUpMember $_39 __func::target", t_6E15F4);

// ScenePartyRequest::openPopUpMember $_39: __func::target_type
static uint32_t t_6E1614() { return 0xfa12a8u; }
RW_FUNCTION(0x6E1614, "ScenePartyRequest::openPopUpMember $_39 __func::target_type", t_6E1614);

// ScenePartyRequest::openPopUpJoin $_38: __func::~__func (deleting)
static void t_6E1628(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1628, "ScenePartyRequest::openPopUpJoin $_38 __func::~__func (deleting)", t_6E1628);

// ScenePartyRequest::openPopUpJoin $_38: __func::__clone()
static uint32_t t_6E1630(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf687acu);
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
RW_FUNCTION(0x6E1630, "ScenePartyRequest::openPopUpJoin $_38 __func::__clone()", t_6E1630);

// ScenePartyRequest::openPopUpJoin $_38: __func::__clone(__base*)
static void t_6E1664(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf687acu);
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
RW_FUNCTION(0x6E1664, "ScenePartyRequest::openPopUpJoin $_38 __func::__clone(__base*)", t_6E1664);

// ScenePartyRequest::openPopUpJoin $_38: __func::destroy
static void t_6E1690(uint32_t self) {
}
RW_FUNCTION(0x6E1690, "ScenePartyRequest::openPopUpJoin $_38 __func::destroy", t_6E1690);

// ScenePartyRequest::openPopUpJoin $_38: __func::destroy_deallocate
static void t_6E1692(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1692, "ScenePartyRequest::openPopUpJoin $_38 __func::destroy_deallocate", t_6E1692);

// ScenePartyRequest::openPopUpJoin $_38: __func::target
static uint32_t t_6E1718(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83530u ? self + 4 : 0; }
RW_FUNCTION(0x6E1718, "ScenePartyRequest::openPopUpJoin $_38 __func::target", t_6E1718);

// ScenePartyRequest::openPopUpJoin $_38: __func::target_type
static uint32_t t_6E1738() { return 0xfa12bcu; }
RW_FUNCTION(0x6E1738, "ScenePartyRequest::openPopUpJoin $_38 __func::target_type", t_6E1738);

// ScenePartyRequest::openPopUpJoin $_37: __func::~__func (deleting)
static void t_6E174C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E174C, "ScenePartyRequest::openPopUpJoin $_37 __func::~__func (deleting)", t_6E174C);

// ScenePartyRequest::openPopUpJoin $_37: __func::__clone()
static uint32_t t_6E1754(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf687d8u);
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
RW_FUNCTION(0x6E1754, "ScenePartyRequest::openPopUpJoin $_37 __func::__clone()", t_6E1754);

// ScenePartyRequest::openPopUpJoin $_37: __func::__clone(__base*)
static void t_6E1788(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf687d8u);
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
RW_FUNCTION(0x6E1788, "ScenePartyRequest::openPopUpJoin $_37 __func::__clone(__base*)", t_6E1788);

// ScenePartyRequest::openPopUpJoin $_37: __func::destroy
static void t_6E17B4(uint32_t self) {
}
RW_FUNCTION(0x6E17B4, "ScenePartyRequest::openPopUpJoin $_37 __func::destroy", t_6E17B4);

// ScenePartyRequest::openPopUpJoin $_37: __func::destroy_deallocate
static void t_6E17B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E17B6, "ScenePartyRequest::openPopUpJoin $_37 __func::destroy_deallocate", t_6E17B6);

// ScenePartyRequest::openPopUpJoin $_37: __func::target
static uint32_t t_6E1838(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe835f0u ? self + 4 : 0; }
RW_FUNCTION(0x6E1838, "ScenePartyRequest::openPopUpJoin $_37 __func::target", t_6E1838);

// ScenePartyRequest::openPopUpJoin $_37: __func::target_type
static uint32_t t_6E1858() { return 0xfa12d0u; }
RW_FUNCTION(0x6E1858, "ScenePartyRequest::openPopUpJoin $_37 __func::target_type", t_6E1858);

// ScenePartyRequest::openPopUpJoin $_36: __func::~__func (deleting)
static void t_6E186C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E186C, "ScenePartyRequest::openPopUpJoin $_36 __func::~__func (deleting)", t_6E186C);

// ScenePartyRequest::openPopUpJoin $_36: __func::__clone()
static uint32_t t_6E1874(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf68804u);
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
RW_FUNCTION(0x6E1874, "ScenePartyRequest::openPopUpJoin $_36 __func::__clone()", t_6E1874);

// ScenePartyRequest::openPopUpJoin $_36: __func::__clone(__base*)
static void t_6E18A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68804u);
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
RW_FUNCTION(0x6E18A8, "ScenePartyRequest::openPopUpJoin $_36 __func::__clone(__base*)", t_6E18A8);

// ScenePartyRequest::openPopUpJoin $_36: __func::destroy
static void t_6E18D4(uint32_t self) {
}
RW_FUNCTION(0x6E18D4, "ScenePartyRequest::openPopUpJoin $_36 __func::destroy", t_6E18D4);

// ScenePartyRequest::openPopUpJoin $_36: __func::destroy_deallocate
static void t_6E18D6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E18D6, "ScenePartyRequest::openPopUpJoin $_36 __func::destroy_deallocate", t_6E18D6);

// ScenePartyRequest::openPopUpJoin $_36: __func::target
static uint32_t t_6E1958(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe836b0u ? self + 4 : 0; }
RW_FUNCTION(0x6E1958, "ScenePartyRequest::openPopUpJoin $_36 __func::target", t_6E1958);

// ScenePartyRequest::openPopUpJoin $_36: __func::target_type
static uint32_t t_6E1978() { return 0xfa12e4u; }
RW_FUNCTION(0x6E1978, "ScenePartyRequest::openPopUpJoin $_36 __func::target_type", t_6E1978);

// ScenePartyRequest::openPopUpJoin $_34: __func::~__func (deleting)
static void t_6E198C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E198C, "ScenePartyRequest::openPopUpJoin $_34 __func::~__func (deleting)", t_6E198C);

// ScenePartyRequest::openPopUpJoin $_34: __func::__clone()
static uint32_t t_6E1994(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68830u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E1994, "ScenePartyRequest::openPopUpJoin $_34 __func::__clone()", t_6E1994);

// ScenePartyRequest::openPopUpJoin $_34: __func::__clone(__base*)
static void t_6E19C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68830u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E19C0, "ScenePartyRequest::openPopUpJoin $_34 __func::__clone(__base*)", t_6E19C0);

// ScenePartyRequest::openPopUpJoin $_34: __func::destroy
static void t_6E19E0(uint32_t self) {
}
RW_FUNCTION(0x6E19E0, "ScenePartyRequest::openPopUpJoin $_34 __func::destroy", t_6E19E0);

// ScenePartyRequest::openPopUpJoin $_34: __func::destroy_deallocate
static void t_6E19E2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E19E2, "ScenePartyRequest::openPopUpJoin $_34 __func::destroy_deallocate", t_6E19E2);

// ScenePartyRequest::openPopUpJoin $_34: __func::target
static uint32_t t_6E1BC4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83750u ? self + 4 : 0; }
RW_FUNCTION(0x6E1BC4, "ScenePartyRequest::openPopUpJoin $_34 __func::target", t_6E1BC4);

// ScenePartyRequest::openPopUpJoin $_34: __func::target_type
static uint32_t t_6E1BE4() { return 0xfa12f8u; }
RW_FUNCTION(0x6E1BE4, "ScenePartyRequest::openPopUpJoin $_34 __func::target_type", t_6E1BE4);

// ScenePartyRequest::openPopUpRank $_33: __func::~__func (deleting)
static void t_6E1BF8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1BF8, "ScenePartyRequest::openPopUpRank $_33 __func::~__func (deleting)", t_6E1BF8);

// ScenePartyRequest::openPopUpRank $_33: __func::__clone()
static uint32_t t_6E1C00(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6885cu);
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
RW_FUNCTION(0x6E1C00, "ScenePartyRequest::openPopUpRank $_33 __func::__clone()", t_6E1C00);

// ScenePartyRequest::openPopUpRank $_33: __func::__clone(__base*)
static void t_6E1C3C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6885cu);
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
RW_FUNCTION(0x6E1C3C, "ScenePartyRequest::openPopUpRank $_33 __func::__clone(__base*)", t_6E1C3C);

// ScenePartyRequest::openPopUpRank $_33: __func::destroy
static void t_6E1C6C(uint32_t self) {
}
RW_FUNCTION(0x6E1C6C, "ScenePartyRequest::openPopUpRank $_33 __func::destroy", t_6E1C6C);

// ScenePartyRequest::openPopUpRank $_33: __func::destroy_deallocate
static void t_6E1C6E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1C6E, "ScenePartyRequest::openPopUpRank $_33 __func::destroy_deallocate", t_6E1C6E);

// ScenePartyRequest::openPopUpRank $_33: __func::target
static uint32_t t_6E1C90(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83810u ? self + 4 : 0; }
RW_FUNCTION(0x6E1C90, "ScenePartyRequest::openPopUpRank $_33 __func::target", t_6E1C90);

// ScenePartyRequest::openPopUpRank $_33: __func::target_type
static uint32_t t_6E1CB0() { return 0xfa130cu; }
RW_FUNCTION(0x6E1CB0, "ScenePartyRequest::openPopUpRank $_33 __func::target_type", t_6E1CB0);

// ScenePartyRequest::openPopUpRank $_32: __func::~__func (deleting)
static void t_6E1CC4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1CC4, "ScenePartyRequest::openPopUpRank $_32 __func::~__func (deleting)", t_6E1CC4);

// ScenePartyRequest::openPopUpRank $_32: __func::__clone()
static uint32_t t_6E1CCC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf68888u);
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
RW_FUNCTION(0x6E1CCC, "ScenePartyRequest::openPopUpRank $_32 __func::__clone()", t_6E1CCC);

// ScenePartyRequest::openPopUpRank $_32: __func::__clone(__base*)
static void t_6E1D08(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68888u);
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
RW_FUNCTION(0x6E1D08, "ScenePartyRequest::openPopUpRank $_32 __func::__clone(__base*)", t_6E1D08);

// ScenePartyRequest::openPopUpRank $_32: __func::destroy
static void t_6E1D38(uint32_t self) {
}
RW_FUNCTION(0x6E1D38, "ScenePartyRequest::openPopUpRank $_32 __func::destroy", t_6E1D38);

// ScenePartyRequest::openPopUpRank $_32: __func::destroy_deallocate
static void t_6E1D3A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1D3A, "ScenePartyRequest::openPopUpRank $_32 __func::destroy_deallocate", t_6E1D3A);

// ScenePartyRequest::openPopUpRank $_32: __func::target
static uint32_t t_6E1D5C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe838d0u ? self + 4 : 0; }
RW_FUNCTION(0x6E1D5C, "ScenePartyRequest::openPopUpRank $_32 __func::target", t_6E1D5C);

// ScenePartyRequest::openPopUpRank $_32: __func::target_type
static uint32_t t_6E1D7C() { return 0xfa1320u; }
RW_FUNCTION(0x6E1D7C, "ScenePartyRequest::openPopUpRank $_32 __func::target_type", t_6E1D7C);

// ScenePartyRequest::openPopUpRank $_31: __func::~__func (deleting)
static void t_6E1D90(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1D90, "ScenePartyRequest::openPopUpRank $_31 __func::~__func (deleting)", t_6E1D90);

// ScenePartyRequest::openPopUpRank $_31: __func::__clone()
static uint32_t t_6E1D98(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf688b4u);
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
RW_FUNCTION(0x6E1D98, "ScenePartyRequest::openPopUpRank $_31 __func::__clone()", t_6E1D98);

// ScenePartyRequest::openPopUpRank $_31: __func::__clone(__base*)
static void t_6E1DD4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf688b4u);
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
RW_FUNCTION(0x6E1DD4, "ScenePartyRequest::openPopUpRank $_31 __func::__clone(__base*)", t_6E1DD4);

// ScenePartyRequest::openPopUpRank $_31: __func::destroy
static void t_6E1E04(uint32_t self) {
}
RW_FUNCTION(0x6E1E04, "ScenePartyRequest::openPopUpRank $_31 __func::destroy", t_6E1E04);

// ScenePartyRequest::openPopUpRank $_31: __func::destroy_deallocate
static void t_6E1E06(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1E06, "ScenePartyRequest::openPopUpRank $_31 __func::destroy_deallocate", t_6E1E06);

// ScenePartyRequest::openPopUpRank $_31: __func::target
static uint32_t t_6E1E28(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83990u ? self + 4 : 0; }
RW_FUNCTION(0x6E1E28, "ScenePartyRequest::openPopUpRank $_31 __func::target", t_6E1E28);

// ScenePartyRequest::openPopUpRank $_31: __func::target_type
static uint32_t t_6E1E48() { return 0xfa1334u; }
RW_FUNCTION(0x6E1E48, "ScenePartyRequest::openPopUpRank $_31 __func::target_type", t_6E1E48);

// ScenePartyRequest::openPopUpRank $_30: __func::~__func (deleting)
static void t_6E1E5C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1E5C, "ScenePartyRequest::openPopUpRank $_30 __func::~__func (deleting)", t_6E1E5C);

// ScenePartyRequest::openPopUpRank $_30: __func::__clone()
static uint32_t t_6E1E64(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf688e0u);
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
RW_FUNCTION(0x6E1E64, "ScenePartyRequest::openPopUpRank $_30 __func::__clone()", t_6E1E64);

// ScenePartyRequest::openPopUpRank $_30: __func::__clone(__base*)
static void t_6E1EA0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf688e0u);
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
RW_FUNCTION(0x6E1EA0, "ScenePartyRequest::openPopUpRank $_30 __func::__clone(__base*)", t_6E1EA0);

// ScenePartyRequest::openPopUpRank $_30: __func::destroy
static void t_6E1ED0(uint32_t self) {
}
RW_FUNCTION(0x6E1ED0, "ScenePartyRequest::openPopUpRank $_30 __func::destroy", t_6E1ED0);

// ScenePartyRequest::openPopUpRank $_30: __func::destroy_deallocate
static void t_6E1ED2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1ED2, "ScenePartyRequest::openPopUpRank $_30 __func::destroy_deallocate", t_6E1ED2);

// ScenePartyRequest::openPopUpRank $_30: __func::target
static uint32_t t_6E1EF4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83a50u ? self + 4 : 0; }
RW_FUNCTION(0x6E1EF4, "ScenePartyRequest::openPopUpRank $_30 __func::target", t_6E1EF4);

// ScenePartyRequest::openPopUpRank $_30: __func::target_type
static uint32_t t_6E1F14() { return 0xfa1348u; }
RW_FUNCTION(0x6E1F14, "ScenePartyRequest::openPopUpRank $_30 __func::target_type", t_6E1F14);

// ScenePartyRequest::openPopUpRank $_29: __func::~__func (deleting)
static void t_6E1F28(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1F28, "ScenePartyRequest::openPopUpRank $_29 __func::~__func (deleting)", t_6E1F28);

// ScenePartyRequest::openPopUpRank $_29: __func::__clone()
static uint32_t t_6E1F30(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6890cu);
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
RW_FUNCTION(0x6E1F30, "ScenePartyRequest::openPopUpRank $_29 __func::__clone()", t_6E1F30);

// ScenePartyRequest::openPopUpRank $_29: __func::__clone(__base*)
static void t_6E1F6C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6890cu);
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
RW_FUNCTION(0x6E1F6C, "ScenePartyRequest::openPopUpRank $_29 __func::__clone(__base*)", t_6E1F6C);

// ScenePartyRequest::openPopUpRank $_29: __func::destroy
static void t_6E1F9C(uint32_t self) {
}
RW_FUNCTION(0x6E1F9C, "ScenePartyRequest::openPopUpRank $_29 __func::destroy", t_6E1F9C);

// ScenePartyRequest::openPopUpRank $_29: __func::destroy_deallocate
static void t_6E1F9E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1F9E, "ScenePartyRequest::openPopUpRank $_29 __func::destroy_deallocate", t_6E1F9E);

// ScenePartyRequest::openPopUpRank $_29: __func::target
static uint32_t t_6E1FC0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83b10u ? self + 4 : 0; }
RW_FUNCTION(0x6E1FC0, "ScenePartyRequest::openPopUpRank $_29 __func::target", t_6E1FC0);

// ScenePartyRequest::openPopUpRank $_29: __func::target_type
static uint32_t t_6E1FE0() { return 0xfa135cu; }
RW_FUNCTION(0x6E1FE0, "ScenePartyRequest::openPopUpRank $_29 __func::target_type", t_6E1FE0);

// ScenePartyRequest::openPopUpRank $_28: __func::~__func (deleting)
static void t_6E1FF4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E1FF4, "ScenePartyRequest::openPopUpRank $_28 __func::~__func (deleting)", t_6E1FF4);

// ScenePartyRequest::openPopUpRank $_28: __func::__clone()
static uint32_t t_6E1FFC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf68938u);
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
RW_FUNCTION(0x6E1FFC, "ScenePartyRequest::openPopUpRank $_28 __func::__clone()", t_6E1FFC);

// ScenePartyRequest::openPopUpRank $_28: __func::__clone(__base*)
static void t_6E2038(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68938u);
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
RW_FUNCTION(0x6E2038, "ScenePartyRequest::openPopUpRank $_28 __func::__clone(__base*)", t_6E2038);

// ScenePartyRequest::openPopUpRank $_28: __func::destroy
static void t_6E2068(uint32_t self) {
}
RW_FUNCTION(0x6E2068, "ScenePartyRequest::openPopUpRank $_28 __func::destroy", t_6E2068);

// ScenePartyRequest::openPopUpRank $_28: __func::destroy_deallocate
static void t_6E206A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E206A, "ScenePartyRequest::openPopUpRank $_28 __func::destroy_deallocate", t_6E206A);

// ScenePartyRequest::openPopUpRank $_28: __func::target
static uint32_t t_6E208C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83bd0u ? self + 4 : 0; }
RW_FUNCTION(0x6E208C, "ScenePartyRequest::openPopUpRank $_28 __func::target", t_6E208C);

// ScenePartyRequest::openPopUpRank $_28: __func::target_type
static uint32_t t_6E20AC() { return 0xfa1370u; }
RW_FUNCTION(0x6E20AC, "ScenePartyRequest::openPopUpRank $_28 __func::target_type", t_6E20AC);

// ScenePartyRequest::openPopUpRank $_26: __func::~__func (deleting)
static void t_6E20C0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E20C0, "ScenePartyRequest::openPopUpRank $_26 __func::~__func (deleting)", t_6E20C0);

// ScenePartyRequest::openPopUpRank $_26: __func::__clone()
static uint32_t t_6E20C8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68964u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E20C8, "ScenePartyRequest::openPopUpRank $_26 __func::__clone()", t_6E20C8);

// ScenePartyRequest::openPopUpRank $_26: __func::__clone(__base*)
static void t_6E20F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68964u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E20F4, "ScenePartyRequest::openPopUpRank $_26 __func::__clone(__base*)", t_6E20F4);

// ScenePartyRequest::openPopUpRank $_26: __func::destroy
static void t_6E2114(uint32_t self) {
}
RW_FUNCTION(0x6E2114, "ScenePartyRequest::openPopUpRank $_26 __func::destroy", t_6E2114);

// ScenePartyRequest::openPopUpRank $_26: __func::destroy_deallocate
static void t_6E2116(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E2116, "ScenePartyRequest::openPopUpRank $_26 __func::destroy_deallocate", t_6E2116);

// ScenePartyRequest::openPopUpRank $_26: __func::target
static uint32_t t_6E2348(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83c70u ? self + 4 : 0; }
RW_FUNCTION(0x6E2348, "ScenePartyRequest::openPopUpRank $_26 __func::target", t_6E2348);

// ScenePartyRequest::openPopUpRank $_26: __func::target_type
static uint32_t t_6E2368() { return 0xfa1384u; }
RW_FUNCTION(0x6E2368, "ScenePartyRequest::openPopUpRank $_26 __func::target_type", t_6E2368);

// ScenePartyRequest::openPopUpPlayStyle $_25: __func::~__func (deleting)
static void t_6E237C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E237C, "ScenePartyRequest::openPopUpPlayStyle $_25 __func::~__func (deleting)", t_6E237C);

// ScenePartyRequest::openPopUpPlayStyle $_25: __func::__clone()
static uint32_t t_6E2384(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf68990u);
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
RW_FUNCTION(0x6E2384, "ScenePartyRequest::openPopUpPlayStyle $_25 __func::__clone()", t_6E2384);

// ScenePartyRequest::openPopUpPlayStyle $_25: __func::__clone(__base*)
static void t_6E23B8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68990u);
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
RW_FUNCTION(0x6E23B8, "ScenePartyRequest::openPopUpPlayStyle $_25 __func::__clone(__base*)", t_6E23B8);

// ScenePartyRequest::openPopUpPlayStyle $_25: __func::destroy
static void t_6E23E0(uint32_t self) {
}
RW_FUNCTION(0x6E23E0, "ScenePartyRequest::openPopUpPlayStyle $_25 __func::destroy", t_6E23E0);

// ScenePartyRequest::openPopUpPlayStyle $_25: __func::destroy_deallocate
static void t_6E23E2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E23E2, "ScenePartyRequest::openPopUpPlayStyle $_25 __func::destroy_deallocate", t_6E23E2);

// ScenePartyRequest::openPopUpPlayStyle $_25: __func::target
static uint32_t t_6E2404(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83d40u ? self + 4 : 0; }
RW_FUNCTION(0x6E2404, "ScenePartyRequest::openPopUpPlayStyle $_25 __func::target", t_6E2404);

// ScenePartyRequest::openPopUpPlayStyle $_25: __func::target_type
static uint32_t t_6E2424() { return 0xfa1398u; }
RW_FUNCTION(0x6E2424, "ScenePartyRequest::openPopUpPlayStyle $_25 __func::target_type", t_6E2424);

// ScenePartyRequest::openPopUpPlayStyle $_24: __func::~__func (deleting)
static void t_6E2438(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E2438, "ScenePartyRequest::openPopUpPlayStyle $_24 __func::~__func (deleting)", t_6E2438);

// ScenePartyRequest::openPopUpPlayStyle $_24: __func::__clone()
static uint32_t t_6E2440(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf689bcu);
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
RW_FUNCTION(0x6E2440, "ScenePartyRequest::openPopUpPlayStyle $_24 __func::__clone()", t_6E2440);

// ScenePartyRequest::openPopUpPlayStyle $_24: __func::__clone(__base*)
static void t_6E2474(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf689bcu);
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
RW_FUNCTION(0x6E2474, "ScenePartyRequest::openPopUpPlayStyle $_24 __func::__clone(__base*)", t_6E2474);

// ScenePartyRequest::openPopUpPlayStyle $_24: __func::destroy
static void t_6E249C(uint32_t self) {
}
RW_FUNCTION(0x6E249C, "ScenePartyRequest::openPopUpPlayStyle $_24 __func::destroy", t_6E249C);

// ScenePartyRequest::openPopUpPlayStyle $_24: __func::destroy_deallocate
static void t_6E249E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E249E, "ScenePartyRequest::openPopUpPlayStyle $_24 __func::destroy_deallocate", t_6E249E);

// ScenePartyRequest::openPopUpPlayStyle $_24: __func::target
static uint32_t t_6E24C0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83e20u ? self + 4 : 0; }
RW_FUNCTION(0x6E24C0, "ScenePartyRequest::openPopUpPlayStyle $_24 __func::target", t_6E24C0);

// ScenePartyRequest::openPopUpPlayStyle $_24: __func::target_type
static uint32_t t_6E24E0() { return 0xfa13acu; }
RW_FUNCTION(0x6E24E0, "ScenePartyRequest::openPopUpPlayStyle $_24 __func::target_type", t_6E24E0);

// ScenePartyRequest::openPopUpPlayStyle $_23: __func::~__func (deleting)
static void t_6E24F4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E24F4, "ScenePartyRequest::openPopUpPlayStyle $_23 __func::~__func (deleting)", t_6E24F4);

// ScenePartyRequest::openPopUpPlayStyle $_23: __func::__clone()
static uint32_t t_6E24FC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf689e8u);
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
RW_FUNCTION(0x6E24FC, "ScenePartyRequest::openPopUpPlayStyle $_23 __func::__clone()", t_6E24FC);

// ScenePartyRequest::openPopUpPlayStyle $_23: __func::__clone(__base*)
static void t_6E2530(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf689e8u);
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
RW_FUNCTION(0x6E2530, "ScenePartyRequest::openPopUpPlayStyle $_23 __func::__clone(__base*)", t_6E2530);

// ScenePartyRequest::openPopUpPlayStyle $_23: __func::destroy
static void t_6E2558(uint32_t self) {
}
RW_FUNCTION(0x6E2558, "ScenePartyRequest::openPopUpPlayStyle $_23 __func::destroy", t_6E2558);

// ScenePartyRequest::openPopUpPlayStyle $_23: __func::destroy_deallocate
static void t_6E255A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E255A, "ScenePartyRequest::openPopUpPlayStyle $_23 __func::destroy_deallocate", t_6E255A);

// ScenePartyRequest::openPopUpPlayStyle $_23: __func::target
static uint32_t t_6E257C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83f00u ? self + 4 : 0; }
RW_FUNCTION(0x6E257C, "ScenePartyRequest::openPopUpPlayStyle $_23 __func::target", t_6E257C);

// ScenePartyRequest::openPopUpPlayStyle $_23: __func::target_type
static uint32_t t_6E259C() { return 0xfa13c0u; }
RW_FUNCTION(0x6E259C, "ScenePartyRequest::openPopUpPlayStyle $_23 __func::target_type", t_6E259C);

// ScenePartyRequest::openPopUpPlayStyle $_22: __func::~__func (deleting)
static void t_6E25B0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E25B0, "ScenePartyRequest::openPopUpPlayStyle $_22 __func::~__func (deleting)", t_6E25B0);

// ScenePartyRequest::openPopUpPlayStyle $_22: __func::__clone()
static uint32_t t_6E25B8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf68a14u);
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
RW_FUNCTION(0x6E25B8, "ScenePartyRequest::openPopUpPlayStyle $_22 __func::__clone()", t_6E25B8);

// ScenePartyRequest::openPopUpPlayStyle $_22: __func::__clone(__base*)
static void t_6E25EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68a14u);
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
RW_FUNCTION(0x6E25EC, "ScenePartyRequest::openPopUpPlayStyle $_22 __func::__clone(__base*)", t_6E25EC);

// ScenePartyRequest::openPopUpPlayStyle $_22: __func::destroy
static void t_6E2614(uint32_t self) {
}
RW_FUNCTION(0x6E2614, "ScenePartyRequest::openPopUpPlayStyle $_22 __func::destroy", t_6E2614);

// ScenePartyRequest::openPopUpPlayStyle $_22: __func::destroy_deallocate
static void t_6E2616(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E2616, "ScenePartyRequest::openPopUpPlayStyle $_22 __func::destroy_deallocate", t_6E2616);

// ScenePartyRequest::openPopUpPlayStyle $_22: __func::target
static uint32_t t_6E2638(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe83fe0u ? self + 4 : 0; }
RW_FUNCTION(0x6E2638, "ScenePartyRequest::openPopUpPlayStyle $_22 __func::target", t_6E2638);

// ScenePartyRequest::openPopUpPlayStyle $_22: __func::target_type
static uint32_t t_6E2658() { return 0xfa13d4u; }
RW_FUNCTION(0x6E2658, "ScenePartyRequest::openPopUpPlayStyle $_22 __func::target_type", t_6E2658);

// ScenePartyRequest::openPopUpPlayStyle $_20: __func::~__func (deleting)
static void t_6E266C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E266C, "ScenePartyRequest::openPopUpPlayStyle $_20 __func::~__func (deleting)", t_6E266C);

// ScenePartyRequest::openPopUpPlayStyle $_20: __func::__clone()
static uint32_t t_6E2674(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68a40u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E2674, "ScenePartyRequest::openPopUpPlayStyle $_20 __func::__clone()", t_6E2674);

// ScenePartyRequest::openPopUpPlayStyle $_20: __func::__clone(__base*)
static void t_6E26A0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68a40u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E26A0, "ScenePartyRequest::openPopUpPlayStyle $_20 __func::__clone(__base*)", t_6E26A0);

// ScenePartyRequest::openPopUpPlayStyle $_20: __func::destroy
static void t_6E26C0(uint32_t self) {
}
RW_FUNCTION(0x6E26C0, "ScenePartyRequest::openPopUpPlayStyle $_20 __func::destroy", t_6E26C0);

// ScenePartyRequest::openPopUpPlayStyle $_20: __func::destroy_deallocate
static void t_6E26C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E26C2, "ScenePartyRequest::openPopUpPlayStyle $_20 __func::destroy_deallocate", t_6E26C2);

// ScenePartyRequest::openPopUpPlayStyle $_20: __func::target
static uint32_t t_6E28B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84090u ? self + 4 : 0; }
RW_FUNCTION(0x6E28B8, "ScenePartyRequest::openPopUpPlayStyle $_20 __func::target", t_6E28B8);

// ScenePartyRequest::openPopUpPlayStyle $_20: __func::target_type
static uint32_t t_6E28D8() { return 0xfa13e8u; }
RW_FUNCTION(0x6E28D8, "ScenePartyRequest::openPopUpPlayStyle $_20 __func::target_type", t_6E28D8);

// ScenePartyRequest::runInEntryLayout $_19: __func::~__func (deleting)
static void t_6E28EC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E28EC, "ScenePartyRequest::runInEntryLayout $_19 __func::~__func (deleting)", t_6E28EC);

// ScenePartyRequest::runInEntryLayout $_19: __func::__clone()
static uint32_t t_6E28F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68a6cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E28F4, "ScenePartyRequest::runInEntryLayout $_19 __func::__clone()", t_6E28F4);

// ScenePartyRequest::runInEntryLayout $_19: __func::__clone(__base*)
static void t_6E2920(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68a6cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E2920, "ScenePartyRequest::runInEntryLayout $_19 __func::__clone(__base*)", t_6E2920);

// ScenePartyRequest::runInEntryLayout $_19: __func::destroy
static void t_6E2940(uint32_t self) {
}
RW_FUNCTION(0x6E2940, "ScenePartyRequest::runInEntryLayout $_19 __func::destroy", t_6E2940);

// ScenePartyRequest::runInEntryLayout $_19: __func::destroy_deallocate
static void t_6E2942(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E2942, "ScenePartyRequest::runInEntryLayout $_19 __func::destroy_deallocate", t_6E2942);

// ScenePartyRequest::runInEntryLayout $_19: __func::target
static uint32_t t_6E2A44(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84140u ? self + 4 : 0; }
RW_FUNCTION(0x6E2A44, "ScenePartyRequest::runInEntryLayout $_19 __func::target", t_6E2A44);

// ScenePartyRequest::runInEntryLayout $_19: __func::target_type
static uint32_t t_6E2A64() { return 0xfa13fcu; }
RW_FUNCTION(0x6E2A64, "ScenePartyRequest::runInEntryLayout $_19 __func::target_type", t_6E2A64);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_: __func::~__func (deleting)
static void t_6E2A78(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E2A78, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ __func::~__func (deleting)", t_6E2A78);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_: __func::__clone()
static uint32_t t_6E2A80(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf68a98u);
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
RW_FUNCTION(0x6E2A80, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ __func::__clone()", t_6E2A80);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_: __func::__clone(__base*)
static void t_6E2AB4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68a98u);
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
RW_FUNCTION(0x6E2AB4, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ __func::__clone(__base*)", t_6E2AB4);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_: __func::destroy
static void t_6E2AE0(uint32_t self) {
}
RW_FUNCTION(0x6E2AE0, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ __func::destroy", t_6E2AE0);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_: __func::destroy_deallocate
static void t_6E2AE2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E2AE2, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ __func::destroy_deallocate", t_6E2AE2);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_: __func::target
static uint32_t t_6E2F2C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe841f0u ? self + 8 : 0; }
RW_FUNCTION(0x6E2F2C, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ __func::target", t_6E2F2C);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_: __func::target_type
static uint32_t t_6E2F50() { return 0xfa1410u; }
RW_FUNCTION(0x6E2F50, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ __func::target_type", t_6E2F50);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_: __func::target
static uint32_t t_6E34D0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe842b0u ? self + 8 : 0; }
RW_FUNCTION(0x6E34D0, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ __func::target", t_6E34D0);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_: __func::target_type
static uint32_t t_6E34F4() { return 0xfa1424u; }
RW_FUNCTION(0x6E34F4, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ __func::target_type", t_6E34F4);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_: __func::~__func (deleting)
static void t_6E3508(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3508, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_ __func::~__func (deleting)", t_6E3508);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_: __func::__clone()
static uint32_t t_6E3510(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68af0u);
    }
    return v1;
}
RW_FUNCTION(0x6E3510, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_ __func::__clone()", t_6E3510);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_: __func::__clone(__base*)
static void t_6E3534(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68af0u);
    }
}
RW_FUNCTION(0x6E3534, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_ __func::__clone(__base*)", t_6E3534);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_: __func::destroy
static void t_6E3550(uint32_t self) {
}
RW_FUNCTION(0x6E3550, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_ __func::destroy", t_6E3550);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_: __func::destroy_deallocate
static void t_6E3552(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3552, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_ __func::destroy_deallocate", t_6E3552);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_: __func::target
static uint32_t t_6E355C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84390u ? self + 4 : 0; }
RW_FUNCTION(0x6E355C, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_ __func::target", t_6E355C);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_: __func::target_type
static uint32_t t_6E357C() { return 0xfa1438u; }
RW_FUNCTION(0x6E357C, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE1_ __func::target_type", t_6E357C);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_: __func::target
static uint32_t t_6E38E4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84480u ? self + 8 : 0; }
RW_FUNCTION(0x6E38E4, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ __func::target", t_6E38E4);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_: __func::target_type
static uint32_t t_6E3908() { return 0xfa144cu; }
RW_FUNCTION(0x6E3908, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ __func::target_type", t_6E3908);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_: __func::~__func (deleting)
static void t_6E391C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E391C, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_ __func::~__func (deleting)", t_6E391C);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_: __func::__clone()
static uint32_t t_6E3924(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68b48u);
    }
    return v1;
}
RW_FUNCTION(0x6E3924, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_ __func::__clone()", t_6E3924);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_: __func::__clone(__base*)
static void t_6E3948(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68b48u);
    }
}
RW_FUNCTION(0x6E3948, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_ __func::__clone(__base*)", t_6E3948);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_: __func::destroy
static void t_6E3964(uint32_t self) {
}
RW_FUNCTION(0x6E3964, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_ __func::destroy", t_6E3964);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_: __func::destroy_deallocate
static void t_6E3966(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3966, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_ __func::destroy_deallocate", t_6E3966);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_: __func::target
static uint32_t t_6E3A90(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84580u ? self + 4 : 0; }
RW_FUNCTION(0x6E3A90, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_ __func::target", t_6E3A90);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_: __func::target_type
static uint32_t t_6E3AB0() { return 0xfa1460u; }
RW_FUNCTION(0x6E3AB0, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE0_ UlvE_ __func::target_type", t_6E3AB0);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6E3AC4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3AC4, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6E3AC4);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6E3ACC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68b74u);
    }
    return v1;
}
RW_FUNCTION(0x6E3ACC, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6E3ACC);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6E3AF0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68b74u);
    }
}
RW_FUNCTION(0x6E3AF0, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6E3AF0);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6E3B0C(uint32_t self) {
}
RW_FUNCTION(0x6E3B0C, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_ __func::destroy", t_6E3B0C);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6E3B0E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3B0E, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6E3B0E);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6E3B18(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84680u ? self + 4 : 0; }
RW_FUNCTION(0x6E3B18, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_ __func::target", t_6E3B18);

// ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6E3B38() { return 0xfa1474u; }
RW_FUNCTION(0x6E3B38, "ScenePartyRequest::runInEntryLayout $_19 UlvE_ UlbE_ UlbE_ __func::target_type", t_6E3B38);

// ScenePartyRequest::runInJoinLayout $_18: __func::~__func (deleting)
static void t_6E3B4C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3B4C, "ScenePartyRequest::runInJoinLayout $_18 __func::~__func (deleting)", t_6E3B4C);

// ScenePartyRequest::runInJoinLayout $_18: __func::__clone()
static uint32_t t_6E3B54(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68ba0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E3B54, "ScenePartyRequest::runInJoinLayout $_18 __func::__clone()", t_6E3B54);

// ScenePartyRequest::runInJoinLayout $_18: __func::__clone(__base*)
static void t_6E3B80(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68ba0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E3B80, "ScenePartyRequest::runInJoinLayout $_18 __func::__clone(__base*)", t_6E3B80);

// ScenePartyRequest::runInJoinLayout $_18: __func::destroy
static void t_6E3BA0(uint32_t self) {
}
RW_FUNCTION(0x6E3BA0, "ScenePartyRequest::runInJoinLayout $_18 __func::destroy", t_6E3BA0);

// ScenePartyRequest::runInJoinLayout $_18: __func::destroy_deallocate
static void t_6E3BA2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3BA2, "ScenePartyRequest::runInJoinLayout $_18 __func::destroy_deallocate", t_6E3BA2);

// ScenePartyRequest::runInJoinLayout $_18: __func::target
static uint32_t t_6E3CA4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84750u ? self + 4 : 0; }
RW_FUNCTION(0x6E3CA4, "ScenePartyRequest::runInJoinLayout $_18 __func::target", t_6E3CA4);

// ScenePartyRequest::runInJoinLayout $_18: __func::target_type
static uint32_t t_6E3CC4() { return 0xfa1488u; }
RW_FUNCTION(0x6E3CC4, "ScenePartyRequest::runInJoinLayout $_18 __func::target_type", t_6E3CC4);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_: __func::~__func (deleting)
static void t_6E3CD8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3CD8, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ __func::~__func (deleting)", t_6E3CD8);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_: __func::__clone()
static uint32_t t_6E3CE0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf68bccu);
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
RW_FUNCTION(0x6E3CE0, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ __func::__clone()", t_6E3CE0);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_: __func::__clone(__base*)
static void t_6E3D14(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68bccu);
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
RW_FUNCTION(0x6E3D14, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ __func::__clone(__base*)", t_6E3D14);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_: __func::destroy
static void t_6E3D40(uint32_t self) {
}
RW_FUNCTION(0x6E3D40, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ __func::destroy", t_6E3D40);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_: __func::destroy_deallocate
static void t_6E3D42(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3D42, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ __func::destroy_deallocate", t_6E3D42);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_: __func::target
static uint32_t t_6E3E58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe847f0u ? self + 8 : 0; }
RW_FUNCTION(0x6E3E58, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ __func::target", t_6E3E58);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_: __func::target_type
static uint32_t t_6E3E7C() { return 0xfa149cu; }
RW_FUNCTION(0x6E3E7C, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ __func::target_type", t_6E3E7C);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_: __func::~__func (deleting)
static void t_6E3E90(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3E90, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ __func::~__func (deleting)", t_6E3E90);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_: __func::__clone()
static uint32_t t_6E3E98(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf68bf8u);
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
RW_FUNCTION(0x6E3E98, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ __func::__clone()", t_6E3E98);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_: __func::__clone(__base*)
static void t_6E3ECC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68bf8u);
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
RW_FUNCTION(0x6E3ECC, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ __func::__clone(__base*)", t_6E3ECC);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_: __func::destroy
static void t_6E3EF8(uint32_t self) {
}
RW_FUNCTION(0x6E3EF8, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ __func::destroy", t_6E3EF8);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_: __func::destroy_deallocate
static void t_6E3EFA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E3EFA, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ __func::destroy_deallocate", t_6E3EFA);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_: __func::target
static uint32_t t_6E40E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe848b0u ? self + 8 : 0; }
RW_FUNCTION(0x6E40E8, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ __func::target", t_6E40E8);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_: __func::target_type
static uint32_t t_6E410C() { return 0xfa14b0u; }
RW_FUNCTION(0x6E410C, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ __func::target_type", t_6E410C);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_: __func::~__func (deleting)
static void t_6E4120(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4120, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_ __func::~__func (deleting)", t_6E4120);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_: __func::__clone()
static uint32_t t_6E4128(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68c24u);
    }
    return v1;
}
RW_FUNCTION(0x6E4128, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_ __func::__clone()", t_6E4128);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_: __func::__clone(__base*)
static void t_6E414C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68c24u);
    }
}
RW_FUNCTION(0x6E414C, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_ __func::__clone(__base*)", t_6E414C);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_: __func::destroy
static void t_6E4168(uint32_t self) {
}
RW_FUNCTION(0x6E4168, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_ __func::destroy", t_6E4168);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_: __func::destroy_deallocate
static void t_6E416A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E416A, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_ __func::destroy_deallocate", t_6E416A);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_: __func::target
static uint32_t t_6E4174(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84990u ? self + 4 : 0; }
RW_FUNCTION(0x6E4174, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_ __func::target", t_6E4174);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_: __func::target_type
static uint32_t t_6E4194() { return 0xfa14c4u; }
RW_FUNCTION(0x6E4194, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE1_ __func::target_type", t_6E4194);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_: __func::~__func (deleting)
static void t_6E41A8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E41A8, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_ __func::~__func (deleting)", t_6E41A8);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_: __func::__clone()
static uint32_t t_6E41B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf68c50u);
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
RW_FUNCTION(0x6E41B0, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_ __func::__clone()", t_6E41B0);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_: __func::__clone(__base*)
static void t_6E41E4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68c50u);
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
RW_FUNCTION(0x6E41E4, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_ __func::__clone(__base*)", t_6E41E4);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_: __func::destroy
static void t_6E4210(uint32_t self) {
}
RW_FUNCTION(0x6E4210, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_ __func::destroy", t_6E4210);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_: __func::destroy_deallocate
static void t_6E4212(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4212, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_ __func::destroy_deallocate", t_6E4212);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_: __func::target
static uint32_t t_6E4304(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84a80u ? self + 8 : 0; }
RW_FUNCTION(0x6E4304, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_ __func::target", t_6E4304);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_: __func::target_type
static uint32_t t_6E4328() { return 0xfa14d8u; }
RW_FUNCTION(0x6E4328, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE0_ __func::target_type", t_6E4328);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6E433C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E433C, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6E433C);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6E4344(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68c7cu);
    }
    return v1;
}
RW_FUNCTION(0x6E4344, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_ __func::__clone()", t_6E4344);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_6E4368(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68c7cu);
    }
}
RW_FUNCTION(0x6E4368, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_ __func::__clone(__base*)", t_6E4368);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_: __func::destroy
static void t_6E4384(uint32_t self) {
}
RW_FUNCTION(0x6E4384, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_ __func::destroy", t_6E4384);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_6E4386(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4386, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_ __func::destroy_deallocate", t_6E4386);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_: __func::target
static uint32_t t_6E4390(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84b70u ? self + 4 : 0; }
RW_FUNCTION(0x6E4390, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_ __func::target", t_6E4390);

// ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_6E43B0() { return 0xfa14ecu; }
RW_FUNCTION(0x6E43B0, "ScenePartyRequest::runInJoinLayout $_18 UlvE_ UlbE_ UlbE_ __func::target_type", t_6E43B0);

// ScenePartyRequest::runInJoinLayout $_17: __func::~__func (deleting)
static void t_6E43C4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E43C4, "ScenePartyRequest::runInJoinLayout $_17 __func::~__func (deleting)", t_6E43C4);

// ScenePartyRequest::runInJoinLayout $_17: __func::__clone()
static uint32_t t_6E43CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68ca8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E43CC, "ScenePartyRequest::runInJoinLayout $_17 __func::__clone()", t_6E43CC);

// ScenePartyRequest::runInJoinLayout $_17: __func::__clone(__base*)
static void t_6E43F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68ca8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E43F8, "ScenePartyRequest::runInJoinLayout $_17 __func::__clone(__base*)", t_6E43F8);

// ScenePartyRequest::runInJoinLayout $_17: __func::destroy
static void t_6E4418(uint32_t self) {
}
RW_FUNCTION(0x6E4418, "ScenePartyRequest::runInJoinLayout $_17 __func::destroy", t_6E4418);

// ScenePartyRequest::runInJoinLayout $_17: __func::destroy_deallocate
static void t_6E441A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E441A, "ScenePartyRequest::runInJoinLayout $_17 __func::destroy_deallocate", t_6E441A);

// ScenePartyRequest::runInJoinLayout $_17: __func::target
static uint32_t t_6E4518(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84c60u ? self + 4 : 0; }
RW_FUNCTION(0x6E4518, "ScenePartyRequest::runInJoinLayout $_17 __func::target", t_6E4518);

// ScenePartyRequest::runInJoinLayout $_17: __func::target_type
static uint32_t t_6E4538() { return 0xfa1500u; }
RW_FUNCTION(0x6E4538, "ScenePartyRequest::runInJoinLayout $_17 __func::target_type", t_6E4538);

// ScenePartyRequest::runInJoinLayout $_17 UlvE_: __func::~__func (deleting)
static void t_6E454C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E454C, "ScenePartyRequest::runInJoinLayout $_17 UlvE_ __func::~__func (deleting)", t_6E454C);

// ScenePartyRequest::runInJoinLayout $_17 UlvE_: __func::__clone()
static uint32_t t_6E4554(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68cd4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E4554, "ScenePartyRequest::runInJoinLayout $_17 UlvE_ __func::__clone()", t_6E4554);

// ScenePartyRequest::runInJoinLayout $_17 UlvE_: __func::__clone(__base*)
static void t_6E4580(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68cd4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E4580, "ScenePartyRequest::runInJoinLayout $_17 UlvE_ __func::__clone(__base*)", t_6E4580);

// ScenePartyRequest::runInJoinLayout $_17 UlvE_: __func::destroy
static void t_6E45A0(uint32_t self) {
}
RW_FUNCTION(0x6E45A0, "ScenePartyRequest::runInJoinLayout $_17 UlvE_ __func::destroy", t_6E45A0);

// ScenePartyRequest::runInJoinLayout $_17 UlvE_: __func::destroy_deallocate
static void t_6E45A2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E45A2, "ScenePartyRequest::runInJoinLayout $_17 UlvE_ __func::destroy_deallocate", t_6E45A2);

// ScenePartyRequest::runInJoinLayout $_17 UlvE_: __func::target
static uint32_t t_6E45B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84d30u ? self + 4 : 0; }
RW_FUNCTION(0x6E45B4, "ScenePartyRequest::runInJoinLayout $_17 UlvE_ __func::target", t_6E45B4);

// ScenePartyRequest::runInJoinLayout $_17 UlvE_: __func::target_type
static uint32_t t_6E45D4() { return 0xfa1514u; }
RW_FUNCTION(0x6E45D4, "ScenePartyRequest::runInJoinLayout $_17 UlvE_ __func::target_type", t_6E45D4);

// ScenePartyRequest::init $_16: __func::~__func (deleting)
static void t_6E45E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E45E8, "ScenePartyRequest::init $_16 __func::~__func (deleting)", t_6E45E8);

// ScenePartyRequest::init $_16: __func::__clone()
static uint32_t t_6E45F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68d00u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E45F0, "ScenePartyRequest::init $_16 __func::__clone()", t_6E45F0);

// ScenePartyRequest::init $_16: __func::__clone(__base*)
static void t_6E461C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68d00u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E461C, "ScenePartyRequest::init $_16 __func::__clone(__base*)", t_6E461C);

// ScenePartyRequest::init $_16: __func::destroy
static void t_6E463C(uint32_t self) {
}
RW_FUNCTION(0x6E463C, "ScenePartyRequest::init $_16 __func::destroy", t_6E463C);

// ScenePartyRequest::init $_16: __func::destroy_deallocate
static void t_6E463E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E463E, "ScenePartyRequest::init $_16 __func::destroy_deallocate", t_6E463E);

// ScenePartyRequest::init $_16: __func::target
static uint32_t t_6E472C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84e00u ? self + 4 : 0; }
RW_FUNCTION(0x6E472C, "ScenePartyRequest::init $_16 __func::target", t_6E472C);

// ScenePartyRequest::init $_16: __func::target_type
static uint32_t t_6E474C() { return 0xfa1528u; }
RW_FUNCTION(0x6E474C, "ScenePartyRequest::init $_16 __func::target_type", t_6E474C);

// ScenePartyRequest::init $_16 UlvE_: __func::~__func (deleting)
static void t_6E4760(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4760, "ScenePartyRequest::init $_16 UlvE_ __func::~__func (deleting)", t_6E4760);

// ScenePartyRequest::init $_16 UlvE_: __func::__clone()
static uint32_t t_6E4768(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68d2cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E4768, "ScenePartyRequest::init $_16 UlvE_ __func::__clone()", t_6E4768);

// ScenePartyRequest::init $_16 UlvE_: __func::__clone(__base*)
static void t_6E4794(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68d2cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E4794, "ScenePartyRequest::init $_16 UlvE_ __func::__clone(__base*)", t_6E4794);

// ScenePartyRequest::init $_16 UlvE_: __func::destroy
static void t_6E47B4(uint32_t self) {
}
RW_FUNCTION(0x6E47B4, "ScenePartyRequest::init $_16 UlvE_ __func::destroy", t_6E47B4);

// ScenePartyRequest::init $_16 UlvE_: __func::destroy_deallocate
static void t_6E47B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E47B6, "ScenePartyRequest::init $_16 UlvE_ __func::destroy_deallocate", t_6E47B6);

// ScenePartyRequest::init $_16 UlvE_: __func::target
static uint32_t t_6E47C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84ea0u ? self + 4 : 0; }
RW_FUNCTION(0x6E47C8, "ScenePartyRequest::init $_16 UlvE_ __func::target", t_6E47C8);

// ScenePartyRequest::init $_16 UlvE_: __func::target_type
static uint32_t t_6E47E8() { return 0xfa153cu; }
RW_FUNCTION(0x6E47E8, "ScenePartyRequest::init $_16 UlvE_ __func::target_type", t_6E47E8);

// ScenePartyRequest::init $_15: __func::~__func (deleting)
static void t_6E47FC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E47FC, "ScenePartyRequest::init $_15 __func::~__func (deleting)", t_6E47FC);

// ScenePartyRequest::init $_15: __func::__clone()
static uint32_t t_6E4804(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68d58u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E4804, "ScenePartyRequest::init $_15 __func::__clone()", t_6E4804);

// ScenePartyRequest::init $_15: __func::__clone(__base*)
static void t_6E4830(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68d58u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E4830, "ScenePartyRequest::init $_15 __func::__clone(__base*)", t_6E4830);

// ScenePartyRequest::init $_15: __func::destroy
static void t_6E4850(uint32_t self) {
}
RW_FUNCTION(0x6E4850, "ScenePartyRequest::init $_15 __func::destroy", t_6E4850);

// ScenePartyRequest::init $_15: __func::destroy_deallocate
static void t_6E4852(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4852, "ScenePartyRequest::init $_15 __func::destroy_deallocate", t_6E4852);

// ScenePartyRequest::init $_15: __func::target
static uint32_t t_6E4940(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84f30u ? self + 4 : 0; }
RW_FUNCTION(0x6E4940, "ScenePartyRequest::init $_15 __func::target", t_6E4940);

// ScenePartyRequest::init $_15: __func::target_type
static uint32_t t_6E4960() { return 0xfa1550u; }
RW_FUNCTION(0x6E4960, "ScenePartyRequest::init $_15 __func::target_type", t_6E4960);

// ScenePartyRequest::init $_15 UlvE_: __func::~__func (deleting)
static void t_6E4974(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4974, "ScenePartyRequest::init $_15 UlvE_ __func::~__func (deleting)", t_6E4974);

// ScenePartyRequest::init $_15 UlvE_: __func::__clone()
static uint32_t t_6E497C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68d84u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E497C, "ScenePartyRequest::init $_15 UlvE_ __func::__clone()", t_6E497C);

// ScenePartyRequest::init $_15 UlvE_: __func::__clone(__base*)
static void t_6E49A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68d84u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E49A8, "ScenePartyRequest::init $_15 UlvE_ __func::__clone(__base*)", t_6E49A8);

// ScenePartyRequest::init $_15 UlvE_: __func::destroy
static void t_6E49C8(uint32_t self) {
}
RW_FUNCTION(0x6E49C8, "ScenePartyRequest::init $_15 UlvE_ __func::destroy", t_6E49C8);

// ScenePartyRequest::init $_15 UlvE_: __func::destroy_deallocate
static void t_6E49CA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E49CA, "ScenePartyRequest::init $_15 UlvE_ __func::destroy_deallocate", t_6E49CA);

// ScenePartyRequest::init $_15 UlvE_: __func::target
static uint32_t t_6E49DC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe84fd0u ? self + 4 : 0; }
RW_FUNCTION(0x6E49DC, "ScenePartyRequest::init $_15 UlvE_ __func::target", t_6E49DC);

// ScenePartyRequest::init $_15 UlvE_: __func::target_type
static uint32_t t_6E49FC() { return 0xfa1564u; }
RW_FUNCTION(0x6E49FC, "ScenePartyRequest::init $_15 UlvE_ __func::target_type", t_6E49FC);

// ScenePartyRequest::init $_14: __func::~__func (deleting)
static void t_6E4A10(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4A10, "ScenePartyRequest::init $_14 __func::~__func (deleting)", t_6E4A10);

// ScenePartyRequest::init $_14: __func::__clone()
static uint32_t t_6E4A18(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68db0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E4A18, "ScenePartyRequest::init $_14 __func::__clone()", t_6E4A18);

// ScenePartyRequest::init $_14: __func::__clone(__base*)
static void t_6E4A44(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68db0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E4A44, "ScenePartyRequest::init $_14 __func::__clone(__base*)", t_6E4A44);

// ScenePartyRequest::init $_14: __func::destroy
static void t_6E4A64(uint32_t self) {
}
RW_FUNCTION(0x6E4A64, "ScenePartyRequest::init $_14 __func::destroy", t_6E4A64);

// ScenePartyRequest::init $_14: __func::destroy_deallocate
static void t_6E4A66(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4A66, "ScenePartyRequest::init $_14 __func::destroy_deallocate", t_6E4A66);

// ScenePartyRequest::init $_14: __func::target
static uint32_t t_6E4B54(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85060u ? self + 4 : 0; }
RW_FUNCTION(0x6E4B54, "ScenePartyRequest::init $_14 __func::target", t_6E4B54);

// ScenePartyRequest::init $_14: __func::target_type
static uint32_t t_6E4B74() { return 0xfa1578u; }
RW_FUNCTION(0x6E4B74, "ScenePartyRequest::init $_14 __func::target_type", t_6E4B74);

// ScenePartyRequest::init $_14 UlvE_: __func::~__func (deleting)
static void t_6E4B88(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4B88, "ScenePartyRequest::init $_14 UlvE_ __func::~__func (deleting)", t_6E4B88);

// ScenePartyRequest::init $_14 UlvE_: __func::__clone()
static uint32_t t_6E4B90(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68ddcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E4B90, "ScenePartyRequest::init $_14 UlvE_ __func::__clone()", t_6E4B90);

// ScenePartyRequest::init $_14 UlvE_: __func::__clone(__base*)
static void t_6E4BBC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68ddcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E4BBC, "ScenePartyRequest::init $_14 UlvE_ __func::__clone(__base*)", t_6E4BBC);

// ScenePartyRequest::init $_14 UlvE_: __func::destroy
static void t_6E4BDC(uint32_t self) {
}
RW_FUNCTION(0x6E4BDC, "ScenePartyRequest::init $_14 UlvE_ __func::destroy", t_6E4BDC);

// ScenePartyRequest::init $_14 UlvE_: __func::destroy_deallocate
static void t_6E4BDE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4BDE, "ScenePartyRequest::init $_14 UlvE_ __func::destroy_deallocate", t_6E4BDE);

// ScenePartyRequest::init $_14 UlvE_: __func::target
static uint32_t t_6E4BF0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85100u ? self + 4 : 0; }
RW_FUNCTION(0x6E4BF0, "ScenePartyRequest::init $_14 UlvE_ __func::target", t_6E4BF0);

// ScenePartyRequest::init $_14 UlvE_: __func::target_type
static uint32_t t_6E4C10() { return 0xfa158cu; }
RW_FUNCTION(0x6E4C10, "ScenePartyRequest::init $_14 UlvE_ __func::target_type", t_6E4C10);

// ScenePartyRequest::init $_13: __func::~__func (deleting)
static void t_6E4C24(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4C24, "ScenePartyRequest::init $_13 __func::~__func (deleting)", t_6E4C24);

// ScenePartyRequest::init $_13: __func::__clone()
static uint32_t t_6E4C2C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68e08u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E4C2C, "ScenePartyRequest::init $_13 __func::__clone()", t_6E4C2C);

// ScenePartyRequest::init $_13: __func::__clone(__base*)
static void t_6E4C58(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68e08u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E4C58, "ScenePartyRequest::init $_13 __func::__clone(__base*)", t_6E4C58);

// ScenePartyRequest::init $_13: __func::destroy
static void t_6E4C78(uint32_t self) {
}
RW_FUNCTION(0x6E4C78, "ScenePartyRequest::init $_13 __func::destroy", t_6E4C78);

// ScenePartyRequest::init $_13: __func::destroy_deallocate
static void t_6E4C7A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4C7A, "ScenePartyRequest::init $_13 __func::destroy_deallocate", t_6E4C7A);

// ScenePartyRequest::init $_13: __func::target
static uint32_t t_6E4D68(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85190u ? self + 4 : 0; }
RW_FUNCTION(0x6E4D68, "ScenePartyRequest::init $_13 __func::target", t_6E4D68);

// ScenePartyRequest::init $_13: __func::target_type
static uint32_t t_6E4D88() { return 0xfa15a0u; }
RW_FUNCTION(0x6E4D88, "ScenePartyRequest::init $_13 __func::target_type", t_6E4D88);

// ScenePartyRequest::init $_13 UlvE_: __func::~__func (deleting)
static void t_6E4D9C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4D9C, "ScenePartyRequest::init $_13 UlvE_ __func::~__func (deleting)", t_6E4D9C);

// ScenePartyRequest::init $_13 UlvE_: __func::__clone()
static uint32_t t_6E4DA4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68e34u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E4DA4, "ScenePartyRequest::init $_13 UlvE_ __func::__clone()", t_6E4DA4);

// ScenePartyRequest::init $_13 UlvE_: __func::__clone(__base*)
static void t_6E4DD0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68e34u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E4DD0, "ScenePartyRequest::init $_13 UlvE_ __func::__clone(__base*)", t_6E4DD0);

// ScenePartyRequest::init $_13 UlvE_: __func::destroy
static void t_6E4DF0(uint32_t self) {
}
RW_FUNCTION(0x6E4DF0, "ScenePartyRequest::init $_13 UlvE_ __func::destroy", t_6E4DF0);

// ScenePartyRequest::init $_13 UlvE_: __func::destroy_deallocate
static void t_6E4DF2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4DF2, "ScenePartyRequest::init $_13 UlvE_ __func::destroy_deallocate", t_6E4DF2);

// ScenePartyRequest::init $_13 UlvE_: __func::target
static uint32_t t_6E4E04(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85230u ? self + 4 : 0; }
RW_FUNCTION(0x6E4E04, "ScenePartyRequest::init $_13 UlvE_ __func::target", t_6E4E04);

// ScenePartyRequest::init $_13 UlvE_: __func::target_type
static uint32_t t_6E4E24() { return 0xfa15b4u; }
RW_FUNCTION(0x6E4E24, "ScenePartyRequest::init $_13 UlvE_ __func::target_type", t_6E4E24);

// ScenePartyRequest::init $_12: __func::~__func (deleting)
static void t_6E4E38(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4E38, "ScenePartyRequest::init $_12 __func::~__func (deleting)", t_6E4E38);

// ScenePartyRequest::init $_12: __func::__clone()
static uint32_t t_6E4E40(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68e60u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E4E40, "ScenePartyRequest::init $_12 __func::__clone()", t_6E4E40);

// ScenePartyRequest::init $_12: __func::__clone(__base*)
static void t_6E4E6C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68e60u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E4E6C, "ScenePartyRequest::init $_12 __func::__clone(__base*)", t_6E4E6C);

// ScenePartyRequest::init $_12: __func::destroy
static void t_6E4E8C(uint32_t self) {
}
RW_FUNCTION(0x6E4E8C, "ScenePartyRequest::init $_12 __func::destroy", t_6E4E8C);

// ScenePartyRequest::init $_12: __func::destroy_deallocate
static void t_6E4E8E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4E8E, "ScenePartyRequest::init $_12 __func::destroy_deallocate", t_6E4E8E);

// ScenePartyRequest::init $_12: __func::target
static uint32_t t_6E4F7C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe852c0u ? self + 4 : 0; }
RW_FUNCTION(0x6E4F7C, "ScenePartyRequest::init $_12 __func::target", t_6E4F7C);

// ScenePartyRequest::init $_12: __func::target_type
static uint32_t t_6E4F9C() { return 0xfa15c8u; }
RW_FUNCTION(0x6E4F9C, "ScenePartyRequest::init $_12 __func::target_type", t_6E4F9C);

// ScenePartyRequest::init $_12 UlvE_: __func::~__func (deleting)
static void t_6E4FB0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E4FB0, "ScenePartyRequest::init $_12 UlvE_ __func::~__func (deleting)", t_6E4FB0);

// ScenePartyRequest::init $_12 UlvE_: __func::__clone()
static uint32_t t_6E4FB8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68e8cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E4FB8, "ScenePartyRequest::init $_12 UlvE_ __func::__clone()", t_6E4FB8);

// ScenePartyRequest::init $_12 UlvE_: __func::__clone(__base*)
static void t_6E4FE4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68e8cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E4FE4, "ScenePartyRequest::init $_12 UlvE_ __func::__clone(__base*)", t_6E4FE4);

// ScenePartyRequest::init $_12 UlvE_: __func::destroy
static void t_6E5004(uint32_t self) {
}
RW_FUNCTION(0x6E5004, "ScenePartyRequest::init $_12 UlvE_ __func::destroy", t_6E5004);

// ScenePartyRequest::init $_12 UlvE_: __func::destroy_deallocate
static void t_6E5006(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5006, "ScenePartyRequest::init $_12 UlvE_ __func::destroy_deallocate", t_6E5006);

// ScenePartyRequest::init $_12 UlvE_: __func::target
static uint32_t t_6E5018(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85360u ? self + 4 : 0; }
RW_FUNCTION(0x6E5018, "ScenePartyRequest::init $_12 UlvE_ __func::target", t_6E5018);

// ScenePartyRequest::init $_12 UlvE_: __func::target_type
static uint32_t t_6E5038() { return 0xfa15dcu; }
RW_FUNCTION(0x6E5038, "ScenePartyRequest::init $_12 UlvE_ __func::target_type", t_6E5038);

// ScenePartyRequest::init $_11: __func::~__func (deleting)
static void t_6E504C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E504C, "ScenePartyRequest::init $_11 __func::~__func (deleting)", t_6E504C);

// ScenePartyRequest::init $_11: __func::__clone()
static uint32_t t_6E5054(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68eb8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E5054, "ScenePartyRequest::init $_11 __func::__clone()", t_6E5054);

// ScenePartyRequest::init $_11: __func::__clone(__base*)
static void t_6E5080(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68eb8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E5080, "ScenePartyRequest::init $_11 __func::__clone(__base*)", t_6E5080);

// ScenePartyRequest::init $_11: __func::destroy
static void t_6E50A0(uint32_t self) {
}
RW_FUNCTION(0x6E50A0, "ScenePartyRequest::init $_11 __func::destroy", t_6E50A0);

// ScenePartyRequest::init $_11: __func::destroy_deallocate
static void t_6E50A2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E50A2, "ScenePartyRequest::init $_11 __func::destroy_deallocate", t_6E50A2);

// ScenePartyRequest::init $_11: __func::target
static uint32_t t_6E5190(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe853f0u ? self + 4 : 0; }
RW_FUNCTION(0x6E5190, "ScenePartyRequest::init $_11 __func::target", t_6E5190);

// ScenePartyRequest::init $_11: __func::target_type
static uint32_t t_6E51B0() { return 0xfa15f0u; }
RW_FUNCTION(0x6E51B0, "ScenePartyRequest::init $_11 __func::target_type", t_6E51B0);

// ScenePartyRequest::init $_11 UlvE_: __func::~__func (deleting)
static void t_6E51C4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E51C4, "ScenePartyRequest::init $_11 UlvE_ __func::~__func (deleting)", t_6E51C4);

// ScenePartyRequest::init $_11 UlvE_: __func::__clone()
static uint32_t t_6E51CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68ee4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E51CC, "ScenePartyRequest::init $_11 UlvE_ __func::__clone()", t_6E51CC);

// ScenePartyRequest::init $_11 UlvE_: __func::__clone(__base*)
static void t_6E51F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68ee4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E51F8, "ScenePartyRequest::init $_11 UlvE_ __func::__clone(__base*)", t_6E51F8);

// ScenePartyRequest::init $_11 UlvE_: __func::destroy
static void t_6E5218(uint32_t self) {
}
RW_FUNCTION(0x6E5218, "ScenePartyRequest::init $_11 UlvE_ __func::destroy", t_6E5218);

// ScenePartyRequest::init $_11 UlvE_: __func::destroy_deallocate
static void t_6E521A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E521A, "ScenePartyRequest::init $_11 UlvE_ __func::destroy_deallocate", t_6E521A);

// ScenePartyRequest::init $_11 UlvE_: __func::target
static uint32_t t_6E522C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85490u ? self + 4 : 0; }
RW_FUNCTION(0x6E522C, "ScenePartyRequest::init $_11 UlvE_ __func::target", t_6E522C);

// ScenePartyRequest::init $_11 UlvE_: __func::target_type
static uint32_t t_6E524C() { return 0xfa1604u; }
RW_FUNCTION(0x6E524C, "ScenePartyRequest::init $_11 UlvE_ __func::target_type", t_6E524C);

// ScenePartyRequest::init $_10: __func::~__func (deleting)
static void t_6E5260(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5260, "ScenePartyRequest::init $_10 __func::~__func (deleting)", t_6E5260);

// ScenePartyRequest::init $_10: __func::__clone()
static uint32_t t_6E5268(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68f10u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E5268, "ScenePartyRequest::init $_10 __func::__clone()", t_6E5268);

// ScenePartyRequest::init $_10: __func::__clone(__base*)
static void t_6E5294(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68f10u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E5294, "ScenePartyRequest::init $_10 __func::__clone(__base*)", t_6E5294);

// ScenePartyRequest::init $_10: __func::destroy
static void t_6E52B4(uint32_t self) {
}
RW_FUNCTION(0x6E52B4, "ScenePartyRequest::init $_10 __func::destroy", t_6E52B4);

// ScenePartyRequest::init $_10: __func::destroy_deallocate
static void t_6E52B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E52B6, "ScenePartyRequest::init $_10 __func::destroy_deallocate", t_6E52B6);

// ScenePartyRequest::init $_10: __func::target
static uint32_t t_6E53A4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85520u ? self + 4 : 0; }
RW_FUNCTION(0x6E53A4, "ScenePartyRequest::init $_10 __func::target", t_6E53A4);

// ScenePartyRequest::init $_10: __func::target_type
static uint32_t t_6E53C4() { return 0xfa1618u; }
RW_FUNCTION(0x6E53C4, "ScenePartyRequest::init $_10 __func::target_type", t_6E53C4);

// ScenePartyRequest::init $_10 UlvE_: __func::~__func (deleting)
static void t_6E53D8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E53D8, "ScenePartyRequest::init $_10 UlvE_ __func::~__func (deleting)", t_6E53D8);

// ScenePartyRequest::init $_10 UlvE_: __func::__clone()
static uint32_t t_6E53E0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68f3cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E53E0, "ScenePartyRequest::init $_10 UlvE_ __func::__clone()", t_6E53E0);

// ScenePartyRequest::init $_10 UlvE_: __func::__clone(__base*)
static void t_6E540C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68f3cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E540C, "ScenePartyRequest::init $_10 UlvE_ __func::__clone(__base*)", t_6E540C);

// ScenePartyRequest::init $_10 UlvE_: __func::destroy
static void t_6E542C(uint32_t self) {
}
RW_FUNCTION(0x6E542C, "ScenePartyRequest::init $_10 UlvE_ __func::destroy", t_6E542C);

// ScenePartyRequest::init $_10 UlvE_: __func::destroy_deallocate
static void t_6E542E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E542E, "ScenePartyRequest::init $_10 UlvE_ __func::destroy_deallocate", t_6E542E);

// ScenePartyRequest::init $_10 UlvE_: __func::target
static uint32_t t_6E5440(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe855c0u ? self + 4 : 0; }
RW_FUNCTION(0x6E5440, "ScenePartyRequest::init $_10 UlvE_ __func::target", t_6E5440);

// ScenePartyRequest::init $_10 UlvE_: __func::target_type
static uint32_t t_6E5460() { return 0xfa162cu; }
RW_FUNCTION(0x6E5460, "ScenePartyRequest::init $_10 UlvE_ __func::target_type", t_6E5460);

// ScenePartyRequest::init $_9: __func::~__func (deleting)
static void t_6E5474(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5474, "ScenePartyRequest::init $_9 __func::~__func (deleting)", t_6E5474);

// ScenePartyRequest::init $_9: __func::__clone()
static uint32_t t_6E547C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68f68u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E547C, "ScenePartyRequest::init $_9 __func::__clone()", t_6E547C);

// ScenePartyRequest::init $_9: __func::__clone(__base*)
static void t_6E54A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68f68u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E54A8, "ScenePartyRequest::init $_9 __func::__clone(__base*)", t_6E54A8);

// ScenePartyRequest::init $_9: __func::destroy
static void t_6E54C8(uint32_t self) {
}
RW_FUNCTION(0x6E54C8, "ScenePartyRequest::init $_9 __func::destroy", t_6E54C8);

// ScenePartyRequest::init $_9: __func::destroy_deallocate
static void t_6E54CA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E54CA, "ScenePartyRequest::init $_9 __func::destroy_deallocate", t_6E54CA);

// ScenePartyRequest::init $_9: __func::target
static uint32_t t_6E55B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85650u ? self + 4 : 0; }
RW_FUNCTION(0x6E55B8, "ScenePartyRequest::init $_9 __func::target", t_6E55B8);

// ScenePartyRequest::init $_9: __func::target_type
static uint32_t t_6E55D8() { return 0xfa1640u; }
RW_FUNCTION(0x6E55D8, "ScenePartyRequest::init $_9 __func::target_type", t_6E55D8);

// ScenePartyRequest::init $_9 UlvE_: __func::~__func (deleting)
static void t_6E55EC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E55EC, "ScenePartyRequest::init $_9 UlvE_ __func::~__func (deleting)", t_6E55EC);

// ScenePartyRequest::init $_9 UlvE_: __func::__clone()
static uint32_t t_6E55F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68f94u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E55F4, "ScenePartyRequest::init $_9 UlvE_ __func::__clone()", t_6E55F4);

// ScenePartyRequest::init $_9 UlvE_: __func::__clone(__base*)
static void t_6E5620(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68f94u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E5620, "ScenePartyRequest::init $_9 UlvE_ __func::__clone(__base*)", t_6E5620);

// ScenePartyRequest::init $_9 UlvE_: __func::destroy
static void t_6E5640(uint32_t self) {
}
RW_FUNCTION(0x6E5640, "ScenePartyRequest::init $_9 UlvE_ __func::destroy", t_6E5640);

// ScenePartyRequest::init $_9 UlvE_: __func::destroy_deallocate
static void t_6E5642(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5642, "ScenePartyRequest::init $_9 UlvE_ __func::destroy_deallocate", t_6E5642);

// ScenePartyRequest::init $_9 UlvE_: __func::target
static uint32_t t_6E5654(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe856f0u ? self + 4 : 0; }
RW_FUNCTION(0x6E5654, "ScenePartyRequest::init $_9 UlvE_ __func::target", t_6E5654);

// ScenePartyRequest::init $_9 UlvE_: __func::target_type
static uint32_t t_6E5674() { return 0xfa1654u; }
RW_FUNCTION(0x6E5674, "ScenePartyRequest::init $_9 UlvE_ __func::target_type", t_6E5674);

// ScenePartyRequest::init $_7: __func::~__func (deleting)
static void t_6E5688(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5688, "ScenePartyRequest::init $_7 __func::~__func (deleting)", t_6E5688);

// ScenePartyRequest::init $_7: __func::__clone()
static uint32_t t_6E5690(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf68fc0u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E5690, "ScenePartyRequest::init $_7 __func::__clone()", t_6E5690);

// ScenePartyRequest::init $_7: __func::__clone(__base*)
static void t_6E56C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68fc0u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E56C0, "ScenePartyRequest::init $_7 __func::__clone(__base*)", t_6E56C0);

// ScenePartyRequest::init $_7: __func::destroy
static void t_6E56E4(uint32_t self) {
}
RW_FUNCTION(0x6E56E4, "ScenePartyRequest::init $_7 __func::destroy", t_6E56E4);

// ScenePartyRequest::init $_7: __func::destroy_deallocate
static void t_6E56E6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E56E6, "ScenePartyRequest::init $_7 __func::destroy_deallocate", t_6E56E6);

// ScenePartyRequest::init $_7: __func::target
static uint32_t t_6E582C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe857b0u ? self + 4 : 0; }
RW_FUNCTION(0x6E582C, "ScenePartyRequest::init $_7 __func::target", t_6E582C);

// ScenePartyRequest::init $_7: __func::target_type
static uint32_t t_6E584C() { return 0xfa1668u; }
RW_FUNCTION(0x6E584C, "ScenePartyRequest::init $_7 __func::target_type", t_6E584C);

// ScenePartyRequest::init $_85 UlbE0_: __func::~__func (deleting)
static void t_6E5860(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5860, "ScenePartyRequest::init $_85 UlbE0_ __func::~__func (deleting)", t_6E5860);

// ScenePartyRequest::init $_85 UlbE0_: __func::__clone()
static uint32_t t_6E5868(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf68fecu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E5868, "ScenePartyRequest::init $_85 UlbE0_ __func::__clone()", t_6E5868);

// ScenePartyRequest::init $_85 UlbE0_: __func::__clone(__base*)
static void t_6E5894(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf68fecu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E5894, "ScenePartyRequest::init $_85 UlbE0_ __func::__clone(__base*)", t_6E5894);

// ScenePartyRequest::init $_85 UlbE0_: __func::destroy
static void t_6E58B4(uint32_t self) {
}
RW_FUNCTION(0x6E58B4, "ScenePartyRequest::init $_85 UlbE0_ __func::destroy", t_6E58B4);

// ScenePartyRequest::init $_85 UlbE0_: __func::destroy_deallocate
static void t_6E58B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E58B6, "ScenePartyRequest::init $_85 UlbE0_ __func::destroy_deallocate", t_6E58B6);

// ScenePartyRequest::init $_85 UlbE0_: __func::target
static uint32_t t_6E5994(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85850u ? self + 4 : 0; }
RW_FUNCTION(0x6E5994, "ScenePartyRequest::init $_85 UlbE0_ __func::target", t_6E5994);

// ScenePartyRequest::init $_85 UlbE0_: __func::target_type
static uint32_t t_6E59B4() { return 0xfa167cu; }
RW_FUNCTION(0x6E59B4, "ScenePartyRequest::init $_85 UlbE0_ __func::target_type", t_6E59B4);

// ScenePartyRequest::init $_85 UlbE0_ UlbE_: __func::~__func (deleting)
static void t_6E59C8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E59C8, "ScenePartyRequest::init $_85 UlbE0_ UlbE_ __func::~__func (deleting)", t_6E59C8);

// ScenePartyRequest::init $_85 UlbE0_ UlbE_: __func::__clone()
static uint32_t t_6E59D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69018u);
    }
    return v1;
}
RW_FUNCTION(0x6E59D0, "ScenePartyRequest::init $_85 UlbE0_ UlbE_ __func::__clone()", t_6E59D0);

// ScenePartyRequest::init $_85 UlbE0_ UlbE_: __func::__clone(__base*)
static void t_6E59F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69018u);
    }
}
RW_FUNCTION(0x6E59F4, "ScenePartyRequest::init $_85 UlbE0_ UlbE_ __func::__clone(__base*)", t_6E59F4);

// ScenePartyRequest::init $_85 UlbE0_ UlbE_: __func::destroy
static void t_6E5A10(uint32_t self) {
}
RW_FUNCTION(0x6E5A10, "ScenePartyRequest::init $_85 UlbE0_ UlbE_ __func::destroy", t_6E5A10);

// ScenePartyRequest::init $_85 UlbE0_ UlbE_: __func::destroy_deallocate
static void t_6E5A12(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5A12, "ScenePartyRequest::init $_85 UlbE0_ UlbE_ __func::destroy_deallocate", t_6E5A12);

// ScenePartyRequest::init $_85 UlbE0_ UlbE_: __func::target
static uint32_t t_6E5A1C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85910u ? self + 4 : 0; }
RW_FUNCTION(0x6E5A1C, "ScenePartyRequest::init $_85 UlbE0_ UlbE_ __func::target", t_6E5A1C);

// ScenePartyRequest::init $_85 UlbE0_ UlbE_: __func::target_type
static uint32_t t_6E5A3C() { return 0xfa1690u; }
RW_FUNCTION(0x6E5A3C, "ScenePartyRequest::init $_85 UlbE0_ UlbE_ __func::target_type", t_6E5A3C);

// ScenePartyRequest::init $_85 UlbE_: __func::~__func (deleting)
static void t_6E5A50(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5A50, "ScenePartyRequest::init $_85 UlbE_ __func::~__func (deleting)", t_6E5A50);

// ScenePartyRequest::init $_85 UlbE_: __func::__clone()
static uint32_t t_6E5A58(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69044u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E5A58, "ScenePartyRequest::init $_85 UlbE_ __func::__clone()", t_6E5A58);

// ScenePartyRequest::init $_85 UlbE_: __func::__clone(__base*)
static void t_6E5A84(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69044u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E5A84, "ScenePartyRequest::init $_85 UlbE_ __func::__clone(__base*)", t_6E5A84);

// ScenePartyRequest::init $_85 UlbE_: __func::destroy
static void t_6E5AA4(uint32_t self) {
}
RW_FUNCTION(0x6E5AA4, "ScenePartyRequest::init $_85 UlbE_ __func::destroy", t_6E5AA4);

// ScenePartyRequest::init $_85 UlbE_: __func::destroy_deallocate
static void t_6E5AA6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5AA6, "ScenePartyRequest::init $_85 UlbE_ __func::destroy_deallocate", t_6E5AA6);

// ScenePartyRequest::init $_85 UlbE_: __func::target
static uint32_t t_6E5B84(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe859c0u ? self + 4 : 0; }
RW_FUNCTION(0x6E5B84, "ScenePartyRequest::init $_85 UlbE_ __func::target", t_6E5B84);

// ScenePartyRequest::init $_85 UlbE_: __func::target_type
static uint32_t t_6E5BA4() { return 0xfa16a4u; }
RW_FUNCTION(0x6E5BA4, "ScenePartyRequest::init $_85 UlbE_ __func::target_type", t_6E5BA4);

// ScenePartyRequest::init $_85 UlbE_ UlbE_: __func::~__func (deleting)
static void t_6E5BB8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5BB8, "ScenePartyRequest::init $_85 UlbE_ UlbE_ __func::~__func (deleting)", t_6E5BB8);

// ScenePartyRequest::init $_85 UlbE_ UlbE_: __func::__clone()
static uint32_t t_6E5BC0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69070u);
    }
    return v1;
}
RW_FUNCTION(0x6E5BC0, "ScenePartyRequest::init $_85 UlbE_ UlbE_ __func::__clone()", t_6E5BC0);

// ScenePartyRequest::init $_85 UlbE_ UlbE_: __func::__clone(__base*)
static void t_6E5BE4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69070u);
    }
}
RW_FUNCTION(0x6E5BE4, "ScenePartyRequest::init $_85 UlbE_ UlbE_ __func::__clone(__base*)", t_6E5BE4);

// ScenePartyRequest::init $_85 UlbE_ UlbE_: __func::destroy
static void t_6E5C00(uint32_t self) {
}
RW_FUNCTION(0x6E5C00, "ScenePartyRequest::init $_85 UlbE_ UlbE_ __func::destroy", t_6E5C00);

// ScenePartyRequest::init $_85 UlbE_ UlbE_: __func::destroy_deallocate
static void t_6E5C02(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5C02, "ScenePartyRequest::init $_85 UlbE_ UlbE_ __func::destroy_deallocate", t_6E5C02);

// ScenePartyRequest::init $_85 UlbE_ UlbE_: __func::target
static uint32_t t_6E5C0C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85a70u ? self + 4 : 0; }
RW_FUNCTION(0x6E5C0C, "ScenePartyRequest::init $_85 UlbE_ UlbE_ __func::target", t_6E5C0C);

// ScenePartyRequest::init $_85 UlbE_ UlbE_: __func::target_type
static uint32_t t_6E5C2C() { return 0xfa16b8u; }
RW_FUNCTION(0x6E5C2C, "ScenePartyRequest::init $_85 UlbE_ UlbE_ __func::target_type", t_6E5C2C);

// ScenePartyRequest::init $_5: __func::~__func (deleting)
static void t_6E5C40(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5C40, "ScenePartyRequest::init $_5 __func::~__func (deleting)", t_6E5C40);

// ScenePartyRequest::init $_5: __func::__clone()
static uint32_t t_6E5C48(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6909cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E5C48, "ScenePartyRequest::init $_5 __func::__clone()", t_6E5C48);

// ScenePartyRequest::init $_5: __func::__clone(__base*)
static void t_6E5C74(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6909cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E5C74, "ScenePartyRequest::init $_5 __func::__clone(__base*)", t_6E5C74);

// ScenePartyRequest::init $_5: __func::destroy
static void t_6E5C94(uint32_t self) {
}
RW_FUNCTION(0x6E5C94, "ScenePartyRequest::init $_5 __func::destroy", t_6E5C94);

// ScenePartyRequest::init $_5: __func::destroy_deallocate
static void t_6E5C96(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5C96, "ScenePartyRequest::init $_5 __func::destroy_deallocate", t_6E5C96);

// ScenePartyRequest::init $_5: __func::target
static uint32_t t_6E5CB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85b40u ? self + 4 : 0; }
RW_FUNCTION(0x6E5CB8, "ScenePartyRequest::init $_5 __func::target", t_6E5CB8);

// ScenePartyRequest::init $_5: __func::target_type
static uint32_t t_6E5CD8() { return 0xfa16ccu; }
RW_FUNCTION(0x6E5CD8, "ScenePartyRequest::init $_5 __func::target_type", t_6E5CD8);

// ScenePartyRequest::init $_4: __func::~__func (deleting)
static void t_6E5CEC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5CEC, "ScenePartyRequest::init $_4 __func::~__func (deleting)", t_6E5CEC);

// ScenePartyRequest::init $_4: __func::__clone()
static uint32_t t_6E5CF4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf690c8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E5CF4, "ScenePartyRequest::init $_4 __func::__clone()", t_6E5CF4);

// ScenePartyRequest::init $_4: __func::__clone(__base*)
static void t_6E5D20(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf690c8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E5D20, "ScenePartyRequest::init $_4 __func::__clone(__base*)", t_6E5D20);

// ScenePartyRequest::init $_4: __func::destroy
static void t_6E5D40(uint32_t self) {
}
RW_FUNCTION(0x6E5D40, "ScenePartyRequest::init $_4 __func::destroy", t_6E5D40);

// ScenePartyRequest::init $_4: __func::destroy_deallocate
static void t_6E5D42(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5D42, "ScenePartyRequest::init $_4 __func::destroy_deallocate", t_6E5D42);

// ScenePartyRequest::init $_4: __func::target
static uint32_t t_6E5D54(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85bd0u ? self + 4 : 0; }
RW_FUNCTION(0x6E5D54, "ScenePartyRequest::init $_4 __func::target", t_6E5D54);

// ScenePartyRequest::init $_4: __func::target_type
static uint32_t t_6E5D74() { return 0xfa16e0u; }
RW_FUNCTION(0x6E5D74, "ScenePartyRequest::init $_4 __func::target_type", t_6E5D74);

// ScenePartyRequest::init $_3: __func::~__func (deleting)
static void t_6E5D88(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5D88, "ScenePartyRequest::init $_3 __func::~__func (deleting)", t_6E5D88);

// ScenePartyRequest::init $_3: __func::__clone()
static uint32_t t_6E5D90(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf690f4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E5D90, "ScenePartyRequest::init $_3 __func::__clone()", t_6E5D90);

// ScenePartyRequest::init $_3: __func::__clone(__base*)
static void t_6E5DBC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf690f4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E5DBC, "ScenePartyRequest::init $_3 __func::__clone(__base*)", t_6E5DBC);

// ScenePartyRequest::init $_3: __func::destroy
static void t_6E5DDC(uint32_t self) {
}
RW_FUNCTION(0x6E5DDC, "ScenePartyRequest::init $_3 __func::destroy", t_6E5DDC);

// ScenePartyRequest::init $_3: __func::destroy_deallocate
static void t_6E5DDE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5DDE, "ScenePartyRequest::init $_3 __func::destroy_deallocate", t_6E5DDE);

// ScenePartyRequest::init $_3: __func::target
static uint32_t t_6E5DF0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85c60u ? self + 4 : 0; }
RW_FUNCTION(0x6E5DF0, "ScenePartyRequest::init $_3 __func::target", t_6E5DF0);

// ScenePartyRequest::init $_3: __func::target_type
static uint32_t t_6E5E10() { return 0xfa16f4u; }
RW_FUNCTION(0x6E5E10, "ScenePartyRequest::init $_3 __func::target_type", t_6E5E10);

// ScenePartyRequest::init $_2: __func::~__func (deleting)
static void t_6E5E24(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5E24, "ScenePartyRequest::init $_2 __func::~__func (deleting)", t_6E5E24);

// ScenePartyRequest::init $_2: __func::__clone()
static uint32_t t_6E5E2C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69120u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6E5E2C, "ScenePartyRequest::init $_2 __func::__clone()", t_6E5E2C);

// ScenePartyRequest::init $_2: __func::__clone(__base*)
static void t_6E5E58(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69120u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6E5E58, "ScenePartyRequest::init $_2 __func::__clone(__base*)", t_6E5E58);

// ScenePartyRequest::init $_2: __func::destroy
static void t_6E5E78(uint32_t self) {
}
RW_FUNCTION(0x6E5E78, "ScenePartyRequest::init $_2 __func::destroy", t_6E5E78);

// ScenePartyRequest::init $_2: __func::destroy_deallocate
static void t_6E5E7A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5E7A, "ScenePartyRequest::init $_2 __func::destroy_deallocate", t_6E5E7A);

// ScenePartyRequest::init $_2: __func::target
static uint32_t t_6E5E8C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85cf0u ? self + 4 : 0; }
RW_FUNCTION(0x6E5E8C, "ScenePartyRequest::init $_2 __func::target", t_6E5E8C);

// ScenePartyRequest::init $_2: __func::target_type
static uint32_t t_6E5EAC() { return 0xfa1708u; }
RW_FUNCTION(0x6E5EAC, "ScenePartyRequest::init $_2 __func::target_type", t_6E5EAC);

// ScenePartyRequest::callPartySearchAPI $_1: __func::~__func
static uint32_t t_6E5EC0(uint32_t self) {
    st32(self, 0xf6914cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6E5EC0, "ScenePartyRequest::callPartySearchAPI $_1 __func::~__func", t_6E5EC0);

// ScenePartyRequest::callPartySearchAPI $_1: __func::~__func (deleting)
static void t_6E5EFC(uint32_t self) {
    st32(self, 0xf6914cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5EFC, "ScenePartyRequest::callPartySearchAPI $_1 __func::~__func (deleting)", t_6E5EFC);

// ScenePartyRequest::callPartySearchAPI $_1: __func::__clone()
static uint32_t t_6E5F3C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6914cu);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6E5F3C, "ScenePartyRequest::callPartySearchAPI $_1 __func::__clone()", t_6E5F3C);

// ScenePartyRequest::callPartySearchAPI $_1: __func::__clone(__base*)
static void t_6E5F8C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6914cu);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x6E5F8C, "ScenePartyRequest::callPartySearchAPI $_1 __func::__clone(__base*)", t_6E5F8C);

// ScenePartyRequest::callPartySearchAPI $_1: __func::destroy
static void t_6E5FC0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6E5FC0, "ScenePartyRequest::callPartySearchAPI $_1 __func::destroy", t_6E5FC0);

// ScenePartyRequest::callPartySearchAPI $_1: __func::destroy_deallocate
static void t_6E5FE0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E5FE0, "ScenePartyRequest::callPartySearchAPI $_1 __func::destroy_deallocate", t_6E5FE0);

// ScenePartyRequest::callPartySearchAPI $_1: __func::target
static uint32_t t_6E6020(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85de0u ? self + 8 : 0; }
RW_FUNCTION(0x6E6020, "ScenePartyRequest::callPartySearchAPI $_1 __func::target", t_6E6020);

// ScenePartyRequest::callPartySearchAPI $_1: __func::target_type
static uint32_t t_6E6040() { return 0xfa171cu; }
RW_FUNCTION(0x6E6040, "ScenePartyRequest::callPartySearchAPI $_1 __func::target_type", t_6E6040);

// ScenePartyRequest::callUserSearchAPI $_0: __func::~__func
static uint32_t t_6E6054(uint32_t self) {
    st32(self, 0xf69178u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6E6054, "ScenePartyRequest::callUserSearchAPI $_0 __func::~__func", t_6E6054);

// ScenePartyRequest::callUserSearchAPI $_0: __func::~__func (deleting)
static void t_6E6090(uint32_t self) {
    st32(self, 0xf69178u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E6090, "ScenePartyRequest::callUserSearchAPI $_0 __func::~__func (deleting)", t_6E6090);

// ScenePartyRequest::callUserSearchAPI $_0: __func::__clone()
static uint32_t t_6E60D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf69178u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6E60D0, "ScenePartyRequest::callUserSearchAPI $_0 __func::__clone()", t_6E60D0);

// ScenePartyRequest::callUserSearchAPI $_0: __func::__clone(__base*)
static void t_6E6120(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69178u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x6E6120, "ScenePartyRequest::callUserSearchAPI $_0 __func::__clone(__base*)", t_6E6120);

// ScenePartyRequest::callUserSearchAPI $_0: __func::destroy
static void t_6E6154(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6E6154, "ScenePartyRequest::callUserSearchAPI $_0 __func::destroy", t_6E6154);

// ScenePartyRequest::callUserSearchAPI $_0: __func::destroy_deallocate
static void t_6E6174(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E6174, "ScenePartyRequest::callUserSearchAPI $_0 __func::destroy_deallocate", t_6E6174);

// ScenePartyRequest::callUserSearchAPI $_0: __func::target
static uint32_t t_6E61B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe85f20u ? self + 8 : 0; }
RW_FUNCTION(0x6E61B4, "ScenePartyRequest::callUserSearchAPI $_0 __func::target", t_6E61B4);

// ScenePartyRequest::callUserSearchAPI $_0: __func::target_type
static uint32_t t_6E61D4() { return 0xfa1730u; }
RW_FUNCTION(0x6E61D4, "ScenePartyRequest::callUserSearchAPI $_0 __func::target_type", t_6E61D4);

// ScenePartyRequest::init $_8: __func::~__func
static uint32_t t_6E61E8(uint32_t self) {
    st32(self, 0xf691a4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6E61E8, "ScenePartyRequest::init $_8 __func::~__func", t_6E61E8);

// ScenePartyRequest::init $_8: __func::~__func (deleting)
static void t_6E6224(uint32_t self) {
    st32(self, 0xf691a4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E6224, "ScenePartyRequest::init $_8 __func::~__func (deleting)", t_6E6224);

// ScenePartyRequest::init $_8: __func::__clone()
static uint32_t t_6E6264(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf691a4u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x6E6264, "ScenePartyRequest::init $_8 __func::__clone()", t_6E6264);

// ScenePartyRequest::init $_8: __func::__clone(__base*)
static void t_6E62B4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf691a4u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x6E62B4, "ScenePartyRequest::init $_8 __func::__clone(__base*)", t_6E62B4);

// ScenePartyRequest::init $_8: __func::destroy
static void t_6E62E8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x6E62E8, "ScenePartyRequest::init $_8 __func::destroy", t_6E62E8);

// ScenePartyRequest::init $_8: __func::destroy_deallocate
static void t_6E6308(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6E6308, "ScenePartyRequest::init $_8 __func::destroy_deallocate", t_6E6308);

// ScenePartyRequest::init $_8: __func::target
static uint32_t t_6E6350(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe86050u ? self + 8 : 0; }
RW_FUNCTION(0x6E6350, "ScenePartyRequest::init $_8 __func::target", t_6E6350);

// ScenePartyRequest::init $_8: __func::target_type
static uint32_t t_6E6370() { return 0xfa1744u; }
RW_FUNCTION(0x6E6370, "ScenePartyRequest::init $_8 __func::target_type", t_6E6370);
