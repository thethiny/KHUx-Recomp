// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ChatStampListScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ChatStampListScrollView::changeRow $_3: __func::~__func (deleting)
static void t_55CA5C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x55CA5C, "ChatStampListScrollView::changeRow $_3 __func::~__func (deleting)", t_55CA5C);

// ChatStampListScrollView::changeRow $_3: __func::__clone()
static uint32_t t_55CA64(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf58bccu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x55CA64, "ChatStampListScrollView::changeRow $_3 __func::__clone()", t_55CA64);

// ChatStampListScrollView::changeRow $_3: __func::__clone(__base*)
static void t_55CA94(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf58bccu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x55CA94, "ChatStampListScrollView::changeRow $_3 __func::__clone(__base*)", t_55CA94);

// ChatStampListScrollView::changeRow $_3: __func::destroy
static void t_55CABC(uint32_t self) {
}
RW_FUNCTION(0x55CABC, "ChatStampListScrollView::changeRow $_3 __func::destroy", t_55CABC);

// ChatStampListScrollView::changeRow $_3: __func::destroy_deallocate
static void t_55CABE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x55CABE, "ChatStampListScrollView::changeRow $_3 __func::destroy_deallocate", t_55CABE);

// ChatStampListScrollView::changeRow $_3: __func::target
static uint32_t t_55CD2C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe30e50u ? self + 4 : 0; }
RW_FUNCTION(0x55CD2C, "ChatStampListScrollView::changeRow $_3 __func::target", t_55CD2C);

// ChatStampListScrollView::changeRow $_3: __func::target_type
static uint32_t t_55CD4C() { return 0xf89a8cu; }
RW_FUNCTION(0x55CD4C, "ChatStampListScrollView::changeRow $_3 __func::target_type", t_55CD4C);
