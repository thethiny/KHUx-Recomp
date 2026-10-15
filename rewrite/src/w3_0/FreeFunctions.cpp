// Free functions of the game: the BGM stack pop (audio), the GL vertex-array extension lookup, and three JNI
// helpers (video resume, the native library folder, a string setting read through Cocos2dxHelper).
#include "../scene/SceneCommon.hpp"
#include "core/Guest.hpp"

using namespace scene;

namespace {
// ---- BGM stack: std::deque<std::pair<int id, int handle>> of the BGMs interrupted by AudioPushGameMainBGM
constexpr uint32_t BGM_CURRENT_ID = 0x1003C30;        // the playing main BGM id
constexpr uint32_t BGM_CURRENT_HANDLE = 0x1003C34;    // its audio handle
constexpr uint32_t BGM_STACK_MAP_BEGIN = 0x1003C68;   // deque: block map begin / end, start index, size
constexpr uint32_t BGM_STACK_MAP_END = 0x1003C6C;
constexpr uint32_t BGM_STACK_START = 0x1003C74;
constexpr uint32_t BGM_STACK_SIZE = 0x1003C78;
constexpr uint32_t AUDIO_STOP_GAME_MAIN_BGM = 0x5C0F59;   // AudioStopGameMainBGM(int fade)
constexpr uint32_t AUDIO_MANAGER_GET_INSTANCE = 0x5BECFD;
constexpr uint32_t AUDIO_MANAGER_RESUME = 0x5BF48D;       // audio::Manager::resume(handle, fade)

// ---- GL extensions
constexpr uint32_t EGL_GET_PROC_ADDRESS = 0x44480C;           // eglGetProcAddress (import)
constexpr uint32_t GL_GEN_VERTEX_ARRAYS_EXT = 0x1008924;      // glGenVertexArraysOESEXT (function pointer global)
constexpr uint32_t GL_BIND_VERTEX_ARRAY_EXT = 0x1008928;      // glBindVertexArrayOESEXT
constexpr uint32_t GL_DELETE_VERTEX_ARRAYS_EXT = 0x100892C;   // glDeleteVertexArraysOESEXT
constexpr uint32_t STR_GL_GEN_VERTEX_ARRAYS = 0xE1325D;       // "glGenVertexArraysOES"
constexpr uint32_t STR_GL_BIND_VERTEX_ARRAY = 0xE13272;       // "glBindVertexArrayOES"
constexpr uint32_t STR_GL_DELETE_VERTEX_ARRAYS = 0xE13287;    // "glDeleteVertexArraysOES"

// ---- JNI
constexpr uint32_t CALL_VIDEO_NON_PARAMETER_FUN = 0x8436A1;   // callVideoNonParameterFun(int, const char*)
constexpr uint32_t STR_RESUME_VIDEO = 0xE0C1CD;               // "resumeVideo"
constexpr uint32_t STR_EMPTY = 0xDF8A1A;                      // ""
constexpr uint32_t STRLEN = 0x40E08C;                         // strlen (import)
constexpr uint32_t JNI_GET_ENV = 0xA41F11;                    // cocos2d::JniHelper::getEnv()
constexpr uint32_t JNI_GET_FIELD_ID = 0xA4235B;               // JniHelper::getFieldID(jfieldID&, cls, name, sig)
constexpr uint32_t JNI_GET_STATIC_METHOD_INFO = 0xA4225D;     // JniHelper::getStaticMethodInfo(info&, cls, name, sig)
constexpr uint32_t JNI_JSTRING_TO_STRING = 0xA423E9;          // JniHelper::jstring2string(jstring) -> std::string
constexpr uint32_t JNI_CALL_STATIC_OBJECT_METHOD = 0x4E0939;  // _JNIEnv::CallStaticObjectMethod(cls, method, ...)
constexpr uint32_t JNI_APPLICATION_INFO = 0x1008954;          // JniHelper::applicationInfo (jobject)
constexpr uint32_t STR_APPLICATION_INFO_CLASS = 0xE13414;     // "android/content/pm/ApplicationInfo"
constexpr uint32_t STR_NATIVE_LIBRARY_DIR = 0xE13437;         // "nativeLibraryDir"
constexpr uint32_t STR_JAVA_STRING_SIG = 0xE0B9CD;            // "Ljava/lang/String;"
constexpr uint32_t STR_COCOS2DX_HELPER = 0xE10FFB;            // "org/cocos2dx/lib/Cocos2dxHelper"
constexpr uint32_t STR_GET_STRING_FOR_KEY = 0xE13547;         // "getStringForKey"
constexpr uint32_t STR_GET_STRING_FOR_KEY_SIG = 0xE13557;     // "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"
// JNIEnv function table (byte offsets)
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C;
constexpr uint32_t JNI_GET_OBJECT_FIELD = 0x17C;
constexpr uint32_t JNI_NEW_STRING_UTF = 0x29C;
}  // namespace

// AudioPopGameMainBGM(fade): take the last BGM pushed on the stack; when it is not the one playing, stop the
// current BGM and resume the pushed one. Returns the (now) current handle, 0 when the stack is empty.
uint32_t AudioPopGameMainBGM(int32_t fade) {
    uint32_t size = rw::load<uint32_t>(BGM_STACK_SIZE);
    if (size == 0) return 0;
    uint32_t start = rw::load<uint32_t>(BGM_STACK_START);
    uint32_t last = start + size - 1;
    uint32_t mapBegin = rw::load<uint32_t>(BGM_STACK_MAP_BEGIN);
    // 512 entries of 8 bytes per block
    uint32_t entry = ld32(mapBegin + ((last >> 7) & 0x1FFFFFC)) + (last & 0x1FF) * 8;
    uint32_t id = ld32(entry);
    uint32_t handle = ld32(entry + 4);
    rw::save<uint32_t>(BGM_STACK_SIZE, size - 1);
    // pop_back: free the last block once two blocks' worth of room is spare at the back
    uint32_t mapEnd = rw::load<uint32_t>(BGM_STACK_MAP_END);
    uint32_t capacity = mapEnd == mapBegin ? 0 : (mapEnd - mapBegin) * 128 - 1;
    if ((capacity + 1 - size - start) >> 10) {
        guest::deleteObject(ld32(mapEnd - 4));
        rw::save<uint32_t>(BGM_STACK_MAP_END, rw::load<uint32_t>(BGM_STACK_MAP_END) - 4);
    }
    if (id == rw::load<uint32_t>(BGM_CURRENT_ID)) return rw::load<uint32_t>(BGM_CURRENT_HANDLE);
    call(AUDIO_STOP_GAME_MAIN_BGM, {uint32_t(fade)});
    rw::save<uint32_t>(BGM_CURRENT_ID, id);
    rw::save<uint32_t>(BGM_CURRENT_HANDLE, handle);
    uint32_t manager = call(AUDIO_MANAGER_GET_INSTANCE, {});
    call(AUDIO_MANAGER_RESUME, {manager, rw::load<uint32_t>(BGM_CURRENT_HANDLE), uint32_t(fade)});
    return rw::load<uint32_t>(BGM_CURRENT_HANDLE);
}
RW_FUNCTION(0x5C18D8, "AudioPopGameMainBGM(int)", AudioPopGameMainBGM);

// resumeVideoJNI(id): the Java-side video player call. NOVERIFY: it resumes a video on the Java side (outside
// guest memory, not in the runtime's effect list), so a shadow run would do it twice.
uint32_t resumeVideoJNI(int32_t id) { return call(CALL_VIDEO_NON_PARAMETER_FUN, {uint32_t(id), STR_RESUME_VIDEO}); }
RW_FUNCTION_NOVERIFY(0x843930, "resumeVideoJNI(int)", resumeVideoJNI);

// initExtensions(): look up the GL_OES_vertex_array_object entry points.
uint32_t initExtensions() {
    rw::save<uint32_t>(GL_GEN_VERTEX_ARRAYS_EXT, call(EGL_GET_PROC_ADDRESS, {STR_GL_GEN_VERTEX_ARRAYS}));
    rw::save<uint32_t>(GL_BIND_VERTEX_ARRAY_EXT, call(EGL_GET_PROC_ADDRESS, {STR_GL_BIND_VERTEX_ARRAY}));
    uint32_t del = call(EGL_GET_PROC_ADDRESS, {STR_GL_DELETE_VERTEX_ARRAYS});
    rw::save<uint32_t>(GL_DELETE_VERTEX_ARRAYS_EXT, del);
    return del;
}
RW_FUNCTION(0xA3F1C8, "initExtensions()", initExtensions);

// getNativeLibraryDirectoryJNI() -> std::string (sret): ApplicationInfo.nativeLibraryDir, "" when the field is
// missing.
void getNativeLibraryDirectoryJNI(uint32_t out) {
    Frame f;
    uint32_t fieldId = f.alloc(8);
    uint32_t value = f.alloc(12);
    strInit(out, STR_EMPTY, 0);
    if (call(JNI_GET_FIELD_ID, {fieldId, STR_APPLICATION_INFO_CLASS, STR_NATIVE_LIBRARY_DIR, STR_JAVA_STRING_SIG}) != 1)
        return;
    uint32_t env = call(JNI_GET_ENV, {});
    uint32_t getObjectField = ld32(ld32(env) + JNI_GET_OBJECT_FIELD);
    uint32_t jstr = call(getObjectField, {env, ld32(JNI_APPLICATION_INFO), ld32(fieldId)});
    call(JNI_JSTRING_TO_STRING, {value, jstr});
    strMoveAssign(out, value);
    vcall(env, JNI_DELETE_LOCAL_REF, {env, jstr});
}
RW_FUNCTION(0xA41060, "getNativeLibraryDirectoryJNI()", getNativeLibraryDirectoryJNI);

// getStringForKeyJNI(key, defaultValue) -> std::string (sret): Cocos2dxHelper.getStringForKey(key, default), or
// defaultValue when the Java method is missing.
void getStringForKeyJNI(uint32_t out, uint32_t key, uint32_t defaultValue) {
    Frame f;
    uint32_t info = f.alloc(12);    // JniMethodInfo: env, classID, methodID
    uint32_t result = f.alloc(12);
    uint32_t text = f.alloc(12);
    strInit(text, STR_EMPTY, 0);
    if (call(JNI_GET_STATIC_METHOD_INFO, {info, STR_COCOS2DX_HELPER, STR_GET_STRING_FOR_KEY,
                                          STR_GET_STRING_FOR_KEY_SIG}) != 1) {
        uint32_t n = call(STRLEN, {defaultValue});
        strInit(out, defaultValue, n);
        strFree(text);
        return;
    }
    uint32_t env = ld32(info);
    uint32_t jkey = call(ld32(ld32(env) + JNI_NEW_STRING_UTF), {env, key});
    env = ld32(info);
    uint32_t jdefault = call(ld32(ld32(env) + JNI_NEW_STRING_UTF), {env, defaultValue});
    uint32_t jresult =
        call(JNI_CALL_STATIC_OBJECT_METHOD, {ld32(info), ld32(info + 4), ld32(info + 8), jkey, jdefault});
    call(JNI_JSTRING_TO_STRING, {result, jresult});
    strMoveAssign(text, result);
    env = ld32(info);
    vcall(env, JNI_DELETE_LOCAL_REF, {env, ld32(info + 4)});
    env = ld32(info);
    vcall(env, JNI_DELETE_LOCAL_REF, {env, jkey});
    env = ld32(info);
    vcall(env, JNI_DELETE_LOCAL_REF, {env, jdefault});
    env = ld32(info);
    vcall(env, JNI_DELETE_LOCAL_REF, {env, jresult});
    // out = std::move(text) (out is raw memory: a plain copy of the three words)
    st32(out, ld32(text));
    st32(out + 4, ld32(text + 4));
    st32(out + 8, ld32(text + 8));
}
RW_FUNCTION(0xA4172C, "getStringForKeyJNI(const char*, const char*)", getStringForKeyJNI);
