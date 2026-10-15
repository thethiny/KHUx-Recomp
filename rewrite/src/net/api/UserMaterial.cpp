#include "UserMaterial.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t SHARED_PTR_CTOR = 0x4A0BA9;   // std::shared_ptr<UserMaterial>::shared_ptr(UserMaterial*)
constexpr uint32_t MAKE_SHARED = 0x4B0A3D;       // std::shared_ptr<UserMaterial>::make_shared<unsigned>(unsigned&&)
// std::unordered_map<unsigned long long, int>::__insert_unique(pair<ull, int>&&) / (pair<ull, unsigned>&&)
constexpr uint32_t MAP_INSERT_INT = 0x4B176D;
constexpr uint32_t MAP_INSERT_UINT = 0x4B1071;
constexpr uint32_t MAP_DTOR = 0x4B09C1;          // ~__hash_table<unsigned long long, int>
constexpr uint32_t RECORD = 16;
}  // namespace

uint32_t UserMaterial::Material::init(Material *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &id = (*json)["userMaterialId"];
    if (!id.IsUint64()) return 0;
    self->userMaterialId = id.GetUint64();
    const JsonValue &material = (*json)["materialId"];
    if (!material.IsInt()) return 0;
    self->materialId = material.GetInt();
    const JsonValue &number = (*json)["number"];
    if (!number.IsInt()) return 0;
    self->number = number.GetInt();
    return 1;
}
RW_FUNCTION(0x4A09E4, "hole::network::api::UserMaterial::Material::init", UserMaterial::Material::init);

uint32_t UserMaterial::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x60: the Material local at +0x18, the map pair at +0x28, the insert result at +0x3c
    GuestFrame frame(0x60);
    Material *record = frame.at<Material>(0x18);
    IdIndexPair *pair = frame.at<IdIndexPair>(0x28);

    UserMaterial *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &list = (*response)["userMaterials"];
        if (list.IsArray()) {
            uint32_t n = list.Size();
            self = newObject<UserMaterial>(sizeof(UserMaterial));
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
                if (!Material::init(record, &list.At(k))) break;
                writeBytes(self, k * RECORD, record, RECORD);
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
RW_FUNCTION(0x4A0A70, "hole::network::api::UserMaterial::parse", UserMaterial::parse);

uint32_t UserMaterial::create(SharedPtr *out, const StdVector *materials) {
    // original frame 0x50: the count at +0x14, the map pair at +0x18, the insert result at +0x2c
    GuestFrame frame(0x50);
    rw::store32(frame.at<uint32_t>(0x14), uint32_t(int32_t(materials->end - materials->begin) >> 4));
    rw::call(MAKE_SHARED, {rw::guest_addr(out), frame.addr(0x14)});
    uint32_t begin = materials->begin;
    if (begin != materials->end) {
        uint32_t bytes = materials->end - begin;
        UserMaterial *self = rw::host_ptr<UserMaterial>(out->ptr);
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
RW_FUNCTION(0x4A0BFC, "hole::network::api::UserMaterial::create", UserMaterial::create);

UserMaterial *UserMaterial::destroy(UserMaterial *self) {
    self->vtable = VTABLE;
    rw::call(MAP_DTOR, {rw::guest_addr(self) + 36});
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x4B0B7C, "hole::network::api::UserMaterial::~UserMaterial", UserMaterial::destroy);

uint32_t UserMaterial::destroyDelete(UserMaterial *self) {
    self->vtable = VTABLE;
    rw::call(MAP_DTOR, {rw::guest_addr(self) + 36});
    rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return rw::call(guest::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x4B0BB0, "hole::network::api::UserMaterial::~UserMaterial (deleting)", UserMaterial::destroyDelete);

}  // namespace hole::network::api
