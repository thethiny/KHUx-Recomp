// Shop > a gacha's draw button: DrawScrollView::openRaisePopup (enough magic stones? then the medal-slot check and
// the "draw?" confirmation, else the "not enough" dialog), hasMagicStone, openCheckRaisePopup (the confirmation popup:
// the cost text, the stones owned, Close / Draw) and the popup's lambdas: the close animation (sub_5B1E5C) and the
// two closures it carries (clone / destroy / call).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;               // FadeTo::create(float duration, uint8 opacity)
constexpr uint32_t Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t TextData_format = 0x492929;             // (string out, Type, id, ...)
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;    // (node, function<void()> by value)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;   // (node, function<void()> by value)
constexpr uint32_t SceneUtil_textToCustomTextView = 0x45BF15;       // (Text*, const string& name)
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;          // (node, const string& name, text id)
constexpr uint32_t SceneUtil_isOverMedalSlot = 0x46A085;
constexpr uint32_t SceneUtil_openMedalOverDialog = 0x460251;        // (bool, BackScene, function<void(bool)> by value)
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;    // (const string&, function<void(bool)>, const char*, const char*)
constexpr uint32_t SM_getPopupLayer2 = 0x613E71;
constexpr uint32_t SM_openPopup = 0x61524D;                // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD, audio_Manager_getLayer = 0x5BF33D;   // (out, mgr, id)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;          // (layer, ResourceID, int, int)
constexpr uint32_t FunctionVV_call = 0x470E69;             // std::function<void()>::operator()()
constexpr uint32_t GachaData_copy = 0x5AE2CD;              // DrawScrollView::GachaData(const GachaData&)
constexpr uint32_t hasMagicStone = 0x5AF979, openCheckRaisePopup = 0x5AFA31;
}  // namespace g

// __func vtables (+8)
constexpr uint32_t VT_MedalOver = 0xF5C668;    // openRaisePopup {this}
constexpr uint32_t VT_NoStone = 0xF5C79C;      // openRaisePopup {this}
constexpr uint32_t VT_RaiseOk = 0xF5C5E4;      // openRaisePopup {GachaData, this} (heap 0x48)
constexpr uint32_t VT_CloseButton = 0xF5C534;  // openCheckRaisePopup {draw button}
constexpr uint32_t VT_PopupCheck = 0xF5C508;   // openCheckRaisePopup function<bool()> {close button}
constexpr uint32_t VT_PopupResult = 0xF5C4DC;  // {widget, layer, shadow, this, function<void()>} (heap 0x30)
constexpr uint32_t VT_DrawButton = 0xF5C484;   // {widget, layer, shadow, this, function<void()>} (heap 0x30)
constexpr uint32_t VT_Closed = 0xF5C4B0;       // {layer, shadow, widget, this, bool, function<void()>} (heap 0x38)
// strings
constexpr uint32_t S_PopupJson = 0xE00914;     // "cocostudio/publish/PopupNormal_R_DrawCheck.json"
constexpr uint32_t S_TxtWording = 0xDF862F, S_TxtDrawNum = 0xE00944, S_Empty = 0xDF8A1A;
constexpr uint32_t S_TxtCrownPossessed = 0xDF8658, S_TxtCrownPossessedLabel = 0xDF866B, S_FmtD = 0xDF86DE;   // "%d"
constexpr uint32_t S_ButtonClose = 0xDF8692, S_ButtonDraw = 0xE00950, S_TxtCancel = 0xDF86A6, S_TxtDraw = 0xE0095C;
constexpr int32_t TXT_NO_STONE = 102600016, TXT_CHECK = 102602049, TXT_CROWN = 100310002, TXT_CANCEL = 100100002;
constexpr int32_t TXT_DRAW = 102600001;
constexpr uint32_t SE_ERROR = 509000180;
constexpr uint32_t API_STONES = 0xA4;          // SafeBuffer: paid stones at 0, free at 4
constexpr uint32_t GACHA_SIZE = 0x40;          // DrawScrollView::GachaData; +0x1C cost, +0x20 draws
constexpr uint32_t F_POPUP_OPEN = 0x650;       // DrawScrollView: a popup is open
constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214, V_widgetSetTouchEnabled = 0x29C, V_removeChild = 0x12C;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
void playErrorSE() {
    const uint32_t mgr = call(g::audio_Manager_getInstance, {});
    Frame f;
    const uint32_t layer = f.alloc(8);
    call(g::audio_Manager_getLayer, {layer, mgr, 10000});
    rw::call(g::audio_Layer_playSE, {ld32(layer), SE_ERROR, 0, 0});
    releaseIf(ld32(layer + 4));
}
// a GachaData's three strings (+0, +0xC, +0x24) freed
void freeGacha(uint32_t gd) {
    strFree(gd + 0x24);
    strFree(gd + 0xC);
    strFree(gd);
}
// a closure {widget, layer, shadow, this, function<void()>} at `src` copied into a heap __func `vtable` (0x30):
// the four words, then the function moved from a copy
uint32_t heapClosure(uint32_t dst, uint32_t vtable, uint32_t words, uint32_t fnCopy) {
    st32(dst + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x30});
    if (obj) {
        st32(obj, vtable);
        for (uint32_t k = 0; k < 16; k += 4) st32(obj + 8 + k, ld32(words + k));
        call(fn::function_vv_move, {obj + 0x18, fnCopy});
    }
    st32(dst + 16, obj);
    return obj;
}
}  // namespace

struct DrawRaise {
    static void openRaisePopup(uint32_t self, uint32_t gacha);
    static uint32_t hasMagicStone(uint32_t self, int32_t cost);
    static void openCheckRaisePopup(uint32_t self, int32_t draws, int32_t cost, uint32_t done);
    static void closePopup(uint32_t closure, uint32_t run);
    static uint32_t cloneClosed(uint32_t functor);
    static void destroyClosed(uint32_t functor);
    static void callClosed(uint32_t functor);
    static uint32_t clonePopupResult(uint32_t functor);
    static void destroyPopupResult(uint32_t functor);
    static void popupResult(uint32_t functor, uint32_t okPtr);
};

// Draw: with enough stones, the medal slots full -> the medal-over dialog, else the confirmation; without, the
// "not enough stones" dialog.
void DrawRaise::openRaisePopup(uint32_t me, uint32_t gacha) {
    Frame f;
    if (hasMagicStone(0, int32_t(ld32(gacha + 0x1C))) == 1) {
        const uint32_t gd = f.alloc(GACHA_SIZE);
        call(g::GachaData_copy, {gd, gacha});
        if (call(g::SceneUtil_isOverMedalSlot, {1}) == 1) {
            const uint32_t fo = newFunction(f, VT_MedalOver, {me});
            call(g::SceneUtil_openMedalOverDialog, {1, 3, fo});
            functionDestroy(fo);
            playErrorSE();
        } else {
            const int32_t cost = int32_t(ld32(gacha + 0x1C)), draws = int32_t(ld32(gacha + 0x20));
            const uint32_t copy = f.alloc(GACHA_SIZE);
            call(g::GachaData_copy, {copy, gd});
            const uint32_t fo = f.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x48});
            if (obj) {
                st32(obj, VT_RaiseOk);
                // the GachaData moved in: its strings taken (the copy's emptied), the numbers copied
                for (uint32_t k = 0; k < GACHA_SIZE; k += 4) st32(obj + 4 + k, ld32(copy + k));
                for (uint32_t s : {0u, 0xCu, 0x24u}) {
                    st32(copy + s, 0);
                    st32(copy + s + 4, 0);
                    st32(copy + s + 8, 0);
                }
                st32(obj + 0x44, me);
            }
            st32(fo + 16, obj);
            call(g::openCheckRaisePopup, {me, uint32_t(draws), uint32_t(cost), fo});
            functionDestroy(fo);
            freeGacha(copy);
        }
        freeGacha(gd);
    } else {
        const uint32_t td = textData(f, TXT_NO_STONE);
        const uint32_t s = newStr(f, textDataStr(td));
        const uint32_t fo = newFunction(f, VT_NoStone, {me});
        call(g::SceneUtil_openOkCancelDialog4Line, {s, fo, 0, 0});
        functionDestroy(fo);
        strFree(s);
        sharedRelease(td);
        playErrorSE();
    }
}

// the user's magic stones (paid + free) cover `cost`
uint32_t DrawRaise::hasMagicStone(uint32_t, int32_t cost) {
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctl = ld32(api + API_STONES + 4), sb = ld32(api + API_STONES);
    addRefIf(ctl);
    Frame f;
    const uint32_t a = f.alloc(4), b = f.alloc(4);
    rw::call(g::SafeBuffer_read, {sb, a, ld32(sb + 0xC) + 4, 4, 4});
    const int32_t free = int32_t(ld32(a));
    rw::call(g::SafeBuffer_read, {sb, b, ld32(sb + 0xC), 4, 0});
    const int32_t total = int32_t(ld32(b)) + free;
    releaseIf(ctl);
    return total >= cost ? 1 : 0;
}

// The confirmation popup (PopupNormal_R_DrawCheck) on a fading shadow: the cost text, the stones owned, Close and
// Draw; `done` runs after a confirmed draw.
void DrawRaise::openCheckRaisePopup(uint32_t me, int32_t draws, int32_t cost, uint32_t done) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow}) & 0xFF;
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChildZ, {layer, shadow, 3});
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_PopupJson});
    vcall(widget, V_widgetSetTouchEnabled, {widget, 0});
    vcall(shadow, node::addChild, {shadow, widget});
    Frame f;
    {
        const uint32_t anchor = f.alloc(8);
        call(g::Vec2_ctorXY, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(widget, node::setAnchorPoint, {widget, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
    // the closure {widget, layer, shadow, this, done}
    const uint32_t c0 = f.alloc(0x28);
    st32(c0, widget);
    st32(c0 + 4, layer);
    st32(c0 + 8, shadow);
    st32(c0 + 12, me);
    call(fn::function_vv_copy, {c0 + 0x10, done});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t stonesCtl = ld32(api + API_STONES + 4), stones = ld32(api + API_STONES);
    addRefIf(stonesCtl);
    {
        const uint32_t wording = findNode(widget, S_TxtWording, 0xB);
        Frame t;
        const uint32_t name = newStr(t, S_TxtWording, 0xB);
        const uint32_t view = call(g::SceneUtil_textToCustomTextView, {wording, name});
        strFree(name);
        const uint32_t text = t.alloc(12);
        rw::call(g::TextData_format, {text, 0, uint32_t(TXT_CHECK), uint32_t(cost), uint32_t(draws)});
        const uint32_t s = newStr(t, strData(text));
        call(g::CustomTextView_setXml, {view, s});
        strFree(s);
        strFree(text);
    }
    setTextC(findNode(widget, S_TxtDrawNum, 0xB), S_Empty);
    {
        Frame t;
        const uint32_t s = newStr(t, S_TxtCrownPossessed, 0x12);
        call(g::SceneUtil_settingTitleLabel, {widget, s, uint32_t(TXT_CROWN)});
        strFree(s);
    }
    {
        const uint32_t label = findNode(widget, S_TxtCrownPossessedLabel, 0x18);
        SceneUtil::setShadow(label);
        Frame t;
        const uint32_t a = t.alloc(4), b = t.alloc(4);
        rw::call(g::SafeBuffer_read, {stones, a, ld32(stones + 0xC) + 4, 4, 4});
        rw::call(g::SafeBuffer_read, {stones, b, ld32(stones + 0xC), 4, 0});
        const uint32_t buf = stringBuffer(t, {S_FmtD, ld32(b) + ld32(a)});
        setTextC(label, buf);
    }
    const uint32_t closeButton = findNode(widget, S_ButtonClose, 0xC);
    const uint32_t drawButton = findNode(widget, S_ButtonDraw, 0xB);
    {
        Frame t;
        const uint32_t s = newStr(t, S_TxtCancel, 0xA);
        call(g::SceneUtil_settingTitleLabel, {widget, s, uint32_t(TXT_CANCEL)});
        strFree(s);
    }
    addTouch(closeButton, VT_CloseButton, {drawButton});
    // the popup (the close button checks it; its result closes it)
    const uint32_t c1 = f.alloc(0x28);
    for (uint32_t k = 0; k < 16; k += 4) st32(c1 + k, ld32(c0 + k));
    functionCopy(c1 + 0x10, c0 + 0x10);
    {
        const uint32_t m = sceneManager();
        Frame p;
        const uint32_t check = newFunction(p, VT_PopupCheck, {closeButton});
        const uint32_t c2 = p.alloc(0x28);
        for (uint32_t k = 0; k < 16; k += 4) st32(c2 + k, ld32(c1 + k));
        functionCopy(c2 + 0x10, c1 + 0x10);
        const uint32_t result = p.alloc(20);
        heapClosure(result, VT_PopupResult, c2, c2 + 0x10);
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(c2 + 0x10);
        functionDestroy(check);
    }
    {
        Frame t;
        const uint32_t s = newStr(t, S_TxtDraw, 8);
        call(g::SceneUtil_settingTitleLabel, {widget, s, uint32_t(TXT_DRAW)});
        strFree(s);
    }
    {
        Frame p;
        const uint32_t c3 = p.alloc(0x28);
        for (uint32_t k = 0; k < 16; k += 4) st32(c3 + k, ld32(c0 + k));
        functionCopy(c3 + 0x10, c0 + 0x10);
        const uint32_t fo = p.alloc(20);
        heapClosure(fo, VT_DrawButton, c3, c3 + 0x10);
        call(fn::Widget_addTouchEventListener, {drawButton, fo});
        functionDestroy(fo);
        functionDestroy(c3 + 0x10);
    }
    {
        Frame p;
        const uint32_t none = newEmptyFunction(p);
        const uint32_t action = call(g::SceneUtil_createOpenPopupAnimation, {widget, none});
        call(g::Node_runAction, {widget, call(g::EaseSineOut_create, {action})});
        functionDestroy(none);
    }
    functionDestroy(c1 + 0x10);
    releaseIf(stonesCtl);
    functionDestroy(c0 + 0x10);
}

// sub_5B1E5C (closure {widget, layer, shadow, this, function}, run): the popup closes (its animation's end calls the
// closed lambda {layer, shadow, widget, this, run, function}); the shadow fades out.
void DrawRaise::closePopup(uint32_t closure, uint32_t run) {
    const uint32_t widget = ld32(closure);
    Frame f;
    const uint32_t local = f.alloc(0x30);
    st32(local, ld32(closure + 4));
    st32(local + 4, ld32(closure + 8));
    st32(local + 8, widget);
    st32(local + 12, ld32(closure + 12));
    st8(local + 0x10, run);
    call(fn::function_vv_copy, {local + 0x18, closure + 0x10});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x38});
    if (obj) {
        st32(obj, VT_Closed);
        for (uint32_t k = 0; k < 16; k += 4) st32(obj + 8 + k, ld32(local + k));
        st8(obj + 0x18, ld8(local + 0x10));
        call(fn::function_vv_move, {obj + 0x20, local + 0x18});
    }
    st32(fo + 16, obj);
    const uint32_t action = call(g::SceneUtil_createClosePopupAnimation, {widget, fo});
    call(g::Node_runAction, {widget, call(g::EaseSineOut_create, {action})});
    functionDestroy(fo);
    functionDestroy(local + 0x18);
    const uint32_t shadow = ld32(closure + 8);
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), 0})});
}

// the closed lambda's __clone / destroy_deallocate / operator()
uint32_t DrawRaise::cloneClosed(uint32_t functor) {
    const uint32_t obj = call(fn::operator_new, {0x38});
    if (obj) {
        st32(obj, VT_Closed);
        for (uint32_t k = 8; k < 0x18; k += 4) st32(obj + k, ld32(functor + k));
        st8(obj + 0x18, ld8(functor + 0x18));
        call(fn::function_vv_copy, {obj + 0x20, functor + 0x20});
    }
    return obj;
}
void DrawRaise::destroyClosed(uint32_t functor) {
    functionDestroy(functor + 0x20);
    call(fn::operator_delete, {functor});
}
// the popup is gone: remove it and the shadow, the list takes touches again; then the function (when run)
void DrawRaise::callClosed(uint32_t functor) {
    const uint32_t layer = ld32(functor + 8);
    const uint32_t list = ld32(functor + 0x14);
    vcall(layer, V_removeChild, {layer, ld32(functor + 0xC), 1});
    vcall(ld32(functor + 8), V_removeChild, {ld32(functor + 8), ld32(functor + 0x10), 1});
    st8(list + F_POPUP_OPEN, 0);
    if (ld8(functor + 0x18) && ld32(functor + 0x30)) call(g::FunctionVV_call, {functor + 0x20});
}

// the popup result lambda's __clone / destroy_deallocate / operator()(bool)
uint32_t DrawRaise::clonePopupResult(uint32_t functor) {
    const uint32_t obj = call(fn::operator_new, {0x30});
    if (obj) {
        st32(obj, VT_PopupResult);
        for (uint32_t k = 8; k < 0x18; k += 4) st32(obj + k, ld32(functor + k));
        call(fn::function_vv_copy, {obj + 0x18, functor + 0x18});
    }
    return obj;
}
void DrawRaise::destroyPopupResult(uint32_t functor) {
    functionDestroy(functor + 0x18);
    call(fn::operator_delete, {functor});
}
// cancelled (false): cancel sound, close without running
void DrawRaise::popupResult(uint32_t functor, uint32_t okPtr) {
    if (ld8(okPtr)) return;
    call(g::AudioPlaySystemSECancel, {});
    closePopup(functor + 8, 0);
}

RW_FUNCTION(0x5AF5AC, "DrawScrollView::openRaisePopup", DrawRaise::openRaisePopup);
RW_FUNCTION(0x5AF978, "DrawScrollView::hasMagicStone", DrawRaise::hasMagicStone);
RW_FUNCTION(0x5AFA30, "DrawScrollView::openCheckRaisePopup", DrawRaise::openCheckRaisePopup);
RW_FUNCTION(0x5B1E5C, "DrawScrollView::openCheckRaisePopup close lambda (sub_5B1E5C)", DrawRaise::closePopup);
RW_FUNCTION(0x5B2030, "DrawScrollView popup closed lambda __func::__clone() (sub_5B2030)", DrawRaise::cloneClosed);
RW_FUNCTION(0x5B20F4, "DrawScrollView popup closed lambda __func::destroy_deallocate() (sub_5B20F4)", DrawRaise::destroyClosed);
RW_FUNCTION(0x5B2120, "DrawScrollView popup closed lambda __func::operator() (sub_5B2120)", DrawRaise::callClosed);
RW_FUNCTION(0x5B2224, "DrawScrollView popup result lambda __func::__clone() (sub_5B2224)", DrawRaise::clonePopupResult);
RW_FUNCTION(0x5B22E0, "DrawScrollView popup result lambda __func::destroy_deallocate() (sub_5B22E0)",
            DrawRaise::destroyPopupResult);
RW_FUNCTION(0x5B230C, "DrawScrollView popup result lambda __func::operator()(bool) (sub_5B230C)", DrawRaise::popupResult);
