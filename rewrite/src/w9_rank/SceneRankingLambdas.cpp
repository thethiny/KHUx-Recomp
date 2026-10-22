// SceneRanking / SceneRankingReward leftovers: the two createScene functions (no callers; the scenes are made through
// createSceneLayer) and the lambdas of the ranking flows - back to My Page, the Facebook logout retry, and the chain of
// "ranking ended" reward popups (SceneRanking::openRankingRewardPopup, one popup per RankingReward::Result).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t memcpy = 0x40E164;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;
constexpr uint32_t FunctionVV_copy = 0x4683CD, FunctionVV_move = 0x47F905;
constexpr uint32_t FunctionVB_assign = 0x490109;           // function<void(bool)>::operator=(const function&)
constexpr uint32_t ResultVector_dtor = 0x4AE7F7;           // __vector_base<RankingReward::Result>
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t FacebookManager_getInstance = 0x4E5585, FacebookManager_logout = 0x4E5751;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD, audio_Manager_getLayer = 0x5BF33D;
constexpr uint32_t audio_Layer_playSE = 0x5BF785;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneRanking_create = 0x64A271, SceneRankingReward_create = 0x702405;
constexpr uint32_t SceneRanking_openRankingRewardPopup = 0x6FF349;
constexpr uint32_t ResultVector_copy = 0x700BD1;           // vector<RankingReward::Result>(const vector&)
constexpr uint32_t FadeTo_create = 0x86B3E1;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Scene_create = 0x8A5F37;
}  // namespace g

constexpr uint32_t GOT_FUNC_LAYER_FACTORY_VTABLE = 0xFD6074;   // &_ZTV __func<Layer*(*)(), ..., Layer*()> (vptr + 8)
constexpr uint32_t GOT_CREATE_MYPAGE = 0xFD6078;               // &createSceneLayer<SceneMyPage>
constexpr uint32_t SM_RETURNING = 0x4CC, SM_ON_MYPAGE = 0x548, SM_POPUP_BUSY = 0x308;
constexpr uint32_t VT_LOGOUT = 0xF60EF8;                   // {scene}
constexpr uint32_t VT_CLOSED = 0xF6A174;                   // heap 0x40: {?, a, b, popup, results, index, ?, on end}
constexpr uint32_t SE_REWARD = 0x1E584184;
constexpr uint32_t S_AreaPoint = 0xDF9FB4, S_RewardLwf = 0xDF9FBF;   // "Area_Point", Result_Reward.lwf
constexpr uint32_t V_removeChild = 0x12C;
}  // namespace

struct RankingLambdas {
    static uint32_t rankingCreateScene();
    static uint32_t rewardCreateScene();
    static void backToMyPage(uint32_t fo);
    static void logout(uint32_t fo);
    static void rewardEffect(uint32_t fo);
    static void closeTouched(uint32_t fo, uint32_t sender, uint32_t typeRef);
    static void popupClosed(uint32_t fo);
};

uint32_t RankingLambdas::rankingCreateScene() {
    const uint32_t scene = call(g::Scene_create, {});
    const uint32_t layer = call(g::SceneRanking_create, {});
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}

uint32_t RankingLambdas::rewardCreateScene() {
    const uint32_t scene = call(g::Scene_create, {});
    const uint32_t layer = call(g::SceneRankingReward_create, {});
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}

// back to My Page: the "returning" flag, no My Page callback, the My Page APIs, then the scene change
void RankingLambdas::backToMyPage(uint32_t) {
    Frame f;
    const uint32_t mgr = call(fn::SceneManager_getInstance, {});
    st8(mgr + SM_RETURNING, 1);
    {
        const uint32_t empty = newEmptyFunction(f);
        call(g::FunctionVB_assign, {mgr + SM_ON_MYPAGE, empty});
        functionDestroy(empty);
    }
    {
        const uint32_t empty = newEmptyFunction(f);
        call(g::SceneUtil_callPrepareMypageAPIs, {empty});
        functionDestroy(empty);
    }
    const uint32_t m = call(fn::SceneManager_getInstance, {});
    const uint32_t layer = newFunction(f, ld32(GOT_FUNC_LAYER_FACTORY_VTABLE) + 8, {ld32(GOT_CREATE_MYPAGE)});
    call(g::SceneManager_changeSceneLayer, {m, layer, 1, 1, 1});
    functionDestroy(layer);
}

// {scene}: unless a request waits for a retry, log out of Facebook
void RankingLambdas::logout(uint32_t fo) {
    if (call(g::APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) return;
    const uint32_t scene = ld32(fo + 4);
    const uint32_t mgr = call(g::FacebookManager_getInstance, {});
    Frame f;
    const uint32_t cb = newFunction(f, VT_LOGOUT, {scene});
    call(g::FacebookManager_logout, {mgr, cb});
    functionDestroy(cb);
}

// {popup}: the reward sound and the reward LWF on the popup's Area_Point
void RankingLambdas::rewardEffect(uint32_t fo) {
    Frame f;
    const uint32_t layer = f.alloc(8);
    call(g::audio_Manager_getLayer, {layer, call(g::audio_Manager_getInstance, {}), 0});
    call(g::audio_Layer_playSE, {ld32(layer), SE_REWARD, 0, 0});
    if (ld32(layer + 4)) call(fn::shared_release, {ld32(layer + 4)});
    const uint32_t root = ld32(fo + 4);
    const uint32_t name = f.alloc(12);
    st8(name, 20);
    call(g::memcpy, {name + 1, S_AreaPoint, 10});
    st8(name + 0xB, 0);
    const uint32_t area = SceneUtil::searchNodeByName(root, name);
    strFree(name);
    const uint32_t lwf = call(g::AudioLWFNode_create, {S_RewardLwf, 0});
    vcall(area, node::addChild, {area, lwf});
}

// the popup's close button {?, ?, popup, a, b, results, index, ?, on end}: released, it closes the popup (the closed
// lambda gets a copy of the captures) and fades the shadow out
void RankingLambdas::closeTouched(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    call(g::AudioPlaySystemSEDecide, {});
    st8(call(fn::SceneManager_getInstance, {}) + SM_POPUP_BUSY, 0);
    const uint32_t popup = ld32(fo + 8);
    Frame f;
    const uint32_t caps = f.alloc(0x38);              // {a, b, popup, results (12), index, ?, on end (0x18)}
    st32(caps, ld32(fo + 0xC));
    st32(caps + 4, ld32(fo + 0x10));
    st32(caps + 8, popup);
    call(g::ResultVector_copy, {caps + 0xC, fo + 0x14});
    st32(caps + 0x18, ld32(fo + 0x20));
    call(g::FunctionVV_copy, {caps + 0x20, fo + 0x28});
    const uint32_t closed = newEmptyFunction(f);
    const uint32_t obj = call(fn::operator_new, {0x40});
    if (obj) {
        st32(obj, VT_CLOSED);
        st32(obj + 8, ld32(caps));
        st32(obj + 0xC, ld32(caps + 4));
        st32(obj + 0x10, ld32(caps + 8));
        call(g::ResultVector_copy, {obj + 0x14, caps + 0xC});
        st32(obj + 0x20, ld32(caps + 0x18));
        call(g::FunctionVV_move, {obj + 0x28, caps + 0x20});
        st32(obj + 0x20, ld32(caps + 0x18));
    }
    st32(closed + 16, obj);
    call(g::Node_runAction, {popup, call(g::SceneUtil_createClosePopupAnimation, {popup, closed})});
    functionDestroy(closed);
    functionDestroy(caps + 0x20);
    call(g::ResultVector_dtor, {caps + 0xC});
    const uint32_t shadow = ld32(fo + 0x10);
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), 0})});
}

// the popup closed {?, a, b, popup?, results, index, ?, on end}: both nodes go; the next result's popup, or the end
void RankingLambdas::popupClosed(uint32_t fo) {
    const uint32_t parent = ld32(fo + 8);
    vcall(parent, V_removeChild, {parent, ld32(fo + 0xC), 1});
    vcall(parent, V_removeChild, {parent, ld32(fo + 0x10), 1});
    const uint32_t count = ((ld32(fo + 0x18) - ld32(fo + 0x14)) >> 2) * 0xAAAAAAABu;
    const uint32_t next = ld32(fo + 0x20) + 1;
    if (count > next) {
        Frame f;
        const uint32_t cb = f.alloc(0x18);
        call(g::FunctionVV_copy, {cb, fo + 0x28});
        call(g::SceneRanking_openRankingRewardPopup, {next, cb});
        functionDestroy(cb);
    } else if (ld32(fo + 0x38)) {
        call(fn::function_vv_call, {fo + 0x28});
    }
}

RW_FUNCTION(0x6FBD5C, "SceneRanking::createScene", RankingLambdas::rankingCreateScene);
RW_FUNCTION(0x70E4E6, "SceneRankingReward::createScene", RankingLambdas::rewardCreateScene);
RW_FUNCTION(0x64890C, "SceneRanking back to My Page lambda __func::operator() (sub_64890C)", RankingLambdas::backToMyPage);
RW_FUNCTION(0x648AC0, "SceneRanking Facebook logout lambda __func::operator() (sub_648AC0)", RankingLambdas::logout);
RW_FUNCTION(0x701344, "SceneRanking reward popup effect lambda __func::operator() (sub_701344)",
            RankingLambdas::rewardEffect);
RW_FUNCTION(0x701684, "SceneRanking reward popup close button lambda __func::operator() (sub_701684)",
            RankingLambdas::closeTouched);
RW_FUNCTION(0x701A7C, "SceneRanking reward popup closed lambda __func::operator() (sub_701A7C)",
            RankingLambdas::popupClosed);
