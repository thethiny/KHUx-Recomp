// The std::function __func thunks of the ShowEffect lambdas (generated from the lifted code by a scratch tool and
// checked against it): clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "Common.hpp"

using namespace w36;

// ShowEffect $_4: __func::~__func (deleting)
static void t_5D8D20(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8D20, "ShowEffect $_4 __func::~__func (deleting)", t_5D8D20);

// ShowEffect $_4: __func::__clone()
static uint32_t t_5D8D28(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d76cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5D8D28, "ShowEffect $_4 __func::__clone()", t_5D8D28);

// ShowEffect $_4: __func::__clone(__base*)
static void t_5D8D54(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d76cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5D8D54, "ShowEffect $_4 __func::__clone(__base*)", t_5D8D54);

// ShowEffect $_4: __func::destroy
static void t_5D8D74(uint32_t self) {
}
RW_FUNCTION(0x5D8D74, "ShowEffect $_4 __func::destroy", t_5D8D74);

// ShowEffect $_4: __func::destroy_deallocate
static void t_5D8D76(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8D76, "ShowEffect $_4 __func::destroy_deallocate", t_5D8D76);

// ShowEffect $_4: __func::target
static uint32_t t_5D8D88(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4c450u ? self + 4 : 0; }
RW_FUNCTION(0x5D8D88, "ShowEffect $_4 __func::target", t_5D8D88);

// ShowEffect $_4: __func::target_type
static uint32_t t_5D8DA8() { return 0xf919b8u; }
RW_FUNCTION(0x5D8DA8, "ShowEffect $_4 __func::target_type", t_5D8DA8);

// ShowEffect $_3: __func::~__func (deleting)
static void t_5D8DBC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8DBC, "ShowEffect $_3 __func::~__func (deleting)", t_5D8DBC);

// ShowEffect $_3: __func::__clone()
static uint32_t t_5D8DC4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d798u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5D8DC4, "ShowEffect $_3 __func::__clone()", t_5D8DC4);

// ShowEffect $_3: __func::__clone(__base*)
static void t_5D8DF0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d798u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5D8DF0, "ShowEffect $_3 __func::__clone(__base*)", t_5D8DF0);

// ShowEffect $_3: __func::destroy
static void t_5D8E10(uint32_t self) {
}
RW_FUNCTION(0x5D8E10, "ShowEffect $_3 __func::destroy", t_5D8E10);

// ShowEffect $_3: __func::destroy_deallocate
static void t_5D8E12(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8E12, "ShowEffect $_3 __func::destroy_deallocate", t_5D8E12);

// ShowEffect $_3: __func::target
static uint32_t t_5D8E24(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4c4c0u ? self + 4 : 0; }
RW_FUNCTION(0x5D8E24, "ShowEffect $_3 __func::target", t_5D8E24);

// ShowEffect $_3: __func::target_type
static uint32_t t_5D8E44() { return 0xf919ccu; }
RW_FUNCTION(0x5D8E44, "ShowEffect $_3 __func::target_type", t_5D8E44);

// ShowEffect $_2: __func::~__func (deleting)
static void t_5D8E58(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8E58, "ShowEffect $_2 __func::~__func (deleting)", t_5D8E58);

// ShowEffect $_2: __func::__clone()
static uint32_t t_5D8E60(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d7c4u);
    }
    return v1;
}
RW_FUNCTION(0x5D8E60, "ShowEffect $_2 __func::__clone()", t_5D8E60);

// ShowEffect $_2: __func::__clone(__base*)
static void t_5D8E84(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d7c4u);
    }
}
RW_FUNCTION(0x5D8E84, "ShowEffect $_2 __func::__clone(__base*)", t_5D8E84);

// ShowEffect $_2: __func::destroy
static void t_5D8EA0(uint32_t self) {
}
RW_FUNCTION(0x5D8EA0, "ShowEffect $_2 __func::destroy", t_5D8EA0);

// ShowEffect $_2: __func::destroy_deallocate
static void t_5D8EA2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8EA2, "ShowEffect $_2 __func::destroy_deallocate", t_5D8EA2);

// ShowEffect $_2: __func::target
static uint32_t t_5D8EB8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4c540u ? self + 4 : 0; }
RW_FUNCTION(0x5D8EB8, "ShowEffect $_2 __func::target", t_5D8EB8);

// ShowEffect $_2: __func::target_type
static uint32_t t_5D8ED8() { return 0xf919e0u; }
RW_FUNCTION(0x5D8ED8, "ShowEffect $_2 __func::target_type", t_5D8ED8);

// ShowEffect $_1: __func::~__func (deleting)
static void t_5D8EEC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8EEC, "ShowEffect $_1 __func::~__func (deleting)", t_5D8EEC);

// ShowEffect $_1: __func::__clone()
static uint32_t t_5D8EF4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d7f0u);
    }
    return v1;
}
RW_FUNCTION(0x5D8EF4, "ShowEffect $_1 __func::__clone()", t_5D8EF4);

// ShowEffect $_1: __func::__clone(__base*)
static void t_5D8F18(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d7f0u);
    }
}
RW_FUNCTION(0x5D8F18, "ShowEffect $_1 __func::__clone(__base*)", t_5D8F18);

// ShowEffect $_1: __func::destroy
static void t_5D8F34(uint32_t self) {
}
RW_FUNCTION(0x5D8F34, "ShowEffect $_1 __func::destroy", t_5D8F34);

// ShowEffect $_1: __func::destroy_deallocate
static void t_5D8F36(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D8F36, "ShowEffect $_1 __func::destroy_deallocate", t_5D8F36);

// ShowEffect $_1: __func::target
static uint32_t t_5D8F4C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4c5c0u ? self + 4 : 0; }
RW_FUNCTION(0x5D8F4C, "ShowEffect $_1 __func::target", t_5D8F4C);
