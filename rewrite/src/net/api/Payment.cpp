#include "Payment.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t PAYMENT_SHARED_PTR_CTOR = 0x4A71E9;    // std::shared_ptr<PaymentInfo>::shared_ptr(...)
constexpr uint32_t PREPARE_SHARED_PTR_CTOR = 0x4A978D;    // std::shared_ptr<PurchasePrepare>::shared_ptr(...)
constexpr uint32_t SHOP_SHARED_PTR_CTOR = 0x4A9949;       // std::shared_ptr<UserShop>::shared_ptr(UserShop*)
constexpr uint32_t BENEFIT_PUSH_SLOW = 0x4B3C49;          // vector<UserShop::Benefit>::__push_back_slow_path
constexpr uint32_t BENEFIT_VECTOR_BASE_DTOR = 0x4B3C09;   // __vector_base<UserShop::Benefit>::~__vector_base
}  // namespace

// ---------------------------------------------------------------------------------------------- PaymentInfo
uint32_t PaymentInfo::parse(SharedPtr *out, const JsonValue *response) {
    // the object is made before the response is looked at
    PaymentInfo *self = newObject<PaymentInfo>(32);
    constructSafeBuffer(self, 12);
    self->vtable = VTABLE;
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &info = (*response)["paymentInfo"];
        if (info.IsObject()) {
            static const char *const kKeys[] = {"cesaLimitAge", "chargeMoneyThisMonth", "lastchargeMoneyThisMonth"};
            ok = true;
            for (uint32_t k = 0; k < 3 && ok; k++) {
                const JsonValue &v = info[kKeys[k]];
                ok = v.IsInt();
                if (ok) writeU32(self, 4 * k, v.GetUint());
            }
        }
    }
    if (!ok) {
        deleteVirtual(self);
        self = nullptr;
    }
    rw::call(PAYMENT_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A707C, "hole::network::api::PaymentInfo::parse", PaymentInfo::parse);

uint32_t PaymentInfo::destroyDelete(PaymentInfo *self) { return deleteSafeBufferResponse(self); }
RW_FUNCTION(0x4B4EBC, "hole::network::api::PaymentInfo::~PaymentInfo (deleting)", PaymentInfo::destroyDelete);

// ---------------------------------------------------------------------------------------------- PurchasePrepare
uint32_t PurchasePrepare::parse(SharedPtr *out, const JsonValue *response) {
    PurchasePrepare *self = newObject<PurchasePrepare>(32);
    constructSafeBuffer(self, 4);
    self->vtable = VTABLE;
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &id = (*response)["bridgeTransId"];
        if (id.IsUint()) {
            writeU32(self, BRIDGE_TRANS_ID, id.GetUint());
            ok = true;
        }
    }
    if (!ok) {
        deleteVirtual(self);
        self = nullptr;
    }
    rw::call(PREPARE_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A96B8, "hole::network::api::PurchasePrepare::parse", PurchasePrepare::parse);

uint32_t PurchasePrepare::destroyDelete(PurchasePrepare *self) { return deleteSafeBufferResponse(self); }
RW_FUNCTION(0x4B3DC4, "hole::network::api::PurchasePrepare::~PurchasePrepare (deleting)", PurchasePrepare::destroyDelete);

// ---------------------------------------------------------------------------------------------- UserShop
uint32_t UserShop::Benefit::init(Benefit *self, const JsonValue *json) {
    const JsonValue &id = (*json)["benefitId"];
    if (!id.IsInt()) return 0;
    self->benefitId = id.GetInt();
    const JsonValue &end = (*json)["endDatetime"];
    if (!end.IsString()) return 0;
    return rw::call(guest::Date_init, {rw::guest_addr(self->endDatetime), end.GetString(), 0});
}
RW_FUNCTION(0x4A97E0, "hole::network::api::UserShop::Benefit::init", UserShop::Benefit::init);

uint32_t UserShop::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x38);                   // original frame; the Benefit local at +0x14
    Benefit *local = frame.at<Benefit>(0x14);
    UserShop *self = newObject<UserShop>(sizeof(UserShop));
    self->benefits.begin = 0;
    self->benefits.end = 0;
    self->benefits.capEnd = 0;
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &list = (*response)["userBenefits"];
        if (!list.IsArray()) return false;
        uint32_t n = list.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = list.At(k);
            if (!entry.IsObject()) return false;
            const uint8_t *flags = local->endDatetime + 7;
            rw::store8(flags, uint8_t(*flags & 0xFD));
            if (Benefit::init(local, &entry) != 1) return false;
            addBenefit(self, local);
        }
        return true;
    };
    if (!parseAll()) {
        rw::call(BENEFIT_VECTOR_BASE_DTOR, {rw::guest_addr(&self->benefits)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(SHOP_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A9844, "hole::network::api::UserShop::parse", UserShop::parse);

uint32_t UserShop::addBenefit(UserShop *self, const Benefit *benefit) {
    uint32_t end = self->benefits.end;
    if (end == self->benefits.capEnd)
        return rw::call(BENEFIT_PUSH_SLOW, {rw::guest_addr(&self->benefits), rw::guest_addr(benefit)});
    uint32_t base = 0;
    if (end) {
        for (uint32_t w = 0; w < 12; w += 4) rw::save<uint32_t>(end + w, rw::get<uint32_t>(benefit, w));
        base = self->benefits.end;
    }
    self->benefits.end = base + sizeof(Benefit);
    return rw::guest_addr(self);
}
RW_FUNCTION(0x4C4FB8, "hole::network::api::UserShop::addBenefit", UserShop::addBenefit);

}  // namespace hole::network::api
