// SceneDeckEdit's keyblade animations and the lambdas around them:
// - playKeybladeStepUp / playKeybladeEvolve: a shadow and the KeyBlade_StepUp / KeyBlade_Evolution LWF on popup
//   layer 2; the "_root" enterFrame handler fades it out at its end (step up: also at frame 118) and calls `done`;
//   "keyblade" (evolve: also "keyblade_previous") load handlers attach the keyblade's own LWF; then a fade-in.
// - runUpdateKeybladeEvent's tutorial chain (the deck tutorial): after the keyblade update, CompleteTutorialPhase,
//   then the stage list (executeStageGet160310), then the raids (executeRaidGet), then the callback; a failure on
//   the way shows the retry dialog (when asked) and ends in $_38 with false. Each step's closure holds the
//   remaining steps: {bool, bool, function<void(bool)>} groups, the empty lambdas' bytes copied as the original
//   does.
// - small lambdas of onTapMedalInfoButton, updateDeckView and initMedalSelectPanel.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SM_getPopupLayer2 = 0x613E71;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;        // (sm, const function<void()>&, bool, int, int)
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createFadeInAnimation = 0x45BCA1, SceneUtil_createFadeOutAnimation = 0x45BCE5;
constexpr uint32_t SceneUtil_CompleteTutorialPhase = 0x472A75;   // (const function<void()>&, int, int)
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t HandlerMap_insert_unique = 0x530869, HandlerMap_destroy = 0x530811;
constexpr uint32_t LWF_AddMovieEventHandler = 0x808645;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;  // (api, const function<void(bool)>&, bool)
constexpr uint32_t APIManager_executeStageGet160310 = 0x4CADB9, APIManager_executeRaidGet = 0x4CBEC5;
constexpr uint32_t KeybladeEventDone = 0x668B09;           // runUpdateKeybladeEvent $_38 body (lambda, bool)
constexpr uint32_t function_vb_call = 0x474139;
constexpr uint32_t SceneDeckEdit_updateDeckStauts = 0x66707D;
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t MiniMedal_setItemIDs = 0x4FD1FD, MiniMedal_changeShowType = 0x4FD531;
constexpr uint32_t MiniMedal_setDisableMedalID = 0x4FD425;  // (mini, u64 id, bool, bool)
constexpr uint32_t Revolver_changeShowType = 0x52D5B9;
constexpr uint32_t VectorU64_pushSlow = 0x4ECB6D, VectorU64_dtor = 0x468499;
constexpr uint32_t MEMCPY = 0x40E164;
}  // namespace g

constexpr uint32_t S_StepUpLwf = 0xE04184;    // "lwf/memudemo/KeyBlade_StepUp/KeyBlade_StepUp.lwf"
constexpr uint32_t S_EvolveLwf = 0xE041B5;    // "lwf/memudemo/KeyBlade_Evolution/KeyBlade_Evolution.lwf"
constexpr uint32_t S_enterFrame = 0xDFE0F3;
constexpr uint32_t S_root = 0xDFE0ED, S_keyblade = 0xE01578, S_keyblade_previous = 0xE041EC;
constexpr uint32_t S_KeybladeLwf = 0xE0444D;  // "lwf/avatar/keyblade/keyblade_%04d_%02d/keyblade_%04d_%02d.lwf"

// __func vtables (+8)
constexpr uint32_t VT_StepUpEnter = 0xF63418;   // $_42 {lwf node, layer, shadow, function<void()>} (0x30)
constexpr uint32_t VT_StepUpLoad = 0xF633EC;    // $_43 {shared_ptr<UserKeyBladeData>, lwf node}
constexpr uint32_t VT_StepUpShown = 0xF633C0;   // $_44 {lwf node}: the fade-in's end
constexpr uint32_t VT_StepUpFade = 0xF63444;    // $_42's fade-out end {layer, shadow, lwf node, function<void()>}
constexpr uint32_t VT_EvolveEnter = 0xF63368, VT_EvolvePrevLoad = 0xF6333C, VT_EvolveLoad = 0xF63310;
constexpr uint32_t VT_EvolveShown = 0xF632E4, VT_EvolveFade = 0xF63394;
constexpr uint32_t VT_Tut1 = 0xF6349C;   // keyblade updated: CompleteTutorialPhase next (0x80)
constexpr uint32_t VT_Tut2 = 0xF634C8;   // tutorial phase done: the stage list next (0x60)
constexpr uint32_t VT_Tut3 = 0xF634F4;   // stage list fetched: the raids next (0x58)
constexpr uint32_t VT_Tut4 = 0xF63520;   // raids fetched: the callback (0x30)
constexpr uint32_t VT_Tut1Retry = 0xF635A4, VT_Tut3Retry = 0xF63578, VT_Tut4Retry = 0xF6354C;
constexpr uint32_t VT_MedalInfoBack = 0xF635FC;   // onTapMedalInfoButton's changeSceneLayer lambda

constexpr uint32_t LWFNODE_LWF = 0x3E0;
constexpr uint32_t MOVIE_TOTAL_FRAMES = 0x98, MOVIE_CURRENT_FRAME = 0x9C;
constexpr uint32_t V_ATTACH_LWF = 0x2F0;     // AudioLWFNode: (sret shared_ptr, node, path, movie, name, function)
constexpr uint32_t V_SET_OPACITY = 0x214;
constexpr uint32_t UKB_MASTER = 0x20, KB_LWF_ID = 0xCC;
// SceneDeckEdit
constexpr uint32_t DE_DECK_MEDALS = 0x29C, DE_REVOLVER = 0x380, DE_MINI = 0x390, V_GET_SORT_FILTER = 0x308;
constexpr uint32_t REVOLVER_EXCLUDED = 0x2C0;
constexpr uint32_t SHOW_TYPE_TABLE = 0xE6E270, SHOW_TYPE_KEYS = 0x29F, SHOW_TYPE_DEFAULT = 4;

uint32_t api() { return call(fn::APIManager_getInstance, {}); }

// a std::string at `s` holding a short literal, built in place
void shortKey(uint32_t s, const char *text) {
    const uint32_t n = uint32_t(std::strlen(text));
    st8(s, uint8_t(n * 2));
    for (uint32_t k = 0; k < n; k++) st8(s + 1 + k, uint8_t(text[k]));
    st8(s + 1 + n, 0);
}
// lwf->AddMovieEventHandler(name, {pair}): pair = <string, function<void(Movie*)>> (key +0, function +16)
void addHandler(uint32_t lwf, uint32_t name, uint32_t pair) {
    Frame f;
    const uint32_t map = f.alloc(12);
    st32(map + 4, 0);
    st32(map + 8, 0);
    st32(map, map + 4);
    call(g::HandlerMap_insert_unique, {map, map + 4, pair});
    call(g::LWF_AddMovieEventHandler, {lwf, name, map});
    call(g::HandlerMap_destroy, {map, ld32(map + 4)});
}
// the std::function destructor as inlined, keeping what the last call returned (0 when empty)
uint32_t destroyRet(uint32_t fo) {
    const uint32_t p = ld32(fo + 16);
    if (p == fo) return vcall(p, 0x10, {p});
    if (p) return vcall(p, 0x14, {p});
    return 0;
}

// "_root" enterFrame: {node, layer, shadow, done} moved into a heap __func (0x30)
void addEnterFrame(uint32_t lwf, uint32_t node, uint32_t layer, uint32_t shadow, uint32_t done, uint32_t vt) {
    Frame f;
    const uint32_t name = newStr(f, S_root, 5);
    const uint32_t cb = f.alloc(20);               // the lambda's captured callback
    call(fn::function_vv_copy, {cb, done});
    const uint32_t pair = f.alloc(40);
    st8(pair, 20);                                 // "enterFrame" (short), copied with memcpy
    call(g::MEMCPY, {pair + 1, S_enterFrame, 10});
    st8(pair + 11, 0);
    const uint32_t moved = f.alloc(20);
    call(fn::function_vv_move, {moved, cb});
    const uint32_t fo = pair + 16;
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x30});
    if (h) {
        st32(h, vt);
        st32(h + 8, node);
        st32(h + 0xC, layer);
        st32(h + 0x10, shadow);
        call(fn::function_vv_move, {h + 0x18, moved});
    }
    st32(fo + 16, h);
    functionDestroy(moved);
    addHandler(lwf, name, pair);
    functionDestroy(fo);
    strFree(pair);
    functionDestroy(cb);
    strFree(name);
}
// `name` load: {keyblade (shared_ptr, a reference taken), node} stored inline
void addLoad(uint32_t lwf, uint32_t nameLit, uint32_t nameLen, uint32_t keyblade, uint32_t node, uint32_t vt) {
    Frame f;
    const uint32_t name = newStr(f, nameLit, nameLen);
    const uint32_t p = ld32(keyblade), ctl = ld32(keyblade + 4);
    if (ctl) call(fn::shared_add, {ctl});
    const uint32_t pair = f.alloc(40);
    shortKey(pair, "load");
    const uint32_t fo = pair + 16;
    st32(fo, vt);
    st32(fo + 4, p);
    st32(fo + 8, ctl);
    st32(fo + 0xC, node);
    st32(fo + 16, fo);
    addHandler(lwf, name, pair);
    functionDestroy(fo);
    strFree(pair);
    strFree(name);
}
// the shadow (transparent) and the LWF centred on it, both on popup layer 2
uint32_t openAnimation(uint32_t path, uint32_t &layer, uint32_t &shadow) {
    layer = call(g::SM_getPopupLayer2, {sceneManager()});
    shadow = call(g::SceneUtil_createShadowSprite, {});
    vcall(shadow, V_SET_OPACITY, {shadow, 0});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t node = call(g::AudioLWFNode_create, {path, 0});
    const uint32_t setPos = ld32(ld32(node) + node::setPosition);
    const float w = ldf(vcall(shadow, node::getContentSize, {shadow}));
    const float h = ldf(vcall(shadow, node::getContentSize, {shadow}) + 4);
    {
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, fbits(w * 0.5f), fbits(h * 0.5f)});
        call(setPos, {node, pos});
        call(g::Vec2_dtor, {pos});
    }
    vcall(layer, node::addChild, {layer, node});
    return node;
}
void fadeIn(uint32_t node, uint32_t vt) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {node});
    call(fn::Node_runAction, {node, call(g::SceneUtil_createFadeInAnimation, {node, fo})});
    functionDestroy(fo);
}

uint32_t playKeybladeStepUp(uint32_t, uint32_t keyblade, uint32_t done) {
    uint32_t layer, shadow;
    const uint32_t node = openAnimation(S_StepUpLwf, layer, shadow);
    const uint32_t lwf = ld32(node + LWFNODE_LWF);
    addEnterFrame(lwf, node, layer, shadow, done, VT_StepUpEnter);
    addLoad(ld32(node + LWFNODE_LWF), S_keyblade, 8, keyblade, node, VT_StepUpLoad);
    fadeIn(node, VT_StepUpShown);
    return 0;
}
uint32_t playKeybladeEvolve(uint32_t, uint32_t previous, uint32_t keyblade, uint32_t done) {
    uint32_t layer, shadow;
    const uint32_t node = openAnimation(S_EvolveLwf, layer, shadow);
    const uint32_t lwf = ld32(node + LWFNODE_LWF);
    addEnterFrame(lwf, node, layer, shadow, done, VT_EvolveEnter);
    addLoad(ld32(node + LWFNODE_LWF), S_keyblade_previous, 17, previous, node, VT_EvolvePrevLoad);
    addLoad(ld32(node + LWFNODE_LWF), S_keyblade, 8, keyblade, node, VT_EvolveLoad);
    fadeIn(node, VT_EvolveShown);
    return 0;
}

// load handler {keyblade, ctl, node}(Movie*): the keyblade's LWF (lwf/avatar/keyblade/keyblade_XXXX_YY) into the
// movie instance `name`
uint32_t attachKeyblade(uint32_t self, uint32_t moviePtr, uint32_t name) {
    const uint32_t movie = ld32(moviePtr);
    Frame f;
    const uint32_t id = f.alloc(4);
    const uint32_t sb = ld32(ld32(self + 4) + UKB_MASTER);
    call(g::SafeBuffer_read, {sb, id, ld32(sb + 0xC) + KB_LWF_ID, 4, KB_LWF_ID});
    const int32_t v = int32_t(ld32(id));
    const uint32_t node = ld32(self + 0xC);
    const uint32_t attach = ld32(ld32(node) + V_ATTACH_LWF);
    const uint32_t buf = stringBuffer(f, {S_KeybladeLwf, uint32_t(v / 100), uint32_t(v % 100), uint32_t(v / 100),
                                          uint32_t(v % 100)});
    const uint32_t ret = f.alloc(8);
    const uint32_t empty = newEmptyFunction(f);
    call(attach, {ret, node, buf, movie, name, empty});
    if (ld32(ret + 4)) call(fn::shared_release, {ld32(ret + 4)});
    functionDestroy(empty);
    return 0;
}
uint32_t stepUpLoad(uint32_t self, uint32_t movie) { return attachKeyblade(self, movie, S_keyblade); }
uint32_t evolveLoad(uint32_t self, uint32_t movie) { return attachKeyblade(self, movie, S_keyblade); }
uint32_t evolvePrevLoad(uint32_t self, uint32_t movie) { return attachKeyblade(self, movie, S_keyblade_previous); }

// enterFrame {node, layer, shadow, done}: at the end, fade the LWF out ({layer, shadow, node, done} at its end)
void fadeOut(uint32_t self, uint32_t vt) {
    const uint32_t node = ld32(self + 8), layer = ld32(self + 0xC), shadow = ld32(self + 0x10);
    Frame f;
    const uint32_t cb = f.alloc(20);
    call(fn::function_vv_copy, {cb, self + 0x18});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x30});
    if (h) {
        st32(h, vt);
        st32(h + 8, layer);
        st32(h + 0xC, shadow);
        st32(h + 0x10, node);
        call(fn::function_vv_move, {h + 0x18, cb});
    }
    st32(fo + 16, h);
    call(fn::Node_runAction, {node, call(g::SceneUtil_createFadeOutAnimation, {node, fo})});
    functionDestroy(fo);
    functionDestroy(cb);
}
uint32_t stepUpEnterFrame(uint32_t self, uint32_t moviePtr) {
    const uint32_t movie = ld32(moviePtr);
    const uint32_t cur = ld32(movie + MOVIE_CURRENT_FRAME);
    if (cur == 118 || cur == ld32(movie + MOVIE_TOTAL_FRAMES)) fadeOut(self, VT_StepUpFade);
    return 0;
}
uint32_t evolveEnterFrame(uint32_t self, uint32_t moviePtr) {
    const uint32_t movie = ld32(moviePtr);
    if (ld32(movie + MOVIE_CURRENT_FRAME) == ld32(movie + MOVIE_TOTAL_FRAMES)) fadeOut(self, VT_EvolveFade);
    return 0;
}

// ---------------------------------------------------------------- the deck tutorial's update chain
// A group: byte (empty lambda) at +0, byte at +8, function<void(bool)> at +0x10; groups 0x28 apart.
void copyGroup(uint32_t dst, uint32_t src) {
    st8(dst, ld8(src));
    st8(dst + 8, ld8(src + 8));
    call(fn::function_vb_copy, {dst + 0x10, src + 0x10});
}
void moveGroup(uint32_t dst, uint32_t src) {
    st8(dst, ld8(src));
    st8(dst + 8, ld8(src + 8));
    call(fn::function_vb_move, {dst + 0x10, src + 0x10});
}
// closures: vtable, then `groups` groups from +8 (and `tail` = the byte after them)
uint32_t chainClone(uint32_t self, uint32_t vt, uint32_t size, uint32_t groups, bool tail) {
    const uint32_t c = call(fn::operator_new, {size});
    if (c) {
        st32(c, vt);
        for (uint32_t k = 0; k < groups; k++) copyGroup(c + 8 + 0x28 * k, self + 8 + 0x28 * k);
        if (tail) st8(c + 8 + 0x28 * groups, ld8(self + 8 + 0x28 * groups));
    }
    return c;
}
void chainCloneInto(uint32_t self, uint32_t dst, uint32_t vt, uint32_t groups, bool tail) {
    st32(dst, vt);
    for (uint32_t k = 0; k < groups; k++) copyGroup(dst + 8 + 0x28 * k, self + 8 + 0x28 * k);
    if (tail) st8(dst + 8 + 0x28 * groups, ld8(self + 8 + 0x28 * groups));
}
uint32_t chainDestroy(uint32_t self, uint32_t groups) {
    for (uint32_t k = groups - 1; k > 0; k--) functionDestroy(self + 0x18 + 0x28 * k);
    return destroyRet(self + 0x18);
}
// the retry dialog of a failed request (when the closure asks for it and the API manager allows it)
void retryDialog(uint32_t self, uint32_t vt) {
    if (!ld8(self + 8)) return;
    if (!ld8(api() + 0x14)) return;
    const uint32_t a = api();
    Frame f;
    const uint32_t fo = newFunction(f, vt, {});
    call(g::APIManager_openErrorDialog, {a, fo, 1});
    functionDestroy(fo);
}
void failed(uint32_t self, uint32_t vt) {
    retryDialog(self, vt);
    call(g::KeybladeEventDone, {self + 0x10, 0});
}

// Tut1 (0x80, 3 groups): keyblade update answered
uint32_t tut1Dtor(uint32_t self) {
    st32(self, VT_Tut1);
    chainDestroy(self, 3);
    return self;
}
uint32_t tut1Delete(uint32_t self) {
    st32(self, VT_Tut1);
    chainDestroy(self, 3);
    return call(fn::operator_delete, {self});
}
uint32_t tut1Clone(uint32_t self) { return chainClone(self, VT_Tut1, 0x80, 3, false); }
uint32_t tut1CloneInto(uint32_t self, uint32_t dst) {
    if (!dst) return self;
    chainCloneInto(self, dst, VT_Tut1, 3, false);
    return ld8(self + 8);
}
uint32_t tut1Destroy(uint32_t self) { return chainDestroy(self, 3); }
uint32_t tut1DestroyDeallocate(uint32_t self) {
    chainDestroy(self, 3);
    return call(fn::operator_delete, {self});
}
uint32_t tut1Call(uint32_t self, uint32_t okp) {
    const uint32_t ok = ld8(okp);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        failed(self, VT_Tut1Retry);
        return 0;
    }
    Frame f;
    const uint32_t a = f.alloc(0x58), b = f.alloc(0x58);   // the two remaining groups and a flag (true)
    copyGroup(a, self + 0x30);
    copyGroup(a + 0x28, self + 0x58);
    st8(a + 0x50, 1);
    for (uint32_t k = 0; k < 2; k++) {
        st8(b + 0x28 * k, ld8(a + 0x28 * k));
        st8(b + 0x28 * k + 8, ld8(a + 0x28 * k + 8));
        functionCopy(b + 0x28 * k + 0x10, a + 0x28 * k + 0x10);
    }
    st8(b + 0x50, ld8(a + 0x50));
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x60});
    if (h) {
        st32(h, VT_Tut2);
        moveGroup(h + 8, b);
        moveGroup(h + 0x30, b + 0x28);
        st8(h + 0x58, ld8(b + 0x50));
    }
    st32(fo + 16, h);
    call(g::SceneUtil_CompleteTutorialPhase, {fo, 0, 0});
    functionDestroy(fo);
    functionDestroy(b + 0x38);
    functionDestroy(b + 0x10);
    functionDestroy(a + 0x38);
    functionDestroy(a + 0x10);
    return 0;
}

// Tut2 (0x60, 2 groups + flag): the tutorial phase completed
uint32_t tut2Dtor(uint32_t self) {
    st32(self, VT_Tut2);
    chainDestroy(self, 2);
    return self;
}
uint32_t tut2Delete(uint32_t self) {
    st32(self, VT_Tut2);
    chainDestroy(self, 2);
    return call(fn::operator_delete, {self});
}
uint32_t tut2Clone(uint32_t self) { return chainClone(self, VT_Tut2, 0x60, 2, true); }
uint32_t tut2CloneInto(uint32_t self, uint32_t dst) {
    if (!dst) return self;
    chainCloneInto(self, dst, VT_Tut2, 2, true);
    return 0x58;
}
uint32_t tut2Destroy(uint32_t self) { return chainDestroy(self, 2); }
uint32_t tut2DestroyDeallocate(uint32_t self) {
    chainDestroy(self, 2);
    return call(fn::operator_delete, {self});
}
uint32_t tut2Call(uint32_t self) {
    const uint32_t a0 = api();
    Frame f;
    const uint32_t a = f.alloc(0x50);
    copyGroup(a, self + 8);
    copyGroup(a + 0x28, self + 0x30);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x58});
    if (h) {
        st32(h, VT_Tut3);
        moveGroup(h + 8, a);
        moveGroup(h + 0x30, a + 0x28);
    }
    st32(fo + 16, h);
    call(g::APIManager_executeStageGet160310, {a0, fo});
    functionDestroy(fo);
    functionDestroy(a + 0x38);
    functionDestroy(a + 0x10);
    return 0;
}

// Tut3 (0x58, 2 groups): the stage list fetched
uint32_t tut3Dtor(uint32_t self) {
    st32(self, VT_Tut3);
    chainDestroy(self, 2);
    return self;
}
uint32_t tut3Delete(uint32_t self) {
    st32(self, VT_Tut3);
    chainDestroy(self, 2);
    return call(fn::operator_delete, {self});
}
uint32_t tut3Clone(uint32_t self) { return chainClone(self, VT_Tut3, 0x58, 2, false); }
uint32_t tut3CloneInto(uint32_t self, uint32_t dst) {
    if (!dst) return self;
    chainCloneInto(self, dst, VT_Tut3, 2, false);
    return ld8(self + 8);
}
uint32_t tut3Destroy(uint32_t self) { return chainDestroy(self, 2); }
uint32_t tut3DestroyDeallocate(uint32_t self) {
    chainDestroy(self, 2);
    return call(fn::operator_delete, {self});
}
uint32_t tut3Call(uint32_t self, uint32_t okp) {
    const uint32_t ok = ld8(okp);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        failed(self, VT_Tut3Retry);
        return 0;
    }
    const uint32_t a0 = api();
    Frame f;
    const uint32_t a = f.alloc(0x28);
    copyGroup(a, self + 0x30);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x30});
    if (h) {
        st32(h, VT_Tut4);
        moveGroup(h + 8, a);
    }
    st32(fo + 16, h);
    call(g::APIManager_executeRaidGet, {a0, fo});
    functionDestroy(fo);
    functionDestroy(a + 0x10);
    return 0;
}

// Tut4 (one group): the raids fetched, the callback gets true
uint32_t tut4Call(uint32_t self, uint32_t okp) {
    const uint32_t ok = ld8(okp);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        failed(self, VT_Tut4Retry);
        return 0;
    }
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    call(g::function_vb_call, {self + 0x18, 1});
    return 0;
}

// onTapMedalInfoButton $_ (): back to the deck edit layer
uint32_t medalInfoBack(uint32_t) {
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = newFunction(f, VT_MedalInfoBack, {});
    call(g::SM_changeSceneLayer, {sm, fo, 1, 0, 0});
    functionDestroy(fo);
    return 0;
}

// updateDeckView $_ {revolver, scene}: updateDeckStauts(the revolver's index, the keyblade, the deck)
uint32_t deckViewChanged(uint32_t self) {
    const uint32_t scene = ld32(self + 8);
    const uint32_t index = ld32(ld32(self + 4) + 0x2E4);
    Frame f;
    const uint32_t kb = f.alloc(8);
    const uint32_t ctl = ld32(scene + 0x2BC);
    st32(kb, ld32(scene + 0x2B8));
    st32(kb + 4, ctl);
    if (ctl) call(fn::shared_add, {ctl});
    call(g::SceneDeckEdit_updateDeckStauts, {ld32(self + 8), index, kb, ld32(self + 8) + 0x2C0});
    if (ctl) call(fn::shared_release, {ctl});
    return 0;
}

// initMedalSelectPanel $_ {scene}: the medal list again: the deck's medal ids, the first page, the sort's show
// type, and the revolver's excluded medals disabled
uint32_t medalPanelReset(uint32_t self) {
    const uint32_t me = ld32(self + 4);
    Frame f;
    const uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    if (ld32(me + DE_DECK_MEDALS + 4) != ld32(me + DE_DECK_MEDALS)) {
        uint32_t i = 0;
        do {
            const uint32_t m = ld32(ld32(me + DE_DECK_MEDALS) + 8 * i);
            const uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(g::VectorU64_pushSlow, {ids, m});
            } else {
                if (end) {
                    st32(end, ld32(m));
                    st32(end + 4, ld32(m + 4));
                }
                st32(ids + 4, (end ? ld32(ids + 4) : 0) + 8);
            }
            i++;
        } while (i < (ld32(me + DE_DECK_MEDALS + 4) - ld32(me + DE_DECK_MEDALS)) >> 3);
    }
    call(g::IScrollProtocol_setIndex, {ld32(me + DE_MINI), 0});
    call(g::MiniMedal_setItemIDs, {ld32(me + DE_MINI), ids});
    uint32_t showType = SHOW_TYPE_DEFAULT;
    {
        const uint32_t state = f.alloc(8);
        vcall(me, V_GET_SORT_FILTER, {state, me});
        const uint32_t k = uint32_t(ld8(ld32(state) + 1)) - 1;
        if (k <= 9 && ((SHOW_TYPE_KEYS >> (k & 0xFFFF)) & 1)) showType = ld32(SHOW_TYPE_TABLE + 4 * k);
        if (ld32(state + 4)) call(fn::shared_release, {ld32(state + 4)});
    }
    call(g::MiniMedal_changeShowType, {ld32(me + DE_MINI), showType});
    call(g::Revolver_changeShowType, {ld32(me + DE_REVOLVER), showType});
    const uint32_t excluded = ld32(me + DE_REVOLVER) + REVOLVER_EXCLUDED;
    if (ld32(excluded + 4) != ld32(excluded)) {
        uint32_t j = 0;
        do {
            const uint32_t m = ld32(ld32(excluded) + 8 * j);
            Frame g2;
            const uint32_t args = g2.alloc(8);
            st32(args, 1);
            st32(args + 4, 1);
            rw::call_exact(g::MiniMedal_setDisableMedalID, {ld32(me + DE_MINI), 1, ld32(m), ld32(m + 4)});
            j++;
        } while (j < (ld32(excluded + 4) - ld32(excluded)) >> 3);
    }
    call(g::VectorU64_dtor, {ids});
    return 0;
}
}  // namespace

RW_FUNCTION(0x668BE0, "SceneDeckEdit::playKeybladeStepUp", playKeybladeStepUp);
RW_FUNCTION(0x669114, "SceneDeckEdit::playKeybladeEvolve", playKeybladeEvolve);
RW_FUNCTION(0x675BD4, "SceneDeckEdit::playKeybladeStepUp $_43 keyblade load (sub_675BD4)", stepUpLoad);
RW_FUNCTION(0x675E98, "SceneDeckEdit::playKeybladeStepUp $_42 enterFrame (sub_675E98)", stepUpEnterFrame);
RW_FUNCTION(0x6751D0, "SceneDeckEdit::playKeybladeEvolve keyblade load (sub_6751D0)", evolveLoad);
RW_FUNCTION(0x675430, "SceneDeckEdit::playKeybladeEvolve keyblade_previous load (sub_675430)", evolvePrevLoad);
RW_FUNCTION(0x6756F4, "SceneDeckEdit::playKeybladeEvolve $_45 enterFrame (sub_6756F4)", evolveEnterFrame);
RW_FUNCTION(0x676380, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 1 __func::~__func", tut1Dtor);
RW_FUNCTION(0x6763F4, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 1 __func::~__func (deleting)", tut1Delete);
RW_FUNCTION(0x67646C, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 1 __func::__clone()", tut1Clone);
RW_FUNCTION(0x6765C0, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 1 __func::__clone(p)", tut1CloneInto);
RW_FUNCTION(0x6766F0, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 1 __func::destroy", tut1Destroy);
RW_FUNCTION(0x676748, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 1 __func::destroy_deallocate", tut1DestroyDeallocate);
RW_FUNCTION(0x6767AC, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 1 (sub_6767AC)", tut1Call);
RW_FUNCTION(0x676BFC, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 2 __func::~__func", tut2Dtor);
RW_FUNCTION(0x676C54, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 2 __func::~__func (deleting)", tut2Delete);
RW_FUNCTION(0x676CB0, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 2 __func::__clone()", tut2Clone);
RW_FUNCTION(0x676D98, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 2 __func::__clone(p)", tut2CloneInto);
RW_FUNCTION(0x676E5C, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 2 __func::destroy", tut2Destroy);
RW_FUNCTION(0x676E98, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 2 __func::destroy_deallocate", tut2DestroyDeallocate);
RW_FUNCTION(0x676EE0, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 2 (sub_676EE0)", tut2Call);
RW_FUNCTION(0x677124, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 3 __func::~__func", tut3Dtor);
RW_FUNCTION(0x67717C, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 3 __func::~__func (deleting)", tut3Delete);
RW_FUNCTION(0x6771D8, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 3 __func::__clone()", tut3Clone);
RW_FUNCTION(0x6772BC, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 3 __func::__clone(p)", tut3CloneInto);
RW_FUNCTION(0x67737C, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 3 __func::destroy", tut3Destroy);
RW_FUNCTION(0x6773B8, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 3 __func::destroy_deallocate", tut3DestroyDeallocate);
RW_FUNCTION(0x677400, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 3 (sub_677400)", tut3Call);
RW_FUNCTION(0x677750, "SceneDeckEdit::runUpdateKeybladeEvent tutorial step 4 (sub_677750)", tut4Call);
RW_FUNCTION(0x677A84, "SceneDeckEdit::onTapMedalInfoButton back lambda (sub_677A84)", medalInfoBack);
RW_FUNCTION(0x677E30, "SceneDeckEdit::updateDeckView revolver changed lambda (sub_677E30)", deckViewChanged);
RW_FUNCTION(0x678CC0, "SceneDeckEdit::initMedalSelectPanel medal list reset lambda (sub_678CC0)", medalPanelReset);
