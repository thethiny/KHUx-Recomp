// The fusion's lambdas, part 1: the rare caution popup's close callback ($_18) and its close-animation lambda ($_36);
// runMixed's enhance callback ($_39: tutorial phase, then $_38) and $_38, the fusion animation: the result LWF for
// the number of materials, the base medal and each material put into its "drowMedal%03d" movie when that movie
// loads, a FlashMovieView on popup layer 2, and when it ends (after one frame's delay) the result popup.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_CompleteTutorialPhase = 0x472A75;   // (function<void()> by value, bool, bool)
constexpr uint32_t FunctionVB_copy = 0x45C84D, FunctionVB_call = 0x474139;
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;
constexpr uint32_t StringFromChars = 0x47CD93;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t MovieMap_destroy = 0x530811, MovieMap_insertUnique = 0x530869;  // map<string, fn<void(Movie*)>>
constexpr uint32_t FlashMovieView_create = 0x5682A1;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_updateHeader = 0x611169, SM_getPopupLayer2 = 0x613E71;
constexpr uint32_t Mixed_openResultPopup = 0x69DE35;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t LWF_AddMovieEventHandler = 0x808645;
constexpr uint32_t CallFunc_create = 0x866B39, Sequence_create = 0x867999, DelayTime_create = 0x86852D;
constexpr uint32_t FadeTo_create = 0x86B3E1, Node_runAction = 0x894AED;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t aeabi_d2f = 0xCD6B24;
constexpr uint32_t memcpy = 0x40E164;            // (Hex-Rays shows these constant-length copies as strcpy)
}  // namespace g

// SceneMixedMedal
constexpr uint32_t F_BASE = 0x31C;                 // the base medal's UserMedalData
constexpr uint32_t F_MATERIAL_IDS = 0x394;         // vector<int>: the materials' master ids
constexpr uint32_t VT_RareClose = 0xF65B50;        // $_18 {ok, close, window, layer, shadow, popup, fn<void(bool)>}
constexpr uint32_t VT_RareRemove = 0xF65B7C;       // $_36 {layer, shadow, popup}
constexpr uint32_t VT_EnhanceDone = 0xF658E8;      // $_39 {this, this}
constexpr uint32_t VT_Animation = 0xF65914;        // $_38 {this, this}
constexpr uint32_t VT_EnhanceError = 0xF659F0;     // $_39's error dialog callback {}
constexpr uint32_t VT_BaseLoad = 0xF659C4;         // $_38 "load" handler {node, base medal shared_ptr}
constexpr uint32_t VT_MaterialLoad = 0xF65998;     // $_38 "load" handler {material master id, node}
constexpr uint32_t VT_MovieDone = 0xF65940;        // $_38 movie end {layer, view, this, node} (heap 0x14)
constexpr uint32_t VT_ShowResult = 0xF6596C;       // {layer, view, this}
constexpr uint32_t S_ResultLwf = 0xE05297;         // "lwf/medal/compose/strength/card_strength_result_%02d/...lwf"
constexpr uint32_t S_DrowMedal001 = 0xE052EA, S_DrowMedalFmt = 0xE04B36;   // "drowMedal001", "drowMedal%03d"
constexpr uint32_t S_MedalLwf = 0xE052F7;          // "mixture/medal/compose/%d/Medal_L/Medal_L.lwf"
constexpr uint32_t S_Empty = 0xDF8A1A;
constexpr uint32_t NODE_LWF = 0x3E0;               // the LWF node's LWF::LWF*
constexpr uint32_t VIEW_ON_END = 0x278, VIEW_AUTO_REMOVE = 0x274;   // FlashMovieView: fn<void()>, flag
constexpr uint32_t V_setTouchEnabledW = 0x29C, V_attachMovieLwf = 0x2F0;

// getMedalData(out, manager, user id, master id, UserMedal::Medal{} by value)
void medalData(uint32_t out, uint32_t lo, uint32_t hi, uint32_t masterId) {
    const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
    call(g::MedalDataManager_getMedalData, {out, mdm, lo, hi, masterId, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0});
}
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// LWF::AddMovieEventHandler(lwf, key, { {"load", handler} }): the one-pair map built from an initializer list
void addLoadHandler(uint32_t lwf, uint32_t key, uint32_t vt, uint32_t w1, uint32_t w2, uint32_t w3, int words) {
    Frame f;
    const uint32_t pair = f.alloc(0x28);   // pair<const string, function<void(Movie*)>>
    st8(pair, 8);
    st8(pair + 1, 'l');
    st8(pair + 2, 'o');
    st8(pair + 3, 'a');
    st8(pair + 4, 'd');
    st8(pair + 5, 0);
    const uint32_t fnAt = pair + 0x10;
    st32(fnAt + 0x10, fnAt);
    st32(fnAt, vt);
    st32(fnAt + 4, w1);
    st32(fnAt + 8, w2);
    if (words > 2) st32(fnAt + 0xC, w3);
    const uint32_t map = f.alloc(12);
    st32(map + 4, 0);
    st32(map + 8, 0);
    st32(map, map + 4);
    call(g::MovieMap_insertUnique, {map, map + 4, pair});
    call(g::LWF_AddMovieEventHandler, {lwf, key, map});
    call(g::MovieMap_destroy, {map, ld32(map + 4)});
    functionDestroy(fnAt);
    strFree(pair);
}
}  // namespace

struct MixedFusionLambdas {
    static uint32_t cloneRareClose(uint32_t functor);
    static void destroyRareClose(uint32_t functor);
    static void rareClose(uint32_t functor, uint32_t okRef);
    static void cloneRareRemove(uint32_t functor, uint32_t dst);
    static void rareRemove(uint32_t functor);
    static void cloneTwoWords(uint32_t functor, uint32_t dst);
    static void enhanceDone(uint32_t functor, uint32_t okRef);
    static void cloneAnimation(uint32_t functor, uint32_t dst);
    static void animation(uint32_t functor);
    static uint32_t cloneMovieDone(uint32_t functor);
    static void movieDone(uint32_t functor);
    static void cloneShowResult(uint32_t functor, uint32_t dst);
    static void showResult(uint32_t functor);
    static void cloneMaterialLoad(uint32_t functor, uint32_t dst);
    static void materialLoad(uint32_t functor, uint32_t movieRef);
    static void cloneBaseLoad(uint32_t functor, uint32_t dst);
    static void baseLoad(uint32_t functor, uint32_t movieRef);
};

// ---------------------------------------------------------------------------------------------- the caution popup
uint32_t MixedFusionLambdas::cloneRareClose(uint32_t functor) {
    const uint32_t copy = call(fn::operator_new, {0x38});
    if (copy) {
        st32(copy, VT_RareClose);
        for (uint32_t k = 8; k < 0x20; k += 4) st32(copy + k, ld32(functor + k));
        call(g::FunctionVB_copy, {copy + 0x20, functor + 0x20});
    }
    return copy;
}

void MixedFusionLambdas::destroyRareClose(uint32_t functor) {
    functionDestroy(functor + 0x20);
    call(fn::operator_delete, {functor});
}

// $_18 (the popup closes with OK / Cancel): both buttons stop taking touches, the decide / cancel sound, the close
// animation on the window (then $_36), the shadow fades out, and the callback gets the answer.
void MixedFusionLambdas::rareClose(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    for (uint32_t at : {8u, 0xCu}) {
        const uint32_t b = ld32(functor + at);
        vcall(b, V_setTouchEnabledW, {b, 0});
    }
    call(ok ? g::AudioPlaySystemSEDecide : g::AudioPlaySystemSECancel, {});
    const uint32_t window = ld32(functor + 0x10);
    {
        Frame f;
        const uint32_t done =
            newFunction(f, VT_RareRemove, {ld32(functor + 0x14), ld32(functor + 0x18), ld32(functor + 0x1C)});
        call(g::Node_runAction, {window, call(g::SceneUtil_createClosePopupAnimation, {window, done})});
        functionDestroy(done);
    }
    const uint32_t shadow = ld32(functor + 0x18);
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), 0})});
    if (ld32(functor + 0x30)) call(g::FunctionVB_call, {functor + 0x20, ok ? 1u : 0u});
}

void MixedFusionLambdas::cloneRareRemove(uint32_t functor, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_RareRemove);
    st32(dst + 0xC, ld32(functor + 0xC));
    st32(dst + 8, ld32(functor + 8));
    st32(dst + 4, ld32(functor + 4));
}

// $_36: the shadow and the popup leave the layer
void MixedFusionLambdas::rareRemove(uint32_t functor) {
    const uint32_t layer = ld32(functor + 4);
    vcall(layer, node::removeChild, {layer, ld32(functor + 8), 1});
    vcall(layer, node::removeChild, {layer, ld32(functor + 0xC), 1});
}

// ---------------------------------------------------------------------------------------------- the enhance result
// __clone(p) of a lambda holding two words ($_39 and $_38 share the shape)
void MixedFusionLambdas::cloneTwoWords(uint32_t functor, uint32_t dst) {
    if (!dst) return;
    st32(dst, ld32(functor));
    st32(dst + 8, ld32(functor + 8));
    st32(dst + 4, ld32(functor + 4));
}

// $_39 (the enhance API answered): the loading indicator goes; on success the tutorial phase is completed and then
// the animation ($_38) runs, otherwise the API's error dialog when it asks for one.
void MixedFusionLambdas::enhanceDone(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    Frame f;
    if (ok) {
        const uint32_t next = newFunction(f, VT_Animation, {ld32(functor + 4), ld32(functor + 8)});
        call(g::SceneUtil_CompleteTutorialPhase, {next, 0, 1});
        functionDestroy(next);
        return;
    }
    if (!ld8(call(g::APIManager_getInstance, {}) + 0x14)) return;
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t cb = newFunction(f, VT_EnhanceError, {});
    call(g::APIManager_openErrorDialog, {api, cb, 1});
    functionDestroy(cb);
}

void MixedFusionLambdas::cloneAnimation(uint32_t functor, uint32_t dst) { cloneTwoWords(functor, dst); }

// $_38: the fusion animation
void MixedFusionLambdas::animation(uint32_t functor) {
    const uint32_t me = ld32(functor + 4);
    Frame f;
    uint32_t node;
    {
        const uint32_t count = uint32_t(int32_t(ld32(me + F_MATERIAL_IDS + 4) - ld32(me + F_MATERIAL_IDS)) >> 2);
        const uint32_t path = stringBuffer(f, {S_ResultLwf, count, count});
        node = call(g::AudioLWFNode_create, {path, 0});
    }
    {
        const uint32_t h = ld32(vcall(me, node::getContentSize, {me}) + 4);
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, 0, h});
        vcall(node, node::setPosition, {node, pos});
        call(g::Vec2_dtor, {pos});
    }
    const uint32_t base = f.alloc(8);
    {
        const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
        const uint32_t b = ld32(me + F_BASE);
        call(g::MedalDataManager_getMedalData, {base, mdm, ld32(b), ld32(b + 4), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0});
    }
    {
        const uint32_t lwf = ld32(node + NODE_LWF);
        const uint32_t key = f.alloc(12);
        const uint32_t text = call(fn::operator_new, {0x10});
        st32(key, 0x11);
        st32(key + 4, 12);
        st32(key + 8, text);
        call(g::memcpy, {text, S_DrowMedal001, 12});
        st8(text + 0xC, 0);
        const uint32_t obj = ld32(base), ctl = ld32(base + 4);
        if (ctl) call(fn::shared_add, {ctl});
        addLoadHandler(lwf, key, VT_BaseLoad, node, obj, ctl, 3);
        strFree(key);
    }
    for (uint32_t i = 0; ld32(me + F_MATERIAL_IDS + 4) != ld32(me + F_MATERIAL_IDS) &&
                         i < uint32_t(int32_t(ld32(me + F_MATERIAL_IDS + 4) - ld32(me + F_MATERIAL_IDS)) >> 2);
         ++i) {
        const uint32_t id = ld32(ld32(me + F_MATERIAL_IDS) + 4 * i);
        const uint32_t lwf = ld32(node + NODE_LWF);
        Frame g2;
        const uint32_t name = stringBuffer(g2, {S_DrowMedalFmt, i + 2});
        const uint32_t key = newStr(g2, name);
        addLoadHandler(lwf, key, VT_MaterialLoad, id, node, 0, 2);
        strFree(key);
    }
    const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
    const uint32_t view = call(g::FlashMovieView_create, {node, 0});
    vcall(layer, node::addChild, {layer, view});
    const uint32_t scene = ld32(functor + 8);
    const uint32_t done = f.alloc(20);
    st32(done + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x14});
    if (heap) {
        st32(heap, VT_MovieDone);
        st32(heap + 4, layer);
        st32(heap + 8, view);
        st32(heap + 0xC, scene);
        st32(heap + 0x10, node);
    }
    st32(done + 16, heap);
    call(g::FunctionVV_assign, {view + VIEW_ON_END, done});
    functionDestroy(done);
    st8(view + VIEW_AUTO_REMOVE, 1);
    release(ld32(base + 4));
}

uint32_t MixedFusionLambdas::cloneMovieDone(uint32_t functor) {
    const uint32_t copy = call(fn::operator_new, {0x14});
    if (copy) {
        st32(copy, VT_MovieDone);
        for (uint32_t k = 4; k < 0x14; k += 4) st32(copy + k, ld32(functor + k));
    }
    return copy;
}

// the movie ended: one animation interval later (DelayTime, CallFunc) the result is shown
void MixedFusionLambdas::movieDone(uint32_t functor) {
    const uint32_t layer = ld32(functor + 4), view = ld32(functor + 8), scene = ld32(functor + 0xC);
    const uint32_t director = call(g::Director_getInstance, {});
    const uint32_t interval = call(g::aeabi_d2f, {ld32(director + 0xA8), ld32(director + 0xAC)});
    const uint32_t node = ld32(functor + 0x10);
    const uint32_t delay = call(g::DelayTime_create, {interval});
    Frame f;
    const uint32_t cb = newFunction(f, VT_ShowResult, {layer, view, scene});
    const uint32_t callFunc = call(g::CallFunc_create, {cb});
    call(g::Node_runAction, {node, call(g::Sequence_create, {delay, callFunc, 0})});
    functionDestroy(cb);
}

void MixedFusionLambdas::cloneShowResult(uint32_t functor, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_ShowResult);
    for (uint32_t k = 4; k < 0x10; k += 4) st32(dst + k, ld32(functor + k));
}

// the movie view leaves the layer, the header refreshes, and the result popup opens for the base medal's new data
void MixedFusionLambdas::showResult(uint32_t functor) {
    const uint32_t layer = ld32(functor + 4);
    vcall(layer, node::removeChild, {layer, ld32(functor + 8), 1});
    const uint32_t scene = ld32(functor + 0xC);
    call(g::SM_updateHeader, {sceneManager()});
    Frame f;
    const uint32_t medal = f.alloc(8), arg = f.alloc(8);
    {
        const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
        const uint32_t b = ld32(scene + F_BASE);
        call(g::MedalDataManager_getMedalData, {medal, mdm, ld32(b), ld32(b + 4), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 0});
    }
    st32(arg, ld32(medal));
    const uint32_t ctl = ld32(medal + 4);
    st32(arg + 4, ctl);
    if (ctl) call(fn::shared_add, {ctl});
    call(g::Mixed_openResultPopup, {scene, arg});
    release(ctl);
    release(ld32(medal + 4));
}

void MixedFusionLambdas::cloneMaterialLoad(uint32_t functor, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_MaterialLoad);
    st32(dst + 8, ld32(functor + 8));
    st32(dst + 4, ld32(functor + 4));
}

// a material movie loaded: the material medal's large image LWF goes into it
void MixedFusionLambdas::materialLoad(uint32_t functor, uint32_t movieRef) {
    const uint32_t movie = ld32(movieRef);
    Frame f;
    const uint32_t medal = f.alloc(8);
    medalData(medal, 0, 0, ld32(functor + 4));
    const uint32_t node = ld32(functor + 8);
    const uint32_t sb = ld32(ld32(medal) + 0x58);
    const uint32_t image = f.alloc(4);
    rw::call(g::SafeBuffer_read, {sb, image, ld32(sb + 0xC) + 4, 4, 4});
    const uint32_t path = stringBuffer(f, {S_MedalLwf, ld32(image)});
    const uint32_t none = newEmptyFunction(f);
    const uint32_t out = f.alloc(8);
    vcall(node, V_attachMovieLwf, {out, node, path, movie, S_Empty, none});
    release(ld32(out + 4));
    functionDestroy(none);
    release(ld32(medal + 4));
}

void MixedFusionLambdas::cloneBaseLoad(uint32_t functor, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_BaseLoad);
    st32(dst + 4, ld32(functor + 4));
    st32(dst + 8, ld32(functor + 8));
    const uint32_t ctl = ld32(functor + 0xC);
    st32(dst + 0xC, ctl);
    if (ctl) call(fn::shared_add, {ctl});
    st32(dst + 4, ld32(functor + 4));
}

// the base medal's movie loaded: its large image LWF goes into it
void MixedFusionLambdas::baseLoad(uint32_t functor, uint32_t movieRef) {
    const uint32_t movie = ld32(movieRef);
    const uint32_t node = ld32(functor + 4);
    Frame f;
    const uint32_t sb = ld32(ld32(functor + 8) + 0x58);
    const uint32_t image = f.alloc(4);
    rw::call(g::SafeBuffer_read, {sb, image, ld32(sb + 0xC) + 4, 4, 4});
    const uint32_t path = stringBuffer(f, {S_MedalLwf, ld32(image)});
    const uint32_t none = newEmptyFunction(f);
    const uint32_t out = f.alloc(8);
    vcall(node, V_attachMovieLwf, {out, node, path, movie, S_Empty, none});
    release(ld32(out + 4));
    functionDestroy(none);
}

RW_FUNCTION(0x6A5808, "SceneMixedMedal::openRareWarnPopup $_18 __func::__clone() (sub_6A5808)", MixedFusionLambdas::cloneRareClose);
RW_FUNCTION(0x6A58D4, "SceneMixedMedal::openRareWarnPopup $_18 __func::destroy_deallocate (sub_6A58D4)", MixedFusionLambdas::destroyRareClose);
RW_FUNCTION(0x6A5900, "SceneMixedMedal::openRareWarnPopup $_18 __func::operator() (sub_6A5900)", MixedFusionLambdas::rareClose);
RW_FUNCTION(0x6A5A90, "SceneMixedMedal::openRareWarnPopup $_36 __func::__clone(p) (sub_6A5A90)", MixedFusionLambdas::cloneRareRemove);
RW_FUNCTION(0x6A5AC2, "SceneMixedMedal::openRareWarnPopup $_36 __func::operator() (sub_6A5AC2)", MixedFusionLambdas::rareRemove);
RW_FUNCTION(0x6A4000, "SceneMixedMedal::runMixed $_39 __func::__clone(p) (sub_6A4000)", MixedFusionLambdas::cloneTwoWords);
RW_FUNCTION(0x6A4030, "SceneMixedMedal::runMixed $_39 __func::operator() (sub_6A4030)", MixedFusionLambdas::enhanceDone);
RW_FUNCTION(0x6A41A0, "SceneMixedMedal::runMixed $_38 __func::__clone(p) (sub_6A41A0)", MixedFusionLambdas::cloneAnimation);
RW_FUNCTION(0x6A41D0, "SceneMixedMedal::runMixed $_38 __func::operator() (sub_6A41D0)", MixedFusionLambdas::animation);
RW_FUNCTION(0x6A47AC, "SceneMixedMedal::runMixed $_38 movie end __func::__clone() (sub_6A47AC)", MixedFusionLambdas::cloneMovieDone);
RW_FUNCTION(0x6A4818, "SceneMixedMedal::runMixed $_38 movie end __func::operator() (sub_6A4818)", MixedFusionLambdas::movieDone);
RW_FUNCTION(0x6A4968, "SceneMixedMedal::runMixed $_38 show result __func::__clone(p) (sub_6A4968)", MixedFusionLambdas::cloneShowResult);
RW_FUNCTION(0x6A499C, "SceneMixedMedal::runMixed $_38 show result __func::operator() (sub_6A499C)", MixedFusionLambdas::showResult);
RW_FUNCTION(0x6A4B14, "SceneMixedMedal::runMixed $_38 material load __func::__clone(p) (sub_6A4B14)", MixedFusionLambdas::cloneMaterialLoad);
RW_FUNCTION(0x6A4B44, "SceneMixedMedal::runMixed $_38 material load __func::operator() (sub_6A4B44)", MixedFusionLambdas::materialLoad);
RW_FUNCTION(0x6A4D84, "SceneMixedMedal::runMixed $_38 base load __func::__clone(p) (sub_6A4D84)", MixedFusionLambdas::cloneBaseLoad);
RW_FUNCTION(0x6A4DE8, "SceneMixedMedal::runMixed $_38 base load __func::operator() (sub_6A4DE8)", MixedFusionLambdas::baseLoad);
