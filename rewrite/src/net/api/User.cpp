#include "User.hpp"

#include "UserPoint.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t USER_SHARED_PTR_CTOR = 0x49E855;          // std::shared_ptr<User>::shared_ptr(User*)
constexpr uint32_t USER_DETAIL_SHARED_PTR_CTOR = 0x49F45D;   // std::shared_ptr<UserDetail>::shared_ptr(UserDetail*)
constexpr uint32_t RESUMPTION_SHARED_PTR_CTOR = 0x49F671;    // std::shared_ptr<StageResumption>::shared_ptr(...)

bool readInt(SafeBuffer *self, const JsonValue &obj, const char *key, uint32_t index) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    writeU32(self, index, v.GetUint());
    return true;
}
bool readUint64(SafeBuffer *self, const JsonValue &obj, const char *key, uint32_t index) {
    const JsonValue &v = obj[key];
    if (!v.IsUint64()) return false;
    writeU64(self, index, v.GetUint64());
    return true;
}
}  // namespace

// ------------------------------------------------------------------------------------------------- User
uint32_t User::parse(SharedPtr *out, const JsonValue *json, uint32_t withTag, uint32_t section) {
    GuestFrame frame(0x150);                    // original frame; the text / Date buffer at +0x30
    constexpr uint32_t BUFFER = 0x30;
    uint8_t *buffer = frame.at<uint8_t>(BUFFER);

    if (json->IsObject()) {
        const JsonValue &part = json->member(section);
        if (part.IsObject()) {
            const JsonValue &user = part["user"];
            if (user.IsObject()) {
                User *self = newObject<User>(32);
                constructSafeBuffer(self, SIZE);
                self->vtable = VTABLE;

                // strncpy the string into the buffer, terminate it at `last`, copy `size` bytes into the record
                auto readText = [&](const char *key, uint32_t size, uint32_t last, uint32_t index) {
                    const JsonValue &v = user[key];
                    if (!v.IsString()) return false;
                    rw::call(guest::strncpy_, {frame.addr(BUFFER), v.GetString(), size});
                    rw::store8(buffer + last, 0);
                    writeBytes(self, index, buffer, size);
                    return true;
                };
                auto readAll = [&]() -> bool {
                    if (!readUint64(self, user, "userId", USER_ID)) return false;
                    if (!readUint64(self, user, "nativeUserId", NATIVE_USER_ID)) return false;
                    if (!readInt(self, user, "platformId", PLATFORM_ID)) return false;
                    if (!readText("userName", 0x21, 0x20, USER_NAME)) return false;
                    if (!readInt(self, user, "gender", GENDER)) return false;
                    if (!readText("comment", 0x101, 0x100, COMMENT)) return false;
                    if (!readInt(self, user, "deviceType", DEVICE_TYPE)) return false;
                    if (!readInt(self, user, "continueLoginCount", CONTINUE_LOGIN_COUNT)) return false;
                    const JsonValue &fleeze = user["isFleeze"];
                    if (!fleeze.IsInt()) return false;
                    uint8_t frozen = fleeze.GetUint() != 0 ? 1 : 0;
                    writeBytes(self, IS_FLEEZE, &frozen, 1);
                    const JsonValue &when = user["fleezedDatetime"];
                    if (!when.IsString()) return false;
                    rw::store8(buffer + 7, 0);                           // Date flags cleared before init
                    if (rw::call(guest::Date_init, {frame.addr(BUFFER), when.GetString(), 0}) == 0) return false;
                    writeBytes(self, FLEEZED_DATETIME, buffer, 8);
                    if (withTag != 1) return true;
                    return readText("nativeTagName", 0xF, 0xE, NATIVE_TAG_NAME);
                };
                if (!readAll()) {
                    deleteVirtual(self);
                    self = nullptr;
                }
                rw::call(USER_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
                return 0;   // stack-protector check
            }
        }
    }
    out->reset();
    return 0;
}
RW_FUNCTION(0x49E448, "hole::network::api::User::parse", User::parse);

uint32_t User::destroyDelete(User *self) { return deleteSafeBufferResponse(self); }
RW_FUNCTION(0x4B6DB4, "hole::network::api::User::~User (deleting)", User::destroyDelete);

// ------------------------------------------------------------------------------------------------- UserDetail
uint32_t UserDetail::parse(SharedPtr *out, const JsonValue *response) {
    // Every value is staged in the 6-int local at +0x20 before it is written. The play-timezone array is
    // written from it whole, so slots the array does not fill keep earlier values / stack leftovers, and an
    // array longer than 6 runs past the local: both kept.
    GuestFrame frame(0x58);
    constexpr uint32_t LOCAL = 0x20;
    uint8_t *local = frame.at<uint8_t>(LOCAL);

    if (response->IsObject()) {
        const JsonValue &userData = (*response)["userData"];
        if (userData.IsObject()) {
            const JsonValue &detail = userData["userDetail"];
            if (detail.IsObject()) {
                UserDetail *self = newObject<UserDetail>(32);
                constructSafeBuffer(self, SIZE);
                self->vtable = VTABLE;

                auto stage32 = [&](uint32_t index, uint32_t v) {
                    rw::store32(local, v);
                    writeBytes(self, index, local, 4);
                };
                auto readStagedInt = [&](const char *key, uint32_t index) {
                    const JsonValue &v = detail[key];
                    if (!v.IsInt()) return false;
                    stage32(index, v.GetUint());
                    return true;
                };
                auto readAll = [&]() -> bool {
                    static const char *const head[] = {"level", "exp", "luxRank", "luxGetRatio", "titleLeftId",
                                                       "titleRightId", "titlePlateId", "maxDeckCost"};
                    for (uint32_t k = 0; k < 8; k++)
                        if (!readStagedInt(head[k], 4 * k)) return false;
                    const JsonValue &zones = detail["playTimezones"];
                    if (!zones.IsArray()) return false;
                    uint32_t count = zones.Size();
                    stage32(PLAY_TIMEZONE_COUNT, count < 6 ? count : 6);
                    const JsonValue &again = detail["playTimezones"];
                    if (!again.IsArray()) return false;
                    uint32_t n = again.Size();
                    for (uint32_t k = 0; k < n; k++) {
                        const JsonValue &v = again.At(k);
                        if (!v.IsInt()) return false;
                        rw::store32(local + 4 * k, v.GetUint());           // no bound (original)
                    }
                    writeBytes(self, PLAY_TIMEZONES, local, 24);
                    if (!readStagedInt("playFrequently", PLAY_FREQUENTLY)) return false;
                    const JsonValue &party = detail["partyId"];
                    if (!party.IsUint64()) return false;
                    rw::store32(local, party.w0);
                    rw::store32(local + 4, party.w1);
                    writeBytes(self, PARTY_ID, local, 8);
                    static const char *const tail[] = {"unionId", "maxMedal", "mvpCount", "equipCoordinateNo",
                                                       "lastClearStageId", "lastPlayNormalSphereBoardId",
                                                       "lastPlayStageSphereBoardId", "lastPlayRaidSphereBoardId",
                                                       "lastPlayColosseumSphereBoardId"};
                    for (uint32_t k = 0; k < 9; k++)
                        if (!readStagedInt(tail[k], UNION_ID + 4 * k)) return false;
                    const JsonValue &guilt = detail["isGuilt"];
                    if (!guilt.IsUint()) return false;
                    rw::store8(local, uint8_t(guilt.GetUint()));
                    writeBytes(self, IS_GUILT, local, 1);
                    return true;
                };
                if (!readAll()) {
                    deleteVirtual(self);
                    self = nullptr;
                }
                rw::call(USER_DETAIL_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
                return 0;   // stack-protector check
            }
        }
    }
    out->reset();
    return 0;
}
RW_FUNCTION(0x49EE1C, "hole::network::api::UserDetail::parse", UserDetail::parse);

uint32_t UserDetail::destroyDelete(UserDetail *self) { return deleteSafeBufferResponse(self); }
RW_FUNCTION(0x4B6D9C, "hole::network::api::UserDetail::~UserDetail (deleting)", UserDetail::destroyDelete);

// ------------------------------------------------------------------------------------------------- StageResumption
uint32_t StageResumption::parse(SharedPtr *out, const JsonValue *response) {
    if (response->IsObject()) {
        const JsonValue &userData = (*response)["userData"];
        if (userData.IsObject()) {
            const JsonValue &resumption = userData["stageResumption"];
            if (resumption.IsObject()) {
                StageResumption *self = newObject<StageResumption>(32);
                constructSafeBuffer(self, SIZE);
                self->vtable = VTABLE;
                auto readAll = [&]() -> bool {
                    const JsonValue &status = resumption["resumptionStatus"];
                    if (!status.IsInt() || status.GetUint() > 3) return false;
                    writeU32(self, RESUMPTION_STATUS, status.GetUint());
                    if (!readInt(self, resumption, "stageId", STAGE_ID)) return false;
                    if (!readUint64(self, resumption, "raidId", RAID_ID)) return false;
                    return readInt(self, resumption, "colosseumStageId", COLOSSEUM_STAGE_ID);
                };
                if (!readAll()) {
                    deleteVirtual(self);
                    self = nullptr;
                }
                rw::call(RESUMPTION_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
                return 0;   // stack-protector check
            }
        }
    }
    out->reset();
    return 0;
}
RW_FUNCTION(0x49F4B0, "hole::network::api::StageResumption::parse", StageResumption::parse);

uint32_t StageResumption::destroyDelete(StageResumption *self) { return deleteSafeBufferResponse(self); }
RW_FUNCTION(0x4B6D90, "hole::network::api::StageResumption::~StageResumption (deleting)",
            StageResumption::destroyDelete);

// The deleting destructor of UserPoint (its parser is in UserPoint.cpp).
static uint32_t userPointDestroyDelete(UserPoint *self) { return deleteSafeBufferResponse(self); }
RW_FUNCTION(0x4B6DA8, "hole::network::api::UserPoint::~UserPoint (deleting)", userPointDestroyDelete);

}  // namespace hole::network::api
