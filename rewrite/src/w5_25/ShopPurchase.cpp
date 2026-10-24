// Shop > Buy Jewels, the purchase side: BuyMagicStoneDialog::openDiscount (the discounted pack flow, init type 1)
// and its pack list (initItemIDsAndProductIDsDiscount: the Misc 705 pack, or Misc 706 when that one is off sale),
// purchaseCallback (the store's answer: the success popup, an error dialog or a "could not purchase" notice, by
// PurchaseManager's state) with its success lambda $_11, getSkipTicketCnt (a pack's stage skip tickets, unless
// its benefit is already used up), SceneUtil::openPurchaseSucceededPopup ("n Jewels [and m skip tickets]
// purchased"), the Google Play side (iap::StoreManagerImpl::purchase, the JNI onPurchaseFinish callback) and
// bg::ioutil::_write<std::string> (a length-prefixed string into a MemoryStream).
#include "../master/Shop.hpp"
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t PurchaseManager_getInstance = 0x4D7A71;
constexpr uint32_t PurchaseManager_init = 0x4D7B51;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t SceneManager_getPopupLayer3 = 0x613E79;
constexpr uint32_t SceneManager_openPopup = 0x61524D;           // (mgr, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;       // (api, const function<void(bool)>&, bool)
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5;     // (scroll, bool)
constexpr uint32_t PopupDialog_create = 0x4F3C15;               // (ButtonType, WindowType)
constexpr uint32_t PopupDialog_open = 0x4F420D;                 // (popup, Node* layer, const string&, function<void(bool)>)
constexpr uint32_t BuyMagicStoneDialog_init = 0x588E0D;         // (callback, Type)
constexpr uint32_t BuyMagicStoneDialog_updatePaymentInfo = 0x58A4E9;
constexpr uint32_t SceneUtil_openPurchaseSucceededPopup = 0x478EDD;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;   // (node, function<void()>)
constexpr uint32_t SceneUtil_textToCustomTextView = 0x45BF15;       // (Text*, const string& name)
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t TextData_format = 0x492929;                  // (string out, Type, id, ...)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t VectorInt_pushBackSlow = 0x4D6CC1;           // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t VectorString_pushBackSlow = 0x484465;        // vector<string>::__push_back_slow_path(string&&)
constexpr uint32_t Shop_create = 0x790951;                      // master::Shop::create(shared_ptr out, int id)
constexpr uint32_t FunctionVB_call = 0x474139;                  // std::function<void(bool)>::operator()(bool)
// iap
constexpr uint32_t StoreManager_getInstance = 0x4E0DD5;
constexpr uint32_t FunctionPurchase_call = 0x4E1921;  // function<void(Result, const Purchase*)>::operator()
constexpr uint32_t StringAssignCopy = 0x498A1D;       // std::string::operator=(const string&) (sub_498A1C)
constexpr uint32_t JniHelper_getStaticMethodInfo = rwlib::cocos2d::JniHelper::getStaticMethodInfo;    // (info, cls, name, sig)
constexpr uint32_t JniHelper_getMethodInfo = rwlib::cocos2d::JniHelper::getMethodInfo;
constexpr uint32_t JNIEnv_CallStaticObjectMethod = 0x4E0939;
constexpr uint32_t JNIEnv_CallVoidMethod = 0x4E17DD;
constexpr uint32_t base64Encode = 0x905B2D;           // (const uchar* in, uint len, char** out)
constexpr uint32_t free_ = 0x40FF64;
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t MemoryStream_write = 0x491A79;     // (stream, const void*, size, count)
}  // namespace g

// __func vtables (+8)
constexpr uint32_t VT_OpenDiscount = 0xF5A828;     // openDiscount $_1 {function<void()> onClose, function<void()> x}
constexpr uint32_t VT_PurchaseDone = 0xF5A594;     // purchaseCallback $_11 {Widget* dialog, function<void(bool)>}
constexpr uint32_t VT_SucceededOk = 0xF53B94;      // openPurchaseSucceededPopup $_123 (OK touch) {}
constexpr uint32_t VT_SucceededOpened = 0xF53B68;  // $_124 (open animation done) {}
constexpr uint32_t VT_SucceededCanClose = 0xF53B3C;   // $_126 bool() {}
constexpr uint32_t VT_SucceededClose = 0xF53AE4;   // $_127 {button, popup, layer, shadow, function<void(bool)>}

// strings
constexpr uint32_t S_ConfirmationJson = 0xDF9E1E;  // "cocostudio/publish/ShopBuyItem_confirmation2.json"
constexpr uint32_t S_Base_Jewel1 = 0xDF9E50, S_confirmation_Jewel1 = 0xDF9E5C;
constexpr uint32_t S_Base_Jewel2 = 0xDF9E70, S_confirmation_Jewel2 = 0xDF9E7C;
constexpr uint32_t S_Base_Ticket = 0xDF9E90, S_confirmation_Ticket = 0xDF9E9C;
constexpr uint32_t S_Button_OK = 0xDF8688, S_Txt_OK = 0xDF869F;
constexpr uint32_t S_GooglePlayManager = 0xDFC444;   // "org/cocos2dx/cpp/GooglePlayManager"
constexpr uint32_t S_GetInstance = 0xDFC411, S_GetInstanceSig = 0xDFC41D;
constexpr uint32_t S_Purchase = 0xDFC4DC, S_PurchaseSig = 0xDFC4E5;   // "purchase", "(Ljava/lang/String;)V"

// text ids
constexpr int32_t TXT_PURCHASE_FAILED = 105100010;   // purchase could not complete (state 2, no API dialog)
constexpr int32_t TXT_PURCHASE_CANCELLED = 105100003;   // state 0
constexpr int32_t TXT_JEWELS_BOUGHT = 102600030;     // "%d Jewels"
constexpr int32_t TXT_TICKETS_BOUGHT = 102602060;    // skip tickets
constexpr int32_t TXT_OK = 100100001;
constexpr int32_t MISC_DISCOUNT_PACK = 705, MISC_DISCOUNT_PACK_2 = 706;

constexpr uint32_t API_LOGGED_IN = 0x14, API_RET = 0x44, RET_FUNCTION_FLAGS = 0x34;
constexpr uint32_t API_USED_BENEFITS = 0xAC;          // vector<{int benefitId, ...}> (12-byte entries)
constexpr uint32_t PURCHASE_MANAGER_STATE = 0x28;     // 0 cancelled, 1 finished, 2 failed
constexpr uint32_t STORE_MANAGER = 0x10039E4;         // iap::StoreManager (shared_ptr<StoreManagerImpl>)
constexpr uint32_t IMPL_AVAILABLE = 0x38, IMPL_ON_PURCHASE = 0x20;   // + its callable pointer at +0x30
constexpr uint32_t RESULT_OK = 1, RESULT_FAILED = 2;
// iap::Purchase: product id (string), +0xC an int left uninitialised by these constructions, then strings at
// +0x10, +0x1C, +0x28 (receipt, base64), +0x34 (signature)
constexpr uint32_t PURCHASE_SIZE = 0x40;
// JNINativeInterface offsets
constexpr uint32_t JNI_DELETE_LOCAL_REF = 0x5C, JNI_GET_STRING_LENGTH = 0x290, JNI_NEW_STRING_UTF = 0x29C;
constexpr uint32_t JNI_GET_STRING_UTF_CHARS = 0x2A4;

uint32_t api() { return call(fn::APIManager_getInstance, {}); }
void loadingIndicator(uint32_t on) { call(g::SceneManager_enableLoadingIndicator, {sceneManager(), on}); }
uint32_t popupLayer3() { return call(g::SceneManager_getPopupLayer3, {sceneManager()}); }

// APIManager::openErrorDialog(an empty callback, true)
void openErrorDialogNoCallback(uint32_t a) {
    Frame f;
    uint32_t cb = newEmptyFunction(f);
    call(g::APIManager_openErrorDialog, {a, cb, 1});
    functionDestroy(cb);
}

// PopupDialog::open(popup, layer, TextData text, callback), then the popup sound
void openNotice(uint32_t popup, uint32_t layer, int32_t textId, uint32_t callback) {
    Frame f;
    uint32_t sp = textData(f, textId);
    uint32_t text = newStr(f, textDataStr(sp));
    call(g::PopupDialog_open, {popup, layer, text, callback});
    functionDestroy(callback);
    strFree(text);
    sharedRelease(sp);
    call(fn::AudioPlaySystemSEPopUp, {});
}

void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }

// std::string from a literal into the stack, used and freed
uint32_t nodeNamed(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
void toCustomTextView(uint32_t text, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    call(g::SceneUtil_textToCustomTextView, {text, s});
    strFree(s);
}
void setXmlC(uint32_t view, uint32_t cstr) {
    Frame f;
    uint32_t s = newStr(f, cstr);
    call(g::CustomTextView_setXml, {view, s});
    strFree(s);
}
// the Jewel count line: TextData 102600030 printf'ed with `jewels`
void setJewelsText(uint32_t popup, uint32_t name, int32_t jewels) {
    uint32_t view = nodeNamed(popup, name, 19);
    Frame f;
    uint32_t sp = textData(f, TXT_JEWELS_BOUGHT);
    uint32_t buf = stringBuffer(f, {textDataStr(sp), uint32_t(jewels)});
    setXmlC(view, buf);
    sharedRelease(sp);
}

void purchaseInit(uint32_t p) {     // the zero-initialised fields of an iap::Purchase built on the stack
    st32(p, 0);
    st32(p + 4, 0);
    st32(p + 8, 0);
    call(g::aeabi_memset, {p + 0x10, 0x30, 0});
}
void purchaseDestroy(uint32_t p) {
    strFree(p + 0x34);
    strFree(p + 0x28);
    strFree(p + 0x1C);
    strFree(p + 0x10);
    strFree(p);
}
// the purchase callback of the store (impl +0x20), when set
void notifyPurchase(uint32_t impl, uint32_t result, uint32_t purchase) {
    if (ld32(impl + IMPL_ON_PURCHASE + 16)) call(g::FunctionPurchase_call, {impl + IMPL_ON_PURCHASE, result, purchase});
}
uint32_t jni(uint32_t env, uint32_t offset, std::initializer_list<uint32_t> args) {
    return call(ld32(ld32(env) + offset), args);
}

struct ShopPurchase {
    // ------------------------------------------------------------------------------------------- BuyMagicStoneDialog
    // openDiscount(onClose, x): like open, with the discount packs (init type 1) and $_1 keeping both callbacks
    static void openDiscount(uint32_t onClose, uint32_t other) {
        call(g::PurchaseManager_init, {call(g::PurchaseManager_getInstance, {})});
        if (ld8(call(g::PurchaseManager_getInstance, {}))) {
            if (ld32(onClose + 16)) call(fn::function_vv_call, {onClose});
            return;
        }
        loadingIndicator(1);
        Frame f;
        uint32_t lambda = f.alloc(48);      // {function onClose, function other}
        call(fn::function_vv_copy, {lambda, onClose});
        call(fn::function_vv_copy, {lambda + 24, other});
        uint32_t cb = newEmptyFunction(f);
        uint32_t heap = call(fn::operator_new, {0x38});
        if (heap) {
            st32(heap, VT_OpenDiscount);
            functionCopy(heap + 8, lambda);
            functionCopy(heap + 0x20, lambda + 24);
        }
        st32(cb + 16, heap);
        call(g::BuyMagicStoneDialog_init, {cb, 1});
        functionDestroy(cb);
        functionDestroy(lambda + 24);
        functionDestroy(lambda);
    }

    // the discount pack: Misc 705's shop id, or Misc 706's when that pack is not on sale; its product id
    static void initItemIDsAndProductIDsDiscount(uint32_t itemIds, uint32_t productIds) {
        Frame f;
        uint32_t misc = masterRow(f, fn::Misc_create, MISC_DISCOUNT_PACK);
        int32_t shopId = safe32(ld32(misc), 4);
        sharedRelease(misc);
        uint32_t shop = masterRow(f, g::Shop_create, uint32_t(shopId));
        if (!safe32(ld32(shop), master::Shop::STATUS)) {
            uint32_t misc2 = masterRow(f, fn::Misc_create, MISC_DISCOUNT_PACK_2);
            shopId = safe32(ld32(misc2), 4);
            sharedRelease(misc2);
            uint32_t shop2 = masterRow(f, g::Shop_create, uint32_t(shopId));
            // shop = std::move(shop2)
            uint32_t oldCtrl = ld32(shop + 4);
            st32(shop, ld32(shop2));
            st32(shop + 4, ld32(shop2 + 4));
            st32(shop2, 0);
            st32(shop2 + 4, 0);
            if (oldCtrl) call(fn::shared_release, {oldCtrl});
        }
        uint32_t id = f.alloc(4);
        *rw::frame_ptr<int32_t>(id) = shopId;
        // itemIds.clear(), productIds.clear() (as inlined)
        uint32_t b = ld32(itemIds), e = ld32(itemIds + 4);
        if (e != b) st32(itemIds + 4, e + (~(e - 4 - b) & ~3u));
        uint32_t pb = ld32(productIds);
        for (uint32_t pe = ld32(productIds + 4); pe != pb;) {
            uint32_t s = pe - 12;
            st32(productIds + 4, s);
            if (ld8(s) & 1) {
                call(fn::operator_delete, {ld32(s + 8)});
                s = ld32(productIds + 4);
            }
            pe = s;
        }
        uint32_t ie = ld32(itemIds + 4);
        if (ie == ld32(itemIds + 8)) {
            call(g::VectorInt_pushBackSlow, {itemIds, id});
        } else {
            uint32_t n = 0;
            if (ie) {
                st32(ie, uint32_t(shopId));
                n = ld32(itemIds + 4);
            }
            st32(itemIds + 4, n + 4);
        }
        uint32_t s = newStr(f, safeBytes(f, ld32(shop), master::Shop::PRODUCT_ID, 0x81));
        uint32_t pe = ld32(productIds + 4);
        if (pe >= ld32(productIds + 8)) {
            call(g::VectorString_pushBackSlow, {productIds, s});
        } else {
            uint32_t n = 0;
            if (pe) {
                st32(pe, ld32(s));
                st32(pe + 4, ld32(s + 4));
                st32(pe + 8, ld32(s + 8));
                st32(s + 8, 0);
                st32(s + 4, 0);
                st32(s, 0);
                n = ld32(productIds + 4);
            }
            st32(productIds + 4, n + 12);
        }
        strFree(s);
        sharedRelease(shop);
    }

    // the store's answer to a purchase (PurchaseManager state: 1 finished, 2 failed, 0 cancelled)
    static void purchaseCallback(uint32_t ok, int32_t jewels, uint32_t scroll, uint32_t dialog, int32_t tickets,
                                 uint32_t onSuccess, uint32_t onClose) {
        call(g::SceneManager_updateHeader, {sceneManager()});
        if (scroll) call(g::IScrollProtocol_updateLayout, {scroll, 0});
        uint32_t popup = call(g::PopupDialog_create, {0, 0});
        uint32_t state = ld32(call(g::PurchaseManager_getInstance, {}) + PURCHASE_MANAGER_STATE);
        if (state == 2) {
            loadingIndicator(0);
            if (ld8(api() + API_LOGGED_IN)) {
                openErrorDialogNoCallback(api());
                return;
            }
            uint32_t layer = popupLayer3();
            uint32_t notice = call(g::PopupDialog_create, {0, 0});
            Frame f;
            openNotice(notice, layer, TXT_PURCHASE_FAILED, newEmptyFunction(f));
        } else if (state == 1) {
            loadingIndicator(0);
            if (ok != 1) {
                loadingIndicator(0);
                openErrorDialogNoCallback(api());
            } else {
                Frame f;
                uint32_t copy = f.alloc(20);
                call(fn::function_vb_copy, {copy, onSuccess});
                uint32_t layer = popupLayer3();
                uint32_t lambda = f.alloc(20);      // $_11's onSuccess
                functionCopy(lambda, copy);
                uint32_t cb = newEmptyFunction(f);
                uint32_t heap = call(fn::operator_new, {0x28});
                if (heap) {
                    st32(heap, VT_PurchaseDone);
                    st32(heap + 8, dialog);
                    functionCopy(heap + 0x10, lambda);
                }
                st32(cb + 16, heap);
                call(g::SceneUtil_openPurchaseSucceededPopup, {layer, uint32_t(jewels), uint32_t(tickets), cb});
                functionDestroy(cb);
                functionDestroy(lambda);
                functionDestroy(copy);
                return;
            }
        } else {
            if (state == 0) {
                loadingIndicator(0);
                uint32_t layer = popupLayer3();
                Frame f;
                uint32_t sp = textData(f, TXT_PURCHASE_CANCELLED);
                uint32_t text = newStr(f, textDataStr(sp));
                uint32_t cb = f.alloc(20);
                call(fn::function_vb_copy, {cb, onClose});
                call(g::PopupDialog_open, {popup, layer, text, cb});
                functionDestroy(cb);
                strFree(text);
                sharedRelease(sp);
                call(fn::AudioPlaySystemSEPopUp, {});
            }
            return;
        }
        if (ld32(onClose + 16)) call(g::FunctionVB_call, {onClose, 1});
    }

    // $_11 (bool): the success popup closed - refresh the dialog's payment info, then onSuccess(true)
    static void purchaseDone(uint32_t functor, uint32_t) {
        uint32_t dialog = ld32(functor + 8);
        if (dialog) call(g::BuyMagicStoneDialog_updatePaymentInfo, {dialog});
        if (ld32(functor + 0x20)) call(g::FunctionVB_call, {functor + 0x10, 1});
    }

    // the stage skip tickets of a pack: none once its benefit (shop +BENEFIT_ID) is in the API's used list, when
    // the server enables benefits (function flag bit 2) and the pack has one (+APPLE_STATUS)
    static int32_t getSkipTicketCnt(int32_t shopId) {
        Frame f;
        uint32_t shop = masterRow(f, g::Shop_create, uint32_t(shopId));
        uint32_t row = ld32(shop);
        int32_t tickets = safe32(row, master::Shop::STAGE_SKIP_TICKET);
        int32_t result = tickets;
        if (safe32(row, master::Shop::APPLE_STATUS)) {
            uint32_t ret = ld32(api() + API_RET);
            if (ret && (ld8(ret + RET_FUNCTION_FLAGS) & 4)) {
                uint32_t used = ld32(api() + API_USED_BENEFITS);
                result = 0;
                if (ld32(used + 4) != ld32(used)) {
                    for (uint32_t i = 0;;) {
                        uint32_t benefit = ld32(ld32(used) + 12 * i);
                        int32_t benefitId = safe32(row, master::Shop::BENEFIT_ID);
                        ++i;
                        if (benefit == uint32_t(benefitId)) {
                            result = tickets;
                            break;
                        }
                        if (i >= (ld32(used + 4) - ld32(used)) / 12) break;
                    }
                }
            }
        }
        sharedRelease(shop);
        return result;
    }

    // ------------------------------------------------------------------------------------------- SceneUtil
    // "Purchased": n Jewels (and m stage skip tickets when the pack has some); OK closes it and calls onClose
    static void openPurchaseSucceededPopup(uint32_t layer, int32_t jewels, int32_t tickets, uint32_t onClose) {
        call(fn::AudioPlaySystemSEPopUp, {});
        uint32_t shadow = SceneUtil::createShadowSprite();
        uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
        vcall(layer, node::addChild, {layer, shadow});
        uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_ConfirmationJson});
        vcall(shadow, node::addChild, {shadow, popup});
        Frame f;
        {
            Frame t;
            uint32_t anchor = t.alloc(8);
            call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
            vcall(popup, node::setAnchorPoint, {popup, anchor});
            call(g::Vec2_dtor, {anchor});
        }
        vcall(popup, node::setPosition, {popup, vcall(shadow, node::getPosition, {shadow})});
        uint32_t onCloseCopy = f.alloc(20);
        call(fn::function_vb_copy, {onCloseCopy, onClose});
        uint32_t jewel1 = nodeNamed(popup, S_Base_Jewel1, 11);
        uint32_t jewel1Text = nodeNamed(popup, S_confirmation_Jewel1, 19);
        uint32_t jewel2 = nodeNamed(popup, S_Base_Jewel2, 11);
        uint32_t jewel2Text = nodeNamed(popup, S_confirmation_Jewel2, 19);
        uint32_t ticket = nodeNamed(popup, S_Base_Ticket, 11);
        uint32_t ticketText = nodeNamed(popup, S_confirmation_Ticket, 19);
        if (tickets >= 1) {
            setVisible(jewel1, 0);
            setVisible(jewel1Text, 0);
            setVisible(jewel2, 1);
            setVisible(jewel2Text, 1);
            setVisible(ticket, 1);
            setVisible(ticketText, 1);
            toCustomTextView(jewel2Text, S_confirmation_Jewel2, 19);
            toCustomTextView(ticketText, S_confirmation_Ticket, 19);
            setJewelsText(popup, S_confirmation_Jewel2, jewels);
            uint32_t view = nodeNamed(popup, S_confirmation_Ticket, 19);
            Frame t;
            uint32_t text = t.alloc(12);
            call(g::TextData_format, {text, 0, uint32_t(TXT_TICKETS_BOUGHT), uint32_t(tickets)});
            setXmlC(view, strData(text));
            strFree(text);
        } else {
            setVisible(jewel1, 1);
            setVisible(jewel1Text, 1);
            setVisible(jewel2, 0);
            setVisible(jewel2Text, 0);
            setVisible(ticket, 0);
            setVisible(ticketText, 0);
            toCustomTextView(jewel1Text, S_confirmation_Jewel1, 19);
            setJewelsText(popup, S_confirmation_Jewel1, jewels);
        }
        uint32_t ok = nodeNamed(popup, S_Button_OK, 9);
        titleLabel(ok, S_Txt_OK, 6, TXT_OK);
        addTouch(ok, VT_SucceededOk, {});
        {
            Frame t;
            uint32_t opened = newFunction(t, VT_SucceededOpened, {});
            call(fn::Node_runAction, {popup, call(g::SceneUtil_createOpenPopupAnimation, {popup, opened})});
            functionDestroy(opened);
        }
        // $_127 {button, popup, layer, shadow, onClose}, through two lambda temporaries
        uint32_t lambda1 = f.alloc(20);
        functionCopy(lambda1, onCloseCopy);
        uint32_t mgr = sceneManager();
        uint32_t canClose = newFunction(f, VT_SucceededCanClose, {});
        uint32_t lambda2 = f.alloc(20);
        functionCopy(lambda2, lambda1);
        uint32_t cb = newEmptyFunction(f);
        uint32_t heap = call(fn::operator_new, {0x38});
        if (heap) {
            st32(heap, VT_SucceededClose);
            st32(heap + 8, ok);
            st32(heap + 0x10, popup);
            st32(heap + 0x14, layer);
            st32(heap + 0x18, shadow);
            call(fn::function_vb_move, {heap + 0x20, lambda2});
        }
        st32(cb + 16, heap);
        call(g::SceneManager_openPopup, {mgr, canClose, cb, 1});
        functionDestroy(cb);
        functionDestroy(lambda2);
        functionDestroy(canClose);
        functionDestroy(lambda1);
        functionDestroy(onCloseCopy);
    }

    // ------------------------------------------------------------------------------------------- iap
    // Start a Google Play purchase of `productId`; without billing (or the Java side) the purchase callback gets
    // RESULT_FAILED with a Purchase holding just the product id
    static void purchase(uint32_t impl, uint32_t productId) {
        // the original frame: the Purchase records sit at fixed offsets and their +0xC int is never initialised
        guest::Frame frame(0x108);
        uint32_t sp = frame.addr();
        auto fail = [&](uint32_t p) {
            purchaseInit(p);
            call(g::StringAssignCopy, {p, productId});
            notifyPurchase(impl, RESULT_FAILED, p);
            purchaseDestroy(p);
        };
        if (!ld8(impl + IMPL_AVAILABLE)) return fail(sp + 0xB0);
        uint32_t s = sp + 0xA4;
        if (!call(g::JniHelper_getStaticMethodInfo, {s, S_GooglePlayManager, S_GetInstance, S_GetInstanceSig}))
            return fail(sp + 0x64);
        uint32_t manager = call(g::JNIEnv_CallStaticObjectMethod, {ld32(s), ld32(s + 4), ld32(s + 8)});
        uint32_t m = sp + 0x58;
        if (!call(g::JniHelper_getMethodInfo, {m, S_GooglePlayManager, S_Purchase, S_PurchaseSig})) return fail(sp + 0x18);
        uint32_t env = ld32(m);
        uint32_t jid = jni(env, JNI_NEW_STRING_UTF, {env, strData(productId)});
        call(g::JNIEnv_CallVoidMethod, {ld32(m), manager, ld32(m + 8), jid});
        uint32_t e = ld32(s);
        jni(e, JNI_DELETE_LOCAL_REF, {e, jid});
        jni(ld32(s), JNI_DELETE_LOCAL_REF, {ld32(s), manager});
        jni(ld32(s), JNI_DELETE_LOCAL_REF, {ld32(s), ld32(m + 4)});
        jni(ld32(s), JNI_DELETE_LOCAL_REF, {ld32(s), ld32(s + 4)});
    }

    // GooglePlayManager.onPurchaseFinish(boolean ok, String productId, String receipt, String signature): the
    // Purchase (receipt base64-encoded) to the store's purchase callback
    static void onPurchaseFinish(uint32_t env, uint32_t, uint32_t ok, uint32_t jProduct, uint32_t jReceipt,
                                 uint32_t jSignature) {
        guest::Frame frame(0x78);
        uint32_t sp = frame.addr();
        uint32_t product = jni(env, JNI_GET_STRING_UTF_CHARS, {env, jProduct, 0});
        uint32_t receipt = jni(env, JNI_GET_STRING_UTF_CHARS, {env, jReceipt, 0});
        uint32_t signature = jni(env, JNI_GET_STRING_UTF_CHARS, {env, jSignature, 0});
        uint32_t p = sp + 0x20;
        purchaseInit(p);
        call(fn::string_assign, {p, product, host_strlen(product)});
        call(fn::string_assign, {p + 0x34, signature, host_strlen(signature)});
        uint32_t len = jni(env, JNI_GET_STRING_LENGTH, {env, jReceipt});
        uint32_t out = sp + 0x1C;
        st32(out, 0);
        call(g::base64Encode, {receipt, len, out});
        call(fn::string_assign, {p + 0x28, ld32(out), host_strlen(ld32(out))});
        call(g::free_, {ld32(out)});
        uint32_t result = ok ? RESULT_OK : RESULT_FAILED;
        call(g::StoreManager_getInstance, {});
        notifyPurchase(ld32(STORE_MANAGER), result, p);
        purchaseDestroy(p);
    }

    // ------------------------------------------------------------------------------------------- bg::ioutil
    // a std::string as its length (4 bytes) then its characters one by one; false when the stream refuses one
    static uint32_t writeString(uint32_t str, uint32_t stream) {
        Frame f;
        uint32_t len = f.alloc(4);
        *rw::frame_ptr<uint32_t>(len) = strSize(str);
        if (call(g::MemoryStream_write, {stream, len, 4, 1}) != 1) return 0;
        uint32_t p = strData(str), end = p + strSize(str);
        for (; p != end; ++p)
            if (call(g::MemoryStream_write, {stream, p, 1, 1}) != 1) return 0;
        return 1;
    }
};
}  // namespace

RW_FUNCTION(0x589004, "BuyMagicStoneDialog::openDiscount(const function<void()>&, const function<void()>&)",
            ShopPurchase::openDiscount);
RW_FUNCTION(0x589414, "BuyMagicStoneDialog::initItemIDsAndProductIDsDiscount(vector<int>&, vector<string>&)",
            ShopPurchase::initItemIDsAndProductIDsDiscount);
RW_FUNCTION(0x58A7A0, "BuyMagicStoneDialog::purchaseCallback", ShopPurchase::purchaseCallback);
RW_FUNCTION(0x58C33C, "BuyMagicStoneDialog::purchaseCallback $_11 __func::operator()(bool) (sub_58C33C)",
            ShopPurchase::purchaseDone);
RW_FUNCTION(0x58AC40, "BuyMagicStoneDialog::getSkipTicketCnt(int)", ShopPurchase::getSkipTicketCnt);
RW_FUNCTION(0x478EDC, "SceneUtil::openPurchaseSucceededPopup(Node*, int, int, function<void(bool)>)",
            ShopPurchase::openPurchaseSucceededPopup);
RW_FUNCTION(0x4E10E0, "iap::StoreManagerImpl::purchase(const string&)", ShopPurchase::purchase);
RW_FUNCTION(0x4E1D0C, "Java_org_cocos2dx_cpp_GooglePlayManager_onPurchaseFinish", ShopPurchase::onPurchaseFinish);
RW_FUNCTION(0x4D9254, "bg::ioutil::_write<std::string>(const string&, MemoryStream*, const vector_tag&)",
            ShopPurchase::writeString);
