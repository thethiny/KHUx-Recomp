// KeyBladeDataManager: the owned-keyblade view model used by deck editing.
#include "KeyBladeDataManager.hpp"

namespace {
constexpr uint32_t MAKE_SHARED_USER_KEYBLADE_DATA = 0x76B871;   // std::make_shared<UserKeyBladeData>()
constexpr uint32_t APIMANAGER_GET_INSTANCE = 0x4BA8C9;
constexpr uint32_t APIMANAGER_USER_KEYBLADE = 0xEC;              // shared_ptr<UserKeyblade> (88-byte records)
constexpr uint32_t KEYBLADE_CREATE = 0x783F9D;                   // master::Keyblade::create(int)
constexpr uint32_t STRINGBUFFER_FSET = 0x491501;
constexpr uint32_t STR_KEYBLADE_IMAGE = 0xDF9213;                // "img/keyBlade/KeyBlade_%06d.png"
constexpr uint32_t KEYBLADE_DISPLAY_ID = 204;                    // master::Keyblade data position
constexpr uint32_t RECORD_SIZE = 88;
constexpr uint32_t RECORD_KEYBLADE_ID = 12;                      // UserKeyblade::Keyblade::keybladeId
}  // namespace

uint32_t KeyBladeDataManager::getInstance() {
    uint32_t self = rw::load<uint32_t>(INSTANCE);
    if (!self) {
        self = guest::newObject(0x404);
        rw::save<uint32_t>(self + 0x400, 0);
        rw::save<uint8_t>(self, 0);
        rw::save<uint32_t>(INSTANCE, self);
    }
    return self;
}
RW_FUNCTION(0x76B60C, "KeyBladeDataManager::getInstance", KeyBladeDataManager::getInstance);

void KeyBladeDataManager::construct(uint32_t self) {
    rw::save<uint32_t>(self + 0x400, 0);
    rw::save<uint8_t>(self, 0);
}
RW_FUNCTION(0x76B648, "KeyBladeDataManager::KeyBladeDataManager", KeyBladeDataManager::construct);

void KeyBladeDataManager::destruct(uint32_t) {}
RW_FUNCTION(0x76B654, "KeyBladeDataManager::~KeyBladeDataManager", KeyBladeDataManager::destruct);

void KeyBladeDataManager::getKeyBladeData(guest::SharedPtr *out, uint32_t, uint64_t id, uint32_t keybladeId) {
    // the UserKeyblade::Keyblade argument passed by value (88 bytes, 8 bytes into the stack arguments)
    const uint32_t record = rw::cpu->r[13] + 8;
    guest::Frame f(16);
    rw::call(MAKE_SHARED_USER_KEYBLADE_DATA, {rw::guest_addr(out)});
    uint32_t api = rw::call(APIMANAGER_GET_INSTANCE, {});
    uint32_t apiCtrl = rw::load<uint32_t>(api + APIMANAGER_USER_KEYBLADE + 4);
    uint32_t list = rw::load<uint32_t>(api + APIMANAGER_USER_KEYBLADE);
    if (apiCtrl) guest::addRef(apiCtrl);
    const SafeBuffer *sb = rw::host_ptr<SafeBuffer>(list);
    uint32_t count = rw::load<uint32_t>(list + 0x20);
    bool found = false;
    uint32_t pos = 0;
    for (uint32_t k = 0; k < count; k++, pos += RECORD_SIZE) {
        uint64_t recId = uint32_t(sb->get32(pos)) | (uint64_t(uint32_t(sb->get32(pos + 4))) << 32);
        if (recId == id) {
            found = k != 0xFFFFFFFFu;
            break;
        }
    }
    UserKeyBladeData *data;
    const uint8_t zeros[8] = {};
    if (found) {
        for (uint32_t k = 0; k < RECORD_SIZE; k += 4) rw::save<uint32_t>(record + k, uint32_t(sb->get32(pos + k)));
        data = rw::host_ptr<UserKeyBladeData>(out->obj);
        SafeBuffer::write(data, data->data.get(), rw::host_ptr<uint8_t>(record), 8, 0);
    } else {
        if (keybladeId) rw::save<uint32_t>(record + RECORD_KEYBLADE_ID, keybladeId);
        rw::save<uint32_t>(record, 0);
        rw::save<uint32_t>(record + 4, 0);
        data = rw::host_ptr<UserKeyBladeData>(out->obj);
        SafeBuffer::write(data, data->data.get(), zeros, 8, 0);
    }
    uint32_t masterId;
    if (keybladeId) {
        SafeBuffer::write(data, data->data.get(), zeros, 8, 0);
        masterId = keybladeId;
    } else {
        masterId = rw::load<uint32_t>(record + RECORD_KEYBLADE_ID);
    }
    uint32_t row = f.addr(0);
    rw::call(KEYBLADE_CREATE, {row, masterId});
    uint32_t ctrl = rw::load<uint32_t>(row + 4), obj = rw::load<uint32_t>(row);
    if (ctrl) guest::addRef(ctrl);
    data->keyblade.obj = obj;
    uint32_t old = data->keyblade.ctrl;
    data->keyblade.ctrl = ctrl;
    if (old) guest::release(old);
    if (rw::load<uint32_t>(row + 4)) guest::release(rw::load<uint32_t>(row + 4));
    if (apiCtrl) guest::release(apiCtrl);
}
RW_FUNCTION(0x76B658, "KeyBladeDataManager::getKeyBladeData", KeyBladeDataManager::getKeyBladeData);

void KeyBladeDataManager::getKeyBladeImagePath(uint32_t stringBuffer, const guest::SharedPtr *data) {
    uint32_t row = rw::host_ptr<UserKeyBladeData>(data->obj)->keyblade.obj;
    int32_t displayId = rw::host_ptr<SafeBuffer>(row)->get32(KEYBLADE_DISPLAY_ID);
    rw::call(STRINGBUFFER_FSET, {stringBuffer, STR_KEYBLADE_IMAGE, uint32_t(displayId)});
}
RW_FUNCTION(0x76B808, "KeyBladeDataManager::getKeyBladeImagePath", KeyBladeDataManager::getKeyBladeImagePath);

KeyBladeDataManager::UserKeyBladeData *KeyBladeDataManager::UserKeyBladeData::destruct(UserKeyBladeData *self) {
    self->vtable = VTABLE;
    if (self->keyblade.ctrl) guest::release(self->keyblade.ctrl);
    SafeBuffer::destruct(self);
    return self;
}
RW_FUNCTION(0x76B9A0, "KeyBladeDataManager::UserKeyBladeData::~UserKeyBladeData",
            KeyBladeDataManager::UserKeyBladeData::destruct);

void KeyBladeDataManager::UserKeyBladeData::destructDelete(UserKeyBladeData *self) {
    destruct(self);
    guest::deleteObject(rw::guest_addr(self));
}
RW_FUNCTION(0x76B9D4, "KeyBladeDataManager::UserKeyBladeData::~UserKeyBladeData (deleting)",
            KeyBladeDataManager::UserKeyBladeData::destructDelete);
