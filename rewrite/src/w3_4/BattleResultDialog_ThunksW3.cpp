// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the BattleResultDialog lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// BattleResultDialog::initSubMissionLwf $_21: __func::destroy
static void t_584154(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x584154, "BattleResultDialog::initSubMissionLwf $_21 __func::destroy", t_584154);

// BattleResultDialog::initSubMissionLwf $_21: __func::destroy_deallocate
static void t_584174(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584174, "BattleResultDialog::initSubMissionLwf $_21 __func::destroy_deallocate", t_584174);

// BattleResultDialog::initSubMissionLwf $_21: __func::target
static uint32_t t_5841DC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37320u ? self + 8 : 0; }
RW_FUNCTION(0x5841DC, "BattleResultDialog::initSubMissionLwf $_21 __func::target", t_5841DC);

// BattleResultDialog::initSubMissionLwf $_21: __func::target_type
static uint32_t t_584200() { return 0xf8b594u; }
RW_FUNCTION(0x584200, "BattleResultDialog::initSubMissionLwf $_21 __func::target_type", t_584200);

// BattleResultDialog::close $_20: __func::~__func (deleting)
static void t_584214(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584214, "BattleResultDialog::close $_20 __func::~__func (deleting)", t_584214);

// BattleResultDialog::close $_20: __func::__clone()
static uint32_t t_58421C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59eb4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x58421C, "BattleResultDialog::close $_20 __func::__clone()", t_58421C);

// BattleResultDialog::close $_20: __func::__clone(__base*)
static void t_584248(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59eb4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584248, "BattleResultDialog::close $_20 __func::__clone(__base*)", t_584248);

// BattleResultDialog::close $_20: __func::destroy
static void t_584268(uint32_t self) {
}
RW_FUNCTION(0x584268, "BattleResultDialog::close $_20 __func::destroy", t_584268);

// BattleResultDialog::close $_20: __func::destroy_deallocate
static void t_58426A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58426A, "BattleResultDialog::close $_20 __func::destroy_deallocate", t_58426A);

// BattleResultDialog::close $_20: __func::target
static uint32_t t_5842C0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe373d0u ? self + 4 : 0; }
RW_FUNCTION(0x5842C0, "BattleResultDialog::close $_20 __func::target", t_5842C0);

// BattleResultDialog::close $_20: __func::target_type
static uint32_t t_5842E0() { return 0xf8b5a8u; }
RW_FUNCTION(0x5842E0, "BattleResultDialog::close $_20 __func::target_type", t_5842E0);

// BattleResultDialog::openClearRank $_19: __func::~__func (deleting)
static void t_5842F4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5842F4, "BattleResultDialog::openClearRank $_19 __func::~__func (deleting)", t_5842F4);

// BattleResultDialog::openClearRank $_19: __func::__clone()
static uint32_t t_5842FC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59ee0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5842FC, "BattleResultDialog::openClearRank $_19 __func::__clone()", t_5842FC);

// BattleResultDialog::openClearRank $_19: __func::__clone(__base*)
static void t_584328(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59ee0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584328, "BattleResultDialog::openClearRank $_19 __func::__clone(__base*)", t_584328);

// BattleResultDialog::openClearRank $_19: __func::destroy
static void t_584348(uint32_t self) {
}
RW_FUNCTION(0x584348, "BattleResultDialog::openClearRank $_19 __func::destroy", t_584348);

// BattleResultDialog::openClearRank $_19: __func::destroy_deallocate
static void t_58434A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58434A, "BattleResultDialog::openClearRank $_19 __func::destroy_deallocate", t_58434A);

// BattleResultDialog::openClearRank $_19: __func::target
static uint32_t t_584450(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37480u ? self + 4 : 0; }
RW_FUNCTION(0x584450, "BattleResultDialog::openClearRank $_19 __func::target", t_584450);

// BattleResultDialog::openClearRank $_19: __func::target_type
static uint32_t t_584470() { return 0xf8b5bcu; }
RW_FUNCTION(0x584470, "BattleResultDialog::openClearRank $_19 __func::target_type", t_584470);

// BattleResultDialog::openClearRank $_19 UlvE_: __func::~__func (deleting)
static void t_584484(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584484, "BattleResultDialog::openClearRank $_19 UlvE_ __func::~__func (deleting)", t_584484);

// BattleResultDialog::openClearRank $_19 UlvE_: __func::__clone()
static uint32_t t_58448C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59f0cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x58448C, "BattleResultDialog::openClearRank $_19 UlvE_ __func::__clone()", t_58448C);

// BattleResultDialog::openClearRank $_19 UlvE_: __func::__clone(__base*)
static void t_5844B8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59f0cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5844B8, "BattleResultDialog::openClearRank $_19 UlvE_ __func::__clone(__base*)", t_5844B8);

// BattleResultDialog::openClearRank $_19 UlvE_: __func::destroy
static void t_5844D8(uint32_t self) {
}
RW_FUNCTION(0x5844D8, "BattleResultDialog::openClearRank $_19 UlvE_ __func::destroy", t_5844D8);

// BattleResultDialog::openClearRank $_19 UlvE_: __func::destroy_deallocate
static void t_5844DA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5844DA, "BattleResultDialog::openClearRank $_19 UlvE_ __func::destroy_deallocate", t_5844DA);

// BattleResultDialog::openClearRank $_19 UlvE_: __func::target
static uint32_t t_5844F0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37540u ? self + 4 : 0; }
RW_FUNCTION(0x5844F0, "BattleResultDialog::openClearRank $_19 UlvE_ __func::target", t_5844F0);

// BattleResultDialog::openClearRank $_19 UlvE_: __func::target_type
static uint32_t t_584510() { return 0xf8b5d0u; }
RW_FUNCTION(0x584510, "BattleResultDialog::openClearRank $_19 UlvE_ __func::target_type", t_584510);

// BattleResultDialog::openClearRank $_18: __func::~__func (deleting)
static void t_584524(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584524, "BattleResultDialog::openClearRank $_18 __func::~__func (deleting)", t_584524);

// BattleResultDialog::openClearRank $_18: __func::__clone()
static uint32_t t_58452C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59f38u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x58452C, "BattleResultDialog::openClearRank $_18 __func::__clone()", t_58452C);

// BattleResultDialog::openClearRank $_18: __func::__clone(__base*)
static void t_584558(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59f38u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584558, "BattleResultDialog::openClearRank $_18 __func::__clone(__base*)", t_584558);

// BattleResultDialog::openClearRank $_18: __func::destroy
static void t_584578(uint32_t self) {
}
RW_FUNCTION(0x584578, "BattleResultDialog::openClearRank $_18 __func::destroy", t_584578);

// BattleResultDialog::openClearRank $_18: __func::destroy_deallocate
static void t_58457A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58457A, "BattleResultDialog::openClearRank $_18 __func::destroy_deallocate", t_58457A);

// BattleResultDialog::openClearRank $_18: __func::target
static uint32_t t_58460C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37630u ? self + 4 : 0; }
RW_FUNCTION(0x58460C, "BattleResultDialog::openClearRank $_18 __func::target", t_58460C);

// BattleResultDialog::openClearRank $_18: __func::target_type
static uint32_t t_58462C() { return 0xf8b5e4u; }
RW_FUNCTION(0x58462C, "BattleResultDialog::openClearRank $_18 __func::target_type", t_58462C);

// BattleResultDialog::openClearRank $_17: __func::~__func (deleting)
static void t_584640(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584640, "BattleResultDialog::openClearRank $_17 __func::~__func (deleting)", t_584640);

// BattleResultDialog::openClearRank $_17: __func::__clone()
static uint32_t t_584648(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59f64u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x584648, "BattleResultDialog::openClearRank $_17 __func::__clone()", t_584648);

// BattleResultDialog::openClearRank $_17: __func::__clone(__base*)
static void t_584674(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59f64u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584674, "BattleResultDialog::openClearRank $_17 __func::__clone(__base*)", t_584674);

// BattleResultDialog::openClearRank $_17: __func::destroy
static void t_584694(uint32_t self) {
}
RW_FUNCTION(0x584694, "BattleResultDialog::openClearRank $_17 __func::destroy", t_584694);

// BattleResultDialog::openClearRank $_17: __func::destroy_deallocate
static void t_584696(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584696, "BattleResultDialog::openClearRank $_17 __func::destroy_deallocate", t_584696);

// BattleResultDialog::openClearRank $_17: __func::target
static uint32_t t_5846B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37710u ? self + 4 : 0; }
RW_FUNCTION(0x5846B8, "BattleResultDialog::openClearRank $_17 __func::target", t_5846B8);

// BattleResultDialog::openClearRank $_17: __func::target_type
static uint32_t t_5846D8() { return 0xf8b5f8u; }
RW_FUNCTION(0x5846D8, "BattleResultDialog::openClearRank $_17 __func::target_type", t_5846D8);

// BattleResultDialog::openGettingItem $_16: __func::~__func (deleting)
static void t_5846EC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5846EC, "BattleResultDialog::openGettingItem $_16 __func::~__func (deleting)", t_5846EC);

// BattleResultDialog::openGettingItem $_16: __func::__clone()
static uint32_t t_5846F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59f90u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5846F4, "BattleResultDialog::openGettingItem $_16 __func::__clone()", t_5846F4);

// BattleResultDialog::openGettingItem $_16: __func::__clone(__base*)
static void t_584720(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59f90u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584720, "BattleResultDialog::openGettingItem $_16 __func::__clone(__base*)", t_584720);

// BattleResultDialog::openGettingItem $_16: __func::destroy
static void t_584740(uint32_t self) {
}
RW_FUNCTION(0x584740, "BattleResultDialog::openGettingItem $_16 __func::destroy", t_584740);

// BattleResultDialog::openGettingItem $_16: __func::destroy_deallocate
static void t_584742(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584742, "BattleResultDialog::openGettingItem $_16 __func::destroy_deallocate", t_584742);

// BattleResultDialog::openGettingItem $_16: __func::target
static uint32_t t_584848(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe377d0u ? self + 4 : 0; }
RW_FUNCTION(0x584848, "BattleResultDialog::openGettingItem $_16 __func::target", t_584848);

// BattleResultDialog::openGettingItem $_16: __func::target_type
static uint32_t t_584868() { return 0xf8b60cu; }
RW_FUNCTION(0x584868, "BattleResultDialog::openGettingItem $_16 __func::target_type", t_584868);

// BattleResultDialog::openGettingItem $_16 UlvE_: __func::~__func (deleting)
static void t_58487C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58487C, "BattleResultDialog::openGettingItem $_16 UlvE_ __func::~__func (deleting)", t_58487C);

// BattleResultDialog::openGettingItem $_16 UlvE_: __func::__clone()
static uint32_t t_584884(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59fbcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x584884, "BattleResultDialog::openGettingItem $_16 UlvE_ __func::__clone()", t_584884);

// BattleResultDialog::openGettingItem $_16 UlvE_: __func::__clone(__base*)
static void t_5848B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59fbcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5848B0, "BattleResultDialog::openGettingItem $_16 UlvE_ __func::__clone(__base*)", t_5848B0);

// BattleResultDialog::openGettingItem $_16 UlvE_: __func::destroy
static void t_5848D0(uint32_t self) {
}
RW_FUNCTION(0x5848D0, "BattleResultDialog::openGettingItem $_16 UlvE_ __func::destroy", t_5848D0);

// BattleResultDialog::openGettingItem $_16 UlvE_: __func::destroy_deallocate
static void t_5848D2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5848D2, "BattleResultDialog::openGettingItem $_16 UlvE_ __func::destroy_deallocate", t_5848D2);

// BattleResultDialog::openGettingItem $_16 UlvE_: __func::target
static uint32_t t_584984(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37890u ? self + 4 : 0; }
RW_FUNCTION(0x584984, "BattleResultDialog::openGettingItem $_16 UlvE_ __func::target", t_584984);

// BattleResultDialog::openGettingItem $_16 UlvE_: __func::target_type
static uint32_t t_5849A4() { return 0xf8b620u; }
RW_FUNCTION(0x5849A4, "BattleResultDialog::openGettingItem $_16 UlvE_ __func::target_type", t_5849A4);

// BattleResultDialog::openGettingItem $_15: __func::~__func (deleting)
static void t_5849B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5849B8, "BattleResultDialog::openGettingItem $_15 __func::~__func (deleting)", t_5849B8);

// BattleResultDialog::openGettingItem $_15: __func::__clone()
static uint32_t t_5849C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf59fe8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5849C0, "BattleResultDialog::openGettingItem $_15 __func::__clone()", t_5849C0);

// BattleResultDialog::openGettingItem $_15: __func::__clone(__base*)
static void t_5849EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf59fe8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5849EC, "BattleResultDialog::openGettingItem $_15 __func::__clone(__base*)", t_5849EC);

// BattleResultDialog::openGettingItem $_15: __func::destroy
static void t_584A0C(uint32_t self) {
}
RW_FUNCTION(0x584A0C, "BattleResultDialog::openGettingItem $_15 __func::destroy", t_584A0C);

// BattleResultDialog::openGettingItem $_15: __func::destroy_deallocate
static void t_584A0E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584A0E, "BattleResultDialog::openGettingItem $_15 __func::destroy_deallocate", t_584A0E);

// BattleResultDialog::openGettingItem $_15: __func::target
static uint32_t t_584ABC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37980u ? self + 4 : 0; }
RW_FUNCTION(0x584ABC, "BattleResultDialog::openGettingItem $_15 __func::target", t_584ABC);

// BattleResultDialog::openGettingItem $_15: __func::target_type
static uint32_t t_584ADC() { return 0xf8b634u; }
RW_FUNCTION(0x584ADC, "BattleResultDialog::openGettingItem $_15 __func::target_type", t_584ADC);

// BattleResultDialog::openGettingItem $_14: __func::~__func (deleting)
static void t_584B18(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584B18, "BattleResultDialog::openGettingItem $_14 __func::~__func (deleting)", t_584B18);

// BattleResultDialog::openGettingItem $_14: __func::__clone()
static uint32_t t_584B20(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a014u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x584B20, "BattleResultDialog::openGettingItem $_14 __func::__clone()", t_584B20);

// BattleResultDialog::openGettingItem $_14: __func::__clone(__base*)
static void t_584B4C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a014u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584B4C, "BattleResultDialog::openGettingItem $_14 __func::__clone(__base*)", t_584B4C);

// BattleResultDialog::openGettingItem $_14: __func::destroy
static void t_584B6C(uint32_t self) {
}
RW_FUNCTION(0x584B6C, "BattleResultDialog::openGettingItem $_14 __func::destroy", t_584B6C);

// BattleResultDialog::openGettingItem $_14: __func::destroy_deallocate
static void t_584B6E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584B6E, "BattleResultDialog::openGettingItem $_14 __func::destroy_deallocate", t_584B6E);

// BattleResultDialog::openGettingItem $_14: __func::target
static uint32_t t_584B90(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37a40u ? self + 4 : 0; }
RW_FUNCTION(0x584B90, "BattleResultDialog::openGettingItem $_14 __func::target", t_584B90);

// BattleResultDialog::openGettingItem $_14: __func::target_type
static uint32_t t_584BB0() { return 0xf8b648u; }
RW_FUNCTION(0x584BB0, "BattleResultDialog::openGettingItem $_14 __func::target_type", t_584BB0);

// BattleResultDialog::openGettingItem $_13: __func::~__func (deleting)
static void t_584BC4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584BC4, "BattleResultDialog::openGettingItem $_13 __func::~__func (deleting)", t_584BC4);

// BattleResultDialog::openGettingItem $_13: __func::__clone()
static uint32_t t_584BCC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a040u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x584BCC, "BattleResultDialog::openGettingItem $_13 __func::__clone()", t_584BCC);

// BattleResultDialog::openGettingItem $_13: __func::__clone(__base*)
static void t_584BF8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a040u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584BF8, "BattleResultDialog::openGettingItem $_13 __func::__clone(__base*)", t_584BF8);

// BattleResultDialog::openGettingItem $_13: __func::destroy
static void t_584C18(uint32_t self) {
}
RW_FUNCTION(0x584C18, "BattleResultDialog::openGettingItem $_13 __func::destroy", t_584C18);

// BattleResultDialog::openGettingItem $_13: __func::destroy_deallocate
static void t_584C1A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584C1A, "BattleResultDialog::openGettingItem $_13 __func::destroy_deallocate", t_584C1A);

// BattleResultDialog::openGettingItem $_13: __func::target
static uint32_t t_584C54(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37b20u ? self + 4 : 0; }
RW_FUNCTION(0x584C54, "BattleResultDialog::openGettingItem $_13 __func::target", t_584C54);

// BattleResultDialog::openGettingItem $_13: __func::target_type
static uint32_t t_584C74() { return 0xf8b65cu; }
RW_FUNCTION(0x584C74, "BattleResultDialog::openGettingItem $_13 __func::target_type", t_584C74);

// BattleResultDialog::openColosseumResult $_12: __func::~__func (deleting)
static void t_584C88(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584C88, "BattleResultDialog::openColosseumResult $_12 __func::~__func (deleting)", t_584C88);

// BattleResultDialog::openColosseumResult $_12: __func::__clone()
static uint32_t t_584C90(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a06cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x584C90, "BattleResultDialog::openColosseumResult $_12 __func::__clone()", t_584C90);

// BattleResultDialog::openColosseumResult $_12: __func::__clone(__base*)
static void t_584CBC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a06cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584CBC, "BattleResultDialog::openColosseumResult $_12 __func::__clone(__base*)", t_584CBC);

// BattleResultDialog::openColosseumResult $_12: __func::destroy
static void t_584CDC(uint32_t self) {
}
RW_FUNCTION(0x584CDC, "BattleResultDialog::openColosseumResult $_12 __func::destroy", t_584CDC);

// BattleResultDialog::openColosseumResult $_12: __func::destroy_deallocate
static void t_584CDE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584CDE, "BattleResultDialog::openColosseumResult $_12 __func::destroy_deallocate", t_584CDE);

// BattleResultDialog::openColosseumResult $_12: __func::target
static uint32_t t_584DE4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37be0u ? self + 4 : 0; }
RW_FUNCTION(0x584DE4, "BattleResultDialog::openColosseumResult $_12 __func::target", t_584DE4);

// BattleResultDialog::openColosseumResult $_12: __func::target_type
static uint32_t t_584E04() { return 0xf8b670u; }
RW_FUNCTION(0x584E04, "BattleResultDialog::openColosseumResult $_12 __func::target_type", t_584E04);

// BattleResultDialog::openColosseumResult $_12 UlvE_: __func::~__func (deleting)
static void t_584E18(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584E18, "BattleResultDialog::openColosseumResult $_12 UlvE_ __func::~__func (deleting)", t_584E18);

// BattleResultDialog::openColosseumResult $_12 UlvE_: __func::__clone()
static uint32_t t_584E20(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a098u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x584E20, "BattleResultDialog::openColosseumResult $_12 UlvE_ __func::__clone()", t_584E20);

// BattleResultDialog::openColosseumResult $_12 UlvE_: __func::__clone(__base*)
static void t_584E4C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a098u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584E4C, "BattleResultDialog::openColosseumResult $_12 UlvE_ __func::__clone(__base*)", t_584E4C);

// BattleResultDialog::openColosseumResult $_12 UlvE_: __func::destroy
static void t_584E6C(uint32_t self) {
}
RW_FUNCTION(0x584E6C, "BattleResultDialog::openColosseumResult $_12 UlvE_ __func::destroy", t_584E6C);

// BattleResultDialog::openColosseumResult $_12 UlvE_: __func::destroy_deallocate
static void t_584E6E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584E6E, "BattleResultDialog::openColosseumResult $_12 UlvE_ __func::destroy_deallocate", t_584E6E);

// BattleResultDialog::openColosseumResult $_12 UlvE_: __func::target
static uint32_t t_584F20(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37cc0u ? self + 4 : 0; }
RW_FUNCTION(0x584F20, "BattleResultDialog::openColosseumResult $_12 UlvE_ __func::target", t_584F20);

// BattleResultDialog::openColosseumResult $_12 UlvE_: __func::target_type
static uint32_t t_584F40() { return 0xf8b684u; }
RW_FUNCTION(0x584F40, "BattleResultDialog::openColosseumResult $_12 UlvE_ __func::target_type", t_584F40);

// BattleResultDialog::openColosseumResult $_11: __func::~__func (deleting)
static void t_584F54(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584F54, "BattleResultDialog::openColosseumResult $_11 __func::~__func (deleting)", t_584F54);

// BattleResultDialog::openColosseumResult $_11: __func::__clone()
static uint32_t t_584F5C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a0c4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x584F5C, "BattleResultDialog::openColosseumResult $_11 __func::__clone()", t_584F5C);

// BattleResultDialog::openColosseumResult $_11: __func::__clone(__base*)
static void t_584F88(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a0c4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x584F88, "BattleResultDialog::openColosseumResult $_11 __func::__clone(__base*)", t_584F88);

// BattleResultDialog::openColosseumResult $_11: __func::destroy
static void t_584FA8(uint32_t self) {
}
RW_FUNCTION(0x584FA8, "BattleResultDialog::openColosseumResult $_11 __func::destroy", t_584FA8);

// BattleResultDialog::openColosseumResult $_11: __func::destroy_deallocate
static void t_584FAA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x584FAA, "BattleResultDialog::openColosseumResult $_11 __func::destroy_deallocate", t_584FAA);

// BattleResultDialog::openColosseumResult $_11: __func::target
static uint32_t t_5850A4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37db0u ? self + 4 : 0; }
RW_FUNCTION(0x5850A4, "BattleResultDialog::openColosseumResult $_11 __func::target", t_5850A4);

// BattleResultDialog::openColosseumResult $_11: __func::target_type
static uint32_t t_5850C4() { return 0xf8b698u; }
RW_FUNCTION(0x5850C4, "BattleResultDialog::openColosseumResult $_11 __func::target_type", t_5850C4);

// BattleResultDialog::openColosseumResult $_10: __func::~__func (deleting)
static void t_5850D8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5850D8, "BattleResultDialog::openColosseumResult $_10 __func::~__func (deleting)", t_5850D8);

// BattleResultDialog::openColosseumResult $_10: __func::__clone()
static uint32_t t_5850E0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a0f0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5850E0, "BattleResultDialog::openColosseumResult $_10 __func::__clone()", t_5850E0);

// BattleResultDialog::openColosseumResult $_10: __func::__clone(__base*)
static void t_58510C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a0f0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x58510C, "BattleResultDialog::openColosseumResult $_10 __func::__clone(__base*)", t_58510C);

// BattleResultDialog::openColosseumResult $_10: __func::destroy
static void t_58512C(uint32_t self) {
}
RW_FUNCTION(0x58512C, "BattleResultDialog::openColosseumResult $_10 __func::destroy", t_58512C);

// BattleResultDialog::openColosseumResult $_10: __func::destroy_deallocate
static void t_58512E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58512E, "BattleResultDialog::openColosseumResult $_10 __func::destroy_deallocate", t_58512E);

// BattleResultDialog::openColosseumResult $_10: __func::target
static uint32_t t_585230(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37e80u ? self + 4 : 0; }
RW_FUNCTION(0x585230, "BattleResultDialog::openColosseumResult $_10 __func::target", t_585230);

// BattleResultDialog::openColosseumResult $_10: __func::target_type
static uint32_t t_585250() { return 0xf8b6acu; }
RW_FUNCTION(0x585250, "BattleResultDialog::openColosseumResult $_10 __func::target_type", t_585250);

// BattleResultDialog::openRaidResult $_9: __func::~__func (deleting)
static void t_585264(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x585264, "BattleResultDialog::openRaidResult $_9 __func::~__func (deleting)", t_585264);

// BattleResultDialog::openRaidResult $_9: __func::__clone()
static uint32_t t_58526C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5a11cu);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x58526C, "BattleResultDialog::openRaidResult $_9 __func::__clone()", t_58526C);

// BattleResultDialog::openRaidResult $_9: __func::__clone(__base*)
static void t_58529C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a11cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x58529C, "BattleResultDialog::openRaidResult $_9 __func::__clone(__base*)", t_58529C);

// BattleResultDialog::openRaidResult $_9: __func::destroy
static void t_5852C0(uint32_t self) {
}
RW_FUNCTION(0x5852C0, "BattleResultDialog::openRaidResult $_9 __func::destroy", t_5852C0);

// BattleResultDialog::openRaidResult $_9: __func::destroy_deallocate
static void t_5852C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5852C2, "BattleResultDialog::openRaidResult $_9 __func::destroy_deallocate", t_5852C2);

// BattleResultDialog::openRaidResult $_9: __func::target
static uint32_t t_5853CC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe37f70u ? self + 4 : 0; }
RW_FUNCTION(0x5853CC, "BattleResultDialog::openRaidResult $_9 __func::target", t_5853CC);

// BattleResultDialog::openRaidResult $_9: __func::target_type
static uint32_t t_5853EC() { return 0xf8b6c0u; }
RW_FUNCTION(0x5853EC, "BattleResultDialog::openRaidResult $_9 __func::target_type", t_5853EC);

// BattleResultDialog::openRaidResult $_9 UlvE_: __func::~__func (deleting)
static void t_585400(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x585400, "BattleResultDialog::openRaidResult $_9 UlvE_ __func::~__func (deleting)", t_585400);

// BattleResultDialog::openRaidResult $_9 UlvE_: __func::__clone()
static uint32_t t_585408(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a148u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x585408, "BattleResultDialog::openRaidResult $_9 UlvE_ __func::__clone()", t_585408);

// BattleResultDialog::openRaidResult $_9 UlvE_: __func::__clone(__base*)
static void t_585434(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a148u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x585434, "BattleResultDialog::openRaidResult $_9 UlvE_ __func::__clone(__base*)", t_585434);

// BattleResultDialog::openRaidResult $_9 UlvE_: __func::destroy
static void t_585454(uint32_t self) {
}
RW_FUNCTION(0x585454, "BattleResultDialog::openRaidResult $_9 UlvE_ __func::destroy", t_585454);

// BattleResultDialog::openRaidResult $_9 UlvE_: __func::destroy_deallocate
static void t_585456(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x585456, "BattleResultDialog::openRaidResult $_9 UlvE_ __func::destroy_deallocate", t_585456);

// BattleResultDialog::openRaidResult $_9 UlvE_: __func::target
static uint32_t t_585504(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe38090u ? self + 4 : 0; }
RW_FUNCTION(0x585504, "BattleResultDialog::openRaidResult $_9 UlvE_ __func::target", t_585504);

// BattleResultDialog::openRaidResult $_9 UlvE_: __func::target_type
static uint32_t t_585524() { return 0xf8b6d4u; }
RW_FUNCTION(0x585524, "BattleResultDialog::openRaidResult $_9 UlvE_ __func::target_type", t_585524);

// BattleResultDialog::openRaidResult $_8: __func::~__func (deleting)
static void t_585538(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x585538, "BattleResultDialog::openRaidResult $_8 __func::~__func (deleting)", t_585538);

// BattleResultDialog::openRaidResult $_8: __func::__clone()
static uint32_t t_585540(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a174u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x585540, "BattleResultDialog::openRaidResult $_8 __func::__clone()", t_585540);

// BattleResultDialog::openRaidResult $_8: __func::__clone(__base*)
static void t_58556C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a174u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x58556C, "BattleResultDialog::openRaidResult $_8 __func::__clone(__base*)", t_58556C);

// BattleResultDialog::openRaidResult $_8: __func::destroy
static void t_58558C(uint32_t self) {
}
RW_FUNCTION(0x58558C, "BattleResultDialog::openRaidResult $_8 __func::destroy", t_58558C);

// BattleResultDialog::openRaidResult $_8: __func::destroy_deallocate
static void t_58558E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58558E, "BattleResultDialog::openRaidResult $_8 __func::destroy_deallocate", t_58558E);

// BattleResultDialog::openRaidResult $_8: __func::target
static uint32_t t_5856A8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe381e0u ? self + 4 : 0; }
RW_FUNCTION(0x5856A8, "BattleResultDialog::openRaidResult $_8 __func::target", t_5856A8);

// BattleResultDialog::openRaidResult $_8: __func::target_type
static uint32_t t_5856C8() { return 0xf8b6e8u; }
RW_FUNCTION(0x5856C8, "BattleResultDialog::openRaidResult $_8 __func::target_type", t_5856C8);

// BattleResultDialog::openRaidResult $_7: __func::~__func (deleting)
static void t_5856DC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5856DC, "BattleResultDialog::openRaidResult $_7 __func::~__func (deleting)", t_5856DC);

// BattleResultDialog::openRaidResult $_7: __func::__clone()
static uint32_t t_5856E4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a1a0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5856E4, "BattleResultDialog::openRaidResult $_7 __func::__clone()", t_5856E4);

// BattleResultDialog::openRaidResult $_7: __func::__clone(__base*)
static void t_585710(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a1a0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x585710, "BattleResultDialog::openRaidResult $_7 __func::__clone(__base*)", t_585710);

// BattleResultDialog::openRaidResult $_7: __func::destroy
static void t_585730(uint32_t self) {
}
RW_FUNCTION(0x585730, "BattleResultDialog::openRaidResult $_7 __func::destroy", t_585730);

// BattleResultDialog::openRaidResult $_7: __func::destroy_deallocate
static void t_585732(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x585732, "BattleResultDialog::openRaidResult $_7 __func::destroy_deallocate", t_585732);

// BattleResultDialog::openRaidResult $_7: __func::target
static uint32_t t_585834(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe382f0u ? self + 4 : 0; }
RW_FUNCTION(0x585834, "BattleResultDialog::openRaidResult $_7 __func::target", t_585834);
