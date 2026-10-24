// Small pieces of the new-account flow: the scene the title opens for an unfinished signup (by the tutorial
// progression of the user data), bg::Data::copy (a resource buffer copied with the given allocator) and the keep
// screen on JNI call (during the tutorial download).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneAgreement_create = 0x67F7F5, SceneInvitation_create = 0x67F619;
constexpr uint32_t SceneTutorialDownload_create = 0x67F5C5, SceneNameRegister_create = 0x67F6AD;
constexpr uint32_t SceneAvatarEdit_create = 0x54BEF9, SceneUnionRegister_create = 0x67F73D;
constexpr uint32_t ResourceDataManager_getInstance = 0x7728AD, ResourceDataManager_mount = 0x773355;
constexpr uint32_t start_movie = 0x6C4A55;
constexpr uint32_t function_free_call = 0x4919ED;          // std::function<void(void*)>::operator()
constexpr uint32_t function_alloc_call = 0x4918A9;         // std::function<void*(unsigned)>::operator()
constexpr uint32_t function_free_assign = 0x491839;        // std::function<void(void*)>::operator=(const&)
constexpr uint32_t libc_memcpy = 0x40E164;
constexpr uint32_t JniHelper_getStaticMethodInfo = rwlib::cocos2d::JniHelper::getStaticMethodInfo, JNIEnv_CallStaticVoidMethod = 0x4E0D09;
constexpr uint32_t S_HelperClass = 0xE10FFB, S_SetKeepScreenOn = 0xE1349D, S_SigZV = 0xDFC75E;
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C;
// APIManager: the user (+0x44, its control block +0x48; flags +0x34 bit 0 = invitation allowed), the user data
// SafeBuffer (+0x4C, control block +0x50; word 1 = the tutorial progression)
constexpr uint32_t API_USER = 0x44, API_USER_CTRL = 0x48, API_DATA = 0x4C, API_DATA_CTRL = 0x50, USER_FLAGS = 0x34;
constexpr uint32_t SM_START_MOVIE = 0x28C;
}  // namespace

namespace signup_misc {

// the title's next scene for an account that has not finished the signup: 1 invitation (when allowed) or the
// download, 2 the download, 3 the name, 4 the avatar, 5 the Union, 6 the opening movie, else the agreement
uint32_t firstScene() {
    const uint32_t userCtrl = ld32(call(fn::APIManager_getInstance, {}) + API_USER_CTRL);
    if (userCtrl) call(fn::shared_add, {userCtrl});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t sb = ld32(api + API_DATA), dataCtrl = ld32(api + API_DATA_CTRL);
    if (dataCtrl) call(fn::shared_add, {dataCtrl});
    Frame f;
    const uint32_t value = f.alloc(4);
    call(SafeBuffer_read, {sb, value, ld32(sb + 0xC) + 4, 4, 4});
    const uint32_t progression = ld32(value);
    uint32_t scene;
    auto mounted = [] { call(ResourceDataManager_mount, {call(ResourceDataManager_getInstance, {})}); };
    switch (progression) {
    case 1: {
        const uint32_t user = ld32(call(fn::APIManager_getInstance, {}) + API_USER);
        if (user && (ld8(user + USER_FLAGS) & 1)) {
            scene = call(SceneInvitation_create, {ld8(user + USER_FLAGS)});
            break;
        }
        scene = call(SceneTutorialDownload_create, {user ? uint32_t(ld8(user + USER_FLAGS)) : 0u});
        break;
    }
    case 2:
        scene = call(SceneTutorialDownload_create, {progression - 1});
        break;
    case 3:
        mounted();
        scene = call(SceneNameRegister_create, {});
        break;
    case 4:
        mounted();
        scene = call(SceneAvatarEdit_create, {});
        break;
    case 5:
        mounted();
        scene = call(SceneUnionRegister_create, {});
        break;
    case 6:
        mounted();
        st32(sceneManager() + SM_START_MOVIE, 1);
        scene = call(start_movie, {0, 0});
        break;
    default:
        scene = call(SceneAgreement_create, {progression - 1});
        break;
    }
    if (dataCtrl) call(fn::shared_release, {dataCtrl});
    if (userCtrl) call(fn::shared_release, {userCtrl});
    return scene;
}
RW_FUNCTION(0x67F3C0, "SceneDownload: the signup scene to resume (sub_67F3C0)", signup_misc::firstScene);

// bg::Data::copy(src, n, alloc, free): the old buffer freed with the old free function, a new one from `alloc`,
// the bytes copied, `free` kept
uint32_t dataCopy(uint32_t self, uint32_t src, uint32_t n, uint32_t alloc, uint32_t freeFn) {
    if (ld32(self + 0x20)) {
        call(function_free_call, {self + 0x10, ld32(self + 4)});
        st32(self + 4, 0);
    }
    const uint32_t p = call(function_alloc_call, {alloc, n});
    st32(self + 4, p);
    st32(self + 8, n);
    call(libc_memcpy, {p, src, n});
    return call(function_free_assign, {self + 0x10, freeFn});
}
RW_FUNCTION(0x4E09DC, "bg::Data::copy", signup_misc::dataCopy);

uint32_t setKeepScreenOnJni(uint32_t on) {
    Frame f;
    const uint32_t info = f.alloc(12);
    if (call(JniHelper_getStaticMethodInfo, {info, S_HelperClass, S_SetKeepScreenOn, S_SigZV}) == 1) {
        call(JNIEnv_CallStaticVoidMethod, {ld32(info), ld32(info + 4), ld32(info + 8), on});
        const uint32_t env = ld32(info);
        call(ld32(ld32(env) + JNI_DELETE_LOCAL_REF), {env, ld32(info + 4)});
    }
    return 0;
}
RW_FUNCTION(0xA4138C, "setKeepScreenOnJni", signup_misc::setKeepScreenOnJni);

}  // namespace signup_misc
