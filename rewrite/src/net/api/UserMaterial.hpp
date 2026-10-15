// UserMaterial: the player's materials (synthesis items). The records live in a SafeBuffer (16 bytes each), and
// an unordered_map<userMaterialId, record index> finds them by id.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct UserMaterial : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F070;     // UserMaterial vtable (+8)

    // One record (also the SafeBuffer layout: record k at data position 16 * k).
    struct Material {
        rw::Field<uint64_t> userMaterialId;   // +0
        rw::Field<int32_t> materialId;        // +8 master material id
        rw::Field<int32_t> number;            // +12 count owned

        // {"userMaterialId", "materialId", "number"}: 1 when all three are there, else 0 (fields set so far stay)
        static uint32_t init(Material *self, const JsonValue *json);
    };

    rw::Field<uint32_t> count;                // +32 records
    const uint8_t indexById[20];                    // +36 std::unordered_map<unsigned long long, int>

    // response["userMaterials"] (an array) -> *out; empty when absent, null when a record is incomplete
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    // a UserMaterial holding the given records (make_shared)
    static uint32_t create(SharedPtr *out, const StdVector *materials);
    static UserMaterial *destroy(UserMaterial *self);          // ~UserMaterial (D2)
    static uint32_t destroyDelete(UserMaterial *self);         // ~UserMaterial + operator delete (D0)
};
RW_LAYOUT(UserMaterial::Material, materialId, 8);
RW_SIZE(UserMaterial::Material, 16);
RW_LAYOUT(UserMaterial, count, 32);
RW_LAYOUT(UserMaterial, indexById, 36);
RW_SIZE(UserMaterial, 56);

}  // namespace hole::network::api
