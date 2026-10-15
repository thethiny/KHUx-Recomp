// The std::function __func thunks of the lambdas attributed to BuyMagicStoneDialog (owners.py; the comment on each names the
// lambda it belongs to, which may be a neighbouring class): ~__func, __clone (copy the captures into a new /
// given block), destroy / destroy_deallocate (destroy the captured objects), target / target_type (typeinfo
// checks). Generated from the lifted code by a symbolic walk of each thunk; each mirrors its original exactly.
#include "../scene_social/SocialCommon.hpp"

using namespace social;

namespace {
inline uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
}  // namespace

// AdvEventBannerScrollView::setEventBanner $_1: __func::~__func
static uint32_t t_5887D0(uint32_t self) {
    st32(self, 0xf5a460u);
    uint32_t v1 = ld8(self + 28);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 36);
        call(fn::operator_delete, {v2});
    }
    call(0x4b58ef, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api5Stage5StoryENS_9allocatorIS5_EEED2Ev
    return self;
}
RW_FUNCTION(0x5887D0, "AdvEventBannerScrollView::setEventBanner $_1 __func::~__func", t_5887D0);

// AdvEventBannerScrollView::setEventBanner $_1: __func::~__func (deleting)
static void t_588808(uint32_t self) {
    st32(self, 0xf5a460u);
    uint32_t v1 = ld8(self + 28);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 36);
        call(fn::operator_delete, {v2});
    }
    call(0x4b58ef, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api5Stage5StoryENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x588808, "AdvEventBannerScrollView::setEventBanner $_1 __func::~__func (deleting)", t_588808);

// AdvEventBannerScrollView::setEventBanner $_1: __func::destroy
static void t_5889A4(uint32_t self) {
    uint32_t v1 = ld8(self + 28);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 36);
        call(fn::operator_delete, {v2});
    }
    call(0x4b58ef, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api5Stage5StoryENS_9allocatorIS5_EEED2Ev
}
RW_FUNCTION(0x5889A4, "AdvEventBannerScrollView::setEventBanner $_1 __func::destroy", t_5889A4);

// AdvEventBannerScrollView::setEventBanner $_1: __func::destroy_deallocate
static void t_5889C4(uint32_t self) {
    uint32_t v1 = ld8(self + 28);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 36);
        call(fn::operator_delete, {v2});
    }
    call(0x4b58ef, {self + 8});   // _ZNSt3__113__vector_baseIN4hole7network3api5Stage5StoryENS_9allocatorIS5_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x5889C4, "AdvEventBannerScrollView::setEventBanner $_1 __func::destroy_deallocate", t_5889C4);

// AdvEventBannerScrollView::setEventBanner $_1: __func::target
static uint32_t t_588A60(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39170u ? self + 4 : 0; }
RW_FUNCTION(0x588A60, "AdvEventBannerScrollView::setEventBanner $_1 __func::target", t_588A60);

// AdvEventBannerScrollView::setEventBanner $_1: __func::target_type
static uint32_t t_588A80() { return 0xf8bbd4u; }
RW_FUNCTION(0x588A80, "AdvEventBannerScrollView::setEventBanner $_1 __func::target_type", t_588A80);

// AdvEventBannerScrollView::loadEventBanner $_0 (shared): __func::~__func
static uint32_t t_588C00(uint32_t self) {
    return self;
}
RW_FUNCTION(0x588C00, "AdvEventBannerScrollView::loadEventBanner $_0 (shared) __func::~__func", t_588C00);

// BuyMagicStoneDialog::initTokushohoButton $_15: __func::~__func (deleting)
static void t_58B8F0(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58B8F0, "BuyMagicStoneDialog::initTokushohoButton $_15 __func::~__func (deleting)", t_58B8F0);

// BuyMagicStoneDialog::initTokushohoButton $_15: __func::__clone()
static uint32_t t_58B8F8(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a4b8u);
    }
    return v1;
}
RW_FUNCTION(0x58B8F8, "BuyMagicStoneDialog::initTokushohoButton $_15 __func::__clone()", t_58B8F8);

// BuyMagicStoneDialog::initTokushohoButton $_15: __func::__clone(__base*)
static void t_58B91C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a4b8u);
    }
}
RW_FUNCTION(0x58B91C, "BuyMagicStoneDialog::initTokushohoButton $_15 __func::__clone(__base*)", t_58B91C);

// BuyMagicStoneDialog::initTokushohoButton $_15: __func::destroy
static void t_58B938(uint32_t self) {
}
RW_FUNCTION(0x58B938, "BuyMagicStoneDialog::initTokushohoButton $_15 __func::destroy", t_58B938);

// BuyMagicStoneDialog::initTokushohoButton $_15: __func::destroy_deallocate
static void t_58B93A(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58B93A, "BuyMagicStoneDialog::initTokushohoButton $_15 __func::destroy_deallocate", t_58B93A);

// BuyMagicStoneDialog::initTokushohoButton $_15: __func::target
static uint32_t t_58BB00(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe393a0u ? self + 4 : 0; }
RW_FUNCTION(0x58BB00, "BuyMagicStoneDialog::initTokushohoButton $_15 __func::target", t_58BB00);

// BuyMagicStoneDialog::initTokushohoButton $_15: __func::target_type
static uint32_t t_58BB20() { return 0xf8bc04u; }
RW_FUNCTION(0x58BB20, "BuyMagicStoneDialog::initTokushohoButton $_15 __func::target_type", t_58BB20);

// BuyMagicStoneDialog::initFundSettlementButton $_14: __func::~__func (deleting)
static void t_58BB34(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58BB34, "BuyMagicStoneDialog::initFundSettlementButton $_14 __func::~__func (deleting)", t_58BB34);

// BuyMagicStoneDialog::initFundSettlementButton $_14: __func::__clone()
static uint32_t t_58BB3C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a4e4u);
    }
    return v1;
}
RW_FUNCTION(0x58BB3C, "BuyMagicStoneDialog::initFundSettlementButton $_14 __func::__clone()", t_58BB3C);

// BuyMagicStoneDialog::initFundSettlementButton $_14: __func::__clone(__base*)
static void t_58BB60(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a4e4u);
    }
}
RW_FUNCTION(0x58BB60, "BuyMagicStoneDialog::initFundSettlementButton $_14 __func::__clone(__base*)", t_58BB60);

// BuyMagicStoneDialog::initFundSettlementButton $_14: __func::destroy
static void t_58BB7C(uint32_t self) {
}
RW_FUNCTION(0x58BB7C, "BuyMagicStoneDialog::initFundSettlementButton $_14 __func::destroy", t_58BB7C);

// BuyMagicStoneDialog::initFundSettlementButton $_14: __func::destroy_deallocate
static void t_58BB7E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58BB7E, "BuyMagicStoneDialog::initFundSettlementButton $_14 __func::destroy_deallocate", t_58BB7E);

// BuyMagicStoneDialog::initFundSettlementButton $_14: __func::target
static uint32_t t_58BD44(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39490u ? self + 4 : 0; }
RW_FUNCTION(0x58BD44, "BuyMagicStoneDialog::initFundSettlementButton $_14 __func::target", t_58BD44);

// BuyMagicStoneDialog::initFundSettlementButton $_14: __func::target_type
static uint32_t t_58BD64() { return 0xf8bc18u; }
RW_FUNCTION(0x58BD64, "BuyMagicStoneDialog::initFundSettlementButton $_14 __func::target_type", t_58BD64);

// BuyMagicStoneDialog::initJewelInfoButton $_13: __func::~__func (deleting)
static void t_58BD78(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58BD78, "BuyMagicStoneDialog::initJewelInfoButton $_13 __func::~__func (deleting)", t_58BD78);

// BuyMagicStoneDialog::initJewelInfoButton $_13: __func::__clone()
static uint32_t t_58BD80(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a510u);
    }
    return v1;
}
RW_FUNCTION(0x58BD80, "BuyMagicStoneDialog::initJewelInfoButton $_13 __func::__clone()", t_58BD80);

// BuyMagicStoneDialog::initJewelInfoButton $_13: __func::__clone(__base*)
static void t_58BDA4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a510u);
    }
}
RW_FUNCTION(0x58BDA4, "BuyMagicStoneDialog::initJewelInfoButton $_13 __func::__clone(__base*)", t_58BDA4);

// BuyMagicStoneDialog::initJewelInfoButton $_13: __func::destroy
static void t_58BDC0(uint32_t self) {
}
RW_FUNCTION(0x58BDC0, "BuyMagicStoneDialog::initJewelInfoButton $_13 __func::destroy", t_58BDC0);

// BuyMagicStoneDialog::initJewelInfoButton $_13: __func::destroy_deallocate
static void t_58BDC2(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58BDC2, "BuyMagicStoneDialog::initJewelInfoButton $_13 __func::destroy_deallocate", t_58BDC2);

// BuyMagicStoneDialog::initJewelInfoButton $_13: __func::target
static uint32_t t_58BDDC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39580u ? self + 4 : 0; }
RW_FUNCTION(0x58BDDC, "BuyMagicStoneDialog::initJewelInfoButton $_13 __func::target", t_58BDDC);

// BuyMagicStoneDialog::initJewelInfoButton $_13: __func::target_type
static uint32_t t_58BDFC() { return 0xf8bc2cu; }
RW_FUNCTION(0x58BDFC, "BuyMagicStoneDialog::initJewelInfoButton $_13 __func::target_type", t_58BDFC);

// BuyMagicStoneDialog::hasOwnedItem : __func::~__func
static uint32_t t_58BE10(uint32_t self) {
    st32(self, 0xf5a53cu);
    uint32_t v1 = ld8(self + 4);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 12);
        call(fn::operator_delete, {v2});
    }
    return self;
}
RW_FUNCTION(0x58BE10, "BuyMagicStoneDialog::hasOwnedItem  __func::~__func", t_58BE10);

// BuyMagicStoneDialog::hasOwnedItem : __func::~__func (deleting)
static void t_58BE40(uint32_t self) {
    st32(self, 0xf5a53cu);
    uint32_t v1 = ld8(self + 4);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 12);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58BE40, "BuyMagicStoneDialog::hasOwnedItem  __func::~__func (deleting)", t_58BE40);

// BuyMagicStoneDialog::hasOwnedItem : __func::destroy
static void t_58BF2C(uint32_t self) {
    uint32_t v1 = ld8(self + 4);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 12);
        call(fn::operator_delete, {v2});
    }
}
RW_FUNCTION(0x58BF2C, "BuyMagicStoneDialog::hasOwnedItem  __func::destroy", t_58BF2C);

// BuyMagicStoneDialog::hasOwnedItem : __func::destroy_deallocate
static void t_58BF3E(uint32_t self) {
    uint32_t v1 = ld8(self + 4);
    if (v1 & 1) {
        uint32_t v2 = ld32(self + 12);
        call(fn::operator_delete, {v2});
    }
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58BF3E, "BuyMagicStoneDialog::hasOwnedItem  __func::destroy_deallocate", t_58BF3E);

// BuyMagicStoneDialog::hasOwnedItem : __func::target
static uint32_t t_58C03C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe396d0u ? self + 4 : 0; }
RW_FUNCTION(0x58C03C, "BuyMagicStoneDialog::hasOwnedItem  __func::target", t_58C03C);

// BuyMagicStoneDialog::hasOwnedItem : __func::target_type
static uint32_t t_58C05C() { return 0xf8bc40u; }
RW_FUNCTION(0x58C05C, "BuyMagicStoneDialog::hasOwnedItem  __func::target_type", t_58C05C);

// BuyMagicStoneDialog::hasOwnedItem : __func::~__func (deleting)
static void t_58C070(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58C070, "BuyMagicStoneDialog::hasOwnedItem  __func::~__func (deleting)", t_58C070);

// BuyMagicStoneDialog::hasOwnedItem : __func::__clone()
static uint32_t t_58C078(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {12});
    if (v1) {
        st32(v1, 0xf5a568u);
        uint32_t v2 = ld32(self + 4);
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x58C078, "BuyMagicStoneDialog::hasOwnedItem  __func::__clone()", t_58C078);

// BuyMagicStoneDialog::hasOwnedItem : __func::__clone(__base*)
static void t_58C0A8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a568u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x58C0A8, "BuyMagicStoneDialog::hasOwnedItem  __func::__clone(__base*)", t_58C0A8);

// BuyMagicStoneDialog::hasOwnedItem : __func::destroy
static void t_58C0CC(uint32_t self) {
}
RW_FUNCTION(0x58C0CC, "BuyMagicStoneDialog::hasOwnedItem  __func::destroy", t_58C0CC);

// BuyMagicStoneDialog::hasOwnedItem : __func::destroy_deallocate
static void t_58C0CE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58C0CE, "BuyMagicStoneDialog::hasOwnedItem  __func::destroy_deallocate", t_58C0CE);

// BuyMagicStoneDialog::hasOwnedItem : __func::target
static uint32_t t_58C1A8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe398c0u ? self + 4 : 0; }
RW_FUNCTION(0x58C1A8, "BuyMagicStoneDialog::hasOwnedItem  __func::target", t_58C1A8);

// BuyMagicStoneDialog::hasOwnedItem : __func::target_type
static uint32_t t_58C1C8() { return 0xf8bc54u; }
RW_FUNCTION(0x58C1C8, "BuyMagicStoneDialog::hasOwnedItem  __func::target_type", t_58C1C8);

// BuyMagicStoneDialog::purchaseCallback $_11: __func::~__func
static uint32_t t_58C1DC(uint32_t self) {
    st32(self, 0xf5a594u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x58C1DC, "BuyMagicStoneDialog::purchaseCallback $_11 __func::~__func", t_58C1DC);

// BuyMagicStoneDialog::purchaseCallback $_11: __func::~__func (deleting)
static void t_58C218(uint32_t self) {
    st32(self, 0xf5a594u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58C218, "BuyMagicStoneDialog::purchaseCallback $_11 __func::~__func (deleting)", t_58C218);

// BuyMagicStoneDialog::purchaseCallback $_11: __func::__clone()
static uint32_t t_58C258(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5a594u);
        uint32_t v2 = ld32(self + 8);
        st32(v1 + 8, v2);
        call(0x45c84d, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v3 = ld32(self + 8);
        st32(v1 + 8, v3);
    }
    return v1;
}
RW_FUNCTION(0x58C258, "BuyMagicStoneDialog::purchaseCallback $_11 __func::__clone()", t_58C258);

// BuyMagicStoneDialog::purchaseCallback $_11: __func::__clone(__base*)
static void t_58C2B0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a594u);
        uint32_t v1 = ld32(self + 8);
        st32(p + 8, v1);
        call(0x45c84d, {p + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
        uint32_t v2 = ld32(self + 8);
        st32(p + 8, v2);
    }
}
RW_FUNCTION(0x58C2B0, "BuyMagicStoneDialog::purchaseCallback $_11 __func::__clone(__base*)", t_58C2B0);

// BuyMagicStoneDialog::purchaseCallback $_11: __func::destroy
static void t_58C2F0(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x58C2F0, "BuyMagicStoneDialog::purchaseCallback $_11 __func::destroy", t_58C2F0);

// BuyMagicStoneDialog::purchaseCallback $_11: __func::destroy_deallocate
static void t_58C310(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58C310, "BuyMagicStoneDialog::purchaseCallback $_11 __func::destroy_deallocate", t_58C310);

// BuyMagicStoneDialog::purchaseCallback $_11: __func::target
static uint32_t t_58C360(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39a60u ? self + 8 : 0; }
RW_FUNCTION(0x58C360, "BuyMagicStoneDialog::purchaseCallback $_11 __func::target", t_58C360);

// BuyMagicStoneDialog::purchaseCallback $_11: __func::target_type
static uint32_t t_58C384() { return 0xf8bc68u; }
RW_FUNCTION(0x58C384, "BuyMagicStoneDialog::purchaseCallback $_11 __func::target_type", t_58C384);

// BuyMagicStoneDialog::checkTransaction $_9: __func::~__func
static uint32_t t_58CAE8(uint32_t self) {
    st32(self, 0xf5a5c0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x58CAE8, "BuyMagicStoneDialog::checkTransaction $_9 __func::~__func", t_58CAE8);

// BuyMagicStoneDialog::checkTransaction $_9: __func::~__func (deleting)
static void t_58CB24(uint32_t self) {
    st32(self, 0xf5a5c0u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58CB24, "BuyMagicStoneDialog::checkTransaction $_9 __func::~__func (deleting)", t_58CB24);

// BuyMagicStoneDialog::checkTransaction $_9: __func::__clone()
static uint32_t t_58CB64(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5a5c0u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x58CB64, "BuyMagicStoneDialog::checkTransaction $_9 __func::__clone()", t_58CB64);

// BuyMagicStoneDialog::checkTransaction $_9: __func::__clone(__base*)
static void t_58CBB4(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a5c0u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x58CBB4, "BuyMagicStoneDialog::checkTransaction $_9 __func::__clone(__base*)", t_58CBB4);

// BuyMagicStoneDialog::checkTransaction $_9: __func::destroy
static void t_58CBE8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x58CBE8, "BuyMagicStoneDialog::checkTransaction $_9 __func::destroy", t_58CBE8);

// BuyMagicStoneDialog::checkTransaction $_9: __func::destroy_deallocate
static void t_58CC08(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58CC08, "BuyMagicStoneDialog::checkTransaction $_9 __func::destroy_deallocate", t_58CC08);

// BuyMagicStoneDialog::checkTransaction $_9: __func::target
static uint32_t t_58CD74(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39b60u ? self + 8 : 0; }
RW_FUNCTION(0x58CD74, "BuyMagicStoneDialog::checkTransaction $_9 __func::target", t_58CD74);

// BuyMagicStoneDialog::checkTransaction $_9: __func::target_type
static uint32_t t_58CD94() { return 0xf8bc7cu; }
RW_FUNCTION(0x58CD94, "BuyMagicStoneDialog::checkTransaction $_9 __func::target_type", t_58CD94);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_: __func::~__func
static uint32_t t_58CDA8(uint32_t self) {
    st32(self, 0xf5a5ecu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x58CDA8, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ __func::~__func", t_58CDA8);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_: __func::~__func (deleting)
static void t_58CDE4(uint32_t self) {
    st32(self, 0xf5a5ecu);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58CDE4, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ __func::~__func (deleting)", t_58CDE4);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_: __func::__clone()
static uint32_t t_58CE24(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5a5ecu);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x58CE24, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ __func::__clone()", t_58CE24);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_: __func::__clone(__base*)
static void t_58CE74(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a5ecu);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x58CE74, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ __func::__clone(__base*)", t_58CE74);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_: __func::destroy
static void t_58CEA8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x58CEA8, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ __func::destroy", t_58CEA8);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_: __func::destroy_deallocate
static void t_58CEC8(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58CEC8, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ __func::destroy_deallocate", t_58CEC8);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_: __func::target
static uint32_t t_58D16C(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39c40u ? self + 8 : 0; }
RW_FUNCTION(0x58D16C, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ __func::target", t_58D16C);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_: __func::target_type
static uint32_t t_58D18C() { return 0xf8bc90u; }
RW_FUNCTION(0x58D18C, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ __func::target_type", t_58D18C);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_: __func::~__func
static uint32_t t_58D1A0(uint32_t self) {
    st32(self, 0xf5a618u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x58D1A0, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_ __func::~__func", t_58D1A0);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_: __func::~__func (deleting)
static void t_58D1DC(uint32_t self) {
    st32(self, 0xf5a618u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D1DC, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_ __func::~__func (deleting)", t_58D1DC);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_: __func::__clone()
static uint32_t t_58D21C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5a618u);
        call(0x45c84d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x58D21C, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_ __func::__clone()", t_58D21C);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_: __func::__clone(__base*)
static void t_58D26C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a618u);
        call(0x45c84d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x58D26C, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_ __func::__clone(__base*)", t_58D26C);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_: __func::destroy
static void t_58D2A0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x58D2A0, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_ __func::destroy", t_58D2A0);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_: __func::destroy_deallocate
static void t_58D2C0(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D2C0, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_ __func::destroy_deallocate", t_58D2C0);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_: __func::target
static uint32_t t_58D2F8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39d40u ? self + 8 : 0; }
RW_FUNCTION(0x58D2F8, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_ __func::target", t_58D2F8);

// BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_: __func::target_type
static uint32_t t_58D318() { return 0xf8bca4u; }
RW_FUNCTION(0x58D318, "BuyMagicStoneDialog::checkTransaction $_9 UlbE_ UlbE_ __func::target_type", t_58D318);

// BuyMagicStoneDialog::openStoneInfoPopup $_8: __func::~__func (deleting)
static void t_58D32C(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D32C, "BuyMagicStoneDialog::openStoneInfoPopup $_8 __func::~__func (deleting)", t_58D32C);

// BuyMagicStoneDialog::openStoneInfoPopup $_8: __func::__clone()
static uint32_t t_58D334(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a644u);
    }
    return v1;
}
RW_FUNCTION(0x58D334, "BuyMagicStoneDialog::openStoneInfoPopup $_8 __func::__clone()", t_58D334);

// BuyMagicStoneDialog::openStoneInfoPopup $_8: __func::__clone(__base*)
static void t_58D358(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a644u);
    }
}
RW_FUNCTION(0x58D358, "BuyMagicStoneDialog::openStoneInfoPopup $_8 __func::__clone(__base*)", t_58D358);

// BuyMagicStoneDialog::openStoneInfoPopup $_8: __func::destroy
static void t_58D374(uint32_t self) {
}
RW_FUNCTION(0x58D374, "BuyMagicStoneDialog::openStoneInfoPopup $_8 __func::destroy", t_58D374);

// BuyMagicStoneDialog::openStoneInfoPopup $_8: __func::destroy_deallocate
static void t_58D376(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D376, "BuyMagicStoneDialog::openStoneInfoPopup $_8 __func::destroy_deallocate", t_58D376);

// BuyMagicStoneDialog::openStoneInfoPopup $_8: __func::target
static uint32_t t_58D380(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39e20u ? self + 4 : 0; }
RW_FUNCTION(0x58D380, "BuyMagicStoneDialog::openStoneInfoPopup $_8 __func::target", t_58D380);

// BuyMagicStoneDialog::openStoneInfoPopup $_8: __func::target_type
static uint32_t t_58D3A0() { return 0xf8bcb8u; }
RW_FUNCTION(0x58D3A0, "BuyMagicStoneDialog::openStoneInfoPopup $_8 __func::target_type", t_58D3A0);

// BuyMagicStoneDialog::openStoneInfoPopup $_7: __func::~__func (deleting)
static void t_58D3B4(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D3B4, "BuyMagicStoneDialog::openStoneInfoPopup $_7 __func::~__func (deleting)", t_58D3B4);

// BuyMagicStoneDialog::openStoneInfoPopup $_7: __func::__clone()
static uint32_t t_58D3BC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5a670u);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x58D3BC, "BuyMagicStoneDialog::openStoneInfoPopup $_7 __func::__clone()", t_58D3BC);

// BuyMagicStoneDialog::openStoneInfoPopup $_7: __func::__clone(__base*)
static void t_58D3EC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a670u);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x58D3EC, "BuyMagicStoneDialog::openStoneInfoPopup $_7 __func::__clone(__base*)", t_58D3EC);

// BuyMagicStoneDialog::openStoneInfoPopup $_7: __func::destroy
static void t_58D414(uint32_t self) {
}
RW_FUNCTION(0x58D414, "BuyMagicStoneDialog::openStoneInfoPopup $_7 __func::destroy", t_58D414);

// BuyMagicStoneDialog::openStoneInfoPopup $_7: __func::destroy_deallocate
static void t_58D416(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D416, "BuyMagicStoneDialog::openStoneInfoPopup $_7 __func::destroy_deallocate", t_58D416);

// BuyMagicStoneDialog::openStoneInfoPopup $_7: __func::target
static uint32_t t_58D4FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39ed0u ? self + 4 : 0; }
RW_FUNCTION(0x58D4FC, "BuyMagicStoneDialog::openStoneInfoPopup $_7 __func::target", t_58D4FC);

// BuyMagicStoneDialog::openStoneInfoPopup $_7: __func::target_type
static uint32_t t_58D51C() { return 0xf8bcccu; }
RW_FUNCTION(0x58D51C, "BuyMagicStoneDialog::openStoneInfoPopup $_7 __func::target_type", t_58D51C);

// BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_: __func::~__func (deleting)
static void t_58D530(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D530, "BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_ __func::~__func (deleting)", t_58D530);

// BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_: __func::__clone()
static uint32_t t_58D538(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {16});
    if (v1) {
        st32(v1, 0xf5a69cu);
        uint32_t v2 = ld32(self + 4 + 0);
        uint32_t v3 = ld32(self + 4 + 4);
        uint32_t v4 = ld32(self + 4 + 8);
        st32(v1 + 4 + 0, v2);
        st32(v1 + 4 + 4, v3);
        st32(v1 + 4 + 8, v4);
    }
    return v1;
}
RW_FUNCTION(0x58D538, "BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_ __func::__clone()", t_58D538);

// BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_: __func::__clone(__base*)
static void t_58D568(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a69cu);
        uint32_t v1 = ld32(self + 4);
        uint32_t v2 = ld32(self + 8);
        uint32_t v3 = ld32(self + 12);
        st32(p + 12, v3);
        st32(p + 8, v2);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x58D568, "BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_ __func::__clone(__base*)", t_58D568);

// BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_: __func::destroy
static void t_58D590(uint32_t self) {
}
RW_FUNCTION(0x58D590, "BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_ __func::destroy", t_58D590);

// BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_: __func::destroy_deallocate
static void t_58D592(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D592, "BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_ __func::destroy_deallocate", t_58D592);

// BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_: __func::target
static uint32_t t_58D5C4(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe39f90u ? self + 4 : 0; }
RW_FUNCTION(0x58D5C4, "BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_ __func::target", t_58D5C4);

// BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_: __func::target_type
static uint32_t t_58D5E4() { return 0xf8bce0u; }
RW_FUNCTION(0x58D5E4, "BuyMagicStoneDialog::openStoneInfoPopup $_17 UlvE_ __func::target_type", t_58D5E4);

// BuyMagicStoneDialog::openStoneInfoPopup $_6: __func::~__func (deleting)
static void t_58D5F8(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D5F8, "BuyMagicStoneDialog::openStoneInfoPopup $_6 __func::~__func (deleting)", t_58D5F8);

// BuyMagicStoneDialog::openStoneInfoPopup $_6: __func::__clone()
static uint32_t t_58D600(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a6c8u);
        uint32_t v2 = ld32(self + 4);
        st32(v1 + 4, v2);
    }
    return v1;
}
RW_FUNCTION(0x58D600, "BuyMagicStoneDialog::openStoneInfoPopup $_6 __func::__clone()", t_58D600);

// BuyMagicStoneDialog::openStoneInfoPopup $_6: __func::__clone(__base*)
static void t_58D62C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a6c8u);
        uint32_t v1 = ld32(self + 4);
        st32(p + 4, v1);
    }
}
RW_FUNCTION(0x58D62C, "BuyMagicStoneDialog::openStoneInfoPopup $_6 __func::__clone(__base*)", t_58D62C);

// BuyMagicStoneDialog::openStoneInfoPopup $_6: __func::destroy
static void t_58D64C(uint32_t self) {
}
RW_FUNCTION(0x58D64C, "BuyMagicStoneDialog::openStoneInfoPopup $_6 __func::destroy", t_58D64C);

// BuyMagicStoneDialog::openStoneInfoPopup $_6: __func::destroy_deallocate
static void t_58D64E(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D64E, "BuyMagicStoneDialog::openStoneInfoPopup $_6 __func::destroy_deallocate", t_58D64E);

// BuyMagicStoneDialog::openStoneInfoPopup $_6: __func::target
static uint32_t t_58D660(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3a050u ? self + 4 : 0; }
RW_FUNCTION(0x58D660, "BuyMagicStoneDialog::openStoneInfoPopup $_6 __func::target", t_58D660);

// BuyMagicStoneDialog::openStoneInfoPopup $_6: __func::target_type
static uint32_t t_58D680() { return 0xf8bcf4u; }
RW_FUNCTION(0x58D680, "BuyMagicStoneDialog::openStoneInfoPopup $_6 __func::target_type", t_58D680);

// BuyMagicStoneDialog::openStoneInfoPopup $_5: __func::~__func (deleting)
static void t_58D694(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D694, "BuyMagicStoneDialog::openStoneInfoPopup $_5 __func::~__func (deleting)", t_58D694);

// BuyMagicStoneDialog::openStoneInfoPopup $_5: __func::__clone()
static uint32_t t_58D69C(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a6f4u);
    }
    return v1;
}
RW_FUNCTION(0x58D69C, "BuyMagicStoneDialog::openStoneInfoPopup $_5 __func::__clone()", t_58D69C);

// BuyMagicStoneDialog::openStoneInfoPopup $_5: __func::__clone(__base*)
static void t_58D6C0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a6f4u);
    }
}
RW_FUNCTION(0x58D6C0, "BuyMagicStoneDialog::openStoneInfoPopup $_5 __func::__clone(__base*)", t_58D6C0);

// BuyMagicStoneDialog::openStoneInfoPopup $_5: __func::destroy
static void t_58D6DC(uint32_t self) {
}
RW_FUNCTION(0x58D6DC, "BuyMagicStoneDialog::openStoneInfoPopup $_5 __func::destroy", t_58D6DC);

// BuyMagicStoneDialog::openStoneInfoPopup $_5: __func::destroy_deallocate
static void t_58D6DE(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D6DE, "BuyMagicStoneDialog::openStoneInfoPopup $_5 __func::destroy_deallocate", t_58D6DE);

// BuyMagicStoneDialog::openStoneInfoPopup $_5: __func::target
static uint32_t t_58D6FC(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3a130u ? self + 4 : 0; }
RW_FUNCTION(0x58D6FC, "BuyMagicStoneDialog::openStoneInfoPopup $_5 __func::target", t_58D6FC);

// BuyMagicStoneDialog::openStoneInfoPopup $_5: __func::target_type
static uint32_t t_58D71C() { return 0xf8bd08u; }
RW_FUNCTION(0x58D71C, "BuyMagicStoneDialog::openStoneInfoPopup $_5 __func::target_type", t_58D71C);

// BuyMagicStoneDialog::callSendProductsRequest : __func::~__func
static uint32_t t_58D730(uint32_t self) {
    st32(self, 0xf5a720u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x58D730, "BuyMagicStoneDialog::callSendProductsRequest  __func::~__func", t_58D730);

// BuyMagicStoneDialog::callSendProductsRequest : __func::~__func (deleting)
static void t_58D76C(uint32_t self) {
    st32(self, 0xf5a720u);
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D76C, "BuyMagicStoneDialog::callSendProductsRequest  __func::~__func (deleting)", t_58D76C);

// BuyMagicStoneDialog::callSendProductsRequest : __func::__clone()
static uint32_t t_58D7AC(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {32});
    if (v1) {
        st32(v1, 0xf5a720u);
        call(0x58996d, {v1 + 8, self + 8});   // _ZNSt3__18functionIFvbPKNS_13unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEN3iap11ProductInfoENS_4hashIS7_EENS_8equal_toIS7_EENS5_INS_4pairIKS7_S9_EEEEEEEEC2ERKSM_
    }
    return v1;
}
RW_FUNCTION(0x58D7AC, "BuyMagicStoneDialog::callSendProductsRequest  __func::__clone()", t_58D7AC);

// BuyMagicStoneDialog::callSendProductsRequest : __func::__clone(__base*)
static void t_58D7FC(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a720u);
        call(0x58996d, {p + 8, self + 8});   // _ZNSt3__18functionIFvbPKNS_13unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEEN3iap11ProductInfoENS_4hashIS7_EENS_8equal_toIS7_EENS5_INS_4pairIKS7_S9_EEEEEEEEC2ERKSM_
    }
}
RW_FUNCTION(0x58D7FC, "BuyMagicStoneDialog::callSendProductsRequest  __func::__clone(__base*)", t_58D7FC);

// BuyMagicStoneDialog::callSendProductsRequest : __func::destroy
static void t_58D830(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x58D830, "BuyMagicStoneDialog::callSendProductsRequest  __func::destroy", t_58D830);

// BuyMagicStoneDialog::callSendProductsRequest : __func::destroy_deallocate
static void t_58D850(uint32_t self) {
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D850, "BuyMagicStoneDialog::callSendProductsRequest  __func::destroy_deallocate", t_58D850);

// BuyMagicStoneDialog::callSendProductsRequest : __func::target
static uint32_t t_58D898(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3a2b0u ? self + 8 : 0; }
RW_FUNCTION(0x58D898, "BuyMagicStoneDialog::callSendProductsRequest  __func::target", t_58D898);

// BuyMagicStoneDialog::callSendProductsRequest : __func::target_type
static uint32_t t_58D8B8() { return 0xf8bd1cu; }
RW_FUNCTION(0x58D8B8, "BuyMagicStoneDialog::callSendProductsRequest  __func::target_type", t_58D8B8);

// BuyMagicStoneDialog::callGetPaymentInfo $_3: __func::~__func
static uint32_t t_58D8CC(uint32_t self) {
    st32(self, 0xf5a74cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x58D8CC, "BuyMagicStoneDialog::callGetPaymentInfo $_3 __func::~__func", t_58D8CC);

// BuyMagicStoneDialog::callGetPaymentInfo $_3: __func::~__func (deleting)
static void t_58D908(uint32_t self) {
    st32(self, 0xf5a74cu);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D908, "BuyMagicStoneDialog::callGetPaymentInfo $_3 __func::~__func (deleting)", t_58D908);

// BuyMagicStoneDialog::callGetPaymentInfo $_3: __func::__clone()
static uint32_t t_58D948(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5a74cu);
        call(0x45c84d, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x58D948, "BuyMagicStoneDialog::callGetPaymentInfo $_3 __func::__clone()", t_58D948);

// BuyMagicStoneDialog::callGetPaymentInfo $_3: __func::__clone(__base*)
static void t_58D998(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a74cu);
        call(0x45c84d, {p + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x58D998, "BuyMagicStoneDialog::callGetPaymentInfo $_3 __func::__clone(__base*)", t_58D998);

// BuyMagicStoneDialog::callGetPaymentInfo $_3: __func::destroy
static void t_58D9CC(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x58D9CC, "BuyMagicStoneDialog::callGetPaymentInfo $_3 __func::destroy", t_58D9CC);

// BuyMagicStoneDialog::callGetPaymentInfo $_3: __func::destroy_deallocate
static void t_58D9EC(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58D9EC, "BuyMagicStoneDialog::callGetPaymentInfo $_3 __func::destroy_deallocate", t_58D9EC);

// BuyMagicStoneDialog::callGetPaymentInfo $_3: __func::target
static uint32_t t_58DBC0(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3a450u ? self + 8 : 0; }
RW_FUNCTION(0x58DBC0, "BuyMagicStoneDialog::callGetPaymentInfo $_3 __func::target", t_58DBC0);

// BuyMagicStoneDialog::callGetPaymentInfo $_3: __func::target_type
static uint32_t t_58DBE0() { return 0xf8bd30u; }
RW_FUNCTION(0x58DBE0, "BuyMagicStoneDialog::callGetPaymentInfo $_3 __func::target_type", t_58DBE0);

// BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_: __func::~__func
static uint32_t t_58DBF4(uint32_t self) {
    st32(self, 0xf5a778u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    return self;
}
RW_FUNCTION(0x58DBF4, "BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_ __func::~__func", t_58DBF4);

// BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_: __func::~__func (deleting)
static void t_58DC30(uint32_t self) {
    st32(self, 0xf5a778u);
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58DC30, "BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_ __func::~__func (deleting)", t_58DC30);

// BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_: __func::__clone()
static uint32_t t_58DC70(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {40});
    if (v1) {
        st32(v1, 0xf5a778u);
        call(0x45c84d, {v1 + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
    return v1;
}
RW_FUNCTION(0x58DC70, "BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_ __func::__clone()", t_58DC70);

// BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_: __func::__clone(__base*)
static void t_58DCC0(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a778u);
        call(0x45c84d, {p + 16, self + 16});   // _ZNSt3__18functionIFvbEEC2ERKS2_
    }
}
RW_FUNCTION(0x58DCC0, "BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_ __func::__clone(__base*)", t_58DCC0);

// BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_: __func::destroy
static void t_58DCF4(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
}
RW_FUNCTION(0x58DCF4, "BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_ __func::destroy", t_58DCF4);

// BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_: __func::destroy_deallocate
static void t_58DD14(uint32_t self) {
    uint32_t v1 = ld32(self + 32);
    functionDestroy(self + 16);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58DD14, "BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_ __func::destroy_deallocate", t_58DD14);

// BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_: __func::target
static uint32_t t_58DE04(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3a530u ? self + 8 : 0; }
RW_FUNCTION(0x58DE04, "BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_ __func::target", t_58DE04);

// BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_: __func::target_type
static uint32_t t_58DE24() { return 0xf8bd44u; }
RW_FUNCTION(0x58DE24, "BuyMagicStoneDialog::callGetPaymentInfo $_3 UlbE_ __func::target_type", t_58DE24);

// BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_: __func::~__func (deleting)
static void t_58DE38(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58DE38, "BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_ __func::~__func (deleting)", t_58DE38);

// BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_: __func::__clone()
static uint32_t t_58DE40(uint32_t self) {
    uint32_t v1 = call(fn::operator_new, {8});
    if (v1) {
        st32(v1, 0xf5a7a4u);
    }
    return v1;
}
RW_FUNCTION(0x58DE40, "BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_ __func::__clone()", t_58DE40);

// BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_: __func::__clone(__base*)
static void t_58DE64(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a7a4u);
    }
}
RW_FUNCTION(0x58DE64, "BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_ __func::__clone(__base*)", t_58DE64);

// BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_: __func::destroy
static void t_58DE80(uint32_t self) {
}
RW_FUNCTION(0x58DE80, "BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_ __func::destroy", t_58DE80);

// BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_: __func::destroy_deallocate
static void t_58DE82(uint32_t self) {
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58DE82, "BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_ __func::destroy_deallocate", t_58DE82);

// BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_: __func::target
static uint32_t t_58DE98(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3a620u ? self + 4 : 0; }
RW_FUNCTION(0x58DE98, "BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_ __func::target", t_58DE98);

// BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_: __func::target_type
static uint32_t t_58DEB8() { return 0xf8bd58u; }
RW_FUNCTION(0x58DEB8, "BuyMagicStoneDialog::callGetPaymentInfo $_16 UlbE_ __func::target_type", t_58DEB8);

// BuyMagicStoneDialog::init : __func::~__func
static uint32_t t_58DECC(uint32_t self) {
    st32(self, 0xf5a7d0u);
    call(0x478961, {self + 48});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(0x47363f, {self + 8});   // _ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev
    return self;
}
RW_FUNCTION(0x58DECC, "BuyMagicStoneDialog::init  __func::~__func", t_58DECC);

// BuyMagicStoneDialog::init : __func::~__func (deleting)
static void t_58DF1C(uint32_t self) {
    st32(self, 0xf5a7d0u);
    call(0x478961, {self + 48});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(0x47363f, {self + 8});   // _ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58DF1C, "BuyMagicStoneDialog::init  __func::~__func (deleting)", t_58DF1C);

// BuyMagicStoneDialog::init : __func::destroy
static void t_58E0B4(uint32_t self) {
    call(0x478961, {self + 48});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(0x47363f, {self + 8});   // _ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev
}
RW_FUNCTION(0x58E0B4, "BuyMagicStoneDialog::init  __func::destroy", t_58E0B4);

// BuyMagicStoneDialog::init : __func::destroy_deallocate
static void t_58E0E8(uint32_t self) {
    call(0x478961, {self + 48});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 40);
    functionDestroy(self + 24);
    call(0x47363f, {self + 8});   // _ZNSt3__113__vector_baseINS_12basic_stringIcNS_11char_traitsIcEENS_9allocatorIcEEEENS4_IS6_EEED2Ev
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58E0E8, "BuyMagicStoneDialog::init  __func::destroy_deallocate", t_58E0E8);

// BuyMagicStoneDialog::init : __func::target
static uint32_t t_58E2D8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3a7b0u ? self + 8 : 0; }
RW_FUNCTION(0x58E2D8, "BuyMagicStoneDialog::init  __func::target", t_58E2D8);

// BuyMagicStoneDialog::init : __func::target_type
static uint32_t t_58E2FC() { return 0xf8bd6cu; }
RW_FUNCTION(0x58E2FC, "BuyMagicStoneDialog::init  __func::target_type", t_58E2FC);

// BuyMagicStoneDialog::init : __func::~__func
static uint32_t t_58E398(uint32_t self) {
    st32(self, 0xf5a7fcu);
    call(0x478961, {self + 32});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x58E398, "BuyMagicStoneDialog::init  __func::~__func", t_58E398);

// BuyMagicStoneDialog::init : __func::~__func (deleting)
static void t_58E3E0(uint32_t self) {
    st32(self, 0xf5a7fcu);
    call(0x478961, {self + 32});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58E3E0, "BuyMagicStoneDialog::init  __func::~__func (deleting)", t_58E3E0);

// BuyMagicStoneDialog::init : __func::__clone(__base*)
static void t_58E4B8(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a7fcu);
        call(0x589771, {p + 8, self + 8});   // _ZNSt3__18functionIFvRKNS_6vectorIiNS_9allocatorIiEEEEPKNS_13unordered_mapINS_12basic_stringIcNS_11char_traitsIcEENS2_IcEEEEN3iap11ProductInfoENS_4hashISC_EENS_8equal_toISC_EENS2_INS_4pairIKSC_SE_EEEEEEbEEC2ERKSR_
        call(0x4af707, {p + 32, self + 32});   // _ZNSt3__16vectorIiNS_9allocatorIiEEEC2ERKS3_
    }
}
RW_FUNCTION(0x58E4B8, "BuyMagicStoneDialog::init  __func::__clone(__base*)", t_58E4B8);

// BuyMagicStoneDialog::init : __func::destroy
static void t_58E52C(uint32_t self) {
    call(0x478961, {self + 32});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x58E52C, "BuyMagicStoneDialog::init  __func::destroy", t_58E52C);

// BuyMagicStoneDialog::init : __func::destroy_deallocate
static void t_58E552(uint32_t self) {
    call(0x478961, {self + 32});   // _ZNSt3__113__vector_baseIiNS_9allocatorIiEEED2Ev
    uint32_t v1 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58E552, "BuyMagicStoneDialog::init  __func::destroy_deallocate", t_58E552);

// BuyMagicStoneDialog::init : __func::target
static uint32_t t_58E5B8(uint32_t self, uint32_t ti) { return ld32(ti + 4) == 0xe3aa00u ? self + 8 : 0; }
RW_FUNCTION(0x58E5B8, "BuyMagicStoneDialog::init  __func::target", t_58E5B8);

// BuyMagicStoneDialog::init : __func::target_type
static uint32_t t_58E5D8() { return 0xf8bd80u; }
RW_FUNCTION(0x58E5D8, "BuyMagicStoneDialog::init  __func::target_type", t_58E5D8);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func
static uint32_t t_58E5EC(uint32_t self) {
    st32(self, 0xf5a828u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    return self;
}
RW_FUNCTION(0x58E5EC, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func", t_58E5EC);

// BuyMagicStoneDialog::openDiscount $_1: __func::~__func (deleting)
static void t_58E644(uint32_t self) {
    st32(self, 0xf5a828u);
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58E644, "BuyMagicStoneDialog::openDiscount $_1 __func::~__func (deleting)", t_58E644);

// BuyMagicStoneDialog::openDiscount $_1: __func::__clone(__base*)
static void t_58E72C(uint32_t self, uint32_t p) {
    if (p) {
        st32(p, 0xf5a828u);
        call(0x4683cd, {p + 8, self + 8});   // _ZNSt3__18functionIFvvEEC2ERKS2_
        call(0x4683cd, {p + 32, self + 32});   // _ZNSt3__18functionIFvvEEC2ERKS2_
    }
}
RW_FUNCTION(0x58E72C, "BuyMagicStoneDialog::openDiscount $_1 __func::__clone(__base*)", t_58E72C);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy
static void t_58E7A0(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
}
RW_FUNCTION(0x58E7A0, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy", t_58E7A0);

// BuyMagicStoneDialog::openDiscount $_1: __func::destroy_deallocate
static void t_58E7E0(uint32_t self) {
    uint32_t v1 = ld32(self + 48);
    functionDestroy(self + 32);
    uint32_t v2 = ld32(self + 24);
    functionDestroy(self + 8);
    call(fn::operator_delete, {self});
}
RW_FUNCTION(0x58E7E0, "BuyMagicStoneDialog::openDiscount $_1 __func::destroy_deallocate", t_58E7E0);
