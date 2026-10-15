// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the FlashDrama lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// FlashDrama::runSpeech $_5: __func::~__func (deleting)
static void t_6F441E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F441E, "FlashDrama::runSpeech $_5 __func::~__func (deleting)", t_6F441E);

// FlashDrama::runSpeech $_5: __func::__clone()
static uint32_t t_6F4428(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf69dd8u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F4428, "FlashDrama::runSpeech $_5 __func::__clone()", t_6F4428);

// FlashDrama::runSpeech $_5: __func::__clone(__base*)
static void t_6F4458(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69dd8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F4458, "FlashDrama::runSpeech $_5 __func::__clone(__base*)", t_6F4458);

// FlashDrama::runSpeech $_5: __func::destroy
static void t_6F447C(uint32_t self) {
}
RW_FUNCTION(0x6F447C, "FlashDrama::runSpeech $_5 __func::destroy", t_6F447C);

// FlashDrama::runSpeech $_5: __func::destroy_deallocate
static void t_6F447E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F447E, "FlashDrama::runSpeech $_5 __func::destroy_deallocate", t_6F447E);

// FlashDrama::runSpeech $_5: __func::target
static uint32_t t_6F44D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe89ee0u ? self + 4 : 0; }
RW_FUNCTION(0x6F44D8, "FlashDrama::runSpeech $_5 __func::target", t_6F44D8);

// FlashDrama::runSpeech $_5: __func::target_type
static uint32_t t_6F44F8() { return 0xfa292cu; }
RW_FUNCTION(0x6F44F8, "FlashDrama::runSpeech $_5 __func::target_type", t_6F44F8);

// FlashDrama::addEventListeners $_4: __func::~__func
static uint32_t t_6F45F4(uint32_t self) {
    return self;
}
RW_FUNCTION(0x6F45F4, "FlashDrama::addEventListeners $_4 __func::~__func", t_6F45F4);

// FlashDrama::addEventListeners $_4: __func::~__func (deleting)
static void t_6F45F6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F45F6, "FlashDrama::addEventListeners $_4 __func::~__func (deleting)", t_6F45F6);

// FlashDrama::addEventListeners $_4: __func::__clone()
static uint32_t t_6F4600(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69e04u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F4600, "FlashDrama::addEventListeners $_4 __func::__clone()", t_6F4600);

// FlashDrama::addEventListeners $_4: __func::__clone(__base*)
static void t_6F462C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69e04u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F462C, "FlashDrama::addEventListeners $_4 __func::__clone(__base*)", t_6F462C);

// FlashDrama::addEventListeners $_4: __func::destroy
static void t_6F464C(uint32_t self) {
}
RW_FUNCTION(0x6F464C, "FlashDrama::addEventListeners $_4 __func::destroy", t_6F464C);

// FlashDrama::addEventListeners $_4: __func::destroy_deallocate
static void t_6F464E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F464E, "FlashDrama::addEventListeners $_4 __func::destroy_deallocate", t_6F464E);

// FlashDrama::addEventListeners $_4: __func::target
static uint32_t t_6F4774(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe89fc0u ? self + 4 : 0; }
RW_FUNCTION(0x6F4774, "FlashDrama::addEventListeners $_4 __func::target", t_6F4774);

// FlashDrama::addEventListeners $_4: __func::target_type
static uint32_t t_6F4794() { return 0xfa2940u; }
RW_FUNCTION(0x6F4794, "FlashDrama::addEventListeners $_4 __func::target_type", t_6F4794);

// FlashDrama::addEventListeners $_3: __func::~__func (deleting)
static void t_6F4E4E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F4E4E, "FlashDrama::addEventListeners $_3 __func::~__func (deleting)", t_6F4E4E);

// FlashDrama::addEventListeners $_3: __func::__clone()
static uint32_t t_6F4E58(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69e30u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F4E58, "FlashDrama::addEventListeners $_3 __func::__clone()", t_6F4E58);

// FlashDrama::addEventListeners $_3: __func::__clone(__base*)
static void t_6F4E84(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69e30u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F4E84, "FlashDrama::addEventListeners $_3 __func::__clone(__base*)", t_6F4E84);

// FlashDrama::addEventListeners $_3: __func::destroy
static void t_6F4EA4(uint32_t self) {
}
RW_FUNCTION(0x6F4EA4, "FlashDrama::addEventListeners $_3 __func::destroy", t_6F4EA4);

// FlashDrama::addEventListeners $_3: __func::destroy_deallocate
static void t_6F4EA6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F4EA6, "FlashDrama::addEventListeners $_3 __func::destroy_deallocate", t_6F4EA6);

// FlashDrama::addEventListeners $_3: __func::target
static uint32_t t_6F4EC0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8a060u ? self + 4 : 0; }
RW_FUNCTION(0x6F4EC0, "FlashDrama::addEventListeners $_3 __func::target", t_6F4EC0);

// FlashDrama::addEventListeners $_3: __func::target_type
static uint32_t t_6F4EE0() { return 0xfa2954u; }
RW_FUNCTION(0x6F4EE0, "FlashDrama::addEventListeners $_3 __func::target_type", t_6F4EE0);

// FlashDrama::addEventListeners $_2: __func::~__func (deleting)
static void t_6F4EF4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F4EF4, "FlashDrama::addEventListeners $_2 __func::~__func (deleting)", t_6F4EF4);

// FlashDrama::addEventListeners $_2: __func::__clone()
static uint32_t t_6F4EFC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf69e5cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6F4EFC, "FlashDrama::addEventListeners $_2 __func::__clone()", t_6F4EFC);

// FlashDrama::addEventListeners $_2: __func::__clone(__base*)
static void t_6F4F28(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf69e5cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6F4F28, "FlashDrama::addEventListeners $_2 __func::__clone(__base*)", t_6F4F28);

// FlashDrama::addEventListeners $_2: __func::destroy
static void t_6F4F48(uint32_t self) {
}
RW_FUNCTION(0x6F4F48, "FlashDrama::addEventListeners $_2 __func::destroy", t_6F4F48);

// FlashDrama::addEventListeners $_2: __func::destroy_deallocate
static void t_6F4F4A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6F4F4A, "FlashDrama::addEventListeners $_2 __func::destroy_deallocate", t_6F4F4A);

// FlashDrama::addEventListeners $_2: __func::target
static uint32_t t_6F4F60(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe8a100u ? self + 4 : 0; }
RW_FUNCTION(0x6F4F60, "FlashDrama::addEventListeners $_2 __func::target", t_6F4F60);

// FlashDrama::addEventListeners $_2: __func::target_type
static uint32_t t_6F4F80() { return 0xfa2968u; }
RW_FUNCTION(0x6F4F80, "FlashDrama::addEventListeners $_2 __func::target_type", t_6F4F80);
