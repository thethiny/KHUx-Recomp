// Batch #13: the JNI entry points Java calls back into - AppActivity.didRegister (the push token), the
// TrackingSDKManager's app config lookup, and the Facebook SDK's answers (FacebookManager.onLogin .. onGetFriends:
// success, the SDK's error code + 1, a message and the call's data) - plus cocostudio's trigger sendEvent.
//
// The Facebook answers go to BGFacebook::FacebookManagerImpl (the FacebookManager singleton's impl, +4), which
// hands them to the callback the request stored.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t TokenCallback_call = 0x4E28AD;        // std::function<void(const char*)>::operator()
constexpr uint32_t AppConfig_get = 0x799FD1;             // (string* out, const char* key)
constexpr uint32_t FacebookManager_getInstance = 0x4E5585;
constexpr uint32_t FacebookImpl_onLogin = 0x4E4C8D;      // (impl, bool, ErrorCode, const string& message)
constexpr uint32_t FacebookImpl_onLogout = 0x4E4DE9;
constexpr uint32_t FacebookImpl_onGetFriends = 0x4E4F11; // (impl, bool, ErrorCode, const string&, vector<string>*)
constexpr uint32_t FacebookImpl_onSharePhoto = 0x4E506D;
constexpr uint32_t FacebookImpl_onInviteFriends = 0x4E518D;
constexpr uint32_t FacebookImpl_onGetProfileImageUrl = 0x4E52B5;  // (impl, bool, ErrorCode, const string&, const string& url)
constexpr uint32_t FacebookImpl_onRefreshCurrentAccessToken = 0x4E5451;
constexpr uint32_t StringVector_pushBackSlow = 0x484465;  // vector<string>::__push_back_slow_path(string&&)
constexpr uint32_t StringVector_dtor = 0x47363F;
constexpr uint32_t SPRINTF = 0xCA4A95;
constexpr uint32_t EventCustom_ctor = rwlib::cocos2d::EventCustom::ctor_base;           // (event, const string& name)
constexpr uint32_t Event_dtor = rwlib::cocos2d::Event::dtor_base;
constexpr uint32_t TriggerMng_getInstance = rwlib::cocostudio::TriggerMng::getInstance;
constexpr uint32_t TriggerMng_dispatchEvent = rwlib::cocostudio::TriggerMng::dispatchEvent;   // (mgr, EventCustom*)
}  // namespace g

// JNINativeInterface offsets
constexpr uint32_t JNI_NEW_STRING_UTF = 0x29C, JNI_GET_STRING_UTF_CHARS = 0x2A4, JNI_RELEASE_STRING_UTF_CHARS = 0x2A8;
constexpr uint32_t JNI_GET_ARRAY_LENGTH = 0x2AC, JNI_GET_OBJECT_ARRAY_ELEMENT = 0x2B4;

constexpr uint32_t TOKEN_CALLBACK = 0x10039F0;   // RemoteNotification's std::function<void(const char*)>
constexpr uint32_t FACEBOOK_IMPL = 0x1003A34;    // the FacebookManager singleton's FacebookManagerImpl*
constexpr uint32_t VT_EVENT_CUSTOM_GOT = 0xFD72C0;
constexpr uint32_t S_EMPTY = 0xDF8A1A;
constexpr uint32_t S_INT_FORMAT = 0xDF86DE;      // "%d"

uint32_t jni(uint32_t env, uint32_t offset, std::initializer_list<uint32_t> args) {
    return call(ld32(ld32(env) + offset), args);
}
uint32_t utfChars(uint32_t env, uint32_t jstr, uint32_t isCopy = 0) {
    return jni(env, JNI_GET_STRING_UTF_CHARS, {env, jstr, isCopy});
}
void releaseUtfChars(uint32_t env, uint32_t jstr, uint32_t chars) {
    jni(env, JNI_RELEASE_STRING_UTF_CHARS, {env, jstr, chars});
}

// the answers with only a message: impl->onX(success, code - 1, message)
void facebookAnswer(uint32_t implFn, uint32_t env, uint32_t success, int32_t code, uint32_t jmessage) {
    Frame f;
    const uint32_t chars = utfChars(env, jmessage);
    call(g::FacebookManager_getInstance, {});
    const uint32_t message = f.alloc(12);
    strInitC(message, chars);
    call(implFn, {ld32(FACEBOOK_IMPL), success != 0, uint32_t(code - 1), message});
    strFree(message);
    releaseUtfChars(env, jmessage, chars);
}

// the answers with a list of strings (friend ids): read from the Java array only when the call succeeded
void facebookListAnswer(uint32_t implFn, uint32_t env, uint32_t success, uint32_t reported, int32_t code,
                        uint32_t jmessage, uint32_t jlist) {
    Frame f;
    const uint32_t chars = utfChars(env, jmessage);
    const uint32_t list = f.alloc(12);  // std::vector<std::string>: begin, end, capacity end
    st32(list, 0);
    st32(list + 4, 0);
    st32(list + 8, 0);
    if (success) {
        const int32_t n = int32_t(jni(env, JNI_GET_ARRAY_LENGTH, {env, jlist}));
        const uint32_t item = f.alloc(12);
        for (int32_t i = 0; i < n; i++) {
            const uint32_t jitem = jni(env, JNI_GET_OBJECT_ARRAY_ELEMENT, {env, jlist, uint32_t(i)});
            const uint32_t text = utfChars(env, jitem);
            strInitC(item, text);
            const uint32_t end = ld32(list + 4);
            if (end < ld32(list + 8)) {  // moved in place
                for (uint32_t k = 0; k < 12; k += 4) st32(end + k, ld32(item + k));
                for (uint32_t k = 0; k < 12; k += 4) st32(item + k, 0);
                st32(list + 4, end + 12);
            } else {
                call(g::StringVector_pushBackSlow, {list, item});
            }
            strFree(item);
            releaseUtfChars(env, jitem, text);
        }
    }
    call(g::FacebookManager_getInstance, {});
    const uint32_t message = f.alloc(12);
    strInitC(message, chars);
    call(implFn, {ld32(FACEBOOK_IMPL), reported, uint32_t(code - 1), message, list});
    strFree(message);
    releaseUtfChars(env, jmessage, chars);
    call(g::StringVector_dtor, {list});
}
}  // namespace

struct JniCallbacks {
    // AppActivity.didRegister(String token): hands the token to the callback RemoteNotification::getToken stored
    // (once: the callback is destroyed after the call)
    static void didRegister(uint32_t env, uint32_t /*thiz*/, uint32_t jtoken) {
        Frame f;
        const uint32_t isCopy = f.alloc(8);
        st8(isCopy, 0);
        const uint32_t token = jtoken ? utfChars(env, jtoken, isCopy) : 0;
        if (ld32(TOKEN_CALLBACK + 0x10)) {
            call(g::TokenCallback_call, {TOKEN_CALLBACK, token});
            const uint32_t target = ld32(TOKEN_CALLBACK + 0x10);
            if (target == TOKEN_CALLBACK) call(ld32(ld32(TOKEN_CALLBACK) + 0x10), {TOKEN_CALLBACK});
            else if (target) call(ld32(ld32(target) + 0x14), {target});
            st32(TOKEN_CALLBACK + 0x10, 0);
        }
        if (ld8(isCopy) == 1) releaseUtfChars(env, jtoken, token);
    }

    // TrackingSDKManager.getAppConfig(String key): AppConfig's value as a Java string
    static uint32_t getAppConfig(uint32_t env, uint32_t /*thiz*/, uint32_t jkey) {
        Frame f;
        const uint32_t key = utfChars(env, jkey);
        const uint32_t value = f.alloc(12);
        call(g::AppConfig_get, {value, key});
        releaseUtfChars(env, jkey, key);
        const uint32_t result = jni(env, JNI_NEW_STRING_UTF, {env, strData(value)});
        strFree(value);
        return result;
    }

    static void onLogin(uint32_t env, uint32_t, uint32_t success, int32_t code, uint32_t jmessage) {
        facebookAnswer(g::FacebookImpl_onLogin, env, success, code, jmessage);
    }
    static void onLogout(uint32_t env, uint32_t, uint32_t success, int32_t code, uint32_t jmessage) {
        facebookAnswer(g::FacebookImpl_onLogout, env, success, code, jmessage);
    }
    static void onSharePhoto(uint32_t env, uint32_t, uint32_t success, int32_t code, uint32_t jmessage) {
        facebookAnswer(g::FacebookImpl_onSharePhoto, env, success, code, jmessage);
    }
    static void onRefreshCurrentAccessToken(uint32_t env, uint32_t, uint32_t success, int32_t code,
                                            uint32_t jmessage) {
        facebookAnswer(g::FacebookImpl_onRefreshCurrentAccessToken, env, success, code, jmessage);
    }
    // onGetFriends reports success to the impl even when the call failed (then with no friends)
    static void onGetFriends(uint32_t env, uint32_t, uint32_t success, int32_t code, uint32_t jmessage,
                             uint32_t jfriends) {
        facebookListAnswer(g::FacebookImpl_onGetFriends, env, success, 1, code, jmessage, jfriends);
    }
    static void onInviteFriends(uint32_t env, uint32_t, uint32_t success, int32_t code, uint32_t jmessage,
                                uint32_t jinvited) {
        facebookListAnswer(g::FacebookImpl_onInviteFriends, env, success, success != 0, code, jmessage, jinvited);
    }

    // onGetProfileImageUrl(success, code, message, url): a failed call reports an empty url
    static void onGetProfileImageUrl(uint32_t env, uint32_t, uint32_t success, int32_t code, uint32_t jmessage,
                                     uint32_t jurl) {
        Frame f;
        const uint32_t chars = utfChars(env, jmessage);
        const uint32_t message = f.alloc(12), url = f.alloc(12);
        if (success) {
            const uint32_t urlChars = utfChars(env, jurl);
            call(g::FacebookManager_getInstance, {});
            strInitC(message, chars);
            strInitC(url, urlChars);
            call(g::FacebookImpl_onGetProfileImageUrl, {ld32(FACEBOOK_IMPL), 1, uint32_t(code - 1), message, url});
            strFree(url);
            strFree(message);
            releaseUtfChars(env, jurl, urlChars);
        } else {
            call(g::FacebookManager_getInstance, {});
            strInitC(message, chars);
            strInit(url, S_EMPTY, 0);
            call(g::FacebookImpl_onGetProfileImageUrl, {ld32(FACEBOOK_IMPL), 0, uint32_t(code - 1), message, url});
            strFree(url);
            strFree(message);
        }
        releaseUtfChars(env, jmessage, chars);
    }

    // cocostudio's sendEvent(event): dispatches EventCustom("<event>") to the TriggerMng
    static void sendEvent(uint32_t event) {
        Frame f;
        const uint32_t digits = guest::newArray(10);
        call(g::SPRINTF, {digits, S_INT_FORMAT, event});
        const uint32_t name = f.alloc(12);
        strInitC(name, digits);
        guest::deleteArray(digits);
        const uint32_t ev = f.alloc(0x30);
        call(g::EventCustom_ctor, {ev, name});
        call(g::TriggerMng_dispatchEvent, {call(g::TriggerMng_getInstance, {}), ev});
        // ~EventCustom, inlined: its name string, then ~Event
        st32(ev, ld32(VT_EVENT_CUSTOM_GOT) + 8);
        strFree(ev + 0x24);
        call(g::Event_dtor, {ev});
        strFree(name);
    }
};
RW_FUNCTION(0x4E27C0, "Java_org_cocos2dx_cpp_AppActivity_didRegister", JniCallbacks::didRegister);
RW_FUNCTION(0x4E2BB0, "Java_org_cocos2dx_cpp_TrackingSDKManager_getAppConfig", JniCallbacks::getAppConfig);
RW_FUNCTION(0x4E5804, "Java_org_cocos2dx_cpp_FacebookManager_onLogin", JniCallbacks::onLogin);
RW_FUNCTION(0x4E58F0, "Java_org_cocos2dx_cpp_FacebookManager_onLogout", JniCallbacks::onLogout);
RW_FUNCTION(0x4E59DC, "Java_org_cocos2dx_cpp_FacebookManager_onGetFriends", JniCallbacks::onGetFriends);
RW_FUNCTION(0x4E5BC8, "Java_org_cocos2dx_cpp_FacebookManager_onSharePhoto", JniCallbacks::onSharePhoto);
RW_FUNCTION(0x4E5CB4, "Java_org_cocos2dx_cpp_FacebookManager_onInviteFriends", JniCallbacks::onInviteFriends);
RW_FUNCTION(0x4E5EA8, "Java_org_cocos2dx_cpp_FacebookManager_onGetProfileImageUrl",
            JniCallbacks::onGetProfileImageUrl);
RW_FUNCTION(0x4E60A0, "Java_org_cocos2dx_cpp_FacebookManager_onRefreshCurrentAccessToken",
            JniCallbacks::onRefreshCurrentAccessToken);
RW_FUNCTION(0x7C9444, "cocostudio sendEvent(unsigned int)", JniCallbacks::sendEvent);
