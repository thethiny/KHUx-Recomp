// The std::function __func thunks of the lambdas attributed to MapPrizes (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// MapPrizes::createArmature $_1: __func::~__func (deleting)
static void t_5DEC78(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5DEC78, "MapPrizes::createArmature $_1 __func::~__func (deleting)", t_5DEC78);

// MapPrizes::createArmature $_1: __func::__clone()
static uint32_t t_5DEC80(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d9acu);
    }
    return v1;
}
RW_FUNCTION(0x5DEC80, "MapPrizes::createArmature $_1 __func::__clone()", t_5DEC80);

// MapPrizes::createArmature $_1: __func::__clone(__base*)
static void t_5DECA4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d9acu);
    }
}
RW_FUNCTION(0x5DECA4, "MapPrizes::createArmature $_1 __func::__clone(__base*)", t_5DECA4);

// MapPrizes::createArmature $_1: __func::destroy
static void t_5DECC0(uint32_t self) {
}
RW_FUNCTION(0x5DECC0, "MapPrizes::createArmature $_1 __func::destroy", t_5DECC0);

// MapPrizes::createArmature $_1: __func::destroy_deallocate
static void t_5DECC2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5DECC2, "MapPrizes::createArmature $_1 __func::destroy_deallocate", t_5DECC2);

// MapPrizes::createArmature $_1: __func::target
static uint32_t t_5DEDA4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4cde0u ? self + 4 : 0; }
RW_FUNCTION(0x5DEDA4, "MapPrizes::createArmature $_1 __func::target", t_5DEDA4);
