// sqexsdlib C API (SdSoundSystem_*), the AKB sound bank file accessors (akb*), the attribute result table, and
// the OpenSL ES output (CoreAudioOut*).
#include "SqexPool.hpp"

using namespace b2;

namespace sqex {

// SdSoundSystem state at 0x100E598: mutex +0, bank pool +4 (see SqexPool.hpp), initialized flag +0x28,
// delegate thread +0x2C, bank serial u16 +0x38.
constexpr uint32_t SD = 0x100E598, SD_POOL = SD + 4, SD_SERIAL = SD + 0x38;
constexpr uint32_t BANK_SLOT_VTABLE_GOT = 0xFD9254;
constexpr uint32_t FIND_BANK = 0xCD8E1C;          // sub_CD8E1C: bank by id (under the mutex)
constexpr uint32_t BANK_IS_FREE = 0xCE963C;       // BankImpl: no sound left playing
constexpr uint32_t BANK_STOP_SOUNDS = 0xCE9950;   // BankImpl::StopBelongSounds
constexpr uint32_t SOUND_FROM_ID = 0xCE1334;      // Sound::Sound(int)

struct SdSoundSystem {
    static int32_t initialize(uint32_t config) {
        if (ld32(SD + 0x28) == 1) return 0;
        log(4, 0xEECB94);              // "sdlibapi api ver 14.11.27.A"
        rw::call(0xCE4378, {});        // SQEX::Memory::Initialize
        pool::initialize(SD_POOL, BANK_SLOT_VTABLE_GOT);
        int32_t r = int32_t(rw::call(0xCE05B4, {config}));  // SoundSystem::Initialize
        if (r < 0) {
            st32(SD + 0x28, 0);
            return -1;
        }
        guest::Frame f(8);
        *f.at<uint32_t>(0) = 0xCD8EAC;  // thread entry
        *f.at<uint32_t>(4) = 0;
        rw::call(0xCE2398, {SD + 0x2C, f.addr()});  // Thread::Create
        st32(SD + 0x28, 1);
        return r != 0 ? -1 : 0;
    }

    static uint32_t addData(uint32_t data) {
        mutexLock(SD);
        uint32_t result = 0;
        uint32_t bank = malloc_(0x1C, 4);
        rw::call(0xCE9D00, {bank});  // OnMemoryBank::OnMemoryBank
        if (bank == 0) {
            log(5, 0xEECBB0);  // "Create Bank failed!! heap memory insufficient"
        } else {
            uint32_t node = ld32(SD_POOL + pool::FREE);
            if (node == 0) node = pool::grow(SD_POOL, BANK_SLOT_VTABLE_GOT);
            if (node == 0) {
                log(5, 0xEECBF4);  // "bank list append failed"
                vcallo(bank, 4, {bank});
            } else {
                int32_t idx = lds32(node + 0xC);
                pool::take(SD_POOL, node, bank);
                if (idx >= 0x10000) {
                    log(5, 0xEECBE0);  // "bank list full!!"
                    pool::release(SD_POOL, idx);
                    vcallo(bank, 4, {bank});
                } else {
                    guest::Frame f(0x10);
                    uint32_t serial = pool::nextSerial(SD_SERIAL);
                    *f.at<uint32_t>(4) = (uint32_t(idx) & 0xFFFF) | (serial << 16);  // INIT_PARAM: id, data
                    *f.at<uint32_t>(8) = data;
                    st16(SD_SERIAL, uint16_t(serial));
                    if (vcallo(bank, 8, {bank, f.addr(4)}) == 0) {  // Initialize
                        result = ld32(bank + 4);
                    } else {
                        pool::release(SD_POOL, idx);
                        vcallo(bank, 4, {bank});
                    }
                }
            }
        }
        mutexUnlock(SD);
        return result;
    }

    // removes bank `id` once none of its sounds plays (waiting: stops them and polls)
    static int32_t removeDataSync(uint32_t id, uint32_t wait) {
        mutexLock(SD);
        uint32_t bank = rw::call(FIND_BANK, {id});
        if (bank == 0) {
            mutexUnlock(SD);
            return 0;
        }
        st8(bank + 0x14, 1);
        auto drop = [&]() {
            pool::release(SD_POOL, int32_t(ld16(bank + 4)));
            vcallo(bank, 4, {bank});
        };
        if (wait == 0) {
            int32_t r = -1;
            if (rw::call(BANK_IS_FREE, {bank}) == 1) {
                drop();
                r = 0;
            }
            mutexUnlock(SD);
            return r;
        }
        mutexUnlock(SD);
        int64_t start = int64_t(rw::call64(0xCE2428, {}));  // Timer::GetTime
        for (;;) {
            uint32_t freeNow = rw::call(BANK_IS_FREE, {bank});
            if (freeNow != 0) break;
            mutexLock(SD);
            rw::call(BANK_STOP_SOUNDS, {bank, freeNow});
            mutexUnlock(SD);
            rw::call(0xCE2364, {1});  // Thread::Sleep(1)
            int64_t now = int64_t(rw::call64(0xCE2428, {}));
            if (int64_t(uint64_t(now) - uint64_t(start)) > 5000) rw::call(0xCE99C4, {bank});  // PrintDebugInfo
        }
        mutexLock(SD);
        drop();
        mutexUnlock(SD);
        return 0;
    }

    static uint32_t bankIsUsing(uint32_t id) {
        mutexLock(SD);
        uint32_t bank = rw::call(FIND_BANK, {id});
        uint32_t r = 0;
        if (bank != 0) {
            uint32_t v = rw::call(BANK_IS_FREE, {bank});
            r = v <= 1 ? 1 - v : 0;
        }
        mutexUnlock(SD);
        return r;
    }
    static uint32_t bankStopSounds(uint32_t id, int32_t fadeMs) {
        mutexLock(SD);
        uint32_t bank = rw::call(FIND_BANK, {id});
        if (bank != 0) {
            if (fadeMs < 0) {
                log(5, 0xEECC0C, uint32_t(fadeMs));
                fadeMs = 0;
            }
            rw::call(BANK_STOP_SOUNDS, {bank, uint32_t(fadeMs)});
        }
        return mutexUnlock(SD);
    }
    static int32_t createSound(uint32_t bankId, uint32_t index) {
        mutexLock(SD);
        int32_t r;
        uint32_t bank = rw::call(FIND_BANK, {bankId});
        if (bank == 0) {
            log(5, 0xEECC5C, bankId);
            r = -1;
        } else if (ld8(bank + 0x14) != 0) {  // being removed
            log(5, 0xEECC90, bankId);
            r = -1;
        } else {
            r = int32_t(rw::call(0xCE9650, {bank, index}));  // BankImpl::CreateSound
        }
        mutexUnlock(SD);
        return r;
    }
    static int32_t soundStartFadeIn(uint32_t handle, int32_t fadeMs, uint32_t volume, uint32_t target,
                                    uint32_t seek) {
        uint32_t fade;
        if (fadeMs < 0) {
            log(5, 0xEECD14, uint32_t(fadeMs));
            fade = 0;
        } else {
            fade = uint32_t(fadeMs);
        }
        guest::Frame f(8);
        rw::call(SOUND_FROM_ID, {f.addr(4), handle});
        rw::call(0xCE13E0, {f.addr(4), volume, 0});     // Sound::SetVolume(volume, 0)
        rw::call(0xCE13E0, {f.addr(4), target, fade});  // Sound::SetVolume(target, fade)
        return rw::call(0xCE133C, {f.addr(4), 0, seek}) != 0 ? -1 : 0;  // Sound::Play
    }
    static uint32_t soundStop(uint32_t handle, int32_t fadeMs) {
        uint32_t fade = uint32_t(fadeMs);
        if (fadeMs < 0) {
            log(5, 0xEECD68, uint32_t(fadeMs));
            fade = 0;
        }
        guest::Frame f(0xC);
        rw::call(SOUND_FROM_ID, {f.addr(4), handle});
        return rw::call(0xCE1368, {f.addr(4), fade});  // Sound::Stop
    }
    static uint32_t soundIsExist(uint32_t handle) {
        guest::Frame f(8);
        rw::call(SOUND_FROM_ID, {f.addr(4), handle});
        return rw::call(0xCE14FC, {f.addr(4)}) != 0 ? 1 : 0;  // Sound::GetStatus
    }
};
RW_FUNCTION(0xCD8FF0, "SdSoundSystem_Initialize", SdSoundSystem::initialize);

// the sound thread (stop flag SD + 0x1C, last frame time SD + 0x20) and the bank lookup
struct SdThread {
    // sub_CD8E1C(id): the bank with this id (slot id & 0xFFFF of the pool, its id matching), or null
    static uint32_t findBank(uint32_t id) {
        mutexLock(SD);
        uint32_t bank = 0;
        const int32_t slot = int32_t(id & 0xFFFF);
        const uint32_t slots = ld32(SD + 0xC);
        if (slot < lds32(SD + 4) && slots != 0) {
            const uint32_t entry = slots + uint32_t(slot) * 24;
            if (entry != 0 && (ld8(entry + 0x10) & 1)) {
                const uint32_t b = ld32(entry + 0x14);
                if (id == ld32(b + 4)) bank = b;
            }
        }
        mutexUnlock(SD);
        return bank;
    }

    // sub_CD8EAC: every frame update each used bank (vtable +0x14) and the sound system, then sleep what is left of
    // 16 ms
    static void run(uint32_t /*argument*/) {
        st8(SD + 0x1C, 0);
        while (!ld8(SD + 0x1C)) {
            const uint64_t start = rw::call64(0xCE247C, {});  // SQEX::Timer::GetUTime
            mutexLock(SD);
            for (uint32_t node = ld32(SD + 0x14); node != 0; node = ld32(node + 8)) {
                const uint32_t bank = ld32(node + 0x14);
                vcallo(bank, 0x14, {bank});
                if (!(ld8(node + 0x10) & 1)) break;
            }
            mutexUnlock(SD);
            rw::call(0xCE0B38, {});  // SoundSystem::Update
            const uint64_t spent = rw::call64(0xCE247C, {}) - start;
            st32(SD + 0x20, uint32_t(spent));
            st32(SD + 0x24, uint32_t(spent >> 32));
            const int32_t sleep = int32_t(int64_t(spent) / -1000) + 16;  // __aeabi_ldivmod (truncating)
            if (sleep > 0) rw::call(0xCE2364, {uint32_t(sleep)});      // SQEX::Thread::Sleep
        }
    }
};
RW_FUNCTION(0xCD8E1C, "SdSoundSystem (sub_CD8E1C): the bank of an id", SdThread::findBank);
RW_FUNCTION(0xCD8EAC, "SdSoundSystem (sub_CD8EAC): the sound thread", SdThread::run);
RW_FUNCTION(0xCD92E0, "SdSoundSystem_AddData", SdSoundSystem::addData);
RW_FUNCTION(0xCDA460, "SdSoundSystem_RemoveDataSync", SdSoundSystem::removeDataSync);
RW_FUNCTION(0xCDA834, "SdSoundSystem_BankCtrl_IsUsing", SdSoundSystem::bankIsUsing);
RW_FUNCTION(0xCDA880, "SdSoundSystem_BankCtrl_StopSounds", SdSoundSystem::bankStopSounds);
RW_FUNCTION(0xCDA94C, "SdSoundSystem_CreateSound", SdSoundSystem::createSound);
RW_FUNCTION(0xCDACAC, "SdSoundSystem_SoundCtrl_StartFadeIn", SdSoundSystem::soundStartFadeIn);
RW_FUNCTION(0xCDAD4C, "SdSoundSystem_SoundCtrl_Stop", SdSoundSystem::soundStop);
RW_FUNCTION(0xCDADB0, "SdSoundSystem_SoundCtrl_IsExist", SdSoundSystem::soundIsExist);

// ============================================================================================ AKB files
// AKB1 ("AKB ", version u16 +4, header size u16 +6, file size +8) and AKB2 (sub_CDC360) layouts.
struct Akb {
    static uint32_t isAkb2(uint32_t p) { return rw::call(0xCDC360, {p}); }
    static uint32_t isAkb1(uint32_t p) {
        if (ld32(p) != 0x20424B41) return 0;  // "AKB " (little endian)
        uint32_t ver = ld16(p + 4);
        if (ver > 3) return 0;
        return ld16(p + 6) == ld16(0xF46A90 + ver * 2) ? 1 : 0;  // the header size of that version
    }
    static uint32_t getVersion(uint32_t p) {
        if (isAkb1(p) == 1) return ld16(p + 4);
        return isAkb2(p) == 1 ? ld8(p + 4) : 0xFFFF;
    }
    static uint32_t getHeaderSize(uint32_t p) {
        if (isAkb1(p) == 1 || isAkb2(p) == 1) return ld16(p + 6);
        return 0xFFFF;
    }
    // sub_CDC460: AKB1 - its header size (-1 for version 0); AKB2 - the offset after the header and its n entries
    static uint32_t getDataOffset(uint32_t p) {
        if (isAkb1(p) == 1) {
            if (getVersion(p) == 0) return 0xFFFFFFFFu;
            return getHeaderSize(p);
        }
        const uint32_t n = ld8(p + 0xF);
        if (n == 0) return ld16(p + 2);
        const uint32_t header = ld16(p + 2);
        const uint32_t table = isAkb1(p) == 1 ? 0 : p + header;
        return ld16(table + 2) * n + header;
    }
    static uint32_t getFileSize(uint32_t p) {
        if (isAkb1(p) == 1 || isAkb2(p) == 1) return ld32(p + 8);
        return 0;
    }
    static uint32_t getSoundData(uint32_t p, uint32_t index) {
        if (isAkb1(p) == 1) return p;
        if (isAkb2(p) != 1) return 0;
        return p + rw::call(0xCDC534, {p, index});
    }
    static uint32_t soundGetAttributeData(uint32_t s) {
        if (isAkb1(s) == 1) {
            uint32_t v = getVersion(s);
            if (v == 0) return 0;
        }
        return s + rw::call(0xCDC460, {s});
    }
    static uint32_t soundGetSoundID(uint32_t s) {
        if (isAkb1(s) != 1) return ld32(s + 8);
        return getVersion(s) != 0 ? ld32(s + 0x24) : 0xFFFFFFFF;
    }
    static uint32_t soundGetVolume(uint32_t s) {  // float bits
        if (isAkb1(s) != 1) return ld32(s + 4);
        return getVersion(s) != 0 ? ld32(s + 0x20) : 0x3F800000;
    }
    static uint32_t soundGetMaterialDataOffset(uint32_t s, uint32_t index) {
        if (isAkb1(s) == 1) return 0;
        uint32_t table = isAkb1(s) == 1 ? 0 : s + ld16(s + 2);
        return ld32(table + index * 16 + 4);
    }
    static uint32_t materialGetAudioDataOffset(uint32_t m) {
        if (isAkb1(m) != 1) return ld16(m + 4) + ld32(m + 0x18);
        uint32_t base = getHeaderSize(m);
        uint32_t extra;
        if (isAkb1(m) == 1) {
            uint32_t v = getVersion(m);
            extra = v != 0 ? ld16(m + 0x28) : v;
        } else {
            extra = ld16(m + 0xC);
        }
        base = extra + base;
        return base + (isAkb1(m) == 1 ? ld32(m + 0x1C) : ld32(m + 0x18));
    }
    static uint32_t materialIsLoopAudio(uint32_t m) {
        if (isAkb1(m) == 1) return ld32(m + 0x18) > ld32(m + 0x14) ? 1 : 0;
        return ld32(m + 0x14) > ld32(m + 0x10) ? 1 : 0;
    }
    static uint32_t materialGetAudioDataSize(uint32_t m) {
        if (isAkb1(m) != 1) return ld32(m + 8);
        uint32_t size = getFileSize(m);
        return size - materialGetAudioDataOffset(m);
    }
    static uint32_t materialFlag(uint32_t m, uint32_t bit, uint32_t minVersion) {
        if (isAkb1(m) != 1) return (ld8(m + 3) >> bit) & 1;
        return getVersion(m) > minVersion ? (ld8(m + 0x2B) >> bit) & 1 : 0;
    }
    static uint32_t materialGetEnableRandomVolume(uint32_t m) { return materialFlag(m, 0, 1); }
    static uint32_t materialGetEnableRandomPitch(uint32_t m) { return materialFlag(m, 1, 1); }
    static uint32_t materialGetEnableEncryption(uint32_t m) {
        if (isAkb1(m) == 1) return getVersion(m) > 2 ? (ld8(m + 0x2B) >> 3) & 1 : 0;
        if (rw::call(0xCDC888, {m}) == 0) return 0;
        return (ld8(m + 3) >> 3) & 1;
    }
    // attribute data: result command `n` (1: at +0x10, 2..5: 24-byte entries from +0x28)
    static uint32_t attrGetResultCommand(uint32_t attr, uint32_t n) {
        if (n - 1 > 4) return 0;
        if (n == 1) return attr + 0x10;
        return attr + (n - 2) * 24 + 0x28;
    }
};
RW_FUNCTION(0xCDC2E0, "akbIsAkb1File", Akb::isAkb1);
RW_FUNCTION(0xCDC3F8, "akbGetVersion", Akb::getVersion);
RW_FUNCTION(0xCDC42C, "akbGetHeaderSize", Akb::getHeaderSize);
RW_FUNCTION(0xCDC460, "akb (sub_CDC460): the data offset", Akb::getDataOffset);
RW_FUNCTION(0xCDC4D4, "akbGetFileSize", Akb::getFileSize);
RW_FUNCTION(0xCDC584, "akbGetSoundData", Akb::getSoundData);
RW_FUNCTION(0xCDC61C, "akbSoundGetAttributeData", Akb::soundGetAttributeData);
RW_FUNCTION(0xCDC65C, "akbSoundGetSoundID", Akb::soundGetSoundID);
RW_FUNCTION(0xCDC698, "akbSoundGetVolume", Akb::soundGetVolume);
RW_FUNCTION(0xCDC7A4, "akbSoundGetMaterialDataOffset", Akb::soundGetMaterialDataOffset);
RW_FUNCTION(0xCDC8C0, "akbMaterialGetAudioDataOffset", Akb::materialGetAudioDataOffset);
RW_FUNCTION(0xCDCA04, "akbMaterialIsLoopAudio", Akb::materialIsLoopAudio);
RW_FUNCTION(0xCDCBA8, "akbMaterialGetAudioDataSize", Akb::materialGetAudioDataSize);
RW_FUNCTION(0xCDCBE0, "akbMaterialGetEnableRandomVolume", Akb::materialGetEnableRandomVolume);
RW_FUNCTION(0xCDCC24, "akbMaterialGetEnableRandomPitch", Akb::materialGetEnableRandomPitch);
RW_FUNCTION(0xCDCF70, "akbMaterialGetEnableEncryption", Akb::materialGetEnableEncryption);
RW_FUNCTION(0xCDD00C, "attrGetResultCommand", Akb::attrGetResultCommand);

// ============================================================================================ OpenSL ES output
// state at 0x100E768: render callback +0, engine object +4, output mix +8, audio player +0xC
constexpr uint32_t OUT = 0x100E768;
constexpr uint32_t IID_ENGINE_GOT = 0xFD9354, IID_BUFFERQUEUE_GOT = 0xFD9358, IID_PLAY_GOT = 0xFD9360;
constexpr uint32_t KICK_BUFFER_GOT = 0xFD935C;

// (*itf)->method(itf, ...): OpenSL interfaces are pointers to a table of function pointers
static uint32_t slcall(uint32_t itf, uint32_t slot, std::initializer_list<uint32_t> args) {
    return rw::call(ld32(ld32(itf) + slot * 4), args);
}

struct CoreAudioOut {
    static int32_t init(uint32_t sampleRate, uint32_t channels, uint32_t callback) {
        guest::Frame f(0x64);
        if (rw::call(0x44EAF0, {OUT + 4, 0, 0, 0, 0, 0}) != 0) {  // slCreateEngine
            log(5, 0xEEDE48);
            return -1;
        }
        uint32_t engine = ld32(OUT + 4);
        if (slcall(engine, 0, {engine, 0}) != 0) {  // Realize
            log(5, 0xEEDE74);
            return -1;
        }
        engine = ld32(OUT + 4);
        if (slcall(engine, 3, {engine, ld32(got(IID_ENGINE_GOT)), f.addr(0x14)}) != 0) {  // GetInterface
            log(5, 0xEEDEA0);
            return -1;
        }
        uint32_t engineItf = *f.at<uint32_t>(0x14);
        if (slcall(engineItf, 7, {engineItf, OUT + 8, 0, 0, 0}) != 0) {  // CreateOutputMix
            log(5, 0xEEDED0);
            return -1;
        }
        uint32_t mix = ld32(OUT + 8);
        if (slcall(mix, 0, {mix, 0}) != 0) {  // Realize
            log(5, 0xEEDEFC);
            return -1;
        }
        // data source: buffer queue locator + PCM format; data sink: the output mix
        *f.at<uint32_t>(0x58) = 0;
        *f.at<uint32_t>(0x24) = ld32(0xF46A9C);
        *f.at<uint32_t>(0x28) = ld32(0xF46AA0);
        *f.at<uint32_t>(0x2C) = f.addr(0x24);
        *f.at<uint32_t>(0x48) = channels;
        *f.at<uint32_t>(0x30) = f.addr(0x44);
        *f.at<uint32_t>(0x4C) = sampleRate * 1000;
        *f.at<uint32_t>(0x44) = 2;   // SL_DATAFORMAT_PCM
        *f.at<uint32_t>(0x5C) = 2;   // little endian
        *f.at<uint32_t>(0x54) = 16;
        *f.at<uint32_t>(0x50) = 16;
        if (channels == 1) *f.at<uint32_t>(0x58) = 4;
        else if (channels == 2) *f.at<uint32_t>(0x58) = 3;
        uint32_t iidBufferQueue = ld32(got(IID_BUFFERQUEUE_GOT));
        *f.at<uint32_t>(0x40) = 0;
        *f.at<uint32_t>(0x3C) = f.addr(0x34);
        *f.at<uint32_t>(0x18) = iidBufferQueue;
        *f.at<uint32_t>(0x38) = ld32(OUT + 8);
        *f.at<uint32_t>(0x1C) = 1;
        *f.at<uint32_t>(0x34) = 4;   // SL_DATALOCATOR_OUTPUTMIX
        uint32_t r = slcall(engineItf, 2, {engineItf, OUT + 0xC, f.addr(0x2C), f.addr(0x3C), 1, f.addr(0x18),
                                           f.addr(0x1C)});  // CreateAudioPlayer
        if (r != 0) {
            log(5, 0xEEDF28, r);
            return -1;
        }
        uint32_t player = ld32(OUT + 0xC);
        r = slcall(player, 0, {player, 0});  // Realize
        if (r != 0) {
            log(5, 0xEEDF64, r);
            return -1;
        }
        player = ld32(OUT + 0xC);
        *f.at<uint32_t>(0x20) = 0;
        r = slcall(player, 3, {player, iidBufferQueue, f.addr(0x20)});  // GetInterface
        if (r != 0) {
            log(5, 0xEEDF98, r);
            return -1;
        }
        uint32_t bq = *f.at<uint32_t>(0x20);
        r = slcall(bq, 3, {bq, 0xCE80B8, 0});  // RegisterCallback
        if (r != 0) {
            log(5, 0xEEDFC8, r);
            return -1;
        }
        st32(OUT, callback);
        rw::call(0x442310, {got(KICK_BUFFER_GOT), 0, 0x400});  // memset(kickBuffer_)
        return 0;
    }
    static int32_t queueBuffer(uint32_t buffer, uint32_t size) {
        guest::Frame f(0xC);
        uint32_t player = ld32(OUT + 0xC);
        *f.at<uint32_t>(4) = 0;
        uint32_t r = slcall(player, 3, {player, ld32(got(IID_BUFFERQUEUE_GOT)), f.addr(4)});
        if (r != 0) {
            log(5, 0xEEE008, r);
            return -1;
        }
        uint32_t bq = *f.at<uint32_t>(4);
        r = slcall(bq, 0, {bq, buffer, size});  // Enqueue
        if (r != 0) {
            log(5, 0xEEE040, r);
            return -1;
        }
        return 0;
    }
    static int32_t kick() {
        uint32_t kickBuffer = got(KICK_BUFFER_GOT);
        queueBuffer(kickBuffer, 0x400);
        queueBuffer(kickBuffer, 0x400);
        guest::Frame f(8);
        uint32_t player = ld32(OUT + 0xC);
        if (slcall(player, 3, {player, ld32(got(IID_PLAY_GOT)), f.addr(4)}) != 0) {
            log(5, 0xEEE07C);  // "[CoreAudioOutKick] get play itf failed"
            return -1;
        }
        uint32_t play = *f.at<uint32_t>(4);
        if (slcall(play, 0, {play, 3}) != 0) {  // SetPlayState(PLAYING)
            log(5, 0xEEE0A4);
            return -1;
        }
        return 0;
    }
};
RW_FUNCTION(0xCE8260, "CoreAudioOutInit", CoreAudioOut::init);
RW_FUNCTION(0xCE86A4, "CoreAudioOutQueueBuffer", CoreAudioOut::queueBuffer);
RW_FUNCTION(0xCE8778, "CoreAudioOutKick", CoreAudioOut::kick);
// sub_CE80B8: the buffer queue callback - the render callback (OUT + 0) with the same arguments, when set
static void coreAudioOutCallback(uint32_t a0, uint32_t a1, uint32_t a2) {
    if (const uint32_t fn = ld32(OUT)) rw::call(fn, {a0, a1, a2, fn});
}
RW_FUNCTION(0xCE80B8, "CoreAudioOut (sub_CE80B8): the buffer queue callback", coreAudioOutCallback);

}  // namespace sqex
