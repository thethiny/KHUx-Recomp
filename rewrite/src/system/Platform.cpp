// Android platform bridges over JNI: TrackingSDKUtil (analytics events), LocalNotification (scheduled
// notifications) and RemoteNotification (GCM push token).
#include "SystemCommon.hpp"

namespace platform {

namespace {
constexpr uint32_t GET_STATIC_METHOD_INFO = rwlib::cocos2d::JniHelper::getStaticMethodInfo;   // cocos2d::JniHelper::getStaticMethodInfo(info, cls, name, sig)
constexpr uint32_t GET_METHOD_INFO = rwlib::cocos2d::JniHelper::getMethodInfo;          // cocos2d::JniHelper::getMethodInfo(info, cls, name, sig)
constexpr uint32_t CALL_STATIC_OBJECT_METHOD = 0x4E0939;   // _JNIEnv::CallStaticObjectMethod(env, cls, mid, ...)
constexpr uint32_t CALL_STATIC_VOID_METHOD = 0x4E0D09;     // _JNIEnv::CallStaticVoidMethod(env, cls, mid, ...)
constexpr uint32_t CALL_VOID_METHOD = 0x4E17DD;            // _JNIEnv::CallVoidMethod(env, obj, mid, ...)
constexpr uint32_t ANDROID_LOG_PRINT = 0x413D38;           // __android_log_print (import)
constexpr uint32_t APPSTATE_GET_INSTANCE = 0x4E66DD;       // AppState::getInstance
constexpr uint32_t APPSTATE_CHECK_TRACKING_EVENT = 0x4EBFE5;   // AppState::checkTrackingEvent(const char*)
constexpr uint32_t APPSTATE_SAVE_TRACKING_EVENT = 0x4EC045;    // AppState::saveTrackingEvent(const char*)
constexpr uint32_t SEND_EVENT_INTERNAL = 0x4E2A5D;          // TrackingSDKUtil::sendEventInternal (an effect boundary)
constexpr uint32_t APPCONFIG_GET = 0x799FD1;               // AppConfig::get(string* out, const char* key)
constexpr uint32_t FUNCTION_ASSIGN = 0x4E2751;             // std::function<void(const char*)>::operator=(const function&)

// JNINativeInterface offsets
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C;
constexpr uint32_t JNI_NEW_STRING_UTF = 0x29C;

constexpr uint32_t STR_APP_ACTIVITY = 0xDFC3F4;        // "org/cocos2dx/cpp/AppActivity"
constexpr uint32_t STR_SHOW_LOCAL = 0xDFC3A9;          // "showLocalNotification"
constexpr uint32_t STR_SHOW_LOCAL_SIG = 0xDFC3BF;      // "(Ljava/lang/String;II)V"
constexpr uint32_t STR_CANCEL_LOCAL = 0xDFC3D7;        // "cancelLocalNotification"
constexpr uint32_t STR_CANCEL_LOCAL_SIG = 0xDFC3EF;    // "(I)V"
constexpr uint32_t STR_TRACKING_MANAGER = 0xDFC522;    // "org/cocos2dx/cpp/TrackingSDKManager"
constexpr uint32_t STR_GET_INSTANCE = 0xDFC411;        // "getInstance"
constexpr uint32_t STR_GET_INSTANCE_SIG = 0xDFC546;    // "()Lorg/cocos2dx/cpp/TrackingSDKManager;"
constexpr uint32_t STR_SEND_EVENT = 0xDFC5D8;          // "sendEvent"
constexpr uint32_t STR_SEND_EVENT_SIG2 = 0xDFC5B0;     // sendEvent(String, String) signature
constexpr uint32_t STR_STRING_ARG_SIG = 0xDFC4E5;      // "(Ljava/lang/String;)V"
constexpr uint32_t STR_LOG_TAG = 0xDFC56E;             // "TrackingSDKUtil::sendEvent"
constexpr uint32_t STR_LOG_NO_INSTANCE = 0xDFC589;     // "getStaticMethodInfo getInstance failed"
constexpr uint32_t STR_LOG_NO_METHOD = 0xDFC5E2;       // "getMethodInfo sendEvent failed"
constexpr uint32_t STR_GCM_PROJECT_ID = 0xDFC4FB;      // "gcm_project_id"
constexpr uint32_t STR_REGISTER_GCM = 0xDFC50A;        // "registerGcmInBackground"

constexpr uint32_t TOKEN_REQUESTED = 0x1003A10;        // bool: the push token was asked for
constexpr uint32_t TOKEN_CALLBACK = 0x10039F0;         // std::function<void(const char*)>: receives the token

uint32_t jni(uint32_t env, uint32_t offset, std::initializer_list<uint32_t> args) {
    return rw::call(sys::ld32(sys::ld32(env) + offset), args);
}
void deleteLocalRef(uint32_t env, uint32_t ref) { jni(env, JNI_DELETE_LOCAL_REF, {env, ref}); }
uint32_t newStringUTF(uint32_t env, uint32_t s) { return jni(env, JNI_NEW_STRING_UTF, {env, s}); }
}  // namespace

// ------------------------------------------------------------------------------------------------ TrackingSDKUtil
// Sends the event unless it was sent before (recorded in the AppState).
uint32_t sendEventOnlyOnce(uint32_t name, uint32_t value) {
    uint32_t state = rw::call(APPSTATE_GET_INSTANCE, {});
    uint32_t sent = rw::call(APPSTATE_CHECK_TRACKING_EVENT, {state, name});
    if (sent) return sent;
    rw::call(SEND_EVENT_INTERNAL, {name, value});
    state = rw::call(APPSTATE_GET_INSTANCE, {});
    return rw::call(APPSTATE_SAVE_TRACKING_EVENT, {state, name});
}
RW_FUNCTION(0x4E04E4, "TrackingSDKUtil::sendEventOnlyOnce", sendEventOnlyOnce);

void sendEvent(uint32_t name, uint32_t value) { rw::call(SEND_EVENT_INTERNAL, {name, value}); }
RW_FUNCTION(0x4E0518, "TrackingSDKUtil::sendEvent", sendEvent);

// TrackingSDKManager.getInstance().sendEvent(name[, value])
void sendEventInternal(uint32_t name, uint32_t value) {
    guest::Frame f(0x30);
    const uint32_t method = f.addr(0x10), manager = f.addr(0x1C);   // JniMethodInfo { env, classID, methodID }
    if (!rw::call(GET_STATIC_METHOD_INFO, {manager, STR_TRACKING_MANAGER, STR_GET_INSTANCE, STR_GET_INSTANCE_SIG})) {
        rw::call(ANDROID_LOG_PRINT, {6, STR_LOG_TAG, STR_LOG_NO_INSTANCE});
        return;
    }
    uint32_t instance =
        rw::call(CALL_STATIC_OBJECT_METHOD, {sys::ld32(manager), sys::ld32(manager + 4), sys::ld32(manager + 8)});
    uint32_t sig = value ? STR_SEND_EVENT_SIG2 : STR_STRING_ARG_SIG;
    if (!rw::call(GET_METHOD_INFO, {method, STR_TRACKING_MANAGER, STR_SEND_EVENT, sig})) {
        rw::call(ANDROID_LOG_PRINT, {6, STR_LOG_TAG, STR_LOG_NO_METHOD});
        return;
    }
    const uint32_t env = sys::ld32(manager);
    uint32_t jname = newStringUTF(env, name);
    if (value) {
        uint32_t jvalue = newStringUTF(sys::ld32(manager), value);
        rw::call(CALL_VOID_METHOD, {sys::ld32(method), instance, sys::ld32(method + 8), jname, jvalue});
        deleteLocalRef(sys::ld32(manager), jvalue);
    } else {
        rw::call(CALL_VOID_METHOD, {sys::ld32(method), instance, sys::ld32(method + 8), jname});
    }
    deleteLocalRef(sys::ld32(manager), jname);
    deleteLocalRef(sys::ld32(manager), sys::ld32(method + 4));
    deleteLocalRef(sys::ld32(manager), sys::ld32(manager + 4));
}
RW_FUNCTION(0x4E2A5C, "TrackingSDKUtil::sendEventInternal", sendEventInternal);

// ------------------------------------------------------------------------------------------------ LocalNotification
// AppActivity.showLocalNotification(message, seconds, id)
void localShow(const guest::String *message, uint32_t seconds, uint32_t id) {
    guest::Frame f(0x28);
    const uint32_t info = f.addr(0x14);
    if (rw::call(GET_STATIC_METHOD_INFO, {info, STR_APP_ACTIVITY, STR_SHOW_LOCAL, STR_SHOW_LOCAL_SIG}) != 1) return;
    uint32_t env = sys::ld32(info);
    uint32_t jmsg = newStringUTF(env, message->data());
    rw::call(CALL_STATIC_VOID_METHOD, {sys::ld32(info), sys::ld32(info + 4), sys::ld32(info + 8), jmsg, seconds, id});
    deleteLocalRef(sys::ld32(info), jmsg);
    deleteLocalRef(sys::ld32(info), sys::ld32(info + 4));
}
RW_FUNCTION(0x4E0C58, "LocalNotification::show", localShow);

// AppActivity.cancelLocalNotification(id)
void localCancel(uint32_t id) {
    guest::Frame f(0x18);
    const uint32_t info = f.addr(8);
    if (rw::call(GET_STATIC_METHOD_INFO, {info, STR_APP_ACTIVITY, STR_CANCEL_LOCAL, STR_CANCEL_LOCAL_SIG}) != 1) return;
    rw::call(CALL_STATIC_VOID_METHOD, {sys::ld32(info), sys::ld32(info + 4), sys::ld32(info + 8), id});
    deleteLocalRef(sys::ld32(info), sys::ld32(info + 4));
}
RW_FUNCTION(0x4E0D5C, "LocalNotification::cancel", localCancel);

// ------------------------------------------------------------------------------------------------ RemoteNotification
// True the first time only.
uint32_t needsGetToken() {
    uint8_t asked = sys::ld8(TOKEN_REQUESTED);
    sys::st8(TOKEN_REQUESTED, 1);
    return uint32_t(asked) ^ 1;
}
RW_FUNCTION(0x4E2610, "RemoteNotification::needsGetToken", needsGetToken);

// Register for GCM with the app's project id; the token arrives later through `callback`.
void getToken(uint32_t callback) {
    if (!sys::ld32(callback + 16)) return;   // an empty std::function
    guest::Frame f(0x28);
    const uint32_t info = f.addr(8), projectId = f.addr(0x14);
    rw::call(APPCONFIG_GET, {projectId, STR_GCM_PROJECT_ID});
    const guest::String *id = rw::host_ptr<guest::String>(projectId);
    if (id->size() &&
        rw::call(GET_STATIC_METHOD_INFO, {info, STR_APP_ACTIVITY, STR_REGISTER_GCM, STR_STRING_ARG_SIG})) {
        rw::call(FUNCTION_ASSIGN, {TOKEN_CALLBACK, callback});
        uint32_t env = sys::ld32(info);
        uint32_t jid = newStringUTF(env, id->data());
        rw::call(CALL_STATIC_VOID_METHOD, {sys::ld32(info), sys::ld32(info + 4), sys::ld32(info + 8), jid});
        deleteLocalRef(sys::ld32(info), jid);
        deleteLocalRef(sys::ld32(info), sys::ld32(info + 4));
    }
    if (id->isLong()) guest::deleteObject(sys::ld32(projectId + 8));
}
RW_FUNCTION(0x4E262C, "RemoteNotification::getToken", getToken);

}  // namespace platform
