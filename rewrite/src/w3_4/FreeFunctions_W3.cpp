// Free functions of group w3_4: the createSceneLayer<T> factories (the layer constructors handed to
// SceneManager as std::function<cocos2d::Layer*()>), the "cancel" system sound, the debug damage popup and a few
// cocos2d JNI bridges (Cocos2dxHelper / Cocos2dxVideoHelper static methods). The stack-protector epilogue of the
// originals (compare the canary, __stack_chk_fail on mismatch) is not reproduced: the canary never changes.
#include "core/Guest.hpp"

namespace w3_4_free {

namespace {
uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }

// ---- createSceneLayer<T>: T::create()
template <uint32_t CREATE> uint32_t createSceneLayer() { return rw::call(CREATE, {}); }

// ---- audio
constexpr uint32_t AUDIO_GET_INSTANCE = 0x5BECFD;   // audio::Manager::getInstance()
constexpr uint32_t AUDIO_GET_LAYER = 0x5BF33D;      // audio::Manager::getLayer(shared_ptr<Layer>* out, mgr, int id)
constexpr uint32_t LAYER_PLAY_SE = 0x5BF785;        // audio::Layer::playSE(layer, ResourceID, int, int)
constexpr uint32_t SYSTEM_SE_LAYER = 10000;         // the audio layer of the system sounds
constexpr uint32_t SE_CANCEL = 0x1E5841A2;          // ResourceID (hash) of the cancel sound

// ---- cocos2d
constexpr uint32_t VEC2_COPY = 0x8E4C03;            // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t VEC2_DTOR = 0x8E4C17;            // cocos2d::Vec2::~Vec2()
constexpr uint32_t DISP_DMG_NUM = 0x5EB45D;         // DisplayDamageNumber::disp_dmgNum(int, Node*, Vec2, float, bool x4, int, float)
constexpr uint32_t NODE_GET_PARENT = 0x11C;         // vtable byte offsets
constexpr uint32_t NODE_GET_POSITION = 0x54;

// ---- JNI
constexpr uint32_t GET_STATIC_METHOD_INFO = 0xA4225D;    // cocos2d::JniHelper::getStaticMethodInfo(info, cls, name, sig)
constexpr uint32_t CALL_STATIC_VOID_METHOD = 0x4E0D09;   // _JNIEnv::CallStaticVoidMethod(env, cls, mid, ...)
constexpr uint32_t CALL_VIDEO_NO_PARAM = 0x8436A1;       // callVideoNonParameterFun(int index, const char* method)
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C;          // JNINativeInterface byte offsets
constexpr uint32_t JNI_NEW_STRING_UTF = 0x29C;

constexpr uint32_t STR_EMPTY = 0xDF8A1A;                 // ""
constexpr uint32_t STR_COCOS2DX_HELPER = 0xE10FFB;       // "org/cocos2dx/lib/Cocos2dxHelper"
constexpr uint32_t STR_VIDEO_HELPER = 0xE0C120;          // "org/cocos2dx/lib/Cocos2dxVideoHelper"
constexpr uint32_t STR_REMOVE_VIDEO_WIDGET = 0xE0C157;   // "removeVideoWidget"
constexpr uint32_t STR_SET_KEEP_RATIO = 0xE0C205;        // "setVideoKeepRatioEnabled"
constexpr uint32_t STR_SIG_IZ_V = 0xE0B23C;              // "(IZ)V"
constexpr uint32_t STR_SHOW_DIALOG = 0xE13385;           // "showDialog"
constexpr uint32_t STR_SIG_SS_V = 0xDFC5B0;              // "(Ljava/lang/String;Ljava/lang/String;)V"
constexpr uint32_t STR_DISABLE_ACCEL = 0xE13488;         // "disableAccelerometer"
constexpr uint32_t STR_SIG_V = 0xDFC723;                 // "()V"
constexpr uint32_t STR_SET_DOUBLE = 0xE135EC;            // "setDoubleForKey"
constexpr uint32_t STR_SIG_SD_V = 0xE135FC;              // "(Ljava/lang/String;D)V"

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

RW_FUNCTION(0x46FDBC, "createSceneLayer<SceneMyPage>()", createSceneLayer<0x4857F9>);
RW_FUNCTION(0x48A62C, "createSceneLayer<SceneMedalList>()", createSceneLayer<0x48A635>);
RW_FUNCTION(0x53E304, "createSceneLayer<ScenePartyTop>()", createSceneLayer<0x53E30D>);
RW_FUNCTION(0x579EB0, "createSceneLayer<SceneAdventureSelect>()", createSceneLayer<0x579EB9>);
RW_FUNCTION(0x57A734, "createSceneLayer<SceneRaidList>()", createSceneLayer<0x57A73D>);
RW_FUNCTION(0x57AFCC, "createSceneLayer<SceneDeckEdit>()", createSceneLayer<0x57AFD5>);
RW_FUNCTION(0x5AC5C8, "createSceneLayer<SceneSelectUnion>()", createSceneLayer<0x5AC5D1>);
RW_FUNCTION(0x5AC9F4, "createSceneLayer<SceneAlbum>()", createSceneLayer<0x5AC9FD>);
RW_FUNCTION(0x5B27C4, "createSceneLayer<SceneShopTop>()", createSceneLayer<0x5B27CD>);
RW_FUNCTION(0x601364, "createSceneLayer<SceneBattleResult>()", createSceneLayer<0x6053F1>);
RW_FUNCTION(0x60CED8, "createSceneLayer<SceneSplash>()", createSceneLayer<0x638475>);
RW_FUNCTION(0x612554, "createSceneLayer<SceneSphereBoardSelect>()", createSceneLayer<0x636E29>);
RW_FUNCTION(0x637EB8, "createSceneLayer<SceneTitle>()", createSceneLayer<0x637EC1>);
RW_FUNCTION(0x65762C, "createSceneLayer<ScenePartyManagement>()", createSceneLayer<0x657635>);
RW_FUNCTION(0x657A78, "createSceneLayer<ScenePartyRequest>()", createSceneLayer<0x657A81>);
RW_FUNCTION(0x657DE8, "createSceneLayer<ScenePartyCreate>()", createSceneLayer<0x657DF1>);
RW_FUNCTION(0x659E64, "createSceneLayer<ScenePartyMember>()", createSceneLayer<0x659E6D>);
RW_FUNCTION(0x65D920, "createSceneLayer<SceneMixedMedal>()", createSceneLayer<0x65E345>);
RW_FUNCTION(0x681260, "createSceneLayer<SceneMovie>()", createSceneLayer<0x638CE1>);
RW_FUNCTION(0x68D864, "createSceneLayer<SceneSellOutMaterialItem>()", createSceneLayer<0x6861AD>);
RW_FUNCTION(0x6C83AC, "createSceneLayer<SceneNotice>()", createSceneLayer<0x67F541>);
RW_FUNCTION(0x6FAE00, "createSceneLayer<SceneUnionRanking>()", createSceneLayer<0x6FAE09>);
RW_FUNCTION(0x70B1E8, "createSceneLayer<SceneUnionRegister>()", createSceneLayer<0x67F73D>);
RW_FUNCTION(0x711DAC, "createSceneLayer<SceneBirthRegister>()", createSceneLayer<0x711EF1>);
RW_FUNCTION(0x712418, "createSceneLayer<SceneTutorialDownload>()", createSceneLayer<0x67F5C5>);
RW_FUNCTION(0x712420, "createSceneLayer<SceneInvitation>()", createSceneLayer<0x67F619>);
RW_FUNCTION(0x715950, "createSceneLayer<SceneTakeOver>()", createSceneLayer<0x71244D>);
RW_FUNCTION(0x72CC58, "createSceneLayer<SceneRankingReward>()", createSceneLayer<0x702405>);
RW_FUNCTION(0x7355E4, "createSceneLayer<SceneLoginBonus>()", createSceneLayer<0x6F6329>);
RW_FUNCTION(0x739058, "createSceneLayer<SceneParade>()", createSceneLayer<0x739061>);
RW_FUNCTION(0x73C5B8, "createSceneLayer<SceneNameRegister>()", createSceneLayer<0x67F6AD>);

// Play the system "cancel" sound; returns the sound id from playSE.
uint32_t AudioPlaySystemSECancel() {
    guest::Frame f(0x10);
    const uint32_t layer = f.addr(4);   // std::shared_ptr<audio::Layer>
    rw::call(AUDIO_GET_LAYER, {layer, rw::call(AUDIO_GET_INSTANCE, {}), SYSTEM_SE_LAYER});
    uint32_t id = rw::call(LAYER_PLAY_SE, {ld32(layer), SE_CANCEL, 0, 0});
    if (ld32(layer + 4)) guest::release(ld32(layer + 4));
    return id;
}
RW_FUNCTION(0x5C0D80, "AudioPlaySystemSECancel()", AudioPlaySystemSECancel);

// Debug helper: pop a damage number over node (at its position, in its parent).
void display_damage(int32_t damage, uint32_t node) {
    guest::Frame f(0x30);
    const uint32_t pos = f.addr(0x20);
    uint32_t parent = rw::call(ld32(ld32(node) + NODE_GET_PARENT), {node});
    uint32_t p = rw::call(ld32(ld32(node) + NODE_GET_POSITION), {node});
    rw::call(VEC2_COPY, {pos, p});
    // disp_dmgNum(damage, parent, pos, -64.0f, false, false, false, false, 600, 2.0f)
    rw::call(DISP_DMG_NUM, {uint32_t(damage), parent, pos, rw::fbits(-64.0f), 0, 0, 0, 0, 600, rw::fbits(2.0f)});
    rw::call(VEC2_DTOR, {pos});
}
RW_FUNCTION(0x5EB550, "display_damage(int, cocos2d::Node*)", display_damage);

// Cocos2dxVideoHelper.removeVideoWidget(index)
uint32_t removeVideoWidgetJNI(int32_t index) {
    return rw::call(CALL_VIDEO_NO_PARAM, {uint32_t(index), STR_REMOVE_VIDEO_WIDGET});
}
RW_FUNCTION(0x843714, "removeVideoWidgetJNI(int)", removeVideoWidgetJNI);

// Cocos2dxVideoHelper.setVideoKeepRatioEnabled(index, enabled)
void setVideoKeepRatioEnabled(int32_t index, uint32_t enabled) {
    guest::Frame f(0x1C);
    const MethodInfo info{f.addr(0xC)};
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_VIDEO_HELPER, STR_SET_KEEP_RATIO, STR_SIG_IZ_V}) != 1) return;
    rw::call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method(), uint32_t(index), enabled});
    deleteLocalRef(info.env(), info.cls());
}
RW_FUNCTION(0x843A60, "setVideoKeepRatioEnabled(int, bool)", setVideoKeepRatioEnabled);

// Cocos2dxHelper.showDialog(title, message) (a null title becomes "")
void showDialogJNI(uint32_t message, uint32_t title) {
    if (!message) return;
    guest::Frame f(0x1C);
    const MethodInfo info{f.addr(0xC)};
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_COCOS2DX_HELPER, STR_SHOW_DIALOG, STR_SIG_SS_V}) != 1) return;
    uint32_t jtitle = newStringUTF(info.env(), title ? title : STR_EMPTY);
    uint32_t jmessage = newStringUTF(info.env(), message);
    rw::call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method(), jtitle, jmessage});
    deleteLocalRef(info.env(), jtitle);
    deleteLocalRef(info.env(), jmessage);
    deleteLocalRef(info.env(), info.cls());
}
RW_FUNCTION(0xA40B5C, "showDialogJNI(const char*, const char*)", showDialogJNI);

// Cocos2dxHelper.disableAccelerometer()
void disableAccelerometerJni() {
    guest::Frame f(0x18);
    const MethodInfo info{f.addr(8)};
    uint32_t found = rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_COCOS2DX_HELPER, STR_DISABLE_ACCEL, STR_SIG_V});
    if (found != 1) return;
    // r3 still holds what getStaticMethodInfo left there; "()V" reads no variadic argument
    rw::call(CALL_STATIC_VOID_METHOD, {info.env(), info.cls(), info.method()});
    deleteLocalRef(info.env(), info.cls());
}
RW_FUNCTION(0xA4131C, "disableAccelerometerJni()", disableAccelerometerJni);

// Cocos2dxHelper.setDoubleForKey(key, value) (UserDefault on Android)
void setDoubleForKeyJNI(uint32_t key, double value) {
    guest::Frame f(0x24);
    const MethodInfo info{f.addr(0x14)};
    if (rw::call(GET_STATIC_METHOD_INFO, {info.at, STR_COCOS2DX_HELPER, STR_SET_DOUBLE, STR_SIG_SD_V}) != 1) return;
    uint32_t jkey = newStringUTF(info.env(), key);
    uint64_t bits;
    std::memcpy(&bits, &value, 8);
    // variadic double: r3 holds jkey, so the double goes to the stack (lo, hi)
    rw::call(CALL_STATIC_VOID_METHOD,
             {info.env(), info.cls(), info.method(), jkey, uint32_t(bits), uint32_t(bits >> 32)});
    deleteLocalRef(info.env(), info.cls());
    deleteLocalRef(info.env(), jkey);
}
RW_FUNCTION(0xA41A80, "setDoubleForKeyJNI(const char*, double)", setDoubleForKeyJNI);

}  // namespace w3_4_free
