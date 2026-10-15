#include "TextData.hpp"

namespace textdata {

namespace {
constexpr uint32_t STRINGBUFFER_CTOR = 0x49140D;         // StringBuffer::StringBuffer(fmt, ...)
constexpr uint32_t VFS_READ_DATA = 0x8DD46D;             // bg::vfs::readData(shared_ptr<Data>* out, const char*)
constexpr uint32_t TEXTFORMAT_EXECUTE_VA = 0x492CAD;     // TextFormat::execute(string* out, fmt, va_list)
constexpr uint32_t TEXTDATA_FORMAT = 0x492929;           // TextData::format(string* out, Type, id, ...)

constexpr int32_t DAY = 86400, HOUR = 3600, MINUTE = 60;

// text/<type>/<id>.txt into a StringBuffer at buf
void buildPath(uint32_t buf, int32_t type, int32_t id) {
    uint32_t name = rw::load<uint32_t>(TYPE_NAMES + 4u * uint32_t(type));
    rw::call(STRINGBUFFER_CTOR, {buf, PATH_FORMAT, name, uint32_t(id)});
}

// "N units" text: days from a day up, hours from an hour, minutes below (negative -> 0)
void timeString(guest::String *out, int32_t seconds, int32_t dayId, int32_t hourId, int32_t minuteId) {
    int32_t id, value;
    if (seconds >= DAY) {
        value = seconds / DAY;
        id = dayId;
    } else if (seconds >= HOUR) {
        value = seconds / HOUR;
        id = hourId;
    } else {
        int32_t minutes = seconds / MINUTE;
        value = seconds < 0 ? 0 : minutes;
        id = minuteId;
    }
    rw::call(TEXTDATA_FORMAT, {rw::guest_addr(out), 0, uint32_t(id), uint32_t(value)});
}
}  // namespace

void get(guest::SharedPtr *out, int32_t type, int32_t id) {
    guest::Frame f(0x410);
    const uint32_t buf = f.addr(4);
    buildPath(buf, type, id);
    rw::call(VFS_READ_DATA, {rw::guest_addr(out), buf});
}
RW_FUNCTION(0x4928AC, "TextData::get", get);

void format(guest::String *out, int32_t type, int32_t id) {
    sys::VaSpill va(3);
    guest::Frame f(0x430);
    const uint32_t data = f.addr(0x10), buf = f.addr(0x1C);   // shared_ptr<bg::Data>, StringBuffer
    buildPath(buf, type, id);
    rw::call(VFS_READ_DATA, {data, buf});
    uint32_t text = rw::load<uint32_t>(rw::load<uint32_t>(data) + 4);
    rw::call(TEXTFORMAT_EXECUTE_VA, {rw::guest_addr(out), text, va.va()});
    uint32_t ctrl = rw::load<uint32_t>(data + 4);
    if (ctrl) guest::release(ctrl);
}
RW_FUNCTION(0x492928, "TextData::format", format);

void getUnionName(guest::SharedPtr *out, int32_t unionId) {
    int32_t id;
    switch (unionId) {
    case 1: id = 103710001; break;
    case 2: id = 103710002; break;
    case 4: id = 103710004; break;
    case 5: id = 103710005; break;
    default: id = 103710003; break;
    }
    get(out, 0, id);
}
RW_FUNCTION(0x492B14, "hole::text::getUnionName", getUnionName);

void createTimeLeftString(guest::String *out, int32_t seconds) {
    timeString(out, seconds, 100211002, 100211003, 100211004);
}
RW_FUNCTION(0x492B74, "hole::text::createTimeLeftString", createTimeLeftString);

void createTimeAgoString(guest::String *out, int32_t seconds) {
    timeString(out, seconds, 102100010, 102100011, 102100012);
}
RW_FUNCTION(0x492BE8, "hole::text::createTimeAgoString", createTimeAgoString);

void textFormatExecute(guest::String *out, uint32_t fmt) {
    sys::VaSpill va(2);
    rw::call(TEXTFORMAT_EXECUTE_VA, {rw::guest_addr(out), fmt, va.va()});
}
RW_FUNCTION(0x492C5C, "TextFormat::execute(const char*, ...)", textFormatExecute);

}  // namespace textdata
