// DrawScrollView: the bonds (Kizuna) draw. openKizunaPopup(gacha) checks the player's bonds points against the
// draw's cost (up to ten draws at once): not enough -> an OK dialog and the error sound; the medal box full -> the
// medal over dialog; else openCheckbondsPopup (PopupNormal_P_DrawCheck: the cost and count, the points held,
// Cancel through SceneManager::openPopup and Draw). The popup's close helper (sub_5B15E8) and its Draw button
// (sub_5B1518) are here too, with hasBondsPoint. openKizunaPopup also copies the gacha into a local vector it
// never reads (as the original does).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09, AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_getPopupLayer2 = 0x613E71, SM_openPopup = 0x61524D, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_textToCustomTextView = 0x45BF15;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_getMoneyStr = 0x461DC5;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775, SceneUtil_openMedalOverDialog = 0x460251;
constexpr uint32_t SceneUtil_isOverMedalSlot = 0x46A085;
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t FadeTo_create = 0x86B3E1, EaseSineOut_create = 0x860D09;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t AudioManager_getInstance = 0x5BECFD, AudioManager_getLayer = 0x5BF33D;
constexpr uint32_t AudioLayer_playSE = 0x5BF785;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_getTutorialCloseFunc = 0x4EBA3D, AppState_execTutorialCloseFunc = 0x4EBA5D;
constexpr uint32_t GachaData_copy = 0x5AE2CD, GachaVector_pushSlow = 0x5B2909, GachaVector_dtor = 0x5B133D;
constexpr uint32_t DrawScrollView_hasBondsPoint = 0x5B07B5, DrawScrollView_openCheckbondsPopup = 0x5B0855;
constexpr uint32_t CloseCheckbonds = 0x5B15E9;   // sub_5B15E8
constexpr uint32_t STRLEN = 0x40E08C;
}  // namespace g

constexpr uint32_t S_DrawCheckJson = 0xE00965;   // "cocostudio/publish/PopupNormal_P_DrawCheck.json"
constexpr uint32_t S_Txt_Wording = 0xDF862F, S_Txt_DrawNum = 0xE00944, S_Empty = 0xDF8A1A;
constexpr uint32_t S_BondsConsumption = 0xE00995, S_BondsConsumptionLabel = 0xE009AA, S_FmtD = 0xDF86DE;
constexpr uint32_t S_BondsPossessed = 0xE009C5, S_BondsPossessedLabel = 0xE009D8, S_Txt_Cancel = 0xDF86A6;
constexpr uint32_t S_Button_Close = 0xDF8692, S_Txt_Draw = 0xE0095C, S_Button_Draw = 0xE00950;
constexpr int32_t TXT_NOT_ENOUGH = 102600014, TXT_CHECK = 102600050, TXT_CONSUMPTION = 100320003;
constexpr int32_t TXT_POSSESSED = 100320002, TXT_CANCEL = 100100002, TXT_DRAW = 102600001;
constexpr uint32_t SE_ERROR = 509000180, TUTORIAL_BONDS = 109;

// __func vtables (+8)
constexpr uint32_t VT_NotEnoughOk = 0xF5C770, VT_MedalOver = 0xF5C5B8;   // {this}
constexpr uint32_t VT_Checkbonds = 0xF5C560;   // {gacha id, count, this}
constexpr uint32_t VT_Close = 0xF5C458, VT_CanClose = 0xF5C42C;          // {} / {close}
constexpr uint32_t VT_Result = 0xF5C400, VT_Draw = 0xF5C3A8;             // {popup, layer, shadow, this, function}
constexpr uint32_t VT_Closed = 0xF5C3D4;       // the close animation's end (0x38)

// GachaData (0x40): strings at +0, +0xC, +0x24; +0x18 id, +0x1C points per draw, +0x20 draws
constexpr uint32_t GD_ID = 0x18, GD_COST = 0x1C, GD_DRAWS = 0x20;
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C, V_ADD_CHILD_Z = 0xF4, V_GET_POSITION = 0x54;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// the player's bonds points (user data +0x1C), with the user data's shared_ptr held
int32_t bondsPoints(uint32_t &ctl, uint32_t out) {
    const uint32_t a = call(fn::APIManager_getInstance, {});
    ctl = ld32(a + 0x90);
    const uint32_t sb = ld32(a + 0x8C);
    if (ctl) call(fn::shared_add, {ctl});
    call(g::SafeBuffer_read, {sb, out, ld32(sb + 0xC) + 0x1C, 4, 0x1C});
    return int32_t(ld32(out));
}
void errorSound() {
    Frame f;
    const uint32_t layer = f.alloc(8);
    call(g::AudioManager_getLayer, {layer, call(g::AudioManager_getInstance, {}), 10000});
    call(g::AudioLayer_playSE, {ld32(layer), SE_ERROR, 0, 0});
    releaseIf(ld32(layer + 4));
}

uint32_t hasBondsPoint(uint32_t, uint32_t cost) {
    Frame f;
    const uint32_t out = f.alloc(4);
    uint32_t ctl;
    const int32_t points = bondsPoints(ctl, out);
    releaseIf(ctl);
    return points >= int32_t(cost) ? 1 : 0;
}

uint32_t openKizunaPopup(uint32_t self, uint32_t gacha) {
    Frame f;
    const uint32_t out = f.alloc(4), local = f.alloc(0x40), list = f.alloc(12);
    uint32_t ctl;
    const int32_t points = bondsPoints(ctl, out);
    st32(list, 0);
    st32(list + 4, 0);
    st32(list + 8, 0);
    call(g::GachaData_copy, {local, gacha});
    {
        const int32_t unit = int32_t(ld32(local + GD_COST));
        int32_t n = points / unit;
        if (n > 10) n = 10;
        st32(local + GD_COST, uint32_t(unit * n));
        if (n > 1) {
            const uint32_t end = ld32(list + 4);
            if (end == ld32(list + 8)) {
                call(g::GachaVector_pushSlow, {list, local});
            } else {
                if (end) call(g::GachaData_copy, {end, local});
                st32(list + 4, (end ? ld32(list + 4) : 0) + 0x40);
            }
        }
    }
    int32_t cost = int32_t(ld32(gacha + GD_COST));
    int32_t count = points / cost;
    if (count > 10) count = 10;
    const uint32_t id = ld32(gacha + GD_ID);
    if (count < 2) count = int32_t(ld32(gacha + GD_DRAWS));
    else cost *= count;
    if (call(g::DrawScrollView_hasBondsPoint, {gacha, uint32_t(cost)}) != 1) {
        Frame t;
        const uint32_t text = textData(t, TXT_NOT_ENOUGH);
        const uint32_t s = newStr(t, textDataStr(text));
        const uint32_t fo = newFunction(t, VT_NotEnoughOk, {self});
        call(g::SceneUtil_openOkDialog4Line, {s, fo, 0});
        functionDestroy(fo);
        strFree(s);
        releaseIf(ld32(text + 4));
        errorSound();
    } else if (call(g::SceneUtil_isOverMedalSlot, {1}) == 1) {
        Frame t;
        const uint32_t fo = newFunction(t, VT_MedalOver, {self});
        call(g::SceneUtil_openMedalOverDialog, {1, 3, fo});
        functionDestroy(fo);
        errorSound();
    } else {
        Frame t;
        const uint32_t fo = newFunction(t, VT_Checkbonds, {id, uint32_t(count), self});
        call(g::DrawScrollView_openCheckbondsPopup, {self, uint32_t(count), uint32_t(cost), fo});
        functionDestroy(fo);
    }
    strFree(local + 0x24);
    strFree(local + 0xC);
    strFree(local);
    call(g::GachaVector_dtor, {list});
    releaseIf(ctl);
    return 0;
}

// the closures' captures {popup, layer, shadow, this} and a function<void()> copied (inline clone) after them
void cloneCaptures(uint32_t dst, uint32_t src) {
    for (uint32_t k = 0; k < 0x10; k += 4) st32(dst + k, ld32(src + k));
}
uint32_t heapClosure(uint32_t vt, uint32_t words, uint32_t fn) {
    const uint32_t h = call(fn::operator_new, {0x30});
    if (h) {
        st32(h, vt);
        for (uint32_t k = 0; k < 0x10; k += 4) st32(h + 8 + k, ld32(words + k));
        call(fn::function_vv_move, {h + 0x18, fn});
    }
    return h;
}

uint32_t openCheckbondsPopup(uint32_t self, uint32_t count, uint32_t cost, uint32_t done) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_DrawCheckJson});
    vcall(popup, V_SET_TOUCH_ENABLED, {popup, 0});
    vcall(shadow, V_ADD_CHILD_Z, {shadow, popup, 3});
    {
        const uint32_t setAnchor = ld32(ld32(popup) + node::setAnchorPoint);
        Frame t;
        const uint32_t v = t.alloc(8);
        call(g::Vec2_ctor, {v, fbits(0.5f), fbits(0.5f)});
        call(setAnchor, {popup, v});
        call(g::Vec2_dtor, {v});
    }
    {
        const uint32_t setPos = ld32(ld32(popup) + node::setPosition);
        call(setPos, {popup, vcall(shadow, V_GET_POSITION, {shadow})});
    }
    Frame f;
    const uint32_t lam = f.alloc(0x24);
    st32(lam, popup);
    st32(lam + 4, layer);
    st32(lam + 8, shadow);
    st32(lam + 0xC, self);
    call(fn::function_vv_copy, {lam + 0x10, done});
    const uint32_t keepCtl = ld32(call(fn::APIManager_getInstance, {}) + 0xA8);
    if (keepCtl) call(fn::shared_add, {keepCtl});
    {
        const uint32_t text = findNode(popup, S_Txt_Wording, 11);
        Frame t;
        const uint32_t name = newStr(t, S_Txt_Wording, 11);
        const uint32_t view = call(g::SceneUtil_textToCustomTextView, {text, name});
        strFree(name);
        const uint32_t data = textData(t, TXT_CHECK);
        const uint32_t buf = stringBuffer(t, {textDataStr(data), cost, count});
        const uint32_t s = newStr(t, buf, call(g::STRLEN, {buf}));
        call(g::CustomTextView_setXml, {view, s});
        strFree(s);
        releaseIf(ld32(data + 4));
    }
    {
        const uint32_t num = findNode(popup, S_Txt_DrawNum, 11);
        Frame t;
        const uint32_t s = newStr(t, S_Empty, 0);
        call(fn::Text_setString, {num, s});
        strFree(s);
    }
    {
        Frame t;
        const uint32_t s = newStr(t, S_BondsConsumption, 0x14);
        call(g::SceneUtil_settingTitleLabel, {popup, s, uint32_t(TXT_CONSUMPTION)});
        strFree(s);
    }
    {
        const uint32_t label = findNode(popup, S_BondsConsumptionLabel, 0x1A);
        Frame t;
        const uint32_t buf = stringBuffer(t, {S_FmtD, cost});
        const uint32_t s = newStr(t, buf, call(g::STRLEN, {buf}));
        call(fn::Text_setString, {label, s});
        strFree(s);
    }
    uint32_t pointsCtl;
    int32_t points;
    {
        Frame t;
        points = bondsPoints(pointsCtl, t.alloc(4));
    }
    {
        Frame t;
        const uint32_t s = newStr(t, S_BondsPossessed, 0x12);
        call(g::SceneUtil_settingTitleLabel, {popup, s, uint32_t(TXT_POSSESSED)});
        strFree(s);
    }
    {
        const uint32_t label = findNode(popup, S_BondsPossessedLabel, 0x18);
        Frame t;
        const uint32_t money = t.alloc(12);
        call(g::SceneUtil_getMoneyStr, {money, 0, uint32_t(points), uint32_t(points >> 31)});
        call(fn::Text_setString, {label, money});
        strFree(money);
    }
    {
        Frame t;
        const uint32_t s = newStr(t, S_Txt_Cancel, 10);
        call(g::SceneUtil_settingTitleLabel, {popup, s, uint32_t(TXT_CANCEL)});
        strFree(s);
    }
    const uint32_t close = findNode(popup, S_Button_Close, 12);
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_Close, {});
        call(g::Widget_addTouchEventListener, {close, fo});
        functionDestroy(fo);
    }
    // a copy of the captures for the popup's result, kept to the end
    const uint32_t lam2 = f.alloc(0x24);
    cloneCaptures(lam2, lam);
    functionCopy(lam2 + 0x10, lam + 0x10);
    {
        const uint32_t sm = sceneManager();
        Frame t;
        const uint32_t check = newFunction(t, VT_CanClose, {close});
        const uint32_t lam3 = t.alloc(0x10), fn3 = t.alloc(20);
        cloneCaptures(lam3, lam2);
        functionCopy(fn3, lam2 + 0x10);
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        st32(result + 16, heapClosure(VT_Result, lam3, fn3));
        call(g::SM_openPopup, {sm, check, result, 1});
        functionDestroy(result);
        functionDestroy(fn3);
        functionDestroy(check);
    }
    {
        Frame t;
        const uint32_t s = newStr(t, S_Txt_Draw, 8);
        call(g::SceneUtil_settingTitleLabel, {popup, s, uint32_t(TXT_DRAW)});
        strFree(s);
    }
    const uint32_t draw = findNode(popup, S_Button_Draw, 11);
    {
        Frame t;
        const uint32_t lam4 = t.alloc(0x10), fn4 = t.alloc(20);
        cloneCaptures(lam4, lam);
        functionCopy(fn4, lam + 0x10);
        const uint32_t fo = t.alloc(20);
        st32(fo + 16, 0);
        st32(fo + 16, heapClosure(VT_Draw, lam4, fn4));
        call(g::Widget_addTouchEventListener, {draw, fo});
        functionDestroy(fo);
        functionDestroy(fn4);
    }
    {
        Frame t;
        const uint32_t none = newEmptyFunction(t);
        const uint32_t open = call(g::SceneUtil_createOpenPopupAnimation, {popup, none});
        call(fn::Node_runAction, {popup, call(g::EaseSineOut_create, {open})});
        functionDestroy(none);
    }
    functionDestroy(lam2 + 0x10);
    releaseIf(pointsCtl);
    releaseIf(keepCtl);
    functionDestroy(lam + 0x10);
    return 0xA34;
}

// the Draw button {-, popup, layer, shadow, this, function}(sender, type): ended -> close the popup (drawing),
// then the tutorial's close function for the bonds draw (109) when one is set
uint32_t drawButton(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    call(g::SM_closeCurrentPopup, {sceneManager(), 1});
    call(g::AudioPlaySystemSEDecide, {});
    call(g::CloseCheckbonds, {self + 8, 1});
    Frame f;
    const uint32_t fo = f.alloc(20);
    call(g::AppState_getTutorialCloseFunc, {fo, call(g::AppState_getInstance, {}), TUTORIAL_BONDS});
    const uint32_t p = ld32(fo + 16);
    if (!p) return 0;
    functionDestroy(fo);
    call(g::AppState_execTutorialCloseFunc, {call(g::AppState_getInstance, {}), TUTORIAL_BONDS});
    return 0;
}

// close the popup {popup, layer, shadow, this, function}: the close animation (eased) with its end
// {layer, shadow, popup, this, draw, function}, the shadow fading out
uint32_t closeCheckbonds(uint32_t lam, uint32_t draw) {
    const uint32_t popup = ld32(lam);
    Frame f;
    const uint32_t l = f.alloc(0x18), fnc = f.alloc(20);
    st32(l, ld32(lam + 4));
    st32(l + 4, ld32(lam + 8));
    st32(l + 8, popup);
    st32(l + 0xC, ld32(lam + 0xC));
    st8(l + 0x10, uint8_t(draw));
    call(fn::function_vv_copy, {fnc, lam + 0x10});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x38});
    if (h) {
        st32(h, VT_Closed);
        for (uint32_t k = 0; k < 0x10; k += 4) st32(h + 8 + k, ld32(l + k));
        st8(h + 0x18, ld8(l + 0x10));
        call(fn::function_vv_move, {h + 0x20, fnc});
    }
    st32(fo + 16, h);
    const uint32_t close = call(g::SceneUtil_createClosePopupAnimation, {popup, fo});
    call(fn::Node_runAction, {popup, call(g::EaseSineOut_create, {close})});
    functionDestroy(fo);
    functionDestroy(fnc);
    const uint32_t shadow = ld32(lam + 8);
    call(fn::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), 0})});
    return 0;
}
}  // namespace

RW_FUNCTION(0x5B0410, "DrawScrollView::openKizunaPopup(GachaData)", openKizunaPopup);
RW_FUNCTION(0x5B07B4, "DrawScrollView::hasBondsPoint(int)", hasBondsPoint);
RW_FUNCTION(0x5B0854, "DrawScrollView::openCheckbondsPopup(int, int, function<void()>)", openCheckbondsPopup);
RW_FUNCTION(0x5B1518, "DrawScrollView::openCheckbondsPopup Draw button (sub_5B1518)", drawButton);
RW_FUNCTION(0x5B15E8, "DrawScrollView::openCheckbondsPopup close (sub_5B15E8)", closeCheckbonds);
