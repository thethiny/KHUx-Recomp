// UUID::create: 16 random bytes from java.util.UUID.randomUUID() (most significant 8 bytes first, big endian),
// as a shared_ptr<bg::Data>.
#include "SystemCommon.hpp"

namespace uuid {

namespace {
constexpr uint32_t GET_STATIC_METHOD_INFO = rwlib::cocos2d::JniHelper::getStaticMethodInfo;   // cocos2d::JniHelper::getStaticMethodInfo(info, cls, name, sig)
constexpr uint32_t GET_METHOD_INFO = rwlib::cocos2d::JniHelper::getMethodInfo;          // cocos2d::JniHelper::getMethodInfo(info, cls, name, sig)
constexpr uint32_t CALL_STATIC_OBJECT_METHOD = 0x4E0939;   // _JNIEnv::CallStaticObjectMethod(env, cls, mid, ...)
constexpr uint32_t CALL_LONG_METHOD = 0x4E098D;            // _JNIEnv::CallLongMethod(env, obj, mid, ...)
constexpr uint32_t MAKE_SHARED_DATA = 0x4E0A3D;            // std::make_shared<bg::Data>() (sret)
constexpr uint32_t DATA_COPY = 0x4E09DD;   // bg::Data::copy(data, bytes, size, function<void*(size_t)>, function<void(void*)>)
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C;            // JNINativeInterface::DeleteLocalRef

constexpr uint32_t STR_CLASS = 0xDFC341;        // "java.util.UUID"
constexpr uint32_t STR_RANDOM = 0xDFC350;       // "randomUUID"
constexpr uint32_t STR_RANDOM_SIG = 0xDFC35B;   // "()Ljava/util/UUID;"
constexpr uint32_t STR_LEAST = 0xDFC36E;        // "getLeastSignificantBits"
constexpr uint32_t STR_LONG_SIG = 0xDFC386;     // "()J"
constexpr uint32_t STR_MOST = 0xDFC38A;         // "getMostSignificantBits"

constexpr uint32_t GOT_FUNC_FREE_VTABLE = 0xFD60E4;     // &vtable of __func<void(*)(void*), ...>
constexpr uint32_t GOT_FUNC_MALLOC_VTABLE = 0xFD60F0;   // &vtable of __func<void*(*)(size_t), ...>
constexpr uint32_t GOT_MALLOC = 0xFD64F8;
constexpr uint32_t GOT_FREE = 0xFD64C0;

// ~std::function (libc++): the callable in the buffer is destroyed in place, a heap one destroyed and freed.
void destroyFunction(uint32_t fn) {
    uint32_t f = sys::ld32(fn + 16);
    if (f == fn) rw::call(sys::ld32(sys::ld32(fn) + 16), {fn});
    else if (f) rw::call(sys::ld32(sys::ld32(f) + 20), {f});
}
}  // namespace

void create(guest::SharedPtr *out) {
    guest::Frame f(0x8C);
    const uint32_t most = f.addr(0x1C), least = f.addr(0x28), random = f.addr(0x34);   // JniMethodInfo
    const uint32_t freeFn = f.addr(0x40), mallocFn = f.addr(0x58), bytes = f.addr(0x74);
    rw::call(GET_STATIC_METHOD_INFO, {random, STR_CLASS, STR_RANDOM, STR_RANDOM_SIG});
    uint32_t obj = rw::call(CALL_STATIC_OBJECT_METHOD, {sys::ld32(random), sys::ld32(random + 4), sys::ld32(random + 8)});
    rw::call(GET_METHOD_INFO, {least, STR_CLASS, STR_LEAST, STR_LONG_SIG});
    uint64_t lsb = rw::call64(CALL_LONG_METHOD, {sys::ld32(least), obj, sys::ld32(least + 8)});
    rw::call(GET_METHOD_INFO, {most, STR_CLASS, STR_MOST, STR_LONG_SIG});
    uint64_t msb = rw::call64(CALL_LONG_METHOD, {sys::ld32(most), obj, sys::ld32(most + 8)});
    uint32_t env = sys::ld32(random);
    rw::call(sys::ld32(sys::ld32(env) + JNI_DELETE_LOCAL_REF), {env, obj});
    for (uint32_t k = 0; k < 8; k++) {
        sys::st8(bytes + 7 - k, uint8_t(msb >> (8 * k)));
        sys::st8(bytes + 15 - k, uint8_t(lsb >> (8 * k)));
    }
    rw::call(MAKE_SHARED_DATA, {rw::guest_addr(out)});
    uint32_t data = out->obj;
    // std::function<void*(size_t)>(malloc), std::function<void(void*)>(free) on the stack
    sys::st32(mallocFn + 16, mallocFn);
    sys::st32(mallocFn, sys::ld32(GOT_FUNC_MALLOC_VTABLE) + 8);
    sys::st32(mallocFn + 4, sys::ld32(GOT_MALLOC));
    sys::st32(freeFn + 16, freeFn);
    sys::st32(freeFn, sys::ld32(GOT_FUNC_FREE_VTABLE) + 8);
    sys::st32(freeFn + 4, sys::ld32(GOT_FREE));
    rw::call(DATA_COPY, {data, bytes, 16, mallocFn, freeFn});
    destroyFunction(freeFn);
    destroyFunction(mallocFn);
}
RW_FUNCTION(0x4E0718, "UUID::create", create);

}  // namespace uuid
