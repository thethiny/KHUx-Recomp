// Batch #10: SdSoundSystem's streaming banks (AddStreamDataFromPath / FromFile: a StreamingBank in the bank pool,
// the same steps as SdSoundSystem_AddData's on-memory bank) and SdSoundSystem_Terminate.
#include "../w4_b2/SqexPool.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t SD = 0x100E598, SD_POOL = SD + 4, SD_SERIAL = SD + 0x38;
constexpr uint32_t SD_STOP = SD + 0x1C, SD_INITIALIZED = SD + 0x28, SD_THREAD = SD + 0x2C;
constexpr uint32_t BANK_SLOT_VTABLE_GOT = 0xFD9254;
constexpr uint32_t STREAMING_BANK_CTOR = 0xCDC18C;
constexpr uint32_t THREAD_IS_ACTIVE = 0xCE2388, THREAD_JOIN = 0xCE23F4;
constexpr uint32_t RELEASE_KAMCORD_CAPTURE = 0xCDB570;   // SdSoundSystem_ReleaseKamcordAudioCaptureIOS (empty)
constexpr uint32_t SOUND_SYSTEM_RELEASE = 0xCE0B88;
constexpr uint32_t MEMORY_RELEASE = 0xCE4394;
constexpr uint32_t S_CREATE_BANK_FAILED = 0xEECBB0;      // "Create Bank failed!! heap memory insufficient"
constexpr uint32_t S_BANK_LIST_FULL = 0xEECBE0, S_BANK_LIST_APPEND_FAILED = 0xEECBF4;
constexpr uint32_t S_TERMINATE_SUCCESS = 0xEECFA8;       // "SdSoundSystem_Terminate success"

}  // namespace

struct SdSoundSystemStream {
    // a StreamingBank reading `source` (a path, or with fromPath 0 a file name for SQEX::File), registered in the
    // bank pool: its id, or 0 when it could not be created
    static uint32_t addStreamData(uint32_t source, uint32_t fromPath) {
        mutexLock(SD);
        uint32_t result = 0;
        const uint32_t bank = malloc_(0x34, 4);
        rw::call(STREAMING_BANK_CTOR, {bank});
        if (bank == 0) {
            log(5, S_CREATE_BANK_FAILED);
        } else {
            uint32_t node = ld32(SD_POOL + pool::FREE);
            if (node == 0) node = pool::grow(SD_POOL, BANK_SLOT_VTABLE_GOT);
            if (node == 0) {
                log(5, S_BANK_LIST_APPEND_FAILED);
                vcallo(bank, 4, {bank});
            } else {
                const int32_t idx = lds32(node + 0xC);
                pool::take(SD_POOL, node, bank);
                if (idx >= 0x10000) {
                    log(5, S_BANK_LIST_FULL);
                    pool::release(SD_POOL, idx);
                    vcallo(bank, 4, {bank});
                } else {
                    guest::Frame f(0x10);   // INIT_PARAM {id, source, fromPath}
                    *f.at<uint32_t>(8) = source;
                    *f.at<uint8_t>(0xC) = uint8_t(fromPath);
                    const uint32_t serial = pool::nextSerial(SD_SERIAL);
                    *f.at<uint32_t>(4) = (uint32_t(idx) & 0xFFFF) | (serial << 16);
                    st16(SD_SERIAL, uint16_t(serial));
                    if (vcallo(bank, 8, {bank, f.addr(4)}) == 0) {   // Initialize
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
    static uint32_t addStreamDataFromPath(uint32_t path) { return addStreamData(path, 1); }
    static uint32_t addStreamDataFromFile(uint32_t file) { return addStreamData(file, 0); }

    // stops the sound thread, releases the sound system, destroys every bank and the pool, then SQEX's memory
    static void terminate() {
        if (ld32(SD_INITIALIZED) != 0) {
            const uint32_t active = rw::call(THREAD_IS_ACTIVE, {SD_THREAD});
            if (active == 1) {
                st8(SD_STOP, uint8_t(active));
                rw::call(THREAD_JOIN, {SD_THREAD});
            }
            rw::call(RELEASE_KAMCORD_CAPTURE, {});
            rw::call(SOUND_SYSTEM_RELEASE, {});
            for (uint32_t node = ld32(SD_POOL + pool::USED); node != 0;) {
                const uint32_t bank = ld32(node + 0x14);
                if (bank != 0) vcallo(bank, 4, {bank});
                const uint32_t next = ld32(node + 8);
                // a node that is no longer a valid used slot is visited again (as the original does)
                if (pool::release(SD_POOL, lds32(node + 0xC))) node = next;
            }
            if (ld32(SD_POOL + pool::SLOTS) != 0) pool::destroySlots(SD_POOL);
            st32(SD_POOL + pool::SLOTS, 0);
            st32(SD_POOL + pool::FREE, 0);
            st32(SD_POOL + pool::USED, 0);
            st32(SD_POOL + pool::TAIL, 0);
            st32(SD_POOL + pool::COUNT, 0);
            rw::call(MEMORY_RELEASE, {});
            log(4, S_TERMINATE_SUCCESS);
        }
        st32(SD_INITIALIZED, 0);
    }
};
RW_FUNCTION(0xCD98B0, "SdSoundSystem_AddStreamDataFromPath", SdSoundSystemStream::addStreamDataFromPath);
RW_FUNCTION(0xCD9E88, "SdSoundSystem_AddStreamDataFromFile", SdSoundSystemStream::addStreamDataFromFile);
RW_FUNCTION(0xCDB574, "SdSoundSystem_Terminate", SdSoundSystemStream::terminate);

}  // namespace sqex
