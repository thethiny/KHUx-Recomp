// BuyMagicStoneDialog leftovers (the Jewel shop): the owned Jewels popup (paid / free), the two web info buttons,
// the purchase and restore lambdas, the popup close, and the product list's sort helpers (std::__sort4 /
// __sort5 / __insertion_sort_incomplete with the shop's comparator).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_textToCustomTextView = 0x45BF15;
constexpr uint32_t SceneUtil_openWebViewInfoPopup = 0x47463D;   // (title, url, function, post)
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t SM_getInstance = 0x60C26D, SM_getPopupLayer2 = 0x613E71, SM_getPopupLayer3 = 0x613E79;
constexpr uint32_t SM_openPopup = 0x61524D, SM_enableLoadingIndicator = 0x60D169, SM_updateHeader = 0x611169;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09, AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t PurchaseManager_getInstance = 0x4D7A71;
constexpr uint32_t PurchaseManager_purchase = 0x4D831D;          // (pm, const char*, function)
constexpr uint32_t PurchaseManager_resumeTransaction = 0x4D8009; // (pm, function)
constexpr uint32_t BuyMagicStoneDialog_purchaseCallback = 0x58A7A1;
constexpr uint32_t PopupDialog_create = 0x4F3C15, PopupDialog_open = 0x4F420D;
constexpr uint32_t FunctionVB_copy = 0x45C84D;
constexpr uint32_t FunctionVB_call = 0x474139;                   // std::function<void(bool)>::operator()(bool)
constexpr uint32_t shared_add = 0xC8DBB5, shared_release = 0xC8DBD1;
constexpr uint32_t sort3 = 0x58C7C1, compare = 0x58C709;
}  // namespace g

constexpr uint32_t V_getOpacity = 0x20C, V_setOpacity = 0x214;
inline uint32_t mgr() { return call(g::SM_getInstance, {}); }
void releaseIf(uint32_t ctl) {
    if (ctl) call(g::shared_release, {ctl});
}
// a label with the shadow style and a "%d"-style TextData format of a value read from the stone row
void countLabel(uint32_t label, uint32_t row, uint32_t offset) {
    call(g::SceneUtil_setShadow, {label});
    Frame f;
    const uint32_t text = f.alloc(8), word = f.alloc(4);
    call(g::TextData_get, {text, 0, 102600028});
    const uint32_t fmt = ld32(ld32(text) + 4);
    call(g::SafeBuffer_read, {row, word, ld32(row + 0xC) + offset, 4, offset});
    const uint32_t s = newStr(f, stringBuffer(f, {fmt, ld32(word)}));
    call(g::Text_setString, {label, s});
    strFree(s);
    releaseIf(ld32(text + 4));
}

// The owned Jewels: paid ("Charge") and free ("Free") counts, the caution texts, OK; closed by the result
// lambda ($_ {widget, layer, shadow})
uint32_t openStoneInfoPopup(uint32_t) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer2, {mgr()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, V_getOpacity, {shadow});
    vcall(shadow, V_setOpacity, {shadow, 0});
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t widget = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), 0xDFFEBA});
    vcall(shadow, node::addChild, {shadow, widget});
    {
        Frame f;
        const uint32_t anchor = f.alloc(8);
        call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(widget, node::setAnchorPoint, {widget, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t ctrl = ld32(api + 0xA8), stones = ld32(api + 0xA4);
    if (ctrl) call(g::shared_add, {ctrl});
    const uint32_t charge = findNode(widget, 0xDFFEEA, 6);              // "Charge"
    titleLabel(widget, 0xDFFEEA, 6, 102600037);
    countLabel(findNode(charge, 0xDF8A89, 9), stones, 4);               // "Num_Label": paid
    call(g::SceneUtil_setShadow, {findNode(charge, 0xDF8A28, 6)});       // "Number"
    const uint32_t free = findNode(widget, 0xDFFEF1, 4);                // "Free"
    titleLabel(widget, 0xDFFEF1, 4, 102600038);
    countLabel(findNode(free, 0xDF8A89, 9), stones, 0);
    call(g::SceneUtil_setShadow, {findNode(free, 0xDF8A28, 6)});
    {
        const uint32_t caution2 = findNode(widget, 0xDFFEF6, 15);       // "Caution_Label02"
        call(g::SceneUtil_setShadow, {caution2});
        Frame f;
        const uint32_t empty = newStr(f, 0xDF8A1A, 0);
        call(g::Text_setString, {caution2, empty});
        strFree(empty);
    }
    {
        const uint32_t caution1 = findNode(widget, 0xDFFB10, 15);       // "Caution_Label01"
        Frame f;
        const uint32_t name = newStr(f, 0xDFFB10, 15);
        const uint32_t view = call(g::SceneUtil_textToCustomTextView, {caution1, name});
        strFree(name);
        const uint32_t text = f.alloc(8);
        call(g::TextData_get, {text, 0, 102600033});
        const uint32_t xml = newStr(f, ld32(ld32(text) + 4));
        call(g::CustomTextView_setXml, {view, xml});
        strFree(xml);
        releaseIf(ld32(text + 4));
    }
    const uint32_t ok = findNode(widget, 0xDF8688, 9);                  // "Button_OK"
    {
        Frame f;
        const uint32_t fo = newFunction(f, 0xF5A6F4, {});
        call(g::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
    }
    titleLabel(ok, 0xDF869F, 6, 100100001);                             // "Txt_OK"
    {
        const uint32_t m = mgr();
        Frame f;
        const uint32_t check = newFunction(f, 0xF5A6C8, {ok});
        const uint32_t result = newFunction(f, 0xF5A670, {widget, layer, shadow});
        rw::call(g::SM_openPopup, {m, check, result, 0});
        functionDestroy(result);
        functionDestroy(check);
    }
    {
        Frame f;
        const uint32_t fo = newFunction(f, 0xF5A644, {});
        const uint32_t anim = call(g::EaseSineOut_create, {call(g::SceneUtil_createOpenPopupAnimation, {widget, fo})});
        call(g::Node_runAction, {widget, anim});
        functionDestroy(fo);
    }
    releaseIf(ctrl);
    return 0x9A4;   // r0 at the return: the frame size (add sp, r0)
}

// sub_58B944 / sub_58BB88: an info button opens a web page (its URL from the shop info, APIManager +0x1A8)
void openWebInfo(uint32_t typeRef, uint32_t urlOffset, int32_t title) {
    if (ld32(typeRef) != 2) return;
    call(g::AudioPlaySystemSEDecide, {});
    const uint32_t info = ld32(call(g::APIManager_getInstance, {}) + 0x1A8);
    Frame f;
    const uint32_t url = f.alloc(12);
    const uint32_t src = info + urlOffset;
    if (strIsLong(src)) {
        strInit(url, ld32(src + 8), ld32(src + 4));
    } else {
        st32(url, ld32(src));
        st32(url + 4, ld32(src + 4));
        st32(url + 8, ld32(src + 8));
    }
    const uint32_t text = f.alloc(8);
    call(g::TextData_get, {text, 0, uint32_t(title)});
    const uint32_t titleStr = newStr(f, ld32(ld32(text) + 4));
    const uint32_t urlStr = newStr(f, strData(url));
    const uint32_t fo = newEmptyFunction(f);
    const uint32_t post = f.alloc(12);
    st8(post, 0);
    st8(post + 1, 0);
    call(g::SceneUtil_openWebViewInfoPopup, {titleStr, urlStr, fo, post});
    strFree(post);
    functionDestroy(fo);
    strFree(urlStr);
    strFree(titleStr);
    releaseIf(ld32(text + 4));
    strFree(url);
}
void webInfoA(uint32_t, uint32_t, uint32_t typeRef) { openWebInfo(typeRef, 0x60, 102600053); }
void webInfoB(uint32_t, uint32_t, uint32_t typeRef) { openWebInfo(typeRef, 0x54, 102600052); }

// sub_58BF5C, a product's buy lambda {product id (string), a, b}: unless a purchase runs, the loading indicator
// and the store purchase ($_ {a, b} -> sub_58C0D8)
void buyProduct(uint32_t functor) {
    const uint32_t product = functor + 4;
    const uint32_t pm = call(g::PurchaseManager_getInstance, {});
    if (ld8(pm)) return;
    call(g::SM_enableLoadingIndicator, {mgr(), 1});
    const uint32_t id = strData(product);
    Frame f;
    const uint32_t fo = newFunction(f, 0xF5A568, {ld32(functor + 0x10), ld32(functor + 0x14)});
    call(g::PurchaseManager_purchase, {pm, id, fo});
    functionDestroy(fo);
}
// sub_58C0D8, the purchase result {a, b}: BuyMagicStoneDialog::purchaseCallback(ok, a, 0, 0, b, {}, {})
void purchased(uint32_t functor, uint32_t okRef) {
    Frame f;
    const uint32_t fn1 = newEmptyFunction(f), fn2 = newEmptyFunction(f);
    rw::call(g::BuyMagicStoneDialog_purchaseCallback, {ld8(okRef), ld32(functor + 4), 0, 0, ld32(functor + 8), fn2, fn1});
    functionDestroy(fn1);
    functionDestroy(fn2);
}
// sub_58CC34, restore {done}: the loading indicator and the store's transaction resume ($_ {done})
void restore(uint32_t functor) {
    call(g::SM_enableLoadingIndicator, {mgr(), 1});
    const uint32_t pm = call(g::PurchaseManager_getInstance, {});
    Frame f;
    const uint32_t done = f.alloc(20);
    call(g::FunctionVB_copy, {done, functor + 8});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x20});
    if (obj) {
        st32(obj, 0xF5A5EC);
        functionCopy(obj + 8, done);
    }
    st32(fo + 16, obj);
    call(g::PurchaseManager_resumeTransaction, {pm, fo});
    functionDestroy(fo);
    functionDestroy(done);
}
// sub_58CEF4, the resume result {done}: success refreshes the header and shows the "restored" dialog ($_ {done}
// on close); failure shows the error dialog and calls done(false)
void restored(uint32_t functor, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    call(g::SM_enableLoadingIndicator, {mgr(), 0});
    const uint32_t done = functor + 8;
    if (!ok) {
        const uint32_t api = call(g::APIManager_getInstance, {});
        {
            Frame f;
            const uint32_t fo = newEmptyFunction(f);
            call(g::APIManager_openErrorDialog, {api, fo, 1});
            functionDestroy(fo);
        }
        call(g::FunctionVB_call, {done, 0});
        return;
    }
    call(g::SM_updateHeader, {mgr()});
    Frame f;
    const uint32_t a = f.alloc(20);
    call(g::FunctionVB_copy, {a, done});
    const uint32_t dialog = call(g::PopupDialog_create, {0, 0});
    const uint32_t layer3 = call(g::SM_getPopupLayer3, {mgr()});
    const uint32_t text = f.alloc(8);
    call(g::TextData_get, {text, 0, 105100009});
    const uint32_t msg = newStr(f, ld32(ld32(text) + 4));
    const uint32_t b = f.alloc(20);
    functionCopy(b, a);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x20});
    if (obj) {
        st32(obj, 0xF5A618);
        functionCopy(obj + 8, b);
    }
    st32(fo + 16, obj);
    call(g::PopupDialog_open, {dialog, layer3, msg, fo});
    functionDestroy(fo);
    functionDestroy(b);
    strFree(msg);
    releaseIf(ld32(text + 4));
    functionDestroy(a);
}
// sub_58D420, the owned Jewels popup's result {popup, layer, shadow}: on OK the decide sound, the close
// animation ($_ {layer, shadow, popup}) and the shadow faded out
void stoneInfoResult(uint32_t functor, uint32_t okRef) {
    if (!ld8(okRef)) return;
    call(g::AudioPlaySystemSEDecide, {});
    const uint32_t popup = ld32(functor + 4);
    {
        Frame f;
        const uint32_t fo = newFunction(f, 0xF5A69C, {ld32(functor + 8), ld32(functor + 0xC), popup});
        const uint32_t anim = call(g::EaseSineOut_create, {call(g::SceneUtil_createClosePopupAnimation, {popup, fo})});
        call(g::Node_runAction, {popup, anim});
        functionDestroy(fo);
    }
    const uint32_t shadow = ld32(functor + 0xC);
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), 0})});
}

// std::__sort4 / __sort5 / __insertion_sort_incomplete over int* with the shop's comparator
inline bool less(uint32_t a, uint32_t b) { return call(g::compare, {a, b}) == 1; }
inline void swap(uint32_t a, uint32_t b) {
    const uint32_t t = ld32(b);
    st32(b, ld32(a));
    st32(a, t);
}
uint32_t sort4(uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint32_t r = call(g::sort3, {a, b, c});
    if (!less(d, c)) return r;
    swap(c, d);
    if (!less(c, b)) return r + 1;
    swap(b, c);
    if (!less(b, a)) return r + 2;
    swap(a, b);
    return r + 3;
}
uint32_t sort5(uint32_t a, uint32_t b, uint32_t c, uint32_t d, uint32_t e) {
    uint32_t r = call(0x58C85D, {a, b, c, d});
    if (!less(e, d)) return r;
    swap(d, e);
    if (!less(d, c)) return r + 1;
    swap(c, d);
    if (!less(c, b)) return r + 2;
    swap(b, c);
    if (!less(b, a)) return r + 3;
    swap(a, b);
    return r + 4;
}
uint32_t insertionSortIncomplete(uint32_t first, uint32_t last) {
    switch ((last - first) >> 2) {
    case 0:
    case 1: return 1;
    case 2:
        if (less(last - 4, first)) swap(first, last - 4);
        return 1;
    case 3: call(g::sort3, {first, first + 4, last - 4}); return 1;
    case 4: call(0x58C85D, {first, first + 4, first + 8, last - 4}); return 1;
    case 5: call(0x58C8D5, {first, first + 4, first + 8, first + 12, last - 4}); return 1;
    default: break;
    }
    uint32_t j = first + 8;
    call(g::sort3, {first, first + 4, j});
    Frame f;
    const uint32_t t = f.alloc(4);
    uint32_t count = 0;
    for (uint32_t i = j + 4; i != last; i += 4) {
        if (less(i, j)) {
            st32(t, ld32(i));
            uint32_t k = j, hole = i;
            do {
                st32(hole, ld32(k));
                hole = k;
            } while (hole != first && less(t, k -= 4));
            st32(hole, ld32(t));
            if (++count == 8) return i + 4 == last;
        }
        j = i;
    }
    return 1;
}
}  // namespace

RW_FUNCTION(0x589A28, "BuyMagicStoneDialog::openStoneInfoPopup()", openStoneInfoPopup);
RW_FUNCTION(0x58B944, "BuyMagicStoneDialog web info button A (sub_58B944)", webInfoA);
RW_FUNCTION(0x58BB88, "BuyMagicStoneDialog web info button B (sub_58BB88)", webInfoB);
RW_FUNCTION(0x58BF5C, "BuyMagicStoneDialog buy product (sub_58BF5C)", buyProduct);
RW_FUNCTION(0x58C0D8, "BuyMagicStoneDialog purchase result (sub_58C0D8)", purchased);
RW_FUNCTION(0x58CC34, "BuyMagicStoneDialog restore (sub_58CC34)", restore);
RW_FUNCTION(0x58CEF4, "BuyMagicStoneDialog restore result (sub_58CEF4)", restored);
RW_FUNCTION(0x58D420, "BuyMagicStoneDialog owned Jewels result (sub_58D420)", stoneInfoResult);
RW_FUNCTION(0x58C85C, "std::__sort4 (shop order) (sub_58C85C)", sort4);
RW_FUNCTION(0x58C8D4, "std::__sort5 (shop order) (sub_58C8D4)", sort5);
RW_FUNCTION(0x58C96C, "std::__insertion_sort_incomplete (shop order) (sub_58C96C)", insertionSortIncomplete);
