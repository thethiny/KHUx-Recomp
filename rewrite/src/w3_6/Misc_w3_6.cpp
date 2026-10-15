// Free functions of group w3_6: the game's main-BGM start helper, cocos2d's JNI bridges (Cocos2dxHelper /
// Cocos2dxVideoHelper / AppActivity static methods), the class loader lookup and the ATITC texture decoder.
// The stack-protector epilogue of the originals (compare the canary, call __stack_chk_fail on mismatch) is not
// reproduced: the canary never changes.
#include "core/Guest.hpp"

namespace w3_6_misc {

namespace {
constexpr uint32_t GET_STATIC_METHOD_INFO = 0xA4225D;     // cocos2d::JniHelper::getStaticMethodInfo(info, cls, name, sig)
constexpr uint32_t JNI_GET_ENV = 0xA41F11;                // cocos2d::JniHelper::getEnv()
constexpr uint32_t CALL_STATIC_VOID_METHOD = 0x4E0D09;    // _JNIEnv::CallStaticVoidMethod(env, cls, mid, ...)
constexpr uint32_t CALL_STATIC_BOOLEAN_METHOD = 0x81AB19; // _JNIEnv::CallStaticBooleanMethod(env, cls, mid, ...)
constexpr uint32_t CALL_OBJECT_METHOD = 0x4E49AD;         // _JNIEnv::CallObjectMethod(env, obj, mid, ...)
constexpr uint32_t AUDIO_GET_INSTANCE = 0x5BECFD;         // audio::Manager::getInstance()
constexpr uint32_t AUDIO_LOCK = 0x5BF18D;                 // audio::Manager::lock(ResourceID)
constexpr uint32_t AUDIO_GET_LAYER = 0x5BF33D;            // audio::Manager::getLayer(shared_ptr<Layer>* out, mgr, int)
constexpr uint32_t AUDIO_LAYER_PLAY_BGM = 0x5BF811;       // audio::Layer::playBGM(ResourceID, int fadeFrames)
constexpr uint32_t ATITC_DECODE_BLOCK = 0x905521;         // sub_905520: decode one 4x4 ATITC block

// JNINativeInterface offsets
constexpr uint32_t JNI_EXCEPTION_CLEAR = 0x44;
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C;
constexpr uint32_t JNI_NEW_STRING_UTF = 0x29C;

constexpr uint32_t CLASS_LOADER = 0x1008948;              // cocos2d::JniHelper::classloader
constexpr uint32_t LOAD_CLASS_METHOD = 0x100894C;         // cocos2d::JniHelper::loadclassMethod_methodID

constexpr uint32_t STR_COCOS2DX_HELPER = 0xE10FFB;        // "org/cocos2dx/lib/Cocos2dxHelper"
constexpr uint32_t STR_TERMINATE_PROCESS = 0xE133BF;      // "terminateProcess"
constexpr uint32_t STR_VOID_SIG = 0xDFC723;               // "()V"
constexpr uint32_t STR_GET_BOOL_FOR_KEY = 0xE134AD;       // "getBoolForKey"
constexpr uint32_t STR_GET_BOOL_FOR_KEY_SIG = 0xE134BB;   // "(Ljava/lang/String;Z)Z"
constexpr uint32_t STR_VIDEO_HELPER = 0xE0C120;           // "org/cocos2dx/lib/Cocos2dxVideoHelper"
constexpr uint32_t STR_SET_FULL_SCREEN = 0xE0C176;        // "setFullScreenEnabled"
constexpr uint32_t STR_SET_FULL_SCREEN_SIG = 0xE0C18B;    // "(IZII)V"
constexpr uint32_t STR_APP_ACTIVITY = 0xDFC3F4;           // "org/cocos2dx/cpp/AppActivity"
constexpr uint32_t STR_CLIP_BOARD_COPY = 0xDFC601;        // "clipBoardCopy"
constexpr uint32_t STR_STRING_VOID_SIG = 0xDFC4E5;        // "(Ljava/lang/String;)V"

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

// Start the game's main BGM `resource` (fading in over fadeFrames) unless one is already playing, and lock the
// resource; returns the playing sound id.
uint32_t AudioPlayGameMainBGM(uint32_t resource, int32_t fadeFrames) {
    guest::Frame f(0x14);
    const uint32_t layer = f.addr(0x8);   // shared_ptr<audio::Layer>
    if (ld32(GAME_MAIN_BGM_RESOURCE) == 0) {
        rw::save<uint32_t>(GAME_MAIN_BGM_RESOURCE, resource);
        uint32_t manager = rw::call(AUDIO_GET_INSTANCE, {GAME_MAIN_BGM_RESOURCE});
        rw::call(AUDIO_GET_LAYER, {layer, manager, 0x7FFFFFFF});
        uint32_t sound = rw::call(AUDIO_LAYER_PLAY_BGM, {ld32(layer), resource, uint32_t(fadeFrames)});
        if (uint32_t ctrl = ld32(layer + 4)) guest::release(ctrl);
        rw::save<uint32_t>(GAME_MAIN_BGM_SOUND, sound);
        rw::call(AUDIO_LOCK, {rw::call(AUDIO_GET_INSTANCE, {GAME_MAIN_BGM_RESOURCE}), resource});
    }
    return ld32(GAME_MAIN_BGM_SOUND);
}
RW_FUNCTION(0x5C0E90, "AudioPlayGameMainBGM(audio::ResourceID, int)", AudioPlayGameMainBGM);

// Cocos2dxVideoHelper.setFullScreenEnabled(index, enabled, width, height)
void setFullScreenEnabledJni(int32_t index, uint32_t enabled, int32_t width, int32_t height) {
    guest::Frame f(0x2C);
    const MethodInfo info{f.addr(0x1C)};
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_VIDEO_HELPER, STR_SET_FULL_SCREEN, STR_SET_FULL_SCREEN_SIG}) != 1)
        return;
    rw::call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method(), uint32_t(index), enabled,
                                       uint32_t(width), uint32_t(height)});
    deleteLocalRef(info.env(), info.cls());
}
RW_FUNCTION(0x8437CC, "setFullScreenEnabledJni(int, bool, int, int)", setFullScreenEnabledJni);

// Cocos2dxHelper.terminateProcess()
void terminateProcessJNI() {
    guest::Frame f(0x18);
    const MethodInfo info{f.addr(0x8)};
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_COCOS2DX_HELPER, STR_TERMINATE_PROCESS, STR_VOID_SIG}) != 1)
        return;
    rw::call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method()});
    deleteLocalRef(info.env(), info.cls());
}
RW_FUNCTION(0xA40D74, "terminateProcessJNI()", terminateProcessJNI);

// Cocos2dxHelper.getBoolForKey(key, defaultValue) (UserDefault on Android); the default when the method is missing.
uint32_t getBoolForKeyJNI(uint32_t key, uint32_t defaultValue) {
    guest::Frame f(0x1C);
    const MethodInfo info{f.addr(0xC)};
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_COCOS2DX_HELPER, STR_GET_BOOL_FOR_KEY,
                                          STR_GET_BOOL_FOR_KEY_SIG}) != 1)
        return defaultValue;
    uint32_t jkey = newStringUTF(info.env(), key);
    uint32_t value = rw::call(CALL_STATIC_BOOLEAN_METHOD, {info.env(), info.cls(), info.method(), jkey, defaultValue});
    deleteLocalRef(info.env(), info.cls());
    deleteLocalRef(info.env(), jkey);
    return value != 0 ? 1 : 0;
}
RW_FUNCTION(0xA41404, "getBoolForKeyJNI(const char*, bool)", getBoolForKeyJNI);

// The Java class `className` through the app's class loader (ClassLoader.loadClass); 0 (and the pending exception
// cleared) when it is not found or className is null.
uint32_t getClassID(uint32_t className) {
    if (!className) return 0;
    uint32_t env = rw::call(JNI_GET_ENV, {0xA7});
    uint32_t jname = newStringUTF(env, className);
    uint32_t cls = rw::call(CALL_OBJECT_METHOD, {env, ld32(CLASS_LOADER), ld32(LOAD_CLASS_METHOD), jname});
    if (!cls) jni(env, JNI_EXCEPTION_CLEAR, {env});
    deleteLocalRef(env, jname);
    return cls;
}
RW_FUNCTION(0xA41E94, "_getClassID(const char*)", getClassID);

// AppActivity.clipBoardCopy(text): copy text to the clipboard.
void boardCopy(uint32_t text) {
    guest::Frame f(0x18);
    const MethodInfo info{f.addr(0x8)};
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_APP_ACTIVITY, STR_CLIP_BOARD_COPY, STR_STRING_VOID_SIG}) != 1)
        return;
    uint32_t jtext = newStringUTF(info.env(), text);
    rw::call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method(), jtext});
    deleteLocalRef(info.env(), jtext);
    deleteLocalRef(info.env(), info.cls());
}
RW_FUNCTION(0x4E2C80, "BoardCopy::copy(const char*)", boardCopy);

// Decode an ATITC texture (4x4 blocks, `flag` 1 = RGB (8-byte blocks), 3 = explicit alpha, 5 = interpolated
// alpha (16-byte blocks: 8 alpha bytes first)) from src into the RGBA8888 image at dst (width x height pixels).
// The rows advance by width/4*4 (at least 4) + 3*width pixels after every block row.
void atitc_decode(uint32_t src, uint32_t dst, int32_t width, int32_t height, int32_t flag) {
    guest::Frame f(0x54);
    const uint32_t srcPtr = f.addr(0x4C);   // the block decoder advances *srcPtr past the colour block
    *rw::frame_ptr<uint32_t>(srcPtr) = src;
    const int32_t blockRows = height / 4;
    if (height < 4) return;
    const uint32_t rounded = uint32_t(width + int32_t(uint32_t(width >> 31) >> 30));
    const uint32_t rowStride = width <= 7 ? 4 : (rounded & ~3u);
    const uint32_t skip = uint32_t(3 * width);
    const int32_t blockCols = int32_t(rounded) >> 2;
    uint32_t row = dst;
    for (int32_t y = 0; y < blockRows; y++) {
        if (width >= 4) {
            uint32_t out = row;
            for (int32_t x = 0; x < blockCols; x++) {
                if (flag == 3 || flag == 5) {
                    uint32_t p = rw::load<uint32_t>(srcPtr);
                    auto b = [&](uint32_t k) { return uint32_t(rw::load<uint8_t>(p + k)); };
                    uint32_t b0 = b(0), b1 = b(1), b2 = b(2), b3 = b(3), b4 = b(4), b5 = b(5), b6 = b(6), b7 = b(7);
                    *rw::frame_ptr<uint32_t>(srcPtr) = p + 8;
                    uint32_t alphaLo = (((b3 << 8) | b2) << 16) | ((b1 << 8) | b0);
                    uint32_t alphaHi = (((b7 << 8) | b6) << 16) | ((b5 << 8) | b4);
                    rw::call(ATITC_DECODE_BLOCK, {srcPtr, out, uint32_t(width), 1, alphaLo, alphaHi, uint32_t(flag)});
                } else if (flag == 1) {
                    rw::call(ATITC_DECODE_BLOCK, {srcPtr, out, uint32_t(width), 0, 0, 0, 1});
                }
                out += 16;
            }
            row += rowStride * 4;
        }
        row += skip * 4;
    }
}
RW_FUNCTION(0x9053BC, "atitc_decode(unsigned char*, unsigned char*, int, int, ATITCDecodeFlag)", atitc_decode);

}  // namespace w3_6_misc
