// SceneDeckEdit's medal select panel (Equipment > a deck medal): openMedalSelectPanel (a tutorial popup first when
// a deck medal is a fusion material), openMedalSelectPanelMain (the MiniMedalScrollView of the user's medals, the
// cost labels, the popup and its close lambdas), backScene, and the runUpdateKeybladeEvent $_38 helpers.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, int, UserMedal::Medal)
constexpr uint32_t SafeBuffer_read = 0x45A269;                  // (sb, out, src, n, offset on the stack)
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;      // (sret, id, function<void()>, w, w, w, bool)
constexpr uint32_t SceneDeckEdit_openMedalSelectPanelMain = 0x662611;
constexpr uint32_t BaseSlideView_setSlideEnabled = 0x526111;
constexpr uint32_t MiniMedal_clearDisableMedals = 0x6654E9;
constexpr uint32_t MiniMedal_setItemIDs = 0x4FD1FD;
constexpr uint32_t MiniMedal_setDisableMedalID = 0x4FD425;      // (mini, u64 id, bool, bool)
constexpr uint32_t SceneDeckEdit_setKeybladeArrowVisible = 0x663DD9;
constexpr uint32_t Revolver_setMoveMedalEnabled = 0x52D5B1;
constexpr uint32_t Revolver_setIsShowMedalInfo = 0x52E0D5;
constexpr uint32_t Revolver_changeShowType = 0x52D5B9;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_setNormalColor = 0x468669, SceneUtil_setWarningColor = 0x4686A9;
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t VectorMedal_copy = 0x665539, VectorMedal_dtor = 0x46B119, VectorU64_dtor = 0x468499;
constexpr uint32_t VectorU64_pushSlow = 0x4ECB6D;
constexpr uint32_t AppState_isClearTutorialPopupIDs = 0x4EB815, AppState_setTutorialCloseFunc = 0x4EB8D9;
constexpr uint32_t Widget_isTouchEnabled = rwlib::cocos2d::ui::Widget::isTouchEnabled;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t IInOut_runOutAnimation = 0x6F24B5;
constexpr uint32_t IInOut_canRunOut = 0x51022D;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;       // (api, const function<void(bool)>&, bool)
constexpr uint32_t function_vb_call = 0x474139;
constexpr uint32_t V_setTouchEnabled = 0x29C, V_getScale = 0x44, V_setScale = 0x40;
constexpr uint32_t S_WinUnder = 0xE03EF1, S_CloseButton = 0xDF8D2F, S_TxtPossession = 0xE04023;
constexpr uint32_t S_TxtCost = 0xE04032, S_TxtSlash = 0xE04041, S_TxtCostMax = 0xE04051, S_PercentD = 0xDF86DE;
constexpr int32_t TXT_POSSESSION = 101200044;
constexpr uint32_t TUTORIAL_MATERIAL = 14, TUTORIAL_SELECT_SEEN = 110, TUTORIAL_SELECT = 111;
constexpr uint32_t MEDAL_MATERIAL_FLAG = 0x454, MEDAL_RECORD_SIZE = 0x48;
constexpr uint32_t USERDATA_COST_MAX = 0x34;
constexpr uint32_t VT_TutorialDone = 0xF63864;   // openMedalSelectPanel's tutorial callback {this}
constexpr uint32_t VT_CanClose = 0xF63838;       // $_25 {close button, this}
constexpr uint32_t VT_OnClose = 0xF637E0;        // $_26 {this, Win_under, close button, Win_under's scale}
constexpr uint32_t VT_Closed = 0xF6380C;         // $_26's animation end {this, Win_under, scale}
constexpr uint32_t VT_BackOut = 0xF632B8;        // backScene $_49 {}
constexpr uint32_t VT_KbEvent = 0xF63470;        // runUpdateKeybladeEvent $_38 {bool, function<void(bool)>} (0x28)
constexpr uint32_t VT_KbRetry = 0xF63EC0;        // $_38's error-dialog lambda
// SceneDeckEdit
constexpr uint32_t F_INOUT = 0x288, F_COST_MEDALS = 0x290, F_DECK_MEDALS = 0x29C, F_PANEL = 0x2F8, F_SELECT = 0x2FC;
constexpr uint32_t F_BUTTON_A = 0x310, F_BUTTON_B = 0x318, F_BUTTON_C = 0x31C, F_SELECTING = 0x37C;
constexpr uint32_t F_REVOLVER = 0x380, F_SLIDE = 0x38C, F_MINI = 0x390, F_SE_FLAG = 0x3A1, F_BACK_ANIM = 0x3A8;
constexpr uint32_t F_CAN_BACK = 0x3C0;
constexpr uint32_t REVOLVER_EXCLUDED = 0x2C0, REVOLVER_MOVING = 0x300, MINI_SHOW_TYPE = 0x6F4;
// SceneManager
constexpr uint32_t SM_SELECTED_MEDAL = 0x2A0, SM_PANEL_OPEN = 0x2E1;

void setTouch(uint32_t w, uint32_t on) { vcall(w, V_setTouchEnabled, {w, on}); }
void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }
void setNumber(uint32_t text, uint32_t value) {
    Frame f;
    const uint32_t buf = stringBuffer(f, {S_PercentD, value});
    setTextC(text, buf);
}
uint32_t readU32(uint32_t sb, uint32_t offset, uint32_t out) {
    Frame f;
    const uint32_t args = f.alloc(8);
    st32(args, offset);
    rw::call_exact(SafeBuffer_read, {sb, out, ld32(sb + 0xC) + offset, 4});
    return ld32(out);
}
}  // namespace

namespace deck_medal_select {

// A tutorial popup first when a deck medal (not one of the revolver's excluded ones) is a fusion material, else
// the panel. The original frame is kept: getMedalData's medal record argument has a never-set word.
uint32_t openMedalSelectPanel(uint32_t self) {
    Frame fr;
    const uint32_t sp = fr.allocExact(0x128);
    if (vcall(ld32(self + F_PANEL), node::isVisible, {ld32(self + F_PANEL)})) return 0;
    const uint32_t medal = sp + 0x84, flag = sp + 0x8C, done = sp + 0x90, popup = sp + 0xA8, record = sp + 0xC0;
    bool material = false;
    const uint32_t rev = ld32(self + F_REVOLVER);
    for (uint32_t i = 0; i < (ld32(self + F_DECK_MEDALS + 4) - ld32(self + F_DECK_MEDALS)) >> 3; i++) {
        const uint32_t m = ld32(ld32(self + F_DECK_MEDALS) + i * 8);
        const uint32_t lo = ld32(m), hi = ld32(m + 4);
        bool excluded = false;
        const uint32_t xb = ld32(rev + REVOLVER_EXCLUDED), xe = ld32(rev + REVOLVER_EXCLUDED + 4);
        for (uint32_t j = 0; xe != xb && j < (xe - xb) >> 3; j++) {
            const uint32_t x = ld32(xb + j * 8);
            if (ld32(x) == lo && ld32(x + 4) == hi) {
                excluded = true;
                break;
            }
        }
        if (excluded) continue;
        const uint32_t mgr = call(MedalDataManager_getInstance, {});
        for (uint32_t k = 0; k < 0x2C; k += 4) fst32(record + k, 0);
        for (uint32_t k = 0x30; k < 0x48; k += 4) fst32(record + k, 0);
        fst32(sp, 0);
        for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4) fst32(sp + 8 + k, ld32(record + k));
        rw::call_exact(MedalDataManager_getMedalData, {medal, mgr, lo, hi});
        const uint32_t sb = ld32(ld32(medal) + 0x58);
        fst32(sp, MEDAL_MATERIAL_FLAG);
        rw::call_exact(SafeBuffer_read, {sb, flag, ld32(sb + 0xC) + MEDAL_MATERIAL_FLAG, 4});
        const bool isMaterial = ld32(flag) == 1;
        releaseIf(ld32(medal + 4));
        if (isMaterial) {
            material = true;
            break;
        }
    }
    if (!material) {
        call(fn::AudioPlaySystemSEPopUp, {});
        call(SceneDeckEdit_openMedalSelectPanelMain, {self});
        return 0;
    }
    fst32(done + 0x10, done);
    fst32(done, VT_TutorialDone);
    fst32(done + 4, self);
    fst32(sp, 0);
    fst32(sp + 4, 0);
    fst32(sp + 8, 0);
    rw::call_exact(SceneUtil_openTutorialPopup, {popup, TUTORIAL_MATERIAL, done, 0});
    functionDestroy(popup);
    functionDestroy(done);
    return 0;
}
RW_FUNCTION(0x6652C0, "SceneDeckEdit::openMedalSelectPanel", deck_medal_select::openMedalSelectPanel);

// The panel: the deck's medal ids in the MiniMedalScrollView (scrolled to the medal picked before), the revolver's
// medals disabled, "owned" / cost / max cost labels, the popup ($_25 can close, $_26 closed), the tutorial popup.
uint32_t openMedalSelectPanelMain(uint32_t self) {
    constexpr uint32_t EPILOGUE_R0 = 0x97C;
    const uint32_t select = ld32(self + F_SELECT);
    if (vcall(select, node::isVisible, {select})) return EPILOGUE_R0;
    st8(self + F_SELECTING, 1);
    call(BaseSlideView_setSlideEnabled, {ld32(self + F_SLIDE), 0});
    call(MiniMedal_clearDisableMedals, {ld32(self + F_MINI)});
    call(SceneDeckEdit_setKeybladeArrowVisible, {self, 0});
    call(Revolver_setMoveMedalEnabled, {ld32(self + F_REVOLVER), 1});
    call(Revolver_setIsShowMedalInfo, {ld32(self + F_REVOLVER), 1});
    call(Revolver_changeShowType, {ld32(self + F_REVOLVER), ld32(ld32(self + F_MINI) + MINI_SHOW_TYPE)});
    const uint32_t win = findNode(ld32(self + F_SELECT), S_WinUnder, 9);
    const uint32_t scale = vcall(win, V_getScale, {win});
    const uint32_t close = findNode(ld32(self + F_SELECT), S_CloseButton, 12);
    setTouch(win, 1);
    setTouch(ld32(self + F_BUTTON_A), 0);
    setTouch(ld32(self + F_BUTTON_B), 0);
    const uint32_t tutorialDone = call(SceneUtil_getNexTutorialphase, {}) > 994 ? 1 : 0;
    setTouch(ld32(self + F_BUTTON_C), tutorialDone);
    setTouch(close, tutorialDone);

    Frame f;
    const uint32_t ids = f.alloc(12), idTmp = f.alloc(8);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    uint32_t index = 0;
    for (uint32_t i = 0; ld32(self + F_DECK_MEDALS + 4) != ld32(self + F_DECK_MEDALS) &&
                         i < (ld32(self + F_DECK_MEDALS + 4) - ld32(self + F_DECK_MEDALS)) >> 3; i++) {
        const uint32_t m = ld32(ld32(self + F_DECK_MEDALS) + i * 8);
        const uint32_t lo = ld32(m), hi = ld32(m + 4);
        st32(idTmp, lo);
        st32(idTmp + 4, hi);
        const uint32_t end = ld32(ids + 4);
        if (end == ld32(ids + 8)) {
            call(VectorU64_pushSlow, {ids, idTmp});
        } else {
            if (end) {
                st32(end, lo);
                st32(end + 4, hi);
            }
            st32(ids + 4, ld32(ids + 4) + 8);
        }
        const uint32_t sm = sceneManager();
        if (ld32(sm + SM_SELECTED_MEDAL) == lo && ld32(sm + SM_SELECTED_MEDAL + 4) == hi) index = i;
    }
    {
        const uint32_t sm = sceneManager();
        const uint32_t canClose = newFunction(f, VT_CanClose, {close, self});
        const uint32_t onClose = f.alloc(24);
        st32(onClose + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x14});
        if (h) {
            st32(h, VT_OnClose);
            st32(h + 4, self);
            st32(h + 8, win);
            st32(h + 0xC, close);
            st32(h + 0x10, scale);
        }
        st32(onClose + 16, h);
        call(fn::SceneManager_openPopup, {sm, canClose, onClose, 1});
        functionDestroy(onClose);
        functionDestroy(canClose);
    }
    const uint32_t mini = ld32(self + F_MINI);
    vcall(mini, node::setVisible, {mini, 1});
    call(MiniMedal_setItemIDs, {ld32(self + F_MINI), ids});
    call(IScrollProtocol_setIndex, {ld32(self + F_MINI), ld8(sceneManager() + SM_PANEL_OPEN) ? index : 0});
    const uint32_t excluded = f.alloc(12);
    call(VectorMedal_copy, {excluded, ld32(self + F_REVOLVER) + REVOLVER_EXCLUDED});
    for (uint32_t j = 0; ld32(excluded + 4) != ld32(excluded) && j < (ld32(excluded + 4) - ld32(excluded)) >> 3; j++) {
        const uint32_t m = ld32(ld32(excluded) + j * 8);
        Frame g;
        const uint32_t args = g.alloc(8);
        st32(args, 1);
        st32(args + 4, 1);
        rw::call_exact(MiniMedal_setDisableMedalID, {ld32(self + F_MINI), 1, ld32(m), ld32(m + 4)});
    }
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t user = ld32(api + 0x94), userCtl = ld32(api + 0x98);
    if (userCtl) call(fn::shared_add, {userCtl});
    titleLabel(win, S_TxtPossession, 14, TXT_POSSESSION);
    const uint32_t cost = findNode(win, S_TxtCost, 14);
    SceneUtil::setShadow(cost);
    const int32_t count = int32_t(ld32(self + F_COST_MEDALS + 4) - ld32(self + F_COST_MEDALS)) >> 3;
    setNumber(cost, uint32_t(count));
    const uint32_t out = f.alloc(8);
    if (int32_t(readU32(user, USERDATA_COST_MAX, out)) >= count) call(SceneUtil_setNormalColor, {cost});
    else call(SceneUtil_setWarningColor, {cost});
    SceneUtil::setShadow(findNode(win, S_TxtSlash, 15));
    const uint32_t costMax = findNode(win, S_TxtCostMax, 17);
    SceneUtil::setShadow(costMax);
    setNumber(costMax, readU32(user, USERDATA_COST_MAX, out + 4));
    if (!ld8(sceneManager() + SM_PANEL_OPEN)) {
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        call(fn::Node_runAction, {win, SceneUtil::createOpenPopupAnimation(win, none)});
        functionDestroy(none);
    }
    st8(sceneManager() + SM_PANEL_OPEN, 1);
    vcall(ld32(self + F_SELECT), node::setVisible, {ld32(self + F_SELECT), 1});
    const uint32_t app = call(fn::AppState_getInstance, {});
    if (call(AppState_isClearTutorialPopupIDs, {app, TUTORIAL_SELECT_SEEN}) == 1) {
        const uint32_t app2 = call(fn::AppState_getInstance, {});
        Frame g;
        const uint32_t none = newEmptyFunction(g);
        const uint32_t popup = g.alloc(24), closeFn = g.alloc(24);
        const uint32_t args = g.alloc(12);
        st32(args, 0);
        st32(args + 4, 0);
        st32(args + 8, 0);
        rw::call_exact(SceneUtil_openTutorialPopup, {popup, TUTORIAL_SELECT, none, win});
        call(AppState_setTutorialCloseFunc, {closeFn, app2, popup, TUTORIAL_SELECT});
        functionDestroy(closeFn);
        functionDestroy(popup);
        functionDestroy(none);
    }
    releaseIf(userCtl);
    call(VectorMedal_dtor, {excluded});
    call(VectorU64_dtor, {ids});
    return EPILOGUE_R0;
}
RW_FUNCTION(0x662610, "SceneDeckEdit::openMedalSelectPanelMain", deck_medal_select::openMedalSelectPanelMain);

// back (once, when the out animation may run): the screen animates out, then $_49
uint32_t backScene(uint32_t self) {
    if (!ld8(self + F_CAN_BACK)) return 0;
    if (call(IInOut_canRunOut, {self + F_BACK_ANIM}) != 1) return 0;
    st8(self + F_CAN_BACK, 0);
    Frame f;
    const uint32_t then = newFunction(f, VT_BackOut, {});
    call(IInOut_runOutAnimation, {self + F_INOUT, then});
    functionDestroy(then);
    return 0;
}
RW_FUNCTION(0x6697B4, "SceneDeckEdit::backScene", deck_medal_select::backScene);

// ---- the panel popup's lambdas
// $_25: the popup may close while its Close button takes touches and no medal is being moved
uint32_t canClose(uint32_t self) {
    if (call(Widget_isTouchEnabled, {ld32(self + 4)}) != 1) return 0;
    return ld8(ld32(ld32(self + 8) + F_REVOLVER) + REVOLVER_MOVING) ? 0 : 1;
}
RW_FUNCTION(0x678B56, "SceneDeckEdit::openMedalSelectPanelMain::$_25 operator()", deck_medal_select::canClose);

void canCloseCloneInto(uint32_t self, uint32_t p) {
    if (!p) return;
    st32(p, VT_CanClose);
    const uint32_t a = ld32(self + 4);
    st32(p + 8, ld32(self + 8));
    st32(p + 4, a);
}
RW_FUNCTION(0x678B28, "SceneDeckEdit::openMedalSelectPanelMain::$_25 __clone(p)", deck_medal_select::canCloseCloneInto);

// $_26: closed by Close (false): the controls back, the panel animates out
uint32_t closed(uint32_t self, uint32_t okRef) {
    if (ld8(okRef)) return 0;
    const uint32_t scene = ld32(self + 4);
    if (ld8(scene + F_SE_FLAG)) {
        call(AudioPlaySystemSECancel, {});
        st8(sceneManager() + SM_PANEL_OPEN, 0);
    }
    setTouch(ld32(self + 8), 0);
    call(SceneDeckEdit_setKeybladeArrowVisible, {scene, 1});
    setTouch(ld32(scene + F_BUTTON_C), 0);
    setTouch(ld32(self + 0xC), 0);
    call(Revolver_setMoveMedalEnabled, {ld32(scene + F_REVOLVER), 0});
    const uint32_t win = ld32(self + 8);
    Frame f;
    const uint32_t done = newFunction(f, VT_Closed, {scene, win, ld32(self + 0x10)});
    call(fn::Node_runAction, {win, SceneUtil::createClosePopupAnimation(win, done)});
    functionDestroy(done);
    return 0;
}
RW_FUNCTION(0x67886C, "SceneDeckEdit::openMedalSelectPanelMain::$_26 operator()", deck_medal_select::closed);

uint32_t closedClone(uint32_t self) {
    const uint32_t c = call(fn::operator_new, {0x14});
    if (c) {
        st32(c, VT_OnClose);
        const uint32_t a = ld32(self + 4), b = ld32(self + 8), d = ld32(self + 0xC), e = ld32(self + 0x10);
        st32(c + 0x10, e);
        st32(c + 0xC, d);
        st32(c + 8, b);
        st32(c + 4, a);
    }
    return c;
}
RW_FUNCTION(0x678800, "SceneDeckEdit::openMedalSelectPanelMain::$_26 __clone()", deck_medal_select::closedClone);

// $_26's animation end: the panel hidden, Win_under's scale back, the deck controls back on
uint32_t panelHidden(uint32_t self) {
    const uint32_t scene = ld32(self + 4);
    vcall(ld32(scene + F_SELECT), node::setVisible, {ld32(scene + F_SELECT), 0});
    vcall(ld32(scene + F_MINI), node::setVisible, {ld32(scene + F_MINI), 0});
    vcall(ld32(self + 8), V_setScale, {ld32(self + 8), ld32(self + 0xC)});
    call(BaseSlideView_setSlideEnabled, {ld32(scene + F_SLIDE), 1});
    call(Revolver_setIsShowMedalInfo, {ld32(scene + F_REVOLVER), 0});
    call(Revolver_changeShowType, {ld32(scene + F_REVOLVER), 0});
    setTouch(ld32(scene + F_BUTTON_A), 1);
    return vcall(ld32(scene + F_BUTTON_B), V_setTouchEnabled, {ld32(scene + F_BUTTON_B), 1});
}
RW_FUNCTION(0x678A32, "SceneDeckEdit::openMedalSelectPanelMain::$_26 animation end operator()", deck_medal_select::panelHidden);

void panelHiddenCloneInto(uint32_t self, uint32_t p) {
    if (!p) return;
    st32(p, VT_Closed);
    const uint32_t a = ld32(self + 4), b = ld32(self + 8), c = ld32(self + 0xC);
    st32(p + 0xC, c);
    st32(p + 8, b);
    st32(p + 4, a);
}
RW_FUNCTION(0x678A00, "SceneDeckEdit::openMedalSelectPanelMain::$_26 animation end __clone(p)", deck_medal_select::panelHiddenCloneInto);

// ---- runUpdateKeybladeEvent $_38 {bool, function<void(bool)>}: the loading indicator off; on failure (when
// asked) the error dialog first; the callback gets the result
uint32_t keybladeEventDone(uint32_t lambda, uint32_t ok) {
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok && ld8(lambda) && ld8(call(fn::APIManager_getInstance, {}) + 0x14)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t retry = newFunction(f, VT_KbRetry, {});
        call(APIManager_openErrorDialog, {api, retry, 1});
        functionDestroy(retry);
    }
    call(function_vb_call, {lambda + 8, ok ? 1u : 0u});
    return 0;
}
RW_FUNCTION(0x668B08, "SceneDeckEdit::runUpdateKeybladeEvent::$_38 body", deck_medal_select::keybladeEventDone);

uint32_t keybladeEventClone(uint32_t self) {
    const uint32_t c = call(fn::operator_new, {0x28});
    if (c) {
        st32(c, VT_KbEvent);
        st8(c + 8, ld8(self + 8));
        call(fn::function_vb_copy, {c + 0x10, self + 0x10});
        st8(c + 8, ld8(self + 8));
    }
    return c;
}
RW_FUNCTION(0x67625C, "SceneDeckEdit::runUpdateKeybladeEvent::$_38 __clone()", deck_medal_select::keybladeEventClone);

uint32_t keybladeEventDestroyDeallocate(uint32_t self) {
    functionDestroy(self + 0x10);
    return call(fn::operator_delete, {self});
}
RW_FUNCTION(0x676314, "SceneDeckEdit::runUpdateKeybladeEvent::$_38 destroy_deallocate", deck_medal_select::keybladeEventDestroyDeallocate);

}  // namespace deck_medal_select
