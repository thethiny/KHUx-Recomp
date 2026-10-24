// Work order group "_between" (MyPage itself and the taps between screens), plus single leftovers of the Jewel
// and Quest screens: small helpers, lambda bodies and std::function clones that run around MyPage.
#include "../scene/SceneCommon.hpp"
#include "../system/SystemCommon.hpp"

using namespace scene;

namespace {
namespace a {
constexpr uint32_t STRLEN = 0x40E08C;                     // strlen (import)
constexpr uint32_t MEMCPY = 0x40E164;                     // memcpy (import)
constexpr uint32_t MALLOC = 0x40FF58;                     // malloc (import)
constexpr uint32_t String_initReserve = 0x48461F;         // sub_48461E: std::string::__init(const char*, sz, reserve)
constexpr uint32_t VectorOfCharVectors_dtor = 0x49489D;   // std::__vector_base<std::vector<char>>::~__vector_base
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_openAvatarInfoPopup = 0x46A229;  // (u64 id, bool, function<void(bool)>, function<bool()>)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_isOpenedPopup = 0x621519;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;  // (bool)
constexpr uint32_t SceneManager_closeGlobalMenu = 0x611BD5;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t KeyCodeMap_find = 0xA428A5;            // unordered_map<int, EventKeyboard::KeyCode>::find(const int&)
constexpr uint32_t EventKeyboard_ctor = rwlib::cocos2d::EventKeyboard::ctor_base;         // (KeyCode, bool pressed)
constexpr uint32_t EventDispatcher_dispatchEvent = rwlib::cocos2d::EventDispatcher::dispatchEvent;
constexpr uint32_t Event_dtor = rwlib::cocos2d::Event::dtor_base;
constexpr uint32_t Bitmap_reset = rwlib::sub_A3ED7C;               // sub_A3ED7C: frees the previous bitmap buffer
constexpr uint32_t DCMPEQ = 0xCD69D8;                     // __aeabi_dcmpeq
constexpr uint32_t MULDF3 = 0xCD64A0;                     // __muldf3
constexpr uint32_t VSNPRINTF = rwlib::sub_CA3B24;                  // sub_CA3B24: vsnprintf(buf, size, fmt, va_list)
}  // namespace a

// Android key code -> cocos2d key code (cocos2d's g_keyCodeMap)
constexpr uint32_t KEY_CODE_MAP = 0x100895C;
// Cocos2dxBitmap: the bitmap being drawn into (width, height, pixels)
constexpr uint32_t BITMAP_WIDTH = 0x1008918, BITMAP_HEIGHT = 0x100891C, BITMAP_PIXELS = 0x1008920;
// JNIEnv slot: SetByteArrayRegion(env, array, start, len, buf)
constexpr uint32_t JNI_SetByteArrayRegion = 0x320;

// ------------------------------------------------------------------------------------------------ cocos2d JNI
// A key press from Java: dispatched as an EventKeyboard when cocos2d knows the key. Returns whether it did.
uint32_t nativeKeyDown(uint32_t env, uint32_t thiz, uint32_t keyCode) {
    (void)env; (void)thiz;
    Frame f;
    const uint32_t key = f.alloc(4);
    st32(key, keyCode);
    call(a::Director_getInstance, {});
    const uint32_t node = call(a::KeyCodeMap_find, {KEY_CODE_MAP, key});
    if (!node) return 0;
    const uint32_t event = f.alloc(0x34);
    call(a::EventKeyboard_ctor, {event, ld32(node + 0xC), 0});
    const uint32_t director = call(a::Director_getInstance, {});
    call(a::EventDispatcher_dispatchEvent, {ld32(director + 0x84), event});
    call(a::Event_dtor, {event});
    return 1;
}

// Cocos2dxBitmap.nativeInitBitmapDC(width, height, pixels): keeps the size, copies the Java pixel array into a
// fresh malloc'd buffer.
uint32_t nativeInitBitmapDC(uint32_t env, uint32_t thiz, uint32_t width, uint32_t height, uint32_t pixels) {
    (void)thiz;
    call(a::Bitmap_reset, {});
    st32(BITMAP_WIDTH, width);
    st32(BITMAP_HEIGHT, height);
    const uint32_t bytes = height * width * 4;
    const uint32_t buf = call(a::MALLOC, {bytes});
    st32(BITMAP_PIXELS, buf);
    return vcall(env, JNI_SetByteArrayRegion, {env, pixels, 0, bytes, buf});
}

// ------------------------------------------------------------------------------------------------ libc
// frexp(x, &exp): x = m * 2^exp with 0.5 <= |m| < 1; subnormals are scaled by 2^64 first.
uint64_t frexpDouble(uint32_t lo, uint32_t hi, uint32_t expOut) {
    const uint32_t e = (hi >> 20) & 0x7FF;
    if (e == 0x7FF) return uint64_t(hi) << 32 | lo;              // inf / nan: unchanged, exp untouched
    if (e != 0) {
        st32(expOut, e - 1022);
        return uint64_t((hi & 0x800FFFFF) | 0x3FE00000) << 32 | lo;
    }
    if (call(a::DCMPEQ, {lo, hi, 0, 0})) {                       // +-0
        st32(expOut, 0);
        return uint64_t(hi) << 32 | lo;
    }
    const uint64_t scaled = rw::call64(a::MULDF3, {lo, hi, 0, 0x43F00000});   // * 2^64
    const uint64_t r = frexpDouble(uint32_t(scaled), uint32_t(scaled >> 32), expOut);
    st32(expOut, ld32(expOut) - 64);
    return r;
}

// snprintf(buf, size, fmt, ...) over sub_CA3B24
uint32_t snprintfGuest(uint32_t buf, uint32_t size, uint32_t fmt) {
    sys::VaSpill va(3);
    return call(a::VSNPRINTF, {buf, size, fmt, va.va()});
}

// ------------------------------------------------------------------------------------------------ libc++
// operator+(const char*, const std::string&) into out
uint32_t stringConcatCStr(uint32_t out, uint32_t lhs, uint32_t rhs) {
    st32(out, 0);
    st32(out + 4, 0);
    st32(out + 8, 0);
    const uint32_t lhsLen = call(a::STRLEN, {lhs});
    const uint32_t rhsLen = strSize(rhs);
    call(a::String_initReserve, {out, lhs, lhsLen, rhsLen + lhsLen});
    return call(fn::string_append, {out, strData(rhs), rhsLen});
}

// std::__vector_base<TextFormat element (0x440 bytes)>::~__vector_base: each element holds two plain vectors and a
// vector<vector<char>>, destroyed from the back.
uint32_t textFormatVectorDtor(uint32_t self) {
    uint32_t begin = ld32(self);
    if (!begin) return self;
    uint32_t end = ld32(self + 4);
    if (end != begin) {
        do {
            const uint32_t e = end - 0x440;
            st32(self + 4, e);
            call(a::VectorOfCharVectors_dtor, {e + 0x28});
            for (uint32_t v : {e + 0x1Cu, e + 0x10u}) {
                const uint32_t data = ld32(v);
                if (!data) continue;
                if (ld32(v + 4) != data) st32(v + 4, data);
                call(fn::operator_delete, {data});
            }
            end = ld32(self + 4);
        } while (end != begin);
        begin = ld32(self);
    }
    call(fn::operator_delete, {begin});
    return self;
}

// ------------------------------------------------------------------------------------------------ std::function clones
// __func<SceneAvatarEdit lambda (80 bytes of captures)>::__clone(): a heap copy
uint32_t cloneAvatarEditLambda(uint32_t vtable, uint32_t self) {
    const uint32_t copy = call(fn::operator_new, {0x54});
    if (copy) {
        st32(copy, vtable);
        call(a::MEMCPY, {copy + 4, self + 4, 0x50});
    }
    return copy;
}
uint32_t cloneAvatarEditLambdaA(uint32_t self) { return cloneAvatarEditLambda(0xF587D8, self); }
uint32_t cloneAvatarEditLambdaB(uint32_t self) { return cloneAvatarEditLambda(0xF58830, self); }

// ------------------------------------------------------------------------------------------------ lambda bodies
// SceneAlbum: the avatar button's touch callback - on release, close the album popup and open the avatar info
// popup (no avatar id, no close callback, a "can open" check that is a lambda of its own).
void albumAvatarButtonTouched(uint32_t self, uint32_t senderRef, uint32_t typeRef) {
    (void)self; (void)senderRef;
    if (ld32(typeRef) != 2) return;                               // Widget::TouchEventType::ENDED
    const uint32_t r = call(a::AudioPlaySystemSEDecide, {});
    const uint32_t sm = call(a::SceneManager_getInstance, {r});
    call(a::SceneManager_closeCurrentPopup, {sm, 0});
    Frame f;
    const uint32_t canOpen = newFunction(f, 0xF5C2CC, {});       // std::function<bool()>
    const uint32_t onClose = newEmptyFunction(f);                 // std::function<void(bool)>
    rw::call(a::SceneUtil_openAvatarInfoPopup, {0, 0, 1, onClose, canOpen});
    functionDestroy(canOpen);
    functionDestroy(onClose);
}

// SceneManager: the Android back key - ignored while a request runs, during the tutorial or a scene lock;
// otherwise it closes the top popup (when it may be closed), else the global menu, else runs the scene's own
// back handler.
void backKeyPressed(uint32_t self) {
    const uint32_t sm = ld32(self + 4);
    if (ld8(call(a::APIManager_getInstance, {}) + 4)) return;    // a request is running
    if (ld32(sm + 0x4B8)) return;
    if (int32_t(call(a::SceneUtil_getNexTutorialphase, {})) < 995) return;
    const uint32_t noPopup = call(a::SceneManager_isOpenedPopup, {sm}) ^ 1;
    const uint32_t popupsBegin = ld32(sm + 0x58C), popupsEnd = ld32(sm + 0x590);
    if (popupsEnd == popupsBegin && noPopup) {
        if (ld32(sm + 0x5C0)) call(fn::function_vv_call, {sm + 0x5B0});   // the scene's back handler
        return;
    }
    if (popupsEnd != popupsBegin) {
        // vector<bool> of "closable" flags, one per popup: the last one
        const uint32_t last = ld32(sm + 0x5A8) - 1;
        const uint32_t word = ld32(ld32(sm + 0x5A4) + ((last >> 3) & 0x1FFFFFFC));
        if ((1u << (last & 31)) & word) call(a::SceneManager_closeCurrentPopup, {sm, 0});
        return;
    }
    if (ld8(sm + 0x368)) call(a::SceneManager_closeGlobalMenu, {sm});
}

// SceneSelectUnion: removes two nodes from the layer at +0x278 (removeChild(child, cleanup)): the child at +0x27C,
// then the scene object itself
void selectUnionRemoveChildren(uint32_t self) {
    const uint32_t scene = ld32(self + 4);
    const uint32_t layer = ld32(scene + 0x278);
    vcall(layer, 0x12C, {layer, ld32(scene + 0x27C), 1});
    const uint32_t layer2 = ld32(scene + 0x278);
    vcall(layer2, 0x12C, {layer2, scene, 1});
}
}  // namespace

RW_FUNCTION(0xA42824, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown", nativeKeyDown);
RW_FUNCTION(0xA3F174, "Java_org_cocos2dx_lib_Cocos2dxBitmap_nativeInitBitmapDC", nativeInitBitmapDC);
RW_FUNCTION(0xCA4984, "frexp (sub_CA4984)", frexpDouble);
RW_FUNCTION(0xCA4A78, "snprintf (sub_CA4A78)", snprintfGuest);
RW_FUNCTION(0x461FCC, "std::operator+(const char*, const std::string&) (sub_461FCC)", stringConcatCStr);
RW_FUNCTION(0x493E04, "TextFormat element vector ~__vector_base (sub_493E04)", textFormatVectorDtor);
RW_FUNCTION(0x54C000, "SceneAvatarEdit lambda __func::__clone() (sub_54C000)", cloneAvatarEditLambdaA);
RW_FUNCTION(0x54C3F0, "SceneAvatarEdit lambda __func::__clone() (sub_54C3F0)", cloneAvatarEditLambdaB);
RW_FUNCTION(0x5ACADC, "SceneAlbum avatar button touch lambda (sub_5ACADC)", albumAvatarButtonTouched);
RW_FUNCTION(0x637FB8, "SceneManager back key lambda (sub_637FB8)", backKeyPressed);
RW_FUNCTION(0x5A80E6, "SceneSelectUnion lambda: remove two children (sub_5A80E6)", selectUnionRemoveChildren);

// ------------------------------------------------------------------------------------------------ Quests
namespace {
namespace adv {
constexpr uint32_t BaseStageScrollView_isScroll = 0x59DC19;
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D;   // (bool)
constexpr uint32_t changeOutAnimation = 0x71F00D;              // SceneAdventureSelect::changeOutAnimation(function<void()>)
// SceneAdventureSelect fields
constexpr uint32_t STAGE_VIEW = 0x300;       // BaseStageScrollView*
constexpr uint32_t MODE = 0x28C;             // which list the scene shows (0..6)
constexpr uint32_t SUB_MENU = 0x370;         // node hidden on leaving
constexpr uint32_t FLAG_325 = 0x325, FLAG_348 = 0x348;
constexpr uint32_t CAN_BACK = 0x3FC;         // set while the scene accepts the back button
// the out-animation's completion lambdas (__func vtables)
constexpr uint32_t VT_BACK_TO_MYPAGE = 0xF6BE54, VT_BACK_63 = 0xF6BE80, VT_BACK_1C = 0xF6BEAC;
}  // namespace adv

// The back button: closes an open popup first; otherwise (unless the stage list is still scrolling) hides the
// sub menu, clears the scene manager's flag at +0x308 and plays the out animation, whose completion goes back by
// mode: modes 0, 1, 5, 6 and 2..4 (both with the header locked and the scene captured), anything else to MyPage.
void adventureSelectBackScene(uint32_t self) {
    if (!ld8(self + adv::CAN_BACK)) return;
    const uint32_t view = ld32(self + adv::STAGE_VIEW);
    if (view && ld8(view + 0x5A0)) return;
    if (call(a::SceneManager_isOpenedPopup, {call(a::SceneManager_getInstance, {})}) == 1) {
        call(a::SceneManager_closeCurrentPopup, {call(a::SceneManager_getInstance, {}), 0});
        return;
    }
    const uint32_t list = ld32(self + adv::STAGE_VIEW);
    if (list && call(adv::BaseStageScrollView_isScroll, {list})) return;
    const uint32_t menu = ld32(self + adv::SUB_MENU);
    if (menu) vcall(menu, node::setVisible, {menu, 0});
    st8(call(a::SceneManager_getInstance, {}) + 0x308, 0);
    const uint32_t mode = ld32(self + adv::MODE);
    const uint32_t bit = mode <= 6 ? 1u << mode : 0;
    Frame f;
    uint32_t done;
    if (bit & 0x63) {
        call(adv::SceneManager_setCanTouchHeader, {call(a::SceneManager_getInstance, {}), 0});
        st8(self + adv::CAN_BACK, 0);
        st8(self + adv::FLAG_325, 0);
        done = newFunction(f, adv::VT_BACK_63, {self});
    } else if (bit & 0x1C) {
        call(adv::SceneManager_setCanTouchHeader, {call(a::SceneManager_getInstance, {}), 0});
        st8(self + adv::FLAG_325, 0);
        st8(self + adv::CAN_BACK, 0);
        st8(self + adv::FLAG_348, 0);
        done = newFunction(f, adv::VT_BACK_1C, {self});
    } else {
        st8(self + adv::CAN_BACK, 0);
        done = newFunction(f, adv::VT_BACK_TO_MYPAGE, {});
    }
    call(adv::changeOutAnimation, {self, done});
    functionDestroy(done);
}
}  // namespace

RW_FUNCTION(0x72834C, "SceneAdventureSelect::backScene", adventureSelectBackScene);
