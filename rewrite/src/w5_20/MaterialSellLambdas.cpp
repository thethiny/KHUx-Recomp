// The lambdas of the material sell dialogs (MaterialSellDialogs.cpp): the amount slider and its - / + buttons
// (heap __funcs {..., the material's shared_ptr}) with the shared text update (sub_68A804: the amount and the
// total), the captureless Sell / Cancel buttons' and the can-close lambda's clones, the detail dialog's close
// callback (Sell: open the check; Cancel: clear the list's selection) and the check dialog's (Sell: the sale
// request; both close the dialog and clear the selection).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SceneManager_60D169 = 0x60D169;        // (mgr, bool): before a request
constexpr uint32_t APIManager_sellMaterial = 0x4C986D;     // (api, u64 user material id, count, function<...>)
constexpr uint32_t MaterialScrollView_clearSelect = 0x501D99, IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t Slider_getPercent = 0x83D3ED, Slider_setPercent = 0x83CEED;
constexpr uint32_t FadeTo_create = 0x86B3E1, Node_runAction = 0x894AED;
constexpr uint32_t openSellCheckDialog = 0x688105;
constexpr uint32_t aeabi_idiv = 0xCD5E8D;
constexpr uint32_t floatsisf = 0xCD6D68, divsf3 = 0xCD6FAC, mulsf3 = 0xCD6E14, fixsfsi = 0xCD7240;
constexpr uint32_t floorf = 0x415244;
}  // namespace g

constexpr uint32_t F_SELLING = 0x290, F_LIST = 0x28C;
constexpr uint32_t MAT_PRICE = 0xD0;
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t VT_Slider = 0xF64BD8, VT_StockDown = 0xF64BAC, VT_StockUp = 0xF64B80;
constexpr uint32_t VT_DetailOk = 0xF64B54, VT_DetailCancel = 0xF64B28, VT_DetailCanClose = 0xF64AFC;
constexpr uint32_t VT_DetailCloseDone = 0xF64AD0;           // {layer, backdrop, dialog}
constexpr uint32_t VT_CheckClosed = 0xF64970, VT_CheckCloseDone = 0xF6499C;
constexpr uint32_t VT_SaleDone = 0xF649C8;                  // {this, material shared_ptr, count} (heap 0x14)
constexpr uint32_t S_Fmt = 0xDF86DE;

inline void addRef(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// floor(amount / stock * 100) as the slider's percent (soft-float, as the game computes it)
void setSlider(uint32_t slider, uint32_t amount, uint32_t stock) {
    const uint32_t a = call(g::floatsisf, {amount});
    const uint32_t b = call(g::floatsisf, {stock});
    const uint32_t q = call(g::mulsf3, {call(g::divsf3, {a, b}), fbits(100.0f)});
    call(g::Slider_setPercent, {slider, call(g::fixsfsi, {call(g::floorf, {q})})});
}
// the dialog's close: the buttons stop taking touches, the close animation (then the done lambda), the backdrop
// fades out
void closeDialog(std::initializer_list<uint32_t> buttons, uint32_t vt, uint32_t layer, uint32_t backdrop,
                 uint32_t widget) {
    for (uint32_t b : buttons) vcall(b, V_setTouchEnabled, {b, 0});
    {
        Frame f;
        const uint32_t fo = newFunction(f, vt, {layer, backdrop, widget});
        call(g::Node_runAction, {widget, call(g::SceneUtil_createClosePopupAnimation, {widget, fo})});
        functionDestroy(fo);
    }
    call(g::Node_runAction, {backdrop, call(g::FadeTo_create, {fbits(0.1f), 0})});
}
void clearListSelection(uint32_t self) {
    call(g::MaterialScrollView_clearSelect, {ld32(self + F_LIST)});
    call(g::IScrollProtocol_updateLayout, {ld32(self + F_LIST), 0});
}
}  // namespace

struct MaterialSellLambdas {
    static uint32_t cloneSlider(uint32_t src) { return cloneAmount(src, VT_Slider); }
    static uint32_t cloneDown(uint32_t src) { return cloneAmount(src, VT_StockDown); }
    static uint32_t cloneUp(uint32_t src) { return cloneAmount(src, VT_StockUp); }
    static void cloneSliderTo(uint32_t src, uint32_t dst) { cloneAmountTo(src, dst, VT_Slider); }
    static void cloneDownTo(uint32_t src, uint32_t dst) { cloneAmountTo(src, dst, VT_StockDown); }
    static void cloneUpTo(uint32_t src, uint32_t dst) { cloneAmountTo(src, dst, VT_StockUp); }
    static uint32_t cloneAmount(uint32_t src, uint32_t vt);
    static void cloneAmountTo(uint32_t src, uint32_t dst, uint32_t vt);
    static void slider(uint32_t self, uint32_t ref, uint32_t type);
    static void down(uint32_t self, uint32_t ref, uint32_t type);
    static void up(uint32_t self, uint32_t ref, uint32_t type);
    static void updateTexts(uint32_t cap);
    static uint32_t cloneDetailOk(uint32_t) { return cloneEmpty(VT_DetailOk); }
    static uint32_t cloneDetailCancel(uint32_t) { return cloneEmpty(VT_DetailCancel); }
    static uint32_t cloneEmpty(uint32_t vt);
    static uint32_t cloneDetailCanClose(uint32_t src);
    static void detailClosed(uint32_t self, uint32_t ok);
    static uint32_t cloneCheckClosed(uint32_t src);
    static void cloneCheckClosedTo(uint32_t src, uint32_t dst);
    static void checkClosed(uint32_t self, uint32_t ok);
};

// {+4..+0x1C: seven words, +0x20: the material's control block}
uint32_t MaterialSellLambdas::cloneAmount(uint32_t src, uint32_t vt) {
    const uint32_t p = call(fn::operator_new, {0x24});
    if (p) cloneAmountTo(src, p, vt);
    return p;
}
void MaterialSellLambdas::cloneAmountTo(uint32_t src, uint32_t dst, uint32_t vt) {
    if (!dst) return;
    st32(dst, vt);
    for (uint32_t k = 4; k < 0x20; k += 4) st32(dst + k, ld32(src + k));
    st32(dst + 0x20, ld32(src + 0x20));
    addRef(ld32(dst + 0x20));
}

// slider {count, slider, this, then the text captures}: the amount follows the slider (at least 1)
void MaterialSellLambdas::slider(uint32_t self, uint32_t, uint32_t type) {
    if (ld32(type) != 0) return;
    const uint32_t count = ld32(self + 4), scene = ld32(self + 0xC);
    const uint32_t percent = call(g::Slider_getPercent, {ld32(self + 8)});
    const int32_t n = int32_t(call(g::aeabi_idiv, {percent * count, 100}));
    st32(scene + F_SELLING, uint32_t(n < 1 ? 1 : n));
    updateTexts(self + 0x10);
}
// - {this, slider, count, ...}: one less (at least 1)
void MaterialSellLambdas::down(uint32_t self, uint32_t, uint32_t type) {
    if (ld32(type) != 2) return;
    const uint32_t scene = ld32(self + 4);
    call(g::AudioPlaySystemSEDecide, {});
    const int32_t n = int32_t(ld32(scene + F_SELLING));
    const uint32_t v = n < 2 ? 1 : uint32_t(n - 1);
    st32(scene + F_SELLING, v);
    setSlider(ld32(self + 8), v, ld32(self + 0xC));
    updateTexts(self + 0x10);
}
// + {this, count, slider, ...}: one more (at most the stock)
void MaterialSellLambdas::up(uint32_t self, uint32_t, uint32_t type) {
    if (ld32(type) != 2) return;
    const uint32_t scene = ld32(self + 4);
    call(g::AudioPlaySystemSEDecide, {});
    const int32_t n = int32_t(ld32(scene + F_SELLING)) + 1;
    st32(scene + F_SELLING, uint32_t(n));
    const int32_t stock = int32_t(ld32(self + 8));
    const uint32_t v = uint32_t(stock < n ? stock : n);
    st32(scene + F_SELLING, v);
    setSlider(ld32(self + 0xC), v, ld32(self + 8));
    updateTexts(self + 0x10);
}
// {amount text, this, total text, material}: "<amount>" and the money for it
void MaterialSellLambdas::updateTexts(uint32_t cap) {
    const uint32_t amountText = ld32(cap), scene = ld32(cap + 4);
    {
        Frame f;
        setTextC(amountText, stringBuffer(f, {S_Fmt, ld32(scene + F_SELLING)}));
    }
    const uint32_t total = ld32(cap + 8), mat = ld32(cap + 0xC);
    Frame f;
    const uint32_t v = f.alloc(4);
    rw::call(g::SafeBuffer_read, {mat, v, ld32(mat + 0xC) + MAT_PRICE, 4, MAT_PRICE});
    const int32_t money = int32_t(ld32(v) * ld32(scene + F_SELLING));
    const uint32_t s = f.alloc(12);
    call(g::SceneUtil_getMoneyStr, {s, 0, uint32_t(money), uint32_t(money >> 31)});
    setTextC(total, (ld8(s) & 1) ? ld32(s + 8) : s + 1);
    strFree(s);
}

uint32_t MaterialSellLambdas::cloneEmpty(uint32_t vt) {
    const uint32_t p = call(fn::operator_new, {8});
    if (p) st32(p, vt);
    return p;
}
uint32_t MaterialSellLambdas::cloneDetailCanClose(uint32_t src) {
    const uint32_t p = call(fn::operator_new, {8});
    if (p) {
        st32(p, VT_DetailCanClose);
        st32(p + 4, ld32(src + 4));
    }
    return p;
}

// the detail dialog closed {this, index, Sell, Cancel, -, +, dialog, layer, backdrop}
void MaterialSellLambdas::detailClosed(uint32_t self, uint32_t ok) {
    const uint32_t scene = ld32(self + 4);
    if (ld8(ok)) {
        call(g::AudioPlaySystemSEDecide, {});
        call(g::openSellCheckDialog, {scene, ld32(self + 8), ld32(scene + F_SELLING)});
    } else {
        call(g::AudioPlaySystemSECancel, {});
        clearListSelection(scene);
    }
    closeDialog({ld32(self + 0xC), ld32(self + 0x10), ld32(self + 0x14), ld32(self + 0x18)}, VT_DetailCloseDone,
                ld32(self + 0x20), ld32(self + 0x24), ld32(self + 0x1C));
}

// the check dialog's close lambda {-, id (u64), count, this, material shared_ptr, count, Sell, Cancel, dialog,
// layer, backdrop, this}
uint32_t MaterialSellLambdas::cloneCheckClosed(uint32_t src) {
    const uint32_t p = call(fn::operator_new, {0x40});
    if (p) cloneCheckClosedTo(src, p);
    return p;
}
void MaterialSellLambdas::cloneCheckClosedTo(uint32_t src, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_CheckClosed);
    for (uint32_t k = 8; k < 0x1C; k += 4) st32(dst + k, ld32(src + k));
    st32(dst + 0x1C, ld32(src + 0x1C));
    addRef(ld32(dst + 0x1C));
    for (uint32_t k = 0x20; k < 0x34; k += 4) st32(dst + k, ld32(src + k));
    st32(dst + 0x38, ld32(src + 0x38));
    st32(dst + 0x34, ld32(src + 0x34));
}
void MaterialSellLambdas::checkClosed(uint32_t self, uint32_t ok) {
    if (ld8(ok)) {
        call(g::AudioPlaySystemSEDecide, {});
        call(g::SceneManager_60D169, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t lo = ld32(self + 8), hi = ld32(self + 0xC), count = ld32(self + 0x10);
        const uint32_t scene = ld32(self + 0x14), mat = ld32(self + 0x18), ctl = ld32(self + 0x1C);
        addRef(ctl);
        const uint32_t amount = ld32(self + 0x20);
        Frame f;
        const uint32_t fo = newEmptyFunction(f);
        uint32_t keep = 0;
        const uint32_t heap = call(fn::operator_new, {0x14});
        if (heap) {
            st32(heap, VT_SaleDone);
            st32(heap + 4, scene);
            st32(heap + 8, mat);
            st32(heap + 0xC, ctl);
            st32(heap + 0x10, amount);
        } else {
            keep = ctl;
        }
        st32(fo + 16, heap);
        call(g::APIManager_sellMaterial, {api, 0, lo, hi, count, fo});
        functionDestroy(fo);
        release(keep);
    } else {
        call(g::AudioPlaySystemSECancel, {});
    }
    closeDialog({ld32(self + 0x24), ld32(self + 0x28)}, VT_CheckCloseDone, ld32(self + 0x30), ld32(self + 0x34),
                ld32(self + 0x2C));
    clearListSelection(ld32(self + 0x38));
}

RW_FUNCTION(0x68AB7C, "SceneSellOutMaterialItem slider lambda __func::__clone() (sub_68AB7C)", MaterialSellLambdas::cloneSlider);
RW_FUNCTION(0x68ABD0, "SceneSellOutMaterialItem slider lambda __func::__clone(p) (sub_68ABD0)", MaterialSellLambdas::cloneSliderTo);
RW_FUNCTION(0x68AC3C, "SceneSellOutMaterialItem slider lambda __func::operator() (sub_68AC3C)", MaterialSellLambdas::slider);
RW_FUNCTION(0x68A9C4, "SceneSellOutMaterialItem stock-down lambda __func::__clone() (sub_68A9C4)", MaterialSellLambdas::cloneDown);
RW_FUNCTION(0x68AA18, "SceneSellOutMaterialItem stock-down lambda __func::__clone(p) (sub_68AA18)", MaterialSellLambdas::cloneDownTo);
RW_FUNCTION(0x68AA84, "SceneSellOutMaterialItem stock-down lambda __func::operator() (sub_68AA84)", MaterialSellLambdas::down);
RW_FUNCTION(0x68A6A4, "SceneSellOutMaterialItem stock-up lambda __func::__clone() (sub_68A6A4)", MaterialSellLambdas::cloneUp);
RW_FUNCTION(0x68A6F8, "SceneSellOutMaterialItem stock-up lambda __func::__clone(p) (sub_68A6F8)", MaterialSellLambdas::cloneUpTo);
RW_FUNCTION(0x68A764, "SceneSellOutMaterialItem stock-up lambda __func::operator() (sub_68A764)", MaterialSellLambdas::up);
RW_FUNCTION(0x68A804, "SceneSellOutMaterialItem sell amount texts (sub_68A804)", MaterialSellLambdas::updateTexts);
RW_FUNCTION(0x68A5B4, "SceneSellOutMaterialItem detail Sell lambda __func::__clone() (sub_68A5B4)", MaterialSellLambdas::cloneDetailOk);
RW_FUNCTION(0x68A518, "SceneSellOutMaterialItem detail Cancel lambda __func::__clone() (sub_68A518)", MaterialSellLambdas::cloneDetailCancel);
RW_FUNCTION(0x68A47C, "SceneSellOutMaterialItem detail can-close lambda __func::__clone() (sub_68A47C)", MaterialSellLambdas::cloneDetailCanClose);
RW_FUNCTION(0x68A240, "SceneSellOutMaterialItem detail closed lambda __func::operator() (sub_68A240)", MaterialSellLambdas::detailClosed);
RW_FUNCTION(0x688D64, "SceneSellOutMaterialItem check closed lambda __func::__clone() (sub_688D64)", MaterialSellLambdas::cloneCheckClosed);
RW_FUNCTION(0x688DCC, "SceneSellOutMaterialItem check closed lambda __func::__clone(p) (sub_688DCC)", MaterialSellLambdas::cloneCheckClosedTo);
RW_FUNCTION(0x688E50, "SceneSellOutMaterialItem check closed lambda __func::operator() (sub_688E50)", MaterialSellLambdas::checkClosed);
