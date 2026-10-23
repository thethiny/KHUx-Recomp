// Batch #10: SQEX::StreamingSound - a SoundImpl fed from a ring buffer that a delegate (the sound's IDelegate
// part at +0xFC, run by the DelegateManager's thread) refills from its IStreamReader.
//
// StreamingSound (0x138): SoundImpl, IDelegate vtable +0xFC, +0x100 / +0x104, recursive mutex +0x108, ring buffer
// +0x10C, write position +0x114, read position +0x118, ring size +0x120, data end (-1: not reached) +0x124, loop
// length +0x128, loop start read position +0x12C, +0x130, initialized u8 +0x134.
#include "../w4_b2/SqexPool.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t VT_STREAMING_SOUND_GOT = 0xFD92AC;
constexpr uint32_t MUTEXATTR_INIT = 0x44D098, MUTEXATTR_SETTYPE = 0x44D0A4, MUTEX_INIT = 0x4466F0,
                   MUTEXATTR_DESTROY = 0x44D0B0;   // (imports)
constexpr uint32_t IDIVMOD = 0xCD5F29;             // __aeabi_idivmod: r0 quotient, r1 remainder
constexpr uint32_t UDIVSI3 = 0xCD5DF9;
constexpr uint32_t SOUND_IMPL_CTOR = 0xCDD880, SOUND_IMPL_UPDATE = 0xCDEA30, SOUND_IMPL_GET_STATE = 0xCDD970;
constexpr uint32_t DELEGATE_IS_EXIST = 0xCF4328, DELEGATE_ADD = 0xCF42A0;
constexpr uint32_t GET_MATERIAL_DATA = 0xCE1D34, IS_DATA_END = 0xCE1B8C, GET_LOOP_START_READ_POS = 0xCE1D48;
constexpr uint32_t AKB_MATERIAL_FORMAT = 0xCDC8A4, AKB_MATERIAL_IS_LOOP = 0xCDCA04,
                   AKB_MATERIAL_LOOP_START = 0xCDCB70, AKB_MATERIAL_CHANNEL = 0xCDC9CC,
                   AKB_OGG_LOOP_START_BYTES = 0xCDCE5C, AKB_ATRAC9_LOOP_START = 0xCDCEE0,
                   AKB_ATRAC9_SAMPLES_PER_BLOCK = 0xCDCF28, AKB_ATRAC9_BLOCK_ALIGN = 0xCDCF10;
constexpr uint32_t AKB_IS_AKB1 = 0xCDC2E0, AKB_IS_AKB2 = 0xCDC360, AKB_GET_HEADER_SIZE = 0xCDC42C,
                   AKB_GET_SOUND_DATA_OFFSET = 0xCDC534, AKB_SOUND_OFFSET_TABLE_SIZE = 0xCDC5CC,
                   AKB_SOUND_HEADER_SIZE = 0xCDC7FC, AKB_SOUND_MATERIAL_TABLE_SIZE = 0xCDC818,
                   AKB_SOUND_MATERIAL_OFFSET = 0xCDC7A4, AKB_MATERIAL_AUDIO_OFFSET = 0xCDC8C0,
                   AKB_MATERIAL_AUDIO_SIZE = 0xCDCBA8, AKB_MSADPCM_BLOCK_ALIGN = 0xCDCDDC;
constexpr uint32_t GET_MATERIAL_DATA_OFFSET = 0xCE1D04;
constexpr uint32_t MEMMOVE = 0x4422C8;             // (import)
constexpr uint32_t FORMAT_PCM = 1, FORMAT_MSADPCM = 2, FORMAT_OGG_VORBIS = 5, FORMAT_ATRAC9 = 7;
constexpr uint32_t S_NO_AKB_FILE = 0xEED9A4;      // "no akb file"
constexpr uint32_t STREAMING_SOUND_CTOR = 0xCE1A6C, SOUND_IMPL_DESTROY = 0xCDDAC0,
                   SOUND_IMPL_SET_EXTERNAL_ID = 0xCDDAA8, SOUND_CTOR = 0xCE1334;
constexpr uint32_t SOUND_SLOT_VTABLE_GOT = 0xFD9290;
constexpr uint32_t S_CREATE_SOUND_FAILED = 0xEED30C;   // "create OnMemorySound failed!! heap memory insufficient"
constexpr uint32_t S_SOUND_LIST_FULL = 0xEED344, S_SOUND_LIST_APPEND_FAILED = 0xEED394;
constexpr uint32_t S_STREAMING_INIT_FAILED = 0xEED3B0; // "SoundSystem::CreateStreamingSound StreamingSound ..."
constexpr uint32_t S_GET_READ_ADDR_INVALID = 0xEED8B0, S_IS_DATA_END_INVALID = 0xEED8E0,
                   S_GET_READABLE_SIZE_INVALID = 0xEED90C, S_STEP_OVER_LOOP_END_INVALID = 0xEED970;

uint32_t remainder(uint32_t a, uint32_t b) { return uint32_t(rw::call64(IDIVMOD, {a, b}) >> 32); }

}  // namespace

struct StreamingSound {
    static uint32_t update(uint32_t sound, uint32_t elapsed) {
        rw::call(SOUND_IMPL_UPDATE, {sound, elapsed});
        const uint32_t state = rw::call(SOUND_IMPL_GET_STATE, {sound});
        if (state == 3) return state;
        const uint32_t delegate = sound + 0xFC;
        const uint32_t queued = rw::call(DELEGATE_IS_EXIST, {delegate});
        if (queued != 0) return queued;
        return rw::call(DELEGATE_ADD, {delegate});
    }

    static uint32_t ctor(uint32_t sound) {
        rw::call(SOUND_IMPL_CTOR, {sound});
        st32(sound + 0x100, 0);
        const uint32_t vt = ld32(VT_STREAMING_SOUND_GOT);
        st32(sound + 0x104, 0);
        st32(sound, vt + 8);
        st32(sound + 0xFC, vt + 0x34);
        guest::Frame f(8);
        rw::call(MUTEXATTR_INIT, {f.addr(4)});
        rw::call(MUTEXATTR_SETTYPE, {f.addr(4), 1});   // recursive
        rw::call(MUTEX_INIT, {sound + 0x108, f.addr(4)});
        rw::call(MUTEXATTR_DESTROY, {f.addr(4)});
        for (uint32_t at = 0x10C; at <= 0x120; at += 4) st32(sound + at, 0);
        st32(sound + 0x128, 0);
        st32(sound + 0x12C, 0);
        st32(sound + 0x130, 0);
        st8(sound + 0x134, 0);
        st32(sound + 0x124, 0xFFFFFFFF);
        return sound;
    }

    // the ring buffer address the read position maps to; *loopStart gets the loop start read position
    static uint32_t getReadAddr(uint32_t sound, uint32_t loopStart) {
        if (ld8(sound + 0x134) == 0) {
            log(5, S_GET_READ_ADDR_INVALID);
            return 0;
        }
        mutexLock(sound + 0x108);
        if (loopStart != 0) st32(loopStart, ld32(sound + 0x12C));
        const uint32_t addr = ld32(sound + 0x10C) + remainder(ld32(sound + 0x118), ld32(sound + 0x120));
        mutexUnlock(sound + 0x108);
        return addr;
    }
    // the read position reached the data end
    static uint32_t isDataEnd(uint32_t sound) {
        if (ld8(sound + 0x134) == 0) {
            log(5, S_IS_DATA_END_INVALID);
            return 0;
        }
        mutexLock(sound + 0x108);
        const int32_t end = lds32(sound + 0x124);
        const uint32_t r = end < 0 ? 0 : (end > lds32(sound + 0x118) ? 0 : 1);
        mutexUnlock(sound + 0x108);
        return r;
    }
    // the bytes readable now: up to the write position (or the data end before it), not past the ring's end
    static uint32_t getReadableSize(uint32_t sound) {
        if (ld8(sound + 0x134) == 0) {
            log(5, S_GET_READABLE_SIZE_INVALID);
            return 0;
        }
        mutexLock(sound + 0x108);
        const int32_t end = lds32(sound + 0x124), write = lds32(sound + 0x114), read = lds32(sound + 0x118);
        int32_t size = write - read;
        if (end >= 0 && write > end) size = end - read;
        const int32_t ring = lds32(sound + 0x120);
        const int32_t at = int32_t(remainder(uint32_t(read), uint32_t(ring)));
        if (ring < at + size) size = ring - at;
        mutexUnlock(sound + 0x108);
        return uint32_t(size);
    }
    // where reading resumes after the loop end, in bytes of the material's encoding
    static uint32_t getMaterialLoopStartReadPos(uint32_t sound) {
        const uint32_t m = rw::call(GET_MATERIAL_DATA, {sound});
        switch (rw::call(AKB_MATERIAL_FORMAT, {m})) {
        case FORMAT_OGG_VORBIS: return rw::call(AKB_OGG_LOOP_START_BYTES, {m});
        case FORMAT_PCM: {
            const uint32_t loopStart = rw::call(AKB_MATERIAL_LOOP_START, {m});
            return rw::call(AKB_MATERIAL_CHANNEL, {m}) * (loopStart << 1);
        }
        case FORMAT_ATRAC9: {
            const uint32_t loopStart = rw::call(AKB_ATRAC9_LOOP_START, {m});
            const uint32_t blocks = rw::call(UDIVSI3, {loopStart, rw::call(AKB_ATRAC9_SAMPLES_PER_BLOCK, {m})});
            return rw::call(AKB_ATRAC9_BLOCK_ALIGN, {m}) * blocks;
        }
        default: return 0;
        }
    }
    // at the data end of a looping material: the read position moves to the next loop boundary and reading
    // resumes at the loop start
    static uint32_t stepOverLoopEnd(uint32_t sound) {
        if (ld8(sound + 0x134) == 0) return rw::call(ANDROID_LOG, {5, LOG_TAG, S_STEP_OVER_LOOP_END_INVALID});
        mutexLock(sound + 0x108);
        if (rw::call(IS_DATA_END, {sound}) == 1 &&
            rw::call(AKB_MATERIAL_IS_LOOP, {rw::call(GET_MATERIAL_DATA, {sound})}) == 1) {
            const uint32_t read = ld32(sound + 0x118), length = ld32(sound + 0x128);
            const uint32_t rem = remainder(read, length);
            if (rem != 0) st32(sound + 0x118, read + (length - rem));
            const uint32_t loopStart = rw::call(GET_LOOP_START_READ_POS, {sound});
            st32(sound + 0x124, 0xFFFFFFFF);
            st32(sound + 0x12C, loopStart);
        }
        return mutexUnlock(sound + 0x108);
    }

    // the delegate's work, one phase per call (+0x100): 0 start; 1 read the file head until the AKB header up to
    // the first material's audio data is in; 2 move that head to the buffer's end, the rest of the buffer becomes
    // the ring (a whole number of blocks of the material's encoding); 3 refill the ring from the reader, wrapping
    // the positions, and at the data end loop back (looping materials) or mark where the data ends
    static void invoke(uint32_t sound) {
        switch (ld32(sound + 0x100)) {
        case 0:
            mutexLock(sound + 0x108);
            st32(sound + 0x100, 1);
            mutexUnlock(sound + 0x108);
            return;
        case 1: readHead(sound); return;
        case 2: setUpRing(sound); return;
        case 3: refill(sound); return;
        default: return;
        }
    }

private:
    // IStreamReader::Read(READ_PARAM*): {dst, size, file offset, buffered, total} on the stack; the bytes read
    static uint32_t read(uint32_t sound, uint32_t dst, uint32_t size, uint32_t offset, uint32_t buffered,
                         uint32_t total) {
        guest::Frame f(0x18);
        *f.at<uint32_t>(4) = dst;
        *f.at<uint32_t>(8) = size;
        *f.at<uint32_t>(0xC) = offset;
        *f.at<uint32_t>(0x10) = buffered;
        *f.at<uint32_t>(0x14) = total;
        const uint32_t reader = ld32(sound + 0x104);
        return vcallo(reader, 8, {reader, f.addr(4)});
    }

    static void readHead(uint32_t sound) {
        const uint32_t write = ld32(sound + 0x114), capacity = ld32(sound + 0x110);
        const uint32_t n = read(sound, ld32(sound + 0x10C) + write, capacity - write, write, 0, capacity);
        const int32_t have = int32_t(ld32(sound + 0x114) + n);
        st32(sound + 0x114, uint32_t(have));
        if (have <= 7) return;
        const uint32_t buf = ld32(sound + 0x10C);
        const auto need = [&](int32_t size) { return size <= lds32(sound + 0x114); };
        if (rw::call(AKB_IS_AKB1, {buf}) == 1) {
            if (lds32(sound + 0x114) < int32_t(rw::call(AKB_GET_HEADER_SIZE, {buf}))) return;
            if (lds32(sound + 0x114) < int32_t(rw::call(AKB_MATERIAL_AUDIO_OFFSET, {ld32(sound + 0x10C)}))) return;
        } else {
            if (rw::call(AKB_IS_AKB2, {buf}) != 1) {
                log(5, S_NO_AKB_FILE);
                return;
            }
            int32_t at = int32_t(rw::call(AKB_GET_HEADER_SIZE, {ld32(sound + 0x10C)}));
            if (!need(at)) return;
            at += 4 + int32_t(rw::call(AKB_SOUND_OFFSET_TABLE_SIZE, {ld32(sound + 0x10C)}));
            if (!need(at)) return;
            const uint32_t soundAt = uint32_t(at - 4);
            const uint32_t akbSound = ld32(sound + 0x10C) + soundAt;
            at += int32_t(rw::call(AKB_SOUND_HEADER_SIZE, {akbSound})) - 4;
            if (!need(at)) return;
            if (!need(int32_t(rw::call(AKB_SOUND_MATERIAL_TABLE_SIZE, {akbSound})) + at)) return;
            const uint32_t materialAt = rw::call(AKB_SOUND_MATERIAL_OFFSET, {akbSound, 0});
            const int32_t audioAt = int32_t(soundAt + materialAt + 8);
            if (!need(audioAt)) return;
            if (!need(int32_t(rw::call(AKB_MATERIAL_AUDIO_OFFSET, {akbSound + materialAt})) + audioAt)) return;
        }
        st32(sound + 0x100, 2);
    }

    static void setUpRing(uint32_t sound) {
        uint32_t head = rw::call(AKB_GET_SOUND_DATA_OFFSET, {ld32(sound + 0x10C), 0});
        head += rw::call(AKB_SOUND_MATERIAL_OFFSET, {ld32(sound + 0x10C) + head, 0});
        head += rw::call(AKB_MATERIAL_AUDIO_OFFSET, {ld32(sound + 0x10C) + head});
        const uint32_t buf = ld32(sound + 0x10C);
        const uint32_t ringEnd = buf + ((ld32(sound + 0x110) - head) & ~0xFu);
        st32(sound + 0x130, ringEnd);
        rw::call(MEMMOVE, {ringEnd, buf, head});   // the header now sits after the ring
        const uint32_t m = rw::call(GET_MATERIAL_DATA, {sound});
        uint32_t block;
        switch (rw::call(AKB_MATERIAL_FORMAT, {m})) {
        case FORMAT_OGG_VORBIS: block = 0x400; break;
        case FORMAT_PCM: block = uint32_t(int32_t(int16_t(rw::call(AKB_MATERIAL_CHANNEL, {m})))) << 1; break;
        case FORMAT_MSADPCM: block = rw::call(AKB_MSADPCM_BLOCK_ALIGN, {m}); break;
        case FORMAT_ATRAC9: block = rw::call(AKB_ATRAC9_BLOCK_ALIGN, {m}); break;
        default: block = ld32(sound + 0x128); break;
        }
        st32(sound + 0x128, block);
        const uint32_t space = ld32(sound + 0x130) - ld32(sound + 0x10C);
        st32(sound + 0x120, space - remainder(space, block));
        if (rw::call(AKB_MATERIAL_AUDIO_SIZE, {m}) < ld32(sound + 0x120))
            st32(sound + 0x120, rw::call(AKB_MATERIAL_AUDIO_SIZE, {m}));
        st32(sound + 0x11C, head);
        st32(sound + 0x114, 0);
        st32(sound + 0x118, 0);
        st32(sound + 0x100, 3);
    }

    static void refill(uint32_t sound) {
        const int32_t ring = lds32(sound + 0x120);
        const int32_t write = lds32(sound + 0x114), readPos = lds32(sound + 0x118);
        const int32_t filled = write - readPos;
        const uint32_t buffered = ld8(sound + 0x134) ? uint32_t(filled) : 0;
        if (ring <= filled) {
            st8(sound + 0x134, 1);   // full once: the sound may start
            return;
        }
        int32_t n = ring + readPos - write;
        const int32_t at = int32_t(remainder(uint32_t(write), uint32_t(ring)));
        const uint32_t dst = ld32(sound + 0x10C) + uint32_t(at);
        if (ring < at + n) n = ring - at;
        const uint32_t m = rw::call(GET_MATERIAL_DATA, {sound});
        int32_t end = int32_t(rw::call(GET_MATERIAL_DATA_OFFSET, {sound}));
        end = int32_t(rw::call(AKB_MATERIAL_AUDIO_OFFSET, {m})) + end;
        end += int32_t(rw::call(AKB_MATERIAL_AUDIO_SIZE, {m}));
        const int32_t offset = lds32(sound + 0x11C);
        if (n >= end - offset) n = end - offset;
        if (dst == 0 || n <= 0) return;
        const uint32_t got = read(sound, dst, uint32_t(n), uint32_t(offset), buffered, ld32(sound + 0x120));
        mutexLock(sound + 0x108);
        const int32_t fileAt = int32_t(got) + lds32(sound + 0x11C);
        st32(sound + 0x11C, uint32_t(fileAt));
        uint32_t w = got + ld32(sound + 0x114);
        st32(sound + 0x114, w);
        if (fileAt >= end) {
            st32(sound + 0x124, w);   // the data ends here
            if (rw::call(AKB_MATERIAL_IS_LOOP, {m}) == 1) {
                w = ld32(sound + 0x114);
                const uint32_t length = ld32(sound + 0x128);
                const uint32_t rem = remainder(w, length);
                if (rem != 0) {
                    w += length - rem;
                    st32(sound + 0x114, w);
                }
                const uint32_t audioAt = rw::call(AKB_MATERIAL_AUDIO_OFFSET, {m});
                st32(sound + 0x11C, rw::call(GET_LOOP_START_READ_POS, {sound}) + audioAt);
            }
            w = ld32(sound + 0x114);
        }
        const int32_t size = lds32(sound + 0x120);
        if (size <= int32_t(w) && size <= lds32(sound + 0x118)) {   // both past one ring length: wrap
            st32(sound + 0x114, w - uint32_t(size));
            st32(sound + 0x118, ld32(sound + 0x118) - uint32_t(size));
            st32(sound + 0x124, ld32(sound + 0x124) - uint32_t(size));
        }
        mutexUnlock(sound + 0x108);
    }
};
// SoundManager::CreateStreamingSound: a StreamingSound in the sound pool, initialized with the reader and its
// buffer (the same steps as SoundManager::CreateSound's OnMemorySound, w4_b2/SqexManager.cpp)
struct StreamingSoundManager {
    static int32_t createStreamingSound(uint32_t out, uint32_t reader, uint32_t buffer, uint32_t size,
                                        uint32_t hasExternalId, uint32_t externalId, uint32_t layout) {
        mutexLock(SOUND_MUTEX);
        int32_t result = -1;
        const uint32_t sound = malloc_(0x138, 4);
        rw::call(STREAMING_SOUND_CTOR, {sound});
        const auto fail = [&](uint32_t message) {
            rw::call(SOUND_IMPL_DESTROY, {sound});
            vcallo(sound, 4, {sound});   // deleting destructor
            if (message) log(5, message);
        };
        if (sound == 0) {
            log(5, S_CREATE_SOUND_FAILED);
        } else {
            uint32_t node = ld32(SOUND_MANAGER + pool::FREE);
            if (node == 0) node = pool::grow(SOUND_MANAGER, SOUND_SLOT_VTABLE_GOT);
            if (node == 0) {
                fail(S_SOUND_LIST_APPEND_FAILED);
            } else {
                const int32_t idx = lds32(node + 0xC);
                pool::take(SOUND_MANAGER, node, sound);
                if (idx >= 0x10000) {
                    log(5, S_SOUND_LIST_FULL, SOUND_MANAGER);
                    pool::release(SOUND_MANAGER, idx);
                    fail(0);
                } else {
                    const uint32_t serial = pool::nextSerial(SOUND_MANAGER + 0x1C);
                    const uint32_t id = (uint32_t(idx) & 0xFFFF) | (serial << 16);
                    const uint32_t init = ld32(ld32(sound) + 0x1C);
                    st16(SOUND_MANAGER + 0x1C, uint16_t(serial));
                    const int32_t r = int32_t(rw::call(init, {sound, id, reader, buffer, size}));
                    if ((hasExternalId & 0xFF) != 0)
                        rw::call(SOUND_IMPL_SET_EXTERNAL_ID, {sound, uint32_t(int32_t(int16_t(externalId)))});
                    st8(sound + 0xD0, uint8_t(layout));
                    if (r < 0) {
                        pool::release(SOUND_MANAGER, idx);
                        fail(S_STREAMING_INIT_FAILED);
                    } else {
                        guest::Frame f(8);
                        rw::call(SOUND_CTOR, {f.addr(4), ld32(sound + 8)});
                        st32(out, *f.at<uint32_t>(4));
                        result = 0;
                    }
                }
            }
        }
        mutexUnlock(SOUND_MUTEX);
        return result;
    }
};
RW_FUNCTION(0xCDFA8C, "SQEX::SoundManager::CreateStreamingSound", StreamingSoundManager::createStreamingSound);
RW_FUNCTION(0xCE1E90, "SQEX::StreamingSound::Invoke", StreamingSound::invoke);
RW_FUNCTION(0xCE1948, "SQEX::StreamingSound::Update", StreamingSound::update);
RW_FUNCTION(0xCE1A6C, "SQEX::StreamingSound::StreamingSound", StreamingSound::ctor);
RW_FUNCTION(0xCE1B0C, "SQEX::StreamingSound::GetReadAddr", StreamingSound::getReadAddr);
RW_FUNCTION(0xCE1B8C, "SQEX::StreamingSound::IsDataEnd", StreamingSound::isDataEnd);
RW_FUNCTION(0xCE1C08, "SQEX::StreamingSound::GetReadableSize", StreamingSound::getReadableSize);
RW_FUNCTION(0xCE1D48, "SQEX::StreamingSound::GetMaterialLoopStartReadPos", StreamingSound::getMaterialLoopStartReadPos);
RW_FUNCTION(0xCE1DD8, "SQEX::StreamingSound::StepOverLoopEnd", StreamingSound::stepOverLoopEnd);

}  // namespace sqex
