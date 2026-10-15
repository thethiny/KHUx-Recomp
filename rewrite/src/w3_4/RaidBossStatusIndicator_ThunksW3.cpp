// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the RaidBossStatusIndicator lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// RaidBossStatusIndicator::init $_6: __func::target
static uint32_t t_5EA864(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4d770u ? self + 4 : 0; }
RW_FUNCTION(0x5EA864, "RaidBossStatusIndicator::init $_6 __func::target", t_5EA864);

// RaidBossStatusIndicator::init $_6: __func::target_type
static uint32_t t_5EA884() { return 0xf949a4u; }
RW_FUNCTION(0x5EA884, "RaidBossStatusIndicator::init $_6 __func::target_type", t_5EA884);
