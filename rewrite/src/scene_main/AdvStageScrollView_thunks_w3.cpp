// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// AdvStageScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// AdvStageScrollView::settingAvatarFrame $_17: __func::~__func
static uint32_t t_572408(uint32_t self) {
    st32(self, 0xf593b4u);
    uint32_t v1 = ld8(self + 12);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 20);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x572408, "AdvStageScrollView::settingAvatarFrame $_17 __func::~__func", t_572408);

// AdvStageScrollView::settingAvatarFrame $_17: __func::~__func (deleting)
static void t_572438(uint32_t self) {
    st32(self, 0xf593b4u);
    uint32_t v1 = ld8(self + 12);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 20);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x572438, "AdvStageScrollView::settingAvatarFrame $_17 __func::~__func (deleting)", t_572438);

// AdvStageScrollView::settingAvatarFrame $_17: __func::__clone(__base*)
static void t_5724D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf593b4u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
        uint32_t v3 = ld8(self + 12);
        if (v3 & 1) {
            uint32_t v4 = ld32(self + 16);
            uint32_t v5 = ld32(self + 20);
            call(0x47cd93, {p + 12, v5, v4});   // sub_47CD92
            return;
        }
        uint32_t v6 = ld32(self + 12 + 0);
        uint32_t v7 = ld32(self + 12 + 4);
        uint32_t v8 = ld32(self + 12 + 8);
        st32(p + 12 + 0, v6);
        st32(p + 12 + 4, v7);
        st32(p + 12 + 8, v8);
        return;
    }
}
RW_FUNCTION(0x5724D8, "AdvStageScrollView::settingAvatarFrame $_17 __func::__clone(__base*)", t_5724D8);

// AdvStageScrollView::settingAvatarFrame $_17: __func::destroy
static void t_57252C(uint32_t self) {
    uint32_t v1 = ld8(self + 12);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 20);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x57252C, "AdvStageScrollView::settingAvatarFrame $_17 __func::destroy", t_57252C);

// AdvStageScrollView::settingAvatarFrame $_17: __func::destroy_deallocate
static void t_57253E(uint32_t self) {
    uint32_t v1 = ld8(self + 12);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 20);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57253E, "AdvStageScrollView::settingAvatarFrame $_17 __func::destroy_deallocate", t_57253E);

// AdvStageScrollView::settingAvatarFrame $_17: __func::target
static uint32_t t_572594(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe33d30u ? self + 4 : 0; }
RW_FUNCTION(0x572594, "AdvStageScrollView::settingAvatarFrame $_17 __func::target", t_572594);

// AdvStageScrollView::settingAvatarFrame $_17: __func::target_type
static uint32_t t_5725B4() { return 0xf8ab74u; }
RW_FUNCTION(0x5725B4, "AdvStageScrollView::settingAvatarFrame $_17 __func::target_type", t_5725B4);

// AdvStageScrollView::settingAvatarFrame $_17: __func::__clone() (written by hand) - captures: 2 words at +4,
// std::string at +12
static uint32_t t_57246C(uint32_t self) {
    uint32_t obj = call(fn::operator_new, {0x18});
    if (obj) {
        st32(obj, 0xf593b4u);
        uint32_t w0 = ld32(self + 4), w1 = ld32(self + 8);
        st32(obj + 8, w1);
        st32(obj + 4, w0);
        if (ld8(self + 12) & 1) {
            call(0x47cd93, {obj + 12, ld32(self + 20), ld32(self + 16)});   // std::string::__init(data, size)
        } else {
            uint32_t a = ld32(self + 12), b = ld32(self + 16), c = ld32(self + 20);
            st32(obj + 12, a);
            st32(obj + 16, b);
            st32(obj + 20, c);
        }
    }
    return obj;
}
RW_FUNCTION(0x57246C, "AdvStageScrollView::settingAvatarFrame $_17 __func::__clone()", t_57246C);
