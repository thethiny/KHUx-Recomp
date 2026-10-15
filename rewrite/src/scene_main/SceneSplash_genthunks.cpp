// The std::function __func thunks of the lambdas attributed to SceneSplash (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneManager::init $_3: __func::target
static uint32_t t_63819C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe5a4d0u ? self + 4 : 0; }
RW_FUNCTION(0x63819C, "SceneManager::init $_3 __func::target", t_63819C);

// SceneManager::init $_3: __func::target_type
static uint32_t t_6381BC() { return 0xf98ae0u; }
RW_FUNCTION(0x6381BC, "SceneManager::init $_3 __func::target_type", t_6381BC);

// SceneManager::init $_2: __func::~__func (deleting)
static void t_6381D0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6381D0, "SceneManager::init $_2 __func::~__func (deleting)", t_6381D0);

// SceneManager::init $_2: __func::__clone()
static uint32_t t_6381D8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf608f4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6381D8, "SceneManager::init $_2 __func::__clone()", t_6381D8);

// SceneManager::init $_2: __func::__clone(__base*)
static void t_638204(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf608f4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x638204, "SceneManager::init $_2 __func::__clone(__base*)", t_638204);

// SceneManager::init $_2: __func::destroy
static void t_638224(uint32_t self) {
}
RW_FUNCTION(0x638224, "SceneManager::init $_2 __func::destroy", t_638224);

// SceneManager::init $_2: __func::destroy_deallocate
static void t_638226(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x638226, "SceneManager::init $_2 __func::destroy_deallocate", t_638226);

// SceneManager::init $_2: __func::target
static uint32_t t_6382A8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe5a560u ? self + 4 : 0; }
RW_FUNCTION(0x6382A8, "SceneManager::init $_2 __func::target", t_6382A8);

// SceneManager::init $_2: __func::target_type
static uint32_t t_6382C8() { return 0xf98af4u; }
RW_FUNCTION(0x6382C8, "SceneManager::init $_2 __func::target_type", t_6382C8);

// SceneManager::init $_1: __func::~__func (deleting)
static void t_6382DC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6382DC, "SceneManager::init $_1 __func::~__func (deleting)", t_6382DC);

// SceneManager::init $_1: __func::__clone()
static uint32_t t_6382E4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf60920u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6382E4, "SceneManager::init $_1 __func::__clone()", t_6382E4);

// SceneManager::init $_1: __func::__clone(__base*)
static void t_638310(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf60920u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x638310, "SceneManager::init $_1 __func::__clone(__base*)", t_638310);

// SceneManager::init $_1: __func::destroy
static void t_638330(uint32_t self) {
}
RW_FUNCTION(0x638330, "SceneManager::init $_1 __func::destroy", t_638330);

// SceneManager::init $_1: __func::destroy_deallocate
static void t_638332(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x638332, "SceneManager::init $_1 __func::destroy_deallocate", t_638332);

// SceneManager::init $_1: __func::target
static uint32_t t_6383B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe5a5f0u ? self + 4 : 0; }
RW_FUNCTION(0x6383B4, "SceneManager::init $_1 __func::target", t_6383B4);

// SceneManager::init $_1: __func::target_type
static uint32_t t_6383D4() { return 0xf98b08u; }
RW_FUNCTION(0x6383D4, "SceneManager::init $_1 __func::target_type", t_6383D4);

// SceneManager::init $_0: __func::~__func (deleting)
static void t_6383E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6383E8, "SceneManager::init $_0 __func::~__func (deleting)", t_6383E8);

// SceneManager::init $_0: __func::__clone()
static uint32_t t_6383F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6094cu);
    }
    return v1;
}
RW_FUNCTION(0x6383F0, "SceneManager::init $_0 __func::__clone()", t_6383F0);

// SceneManager::init $_0: __func::__clone(__base*)
static void t_638414(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6094cu);
    }
}
RW_FUNCTION(0x638414, "SceneManager::init $_0 __func::__clone(__base*)", t_638414);

// SceneManager::init $_0: __func::destroy
static void t_638430(uint32_t self) {
}
RW_FUNCTION(0x638430, "SceneManager::init $_0 __func::destroy", t_638430);

// SceneManager::init $_0: __func::destroy_deallocate
static void t_638432(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x638432, "SceneManager::init $_0 __func::destroy_deallocate", t_638432);

// SceneManager::init $_0: __func::target
static uint32_t t_638440(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe5a680u ? self + 4 : 0; }
RW_FUNCTION(0x638440, "SceneManager::init $_0 __func::target", t_638440);

// SceneManager::init $_0: __func::target_type
static uint32_t t_638460() { return 0xf98b1cu; }
RW_FUNCTION(0x638460, "SceneManager::init $_0 __func::target_type", t_638460);

// SceneManager::openChatPopup $_129: __func::~__func (deleting)
static void t_6384E0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6384E0, "SceneManager::openChatPopup $_129 __func::~__func (deleting)", t_6384E0);

// SceneManager::openChatPopup $_129: __func::destroy
static void t_63854C(uint32_t self) {
}
RW_FUNCTION(0x63854C, "SceneManager::openChatPopup $_129 __func::destroy", t_63854C);

// SceneManager::openChatPopup $_129: __func::destroy_deallocate
static void t_63854E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x63854E, "SceneManager::openChatPopup $_129 __func::destroy_deallocate", t_63854E);

// SceneManager::openChatPopup $_129: __func::target
static uint32_t t_638744(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe5a700u ? self + 4 : 0; }
RW_FUNCTION(0x638744, "SceneManager::openChatPopup $_129 __func::target", t_638744);

// SceneManager::openChatPopup $_129: __func::target_type
static uint32_t t_638764() { return 0xf98b30u; }
RW_FUNCTION(0x638764, "SceneManager::openChatPopup $_129 __func::target_type", t_638764);

// SceneManager::chatDataUpdate $_37 (shared): __func::~__func
static uint32_t t_638778(uint32_t self) {
    return self;
}
RW_FUNCTION(0x638778, "SceneManager::chatDataUpdate $_37 (shared) __func::~__func", t_638778);

// SceneManager::openChatPopup $_130: __func::~__func (deleting)
static void t_63877A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x63877A, "SceneManager::openChatPopup $_130 __func::~__func (deleting)", t_63877A);

// SceneManager::openChatPopup $_130: __func::__clone()
static uint32_t t_638784(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf609a4u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20 + 0);
        uint32_t v7 = ld32(self + 20 + 4);
        uint32_t v8 = ld32(self + 20 + 8);
        uint32_t v9 = ld32(self + 20 + 12);
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
        st32(v1 + 20 + 12, v9);
        uint32_t v10 = ld32(self + 36);
        st32(v1 + 36, v10);
    }
    return v1;
}
RW_FUNCTION(0x638784, "SceneManager::openChatPopup $_130 __func::__clone()", t_638784);

// SceneManager::openChatPopup $_130: __func::__clone(__base*)
static void t_6387C4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf609a4u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20 + 0);
        uint32_t v6 = ld32(self + 20 + 4);
        uint32_t v7 = ld32(self + 20 + 8);
        uint32_t v8 = ld32(self + 20 + 12);
        st32(p + 20 + 0, v5);
        st32(p + 20 + 4, v6);
        st32(p + 20 + 8, v7);
        st32(p + 20 + 12, v8);
        uint32_t v9 = ld32(self + 36);
        st32(p + 36, v9);
    }
}
RW_FUNCTION(0x6387C4, "SceneManager::openChatPopup $_130 __func::__clone(__base*)", t_6387C4);

// SceneManager::openChatPopup $_130: __func::destroy
static void t_6387F8(uint32_t self) {
}
RW_FUNCTION(0x6387F8, "SceneManager::openChatPopup $_130 __func::destroy", t_6387F8);

// SceneManager::openChatPopup $_130: __func::destroy_deallocate
static void t_6387FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6387FA, "SceneManager::openChatPopup $_130 __func::destroy_deallocate", t_6387FA);

// SceneTitle::init $_3 UlvE_: __func::target
static uint32_t t_680370(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6f010u ? self + 4 : 0; }
RW_FUNCTION(0x680370, "SceneTitle::init $_3 UlvE_ __func::target", t_680370);

// SceneTitle::init $_3 UlvE_: __func::target_type
static uint32_t t_680390() { return 0xf9bc20u; }
RW_FUNCTION(0x680390, "SceneTitle::init $_3 UlvE_ __func::target_type", t_680390);

// SceneTitle::init $_2: __func::~__func (deleting)
static void t_6803A4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6803A4, "SceneTitle::init $_2 __func::~__func (deleting)", t_6803A4);

// SceneTitle::init $_2: __func::__clone()
static uint32_t t_6803AC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64288u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6803AC, "SceneTitle::init $_2 __func::__clone()", t_6803AC);

// SceneTitle::init $_2: __func::__clone(__base*)
static void t_6803D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64288u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6803D8, "SceneTitle::init $_2 __func::__clone(__base*)", t_6803D8);

// SceneTitle::init $_2: __func::destroy
static void t_6803F8(uint32_t self) {
}
RW_FUNCTION(0x6803F8, "SceneTitle::init $_2 __func::destroy", t_6803F8);

// SceneTitle::init $_2: __func::destroy_deallocate
static void t_6803FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6803FA, "SceneTitle::init $_2 __func::destroy_deallocate", t_6803FA);

// SceneTitle::init $_2: __func::target
static uint32_t t_68040C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6f090u ? self + 4 : 0; }
RW_FUNCTION(0x68040C, "SceneTitle::init $_2 __func::target", t_68040C);

// SceneTitle::init $_2: __func::target_type
static uint32_t t_68042C() { return 0xf9bc34u; }
RW_FUNCTION(0x68042C, "SceneTitle::init $_2 __func::target_type", t_68042C);

// SceneTitle::init $_1: __func::~__func (deleting)
static void t_680440(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680440, "SceneTitle::init $_1 __func::~__func (deleting)", t_680440);

// SceneTitle::init $_1: __func::__clone()
static uint32_t t_680448(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf642b4u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x680448, "SceneTitle::init $_1 __func::__clone()", t_680448);

// SceneTitle::init $_1: __func::__clone(__base*)
static void t_680474(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf642b4u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x680474, "SceneTitle::init $_1 __func::__clone(__base*)", t_680474);

// SceneTitle::init $_1: __func::destroy
static void t_680494(uint32_t self) {
}
RW_FUNCTION(0x680494, "SceneTitle::init $_1 __func::destroy", t_680494);

// SceneTitle::init $_1: __func::destroy_deallocate
static void t_680496(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680496, "SceneTitle::init $_1 __func::destroy_deallocate", t_680496);

// SceneTitle::init $_1: __func::target
static uint32_t t_6804B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6f100u ? self + 4 : 0; }
RW_FUNCTION(0x6804B8, "SceneTitle::init $_1 __func::target", t_6804B8);

// SceneTitle::init $_1: __func::target_type
static uint32_t t_6804D8() { return 0xf9bc48u; }
RW_FUNCTION(0x6804D8, "SceneTitle::init $_1 __func::target_type", t_6804D8);

// SceneTitle::init $_0: __func::~__func (deleting)
static void t_6804EC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6804EC, "SceneTitle::init $_0 __func::~__func (deleting)", t_6804EC);

// SceneTitle::init $_0: __func::__clone()
static uint32_t t_6804F4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf642e0u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6804F4, "SceneTitle::init $_0 __func::__clone()", t_6804F4);

// SceneTitle::init $_0: __func::__clone(__base*)
static void t_680520(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf642e0u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x680520, "SceneTitle::init $_0 __func::__clone(__base*)", t_680520);

// SceneTitle::init $_0: __func::destroy
static void t_680540(uint32_t self) {
}
RW_FUNCTION(0x680540, "SceneTitle::init $_0 __func::destroy", t_680540);

// SceneTitle::init $_0: __func::destroy_deallocate
static void t_680542(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680542, "SceneTitle::init $_0 __func::destroy_deallocate", t_680542);

// SceneTitle::init $_0: __func::target
static uint32_t t_6805F4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6f170u ? self + 4 : 0; }
RW_FUNCTION(0x6805F4, "SceneTitle::init $_0 __func::target", t_6805F4);

// SceneTitle::init $_0: __func::target_type
static uint32_t t_680614() { return 0xf9bc5cu; }
RW_FUNCTION(0x680614, "SceneTitle::init $_0 __func::target_type", t_680614);

// SceneTitle::init $_0 UlbE_: __func::~__func (deleting)
static void t_680628(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680628, "SceneTitle::init $_0 UlbE_ __func::~__func (deleting)", t_680628);

// SceneTitle::init $_0 UlbE_: __func::__clone()
static uint32_t t_680630(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf6430cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x680630, "SceneTitle::init $_0 UlbE_ __func::__clone()", t_680630);

// SceneTitle::init $_0 UlbE_: __func::__clone(__base*)
static void t_68065C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf6430cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x68065C, "SceneTitle::init $_0 UlbE_ __func::__clone(__base*)", t_68065C);

// SceneTitle::init $_0 UlbE_: __func::destroy
static void t_68067C(uint32_t self) {
}
RW_FUNCTION(0x68067C, "SceneTitle::init $_0 UlbE_ __func::destroy", t_68067C);

// SceneTitle::init $_0 UlbE_: __func::destroy_deallocate
static void t_68067E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x68067E, "SceneTitle::init $_0 UlbE_ __func::destroy_deallocate", t_68067E);

// SceneTitle::init $_0 UlbE_: __func::target
static uint32_t t_680730(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6f1f0u ? self + 4 : 0; }
RW_FUNCTION(0x680730, "SceneTitle::init $_0 UlbE_ __func::target", t_680730);

// SceneTitle::init $_0 UlbE_: __func::target_type
static uint32_t t_680750() { return 0xf9bc70u; }
RW_FUNCTION(0x680750, "SceneTitle::init $_0 UlbE_ __func::target_type", t_680750);

// SceneTitle::init $_0 UlbE_ UlbE_: __func::~__func (deleting)
static void t_680764(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680764, "SceneTitle::init $_0 UlbE_ UlbE_ __func::~__func (deleting)", t_680764);

// SceneTitle::init $_0 UlbE_ UlbE_: __func::__clone()
static uint32_t t_68076C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64338u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x68076C, "SceneTitle::init $_0 UlbE_ UlbE_ __func::__clone()", t_68076C);

// SceneTitle::init $_0 UlbE_ UlbE_: __func::__clone(__base*)
static void t_680798(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64338u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x680798, "SceneTitle::init $_0 UlbE_ UlbE_ __func::__clone(__base*)", t_680798);

// SceneTitle::init $_0 UlbE_ UlbE_: __func::destroy
static void t_6807B8(uint32_t self) {
}
RW_FUNCTION(0x6807B8, "SceneTitle::init $_0 UlbE_ UlbE_ __func::destroy", t_6807B8);

// SceneTitle::init $_0 UlbE_ UlbE_: __func::destroy_deallocate
static void t_6807BA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6807BA, "SceneTitle::init $_0 UlbE_ UlbE_ __func::destroy_deallocate", t_6807BA);

// SceneTitle::init $_0 UlbE_ UlbE_: __func::target
static uint32_t t_680A08(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6f290u ? self + 4 : 0; }
RW_FUNCTION(0x680A08, "SceneTitle::init $_0 UlbE_ UlbE_ __func::target", t_680A08);

// SceneTitle::init $_0 UlbE_ UlbE_: __func::target_type
static uint32_t t_680A28() { return 0xf9bc84u; }
RW_FUNCTION(0x680A28, "SceneTitle::init $_0 UlbE_ UlbE_ __func::target_type", t_680A28);

// SceneTitle::init $_9 UlbE0_: __func::~__func (deleting)
static void t_680A3C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680A3C, "SceneTitle::init $_9 UlbE0_ __func::~__func (deleting)", t_680A3C);

// SceneTitle::init $_9 UlbE0_: __func::__clone()
static uint32_t t_680A44(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64364u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x680A44, "SceneTitle::init $_9 UlbE0_ __func::__clone()", t_680A44);

// SceneTitle::init $_9 UlbE0_: __func::__clone(__base*)
static void t_680A70(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64364u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x680A70, "SceneTitle::init $_9 UlbE0_ __func::__clone(__base*)", t_680A70);

// SceneTitle::init $_9 UlbE0_: __func::destroy
static void t_680A90(uint32_t self) {
}
RW_FUNCTION(0x680A90, "SceneTitle::init $_9 UlbE0_ __func::destroy", t_680A90);

// SceneTitle::init $_9 UlbE0_: __func::destroy_deallocate
static void t_680A92(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680A92, "SceneTitle::init $_9 UlbE0_ __func::destroy_deallocate", t_680A92);

// SceneTitle::init $_9 UlbE0_: __func::target
static uint32_t t_680B64(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6f330u ? self + 4 : 0; }
RW_FUNCTION(0x680B64, "SceneTitle::init $_9 UlbE0_ __func::target", t_680B64);

// SceneTitle::init $_9 UlbE0_: __func::target_type
static uint32_t t_680B84() { return 0xf9bc98u; }
RW_FUNCTION(0x680B84, "SceneTitle::init $_9 UlbE0_ __func::target_type", t_680B84);

// SceneTitle::init $_9 UlbE0_ UlvE_: __func::~__func (deleting)
static void t_680B98(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680B98, "SceneTitle::init $_9 UlbE0_ UlvE_ __func::~__func (deleting)", t_680B98);

// SceneTitle::init $_9 UlbE0_ UlvE_: __func::__clone()
static uint32_t t_680BA0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf64390u);
    }
    return v1;
}
RW_FUNCTION(0x680BA0, "SceneTitle::init $_9 UlbE0_ UlvE_ __func::__clone()", t_680BA0);

// SceneTitle::init $_9 UlbE0_ UlvE_: __func::__clone(__base*)
static void t_680BC4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf64390u);
    }
}
RW_FUNCTION(0x680BC4, "SceneTitle::init $_9 UlbE0_ UlvE_ __func::__clone(__base*)", t_680BC4);

// SceneTitle::init $_9 UlbE0_ UlvE_: __func::destroy
static void t_680BE0(uint32_t self) {
}
RW_FUNCTION(0x680BE0, "SceneTitle::init $_9 UlbE0_ UlvE_ __func::destroy", t_680BE0);

// SceneTitle::init $_9 UlbE0_ UlvE_: __func::destroy_deallocate
static void t_680BE2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680BE2, "SceneTitle::init $_9 UlbE0_ UlvE_ __func::destroy_deallocate", t_680BE2);

// SceneTitle::init $_9 UlbE0_ UlvE_: __func::target
static uint32_t t_680C70(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6f3d0u ? self + 4 : 0; }
RW_FUNCTION(0x680C70, "SceneTitle::init $_9 UlbE0_ UlvE_ __func::target", t_680C70);

// SceneTitle::init $_9 UlbE0_ UlvE_: __func::target_type
static uint32_t t_680C90() { return 0xf9bcacu; }
RW_FUNCTION(0x680C90, "SceneTitle::init $_9 UlbE0_ UlvE_ __func::target_type", t_680C90);

// SceneTitle::init $_9 UlbE_: __func::~__func (deleting)
static void t_680CA4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680CA4, "SceneTitle::init $_9 UlbE_ __func::~__func (deleting)", t_680CA4);

// SceneTitle::init $_9 UlbE_: __func::__clone()
static uint32_t t_680CAC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf643bcu);
    }
    return v1;
}
RW_FUNCTION(0x680CAC, "SceneTitle::init $_9 UlbE_ __func::__clone()", t_680CAC);

// SceneTitle::init $_9 UlbE_: __func::__clone(__base*)
static void t_680CD0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf643bcu);
    }
}
RW_FUNCTION(0x680CD0, "SceneTitle::init $_9 UlbE_ __func::__clone(__base*)", t_680CD0);

// SceneTitle::init $_9 UlbE_: __func::destroy
static void t_680CEC(uint32_t self) {
}
RW_FUNCTION(0x680CEC, "SceneTitle::init $_9 UlbE_ __func::destroy", t_680CEC);

// SceneTitle::init $_9 UlbE_: __func::destroy_deallocate
static void t_680CEE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x680CEE, "SceneTitle::init $_9 UlbE_ __func::destroy_deallocate", t_680CEE);

// SceneTitle::init $_9 UlbE_: __func::target
static uint32_t t_680D04(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe6f470u ? self + 4 : 0; }
RW_FUNCTION(0x680D04, "SceneTitle::init $_9 UlbE_ __func::target", t_680D04);

// SceneTitle::init $_9 UlbE_: __func::target_type
static uint32_t t_680D24() { return 0xf9bcc0u; }
RW_FUNCTION(0x680D24, "SceneTitle::init $_9 UlbE_ __func::target_type", t_680D24);
