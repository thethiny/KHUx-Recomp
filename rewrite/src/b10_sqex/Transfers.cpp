// Batch #10: SQEX transfers - the objects that feed a voice decoded PCM, two buffers in turn (Execute fills
// ExecuteParam {buffer, bytes}): on-memory PCM and MS-ADPCM materials, and the streaming PCM / MS-ADPCM / Ogg Vorbis
// transfers that read from a StreamingSound's ring buffer.
//
// Execute returns 0 (data), 1 (finished: also sets +4) or 2 (error).
// TransferPcm: vtable, finished u8 +4, read position (bytes into the material) +8, material +0xC.
// TransferMSAdpcm: vtable, finished +4, material +8, decoder +0xC, buffers +0x10 / +0x14, buffer size +0x18, current
//   buffer +0x1C.
// Streaming: vtable, finished +4, buffers +8 / +0xC, current u8 +0x10, buffer size +0x14, voice +0x18 (its
//   StreamingSound +4, material +8); Vorbis adds OggVorbis_File +0x20 (0x298 bytes) and the decoded sample +0x2B8.
#include "../w4_b2/Sqex.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t VT_MSADPCM_GOT = 0xFD92B8, VT_PCM_STREAMING_GOT = 0xFD92C4, VT_MSADPCM_STREAMING_GOT = 0xFD92C8,
                   VT_VORBIS_STREAMING_GOT = 0xFD92CC, VT_OBJECT_GOT = 0xFD9250;
constexpr uint32_t VORBIS_READ_CALLBACKS_GOT = 0xFD92D0;   // ov_callbacks.read_func of the streaming transfer
constexpr uint32_t MEMSET = 0x442310, MEMCPY = 0x442298;
constexpr uint32_t DIVSI3 = 0xCD5E8D, UDIVSI3 = 0xCD5DF9, LDIVMOD = 0xCD72F8;
// akb material accessors
constexpr uint32_t M_AUDIO_DATA = 0xCDC93C, M_AUDIO_OFFSET = 0xCDC8C0, M_CHANNEL = 0xCDC9CC, M_SAMPLE_RATE = 0xCDC9E8,
                   M_IS_LOOP = 0xCDCA04, M_NUM_SAMPLES = 0xCDCAAC, M_LOOP_START = 0xCDCB70, M_LOOP_END = 0xCDCB8C,
                   M_AUDIO_SIZE = 0xCDCBA8, M_ADPCM_BLOCK_ALIGN = 0xCDCDDC, M_ADPCM_NUM_SAMPLES = 0xCDCDFC,
                   M_ADPCM_LOOP_START = 0xCDCE1C, M_ADPCM_LOOP_END = 0xCDCE3C, M_OGG_LOOP_START_BYTES = 0xCDCE5C,
                   M_ENCRYPTION = 0xCDCF70, M_DECODE = 0xCDCFC0;
// msadpcm
constexpr uint32_t ADPCM_DECODER_DECODE = 0xCE8A50, ADPCM_SAMPLES_PER_BLOCK = 0xCE8EB8, ADPCM_DECODER_INIT = 0xCE8ECC,
                   ADPCM_DECODE = 0xCE9048;
// StreamingSound
constexpr uint32_t SS_GET_READ_ADDR = 0xCE1B0C, SS_IS_DATA_END = 0xCE1B8C, SS_GET_READABLE_SIZE = 0xCE1C08,
                   SS_SET_READ_BYTES = 0xCE1C98, SS_STEP_OVER_LOOP_END = 0xCE1DD8;
// libvorbis / libogg / libvorbisfile
constexpr uint32_t OV_CLEAR = 0xCEEDB4, OV_OPEN_CALLBACKS = 0xCEFBAC, OV_READ = 0xCF1394, OV_PCM_TOTAL = 0xCEF2F0,
                   GET_NEXT_PAGE = 0xCE3BFC, OGG_SYNC_RESET = 0xCF3610, OGG_STREAM_RESET_SERIALNO = 0xCF369C,
                   OGG_STREAM_PAGEIN = 0xCF3254, OGG_STREAM_PACKETOUT = 0xCF36D0, OGG_STREAM_PACKETPEEK = 0xCF3704,
                   OGG_PAGE_SERIALNO = 0xCF237C, OGG_PAGE_BOS = 0xCF2270, VORBIS_SYNTHESIS_RESTART = 0xCE9F34,
                   VORBIS_PACKET_BLOCKSIZE = 0xCED6F4, VORBIS_INFO_BLOCKSIZE = 0xCEADD4,
                   VORBIS_SYNTHESIS_TRACKONLY = 0xCED6EC, VORBIS_SYNTHESIS_BLOCKIN = 0xCEA3BC,
                   VORBIS_SYNTHESIS_PCMOUT = 0xCEAA04, VORBIS_SYNTHESIS_READ = 0xCEAA80, VORBIS_SYNTHESIS = 0xCED6E4,
                   VORBIS_DSP_CLEAR = 0xCE9FA4, VORBIS_BLOCK_CLEAR = 0xCE9EF0, VORBIS_INFO_CLEAR = 0xCEAE20,
                   VORBIS_COMMENT_CLEAR = 0xCEAD08;
constexpr uint32_t S_ADPCM_BUFFER_FAILED = 0xEEDA58, S_PCM_STREAM_BUFFER_FAILED = 0xEEDAF4,
                   S_ADPCM_STREAM_BUFFER_FAILED = 0xEEDB38, S_VORBIS_STREAM_BUFFER_FAILED = 0xEEDB94,
                   S_OV_OPEN_FAILED = 0xEED86C, S_OV_READ_ERROR = 0xEEDB80;

uint32_t m(uint32_t fn, uint32_t material) { return rw::call(fn, {material}); }
int32_t idiv(int32_t a, int32_t b) { return int32_t(rw::call(DIVSI3, {uint32_t(a), uint32_t(b)})); }
int64_t ldiv(int64_t a, int64_t b) {
    return int64_t(rw::call64(LDIVMOD, {uint32_t(a), uint32_t(uint64_t(a) >> 32), uint32_t(b), uint32_t(uint64_t(b) >> 32)}));
}
int64_t ld64(uint32_t a) { return int64_t(uint64_t(ld32(a)) | (uint64_t(ld32(a + 4)) << 32)); }
void st64(uint32_t a, int64_t v) {
    st32(a, uint32_t(uint64_t(v)));
    st32(a + 4, uint32_t(uint64_t(v) >> 32));
}
// a seek in ms as a sample position: ms * rate / 1000
int64_t seekSamples(uint32_t ms, uint32_t material) {
    const uint64_t product = uint64_t(ms) * uint64_t(m(M_SAMPLE_RATE, material));
    return ldiv(int64_t(product), 1000);
}

// two transfer buffers of `size` bytes at buffers / buffers + 4 (each failure logged)
void allocBuffers(uint32_t sizeAt, uint32_t buffers, uint32_t message) {
    const uint32_t a = malloc_(ld32(sizeAt), 4);
    st32(buffers, a);
    if (a == 0) log(5, message);
    const uint32_t b = malloc_(ld32(sizeAt), 4);
    st32(buffers + 4, b);
    if (b == 0) log(5, message);
}

// the streaming transfers' common end: when the ring's data ended, a looping material steps over the loop end,
// anything else is finished; the buffers swap
uint32_t streamingDone(uint32_t self, uint32_t sound, uint32_t material) {
    uint32_t r = 0;
    const uint32_t end = rw::call(SS_IS_DATA_END, {sound});
    if (end == 1) {
        if (rw::call(M_IS_LOOP, {material}) != 1) {
            st8(self + 4, uint8_t(end));
            r = end;
        } else {
            rw::call(SS_STEP_OVER_LOOP_END, {sound});
        }
    }
    st8(self + 0x10, uint8_t((ld8(self + 0x10) + 1) & 1));
    return r;
}

}  // namespace

struct TransferPcm {
    static int32_t initialize(uint32_t self, uint32_t seekMs) {
        if (seekMs == 0) return 0;
        const uint32_t material = ld32(self + 0xC);
        int64_t pos = seekSamples(seekMs, material);
        if (rw::call(M_IS_LOOP, {ld32(self + 0xC)}) == 0) {
            if (pos >= int64_t(m(M_NUM_SAMPLES, ld32(self + 0xC)))) return 3;
        } else if (pos >= int64_t(m(M_LOOP_END, ld32(self + 0xC)))) {
            const uint32_t loopStart = m(M_LOOP_START, ld32(self + 0xC));
            const int64_t loopEnd = m(M_LOOP_END, ld32(self + 0xC));
            pos = pos - ldiv(pos, loopEnd) * loopEnd + int64_t(loopStart);
        }
        const uint32_t channels = m(M_CHANNEL, ld32(self + 0xC)) & 0xFF;
        st32(self + 8, uint32_t(channels * uint32_t(pos)) * 2 + ld32(self + 8));
        return 0;
    }
    // up to 2 KB per channel straight from the material (a loop end wraps to the loop start)
    static int32_t execute(uint32_t self, uint32_t param) {
        st32(param, ld32(self + 0xC) + ld32(self + 8));
        st32(param + 4, m(M_CHANNEL, ld32(self + 0xC)) << 11);
        int32_t at = int32_t(ld32(self + 8) - m(M_AUDIO_OFFSET, ld32(self + 0xC)));
        if (rw::call(M_IS_LOOP, {ld32(self + 0xC)}) != 0) {
            const int32_t frame = int32_t(int16_t(uint16_t(m(M_CHANNEL, ld32(self + 0xC))))) << 1;
            const int32_t endBytes = int32_t(m(M_LOOP_END, ld32(self + 0xC))) * frame;
            int32_t n = endBytes - at;
            if (lds32(param + 4) > n) st32(param + 4, uint32_t(n));
            else n = lds32(param + 4);
            at += n;
            st32(self + 8, ld32(self + 8) + uint32_t(n));
            if (endBytes > at) return 0;
            st32(self + 8, uint32_t(frame) * m(M_LOOP_START, ld32(self + 0xC)) + m(M_AUDIO_OFFSET, ld32(self + 0xC)));
            return 0;
        }
        int32_t n;
        if (lds32(param + 4) > int32_t(m(M_AUDIO_SIZE, ld32(self + 0xC))) - at) {
            n = int32_t(m(M_AUDIO_SIZE, ld32(self + 0xC))) - at;
            st32(param + 4, uint32_t(n));
        } else {
            n = lds32(param + 4);
        }
        at += n;
        st32(self + 8, ld32(self + 8) + uint32_t(n));
        if (at < int32_t(m(M_AUDIO_SIZE, ld32(self + 0xC)))) return 0;
        st8(self + 4, 1);
        return 1;
    }
};
RW_FUNCTION(0xCE3454, "SQEX::TransferPcm::Initialize", TransferPcm::initialize);
RW_FUNCTION(0xCE332C, "SQEX::TransferPcm::Execute", TransferPcm::execute);

struct TransferMSAdpcm {
    static uint32_t ctor(uint32_t self, uint32_t material) {
        st32(self + 8, material);
        st8(self + 4, 0);
        st32(self, ld32(VT_MSADPCM_GOT) + 8);
        st32(self + 0xC, 0);
        st32(self + 0x1C, 0);
        const uint32_t channels = m(M_CHANNEL, material);
        rw::call(ADPCM_SAMPLES_PER_BLOCK, {m(M_ADPCM_BLOCK_ALIGN, material), channels});   // (unused)
        st32(self + 0x18, channels << 11);
        allocBuffers(self + 0x18, self + 0x10, S_ADPCM_BUFFER_FAILED);
        return self;
    }
    // the decoder, started at the seek position (past the loop end it wraps into the loop)
    static int32_t initialize(uint32_t self, uint32_t seekMs) {
        uint32_t start = 0;
        if (seekMs != 0) {
            int64_t pos = seekSamples(seekMs, ld32(self + 8));
            if (rw::call(M_IS_LOOP, {ld32(self + 8)}) == 0) {
                if (pos >= int64_t(m(M_ADPCM_NUM_SAMPLES, ld32(self + 8)))) return 3;
            } else if (pos >= int64_t(m(M_ADPCM_LOOP_END, ld32(self + 8)))) {
                const uint32_t loopStart = m(M_ADPCM_LOOP_START, ld32(self + 8));
                const int64_t loopEnd = m(M_ADPCM_LOOP_END, ld32(self + 8));
                pos = int64_t(uint64_t(pos - ldiv(pos, loopEnd) * loopEnd));
                pos = int64_t((uint64_t(pos) & 0xFFFFFFFF00000000ull) | uint32_t(loopStart + uint32_t(pos)));
            }
            start = uint32_t(pos);
        }
        uint32_t loopStart = 0, loopEnd = 0;
        if (rw::call(M_IS_LOOP, {ld32(self + 8)}) != 0) {
            loopStart = m(M_ADPCM_LOOP_START, ld32(self + 8));
            loopEnd = m(M_ADPCM_LOOP_END, ld32(self + 8));
        }
        const uint32_t data = m(M_AUDIO_DATA, ld32(self + 8));
        const uint32_t size = m(M_AUDIO_SIZE, ld32(self + 8));
        const uint32_t channels = m(M_CHANNEL, ld32(self + 8));
        const uint32_t align = m(M_ADPCM_BLOCK_ALIGN, ld32(self + 8));
        const uint32_t encrypted = m(M_ENCRYPTION, ld32(self + 8)) == 1;
        st32(self + 0xC, rw::call(ADPCM_DECODER_INIT, {data, size, channels, align, loopStart, loopEnd, start, encrypted}));
        return 0;
    }
    static int32_t execute(uint32_t self, uint32_t param) {
        const int32_t n = int32_t(rw::call(ADPCM_DECODER_DECODE,
                                           {ld32(self + 0xC), ld32(self + 0x10 + ld32(self + 0x1C) * 4), ld32(self + 0x18)}));
        if (n == 0) {
            if (rw::call(M_IS_LOOP, {ld32(self + 8)}) != 0) return 0;
            st8(self + 4, 1);
            return 1;
        }
        if (n < 0) return 2;
        const uint32_t cur = ld32(self + 0x1C);
        st32(param + 4, uint32_t(n));
        st32(param, ld32(self + 0x10 + cur * 4));
        st32(self + 0x1C, uint32_t(int32_t(cur + 1) % 2));
        return 0;
    }
};
RW_FUNCTION(0xCE2D10, "SQEX::TransferMSAdpcm::TransferMSAdpcm", TransferMSAdpcm::ctor);
RW_FUNCTION(0xCE2B28, "SQEX::TransferMSAdpcm::Initialize", TransferMSAdpcm::initialize);
RW_FUNCTION(0xCE2C88, "SQEX::TransferMSAdpcm::Execute", TransferMSAdpcm::execute);

struct TransferStreaming {
    static void init(uint32_t self, uint32_t voice, uint32_t vtableGot) {
        st32(self + 0x18, voice);
        st8(self + 4, 0);
        st32(self, ld32(vtableGot) + 8);
        st8(self + 0x10, 0);
        st32(self + 0x14, 0);
    }
    static uint32_t pcmCtor(uint32_t self, uint32_t voice) {
        init(self, voice, VT_PCM_STREAMING_GOT);
        st32(self + 0x14, m(M_CHANNEL, ld32(voice + 8)) << 12);
        allocBuffers(self + 0x14, self + 8, S_PCM_STREAM_BUFFER_FAILED);
        return self;
    }
    // as much of the ring as is readable, copied into the current buffer
    static uint32_t pcmExecute(uint32_t self, uint32_t param) {
        const uint32_t sound = ld32(ld32(self + 0x18) + 4);
        const uint32_t size = ld32(self + 0x14);
        st32(param, ld32(self + 8 + ld8(self + 0x10) * 4));
        st32(param + 4, size);
        if (int32_t(size) > int32_t(rw::call(SS_GET_READABLE_SIZE, {sound})))
            st32(param + 4, rw::call(SS_GET_READABLE_SIZE, {sound}));
        const uint32_t dst = ld32(param);
        rw::call(MEMCPY, {dst, rw::call(SS_GET_READ_ADDR, {sound, 0}), ld32(param + 4)});
        rw::call(SS_SET_READ_BYTES, {sound, ld32(param + 4)});
        return streamingDone(self, sound, ld32(ld32(self + 0x18) + 8));
    }

    static uint32_t msadpcmCtor(uint32_t self, uint32_t voice) {
        st8(self + 4, 0);
        st8(self + 0x10, 0);
        st32(self, ld32(VT_MSADPCM_STREAMING_GOT) + 8);
        st32(self + 0x18, voice);
        const uint32_t material = ld32(voice + 8);
        st32(self + 0x14, 0);
        const uint32_t align = m(M_ADPCM_BLOCK_ALIGN, material);
        const uint32_t spb = rw::call(ADPCM_SAMPLES_PER_BLOCK, {align, m(M_CHANNEL, material)});
        st32(self + 0x14, m(M_CHANNEL, material) * (spb << 4));
        allocBuffers(self + 0x14, self + 8, S_ADPCM_STREAM_BUFFER_FAILED);
        return self;
    }
    // decodes the readable part of the ring (msadpcm_decode) into the current buffer
    static uint32_t msadpcmExecute(uint32_t self, uint32_t param) {
        const uint32_t voice = ld32(self + 0x18);
        const uint32_t sound = ld32(voice + 4), material = ld32(voice + 8);
        guest::Frame f(0x20);
        *f.at<uint32_t>(0x18) = m(M_ADPCM_BLOCK_ALIGN, material);
        const uint32_t channels = m(M_CHANNEL, material);
        *f.at<uint32_t>(0xC) = ld32(self + 0x14);
        *f.at<uint32_t>(8) = ld32(self + 8 + ld8(self + 0x10) * 4);
        *f.at<uint32_t>(0x1C) = channels;
        *f.at<uint32_t>(0) = rw::call(SS_GET_READ_ADDR, {sound, 0});
        *f.at<uint32_t>(4) = rw::call(SS_GET_READABLE_SIZE, {sound});
        rw::call(ADPCM_DECODE, {f.addr(0)});
        st32(param, *f.at<uint32_t>(8));
        st32(param + 4, *f.at<uint32_t>(0x14));
        rw::call(SS_SET_READ_BYTES, {sound, *f.at<uint32_t>(0x10)});
        return streamingDone(self, sound, material);
    }
};
RW_FUNCTION(0xCE36B8, "SQEX::TransferPcmStreaming::TransferPcmStreaming", TransferStreaming::pcmCtor);
RW_FUNCTION(0xCE35E8, "SQEX::TransferPcmStreaming::Execute", TransferStreaming::pcmExecute);
RW_FUNCTION(0xCE38C4, "SQEX::TransferMSAdpcmStreaming::TransferMSAdpcmStreaming", TransferStreaming::msadpcmCtor);
RW_FUNCTION(0xCE37E8, "SQEX::TransferMSAdpcmStreaming::Execute", TransferStreaming::msadpcmExecute);

struct TransferVorbisStreaming {
    static uint32_t ctor(uint32_t self, uint32_t voice) {
        st32(self + 0x18, voice);
        st8(self + 4, 0);
        st32(self, ld32(VT_VORBIS_STREAMING_GOT) + 8);
        st8(self + 0x10, 0);
        st32(self + 0x14, 0);
        rw::call(MEMSET, {self + 0x20, 0, 0x298});
        st32(self + 0x2B8, 0);
        st32(self + 0x14, m(M_CHANNEL, ld32(voice + 8)) << 12);
        allocBuffers(self + 0x14, self + 8, S_VORBIS_STREAM_BUFFER_FAILED);
        return self;
    }
    static uint32_t dtor(uint32_t self) {
        st32(self, ld32(VT_VORBIS_STREAMING_GOT) + 8);
        rw::call(OV_CLEAR, {self + 0x20});
        free_(ld32(self + 8));
        free_(ld32(self + 0xC));
        st32(self, ld32(VT_OBJECT_GOT) + 8);
        return self;
    }
    // libvorbisfile's read callback: whole items from the ring (decrypted when the material is), the ring read
    // position moved on, a looping material stepped over its loop end
    static uint32_t readCallback(uint32_t ptr, uint32_t size, uint32_t nmemb, uint32_t self) {
        const uint32_t sound = ld32(ld32(self + 0x18) + 4);
        int32_t avail = int32_t(rw::call(SS_GET_READABLE_SIZE, {sound}));
        const int32_t want = int32_t(nmemb * size);
        if (avail >= want) avail = want;
        const uint32_t count = rw::call(UDIVSI3, {uint32_t(avail), size});
        const uint32_t bytes = size * count;
        if (rw::call(M_ENCRYPTION, {ld32(ld32(self + 0x18) + 8)}) == 1) {
            guest::Frame f(8);
            const uint32_t src = rw::call(SS_GET_READ_ADDR, {sound, f.addr(4)});
            rw::call(M_DECODE, {*f.at<uint32_t>(4), ptr, src, bytes});
        } else {
            rw::call(MEMCPY, {ptr, rw::call(SS_GET_READ_ADDR, {sound, 0}), bytes});
        }
        rw::call(SS_SET_READ_BYTES, {sound, bytes});
        if (rw::call(SS_IS_DATA_END, {sound}) == 1 && rw::call(M_IS_LOOP, {ld32(ld32(self + 0x18) + 8)}) == 1)
            rw::call(SS_STEP_OVER_LOOP_END, {sound});
        return count;
    }
    // opens the Vorbis stream on what the ring holds (decrypted into buffer 0 first when encrypted)
    static int32_t initialize(uint32_t self, uint32_t /*seekMs*/) {
        const uint32_t voice = ld32(self + 0x18);
        const uint32_t sound = ld32(voice + 4), material = ld32(voice + 8);
        guest::Frame f(0x28);
        *f.at<uint32_t>(0x14) = 0;
        *f.at<uint32_t>(0x18) = 0;
        *f.at<uint32_t>(0x10) = ld32(VORBIS_READ_CALLBACKS_GOT);
        *f.at<uint32_t>(0x1C) = 0;
        uint32_t avail = rw::call(SS_GET_READABLE_SIZE, {sound});
        uint32_t initial = rw::call(SS_GET_READ_ADDR, {sound, 0});
        if (rw::call(M_ENCRYPTION, {material}) == 1) {
            const uint32_t buffer = ld32(self + 8);
            if (int32_t(avail) >= lds32(self + 0x14)) avail = ld32(self + 0x14);
            rw::call(M_DECODE, {0, buffer, rw::call(SS_GET_READ_ADDR, {sound, 0}), avail});
            initial = ld32(self + 8);
        }
        if (int32_t(rw::call(OV_OPEN_CALLBACKS, {self, self + 0x20, initial, avail, *f.at<uint32_t>(0x10),
                                                 *f.at<uint32_t>(0x14), *f.at<uint32_t>(0x18), *f.at<uint32_t>(0x1C)})) < 0) {
            log(5, S_OV_OPEN_FAILED);
            return 2;
        }
        rw::call(SS_SET_READ_BYTES, {sound, avail});
        return 0;
    }

    // ov_read into the current buffer (not past a looping material's loop end; reaching it seeks back to the loop
    // start, the libvorbisfile page seek done inline)
    static int32_t execute(uint32_t self, uint32_t param) {
        const uint32_t voice = ld32(self + 0x18);
        const uint32_t material = ld32(voice + 8), sound = ld32(voice + 4);
        const int32_t frame = int32_t(int16_t(uint16_t(m(M_CHANNEL, material)))) * 2;
        int32_t want = lds32(self + 0x14);
        if (rw::call(M_IS_LOOP, {material}) == 1) {
            const int32_t next = idiv(want, frame) + lds32(self + 0x2B8);
            if (next >= int32_t(m(M_LOOP_END, material)) + 1)
                want = (int32_t(m(M_LOOP_END, material)) - lds32(self + 0x2B8)) * frame + frame;
        }
        const uint32_t vf = self + 0x20;
        guest::Frame f(0xA0);
        const int32_t got = int32_t(rw::call(OV_READ, {vf, ld32(self + 8 + ld8(self + 0x10) * 4), uint32_t(want), f.addr(0x34)}));
        if (got < 0) {
            log(5, S_OV_READ_ERROR);
            return 2;
        }
        st32(self + 0x2B8, uint32_t(idiv(got, frame) + lds32(self + 0x2B8)));
        if (rw::call(M_IS_LOOP, {material}) == 1 && lds32(self + 0x2B8) >= int32_t(m(M_LOOP_END, material)) + 1)
            seekToLoopStart(self, material, f);
        int32_t r = 0;
        if (got == 0) {
            if (rw::call(M_IS_LOOP, {material}) != 0 || rw::call(SS_IS_DATA_END, {sound}) == 0) {
                log(5, S_OV_READ_ERROR);
                return 2;
            }
            st8(self + 4, 1);
            r = 1;
        }
        st32(param, ld32(self + 8 + ld8(self + 0x10) * 4));
        st32(param + 4, uint32_t(got));
        st8(self + 0x10, uint8_t((ld8(self + 0x10) + 1) & 1));
        return r;
    }

private:
    // OggVorbis_File (at self + 0x20) fields as self offsets
    static constexpr uint32_t VF_OFFSET = 0x28, VF_OY = 0x38, VF_VI = 0x68, VF_VC = 0x6C, VF_PCM_OFFSET = 0x70,
                              VF_READY_STATE = 0x78, VF_SERIALNO = 0x7C, VF_LINK = 0x80, VF_BITTRACK = 0x88,
                              VF_SAMPTRACK = 0x90, VF_OS = 0x98, VF_VD = 0x200, VF_VB = 0x250, VF_SEEKABLE = 0x24;
    // stack: ogg_page og +0x38 / +0x48, ogg_packet op +0x58 / +0x78 (bytes +4, e_o_s +0xC, granulepos +0x10)
    static constexpr uint32_t OG = 0x38, OG2 = 0x48, OP = 0x58, OP2 = 0x78;

    static int64_t nextPage(uint32_t self, uint32_t og) {
        return int64_t(rw::call64(GET_NEXT_PAGE, {self + 0x20, og}));
    }
    static int64_t pcmout(uint32_t self) {
        return int32_t(rw::call(VORBIS_SYNTHESIS_PCMOUT, {self + VF_VD, 0}));
    }

    // back to the loop start: the page holding it (from the loop start's byte offset), its packets tracked up to the
    // block containing the target sample, then decoded samples dropped until the PCM offset reaches it
    static void seekToLoopStart(uint32_t self, uint32_t material, guest::Frame &f) {
        st32(self + 0x2B8, m(M_LOOP_START, material));
        const uint32_t startBytes = m(M_OGG_LOOP_START_BYTES, material);
        const int64_t target = int64_t(uint64_t(m(M_LOOP_START, material)));
        st32(self + VF_OFFSET, startBytes);
        st32(self + VF_OFFSET + 4, 0);
        rw::call(OGG_SYNC_RESET, {self + VF_OY});
        st64(self + VF_PCM_OFFSET, -1);
        nextPage(self, f.addr(OG));
        rw::call(VORBIS_SYNTHESIS_RESTART, {self + VF_VD});
        rw::call(OGG_STREAM_RESET_SERIALNO, {self + VF_OS, ld32(self + VF_SERIALNO)});
        rw::call(OGG_STREAM_PAGEIN, {self + VF_OS, f.addr(OG)});
        // the first packet with a granule position
        for (;;) {
            rw::call(OGG_STREAM_PACKETPEEK, {self + VF_OS, f.addr(OP)});
            if (ld64(f.addr(OP + 0x10)) != -1) break;
            rw::call(OGG_STREAM_PACKETOUT, {self + VF_OS, 0});
        }
        st64(self + VF_PCM_OFFSET, ld64(f.addr(OP + 0x10)));
        // track packets until the next block would pass the target
        int32_t lastBlock = 0;
        for (;;) {
            const int32_t ret = int32_t(rw::call(OGG_STREAM_PACKETPEEK, {self + VF_OS, f.addr(OP2)}));
            if (ret > 0) {
                const int32_t block = int32_t(rw::call(
                    VORBIS_PACKET_BLOCKSIZE, {ld32(self + VF_VI) + ld32(self + VF_LINK) * 32, f.addr(OP2)}));
                if (block < 0) {
                    rw::call(OGG_STREAM_PACKETOUT, {self + VF_OS, 0});
                    continue;
                }
                if (lastBlock != 0)
                    st64(self + VF_PCM_OFFSET, ld64(self + VF_PCM_OFFSET) + ((block + lastBlock) >> 2));
                const int64_t offset = ld64(self + VF_PCM_OFFSET);
                const int32_t big = int32_t(rw::call(VORBIS_INFO_BLOCKSIZE, {ld32(self + VF_VI), 1}));
                if (offset + ((block + big) >> 2) >= target) break;
                rw::call(OGG_STREAM_PACKETOUT, {self + VF_OS, 0});
                rw::call(VORBIS_SYNTHESIS_TRACKONLY, {self + VF_VB, f.addr(OP2)});
                rw::call(VORBIS_SYNTHESIS_BLOCKIN, {self + VF_VD, self + VF_VB});
                const int64_t granule = ld64(f.addr(OP2 + 0x10));
                if (granule >= 0) st64(self + VF_PCM_OFFSET, granule);
                lastBlock = block;
            } else {
                if (ret != -3 && ret < 0) break;
                if (nextPage(self, f.addr(OG2)) < 0) break;
                rw::call(OGG_STREAM_PAGEIN, {self + VF_OS, f.addr(OG2)});
            }
        }
        // decode and drop samples up to the target
        st64(self + VF_BITTRACK, 0);
        st64(self + VF_SAMPTRACK, 0);
        int64_t pos = ld64(self + VF_PCM_OFFSET);
        while (pos < target) {
            const int64_t need = target - pos;
            int64_t have = pcmout(self);
            if (need < have) have = int32_t(uint32_t(need));
            rw::call(VORBIS_SYNTHESIS_READ, {self + VF_VD, uint32_t(have)});
            pos = ld64(self + VF_PCM_OFFSET) + have;
            st64(self + VF_PCM_OFFSET, pos);
            if (have >= need) continue;
            pos = fetchPacket(self, f);
        }
    }

    // libvorbisfile's _fetch_and_process_packet, as inlined: the next decoded packet (the new PCM offset), or the
    // stream's total at its end
    static int64_t fetchPacket(uint32_t self, guest::Frame &f) {
        const uint32_t os = self + VF_OS;
        for (;;) {
            int32_t state = lds32(self + VF_READY_STATE);
            if (state == 4) {
                const int32_t ret = int32_t(rw::call(OGG_STREAM_PACKETOUT, {os, f.addr(OP2)}));
                if (ret == -1) break;
                if (ret > 0) {
                    const int64_t granule = ld64(f.addr(OP2 + 0x10));
                    if (rw::call(VORBIS_SYNTHESIS, {self + VF_VB, f.addr(OP2)}) != 0) continue;
                    if (pcmout(self) != 0) break;
                    rw::call(VORBIS_SYNTHESIS_BLOCKIN, {self + VF_VD, self + VF_VB});
                    const int64_t samples = pcmout(self);
                    st64(self + VF_BITTRACK, ld64(self + VF_BITTRACK) + int64_t(int32_t(ld32(f.addr(OP2 + 4)) << 3)));
                    st64(self + VF_SAMPTRACK, ld64(self + VF_SAMPTRACK) + samples);
                    if (granule == -1 || ld32(f.addr(OP2 + 0xC)) != 0) return ld64(self + VF_PCM_OFFSET);
                    const int64_t g = granule < 0 ? 0 : granule;
                    const int64_t offset = g - pcmout(self);
                    st64(self + VF_PCM_OFFSET, offset);
                    return offset;
                }
                state = lds32(self + VF_READY_STATE);
            }
            if (state > 1) {
                bool pageIn = false;
                while (!pageIn) {
                    if (nextPage(self, f.addr(OG2)) < 0) {
                        const int64_t total = int64_t(rw::call64(OV_PCM_TOTAL, {self + 0x20, 0xFFFFFFFF}));
                        st64(self + VF_PCM_OFFSET, total);
                        return total;
                    }
                    st64(self + VF_BITTRACK, ld64(self + VF_BITTRACK) + int64_t(int32_t(ld32(f.addr(OG2 + 4)) << 3)));
                    if (lds32(self + VF_READY_STATE) != 4) break;
                    const uint32_t serial = ld32(self + VF_SERIALNO);
                    if (serial == rw::call(OGG_PAGE_SERIALNO, {f.addr(OG2)})) break;
                    if (rw::call(OGG_PAGE_BOS, {f.addr(OG2)}) != 0) {
                        rw::call(VORBIS_DSP_CLEAR, {self + VF_VD});
                        rw::call(VORBIS_BLOCK_CLEAR, {self + VF_VB});
                        st32(self + VF_READY_STATE, 2);
                        if (ld32(self + VF_SEEKABLE) == 0) {
                            rw::call(VORBIS_INFO_CLEAR, {ld32(self + VF_VI)});
                            rw::call(VORBIS_COMMENT_CLEAR, {ld32(self + VF_VC)});
                        }
                        pageIn = true;
                    }
                }
            }
            rw::call(OGG_STREAM_PAGEIN, {os, f.addr(OG2)});
        }
        const int64_t total = int64_t(rw::call64(OV_PCM_TOTAL, {self + 0x20, 0xFFFFFFFF}));
        st64(self + VF_PCM_OFFSET, total);
        return total;
    }
};
RW_FUNCTION(0xCE42A4, "SQEX::TransferVorbisStreaming::TransferVorbisStreaming", TransferVorbisStreaming::ctor);
RW_FUNCTION(0xCE39B4, "SQEX::TransferVorbisStreaming::~TransferVorbisStreaming", TransferVorbisStreaming::dtor);
RW_FUNCTION(0xCE3A1C, "SQEX::TransferVorbisStreaming::ov_read_callback", TransferVorbisStreaming::readCallback);
RW_FUNCTION(0xCE3AF8, "SQEX::TransferVorbisStreaming::Initialize", TransferVorbisStreaming::initialize);
RW_FUNCTION(0xCE3CC0, "SQEX::TransferVorbisStreaming::Execute", TransferVorbisStreaming::execute);

}  // namespace sqex
