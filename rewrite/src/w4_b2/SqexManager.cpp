// SQEX sound driver: the sound pool (SoundManager), threads, timers, voices.
#include "SqexPool.hpp"

using namespace b2;

namespace sqex {

constexpr uint32_t SOUNDIMPL_GET_STATE = 0xCDD970;  // SQEX::SoundImpl::GetState
constexpr uint32_t SOUNDIMPL_DESTROY = 0xCDDAC0;    // SQEX::SoundImpl::Destroy

// ============================================================================================ SoundManager
struct SoundManager {
    static constexpr uint32_t G = SOUND_MANAGER, SLOT_VTABLE_GOT = 0xFD9290;  // List<Sound>::Node

    static uint32_t getSound(int32_t id) {
        SoundManagerState &S = soundManager();
        uint32_t slot = uint32_t(id) & 0xFFFF;
        if (int32_t(slot) >= S.capacity) return 0;
        uint32_t slots = S.slots;
        if (slots == 0) return 0;
        uint32_t node = slots + slot * 24;
        if (node == 0) return 0;
        uint32_t used = ld8(node + 0x10) & 1;
        if (used == 0) return 0;
        uint32_t sound = ld32(node + 0x14);
        return uint32_t(id) == ld32(sound + 8) ? sound : 0;
    }

    static uint32_t getSoundState(int32_t id) {
        mutexLock(SOUND_MUTEX);
        uint32_t s = getSound(id);
        if (s != 0) s = rw::call(SOUNDIMPL_GET_STATE, {s});
        mutexUnlock(SOUND_MUTEX);
        return s;
    }

    static int32_t initialize() {
        pool::initialize(G, SLOT_VTABLE_GOT);
        return 0;
    }

    static int32_t createSound(const uint32_t *out, uint32_t data, int32_t index, uint32_t hasExternalId,
                               int16_t externalId, uint8_t layout) {
        mutexLock(SOUND_MUTEX);
        int32_t result = -1;
        uint32_t sound = malloc_(0x100, 4);
        rw::call(0xCF456C, {sound});  // OnMemorySound::OnMemorySound
        auto fail = [&](uint32_t fmt) {
            rw::call(SOUNDIMPL_DESTROY, {sound});
            vcallo(sound, 4, {sound});  // deleting destructor
            if (fmt) log(5, fmt);
        };
        if (sound == 0) {
            log(5, 0xEED30C);  // "create OnMemorySound failed!! heap memory insufficient"
        } else {
            uint32_t node = ld32(G + pool::FREE);
            if (node == 0) node = pool::grow(G, SLOT_VTABLE_GOT);
            if (node == 0) {
                fail(0xEED394);  // "sound list append failed"
            } else {
                int32_t idx = lds32(node + 0xC);
                pool::take(G, node, sound);
                if (idx >= 0x10000) {
                    log(5, 0xEED344, node);  // "sound list full!!"
                    pool::release(G, idx);
                    fail(0);
                } else {
                    uint32_t serial = pool::nextSerial(G + 0x1C);
                    uint32_t id = (uint32_t(idx) & 0xFFFF) | (serial << 16);
                    uint32_t init = ld32(ld32(sound) + 0x1C);
                    st16(G + 0x1C, uint16_t(serial));
                    uint32_t soundData = rw::call(0xCDC584, {data, uint32_t(index)});  // akbGetSoundData
                    int32_t r = int32_t(rw::call(init, {sound, id, soundData}));
                    if (hasExternalId != 0) rw::call(0xCDDAA8, {sound, uint32_t(int32_t(externalId))});
                    st8(sound + 0xD0, layout);
                    if (r < 0) {
                        pool::release(G, idx);
                        fail(0xEED358);  // "SoundManager::CreateSound OnMemorySound initialize failed"
                    } else {
                        guest::Frame f(8);
                        rw::call(0xCE1334, {f.addr(), ld32(sound + 8)});  // Sound::Sound(int id)
                        rw::save<uint32_t>(rw::guest_addr(out), ld32(f.addr()));
                        result = 0;
                    }
                }
            }
        }
        mutexUnlock(SOUND_MUTEX);
        return result;
    }

    static uint32_t update(uint32_t dt) {
        mutexLock(SOUND_MUTEX);
        uint32_t node = ld32(G + pool::USED);
        mutexUnlock(SOUND_MUTEX);
        auto nextUsed = [&]() {
            mutexLock(SOUND_MUTEX);
            uint32_t f = ld8(node + 0x10) & 1;
            node = f ? ld32(node + 8) : 0;
            mutexUnlock(SOUND_MUTEX);
        };
        for (;;) {
            mutexLock(SOUND_MUTEX);
            if (node == 0) return mutexUnlock(SOUND_MUTEX);
            mutexUnlock(SOUND_MUTEX);
            uint32_t s = ld32(node + 0x14);
            vcallo(s, 0x18, {s, dt});  // Update
            if (rw::call(SOUNDIMPL_GET_STATE, {ld32(node + 0x14)}) != 3) {
                nextUsed();
                continue;
            }
            s = ld32(node + 0x14);
            if (vcallo(s, 8, {s}) == 1) {
                uint32_t d = ld32(node + 0x14);
                if (d != 0) d += 0xFC;
                if (rw::call(0xCF4328, {d}) == 1) {  // DelegateManager::IsExist
                    nextUsed();
                    continue;
                }
            }
            if (rw::call(SOUNDIMPL_DESTROY, {ld32(node + 0x14)}) != 0) {
                nextUsed();
                continue;
            }
            mutexLock(SOUND_MUTEX);
            s = ld32(node + 0x14);
            if (s != 0) vcallo(s, 4, {s});  // deleting destructor
            int32_t idx = lds32(node + 0xC);
            uint32_t next = ld32(node + 8);
            if (pool::release(G, idx)) node = next;
            mutexUnlock(SOUND_MUTEX);
        }
    }
};
RW_FUNCTION(0xCDEDF8, "SQEX::SoundManager::GetSound", SoundManager::getSound);
RW_FUNCTION(0xCDEE70, "SQEX::SoundManager::GetSoundState", SoundManager::getSoundState);
RW_FUNCTION(0xCDF0AC, "SQEX::SoundManager::Initialize", SoundManager::initialize);
RW_FUNCTION(0xCDF37C, "SQEX::SoundManager::CreateSound", SoundManager::createSound);
RW_FUNCTION(0xCE0170, "SQEX::SoundManager::Update", SoundManager::update);

// ============================================================================================ Thread / Timer
struct ThreadFn {
    // CreateParam: entry function, argument. The thread runs SQEX::Thread::_Run(this).
    static int32_t create(const uint32_t *self, const uint32_t *param) {
        uint32_t entry = param[0];
        if (entry == 0) return -1;
        uint32_t t = rw::guest_addr(self);
        st32(t, entry);
        st32(t + 4, param[1]);
        if (rw::call(0x410900, {t + 8, 0, got(0xFD92B0), t}) != 0) {  // pthread_create(&tid, 0, _Run, this)
            st32(t + 8, 0);
            return -1;
        }
        return 0;
    }
};
RW_FUNCTION(0xCE2398, "SQEX::Thread::Create", ThreadFn::create);

struct Timer {
    // milliseconds since the epoch (32-bit, sign-extended)
    static int64_t getTime() {
        guest::Frame f(0x18);
        if (rw::call(0x43DBE8, {f.addr(0), f.addr(8)}) != 0) return 0;  // gettimeofday
        int32_t sec = *f.at<int32_t>(0), usec = *f.at<int32_t>(4);
        return int64_t(int32_t(uint32_t(sec) * 1000u + uint32_t(usec / 1000)));
    }
    // microseconds (32-bit, sign-extended)
    static int64_t getUTime() {
        guest::Frame f(0x18);
        if (rw::call(0x43DBE8, {f.addr(0), f.addr(8)}) != 0) return 0;
        int32_t sec = *f.at<int32_t>(0), usec = *f.at<int32_t>(4);
        return int64_t(int32_t(uint32_t(sec) * 1000000u + uint32_t(usec)));
    }
};
RW_FUNCTION(0xCE2428, "SQEX::Timer::GetTime", Timer::getTime);
RW_FUNCTION(0xCE247C, "SQEX::Timer::GetUTime", Timer::getUTime);

// ============================================================================================ Voice
// SQEX::Voice: vtable +0, sound +4, material +8, callback vtable +0xC, core source +0x10, transfer +0x14,
// finished byte +0x18 (28 bytes)
struct Voice {
    static uint32_t construct(uint32_t self, uint32_t sound, uint32_t material) {
        st32(self + 4, sound);
        st32(self + 8, material);
        uint32_t vt = got(0xFD92B4);
        st32(self + 0x10, 0);
        st32(self + 0x14, 0);
        st32(self, vt + 8);
        st32(self + 0xC, vt + 0x44);
        st8(self + 0x18, 0);
        return self;
    }
    static uint32_t destruct(uint32_t self) {
        uint32_t source = ld32(self + 0x10);
        uint32_t vt = got(0xFD92B4);
        st32(self, vt + 8);
        st32(self + 0xC, vt + 0x44);
        if (source != 0) {
            vcallo(source, 0x14, {source});  // ICoreSource::Destroy
            st32(self + 0x10, 0);
        }
        uint32_t transfer = ld32(self + 0x14);
        if (transfer != 0) {
            vcallo(transfer, 4, {transfer});  // deleting destructor
            st32(self + 0x14, 0);
        }
        st32(self, got(0xFD9250) + 8);  // SQEX::Object vtable
        return self;
    }
    static uint32_t stop(uint32_t self) {
        st8(self + 0x18, 1);
        uint32_t source = ld32(self + 0x10);
        return vcallo(source, 0x10, {source});
    }
    static int32_t create(uint32_t self, uint32_t streaming, uint32_t seek) {
        uint32_t material = ld32(self + 8);
        int32_t format = int32_t(rw::call(0xCDC8A4, {material}));  // akbMaterialGetAudioFormat
        uint32_t channels = rw::call(0xCDC9CC, {material});        // akbMaterialGetChannel
        uint32_t rate = rw::call(0xCDC9E8, {material});            // akbMaterialGetSampleRate
        auto make = [&](uint32_t size, uint32_t ctor, uint32_t arg) {
            uint32_t t = malloc_(size, 4);
            rw::call(ctor, {t, arg});
            st32(self + 0x14, t);
            return t;
        };
        uint32_t transfer;
        bool stream = (streaming & 0xFF) != 0;
        if (format == 1) transfer = stream ? make(0x1C, 0xCE36B8, self) : make(0x10, 0xCE3534, material);  // Pcm
        else if (format == 2) transfer = stream ? make(0x1C, 0xCE38C4, self) : make(0x20, 0xCE2D10, material);  // MSAdpcm
        else if (format == 5) transfer = stream ? make(0x2C0, 0xCE42A4, self) : make(0x2B8, 0xCE3214, material);  // Vorbis
        else {
            log(5, 0xEED9B0, uint32_t(format));  // "akb file unkwon audio format!!! %d"
            st8(self + 0x18, 1);
            return -1;
        }
        if (transfer == 0) {
            log(5, 0xEED9D4);  // "Create Transfer failed!! heap memory insufficient"
            st8(self + 0x18, 1);
            return -1;
        }
        uint32_t r = vcallo(transfer, 0x10, {transfer, seek});  // Transfer::Initialize
        if (r == 3) {
            log(5, 0xEEDA08);  // "[warn] seek time over"
            st8(self + 0x18, 1);
            return -1;
        }
        if (r != 0) {
            log(5, 0xEEDA20);  // "[Error] Voice::Initialize transfer initialize failed"
            st8(self + 0x18, 1);
            return -1;
        }
        return int32_t(rw::call(0xCE6B28, {self + 0x10, rate, channels, self + 0xC}));  // CoreSystem::CreateSource
    }
    static int32_t executeTransfer(uint32_t self) {
        if (ld8(self + 0x18) != 0) return 0;
        guest::Frame f(8);
        *f.at<uint32_t>(0) = 0;  // ExecuteParam: buffer, size
        *f.at<uint32_t>(4) = 0;
        uint32_t transfer = ld32(self + 0x14);
        vcallo(transfer, 0x14, {transfer, f.addr()});  // Transfer::Execute
        uint32_t buffer = *f.at<uint32_t>(0);
        if (buffer == 0) return 0;
        int32_t size = *f.at<int32_t>(4);
        if (size <= 0) return 0;
        uint32_t source = ld32(self + 0x10);
        return sxtb(vcallo(source, 0x24, {source, buffer, uint32_t(size)}));  // QueueBuffer
    }
    static uint32_t play(uint32_t self) {
        executeTransfer(self);
        executeTransfer(self);
        uint32_t source = ld32(self + 0x10);
        return vcallo(source, 0xC, {source});  // Start
    }
    static uint32_t onBufferEnd(uint32_t self) {
        executeTransfer(self);
        uint32_t transfer = ld32(self + 0x14);
        uint32_t r = vcallo(transfer, 8, {transfer});  // IsEnd
        if (r != 1) return r;
        uint32_t source = ld32(self + 0x10);
        uint32_t q = vcallo(source, 8, {source});  // queued buffers
        if (int32_t(q) <= 0) st8(self + 0x18, uint8_t(r));
        return q;
    }
};
RW_FUNCTION(0xCE2900, "SQEX::Voice::Voice", Voice::construct);
RW_FUNCTION(0xCE252C, "SQEX::Voice::~Voice", Voice::destruct);
RW_FUNCTION(0xCE25AC, "SQEX::Voice::Stop", Voice::stop);
RW_FUNCTION(0xCE2694, "SQEX::Voice::Create", Voice::create);
RW_FUNCTION(0xCE2948, "SQEX::Voice::ExecuteTransfer", Voice::executeTransfer);
RW_FUNCTION(0xCE29CC, "SQEX::Voice::Play", Voice::play);
RW_FUNCTION(0xCE29F4, "SQEX::Voice::OnBufferEnd", Voice::onBufferEnd);

}  // namespace sqex
