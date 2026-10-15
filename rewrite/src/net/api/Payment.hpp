// Payment responses: the purchase limits (PaymentInfo), a purchase's bridge transaction id (PurchasePrepare), and
// the shop benefits the player holds (UserShop).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct PaymentInfo : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F458;
    static constexpr uint32_t CESA_LIMIT_AGE = 0;
    static constexpr uint32_t CHARGE_MONEY_THIS_MONTH = 4;
    static constexpr uint32_t LAST_CHARGE_MONEY_THIS_MONTH = 8;

    static uint32_t parse(SharedPtr *out, const JsonValue *response);   // response["paymentInfo"]
    static uint32_t destroyDelete(PaymentInfo *self);                  // D0
};
RW_SIZE(PaymentInfo, 32);

struct PurchasePrepare : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F3E8;
    static constexpr uint32_t BRIDGE_TRANS_ID = 0;   // unsigned

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static uint32_t destroyDelete(PurchasePrepare *self);              // D0
};
RW_SIZE(PurchasePrepare, 32);

struct UserShop {
    struct Benefit {
        rw::Field<int32_t> benefitId;         // +0
        const uint8_t endDatetime[8];               // +4 Date

        // {"benefitId", "endDatetime"}: Date::init's result once benefitId is in (0 before that). The object is
        // not type-checked (rapidjson's lookup on a non-object gives its null value).
        static uint32_t init(Benefit *self, const JsonValue *json);
    };

    StdVector benefits;                       // +0 std::vector<Benefit> ("userBenefits")

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    // benefits.push_back(benefit): the vector when there was room, else what the slow path returns
    static uint32_t addBenefit(UserShop *self, const Benefit *benefit);
};
RW_SIZE(UserShop::Benefit, 12);
RW_SIZE(UserShop, 12);

}  // namespace hole::network::api
