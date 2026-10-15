// Free functions of wave 3 / group 2: audio start-up, the main-BGM id, and a few cocos2d JNI bridges
// (Cocos2dxVideoHelper, Cocos2dxBitmap, Cocos2dxHelper).
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"

namespace w3_2_free {

namespace {
using scene::call;
using scene::ld32;

constexpr uint32_t GET_STATIC_METHOD_INFO = 0xA4225D;    // cocos2d::JniHelper::getStaticMethodInfo(info, cls, name, sig)
constexpr uint32_t CALL_STATIC_VOID_METHOD = 0x4E0D09;   // _JNIEnv::CallStaticVoidMethod(env, cls, mid, ...)
constexpr uint32_t CALL_STATIC_INT_METHOD = 0x81AB6D;    // _JNIEnv::CallStaticIntMethod(env, cls, mid, ...)
constexpr uint32_t CALL_STATIC_OBJECT_METHOD = 0x4E0939; // _JNIEnv::CallStaticObjectMethod(env, cls, mid, ...)
constexpr uint32_t STRING_ASSIGN = 0x48D811;             // std::string::assign(const char*, size_t)

// JNINativeInterface slots (byte offsets)
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C;
constexpr uint32_t JNI_NEW_STRING_UTF = 0x29C;
constexpr uint32_t JNI_GET_STRING_UTF_CHARS = 0x2A4;
constexpr uint32_t JNI_RELEASE_STRING_UTF_CHARS = 0x2A8;

constexpr uint32_t STR_VIDEO_HELPER = 0xE0C120;        // "org/cocos2dx/lib/Cocos2dxVideoHelper"
constexpr uint32_t STR_CREATE_VIDEO_WIDGET = 0xE0C145; // "createVideoWidget"
constexpr uint32_t STR_SIG_I = 0xE0B3D2;               // "()I"
constexpr uint32_t STR_SEEK_VIDEO_TO = 0xE0C1E3;       // "seekVideoTo"
constexpr uint32_t STR_SIG_II_V = 0xE0C1EF;            // "(II)V"
constexpr uint32_t STR_BITMAP = 0xE131E7;              // "org/cocos2dx/lib/Cocos2dxBitmap"
constexpr uint32_t STR_GET_STRING_WITH_ELLIPSIS = 0xE13346;   // "getStringWithEllipsis"
constexpr uint32_t STR_SIG_SFF_S = 0xE1335C;           // "(Ljava/lang/String;FF)Ljava/lang/String;"
constexpr uint32_t STR_EMPTY = 0xDF8A1A;               // ""
constexpr uint32_t STR_COCOS2DX_HELPER = 0xE10FFB;     // "org/cocos2dx/lib/Cocos2dxHelper"
constexpr uint32_t STR_ENABLE_ACCEL = 0xE1345B;        // "enableAccelerometer"
constexpr uint32_t STR_SIG_V = 0xDFC723;               // "()V"
constexpr uint32_t STR_SET_INTEGER_FOR_KEY = 0xE1359E; // "setIntegerForKey"
constexpr uint32_t STR_SIG_SI_V = 0xE135AF;            // "(Ljava/lang/String;I)V"

// cocos2d::JniMethodInfo { JNIEnv* env; jclass classID; jmethodID methodID; } in a guest stack frame
struct MethodInfo {
    uint32_t at;
    uint32_t env() const { return ld32(at); }
    uint32_t cls() const { return ld32(at + 4); }
    uint32_t method() const { return ld32(at + 8); }
};
uint32_t jni(uint32_t env, uint32_t slot, std::initializer_list<uint32_t> args) {
    return rw::call(ld32(ld32(env) + slot), args);
}
void deleteLocalRef(uint32_t env, uint32_t ref) { jni(env, JNI_DELETE_LOCAL_REF, {env, ref}); }

// audio
constexpr uint32_t Audio_initResourceIDMap = 0x5C227D;
constexpr uint32_t Audio_getInstance = 0x5BECFD;
constexpr uint32_t Audio_create = 0x5BEDE9;
constexpr uint32_t Audio_addLayer = 0x5BF24D;           // Manager::addLayer(int layer)
constexpr uint32_t Audio_load = 0x5BEEF5;               // Manager::load(ResourceID)
constexpr uint32_t Audio_lock = 0x5BF18D;               // Manager::lock(ResourceID): keep it resident
constexpr uint32_t PRELOADED_SOUNDS = 0xE49350;         // ResourceID[3], walked from the end down
constexpr uint32_t GAME_MAIN_BGM = 0x1003C30;           // ResourceID of the game's main BGM
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;
constexpr uint32_t EVENT_AFTER_DRAW = 0xFEF684;         // cocos2d::Director::EVENT_AFTER_DRAW (const char*)
constexpr uint32_t STRING_INIT = 0x47CD93;              // std::string::__init(const char*, size_t)
constexpr uint32_t EventDispatcher_addCustomEventListener = 0x8F36B9;   // (dispatcher, const string&, const function&)
constexpr uint32_t VT_AFTER_DRAW_LAMBDA = 0xF5CAE0;     // AudioSystemInit::$_0 (__func vtable + 8; no captures)
}  // namespace

// Starts the audio system: three layers (10000, INT_MAX, 0), the always-loaded sounds, and a per-frame
// "director_after_draw" listener that drives the audio manager.
void AudioSystemInit() {
    call(Audio_initResourceIDMap, {});
    uint32_t mgr = call(Audio_getInstance, {});
    call(Audio_create, {mgr});
    call(Audio_addLayer, {mgr, 10000});
    call(Audio_addLayer, {mgr, 0x7FFFFFFF});
    call(Audio_addLayer, {mgr, 0});
    for (uint32_t k = 0; k != 3; k++) {
        uint32_t id = ld32(PRELOADED_SOUNDS - 4 * k);
        call(Audio_load, {mgr, id});
        call(Audio_lock, {mgr, id});
    }
    uint32_t dispatcher = ld32(call(Director_getInstance, {}) + DIRECTOR_EVENT_DISPATCHER);
    uint32_t name = ld32(EVENT_AFTER_DRAW);
    scene::Frame f;
    uint32_t event = f.alloc(12);
    call(STRING_INIT, {event, name, scene::host_strlen(name)});
    uint32_t listener = scene::newFunction(f, VT_AFTER_DRAW_LAMBDA, {});
    call(EventDispatcher_addCustomEventListener, {dispatcher, event, listener});
    scene::functionDestroy(listener);
    scene::strFree(event);
}
RW_FUNCTION(0x5C0BA0, "AudioSystemInit()", AudioSystemInit);

uint32_t AudioGetGameMainBGMResouceID() { return ld32(GAME_MAIN_BGM); }
RW_FUNCTION(0x5C1A64, "AudioGetGameMainBGMResouceID()", AudioGetGameMainBGMResouceID);

// Cocos2dxVideoHelper.createVideoWidget(): the new widget's index, -1 when the method is missing
int32_t createVideoWidgetJNI() {
    guest::Frame f(0x18);
    const MethodInfo info{f.addr(8)};
    int32_t index = -1;
    if (call(GET_STATIC_METHOD_INFO, {info.at, STR_VIDEO_HELPER, STR_CREATE_VIDEO_WIDGET, STR_SIG_I}) == 1) {
        index = int32_t(call(CALL_STATIC_INT_METHOD, {info.env(), info.cls(), info.method()}));
        deleteLocalRef(info.env(), info.cls());
    }
    return index;
}
RW_FUNCTION(0x843624, "createVideoWidgetJNI()", createVideoWidgetJNI);

// Cocos2dxVideoHelper.seekVideoTo(index, msec)
void seekVideoToJNI(uint32_t index, uint32_t msec) {
    guest::Frame f(0x1C);
    const MethodInfo info{f.addr(0xC)};
    if (call(GET_STATIC_METHOD_INFO, {info.at, STR_VIDEO_HELPER, STR_SEEK_VIDEO_TO, STR_SIG_II_V}) != 1) return;
    call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method(), index, msec});
    deleteLocalRef(info.env(), info.cls());
}
RW_FUNCTION(0x843960, "seekVideoToJNI(int, int)", seekVideoToJNI);

// Cocos2dxBitmap.getStringWithEllipsis(text, width, fontSize): the text cut to fit, with "..." (empty string
// when the method is missing)
void getStringWithEllipsisJni(guest::String *out, uint32_t text, float width, float fontSize) {
    uint32_t o = rw::guest_addr(out);
    scene::st32(o, 0);
    scene::st32(o + 4, 0);
    scene::st32(o + 8, 0);
    guest::Frame f(0x34);
    const MethodInfo info{f.addr(0x24)};
    if (call(GET_STATIC_METHOD_INFO, {info.at, STR_BITMAP, STR_GET_STRING_WITH_ELLIPSIS, STR_SIG_SFF_S}) != 1) return;
    uint32_t jtext = jni(info.env(), JNI_NEW_STRING_UTF, {info.env(), text ? text : STR_EMPTY});
    // variadic floats travel as doubles (__extendsfdf2), on the stack after jtext
    double w = width, fs = fontSize;
    uint64_t wb, fb;
    std::memcpy(&wb, &w, 8);
    std::memcpy(&fb, &fs, 8);
    uint32_t jresult = call(CALL_STATIC_OBJECT_METHOD, {info.env(), info.cls(), info.method(), jtext, uint32_t(wb),
                                                        uint32_t(wb >> 32), uint32_t(fb), uint32_t(fb >> 32)});
    uint32_t chars = jni(info.env(), JNI_GET_STRING_UTF_CHARS, {info.env(), jresult, 0});
    call(STRING_ASSIGN, {o, chars, scene::host_strlen(chars)});
    jni(info.env(), JNI_RELEASE_STRING_UTF_CHARS, {info.env(), jresult, chars});
    deleteLocalRef(info.env(), jtext);
    deleteLocalRef(info.env(), info.cls());
}
RW_FUNCTION(0xA40870, "getStringWithEllipsisJni(const char*, float, float)", getStringWithEllipsisJni);

// Cocos2dxHelper.enableAccelerometer()
void enableAccelerometerJni() {
    guest::Frame f(0x18);
    const MethodInfo info{f.addr(8)};
    if (call(GET_STATIC_METHOD_INFO, {info.at, STR_COCOS2DX_HELPER, STR_ENABLE_ACCEL, STR_SIG_V}) != 1) return;
    call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method()});
    deleteLocalRef(info.env(), info.cls());
}
RW_FUNCTION(0xA41224, "enableAccelerometerJni()", enableAccelerometerJni);

// Cocos2dxHelper.setIntegerForKey(key, value) (UserDefault on Android)
void setIntegerForKeyJNI(uint32_t key, uint32_t value) {
    guest::Frame f(0x1C);
    const MethodInfo info{f.addr(0xC)};
    if (call(GET_STATIC_METHOD_INFO, {info.at, STR_COCOS2DX_HELPER, STR_SET_INTEGER_FOR_KEY, STR_SIG_SI_V}) != 1) return;
    uint32_t jkey = jni(info.env(), JNI_NEW_STRING_UTF, {info.env(), key});
    call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method(), jkey, value});
    deleteLocalRef(info.env(), info.cls());
    deleteLocalRef(info.env(), jkey);
}
RW_FUNCTION(0xA41924, "setIntegerForKeyJNI(const char*, int)", setIntegerForKeyJNI);

}  // namespace w3_2_free
