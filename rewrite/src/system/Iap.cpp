// iap: the in-app purchase store (Google Play through the Java GooglePlayManager). StoreManager is a singleton
// holding a shared_ptr<StoreManagerImpl>; the impl asks Java for product details (sendProductsRequest) and gets
// the answer through the JNI callbacks below (onFinishInitialize, onRequestProductsResult -> notifyProduct).
//
// StoreManagerImpl: +0 vtable, +4 state (1 = not ready / busy), +8 std::function<void(Result, const
// unordered_map<string, ProductInfo>*)> (the products request's callback), +0x20 another std::function,
// +0x38 initialized (Google Play billing is available), +0x39 a products request was sent.
// ProductInfo: three strings (product id, price, title).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
using Addr = uint32_t;
constexpr uint32_t STORE_MANAGER = 0x10039E4;          // iap::StoreManager (shared_ptr<StoreManagerImpl>)
constexpr uint32_t STORE_MANAGER_GUARD = 0x10039E0;
constexpr uint32_t GOT_StoreManager_dtor = 0xFD64FC;   // &iap::StoreManager::~StoreManager (for __cxa_atexit)
constexpr uint32_t VT_StoreManagerImpl = 0xF7FAF0;

constexpr uint32_t StoreManager_getInstance = 0x4E0DD5;
constexpr uint32_t StoreManager_ctor = 0x4E0E85;
constexpr uint32_t StoreManagerImpl_makeShared = 0x4E2511;   // std::make_shared<StoreManagerImpl>(shared_ptr* out)
constexpr uint32_t StoreManagerImpl_notifyProduct = 0x4E14C5;
constexpr uint32_t ProductInfo_copy = 0x4D9891;
constexpr uint32_t ProductMap_insertMove = 0x4E1F09;         // __insert_unique(pair<string, ProductInfo>&&)
constexpr uint32_t ProductMap_dtor = 0x4DABC7;
constexpr uint32_t FunctionProducts_assign = 0x4E176D;       // std::function<void(Result, const map*)>::operator=
constexpr uint32_t FunctionProducts_call = 0x4E189D;         // operator()(Result, const map*)
constexpr uint32_t AppConfig_get = 0x799FD1;                 // (string* out, const char* key)
constexpr uint32_t JniHelper_getStaticMethodInfo = rwlib::cocos2d::JniHelper::getStaticMethodInfo; // (info, cls, name, sig)
constexpr uint32_t JniHelper_getMethodInfo = rwlib::cocos2d::JniHelper::getMethodInfo;
constexpr uint32_t JNIEnv_CallStaticObjectMethod = 0x4E0939;
constexpr uint32_t JNIEnv_CallVoidMethod = 0x4E17DD;
constexpr uint32_t JNIEnv_CallBooleanMethod = 0x4E16B5;
constexpr uint32_t aeabi_memset = 0x40E7A0;

// JNINativeInterface offsets
constexpr uint32_t JNI_FIND_CLASS = 0x18, JNI_DELETE_LOCAL_REF = 0x5C, JNI_NEW_STRING_UTF = 0x29C;
constexpr uint32_t JNI_GET_STRING_UTF_CHARS = 0x2A4, JNI_RELEASE_STRING_UTF_CHARS = 0x2A8;
constexpr uint32_t JNI_GET_ARRAY_LENGTH = 0x2AC, JNI_NEW_OBJECT_ARRAY = 0x2B0;
constexpr uint32_t JNI_GET_OBJECT_ARRAY_ELEMENT = 0x2B4, JNI_SET_OBJECT_ARRAY_ELEMENT = 0x2B8;

constexpr uint32_t S_GooglePlayManager = 0xDFC444;   // "org/cocos2dx/cpp/GooglePlayManager"
constexpr uint32_t S_GetInstance = 0xDFC411;         // "getInstance"
constexpr uint32_t S_GetInstanceSig = 0xDFC41D;      // "()Lorg/cocos2dx/cpp/GooglePlayManager;"
constexpr uint32_t S_InitWithKey = 0xDFC467;         // "initWithKey"
constexpr uint32_t S_InitWithKeySig = 0xDFC473;      // "(Ljava/lang/String;)Z"
constexpr uint32_t S_LicenseKey = 0xDFC489;          // "googleplay_license_key"
constexpr uint32_t S_SendProductsRequest = 0xDFC4A0; // "sendProductsRequest"
constexpr uint32_t S_SendProductsSig = 0xDFC4B4;     // "([Ljava/lang/String;)V"
constexpr uint32_t S_JavaLangString = 0xDFC4CB;      // "java/lang/String"

constexpr uint32_t RESULT_OK = 1, RESULT_FAILED = 2;

uint32_t jni(uint32_t env, uint32_t offset, std::initializer_list<uint32_t> args) {
    return call(ld32(ld32(env) + offset), args);
}
void deleteLocalRef(uint32_t env, uint32_t ref) { jni(env, JNI_DELETE_LOCAL_REF, {env, ref}); }
uint32_t impl() { return ld32(STORE_MANAGER); }
// std::string copy construction (long strings re-allocated)
void strCopy(uint32_t dst, uint32_t src) {
    if (strIsLong(src)) {
        strInit(dst, ld32(src + 8), ld32(src + 4));
    } else {
        st32(dst, ld32(src));
        st32(dst + 4, ld32(src + 4));
        st32(dst + 8, ld32(src + 8));
    }
}
}  // namespace

// ------------------------------------------------------------------------------------------------ StoreManager
static Addr iap_StoreManager_getInstance() {
    if (guest::staticInitBegin(STORE_MANAGER_GUARD)) {
        call(StoreManager_ctor, {STORE_MANAGER});
        call(guest::CXA_ATEXIT, {ld32(GOT_StoreManager_dtor), STORE_MANAGER, guest::DSO_HANDLE});
        guest::staticInitDone(STORE_MANAGER_GUARD);
    }
    return STORE_MANAGER;
}
RW_FUNCTION(0x4E0DD4, "iap::StoreManager::getInstance()", iap_StoreManager_getInstance);

static Addr iap_StoreManager_construct(Addr self) {
    st32(self, 0);
    st32(self + 4, 0);
    Frame f;
    uint32_t sp = f.alloc(8);
    call(StoreManagerImpl_makeShared, {sp});
    uint32_t ctrl = ld32(sp + 4), obj = ld32(sp);
    st32(sp, 0);
    st32(sp + 4, 0);
    st32(self, obj);
    uint32_t old = ld32(self + 4);
    st32(self + 4, ctrl);
    if (old) call(fn::shared_release, {old});
    return self;
}
RW_FUNCTION(0x4E0E84, "iap::StoreManager::StoreManager()", iap_StoreManager_construct);

// ------------------------------------------------------------------------------------------------ StoreManagerImpl
// Initialise Google Play billing with the license key (GooglePlayManager.initWithKey); the answer comes back
// through onFinishInitialize.
static Addr iap_StoreManagerImpl_construct(Addr self) {
    st32(self, VT_StoreManagerImpl);
    st32(self + 0x18, 0);
    st32(self + 0x30, 0);
    st8(self + 0x39, 0);
    st8(self + 0x38, 0);
    st32(self + 4, 1);
    st8(self + 0x39, 0);
    Frame f;
    uint32_t s = f.alloc(12);
    if (!call(JniHelper_getStaticMethodInfo, {s, S_GooglePlayManager, S_GetInstance, S_GetInstanceSig}))
        return self;
    uint32_t manager = call(JNIEnv_CallStaticObjectMethod, {ld32(s), ld32(s + 4), ld32(s + 8)});
    uint32_t m = f.alloc(12);
    if (call(JniHelper_getMethodInfo, {m, S_GooglePlayManager, S_InitWithKey, S_InitWithKeySig}) != 1) return self;
    uint32_t env = ld32(s);
    uint32_t key = f.alloc(12);
    call(AppConfig_get, {key, S_LicenseKey});
    uint32_t jkey = jni(env, JNI_NEW_STRING_UTF, {env, strData(key)});
    strFree(key);
    call(JNIEnv_CallBooleanMethod, {ld32(m), manager, ld32(m + 8), jkey});
    deleteLocalRef(ld32(s), jkey);
    deleteLocalRef(ld32(s), ld32(m + 4));
    deleteLocalRef(ld32(s), ld32(s + 4));
    return self;
}
RW_FUNCTION(0x4E1524, "iap::StoreManagerImpl::StoreManagerImpl()", iap_StoreManagerImpl_construct);

// Ask Java for the details of these product ids; returns 1 when the request went out (the callback then runs from
// onRequestProductsResult), 0 when billing is not initialised, busy, or the Java side is missing.
static uint32_t iap_StoreManagerImpl_sendProductsRequest(Addr self, Addr productIds, Addr callback) {
    if (!ld8(self + 0x38) || ld32(self + 4) == 1) return 0;
    call(FunctionProducts_assign, {self + 8, callback});
    st8(self + 0x39, 1);
    Frame f;
    uint32_t s = f.alloc(12);
    if (call(JniHelper_getStaticMethodInfo, {s, S_GooglePlayManager, S_GetInstance, S_GetInstanceSig}) != 1) return 0;
    uint32_t manager = call(JNIEnv_CallStaticObjectMethod, {ld32(s), ld32(s + 4), ld32(s + 8)});
    uint32_t m = f.alloc(12);
    if (call(JniHelper_getMethodInfo, {m, S_GooglePlayManager, S_SendProductsRequest, S_SendProductsSig}) != 1)
        return 0;
    uint32_t count = ld32(productIds + 12);
    uint32_t env = ld32(m);
    uint32_t stringClass = jni(env, JNI_FIND_CLASS, {env, S_JavaLangString});
    uint32_t array = jni(env, JNI_NEW_OBJECT_ARRAY, {env, count, stringClass, 0});
    uint32_t i = 0;
    for (uint32_t node = ld32(productIds + 8); node; node = ld32(node)) {
        uint32_t e = ld32(m);
        uint32_t jid = jni(e, JNI_NEW_STRING_UTF, {e, strData(node + 8)});
        e = ld32(m);
        jni(e, JNI_SET_OBJECT_ARRAY_ELEMENT, {e, array, i, jid});
        deleteLocalRef(ld32(s), jid);
        i++;
    }
    call(JNIEnv_CallVoidMethod, {ld32(m), manager, ld32(m + 8), array});
    deleteLocalRef(ld32(s), array);
    deleteLocalRef(ld32(s), manager);
    deleteLocalRef(ld32(s), ld32(m + 4));
    deleteLocalRef(ld32(s), ld32(s + 4));
    return 1;
}
RW_FUNCTION(0x4E0F14, "iap::StoreManagerImpl::sendProductsRequest(const unordered_set<string>&, const function&)",
            iap_StoreManagerImpl_sendProductsRequest);

// the products request's answer: run its callback once (then drop it); ready for the next request
static uint32_t iap_StoreManagerImpl_notifyProduct(Addr self, uint32_t result, Addr products) {
    if (ld32(self + 0x18)) {
        call(FunctionProducts_call, {self + 8, result, products});
        functionDestroy(self + 8);
        st32(self + 0x18, 0);
    }
    st32(self + 4, 0);
    return 0;
}
RW_FUNCTION(0x4E14C4, "iap::StoreManagerImpl::notifyProduct(Result, const unordered_map<string, ProductInfo>*)",
            iap_StoreManagerImpl_notifyProduct);

// ProductInfo(const ProductInfo&)
static Addr iap_ProductInfo_copy(Addr self, Addr other) {
    strCopy(self, other);
    strCopy(self + 12, other + 12);
    strCopy(self + 24, other + 24);
    return self;
}
RW_FUNCTION(0x4D9890, "iap::ProductInfo::ProductInfo(const ProductInfo&)", iap_ProductInfo_copy);

// ------------------------------------------------------------------------------------------------ JNI callbacks
// GooglePlayManager.onFinishInitialize(boolean available)
static Addr Java_GooglePlayManager_onFinishInitialize(Addr /*env*/, Addr /*thiz*/, uint32_t available) {
    call(StoreManager_getInstance, {});
    uint32_t p = impl();
    if (available) st8(p + 0x38, 1);
    st32(p + 4, 0);
    return p;
}
RW_FUNCTION(0x4E19A8, "Java_org_cocos2dx_cpp_GooglePlayManager_onFinishInitialize",
            Java_GooglePlayManager_onFinishInitialize);

// GooglePlayManager.onRequestProductsResult(boolean ok, String[] ids, String[] prices, String[] titles): the
// products as a map id -> ProductInfo for notifyProduct
static void Java_GooglePlayManager_onRequestProductsResult(Addr env, Addr /*thiz*/, uint32_t ok, Addr ids,
                                                          Addr prices, Addr titles) {
    Frame f;
    uint32_t map = f.alloc(20);
    for (uint32_t k = 0; k < 16; k += 4) st32(map + k, 0);
    stf(map + 16, 1.0f);
    if (!ok) {
        call(StoreManager_getInstance, {});
        call(StoreManagerImpl_notifyProduct, {impl(), RESULT_FAILED, 0});
        call(ProductMap_dtor, {map});
        return;
    }
    int32_t count = int32_t(jni(env, JNI_GET_ARRAY_LENGTH, {env, ids}));
    for (int32_t i = 0; i < count; i++) {
        Frame t;
        uint32_t jid = jni(env, JNI_GET_OBJECT_ARRAY_ELEMENT, {env, ids, uint32_t(i)});
        uint32_t id = jni(env, JNI_GET_STRING_UTF_CHARS, {env, jid, 0});
        uint32_t jprice = jni(env, JNI_GET_OBJECT_ARRAY_ELEMENT, {env, prices, uint32_t(i)});
        uint32_t price = jni(env, JNI_GET_STRING_UTF_CHARS, {env, jprice, 0});
        uint32_t jtitle = jni(env, JNI_GET_OBJECT_ARRAY_ELEMENT, {env, titles, uint32_t(i)});
        uint32_t title = jni(env, JNI_GET_STRING_UTF_CHARS, {env, jtitle, 0});
        uint32_t info = t.alloc(36);
        call(aeabi_memset, {info, 0x24, 0});
        call(fn::string_assign, {info, id, host_strlen(id)});
        call(fn::string_assign, {info + 12, price, host_strlen(price)});
        call(fn::string_assign, {info + 24, title, host_strlen(title)});
        // pair<string, ProductInfo>{info.id, info}, moved into the map
        uint32_t pair = t.alloc(48);
        strCopy(pair, info);
        call(ProductInfo_copy, {pair + 12, info});
        uint32_t result = t.alloc(8);
        call(ProductMap_insertMove, {result, map, pair});
        strFree(pair + 36);
        strFree(pair + 24);
        strFree(pair + 12);
        strFree(pair);
        jni(env, JNI_RELEASE_STRING_UTF_CHARS, {env, jid, id});
        jni(env, JNI_RELEASE_STRING_UTF_CHARS, {env, jprice, price});
        jni(env, JNI_RELEASE_STRING_UTF_CHARS, {env, jtitle, title});
        strFree(info + 24);
        strFree(info + 12);
        strFree(info);
    }
    call(StoreManager_getInstance, {});
    call(StoreManagerImpl_notifyProduct, {impl(), RESULT_OK, map});
    call(ProductMap_dtor, {map});
}
RW_FUNCTION(0x4E19D4, "Java_org_cocos2dx_cpp_GooglePlayManager_onRequestProductsResult",
            Java_GooglePlayManager_onRequestProductsResult);
