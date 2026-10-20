// Shop > Sell (materials) > a material: the two sell dialogs of SceneSellOutMaterialItem. openSellDetailPopup
// (IDA split it in two; one function here) - icon, name, description, stock and unit price, the amount slider with
// its - / + buttons (starting at 1) and the total, Sell / Cancel; its close callback opens the check. And
// openSellCheckDialog - "<name> x<count>", the total, Sell / Cancel; its close callback sends the sale.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t SceneUtil_getMoneyStr = 0x461DC5;
constexpr uint32_t VecUll_copy = 0x468431, VecUll_destroy = 0x468499;
constexpr uint32_t throw_out_of_range = 0x47CCE1;
constexpr uint32_t Material_create = 0x7852E5;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5, GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t SceneManager_getPopupLayer = 0x613E69, SceneManager_getPopupLayer2 = 0x613E71;
constexpr uint32_t SceneManager_openPopup = 0x61524D;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t Slider_setPercent = 0x83CEED, Slider_addEventListener = 0x83D2EB;
constexpr uint32_t FadeTo_create = 0x86B3E1, Node_runAction = 0x894AED;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t floatsisf = 0xCD6D68, divsf3 = 0xCD6FAC, mulsf3 = 0xCD6E14, fixsfsi = 0xCD7240;
constexpr uint32_t floorf = 0x415244;
}  // namespace g

constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
constexpr uint32_t F_SELLING = 0x290, F_LIST = 0x28C, LIST_IDS = 0x628;   // the list's vector<ull> user material ids
constexpr uint32_t API_MATERIALS = 0xE4;                    // shared_ptr<SafeBuffer>: 16-byte records {id, material, count}
constexpr uint32_t MAT_ID = 0x0, MAT_NAME = 0xC, MAT_INFO = 0x4D, MAT_PRICE = 0xD0;
constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214;
// lambda vtables
constexpr uint32_t VT_Slider = 0xF64BD8, VT_StockDown = 0xF64BAC, VT_StockUp = 0xF64B80;   // heap 0x24
constexpr uint32_t VT_DetailOk = 0xF64B54, VT_DetailCancel = 0xF64B28;   // {}: close the popup
constexpr uint32_t VT_DetailCanClose = 0xF64AFC;            // function<bool()> {cancel}
constexpr uint32_t VT_DetailClosed = 0xF64AA4;              // heap 0x28
constexpr uint32_t VT_CheckOk = 0xF64A78, VT_CheckCancel = 0xF64A4C;     // {}
constexpr uint32_t VT_CheckCanClose = 0xF64A20;             // {cancel}
constexpr uint32_t VT_CheckClosed = 0xF64970;               // heap 0x40

constexpr uint32_t S_Fmt = 0xDF86DE, S_IconFmt = 0xDF9101, S_SellIcon = 0xDFD0FC, S_Data2 = 0xDFD118;
constexpr uint32_t S_Step1 = 0xE04859, S_Step2 = 0xE049A4, S_Name = 0xE04888, S_Info = 0xE0489F;
constexpr uint32_t S_Stock3 = 0xE048B6, S_MoneyStock = 0xE048CE, S_Data1 = 0xE048E1, S_Data3 = 0xE048EB;
constexpr uint32_t S_Data4 = 0xE048F5, S_Data5 = 0xE04902, S_Stock2 = 0xE0490C, S_MoneyTotal = 0xE04924;
constexpr uint32_t S_MoneyTotal2 = 0xE049D3, S_Bar = 0xE0493C, S_Down = 0xE0494E, S_Up = 0xE04960;
constexpr uint32_t S_Ok = 0xE04970, S_Cancel = 0xE0497F, S_TxtOk = 0xE0498D, S_TxtCancel = 0xE04999;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void shadow(uint32_t text) { call(g::SceneUtil_setShadow, {text}); }
inline void addRef(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
int32_t read32(Frame &f, uint32_t sb, uint32_t off) {
    const uint32_t v = f.alloc(4);
    rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + off, 4, off});
    return int32_t(ld32(v));
}
uint32_t readBytes(Frame &f, uint32_t sb, uint32_t off, uint32_t len) {
    const uint32_t v = f.alloc((len + 3) & ~3u);
    rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + off, len, off});
    return v;
}
void setTitle(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(g::SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}
void moneyText(uint32_t text, int32_t value) {
    Frame f;
    const uint32_t s = f.alloc(12);
    call(g::SceneUtil_getMoneyStr, {s, 0, uint32_t(value), uint32_t(value >> 31)});
    setTextC(text, (ld8(s) & 1) ? ld32(s + 8) : s + 1);
    strFree(s);
}
// the dark backdrop fading in on the popup layer, then the dialog centred on the screen (with the side margins)
uint32_t openDialog(uint32_t layer, uint32_t json, uint32_t &shadowSprite) {
    shadowSprite = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadowSprite, V_getOpacity, {shadowSprite});
    vcall(shadowSprite, V_setOpacity, {shadowSprite, 0});
    call(g::Node_runAction, {shadowSprite, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadowSprite});
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), json});
    vcall(widget, node::setAnchorPoint, {widget, ld32(GOT_ANCHOR_MIDDLE)});
    const float px = bitsf(vcall(widget, node::getPositionX, {widget}));
    const float w = ldf(vcall(widget, node::getContentSize, {widget}));
    const float side = bitsf(call(g::SceneUtil_getSideMargin, {}));
    const float py = bitsf(vcall(widget, node::getPositionY, {widget}));
    const float h = ldf(vcall(widget, node::getContentSize, {widget}) + 4);
    const float y = py + h * 0.5f;
    const float sideH = bitsf(call(g::SceneUtil_getSideHeightMargin, {}));
    Frame f;
    const uint32_t pos = f.alloc(8);
    call(g::Vec2_ctor, {pos, fbits(side + (px + w * 0.5f)), fbits(sideH + y)});
    vcall(widget, node::setPosition, {widget, pos});
    call(g::Vec2_dtor, {pos});
    vcall(layer, node::addChild, {layer, widget});
    return widget;
}
// the user material `index` of the list: its id, master material id and count from the user records
struct Picked {
    uint32_t lo, hi;
    int32_t material, count;
};
Picked pick(Frame &f, uint32_t self, uint32_t ids, uint32_t index, uint32_t &apiCtl) {
    call(g::VecUll_copy, {ids, ld32(self + F_LIST) + LIST_IDS});
    if (((ld32(ids + 4) - ld32(ids)) >> 3) <= index) call(g::throw_out_of_range, {ids});
    Picked p{ld32(ld32(ids) + 8 * index), ld32(ld32(ids) + 8 * index + 4), 0, 0};
    const uint32_t api = call(fn::APIManager_getInstance, {});
    apiCtl = ld32(api + API_MATERIALS + 4);
    const uint32_t records = ld32(api + API_MATERIALS);
    addRef(apiCtl);
    for (uint32_t k = 0; k < ld32(records + 0x20); ++k) {
        const uint32_t r = readBytes(f, records, 16 * k, 0x10);
        if (ld32(r) == p.lo && ld32(r + 4) == p.hi) {
            p.material = int32_t(ld32(r + 8));
            p.count = int32_t(ld32(r + 12));
            break;
        }
    }
    return p;
}
// a touch lambda on the heap {captures...} built into a std::function, handed to `add`, destroyed after
void heapListener(uint32_t add, uint32_t target, uint32_t vt, std::initializer_list<uint32_t> words, uint32_t ctl) {
    Frame f;
    const uint32_t fo = newEmptyFunction(f);
    const uint32_t heap = call(fn::operator_new, {0x24});
    if (heap) {
        st32(heap + 0x20, ctl);
        st32(heap, vt);
        uint32_t k = 4;
        for (uint32_t w : words) {
            st32(heap + k, w);
            k += 4;
        }
        ctl = 0;
    }
    st32(fo + 16, heap);
    call(add, {target, fo});
    functionDestroy(fo);
    release(ctl);
}
}  // namespace

struct MaterialSellDialogs {
    static void detail(uint32_t self, uint32_t index);
    static void check(uint32_t self, uint32_t index, uint32_t count);
};

void MaterialSellDialogs::detail(uint32_t self, uint32_t index) {
    call(g::AudioPlaySystemSEPopUp, {});
    st32(self + F_SELLING, 1);
    const uint32_t layer = call(g::SceneManager_getPopupLayer, {sceneManager()});
    uint32_t backdrop;
    const uint32_t widget = openDialog(layer, S_Step1, backdrop);
    Frame f;
    const uint32_t ids = f.alloc(12);
    uint32_t apiCtl;
    const Picked p = pick(f, self, ids, index, apiCtl);
    const uint32_t matPtr = f.alloc(8);
    call(g::Material_create, {matPtr, uint32_t(p.material)});
    const uint32_t mat = ld32(matPtr);
    {
        const uint32_t icon = find(widget, S_SellIcon, 0x10);
        const int32_t id = read32(f, mat, MAT_ID);
        Frame g2;
        const uint32_t path = stringBuffer(g2, {S_IconFmt, uint32_t(id)});
        const uint32_t s = newStr(g2, path, host_strlen(path));
        call(g::ImageView_loadTexture, {icon, s, 0});
        strFree(s);
    }
    {
        const uint32_t t = find(widget, S_Name, 0x16);
        shadow(t);
        Frame g2;
        setTextC(t, stringBuffer(g2, {readBytes(g2, mat, MAT_NAME, 0x41)}));
    }
    {
        const uint32_t t = find(widget, S_Info, 0x16);
        shadow(t);
        Frame g2;
        setTextC(t, stringBuffer(g2, {readBytes(g2, mat, MAT_INFO, 0x81)}));
    }
    {
        const uint32_t t = find(widget, S_Stock3, 0x17);
        shadow(t);
        Frame g2;
        setTextC(t, stringBuffer(g2, {S_Fmt, uint32_t(p.count)}));
    }
    {
        const uint32_t t = find(widget, S_MoneyStock, 0x12);
        shadow(t);
        Frame g2;
        const int32_t price = read32(g2, mat, MAT_PRICE);
        setTextC(t, stringBuffer(g2, {S_Fmt, uint32_t(price)}));
    }
    for (auto [name, len, id] : {std::tuple<uint32_t, uint32_t, int32_t>{S_Data1, 9, 100300004}, {S_Data2, 9, 102000001},
                                 {S_Data3, 9, 0}, {S_Data4, 0xC, 102000002}, {S_Data5, 9, 100300005}}) {
        const uint32_t t = find(widget, name, len);
        shadow(t);
        if (id) setTextId(t, id);
    }
    const uint32_t stockText = find(widget, S_Stock2, 0x17);
    shadow(stockText);
    {
        Frame g2;
        setTextC(stockText, stringBuffer(g2, {S_Fmt, 1}));
    }
    const uint32_t totalText = find(widget, S_MoneyTotal, 0x17);
    shadow(totalText);
    moneyText(totalText, read32(f, mat, MAT_PRICE));
    const uint32_t matCtl = ld32(matPtr + 4);
    addRef(matCtl);
    const uint32_t slider = find(widget, S_Bar, 0x11);
    {
        const uint32_t a = call(g::floatsisf, {ld32(self + F_SELLING)});
        const uint32_t b = call(g::floatsisf, {uint32_t(p.count)});
        const uint32_t q = call(g::mulsf3, {call(g::divsf3, {a, b}), fbits(100.0f)});
        call(g::Slider_setPercent, {slider, call(g::fixsfsi, {call(g::floorf, {q})})});
    }
    addRef(matCtl);
    heapListener(g::Slider_addEventListener, slider, VT_Slider,
                 {uint32_t(p.count), slider, self, stockText, self, totalText, mat}, matCtl);
    const uint32_t down = find(widget, S_Down, 0x11);
    addRef(matCtl);
    heapListener(fn::Widget_addTouchEventListener, down, VT_StockDown,
                 {self, slider, uint32_t(p.count), stockText, self, totalText, mat}, matCtl);
    const uint32_t up = find(widget, S_Up, 0xF);
    addRef(matCtl);
    heapListener(fn::Widget_addTouchEventListener, up, VT_StockUp,
                 {self, uint32_t(p.count), slider, stockText, self, totalText, mat}, matCtl);
    const uint32_t ok = find(widget, S_Ok, 0xE);
    const uint32_t cancel = find(widget, S_Cancel, 0xD);
    addTouch(ok, VT_DetailOk, {});
    setTitle(ok, S_TxtOk, 0xB, 100100029);
    addTouch(cancel, VT_DetailCancel, {});
    setTitle(cancel, S_TxtCancel, 0xA, 100100002);
    {
        Frame g2;
        const uint32_t fo = newEmptyFunction(g2);
        call(g::Node_runAction, {widget, call(g::SceneUtil_createOpenPopupAnimation, {widget, fo})});
        functionDestroy(fo);
    }
    {
        const uint32_t sm = sceneManager();
        Frame g2;
        const uint32_t canClose = newFunction(g2, VT_DetailCanClose, {cancel});
        const uint32_t closed = newEmptyFunction(g2);
        const uint32_t heap = call(fn::operator_new, {0x28});
        if (heap) {
            const uint32_t w[10] = {VT_DetailClosed, self, index, ok, cancel, down, up, widget, layer, backdrop};
            for (uint32_t k = 0; k < 10; ++k) st32(heap + 4 * k, w[k]);
        }
        st32(closed + 16, heap);
        call(g::SceneManager_openPopup, {sm, canClose, closed, 1});
        functionDestroy(closed);
        functionDestroy(canClose);
    }
    release(matCtl);
    release(ld32(matPtr + 4));
    release(apiCtl);
    call(g::VecUll_destroy, {ids});
}

void MaterialSellDialogs::check(uint32_t self, uint32_t index, uint32_t count) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SceneManager_getPopupLayer2, {sceneManager()});
    uint32_t backdrop;
    const uint32_t widget = openDialog(layer, S_Step2, backdrop);
    Frame f;
    const uint32_t ids = f.alloc(12);
    uint32_t apiCtl;
    const Picked p = pick(f, self, ids, index, apiCtl);
    const uint32_t matPtr = f.alloc(8);
    call(g::Material_create, {matPtr, uint32_t(p.material)});
    const uint32_t mat = ld32(matPtr);
    {
        const uint32_t t = find(widget, S_Name, 0x16);
        shadow(t);
        Frame g2;
        const uint32_t td = textData(g2, 102000007);
        const uint32_t fmt = textDataStr(td);
        const uint32_t name = readBytes(g2, mat, MAT_NAME, 0x41);
        setTextC(t, stringBuffer(g2, {fmt, name, count}));
        sharedRelease(td);
    }
    {
        const uint32_t t = find(widget, S_MoneyTotal2, 0x18);
        shadow(t);
        moneyText(t, read32(f, mat, MAT_PRICE) * int32_t(count));
    }
    setTitle(widget, S_Data1, 9, 102000003);
    shadow(find(widget, S_Data2, 9));
    setTitle(widget, S_Data3, 9, 100300005);
    const uint32_t matObj = ld32(matPtr), matCtl = ld32(matPtr + 4);
    addRef(matCtl);
    const uint32_t ok = find(widget, S_Ok, 0xE);
    const uint32_t cancel = find(widget, S_Cancel, 0xD);
    addRef(matCtl);
    addTouch(ok, VT_CheckOk, {});
    setTitle(ok, S_TxtOk, 0xB, 100100029);
    addTouch(cancel, VT_CheckCancel, {});
    setTitle(cancel, S_TxtCancel, 0xA, 100100002);
    {
        Frame g2;
        const uint32_t fo = newEmptyFunction(g2);
        call(g::Node_runAction, {widget, call(g::SceneUtil_createOpenPopupAnimation, {widget, fo})});
        functionDestroy(fo);
    }
    {
        const uint32_t sm = sceneManager();
        Frame g2;
        const uint32_t canClose = newFunction(g2, VT_CheckCanClose, {cancel});
        addRef(matCtl);
        const uint32_t closed = newEmptyFunction(g2);
        uint32_t keep = matCtl;
        const uint32_t heap = call(fn::operator_new, {0x40});
        if (heap) {
            st32(heap, VT_CheckClosed);
            st32(heap + 0xC, p.hi);
            st32(heap + 8, p.lo);
            st32(heap + 0x10, count);
            st32(heap + 0x14, self);
            st32(heap + 0x18, matObj);
            st32(heap + 0x1C, matCtl);
            st32(heap + 0x20, count);
            st32(heap + 0x24, ok);
            st32(heap + 0x28, cancel);
            st32(heap + 0x2C, widget);
            st32(heap + 0x30, layer);
            st32(heap + 0x34, backdrop);
            st32(heap + 0x38, self);
            keep = 0;
        }
        st32(closed + 16, heap);
        call(g::SceneManager_openPopup, {sm, canClose, closed, 1});
        functionDestroy(closed);
        release(keep);
        functionDestroy(canClose);
    }
    release(matCtl);
    release(matCtl);
    release(ld32(matPtr + 4));
    release(apiCtl);
    call(g::VecUll_destroy, {ids});
}

RW_FUNCTION(0x686E6C, "SceneSellOutMaterialItem::openSellDetailPopup", MaterialSellDialogs::detail);
RW_FUNCTION(0x688104, "SceneSellOutMaterialItem::openSellCheckDialog", MaterialSellDialogs::check);
