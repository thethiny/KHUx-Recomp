#include "Ret.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t MAKE_SHARED = 0x4B1C85;       // std::shared_ptr<Ret>::make_shared<>()
constexpr uint32_t RELEASE_SHARED = 0xC8DBD1;    // std::__shared_weak_count::__release_shared()
constexpr uint32_t STRING_CTOR = 0x47CD93;       // std::string::string(const char*, size_t)
constexpr uint32_t STRING_REPLACE = 0x4B6DCD;    // std::string::replace(size_t pos, size_t n1, const char*, size_t n2)
constexpr uint32_t SEARCH = 0x49A42B;            // std::__search<bool(*&)(char,char), const char*, const char*>
constexpr uint32_t CHAR_EQ = 0x49A419;           // std::char_traits<char>::eq (the predicate __search gets)
// literals in the library
constexpr uint32_t STR_ESCAPED_NEWLINE = 0xDFA609;   // "\\n" (backslash, n)
constexpr uint32_t STR_NEWLINE = 0xDF9E14;           // "\n"

// Ret::parse frame (5 pushed registers + 0x44): two temporary strings, the made shared_ptr
constexpr uint32_t RET_FRAME = 0x58;
constexpr uint32_t LOCAL_TO = 0x20;          // std::string "\n"
constexpr uint32_t LOCAL_FROM = 0x2C;        // std::string "\\n"
constexpr uint32_t LOCAL_MADE = 0x38;        // shared_ptr<Ret>

// ReplaceString frame (5 pushed registers + 0x3c): the __search predicate slots
constexpr uint32_t REPLACE_FRAME = 0x50;
constexpr uint32_t LOCAL_PRED_NEXT = 0x30;
constexpr uint32_t LOCAL_PRED_FIRST = 0x34;

// The mandatory header fields, then the optional error message. false = the header is unusable.
bool readHeader(Ret *self, const JsonValue *response, const GuestFrame &frame) {
    if (!response->IsObject()) return false;
    const JsonValue &ret = (*response)["ret"];
    if (!ret.IsObject()) return false;

    const JsonValue &maintenance = ret["isMaintenance"];
    if (!maintenance.IsBool()) return false;
    self->isMaintenance = uint8_t(maintenance.IsTrue());
    const JsonValue &timeout = ret["sessionTO"];
    if (!timeout.IsBool()) return false;
    self->sessionTimeout = uint8_t(timeout.IsTrue());
    const JsonValue &newDay = ret["isNewDayPeriod"];
    if (!newDay.IsInt()) return false;
    self->isNewDayPeriod = uint8_t(newDay.GetUint() != 0);

    const JsonValue &app = ret["versionApp"];
    if (!app.IsString()) return false;
    self->versionApp.assign(app.GetString());
    const JsonValue &res = ret["versionRes"];
    if (!res.IsInt()) return false;
    self->versionRes = res.GetInt();
    const JsonValue &dat = ret["versionDat"];
    if (!dat.IsInt()) return false;
    self->versionDat = dat.GetInt();
    const JsonValue &flags = ret["functionFlags"];
    if (!flags.IsInt()) return false;
    self->functionFlags = flags.GetInt();
    const JsonValue &time = ret["serverTime"];
    if (!time.IsString()) return false;
    if (rw::call(guest::Date_init, {rw::guest_addr(self->serverTime), time.GetString(), 0}) != 1) return false;

    const JsonValue &error = ret["error"];
    if (error.IsString()) {
        self->error.assign(error.GetString());
        StdString *from = frame.at<StdString>(LOCAL_FROM);
        StdString *to = frame.at<StdString>(LOCAL_TO);
        rw::call(STRING_CTOR, {frame.addr(LOCAL_FROM), STR_ESCAPED_NEWLINE, 2});
        rw::call(STRING_CTOR, {frame.addr(LOCAL_TO), STR_NEWLINE, 1});
        ReplaceString(&self->error, from, to);
        if (to->isLong()) guestDelete(to->w2);
        if (from->isLong()) guestDelete(from->w2);
        const JsonValue &url = ret["viewUrl"];
        if (url.IsString()) self->viewUrl.assign(url.GetString());
    }
    return true;
}
}  // namespace

uint32_t Ret::parse(SharedPtr *out, int32_t action, const JsonValue *response) {
    GuestFrame frame(RET_FRAME);
    SharedPtr *made = frame.at<SharedPtr>(LOCAL_MADE);
    rw::call(MAKE_SHARED, {frame.addr(LOCAL_MADE)});
    Ret *self = rw::host_ptr<Ret>(made->ptr);
    if (readHeader(self, response, frame)) {
        self->action = action;
        out->ptr = made->ptr.get();          // move the shared_ptr out
        out->ctrl = made->ctrl.get();
        made->reset();
        return 0;
    }
    out->reset();
    if (made->ctrl) rw::call(RELEASE_SHARED, {made->ctrl});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x49D9AC, "hole::network::api::Ret::parse", Ret::parse);

uint32_t ReplaceString(StdString *self, const StdString *from, const StdString *to) {
    GuestFrame frame(REPLACE_FRAME);
    const uint32_t fromLen = from->size();
    const uint32_t toLen = to->size();
    uint32_t size = self->size();
    uint32_t data = self->data();
    if (size < fromLen) return 0;

    uint32_t pos = 0;
    if (fromLen != 0) {
        rw::store32(frame.at<uint32_t>(LOCAL_PRED_FIRST), CHAR_EQ);
        uint32_t hit = rw::call(SEARCH, {data, data + size, from->data(), from->data() + fromLen,
                                         frame.addr(LOCAL_PRED_FIRST)});
        if (hit == data + size) return 0;
        pos = hit - data;
        if (pos == 0xFFFFFFFFu) return 0;
    }
    for (;;) {
        rw::call(STRING_REPLACE, {rw::guest_addr(self), pos, fromLen, to->data(), to->size()});
        pos += toLen;
        size = self->size();
        data = self->data();
        const uint32_t n = from->size();
        if (size < pos || size - pos < n) return 0;
        if (n != 0) {
            rw::store32(frame.at<uint32_t>(LOCAL_PRED_NEXT), CHAR_EQ);
            uint32_t hit = rw::call(SEARCH, {data + pos, data + size, from->data(), from->data() + n,
                                             frame.addr(LOCAL_PRED_NEXT)});
            if (hit == data + size) return 0;
            pos = hit - data;
        }
        if (pos == 0xFFFFFFFFu) return 0;
    }
}
RW_FUNCTION(0x49DD48, "hole::network::api::ReplaceString", ReplaceString);

}  // namespace hole::network::api
