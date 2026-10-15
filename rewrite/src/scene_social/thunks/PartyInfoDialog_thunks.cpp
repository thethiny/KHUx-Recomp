// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// PartyInfoDialog lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// PartyInfoDialog::close $_15: __func::~__func (deleting)
static void t_5961E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5961E8, "PartyInfoDialog::close $_15 __func::~__func (deleting)", t_5961E8);

// PartyInfoDialog::close $_15: __func::__clone()
static uint32_t t_5961F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ac74u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5961F0, "PartyInfoDialog::close $_15 __func::__clone()", t_5961F0);

// PartyInfoDialog::close $_15: __func::__clone(__base*)
static void t_59621C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ac74u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x59621C, "PartyInfoDialog::close $_15 __func::__clone(__base*)", t_59621C);

// PartyInfoDialog::close $_15: __func::destroy
static void t_59623C(uint32_t self) {
}
RW_FUNCTION(0x59623C, "PartyInfoDialog::close $_15 __func::destroy", t_59623C);

// PartyInfoDialog::close $_15: __func::destroy_deallocate
static void t_59623E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x59623E, "PartyInfoDialog::close $_15 __func::destroy_deallocate", t_59623E);

// PartyInfoDialog::close $_15: __func::target
static uint32_t t_59629C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3e9f0u ? self + 4 : 0; }
RW_FUNCTION(0x59629C, "PartyInfoDialog::close $_15 __func::target", t_59629C);

// PartyInfoDialog::close $_15: __func::target_type
static uint32_t t_5962BC() { return 0xf8c224u; }
RW_FUNCTION(0x5962BC, "PartyInfoDialog::close $_15 __func::target_type", t_5962BC);

// PartyInfoDialog::createAlertLayout $_14: __func::~__func (deleting)
static void t_5962D0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5962D0, "PartyInfoDialog::createAlertLayout $_14 __func::~__func (deleting)", t_5962D0);

// PartyInfoDialog::createAlertLayout $_14: __func::__clone()
static uint32_t t_5962D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5aca0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5962D8, "PartyInfoDialog::createAlertLayout $_14 __func::__clone()", t_5962D8);

// PartyInfoDialog::createAlertLayout $_14: __func::__clone(__base*)
static void t_596304(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5aca0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x596304, "PartyInfoDialog::createAlertLayout $_14 __func::__clone(__base*)", t_596304);

// PartyInfoDialog::createAlertLayout $_14: __func::destroy
static void t_596324(uint32_t self) {
}
RW_FUNCTION(0x596324, "PartyInfoDialog::createAlertLayout $_14 __func::destroy", t_596324);

// PartyInfoDialog::createAlertLayout $_14: __func::destroy_deallocate
static void t_596326(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596326, "PartyInfoDialog::createAlertLayout $_14 __func::destroy_deallocate", t_596326);

// PartyInfoDialog::createAlertLayout $_14: __func::target
static uint32_t t_59634C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3ea90u ? self + 4 : 0; }
RW_FUNCTION(0x59634C, "PartyInfoDialog::createAlertLayout $_14 __func::target", t_59634C);

// PartyInfoDialog::createAlertLayout $_14: __func::target_type
static uint32_t t_59636C() { return 0xf8c238u; }
RW_FUNCTION(0x59636C, "PartyInfoDialog::createAlertLayout $_14 __func::target_type", t_59636C);

// PartyInfoDialog::createAlertLayout $_13: __func::~__func (deleting)
static void t_596380(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596380, "PartyInfoDialog::createAlertLayout $_13 __func::~__func (deleting)", t_596380);

// PartyInfoDialog::createAlertLayout $_13: __func::__clone()
static uint32_t t_596388(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5acccu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x596388, "PartyInfoDialog::createAlertLayout $_13 __func::__clone()", t_596388);

// PartyInfoDialog::createAlertLayout $_13: __func::__clone(__base*)
static void t_5963B4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5acccu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5963B4, "PartyInfoDialog::createAlertLayout $_13 __func::__clone(__base*)", t_5963B4);

// PartyInfoDialog::createAlertLayout $_13: __func::destroy
static void t_5963D4(uint32_t self) {
}
RW_FUNCTION(0x5963D4, "PartyInfoDialog::createAlertLayout $_13 __func::destroy", t_5963D4);

// PartyInfoDialog::createAlertLayout $_13: __func::destroy_deallocate
static void t_5963D6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5963D6, "PartyInfoDialog::createAlertLayout $_13 __func::destroy_deallocate", t_5963D6);

// PartyInfoDialog::createAlertLayout $_13: __func::target
static uint32_t t_5963E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3eb30u ? self + 4 : 0; }
RW_FUNCTION(0x5963E8, "PartyInfoDialog::createAlertLayout $_13 __func::target", t_5963E8);

// PartyInfoDialog::createAlertLayout $_13: __func::target_type
static uint32_t t_596408() { return 0xf8c24cu; }
RW_FUNCTION(0x596408, "PartyInfoDialog::createAlertLayout $_13 __func::target_type", t_596408);

// PartyInfoDialog::createAlertLayout $_12: __func::~__func (deleting)
static void t_59641C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x59641C, "PartyInfoDialog::createAlertLayout $_12 __func::~__func (deleting)", t_59641C);

// PartyInfoDialog::createAlertLayout $_12: __func::__clone()
static uint32_t t_596424(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5acf8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x596424, "PartyInfoDialog::createAlertLayout $_12 __func::__clone()", t_596424);

// PartyInfoDialog::createAlertLayout $_12: __func::__clone(__base*)
static void t_596450(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5acf8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x596450, "PartyInfoDialog::createAlertLayout $_12 __func::__clone(__base*)", t_596450);

// PartyInfoDialog::createAlertLayout $_12: __func::destroy
static void t_596470(uint32_t self) {
}
RW_FUNCTION(0x596470, "PartyInfoDialog::createAlertLayout $_12 __func::destroy", t_596470);

// PartyInfoDialog::createAlertLayout $_12: __func::destroy_deallocate
static void t_596472(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596472, "PartyInfoDialog::createAlertLayout $_12 __func::destroy_deallocate", t_596472);

// PartyInfoDialog::createAlertLayout $_12: __func::target
static uint32_t t_5964A8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3ebf0u ? self + 4 : 0; }
RW_FUNCTION(0x5964A8, "PartyInfoDialog::createAlertLayout $_12 __func::target", t_5964A8);

// PartyInfoDialog::createAlertLayout $_12: __func::target_type
static uint32_t t_5964C8() { return 0xf8c260u; }
RW_FUNCTION(0x5964C8, "PartyInfoDialog::createAlertLayout $_12 __func::target_type", t_5964C8);

// PartyInfoDialog::createAlertLayout $_11: __func::~__func (deleting)
static void t_5964DC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5964DC, "PartyInfoDialog::createAlertLayout $_11 __func::~__func (deleting)", t_5964DC);

// PartyInfoDialog::createAlertLayout $_11: __func::__clone()
static uint32_t t_5964E4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ad24u);
    }
    return v1;
}
RW_FUNCTION(0x5964E4, "PartyInfoDialog::createAlertLayout $_11 __func::__clone()", t_5964E4);

// PartyInfoDialog::createAlertLayout $_11: __func::__clone(__base*)
static void t_596508(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ad24u);
    }
}
RW_FUNCTION(0x596508, "PartyInfoDialog::createAlertLayout $_11 __func::__clone(__base*)", t_596508);

// PartyInfoDialog::createAlertLayout $_11: __func::destroy
static void t_596524(uint32_t self) {
}
RW_FUNCTION(0x596524, "PartyInfoDialog::createAlertLayout $_11 __func::destroy", t_596524);

// PartyInfoDialog::createAlertLayout $_11: __func::destroy_deallocate
static void t_596526(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596526, "PartyInfoDialog::createAlertLayout $_11 __func::destroy_deallocate", t_596526);

// PartyInfoDialog::createAlertLayout $_11: __func::target
static uint32_t t_596530(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3ec90u ? self + 4 : 0; }
RW_FUNCTION(0x596530, "PartyInfoDialog::createAlertLayout $_11 __func::target", t_596530);

// PartyInfoDialog::createAlertLayout $_11: __func::target_type
static uint32_t t_596550() { return 0xf8c274u; }
RW_FUNCTION(0x596550, "PartyInfoDialog::createAlertLayout $_11 __func::target_type", t_596550);

// PartyInfoDialog::createRequestLayout $_10: __func::~__func (deleting)
static void t_596564(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596564, "PartyInfoDialog::createRequestLayout $_10 __func::~__func (deleting)", t_596564);

// PartyInfoDialog::createRequestLayout $_10: __func::__clone()
static uint32_t t_59656C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5ad50u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x59656C, "PartyInfoDialog::createRequestLayout $_10 __func::__clone()", t_59656C);

// PartyInfoDialog::createRequestLayout $_10: __func::__clone(__base*)
static void t_59659C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ad50u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x59659C, "PartyInfoDialog::createRequestLayout $_10 __func::__clone(__base*)", t_59659C);

// PartyInfoDialog::createRequestLayout $_10: __func::destroy
static void t_5965C0(uint32_t self) {
}
RW_FUNCTION(0x5965C0, "PartyInfoDialog::createRequestLayout $_10 __func::destroy", t_5965C0);

// PartyInfoDialog::createRequestLayout $_10: __func::destroy_deallocate
static void t_5965C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5965C2, "PartyInfoDialog::createRequestLayout $_10 __func::destroy_deallocate", t_5965C2);

// PartyInfoDialog::createRequestLayout $_10: __func::target
static uint32_t t_59661C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3ed30u ? self + 4 : 0; }
RW_FUNCTION(0x59661C, "PartyInfoDialog::createRequestLayout $_10 __func::target", t_59661C);

// PartyInfoDialog::createRequestLayout $_10: __func::target_type
static uint32_t t_59663C() { return 0xf8c288u; }
RW_FUNCTION(0x59663C, "PartyInfoDialog::createRequestLayout $_10 __func::target_type", t_59663C);

// PartyInfoDialog::createRequestLayout $_9: __func::~__func (deleting)
static void t_596650(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596650, "PartyInfoDialog::createRequestLayout $_9 __func::~__func (deleting)", t_596650);

// PartyInfoDialog::createRequestLayout $_9: __func::__clone()
static uint32_t t_596658(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ad7cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x596658, "PartyInfoDialog::createRequestLayout $_9 __func::__clone()", t_596658);

// PartyInfoDialog::createRequestLayout $_9: __func::__clone(__base*)
static void t_596684(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ad7cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x596684, "PartyInfoDialog::createRequestLayout $_9 __func::__clone(__base*)", t_596684);

// PartyInfoDialog::createRequestLayout $_9: __func::destroy
static void t_5966A4(uint32_t self) {
}
RW_FUNCTION(0x5966A4, "PartyInfoDialog::createRequestLayout $_9 __func::destroy", t_5966A4);

// PartyInfoDialog::createRequestLayout $_9: __func::destroy_deallocate
static void t_5966A6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5966A6, "PartyInfoDialog::createRequestLayout $_9 __func::destroy_deallocate", t_5966A6);

// PartyInfoDialog::createRequestLayout $_9: __func::target
static uint32_t t_5966B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3ede0u ? self + 4 : 0; }
RW_FUNCTION(0x5966B8, "PartyInfoDialog::createRequestLayout $_9 __func::target", t_5966B8);

// PartyInfoDialog::createRequestLayout $_9: __func::target_type
static uint32_t t_5966D8() { return 0xf8c29cu; }
RW_FUNCTION(0x5966D8, "PartyInfoDialog::createRequestLayout $_9 __func::target_type", t_5966D8);

// PartyInfoDialog::createRequestLayout $_8: __func::~__func (deleting)
static void t_5966EC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5966EC, "PartyInfoDialog::createRequestLayout $_8 __func::~__func (deleting)", t_5966EC);

// PartyInfoDialog::createRequestLayout $_8: __func::__clone()
static uint32_t t_5966F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ada8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5966F4, "PartyInfoDialog::createRequestLayout $_8 __func::__clone()", t_5966F4);

// PartyInfoDialog::createRequestLayout $_8: __func::__clone(__base*)
static void t_596720(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ada8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x596720, "PartyInfoDialog::createRequestLayout $_8 __func::__clone(__base*)", t_596720);

// PartyInfoDialog::createRequestLayout $_8: __func::destroy
static void t_596740(uint32_t self) {
}
RW_FUNCTION(0x596740, "PartyInfoDialog::createRequestLayout $_8 __func::destroy", t_596740);

// PartyInfoDialog::createRequestLayout $_8: __func::destroy_deallocate
static void t_596742(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596742, "PartyInfoDialog::createRequestLayout $_8 __func::destroy_deallocate", t_596742);

// PartyInfoDialog::createRequestLayout $_8: __func::target
static uint32_t t_59684C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3eeb0u ? self + 4 : 0; }
RW_FUNCTION(0x59684C, "PartyInfoDialog::createRequestLayout $_8 __func::target", t_59684C);

// PartyInfoDialog::createRequestLayout $_8: __func::target_type
static uint32_t t_59686C() { return 0xf8c2b0u; }
RW_FUNCTION(0x59686C, "PartyInfoDialog::createRequestLayout $_8 __func::target_type", t_59686C);

// PartyInfoDialog::createRequestLayout $_8 UlbE_: __func::~__func (deleting)
static void t_596880(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596880, "PartyInfoDialog::createRequestLayout $_8 UlbE_ __func::~__func (deleting)", t_596880);

// PartyInfoDialog::createRequestLayout $_8 UlbE_: __func::__clone()
static uint32_t t_596888(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5add4u);
    }
    return v1;
}
RW_FUNCTION(0x596888, "PartyInfoDialog::createRequestLayout $_8 UlbE_ __func::__clone()", t_596888);

// PartyInfoDialog::createRequestLayout $_8 UlbE_: __func::__clone(__base*)
static void t_5968AC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5add4u);
    }
}
RW_FUNCTION(0x5968AC, "PartyInfoDialog::createRequestLayout $_8 UlbE_ __func::__clone(__base*)", t_5968AC);

// PartyInfoDialog::createRequestLayout $_8 UlbE_: __func::destroy
static void t_5968C8(uint32_t self) {
}
RW_FUNCTION(0x5968C8, "PartyInfoDialog::createRequestLayout $_8 UlbE_ __func::destroy", t_5968C8);

// PartyInfoDialog::createRequestLayout $_8 UlbE_: __func::destroy_deallocate
static void t_5968CA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5968CA, "PartyInfoDialog::createRequestLayout $_8 UlbE_ __func::destroy_deallocate", t_5968CA);

// PartyInfoDialog::createRequestLayout $_8 UlbE_: __func::target
static uint32_t t_596994(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3ef80u ? self + 4 : 0; }
RW_FUNCTION(0x596994, "PartyInfoDialog::createRequestLayout $_8 UlbE_ __func::target", t_596994);

// PartyInfoDialog::createRequestLayout $_8 UlbE_: __func::target_type
static uint32_t t_5969B4() { return 0xf8c2c4u; }
RW_FUNCTION(0x5969B4, "PartyInfoDialog::createRequestLayout $_8 UlbE_ __func::target_type", t_5969B4);

// PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_: __func::~__func (deleting)
static void t_5969C8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5969C8, "PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_ __func::~__func (deleting)", t_5969C8);

// PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_: __func::__clone()
static uint32_t t_5969D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5ae00u);
    }
    return v1;
}
RW_FUNCTION(0x5969D0, "PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_ __func::__clone()", t_5969D0);

// PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_: __func::__clone(__base*)
static void t_5969F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5ae00u);
    }
}
RW_FUNCTION(0x5969F4, "PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_ __func::__clone(__base*)", t_5969F4);

// PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_: __func::destroy
static void t_596A10(uint32_t self) {
}
RW_FUNCTION(0x596A10, "PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_ __func::destroy", t_596A10);

// PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_: __func::destroy_deallocate
static void t_596A12(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596A12, "PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_ __func::destroy_deallocate", t_596A12);

// PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_: __func::target
static uint32_t t_596A28(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3f0a0u ? self + 4 : 0; }
RW_FUNCTION(0x596A28, "PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_ __func::target", t_596A28);

// PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_: __func::target_type
static uint32_t t_596A48() { return 0xf8c2d8u; }
RW_FUNCTION(0x596A48, "PartyInfoDialog::createRequestLayout $_8 UlbE_ UlbE_ __func::target_type", t_596A48);

// PartyInfoDialog::createRequestLayout $_7: __func::~__func (deleting)
static void t_596A5C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x596A5C, "PartyInfoDialog::createRequestLayout $_7 __func::~__func (deleting)", t_596A5C);

// PartyInfoDialog::createRequestLayout $_7: __func::__clone()
static uint32_t t_596A64(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5ae2cu);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x596A64, "PartyInfoDialog::createRequestLayout $_7 __func::__clone()", t_596A64);
