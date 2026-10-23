// Batch #10: SQEX's MS-ADPCM decoder (msadpcm_*): a streaming decoder handle that decodes one block at a time
// (optionally decrypting it first, akbMaterialDecode) with loop support, and msadpcm_decode, a whole-buffer
// decoder.
//
// Decoder handle (0x2C): source +0, source size +4, source position +8, block buffer +0xC, its size +0x10, bytes
// left in it +0x14, loop start +0x18 / loop end +0x1C (samples), block align u16 +0x20, samples per block u16
// +0x22, samples to drop u16 +0x24, channels u8 +0x26, decryption buffer +0x28.
// Per-channel state (8 bytes): predictor u8 +0, delta s16 +2, sample1 s16 +4, sample2 s16 +6.
#include "../w4_b2/Sqex.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t TABLES = 0xF46AA4;   // int nibble[16], adapt[16], coef1[7], coef2[7]
constexpr uint32_t MEMSET = 0x442310, MEMCPY = 0x442298;
constexpr uint32_t UDIVSI3 = 0xCD5DF9, UIDIVMOD = 0xCD5E79;
constexpr uint32_t AKB_MATERIAL_DECODE = 0xCDCFC0;
constexpr uint32_t GET_SAMPLES_PER_BLOCK = 0xCE8EB8;
constexpr uint32_t S_HANDLE_FAILED = 0xEEE19C, S_BUFFER_FAILED = 0xEEE1DC;

uint32_t udiv(uint32_t a, uint32_t b) { return rw::call(UDIVSI3, {a, b}); }
uint32_t umod(uint32_t a, uint32_t b) { return uint32_t(rw::call64(UIDIVMOD, {a, b}) >> 32); }

// the tables as the decoders copy them to their stack: coef1[7], coef2[7], nibble[16], adapt[16] in a row (an
// out-of-range predictor reads on into the next table)
struct Tables {
    int32_t t[46];
    Tables() {
        for (uint32_t k = 0; k < 7; k++) t[k] = lds32(TABLES + 0x80 + k * 4);
        for (uint32_t k = 0; k < 7; k++) t[7 + k] = lds32(TABLES + 0x9C + k * 4);
        for (uint32_t k = 0; k < 16; k++) t[14 + k] = lds32(TABLES + k * 4);
        for (uint32_t k = 0; k < 16; k++) t[30 + k] = lds32(TABLES + 0x40 + k * 4);
    }
    int32_t at(uint32_t k) const { return k < 46 ? t[k] : 0; }
    int32_t coef1(uint32_t p) const { return at(p); }
    int32_t coef2(uint32_t p) const { return at(7 + p); }
    int32_t nibble(uint32_t n) const { return t[14 + n]; }
    int32_t adapt(uint32_t n) const { return t[30 + n]; }
};

struct Channel {
    uint8_t predictor;
    int16_t delta, sample1, sample2;
};

// one nibble of a channel: the next sample
int16_t decodeNibble(const Tables &T, Channel &c, uint32_t nibble) {
    const int32_t predicted = T.coef2(c.predictor) * c.sample2 + T.coef1(c.predictor) * c.sample1;
    const uint32_t scaled = uint32_t(T.adapt(nibble) * int32_t(c.delta));
    int32_t sample = T.nibble(nibble) * c.delta + (predicted >> 8);
    c.sample2 = c.sample1;
    if (sample < -32768) sample = -32768;
    if (sample >= 0x7FFF) sample = 0x7FFF;
    const int16_t newDelta = int16_t(uint16_t(scaled >> 8));
    c.delta = newDelta > 15 ? newDelta : 16;
    c.sample1 = int16_t(sample);
    return int16_t(sample);
}

}  // namespace

struct Msadpcm {
    static uint32_t dataEnd(uint32_t h) { return ld32(h + 8) >= ld32(h + 4); }
    static uint32_t samplesPerBlock(uint32_t blockAlign, uint32_t channels) {
        return udiv(blockAlign << 1, channels) - 12;
    }

    // a decoder over `size` bytes of MS-ADPCM at `data`; loopEnd's block decides how far it decodes; with
    // `decrypt` the blocks are decrypted into a buffer of their own
    static uint32_t initialize(uint32_t data, uint32_t size, uint32_t channels, uint32_t blockAlign,
                               uint32_t loopStart, uint32_t loopEnd, uint32_t totalSamples, uint32_t decrypt) {
        const uint32_t h = malloc_(0x2C, 4);
        if (h == 0) {
            log(5, S_HANDLE_FAILED);
            return h;
        }
        rw::call(MEMSET, {h, 0, 0x2C});
        st8(h + 0x26, uint8_t(channels));
        st32(h + 0x18, loopStart);
        st32(h + 0x14, 0);
        st16(h + 0x24, 0);
        st32(h + 0x1C, loopEnd);
        st32(h, data);
        st32(h + 4, size);
        st16(h + 0x20, uint16_t(blockAlign));
        const uint32_t spb = rw::call(GET_SAMPLES_PER_BLOCK, {blockAlign, channels});
        st16(h + 0x22, uint16_t(spb));
        st32(h + 0x10, (channels * (spb & 0xFFFF)) << 1);
        const uint32_t buffer = malloc_(ld32(h + 0x10), 4);
        st32(h + 0xC, buffer);
        st32(h + 8, ld16(h + 0x20) * udiv(totalSamples, ld16(h + 0x22)));
        if (buffer == 0) {
            log(5, S_BUFFER_FAILED);
            free_(h);
            return buffer;
        }
        if ((decrypt & 0xFF) == 0) {
            st32(h + 0x28, 0);
            return h;
        }
        const uint32_t decryptBuffer = malloc_(ld16(h + 0x20), 4);
        st32(h + 0x28, decryptBuffer);
        if (decryptBuffer == 0) {
            log(5, S_BUFFER_FAILED);
            free_(ld32(h + 0xC));
            free_(h);
            return 0;
        }
        return h;
    }

    // up to `size` bytes of PCM into out: what is left of the last block, then block after block (a block past
    // the loop end restarts the source at the loop start's block, dropping the samples before the loop start)
    static uint32_t decoderDecode(uint32_t h, uint32_t out, uint32_t size) {
        uint32_t left;
        if (ld32(h + 0x14) != 0) {
            const uint32_t have = ld32(h + 0x14);
            const uint32_t n = size >= have ? have : size;
            left = size - n;
            rw::call(MEMCPY, {out, ld32(h + 0xC) + ld32(h + 0x10) - have, n});
            st32(h + 0x14, ld32(h + 0x14) - n);
            if (size == n) return size;
        } else {
            left = size;
            if (size == 0) return size;
        }
        const Tables T;
        while (ld32(h + 8) < ld32(h + 4)) {
            const uint32_t pos = ld32(h + 8);
            uint32_t dst = ld32(h + 0xC);
            uint32_t block = ld32(h) + pos;
            uint32_t end;
            uint32_t blockPos;
            if (ld32(h + 0x28) != 0) {
                rw::call(AKB_MATERIAL_DECODE, {pos, ld32(h + 0x28), block, ld16(h + 0x20)});
                block = ld32(h + 0x28);
                end = block + ld16(h + 0x20);
                blockPos = ld32(h + 8);
            } else {
                blockPos = ld32(h + 8);
                end = block + ld16(h + 0x20);
            }
            const uint32_t buffer = ld32(h + 0xC);
            Channel ch[2] = {};
            uint32_t channels = ld8(h + 0x26);
            const auto drop = [&]() {   // a sample frame to drop: the next one overwrites it
                if (ld16(h + 0x24) != 0) {
                    dst -= channels * 2;
                    st16(h + 0x24, uint16_t(ld16(h + 0x24) - 1));
                }
            };
            if (channels != 0) {
                // header: predictors, deltas, sample1s, sample2s (the sample2s are the block's first frame)
                ch[0].predictor = ld8(block);
                if (channels != 1) ch[1].predictor = ld8(block + 1);
                const uint32_t words = block + channels;
                ch[0].delta = lds16(words);
                if (channels == 2) {
                    block += 14;
                    ch[1].delta = lds16(words + 2);
                    ch[0].sample1 = lds16(words + 4);
                    ch[1].sample1 = lds16(words + 6);
                    ch[0].sample2 = lds16(words + 8);
                    ch[1].sample2 = lds16(words + 10);
                } else {
                    block = words + 6;
                    ch[0].sample1 = lds16(words + 2);
                    ch[0].sample2 = lds16(words + 4);
                }
                st16(dst, uint16_t(ch[0].sample2));
                channels = ld8(h + 0x26);
                if (channels > 1) {
                    st16(dst + 2, uint16_t(ch[1].sample2));
                    dst += 4;
                    channels = ld8(h + 0x26);
                } else {
                    dst += 2;
                }
            }
            drop();
            if (channels != 0) {
                st16(dst, uint16_t(ch[0].sample1));
                channels = ld8(h + 0x26);
                if (channels <= 1) dst += 2;
                if (ld8(h + 0x26) > 1) {
                    st16(dst + 2, uint16_t(ch[1].sample1));
                    dst += 4;
                    channels = ld8(h + 0x26);
                }
            }
            drop();
            // nibbles, high first, channels interleaved
            for (uint32_t p = block; end > p && p != end; p++) {
                const uint32_t byte = ld8(p);
                int32_t counted = 0;
                do {
                    if (channels != 0) {
                        int32_t k = 1;
                        int32_t last;
                        for (;;) {
                            last = k + counted;
                            const uint32_t nibble = (byte >> (4 * ((k + counted) & 1))) & 0xF;
                            const int16_t sample = decodeNibble(T, ch[(k - 1) & 1], nibble);
                            st16(dst, uint16_t(sample));
                            dst += 2;
                            channels = ld8(h + 0x26);
                            if (int32_t(channels) <= k) break;
                            k++;
                        }
                        counted = last;
                    }
                    drop();
                } while (counted <= 1);
            }
            // the block is in the buffer: loop handling, then as much as fits into out
            const uint32_t loopEnd = ld32(h + 0x1C), loopStart = ld32(h + 0x18);
            uint32_t have = ld32(h + 0x10);
            const uint32_t align = ld16(h + 0x20);
            st32(h + 0x14, have);
            st32(h + 8, blockPos + align);
            uint32_t skip = 0;
            if (loopEnd > loopStart) {
                const uint32_t decoded = ld16(h + 0x22) * udiv(blockPos + align, align);
                if (loopEnd <= decoded) {
                    skip = (decoded - loopEnd) * (channels << 1);
                    have -= skip;
                    st32(h + 0x14, have);
                    const uint32_t spb = ld16(h + 0x22);
                    st16(h + 0x24, uint16_t(umod(loopStart, spb)));
                    st32(h + 8, align * udiv(loopStart, spb));
                }
            }
            const uint32_t n = have >= left ? left : have;
            rw::call(MEMCPY, {out + (size - left), buffer + skip, n});
            left -= n;
            st32(h + 0x14, ld32(h + 0x14) - n);
            if (left == 0) break;
        }
        return size - left;
    }

    // decodes a whole buffer: {src, src size, dst, dst size, block offset out +0x10, bytes written +0x14,
    // block align +0x18, channels +0x1C}
    static uint32_t decode(uint32_t d) {
        const Tables T;
        const int32_t srcSize = lds32(d + 4);
        st32(d + 0x10, 0);
        st32(d + 0x14, 0);
        uint32_t dst = ld32(d + 8);
        if (srcSize <= 0) return 0;
        const int32_t dstSize = lds32(d + 0xC);
        if (dstSize <= 0) return 0;
        const int32_t channels = lds32(d + 0x1C);
        const int32_t align = lds32(d + 0x18);
        const uint32_t src = ld32(d);
        int32_t blockEnd = align;   // the current block is [blockEnd - align, blockEnd)
        uint32_t words = src + uint32_t(channels);
        do {
            uint32_t p = src + uint32_t(blockEnd - align);
            Channel ch[2] = {};
            if (channels > 0) {
                ch[0].predictor = ld8(p);
                if (channels == 1) {
                    ch[0].delta = lds16(words);
                    ch[0].sample1 = lds16(words + 2);
                    ch[0].sample2 = lds16(words + 4);
                    p = words + 6;
                    st16(dst, uint16_t(ch[0].sample2));
                    st16(dst + uint32_t(channels) * 2, uint16_t(ch[0].sample1));
                } else {
                    ch[1].predictor = ld8(src + 1 + uint32_t(blockEnd - align));
                    ch[0].delta = lds16(words);
                    ch[1].delta = lds16(words + 2);
                    ch[0].sample1 = lds16(words + 4);
                    ch[1].sample1 = lds16(words + 6);
                    ch[0].sample2 = lds16(words + 8);
                    ch[1].sample2 = lds16(words + 10);
                    p = words + 12;
                    st16(dst, uint16_t(ch[0].sample2));
                    st16(dst + 2, uint16_t(ch[1].sample2));
                    st16(dst + uint32_t(channels) * 2, uint16_t(ch[0].sample1));
                    st16(dst + uint32_t(channels) * 2 + 2, uint16_t(ch[1].sample1));
                }
                dst += uint32_t(channels) * 4;
                st32(d + 0x14, ld32(d + 0x14) + 2 + uint32_t(2 * channels - 2) + uint32_t(channels) * 2);
            }
            for (; src + uint32_t(blockEnd) > p; p++) {
                const uint32_t byte = ld8(p);
                int32_t counted = 0;
                do {
                    if (channels > 0) {
                        uint32_t written = ld32(d + 0x14);
                        const uint32_t base = dst - written;
                        for (int32_t c = 0; c < channels; c++) {
                            counted++;
                            const uint32_t nibble = (byte >> (4 * (counted & 1))) & 0xF;
                            st16(base + written, uint16_t(decodeNibble(T, ch[c & 1], nibble)));
                            st32(d + 0x14, written + 2);
                            written += 2;
                        }
                        dst += uint32_t(channels) * 2;
                    }
                } while (counted <= 1);
            }
            st32(d + 0x10, uint32_t(blockEnd));
            if (srcSize <= blockEnd) break;
            blockEnd += align;
            words += uint32_t(align);
        } while (lds32(d + 0x14) < dstSize);
        return 0;
    }
};
RW_FUNCTION(0xCE8EA4, "msadpcm_decoder_dataend", Msadpcm::dataEnd);
RW_FUNCTION(0xCE8EB8, "msadpcm_get_samplesPerBlock", Msadpcm::samplesPerBlock);
RW_FUNCTION(0xCE8ECC, "msadpcm_decoder_initialize", Msadpcm::initialize);
RW_FUNCTION(0xCE8A50, "msadpcm_decoder_decode", Msadpcm::decoderDecode);
RW_FUNCTION(0xCE9048, "msadpcm_decode", Msadpcm::decode);

}  // namespace sqex
