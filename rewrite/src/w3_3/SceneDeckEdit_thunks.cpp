// The std::function __func thunks of the lambdas attributed to SceneDeckEdit (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// StartDeckEditDialog::supportScrollRunOutAnimation $_21: __func::__clone()
static uint32_t t_57ABA8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf598dcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        call(0x8e4c03, {v1 + 8, self + 8});   // _ZN7cocos2d4Vec2C2ERKS0_
        uint32_t v3 = ld32(self + 16);
        st32(v1 + 16, v3);
    }
    return v1;
}
RW_FUNCTION(0x57ABA8, "StartDeckEditDialog::supportScrollRunOutAnimation $_21 __func::__clone()", t_57ABA8);

// StartDeckEditDialog::supportScrollRunOutAnimation $_21: __func::__clone(__base*)
static void t_57AC00(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf598dcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        call(0x8e4c03, {p + 8, self + 8});   // _ZN7cocos2d4Vec2C2ERKS0_
        uint32_t v2 = ld32(self + 16);
        st32(p + 16, v2);
    }
}
RW_FUNCTION(0x57AC00, "StartDeckEditDialog::supportScrollRunOutAnimation $_21 __func::__clone(__base*)", t_57AC00);

// StartDeckEditDialog::supportScrollRunOutAnimation $_21: __func::destroy
static void t_57AC3C(uint32_t self) {
    call(0x8e4c17, {self + 8});   // _ZN7cocos2d4Vec2D2Ev
}
RW_FUNCTION(0x57AC3C, "StartDeckEditDialog::supportScrollRunOutAnimation $_21 __func::destroy", t_57AC3C);

// StartDeckEditDialog::supportScrollRunOutAnimation $_21: __func::destroy_deallocate
static void t_57AC46(uint32_t self) {
    call(0x8e4c17, {self + 8});   // _ZN7cocos2d4Vec2D2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57AC46, "StartDeckEditDialog::supportScrollRunOutAnimation $_21 __func::destroy_deallocate", t_57AC46);

// StartDeckEditDialog::supportScrollRunOutAnimation $_21: __func::target
static uint32_t t_57AC98(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35950u ? self + 4 : 0; }
RW_FUNCTION(0x57AC98, "StartDeckEditDialog::supportScrollRunOutAnimation $_21 __func::target", t_57AC98);

// StartDeckEditDialog::supportScrollRunOutAnimation $_21: __func::target_type
static uint32_t t_57ACB8() { return 0xf8b058u; }
RW_FUNCTION(0x57ACB8, "StartDeckEditDialog::supportScrollRunOutAnimation $_21 __func::target_type", t_57ACB8);

// StartDeckEditDialog::supportScrollRunInAnimation $_20: __func::~__func (deleting)
static void t_57ACCC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57ACCC, "StartDeckEditDialog::supportScrollRunInAnimation $_20 __func::~__func (deleting)", t_57ACCC);

// StartDeckEditDialog::supportScrollRunInAnimation $_20: __func::__clone()
static uint32_t t_57ACD4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59908u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x57ACD4, "StartDeckEditDialog::supportScrollRunInAnimation $_20 __func::__clone()", t_57ACD4);

// StartDeckEditDialog::supportScrollRunInAnimation $_20: __func::__clone(__base*)
static void t_57AD00(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59908u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x57AD00, "StartDeckEditDialog::supportScrollRunInAnimation $_20 __func::__clone(__base*)", t_57AD00);

// StartDeckEditDialog::supportScrollRunInAnimation $_20: __func::destroy
static void t_57AD20(uint32_t self) {
}
RW_FUNCTION(0x57AD20, "StartDeckEditDialog::supportScrollRunInAnimation $_20 __func::destroy", t_57AD20);

// StartDeckEditDialog::supportScrollRunInAnimation $_20: __func::destroy_deallocate
static void t_57AD22(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57AD22, "StartDeckEditDialog::supportScrollRunInAnimation $_20 __func::destroy_deallocate", t_57AD22);

// StartDeckEditDialog::supportScrollRunInAnimation $_20: __func::target
static uint32_t t_57AD44(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35a10u ? self + 4 : 0; }
RW_FUNCTION(0x57AD44, "StartDeckEditDialog::supportScrollRunInAnimation $_20 __func::target", t_57AD44);

// StartDeckEditDialog::supportScrollRunInAnimation $_20: __func::target_type
static uint32_t t_57AD64() { return 0xf8b06cu; }
RW_FUNCTION(0x57AD64, "StartDeckEditDialog::supportScrollRunInAnimation $_20 __func::target_type", t_57AD64);

// StartDeckEditDialog::supportScrollRunInAnimation $_19: __func::~__func (deleting)
static void t_57AD78(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57AD78, "StartDeckEditDialog::supportScrollRunInAnimation $_19 __func::~__func (deleting)", t_57AD78);

// StartDeckEditDialog::supportScrollRunInAnimation $_19: __func::__clone()
static uint32_t t_57AD80(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59934u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x57AD80, "StartDeckEditDialog::supportScrollRunInAnimation $_19 __func::__clone()", t_57AD80);

// StartDeckEditDialog::supportScrollRunInAnimation $_19: __func::__clone(__base*)
static void t_57ADAC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59934u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x57ADAC, "StartDeckEditDialog::supportScrollRunInAnimation $_19 __func::__clone(__base*)", t_57ADAC);

// StartDeckEditDialog::supportScrollRunInAnimation $_19: __func::destroy
static void t_57ADCC(uint32_t self) {
}
RW_FUNCTION(0x57ADCC, "StartDeckEditDialog::supportScrollRunInAnimation $_19 __func::destroy", t_57ADCC);

// StartDeckEditDialog::supportScrollRunInAnimation $_19: __func::destroy_deallocate
static void t_57ADCE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57ADCE, "StartDeckEditDialog::supportScrollRunInAnimation $_19 __func::destroy_deallocate", t_57ADCE);

// StartDeckEditDialog::supportScrollRunInAnimation $_19: __func::target
static uint32_t t_57ADE0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35ad0u ? self + 4 : 0; }
RW_FUNCTION(0x57ADE0, "StartDeckEditDialog::supportScrollRunInAnimation $_19 __func::target", t_57ADE0);

// StartDeckEditDialog::supportScrollRunInAnimation $_19: __func::target_type
static uint32_t t_57AE00() { return 0xf8b080u; }
RW_FUNCTION(0x57AE00, "StartDeckEditDialog::supportScrollRunInAnimation $_19 __func::target_type", t_57AE00);

// StartDeckEditDialog::close $_18: __func::~__func (deleting)
static void t_57AE14(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57AE14, "StartDeckEditDialog::close $_18 __func::~__func (deleting)", t_57AE14);

// StartDeckEditDialog::close $_18: __func::__clone()
static uint32_t t_57AE1C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59960u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x57AE1C, "StartDeckEditDialog::close $_18 __func::__clone()", t_57AE1C);

// StartDeckEditDialog::close $_18: __func::__clone(__base*)
static void t_57AE48(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59960u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x57AE48, "StartDeckEditDialog::close $_18 __func::__clone(__base*)", t_57AE48);

// StartDeckEditDialog::close $_18: __func::destroy
static void t_57AE68(uint32_t self) {
}
RW_FUNCTION(0x57AE68, "StartDeckEditDialog::close $_18 __func::destroy", t_57AE68);

// StartDeckEditDialog::close $_18: __func::destroy_deallocate
static void t_57AE6A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57AE6A, "StartDeckEditDialog::close $_18 __func::destroy_deallocate", t_57AE6A);

// StartDeckEditDialog::close $_18: __func::target
static uint32_t t_57AF98(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35b70u ? self + 4 : 0; }
RW_FUNCTION(0x57AF98, "StartDeckEditDialog::close $_18 __func::target", t_57AF98);

// StartDeckEditDialog::close $_18: __func::target_type
static uint32_t t_57AFB8() { return 0xf8b094u; }
RW_FUNCTION(0x57AFB8, "StartDeckEditDialog::close $_18 __func::target_type", t_57AFB8);

// SceneSaleMedal::openSellPopup $_13: __func::target
static uint32_t t_65F2AC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe642d0u ? self + 4 : 0; }
RW_FUNCTION(0x65F2AC, "SceneSaleMedal::openSellPopup $_13 __func::target", t_65F2AC);

// SceneSaleMedal::openSellPopup $_13: __func::target_type
static uint32_t t_65F2CC() { return 0xf9a708u; }
RW_FUNCTION(0x65F2CC, "SceneSaleMedal::openSellPopup $_13 __func::target_type", t_65F2CC);

// SceneSaleMedal::openSellPopup $_13 UlbE_: __func::~__func (deleting)
static void t_65F2E0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F2E0, "SceneSaleMedal::openSellPopup $_13 UlbE_ __func::~__func (deleting)", t_65F2E0);

// SceneSaleMedal::openSellPopup $_13 UlbE_: __func::__clone()
static uint32_t t_65F2E8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6299cu);
    }
    return v1;
}
RW_FUNCTION(0x65F2E8, "SceneSaleMedal::openSellPopup $_13 UlbE_ __func::__clone()", t_65F2E8);

// SceneSaleMedal::openSellPopup $_13 UlbE_: __func::__clone(__base*)
static void t_65F30C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6299cu);
    }
}
RW_FUNCTION(0x65F30C, "SceneSaleMedal::openSellPopup $_13 UlbE_ __func::__clone(__base*)", t_65F30C);

// SceneSaleMedal::openSellPopup $_13 UlbE_: __func::destroy
static void t_65F328(uint32_t self) {
}
RW_FUNCTION(0x65F328, "SceneSaleMedal::openSellPopup $_13 UlbE_ __func::destroy", t_65F328);

// SceneSaleMedal::openSellPopup $_13 UlbE_: __func::destroy_deallocate
static void t_65F32A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F32A, "SceneSaleMedal::openSellPopup $_13 UlbE_ __func::destroy_deallocate", t_65F32A);

// SceneSaleMedal::openSellPopup $_13 UlbE_: __func::target
static uint32_t t_65F340(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64370u ? self + 4 : 0; }
RW_FUNCTION(0x65F340, "SceneSaleMedal::openSellPopup $_13 UlbE_ __func::target", t_65F340);

// SceneSaleMedal::openSellPopup $_13 UlbE_: __func::target_type
static uint32_t t_65F360() { return 0xf9a71cu; }
RW_FUNCTION(0x65F360, "SceneSaleMedal::openSellPopup $_13 UlbE_ __func::target_type", t_65F360);

// SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_: __func::~__func (deleting)
static void t_65F374(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F374, "SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_ __func::~__func (deleting)", t_65F374);

// SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_: __func::__clone()
static uint32_t t_65F37C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf629c8u);
    }
    return v1;
}
RW_FUNCTION(0x65F37C, "SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_ __func::__clone()", t_65F37C);

// SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_: __func::__clone(__base*)
static void t_65F3A0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf629c8u);
    }
}
RW_FUNCTION(0x65F3A0, "SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_ __func::__clone(__base*)", t_65F3A0);

// SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_: __func::destroy
static void t_65F3BC(uint32_t self) {
}
RW_FUNCTION(0x65F3BC, "SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_ __func::destroy", t_65F3BC);

// SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_: __func::destroy_deallocate
static void t_65F3BE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F3BE, "SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_ __func::destroy_deallocate", t_65F3BE);

// SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_: __func::target
static uint32_t t_65F3D4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64460u ? self + 4 : 0; }
RW_FUNCTION(0x65F3D4, "SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_ __func::target", t_65F3D4);

// SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_: __func::target_type
static uint32_t t_65F3F4() { return 0xf9a730u; }
RW_FUNCTION(0x65F3F4, "SceneSaleMedal::openSellPopup $_8 UlbE_ UlbE_ __func::target_type", t_65F3F4);

// SceneSaleMedal::runOutAnimationMain $_7: __func::~__func
static uint32_t t_65F408(uint32_t self) {
    st32(self, 0xf629f4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x65F408, "SceneSaleMedal::runOutAnimationMain $_7 __func::~__func", t_65F408);

// SceneSaleMedal::runOutAnimationMain $_7: __func::~__func (deleting)
static void t_65F444(uint32_t self) {
    st32(self, 0xf629f4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F444, "SceneSaleMedal::runOutAnimationMain $_7 __func::~__func (deleting)", t_65F444);

// SceneSaleMedal::runOutAnimationMain $_7: __func::__clone()
static uint32_t t_65F484(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf629f4u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x65F484, "SceneSaleMedal::runOutAnimationMain $_7 __func::__clone()", t_65F484);

// SceneSaleMedal::runOutAnimationMain $_7: __func::__clone(__base*)
static void t_65F4D4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf629f4u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x65F4D4, "SceneSaleMedal::runOutAnimationMain $_7 __func::__clone(__base*)", t_65F4D4);

// SceneSaleMedal::runOutAnimationMain $_7: __func::destroy
static void t_65F508(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x65F508, "SceneSaleMedal::runOutAnimationMain $_7 __func::destroy", t_65F508);

// SceneSaleMedal::runOutAnimationMain $_7: __func::destroy_deallocate
static void t_65F528(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F528, "SceneSaleMedal::runOutAnimationMain $_7 __func::destroy_deallocate", t_65F528);

// SceneSaleMedal::runOutAnimationMain $_7: __func::target
static uint32_t t_65F560(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64550u ? self + 8 : 0; }
RW_FUNCTION(0x65F560, "SceneSaleMedal::runOutAnimationMain $_7 __func::target", t_65F560);

// SceneSaleMedal::runOutAnimationMain $_7: __func::target_type
static uint32_t t_65F580() { return 0xf9a744u; }
RW_FUNCTION(0x65F580, "SceneSaleMedal::runOutAnimationMain $_7 __func::target_type", t_65F580);

// SceneSaleMedal::init $_6: __func::~__func (deleting)
static void t_65F690(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F690, "SceneSaleMedal::init $_6 __func::~__func (deleting)", t_65F690);

// SceneSaleMedal::init $_6: __func::__clone()
static uint32_t t_65F698(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62a20u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x65F698, "SceneSaleMedal::init $_6 __func::__clone()", t_65F698);

// SceneSaleMedal::init $_6: __func::__clone(__base*)
static void t_65F6C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62a20u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65F6C4, "SceneSaleMedal::init $_6 __func::__clone(__base*)", t_65F6C4);

// SceneSaleMedal::init $_6: __func::destroy
static void t_65F6E4(uint32_t self) {
}
RW_FUNCTION(0x65F6E4, "SceneSaleMedal::init $_6 __func::destroy", t_65F6E4);

// SceneSaleMedal::init $_6: __func::destroy_deallocate
static void t_65F6E6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F6E6, "SceneSaleMedal::init $_6 __func::destroy_deallocate", t_65F6E6);

// SceneSaleMedal::init $_6: __func::target
static uint32_t t_65F8D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64600u ? self + 4 : 0; }
RW_FUNCTION(0x65F8D8, "SceneSaleMedal::init $_6 __func::target", t_65F8D8);

// SceneSaleMedal::init $_6: __func::target_type
static uint32_t t_65F8F8() { return 0xf9a758u; }
RW_FUNCTION(0x65F8F8, "SceneSaleMedal::init $_6 __func::target_type", t_65F8F8);

// SceneSaleMedal::init $_6: __func::~__func (deleting)
static void t_65F90C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F90C, "SceneSaleMedal::init $_6 __func::~__func (deleting)", t_65F90C);

// SceneSaleMedal::init $_6: __func::__clone()
static uint32_t t_65F914(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf62a4cu);
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
RW_FUNCTION(0x65F914, "SceneSaleMedal::init $_6 __func::__clone()", t_65F914);

// SceneSaleMedal::init $_6: __func::__clone(__base*)
static void t_65F948(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62a4cu);
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
RW_FUNCTION(0x65F948, "SceneSaleMedal::init $_6 __func::__clone(__base*)", t_65F948);

// SceneSaleMedal::init $_6: __func::destroy
static void t_65F974(uint32_t self) {
}
RW_FUNCTION(0x65F974, "SceneSaleMedal::init $_6 __func::destroy", t_65F974);

// SceneSaleMedal::init $_6: __func::destroy_deallocate
static void t_65F976(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65F976, "SceneSaleMedal::init $_6 __func::destroy_deallocate", t_65F976);

// SceneSaleMedal::init $_6: __func::target
static uint32_t t_65F9FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe646b0u ? self + 8 : 0; }
RW_FUNCTION(0x65F9FC, "SceneSaleMedal::init $_6 __func::target", t_65F9FC);

// SceneSaleMedal::init $_6: __func::target_type
static uint32_t t_65FA20() { return 0xf9a76cu; }
RW_FUNCTION(0x65FA20, "SceneSaleMedal::init $_6 __func::target_type", t_65FA20);

// SceneSaleMedal::init $_6 (shared): __func::~__func
static uint32_t t_65FA34(uint32_t self) {
    return self;
}
RW_FUNCTION(0x65FA34, "SceneSaleMedal::init $_6 (shared) __func::~__func", t_65FA34);

// SceneSaleMedal::init $_6 (shared): __func::~__func
static uint32_t t_65FA36(uint32_t self) {
    return self;
}
RW_FUNCTION(0x65FA36, "SceneSaleMedal::init $_6 (shared) __func::~__func", t_65FA36);

// SceneSaleMedal::init $_5: __func::~__func (deleting)
static void t_65FA38(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FA38, "SceneSaleMedal::init $_5 __func::~__func (deleting)", t_65FA38);

// SceneSaleMedal::init $_5: __func::__clone()
static uint32_t t_65FA40(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62a78u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x65FA40, "SceneSaleMedal::init $_5 __func::__clone()", t_65FA40);

// SceneSaleMedal::init $_5: __func::__clone(__base*)
static void t_65FA6C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62a78u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65FA6C, "SceneSaleMedal::init $_5 __func::__clone(__base*)", t_65FA6C);

// SceneSaleMedal::init $_5: __func::destroy
static void t_65FA8C(uint32_t self) {
}
RW_FUNCTION(0x65FA8C, "SceneSaleMedal::init $_5 __func::destroy", t_65FA8C);

// SceneSaleMedal::init $_5: __func::destroy_deallocate
static void t_65FA8E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FA8E, "SceneSaleMedal::init $_5 __func::destroy_deallocate", t_65FA8E);

// SceneSaleMedal::init $_5: __func::target
static uint32_t t_65FAAC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64760u ? self + 4 : 0; }
RW_FUNCTION(0x65FAAC, "SceneSaleMedal::init $_5 __func::target", t_65FAAC);

// SceneSaleMedal::init $_5: __func::target_type
static uint32_t t_65FACC() { return 0xf9a780u; }
RW_FUNCTION(0x65FACC, "SceneSaleMedal::init $_5 __func::target_type", t_65FACC);

// SceneSaleMedal::init $_4: __func::~__func (deleting)
static void t_65FAE0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FAE0, "SceneSaleMedal::init $_4 __func::~__func (deleting)", t_65FAE0);

// SceneSaleMedal::init $_4: __func::__clone()
static uint32_t t_65FAE8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62aa4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x65FAE8, "SceneSaleMedal::init $_4 __func::__clone()", t_65FAE8);

// SceneSaleMedal::init $_4: __func::__clone(__base*)
static void t_65FB14(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62aa4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65FB14, "SceneSaleMedal::init $_4 __func::__clone(__base*)", t_65FB14);

// SceneSaleMedal::init $_4: __func::destroy
static void t_65FB34(uint32_t self) {
}
RW_FUNCTION(0x65FB34, "SceneSaleMedal::init $_4 __func::destroy", t_65FB34);

// SceneSaleMedal::init $_4: __func::destroy_deallocate
static void t_65FB36(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FB36, "SceneSaleMedal::init $_4 __func::destroy_deallocate", t_65FB36);

// SceneSaleMedal::init $_4: __func::target
static uint32_t t_65FC80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe647e0u ? self + 4 : 0; }
RW_FUNCTION(0x65FC80, "SceneSaleMedal::init $_4 __func::target", t_65FC80);

// SceneSaleMedal::init $_4: __func::target_type
static uint32_t t_65FCA0() { return 0xf9a794u; }
RW_FUNCTION(0x65FCA0, "SceneSaleMedal::init $_4 __func::target_type", t_65FCA0);

// SceneSaleMedal::init $_3: __func::~__func (deleting)
static void t_65FCB4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FCB4, "SceneSaleMedal::init $_3 __func::~__func (deleting)", t_65FCB4);

// SceneSaleMedal::init $_3: __func::__clone()
static uint32_t t_65FCBC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62ad0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x65FCBC, "SceneSaleMedal::init $_3 __func::__clone()", t_65FCBC);

// SceneSaleMedal::init $_3: __func::__clone(__base*)
static void t_65FCE8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62ad0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65FCE8, "SceneSaleMedal::init $_3 __func::__clone(__base*)", t_65FCE8);

// SceneSaleMedal::init $_3: __func::destroy
static void t_65FD08(uint32_t self) {
}
RW_FUNCTION(0x65FD08, "SceneSaleMedal::init $_3 __func::destroy", t_65FD08);

// SceneSaleMedal::init $_3: __func::destroy_deallocate
static void t_65FD0A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FD0A, "SceneSaleMedal::init $_3 __func::destroy_deallocate", t_65FD0A);

// SceneSaleMedal::init $_3: __func::target
static uint32_t t_65FD24(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64880u ? self + 4 : 0; }
RW_FUNCTION(0x65FD24, "SceneSaleMedal::init $_3 __func::target", t_65FD24);

// SceneSaleMedal::init $_3: __func::target_type
static uint32_t t_65FD44() { return 0xf9a7a8u; }
RW_FUNCTION(0x65FD44, "SceneSaleMedal::init $_3 __func::target_type", t_65FD44);

// SceneSaleMedal::init $_2: __func::~__func (deleting)
static void t_65FD58(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FD58, "SceneSaleMedal::init $_2 __func::~__func (deleting)", t_65FD58);

// SceneSaleMedal::init $_2: __func::__clone()
static uint32_t t_65FD60(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62afcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x65FD60, "SceneSaleMedal::init $_2 __func::__clone()", t_65FD60);

// SceneSaleMedal::init $_2: __func::__clone(__base*)
static void t_65FD8C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62afcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65FD8C, "SceneSaleMedal::init $_2 __func::__clone(__base*)", t_65FD8C);

// SceneSaleMedal::init $_2: __func::destroy
static void t_65FDAC(uint32_t self) {
}
RW_FUNCTION(0x65FDAC, "SceneSaleMedal::init $_2 __func::destroy", t_65FDAC);

// SceneSaleMedal::init $_2: __func::destroy_deallocate
static void t_65FDAE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FDAE, "SceneSaleMedal::init $_2 __func::destroy_deallocate", t_65FDAE);

// SceneSaleMedal::init $_2: __func::target
static uint32_t t_65FDD0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64920u ? self + 4 : 0; }
RW_FUNCTION(0x65FDD0, "SceneSaleMedal::init $_2 __func::target", t_65FDD0);

// SceneSaleMedal::init $_2: __func::target_type
static uint32_t t_65FDF0() { return 0xf9a7bcu; }
RW_FUNCTION(0x65FDF0, "SceneSaleMedal::init $_2 __func::target_type", t_65FDF0);

// SceneSaleMedal::init $_1: __func::~__func (deleting)
static void t_65FE04(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FE04, "SceneSaleMedal::init $_1 __func::~__func (deleting)", t_65FE04);

// SceneSaleMedal::init $_1: __func::__clone()
static uint32_t t_65FE0C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62b28u);
    }
    return v1;
}
RW_FUNCTION(0x65FE0C, "SceneSaleMedal::init $_1 __func::__clone()", t_65FE0C);

// SceneSaleMedal::init $_1: __func::__clone(__base*)
static void t_65FE30(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62b28u);
    }
}
RW_FUNCTION(0x65FE30, "SceneSaleMedal::init $_1 __func::__clone(__base*)", t_65FE30);

// SceneSaleMedal::init $_1: __func::destroy
static void t_65FE4C(uint32_t self) {
}
RW_FUNCTION(0x65FE4C, "SceneSaleMedal::init $_1 __func::destroy", t_65FE4C);

// SceneSaleMedal::init $_1: __func::destroy_deallocate
static void t_65FE4E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FE4E, "SceneSaleMedal::init $_1 __func::destroy_deallocate", t_65FE4E);

// SceneSaleMedal::init $_1: __func::target
static uint32_t t_65FEE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe649b0u ? self + 4 : 0; }
RW_FUNCTION(0x65FEE4, "SceneSaleMedal::init $_1 __func::target", t_65FEE4);

// SceneSaleMedal::init $_1: __func::target_type
static uint32_t t_65FF04() { return 0xf9a7d0u; }
RW_FUNCTION(0x65FF04, "SceneSaleMedal::init $_1 __func::target_type", t_65FF04);

// SceneSaleMedal::init $_0: __func::~__func (deleting)
static void t_65FF18(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FF18, "SceneSaleMedal::init $_0 __func::~__func (deleting)", t_65FF18);

// SceneSaleMedal::init $_0: __func::__clone()
static uint32_t t_65FF20(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62b54u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x65FF20, "SceneSaleMedal::init $_0 __func::__clone()", t_65FF20);

// SceneSaleMedal::init $_0: __func::__clone(__base*)
static void t_65FF4C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62b54u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x65FF4C, "SceneSaleMedal::init $_0 __func::__clone(__base*)", t_65FF4C);

// SceneSaleMedal::init $_0: __func::destroy
static void t_65FF6C(uint32_t self) {
}
RW_FUNCTION(0x65FF6C, "SceneSaleMedal::init $_0 __func::destroy", t_65FF6C);

// SceneSaleMedal::init $_0: __func::destroy_deallocate
static void t_65FF6E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x65FF6E, "SceneSaleMedal::init $_0 __func::destroy_deallocate", t_65FF6E);

// SceneSaleMedal::init $_0: __func::target
static uint32_t t_65FF80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64a30u ? self + 4 : 0; }
RW_FUNCTION(0x65FF80, "SceneSaleMedal::init $_0 __func::target", t_65FF80);

// SceneSaleMedal::init $_0: __func::target_type
static uint32_t t_65FFA0() { return 0xf9a7e4u; }
RW_FUNCTION(0x65FFA0, "SceneSaleMedal::init $_0 __func::target_type", t_65FFA0);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66DF24(uint32_t self) {
    st32(self, 0xf62b80u);
    uint32_t v1 = ld32(self + 224);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 208);
    functionDestroy(self + 192);
    uint32_t v3 = ld32(self + 12);
    if (v3) {
        call(0xc8dbd1, {v3});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66DF24, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66DF24);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66DF78(uint32_t self) {
    st32(self, 0xf62b80u);
    uint32_t v1 = ld32(self + 224);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 208);
    functionDestroy(self + 192);
    uint32_t v3 = ld32(self + 12);
    if (v3) {
        call(0xc8dbd1, {v3});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66DF78, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66DF78);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66E128(uint32_t self) {
    uint32_t v1 = ld32(self + 224);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 208);
    functionDestroy(self + 192);
    uint32_t v3 = ld32(self + 12);
    if (v3) {
        call(0xc8dbd1, {v3});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66E128, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66E128);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66E164(uint32_t self) {
    uint32_t v1 = ld32(self + 224);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 208);
    functionDestroy(self + 192);
    uint32_t v3 = ld32(self + 12);
    if (v3) {
        call(0xc8dbd1, {v3});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66E164, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66E164);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66E570(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64b90u ? self + 8 : 0; }
RW_FUNCTION(0x66E570, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66E570);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66E594() { return 0xf9ab74u; }
RW_FUNCTION(0x66E594, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66E594);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66E5A8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66E5A8, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66E5A8);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66E5B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf62bacu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x66E5B0, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66E5B0);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66E5E0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62bacu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x66E5E0, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66E5E0);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66E608(uint32_t self) {
}
RW_FUNCTION(0x66E608, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66E608);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66E60A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66E60A, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66E60A);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66E63C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64d80u ? self + 4 : 0; }
RW_FUNCTION(0x66E63C, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66E63C);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66E65C() { return 0xf9ab88u; }
RW_FUNCTION(0x66E65C, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66E65C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66E670(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66E670, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66E670);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66E678(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62bd8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x66E678, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66E678);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66E6A4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62bd8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x66E6A4, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66E6A4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66E6C4(uint32_t self) {
}
RW_FUNCTION(0x66E6C4, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66E6C4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66E6C6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66E6C6, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66E6C6);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66E6D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe64f70u ? self + 4 : 0; }
RW_FUNCTION(0x66E6D8, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66E6D8);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66E6F8() { return 0xf9ab9cu; }
RW_FUNCTION(0x66E6F8, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66E6F8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66E70C(uint32_t self) {
    st32(self, 0xf62c04u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66E70C, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66E70C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66E738(uint32_t self) {
    st32(self, 0xf62c04u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66E738, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66E738);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66E768(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf62c04u);
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
        uint32_t v7 = ld32(self + 16 + 0);
        uint32_t v8 = ld32(self + 16 + 4);
        uint32_t v9 = ld32(self + 16 + 8);
        uint32_t v10 = ld32(self + 16 + 12);
        st32(v1 + 28, v10);
        st32(v1 + 24, v9);
        st32(v1 + 20, v8);
        st32(v1 + 16, v7);
        uint32_t v11 = ld32(self + 32);
        st32(v1 + 32, v11);
        st32(v1 + 4, m6);
    }
    return v1;
}
RW_FUNCTION(0x66E768, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66E768);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66E7C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62c04u);
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
        uint32_t v7 = ld32(self + 16 + 12);
        st32(p + 16 + 0, v4);
        st32(p + 16 + 4, v5);
        st32(p + 16 + 8, v6);
        st32(p + 16 + 12, v7);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        uint32_t v9 = ld32(self + 4);
        st32(p + 4, v9);
    }
}
RW_FUNCTION(0x66E7C0, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66E7C0);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66E80C(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66E80C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66E80C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66E81A(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66E81A, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66E81A);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66E8A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe65180u ? self + 4 : 0; }
RW_FUNCTION(0x66E8A0, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66E8A0);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66E8C0() { return 0xf9abb0u; }
RW_FUNCTION(0x66E8C0, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66E8C0);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66E8D4(uint32_t self) {
    st32(self, 0xf62c30u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66E8D4, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66E8D4);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66E900(uint32_t self) {
    st32(self, 0xf62c30u);
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66E900, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66E900);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66E930(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {36});
    if (v1) {
        st32(v1, 0xf62c30u);
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
        uint32_t v7 = ld32(self + 16 + 0);
        uint32_t v8 = ld32(self + 16 + 4);
        uint32_t v9 = ld32(self + 16 + 8);
        uint32_t v10 = ld32(self + 16 + 12);
        st32(v1 + 28, v10);
        st32(v1 + 24, v9);
        st32(v1 + 20, v8);
        st32(v1 + 16, v7);
        uint32_t v11 = ld32(self + 32);
        st32(v1 + 32, v11);
        st32(v1 + 4, m6);
    }
    return v1;
}
RW_FUNCTION(0x66E930, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66E930);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66E988(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62c30u);
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
        uint32_t v7 = ld32(self + 16 + 12);
        st32(p + 16 + 0, v4);
        st32(p + 16 + 4, v5);
        st32(p + 16 + 8, v6);
        st32(p + 16 + 12, v7);
        uint32_t v8 = ld32(self + 32);
        st32(p + 32, v8);
        uint32_t v9 = ld32(self + 4);
        st32(p + 4, v9);
    }
}
RW_FUNCTION(0x66E988, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66E988);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66E9D4(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66E9D4, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66E9D4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66E9E2(uint32_t self) {
    uint32_t v1 = ld32(self + 12);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66E9E2, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66E9E2);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66EA68(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe65390u ? self + 4 : 0; }
RW_FUNCTION(0x66EA68, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66EA68);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66EA88() { return 0xf9abc4u; }
RW_FUNCTION(0x66EA88, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66EA88);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66EA9C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66EA9C, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66EA9C);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66EAA4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62c5cu);
    }
    return v1;
}
RW_FUNCTION(0x66EAA4, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66EAA4);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66EAC8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62c5cu);
    }
}
RW_FUNCTION(0x66EAC8, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66EAC8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66EAE4(uint32_t self) {
}
RW_FUNCTION(0x66EAE4, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66EAE4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66EAE6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66EAE6, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66EAE6);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66EB08(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe655a0u ? self + 4 : 0; }
RW_FUNCTION(0x66EB08, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66EB08);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66EB28() { return 0xf9abd8u; }
RW_FUNCTION(0x66EB28, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66EB28);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66EB3C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66EB3C, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66EB3C);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66EB44(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf62c88u);
    }
    return v1;
}
RW_FUNCTION(0x66EB44, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66EB44);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66EB68(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62c88u);
    }
}
RW_FUNCTION(0x66EB68, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66EB68);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66EB84(uint32_t self) {
}
RW_FUNCTION(0x66EB84, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66EB84);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66EB86(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66EB86, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66EB86);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66EBA8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe657b0u ? self + 4 : 0; }
RW_FUNCTION(0x66EBA8, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66EBA8);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66EBC8() { return 0xf9abecu; }
RW_FUNCTION(0x66EBC8, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66EBC8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66EBDC(uint32_t self) {
    st32(self, 0xf62cb4u);
    uint32_t v1 = ld32(self + 96);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66EBDC, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66EBDC);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66EC08(uint32_t self) {
    st32(self, 0xf62cb4u);
    uint32_t v1 = ld32(self + 96);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66EC08, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66EC08);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66ECC8(uint32_t self) {
    uint32_t v1 = ld32(self + 96);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66ECC8, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66ECC8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66ECD6(uint32_t self) {
    uint32_t v1 = ld32(self + 96);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66ECD6, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66ECD6);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66EE30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe659c0u ? self + 4 : 0; }
RW_FUNCTION(0x66EE30, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66EE30);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66EE50() { return 0xf9ac00u; }
RW_FUNCTION(0x66EE50, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66EE50);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66EE64(uint32_t self) {
    st32(self, 0xf62ce0u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66EE64, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66EE64);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66EE98(uint32_t self) {
    st32(self, 0xf62ce0u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66EE98, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66EE98);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66EED0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62ce0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x66EED0, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66EED0);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66EF30(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62ce0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x66EF30, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66EF30);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66EF84(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66EF84, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66EF84);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66EFA0(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66EFA0, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66EFA0);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66F074(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe65bd0u ? self + 4 : 0; }
RW_FUNCTION(0x66F074, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66F074);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66F094() { return 0xf9ac14u; }
RW_FUNCTION(0x66F094, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66F094);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66F0A8(uint32_t self) {
    st32(self, 0xf62d0cu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66F0A8, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66F0A8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66F0DC(uint32_t self) {
    st32(self, 0xf62d0cu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66F0DC, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66F0DC);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66F114(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62d0cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x66F114, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66F114);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66F174(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62d0cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x66F174, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66F174);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66F1C8(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66F1C8, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66F1C8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66F1E4(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66F1E4, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66F1E4);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66F2B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe65de0u ? self + 4 : 0; }
RW_FUNCTION(0x66F2B8, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66F2B8);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66F2D8() { return 0xf9ac28u; }
RW_FUNCTION(0x66F2D8, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66F2D8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66F2EC(uint32_t self) {
    st32(self, 0xf62d38u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66F2EC, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66F2EC);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66F320(uint32_t self) {
    st32(self, 0xf62d38u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66F320, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66F320);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66F358(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62d38u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x66F358, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66F358);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66F3B8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62d38u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x66F3B8, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66F3B8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66F40C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66F40C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66F40C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66F428(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66F428, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66F428);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66F4FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe65ff0u ? self + 4 : 0; }
RW_FUNCTION(0x66F4FC, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66F4FC);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66F51C() { return 0xf9ac3cu; }
RW_FUNCTION(0x66F51C, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66F51C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66F530(uint32_t self) {
    st32(self, 0xf62d64u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66F530, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66F530);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66F564(uint32_t self) {
    st32(self, 0xf62d64u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66F564, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66F564);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66F59C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62d64u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x66F59C, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66F59C);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66F5FC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62d64u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x66F5FC, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66F5FC);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66F650(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66F650, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66F650);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66F66C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66F66C, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66F66C);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66F740(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe66200u ? self + 4 : 0; }
RW_FUNCTION(0x66F740, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66F740);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66F760() { return 0xf9ac50u; }
RW_FUNCTION(0x66F760, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66F760);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66F774(uint32_t self) {
    st32(self, 0xf62d90u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66F774, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66F774);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66F7A8(uint32_t self) {
    st32(self, 0xf62d90u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66F7A8, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66F7A8);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66F7E0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62d90u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x66F7E0, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66F7E0);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66F840(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62d90u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x66F840, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66F840);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66F894(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66F894, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66F894);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66F8B0(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66F8B0, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66F8B0);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66F984(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe66410u ? self + 4 : 0; }
RW_FUNCTION(0x66F984, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66F984);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66F9A4() { return 0xf9ac64u; }
RW_FUNCTION(0x66F9A4, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66F9A4);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66F9B8(uint32_t self) {
    st32(self, 0xf62dbcu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66F9B8, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66F9B8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66F9EC(uint32_t self) {
    st32(self, 0xf62dbcu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66F9EC, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66F9EC);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66FA24(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62dbcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x66FA24, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66FA24);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66FA84(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62dbcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x66FA84, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66FA84);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66FAD8(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66FAD8, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66FAD8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66FAF4(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66FAF4, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66FAF4);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66FBC8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe66620u ? self + 4 : 0; }
RW_FUNCTION(0x66FBC8, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66FBC8);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66FBE8() { return 0xf9ac78u; }
RW_FUNCTION(0x66FBE8, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66FBE8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66FBFC(uint32_t self) {
    st32(self, 0xf62de8u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66FBFC, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66FBFC);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66FC30(uint32_t self) {
    st32(self, 0xf62de8u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66FC30, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66FC30);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66FC68(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62de8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x66FC68, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66FC68);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66FCC8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62de8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x66FCC8, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66FCC8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66FD1C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66FD1C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66FD1C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66FD38(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66FD38, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66FD38);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_66FE0C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe66830u ? self + 4 : 0; }
RW_FUNCTION(0x66FE0C, "IMedalSortFilter::openMedalSortPopup  __func::target", t_66FE0C);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_66FE2C() { return 0xf9ac8cu; }
RW_FUNCTION(0x66FE2C, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_66FE2C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_66FE40(uint32_t self) {
    st32(self, 0xf62e14u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x66FE40, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_66FE40);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_66FE74(uint32_t self) {
    st32(self, 0xf62e14u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66FE74, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_66FE74);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_66FEAC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62e14u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x66FEAC, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_66FEAC);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_66FF0C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62e14u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x66FF0C, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_66FF0C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_66FF60(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x66FF60, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_66FF60);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_66FF7C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x66FF7C, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_66FF7C);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_670050(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe66a40u ? self + 4 : 0; }
RW_FUNCTION(0x670050, "IMedalSortFilter::openMedalSortPopup  __func::target", t_670050);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_670070() { return 0xf9aca0u; }
RW_FUNCTION(0x670070, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_670070);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_670084(uint32_t self) {
    st32(self, 0xf62e40u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x670084, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_670084);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_6700B8(uint32_t self) {
    st32(self, 0xf62e40u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6700B8, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_6700B8);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_6700F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62e40u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x6700F0, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_6700F0);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_670150(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62e40u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x670150, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_670150);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_6701A4(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x6701A4, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_6701A4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_6701C0(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6701C0, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_6701C0);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_670294(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe66c50u ? self + 4 : 0; }
RW_FUNCTION(0x670294, "IMedalSortFilter::openMedalSortPopup  __func::target", t_670294);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_6702B4() { return 0xf9acb4u; }
RW_FUNCTION(0x6702B4, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_6702B4);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_6702C8(uint32_t self) {
    st32(self, 0xf62e6cu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6702C8, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_6702C8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_6702FC(uint32_t self) {
    st32(self, 0xf62e6cu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6702FC, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_6702FC);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_670334(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62e6cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x670334, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_670334);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_670394(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62e6cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x670394, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_670394);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_6703E8(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x6703E8, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_6703E8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_670404(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x670404, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_670404);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_6704D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe66e60u ? self + 4 : 0; }
RW_FUNCTION(0x6704D8, "IMedalSortFilter::openMedalSortPopup  __func::target", t_6704D8);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_6704F8() { return 0xf9acc8u; }
RW_FUNCTION(0x6704F8, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_6704F8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_67050C(uint32_t self) {
    st32(self, 0xf62e98u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x67050C, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_67050C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_670540(uint32_t self) {
    st32(self, 0xf62e98u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x670540, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_670540);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_670578(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62e98u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x670578, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_670578);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_6705D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62e98u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x6705D8, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_6705D8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_67062C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x67062C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_67062C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_670648(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x670648, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_670648);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_67071C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe67070u ? self + 4 : 0; }
RW_FUNCTION(0x67071C, "IMedalSortFilter::openMedalSortPopup  __func::target", t_67071C);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_67073C() { return 0xf9acdcu; }
RW_FUNCTION(0x67073C, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_67073C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_670750(uint32_t self) {
    st32(self, 0xf62ec4u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x670750, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_670750);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_670784(uint32_t self) {
    st32(self, 0xf62ec4u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x670784, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_670784);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_6707BC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62ec4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x6707BC, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_6707BC);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_67081C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62ec4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x67081C, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_67081C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_670870(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x670870, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_670870);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_67088C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67088C, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_67088C);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_670960(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe67280u ? self + 4 : 0; }
RW_FUNCTION(0x670960, "IMedalSortFilter::openMedalSortPopup  __func::target", t_670960);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_670980() { return 0xf9acf0u; }
RW_FUNCTION(0x670980, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_670980);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_670994(uint32_t self) {
    st32(self, 0xf62ef0u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x670994, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_670994);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_6709C8(uint32_t self) {
    st32(self, 0xf62ef0u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6709C8, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_6709C8);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_670A00(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62ef0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x670A00, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_670A00);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_670A60(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62ef0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x670A60, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_670A60);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_670AB4(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x670AB4, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_670AB4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_670AD0(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x670AD0, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_670AD0);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_670BA4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe67490u ? self + 4 : 0; }
RW_FUNCTION(0x670BA4, "IMedalSortFilter::openMedalSortPopup  __func::target", t_670BA4);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_670BC4() { return 0xf9ad04u; }
RW_FUNCTION(0x670BC4, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_670BC4);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_670BD8(uint32_t self) {
    st32(self, 0xf62f1cu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x670BD8, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_670BD8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_670C0C(uint32_t self) {
    st32(self, 0xf62f1cu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x670C0C, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_670C0C);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_670C44(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62f1cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x670C44, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_670C44);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_670CA4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62f1cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x670CA4, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_670CA4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_670CF8(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x670CF8, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_670CF8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_670D14(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x670D14, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_670D14);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_670DE8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe676a0u ? self + 4 : 0; }
RW_FUNCTION(0x670DE8, "IMedalSortFilter::openMedalSortPopup  __func::target", t_670DE8);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_670E08() { return 0xf9ad18u; }
RW_FUNCTION(0x670E08, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_670E08);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_670E1C(uint32_t self) {
    st32(self, 0xf62f48u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x670E1C, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_670E1C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_670E50(uint32_t self) {
    st32(self, 0xf62f48u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x670E50, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_670E50);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_670E88(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62f48u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x670E88, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_670E88);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_670EE8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62f48u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x670EE8, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_670EE8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_670F3C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x670F3C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_670F3C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_670F58(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x670F58, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_670F58);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_67102C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe678b0u ? self + 4 : 0; }
RW_FUNCTION(0x67102C, "IMedalSortFilter::openMedalSortPopup  __func::target", t_67102C);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_67104C() { return 0xf9ad2cu; }
RW_FUNCTION(0x67104C, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_67104C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_671060(uint32_t self) {
    st32(self, 0xf62f74u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x671060, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_671060);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_671094(uint32_t self) {
    st32(self, 0xf62f74u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671094, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_671094);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_6710CC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62f74u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x6710CC, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_6710CC);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_67112C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62f74u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x67112C, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_67112C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_671180(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x671180, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_671180);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_67119C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67119C, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_67119C);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_671270(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe67ac0u ? self + 4 : 0; }
RW_FUNCTION(0x671270, "IMedalSortFilter::openMedalSortPopup  __func::target", t_671270);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_671290() { return 0xf9ad40u; }
RW_FUNCTION(0x671290, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_671290);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_6712A4(uint32_t self) {
    st32(self, 0xf62fa0u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6712A4, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_6712A4);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_6712D8(uint32_t self) {
    st32(self, 0xf62fa0u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6712D8, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_6712D8);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_671310(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62fa0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x671310, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_671310);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_671370(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62fa0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x671370, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_671370);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_6713C4(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x6713C4, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_6713C4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_6713E0(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6713E0, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_6713E0);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_6714B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe67cd0u ? self + 4 : 0; }
RW_FUNCTION(0x6714B4, "IMedalSortFilter::openMedalSortPopup  __func::target", t_6714B4);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_6714D4() { return 0xf9ad54u; }
RW_FUNCTION(0x6714D4, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_6714D4);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_6714E8(uint32_t self) {
    st32(self, 0xf62fccu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6714E8, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_6714E8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_67151C(uint32_t self) {
    st32(self, 0xf62fccu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67151C, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_67151C);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_671554(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62fccu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x671554, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_671554);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_6715B4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62fccu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x6715B4, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_6715B4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_671608(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x671608, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_671608);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_671624(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671624, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_671624);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_6716F8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe67ee0u ? self + 4 : 0; }
RW_FUNCTION(0x6716F8, "IMedalSortFilter::openMedalSortPopup  __func::target", t_6716F8);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_671718() { return 0xf9ad68u; }
RW_FUNCTION(0x671718, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_671718);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_67172C(uint32_t self) {
    st32(self, 0xf62ff8u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x67172C, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_67172C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_671760(uint32_t self) {
    st32(self, 0xf62ff8u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671760, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_671760);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_671798(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf62ff8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x671798, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_671798);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_6717F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf62ff8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x6717F8, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_6717F8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_67184C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x67184C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_67184C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_671868(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671868, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_671868);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_67193C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe680f0u ? self + 4 : 0; }
RW_FUNCTION(0x67193C, "IMedalSortFilter::openMedalSortPopup  __func::target", t_67193C);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_67195C() { return 0xf9ad7cu; }
RW_FUNCTION(0x67195C, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_67195C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_671970(uint32_t self) {
    st32(self, 0xf63024u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x671970, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_671970);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_6719A4(uint32_t self) {
    st32(self, 0xf63024u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6719A4, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_6719A4);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_6719DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf63024u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t m8 = v4;
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
            uint32_t v7 = ld32(self + 12);
            m8 = v7;
        }
        st32(v1 + 12, m8);
        uint32_t v9 = ld32(self + 24);
        uint32_t v10 = ld32(self + 28);
        st32(v1 + 24, v9);
        st32(v1 + 28, v10);
    }
    return v1;
}
RW_FUNCTION(0x6719DC, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_6719DC);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_671A3C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63024u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 12);
        st32(p + 12, v6);
        uint32_t v7 = ld32(self + 24);
        uint32_t v8 = ld32(self + 28);
        st32(p + 24, v7);
        st32(p + 28, v8);
    }
}
RW_FUNCTION(0x671A3C, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_671A3C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_671A90(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x671A90, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_671A90);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_671AAC(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 8);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671AAC, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_671AAC);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_671B80(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe68300u ? self + 4 : 0; }
RW_FUNCTION(0x671B80, "IMedalSortFilter::openMedalSortPopup  __func::target", t_671B80);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_671BA0() { return 0xf9ad90u; }
RW_FUNCTION(0x671BA0, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_671BA0);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_671BB4(uint32_t self) {
    st32(self, 0xf63050u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x671BB4, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_671BB4);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_671BE0(uint32_t self) {
    st32(self, 0xf63050u);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671BE0, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_671BE0);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_671C98(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x671C98, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_671C98);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_671CA6(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671CA6, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_671CA6);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_671D24(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe68510u ? self + 4 : 0; }
RW_FUNCTION(0x671D24, "IMedalSortFilter::openMedalSortPopup  __func::target", t_671D24);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_671D44() { return 0xf9ada4u; }
RW_FUNCTION(0x671D44, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_671D44);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_671D58(uint32_t self) {
    st32(self, 0xf6307cu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x671D58, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_671D58);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_671D84(uint32_t self) {
    st32(self, 0xf6307cu);
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671D84, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_671D84);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_671E3C(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x671E3C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_671E3C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_671E4A(uint32_t self) {
    uint32_t v1 = ld32(self + 20);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671E4A, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_671E4A);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_671EC8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe68720u ? self + 4 : 0; }
RW_FUNCTION(0x671EC8, "IMedalSortFilter::openMedalSortPopup  __func::target", t_671EC8);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_671EE8() { return 0xf9adb8u; }
RW_FUNCTION(0x671EE8, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_671EE8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_671EFC(uint32_t self) {
    st32(self, 0xf630a8u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x671EFC, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_671EFC);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_671F28(uint32_t self) {
    st32(self, 0xf630a8u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x671F28, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_671F28);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_671F58(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf630a8u);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x671F58, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_671F58);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_671FB8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf630a8u);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x671FB8, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_671FB8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_672004(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x672004, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_672004);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_672012(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x672012, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_672012);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_672174(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe68930u ? self + 4 : 0; }
RW_FUNCTION(0x672174, "IMedalSortFilter::openMedalSortPopup  __func::target", t_672174);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_672194() { return 0xf9adccu; }
RW_FUNCTION(0x672194, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_672194);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_6721A8(uint32_t self) {
    st32(self, 0xf630d4u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6721A8, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_6721A8);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_6721D4(uint32_t self) {
    st32(self, 0xf630d4u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6721D4, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_6721D4);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_672204(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf630d4u);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x672204, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_672204);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_672264(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf630d4u);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x672264, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_672264);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_6722B0(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x6722B0, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_6722B0);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_6722BE(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6722BE, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_6722BE);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_672420(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe68b40u ? self + 4 : 0; }
RW_FUNCTION(0x672420, "IMedalSortFilter::openMedalSortPopup  __func::target", t_672420);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_672440() { return 0xf9ade0u; }
RW_FUNCTION(0x672440, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_672440);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_672454(uint32_t self) {
    st32(self, 0xf63100u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x672454, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_672454);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_672480(uint32_t self) {
    st32(self, 0xf63100u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x672480, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_672480);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_6724B0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf63100u);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x6724B0, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_6724B0);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_672510(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63100u);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x672510, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_672510);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_67255C(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x67255C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_67255C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_67256A(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67256A, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_67256A);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_6726CC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe68d50u ? self + 4 : 0; }
RW_FUNCTION(0x6726CC, "IMedalSortFilter::openMedalSortPopup  __func::target", t_6726CC);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_6726EC() { return 0xf9adf4u; }
RW_FUNCTION(0x6726EC, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_6726EC);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_672700(uint32_t self) {
    st32(self, 0xf6312cu);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x672700, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_672700);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_67272C(uint32_t self) {
    st32(self, 0xf6312cu);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67272C, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_67272C);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_67275C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf6312cu);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x67275C, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_67275C);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_6727BC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6312cu);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x6727BC, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_6727BC);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_672808(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x672808, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_672808);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_672816(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x672816, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_672816);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_672978(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe68f60u ? self + 4 : 0; }
RW_FUNCTION(0x672978, "IMedalSortFilter::openMedalSortPopup  __func::target", t_672978);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_672998() { return 0xf9ae08u; }
RW_FUNCTION(0x672998, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_672998);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_6729AC(uint32_t self) {
    st32(self, 0xf63158u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6729AC, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_6729AC);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_6729D8(uint32_t self) {
    st32(self, 0xf63158u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6729D8, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_6729D8);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_672A08(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf63158u);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x672A08, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_672A08);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_672A68(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63158u);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x672A68, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_672A68);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_672AB4(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x672AB4, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_672AB4);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_672AC2(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x672AC2, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_672AC2);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_672C24(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe69170u ? self + 4 : 0; }
RW_FUNCTION(0x672C24, "IMedalSortFilter::openMedalSortPopup  __func::target", t_672C24);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_672C44() { return 0xf9ae1cu; }
RW_FUNCTION(0x672C44, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_672C44);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_672C58(uint32_t self) {
    st32(self, 0xf63184u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x672C58, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_672C58);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_672C84(uint32_t self) {
    st32(self, 0xf63184u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x672C84, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_672C84);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_672CB4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf63184u);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x672CB4, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_672CB4);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_672D14(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63184u);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x672D14, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_672D14);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_672D60(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x672D60, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_672D60);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_672D6E(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x672D6E, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_672D6E);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_672ED0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe69380u ? self + 4 : 0; }
RW_FUNCTION(0x672ED0, "IMedalSortFilter::openMedalSortPopup  __func::target", t_672ED0);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_672EF0() { return 0xf9ae30u; }
RW_FUNCTION(0x672EF0, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_672EF0);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_672F04(uint32_t self) {
    st32(self, 0xf631b0u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x672F04, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_672F04);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_672F30(uint32_t self) {
    st32(self, 0xf631b0u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x672F30, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_672F30);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_672F60(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf631b0u);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x672F60, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_672F60);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_672FC0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf631b0u);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x672FC0, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_672FC0);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_67300C(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x67300C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_67300C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_67301A(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67301A, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_67301A);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_67317C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe69590u ? self + 4 : 0; }
RW_FUNCTION(0x67317C, "IMedalSortFilter::openMedalSortPopup  __func::target", t_67317C);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_67319C() { return 0xf9ae44u; }
RW_FUNCTION(0x67319C, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_67319C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_6731B0(uint32_t self) {
    st32(self, 0xf631dcu);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6731B0, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_6731B0);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_6731DC(uint32_t self) {
    st32(self, 0xf631dcu);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6731DC, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_6731DC);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_67320C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf631dcu);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x67320C, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_67320C);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_67326C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf631dcu);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x67326C, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_67326C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_6732B8(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x6732B8, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_6732B8);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_6732C6(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6732C6, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_6732C6);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_673428(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe697a0u ? self + 4 : 0; }
RW_FUNCTION(0x673428, "IMedalSortFilter::openMedalSortPopup  __func::target", t_673428);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_673448() { return 0xf9ae58u; }
RW_FUNCTION(0x673448, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_673448);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_67345C(uint32_t self) {
    st32(self, 0xf63208u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x67345C, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_67345C);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_673488(uint32_t self) {
    st32(self, 0xf63208u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x673488, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_673488);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_6734B8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf63208u);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x6734B8, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_6734B8);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_673518(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63208u);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x673518, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_673518);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_673564(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x673564, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_673564);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_673572(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x673572, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_673572);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_6736D0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe699b0u ? self + 4 : 0; }
RW_FUNCTION(0x6736D0, "IMedalSortFilter::openMedalSortPopup  __func::target", t_6736D0);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_6736F0() { return 0xf9ae6cu; }
RW_FUNCTION(0x6736F0, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_6736F0);

// IMedalSortFilter::openMedalSortPopup : __func::~__func
static uint32_t t_673704(uint32_t self) {
    st32(self, 0xf63234u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x673704, "IMedalSortFilter::openMedalSortPopup  __func::~__func", t_673704);

// IMedalSortFilter::openMedalSortPopup : __func::~__func (deleting)
static void t_673730(uint32_t self) {
    st32(self, 0xf63234u);
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x673730, "IMedalSortFilter::openMedalSortPopup  __func::~__func (deleting)", t_673730);

// IMedalSortFilter::openMedalSortPopup : __func::__clone()
static uint32_t t_673760(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf63234u);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        st32(v1 + 52, v14);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        if (v15) {
            call(0xc8dbb5, {v15});   // sub_C8DBB4
        }
    }
    return v1;
}
RW_FUNCTION(0x673760, "IMedalSortFilter::openMedalSortPopup  __func::__clone()", t_673760);

// IMedalSortFilter::openMedalSortPopup : __func::__clone(__base*)
static void t_6737C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63234u);
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
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        st32(p + 52, v13);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        if (v14) {
            call(0xc8dbb5, {v14});   // sub_C8DBB4
        }
    }
}
RW_FUNCTION(0x6737C0, "IMedalSortFilter::openMedalSortPopup  __func::__clone(__base*)", t_6737C0);

// IMedalSortFilter::openMedalSortPopup : __func::destroy
static void t_67380C(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x67380C, "IMedalSortFilter::openMedalSortPopup  __func::destroy", t_67380C);

// IMedalSortFilter::openMedalSortPopup : __func::destroy_deallocate
static void t_67381A(uint32_t self) {
    uint32_t v1 = ld32(self + 56);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67381A, "IMedalSortFilter::openMedalSortPopup  __func::destroy_deallocate", t_67381A);

// IMedalSortFilter::openMedalSortPopup : __func::target
static uint32_t t_673988(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe69bc0u ? self + 4 : 0; }
RW_FUNCTION(0x673988, "IMedalSortFilter::openMedalSortPopup  __func::target", t_673988);

// IMedalSortFilter::openMedalSortPopup : __func::target_type
static uint32_t t_6739A8() { return 0xf9ae80u; }
RW_FUNCTION(0x6739A8, "IMedalSortFilter::openMedalSortPopup  __func::target_type", t_6739A8);

// SceneDeckEdit::backScene $_49: __func::~__func (deleting)
static void t_674E8C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x674E8C, "SceneDeckEdit::backScene $_49 __func::~__func (deleting)", t_674E8C);

// SceneDeckEdit::backScene $_49: __func::__clone()
static uint32_t t_674E94(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf632b8u);
    }
    return v1;
}
RW_FUNCTION(0x674E94, "SceneDeckEdit::backScene $_49 __func::__clone()", t_674E94);

// SceneDeckEdit::backScene $_49: __func::__clone(__base*)
static void t_674EB8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf632b8u);
    }
}
RW_FUNCTION(0x674EB8, "SceneDeckEdit::backScene $_49 __func::__clone(__base*)", t_674EB8);

// SceneDeckEdit::backScene $_49: __func::destroy
static void t_674ED4(uint32_t self) {
}
RW_FUNCTION(0x674ED4, "SceneDeckEdit::backScene $_49 __func::destroy", t_674ED4);

// SceneDeckEdit::backScene $_49: __func::destroy_deallocate
static void t_674ED6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x674ED6, "SceneDeckEdit::backScene $_49 __func::destroy_deallocate", t_674ED6);

// SceneDeckEdit::backScene $_49: __func::target
static uint32_t t_675000(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe69fa0u ? self + 4 : 0; }
RW_FUNCTION(0x675000, "SceneDeckEdit::backScene $_49 __func::target", t_675000);

// SceneDeckEdit::backScene $_49: __func::target_type
static uint32_t t_675020() { return 0xf9aebcu; }
RW_FUNCTION(0x675020, "SceneDeckEdit::backScene $_49 __func::target_type", t_675020);

// SceneDeckEdit::playKeybladeEvolve $_48: __func::~__func (deleting)
static void t_675034(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x675034, "SceneDeckEdit::playKeybladeEvolve $_48 __func::~__func (deleting)", t_675034);

// SceneDeckEdit::playKeybladeEvolve $_48: __func::__clone()
static uint32_t t_67503C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf632e4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x67503C, "SceneDeckEdit::playKeybladeEvolve $_48 __func::__clone()", t_67503C);

// SceneDeckEdit::playKeybladeEvolve $_48: __func::__clone(__base*)
static void t_675068(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf632e4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x675068, "SceneDeckEdit::playKeybladeEvolve $_48 __func::__clone(__base*)", t_675068);

// SceneDeckEdit::playKeybladeEvolve $_48: __func::destroy
static void t_675088(uint32_t self) {
}
RW_FUNCTION(0x675088, "SceneDeckEdit::playKeybladeEvolve $_48 __func::destroy", t_675088);

// SceneDeckEdit::playKeybladeEvolve $_48: __func::destroy_deallocate
static void t_67508A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67508A, "SceneDeckEdit::playKeybladeEvolve $_48 __func::destroy_deallocate", t_67508A);

// SceneDeckEdit::playKeybladeEvolve $_48: __func::target
static uint32_t t_67509C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6a090u ? self + 4 : 0; }
RW_FUNCTION(0x67509C, "SceneDeckEdit::playKeybladeEvolve $_48 __func::target", t_67509C);

// SceneDeckEdit::playKeybladeEvolve $_48: __func::target_type
static uint32_t t_6750BC() { return 0xf9aed0u; }
RW_FUNCTION(0x6750BC, "SceneDeckEdit::playKeybladeEvolve $_48 __func::target_type", t_6750BC);

// SceneDeckEdit::playKeybladeEvolve $_47: __func::~__func
static uint32_t t_6750D0(uint32_t self) {
    st32(self, 0xf63310u);
    uint32_t v1 = ld32(self + 8);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x6750D0, "SceneDeckEdit::playKeybladeEvolve $_47 __func::~__func", t_6750D0);

// SceneDeckEdit::playKeybladeEvolve $_47: __func::~__func (deleting)
static void t_6750FC(uint32_t self) {
    st32(self, 0xf63310u);
    uint32_t v1 = ld32(self + 8);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6750FC, "SceneDeckEdit::playKeybladeEvolve $_47 __func::~__func (deleting)", t_6750FC);

// SceneDeckEdit::playKeybladeEvolve $_47: __func::__clone()
static uint32_t t_67512C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf63310u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
    }
    return v1;
}
RW_FUNCTION(0x67512C, "SceneDeckEdit::playKeybladeEvolve $_47 __func::__clone()", t_67512C);

// SceneDeckEdit::playKeybladeEvolve $_47: __func::__clone(__base*)
static void t_675170(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63310u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
    }
}
RW_FUNCTION(0x675170, "SceneDeckEdit::playKeybladeEvolve $_47 __func::__clone(__base*)", t_675170);

// SceneDeckEdit::playKeybladeEvolve $_47: __func::destroy
static void t_6751A8(uint32_t self) {
    uint32_t v1 = ld32(self + 8);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x6751A8, "SceneDeckEdit::playKeybladeEvolve $_47 __func::destroy", t_6751A8);

// SceneDeckEdit::playKeybladeEvolve $_47: __func::destroy_deallocate
static void t_6751B6(uint32_t self) {
    uint32_t v1 = ld32(self + 8);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6751B6, "SceneDeckEdit::playKeybladeEvolve $_47 __func::destroy_deallocate", t_6751B6);

// SceneDeckEdit::playKeybladeEvolve $_47: __func::target
static uint32_t t_6752FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6a1e0u ? self + 4 : 0; }
RW_FUNCTION(0x6752FC, "SceneDeckEdit::playKeybladeEvolve $_47 __func::target", t_6752FC);

// SceneDeckEdit::playKeybladeEvolve $_47: __func::target_type
static uint32_t t_67531C() { return 0xf9aee4u; }
RW_FUNCTION(0x67531C, "SceneDeckEdit::playKeybladeEvolve $_47 __func::target_type", t_67531C);

// SceneDeckEdit::playKeybladeEvolve $_46: __func::~__func
static uint32_t t_675330(uint32_t self) {
    st32(self, 0xf6333cu);
    uint32_t v1 = ld32(self + 8);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x675330, "SceneDeckEdit::playKeybladeEvolve $_46 __func::~__func", t_675330);

// SceneDeckEdit::playKeybladeEvolve $_46: __func::~__func (deleting)
static void t_67535C(uint32_t self) {
    st32(self, 0xf6333cu);
    uint32_t v1 = ld32(self + 8);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67535C, "SceneDeckEdit::playKeybladeEvolve $_46 __func::~__func (deleting)", t_67535C);

// SceneDeckEdit::playKeybladeEvolve $_46: __func::__clone()
static uint32_t t_67538C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf6333cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        if (v3) {
            call(0xc8dbb5, {v3});   // sub_C8DBB4
        }
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
    }
    return v1;
}
RW_FUNCTION(0x67538C, "SceneDeckEdit::playKeybladeEvolve $_46 __func::__clone()", t_67538C);

// SceneDeckEdit::playKeybladeEvolve $_46: __func::__clone(__base*)
static void t_6753D0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6333cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        if (v2) {
            call(0xc8dbb5, {v2});   // sub_C8DBB4
        }
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
    }
}
RW_FUNCTION(0x6753D0, "SceneDeckEdit::playKeybladeEvolve $_46 __func::__clone(__base*)", t_6753D0);

// SceneDeckEdit::playKeybladeEvolve $_46: __func::destroy
static void t_675408(uint32_t self) {
    uint32_t v1 = ld32(self + 8);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x675408, "SceneDeckEdit::playKeybladeEvolve $_46 __func::destroy", t_675408);

// SceneDeckEdit::playKeybladeEvolve $_46: __func::destroy_deallocate
static void t_675416(uint32_t self) {
    uint32_t v1 = ld32(self + 8);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x675416, "SceneDeckEdit::playKeybladeEvolve $_46 __func::destroy_deallocate", t_675416);

// SceneDeckEdit::playKeybladeEvolve $_46: __func::target
static uint32_t t_67555C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6a330u ? self + 4 : 0; }
RW_FUNCTION(0x67555C, "SceneDeckEdit::playKeybladeEvolve $_46 __func::target", t_67555C);

// SceneDeckEdit::playKeybladeEvolve $_46: __func::target_type
static uint32_t t_67557C() { return 0xf9aef8u; }
RW_FUNCTION(0x67557C, "SceneDeckEdit::playKeybladeEvolve $_46 __func::target_type", t_67557C);

// SceneDeckEdit::playKeybladeEvolve $_45: __func::~__func
static uint32_t t_675590(uint32_t self) {
    st32(self, 0xf63368u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x675590, "SceneDeckEdit::playKeybladeEvolve $_45 __func::~__func", t_675590);

// SceneDeckEdit::playKeybladeEvolve $_45: __func::~__func (deleting)
static void t_6755CC(uint32_t self) {
    st32(self, 0xf63368u);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6755CC, "SceneDeckEdit::playKeybladeEvolve $_45 __func::~__func (deleting)", t_6755CC);

// SceneDeckEdit::playKeybladeEvolve $_45: __func::__clone()
static uint32_t t_67560C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf63368u);
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
RW_FUNCTION(0x67560C, "SceneDeckEdit::playKeybladeEvolve $_45 __func::__clone()", t_67560C);

// SceneDeckEdit::playKeybladeEvolve $_45: __func::__clone(__base*)
static void t_675668(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63368u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x675668, "SceneDeckEdit::playKeybladeEvolve $_45 __func::__clone(__base*)", t_675668);

// SceneDeckEdit::playKeybladeEvolve $_45: __func::destroy
static void t_6756A8(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x6756A8, "SceneDeckEdit::playKeybladeEvolve $_45 __func::destroy", t_6756A8);

// SceneDeckEdit::playKeybladeEvolve $_45: __func::destroy_deallocate
static void t_6756C8(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6756C8, "SceneDeckEdit::playKeybladeEvolve $_45 __func::destroy_deallocate", t_6756C8);
