// SoundSystem: the static front end of the SQEX sound library (sdlib 15.6.05.A): start-up / shutdown of its
// managers, suspend / resume, sound creation from AKB data, the per-frame update (from the game loop or an update
// thread) and AKB -> PCM conversion. State lives in a small block of globals at 0x100E610.
#include "SystemCommon.hpp"

namespace soundsystem {

namespace {
// globals
constexpr uint32_t INITIALIZED = 0x100E610;     // uint8
constexpr uint32_t LAST_TIME = 0x100E618;       // uint64: Timer::GetTime() of the last update
constexpr uint32_t SUSPENDED = 0x100E620;       // uint8
constexpr uint32_t STOP_THREAD = 0x100E621;     // uint8: asks the update thread to stop
constexpr uint32_t UPDATE_THREAD = 0x100E624;   // SQEX::Thread
constexpr uint32_t SOUND_LIMIT = 0x1002BD4;     // int, -1 = none
constexpr uint32_t OGG_CALLBACKS = 0x1002BD8;   // ov_callbacks (4 function pointers) for in-memory Ogg data
constexpr uint32_t GOT_OGG_MALLOC = 0xFD9298;   // &ogg_malloc (the libogg/libvorbis allocator hooks)
constexpr uint32_t GOT_OGG_CALLOC = 0xFD929C;
constexpr uint32_t GOT_OGG_REALLOC = 0xFD92A0;
constexpr uint32_t GOT_OGG_FREE = 0xFD92A4;

// library entry points (ARM)
constexpr uint32_t LOG = 0x413D38;              // __android_log_print
constexpr uint32_t MEMCPY = 0x442298;
constexpr uint32_t MEMORY_INITIALIZE = 0xCE4378, MEMORY_RELEASE = 0xCE4394;
constexpr uint32_t MEMORY_MALLOC = 0xCE43B4, MEMORY_CALLOC = 0xCE43B8, MEMORY_REALLOC = 0xCE43BC;
constexpr uint32_t MEMORY_FREE_THUNK = 0xCE0444;   // j_SQEX::Memory::Free
constexpr uint32_t TIMER_INITIALIZE = 0xCE2420, TIMER_GET_TIME = 0xCE2428;
constexpr uint32_t DRIVER_CONFIG_CTOR = 0xCE5DF8, DRIVER_CONFIG_DTOR = 0xCE5E30, DRIVER_CONFIG_ANALYSE = 0xCE5E34;
constexpr uint32_t CORE_INITIALIZE = 0xCE694C, CORE_RELEASE = 0xCE7478, CORE_SUSPEND = 0xCE6D08;
constexpr uint32_t CORE_RESUME = 0xCE6D48, CORE_SET_MASTER_VOLUME = 0xCE6C88, CORE_GET_MASTER_VOLUME = 0xCE6CF8;
constexpr uint32_t CORE_UPDATE = 0xCE6D78;
constexpr uint32_t DELEGATE_INITIALIZE = 0xCF41FC, DELEGATE_RELEASE = 0xCF4260;
constexpr uint32_t ACTION_INITIALIZE = 0xCDD240, ACTION_RELEASE = 0xCDD3B4, ACTION_CLEAR = 0xCDD4F4;
constexpr uint32_t ACTION_ADD = 0xCDD430, ACTION_UPDATE = 0xCDD554;
constexpr uint32_t SOUND_INITIALIZE = 0xCDF0AC, SOUND_RELEASE = 0xCDF228, SOUND_STOP_ALL = 0xCDF2A4;
constexpr uint32_t SOUND_GET_COUNT = 0xCDEDE4, SOUND_UPDATE = 0xCE0170;
constexpr uint32_t SOUND_CREATE = 0xCDF37C;              // CreateSound(Sound*, data, size, uint8, short, uint8)
constexpr uint32_t SOUND_CREATE_STREAMING = 0xCDFA8C;    // CreateStreamingSound(Sound*, reader, ctx, int, uint8, short, uint8)
constexpr uint32_t CATEGORY_INITIALIZE = 0xCE5BC4, CATEGORY_RELEASE = 0xCE5CB0, CATEGORY_UPDATE = 0xCE5D44;
constexpr uint32_t THREAD_SLEEP = 0xCE2364, THREAD_IS_ACTIVE = 0xCE2388, THREAD_CREATE = 0xCE2398;
constexpr uint32_t THREAD_JOIN = 0xCE23F4;
constexpr uint32_t AKB_IS_AKB_FILE = 0xCDC3CC, AKB_GET_HEADER_SIZE = 0xCDC42C, AKB_GET_FILE_SIZE = 0xCDC4D4;
constexpr uint32_t AKB_GET_AUDIO_FORMAT = 0xCDC8A4, AKB_GET_AUDIO_DATA = 0xCDC93C, AKB_GET_CHANNEL = 0xCDC9CC;
constexpr uint32_t AKB_IS_LOOP_AUDIO = 0xCDCA04, AKB_GET_NUM_SAMPLES = 0xCDCAAC, AKB_GET_AUDIO_DATA_SIZE = 0xCDCBA8;
constexpr uint32_t AKB_MSADPCM_BLOCK_ALIGN = 0xCDCDDC, AKB_MSADPCM_NUM_SAMPLES = 0xCDCDFC;
constexpr uint32_t AKB_MSADPCM_LOOP_START = 0xCDCE1C, AKB_MSADPCM_LOOP_END = 0xCDCE3C;
constexpr uint32_t AKB_GET_ENABLE_ENCRYPTION = 0xCDCF70;
constexpr uint32_t MSADPCM_INITIALIZE = 0xCE8ECC, MSADPCM_DECODE = 0xCE8A50, MSADPCM_RELEASE = 0xCE8A34;
constexpr uint32_t OV_OPEN_CALLBACKS = 0xCEFBAC, OV_READ = 0xCF1394, OV_CLEAR = 0xCEEDB4;

// log strings
constexpr uint32_t TAG = 0xEECB88;                        // "sqexsdlib"
constexpr uint32_t MSG_ALREADY_INITIALIZED = 0xEED498, MSG_VERSION = 0xEED4E0, MSG_ANALYSE_FAILED = 0xEED4FC;
constexpr uint32_t MSG_CORE_FAILED = 0xEED538, MSG_DELEGATE_FAILED = 0xEED578, MSG_ACTION_FAILED = 0xEED59C;
constexpr uint32_t MSG_SOUND_FAILED = 0xEED5C0, MSG_CATEGORY_FAILED = 0xEED5E0;
constexpr uint32_t MSG_ALREADY_SUSPENDED = 0xEED604, MSG_SUSPEND = 0xEED630, MSG_NOT_SUSPENDED = 0xEED644;
constexpr uint32_t MSG_RESUME = 0xEED680, MSG_MASTER_VOLUME_FAILED = 0xEED690;
constexpr uint32_t MSG_RELEASE_NOT_INITIALIZED = 0xEED6C8, MSG_RELEASE_1 = 0xEED708, MSG_RELEASE_2 = 0xEED720;
constexpr uint32_t MSG_DONE = 0xEED730, MSG_STOP_THREAD = 0xEED738, MSG_STOP_SOUNDS = 0xEED754;
constexpr uint32_t MSG_STOPPING = 0xEED76C, MSG_STOPPED = 0xEED778, MSG_RELEASE_CATEGORY = 0xEED780;
constexpr uint32_t MSG_RELEASE_ACTION = 0xEED79C, MSG_RELEASE_DELEGATE = 0xEED7B8, MSG_RELEASE_SOUND = 0xEED7D4;
constexpr uint32_t MSG_RELEASE_CORE = 0xEED7F0, MSG_RELEASED = 0xEED804;
constexpr uint32_t MSG_THREAD_ACTIVE = 0xEED824, MSG_THREAD_CREATE_FAILED = 0xEED844;
constexpr uint32_t MSG_OV_OPEN_FAILED = 0xEED86C, MSG_REALLOC_FAILED = 0xEED408, MSG_CALLOC_FAILED = 0xEED438;
constexpr uint32_t MSG_MALLOC_FAILED = 0xEED468;

constexpr uint32_t UPDATE_THREAD_ENTRY = 0xCE0E3C;   // sub_CE0E3C

void log(uint32_t prio, uint32_t msg) { rw::call(LOG, {prio, TAG, msg}); }
int32_t result(uint32_t r) { return int32_t(int8_t(r)); }   // sxtb: an SQEX result byte

// Queue ACTION { type, word } built in the original frame (entry sp - frameSize); AddAction copies all 40 bytes.
int32_t addAction(uint32_t frameSize, uint32_t type, uint32_t word) {
    sys::ExactFrame f(frameSize);
    sys::st32(f.addr(), type);
    sys::st32(f.addr(4), word);
    return result(rw::call(ACTION_ADD, {f.addr()}));
}
}  // namespace

int32_t Initialize(uint32_t config) {
    if (sys::ld8(INITIALIZED)) {
        log(5, MSG_ALREADY_INITIALIZED);
        return -1;
    }
    log(4, MSG_VERSION);
    rw::call(MEMORY_INITIALIZE, {});
    rw::call(TIMER_INITIALIZE, {});
    sys::st8(INITIALIZED, 0);
    sys::st8(SUSPENDED, 0);
    sys::st32(SOUND_LIMIT, 0xFFFFFFFFu);
    sys::st8(STOP_THREAD, 0);
    sys::st32(LAST_TIME, 0);
    sys::st32(LAST_TIME + 4, 0);
    sys::st32(sys::ld32(GOT_OGG_MALLOC), 0xCE0570);    // sub_CE0570
    sys::st32(sys::ld32(GOT_OGG_CALLOC), 0xCE052C);    // sub_CE052C
    sys::st32(sys::ld32(GOT_OGG_REALLOC), 0xCE04E8);   // sub_CE04E8
    sys::st32(sys::ld32(GOT_OGG_FREE), MEMORY_FREE_THUNK);
    guest::Frame f(0x30);
    const uint32_t driver = f.addr();   // SQEX::DriverConfig
    rw::call(DRIVER_CONFIG_CTOR, {driver});
    if (int32_t(rw::call(DRIVER_CONFIG_ANALYSE, {driver, config})) < 0) log(5, MSG_ANALYSE_FAILED);
    int32_t r = -1;
    if (int32_t(rw::call(CORE_INITIALIZE, {0x7D00, 2})) < 0) log(5, MSG_CORE_FAILED);
    else if (int32_t(rw::call(DELEGATE_INITIALIZE, {})) < 0) log(5, MSG_DELEGATE_FAILED);
    else if (int32_t(rw::call(ACTION_INITIALIZE, {})) < 0) log(5, MSG_ACTION_FAILED);
    else if (int32_t(rw::call(SOUND_INITIALIZE, {})) < 0) log(5, MSG_SOUND_FAILED);
    else if (int32_t(rw::call(CATEGORY_INITIALIZE, {driver})) < 0) log(5, MSG_CATEGORY_FAILED);
    else {
        uint64_t now = rw::call64(TIMER_GET_TIME, {});
        sys::st8(INITIALIZED, 1);
        sys::st32(LAST_TIME, uint32_t(now));
        sys::st32(LAST_TIME + 4, uint32_t(now >> 32));
        r = 0;
    }
    rw::call(DRIVER_CONFIG_DTOR, {driver});
    return r;
}
RW_FUNCTION(0xCE05B4, "SoundSystem::Initialize", Initialize);

int32_t Suspend() {
    if (sys::ld8(SUSPENDED)) {
        log(4, MSG_ALREADY_SUSPENDED);
        return 0;
    }
    log(4, MSG_SUSPEND);
    if (rw::call(CORE_SUSPEND, {}) != 0) return -1;
    sys::st8(SUSPENDED, 1);
    return addAction(0x30, 1, 0);
}
RW_FUNCTION(0xCE0850, "SoundSystem::Suspend", Suspend);

uint32_t IsSuspended() { return sys::ld8(SUSPENDED); }
RW_FUNCTION(0xCE08F4, "SoundSystem::IsSuspended", IsSuspended);

int32_t Resume(uint32_t fade) {
    if (!sys::ld8(SUSPENDED)) {
        log(4, MSG_NOT_SUSPENDED);
        return 0;
    }
    log(4, MSG_RESUME);
    if (rw::call(CORE_RESUME, {fade}) != 0) return -1;
    sys::st8(SUSPENDED, 0);
    return addAction(0x38, 2, 0);
}
RW_FUNCTION(0xCE0908, "SoundSystem::Resume", Resume);

int32_t SetMasterVolume(uint32_t volume, uint32_t fade) {
    if (int32_t(rw::call(CORE_SET_MASTER_VOLUME, {volume, fade})) >= 0) return 0;
    log(5, MSG_MASTER_VOLUME_FAILED);
    return -1;
}
RW_FUNCTION(0xCE09B4, "SoundSystem::SetMasterVolume", SetMasterVolume);

uint32_t GetMasterVolume() { return rw::call(CORE_GET_MASTER_VOLUME, {}); }   // float bits
RW_FUNCTION(0xCE09F8, "SoundSystem::GetMasterVolume", GetMasterVolume);

int32_t StopAllSounds(uint32_t fade) { return addAction(0x30, 3, fade); }
RW_FUNCTION(0xCE09FC, "SoundSystem::StopAllSounds", StopAllSounds);

// ---- sound creation: SoundManager::CreateSound(sound, data, size, externalId?, externalId, layout)
uint32_t CreateSound(uint32_t sound, uint32_t data, uint32_t size) {
    return rw::call(SOUND_CREATE, {sound, data, size, 0, 0, 0});
}
RW_FUNCTION(0xCE0A20, "SoundSystem::CreateSound", CreateSound);

uint32_t CreateSoundWithExternalID(uint32_t sound, uint32_t data, uint32_t size, uint32_t externalId) {
    return rw::call(SOUND_CREATE, {sound, data, size, 1, externalId, 0});
}
RW_FUNCTION(0xCE0A40, "SoundSystem::CreateSoundWithExternalID", CreateSoundWithExternalID);

uint32_t CreateLayoutSound(uint32_t sound, uint32_t data, uint32_t size) {
    return rw::call(SOUND_CREATE, {sound, data, size, 0, 0, 1});
}
RW_FUNCTION(0xCE0A64, "SoundSystem::CreateLayoutSound", CreateLayoutSound);

uint32_t CreateLayoutSoundWithExternalID(uint32_t sound, uint32_t data, uint32_t size, uint32_t externalId) {
    return rw::call(SOUND_CREATE, {sound, data, size, 1, externalId, 1});
}
RW_FUNCTION(0xCE0A84, "SoundSystem::CreateLayoutSoundWithExternalID", CreateLayoutSoundWithExternalID);

uint32_t CreateStreamingSound(uint32_t sound, uint32_t reader, uint32_t ctx, uint32_t size) {
    return rw::call(SOUND_CREATE_STREAMING, {sound, reader, ctx, size, 0, 0, 0});
}
RW_FUNCTION(0xCE0AA4, "SoundSystem::CreateStreamingSound", CreateStreamingSound);

uint32_t CreateStreamingSoundWithExternalID(uint32_t sound, uint32_t reader, uint32_t ctx, uint32_t size,
                                            int16_t externalId) {
    return rw::call(SOUND_CREATE_STREAMING, {sound, reader, ctx, size, 1, uint32_t(int32_t(externalId)), 0});
}
RW_FUNCTION(0xCE0AC8, "SoundSystem::CreateStreamingSoundWithExternalID", CreateStreamingSoundWithExternalID);

uint32_t CreateStreamingLayoutSound(uint32_t sound, uint32_t reader, uint32_t ctx, uint32_t size) {
    return rw::call(SOUND_CREATE_STREAMING, {sound, reader, ctx, size, 0, 0, 1});
}
RW_FUNCTION(0xCE0AEC, "SoundSystem::CreateStreamingLayoutSound", CreateStreamingLayoutSound);

uint32_t CreateStreamingLayoutSoundWithExternalID(uint32_t sound, uint32_t reader, uint32_t ctx, uint32_t size,
                                                  int16_t externalId) {
    return rw::call(SOUND_CREATE_STREAMING, {sound, reader, ctx, size, 1, uint32_t(int32_t(externalId)), 1});
}
RW_FUNCTION(0xCE0B10, "SoundSystem::CreateStreamingLayoutSoundWithExternalID", CreateStreamingLayoutSoundWithExternalID);

// One tick of every manager with the time since the last tick.
uint32_t Update() {
    uint64_t now = rw::call64(TIMER_GET_TIME, {});
    uint32_t delta = uint32_t(now) - sys::ld32(LAST_TIME);
    sys::st32(LAST_TIME, uint32_t(now));
    sys::st32(LAST_TIME + 4, uint32_t(now >> 32));
    rw::call(ACTION_UPDATE, {delta});
    rw::call(CATEGORY_UPDATE, {sys::fb(float(delta))});
    rw::call(SOUND_UPDATE, {delta});
    return rw::call(CORE_UPDATE, {delta});
}
RW_FUNCTION(0xCE0B38, "SoundSystem::Update", Update);

int32_t Release() {
    if (!sys::ld8(INITIALIZED)) {
        log(5, MSG_RELEASE_NOT_INITIALIZED);
        return -1;
    }
    log(4, MSG_RELEASE_1);
    log(4, MSG_RELEASE_2);
    rw::call(ACTION_CLEAR, {});
    log(4, MSG_DONE);
    uint32_t active = rw::call(THREAD_IS_ACTIVE, {UPDATE_THREAD});
    if (active == 1) {
        log(4, MSG_STOP_THREAD);
        sys::st8(STOP_THREAD, uint8_t(active));
        rw::call(THREAD_JOIN, {UPDATE_THREAD});
        log(4, MSG_DONE);
    }
    log(4, MSG_STOP_SOUNDS);
    while (int32_t(rw::call(SOUND_GET_COUNT, {})) > 0) {
        log(4, MSG_STOPPING);
        rw::call(SOUND_STOP_ALL, {0});
        rw::call(THREAD_SLEEP, {10});
        log(4, MSG_STOPPED);
        Update();
    }
    log(4, MSG_DONE);
    sys::st8(INITIALIZED, 0);
    log(4, MSG_RELEASE_CATEGORY);
    rw::call(CATEGORY_RELEASE, {});
    log(4, MSG_DONE);
    log(4, MSG_RELEASE_ACTION);
    rw::call(ACTION_RELEASE, {});
    log(4, MSG_DONE);
    log(4, MSG_RELEASE_DELEGATE);
    rw::call(DELEGATE_RELEASE, {});
    log(4, MSG_DONE);
    log(4, MSG_RELEASE_SOUND);
    rw::call(SOUND_RELEASE, {});
    log(4, MSG_DONE);
    log(4, MSG_RELEASE_CORE);
    rw::call(CORE_RELEASE, {});
    rw::call(MEMORY_RELEASE, {});
    log(4, MSG_DONE);
    log(4, MSG_RELEASED);
    return 0;
}
RW_FUNCTION(0xCE0B88, "SoundSystem::Release", Release);

// sub_CE0E3C: the update thread: Update every 16 ms until asked to stop.
void updateThread() {
    sys::st8(STOP_THREAD, 0);
    while (!sys::ld8(STOP_THREAD)) {
        Update();
        rw::call(THREAD_SLEEP, {16});
    }
}
RW_FUNCTION(0xCE0E3C, "SoundSystem update thread (sub_CE0E3C)", updateThread);

int32_t CreateUpdateTimer() {
    if (rw::call(THREAD_IS_ACTIVE, {UPDATE_THREAD}) == 1) {
        log(5, MSG_THREAD_ACTIVE);
        return -1;
    }
    guest::Frame f(8);
    sys::st32(f.addr(), UPDATE_THREAD_ENTRY);   // Thread::CreateParam { entry, arg }
    sys::st32(f.addr(4), 0);
    if (rw::call(THREAD_CREATE, {UPDATE_THREAD, f.addr()}) == 0xFFFFFFFFu) {
        log(5, MSG_THREAD_CREATE_FAILED);
        return -1;
    }
    return 0;
}
RW_FUNCTION(0xCE0E7C, "SoundSystem::CreateUpdateTimer", CreateUpdateTimer);

// Bytes ConvertPCM needs for this AKB: the header plus 16-bit PCM of every sample and channel.
int32_t GetRequiredMemorySizeToConvertPCM(const uint32_t *size, uint32_t akb) {
    if (!rw::call(AKB_IS_AKB_FILE, {akb})) return -1;
    uint32_t format = rw::call(AKB_GET_AUDIO_FORMAT, {akb});
    uint32_t header, samples;
    switch (format) {
    case 0: rw::store32(size, rw::call(AKB_GET_HEADER_SIZE, {akb})); return 0;
    case 1: rw::store32(size, rw::call(AKB_GET_FILE_SIZE, {akb})); return 0;
    case 2:
        header = rw::call(AKB_GET_HEADER_SIZE, {akb});
        samples = rw::call(AKB_MSADPCM_NUM_SAMPLES, {akb});
        break;
    case 5:
        header = rw::call(AKB_GET_HEADER_SIZE, {akb});
        samples = rw::call(AKB_GET_NUM_SAMPLES, {akb});
        break;
    default: return -1;
    }
    uint32_t channels = rw::call(AKB_GET_CHANNEL, {akb});
    rw::store32(size, channels * (samples << 1) + header);
    return 0;
}
RW_FUNCTION(0xCE0F34, "SoundSystem::GetRequiredMemorySizeToConvertPCM", GetRequiredMemorySizeToConvertPCM);

// Decode the AKB's MS-ADPCM / Ogg Vorbis audio into 16-bit PCM after its header at dst.
int32_t ConvertPCM(uint32_t dst, uint32_t akb) {
    if (!rw::call(AKB_IS_AKB_FILE, {akb})) return -1;
    guest::Frame f(0x2E8);
    rw::call(MEMCPY, {dst, akb, sys::ld16(akb + 6)});   // the header
    sys::st32(dst + 0x1C, 0);
    sys::st8(dst + 0xC, 1);
    uint32_t format = rw::call(AKB_GET_AUDIO_FORMAT, {akb});
    if (format == 0) {
        sys::st8(dst + 0xC, 0);
        return 0;
    }
    if (format == 1) {
        rw::call(MEMCPY, {dst, akb, rw::call(AKB_GET_FILE_SIZE, {akb})});
        return 0;
    }
    if (format == 5) {
        // Ogg Vorbis: a memory data source { position, data, size } and the game's ov_callbacks
        const uint32_t bitstream = f.addr(0x20), source = f.addr(0x24), vf = f.addr(0x40);
        sys::st32(source, 0);
        uint32_t cb[4];
        for (uint32_t k = 0; k < 4; k++) cb[k] = sys::ld32(OGG_CALLBACKS + 4 * k);
        sys::st32(source + 4, rw::call(AKB_GET_AUDIO_DATA, {akb}));
        sys::st32(source + 8, rw::call(AKB_GET_AUDIO_DATA_SIZE, {akb}));
        if (int32_t(rw::call(OV_OPEN_CALLBACKS, {source, vf, 0, 0, cb[0], cb[1], cb[2], cb[3]})) < 0) {
            log(5, MSG_OV_OPEN_FAILED);
            return -1;
        }
        uint32_t out = dst + rw::call(AKB_GET_HEADER_SIZE, {akb});
        uint32_t samples = rw::call(AKB_GET_NUM_SAMPLES, {akb});
        uint32_t total = rw::call(AKB_GET_CHANNEL, {akb}) * (samples << 1);
        for (uint32_t done = 0; total != 0;) {
            int32_t n = int32_t(rw::call(OV_READ, {vf, out + done, total - done, bitstream}));
            if (n <= 0) {
                rw::call(OV_CLEAR, {vf});
                return n == 0 ? 0 : -1;
            }
            done += uint32_t(n);
            if (total <= done) break;
        }
        rw::call(OV_CLEAR, {vf});
        return 0;
    }
    if (format != 2) return -1;
    // MS-ADPCM
    uint32_t out = dst + rw::call(AKB_GET_HEADER_SIZE, {akb});
    uint32_t samples = rw::call(AKB_MSADPCM_NUM_SAMPLES, {akb});
    uint32_t total = rw::call(AKB_GET_CHANNEL, {akb}) * (samples << 1);
    uint32_t loopStart = 0, loopEnd = 0;
    if (rw::call(AKB_IS_LOOP_AUDIO, {akb})) {
        loopStart = rw::call(AKB_MSADPCM_LOOP_START, {akb});
        loopEnd = rw::call(AKB_MSADPCM_LOOP_END, {akb});
    }
    uint32_t data = rw::call(AKB_GET_AUDIO_DATA, {akb});
    uint32_t dataSize = rw::call(AKB_GET_AUDIO_DATA_SIZE, {akb});
    uint32_t channels = rw::call(AKB_GET_CHANNEL, {akb});
    uint32_t blockAlign = rw::call(AKB_MSADPCM_BLOCK_ALIGN, {akb});
    uint32_t encrypted = rw::call(AKB_GET_ENABLE_ENCRYPTION, {akb}) == 1 ? 1 : 0;
    uint32_t decoder =
        rw::call(MSADPCM_INITIALIZE, {data, dataSize, channels, blockAlign, loopStart, loopEnd, 0, encrypted});
    for (uint32_t done = 0; total != done;) {
        int32_t n = int32_t(rw::call(MSADPCM_DECODE, {decoder, out + done, total - done}));
        if (n <= 0) break;
        done += uint32_t(n);
        if (total <= done) break;
    }
    rw::call(MSADPCM_RELEASE, {decoder});
    return 0;
}
RW_FUNCTION(0xCE0FFC, "SoundSystem::ConvertPCM", ConvertPCM);

int32_t SetSoundLimit(int32_t limit) {
    if (limit < -1 || limit == 0) return -1;
    sys::st32(SOUND_LIMIT, uint32_t(limit));
    return 0;
}
RW_FUNCTION(0xCE12D8, "SoundSystem::SetSoundLimit", SetSoundLimit);

uint32_t GetSoundLimit() { return sys::ld32(SOUND_LIMIT); }
RW_FUNCTION(0xCE1314, "SoundSystem::GetSoundLimit", GetSoundLimit);

uint32_t GetSoundCount() { return rw::call(SOUND_GET_COUNT, {}); }
RW_FUNCTION(0xCE1324, "SoundSystem::GetSoundCount", GetSoundCount);

// ---- the libogg / libvorbis allocator hooks (16-byte aligned SQEX::Memory, a failure is logged)
uint32_t oggRealloc(uint32_t p, uint32_t size) {
    uint32_t r = rw::call(MEMORY_REALLOC, {p, size, 4});
    if (!r) log(5, MSG_REALLOC_FAILED);
    return r;
}
RW_FUNCTION(0xCE04E8, "SoundSystem ogg_realloc hook (sub_CE04E8)", oggRealloc);

uint32_t oggCalloc(uint32_t n, uint32_t size) {
    uint32_t r = rw::call(MEMORY_CALLOC, {n, size, 4});
    if (!r) log(5, MSG_CALLOC_FAILED);
    return r;
}
RW_FUNCTION(0xCE052C, "SoundSystem ogg_calloc hook (sub_CE052C)", oggCalloc);

uint32_t oggMalloc(uint32_t size) {
    uint32_t r = rw::call(MEMORY_MALLOC, {size, 4});
    if (!r) log(5, MSG_MALLOC_FAILED);
    return r;
}
RW_FUNCTION(0xCE0570, "SoundSystem ogg_malloc hook (sub_CE0570)", oggMalloc);

}  // namespace soundsystem
