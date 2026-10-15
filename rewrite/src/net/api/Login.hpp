// Login (action "login": {"login": {newcomer, tutorial, acquirableLoginBonus, progression}}) and LoginBonus
// ({"loginBonus": {"continue": {...}, "total": {...}, "specials": [ids]}}).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

// Login flags, kept in a SafeBuffer of 8 bytes.
struct Login : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F5E0;
    static constexpr uint32_t SIZE = 8;
    static constexpr uint32_t NEWCOMER = 0;                  // bool (1 byte)
    static constexpr uint32_t TUTORIAL = 1;                  // bool
    static constexpr uint32_t ACQUIRABLE_LOGIN_BONUS = 2;    // bool
    static constexpr uint32_t PROGRESSION = 4;               // int

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static uint32_t destroyDelete(Login *self);              // D0
};
RW_SIZE(Login, 32);

struct LoginBonus {
    // one bonus track: which bonus and how many days so far
    struct DailyBonus {
        rw::Field<int32_t> loginBonusId;     // +0
        rw::Field<int32_t> dayCount;         // +4
        // {"loginBonusId", "dayCount"}: 1 when both are ints (fields set so far stay), else 0
        static uint32_t init(DailyBonus *self, const JsonValue *json);
    };

    DailyBonus continueBonus;                // +0  "continue"
    DailyBonus total;                        // +8  "total"
    StdVector specials;                      // +16 std::vector<int>

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(LoginBonus, specials, 16);
RW_SIZE(LoginBonus, 28);

}  // namespace hole::network::api
