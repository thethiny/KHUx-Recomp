// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// OtherMenuDialog lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// OtherMenuDialog::openMoviePopup $_65: __func::~__func (deleting)
static void t_5A3620(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3620, "OtherMenuDialog::openMoviePopup $_65 __func::~__func (deleting)", t_5A3620);

// OtherMenuDialog::openMoviePopup $_65: __func::__clone()
static uint32_t t_5A3628(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b380u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A3628, "OtherMenuDialog::openMoviePopup $_65 __func::__clone()", t_5A3628);

// OtherMenuDialog::openMoviePopup $_65: __func::__clone(__base*)
static void t_5A3654(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b380u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A3654, "OtherMenuDialog::openMoviePopup $_65 __func::__clone(__base*)", t_5A3654);

// OtherMenuDialog::openMoviePopup $_65: __func::destroy
static void t_5A3674(uint32_t self) {
}
RW_FUNCTION(0x5A3674, "OtherMenuDialog::openMoviePopup $_65 __func::destroy", t_5A3674);

// OtherMenuDialog::openMoviePopup $_65: __func::destroy_deallocate
static void t_5A3676(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3676, "OtherMenuDialog::openMoviePopup $_65 __func::destroy_deallocate", t_5A3676);

// OtherMenuDialog::openMoviePopup $_65: __func::target
static uint32_t t_5A375C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe40c60u ? self + 4 : 0; }
RW_FUNCTION(0x5A375C, "OtherMenuDialog::openMoviePopup $_65 __func::target", t_5A375C);

// OtherMenuDialog::openMoviePopup $_65: __func::target_type
static uint32_t t_5A377C() { return 0xf8d46cu; }
RW_FUNCTION(0x5A377C, "OtherMenuDialog::openMoviePopup $_65 __func::target_type", t_5A377C);

// OtherMenuDialog::openMoviePopup $_64 (shared): __func::~__func
static uint32_t t_5A3790(uint32_t self) {
    return self;
}
RW_FUNCTION(0x5A3790, "OtherMenuDialog::openMoviePopup $_64 (shared) __func::~__func", t_5A3790);

// OtherMenuDialog::openMoviePopup $_64: __func::~__func (deleting)
static void t_5A3792(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3792, "OtherMenuDialog::openMoviePopup $_64 __func::~__func (deleting)", t_5A3792);

// OtherMenuDialog::openMoviePopup $_64: __func::__clone()
static uint32_t t_5A379C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5b3acu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x5A379C, "OtherMenuDialog::openMoviePopup $_64 __func::__clone()", t_5A379C);

// OtherMenuDialog::openMoviePopup $_64: __func::__clone(__base*)
static void t_5A37CC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b3acu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A37CC, "OtherMenuDialog::openMoviePopup $_64 __func::__clone(__base*)", t_5A37CC);

// OtherMenuDialog::openMoviePopup $_64: __func::destroy
static void t_5A37F4(uint32_t self) {
}
RW_FUNCTION(0x5A37F4, "OtherMenuDialog::openMoviePopup $_64 __func::destroy", t_5A37F4);

// OtherMenuDialog::openMoviePopup $_64: __func::destroy_deallocate
static void t_5A37F6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A37F6, "OtherMenuDialog::openMoviePopup $_64 __func::destroy_deallocate", t_5A37F6);

// OtherMenuDialog::openMoviePopup $_64: __func::target
static uint32_t t_5A3870(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe40d30u ? self + 4 : 0; }
RW_FUNCTION(0x5A3870, "OtherMenuDialog::openMoviePopup $_64 __func::target", t_5A3870);

// OtherMenuDialog::openMoviePopup $_64: __func::target_type
static uint32_t t_5A3890() { return 0xf8d480u; }
RW_FUNCTION(0x5A3890, "OtherMenuDialog::openMoviePopup $_64 __func::target_type", t_5A3890);

// OtherMenuDialog::openMoviePopup $_63: __func::~__func (deleting)
static void t_5A38A4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A38A4, "OtherMenuDialog::openMoviePopup $_63 __func::~__func (deleting)", t_5A38A4);

// OtherMenuDialog::openMoviePopup $_63: __func::__clone()
static uint32_t t_5A38AC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5b3d8u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A38AC, "OtherMenuDialog::openMoviePopup $_63 __func::__clone()", t_5A38AC);

// OtherMenuDialog::openMoviePopup $_63: __func::__clone(__base*)
static void t_5A38DC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b3d8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A38DC, "OtherMenuDialog::openMoviePopup $_63 __func::__clone(__base*)", t_5A38DC);

// OtherMenuDialog::openMoviePopup $_63: __func::destroy
static void t_5A3900(uint32_t self) {
}
RW_FUNCTION(0x5A3900, "OtherMenuDialog::openMoviePopup $_63 __func::destroy", t_5A3900);

// OtherMenuDialog::openMoviePopup $_63: __func::destroy_deallocate
static void t_5A3902(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3902, "OtherMenuDialog::openMoviePopup $_63 __func::destroy_deallocate", t_5A3902);

// OtherMenuDialog::openMoviePopup $_63: __func::target
static uint32_t t_5A3968(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe40df0u ? self + 4 : 0; }
RW_FUNCTION(0x5A3968, "OtherMenuDialog::openMoviePopup $_63 __func::target", t_5A3968);

// OtherMenuDialog::openMoviePopup $_63: __func::target_type
static uint32_t t_5A3988() { return 0xf8d494u; }
RW_FUNCTION(0x5A3988, "OtherMenuDialog::openMoviePopup $_63 __func::target_type", t_5A3988);

// OtherMenuDialog::openInputSuccessUserCodePopup $_62: __func::~__func (deleting)
static void t_5A399C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A399C, "OtherMenuDialog::openInputSuccessUserCodePopup $_62 __func::~__func (deleting)", t_5A399C);

// OtherMenuDialog::openInputSuccessUserCodePopup $_62: __func::__clone()
static uint32_t t_5A39A4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b404u);
    }
    return v1;
}
RW_FUNCTION(0x5A39A4, "OtherMenuDialog::openInputSuccessUserCodePopup $_62 __func::__clone()", t_5A39A4);

// OtherMenuDialog::openInputSuccessUserCodePopup $_62: __func::__clone(__base*)
static void t_5A39C8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b404u);
    }
}
RW_FUNCTION(0x5A39C8, "OtherMenuDialog::openInputSuccessUserCodePopup $_62 __func::__clone(__base*)", t_5A39C8);

// OtherMenuDialog::openInputSuccessUserCodePopup $_62: __func::destroy
static void t_5A39E4(uint32_t self) {
}
RW_FUNCTION(0x5A39E4, "OtherMenuDialog::openInputSuccessUserCodePopup $_62 __func::destroy", t_5A39E4);

// OtherMenuDialog::openInputSuccessUserCodePopup $_62: __func::destroy_deallocate
static void t_5A39E6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A39E6, "OtherMenuDialog::openInputSuccessUserCodePopup $_62 __func::destroy_deallocate", t_5A39E6);

// OtherMenuDialog::openInputSuccessUserCodePopup $_62: __func::target
static uint32_t t_5A39F0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe40ea0u ? self + 4 : 0; }
RW_FUNCTION(0x5A39F0, "OtherMenuDialog::openInputSuccessUserCodePopup $_62 __func::target", t_5A39F0);

// OtherMenuDialog::openInputSuccessUserCodePopup $_62: __func::target_type
static uint32_t t_5A3A10() { return 0xf8d4a8u; }
RW_FUNCTION(0x5A3A10, "OtherMenuDialog::openInputSuccessUserCodePopup $_62 __func::target_type", t_5A3A10);

// OtherMenuDialog::openInputSuccessUserCodePopup $_61: __func::~__func
static uint32_t t_5A3A24(uint32_t self) {
    st32(self, 0xf5b430u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A3A24, "OtherMenuDialog::openInputSuccessUserCodePopup $_61 __func::~__func", t_5A3A24);

// OtherMenuDialog::openInputSuccessUserCodePopup $_61: __func::~__func (deleting)
static void t_5A3A60(uint32_t self) {
    st32(self, 0xf5b430u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3A60, "OtherMenuDialog::openInputSuccessUserCodePopup $_61 __func::~__func (deleting)", t_5A3A60);

// OtherMenuDialog::openInputSuccessUserCodePopup $_61: __func::__clone()
static uint32_t t_5A3AA0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b430u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A3AA0, "OtherMenuDialog::openInputSuccessUserCodePopup $_61 __func::__clone()", t_5A3AA0);

// OtherMenuDialog::openInputSuccessUserCodePopup $_61: __func::__clone(__base*)
static void t_5A3AFC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b430u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A3AFC, "OtherMenuDialog::openInputSuccessUserCodePopup $_61 __func::__clone(__base*)", t_5A3AFC);

// OtherMenuDialog::openInputSuccessUserCodePopup $_61: __func::destroy
static void t_5A3B3C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A3B3C, "OtherMenuDialog::openInputSuccessUserCodePopup $_61 __func::destroy", t_5A3B3C);

// OtherMenuDialog::openInputSuccessUserCodePopup $_61: __func::destroy_deallocate
static void t_5A3B5C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3B5C, "OtherMenuDialog::openInputSuccessUserCodePopup $_61 __func::destroy_deallocate", t_5A3B5C);

// OtherMenuDialog::openInputSuccessUserCodePopup $_61: __func::target
static uint32_t t_5A3CD4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe40fb0u ? self + 8 : 0; }
RW_FUNCTION(0x5A3CD4, "OtherMenuDialog::openInputSuccessUserCodePopup $_61 __func::target", t_5A3CD4);

// OtherMenuDialog::openInputSuccessUserCodePopup $_61: __func::target_type
static uint32_t t_5A3CF8() { return 0xf8d4bcu; }
RW_FUNCTION(0x5A3CF8, "OtherMenuDialog::openInputSuccessUserCodePopup $_61 __func::target_type", t_5A3CF8);

// OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_: __func::~__func
static uint32_t t_5A3D0C(uint32_t self) {
    st32(self, 0xf5b45cu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A3D0C, "OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_ __func::~__func", t_5A3D0C);

// OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_: __func::~__func (deleting)
static void t_5A3D48(uint32_t self) {
    st32(self, 0xf5b45cu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3D48, "OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_ __func::~__func (deleting)", t_5A3D48);

// OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_: __func::__clone()
static uint32_t t_5A3D88(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b45cu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A3D88, "OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_ __func::__clone()", t_5A3D88);

// OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_: __func::__clone(__base*)
static void t_5A3DE4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b45cu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A3DE4, "OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_ __func::__clone(__base*)", t_5A3DE4);

// OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_: __func::destroy
static void t_5A3E24(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A3E24, "OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_ __func::destroy", t_5A3E24);

// OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_: __func::destroy_deallocate
static void t_5A3E44(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3E44, "OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_ __func::destroy_deallocate", t_5A3E44);

// OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_: __func::target
static uint32_t t_5A3EAC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe410a0u ? self + 8 : 0; }
RW_FUNCTION(0x5A3EAC, "OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_ __func::target", t_5A3EAC);

// OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_: __func::target_type
static uint32_t t_5A3ED0() { return 0xf8d4d0u; }
RW_FUNCTION(0x5A3ED0, "OtherMenuDialog::openInputSuccessUserCodePopup $_60 UlvE_ __func::target_type", t_5A3ED0);

// OtherMenuDialog::openInputUserCodePopup $_59: __func::~__func (deleting)
static void t_5A3EE4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3EE4, "OtherMenuDialog::openInputUserCodePopup $_59 __func::~__func (deleting)", t_5A3EE4);

// OtherMenuDialog::openInputUserCodePopup $_59: __func::__clone()
static uint32_t t_5A3EEC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b488u);
    }
    return v1;
}
RW_FUNCTION(0x5A3EEC, "OtherMenuDialog::openInputUserCodePopup $_59 __func::__clone()", t_5A3EEC);

// OtherMenuDialog::openInputUserCodePopup $_59: __func::__clone(__base*)
static void t_5A3F10(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b488u);
    }
}
RW_FUNCTION(0x5A3F10, "OtherMenuDialog::openInputUserCodePopup $_59 __func::__clone(__base*)", t_5A3F10);

// OtherMenuDialog::openInputUserCodePopup $_59: __func::destroy
static void t_5A3F2C(uint32_t self) {
}
RW_FUNCTION(0x5A3F2C, "OtherMenuDialog::openInputUserCodePopup $_59 __func::destroy", t_5A3F2C);

// OtherMenuDialog::openInputUserCodePopup $_59: __func::destroy_deallocate
static void t_5A3F2E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3F2E, "OtherMenuDialog::openInputUserCodePopup $_59 __func::destroy_deallocate", t_5A3F2E);

// OtherMenuDialog::openInputUserCodePopup $_59: __func::target
static uint32_t t_5A3F38(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41180u ? self + 4 : 0; }
RW_FUNCTION(0x5A3F38, "OtherMenuDialog::openInputUserCodePopup $_59 __func::target", t_5A3F38);

// OtherMenuDialog::openInputUserCodePopup $_59: __func::target_type
static uint32_t t_5A3F58() { return 0xf8d4e4u; }
RW_FUNCTION(0x5A3F58, "OtherMenuDialog::openInputUserCodePopup $_59 __func::target_type", t_5A3F58);

// OtherMenuDialog::openInputUserCodePopup $_58: __func::~__func
static uint32_t t_5A3F6C(uint32_t self) {
    st32(self, 0xf5b4b4u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A3F6C, "OtherMenuDialog::openInputUserCodePopup $_58 __func::~__func", t_5A3F6C);

// OtherMenuDialog::openInputUserCodePopup $_58: __func::~__func (deleting)
static void t_5A3FA8(uint32_t self) {
    st32(self, 0xf5b4b4u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A3FA8, "OtherMenuDialog::openInputUserCodePopup $_58 __func::~__func (deleting)", t_5A3FA8);

// OtherMenuDialog::openInputUserCodePopup $_58: __func::__clone()
static uint32_t t_5A3FE8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf5b4b4u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v5 = ld32(self + 48);
        st32(v1 + 48, v5);
    }
    return v1;
}
RW_FUNCTION(0x5A3FE8, "OtherMenuDialog::openInputUserCodePopup $_58 __func::__clone()", t_5A3FE8);

// OtherMenuDialog::openInputUserCodePopup $_58: __func::__clone(__base*)
static void t_5A4048(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b4b4u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v4 = ld32(self + 48);
        st32(p + 48, v4);
    }
}
RW_FUNCTION(0x5A4048, "OtherMenuDialog::openInputUserCodePopup $_58 __func::__clone(__base*)", t_5A4048);

// OtherMenuDialog::openInputUserCodePopup $_58: __func::destroy
static void t_5A4090(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A4090, "OtherMenuDialog::openInputUserCodePopup $_58 __func::destroy", t_5A4090);

// OtherMenuDialog::openInputUserCodePopup $_58: __func::destroy_deallocate
static void t_5A40B0(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A40B0, "OtherMenuDialog::openInputUserCodePopup $_58 __func::destroy_deallocate", t_5A40B0);

// OtherMenuDialog::openInputUserCodePopup $_58: __func::target
static uint32_t t_5A4198(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41250u ? self + 8 : 0; }
RW_FUNCTION(0x5A4198, "OtherMenuDialog::openInputUserCodePopup $_58 __func::target", t_5A4198);

// OtherMenuDialog::openInputUserCodePopup $_58: __func::target_type
static uint32_t t_5A41BC() { return 0xf8d4f8u; }
RW_FUNCTION(0x5A41BC, "OtherMenuDialog::openInputUserCodePopup $_58 __func::target_type", t_5A41BC);

// OtherMenuDialog::openInputUserCodePopup $_56 UlvE_: __func::~__func
static uint32_t t_5A4318(uint32_t self) {
    st32(self, 0xf5b4e0u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A4318, "OtherMenuDialog::openInputUserCodePopup $_56 UlvE_ __func::~__func", t_5A4318);

// OtherMenuDialog::openInputUserCodePopup $_56 UlvE_: __func::~__func (deleting)
static void t_5A4354(uint32_t self) {
    st32(self, 0xf5b4e0u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4354, "OtherMenuDialog::openInputUserCodePopup $_56 UlvE_ __func::~__func (deleting)", t_5A4354);

// OtherMenuDialog::openInputUserCodePopup $_56 UlvE_: __func::__clone()
static uint32_t t_5A4394(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b4e0u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A4394, "OtherMenuDialog::openInputUserCodePopup $_56 UlvE_ __func::__clone()", t_5A4394);

// OtherMenuDialog::openInputUserCodePopup $_56 UlvE_: __func::__clone(__base*)
static void t_5A43F0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b4e0u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A43F0, "OtherMenuDialog::openInputUserCodePopup $_56 UlvE_ __func::__clone(__base*)", t_5A43F0);

// OtherMenuDialog::openInputUserCodePopup $_56 UlvE_: __func::destroy
static void t_5A4430(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A4430, "OtherMenuDialog::openInputUserCodePopup $_56 UlvE_ __func::destroy", t_5A4430);

// OtherMenuDialog::openInputUserCodePopup $_56 UlvE_: __func::destroy_deallocate
static void t_5A4450(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4450, "OtherMenuDialog::openInputUserCodePopup $_56 UlvE_ __func::destroy_deallocate", t_5A4450);

// OtherMenuDialog::openInputUserCodePopup $_56 UlvE_: __func::target
static uint32_t t_5A44B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41330u ? self + 8 : 0; }
RW_FUNCTION(0x5A44B8, "OtherMenuDialog::openInputUserCodePopup $_56 UlvE_ __func::target", t_5A44B8);

// OtherMenuDialog::openInputUserCodePopup $_56 UlvE_: __func::target_type
static uint32_t t_5A44DC() { return 0xf8d50cu; }
RW_FUNCTION(0x5A44DC, "OtherMenuDialog::openInputUserCodePopup $_56 UlvE_ __func::target_type", t_5A44DC);

// OtherMenuDialog::openInputUserCodePopup $_57: __func::~__func (deleting)
static void t_5A44F0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A44F0, "OtherMenuDialog::openInputUserCodePopup $_57 __func::~__func (deleting)", t_5A44F0);

// OtherMenuDialog::openInputUserCodePopup $_57: __func::__clone()
static uint32_t t_5A44F8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b50cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A44F8, "OtherMenuDialog::openInputUserCodePopup $_57 __func::__clone()", t_5A44F8);

// OtherMenuDialog::openInputUserCodePopup $_57: __func::__clone(__base*)
static void t_5A4524(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b50cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A4524, "OtherMenuDialog::openInputUserCodePopup $_57 __func::__clone(__base*)", t_5A4524);

// OtherMenuDialog::openInputUserCodePopup $_57: __func::destroy
static void t_5A4544(uint32_t self) {
}
RW_FUNCTION(0x5A4544, "OtherMenuDialog::openInputUserCodePopup $_57 __func::destroy", t_5A4544);

// OtherMenuDialog::openInputUserCodePopup $_57: __func::destroy_deallocate
static void t_5A4546(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4546, "OtherMenuDialog::openInputUserCodePopup $_57 __func::destroy_deallocate", t_5A4546);

// OtherMenuDialog::openInputUserCodePopup $_57: __func::target
static uint32_t t_5A4558(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41410u ? self + 4 : 0; }
RW_FUNCTION(0x5A4558, "OtherMenuDialog::openInputUserCodePopup $_57 __func::target", t_5A4558);

// OtherMenuDialog::openInputUserCodePopup $_57: __func::target_type
static uint32_t t_5A4578() { return 0xf8d520u; }
RW_FUNCTION(0x5A4578, "OtherMenuDialog::openInputUserCodePopup $_57 __func::target_type", t_5A4578);

// OtherMenuDialog::openInputUserCodePopup $_55: __func::~__func (deleting)
static void t_5A458C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A458C, "OtherMenuDialog::openInputUserCodePopup $_55 __func::~__func (deleting)", t_5A458C);

// OtherMenuDialog::openInputUserCodePopup $_55: __func::__clone()
static uint32_t t_5A4594(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5b538u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A4594, "OtherMenuDialog::openInputUserCodePopup $_55 __func::__clone()", t_5A4594);

// OtherMenuDialog::openInputUserCodePopup $_55: __func::__clone(__base*)
static void t_5A45C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b538u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A45C4, "OtherMenuDialog::openInputUserCodePopup $_55 __func::__clone(__base*)", t_5A45C4);

// OtherMenuDialog::openInputUserCodePopup $_55: __func::destroy
static void t_5A45E8(uint32_t self) {
}
RW_FUNCTION(0x5A45E8, "OtherMenuDialog::openInputUserCodePopup $_55 __func::destroy", t_5A45E8);

// OtherMenuDialog::openInputUserCodePopup $_55: __func::destroy_deallocate
static void t_5A45EA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A45EA, "OtherMenuDialog::openInputUserCodePopup $_55 __func::destroy_deallocate", t_5A45EA);

// OtherMenuDialog::openInputUserCodePopup $_55: __func::target
static uint32_t t_5A46DC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41510u ? self + 4 : 0; }
RW_FUNCTION(0x5A46DC, "OtherMenuDialog::openInputUserCodePopup $_55 __func::target", t_5A46DC);

// OtherMenuDialog::openInputUserCodePopup $_55: __func::target_type
static uint32_t t_5A46FC() { return 0xf8d534u; }
RW_FUNCTION(0x5A46FC, "OtherMenuDialog::openInputUserCodePopup $_55 __func::target_type", t_5A46FC);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_: __func::~__func (deleting)
static void t_5A4710(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4710, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ __func::~__func (deleting)", t_5A4710);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_: __func::__clone()
static uint32_t t_5A4718(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b564u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A4718, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ __func::__clone()", t_5A4718);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_: __func::__clone(__base*)
static void t_5A4744(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b564u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A4744, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ __func::__clone(__base*)", t_5A4744);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_: __func::destroy
static void t_5A4764(uint32_t self) {
}
RW_FUNCTION(0x5A4764, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ __func::destroy", t_5A4764);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_: __func::destroy_deallocate
static void t_5A4766(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4766, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ __func::destroy_deallocate", t_5A4766);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_: __func::target
static uint32_t t_5A4864(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41620u ? self + 4 : 0; }
RW_FUNCTION(0x5A4864, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ __func::target", t_5A4864);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_: __func::target_type
static uint32_t t_5A4884() { return 0xf8d548u; }
RW_FUNCTION(0x5A4884, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ __func::target_type", t_5A4884);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_: __func::~__func (deleting)
static void t_5A4898(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4898, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_ __func::~__func (deleting)", t_5A4898);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_: __func::__clone()
static uint32_t t_5A48A0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b590u);
    }
    return v1;
}
RW_FUNCTION(0x5A48A0, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_ __func::__clone()", t_5A48A0);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_: __func::__clone(__base*)
static void t_5A48C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b590u);
    }
}
RW_FUNCTION(0x5A48C4, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_ __func::__clone(__base*)", t_5A48C4);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_: __func::destroy
static void t_5A48E0(uint32_t self) {
}
RW_FUNCTION(0x5A48E0, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_ __func::destroy", t_5A48E0);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_: __func::destroy_deallocate
static void t_5A48E2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A48E2, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_ __func::destroy_deallocate", t_5A48E2);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_: __func::target
static uint32_t t_5A48EC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41780u ? self + 4 : 0; }
RW_FUNCTION(0x5A48EC, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_ __func::target", t_5A48EC);

// OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_: __func::target_type
static uint32_t t_5A490C() { return 0xf8d55cu; }
RW_FUNCTION(0x5A490C, "OtherMenuDialog::openInputUserCodePopup $_55 UlbE_ UlbE_ __func::target_type", t_5A490C);

// OtherMenuDialog::openInputUserCodePopup $_54: __func::~__func (deleting)
static void t_5A4920(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4920, "OtherMenuDialog::openInputUserCodePopup $_54 __func::~__func (deleting)", t_5A4920);

// OtherMenuDialog::openInputUserCodePopup $_54: __func::__clone()
static uint32_t t_5A4928(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b5bcu);
    }
    return v1;
}
RW_FUNCTION(0x5A4928, "OtherMenuDialog::openInputUserCodePopup $_54 __func::__clone()", t_5A4928);

// OtherMenuDialog::openInputUserCodePopup $_54: __func::__clone(__base*)
static void t_5A494C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b5bcu);
    }
}
RW_FUNCTION(0x5A494C, "OtherMenuDialog::openInputUserCodePopup $_54 __func::__clone(__base*)", t_5A494C);

// OtherMenuDialog::openInputUserCodePopup $_54: __func::destroy
static void t_5A4968(uint32_t self) {
}
RW_FUNCTION(0x5A4968, "OtherMenuDialog::openInputUserCodePopup $_54 __func::destroy", t_5A4968);

// OtherMenuDialog::openInputUserCodePopup $_54: __func::destroy_deallocate
static void t_5A496A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A496A, "OtherMenuDialog::openInputUserCodePopup $_54 __func::destroy_deallocate", t_5A496A);

// OtherMenuDialog::openInputUserCodePopup $_54: __func::target
static uint32_t t_5A4988(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe418d0u ? self + 4 : 0; }
RW_FUNCTION(0x5A4988, "OtherMenuDialog::openInputUserCodePopup $_54 __func::target", t_5A4988);

// OtherMenuDialog::openInputUserCodePopup $_54: __func::target_type
static uint32_t t_5A49A8() { return 0xf8d570u; }
RW_FUNCTION(0x5A49A8, "OtherMenuDialog::openInputUserCodePopup $_54 __func::target_type", t_5A49A8);

// OtherMenuDialog::openIssueUserCodePopup $_53: __func::~__func (deleting)
static void t_5A49BC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A49BC, "OtherMenuDialog::openIssueUserCodePopup $_53 __func::~__func (deleting)", t_5A49BC);

// OtherMenuDialog::openIssueUserCodePopup $_53: __func::__clone()
static uint32_t t_5A49C4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b5e8u);
    }
    return v1;
}
RW_FUNCTION(0x5A49C4, "OtherMenuDialog::openIssueUserCodePopup $_53 __func::__clone()", t_5A49C4);

// OtherMenuDialog::openIssueUserCodePopup $_53: __func::__clone(__base*)
static void t_5A49E8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b5e8u);
    }
}
RW_FUNCTION(0x5A49E8, "OtherMenuDialog::openIssueUserCodePopup $_53 __func::__clone(__base*)", t_5A49E8);

// OtherMenuDialog::openIssueUserCodePopup $_53: __func::destroy
static void t_5A4A04(uint32_t self) {
}
RW_FUNCTION(0x5A4A04, "OtherMenuDialog::openIssueUserCodePopup $_53 __func::destroy", t_5A4A04);

// OtherMenuDialog::openIssueUserCodePopup $_53: __func::destroy_deallocate
static void t_5A4A06(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4A06, "OtherMenuDialog::openIssueUserCodePopup $_53 __func::destroy_deallocate", t_5A4A06);

// OtherMenuDialog::openIssueUserCodePopup $_53: __func::target
static uint32_t t_5A4A10(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe419a0u ? self + 4 : 0; }
RW_FUNCTION(0x5A4A10, "OtherMenuDialog::openIssueUserCodePopup $_53 __func::target", t_5A4A10);

// OtherMenuDialog::openIssueUserCodePopup $_53: __func::target_type
static uint32_t t_5A4A30() { return 0xf8d584u; }
RW_FUNCTION(0x5A4A30, "OtherMenuDialog::openIssueUserCodePopup $_53 __func::target_type", t_5A4A30);

// OtherMenuDialog::openIssueUserCodePopup $_52: __func::~__func
static uint32_t t_5A4A44(uint32_t self) {
    st32(self, 0xf5b614u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A4A44, "OtherMenuDialog::openIssueUserCodePopup $_52 __func::~__func", t_5A4A44);

// OtherMenuDialog::openIssueUserCodePopup $_52: __func::~__func (deleting)
static void t_5A4A80(uint32_t self) {
    st32(self, 0xf5b614u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4A80, "OtherMenuDialog::openIssueUserCodePopup $_52 __func::~__func (deleting)", t_5A4A80);

// OtherMenuDialog::openIssueUserCodePopup $_52: __func::__clone()
static uint32_t t_5A4AC0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b614u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A4AC0, "OtherMenuDialog::openIssueUserCodePopup $_52 __func::__clone()", t_5A4AC0);

// OtherMenuDialog::openIssueUserCodePopup $_52: __func::__clone(__base*)
static void t_5A4B1C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b614u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A4B1C, "OtherMenuDialog::openIssueUserCodePopup $_52 __func::__clone(__base*)", t_5A4B1C);

// OtherMenuDialog::openIssueUserCodePopup $_52: __func::destroy
static void t_5A4B5C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A4B5C, "OtherMenuDialog::openIssueUserCodePopup $_52 __func::destroy", t_5A4B5C);

// OtherMenuDialog::openIssueUserCodePopup $_52: __func::destroy_deallocate
static void t_5A4B7C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4B7C, "OtherMenuDialog::openIssueUserCodePopup $_52 __func::destroy_deallocate", t_5A4B7C);

// OtherMenuDialog::openIssueUserCodePopup $_52: __func::target
static uint32_t t_5A4CF4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41a70u ? self + 8 : 0; }
RW_FUNCTION(0x5A4CF4, "OtherMenuDialog::openIssueUserCodePopup $_52 __func::target", t_5A4CF4);

// OtherMenuDialog::openIssueUserCodePopup $_52: __func::target_type
static uint32_t t_5A4D18() { return 0xf8d598u; }
RW_FUNCTION(0x5A4D18, "OtherMenuDialog::openIssueUserCodePopup $_52 __func::target_type", t_5A4D18);

// OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_: __func::~__func
static uint32_t t_5A4D2C(uint32_t self) {
    st32(self, 0xf5b640u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A4D2C, "OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_ __func::~__func", t_5A4D2C);

// OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_: __func::~__func (deleting)
static void t_5A4D68(uint32_t self) {
    st32(self, 0xf5b640u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4D68, "OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_ __func::~__func (deleting)", t_5A4D68);

// OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_: __func::__clone()
static uint32_t t_5A4DA8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b640u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A4DA8, "OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_ __func::__clone()", t_5A4DA8);

// OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_: __func::__clone(__base*)
static void t_5A4E04(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b640u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A4E04, "OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_ __func::__clone(__base*)", t_5A4E04);

// OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_: __func::destroy
static void t_5A4E44(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A4E44, "OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_ __func::destroy", t_5A4E44);

// OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_: __func::destroy_deallocate
static void t_5A4E64(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4E64, "OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_ __func::destroy_deallocate", t_5A4E64);

// OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_: __func::target
static uint32_t t_5A4ECC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41b50u ? self + 8 : 0; }
RW_FUNCTION(0x5A4ECC, "OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_ __func::target", t_5A4ECC);

// OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_: __func::target_type
static uint32_t t_5A4EF0() { return 0xf8d5acu; }
RW_FUNCTION(0x5A4EF0, "OtherMenuDialog::openIssueUserCodePopup $_50 UlvE_ __func::target_type", t_5A4EF0);

// OtherMenuDialog::openIssueUserCodePopup $_51: __func::~__func (deleting)
static void t_5A4F04(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4F04, "OtherMenuDialog::openIssueUserCodePopup $_51 __func::~__func (deleting)", t_5A4F04);

// OtherMenuDialog::openIssueUserCodePopup $_51: __func::__clone()
static uint32_t t_5A4F0C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b66cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A4F0C, "OtherMenuDialog::openIssueUserCodePopup $_51 __func::__clone()", t_5A4F0C);

// OtherMenuDialog::openIssueUserCodePopup $_51: __func::__clone(__base*)
static void t_5A4F38(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b66cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A4F38, "OtherMenuDialog::openIssueUserCodePopup $_51 __func::__clone(__base*)", t_5A4F38);

// OtherMenuDialog::openIssueUserCodePopup $_51: __func::destroy
static void t_5A4F58(uint32_t self) {
}
RW_FUNCTION(0x5A4F58, "OtherMenuDialog::openIssueUserCodePopup $_51 __func::destroy", t_5A4F58);

// OtherMenuDialog::openIssueUserCodePopup $_51: __func::destroy_deallocate
static void t_5A4F5A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4F5A, "OtherMenuDialog::openIssueUserCodePopup $_51 __func::destroy_deallocate", t_5A4F5A);

// OtherMenuDialog::openIssueUserCodePopup $_51: __func::target
static uint32_t t_5A4F6C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41c30u ? self + 4 : 0; }
RW_FUNCTION(0x5A4F6C, "OtherMenuDialog::openIssueUserCodePopup $_51 __func::target", t_5A4F6C);

// OtherMenuDialog::openIssueUserCodePopup $_51: __func::target_type
static uint32_t t_5A4F8C() { return 0xf8d5c0u; }
RW_FUNCTION(0x5A4F8C, "OtherMenuDialog::openIssueUserCodePopup $_51 __func::target_type", t_5A4F8C);

// OtherMenuDialog::openIssueUserCodePopup $_49: __func::~__func (deleting)
static void t_5A4FA0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4FA0, "OtherMenuDialog::openIssueUserCodePopup $_49 __func::~__func (deleting)", t_5A4FA0);

// OtherMenuDialog::openIssueUserCodePopup $_49: __func::__clone()
static uint32_t t_5A4FA8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b698u);
    }
    return v1;
}
RW_FUNCTION(0x5A4FA8, "OtherMenuDialog::openIssueUserCodePopup $_49 __func::__clone()", t_5A4FA8);

// OtherMenuDialog::openIssueUserCodePopup $_49: __func::__clone(__base*)
static void t_5A4FCC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b698u);
    }
}
RW_FUNCTION(0x5A4FCC, "OtherMenuDialog::openIssueUserCodePopup $_49 __func::__clone(__base*)", t_5A4FCC);

// OtherMenuDialog::openIssueUserCodePopup $_49: __func::destroy
static void t_5A4FE8(uint32_t self) {
}
RW_FUNCTION(0x5A4FE8, "OtherMenuDialog::openIssueUserCodePopup $_49 __func::destroy", t_5A4FE8);

// OtherMenuDialog::openIssueUserCodePopup $_49: __func::destroy_deallocate
static void t_5A4FEA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A4FEA, "OtherMenuDialog::openIssueUserCodePopup $_49 __func::destroy_deallocate", t_5A4FEA);

// OtherMenuDialog::openIssueUserCodePopup $_49: __func::target
static uint32_t t_5A5008(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41d30u ? self + 4 : 0; }
RW_FUNCTION(0x5A5008, "OtherMenuDialog::openIssueUserCodePopup $_49 __func::target", t_5A5008);

// OtherMenuDialog::openIssueUserCodePopup $_49: __func::target_type
static uint32_t t_5A5028() { return 0xf8d5d4u; }
RW_FUNCTION(0x5A5028, "OtherMenuDialog::openIssueUserCodePopup $_49 __func::target_type", t_5A5028);

// OtherMenuDialog::openUserCodePopup $_48: __func::~__func (deleting)
static void t_5A503C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A503C, "OtherMenuDialog::openUserCodePopup $_48 __func::~__func (deleting)", t_5A503C);

// OtherMenuDialog::openUserCodePopup $_48: __func::__clone()
static uint32_t t_5A5044(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b6c4u);
    }
    return v1;
}
RW_FUNCTION(0x5A5044, "OtherMenuDialog::openUserCodePopup $_48 __func::__clone()", t_5A5044);

// OtherMenuDialog::openUserCodePopup $_48: __func::__clone(__base*)
static void t_5A5068(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b6c4u);
    }
}
RW_FUNCTION(0x5A5068, "OtherMenuDialog::openUserCodePopup $_48 __func::__clone(__base*)", t_5A5068);

// OtherMenuDialog::openUserCodePopup $_48: __func::destroy
static void t_5A5084(uint32_t self) {
}
RW_FUNCTION(0x5A5084, "OtherMenuDialog::openUserCodePopup $_48 __func::destroy", t_5A5084);

// OtherMenuDialog::openUserCodePopup $_48: __func::destroy_deallocate
static void t_5A5086(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5086, "OtherMenuDialog::openUserCodePopup $_48 __func::destroy_deallocate", t_5A5086);

// OtherMenuDialog::openUserCodePopup $_48: __func::target
static uint32_t t_5A5090(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41e00u ? self + 4 : 0; }
RW_FUNCTION(0x5A5090, "OtherMenuDialog::openUserCodePopup $_48 __func::target", t_5A5090);

// OtherMenuDialog::openUserCodePopup $_48: __func::target_type
static uint32_t t_5A50B0() { return 0xf8d5e8u; }
RW_FUNCTION(0x5A50B0, "OtherMenuDialog::openUserCodePopup $_48 __func::target_type", t_5A50B0);

// OtherMenuDialog::openUserCodePopup $_47: __func::~__func
static uint32_t t_5A50C4(uint32_t self) {
    st32(self, 0xf5b6f0u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A50C4, "OtherMenuDialog::openUserCodePopup $_47 __func::~__func", t_5A50C4);

// OtherMenuDialog::openUserCodePopup $_47: __func::~__func (deleting)
static void t_5A5100(uint32_t self) {
    st32(self, 0xf5b6f0u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5100, "OtherMenuDialog::openUserCodePopup $_47 __func::~__func (deleting)", t_5A5100);

// OtherMenuDialog::openUserCodePopup $_47: __func::__clone()
static uint32_t t_5A5140(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b6f0u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A5140, "OtherMenuDialog::openUserCodePopup $_47 __func::__clone()", t_5A5140);

// OtherMenuDialog::openUserCodePopup $_47: __func::__clone(__base*)
static void t_5A519C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b6f0u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A519C, "OtherMenuDialog::openUserCodePopup $_47 __func::__clone(__base*)", t_5A519C);

// OtherMenuDialog::openUserCodePopup $_47: __func::destroy
static void t_5A51DC(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A51DC, "OtherMenuDialog::openUserCodePopup $_47 __func::destroy", t_5A51DC);

// OtherMenuDialog::openUserCodePopup $_47: __func::destroy_deallocate
static void t_5A51FC(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A51FC, "OtherMenuDialog::openUserCodePopup $_47 __func::destroy_deallocate", t_5A51FC);

// OtherMenuDialog::openUserCodePopup $_47: __func::target
static uint32_t t_5A5380(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41ed0u ? self + 8 : 0; }
RW_FUNCTION(0x5A5380, "OtherMenuDialog::openUserCodePopup $_47 __func::target", t_5A5380);

// OtherMenuDialog::openUserCodePopup $_47: __func::target_type
static uint32_t t_5A53A4() { return 0xf8d5fcu; }
RW_FUNCTION(0x5A53A4, "OtherMenuDialog::openUserCodePopup $_47 __func::target_type", t_5A53A4);

// OtherMenuDialog::openUserCodePopup $_45 UlvE_: __func::~__func
static uint32_t t_5A53B8(uint32_t self) {
    st32(self, 0xf5b71cu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A53B8, "OtherMenuDialog::openUserCodePopup $_45 UlvE_ __func::~__func", t_5A53B8);

// OtherMenuDialog::openUserCodePopup $_45 UlvE_: __func::~__func (deleting)
static void t_5A53F4(uint32_t self) {
    st32(self, 0xf5b71cu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A53F4, "OtherMenuDialog::openUserCodePopup $_45 UlvE_ __func::~__func (deleting)", t_5A53F4);

// OtherMenuDialog::openUserCodePopup $_45 UlvE_: __func::__clone()
static uint32_t t_5A5434(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b71cu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A5434, "OtherMenuDialog::openUserCodePopup $_45 UlvE_ __func::__clone()", t_5A5434);

// OtherMenuDialog::openUserCodePopup $_45 UlvE_: __func::__clone(__base*)
static void t_5A5490(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b71cu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A5490, "OtherMenuDialog::openUserCodePopup $_45 UlvE_ __func::__clone(__base*)", t_5A5490);

// OtherMenuDialog::openUserCodePopup $_45 UlvE_: __func::destroy
static void t_5A54D0(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A54D0, "OtherMenuDialog::openUserCodePopup $_45 UlvE_ __func::destroy", t_5A54D0);

// OtherMenuDialog::openUserCodePopup $_45 UlvE_: __func::destroy_deallocate
static void t_5A54F0(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A54F0, "OtherMenuDialog::openUserCodePopup $_45 UlvE_ __func::destroy_deallocate", t_5A54F0);

// OtherMenuDialog::openUserCodePopup $_45 UlvE_: __func::target
static uint32_t t_5A5558(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe41fb0u ? self + 8 : 0; }
RW_FUNCTION(0x5A5558, "OtherMenuDialog::openUserCodePopup $_45 UlvE_ __func::target", t_5A5558);

// OtherMenuDialog::openUserCodePopup $_45 UlvE_: __func::target_type
static uint32_t t_5A557C() { return 0xf8d610u; }
RW_FUNCTION(0x5A557C, "OtherMenuDialog::openUserCodePopup $_45 UlvE_ __func::target_type", t_5A557C);

// OtherMenuDialog::openUserCodePopup $_46: __func::~__func (deleting)
static void t_5A5590(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5590, "OtherMenuDialog::openUserCodePopup $_46 __func::~__func (deleting)", t_5A5590);

// OtherMenuDialog::openUserCodePopup $_46: __func::__clone()
static uint32_t t_5A5598(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b748u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A5598, "OtherMenuDialog::openUserCodePopup $_46 __func::__clone()", t_5A5598);

// OtherMenuDialog::openUserCodePopup $_46: __func::__clone(__base*)
static void t_5A55C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b748u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A55C4, "OtherMenuDialog::openUserCodePopup $_46 __func::__clone(__base*)", t_5A55C4);

// OtherMenuDialog::openUserCodePopup $_46: __func::destroy
static void t_5A55E4(uint32_t self) {
}
RW_FUNCTION(0x5A55E4, "OtherMenuDialog::openUserCodePopup $_46 __func::destroy", t_5A55E4);

// OtherMenuDialog::openUserCodePopup $_46: __func::destroy_deallocate
static void t_5A55E6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A55E6, "OtherMenuDialog::openUserCodePopup $_46 __func::destroy_deallocate", t_5A55E6);

// OtherMenuDialog::openUserCodePopup $_46: __func::target
static uint32_t t_5A55F8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42090u ? self + 4 : 0; }
RW_FUNCTION(0x5A55F8, "OtherMenuDialog::openUserCodePopup $_46 __func::target", t_5A55F8);

// OtherMenuDialog::openUserCodePopup $_46: __func::target_type
static uint32_t t_5A5618() { return 0xf8d624u; }
RW_FUNCTION(0x5A5618, "OtherMenuDialog::openUserCodePopup $_46 __func::target_type", t_5A5618);

// OtherMenuDialog::openUserCodePopup $_44: __func::~__func (deleting)
static void t_5A562C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A562C, "OtherMenuDialog::openUserCodePopup $_44 __func::~__func (deleting)", t_5A562C);

// OtherMenuDialog::openUserCodePopup $_44: __func::__clone()
static uint32_t t_5A5634(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b774u);
    }
    return v1;
}
RW_FUNCTION(0x5A5634, "OtherMenuDialog::openUserCodePopup $_44 __func::__clone()", t_5A5634);

// OtherMenuDialog::openUserCodePopup $_44: __func::__clone(__base*)
static void t_5A5658(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b774u);
    }
}
RW_FUNCTION(0x5A5658, "OtherMenuDialog::openUserCodePopup $_44 __func::__clone(__base*)", t_5A5658);

// OtherMenuDialog::openUserCodePopup $_44: __func::destroy
static void t_5A5674(uint32_t self) {
}
RW_FUNCTION(0x5A5674, "OtherMenuDialog::openUserCodePopup $_44 __func::destroy", t_5A5674);

// OtherMenuDialog::openUserCodePopup $_44: __func::destroy_deallocate
static void t_5A5676(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5676, "OtherMenuDialog::openUserCodePopup $_44 __func::destroy_deallocate", t_5A5676);

// OtherMenuDialog::openUserCodePopup $_44: __func::target
static uint32_t t_5A5694(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42190u ? self + 4 : 0; }
RW_FUNCTION(0x5A5694, "OtherMenuDialog::openUserCodePopup $_44 __func::target", t_5A5694);

// OtherMenuDialog::openUserCodePopup $_44: __func::target_type
static uint32_t t_5A56B4() { return 0xf8d638u; }
RW_FUNCTION(0x5A56B4, "OtherMenuDialog::openUserCodePopup $_44 __func::target_type", t_5A56B4);

// OtherMenuDialog::openUserCodePopup $_43: __func::~__func (deleting)
static void t_5A56C8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A56C8, "OtherMenuDialog::openUserCodePopup $_43 __func::~__func (deleting)", t_5A56C8);

// OtherMenuDialog::openUserCodePopup $_43: __func::__clone()
static uint32_t t_5A56D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b7a0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A56D0, "OtherMenuDialog::openUserCodePopup $_43 __func::__clone()", t_5A56D0);

// OtherMenuDialog::openUserCodePopup $_43: __func::__clone(__base*)
static void t_5A56FC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b7a0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A56FC, "OtherMenuDialog::openUserCodePopup $_43 __func::__clone(__base*)", t_5A56FC);

// OtherMenuDialog::openUserCodePopup $_43: __func::destroy
static void t_5A571C(uint32_t self) {
}
RW_FUNCTION(0x5A571C, "OtherMenuDialog::openUserCodePopup $_43 __func::destroy", t_5A571C);

// OtherMenuDialog::openUserCodePopup $_43: __func::destroy_deallocate
static void t_5A571E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A571E, "OtherMenuDialog::openUserCodePopup $_43 __func::destroy_deallocate", t_5A571E);

// OtherMenuDialog::openUserCodePopup $_43: __func::target
static uint32_t t_5A57C4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42290u ? self + 4 : 0; }
RW_FUNCTION(0x5A57C4, "OtherMenuDialog::openUserCodePopup $_43 __func::target", t_5A57C4);

// OtherMenuDialog::openUserCodePopup $_43: __func::target_type
static uint32_t t_5A57E4() { return 0xf8d64cu; }
RW_FUNCTION(0x5A57E4, "OtherMenuDialog::openUserCodePopup $_43 __func::target_type", t_5A57E4);

// OtherMenuDialog::openUserCodePopup $_42: __func::~__func (deleting)
static void t_5A57F8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A57F8, "OtherMenuDialog::openUserCodePopup $_42 __func::~__func (deleting)", t_5A57F8);

// OtherMenuDialog::openUserCodePopup $_42: __func::__clone()
static uint32_t t_5A5800(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b7ccu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A5800, "OtherMenuDialog::openUserCodePopup $_42 __func::__clone()", t_5A5800);

// OtherMenuDialog::openUserCodePopup $_42: __func::__clone(__base*)
static void t_5A582C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b7ccu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A582C, "OtherMenuDialog::openUserCodePopup $_42 __func::__clone(__base*)", t_5A582C);

// OtherMenuDialog::openUserCodePopup $_42: __func::destroy
static void t_5A584C(uint32_t self) {
}
RW_FUNCTION(0x5A584C, "OtherMenuDialog::openUserCodePopup $_42 __func::destroy", t_5A584C);

// OtherMenuDialog::openUserCodePopup $_42: __func::destroy_deallocate
static void t_5A584E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A584E, "OtherMenuDialog::openUserCodePopup $_42 __func::destroy_deallocate", t_5A584E);

// OtherMenuDialog::openUserCodePopup $_42: __func::target
static uint32_t t_5A590C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42390u ? self + 4 : 0; }
RW_FUNCTION(0x5A590C, "OtherMenuDialog::openUserCodePopup $_42 __func::target", t_5A590C);

// OtherMenuDialog::openUserCodePopup $_42: __func::target_type
static uint32_t t_5A592C() { return 0xf8d660u; }
RW_FUNCTION(0x5A592C, "OtherMenuDialog::openUserCodePopup $_42 __func::target_type", t_5A592C);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_: __func::~__func (deleting)
static void t_5A5940(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5940, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ __func::~__func (deleting)", t_5A5940);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_: __func::__clone()
static uint32_t t_5A5948(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b7f8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A5948, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ __func::__clone()", t_5A5948);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_: __func::__clone(__base*)
static void t_5A5974(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b7f8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A5974, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ __func::__clone(__base*)", t_5A5974);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_: __func::destroy
static void t_5A5994(uint32_t self) {
}
RW_FUNCTION(0x5A5994, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ __func::destroy", t_5A5994);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_: __func::destroy_deallocate
static void t_5A5996(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5996, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ __func::destroy_deallocate", t_5A5996);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_: __func::target
static uint32_t t_5A5A88(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42490u ? self + 4 : 0; }
RW_FUNCTION(0x5A5A88, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ __func::target", t_5A5A88);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_: __func::target_type
static uint32_t t_5A5AA8() { return 0xf8d674u; }
RW_FUNCTION(0x5A5AA8, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ __func::target_type", t_5A5AA8);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_: __func::~__func (deleting)
static void t_5A5ABC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5ABC, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_ __func::~__func (deleting)", t_5A5ABC);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_: __func::__clone()
static uint32_t t_5A5AC4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b824u);
    }
    return v1;
}
RW_FUNCTION(0x5A5AC4, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_ __func::__clone()", t_5A5AC4);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_: __func::__clone(__base*)
static void t_5A5AE8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b824u);
    }
}
RW_FUNCTION(0x5A5AE8, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_ __func::__clone(__base*)", t_5A5AE8);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_: __func::destroy
static void t_5A5B04(uint32_t self) {
}
RW_FUNCTION(0x5A5B04, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_ __func::destroy", t_5A5B04);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_: __func::destroy_deallocate
static void t_5A5B06(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5B06, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_ __func::destroy_deallocate", t_5A5B06);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_: __func::target
static uint32_t t_5A5B10(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe425d0u ? self + 4 : 0; }
RW_FUNCTION(0x5A5B10, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_ __func::target", t_5A5B10);

// OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_: __func::target_type
static uint32_t t_5A5B30() { return 0xf8d688u; }
RW_FUNCTION(0x5A5B30, "OtherMenuDialog::openUserCodePopup $_42 UlbE_ UlbE_ __func::target_type", t_5A5B30);

// OtherMenuDialog::openSerialCodeListPopup $_41: __func::~__func (deleting)
static void t_5A5B44(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5B44, "OtherMenuDialog::openSerialCodeListPopup $_41 __func::~__func (deleting)", t_5A5B44);

// OtherMenuDialog::openSerialCodeListPopup $_41: __func::__clone()
static uint32_t t_5A5B4C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b850u);
    }
    return v1;
}
RW_FUNCTION(0x5A5B4C, "OtherMenuDialog::openSerialCodeListPopup $_41 __func::__clone()", t_5A5B4C);

// OtherMenuDialog::openSerialCodeListPopup $_41: __func::__clone(__base*)
static void t_5A5B70(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b850u);
    }
}
RW_FUNCTION(0x5A5B70, "OtherMenuDialog::openSerialCodeListPopup $_41 __func::__clone(__base*)", t_5A5B70);

// OtherMenuDialog::openSerialCodeListPopup $_41: __func::destroy
static void t_5A5B8C(uint32_t self) {
}
RW_FUNCTION(0x5A5B8C, "OtherMenuDialog::openSerialCodeListPopup $_41 __func::destroy", t_5A5B8C);

// OtherMenuDialog::openSerialCodeListPopup $_41: __func::destroy_deallocate
static void t_5A5B8E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5B8E, "OtherMenuDialog::openSerialCodeListPopup $_41 __func::destroy_deallocate", t_5A5B8E);

// OtherMenuDialog::openSerialCodeListPopup $_41: __func::target
static uint32_t t_5A5B98(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe426e0u ? self + 4 : 0; }
RW_FUNCTION(0x5A5B98, "OtherMenuDialog::openSerialCodeListPopup $_41 __func::target", t_5A5B98);

// OtherMenuDialog::openSerialCodeListPopup $_41: __func::target_type
static uint32_t t_5A5BB8() { return 0xf8d69cu; }
RW_FUNCTION(0x5A5BB8, "OtherMenuDialog::openSerialCodeListPopup $_41 __func::target_type", t_5A5BB8);

// OtherMenuDialog::openSerialCodeListPopup $_40: __func::~__func
static uint32_t t_5A5BCC(uint32_t self) {
    st32(self, 0xf5b87cu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A5BCC, "OtherMenuDialog::openSerialCodeListPopup $_40 __func::~__func", t_5A5BCC);

// OtherMenuDialog::openSerialCodeListPopup $_40: __func::~__func (deleting)
static void t_5A5C08(uint32_t self) {
    st32(self, 0xf5b87cu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5C08, "OtherMenuDialog::openSerialCodeListPopup $_40 __func::~__func (deleting)", t_5A5C08);

// OtherMenuDialog::openSerialCodeListPopup $_40: __func::__clone()
static uint32_t t_5A5C48(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b87cu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A5C48, "OtherMenuDialog::openSerialCodeListPopup $_40 __func::__clone()", t_5A5C48);

// OtherMenuDialog::openSerialCodeListPopup $_40: __func::__clone(__base*)
static void t_5A5CA4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b87cu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A5CA4, "OtherMenuDialog::openSerialCodeListPopup $_40 __func::__clone(__base*)", t_5A5CA4);

// OtherMenuDialog::openSerialCodeListPopup $_40: __func::destroy
static void t_5A5CE4(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A5CE4, "OtherMenuDialog::openSerialCodeListPopup $_40 __func::destroy", t_5A5CE4);

// OtherMenuDialog::openSerialCodeListPopup $_40: __func::destroy_deallocate
static void t_5A5D04(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5D04, "OtherMenuDialog::openSerialCodeListPopup $_40 __func::destroy_deallocate", t_5A5D04);

// OtherMenuDialog::openSerialCodeListPopup $_40: __func::target
static uint32_t t_5A5E8C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe427b0u ? self + 8 : 0; }
RW_FUNCTION(0x5A5E8C, "OtherMenuDialog::openSerialCodeListPopup $_40 __func::target", t_5A5E8C);

// OtherMenuDialog::openSerialCodeListPopup $_40: __func::target_type
static uint32_t t_5A5EB0() { return 0xf8d6b0u; }
RW_FUNCTION(0x5A5EB0, "OtherMenuDialog::openSerialCodeListPopup $_40 __func::target_type", t_5A5EB0);

// OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_: __func::~__func
static uint32_t t_5A5EC4(uint32_t self) {
    st32(self, 0xf5b8a8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A5EC4, "OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_ __func::~__func", t_5A5EC4);

// OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_: __func::~__func (deleting)
static void t_5A5F00(uint32_t self) {
    st32(self, 0xf5b8a8u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5F00, "OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_ __func::~__func (deleting)", t_5A5F00);

// OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_: __func::__clone()
static uint32_t t_5A5F40(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b8a8u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A5F40, "OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_ __func::__clone()", t_5A5F40);

// OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_: __func::__clone(__base*)
static void t_5A5F9C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b8a8u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A5F9C, "OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_ __func::__clone(__base*)", t_5A5F9C);

// OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_: __func::destroy
static void t_5A5FDC(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A5FDC, "OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_ __func::destroy", t_5A5FDC);

// OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_: __func::destroy_deallocate
static void t_5A5FFC(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A5FFC, "OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_ __func::destroy_deallocate", t_5A5FFC);

// OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_: __func::target
static uint32_t t_5A6064(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42890u ? self + 8 : 0; }
RW_FUNCTION(0x5A6064, "OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_ __func::target", t_5A6064);

// OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_: __func::target_type
static uint32_t t_5A6088() { return 0xf8d6c4u; }
RW_FUNCTION(0x5A6088, "OtherMenuDialog::openSerialCodeListPopup $_38 UlvE_ __func::target_type", t_5A6088);

// OtherMenuDialog::openSerialCodeListPopup $_39: __func::~__func (deleting)
static void t_5A609C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A609C, "OtherMenuDialog::openSerialCodeListPopup $_39 __func::~__func (deleting)", t_5A609C);

// OtherMenuDialog::openSerialCodeListPopup $_39: __func::__clone()
static uint32_t t_5A60A4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b8d4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A60A4, "OtherMenuDialog::openSerialCodeListPopup $_39 __func::__clone()", t_5A60A4);

// OtherMenuDialog::openSerialCodeListPopup $_39: __func::__clone(__base*)
static void t_5A60D0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b8d4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A60D0, "OtherMenuDialog::openSerialCodeListPopup $_39 __func::__clone(__base*)", t_5A60D0);

// OtherMenuDialog::openSerialCodeListPopup $_39: __func::destroy
static void t_5A60F0(uint32_t self) {
}
RW_FUNCTION(0x5A60F0, "OtherMenuDialog::openSerialCodeListPopup $_39 __func::destroy", t_5A60F0);

// OtherMenuDialog::openSerialCodeListPopup $_39: __func::destroy_deallocate
static void t_5A60F2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A60F2, "OtherMenuDialog::openSerialCodeListPopup $_39 __func::destroy_deallocate", t_5A60F2);

// OtherMenuDialog::openSerialCodeListPopup $_39: __func::target
static uint32_t t_5A6104(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42970u ? self + 4 : 0; }
RW_FUNCTION(0x5A6104, "OtherMenuDialog::openSerialCodeListPopup $_39 __func::target", t_5A6104);

// OtherMenuDialog::openSerialCodeListPopup $_39: __func::target_type
static uint32_t t_5A6124() { return 0xf8d6d8u; }
RW_FUNCTION(0x5A6124, "OtherMenuDialog::openSerialCodeListPopup $_39 __func::target_type", t_5A6124);

// OtherMenuDialog::openSerialCodeListPopup $_37: __func::~__func (deleting)
static void t_5A6138(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6138, "OtherMenuDialog::openSerialCodeListPopup $_37 __func::~__func (deleting)", t_5A6138);

// OtherMenuDialog::openSerialCodeListPopup $_37: __func::__clone()
static uint32_t t_5A6140(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b900u);
    }
    return v1;
}
RW_FUNCTION(0x5A6140, "OtherMenuDialog::openSerialCodeListPopup $_37 __func::__clone()", t_5A6140);

// OtherMenuDialog::openSerialCodeListPopup $_37: __func::__clone(__base*)
static void t_5A6164(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b900u);
    }
}
RW_FUNCTION(0x5A6164, "OtherMenuDialog::openSerialCodeListPopup $_37 __func::__clone(__base*)", t_5A6164);

// OtherMenuDialog::openSerialCodeListPopup $_37: __func::destroy
static void t_5A6180(uint32_t self) {
}
RW_FUNCTION(0x5A6180, "OtherMenuDialog::openSerialCodeListPopup $_37 __func::destroy", t_5A6180);

// OtherMenuDialog::openSerialCodeListPopup $_37: __func::destroy_deallocate
static void t_5A6182(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6182, "OtherMenuDialog::openSerialCodeListPopup $_37 __func::destroy_deallocate", t_5A6182);

// OtherMenuDialog::openSerialCodeListPopup $_37: __func::target
static uint32_t t_5A61A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42a70u ? self + 4 : 0; }
RW_FUNCTION(0x5A61A0, "OtherMenuDialog::openSerialCodeListPopup $_37 __func::target", t_5A61A0);

// OtherMenuDialog::openSerialCodeListPopup $_37: __func::target_type
static uint32_t t_5A61C0() { return 0xf8d6ecu; }
RW_FUNCTION(0x5A61C0, "OtherMenuDialog::openSerialCodeListPopup $_37 __func::target_type", t_5A61C0);

// OtherMenuDialog::openSerialCodeListPopup $_36: __func::~__func (deleting)
static void t_5A61D4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A61D4, "OtherMenuDialog::openSerialCodeListPopup $_36 __func::~__func (deleting)", t_5A61D4);

// OtherMenuDialog::openSerialCodeListPopup $_36: __func::__clone()
static uint32_t t_5A61DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5b92cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x5A61DC, "OtherMenuDialog::openSerialCodeListPopup $_36 __func::__clone()", t_5A61DC);

// OtherMenuDialog::openSerialCodeListPopup $_36: __func::__clone(__base*)
static void t_5A620C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b92cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A620C, "OtherMenuDialog::openSerialCodeListPopup $_36 __func::__clone(__base*)", t_5A620C);

// OtherMenuDialog::openSerialCodeListPopup $_36: __func::destroy
static void t_5A6234(uint32_t self) {
}
RW_FUNCTION(0x5A6234, "OtherMenuDialog::openSerialCodeListPopup $_36 __func::destroy", t_5A6234);

// OtherMenuDialog::openSerialCodeListPopup $_36: __func::destroy_deallocate
static void t_5A6236(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6236, "OtherMenuDialog::openSerialCodeListPopup $_36 __func::destroy_deallocate", t_5A6236);

// OtherMenuDialog::openSerialCodeListPopup $_36: __func::target
static uint32_t t_5A63BC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42b40u ? self + 4 : 0; }
RW_FUNCTION(0x5A63BC, "OtherMenuDialog::openSerialCodeListPopup $_36 __func::target", t_5A63BC);

// OtherMenuDialog::openSerialCodeListPopup $_36: __func::target_type
static uint32_t t_5A63DC() { return 0xf8d700u; }
RW_FUNCTION(0x5A63DC, "OtherMenuDialog::openSerialCodeListPopup $_36 __func::target_type", t_5A63DC);

// OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_: __func::~__func (deleting)
static void t_5A63F0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A63F0, "OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_ __func::~__func (deleting)", t_5A63F0);

// OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_: __func::__clone()
static uint32_t t_5A63F8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b958u);
    }
    return v1;
}
RW_FUNCTION(0x5A63F8, "OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_ __func::__clone()", t_5A63F8);

// OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_: __func::__clone(__base*)
static void t_5A641C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b958u);
    }
}
RW_FUNCTION(0x5A641C, "OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_ __func::__clone(__base*)", t_5A641C);

// OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_: __func::destroy
static void t_5A6438(uint32_t self) {
}
RW_FUNCTION(0x5A6438, "OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_ __func::destroy", t_5A6438);

// OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_: __func::destroy_deallocate
static void t_5A643A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A643A, "OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_ __func::destroy_deallocate", t_5A643A);

// OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_: __func::target
static uint32_t t_5A6450(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42c20u ? self + 4 : 0; }
RW_FUNCTION(0x5A6450, "OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_ __func::target", t_5A6450);

// OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_: __func::target_type
static uint32_t t_5A6470() { return 0xf8d714u; }
RW_FUNCTION(0x5A6470, "OtherMenuDialog::openSerialCodeListPopup $_36 UlbE_ __func::target_type", t_5A6470);

// OtherMenuDialog::openSerialCodePopup $_35: __func::~__func (deleting)
static void t_5A6484(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6484, "OtherMenuDialog::openSerialCodePopup $_35 __func::~__func (deleting)", t_5A6484);

// OtherMenuDialog::openSerialCodePopup $_35: __func::__clone()
static uint32_t t_5A648C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5b984u);
    }
    return v1;
}
RW_FUNCTION(0x5A648C, "OtherMenuDialog::openSerialCodePopup $_35 __func::__clone()", t_5A648C);

// OtherMenuDialog::openSerialCodePopup $_35: __func::__clone(__base*)
static void t_5A64B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b984u);
    }
}
RW_FUNCTION(0x5A64B0, "OtherMenuDialog::openSerialCodePopup $_35 __func::__clone(__base*)", t_5A64B0);

// OtherMenuDialog::openSerialCodePopup $_35: __func::destroy
static void t_5A64CC(uint32_t self) {
}
RW_FUNCTION(0x5A64CC, "OtherMenuDialog::openSerialCodePopup $_35 __func::destroy", t_5A64CC);

// OtherMenuDialog::openSerialCodePopup $_35: __func::destroy_deallocate
static void t_5A64CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A64CE, "OtherMenuDialog::openSerialCodePopup $_35 __func::destroy_deallocate", t_5A64CE);

// OtherMenuDialog::openSerialCodePopup $_35: __func::target
static uint32_t t_5A64D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42d00u ? self + 4 : 0; }
RW_FUNCTION(0x5A64D8, "OtherMenuDialog::openSerialCodePopup $_35 __func::target", t_5A64D8);

// OtherMenuDialog::openSerialCodePopup $_35: __func::target_type
static uint32_t t_5A64F8() { return 0xf8d728u; }
RW_FUNCTION(0x5A64F8, "OtherMenuDialog::openSerialCodePopup $_35 __func::target_type", t_5A64F8);

// OtherMenuDialog::openSerialCodePopup $_34: __func::~__func
static uint32_t t_5A650C(uint32_t self) {
    st32(self, 0xf5b9b0u);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    return self;
}
RW_FUNCTION(0x5A650C, "OtherMenuDialog::openSerialCodePopup $_34 __func::~__func", t_5A650C);

// OtherMenuDialog::openSerialCodePopup $_34: __func::~__func (deleting)
static void t_5A6548(uint32_t self) {
    st32(self, 0xf5b9b0u);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6548, "OtherMenuDialog::openSerialCodePopup $_34 __func::~__func (deleting)", t_5A6548);

// OtherMenuDialog::openSerialCodePopup $_34: __func::__clone()
static uint32_t t_5A6588(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {64});
    if (v1) {
        st32(v1, 0xf5b9b0u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        uint32_t v5 = ld32(self + 8 + 12);
        st32(v1 + 20, v5);
        st32(v1 + 16, v4);
        st32(v1 + 12, v3);
        st32(v1 + 8, v2);
        uint32_t v6 = ld32(self + 24 + 0);
        uint32_t v7 = ld32(self + 24 + 4);
        uint32_t v8 = ld32(self + 24 + 8);
        st32(v1 + 24 + 0, v6);
        st32(v1 + 24 + 4, v7);
        st32(v1 + 24 + 8, v8);
        call(0x4683cd, {v1 + 40, self + 40});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A6588, "OtherMenuDialog::openSerialCodePopup $_34 __func::__clone()", t_5A6588);

// OtherMenuDialog::openSerialCodePopup $_34: __func::__clone(__base*)
static void t_5A65F0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b9b0u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        uint32_t v4 = ld32(self + 8 + 12);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        st32(p + 8 + 12, v4);
        uint32_t v5 = ld32(self + 24 + 0);
        uint32_t v6 = ld32(self + 24 + 4);
        uint32_t v7 = ld32(self + 24 + 8);
        st32(p + 24 + 0, v5);
        st32(p + 24 + 4, v6);
        st32(p + 24 + 8, v7);
        call(0x4683cd, {p + 40, self + 40});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A65F0, "OtherMenuDialog::openSerialCodePopup $_34 __func::__clone(__base*)", t_5A65F0);

// OtherMenuDialog::openSerialCodePopup $_34: __func::destroy
static void t_5A663C(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
}
RW_FUNCTION(0x5A663C, "OtherMenuDialog::openSerialCodePopup $_34 __func::destroy", t_5A663C);

// OtherMenuDialog::openSerialCodePopup $_34: __func::destroy_deallocate
static void t_5A665C(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A665C, "OtherMenuDialog::openSerialCodePopup $_34 __func::destroy_deallocate", t_5A665C);

// OtherMenuDialog::openSerialCodePopup $_34: __func::target
static uint32_t t_5A680C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42dd0u ? self + 8 : 0; }
RW_FUNCTION(0x5A680C, "OtherMenuDialog::openSerialCodePopup $_34 __func::target", t_5A680C);

// OtherMenuDialog::openSerialCodePopup $_34: __func::target_type
static uint32_t t_5A6830() { return 0xf8d73cu; }
RW_FUNCTION(0x5A6830, "OtherMenuDialog::openSerialCodePopup $_34 __func::target_type", t_5A6830);

// OtherMenuDialog::openSerialCodePopup $_32 UlvE_: __func::~__func
static uint32_t t_5A6844(uint32_t self) {
    st32(self, 0xf5b9dcu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A6844, "OtherMenuDialog::openSerialCodePopup $_32 UlvE_ __func::~__func", t_5A6844);

// OtherMenuDialog::openSerialCodePopup $_32 UlvE_: __func::~__func (deleting)
static void t_5A6880(uint32_t self) {
    st32(self, 0xf5b9dcu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6880, "OtherMenuDialog::openSerialCodePopup $_32 UlvE_ __func::~__func (deleting)", t_5A6880);

// OtherMenuDialog::openSerialCodePopup $_32 UlvE_: __func::__clone()
static uint32_t t_5A68C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5b9dcu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A68C0, "OtherMenuDialog::openSerialCodePopup $_32 UlvE_ __func::__clone()", t_5A68C0);

// OtherMenuDialog::openSerialCodePopup $_32 UlvE_: __func::__clone(__base*)
static void t_5A691C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5b9dcu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A691C, "OtherMenuDialog::openSerialCodePopup $_32 UlvE_ __func::__clone(__base*)", t_5A691C);

// OtherMenuDialog::openSerialCodePopup $_32 UlvE_: __func::destroy
static void t_5A695C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A695C, "OtherMenuDialog::openSerialCodePopup $_32 UlvE_ __func::destroy", t_5A695C);

// OtherMenuDialog::openSerialCodePopup $_32 UlvE_: __func::destroy_deallocate
static void t_5A697C(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A697C, "OtherMenuDialog::openSerialCodePopup $_32 UlvE_ __func::destroy_deallocate", t_5A697C);

// OtherMenuDialog::openSerialCodePopup $_32 UlvE_: __func::target
static uint32_t t_5A69E4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42eb0u ? self + 8 : 0; }
RW_FUNCTION(0x5A69E4, "OtherMenuDialog::openSerialCodePopup $_32 UlvE_ __func::target", t_5A69E4);

// OtherMenuDialog::openSerialCodePopup $_32 UlvE_: __func::target_type
static uint32_t t_5A6A08() { return 0xf8d750u; }
RW_FUNCTION(0x5A6A08, "OtherMenuDialog::openSerialCodePopup $_32 UlvE_ __func::target_type", t_5A6A08);

// OtherMenuDialog::openSerialCodePopup $_33: __func::~__func (deleting)
static void t_5A6A1C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6A1C, "OtherMenuDialog::openSerialCodePopup $_33 __func::~__func (deleting)", t_5A6A1C);

// OtherMenuDialog::openSerialCodePopup $_33: __func::__clone()
static uint32_t t_5A6A24(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ba08u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A6A24, "OtherMenuDialog::openSerialCodePopup $_33 __func::__clone()", t_5A6A24);

// OtherMenuDialog::openSerialCodePopup $_33: __func::__clone(__base*)
static void t_5A6A50(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ba08u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A6A50, "OtherMenuDialog::openSerialCodePopup $_33 __func::__clone(__base*)", t_5A6A50);

// OtherMenuDialog::openSerialCodePopup $_33: __func::destroy
static void t_5A6A70(uint32_t self) {
}
RW_FUNCTION(0x5A6A70, "OtherMenuDialog::openSerialCodePopup $_33 __func::destroy", t_5A6A70);

// OtherMenuDialog::openSerialCodePopup $_33: __func::destroy_deallocate
static void t_5A6A72(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6A72, "OtherMenuDialog::openSerialCodePopup $_33 __func::destroy_deallocate", t_5A6A72);

// OtherMenuDialog::openSerialCodePopup $_33: __func::target
static uint32_t t_5A6A84(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe42f90u ? self + 4 : 0; }
RW_FUNCTION(0x5A6A84, "OtherMenuDialog::openSerialCodePopup $_33 __func::target", t_5A6A84);

// OtherMenuDialog::openSerialCodePopup $_33: __func::target_type
static uint32_t t_5A6AA4() { return 0xf8d764u; }
RW_FUNCTION(0x5A6AA4, "OtherMenuDialog::openSerialCodePopup $_33 __func::target_type", t_5A6AA4);

// OtherMenuDialog::openSerialCodePopup $_31: __func::~__func (deleting)
static void t_5A6AB8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6AB8, "OtherMenuDialog::openSerialCodePopup $_31 __func::~__func (deleting)", t_5A6AB8);

// OtherMenuDialog::openSerialCodePopup $_31: __func::__clone()
static uint32_t t_5A6AC0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ba34u);
    }
    return v1;
}
RW_FUNCTION(0x5A6AC0, "OtherMenuDialog::openSerialCodePopup $_31 __func::__clone()", t_5A6AC0);

// OtherMenuDialog::openSerialCodePopup $_31: __func::__clone(__base*)
static void t_5A6AE4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ba34u);
    }
}
RW_FUNCTION(0x5A6AE4, "OtherMenuDialog::openSerialCodePopup $_31 __func::__clone(__base*)", t_5A6AE4);

// OtherMenuDialog::openSerialCodePopup $_31: __func::destroy
static void t_5A6B00(uint32_t self) {
}
RW_FUNCTION(0x5A6B00, "OtherMenuDialog::openSerialCodePopup $_31 __func::destroy", t_5A6B00);

// OtherMenuDialog::openSerialCodePopup $_31: __func::destroy_deallocate
static void t_5A6B02(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6B02, "OtherMenuDialog::openSerialCodePopup $_31 __func::destroy_deallocate", t_5A6B02);

// OtherMenuDialog::openSerialCodePopup $_31: __func::target
static uint32_t t_5A6B20(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43090u ? self + 4 : 0; }
RW_FUNCTION(0x5A6B20, "OtherMenuDialog::openSerialCodePopup $_31 __func::target", t_5A6B20);

// OtherMenuDialog::openSerialCodePopup $_31: __func::target_type
static uint32_t t_5A6B40() { return 0xf8d778u; }
RW_FUNCTION(0x5A6B40, "OtherMenuDialog::openSerialCodePopup $_31 __func::target_type", t_5A6B40);

// OtherMenuDialog::openSerialCodePopup $_30: __func::~__func (deleting)
static void t_5A6B54(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6B54, "OtherMenuDialog::openSerialCodePopup $_30 __func::~__func (deleting)", t_5A6B54);

// OtherMenuDialog::openSerialCodePopup $_30: __func::__clone()
static uint32_t t_5A6B5C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf5ba60u);
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
RW_FUNCTION(0x5A6B5C, "OtherMenuDialog::openSerialCodePopup $_30 __func::__clone()", t_5A6B5C);

// OtherMenuDialog::openSerialCodePopup $_30: __func::__clone(__base*)
static void t_5A6B90(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ba60u);
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
RW_FUNCTION(0x5A6B90, "OtherMenuDialog::openSerialCodePopup $_30 __func::__clone(__base*)", t_5A6B90);

// OtherMenuDialog::openSerialCodePopup $_30: __func::destroy
static void t_5A6BBC(uint32_t self) {
}
RW_FUNCTION(0x5A6BBC, "OtherMenuDialog::openSerialCodePopup $_30 __func::destroy", t_5A6BBC);

// OtherMenuDialog::openSerialCodePopup $_30: __func::destroy_deallocate
static void t_5A6BBE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6BBE, "OtherMenuDialog::openSerialCodePopup $_30 __func::destroy_deallocate", t_5A6BBE);

// OtherMenuDialog::openSerialCodePopup $_30: __func::target
static uint32_t t_5A6CCC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43190u ? self + 4 : 0; }
RW_FUNCTION(0x5A6CCC, "OtherMenuDialog::openSerialCodePopup $_30 __func::target", t_5A6CCC);

// OtherMenuDialog::openSerialCodePopup $_30: __func::target_type
static uint32_t t_5A6CEC() { return 0xf8d78cu; }
RW_FUNCTION(0x5A6CEC, "OtherMenuDialog::openSerialCodePopup $_30 __func::target_type", t_5A6CEC);

// OtherMenuDialog::openSerialCodePopup $_30 UlvE_: __func::~__func (deleting)
static void t_5A6D00(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6D00, "OtherMenuDialog::openSerialCodePopup $_30 UlvE_ __func::~__func (deleting)", t_5A6D00);

// OtherMenuDialog::openSerialCodePopup $_30 UlvE_: __func::__clone()
static uint32_t t_5A6D08(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf5ba8cu);
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
RW_FUNCTION(0x5A6D08, "OtherMenuDialog::openSerialCodePopup $_30 UlvE_ __func::__clone()", t_5A6D08);

// OtherMenuDialog::openSerialCodePopup $_30 UlvE_: __func::__clone(__base*)
static void t_5A6D3C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ba8cu);
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
RW_FUNCTION(0x5A6D3C, "OtherMenuDialog::openSerialCodePopup $_30 UlvE_ __func::__clone(__base*)", t_5A6D3C);

// OtherMenuDialog::openSerialCodePopup $_30 UlvE_: __func::destroy
static void t_5A6D68(uint32_t self) {
}
RW_FUNCTION(0x5A6D68, "OtherMenuDialog::openSerialCodePopup $_30 UlvE_ __func::destroy", t_5A6D68);

// OtherMenuDialog::openSerialCodePopup $_30 UlvE_: __func::destroy_deallocate
static void t_5A6D6A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6D6A, "OtherMenuDialog::openSerialCodePopup $_30 UlvE_ __func::destroy_deallocate", t_5A6D6A);

// OtherMenuDialog::openSerialCodePopup $_30 UlvE_: __func::target
static uint32_t t_5A6DB0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43290u ? self + 4 : 0; }
RW_FUNCTION(0x5A6DB0, "OtherMenuDialog::openSerialCodePopup $_30 UlvE_ __func::target", t_5A6DB0);

// OtherMenuDialog::openSerialCodePopup $_30 UlvE_: __func::target_type
static uint32_t t_5A6DD0() { return 0xf8d7a0u; }
RW_FUNCTION(0x5A6DD0, "OtherMenuDialog::openSerialCodePopup $_30 UlvE_ __func::target_type", t_5A6DD0);

// OtherMenuDialog::openSerialCodePopup $_29: __func::~__func (deleting)
static void t_5A6DE4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6DE4, "OtherMenuDialog::openSerialCodePopup $_29 __func::~__func (deleting)", t_5A6DE4);

// OtherMenuDialog::openSerialCodePopup $_29: __func::__clone()
static uint32_t t_5A6DEC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5bab8u);
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
RW_FUNCTION(0x5A6DEC, "OtherMenuDialog::openSerialCodePopup $_29 __func::__clone()", t_5A6DEC);

// OtherMenuDialog::openSerialCodePopup $_29: __func::__clone(__base*)
static void t_5A6E20(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bab8u);
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
RW_FUNCTION(0x5A6E20, "OtherMenuDialog::openSerialCodePopup $_29 __func::__clone(__base*)", t_5A6E20);

// OtherMenuDialog::openSerialCodePopup $_29: __func::destroy
static void t_5A6E48(uint32_t self) {
}
RW_FUNCTION(0x5A6E48, "OtherMenuDialog::openSerialCodePopup $_29 __func::destroy", t_5A6E48);

// OtherMenuDialog::openSerialCodePopup $_29: __func::destroy_deallocate
static void t_5A6E4A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A6E4A, "OtherMenuDialog::openSerialCodePopup $_29 __func::destroy_deallocate", t_5A6E4A);

// OtherMenuDialog::openSerialCodePopup $_29: __func::target
static uint32_t t_5A7120(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe433d0u ? self + 4 : 0; }
RW_FUNCTION(0x5A7120, "OtherMenuDialog::openSerialCodePopup $_29 __func::target", t_5A7120);

// OtherMenuDialog::openSerialCodePopup $_29: __func::target_type
static uint32_t t_5A7140() { return 0xf8d7b4u; }
RW_FUNCTION(0x5A7140, "OtherMenuDialog::openSerialCodePopup $_29 __func::target_type", t_5A7140);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_: __func::~__func (deleting)
static void t_5A7154(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7154, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ __func::~__func (deleting)", t_5A7154);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_: __func::__clone()
static uint32_t t_5A715C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf5bae4u);
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
RW_FUNCTION(0x5A715C, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ __func::__clone()", t_5A715C);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_: __func::__clone(__base*)
static void t_5A7190(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bae4u);
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
RW_FUNCTION(0x5A7190, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ __func::__clone(__base*)", t_5A7190);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_: __func::destroy
static void t_5A71BC(uint32_t self) {
}
RW_FUNCTION(0x5A71BC, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ __func::destroy", t_5A71BC);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_: __func::destroy_deallocate
static void t_5A71BE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A71BE, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ __func::destroy_deallocate", t_5A71BE);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_: __func::target
static uint32_t t_5A73B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe434e0u ? self + 4 : 0; }
RW_FUNCTION(0x5A73B4, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ __func::target", t_5A73B4);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_: __func::target_type
static uint32_t t_5A73D4() { return 0xf8d7c8u; }
RW_FUNCTION(0x5A73D4, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ __func::target_type", t_5A73D4);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_: __func::~__func (deleting)
static void t_5A73E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A73E8, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_ __func::~__func (deleting)", t_5A73E8);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_: __func::__clone()
static uint32_t t_5A73F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf5bb10u);
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
RW_FUNCTION(0x5A73F0, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_ __func::__clone()", t_5A73F0);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_: __func::__clone(__base*)
static void t_5A7424(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bb10u);
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
RW_FUNCTION(0x5A7424, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_ __func::__clone(__base*)", t_5A7424);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_: __func::destroy
static void t_5A7450(uint32_t self) {
}
RW_FUNCTION(0x5A7450, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_ __func::destroy", t_5A7450);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_: __func::destroy_deallocate
static void t_5A7452(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7452, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_ __func::destroy_deallocate", t_5A7452);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_: __func::target
static uint32_t t_5A74A4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43630u ? self + 4 : 0; }
RW_FUNCTION(0x5A74A4, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_ __func::target", t_5A74A4);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_: __func::target_type
static uint32_t t_5A74C4() { return 0xf8d7dcu; }
RW_FUNCTION(0x5A74C4, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE0_ __func::target_type", t_5A74C4);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_: __func::~__func (deleting)
static void t_5A74D8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A74D8, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_ __func::~__func (deleting)", t_5A74D8);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_: __func::__clone()
static uint32_t t_5A74E0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bb3cu);
    }
    return v1;
}
RW_FUNCTION(0x5A74E0, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_ __func::__clone()", t_5A74E0);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_: __func::__clone(__base*)
static void t_5A7504(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bb3cu);
    }
}
RW_FUNCTION(0x5A7504, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_ __func::__clone(__base*)", t_5A7504);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_: __func::destroy
static void t_5A7520(uint32_t self) {
}
RW_FUNCTION(0x5A7520, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_ __func::destroy", t_5A7520);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_: __func::destroy_deallocate
static void t_5A7522(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7522, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_ __func::destroy_deallocate", t_5A7522);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_: __func::target
static uint32_t t_5A752C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43780u ? self + 4 : 0; }
RW_FUNCTION(0x5A752C, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_ __func::target", t_5A752C);

// OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_: __func::target_type
static uint32_t t_5A754C() { return 0xf8d7f0u; }
RW_FUNCTION(0x5A754C, "OtherMenuDialog::openSerialCodePopup $_29 UlbE_ UlbE_ __func::target_type", t_5A754C);

// OtherMenuDialog::openLegalPopup $_28: __func::~__func
static uint32_t t_5A7560(uint32_t self) {
    st32(self, 0xf5bb68u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A7560, "OtherMenuDialog::openLegalPopup $_28 __func::~__func", t_5A7560);

// OtherMenuDialog::openLegalPopup $_28: __func::~__func (deleting)
static void t_5A759C(uint32_t self) {
    st32(self, 0xf5bb68u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A759C, "OtherMenuDialog::openLegalPopup $_28 __func::~__func (deleting)", t_5A759C);

// OtherMenuDialog::openLegalPopup $_28: __func::__clone()
static uint32_t t_5A75DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5bb68u);
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
RW_FUNCTION(0x5A75DC, "OtherMenuDialog::openLegalPopup $_28 __func::__clone()", t_5A75DC);

// OtherMenuDialog::openLegalPopup $_28: __func::__clone(__base*)
static void t_5A7638(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bb68u);
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
RW_FUNCTION(0x5A7638, "OtherMenuDialog::openLegalPopup $_28 __func::__clone(__base*)", t_5A7638);

// OtherMenuDialog::openLegalPopup $_28: __func::destroy
static void t_5A7678(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A7678, "OtherMenuDialog::openLegalPopup $_28 __func::destroy", t_5A7678);

// OtherMenuDialog::openLegalPopup $_28: __func::destroy_deallocate
static void t_5A7698(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7698, "OtherMenuDialog::openLegalPopup $_28 __func::destroy_deallocate", t_5A7698);

// OtherMenuDialog::openLegalPopup $_28: __func::target
static uint32_t t_5A7838(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43890u ? self + 8 : 0; }
RW_FUNCTION(0x5A7838, "OtherMenuDialog::openLegalPopup $_28 __func::target", t_5A7838);

// OtherMenuDialog::openLegalPopup $_28: __func::target_type
static uint32_t t_5A785C() { return 0xf8d804u; }
RW_FUNCTION(0x5A785C, "OtherMenuDialog::openLegalPopup $_28 __func::target_type", t_5A785C);

// OtherMenuDialog::openLegalPopup $_28 UlvE_: __func::~__func
static uint32_t t_5A7870(uint32_t self) {
    st32(self, 0xf5bb94u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x5A7870, "OtherMenuDialog::openLegalPopup $_28 UlvE_ __func::~__func", t_5A7870);

// OtherMenuDialog::openLegalPopup $_28 UlvE_: __func::~__func (deleting)
static void t_5A78AC(uint32_t self) {
    st32(self, 0xf5bb94u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A78AC, "OtherMenuDialog::openLegalPopup $_28 UlvE_ __func::~__func (deleting)", t_5A78AC);

// OtherMenuDialog::openLegalPopup $_28 UlvE_: __func::__clone()
static uint32_t t_5A78EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5bb94u);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A78EC, "OtherMenuDialog::openLegalPopup $_28 UlvE_ __func::__clone()", t_5A78EC);

// OtherMenuDialog::openLegalPopup $_28 UlvE_: __func::__clone(__base*)
static void t_5A7948(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bb94u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A7948, "OtherMenuDialog::openLegalPopup $_28 UlvE_ __func::__clone(__base*)", t_5A7948);

// OtherMenuDialog::openLegalPopup $_28 UlvE_: __func::destroy
static void t_5A7988(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x5A7988, "OtherMenuDialog::openLegalPopup $_28 UlvE_ __func::destroy", t_5A7988);

// OtherMenuDialog::openLegalPopup $_28 UlvE_: __func::destroy_deallocate
static void t_5A79A8(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A79A8, "OtherMenuDialog::openLegalPopup $_28 UlvE_ __func::destroy_deallocate", t_5A79A8);

// OtherMenuDialog::openLegalPopup $_28 UlvE_: __func::target
static uint32_t t_5A7A10(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43960u ? self + 8 : 0; }
RW_FUNCTION(0x5A7A10, "OtherMenuDialog::openLegalPopup $_28 UlvE_ __func::target", t_5A7A10);

// OtherMenuDialog::openLegalPopup $_28 UlvE_: __func::target_type
static uint32_t t_5A7A34() { return 0xf8d818u; }
RW_FUNCTION(0x5A7A34, "OtherMenuDialog::openLegalPopup $_28 UlvE_ __func::target_type", t_5A7A34);

// OtherMenuDialog::openLegalPopup $_27: __func::~__func (deleting)
static void t_5A7A48(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7A48, "OtherMenuDialog::openLegalPopup $_27 __func::~__func (deleting)", t_5A7A48);

// OtherMenuDialog::openLegalPopup $_27: __func::__clone()
static uint32_t t_5A7A50(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bbc0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A7A50, "OtherMenuDialog::openLegalPopup $_27 __func::__clone()", t_5A7A50);

// OtherMenuDialog::openLegalPopup $_27: __func::__clone(__base*)
static void t_5A7A7C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bbc0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A7A7C, "OtherMenuDialog::openLegalPopup $_27 __func::__clone(__base*)", t_5A7A7C);

// OtherMenuDialog::openLegalPopup $_27: __func::destroy
static void t_5A7A9C(uint32_t self) {
}
RW_FUNCTION(0x5A7A9C, "OtherMenuDialog::openLegalPopup $_27 __func::destroy", t_5A7A9C);

// OtherMenuDialog::openLegalPopup $_27: __func::destroy_deallocate
static void t_5A7A9E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7A9E, "OtherMenuDialog::openLegalPopup $_27 __func::destroy_deallocate", t_5A7A9E);

// OtherMenuDialog::openLegalPopup $_27: __func::target
static uint32_t t_5A7AB0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43a30u ? self + 4 : 0; }
RW_FUNCTION(0x5A7AB0, "OtherMenuDialog::openLegalPopup $_27 __func::target", t_5A7AB0);

// OtherMenuDialog::openLegalPopup $_27: __func::target_type
static uint32_t t_5A7AD0() { return 0xf8d82cu; }
RW_FUNCTION(0x5A7AD0, "OtherMenuDialog::openLegalPopup $_27 __func::target_type", t_5A7AD0);

// OtherMenuDialog::openLegalPopup $_26: __func::~__func (deleting)
static void t_5A7AE4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7AE4, "OtherMenuDialog::openLegalPopup $_26 __func::~__func (deleting)", t_5A7AE4);

// OtherMenuDialog::openLegalPopup $_26: __func::__clone()
static uint32_t t_5A7AEC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bbecu);
    }
    return v1;
}
RW_FUNCTION(0x5A7AEC, "OtherMenuDialog::openLegalPopup $_26 __func::__clone()", t_5A7AEC);

// OtherMenuDialog::openLegalPopup $_26: __func::__clone(__base*)
static void t_5A7B10(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bbecu);
    }
}
RW_FUNCTION(0x5A7B10, "OtherMenuDialog::openLegalPopup $_26 __func::__clone(__base*)", t_5A7B10);

// OtherMenuDialog::openLegalPopup $_26: __func::destroy
static void t_5A7B2C(uint32_t self) {
}
RW_FUNCTION(0x5A7B2C, "OtherMenuDialog::openLegalPopup $_26 __func::destroy", t_5A7B2C);

// OtherMenuDialog::openLegalPopup $_26: __func::destroy_deallocate
static void t_5A7B2E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7B2E, "OtherMenuDialog::openLegalPopup $_26 __func::destroy_deallocate", t_5A7B2E);

// OtherMenuDialog::openLegalPopup $_26: __func::target
static uint32_t t_5A7B38(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43b00u ? self + 4 : 0; }
RW_FUNCTION(0x5A7B38, "OtherMenuDialog::openLegalPopup $_26 __func::target", t_5A7B38);

// OtherMenuDialog::openLegalPopup $_26: __func::target_type
static uint32_t t_5A7B58() { return 0xf8d840u; }
RW_FUNCTION(0x5A7B58, "OtherMenuDialog::openLegalPopup $_26 __func::target_type", t_5A7B58);

// OtherMenuDialog::openLegalPopup $_25: __func::~__func (deleting)
static void t_5A7B6C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7B6C, "OtherMenuDialog::openLegalPopup $_25 __func::~__func (deleting)", t_5A7B6C);

// OtherMenuDialog::openLegalPopup $_25: __func::__clone()
static uint32_t t_5A7B74(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bc18u);
    }
    return v1;
}
RW_FUNCTION(0x5A7B74, "OtherMenuDialog::openLegalPopup $_25 __func::__clone()", t_5A7B74);

// OtherMenuDialog::openLegalPopup $_25: __func::__clone(__base*)
static void t_5A7B98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bc18u);
    }
}
RW_FUNCTION(0x5A7B98, "OtherMenuDialog::openLegalPopup $_25 __func::__clone(__base*)", t_5A7B98);

// OtherMenuDialog::openLegalPopup $_25: __func::destroy
static void t_5A7BB4(uint32_t self) {
}
RW_FUNCTION(0x5A7BB4, "OtherMenuDialog::openLegalPopup $_25 __func::destroy", t_5A7BB4);

// OtherMenuDialog::openLegalPopup $_25: __func::destroy_deallocate
static void t_5A7BB6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7BB6, "OtherMenuDialog::openLegalPopup $_25 __func::destroy_deallocate", t_5A7BB6);

// OtherMenuDialog::openLegalPopup $_25: __func::target
static uint32_t t_5A7D78(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43bf0u ? self + 4 : 0; }
RW_FUNCTION(0x5A7D78, "OtherMenuDialog::openLegalPopup $_25 __func::target", t_5A7D78);

// OtherMenuDialog::openLegalPopup $_25: __func::target_type
static uint32_t t_5A7D98() { return 0xf8d854u; }
RW_FUNCTION(0x5A7D98, "OtherMenuDialog::openLegalPopup $_25 __func::target_type", t_5A7D98);

// OtherMenuDialog::openLegalPopup $_24: __func::~__func (deleting)
static void t_5A7DAC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7DAC, "OtherMenuDialog::openLegalPopup $_24 __func::~__func (deleting)", t_5A7DAC);

// OtherMenuDialog::openLegalPopup $_24: __func::__clone()
static uint32_t t_5A7DB4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bc44u);
    }
    return v1;
}
RW_FUNCTION(0x5A7DB4, "OtherMenuDialog::openLegalPopup $_24 __func::__clone()", t_5A7DB4);

// OtherMenuDialog::openLegalPopup $_24: __func::__clone(__base*)
static void t_5A7DD8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bc44u);
    }
}
RW_FUNCTION(0x5A7DD8, "OtherMenuDialog::openLegalPopup $_24 __func::__clone(__base*)", t_5A7DD8);

// OtherMenuDialog::openLegalPopup $_24: __func::destroy
static void t_5A7DF4(uint32_t self) {
}
RW_FUNCTION(0x5A7DF4, "OtherMenuDialog::openLegalPopup $_24 __func::destroy", t_5A7DF4);

// OtherMenuDialog::openLegalPopup $_24: __func::destroy_deallocate
static void t_5A7DF6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7DF6, "OtherMenuDialog::openLegalPopup $_24 __func::destroy_deallocate", t_5A7DF6);

// OtherMenuDialog::openLegalPopup $_24: __func::target
static uint32_t t_5A7FB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43ce0u ? self + 4 : 0; }
RW_FUNCTION(0x5A7FB8, "OtherMenuDialog::openLegalPopup $_24 __func::target", t_5A7FB8);

// OtherMenuDialog::openLegalPopup $_24: __func::target_type
static uint32_t t_5A7FD8() { return 0xf8d868u; }
RW_FUNCTION(0x5A7FD8, "OtherMenuDialog::openLegalPopup $_24 __func::target_type", t_5A7FD8);

// OtherMenuDialog::openLegalPopup $_23: __func::~__func (deleting)
static void t_5A7FEC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A7FEC, "OtherMenuDialog::openLegalPopup $_23 __func::~__func (deleting)", t_5A7FEC);

// OtherMenuDialog::openLegalPopup $_23: __func::__clone()
static uint32_t t_5A7FF4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bc70u);
    }
    return v1;
}
RW_FUNCTION(0x5A7FF4, "OtherMenuDialog::openLegalPopup $_23 __func::__clone()", t_5A7FF4);

// OtherMenuDialog::openLegalPopup $_23: __func::__clone(__base*)
static void t_5A8018(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bc70u);
    }
}
RW_FUNCTION(0x5A8018, "OtherMenuDialog::openLegalPopup $_23 __func::__clone(__base*)", t_5A8018);

// OtherMenuDialog::openLegalPopup $_23: __func::destroy
static void t_5A8034(uint32_t self) {
}
RW_FUNCTION(0x5A8034, "OtherMenuDialog::openLegalPopup $_23 __func::destroy", t_5A8034);

// OtherMenuDialog::openLegalPopup $_23: __func::destroy_deallocate
static void t_5A8036(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A8036, "OtherMenuDialog::openLegalPopup $_23 __func::destroy_deallocate", t_5A8036);

// OtherMenuDialog::openLegalPopup $_23: __func::target
static uint32_t t_5A8054(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43dd0u ? self + 4 : 0; }
RW_FUNCTION(0x5A8054, "OtherMenuDialog::openLegalPopup $_23 __func::target", t_5A8054);

// OtherMenuDialog::openLegalPopup $_23: __func::target_type
static uint32_t t_5A8074() { return 0xf8d87cu; }
RW_FUNCTION(0x5A8074, "OtherMenuDialog::openLegalPopup $_23 __func::target_type", t_5A8074);

// OtherMenuDialog::close $_22: __func::~__func (deleting)
static void t_5A8088(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A8088, "OtherMenuDialog::close $_22 __func::~__func (deleting)", t_5A8088);

// OtherMenuDialog::close $_22: __func::__clone()
static uint32_t t_5A8090(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bc9cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A8090, "OtherMenuDialog::close $_22 __func::__clone()", t_5A8090);

// OtherMenuDialog::close $_22: __func::__clone(__base*)
static void t_5A80BC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bc9cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5A80BC, "OtherMenuDialog::close $_22 __func::__clone(__base*)", t_5A80BC);

// OtherMenuDialog::close $_22: __func::destroy
static void t_5A80DC(uint32_t self) {
}
RW_FUNCTION(0x5A80DC, "OtherMenuDialog::close $_22 __func::destroy", t_5A80DC);

// OtherMenuDialog::close $_22: __func::destroy_deallocate
static void t_5A80DE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A80DE, "OtherMenuDialog::close $_22 __func::destroy_deallocate", t_5A80DE);

// OtherMenuDialog::close $_22: __func::target
static uint32_t t_5A811C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43e80u ? self + 4 : 0; }
RW_FUNCTION(0x5A811C, "OtherMenuDialog::close $_22 __func::target", t_5A811C);

// OtherMenuDialog::close $_22: __func::target_type
static uint32_t t_5A813C() { return 0xf8d890u; }
RW_FUNCTION(0x5A813C, "OtherMenuDialog::close $_22 __func::target_type", t_5A813C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func
static uint32_t t_5A8150(uint32_t self) {
    st32(self, 0xf5bcc8u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A8150, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func", t_5A8150);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func (deleting)
static void t_5A818C(uint32_t self) {
    st32(self, 0xf5bcc8u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A818C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func (deleting)", t_5A818C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone()
static uint32_t t_5A81CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5bcc8u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 32);
        st32(v1 + 32, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A81CC, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone()", t_5A81CC);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone(__base*)
static void t_5A8220(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bcc8u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v1 = ld32(self + 32);
        st32(p + 32, v1);
    }
}
RW_FUNCTION(0x5A8220, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone(__base*)", t_5A8220);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy
static void t_5A825C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A825C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy", t_5A825C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy_deallocate
static void t_5A827C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A827C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy_deallocate", t_5A827C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target
static uint32_t t_5A865C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe43f90u ? self + 8 : 0; }
RW_FUNCTION(0x5A865C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target", t_5A865C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target_type
static uint32_t t_5A867C() { return 0xf8d8a4u; }
RW_FUNCTION(0x5A867C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target_type", t_5A867C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func
static uint32_t t_5A8690(uint32_t self) {
    st32(self, 0xf5bcf4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A8690, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func", t_5A8690);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func (deleting)
static void t_5A86CC(uint32_t self) {
    st32(self, 0xf5bcf4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A86CC, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func (deleting)", t_5A86CC);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone()
static uint32_t t_5A870C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5bcf4u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A870C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone()", t_5A870C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone(__base*)
static void t_5A875C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bcf4u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A875C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone(__base*)", t_5A875C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy
static void t_5A8790(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A8790, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy", t_5A8790);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy_deallocate
static void t_5A87B0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A87B0, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy_deallocate", t_5A87B0);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target
static uint32_t t_5A8A90(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe440d0u ? self + 8 : 0; }
RW_FUNCTION(0x5A8A90, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target", t_5A8A90);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target_type
static uint32_t t_5A8AB0() { return 0xf8d8b8u; }
RW_FUNCTION(0x5A8AB0, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target_type", t_5A8AB0);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func
static uint32_t t_5A8AC4(uint32_t self) {
    st32(self, 0xf5bd20u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A8AC4, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func", t_5A8AC4);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func (deleting)
static void t_5A8B00(uint32_t self) {
    st32(self, 0xf5bd20u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A8B00, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func (deleting)", t_5A8B00);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone()
static uint32_t t_5A8B40(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5bd20u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A8B40, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone()", t_5A8B40);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone(__base*)
static void t_5A8B90(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bd20u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A8B90, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone(__base*)", t_5A8B90);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy
static void t_5A8BC4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A8BC4, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy", t_5A8BC4);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy_deallocate
static void t_5A8BE4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A8BE4, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy_deallocate", t_5A8BE4);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target
static uint32_t t_5A8C24(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe44280u ? self + 8 : 0; }
RW_FUNCTION(0x5A8C24, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target", t_5A8C24);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target_type
static uint32_t t_5A8C44() { return 0xf8d8ccu; }
RW_FUNCTION(0x5A8C44, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target_type", t_5A8C44);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func
static uint32_t t_5A8C58(uint32_t self) {
    st32(self, 0xf5bd4cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A8C58, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func", t_5A8C58);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func (deleting)
static void t_5A8C94(uint32_t self) {
    st32(self, 0xf5bd4cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A8C94, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func (deleting)", t_5A8C94);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone()
static uint32_t t_5A8CD4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5bd4cu);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A8CD4, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone()", t_5A8CD4);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone(__base*)
static void t_5A8D24(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bd4cu);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A8D24, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone(__base*)", t_5A8D24);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy
static void t_5A8D58(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A8D58, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy", t_5A8D58);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy_deallocate
static void t_5A8D78(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A8D78, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy_deallocate", t_5A8D78);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target
static uint32_t t_5A8F24(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe44440u ? self + 8 : 0; }
RW_FUNCTION(0x5A8F24, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target", t_5A8F24);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target_type
static uint32_t t_5A8F44() { return 0xf8d8e0u; }
RW_FUNCTION(0x5A8F44, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target_type", t_5A8F44);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func
static uint32_t t_5A8F58(uint32_t self) {
    st32(self, 0xf5bd78u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A8F58, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func", t_5A8F58);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func (deleting)
static void t_5A8F94(uint32_t self) {
    st32(self, 0xf5bd78u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A8F94, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func (deleting)", t_5A8F94);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone()
static uint32_t t_5A8FD4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5bd78u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A8FD4, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone()", t_5A8FD4);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone(__base*)
static void t_5A9024(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bd78u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A9024, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone(__base*)", t_5A9024);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy
static void t_5A9058(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A9058, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy", t_5A9058);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy_deallocate
static void t_5A9078(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A9078, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy_deallocate", t_5A9078);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target
static uint32_t t_5A90EC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe44620u ? self + 8 : 0; }
RW_FUNCTION(0x5A90EC, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target", t_5A90EC);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target_type
static uint32_t t_5A910C() { return 0xf8d8f4u; }
RW_FUNCTION(0x5A910C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target_type", t_5A910C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func
static uint32_t t_5A9120(uint32_t self) {
    st32(self, 0xf5bda4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A9120, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func", t_5A9120);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::~__func (deleting)
static void t_5A915C(uint32_t self) {
    st32(self, 0xf5bda4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A915C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::~__func (deleting)", t_5A915C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone()
static uint32_t t_5A919C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5bda4u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A919C, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone()", t_5A919C);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::__clone(__base*)
static void t_5A91EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bda4u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A91EC, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::__clone(__base*)", t_5A91EC);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy
static void t_5A9220(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A9220, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy", t_5A9220);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::destroy_deallocate
static void t_5A9240(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A9240, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::destroy_deallocate", t_5A9240);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target
static uint32_t t_5A9280(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe447f0u ? self + 8 : 0; }
RW_FUNCTION(0x5A9280, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target", t_5A9280);

// OtherMenuDialog::openFacebookLoginLogout $_21: __func::target_type
static uint32_t t_5A92A0() { return 0xf8d908u; }
RW_FUNCTION(0x5A92A0, "OtherMenuDialog::openFacebookLoginLogout $_21 __func::target_type", t_5A92A0);

// OtherMenuDialog::openFacebookLoginLogout $_20: __func::~__func
static uint32_t t_5A92B4(uint32_t self) {
    st32(self, 0xf5bdd0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A92B4, "OtherMenuDialog::openFacebookLoginLogout $_20 __func::~__func", t_5A92B4);

// OtherMenuDialog::openFacebookLoginLogout $_20: __func::~__func (deleting)
static void t_5A92F0(uint32_t self) {
    st32(self, 0xf5bdd0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A92F0, "OtherMenuDialog::openFacebookLoginLogout $_20 __func::~__func (deleting)", t_5A92F0);

// OtherMenuDialog::openFacebookLoginLogout $_20: __func::__clone()
static uint32_t t_5A9330(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5bdd0u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 32);
        st32(v1 + 32, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A9330, "OtherMenuDialog::openFacebookLoginLogout $_20 __func::__clone()", t_5A9330);

// OtherMenuDialog::openFacebookLoginLogout $_20: __func::__clone(__base*)
static void t_5A9384(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bdd0u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v1 = ld32(self + 32);
        st32(p + 32, v1);
    }
}
RW_FUNCTION(0x5A9384, "OtherMenuDialog::openFacebookLoginLogout $_20 __func::__clone(__base*)", t_5A9384);

// OtherMenuDialog::openFacebookLoginLogout $_20: __func::destroy
static void t_5A93C0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A93C0, "OtherMenuDialog::openFacebookLoginLogout $_20 __func::destroy", t_5A93C0);

// OtherMenuDialog::openFacebookLoginLogout $_20: __func::destroy_deallocate
static void t_5A93E0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A93E0, "OtherMenuDialog::openFacebookLoginLogout $_20 __func::destroy_deallocate", t_5A93E0);

// OtherMenuDialog::openFacebookLoginLogout $_20: __func::target
static uint32_t t_5A9608(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe44930u ? self + 8 : 0; }
RW_FUNCTION(0x5A9608, "OtherMenuDialog::openFacebookLoginLogout $_20 __func::target", t_5A9608);

// OtherMenuDialog::openFacebookLoginLogout $_20: __func::target_type
static uint32_t t_5A9628() { return 0xf8d91cu; }
RW_FUNCTION(0x5A9628, "OtherMenuDialog::openFacebookLoginLogout $_20 __func::target_type", t_5A9628);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::~__func
static uint32_t t_5A963C(uint32_t self) {
    st32(self, 0xf5bdfcu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A963C, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::~__func", t_5A963C);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::~__func (deleting)
static void t_5A9678(uint32_t self) {
    st32(self, 0xf5bdfcu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A9678, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::~__func (deleting)", t_5A9678);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::__clone()
static uint32_t t_5A96B8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5bdfcu);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 32);
        st32(v1 + 32, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A96B8, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::__clone()", t_5A96B8);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::__clone(__base*)
static void t_5A970C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bdfcu);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v1 = ld32(self + 32);
        st32(p + 32, v1);
    }
}
RW_FUNCTION(0x5A970C, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::__clone(__base*)", t_5A970C);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::destroy
static void t_5A9748(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A9748, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::destroy", t_5A9748);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::destroy_deallocate
static void t_5A9768(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A9768, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::destroy_deallocate", t_5A9768);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::target
static uint32_t t_5A9A14(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe44a10u ? self + 8 : 0; }
RW_FUNCTION(0x5A9A14, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::target", t_5A9A14);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::target_type
static uint32_t t_5A9A34() { return 0xf8d930u; }
RW_FUNCTION(0x5A9A34, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::target_type", t_5A9A34);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::~__func
static uint32_t t_5A9A94(uint32_t self) {
    st32(self, 0xf5be28u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A9A94, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::~__func", t_5A9A94);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::~__func (deleting)
static void t_5A9AD0(uint32_t self) {
    st32(self, 0xf5be28u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A9AD0, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::~__func (deleting)", t_5A9AD0);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::__clone()
static uint32_t t_5A9B10(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5be28u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 32);
        st32(v1 + 32, v2);
    }
    return v1;
}
RW_FUNCTION(0x5A9B10, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::__clone()", t_5A9B10);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::__clone(__base*)
static void t_5A9B64(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5be28u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v1 = ld32(self + 32);
        st32(p + 32, v1);
    }
}
RW_FUNCTION(0x5A9B64, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::__clone(__base*)", t_5A9B64);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::destroy
static void t_5A9BA0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A9BA0, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::destroy", t_5A9BA0);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::destroy_deallocate
static void t_5A9BC0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A9BC0, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::destroy_deallocate", t_5A9BC0);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::target
static uint32_t t_5A9E30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe44b70u ? self + 8 : 0; }
RW_FUNCTION(0x5A9E30, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::target", t_5A9E30);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::target_type
static uint32_t t_5A9E50() { return 0xf8d944u; }
RW_FUNCTION(0x5A9E50, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::target_type", t_5A9E50);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::~__func
static uint32_t t_5A9E64(uint32_t self) {
    st32(self, 0xf5be54u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A9E64, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::~__func", t_5A9E64);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::~__func (deleting)
static void t_5A9EA0(uint32_t self) {
    st32(self, 0xf5be54u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A9EA0, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::~__func (deleting)", t_5A9EA0);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::__clone()
static uint32_t t_5A9EE0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5be54u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5A9EE0, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::__clone()", t_5A9EE0);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::__clone(__base*)
static void t_5A9F30(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5be54u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x5A9F30, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::__clone(__base*)", t_5A9F30);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::destroy
static void t_5A9F64(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5A9F64, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::destroy", t_5A9F64);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::destroy_deallocate
static void t_5A9F84(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5A9F84, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::destroy_deallocate", t_5A9F84);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::target
static uint32_t t_5A9FC4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe44d50u ? self + 8 : 0; }
RW_FUNCTION(0x5A9FC4, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::target", t_5A9FC4);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_: __func::target_type
static uint32_t t_5A9FE4() { return 0xf8d958u; }
RW_FUNCTION(0x5A9FE4, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ __func::target_type", t_5A9FE4);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_: __func::~__func
static uint32_t t_5A9FF8(uint32_t self) {
    st32(self, 0xf5be80u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5A9FF8, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_ __func::~__func", t_5A9FF8);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_: __func::~__func (deleting)
static void t_5AA034(uint32_t self) {
    st32(self, 0xf5be80u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA034, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_ __func::~__func (deleting)", t_5AA034);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_: __func::__clone()
static uint32_t t_5AA074(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5be80u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 32);
        st32(v1 + 32, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AA074, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_ __func::__clone()", t_5AA074);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_: __func::__clone(__base*)
static void t_5AA0C8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5be80u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v1 = ld32(self + 32);
        st32(p + 32, v1);
    }
}
RW_FUNCTION(0x5AA0C8, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_ __func::__clone(__base*)", t_5AA0C8);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_: __func::destroy
static void t_5AA104(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5AA104, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_ __func::destroy", t_5AA104);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_: __func::destroy_deallocate
static void t_5AA124(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA124, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_ __func::destroy_deallocate", t_5AA124);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_: __func::target
static uint32_t t_5AA244(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe44ed0u ? self + 8 : 0; }
RW_FUNCTION(0x5AA244, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_ __func::target", t_5AA244);

// OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_: __func::target_type
static uint32_t t_5AA264() { return 0xf8d96cu; }
RW_FUNCTION(0x5AA264, "OtherMenuDialog::openFacebookLoginLogout $_20 UlbE_ UlbE_ __func::target_type", t_5AA264);

// OtherMenuDialog::priOpenUserCodePopup $_18: __func::~__func (deleting)
static void t_5AA278(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA278, "OtherMenuDialog::priOpenUserCodePopup $_18 __func::~__func (deleting)", t_5AA278);

// OtherMenuDialog::priOpenUserCodePopup $_18: __func::__clone()
static uint32_t t_5AA280(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5beacu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AA280, "OtherMenuDialog::priOpenUserCodePopup $_18 __func::__clone()", t_5AA280);

// OtherMenuDialog::priOpenUserCodePopup $_18: __func::__clone(__base*)
static void t_5AA2AC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5beacu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AA2AC, "OtherMenuDialog::priOpenUserCodePopup $_18 __func::__clone(__base*)", t_5AA2AC);

// OtherMenuDialog::priOpenUserCodePopup $_18: __func::destroy
static void t_5AA2CC(uint32_t self) {
}
RW_FUNCTION(0x5AA2CC, "OtherMenuDialog::priOpenUserCodePopup $_18 __func::destroy", t_5AA2CC);

// OtherMenuDialog::priOpenUserCodePopup $_18: __func::destroy_deallocate
static void t_5AA2CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA2CE, "OtherMenuDialog::priOpenUserCodePopup $_18 __func::destroy_deallocate", t_5AA2CE);

// OtherMenuDialog::priOpenUserCodePopup $_18: __func::target
static uint32_t t_5AA544(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe44fb0u ? self + 4 : 0; }
RW_FUNCTION(0x5AA544, "OtherMenuDialog::priOpenUserCodePopup $_18 __func::target", t_5AA544);

// OtherMenuDialog::priOpenUserCodePopup $_18: __func::target_type
static uint32_t t_5AA564() { return 0xf8d980u; }
RW_FUNCTION(0x5AA564, "OtherMenuDialog::priOpenUserCodePopup $_18 __func::target_type", t_5AA564);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_: __func::~__func (deleting)
static void t_5AA578(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA578, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_ __func::~__func (deleting)", t_5AA578);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_: __func::__clone()
static uint32_t t_5AA580(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bed8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AA580, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_ __func::__clone()", t_5AA580);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_: __func::__clone(__base*)
static void t_5AA5AC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bed8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AA5AC, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_ __func::__clone(__base*)", t_5AA5AC);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_: __func::destroy
static void t_5AA5CC(uint32_t self) {
}
RW_FUNCTION(0x5AA5CC, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_ __func::destroy", t_5AA5CC);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_: __func::destroy_deallocate
static void t_5AA5CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA5CE, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_ __func::destroy_deallocate", t_5AA5CE);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_: __func::target
static uint32_t t_5AA660(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45070u ? self + 4 : 0; }
RW_FUNCTION(0x5AA660, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_ __func::target", t_5AA660);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_: __func::target_type
static uint32_t t_5AA680() { return 0xf8d994u; }
RW_FUNCTION(0x5AA680, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE0_ __func::target_type", t_5AA680);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_: __func::~__func (deleting)
static void t_5AA694(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA694, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_ __func::~__func (deleting)", t_5AA694);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_: __func::__clone()
static uint32_t t_5AA69C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bf04u);
    }
    return v1;
}
RW_FUNCTION(0x5AA69C, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_ __func::__clone()", t_5AA69C);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_: __func::__clone(__base*)
static void t_5AA6C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bf04u);
    }
}
RW_FUNCTION(0x5AA6C0, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_ __func::__clone(__base*)", t_5AA6C0);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_: __func::destroy
static void t_5AA6DC(uint32_t self) {
}
RW_FUNCTION(0x5AA6DC, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_ __func::destroy", t_5AA6DC);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_: __func::destroy_deallocate
static void t_5AA6DE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA6DE, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_ __func::destroy_deallocate", t_5AA6DE);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_: __func::target
static uint32_t t_5AA6E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45130u ? self + 4 : 0; }
RW_FUNCTION(0x5AA6E8, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_ __func::target", t_5AA6E8);

// OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_: __func::target_type
static uint32_t t_5AA708() { return 0xf8d9a8u; }
RW_FUNCTION(0x5AA708, "OtherMenuDialog::priOpenUserCodePopup $_18 UlbE_ __func::target_type", t_5AA708);

// OtherMenuDialog::open $_17: __func::~__func (deleting)
static void t_5AA71C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA71C, "OtherMenuDialog::open $_17 __func::~__func (deleting)", t_5AA71C);

// OtherMenuDialog::open $_17: __func::__clone()
static uint32_t t_5AA724(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bf30u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AA724, "OtherMenuDialog::open $_17 __func::__clone()", t_5AA724);

// OtherMenuDialog::open $_17: __func::__clone(__base*)
static void t_5AA750(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bf30u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AA750, "OtherMenuDialog::open $_17 __func::__clone(__base*)", t_5AA750);

// OtherMenuDialog::open $_17: __func::destroy
static void t_5AA770(uint32_t self) {
}
RW_FUNCTION(0x5AA770, "OtherMenuDialog::open $_17 __func::destroy", t_5AA770);

// OtherMenuDialog::open $_17: __func::destroy_deallocate
static void t_5AA772(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA772, "OtherMenuDialog::open $_17 __func::destroy_deallocate", t_5AA772);

// OtherMenuDialog::open $_17: __func::target
static uint32_t t_5AA784(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe451e0u ? self + 4 : 0; }
RW_FUNCTION(0x5AA784, "OtherMenuDialog::open $_17 __func::target", t_5AA784);

// OtherMenuDialog::open $_17: __func::target_type
static uint32_t t_5AA7A4() { return 0xf8d9bcu; }
RW_FUNCTION(0x5AA7A4, "OtherMenuDialog::open $_17 __func::target_type", t_5AA7A4);

// OtherMenuDialog::open $_16: __func::~__func (deleting)
static void t_5AA7B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA7B8, "OtherMenuDialog::open $_16 __func::~__func (deleting)", t_5AA7B8);

// OtherMenuDialog::open $_16: __func::__clone()
static uint32_t t_5AA7C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5bf5cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AA7C0, "OtherMenuDialog::open $_16 __func::__clone()", t_5AA7C0);

// OtherMenuDialog::open $_16: __func::__clone(__base*)
static void t_5AA7EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bf5cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AA7EC, "OtherMenuDialog::open $_16 __func::__clone(__base*)", t_5AA7EC);

// OtherMenuDialog::open $_16: __func::destroy
static void t_5AA80C(uint32_t self) {
}
RW_FUNCTION(0x5AA80C, "OtherMenuDialog::open $_16 __func::destroy", t_5AA80C);

// OtherMenuDialog::open $_16: __func::destroy_deallocate
static void t_5AA80E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA80E, "OtherMenuDialog::open $_16 __func::destroy_deallocate", t_5AA80E);

// OtherMenuDialog::open $_16: __func::target
static uint32_t t_5AA820(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45280u ? self + 4 : 0; }
RW_FUNCTION(0x5AA820, "OtherMenuDialog::open $_16 __func::target", t_5AA820);

// OtherMenuDialog::open $_16: __func::target_type
static uint32_t t_5AA840() { return 0xf8d9d0u; }
RW_FUNCTION(0x5AA840, "OtherMenuDialog::open $_16 __func::target_type", t_5AA840);

// OtherMenuDialog::open $_15: __func::~__func (deleting)
static void t_5AA854(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA854, "OtherMenuDialog::open $_15 __func::~__func (deleting)", t_5AA854);

// OtherMenuDialog::open $_15: __func::__clone()
static uint32_t t_5AA85C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5bf88u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x5AA85C, "OtherMenuDialog::open $_15 __func::__clone()", t_5AA85C);

// OtherMenuDialog::open $_15: __func::__clone(__base*)
static void t_5AA88C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bf88u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AA88C, "OtherMenuDialog::open $_15 __func::__clone(__base*)", t_5AA88C);

// OtherMenuDialog::open $_15: __func::destroy
static void t_5AA8B4(uint32_t self) {
}
RW_FUNCTION(0x5AA8B4, "OtherMenuDialog::open $_15 __func::destroy", t_5AA8B4);

// OtherMenuDialog::open $_15: __func::destroy_deallocate
static void t_5AA8B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AA8B6, "OtherMenuDialog::open $_15 __func::destroy_deallocate", t_5AA8B6);

// OtherMenuDialog::open $_15: __func::target
static uint32_t t_5AA9D0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45340u ? self + 4 : 0; }
RW_FUNCTION(0x5AA9D0, "OtherMenuDialog::open $_15 __func::target", t_5AA9D0);

// OtherMenuDialog::open $_15: __func::target_type
static uint32_t t_5AA9F0() { return 0xf8d9e4u; }
RW_FUNCTION(0x5AA9F0, "OtherMenuDialog::open $_15 __func::target_type", t_5AA9F0);

// OtherMenuDialog::open $_66 UlvE_: __func::~__func
static uint32_t t_5AAC98(uint32_t self) {
    st32(self, 0xf5bfb4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5AAC98, "OtherMenuDialog::open $_66 UlvE_ __func::~__func", t_5AAC98);

// OtherMenuDialog::open $_66 UlvE_: __func::~__func (deleting)
static void t_5AACD4(uint32_t self) {
    st32(self, 0xf5bfb4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AACD4, "OtherMenuDialog::open $_66 UlvE_ __func::~__func (deleting)", t_5AACD4);

// OtherMenuDialog::open $_66 UlvE_: __func::destroy
static void t_5AAD98(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5AAD98, "OtherMenuDialog::open $_66 UlvE_ __func::destroy", t_5AAD98);

// OtherMenuDialog::open $_66 UlvE_: __func::destroy_deallocate
static void t_5AADB8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AADB8, "OtherMenuDialog::open $_66 UlvE_ __func::destroy_deallocate", t_5AADB8);

// OtherMenuDialog::open $_66 UlvE_: __func::target
static uint32_t t_5AAE08(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45400u ? self + 8 : 0; }
RW_FUNCTION(0x5AAE08, "OtherMenuDialog::open $_66 UlvE_ __func::target", t_5AAE08);

// OtherMenuDialog::open $_66 UlvE_: __func::target_type
static uint32_t t_5AAE28() { return 0xf8d9f8u; }
RW_FUNCTION(0x5AAE28, "OtherMenuDialog::open $_66 UlvE_ __func::target_type", t_5AAE28);

// OtherMenuDialog::open $_15 UlbE_ UlbE_: __func::~__func (deleting)
static void t_5AAE72(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AAE72, "OtherMenuDialog::open $_15 UlbE_ UlbE_ __func::~__func (deleting)", t_5AAE72);

// OtherMenuDialog::open $_15 UlbE_ UlbE_: __func::__clone()
static uint32_t t_5AAE7C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5bfe0u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AAE7C, "OtherMenuDialog::open $_15 UlbE_ UlbE_ __func::__clone()", t_5AAE7C);

// OtherMenuDialog::open $_15 UlbE_ UlbE_: __func::__clone(__base*)
static void t_5AAEAC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5bfe0u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AAEAC, "OtherMenuDialog::open $_15 UlbE_ UlbE_ __func::__clone(__base*)", t_5AAEAC);

// OtherMenuDialog::open $_15 UlbE_ UlbE_: __func::destroy
static void t_5AAED0(uint32_t self) {
}
RW_FUNCTION(0x5AAED0, "OtherMenuDialog::open $_15 UlbE_ UlbE_ __func::destroy", t_5AAED0);

// OtherMenuDialog::open $_15 UlbE_ UlbE_: __func::destroy_deallocate
static void t_5AAED2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AAED2, "OtherMenuDialog::open $_15 UlbE_ UlbE_ __func::destroy_deallocate", t_5AAED2);

// OtherMenuDialog::open $_15 UlbE_ UlbE_: __func::target
static uint32_t t_5AAEE8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45510u ? self + 4 : 0; }
RW_FUNCTION(0x5AAEE8, "OtherMenuDialog::open $_15 UlbE_ UlbE_ __func::target", t_5AAEE8);

// OtherMenuDialog::open $_15 UlbE_ UlbE_: __func::target_type
static uint32_t t_5AAF08() { return 0xf8da0cu; }
RW_FUNCTION(0x5AAF08, "OtherMenuDialog::open $_15 UlbE_ UlbE_ __func::target_type", t_5AAF08);

// OtherMenuDialog::open $_15: __func::~__func (deleting)
static void t_5AAF1C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AAF1C, "OtherMenuDialog::open $_15 __func::~__func (deleting)", t_5AAF1C);

// OtherMenuDialog::open $_15: __func::__clone()
static uint32_t t_5AAF24(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5c00cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x5AAF24, "OtherMenuDialog::open $_15 __func::__clone()", t_5AAF24);

// OtherMenuDialog::open $_15: __func::__clone(__base*)
static void t_5AAF54(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c00cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AAF54, "OtherMenuDialog::open $_15 __func::__clone(__base*)", t_5AAF54);

// OtherMenuDialog::open $_15: __func::destroy
static void t_5AAF7C(uint32_t self) {
}
RW_FUNCTION(0x5AAF7C, "OtherMenuDialog::open $_15 __func::destroy", t_5AAF7C);

// OtherMenuDialog::open $_15: __func::destroy_deallocate
static void t_5AAF7E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AAF7E, "OtherMenuDialog::open $_15 __func::destroy_deallocate", t_5AAF7E);

// OtherMenuDialog::open $_15: __func::target
static uint32_t t_5AB078(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45690u ? self + 4 : 0; }
RW_FUNCTION(0x5AB078, "OtherMenuDialog::open $_15 __func::target", t_5AB078);

// OtherMenuDialog::open $_15: __func::target_type
static uint32_t t_5AB098() { return 0xf8da20u; }
RW_FUNCTION(0x5AB098, "OtherMenuDialog::open $_15 __func::target_type", t_5AB098);

// OtherMenuDialog::open $_14: __func::~__func (deleting)
static void t_5AB0AC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB0AC, "OtherMenuDialog::open $_14 __func::~__func (deleting)", t_5AB0AC);

// OtherMenuDialog::open $_14: __func::__clone()
static uint32_t t_5AB0B4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c038u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AB0B4, "OtherMenuDialog::open $_14 __func::__clone()", t_5AB0B4);

// OtherMenuDialog::open $_14: __func::__clone(__base*)
static void t_5AB0E0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c038u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AB0E0, "OtherMenuDialog::open $_14 __func::__clone(__base*)", t_5AB0E0);

// OtherMenuDialog::open $_14: __func::destroy
static void t_5AB100(uint32_t self) {
}
RW_FUNCTION(0x5AB100, "OtherMenuDialog::open $_14 __func::destroy", t_5AB100);

// OtherMenuDialog::open $_14: __func::destroy_deallocate
static void t_5AB102(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB102, "OtherMenuDialog::open $_14 __func::destroy_deallocate", t_5AB102);

// OtherMenuDialog::open $_14: __func::target
static uint32_t t_5AB124(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe457f0u ? self + 4 : 0; }
RW_FUNCTION(0x5AB124, "OtherMenuDialog::open $_14 __func::target", t_5AB124);

// OtherMenuDialog::open $_14: __func::target_type
static uint32_t t_5AB144() { return 0xf8da34u; }
RW_FUNCTION(0x5AB144, "OtherMenuDialog::open $_14 __func::target_type", t_5AB144);

// OtherMenuDialog::open $_13: __func::~__func (deleting)
static void t_5AB158(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB158, "OtherMenuDialog::open $_13 __func::~__func (deleting)", t_5AB158);

// OtherMenuDialog::open $_13: __func::__clone()
static uint32_t t_5AB160(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c064u);
    }
    return v1;
}
RW_FUNCTION(0x5AB160, "OtherMenuDialog::open $_13 __func::__clone()", t_5AB160);

// OtherMenuDialog::open $_13: __func::__clone(__base*)
static void t_5AB184(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c064u);
    }
}
RW_FUNCTION(0x5AB184, "OtherMenuDialog::open $_13 __func::__clone(__base*)", t_5AB184);

// OtherMenuDialog::open $_13: __func::destroy
static void t_5AB1A0(uint32_t self) {
}
RW_FUNCTION(0x5AB1A0, "OtherMenuDialog::open $_13 __func::destroy", t_5AB1A0);

// OtherMenuDialog::open $_13: __func::destroy_deallocate
static void t_5AB1A2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB1A2, "OtherMenuDialog::open $_13 __func::destroy_deallocate", t_5AB1A2);

// OtherMenuDialog::open $_13: __func::target
static uint32_t t_5AB368(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe458b0u ? self + 4 : 0; }
RW_FUNCTION(0x5AB368, "OtherMenuDialog::open $_13 __func::target", t_5AB368);

// OtherMenuDialog::open $_13: __func::target_type
static uint32_t t_5AB388() { return 0xf8da48u; }
RW_FUNCTION(0x5AB388, "OtherMenuDialog::open $_13 __func::target_type", t_5AB388);

// OtherMenuDialog::open $_12: __func::~__func (deleting)
static void t_5AB39C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB39C, "OtherMenuDialog::open $_12 __func::~__func (deleting)", t_5AB39C);

// OtherMenuDialog::open $_12: __func::__clone()
static uint32_t t_5AB3A4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c090u);
    }
    return v1;
}
RW_FUNCTION(0x5AB3A4, "OtherMenuDialog::open $_12 __func::__clone()", t_5AB3A4);

// OtherMenuDialog::open $_12: __func::__clone(__base*)
static void t_5AB3C8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c090u);
    }
}
RW_FUNCTION(0x5AB3C8, "OtherMenuDialog::open $_12 __func::__clone(__base*)", t_5AB3C8);

// OtherMenuDialog::open $_12: __func::destroy
static void t_5AB3E4(uint32_t self) {
}
RW_FUNCTION(0x5AB3E4, "OtherMenuDialog::open $_12 __func::destroy", t_5AB3E4);

// OtherMenuDialog::open $_12: __func::destroy_deallocate
static void t_5AB3E6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB3E6, "OtherMenuDialog::open $_12 __func::destroy_deallocate", t_5AB3E6);

// OtherMenuDialog::open $_12: __func::target
static uint32_t t_5AB400(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45970u ? self + 4 : 0; }
RW_FUNCTION(0x5AB400, "OtherMenuDialog::open $_12 __func::target", t_5AB400);

// OtherMenuDialog::open $_12: __func::target_type
static uint32_t t_5AB420() { return 0xf8da5cu; }
RW_FUNCTION(0x5AB420, "OtherMenuDialog::open $_12 __func::target_type", t_5AB420);

// OtherMenuDialog::open $_11: __func::~__func (deleting)
static void t_5AB434(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB434, "OtherMenuDialog::open $_11 __func::~__func (deleting)", t_5AB434);

// OtherMenuDialog::open $_11: __func::__clone()
static uint32_t t_5AB43C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c0bcu);
    }
    return v1;
}
RW_FUNCTION(0x5AB43C, "OtherMenuDialog::open $_11 __func::__clone()", t_5AB43C);

// OtherMenuDialog::open $_11: __func::__clone(__base*)
static void t_5AB460(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c0bcu);
    }
}
RW_FUNCTION(0x5AB460, "OtherMenuDialog::open $_11 __func::__clone(__base*)", t_5AB460);

// OtherMenuDialog::open $_11: __func::destroy
static void t_5AB47C(uint32_t self) {
}
RW_FUNCTION(0x5AB47C, "OtherMenuDialog::open $_11 __func::destroy", t_5AB47C);

// OtherMenuDialog::open $_11: __func::destroy_deallocate
static void t_5AB47E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB47E, "OtherMenuDialog::open $_11 __func::destroy_deallocate", t_5AB47E);

// OtherMenuDialog::open $_11: __func::target
static uint32_t t_5AB65C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45a30u ? self + 4 : 0; }
RW_FUNCTION(0x5AB65C, "OtherMenuDialog::open $_11 __func::target", t_5AB65C);

// OtherMenuDialog::open $_11: __func::target_type
static uint32_t t_5AB67C() { return 0xf8da70u; }
RW_FUNCTION(0x5AB67C, "OtherMenuDialog::open $_11 __func::target_type", t_5AB67C);

// OtherMenuDialog::open $_10: __func::~__func (deleting)
static void t_5AB690(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB690, "OtherMenuDialog::open $_10 __func::~__func (deleting)", t_5AB690);

// OtherMenuDialog::open $_10: __func::__clone()
static uint32_t t_5AB698(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c0e8u);
    }
    return v1;
}
RW_FUNCTION(0x5AB698, "OtherMenuDialog::open $_10 __func::__clone()", t_5AB698);

// OtherMenuDialog::open $_10: __func::__clone(__base*)
static void t_5AB6BC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c0e8u);
    }
}
RW_FUNCTION(0x5AB6BC, "OtherMenuDialog::open $_10 __func::__clone(__base*)", t_5AB6BC);

// OtherMenuDialog::open $_10: __func::destroy
static void t_5AB6D8(uint32_t self) {
}
RW_FUNCTION(0x5AB6D8, "OtherMenuDialog::open $_10 __func::destroy", t_5AB6D8);

// OtherMenuDialog::open $_10: __func::destroy_deallocate
static void t_5AB6DA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB6DA, "OtherMenuDialog::open $_10 __func::destroy_deallocate", t_5AB6DA);

// OtherMenuDialog::open $_10: __func::target
static uint32_t t_5AB7EC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45af0u ? self + 4 : 0; }
RW_FUNCTION(0x5AB7EC, "OtherMenuDialog::open $_10 __func::target", t_5AB7EC);

// OtherMenuDialog::open $_10: __func::target_type
static uint32_t t_5AB80C() { return 0xf8da84u; }
RW_FUNCTION(0x5AB80C, "OtherMenuDialog::open $_10 __func::target_type", t_5AB80C);

// OtherMenuDialog::open $_10 UlbE_: __func::~__func (deleting)
static void t_5AB820(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB820, "OtherMenuDialog::open $_10 UlbE_ __func::~__func (deleting)", t_5AB820);

// OtherMenuDialog::open $_10 UlbE_: __func::__clone()
static uint32_t t_5AB828(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c114u);
    }
    return v1;
}
RW_FUNCTION(0x5AB828, "OtherMenuDialog::open $_10 UlbE_ __func::__clone()", t_5AB828);

// OtherMenuDialog::open $_10 UlbE_: __func::__clone(__base*)
static void t_5AB84C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c114u);
    }
}
RW_FUNCTION(0x5AB84C, "OtherMenuDialog::open $_10 UlbE_ __func::__clone(__base*)", t_5AB84C);

// OtherMenuDialog::open $_10 UlbE_: __func::destroy
static void t_5AB868(uint32_t self) {
}
RW_FUNCTION(0x5AB868, "OtherMenuDialog::open $_10 UlbE_ __func::destroy", t_5AB868);

// OtherMenuDialog::open $_10 UlbE_: __func::destroy_deallocate
static void t_5AB86A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AB86A, "OtherMenuDialog::open $_10 UlbE_ __func::destroy_deallocate", t_5AB86A);

// OtherMenuDialog::open $_10 UlbE_: __func::target
static uint32_t t_5ABA84(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45bc0u ? self + 4 : 0; }
RW_FUNCTION(0x5ABA84, "OtherMenuDialog::open $_10 UlbE_ __func::target", t_5ABA84);

// OtherMenuDialog::open $_10 UlbE_: __func::target_type
static uint32_t t_5ABAA4() { return 0xf8da98u; }
RW_FUNCTION(0x5ABAA4, "OtherMenuDialog::open $_10 UlbE_ __func::target_type", t_5ABAA4);

// OtherMenuDialog::open $_9: __func::~__func (deleting)
static void t_5ABAB8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ABAB8, "OtherMenuDialog::open $_9 __func::~__func (deleting)", t_5ABAB8);

// OtherMenuDialog::open $_9: __func::__clone()
static uint32_t t_5ABAC0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c140u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5ABAC0, "OtherMenuDialog::open $_9 __func::__clone()", t_5ABAC0);

// OtherMenuDialog::open $_9: __func::__clone(__base*)
static void t_5ABAEC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c140u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5ABAEC, "OtherMenuDialog::open $_9 __func::__clone(__base*)", t_5ABAEC);

// OtherMenuDialog::open $_9: __func::destroy
static void t_5ABB0C(uint32_t self) {
}
RW_FUNCTION(0x5ABB0C, "OtherMenuDialog::open $_9 __func::destroy", t_5ABB0C);

// OtherMenuDialog::open $_9: __func::destroy_deallocate
static void t_5ABB0E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ABB0E, "OtherMenuDialog::open $_9 __func::destroy_deallocate", t_5ABB0E);

// OtherMenuDialog::open $_9: __func::target
static uint32_t t_5ABBA8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45cc0u ? self + 4 : 0; }
RW_FUNCTION(0x5ABBA8, "OtherMenuDialog::open $_9 __func::target", t_5ABBA8);

// OtherMenuDialog::open $_9: __func::target_type
static uint32_t t_5ABBC8() { return 0xf8daacu; }
RW_FUNCTION(0x5ABBC8, "OtherMenuDialog::open $_9 __func::target_type", t_5ABBC8);

// OtherMenuDialog::open $_8: __func::~__func (deleting)
static void t_5ABBDC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ABBDC, "OtherMenuDialog::open $_8 __func::~__func (deleting)", t_5ABBDC);

// OtherMenuDialog::open $_8: __func::__clone()
static uint32_t t_5ABBE4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c16cu);
    }
    return v1;
}
RW_FUNCTION(0x5ABBE4, "OtherMenuDialog::open $_8 __func::__clone()", t_5ABBE4);

// OtherMenuDialog::open $_8: __func::__clone(__base*)
static void t_5ABC08(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c16cu);
    }
}
RW_FUNCTION(0x5ABC08, "OtherMenuDialog::open $_8 __func::__clone(__base*)", t_5ABC08);

// OtherMenuDialog::open $_8: __func::destroy
static void t_5ABC24(uint32_t self) {
}
RW_FUNCTION(0x5ABC24, "OtherMenuDialog::open $_8 __func::destroy", t_5ABC24);

// OtherMenuDialog::open $_8: __func::destroy_deallocate
static void t_5ABC26(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ABC26, "OtherMenuDialog::open $_8 __func::destroy_deallocate", t_5ABC26);

// OtherMenuDialog::open $_8: __func::target
static uint32_t t_5ABD38(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45d80u ? self + 4 : 0; }
RW_FUNCTION(0x5ABD38, "OtherMenuDialog::open $_8 __func::target", t_5ABD38);

// OtherMenuDialog::open $_8: __func::target_type
static uint32_t t_5ABD58() { return 0xf8dac0u; }
RW_FUNCTION(0x5ABD58, "OtherMenuDialog::open $_8 __func::target_type", t_5ABD58);

// OtherMenuDialog::open $_8 UlbE_: __func::~__func (deleting)
static void t_5ABD6C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ABD6C, "OtherMenuDialog::open $_8 UlbE_ __func::~__func (deleting)", t_5ABD6C);

// OtherMenuDialog::open $_8 UlbE_: __func::__clone()
static uint32_t t_5ABD74(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c198u);
    }
    return v1;
}
RW_FUNCTION(0x5ABD74, "OtherMenuDialog::open $_8 UlbE_ __func::__clone()", t_5ABD74);

// OtherMenuDialog::open $_8 UlbE_: __func::__clone(__base*)
static void t_5ABD98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c198u);
    }
}
RW_FUNCTION(0x5ABD98, "OtherMenuDialog::open $_8 UlbE_ __func::__clone(__base*)", t_5ABD98);

// OtherMenuDialog::open $_8 UlbE_: __func::destroy
static void t_5ABDB4(uint32_t self) {
}
RW_FUNCTION(0x5ABDB4, "OtherMenuDialog::open $_8 UlbE_ __func::destroy", t_5ABDB4);

// OtherMenuDialog::open $_8 UlbE_: __func::destroy_deallocate
static void t_5ABDB6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ABDB6, "OtherMenuDialog::open $_8 UlbE_ __func::destroy_deallocate", t_5ABDB6);

// OtherMenuDialog::open $_8 UlbE_: __func::target
static uint32_t t_5AC0DC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45e50u ? self + 4 : 0; }
RW_FUNCTION(0x5AC0DC, "OtherMenuDialog::open $_8 UlbE_ __func::target", t_5AC0DC);

// OtherMenuDialog::open $_8 UlbE_: __func::target_type
static uint32_t t_5AC0FC() { return 0xf8dad4u; }
RW_FUNCTION(0x5AC0FC, "OtherMenuDialog::open $_8 UlbE_ __func::target_type", t_5AC0FC);

// OtherMenuDialog::open $_7: __func::~__func (deleting)
static void t_5AC110(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC110, "OtherMenuDialog::open $_7 __func::~__func (deleting)", t_5AC110);

// OtherMenuDialog::open $_7: __func::__clone()
static uint32_t t_5AC118(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c1c4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AC118, "OtherMenuDialog::open $_7 __func::__clone()", t_5AC118);

// OtherMenuDialog::open $_7: __func::__clone(__base*)
static void t_5AC144(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c1c4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AC144, "OtherMenuDialog::open $_7 __func::__clone(__base*)", t_5AC144);

// OtherMenuDialog::open $_7: __func::destroy
static void t_5AC164(uint32_t self) {
}
RW_FUNCTION(0x5AC164, "OtherMenuDialog::open $_7 __func::destroy", t_5AC164);

// OtherMenuDialog::open $_7: __func::destroy_deallocate
static void t_5AC166(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC166, "OtherMenuDialog::open $_7 __func::destroy_deallocate", t_5AC166);

// OtherMenuDialog::open $_7: __func::target
static uint32_t t_5AC204(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe45f50u ? self + 4 : 0; }
RW_FUNCTION(0x5AC204, "OtherMenuDialog::open $_7 __func::target", t_5AC204);

// OtherMenuDialog::open $_7: __func::target_type
static uint32_t t_5AC224() { return 0xf8dae8u; }
RW_FUNCTION(0x5AC224, "OtherMenuDialog::open $_7 __func::target_type", t_5AC224);

// OtherMenuDialog::open $_6: __func::~__func (deleting)
static void t_5AC238(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC238, "OtherMenuDialog::open $_6 __func::~__func (deleting)", t_5AC238);

// OtherMenuDialog::open $_6: __func::__clone()
static uint32_t t_5AC240(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c1f0u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AC240, "OtherMenuDialog::open $_6 __func::__clone()", t_5AC240);

// OtherMenuDialog::open $_6: __func::__clone(__base*)
static void t_5AC26C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c1f0u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x5AC26C, "OtherMenuDialog::open $_6 __func::__clone(__base*)", t_5AC26C);

// OtherMenuDialog::open $_6: __func::destroy
static void t_5AC28C(uint32_t self) {
}
RW_FUNCTION(0x5AC28C, "OtherMenuDialog::open $_6 __func::destroy", t_5AC28C);

// OtherMenuDialog::open $_6: __func::destroy_deallocate
static void t_5AC28E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC28E, "OtherMenuDialog::open $_6 __func::destroy_deallocate", t_5AC28E);

// OtherMenuDialog::open $_6: __func::target
static uint32_t t_5AC454(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe46010u ? self + 4 : 0; }
RW_FUNCTION(0x5AC454, "OtherMenuDialog::open $_6 __func::target", t_5AC454);

// OtherMenuDialog::open $_6: __func::target_type
static uint32_t t_5AC474() { return 0xf8dafcu; }
RW_FUNCTION(0x5AC474, "OtherMenuDialog::open $_6 __func::target_type", t_5AC474);

// OtherMenuDialog::open $_6 UlbE_: __func::~__func (deleting)
static void t_5AC488(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC488, "OtherMenuDialog::open $_6 UlbE_ __func::~__func (deleting)", t_5AC488);

// OtherMenuDialog::open $_6 UlbE_: __func::__clone()
static uint32_t t_5AC490(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c21cu);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AC490, "OtherMenuDialog::open $_6 UlbE_ __func::__clone()", t_5AC490);

// OtherMenuDialog::open $_6 UlbE_: __func::__clone(__base*)
static void t_5AC4BC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c21cu);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x5AC4BC, "OtherMenuDialog::open $_6 UlbE_ __func::__clone(__base*)", t_5AC4BC);

// OtherMenuDialog::open $_6 UlbE_: __func::destroy
static void t_5AC4DC(uint32_t self) {
}
RW_FUNCTION(0x5AC4DC, "OtherMenuDialog::open $_6 UlbE_ __func::destroy", t_5AC4DC);

// OtherMenuDialog::open $_6 UlbE_: __func::destroy_deallocate
static void t_5AC4DE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC4DE, "OtherMenuDialog::open $_6 UlbE_ __func::destroy_deallocate", t_5AC4DE);

// OtherMenuDialog::open $_6 UlbE_: __func::target
static uint32_t t_5AC594(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe460e0u ? self + 4 : 0; }
RW_FUNCTION(0x5AC594, "OtherMenuDialog::open $_6 UlbE_ __func::target", t_5AC594);

// OtherMenuDialog::open $_6 UlbE_: __func::target_type
static uint32_t t_5AC5B4() { return 0xf8db10u; }
RW_FUNCTION(0x5AC5B4, "OtherMenuDialog::open $_6 UlbE_ __func::target_type", t_5AC5B4);

// OtherMenuDialog::open $_5: __func::~__func (deleting)
static void t_5AC658(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC658, "OtherMenuDialog::open $_5 __func::~__func (deleting)", t_5AC658);

// OtherMenuDialog::open $_5: __func::__clone()
static uint32_t t_5AC660(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5c248u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AC660, "OtherMenuDialog::open $_5 __func::__clone()", t_5AC660);

// OtherMenuDialog::open $_5: __func::__clone(__base*)
static void t_5AC690(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c248u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5AC690, "OtherMenuDialog::open $_5 __func::__clone(__base*)", t_5AC690);

// OtherMenuDialog::open $_5: __func::destroy
static void t_5AC6B4(uint32_t self) {
}
RW_FUNCTION(0x5AC6B4, "OtherMenuDialog::open $_5 __func::destroy", t_5AC6B4);

// OtherMenuDialog::open $_5: __func::destroy_deallocate
static void t_5AC6B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC6B6, "OtherMenuDialog::open $_5 __func::destroy_deallocate", t_5AC6B6);

// OtherMenuDialog::open $_5: __func::target
static uint32_t t_5AC800(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe461e0u ? self + 4 : 0; }
RW_FUNCTION(0x5AC800, "OtherMenuDialog::open $_5 __func::target", t_5AC800);

// OtherMenuDialog::open $_5: __func::target_type
static uint32_t t_5AC820() { return 0xf8db24u; }
RW_FUNCTION(0x5AC820, "OtherMenuDialog::open $_5 __func::target_type", t_5AC820);

// OtherMenuDialog::open $_4: __func::~__func (deleting)
static void t_5AC834(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC834, "OtherMenuDialog::open $_4 __func::~__func (deleting)", t_5AC834);

// OtherMenuDialog::open $_4: __func::__clone()
static uint32_t t_5AC83C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c274u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5AC83C, "OtherMenuDialog::open $_4 __func::__clone()", t_5AC83C);

// OtherMenuDialog::open $_4: __func::__clone(__base*)
static void t_5AC868(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c274u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x5AC868, "OtherMenuDialog::open $_4 __func::__clone(__base*)", t_5AC868);

// OtherMenuDialog::open $_4: __func::destroy
static void t_5AC888(uint32_t self) {
}
RW_FUNCTION(0x5AC888, "OtherMenuDialog::open $_4 __func::destroy", t_5AC888);

// OtherMenuDialog::open $_4: __func::destroy_deallocate
static void t_5AC88A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5AC88A, "OtherMenuDialog::open $_4 __func::destroy_deallocate", t_5AC88A);

// OtherMenuDialog::open $_4: __func::target
static uint32_t t_5AC9C0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe462a0u ? self + 4 : 0; }
RW_FUNCTION(0x5AC9C0, "OtherMenuDialog::open $_4 __func::target", t_5AC9C0);

// OtherMenuDialog::open $_4: __func::target_type
static uint32_t t_5AC9E0() { return 0xf8db38u; }
RW_FUNCTION(0x5AC9E0, "OtherMenuDialog::open $_4 __func::target_type", t_5AC9E0);

// OtherMenuDialog::open $_3: __func::~__func (deleting)
static void t_5ACA88(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ACA88, "OtherMenuDialog::open $_3 __func::~__func (deleting)", t_5ACA88);

// OtherMenuDialog::open $_3: __func::__clone()
static uint32_t t_5ACA90(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c2a0u);
    }
    return v1;
}
RW_FUNCTION(0x5ACA90, "OtherMenuDialog::open $_3 __func::__clone()", t_5ACA90);

// OtherMenuDialog::open $_3: __func::__clone(__base*)
static void t_5ACAB4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c2a0u);
    }
}
RW_FUNCTION(0x5ACAB4, "OtherMenuDialog::open $_3 __func::__clone(__base*)", t_5ACAB4);

// OtherMenuDialog::open $_3: __func::destroy
static void t_5ACAD0(uint32_t self) {
}
RW_FUNCTION(0x5ACAD0, "OtherMenuDialog::open $_3 __func::destroy", t_5ACAD0);

// OtherMenuDialog::open $_3: __func::destroy_deallocate
static void t_5ACAD2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ACAD2, "OtherMenuDialog::open $_3 __func::destroy_deallocate", t_5ACAD2);

// OtherMenuDialog::open $_3: __func::target
static uint32_t t_5ACBD0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe46360u ? self + 4 : 0; }
RW_FUNCTION(0x5ACBD0, "OtherMenuDialog::open $_3 __func::target", t_5ACBD0);

// OtherMenuDialog::open $_3: __func::target_type
static uint32_t t_5ACBF0() { return 0xf8db4cu; }
RW_FUNCTION(0x5ACBF0, "OtherMenuDialog::open $_3 __func::target_type", t_5ACBF0);

// OtherMenuDialog::open $_3 UlvE_: __func::~__func (deleting)
static void t_5ACC04(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ACC04, "OtherMenuDialog::open $_3 UlvE_ __func::~__func (deleting)", t_5ACC04);

// OtherMenuDialog::open $_3 UlvE_: __func::__clone()
static uint32_t t_5ACC0C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c2ccu);
    }
    return v1;
}
RW_FUNCTION(0x5ACC0C, "OtherMenuDialog::open $_3 UlvE_ __func::__clone()", t_5ACC0C);

// OtherMenuDialog::open $_3 UlvE_: __func::__clone(__base*)
static void t_5ACC30(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c2ccu);
    }
}
RW_FUNCTION(0x5ACC30, "OtherMenuDialog::open $_3 UlvE_ __func::__clone(__base*)", t_5ACC30);

// OtherMenuDialog::open $_3 UlvE_: __func::destroy
static void t_5ACC4C(uint32_t self) {
}
RW_FUNCTION(0x5ACC4C, "OtherMenuDialog::open $_3 UlvE_ __func::destroy", t_5ACC4C);

// OtherMenuDialog::open $_3 UlvE_: __func::destroy_deallocate
static void t_5ACC4E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ACC4E, "OtherMenuDialog::open $_3 UlvE_ __func::destroy_deallocate", t_5ACC4E);

// OtherMenuDialog::open $_3 UlvE_: __func::target
static uint32_t t_5ACC64(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe46430u ? self + 4 : 0; }
RW_FUNCTION(0x5ACC64, "OtherMenuDialog::open $_3 UlvE_ __func::target", t_5ACC64);

// OtherMenuDialog::open $_3 UlvE_: __func::target_type
static uint32_t t_5ACC84() { return 0xf8db60u; }
RW_FUNCTION(0x5ACC84, "OtherMenuDialog::open $_3 UlvE_ __func::target_type", t_5ACC84);

// OtherMenuDialog::open $_1: __func::~__func (deleting)
static void t_5ACC98(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ACC98, "OtherMenuDialog::open $_1 __func::~__func (deleting)", t_5ACC98);

// OtherMenuDialog::open $_1: __func::__clone()
static uint32_t t_5ACCA0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5c2f8u);
    }
    return v1;
}
RW_FUNCTION(0x5ACCA0, "OtherMenuDialog::open $_1 __func::__clone()", t_5ACCA0);

// OtherMenuDialog::open $_1: __func::__clone(__base*)
static void t_5ACCC4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5c2f8u);
    }
}
RW_FUNCTION(0x5ACCC4, "OtherMenuDialog::open $_1 __func::__clone(__base*)", t_5ACCC4);

// OtherMenuDialog::open $_1: __func::destroy
static void t_5ACCE0(uint32_t self) {
}
RW_FUNCTION(0x5ACCE0, "OtherMenuDialog::open $_1 __func::destroy", t_5ACCE0);

// OtherMenuDialog::open $_1: __func::destroy_deallocate
static void t_5ACCE2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5ACCE2, "OtherMenuDialog::open $_1 __func::destroy_deallocate", t_5ACCE2);

// OtherMenuDialog::open $_1: __func::target
static uint32_t t_5ACD9C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe46530u ? self + 4 : 0; }
RW_FUNCTION(0x5ACD9C, "OtherMenuDialog::open $_1 __func::target", t_5ACD9C);
