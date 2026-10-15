// Generated from the lifted code by gen_thunks.py (scratch tool): the std::function __func thunks of the
// ColosseumScrollView lambdas - clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// ColosseumScrollView::registItemTouchEvent $_3: __func::~__func (deleting)
static void t_54EA64(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54EA64, "ColosseumScrollView::registItemTouchEvent $_3 __func::~__func (deleting)", t_54EA64);

// ColosseumScrollView::registItemTouchEvent $_3: __func::__clone()
static uint32_t t_54EA6C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf58990u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 16, v5);
        st32(v1 + 12, v4);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x54EA6C, "ColosseumScrollView::registItemTouchEvent $_3 __func::__clone()", t_54EA6C);

// ColosseumScrollView::registItemTouchEvent $_3: __func::__clone(__base*)
static void t_54EAA0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf58990u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x54EAA0, "ColosseumScrollView::registItemTouchEvent $_3 __func::__clone(__base*)", t_54EAA0);

// ColosseumScrollView::registItemTouchEvent $_3: __func::destroy
static void t_54EACC(uint32_t self) {
}
RW_FUNCTION(0x54EACC, "ColosseumScrollView::registItemTouchEvent $_3 __func::destroy", t_54EACC);

// ColosseumScrollView::registItemTouchEvent $_3: __func::destroy_deallocate
static void t_54EACE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54EACE, "ColosseumScrollView::registItemTouchEvent $_3 __func::destroy_deallocate", t_54EACE);

// ColosseumScrollView::registItemTouchEvent $_3: __func::target
static uint32_t t_54EAE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe30030u ? self + 4 : 0; }
RW_FUNCTION(0x54EAE4, "ColosseumScrollView::registItemTouchEvent $_3 __func::target", t_54EAE4);

// ColosseumScrollView::registItemTouchEvent $_3: __func::target_type
static uint32_t t_54EB04() { return 0xf87964u; }
RW_FUNCTION(0x54EB04, "ColosseumScrollView::registItemTouchEvent $_3 __func::target_type", t_54EB04);

// ColosseumScrollView::registItemTouchEvent $_4: __func::~__func (deleting)
static void t_54ECD8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54ECD8, "ColosseumScrollView::registItemTouchEvent $_4 __func::~__func (deleting)", t_54ECD8);

// ColosseumScrollView::registItemTouchEvent $_4: __func::__clone()
static uint32_t t_54ECE0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf589bcu);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x54ECE0, "ColosseumScrollView::registItemTouchEvent $_4 __func::__clone()", t_54ECE0);

// ColosseumScrollView::registItemTouchEvent $_4: __func::__clone(__base*)
static void t_54ED10(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf589bcu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x54ED10, "ColosseumScrollView::registItemTouchEvent $_4 __func::__clone(__base*)", t_54ED10);

// ColosseumScrollView::registItemTouchEvent $_4: __func::destroy
static void t_54ED34(uint32_t self) {
}
RW_FUNCTION(0x54ED34, "ColosseumScrollView::registItemTouchEvent $_4 __func::destroy", t_54ED34);

// ColosseumScrollView::registItemTouchEvent $_4: __func::destroy_deallocate
static void t_54ED36(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54ED36, "ColosseumScrollView::registItemTouchEvent $_4 __func::destroy_deallocate", t_54ED36);

// ColosseumScrollView::registItemTouchEvent $_4: __func::target
static uint32_t t_54EDA4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe30100u ? self + 4 : 0; }
RW_FUNCTION(0x54EDA4, "ColosseumScrollView::registItemTouchEvent $_4 __func::target", t_54EDA4);

// ColosseumScrollView::registItemTouchEvent $_4: __func::target_type
static uint32_t t_54EDC4() { return 0xf87978u; }
RW_FUNCTION(0x54EDC4, "ColosseumScrollView::registItemTouchEvent $_4 __func::target_type", t_54EDC4);

// ColosseumScrollView::registItemTouchEvent $_2: __func::~__func (deleting)
static void t_54EDD8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54EDD8, "ColosseumScrollView::registItemTouchEvent $_2 __func::~__func (deleting)", t_54EDD8);

// ColosseumScrollView::registItemTouchEvent $_2: __func::__clone()
static uint32_t t_54EDE0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf589e8u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x54EDE0, "ColosseumScrollView::registItemTouchEvent $_2 __func::__clone()", t_54EDE0);

// ColosseumScrollView::registItemTouchEvent $_2: __func::__clone(__base*)
static void t_54EE18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf589e8u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        uint32_t v6 = ld32(self + 24);
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x54EE18, "ColosseumScrollView::registItemTouchEvent $_2 __func::__clone(__base*)", t_54EE18);

// ColosseumScrollView::registItemTouchEvent $_2: __func::destroy
static void t_54EE44(uint32_t self) {
}
RW_FUNCTION(0x54EE44, "ColosseumScrollView::registItemTouchEvent $_2 __func::destroy", t_54EE44);

// ColosseumScrollView::registItemTouchEvent $_2: __func::destroy_deallocate
static void t_54EE46(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54EE46, "ColosseumScrollView::registItemTouchEvent $_2 __func::destroy_deallocate", t_54EE46);

// ColosseumScrollView::registItemTouchEvent $_2: __func::target
static uint32_t t_54EF58(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe301f0u ? self + 4 : 0; }
RW_FUNCTION(0x54EF58, "ColosseumScrollView::registItemTouchEvent $_2 __func::target", t_54EF58);

// ColosseumScrollView::registItemTouchEvent $_2: __func::target_type
static uint32_t t_54EF78() { return 0xf8798cu; }
RW_FUNCTION(0x54EF78, "ColosseumScrollView::registItemTouchEvent $_2 __func::target_type", t_54EF78);

// ColosseumScrollView::registItemTouchEvent $_1: __func::~__func (deleting)
static void t_54EF8C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54EF8C, "ColosseumScrollView::registItemTouchEvent $_1 __func::~__func (deleting)", t_54EF8C);

// ColosseumScrollView::registItemTouchEvent $_1: __func::__clone()
static uint32_t t_54EF94(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf58a14u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x54EF94, "ColosseumScrollView::registItemTouchEvent $_1 __func::__clone()", t_54EF94);

// ColosseumScrollView::registItemTouchEvent $_1: __func::__clone(__base*)
static void t_54EFC4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf58a14u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x54EFC4, "ColosseumScrollView::registItemTouchEvent $_1 __func::__clone(__base*)", t_54EFC4);

// ColosseumScrollView::registItemTouchEvent $_1: __func::destroy
static void t_54EFE8(uint32_t self) {
}
RW_FUNCTION(0x54EFE8, "ColosseumScrollView::registItemTouchEvent $_1 __func::destroy", t_54EFE8);

// ColosseumScrollView::registItemTouchEvent $_1: __func::destroy_deallocate
static void t_54EFEA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54EFEA, "ColosseumScrollView::registItemTouchEvent $_1 __func::destroy_deallocate", t_54EFEA);

// ColosseumScrollView::registItemTouchEvent $_1: __func::target
static uint32_t t_54F124(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe302e0u ? self + 4 : 0; }
RW_FUNCTION(0x54F124, "ColosseumScrollView::registItemTouchEvent $_1 __func::target", t_54F124);

// ColosseumScrollView::registItemTouchEvent $_1: __func::target_type
static uint32_t t_54F144() { return 0xf879a0u; }
RW_FUNCTION(0x54F144, "ColosseumScrollView::registItemTouchEvent $_1 __func::target_type", t_54F144);

// ColosseumScrollView::changeRow $_0: __func::~__func (deleting)
static void t_54F158(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54F158, "ColosseumScrollView::changeRow $_0 __func::~__func (deleting)", t_54F158);

// ColosseumScrollView::changeRow $_0: __func::__clone()
static uint32_t t_54F160(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf58a40u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x54F160, "ColosseumScrollView::changeRow $_0 __func::__clone()", t_54F160);

// ColosseumScrollView::changeRow $_0: __func::__clone(__base*)
static void t_54F190(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf58a40u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x54F190, "ColosseumScrollView::changeRow $_0 __func::__clone(__base*)", t_54F190);

// ColosseumScrollView::changeRow $_0: __func::destroy
static void t_54F1B8(uint32_t self) {
}
RW_FUNCTION(0x54F1B8, "ColosseumScrollView::changeRow $_0 __func::destroy", t_54F1B8);

// ColosseumScrollView::changeRow $_0: __func::destroy_deallocate
static void t_54F1BA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x54F1BA, "ColosseumScrollView::changeRow $_0 __func::destroy_deallocate", t_54F1BA);

// ColosseumScrollView::changeRow $_0: __func::target
static uint32_t t_54F204(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe303d0u ? self + 4 : 0; }
RW_FUNCTION(0x54F204, "ColosseumScrollView::changeRow $_0 __func::target", t_54F204);

// ColosseumScrollView::changeRow $_0: __func::target_type
static uint32_t t_54F224() { return 0xf879b4u; }
RW_FUNCTION(0x54F224, "ColosseumScrollView::changeRow $_0 __func::target_type", t_54F224);
