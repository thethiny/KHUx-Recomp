// Shop > a medal draw: the lambdas of SceneDrawRaise::openDrawResultsPopup (the results of a multi-draw) and
// DrawScrollView::openCheckRaisePopup's OK button.
//   $_12 (the rare-medal flash, then the drop): the draw SE, the Draw_MedalGet LWF on the medal's Area_Point and
//        SceneDrawRaise::dropAnimation with UlvE_ (which shows the OK / share buttons afterwards);
//   $_10 (OK touched): closes the popup with two close animations; UlvE0_ removes the results and either opens the
//        bonus ("omake") popup or returns to the shop top ($_18 UlvE_ is the first animation's empty callback).
// The __clone thunks copy their captured words.
#include "../scene/SceneUi.hpp"
#include "../scene_main/FunctionThunks.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;      // (shared_ptr<Layer> out, mgr, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;          // (layer, ResourceID, int, int)
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;         // (const char* path, void*)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;  // (node, function<void()>)
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t SceneDrawRaise_dropAnimation = 0x692661;   // (this, function<void()>)
constexpr uint32_t SceneDrawRaise_openDrawOmakePopup = 0x691B55;
constexpr uint32_t SceneShopTop_callShopAPI = 0x68BB9D;       // (function<void(bool)>)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;  // (mgr, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1; // (mgr, bool)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t CheckRaise_onClose = 0x5B1E5D;          // sub_5B1E5C(captured callback, bool)
constexpr uint32_t AppState_getTutorialCloseFunc = 0x4EBA3D;  // (function out, state, int id)
constexpr uint32_t AppState_execTutorialCloseFunc = 0x4EBA5D;
}  // namespace g

constexpr uint32_t SE_MEDAL_GET = 0x1E584184;
constexpr uint32_t S_Area_Point = 0xDF9FB4;
constexpr uint32_t S_DrawMedalGetLwf = 0xE04DC1;   // "lwf/drawraise/Draw_MedalGet/Draw_MedalGet.lwf"
constexpr uint32_t TUTORIAL_DRAW = 109;

// SceneDrawRaise fields
constexpr uint32_t F_HAS_OMAKE = 0x2AC;       // a bonus popup follows the results
constexpr uint32_t F_RESULTS_OPEN = 0x2AE;    // the results popup takes touches
// SceneManager fields
constexpr uint32_t SM_FLAG_4CC = 0x4CC, SM_ON_SHOP_BACK = 0x548;   // std::function<void(bool)>

// __func vtables (+8)
constexpr uint32_t VT_DropDone = 0xF65184;        // $_12 UlvE_ {okButton, shareButton, facebookButton}
constexpr uint32_t VT_CloseEmpty = 0xF652E4;      // $_18 UlvE_ {node, node}
constexpr uint32_t VT_Closed = 0xF65260;          // $_10 UlvE0_ {parent, shadow, popup, scene}
constexpr uint32_t VT_Omake = 0xF652B8;           // UlvE0_'s omake lambda {bool}
constexpr uint32_t VT_ShopTopLayer = 0xF6528C;    // UlvE0_'s scene layer lambda {}

void setVisible(uint32_t node) { vcall(node, node::setVisible, {node, 1}); }

struct GachaLambdas {
    // $_12 {medal, scene, ok, share, facebook}
    static void rareFlash(uint32_t functor) {
        uint32_t scene = ld32(functor + 8);
        {
            Frame f;
            uint32_t layer = f.alloc(8);
            call(g::audio_Manager_getLayer, {layer, call(g::audio_Manager_getInstance, {}), 0});
            call(g::audio_Layer_playSE, {ld32(layer), SE_MEDAL_GET, 0, 0});
            sharedRelease(layer);
        }
        uint32_t area = findNode(ld32(functor + 4), S_Area_Point, 10);
        uint32_t lwf = call(g::AudioLWFNode_create, {S_DrawMedalGetLwf, 0});
        vcall(area, node::addChild, {area, lwf});
        Frame f;
        uint32_t done = newFunction(f, VT_DropDone, {ld32(functor + 0xC), ld32(functor + 0x10), ld32(functor + 0x14)});
        call(g::SceneDrawRaise_dropAnimation, {scene, done});
        functionDestroy(done);
    }

    // $_12 UlvE_: the buttons appear (the Facebook one only when logged in)
    static void dropDone(uint32_t functor) {
        setVisible(ld32(functor + 4));
        setVisible(ld32(functor + 8));
        if (call(g::SceneUtil_isLoggedInFacebook, {}) == 1) setVisible(ld32(functor + 0xC));
    }

    // $_10 (Ref*, TouchEventType) {scene, scene, popup, node, parent, shadow}: OK released while the popup is open
    static void okTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2 || !ld8(ld32(functor + 4) + F_RESULTS_OPEN)) return;
        call(g::AudioPlaySystemSEDecide, {});
        uint32_t scene = ld32(functor + 8);
        st8(scene + F_RESULTS_OPEN, 0);
        uint32_t popup = ld32(functor + 0xC);
        {
            Frame f;
            uint32_t done = newFunction(f, VT_CloseEmpty, {ld32(functor + 0x14), ld32(functor + 0x10)});
            uint32_t anim = call(g::SceneUtil_createClosePopupAnimation, {ld32(functor + 0x10), done});
            call(fn::Node_runAction, {popup, call(g::EaseSineOut_create, {anim})});
            functionDestroy(done);
        }
        uint32_t parent = ld32(functor + 0x14), shadow = ld32(functor + 0x18);
        Frame f;
        uint32_t closed = newEmptyFunction(f);
        uint32_t heap = call(fn::operator_new, {0x14});
        if (heap) {
            st32(heap, VT_Closed);
            st32(heap + 4, parent);
            st32(heap + 8, shadow);
            st32(heap + 0xC, popup);
            st32(heap + 0x10, scene);
        }
        st32(closed + 16, heap);
        uint32_t anim = call(g::SceneUtil_createClosePopupAnimation, {popup, closed});
        call(fn::Node_runAction, {popup, call(g::EaseSineOut_create, {anim})});
        functionDestroy(closed);
    }

    // $_10 UlvE0_ {parent, shadow, popup, scene}: remove the popup; the bonus popup, or back to the shop top
    static void closed(uint32_t functor) {
        uint32_t scene = ld32(functor + 0x10);
        uint32_t parent = ld32(functor + 4);
        vcall(parent, node::removeChild, {parent, ld32(functor + 8), 1});
        parent = ld32(functor + 4);
        vcall(parent, node::removeChild, {parent, ld32(functor + 0xC), 1});
        Frame f;
        if (ld8(scene + F_HAS_OMAKE)) {
            uint32_t omake = newFunction(f, VT_Omake, {});
            st8(omake + 4, 0);
            call(g::SceneDrawRaise_openDrawOmakePopup, {0, omake});
            functionDestroy(omake);
            return;
        }
        uint32_t mgr = sceneManager();
        st8(mgr + SM_FLAG_4CC, 1);
        uint32_t none = newEmptyFunction(f);
        call(g::FunctionVB_assign, {mgr + SM_ON_SHOP_BACK, none});
        functionDestroy(none);
        uint32_t none2 = newEmptyFunction(f);
        call(g::SceneShopTop_callShopAPI, {none2});
        functionDestroy(none2);
        mgr = sceneManager();
        uint32_t layer = newFunction(f, VT_ShopTopLayer, {});
        call(g::SceneManager_changeSceneLayer, {mgr, layer, 1, 1, 1});
        functionDestroy(layer);
    }

    // DrawScrollView::openCheckRaisePopup $_15 (Ref*, TouchEventType) {.., callback at +8}: OK released
    static void checkRaiseOk(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        call(g::SceneManager_closeCurrentPopup, {sceneManager(), 1});
        call(g::AudioPlaySystemSEDecide, {});
        call(g::CheckRaise_onClose, {functor + 8, 1});
        Frame f;
        uint32_t closeFunc = f.alloc(20);
        call(g::AppState_getTutorialCloseFunc, {closeFunc, call(fn::AppState_getInstance, {}), TUTORIAL_DRAW});
        functionDestroy(closeFunc);
        call(g::AppState_execTutorialCloseFunc, {call(fn::AppState_getInstance, {}), TUTORIAL_DRAW});
    }
};

using thunk::cap;
using Clone12 = thunk::Lambda<0xF65158, 0x18, 0, 0, 4, cap(4, 4), cap(8, 4), cap(0xC, 4), cap(0x10, 4), cap(0x14, 4)>;
using Clone12Drop = thunk::Lambda<VT_DropDone, 0x10, 0, 0, 4, cap(4, 4), cap(8, 4), cap(0xC, 4)>;
using Clone11 = thunk::Lambda<0xF651B0, 0xC, 0, 0, 4, cap(4, 4), cap(8, 4)>;
using Clone10 = thunk::Lambda<0xF65234, 0x1C, 0, 0, 4, cap(4, 4), cap(8, 4), cap(0xC, 4), cap(0x10, 4), cap(0x14, 4),
                              cap(0x18, 4)>;
using Clone10Closed = thunk::Lambda<VT_Closed, 0x14, 0, 0, 4, cap(4, 4), cap(8, 4), cap(0xC, 4), cap(0x10, 4)>;
using Clone18 = thunk::Lambda<VT_CloseEmpty, 0xC, 0, 0, 4, cap(4, 4), cap(8, 4)>;
}  // namespace

RW_FUNCTION(0x6934B4, "SceneDrawRaise::openDrawResultsPopup $_12 __func::operator() (sub_6934B4)", GachaLambdas::rareFlash);
RW_FUNCTION(0x6936BE, "SceneDrawRaise::openDrawResultsPopup $_12 UlvE_ __func::operator() (sub_6936BE)",
            GachaLambdas::dropDone);
RW_FUNCTION(0x693AC4, "SceneDrawRaise::openDrawResultsPopup $_10 __func::operator() (sub_693AC4)", GachaLambdas::okTouched);
RW_FUNCTION(0x693CC0, "SceneDrawRaise::openDrawResultsPopup $_10 UlvE0_ __func::operator() (sub_693CC0)",
            GachaLambdas::closed);
RW_FUNCTION(0x5B1D8C, "DrawScrollView::openCheckRaisePopup $_15 __func::operator() (sub_5B1D8C)",
            GachaLambdas::checkRaiseOk);
RW_FUNCTION(0x69344C, "SceneDrawRaise::openDrawResultsPopup $_12 __func::__clone() (sub_69344C)", Clone12::clone);
RW_FUNCTION(0x69368C, "SceneDrawRaise::openDrawResultsPopup $_12 UlvE_ __func::__clone(__base*) (sub_69368C)",
            Clone12Drop::cloneInto);
RW_FUNCTION(0x693760, "SceneDrawRaise::openDrawResultsPopup $_11 __func::__clone(__base*) (sub_693760)", Clone11::cloneInto);
RW_FUNCTION(0x693A54, "SceneDrawRaise::openDrawResultsPopup $_10 __func::__clone() (sub_693A54)", Clone10::clone);
RW_FUNCTION(0x693C54, "SceneDrawRaise::openDrawResultsPopup $_10 UlvE0_ __func::__clone() (sub_693C54)",
            Clone10Closed::clone);
RW_FUNCTION(0x6940F0, "SceneDrawRaise::openDrawResultsPopup $_18 UlvE_ __func::__clone(__base*) (sub_6940F0)",
            Clone18::cloneInto);
