// PurchaseManager: in-app purchases (Google Play billing through iap::StoreManager, receipts checked by the game
// server). A purchase runs as: purchase() -> startPurchase() asks the server to prepare (purchase/prepare), the
// store reports the result to processTransaction() (bound into iap::StoreManager by init()), finishPurchase()
// sends the receipt (purchase/finish). Unfinished transactions are kept in a list and saved with the app state
// (serialize / deserialize), so a purchase interrupted by a crash is finished on the next start.
#pragma once

#include "core/Guest.hpp"

// iap::Purchase: what the store reports for a purchase.
struct IapPurchase {
    guest::String productId;              // +0
    rw::Field<uint8_t> flag;              // +12 copied to Transaction::flag
    const uint8_t pad_0D[3];              // +13
    guest::String orderId;                // +16 (unique per purchase: existsSameTransaction compares it)
    guest::String token;                  // +28
    guest::String purchaseData;           // +40 the signed receipt JSON
    guest::String dataSignature;          // +52
};
RW_LAYOUT(IapPurchase, orderId, 16);
RW_LAYOUT(IapPurchase, dataSignature, 52);

struct PurchaseManager {
    // PurchaseManager::Transaction (held by shared_ptr in the list)
    struct Transaction {
        rw::Field<uint32_t> bridgeTransId;    // +0  server-side id (from purchase/prepare); 0 = not prepared
        guest::String productId;              // +4
        rw::Field<uint8_t> flag;              // +16
        const uint8_t pad_11[3];              // +17
        guest::String orderId;                // +20
        guest::String token;                  // +32
        guest::String purchaseData;           // +44
        guest::String dataSignature;          // +56
        rw::Field<int32_t> state;             // +68 0 = invalid, 1 = purchased, waiting to finish, 2 = finishing
    };

    // std::list<shared_ptr<Transaction>> node
    struct ListNode {
        rw::Field<uint32_t> prev;             // +0
        rw::Field<uint32_t> next;             // +4
        guest::SharedPtr transaction;         // +8
    };

    rw::Field<uint8_t> connecting;            // +0  a purchase request is in progress
    const uint8_t pad_01[3];                  // +1
    // std::list<shared_ptr<Transaction>>: sentinel node (prev, next) and size
    rw::Field<uint32_t> listPrev;             // +4
    rw::Field<uint32_t> listNext;             // +8
    rw::Field<uint32_t> listSize;             // +12
    // std::function<void(bool)> called by finalizeConnection(success)
    const uint8_t callbackBuf[16];            // +16
    rw::Field<uint32_t> callbackFn;           // +32
    const uint8_t pad_24[4];                  // +36
    rw::Field<int32_t> result;                // +40 last iap::Result
    guest::SharedPtr current;                 // +44 transaction being purchased (shared_ptr<Transaction>)
    // std::unordered_map<std::string, iap::ProductInfo> (buckets, bucket count, first node, size, max load)
    rw::Field<uint32_t> productBuckets;       // +52
    rw::Field<uint32_t> productBucketCount;   // +56
    rw::Field<uint32_t> productFirst;         // +60
    rw::Field<uint32_t> productSize;          // +64
    rw::Field<uint32_t> productMaxLoad;       // +68 float bits

    static constexpr uint32_t INSTANCE = 0x10038D8;
    static constexpr uint32_t GUARD = 0x1003920;

    uint32_t addr() const { return rw::guest_addr(this); }
    uint32_t listEnd() const { return addr() + 4; }

    static PurchaseManager *getInstance();
    static PurchaseManager *destruct(PurchaseManager *self);
    static void init(PurchaseManager *self);
    static void deinit(PurchaseManager *self);
    static void processTransaction(PurchaseManager *self, int32_t result, const IapPurchase *purchase);
    static uint32_t hasSuspendedTransaction(const PurchaseManager *self);
    static void getSuspendedTransactionProductIDs(uint32_t out, const PurchaseManager *self);
    static void finishPurchase(PurchaseManager *self, uint32_t node, uint32_t callback, uint32_t resume);
    static void resumeTransaction(PurchaseManager *self, uint32_t callback);
    static void startPurchase(PurchaseManager *self, uint32_t productId, uint32_t callback, uint32_t resume,
                              uint32_t node);
    static void purchase(PurchaseManager *self, uint32_t productId, uint32_t callback);
    static void finalizeConnection(PurchaseManager *self, uint32_t success);
    static void abort(PurchaseManager *self);
    // the request callbacks: startPurchase::$_1 (purchase/prepare), finishPurchase::$_0 (purchase/finish)
    static void onPrepared(uint32_t lambda, uint32_t okp);
    static void onFinished(uint32_t lambda, uint32_t okp);
    static uint32_t findInvalidTransaction(const PurchaseManager *self, const IapPurchase *purchase);
    static uint32_t existsSameTransaction(const PurchaseManager *self, const IapPurchase *purchase);
    static void checkResumedPurchase(PurchaseManager *self, const IapPurchase *purchase);
    static void setProductInfoMap(PurchaseManager *self, uint32_t map);
    static void serialize(uint32_t out, const PurchaseManager *self);
    static void deserialize(PurchaseManager *self, uint32_t data);
};
RW_LAYOUT(PurchaseManager, listPrev, 4);
RW_LAYOUT(PurchaseManager, callbackFn, 32);
RW_LAYOUT(PurchaseManager, result, 40);
RW_LAYOUT(PurchaseManager, current, 44);
RW_LAYOUT(PurchaseManager, productBuckets, 52);
RW_SIZE(PurchaseManager, 72);
RW_LAYOUT(PurchaseManager::Transaction, productId, 4);
RW_LAYOUT(PurchaseManager::Transaction, orderId, 20);
RW_LAYOUT(PurchaseManager::Transaction, purchaseData, 44);
RW_LAYOUT(PurchaseManager::Transaction, state, 68);
RW_SIZE(PurchaseManager::Transaction, 72);
