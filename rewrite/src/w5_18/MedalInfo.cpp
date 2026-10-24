// Equipment > Medal List: the medal info popup and how it is reached - MedalListScrollView's touch lambdas (press,
// move-out / drag, release: the medal under the touch is picked), SceneMedalList's tap and long-press lambdas
// (init $_3 / $_5: the info popup with its button and close callbacks), its answer to those buttons
// (onTapMedalInfoButton: select, Level Up or Evolve) and their scene-change lambdas, SceneUtil::openMedalInfoPopup,
// and a SceneUtil text helper. SceneUtil::updateMedalInfo is in MedalInfoUpdate.cpp.
#include "../scene/SceneUi.hpp"

#include <cmath>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction, Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t FunctionVI_call = 0x4F321D;             // std::function<void(int)>::operator()(int)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t SceneUtil_createTapAnimation = 0x45BBA9;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t StringFromChars = 0x47CD93;              // std::string(out, const char*, len)
constexpr uint32_t MedalList_touchEnded = 0x59B381;         // the release lambda's call operator
constexpr uint32_t MedalListScrollView_setSelectedMedal = 0x59AE3D;
constexpr uint32_t SceneMedalList_onTapMedalInfoButton = 0x73DB0D;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;          // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t Mixed_callPrepareAPI = 0x69F475;          // (function<void(bool)> by value)
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_openMedalInfoPopup = 0x466EF5;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t SM_getPopupLayer2 = 0x613E71, SM_openPopup = 0x61524D;   // openPopup(sm, fn<bool()>, fn<void(bool)>, bool)
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;
constexpr uint32_t SceneUtil_updateMedalInfo = 0x462195;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t SceneUtil_setTextDisabled = 0x4744F5;     // sub_4744F4(text, a, b): the greyed-out label style
constexpr uint32_t FunctionVV_copy = 0x4683CD;               // std::function<void()>(const function&)
constexpr uint32_t FunctionVV_move = 0x47F905;               // std::function<void()>(function&&)
constexpr uint32_t FunctionInfo_copy = 0x4683FF;             // std::function<void(MedalInfoButtonType, bool)> copy
constexpr uint32_t FunctionInfo_move = 0x488969;             //   and move constructors
constexpr uint32_t VectorULL_copy = 0x468431, VectorULL_dtor = 0x468499;
constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_removeNewUserMedalID = 0x4EB2C5;
constexpr uint32_t AppState_getWillLockUserMedalIDs = 0x4EB347, AppState_getWillUnLockUserMedalIDs = 0x4EB507;
constexpr uint32_t AppState_isClearTutorialPopupIDs = 0x4EB815, AppState_setTutorialCloseFunc = 0x4EB8D9;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageButton_setTargetImageView = 0x531631, ImageButton_setSelectEnabled = 0x5318F1;
constexpr uint32_t ImageButton_setIsSelected = 0x5318FD;
constexpr uint32_t TutorialMisc_create = 0x777BB5;
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright, Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t CheckBox_setSelectedState = rwlib::cocos2d::ui::CheckBox::setSelectedState, CheckBox_addEventListener = rwlib::cocos2d::ui::CheckBox::addEventListener;
constexpr uint32_t GOT_VEC2_ZERO = 0xFD605C, GOT_COLOR3B_WHITE = 0xFD6060;
constexpr uint32_t aeabi_fixsfsi = 0xCD7240, aeabi_idiv = 0xCD5E8D;
constexpr uint32_t GOT_ROW_SPACING = 0xFD659C;              // &float added to the cell height (row step)
}  // namespace g

// ------------------------------------------------------------------------------------------------ MedalListScrollView
constexpr uint32_t F_ROWS = 0x51C;            // IScrollProtocol's row widgets
constexpr uint32_t F_COLUMNS = 0x538, F_SCROLL_Y = 0x540, F_ITEM_H = 0x518;
constexpr uint32_t F_ITEM_IDS = 0x628;        // BaseScrollView<ull> item ids (user medal ids)
constexpr uint32_t F_ON_TAP = 0x638;          // std::function<void(int)>
constexpr uint32_t F_PRESSED = 0x689;         // a cell is pressed (cleared on move-out / release)
constexpr uint32_t F_PRESS_INDEX = 0x68C, F_TAPPED = 0x690, F_PRESS_Y = 0x694;
constexpr uint32_t F_DRAGGED = 0x698, F_HOLD = 0x69C, F_BEGAN_INDEX = 0x6A0, F_START = 0x6A4;   // start: x, y
constexpr uint32_t VT_TapDone = 0xF5B278;     // the tap animation's done lambda {two words}

// ------------------------------------------------------------------------------------------------ SceneMedalList
constexpr uint32_t F_INOUT = 0x2A4;
constexpr uint32_t F_LIST = 0x2B4;            // MedalListScrollView
constexpr uint32_t SM_MEDAL_ID = 0x2A0;       // SceneManager: the medal id handed to the next scene (u64)
constexpr uint32_t SM_MIX_FLAG = 0x4CC, SM_MIX_CALLBACK = 0x548;
constexpr uint32_t VT_ToMixed = 0xF6DA00;     // runOutAnimation {}: sub_73E108 (Level Up)
constexpr uint32_t VT_ToEvolve = 0xF6D9A8;    // runOutAnimation {}: sub_73DF48 (Evolve)
constexpr uint32_t VT_EvolveLayer = 0xF6D9D4; // function<Layer*()>: the Evolve scene's create
constexpr uint32_t VT_MixedLayer = 0xF6DA2C;  // function<Layer*()>: SceneMixedMedal's create

// SceneMedalList's list taps: the widgets that stop taking touches while the info popup is open, and its callbacks
constexpr uint32_t F_TOUCH_A = 0x2B8, F_TOUCH_B = 0x2BC, F_CAN_TAP = 0x2C0;
constexpr uint32_t VT_LongPressButton = 0xF6DADC;  // init $_5's popup button callback {this, u64 medal id} (heap 0x18)
constexpr uint32_t VT_LongPressClosed = 0xF6DAB0;  // init $_5's popup close callback {this}
constexpr uint32_t VT_TapButton = 0xF6DBE4, VT_TapClosed = 0xF6DBB8;   // the same for init $_3
constexpr uint32_t LIST_ITEM_IDS = 0x628;          // MedalListScrollView: vector<ull> of the listed medal ids
constexpr uint32_t API_USER = 0x94;           // APIManager: the user's SafeBuffer (+0x54: a flag byte)
constexpr uint32_t V_setTouchEnabled = 0x29C;

// ------------------------------------------------------------------------------------------------ the info popup
constexpr uint32_t S_MedalInfoJson = 0xDF8CF7;   // "cocostudio/publish/MedalInfoScene_ver106.json" (45)
constexpr uint32_t S_MedalInfo = 0xDF8D25, S_LeftUI = 0xDF85F0, S_RightUI = 0xDF85F7, S_CloseButton = 0xDF8D2F;
constexpr uint32_t S_FavoriteCheckBox = 0xDF8D3C, S_But11 = 0xDF8D4E, S_ButtonMix = 0xDF87D4;
constexpr uint32_t S_ButtonLimitCut = 0xDF8D54, S_TxtLimitCut = 0xDF8D64, S_ButtonOK = 0xDF8688, S_TxtOK = 0xDF869F;
constexpr uint32_t S_SupportCheckBox = 0xDF8D71, S_TxtPartner = 0xDF8D82, S_TxtMix = 0xDF87DF;
constexpr int32_t TXT_PARTNER = 101200029, TXT_MIX = 101300001, TXT_LIMIT_CUT = 101400001;
constexpr uint32_t TUTORIAL_MEDAL_INFO = 115, TUTORIAL_MIX = 116;
constexpr uint32_t VT_FavoriteChanged = 0xF54D1C;   // {medal shared_ptr, on-button fn} (heap 0x28)
constexpr uint32_t VT_PartnerTouch = 0xF54C6C;      // {but11, on-button fn, medal, but11, root, on-button fn} (0x50)
constexpr uint32_t VT_CloseTouch = 0xF54C40;        // {}
constexpr uint32_t VT_MixTouch = 0xF54C14, VT_LimitCutTouch = 0xF54BE8;   // {on-button fn} (heap 0x20)
constexpr uint32_t VT_OpenDone = 0xF54BBC;          // {bool}
constexpr uint32_t VT_PopupOpen = 0xF54B90;         // openPopup's fn<bool()> {close button}
constexpr uint32_t VT_PopupClose = 0xF54B38;        // openPopup's fn<void(bool)> (heap 0x48)
constexpr uint32_t MEDAL_ID = 0, MEDAL_LOCKED = 0x24, MEDAL_SB = 0x58;    // MedalDataManager::UserMedalData
constexpr uint32_t API_PARTNER = 0x114;             // APIManager: shared_ptr<SafeBuffer> (+0x58: the partner medal id)
constexpr uint32_t V_setTouchEnabledW = 0x29C;
constexpr uint32_t IMAGEBUTTON_SELECTABLE = 0x272;

uint32_t medalRead32(uint32_t medal, uint32_t off) {
    Frame f;
    const uint32_t v = f.alloc(4);
    const uint32_t sb = ld32(medal + MEDAL_SB);
    rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + off, 4, off});
    return ld32(v);
}
// an id in a vector<ull> copied from `vec`
bool inIds(uint32_t vec, uint32_t lo, uint32_t hi) {
    Frame f;
    const uint32_t copy = f.alloc(12);
    call(g::VectorULL_copy, {copy, vec});
    bool found = false;
    for (uint32_t p = ld32(copy); p != ld32(copy + 4); p += 8)
        if (ld32(p) == lo && ld32(p + 4) == hi) { found = true; break; }
    call(g::VectorULL_dtor, {copy});
    return found;
}
uint32_t imageButton(uint32_t name, uint32_t len) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    const uint32_t b = call(g::ImageButton_create, {s, 0});
    strFree(s);
    return b;
}
// a heap __func holding one std::function<void(MedalInfoButtonType, bool)> copied from `onButton` at +8, as the
// callable of the std::function at `dst`; the temporary copy `tmp` is destroyed by the caller
void heapButtonFn(Frame &f, uint32_t dst, uint32_t vt, uint32_t onButton, uint32_t &tmp) {
    tmp = f.alloc(20);
    call(g::FunctionInfo_copy, {tmp, onButton});
    heapFunction(dst, vt, 0x20, 8, g::FunctionInfo_move, tmp);
}

int32_t rowOf(uint32_t s) {   // rows scrolled past: scrollY / (cell height + spacing), the original's helpers
    const float step = ldf(s + F_ITEM_H) + ldf(ld32(g::GOT_ROW_SPACING));
    const uint32_t scrolled = call(g::aeabi_fixsfsi, {ld32(s + F_SCROLL_Y)});
    const uint32_t rowStep = call(g::aeabi_fixsfsi, {fbits(step)});
    return int32_t(call(g::aeabi_idiv, {scrolled, rowStep}));
}
inline uint32_t itemCount(uint32_t s) { return uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 3); }

// changeSceneLayer(function<Layer*()> {vt}, true, false, 0)
void changeScene(uint32_t vt) {
    const uint32_t m = sceneManager();
    Frame f;
    const uint32_t layer = newFunction(f, vt, {});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 0, 0});
    functionDestroy(layer);
}
}  // namespace

struct MedalListScrollView {
    static void touchEnded(uint32_t closure, uint32_t touch);
    static void touchMoved(uint32_t functor, uint32_t touchRef);
    static uint32_t touchBegan(uint32_t functor, uint32_t touchRef);
};

// The release lambda {this, index, two words for the tap animation}: the item under the touch (row `index`, from the
// scroll offset); a press that moved less than 20 points picks it - the cell's tap animation, then on-tap(item).
void MedalListScrollView::touchEnded(uint32_t closure, uint32_t touch) {
    const uint32_t s = ld32(closure);
    const int32_t rows = rowOf(s);
    const uint32_t item = ld32(closure + 4) + ld32(s + F_COLUMNS) * rows - 1;
    st32(s + F_PRESS_INDEX, item);
    if (!(itemCount(s) > item) || !ld8(s + F_PRESSED)) {
        st8(s + F_PRESSED, 0);
        return;
    }
    Frame f;
    const uint32_t loc = f.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    if (!(std::fabs(ldf(loc + 4) - ldf(s + F_PRESS_Y)) < 20.0f)) {
        call(g::Vec2_dtor, {loc});
        st8(s + F_PRESSED, 0);
        return;
    }
    const uint32_t at = ld32(s + F_PRESS_INDEX);
    if (!(itemCount(s) > at)) {
        call(g::ThrowOutOfRange, {s + F_ITEM_IDS});
        return;
    }
    const uint32_t idAt = ld32(s + F_ITEM_IDS) + 8 * at;
    const uint32_t lo = ld32(idAt), hi = ld32(idAt + 4);
    call(g::Vec2_dtor, {loc});
    if (!(lo | hi)) {
        st8(s + F_PRESSED, 0);
        return;
    }
    st8(s + F_TAPPED, 1);
    const uint32_t cell = ld32(ld32(s + F_ROWS) + 4 * ld32(closure + 4));
    const uint32_t done = newFunction(f, VT_TapDone, {ld32(closure + 8), ld32(closure + 0xC)});
    const uint32_t anim = call(g::SceneUtil_createTapAnimation, {cell, done});
    call(g::Node_runAction, {cell, anim});
    functionDestroy(done);
    if (ld32(s + F_ON_TAP + 0x10))
        call(g::FunctionVI_call, {s + F_ON_TAP, ld32(s + F_PRESS_INDEX)});
}

// The move lambda __func::operator() {this, index, then the release lambda's captures}: a touch that leaves its
// cell ends the press (and runs the release); one that moved 20 points from where it started counts as a drag.
void MedalListScrollView::touchMoved(uint32_t functor, uint32_t touchRef) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t s = ld32(functor + 4);
    if (!ld8(s + F_PRESSED)) return;
    const uint32_t cell = ld32(ld32(s + F_ROWS) + 4 * ld32(functor + 8));
    Frame f;
    const uint32_t loc = f.alloc(8), local = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {local, cell, loc});
    call(g::Vec2_dtor, {loc});
    const uint32_t w = ld32(vcall(cell, node::getContentSize, {cell}));
    const uint32_t h = ld32(vcall(cell, node::getContentSize, {cell}) + 4);
    rw::call(g::Rect_ctor, {rect, 0, 0, w, h});
    if (!call(g::Rect_containsPoint, {rect, local})) {
        st8(s + F_PRESSED, 0);
        call(g::MedalList_touchEnded, {functor + 0xC, touch});
    }
    if (!ld8(s + F_DRAGGED)) {
        const uint32_t p = f.alloc(8);
        call(g::Touch_getLocation, {p, touch});
        const float startX = ldf(s + F_START), x = ldf(p);
        call(g::Vec2_dtor, {p});
        call(g::Touch_getLocation, {p, touch});
        const float startY = ldf(s + F_START + 4), y = ldf(p + 4);
        call(g::Vec2_dtor, {p});
        if (std::fabs(x - startX) >= 20.0f || !(std::fabs(y - startY) < 20.0f)) st8(s + F_DRAGGED, 1);
    }
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {local});
}

// The press lambda __func::operator() {this, index}: unless a tap is still running, remember where the press
// started; a press on the cell marks it pressed and remembers its item.
uint32_t MedalListScrollView::touchBegan(uint32_t functor, uint32_t touchRef) {
    const uint32_t s = ld32(functor + 4);
    if (ld8(s + F_TAPPED)) return 0;
    const uint32_t touch = ld32(touchRef);
    Frame f;
    const uint32_t loc = f.alloc(8), at = f.alloc(8), local = f.alloc(8), rect = f.alloc(16);
    call(g::Touch_getLocation, {loc, touch});
    st32(s + F_PRESS_Y, ld32(loc + 4));
    call(g::Vec2_dtor, {loc});
    const uint32_t cell = ld32(ld32(s + F_ROWS) + 4 * ld32(functor + 8));
    call(g::Touch_getLocation, {at, touch});
    call(g::Node_convertToNodeSpace, {local, cell, at});
    call(g::Vec2_dtor, {at});
    const uint32_t w = ld32(vcall(cell, node::getContentSize, {cell}));
    const uint32_t h = ld32(vcall(cell, node::getContentSize, {cell}) + 4);
    rw::call(g::Rect_ctor, {rect, 0, 0, w, h});
    uint32_t inside = 0;
    if (call(g::Rect_containsPoint, {rect, local})) {
        inside = 1;
        st32(s + F_HOLD, 0);
        st8(s + F_PRESSED, 1);
        const uint32_t p = f.alloc(8);
        call(g::Touch_getLocation, {p, touch});
        st32(s + F_START, ld32(p));
        st32(s + F_START + 4, ld32(p + 4));
        call(g::Vec2_dtor, {p});
        st8(s + F_DRAGGED, 0);
        st32(s + F_BEGAN_INDEX, ld32(functor + 8) + ld32(s + F_COLUMNS) * rowOf(s) - 1);
    }
    call(g::Vec2_dtor, {rect});
    call(g::Vec2_dtor, {local});
    return inside;
}

struct SceneMedalList {
    static void onTapMedalInfoButton(uint32_t self, uint32_t type, uint32_t, uint32_t, uint32_t idLo, uint32_t idHi);
    static void toEvolve(uint32_t functor);
    static void toMixed(uint32_t functor);
};

// The info popup's buttons: 2 / 3 select the medal in the list; 1 (Evolve) and 0 (Level Up) hand the medal id to
// the scene manager and slide the screen out (then the matching lambda changes the scene).
void SceneMedalList::onTapMedalInfoButton(uint32_t me, uint32_t type, uint32_t, uint32_t, uint32_t idLo, uint32_t idHi) {
    if (type - 2 < 2) {
        call(g::MedalListScrollView_setSelectedMedal, {ld32(me + F_LIST), 0, idLo, idHi, 0});
        return;
    }
    if (type != 1 && type != 0) return;
    const uint32_t m = sceneManager();
    st32(m + SM_MEDAL_ID + 4, idHi);
    st32(m + SM_MEDAL_ID, idLo);
    Frame f;
    const uint32_t done = newFunction(f, type == 1 ? VT_ToEvolve : VT_ToMixed, {});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, done});
    functionDestroy(done);
}

// Evolve: change to the Evolve scene.
void SceneMedalList::toEvolve(uint32_t) { changeScene(VT_EvolveLayer); }

// Level Up: the scene manager's mix flag and an empty callback, the fusion prepare call (no callback), then the
// Level Up scene.
void SceneMedalList::toMixed(uint32_t) {
    const uint32_t m = sceneManager();
    st8(m + SM_MIX_FLAG, 1);
    Frame f;
    const uint32_t empty = newEmptyFunction(f);
    call(g::FunctionVB_assign, {m + SM_MIX_CALLBACK, empty});
    functionDestroy(empty);
    const uint32_t none = newEmptyFunction(f);
    call(g::Mixed_callPrepareAPI, {none});
    functionDestroy(none);
    changeScene(VT_MixedLayer);
}

// SceneUtil (sub_4658D4): a text with the shadow style and a TextData entry.
static uint32_t setShadowText(uint32_t text, uint32_t id) {
    call(g::SceneUtil_setShadow, {text});
    setTextId(text, int32_t(id));
    return 0;
}

struct MedalInfoPopup {
    static void open(uint32_t medalRef, uint32_t owned, uint32_t showMix, uint32_t showFavorite, uint32_t onButton,
                     uint32_t skills, uint32_t onClose, uint32_t okMode);
};

// SceneUtil::openMedalInfoPopup(shared_ptr<UserMedalData>, owned, show the Level Up / Evolve buttons, show the
// favorite and partner boxes, on-button(type, bool), skills (a gacha result: no buttons), on-close, OK mode): the
// shadow and MedalInfoScene on popup layer 2, the medal's details (updateMedalInfo), the favorite box (its state
// from the pending lock / unlock lists), the partner box, the close / Level Up / Evolve (or OK) buttons enabled by
// the medal's flags, the medal tutorial, the open animation and the popup registered with the scene manager.
void MedalInfoPopup::open(uint32_t medalRef, uint32_t owned, uint32_t showMix, uint32_t showFavorite,
                          uint32_t onButton, uint32_t skills, uint32_t onClose, uint32_t okMode) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = SceneUtil::createShadowSprite();
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    uint32_t root;
    {
        Frame f;
        const uint32_t s = newStr(f, S_MedalInfoJson, 45);
        root = SceneUtil::createNodeWithSceneFile(s);
        strFree(s);
    }
    vcall(layer, node::addChildZ, {layer, root, 0});
    {
        const uint32_t outer = findNode(root, S_MedalInfo, 9);
        const uint32_t inner = findNode(outer, S_MedalInfo, 9);
        vcall(inner, node::setAnchorPoint, {inner, ld32(g::GOT_VEC2_ZERO)});
    }
    {
        Frame f;
        const uint32_t copy = f.alloc(8), none = f.alloc(8);
        st32(copy, ld32(medalRef));
        st32(copy + 4, ld32(medalRef + 4));
        if (ld32(copy + 4)) call(fn::shared_add, {ld32(copy + 4)});
        st32(none, 0);
        st32(none + 4, 0);
        call(g::SceneUtil_updateMedalInfo, {root, copy, owned, skills, none, 0});
        if (ld32(copy + 4)) call(fn::shared_release, {ld32(copy + 4)});
    }
    {
        const uint32_t left = findNode(root, S_LeftUI, 6);
        vcall(left, node::setVisible, {left, 0});
        const uint32_t right = findNode(root, S_RightUI, 7);
        vcall(right, node::setVisible, {right, 0});
    }
    const uint32_t closeButton = findNode(root, S_CloseButton, 12);
    const uint32_t favorite = findNode(root, S_FavoriteCheckBox, 17);
    const uint32_t but11 = imageButton(S_But11, 5);
    const uint32_t mixButton = findNode(root, S_ButtonMix, 10);
    uint32_t limitButton = findNode(root, S_ButtonLimitCut, 15);
    uint32_t limitText = findNode(root, S_TxtLimitCut, 12);
    if (okMode == 1) {
        vcall(limitButton, node::setVisible, {limitButton, 0});
        vcall(limitText, node::setVisible, {limitText, 0});
        limitButton = findNode(root, S_ButtonOK, 9);
        limitText = findNode(root, S_TxtOK, 6);
    }
    Frame f;
    // the close callback's captures: the popup's widgets and a copy of on-close
    const uint32_t closeFn = f.alloc(20);
    call(g::FunctionVV_copy, {closeFn, onClose});
    const uint32_t widgets[5] = {closeButton, favorite, but11, mixButton, limitButton};
    const uint32_t closeCopy = f.alloc(20);
    functionCopy(closeCopy, closeFn);
    vcall(favorite, node::setVisible, {favorite, showFavorite});
    const uint32_t medal = ld32(medalRef);
    // the favorite box: changes go to on-button
    {
        const uint32_t medalCopy = f.alloc(8);
        st32(medalCopy, medal);
        st32(medalCopy + 4, ld32(medalRef + 4));
        if (ld32(medalCopy + 4)) call(fn::shared_add, {ld32(medalCopy + 4)});
        const uint32_t tmp = f.alloc(20);
        call(g::FunctionInfo_copy, {tmp, onButton});
        const uint32_t listener = f.alloc(20);
        st32(listener + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x28});
        if (heap) {
            st32(heap, VT_FavoriteChanged);
            st32(heap + 8, ld32(medalCopy));
            st32(heap + 0xC, ld32(medalCopy + 4));
            st32(medalCopy, 0);
            st32(medalCopy + 4, 0);
            call(g::FunctionInfo_move, {heap + 0x10, tmp});
        }
        st32(listener + 16, heap);
        call(g::CheckBox_addEventListener, {favorite, listener});
        functionDestroy(listener);
        functionDestroy(tmp);
        if (ld32(medalCopy + 4)) call(fn::shared_release, {ld32(medalCopy + 4)});
    }
    const uint32_t idLo = ld32(medal + MEDAL_ID), idHi = ld32(medal + MEDAL_ID + 4);
    uint32_t selected;
    if (ld8(medal + MEDAL_LOCKED)) {
        const uint32_t as = call(g::AppState_getInstance, {});
        selected = inIds(call(g::AppState_getWillUnLockUserMedalIDs, {as}), idLo, idHi) ? 0 : 1;
    } else {
        const uint32_t as = call(g::AppState_getInstance, {});
        selected = inIds(call(g::AppState_getWillLockUserMedalIDs, {as}), idLo, idHi) ? 1 : 0;
    }
    call(g::CheckBox_setSelectedState, {favorite, selected});
    // the partner box: the user's partner medal id (+0x58 of the API's partner buffer)
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t partnerCtl = ld32(api + API_PARTNER + 4);
    const uint32_t partnerSb = ld32(api + API_PARTNER);
    if (partnerCtl) call(fn::shared_add, {partnerCtl});
    const uint32_t partner = f.alloc(8);
    rw::call(g::SafeBuffer_read, {partnerSb, partner, ld32(partnerSb + 0xC) + 0x58, 8, 0x58});
    const uint32_t partnerLo = ld32(partner), partnerHi = ld32(partner + 4);
    const uint32_t support = findNode(root, S_SupportCheckBox, 16);
    titleLabel(support, S_TxtPartner, 11, TXT_PARTNER);
    call(g::ImageButton_setTargetImageView, {but11, support});
    call(g::ImageButton_setSelectEnabled, {but11, 1});
    vcall(support, node::addChild, {support, but11});
    vcall(support, node::setVisible, {support, showFavorite});
    const bool notPartner = ((idHi ^ partnerHi) | (idLo ^ partnerLo)) != 0;
    if (!notPartner) {
        call(g::ImageButton_setIsSelected, {but11, 0});
        st8(but11 + IMAGEBUTTON_SELECTABLE, 0);
    } else if (medalRead32(medal, 0x454) != 1) {
        vcall(support, node::setVisible, {support, 0});
        st8(but11 + IMAGEBUTTON_SELECTABLE, 0);
    } else {
        call(g::ImageButton_setIsSelected, {but11, 1});
        const uint32_t fnA = f.alloc(20), fnB = f.alloc(20), fnC = f.alloc(20);
        call(g::FunctionInfo_copy, {fnA, onButton});
        call(g::FunctionInfo_copy, {fnB, onButton});
        const uint32_t medalCopy = f.alloc(8);
        st32(medalCopy, medal);
        st32(medalCopy + 4, ld32(medalRef + 4));
        if (ld32(medalCopy + 4)) call(fn::shared_add, {ld32(medalCopy + 4)});
        functionCopy(fnC, fnA);
        const uint32_t listener = f.alloc(20);
        st32(listener + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x50});
        if (heap) {
            st32(heap, VT_PartnerTouch);
            st32(heap + 8, but11);
            call(g::FunctionInfo_move, {heap + 0x10, fnB});
            st32(heap + 0x28, ld32(medalCopy));
            st32(heap + 0x2C, ld32(medalCopy + 4));
            st32(medalCopy, 0);
            st32(medalCopy + 4, 0);
            st32(heap + 8, but11);
            st32(heap + 0x30, but11);
            st32(heap + 0x34, root);
            call(g::FunctionInfo_move, {heap + 0x38, fnC});
            st32(heap + 8, but11);
        }
        st32(listener + 16, heap);
        call(g::ImageButton_addTouchEventListener, {but11, listener});
        functionDestroy(listener);
        functionDestroy(fnC);
        if (ld32(medalCopy + 4)) call(fn::shared_release, {ld32(medalCopy + 4)});
        functionDestroy(fnB);
        functionDestroy(fnA);
    }
    {
        Frame g2;
        const uint32_t cb = newFunction(g2, VT_CloseTouch, {});
        call(g::Widget_addTouchEventListener, {closeButton, cb});
        functionDestroy(cb);
    }
    {
        const uint32_t as = call(g::AppState_getInstance, {});
        if (call(g::AppState_isClearTutorialPopupIDs, {as, TUTORIAL_MEDAL_INFO})) {
            const uint32_t as2 = call(g::AppState_getInstance, {});
            Frame g2;
            const uint32_t none = newEmptyFunction(g2), popup = g2.alloc(20), closer = g2.alloc(20);
            call(g::SceneUtil_openTutorialPopup, {popup, TUTORIAL_MIX, none, mixButton, 0, 0, 0});
            call(g::AppState_setTutorialCloseFunc, {closer, as2, popup, TUTORIAL_MIX});
            functionDestroy(closer);
            functionDestroy(popup);
            functionDestroy(none);
        }
    }
    {
        Frame g2;
        uint32_t tmp;
        const uint32_t listener = g2.alloc(20);
        heapButtonFn(g2, listener, VT_MixTouch, onButton, tmp);
        call(g::Widget_addTouchEventListener, {mixButton, listener});
        functionDestroy(listener);
        functionDestroy(tmp);
    }
    const uint32_t mixText = findNode(root, S_TxtMix, 7);
    SceneUtil::setShadow(mixText);
    setTextId(mixText, TXT_MIX);
    const uint32_t canMix = medalRead32(medal, 0x454);
    if (!skills && canMix == 1) {
        vcall(mixButton, node::setVisible, {mixButton, 1});
        call(g::Widget_setBright, {mixButton, 1});
        vcall(mixButton, V_setTouchEnabledW, {mixButton, 1});
        vcall(mixText, node::setColor, {mixText, ld32(g::GOT_COLOR3B_WHITE)});
        SceneUtil::setShadow(mixText);
    } else {
        vcall(mixButton, node::setVisible, {mixButton, 0});
        call(g::Widget_setBright, {mixButton, 0});
        vcall(mixButton, V_setTouchEnabledW, {mixButton, 0});
        call(g::SceneUtil_setTextDisabled, {mixText, 0, 0});
    }
    vcall(mixButton, node::setVisible, {mixButton, showMix});
    {
        Frame g2;
        uint32_t tmp;
        const uint32_t listener = g2.alloc(20);
        heapButtonFn(g2, listener, VT_LimitCutTouch, onButton, tmp);
        call(g::Widget_addTouchEventListener, {limitButton, listener});
        functionDestroy(listener);
        functionDestroy(tmp);
    }
    SceneUtil::setShadow(limitText);
    setTextId(limitText, TXT_LIMIT_CUT);
    if (!skills && medalRead32(medal, 0x4D8) == 1) {
        call(g::Widget_setBright, {limitButton, 1});
        vcall(limitButton, V_setTouchEnabledW, {limitButton, 1});
        vcall(limitText, node::setColor, {limitText, ld32(g::GOT_COLOR3B_WHITE)});
        SceneUtil::setShadow(limitText);
        vcall(limitButton, node::setVisible, {limitButton, showMix | okMode});
        vcall(limitText, node::setVisible, {limitText, showMix | okMode});
    } else {
        call(g::Widget_setBright, {limitButton, 0});
        vcall(limitButton, V_setTouchEnabledW, {limitButton, 0});
        call(g::SceneUtil_setTextDisabled, {limitText, 0, 1});
        vcall(limitButton, node::setVisible, {limitButton, 0});
        vcall(limitText, node::setVisible, {limitText, 0});
    }
    {
        const uint32_t ok = findNode(root, S_ButtonOK, 9);
        vcall(ok, node::setVisible, {ok, okMode});
    }
    // the open animation's flag: a Level Up hint unless the medal is past the tutorial's level
    const uint32_t misc = f.alloc(8);
    call(g::TutorialMisc_create, {misc, 4});
    int32_t hintLevel = 0;
    if (ld32(misc)) {
        const uint32_t v = f.alloc(4);
        const uint32_t sb = ld32(misc);
        rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + 0xC, 4, 0xC});
        hintLevel = int32_t(ld32(v));
    }
    uint32_t hint = 0;
    if (notPartner && showMix == 1 && canMix == 1) hint = hintLevel > int32_t(medalRead32(medal, 0x470)) ? 0 : 1;
    {
        Frame g2;
        const uint32_t done = g2.alloc(20);
        st32(done, VT_OpenDone);
        st8(done + 4, hint);
        st32(done + 16, done);
        call(g::Node_runAction, {root, SceneUtil::createOpenPopupAnimation(root, done)});
        functionDestroy(done);
    }
    call(g::AppState_removeNewUserMedalID, {call(g::AppState_getInstance, {}), medal + 8, idLo, idHi});
    const uint32_t sm = sceneManager();
    const uint32_t openFn = newFunction(f, VT_PopupOpen, {closeButton});
    // openPopup's close callback: the widgets, root / layer / shadow and a copy of on-close (heap 0x48)
    const uint32_t closeTmp = f.alloc(20);
    functionCopy(closeTmp, closeCopy);
    const uint32_t closeCb = f.alloc(20);
    st32(closeCb + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x48});
    if (heap) {
        st32(heap, VT_PopupClose);
        for (uint32_t k = 0; k < 5; ++k) st32(heap + 8 + 4 * k, widgets[k]);
        st32(heap + 0x20, root);
        st32(heap + 0x24, layer);
        st32(heap + 0x28, shadow);
        call(g::FunctionVV_move, {heap + 0x30, closeTmp});
        for (uint32_t k = 0; k < 5; ++k) st32(heap + 8 + 4 * k, widgets[k]);
    }
    st32(closeCb + 16, heap);
    call(g::SM_openPopup, {sm, openFn, closeCb, 1});
    functionDestroy(closeCb);
    functionDestroy(closeTmp);
    functionDestroy(openFn);
    if (ld32(misc + 4)) call(fn::shared_release, {ld32(misc + 4)});
    if (partnerCtl) call(fn::shared_release, {partnerCtl});
    functionDestroy(closeCopy);
    functionDestroy(closeFn);
}

RW_FUNCTION(0x466EF4, "SceneUtil::openMedalInfoPopup", MedalInfoPopup::open);

// The Medal List's list taps open the info popup the same way: once at a time - two widgets stop taking touches,
// the decide sound, the medal's data, and the popup (owned medal: the user flag at +0x54; Level Up / Evolve shown)
// with its button callback {this, medal id} (heap) and close callback {this}. `pickId` runs after the sound.
template <typename PickId>
void openListMedalInfo(uint32_t me, uint32_t vtButton, uint32_t vtClosed, PickId pickId) {
    st8(me + F_CAN_TAP, 0);
    for (uint32_t at : {F_TOUCH_A, F_TOUCH_B}) {
        const uint32_t w = ld32(me + at);
        vcall(w, V_setTouchEnabled, {w, 0});
    }
    call(g::AudioPlaySystemSEDecide, {});
    uint32_t idLo, idHi;
    pickId(idLo, idHi);
    const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
    Frame f;
    const uint32_t medal = f.alloc(8), arg = f.alloc(8), flag = f.alloc(4);
    // getMedalData(out, manager, id, 0, UserMedal::Medal{} by value: 72 bytes on the stack)
    call(g::MedalDataManager_getMedalData, {medal, mdm, idLo, idHi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0});
    const uint32_t api = call(g::APIManager_getInstance, {});
    st32(arg, ld32(medal));
    st32(arg + 4, ld32(medal + 4));
    if (ld32(arg + 4)) call(fn::shared_add, {ld32(arg + 4)});
    const uint32_t sb = ld32(api + API_USER);
    rw::call(g::SafeBuffer_read, {sb, flag, ld32(sb + 0xC) + 0x54, 1, 0x54});
    const uint32_t owned = ld8(flag);
    const uint32_t onButton = f.alloc(24);
    st32(onButton + 0x10, 0);
    const uint32_t heap = call(fn::operator_new, {0x18});
    if (heap) {
        st32(heap, vtButton);
        st32(heap + 8, me);
        st32(heap + 0xC, 0);
        st32(heap + 0x10, idLo);
        st32(heap + 0x14, idHi);
    }
    st32(onButton + 0x10, heap);
    const uint32_t onClose = newFunction(f, vtClosed, {me});
    call(g::SceneUtil_openMedalInfoPopup, {arg, owned ? 1u : 0u, 1, 1, onButton, 0, onClose, 0});
    functionDestroy(onClose);
    functionDestroy(onButton);
    if (ld32(arg + 4)) call(fn::shared_release, {ld32(arg + 4)});
    if (ld32(medal + 4)) call(fn::shared_release, {ld32(medal + 4)});
}

struct SceneMedalListTaps {
    static void onLongPress(uint32_t functor, uint32_t idRef);
    static void onTap(uint32_t functor, uint32_t indexRef);
    static void onClosed(uint32_t functor);
    static void onButton(uint32_t functor, uint32_t typeRef);
    static uint32_t cloneButton(uint32_t functor);
};

// init $_5 (a medal by id: the long press)
void SceneMedalListTaps::onLongPress(uint32_t functor, uint32_t idRef) {
    const uint32_t me = ld32(functor + 4);
    if (!ld8(me + F_CAN_TAP)) return;
    const uint32_t lo = ld32(idRef), hi = ld32(idRef + 4);
    openListMedalInfo(me, VT_LongPressButton, VT_LongPressClosed, [&](uint32_t &idLo, uint32_t &idHi) {
        idLo = lo;
        idHi = hi;
    });
}

// init $_3 (a medal by list index: the tap), the id from a copy of the list's ids
void SceneMedalListTaps::onTap(uint32_t functor, uint32_t indexRef) {
    const uint32_t me = ld32(functor + 4);
    if (!ld8(me + F_CAN_TAP)) return;
    const uint32_t index = ld32(indexRef);
    Frame f;
    const uint32_t ids = f.alloc(12);
    openListMedalInfo(me, VT_TapButton, VT_TapClosed, [&](uint32_t &idLo, uint32_t &idHi) {
        call(g::VectorULL_copy, {ids, ld32(me + F_LIST) + LIST_ITEM_IDS});
        const uint32_t at = ld32(ids) + 8 * index;
        idLo = ld32(at);
        idHi = ld32(at + 4);
    });
    call(g::VectorULL_dtor, {ids});
}

// the popup's close callback {this}: the list and its widget take touches again
void SceneMedalListTaps::onClosed(uint32_t functor) {
    const uint32_t me = ld32(functor + 4);
    st8(me + F_CAN_TAP, 1);
    for (uint32_t at : {F_TOUCH_A, F_TOUCH_B}) {
        const uint32_t w = ld32(me + at);
        vcall(w, V_setTouchEnabled, {w, 1});
    }
}

// the popup's button callback {this, medal id}: onTapMedalInfoButton(type, false, id)
void SceneMedalListTaps::onButton(uint32_t functor, uint32_t typeRef) {
    const uint32_t me = ld32(functor + 8);
    call(g::SceneMedalList_onTapMedalInfoButton, {me, ld32(typeRef), 0, me, ld32(functor + 0x10), ld32(functor + 0x14)});
}

// init $_3's button callback __func::__clone(): a heap copy of the four capture words
uint32_t SceneMedalListTaps::cloneButton(uint32_t functor) {
    const uint32_t copy = call(fn::operator_new, {0x18});
    if (copy) {
        st32(copy, VT_TapButton);
        for (uint32_t k = 8; k < 0x18; k += 4) st32(copy + k, ld32(functor + k));
    }
    return copy;
}

RW_FUNCTION(0x73E4D4, "SceneMedalList::init $_5 __func::operator() (sub_73E4D4)", SceneMedalListTaps::onLongPress);
RW_FUNCTION(0x73EC78, "SceneMedalList::init $_3 __func::operator() (sub_73EC78)", SceneMedalListTaps::onTap);
RW_FUNCTION(0x73E7AA, "SceneMedalList::init $_5 close lambda __func::operator() (sub_73E7AA)", SceneMedalListTaps::onClosed);
RW_FUNCTION(0x73EF7E, "SceneMedalList::init $_3 close lambda __func::operator() (sub_73EF7E)", SceneMedalListTaps::onClosed);
RW_FUNCTION(0x73E882, "SceneMedalList::init $_5 button lambda __func::operator() (sub_73E882)", SceneMedalListTaps::onButton);
RW_FUNCTION(0x73F056, "SceneMedalList::init $_3 button lambda __func::operator() (sub_73F056)", SceneMedalListTaps::onButton);
RW_FUNCTION(0x73EFEC, "SceneMedalList::init $_3 button lambda __func::__clone() (sub_73EFEC)", SceneMedalListTaps::cloneButton);
RW_FUNCTION(0x4658D4, "SceneUtil shadow text from TextData (sub_4658D4)", setShadowText);
RW_FUNCTION(0x73DB0C, "SceneMedalList::onTapMedalInfoButton", SceneMedalList::onTapMedalInfoButton);
RW_FUNCTION(0x73DF48, "SceneMedalList Evolve lambda __func::operator() (sub_73DF48)", SceneMedalList::toEvolve);
RW_FUNCTION(0x73E108, "SceneMedalList Level Up lambda __func::operator() (sub_73E108)", SceneMedalList::toMixed);
RW_FUNCTION(0x59B380, "MedalListScrollView release lambda (sub_59B380)", MedalListScrollView::touchEnded);
RW_FUNCTION(0x59B69C, "MedalListScrollView move lambda __func::operator() (sub_59B69C)", MedalListScrollView::touchMoved);
RW_FUNCTION(0x59B8F8, "MedalListScrollView press lambda __func::operator() (sub_59B8F8)", MedalListScrollView::touchBegan);
