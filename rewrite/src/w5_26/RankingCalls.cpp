// The ranking screen's requests: SceneRanking::callCurrentRankingAPI picks the request of the selected ranking
// (SceneManager +0x290: 4 Lux, 2 Colosseum, 1 party; anything else calls back at once), callLuxRankingAPI first asks
// for the ranking rewards (executeRankingRewardCheck 4) and its lambdas chain the Lux ranking and the player's own
// Lux rank ($_1, then $_1's lambda); init $_14's lambda shows the list or the error dialog. Also the std::function
// plumbing of those lambdas and of RankingScrollView::initPartyLayer $_0 (a row's touch: a copy of the row's
// UserRanking record).
#include "../scene/SceneUi.hpp"
#include "../scene_main/FunctionThunks.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t function_vb_copy = 0x45C84D;            // std::function<void(bool)>(const&)
constexpr uint32_t function_vb_move = 0x47BC69;            // std::function<void(bool)>(&&)
constexpr uint32_t function_vb_call = 0x474139;            // std::function<void(bool)>::operator()(bool)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;   // (manager, bool)
constexpr uint32_t APIManager_executeRankingRewardCheck = 0x4D32C5;  // (api, RankingRewardType, const function&)
constexpr uint32_t APIManager_executeRankingLux = 0x4D29E9;          // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_executeRankingLuxOwn = 0x4D2B2D;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;            // (api, const function<void(bool)>&, bool)
constexpr uint32_t SceneRanking_callLuxRankingAPI = 0x6FB8E9;
constexpr uint32_t SceneRanking_callPartyRankingAPI = 0x6FBA65;
constexpr uint32_t SceneRanking_callColosseumRankingAPI = 0x6FBBE1;
constexpr uint32_t SceneRanking_changeScrollView = 0x6FE0E5;        // (this, bool)
constexpr uint32_t UserRanking_copy = 0x4B2E63;                     // the row record's copy constructor
constexpr uint32_t VectorULL_dtor = 0x468499;
}  // namespace g

constexpr uint32_t SM_SELECT_RANKING = 0x290;              // SceneManager: the ranking shown (SelectRanking)
constexpr uint32_t RANKING_LUX = 4, RANKING_COLOSSEUM = 2, RANKING_PARTY = 1;
constexpr uint32_t REWARD_CHECK_LUX = 4;
constexpr uint32_t API_OFFLINE = 0x14;                     // APIManager: a request failed (byte)
constexpr uint32_t VT_RewardChecked = 0xF6A4B8;            // callLuxRankingAPI $_1 {function<void(bool)> at +8}
constexpr uint32_t VT_LuxLoaded = 0xF6A4E4;                // $_1's lambda(bool) {function at +8}
constexpr uint32_t VT_OwnLoaded = 0xF6A510;                // that lambda's lambda(bool) {function at +8}
constexpr uint32_t VT_ErrorClosed = 0xF6A594;              // init $_14's lambda's error dialog callback

// a heap __func `vt` (0x20 bytes) holding, at +8, a std::function<void(bool)> moved from a copy of `fn`
// (the copy made inline: clone into the stack temporary first, as libc++ does for a lambda's by-value capture)
uint32_t newWrapped(Frame &f, uint32_t vt, uint32_t fn) {
    uint32_t tmp = f.alloc(20);
    functionCopy(tmp, fn);
    uint32_t cb = newEmptyFunction(f);
    uint32_t heap = call(fn::operator_new, {0x20});
    if (heap) {
        st32(heap, vt);
        call(g::function_vb_move, {heap + 8, tmp});
    }
    st32(cb + 16, heap);
    return cb;
}

struct RankingCalls {
    // SceneRanking::callLuxRankingAPI(function<void(bool)> done)
    static void callLux(uint32_t done) {
        Frame f;
        uint32_t copy = f.alloc(20);
        call(g::function_vb_copy, {copy, done});
        st32(sceneManager() + SM_SELECT_RANKING, RANKING_LUX);
        uint32_t api = call(fn::APIManager_getInstance, {});
        {
            Frame t;
            uint32_t tmp = t.alloc(20);
            functionCopy(tmp, copy);
            uint32_t cb = newEmptyFunction(t);
            uint32_t heap = call(fn::operator_new, {0x20});
            if (heap) {
                st32(heap, VT_RewardChecked);
                call(g::function_vb_move, {heap + 8, tmp});
            }
            st32(cb + 16, heap);
            call(g::APIManager_executeRankingRewardCheck, {api, REWARD_CHECK_LUX, cb});
            functionDestroy(cb);
            functionDestroy(tmp);
        }
        functionDestroy(copy);
    }

    // SceneRanking::callCurrentRankingAPI(function<void(bool)> done)
    static void callCurrent(uint32_t /*self*/, uint32_t done) {
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t which = ld32(sceneManager() + SM_SELECT_RANKING);
        uint32_t request = which == RANKING_LUX ? g::SceneRanking_callLuxRankingAPI
                         : which == RANKING_COLOSSEUM ? g::SceneRanking_callColosseumRankingAPI
                         : which == RANKING_PARTY ? g::SceneRanking_callPartyRankingAPI : 0;
        if (!request) {
            if (ld32(done + 16)) call(g::function_vb_call, {done, 1});
            return;
        }
        Frame f;
        uint32_t copy = f.alloc(20);
        call(g::function_vb_copy, {copy, done});
        call(request, {copy});
        functionDestroy(copy);
    }

    // $_1 / $_1's lambda (bool ok) {done}: on success the next request, wrapping `done` again; else done(false)
    static void chain(uint32_t functor, uint32_t okRef, uint32_t nextVt, uint32_t request) {
        const uint32_t done = functor + 8;
        if (!ld8(okRef)) {
            if (ld32(done + 16)) call(g::function_vb_call, {done, 0});
            return;
        }
        uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        uint32_t copy = f.alloc(20);
        call(g::function_vb_copy, {copy, done});
        uint32_t cb = newEmptyFunction(f);
        uint32_t heap = call(fn::operator_new, {0x20});
        if (heap) {
            st32(heap, nextVt);
            call(g::function_vb_move, {heap + 8, copy});
        }
        st32(cb + 16, heap);
        call(request, {api, cb});
        functionDestroy(cb);
        functionDestroy(copy);
    }
    static void rewardChecked(uint32_t functor, uint32_t okRef) {
        chain(functor, okRef, VT_LuxLoaded, g::APIManager_executeRankingLux);
    }
    static void luxLoaded(uint32_t functor, uint32_t okRef) {
        chain(functor, okRef, VT_OwnLoaded, g::APIManager_executeRankingLuxOwn);
    }

    // init $_14's lambda (bool ok) {scene, animate}: the list, or the error dialog when a request failed
    static void rankingLoaded(uint32_t functor, uint32_t okRef) {
        const uint32_t ok = ld8(okRef);
        const uint32_t scene = ld32(functor + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (ok) {
            call(g::SceneRanking_changeScrollView, {scene, ld8(functor + 8)});
            return;
        }
        uint32_t api = call(fn::APIManager_getInstance, {});
        if (!ld8(api + API_OFFLINE)) return;
        api = call(fn::APIManager_getInstance, {});
        Frame f;
        uint32_t cb = newFunction(f, VT_ErrorClosed, {});
        call(g::APIManager_openErrorDialog, {api, cb, 1});
        functionDestroy(cb);
    }

    // RankingScrollView::initPartyLayer $_0: {+8 two words, +0x10 UserRanking (copy constructor), +0x80 two words}
    static uint32_t partyRowClone(uint32_t self) {
        uint32_t obj = call(fn::operator_new, {0x88});
        if (obj) {
            st32(obj, 0xF58AC4);
            const uint32_t a = ld32(self + 8), b = ld32(self + 0xC);
            st32(obj + 8, a);
            st32(obj + 0xC, b);
            call(g::UserRanking_copy, {obj + 0x10, self + 0x10});
            const uint32_t c = ld32(self + 0x80), d = ld32(self + 0x84);
            st32(obj + 0x84, d);
            st32(obj + 0x80, c);
        }
        return obj;
    }
    // destroy_deallocate: the record's strings (+0x70, +0x34, +0x20) and its vector (+0x50), then the object
    static void partyRowDelete(uint32_t self) {
        strFree(self + 0x70);
        call(g::VectorULL_dtor, {self + 0x50});
        strFree(self + 0x34);
        strFree(self + 0x20);
        call(fn::operator_delete, {self});
    }
};

using LuxRewardChecked = thunk::LambdaFn<VT_RewardChecked, 0x20, 0xFA3314, 0xE8BD30, 8, 8, 0x45C84D>;
using LuxLoaded = thunk::LambdaFn<VT_LuxLoaded, 0x20, 0xFA3328, 0xE8BE00, 8, 8, 0x45C84D>;
using LuxOwnLoaded = thunk::LambdaFn<VT_OwnLoaded, 0x20, 0xFA333C, 0xE8BEE0, 8, 8, 0x45C84D>;
using RankingLoaded = thunk::Lambda<0xF6A568, 0xC, 0xFA3364, 0xE8C070, 4, thunk::cap(4, 4), thunk::cap(8, 4)>;
}  // namespace

RW_FUNCTION(0x6FB8E8, "SceneRanking::callLuxRankingAPI(function<void(bool)>)", RankingCalls::callLux);
RW_FUNCTION(0x700FF8, "SceneRanking::callCurrentRankingAPI(function<void(bool)>)", RankingCalls::callCurrent);
RW_FUNCTION(0x703550, "SceneRanking::callLuxRankingAPI::$_1 __clone()", LuxRewardChecked::clone);
RW_FUNCTION(0x7035F4, "SceneRanking::callLuxRankingAPI::$_1 destroy_deallocate()", LuxRewardChecked::destroyDeallocate);
RW_FUNCTION(0x703620, "SceneRanking::callLuxRankingAPI::$_1 operator()", RankingCalls::rewardChecked);
RW_FUNCTION(0x7037F0, "SceneRanking::callLuxRankingAPI::$_1::lambda(bool) __clone()", LuxLoaded::clone);
RW_FUNCTION(0x703894, "SceneRanking::callLuxRankingAPI::$_1::lambda(bool) destroy_deallocate()", LuxLoaded::destroyDeallocate);
RW_FUNCTION(0x7038C0, "SceneRanking::callLuxRankingAPI::$_1::lambda(bool) operator()", RankingCalls::luxLoaded);
RW_FUNCTION(0x703A90, "SceneRanking::callLuxRankingAPI::$_1::lambda(bool)::lambda(bool) __clone()", LuxOwnLoaded::clone);
RW_FUNCTION(0x703B34, "SceneRanking::callLuxRankingAPI::$_1::lambda(bool)::lambda(bool) destroy_deallocate()",
            LuxOwnLoaded::destroyDeallocate);
RW_FUNCTION(0x703C80, "SceneRanking::init::$_14::lambda(bool) __clone(__base*)", RankingLoaded::cloneInto);
RW_FUNCTION(0x703CB0, "SceneRanking::init::$_14::lambda(bool) operator()", RankingCalls::rankingLoaded);
RW_FUNCTION(0x5531C4, "RankingScrollView::initPartyLayer::$_0 __clone()", RankingCalls::partyRowClone);
RW_FUNCTION(0x5532B2, "RankingScrollView::initPartyLayer::$_0 destroy_deallocate()", RankingCalls::partyRowDelete);
