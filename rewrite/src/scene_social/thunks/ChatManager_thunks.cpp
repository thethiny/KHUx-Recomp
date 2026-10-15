// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ChatManager lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ChatManager::report $_21: __func::~__func
static uint32_t t_4DCEE0(uint32_t self) {
    st32(self, 0xf55708u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x4DCEE0, "ChatManager::report $_21 __func::~__func", t_4DCEE0);

// ChatManager::report $_21: __func::~__func (deleting)
static void t_4DCF1C(uint32_t self) {
    st32(self, 0xf55708u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DCF1C, "ChatManager::report $_21 __func::~__func (deleting)", t_4DCF1C);

// ChatManager::report $_21: __func::destroy
static void t_4DCFF0(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x4DCFF0, "ChatManager::report $_21 __func::destroy", t_4DCFF0);

// ChatManager::report $_21: __func::destroy_deallocate
static void t_4DD010(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD010, "ChatManager::report $_21 __func::destroy_deallocate", t_4DD010);

// ChatManager::report $_21: __func::target
static uint32_t t_4DD08C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1dd00u ? self + 8 : 0; }
RW_FUNCTION(0x4DD08C, "ChatManager::report $_21 __func::target", t_4DD08C);

// ChatManager::report $_21: __func::target_type
static uint32_t t_4DD0B0() { return 0xf7f9ecu; }
RW_FUNCTION(0x4DD0B0, "ChatManager::report $_21 __func::target_type", t_4DD0B0);

// ChatManager::unblock $_20: __func::~__func
static uint32_t t_4DD0C4(uint32_t self) {
    st32(self, 0xf55734u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x4DD0C4, "ChatManager::unblock $_20 __func::~__func", t_4DD0C4);

// ChatManager::unblock $_20: __func::~__func (deleting)
static void t_4DD100(uint32_t self) {
    st32(self, 0xf55734u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD100, "ChatManager::unblock $_20 __func::~__func (deleting)", t_4DD100);

// ChatManager::unblock $_20: __func::destroy
static void t_4DD1D4(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x4DD1D4, "ChatManager::unblock $_20 __func::destroy", t_4DD1D4);

// ChatManager::unblock $_20: __func::destroy_deallocate
static void t_4DD1F4(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD1F4, "ChatManager::unblock $_20 __func::destroy_deallocate", t_4DD1F4);

// ChatManager::unblock $_20: __func::target
static uint32_t t_4DD270(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1de00u ? self + 8 : 0; }
RW_FUNCTION(0x4DD270, "ChatManager::unblock $_20 __func::target", t_4DD270);

// ChatManager::unblock $_20: __func::target_type
static uint32_t t_4DD294() { return 0xf7fa00u; }
RW_FUNCTION(0x4DD294, "ChatManager::unblock $_20 __func::target_type", t_4DD294);

// ChatManager::block $_19: __func::~__func
static uint32_t t_4DD2A8(uint32_t self) {
    st32(self, 0xf55760u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x4DD2A8, "ChatManager::block $_19 __func::~__func", t_4DD2A8);

// ChatManager::block $_19: __func::~__func (deleting)
static void t_4DD2E4(uint32_t self) {
    st32(self, 0xf55760u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD2E4, "ChatManager::block $_19 __func::~__func (deleting)", t_4DD2E4);

// ChatManager::block $_19: __func::destroy
static void t_4DD3B8(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x4DD3B8, "ChatManager::block $_19 __func::destroy", t_4DD3B8);

// ChatManager::block $_19: __func::destroy_deallocate
static void t_4DD3D8(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD3D8, "ChatManager::block $_19 __func::destroy_deallocate", t_4DD3D8);

// ChatManager::block $_19: __func::target
static uint32_t t_4DD454(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1df00u ? self + 8 : 0; }
RW_FUNCTION(0x4DD454, "ChatManager::block $_19 __func::target", t_4DD454);

// ChatManager::block $_19: __func::target_type
static uint32_t t_4DD478() { return 0xf7fa14u; }
RW_FUNCTION(0x4DD478, "ChatManager::block $_19 __func::target_type", t_4DD478);

// ChatManager::deleteMessage $_18: __func::~__func
static uint32_t t_4DD48C(uint32_t self) {
    st32(self, 0xf5578cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x4DD48C, "ChatManager::deleteMessage $_18 __func::~__func", t_4DD48C);

// ChatManager::deleteMessage $_18: __func::~__func (deleting)
static void t_4DD4C8(uint32_t self) {
    st32(self, 0xf5578cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD4C8, "ChatManager::deleteMessage $_18 __func::~__func (deleting)", t_4DD4C8);

// ChatManager::deleteMessage $_18: __func::destroy
static void t_4DD59C(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x4DD59C, "ChatManager::deleteMessage $_18 __func::destroy", t_4DD59C);

// ChatManager::deleteMessage $_18: __func::destroy_deallocate
static void t_4DD5BC(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD5BC, "ChatManager::deleteMessage $_18 __func::destroy_deallocate", t_4DD5BC);

// ChatManager::deleteMessage $_18: __func::target
static uint32_t t_4DD638(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1e010u ? self + 8 : 0; }
RW_FUNCTION(0x4DD638, "ChatManager::deleteMessage $_18 __func::target", t_4DD638);

// ChatManager::deleteMessage $_18: __func::target_type
static uint32_t t_4DD65C() { return 0xf7fa28u; }
RW_FUNCTION(0x4DD65C, "ChatManager::deleteMessage $_18 __func::target_type", t_4DD65C);

// ChatManager::createMessage $_17: __func::~__func
static uint32_t t_4DD670(uint32_t self) {
    st32(self, 0xf557b8u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x4DD670, "ChatManager::createMessage $_17 __func::~__func", t_4DD670);

// ChatManager::createMessage $_17: __func::~__func (deleting)
static void t_4DD6AC(uint32_t self) {
    st32(self, 0xf557b8u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD6AC, "ChatManager::createMessage $_17 __func::~__func (deleting)", t_4DD6AC);

// ChatManager::createMessage $_17: __func::destroy
static void t_4DD780(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x4DD780, "ChatManager::createMessage $_17 __func::destroy", t_4DD780);

// ChatManager::createMessage $_17: __func::destroy_deallocate
static void t_4DD7A0(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD7A0, "ChatManager::createMessage $_17 __func::destroy_deallocate", t_4DD7A0);

// ChatManager::createMessage $_17: __func::target
static uint32_t t_4DD81C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1e120u ? self + 8 : 0; }
RW_FUNCTION(0x4DD81C, "ChatManager::createMessage $_17 __func::target", t_4DD81C);

// ChatManager::createMessage $_17: __func::target_type
static uint32_t t_4DD840() { return 0xf7fa3cu; }
RW_FUNCTION(0x4DD840, "ChatManager::createMessage $_17 __func::target_type", t_4DD840);

// ChatManager::update $_16: __func::~__func
static uint32_t t_4DD854(uint32_t self) {
    st32(self, 0xf557e4u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x4DD854, "ChatManager::update $_16 __func::~__func", t_4DD854);

// ChatManager::update $_16: __func::~__func (deleting)
static void t_4DD890(uint32_t self) {
    st32(self, 0xf557e4u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD890, "ChatManager::update $_16 __func::~__func (deleting)", t_4DD890);

// ChatManager::update $_16: __func::destroy
static void t_4DD964(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x4DD964, "ChatManager::update $_16 __func::destroy", t_4DD964);

// ChatManager::update $_16: __func::destroy_deallocate
static void t_4DD984(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DD984, "ChatManager::update $_16 __func::destroy_deallocate", t_4DD984);

// ChatManager::update $_16: __func::target
static uint32_t t_4DDA00(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1e230u ? self + 8 : 0; }
RW_FUNCTION(0x4DDA00, "ChatManager::update $_16 __func::target", t_4DDA00);

// ChatManager::update $_16: __func::target_type
static uint32_t t_4DDA24() { return 0xf7fa50u; }
RW_FUNCTION(0x4DDA24, "ChatManager::update $_16 __func::target_type", t_4DDA24);

// ChatManager::getMessage : __func::~__func
static uint32_t t_4DDA38(uint32_t self) {
    st32(self, 0xf55810u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x4DDA38, "ChatManager::getMessage  __func::~__func", t_4DDA38);

// ChatManager::getMessage : __func::~__func (deleting)
static void t_4DDA74(uint32_t self) {
    st32(self, 0xf55810u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DDA74, "ChatManager::getMessage  __func::~__func (deleting)", t_4DDA74);

// ChatManager::getMessage : __func::destroy
static void t_4DDB48(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x4DDB48, "ChatManager::getMessage  __func::destroy", t_4DDB48);

// ChatManager::getMessage : __func::destroy_deallocate
static void t_4DDB68(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DDB68, "ChatManager::getMessage  __func::destroy_deallocate", t_4DDB68);

// ChatManager::getMessage : __func::target
static uint32_t t_4DDECC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1e380u ? self + 8 : 0; }
RW_FUNCTION(0x4DDECC, "ChatManager::getMessage  __func::target", t_4DDECC);

// ChatManager::getMessage : __func::target_type
static uint32_t t_4DDEF0() { return 0xf7fa64u; }
RW_FUNCTION(0x4DDEF0, "ChatManager::getMessage  __func::target_type", t_4DDEF0);

// ChatManager::getUser : __func::~__func
static uint32_t t_4DE368(uint32_t self) {
    st32(self, 0xf5583cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x4DE368, "ChatManager::getUser  __func::~__func", t_4DE368);

// ChatManager::getUser : __func::~__func (deleting)
static void t_4DE3A4(uint32_t self) {
    st32(self, 0xf5583cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DE3A4, "ChatManager::getUser  __func::~__func (deleting)", t_4DE3A4);

// ChatManager::getUser : __func::destroy
static void t_4DE478(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x4DE478, "ChatManager::getUser  __func::destroy", t_4DE478);

// ChatManager::getUser : __func::destroy_deallocate
static void t_4DE498(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DE498, "ChatManager::getUser  __func::destroy_deallocate", t_4DE498);

// ChatManager::getUser : __func::target
static uint32_t t_4DE844(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1e500u ? self + 8 : 0; }
RW_FUNCTION(0x4DE844, "ChatManager::getUser  __func::target", t_4DE844);

// ChatManager::getUser : __func::target_type
static uint32_t t_4DE868() { return 0xf7fa78u; }
RW_FUNCTION(0x4DE868, "ChatManager::getUser  __func::target_type", t_4DE868);

// ---- further thunks attributed to ChatManager (wave 3)

// PurchaseManager::finishPurchase $_0: __func::~__func
static uint32_t t_4DA28C(uint32_t self) {
    st32(self, 0xf556dcu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x4DA28C, "PurchaseManager::finishPurchase $_0 __func::~__func", t_4DA28C);

// PurchaseManager::finishPurchase $_0: __func::~__func (deleting)
static void t_4DA2D4(uint32_t self) {
    st32(self, 0xf556dcu);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DA2D4, "PurchaseManager::finishPurchase $_0 __func::~__func (deleting)", t_4DA2D4);

// PurchaseManager::finishPurchase $_0: __func::__clone()
static uint32_t t_4DA320(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf556dcu);
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
        uint32_t v6 = ld8(self + 24);
        st8(v1 + 24, v6);
        call(0x45c84d, {v1 + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v7 = ld8(self + 24);
        st8(v1 + 24, v7);
    }
    return v1;
}
RW_FUNCTION(0x4DA320, "PurchaseManager::finishPurchase $_0 __func::__clone()", t_4DA320);

// PurchaseManager::finishPurchase $_0: __func::__clone(__base*)
static void t_4DA3A0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf556dcu);
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
        uint32_t v5 = ld8(self + 24);
        st8(p + 24, v5);
        call(0x45c84d, {p + 32, self + 32});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v6 = ld8(self + 24);
        st8(p + 24, v6);
    }
}
RW_FUNCTION(0x4DA3A0, "PurchaseManager::finishPurchase $_0 __func::__clone(__base*)", t_4DA3A0);

// PurchaseManager::finishPurchase $_0: __func::destroy
static void t_4DA40C(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x4DA40C, "PurchaseManager::finishPurchase $_0 __func::destroy", t_4DA40C);

// PurchaseManager::finishPurchase $_0: __func::destroy_deallocate
static void t_4DA43A(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4DA43A, "PurchaseManager::finishPurchase $_0 __func::destroy_deallocate", t_4DA43A);

// PurchaseManager::finishPurchase $_0: __func::target
static uint32_t t_4DA4CC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1d980u ? self + 8 : 0; }
RW_FUNCTION(0x4DA4CC, "PurchaseManager::finishPurchase $_0 __func::target", t_4DA4CC);

// PurchaseManager::finishPurchase $_0: __func::target_type
static uint32_t t_4DA4F0() { return 0xf7f95cu; }
RW_FUNCTION(0x4DA4F0, "PurchaseManager::finishPurchase $_0 __func::target_type", t_4DA4F0);

// ChatManager::report $_21: __func::__clone()
static uint32_t t_4DCF5C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf55708u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4dbfed, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
    return v1;
}
RW_FUNCTION(0x4DCF5C, "ChatManager::report $_21 __func::__clone()", t_4DCF5C);

// ChatManager::report $_21: __func::__clone(__base*)
static void t_4DCFB4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55708u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4dbfed, {p + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
}
RW_FUNCTION(0x4DCFB4, "ChatManager::report $_21 __func::__clone(__base*)", t_4DCFB4);

// ChatManager::unblock $_20: __func::__clone()
static uint32_t t_4DD140(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf55734u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4dbfed, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
    return v1;
}
RW_FUNCTION(0x4DD140, "ChatManager::unblock $_20 __func::__clone()", t_4DD140);

// ChatManager::unblock $_20: __func::__clone(__base*)
static void t_4DD198(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55734u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4dbfed, {p + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
}
RW_FUNCTION(0x4DD198, "ChatManager::unblock $_20 __func::__clone(__base*)", t_4DD198);

// ChatManager::block $_19: __func::__clone()
static uint32_t t_4DD324(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf55760u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4dbfed, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
    return v1;
}
RW_FUNCTION(0x4DD324, "ChatManager::block $_19 __func::__clone()", t_4DD324);

// ChatManager::block $_19: __func::__clone(__base*)
static void t_4DD37C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55760u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4dbfed, {p + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
}
RW_FUNCTION(0x4DD37C, "ChatManager::block $_19 __func::__clone(__base*)", t_4DD37C);

// ChatManager::deleteMessage $_18: __func::__clone()
static uint32_t t_4DD508(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5578cu);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4dbfed, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
    return v1;
}
RW_FUNCTION(0x4DD508, "ChatManager::deleteMessage $_18 __func::__clone()", t_4DD508);

// ChatManager::deleteMessage $_18: __func::__clone(__base*)
static void t_4DD560(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5578cu);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4dbfed, {p + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
}
RW_FUNCTION(0x4DD560, "ChatManager::deleteMessage $_18 __func::__clone(__base*)", t_4DD560);

// ChatManager::createMessage $_17: __func::__clone()
static uint32_t t_4DD6EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf557b8u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4dbfed, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
    return v1;
}
RW_FUNCTION(0x4DD6EC, "ChatManager::createMessage $_17 __func::__clone()", t_4DD6EC);

// ChatManager::createMessage $_17: __func::__clone(__base*)
static void t_4DD744(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf557b8u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4dbfed, {p + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
}
RW_FUNCTION(0x4DD744, "ChatManager::createMessage $_17 __func::__clone(__base*)", t_4DD744);

// ChatManager::update $_16: __func::__clone()
static uint32_t t_4DD8D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf557e4u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4dbfed, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
    return v1;
}
RW_FUNCTION(0x4DD8D0, "ChatManager::update $_16 __func::__clone()", t_4DD8D0);

// ChatManager::update $_16: __func::__clone(__base*)
static void t_4DD928(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf557e4u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4dbfed, {p + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEEEC2ERKS4_
    }
}
RW_FUNCTION(0x4DD928, "ChatManager::update $_16 __func::__clone(__base*)", t_4DD928);

// ChatManager::getMessage : __func::__clone()
static uint32_t t_4DDAB4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf55810u);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4dbc57, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeERKNS_6vectorINS1_7MessageENS_9allocatorIS4_EEEEjjjEEC2ERKSB_
    }
    return v1;
}
RW_FUNCTION(0x4DDAB4, "ChatManager::getMessage  __func::__clone()", t_4DDAB4);

// ChatManager::getMessage : __func::__clone(__base*)
static void t_4DDB0C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf55810u);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4dbc57, {p + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeERKNS_6vectorINS1_7MessageENS_9allocatorIS4_EEEEjjjEEC2ERKSB_
    }
}
RW_FUNCTION(0x4DDB0C, "ChatManager::getMessage  __func::__clone(__base*)", t_4DDB0C);

// ChatManager::getUser : __func::__clone()
static uint32_t t_4DE3E4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5583cu);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(v1 + 8, v2);
        st32(v1 + 12, v3);
        call(0x4db7ad, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEjjRKNS_6vectorINS1_5BlockENS_9allocatorIS4_EEEEEEC2ERKSB_
    }
    return v1;
}
RW_FUNCTION(0x4DE3E4, "ChatManager::getUser  __func::__clone()", t_4DE3E4);

// ChatManager::getUser : __func::__clone(__base*)
static void t_4DE43C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5583cu);
        uint32_t v1 = ld32(self + 8);
        uint32_t v2 = ld32(self + 12);
        st32(p + 8, v1);
        st32(p + 12, v2);
        call(0x4db7ad, {p + 16, self + 16});   // _ZNSt3__18functionIFvN11ChatManager10StatusCodeEjjRKNS_6vectorINS1_5BlockENS_9allocatorIS4_EEEEEEC2ERKSB_
    }
}
RW_FUNCTION(0x4DE43C, "ChatManager::getUser  __func::__clone(__base*)", t_4DE43C);
