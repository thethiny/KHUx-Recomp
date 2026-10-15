#include "BattleMisc.hpp"
#include "ParseUtilC.hpp"

namespace master {

using BattleMiscFunctions = TableFunctions<BattleMisc>;
RW_FUNCTION(0x779ab4, "master::BattleMisc::getBattleMiscIds", BattleMiscFunctions::getIds);
RW_FUNCTION(0x779ad0, "master::BattleMisc::create", BattleMiscFunctions::create);
RW_FUNCTION(0x779dbc, "master::BattleMisc::getHash", BattleMiscFunctions::getHash);
RW_FUNCTION(0x779ea8, "master::BattleMisc::getID", BattleMiscFunctions::getID);
RW_FUNCTION(0x779ef4, "master::BattleMisc::~BattleMisc (deleting)", BattleMiscFunctions::destructDelete);

void BattleMisc::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    guest::Frame f(0x30);
    RowBuilder b(f);
    uint32_t id = member(json, 0xe07ec5);           // "battleMiscId"
    if (isInt(id)) {
        uint32_t value = member(json, 0xe07ed2);    // "value"
        if (isInt(value)) {
            b.create(SIZE, VTABLE);
            b.int32(BATTLE_MISC_ID, intOf(id), 0x14);
            b.int32(VALUE, intOf(value), 0x10);
        }
    }
    makeShared(out, SHARED_PTR_CTOR, b.obj());
}
RW_FUNCTION(0x779C64, "master::BattleMisc::parse", BattleMisc::parse);

}  // namespace master
