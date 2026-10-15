// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneMedalList lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneMedalList::init $_1 (shared): __func::~__func
static uint32_t t_606D60(uint32_t self) {
    return self;
}
RW_FUNCTION(0x606D60, "SceneMedalList::init $_1 (shared) __func::~__func", t_606D60);

// SceneMedalList::backScene $_9: __func::~__func (deleting)
static void t_73DD4C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73DD4C, "SceneMedalList::backScene $_9 __func::~__func (deleting)", t_73DD4C);

// SceneMedalList::backScene $_9: __func::__clone()
static uint32_t t_73DD54(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d97cu);
    }
    return v1;
}
RW_FUNCTION(0x73DD54, "SceneMedalList::backScene $_9 __func::__clone()", t_73DD54);

// SceneMedalList::backScene $_9: __func::__clone(__base*)
static void t_73DD78(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d97cu);
    }
}
RW_FUNCTION(0x73DD78, "SceneMedalList::backScene $_9 __func::__clone(__base*)", t_73DD78);

// SceneMedalList::backScene $_9: __func::destroy
static void t_73DD94(uint32_t self) {
}
RW_FUNCTION(0x73DD94, "SceneMedalList::backScene $_9 __func::destroy", t_73DD94);

// SceneMedalList::backScene $_9: __func::destroy_deallocate
static void t_73DD96(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73DD96, "SceneMedalList::backScene $_9 __func::destroy_deallocate", t_73DD96);

// SceneMedalList::backScene $_9: __func::target
static uint32_t t_73DEC0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9c6f0u ? self + 4 : 0; }
RW_FUNCTION(0x73DEC0, "SceneMedalList::backScene $_9 __func::target", t_73DEC0);

// SceneMedalList::backScene $_9: __func::target_type
static uint32_t t_73DEE0() { return 0xfa7804u; }
RW_FUNCTION(0x73DEE0, "SceneMedalList::backScene $_9 __func::target_type", t_73DEE0);

// SceneMedalList::onTapMedalInfoButton $_8: __func::~__func (deleting)
static void t_73DEF4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73DEF4, "SceneMedalList::onTapMedalInfoButton $_8 __func::~__func (deleting)", t_73DEF4);

// SceneMedalList::onTapMedalInfoButton $_8: __func::__clone()
static uint32_t t_73DEFC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d9a8u);
    }
    return v1;
}
RW_FUNCTION(0x73DEFC, "SceneMedalList::onTapMedalInfoButton $_8 __func::__clone()", t_73DEFC);

// SceneMedalList::onTapMedalInfoButton $_8: __func::__clone(__base*)
static void t_73DF20(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d9a8u);
    }
}
RW_FUNCTION(0x73DF20, "SceneMedalList::onTapMedalInfoButton $_8 __func::__clone(__base*)", t_73DF20);

// SceneMedalList::onTapMedalInfoButton $_8: __func::destroy
static void t_73DF3C(uint32_t self) {
}
RW_FUNCTION(0x73DF3C, "SceneMedalList::onTapMedalInfoButton $_8 __func::destroy", t_73DF3C);

// SceneMedalList::onTapMedalInfoButton $_8: __func::destroy_deallocate
static void t_73DF3E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73DF3E, "SceneMedalList::onTapMedalInfoButton $_8 __func::destroy_deallocate", t_73DF3E);

// SceneMedalList::onTapMedalInfoButton $_8: __func::target
static uint32_t t_73DFE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9c7b0u ? self + 4 : 0; }
RW_FUNCTION(0x73DFE4, "SceneMedalList::onTapMedalInfoButton $_8 __func::target", t_73DFE4);

// SceneMedalList::onTapMedalInfoButton $_8: __func::target_type
static uint32_t t_73E004() { return 0xfa7818u; }
RW_FUNCTION(0x73E004, "SceneMedalList::onTapMedalInfoButton $_8 __func::target_type", t_73E004);

// SceneMedalList::onTapMedalInfoButton $_8 UlvE_: __func::~__func (deleting)
static void t_73E018(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E018, "SceneMedalList::onTapMedalInfoButton $_8 UlvE_ __func::~__func (deleting)", t_73E018);

// SceneMedalList::onTapMedalInfoButton $_8 UlvE_: __func::__clone()
static uint32_t t_73E020(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6d9d4u);
    }
    return v1;
}
RW_FUNCTION(0x73E020, "SceneMedalList::onTapMedalInfoButton $_8 UlvE_ __func::__clone()", t_73E020);

// SceneMedalList::onTapMedalInfoButton $_8 UlvE_: __func::__clone(__base*)
static void t_73E044(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6d9d4u);
    }
}
RW_FUNCTION(0x73E044, "SceneMedalList::onTapMedalInfoButton $_8 UlvE_ __func::__clone(__base*)", t_73E044);

// SceneMedalList::onTapMedalInfoButton $_8 UlvE_: __func::destroy
static void t_73E060(uint32_t self) {
}
RW_FUNCTION(0x73E060, "SceneMedalList::onTapMedalInfoButton $_8 UlvE_ __func::destroy", t_73E060);

// SceneMedalList::onTapMedalInfoButton $_8 UlvE_: __func::destroy_deallocate
static void t_73E062(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E062, "SceneMedalList::onTapMedalInfoButton $_8 UlvE_ __func::destroy_deallocate", t_73E062);

// SceneMedalList::onTapMedalInfoButton $_8 UlvE_: __func::target
static uint32_t t_73E080(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9c8c0u ? self + 4 : 0; }
RW_FUNCTION(0x73E080, "SceneMedalList::onTapMedalInfoButton $_8 UlvE_ __func::target", t_73E080);

// SceneMedalList::onTapMedalInfoButton $_8 UlvE_: __func::target_type
static uint32_t t_73E0A0() { return 0xfa782cu; }
RW_FUNCTION(0x73E0A0, "SceneMedalList::onTapMedalInfoButton $_8 UlvE_ __func::target_type", t_73E0A0);

// SceneMedalList::onTapMedalInfoButton $_7: __func::~__func (deleting)
static void t_73E0B4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E0B4, "SceneMedalList::onTapMedalInfoButton $_7 __func::~__func (deleting)", t_73E0B4);

// SceneMedalList::onTapMedalInfoButton $_7: __func::__clone()
static uint32_t t_73E0BC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6da00u);
    }
    return v1;
}
RW_FUNCTION(0x73E0BC, "SceneMedalList::onTapMedalInfoButton $_7 __func::__clone()", t_73E0BC);

// SceneMedalList::onTapMedalInfoButton $_7: __func::__clone(__base*)
static void t_73E0E0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6da00u);
    }
}
RW_FUNCTION(0x73E0E0, "SceneMedalList::onTapMedalInfoButton $_7 __func::__clone(__base*)", t_73E0E0);

// SceneMedalList::onTapMedalInfoButton $_7: __func::destroy
static void t_73E0FC(uint32_t self) {
}
RW_FUNCTION(0x73E0FC, "SceneMedalList::onTapMedalInfoButton $_7 __func::destroy", t_73E0FC);

// SceneMedalList::onTapMedalInfoButton $_7: __func::destroy_deallocate
static void t_73E0FE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E0FE, "SceneMedalList::onTapMedalInfoButton $_7 __func::destroy_deallocate", t_73E0FE);

// SceneMedalList::onTapMedalInfoButton $_7: __func::target
static uint32_t t_73E218(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9c9b0u ? self + 4 : 0; }
RW_FUNCTION(0x73E218, "SceneMedalList::onTapMedalInfoButton $_7 __func::target", t_73E218);

// SceneMedalList::onTapMedalInfoButton $_7: __func::target_type
static uint32_t t_73E238() { return 0xfa7840u; }
RW_FUNCTION(0x73E238, "SceneMedalList::onTapMedalInfoButton $_7 __func::target_type", t_73E238);

// SceneMedalList::onTapMedalInfoButton $_7 UlvE_: __func::~__func (deleting)
static void t_73E24C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E24C, "SceneMedalList::onTapMedalInfoButton $_7 UlvE_ __func::~__func (deleting)", t_73E24C);

// SceneMedalList::onTapMedalInfoButton $_7 UlvE_: __func::__clone()
static uint32_t t_73E254(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6da2cu);
    }
    return v1;
}
RW_FUNCTION(0x73E254, "SceneMedalList::onTapMedalInfoButton $_7 UlvE_ __func::__clone()", t_73E254);

// SceneMedalList::onTapMedalInfoButton $_7 UlvE_: __func::__clone(__base*)
static void t_73E278(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6da2cu);
    }
}
RW_FUNCTION(0x73E278, "SceneMedalList::onTapMedalInfoButton $_7 UlvE_ __func::__clone(__base*)", t_73E278);

// SceneMedalList::onTapMedalInfoButton $_7 UlvE_: __func::destroy
static void t_73E294(uint32_t self) {
}
RW_FUNCTION(0x73E294, "SceneMedalList::onTapMedalInfoButton $_7 UlvE_ __func::destroy", t_73E294);

// SceneMedalList::onTapMedalInfoButton $_7 UlvE_: __func::destroy_deallocate
static void t_73E296(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E296, "SceneMedalList::onTapMedalInfoButton $_7 UlvE_ __func::destroy_deallocate", t_73E296);

// SceneMedalList::onTapMedalInfoButton $_7 UlvE_: __func::target
static uint32_t t_73E2B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9cac0u ? self + 4 : 0; }
RW_FUNCTION(0x73E2B4, "SceneMedalList::onTapMedalInfoButton $_7 UlvE_ __func::target", t_73E2B4);

// SceneMedalList::onTapMedalInfoButton $_7 UlvE_: __func::target_type
static uint32_t t_73E2D4() { return 0xfa7854u; }
RW_FUNCTION(0x73E2D4, "SceneMedalList::onTapMedalInfoButton $_7 UlvE_ __func::target_type", t_73E2D4);

// SceneMedalList::runOutAnimationMain $_6: __func::~__func
static uint32_t t_73E2E8(uint32_t self) {
    st32(self, 0xf6da58u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x73E2E8, "SceneMedalList::runOutAnimationMain $_6 __func::~__func", t_73E2E8);

// SceneMedalList::runOutAnimationMain $_6: __func::~__func (deleting)
static void t_73E324(uint32_t self) {
    st32(self, 0xf6da58u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E324, "SceneMedalList::runOutAnimationMain $_6 __func::~__func (deleting)", t_73E324);

// SceneMedalList::runOutAnimationMain $_6: __func::__clone()
static uint32_t t_73E364(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf6da58u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x73E364, "SceneMedalList::runOutAnimationMain $_6 __func::__clone()", t_73E364);

// SceneMedalList::runOutAnimationMain $_6: __func::__clone(__base*)
static void t_73E3B4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6da58u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x73E3B4, "SceneMedalList::runOutAnimationMain $_6 __func::__clone(__base*)", t_73E3B4);

// SceneMedalList::runOutAnimationMain $_6: __func::destroy
static void t_73E3E8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x73E3E8, "SceneMedalList::runOutAnimationMain $_6 __func::destroy", t_73E3E8);

// SceneMedalList::runOutAnimationMain $_6: __func::destroy_deallocate
static void t_73E408(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E408, "SceneMedalList::runOutAnimationMain $_6 __func::destroy_deallocate", t_73E408);

// SceneMedalList::runOutAnimationMain $_6: __func::target
static uint32_t t_73E440(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9cba0u ? self + 8 : 0; }
RW_FUNCTION(0x73E440, "SceneMedalList::runOutAnimationMain $_6 __func::target", t_73E440);

// SceneMedalList::runOutAnimationMain $_6: __func::target_type
static uint32_t t_73E460() { return 0xfa7868u; }
RW_FUNCTION(0x73E460, "SceneMedalList::runOutAnimationMain $_6 __func::target_type", t_73E460);

// SceneMedalList::init $_5: __func::~__func (deleting)
static void t_73E474(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E474, "SceneMedalList::init $_5 __func::~__func (deleting)", t_73E474);

// SceneMedalList::init $_5: __func::__clone()
static uint32_t t_73E47C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6da84u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x73E47C, "SceneMedalList::init $_5 __func::__clone()", t_73E47C);

// SceneMedalList::init $_5: __func::__clone(__base*)
static void t_73E4A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6da84u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x73E4A8, "SceneMedalList::init $_5 __func::__clone(__base*)", t_73E4A8);

// SceneMedalList::init $_5: __func::destroy
static void t_73E4C8(uint32_t self) {
}
RW_FUNCTION(0x73E4C8, "SceneMedalList::init $_5 __func::destroy", t_73E4C8);

// SceneMedalList::init $_5: __func::destroy_deallocate
static void t_73E4CA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E4CA, "SceneMedalList::init $_5 __func::destroy_deallocate", t_73E4CA);

// SceneMedalList::init $_5: __func::target
static uint32_t t_73E718(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9cc50u ? self + 4 : 0; }
RW_FUNCTION(0x73E718, "SceneMedalList::init $_5 __func::target", t_73E718);

// SceneMedalList::init $_5: __func::target_type
static uint32_t t_73E738() { return 0xfa787cu; }
RW_FUNCTION(0x73E738, "SceneMedalList::init $_5 __func::target_type", t_73E738);

// SceneMedalList::init $_5 UlvE_: __func::~__func (deleting)
static void t_73E74C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E74C, "SceneMedalList::init $_5 UlvE_ __func::~__func (deleting)", t_73E74C);

// SceneMedalList::init $_5 UlvE_: __func::__clone()
static uint32_t t_73E754(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6dab0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x73E754, "SceneMedalList::init $_5 UlvE_ __func::__clone()", t_73E754);

// SceneMedalList::init $_5 UlvE_: __func::__clone(__base*)
static void t_73E780(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6dab0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x73E780, "SceneMedalList::init $_5 UlvE_ __func::__clone(__base*)", t_73E780);

// SceneMedalList::init $_5 UlvE_: __func::destroy
static void t_73E7A0(uint32_t self) {
}
RW_FUNCTION(0x73E7A0, "SceneMedalList::init $_5 UlvE_ __func::destroy", t_73E7A0);

// SceneMedalList::init $_5 UlvE_: __func::destroy_deallocate
static void t_73E7A2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E7A2, "SceneMedalList::init $_5 UlvE_ __func::destroy_deallocate", t_73E7A2);

// SceneMedalList::init $_5 UlvE_: __func::target
static uint32_t t_73E7DC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe9ccd0u ? self + 4 : 0; }
RW_FUNCTION(0x73E7DC, "SceneMedalList::init $_5 UlvE_ __func::target", t_73E7DC);

// SceneMedalList::init $_5 UlvE_: __func::target_type
static uint32_t t_73E7FC() { return 0xfa7890u; }
RW_FUNCTION(0x73E7FC, "SceneMedalList::init $_5 UlvE_ __func::target_type", t_73E7FC);

// SceneMedalList::init $_5: __func::~__func (deleting)
static void t_73E810(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x73E810, "SceneMedalList::init $_5 __func::~__func (deleting)", t_73E810);

// SceneMedalList::init $_5: __func::__clone()
static uint32_t t_73E818(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf6dadcu);
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
RW_FUNCTION(0x73E818, "SceneMedalList::init $_5 __func::__clone()", t_73E818);

// SceneMedalList::init $_5: __func::__clone(__base*)
static void t_73E84C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6dadcu);
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
RW_FUNCTION(0x73E84C, "SceneMedalList::init $_5 __func::__clone(__base*)", t_73E84C);
