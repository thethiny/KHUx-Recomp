// Medal List > long press > Level Up: the parts of SceneMixedMedal that run on entering and leaving the screen -
// backScene, finalize (the material list stops taking taps and drops its callbacks), its back lambda (back to the
// Equipment screen, the Medal List or Level Up again by the scene's origin) and a touch lambda assign. The fusion
// flow itself (runMixed, the rare-material warning, the result popup, the material sort) needs a fusion in the tour
// and is not part of this file.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t FunctionVTE_swap = 0x4F3699;            // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t MiniMedalScrollView_setEnabledTapItem = 0x4FD6AD;
constexpr uint32_t SM_isOpenedPopup = 0x621519, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t Mixed_finalize = 0x69F56D;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_MEDAL_LIST = 0xFD60BC, GOT_CREATE_MIXED = 0xFD68E8;
constexpr uint32_t GOT_CREATE_DECK_EDIT = 0xFD66B0;
}  // namespace g

constexpr uint32_t F_INOUT = 0x2A4;
constexpr uint32_t F_ROOT = 0x2B8, F_LIST = 0x2BC;   // MiniMedalScrollView
constexpr uint32_t F_FROM = 0x2F8;                   // where the screen was opened from (0 deck, 1 list, 2 level up)
constexpr uint32_t F_SORT_STATE = 0x2F4;             // (+0x272: a flag)
constexpr uint32_t F_SELECTING = 0x38C, F_CAN_BACK = 0x3BC;
constexpr uint32_t LIST_ON_TAP = 0x638, LIST_ON_LONG_PRESS = 0x650, LIST_ON_CHANGE = 0x668;   // std::function
constexpr uint32_t VT_Back = 0xF6554C;               // backScene {this}: sub_69FC70
constexpr uint32_t VT_TouchLambda = 0xF65D08;        // {two words}
constexpr uint32_t S_WinUnder = 0xE03EF1, S_ButtonSort = 0xE03DC6;
constexpr uint32_t SM_HEADER_308 = 0x308;
constexpr uint32_t V_setTouchEnabled = 0x29C;

void changeScene(uint32_t factoryGot) {
    const uint32_t m = sceneManager();
    Frame f;
    const uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(factoryGot)});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 0, 0});
    functionDestroy(layer);
}
// destroy the std::function at `fn` and leave it empty
void clearFunction(uint32_t fn) {
    functionDestroy(fn);
    st32(fn + 0x10, 0);
}
}  // namespace

struct SceneMixedMedal {
    static void backScene(uint32_t self);
    static void finalize(uint32_t self);
    static void backDone(uint32_t functor);
    static void assignTouchLambda(uint32_t dst, uint32_t captures);
};

// Back: an open popup closes first; otherwise finalize and slide out (then the lambda).
void SceneMixedMedal::backScene(uint32_t me) {
    if (!ld8(me + F_CAN_BACK)) return;
    if (call(g::SM_isOpenedPopup, {sceneManager()}) == 1) {
        call(g::SM_closeCurrentPopup, {sceneManager(), 0});
        return;
    }
    st8(me + F_CAN_BACK, 0);
    st8(sceneManager() + SM_HEADER_308, 0);
    call(g::Mixed_finalize, {me});
    Frame f;
    const uint32_t fo = newFunction(f, VT_Back, {me});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

// The material list stops taking taps and drops its three callbacks; the sort button is disabled.
void SceneMixedMedal::finalize(uint32_t me) {
    call(g::MiniMedalScrollView_setEnabledTapItem, {ld32(me + F_LIST), 0});
    clearFunction(ld32(me + F_LIST) + LIST_ON_TAP);
    clearFunction(ld32(me + F_LIST) + LIST_ON_LONG_PRESS);
    clearFunction(ld32(me + F_LIST) + LIST_ON_CHANGE);
    st8(me + F_SELECTING, 0);
    const uint32_t under = findNode(ld32(me + F_ROOT), S_WinUnder, 9);
    const uint32_t sort = findNode(under, S_ButtonSort, 0xB);
    vcall(sort, V_setTouchEnabled, {sort, 0});
    st8(ld32(me + F_SORT_STATE) + 0x272, 0);
}

// backScene's lambda {this}: back to the Medal List (1), Level Up again (2) or the Equipment screen (0).
void SceneMixedMedal::backDone(uint32_t functor) {
    const uint32_t from = ld32(ld32(functor + 4) + F_FROM);
    if (from == 2)
        changeScene(g::GOT_CREATE_MIXED);
    else if (from == 1)
        changeScene(g::GOT_CREATE_MEDAL_LIST);
    else if (from == 0)
        changeScene(g::GOT_CREATE_DECK_EDIT);
}

// std::function<void(Touch*, Event*)>::operator=(lambda&&) for a lambda of two words (stored inline), swapped in
void SceneMixedMedal::assignTouchLambda(uint32_t dst, uint32_t captures) {
    Frame f;
    const uint32_t fo = newFunction(f, VT_TouchLambda, {ld32(captures), ld32(captures + 4)});
    call(g::FunctionVTE_swap, {fo, dst});
    functionDestroy(fo);
}

RW_FUNCTION(0x69F488, "SceneMixedMedal::backScene", SceneMixedMedal::backScene);
RW_FUNCTION(0x69F56C, "SceneMixedMedal::finalize", SceneMixedMedal::finalize);
RW_FUNCTION(0x69FC70, "SceneMixedMedal::backScene lambda __func::operator() (sub_69FC70)", SceneMixedMedal::backDone);
RW_FUNCTION(0x699998, "SceneMixedMedal touch lambda std::function::operator=(&&) (sub_699998)",
            SceneMixedMedal::assignTouchLambda);
