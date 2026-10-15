#include "Login.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t LOGIN_SHARED_PTR_CTOR = 0x49E1B9;       // std::shared_ptr<Login>::shared_ptr(Login*)
constexpr uint32_t LOGIN_BONUS_SHARED_PTR_CTOR = 0x49E3ED; // std::shared_ptr<LoginBonus>::shared_ptr(LoginBonus*)
constexpr uint32_t VECTOR_INT_PUSH_SLOW = 0x47D709;        // std::vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t VECTOR_INT_BASE_DTOR = 0x478961;        // std::__vector_base<int>::~__vector_base()

// A bool member: present when it has the bool flag; its value is "is true".
bool readBool(Login *self, const JsonValue &obj, const char *key, uint32_t index) {
    uint32_t flags = obj[key].flags;
    if (!(flags & JsonValue::kBoolFlag)) return false;
    uint8_t value = flags == JsonValue::kTrueFlag ? 1 : 0;
    writeBytes(self, index, &value, 1);
    return true;
}
}  // namespace

uint32_t Login::parse(SharedPtr *out, const JsonValue *response) {
    if (response->IsObject()) {
        const JsonValue &login = (*response)["login"];
        if (login.IsObject()) {
            Login *self = newObject<Login>(32);
            constructSafeBuffer(self, SIZE);
            self->vtable = VTABLE;
            bool ok = readBool(self, login, "newcomer", NEWCOMER) && readBool(self, login, "tutorial", TUTORIAL) &&
                      readBool(self, login, "acquirableLoginBonus", ACQUIRABLE_LOGIN_BONUS);
            if (ok) {
                const JsonValue &progression = login["progression"];
                ok = progression.IsInt();
                if (ok) writeU32(self, PROGRESSION, progression.GetUint());
            }
            if (!ok) {
                deleteVirtual(self);
                self = nullptr;
            }
            rw::call(LOGIN_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
            return 0;   // stack-protector check
        }
    }
    out->reset();
    return 0;
}
RW_FUNCTION(0x49DFD8, "hole::network::api::Login::parse", Login::parse);

uint32_t Login::destroyDelete(Login *self) {
    return deleteSafeBufferResponse(self);
}
RW_FUNCTION(0x4B6DC0, "hole::network::api::Login::~Login (deleting)", Login::destroyDelete);

uint32_t LoginBonus::DailyBonus::init(DailyBonus *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &id = (*json)["loginBonusId"];
    if (!id.IsInt()) return 0;
    self->loginBonusId = id.GetInt();
    const JsonValue &days = (*json)["dayCount"];
    if (!days.IsInt()) return 0;
    self->dayCount = days.GetInt();
    return 1;
}
RW_FUNCTION(0x49E20C, "hole::network::api::LoginBonus::DailyBonus::init", LoginBonus::DailyBonus::init);

uint32_t LoginBonus::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x38);                      // original frame; the pushed int local at +0x1c
    constexpr uint32_t LOCAL_VALUE = 0x1C;

    LoginBonus *self = newObject<LoginBonus>(sizeof(LoginBonus));
    self->specials.begin = 0;
    self->specials.end = 0;
    self->specials.capEnd = 0;

    // all-or-nothing: any missing / mistyped part frees the object
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &bonus = (*response)["loginBonus"];
        if (!bonus.IsObject()) return false;
        const JsonValue &cont = bonus["continue"];
        if (!cont.IsObject() || DailyBonus::init(&self->continueBonus, &cont) != 1) return false;
        const JsonValue &total = bonus["total"];
        if (!total.IsObject() || DailyBonus::init(&self->total, &total) != 1) return false;
        const JsonValue &specials = bonus["specials"];
        if (!specials.IsArray()) return false;
        uint32_t n = specials.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &v = specials.At(k);
            if (!v.IsInt()) return false;
            uint32_t id = v.GetUint();
            rw::store32(frame.at<uint32_t>(LOCAL_VALUE), id);
            uint32_t end = self->specials.end;
            if (end >= self->specials.capEnd) {
                rw::call(VECTOR_INT_PUSH_SLOW, {rw::guest_addr(&self->specials), frame.addr(LOCAL_VALUE)});
            } else {
                // inlined push_back (a null end pointer becomes 4: libc++'s placement-new null check)
                uint32_t base = 0;
                if (end) {
                    rw::save<uint32_t>(end, id);
                    base = self->specials.end;
                }
                self->specials.end = base + 4;
            }
        }
        return true;
    };

    LoginBonus *result = self;
    if (!parseAll()) {
        rw::call(VECTOR_INT_BASE_DTOR, {rw::guest_addr(&self->specials)});
        guestDelete(rw::guest_addr(self));
        result = nullptr;
    }
    rw::call(LOGIN_BONUS_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(result), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x49E270, "hole::network::api::LoginBonus::parse", LoginBonus::parse);

}  // namespace hole::network::api
