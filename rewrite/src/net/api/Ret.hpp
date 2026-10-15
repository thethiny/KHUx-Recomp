// Ret: the "ret" header every API response carries (maintenance / session flags, client and data versions, the
// server time, and an error message with its help URL). The API layer parses it first for every action.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct Ret {
    rw::Field<int32_t> action;             // +0  the API action this response answers (set last, on success)
    rw::Field<uint8_t> valid;              // +4  1 from the constructor (make_shared)
    rw::Field<uint8_t> isMaintenance;      // +5
    rw::Field<uint8_t> sessionTimeout;     // +6  "sessionTO"
    rw::Field<uint8_t> isNewDayPeriod;     // +7
    StdString error;                       // +8  message ("\\n" sequences turned into newlines)
    StdString viewUrl;                     // +20 help page for the error
    StdString versionApp;                  // +32
    rw::Field<int32_t> versionRes;         // +44 resource (misc.mp4) revision
    rw::Field<int32_t> versionDat;         // +48 master data revision
    rw::Field<int32_t> functionFlags;      // +52
    const uint8_t serverTime[8];                 // +56 Date

    // response["ret"] -> *out (empty when the header is absent or incomplete; "error" / "viewUrl" are optional)
    static uint32_t parse(SharedPtr *out, int32_t action, const JsonValue *response);
};
RW_LAYOUT(Ret, isMaintenance, 5);
RW_LAYOUT(Ret, error, 8);
RW_LAYOUT(Ret, viewUrl, 20);
RW_LAYOUT(Ret, versionApp, 32);
RW_LAYOUT(Ret, versionRes, 44);
RW_LAYOUT(Ret, functionFlags, 52);
RW_LAYOUT(Ret, serverTime, 56);
RW_SIZE(Ret, 64);

// Replace every occurrence of `from` in *self with `to` (left to right, continuing after each replacement).
uint32_t ReplaceString(StdString *self, const StdString *from, const StdString *to);

}  // namespace hole::network::api
