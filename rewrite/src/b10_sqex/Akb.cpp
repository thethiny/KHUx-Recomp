// Batch #10: the rest of the AKB sound bank accessors (akb*; the others are in w4_b2/SdSoundSystem.cpp) and
// attrFloatCmp, the attribute system's float comparison.
//
// AKB1 files start "AKB " (version u16 +4, header size u16 +6); AKB2 files "AKB2" (version u8 +4, header size
// u16 +6, sound count u16 +0xC, then the sound offset table of 16-byte entries {.., offset +4}). An AKB1 file is
// one sound with one material; most per-sound / per-material fields differ between the two layouts.
#include "../w4_b2/Sqex.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t AKB2_HEADER_SIZES = 0xF46A98;   // u16[2]: the header size of AKB2 versions 0 and 1
constexpr uint32_t AKB_DECODE_TABLE_GOT = 0xFD927C; // the 256-byte XOR table of encrypted materials

constexpr uint32_t IS_AKB1 = 0xCDC2E0, IS_AKB2 = 0xCDC360, GET_VERSION = 0xCDC3F8, GET_HEADER_SIZE = 0xCDC42C;
constexpr uint32_t SOUND_NUM_MATERIALS = 0xCDC73C, SOUND_MATERIAL_DATA = 0xCDC7E8;
constexpr uint32_t MATERIAL_AUDIO_FORMAT = 0xCDC8A4, MATERIAL_EXTRA_DATA_OFFSET = 0xCDC958,
                   MATERIAL_SAMPLE_RATE = 0xCDC9E8, MATERIAL_IS_LOOP = 0xCDCA04, MATERIAL_NUM_SAMPLES = 0xCDCAAC;
constexpr uint32_t LDIVMOD = 0xCD72F8;              // __aeabi_ldivmod
constexpr uint32_t FORMAT_OGG_VORBIS = 5;

bool isAkb1(uint32_t p) { return rw::call(IS_AKB1, {p}) == 1; }
bool isAkb2(uint32_t p) { return rw::call(IS_AKB2, {p}) == 1; }

// a material's {min, max} pair at `akb2Offset` (AKB1: at akb2Offset + 4, version 2+, else {fallback, fallback})
uint32_t materialRange(uint32_t out, uint32_t m, uint32_t akb2Offset, uint32_t fallback) {
    if (!isAkb1(m)) {
        st32(out, ld32(m + akb2Offset));
        st32(out + 4, ld32(m + akb2Offset + 4));
    } else if (rw::call(GET_VERSION, {m}) > 1) {
        st32(out, ld32(m + akb2Offset + 4));
        st32(out + 4, ld32(m + akb2Offset + 8));
    } else {
        st32(out, fallback);
        st32(out + 4, fallback);
    }
    return out;
}

}  // namespace

struct AkbMore {
    static uint32_t isAkb2File(uint32_t p) {
        if (ld32(p) != 0x32424B41) return 0;   // "AKB2"
        const uint32_t version = ld8(p + 4);
        if (version > 1) return 0;
        return ld16(p + 6) == ld16(AKB2_HEADER_SIZES + version * 2);
    }
    static uint32_t isAkbFile(uint32_t p) {
        const uint32_t r = rw::call(IS_AKB1, {p});
        if (r == 1) return r;
        return isAkb2(p);
    }
    static uint32_t getNumSounds(uint32_t p) {
        const uint32_t r = rw::call(IS_AKB1, {p});
        if (r == 1) return r;
        return isAkb2(p) ? ld16(p + 0xC) : 0;
    }
    // the offset of sound `index` (0 when not an AKB2 file, -1 past the last sound)
    static uint32_t getSoundDataOffset(uint32_t p, int32_t index) {
        if (!isAkb2(p) || !isAkb2(p)) return 0;
        if (int32_t(ld16(p + 0xC)) <= index) return 0xFFFFFFFF;
        return ld32(p + uint32_t(index) * 16 + ld16(p + 6) + 4);
    }
    static uint32_t getSoundOffsetTableSize(uint32_t p) {
        if (isAkb1(p) || !isAkb2(p)) return 0;
        return uint32_t(ld16(p + 0xC)) << 4;
    }

    // AKB2 sounds keep a work byte at +0x10
    static void soundSetWork(uint32_t s, uint32_t work) {
        if (rw::call(IS_AKB1, {s}) == 0) st8(s + 0x10, uint8_t(work));
    }
    // material `index`'s random-selection weight (byte 1 of its entry in the material table)
    static uint32_t soundGetRandomValue(uint32_t s, int32_t index) {
        if (isAkb1(s)) return 0;
        if (int32_t(rw::call(SOUND_NUM_MATERIALS, {s})) <= index) return 0;
        const uint32_t table = isAkb1(s) ? 0 : s + ld16(s + 2);
        return ld8(table + uint32_t(index) * 16 + 1);
    }
    static uint32_t soundGetMaterialOffsetTableSize(uint32_t s) {
        return isAkb1(s) ? 0 : uint32_t(ld8(s + 0xF)) << 4;
    }
    // any of the sound's materials loops
    static uint32_t soundGetIsLoop(uint32_t s) {
        const int32_t n = int32_t(rw::call(SOUND_NUM_MATERIALS, {s}));
        for (int32_t k = 0; k < n; k++) {
            const uint32_t m = rw::call(SOUND_MATERIAL_DATA, {s, uint32_t(k)});
            if (rw::call(MATERIAL_IS_LOOP, {m}) == 1) return 1;
        }
        return 0;
    }
    // the longest material in ms: samples * 1000 / sample rate
    static uint32_t soundGetPlayTime(uint32_t s) {
        const int32_t n = int32_t(rw::call(SOUND_NUM_MATERIALS, {s}));
        if (n == 0) return 0;
        int32_t longest = 0;
        for (int32_t k = 0; k < n; k++) {
            const uint32_t m = rw::call(SOUND_MATERIAL_DATA, {s, uint32_t(k)});
            const uint32_t samples = rw::call(MATERIAL_NUM_SAMPLES, {m});
            const uint32_t rate = rw::call(MATERIAL_SAMPLE_RATE, {m}) & 0xFFFF;
            const uint64_t ms1000 = uint64_t(samples) * 1000u;
            const int32_t ms = int32_t(uint32_t(rw::call64(LDIVMOD, {uint32_t(ms1000), uint32_t(ms1000 >> 32), rate, 0})));
            if (longest < ms) longest = ms;
        }
        return uint32_t(longest);
    }

    static uint32_t materialGetExtraDataOffset(uint32_t m) {
        if (!isAkb1(m)) return ld16(m + 4);
        const uint32_t header = rw::call(GET_HEADER_SIZE, {m});
        uint32_t extra;
        if (isAkb1(m)) {
            const uint32_t version = rw::call(GET_VERSION, {m});
            extra = version != 0 ? ld16(m + 0x28) : version;
        } else {
            extra = ld16(m + 0xC);
        }
        return extra + header;
    }
    static uint32_t materialGetEnableRandomPan(uint32_t m) {
        if (!isAkb1(m)) return (ld8(m + 3) >> 2) & 1;
        return rw::call(GET_VERSION, {m}) > 1 ? (ld8(m + 0x2B) >> 2) & 1 : 0;
    }
    static uint32_t materialGetRandomVolume(uint32_t out, uint32_t m) {
        return materialRange(out, m, 0x28, 0x3F800000);   // 1.0
    }
    static uint32_t materialGetRandomPitch(uint32_t out, uint32_t m) {
        return materialRange(out, m, 0x30, 0x3F800000);
    }
    static uint32_t materialGetRandomPan(uint32_t out, uint32_t m) { return materialRange(out, m, 0x38, 0); }

    // an Ogg Vorbis material's loop start in bytes (its extra data +4); -1 for an AKB1 material without a loop or
    // not in Ogg Vorbis
    static uint32_t materialExtraOggVorbisGetLoopStartOffsetBytes(uint32_t m) {
        if (isAkb1(m)) {
            const uint32_t loopEnd = isAkb1(m) ? ld32(m + 0x1C) : ld32(m + 0x18);
            if (loopEnd == 0 || rw::call(MATERIAL_AUDIO_FORMAT, {m}) != FORMAT_OGG_VORBIS) return 0xFFFFFFFF;
        }
        return ld32(m + rw::call(MATERIAL_EXTRA_DATA_OFFSET, {m}) + 4);
    }
    // decrypts n bytes: dst[k] = src[k] ^ table[(seed + k) & 0xFF]
    static uint32_t materialDecode(uint32_t seed, uint32_t dst, uint32_t src, uint32_t n) {
        seed &= 0xFF;
        if (n == 0) return seed;
        const uint32_t table = ld32(AKB_DECODE_TABLE_GOT);
        for (uint32_t k = 0; k < n; k++) st8(dst + k, uint8_t(ld8(table + ((seed + k) & 0xFF)) ^ ld8(src + k)));
        return seed;
    }

    // attribute condition: op (high nibble) 0 >=, 1 >, 2 <, 3 <=, 4 ==, 5 != (NaN compares false, except !=)
    static uint32_t attrFloatCmp(uint32_t op, uint32_t aBits, uint32_t bBits) {
        const float a = bitsf(aBits), b = bitsf(bBits);
        switch (op & 0xF0) {
        case 0x00: return a >= b;
        case 0x10: return a > b;
        case 0x20: return a < b;
        case 0x30: return a <= b;
        case 0x40: return a == b;
        case 0x50: return a != b;
        default: return 0;
        }
    }
};
RW_FUNCTION(0xCDC360, "akbIsAkb2File", AkbMore::isAkb2File);
RW_FUNCTION(0xCDC3CC, "akbIsAkbFile", AkbMore::isAkbFile);
RW_FUNCTION(0xCDC508, "akbGetNumSounds", AkbMore::getNumSounds);
RW_FUNCTION(0xCDC534, "akbGetSoundDataOffset", AkbMore::getSoundDataOffset);
RW_FUNCTION(0xCDC5CC, "akbGetSoundOffsetTableSize", AkbMore::getSoundOffsetTableSize);
RW_FUNCTION(0xCDC720, "akbSoundSetWork", AkbMore::soundSetWork);
RW_FUNCTION(0xCDC754, "akbSoundGetRandomValue", AkbMore::soundGetRandomValue);
RW_FUNCTION(0xCDC818, "akbSoundGetMaterialOffsetTableSize", AkbMore::soundGetMaterialOffsetTableSize);
RW_FUNCTION(0xCDC958, "akbMaterialGetExtraDataOffset", AkbMore::materialGetExtraDataOffset);
RW_FUNCTION(0xCDCA34, "akbSoundGetIsLoop", AkbMore::soundGetIsLoop);
RW_FUNCTION(0xCDCAC8, "akbSoundGetPlayTime", AkbMore::soundGetPlayTime);
RW_FUNCTION(0xCDCC68, "akbMaterialGetEnableRandomPan", AkbMore::materialGetEnableRandomPan);
RW_FUNCTION(0xCDCCAC, "akbMaterialGetRandomVolume", AkbMore::materialGetRandomVolume);
RW_FUNCTION(0xCDCD08, "akbMaterialGetRandomPitch", AkbMore::materialGetRandomPitch);
RW_FUNCTION(0xCDCD64, "akbMaterialGetRandomPan", AkbMore::materialGetRandomPan);
RW_FUNCTION(0xCDCE5C, "akbMaterialExtraOggVorbisGetLoopStartOffsetBytes",
            AkbMore::materialExtraOggVorbisGetLoopStartOffsetBytes);
RW_FUNCTION(0xCDCFC0, "akbMaterialDecode", AkbMore::materialDecode);
RW_FUNCTION(0xCDD0D8, "attrFloatCmp", AkbMore::attrFloatCmp);

}  // namespace sqex
