// BGFacebook::FacebookManager(Impl) and the iap store's destructor: the Facebook login / friends / share / invite
// bridge to the Java FacebookManager (JNI), and its callbacks. The private server has no Facebook and no store, so
// none of this can run: written from the original, UNVERIFIED (user, 2026-10-22).
//
// FacebookManagerImpl: +0 the request in progress (1 login, 2 logout, 3 friends, 4 share, 5 invite, 6 profile
// image, 7 token refresh; 0 none), +4 connecting, +5 logged in, the callbacks (std::function) +0x08 login,
// +0x20 logout, +0x38 friends, +0x50 share, +0x68 invite, +0x80 profile image, +0x98 refresh, +0xB0 the
// connecting shadow.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t JniHelper_getStaticMethodInfo = 0xA4225D, JniHelper_getMethodInfo = 0xA422DD;
constexpr uint32_t JNIEnv_CallStaticObjectMethod = 0x4E0939, JNIEnv_CallBooleanMethod = 0x4E16B5;
constexpr uint32_t JNIEnv_CallVoidMethod = 0x4E17DD, JNIEnv_CallObjectMethod = 0x4E49AD;
// JNIEnv function table slots
constexpr uint32_t JNI_FindClass = 0x18, JNI_DeleteLocalRef = 0x5C, JNI_NewStringUTF = 0x29C;
constexpr uint32_t JNI_GetStringUTFChars = 0x2A4, JNI_ReleaseStringUTFChars = 0x2A8;
constexpr uint32_t JNI_NewObjectArray = 0x2B0, JNI_SetObjectArrayElement = 0x2B8;
constexpr uint32_t S_Class = 0xDFC634, S_GetInstance = 0xDFC411, S_GetInstanceSig = 0xDFC60F;
constexpr uint32_t S_Busy = 0xDFC6C3;                      // 21 characters: a request is already running
constexpr uint32_t S_Cancelled = 0xDFC675;                 // 39 characters
constexpr uint32_t S_NoGetInstance = 0xDFC6D9;             // "not found: static FacebookManager.getInstance()"
constexpr uint32_t S_Empty = 0xDF8A1A, S_JavaString = 0xDFC4CB;
constexpr uint32_t Impl_startConnection = 0x4E2EFD, Impl_hasPermissions = 0x4E34D5;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_getPopupLayer = 0x613E69, SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t CallFunc_create = 0x866B39, Sequence_create = 0x867999;
constexpr uint32_t make_shared_Impl = 0x4E618D;
constexpr uint32_t vector_string_copy = 0x47A9A5, vector_string_dtor = 0x47363F;
constexpr uint32_t libc_strlen = 0x40E08C;
constexpr uint32_t VT_ShadowRemove = 0xF55868;             // hideConnectingShadow's CallFunc lambda {this}
constexpr uint32_t GOT_StoreManagerImpl_vtable = 0xFD6500;   // the vtable symbol (vptr = +8)
constexpr uint32_t F_STATE = 0, F_CONNECTING = 4, F_LOGGED_IN = 5, F_SHADOW = 0xB0;

// the three callback types: (bool, ErrorCode, string), (..., vector<string>*), (..., string)
enum Kind { K_PLAIN, K_VECTOR, K_STRING };
constexpr uint32_t CB_CALL[3] = {0x4E33B9, 0x4E3325, 0x4E3441};
constexpr uint32_t CB_ASSIGN[3] = {0x4E3885, 0x4E3F79, 0x4E47F1};
constexpr uint32_t CB_COPY[3] = {0x4E4DB5, 0x4E5039, 0x4E541D};

uint32_t jni(uint32_t env, uint32_t slot, std::initializer_list<uint32_t> args) {
    uint32_t a[5] = {env, 0, 0, 0, 0};
    uint32_t n = 1;
    for (uint32_t x : args) a[n++] = x;
    const uint32_t f = ld32(ld32(env) + slot);
    switch (n) {
    case 2: return call(f, {a[0], a[1]});
    case 3: return call(f, {a[0], a[1], a[2]});
    case 4: return call(f, {a[0], a[1], a[2], a[3]});
    default: return call(f, {a[0], a[1], a[2], a[3], a[4]});
    }
}
void deleteRef(uint32_t env, uint32_t ref) { jni(env, JNI_DeleteLocalRef, {ref}); }
uint32_t cstr(uint32_t s) { return strIsLong(s) ? ld32(s + 8) : s + 1; }
// a std::string copy into dst (libc++: a long string through init, a short one as its three words)
void strCopy(uint32_t dst, uint32_t src) {
    if (strIsLong(src)) strInit(dst, ld32(src + 8), ld32(src + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
}
void zero12(uint32_t p) { for (uint32_t k = 0; k < 12; k += 4) st32(p + k, 0); }

// the request ended: nothing in progress, the loading indicator off
void endRequest(uint32_t self) {
    st8(self + F_CONNECTING, 0);
    st32(self + F_STATE, 0);
    call(SceneManager_enableLoadingIndicator, {call(fn::SceneManager_getInstance, {}), 0});
}

// what the error paths build besides the message: a zeroed vector<string> passed as the extra argument, one that
// is built and destroyed unused, or nothing
enum Vec { V_NONE, V_PASS, V_UNUSED };
struct Request {
    uint32_t state, slot, kind;
    uint32_t name, sig, notFound, notFoundLen;
    Vec staticVec, methodVec;
};

// cb(false, code, message[, extra]); the extra is a zeroed vector (vec), 0 or an empty string by kind
void fail(uint32_t cb, uint32_t kind, uint32_t code, uint32_t text, uint32_t len, Vec vec) {
    Frame f;
    uint32_t v = 0;
    if (vec != V_NONE) {
        v = f.alloc(12);
        zero12(v);
    }
    const uint32_t msg = newStr(f, text, len);
    if (kind == K_PLAIN) call(CB_CALL[kind], {cb, 0, code, msg});
    else if (kind == K_VECTOR) call(CB_CALL[kind], {cb, 0, code, msg, vec == V_PASS ? v : 0u});
    else {
        const uint32_t extra = newStr(f, S_Empty, 0);
        call(CB_CALL[kind], {cb, 0, code, msg, extra});
        strFree(extra);
    }
    strFree(msg);
    if (v) call(vector_string_dtor, {v});
}

// a request is already running: cb(false, 1, ...). The profile image request builds its extra string from a null
// pointer (strlen(0)): kept as in the game
void busy(uint32_t cb, uint32_t kind) {
    Frame f;
    const uint32_t msg = newStr(f, S_Busy, 0x15);
    if (kind == K_PLAIN) call(CB_CALL[kind], {cb, 0, 1, msg});
    else if (kind == K_VECTOR) call(CB_CALL[kind], {cb, 0, 1, msg, 0});
    else {
        const uint32_t n = call(libc_strlen, {0});
        const uint32_t extra = newStr(f, 0, n);
        call(CB_CALL[kind], {cb, 0, 1, msg, extra});
        strFree(extra);
    }
    strFree(msg);
}

// FacebookManager.getInstance().<name>(...): the callback kept, the Java call made by `invoke(env, obj, mid)`
// (which also deletes the local references it made), then obj, the method's class and the static class deleted
template <class Invoke>
uint32_t request(uint32_t self, uint32_t cb, const Request &r, Invoke invoke) {
    if (!call(Impl_startConnection, {self})) {
        busy(cb, r.kind);
        return 0;
    }
    st32(self + F_STATE, r.state);
    call(CB_ASSIGN[r.kind], {self + r.slot, cb});
    Frame f;
    const uint32_t si = f.alloc(12), mi = f.alloc(12);
    if (!call(JniHelper_getStaticMethodInfo, {si, S_Class, S_GetInstance, S_GetInstanceSig})) {
        endRequest(self);
        fail(cb, r.kind, 4, S_NoGetInstance, 0x2F, r.staticVec);
        return 0;
    }
    const uint32_t obj = call(JNIEnv_CallStaticObjectMethod, {ld32(si), ld32(si + 4), ld32(si + 8)});
    if (!call(JniHelper_getMethodInfo, {mi, S_Class, r.name, r.sig})) {
        endRequest(self);
        fail(cb, r.kind, 4, r.notFound, r.notFoundLen, r.methodVec);
        return 0;
    }
    invoke(ld32(si), ld32(mi), obj, ld32(mi + 8));
    const uint32_t env = ld32(si);
    deleteRef(env, obj);
    deleteRef(env, ld32(mi + 4));
    deleteRef(env, ld32(si + 4));
    return 0;
}

const Request LOGIN{1, 0x08, K_PLAIN, 0xDFA60C, 0xDFC75E, 0xDFC763, 0x22, V_NONE, V_NONE};
const Request LOGOUT{2, 0x20, K_PLAIN, 0xDFC786, 0xDFC723, 0xDFC78D, 0x23, V_NONE, V_NONE};
const Request FRIENDS{3, 0x38, K_VECTOR, 0xDFC7B1, 0xDFC3EF, 0xDFC7BC, 0x27, V_PASS, V_PASS};
const Request SHARE{4, 0x50, K_PLAIN, 0xDFC7E4, 0xDFC7EF, 0xDFC818, 0x27, V_UNUSED, V_NONE};
const Request INVITE{5, 0x68, K_VECTOR, 0xDFC840, 0xDFC5B0, 0xDFC854, 0x30, V_NONE, V_NONE};
const Request PROFILE{6, 0x80, K_STRING, 0xDFC885, 0xDFC4E5, 0xDFC898, 0x2F, V_UNUSED, V_NONE};
const Request REFRESH{7, 0x98, K_PLAIN, 0xDFC709, 0xDFC723, 0xDFC727, 0x36, V_NONE, V_NONE};

// FacebookManager.getInstance().<name>() returning a java String, as a std::string into out (empty when the Java
// side is missing)
uint32_t javaString(uint32_t out, uint32_t name) {
    Frame f;
    const uint32_t si = f.alloc(12), mi = f.alloc(12);
    if (call(JniHelper_getStaticMethodInfo, {si, S_Class, S_GetInstance, S_GetInstanceSig})) {
        const uint32_t obj = call(JNIEnv_CallStaticObjectMethod, {ld32(si), ld32(si + 4), ld32(si + 8)});
        if (call(JniHelper_getMethodInfo, {mi, S_Class, name, 0xDFC8DE})) {
            const uint32_t env = ld32(si);
            const uint32_t js = call(JNIEnv_CallObjectMethod, {ld32(mi), obj, ld32(mi + 8)});
            const uint32_t chars = jni(env, JNI_GetStringUTFChars, {js, 0});
            const uint32_t tmp = newStr(f, chars);
            jni(env, JNI_ReleaseStringUTFChars, {js, chars});
            deleteRef(env, obj);
            deleteRef(env, ld32(mi + 4));
            deleteRef(env, ld32(si + 4));
            for (uint32_t k = 0; k < 12; k += 4) st32(out + k, ld32(tmp + k));
            zero12(tmp);
            return 0;
        }
    }
    strInit(out, S_Empty, 0);
    return 0;
}

// the callbacks from Java: the request ends, the kept callback is taken out (its slot emptied) and called
void answer(uint32_t self, uint32_t slot, uint32_t kind, uint32_t ok, uint32_t code, uint32_t text, uint32_t extra,
            int loggedIn) {
    endRequest(self);
    Frame f;
    const uint32_t cb = f.alloc(20);
    call(CB_COPY[kind], {cb, self + slot});
    functionDestroy(self + slot);
    st32(self + slot + 16, 0);
    if (loggedIn >= 0) st8(self + F_LOGGED_IN, uint32_t(loggedIn));
    const uint32_t msg = f.alloc(12);
    strCopy(msg, text);
    if (kind == K_PLAIN) call(CB_CALL[kind], {cb, ok, code, msg});
    else if (kind == K_VECTOR) call(CB_CALL[kind], {cb, ok, code, msg, extra});
    else {
        const uint32_t url = f.alloc(12);
        strCopy(url, extra);
        call(CB_CALL[kind], {cb, ok, code, msg, url});
        strFree(url);
    }
    strFree(msg);
    functionDestroy(cb);
}
}  // namespace

namespace facebook {

uint32_t implCtor(uint32_t self) {
    st32(self + F_STATE, 0);
    st8(self + F_CONNECTING, 0);
    st8(self + F_LOGGED_IN, 0);
    for (uint32_t slot = 0x08; slot <= 0x98; slot += 0x18) st32(slot + self + 16, 0);
    st32(self + F_SHADOW, 0);
    Frame f;
    const uint32_t si = f.alloc(12), mi = f.alloc(12);
    if (call(JniHelper_getStaticMethodInfo, {si, S_Class, S_GetInstance, S_GetInstanceSig})) {
        const uint32_t obj = call(JNIEnv_CallStaticObjectMethod, {ld32(si), ld32(si + 4), ld32(si + 8)});
        if (call(JniHelper_getMethodInfo, {mi, S_Class, 0xDFC655, 0xDFC660}) == 1) {
            st8(self + F_LOGGED_IN, call(JNIEnv_CallBooleanMethod, {ld32(mi), obj, ld32(mi + 8)}) ? 1u : 0u);
            const uint32_t env = ld32(si);
            deleteRef(env, obj);
            deleteRef(env, ld32(mi + 4));
            deleteRef(env, ld32(si + 4));
        }
    }
    return self;
}
RW_FUNCTION(0x4E2D18, "BGFacebook::FacebookManagerImpl::FacebookManagerImpl", facebook::implCtor);

uint32_t implDtor(uint32_t self) {
    for (uint32_t slot = 0x98; slot >= 0x08; slot -= 0x18) functionDestroy(self + slot);
    return self;
}
RW_FUNCTION(0x4E6266, "BGFacebook::FacebookManagerImpl::~FacebookManagerImpl", facebook::implDtor);

// Java's cancelConnecting(): when it cancelled, the running request's callback gets (false, 2, ...) and is
// emptied (login / logout have none to call); the request ends either way it was cancelled
uint32_t cancelConnecting(uint32_t self) {
    if (!ld8(self + F_CONNECTING)) return 0;
    Frame f;
    const uint32_t si = f.alloc(12), mi = f.alloc(12);
    if (call(JniHelper_getStaticMethodInfo, {si, S_Class, S_GetInstance, S_GetInstanceSig}) != 1) return 0;
    const uint32_t obj = call(JNIEnv_CallStaticObjectMethod, {ld32(si), ld32(si + 4), ld32(si + 8)});
    if (call(JniHelper_getMethodInfo, {mi, S_Class, 0xDFC664, 0xDFC660}) != 1) return 0;
    const uint32_t cancelled = call(JNIEnv_CallBooleanMethod, {ld32(mi), obj, ld32(mi + 8)});
    const uint32_t env = ld32(si);
    deleteRef(env, obj);
    deleteRef(env, ld32(mi + 4));
    deleteRef(env, ld32(si + 4));
    if (!cancelled) return 0;
    static const uint32_t SLOT[5] = {0x38, 0x50, 0x68, 0x80, 0x98};
    static const uint32_t KIND[5] = {K_VECTOR, K_PLAIN, K_VECTOR, K_STRING, K_PLAIN};
    const uint32_t state = ld32(self + F_STATE);
    if (state >= 3 && state <= 7) {
        const uint32_t slot = self + SLOT[state - 3], kind = KIND[state - 3];
        if (ld32(slot + 16)) {
            Frame g;
            const uint32_t msg = newStr(g, S_Cancelled, 0x27);
            if (kind == K_PLAIN) call(CB_CALL[kind], {slot, 0, 2, msg});
            else if (kind == K_VECTOR) call(CB_CALL[kind], {slot, 0, 2, msg, 0});
            else {
                const uint32_t extra = newStr(g, S_Empty, 0);
                call(CB_CALL[kind], {slot, 0, 2, msg, extra});
                strFree(extra);
            }
            strFree(msg);
            functionDestroy(slot);
            st32(slot + 16, 0);
        }
    }
    endRequest(self);
    return 0;
}
RW_FUNCTION(0x4E2F3C, "BGFacebook::FacebookManagerImpl::cancelConnecting", facebook::cancelConnecting);

// Java's hasPermissions(String[]) while logged in
uint32_t implHasPermissions(uint32_t self, uint32_t names) {
    if (!ld8(self + F_LOGGED_IN)) return 0;
    Frame f;
    const uint32_t si = f.alloc(12), mi = f.alloc(12);
    if (call(JniHelper_getStaticMethodInfo, {si, S_Class, S_GetInstance, S_GetInstanceSig}) != 1) return 0;
    const uint32_t obj = call(JNIEnv_CallStaticObjectMethod, {ld32(si), ld32(si + 4), ld32(si + 8)});
    if (call(JniHelper_getMethodInfo, {mi, S_Class, 0xDFC69D, 0xDFC6AC}) != 1) return 0;
    const uint32_t env = ld32(si);
    auto count = [&] { return (ld32(names + 4) - ld32(names)) / 12; };
    const uint32_t cls = jni(env, JNI_FindClass, {S_JavaString});
    const uint32_t arr = jni(env, JNI_NewObjectArray, {count(), cls, 0});
    for (uint32_t i = 0; i < count(); i++) {
        const uint32_t js = jni(env, JNI_NewStringUTF, {cstr(ld32(names) + 12 * i)});
        jni(env, JNI_SetObjectArrayElement, {arr, i, js});
    }
    const uint32_t r = call(JNIEnv_CallBooleanMethod, {ld32(mi), obj, ld32(mi + 8), arr});
    deleteRef(env, obj);
    deleteRef(env, ld32(mi + 4));
    deleteRef(env, ld32(si + 4));
    deleteRef(env, arr);
    return r ? 1 : 0;
}
RW_FUNCTION(0x4E34D4, "BGFacebook::FacebookManagerImpl::hasPermissions", facebook::implHasPermissions);

uint32_t refreshCurrentAccessToken(uint32_t self, uint32_t cb) {
    return request(self, cb, REFRESH, [](uint32_t, uint32_t env, uint32_t obj, uint32_t mid) {
        call(JNIEnv_CallVoidMethod, {env, obj, mid});
    });
}
RW_FUNCTION(0x4E3670, "BGFacebook::FacebookManagerImpl::refreshCurrentAccessToken", facebook::refreshCurrentAccessToken);

uint32_t login(uint32_t self, uint32_t publish, uint32_t cb) {
    return request(self, cb, LOGIN, [&](uint32_t, uint32_t env, uint32_t obj, uint32_t mid) {
        call(JNIEnv_CallVoidMethod, {env, obj, mid, publish});
    });
}
RW_FUNCTION(0x4E38F4, "BGFacebook::FacebookManagerImpl::login", facebook::login);

uint32_t logout(uint32_t self, uint32_t cb) {
    return request(self, cb, LOGOUT, [](uint32_t, uint32_t env, uint32_t obj, uint32_t mid) {
        call(JNIEnv_CallVoidMethod, {env, obj, mid});
    });
}
RW_FUNCTION(0x4E3B10, "BGFacebook::FacebookManagerImpl::logout", facebook::logout);

uint32_t getFriends(uint32_t self, uint32_t limit, uint32_t cb) {
    return request(self, cb, FRIENDS, [&](uint32_t, uint32_t env, uint32_t obj, uint32_t mid) {
        call(JNIEnv_CallVoidMethod, {env, obj, mid, limit});
    });
}
RW_FUNCTION(0x4E3D24, "BGFacebook::FacebookManagerImpl::getFriends", facebook::getFriends);

uint32_t sharePhoto(uint32_t self, uint32_t flag, uint32_t a, uint32_t b, uint32_t cb) {
    return request(self, cb, SHARE, [&](uint32_t senv, uint32_t env, uint32_t obj, uint32_t mid) {
        const uint32_t ja = jni(senv, JNI_NewStringUTF, {cstr(a)});
        const uint32_t jb = jni(senv, JNI_NewStringUTF, {cstr(b)});
        call(JNIEnv_CallVoidMethod, {env, obj, mid, flag, ja, jb});
        deleteRef(senv, ja);
        deleteRef(senv, jb);
    });
}
RW_FUNCTION(0x4E3FE8, "BGFacebook::FacebookManagerImpl::sharePhoto", facebook::sharePhoto);

uint32_t inviteFriends(uint32_t self, uint32_t a, uint32_t b, uint32_t cb) {
    return request(self, cb, INVITE, [&](uint32_t senv, uint32_t env, uint32_t obj, uint32_t mid) {
        const uint32_t ja = jni(senv, JNI_NewStringUTF, {cstr(a)});
        const uint32_t jb = jni(senv, JNI_NewStringUTF, {cstr(b)});
        call(JNIEnv_CallVoidMethod, {env, obj, mid, ja, jb});
        deleteRef(senv, ja);
        deleteRef(senv, jb);
    });
}
RW_FUNCTION(0x4E4274, "BGFacebook::FacebookManagerImpl::inviteFriends", facebook::inviteFriends);

// the user id string is turned into a java String that is never deleted (as in the game)
uint32_t getProfileImageUrl(uint32_t self, uint32_t user, uint32_t cb) {
    return request(self, cb, PROFILE, [&](uint32_t senv, uint32_t env, uint32_t obj, uint32_t mid) {
        const uint32_t js = jni(senv, JNI_NewStringUTF, {cstr(user)});
        call(JNIEnv_CallVoidMethod, {env, obj, mid, js});
    });
}
RW_FUNCTION(0x4E44F4, "BGFacebook::FacebookManagerImpl::getProfileImageUrl", facebook::getProfileImageUrl);

uint32_t getCurrentAccessToken(uint32_t out, uint32_t) { return javaString(out, 0xDFC8C8); }
RW_FUNCTION(0x4E4860, "BGFacebook::FacebookManagerImpl::getCurrentAccessToken", facebook::getCurrentAccessToken);
uint32_t getUserID(uint32_t out, uint32_t) { return javaString(out, 0xDFC8F3); }
RW_FUNCTION(0x4E49FC, "BGFacebook::FacebookManagerImpl::getUserID", facebook::getUserID);

uint32_t showConnectingShadow(uint32_t self) {
    if (ld32(self + F_SHADOW)) return ld32(self + F_SHADOW);
    const uint32_t layer = call(SceneManager_getPopupLayer, {call(fn::SceneManager_getInstance, {})});
    const uint32_t shadow = call(SceneUtil_createShadowSprite, {layer});
    st32(self + F_SHADOW, shadow);
    const uint32_t opacity = vcall(shadow, 0x20C, {shadow});
    vcall(shadow, 0x214, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
    return vcall(layer, 0xF0, {layer, shadow});
}
RW_FUNCTION(0x4E4B48, "BGFacebook::FacebookManagerImpl::showConnectingShadow", facebook::showConnectingShadow);

uint32_t hideConnectingShadow(uint32_t self) {
    const uint32_t shadow = ld32(self + F_SHADOW);
    if (!shadow) return 0;
    const uint32_t fade = call(fn::FadeTo_create, {fbits(0.1f), 0});
    Frame f;
    const uint32_t fo = newFunction(f, VT_ShadowRemove, {self});
    const uint32_t then = call(CallFunc_create, {fo});
    call(fn::Node_runAction, {shadow, call(Sequence_create, {fade, then, 0})});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x4E4BB8, "BGFacebook::FacebookManagerImpl::hideConnectingShadow", facebook::hideConnectingShadow);

uint32_t onLogin(uint32_t self, uint32_t ok, uint32_t code, uint32_t text) {
    answer(self, 0x08, K_PLAIN, ok, code, text, 0, ok == 1 ? 1 : -1);
    return 0;
}
RW_FUNCTION(0x4E4C8C, "BGFacebook::FacebookManagerImpl::onLogin", facebook::onLogin);
uint32_t onLogout(uint32_t self, uint32_t ok, uint32_t code, uint32_t text) {
    answer(self, 0x20, K_PLAIN, ok, code, text, 0, ok == 1 ? 0 : -1);
    return 0;
}
RW_FUNCTION(0x4E4DE8, "BGFacebook::FacebookManagerImpl::onLogout", facebook::onLogout);
uint32_t onGetFriends(uint32_t self, uint32_t ok, uint32_t code, uint32_t text, uint32_t friends) {
    answer(self, 0x38, K_VECTOR, ok, code, text, friends, -1);
    return 0;
}
RW_FUNCTION(0x4E4F10, "BGFacebook::FacebookManagerImpl::onGgetFriends", facebook::onGetFriends);
uint32_t onSharePhoto(uint32_t self, uint32_t ok, uint32_t code, uint32_t text) {
    answer(self, 0x50, K_PLAIN, ok, code, text, 0, -1);
    return 0;
}
RW_FUNCTION(0x4E506C, "BGFacebook::FacebookManagerImpl::onSharePhoto", facebook::onSharePhoto);
uint32_t onInviteFriends(uint32_t self, uint32_t ok, uint32_t code, uint32_t text, uint32_t friends) {
    answer(self, 0x68, K_VECTOR, ok, code, text, friends, -1);
    return 0;
}
RW_FUNCTION(0x4E518C, "BGFacebook::FacebookManagerImpl::onInviteFriends", facebook::onInviteFriends);
uint32_t onGetProfileImageUrl(uint32_t self, uint32_t ok, uint32_t code, uint32_t text, uint32_t url) {
    answer(self, 0x80, K_STRING, ok, code, text, url, -1);
    return 0;
}
RW_FUNCTION(0x4E52B4, "BGFacebook::FacebookManagerImpl::onGetProfileImageUrl", facebook::onGetProfileImageUrl);
uint32_t onRefreshCurrentAccessToken(uint32_t self, uint32_t ok, uint32_t code, uint32_t text) {
    answer(self, 0x98, K_PLAIN, ok, code, text, 0, ok ? -1 : 0);
    return 0;
}
RW_FUNCTION(0x4E5450, "BGFacebook::FacebookManagerImpl::onRefreshCurrentAccessToken", facebook::onRefreshCurrentAccessToken);

// FacebookManager: a shared_ptr to its Impl
uint32_t managerCtor(uint32_t self) {
    st32(self, 0);
    st32(self + 4, 0);
    Frame f;
    const uint32_t sp = f.alloc(8);
    call(make_shared_Impl, {sp});
    const uint32_t old = ld32(self + 4);
    st32(self, ld32(sp));
    st32(self + 4, ld32(sp + 4));
    st32(sp, 0);
    st32(sp + 4, 0);
    if (old) call(fn::shared_release, {old});
    return self;
}
RW_FUNCTION(0x4E5634, "BGFacebook::FacebookManager::FacebookManager", facebook::managerCtor);

uint32_t managerHasPermissions(uint32_t self, uint32_t names) {
    Frame f;
    const uint32_t copy = f.alloc(12);
    call(vector_string_copy, {copy, names});
    const uint32_t r = call(Impl_hasPermissions, {ld32(self), copy});
    call(vector_string_dtor, {copy});
    return r;
}
RW_FUNCTION(0x4E56C8, "BGFacebook::FacebookManager::hasPermissions", facebook::managerHasPermissions);

// iap::StoreManagerImpl: its two callbacks (+0x08, +0x20)
uint32_t storeDtor(uint32_t self) {
    st32(self, ld32(GOT_StoreManagerImpl_vtable) + 8);
    functionDestroy(self + 0x20);
    functionDestroy(self + 0x08);
    return self;
}
RW_FUNCTION(0x4E1704, "iap::StoreManagerImpl::~StoreManagerImpl", facebook::storeDtor);

}  // namespace facebook
