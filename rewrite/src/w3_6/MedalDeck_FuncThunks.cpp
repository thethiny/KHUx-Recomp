// The std::function __func thunks of the MedalDeck lambdas (generated from the lifted code by a scratch tool and
// checked against it): clone (copy the captures into a new / given block), destroy (destroy the captured
// objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "Common.hpp"

using namespace w36;

// MedalDeck::addOntoDeck $_0: __func::~__func (deleting)
static void t_5E132C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5E132C, "MedalDeck::addOntoDeck $_0 __func::~__func (deleting)", t_5E132C);

// MedalDeck::addOntoDeck $_0: __func::__clone()
static uint32_t t_5E1334(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5daa4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5E1334, "MedalDeck::addOntoDeck $_0 __func::__clone()", t_5E1334);

// MedalDeck::addOntoDeck $_0: __func::__clone(__base*)
static void t_5E1360(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5daa4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5E1360, "MedalDeck::addOntoDeck $_0 __func::__clone(__base*)", t_5E1360);

// MedalDeck::addOntoDeck $_0: __func::destroy
static void t_5E1380(uint32_t self) {
}
RW_FUNCTION(0x5E1380, "MedalDeck::addOntoDeck $_0 __func::destroy", t_5E1380);

// MedalDeck::addOntoDeck $_0: __func::destroy_deallocate
static void t_5E1382(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5E1382, "MedalDeck::addOntoDeck $_0 __func::destroy_deallocate", t_5E1382);

// MedalDeck::addOntoDeck $_0: __func::target
static uint32_t t_5E13FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4d280u ? self + 4 : 0; }
RW_FUNCTION(0x5E13FC, "MedalDeck::addOntoDeck $_0 __func::target", t_5E13FC);

// MedalDeck::addOntoDeck $_0: __func::target_type
static uint32_t t_5E141C() { return 0xf93738u; }
RW_FUNCTION(0x5E141C, "MedalDeck::addOntoDeck $_0 __func::target_type", t_5E141C);
