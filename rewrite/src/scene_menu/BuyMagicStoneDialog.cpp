// BuyMagicStoneDialog: the Buy Jewels flow (all static: the dialog object is built by the callback of init).
// open -> init: pick the Jewel packs (master::Shop rows of this OS, sorted by sortId), ask the server for the
// payment info (/payment/info, the player's monthly billing), then the shop state (/user/shop), then the store for
// the products' prices (iap::StoreManager::sendProductsRequest), then call back with the ids and the prices.
// The three info buttons (Jewel info, Fund Settlement Act, Specified Commercial Transactions Act) are shown when
// the server's function flags enable them. Then the shop popup (open $_0: the pack list) or the one-pack discount
// popup (openDiscount $_1) and their lambdas. openStoneInfoPopup is in b5_menu/BuyMagicStoneDialog_leftovers.cpp,
// purchaseCallback in w5_25/ShopPurchase.cpp.
#include "../master/Shop.hpp"
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
using Addr = uint32_t;
namespace g {
constexpr uint32_t PurchaseManager_getInstance = 0x4D7A71;
constexpr uint32_t PurchaseManager_init = 0x4D7B51;
constexpr uint32_t PurchaseManager_hasSuspendedTransaction = 0x4D7D8D;
constexpr uint32_t PurchaseManager_getSuspendedTransactionProductIDs = 0x4D7DB1;   // (unordered_set<string>* out, mgr)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_getPopupLayer3 = 0x613E79;
constexpr uint32_t APIManager_executePaymentInfo = 0x4CE74D;    // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_executeUserShopGet = 0x4C6CD5;    // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;       // (api, const function<void(bool)>&, bool)
constexpr uint32_t StoreManager_getInstance = 0x4E0DD5;
constexpr uint32_t StoreManager_cancelProductsRequest = 0x4E10BD;
constexpr uint32_t StoreManager_sendProductsRequest = 0x4E0F0B;  // (store, const unordered_set<string>&, const function&)
constexpr uint32_t PopupDialog_create = 0x4F3C15;               // (ButtonType, WindowType)
constexpr uint32_t PopupDialog_open = 0x4F420D;                 // (popup, Node* layer, const string&, function<void(bool)>)
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;      // (const string&, function<void(bool)>, const char*)
constexpr uint32_t Shop_create = 0x790951;                      // master::Shop::create(shared_ptr out, int id)
constexpr uint32_t Shop_getShopIds = 0x790935;                  // (std::set<int>* out)
constexpr uint32_t IntTree_destroy = 0x48D9B5;                  // std::__tree<int>::destroy(tree, node)
constexpr uint32_t VectorInt_copy = 0x4AF707;                   // vector<int>(const vector&)
constexpr uint32_t VectorInt_pushBackSlow = 0x4D6CC1;           // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t VectorBaseInt_dtor = 0x478961;
constexpr uint32_t VectorString_copy = 0x47A9A5;                // vector<string>(const vector&)
constexpr uint32_t VectorString_pushBackSlow = 0x484465;        // vector<string>::__push_back_slow_path(string&&)
constexpr uint32_t VectorBaseString_dtor = 0x47363F;
constexpr uint32_t StringSet_insert = 0x4DA505;                 // unordered_set<string>::__insert_unique(pair* out, set, const string&)
constexpr uint32_t StringSet_dtor = 0x4D6DD1;
constexpr uint32_t String_compareC = 0x47CBD5;                  // std::string::compare(const char*)
constexpr uint32_t FunctionVB_call = 0x474139;                  // std::function<void(bool)>::operator()(bool)
// std::function<void(const vector<int>&, const unordered_map<string, ProductInfo>*, bool)>
constexpr uint32_t FunctionProducts_copy = 0x589771;
constexpr uint32_t FunctionProducts_call = 0x58E311;
// std::function<void(bool, const unordered_map<string, ProductInfo>*)>
constexpr uint32_t FunctionStore_copy = 0x58996D;
constexpr uint32_t FunctionStore_call = 0x5899A1;
constexpr uint32_t GetSkipTicketCnt = 0x58AC41;                 // BuyMagicStoneDialog::getSkipTicketCnt(int shopId)
// the libc++ sort of the pack ids (by sortId)
constexpr uint32_t SortIds = 0x58C399, SortComp = 0x58C709, Sort3 = 0x58C7C1, Sort4 = 0x58C85D, Sort5 = 0x58C8D5;
constexpr uint32_t InsertionSortIncomplete = 0x58C96D;
}  // namespace g

// __func vtables (+8) of the lambdas
constexpr uint32_t VT_Open = 0xF5AA38;               // open $_0 {function<void()> onClose, bool}
constexpr uint32_t VT_Init = 0xF5A7D0;               // init $_2 {vector<string> productIds, callback, vector<int> itemIds}
constexpr uint32_t VT_InitProducts = 0xF5A7FC;       // init $_2's inner lambda {callback, vector<int> itemIds}
constexpr uint32_t VT_PaymentInfo = 0xF5A74C;        // callGetPaymentInfo $_3 {function<void(bool)>}
constexpr uint32_t VT_ShopGet = 0xF5A778;            // $_3's inner lambda {function<void(bool)>}
constexpr uint32_t VT_ErrorDialog = 0xF5A7A4;        // $_3's error dialog lambda {}
constexpr uint32_t VT_StoreProducts = 0xF5A720;      // callSendProductsRequest lambda {function<void(bool, map*)>}
constexpr uint32_t VT_SuspendedPopup = 0xF5A5C0;     // checkTransaction $_9 {function<void(bool)>}
constexpr uint32_t VT_OwnedPopup = 0xF5A53C;         // hasOwnedItem lambda {string productId, int jewels, int tickets}
constexpr uint32_t VT_JewelInfo = 0xF5A510;          // initJewelInfoButton $_13
constexpr uint32_t VT_FundSettlement = 0xF5A4E4;     // initFundSettlementButton $_14
constexpr uint32_t VT_Tokushoho = 0xF5A4B8;          // initTokushohoButton $_15

// strings
constexpr uint32_t S_BillingLabel = 0xDFFF06;        // "Txt_Billing_Label"
constexpr uint32_t S_LimitLabel = 0xDFFF18;          // "Txt_limit_Label"
constexpr uint32_t S_Owned = 0xDFFF28;               // "owned."
constexpr uint32_t S_TxtCrownBuy1 = 0xDFFF2F, S_ButtonCrownBuy1 = 0xDFFF3D;
constexpr uint32_t S_TxtCrownBuy2 = 0xDFFF4E, S_ButtonCrownBuy2 = 0xDFFF5C;
constexpr uint32_t S_TxtCrownBuy3 = 0xDFFF6D, S_ButtonCrownBuy3 = 0xDFFF7B;

// text ids
constexpr int32_t TXT_BILLING = 102600044;           // billed this month
constexpr int32_t TXT_LIMIT = 102600045;             // monthly limit
constexpr int32_t TXT_MAX_MAGIC_STONE = 102600048;   // holding the maximum of Jewels
constexpr int32_t TXT_JEWEL_INFO = 102600024, TXT_FUND_SETTLEMENT = 102600025, TXT_TOKUSHOHO = 102600026;
constexpr int32_t TXT_SUSPENDED_TRANSACTION = 105100008;
constexpr int32_t TXT_OWNED_ITEM = 105100009;
constexpr int32_t MISC_PURCHASE_LIMIT = 118;
constexpr int32_t SHOP_STATUS_ON_SALE = 1, SHOP_OS_ANDROID = 2;
constexpr int32_t MINOR_AGE = 19;                    // payment info age up to this: show the billing limit

// APIManager members
constexpr uint32_t API_LOGGED_IN = 0x14;             // byte: show error dialogs
constexpr uint32_t API_RET = 0x44, RET_FUNCTION_FLAGS = 0x34;
constexpr uint32_t API_PAYMENT_INFO = 0x1D4;         // shared_ptr<PaymentInfo> (SafeBuffer: age 0, billed 4, rest 8)

// std::function copy into heap __func storage: allocate, store the vtable, clone `src` into `heap + fnOffset`
uint32_t heapFunc(uint32_t vtable, uint32_t size, uint32_t fnOffset, uint32_t src) {
    uint32_t heap = call(fn::operator_new, {size});
    if (heap) {
        st32(heap, vtable);
        functionCopy(heap + fnOffset, src);
    }
    return heap;
}
// a std::function on the stack holding the heap __func `heap` (0 when the allocation failed)
uint32_t heapFunction(Frame &f, uint32_t heap) {
    uint32_t fo = f.alloc(24);
    st32(fo + 16, heap);
    return fo;
}
void moveVector(uint32_t dst, uint32_t src) {
    for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
    for (uint32_t k = 0; k < 12; k += 4) st32(src + k, 0);
}
uint32_t api() { return call(fn::APIManager_getInstance, {}); }
void loadingIndicator(uint32_t on) { call(g::SceneManager_enableLoadingIndicator, {sceneManager(), on}); }
// the connection error dialog of the payment / shop requests (only when the API layer reports errors)
void openErrorDialog() {
    if (!ld8(api() + API_LOGGED_IN)) return;
    uint32_t a = api();
    Frame f;
    uint32_t fo = newFunction(f, VT_ErrorDialog, {});
    call(g::APIManager_openErrorDialog, {a, fo, 1});
    functionDestroy(fo);
}
// std::set<int> in-order successor
uint32_t treeNext(uint32_t node) {
    uint32_t r = ld32(node + 4);
    if (r) {
        while (ld32(r)) r = ld32(r);
        return r;
    }
    uint32_t p;
    do {
        p = ld32(node + 8);
        bool left = ld32(p) == node;
        node = p;
        if (left) break;
    } while (true);
    return p;
}
uint32_t retFlags(uint32_t byteOffset) {
    uint32_t ret = ld32(api() + API_RET);
    return ret ? ld8(ret + RET_FUNCTION_FLAGS + byteOffset) : 0;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ open / init
// open(onClose, flag): nothing to buy while a purchase runs (onClose right away); otherwise load the packs
static void BuyMagicStoneDialog_open(Addr onClose, uint32_t flag) {
    call(g::PurchaseManager_init, {call(g::PurchaseManager_getInstance, {})});
    if (ld8(call(g::PurchaseManager_getInstance, {}))) {
        if (ld32(onClose + 16)) call(fn::function_vv_call, {onClose});
        return;
    }
    loadingIndicator(1);
    Frame f;
    uint32_t lambda = f.alloc(32);
    call(fn::function_vv_copy, {lambda, onClose});
    st8(lambda + 0x18, uint8_t(flag));
    uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, VT_Open);
        functionCopy(heap + 8, lambda);
        st8(heap + 0x20, ld8(lambda + 0x18));
    }
    uint32_t fo = heapFunction(f, heap);
    call(0x588E0D, {fo, 0});   // init(callback, Type 0)
    functionDestroy(fo);
    functionDestroy(lambda);
}
RW_FUNCTION(0x588CAC, "BuyMagicStoneDialog::open(const function<void()>&, bool)", BuyMagicStoneDialog_open);

// init(callback, type): the pack ids / product ids (type 0 normal, 1 discount), then the payment info
static void BuyMagicStoneDialog_init(Addr callback, uint32_t type) {
    Frame f;
    uint32_t productIds = f.alloc(12), itemIds = f.alloc(12);
    for (uint32_t k = 0; k < 12; k += 4) {
        st32(itemIds + k, 0);
        st32(productIds + k, 0);
    }
    if (type == 1) call(0x589415, {itemIds, productIds});   // initItemIDsAndProductIDsDiscount
    else if (type == 0) call(0x589209, {itemIds, productIds});
    loadingIndicator(1);
    uint32_t lambda = f.alloc(0x38);
    call(g::VectorString_copy, {lambda, productIds});
    call(g::FunctionProducts_copy, {lambda + 0x10, callback});
    call(g::VectorInt_copy, {lambda + 0x28, itemIds});
    uint32_t heap = call(fn::operator_new, {0x40});
    if (heap) {
        st32(heap, VT_Init);
        moveVector(heap + 8, lambda);
        functionCopy(heap + 0x18, lambda + 0x10);
        moveVector(heap + 0x30, lambda + 0x28);
    }
    uint32_t fo = heapFunction(f, heap);
    call(0x589639, {fo});   // callGetPaymentInfo
    functionDestroy(fo);
    call(g::VectorBaseInt_dtor, {lambda + 0x28});
    functionDestroy(lambda + 0x10);
    call(g::VectorBaseString_dtor, {lambda});
    call(g::VectorBaseString_dtor, {productIds});
    call(g::VectorBaseInt_dtor, {itemIds});
}
RW_FUNCTION(0x588E0C, "BuyMagicStoneDialog::init(const function<...>&, Type)", BuyMagicStoneDialog_init);

// The Jewel packs on sale for this OS (shop ids, sorted by sortId) and their store product ids
static void BuyMagicStoneDialog_initItemIDsAndProductIDs(Addr itemIds, Addr productIds) {
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
    api();
    Frame f;
    uint32_t ids = f.alloc(12);
    call(g::Shop_getShopIds, {ids});
    const uint32_t end = ids + 4;
    for (uint32_t node = ld32(ids); node != end; node = treeNext(node)) {
        Frame t;
        uint32_t shop = masterRow(t, g::Shop_create, ld32(node + 0x10));
        uint32_t row = ld32(shop);
        if (safe32(row, master::Shop::STATUS) == SHOP_STATUS_ON_SALE &&
            safe32(row, master::Shop::OS) == SHOP_OS_ANDROID) {
            uint32_t ie = ld32(itemIds + 4);
            if (ie == ld32(itemIds + 8)) {
                call(g::VectorInt_pushBackSlow, {itemIds, node + 0x10});
            } else {
                uint32_t n = 0;
                if (ie) {
                    st32(ie, ld32(node + 0x10));
                    n = ld32(itemIds + 4);
                }
                st32(itemIds + 4, n + 4);
            }
            uint32_t s = newStr(t, safeBytes(t, row, master::Shop::PRODUCT_ID, 0x81));
            uint32_t pe = ld32(productIds + 4);
            if (pe >= ld32(productIds + 8)) {
                call(g::VectorString_pushBackSlow, {productIds, s});
            } else {
                uint32_t n = 0;
                if (pe) {
                    st32(pe, ld32(s));
                    st32(pe + 4, ld32(s + 4));
                    st32(pe + 8, ld32(s + 8));
                    st32(s + 4, 0);
                    st32(s + 8, 0);
                    st32(s, 0);
                    n = ld32(productIds + 4);
                }
                st32(productIds + 4, n + 12);
            }
            strFree(s);
        }
        sharedRelease(shop);
    }
    call(g::SortIds, {ld32(itemIds), ld32(itemIds + 4)});
    call(g::IntTree_destroy, {ids, ld32(ids + 4)});
}
RW_FUNCTION(0x589208, "BuyMagicStoneDialog::initItemIDsAndProductIDs(vector<int>&, vector<string>&)",
            BuyMagicStoneDialog_initItemIDsAndProductIDs);

// ------------------------------------------------------------------------------------------------ requests
static void BuyMagicStoneDialog_callGetPaymentInfo(Addr callback) {
    uint32_t a = api();
    Frame f;
    uint32_t copy = f.alloc(24);
    call(fn::function_vb_copy, {copy, callback});
    uint32_t fo = heapFunction(f, 0);
    st32(fo + 16, heapFunc(VT_PaymentInfo, 0x28, 0x10, copy));
    call(g::APIManager_executePaymentInfo, {a, fo});
    functionDestroy(fo);
    functionDestroy(copy);
}
RW_FUNCTION(0x589638, "BuyMagicStoneDialog::callGetPaymentInfo(const function<void(bool)>&)",
            BuyMagicStoneDialog_callGetPaymentInfo);

// ask the store for the products (and for those of suspended transactions); callback(false, nullptr) when the
// request could not be sent
static void BuyMagicStoneDialog_callSendProductsRequest(Addr productIds, Addr callback) {
    call(g::StoreManager_cancelProductsRequest, {call(g::StoreManager_getInstance, {})});
    Frame f;
    uint32_t set = f.alloc(20);
    for (uint32_t k = 0; k < 16; k += 4) st32(set + k, 0);
    stf(set + 16, 1.0f);
    uint32_t result = f.alloc(8);
    for (uint32_t p = ld32(productIds), e = ld32(productIds + 4); p != e; p += 12)
        call(g::StringSet_insert, {result, set, p});
    uint32_t suspended = f.alloc(20);
    call(g::PurchaseManager_getSuspendedTransactionProductIDs, {suspended, call(g::PurchaseManager_getInstance, {})});
    for (uint32_t node = ld32(suspended + 8); node; node = ld32(node))
        call(g::StringSet_insert, {result, set, node + 8});
    call(g::StringSet_dtor, {suspended});
    uint32_t store = call(g::StoreManager_getInstance, {});
    uint32_t copy = f.alloc(24);
    call(g::FunctionStore_copy, {copy, callback});
    uint32_t fo = heapFunction(f, 0);
    st32(fo + 16, heapFunc(VT_StoreProducts, 0x20, 8, copy));
    uint32_t sent = call(g::StoreManager_sendProductsRequest, {store, set, fo});
    functionDestroy(fo);
    functionDestroy(copy);
    if (!sent) call(g::FunctionStore_call, {callback, 0, 0});
    call(g::StringSet_dtor, {set});
}
RW_FUNCTION(0x5897A4, "BuyMagicStoneDialog::callSendProductsRequest(const vector<string>&, const function&)",
            BuyMagicStoneDialog_callSendProductsRequest);

// a purchase left unfinished: tell the player (popup) and return 1; the popup's OK resumes through callback
static uint32_t BuyMagicStoneDialog_checkTransaction(Addr callback) {
    uint32_t pm = call(g::PurchaseManager_getInstance, {});
    if (ld8(pm) || call(g::PurchaseManager_hasSuspendedTransaction, {pm}) != 1) return 0;
    Frame f;
    uint32_t copy = f.alloc(24);
    call(fn::function_vb_copy, {copy, callback});
    uint32_t popup = call(g::PopupDialog_create, {0, 0});
    uint32_t layer = call(g::SceneManager_getPopupLayer3, {sceneManager()});
    uint32_t td = textData(f, TXT_SUSPENDED_TRANSACTION);
    uint32_t message = newStr(f, textDataStr(td));
    uint32_t lambda = f.alloc(24);
    functionCopy(lambda, copy);
    uint32_t fo = heapFunction(f, 0);
    st32(fo + 16, heapFunc(VT_SuspendedPopup, 0x20, 8, lambda));
    call(g::PopupDialog_open, {popup, layer, message, fo});
    functionDestroy(fo);
    functionDestroy(lambda);
    strFree(message);
    sharedRelease(td);
    call(fn::AudioPlaySystemSEPopUp, {});
    functionDestroy(copy);
    return 1;
}
RW_FUNCTION(0x58A2B0, "BuyMagicStoneDialog::checkTransaction(const function<void(bool)>&)",
            BuyMagicStoneDialog_checkTransaction);

// the billing labels of a minor's account (billed this month, monthly limit); hidden for adults
static void BuyMagicStoneDialog_updatePaymentInfo(Addr node) {
    uint32_t a = api();
    uint32_t ctrl = ld32(a + API_PAYMENT_INFO + 4);
    uint32_t info = ld32(a + API_PAYMENT_INFO);
    if (ctrl) call(fn::shared_add, {ctrl});
    uint32_t billing = findNode(node, S_BillingLabel, 17);
    uint32_t limit = findNode(node, S_LimitLabel, 15);
    if (safe32(info, 0) <= MINOR_AGE) {
        {
            Frame f;
            uint32_t td = textData(f, TXT_BILLING);
            setTextC(billing, stringBuffer(f, {textDataStr(td), uint32_t(safe32(info, 4))}));
            sharedRelease(td);
        }
        {
            Frame f;
            uint32_t td = textData(f, TXT_LIMIT);
            int32_t billed = safe32(info, 4);
            int32_t rest = safe32(info, 8);
            setTextC(limit, stringBuffer(f, {textDataStr(td), uint32_t(rest + billed)}));
            sharedRelease(td);
        }
        SceneUtil::setShadow(billing);
        SceneUtil::setShadow(limit);
    } else {
        vcall(billing, node::setVisible, {billing, 0});
        vcall(limit, node::setVisible, {limit, 0});
    }
    if (ctrl) call(fn::shared_release, {ctrl});
}
RW_FUNCTION(0x58A4E8, "BuyMagicStoneDialog::updatePaymentInfo(Node*)", BuyMagicStoneDialog_updatePaymentInfo);

// A store product reported as "owned." (bought but not consumed): find its pack and open the popup that
// delivers it; returns 1 when the popup was opened
static uint32_t BuyMagicStoneDialog_hasOwnedItem(Addr products) {
    Frame f;
    uint32_t productId = f.alloc(12);
    st32(productId, 0);
    st32(productId + 4, 0);
    st32(productId + 8, 0);
    uint32_t result = 0;
    uint32_t node = products ? ld32(products + 8) : 0;
    for (; node; node = ld32(node))
        if (!call(g::String_compareC, {node + 0x20, S_Owned})) break;
    if (node) {
        uint32_t key = strData(node + 8);
        call(fn::string_assign, {productId, key, host_strlen(key)});
        uint32_t ids = f.alloc(12);
        call(g::Shop_getShopIds, {ids});
        int32_t jewels = 0, tickets = 0;
        for (uint32_t it = ld32(ids); it != ids + 4; it = treeNext(it)) {
            Frame t;
            uint32_t shop = masterRow(t, g::Shop_create, ld32(it + 0x10));
            uint32_t row = ld32(shop);
            bool found = false;
            if (safe32(row, master::Shop::STATUS) == SHOP_STATUS_ON_SALE &&
                safe32(row, master::Shop::OS) == SHOP_OS_ANDROID) {
                uint32_t s = newStr(t, safeBytes(t, row, master::Shop::PRODUCT_ID, 0x81));
                uint32_t n = strSize(s);
                if (n == strSize(productId) &&
                    !std::memcmp(rw::host_ptr<const char>(strData(s)), rw::host_ptr<const char>(strData(productId)), n)) {
                    int32_t bonus = safe32(row, master::Shop::BONUS);
                    int32_t number = safe32(row, master::Shop::NUMBER);
                    tickets = int32_t(call(g::GetSkipTicketCnt, {ld32(it + 0x10)}));
                    jewels = number + bonus;
                    found = true;
                }
                strFree(s);
            }
            sharedRelease(shop);
            if (found) break;
        }
        call(g::IntTree_destroy, {ids, ld32(ids + 4)});
        // the lambda {productId, jewels, tickets}, copied once more into the popup's callback
        uint32_t lambda = f.alloc(20);
        if (strIsLong(productId)) {
            strInit(lambda, ld32(productId + 8), ld32(productId + 4));
        } else {
            for (uint32_t k = 0; k < 12; k += 4) st32(lambda + k, ld32(productId + k));
        }
        st32(lambda + 12, uint32_t(jewels));
        st32(lambda + 16, uint32_t(tickets));
        uint32_t popup = call(g::PopupDialog_create, {0, 0});
        uint32_t layer = call(g::SceneManager_getPopupLayer3, {sceneManager()});
        uint32_t td = textData(f, TXT_OWNED_ITEM);
        uint32_t message = newStr(f, textDataStr(td));
        uint32_t copy = f.alloc(20);
        if (strIsLong(lambda)) {
            strInit(copy, ld32(lambda + 8), ld32(lambda + 4));
        } else {
            for (uint32_t k = 0; k < 12; k += 4) st32(copy + k, ld32(lambda + k));
        }
        st32(copy + 12, ld32(lambda + 12));
        st32(copy + 16, ld32(lambda + 16));
        uint32_t heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_OwnedPopup);
            for (uint32_t k = 0; k < 12; k += 4) st32(heap + 4 + k, ld32(copy + k));
            for (uint32_t k = 0; k < 12; k += 4) st32(copy + k, 0);
            st32(heap + 0x14, ld32(copy + 16));
            st32(heap + 0x10, ld32(copy + 12));
        }
        uint32_t fo = heapFunction(f, heap);
        call(g::PopupDialog_open, {popup, layer, message, fo});
        result = 1;
        functionDestroy(fo);
        strFree(copy);
        strFree(message);
        sharedRelease(td);
        strFree(lambda);
    }
    strFree(productId);
    return result;
}
RW_FUNCTION(0x58AD80, "BuyMagicStoneDialog::hasOwnedItem(const unordered_map<string, ProductInfo>*)",
            BuyMagicStoneDialog_hasOwnedItem);

// holding the maximum of Jewels: say so (OK dialog)
static void BuyMagicStoneDialog_checkHasMaxMagicStone() {
    int32_t limit;
    {
        Frame f;
        uint32_t misc = masterRow(f, fn::Misc_create, MISC_PURCHASE_LIMIT);
        limit = safe32(ld32(misc), 4);
        sharedRelease(misc);
    }
    if (!SceneUtil::hasMagicStone(limit)) return;
    Frame f;
    uint32_t td = textData(f, TXT_MAX_MAGIC_STONE);
    uint32_t message = newStr(f, stringBuffer(f, {textDataStr(td), uint32_t(limit)}));
    uint32_t fo = newEmptyFunction(f);
    call(g::SceneUtil_openOkDialog4Line, {message, fo, 0});
    functionDestroy(fo);
    strFree(message);
    sharedRelease(td);
}
RW_FUNCTION(0x58B1D0, "BuyMagicStoneDialog::checkHasMaxMagicStone()", BuyMagicStoneDialog_checkHasMaxMagicStone);

// ------------------------------------------------------------------------------------------------ buttons
namespace {
// label text, the button's touch lambda, and the button hidden (and disabled) unless the server's function flag
// (bit `mask` of functionFlags byte `flagByte`) is set
void initInfoButton(uint32_t root, uint32_t txt, uint32_t button, int32_t textId, uint32_t vtable,
                    uint32_t flagByte, uint32_t mask) {
    uint32_t label = findNode(root, txt, 13);
    setTextId(label, textId);
    SceneUtil::setShadow(label);
    uint32_t b = findNode(root, button, 16);
    vcall(b, 0x29C, {b, 1});   // setTouchEnabled
    addTouch(b, vtable, {});
    if (!(retFlags(flagByte) & mask)) {
        vcall(b, node::setVisible, {b, 0});
        vcall(b, 0x29C, {b, 0});
    }
}
}  // namespace

static void BuyMagicStoneDialog_initJewelInfoButton(Addr root) {
    initInfoButton(root, S_TxtCrownBuy1, S_ButtonCrownBuy1, TXT_JEWEL_INFO, VT_JewelInfo, 1, 0x04);
}
RW_FUNCTION(0x58B310, "BuyMagicStoneDialog::initJewelInfoButton(Widget*)", BuyMagicStoneDialog_initJewelInfoButton);

static void BuyMagicStoneDialog_initFundSettlementButton(Addr root) {
    initInfoButton(root, S_TxtCrownBuy2, S_ButtonCrownBuy2, TXT_FUND_SETTLEMENT, VT_FundSettlement, 0, 0x80);
}
RW_FUNCTION(0x58B504, "BuyMagicStoneDialog::initFundSettlementButton(Widget*)",
            BuyMagicStoneDialog_initFundSettlementButton);

static void BuyMagicStoneDialog_initTokushohoButton(Addr root) {
    initInfoButton(root, S_TxtCrownBuy3, S_ButtonCrownBuy3, TXT_TOKUSHOHO, VT_Tokushoho, 1, 0x01);
}
RW_FUNCTION(0x58B6F8, "BuyMagicStoneDialog::initTokushohoButton(Widget*)", BuyMagicStoneDialog_initTokushohoButton);

// ------------------------------------------------------------------------------------------------ the pack sort
namespace {
bool less(uint32_t a, uint32_t b) { return call(g::SortComp, {a, b}) != 0; }
void swap4(uint32_t x, uint32_t y) {
    uint32_t t = ld32(x);
    st32(x, ld32(y));
    st32(y, t);
}
uint32_t sort3(uint32_t x, uint32_t y, uint32_t z) { return call(g::Sort3, {x, y, z}); }
}  // namespace

// a before b: by master::Shop sortId
static uint32_t BuyMagicStoneDialog_sortComp(Addr a, Addr b) {
    int32_t sa, sb;
    {
        Frame f;
        uint32_t shop = masterRow(f, g::Shop_create, ld32(a));
        sa = safe32(ld32(shop), master::Shop::SORT_ID);
        sharedRelease(shop);
    }
    {
        Frame f;
        uint32_t shop = masterRow(f, g::Shop_create, ld32(b));
        sb = safe32(ld32(shop), master::Shop::SORT_ID);
        sharedRelease(shop);
    }
    return sa < sb;
}
RW_FUNCTION(0x58C708, "BuyMagicStoneDialog::initItemIDsAndProductIDs sort comparator", BuyMagicStoneDialog_sortComp);

// std::__sort3: returns the swaps made
static uint32_t BuyMagicStoneDialog_sort3(Addr x, Addr y, Addr z) {
    const bool yx = less(y, x);
    const bool zy = less(z, y);
    if (!yx) {
        if (!zy) return 0;
        swap4(y, z);
        if (!less(y, x)) return 1;
        swap4(x, y);
        return 2;
    }
    if (zy) {
        swap4(x, z);
        return 1;
    }
    swap4(x, y);
    if (!less(z, y)) return 1;
    swap4(y, z);
    return 2;
}
RW_FUNCTION(0x58C7C0, "std::__sort3<BuyMagicStoneDialog sort comparator&, int*>", BuyMagicStoneDialog_sort3);

namespace {
bool smallSort(uint32_t first, uint32_t last) {
    switch (uint32_t(int32_t(last - first) >> 2)) {
    case 0:
    case 1:
        return true;
    case 2:
        if (less(last - 4, first)) swap4(first, last - 4);
        return true;
    case 3:
        sort3(first, first + 4, last - 4);
        return true;
    case 4:
        call(g::Sort4, {first, first + 4, first + 8, last - 4});
        return true;
    case 5:
        call(g::Sort5, {first, first + 4, first + 8, first + 12, last - 4});
        return true;
    default:
        return false;
    }
}
void insertOne(uint32_t first, uint32_t i, uint32_t j) {
    if (!less(i, j)) return;
    Frame f;
    uint32_t t = f.alloc(4);
    st32(t, ld32(i));
    uint32_t hole = i, k = j;
    while (true) {
        st32(hole, ld32(k));
        hole = k;
        if (hole == first) break;
        if (!less(t, k - 4)) break;
        k -= 4;
    }
    st32(hole, ld32(t));
}
}  // namespace

// std::__sort (libc++ introsort without the depth limit)
static void BuyMagicStoneDialog_sort(Addr first, Addr last) {
    while (true) {
    restart:
        const int32_t bytes = int32_t(last - first);
        const int32_t len = bytes >> 2;
        if (uint32_t(len) <= 5) {
            smallSort(first, last);
            return;
        }
        if (bytes <= 123) {
            // __insertion_sort_3
            uint32_t j = first + 8;
            sort3(first, first + 4, j);
            for (uint32_t i = j + 4; i != last; i += 4) {
                insertOne(first, i, j);
                j = i;
            }
            return;
        }
        const int32_t half = len / 2;
        uint32_t m = first + uint32_t(half) * 4;
        const uint32_t lm1 = last - 4;
        uint32_t swaps;
        if (bytes >= 3997) {
            const int32_t delta = len / 4;
            swaps = call(g::Sort5, {first, first + uint32_t(delta) * 4, m, first + uint32_t(delta + half) * 4, lm1});
        } else {
            swaps = sort3(first, m, lm1);
        }
        uint32_t i = first, j = lm1;
        if (!less(i, m)) {
            while (true) {
                j -= 4;
                if (i == j) {
                    i += 4;
                    j = last;
                    j -= 4;
                    if (!less(first, j)) {
                        while (true) {
                            if (i == j) return;
                            if (less(first, i)) {
                                swap4(i, j);
                                i += 4;
                                break;
                            }
                            i += 4;
                        }
                    }
                    if (i == j) return;
                    while (true) {
                        while (!less(first, i)) i += 4;
                        do {
                            j -= 4;
                        } while (less(first, j));
                        if (!(i < j)) break;
                        swap4(i, j);
                        i += 4;
                    }
                    first = i;
                    goto restart;
                }
                if (less(j, m)) {
                    swap4(i, j);
                    swaps++;
                    break;
                }
            }
        }
        i += 4;
        if (i < j) {
            while (true) {
                while (less(i, m)) i += 4;
                do {
                    j -= 4;
                } while (!less(j, m));
                if (i > j) break;
                swap4(i, j);
                swaps++;
                if (m == i) m = j;
                i += 4;
            }
        }
        if (i != m && less(m, i)) {
            swap4(i, m);
            swaps++;
        }
        if (swaps == 0) {
            const bool fs = call(g::InsertionSortIncomplete, {first, i}) != 0;
            if (call(g::InsertionSortIncomplete, {i + 4, last}) == 1) {
                if (fs) return;
                last = i;
                continue;
            }
            if (fs) {
                first = i + 4;
                continue;
            }
        }
        if (int32_t(i - first) < int32_t(last - i)) {
            call(g::SortIds, {first, i});
            first = i + 4;
        } else {
            call(g::SortIds, {i + 4, last});
            last = i;
        }
    }
}
RW_FUNCTION(0x58C398, "std::__sort<BuyMagicStoneDialog sort comparator&, int*>", BuyMagicStoneDialog_sort);

// ------------------------------------------------------------------------------------------------ lambdas
// callGetPaymentInfo $_3 (functor: vtable, function<void(bool)> at +0x10): the payment info arrived -> the
// shop state; on failure the loading indicator goes and the error dialog opens
static void BuyMagicStoneDialog_onPaymentInfo(Addr functor, Addr ok) {
    if (!ld8(ok)) {
        loadingIndicator(0);
        openErrorDialog();
        return;
    }
    uint32_t a = api();
    Frame f;
    uint32_t copy = f.alloc(24);
    call(fn::function_vb_copy, {copy, functor + 0x10});
    uint32_t fo = heapFunction(f, 0);
    st32(fo + 16, heapFunc(VT_ShopGet, 0x28, 0x10, copy));
    call(g::APIManager_executeUserShopGet, {a, fo});
    functionDestroy(fo);
    functionDestroy(copy);
}
RW_FUNCTION(0x58DA18, "BuyMagicStoneDialog::callGetPaymentInfo $_3::operator()(bool)", BuyMagicStoneDialog_onPaymentInfo);

// $_3's inner lambda: the shop state arrived -> callback(true)
static void BuyMagicStoneDialog_onShopGet(Addr functor, Addr ok) {
    if (ld8(ok)) {
        call(g::FunctionVB_call, {functor + 0x10, 1});
        return;
    }
    loadingIndicator(0);
    openErrorDialog();
}
RW_FUNCTION(0x58DD40, "BuyMagicStoneDialog::callGetPaymentInfo $_3 inner lambda::operator()(bool)",
            BuyMagicStoneDialog_onShopGet);

// init $_2 __func::__clone()
static Addr BuyMagicStoneDialog_initLambdaClone(Addr functor) {
    uint32_t heap = call(fn::operator_new, {0x40});
    if (heap) {
        st32(heap, VT_Init);
        call(g::VectorString_copy, {heap + 8, functor + 8});
        call(g::FunctionProducts_copy, {heap + 0x18, functor + 0x18});
        call(g::VectorInt_copy, {heap + 0x30, functor + 0x30});
    }
    return heap;
}
RW_FUNCTION(0x58DF70, "BuyMagicStoneDialog::init $_2 __func::__clone()", BuyMagicStoneDialog_initLambdaClone);

// init $_2 {productIds +8, callback +0x18, itemIds +0x30}: payment info and shop state are in -> ask the store;
// on failure callback(itemIds, nullptr, false)
static void BuyMagicStoneDialog_onInitReady(Addr functor, Addr ok) {
    if (!ld8(ok)) {
        loadingIndicator(0);
        call(g::FunctionProducts_call, {functor + 0x18, functor + 0x30, 0, 0});
        return;
    }
    Frame f;
    uint32_t lambda = f.alloc(0x24);
    call(g::FunctionProducts_copy, {lambda, functor + 0x18});
    call(g::VectorInt_copy, {lambda + 0x18, functor + 0x30});
    uint32_t heap = call(fn::operator_new, {0x30});
    if (heap) {
        st32(heap, VT_InitProducts);
        functionCopy(heap + 8, lambda);
        moveVector(heap + 0x20, lambda + 0x18);
    }
    uint32_t fo = heapFunction(f, heap);
    call(0x5897A5, {functor + 8, fo});   // callSendProductsRequest
    functionDestroy(fo);
    call(g::VectorBaseInt_dtor, {lambda + 0x18});
    functionDestroy(lambda);
}
RW_FUNCTION(0x58E124, "BuyMagicStoneDialog::init $_2::operator()(bool)", BuyMagicStoneDialog_onInitReady);

// ------------------------------------------------------------------------------------------------ the Jewel shop popup
// open $_0 {onClose +8, flag +0x20}: init's result (the pack ids, the store's products, ok). On failure an OK dialog
// (then onClose). Otherwise the purchase manager takes the products; a suspended purchase or an owned item stops
// here (checkTransaction's / hasOwnedItem's own dialogs). Then ShopBuyItem_Crown_ver120 opens on popup layer 2 over
// a fading shadow: with flag set only its close button; without, the title, the MagicStoneScrollView of the packs
// (in Scroll_Area when an info button is enabled, else in Scroll_Area2 with Win_Over 542 high), the billing info and
// the three info buttons. Closing calls the close lambda, fades out and removes the popup, then onClose.
namespace {
namespace ms {
constexpr uint32_t PurchaseManager_setProductInfoMap = 0x4D86ED;   // (mgr, const unordered_map<string, ProductInfo>*)
constexpr uint32_t PurchaseManager_startPurchase = 0x4D831D;       // (mgr, const char* productId, function<void(bool)>)
constexpr uint32_t PurchaseCallback = 0x58A7A1;      // purchaseCallback(ok, jewels, view, popup, tickets, function, function)
constexpr uint32_t CheckTransaction = 0x58A2B1;
constexpr uint32_t HasOwnedItem = 0x58AD81;
constexpr uint32_t UpdatePaymentInfo = 0x58A4E9, InitJewelInfoButton = 0x58B311, InitFundSettlementButton = 0x58B505,
                   InitTokushohoButton = 0x58B6F9;
constexpr uint32_t ScrollView_create = 0x4FA569;     // MagicStoneScrollView::create()
constexpr uint32_t ScrollView_setProductInfoMap = 0x4FB935;
constexpr uint32_t ScrollView_setItemIDs = 0x478929; // (view, const vector<int>&)
constexpr uint32_t FunctionVV_swap = 0x4EE86D;       // std::function<void()>::swap
constexpr uint32_t FunctionInt_swap = 0x5428E1;      // std::function<void(int)>::swap
constexpr uint32_t Widget_getCustomSize = 0x8262D1;
constexpr uint32_t Size_copy = 0x8DF02D;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;    // (node, const function<void()>&)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;   // (node, const function<void()>&)
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;           // (node, const string&, int textId)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
}  // namespace ms

constexpr uint32_t VT_OpenError = 0xF5AC48;          // the error dialog's callback {function<void()> onClose +8} (0x20)
constexpr uint32_t VT_OpenSuspended = 0xF5AC1C;      // checkTransaction's callback {function<void()> +8} (0x20)
constexpr uint32_t VT_CloseOnly = 0xF5ABF0;          // the close lambda (flag set) {closeButton}
constexpr uint32_t VT_CloseList = 0xF5ABC4;          // the close lambda (the pack list) {closeButton}
constexpr uint32_t VT_CloseTouch = 0xF5AB98;         // Button_Close's touch listener (no captures)
constexpr uint32_t VT_ListSelect = 0xF5AB14;         // the scroll view's pack chosen {view, popup}
constexpr uint32_t VT_Purchase = 0xF5AB40;           // startPurchase's callback {jewels, view, popup, tickets} (0x14)
constexpr uint32_t VT_ListSuspended = 0xF5AB6C;      // the chosen pack's checkTransaction callback (no captures)
constexpr uint32_t VT_OpenAnim = 0xF5AAE8;           // the open animation's end (no captures)
constexpr uint32_t VT_CanClose = 0xF5AABC;           // openPopup's function<bool()> {closeButton}
constexpr uint32_t VT_Closed = 0xF5AA64;             // openPopup's result {close +8, shadow +0x20, popup +0x24, onClose +0x28, layer +0x40} (0x48)
constexpr uint32_t VT_Removed = 0xF5AA90;            // the close animation's end {onClose +8, layer +0x20, shadow +0x24, popup +0x28} (0x30)
constexpr uint32_t S_ShopJson = 0xDFFFEE;            // "cocostudio/publish/ShopBuyItem_Crown_ver120.json"
constexpr uint32_t S_ButtonClose = 0xDF8692, S_TxtTitleLabel = 0xDF9554, S_ScrollArea = 0xDF9B59,
                   S_ScrollArea2 = 0xE0001F;
constexpr char WIN_OVER[] = "Win_Over";              // built inline as a short string
constexpr int32_t TXT_SHOP_ERROR = 105100001, TXT_SHOP_TITLE = 102600002;
constexpr uint32_t G_SHADOW_FADE = 0xFD65C0;         // GOT: the popup shadow's fade time (float)
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C, V_SET_SIZE = 0x2B0;   // ui::Widget
constexpr uint32_t VIEW_ON_SELECT = 0x650;           // MagicStoneScrollView: function<void(int)>, a pack was chosen
constexpr float WIN_OVER_HEIGHT = 542.0f;

float shadowFade() { return ldf(ld32(G_SHADOW_FADE)); }
// a std::function at fo holding an inline lambda (vtable, captures)
void inlineFunction(uint32_t fo, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    st32(fo + 16, fo);
    st32(fo, vtable);
    uint32_t k = 4;
    for (uint32_t c : captures) { st32(fo + k, c); k += 4; }
}
// a widget by name (the name built in the frame slot s)
uint32_t seekNamed(uint32_t root, uint32_t s) {
    uint32_t node = SceneUtil::searchNodeByName(root, s);
    strFree(s);
    return node;
}
uint32_t seek(uint32_t root, uint32_t s, uint32_t name) {
    strInitC(s, name);
    return seekNamed(root, s);
}
// a short std::string written in place (libc++ inlines short literals)
void shortStr(uint32_t s, const char *text) {
    const uint32_t n = uint32_t(std::strlen(text));
    st8(s, uint8_t(n << 1));
    for (uint32_t k = 0; k < n; k++) st8(s + 1 + k, uint8_t(text[k]));
    st8(s + 1 + n, 0);
}
}  // namespace

static void BuyMagicStoneDialog_openResult(Addr functor, Addr ids, Addr mapRef, Addr okRef) {
    Frame frame;
    const uint32_t base = frame.allocExact(0x278);    // push 0x14 + 0x264: the temporaries at their original offsets
    const uint32_t IDS = base + 0x50, FN3 = base + 0x1B0, TMP = base + 0x240, COPY1 = base + 0x1F8;
    const uint32_t onClose = functor + 8;
    call(g::VectorInt_copy, {IDS, ids});
    const uint8_t ok = ld8(okRef);
    const uint32_t map = ld32(mapRef);
    loadingIndicator(0);
    if (!map || !ok || !ld32(map + 0xC)) {
        const uint32_t text = base + 0xBC, str = base + 0xC4, copy = base + 0x210, fo = base + 0x228;
        call(fn::TextData_get, {text, 0, uint32_t(TXT_SHOP_ERROR)});
        strInitC(str, textDataStr(text));
        call(fn::function_vv_copy, {copy, onClose});
        st32(fo + 16, 0);
        st32(fo + 16, heapFunc(VT_OpenError, 0x20, 8, copy));
        call(g::SceneUtil_openOkDialog4Line, {str, fo, 0});
        functionDestroy(fo);
        functionDestroy(copy);
        strFree(str);
        sharedRelease(text);
        call(g::VectorBaseInt_dtor, {IDS});
        return;
    }
    call(ms::PurchaseManager_setProductInfoMap, {call(g::PurchaseManager_getInstance, {}), map});
    call(fn::function_vv_copy, {COPY1, onClose});
    {
        const uint32_t L0 = base + 0x1C8, fo = base + 0x1E0;
        functionCopy(L0, COPY1);
        st32(fo + 16, 0);
        st32(fo + 16, heapFunc(VT_OpenSuspended, 0x20, 8, L0));
        const uint32_t suspended = call(ms::CheckTransaction, {fo});
        functionDestroy(fo);
        functionDestroy(L0);
        if (suspended) {
            functionDestroy(COPY1);
            call(g::VectorBaseInt_dtor, {IDS});
            return;
        }
    }
    if (call(ms::HasOwnedItem, {map}) == 1) {
        if (ld32(COPY1 + 16)) {
            call(fn::function_vv_call, {COPY1});
            functionDestroy(COPY1);
        }
        call(g::VectorBaseInt_dtor, {IDS});
        return;
    }

    // the popup over a fading shadow
    const uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = SceneUtil::createShadowSprite();
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(shadowFade()), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    st32(FN3 + 16, 0);
    const uint8_t flag = ld8(functor + 0x20);
    const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_ShopJson});
    vcall(shadow, node::addChild, {shadow, popup});
    {
        const uint32_t v = base + (flag ? 0xB4 : 0xA0);
        call(ms::Vec2_ctor, {v, fbits(0.5f), fbits(0.5f)});
        vcall(popup, node::setAnchorPoint, {popup, v});
        call(ms::Vec2_dtor, {v});
    }
    vcall(popup, node::setPosition, {popup, vcall(shadow, node::getPosition, {shadow})});
    uint32_t closeButton, view = 0;
    if (flag) {
        closeButton = seek(popup, base + 0xA8, S_ButtonClose);
        inlineFunction(TMP, VT_CloseOnly, {closeButton});
        call(ms::FunctionVV_swap, {TMP, FN3});
        functionDestroy(TMP);
    } else {
        {
            const uint32_t s = base + 0x94;
            strInitC(s, S_TxtTitleLabel);
            call(ms::SceneUtil_settingTitleLabel, {popup, s, uint32_t(TXT_SHOP_TITLE)});
            strFree(s);
        }
        closeButton = seek(popup, base + 0x88, S_ButtonClose);
        inlineFunction(TMP, VT_CloseList, {closeButton});
        call(ms::FunctionVV_swap, {TMP, FN3});
        functionDestroy(TMP);
        const uint32_t area = seek(popup, base + 0x7C, S_ScrollArea);
        const uint32_t area2 = seek(popup, base + 0x70, S_ScrollArea2);
        shortStr(base + 0x64, WIN_OVER);
        const uint32_t winOver = seekNamed(popup, base + 0x64);
        view = call(ms::ScrollView_create, {});
        call(ms::ScrollView_setProductInfoMap, {view, map});
        const bool infoButtons = (retFlags(1) & 0x04) || (retFlags(0) & 0x80) || (retFlags(1) & 0x01);
        const uint32_t into = infoButtons ? area : area2;
        vcall(view, node::setContentSize, {view, vcall(into, node::getContentSize, {into})});
        call(ms::ScrollView_setItemIDs, {view, IDS});
        vcall(into, node::addChild, {into, view});
        if (infoButtons) {
            vcall(area, node::setVisible, {area, 1});
            vcall(area2, node::setVisible, {area2, 0});
        } else {
            vcall(area, node::setVisible, {area, 0});
            vcall(area2, node::setVisible, {area2, 1});
            const uint32_t size = base + 0x5C;
            call(ms::Size_copy, {size, call(ms::Widget_getCustomSize, {winOver})});
            stf(size + 4, WIN_OVER_HEIGHT);
            vcall(winOver, V_SET_SIZE, {winOver, size});
        }
        call(ms::UpdatePaymentInfo, {popup});
        call(ms::InitJewelInfoButton, {popup});
        call(ms::InitFundSettlementButton, {popup});
        call(ms::InitTokushohoButton, {popup});
    }

    // L {close (a copy of FN3) +0, shadow +0x18, popup +0x1C, onClose +0x20, layer +0x38}
    const uint32_t L = base + 0x170;
    functionCopy(L, FN3);
    st32(L + 0x18, shadow);
    st32(L + 0x1C, popup);
    call(fn::function_vv_copy, {L + 0x20, onClose});
    st32(L + 0x38, layer);
    vcall(closeButton, V_SET_TOUCH_ENABLED, {closeButton, 1});
    {
        const uint32_t fo = base + 0x158;
        inlineFunction(fo, VT_CloseTouch, {});
        call(fn::Widget_addTouchEventListener, {closeButton, fo});
        functionDestroy(fo);
    }
    if (!flag) {
        inlineFunction(TMP, VT_ListSelect, {view, popup});
        call(ms::FunctionInt_swap, {TMP, view + VIEW_ON_SELECT});
        functionDestroy(TMP);
    }
    call(fn::AudioPlaySystemSEPopUp, {});
    {
        const uint32_t fo = base + 0x140;
        inlineFunction(fo, VT_OpenAnim, {});
        call(fn::Node_runAction, {popup, call(ms::SceneUtil_createOpenPopupAnimation, {popup, fo})});
        functionDestroy(fo);
    }
    const uint32_t sm = sceneManager();
    const uint32_t canClose = base + 0x128, closed = base + 0x110, C = base + 0xD0;
    inlineFunction(canClose, VT_CanClose, {closeButton});
    functionCopy(C, L);
    st32(C + 0x18, shadow);
    st32(C + 0x1C, popup);
    functionCopy(C + 0x20, L + 0x20);
    st32(C + 0x38, layer);
    st32(closed + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x48});
    if (heap) {
        st32(heap, VT_Closed);
        call(fn::function_vv_move, {heap + 8, C});
        st32(heap + 0x20, ld32(C + 0x18));
        st32(heap + 0x24, ld32(C + 0x1C));
        functionCopy(heap + 0x28, C + 0x20);
        st32(heap + 0x40, ld32(C + 0x38));
    }
    st32(closed + 16, heap);
    call(fn::SceneManager_openPopup, {sm, canClose, closed, 1});
    functionDestroy(closed);
    functionDestroy(C + 0x20);
    functionDestroy(C);
    functionDestroy(canClose);
    functionDestroy(L + 0x20);
    functionDestroy(L);
    functionDestroy(FN3);
    functionDestroy(COPY1);
    call(g::VectorBaseInt_dtor, {IDS});
}
RW_FUNCTION(0x590B9C, "BuyMagicStoneDialog::open $_0::operator()(vector<int>, const unordered_map*, bool)",
            BuyMagicStoneDialog_openResult);

// open $_0's closed lambda __clone()
static Addr BuyMagicStoneDialog_closedClone(Addr functor) {
    const uint32_t heap = call(fn::operator_new, {0x48});
    if (heap) {
        st32(heap, VT_Closed);
        call(fn::function_vv_copy, {heap + 8, functor + 8});
        st32(heap + 0x20, ld32(functor + 0x20));
        st32(heap + 0x24, ld32(functor + 0x24));
        call(fn::function_vv_copy, {heap + 0x28, functor + 0x28});
        st32(heap + 0x40, ld32(functor + 0x40));
    }
    return heap;
}
RW_FUNCTION(0x5919D0, "BuyMagicStoneDialog::open $_0 closed lambda __clone()", BuyMagicStoneDialog_closedClone);

// the popup closed: the close lambda, the shadow fades out, the popup plays its close animation and is then removed
static void BuyMagicStoneDialog_closed(Addr functor, Addr) {
    call(ms::AudioPlaySystemSECancel, {});
    call(fn::function_vv_call, {functor + 8});
    const uint32_t shadow = ld32(functor + 0x20);
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(shadowFade()), 0})});
    const uint32_t popup = ld32(functor + 0x24);
    Frame f;
    const uint32_t L = f.alloc(0x24);
    call(fn::function_vv_copy, {L, functor + 0x28});
    st32(L + 0x18, ld32(functor + 0x40));
    st32(L + 0x1C, ld32(functor + 0x20));
    st32(L + 0x20, ld32(functor + 0x24));
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x30});
    if (heap) {
        st32(heap, VT_Removed);
        functionCopy(heap + 8, L);
        st32(heap + 0x20, ld32(L + 0x18));
        st32(heap + 0x24, ld32(L + 0x1C));
        st32(heap + 0x28, ld32(L + 0x20));
    }
    st32(fo + 16, heap);
    const uint32_t anim = call(ms::SceneUtil_createClosePopupAnimation, {popup, fo});
    call(fn::Node_runAction, {popup, call(ms::EaseSineOut_create, {anim})});
    functionDestroy(fo);
    functionDestroy(L);
}
RW_FUNCTION(0x591B70, "BuyMagicStoneDialog::open $_0 closed lambda::operator()(bool)", BuyMagicStoneDialog_closed);

// the close animation ended: onClose, then the shadow and the popup leave popup layer 2
static void BuyMagicStoneDialog_removed(Addr functor) {
    if (ld32(functor + 0x18)) call(fn::function_vv_call, {functor + 8});
    const uint32_t layer = ld32(functor + 0x20);
    vcall(layer, node::removeChild, {layer, ld32(functor + 0x24), 1});
    vcall(layer, node::removeChild, {layer, ld32(functor + 0x28), 1});
}
RW_FUNCTION(0x591EA8, "BuyMagicStoneDialog::open $_0 removed lambda::operator()()", BuyMagicStoneDialog_removed);

// a pack chosen: unless a purchase runs or a suspended one is pending, the loading indicator and the store purchase
// of the pack's product id (the callback gets number + bonus Jewels and the pack's stage skip tickets)
static void BuyMagicStoneDialog_packSelected(Addr functor, Addr idRef) {
    Frame frame;
    const uint32_t base = frame.allocExact(0x108);    // push 0x14 + 0xF4
    const uint32_t row = base + 0x20, productId = base + 0x4C, fo = base + 0x30, check = base + 0xD0;
    const uint32_t id = ld32(idRef);
    call(ms::AudioPlaySystemSEDecide, {});
    call(g::Shop_create, {row, id});
    const uint32_t jewels = uint32_t(safe32(ld32(row), master::Shop::BONUS) + safe32(ld32(row), master::Shop::NUMBER));
    const uint32_t tickets = call(g::GetSkipTicketCnt, {id});
    const uint32_t mgr = call(g::PurchaseManager_getInstance, {});
    if (!ld8(mgr)) {
        inlineFunction(check, VT_ListSuspended, {});
        const uint32_t suspended = call(ms::CheckTransaction, {check});
        functionDestroy(check);
        if (!suspended) {
            loadingIndicator(1);
            safeBytesAt(productId, ld32(row), master::Shop::PRODUCT_ID, 0x81);
            st32(fo + 16, 0);
            const uint32_t heap = call(fn::operator_new, {0x14});
            if (heap) {
                st32(heap, VT_Purchase);
                st32(heap + 4, jewels);
                st32(heap + 8, ld32(functor + 4));
                st32(heap + 0xC, ld32(functor + 8));
                st32(heap + 0x10, tickets);
            }
            st32(fo + 16, heap);
            call(ms::PurchaseManager_startPurchase, {mgr, productId, fo});
            functionDestroy(fo);
        }
    }
    sharedRelease(row);
}
RW_FUNCTION(0x5920AC, "BuyMagicStoneDialog::open $_0 pack chosen lambda::operator()(int)",
            BuyMagicStoneDialog_packSelected);

// the store purchase ended: purchaseCallback(ok, jewels, view, popup, tickets, {}, {})
static void BuyMagicStoneDialog_purchased(Addr functor, Addr okRef) {
    Frame frame;
    const uint32_t base = frame.allocExact(0x68);     // push 0x14 + 0x54: the stack arguments at sp
    const uint32_t a = base + 0x18, b = base + 0x30;
    st32(a + 16, 0);
    st32(b + 16, 0);
    rw::call_exact(ms::PurchaseCallback, {ld8(okRef), ld32(functor + 4), ld32(functor + 8), ld32(functor + 0xC),
                                          ld32(functor + 0x10), b, a});
    functionDestroy(a);
    functionDestroy(b);
}
RW_FUNCTION(0x5922FC, "BuyMagicStoneDialog::open $_0 purchase lambda::operator()(bool)", BuyMagicStoneDialog_purchased);

// init $_2's inner lambda {callback +8, itemIds +0x20}: the store's products -> callback(itemIds, products, ok)
static void BuyMagicStoneDialog_onProducts(Addr functor, Addr okRef, Addr mapRef) {
    const uint32_t map = ld32(mapRef);
    const uint8_t ok = ld8(okRef);
    loadingIndicator(0);
    call(g::FunctionProducts_call, {functor + 8, functor + 0x20, map, ok ? 1u : 0u});
}
RW_FUNCTION(0x58E584, "BuyMagicStoneDialog::init $_2 inner lambda::operator()(bool, const unordered_map*)",
            BuyMagicStoneDialog_onProducts);

static Addr BuyMagicStoneDialog_initProductsClone(Addr functor) {
    const uint32_t heap = call(fn::operator_new, {0x30});
    if (heap) {
        st32(heap, VT_InitProducts);
        call(g::FunctionProducts_copy, {heap + 8, functor + 8});
        call(g::VectorInt_copy, {heap + 0x20, functor + 0x20});
    }
    return heap;
}
RW_FUNCTION(0x58E42C, "BuyMagicStoneDialog::init $_2 inner lambda __clone()", BuyMagicStoneDialog_initProductsClone);

// ------------------------------------------------------------------------------------------------ the discount popup
// openDiscount $_1 {onClose +8, onPurchased +0x20}: init's result for the one discounted pack (Type 1). On failure
// onClose. Otherwise like open $_0 up to the popup: Purchase_Pop centred over a fading shadow, Button_Purchase (its
// label: TextData 102620006 with the store price, in helvetica 32 when the store knows the product) and
// Close_Button_Purchase; the web view of popup layer 2 is hidden while it is open.
namespace {
namespace dc {
constexpr uint32_t ProductMap_find = 0x5907B5;       // unordered_map<string, ProductInfo>::find(map, const string&)
constexpr uint32_t Text_setFontName = 0x83A499, Text_setFontSize = 0x83A35D;
constexpr uint32_t SceneUtil_settingTitleLabelText = 0x46E8D9;   // (node, const string& name, const string& text)
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t VectorInt_throwOutOfRange = 0x47CCE1;
constexpr uint32_t G_ANCHOR_MIDDLE = 0xFD6054;       // GOT: &Vec2::ANCHOR_MIDDLE
}  // namespace dc
constexpr uint32_t VT_DiscountProducts = 0xF5A828;   // openDiscount $_1 (0x38)
constexpr uint32_t VT_DiscountSuspended = 0xF5AA0C;  // checkTransaction's callback {function<void()> +8} (0x20)
constexpr uint32_t VT_DiscountPurchase = 0xF5A904;   // Button_Purchase {purchase, close, shop row, shopId, popup, webView, onPurchased} (0x40)
constexpr uint32_t VT_DiscountClose = 0xF5A8AC;      // Close_Button_Purchase {purchase, close, popup, webView, onClose} (0x30)
constexpr uint32_t VT_DiscountCanClose = 0xF5A880;   // openPopup's function<bool()> {purchase}
constexpr uint32_t VT_DiscountClosed = 0xF5A854;     // openPopup's result {layer, shadow}
constexpr uint32_t S_PurchaseJson = 0xDFFF8C;        // "cocostudio/publish/Purchase_Pop.json"
constexpr uint32_t S_ButtonPurchase = 0xDFFFB1, S_CloseButtonPurchase = 0xDFFFC1, S_TxtPurchase = 0xDFFFD7;
constexpr char HELVETICA[] = "helvetica", WEB_VIEW[] = "webView";
constexpr int32_t TXT_PURCHASE_PRICE = 102620006;
constexpr uint32_t PURCHASE_FONT_SIZE = 32;
constexpr uint32_t PRODUCT_PRICE = 0x20;             // the map node's ProductInfo price string
}  // namespace

static void BuyMagicStoneDialog_openDiscountResult(Addr functor, Addr ids, Addr mapRef, Addr okRef) {
    Frame frame;
    const uint32_t base = frame.allocExact(0x6A0);    // push 0x14 + 0x68C: the temporaries at their original offsets
    const uint32_t IDS = base + 0x54, COPY = base + 0x668, ROW = base + 0xB0;
    const uint32_t onClose = functor + 8;
    call(g::VectorInt_copy, {IDS, ids});
    const uint8_t ok = ld8(okRef);
    const uint32_t map = ld32(mapRef);
    loadingIndicator(0);
    if (!map || !ok || !ld32(map + 0xC)) {
        if (ld32(functor + 0x18)) call(fn::function_vv_call, {onClose});
        call(g::VectorBaseInt_dtor, {IDS});
        return;
    }
    call(ms::PurchaseManager_setProductInfoMap, {call(g::PurchaseManager_getInstance, {}), map});
    call(fn::function_vv_copy, {COPY, onClose});
    {
        const uint32_t L0 = base + 0x638, fo = base + 0x650;
        functionCopy(L0, COPY);
        st32(fo + 16, 0);
        st32(fo + 16, heapFunc(VT_DiscountSuspended, 0x20, 8, L0));
        const uint32_t suspended = call(ms::CheckTransaction, {fo});
        functionDestroy(fo);
        functionDestroy(L0);
        if (suspended) {
            functionDestroy(COPY);
            call(g::VectorBaseInt_dtor, {IDS});
            return;
        }
    }
    if (call(ms::HasOwnedItem, {map}) == 1) {
        if (ld32(COPY + 16)) {
            call(fn::function_vv_call, {COPY});
            functionDestroy(COPY);
        }
        call(g::VectorBaseInt_dtor, {IDS});
        return;
    }

    // the popup, centred, over a fading shadow
    const uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
    const uint32_t shadow = SceneUtil::createShadowSprite();
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(shadowFade()), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PurchaseJson});
    vcall(popup, node::setAnchorPoint, {popup, ld32(dc::G_ANCHOR_MIDDLE)});
    {
        const float px = bitsf(vcall(popup, node::getPositionX, {popup}));
        const float w = ldf(vcall(popup, node::getContentSize, {popup}));
        const float mx = bitsf(call(dc::SceneUtil_getSideMargin, {}));
        const float x = mx + (px + w * 0.5f);
        const float py = bitsf(vcall(popup, node::getPositionY, {popup}));
        const float h = ldf(vcall(popup, node::getContentSize, {popup}) + 4);
        const float my = bitsf(call(dc::SceneUtil_getSideHeightMargin, {}));
        const float y = my + (py + h * 0.5f);
        const uint32_t v = base + 0xD0;
        call(ms::Vec2_ctor, {v, fbits(x), fbits(y)});
        vcall(popup, node::setPosition, {popup, v});
        call(ms::Vec2_dtor, {v});
    }
    vcall(shadow, node::addChildZ, {shadow, popup, 0});
    const uint32_t purchase = seek(popup, base + 0xC4, S_ButtonPurchase);
    const uint32_t close = seek(popup, base + 0xB8, S_CloseButtonPurchase);
    vcall(purchase, node::setVisible, {purchase, 1});
    vcall(close, node::setVisible, {close, 1});
    const uint32_t first = ld32(IDS);
    if (ld32(IDS + 4) == first) call(dc::VectorInt_throwOutOfRange, {IDS});   // ids.at(0)
    call(g::Shop_create, {ROW, ld32(first)});
    {
        const uint32_t bytes = base + 0x5B4, productId = base + 0xA4;
        safeBytesAt(bytes, ld32(ROW), master::Shop::PRODUCT_ID, 0x81);
        strInitC(productId, bytes);
        const uint32_t found = call(dc::ProductMap_find, {map, productId});
        strFree(productId);
        if (found) {
            const uint32_t txt = seek(purchase, base + 0x98, S_TxtPurchase);
            shortStr(base + 0x8C, HELVETICA);
            call(dc::Text_setFontName, {txt, base + 0x8C});
            strFree(base + 0x8C);
            call(dc::Text_setFontSize, {txt, PURCHASE_FONT_SIZE});
            const uint32_t text = base + 0x84;
            call(fn::TextData_get, {text, 0, uint32_t(TXT_PURCHASE_PRICE)});
            const uint32_t price = found + PRODUCT_PRICE;
            const uint32_t buf = base + 0x1B0;
            call(fn::StringBuffer_ctor, {buf, textDataStr(text), strData(price)});
            const uint32_t name = base + 0x78, label = base + 0x6C;
            strInitC(name, S_TxtPurchase);
            strInitC(label, buf);
            call(dc::SceneUtil_settingTitleLabelText, {purchase, name, label});
            strFree(label);
            strFree(name);
            sharedRelease(text);
        }
    }
    shortStr(base + 0x60, WEB_VIEW);
    const uint32_t webView = seekNamed(layer, base + 0x60);
    if (webView) vcall(webView, node::setVisible, {webView, 0});

    // Button_Purchase: P {purchase, close, shop row (shared), shopId, popup, webView, onPurchased +0x20}
    {
        const uint32_t P = base + 0x160, fo = base + 0x198;
        st32(P, purchase);
        st32(P + 4, close);
        st32(P + 8, ld32(ROW));
        st32(P + 0xC, ld32(ROW + 4));
        if (ld32(ROW + 4)) call(fn::shared_add, {ld32(ROW + 4)});
        st32(P + 0x10, ld32(first));
        st32(P + 0x14, popup);
        st32(P + 0x18, webView);
        call(fn::function_vv_copy, {P + 0x20, functor + 0x20});
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x40});
        if (heap) {
            st32(heap, VT_DiscountPurchase);
            st32(heap + 0xC, ld32(P + 4));
            st32(heap + 8, ld32(P));
            st32(heap + 0x10, ld32(P + 8));
            st32(heap + 0x14, ld32(P + 0xC));
            st32(P + 8, 0);
            st32(P + 0xC, 0);
            st32(heap + 0x18, ld32(P + 0x10));
            st32(heap + 0x1C, ld32(P + 0x14));
            st32(heap + 0x20, ld32(P + 0x18));
            functionCopy(heap + 0x28, P + 0x20);
        }
        st32(fo + 16, heap);
        call(fn::Widget_addTouchEventListener, {purchase, fo});
        functionDestroy(fo);
        functionDestroy(P + 0x20);
        if (ld32(P + 0xC)) call(fn::shared_release, {ld32(P + 0xC)});
    }
    // Close_Button_Purchase: C {purchase, close, popup, webView, onClose +0x10}
    {
        const uint32_t C = base + 0x120, fo = base + 0x148;
        st32(C, purchase);
        st32(C + 4, close);
        st32(C + 8, popup);
        st32(C + 0xC, webView);
        call(fn::function_vv_copy, {C + 0x10, onClose});
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x30});
        if (heap) {
            st32(heap, VT_DiscountClose);
            for (uint32_t k = 0; k < 16; k += 4) st32(heap + 8 + k, ld32(C + k));
            functionCopy(heap + 0x18, C + 0x10);
        }
        st32(fo + 16, heap);
        call(fn::Widget_addTouchEventListener, {close, fo});
        functionDestroy(fo);
        functionDestroy(C + 0x10);
    }
    {
        const uint32_t fo = base + 0x108;
        st32(fo + 16, 0);
        call(fn::Node_runAction, {popup, call(ms::SceneUtil_createOpenPopupAnimation, {popup, fo})});
        functionDestroy(fo);
    }
    const uint32_t sm = sceneManager();
    const uint32_t canClose = base + 0xF0, closed = base + 0xD8;
    inlineFunction(canClose, VT_DiscountCanClose, {purchase});
    inlineFunction(closed, VT_DiscountClosed, {layer, shadow});
    call(fn::SceneManager_openPopup, {sm, canClose, closed, 0});
    functionDestroy(closed);
    functionDestroy(canClose);
    sharedRelease(ROW);
    functionDestroy(COPY);
    call(g::VectorBaseInt_dtor, {IDS});
}
RW_FUNCTION(0x58E828, "BuyMagicStoneDialog::openDiscount $_1::operator()(vector<int>, const unordered_map*, bool)",
            BuyMagicStoneDialog_openDiscountResult);

static Addr BuyMagicStoneDialog_openDiscountClone(Addr functor) {
    const uint32_t heap = call(fn::operator_new, {0x38});
    if (heap) {
        st32(heap, VT_DiscountProducts);
        call(fn::function_vv_copy, {heap + 8, functor + 8});
        call(fn::function_vv_copy, {heap + 0x20, functor + 0x20});
    }
    return heap;
}
RW_FUNCTION(0x58E6A0, "BuyMagicStoneDialog::openDiscount $_1 __clone()", BuyMagicStoneDialog_openDiscountClone);

// ------------------------------------------------------------------------------------------------ the discount buttons
namespace {
constexpr uint32_t VT_DiscountCloseEnd = 0xF5A8D8;   // Close_Button_Purchase's close animation end {purchase, webView, onClose +0x10} (0x28)
constexpr uint32_t VT_DiscountPackSuspended = 0xF5A9E0;   // Button_Purchase's checkTransaction callback (no captures)
constexpr uint32_t VT_DiscountPurchased = 0xF5A930;  // startPurchase's callback {jewels, tickets, popup, purchase, webView, onPurchased +0x20, close +0x38} (0x40)
constexpr uint32_t VT_DiscountDone = 0xF5A988;       // purchaseCallback's done {popup, purchase, webView, onPurchased +0x18} (0x30)
constexpr uint32_t VT_DiscountRetry = 0xF5A95C;      // purchaseCallback's retry {purchase, close}
constexpr uint32_t VT_DiscountDoneEnd = 0xF5A9B4;    // the done close animation's end {purchase, webView, onPurchased +0x10} (0x28)
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;   // (sm, bool)
constexpr uint32_t TOUCH_ENDED = 2;

void setTouchEnabled(uint32_t widget, uint32_t on) { vcall(widget, V_SET_TOUCH_ENABLED, {widget, on}); }

// the popup's close animation; at its end (vtable `endVt`) a heap lambda {purchase, webView, a copy of `fn`}
void closeDiscountPopup(uint32_t popup, uint32_t purchase, uint32_t webView, uint32_t fn, uint32_t endVt) {
    Frame f;
    const uint32_t L = f.alloc(0x20);
    st32(L, purchase);
    st32(L + 4, webView);
    call(fn::function_vv_copy, {L + 8, fn});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, endVt);
        st32(heap + 8, ld32(L));
        st32(heap + 0xC, ld32(L + 4));
        functionCopy(heap + 0x10, L + 8);
    }
    st32(fo + 16, heap);
    call(fn::Node_runAction, {popup, call(ms::SceneUtil_createClosePopupAnimation, {popup, fo})});
    functionDestroy(fo);
    functionDestroy(L + 8);
}
// the close animation ended {purchase +8, webView +0xC, fn +0x10}: the popup closes, the web view comes back, fn
void discountPopupClosed(uint32_t functor, uint32_t closeArg) {
    const uint32_t purchase = ld32(functor + 8);
    setTouchEnabled(purchase, 1);
    call(SceneManager_closeCurrentPopup, {sceneManager(), closeArg});
    setTouchEnabled(ld32(functor + 8), 0);
    const uint32_t webView = ld32(functor + 0xC);
    if (webView) vcall(webView, node::setVisible, {webView, 1});
    if (ld32(functor + 0x20)) call(fn::function_vv_call, {functor + 0x10});
}
}  // namespace

// Close_Button_Purchase {purchase +8, close +0xC, popup +0x10, webView +0x14, onClose +0x18}
static void BuyMagicStoneDialog_discountCloseTouch(Addr functor, Addr, Addr typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    setTouchEnabled(ld32(functor + 8), 0);
    setTouchEnabled(ld32(functor + 0xC), 0);
    call(ms::AudioPlaySystemSECancel, {});
    closeDiscountPopup(ld32(functor + 0x10), ld32(functor + 8), ld32(functor + 0x14), functor + 0x18, VT_DiscountCloseEnd);
}
RW_FUNCTION(0x58F60C, "BuyMagicStoneDialog::openDiscount $_1 close touch lambda::operator()(Ref*, TouchEventType)",
            BuyMagicStoneDialog_discountCloseTouch);

static void BuyMagicStoneDialog_discountCloseEnd(Addr functor) { discountPopupClosed(functor, 0); }
RW_FUNCTION(0x58F91C, "BuyMagicStoneDialog::openDiscount $_1 close animation end::operator()()",
            BuyMagicStoneDialog_discountCloseEnd);

// Button_Purchase {purchase +8, close +0xC, shop row +0x10, shopId +0x18, popup +0x1C, webView +0x20,
// onPurchased +0x28}: the store purchase of the discounted pack (as the pack list's, with the buttons locked)
static void BuyMagicStoneDialog_discountPurchaseTouch(Addr functor, Addr, Addr typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    Frame frame;
    const uint32_t base = frame.allocExact(0x138);    // push 0x14 + 0x124
    const uint32_t L = base + 0x28, productId = base + 0x7C, fo = base + 0x60, check = base + 0x100;
    setTouchEnabled(ld32(functor + 8), 0);
    setTouchEnabled(ld32(functor + 0xC), 0);
    call(ms::AudioPlaySystemSEDecide, {});
    const uint32_t row = ld32(functor + 0x10);
    const uint32_t jewels = uint32_t(safe32(row, master::Shop::NUMBER) + safe32(row, master::Shop::BONUS));
    const uint32_t tickets = call(g::GetSkipTicketCnt, {ld32(functor + 0x18)});
    const uint32_t mgr = call(g::PurchaseManager_getInstance, {});
    if (ld8(mgr)) return;
    inlineFunction(check, VT_DiscountPackSuspended, {});
    const uint32_t suspended = call(ms::CheckTransaction, {check});
    functionDestroy(check);
    if (suspended) return;
    loadingIndicator(1);
    safeBytesAt(productId, ld32(functor + 0x10), master::Shop::PRODUCT_ID, 0x81);
    // L {jewels, tickets, popup, purchase, webView, onPurchased +0x18, close +0x30}
    st32(L, jewels);
    st32(L + 4, tickets);
    st32(L + 8, ld32(functor + 0x1C));
    st32(L + 0xC, ld32(functor + 8));
    st32(L + 0x10, ld32(functor + 0x20));
    call(fn::function_vv_copy, {L + 0x18, functor + 0x28});
    st32(L + 0x30, ld32(functor + 0xC));
    st32(fo + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x40});
    if (heap) {
        st32(heap, VT_DiscountPurchased);
        st32(heap + 0x14, ld32(L + 0xC));
        st32(heap + 0x10, ld32(L + 8));
        st32(heap + 0xC, ld32(L + 4));
        st32(heap + 8, ld32(L));
        st32(heap + 0x18, ld32(L + 0x10));
        functionCopy(heap + 0x20, L + 0x18);
        st32(heap + 0x38, ld32(L + 0x30));
    }
    st32(fo + 16, heap);
    call(ms::PurchaseManager_startPurchase, {mgr, productId, fo});
    functionDestroy(fo);
    functionDestroy(L + 0x18);
}
RW_FUNCTION(0x58FB90, "BuyMagicStoneDialog::openDiscount $_1 purchase touch lambda::operator()(Ref*, TouchEventType)",
            BuyMagicStoneDialog_discountPurchaseTouch);

// the store purchase ended: purchaseCallback(ok, jewels, no view, no popup, tickets, done, retry)
static void BuyMagicStoneDialog_discountPurchased(Addr functor, Addr okRef) {
    Frame frame;
    const uint32_t base = frame.allocExact(0x98);     // push 0x14 + 0x84: the stack arguments at sp
    const uint32_t retry = base + 0x20, L = base + 0x38, done = base + 0x60;
    const uint8_t ok = ld8(okRef);
    const uint32_t tickets = ld32(functor + 0xC), jewels = ld32(functor + 8);
    st32(L, ld32(functor + 0x10));
    st32(L + 4, ld32(functor + 0x14));
    st32(L + 8, ld32(functor + 0x18));
    call(fn::function_vv_copy, {L + 0x10, functor + 0x20});
    st32(done + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x30});
    if (heap) {
        st32(heap, VT_DiscountDone);
        for (uint32_t k = 0; k < 12; k += 4) st32(heap + 8 + k, ld32(L + k));
        functionCopy(heap + 0x18, L + 0x10);
    }
    st32(done + 16, heap);
    inlineFunction(retry, VT_DiscountRetry, {ld32(functor + 0x14), ld32(functor + 0x38)});
    rw::call_exact(ms::PurchaseCallback, {ok ? 1u : 0u, jewels, 0, 0, tickets, done, retry});
    functionDestroy(retry);
    functionDestroy(done);
    functionDestroy(L + 0x10);
}
RW_FUNCTION(0x58FF9C, "BuyMagicStoneDialog::openDiscount $_1 purchase lambda::operator()(bool)",
            BuyMagicStoneDialog_discountPurchased);

// purchaseCallback's done {popup +8, purchase +0xC, webView +0x10, onPurchased +0x18}: the popup closes
static void BuyMagicStoneDialog_discountDone(Addr functor) {
    closeDiscountPopup(ld32(functor + 8), ld32(functor + 0xC), ld32(functor + 0x10), functor + 0x18, VT_DiscountDoneEnd);
}
RW_FUNCTION(0x5903C0, "BuyMagicStoneDialog::openDiscount $_1 purchase done lambda::operator()()",
            BuyMagicStoneDialog_discountDone);

static void BuyMagicStoneDialog_discountDoneEnd(Addr functor) { discountPopupClosed(functor, 1); }
RW_FUNCTION(0x5906A4, "BuyMagicStoneDialog::openDiscount $_1 done animation end::operator()()",
            BuyMagicStoneDialog_discountDoneEnd);
