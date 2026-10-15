// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneUtil lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneUtil::openUserTitlePopup $_57: __func::~__func (deleting)
static void t_48467A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48467A, "SceneUtil::openUserTitlePopup $_57 __func::~__func (deleting)", t_48467A);

// SceneUtil::openMedalOverDialog $_18 UlvE_: __func::target_type
static uint32_t t_48A3A8() { return 0xf7dc68u; }
RW_FUNCTION(0x48A3A8, "SceneUtil::openMedalOverDialog $_18 UlvE_ __func::target_type", t_48A3A8);

// SceneUtil::openMedalOverDialog $_20: __func::~__func
static uint32_t t_48A3BC(uint32_t self) {
    st32(self, 0xf54dccu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x48A3BC, "SceneUtil::openMedalOverDialog $_20 __func::~__func", t_48A3BC);

// SceneUtil::openMedalOverDialog $_20: __func::~__func (deleting)
static void t_48A3F8(uint32_t self) {
    st32(self, 0xf54dccu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48A3F8, "SceneUtil::openMedalOverDialog $_20 __func::~__func (deleting)", t_48A3F8);

// SceneUtil::openMedalOverDialog $_20: __func::__clone()
static uint32_t t_48A438(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf54dccu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
        call(0x45c84d, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48A438, "SceneUtil::openMedalOverDialog $_20 __func::__clone()", t_48A438);

// SceneUtil::openMedalOverDialog $_20: __func::__clone(__base*)
static void t_48A494(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54dccu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48A494, "SceneUtil::openMedalOverDialog $_20 __func::__clone(__base*)", t_48A494);

// SceneUtil::openMedalOverDialog $_20: __func::destroy
static void t_48A4D4(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x48A4D4, "SceneUtil::openMedalOverDialog $_20 __func::destroy", t_48A4D4);

// SceneUtil::openMedalOverDialog $_20: __func::destroy_deallocate
static void t_48A4F4(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48A4F4, "SceneUtil::openMedalOverDialog $_20 __func::destroy_deallocate", t_48A4F4);

// SceneUtil::openMedalOverDialog $_20: __func::target
static uint32_t t_48A5F4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf5a40u ? self + 8 : 0; }
RW_FUNCTION(0x48A5F4, "SceneUtil::openMedalOverDialog $_20 __func::target", t_48A5F4);

// SceneUtil::openMedalOverDialog $_20: __func::target_type
static uint32_t t_48A618() { return 0xf7dc7cu; }
RW_FUNCTION(0x48A618, "SceneUtil::openMedalOverDialog $_20 __func::target_type", t_48A618);

// SceneUtil::openMedalOverDialog $_19: __func::~__func
static uint32_t t_48A780(uint32_t self) {
    st32(self, 0xf54df8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x48A780, "SceneUtil::openMedalOverDialog $_19 __func::~__func", t_48A780);

// SceneUtil::openMedalOverDialog $_19: __func::~__func (deleting)
static void t_48A7BC(uint32_t self) {
    st32(self, 0xf54df8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48A7BC, "SceneUtil::openMedalOverDialog $_19 __func::~__func (deleting)", t_48A7BC);

// SceneUtil::openMedalOverDialog $_19: __func::__clone()
static uint32_t t_48A7FC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf54df8u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
        call(0x45c84d, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48A7FC, "SceneUtil::openMedalOverDialog $_19 __func::__clone()", t_48A7FC);

// SceneUtil::openMedalOverDialog $_19: __func::__clone(__base*)
static void t_48A858(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54df8u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48A858, "SceneUtil::openMedalOverDialog $_19 __func::__clone(__base*)", t_48A858);

// SceneUtil::openMedalOverDialog $_19: __func::destroy
static void t_48A898(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x48A898, "SceneUtil::openMedalOverDialog $_19 __func::destroy", t_48A898);

// SceneUtil::openMedalOverDialog $_19: __func::destroy_deallocate
static void t_48A8B8(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48A8B8, "SceneUtil::openMedalOverDialog $_19 __func::destroy_deallocate", t_48A8B8);

// SceneUtil::openMedalOverDialog $_19: __func::target
static uint32_t t_48AA08(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf5b40u ? self + 8 : 0; }
RW_FUNCTION(0x48AA08, "SceneUtil::openMedalOverDialog $_19 __func::target", t_48AA08);

// SceneUtil::openMedalOverDialog $_19: __func::target_type
static uint32_t t_48AA2C() { return 0xf7dc90u; }
RW_FUNCTION(0x48AA2C, "SceneUtil::openMedalOverDialog $_19 __func::target_type", t_48AA2C);

// SceneUtil::openMedalOverDialog $_19 UlbbE_: __func::~__func
static uint32_t t_48AA40(uint32_t self) {
    st32(self, 0xf54e24u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x48AA40, "SceneUtil::openMedalOverDialog $_19 UlbbE_ __func::~__func", t_48AA40);

// SceneUtil::openMedalOverDialog $_19 UlbbE_: __func::~__func (deleting)
static void t_48AA7C(uint32_t self) {
    st32(self, 0xf54e24u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48AA7C, "SceneUtil::openMedalOverDialog $_19 UlbbE_ __func::~__func (deleting)", t_48AA7C);

// SceneUtil::openMedalOverDialog $_19 UlbbE_: __func::__clone()
static uint32_t t_48AABC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf54e24u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        st32(v1 + 16, v4);
        st32(v1 + 20, v5);
        call(0x45c84d, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48AABC, "SceneUtil::openMedalOverDialog $_19 UlbbE_ __func::__clone()", t_48AABC);

// SceneUtil::openMedalOverDialog $_19 UlbbE_: __func::__clone(__base*)
static void t_48AB18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54e24u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        call(0x45c84d, {p + 24, self + 24});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48AB18, "SceneUtil::openMedalOverDialog $_19 UlbbE_ __func::__clone(__base*)", t_48AB18);

// SceneUtil::openMedalOverDialog $_19 UlbbE_: __func::destroy
static void t_48AB58(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x48AB58, "SceneUtil::openMedalOverDialog $_19 UlbbE_ __func::destroy", t_48AB58);

// SceneUtil::openMedalOverDialog $_19 UlbbE_: __func::destroy_deallocate
static void t_48AB78(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48AB78, "SceneUtil::openMedalOverDialog $_19 UlbbE_ __func::destroy_deallocate", t_48AB78);

// SceneUtil::openMedalOverDialog $_19 UlbbE_: __func::target
static uint32_t t_48AC50(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf5c50u ? self + 8 : 0; }
RW_FUNCTION(0x48AC50, "SceneUtil::openMedalOverDialog $_19 UlbbE_ __func::target", t_48AC50);

// SceneUtil::openMedalOverDialog $_19 UlbbE_: __func::target_type
static uint32_t t_48AC74() { return 0xf7dca4u; }
RW_FUNCTION(0x48AC74, "SceneUtil::openMedalOverDialog $_19 UlbbE_ __func::target_type", t_48AC74);

// SceneUtil::openMedalMaxUpResultDialog $_17: __func::~__func
static uint32_t t_48AC88(uint32_t self) {
    st32(self, 0xf54e50u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48AC88, "SceneUtil::openMedalMaxUpResultDialog $_17 __func::~__func", t_48AC88);

// SceneUtil::openMedalMaxUpResultDialog $_17: __func::~__func (deleting)
static void t_48ACC4(uint32_t self) {
    st32(self, 0xf54e50u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48ACC4, "SceneUtil::openMedalMaxUpResultDialog $_17 __func::~__func (deleting)", t_48ACC4);

// SceneUtil::openMedalMaxUpResultDialog $_17: __func::__clone()
static uint32_t t_48AD04(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf54e50u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 32 + 0);
        uint32_t v3 = ld32(self + 32 + 4);
        uint32_t v4 = ld32(self + 32 + 8);
        st32(v1 + 32 + 0, v2);
        st32(v1 + 32 + 4, v3);
        st32(v1 + 32 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x48AD04, "SceneUtil::openMedalMaxUpResultDialog $_17 __func::__clone()", t_48AD04);

// SceneUtil::openMedalMaxUpResultDialog $_17: __func::__clone(__base*)
static void t_48AD60(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54e50u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v1 = ld32(self + 32 + 0);
        uint32_t v2 = ld32(self + 32 + 4);
        uint32_t v3 = ld32(self + 32 + 8);
        st32(p + 32 + 0, v1);
        st32(p + 32 + 4, v2);
        st32(p + 32 + 8, v3);
    }
}
RW_FUNCTION(0x48AD60, "SceneUtil::openMedalMaxUpResultDialog $_17 __func::__clone(__base*)", t_48AD60);

// SceneUtil::openMedalMaxUpResultDialog $_17: __func::destroy
static void t_48ADA4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48ADA4, "SceneUtil::openMedalMaxUpResultDialog $_17 __func::destroy", t_48ADA4);

// SceneUtil::openMedalMaxUpResultDialog $_17: __func::destroy_deallocate
static void t_48ADC4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48ADC4, "SceneUtil::openMedalMaxUpResultDialog $_17 __func::destroy_deallocate", t_48ADC4);

// SceneUtil::openMedalMaxUpResultDialog $_17: __func::target
static uint32_t t_48AECC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf5d90u ? self + 8 : 0; }
RW_FUNCTION(0x48AECC, "SceneUtil::openMedalMaxUpResultDialog $_17 __func::target", t_48AECC);

// SceneUtil::openMedalMaxUpResultDialog $_17: __func::target_type
static uint32_t t_48AEEC() { return 0xf7dcb8u; }
RW_FUNCTION(0x48AEEC, "SceneUtil::openMedalMaxUpResultDialog $_17 __func::target_type", t_48AEEC);

// SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_: __func::~__func (deleting)
static void t_48AF00(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48AF00, "SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_ __func::~__func (deleting)", t_48AF00);

// SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_: __func::__clone()
static uint32_t t_48AF08(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf54e7cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x48AF08, "SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_ __func::__clone()", t_48AF08);

// SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_: __func::__clone(__base*)
static void t_48AF38(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54e7cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x48AF38, "SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_ __func::__clone(__base*)", t_48AF38);

// SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_: __func::destroy
static void t_48AF60(uint32_t self) {
}
RW_FUNCTION(0x48AF60, "SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_ __func::destroy", t_48AF60);

// SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_: __func::destroy_deallocate
static void t_48AF62(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48AF62, "SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_ __func::destroy_deallocate", t_48AF62);

// SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_: __func::target
static uint32_t t_48AF94(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf5e70u ? self + 4 : 0; }
RW_FUNCTION(0x48AF94, "SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_ __func::target", t_48AF94);

// SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_: __func::target_type
static uint32_t t_48AFB4() { return 0xf7dcccu; }
RW_FUNCTION(0x48AFB4, "SceneUtil::openMedalMaxUpResultDialog $_137 UlvE_ __func::target_type", t_48AFB4);

// SceneUtil::openMedalMaxUpDialog $_16: __func::~__func
static uint32_t t_48AFC8(uint32_t self) {
    st32(self, 0xf54ea8u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48AFC8, "SceneUtil::openMedalMaxUpDialog $_16 __func::~__func", t_48AFC8);

// SceneUtil::openMedalMaxUpDialog $_16: __func::~__func (deleting)
static void t_48B004(uint32_t self) {
    st32(self, 0xf54ea8u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B004, "SceneUtil::openMedalMaxUpDialog $_16 __func::~__func (deleting)", t_48B004);

// SceneUtil::openMedalMaxUpDialog $_16: __func::__clone()
static uint32_t t_48B044(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf54ea8u);
        call(0x45cd55, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48B044, "SceneUtil::openMedalMaxUpDialog $_16 __func::__clone()", t_48B044);

// SceneUtil::openMedalMaxUpDialog $_16: __func::__clone(__base*)
static void t_48B094(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54ea8u);
        call(0x45cd55, {p + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48B094, "SceneUtil::openMedalMaxUpDialog $_16 __func::__clone(__base*)", t_48B094);

// SceneUtil::openMedalMaxUpDialog $_16: __func::destroy
static void t_48B0C8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48B0C8, "SceneUtil::openMedalMaxUpDialog $_16 __func::destroy", t_48B0C8);

// SceneUtil::openMedalMaxUpDialog $_16: __func::destroy_deallocate
static void t_48B0E8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B0E8, "SceneUtil::openMedalMaxUpDialog $_16 __func::destroy_deallocate", t_48B0E8);

// SceneUtil::openMedalMaxUpDialog $_16: __func::target
static uint32_t t_48B124(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf5f50u ? self + 8 : 0; }
RW_FUNCTION(0x48B124, "SceneUtil::openMedalMaxUpDialog $_16 __func::target", t_48B124);

// SceneUtil::openMedalMaxUpDialog $_16: __func::target_type
static uint32_t t_48B144() { return 0xf7dce0u; }
RW_FUNCTION(0x48B144, "SceneUtil::openMedalMaxUpDialog $_16 __func::target_type", t_48B144);

// SceneUtil::openContinuPopup $_13: __func::~__func
static uint32_t t_48B220(uint32_t self) {
    st32(self, 0xf54ed4u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x48B220, "SceneUtil::openContinuPopup $_13 __func::~__func", t_48B220);

// SceneUtil::openContinuPopup $_13: __func::~__func (deleting)
static void t_48B25C(uint32_t self) {
    st32(self, 0xf54ed4u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B25C, "SceneUtil::openContinuPopup $_13 __func::~__func (deleting)", t_48B25C);

// SceneUtil::openContinuPopup $_13: __func::__clone(__base*)
static void t_48B30C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54ed4u);
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
RW_FUNCTION(0x48B30C, "SceneUtil::openContinuPopup $_13 __func::__clone(__base*)", t_48B30C);

// SceneUtil::openContinuPopup $_13: __func::destroy
static void t_48B35C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x48B35C, "SceneUtil::openContinuPopup $_13 __func::destroy", t_48B35C);

// SceneUtil::openContinuPopup $_13: __func::destroy_deallocate
static void t_48B37C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B37C, "SceneUtil::openContinuPopup $_13 __func::destroy_deallocate", t_48B37C);

// SceneUtil::openContinuPopup $_13: __func::target
static uint32_t t_48B480(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6010u ? self + 8 : 0; }
RW_FUNCTION(0x48B480, "SceneUtil::openContinuPopup $_13 __func::target", t_48B480);

// SceneUtil::openContinuPopup $_13: __func::target_type
static uint32_t t_48B4A4() { return 0xf7dcf4u; }
RW_FUNCTION(0x48B4A4, "SceneUtil::openContinuPopup $_13 __func::target_type", t_48B4A4);

// SceneUtil::openContinuPopup $_136 UlvE_: __func::~__func (deleting)
static void t_48B4B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B4B8, "SceneUtil::openContinuPopup $_136 UlvE_ __func::~__func (deleting)", t_48B4B8);

// SceneUtil::openContinuPopup $_136 UlvE_: __func::__clone()
static uint32_t t_48B4C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf54f00u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x48B4C0, "SceneUtil::openContinuPopup $_136 UlvE_ __func::__clone()", t_48B4C0);

// SceneUtil::openContinuPopup $_136 UlvE_: __func::__clone(__base*)
static void t_48B4F0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54f00u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x48B4F0, "SceneUtil::openContinuPopup $_136 UlvE_ __func::__clone(__base*)", t_48B4F0);

// SceneUtil::openContinuPopup $_136 UlvE_: __func::destroy
static void t_48B518(uint32_t self) {
}
RW_FUNCTION(0x48B518, "SceneUtil::openContinuPopup $_136 UlvE_ __func::destroy", t_48B518);

// SceneUtil::openContinuPopup $_136 UlvE_: __func::destroy_deallocate
static void t_48B51A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B51A, "SceneUtil::openContinuPopup $_136 UlvE_ __func::destroy_deallocate", t_48B51A);

// SceneUtil::openContinuPopup $_136 UlvE_: __func::target
static uint32_t t_48B54C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf60d0u ? self + 4 : 0; }
RW_FUNCTION(0x48B54C, "SceneUtil::openContinuPopup $_136 UlvE_ __func::target", t_48B54C);

// SceneUtil::openContinuPopup $_136 UlvE_: __func::target_type
static uint32_t t_48B56C() { return 0xf7dd08u; }
RW_FUNCTION(0x48B56C, "SceneUtil::openContinuPopup $_136 UlvE_ __func::target_type", t_48B56C);

// SceneUtil::openContinuPopup $_12: __func::~__func (deleting)
static void t_48B580(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B580, "SceneUtil::openContinuPopup $_12 __func::~__func (deleting)", t_48B580);

// SceneUtil::openContinuPopup $_12: __func::__clone()
static uint32_t t_48B588(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf54f2cu);
    }
    return v1;
}
RW_FUNCTION(0x48B588, "SceneUtil::openContinuPopup $_12 __func::__clone()", t_48B588);

// SceneUtil::openContinuPopup $_12: __func::__clone(__base*)
static void t_48B5AC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54f2cu);
    }
}
RW_FUNCTION(0x48B5AC, "SceneUtil::openContinuPopup $_12 __func::__clone(__base*)", t_48B5AC);

// SceneUtil::openContinuPopup $_12: __func::destroy
static void t_48B5C8(uint32_t self) {
}
RW_FUNCTION(0x48B5C8, "SceneUtil::openContinuPopup $_12 __func::destroy", t_48B5C8);

// SceneUtil::openContinuPopup $_12: __func::destroy_deallocate
static void t_48B5CA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B5CA, "SceneUtil::openContinuPopup $_12 __func::destroy_deallocate", t_48B5CA);

// SceneUtil::openContinuPopup $_12: __func::target
static uint32_t t_48B5D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6190u ? self + 4 : 0; }
RW_FUNCTION(0x48B5D8, "SceneUtil::openContinuPopup $_12 __func::target", t_48B5D8);

// SceneUtil::openContinuPopup $_12: __func::target_type
static uint32_t t_48B5F8() { return 0xf7dd1cu; }
RW_FUNCTION(0x48B5F8, "SceneUtil::openContinuPopup $_12 __func::target_type", t_48B5F8);

// SceneUtil::openContinuPopup $_11: __func::~__func (deleting)
static void t_48B60C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B60C, "SceneUtil::openContinuPopup $_11 __func::~__func (deleting)", t_48B60C);

// SceneUtil::openContinuPopup $_11: __func::__clone()
static uint32_t t_48B614(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf54f58u);
    }
    return v1;
}
RW_FUNCTION(0x48B614, "SceneUtil::openContinuPopup $_11 __func::__clone()", t_48B614);

// SceneUtil::openContinuPopup $_11: __func::__clone(__base*)
static void t_48B638(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54f58u);
    }
}
RW_FUNCTION(0x48B638, "SceneUtil::openContinuPopup $_11 __func::__clone(__base*)", t_48B638);

// SceneUtil::openContinuPopup $_11: __func::destroy
static void t_48B654(uint32_t self) {
}
RW_FUNCTION(0x48B654, "SceneUtil::openContinuPopup $_11 __func::destroy", t_48B654);

// SceneUtil::openContinuPopup $_11: __func::destroy_deallocate
static void t_48B656(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B656, "SceneUtil::openContinuPopup $_11 __func::destroy_deallocate", t_48B656);

// SceneUtil::openContinuPopup $_11: __func::target
static uint32_t t_48B678(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6270u ? self + 4 : 0; }
RW_FUNCTION(0x48B678, "SceneUtil::openContinuPopup $_11 __func::target", t_48B678);

// SceneUtil::openContinuPopup $_11: __func::target_type
static uint32_t t_48B698() { return 0xf7dd30u; }
RW_FUNCTION(0x48B698, "SceneUtil::openContinuPopup $_11 __func::target_type", t_48B698);

// SceneUtil::openContinuPopup $_10: __func::~__func (deleting)
static void t_48B6AC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B6AC, "SceneUtil::openContinuPopup $_10 __func::~__func (deleting)", t_48B6AC);

// SceneUtil::openContinuPopup $_10: __func::__clone()
static uint32_t t_48B6B4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf54f84u);
    }
    return v1;
}
RW_FUNCTION(0x48B6B4, "SceneUtil::openContinuPopup $_10 __func::__clone()", t_48B6B4);

// SceneUtil::openContinuPopup $_10: __func::__clone(__base*)
static void t_48B6D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54f84u);
    }
}
RW_FUNCTION(0x48B6D8, "SceneUtil::openContinuPopup $_10 __func::__clone(__base*)", t_48B6D8);

// SceneUtil::openContinuPopup $_10: __func::destroy
static void t_48B6F4(uint32_t self) {
}
RW_FUNCTION(0x48B6F4, "SceneUtil::openContinuPopup $_10 __func::destroy", t_48B6F4);

// SceneUtil::openContinuPopup $_10: __func::destroy_deallocate
static void t_48B6F6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B6F6, "SceneUtil::openContinuPopup $_10 __func::destroy_deallocate", t_48B6F6);

// SceneUtil::openContinuPopup $_10: __func::target
static uint32_t t_48B718(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6350u ? self + 4 : 0; }
RW_FUNCTION(0x48B718, "SceneUtil::openContinuPopup $_10 __func::target", t_48B718);

// SceneUtil::openContinuPopup $_10: __func::target_type
static uint32_t t_48B738() { return 0xf7dd44u; }
RW_FUNCTION(0x48B738, "SceneUtil::openContinuPopup $_10 __func::target_type", t_48B738);

// SceneUtil::openContinueDialog $_9: __func::~__func
static uint32_t t_48B74C(uint32_t self) {
    st32(self, 0xf54fb0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48B74C, "SceneUtil::openContinueDialog $_9 __func::~__func", t_48B74C);

// SceneUtil::openContinueDialog $_9: __func::~__func (deleting)
static void t_48B788(uint32_t self) {
    st32(self, 0xf54fb0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B788, "SceneUtil::openContinueDialog $_9 __func::~__func (deleting)", t_48B788);

// SceneUtil::openContinueDialog $_9: __func::__clone()
static uint32_t t_48B7C8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf54fb0u);
        call(0x45cd55, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48B7C8, "SceneUtil::openContinueDialog $_9 __func::__clone()", t_48B7C8);

// SceneUtil::openContinueDialog $_9: __func::__clone(__base*)
static void t_48B818(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54fb0u);
        call(0x45cd55, {p + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48B818, "SceneUtil::openContinueDialog $_9 __func::__clone(__base*)", t_48B818);

// SceneUtil::openContinueDialog $_9: __func::destroy
static void t_48B84C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48B84C, "SceneUtil::openContinueDialog $_9 __func::destroy", t_48B84C);

// SceneUtil::openContinueDialog $_9: __func::destroy_deallocate
static void t_48B86C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B86C, "SceneUtil::openContinueDialog $_9 __func::destroy_deallocate", t_48B86C);

// SceneUtil::openContinueDialog $_9: __func::target
static uint32_t t_48B8B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6400u ? self + 8 : 0; }
RW_FUNCTION(0x48B8B4, "SceneUtil::openContinueDialog $_9 __func::target", t_48B8B4);

// SceneUtil::openContinueDialog $_9: __func::target_type
static uint32_t t_48B8D4() { return 0xf7dd58u; }
RW_FUNCTION(0x48B8D4, "SceneUtil::openContinueDialog $_9 __func::target_type", t_48B8D4);

// SceneUtil::openContinueDialog $_8: __func::~__func
static uint32_t t_48B8E8(uint32_t self) {
    st32(self, 0xf54fdcu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48B8E8, "SceneUtil::openContinueDialog $_8 __func::~__func", t_48B8E8);

// SceneUtil::openContinueDialog $_8: __func::~__func (deleting)
static void t_48B924(uint32_t self) {
    st32(self, 0xf54fdcu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48B924, "SceneUtil::openContinueDialog $_8 __func::~__func (deleting)", t_48B924);

// SceneUtil::openContinueDialog $_8: __func::__clone()
static uint32_t t_48B964(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf54fdcu);
        call(0x45cd55, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48B964, "SceneUtil::openContinueDialog $_8 __func::__clone()", t_48B964);

// SceneUtil::openContinueDialog $_8: __func::__clone(__base*)
static void t_48B9B4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf54fdcu);
        call(0x45cd55, {p + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48B9B4, "SceneUtil::openContinueDialog $_8 __func::__clone(__base*)", t_48B9B4);

// SceneUtil::openContinueDialog $_8: __func::destroy
static void t_48B9E8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48B9E8, "SceneUtil::openContinueDialog $_8 __func::destroy", t_48B9E8);

// SceneUtil::openContinueDialog $_8: __func::destroy_deallocate
static void t_48BA08(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48BA08, "SceneUtil::openContinueDialog $_8 __func::destroy_deallocate", t_48BA08);

// SceneUtil::openContinueDialog $_8: __func::target
static uint32_t t_48BBB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf64b0u ? self + 8 : 0; }
RW_FUNCTION(0x48BBB8, "SceneUtil::openContinueDialog $_8 __func::target", t_48BBB8);

// SceneUtil::openContinueDialog $_8: __func::target_type
static uint32_t t_48BBD8() { return 0xf7dd6cu; }
RW_FUNCTION(0x48BBD8, "SceneUtil::openContinueDialog $_8 __func::target_type", t_48BBD8);

// SceneUtil::openContinueDialog $_8 UlbE_: __func::~__func
static uint32_t t_48BBEC(uint32_t self) {
    st32(self, 0xf55008u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48BBEC, "SceneUtil::openContinueDialog $_8 UlbE_ __func::~__func", t_48BBEC);

// SceneUtil::openContinueDialog $_8 UlbE_: __func::~__func (deleting)
static void t_48BC28(uint32_t self) {
    st32(self, 0xf55008u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48BC28, "SceneUtil::openContinueDialog $_8 UlbE_ __func::~__func (deleting)", t_48BC28);

// SceneUtil::openContinueDialog $_8 UlbE_: __func::__clone()
static uint32_t t_48BC68(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf55008u);
        call(0x45cd55, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48BC68, "SceneUtil::openContinueDialog $_8 UlbE_ __func::__clone()", t_48BC68);

// SceneUtil::openContinueDialog $_8 UlbE_: __func::__clone(__base*)
static void t_48BCB8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55008u);
        call(0x45cd55, {p + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48BCB8, "SceneUtil::openContinueDialog $_8 UlbE_ __func::__clone(__base*)", t_48BCB8);

// SceneUtil::openContinueDialog $_8 UlbE_: __func::destroy
static void t_48BCEC(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48BCEC, "SceneUtil::openContinueDialog $_8 UlbE_ __func::destroy", t_48BCEC);

// SceneUtil::openContinueDialog $_8 UlbE_: __func::destroy_deallocate
static void t_48BD0C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48BD0C, "SceneUtil::openContinueDialog $_8 UlbE_ __func::destroy_deallocate", t_48BD0C);

// SceneUtil::openContinueDialog $_8 UlbE_: __func::target
static uint32_t t_48BDDC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6570u ? self + 8 : 0; }
RW_FUNCTION(0x48BDDC, "SceneUtil::openContinueDialog $_8 UlbE_ __func::target", t_48BDDC);

// SceneUtil::openContinueDialog $_8 UlbE_: __func::target_type
static uint32_t t_48BDFC() { return 0xf7dd80u; }
RW_FUNCTION(0x48BDFC, "SceneUtil::openContinueDialog $_8 UlbE_ __func::target_type", t_48BDFC);

// SceneUtil::openChargeApPopup $_7: __func::~__func
static uint32_t t_48BE10(uint32_t self) {
    st32(self, 0xf55034u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48BE10, "SceneUtil::openChargeApPopup $_7 __func::~__func", t_48BE10);

// SceneUtil::openChargeApPopup $_7: __func::~__func (deleting)
static void t_48BE4C(uint32_t self) {
    st32(self, 0xf55034u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48BE4C, "SceneUtil::openChargeApPopup $_7 __func::~__func (deleting)", t_48BE4C);

// SceneUtil::openChargeApPopup $_7: __func::__clone()
static uint32_t t_48BE8C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf55034u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 32 + 0);
        uint32_t v3 = ld32(self + 32 + 4);
        uint32_t v4 = ld32(self + 32 + 8);
        uint32_t v5 = ld32(self + 32 + 12);
        st32(v1 + 32 + 0, v2);
        st32(v1 + 32 + 4, v3);
        st32(v1 + 32 + 8, v4);
        st32(v1 + 32 + 12, v5);
        uint32_t v6 = ld32(self + 48);
        st32(v1 + 48, v6);
    }
    return v1;
}
RW_FUNCTION(0x48BE8C, "SceneUtil::openChargeApPopup $_7 __func::__clone()", t_48BE8C);

// SceneUtil::openChargeApPopup $_7: __func::__clone(__base*)
static void t_48BEEC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55034u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v1 = ld32(self + 32 + 0);
        uint32_t v2 = ld32(self + 32 + 4);
        uint32_t v3 = ld32(self + 32 + 8);
        uint32_t v4 = ld32(self + 32 + 12);
        st32(p + 32 + 0, v1);
        st32(p + 32 + 4, v2);
        st32(p + 32 + 8, v3);
        st32(p + 32 + 12, v4);
        uint32_t v5 = ld32(self + 48);
        st32(p + 48, v5);
    }
}
RW_FUNCTION(0x48BEEC, "SceneUtil::openChargeApPopup $_7 __func::__clone(__base*)", t_48BEEC);

// SceneUtil::openChargeApPopup $_7: __func::destroy
static void t_48BF34(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48BF34, "SceneUtil::openChargeApPopup $_7 __func::destroy", t_48BF34);

// SceneUtil::openChargeApPopup $_7: __func::destroy_deallocate
static void t_48BF54(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48BF54, "SceneUtil::openChargeApPopup $_7 __func::destroy_deallocate", t_48BF54);

// SceneUtil::openChargeApPopup $_7: __func::target
static uint32_t t_48C088(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6630u ? self + 8 : 0; }
RW_FUNCTION(0x48C088, "SceneUtil::openChargeApPopup $_7 __func::target", t_48C088);

// SceneUtil::openChargeApPopup $_7: __func::target_type
static uint32_t t_48C0A8() { return 0xf7dd94u; }
RW_FUNCTION(0x48C0A8, "SceneUtil::openChargeApPopup $_7 __func::target_type", t_48C0A8);

// SceneUtil::openChargeApPopup $_135 UlvE_: __func::~__func (deleting)
static void t_48C0BC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C0BC, "SceneUtil::openChargeApPopup $_135 UlvE_ __func::~__func (deleting)", t_48C0BC);

// SceneUtil::openChargeApPopup $_135 UlvE_: __func::__clone()
static uint32_t t_48C0C4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf55060u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x48C0C4, "SceneUtil::openChargeApPopup $_135 UlvE_ __func::__clone()", t_48C0C4);

// SceneUtil::openChargeApPopup $_135 UlvE_: __func::__clone(__base*)
static void t_48C0F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55060u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x48C0F4, "SceneUtil::openChargeApPopup $_135 UlvE_ __func::__clone(__base*)", t_48C0F4);

// SceneUtil::openChargeApPopup $_135 UlvE_: __func::destroy
static void t_48C11C(uint32_t self) {
}
RW_FUNCTION(0x48C11C, "SceneUtil::openChargeApPopup $_135 UlvE_ __func::destroy", t_48C11C);

// SceneUtil::openChargeApPopup $_135 UlvE_: __func::destroy_deallocate
static void t_48C11E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C11E, "SceneUtil::openChargeApPopup $_135 UlvE_ __func::destroy_deallocate", t_48C11E);

// SceneUtil::openChargeApPopup $_135 UlvE_: __func::target
static uint32_t t_48C15C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf66f0u ? self + 4 : 0; }
RW_FUNCTION(0x48C15C, "SceneUtil::openChargeApPopup $_135 UlvE_ __func::target", t_48C15C);

// SceneUtil::openChargeApPopup $_135 UlvE_: __func::target_type
static uint32_t t_48C17C() { return 0xf7dda8u; }
RW_FUNCTION(0x48C17C, "SceneUtil::openChargeApPopup $_135 UlvE_ __func::target_type", t_48C17C);

// SceneUtil::openChargeApPopup $_6: __func::~__func (deleting)
static void t_48C190(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C190, "SceneUtil::openChargeApPopup $_6 __func::~__func (deleting)", t_48C190);

// SceneUtil::openChargeApPopup $_6: __func::__clone()
static uint32_t t_48C198(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5508cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x48C198, "SceneUtil::openChargeApPopup $_6 __func::__clone()", t_48C198);

// SceneUtil::openChargeApPopup $_6: __func::__clone(__base*)
static void t_48C1C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5508cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x48C1C4, "SceneUtil::openChargeApPopup $_6 __func::__clone(__base*)", t_48C1C4);

// SceneUtil::openChargeApPopup $_6: __func::destroy
static void t_48C1E4(uint32_t self) {
}
RW_FUNCTION(0x48C1E4, "SceneUtil::openChargeApPopup $_6 __func::destroy", t_48C1E4);

// SceneUtil::openChargeApPopup $_6: __func::destroy_deallocate
static void t_48C1E6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C1E6, "SceneUtil::openChargeApPopup $_6 __func::destroy_deallocate", t_48C1E6);

// SceneUtil::openChargeApPopup $_6: __func::target
static uint32_t t_48C1F8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf67b0u ? self + 4 : 0; }
RW_FUNCTION(0x48C1F8, "SceneUtil::openChargeApPopup $_6 __func::target", t_48C1F8);

// SceneUtil::openChargeApPopup $_6: __func::target_type
static uint32_t t_48C218() { return 0xf7ddbcu; }
RW_FUNCTION(0x48C218, "SceneUtil::openChargeApPopup $_6 __func::target_type", t_48C218);

// SceneUtil::openChargeApPopup $_5: __func::~__func (deleting)
static void t_48C22C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C22C, "SceneUtil::openChargeApPopup $_5 __func::~__func (deleting)", t_48C22C);

// SceneUtil::openChargeApPopup $_5: __func::__clone()
static uint32_t t_48C234(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf550b8u);
    }
    return v1;
}
RW_FUNCTION(0x48C234, "SceneUtil::openChargeApPopup $_5 __func::__clone()", t_48C234);

// SceneUtil::openChargeApPopup $_5: __func::__clone(__base*)
static void t_48C258(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf550b8u);
    }
}
RW_FUNCTION(0x48C258, "SceneUtil::openChargeApPopup $_5 __func::__clone(__base*)", t_48C258);

// SceneUtil::openChargeApPopup $_5: __func::destroy
static void t_48C274(uint32_t self) {
}
RW_FUNCTION(0x48C274, "SceneUtil::openChargeApPopup $_5 __func::destroy", t_48C274);

// SceneUtil::openChargeApPopup $_5: __func::destroy_deallocate
static void t_48C276(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C276, "SceneUtil::openChargeApPopup $_5 __func::destroy_deallocate", t_48C276);

// SceneUtil::openChargeApPopup $_5: __func::target
static uint32_t t_48C294(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6890u ? self + 4 : 0; }
RW_FUNCTION(0x48C294, "SceneUtil::openChargeApPopup $_5 __func::target", t_48C294);

// SceneUtil::openChargeApPopup $_5: __func::target_type
static uint32_t t_48C2B4() { return 0xf7ddd0u; }
RW_FUNCTION(0x48C2B4, "SceneUtil::openChargeApPopup $_5 __func::target_type", t_48C2B4);

// SceneUtil::openChargeApPopup $_4: __func::~__func (deleting)
static void t_48C2C8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C2C8, "SceneUtil::openChargeApPopup $_4 __func::~__func (deleting)", t_48C2C8);

// SceneUtil::openChargeApPopup $_4: __func::__clone()
static uint32_t t_48C2D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf550e4u);
    }
    return v1;
}
RW_FUNCTION(0x48C2D0, "SceneUtil::openChargeApPopup $_4 __func::__clone()", t_48C2D0);

// SceneUtil::openChargeApPopup $_4: __func::__clone(__base*)
static void t_48C2F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf550e4u);
    }
}
RW_FUNCTION(0x48C2F4, "SceneUtil::openChargeApPopup $_4 __func::__clone(__base*)", t_48C2F4);

// SceneUtil::openChargeApPopup $_4: __func::destroy
static void t_48C310(uint32_t self) {
}
RW_FUNCTION(0x48C310, "SceneUtil::openChargeApPopup $_4 __func::destroy", t_48C310);

// SceneUtil::openChargeApPopup $_4: __func::destroy_deallocate
static void t_48C312(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C312, "SceneUtil::openChargeApPopup $_4 __func::destroy_deallocate", t_48C312);

// SceneUtil::openChargeApPopup $_4: __func::target
static uint32_t t_48C330(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6970u ? self + 4 : 0; }
RW_FUNCTION(0x48C330, "SceneUtil::openChargeApPopup $_4 __func::target", t_48C330);

// SceneUtil::openChargeApPopup $_4: __func::target_type
static uint32_t t_48C350() { return 0xf7dde4u; }
RW_FUNCTION(0x48C350, "SceneUtil::openChargeApPopup $_4 __func::target_type", t_48C350);

// SceneUtil::openChargeApDialog $_3: __func::~__func
static uint32_t t_48C364(uint32_t self) {
    st32(self, 0xf55110u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48C364, "SceneUtil::openChargeApDialog $_3 __func::~__func", t_48C364);

// SceneUtil::openChargeApDialog $_3: __func::~__func (deleting)
static void t_48C3A0(uint32_t self) {
    st32(self, 0xf55110u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C3A0, "SceneUtil::openChargeApDialog $_3 __func::~__func (deleting)", t_48C3A0);

// SceneUtil::openChargeApDialog $_3: __func::__clone()
static uint32_t t_48C3E0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf55110u);
        call(0x45cd55, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48C3E0, "SceneUtil::openChargeApDialog $_3 __func::__clone()", t_48C3E0);

// SceneUtil::openChargeApDialog $_3: __func::__clone(__base*)
static void t_48C430(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55110u);
        call(0x45cd55, {p + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48C430, "SceneUtil::openChargeApDialog $_3 __func::__clone(__base*)", t_48C430);

// SceneUtil::openChargeApDialog $_3: __func::destroy
static void t_48C464(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48C464, "SceneUtil::openChargeApDialog $_3 __func::destroy", t_48C464);

// SceneUtil::openChargeApDialog $_3: __func::destroy_deallocate
static void t_48C484(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C484, "SceneUtil::openChargeApDialog $_3 __func::destroy_deallocate", t_48C484);

// SceneUtil::openChargeApDialog $_3: __func::target
static uint32_t t_48C828(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6a20u ? self + 8 : 0; }
RW_FUNCTION(0x48C828, "SceneUtil::openChargeApDialog $_3 __func::target", t_48C828);

// SceneUtil::openChargeApDialog $_3: __func::target_type
static uint32_t t_48C848() { return 0xf7ddf8u; }
RW_FUNCTION(0x48C848, "SceneUtil::openChargeApDialog $_3 __func::target_type", t_48C848);

// SceneUtil::openChargeApDialog $_3 UlbE0_: __func::~__func
static uint32_t t_48C85C(uint32_t self) {
    st32(self, 0xf5513cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48C85C, "SceneUtil::openChargeApDialog $_3 UlbE0_ __func::~__func", t_48C85C);

// SceneUtil::openChargeApDialog $_3 UlbE0_: __func::~__func (deleting)
static void t_48C898(uint32_t self) {
    st32(self, 0xf5513cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C898, "SceneUtil::openChargeApDialog $_3 UlbE0_ __func::~__func (deleting)", t_48C898);

// SceneUtil::openChargeApDialog $_3 UlbE0_: __func::__clone()
static uint32_t t_48C8D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5513cu);
        call(0x45cd55, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48C8D8, "SceneUtil::openChargeApDialog $_3 UlbE0_ __func::__clone()", t_48C8D8);

// SceneUtil::openChargeApDialog $_3 UlbE0_: __func::__clone(__base*)
static void t_48C928(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5513cu);
        call(0x45cd55, {p + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48C928, "SceneUtil::openChargeApDialog $_3 UlbE0_ __func::__clone(__base*)", t_48C928);

// SceneUtil::openChargeApDialog $_3 UlbE0_: __func::destroy
static void t_48C95C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48C95C, "SceneUtil::openChargeApDialog $_3 UlbE0_ __func::destroy", t_48C95C);

// SceneUtil::openChargeApDialog $_3 UlbE0_: __func::destroy_deallocate
static void t_48C97C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48C97C, "SceneUtil::openChargeApDialog $_3 UlbE0_ __func::destroy_deallocate", t_48C97C);

// SceneUtil::openChargeApDialog $_3 UlbE0_: __func::target
static uint32_t t_48C9C4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6ae0u ? self + 8 : 0; }
RW_FUNCTION(0x48C9C4, "SceneUtil::openChargeApDialog $_3 UlbE0_ __func::target", t_48C9C4);

// SceneUtil::openChargeApDialog $_3 UlbE0_: __func::target_type
static uint32_t t_48C9E4() { return 0xf7de0cu; }
RW_FUNCTION(0x48C9E4, "SceneUtil::openChargeApDialog $_3 UlbE0_ __func::target_type", t_48C9E4);

// SceneUtil::openChargeApDialog $_3 UlbE_: __func::~__func
static uint32_t t_48C9F8(uint32_t self) {
    st32(self, 0xf55168u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48C9F8, "SceneUtil::openChargeApDialog $_3 UlbE_ __func::~__func", t_48C9F8);

// SceneUtil::openChargeApDialog $_3 UlbE_: __func::~__func (deleting)
static void t_48CA34(uint32_t self) {
    st32(self, 0xf55168u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48CA34, "SceneUtil::openChargeApDialog $_3 UlbE_ __func::~__func (deleting)", t_48CA34);

// SceneUtil::openChargeApDialog $_3 UlbE_: __func::__clone()
static uint32_t t_48CA74(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf55168u);
        call(0x45cd55, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48CA74, "SceneUtil::openChargeApDialog $_3 UlbE_ __func::__clone()", t_48CA74);

// SceneUtil::openChargeApDialog $_3 UlbE_: __func::__clone(__base*)
static void t_48CAC4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55168u);
        call(0x45cd55, {p + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48CAC4, "SceneUtil::openChargeApDialog $_3 UlbE_ __func::__clone(__base*)", t_48CAC4);

// SceneUtil::openChargeApDialog $_3 UlbE_: __func::destroy
static void t_48CAF8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48CAF8, "SceneUtil::openChargeApDialog $_3 UlbE_ __func::destroy", t_48CAF8);

// SceneUtil::openChargeApDialog $_3 UlbE_: __func::destroy_deallocate
static void t_48CB18(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48CB18, "SceneUtil::openChargeApDialog $_3 UlbE_ __func::destroy_deallocate", t_48CB18);

// SceneUtil::openChargeApDialog $_3 UlbE_: __func::target
static uint32_t t_48CD30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6bb0u ? self + 8 : 0; }
RW_FUNCTION(0x48CD30, "SceneUtil::openChargeApDialog $_3 UlbE_ __func::target", t_48CD30);

// SceneUtil::openChargeApDialog $_3 UlbE_: __func::target_type
static uint32_t t_48CD50() { return 0xf7de20u; }
RW_FUNCTION(0x48CD50, "SceneUtil::openChargeApDialog $_3 UlbE_ __func::target_type", t_48CD50);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_: __func::~__func
static uint32_t t_48CD64(uint32_t self) {
    st32(self, 0xf55194u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48CD64, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_ __func::~__func", t_48CD64);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_: __func::~__func (deleting)
static void t_48CDA0(uint32_t self) {
    st32(self, 0xf55194u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48CDA0, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_ __func::~__func (deleting)", t_48CDA0);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_: __func::__clone()
static uint32_t t_48CDE0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf55194u);
        call(0x45cd55, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x48CDE0, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_ __func::__clone()", t_48CDE0);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_: __func::__clone(__base*)
static void t_48CE30(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55194u);
        call(0x45cd55, {p + 8, self + 8});   // _ZNSt3__18functionIFvbbEEC2ERKS2_
    }
}
RW_FUNCTION(0x48CE30, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_ __func::__clone(__base*)", t_48CE30);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_: __func::destroy
static void t_48CE64(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x48CE64, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_ __func::destroy", t_48CE64);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_: __func::destroy_deallocate
static void t_48CE84(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48CE84, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_ __func::destroy_deallocate", t_48CE84);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_: __func::target
static uint32_t t_48CEC0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6c90u ? self + 8 : 0; }
RW_FUNCTION(0x48CEC0, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_ __func::target", t_48CEC0);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_: __func::target_type
static uint32_t t_48CEE0() { return 0xf7de34u; }
RW_FUNCTION(0x48CEE0, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE0_ __func::target_type", t_48CEE0);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_: __func::~__func (deleting)
static void t_48CEF4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48CEF4, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_ __func::~__func (deleting)", t_48CEF4);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_: __func::__clone()
static uint32_t t_48CEFC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf551c0u);
    }
    return v1;
}
RW_FUNCTION(0x48CEFC, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_ __func::__clone()", t_48CEFC);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_: __func::__clone(__base*)
static void t_48CF20(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf551c0u);
    }
}
RW_FUNCTION(0x48CF20, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_ __func::__clone(__base*)", t_48CF20);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_: __func::destroy
static void t_48CF3C(uint32_t self) {
}
RW_FUNCTION(0x48CF3C, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_ __func::destroy", t_48CF3C);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_: __func::destroy_deallocate
static void t_48CF3E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48CF3E, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_ __func::destroy_deallocate", t_48CF3E);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_: __func::target
static uint32_t t_48CF48(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6d80u ? self + 4 : 0; }
RW_FUNCTION(0x48CF48, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_ __func::target", t_48CF48);

// SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_: __func::target_type
static uint32_t t_48CF68() { return 0xf7de48u; }
RW_FUNCTION(0x48CF68, "SceneUtil::openChargeApDialog $_3 UlbE_ UlbE_ __func::target_type", t_48CF68);

// SceneUtil::editBoxEditingDidEnd $_2: __func::~__func (deleting)
static void t_48CF7C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48CF7C, "SceneUtil::editBoxEditingDidEnd $_2 __func::~__func (deleting)", t_48CF7C);

// SceneUtil::editBoxEditingDidEnd $_2: __func::__clone()
static uint32_t t_48CF84(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf551ecu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x48CF84, "SceneUtil::editBoxEditingDidEnd $_2 __func::__clone()", t_48CF84);

// SceneUtil::editBoxEditingDidEnd $_2: __func::__clone(__base*)
static void t_48CFB0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf551ecu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x48CFB0, "SceneUtil::editBoxEditingDidEnd $_2 __func::__clone(__base*)", t_48CFB0);

// SceneUtil::editBoxEditingDidEnd $_2: __func::destroy
static void t_48CFD0(uint32_t self) {
}
RW_FUNCTION(0x48CFD0, "SceneUtil::editBoxEditingDidEnd $_2 __func::destroy", t_48CFD0);

// SceneUtil::editBoxEditingDidEnd $_2: __func::destroy_deallocate
static void t_48CFD2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48CFD2, "SceneUtil::editBoxEditingDidEnd $_2 __func::destroy_deallocate", t_48CFD2);

// SceneUtil::editBoxEditingDidEnd $_2: __func::target
static uint32_t t_48D008(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6e60u ? self + 4 : 0; }
RW_FUNCTION(0x48D008, "SceneUtil::editBoxEditingDidEnd $_2 __func::target", t_48D008);

// SceneUtil::editBoxEditingDidEnd $_2: __func::target_type
static uint32_t t_48D028() { return 0xf7de5cu; }
RW_FUNCTION(0x48D028, "SceneUtil::editBoxEditingDidEnd $_2 __func::target_type", t_48D028);

// SceneUtil::textFieldToEditBox : __func::~__func
static uint32_t t_48D03C(uint32_t self) {
    st32(self, 0xf55218u);
    uint32_t v1 = ld8(self + 20);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 28);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x48D03C, "SceneUtil::textFieldToEditBox  __func::~__func", t_48D03C);

// SceneUtil::textFieldToEditBox : __func::~__func (deleting)
static void t_48D06C(uint32_t self) {
    st32(self, 0xf55218u);
    uint32_t v1 = ld8(self + 20);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 28);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48D06C, "SceneUtil::textFieldToEditBox  __func::~__func (deleting)", t_48D06C);

// SceneUtil::textFieldToEditBox : __func::destroy
static void t_48D178(uint32_t self) {
    uint32_t v1 = ld8(self + 20);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 28);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x48D178, "SceneUtil::textFieldToEditBox  __func::destroy", t_48D178);

// SceneUtil::textFieldToEditBox : __func::destroy_deallocate
static void t_48D18A(uint32_t self) {
    uint32_t v1 = ld8(self + 20);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 28);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48D18A, "SceneUtil::textFieldToEditBox  __func::destroy_deallocate", t_48D18A);

// SceneUtil::textFieldToEditBox : __func::target
static uint32_t t_48D590(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf6fc0u ? self + 4 : 0; }
RW_FUNCTION(0x48D590, "SceneUtil::textFieldToEditBox  __func::target", t_48D590);

// SceneUtil::textFieldToEditBox : __func::target_type
static uint32_t t_48D5B0() { return 0xf7de70u; }
RW_FUNCTION(0x48D5B0, "SceneUtil::textFieldToEditBox  __func::target_type", t_48D5B0);

// SceneUtil::TutorialMask $_67 (shared): __func::~__func
static uint32_t t_48D5C4(uint32_t self) {
    return self;
}
RW_FUNCTION(0x48D5C4, "SceneUtil::TutorialMask $_67 (shared) __func::~__func", t_48D5C4);

// SceneUtil::textFieldToEditBox : __func::~__func (deleting)
static void t_48D5C6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48D5C6, "SceneUtil::textFieldToEditBox  __func::~__func (deleting)", t_48D5C6);

// SceneUtil::textFieldToEditBox : __func::__clone()
static uint32_t t_48D5D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf55244u);
    }
    return v1;
}
RW_FUNCTION(0x48D5D0, "SceneUtil::textFieldToEditBox  __func::__clone()", t_48D5D0);

// SceneUtil::textFieldToEditBox : __func::__clone(__base*)
static void t_48D5F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55244u);
    }
}
RW_FUNCTION(0x48D5F4, "SceneUtil::textFieldToEditBox  __func::__clone(__base*)", t_48D5F4);

// SceneUtil::textFieldToEditBox : __func::destroy
static void t_48D610(uint32_t self) {
}
RW_FUNCTION(0x48D610, "SceneUtil::textFieldToEditBox  __func::destroy", t_48D610);

// SceneUtil::textFieldToEditBox : __func::destroy_deallocate
static void t_48D612(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48D612, "SceneUtil::textFieldToEditBox  __func::destroy_deallocate", t_48D612);

// SceneUtil::textFieldToEditBox : __func::target
static uint32_t t_48D620(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf71d0u ? self + 4 : 0; }
RW_FUNCTION(0x48D620, "SceneUtil::textFieldToEditBox  __func::target", t_48D620);

// SceneUtil::textFieldToEditBox : __func::target_type
static uint32_t t_48D640() { return 0xf7de84u; }
RW_FUNCTION(0x48D640, "SceneUtil::textFieldToEditBox  __func::target_type", t_48D640);

// SceneUtil::createShadowSprite $_0: __func::~__func (deleting)
static void t_48D654(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48D654, "SceneUtil::createShadowSprite $_0 __func::~__func (deleting)", t_48D654);

// SceneUtil::createShadowSprite $_0: __func::__clone()
static uint32_t t_48D65C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf55270u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x48D65C, "SceneUtil::createShadowSprite $_0 __func::__clone()", t_48D65C);

// SceneUtil::createShadowSprite $_0: __func::__clone(__base*)
static void t_48D688(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55270u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x48D688, "SceneUtil::createShadowSprite $_0 __func::__clone(__base*)", t_48D688);

// SceneUtil::createShadowSprite $_0: __func::destroy
static void t_48D6A8(uint32_t self) {
}
RW_FUNCTION(0x48D6A8, "SceneUtil::createShadowSprite $_0 __func::destroy", t_48D6A8);

// SceneUtil::createShadowSprite $_0: __func::destroy_deallocate
static void t_48D6AA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48D6AA, "SceneUtil::createShadowSprite $_0 __func::destroy_deallocate", t_48D6AA);

// SceneUtil::createShadowSprite $_0: __func::target
static uint32_t t_48D6D0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xdf7350u ? self + 4 : 0; }
RW_FUNCTION(0x48D6D0, "SceneUtil::createShadowSprite $_0 __func::target", t_48D6D0);

// SceneUtil::createShadowSprite $_0: __func::target_type
static uint32_t t_48D6F0() { return 0xf7de98u; }
RW_FUNCTION(0x48D6F0, "SceneUtil::createShadowSprite $_0 __func::target_type", t_48D6F0);

// SceneUtil::callPrepareMypageAPIs $_64: __func::~__func
static uint32_t t_48DB90(uint32_t self) {
    st32(self, 0xf5529cu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x48DB90, "SceneUtil::callPrepareMypageAPIs $_64 __func::~__func", t_48DB90);

// SceneUtil::callPrepareMypageAPIs $_64: __func::~__func (deleting)
static void t_48DBE8(uint32_t self) {
    st32(self, 0xf5529cu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x48DBE8, "SceneUtil::callPrepareMypageAPIs $_64 __func::~__func (deleting)", t_48DBE8);

// ---- written by hand (shapes the generator does not cover)

// SceneUtil::openContinuPopup $_13: __func::__clone() - captures: 3 words at +8, std::function<void(bool)> at
// +0x18 (the original copies the 3 words before and again after copying the function)
static uint32_t t_48B29C(uint32_t self) {
    uint32_t obj = call(fn::operator_new, {0x30});
    if (obj) {
        st32(obj, 0xf54ed4u);
        uint32_t w0 = ld32(self + 8), w1 = ld32(self + 12), w2 = ld32(self + 16);
        st32(obj + 8, w0);
        st32(obj + 12, w1);
        st32(obj + 16, w2);
        call(fn::function_vb_copy, {obj + 0x18, self + 0x18});   // std::function<void(bool)>(const function&)
        w0 = ld32(self + 8), w1 = ld32(self + 12), w2 = ld32(self + 16);
        st32(obj + 8, w0);
        st32(obj + 12, w1);
        st32(obj + 16, w2);
    }
    return obj;
}
RW_FUNCTION(0x48B29C, "SceneUtil::openContinuPopup $_13 __func::__clone()", t_48B29C);

// copy of a captured std::string (libc++ inline copy constructor): a long string through __init, a short one as
// its three words
static void copyCapturedString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) {
        call(fn::string_init, {dst, ld32(src + 8), ld32(src + 4)});
    } else {
        uint32_t w0 = ld32(src), w1 = ld32(src + 4), w2 = ld32(src + 8);
        st32(dst, w0);
        st32(dst + 4, w1);
        st32(dst + 8, w2);
    }
}

// SceneUtil::textFieldToEditBox lambda: __func::__clone() - captures: 4 words at +4, std::string at +0x14,
// 2 words at +0x20
static uint32_t t_48D0A0(uint32_t self) {
    uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, 0xf55218u);
        uint32_t w0 = ld32(self + 4), w1 = ld32(self + 8), w2 = ld32(self + 12), w3 = ld32(self + 16);
        st32(obj + 16, w3);
        st32(obj + 12, w2);
        st32(obj + 8, w1);
        st32(obj + 4, w0);
        copyCapturedString(obj + 0x14, self + 0x14);
        uint32_t a = ld32(self + 0x20), b = ld32(self + 0x24);
        st32(obj + 0x20, a);
        st32(obj + 0x24, b);
    }
    return obj;
}
RW_FUNCTION(0x48D0A0, "SceneUtil::textFieldToEditBox lambda __func::__clone()", t_48D0A0);

// SceneUtil::textFieldToEditBox lambda: __func::__clone(__base*)
static void t_48D118(uint32_t self, uint32_t p) {
    if (!p) return;
    st32(p, 0xf55218u);
    uint32_t w0 = ld32(self + 4), w1 = ld32(self + 8), w2 = ld32(self + 12), w3 = ld32(self + 16);
    st32(p + 4, w0);
    st32(p + 8, w1);
    st32(p + 12, w2);
    st32(p + 16, w3);
    copyCapturedString(p + 0x14, self + 0x14);
    uint32_t a = ld32(self + 0x20), b = ld32(self + 0x24);
    st32(p + 0x20, a);
    st32(p + 0x24, b);
}
RW_FUNCTION(0x48D118, "SceneUtil::textFieldToEditBox lambda __func::__clone(__base*)", t_48D118);

// SceneUtil::callPrepareMypageAPIs $_64: __func::__clone() - two captured std::function<void(bool)> (+8, +0x20)
static uint32_t t_48DC44(uint32_t self) {
    uint32_t obj = call(fn::operator_new, {0x38});
    if (obj) {
        st32(obj, 0xf5529cu);
        call(fn::function_vb_copy, {obj + 8, self + 8});
        call(fn::function_vb_copy, {obj + 0x20, self + 0x20});
    }
    return obj;
}
RW_FUNCTION(0x48DC44, "SceneUtil::callPrepareMypageAPIs $_64 __func::__clone()", t_48DC44);
