#include "UserPoint.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t FRAME = 0x48;          // original frame: 5 pushed registers + 0x34 of locals
constexpr uint32_t LOCAL_DATE = 0x20;     // the Date local (8 bytes)
constexpr uint32_t SHARED_PTR_CTOR = 0x49EDC9;   // std::shared_ptr<UserPoint>::shared_ptr(UserPoint*)

// The int fields, in parse order after the two Lux values.
struct IntField { const char *key; uint32_t index; };
constexpr IntField kStats[] = {
    {"spherePoint", UserPoint::SPHERE_POINT}, {"kizunaPoint", UserPoint::KIZUNA_POINT},
    {"raidPoint", UserPoint::RAID_POINT},     {"attack", UserPoint::ATTACK},
    {"defense", UserPoint::DEFENSE},          {"baseHp", UserPoint::BASE_HP},
    {"hp", UserPoint::HP},                    {"ap", UserPoint::AP},
    {"maxHp", UserPoint::MAX_HP},             {"maxAp", UserPoint::MAX_AP},
};
constexpr IntField kTail[] = {
    {"stageSpherePoint", UserPoint::STAGE_SPHERE_POINT},
    {"raidSpherePoint", UserPoint::RAID_SPHERE_POINT},
    {"colosseumSpherePoint", UserPoint::COLOSSEUM_SPHERE_POINT},
    {"stageSkipTicket", UserPoint::STAGE_SKIP_TICKET},
};

bool readInt(UserPoint *self, const JsonValue &obj, const IntField &f) {
    const JsonValue &v = obj[f.key];
    if (!v.IsInt()) return false;
    writeU32(self, f.index, v.GetUint());
    return true;
}

bool readUint64(UserPoint *self, const JsonValue &obj, const char *key, uint32_t index) {
    const JsonValue &v = obj[key];
    if (!v.IsUint64()) return false;
    writeU64(self, index, v.GetUint64());
    return true;
}

// Every field must be present with the right type; the first bad one stops the parse.
bool readAll(UserPoint *self, const JsonValue &obj, const GuestFrame &frame) {
    if (!readInt(self, obj, {"money", UserPoint::MONEY})) return false;
    if (!readUint64(self, obj, "lux", UserPoint::LUX)) return false;
    if (!readUint64(self, obj, "totalLux", UserPoint::TOTAL_LUX)) return false;
    for (const IntField &f : kStats)
        if (!readInt(self, obj, f)) return false;

    const JsonValue &ap = obj["lastApDatetime"];
    if (!ap.IsString()) return false;
    rw::store8(frame.at<uint8_t>(LOCAL_DATE + 7), 0);          // Date flags byte cleared before init
    if (rw::call(guest::Date_init, {frame.addr(LOCAL_DATE), ap.GetString(), 0}) == 0) return false;
    writeBytes(self, UserPoint::LAST_AP_DATETIME, frame.at<uint8_t>(LOCAL_DATE), 8);

    for (const IntField &f : kTail)
        if (!readInt(self, obj, f)) return false;
    return true;
}
}  // namespace

uint32_t UserPoint::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(FRAME);
    if (response->IsObject()) {
        const JsonValue &userData = (*response)["userData"];
        if (userData.IsObject()) {
            const JsonValue &point = userData["userPoint"];
            if (point.IsObject()) {
                UserPoint *self = newObject<UserPoint>(32);
                constructSafeBuffer(self, SIZE);
                self->vtable = VTABLE;
                if (!readAll(self, point, frame)) {
                    deleteVirtual(self);
                    self = nullptr;
                }
                rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
                return 0;
            }
        }
    }
    out->reset();
    return 0;   // r0 = the stack-protector check (guard - saved guard)
}
RW_FUNCTION(0x49E8A8, "hole::network::api::UserPoint::parse", UserPoint::parse);

}  // namespace hole::network::api
