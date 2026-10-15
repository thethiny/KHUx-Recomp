#include "UserTitle.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t SHARED_PTR_CTOR = 0x4A17C1;   // std::shared_ptr<UserTitle>::shared_ptr(UserTitle*)
constexpr uint32_t MAKE_SHARED = 0x4B02A1;       // std::shared_ptr<UserTitle>::make_shared<unsigned>(unsigned&&)
// std::unordered_map<int, int>::__insert_unique(pair<ull, int>&&) / (pair<ull, unsigned>&&)
constexpr uint32_t MAP_INSERT_INT = 0x4B0499;
constexpr uint32_t MAP_INSERT_UINT = 0x4B0239;
constexpr uint32_t MAP_DTOR = 0x4942D3;          // ~__hash_table<int, int>
constexpr uint32_t RECORD = 16;
}  // namespace

uint32_t UserTitle::Title::init(Title *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &id = (*json)["userTitleId"];
    if (!id.IsUint64()) return 0;
    self->userTitleId = id.GetUint64();
    const JsonValue &title = (*json)["titleId"];
    if (!title.IsInt()) return 0;
    self->titleId = title.GetInt();
    return 1;
}
RW_FUNCTION(0x4A1620, "hole::network::api::UserTitle::Title::init", UserTitle::Title::init);

uint32_t UserTitle::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x60: the Title local at +0x18, the map pair at +0x28, the insert result at +0x3c
    GuestFrame frame(0x60);
    Title *record = frame.at<Title>(0x18);
    IdIndexPair *pair = frame.at<IdIndexPair>(0x28);

    UserTitle *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &list = (*response)["userTitles"];
        if (list.IsArray()) {
            uint32_t n = list.Size();
            self = newObject<UserTitle>(sizeof(UserTitle));
            constructSafeBuffer(self, n * RECORD);
            self->vtable = VTABLE;
            self->count = n;
            rw::set<uint32_t>(self, 36, 0);                  // empty unordered_map, max_load_factor 1.0f
            rw::set<uint32_t>(self, 40, 0);
            rw::set<uint32_t>(self, 44, 0);
            rw::set<uint32_t>(self, 48, 0);
            rw::set<uint32_t>(self, 52, 0x3F800000);
            uint32_t k = 0;
            for (; k < n; k++) {
                if (!Title::init(record, &list.At(k))) break;
                writeBytes(self, k * RECORD, record, RECORD);   // all 16 bytes, padding included
                pair->keyLo = rw::get<uint32_t>(record, 0);
                pair->keyHi = rw::get<uint32_t>(record, 4);
                pair->index = int32_t(k);
                rw::call(MAP_INSERT_INT, {frame.addr(0x3C), rw::guest_addr(self) + 36, frame.addr(0x28)});
            }
            if (k != n) {
                deleteVirtual(self);
                self = nullptr;
            }
        }
    }
    rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A1688, "hole::network::api::UserTitle::parse", UserTitle::parse);

uint32_t UserTitle::create(SharedPtr *out, const StdVector *titles) {
    // original frame 0x50: the count at +0x14, the map pair at +0x18, the insert result at +0x2c
    GuestFrame frame(0x50);
    rw::store32(frame.at<uint32_t>(0x14), uint32_t(int32_t(titles->end - titles->begin) >> 4));
    rw::call(MAKE_SHARED, {rw::guest_addr(out), frame.addr(0x14)});
    uint32_t begin = titles->begin;
    if (begin != titles->end) {
        uint32_t bytes = titles->end - begin;
        UserTitle *self = rw::host_ptr<UserTitle>(out->ptr);
        IdIndexPair *pair = frame.at<IdIndexPair>(0x18);
        uint32_t offset = 0;
        for (uint32_t k = 0;; k++) {
            writeBytes(self, offset, rw::host_ptr<uint8_t>(begin + offset), RECORD);
            pair->keyLo = rw::load<uint32_t>(begin + offset);
            pair->keyHi = rw::load<uint32_t>(begin + offset + 4);
            pair->index = int32_t(k);
            rw::call(MAP_INSERT_UINT, {frame.addr(0x2C), rw::guest_addr(self) + 36, frame.addr(0x18)});
            offset += RECORD;
            if (offset == bytes) break;
        }
    }
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A1814, "hole::network::api::UserTitle::create", UserTitle::create);

UserTitle *UserTitle::destroy(UserTitle *self) {
    self->vtable = VTABLE;
    rw::call(MAP_DTOR, {rw::guest_addr(self) + 36});
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x4B03E0, "hole::network::api::UserTitle::~UserTitle", UserTitle::destroy);

uint32_t UserTitle::destroyDelete(UserTitle *self) {
    self->vtable = VTABLE;
    rw::call(MAP_DTOR, {rw::guest_addr(self) + 36});
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return rw::call(guest::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x4B0414, "hole::network::api::UserTitle::~UserTitle (deleting)", UserTitle::destroyDelete);

}  // namespace hole::network::api
