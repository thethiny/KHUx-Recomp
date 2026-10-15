// The std::function __func thunks of the lambdas attributed to iap (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// PurchaseManager::startPurchase $_1: __func::~__func
static uint32_t t_4D9C10(uint32_t self) {
    st32(self, 0xf556b0u);
    uint32_t v1 = ld8(self + 48);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 56);
        call(fn::operator_delete, {v2});
    }
    uint32_t v3 = ld32(self + 40);
    functionDestroy(self + 24);
    return self;
}
RW_FUNCTION(0x4D9C10, "PurchaseManager::startPurchase $_1 __func::~__func", t_4D9C10);

// PurchaseManager::startPurchase $_1: __func::~__func (deleting)
static void t_4D9C5C(uint32_t self) {
    st32(self, 0xf556b0u);
    uint32_t v1 = ld8(self + 48);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 56);
        call(fn::operator_delete, {v2});
    }
    uint32_t v3 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4D9C5C, "PurchaseManager::startPurchase $_1 __func::~__func (deleting)", t_4D9C5C);

// PurchaseManager::startPurchase $_1: __func::destroy
static void t_4D9E04(uint32_t self) {
    uint32_t v1 = ld8(self + 48);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 56);
        call(fn::operator_delete, {v2});
    }
    uint32_t v3 = ld32(self + 40);
    functionDestroy(self + 24);
}
RW_FUNCTION(0x4D9E04, "PurchaseManager::startPurchase $_1 __func::destroy", t_4D9E04);

// PurchaseManager::startPurchase $_1: __func::destroy_deallocate
static void t_4D9E34(uint32_t self) {
    uint32_t v1 = ld8(self + 48);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 56);
        call(fn::operator_delete, {v2});
    }
    uint32_t v3 = ld32(self + 40);
    functionDestroy(self + 24);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x4D9E34, "PurchaseManager::startPurchase $_1 __func::destroy_deallocate", t_4D9E34);

// PurchaseManager::startPurchase $_1: __func::target
static uint32_t t_4DA038(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe1d840u ? self + 8 : 0; }
RW_FUNCTION(0x4DA038, "PurchaseManager::startPurchase $_1 __func::target", t_4DA038);

// PurchaseManager::startPurchase $_1: __func::target_type
static uint32_t t_4DA058() { return 0xf7f948u; }
RW_FUNCTION(0x4DA058, "PurchaseManager::startPurchase $_1 __func::target_type", t_4DA058);
