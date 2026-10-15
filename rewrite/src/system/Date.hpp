// Date / Time: the client's calendar values ("YYYY-MM-DD hh:mm:ss" from the server), and hole::time, the offset
// between the server's clock and the device clock (set from the server's reference date at login).
#pragma once

#include "rw/rw.hpp"
#include "core/Guest.hpp"

// A calendar date and time of day, 8 bytes.
struct Date {
    rw::Field<uint16_t> year;    // +0
    rw::Field<uint8_t> month;    // +2 1..12
    rw::Field<uint8_t> day;      // +3 1..31
    rw::Field<uint8_t> hour;     // +4
    rw::Field<uint8_t> minute;   // +5
    rw::Field<uint8_t> second;   // +6
    rw::Field<uint8_t> flags;    // +7 bit 0: Japan time (UTC+9), bit 1: valid

    static constexpr uint8_t FLAG_JST = 1;
    static constexpr uint8_t FLAG_VALID = 2;
    static constexpr int32_t JST_OFFSET = 9 * 3600;

    // Seconds from now (server time) to this date.
    static int32_t calculateIntervalSinceNow(const Date *self);
    // Parse "YYYY-MM-DD hh:mm:ss"; out-of-range parts are clamped and leave the date invalid. 1 = parsed.
    static uint32_t init(Date *self, uint32_t text, uint32_t jst);
    static void setIntervalFrom1970(Date *self, int32_t seconds);
    // Server time now + seconds.
    static void setIntervalFromNow(Date *self, int32_t seconds);
    static void setNow(Date *self);
    // Unix time of this date.
    static int32_t calculateIntervalSince1970(const Date *self);
    // The same instant as a device-local / UTC calendar date (sret; the flags byte of *out is not written).
    static void makeLocalDate(Date *out, const Date *self);
    static void makeUTCDate(Date *out, const Date *self);
};
RW_SIZE(Date, 8);

// A time of day, 4 bytes.
struct Time {
    rw::Field<uint8_t> hour;     // +0
    rw::Field<uint8_t> minute;   // +1
    rw::Field<uint8_t> second;   // +2
    rw::Field<uint8_t> flags;    // +3 as Date::flags

    // Parse "hh:mm:ss". 1 = parsed.
    static uint32_t init(Time *self, uint32_t text, uint32_t jst);
    // The current device-local day at this time of day, as a local Date (sret; flags byte not written).
    static void makeLocalDate(Date *out, const Time *self);
};
RW_SIZE(Time, 4);

namespace hole_time {
// hole::time::setReferenceDate(const Date& serverNow): the server-minus-device clock offset.
uint32_t setReferenceDate(const Date *serverNow);
}
