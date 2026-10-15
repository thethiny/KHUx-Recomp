// Generated from the lifted code by gen_thunks.py (scratch tool, wave 3 group 4): the std::function __func
// thunks of the SceneManager lambdas - clone (copy the captures into a new / given block), destroy (destroy the
// captured objects), target / target_type (typeinfo checks). Each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// SceneManager::ChainOfTutorialShop $_120: __func::~__func (deleting)
static void t_62207A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62207A, "SceneManager::ChainOfTutorialShop $_120 __func::~__func (deleting)", t_62207A);

// SceneManager::ChainOfTutorialShop $_120: __func::__clone()
static uint32_t t_622084(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5efb0u);
    }
    return v1;
}
RW_FUNCTION(0x622084, "SceneManager::ChainOfTutorialShop $_120 __func::__clone()", t_622084);

// SceneManager::ChainOfTutorialShop $_120: __func::__clone(__base*)
static void t_6220A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5efb0u);
    }
}
RW_FUNCTION(0x6220A8, "SceneManager::ChainOfTutorialShop $_120 __func::__clone(__base*)", t_6220A8);

// SceneManager::ChainOfTutorialShop $_120: __func::destroy
static void t_6220C4(uint32_t self) {
}
RW_FUNCTION(0x6220C4, "SceneManager::ChainOfTutorialShop $_120 __func::destroy", t_6220C4);

// SceneManager::ChainOfTutorialShop $_120: __func::destroy_deallocate
static void t_6220C6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6220C6, "SceneManager::ChainOfTutorialShop $_120 __func::destroy_deallocate", t_6220C6);

// SceneManager::ChainOfTutorialShop $_120: __func::target
static uint32_t t_6221FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe52a10u ? self + 4 : 0; }
RW_FUNCTION(0x6221FC, "SceneManager::ChainOfTutorialShop $_120 __func::target", t_6221FC);

// SceneManager::ChainOfTutorialShop $_120: __func::target_type
static uint32_t t_62221C() { return 0xf97f78u; }
RW_FUNCTION(0x62221C, "SceneManager::ChainOfTutorialShop $_120 __func::target_type", t_62221C);

// SceneManager::ChainOfTutorialDeckEdit $_119: __func::~__func (deleting)
static void t_622230(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x622230, "SceneManager::ChainOfTutorialDeckEdit $_119 __func::~__func (deleting)", t_622230);

// SceneManager::ChainOfTutorialDeckEdit $_119: __func::__clone()
static uint32_t t_622238(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5efdcu);
    }
    return v1;
}
RW_FUNCTION(0x622238, "SceneManager::ChainOfTutorialDeckEdit $_119 __func::__clone()", t_622238);

// SceneManager::ChainOfTutorialDeckEdit $_119: __func::__clone(__base*)
static void t_62225C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5efdcu);
    }
}
RW_FUNCTION(0x62225C, "SceneManager::ChainOfTutorialDeckEdit $_119 __func::__clone(__base*)", t_62225C);

// SceneManager::ChainOfTutorialDeckEdit $_119: __func::destroy
static void t_622278(uint32_t self) {
}
RW_FUNCTION(0x622278, "SceneManager::ChainOfTutorialDeckEdit $_119 __func::destroy", t_622278);

// SceneManager::ChainOfTutorialDeckEdit $_119: __func::destroy_deallocate
static void t_62227A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62227A, "SceneManager::ChainOfTutorialDeckEdit $_119 __func::destroy_deallocate", t_62227A);

// SceneManager::ChainOfTutorialDeckEdit $_119: __func::target
static uint32_t t_622384(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe52af0u ? self + 4 : 0; }
RW_FUNCTION(0x622384, "SceneManager::ChainOfTutorialDeckEdit $_119 __func::target", t_622384);

// SceneManager::ChainOfTutorialDeckEdit $_119: __func::target_type
static uint32_t t_6223A4() { return 0xf97f8cu; }
RW_FUNCTION(0x6223A4, "SceneManager::ChainOfTutorialDeckEdit $_119 __func::target_type", t_6223A4);

// SceneManager::ChainOfTutorialSphere $_118: __func::~__func (deleting)
static void t_6223B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6223B8, "SceneManager::ChainOfTutorialSphere $_118 __func::~__func (deleting)", t_6223B8);

// SceneManager::ChainOfTutorialSphere $_118: __func::__clone()
static uint32_t t_6223C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f008u);
    }
    return v1;
}
RW_FUNCTION(0x6223C0, "SceneManager::ChainOfTutorialSphere $_118 __func::__clone()", t_6223C0);

// SceneManager::ChainOfTutorialSphere $_118: __func::__clone(__base*)
static void t_6223E4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f008u);
    }
}
RW_FUNCTION(0x6223E4, "SceneManager::ChainOfTutorialSphere $_118 __func::__clone(__base*)", t_6223E4);

// SceneManager::ChainOfTutorialSphere $_118: __func::destroy
static void t_622400(uint32_t self) {
}
RW_FUNCTION(0x622400, "SceneManager::ChainOfTutorialSphere $_118 __func::destroy", t_622400);

// SceneManager::ChainOfTutorialSphere $_118: __func::destroy_deallocate
static void t_622402(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x622402, "SceneManager::ChainOfTutorialSphere $_118 __func::destroy_deallocate", t_622402);

// SceneManager::ChainOfTutorialSphere $_118: __func::target
static uint32_t t_62250C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe52bc0u ? self + 4 : 0; }
RW_FUNCTION(0x62250C, "SceneManager::ChainOfTutorialSphere $_118 __func::target", t_62250C);

// SceneManager::ChainOfTutorialSphere $_118: __func::target_type
static uint32_t t_62252C() { return 0xf97fa0u; }
RW_FUNCTION(0x62252C, "SceneManager::ChainOfTutorialSphere $_118 __func::target_type", t_62252C);

// SceneManager::ChainOfTutorialAdventureSelect $_117: __func::~__func (deleting)
static void t_622540(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x622540, "SceneManager::ChainOfTutorialAdventureSelect $_117 __func::~__func (deleting)", t_622540);

// SceneManager::ChainOfTutorialAdventureSelect $_117: __func::__clone()
static uint32_t t_622548(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f034u);
    }
    return v1;
}
RW_FUNCTION(0x622548, "SceneManager::ChainOfTutorialAdventureSelect $_117 __func::__clone()", t_622548);

// SceneManager::ChainOfTutorialAdventureSelect $_117: __func::__clone(__base*)
static void t_62256C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f034u);
    }
}
RW_FUNCTION(0x62256C, "SceneManager::ChainOfTutorialAdventureSelect $_117 __func::__clone(__base*)", t_62256C);

// SceneManager::ChainOfTutorialAdventureSelect $_117: __func::destroy
static void t_622588(uint32_t self) {
}
RW_FUNCTION(0x622588, "SceneManager::ChainOfTutorialAdventureSelect $_117 __func::destroy", t_622588);

// SceneManager::ChainOfTutorialAdventureSelect $_117: __func::destroy_deallocate
static void t_62258A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62258A, "SceneManager::ChainOfTutorialAdventureSelect $_117 __func::destroy_deallocate", t_62258A);

// SceneManager::ChainOfTutorialAdventureSelect $_117: __func::target
static uint32_t t_622694(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe52ca0u ? self + 4 : 0; }
RW_FUNCTION(0x622694, "SceneManager::ChainOfTutorialAdventureSelect $_117 __func::target", t_622694);

// SceneManager::ChainOfTutorialAdventureSelect $_117: __func::target_type
static uint32_t t_6226B4() { return 0xf97fb4u; }
RW_FUNCTION(0x6226B4, "SceneManager::ChainOfTutorialAdventureSelect $_117 __func::target_type", t_6226B4);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_: __func::~__func (deleting)
static void t_6226C8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6226C8, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ __func::~__func (deleting)", t_6226C8);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_: __func::__clone()
static uint32_t t_6226D0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f060u);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6226D0, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ __func::__clone()", t_6226D0);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_: __func::__clone(__base*)
static void t_6226FC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f060u);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x6226FC, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ __func::__clone(__base*)", t_6226FC);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_: __func::destroy
static void t_62271C(uint32_t self) {
}
RW_FUNCTION(0x62271C, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ __func::destroy", t_62271C);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_: __func::destroy_deallocate
static void t_62271E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62271E, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ __func::destroy_deallocate", t_62271E);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_: __func::target
static uint32_t t_6227CC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe52da0u ? self + 4 : 0; }
RW_FUNCTION(0x6227CC, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ __func::target", t_6227CC);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_: __func::target_type
static uint32_t t_6227EC() { return 0xf97fc8u; }
RW_FUNCTION(0x6227EC, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ __func::target_type", t_6227EC);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_: __func::~__func (deleting)
static void t_622800(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x622800, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_ __func::~__func (deleting)", t_622800);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_: __func::__clone()
static uint32_t t_622808(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f08cu);
        uint32_t v2 = ld8(self + 4);
        st8(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x622808, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_ __func::__clone()", t_622808);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_: __func::__clone(__base*)
static void t_622834(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f08cu);
        uint32_t v1 = ld8(self + 4);
        st8(p + 4, v1);
    }
}
RW_FUNCTION(0x622834, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_ __func::__clone(__base*)", t_622834);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_: __func::destroy
static void t_622854(uint32_t self) {
}
RW_FUNCTION(0x622854, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_ __func::destroy", t_622854);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_: __func::destroy_deallocate
static void t_622856(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x622856, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_ __func::destroy_deallocate", t_622856);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_: __func::target
static uint32_t t_622860(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe52ec0u ? self + 4 : 0; }
RW_FUNCTION(0x622860, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_ __func::target", t_622860);

// SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_: __func::target_type
static uint32_t t_622880() { return 0xf97fdcu; }
RW_FUNCTION(0x622880, "SceneManager::ChainOfTutorialAdventureSelect $_117 UlbE_ UlbE_ __func::target_type", t_622880);

// SceneManager::ChainOfTutorialAdventureSelect $_116: __func::~__func (deleting)
static void t_622894(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x622894, "SceneManager::ChainOfTutorialAdventureSelect $_116 __func::~__func (deleting)", t_622894);

// SceneManager::ChainOfTutorialAdventureSelect $_116: __func::__clone()
static uint32_t t_62289C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f0b8u);
    }
    return v1;
}
RW_FUNCTION(0x62289C, "SceneManager::ChainOfTutorialAdventureSelect $_116 __func::__clone()", t_62289C);

// SceneManager::ChainOfTutorialAdventureSelect $_116: __func::__clone(__base*)
static void t_6228C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f0b8u);
    }
}
RW_FUNCTION(0x6228C0, "SceneManager::ChainOfTutorialAdventureSelect $_116 __func::__clone(__base*)", t_6228C0);

// SceneManager::ChainOfTutorialAdventureSelect $_116: __func::destroy
static void t_6228DC(uint32_t self) {
}
RW_FUNCTION(0x6228DC, "SceneManager::ChainOfTutorialAdventureSelect $_116 __func::destroy", t_6228DC);

// SceneManager::ChainOfTutorialAdventureSelect $_116: __func::destroy_deallocate
static void t_6228DE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6228DE, "SceneManager::ChainOfTutorialAdventureSelect $_116 __func::destroy_deallocate", t_6228DE);

// SceneManager::ChainOfTutorialAdventureSelect $_116: __func::target
static uint32_t t_6229A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe52fc0u ? self + 4 : 0; }
RW_FUNCTION(0x6229A0, "SceneManager::ChainOfTutorialAdventureSelect $_116 __func::target", t_6229A0);

// SceneManager::ChainOfTutorialAdventureSelect $_116: __func::target_type
static uint32_t t_6229C0() { return 0xf97ff0u; }
RW_FUNCTION(0x6229C0, "SceneManager::ChainOfTutorialAdventureSelect $_116 __func::target_type", t_6229C0);

// SceneManager::callCurrentSceneRunOutAnimation $_115: __func::~__func
static uint32_t t_6229D4(uint32_t self) {
    st32(self, 0xf5f0e4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x6229D4, "SceneManager::callCurrentSceneRunOutAnimation $_115 __func::~__func", t_6229D4);

// SceneManager::callCurrentSceneRunOutAnimation $_115: __func::~__func (deleting)
static void t_622A10(uint32_t self) {
    st32(self, 0xf5f0e4u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x622A10, "SceneManager::callCurrentSceneRunOutAnimation $_115 __func::~__func (deleting)", t_622A10);

// SceneManager::callCurrentSceneRunOutAnimation $_115: __func::__clone()
static uint32_t t_622A50(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5f0e4u);
        call(0x4683cd, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x622A50, "SceneManager::callCurrentSceneRunOutAnimation $_115 __func::__clone()", t_622A50);

// SceneManager::callCurrentSceneRunOutAnimation $_115: __func::__clone(__base*)
static void t_622AA0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f0e4u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x622AA0, "SceneManager::callCurrentSceneRunOutAnimation $_115 __func::__clone(__base*)", t_622AA0);

// SceneManager::callCurrentSceneRunOutAnimation $_115: __func::destroy
static void t_622AD4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x622AD4, "SceneManager::callCurrentSceneRunOutAnimation $_115 __func::destroy", t_622AD4);

// SceneManager::callCurrentSceneRunOutAnimation $_115: __func::destroy_deallocate
static void t_622AF4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x622AF4, "SceneManager::callCurrentSceneRunOutAnimation $_115 __func::destroy_deallocate", t_622AF4);

// SceneManager::callCurrentSceneRunOutAnimation $_115: __func::target
static uint32_t t_622B30(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe530b0u ? self + 8 : 0; }
RW_FUNCTION(0x622B30, "SceneManager::callCurrentSceneRunOutAnimation $_115 __func::target", t_622B30);

// SceneManager::callCurrentSceneRunOutAnimation $_115: __func::target_type
static uint32_t t_622B50() { return 0xf98004u; }
RW_FUNCTION(0x622B50, "SceneManager::callCurrentSceneRunOutAnimation $_115 __func::target_type", t_622B50);

// SceneManager::openPartyBecomePopup $_114: __func::~__func
static uint32_t t_622ED0(uint32_t self) {
    st32(self, 0xf5f110u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x622ED0, "SceneManager::openPartyBecomePopup $_114 __func::~__func", t_622ED0);

// SceneManager::openPartyBecomePopup $_114: __func::~__func (deleting)
static void t_622F0C(uint32_t self) {
    st32(self, 0xf5f110u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x622F0C, "SceneManager::openPartyBecomePopup $_114 __func::~__func (deleting)", t_622F0C);

// SceneManager::openPartyBecomePopup $_114: __func::__clone()
static uint32_t t_622F4C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5f110u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 32 + 0);
        uint32_t v3 = ld32(self + 32 + 4);
        uint32_t v4 = ld32(self + 32 + 8);
        st32(v1 + 32 + 0, v2);
        st32(v1 + 32 + 4, v3);
        st32(v1 + 32 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x622F4C, "SceneManager::openPartyBecomePopup $_114 __func::__clone()", t_622F4C);

// SceneManager::openPartyBecomePopup $_114: __func::__clone(__base*)
static void t_622FA8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f110u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v1 = ld32(self + 32 + 0);
        uint32_t v2 = ld32(self + 32 + 4);
        uint32_t v3 = ld32(self + 32 + 8);
        st32(p + 32 + 0, v1);
        st32(p + 32 + 4, v2);
        st32(p + 32 + 8, v3);
    }
}
RW_FUNCTION(0x622FA8, "SceneManager::openPartyBecomePopup $_114 __func::__clone(__base*)", t_622FA8);

// SceneManager::openPartyBecomePopup $_114: __func::destroy
static void t_622FEC(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x622FEC, "SceneManager::openPartyBecomePopup $_114 __func::destroy", t_622FEC);

// SceneManager::openPartyBecomePopup $_114: __func::destroy_deallocate
static void t_62300C(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62300C, "SceneManager::openPartyBecomePopup $_114 __func::destroy_deallocate", t_62300C);

// SceneManager::openPartyBecomePopup $_114: __func::target
static uint32_t t_623120(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe531c0u ? self + 8 : 0; }
RW_FUNCTION(0x623120, "SceneManager::openPartyBecomePopup $_114 __func::target", t_623120);

// SceneManager::openPartyBecomePopup $_114: __func::target_type
static uint32_t t_623140() { return 0xf98018u; }
RW_FUNCTION(0x623140, "SceneManager::openPartyBecomePopup $_114 __func::target_type", t_623140);

// SceneManager::openPartyBecomePopup $_142 UlvE_: __func::~__func (deleting)
static void t_623154(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623154, "SceneManager::openPartyBecomePopup $_142 UlvE_ __func::~__func (deleting)", t_623154);

// SceneManager::openPartyBecomePopup $_142 UlvE_: __func::__clone()
static uint32_t t_62315C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5f13cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x62315C, "SceneManager::openPartyBecomePopup $_142 UlvE_ __func::__clone()", t_62315C);

// SceneManager::openPartyBecomePopup $_142 UlvE_: __func::__clone(__base*)
static void t_62318C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f13cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x62318C, "SceneManager::openPartyBecomePopup $_142 UlvE_ __func::__clone(__base*)", t_62318C);

// SceneManager::openPartyBecomePopup $_142 UlvE_: __func::destroy
static void t_6231B4(uint32_t self) {
}
RW_FUNCTION(0x6231B4, "SceneManager::openPartyBecomePopup $_142 UlvE_ __func::destroy", t_6231B4);

// SceneManager::openPartyBecomePopup $_142 UlvE_: __func::destroy_deallocate
static void t_6231B6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6231B6, "SceneManager::openPartyBecomePopup $_142 UlvE_ __func::destroy_deallocate", t_6231B6);

// SceneManager::openPartyBecomePopup $_142 UlvE_: __func::target
static uint32_t t_6231E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe532a0u ? self + 4 : 0; }
RW_FUNCTION(0x6231E8, "SceneManager::openPartyBecomePopup $_142 UlvE_ __func::target", t_6231E8);

// SceneManager::openPartyBecomePopup $_142 UlvE_: __func::target_type
static uint32_t t_623208() { return 0xf9802cu; }
RW_FUNCTION(0x623208, "SceneManager::openPartyBecomePopup $_142 UlvE_ __func::target_type", t_623208);

// SceneManager::openPartyNewPopup $_113: __func::~__func (deleting)
static void t_62321C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62321C, "SceneManager::openPartyNewPopup $_113 __func::~__func (deleting)", t_62321C);

// SceneManager::openPartyNewPopup $_113: __func::__clone()
static uint32_t t_623224(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f168u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x623224, "SceneManager::openPartyNewPopup $_113 __func::__clone()", t_623224);

// SceneManager::openPartyNewPopup $_113: __func::__clone(__base*)
static void t_623258(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f168u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x623258, "SceneManager::openPartyNewPopup $_113 __func::__clone(__base*)", t_623258);

// SceneManager::openPartyNewPopup $_113: __func::destroy
static void t_623280(uint32_t self) {
}
RW_FUNCTION(0x623280, "SceneManager::openPartyNewPopup $_113 __func::destroy", t_623280);

// SceneManager::openPartyNewPopup $_113: __func::destroy_deallocate
static void t_623282(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623282, "SceneManager::openPartyNewPopup $_113 __func::destroy_deallocate", t_623282);

// SceneManager::openPartyNewPopup $_113: __func::target
static uint32_t t_6232B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53380u ? self + 4 : 0; }
RW_FUNCTION(0x6232B0, "SceneManager::openPartyNewPopup $_113 __func::target", t_6232B0);

// SceneManager::openPartyNewPopup $_113: __func::target_type
static uint32_t t_6232D0() { return 0xf98040u; }
RW_FUNCTION(0x6232D0, "SceneManager::openPartyNewPopup $_113 __func::target_type", t_6232D0);

// SceneManager::openPartyNewPopup $_112: __func::~__func (deleting)
static void t_6232E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6232E4, "SceneManager::openPartyNewPopup $_112 __func::~__func (deleting)", t_6232E4);

// SceneManager::openPartyNewPopup $_112: __func::__clone()
static uint32_t t_6232EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f194u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6232EC, "SceneManager::openPartyNewPopup $_112 __func::__clone()", t_6232EC);

// SceneManager::openPartyNewPopup $_112: __func::__clone(__base*)
static void t_623320(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f194u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x623320, "SceneManager::openPartyNewPopup $_112 __func::__clone(__base*)", t_623320);

// SceneManager::openPartyNewPopup $_112: __func::destroy
static void t_623348(uint32_t self) {
}
RW_FUNCTION(0x623348, "SceneManager::openPartyNewPopup $_112 __func::destroy", t_623348);

// SceneManager::openPartyNewPopup $_112: __func::destroy_deallocate
static void t_62334A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62334A, "SceneManager::openPartyNewPopup $_112 __func::destroy_deallocate", t_62334A);

// SceneManager::openPartyNewPopup $_112: __func::target
static uint32_t t_6234B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53450u ? self + 4 : 0; }
RW_FUNCTION(0x6234B0, "SceneManager::openPartyNewPopup $_112 __func::target", t_6234B0);

// SceneManager::openPartyNewPopup $_112: __func::target_type
static uint32_t t_6234D0() { return 0xf98054u; }
RW_FUNCTION(0x6234D0, "SceneManager::openPartyNewPopup $_112 __func::target_type", t_6234D0);

// SceneManager::openPartyNewPopup $_112 UlbE_: __func::~__func (deleting)
static void t_6234E4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6234E4, "SceneManager::openPartyNewPopup $_112 UlbE_ __func::~__func (deleting)", t_6234E4);

// SceneManager::openPartyNewPopup $_112 UlbE_: __func::__clone()
static uint32_t t_6234EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f1c0u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6234EC, "SceneManager::openPartyNewPopup $_112 UlbE_ __func::__clone()", t_6234EC);

// SceneManager::openPartyNewPopup $_112 UlbE_: __func::__clone(__base*)
static void t_623520(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f1c0u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x623520, "SceneManager::openPartyNewPopup $_112 UlbE_ __func::__clone(__base*)", t_623520);

// SceneManager::openPartyNewPopup $_112 UlbE_: __func::destroy
static void t_623548(uint32_t self) {
}
RW_FUNCTION(0x623548, "SceneManager::openPartyNewPopup $_112 UlbE_ __func::destroy", t_623548);

// SceneManager::openPartyNewPopup $_112 UlbE_: __func::destroy_deallocate
static void t_62354A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62354A, "SceneManager::openPartyNewPopup $_112 UlbE_ __func::destroy_deallocate", t_62354A);

// SceneManager::openPartyNewPopup $_112 UlbE_: __func::target
static uint32_t t_623578(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53520u ? self + 4 : 0; }
RW_FUNCTION(0x623578, "SceneManager::openPartyNewPopup $_112 UlbE_ __func::target", t_623578);

// SceneManager::openPartyNewPopup $_112 UlbE_: __func::target_type
static uint32_t t_623598() { return 0xf98068u; }
RW_FUNCTION(0x623598, "SceneManager::openPartyNewPopup $_112 UlbE_ __func::target_type", t_623598);

// SceneManager::openPartyNewPopup $_111: __func::~__func (deleting)
static void t_6235AC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6235AC, "SceneManager::openPartyNewPopup $_111 __func::~__func (deleting)", t_6235AC);

// SceneManager::openPartyNewPopup $_111: __func::__clone()
static uint32_t t_6235B4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f1ecu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6235B4, "SceneManager::openPartyNewPopup $_111 __func::__clone()", t_6235B4);

// SceneManager::openPartyNewPopup $_111: __func::__clone(__base*)
static void t_6235E8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f1ecu);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6235E8, "SceneManager::openPartyNewPopup $_111 __func::__clone(__base*)", t_6235E8);

// SceneManager::openPartyNewPopup $_111: __func::destroy
static void t_623610(uint32_t self) {
}
RW_FUNCTION(0x623610, "SceneManager::openPartyNewPopup $_111 __func::destroy", t_623610);

// SceneManager::openPartyNewPopup $_111: __func::destroy_deallocate
static void t_623612(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623612, "SceneManager::openPartyNewPopup $_111 __func::destroy_deallocate", t_623612);

// SceneManager::openPartyNewPopup $_111: __func::target
static uint32_t t_623860(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe535f0u ? self + 4 : 0; }
RW_FUNCTION(0x623860, "SceneManager::openPartyNewPopup $_111 __func::target", t_623860);

// SceneManager::openPartyNewPopup $_111: __func::target_type
static uint32_t t_623880() { return 0xf9807cu; }
RW_FUNCTION(0x623880, "SceneManager::openPartyNewPopup $_111 __func::target_type", t_623880);

// SceneManager::openPartyNewPopup $_111 UlbE0_: __func::~__func (deleting)
static void t_623894(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623894, "SceneManager::openPartyNewPopup $_111 UlbE0_ __func::~__func (deleting)", t_623894);

// SceneManager::openPartyNewPopup $_111 UlbE0_: __func::__clone()
static uint32_t t_62389C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f218u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x62389C, "SceneManager::openPartyNewPopup $_111 UlbE0_ __func::__clone()", t_62389C);

// SceneManager::openPartyNewPopup $_111 UlbE0_: __func::__clone(__base*)
static void t_6238D0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f218u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x6238D0, "SceneManager::openPartyNewPopup $_111 UlbE0_ __func::__clone(__base*)", t_6238D0);

// SceneManager::openPartyNewPopup $_111 UlbE0_: __func::destroy
static void t_6238F8(uint32_t self) {
}
RW_FUNCTION(0x6238F8, "SceneManager::openPartyNewPopup $_111 UlbE0_ __func::destroy", t_6238F8);

// SceneManager::openPartyNewPopup $_111 UlbE0_: __func::destroy_deallocate
static void t_6238FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6238FA, "SceneManager::openPartyNewPopup $_111 UlbE0_ __func::destroy_deallocate", t_6238FA);

// SceneManager::openPartyNewPopup $_111 UlbE0_: __func::target
static uint32_t t_623928(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe536d0u ? self + 4 : 0; }
RW_FUNCTION(0x623928, "SceneManager::openPartyNewPopup $_111 UlbE0_ __func::target", t_623928);

// SceneManager::openPartyNewPopup $_111 UlbE0_: __func::target_type
static uint32_t t_623948() { return 0xf98090u; }
RW_FUNCTION(0x623948, "SceneManager::openPartyNewPopup $_111 UlbE0_ __func::target_type", t_623948);

// SceneManager::openPartyNewPopup $_111 UlbE_: __func::~__func (deleting)
static void t_62395C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62395C, "SceneManager::openPartyNewPopup $_111 UlbE_ __func::~__func (deleting)", t_62395C);

// SceneManager::openPartyNewPopup $_111 UlbE_: __func::__clone()
static uint32_t t_623964(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f244u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x623964, "SceneManager::openPartyNewPopup $_111 UlbE_ __func::__clone()", t_623964);

// SceneManager::openPartyNewPopup $_111 UlbE_: __func::__clone(__base*)
static void t_623998(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f244u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x623998, "SceneManager::openPartyNewPopup $_111 UlbE_ __func::__clone(__base*)", t_623998);

// SceneManager::openPartyNewPopup $_111 UlbE_: __func::destroy
static void t_6239C0(uint32_t self) {
}
RW_FUNCTION(0x6239C0, "SceneManager::openPartyNewPopup $_111 UlbE_ __func::destroy", t_6239C0);

// SceneManager::openPartyNewPopup $_111 UlbE_: __func::destroy_deallocate
static void t_6239C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6239C2, "SceneManager::openPartyNewPopup $_111 UlbE_ __func::destroy_deallocate", t_6239C2);

// SceneManager::openPartyNewPopup $_111 UlbE_: __func::target
static uint32_t t_623B28(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe537b0u ? self + 4 : 0; }
RW_FUNCTION(0x623B28, "SceneManager::openPartyNewPopup $_111 UlbE_ __func::target", t_623B28);

// SceneManager::openPartyNewPopup $_111 UlbE_: __func::target_type
static uint32_t t_623B48() { return 0xf980a4u; }
RW_FUNCTION(0x623B48, "SceneManager::openPartyNewPopup $_111 UlbE_ __func::target_type", t_623B48);

// SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_: __func::~__func (deleting)
static void t_623B5C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623B5C, "SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_ __func::~__func (deleting)", t_623B5C);

// SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_: __func::__clone()
static uint32_t t_623B64(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f270u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x623B64, "SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_ __func::__clone()", t_623B64);

// SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_: __func::__clone(__base*)
static void t_623B98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f270u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x623B98, "SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_ __func::__clone(__base*)", t_623B98);

// SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_: __func::destroy
static void t_623BC0(uint32_t self) {
}
RW_FUNCTION(0x623BC0, "SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_ __func::destroy", t_623BC0);

// SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_: __func::destroy_deallocate
static void t_623BC2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623BC2, "SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_ __func::destroy_deallocate", t_623BC2);

// SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_: __func::target
static uint32_t t_623BF0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53890u ? self + 4 : 0; }
RW_FUNCTION(0x623BF0, "SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_ __func::target", t_623BF0);

// SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_: __func::target_type
static uint32_t t_623C10() { return 0xf980b8u; }
RW_FUNCTION(0x623C10, "SceneManager::openPartyNewPopup $_111 UlbE_ UlbE_ __func::target_type", t_623C10);

// SceneManager::openPartyNewPopup $_110: __func::~__func (deleting)
static void t_623C24(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623C24, "SceneManager::openPartyNewPopup $_110 __func::~__func (deleting)", t_623C24);

// SceneManager::openPartyNewPopup $_110: __func::__clone()
static uint32_t t_623C2C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f29cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x623C2C, "SceneManager::openPartyNewPopup $_110 __func::__clone()", t_623C2C);

// SceneManager::openPartyNewPopup $_110: __func::__clone(__base*)
static void t_623C60(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f29cu);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x623C60, "SceneManager::openPartyNewPopup $_110 __func::__clone(__base*)", t_623C60);

// SceneManager::openPartyNewPopup $_110: __func::destroy
static void t_623C88(uint32_t self) {
}
RW_FUNCTION(0x623C88, "SceneManager::openPartyNewPopup $_110 __func::destroy", t_623C88);

// SceneManager::openPartyNewPopup $_110: __func::destroy_deallocate
static void t_623C8A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623C8A, "SceneManager::openPartyNewPopup $_110 __func::destroy_deallocate", t_623C8A);

// SceneManager::openPartyNewPopup $_110: __func::target
static uint32_t t_623F60(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53970u ? self + 4 : 0; }
RW_FUNCTION(0x623F60, "SceneManager::openPartyNewPopup $_110 __func::target", t_623F60);

// SceneManager::openPartyNewPopup $_110: __func::target_type
static uint32_t t_623F80() { return 0xf980ccu; }
RW_FUNCTION(0x623F80, "SceneManager::openPartyNewPopup $_110 __func::target_type", t_623F80);

// SceneManager::openPartyNewPopup $_110 UlbE1_: __func::~__func (deleting)
static void t_623F94(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623F94, "SceneManager::openPartyNewPopup $_110 UlbE1_ __func::~__func (deleting)", t_623F94);

// SceneManager::openPartyNewPopup $_110 UlbE1_: __func::__clone()
static uint32_t t_623F9C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f2c8u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x623F9C, "SceneManager::openPartyNewPopup $_110 UlbE1_ __func::__clone()", t_623F9C);

// SceneManager::openPartyNewPopup $_110 UlbE1_: __func::__clone(__base*)
static void t_623FD0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f2c8u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x623FD0, "SceneManager::openPartyNewPopup $_110 UlbE1_ __func::__clone(__base*)", t_623FD0);

// SceneManager::openPartyNewPopup $_110 UlbE1_: __func::destroy
static void t_623FF8(uint32_t self) {
}
RW_FUNCTION(0x623FF8, "SceneManager::openPartyNewPopup $_110 UlbE1_ __func::destroy", t_623FF8);

// SceneManager::openPartyNewPopup $_110 UlbE1_: __func::destroy_deallocate
static void t_623FFA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x623FFA, "SceneManager::openPartyNewPopup $_110 UlbE1_ __func::destroy_deallocate", t_623FFA);

// SceneManager::openPartyNewPopup $_110 UlbE1_: __func::target
static uint32_t t_624028(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53a50u ? self + 4 : 0; }
RW_FUNCTION(0x624028, "SceneManager::openPartyNewPopup $_110 UlbE1_ __func::target", t_624028);

// SceneManager::openPartyNewPopup $_110 UlbE1_: __func::target_type
static uint32_t t_624048() { return 0xf980e0u; }
RW_FUNCTION(0x624048, "SceneManager::openPartyNewPopup $_110 UlbE1_ __func::target_type", t_624048);

// SceneManager::openPartyNewPopup $_110 UlbE0_: __func::~__func (deleting)
static void t_62405C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62405C, "SceneManager::openPartyNewPopup $_110 UlbE0_ __func::~__func (deleting)", t_62405C);

// SceneManager::openPartyNewPopup $_110 UlbE0_: __func::__clone()
static uint32_t t_624064(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f2f4u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x624064, "SceneManager::openPartyNewPopup $_110 UlbE0_ __func::__clone()", t_624064);

// SceneManager::openPartyNewPopup $_110 UlbE0_: __func::__clone(__base*)
static void t_624098(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f2f4u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x624098, "SceneManager::openPartyNewPopup $_110 UlbE0_ __func::__clone(__base*)", t_624098);

// SceneManager::openPartyNewPopup $_110 UlbE0_: __func::destroy
static void t_6240C0(uint32_t self) {
}
RW_FUNCTION(0x6240C0, "SceneManager::openPartyNewPopup $_110 UlbE0_ __func::destroy", t_6240C0);

// SceneManager::openPartyNewPopup $_110 UlbE0_: __func::destroy_deallocate
static void t_6240C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6240C2, "SceneManager::openPartyNewPopup $_110 UlbE0_ __func::destroy_deallocate", t_6240C2);

// SceneManager::openPartyNewPopup $_110 UlbE0_: __func::target
static uint32_t t_624228(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53b40u ? self + 4 : 0; }
RW_FUNCTION(0x624228, "SceneManager::openPartyNewPopup $_110 UlbE0_ __func::target", t_624228);

// SceneManager::openPartyNewPopup $_110 UlbE0_: __func::target_type
static uint32_t t_624248() { return 0xf980f4u; }
RW_FUNCTION(0x624248, "SceneManager::openPartyNewPopup $_110 UlbE0_ __func::target_type", t_624248);

// SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_: __func::~__func (deleting)
static void t_62425C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62425C, "SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_ __func::~__func (deleting)", t_62425C);

// SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_: __func::__clone()
static uint32_t t_624264(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f320u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x624264, "SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_ __func::__clone()", t_624264);

// SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_: __func::__clone(__base*)
static void t_624298(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f320u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x624298, "SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_ __func::__clone(__base*)", t_624298);

// SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_: __func::destroy
static void t_6242C0(uint32_t self) {
}
RW_FUNCTION(0x6242C0, "SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_ __func::destroy", t_6242C0);

// SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_: __func::destroy_deallocate
static void t_6242C2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6242C2, "SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_ __func::destroy_deallocate", t_6242C2);

// SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_: __func::target
static uint32_t t_6242F0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53c30u ? self + 4 : 0; }
RW_FUNCTION(0x6242F0, "SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_ __func::target", t_6242F0);

// SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_: __func::target_type
static uint32_t t_624310() { return 0xf98108u; }
RW_FUNCTION(0x624310, "SceneManager::openPartyNewPopup $_110 UlbE0_ UlbE_ __func::target_type", t_624310);

// SceneManager::openPartyNewPopup $_110 UlbE_: __func::~__func (deleting)
static void t_624324(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x624324, "SceneManager::openPartyNewPopup $_110 UlbE_ __func::~__func (deleting)", t_624324);

// SceneManager::openPartyNewPopup $_110 UlbE_: __func::__clone()
static uint32_t t_62432C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f34cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x62432C, "SceneManager::openPartyNewPopup $_110 UlbE_ __func::__clone()", t_62432C);

// SceneManager::openPartyNewPopup $_110 UlbE_: __func::__clone(__base*)
static void t_624360(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f34cu);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x624360, "SceneManager::openPartyNewPopup $_110 UlbE_ __func::__clone(__base*)", t_624360);

// SceneManager::openPartyNewPopup $_110 UlbE_: __func::destroy
static void t_624388(uint32_t self) {
}
RW_FUNCTION(0x624388, "SceneManager::openPartyNewPopup $_110 UlbE_ __func::destroy", t_624388);

// SceneManager::openPartyNewPopup $_110 UlbE_: __func::destroy_deallocate
static void t_62438A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62438A, "SceneManager::openPartyNewPopup $_110 UlbE_ __func::destroy_deallocate", t_62438A);

// SceneManager::openPartyNewPopup $_110 UlbE_: __func::target
static uint32_t t_6245D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53d10u ? self + 4 : 0; }
RW_FUNCTION(0x6245D8, "SceneManager::openPartyNewPopup $_110 UlbE_ __func::target", t_6245D8);

// SceneManager::openPartyNewPopup $_110 UlbE_: __func::target_type
static uint32_t t_6245F8() { return 0xf9811cu; }
RW_FUNCTION(0x6245F8, "SceneManager::openPartyNewPopup $_110 UlbE_ __func::target_type", t_6245F8);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_: __func::~__func (deleting)
static void t_62460C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62460C, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_ __func::~__func (deleting)", t_62460C);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_: __func::__clone()
static uint32_t t_624614(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f378u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x624614, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_ __func::__clone()", t_624614);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_: __func::__clone(__base*)
static void t_624648(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f378u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x624648, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_ __func::__clone(__base*)", t_624648);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_: __func::destroy
static void t_624670(uint32_t self) {
}
RW_FUNCTION(0x624670, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_ __func::destroy", t_624670);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_: __func::destroy_deallocate
static void t_624672(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x624672, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_ __func::destroy_deallocate", t_624672);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_: __func::target
static uint32_t t_6246A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53df0u ? self + 4 : 0; }
RW_FUNCTION(0x6246A0, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_ __func::target", t_6246A0);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_: __func::target_type
static uint32_t t_6246C0() { return 0xf98130u; }
RW_FUNCTION(0x6246C0, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE0_ __func::target_type", t_6246C0);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_: __func::~__func (deleting)
static void t_6246D4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6246D4, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ __func::~__func (deleting)", t_6246D4);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_: __func::__clone()
static uint32_t t_6246DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f3a4u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6246DC, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ __func::__clone()", t_6246DC);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_: __func::__clone(__base*)
static void t_624710(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f3a4u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x624710, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ __func::__clone(__base*)", t_624710);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_: __func::destroy
static void t_624738(uint32_t self) {
}
RW_FUNCTION(0x624738, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ __func::destroy", t_624738);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_: __func::destroy_deallocate
static void t_62473A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62473A, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ __func::destroy_deallocate", t_62473A);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_: __func::target
static uint32_t t_6248A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53ee0u ? self + 4 : 0; }
RW_FUNCTION(0x6248A0, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ __func::target", t_6248A0);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_: __func::target_type
static uint32_t t_6248C0() { return 0xf98144u; }
RW_FUNCTION(0x6248C0, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ __func::target_type", t_6248C0);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_: __func::~__func (deleting)
static void t_6248D4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6248D4, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_ __func::~__func (deleting)", t_6248D4);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_: __func::__clone()
static uint32_t t_6248DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {24});
    if (v1) {
        st32(v1, 0xf5f3d0u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        uint32_t v5 = ld32(self + 4 + 12);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
        st32(v1 + 4 + 12, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
    }
    return v1;
}
RW_FUNCTION(0x6248DC, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_ __func::__clone()", t_6248DC);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_: __func::__clone(__base*)
static void t_624910(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f3d0u);
        uint32_t v1 = ld32(self + 4 + 0);
        uint32_t v2 = ld32(self + 4 + 4);
        uint32_t v3 = ld32(self + 4 + 8);
        uint32_t v4 = ld32(self + 4 + 12);
        st32(p + 4 + 0, v1);
        st32(p + 4 + 4, v2);
        st32(p + 4 + 8, v3);
        st32(p + 4 + 12, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x624910, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_ __func::__clone(__base*)", t_624910);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_: __func::destroy
static void t_624938(uint32_t self) {
}
RW_FUNCTION(0x624938, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_ __func::destroy", t_624938);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_: __func::destroy_deallocate
static void t_62493A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62493A, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_ __func::destroy_deallocate", t_62493A);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_: __func::target
static uint32_t t_624968(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe53fe0u ? self + 4 : 0; }
RW_FUNCTION(0x624968, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_ __func::target", t_624968);

// SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_: __func::target_type
static uint32_t t_624988() { return 0xf98158u; }
RW_FUNCTION(0x624988, "SceneManager::openPartyNewPopup $_110 UlbE_ UlbE_ UlbE_ __func::target_type", t_624988);

// SceneManager::openPartyNewPopup $_107: __func::~__func
static uint32_t t_62499C(uint32_t self) {
    st32(self, 0xf5f3fcu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x62499C, "SceneManager::openPartyNewPopup $_107 __func::~__func", t_62499C);

// SceneManager::openPartyNewPopup $_107: __func::~__func (deleting)
static void t_6249D8(uint32_t self) {
    st32(self, 0xf5f3fcu);
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6249D8, "SceneManager::openPartyNewPopup $_107 __func::~__func (deleting)", t_6249D8);

// SceneManager::openPartyNewPopup $_107: __func::__clone()
static uint32_t t_624A18(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5f3fcu);
        uint32_t v2 = ld32(self + 8 + 0);
        uint32_t v3 = ld32(self + 8 + 4);
        uint32_t v4 = ld32(self + 8 + 8);
        st32(v1 + 8 + 0, v2);
        st32(v1 + 8 + 4, v3);
        st32(v1 + 8 + 8, v4);
        call(0x4683cd, {v1 + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x624A18, "SceneManager::openPartyNewPopup $_107 __func::__clone()", t_624A18);

// SceneManager::openPartyNewPopup $_107: __func::__clone(__base*)
static void t_624A74(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f3fcu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8, v1);
        st32(p + 12, v2);
        st32(p + 16, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x624A74, "SceneManager::openPartyNewPopup $_107 __func::__clone(__base*)", t_624A74);

// SceneManager::openPartyNewPopup $_107: __func::destroy
static void t_624AB4(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x624AB4, "SceneManager::openPartyNewPopup $_107 __func::destroy", t_624AB4);

// SceneManager::openPartyNewPopup $_107: __func::destroy_deallocate
static void t_624AD4(uint32_t self) {
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x624AD4, "SceneManager::openPartyNewPopup $_107 __func::destroy_deallocate", t_624AD4);

// SceneManager::openPartyNewPopup $_107: __func::target
static uint32_t t_624B3C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe540d0u ? self + 8 : 0; }
RW_FUNCTION(0x624B3C, "SceneManager::openPartyNewPopup $_107 __func::target", t_624B3C);

// SceneManager::openPartyNewPopup $_107: __func::target_type
static uint32_t t_624B60() { return 0xf9816cu; }
RW_FUNCTION(0x624B60, "SceneManager::openPartyNewPopup $_107 __func::target_type", t_624B60);

// SceneManager::openPartyNewPopup $_106: __func::~__func (deleting)
static void t_624B74(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x624B74, "SceneManager::openPartyNewPopup $_106 __func::~__func (deleting)", t_624B74);

// SceneManager::openPartyNewPopup $_106: __func::__clone()
static uint32_t t_624B7C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f428u);
    }
    return v1;
}
RW_FUNCTION(0x624B7C, "SceneManager::openPartyNewPopup $_106 __func::__clone()", t_624B7C);

// SceneManager::openPartyNewPopup $_106: __func::__clone(__base*)
static void t_624BA0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f428u);
    }
}
RW_FUNCTION(0x624BA0, "SceneManager::openPartyNewPopup $_106 __func::__clone(__base*)", t_624BA0);

// SceneManager::openPartyNewPopup $_106: __func::destroy
static void t_624BBC(uint32_t self) {
}
RW_FUNCTION(0x624BBC, "SceneManager::openPartyNewPopup $_106 __func::destroy", t_624BBC);

// SceneManager::openPartyNewPopup $_106: __func::destroy_deallocate
static void t_624BBE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x624BBE, "SceneManager::openPartyNewPopup $_106 __func::destroy_deallocate", t_624BBE);

// SceneManager::openPartyNewPopup $_106: __func::target
static uint32_t t_624BCC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe541a0u ? self + 4 : 0; }
RW_FUNCTION(0x624BCC, "SceneManager::openPartyNewPopup $_106 __func::target", t_624BCC);

// SceneManager::openPartyNewPopup $_106: __func::target_type
static uint32_t t_624BEC() { return 0xf98180u; }
RW_FUNCTION(0x624BEC, "SceneManager::openPartyNewPopup $_106 __func::target_type", t_624BEC);

// SceneManager::openPartyNewPopup $_104: __func::~__func
static uint32_t t_624C00(uint32_t self) {
    st32(self, 0xf5f454u);
    uint32_t v1 = ld32(self + 28);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x624C00, "SceneManager::openPartyNewPopup $_104 __func::~__func", t_624C00);

// SceneManager::openPartyNewPopup $_104: __func::~__func (deleting)
static void t_624C34(uint32_t self) {
    st32(self, 0xf5f454u);
    uint32_t v1 = ld32(self + 28);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x624C34, "SceneManager::openPartyNewPopup $_104 __func::~__func (deleting)", t_624C34);

// SceneManager::openPartyNewPopup $_104: __func::__clone()
static uint32_t t_624C6C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {64});
    if (v1) {
        st32(v1, 0xf5f454u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
        uint32_t v4 = ld8(self + 12);
        st8(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
        }
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        uint32_t v8 = ld32(self + 28);
        st32(v1 + 28, v8);
        if (v8) {
            call(0xc8dbb5, {v8});   // sub_C8DBB4
        }
        uint32_t v9 = ld32(self + 32 + 0);
        uint32_t v10 = ld32(self + 32 + 4);
        uint32_t v11 = ld32(self + 32 + 8);
        uint32_t v12 = ld32(self + 32 + 12);
        st32(v1 + 32 + 0, v9);
        st32(v1 + 32 + 4, v10);
        st32(v1 + 32 + 8, v11);
        st32(v1 + 32 + 12, v12);
        uint32_t v13 = ld32(self + 48 + 0);
        uint32_t v14 = ld32(self + 48 + 4);
        uint32_t v15 = ld32(self + 48 + 8);
        uint32_t v16 = ld32(self + 48 + 12);
        st32(v1 + 48 + 0, v13);
        st32(v1 + 48 + 4, v14);
        st32(v1 + 48 + 8, v15);
        st32(v1 + 48 + 12, v16);
    }
    return v1;
}
RW_FUNCTION(0x624C6C, "SceneManager::openPartyNewPopup $_104 __func::__clone()", t_624C6C);

// SceneManager::openPartyNewPopup $_104: __func::__clone(__base*)
static void t_624CE0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f454u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
        uint32_t v3 = ld8(self + 12);
        st8(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 24);
        st32(p + 24, v6);
        uint32_t v7 = ld32(self + 28);
        st32(p + 28, v7);
        if (v7) {
            call(0xc8dbb5, {v7});   // sub_C8DBB4
        }
        uint32_t v8 = ld32(self + 32 + 0);
        uint32_t v9 = ld32(self + 32 + 4);
        uint32_t v10 = ld32(self + 32 + 8);
        uint32_t v11 = ld32(self + 32 + 12);
        st32(p + 32 + 0, v8);
        st32(p + 32 + 4, v9);
        st32(p + 32 + 8, v10);
        st32(p + 32 + 12, v11);
        uint32_t v12 = ld32(self + 48 + 0);
        uint32_t v13 = ld32(self + 48 + 4);
        uint32_t v14 = ld32(self + 48 + 8);
        uint32_t v15 = ld32(self + 48 + 12);
        st32(p + 48 + 0, v12);
        st32(p + 48 + 4, v13);
        st32(p + 48 + 8, v14);
        st32(p + 48 + 12, v15);
    }
}
RW_FUNCTION(0x624CE0, "SceneManager::openPartyNewPopup $_104 __func::__clone(__base*)", t_624CE0);

// SceneManager::openPartyNewPopup $_104: __func::destroy
static void t_624D48(uint32_t self) {
    uint32_t v1 = ld32(self + 28);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x624D48, "SceneManager::openPartyNewPopup $_104 __func::destroy", t_624D48);

// SceneManager::openPartyNewPopup $_104: __func::destroy_deallocate
static void t_624D64(uint32_t self) {
    uint32_t v1 = ld32(self + 28);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 20);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x624D64, "SceneManager::openPartyNewPopup $_104 __func::destroy_deallocate", t_624D64);

// SceneManager::openPartyNewPopup $_104: __func::target
static uint32_t t_624F24(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54290u ? self + 4 : 0; }
RW_FUNCTION(0x624F24, "SceneManager::openPartyNewPopup $_104 __func::target", t_624F24);

// SceneManager::openPartyNewPopup $_104: __func::target_type
static uint32_t t_624F44() { return 0xf98194u; }
RW_FUNCTION(0x624F44, "SceneManager::openPartyNewPopup $_104 __func::target_type", t_624F44);

// SceneManager::openPartyNewPopup $_103 UlvE_: __func::~__func
static uint32_t t_624F58(uint32_t self) {
    st32(self, 0xf5f480u);
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 16);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    return self;
}
RW_FUNCTION(0x624F58, "SceneManager::openPartyNewPopup $_103 UlvE_ __func::~__func", t_624F58);

// SceneManager::openPartyNewPopup $_103 UlvE_: __func::~__func (deleting)
static void t_624F8C(uint32_t self) {
    st32(self, 0xf5f480u);
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 16);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x624F8C, "SceneManager::openPartyNewPopup $_103 UlvE_ __func::~__func (deleting)", t_624F8C);

// SceneManager::openPartyNewPopup $_103 UlvE_: __func::__clone()
static uint32_t t_624FC4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {56});
    if (v1) {
        st32(v1, 0xf5f480u);
        uint32_t v2 = ld8(self + 8);
        st8(v1 + 8, v2);
        uint32_t v3 = ld32(self + 4);
        st32(v1 + 4, v3);
        uint32_t v4 = ld32(self + 12);
        st32(v1 + 12, v4);
        uint32_t v5 = ld32(self + 16);
        st32(v1 + 16, v5);
        if (v5) {
            call(0xc8dbb5, {v5});   // sub_C8DBB4
        }
        uint32_t v6 = ld32(self + 20);
        st32(v1 + 20, v6);
        uint32_t v7 = ld32(self + 24);
        st32(v1 + 24, v7);
        if (v7) {
            call(0xc8dbb5, {v7});   // sub_C8DBB4
        }
        uint32_t v8 = ld32(self + 28 + 0);
        uint32_t v9 = ld32(self + 28 + 4);
        uint32_t v10 = ld32(self + 28 + 8);
        uint32_t v11 = ld32(self + 28 + 12);
        st32(v1 + 28 + 0, v8);
        st32(v1 + 28 + 4, v9);
        st32(v1 + 28 + 8, v10);
        st32(v1 + 28 + 12, v11);
        uint32_t v12 = ld32(self + 44 + 0);
        uint32_t v13 = ld32(self + 44 + 4);
        uint32_t v14 = ld32(self + 44 + 8);
        st32(v1 + 44 + 0, v12);
        st32(v1 + 44 + 4, v13);
        st32(v1 + 44 + 8, v14);
    }
    return v1;
}
RW_FUNCTION(0x624FC4, "SceneManager::openPartyNewPopup $_103 UlvE_ __func::__clone()", t_624FC4);

// SceneManager::openPartyNewPopup $_103 UlvE_: __func::__clone(__base*)
static void t_625034(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f480u);
        uint32_t v1 = ld8(self + 8);
        st8(p + 8, v1);
        uint32_t v2 = ld32(self + 4);
        st32(p + 4, v2);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        uint32_t v4 = ld32(self + 16);
        st32(p + 16, v4);
        if (v4) {
            call(0xc8dbb5, {v4});   // sub_C8DBB4
        }
        uint32_t v5 = ld32(self + 20);
        st32(p + 20, v5);
        uint32_t v6 = ld32(self + 24);
        st32(p + 24, v6);
        if (v6) {
            call(0xc8dbb5, {v6});   // sub_C8DBB4
        }
        uint32_t v7 = ld32(self + 28 + 0);
        uint32_t v8 = ld32(self + 28 + 4);
        uint32_t v9 = ld32(self + 28 + 8);
        uint32_t v10 = ld32(self + 28 + 12);
        st32(p + 28 + 0, v7);
        st32(p + 28 + 4, v8);
        st32(p + 28 + 8, v9);
        st32(p + 28 + 12, v10);
        uint32_t v11 = ld32(self + 44 + 0);
        uint32_t v12 = ld32(self + 44 + 4);
        uint32_t v13 = ld32(self + 44 + 8);
        st32(p + 44 + 0, v11);
        st32(p + 44 + 4, v12);
        st32(p + 44 + 8, v13);
    }
}
RW_FUNCTION(0x625034, "SceneManager::openPartyNewPopup $_103 UlvE_ __func::__clone(__base*)", t_625034);

// SceneManager::openPartyNewPopup $_103 UlvE_: __func::destroy
static void t_625098(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 16);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
}
RW_FUNCTION(0x625098, "SceneManager::openPartyNewPopup $_103 UlvE_ __func::destroy", t_625098);

// SceneManager::openPartyNewPopup $_103 UlvE_: __func::destroy_deallocate
static void t_6250B4(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    if (v1) {
        call(0xc8dbd1, {v1});   // sub_C8DBD0
    }
    uint32_t v2 = ld32(self + 16);
    if (v2) {
        call(0xc8dbd1, {v2});   // sub_C8DBD0
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6250B4, "SceneManager::openPartyNewPopup $_103 UlvE_ __func::destroy_deallocate", t_6250B4);

// SceneManager::openPartyNewPopup $_103 UlvE_: __func::target
static uint32_t t_6254B4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54360u ? self + 4 : 0; }
RW_FUNCTION(0x6254B4, "SceneManager::openPartyNewPopup $_103 UlvE_ __func::target", t_6254B4);

// SceneManager::openPartyNewPopup $_103 UlvE_: __func::target_type
static uint32_t t_6254D4() { return 0xf981a8u; }
RW_FUNCTION(0x6254D4, "SceneManager::openPartyNewPopup $_103 UlvE_ __func::target_type", t_6254D4);

// SceneManager::openNoTapScreen $_102: __func::~__func (deleting)
static void t_6254E8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6254E8, "SceneManager::openNoTapScreen $_102 __func::~__func (deleting)", t_6254E8);

// SceneManager::openNoTapScreen $_102: __func::__clone()
static uint32_t t_6254F0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f4acu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6254F0, "SceneManager::openNoTapScreen $_102 __func::__clone()", t_6254F0);

// SceneManager::openNoTapScreen $_102: __func::__clone(__base*)
static void t_62551C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f4acu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x62551C, "SceneManager::openNoTapScreen $_102 __func::__clone(__base*)", t_62551C);

// SceneManager::openNoTapScreen $_102: __func::destroy
static void t_62553C(uint32_t self) {
}
RW_FUNCTION(0x62553C, "SceneManager::openNoTapScreen $_102 __func::destroy", t_62553C);

// SceneManager::openNoTapScreen $_102: __func::destroy_deallocate
static void t_62553E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62553E, "SceneManager::openNoTapScreen $_102 __func::destroy_deallocate", t_62553E);

// SceneManager::openNoTapScreen $_102: __func::target
static uint32_t t_62556C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54410u ? self + 4 : 0; }
RW_FUNCTION(0x62556C, "SceneManager::openNoTapScreen $_102 __func::target", t_62556C);

// SceneManager::openNoTapScreen $_102: __func::target_type
static uint32_t t_62558C() { return 0xf981bcu; }
RW_FUNCTION(0x62558C, "SceneManager::openNoTapScreen $_102 __func::target_type", t_62558C);

// SceneManager::openUnBlockPostPopup $_101: __func::~__func (deleting)
static void t_6255A0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6255A0, "SceneManager::openUnBlockPostPopup $_101 __func::~__func (deleting)", t_6255A0);

// SceneManager::openUnBlockPostPopup $_101: __func::__clone()
static uint32_t t_6255A8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf5f4d8u);
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
RW_FUNCTION(0x6255A8, "SceneManager::openUnBlockPostPopup $_101 __func::__clone()", t_6255A8);

// SceneManager::openUnBlockPostPopup $_101: __func::__clone(__base*)
static void t_6255E0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f4d8u);
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
RW_FUNCTION(0x6255E0, "SceneManager::openUnBlockPostPopup $_101 __func::__clone(__base*)", t_6255E0);

// SceneManager::openUnBlockPostPopup $_101: __func::destroy
static void t_62560C(uint32_t self) {
}
RW_FUNCTION(0x62560C, "SceneManager::openUnBlockPostPopup $_101 __func::destroy", t_62560C);

// SceneManager::openUnBlockPostPopup $_101: __func::destroy_deallocate
static void t_62560E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62560E, "SceneManager::openUnBlockPostPopup $_101 __func::destroy_deallocate", t_62560E);

// SceneManager::openUnBlockPostPopup $_101: __func::target
static uint32_t t_625718(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe544e0u ? self + 4 : 0; }
RW_FUNCTION(0x625718, "SceneManager::openUnBlockPostPopup $_101 __func::target", t_625718);

// SceneManager::openUnBlockPostPopup $_101: __func::target_type
static uint32_t t_625738() { return 0xf981d0u; }
RW_FUNCTION(0x625738, "SceneManager::openUnBlockPostPopup $_101 __func::target_type", t_625738);

// SceneManager::openUnBlockPostPopup $_141 UlvE_: __func::~__func (deleting)
static void t_62574C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62574C, "SceneManager::openUnBlockPostPopup $_141 UlvE_ __func::~__func (deleting)", t_62574C);

// SceneManager::openUnBlockPostPopup $_141 UlvE_: __func::__clone()
static uint32_t t_625754(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5f504u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x625754, "SceneManager::openUnBlockPostPopup $_141 UlvE_ __func::__clone()", t_625754);

// SceneManager::openUnBlockPostPopup $_141 UlvE_: __func::__clone(__base*)
static void t_625784(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f504u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x625784, "SceneManager::openUnBlockPostPopup $_141 UlvE_ __func::__clone(__base*)", t_625784);

// SceneManager::openUnBlockPostPopup $_141 UlvE_: __func::destroy
static void t_6257AC(uint32_t self) {
}
RW_FUNCTION(0x6257AC, "SceneManager::openUnBlockPostPopup $_141 UlvE_ __func::destroy", t_6257AC);

// SceneManager::openUnBlockPostPopup $_141 UlvE_: __func::destroy_deallocate
static void t_6257AE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6257AE, "SceneManager::openUnBlockPostPopup $_141 UlvE_ __func::destroy_deallocate", t_6257AE);

// SceneManager::openUnBlockPostPopup $_141 UlvE_: __func::target
static uint32_t t_6257E0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54600u ? self + 4 : 0; }
RW_FUNCTION(0x6257E0, "SceneManager::openUnBlockPostPopup $_141 UlvE_ __func::target", t_6257E0);

// SceneManager::openUnBlockPostPopup $_141 UlvE_: __func::target_type
static uint32_t t_625800() { return 0xf981e4u; }
RW_FUNCTION(0x625800, "SceneManager::openUnBlockPostPopup $_141 UlvE_ __func::target_type", t_625800);

// SceneManager::openUnBlockPostPopup $_100: __func::~__func (deleting)
static void t_625814(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x625814, "SceneManager::openUnBlockPostPopup $_100 __func::~__func (deleting)", t_625814);

// SceneManager::openUnBlockPostPopup $_100: __func::__clone()
static uint32_t t_62581C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f530u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x62581C, "SceneManager::openUnBlockPostPopup $_100 __func::__clone()", t_62581C);

// SceneManager::openUnBlockPostPopup $_100: __func::__clone(__base*)
static void t_625848(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f530u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x625848, "SceneManager::openUnBlockPostPopup $_100 __func::__clone(__base*)", t_625848);

// SceneManager::openUnBlockPostPopup $_100: __func::destroy
static void t_625868(uint32_t self) {
}
RW_FUNCTION(0x625868, "SceneManager::openUnBlockPostPopup $_100 __func::destroy", t_625868);

// SceneManager::openUnBlockPostPopup $_100: __func::destroy_deallocate
static void t_62586A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62586A, "SceneManager::openUnBlockPostPopup $_100 __func::destroy_deallocate", t_62586A);

// SceneManager::openUnBlockPostPopup $_100: __func::target
static uint32_t t_62587C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54720u ? self + 4 : 0; }
RW_FUNCTION(0x62587C, "SceneManager::openUnBlockPostPopup $_100 __func::target", t_62587C);

// SceneManager::openUnBlockPostPopup $_100: __func::target_type
static uint32_t t_62589C() { return 0xf981f8u; }
RW_FUNCTION(0x62589C, "SceneManager::openUnBlockPostPopup $_100 __func::target_type", t_62589C);

// SceneManager::openUnBlockPostPopup $_99: __func::target
static uint32_t t_6261B0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54860u ? self + 8 : 0; }
RW_FUNCTION(0x6261B0, "SceneManager::openUnBlockPostPopup $_99 __func::target", t_6261B0);

// SceneManager::openUnBlockPostPopup $_99: __func::target_type
static uint32_t t_6261D4() { return 0xf9820cu; }
RW_FUNCTION(0x6261D4, "SceneManager::openUnBlockPostPopup $_99 __func::target_type", t_6261D4);

// SceneManager::openUnBlockPostPopup $_98: __func::~__func
static uint32_t t_6262C0(uint32_t self) {
    st32(self, 0xf5f588u);
    uint32_t v1 = ld32(self + 136);
    functionDestroy(self + 120);
    uint32_t v2 = ld32(self + 104);
    functionDestroy(self + 88);
    uint32_t v3 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v4 = ld8(self + 28);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 36);
        call(fn::operator_delete, {v5});
    }
    return self;
}
RW_FUNCTION(0x6262C0, "SceneManager::openUnBlockPostPopup $_98 __func::~__func", t_6262C0);

// SceneManager::openUnBlockPostPopup $_98: __func::~__func (deleting)
static void t_626344(uint32_t self) {
    st32(self, 0xf5f588u);
    uint32_t v1 = ld32(self + 136);
    functionDestroy(self + 120);
    uint32_t v2 = ld32(self + 104);
    functionDestroy(self + 88);
    uint32_t v3 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v4 = ld8(self + 28);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 36);
        call(fn::operator_delete, {v5});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x626344, "SceneManager::openUnBlockPostPopup $_98 __func::~__func (deleting)", t_626344);

// SceneManager::openUnBlockPostPopup $_98: __func::destroy
static void t_62665C(uint32_t self) {
    uint32_t v1 = ld32(self + 136);
    functionDestroy(self + 120);
    uint32_t v2 = ld32(self + 104);
    functionDestroy(self + 88);
    uint32_t v3 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v4 = ld8(self + 28);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 36);
        call(fn::operator_delete, {v5});
    }
}
RW_FUNCTION(0x62665C, "SceneManager::openUnBlockPostPopup $_98 __func::destroy", t_62665C);

// SceneManager::openUnBlockPostPopup $_98: __func::destroy_deallocate
static void t_6266C8(uint32_t self) {
    uint32_t v1 = ld32(self + 136);
    functionDestroy(self + 120);
    uint32_t v2 = ld32(self + 104);
    functionDestroy(self + 88);
    uint32_t v3 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v4 = ld8(self + 28);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 36);
        call(fn::operator_delete, {v5});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6266C8, "SceneManager::openUnBlockPostPopup $_98 __func::destroy_deallocate", t_6266C8);

// SceneManager::openUnBlockPostPopup $_98: __func::target
static uint32_t t_626DA8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54990u ? self + 8 : 0; }
RW_FUNCTION(0x626DA8, "SceneManager::openUnBlockPostPopup $_98 __func::target", t_626DA8);

// SceneManager::openUnBlockPostPopup $_98: __func::target_type
static uint32_t t_626DCC() { return 0xf98220u; }
RW_FUNCTION(0x626DCC, "SceneManager::openUnBlockPostPopup $_98 __func::target_type", t_626DCC);

// SceneManager::openUnBlockPostPopup $_97: __func::~__func
static uint32_t t_626DE0(uint32_t self) {
    st32(self, 0xf5f5b4u);
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x626DE0, "SceneManager::openUnBlockPostPopup $_97 __func::~__func", t_626DE0);

// SceneManager::openUnBlockPostPopup $_97: __func::~__func (deleting)
static void t_626E38(uint32_t self) {
    st32(self, 0xf5f5b4u);
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x626E38, "SceneManager::openUnBlockPostPopup $_97 __func::~__func (deleting)", t_626E38);

// SceneManager::openUnBlockPostPopup $_97: __func::__clone(__base*)
static void t_626F34(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f5b4u);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v4 = ld32(self + 48);
        uint32_t v5 = ld32(self + 52);
        st32(p + 48, v4);
        st32(p + 52, v5);
        call(0x4683cd, {p + 56, self + 56});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x626F34, "SceneManager::openUnBlockPostPopup $_97 __func::__clone(__base*)", t_626F34);

// SceneManager::openUnBlockPostPopup $_97: __func::destroy
static void t_626FBC(uint32_t self) {
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x626FBC, "SceneManager::openUnBlockPostPopup $_97 __func::destroy", t_626FBC);

// SceneManager::openUnBlockPostPopup $_97: __func::destroy_deallocate
static void t_626FF8(uint32_t self) {
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x626FF8, "SceneManager::openUnBlockPostPopup $_97 __func::destroy_deallocate", t_626FF8);

// SceneManager::openUnBlockPostPopup $_97: __func::target
static uint32_t t_627484(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54ae0u ? self + 8 : 0; }
RW_FUNCTION(0x627484, "SceneManager::openUnBlockPostPopup $_97 __func::target", t_627484);

// SceneManager::openUnBlockPostPopup $_97: __func::target_type
static uint32_t t_6274A8() { return 0xf98234u; }
RW_FUNCTION(0x6274A8, "SceneManager::openUnBlockPostPopup $_97 __func::target_type", t_6274A8);

// SceneManager::openUnBlockPostPopup $_95: __func::~__func (deleting)
static void t_62755A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62755A, "SceneManager::openUnBlockPostPopup $_95 __func::~__func (deleting)", t_62755A);

// SceneManager::openUnBlockPostPopup $_95: __func::__clone()
static uint32_t t_627564(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f5e0u);
    }
    return v1;
}
RW_FUNCTION(0x627564, "SceneManager::openUnBlockPostPopup $_95 __func::__clone()", t_627564);

// SceneManager::openUnBlockPostPopup $_95: __func::__clone(__base*)
static void t_627588(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f5e0u);
    }
}
RW_FUNCTION(0x627588, "SceneManager::openUnBlockPostPopup $_95 __func::__clone(__base*)", t_627588);

// SceneManager::openUnBlockPostPopup $_95: __func::destroy
static void t_6275A4(uint32_t self) {
}
RW_FUNCTION(0x6275A4, "SceneManager::openUnBlockPostPopup $_95 __func::destroy", t_6275A4);

// SceneManager::openUnBlockPostPopup $_95: __func::destroy_deallocate
static void t_6275A6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6275A6, "SceneManager::openUnBlockPostPopup $_95 __func::destroy_deallocate", t_6275A6);

// SceneManager::openUnBlockPostPopup $_95: __func::target
static uint32_t t_6275C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54c20u ? self + 4 : 0; }
RW_FUNCTION(0x6275C8, "SceneManager::openUnBlockPostPopup $_95 __func::target", t_6275C8);

// SceneManager::openUnBlockPostPopup $_95: __func::target_type
static uint32_t t_6275E8() { return 0xf98248u; }
RW_FUNCTION(0x6275E8, "SceneManager::openUnBlockPostPopup $_95 __func::target_type", t_6275E8);

// SceneManager::openBlockListPopup $_94: __func::~__func (deleting)
static void t_6275FC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6275FC, "SceneManager::openBlockListPopup $_94 __func::~__func (deleting)", t_6275FC);

// SceneManager::openBlockListPopup $_94: __func::__clone()
static uint32_t t_627604(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf5f60cu);
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
RW_FUNCTION(0x627604, "SceneManager::openBlockListPopup $_94 __func::__clone()", t_627604);

// SceneManager::openBlockListPopup $_94: __func::__clone(__base*)
static void t_62763C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f60cu);
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
RW_FUNCTION(0x62763C, "SceneManager::openBlockListPopup $_94 __func::__clone(__base*)", t_62763C);

// SceneManager::openBlockListPopup $_94: __func::destroy
static void t_627668(uint32_t self) {
}
RW_FUNCTION(0x627668, "SceneManager::openBlockListPopup $_94 __func::destroy", t_627668);

// SceneManager::openBlockListPopup $_94: __func::destroy_deallocate
static void t_62766A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62766A, "SceneManager::openBlockListPopup $_94 __func::destroy_deallocate", t_62766A);

// SceneManager::openBlockListPopup $_94: __func::target
static uint32_t t_6277A8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54d00u ? self + 4 : 0; }
RW_FUNCTION(0x6277A8, "SceneManager::openBlockListPopup $_94 __func::target", t_6277A8);

// SceneManager::openBlockListPopup $_94: __func::target_type
static uint32_t t_6277C8() { return 0xf9825cu; }
RW_FUNCTION(0x6277C8, "SceneManager::openBlockListPopup $_94 __func::target_type", t_6277C8);

// SceneManager::openBlockListPopup $_140 UlvE_: __func::~__func (deleting)
static void t_6277DC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6277DC, "SceneManager::openBlockListPopup $_140 UlvE_ __func::~__func (deleting)", t_6277DC);

// SceneManager::openBlockListPopup $_140 UlvE_: __func::__clone()
static uint32_t t_6277E4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5f638u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x6277E4, "SceneManager::openBlockListPopup $_140 UlvE_ __func::__clone()", t_6277E4);

// SceneManager::openBlockListPopup $_140 UlvE_: __func::__clone(__base*)
static void t_627814(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f638u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x627814, "SceneManager::openBlockListPopup $_140 UlvE_ __func::__clone(__base*)", t_627814);

// SceneManager::openBlockListPopup $_140 UlvE_: __func::destroy
static void t_62783C(uint32_t self) {
}
RW_FUNCTION(0x62783C, "SceneManager::openBlockListPopup $_140 UlvE_ __func::destroy", t_62783C);

// SceneManager::openBlockListPopup $_140 UlvE_: __func::destroy_deallocate
static void t_62783E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62783E, "SceneManager::openBlockListPopup $_140 UlvE_ __func::destroy_deallocate", t_62783E);

// SceneManager::openBlockListPopup $_140 UlvE_: __func::target
static uint32_t t_627870(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54da0u ? self + 4 : 0; }
RW_FUNCTION(0x627870, "SceneManager::openBlockListPopup $_140 UlvE_ __func::target", t_627870);

// SceneManager::openBlockListPopup $_140 UlvE_: __func::target_type
static uint32_t t_627890() { return 0xf98270u; }
RW_FUNCTION(0x627890, "SceneManager::openBlockListPopup $_140 UlvE_ __func::target_type", t_627890);

// SceneManager::openBlockListPopup $_93: __func::~__func (deleting)
static void t_6278A4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6278A4, "SceneManager::openBlockListPopup $_93 __func::~__func (deleting)", t_6278A4);

// SceneManager::openBlockListPopup $_93: __func::__clone()
static uint32_t t_6278AC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f664u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6278AC, "SceneManager::openBlockListPopup $_93 __func::__clone()", t_6278AC);

// SceneManager::openBlockListPopup $_93: __func::__clone(__base*)
static void t_6278D8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f664u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6278D8, "SceneManager::openBlockListPopup $_93 __func::__clone(__base*)", t_6278D8);

// SceneManager::openBlockListPopup $_93: __func::destroy
static void t_6278F8(uint32_t self) {
}
RW_FUNCTION(0x6278F8, "SceneManager::openBlockListPopup $_93 __func::destroy", t_6278F8);

// SceneManager::openBlockListPopup $_93: __func::destroy_deallocate
static void t_6278FA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6278FA, "SceneManager::openBlockListPopup $_93 __func::destroy_deallocate", t_6278FA);

// SceneManager::openBlockListPopup $_93: __func::target
static uint32_t t_62790C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54e50u ? self + 4 : 0; }
RW_FUNCTION(0x62790C, "SceneManager::openBlockListPopup $_93 __func::target", t_62790C);

// SceneManager::openBlockListPopup $_93: __func::target_type
static uint32_t t_62792C() { return 0xf98284u; }
RW_FUNCTION(0x62792C, "SceneManager::openBlockListPopup $_93 __func::target_type", t_62792C);

// SceneManager::openBlockListPopup $_92: __func::~__func (deleting)
static void t_627940(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x627940, "SceneManager::openBlockListPopup $_92 __func::~__func (deleting)", t_627940);

// SceneManager::openBlockListPopup $_92: __func::__clone()
static uint32_t t_627948(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5f690u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x627948, "SceneManager::openBlockListPopup $_92 __func::__clone()", t_627948);

// SceneManager::openBlockListPopup $_92: __func::__clone(__base*)
static void t_627978(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f690u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x627978, "SceneManager::openBlockListPopup $_92 __func::__clone(__base*)", t_627978);

// SceneManager::openBlockListPopup $_92: __func::destroy
static void t_62799C(uint32_t self) {
}
RW_FUNCTION(0x62799C, "SceneManager::openBlockListPopup $_92 __func::destroy", t_62799C);

// SceneManager::openBlockListPopup $_92: __func::destroy_deallocate
static void t_62799E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62799E, "SceneManager::openBlockListPopup $_92 __func::destroy_deallocate", t_62799E);

// SceneManager::openBlockListPopup $_92: __func::target
static uint32_t t_627C9C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54ef0u ? self + 4 : 0; }
RW_FUNCTION(0x627C9C, "SceneManager::openBlockListPopup $_92 __func::target", t_627C9C);

// SceneManager::openBlockListPopup $_92: __func::target_type
static uint32_t t_627CBC() { return 0xf98298u; }
RW_FUNCTION(0x627CBC, "SceneManager::openBlockListPopup $_92 __func::target_type", t_627CBC);

// SceneManager::openBlockListPopup $_91: __func::~__func (deleting)
static void t_627EDA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x627EDA, "SceneManager::openBlockListPopup $_91 __func::~__func (deleting)", t_627EDA);

// SceneManager::openBlockListPopup $_91: __func::__clone()
static uint32_t t_627EE4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f6bcu);
    }
    return v1;
}
RW_FUNCTION(0x627EE4, "SceneManager::openBlockListPopup $_91 __func::__clone()", t_627EE4);

// SceneManager::openBlockListPopup $_91: __func::__clone(__base*)
static void t_627F08(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f6bcu);
    }
}
RW_FUNCTION(0x627F08, "SceneManager::openBlockListPopup $_91 __func::__clone(__base*)", t_627F08);

// SceneManager::openBlockListPopup $_91: __func::destroy
static void t_627F24(uint32_t self) {
}
RW_FUNCTION(0x627F24, "SceneManager::openBlockListPopup $_91 __func::destroy", t_627F24);

// SceneManager::openBlockListPopup $_91: __func::destroy_deallocate
static void t_627F26(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x627F26, "SceneManager::openBlockListPopup $_91 __func::destroy_deallocate", t_627F26);

// SceneManager::openBlockListPopup $_91: __func::target
static uint32_t t_627F48(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe54fb0u ? self + 4 : 0; }
RW_FUNCTION(0x627F48, "SceneManager::openBlockListPopup $_91 __func::target", t_627F48);

// SceneManager::openBlockListPopup $_91: __func::target_type
static uint32_t t_627F68() { return 0xf982acu; }
RW_FUNCTION(0x627F68, "SceneManager::openBlockListPopup $_91 __func::target_type", t_627F68);

// SceneManager::openReportSelectPopup $_90: __func::~__func
static uint32_t t_627F7C(uint32_t self) {
    st32(self, 0xf5f6e8u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x627F7C, "SceneManager::openReportSelectPopup $_90 __func::~__func", t_627F7C);

// SceneManager::openReportSelectPopup $_90: __func::~__func (deleting)
static void t_627FB8(uint32_t self) {
    st32(self, 0xf5f6e8u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x627FB8, "SceneManager::openReportSelectPopup $_90 __func::~__func (deleting)", t_627FB8);

// SceneManager::openReportSelectPopup $_90: __func::destroy
static void t_628098(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x628098, "SceneManager::openReportSelectPopup $_90 __func::destroy", t_628098);

// SceneManager::openReportSelectPopup $_90: __func::destroy_deallocate
static void t_6280B8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6280B8, "SceneManager::openReportSelectPopup $_90 __func::destroy_deallocate", t_6280B8);

// SceneManager::openReportSelectPopup $_90: __func::target
static uint32_t t_6281D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55060u ? self + 8 : 0; }
RW_FUNCTION(0x6281D8, "SceneManager::openReportSelectPopup $_90 __func::target", t_6281D8);

// SceneManager::openReportSelectPopup $_90: __func::target_type
static uint32_t t_6281F8() { return 0xf982c0u; }
RW_FUNCTION(0x6281F8, "SceneManager::openReportSelectPopup $_90 __func::target_type", t_6281F8);

// SceneManager::openReportSelectPopup $_139 UlvE_: __func::~__func (deleting)
static void t_62820C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62820C, "SceneManager::openReportSelectPopup $_139 UlvE_ __func::~__func (deleting)", t_62820C);

// SceneManager::openReportSelectPopup $_139 UlvE_: __func::__clone()
static uint32_t t_628214(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5f714u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x628214, "SceneManager::openReportSelectPopup $_139 UlvE_ __func::__clone()", t_628214);

// SceneManager::openReportSelectPopup $_139 UlvE_: __func::__clone(__base*)
static void t_628244(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f714u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x628244, "SceneManager::openReportSelectPopup $_139 UlvE_ __func::__clone(__base*)", t_628244);

// SceneManager::openReportSelectPopup $_139 UlvE_: __func::destroy
static void t_62826C(uint32_t self) {
}
RW_FUNCTION(0x62826C, "SceneManager::openReportSelectPopup $_139 UlvE_ __func::destroy", t_62826C);

// SceneManager::openReportSelectPopup $_139 UlvE_: __func::destroy_deallocate
static void t_62826E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62826E, "SceneManager::openReportSelectPopup $_139 UlvE_ __func::destroy_deallocate", t_62826E);

// SceneManager::openReportSelectPopup $_139 UlvE_: __func::target
static uint32_t t_6282A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55140u ? self + 4 : 0; }
RW_FUNCTION(0x6282A0, "SceneManager::openReportSelectPopup $_139 UlvE_ __func::target", t_6282A0);

// SceneManager::openReportSelectPopup $_139 UlvE_: __func::target_type
static uint32_t t_6282C0() { return 0xf982d4u; }
RW_FUNCTION(0x6282C0, "SceneManager::openReportSelectPopup $_139 UlvE_ __func::target_type", t_6282C0);

// SceneManager::openReportSelectPopup $_89: __func::~__func (deleting)
static void t_6282D4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6282D4, "SceneManager::openReportSelectPopup $_89 __func::~__func (deleting)", t_6282D4);

// SceneManager::openReportSelectPopup $_89: __func::__clone()
static uint32_t t_6282DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f740u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6282DC, "SceneManager::openReportSelectPopup $_89 __func::__clone()", t_6282DC);

// SceneManager::openReportSelectPopup $_89: __func::__clone(__base*)
static void t_628308(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f740u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x628308, "SceneManager::openReportSelectPopup $_89 __func::__clone(__base*)", t_628308);

// SceneManager::openReportSelectPopup $_89: __func::destroy
static void t_628328(uint32_t self) {
}
RW_FUNCTION(0x628328, "SceneManager::openReportSelectPopup $_89 __func::destroy", t_628328);

// SceneManager::openReportSelectPopup $_89: __func::destroy_deallocate
static void t_62832A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62832A, "SceneManager::openReportSelectPopup $_89 __func::destroy_deallocate", t_62832A);

// SceneManager::openReportSelectPopup $_89: __func::target
static uint32_t t_62833C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55220u ? self + 4 : 0; }
RW_FUNCTION(0x62833C, "SceneManager::openReportSelectPopup $_89 __func::target", t_62833C);

// SceneManager::openReportSelectPopup $_89: __func::target_type
static uint32_t t_62835C() { return 0xf982e8u; }
RW_FUNCTION(0x62835C, "SceneManager::openReportSelectPopup $_89 __func::target_type", t_62835C);

// SceneManager::openReportSelectPopup $_88: __func::~__func
static uint32_t t_628370(uint32_t self) {
    st32(self, 0xf5f76cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x628370, "SceneManager::openReportSelectPopup $_88 __func::~__func", t_628370);

// SceneManager::openReportSelectPopup $_88: __func::~__func (deleting)
static void t_6283AC(uint32_t self) {
    st32(self, 0xf5f76cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6283AC, "SceneManager::openReportSelectPopup $_88 __func::~__func (deleting)", t_6283AC);

// SceneManager::openReportSelectPopup $_88: __func::destroy
static void t_628484(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x628484, "SceneManager::openReportSelectPopup $_88 __func::destroy", t_628484);

// SceneManager::openReportSelectPopup $_88: __func::destroy_deallocate
static void t_6284A4(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6284A4, "SceneManager::openReportSelectPopup $_88 __func::destroy_deallocate", t_6284A4);

// SceneManager::openReportSelectPopup $_88: __func::target
static uint32_t t_6284FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe552f0u ? self + 8 : 0; }
RW_FUNCTION(0x6284FC, "SceneManager::openReportSelectPopup $_88 __func::target", t_6284FC);

// SceneManager::openReportSelectPopup $_88: __func::target_type
static uint32_t t_628520() { return 0xf982fcu; }
RW_FUNCTION(0x628520, "SceneManager::openReportSelectPopup $_88 __func::target_type", t_628520);

// SceneManager::openReportSelectPopup $_87: __func::~__func (deleting)
static void t_628534(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x628534, "SceneManager::openReportSelectPopup $_87 __func::~__func (deleting)", t_628534);

// SceneManager::openReportSelectPopup $_87: __func::__clone()
static uint32_t t_62853C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f798u);
    }
    return v1;
}
RW_FUNCTION(0x62853C, "SceneManager::openReportSelectPopup $_87 __func::__clone()", t_62853C);

// SceneManager::openReportSelectPopup $_87: __func::__clone(__base*)
static void t_628560(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f798u);
    }
}
RW_FUNCTION(0x628560, "SceneManager::openReportSelectPopup $_87 __func::__clone(__base*)", t_628560);

// SceneManager::openReportSelectPopup $_87: __func::destroy
static void t_62857C(uint32_t self) {
}
RW_FUNCTION(0x62857C, "SceneManager::openReportSelectPopup $_87 __func::destroy", t_62857C);

// SceneManager::openReportSelectPopup $_87: __func::destroy_deallocate
static void t_62857E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62857E, "SceneManager::openReportSelectPopup $_87 __func::destroy_deallocate", t_62857E);

// SceneManager::openReportSelectPopup $_87: __func::target
static uint32_t t_6285A0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe553f0u ? self + 4 : 0; }
RW_FUNCTION(0x6285A0, "SceneManager::openReportSelectPopup $_87 __func::target", t_6285A0);

// SceneManager::openReportSelectPopup $_87: __func::target_type
static uint32_t t_6285C0() { return 0xf98310u; }
RW_FUNCTION(0x6285C0, "SceneManager::openReportSelectPopup $_87 __func::target_type", t_6285C0);

// SceneManager::openReportPopup $_86: __func::~__func (deleting)
static void t_6285D4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6285D4, "SceneManager::openReportPopup $_86 __func::~__func (deleting)", t_6285D4);

// SceneManager::openReportPopup $_86: __func::__clone()
static uint32_t t_6285DC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5f7c4u);
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
        st32(v1 + 20 + 0, v6);
        st32(v1 + 20 + 4, v7);
        st32(v1 + 20 + 8, v8);
    }
    return v1;
}
RW_FUNCTION(0x6285DC, "SceneManager::openReportPopup $_86 __func::__clone()", t_6285DC);

// SceneManager::openReportPopup $_86: __func::__clone(__base*)
static void t_628618(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f7c4u);
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
        uint32_t v7 = ld32(self + 28);
        st32(p + 28, v7);
        st32(p + 24, v6);
        st32(p + 20, v5);
    }
}
RW_FUNCTION(0x628618, "SceneManager::openReportPopup $_86 __func::__clone(__base*)", t_628618);

// SceneManager::openReportPopup $_86: __func::destroy
static void t_628648(uint32_t self) {
}
RW_FUNCTION(0x628648, "SceneManager::openReportPopup $_86 __func::destroy", t_628648);

// SceneManager::openReportPopup $_86: __func::destroy_deallocate
static void t_62864A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62864A, "SceneManager::openReportPopup $_86 __func::destroy_deallocate", t_62864A);

// SceneManager::openReportPopup $_86: __func::target
static uint32_t t_628784(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe554d0u ? self + 4 : 0; }
RW_FUNCTION(0x628784, "SceneManager::openReportPopup $_86 __func::target", t_628784);

// SceneManager::openReportPopup $_86: __func::target_type
static uint32_t t_6287A4() { return 0xf98324u; }
RW_FUNCTION(0x6287A4, "SceneManager::openReportPopup $_86 __func::target_type", t_6287A4);

// SceneManager::openReportPopup $_137 UlvE_: __func::~__func (deleting)
static void t_6287B8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6287B8, "SceneManager::openReportPopup $_137 UlvE_ __func::~__func (deleting)", t_6287B8);

// SceneManager::openReportPopup $_137 UlvE_: __func::__clone()
static uint32_t t_6287C0(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf5f7f0u);
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
RW_FUNCTION(0x6287C0, "SceneManager::openReportPopup $_137 UlvE_ __func::__clone()", t_6287C0);

// SceneManager::openReportPopup $_137 UlvE_: __func::__clone(__base*)
static void t_6287F8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f7f0u);
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
RW_FUNCTION(0x6287F8, "SceneManager::openReportPopup $_137 UlvE_ __func::__clone(__base*)", t_6287F8);

// SceneManager::openReportPopup $_137 UlvE_: __func::destroy
static void t_628824(uint32_t self) {
}
RW_FUNCTION(0x628824, "SceneManager::openReportPopup $_137 UlvE_ __func::destroy", t_628824);

// SceneManager::openReportPopup $_137 UlvE_: __func::destroy_deallocate
static void t_628826(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x628826, "SceneManager::openReportPopup $_137 UlvE_ __func::destroy_deallocate", t_628826);

// SceneManager::openReportPopup $_137 UlvE_: __func::target
static uint32_t t_628978(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe555d0u ? self + 4 : 0; }
RW_FUNCTION(0x628978, "SceneManager::openReportPopup $_137 UlvE_ __func::target", t_628978);

// SceneManager::openReportPopup $_137 UlvE_: __func::target_type
static uint32_t t_628998() { return 0xf98338u; }
RW_FUNCTION(0x628998, "SceneManager::openReportPopup $_137 UlvE_ __func::target_type", t_628998);

// SceneManager::openReportPopup $_85: __func::~__func (deleting)
static void t_6289AC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6289AC, "SceneManager::openReportPopup $_85 __func::~__func (deleting)", t_6289AC);

// SceneManager::openReportPopup $_85: __func::__clone()
static uint32_t t_6289B4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f81cu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x6289B4, "SceneManager::openReportPopup $_85 __func::__clone()", t_6289B4);

// SceneManager::openReportPopup $_85: __func::__clone(__base*)
static void t_6289E0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f81cu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x6289E0, "SceneManager::openReportPopup $_85 __func::__clone(__base*)", t_6289E0);

// SceneManager::openReportPopup $_85: __func::destroy
static void t_628A00(uint32_t self) {
}
RW_FUNCTION(0x628A00, "SceneManager::openReportPopup $_85 __func::destroy", t_628A00);

// SceneManager::openReportPopup $_85: __func::destroy_deallocate
static void t_628A02(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x628A02, "SceneManager::openReportPopup $_85 __func::destroy_deallocate", t_628A02);

// SceneManager::openReportPopup $_85: __func::target
static uint32_t t_628A14(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe556d0u ? self + 4 : 0; }
RW_FUNCTION(0x628A14, "SceneManager::openReportPopup $_85 __func::target", t_628A14);

// SceneManager::openReportPopup $_85: __func::target_type
static uint32_t t_628A34() { return 0xf9834cu; }
RW_FUNCTION(0x628A34, "SceneManager::openReportPopup $_85 __func::target_type", t_628A34);

// SceneManager::openReportPopup $_84: __func::~__func
static uint32_t t_628A48(uint32_t self) {
    st32(self, 0xf5f848u);
    uint32_t v1 = ld32(self + 184);
    functionDestroy(self + 168);
    uint32_t v2 = ld32(self + 152);
    functionDestroy(self + 136);
    uint32_t v3 = ld32(self + 112);
    functionDestroy(self + 96);
    uint32_t v4 = ld8(self + 68);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 76);
        call(fn::operator_delete, {v5});
    }
    uint32_t v6 = ld8(self + 44);
    if (v6 & 1) {
        uint32_t v7 = ld32(self + 52);
        call(fn::operator_delete, {v7});
    }
    return self;
}
RW_FUNCTION(0x628A48, "SceneManager::openReportPopup $_84 __func::~__func", t_628A48);

// SceneManager::openReportPopup $_84: __func::~__func (deleting)
static void t_628AE0(uint32_t self) {
    st32(self, 0xf5f848u);
    uint32_t v1 = ld32(self + 184);
    functionDestroy(self + 168);
    uint32_t v2 = ld32(self + 152);
    functionDestroy(self + 136);
    uint32_t v3 = ld32(self + 112);
    functionDestroy(self + 96);
    uint32_t v4 = ld8(self + 68);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 76);
        call(fn::operator_delete, {v5});
    }
    uint32_t v6 = ld8(self + 44);
    if (v6 & 1) {
        uint32_t v7 = ld32(self + 52);
        call(fn::operator_delete, {v7});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x628AE0, "SceneManager::openReportPopup $_84 __func::~__func (deleting)", t_628AE0);

// SceneManager::openReportPopup $_84: __func::destroy
static void t_628EFC(uint32_t self) {
    uint32_t v1 = ld32(self + 184);
    functionDestroy(self + 168);
    uint32_t v2 = ld32(self + 152);
    functionDestroy(self + 136);
    uint32_t v3 = ld32(self + 112);
    functionDestroy(self + 96);
    uint32_t v4 = ld8(self + 68);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 76);
        call(fn::operator_delete, {v5});
    }
    uint32_t v6 = ld8(self + 44);
    if (v6 & 1) {
        uint32_t v7 = ld32(self + 52);
        call(fn::operator_delete, {v7});
    }
}
RW_FUNCTION(0x628EFC, "SceneManager::openReportPopup $_84 __func::destroy", t_628EFC);

// SceneManager::openReportPopup $_84: __func::destroy_deallocate
static void t_628F7A(uint32_t self) {
    uint32_t v1 = ld32(self + 184);
    functionDestroy(self + 168);
    uint32_t v2 = ld32(self + 152);
    functionDestroy(self + 136);
    uint32_t v3 = ld32(self + 112);
    functionDestroy(self + 96);
    uint32_t v4 = ld8(self + 68);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 76);
        call(fn::operator_delete, {v5});
    }
    uint32_t v6 = ld8(self + 44);
    if (v6 & 1) {
        uint32_t v7 = ld32(self + 52);
        call(fn::operator_delete, {v7});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x628F7A, "SceneManager::openReportPopup $_84 __func::destroy_deallocate", t_628F7A);

// SceneManager::openReportPopup $_84: __func::target
static uint32_t t_629560(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe557f0u ? self + 8 : 0; }
RW_FUNCTION(0x629560, "SceneManager::openReportPopup $_84 __func::target", t_629560);

// SceneManager::openReportPopup $_84: __func::target_type
static uint32_t t_629584() { return 0xf98360u; }
RW_FUNCTION(0x629584, "SceneManager::openReportPopup $_84 __func::target_type", t_629584);

// SceneManager::openReportPopup $_83: __func::~__func
static uint32_t t_62965C(uint32_t self) {
    st32(self, 0xf5f874u);
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x62965C, "SceneManager::openReportPopup $_83 __func::~__func", t_62965C);

// SceneManager::openReportPopup $_83: __func::~__func (deleting)
static void t_6296B4(uint32_t self) {
    st32(self, 0xf5f874u);
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x6296B4, "SceneManager::openReportPopup $_83 __func::~__func (deleting)", t_6296B4);

// SceneManager::openReportPopup $_83: __func::__clone(__base*)
static void t_6297B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f874u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x4683cd, {p + 16, self + 16});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v2 = ld32(self + 40);
        uint32_t v3 = ld32(self + 44);
        st32(p + 44, v3);
        st32(p + 40, v2);
        uint32_t v4 = ld32(self + 48);
        st32(p + 48, v4);
        call(0x4683cd, {p + 56, self + 56});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v5 = ld32(self + 48);
        st32(p + 48, v5);
    }
}
RW_FUNCTION(0x6297B0, "SceneManager::openReportPopup $_83 __func::__clone(__base*)", t_6297B0);

// SceneManager::openReportPopup $_83: __func::destroy
static void t_629838(uint32_t self) {
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x629838, "SceneManager::openReportPopup $_83 __func::destroy", t_629838);

// SceneManager::openReportPopup $_83: __func::destroy_deallocate
static void t_629874(uint32_t self) {
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x629874, "SceneManager::openReportPopup $_83 __func::destroy_deallocate", t_629874);

// SceneManager::openReportPopup $_83: __func::target
static uint32_t t_629BD4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55900u ? self + 8 : 0; }
RW_FUNCTION(0x629BD4, "SceneManager::openReportPopup $_83 __func::target", t_629BD4);

// SceneManager::openReportPopup $_83: __func::target_type
static uint32_t t_629BF8() { return 0xf98374u; }
RW_FUNCTION(0x629BF8, "SceneManager::openReportPopup $_83 __func::target_type", t_629BF8);

// SceneManager::openReportPopup $_82: __func::~__func (deleting)
static void t_629C0C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x629C0C, "SceneManager::openReportPopup $_82 __func::~__func (deleting)", t_629C0C);

// SceneManager::openReportPopup $_82: __func::__clone()
static uint32_t t_629C14(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {60});
    if (v1) {
        st32(v1, 0xf5f8a0u);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        uint32_t v13 = ld32(self + 36 + 12);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
        st32(v1 + 36 + 12, v13);
        uint32_t v14 = ld32(self + 52);
        uint32_t v15 = ld32(self + 56);
        st32(v1 + 56, v15);
        st32(v1 + 52, v14);
    }
    return v1;
}
RW_FUNCTION(0x629C14, "SceneManager::openReportPopup $_82 __func::__clone()", t_629C14);

// SceneManager::openReportPopup $_82: __func::__clone(__base*)
static void t_629C64(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f8a0u);
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
        uint32_t v9 = ld32(self + 36 + 0);
        uint32_t v10 = ld32(self + 36 + 4);
        uint32_t v11 = ld32(self + 36 + 8);
        uint32_t v12 = ld32(self + 36 + 12);
        st32(p + 36 + 0, v9);
        st32(p + 36 + 4, v10);
        st32(p + 36 + 8, v11);
        st32(p + 36 + 12, v12);
        uint32_t v13 = ld32(self + 52);
        uint32_t v14 = ld32(self + 56);
        st32(p + 56, v14);
        st32(p + 52, v13);
    }
}
RW_FUNCTION(0x629C64, "SceneManager::openReportPopup $_82 __func::__clone(__base*)", t_629C64);

// SceneManager::openReportPopup $_82: __func::destroy
static void t_629CA8(uint32_t self) {
}
RW_FUNCTION(0x629CA8, "SceneManager::openReportPopup $_82 __func::destroy", t_629CA8);

// SceneManager::openReportPopup $_82: __func::destroy_deallocate
static void t_629CAA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x629CAA, "SceneManager::openReportPopup $_82 __func::destroy_deallocate", t_629CAA);

// SceneManager::openReportPopup $_82: __func::target
static uint32_t t_629E14(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55a20u ? self + 4 : 0; }
RW_FUNCTION(0x629E14, "SceneManager::openReportPopup $_82 __func::target", t_629E14);

// SceneManager::openReportPopup $_82: __func::target_type
static uint32_t t_629E34() { return 0xf98388u; }
RW_FUNCTION(0x629E34, "SceneManager::openReportPopup $_82 __func::target_type", t_629E34);

// SceneManager::openReportPopup $_138: __func::~__func (deleting)
static void t_629E48(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x629E48, "SceneManager::openReportPopup $_138 __func::~__func (deleting)", t_629E48);

// SceneManager::openReportPopup $_138: __func::__clone()
static uint32_t t_629E50(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {48});
    if (v1) {
        st32(v1, 0xf5f8ccu);
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
        uint32_t v10 = ld32(self + 36 + 0);
        uint32_t v11 = ld32(self + 36 + 4);
        uint32_t v12 = ld32(self + 36 + 8);
        st32(v1 + 36 + 0, v10);
        st32(v1 + 36 + 4, v11);
        st32(v1 + 36 + 8, v12);
    }
    return v1;
}
RW_FUNCTION(0x629E50, "SceneManager::openReportPopup $_138 __func::__clone()", t_629E50);

// SceneManager::openReportPopup $_138: __func::__clone(__base*)
static void t_629E98(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f8ccu);
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
        uint32_t v10 = ld32(self + 40);
        uint32_t v11 = ld32(self + 44);
        st32(p + 44, v11);
        st32(p + 40, v10);
        st32(p + 36, v9);
    }
}
RW_FUNCTION(0x629E98, "SceneManager::openReportPopup $_138 __func::__clone(__base*)", t_629E98);

// SceneManager::openReportPopup $_138: __func::destroy
static void t_629ED4(uint32_t self) {
}
RW_FUNCTION(0x629ED4, "SceneManager::openReportPopup $_138 __func::destroy", t_629ED4);

// SceneManager::openReportPopup $_138: __func::destroy_deallocate
static void t_629ED6(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x629ED6, "SceneManager::openReportPopup $_138 __func::destroy_deallocate", t_629ED6);

// SceneManager::openReportPopup $_138: __func::target
static uint32_t t_629FDC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55b10u ? self + 4 : 0; }
RW_FUNCTION(0x629FDC, "SceneManager::openReportPopup $_138 __func::target", t_629FDC);

// SceneManager::openReportPopup $_138: __func::target_type
static uint32_t t_629FFC() { return 0xf9839cu; }
RW_FUNCTION(0x629FFC, "SceneManager::openReportPopup $_138 __func::target_type", t_629FFC);

// SceneManager::openReportPopup $_80: __func::~__func (deleting)
static void t_62A010(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A010, "SceneManager::openReportPopup $_80 __func::~__func (deleting)", t_62A010);

// SceneManager::openReportPopup $_80: __func::__clone()
static uint32_t t_62A018(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5f8f8u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x62A018, "SceneManager::openReportPopup $_80 __func::__clone()", t_62A018);

// SceneManager::openReportPopup $_80: __func::__clone(__base*)
static void t_62A048(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f8f8u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x62A048, "SceneManager::openReportPopup $_80 __func::__clone(__base*)", t_62A048);

// SceneManager::openReportPopup $_80: __func::destroy
static void t_62A06C(uint32_t self) {
}
RW_FUNCTION(0x62A06C, "SceneManager::openReportPopup $_80 __func::destroy", t_62A06C);

// SceneManager::openReportPopup $_80: __func::destroy_deallocate
static void t_62A06E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A06E, "SceneManager::openReportPopup $_80 __func::destroy_deallocate", t_62A06E);

// SceneManager::openReportPopup $_80: __func::target
static uint32_t t_62A0C8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55c30u ? self + 4 : 0; }
RW_FUNCTION(0x62A0C8, "SceneManager::openReportPopup $_80 __func::target", t_62A0C8);

// SceneManager::openReportPopup $_80: __func::target_type
static uint32_t t_62A0E8() { return 0xf983b0u; }
RW_FUNCTION(0x62A0E8, "SceneManager::openReportPopup $_80 __func::target_type", t_62A0E8);

// SceneManager::openReportPopup $_79: __func::~__func (deleting)
static void t_62A0FC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A0FC, "SceneManager::openReportPopup $_79 __func::~__func (deleting)", t_62A0FC);

// SceneManager::openReportPopup $_79: __func::__clone()
static uint32_t t_62A104(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {20});
    if (v1) {
        st32(v1, 0xf5f924u);
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
RW_FUNCTION(0x62A104, "SceneManager::openReportPopup $_79 __func::__clone()", t_62A104);

// SceneManager::openReportPopup $_79: __func::__clone(__base*)
static void t_62A138(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f924u);
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
RW_FUNCTION(0x62A138, "SceneManager::openReportPopup $_79 __func::__clone(__base*)", t_62A138);

// SceneManager::openReportPopup $_79: __func::destroy
static void t_62A164(uint32_t self) {
}
RW_FUNCTION(0x62A164, "SceneManager::openReportPopup $_79 __func::destroy", t_62A164);

// SceneManager::openReportPopup $_79: __func::destroy_deallocate
static void t_62A166(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A166, "SceneManager::openReportPopup $_79 __func::destroy_deallocate", t_62A166);

// SceneManager::openReportPopup $_79: __func::target
static uint32_t t_62A298(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55d20u ? self + 4 : 0; }
RW_FUNCTION(0x62A298, "SceneManager::openReportPopup $_79 __func::target", t_62A298);

// SceneManager::openReportPopup $_79: __func::target_type
static uint32_t t_62A2B8() { return 0xf983c4u; }
RW_FUNCTION(0x62A2B8, "SceneManager::openReportPopup $_79 __func::target_type", t_62A2B8);

// SceneManager::openBlockPostPopup $_78: __func::~__func (deleting)
static void t_62A2CC(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A2CC, "SceneManager::openBlockPostPopup $_78 __func::~__func (deleting)", t_62A2CC);

// SceneManager::openBlockPostPopup $_78: __func::__clone()
static uint32_t t_62A2D4(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {28});
    if (v1) {
        st32(v1, 0xf5f950u);
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
RW_FUNCTION(0x62A2D4, "SceneManager::openBlockPostPopup $_78 __func::__clone()", t_62A2D4);

// SceneManager::openBlockPostPopup $_78: __func::__clone(__base*)
static void t_62A30C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f950u);
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
RW_FUNCTION(0x62A30C, "SceneManager::openBlockPostPopup $_78 __func::__clone(__base*)", t_62A30C);

// SceneManager::openBlockPostPopup $_78: __func::destroy
static void t_62A338(uint32_t self) {
}
RW_FUNCTION(0x62A338, "SceneManager::openBlockPostPopup $_78 __func::destroy", t_62A338);

// SceneManager::openBlockPostPopup $_78: __func::destroy_deallocate
static void t_62A33A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A33A, "SceneManager::openBlockPostPopup $_78 __func::destroy_deallocate", t_62A33A);

// SceneManager::openBlockPostPopup $_78: __func::target
static uint32_t t_62A444(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55e20u ? self + 4 : 0; }
RW_FUNCTION(0x62A444, "SceneManager::openBlockPostPopup $_78 __func::target", t_62A444);

// SceneManager::openBlockPostPopup $_78: __func::target_type
static uint32_t t_62A464() { return 0xf983d8u; }
RW_FUNCTION(0x62A464, "SceneManager::openBlockPostPopup $_78 __func::target_type", t_62A464);

// SceneManager::openBlockPostPopup $_136 UlvE_: __func::~__func (deleting)
static void t_62A478(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A478, "SceneManager::openBlockPostPopup $_136 UlvE_ __func::~__func (deleting)", t_62A478);

// SceneManager::openBlockPostPopup $_136 UlvE_: __func::__clone()
static uint32_t t_62A480(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5f97cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x62A480, "SceneManager::openBlockPostPopup $_136 UlvE_ __func::__clone()", t_62A480);

// SceneManager::openBlockPostPopup $_136 UlvE_: __func::__clone(__base*)
static void t_62A4B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f97cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x62A4B0, "SceneManager::openBlockPostPopup $_136 UlvE_ __func::__clone(__base*)", t_62A4B0);

// SceneManager::openBlockPostPopup $_136 UlvE_: __func::destroy
static void t_62A4D8(uint32_t self) {
}
RW_FUNCTION(0x62A4D8, "SceneManager::openBlockPostPopup $_136 UlvE_ __func::destroy", t_62A4D8);

// SceneManager::openBlockPostPopup $_136 UlvE_: __func::destroy_deallocate
static void t_62A4DA(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A4DA, "SceneManager::openBlockPostPopup $_136 UlvE_ __func::destroy_deallocate", t_62A4DA);

// SceneManager::openBlockPostPopup $_136 UlvE_: __func::target
static uint32_t t_62A50C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe55f30u ? self + 4 : 0; }
RW_FUNCTION(0x62A50C, "SceneManager::openBlockPostPopup $_136 UlvE_ __func::target", t_62A50C);

// SceneManager::openBlockPostPopup $_136 UlvE_: __func::target_type
static uint32_t t_62A52C() { return 0xf983ecu; }
RW_FUNCTION(0x62A52C, "SceneManager::openBlockPostPopup $_136 UlvE_ __func::target_type", t_62A52C);

// SceneManager::openBlockPostPopup $_77: __func::~__func (deleting)
static void t_62A540(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A540, "SceneManager::openBlockPostPopup $_77 __func::~__func (deleting)", t_62A540);

// SceneManager::openBlockPostPopup $_77: __func::__clone()
static uint32_t t_62A548(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5f9a8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x62A548, "SceneManager::openBlockPostPopup $_77 __func::__clone()", t_62A548);

// SceneManager::openBlockPostPopup $_77: __func::__clone(__base*)
static void t_62A574(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5f9a8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x62A574, "SceneManager::openBlockPostPopup $_77 __func::__clone(__base*)", t_62A574);

// SceneManager::openBlockPostPopup $_77: __func::destroy
static void t_62A594(uint32_t self) {
}
RW_FUNCTION(0x62A594, "SceneManager::openBlockPostPopup $_77 __func::destroy", t_62A594);

// SceneManager::openBlockPostPopup $_77: __func::destroy_deallocate
static void t_62A596(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62A596, "SceneManager::openBlockPostPopup $_77 __func::destroy_deallocate", t_62A596);

// SceneManager::openBlockPostPopup $_77: __func::target
static uint32_t t_62A5A8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe56040u ? self + 4 : 0; }
RW_FUNCTION(0x62A5A8, "SceneManager::openBlockPostPopup $_77 __func::target", t_62A5A8);

// SceneManager::openBlockPostPopup $_77: __func::target_type
static uint32_t t_62A5C8() { return 0xf98400u; }
RW_FUNCTION(0x62A5C8, "SceneManager::openBlockPostPopup $_77 __func::target_type", t_62A5C8);

// SceneManager::openBlockPostPopup $_76: __func::target
static uint32_t t_62B150(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe56170u ? self + 8 : 0; }
RW_FUNCTION(0x62B150, "SceneManager::openBlockPostPopup $_76 __func::target", t_62B150);

// SceneManager::openBlockPostPopup $_76: __func::target_type
static uint32_t t_62B174() { return 0xf98414u; }
RW_FUNCTION(0x62B174, "SceneManager::openBlockPostPopup $_76 __func::target_type", t_62B174);

// SceneManager::openBlockPostPopup $_75: __func::~__func
static uint32_t t_62B264(uint32_t self) {
    st32(self, 0xf5fa00u);
    uint32_t v1 = ld32(self + 176);
    functionDestroy(self + 160);
    uint32_t v2 = ld32(self + 144);
    functionDestroy(self + 128);
    uint32_t v3 = ld32(self + 112);
    functionDestroy(self + 96);
    uint32_t v4 = ld8(self + 60);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 68);
        call(fn::operator_delete, {v5});
    }
    uint32_t v6 = ld8(self + 36);
    if (v6 & 1) {
        uint32_t v7 = ld32(self + 44);
        call(fn::operator_delete, {v7});
    }
    return self;
}
RW_FUNCTION(0x62B264, "SceneManager::openBlockPostPopup $_75 __func::~__func", t_62B264);

// SceneManager::openBlockPostPopup $_75: __func::~__func (deleting)
static void t_62B2FC(uint32_t self) {
    st32(self, 0xf5fa00u);
    uint32_t v1 = ld32(self + 176);
    functionDestroy(self + 160);
    uint32_t v2 = ld32(self + 144);
    functionDestroy(self + 128);
    uint32_t v3 = ld32(self + 112);
    functionDestroy(self + 96);
    uint32_t v4 = ld8(self + 60);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 68);
        call(fn::operator_delete, {v5});
    }
    uint32_t v6 = ld8(self + 36);
    if (v6 & 1) {
        uint32_t v7 = ld32(self + 44);
        call(fn::operator_delete, {v7});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62B2FC, "SceneManager::openBlockPostPopup $_75 __func::~__func (deleting)", t_62B2FC);

// SceneManager::openBlockPostPopup $_75: __func::destroy
static void t_62B6F0(uint32_t self) {
    uint32_t v1 = ld32(self + 176);
    functionDestroy(self + 160);
    uint32_t v2 = ld32(self + 144);
    functionDestroy(self + 128);
    uint32_t v3 = ld32(self + 112);
    functionDestroy(self + 96);
    uint32_t v4 = ld8(self + 60);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 68);
        call(fn::operator_delete, {v5});
    }
    uint32_t v6 = ld8(self + 36);
    if (v6 & 1) {
        uint32_t v7 = ld32(self + 44);
        call(fn::operator_delete, {v7});
    }
}
RW_FUNCTION(0x62B6F0, "SceneManager::openBlockPostPopup $_75 __func::destroy", t_62B6F0);

// SceneManager::openBlockPostPopup $_75: __func::destroy_deallocate
static void t_62B76E(uint32_t self) {
    uint32_t v1 = ld32(self + 176);
    functionDestroy(self + 160);
    uint32_t v2 = ld32(self + 144);
    functionDestroy(self + 128);
    uint32_t v3 = ld32(self + 112);
    functionDestroy(self + 96);
    uint32_t v4 = ld8(self + 60);
    if (v4 & 1) {
        uint32_t v5 = ld32(self + 68);
        call(fn::operator_delete, {v5});
    }
    uint32_t v6 = ld8(self + 36);
    if (v6 & 1) {
        uint32_t v7 = ld32(self + 44);
        call(fn::operator_delete, {v7});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62B76E, "SceneManager::openBlockPostPopup $_75 __func::destroy_deallocate", t_62B76E);

// SceneManager::openBlockPostPopup $_75: __func::target
static uint32_t t_62BD10(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe56290u ? self + 8 : 0; }
RW_FUNCTION(0x62BD10, "SceneManager::openBlockPostPopup $_75 __func::target", t_62BD10);

// SceneManager::openBlockPostPopup $_75: __func::target_type
static uint32_t t_62BD34() { return 0xf98428u; }
RW_FUNCTION(0x62BD34, "SceneManager::openBlockPostPopup $_75 __func::target_type", t_62BD34);

// SceneManager::openBlockPostPopup $_74: __func::~__func
static uint32_t t_62BD48(uint32_t self) {
    st32(self, 0xf5fa2cu);
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x62BD48, "SceneManager::openBlockPostPopup $_74 __func::~__func", t_62BD48);

// SceneManager::openBlockPostPopup $_74: __func::~__func (deleting)
static void t_62BDA0(uint32_t self) {
    st32(self, 0xf5fa2cu);
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62BDA0, "SceneManager::openBlockPostPopup $_74 __func::~__func (deleting)", t_62BDA0);

// SceneManager::openBlockPostPopup $_74: __func::__clone(__base*)
static void t_62BE9C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5fa2cu);
        uint32_t v1 = ld32(self + 8 + 0);
        uint32_t v2 = ld32(self + 8 + 4);
        uint32_t v3 = ld32(self + 8 + 8);
        st32(p + 8 + 0, v1);
        st32(p + 8 + 4, v2);
        st32(p + 8 + 8, v3);
        call(0x4683cd, {p + 24, self + 24});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        uint32_t v4 = ld32(self + 48);
        uint32_t v5 = ld32(self + 52);
        st32(p + 48, v4);
        st32(p + 52, v5);
        call(0x4683cd, {p + 56, self + 56});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x62BE9C, "SceneManager::openBlockPostPopup $_74 __func::__clone(__base*)", t_62BE9C);

// SceneManager::openBlockPostPopup $_74: __func::destroy
static void t_62BF24(uint32_t self) {
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x62BF24, "SceneManager::openBlockPostPopup $_74 __func::destroy", t_62BF24);

// SceneManager::openBlockPostPopup $_74: __func::destroy_deallocate
static void t_62BF60(uint32_t self) {
    uint32_t v1 = ld32(self + 72);
    functionDestroy(self + 56);
    uint32_t v2 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62BF60, "SceneManager::openBlockPostPopup $_74 __func::destroy_deallocate", t_62BF60);

// SceneManager::openBlockPostPopup $_74: __func::target
static uint32_t t_62C3E8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe563e0u ? self + 8 : 0; }
RW_FUNCTION(0x62C3E8, "SceneManager::openBlockPostPopup $_74 __func::target", t_62C3E8);

// SceneManager::openBlockPostPopup $_74: __func::target_type
static uint32_t t_62C40C() { return 0xf9843cu; }
RW_FUNCTION(0x62C40C, "SceneManager::openBlockPostPopup $_74 __func::target_type", t_62C40C);

// SceneManager::openBlockPostPopup $_72: __func::~__func (deleting)
static void t_62C4E2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62C4E2, "SceneManager::openBlockPostPopup $_72 __func::~__func (deleting)", t_62C4E2);

// SceneManager::openBlockPostPopup $_72: __func::__clone()
static uint32_t t_62C4EC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5fa58u);
    }
    return v1;
}
RW_FUNCTION(0x62C4EC, "SceneManager::openBlockPostPopup $_72 __func::__clone()", t_62C4EC);

// SceneManager::openBlockPostPopup $_72: __func::__clone(__base*)
static void t_62C510(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5fa58u);
    }
}
RW_FUNCTION(0x62C510, "SceneManager::openBlockPostPopup $_72 __func::__clone(__base*)", t_62C510);

// SceneManager::openBlockPostPopup $_72: __func::destroy
static void t_62C52C(uint32_t self) {
}
RW_FUNCTION(0x62C52C, "SceneManager::openBlockPostPopup $_72 __func::destroy", t_62C52C);

// SceneManager::openBlockPostPopup $_72: __func::destroy_deallocate
static void t_62C52E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62C52E, "SceneManager::openBlockPostPopup $_72 __func::destroy_deallocate", t_62C52E);

// SceneManager::openBlockPostPopup $_72: __func::target
static uint32_t t_62C550(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe56510u ? self + 4 : 0; }
RW_FUNCTION(0x62C550, "SceneManager::openBlockPostPopup $_72 __func::target", t_62C550);

// SceneManager::openBlockPostPopup $_72: __func::target_type
static uint32_t t_62C570() { return 0xf98450u; }
RW_FUNCTION(0x62C570, "SceneManager::openBlockPostPopup $_72 __func::target_type", t_62C570);

// SceneManager::openBlockPostPopup $_71: __func::~__func (deleting)
static void t_62C584(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x62C584, "SceneManager::openBlockPostPopup $_71 __func::~__func (deleting)", t_62C584);

// SceneManager::openBlockPostPopup $_71: __func::__clone()
static uint32_t t_62C58C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5fa84u);
    }
    return v1;
}
RW_FUNCTION(0x62C58C, "SceneManager::openBlockPostPopup $_71 __func::__clone()", t_62C58C);
