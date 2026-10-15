// _JNIEnv: the inline C++ helpers of jni.h (env->CallXMethod(obj, method, ...)), each forwarding its variadic
// arguments as a va_list to the JNI function table's CallXMethodV.
#include "SystemCommon.hpp"

namespace jnienv {

namespace {
// JNINativeInterface offsets of the V (va_list) functions
constexpr uint32_t NEW_OBJECT_V = 0x74;
constexpr uint32_t CALL_OBJECT_METHOD_V = 0x8C;
constexpr uint32_t CALL_BOOLEAN_METHOD_V = 0x98;
constexpr uint32_t CALL_INT_METHOD_V = 0xC8;
constexpr uint32_t CALL_LONG_METHOD_V = 0xD4;
constexpr uint32_t CALL_FLOAT_METHOD_V = 0xE0;
constexpr uint32_t CALL_VOID_METHOD_V = 0xF8;
constexpr uint32_t CALL_STATIC_OBJECT_METHOD_V = 0x1CC;
constexpr uint32_t CALL_STATIC_BOOLEAN_METHOD_V = 0x1D8;
constexpr uint32_t CALL_STATIC_INT_METHOD_V = 0x208;
constexpr uint32_t CALL_STATIC_FLOAT_METHOD_V = 0x220;
constexpr uint32_t CALL_STATIC_DOUBLE_METHOD_V = 0x22C;
constexpr uint32_t CALL_STATIC_VOID_METHOD_V = 0x238;

// env->functions->X(env, target, method, va_list of the arguments after method)
template <uint32_t OFFSET> uint64_t forward(uint32_t env, uint32_t target, uint32_t method) {
    sys::VaSpill va(3);
    uint32_t fn = sys::ld32(sys::ld32(env) + OFFSET);
    return rw::call64(fn, {env, target, method, va.va()});
}
template <uint32_t OFFSET> uint32_t forward32(uint32_t env, uint32_t target, uint32_t method) {
    return uint32_t(forward<OFFSET>(env, target, method));
}
template <uint32_t OFFSET> void forwardVoid(uint32_t env, uint32_t target, uint32_t method) {
    forward<OFFSET>(env, target, method);
}
}  // namespace

uint32_t CallStaticObjectMethod(uint32_t env, uint32_t cls, uint32_t method) {
    return forward32<CALL_STATIC_OBJECT_METHOD_V>(env, cls, method);
}
RW_FUNCTION(0x4E0938, "_JNIEnv::CallStaticObjectMethod", CallStaticObjectMethod);

uint64_t CallLongMethod(uint32_t env, uint32_t obj, uint32_t method) {
    return forward<CALL_LONG_METHOD_V>(env, obj, method);
}
RW_FUNCTION(0x4E098C, "_JNIEnv::CallLongMethod", CallLongMethod);

void CallStaticVoidMethod(uint32_t env, uint32_t cls, uint32_t method) {
    forwardVoid<CALL_STATIC_VOID_METHOD_V>(env, cls, method);
}
RW_FUNCTION(0x4E0D08, "_JNIEnv::CallStaticVoidMethod", CallStaticVoidMethod);

uint32_t CallBooleanMethod(uint32_t env, uint32_t obj, uint32_t method) {
    return forward32<CALL_BOOLEAN_METHOD_V>(env, obj, method);
}
RW_FUNCTION(0x4E16B4, "_JNIEnv::CallBooleanMethod", CallBooleanMethod);

void CallVoidMethod(uint32_t env, uint32_t obj, uint32_t method) {
    forwardVoid<CALL_VOID_METHOD_V>(env, obj, method);
}
RW_FUNCTION(0x4E17DC, "_JNIEnv::CallVoidMethod", CallVoidMethod);

uint32_t CallObjectMethod(uint32_t env, uint32_t obj, uint32_t method) {
    return forward32<CALL_OBJECT_METHOD_V>(env, obj, method);
}
RW_FUNCTION(0x4E49AC, "_JNIEnv::CallObjectMethod", CallObjectMethod);

uint32_t CallStaticBooleanMethod(uint32_t env, uint32_t cls, uint32_t method) {
    return forward32<CALL_STATIC_BOOLEAN_METHOD_V>(env, cls, method);
}
RW_FUNCTION(0x81AB18, "_JNIEnv::CallStaticBooleanMethod", CallStaticBooleanMethod);

uint32_t CallStaticIntMethod(uint32_t env, uint32_t cls, uint32_t method) {
    return forward32<CALL_STATIC_INT_METHOD_V>(env, cls, method);
}
RW_FUNCTION(0x81AB6C, "_JNIEnv::CallStaticIntMethod", CallStaticIntMethod);

uint32_t NewObject(uint32_t env, uint32_t cls, uint32_t method) {
    return forward32<NEW_OBJECT_V>(env, cls, method);
}
RW_FUNCTION(0x81BF88, "_JNIEnv::NewObject", NewObject);

uint32_t CallFloatMethod(uint32_t env, uint32_t obj, uint32_t method) {
    return forward32<CALL_FLOAT_METHOD_V>(env, obj, method);   // the float's bits in r0 (softfp)
}
RW_FUNCTION(0x820920, "_JNIEnv::CallFloatMethod", CallFloatMethod);

uint32_t CallIntMethod(uint32_t env, uint32_t obj, uint32_t method) {
    return forward32<CALL_INT_METHOD_V>(env, obj, method);
}
RW_FUNCTION(0x820970, "_JNIEnv::CallIntMethod", CallIntMethod);

uint32_t CallStaticFloatMethod(uint32_t env, uint32_t cls, uint32_t method) {
    return forward32<CALL_STATIC_FLOAT_METHOD_V>(env, cls, method);
}
RW_FUNCTION(0x937E74, "_JNIEnv::CallStaticFloatMethod", CallStaticFloatMethod);

uint64_t CallStaticDoubleMethod(uint32_t env, uint32_t cls, uint32_t method) {
    return forward<CALL_STATIC_DOUBLE_METHOD_V>(env, cls, method);   // the double's bits in r0:r1 (softfp)
}
RW_FUNCTION(0xA416D8, "_JNIEnv::CallStaticDoubleMethod", CallStaticDoubleMethod);

}  // namespace jnienv
