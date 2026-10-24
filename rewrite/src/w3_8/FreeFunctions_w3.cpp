// Free functions: the save-file writer, the main-BGM switch, the design frame size, the BTF signature check and
// three JNI bridges (Cocos2dxHelper / video).
#include "core/Guest.hpp"

namespace {

namespace g {
constexpr uint32_t fileUtils_getInstance = rwlib::cocos2d::FileUtils::getInstance;   // cocos2d::FileUtils::getInstance()
constexpr uint32_t string_append = 0x47AC89;           // std::string::append(const char*, size_t)
constexpr uint32_t string_init = 0x47CD93;             // std::string::__init(const char*, size_t)
constexpr uint32_t string_reserve = 0x48D705;          // std::string::reserve(size_t)
constexpr uint32_t fopen_ = 0x414158;
constexpr uint32_t fwrite_ = 0x414164;
constexpr uint32_t rename_ = 0x4149A4;
constexpr uint32_t fclose_ = 0x414170;
constexpr uint32_t memcmp_ = 0x40EF2C;
constexpr uint32_t audioStopGameMainBGM = 0x5C0F59;    // AudioStopGameMainBGM(int fadeTime)
constexpr uint32_t audioPlayGameMainBGM = 0x5C0E91;    // AudioPlayGameMainBGM(audio::ResourceID, int fadeTime)
constexpr uint32_t director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t director_getWinSize = rwlib::cocos2d::Director::getWinSize;     // Director::getWinSize() const (const Size&)
constexpr uint32_t size_copy = rwlib::cocos2d::Size::ctor_base__RKS0_;               // Size::Size(const Size&)
constexpr uint32_t callVideoNonParameterFun = 0x8436A1;   // callVideoNonParameterFun(int, const char*)
constexpr uint32_t jni_getStaticMethodInfo = rwlib::cocos2d::JniHelper::getStaticMethodInfo; // JniHelper::getStaticMethodInfo(JniMethodInfo&, cls, name, sig)
constexpr uint32_t jni_jstring2string = rwlib::cocos2d::JniHelper::jstring2string;      // JniHelper::jstring2string(jstring) (sret std::string)
constexpr uint32_t env_callStaticObjectMethod = 0x4E0939;   // _JNIEnv::CallStaticObjectMethod(cls, method, ...)
constexpr uint32_t env_callStaticFloatMethod = 0x937E75;    // _JNIEnv::CallStaticFloatMethod(cls, method, ...)
// strings
constexpr uint32_t str_s001 = 0xDFCAD6;                // "s001.gif" (the save being written)
constexpr uint32_t str_s000 = 0xDFCACD;                // "s000.gif" (the save file)
constexpr uint32_t str_wb = 0xDFC321;                  // "wb"
constexpr uint32_t str_empty = 0xDF8A1A;               // ""
constexpr uint32_t str_helperClass = 0xE10FFB;         // "org/cocos2dx/lib/Cocos2dxHelper"
constexpr uint32_t str_getWritablePath = 0xE133E7;     // "getCocos2dxWritablePath"
constexpr uint32_t str_sigString = 0xDFC8DE;           // "()Ljava/lang/String;"
constexpr uint32_t str_getFloatForKey = 0xE134FA;      // "getFloatForKey"
constexpr uint32_t str_sigFloatForKey = 0xE13509;      // "(Ljava/lang/String;F)F"
constexpr uint32_t str_startVideo = 0xE0C1B7;          // "startVideo"
constexpr uint32_t btf_signature = 0xEBD7B0;           // "\x89BTF.." (6 bytes)
}  // namespace g

constexpr uint32_t FILEUTILS_GET_WRITABLE_PATH = 0x44;  // FileUtils::getWritablePath() const (virtual, sret)
constexpr uint32_t JNIENV_DELETE_LOCAL_REF = 0x5C;      // JNIEnv function table: DeleteLocalRef
constexpr uint32_t JNIENV_NEW_STRING_UTF = 0x29C;       // JNIEnv function table: NewStringUTF

uint32_t ld(uint32_t a) { return rw::load<uint32_t>(a); }
uint32_t vcallAt(uint32_t obj, uint32_t offset, std::initializer_list<uint32_t> args) {
    return rw::call(ld(ld(obj) + offset), args);
}
bool isLong(uint32_t s) { return rw::load<uint8_t>(s) & 1; }
uint32_t cstr(uint32_t s) { return isLong(s) ? ld(s + 8) : (s | 1); }

// dst = writable path + name (the original moves the appended temporary into dst and frees the temporary)
void writablePathPlus(uint32_t dst, uint32_t tmp, uint32_t name) {
    uint32_t files = rw::call(g::fileUtils_getInstance, {});
    vcallAt(files, FILEUTILS_GET_WRITABLE_PATH, {tmp, files});
    uint32_t r = rw::call(g::string_append, {tmp, name, 8});
    uint32_t w0 = ld(r), w1 = ld(r + 4), w2 = ld(r + 8);
    rw::save<uint32_t>(dst, w0);
    rw::save<uint32_t>(dst + 4, w1);
    rw::save<uint32_t>(dst + 8, w2);
    rw::save<uint32_t>(r, 0);
    rw::save<uint32_t>(r + 4, 0);
    rw::save<uint32_t>(r + 8, 0);
    if (isLong(tmp)) guest::deleteObject(ld(tmp + 8));
}

}  // namespace

// Write the save data to <writable>/s001.gif, then rename it over s000.gif; true when both worked.
// NOVERIFY: writes and renames a real file (fopen/fwrite/rename are not effect boundaries of the verifier).
uint32_t SaveDataToFile(const guest::SharedPtr *data) {
    guest::Frame f(0x40);
    const uint32_t tmp = f.addr(0x00), temp = f.addr(0x0C), tmp2 = f.addr(0x18), save = f.addr(0x24);
    writablePathPlus(temp, tmp, g::str_s001);
    bool tempLong = isLong(temp);
    uint32_t ok = 0;
    uint32_t file = rw::call(g::fopen_, {tempLong ? ld(temp + 8) : (temp | 1), g::str_wb});
    if (file) {
        uint32_t obj = data->obj;
        if (rw::call(g::fwrite_, {ld(obj + 4), ld(obj + 8), 1, file}) == 1) {
            writablePathPlus(save, tmp2, g::str_s000);
            uint32_t from = tempLong ? ld(temp + 8) : (temp | 1);
            if (isLong(save)) {
                uint32_t to = ld(save + 8);
                uint32_t r = rw::call(g::rename_, {from, to});
                guest::deleteObject(ld(save + 8));
                ok = r == 0 ? 1 : 0;
            } else {
                ok = rw::call(g::rename_, {from, save | 1}) == 0 ? 1 : 0;
            }
        }
        rw::call(g::fclose_, {file});
    }
    if (tempLong) guest::deleteObject(ld(temp + 8));
    return ok;
}
RW_FUNCTION_NOVERIFY(0x4EA438, "SaveDataToFile(const std::shared_ptr<bg::Data>&)", SaveDataToFile);

// Switch the main BGM unless it is already playing; r0 = the playing BGM's value when nothing changes.
uint32_t AudioReplaceGameMainBGM(int32_t id, int32_t fadeTime) {
    constexpr uint32_t CURRENT_BGM = 0x1003C30;   // { ResourceID id; int value }
    if (rw::load<int32_t>(CURRENT_BGM) == id) return ld(CURRENT_BGM + 4);
    rw::call(g::audioStopGameMainBGM, {uint32_t(fadeTime)});
    return rw::call(g::audioPlayGameMainBGM, {uint32_t(id), uint32_t(fadeTime)});
}
RW_FUNCTION(0x5C0FA0, "AudioReplaceGameMainBGM(audio::ResourceID, int)", AudioReplaceGameMainBGM);

// The design frame width for the window's aspect ratio (computed once, cached with the height):
// >= 1.775: 1136 x 640; <= 4:3: 960 x 720; < 1.5: 960 x 960/ratio; otherwise 640*ratio x 640.
float AppGetGrameFrameWidth() {
    constexpr uint32_t CACHE = 0x1005520;   // bool computed; float width; float height
    if (rw::load<uint8_t>(CACHE) == 1) return rw::load<float>(CACHE + 4);
    guest::Frame f(8);
    uint32_t winSize = rw::call(g::director_getWinSize, {rw::call(g::director_getInstance, {})});
    rw::call(g::size_copy, {f.addr(), winSize});
    double ratio = double(rw::load<float>(f.addr())) / double(rw::load<float>(f.addr(4)));
    float width;
    if (ratio >= 1.775) {
        width = 1136.0f;
        rw::save<float>(CACHE + 4, width);
        rw::save<float>(CACHE + 8, 640.0f);
    } else if (!(ratio > 1.3333333333333333 || ratio != ratio)) {
        width = 960.0f;
        rw::save<float>(CACHE + 4, width);
        rw::save<float>(CACHE + 8, 720.0f);
    } else if (!(ratio >= 1.5 || ratio != ratio)) {
        width = 960.0f;
        rw::save<float>(CACHE + 4, width);
        rw::save<float>(CACHE + 8, float(960.0 / ratio));
    } else {
        width = float(ratio * 640.0);
        rw::save<float>(CACHE + 4, width);
        rw::save<float>(CACHE + 8, 640.0f);
    }
    rw::save<uint8_t>(CACHE, 1);
    return width;
}
RW_FUNCTION(0x799B30, "AppGetGrameFrameWidth()", AppGetGrameFrameWidth);

// NOVERIFY: calls into Java (JNI) to start the video player.
uint32_t startVideoJNI(int32_t index) { return rw::call(g::callVideoNonParameterFun, {uint32_t(index), g::str_startVideo}); }
RW_FUNCTION_NOVERIFY(0x843900, "startVideoJNI(int)", startVideoJNI);

// A BTF image starts with the 6-byte signature.
uint32_t btf_is_valid(const uint8_t *data, int32_t size) {
    if (size < 4) return 0;
    return rw::call(g::memcmp_, {g::btf_signature, rw::guest_addr(data), 6}) == 0 ? 1 : 0;
}
RW_FUNCTION(0x93745C, "btf_is_valid(unsigned char*, int)", btf_is_valid);

// The app's writable directory from Java (empty when the helper method is missing).
// NOVERIFY: JNI calls into Java.
void getFileDirectoryJNI(const guest::String *out) {
    guest::Frame f(0x20);
    const uint32_t tmp = f.addr(0x00), info = f.addr(0x0C);   // std::string, JniMethodInfo {env, class, method}
    uint32_t o = rw::guest_addr(out);
    rw::call(g::string_init, {o, g::str_empty, 0});
    if (rw::call(g::jni_getStaticMethodInfo, {info, g::str_helperClass, g::str_getWritablePath, g::str_sigString}) != 1)
        return;
    uint32_t jstr = rw::call(g::env_callStaticObjectMethod, {ld(info), ld(info + 4), ld(info + 8)});
    vcallAt(ld(info), JNIENV_DELETE_LOCAL_REF, {ld(info), ld(info + 4)});
    rw::call(g::jni_jstring2string, {tmp, jstr});
    if (isLong(o)) {
        rw::save<uint8_t>(ld(o + 8), 0);
        rw::save<uint32_t>(o + 4, 0);
    } else {
        rw::save<uint8_t>(o + 1, 0);
        rw::save<uint8_t>(o, 0);
    }
    rw::call(g::string_reserve, {o, 0});
    uint32_t w0 = ld(tmp), w1 = ld(tmp + 4), w2 = ld(tmp + 8);
    rw::save<uint32_t>(o, w0);
    rw::save<uint32_t>(o + 4, w1);
    rw::save<uint32_t>(o + 8, w2);
    rw::save<uint32_t>(tmp, 0);
    rw::save<uint32_t>(tmp + 4, 0);
    rw::save<uint32_t>(tmp + 8, 0);
    vcallAt(ld(info), JNIENV_DELETE_LOCAL_REF, {ld(info), jstr});
}
RW_FUNCTION_NOVERIFY(0xA40EB8, "getFileDirectoryJNI()", getFileDirectoryJNI);

// Cocos2dxHelper.getFloatForKey(key, default); the default when the method is missing.
// NOVERIFY: JNI calls into Java.
float getFloatForKeyJNI(uint32_t key, float defaultValue) {
    guest::Frame f(0x10);
    const uint32_t info = f.addr(0x00);   // JniMethodInfo {env, class, method}
    if (rw::call(g::jni_getStaticMethodInfo, {info, g::str_helperClass, g::str_getFloatForKey, g::str_sigFloatForKey}) != 1)
        return defaultValue;
    uint32_t env = ld(info);
    double d = double(defaultValue);
    uint64_t bits;
    std::memcpy(&bits, &d, 8);
    uint32_t jkey = vcallAt(env, JNIENV_NEW_STRING_UTF, {env, key});
    // (cls, method) in r1, r2; jkey in r3; the double (varargs promotion) on the stack
    float value = rw::bitsf(rw::call(g::env_callStaticFloatMethod,
                                     {ld(info), ld(info + 4), ld(info + 8), jkey, uint32_t(bits), uint32_t(bits >> 32)}));
    vcallAt(ld(info), JNIENV_DELETE_LOCAL_REF, {ld(info), ld(info + 4)});
    vcallAt(ld(info), JNIENV_DELETE_LOCAL_REF, {ld(info), jkey});
    return value;
}
RW_FUNCTION_NOVERIFY(0xA41560, "getFloatForKeyJNI(const char*, float)", getFloatForKeyJNI);
