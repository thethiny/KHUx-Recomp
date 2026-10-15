// Free functions of group w3_7: cocos2d's JNI bridges (Cocos2dxHelper / Cocos2dxVideoHelper static methods) and
// the game's BGM stop helper. The stack-protector epilogue of the originals (compare the canary, call
// __stack_chk_fail on mismatch) is not reproduced: the canary never changes.
#include "core/Guest.hpp"

namespace w3_7_misc {

namespace {
constexpr uint32_t GET_STATIC_METHOD_INFO = 0xA4225D;   // cocos2d::JniHelper::getStaticMethodInfo(info, cls, name, sig)
constexpr uint32_t JSTRING_TO_STRING = 0xA423E9;        // cocos2d::JniHelper::jstring2string(std::string* out, jstring)
constexpr uint32_t CALL_STATIC_OBJECT_METHOD = 0x4E0939;   // _JNIEnv::CallStaticObjectMethod(env, cls, mid, ...)
constexpr uint32_t CALL_STATIC_VOID_METHOD = 0x4E0D09;     // _JNIEnv::CallStaticVoidMethod(env, cls, mid, ...)
constexpr uint32_t CALL_STATIC_INT_METHOD = 0x81AB6D;      // _JNIEnv::CallStaticIntMethod(env, cls, mid, ...)
constexpr uint32_t PTHREAD_SELF = 0x414704;                // pthread_self (import)
constexpr uint32_t STRING_INIT = 0x47CD93;                 // std::string::__init(const char*, size_t)
constexpr uint32_t STRING_RESERVE = 0x48D705;              // std::string::reserve(size_t)
constexpr uint32_t AUDIO_GET_INSTANCE = 0x5BECFD;          // audio::Manager::getInstance()
constexpr uint32_t AUDIO_UNLOCK = 0x5BF1ED;                // audio::Manager::unlock(ResourceID)
constexpr uint32_t AUDIO_STOP = 0x5BF3AD;                  // audio::Manager::stop(int id, int fadeFrames)

// JNINativeInterface / JNIInvokeInterface offsets
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C;
constexpr uint32_t JNI_NEW_STRING_UTF = 0x29C;
constexpr uint32_t JVM_DETACH_CURRENT_THREAD = 0x14;

constexpr uint32_t JAVA_VM = 0x1008950;                    // cocos2d::JniHelper::_psJavaVM

constexpr uint32_t STR_EMPTY = 0xDF8A1A;                   // ""
constexpr uint32_t STR_COCOS2DX_HELPER = 0xE10FFB;         // "org/cocos2dx/lib/Cocos2dxHelper"
constexpr uint32_t STR_GET_PACKAGE_NAME = 0xE133D0;        // "getCocos2dxPackageName"
constexpr uint32_t STR_RETURNS_STRING_SIG = 0xDFC8DE;      // "()Ljava/lang/String;"
constexpr uint32_t STR_GET_INTEGER_FOR_KEY = 0xE134D2;     // "getIntegerForKey"
constexpr uint32_t STR_GET_INTEGER_FOR_KEY_SIG = 0xE134E3; // "(Ljava/lang/String;I)I"
constexpr uint32_t STR_VIDEO_HELPER = 0xE0C120;            // "org/cocos2dx/lib/Cocos2dxVideoHelper"
constexpr uint32_t STR_SET_VIDEO_URL = 0xE0C193;           // "setVideoUrl"
constexpr uint32_t STR_SET_VIDEO_URL_SIG = 0xE0C19F;       // "(IILjava/lang/String;)V"

// The game's main BGM: the locked audio resource and the playing sound id.
constexpr uint32_t GAME_MAIN_BGM_RESOURCE = 0x1003C30;
constexpr uint32_t GAME_MAIN_BGM_SOUND = 0x1003C34;

uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }

// cocos2d::JniMethodInfo { JNIEnv* env; jclass classID; jmethodID methodID; } in guest scratch memory
struct MethodInfo {
    uint32_t at;
    uint32_t env() const { return ld32(at); }
    uint32_t cls() const { return ld32(at + 4); }
    uint32_t method() const { return ld32(at + 8); }
};

uint32_t jni(uint32_t env, uint32_t offset, std::initializer_list<uint32_t> args) {
    return rw::call(ld32(ld32(env) + offset), args);
}
void deleteLocalRef(uint32_t env, uint32_t ref) { jni(env, JNI_DELETE_LOCAL_REF, {env, ref}); }
uint32_t newStringUTF(uint32_t env, uint32_t s) { return jni(env, JNI_NEW_STRING_UTF, {env, s}); }
}  // namespace

// Cocos2dxHelper.getCocos2dxPackageName() as a std::string (sret: "" when the method is missing).
void getPackageNameJNI(const guest::String *out) {
    guest::Frame f(0x24);
    const uint32_t result = rw::guest_addr(out);
    const uint32_t converted = f.addr(0x8);   // std::string from jstring2string
    const MethodInfo info{f.addr(0x14)};
    rw::call(STRING_INIT, {result, STR_EMPTY, 0});
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_COCOS2DX_HELPER, STR_GET_PACKAGE_NAME, STR_RETURNS_STRING_SIG}) != 1)
        return;
    uint32_t jname = rw::call(CALL_STATIC_OBJECT_METHOD, {info.env(), info.cls(), info.method()});
    deleteLocalRef(info.env(), info.cls());
    rw::call(JSTRING_TO_STRING, {converted, jname});
    // *out = std::move(converted): clear, shrink, take the representation
    if (out->isLong()) {
        rw::store8(rw::host_ptr<uint8_t>(rw::get<uint32_t>(out, 8)), 0);
        rw::set<uint32_t>(out, 4, 0);
    } else {
        rw::set<uint8_t>(out, 1, 0);
        rw::set<uint8_t>(out, 0, 0);
    }
    rw::call(STRING_RESERVE, {result, 0});
    for (uint32_t k = 0; k < 12; k += 4) rw::set<uint32_t>(out, k, ld32(converted + k));
    for (uint32_t k = 0; k < 12; k += 4) *rw::frame_ptr<uint32_t>(converted + k) = 0;
    deleteLocalRef(info.env(), jname);
}
RW_FUNCTION(0xA40DE4, "getPackageNameJNI()", getPackageNameJNI);

// Cocos2dxHelper.getIntegerForKey(key, defaultValue) (UserDefault on Android).
int32_t getIntegerForKeyJNI(uint32_t key, int32_t defaultValue) {
    guest::Frame f(0x1C);
    const MethodInfo info{f.addr(0xC)};
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_COCOS2DX_HELPER, STR_GET_INTEGER_FOR_KEY,
                                          STR_GET_INTEGER_FOR_KEY_SIG}) != 1)
        return defaultValue;
    uint32_t jkey = newStringUTF(info.env(), key);
    int32_t value = int32_t(rw::call(CALL_STATIC_INT_METHOD, {info.env(), info.cls(), info.method(), jkey,
                                                              uint32_t(defaultValue)}));
    deleteLocalRef(info.env(), info.cls());
    deleteLocalRef(info.env(), jkey);
    return value;
}
RW_FUNCTION(0xA414B8, "getIntegerForKeyJNI(const char*, int)", getIntegerForKeyJNI);

// Cocos2dxVideoHelper.setVideoUrl(index, videoSource, url)
void setVideoURLJNI(int32_t index, int32_t videoSource, const guest::String *url) {
    guest::Frame f(0x24);
    const MethodInfo info{f.addr(0x14)};
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_VIDEO_HELPER, STR_SET_VIDEO_URL, STR_SET_VIDEO_URL_SIG}) != 1)
        return;
    uint32_t jurl = newStringUTF(info.env(), url->data());
    rw::call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method(), uint32_t(index), uint32_t(videoSource),
                                       jurl});
    deleteLocalRef(info.env(), info.cls());
    deleteLocalRef(info.env(), jurl);
}
RW_FUNCTION(0x843854, "setVideoURLJNI(int, int, const std::string&)", setVideoURLJNI);

// pthread key destructor: detach the exiting thread from the Java VM. r0 = DetachCurrentThread's result.
uint32_t detachCurrentThread(uint32_t /*unused*/) {
    rw::call(PTHREAD_SELF, {});
    uint32_t vm = ld32(JAVA_VM);
    return rw::call(ld32(ld32(vm) + JVM_DETACH_CURRENT_THREAD), {vm});
}
RW_FUNCTION(0xA41F48, "_detachCurrentThread(void*)", detachCurrentThread);

// Stop the game's main BGM (if one is playing) with a fade of `fadeFrames`, releasing its locked resource.
void AudioStopGameMainBGM(int32_t fadeFrames) {
    uint32_t resource = ld32(GAME_MAIN_BGM_RESOURCE);
    if (!resource) return;
    rw::call(AUDIO_UNLOCK, {rw::call(AUDIO_GET_INSTANCE, {}), resource});
    uint32_t manager = rw::call(AUDIO_GET_INSTANCE, {});
    rw::call(AUDIO_STOP, {manager, ld32(GAME_MAIN_BGM_SOUND), uint32_t(fadeFrames)});
    rw::save<uint32_t>(GAME_MAIN_BGM_RESOURCE, 0);
    rw::save<uint32_t>(GAME_MAIN_BGM_SOUND, 0);
}
RW_FUNCTION(0x5C0F58, "AudioStopGameMainBGM(int)", AudioStopGameMainBGM);

// ------------------------------------------------------------------------------------------------ LoadDataFromFile
namespace {
constexpr uint32_t FILEUTILS_GET_INSTANCE = 0xA3F44D;   // cocos2d::FileUtils::getInstance()
constexpr uint32_t FILEUTILS_GET_WRITABLE_PATH_SLOT = 17;  // FileUtils::getWritablePath() const (sret std::string)
constexpr uint32_t STRING_APPEND = 0x47AC89;            // std::string::append(const char*, size_t) -> string&
constexpr uint32_t FOPEN = 0x414158, FSEEK = 0x4146D4, FTELL = 0x4146E0, FREAD = 0x414308, FCLOSE = 0x414170;
constexpr uint32_t MALLOC = 0x40FF58;                   // malloc (import)
constexpr uint32_t MAKE_SHARED_DATA = 0x4D7929;         // make_shared<bg::Data>(unsigned char*&, unsigned&, void(&)(void*))
constexpr uint32_t FREE_GOT = 0xFD64C0;                 // GOT slot: &free
constexpr uint32_t STR_SAVE_FILE = 0xDFCACD;            // "s000.gif" (the save file's disguised name)
constexpr uint32_t STR_RB = 0xDFC3A6;                   // "rb"
}  // namespace

// The whole save file (<writable path>/s000.gif) as a shared_ptr<bg::Data> (malloc'ed, freed with free), or null.
void LoadDataFromFile(guest::SharedPtr *out) {
    guest::Frame f(0x3C);
    const uint32_t made = f.addr(0x10), data = f.addr(0x18), size = f.addr(0x1C);
    const uint32_t dir = f.addr(0x20), path = f.addr(0x2C);
    out->obj = 0;
    out->ctrl = 0;
    uint32_t files = rw::call(FILEUTILS_GET_INSTANCE, {});
    rw::vcall(rw::host_ptr<uint8_t>(files), FILEUTILS_GET_WRITABLE_PATH_SLOT, {dir, files});
    // path = std::move(dir.append("s000.gif"))
    uint32_t appended = rw::call(STRING_APPEND, {dir, STR_SAVE_FILE, 8});
    for (uint32_t k = 0; k < 12; k += 4) *rw::frame_ptr<uint32_t>(path + k) = ld32(appended + k);
    for (uint32_t k = 0; k < 12; k += 4) rw::save<uint32_t>(appended + k, 0);
    if (rw::load<uint8_t>(dir) & 1) guest::deleteObject(ld32(dir + 8));
    const bool longPath = rw::load<uint8_t>(path) & 1;
    uint32_t file = rw::call(FOPEN, {longPath ? ld32(path + 8) : path + 1, STR_RB});
    if (file) {
        rw::call(FSEEK, {file, 0, 2});
        uint32_t length = rw::call(FTELL, {file});
        *rw::frame_ptr<uint32_t>(size) = length;
        rw::call(FSEEK, {file, 0, 0});
        *rw::frame_ptr<uint32_t>(data) = rw::call(MALLOC, {length});
        if (rw::call(FREAD, {ld32(data), length, 1, file}) == 1) {
            rw::call(MAKE_SHARED_DATA, {made, data, size, ld32(FREE_GOT)});
            uint32_t ctrl = ld32(made + 4), obj = ld32(made);
            out->obj = obj;
            out->ctrl = ctrl;
        }
        rw::call(FCLOSE, {file});
    }
    if (longPath) guest::deleteObject(ld32(path + 8));
}
RW_FUNCTION(0x4E87A8, "LoadDataFromFile()", LoadDataFromFile);

// ------------------------------------------------------------------------------------------------ s3tc_decode
namespace {
constexpr uint32_t S3TC_DECODE_BLOCK = 0x908D31;   // static s3tc_decode_block(uint8_t** data, uint32_t* dst, int width,
                                                   //   bool hasColor?, uint64_t alpha/color words..., S3TCDecodeFlag)
enum S3TCDecodeFlag : int32_t { DXT1 = 1, DXT3 = 3, DXT5 = 5 };
uint32_t le32(uint32_t p) {   // four bytes read one at a time (the data need not be aligned)
    return uint32_t(rw::load<uint8_t>(p)) | uint32_t(rw::load<uint8_t>(p + 1)) << 8 |
           uint32_t(rw::load<uint8_t>(p + 2)) << 16 | uint32_t(rw::load<uint8_t>(p + 3)) << 24;
}
}  // namespace

// cocos2d's S3TC (DXT1/3/5) decoder: decodes `height / 4` rows of `width / 4` 4x4 blocks into 32-bit pixels.
// For DXT3/5 the 8-byte alpha block is read here and passed on; the block decoder reads the colour block.
void s3tc_decode(uint32_t encodeData, uint32_t decodeData, int32_t width, int32_t height, int32_t flag) {
    guest::Frame f(0x54);
    const uint32_t cursor = f.addr(0x4C);   // uint8_t* advanced by the block decoder
    *rw::frame_ptr<uint32_t>(cursor) = encodeData;
    const int32_t rows = height / 4;
    if (height < 4) return;
    const int32_t blocksPerRow = width / 4;
    const uint32_t rowSkip = width <= 7 ? 4u : uint32_t(width) & ~3u;   // (width / 4) * 4, at least 4
    const uint32_t rowPixels = uint32_t(3 * width);
    uint32_t row = decodeData;
    for (int32_t y = 0; y < rows; y++) {
        if (width >= 4) {
            uint32_t dst = row;
            for (int32_t x = 0; x < blocksPerRow; x++, dst += 16) {
                if (flag == DXT5 || flag == DXT3) {
                    uint32_t p = ld32(cursor);
                    uint32_t alpha0 = le32(p), alpha1 = le32(p + 4);
                    *rw::frame_ptr<uint32_t>(cursor) = p + 8;
                    rw::call(S3TC_DECODE_BLOCK, {cursor, dst, uint32_t(width), 1, alpha0, alpha1, uint32_t(flag)});
                } else if (flag == DXT1) {
                    rw::call(S3TC_DECODE_BLOCK, {cursor, dst, uint32_t(width), 0, 0, 0, 1});
                }
            }
            row += rowSkip * 4;
        }
        row += rowPixels * 4;
    }
}
RW_FUNCTION(0x908BCC, "s3tc_decode(unsigned char*, unsigned char*, int, int, S3TCDecodeFlag)", s3tc_decode);

}  // namespace w3_7_misc
