// Batch #10: SQEX::File (an Android asset or a stdio file), the OpenSL ES output's release / suspend / resume
// (CoreAudioOut*), and SQEX::CoreSystem's master volume, performance numbers and audio capture (the mixer's
// stereo float output resampled to the requested rate, channels and sample format).
#include "../w4_b2/Sqex.hpp"

using namespace b2;

namespace sqex {
namespace {

// imports
constexpr uint32_t FOPEN = 0x414158, FCLOSE = 0x414170, FSEEK = 0x4146D4, FTELL = 0x4146E0;
constexpr uint32_t ASSET_MANAGER_OPEN = 0x43D0D8, ASSET_CLOSE = 0x43D0E4, ASSET_GET_LENGTH = 0x43D108,
                   ASSET_SEEK = 0x43D114;
constexpr uint32_t FLOORF = 0x415244;
constexpr uint32_t UIDIVMOD = 0xCD5E79;   // __aeabi_uidivmod: r1 = remainder
constexpr uint32_t IDIVMOD = 0xCD5F29;
constexpr uint32_t FLOATDISF = 0xCD6D98;
constexpr uint32_t ASSET_MANAGER = 0x100EB78;     // SQEX::File's AAssetManager*
constexpr uint32_t ASSET_MODE_RANDOM = 1;
constexpr uint32_t S_RB = 0xEEAFA0;               // "rb"
constexpr uint32_t S_FILE_OPEN_ERROR = 0xEEE238;  // "file open error!! %s"

constexpr uint32_t AUDIO_OUT = 0x100E768;         // OpenSL ES: engine object +4, output mix +8, player +0xC
constexpr uint32_t SL_IID_PLAY_GOT = 0xFD9360;
constexpr uint32_t SL_PLAYSTATE_PAUSED = 2, SL_PLAYSTATE_PLAYING = 3;
constexpr uint32_t S_PLAYER_NOT_INITIALIZED_SUSPEND = 0xEEE0D0, S_PLAYER_NOT_INITIALIZED_RESUME = 0xEEE164;
constexpr uint32_t S_GET_PLAY_ITF_FAILED = 0xEEE108, S_SET_PLAY_STATE_FAILED = 0xEEE134;

constexpr uint32_t DV_SET_TARGET = 0xCF4450;
constexpr uint32_t MASTER_VOLUME_GOT = 0xFD9340;  // the core's master volume DynamicValue
constexpr uint32_t S_SET_MASTER_VOLUME_FAILED = 0xEEDDF8;
constexpr uint32_t PERFORMANCE_GOT = 0xFD9330;    // u64: the mixer's last frame time
constexpr uint32_t OUTPUT_RATE_GOT = 0xFD9338;    // int: the mixer's sample rate
constexpr uint32_t OUTPUT_CHANNELS_GOT = 0xFD9324;// int: the mixer's channels
constexpr uint32_t CAPTURE_MUTEX_GOT = 0xFD9314, OUTER_CAPTURE_MUTEX_GOT = 0xFD9348;
// the capture state: active u8 +0, format +4 (2: s16, 3: float), channels +8, rate +0xC, ring buffer +0x10,
// ring size +0x14, write position +0x18, read position +0x1C, frames in the window +0x20, window frames
// {L0 +0x24, L1 +0x28, R0 +0x2C, R1 +0x30, steps +0x34 / +0x38}, source position fraction +0x3C, phase +0x40
constexpr uint32_t CAPTURE = 0x100E6C0;
constexpr uint32_t FORMAT_S16 = 2, FORMAT_FLOAT = 3;

int32_t gotInt(uint32_t got) { return lds32(ld32(got)); }
float floorF(float v) { return bitsf(rw::call(FLOORF, {fbits(v)})); }

int32_t setPlayState(uint32_t state, uint32_t notInitialized) {
    const uint32_t player = ld32(AUDIO_OUT + 0xC);
    if (player == 0) {
        log(5, notInitialized);
        return -1;
    }
    guest::Frame f(8);
    if (vcallo(player, 0xC, {player, ld32(ld32(SL_IID_PLAY_GOT)), f.addr(4)}) != 0) {   // GetInterface
        log(5, S_GET_PLAY_ITF_FAILED);
        return -1;
    }
    const uint32_t itf = *f.at<uint32_t>(4);
    if (vcallo(itf, 0, {itf, state}) != 0) {   // SetPlayState
        log(5, S_SET_PLAY_STATE_FAILED);
        return -1;
    }
    return 0;
}

}  // namespace

struct SqexFile {
    // File: {asset u8 +0, AAsset* / FILE* +4, open u8 +8}
    static void close(uint32_t file) {
        if (ld8(file + 8) != 0) rw::call(ld8(file) != 0 ? ASSET_CLOSE : FCLOSE, {ld32(file + 4)});
        st8(file + 8, 0);
    }
    static uint32_t open(uint32_t file, uint32_t name, uint32_t asset) {
        rw::call(0xCE9A58, {file});   // Close
        st8(file, uint8_t(asset));
        const uint32_t h = (asset & 0xFF) != 0 ? rw::call(ASSET_MANAGER_OPEN, {ld32(ASSET_MANAGER), name, ASSET_MODE_RANDOM})
                                               : rw::call(FOPEN, {name, S_RB});
        st32(file + 4, h);
        st8(file + 8, h != 0);
        if (h != 0) return 1;
        log(5, S_FILE_OPEN_ERROR, name);
        return ld8(file + 8);
    }
    // 1 on success for an asset (AAsset_seek >= 0), but fseek's 0 maps to 0 (sic)
    static uint32_t seek(uint32_t file, uint32_t offset, uint32_t whence) {
        if (ld8(file + 8) == 0) return 0;
        if (ld8(file) != 0) return int32_t(rw::call(ASSET_SEEK, {ld32(file + 4), offset, whence})) >= 0;
        return rw::call(FSEEK, {ld32(file + 4), offset, whence}) != 0;
    }
    static uint32_t size(uint32_t file) {
        if (ld8(file + 8) == 0) return 0;
        if (ld8(file) != 0) return rw::call(ASSET_GET_LENGTH, {ld32(file + 4)});
        rw::call(FSEEK, {ld32(file + 4), 0, 2});
        const uint32_t n = rw::call(FTELL, {ld32(file + 4)});
        rw::call(FSEEK, {ld32(file + 4), 0, 0});
        return n;
    }
};
RW_FUNCTION(0xCE9A58, "SQEX::File::Close", SqexFile::close);
RW_FUNCTION(0xCE9AA8, "SQEX::File::Open", SqexFile::open);
RW_FUNCTION(0xCE9B8C, "SQEX::File::Seek", SqexFile::seek);
RW_FUNCTION(0xCE9BD0, "SQEX::File::Size", SqexFile::size);

struct CoreAudioOut {
    // destroys the player, the output mix and the engine (each object's Destroy, vtable +0x18)
    static void release() {
        for (uint32_t at : {AUDIO_OUT + 0xC, AUDIO_OUT + 8, AUDIO_OUT + 4}) {
            const uint32_t obj = ld32(at);
            if (obj == 0) continue;
            vcallo(obj, 0x18, {obj});
            st32(at, 0);
        }
    }
    static int32_t suspend() { return setPlayState(SL_PLAYSTATE_PAUSED, S_PLAYER_NOT_INITIALIZED_SUSPEND); }
    static int32_t resume() { return setPlayState(SL_PLAYSTATE_PLAYING, S_PLAYER_NOT_INITIALIZED_RESUME); }
};
RW_FUNCTION(0xCE8618, "CoreAudioOutRelease", CoreAudioOut::release);
RW_FUNCTION(0xCE8854, "CoreAudioOutSuspend", CoreAudioOut::suspend);
RW_FUNCTION(0xCE8940, "CoreAudioOutResume", CoreAudioOut::resume);

struct CoreSystem {
    static int32_t setMasterVolume(uint32_t volumeBits, uint32_t ms) {
        const float v = bitsf(volumeBits);
        if (v > 1.0f || v < 0.0f) {
            log(5, S_SET_MASTER_VOLUME_FAILED);
            return -1;
        }
        rw::call(DV_SET_TARGET, {ld32(MASTER_VOLUME_GOT), volumeBits, ms});
        return 0;
    }
    // {frame time u64, load: frame time / (512 / rate * 1e6)}
    static uint32_t getPerformance(uint32_t info) {
        const uint32_t time = ld32(PERFORMANCE_GOT), lo = ld32(time), hi = ld32(time + 4);
        st32(info, lo);
        st32(info + 4, hi);
        const float t = bitsf(rw::call(FLOATDISF, {lo, hi}));
        const float frame = (512.0f / float(gotInt(OUTPUT_RATE_GOT))) * 1000000.0f;
        stf(info + 8, t / frame);
        return 0;
    }

    // starts capturing: *rate / *channels 0 take the mixer's; channels must be 1 or 2, format 1 means 3 (float)
    static int32_t startAudioCapture(uint32_t rate, uint32_t channels, uint32_t format) {
        const uint32_t outer = ld32(OUTER_CAPTURE_MUTEX_GOT), inner = ld32(CAPTURE_MUTEX_GOT);
        mutexLock(outer);
        mutexLock(inner);
        int32_t r = -1;
        // (a defaulted channel count or rate skips its range check)
        uint32_t ch = ld32(channels);
        bool ok = ld8(CAPTURE) == 0;
        if (ok && ch == 0) {
            ch = uint32_t(gotInt(OUTPUT_CHANNELS_GOT));
            st32(channels, ch);
        } else if (ok) {
            ok = ch - 1 <= 1;
        }
        if (ok) {
            st32(CAPTURE + 8, ch);
            int32_t hz = lds32(rate);
            if (hz == 0) {
                hz = gotInt(OUTPUT_RATE_GOT);
                st32(rate, uint32_t(hz));
            } else {
                ok = hz >= 0;
            }
            if (ok) {
                uint32_t fmt = ld32(format);
                st32(CAPTURE + 0xC, uint32_t(hz));
                if (fmt == 1) {
                    fmt = FORMAT_FLOAT;
                    st32(format, fmt);
                }
                if (fmt != 0) {
                    st32(CAPTURE + 4, fmt);
                    st32(CAPTURE + 0x3C, 0);
                    st32(CAPTURE + 0x40, 0);
                    for (uint32_t at = 0x20; at <= 0x30; at += 4) st32(CAPTURE + at, 0);
                    const uint32_t size = uint32_t(gotInt(OUTPUT_RATE_GOT) * gotInt(OUTPUT_CHANNELS_GOT)) << 2;
                    st32(CAPTURE + 0x14, size);
                    const uint32_t buffer = malloc_(size, 4);
                    st32(CAPTURE + 0x18, 0);
                    st32(CAPTURE + 0x1C, 0);
                    st8(CAPTURE, 1);
                    st32(CAPTURE + 0x10, buffer);
                    r = 0;
                }
            }
        }
        mutexUnlock(inner);
        mutexUnlock(outer);
        return r;
    }
    static int32_t resetAudioCaptureData() {
        const uint32_t outer = ld32(OUTER_CAPTURE_MUTEX_GOT), inner = ld32(CAPTURE_MUTEX_GOT);
        mutexLock(outer);
        mutexLock(inner);
        int32_t r = -1;
        if (ld8(CAPTURE) != 0) {
            st32(CAPTURE + 0x1C, 0);
            st32(CAPTURE + 0x18, 0);
            r = 0;
        }
        mutexUnlock(inner);
        mutexUnlock(outer);
        return r;
    }
    // the bytes a read could deliver now: captured bytes scaled to the target rate and channels (s16: half)
    static int32_t getReadyAudioBytes() {
        const uint32_t outer = ld32(OUTER_CAPTURE_MUTEX_GOT), inner = ld32(CAPTURE_MUTEX_GOT);
        mutexLock(outer);
        mutexLock(inner);
        int32_t n = 0;
        if (ld8(CAPTURE) != 0) {
            const float rateScale = float(lds32(CAPTURE + 0xC)) / float(gotInt(OUTPUT_RATE_GOT));
            const float channelScale = float(lds32(CAPTURE + 8)) / float(gotInt(OUTPUT_CHANNELS_GOT));
            const float half = ld32(CAPTURE + 4) == FORMAT_S16 ? 0.5f : 1.0f;
            float v = float(lds32(CAPTURE + 0x18) - lds32(CAPTURE + 0x1C)) * rateScale;
            v = v * channelScale;
            n = f2i(v * half);
        }
        mutexUnlock(inner);
        mutexUnlock(outer);
        return n;
    }

    // reads `size` bytes of captured audio into dst (0 unless size is whole frames and the mixer is stereo):
    // each output frame interpolates linearly between two source frames of a window that advances by the rate
    // ratio; the bytes written
    static int32_t readAudioCaptureData(uint32_t dst, int32_t size) {
        const uint32_t outer = ld32(OUTER_CAPTURE_MUTEX_GOT), inner = ld32(CAPTURE_MUTEX_GOT);
        mutexLock(outer);
        mutexLock(inner);
        int32_t produced = 0;
        if (ld8(CAPTURE) == 0) {
            mutexUnlock(inner);
            mutexUnlock(outer);
            return 0;
        }
        mutexUnlock(inner);
        const uint32_t fmt = ld32(CAPTURE + 4);
        const uint32_t sampleBytes = fmt == FORMAT_FLOAT ? 4 : fmt == FORMAT_S16 ? 2 : 0;
        const int32_t channels = lds32(CAPTURE + 8);
        const uint32_t frameBytes = sampleBytes * uint32_t(channels);
        if (uint32_t(rw::call64(UIDIVMOD, {uint32_t(size), frameBytes}) >> 32) == 0 &&
            gotInt(OUTPUT_CHANNELS_GOT) == 2 && (channels == 2 || channels == 1)) {
            // the mix matrix {L->0, R->0, L->1, R->1}; mono takes the left channel at -3 dB only (sic)
            float matrix[4] = {0, 0, 0, 0};
            if (channels == 2) {
                matrix[0] = 1.0f;
                matrix[3] = 1.0f;
            } else {
                matrix[0] = matrix[2] = bitsf(0x3F3504F3);   // 0.70710677
            }
            const float ratio = float(gotInt(OUTPUT_RATE_GOT)) / float(lds32(CAPTURE + 0xC));
            if (size > 0) do {
                mutexLock(inner);
                fillWindow(ratio);
                mutexUnlock(inner);
                const int32_t frames = lds32(CAPTURE + 0x20);
                if (frames <= 1) break;   // not enough captured data
                float phase = ldf(CAPTURE + 0x40);
                const float t = phase / float(lds32(CAPTURE + 0x34));
                const float left = ldf(CAPTURE + 0x24) + (ldf(CAPTURE + 0x28) - ldf(CAPTURE + 0x24)) * t;
                const float right = ldf(CAPTURE + 0x2C) + (ldf(CAPTURE + 0x30) - ldf(CAPTURE + 0x2C)) * t;
                if (channels > 0) {
                    uint32_t out = dst;
                    for (int32_t c = 0; c < channels; c++) {
                        float v = 0.0f + left * matrix[2 * c];
                        v = v + right * matrix[2 * c + 1];
                        if (v > 1.0f) v = 1.0f;
                        else if (v < -1.0f) v = -1.0f;
                        if (fmt == FORMAT_FLOAT) stf(out, v);
                        else if (fmt == FORMAT_S16) st16(out, uint16_t(f2i(v * 32767.0f)));
                        out += sampleBytes;
                    }
                    dst += frameBytes;
                }
                phase = ratio + phase;
                produced += int32_t(frameBytes);
                stf(CAPTURE + 0x40, phase);
                if (phase >= 1.0f) {   // the window moves one source frame on
                    const uint32_t step1 = ld32(CAPTURE + 0x38), l1 = ld32(CAPTURE + 0x28);
                    st32(CAPTURE + 0x28, 0);
                    st32(CAPTURE + 0x34, step1);
                    const uint32_t r1 = ld32(CAPTURE + 0x30);
                    st32(CAPTURE + 0x30, 0);
                    st32(CAPTURE + 0x20, uint32_t(frames - 1));
                    st32(CAPTURE + 0x24, l1);
                    st32(CAPTURE + 0x2C, r1);
                    st32(CAPTURE + 0x38, 0);
                    stf(CAPTURE + 0x40, phase - floorF(phase));
                }
            } while (produced < size);
        }
        mutexUnlock(outer);
        return produced;
    }

private:
    // loads source frames into the window until it holds two or the captured data runs out (under the capture
    // mutex), then writes the window state back
    static void fillWindow(float ratio) {
        float frac = ldf(CAPTURE + 0x3C);
        int32_t frames = lds32(CAPTURE + 0x20);
        int32_t read = lds32(CAPTURE + 0x1C);
        const uint32_t buffer = ld32(CAPTURE + 0x10);
        const int32_t ring = lds32(CAPTURE + 0x14);
        int32_t write = lds32(CAPTURE + 0x18);
        while (frames <= 1 && write - read > 0) {
            const uint32_t at = uint32_t(rw::call64(IDIVMOD, {uint32_t(read), uint32_t(ring)}) >> 32);
            const uint32_t slot = CAPTURE + 0x30 + uint32_t(frames) * 4;
            frames++;
            frac = ratio + frac;
            int32_t step = f2i(frac);
            st32(slot - 0xC, ld32(buffer + at));       // left
            st32(slot - 4, ld32(buffer + at + 4));     // right
            int32_t advance;
            if (step > 0) {
                advance = step << 3;
            } else {
                advance = 8;
                step = 1;
            }
            read += advance;
            st32(slot + 4, uint32_t(step));
            frac = frac - floorF(frac);
            if (ring > read) continue;
            // wrap both positions into the ring (only once the write position passed its end too)
            for (;;) {
                if (ring > write) break;
                write = int32_t(rw::call64(IDIVMOD, {uint32_t(write), uint32_t(ring)}) >> 32);
                read = int32_t(rw::call64(IDIVMOD, {uint32_t(read), uint32_t(ring)}) >> 32);
                if (ring > read) break;
            }
        }
        stf(CAPTURE + 0x3C, frac);
        st32(CAPTURE + 0x20, uint32_t(frames));
        st32(CAPTURE + 0x1C, uint32_t(read));
        st32(CAPTURE + 0x18, uint32_t(write));
    }
};
RW_FUNCTION(0xCE6C88, "SQEX::CoreSystem::SetMasterVolume", CoreSystem::setMasterVolume);
RW_FUNCTION(0xCE6DA0, "SQEX::CoreSystem::GetPerformance", CoreSystem::getPerformance);
RW_FUNCTION(0xCE6E08, "SQEX::CoreSystem::StartAudioCapture", CoreSystem::startAudioCapture);
RW_FUNCTION(0xCE6F8C, "SQEX::CoreSystem::ReadAudioCaptureData", CoreSystem::readAudioCaptureData);
RW_FUNCTION(0xCE74BC, "SQEX::CoreSystem::ResetAudioCaptureData", CoreSystem::resetAudioCaptureData);
RW_FUNCTION(0xCE7528, "SQEX::CoreSystem::GetReadyAudioBytes", CoreSystem::getReadyAudioBytes);

}  // namespace sqex
