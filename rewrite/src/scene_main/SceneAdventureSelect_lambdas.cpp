// SceneAdventureSelect: the lambda bodies (operator() of the $_N lambdas stored in std::function objects, and two
// captureless helper lambdas called directly). `functor` is the __func object: its vtable at +0, then the captures
// (the scene pointer at +4 for most of them). The std::function slots (clone, destroy, ...) are in
// SceneAdventureSelect_thunks.cpp.
#include "SceneAdventureSelect.hpp"

using namespace scene;

namespace {

// SceneAdventureSelect members called from the lambdas (direct calls in the original)
constexpr uint32_t SAS_changeAdventure = 0x71FC3D;          // (scene, AdventureType, bool)
constexpr uint32_t SAS_changeOutAnimation = 0x71F00D;       // (scene, std::function<void()>)
constexpr uint32_t SAS_openPopupStartDeckEdit = 0x727129;   // (scene)
constexpr uint32_t SAS_openTutorial33 = 0x727E95;           // (scene, std::function<void()>)

constexpr uint32_t BuyMagicStoneDialog_open = 0x588CAD;     // (const function<void()>&, bool)
constexpr uint32_t FunctionVB_assign = 0x490109;            // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;   // (mgr, function<Layer*()>*, bool, bool, FadeType)
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;    // (widget, std::function<void(Ref*, TouchEventType)>)
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;

// __func<Layer*(*)(), ...> holding createSceneLayer<SceneMyPage> (the My Page factory)
constexpr uint32_t VT_LayerFactory = 0xF7E130;
constexpr uint32_t CreateSceneLayer_MyPage = 0x46FDBD;

// lambdas created by the bodies below (__func vtables, the value stored at +0)
constexpr uint32_t VT_raidBossRewardPopup_55_inner = 0xF6C090;   // [this, done] () { ... }   40 bytes
constexpr uint32_t VT_initColosseum_35_inner = 0xF6D29C;         // [done] (Ref*, TouchEventType)  32 bytes
constexpr uint32_t VT_initEvent_30_inner = 0xF6D2C8;             // [done] (Ref*, TouchEventType)  32 bytes

// SceneManager fields
constexpr uint32_t SM_NEXT_STAGE = 0x2C8;          // the stage / event picked for the deck screen
constexpr uint32_t SM_NEXT_D8 = 0x2D8;
constexpr uint32_t SM_NEXT_DC = 0x2DC;
constexpr uint32_t SM_LOADING_BLOCKED = 0x4CC;     // byte
constexpr uint32_t SM_ON_FINISH_LOADING = 0x548;   // std::function<void(bool)>

constexpr uint32_t SCENE_STAGE_ID = 0x298;         // SceneAdventureSelect::field_298 (the selected stage)
constexpr uint32_t SCENE_FIELD_2C8 = 0x2C8;
constexpr uint32_t SCENE_FLAG_324 = 0x324;
constexpr uint32_t SCENE_FLAG_3FC = 0x3FC;
constexpr uint32_t SCENE_FLAG_3FD = 0x3FD;

uint32_t sceneOf(uint32_t functor) { return ld32(functor + 4); }

// std::function<void()>::operator() when the captured function at `fn` holds a callable
void callIfSet(uint32_t fn) {
    if (ld32(fn + 16)) call(fn::function_vv_call, {fn});
}

// widget->addTouchEventListener(lambda{copy of done}) with the lambda on the heap (32 bytes: vtable, pad,
// std::function<void()> at +8)
void addTouchWithCallback(uint32_t widget, uint32_t done, uint32_t vtable) {
    Frame f;
    uint32_t copy = f.alloc(24);
    uint32_t listener = f.alloc(24);
    call(fn::function_vv_copy, {copy, done});
    uint32_t heap = heapFunction(listener, vtable, 0x20, 8, fn::function_vv_move, copy);
    (void)heap;
    call(Widget_addTouchEventListener, {widget, listener});
    functionDestroy(listener);
    functionDestroy(copy);
}

}  // namespace

struct SceneAdventureSelectLambdas {
    // runOutAnimationMain $_68: run the captured `done` (if any) when the out animation ends
    static void runOutAnimationMainDone(uint32_t functor) { callIfSet(functor + 8); }

    // backScene $_67: back to My Page - clear the loading callback, run the My Page API chain, change the scene
    static void backToMyPage(uint32_t functor) {
        (void)functor;
        Frame f;
        uint32_t mgr = sceneManager();
        uint32_t empty = newEmptyFunction(f);
        st8(mgr + SM_LOADING_BLOCKED, 1);
        call(FunctionVB_assign, {mgr + SM_ON_FINISH_LOADING, empty});
        functionDestroy(empty);
        uint32_t none = newEmptyFunction(f);
        call(SceneUtil_callPrepareMypageAPIs, {none});
        functionDestroy(none);
        uint32_t m = sceneManager();
        uint32_t factory = newFunction(f, VT_LayerFactory, {CreateSceneLayer_MyPage});
        call(SceneManager_changeSceneLayer, {m, factory, 1, 1, 1});
        functionDestroy(factory);
    }

    // backScene $_66 / $_65: back to the adventure select (type 7) or story top (type 1), flag_3FC set
    static void backToAdventureSelect(uint32_t functor) {
        uint32_t scene = sceneOf(functor);
        call(SAS_changeAdventure, {scene, 7, 0});
        st8(scene + SCENE_FLAG_3FC, 1);
    }
    static void backToStory(uint32_t functor) {
        uint32_t scene = sceneOf(functor);
        call(SAS_changeAdventure, {scene, 1, 0});
        st8(scene + SCENE_FLAG_3FC, 1);
    }

    // checkAp $_64 (bool charged, bool buy): open the magic stone shop when the player chose to buy
    static void checkApResult(uint32_t functor, const uint8_t *charged, const uint8_t *buy) {
        (void)functor;
        (void)charged;
        if (!*buy) return;
        Frame f;
        uint32_t none = newEmptyFunction(f);
        call(BuyMagicStoneDialog_open, {none, 0});
        functionDestroy(none);
    }

    // checkApColosseum $_63: hand the picked colosseum stage to SceneManager, then open the deck screen
    // captures: +4, +8, +0xC (SceneManager values), +0x14 the scene
    static void colosseumApOk(uint32_t functor) {
        uint32_t scene = ld32(functor + 0x14);
        uint32_t stage = ld32(functor + 0xC);
        st32(sceneManager() + SM_NEXT_STAGE, stage);
        uint32_t d8 = ld32(functor + 4);
        st32(sceneManager() + SM_NEXT_D8, d8);
        uint32_t dc = ld32(functor + 8);
        st32(sceneManager() + SM_NEXT_DC, dc);
        call(SAS_openPopupStartDeckEdit, {scene});
    }

    // checkApEvent $_62 / checkApStory $_61: the selected stage goes to SceneManager, then the deck screen
    static void stageApOk(uint32_t functor) {
        uint32_t scene = sceneOf(functor);
        uint32_t stage = ld32(scene + SCENE_STAGE_ID);
        st32(sceneManager() + SM_NEXT_STAGE, stage);
        call(SAS_openPopupStartDeckEdit, {scene});
    }

    // backButtonRunOutAnimation $_60, scrollRunOutAnimation $_58: run the captured `done`
    static void runCapturedDone(uint32_t functor) { callIfSet(functor + 8); }

    // backButtonRunInAnimation $_59: once the back button panel is visible, flag_3FD is set
    static void backButtonShown(uint32_t functor) {
        uint32_t scene = sceneOf(functor);
        uint32_t back = ld32(scene + 0x2CC);   // leftUiBack
        if (vcall(back, node::isVisible, {back}) == 1) st8(scene + SCENE_FLAG_3FD, 1);
    }

    // openPopupStartDeckEdit $_57: the deck screen closed - forget the picked stage
    static void startDeckEditClosed(uint32_t functor) {
        uint32_t scene = sceneOf(functor);
        st32(scene + SCENE_FIELD_2C8, 0);
        st8(scene + SCENE_FLAG_324, 0);
    }

    // openPopupStartDeckEdit $_56 (bool start): leave the scene when the quest starts
    static void startDeckEditResult(uint32_t functor, const uint8_t *start) {
        if (!*start) return;
        uint32_t scene = sceneOf(functor);
        Frame f;
        uint32_t none = newEmptyFunction(f);
        call(SAS_changeOutAnimation, {scene, none});
        functionDestroy(none);
    }

    // raidBossRewardPopup $_55: open tutorial 33, then (inner lambda) continue with the captured `done`
    // captures: +8 the scene, +0x10 done (std::function<void()>)
    static void raidBossRewardClosed(uint32_t functor) {
        uint32_t scene = ld32(functor + 8);
        Frame f;
        uint32_t copy = f.alloc(24);
        uint32_t next = f.alloc(24);
        call(fn::function_vv_copy, {copy, functor + 0x10});
        st32(next + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x28});
        if (heap) {
            st32(heap, VT_raidBossRewardPopup_55_inner);
            st32(heap + 8, scene);
            call(fn::function_vv_move, {heap + 0x10, copy});
            st32(heap + 8, scene);
        }
        st32(next + 16, heap);
        call(SAS_openTutorial33, {scene, next});
        functionDestroy(next);
        functionDestroy(copy);
    }

    // initEvent $_30 / initColosseum $_35 (Button*, std::function<void()> done), captureless and called
    // directly: the button runs `done` through a touch lambda
    static void initEventSetButton(uint32_t button, uint32_t done) {
        addTouchWithCallback(button, done, VT_initEvent_30_inner);
    }
    static void initColosseumSetButton(uint32_t button, uint32_t done) {
        addTouchWithCallback(button, done, VT_initColosseum_35_inner);
    }
};

using L = SceneAdventureSelectLambdas;
RW_FUNCTION(0x72896C, "SceneAdventureSelect::runOutAnimationMain::$_68::operator()", L::runOutAnimationMainDone);
RW_FUNCTION(0x728A04, "SceneAdventureSelect::backScene::$_67::operator()", L::backToMyPage);
RW_FUNCTION(0x728BB6, "SceneAdventureSelect::backScene::$_66::operator()", L::backToAdventureSelect);
RW_FUNCTION(0x728C62, "SceneAdventureSelect::backScene::$_65::operator()", L::backToStory);
RW_FUNCTION(0x728D08, "SceneAdventureSelect::checkAp::$_64::operator()", L::checkApResult);
RW_FUNCTION(0x728E36, "SceneAdventureSelect::checkApColosseum::$_63::operator()", L::colosseumApOk);
RW_FUNCTION(0x728F02, "SceneAdventureSelect::checkApEvent::$_62::operator()", L::stageApOk);
RW_FUNCTION(0x728FB2, "SceneAdventureSelect::checkApStory::$_61::operator()", L::stageApOk);
RW_FUNCTION(0x729150, "SceneAdventureSelect::backButtonRunOutAnimation::$_60::operator()", L::runCapturedDone);
RW_FUNCTION(0x7291F4, "SceneAdventureSelect::backButtonRunInAnimation::$_59::operator()", L::backButtonShown);
RW_FUNCTION(0x729398, "SceneAdventureSelect::scrollRunOutAnimation::$_58::operator()", L::runCapturedDone);
RW_FUNCTION(0x72943A, "SceneAdventureSelect::openPopupStartDeckEdit::$_57::operator()", L::startDeckEditClosed);
RW_FUNCTION(0x7294E0, "SceneAdventureSelect::openPopupStartDeckEdit::$_56::operator()", L::startDeckEditResult);
RW_FUNCTION(0x729700, "SceneAdventureSelect::raidBossRewardPopup::$_55::operator()", L::raidBossRewardClosed);
RW_FUNCTION(0x723DBC, "SceneAdventureSelect::initEvent::$_30::operator()(Button*, function)", L::initEventSetButton);
RW_FUNCTION(0x724768, "SceneAdventureSelect::initColosseum::$_35::operator()(Button*, function)",
            L::initColosseumSetButton);
