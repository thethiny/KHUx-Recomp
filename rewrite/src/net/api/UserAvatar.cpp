#include "UserAvatar.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;           // __aeabi_memset(void*, size_t, int) (import)
constexpr uint32_t AVATAR_SHARED_PTR_CTOR = 0x49F909; // std::shared_ptr<UserAvatar>::shared_ptr(UserAvatar*)
constexpr uint32_t ALL_SHARED_PTR_CTOR = 0x49FA99;    // std::shared_ptr<UserAvatarAll>::shared_ptr(UserAvatarAll*)
// std::unordered_map<int, int>::__insert_unique(pair<int, int>&&) / ~__hash_table<int, int>
constexpr uint32_t INT_MAP_INSERT = 0x4B18D1;
constexpr uint32_t INT_MAP_DTOR = 0x4942D3;
constexpr uint32_t COORDINATE = sizeof(UserAvatar::Coordinate);   // 48

// the pair<int, int> UserAvatarAll inserts into its myCoordinateNo -> index map
struct NoIndexPair {
    rw::Field<int32_t> no;          // +0
    rw::Field<int32_t> index;       // +4
};

const records::Kind kParts = {
    UserAvatarParts::VTABLE, "userAvatarParts", sizeof(UserAvatarParts::Part),
    0x49FD55,       // std::shared_ptr<UserAvatarParts>::shared_ptr(UserAvatarParts*)
    0x4B1575,       // std::shared_ptr<UserAvatarParts>::make_shared<unsigned>(unsigned&&)
    0xAAAAAAAB,     // 1/3: 24 = 8 * 3
};
}  // namespace

// ---------------------------------------------------------------------------------------------- UserAvatar
uint32_t UserAvatar::Coordinate::init(Coordinate *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    struct IntField { const char *key; rw::Field<int32_t> &field; };
    const IntField fields[] = {
        {"myCoordinateNo", self->myCoordinateNo}, {"gender", self->gender},
        {"hairPartsId", self->hairPartsId},       {"hairColorPartsId", self->hairColorPartsId},
        {"facePartsId", self->facePartsId},       {"bodyPartsId", self->bodyPartsId},
        {"skinPartsId", self->skinPartsId},
    };
    for (const IntField &f : fields) {
        const JsonValue &v = (*json)[f.key];
        if (!v.IsInt()) return 0;
        f.field = v.GetInt();
    }
    rw::call(AEABI_MEMSET, {rw::guest_addr(self->accessoriesPartsIds), 5 * 4, 0});
    const JsonValue &accessories = (*json)["accessoriesPartsIds"];
    if (!accessories.IsArray()) return 0;
    uint32_t n = accessories.Size();
    for (uint32_t k = 0; k < n; k++) {
        const JsonValue &id = accessories.At(k);
        if (!id.IsInt()) return 0;
        // no bounds check (original behaviour)
        rw::set<int32_t>(self, offsetof(Coordinate, accessoriesPartsIds) + 4 * k, id.GetInt());
    }
    return 1;
}
RW_FUNCTION(0x49F6C4, "hole::network::api::UserAvatar::Coordinate::init", UserAvatar::Coordinate::init);

uint32_t UserAvatar::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x58: the Coordinate local at +0x10
    GuestFrame frame(0x58);
    Coordinate *coordinate = frame.at<Coordinate>(0x10);

    // the object is made before the response is looked at
    UserAvatar *self = newObject<UserAvatar>(sizeof(UserAvatar));
    constructSafeBuffer(self, COORDINATE);
    self->vtable = VTABLE;
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &avatar = (*response)["userAvatar"];
        ok = avatar.IsObject() && Coordinate::init(coordinate, &avatar) == 1;
    }
    if (ok) {
        writeBytes(self, 0, coordinate, COORDINATE);
    } else {
        deleteVirtual(self);
        self = nullptr;
    }
    rw::call(AVATAR_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x49F82C, "hole::network::api::UserAvatar::parse", UserAvatar::parse);

uint32_t UserAvatar::destroyDelete(UserAvatar *self) {
    // operator delete gets what ~SafeBuffer returns (this)
    uint32_t p = rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return rw::call(guest::operator_delete, {p});
}
RW_FUNCTION(0x4B6D84, "hole::network::api::UserAvatar::~UserAvatar (deleting)", UserAvatar::destroyDelete);

// ---------------------------------------------------------------------------------------------- UserAvatarAll
uint32_t UserAvatarAll::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x78: the map pair at +0x20, the insert result at +0x28, the Coordinate local at +0x30
    GuestFrame frame(0x78);
    NoIndexPair *pair = frame.at<NoIndexPair>(0x20);
    UserAvatar::Coordinate *coordinate = frame.at<UserAvatar::Coordinate>(0x30);

    UserAvatarAll *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &list = (*response)["userAvatars"];
        if (list.IsArray()) {
            uint32_t n = list.Size();
            self = newObject<UserAvatarAll>(sizeof(UserAvatarAll));
            constructSafeBuffer(self, n * COORDINATE);
            self->vtable = VTABLE;
            self->count = n;
            rw::set<uint32_t>(self, 36, 0);                  // empty unordered_map, max_load_factor 1.0f
            rw::set<uint32_t>(self, 40, 0);
            rw::set<uint32_t>(self, 44, 0);
            rw::set<uint32_t>(self, 48, 0);
            rw::set<uint32_t>(self, 52, records::ONE_F);
            uint32_t k = 0;
            for (; k < n; k++) {
                if (!UserAvatar::Coordinate::init(coordinate, &list.At(k))) break;
                writeBytes(self, k * COORDINATE, coordinate, COORDINATE);
                pair->no = coordinate->myCoordinateNo.get();
                pair->index = int32_t(k);
                rw::call(INT_MAP_INSERT, {frame.addr(0x28), rw::guest_addr(self) + 36, frame.addr(0x20)});
            }
            if (k != n) {
                deleteVirtual(self);
                self = nullptr;
            }
        }
    }
    rw::call(ALL_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x49F95C, "hole::network::api::UserAvatarAll::parse", UserAvatarAll::parse);

UserAvatarAll *UserAvatarAll::destroy(UserAvatarAll *self) {
    self->vtable = VTABLE;
    rw::call(INT_MAP_DTOR, {rw::guest_addr(self) + 36});
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x4B6D18, "hole::network::api::UserAvatarAll::~UserAvatarAll", UserAvatarAll::destroy);

uint32_t UserAvatarAll::destroyDelete(UserAvatarAll *self) {
    destroy(self);
    return rw::call(guest::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x4B6D4C, "hole::network::api::UserAvatarAll::~UserAvatarAll (deleting)", UserAvatarAll::destroyDelete);

// ---------------------------------------------------------------------------------------------- UserAvatarParts
uint32_t UserAvatarParts::Part::init(Part *self, const JsonValue *json) {
    // original frame 0x28: the Date local at +8
    GuestFrame frame(0x28);
    if (!json->IsObject()) return 0;
    const JsonValue &id = (*json)["userAvatarPartsId"];
    if (!id.IsUint64()) return 0;
    self->userAvatarPartsId = id.GetUint64();
    const JsonValue &type = (*json)["partsType"];
    if (!type.IsInt()) return 0;
    self->partsType = type.GetInt();
    const JsonValue &parts = (*json)["avatarPartsId"];
    if (!parts.IsInt()) return 0;
    self->avatarPartsId = parts.GetInt();
    return records::readDatetime(*json, frame, 8, self->getDatetime) ? 1 : 0;
}
RW_FUNCTION(0x49FAEC, "hole::network::api::UserAvatarParts::Part::init", UserAvatarParts::Part::init);

uint32_t UserAvatarParts::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x70: the Part local at +0x20, the map pair at +0x38, the insert result at +0x4c
    return records::parseList(out, response, kParts, {0x70, 0x20, 0x38, 0x4C}, &Part::init);
}
RW_FUNCTION(0x49FC0C, "hole::network::api::UserAvatarParts::parse", UserAvatarParts::parse);

uint32_t UserAvatarParts::create(SharedPtr *out, const StdVector *parts) {
    return records::createList(out, parts, kParts);
}
RW_FUNCTION(0x49FDA8, "hole::network::api::UserAvatarParts::create", UserAvatarParts::create);

UserAvatarParts *UserAvatarParts::destroy(UserAvatarParts *self) {
    records::destroy(self, VTABLE);
    return self;
}
RW_FUNCTION(0x4B16B4, "hole::network::api::UserAvatarParts::~UserAvatarParts", UserAvatarParts::destroy);

uint32_t UserAvatarParts::destroyDelete(UserAvatarParts *self) {
    return records::destroyDelete(self, VTABLE);
}
RW_FUNCTION(0x4B16E8, "hole::network::api::UserAvatarParts::~UserAvatarParts (deleting)",
            UserAvatarParts::destroyDelete);

}  // namespace hole::network::api
