// CarouselDeck: the std::function __func thunks of its lambdas (destructors, clone - copy the captures into a new /
// given block -, destroy, target / target_type typeinfo checks), generated from the lifted code by a scratch
// tool (gen_thunks.py, symbolic walk of each thunk) and checked against it. The lambda bodies are written by hand.
#include "../scene/SceneCommon.hpp"

namespace {
using namespace scene;
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }

// CarouselDeck::initGuiltBonusEffect $_9: __func::~__func (deleting)
static void initGuiltBonusEffect_L9_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D2938, "CarouselDeck::initGuiltBonusEffect $_9 __func::~__func (deleting)", initGuiltBonusEffect_L9_deletingDtor);

// CarouselDeck::initGuiltBonusEffect $_9: __func::__clone()
static uint32_t initGuiltBonusEffect_L9_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d1d0u);
    }
    return v1;
}
RW_FUNCTION(0x5D2940, "CarouselDeck::initGuiltBonusEffect $_9 __func::__clone()", initGuiltBonusEffect_L9_clone);

// CarouselDeck::initGuiltBonusEffect $_9: __func::__clone(__base*)
static void initGuiltBonusEffect_L9_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d1d0u);
    }
}
RW_FUNCTION(0x5D2964, "CarouselDeck::initGuiltBonusEffect $_9 __func::__clone(__base*)", initGuiltBonusEffect_L9_cloneInto);

// CarouselDeck::initGuiltBonusEffect $_9: __func::destroy
static void initGuiltBonusEffect_L9_destroy(uint32_t self) {
}
RW_FUNCTION(0x5D2980, "CarouselDeck::initGuiltBonusEffect $_9 __func::destroy", initGuiltBonusEffect_L9_destroy);

// CarouselDeck::initGuiltBonusEffect $_9: __func::destroy_deallocate
static void initGuiltBonusEffect_L9_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D2982, "CarouselDeck::initGuiltBonusEffect $_9 __func::destroy_deallocate", initGuiltBonusEffect_L9_destroyDeallocate);

// CarouselDeck::initGuiltBonusEffect $_9: __func::target
static uint32_t initGuiltBonusEffect_L9_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4aaf0u ? self + 4 : 0; }
RW_FUNCTION(0x5D29C4, "CarouselDeck::initGuiltBonusEffect $_9 __func::target", initGuiltBonusEffect_L9_target);

// CarouselDeck::initGuiltBonusEffect $_9: __func::target_type
static uint32_t initGuiltBonusEffect_L9_targetType() { return 0xf90580u; }
RW_FUNCTION(0x5D29E4, "CarouselDeck::initGuiltBonusEffect $_9 __func::target_type", initGuiltBonusEffect_L9_targetType);

// CarouselDeck::setAbilityInfo $_8: __func::~__func (deleting)
static void setAbilityInfo_L8_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D29F8, "CarouselDeck::setAbilityInfo $_8 __func::~__func (deleting)", setAbilityInfo_L8_deletingDtor);

// CarouselDeck::setAbilityInfo $_8: __func::__clone()
static uint32_t setAbilityInfo_L8_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d1fcu);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5D2A00, "CarouselDeck::setAbilityInfo $_8 __func::__clone()", setAbilityInfo_L8_clone);

// CarouselDeck::setAbilityInfo $_8: __func::__clone(__base*)
static void setAbilityInfo_L8_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d1fcu);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5D2A2C, "CarouselDeck::setAbilityInfo $_8 __func::__clone(__base*)", setAbilityInfo_L8_cloneInto);

// CarouselDeck::setAbilityInfo $_8: __func::destroy
static void setAbilityInfo_L8_destroy(uint32_t self) {
}
RW_FUNCTION(0x5D2A4C, "CarouselDeck::setAbilityInfo $_8 __func::destroy", setAbilityInfo_L8_destroy);

// CarouselDeck::setAbilityInfo $_8: __func::destroy_deallocate
static void setAbilityInfo_L8_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D2A4E, "CarouselDeck::setAbilityInfo $_8 __func::destroy_deallocate", setAbilityInfo_L8_destroyDeallocate);

// CarouselDeck::setAbilityInfo $_8: __func::target
static uint32_t setAbilityInfo_L8_target(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe4abb0u ? self + 4 : 0; }
RW_FUNCTION(0x5D2A60, "CarouselDeck::setAbilityInfo $_8 __func::target", setAbilityInfo_L8_target);

// CarouselDeck::setAbilityInfo $_8: __func::target_type
static uint32_t setAbilityInfo_L8_targetType() { return 0xf90594u; }
RW_FUNCTION(0x5D2A80, "CarouselDeck::setAbilityInfo $_8 __func::target_type", setAbilityInfo_L8_targetType);

// CarouselDeck::createMadalButton $_7: __func::~__func (deleting)
static void createMadalButton_L7_deletingDtor(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D2A94, "CarouselDeck::createMadalButton $_7 __func::~__func (deleting)", createMadalButton_L7_deletingDtor);

// CarouselDeck::createMadalButton $_7: __func::__clone()
static uint32_t createMadalButton_L7_clone(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5d228u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x5D2A9C, "CarouselDeck::createMadalButton $_7 __func::__clone()", createMadalButton_L7_clone);

// CarouselDeck::createMadalButton $_7: __func::__clone(__base*)
static void createMadalButton_L7_cloneInto(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5d228u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x5D2AC8, "CarouselDeck::createMadalButton $_7 __func::__clone(__base*)", createMadalButton_L7_cloneInto);

// CarouselDeck::createMadalButton $_7: __func::destroy
static void createMadalButton_L7_destroy(uint32_t self) {
}
RW_FUNCTION(0x5D2AE8, "CarouselDeck::createMadalButton $_7 __func::destroy", createMadalButton_L7_destroy);

// CarouselDeck::createMadalButton $_7: __func::destroy_deallocate
static void createMadalButton_L7_destroyDeallocate(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5D2AEA, "CarouselDeck::createMadalButton $_7 __func::destroy_deallocate", createMadalButton_L7_destroyDeallocate);

}  // namespace
