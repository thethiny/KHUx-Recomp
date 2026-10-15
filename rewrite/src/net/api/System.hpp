// System responses: the web page URLs (SystemNeedUrl), the COPPA misc values (SystemCoppa), the information page
// URLs (SystemInformationList / 151203), the invite code (SystemInvitecode), and the player's birthday
// (UserBirthday, a SafeBuffer).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserBirthday : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F474;
    static constexpr uint32_t MONTH = 0;      // birthday / 100
    static constexpr uint32_t DAY = 4;        // birthday % 100

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static uint32_t destroyDelete(UserBirthday *self);           // D0
};
RW_SIZE(UserBirthday, 32);

// The URLs of the in-game web pages.
struct SystemNeedUrl {
    static constexpr int COUNT = 10;
    // support, register, update, help, staff, agreement, license, shikin, tokutei, store
    StdString urls[COUNT];                    // +0

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static SystemNeedUrl *destroy(SystemNeedUrl *self);           // D2
};
RW_SIZE(SystemNeedUrl, 0x78);

// response["misc"]: an unordered_map<unsigned, unsigned> of misc ids 116 and 900..907.
struct SystemCoppa {
    const uint8_t values[20];                       // std::unordered_map<unsigned, unsigned>

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(SystemCoppa, 20);

struct SystemInformationList {
    StdString viewUrl;                        // +0

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(SystemInformationList, 12);

struct SystemInformationList151203 {
    StdVector popUpViewUrls;                  // +0 std::vector<std::string> ("popUpViewUrl")

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(SystemInformationList151203, 12);

struct SystemInvitecode {
    StdString inviteCode;                     // +0
    rw::Field<int32_t> invitedNumber;         // +12
    rw::Field<int32_t> inviteRemainingNumber; // +16

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(SystemInvitecode, 20);

}  // namespace hole::network::api
