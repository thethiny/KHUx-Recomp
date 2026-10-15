// The std::function __func thunks of the lambdas attributed to RaidBattle (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// RaidBattle::RaidBossEndSequence $_2: __func::~__func (deleting)
static void t_5F12D2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F12D2, "RaidBattle::RaidBossEndSequence $_2 __func::~__func (deleting)", t_5F12D2);

// RaidBattle::RaidBossEndSequence $_2: __func::__clone()
static uint32_t t_5F12DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5de0cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5F12DC, "RaidBattle::RaidBossEndSequence $_2 __func::__clone()", t_5F12DC);

// RaidBattle::RaidBossEndSequence $_2: __func::__clone(__base*)
static void t_5F1308(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5de0cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5F1308, "RaidBattle::RaidBossEndSequence $_2 __func::__clone(__base*)", t_5F1308);

// RaidBattle::RaidBossEndSequence $_2: __func::destroy
static void t_5F1328(uint32_t self) {
}
RW_FUNCTION(0x5F1328, "RaidBattle::RaidBossEndSequence $_2 __func::destroy", t_5F1328);

// RaidBattle::RaidBossEndSequence $_2: __func::destroy_deallocate
static void t_5F132A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F132A, "RaidBattle::RaidBossEndSequence $_2 __func::destroy_deallocate", t_5F132A);

// RaidBattle::RaidBossEndSequence $_2: __func::target
static uint32_t t_5F134C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4e0d0u ? self + 4 : 0; }
RW_FUNCTION(0x5F134C, "RaidBattle::RaidBossEndSequence $_2 __func::target", t_5F134C);

// RaidBattle::RaidBossEndSequence $_2: __func::target_type
static uint32_t t_5F136C() { return 0xf960c4u; }
RW_FUNCTION(0x5F136C, "RaidBattle::RaidBossEndSequence $_2 __func::target_type", t_5F136C);

// RaidBattle::go_back $_1: __func::~__func (deleting)
static void t_5F1380(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5F1380, "RaidBattle::go_back $_1 __func::~__func (deleting)", t_5F1380);

// RaidBattle::go_back $_1: __func::__clone()
static uint32_t t_5F1388(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5de38u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5F1388, "RaidBattle::go_back $_1 __func::__clone()", t_5F1388);
