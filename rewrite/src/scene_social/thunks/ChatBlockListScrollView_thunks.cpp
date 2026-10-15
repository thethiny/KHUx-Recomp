// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ChatBlockListScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ChatBlockListScrollView::changeRow $_2: __func::~__func
static uint32_t t_55CF6C(uint32_t self) {
    st32(self, 0xf58bf8u);
    uint32_t v1 = ld8(self + 24);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 32);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x55CF6C, "ChatBlockListScrollView::changeRow $_2 __func::~__func", t_55CF6C);

// ChatBlockListScrollView::changeRow $_2: __func::~__func (deleting)
static void t_55CF9C(uint32_t self) {
    st32(self, 0xf58bf8u);
    uint32_t v1 = ld8(self + 24);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 32);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x55CF9C, "ChatBlockListScrollView::changeRow $_2 __func::~__func (deleting)", t_55CF9C);

// ChatBlockListScrollView::changeRow $_2: __func::destroy
static void t_55D0B4(uint32_t self) {
    uint32_t v1 = ld8(self + 24);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 32);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x55D0B4, "ChatBlockListScrollView::changeRow $_2 __func::destroy", t_55D0B4);

// ChatBlockListScrollView::changeRow $_2: __func::destroy_deallocate
static void t_55D0C6(uint32_t self) {
    uint32_t v1 = ld8(self + 24);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 32);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x55D0C6, "ChatBlockListScrollView::changeRow $_2 __func::destroy_deallocate", t_55D0C6);
