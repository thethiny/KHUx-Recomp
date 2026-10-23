// Batch #10: the rest of the sqexsdlib C API (SdSoundSystem_*) - the per-sound / per-category / per-bank controls,
// the sounds created with an external id or a 3D layout, the listener, the performance numbers.
//
// The sound and category controls wrap a handle in a SQEX::Sound / SQEX::Category on the stack and call its
// method. A negative fade time is logged and replaced by 0. Quirks kept from the original: SetRandomVolume calls
// Sound::SetRandomPitch, and MovePosition calls Sound::SetPosition.
#include "../w4_b2/SqexPool.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t SD = 0x100E598;               // SdSoundSystem state (mutex +0, bank pool +4, used list +0x14)
constexpr uint32_t FIND_BANK = 0xCD8E1C;
constexpr uint32_t JAVA_VM_GOT = 0xFD9258;       // the JavaVM*
constexpr uint32_t ASSET_MANAGER_FROM_JAVA = 0x444A04;  // AAssetManager_fromJava (import)
constexpr uint32_t FILE_SET_ASSET_MANAGER = 0xCE9A30;
constexpr uint32_t AKB_SOUND_IS_LOOP = 0xCDCA78;
constexpr uint32_t ACTION_ADD = 0xCDD430;        // SQEX::ActionManager::AddAction(const ACTION&)
constexpr uint32_t CORE_GET_PERFORMANCE = 0xCE6DA0;
constexpr uint32_t FLOATDISF = 0xCD6D98;         // __floatdisf

// SQEX::Sound(int handle) and its methods
constexpr uint32_t SOUND_CTOR = 0xCE1334;
constexpr uint32_t SOUND_PLAY = 0xCE133C, SOUND_SET_VOLUME = 0xCE13E0, SOUND_SET_PITCH = 0xCE140C,
                   SOUND_SET_PAN = 0xCE1438, SOUND_SET_RANDOM_PITCH = 0xCE1498, SOUND_GET_VOLUME = 0xCE14F4,
                   SOUND_GET_STATUS = 0xCE14FC, SOUND_IS_LOOP = 0xCE1574, SOUND_GET_PLAY_TIME = 0xCE157C,
                   SOUND_GET_ELAPSED = 0xCE1584, SOUND_SET_POSITION = 0xCE158C;
// SQEX::Category(int id) and its methods
constexpr uint32_t CATEGORY_CTOR = 0xCE15C8;
constexpr uint32_t CATEGORY_GET_COUNT = 0xCE15D0, CATEGORY_STOP = 0xCE15EC, CATEGORY_PAUSE_ON = 0xCE1618,
                   CATEGORY_PAUSE_OFF = 0xCE1644, CATEGORY_SET_VOLUME = 0xCE1670, CATEGORY_GET_VOLUME = 0xCE16A4,
                   CATEGORY_GET_MAX = 0xCE16C8;
// BankImpl
constexpr uint32_t BANK_CREATE_SOUND_EXT = 0xCE970C, BANK_CREATE_LAYOUT = 0xCE97D0,
                   BANK_CREATE_LAYOUT_EXT = 0xCE988C, BANK_IS_BELONG = 0xCE9994;

// log strings
constexpr uint32_t S_BANK_NOT_FOUND = 0xEECC5C;   // "SdSoundSystem_CreateSound failed! bank not found %d"
constexpr uint32_t S_BANK_REMOVING = 0xEECC90;    // "... bank removing now! %d"
constexpr uint32_t S_SET_VOLUME_TIME = 0xEECE04, S_SET_RANDOM_VOLUME_TIME = 0xEECE54,
                   S_SET_PITCH_TIME = 0xEECEAC, S_SET_RANDOM_PITCH_TIME = 0xEECEFC, S_SET_PAN_TIME = 0xEECF54;

// a SQEX::Sound for the handle, on the stack; the method gets it and `args`
template <class... A> uint32_t soundCall(uint32_t handle, uint32_t method, A... args) {
    guest::Frame f(8);
    rw::call(SOUND_CTOR, {f.addr(4), handle});
    return rw::call(method, {f.addr(4), uint32_t(args)...});
}
template <class... A> uint32_t categoryCall(uint32_t id, uint32_t method, A... args) {
    guest::Frame f(8);
    rw::call(CATEGORY_CTOR, {f.addr(4), id});
    return rw::call(method, {f.addr(4), uint32_t(args)...});
}
// a negative fade time is logged and becomes 0
uint32_t checkTime(int32_t ms, uint32_t message) {
    if (ms >= 0) return uint32_t(ms);
    log(5, message, uint32_t(ms));
    return 0;
}

// a bank call under the SdSoundSystem mutex: -1 (logged) when the bank is missing or being removed
template <class... A> int32_t bankCreate(uint32_t bankId, uint32_t method, A... args) {
    mutexLock(SD);
    int32_t r;
    const uint32_t bank = rw::call(FIND_BANK, {bankId});
    if (bank == 0) {
        log(5, S_BANK_NOT_FOUND, bankId);
        r = -1;
    } else if (ld8(bank + 0x14) != 0) {
        log(5, S_BANK_REMOVING, bankId);
        r = -1;
    } else {
        r = int32_t(rw::call(method, {bank, uint32_t(args)...}));
    }
    mutexUnlock(SD);
    return r;
}
// a bank's virtual query (vtable slot) under the mutex; 0 for a missing bank
uint32_t bankQuery(uint32_t bankId, uint32_t slot, uint32_t arg) {
    mutexLock(SD);
    uint32_t r = rw::call(FIND_BANK, {bankId});
    if (r != 0) r = vcallo(r, slot, {r, arg});
    mutexUnlock(SD);
    return r;
}

// SdSoundSystem_Layout_SetListener*: an action {type, values...} queued for the sound thread
uint32_t queueAction(uint32_t type, std::initializer_list<uint32_t> values, uint32_t frameSize) {
    guest::Frame f(frameSize);
    *f.at<uint32_t>(0) = type;
    uint32_t k = 4;
    for (uint32_t v : values) {
        *f.at<uint32_t>(k) = v;
        k += 4;
    }
    return rw::call(ACTION_ADD, {f.addr(0)});
}

}  // namespace

struct SdSoundSystemApi {
    // the Java AssetManager for SQEX::File (GetEnv through the JavaVM, then AAssetManager_fromJava)
    static uint32_t setAssetManagerFromJava(uint32_t jassetManager) {
        guest::Frame f(8);
        const uint32_t vm = ld32(ld32(JAVA_VM_GOT));
        *f.at<uint32_t>(4) = 0;
        rw::call(ld32(ld32(vm) + 0x10), {vm, f.addr(4), 0});   // GetEnv(vm, &env, 0)
        const uint32_t manager = rw::call(ASSET_MANAGER_FROM_JAVA, {ld32(f.addr(4)), jassetManager});
        return rw::call(FILE_SET_ASSET_MANAGER, {manager});
    }
    static uint32_t akbGetSoundIsLoop(uint32_t akb, uint32_t index) {
        return rw::call(AKB_SOUND_IS_LOOP, {akb, index}) == 1;
    }

    static uint32_t bankIsLoop(uint32_t bankId, uint32_t index) { return bankQuery(bankId, 0xC, index); }
    static uint32_t bankGetPlayTime(uint32_t bankId, uint32_t index) { return bankQuery(bankId, 0x10, index); }

    static int32_t createSoundWithExternalId(uint32_t bankId, uint32_t index, uint32_t externalId) {
        return bankCreate(bankId, BANK_CREATE_SOUND_EXT, index, uint32_t(int32_t(int16_t(externalId))));
    }
    static int32_t createLayoutSound(uint32_t bankId, uint32_t index) {
        return bankCreate(bankId, BANK_CREATE_LAYOUT, index);
    }
    static int32_t createLayoutSoundWithExternalId(uint32_t bankId, uint32_t index, uint32_t externalId) {
        return bankCreate(bankId, BANK_CREATE_LAYOUT_EXT, index, uint32_t(int32_t(int16_t(externalId))));
    }

    static int32_t soundStart(uint32_t handle, uint32_t seek) {
        return soundCall(handle, SOUND_PLAY, 0, seek) != 0 ? -1 : 0;
    }
    static uint32_t soundIsPaused(uint32_t handle) { return soundCall(handle, SOUND_GET_STATUS) == 4; }
    static uint32_t soundSetVolume(uint32_t handle, uint32_t volume, int32_t ms) {
        const uint32_t t = checkTime(ms, S_SET_VOLUME_TIME);
        return soundCall(handle, SOUND_SET_VOLUME, volume, t);
    }
    static uint32_t soundSetRandomVolume(uint32_t handle, uint32_t from, uint32_t to, int32_t ms) {
        const uint32_t t = checkTime(ms, S_SET_RANDOM_VOLUME_TIME);
        return soundCall(handle, SOUND_SET_RANDOM_PITCH, from, to, t);   // sic: the random pitch method
    }
    static uint32_t soundGetVolume(uint32_t handle) { return soundCall(handle, SOUND_GET_VOLUME); }
    static uint32_t soundSetPitch(uint32_t handle, uint32_t pitch, int32_t ms) {
        const uint32_t t = checkTime(ms, S_SET_PITCH_TIME);
        return soundCall(handle, SOUND_SET_PITCH, pitch, t);
    }
    static uint32_t soundSetRandomPitch(uint32_t handle, uint32_t from, uint32_t to, int32_t ms) {
        const uint32_t t = checkTime(ms, S_SET_RANDOM_PITCH_TIME);
        return soundCall(handle, SOUND_SET_RANDOM_PITCH, from, to, t);
    }
    static uint32_t soundIsLoop(uint32_t handle) { return soundCall(handle, SOUND_IS_LOOP) == 1; }
    static uint32_t soundGetPlayTime(uint32_t handle) { return soundCall(handle, SOUND_GET_PLAY_TIME); }
    static uint32_t soundGetElapsedPlaybackTime(uint32_t handle) {
        return soundCall(handle, SOUND_GET_ELAPSED);
    }
    static uint32_t soundSetPanning(uint32_t handle, uint32_t pan, int32_t ms) {
        const uint32_t t = checkTime(ms, S_SET_PAN_TIME);
        return soundCall(handle, SOUND_SET_PAN, pan, t);
    }
    // the id of the bank the sound belongs to (0: none)
    static uint32_t soundGetBankId(uint32_t handle) {
        if (handle == 0) return 0;
        mutexLock(SD);
        uint32_t id = 0;
        for (uint32_t node = ld32(SD + 0x14); node != 0; node = ld32(node + 8)) {
            if (rw::call(BANK_IS_BELONG, {ld32(node + 0x14), handle}) != 0) {
                id = ld32(ld32(node + 0x14) + 4);
                break;
            }
            if (!(ld8(node + 0x10) & 1)) break;
        }
        mutexUnlock(SD);
        return id;
    }
    static uint32_t soundSetPosition(uint32_t handle, uint32_t x, uint32_t y, uint32_t z) {
        return soundCall(handle, SOUND_SET_POSITION, x, y, z);
    }
    static uint32_t soundMovePosition(uint32_t handle, uint32_t x, uint32_t y, uint32_t z) {
        return soundCall(handle, SOUND_SET_POSITION, x, y, z);   // sic: the same method as SetPosition
    }

    static uint32_t categoryGetSoundsCount(uint32_t id) { return categoryCall(id, CATEGORY_GET_COUNT); }
    static uint32_t categoryGetMaxSounds(uint32_t id) { return categoryCall(id, CATEGORY_GET_MAX); }
    static uint32_t categorySetVolume(uint32_t id, uint32_t volume, uint32_t ms) {
        return categoryCall(id, CATEGORY_SET_VOLUME, volume, ms);
    }
    static uint32_t categoryGetVolume(uint32_t id) { return categoryCall(id, CATEGORY_GET_VOLUME); }
    static uint32_t categoryStop(uint32_t id, uint32_t ms) { return categoryCall(id, CATEGORY_STOP, ms); }
    static uint32_t categorySetPause(uint32_t id, uint32_t pause, uint32_t ms) {
        return categoryCall(id, pause ? CATEGORY_PAUSE_ON : CATEGORY_PAUSE_OFF, ms);
    }

    // {core performance word 0, word 2, the sound thread's last frame time (us), that time / 16000 (load)}
    static void utilGetPerformance(uint32_t out) {
        guest::Frame f(0x10);
        rw::call(CORE_GET_PERFORMANCE, {f.addr(0)});
        st32(out + 4, ld32(f.addr(8)));
        st32(out, ld32(f.addr(0)));
        st32(out + 8, ld32(SD + 0x20));
        const uint32_t time = rw::call(FLOATDISF, {ld32(SD + 0x20), ld32(SD + 0x24)});
        stf(out + 0xC, bitsf(time) / 16000.0f);
    }

    // the listener's position, front and up vectors (action 0xF) / its front and up only (action 0x10)
    static uint32_t layoutSetListener(uint32_t px, uint32_t py, uint32_t pz, uint32_t fx, uint32_t fy, uint32_t fz,
                                      uint32_t ux, uint32_t uy, uint32_t uz) {
        return queueAction(0xF, {px, py, pz, fx, fy, fz, ux, uy, uz}, 0x2C);
    }
    static uint32_t layoutSetListenerRotation(uint32_t fx, uint32_t fy, uint32_t fz, uint32_t ux, uint32_t uy,
                                              uint32_t uz) {
        return queueAction(0x10, {fx, fy, fz, ux, uy, uz}, 0x28);
    }
};
RW_FUNCTION(0xCD922C, "SdSoundSystem_SetAssetManagerFromJava", SdSoundSystemApi::setAssetManagerFromJava);
RW_FUNCTION(0xCD927C, "SdSoundSystem_Akb_GetSoundIsLoop", SdSoundSystemApi::akbGetSoundIsLoop);
RW_FUNCTION(0xCDA784, "SdSoundSystem_BankCtrl_IsLoop", SdSoundSystemApi::bankIsLoop);
RW_FUNCTION(0xCDA7DC, "SdSoundSystem_BankCtrl_GetPlayTime", SdSoundSystemApi::bankGetPlayTime);
RW_FUNCTION(0xCDAA00, "SdSoundSystem_CreateSoundWithExternalID", SdSoundSystemApi::createSoundWithExternalId);
RW_FUNCTION(0xCDAABC, "SdSoundSystem_CreateLayoutSound", SdSoundSystemApi::createLayoutSound);
RW_FUNCTION(0xCDAB70, "SdSoundSystem_CreateLayoutSoundWithExternalID",
            SdSoundSystemApi::createLayoutSoundWithExternalId);
RW_FUNCTION(0xCDAC70, "SdSoundSystem_SoundCtrl_Start", SdSoundSystemApi::soundStart);
RW_FUNCTION(0xCDAE5C, "SdSoundSystem_SoundCtrl_IsPaused", SdSoundSystemApi::soundIsPaused);
RW_FUNCTION(0xCDAE90, "SdSoundSystem_SoundCtrl_SetVolume", SdSoundSystemApi::soundSetVolume);
RW_FUNCTION(0xCDAEFC, "SdSoundSystem_SoundCtrl_SetRandomVolume", SdSoundSystemApi::soundSetRandomVolume);
RW_FUNCTION(0xCDAF6C, "SdSoundSystem_SoundCtrl_GetVolume", SdSoundSystemApi::soundGetVolume);
RW_FUNCTION(0xCDAF94, "SdSoundSystem_SoundCtrl_SetPitch", SdSoundSystemApi::soundSetPitch);
RW_FUNCTION(0xCDB000, "SdSoundSystem_SoundCtrl_SetRandomPitch", SdSoundSystemApi::soundSetRandomPitch);
RW_FUNCTION(0xCDB070, "SdSoundSystem_SoundCtrl_IsLoop", SdSoundSystemApi::soundIsLoop);
RW_FUNCTION(0xCDB0A4, "SdSoundSystem_SoundCtrl_GetPlayTime", SdSoundSystemApi::soundGetPlayTime);
RW_FUNCTION(0xCDB0CC, "SdSoundSystem_SoundCtrl_GetElapsedPlaybackTime", SdSoundSystemApi::soundGetElapsedPlaybackTime);
RW_FUNCTION(0xCDB0F4, "SdSoundSystem_SoundCtrl_SetPanning", SdSoundSystemApi::soundSetPanning);
RW_FUNCTION(0xCDB170, "SdSoundSystem_SoundCtrl_GetBankID", SdSoundSystemApi::soundGetBankId);
RW_FUNCTION(0xCDB1F8, "SdSoundSystem_SoundCtrl_SetPosition", SdSoundSystemApi::soundSetPosition);
RW_FUNCTION(0xCDB238, "SdSoundSystem_SoundCtrl_MovePosition", SdSoundSystemApi::soundMovePosition);
RW_FUNCTION(0xCDB2A0, "SdSoundSystem_CategoryCtrl_GetSoundsCount", SdSoundSystemApi::categoryGetSoundsCount);
RW_FUNCTION(0xCDB2C8, "SdSoundSystem_CategoryCtrl_GetMaxSounds", SdSoundSystemApi::categoryGetMaxSounds);
RW_FUNCTION(0xCDB2F8, "SdSoundSystem_CategoryCtrl_SetVolume", SdSoundSystemApi::categorySetVolume);
RW_FUNCTION(0xCDB330, "SdSoundSystem_CategoryCtrl_GetVolume", SdSoundSystemApi::categoryGetVolume);
RW_FUNCTION(0xCDB358, "SdSoundSystem_CategoryCtrl_Stop", SdSoundSystemApi::categoryStop);
RW_FUNCTION(0xCDB388, "SdSoundSystem_CategoryCtrl_SetPause", SdSoundSystemApi::categorySetPause);
RW_FUNCTION(0xCDB3D8, "SdSoundSystem_Util_GetPerformance", SdSoundSystemApi::utilGetPerformance);
RW_FUNCTION(0xCDB43C, "SdSoundSystem_Layout_SetListener", SdSoundSystemApi::layoutSetListener);
RW_FUNCTION(0xCDB494, "SdSoundSystem_Layout_SetListenerRotation", SdSoundSystemApi::layoutSetListenerRotation);

}  // namespace sqex
