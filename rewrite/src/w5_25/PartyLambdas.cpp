// Party screens: lambdas of ScenePartyTop (the join layout's Search button: run the scene out, then (UlvE_) open
// ScenePartyRequest with an empty user search; callPartyTopAPI's $_4: reload the party (executePartyGet) and its
// std::function slots), ScenePartyRequest (the join layout's button: block taps for the scene change delay, then
// run the out animation; init's Search button: the user or party search with the current filters; the search
// result lambda) and ScenePartyManagement (init's touch filter: the touch is inside the list area below its top
// 31 points).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;   // (anim, function<void()>)
constexpr uint32_t FunctionVB_assign = 0x490109;
constexpr uint32_t ScenePartyRequest_callUserSearchAPI = 0x6D5D1D;   // (LevelRange, int, Frequency, LuxRank, fn)
constexpr uint32_t ScenePartyRequest_callPartySearchAPI = 0x6D5F0D;  // (Frequency, PartyRank, AgreeType, PartyScale, fn)
constexpr uint32_t ScenePartyRequest_searchDone = 0x6D6A61;      // sub_6D6A60(captures): show the search result
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;      // (mgr, float seconds)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_executePartyGet = 0x4C69BD;        // (api, unsigned long long, const function<void(bool)>&)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create, CallFunc_create = rwlib::cocos2d::CallFunc::create, Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation, Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;
constexpr uint32_t Size_copy = rwlib::cocos2d::Size::ctor_base__RKS0_, Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff, Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t GOT_FuncLayerPtr_vt = 0xFD6074;   // &vtable of __func<Layer*(*)(), ..., Layer*()>
constexpr uint32_t GOT_createPartyRequest = 0xFD68D0;   // &createSceneLayer<ScenePartyRequest>
}  // namespace g

constexpr uint32_t TOUCH_ENDED = 2;
constexpr uint32_t SM_FLAG_4CC = 0x4CC, SM_ON_BACK = 0x548, SM_SCENE_CHANGE_DELAY = 0x300;
constexpr uint32_t PARTY_TOP_ANIMATION = 0x288;      // ScenePartyTop's IInOutAnimation
constexpr uint32_t API_LOGGED_IN = 0x14;
// ScenePartyRequest: search mode (byte) and the filters
constexpr uint32_t PR_USER_SEARCH = 0x294;
constexpr uint32_t PR_FREQUENCY = 0x298, PR_PARTY_RANK = 0x2A0, PR_AGREE = 0x2A8, PR_SCALE = 0x2B0;
constexpr uint32_t PR_LUX_RANK = 0x2BC, PR_LEVEL_RANGE = 0x2C0, PR_LEVEL = 0x2C8;

// __func vtables (+8)
constexpr uint32_t VT_TopRunOut = 0xF625D4;          // ScenePartyTop::runInJoinLayout $_11 UlvE_ {}
constexpr uint32_t VT_TopBack = 0xF62600;            // its UlvE_ back handler {}
constexpr uint32_t VT_PartyTopApi4 = 0xF6278C;       // callPartyTopAPI $_4 {.., function<void(bool)> at +0x30}
constexpr uint32_t VT_PartyGet = 0xF627B8;           // $_3 UlbE_ {.., function<void(bool)> at +0x28}
constexpr uint32_t VT_JoinRunOut = 0xF68CD4;         // ScenePartyRequest::runInJoinLayout $_17 UlvE_ {node}
constexpr uint32_t VT_UserSearchDone = 0xF69044;     // init $_85 UlbE_ (the user search answered) {x}
constexpr uint32_t VT_PartySearchDone = 0xF68FEC;    // the party search answered {x}
constexpr uint32_t VT_SearchError = 0xF69070;        // the user search's error dialog lambda {}

uint32_t api() { return call(fn::APIManager_getInstance, {}); }

struct PartyLambdas {
    // ScenePartyTop::runInJoinLayout $_11 (Ref*, TouchEventType) {scene}: Search released
    static void topSearch(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        uint32_t scene = ld32(functor + 4);
        call(g::AudioPlaySystemSEDecide, {});
        Frame f;
        uint32_t done = newFunction(f, VT_TopRunOut, {});
        call(g::IInOutAnimation_runOutAnimation, {scene + PARTY_TOP_ANIMATION, done});
        functionDestroy(done);
    }

    // $_11 UlvE_: to ScenePartyRequest (back key handler, an empty user search, the scene change)
    static void topRunOutDone(uint32_t) {
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t back = newFunction(f, VT_TopBack, {});
        st8(mgr + SM_FLAG_4CC, 1);
        call(g::FunctionVB_assign, {mgr + SM_ON_BACK, back});
        functionDestroy(back);
        uint32_t none = newEmptyFunction(f);
        call(g::ScenePartyRequest_callUserSearchAPI, {0, 0, 0, 0, none});
        functionDestroy(none);
        mgr = sceneManager();
        uint32_t layer = newFunction(f, ld32(g::GOT_FuncLayerPtr_vt) + 8, {ld32(g::GOT_createPartyRequest)});
        call(g::SceneManager_changeSceneLayer, {mgr, layer, 1, 1, 1});
        functionDestroy(layer);
    }

    // callPartyTopAPI $_4 (bool ok): reload the party, the callback in a UlbE_ lambda
    static void partyTopApi(uint32_t functor, uint32_t okRef) {
        if (!ld8(okRef)) return;
        uint32_t a = api();
        Frame f;
        uint32_t copy = f.alloc(20);
        call(fn::function_vb_copy, {copy, functor + 0x30});
        uint32_t cb = newEmptyFunction(f);
        uint32_t heap = call(fn::operator_new, {0x40});
        if (heap) {
            st32(heap, VT_PartyGet);
            call(fn::function_vb_move, {heap + 0x28, copy});
        }
        st32(cb + 16, heap);
        call(g::APIManager_executePartyGet, {a, 0, 0, 0, cb});
        functionDestroy(cb);
        functionDestroy(copy);
    }
    static uint32_t cloneApi4(uint32_t self) {
        uint32_t obj = call(fn::operator_new, {0x48});
        if (obj) {
            st32(obj, VT_PartyTopApi4);
            call(fn::function_vb_copy, {obj + 0x30, self + 0x30});
        }
        return obj;
    }
    static void destroyApi4(uint32_t self) {
        functionDestroy(self + 0x30);
        call(fn::operator_delete, {self});
    }
    static uint32_t clonePartyGet(uint32_t self) {
        uint32_t obj = call(fn::operator_new, {0x40});
        if (obj) {
            st32(obj, VT_PartyGet);
            call(fn::function_vb_copy, {obj + 0x28, self + 0x28});
        }
        return obj;
    }
    static void destroyPartyGet(uint32_t self) {
        functionDestroy(self + 0x28);
        call(fn::operator_delete, {self});
    }

    // ScenePartyRequest::runInJoinLayout $_17 (Ref*, TouchEventType) {node}: block taps for the scene change
    // delay, then (UlvE_) the out animation
    static void joinTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        uint32_t node = ld32(functor + 4);
        call(g::AudioPlaySystemSEDecide, {});
        uint32_t mgr = sceneManager();
        call(g::SceneManager_openNoTapScreen, {mgr, ld32(sceneManager() + SM_SCENE_CHANGE_DELAY)});
        uint32_t delay = call(g::DelayTime_create, {ld32(sceneManager() + SM_SCENE_CHANGE_DELAY)});
        Frame f;
        uint32_t done = newFunction(f, VT_JoinRunOut, {node});
        uint32_t seq = call(g::Sequence_create, {delay, call(g::CallFunc_create, {done}), 0});
        call(fn::Node_runAction, {node, seq});
        functionDestroy(done);
    }

    // ScenePartyRequest::init $_7 (Ref*, TouchEventType) {scene, x}: Search released - users or parties
    static void searchTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        call(g::AudioPlaySystemSEDecide, {});
        uint32_t scene = ld32(functor + 4), x = ld32(functor + 8);
        uint32_t users = ld8(scene + PR_USER_SEARCH);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        Frame f;
        if (users) {
            uint32_t done = newFunction(f, VT_UserSearchDone, {x});
            call(g::ScenePartyRequest_callUserSearchAPI, {ld32(scene + PR_LEVEL_RANGE), ld32(scene + PR_LUX_RANK),
                                                          ld32(scene + PR_FREQUENCY), ld32(scene + PR_LEVEL), done});
            functionDestroy(done);
        } else {
            uint32_t done = newFunction(f, VT_PartySearchDone, {x});
            call(g::ScenePartyRequest_callPartySearchAPI, {ld32(scene + PR_FREQUENCY), ld32(scene + PR_PARTY_RANK),
                                                           ld32(scene + PR_AGREE), ld32(scene + PR_SCALE), done});
            functionDestroy(done);
        }
    }

    // ScenePartyRequest::init $_85 UlbE_ (bool ok) {x}: the user search answered
    static void userSearchDone(uint32_t functor, uint32_t okRef) {
        uint32_t ok = ld8(okRef);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (ok) {
            call(g::ScenePartyRequest_searchDone, {functor + 4});
            return;
        }
        if (!ld8(api() + API_LOGGED_IN)) return;
        uint32_t a = api();
        Frame f;
        uint32_t cb = newFunction(f, VT_SearchError, {});
        call(g::APIManager_openErrorDialog, {a, cb, 1});
        functionDestroy(cb);
    }

    // ScenePartyManagement::init $_6 bool(Touch*, Event*) {node}: the touch is in the node, below its top 31 points
    static uint32_t listTouchBegan(uint32_t functor, uint32_t touchRef) {
        uint32_t node = ld32(functor + 4);
        if (!node) return 0;
        uint32_t touch = ld32(touchRef);
        Frame f;
        uint32_t size = f.alloc(8), rect = f.alloc(16), loc = f.alloc(8), local = f.alloc(8);
        call(g::Size_copy, {size, vcall(node, node::getContentSize, {node})});
        call(g::Rect_ctor, {rect, 0, fbits(31.0f), ld32(size), ld32(size + 4)});
        node = ld32(functor + 4);
        call(g::Touch_getLocation, {loc, touch});
        call(g::Node_convertToNodeSpace, {local, node, loc});
        call(g::Vec2_dtor, {loc});
        uint32_t in = call(g::Rect_containsPoint, {rect, local});
        call(g::Vec2_dtor, {local});
        call(g::Vec2_dtor, {rect});
        return in;
    }
};
}  // namespace

RW_FUNCTION(0x659FE4, "ScenePartyTop::runInJoinLayout $_11 __func::operator() (sub_659FE4)", PartyLambdas::topSearch);
RW_FUNCTION(0x65A114, "ScenePartyTop::runInJoinLayout $_11 UlvE_ __func::operator() (sub_65A114)",
            PartyLambdas::topRunOutDone);
RW_FUNCTION(0x65B120, "ScenePartyTop::callPartyTopAPI $_4 __func::operator()(bool) (sub_65B120)",
            PartyLambdas::partyTopApi);
RW_FUNCTION(0x65B050, "ScenePartyTop::callPartyTopAPI $_4 __func::__clone() (sub_65B050)", PartyLambdas::cloneApi4);
RW_FUNCTION(0x65B0F4, "ScenePartyTop::callPartyTopAPI $_4 __func::destroy_deallocate() (sub_65B0F4)",
            PartyLambdas::destroyApi4);
RW_FUNCTION(0x65B2F4, "ScenePartyTop::callPartyTopAPI $_3 UlbE_ __func::__clone() (sub_65B2F4)",
            PartyLambdas::clonePartyGet);
RW_FUNCTION(0x65B398, "ScenePartyTop::callPartyTopAPI $_3 UlbE_ __func::destroy_deallocate() (sub_65B398)",
            PartyLambdas::destroyPartyGet);
RW_FUNCTION(0x6E4424, "ScenePartyRequest::runInJoinLayout $_17 __func::operator() (sub_6E4424)",
            PartyLambdas::joinTouched);
RW_FUNCTION(0x6E56F0, "ScenePartyRequest::init $_7 __func::operator() (sub_6E56F0)", PartyLambdas::searchTouched);
RW_FUNCTION(0x6E5AB0, "ScenePartyRequest::init $_85 UlbE_ __func::operator()(bool) (sub_6E5AB0)",
            PartyLambdas::userSearchDone);
RW_FUNCTION(0x6F0278, "ScenePartyManagement::init $_6 __func::operator()(Touch*, Event*) (sub_6F0278)",
            PartyLambdas::listTouchBegan);
