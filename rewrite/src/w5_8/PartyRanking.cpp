// Party > Rankings: ScenePartyTop's ranking button (sub_659170 slides the party top out, sub_6592A0 asks for the
// party ranking and changes to the Rankings screen) and SceneRanking::callPartyRankingAPI's request chain: the
// ranking reward check, then the party ranking (sub_702F4C), then the user's own entry (sub_7031EC), with their
// lambdas' clone / destroy_deallocate.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t FunctionVB_copy = 0x45C84D;             // std::function<void(bool)>(const function&)
constexpr uint32_t FunctionVB_move = 0x47BC69;             // std::function<void(bool)>(function&&)
constexpr uint32_t FunctionVB_call = 0x474139;             // std::function<void(bool)>::operator()(bool)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneRanking_callPartyRankingAPI = 0x6FBA65;
constexpr uint32_t APIManager_executeRankingRewardCheck = 0x4D32C5;   // (api, RankingRewardType, const function&)
constexpr uint32_t APIManager_executeRankingParty = 0x4D2C71;
constexpr uint32_t APIManager_executeRankingPartyOwn = 0x4D2DB5;
}  // namespace g

constexpr uint32_t VT_RankingOut = 0xF6241C;     // ScenePartyTop ranking button: run-out done {}: sub_6592A0
constexpr uint32_t VT_RankingScene = 0xF62448;   // sub_6592A0's Layer*() factory {}
constexpr uint32_t VT_RewardChecked = 0xF6A434;  // {function<void(bool)>} (heap 0x20): sub_702F4C
constexpr uint32_t VT_PartyRanking = 0xF6A460;   // {function<void(bool)>} (heap 0x20): sub_7031EC
constexpr uint32_t VT_PartyOwn = 0xF6A48C;       // {function<void(bool)>} (heap 0x20)
constexpr uint32_t SM_SELECT_RANKING = 0x290, SM_FLAG_4CC = 0x4CC, SM_CALLBACK_548 = 0x548;
constexpr uint32_t RANKING_REWARD_PARTY = 2;
}  // namespace

struct PartyRanking {
    static void rankingTouched(uint32_t functor, uint32_t sender, uint32_t type);
    static void rankingOutDone(uint32_t functor);
    static void callPartyRankingAPI(uint32_t done);
    static void rewardChecked(uint32_t functor, uint32_t okPtr);
    static void partyRankingDone(uint32_t functor, uint32_t okPtr);
    static uint32_t clonePartyOwn(uint32_t functor);
    static void destroyLambda(uint32_t functor);
};

// ScenePartyTop's ranking button (touch ended): decide sound, slide the party top out.
void PartyRanking::rankingTouched(uint32_t functor, uint32_t, uint32_t type) {
    if (ld32(type) != 2) return;
    const uint32_t top = ld32(functor + 4);
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_RankingOut, {});
    call(g::IInOutAnimation_runOutAnimation, {top + 0x288, fo});
    functionDestroy(fo);
}

// ...then the party ranking request and the Rankings screen.
void PartyRanking::rankingOutDone(uint32_t) {
    Frame f;
    {
        const uint32_t m = sceneManager();
        const uint32_t none = newEmptyFunction(f);
        st8(m + SM_FLAG_4CC, 1);
        call(g::FunctionVB_assign, {m + SM_CALLBACK_548, none});
        functionDestroy(none);
    }
    {
        const uint32_t none = newEmptyFunction(f);
        call(g::SceneRanking_callPartyRankingAPI, {none});
        functionDestroy(none);
    }
    const uint32_t m = sceneManager();
    const uint32_t layer = newFunction(f, VT_RankingScene, {});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 0, 0});
    functionDestroy(layer);
}

// SceneRanking::callPartyRankingAPI(done): select the party ranking; the reward check, then (its lambda) the ranking
void PartyRanking::callPartyRankingAPI(uint32_t done) {
    Frame f;
    const uint32_t a = f.alloc(20), b = f.alloc(20), fo = f.alloc(20);
    call(g::FunctionVB_copy, {a, done});
    st32(sceneManager() + SM_SELECT_RANKING, 1);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    functionCopy(b, a);
    heapFunction(fo, VT_RewardChecked, 0x20, 8, g::FunctionVB_move, b);
    rw::call(g::APIManager_executeRankingRewardCheck, {api, RANKING_REWARD_PARTY, fo});
    functionDestroy(fo);
    functionDestroy(b);
    functionDestroy(a);
}

// a request lambda {done}: on success the next request with {done}; on failure done(false)
static void nextRequest(uint32_t functor, uint32_t okPtr, uint32_t vtable, uint32_t request) {
    const uint32_t done = functor + 8;
    if (ld8(okPtr)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t copy = f.alloc(20), fo = f.alloc(20);
        call(g::FunctionVB_copy, {copy, done});
        heapFunction(fo, vtable, 0x20, 8, g::FunctionVB_move, copy);
        call(request, {api, fo});
        functionDestroy(fo);
        functionDestroy(copy);
    } else if (ld32(done + 0x10)) {
        call(g::FunctionVB_call, {done, 0});
    }
}
void PartyRanking::rewardChecked(uint32_t functor, uint32_t okPtr) {
    nextRequest(functor, okPtr, VT_PartyRanking, g::APIManager_executeRankingParty);
}
void PartyRanking::partyRankingDone(uint32_t functor, uint32_t okPtr) {
    nextRequest(functor, okPtr, VT_PartyOwn, g::APIManager_executeRankingPartyOwn);
}

// __func::__clone() of the own-entry lambda; __func::destroy_deallocate() of two request lambdas
uint32_t PartyRanking::clonePartyOwn(uint32_t functor) {
    const uint32_t obj = call(fn::operator_new, {0x20});
    if (obj) {
        st32(obj, VT_PartyOwn);
        call(g::FunctionVB_copy, {obj + 8, functor + 8});
    }
    return obj;
}
void PartyRanking::destroyLambda(uint32_t functor) {
    functionDestroy(functor + 8);
    call(fn::operator_delete, {functor});
}

RW_FUNCTION(0x659170, "ScenePartyTop ranking button lambda __func::operator() (sub_659170)", PartyRanking::rankingTouched);
RW_FUNCTION(0x6592A0, "ScenePartyTop ranking run-out lambda __func::operator() (sub_6592A0)", PartyRanking::rankingOutDone);
RW_FUNCTION(0x6FBA64, "SceneRanking::callPartyRankingAPI", PartyRanking::callPartyRankingAPI);
RW_FUNCTION(0x702F4C, "SceneRanking::callPartyRankingAPI reward-check lambda __func::operator() (sub_702F4C)",
            PartyRanking::rewardChecked);
RW_FUNCTION(0x7031EC, "SceneRanking party ranking lambda __func::operator() (sub_7031EC)", PartyRanking::partyRankingDone);
RW_FUNCTION(0x7033BC, "SceneRanking party own lambda __func::__clone() (sub_7033BC)", PartyRanking::clonePartyOwn);
RW_FUNCTION(0x7031C0, "SceneRanking reward-check lambda __func::destroy_deallocate() (sub_7031C0)",
            PartyRanking::destroyLambda);
RW_FUNCTION(0x703460, "SceneRanking party own lambda __func::destroy_deallocate() (sub_703460)", PartyRanking::destroyLambda);
