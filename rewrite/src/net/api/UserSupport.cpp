#include "UserSupport.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t FRAME = 0x60;          // original frame: 5 pushed registers + 0x4c of locals
constexpr uint32_t LOCAL = 0x20;          // the value local: ints, uint64s, the name buffer (34 bytes), the Date
constexpr uint32_t SHARED_PTR_CTOR = 0x4A1CA5;   // std::shared_ptr<UserSupport>::shared_ptr(UserSupport*)
constexpr uint32_t STRNCPY = 0x410E4C;           // strncpy (import)

// Each value goes through the guest-stack local (as in the original), then into the SafeBuffer.
class Reader {
public:
    Reader(UserSupport *self, const JsonValue &obj, const GuestFrame &frame)
        : self_(self), obj_(obj), local_(frame.at<uint8_t>(LOCAL)), frame_(frame) {}

    bool readInt(const char *key, uint32_t index) {
        const JsonValue &v = obj_[key];
        if (!v.IsInt()) return false;
        rw::store32(local_, v.w0);
        SafeBuffer::write(self_, self_->data.get() + index, local_, 4, index);
        return true;
    }
    bool readUint64(const char *key, uint32_t index) {
        const JsonValue &v = obj_[key];
        if (!v.IsUint64()) return false;
        rw::store32(local_, v.w0);
        rw::store32(local_ + 4, v.w1);
        SafeBuffer::write(self_, self_->data.get() + index, local_, 8, index);
        return true;
    }
    // strncpy into the local (zero padded, 33 bytes), forced NUL at byte 32
    bool readName(const char *key, uint32_t index) {
        const JsonValue &v = obj_[key];
        if (!v.IsString()) return false;
        rw::call(STRNCPY, {frame_.addr(LOCAL), v.GetString(), UserSupport::USER_NAME_SIZE});
        rw::store8(local_ + UserSupport::USER_NAME_SIZE - 1, 0);
        SafeBuffer::write(self_, self_->data.get() + index, local_, UserSupport::USER_NAME_SIZE, index);
        return true;
    }
    bool readDate(const char *key, uint32_t index) {
        const JsonValue &v = obj_[key];
        if (!v.IsString()) return false;
        rw::store8(local_ + 7, 0);                   // Date flags byte cleared before init
        if (rw::call(guest::Date_init, {frame_.addr(LOCAL), v.GetString(), 0}) == 0) return false;
        SafeBuffer::write(self_, self_->data.get() + index, local_, 8, index);
        return true;
    }

private:
    UserSupport *self_;
    const JsonValue &obj_;
    uint8_t *local_;
    const GuestFrame &frame_;
};

// Every field must be present with the right type; the first bad one stops the parse.
bool readAll(UserSupport *self, const JsonValue &user, const GuestFrame &frame) {
    Reader r(self, user, frame);
    return r.readUint64("supportUserId", UserSupport::SUPPORT_USER_ID) &&
           r.readInt("level", UserSupport::LEVEL) &&
           r.readUint64("partyId", UserSupport::PARTY_ID) &&
           r.readName("userName", UserSupport::USER_NAME) &&
           r.readInt("titleLeftId", UserSupport::TITLE_LEFT_ID) &&
           r.readInt("titleRightId", UserSupport::TITLE_RIGHT_ID) &&
           r.readInt("titlePlateId", UserSupport::TITLE_PLATE_ID) &&
           r.readUint64("userKeybladeId", UserSupport::USER_KEYBLADE_ID) &&
           r.readInt("keybladeId", UserSupport::KEYBLADE_ID) &&
           r.readUint64("userMedalId", UserSupport::USER_MEDAL_ID) &&
           r.readInt("medalId", UserSupport::MEDAL_ID) &&
           r.readDate("lastActionDatetime", UserSupport::LAST_ACTION_DATETIME);
}
}  // namespace

uint32_t UserSupport::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(FRAME);
    // the object is made first, and deleted again when anything is missing
    UserSupport *self = newObject<UserSupport>(32);
    constructSafeBuffer(self, SIZE);
    self->vtable = VTABLE;
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &user = (*response)["supportUser"];
        if (user.IsObject()) ok = readAll(self, user, frame);
    }
    if (!ok) {
        deleteVirtual(self);
        self = nullptr;
    }
    rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A18D4, "hole::network::api::UserSupport::parse", UserSupport::parse);

uint32_t UserSupport::destroyDelete(UserSupport *self) {
    uint32_t p = rw::call(guest::SafeBuffer_dtor, {rw::guest_addr(self)});
    return rw::call(guest::operator_delete, {p});
}
RW_FUNCTION(0x4B6CE2, "hole::network::api::UserSupport::~UserSupport (deleting)", UserSupport::destroyDelete);

}  // namespace hole::network::api
