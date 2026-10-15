#include "EvResource.hpp"
#include "ParseUtilC.hpp"

namespace master {

using EvResourceFunctions = TableFunctions<EvResource>;
RW_FUNCTION(0x792d78, "master::EvResource::getResourceIds", EvResourceFunctions::getIds);
RW_FUNCTION(0x792d94, "master::EvResource::create", EvResourceFunctions::create);
RW_FUNCTION(0x793144, "master::EvResource::getHash", EvResourceFunctions::getHash);
RW_FUNCTION(0x793230, "master::EvResource::getID", EvResourceFunctions::getID);
RW_FUNCTION(0x79327c, "master::EvResource::~EvResource (deleting)", EvResourceFunctions::destructDelete);

void EvResource::parse(guest::SharedPtr *out, uint32_t json) {
    using namespace rowparse;
    constexpr uint32_t END_TIME_KEY = 0xdfb479;     // "endTime"
    guest::Frame f(0x48);
    RowBuilder b(f);
    uint32_t row = 0;
    auto intMember = [&](uint32_t key, uint32_t &v) { v = member(json, key); return isInt(v); };
    uint32_t id, priority, validResource;
    if (intMember(0xdfb267, id) &&                  // "resourceId"
        intMember(0xdfa942, priority) &&            // "priority"
        intMember(0xe09b33, validResource) &&       // "validResource"
        isString(member(json, END_TIME_KEY))) {
        b.create(SIZE, VTABLE);
        b.int32(RESOURCE_ID, intOf(id), 0x24);
        b.int32(PRIORITY, intOf(priority), 0x20);
        b.int32(VALID_RESOURCE, intOf(validResource), 0x1c);
        uint32_t endTime = member(json, END_TIME_KEY);
        if (isString(endTime) && b.parseDate(stringOf(endTime), 0x28) == 1) {
            b.bytes(END_TIME, 0x28, 8);
            row = b.obj();
        } else {
            b.destroy();
        }
    }
    makeShared(out, SHARED_PTR_CTOR, row);
}
RW_FUNCTION(0x792F28, "master::EvResource::parse", EvResource::parse);

}  // namespace master
