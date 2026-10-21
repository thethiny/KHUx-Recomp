// Presents > Receive All: PresentBoxScrollView::popUpAllGetPresent and its chain of result lambdas.
// popUpAllGetPresent sends every listed present id (an unordered_set) with APIManager::executeUserPresentReceive;
// $_6 (the receive result) reloads the box with executeUserPresentGet; $_5 (the reload result) refreshes the
// header and either relists what is left ("some presents could not be received", UlbE0_) or reports everything
// received (UlbE1_, which empties the list); both dialog lambdas then run the scroll view's refresh callback and
// the caller's onDone. Failures open the API error dialog when the API manager asks for it (+0x14).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;   // (mgr, bool)
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t APIManager_executeUserPresentReceive = 0x4CD081;  // (api, const unordered_set<ull>&, fn<void(bool)>)
constexpr uint32_t APIManager_executeUserPresentGet = 0x4CCF3D;      // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;            // (api, const function<void(bool)>&, bool)
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;           // (const string&, function<void(bool)>, const char*)
constexpr uint32_t IScrollProtocol_setItemIDSize = 0x4F6BC9;
constexpr uint32_t IScrollProtocol_resetScroll = 0x4F73ED;
constexpr uint32_t U64Set_insertUnique = 0x482649;    // unordered_set<ull>::__insert_unique (pair out, set, const ull&)
constexpr uint32_t U64Set_dtor = 0x482619;            // ~__hash_table
constexpr uint32_t U64Vector_pushBackSlow = 0x4ECB6D; // vector<ull>::__push_back_slow_path(const ull&)
}  // namespace g

// PresentBoxScrollView: BaseScrollView<unsigned long long>'s item ids and the refresh callback
constexpr uint32_t F_IDS_BEGIN = 0x628, F_IDS_END = 0x62C, F_IDS_CAP = 0x630;
constexpr uint32_t F_ON_REFRESH = 0x698;      // std::function<void()>
constexpr uint32_t APIMANAGER_PRESENTS = 0x104;   // map<ull, UserPresent::Present>*
constexpr uint32_t APIMANAGER_SHOW_ERROR = 0x14;  // byte: open the error dialog on failure

constexpr int32_t TXT_SOME_LEFT = 103500008;
constexpr int32_t TXT_ALL_RECEIVED = 103500012;

// __func vtables (+8). The lambdas below keep {this at +8, onDone (std::function<void()>) at +0x10}; 0x28 bytes.
constexpr uint32_t VT_Received = 0xF56260;        // $_6
constexpr uint32_t VT_Reloaded = 0xF5628C;        // $_5
constexpr uint32_t VT_AllReceived = 0xF562B8;     // $_5 UlbE1_
constexpr uint32_t VT_SomeLeft = 0xF562E4;        // $_5 UlbE0_
constexpr uint32_t VT_ReloadError = 0xF56310;     // $_5 UlbE_ (empty)
constexpr uint32_t VT_ReceiveError = 0xF5633C;    // $_6 UlbE_ (empty)
constexpr uint32_t LAMBDA_SIZE = 0x28, LAMBDA_THIS = 8, LAMBDA_FN = 0x10;

uint32_t api() { return call(fn::APIManager_getInstance, {}); }

void loadingIndicator(bool on) { call(g::SceneManager_enableLoadingIndicator, {sceneManager(), on ? 1u : 0u}); }

// std::function `dst` = heap lambda {self, onDone moved from `fnTmp`}
void heapLambda(uint32_t dst, uint32_t vtable, uint32_t self, uint32_t fnTmp) {
    uint32_t heap = heapFunction(dst, vtable, LAMBDA_SIZE, LAMBDA_FN, fn::function_vv_move, fnTmp);
    if (heap) st32(heap + LAMBDA_THIS, self);
}

// the API error dialog with an empty-capture lambda, when the API manager asks for one
void errorDialog(uint32_t vtable) {
    if (!ld8(api() + APIMANAGER_SHOW_ERROR)) return;
    uint32_t a = api();
    Frame f;
    uint32_t cb = newFunction(f, vtable, {});
    call(g::APIManager_openErrorDialog, {a, cb, 1});
    functionDestroy(cb);
}

// vector<ull>::clear() as compiled: end moves back by the rounded byte count
void clearIds(uint32_t self) {
    uint32_t end = ld32(self + F_IDS_END), begin = ld32(self + F_IDS_BEGIN);
    if (end != begin) st32(self + F_IDS_END, end + (~(end - 8 - begin) & ~7u));
}

void listIdsChanged(uint32_t self) {
    call(g::IScrollProtocol_setItemIDSize, {self, (ld32(self + F_IDS_END) - ld32(self + F_IDS_BEGIN)) >> 3});
    call(g::IScrollProtocol_resetScroll, {self});
}

// the scroll view's refresh callback, then onDone (the lambda's)
void refreshAndDone(uint32_t self, uint32_t functor) {
    if (ld32(self + F_ON_REFRESH + 16)) call(fn::function_vv_call, {self + F_ON_REFRESH});
    if (ld32(functor + LAMBDA_FN + 16)) call(fn::function_vv_call, {functor + LAMBDA_FN});
}

// SceneUtil::openOkDialog4Line(text `textId`, heap lambda {self, copy of onDone})
void okDialog(int32_t textId, uint32_t vtable, uint32_t self, uint32_t onDone) {
    Frame f;
    uint32_t sp = textData(f, textId);
    uint32_t text = newStr(f, textDataStr(sp));
    uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, onDone});
    uint32_t cb = f.alloc(20);
    heapLambda(cb, vtable, self, copy);
    call(g::SceneUtil_openOkDialog4Line, {text, cb, 0});
    functionDestroy(cb);
    functionDestroy(copy);
    strFree(text);
    sharedRelease(sp);
}

struct PresentReceiveAll {
    static void popUpAllGetPresent(uint32_t self, uint32_t onDone) {
        if (ld32(self + F_IDS_END) == ld32(self + F_IDS_BEGIN)) return;
        Frame f;
        uint32_t arg = f.alloc(20);
        call(fn::function_vv_copy, {arg, onDone});
        uint32_t captured = f.alloc(20);            // the lambda's copy of onDone
        functionCopy(captured, arg);
        uint32_t ids = f.alloc(20);                 // unordered_set<ull>: buckets, count, first, size, 1.0f
        for (uint32_t k = 0; k < 16; k += 4) st32(ids + k, 0);
        stf(ids + 16, 1.0f);
        uint32_t inserted = f.alloc(8);
        for (uint32_t i = 0; i < (ld32(self + F_IDS_END) - ld32(self + F_IDS_BEGIN)) >> 3; ++i)
            call(g::U64Set_insertUnique, {inserted, ids, ld32(self + F_IDS_BEGIN) + 8 * i});
        loadingIndicator(true);
        uint32_t a = api();
        uint32_t lambda = f.alloc(20);              // the lambda object's onDone
        functionCopy(lambda, captured);
        uint32_t cb = f.alloc(20);
        heapLambda(cb, VT_Received, self, lambda);
        call(g::APIManager_executeUserPresentReceive, {a, ids, cb});
        functionDestroy(cb);
        functionDestroy(lambda);
        call(g::U64Set_dtor, {ids});
        functionDestroy(captured);
        functionDestroy(arg);
    }

    // $_6 (bool ok): reload the box
    static void received(uint32_t functor, uint32_t okRef) {
        if (!ld8(okRef)) {
            loadingIndicator(false);
            errorDialog(VT_ReceiveError);
            return;
        }
        uint32_t a = api();
        Frame f;
        uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, functor + LAMBDA_FN});
        uint32_t cb = f.alloc(20);
        heapLambda(cb, VT_Reloaded, ld32(functor + LAMBDA_THIS), copy);
        call(g::APIManager_executeUserPresentGet, {a, cb});
        functionDestroy(cb);
        functionDestroy(copy);
    }

    // $_5 (bool ok): the reloaded box
    static void reloaded(uint32_t functor, uint32_t okRef) {
        bool ok = ld8(okRef) != 0;
        uint32_t self = ld32(functor + LAMBDA_THIS);
        loadingIndicator(false);
        if (!ok) {
            errorDialog(VT_ReloadError);
            return;
        }
        call(g::SceneManager_updateHeader, {sceneManager()});
        uint32_t presents = api() + APIMANAGER_PRESENTS;
        if (!ld32(ld32(presents) + 8)) {
            okDialog(TXT_ALL_RECEIVED, VT_AllReceived, self, functor + LAMBDA_FN);
            return;
        }
        // relist the presents still in the box (map<ull, Present> in key order)
        clearIds(self);
        uint32_t map = ld32(presents);
        for (uint32_t node = ld32(map); node != map + 4;) {
            uint32_t end = ld32(self + F_IDS_END);
            if (end == ld32(self + F_IDS_CAP)) {
                call(g::U64Vector_pushBackSlow, {self + F_IDS_BEGIN, node + 16});
            } else {
                uint32_t next = 0;
                if (end) {
                    st32(end, ld32(node + 16));
                    st32(end + 4, ld32(node + 20));
                    next = ld32(self + F_IDS_END);
                }
                st32(self + F_IDS_END, next + 8);
            }
            uint32_t right = ld32(node + 4);
            if (right) {
                for (node = right; ld32(node); node = ld32(node)) {}
            } else {
                uint32_t child;
                do {
                    child = node;
                    node = ld32(node + 8);
                } while (ld32(node) != child);
            }
            if (node == ld32(presents) + 4) break;
        }
        listIdsChanged(self);
        okDialog(TXT_SOME_LEFT, VT_SomeLeft, self, functor + LAMBDA_FN);
    }

    // $_5 UlbE1_ (bool): everything received - empty the list
    static void allReceivedClosed(uint32_t functor, uint32_t okRef) {
        if (!ld8(okRef)) return;
        uint32_t self = ld32(functor + LAMBDA_THIS);
        clearIds(self);
        listIdsChanged(self);
        refreshAndDone(self, functor);
    }

    // $_5 UlbE0_ (bool): some presents left
    static void someLeftClosed(uint32_t functor, uint32_t okRef) {
        if (!ld8(okRef)) return;
        refreshAndDone(ld32(functor + LAMBDA_THIS), functor);
    }
};
}  // namespace

RW_FUNCTION(0x50E3C4, "PresentBoxScrollView::popUpAllGetPresent", PresentReceiveAll::popUpAllGetPresent);
RW_FUNCTION(0x50E8D8, "PresentBoxScrollView::popUpAllGetPresent $_6 __func::operator()(bool) (sub_50E8D8)",
            PresentReceiveAll::received);
RW_FUNCTION(0x50EBFC, "PresentBoxScrollView::popUpAllGetPresent $_5 __func::operator()(bool) (sub_50EBFC)",
            PresentReceiveAll::reloaded);
RW_FUNCTION(0x50F1D4, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE1_ __func::operator()(bool) (sub_50F1D4)",
            PresentReceiveAll::allReceivedClosed);
RW_FUNCTION(0x50F3E0, "PresentBoxScrollView::popUpAllGetPresent $_5 UlbE0_ __func::operator()(bool) (sub_50F3E0)",
            PresentReceiveAll::someLeftClosed);
