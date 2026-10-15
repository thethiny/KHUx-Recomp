// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// SceneChat lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneChat::runOutAnimation $_2: __func::~__func
static uint32_t t_67D5A4(uint32_t self) {
    st32(self, 0xf63f70u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x67D5A4, "SceneChat::runOutAnimation $_2 __func::~__func", t_67D5A4);

// SceneChat::runOutAnimation $_2: __func::~__func (deleting)
static void t_67D5E0(uint32_t self) {
    st32(self, 0xf63f70u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67D5E0, "SceneChat::runOutAnimation $_2 __func::~__func (deleting)", t_67D5E0);

// SceneChat::runOutAnimation $_2: __func::__clone()
static uint32_t t_67D620(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf63f70u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x67D620, "SceneChat::runOutAnimation $_2 __func::__clone()", t_67D620);

// SceneChat::runOutAnimation $_2: __func::__clone(__base*)
static void t_67D670(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63f70u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x67D670, "SceneChat::runOutAnimation $_2 __func::__clone(__base*)", t_67D670);

// SceneChat::runOutAnimation $_2: __func::destroy
static void t_67D6A4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x67D6A4, "SceneChat::runOutAnimation $_2 __func::destroy", t_67D6A4);

// SceneChat::runOutAnimation $_2: __func::destroy_deallocate
static void t_67D6C4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67D6C4, "SceneChat::runOutAnimation $_2 __func::destroy_deallocate", t_67D6C4);

// SceneChat::runOutAnimation $_2: __func::target
static uint32_t t_67D6FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6e3d0u ? self + 8 : 0; }
RW_FUNCTION(0x67D6FC, "SceneChat::runOutAnimation $_2 __func::target", t_67D6FC);

// SceneChat::runOutAnimation $_2: __func::target_type
static uint32_t t_67D71C() { return 0xf9b7b8u; }
RW_FUNCTION(0x67D71C, "SceneChat::runOutAnimation $_2 __func::target_type", t_67D71C);

// SceneChat::init $_1: __func::~__func (deleting)
static void t_67D730(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67D730, "SceneChat::init $_1 __func::~__func (deleting)", t_67D730);

// SceneChat::init $_1: __func::__clone()
static uint32_t t_67D738(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf63f9cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x67D738, "SceneChat::init $_1 __func::__clone()", t_67D738);

// SceneChat::init $_1: __func::__clone(__base*)
static void t_67D764(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63f9cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x67D764, "SceneChat::init $_1 __func::__clone(__base*)", t_67D764);

// SceneChat::init $_1: __func::destroy
static void t_67D784(uint32_t self) {
}
RW_FUNCTION(0x67D784, "SceneChat::init $_1 __func::destroy", t_67D784);

// SceneChat::init $_1: __func::destroy_deallocate
static void t_67D786(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67D786, "SceneChat::init $_1 __func::destroy_deallocate", t_67D786);

// SceneChat::init $_1: __func::target
static uint32_t t_67D798(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6e460u ? self + 4 : 0; }
RW_FUNCTION(0x67D798, "SceneChat::init $_1 __func::target", t_67D798);

// SceneChat::init $_1: __func::target_type
static uint32_t t_67D7B8() { return 0xf9b7ccu; }
RW_FUNCTION(0x67D7B8, "SceneChat::init $_1 __func::target_type", t_67D7B8);

// SceneChat::init $_0: __func::~__func (deleting)
static void t_67D7CC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67D7CC, "SceneChat::init $_0 __func::~__func (deleting)", t_67D7CC);

// SceneChat::init $_0: __func::__clone()
static uint32_t t_67D7D4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf63fc8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x67D7D4, "SceneChat::init $_0 __func::__clone()", t_67D7D4);

// SceneChat::init $_0: __func::__clone(__base*)
static void t_67D800(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf63fc8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x67D800, "SceneChat::init $_0 __func::__clone(__base*)", t_67D800);

// SceneChat::init $_0: __func::destroy
static void t_67D820(uint32_t self) {
}
RW_FUNCTION(0x67D820, "SceneChat::init $_0 __func::destroy", t_67D820);

// SceneChat::init $_0: __func::destroy_deallocate
static void t_67D822(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x67D822, "SceneChat::init $_0 __func::destroy_deallocate", t_67D822);

// SceneChat::init $_0: __func::target
static uint32_t t_67D834(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6e4d0u ? self + 4 : 0; }
RW_FUNCTION(0x67D834, "SceneChat::init $_0 __func::target", t_67D834);

// SceneChat::init $_0: __func::target_type
static uint32_t t_67D854() { return 0xf9b7e0u; }
RW_FUNCTION(0x67D854, "SceneChat::init $_0 __func::target_type", t_67D854);
