// KeyBladeDataManager: the UI-side view of an owned keyblade (UserKeyBladeData): its user id in a SafeBuffer and
// the master::Keyblade row. A singleton (0x404 bytes, unused by the functions here).
#pragma once

#include "rw/rw.hpp"
#include "core/Guest.hpp"
#include "core/SafeBuffer.hpp"

struct KeyBladeDataManager {
    // SafeBuffer(8): data position 0 = the user keyblade id (u64); then shared_ptr<master::Keyblade>.
    struct UserKeyBladeData : SafeBuffer {
        guest::SharedPtr keyblade;             // +0x20
        static constexpr uint32_t VTABLE = 0xFA982C;   // _ZTVN19KeyBladeDataManager16UserKeyBladeDataE + 8
        static UserKeyBladeData *destruct(UserKeyBladeData *self);
        static void destructDelete(UserKeyBladeData *self);
    };

    static constexpr uint32_t INSTANCE = 0x10040F4;

    static uint32_t getInstance();
    static void construct(uint32_t self);
    static void destruct(uint32_t self);
    // shared_ptr<UserKeyBladeData> for user keyblade `id` (the UserKeyblade::Keyblade record by value follows
    // on the stack and is overwritten from the user's list when owned); keybladeId (non-zero) overrides the
    // master keyblade and clears the user id.
    static void getKeyBladeData(guest::SharedPtr *out, uint32_t unusedThis, uint64_t id, uint32_t keybladeId);
    static void getKeyBladeImagePath(uint32_t stringBuffer, const guest::SharedPtr *data);
};
RW_SIZE(KeyBladeDataManager::UserKeyBladeData, 0x28);
