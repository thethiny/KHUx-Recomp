// The fusion result's lambdas: $_28 (the open animation's callback) __clone / destroy_deallocate, sub_6A1AD4 (the
// result lines slide in one after another, the "tap the screen" armature and an invisible full-size button), the
// button's $_41 (close the popup; then, or after the Level Up tutorial, the screen slides out) and the lambdas that
// follow: the popup's removal, the run-out that refreshes the scene manager, the campaign and a new Level Up screen
// at the same list position. $_28's operator() itself (its guilt bonus branch) is not part of this file.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneUtil_createRightInAnimation = 0x45B8BD;   // (node, float offset, float delay, fn)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t WidgetVector_dtor = 0x485A8F, WidgetVector_copy = 0x6A8799;
constexpr uint32_t FunctionVB_assign = 0x490109;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9, APIManager_executeCampaignGet = 0x4D346D;
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;
constexpr uint32_t Mixed_create = 0x65E345;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t FadeTo_create = 0x86B3E1, Node_runAction = 0x894AED;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t memcpy = 0x40E164;
}  // namespace g

constexpr uint32_t VT_Result = 0xF65680;         // $_28 (heap 0x58)
constexpr uint32_t VT_ScreenTap = 0xF657E0;      // $_41 {button, root, layer, shadow, this, at max level} (heap 0x1C)
constexpr uint32_t VT_RunOutDone = 0xF6580C;     // {this}
constexpr uint32_t VT_NewScene = 0xF65838;       // function<Layer*()> {origin, list index}
constexpr uint32_t VT_TutorialDone = 0xF65864;   // {this}
constexpr uint32_t VT_RemovePopup = 0xF65890;    // {layer, shadow, root}
constexpr uint32_t S_TapJson = 0xDFFCF2;         // "cocostudio/publish/Tap_Screen_Animation.ExportJson" (50)
constexpr uint32_t S_TapScreen = 0xDFFB2B, S_Animation1 = 0xDFD0AF, S_CenterButton = 0xDFFB40;
constexpr uint32_t F_INOUT = 0x2A4, F_LIST = 0x2BC, F_FROM = 0x2F8, F_LIST_INDEX = 0x3B8;
constexpr uint32_t LIST_INDEX = 0x544;           // the material list's scroll index
constexpr uint32_t SM_MIX_FLAG = 0x4CC, SM_MIX_CALLBACK = 0x548;
constexpr uint32_t TUTORIAL_LEVEL_UP = 23;
constexpr uint32_t V_getAnimation = 0x298, V_play = 0x68, V_addChildZName = 0xFC;
constexpr uint32_t IMAGEBUTTON_SELECTABLE = 0x272;

inline void addRef(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// a long std::string built in place: operator new(cap - 1), memcpy of the literal, terminator
uint32_t heapString(Frame &f, uint32_t cap1, uint32_t text, uint32_t len) {
    const uint32_t s = f.alloc(12);
    const uint32_t p = call(fn::operator_new, {cap1 - 1});
    st32(s, cap1);
    st32(s + 4, len);
    st32(s + 8, p);
    call(g::memcpy, {p, text, len});
    st8(p + len, 0);
    return s;
}
// a short std::string of up to 10 chars: length byte, memcpy, terminator
uint32_t shortString(Frame &f, uint32_t text, uint32_t len) {
    const uint32_t s = f.alloc(12);
    st8(s, len * 2);
    call(g::memcpy, {s + 1, text, len});
    st8(s + 1 + len, 0);
    return s;
}
}  // namespace

struct MixedResultLambdas {
    static uint32_t cloneResult(uint32_t functor);
    static void destroyResult(uint32_t functor);
    static void showLines(uint32_t ctx);
    static uint32_t cloneScreenTap(uint32_t functor);
    static void screenTap(uint32_t functor, uint32_t refRef, uint32_t typeRef);
    static void runOutDone(uint32_t functor);
    static void cloneNewScene(uint32_t functor, uint32_t dst);
    static uint32_t newScene(uint32_t functor);
    static void tutorialDone(uint32_t functor);
    static void cloneRemovePopup(uint32_t functor, uint32_t dst);
    static void removePopup(uint32_t functor);
};

uint32_t MixedResultLambdas::cloneResult(uint32_t f) {
    const uint32_t c = call(fn::operator_new, {0x58});
    if (!c) return 0;
    st32(c, VT_Result);
    st32(c + 4, ld32(f + 4));
    st32(c + 8, ld32(f + 8));
    call(g::WidgetVector_copy, {c + 0xC, f + 0xC});
    for (uint32_t k = 0x18; k < 0x28; k += 4) st32(c + k, ld32(f + k));
    st32(c + 0x2C, ld32(f + 0x2C));
    st32(c + 0x28, ld32(f + 0x28));
    for (uint32_t k = 0x30; k < 0x44; k += 4) st32(c + k, ld32(f + k));
    st32(c + 0x44, ld32(f + 0x44));
    addRef(ld32(f + 0x44));
    st32(c + 0x48, ld32(f + 0x48));
    st32(c + 0x4C, ld32(f + 0x4C));
    addRef(ld32(f + 0x4C));
    st8(c + 0x54, ld8(f + 0x54));
    st32(c + 0x50, ld32(f + 0x50));
    return c;
}

void MixedResultLambdas::destroyResult(uint32_t f) {
    release(ld32(f + 0x4C));
    release(ld32(f + 0x44));
    call(g::WidgetVector_dtor, {f + 0xC});
    call(fn::operator_delete, {f});
}

// ctx: {area, lines, root, root, layer, shadow, this, at max level}
void MixedResultLambdas::showLines(uint32_t ctx) {
    const uint32_t area = ld32(ctx);
    vcall(area, node::setVisible, {area, 1});
    if (int32_t(ld32(ctx + 8) - ld32(ctx + 4)) >= 1) {
        for (int32_t i = 0; i < int32_t(ld32(ctx + 8) - ld32(ctx + 4)) >> 2; ++i) {
            const uint32_t line = ld32(ld32(ctx + 4) + 4 * i);
            const float delay = float(i) * 0.02f;
            Frame f;
            const uint32_t none = newEmptyFunction(f);
            const uint32_t anim = call(g::SceneUtil_createRightInAnimation, {line, 0, fbits(delay), none});
            call(g::Node_runAction, {line, call(g::EaseSineOut_create, {anim})});
            functionDestroy(none);
        }
    }
    {
        const uint32_t mgr = call(g::ArmatureDataManager_getInstance, {});
        Frame f;
        const uint32_t s = heapString(f, 0x41, S_TapJson, 50);
        call(g::ArmatureDataManager_addArmatureFileInfo, {mgr, s});
        strFree(s);
    }
    const uint32_t root = ld32(ctx + 0x10);
    uint32_t arm;
    {
        Frame f;
        const uint32_t s = heapString(f, 0x21, S_TapScreen, 20);
        arm = call(g::Armature_create, {s});
        strFree(s);
    }
    {
        const float w = ldf(vcall(root, node::getContentSize, {root}));
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, fbits(w * 0.5f), fbits(30.0f)});
        vcall(arm, node::setPosition, {arm, pos});
        call(g::Vec2_dtor, {pos});
    }
    vcall(root, node::addChildZ, {root, arm, 9999});
    {
        const uint32_t anim = vcall(arm, V_getAnimation, {arm});
        Frame f;
        const uint32_t s = shortString(f, S_Animation1, 10);
        vcall(anim, V_play, {anim, s, uint32_t(-1), uint32_t(-1)});
        strFree(s);
    }
    uint32_t button;
    {
        Frame f;
        const uint32_t s = f.alloc(12);
        st8(s, 0);
        st8(s + 1, 0);
        button = call(g::ImageButton_create, {s, 1});
        strFree(s);
    }
    call(g::ImageButton_setTargetView, {button, ld32(ctx + 0x10), fbits(1.0f)});
    {
        const uint32_t r = ld32(ctx + 0x10);
        Frame f;
        const uint32_t name = heapString(f, 0x11, S_CenterButton, 12);
        vcall(r, V_addChildZName, {r, button, 0, name});
        strFree(name);
    }
    Frame f;
    const uint32_t cb = f.alloc(20);
    st32(cb + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x1C});
    if (heap) {
        st32(heap, VT_ScreenTap);
        st32(heap + 4, button);
        for (uint32_t k = 0; k < 5; ++k) st32(heap + 8 + 4 * k, ld32(ctx + 0x14 + 4 * k));
    }
    st32(cb + 16, heap);
    call(g::ImageButton_addTouchEventListener, {button, cb});
    functionDestroy(cb);
}

uint32_t MixedResultLambdas::cloneScreenTap(uint32_t f) {
    const uint32_t c = call(fn::operator_new, {0x1C});
    if (c) {
        st32(c, VT_ScreenTap);
        for (uint32_t k = 4; k < 0x1C; k += 4) st32(c + k, ld32(f + k));
    }
    return c;
}

// $_41 (the screen was tapped): the popup closes (its removal after the close animation), the shadow fades, then
// the screen slides out - or first the Level Up tutorial when the medal reached its maximum level
void MixedResultLambdas::screenTap(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    call(g::AudioPlaySystemSEDecide, {});
    st8(ld32(f + 4) + IMAGEBUTTON_SELECTABLE, 0);
    const uint32_t me = ld32(f + 0x14), root = ld32(f + 8);
    {
        Frame g2;
        const uint32_t done = newFunction(g2, VT_RemovePopup, {ld32(f + 0xC), ld32(f + 0x10), root});
        call(g::Node_runAction, {root, call(g::SceneUtil_createClosePopupAnimation, {root, done})});
        functionDestroy(done);
    }
    const uint32_t shadow = ld32(f + 0x10);
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), 0})});
    Frame g2;
    if (!ld8(f + 0x18)) {
        const uint32_t done = newFunction(g2, VT_RunOutDone, {me});
        call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, done});
        functionDestroy(done);
        return;
    }
    const uint32_t closed = newFunction(g2, VT_TutorialDone, {me});
    const uint32_t popup = g2.alloc(20);
    call(g::SceneUtil_openTutorialPopup, {popup, TUTORIAL_LEVEL_UP, closed, 0, 0, 0, 0});
    functionDestroy(popup);
    functionDestroy(closed);
}

// the screen slid out: the scene manager's mix flag and an empty callback, the campaign API, and a new Level Up
// screen for the same origin and list position
void MixedResultLambdas::runOutDone(uint32_t f) {
    const uint32_t me = ld32(f + 4);
    const uint32_t index = ld32(ld32(me + F_LIST) + LIST_INDEX);
    const uint32_t sm = sceneManager();
    Frame g2;
    {
        const uint32_t none = newEmptyFunction(g2);
        st8(sm + SM_MIX_FLAG, 1);
        call(g::FunctionVB_assign, {sm + SM_MIX_CALLBACK, none});
        functionDestroy(none);
    }
    {
        const uint32_t none = newEmptyFunction(g2);
        call(g::APIManager_executeCampaignGet, {call(g::APIManager_getInstance, {}), none});
        functionDestroy(none);
    }
    const uint32_t from = ld32(me + F_FROM);
    const uint32_t sm2 = sceneManager();
    const uint32_t layer = newFunction(g2, VT_NewScene, {from, index});
    rw::call(g::SM_changeSceneLayer, {sm2, layer, 0, 0, 0});
    functionDestroy(layer);
}

void MixedResultLambdas::cloneNewScene(uint32_t f, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_NewScene);
    st32(dst + 8, ld32(f + 8));
    st32(dst + 4, ld32(f + 4));
}

uint32_t MixedResultLambdas::newScene(uint32_t f) {
    const uint32_t scene = call(g::Mixed_create, {});
    st32(scene + F_FROM, ld32(f + 4));
    const uint32_t index = ld32(f + 8);
    st32(scene + F_LIST_INDEX, index);
    call(g::IScrollProtocol_setIndex, {ld32(scene + F_LIST), index});
    return scene;
}

void MixedResultLambdas::tutorialDone(uint32_t f) {
    const uint32_t me = ld32(f + 4);
    Frame g2;
    const uint32_t done = newFunction(g2, VT_RunOutDone, {me});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, done});
    functionDestroy(done);
}

void MixedResultLambdas::cloneRemovePopup(uint32_t f, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_RemovePopup);
    st32(dst + 0xC, ld32(f + 0xC));
    st32(dst + 8, ld32(f + 8));
    st32(dst + 4, ld32(f + 4));
}

void MixedResultLambdas::removePopup(uint32_t f) {
    const uint32_t layer = ld32(f + 4);
    vcall(layer, node::removeChild, {layer, ld32(f + 8), 1});
    vcall(layer, node::removeChild, {layer, ld32(f + 0xC), 1});
}

RW_FUNCTION(0x6A1298, "SceneMixedMedal::openResultPopup $_28 __func::__clone() (sub_6A1298)", MixedResultLambdas::cloneResult);
RW_FUNCTION(0x6A13EA, "SceneMixedMedal::openResultPopup $_28 __func::destroy_deallocate (sub_6A13EA)", MixedResultLambdas::destroyResult);
RW_FUNCTION(0x6A1AD4, "SceneMixedMedal::openResultPopup show lines (sub_6A1AD4)", MixedResultLambdas::showLines);
RW_FUNCTION(0x6A3670, "SceneMixedMedal::openResultPopup $_41 __func::__clone() (sub_6A3670)", MixedResultLambdas::cloneScreenTap);
RW_FUNCTION(0x6A36E0, "SceneMixedMedal::openResultPopup $_41 __func::operator() (sub_6A36E0)", MixedResultLambdas::screenTap);
RW_FUNCTION(0x6A391C, "SceneMixedMedal::openResultPopup $_40 run out done __func::operator() (sub_6A391C)", MixedResultLambdas::runOutDone);
RW_FUNCTION(0x6A3AC4, "SceneMixedMedal::openResultPopup $_40 new scene __func::__clone(p) (sub_6A3AC4)", MixedResultLambdas::cloneNewScene);
RW_FUNCTION(0x6A3AF2, "SceneMixedMedal::openResultPopup $_40 new scene __func::operator() (sub_6A3AF2)", MixedResultLambdas::newScene);
RW_FUNCTION(0x6A3BB4, "SceneMixedMedal::openResultPopup $_40 tutorial done __func::operator() (sub_6A3BB4)", MixedResultLambdas::tutorialDone);
RW_FUNCTION(0x6A3CB8, "SceneMixedMedal::openResultPopup $_40 remove popup __func::__clone(p) (sub_6A3CB8)", MixedResultLambdas::cloneRemovePopup);
RW_FUNCTION(0x6A3CEA, "SceneMixedMedal::openResultPopup $_40 remove popup __func::operator() (sub_6A3CEA)", MixedResultLambdas::removePopup);
