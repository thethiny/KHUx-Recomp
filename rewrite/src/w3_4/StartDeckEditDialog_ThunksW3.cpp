// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the StartDeckEditDialog lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// StartDeckEditDialog::startRaid $_25: __func::target
static uint32_t t_57A700(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe356b0u ? self + 4 : 0; }
RW_FUNCTION(0x57A700, "StartDeckEditDialog::startRaid $_25 __func::target", t_57A700);

// StartDeckEditDialog::startRaid $_25: __func::target_type
static uint32_t t_57A720() { return 0xf8b008u; }
RW_FUNCTION(0x57A720, "StartDeckEditDialog::startRaid $_25 __func::target_type", t_57A720);

// StartDeckEditDialog::startStory $_24: __func::~__func (deleting)
static void t_57A7C0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57A7C0, "StartDeckEditDialog::startStory $_24 __func::~__func (deleting)", t_57A7C0);

// StartDeckEditDialog::startStory $_24: __func::__clone()
static uint32_t t_57A7C8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59858u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x57A7C8, "StartDeckEditDialog::startStory $_24 __func::__clone()", t_57A7C8);

// StartDeckEditDialog::startStory $_24: __func::__clone(__base*)
static void t_57A7F4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59858u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x57A7F4, "StartDeckEditDialog::startStory $_24 __func::__clone(__base*)", t_57A7F4);

// StartDeckEditDialog::startStory $_24: __func::destroy
static void t_57A814(uint32_t self) {
}
RW_FUNCTION(0x57A814, "StartDeckEditDialog::startStory $_24 __func::destroy", t_57A814);

// StartDeckEditDialog::startStory $_24: __func::destroy_deallocate
static void t_57A816(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57A816, "StartDeckEditDialog::startStory $_24 __func::destroy_deallocate", t_57A816);

// StartDeckEditDialog::startStory $_24: __func::target
static uint32_t t_57A82C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35750u ? self + 4 : 0; }
RW_FUNCTION(0x57A82C, "StartDeckEditDialog::startStory $_24 __func::target", t_57A82C);

// StartDeckEditDialog::startStory $_24: __func::target_type
static uint32_t t_57A84C() { return 0xf8b01cu; }
RW_FUNCTION(0x57A84C, "StartDeckEditDialog::startStory $_24 __func::target_type", t_57A84C);

// StartDeckEditDialog::startStory $_23: __func::~__func (deleting)
static void t_57A860(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57A860, "StartDeckEditDialog::startStory $_23 __func::~__func (deleting)", t_57A860);

// StartDeckEditDialog::startStory $_23: __func::__clone()
static uint32_t t_57A868(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59884u);
    }
    return v1;
}
RW_FUNCTION(0x57A868, "StartDeckEditDialog::startStory $_23 __func::__clone()", t_57A868);

// StartDeckEditDialog::startStory $_23: __func::__clone(__base*)
static void t_57A88C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59884u);
    }
}
RW_FUNCTION(0x57A88C, "StartDeckEditDialog::startStory $_23 __func::__clone(__base*)", t_57A88C);

// StartDeckEditDialog::startStory $_23: __func::destroy
static void t_57A8A8(uint32_t self) {
}
RW_FUNCTION(0x57A8A8, "StartDeckEditDialog::startStory $_23 __func::destroy", t_57A8A8);

// StartDeckEditDialog::startStory $_23: __func::destroy_deallocate
static void t_57A8AA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x57A8AA, "StartDeckEditDialog::startStory $_23 __func::destroy_deallocate", t_57A8AA);

// StartDeckEditDialog::startStory $_23: __func::target
static uint32_t t_57A9B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe357e0u ? self + 4 : 0; }
RW_FUNCTION(0x57A9B0, "StartDeckEditDialog::startStory $_23 __func::target", t_57A9B0);

// StartDeckEditDialog::startStory $_23: __func::target_type
static uint32_t t_57A9D0() { return 0xf8b030u; }
RW_FUNCTION(0x57A9D0, "StartDeckEditDialog::startStory $_23 __func::target_type", t_57A9D0);

// StartDeckEditDialog::supportScrollRunOutAnimation $_22: __func::target
static uint32_t t_57AB18(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe35890u ? self + 4 : 0; }
RW_FUNCTION(0x57AB18, "StartDeckEditDialog::supportScrollRunOutAnimation $_22 __func::target", t_57AB18);

// StartDeckEditDialog::supportScrollRunOutAnimation $_22: __func::target_type
static uint32_t t_57AB38() { return 0xf8b044u; }
RW_FUNCTION(0x57AB38, "StartDeckEditDialog::supportScrollRunOutAnimation $_22 __func::target_type", t_57AB38);
