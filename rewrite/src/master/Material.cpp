#include "Material.hpp"
#include "ParseUtilC.hpp"

namespace master {

using MaterialFunctions = TableFunctions<Material>;
RW_FUNCTION(0x7852c8, "master::Material::getMaterialIds", MaterialFunctions::getIds);
RW_FUNCTION(0x7852e4, "master::Material::create", MaterialFunctions::create);
RW_FUNCTION(0x785710, "master::Material::getHash", MaterialFunctions::getHash);
RW_FUNCTION(0x7857fc, "master::Material::getID", MaterialFunctions::getID);
RW_FUNCTION(0x785848, "master::Material::~Material (deleting)", MaterialFunctions::destructDelete);

void Material::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    guest::Frame f(0xd8);
    RowBuilder b(f);
    auto intMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isInt(v); };
    auto stringMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isString(v); };
    uint32_t id, category, sortId, name, description, sell;
    if (intMember(0xdfb64c, id) &&                  // "materialId"
        intMember(0xdfb643, category) &&            // "category"
        intMember(0xdfae85, sortId) &&              // "sortId"
        stringMember(0xdfade6, name) &&             // "name"
        stringMember(0xdfae44, description) &&      // "description"
        intMember(0xe08ef6, sell)) {                // "sell"
        b.create(SIZE, VTABLE);
        b.int32(MATERIAL_ID, intOf(id), 0x38);
        b.int32(CATEGORY, intOf(category), 0x34);
        b.int32(SORT_ID, intOf(sortId), 0x30);
        b.string(NAME, stringOf(name), 65, 0x3c);
        b.string(DESCRIPTION, stringOf(description), 129, 0x3c);
        b.int32(SELL, intOf(sell), 0x2c);
    }
    makeShared(out, SHARED_PTR_CTOR, b.obj());
}
RW_FUNCTION(0x785478, "master::Material::parse", Material::parse);

}  // namespace master
