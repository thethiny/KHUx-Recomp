// SceneTitle: lambda bodies that run in game but were left after batch 2 (batch 3, named from their __func vtables'
// target_type). `functor` is the __func object: vtable at +0, then the captures (the scene at +4).
#include "SceneTitle.hpp"
#include "../core/SafeBuffer.hpp"

using namespace social;

namespace {

constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_executeSystemNeedUrl = 0x4CD619;     // (api, const std::function<void(bool)>&)
constexpr uint32_t APIManager_executeSystemCoppaGet = 0x4CD79D;
constexpr uint32_t APIManager_executeUserGet = 0x4C6081;
constexpr uint32_t APIManager_executeTutorialClear = 0x4C5C89;
constexpr uint32_t APIManager_executeTutorialProgress = 0x4C581D;  // (api, phase, int, int, ..., const function&)
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;       // (mgr, function<Layer*()>*, bool, bool, FadeType)
constexpr uint32_t SceneManager_setFinishSceneCallback = 0x613E3F; // (mgr, std::function<...>)
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_resetClearTutorialPopupIDs = 0x4EB8B5;
constexpr uint32_t SceneUtil_freeTutorialConvertData = 0x47250D;   // (std::function<void()>)
constexpr uint32_t SceneUtil_updateTutorialPhase = 0x472BA1;       // (phase, std::function*, bool, bool)
constexpr uint32_t SceneUnionRegister_callTutorialStageStartAPI = 0x71792D;
constexpr uint32_t SceneNotice_callPrepareAPI = 0x733AD9;          // (std::function<void(bool)>)
constexpr uint32_t SceneNotice_create = 0x67F541;
constexpr uint32_t SceneDownload_create = 0x67F36D;
constexpr uint32_t ResourceDataManager_getInstance = 0x7728AD;
constexpr uint32_t ResourceDataManager_mount = 0x773355;
constexpr uint32_t FunctionLayer_assign = 0x612E29;                // std::function<Layer*()>::operator=(const function&)
constexpr uint32_t FunctionVB_assign = 0x490109;                   // std::function<void(bool)>::operator=
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;                       // (float duration, uint8 opacity)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;                     // (const std::function<void()>&)
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;                     // (action, ..., nullptr)
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t start_movie = 0x6C4A55;                         // (int, bool): the intro movie layer
constexpr uint32_t SHARED_ADD_REF = 0xC8DBB5, SHARED_RELEASE = 0xC8DBD1;
constexpr uint32_t GOT_COLOR3B = 0xFD6060;                         // the tap overlay's color (a Color3B in the GOT)

// SceneTitle's own helpers (unnamed): stopped = the title must not go on; the second answers "is the tutorial
// still running"
constexpr uint32_t Title_isStopped = 0x67F295;                     // sub_67F294
constexpr uint32_t Title_tutorialState = 0x67F1AD;                 // sub_67F1AC
constexpr uint32_t Title_resumeLayer = 0x67F3C1;                   // sub_67F3C0: the layer to resume the tutorial in

// SceneTitle fields
constexpr uint32_t TITLE_ANIMATION = 0x2A4, TITLE_ANIMATION_2 = 0x2A8, TAP_OVERLAY = 0x2AC, TAP_READY = 0x2B0;
constexpr uint32_t MOVIE_END_FRAME = 0x2B4, TOUCH_ON = 0x2B8;
constexpr uint32_t V_getAnimation = 0x298;            // cocostudio::Armature::getAnimation
constexpr uint32_t V_animFrameIndex = 0x1C;           // ArmatureAnimation: the current frame
constexpr uint32_t V_animGotoFrame = 0x7C;            // ArmatureAnimation: go to a frame

// APIManager / SceneManager fields
constexpr uint32_t API_TUTORIAL = 0x4C;               // shared_ptr (SafeBuffer: the stored tutorial phase at +4)
constexpr uint32_t API_USER = 0x94;                   // SafeBuffer: +0x40 the tutorial converted flag
constexpr uint32_t API_LOGIN = 0x64;                  // login state: byte +0x10
constexpr uint32_t API_STORY_CTRL = 0x128;            // shared_ptr control block (StageStory)
constexpr uint32_t SM_NEXT_STAGE = 0x2C8, SM_ADVENTURE = 0x28C;
constexpr uint32_t SM_LOADING_BLOCKED = 0x4CC, SM_ON_FINISH_LOADING = 0x548;
constexpr uint32_t LAYER_FACTORY = 0x2C8;             // SceneDownload: std::function<Layer*()> of the next layer

constexpr uint32_t VT_needUrl = 0xF6430C, VT_coppa = 0xF64338, VT_userGet = 0xF64364, VT_tutorialClear = 0xF643BC;
constexpr uint32_t VT_freeConvert = 0xF64390;
constexpr uint32_t VT_titleShown = 0xF6425C;
constexpr uint32_t VT_tapFadeIn = 0xF640FC, VT_tapFadeOut = 0xF640D0, VT_tapNext = 0xF6404C;
constexpr uint32_t VT_nextLayer = 0xF64078, VT_afterDownload = 0xF640A4;

constexpr uint32_t AppState_needsUpdateData = 0x4EAE5D;
constexpr uint32_t AppState_enumerateEventResourceIDsToDownload = 0x4EAD05;   // (vector<unsigned>* out, app)
constexpr uint32_t UIntVector_dtor = 0x485ABB;
constexpr uint32_t API_LOGIN_STATE = 0x44;           // shared_ptr: the login state (+0: its phase)

// SceneManager: no "finished loading" callback, loading blocked
void blockLoading() {
    const uint32_t mgr = call(SceneManager_getInstance, {});
    Frame f;
    const uint32_t empty = newEmptyFunction(f);
    st8(mgr + SM_LOADING_BLOCKED, 1);
    call(FunctionVB_assign, {mgr + SM_ON_FINISH_LOADING, empty});
    functionDestroy(empty);
}

// api->request(std::function holding the inline lambda {vtable, captures})
void requestWith(uint32_t request, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    const uint32_t api = call(APIManager_getInstance, {});
    withLambda(vtable, captures, [&](uint32_t fo) { call(request, {api, fo}); });
}

}  // namespace

struct SceneTitleBatch3 {
    // sub_67F1AC: in the tutorial (the login state says 61, or the tutorial record's flag byte 1)
    static uint32_t tutorialState() {
        uint32_t api = call(APIManager_getInstance, {});
        const uint32_t login = ld32(api + API_LOGIN_STATE), loginCtrl = ld32(api + API_LOGIN_STATE + 4);
        if (loginCtrl) call(SHARED_ADD_REF, {loginCtrl});
        api = call(APIManager_getInstance, {});
        const uint32_t tutorial = ld32(api + API_TUTORIAL), tutorialCtrl = ld32(api + API_TUTORIAL + 4);
        if (tutorialCtrl) call(SHARED_ADD_REF, {tutorialCtrl});
        uint32_t result;
        if (login && ld32(login) == 0x3D) {
            result = 1;
        } else if (tutorial) {
            const SafeBuffer *sb = rw::host_ptr<SafeBuffer>(tutorial);
            result = uint8_t(ld8(sb->data.addr + 1) ^ SafeBuffer::mask(sb->key, 1)) != 0 ? 1 : 0;
        } else {
            result = 0;
        }
        if (tutorialCtrl) call(SHARED_RELEASE, {tutorialCtrl});
        if (loginCtrl) call(SHARED_RELEASE, {loginCtrl});
        return result;
    }

    // sub_67F294: the title must download first (data or event resources to update; in the tutorial, past phase 2)
    static uint32_t mustDownload() {
        if (!call(AppState_needsUpdateData, {call(AppState_getInstance, {})})) {
            Frame f;
            const uint32_t ids = f.alloc(12);
            call(AppState_enumerateEventResourceIDsToDownload, {ids, call(AppState_getInstance, {})});
            const uint32_t begin = ld32(ids), end = ld32(ids + 4);
            call(UIntVector_dtor, {ids});
            if (begin == end) return 0;
        }
        if (call(Title_tutorialState, {}) != 1) return 1;
        const uint32_t api = call(APIManager_getInstance, {});
        const uint32_t tutorial = ld32(api + API_TUTORIAL), ctrl = ld32(api + API_TUTORIAL + 4);
        if (ctrl) call(SHARED_ADD_REF, {ctrl});
        const uint32_t result = safe32(tutorial, 4) > 2 ? 1 : 0;
        if (ctrl) call(SHARED_RELEASE, {ctrl});
        return result;
    }

    // init $_0 (bool): the server-URL check, then the COPPA settings, then the tutorial / user state
    static void loginDone(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        requestWith(APIManager_executeSystemNeedUrl, VT_needUrl, {ld32(functor + 4)});
    }
    static void needUrlDone(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        requestWith(APIManager_executeSystemCoppaGet, VT_coppa, {ld32(functor + 4)});
    }
    static void coppaDone(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        const uint32_t self = ld32(functor + 4);
        if (call(Title_isStopped, {})) return;
        const uint32_t state = call(Title_tutorialState, {});
        const uint32_t api = call(APIManager_getInstance, {});
        if (state != 1) {
            withLambda(VT_userGet, {self}, [&](uint32_t fo) { call(APIManager_executeUserGet, {api, fo}); });
            return;
        }
        // in the tutorial: report the stored phase (6: cleared)
        const uint32_t tutorial = ld32(api + API_TUTORIAL), ctrl = ld32(api + API_TUTORIAL + 4);
        if (ctrl) call(SHARED_ADD_REF, {ctrl});
        if (safe32(tutorial, 4) == 6) {
            blockLoading();
            const uint32_t api2 = call(APIManager_getInstance, {});
            withLambda(VT_tutorialClear, {}, [&](uint32_t fo) { call(APIManager_executeTutorialClear, {api2, fo}); });
        } else {
            const int32_t phase = safe32(tutorial, 4);
            blockLoading();
            const uint32_t api2 = call(APIManager_getInstance, {});
            Frame f;
            const uint32_t none = newEmptyFunction(f);
            call(APIManager_executeTutorialProgress, {api2, uint32_t(phase), 0, 0, 0, 0, 0, 0, none});
            functionDestroy(none);
        }
        if (ctrl) call(SHARED_RELEASE, {ctrl});
    }

    // init $_9 {lambda(bool)#2}: the converted tutorial data is freed first, else the tutorial stage starts
    static void userReady(uint32_t /*functor*/, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        if (call(Title_isStopped, {})) return;
        if (safe32(ld32(call(APIManager_getInstance, {}) + API_USER), 0x40)) {
            Frame f;
            const uint32_t done = newFunction(f, VT_freeConvert, {});
            call(SceneUtil_freeTutorialConvertData, {done});
            functionDestroy(done);
        } else {
            call(SceneUnionRegister_callTutorialStageStartAPI, {});
        }
    }
    // ... {lambda()#1}: the notices' APIs
    static void convertFreed(uint32_t /*functor*/) {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SceneNotice_callPrepareAPI, {none});
        functionDestroy(none);
    }

    // init $_3 [this]: show the animations and fade the tap overlay out (then $_3's follow-up)
    static void titleShown(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        const uint32_t animation = ld32(self + TITLE_ANIMATION);
        vcall(animation, node::setVisible, {animation, 1});
        const uint32_t fade = call(EaseSineOut_create, {call(FadeTo_create, {fbits(0.75f), 0})});
        const uint32_t overlay = ld32(self + TAP_OVERLAY);
        vcall(overlay, node::setVisible, {overlay, 1});
        const uint32_t target = ld32(self + TAP_OVERLAY);
        Frame f;
        const uint32_t done = newFunction(f, VT_titleShown, {self});
        const uint32_t next = call(CallFunc_create, {done});
        call(Node_runAction, {target, call(Sequence_create, {fade, next, 0})});
        functionDestroy(done);
    }

    // init $_7 [this](Touch*, Event*): a tap during the intro shows the tap overlay (fade in, then out); after
    // the intro the tap goes on
    static void titleTouched(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        const uint32_t armature = ld32(self + TITLE_ANIMATION);
        const uint32_t animation = vcall(armature, V_getAnimation, {armature});
        const int32_t frame = int32_t(vcall(animation, V_animFrameIndex, {animation}));
        if (!ld8(self + TOUCH_ON)) return;
        const int32_t end = int32_t(ld32(self + MOVIE_END_FRAME));
        if (frame < end - 61) {
            st8(self + TAP_READY, 0);
            const uint32_t overlay = ld32(self + TAP_OVERLAY);
            vcall(overlay, node::setColor, {overlay, ld32(GOT_COLOR3B)});
            vcall(ld32(self + TAP_OVERLAY), node::setOpacity, {ld32(self + TAP_OVERLAY), 0});
            const uint32_t fadeIn = call(EaseSineOut_create, {call(FadeTo_create, {fbits(0.25f), 0xCC})});
            const uint32_t fadeOut = call(EaseSineOut_create, {call(FadeTo_create, {fbits(0.75f), 0})});
            vcall(ld32(self + TAP_OVERLAY), node::setVisible, {ld32(self + TAP_OVERLAY), 1});
            const uint32_t target = ld32(self + TAP_OVERLAY);
            Frame f;
            const uint32_t in = newFunction(f, VT_tapFadeIn, {self});
            const uint32_t afterIn = call(CallFunc_create, {in});
            const uint32_t out = newFunction(f, VT_tapFadeOut, {self});
            const uint32_t afterOut = call(CallFunc_create, {out});
            call(Node_runAction, {target, call(Sequence_create, {fadeIn, afterIn, fadeOut, afterOut, 0})});
            functionDestroy(out);
            functionDestroy(in);
        } else if (frame >= end - 1 && ld8(self + TAP_READY)) {
            call(AudioPlaySystemSEDecide, {});
            Frame f;
            const uint32_t next = newFunction(f, VT_tapNext, {});
            call(fn::function_vv_call, {next});
            functionDestroy(next);
        }
    }
    // ... {lambda()#1}: both animations to the intro's last frame
    static void skipIntro(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        for (uint32_t field : {TITLE_ANIMATION, TITLE_ANIMATION_2}) {
            const uint32_t armature = ld32(self + field);
            const uint32_t animation = vcall(armature, V_getAnimation, {armature});
            vcall(animation, V_animGotoFrame, {animation, ld32(self + MOVIE_END_FRAME) - 1});
        }
    }
    // ... {lambda()#3}: on to the next layer
    static void titleNext(uint32_t /*functor*/) {
        const uint32_t mgr = call(SceneManager_getInstance, {});
        Frame f;
        const uint32_t factory = newFunction(f, VT_nextLayer, {});
        call(SceneManager_changeSceneLayer, {mgr, factory, 1, 1, 1});
        functionDestroy(factory);
    }
    // ... the next layer: the download screen, the interrupted tutorial, the notices or the intro movie
    static uint32_t nextLayer(uint32_t /*functor*/) {
        const uint32_t state = call(Title_tutorialState, {});
        const uint32_t ctrl = ld32(call(APIManager_getInstance, {}) + API_STORY_CTRL);
        if (ctrl) call(SHARED_ADD_REF, {ctrl});
        st32(call(SceneManager_getInstance, {}) + SM_NEXT_STAGE, 0);
        call(AppState_resetClearTutorialPopupIDs, {call(AppState_getInstance, {})});
        {
            const uint32_t mgr = call(SceneManager_getInstance, {});
            Frame f;
            const uint32_t none = newEmptyFunction(f);
            call(SceneManager_setFinishSceneCallback, {mgr, none});
            functionDestroy(none);
        }
        uint32_t layer = 0;
        if (call(Title_isStopped, {}) == 1) {
            layer = call(SceneDownload_create, {});
            if (state == 1) {
                Frame f;
                const uint32_t after = newFunction(f, VT_afterDownload, {});
                call(FunctionLayer_assign, {layer + LAYER_FACTORY, after});
                functionDestroy(after);
            }
        } else if (state == 1) {
            layer = call(Title_resumeLayer, {});
        } else {
            call(ResourceDataManager_mount, {call(ResourceDataManager_getInstance, {})});
            if (safe32(ld32(call(APIManager_getInstance, {}) + API_USER), 0x40)) {
                layer = call(SceneNotice_create, {});
            } else {
                blockLoading();
                const uint32_t login = ld32(call(APIManager_getInstance, {}) + API_LOGIN);
                Frame f;
                const uint32_t none = newEmptyFunction(f);
                if (login && ld8(login + 0x10))
                    call(SceneUtil_updateTutorialPhase, {995, none, 1, 1});
                else
                    call(SceneUtil_updateTutorialPhase, {50, none, 0, 1});
                functionDestroy(none);
                call(AppState_resetClearTutorialPopupIDs, {call(AppState_getInstance, {})});
                st32(call(SceneManager_getInstance, {}) + SM_ADVENTURE, 1);
                layer = call(start_movie, {0, 0});
            }
        }
        if (ctrl) call(SHARED_RELEASE, {ctrl});
        return layer;
    }
};

using L = SceneTitleBatch3;
RW_FUNCTION(0x68054C, "SceneTitle::init::$_0::operator()", L::loginDone);
RW_FUNCTION(0x680688, "SceneTitle::init::$_0::{lambda(bool)#1}::operator()", L::needUrlDone);
RW_FUNCTION(0x6807C4, "SceneTitle::init::$_0::{lambda(bool)#1}::{lambda(bool)#1}::operator()", L::coppaDone);
RW_FUNCTION(0x680A9C, "SceneTitle::init::$_9::{lambda(bool)#2}::operator()", L::userReady);
RW_FUNCTION(0x680BEC, "SceneTitle::init::$_9::{lambda(bool)#2}::{lambda()#1}::operator()", L::convertFreed);
RW_FUNCTION(0x6801CC, "SceneTitle::init::$_3::operator()", L::titleShown);
RW_FUNCTION(0x67EB74, "SceneTitle::init::$_7::operator()", L::titleTouched);
RW_FUNCTION(0x67FAC2, "SceneTitle::init::$_7::{lambda()#1}::operator()", L::skipIntro);
RW_FUNCTION(0x67EE04, "SceneTitle::init::$_7::{lambda()#3}::operator()", L::titleNext);
RW_FUNCTION(0x67EF28, "SceneTitle::init::$_7::{lambda()#3}::{lambda()#1}::operator()", L::nextLayer);
RW_FUNCTION(0x67F1AC, "SceneTitle (sub_67F1AC): in the tutorial", L::tutorialState);
RW_FUNCTION(0x67F294, "SceneTitle (sub_67F294): must download", L::mustDownload);
