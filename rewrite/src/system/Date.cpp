#include "Date.hpp"

namespace {

constexpr uint32_t TIME_OFFSET = 0x1003440;     // int: server time - device time, seconds (hole::time)
constexpr uint32_t DATE_SEPARATORS = 0xDFA05D;  // "-- ::": the character ending each Date field
constexpr uint32_t TIME_SEPARATORS = 0xDFA05A;  // the character ending each Time field
constexpr uint32_t NOW_MICROS = rwlib::sub_C60644;       // sub_C60644(uint64_t *out): gettimeofday in microseconds
constexpr uint32_t MICROS_TO_SECONDS = rwlib::sub_C6067C;  // sub_C6067C(const uint64_t *): seconds
constexpr uint32_t STRCHR = 0x410198;           // strchr (import)
constexpr uint32_t STRTOL = 0x4101A4;           // strtol (import)
constexpr uint32_t LOCALTIME = 0x4101B0;        // localtime (import)
constexpr uint32_t TIMEGM = 0x4101BC;           // timegm (import)
constexpr uint32_t GMTIME = 0x4101C8;           // gmtime (import)

// Device clock now, in seconds.
int32_t deviceNow() {
    guest::Frame f(16);
    rw::call(NOW_MICROS, {f.addr()});
    return int32_t(rw::call(MICROS_TO_SECONDS, {f.addr()}));
}

// Copy a struct tm (guest) into a Date's calendar fields (not the flags).
void fromTm(Date *out, uint32_t tm) {
    out->year = uint16_t(rw::load<uint32_t>(tm + 20) + 1900);
    out->month = uint8_t(rw::load<uint32_t>(tm + 16) + 1);
    out->day = uint8_t(rw::load<uint32_t>(tm + 12));
    out->hour = uint8_t(rw::load<uint32_t>(tm + 8));
    out->minute = uint8_t(rw::load<uint32_t>(tm + 4));
    out->second = uint8_t(rw::load<uint32_t>(tm + 0));
}

// timegm over a struct tm built in the frame: sec, min, hour, mday, mon, year, wday 0, yday 0, isdst -1, 0, 0.
int32_t timegm(uint32_t tm, uint32_t sec, uint32_t min, uint32_t hour, uint32_t mday, uint32_t mon, uint32_t year) {
    const uint32_t v[11] = {sec, min, hour, mday, mon, year, 0, 0, 0xFFFFFFFFu, 0, 0};
    for (uint32_t k = 0; k < 11; k++) rw::save<uint32_t>(tm + 4 * k, v[k]);
    return int32_t(rw::call(TIMEGM, {tm}));
}

// A field value clamped to [lo, hi]; `clamped` becomes 1 when it was out of range.
int32_t clampField(int32_t v, int32_t lo, int32_t hi, uint32_t &clamped) {
    if (v < lo) { clamped = 1; return lo; }
    if (v > hi) { clamped = 1; return hi; }
    return v;
}

}  // namespace

// ------------------------------------------------------------------------------------------------ Date
int32_t Date::calculateIntervalSinceNow(const Date *self) {
    int32_t target = calculateIntervalSince1970(self);
    int32_t now = deviceNow();
    return int32_t(uint32_t(target) - uint32_t(now) + rw::load<uint32_t>(TIME_OFFSET));
}
RW_FUNCTION(0x4922D4, "Date::calculateIntervalSinceNow", Date::calculateIntervalSinceNow);

uint32_t Date::init(Date *self, uint32_t text, uint32_t jst) {
    self->flags = uint8_t(self->flags & ~FLAG_VALID);
    uint32_t clamped = 0;
    for (uint32_t field = 0;; field++) {
        int8_t sep = rw::load<int8_t>(DATE_SEPARATORS + field);
        uint32_t end = rw::call(STRCHR, {text, uint32_t(int32_t(sep))});
        if (!end) return 0;
        int32_t v = int32_t(rw::call(STRTOL, {text, 0, 10}));
        switch (field) {
        case 0: self->year = uint16_t(clampField(v, 1000, 9999, clamped)); break;
        case 1: self->month = uint8_t(clampField(v, 1, 12, clamped)); break;
        case 2: self->day = uint8_t(clampField(v, 1, 31, clamped)); break;
        case 3: self->hour = uint8_t(clampField(v, 0, 23, clamped)); break;
        case 4: self->minute = uint8_t(clampField(v, 0, 59, clamped)); break;
        case 5:
            self->second = uint8_t(clampField(v, 0, 59, clamped));
            self->flags = uint8_t(((clamped << 1) | jst) ^ FLAG_VALID);
            return 1;
        default:
            self->flags = uint8_t(self->flags & ~FLAG_VALID);
            return 0;
        }
        text = end + 1;
    }
}
RW_FUNCTION(0x4924F0, "Date::init", Date::init);

void Date::setIntervalFrom1970(Date *self, int32_t seconds) {
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), seconds);
    uint32_t tm = rw::call(GMTIME, {f.addr()});
    fromTm(self, tm);
    self->flags = FLAG_VALID;
}
RW_FUNCTION(0x492664, "Date::setIntervalFrom1970", Date::setIntervalFrom1970);

void Date::setIntervalFromNow(Date *self, int32_t seconds) {
    int32_t now = deviceNow();
    setIntervalFrom1970(self, int32_t(uint32_t(now) + uint32_t(seconds) - rw::load<uint32_t>(TIME_OFFSET)));
}
RW_FUNCTION(0x4926D0, "Date::setIntervalFromNow", Date::setIntervalFromNow);

void Date::setNow(Date *self) { setIntervalFromNow(self, 0); }
RW_FUNCTION(0x492738, "Date::setNow", Date::setNow);

int32_t Date::calculateIntervalSince1970(const Date *self) {
    uint8_t flags = self->flags;
    guest::Frame f(48);
    int32_t t = timegm(f.addr(), self->second, self->minute, self->hour, self->day, uint32_t(self->month) - 1,
                       uint32_t(self->year) - 1900);
    return (flags & FLAG_JST) ? int32_t(uint32_t(t) - JST_OFFSET) : t;
}
RW_FUNCTION(0x492744, "Date::calculateIntervalSince1970", Date::calculateIntervalSince1970);

void Date::makeLocalDate(Date *out, const Date *self) {
    int32_t t = calculateIntervalSince1970(self);
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), t);
    fromTm(out, rw::call(LOCALTIME, {f.addr()}));
}
RW_FUNCTION(0x4927CC, "Date::makeLocalDate", Date::makeLocalDate);

void Date::makeUTCDate(Date *out, const Date *self) {
    int32_t t = calculateIntervalSince1970(self);
    guest::Frame f(8);
    rw::save<int32_t>(f.addr(), t);
    fromTm(out, rw::call(GMTIME, {f.addr()}));
}
RW_FUNCTION(0x49283C, "Date::makeUTCDate", Date::makeUTCDate);

// ------------------------------------------------------------------------------------------------ Time
uint32_t Time::init(Time *self, uint32_t text, uint32_t jst) {
    self->flags = uint8_t(self->flags & ~Date::FLAG_VALID);
    uint32_t clamped = 0;
    for (uint32_t field = 0;; field++) {
        int8_t sep = rw::load<int8_t>(TIME_SEPARATORS + field);
        uint32_t end = rw::call(STRCHR, {text, uint32_t(int32_t(sep))});
        if (!end) return 0;
        int32_t v = int32_t(rw::call(STRTOL, {text, 0, 10}));
        switch (field) {
        case 0: self->hour = uint8_t(clampField(v, 0, 23, clamped)); break;
        case 1: self->minute = uint8_t(clampField(v, 0, 59, clamped)); break;
        case 2:
            self->second = uint8_t(clampField(v, 0, 59, clamped));
            self->flags = uint8_t(((clamped << 1) | jst) ^ Date::FLAG_VALID);
            return 1;
        default: return 0;
        }
        text = end + 1;
    }
}
RW_FUNCTION(0x492340, "Time::init", Time::init);

void Time::makeLocalDate(Date *out, const Time *self) {
    guest::Frame f(0x50);
    rw::save<int32_t>(f.addr(0xC), deviceNow());
    uint32_t today = rw::call(LOCALTIME, {f.addr(0xC)});
    uint8_t flags = self->flags;
    uint32_t mday = rw::load<uint32_t>(today + 12), mon = rw::load<uint32_t>(today + 16),
             year = rw::load<uint32_t>(today + 20);
    int32_t t = timegm(f.addr(0x18), self->second, self->minute, self->hour, mday, mon, year);
    if (flags & Date::FLAG_JST) t = int32_t(uint32_t(t) - Date::JST_OFFSET);
    rw::save<int32_t>(f.addr(8), t);
    fromTm(out, rw::call(LOCALTIME, {f.addr(8)}));
}
RW_FUNCTION(0x492424, "Time::makeLocalDate", Time::makeLocalDate);

// ------------------------------------------------------------------------------------------------ hole::time
uint32_t hole_time::setReferenceDate(const Date *serverNow) {
    if (!(serverNow->flags & Date::FLAG_VALID)) return rw::guest_addr(serverNow);
    rw::save<uint32_t>(TIME_OFFSET, 0);
    uint32_t offset = 0u - uint32_t(Date::calculateIntervalSinceNow(serverNow));
    rw::save<uint32_t>(TIME_OFFSET, offset);
    return offset;
}
RW_FUNCTION(0x4922AC, "hole::time::setReferenceDate", hole_time::setReferenceDate);
