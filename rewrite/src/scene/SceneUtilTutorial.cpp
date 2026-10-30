// SceneUtil: the tutorial - popups (picture popups and masks pointing at a widget), the tutorial phase and the
// "seen" flags of the tutorial popups (a 64-bit mask kept by the server, updated through APIManager).
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t APIManager_tutorial = 0x64;            // TutorialProgress*: phase, ..., popup flags (u64)
constexpr uint32_t APIManager_userData = 0x94;            // shared_ptr<UserData> (SafeBuffer; Lux rank at 0x40)
constexpr uint32_t Tutorial_phase = 0x00, Tutorial_flags = 0x08, Tutorial_skipped = 0x10;
constexpr uint32_t USERDATA_LUX_RANK = 0x40;
constexpr int32_t PHASE_DONE = 995;                       // tutorial finished: only popups remain

constexpr uint32_t TABLE_PhaseOrder = 0xDEC5F0;           // int[14]: phase, next phase, ...
constexpr uint32_t TABLE_BiIdToTutorialId = 0xDEC628;
constexpr uint32_t TABLE_TutorialIdToBiId = 0xDEC66C;     // int[42]: popup flag bit (+1) per tutorial id
constexpr uint32_t TABLE_MaskJson = 0xF5387C;             // const char*[]: mask layout per (id - 100)
constexpr uint32_t TABLE_MaskAction = 0xF538D0;           // const char*[]: its cocostudio action file
constexpr uint32_t TABLE_MaskText = 0xDEC3E0;             // int[]: text id per (id - 100)
constexpr uint32_t TABLE_MaskPlacement = 0xDEC434;        // int[]: 0 bottom left, 1 bottom right, 2 centre
constexpr uint32_t TABLE_PopupText = 0xDEC488;            // int[]: text id per picture
constexpr uint32_t TABLE_PopupGotoText = 0xDEC53C;        // int[]: "go to" button text id per tutorial id (0: none)

constexpr uint32_t AppState_isClearTutorialPopupIDs = 0x4EB815;
constexpr uint32_t AppState_resetClearTutorialPopupIDs = 0x4EB8B5;
constexpr uint32_t TutorialMisc_create = 0x777BB5;
constexpr uint32_t TutorialMisc_getTutorialIds = 0x777B99;   // std::set<int> (sret)
constexpr uint32_t IntTree_destroy = 0x48D9B5;            // std::__tree<int>::destroy(node)
constexpr uint32_t APIManager_executeTutorialStatusUpdate = 0x4C5DCD;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t FunctionB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t ActionManagerEx_getInstance = rwlib::cocostudio::ActionManagerEx::getInstance;
constexpr uint32_t ActionManagerEx_playActionByName = rwlib::cocostudio::ActionManagerEx::playActionByName;
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;
constexpr uint32_t Layout_dtor = rwlib::cocos2d::ui::Layout::dtor_base;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Node_removeFromParentAndCleanup = rwlib::cocos2d::Node::removeFromParentAndCleanup;
constexpr uint32_t FadeIn_create = rwlib::cocos2d::FadeIn::create;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create;
constexpr uint32_t EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t TouchFunction_swap = 0x483E4D;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t Memset = 0x40E7A0;                     // __aeabi_memset(dst, n, value)

constexpr uint32_t SceneManager_loadingBlocked = 0x4CC;   // SceneManager byte
constexpr uint32_t SceneManager_onFinishLoading = 0x548;  // SceneManager std::function<void(bool)>

constexpr uint32_t VEC2_ZERO = 0x1006950, VEC2_ANCHOR_BOTTOM_LEFT = 0x1006958, VEC2_ANCHOR_BOTTOM_RIGHT = 0xFEF5D0;

constexpr uint32_t VT_StatusUpdateDone = 0xF54400;        // updateTutorialStatus $_74: [loading, callback]
constexpr uint32_t VT_Mask_TouchBegan = 0xF5463C;         // TutorialMask $_67: [target widgets x3]
constexpr uint32_t VT_Mask_Close = 0xF545E4;              // TutorialMask $_68 (returned)
constexpr uint32_t VT_Popup_OkTouch = 0xF54560;           // NormalTutorialPopup $_69
constexpr uint32_t VT_Popup_CloseTouch = 0xF544DC;        // $_70
constexpr uint32_t VT_Popup_CanClose = 0xF544B0;          // $_71 (bool())
constexpr uint32_t VT_Popup_OnClose = 0xF54484;           // $_72 (void(bool))
constexpr uint32_t VT_Popup_Close = 0xF54458;             // $_73 (returned): shows the web view again
constexpr uint32_t VT_TutorialMaskWidget = 0xF7CF94, VT_TutorialMaskWidget_2 = 0xF7D2D8,
                   VT_TutorialMaskWidget_3 = 0xF7D2EC;    // primary vtable and the two secondary ones

constexpr uint32_t STR_Animation0 = 0xDF9996, STR_Mask = 0xDF99A1, STR_Txt_Tutorial = 0xDF99A6;
constexpr uint32_t STR_Tutorial01Json = 0xDF99B3;         // "cocostudio/publish/Tutorial_01.json"
constexpr uint32_t STR_Close_Button = 0xDF8D2F, STR_Tutorial = 0xDF99D7;
constexpr uint32_t STR_TutorialPng = 0xDF99E0;            // "img/tutorial/tutorial_%d.png"
constexpr uint32_t STR_Txt = 0xDF94D1, STR_webView = 0xDF99FD, STR_Button_Goto = 0xDF9A05;
constexpr uint32_t STR_Txt_OK = 0xDF869F, STR_Button_OK = 0xDF8688;

uint32_t tutorial() { return ld32(call(fn::APIManager_getInstance, {}) + APIManager_tutorial); }

// ARM register shift: amounts of 32 and more give 0 (only the low byte of the amount counts)
uint32_t shl(uint32_t v, uint32_t n) { n &= 0xFF; return n >= 32 ? 0 : v << n; }

void callFunction(uint32_t f) {
    if (ld32(f + 16)) call(fn::function_vv_call, {f});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ phases

int32_t SceneUtil::getNexTutorialphase() {
    uint32_t t = tutorial();
    if (t && ld8(t + Tutorial_skipped)) return PHASE_DONE;
    t = tutorial();
    int32_t phase = 0;
    if (t) {
        phase = int32_t(ld32(t + Tutorial_phase));
        if (phase == 50) return int32_t(ld32(TABLE_PhaseOrder + 4));
    }
    // the phase after `phase` in the phase order (entries 1..12); finished when it is not there
    for (uint32_t k = 1; ((k - 1) & ~1u) != 0xC; k++) {
        if (int32_t(ld32(TABLE_PhaseOrder + 4 * k)) == phase) return int32_t(ld32(TABLE_PhaseOrder + 4 * (k + 1)));
    }
    return PHASE_DONE;
}
RW_FUNCTION(0x472B30, "SceneUtil::getNexTutorialphase", SceneUtil::getNexTutorialphase);

int32_t SceneUtil::getNowTutorialphase() {
    uint32_t t = tutorial();
    return t ? int32_t(ld32(t + Tutorial_phase)) : 0;
}
RW_FUNCTION(0x472D24, "SceneUtil::getNowTutorialphase", SceneUtil::getNowTutorialphase);

int32_t SceneUtil::BiId2TutorialId(int32_t biId) { return int32_t(ld32(TABLE_BiIdToTutorialId + 4 * uint32_t(biId))); }
RW_FUNCTION(0x472D38, "SceneUtil::BiId2TutorialId", SceneUtil::BiId2TutorialId);

int32_t SceneUtil::TutorialId2BiId(uint32_t id) { return id <= 0x29 ? int32_t(ld32(TABLE_TutorialIdToBiId + 4 * id)) : 17; }
RW_FUNCTION(0x472D50, "SceneUtil::TutorialId2BiId", SceneUtil::TutorialId2BiId);

// Ask the server to set the tutorial phase / popup flags. When nothing changes, just run the callback.
// With `blockLoading` the scene manager is told not to show its loading indicator.
void SceneUtil::updateTutorialStatus(int32_t phase, uint64_t flags, uint32_t skipped, Addr callback,
                                     uint32_t blockLoading) {
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    uint32_t t = tutorial();
    if (t) {
        uint64_t current = uint64_t(ld32(t + Tutorial_flags)) | uint64_t(ld32(t + Tutorial_flags + 4)) << 32;
        uint32_t sameFlags = current == flags ? 1 : 0;
        uint32_t sameSkip = ((ld8(t + Tutorial_skipped) != 0 ? 1u : 0u) ^ skipped) ^ 1u;
        if (int32_t(ld32(t + Tutorial_phase)) == phase && (sameFlags & sameSkip) == 1) {
            callFunction(callback);
            functionDestroy(cb);
            return;
        }
    }
    if (blockLoading == 1) {
        uint32_t mgr = sceneManager();
        uint32_t empty = newEmptyFunction(f);
        st8(mgr + SceneManager_loadingBlocked, 1);
        call(FunctionB_assign, {mgr + SceneManager_onFinishLoading, empty});
        functionDestroy(empty);
    } else {
        call(SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    }
    uint32_t api = call(fn::APIManager_getInstance, {});
    // the completion lambda [blockLoading, cb] (a copy of cb), then copied again into the heap __func
    uint32_t captured = f.alloc(0x20);       // blockLoading at +0, the std::function at +8 (0x1C bytes)
    functionCopy(captured + 8, cb);
    uint32_t done = f.alloc(20);
    st32(done + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, VT_StatusUpdateDone);
        st8(heap + 8, uint8_t(blockLoading));
        functionCopy(heap + 0x10, captured + 8);
        st8(heap + 8, uint8_t(blockLoading));
    }
    st32(done + 16, heap);
    call(APIManager_executeTutorialStatusUpdate,
         {api, uint32_t(phase), uint32_t(flags), uint32_t(flags >> 32), skipped, done});
    functionDestroy(done);
    functionDestroy(captured + 8);
    functionDestroy(cb);
}
RW_FUNCTION(0x4727CC, "SceneUtil::updateTutorialStatus", SceneUtil::updateTutorialStatus);

void SceneUtil::updateTutorialPhase(int32_t phase, Addr callback, uint32_t skip, uint32_t blockLoading) {
    uint32_t t = tutorial();
    uint64_t flags = uint64_t(ld32(t + Tutorial_flags)) | uint64_t(ld32(t + Tutorial_flags + 4)) << 32;
    uint32_t skipped = 1;
    if (!skip) skipped = ld8(t + Tutorial_skipped) != 0 ? 1 : 0;
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    updateTutorialStatus(phase, flags, skipped, cb, blockLoading);
    functionDestroy(cb);
}
RW_FUNCTION(0x472BA0, "SceneUtil::updateTutorialPhase", SceneUtil::updateTutorialPhase);

void SceneUtil::CompleteTutorialPhase(Addr callback, uint32_t skip, uint32_t blockLoading) {
    int32_t next = getNexTutorialphase();
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    updateTutorialPhase(next, cb, skip, blockLoading);
    functionDestroy(cb);
    call(AppState_resetClearTutorialPopupIDs, {call(fn::AppState_getInstance, {})});
}
RW_FUNCTION(0x472A74, "SceneUtil::CompleteTutorialPhase", SceneUtil::CompleteTutorialPhase);

void SceneUtil::updateTutorialpopupFlag(uint64_t flags, Addr callback, uint32_t blockLoading) {
    uint32_t t = tutorial();
    int32_t phase = int32_t(ld32(t + Tutorial_phase));
    uint32_t skipped = ld8(t + Tutorial_skipped);
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    updateTutorialStatus(phase, flags, skipped, cb, blockLoading);
    functionDestroy(cb);
}
RW_FUNCTION(0x472C68, "SceneUtil::updateTutorialpopupFlag", SceneUtil::updateTutorialpopupFlag);

// Mark the popup of tutorial id `id` as seen (its flag bit), then run the callback.
void SceneUtil::ClearTutorialPopupIDs(uint32_t id, Addr callback) {
    int32_t bit = id <= 0x29 ? int32_t(ld32(TABLE_TutorialIdToBiId + 4 * id)) : 17;
    if (id > 0x29 || bit == 17) {
        callFunction(callback);
        return;
    }
    uint32_t t = tutorial();
    if (!t) {
        callFunction(callback);
        return;
    }
    uint32_t lo = ld32(t + Tutorial_flags), hi = ld32(t + Tutorial_flags + 4);
    uint32_t setLo = 1, setHi = 0;
    if (id != 32) {
        setLo = shl(1, uint32_t(bit - 1));
        setHi = uint32_t(int32_t(setLo) >> 31);
    }
    Frame f;
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    updateTutorialpopupFlag(uint64_t(setLo | lo) | uint64_t(setHi | hi) << 32, cb, 1);
    functionDestroy(cb);
}
RW_FUNCTION(0x4732AC, "SceneUtil::ClearTutorialPopupIDs", SceneUtil::ClearTutorialPopupIDs);

// A converted (old) account: set every popup flag the user's Lux rank already passed, and finish the tutorial.
void SceneUtil::freeTutorialConvertData(Addr callback) {
    uint32_t t = tutorial();
    if (t && ld32(t + Tutorial_phase)) {
        callFunction(callback);
        return;
    }
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t userData = api + APIManager_userData;   // shared_ptr; the SafeBuffer is re-read each time
    Frame f;
    uint32_t ids = f.alloc(12);                      // std::set<int>: begin node, root (end node's left), size
    call(TutorialMisc_getTutorialIds, {ids});
    uint32_t end = ids + 4;                          // the end node is the second word
    uint32_t lo = 0, hi = 0;
    for (uint32_t node = ld32(ids); node != end;) {
        Frame g;
        uint32_t row = masterRow(g, TutorialMisc_create, ld32(node + 16));
        uint32_t data = ld32(row);
        bool mark = false;
        if (data && safe32(data, 0) <= 63) {
            int32_t needRank = safe32(data, 0x14);
            int32_t needMisc = needRank ? 1 : safe32(data, 0x18);
            if (needMisc) {
                if (safe32(data, 0x14)) {
                    mark = safe32(ld32(userData), USERDATA_LUX_RANK) >= safe32(data, 0x14);
                } else if (!safe32(data, 0x18)) {
                    mark = true;
                } else {
                    int32_t rank = safe32(ld32(userData), USERDATA_LUX_RANK);
                    uint32_t misc = masterRow(g, fn::Misc_create, uint32_t(safe32(data, 0x18)));
                    int32_t need = safe32(ld32(misc), 4);
                    sharedRelease(misc);
                    mark = rank >= need;
                }
            }
        }
        if (mark) {
            uint32_t b = shl(1, uint32_t(uint8_t(safe32(data, 0)) - 1));
            lo |= b;
            hi |= uint32_t(int32_t(b) >> 31);
        }
        sharedRelease(row);
        // in-order successor
        uint32_t right = ld32(node + 4);
        if (right) {
            node = right;
            while (ld32(node)) node = ld32(node);
        } else {
            uint32_t child;
            do {
                child = node;
                node = ld32(child + 8);
            } while (ld32(node) != child);
        }
    }
    call(IntTree_destroy, {ids, ld32(ids + 4)});
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    updateTutorialStatus(PHASE_DONE, uint64_t(lo) | uint64_t(hi) << 32, 1, cb, 1);
    functionDestroy(cb);
}
RW_FUNCTION(0x47250C, "SceneUtil::freeTutorialConvertData", SceneUtil::freeTutorialConvertData);

// ------------------------------------------------------------------------------------------------ checks
// Which tutorial ids may show their popup during a given phase.

int32_t SceneUtil::tutorial_check_sub_100(int32_t id) {
    if (id > 100) return uint32_t(id - 101) < 3;
    return uint32_t(id - 1) < 5 || id == 8 || id == 43;
}
RW_FUNCTION(0x473108, "SceneUtil::tutorial_check_sub_100", SceneUtil::tutorial_check_sub_100);

int32_t SceneUtil::tutorial_check_sub_200(int32_t id) {
    if (id > 99) {
        uint32_t k = uint32_t(id - 100);
        return k <= 8 && (shl(1, k) & 0x1F1);
    }
    uint32_t k = uint32_t(id - 6);
    return k <= 0x1D && (shl(1, k) & 0x20000043);
}
RW_FUNCTION(0x47312C, "SceneUtil::tutorial_check_sub_200", SceneUtil::tutorial_check_sub_200);

int32_t SceneUtil::tutorial_check_sub_210(int32_t id) { return id == 12 || id == 36 || id == 109; }
RW_FUNCTION(0x473160, "SceneUtil::tutorial_check_sub_210", SceneUtil::tutorial_check_sub_210);

int32_t SceneUtil::tutorial_check_sub_220(int32_t id) {
    return uint32_t(id - 36) < 2 || uint32_t(id - 110) < 2 || id == 14;
}
RW_FUNCTION(0x473176, "SceneUtil::tutorial_check_sub_220", SceneUtil::tutorial_check_sub_220);

int32_t SceneUtil::tutorial_check_sub_300(int32_t id) {
    if (id <= 36) return uint32_t(id - 9) < 2;
    if (id <= 43) return id == 37;
    return uint32_t(id - 105) < 2 || id == 44 || id == 112;
}
RW_FUNCTION(0x473198, "SceneUtil::tutorial_check_sub_300", SceneUtil::tutorial_check_sub_300);

int32_t SceneUtil::tutorial_check_sub_400(int32_t id) {
    if (id <= 36) return id == 11;
    if (id <= 43) return id == 37;
    return uint32_t(id - 105) < 2 || id == 44 || id == 112;
}
RW_FUNCTION(0x4731CA, "SceneUtil::tutorial_check_sub_400", SceneUtil::tutorial_check_sub_400);

int32_t SceneUtil::tutorial_check_sub_410(int32_t id) { return uint32_t(id - 113) < 2 || id == 18; }
RW_FUNCTION(0x4731FA, "SceneUtil::tutorial_check_sub_410", SceneUtil::tutorial_check_sub_410);

int32_t SceneUtil::tutorial_check_sub_500(int32_t id) {
    if (id <= 104) return id == 42 || id == 44;
    uint32_t k = uint32_t(id - 105);
    return k <= 7 && (shl(1, k) & 0x83);
}
RW_FUNCTION(0x473212, "SceneUtil::tutorial_check_sub_500", SceneUtil::tutorial_check_sub_500);

int32_t SceneUtil::tutorial_check_sub_600(int32_t id) {
    if (id <= 104) {
        uint32_t k = uint32_t(id - 31);
        return k <= 0xD && (shl(1, k) & 0x2041);
    }
    uint32_t k = uint32_t(id - 105);
    return k <= 7 && (shl(1, k) & 0x83);
}
RW_FUNCTION(0x47323C, "SceneUtil::tutorial_check_sub_600", SceneUtil::tutorial_check_sub_600);

int32_t SceneUtil::tutorial_check_sub_700(int32_t id) { return uint32_t(id - 115) < 4 || id == 15 || id == 28; }
RW_FUNCTION(0x47326C, "SceneUtil::tutorial_check_sub_700", SceneUtil::tutorial_check_sub_700);

int32_t SceneUtil::tutorial_check_sub_710(int32_t id) {
    return uint32_t(id - 38) < 2 || uint32_t(id - 119) < 2 || id == 104;
}
RW_FUNCTION(0x473288, "SceneUtil::tutorial_check_sub_710", SceneUtil::tutorial_check_sub_710);

// May the popup of tutorial id `id` show now? During the tutorial only the phase's own popups; afterwards
// the ones not seen yet whose Lux rank requirement is met.
int32_t SceneUtil::dispTutorialCheck(int32_t id) {
    if (id == 26) return 1;
    uint32_t api = call(fn::APIManager_getInstance, {});
    if (!ld32(api + APIManager_tutorial)) return 0;
    int32_t next = getNexTutorialphase();
    if (next == PHASE_DONE) {
        int32_t bit = uint32_t(id) <= 0x29 ? int32_t(ld32(TABLE_TutorialIdToBiId + 4 * uint32_t(id))) : 17;
        Frame f;
        uint32_t row = masterRow(f, TutorialMisc_create, uint32_t(bit));
        int32_t show = 0;
        if (ld32(row)) {
            uint32_t data = ld32(row);
            int32_t rank = safe32(ld32(call(fn::APIManager_getInstance, {}) + APIManager_userData), USERDATA_LUX_RANK);
            int32_t need = safe32(data, 4);
            if (!need) {
                uint32_t misc = masterRow(f, fn::Misc_create, uint32_t(safe32(data, 8)));
                need = safe32(ld32(misc), 4);
                sharedRelease(misc);
            }
            if (rank >= need) {
                uint32_t t = ld32(api + APIManager_tutorial);
                uint32_t lo = ld32(t + Tutorial_flags);
                if (bit == 1) {
                    show = (lo & 1) == 0;
                } else {
                    uint32_t mask = shl(1, uint32_t(bit - 1));
                    uint32_t hi = ld32(t + Tutorial_flags + 4);
                    show = ((uint32_t(int32_t(mask) >> 31) & hi) | (lo & mask)) == 0;
                }
            }
        }
        sharedRelease(row);
        return show;
    }
    if (ld8(ld32(api + APIManager_tutorial) + Tutorial_skipped)) return 0;
    switch (next) {
    case 100: return tutorial_check_sub_100(id);
    case 200: return tutorial_check_sub_200(id);
    case 210: return tutorial_check_sub_210(id);
    case 220: return tutorial_check_sub_220(id);
    case 300: return tutorial_check_sub_300(id);
    case 400: return tutorial_check_sub_400(id);
    case 410: return tutorial_check_sub_410(id);
    case 500: return tutorial_check_sub_500(id);
    case 600: return tutorial_check_sub_600(id);
    case 700: return tutorial_check_sub_700(id);
    case 710: return tutorial_check_sub_710(id);
    default: return 0;
    }
}
RW_FUNCTION(0x472D74, "SceneUtil::dispTutorialCheck", SceneUtil::dispTutorialCheck);

// ------------------------------------------------------------------------------------------------ popups

// Show the tutorial popup `id` (once: unless `force`, popups already seen this session are skipped and the
// callback runs at once). Ids 100-120 are masks over the scene, 1-44 picture popups. Returns (in `result`, a
// std::function<void()>) the popup's close callback, or an empty function.
void SceneUtil::openTutorialPopup(Addr result, uint32_t id, Addr callback, Addr target1, Addr target2, Addr target3,
                                  uint32_t force) {
    if (!force) {
        if (call(AppState_isClearTutorialPopupIDs, {call(fn::AppState_getInstance, {}), id}) == 1) {
            callFunction(callback);
            st32(result + 16, 0);
            return;
        }
    }
    Frame f;
    if (id - 100 < 0x15) {
        uint32_t cb = f.alloc(20);
        call(fn::function_vv_copy, {cb, callback});
        TutorialMask(result, id, cb, target1, target2, target3, force);
        functionDestroy(cb);
    } else if (id - 1 <= 0x2B) {
        uint32_t cb = f.alloc(20);
        call(fn::function_vv_copy, {cb, callback});
        NormalTutorialPopup(result, id, cb, 0);
        functionDestroy(cb);
    } else {
        callFunction(callback);
        st32(result + 16, 0);
    }
}
RW_FUNCTION(0x4684C8, "SceneUtil::openTutorialPopup", SceneUtil::openTutorialPopup);

// A tutorial mask (layout per id) over the scene, letting touches through only on the target widgets.
void SceneUtil::TutorialMask(Addr result, uint32_t id, Addr callback, Addr target1, Addr target2, Addr target3,
                             uint32_t force) {
    // the lambda stores `force` as a byte in a word of the original's stack frame; the other three bytes are
    // whatever that stack slot held on entry (copied into the heap lambda below)
    const uint32_t forceSlot = ld32(rw::cpu->r[13] - 0x64);
    Frame f;
    if (id - 101 < 3) {
        callFunction(callback);
        st32(result + 16, 0);
        return;
    }
    call(fn::AudioPlaySystemSEPopUp, {});
    uint32_t mask = TutorialMaskWidget::create();
    if (!mask) {
        callFunction(callback);
        st32(result + 16, 0);
        return;
    }
    uint32_t k = 4 * (id - 100);
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}),
                                                             ld32(TABLE_MaskJson + k)});
    st32(mask + TutorialMaskWidget::ACTION,
         call(ActionManagerEx_playActionByName,
              {call(ActionManagerEx_getInstance, {}), ld32(TABLE_MaskAction + k), STR_Animation0}));
    vcall(layer, node::addChild, {layer, popup});
    vcall(popup, node::addChild, {popup, mask});
    Vec2 origin = visibleOrigin();
    Size visible = visibleSize();
    uint32_t maskNode = findNode(popup, STR_Mask, 4);
    float shift = 0.0f;
    if (maskNode) {
        float h = ldf(vcall(maskNode, node::getContentSize, {maskNode}) + 4);
        float scaleY = bitsf(vcall(maskNode, node::getScaleY, {maskNode}));
        shift = ((h * scaleY) + -720.0f) * 0.5f - (720.0f - visible.height) * 0.5f;
    }
    switch (ld32(TABLE_MaskPlacement + k)) {
    case 0:
        vcall(popup, node::setAnchorPoint, {popup, VEC2_ANCHOR_BOTTOM_LEFT});
        vcall(popup, node::setPosition, {popup, VEC2_ZERO});
        break;
    case 1: {
        vcall(popup, node::setAnchorPoint, {popup, VEC2_ANCHOR_BOTTOM_RIGHT});
        Frame g;
        vcall(popup, node::setPosition, {popup, newVec2(g, origin.x + visible.width, 0.0f)});
        break;
    }
    case 2: {
        vcall(popup, node::setAnchorPoint, {popup, VEC2_ANCHOR_MIDDLE});
        Frame g;
        vcall(popup, node::setPosition,
              {popup, newVec2(g, origin.x + visible.width * 0.5f, origin.y + visible.height * 0.5f)});
        break;
    }
    default: break;
    }
    if (id - 119 <= 1) {
        float y = bitsf(vcall(popup, node::getPositionY, {popup}));
        Size s = visibleSize();
        vcall(popup, node::setPositionY, {popup, fbits(shift + (y + (s.height + -640.0f) * 0.5f))});
    }
    titleLabel(popup, STR_Txt_Tutorial, 12, int32_t(ld32(TABLE_MaskText + k)));

    uint32_t dispatcher = ld32(director() + 0x84);
    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
    // the close lambda [dispatcher, listener, mask, force, id, layer, popup, callback]
    uint32_t words[7] = {dispatcher, listener, mask, (forceSlot & 0xFFFFFF00u) | (force & 0xFF), id, layer, popup};
    uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, callback});
    uint32_t began = newFunction(f, VT_Mask_TouchBegan, {target1, target2, target3});
    call(TouchFunction_swap, {began, listener + 0x50});
    functionDestroy(began);
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, popup});
    call(fn::Node_runAction, {popup, call(FadeIn_create, {fbits(0.2f)})});
    uint32_t cbCopy = f.alloc(24);
    functionCopy(cbCopy, cb);
    st32(result + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x40});
    if (heap) {
        st32(heap, VT_Mask_Close);
        for (uint32_t w = 0; w < 7; w++) st32(heap + 8 + 4 * w, words[w]);
        functionCopy(heap + 0x28, cbCopy);
    }
    st32(result + 16, heap);
    functionDestroy(cbCopy);
    functionDestroy(cb);
}
RW_FUNCTION(0x47142C, "SceneUtil::TutorialMask", SceneUtil::TutorialMask);

// A picture popup (tutorial_<n>.png with its text) with OK / "go to" buttons; returns its close callback.
void SceneUtil::NormalTutorialPopup(Addr result, int32_t id, Addr callback, uint32_t unused) {
    (void)unused;
    int32_t picture = 40;
    if (id != 12 && id != 41) picture = id != 44 ? id : 6;
    call(fn::AudioPlaySystemSEPopUp, {});
    uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    uint32_t shadow = createShadowSprite();
    uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChildZ, {layer, shadow, 0});
    uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), STR_Tutorial01Json});
    centerPopup(popup);
    vcall(shadow, node::addChildZ, {shadow, popup, 0});
    uint32_t close = findNode(popup, STR_Close_Button, 12);
    if (close) vcall(close, node::setVisible, {close, (uint32_t(id) & ~1u) == 40 ? 1u : 0u});
    uint32_t image = findNode(popup, STR_Tutorial, 8);
    {
        Frame g;
        uint32_t buf = stringBuffer(g, {STR_TutorialPng, uint32_t(picture)});
        uint32_t s = newStr(g, buf);
        call(fn::ImageView_loadTexture, {image, s, 0});
        strFree(s);
    }
    if (picture) titleLabel(popup, STR_Txt, 3, int32_t(ld32(TABLE_PopupText + 4 * uint32_t(picture))));
    uint32_t webView = findNode(layer, STR_webView, 7);
    if (webView) vcall(webView, node::setVisible, {webView, 0});

    Frame f;
    uint32_t cb = f.alloc(20);                       // the lambda temp [layer, shadow, callback]
    call(fn::function_vv_copy, {cb, callback});
    int32_t gotoText = int32_t(ld32(TABLE_PopupGotoText + 4 * uint32_t(id)));
    uint32_t button, other;
    if (gotoText) {
        button = findNode(popup, STR_Button_Goto, 11);
        titleLabel(button, STR_Txt_OK, 6, gotoText);
        other = findNode(popup, STR_Button_OK, 9);
    } else {
        button = findNode(popup, STR_Button_OK, 9);
        titleLabel(button, STR_Txt_OK, 6, 100100001);
        other = findNode(popup, STR_Button_Goto, 11);
    }
    vcall(button, node::setVisible, {button, 1});
    vcall(other, node::setVisible, {other, 0});
    // touch lambdas [button, close, popup, webView, id] (heap: 20 bytes of captures)
    auto touch = [&](uint32_t widget, uint32_t vtable) {
        Frame g;
        uint32_t fo = g.alloc(20);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, vtable);
            st32(heap + 4, button);
            st32(heap + 8, close);
            st32(heap + 12, popup);
            st32(heap + 16, webView);
            st32(heap + 20, uint32_t(id));
        }
        st32(fo + 16, heap);
        call(fn::Widget_addTouchEventListener, {widget, fo});
        functionDestroy(fo);
    };
    touch(button, VT_Popup_OkTouch);
    if ((uint32_t(id) & ~1u) == 0x28 && close) touch(close, VT_Popup_CloseTouch);
    {
        Frame g;
        uint32_t none = newEmptyFunction(g);
        call(fn::Node_runAction, {popup, createOpenPopupAnimation(popup, none)});
        functionDestroy(none);
    }
    uint32_t mgr = sceneManager();
    uint32_t canClose = newFunction(f, VT_Popup_CanClose, {button});
    uint32_t captured = f.alloc(0x20);       // blockLoading at +0, the std::function at +8 (0x1C bytes)
    functionCopy(captured, cb);
    uint32_t onClose = f.alloc(20);
    st32(onClose + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, VT_Popup_OnClose);
        st32(heap + 8, layer);
        st32(heap + 12, shadow);
        functionCopy(heap + 0x10, captured);
    }
    st32(onClose + 16, heap);
    call(fn::SceneManager_openPopup, {mgr, canClose, onClose, 0});
    functionDestroy(onClose);
    functionDestroy(captured);
    functionDestroy(canClose);
    // the returned close callback [webView]
    st32(result + 16, result);
    if (result) {
        st32(result, VT_Popup_Close);
        st32(result + 4, webView);
    }
    functionDestroy(cb);
}
RW_FUNCTION(0x471B7C, "SceneUtil::NormalTutorialPopup", SceneUtil::NormalTutorialPopup);

// ------------------------------------------------------------------------------------------------ TutorialMaskWidget

SceneUtil::Addr SceneUtil::TutorialMaskWidget::create() {
    uint32_t w = call(fn::operator_new, {SIZE});
    call(Memset, {w, SIZE, 0});
    call(Layout_ctor, {w});
    TutorialMaskWidget *self = rw::host_ptr<TutorialMaskWidget>(w);
    self->vtable = VT_TutorialMaskWidget;
    self->vtable2 = VT_TutorialMaskWidget_2;
    self->vtable3 = VT_TutorialMaskWidget_3;
    self->action = 0;
    call(Ref_autorelease, {w});
    return w;
}
RW_FUNCTION(0x471A3C, "SceneUtil::TutorialMaskWidget::create", SceneUtil::TutorialMaskWidget::create);

SceneUtil::TutorialMaskWidget *SceneUtil::TutorialMaskWidget::destroy(TutorialMaskWidget *self) {
    self->vtable = VT_TutorialMaskWidget;
    self->vtable2 = VT_TutorialMaskWidget_2;
    self->vtable3 = VT_TutorialMaskWidget_3;
    call(Node_removeFromParentAndCleanup, {rw::guest_addr(self), 1});
    call(Layout_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x471AC4, "SceneUtil::TutorialMaskWidget::~TutorialMaskWidget", SceneUtil::TutorialMaskWidget::destroy);

void SceneUtil::TutorialMaskWidget::destroyDelete(TutorialMaskWidget *self) {
    call(fn::operator_delete, {rw::guest_addr(destroy(self))});
}
RW_FUNCTION(0x471B48, "SceneUtil::TutorialMaskWidget::~TutorialMaskWidget (deleting)",
            SceneUtil::TutorialMaskWidget::destroyDelete);

bool SceneUtil::TutorialMaskWidget::init(TutorialMaskWidget *self) {
    (void)self;
    return true;
}
RW_FUNCTION(0x47ABD2, "SceneUtil::TutorialMaskWidget::init", SceneUtil::TutorialMaskWidget::init);

// ================================================================================================ the lambdas
namespace {
namespace tutorialpopup {
constexpr uint32_t AppState_addClearTutorialPopupIDs = 0x4EB7A9;   // (app, id)
constexpr uint32_t SceneUtil_ClearTutorialPopupIDs = 0x4732AD;     // (id, function<void()>)
constexpr uint32_t SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t VT_GotoClear = 0xF54508, VT_OkClear = 0xF5458C;   // $_70 / $_69's step {popup, button, layer}
constexpr uint32_t VT_GotoClosed = 0xF54534, VT_OkClosed = 0xF545B8; // the steps' close ends {button, node}
constexpr uint32_t TOUCH_ENDED = 2;

// $_70 / $_69 (Go to / OK) {button, other, popup, layer, tutorial id}(sender, type): both buttons off (OK: the
// other may be missing), the decide sound, the popup marked seen, then its close (`step` {popup, button, layer})
void buttonTouched(uint32_t f, uint32_t typeRef, bool otherOptional, uint32_t step) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    vcall(ld32(f + 4), node::setTouchEnabled, {ld32(f + 4), 0});
    const uint32_t other = ld32(f + 8);
    if (!otherOptional || other) vcall(other, node::setTouchEnabled, {other, 0});
    call(fn::AudioPlaySystemSEDecide, {});
    const uint32_t popup = ld32(f + 0xC), layer = ld32(f + 0x10), button = ld32(f + 4);
    call(AppState_addClearTutorialPopupIDs, {call(fn::AppState_getInstance, {}), ld32(f + 0x14)});
    Frame fr;
    const uint32_t next = newFunction(fr, step, {popup, button, layer});
    call(SceneUtil_ClearTutorialPopupIDs, {ld32(f + 0x14), next});
    functionDestroy(next);
}
void gotoTouched(uint32_t f, uint32_t, uint32_t typeRef) { buttonTouched(f, typeRef, false, VT_GotoClear); }
void okTouched(uint32_t f, uint32_t, uint32_t typeRef) { buttonTouched(f, typeRef, true, VT_OkClear); }

// the step {popup, button, layer}: the popup shrinks (its end `vt` gets {button, layer})
void shrinkStep(uint32_t f, uint32_t vt) {
    const uint32_t popup = ld32(f + 4);
    Frame fr;
    const uint32_t end = newFunction(fr, vt, {ld32(f + 8), ld32(f + 0xC)});
    call(fn::Node_runAction, {popup, call(SceneUtil_createClosePopupAnimation, {ld32(f + 0xC), end})});
    functionDestroy(end);
}
void gotoStep(uint32_t f) { shrinkStep(f, VT_GotoClosed); }
void okStep(uint32_t f) { shrinkStep(f, VT_OkClosed); }

// the close end {button, node}: the popup closed (`arg`) with the button on meanwhile, the node shown again
void closedEnd(uint32_t f, uint32_t arg) {
    vcall(ld32(f + 4), node::setTouchEnabled, {ld32(f + 4), 1});
    call(SM_closeCurrentPopup, {sceneManager(), arg});
    const uint32_t shown = ld32(f + 8);
    if (shown) vcall(shown, node::setVisible, {shown, 1});
    vcall(ld32(f + 4), node::setTouchEnabled, {ld32(f + 4), 0});
}
void gotoClosedEnd(uint32_t f) { closedEnd(f, 0); }
void okClosedEnd(uint32_t f) { closedEnd(f, 1); }
// $_73 {node}: the popup closed, the node shown again
void closedShow(uint32_t f) {
    call(SM_closeCurrentPopup, {sceneManager(), 0});
    const uint32_t shown = ld32(f + 4);
    if (shown) vcall(shown, node::setVisible, {shown, 1});
}
}  // namespace tutorialpopup
}  // namespace

RW_FUNCTION(0x4831E0, "SceneUtil::NormalTutorialPopup $_70::operator() (Go to)", tutorialpopup::gotoTouched);
RW_FUNCTION(0x483584, "SceneUtil::NormalTutorialPopup $_69::operator() (OK)", tutorialpopup::okTouched);
RW_FUNCTION(0x483360, "SceneUtil::NormalTutorialPopup $_70 lambda()#1::operator()", tutorialpopup::gotoStep);
RW_FUNCTION(0x483700, "SceneUtil::NormalTutorialPopup $_69 lambda()#1::operator()", tutorialpopup::okStep);
RW_FUNCTION(0x4834A6, "SceneUtil::NormalTutorialPopup $_70 lambda()#1 lambda()#1::operator()", tutorialpopup::gotoClosedEnd);
RW_FUNCTION(0x483842, "SceneUtil::NormalTutorialPopup $_69 lambda()#1 lambda()#1::operator()", tutorialpopup::okClosedEnd);
RW_FUNCTION(0x482EB6, "SceneUtil::NormalTutorialPopup $_73::operator()", tutorialpopup::closedShow);

namespace {
// updateTutorialStatus $_74 (sent) {-, show errors +8, -, done function<void()> +0x10}(ok): the indicator off;
// ok -> done(); else the API error when errors are shown
void tutorialStatusUpdated(uint32_t f, uint32_t okRef) {
    constexpr uint32_t VT_StatusError = 0xF5442C;
    const uint32_t ok = ld8(okRef);
    call(SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        if (ld32(f + 0x20)) call(fn::function_vv_call, {f + 0x10});
    } else if (ld8(f + 8)) {
        apiErrorDialogIfShown(VT_StatusError);
    }
}
}  // namespace

RW_FUNCTION(0x482CC4, "SceneUtil::updateTutorialStatus $_74::operator()(bool)", tutorialStatusUpdated);
