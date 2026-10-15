#include "Keyblade.hpp"
#include "ParseUtilC.hpp"

namespace master {

using KeybladeFunctions = TableFunctions<Keyblade>;
RW_FUNCTION(0x783f80, "master::Keyblade::getKeybladeIds", KeybladeFunctions::getIds);
RW_FUNCTION(0x783f9c, "master::Keyblade::create", KeybladeFunctions::create);
RW_FUNCTION(0x784918, "master::Keyblade::getHash", KeybladeFunctions::getHash);
RW_FUNCTION(0x784a04, "master::Keyblade::getID", KeybladeFunctions::getID);
RW_FUNCTION(0x784a50, "master::Keyblade::~Keyblade (deleting)", KeybladeFunctions::destructDelete);

void Keyblade::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    constexpr uint32_t ATTRIBUTE_KEY = 0xdfb52f;            // "attribute"
    constexpr uint32_t DARKLIGHT_KEY = 0xe08df8;            // "darklight"
    constexpr uint32_t ACCORD_KEY = 0xe08e02;               // "accord"
    constexpr uint32_t DLACCORD_KEY = 0xe08e09;             // "dlaccord"
    constexpr uint32_t NEED_MATERIAL_ID_KEY = 0xe08e39;     // "needMaterialId"
    constexpr uint32_t NEED_MATERIAL_NUM_KEY = 0xe08e48;    // "needMaterialNum"
    constexpr uint32_t BUF = 0x8c;   // frame buffer shared by the strings and the arrays (stale bytes kept)
    guest::Frame f(0x128);
    RowBuilder b(f);
    uint32_t row = 0;
    auto intMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isInt(v); };
    auto stringMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isString(v); };
    uint32_t id, name, description, lv, displayId, category, passive, passivePower, drawCount, maxBurstGauge,
        validConsist, partnerRate, evolveId, validNeedMaterial;
    if (intMember(0xdfb32d, id) &&                          // "keybladeId"
        stringMember(0xdfade6, name) &&                     // "name"
        stringMember(0xdfae44, description) &&              // "description"
        intMember(0xe08dbb, lv) &&                          // "lv"
        intMember(0xe08887, displayId) &&                   // "displayId"
        intMember(0xdfb643, category) &&                    // "category"
        intMember(0xe08dbe, passive) &&                     // "passive"
        intMember(0xe08dc6, passivePower) &&                // "passivePower"
        intMember(0xe08dd3, drawCount) &&                   // "drawCount"
        intMember(0xe08ddd, maxBurstGauge) &&               // "maxBurstGauge"
        intMember(0xe08deb, validConsist) &&                // "validConsist"
        isArray(member(json, ATTRIBUTE_KEY)) && isArray(member(json, DARKLIGHT_KEY)) &&
        isArray(member(json, ACCORD_KEY)) && isArray(member(json, DLACCORD_KEY)) &&
        intMember(0xe08e12, partnerRate) &&                 // "partnerRate"
        intMember(0xe08e1e, evolveId) &&                    // "evolveId"
        intMember(0xe08e27, validNeedMaterial) &&           // "validNeedMaterial"
        isArray(member(json, NEED_MATERIAL_ID_KEY)) && isArray(member(json, NEED_MATERIAL_NUM_KEY))) {
        b.create(SIZE, VTABLE);
        b.int32(KEYBLADE_ID, intOf(id), 0x88);
        b.string(NAME, stringOf(name), 65, BUF);
        b.string(DESCRIPTION, stringOf(description), 129, BUF);
        b.int32(LV, intOf(lv), 0x84);
        b.int32(DISPLAY_ID, intOf(displayId), 0x80);
        b.int32(CATEGORY, intOf(category), 0x7c);
        b.int32(PASSIVE, intOf(passive), 0x78);
        b.int32(PASSIVE_POWER, intOf(passivePower), 0x74);
        b.int32(DRAW_COUNT, intOf(drawCount), 0x70);
        b.int32(MAX_BURST_GAUGE, intOf(maxBurstGauge), 0x6c);
        b.int32(VALID_CONSIST, intOf(validConsist), 0x68);
        bool ok = b.int32Array(ATTRIBUTE, member(json, ATTRIBUTE_KEY), 5, BUF) &&
                  b.int32Array(DARKLIGHT, member(json, DARKLIGHT_KEY), 5, BUF) &&
                  b.int32Array(ACCORD, member(json, ACCORD_KEY), 5, BUF) &&
                  b.int32Array(DLACCORD, member(json, DLACCORD_KEY), 5, BUF);
        if (ok) {
            b.int32(PARTNER_RATE, intOf(partnerRate), 0x64);
            b.int32(EVOLVE_ID, intOf(evolveId), 0x60);
            b.int32(VALID_NEED_MATERIAL, intOf(validNeedMaterial), 0x5c);
            ok = b.int32Array(NEED_MATERIAL_ID, member(json, NEED_MATERIAL_ID_KEY), 4, BUF) &&
                 b.int32Array(NEED_MATERIAL_NUM, member(json, NEED_MATERIAL_NUM_KEY), 4, BUF);
        }
        if (ok) {
            row = b.obj();
        } else {
            b.destroy();
        }
    }
    makeShared(out, SHARED_PTR_CTOR, row);
}
RW_FUNCTION(0x784130, "master::Keyblade::parse", Keyblade::parse);

}  // namespace master
