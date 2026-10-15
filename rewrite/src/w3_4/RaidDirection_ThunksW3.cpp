// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the RaidDirection lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// RaidDirection::raid_Cover_sub2 $_10: __func::~__func (deleting)
static void t_5F95E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F95E8, "RaidDirection::raid_Cover_sub2 $_10 __func::~__func (deleting)", t_5F95E8);

// RaidDirection::raid_Cover_sub2 $_10: __func::__clone()
static uint32_t t_5F95F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5debcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5F95F0, "RaidDirection::raid_Cover_sub2 $_10 __func::__clone()", t_5F95F0);

// RaidDirection::raid_Cover_sub2 $_10: __func::__clone(__base*)
static void t_5F961C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5debcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5F961C, "RaidDirection::raid_Cover_sub2 $_10 __func::__clone(__base*)", t_5F961C);

// RaidDirection::raid_Cover_sub2 $_10: __func::destroy
static void t_5F963C(uint32_t self) {
}
RW_FUNCTION(0x5F963C, "RaidDirection::raid_Cover_sub2 $_10 __func::destroy", t_5F963C);

// RaidDirection::raid_Cover_sub2 $_10: __func::destroy_deallocate
static void t_5F963E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F963E, "RaidDirection::raid_Cover_sub2 $_10 __func::destroy_deallocate", t_5F963E);

// RaidDirection::raid_Cover_sub2 $_10: __func::target
static uint32_t t_5F9654(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4e5e0u ? self + 4 : 0; }
RW_FUNCTION(0x5F9654, "RaidDirection::raid_Cover_sub2 $_10 __func::target", t_5F9654);

// RaidDirection::raid_Cover_sub2 $_10: __func::target_type
static uint32_t t_5F9674() { return 0xf96734u; }
RW_FUNCTION(0x5F9674, "RaidDirection::raid_Cover_sub2 $_10 __func::target_type", t_5F9674);

// RaidDirection::raid_Cover_sub2 $_9: __func::~__func (deleting)
static void t_5F9688(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F9688, "RaidDirection::raid_Cover_sub2 $_9 __func::~__func (deleting)", t_5F9688);

// RaidDirection::raid_Cover_sub2 $_9: __func::__clone()
static uint32_t t_5F9690(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5dee8u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5F9690, "RaidDirection::raid_Cover_sub2 $_9 __func::__clone()", t_5F9690);

// RaidDirection::raid_Cover_sub2 $_9: __func::__clone(__base*)
static void t_5F96C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5dee8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5F96C0, "RaidDirection::raid_Cover_sub2 $_9 __func::__clone(__base*)", t_5F96C0);

// RaidDirection::raid_Cover_sub2 $_9: __func::destroy
static void t_5F96E4(uint32_t self) {
}
RW_FUNCTION(0x5F96E4, "RaidDirection::raid_Cover_sub2 $_9 __func::destroy", t_5F96E4);

// RaidDirection::raid_Cover_sub2 $_9: __func::destroy_deallocate
static void t_5F96E6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F96E6, "RaidDirection::raid_Cover_sub2 $_9 __func::destroy_deallocate", t_5F96E6);

// RaidDirection::raid_Cover_sub2 $_9: __func::target
static uint32_t t_5F97CC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4e670u ? self + 4 : 0; }
RW_FUNCTION(0x5F97CC, "RaidDirection::raid_Cover_sub2 $_9 __func::target", t_5F97CC);

// RaidDirection::raid_Cover_sub2 $_9: __func::target_type
static uint32_t t_5F97EC() { return 0xf96748u; }
RW_FUNCTION(0x5F97EC, "RaidDirection::raid_Cover_sub2 $_9 __func::target_type", t_5F97EC);

// RaidDirection::raid_Cover_sub2 $_8: __func::~__func (deleting)
static void t_5F9800(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F9800, "RaidDirection::raid_Cover_sub2 $_8 __func::~__func (deleting)", t_5F9800);

// RaidDirection::raid_Cover_sub2 $_8: __func::__clone()
static uint32_t t_5F9808(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5df14u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5F9808, "RaidDirection::raid_Cover_sub2 $_8 __func::__clone()", t_5F9808);

// RaidDirection::raid_Cover_sub2 $_8: __func::__clone(__base*)
static void t_5F9838(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5df14u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5F9838, "RaidDirection::raid_Cover_sub2 $_8 __func::__clone(__base*)", t_5F9838);

// RaidDirection::raid_Cover_sub2 $_8: __func::destroy
static void t_5F985C(uint32_t self) {
}
RW_FUNCTION(0x5F985C, "RaidDirection::raid_Cover_sub2 $_8 __func::destroy", t_5F985C);

// RaidDirection::raid_Cover_sub2 $_8: __func::destroy_deallocate
static void t_5F985E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F985E, "RaidDirection::raid_Cover_sub2 $_8 __func::destroy_deallocate", t_5F985E);

// RaidDirection::raid_Cover_sub2 $_8: __func::target
static uint32_t t_5F9888(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4e700u ? self + 4 : 0; }
RW_FUNCTION(0x5F9888, "RaidDirection::raid_Cover_sub2 $_8 __func::target", t_5F9888);

// RaidDirection::raid_Cover_sub2 $_8: __func::target_type
static uint32_t t_5F98A8() { return 0xf9675cu; }
RW_FUNCTION(0x5F98A8, "RaidDirection::raid_Cover_sub2 $_8 __func::target_type", t_5F98A8);

// RaidDirection::raid_Cover_sub1 $_7: __func::~__func (deleting)
static void t_5F98BC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F98BC, "RaidDirection::raid_Cover_sub1 $_7 __func::~__func (deleting)", t_5F98BC);

// RaidDirection::raid_Cover_sub1 $_7: __func::__clone()
static uint32_t t_5F98C4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5df40u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5F98C4, "RaidDirection::raid_Cover_sub1 $_7 __func::__clone()", t_5F98C4);

// RaidDirection::raid_Cover_sub1 $_7: __func::__clone(__base*)
static void t_5F98F0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5df40u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5F98F0, "RaidDirection::raid_Cover_sub1 $_7 __func::__clone(__base*)", t_5F98F0);

// RaidDirection::raid_Cover_sub1 $_7: __func::destroy
static void t_5F9910(uint32_t self) {
}
RW_FUNCTION(0x5F9910, "RaidDirection::raid_Cover_sub1 $_7 __func::destroy", t_5F9910);

// RaidDirection::raid_Cover_sub1 $_7: __func::destroy_deallocate
static void t_5F9912(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F9912, "RaidDirection::raid_Cover_sub1 $_7 __func::destroy_deallocate", t_5F9912);

// RaidDirection::raid_Cover_sub1 $_7: __func::target
static uint32_t t_5F9928(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4e790u ? self + 4 : 0; }
RW_FUNCTION(0x5F9928, "RaidDirection::raid_Cover_sub1 $_7 __func::target", t_5F9928);

// RaidDirection::raid_Cover_sub1 $_7: __func::target_type
static uint32_t t_5F9948() { return 0xf96770u; }
RW_FUNCTION(0x5F9948, "RaidDirection::raid_Cover_sub1 $_7 __func::target_type", t_5F9948);

// RaidDirection::raid_Cover_sub1 $_6: __func::~__func (deleting)
static void t_5F995C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F995C, "RaidDirection::raid_Cover_sub1 $_6 __func::~__func (deleting)", t_5F995C);

// RaidDirection::raid_Cover_sub1 $_6: __func::__clone()
static uint32_t t_5F9964(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5df6cu);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5F9964, "RaidDirection::raid_Cover_sub1 $_6 __func::__clone()", t_5F9964);

// RaidDirection::raid_Cover_sub1 $_6: __func::__clone(__base*)
static void t_5F9994(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5df6cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5F9994, "RaidDirection::raid_Cover_sub1 $_6 __func::__clone(__base*)", t_5F9994);

// RaidDirection::raid_Cover_sub1 $_6: __func::destroy
static void t_5F99B8(uint32_t self) {
}
RW_FUNCTION(0x5F99B8, "RaidDirection::raid_Cover_sub1 $_6 __func::destroy", t_5F99B8);

// RaidDirection::raid_Cover_sub1 $_6: __func::destroy_deallocate
static void t_5F99BA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F99BA, "RaidDirection::raid_Cover_sub1 $_6 __func::destroy_deallocate", t_5F99BA);
