#include "PurchaseManager.hpp"

#include "scene_main/FunctionThunks.hpp"   // thunk::destroyFunction (the inline ~function)

namespace {

constexpr uint32_t DESTRUCTOR = 0x4D7B11;              // PurchaseManager::~PurchaseManager (registered with atexit)
constexpr uint32_t PROCESS_TRANSACTION = 0x4D7BFD;     // PurchaseManager::processTransaction (bound by init)
constexpr uint32_t FINISH_PURCHASE = 0x4D7E39;         // PurchaseManager::finishPurchase
constexpr uint32_t START_PURCHASE = 0x4D8055;          // PurchaseManager::startPurchase
constexpr uint32_t FINALIZE_CONNECTION = 0x4D8331;     // PurchaseManager::finalizeConnection
constexpr uint32_t FIND_INVALID_TRANSACTION = 0x4D83B9;  // PurchaseManager::findInvalidTransaction
constexpr uint32_t EXISTS_SAME_TRANSACTION = 0x4D844B;   // PurchaseManager::existsSameTransaction
constexpr uint32_t CHECK_RESUMED_PURCHASE = 0x4D84F1;    // PurchaseManager::checkResumedPurchase

constexpr uint32_t TX_LIST_CLEAR = 0x4D9211;           // __list_imp<shared_ptr<Transaction>>::clear
constexpr uint32_t TX_LIST_REMOVE = 0x4D867B;          // list<shared_ptr<Transaction>>::remove(const&)
constexpr uint32_t TX_LIST_ERASE = 0x4D86BD;           // list<shared_ptr<Transaction>>::erase(const_iterator)
constexpr uint32_t TX_LIST_PUSH_FRONT = 0x4D8641;      // list<shared_ptr<Transaction>>::push_front
constexpr uint32_t TX_LIST_PUSH_BACK = 0x4D8FE1;       // list<shared_ptr<Transaction>>::push_back
constexpr uint32_t TX_MAKE_SHARED = 0x4DA06D;          // shared_ptr<Transaction>::make_shared<>() (sret)
constexpr uint32_t PRODUCT_MAP_DTOR = 0x4DABC7;        // unordered_map<string, iap::ProductInfo>::~__hash_table
constexpr uint32_t PRODUCT_MAP_FIND = 0x4DA18F;        // unordered_map<string, iap::ProductInfo>::find
constexpr uint32_t PRODUCT_MAP_ASSIGN = 0x4D930D;      // __hash_table<..ProductInfo..>::__assign_multi(first, last)
constexpr uint32_t STRING_SET_INSERT = 0x4DA505;       // unordered_set<string>::__insert_unique (sret pair)
constexpr uint32_t SAVEBIN_DTOR = 0x4DAB71;            // unordered_map<string, shared_ptr<bg::Data>>::~__hash_table

constexpr uint32_t STORE_GET_INSTANCE = 0x4E0DD5;      // iap::StoreManager::getInstance
constexpr uint32_t STORE_SET_CALLBACK = 0x4E10C1;      // iap::StoreManager::setPurchaseCallback(const function&)
constexpr uint32_t API_GET_INSTANCE = 0x4BA8C9;        // APIManager::getInstance
constexpr uint32_t API_PURCHASE_PREPARE = 0x4CFC49;    // APIManager::executePurchasePrepare(id, currency, cb)
constexpr uint32_t API_PURCHASE_FINISH = 0x4CFE41;     // APIManager::executePurchaseFinish(tx, cb)
constexpr uint32_t APPSTATE_GET_INSTANCE = 0x4E66DD;   // AppState::getInstance
constexpr uint32_t APPSTATE_SAVE = 0x4E8DC9;           // AppState::save

constexpr uint32_t SAVEBIN_SET_U32 = 0x4D8881;         // SaveBin::set<unsigned int>(key, const&)
constexpr uint32_t SAVEBIN_SET_STRING = 0x4D899D;      // SaveBin::set<std::string>(key, const&)
constexpr uint32_t SAVEBIN_SET_INT = 0x4D8AB5;         // SaveBin::set<int>(key, const&)
constexpr uint32_t SAVEBIN_SET_DATA = 0x4F0CE1;        // SaveBin::set(key, const shared_ptr<bg::Data>&)
constexpr uint32_t SAVEBIN_SERIALIZE = 0x4F0E95;       // SaveBin::serialize() (sret)
constexpr uint32_t SAVEBIN_GET_INT = 0x4D8DDD;         // SaveBin::get<int>(key, int&)
constexpr uint32_t SAVEBIN_GET_U32 = 0x4D8E8D;         // SaveBin::get<unsigned int>(key, unsigned&)
constexpr uint32_t SAVEBIN_GET_STRING = 0x4D8F3D;      // SaveBin::get<std::string>(key, string&)
constexpr uint32_t SAVEBIN_GET_DATA = 0x4F0DED;        // SaveBin::get(key) -> shared_ptr<bg::Data> (sret)
constexpr uint32_t SAVEBIN_DESERIALIZE = 0x4F0FE9;     // SaveBin::deserialize(const shared_ptr<bg::Data>&)
constexpr uint32_t STRINGBUFFER_FSET = 0x491501;       // StringBuffer::fset(fmt, ...)

constexpr uint32_t STRING_ASSIGN = 0x498A1D;           // std::string::operator=(const string&) (sub_498A1C)
constexpr uint32_t STRING_INIT = 0x47CD93;             // std::string::__init(const char*, size_t) (sub_47CD92)
constexpr uint32_t FUNCTION_VB_COPY = 0x45C84D;        // std::function<void(bool)>::function(const function&)
constexpr uint32_t FUNCTION_VB_ASSIGN = 0x490109;      // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FUNCTION_VB_CALL = 0x474139;        // std::function<void(bool)>::operator()(bool)
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;            // __aeabi_memset(dst, n, c)
constexpr uint32_t AEABI_MEMCPY = 0x40E164;            // __aeabi_memcpy(dst, src, n)
constexpr uint32_t MEMCMP = 0x40EF2C;                  // memcmp
constexpr uint32_t STRLEN = 0x40E08C;                  // strlen
constexpr uint32_t ANDROID_LOG_PRINT = 0x413D38;       // __android_log_print

// std::bind(&PurchaseManager::processTransaction, this, _1, _2) inside a std::function<void(iap::Result,
// const iap::Purchase*)>: __func vtable + 8
constexpr uint32_t BIND_PROCESS_VTABLE = 0xF7F994;
// the lambdas handed to the APIManager requests (__func vtables + 8)
constexpr uint32_t START_LAMBDA_VTABLE = 0xF556B0;     // startPurchase::$_1 (purchase/prepare finished)
constexpr uint32_t FINISH_LAMBDA_VTABLE = 0xF556DC;    // finishPurchase::$_0 (purchase/finish finished)

constexpr uint32_t FLOAT_ONE = 0x3F800000;             // 1.0f (max load factor of a fresh unordered container)

// strings
constexpr uint32_t S_BRIDGE_TRANS_ID = 0xDFAF88;       // "bridgeTransId"
constexpr uint32_t S_PRODUCT_ID = 0xDFBFCA;            // "productId"
constexpr uint32_t S_PURCHASE_DATA = 0xDFBFE0;         // "purchaseData"
constexpr uint32_t S_DATA_SIGNATURE = 0xDFBFED;        // "dataSignature"
constexpr uint32_t S_COUNT = 0xDFB5EE;                 // "count"
constexpr uint32_t S_FORMAT_D = 0xDF86DE;              // "%d"
constexpr uint32_t S_USD = 0xDFC18E;                   // "USD"
constexpr uint32_t S_ASSERT_TAG = 0xDFC129;            // "cocos2d-x assert"
constexpr uint32_t S_ASSERT_FORMAT = 0xDFC13A;         // "%s function:%s line:%d"
constexpr uint32_t S_ASSERT_FILE = 0xDFC151;           // "jni/../../hole/src/network/PurchaseManager.cpp"
constexpr uint32_t S_START_PURCHASE = 0xDFC180;        // "startPurchase"

using Transaction = PurchaseManager::Transaction;

uint32_t load32(uint32_t a) { return rw::load<uint32_t>(a); }
void put32(const guest::Frame &f, uint32_t off, uint32_t v) { rw::store32(f.at<uint32_t>(off), v); }
void put8(const guest::Frame &f, uint32_t off, uint8_t v) { rw::store8(f.at<uint8_t>(off), v); }
uint32_t get32(const guest::Frame &f, uint32_t off) { return load32(f.addr(off)); }

// The transaction a list node holds.
Transaction *txOf(uint32_t node) { return rw::host_ptr<Transaction>(load32(node + 8)); }

// Copy what the store reported into a transaction (the string assignments, in the original order).
void copyPurchase(Transaction *tx, const IapPurchase *p) {
    rw::call(STRING_ASSIGN, {rw::guest_addr(&tx->productId), rw::guest_addr(&p->productId)});
    tx->flag = p->flag;
    rw::call(STRING_ASSIGN, {rw::guest_addr(&tx->orderId), rw::guest_addr(&p->orderId)});
    rw::call(STRING_ASSIGN, {rw::guest_addr(&tx->token), rw::guest_addr(&p->token)});
    rw::call(STRING_ASSIGN, {rw::guest_addr(&tx->purchaseData), rw::guest_addr(&p->purchaseData)});
    rw::call(STRING_ASSIGN, {rw::guest_addr(&tx->dataSignature), rw::guest_addr(&p->dataSignature)});
}

// String equality as the compiler inlined it: different sizes differ; a long transaction string is compared with
// memcmp, a short one byte by byte.
bool sameString(const guest::String &txString, const guest::String &purchaseString) {
    uint32_t n = txString.size();
    if (n != purchaseString.size()) return false;
    uint32_t a = txString.data(), b = purchaseString.data();
    if (txString.isLong()) return rw::call(MEMCMP, {a, b, n}) == 0;
    for (uint32_t k = 0; k < n; k++)
        if (rw::load<uint8_t>(a + k) != rw::load<uint8_t>(b + k)) return false;
    return true;
}

// The inline ~function(): destroy() when the callable is in the buffer, destroy_deallocate() when on the heap.
void destroyFunction(uint32_t fn) { thunk::destroyFunction(fn); }

void releaseCtrl(uint32_t ctrl) {
    if (ctrl) guest::release(ctrl);
}

}  // namespace

// ---------------------------------------------------------------------------------------------- lifetime
PurchaseManager *PurchaseManager::getInstance() {
    PurchaseManager *self = rw::host_ptr<PurchaseManager>(INSTANCE);
    if (guest::staticInitBegin(GUARD)) {
        self->connecting = 0;
        self->callbackFn = 0;
        self->listPrev = INSTANCE + 4;
        self->listNext = INSTANCE + 4;
        self->listSize = 0;
        rw::call(AEABI_MEMSET, {INSTANCE + 0x2C, 0x18, 0});   // result .. product map size
        self->productMaxLoad = FLOAT_ONE;
        rw::call(guest::CXA_ATEXIT, {DESTRUCTOR, INSTANCE, guest::DSO_HANDLE});
        guest::staticInitDone(GUARD);
    }
    return self;
}
RW_FUNCTION(0x4D7A70, "PurchaseManager::getInstance", PurchaseManager::getInstance);

PurchaseManager *PurchaseManager::destruct(PurchaseManager *self) {
    rw::call(PRODUCT_MAP_DTOR, {self->addr() + 0x34});
    releaseCtrl(self->current.ctrl);
    destroyFunction(self->addr() + 0x10);
    rw::call(TX_LIST_CLEAR, {self->addr() + 4});
    return self;
}
RW_FUNCTION(0x4D7B10, "PurchaseManager::~PurchaseManager", PurchaseManager::destruct);

// init / deinit: both reach iap::StoreManager::getInstance, whose first call constructs the JNI-backed store
// (GooglePlayManager.getInstance through JniHelper) - not an effect the verifier can replay, so not shadow-run.
void PurchaseManager::init(PurchaseManager *self) {
    guest::Frame f(0x40);   // push {r4-r7, lr}; sub sp, #0x2c
    uint32_t store = rw::call(STORE_GET_INSTANCE, {});
    // std::function at sp+8 holding std::bind(&processTransaction, this, _1, _2)
    const uint32_t fn = f.addr(8);
    put32(f, 0x18, fn);                       // callable in the inline buffer
    put32(f, 8, BIND_PROCESS_VTABLE);
    put32(f, 12, PROCESS_TRANSACTION);        // pointer to member: function, adjustment 0
    put32(f, 16, 0);
    put32(f, 20, self->addr());
    rw::call(STORE_SET_CALLBACK, {store, fn});
    destroyFunction(fn);
}
RW_FUNCTION_NOVERIFY(0x4D7B50, "PurchaseManager::init", PurchaseManager::init);

void PurchaseManager::deinit(PurchaseManager *) {
    guest::Frame f(0x38);   // push {r4-r6, lr}; sub sp, #0x28
    uint32_t store = rw::call(STORE_GET_INSTANCE, {});
    const uint32_t fn = f.addr(8);
    put32(f, 0x18, 0);                        // an empty std::function
    rw::call(STORE_SET_CALLBACK, {store, fn});
    destroyFunction(fn);
}
RW_FUNCTION_NOVERIFY(0x4D7D04, "PurchaseManager::deinit", PurchaseManager::deinit);

// ---------------------------------------------------------------------------------------------- store callback
// The store's result for a purchase. While a purchase of ours is running (connecting, and no other request
// busy), a success completes the current transaction and sends it to the server, a failure drops it; anything
// else (a purchase restored at start-up, ...) goes through checkResumedPurchase.
void PurchaseManager::processTransaction(PurchaseManager *self, int32_t result, const IapPurchase *purchase) {
    constexpr int32_t RESULT_SUCCESS = 1;
    bool ours = false;
    if (self->connecting) {
        uint32_t api = rw::call(API_GET_INSTANCE, {});
        ours = rw::load<uint8_t>(api + 4) == 0;
    }
    if (!ours) {
        if (result == RESULT_SUCCESS) rw::call(CHECK_RESUMED_PURCHASE, {self->addr(), rw::guest_addr(purchase)});
        return;
    }
    self->result = result;
    uint32_t current = self->current.obj;
    if (result == RESULT_SUCCESS) {
        if (!current) {
            rw::call(CHECK_RESUMED_PURCHASE, {self->addr(), rw::guest_addr(purchase)});
            return;
        }
        uint32_t node = self->listNext;
        while (node != self->listEnd() && load32(node + 8) != current) node = load32(node + 4);
        copyPurchase(txOf(node), purchase);
        txOf(node)->state = 2;
        rw::call(APPSTATE_SAVE, {rw::call(APPSTATE_GET_INSTANCE, {})});
        rw::call(FINISH_PURCHASE, {self->addr(), node, self->addr() + 0x10, 0});
        return;
    }
    if (current) {
        rw::call(TX_LIST_REMOVE, {self->addr() + 4, self->addr() + 0x2C});
    } else {
        uint32_t node = rw::call(FIND_INVALID_TRANSACTION, {self->addr(), rw::guest_addr(purchase)});
        if (node != self->listEnd()) rw::call(TX_LIST_ERASE, {self->addr() + 4, node});
    }
    rw::call(APPSTATE_SAVE, {rw::call(APPSTATE_GET_INSTANCE, {})});
    rw::call(FINALIZE_CONNECTION, {self->addr(), 0});
}
RW_FUNCTION(0x4D7BFC, "PurchaseManager::processTransaction", PurchaseManager::processTransaction);

// ---------------------------------------------------------------------------------------------- queries
uint32_t PurchaseManager::hasSuspendedTransaction(const PurchaseManager *self) {
    for (uint32_t node = self->listNext; node != self->listEnd(); node = load32(node + 4))
        if (txOf(node)->state != 0) return 1;
    return 0;
}
RW_FUNCTION(0x4D7D8C, "PurchaseManager::hasSuspendedTransaction", PurchaseManager::hasSuspendedTransaction);

// out: std::unordered_set<std::string> of the product IDs of the transactions still open
void PurchaseManager::getSuspendedTransactionProductIDs(uint32_t out, const PurchaseManager *self) {
    guest::Frame f(0x28);   // push {r4-r7, lr}; sub sp, #0x14
    rw::save<uint32_t>(out + 0, 0);
    rw::save<uint32_t>(out + 4, 0);
    rw::save<uint32_t>(out + 8, 0);
    rw::save<uint32_t>(out + 12, 0);
    rw::save<uint32_t>(out + 16, FLOAT_ONE);
    for (uint32_t node = self->listNext; node != self->listEnd(); node = load32(node + 4)) {
        Transaction *tx = txOf(node);
        if (tx->state != 0) rw::call(STRING_SET_INSERT, {f.addr(8), out, rw::guest_addr(&tx->productId)});
    }
}
RW_FUNCTION(0x4D7DB0, "PurchaseManager::getSuspendedTransactionProductIDs",
            PurchaseManager::getSuspendedTransactionProductIDs);

// The first open-list node holding an invalid (state 0) transaction for the purchase's product; end() when none
// (or when purchase is null).
uint32_t PurchaseManager::findInvalidTransaction(const PurchaseManager *self, const IapPurchase *purchase) {
    uint32_t end = self->listEnd();
    if (!purchase) return end;
    uint32_t node = self->listNext;
    for (; node != end; node = load32(node + 4)) {
        Transaction *tx = txOf(node);
        if (tx->state == 0 && sameString(tx->productId, purchase->productId)) return node;
    }
    return node;
}
RW_FUNCTION(0x4D83B8, "PurchaseManager::findInvalidTransaction", PurchaseManager::findInvalidTransaction);

// Whether a transaction with the purchase's order ID is already in the list.
uint32_t PurchaseManager::existsSameTransaction(const PurchaseManager *self, const IapPurchase *purchase) {
    if (!purchase) return 0;
    uint32_t end = self->listEnd();
    for (uint32_t node = self->listNext; node != end; node = load32(node + 4))
        if (sameString(txOf(node)->orderId, purchase->orderId)) return 1;
    return 0;
}
RW_FUNCTION(0x4D844A, "PurchaseManager::existsSameTransaction", PurchaseManager::existsSameTransaction);

// A purchase the store reports outside a running purchase (restored at start-up): unless it is known already,
// fill the matching invalid transaction (finishing when it has a server id, else purchased) or add a new
// purchased transaction at the front.
void PurchaseManager::checkResumedPurchase(PurchaseManager *self, const IapPurchase *purchase) {
    guest::Frame f(0x28);   // push {r4-r7, lr}; sub sp, #0x14
    if (rw::call(EXISTS_SAME_TRANSACTION, {self->addr(), rw::guest_addr(purchase)})) return;
    uint32_t node = rw::call(FIND_INVALID_TRANSACTION, {self->addr(), rw::guest_addr(purchase)});
    if (node != self->listEnd()) {
        copyPurchase(txOf(node), purchase);
        Transaction *tx = txOf(node);
        tx->state = tx->bridgeTransId ? 2 : 1;
        return;
    }
    const uint32_t ptr = f.addr(8);   // shared_ptr<Transaction>
    rw::call(TX_MAKE_SHARED, {ptr});
    Transaction *tx = rw::host_ptr<Transaction>(get32(f, 8));
    copyPurchase(tx, purchase);
    tx->state = 1;
    rw::call(TX_LIST_PUSH_FRONT, {self->addr() + 4, ptr});
    releaseCtrl(get32(f, 12));
}
RW_FUNCTION(0x4D84F0, "PurchaseManager::checkResumedPurchase", PurchaseManager::checkResumedPurchase);

void PurchaseManager::setProductInfoMap(PurchaseManager *self, uint32_t map) {
    if (self->addr() + 0x34 == map) return;
    self->productMaxLoad = load32(map + 16);
    rw::call(PRODUCT_MAP_ASSIGN, {self->addr() + 0x34, load32(map + 8), 0});
}
RW_FUNCTION(0x4D86EC, "PurchaseManager::setProductInfoMap", PurchaseManager::setProductInfoMap);

// ---------------------------------------------------------------------------------------------- purchase flow
// Send the transaction at `node` to the server (purchase/finish); the request's callback is finishPurchase::$_0
// {this, node, shared_ptr<Transaction>, resume, callback}.
void PurchaseManager::finishPurchase(PurchaseManager *self, uint32_t node, uint32_t callback, uint32_t resume) {
    guest::Frame f(0x80);   // push {r4-r7, lr}; sub sp, #0x6c
    // the original frame: tx copy sp+0x0C/0x10, lambda temp sp+0x18 {this, node, tx, ctrl, resume +0x10,
    // function +0x18 (callable at +0x28)}, outer std::function sp+0x48 (callable at sp+0x58)
    put32(f, 4, resume);
    put32(f, 8, callback);
    const uint32_t tx = load32(node + 8);
    put32(f, 0x0C, tx);
    const uint32_t ctrl = load32(node + 12);
    if (ctrl) guest::addRef(ctrl);
    put32(f, 0x10, ctrl);
    const uint32_t api = rw::call(API_GET_INSTANCE, {});
    put32(f, 0x18, self->addr());
    put32(f, 0x1C, node);
    put32(f, 0x20, tx);
    put32(f, 0x24, ctrl);
    if (ctrl) guest::addRef(ctrl);
    put32(f, 0, api);
    put8(f, 0x28, uint8_t(resume));
    const uint32_t lambdaFn = f.addr(0x30);
    rw::call(FUNCTION_VB_COPY, {lambdaFn, callback});
    put32(f, 0x58, 0);
    const uint32_t obj = guest::newObject(0x38);
    if (obj) {
        rw::save<uint32_t>(obj, FINISH_LAMBDA_VTABLE);
        rw::save<uint32_t>(obj + 0x0C, node);
        rw::save<uint32_t>(obj + 0x08, self->addr());
        rw::save<uint32_t>(obj + 0x10, get32(f, 0x20));
        rw::save<uint32_t>(obj + 0x14, get32(f, 0x24));
        put32(f, 0x20, 0);                     // moved into the heap lambda
        put32(f, 0x24, 0);
        rw::save<uint8_t>(obj + 0x18, rw::load<uint8_t>(f.addr(0x28)));
        const uint32_t fn = get32(f, 0x40);
        if (!fn) {
            rw::save<uint32_t>(obj + 0x30, 0);
        } else if (fn == lambdaFn) {
            rw::save<uint32_t>(obj + 0x30, obj + 0x20);
            rw::call(load32(load32(fn) + 0x0C), {fn, obj + 0x20});   // __clone(dst)
        } else {
            rw::save<uint32_t>(obj + 0x30, rw::call(load32(load32(fn) + 0x08), {fn}));   // __clone()
        }
        rw::save<uint8_t>(obj + 0x18, rw::load<uint8_t>(f.addr(0x28)));
    }
    put32(f, 0x58, obj);
    rw::call(API_PURCHASE_FINISH, {get32(f, 0), get32(f, 0x0C), f.addr(0x48)});
    destroyFunction(f.addr(0x48));
    const uint32_t firstCopy = get32(f, 0x10);
    destroyFunction(lambdaFn);
    releaseCtrl(get32(f, 0x24));
    releaseCtrl(firstCopy);
}
RW_FUNCTION(0x4D7E38, "PurchaseManager::finishPurchase", PurchaseManager::finishPurchase);

// Continue the first open transaction: a purchased one is prepared again, a finishing one is sent again.
void PurchaseManager::resumeTransaction(PurchaseManager *self, uint32_t callback) {
    guest::Frame f(24);   // push {r4-r6, lr}; sub sp, #8 (startPurchase's fifth argument goes at sp)
    self->connecting = 1;
    for (uint32_t node = self->listNext; node != self->listEnd(); node = load32(node + 4)) {
        Transaction *tx = txOf(node);
        if (tx->state == 1) {
            put32(f, 0, node);
            rw::call_exact(START_PURCHASE, {self->addr(), tx->productId.data(), callback, 1, node});
            return;
        }
        if (tx->state == 2) {
            rw::call(FINISH_PURCHASE, {self->addr(), node, callback, 1});
            return;
        }
    }
}
RW_FUNCTION(0x4D8008, "PurchaseManager::resumeTransaction", PurchaseManager::resumeTransaction);

// Ask the server to prepare a purchase of productId (purchase/prepare with the product's currency from the
// store's product info, "USD" when the product is unknown); the request's callback is startPurchase::$_1
// {resume, node, this, callback, productId}.
void PurchaseManager::startPurchase(PurchaseManager *self, uint32_t productId, uint32_t callback, uint32_t resume,
                                    uint32_t node) {
    // The original frame (0xB8 bytes) is reproduced: the lambda captures are copied out of it with memcpy and
    // a short product-ID string with ldm/stm, both including bytes the original never wrote (stale stack).
    guest::Frame f(0xB8);   // push {r4-r7, lr}; sub sp, #0xa4
    const uint32_t pidString = f.addr(0x30), keyString = f.addr(0x3C), lambda = f.addr(0x48),
                   lambdaFn = f.addr(0x58), movedString = f.addr(0x70), outerFn = f.addr(0x80);
    put32(f, 0x1C, pidString);
    put32(f, 0x0C, pidString + 8);
    put32(f, 0x18, resume);
    put32(f, 0x20, callback);
    self->connecting = 1;
    rw::call(FUNCTION_VB_ASSIGN, {self->addr() + 0x10, callback});
    self->result = 1;
    put32(f, 0x24, 0);
    self->current.obj = 0;
    const uint32_t oldCtrl = self->current.ctrl;
    self->current.ctrl = 0;
    releaseCtrl(oldCtrl);
    put32(f, 0x28, productId);
    rw::call(STRING_INIT, {keyString, productId, rw::call(STRLEN, {productId})});
    const uint32_t info = rw::call(PRODUCT_MAP_FIND, {self->addr() + 0x34, keyString});
    if (rw::load<uint8_t>(keyString) & 1) guest::deleteObject(get32(f, 0x44));
    uint32_t currency;
    if (info) {
        // node: next, hash, key string +8, iap::ProductInfo +0x14 (currency code string at node + 0x2C)
        currency = (rw::load<uint8_t>(info + 0x2C) & 1) ? load32(info + 0x34) : info + 0x2D;
    } else {
        rw::call(ANDROID_LOG_PRINT, {6, S_ASSERT_TAG, S_ASSERT_FORMAT, S_ASSERT_FILE, S_START_PURCHASE, 125});
        currency = S_USD;
    }
    put32(f, 0x14, currency);
    rw::call(STRING_INIT, {pidString, productId, rw::call(STRLEN, {productId})});
    put32(f, 0x10, rw::call(API_GET_INSTANCE, {}));
    put8(f, 0x48, uint8_t(resume));
    put32(f, 0x4C, node);
    put32(f, 0x50, self->addr());
    rw::call(FUNCTION_VB_COPY, {lambdaFn, callback});
    const uint32_t pidLong = rw::load<uint8_t>(pidString) & 1;
    put32(f, 0x20, 1);
    put32(f, 0x18, pidLong);
    if (pidLong) {
        rw::call(STRING_INIT, {movedString, get32(f, 0x38), get32(f, 0x34)});   // copy (data, size)
    } else {
        put32(f, 0x70, get32(f, 0x30));      // the 12 bytes of the short string, as they are
        put32(f, 0x74, get32(f, 0x34));
        put32(f, 0x78, get32(f, 0x38));
    }
    put32(f, 0x90, 0);
    const uint32_t obj = guest::newObject(0x40);
    if (obj) {
        rw::save<uint32_t>(obj, START_LAMBDA_VTABLE);
        rw::call(AEABI_MEMCPY, {obj + 8, lambda, 12});
        const uint32_t fn = get32(f, 0x68);
        if (!fn) {
            rw::save<uint32_t>(obj + 0x28, 0);
        } else if (fn == lambdaFn) {
            rw::save<uint32_t>(obj + 0x28, obj + 0x18);
            rw::call(load32(load32(lambdaFn) + 0x0C), {lambdaFn, obj + 0x18});   // __clone(dst)
        } else {
            rw::save<uint32_t>(obj + 0x28, rw::call(load32(load32(fn) + 0x08), {fn}));   // __clone()
        }
        rw::save<uint32_t>(obj + 0x30, get32(f, 0x70));   // move the product-ID string
        rw::save<uint32_t>(obj + 0x34, get32(f, 0x74));
        rw::save<uint32_t>(obj + 0x38, get32(f, 0x78));
        put32(f, 0x70, 0);
        put32(f, 0x74, 0);
        put32(f, 0x78, 0);
    }
    put32(f, 0x90, obj);
    rw::call(API_PURCHASE_PREPARE, {get32(f, 0x10), get32(f, 0x28), get32(f, 0x14), outerFn});
    destroyFunction(outerFn);
    if (rw::load<uint8_t>(movedString) & 1) guest::deleteObject(get32(f, 0x78));
    destroyFunction(lambdaFn);
    if (get32(f, 0x18)) guest::deleteObject(load32(get32(f, 0x0C)));
}
RW_FUNCTION(0x4D8054, "PurchaseManager::startPurchase", PurchaseManager::startPurchase);

void PurchaseManager::purchase(PurchaseManager *self, uint32_t productId, uint32_t callback) {
    guest::Frame f(16);   // push {r4, lr}; sub sp, #8 (startPurchase's fifth argument goes at sp)
    put32(f, 0, self->addr() + 4);
    rw::call_exact(START_PURCHASE, {self->addr(), productId, callback, 0, self->addr() + 4});   // node: end()
}
RW_FUNCTION(0x4D831C, "PurchaseManager::purchase", PurchaseManager::purchase);

// End the running purchase: drop the current transaction and call (then clear) the completion callback.
void PurchaseManager::finalizeConnection(PurchaseManager *self, uint32_t success) {
    self->current.obj = 0;
    const uint32_t oldCtrl = self->current.ctrl;
    self->current.ctrl = 0;
    releaseCtrl(oldCtrl);
    self->connecting = 0;
    const uint32_t buf = self->addr() + 0x10;
    uint32_t fn = 0;
    if (self->callbackFn) {
        rw::call(FUNCTION_VB_CALL, {buf, success});
        fn = self->callbackFn;
    }
    if (fn == buf) rw::call(load32(load32(fn) + 0x10), {fn});        // destroy()
    else if (fn) rw::call(load32(load32(fn) + 0x14), {fn});          // destroy_deallocate()
    self->callbackFn = 0;
}
RW_FUNCTION(0x4D8330, "PurchaseManager::finalizeConnection", PurchaseManager::finalizeConnection);

void PurchaseManager::abort(PurchaseManager *self) {
    destroyFunction(self->addr() + 0x10);
    self->callbackFn = 0;
    rw::call(FINALIZE_CONNECTION, {self->addr(), 0});
}
RW_FUNCTION(0x4D8388, "PurchaseManager::abort", PurchaseManager::abort);

// ---------------------------------------------------------------------------------------------- save data
// The transactions that reached the server (bridgeTransId set) and are not just purchased: a SaveBin per
// transaction {bridgeTransId, productId, purchaseData, dataSignature} under "0", "1", ..., plus "count".
void PurchaseManager::serialize(uint32_t out, const PurchaseManager *self) {
    // frame: Data blob sp+0x18, entry SaveBin sp+0x20, count sp+0x34, SaveBin sp+0x38, StringBuffer sp+0x4C
    guest::Frame f(0x468);   // push {r4-r7, lr}; sub sp, #0x454
    const uint32_t blob = f.addr(0x18), entry = f.addr(0x20), count = f.addr(0x34), bin = f.addr(0x38),
                   key = f.addr(0x4C);
    put32(f, 0x0C, 0);
    put32(f, 0x10, FLOAT_ONE);
    put32(f, 0x34, 0);
    put32(f, 0x38, 0);
    put32(f, 0x3C, 0);
    put32(f, 0x40, 0);
    put32(f, 0x44, 0);
    put32(f, 0x48, FLOAT_ONE);
    put32(f, 0x4C + 0x400, 0);    // StringBuffer: length, empty text
    put8(f, 0x4C, 0);
    for (uint32_t node = self->listNext; node != self->listEnd(); node = load32(node + 4)) {
        Transaction *tx = txOf(node);
        if (!tx->bridgeTransId || tx->state == 1) continue;
        put32(f, 0x20, 0);
        put32(f, 0x24, 0);
        put32(f, 0x28, 0);
        put32(f, 0x2C, 0);
        put32(f, 0x30, FLOAT_ONE);
        const uint32_t t = load32(node + 8);
        rw::call(SAVEBIN_SET_U32, {entry, S_BRIDGE_TRANS_ID, t});
        rw::call(SAVEBIN_SET_STRING, {entry, S_PRODUCT_ID, t + 4});
        rw::call(SAVEBIN_SET_STRING, {entry, S_PURCHASE_DATA, load32(node + 8) + 0x2C});
        rw::call(SAVEBIN_SET_STRING, {entry, S_DATA_SIGNATURE, load32(node + 8) + 0x38});
        rw::call(STRINGBUFFER_FSET, {key, S_FORMAT_D, get32(f, 0x34)});
        rw::call(SAVEBIN_SERIALIZE, {blob, entry});
        rw::call(SAVEBIN_SET_DATA, {bin, key, blob});
        releaseCtrl(get32(f, 0x1C));
        put32(f, 0x34, get32(f, 0x34) + 1);
        rw::call(SAVEBIN_DTOR, {entry});
    }
    rw::call(SAVEBIN_SET_INT, {bin, S_COUNT, count});
    rw::call(SAVEBIN_SERIALIZE, {out, bin});
    rw::call(SAVEBIN_DTOR, {bin});
}
RW_FUNCTION(0x4D8708, "PurchaseManager::serialize", PurchaseManager::serialize);

// Rebuild the list from the save data: every entry becomes a finishing transaction; reading stops at the first
// entry without purchase data or signature.
void PurchaseManager::deserialize(PurchaseManager *self, uint32_t data) {
    // frame: shared_ptr<Transaction> sp+0x20, entry SaveBin sp+0x28, entry blob sp+0x3C, count sp+0x44,
    // SaveBin sp+0x48, StringBuffer sp+0x5C
    guest::Frame f(0x478);   // push {r4-r7, lr}; sub sp, #0x464
    const uint32_t ptr = f.addr(0x20), entry = f.addr(0x28), blob = f.addr(0x3C), count = f.addr(0x44),
                   bin = f.addr(0x48), key = f.addr(0x5C);
    rw::call(TX_LIST_CLEAR, {self->addr() + 4});
    put32(f, 0x10, FLOAT_ONE);
    put32(f, 0x48, 0);
    put32(f, 0x4C, 0);
    put32(f, 0x50, 0);
    put32(f, 0x54, 0);
    put32(f, 0x58, FLOAT_ONE);
    rw::call(SAVEBIN_DESERIALIZE, {bin, data});
    if (rw::call(SAVEBIN_GET_INT, {bin, S_COUNT, count})) {
        put32(f, 0x18, 1);
        put32(f, 0x5C + 0x400, 0);    // StringBuffer: length, empty text
        put8(f, 0x5C, 0);
        for (int32_t i = 0; i < int32_t(get32(f, 0x44)); i++) {
            rw::call(STRINGBUFFER_FSET, {key, S_FORMAT_D, uint32_t(i)});
            rw::call(SAVEBIN_GET_DATA, {blob, bin, key});
            put32(f, 0x28, 0);
            put32(f, 0x2C, 0);
            put32(f, 0x30, 0);
            put32(f, 0x34, 0);
            put32(f, 0x38, FLOAT_ONE);
            rw::call(SAVEBIN_DESERIALIZE, {entry, blob});
            rw::call(TX_MAKE_SHARED, {ptr});
            const uint32_t t = get32(f, 0x20);
            Transaction *tx = rw::host_ptr<Transaction>(t);
            rw::call(SAVEBIN_GET_U32, {entry, S_BRIDGE_TRANS_ID, t});
            rw::call(SAVEBIN_GET_STRING, {entry, S_PRODUCT_ID, t + 4});
            rw::call(SAVEBIN_GET_STRING, {entry, S_PURCHASE_DATA, t + 0x2C});
            rw::call(SAVEBIN_GET_STRING, {entry, S_DATA_SIGNATURE, t + 0x38});
            const bool complete = tx->purchaseData.size() != 0 && tx->dataSignature.size() != 0;
            if (complete) {
                tx->state = 2;
                rw::call(TX_LIST_PUSH_BACK, {self->addr() + 4, ptr});
            }
            releaseCtrl(get32(f, 0x24));
            rw::call(SAVEBIN_DTOR, {entry});
            releaseCtrl(get32(f, 0x40));
            if (!complete) break;
        }
    }
    rw::call(SAVEBIN_DTOR, {bin});
}
RW_FUNCTION(0x4D8BD0, "PurchaseManager::deserialize", PurchaseManager::deserialize);
