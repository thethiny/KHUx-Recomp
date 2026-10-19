// BuyMagicStoneDialog: the Buy Jewels flow (all static: the dialog object is built by the callback of init).
// open -> init: pick the Jewel packs (master::Shop rows of this OS, sorted by sortId), ask the server for the
// payment info (/payment/info, the player's monthly billing), then the shop state (/user/shop), then the store for
// the products' prices (iap::StoreManager::sendProductsRequest), then call back with the ids and the prices.
// The three info buttons (Jewel info, Fund Settlement Act, Specified Commercial Transactions Act) are shown when
// the server's function flags enable them. openStoneInfoPopup, purchaseCallback and the discount variants are not
// rewritten yet.
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
