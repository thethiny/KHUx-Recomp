#include "System.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t BIRTHDAY_SHARED_PTR_CTOR = 0x4A6605;     // std::shared_ptr<UserBirthday>::shared_ptr(...)
constexpr uint32_t NEED_URL_SHARED_PTR_CTOR = 0x4A69A5;     // std::shared_ptr<SystemNeedUrl>::shared_ptr(...)
constexpr uint32_t COPPA_SHARED_PTR_CTOR = 0x4A6C79;        // std::shared_ptr<SystemCoppa>::shared_ptr(...)
constexpr uint32_t COPPA_MAP_INDEX = 0x4B4EF9;              // unordered_map<unsigned, unsigned>::operator[](unsigned&&)
constexpr uint32_t COPPA_MAP_DTOR = 0x4B4EC9;               // ~__hash_table<unsigned, unsigned>
constexpr uint32_t INFO_SHARED_PTR_CTOR = 0x4A6D5D;         // std::shared_ptr<SystemInformationList>::shared_ptr(...)
constexpr uint32_t INFO2_SHARED_PTR_CTOR = 0x4A6EF5;        // std::shared_ptr<SystemInformationList151203>::...
constexpr uint32_t STRING_VECTOR_PUSH_SLOW = 0x484465;      // vector<string>::__push_back_slow_path(string&&)
constexpr uint32_t STRING_VECTOR_BASE_DTOR = 0x47363F;      // __vector_base<string>::~__vector_base
constexpr uint32_t INVITE_SHARED_PTR_CTOR = 0x4A701D;       // std::shared_ptr<SystemInvitecode>::shared_ptr(...)

void freeString(const StdString *s) {
    if (s->isLong()) guestDelete(s->w2);
}
}  // namespace

// ---------------------------------------------------------------------------------------------- UserBirthday
uint32_t UserBirthday::parse(SharedPtr *out, const JsonValue *response) {
    // the object is made before the response is looked at
    UserBirthday *self = newObject<UserBirthday>(32);
    constructSafeBuffer(self, 8);
    self->vtable = VTABLE;
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &birthday = (*response)["birthday"];
        if (birthday.IsInt()) {
            int32_t mmdd = birthday.GetInt();
            writeU32(self, MONTH, uint32_t(mmdd / 100));             // __divsi3
            writeU32(self, DAY, uint32_t(mmdd % 100));               // __modsi3
            ok = true;
        }
    }
    if (!ok) {
        deleteVirtual(self);
        self = nullptr;
    }
    rw::call(BIRTHDAY_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A64FC, "hole::network::api::UserBirthday::parse", UserBirthday::parse);

uint32_t UserBirthday::destroyDelete(UserBirthday *self) { return deleteSafeBufferResponse(self); }
RW_FUNCTION(0x4B5374, "hole::network::api::UserBirthday::~UserBirthday (deleting)", UserBirthday::destroyDelete);

// ---------------------------------------------------------------------------------------------- SystemNeedUrl
uint32_t SystemNeedUrl::parse(SharedPtr *out, const JsonValue *response) {
    static const char *const kKeys[COUNT] = {"support", "register", "update", "help", "staff",
                                             "agreement", "license", "shikin", "tokutei", "store"};
    SystemNeedUrl *self = newObject<SystemNeedUrl>(sizeof(SystemNeedUrl));
    rw::call(guest::memset_, {rw::guest_addr(self), sizeof(SystemNeedUrl), 0});
    bool ok = response->IsObject();
    for (int k = 0; ok && k < COUNT; k++) {
        const JsonValue &url = (*response)[kKeys[k]];
        ok = url.IsString();
        if (ok) self->urls[k].assign(url.GetString());
    }
    if (!ok) {
        destroy(self);
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(NEED_URL_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4A6658, "hole::network::api::SystemNeedUrl::parse", SystemNeedUrl::parse);

SystemNeedUrl *SystemNeedUrl::destroy(SystemNeedUrl *self) {
    for (int k = COUNT - 1; k >= 0; k--) freeString(&self->urls[k]);
    return self;
}
RW_FUNCTION(0x4A6910, "hole::network::api::SystemNeedUrl::~SystemNeedUrl", SystemNeedUrl::destroy);

// ---------------------------------------------------------------------------------------------- SystemCoppa
uint32_t SystemCoppa::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x30);                   // original frame; the key local at +0x14
    struct Entry { const char *key; uint32_t id; };
    static const Entry kEntries[] = {{"116", 116}, {"900", 900}, {"901", 901}, {"902", 902}, {"903", 903},
                                     {"904", 904}, {"905", 905}, {"906", 906}, {"907", 907}};
    SystemCoppa *self = newObject<SystemCoppa>(sizeof(SystemCoppa));
    for (int k = 0; k < 4; k++) rw::set<uint32_t>(self, 4 * k, 0);
    rw::set<uint32_t>(self, 16, 0x3F800000);                 // max_load_factor 1.0f
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &misc = (*response)["misc"];
        // the original tests the response again here, not "misc"
        if (response->IsObject()) {
            ok = true;
            for (const Entry &e : kEntries) {
                const JsonValue &v = misc[e.key];
                if (!v.IsUint()) { ok = false; break; }
                uint32_t value = v.GetUint();
                rw::store32(frame.at<uint32_t>(0x14), e.id);
                uint32_t slot = rw::call(COPPA_MAP_INDEX, {rw::guest_addr(self), frame.addr(0x14)});
                rw::save<uint32_t>(slot, value);
            }
        }
    }
    if (!ok) {
        rw::call(COPPA_MAP_DTOR, {rw::guest_addr(self)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(COPPA_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A69FC, "hole::network::api::SystemCoppa::parse", SystemCoppa::parse);

// ---------------------------------------------------------------------------------------------- SystemInformationList
uint32_t SystemInformationList::parse(SharedPtr *out, const JsonValue *response) {
    SystemInformationList *self = newObject<SystemInformationList>(sizeof(SystemInformationList));
    self->viewUrl.clear();
    if (response->IsObject()) {
        const JsonValue &url = (*response)["viewUrl"];
        if (url.IsString()) {
            self->viewUrl.assign(url.GetString());
            return rw::call(INFO_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
        }
        freeString(&self->viewUrl);
    }
    guestDelete(rw::guest_addr(self));
    return rw::call(INFO_SHARED_PTR_CTOR, {rw::guest_addr(out), 0, 0});
}
RW_FUNCTION(0x4A6CD0, "hole::network::api::SystemInformationList::parse", SystemInformationList::parse);

uint32_t SystemInformationList151203::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x38);                   // original frame; the string temporary at +0x14
    StdString *tmp = frame.at<StdString>(0x14);
    SystemInformationList151203 *self = newObject<SystemInformationList151203>(sizeof(SystemInformationList151203));
    StdVector *list = &self->popUpViewUrls;
    list->begin = 0;
    list->end = 0;
    list->capEnd = 0;
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &urls = (*response)["popUpViewUrl"];
        if (!urls.IsArray()) return false;
        uint32_t n = urls.Size();
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &url = urls.At(k);
            if (!url.IsString()) return false;
            uint32_t text = url.GetString();
            rw::call(guest::string_init, {frame.addr(0x14), text, uint32_t(std::strlen(rw::host_ptr<char>(text)))});
            uint32_t end = list->end;
            if (end >= list->capEnd) {
                rw::call(STRING_VECTOR_PUSH_SLOW, {rw::guest_addr(list), frame.addr(0x14)});
            } else {
                uint32_t base = 0;
                if (end) {
                    // move: steal the three words, leave the temporary empty
                    StdString *dst = rw::host_ptr<StdString>(end);
                    dst->w0 = tmp->w0.get();
                    dst->w1 = tmp->w1.get();
                    dst->w2 = tmp->w2.get();
                    tmp->clear();
                    base = list->end;
                }
                list->end = base + 12;
            }
            freeString(tmp);
        }
        return true;
    };
    if (!parseAll()) {
        rw::call(STRING_VECTOR_BASE_DTOR, {rw::guest_addr(list)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(INFO2_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A6DBC, "hole::network::api::SystemInformationList151203::parse", SystemInformationList151203::parse);

// ---------------------------------------------------------------------------------------------- SystemInvitecode
uint32_t SystemInvitecode::parse(SharedPtr *out, const JsonValue *response) {
    SystemInvitecode *self = newObject<SystemInvitecode>(sizeof(SystemInvitecode));
    self->inviteCode.clear();
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &code = (*response)["inviteCode"];
        if (code.IsString()) {
            self->inviteCode.assign(code.GetString());
            const JsonValue &invited = (*response)["invitedNumber"];
            if (invited.IsInt()) {
                self->invitedNumber = invited.GetInt();
                const JsonValue &remaining = (*response)["inviteRemainingNumber"];
                if (remaining.IsInt()) {
                    self->inviteRemainingNumber = remaining.GetInt();
                    ok = true;
                }
            }
        }
    }
    if (!ok) {
        freeString(&self->inviteCode);
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(INVITE_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4A6F4C, "hole::network::api::SystemInvitecode::parse", SystemInvitecode::parse);

}  // namespace hole::network::api
