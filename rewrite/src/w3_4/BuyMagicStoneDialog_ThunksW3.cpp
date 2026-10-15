// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the BuyMagicStoneDialog lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_58F324(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3ac50u ? self + 8 : 0; }
RW_FUNCTION(0x58F324, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_58F324);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_58F344() { return 0xf8bd94u; }
RW_FUNCTION(0x58F344, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_58F344);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_58F358(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58F358, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_58F358);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_58F360(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5a854u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x58F360, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_58F360);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_58F390(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a854u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x58F390, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_58F390);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_58F3B4(uint32_t self) {
}
RW_FUNCTION(0x58F3B4, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_58F3B4);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_58F3B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58F3B6, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_58F3B6);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_58F3D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3add0u ? self + 4 : 0; }
RW_FUNCTION(0x58F3D8, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_58F3D8);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_58F3F8() { return 0xf8bda8u; }
RW_FUNCTION(0x58F3F8, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_58F3F8);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_58F40C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58F40C, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_58F40C);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_58F414(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a880u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x58F414, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_58F414);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_58F440(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a880u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x58F440, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_58F440);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_58F460(uint32_t self) {
}
RW_FUNCTION(0x58F460, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_58F460);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_58F462(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58F462, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_58F462);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_58F474(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3b020u ? self + 4 : 0; }
RW_FUNCTION(0x58F474, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_58F474);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_58F494() { return 0xf8bdbcu; }
RW_FUNCTION(0x58F494, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_58F494);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func
static uint32_t t_58F4A8(uint32_t self) {
    st32(self, 0xf5a8acu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x58F4A8, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func", t_58F4A8);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_58F4E4(uint32_t self) {
    st32(self, 0xf5a8acu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58F4E4, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_58F4E4);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_58F524(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5a8acu);
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
RW_FUNCTION(0x58F524, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_58F524);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_58F580(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a8acu);
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
RW_FUNCTION(0x58F580, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_58F580);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_58F5C0(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x58F5C0, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_58F5C0);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_58F5E0(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58F5E0, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_58F5E0);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_58F788(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3b2a0u ? self + 8 : 0; }
RW_FUNCTION(0x58F788, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_58F788);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_58F7AC() { return 0xf8bdd0u; }
RW_FUNCTION(0x58F7AC, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_58F7AC);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func
static uint32_t t_58F7C0(uint32_t self) {
    st32(self, 0xf5a8d8u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x58F7C0, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func", t_58F7C0);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_58F7FC(uint32_t self) {
    st32(self, 0xf5a8d8u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58F7FC, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_58F7FC);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_58F83C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5a8d8u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x58F83C, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_58F83C);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_58F894(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a8d8u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x58F894, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_58F894);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_58F8D0(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x58F8D0, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_58F8D0);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_58F8F0(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58F8F0, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_58F8F0);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_58F96C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3b550u ? self + 8 : 0; }
RW_FUNCTION(0x58F96C, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_58F96C);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_58F990() { return 0xf8bde4u; }
RW_FUNCTION(0x58F990, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_58F990);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func
static uint32_t t_58F9A4(uint32_t self) {
    st32(self, 0xf5a904u);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x58F9A4, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func", t_58F9A4);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_58F9EC(uint32_t self) {
    st32(self, 0xf5a904u);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58F9EC, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_58F9EC);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_58FA38(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {64});
    if (v1) {
        st32(v1, 0xf5a904u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 12, v3);
        st32(v1 + 8, v2);
        uint32_t v4 = ld32(self + 16);
        st32(v1 + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(v1 + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
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
RW_FUNCTION(0x58FA38, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_58FA38);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_58FABC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a904u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 12, v2);
        st32(p + 8, v1);
        uint32_t v3 = ld32(self + 16);
        st32(p + 16, v3);
        uint32_t v4 = ld32(self + 20);
        st32(p + 20, v4);
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
        }
        uint32_t v5 = ld32(self + 24 + 0);
        uint32_t v6 = ld32(self + 24 + 4);
        uint32_t v7 = ld32(self + 24 + 8);
        st32(p + 24 + 0, v5);
        st32(p + 24 + 4, v6);
        st32(p + 24 + 8, v7);
        call(0x4683cd, {p + 40, self + 40});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x58FABC, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_58FABC);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_58FB2C(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x58FB2C, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_58FB2C);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_58FB5A(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58FB5A, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_58FB5A);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_58FDEC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3b810u ? self + 8 : 0; }
RW_FUNCTION(0x58FDEC, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_58FDEC);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_58FE10() { return 0xf8bdf8u; }
RW_FUNCTION(0x58FE10, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_58FE10);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func
static uint32_t t_58FE24(uint32_t self) {
    st32(self, 0xf5a930u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    return self;
}
RW_FUNCTION(0x58FE24, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func", t_58FE24);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_58FE60(uint32_t self) {
    st32(self, 0xf5a930u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58FE60, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_58FE60);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_58FEA0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {64});
    if (v1) {
        st32(v1, 0xf5a930u);
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
        uint32_t v7 = ld32(self + 56);
        st32(v1 + 56, v7);
    }
    return v1;
}
RW_FUNCTION(0x58FEA0, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_58FEA0);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_58FF04(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a930u);
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
        uint32_t v6 = ld32(self + 56);
        st32(p + 56, v6);
    }
}
RW_FUNCTION(0x58FF04, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_58FF04);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_58FF50(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
}
RW_FUNCTION(0x58FF50, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_58FF50);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_58FF70(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58FF70, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_58FF70);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_590164(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3bad0u ? self + 8 : 0; }
RW_FUNCTION(0x590164, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_590164);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_590188() { return 0xf8be0cu; }
RW_FUNCTION(0x590188, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_590188);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_59019C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x59019C, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_59019C);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_5901A4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5a95cu);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5901A4, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_5901A4);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_5901D4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a95cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5901D4, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_5901D4);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_5901F8(uint32_t self) {
}
RW_FUNCTION(0x5901F8, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_5901F8);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_5901FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5901FA, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_5901FA);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_590228(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3bda0u ? self + 4 : 0; }
RW_FUNCTION(0x590228, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_590228);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_590248() { return 0xf8be20u; }
RW_FUNCTION(0x590248, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_590248);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func
static uint32_t t_59025C(uint32_t self) {
    st32(self, 0xf5a988u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x59025C, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func", t_59025C);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_590298(uint32_t self) {
    st32(self, 0xf5a988u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x590298, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_590298);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_5902D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5a988u);
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
RW_FUNCTION(0x5902D8, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_5902D8);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_590334(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a988u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x590334, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_590334);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_590374(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x590374, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_590374);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_590394(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x590394, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_590394);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_590510(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3c080u ? self + 8 : 0; }
RW_FUNCTION(0x590510, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_590510);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_590534() { return 0xf8be34u; }
RW_FUNCTION(0x590534, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_590534);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func
static uint32_t t_590548(uint32_t self) {
    st32(self, 0xf5a9b4u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x590548, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func", t_590548);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_590584(uint32_t self) {
    st32(self, 0xf5a9b4u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x590584, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_590584);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_5905C4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5a9b4u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4683cd, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5905C4, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_5905C4);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_59061C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a9b4u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x59061C, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_59061C);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_590658(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x590658, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_590658);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_590678(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x590678, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_590678);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_5906F4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3c370u ? self + 8 : 0; }
RW_FUNCTION(0x5906F4, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_5906F4);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_590718() { return 0xf8be48u; }
RW_FUNCTION(0x590718, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_590718);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_59072C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x59072C, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_59072C);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_590734(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a9e0u);
    }
    return v1;
}
RW_FUNCTION(0x590734, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_590734);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_590758(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a9e0u);
    }
}
RW_FUNCTION(0x590758, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_590758);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_590774(uint32_t self) {
}
RW_FUNCTION(0x590774, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_590774);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_590776(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x590776, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_590776);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_590780(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3c650u ? self + 4 : 0; }
RW_FUNCTION(0x590780, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_590780);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_5907A0() { return 0xf8be5cu; }
RW_FUNCTION(0x5907A0, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_5907A0);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func
static uint32_t t_5908B0(uint32_t self) {
    st32(self, 0xf5aa0cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x5908B0, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func", t_5908B0);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_5908EC(uint32_t self) {
    st32(self, 0xf5aa0cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5908EC, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_5908EC);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone()
static uint32_t t_59092C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5aa0cu);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x59092C, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone()", t_59092C);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_59097C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5aa0cu);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x59097C, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_59097C);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_5909B0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x5909B0, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_5909B0);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_5909D0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5909D0, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_5909D0);

// BuyMagicStoneDialog::openDiscount $_1: __func::target
static uint32_t t_590A0C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3c8d0u ? self + 8 : 0; }
RW_FUNCTION(0x590A0C, "BuyMagicStoneDialog::openDiscount $_1 __func::target", t_590A0C);

// BuyMagicStoneDialog::openDiscount $_1: __func::target_type
static uint32_t t_590A2C() { return 0xf8be70u; }
RW_FUNCTION(0x590A2C, "BuyMagicStoneDialog::openDiscount $_1 __func::target_type", t_590A2C);

// BuyMagicStoneDialog::open $_0: __func::~__func
static uint32_t t_590A40(uint32_t self) {
    st32(self, 0xf5aa38u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x590A40, "BuyMagicStoneDialog::open $_0 __func::~__func", t_590A40);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_590A7C(uint32_t self) {
    st32(self, 0xf5aa38u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x590A7C, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_590A7C);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_590ABC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5aa38u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld8(self + 32);
        st8(v1 + 32, v2);
    }
    return v1;
}
RW_FUNCTION(0x590ABC, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_590ABC);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_590B10(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5aa38u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v1 = ld8(self + 32);
        st8(p + 32, v1);
    }
}
RW_FUNCTION(0x590B10, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_590B10);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_590B50(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x590B50, "BuyMagicStoneDialog::open $_0 __func::destroy", t_590B50);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_590B70(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x590B70, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_590B70);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_5918E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3cb00u ? self + 8 : 0; }
RW_FUNCTION(0x5918E8, "BuyMagicStoneDialog::open $_0 __func::target", t_5918E8);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_591908() { return 0xf8be84u; }
RW_FUNCTION(0x591908, "BuyMagicStoneDialog::open $_0 __func::target_type", t_591908);

// BuyMagicStoneDialog::open $_0: __func::~__func
static uint32_t t_59191C(uint32_t self) {
    st32(self, 0xf5aa64u);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x59191C, "BuyMagicStoneDialog::open $_0 __func::~__func", t_59191C);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_591974(uint32_t self) {
    st32(self, 0xf5aa64u);
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x591974, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_591974);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_591A68(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5aa64u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v1 = ld32(self + 32);
        uint32_t v2 = ld32(self + 36);
        st32(p + 32, v1);
        st32(p + 36, v2);
        call(0x4683cd, {p + 40, self + 40});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v3 = ld32(self + 64);
        st32(p + 64, v3);
    }
}
RW_FUNCTION(0x591A68, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_591A68);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_591AE8(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x591AE8, "BuyMagicStoneDialog::open $_0 __func::destroy", t_591AE8);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_591B28(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    functionDestroy(self + 40);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x591B28, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_591B28);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_591D0C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3cc70u ? self + 8 : 0; }
RW_FUNCTION(0x591D0C, "BuyMagicStoneDialog::open $_0 __func::target", t_591D0C);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_591D2C() { return 0xf8be98u; }
RW_FUNCTION(0x591D2C, "BuyMagicStoneDialog::open $_0 __func::target_type", t_591D2C);

// BuyMagicStoneDialog::open $_0: __func::~__func
static uint32_t t_591D40(uint32_t self) {
    st32(self, 0xf5aa90u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x591D40, "BuyMagicStoneDialog::open $_0 __func::~__func", t_591D40);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_591D7C(uint32_t self) {
    st32(self, 0xf5aa90u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x591D7C, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_591D7C);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_591DBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5aa90u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 32 + 0);
        uint32_t v3 = ld32(self + 32 + 4);
        uint32_t v4 = ld32(self + 32 + 8);
        st32(v1 + 32 + 0, v2);
        st32(v1 + 32 + 4, v3);
        st32(v1 + 32 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x591DBC, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_591DBC);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_591E18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5aa90u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v1 = ld32(self + 32 + 0);
        uint32_t v2 = ld32(self + 32 + 4);
        uint32_t v3 = ld32(self + 32 + 8);
        st32(p + 32 + 0, v1);
        st32(p + 32 + 4, v2);
        st32(p + 32 + 8, v3);
    }
}
RW_FUNCTION(0x591E18, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_591E18);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_591E5C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x591E5C, "BuyMagicStoneDialog::open $_0 __func::destroy", t_591E5C);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_591E7C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x591E7C, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_591E7C);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_591EE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3cec0u ? self + 8 : 0; }
RW_FUNCTION(0x591EE4, "BuyMagicStoneDialog::open $_0 __func::target", t_591EE4);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_591F04() { return 0xf8beacu; }
RW_FUNCTION(0x591F04, "BuyMagicStoneDialog::open $_0 __func::target_type", t_591F04);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_591F18(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x591F18, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_591F18);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_591F20(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5aabcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x591F20, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_591F20);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_591F4C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5aabcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x591F4C, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_591F4C);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_591F6C(uint32_t self) {
}
RW_FUNCTION(0x591F6C, "BuyMagicStoneDialog::open $_0 __func::destroy", t_591F6C);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_591F6E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x591F6E, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_591F6E);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_591F80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3d110u ? self + 4 : 0; }
RW_FUNCTION(0x591F80, "BuyMagicStoneDialog::open $_0 __func::target", t_591F80);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_591FA0() { return 0xf8bec0u; }
RW_FUNCTION(0x591FA0, "BuyMagicStoneDialog::open $_0 __func::target_type", t_591FA0);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_591FB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x591FB4, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_591FB4);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_591FBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5aae8u);
    }
    return v1;
}
RW_FUNCTION(0x591FBC, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_591FBC);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_591FE0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5aae8u);
    }
}
RW_FUNCTION(0x591FE0, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_591FE0);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_591FFC(uint32_t self) {
}
RW_FUNCTION(0x591FFC, "BuyMagicStoneDialog::open $_0 __func::destroy", t_591FFC);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_591FFE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x591FFE, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_591FFE);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_592010(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3d350u ? self + 4 : 0; }
RW_FUNCTION(0x592010, "BuyMagicStoneDialog::open $_0 __func::target", t_592010);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_592030() { return 0xf8bed4u; }
RW_FUNCTION(0x592030, "BuyMagicStoneDialog::open $_0 __func::target_type", t_592030);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_592044(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592044, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_592044);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_59204C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5ab14u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x59204C, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_59204C);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_59207C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ab14u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x59207C, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_59207C);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_5920A0(uint32_t self) {
}
RW_FUNCTION(0x5920A0, "BuyMagicStoneDialog::open $_0 __func::destroy", t_5920A0);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_5920A2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5920A2, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_5920A2);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_592254(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3d590u ? self + 4 : 0; }
RW_FUNCTION(0x592254, "BuyMagicStoneDialog::open $_0 __func::target", t_592254);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_592274() { return 0xf8bee8u; }
RW_FUNCTION(0x592274, "BuyMagicStoneDialog::open $_0 __func::target_type", t_592274);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_592288(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592288, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_592288);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_592290(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf5ab40u);
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
RW_FUNCTION(0x592290, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_592290);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_5922C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ab40u);
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
RW_FUNCTION(0x5922C4, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_5922C4);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_5922F0(uint32_t self) {
}
RW_FUNCTION(0x5922F0, "BuyMagicStoneDialog::open $_0 __func::destroy", t_5922F0);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_5922F2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5922F2, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_5922F2);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_5923D4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3d7e0u ? self + 4 : 0; }
RW_FUNCTION(0x5923D4, "BuyMagicStoneDialog::open $_0 __func::target", t_5923D4);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_5923F4() { return 0xf8befcu; }
RW_FUNCTION(0x5923F4, "BuyMagicStoneDialog::open $_0 __func::target_type", t_5923F4);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_592408(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592408, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_592408);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_592410(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ab6cu);
    }
    return v1;
}
RW_FUNCTION(0x592410, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_592410);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_592434(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ab6cu);
    }
}
RW_FUNCTION(0x592434, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_592434);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_592450(uint32_t self) {
}
RW_FUNCTION(0x592450, "BuyMagicStoneDialog::open $_0 __func::destroy", t_592450);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_592452(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592452, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_592452);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_59245C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3da40u ? self + 4 : 0; }
RW_FUNCTION(0x59245C, "BuyMagicStoneDialog::open $_0 __func::target", t_59245C);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_59247C() { return 0xf8bf10u; }
RW_FUNCTION(0x59247C, "BuyMagicStoneDialog::open $_0 __func::target_type", t_59247C);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_592490(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592490, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_592490);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_592498(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ab98u);
    }
    return v1;
}
RW_FUNCTION(0x592498, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_592498);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_5924BC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ab98u);
    }
}
RW_FUNCTION(0x5924BC, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_5924BC);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_5924D8(uint32_t self) {
}
RW_FUNCTION(0x5924D8, "BuyMagicStoneDialog::open $_0 __func::destroy", t_5924D8);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_5924DA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5924DA, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_5924DA);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_5924F8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3dcc0u ? self + 4 : 0; }
RW_FUNCTION(0x5924F8, "BuyMagicStoneDialog::open $_0 __func::target", t_5924F8);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_592518() { return 0xf8bf24u; }
RW_FUNCTION(0x592518, "BuyMagicStoneDialog::open $_0 __func::target_type", t_592518);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_59252C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x59252C, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_59252C);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_592534(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5abc4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x592534, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_592534);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_592560(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5abc4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x592560, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_592560);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_592580(uint32_t self) {
}
RW_FUNCTION(0x592580, "BuyMagicStoneDialog::open $_0 __func::destroy", t_592580);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_592582(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592582, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_592582);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_59259C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3df30u ? self + 4 : 0; }
RW_FUNCTION(0x59259C, "BuyMagicStoneDialog::open $_0 __func::target", t_59259C);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_5925BC() { return 0xf8bf38u; }
RW_FUNCTION(0x5925BC, "BuyMagicStoneDialog::open $_0 __func::target_type", t_5925BC);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_5925D0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5925D0, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_5925D0);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_5925D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5abf0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5925D8, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_5925D8);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_592604(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5abf0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x592604, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_592604);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_592624(uint32_t self) {
}
RW_FUNCTION(0x592624, "BuyMagicStoneDialog::open $_0 __func::destroy", t_592624);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_592626(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592626, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_592626);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_592640(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3e170u ? self + 4 : 0; }
RW_FUNCTION(0x592640, "BuyMagicStoneDialog::open $_0 __func::target", t_592640);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_592660() { return 0xf8bf4cu; }
RW_FUNCTION(0x592660, "BuyMagicStoneDialog::open $_0 __func::target_type", t_592660);

// BuyMagicStoneDialog::open $_0: __func::~__func
static uint32_t t_592674(uint32_t self) {
    st32(self, 0xf5ac1cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x592674, "BuyMagicStoneDialog::open $_0 __func::~__func", t_592674);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_5926B0(uint32_t self) {
    st32(self, 0xf5ac1cu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5926B0, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_5926B0);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_5926F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5ac1cu);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x5926F0, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_5926F0);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_592740(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ac1cu);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x592740, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_592740);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_592774(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x592774, "BuyMagicStoneDialog::open $_0 __func::destroy", t_592774);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_592794(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592794, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_592794);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_5927D0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3e3b0u ? self + 8 : 0; }
RW_FUNCTION(0x5927D0, "BuyMagicStoneDialog::open $_0 __func::target", t_5927D0);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_5927F0() { return 0xf8bf60u; }
RW_FUNCTION(0x5927F0, "BuyMagicStoneDialog::open $_0 __func::target_type", t_5927F0);

// BuyMagicStoneDialog::open $_0: __func::~__func
static uint32_t t_592804(uint32_t self) {
    st32(self, 0xf5ac48u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x592804, "BuyMagicStoneDialog::open $_0 __func::~__func", t_592804);

// BuyMagicStoneDialog::open $_0: __func::~__func (deleting)
static void t_592840(uint32_t self) {
    st32(self, 0xf5ac48u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592840, "BuyMagicStoneDialog::open $_0 __func::~__func (deleting)", t_592840);

// BuyMagicStoneDialog::open $_0: __func::__clone()
static uint32_t t_592880(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5ac48u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x592880, "BuyMagicStoneDialog::open $_0 __func::__clone()", t_592880);

// BuyMagicStoneDialog::open $_0: __func::__clone(__base*)
static void t_5928D0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ac48u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x5928D0, "BuyMagicStoneDialog::open $_0 __func::__clone(__base*)", t_5928D0);

// BuyMagicStoneDialog::open $_0: __func::destroy
static void t_592904(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x592904, "BuyMagicStoneDialog::open $_0 __func::destroy", t_592904);

// BuyMagicStoneDialog::open $_0: __func::destroy_deallocate
static void t_592924(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x592924, "BuyMagicStoneDialog::open $_0 __func::destroy_deallocate", t_592924);

// BuyMagicStoneDialog::open $_0: __func::target
static uint32_t t_592960(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3e5f0u ? self + 8 : 0; }
RW_FUNCTION(0x592960, "BuyMagicStoneDialog::open $_0 __func::target", t_592960);

// BuyMagicStoneDialog::open $_0: __func::target_type
static uint32_t t_592980() { return 0xf8bf74u; }
RW_FUNCTION(0x592980, "BuyMagicStoneDialog::open $_0 __func::target_type", t_592980);
