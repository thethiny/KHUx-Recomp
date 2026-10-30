// DrawScrollView's remaining lambdas: the shop-top gacha button ($_0) and its item touch ($_1), the raise popup's
// draw ($_6, its tutorial / retry answers, $_7's jewel shop), and the clones of the lambdas holding a GachaData.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace drawview {
// DrawScrollView
constexpr uint32_t DV_BUSY = 0x650, DV_SCROLLED = 0x651, DV_READY = 0x690;
constexpr uint32_t DV_ON_DRAW = 0x638;        // function<void()>
constexpr uint32_t DV_OFFSET = 0x540;         // float: the list's scroll offset
constexpr uint32_t DV_IDS = 0x628;            // vector<int>
constexpr uint32_t DV_TOUCH_STATE = 0x67C, DV_PRESSED = 0x680, DV_PRESSING = 0x678, DV_MOVED = 0x688, DV_MOVE = 0x68C;
// GachaData (0x40): strings at +0, +0xC, +0x24; +0x18 id, +0x20 draws, +0x30 kind, +0x38 / +0x3C limits
constexpr uint32_t GD_SIZE = 0x40, GD_ID = 0x18, GD_DRAWS = 0x20, GD_KIND = 0x30, GD_LIMIT = 0x38, GD_LEFT = 0x3C;
constexpr uint32_t KIND_BONDS = 2;
constexpr uint32_t SM_GACHA_KIND = 0x294, SM_LOADING_BLOCKED = 0x4CC, SM_ON_FINISH_LOADING = 0x548;
// callees
constexpr uint32_t GachaData_copy = 0x5AE2CD;
constexpr uint32_t DrawScrollView_openKizunaPopup = 0x5B0411, DrawScrollView_openRaisePopup = 0x5AF5AD;
constexpr uint32_t APIManager_executeGachaDraw = 0x4CE579;   // (api, id, draws, function<void(bool)>)
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t SceneShopTop_callShopAPI = 0x68BB9D;       // (function<void()> by value)
constexpr uint32_t SceneUtil_CompleteTutorialPhase = 0x472A75; // (function by value, -, -)
constexpr uint32_t BuyMagicStoneDialog_open = 0x588CAD;
constexpr uint32_t FunctionB_assign = 0x490109;               // function<void(bool)>::operator=
constexpr uint32_t Touch_getLocation = 0x900E57, Node_convertToNodeSpace = 0x89599D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Rect_containsPoint = 0x8DF2A9, IScrollProtocol_getItemPixel = 0x4F6751;
constexpr uint32_t FIXSFSI = 0xCD7240, DIVSI = 0xCD5E8D;
constexpr uint32_t GOT_CREATE_SHOP_TOP = 0xFD66F8;            // GOT: createSceneLayer<SceneShopTop>
// __func vtables
constexpr uint32_t VT_RaiseDraw = 0xF5C5E4, VT_GachaButton = 0xF5C744;   // $_6 / $_0 (heap 0x48)
constexpr uint32_t VT_LoadingFinished = 0xF5C63C, VT_Drawn = 0xF5C610;   // $_6's two answers (no captures)
constexpr uint32_t TOUCH_ENDED = 2;

void gachaDestroy(uint32_t g) {
    strFree(g + 0x24);
    strFree(g + 0xC);
    strFree(g);
}

// $_0 (a gacha button) {this, GachaData}(sender, type): a scroll ends the press; else, ready and free and with
// draws left -> the decide sound, the kind kept, the bonds or raise popup for a copy of the gacha
void gachaButtonTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (ld8(me + DV_SCROLLED) && ld8(me + DV_BUSY)) {
        st8(me + DV_SCROLLED, 0);
        st8(me + DV_BUSY, 0);
        return;
    }
    st8(me + DV_SCROLLED, 0);
    const uint32_t g = f + 8;
    if (!ld8(me + DV_READY) || ld8(me + DV_BUSY) || (ld32(g + GD_LIMIT) && !ld32(g + GD_LEFT))) return;
    call(fn::AudioPlaySystemSEDecide, {});
    const uint32_t kind = ld32(g + GD_KIND);
    const uint32_t bonds = kind == KIND_BONDS;
    st32(sceneManager() + SM_GACHA_KIND, bonds ? 2 : 1);
    st8(me + DV_BUSY, 1);
    Frame fr;
    const uint32_t copy = fr.alloc(GD_SIZE);
    call(GachaData_copy, {copy, g});
    call(bonds ? DrawScrollView_openKizunaPopup : DrawScrollView_openRaisePopup, {me, copy});
    gachaDestroy(copy);
}

// $_1 (an item touched) {view, rect +8, base index +0x18}(touch, event) -> bool: while free, a touch inside the
// rect presses the item under it (the row from the scroll offset)
uint32_t itemTouchBegan(uint32_t f, uint32_t touchRef, uint32_t) {
    const uint32_t view = ld32(f + 4);
    if (ld8(view + DV_BUSY)) return 0;
    uint32_t pressed = 0;
    Frame fr;
    const uint32_t loc = fr.alloc(8), local = fr.alloc(8);
    call(Touch_getLocation, {loc, ld32(touchRef)});
    call(Node_convertToNodeSpace, {local, view, loc});
    call(Vec2_dtor, {loc});
    if (call(Rect_containsPoint, {f + 8, local}) == 1) {
        st32(view + DV_TOUCH_STATE, 0);
        const uint32_t pixel = call(IScrollProtocol_getItemPixel, {view});
        const uint32_t offset = call(FIXSFSI, {ld32(view + DV_OFFSET)});
        const uint32_t row = call(DIVSI, {offset, call(FIXSFSI, {pixel})});
        const int32_t index = int32_t(ld32(f + 0x18) + row);
        const uint32_t ids = view + DV_IDS;
        if (index >= 1 && uint32_t(index - 1) < ((ld32(ids + 4) - ld32(ids)) >> 2)) {
            st32(view + DV_PRESSED, uint32_t(index - 1));
            pressed = 1;
            st8(view + DV_PRESSING, 1);
            st8(view + DV_MOVED, 1);
            st32(view + DV_MOVE, 0);
        }
    }
    call(Vec2_dtor, {local});
    return pressed;
}

// $_6 (the raise popup's Draw) {GachaData, this +0x44}: loading blocked (its end: the shop reloaded), the draw
// sent (its answer), the draw callback
void raiseDraw(uint32_t f) {
    const uint32_t me = ld32(f + 0x44);
    const uint32_t sm = sceneManager();
    {
        Frame fr;
        const uint32_t done = newFunction(fr, VT_LoadingFinished, {});
        st8(sm + SM_LOADING_BLOCKED, 1);
        call(FunctionB_assign, {sm + SM_ON_FINISH_LOADING, done});
        functionDestroy(done);
    }
    const uint32_t api = call(fn::APIManager_getInstance, {});
    {
        Frame fr;
        const uint32_t drawn = newFunction(fr, VT_Drawn, {});
        call(APIManager_executeGachaDraw, {api, ld32(f + 4 + GD_ID), ld32(f + 4 + GD_DRAWS), drawn});
        functionDestroy(drawn);
    }
    call(fn::function_vv_call, {me + DV_ON_DRAW});
}
// $_6's draw answer (ok): the indicator off; ok -> the draw tutorial phase completed
void drawAnswered(uint32_t, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) return;
    Frame fr;
    const uint32_t none = newEmptyFunction(fr);
    call(SceneUtil_CompleteTutorialPhase, {none, 0, 0});
    functionDestroy(none);
}
// $_6's loading end: no retry pending -> the shop data reloaded and the shop top shown again
void loadingFinished(uint32_t) {
    if (call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) return;
    {
        Frame fr;
        const uint32_t none = newEmptyFunction(fr);
        call(SceneShopTop_callShopAPI, {none});
        functionDestroy(none);
    }
    changeSceneLayerTo(GOT_CREATE_SHOP_TOP, 1, 1, 1);
}
// $_7's answer {this}(buy): buy -> the jewel shop; the view free again
void notEnoughAnswered(uint32_t f, uint32_t buyRef) {
    const uint32_t me = ld32(f + 4);
    if (ld8(buyRef)) {
        Frame fr;
        const uint32_t none = newEmptyFunction(fr);
        call(BuyMagicStoneDialog_open, {none});
        functionDestroy(none);
    }
    st8(me + DV_BUSY, 0);
}

// the clones of $_6 {GachaData, this} and $_0 {this, GachaData} (into a new block / a given one)
uint32_t raiseDrawClone(uint32_t self) {
    const uint32_t h = call(fn::operator_new, {0x48});
    if (h) {
        st32(h, VT_RaiseDraw);
        call(GachaData_copy, {h + 4, self + 4});
        st32(h + 0x44, ld32(self + 0x44));
    }
    return h;
}
void raiseDrawCloneTo(uint32_t self, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_RaiseDraw);
    call(GachaData_copy, {dst + 4, self + 4});
    st32(dst + 0x44, ld32(self + 0x44));
}
uint32_t gachaButtonClone(uint32_t self) {
    const uint32_t h = call(fn::operator_new, {0x48});
    if (h) {
        st32(h, VT_GachaButton);
        st32(h + 4, ld32(self + 4));
        call(GachaData_copy, {h + 8, self + 8});
        st32(h + 4, ld32(self + 4));
    }
    return h;
}
void gachaButtonCloneTo(uint32_t self, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_GachaButton);
    st32(dst + 4, ld32(self + 4));
    call(GachaData_copy, {dst + 8, self + 8});
    st32(dst + 4, ld32(self + 4));
}
}  // namespace drawview
}  // namespace

RW_FUNCTION(0x5B3B90, "DrawScrollView::initShopTopGachaButton $_0::operator()", drawview::gachaButtonTouched);
RW_FUNCTION(0x5B3734, "DrawScrollView::registItemTouchEvent $_1::operator()(Touch*, Event*)", drawview::itemTouchBegan);
RW_FUNCTION(0x5B2D14, "DrawScrollView::openRaisePopup $_6::operator()", drawview::raiseDraw);
RW_FUNCTION(0x5B2EA4, "DrawScrollView::openRaisePopup $_6 lambda(bool)#2::operator()", drawview::drawAnswered);
RW_FUNCTION(0x5B2FCC, "DrawScrollView::openRaisePopup $_6 lambda(bool)#1::operator()", drawview::loadingFinished);
RW_FUNCTION(0x5B3F00, "DrawScrollView::openRaisePopup $_7 lambda(bool)#1::operator()", drawview::notEnoughAnswered);
RW_FUNCTION(0x5B2C28, "DrawScrollView::openRaisePopup $_6 __func::__clone()", drawview::raiseDrawClone);
RW_FUNCTION(0x5B2C78, "DrawScrollView::openRaisePopup $_6 __func::__clone(__base*)", drawview::raiseDrawCloneTo);
RW_FUNCTION(0x5B3A90, "DrawScrollView::initShopTopGachaButton $_0 __func::__clone()", drawview::gachaButtonClone);
RW_FUNCTION(0x5B3AE8, "DrawScrollView::initShopTopGachaButton $_0 __func::__clone(__base*)", drawview::gachaButtonCloneTo);
