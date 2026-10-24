// Free functions of group w3_9: the main-BGM stack (AudioPushGameMainBGM), the game frame size, the BTF image
// decoder, a few cocos2d JNI bridges (Cocos2dxHelper / Cocos2dxVideoHelper) and an OpenSL ES debug dump.
// The stack-protector epilogue of the originals (compare the canary, call __stack_chk_fail on mismatch) is not
// reproduced: the canary never changes.
#include "core/Guest.hpp"

namespace w3_9_misc {

namespace {
constexpr uint32_t MEMMOVE = 0x40EF50;                    // memmove (import)
constexpr uint32_t MEMCPY = 0x40E164;                     // __aeabi_memcpy(dst, src, n)
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;               // __aeabi_memset(dst, n, c)
constexpr uint32_t MEMCMP = 0x40EF2C;                     // memcmp
constexpr uint32_t MALLOC = 0x40FF58;                     // malloc
constexpr uint32_t ANDROID_LOG_PRINT = 0x413D38;          // __android_log_print
constexpr uint32_t UNCOMPRESS = rwlib::uncompress;                 // zlib uncompress(dst, &dstLen, src, srcLen)

constexpr uint32_t AUDIO_GET_INSTANCE = 0x5BECFD;         // audio::Manager::getInstance()
constexpr uint32_t AUDIO_PAUSE = 0x5BF425;                // audio::Manager::pause(int sound, int fadeFrames)
constexpr uint32_t AUDIO_PLAY_GAME_MAIN_BGM = 0x5C0E91;   // AudioPlayGameMainBGM(audio::ResourceID, int)
constexpr uint32_t SPLIT_BUFFER_DTOR = 0x5C1FB1;          // __split_buffer<pair<int,int>*>::~__split_buffer (sub_5C1FB0)

constexpr uint32_t DIRECTOR_GET_INSTANCE = rwlib::cocos2d::Director::getInstance;      // cocos2d::Director::getInstance()
constexpr uint32_t DIRECTOR_GET_WIN_SIZE = rwlib::cocos2d::Director::getWinSize;      // cocos2d::Director::getWinSize() const
constexpr uint32_t SIZE_COPY = rwlib::cocos2d::Size::ctor_base__RKS0_;                  // cocos2d::Size::Size(const Size&)

constexpr uint32_t GET_STATIC_METHOD_INFO = rwlib::cocos2d::JniHelper::getStaticMethodInfo;     // cocos2d::JniHelper::getStaticMethodInfo(info, cls, name, sig)
constexpr uint32_t JSTRING_TO_STRING = rwlib::cocos2d::JniHelper::jstring2string;          // cocos2d::JniHelper::jstring2string(out std::string, jstring)
constexpr uint32_t CALL_STATIC_OBJECT_METHOD = 0x4E0939;  // _JNIEnv::CallStaticObjectMethod(env, cls, mid, ...)
constexpr uint32_t CALL_STATIC_DOUBLE_METHOD = 0xA416D9;  // _JNIEnv::CallStaticDoubleMethod(env, cls, mid, ...)
constexpr uint32_t CALL_VIDEO_NON_PARAMETER_FUN = 0x8436A1;   // callVideoNonParameterFun(int index, const char* method)
constexpr uint32_t STRING_INIT = 0x47CD93;                // std::string::__init(const char*, size_t) (sub_47CD92)
constexpr uint32_t STRING_SHRINK = 0x48D705;              // std::string::reserve(size_t) (sub_48D704; 0 = shrink_to_fit)

// JNINativeInterface offsets
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C;
constexpr uint32_t JNI_NEW_STRING_UTF = 0x29C;

// strings
constexpr uint32_t STR_EMPTY = 0xDF8A1A;                  // ""
constexpr uint32_t STR_COCOS2DX_HELPER = 0xE10FFB;        // "org/cocos2dx/lib/Cocos2dxHelper"
constexpr uint32_t STR_GET_CACHE_PATH = 0xE133FF;         // "getCocos2dxCachePath"
constexpr uint32_t STR_STRING_SIG = 0xDFC8DE;             // "()Ljava/lang/String;"
constexpr uint32_t STR_GET_DOUBLE_FOR_KEY = 0xE13520;     // "getDoubleForKey"
constexpr uint32_t STR_GET_DOUBLE_FOR_KEY_SIG = 0xE13530; // "(Ljava/lang/String;D)D"
constexpr uint32_t STR_PAUSE_VIDEO = 0xE0C1C2;            // "pauseVideo"
constexpr uint32_t STR_BTF_MAGIC = 0xEBD7B0;              // "\x89" "BTF\0\0"
constexpr uint32_t STR_SQEXSDLIB = 0xEECB88;              // "sqexsdlib"
constexpr uint32_t STR_ITF_SUPPORTED = 0xEEDE1C;          // "%s %d itf supported"
constexpr uint32_t STR_ITF_NAMED = 0xEEDE30;              // "--[%d]%s"
constexpr uint32_t STR_ITF_UNKNOWN = 0xEEDE3C;            // "--[%d]%x-%x"

uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
uint32_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
uint32_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }

uint32_t jni(uint32_t env, uint32_t offset, std::initializer_list<uint32_t> args) {
    return rw::call(ld32(ld32(env) + offset), args);
}
void deleteLocalRef(uint32_t env, uint32_t ref) { jni(env, JNI_DELETE_LOCAL_REF, {env, ref}); }
}  // namespace

// ================================================================================================ main BGM stack
// The main BGMs pushed over each other: std::deque<pair<ResourceID resource, int sound>> (512 per block). The
// push_back is inlined into AudioPushGameMainBGM, including the block map (a __split_buffer of block pointers)
// growth; it is reproduced here on the same memory.
namespace {
constexpr uint32_t GAME_MAIN_BGM_RESOURCE = 0x1003C30;    // the playing main BGM's resource (0 = none)
constexpr uint32_t GAME_MAIN_BGM_SOUND = 0x1003C34;       // its sound id
constexpr uint32_t BGM_STACK_MAP = 0x1003C64;             // deque block map: first, begin, end, end of capacity
constexpr uint32_t BGM_STACK_START = 0x1003C74;           // index of the first element
constexpr uint32_t BGM_STACK_SIZE = 0x1003C78;            // number of elements
constexpr uint32_t BLOCK_ELEMENTS = 512;                  // 8-byte elements per 4096-byte block

// A __split_buffer<T*> (first, begin, end, end of capacity) at a guest address (the deque's map or a temporary
// in the frame).
struct SplitBuffer {
    uint32_t at;
    uint32_t first() const { return ld32(at); }
    uint32_t begin() const { return ld32(at + 4); }
    uint32_t end() const { return ld32(at + 8); }
    uint32_t cap() const { return ld32(at + 12); }
    void setFirst(uint32_t v) const { rw::save<uint32_t>(at, v); }
    void setBegin(uint32_t v) const { rw::save<uint32_t>(at + 4, v); }
    void setEnd(uint32_t v) const { rw::save<uint32_t>(at + 8, v); }
    void setCap(uint32_t v) const { rw::save<uint32_t>(at + 12, v); }
};

// Reallocate b with twice its capacity (at least 1), the elements starting a quarter in (push_back: rounded
// down, push_front: rounded up); the old storage goes through tmp and its destructor.
void growSplitBuffer(const SplitBuffer &b, bool forFront, const SplitBuffer &tmp) {
    uint32_t count = (b.cap() - b.first()) >> 1;
    if (!count) count = 1;
    const uint32_t storage = guest::newObject(4 * count);
    const uint32_t start = storage + (forFront ? ((count + 3) & ~3u) : (count & ~3u));
    tmp.setFirst(storage);
    tmp.setBegin(start);
    tmp.setEnd(start);
    tmp.setCap(storage + 4 * count);
    for (uint32_t p = b.begin(); p != b.end(); p += 4) {
        rw::save<uint32_t>(tmp.end(), ld32(p));
        tmp.setEnd(tmp.end() + 4);
    }
    const uint32_t oldFirst = b.first(), oldBegin = b.begin(), oldEnd = b.end(), oldCap = b.cap();
    b.setFirst(tmp.first());
    b.setBegin(tmp.begin());
    b.setEnd(tmp.end());
    b.setCap(tmp.cap());
    tmp.setFirst(oldFirst);
    tmp.setBegin(oldBegin);
    tmp.setEnd(oldEnd);
    tmp.setCap(oldCap);
    rw::call(SPLIT_BUFFER_DTOR, {tmp.at});
}

void splitPushBack(const SplitBuffer &b, uint32_t value, const SplitBuffer &tmp) {
    if (b.end() == b.cap()) {
        if (b.begin() > b.first()) {   // slide the elements to the front
            const int32_t d = ((int32_t(b.begin() - b.first()) >> 2) + 1) / 2;
            const uint32_t n = b.end() - b.begin();
            const uint32_t dst = b.begin() - 4 * uint32_t(d);
            rw::call(MEMMOVE, {dst, b.begin(), n});
            b.setEnd(dst + 4 * (n >> 2));
            b.setBegin(b.begin() - 4 * uint32_t(d));
        } else {
            growSplitBuffer(b, false, tmp);
        }
    }
    rw::save<uint32_t>(b.end(), value);
    b.setEnd(b.end() + 4);
}

void splitPushFront(const SplitBuffer &b, uint32_t value, const SplitBuffer &tmp) {
    if (b.begin() == b.first()) {
        if (b.end() < b.cap()) {   // slide the elements to the back
            const int32_t d = ((int32_t(b.cap() - b.end()) >> 2) + 1) / 2;
            const uint32_t n = b.end() - b.begin();
            const uint32_t dst = b.end() + 4 * (uint32_t(d) - (n >> 2));
            rw::call(MEMMOVE, {dst, b.begin(), n});
            b.setBegin(dst);
            b.setEnd(b.end() + 4 * uint32_t(d));
        } else {
            growSplitBuffer(b, true, tmp);
        }
    }
    rw::save<uint32_t>(b.begin() - 4, value);
    b.setBegin(b.begin() - 4);
}
}  // namespace

// Push the playing main BGM (resource, sound) on the BGM stack, then switch to `resource`: when it differs from
// the playing one, the playing sound is paused (fadeFrames) and the new BGM started. Returns the sound id.
// Not shadow-run: audio::Manager::pause reaches SdSoundSystem_SoundCtrl_SetPause (the sound output), which is not
// an effect the verifier can replay.
uint32_t AudioPushGameMainBGM(uint32_t resource, int32_t fadeFrames) {
    guest::Frame f(0x88);   // push {r4-r7, lr}; sub sp, #0x74
    const SplitBuffer map{BGM_STACK_MAP};
    const SplitBuffer newMap{f.addr(0x24)}, tmpBack{f.addr(0x60)}, tmpFront{f.addr(0x38)}, tmp{f.addr(0x4C)};
    const uint32_t playingResource = ld32(GAME_MAIN_BGM_RESOURCE), playingSound = ld32(GAME_MAIN_BGM_SOUND);

    // deque::push_back: __add_back_capacity when there is no spare slot at the back
    const uint32_t capacity = map.end() == map.begin() ? 0 : ((map.end() - map.begin()) << 7) - 1;
    if (capacity - ld32(BGM_STACK_START) == ld32(BGM_STACK_SIZE)) {
        if (ld32(BGM_STACK_START) >= BLOCK_ELEMENTS) {
            // a whole free block at the front: rotate it to the back
            rw::save<uint32_t>(BGM_STACK_START, ld32(BGM_STACK_START) - BLOCK_ELEMENTS);
            const uint32_t block = ld32(map.begin());
            map.setBegin(map.begin() + 4);
            splitPushBack(map, block, tmp);
        } else if (((map.end() - map.begin()) >> 2) < ((map.cap() - map.first()) >> 2)) {
            // room in the map: a new block at the back (or at the front, rotated to the back)
            const uint32_t block = guest::newObject(BLOCK_ELEMENTS * 8);
            if (map.cap() != map.end()) {
                splitPushBack(map, block, tmp);
            } else {
                splitPushFront(map, block, tmp);
                const uint32_t front = ld32(map.begin());
                map.setBegin(map.begin() + 4);
                splitPushBack(map, front, tmp);
            }
        } else {
            // a new map of twice the capacity: the new block, then the old block pointers in front of it
            uint32_t count = (map.cap() - map.first()) >> 1;
            if (!count) count = 1;
            const uint32_t used = (map.end() - map.begin()) >> 2;
            const uint32_t storage = guest::newObject(4 * count);
            newMap.setFirst(storage);
            newMap.setBegin(storage + 4 * used);
            newMap.setEnd(storage + 4 * used);
            newMap.setCap(storage + 4 * count);
            const uint32_t block = guest::newObject(BLOCK_ELEMENTS * 8);
            splitPushBack(newMap, block, tmpBack);
            for (uint32_t p = map.end(); p != map.begin();) {
                p -= 4;
                splitPushFront(newMap, ld32(p), tmpFront);
            }
            const uint32_t oldFirst = map.first(), oldBegin = map.begin(), oldEnd = map.end(), oldCap = map.cap();
            map.setFirst(newMap.first());
            map.setBegin(newMap.begin());
            map.setEnd(newMap.end());
            map.setCap(newMap.cap());
            newMap.setFirst(oldFirst);
            newMap.setBegin(oldBegin);
            newMap.setEnd(oldEnd);
            newMap.setCap(oldCap);
            rw::call(SPLIT_BUFFER_DTOR, {newMap.at});
        }
    }
    const uint32_t index = ld32(BGM_STACK_SIZE) + ld32(BGM_STACK_START);
    if (map.end() != map.begin()) {
        const uint32_t slot = ld32(map.begin() + ((index >> 7) & 0x1FFFFFC)) + (index & 0x1FF) * 8;
        if (slot) {
            rw::save<uint32_t>(slot, playingResource);
            rw::save<uint32_t>(slot + 4, playingSound);
        }
    }
    rw::save<uint32_t>(BGM_STACK_SIZE, ld32(BGM_STACK_SIZE) + 1);

    if (ld32(GAME_MAIN_BGM_RESOURCE) == resource) return ld32(GAME_MAIN_BGM_SOUND);
    const uint32_t manager = rw::call(AUDIO_GET_INSTANCE, {});
    rw::call(AUDIO_PAUSE, {manager, ld32(GAME_MAIN_BGM_SOUND), uint32_t(fadeFrames)});
    rw::save<uint32_t>(GAME_MAIN_BGM_RESOURCE, 0);
    return rw::call(AUDIO_PLAY_GAME_MAIN_BGM, {resource, uint32_t(fadeFrames)});
}
RW_FUNCTION_NOVERIFY(0x5C0FE4, "AudioPushGameMainBGM(audio::ResourceID, int)", AudioPushGameMainBGM);

// ================================================================================================ frame size
// The game's frame (design resolution) for the window's aspect ratio, computed once: 1136x640 for 16:9 and wider,
// 960x720 for 4:3 and narrower, else 640 high (ratio at least 1.5) or 960 wide. Returns the height.
namespace {
constexpr uint32_t FRAME_SIZE_CACHED = 0x1005520;         // byte: computed
constexpr uint32_t FRAME_WIDTH = 0x1005524;               // float
constexpr uint32_t FRAME_HEIGHT = 0x1005528;              // float
}  // namespace

float AppGetGrameFrameHeight() {
    if (ld8(FRAME_SIZE_CACHED) == 1) return rw::bitsf(ld32(FRAME_HEIGHT));
    guest::Frame f(0x38);   // push {r4-r7, lr}; sub sp, #0x24
    const uint32_t size = f.addr(0x18);
    const uint32_t winSize = rw::call(DIRECTOR_GET_WIN_SIZE, {rw::call(DIRECTOR_GET_INSTANCE, {})});
    rw::call(SIZE_COPY, {size, winSize});
    const double ratio = double(rw::bitsf(ld32(size))) / double(rw::bitsf(ld32(size + 4)));
    float width, height;
    if (ratio >= 1.775) {
        width = 1136.0f;
        height = 640.0f;
    } else if (ratio <= 1.3333333333333333) {
        width = 960.0f;
        height = 720.0f;
    } else if (!(ratio < 1.5)) {   // also NaN (the compares are "less than, or unordered")
        width = float(ratio * 640.0);
        height = 640.0f;
    } else {
        width = 960.0f;
        height = float(960.0 / ratio);
    }
    rw::save<uint32_t>(FRAME_WIDTH, rw::fbits(width));
    rw::save<uint32_t>(FRAME_HEIGHT, rw::fbits(height));
    rw::save<uint8_t>(FRAME_SIZE_CACHED, 1);
    return height;
}
RW_FUNCTION(0x799D20, "AppGetGrameFrameHeight()", AppGetGrameFrameHeight);

// ================================================================================================ BTF images
// btf_read(data, size, &width, &height, &pixels, &byteCount, &flag): decode a BTF image (header "\x89BTF") into
// a malloc'ed RGBA buffer of width x height. The payload (optionally zlib-compressed) is either a palette (4 bytes
// per color) followed by 4-bit or 8-bit indices, or raw 32-bit pixels; it covers a cols x rows rectangle placed at
// (dstX, dstY) inside the image. Returns 1 on success, 0 when the data is not a BTF image.
namespace {
constexpr uint32_t BTF_PALETTE = 0x01;      // a palette count follows the header
constexpr uint32_t BTF_COMPRESSED = 0x08;   // the payload is zlib-compressed
constexpr uint32_t BTF_4BIT = 0x10;         // 4-bit palette indices (low nibble first)
}  // namespace

uint32_t btf_read(uint32_t data, int32_t size, uint32_t outWidth, uint32_t outHeight, uint32_t outPixels,
                  uint32_t outBytes, uint32_t outFlag) {
    guest::Frame f(0x70);   // push {r4-r7, lr}; sub sp, #0x5c
    if (size < 4 || rw::call(MEMCMP, {STR_BTF_MAGIC, data, 6}) != 0) return 0;
    // header: little-endian 16-bit fields
    const uint32_t rows = ld16(data + 0x20);
    const uint32_t flags = ld16(data + 0x10);
    const uint32_t cols = ld16(data + 0x1E);
    const uint32_t dstY = ld16(data + 0x1C);
    const uint32_t dstX = ld16(data + 0x1A);
    const uint32_t height = ld16(data + 0x18);
    const uint32_t width = ld16(data + 0x16);
    const uint32_t imageFlags = ld16(data + 0x06);
    uint32_t sizeField = data + 0x22, payloadOffset = 0x26, paletteCount = 0;
    if (flags & BTF_PALETTE) {
        paletteCount = ld16(data + 0x22);
        sizeField = data + 0x24;
        payloadOffset = 0x28;
    }
    const uint32_t payloadSize = ld16(sizeField) | (ld16(sizeField + 2) << 16);
    const uint32_t payload = guest::newArray(payloadSize);
    rw::call(MEMCPY, {payload, data + payloadOffset, payloadSize});
    const uint32_t byteCount = height * width * 4;
    const uint32_t pixels = rw::call(MALLOC, {byteCount});
    rw::call(AEABI_MEMSET, {pixels, byteCount, 0});

    const uint32_t destLen = f.addr(0x54);
    uint32_t src;
    if (flags & BTF_COMPRESSED) {
        const uint32_t len = paletteCount ? paletteCount * 4 + cols * rows : rows * cols * 4;
        rw::save<uint32_t>(destLen, len);
        src = guest::newArray(len);
        rw::call(UNCOMPRESS, {src, destLen, payload, payloadSize});
        guest::deleteArray(payload);
    } else {
        rw::save<uint32_t>(destLen, payloadSize);
        src = payload;
    }

    const uint32_t stride = width * 4;
    const uint32_t origin = pixels + (dstY * width + dstX) * 4;
    if (flags & BTF_4BIT) {
        // indices after the palette, two per byte (low nibble first); the nibble parity carries across rows
        uint32_t in = src + paletteCount * 4, row = origin;
        uint32_t lowNibble = 1;
        const uint32_t rowParity = (cols - 1) & 1;
        for (uint32_t y = 0; rows; y++) {
            if (cols) {
                uint32_t toggle = lowNibble;
                for (uint32_t x = 0; x != cols; x++) {
                    uint32_t b = ld8(in), index;
                    if (toggle & 1) {
                        index = b & 0xF;
                    } else {
                        in++;
                        index = b >> 4;
                    }
                    rw::save<uint32_t>(row + 4 * x, ld32(src + index * 4));
                    toggle ^= 1;
                }
                lowNibble ^= rowParity ? 0 : 1;
            }
            row += stride;
            if (y == rows - 1) break;
        }
    } else if (paletteCount) {
        // 8-bit indices after the palette
        if (rows) {
            uint32_t in = src + paletteCount * 4, row = origin;
            for (uint32_t y = 0;; y++) {
                for (uint32_t x = 0; x != cols; x++) rw::save<uint32_t>(row + 4 * x, ld32(src + ld8(in + x) * 4));
                row += stride;
                in += cols;
                if (y == rows - 1) break;
            }
        }
    } else {
        // raw 32-bit pixels, one row at a time
        uint32_t in = src, row = origin;
        for (uint32_t y = rows; y; y--) {
            rw::call(MEMCPY, {row, in, cols * 4});
            in += cols * 4;
            row += stride;
        }
    }
    guest::deleteArray(src);
    rw::save<uint32_t>(outWidth, width);
    rw::save<uint32_t>(outHeight, height);
    rw::save<uint32_t>(outPixels, pixels);
    rw::save<uint32_t>(outBytes, byteCount);
    rw::save<uint8_t>(outFlag, uint8_t(imageFlags & 1));
    return 1;
}
RW_FUNCTION(0x937494, "btf_read(unsigned char*, int, int*, int*, unsigned char**, int*, bool*)", btf_read);

// ================================================================================================ JNI bridges
// The JNI calls run Java code and make local references outside guest memory: not shadow-run.

// Cocos2dxHelper.getCocos2dxCachePath() as std::string (sret out); "" when the method is missing.
void getCacheDirectoryJNI(uint32_t out) {
    guest::Frame f(0x38);   // push {r4-r7, lr}; sub sp, #0x24
    const uint32_t result = f.addr(0x08);   // std::string from jstring2string
    const uint32_t info = f.addr(0x14);     // JniMethodInfo {env, classID, methodID}
    rw::call(STRING_INIT, {out, STR_EMPTY, 0});
    if (rw::call(GET_STATIC_METHOD_INFO, {info, STR_COCOS2DX_HELPER, STR_GET_CACHE_PATH, STR_STRING_SIG}) != 1)
        return;
    const uint32_t jpath = rw::call(CALL_STATIC_OBJECT_METHOD, {ld32(info), ld32(info + 4), ld32(info + 8)});
    deleteLocalRef(ld32(info), ld32(info + 4));
    rw::call(JSTRING_TO_STRING, {result, jpath});
    // out = std::move(result): clear, shrink, take the representation
    if (ld8(out) & 1) {
        rw::save<uint8_t>(ld32(out + 8), 0);
        rw::save<uint32_t>(out + 4, 0);
    } else {
        rw::save<uint8_t>(out + 1, 0);
        rw::save<uint8_t>(out, 0);
    }
    rw::call(STRING_SHRINK, {out, 0});
    rw::save<uint32_t>(out, ld32(result));
    rw::save<uint32_t>(out + 4, ld32(result + 4));
    rw::save<uint32_t>(out + 8, ld32(result + 8));
    rw::save<uint32_t>(result, 0);
    rw::save<uint32_t>(result + 4, 0);
    rw::save<uint32_t>(result + 8, 0);
    deleteLocalRef(ld32(info), jpath);
}
RW_FUNCTION_NOVERIFY(0xA40F8C, "getCacheDirectoryJNI()", getCacheDirectoryJNI);

// Cocos2dxHelper.getDoubleForKey(key, defaultValue); defaultValue when the method is missing.
double getDoubleForKeyJNI(uint32_t key, double defaultValue) {
    guest::Frame f(0x30);   // push {r4-r7, lr}; sub sp, #0x1c
    const uint32_t info = f.addr(0x0C);
    if (rw::call(GET_STATIC_METHOD_INFO, {info, STR_COCOS2DX_HELPER, STR_GET_DOUBLE_FOR_KEY,
                                          STR_GET_DOUBLE_FOR_KEY_SIG}) != 1)
        return defaultValue;
    const uint32_t env = ld32(info);
    const uint32_t jkey = jni(env, JNI_NEW_STRING_UTF, {env, key});
    uint64_t bits;
    std::memcpy(&bits, &defaultValue, 8);
    // the double goes on the stack (r3 holds the key): words 5 and 6
    const uint64_t r = rw::call64(CALL_STATIC_DOUBLE_METHOD,
                                  {env, ld32(info + 4), ld32(info + 8), jkey, uint32_t(bits), uint32_t(bits >> 32)});
    deleteLocalRef(ld32(info), ld32(info + 4));
    deleteLocalRef(ld32(info), jkey);
    double value;
    std::memcpy(&value, &r, 8);
    return value;
}
RW_FUNCTION_NOVERIFY(0xA41620, "getDoubleForKeyJNI(const char*, double)", getDoubleForKeyJNI);

// Cocos2dxVideoHelper.pauseVideo(index)
void pauseVideoJNI(int32_t index) { rw::call(CALL_VIDEO_NON_PARAMETER_FUN, {uint32_t(index), STR_PAUSE_VIDEO}); }
RW_FUNCTION_NOVERIFY(0x843918, "pauseVideoJNI(int)", pauseVideoJNI);

// ================================================================================================ OpenSL ES
// Log the interfaces an OpenSL ES object type supports (sqexsdlib debug output): known interface IDs by name from
// the IID table, others as their first word and half-word. Not shadow-run: it queries the OpenSL ES engine.
namespace {
constexpr uint32_t SL_ENGINE_OBJECT = 0x100E76C;          // the engine object (SLObjectItf)
constexpr uint32_t SL_IID_ENGINE_GOT = 0xFD9354;          // GOT slot of &SL_IID_ENGINE
constexpr uint32_t IID_NAMES = 0x1002C00;                 // 49 entries {SLInterfaceID id; char name[32]}
constexpr uint32_t IID_NAME_COUNT = 49;
constexpr uint32_t IID_NAME_STRIDE = 36;
}  // namespace

void LogSupportedInterfaces(uint32_t objectId, uint32_t label) {
    guest::Frame f(0x40);   // push {r4-r11, lr}; sub sp, #0x1c
    const uint32_t engineItf = f.addr(0x0C), count = f.addr(0x10), iid = f.addr(0x14);
    const uint32_t engineObject = ld32(SL_ENGINE_OBJECT);
    rw::call(ld32(ld32(engineObject) + 0x0C), {engineObject, ld32(ld32(SL_IID_ENGINE_GOT)), engineItf});   // GetInterface
    uint32_t engine = ld32(engineItf);
    rw::call(ld32(ld32(engine) + 0x28), {engine, objectId, count});   // QueryNumSupportedInterfaces
    rw::call(ANDROID_LOG_PRINT, {4, STR_SQEXSDLIB, STR_ITF_SUPPORTED, label, ld32(count)});
    for (uint32_t i = 0; i < ld32(count); i++) {
        engine = ld32(engineItf);
        rw::call(ld32(ld32(engine) + 0x2C), {engine, objectId, i, iid});   // QuerySupportedInterfaces
        const uint32_t id = ld32(iid);
        uint32_t k = 0;
        while (k != IID_NAME_COUNT && ld32(IID_NAMES + k * IID_NAME_STRIDE) != id) k++;
        if (k != IID_NAME_COUNT)
            rw::call(ANDROID_LOG_PRINT, {4, STR_SQEXSDLIB, STR_ITF_NAMED, i, IID_NAMES + k * IID_NAME_STRIDE + 4});
        else
            rw::call(ANDROID_LOG_PRINT, {4, STR_SQEXSDLIB, STR_ITF_UNKNOWN, i, ld32(id), ld16(id + 4)});
    }
}
RW_FUNCTION_NOVERIFY(0xCE80DC, "LogSupportedInterfaces(unsigned long, const char*)", LogSupportedInterfaces);

}  // namespace w3_9_misc
