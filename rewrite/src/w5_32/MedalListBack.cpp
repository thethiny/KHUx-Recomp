// SceneMedalList init $_2 (the sort popup's callback: the list refilled with the filtered, sorted medals) and
// SceneDeckEdit::backScene $_49 (back to MyPage).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t VectorU64_pushSlow = 0x4ECB6D;       // std::vector<u64>::__push_back_slow_path(const u64&)
constexpr uint32_t VectorU64_dtor = 0x468499;           // std::__vector_base<u64>::~__vector_base
constexpr uint32_t IMedalSortFilter_getShowType = 0x66527D;   // (sorter, const shared_ptr<State>&, bool equipped)
constexpr uint32_t MedalList_setMedalShowType = 0x59B2AD;
constexpr uint32_t MedalList_setMedalIDs = 0x59AB19;    // (list, const vector<u64>&)
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5;
constexpr uint32_t Function_vb_assign = 0x490109;       // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;   // (std::function<void(bool)> by value)
constexpr uint32_t SM_changeSceneLayer = 0x612B99;      // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t GOT_LAYER_FACTORY_VT = 0xFD6074;     // &vtable of the __func holding a Layer*(*)()
constexpr uint32_t GOT_MYPAGE_FACTORY = 0xFD6078;       // the MyPage layer factory
constexpr uint32_t VT_MedalListSorted = 0xF6DC10;       // $_2 {scene}
// SceneMedalList
constexpr uint32_t F_SORTER = 0x288, F_FILTERED = 0x298, F_LIST = 0x2B4;
constexpr uint32_t V_getSortFilterState = 0x308;        // (shared_ptr out, scene)
// SceneManager
constexpr uint32_t SM_BACK_TO_MYPAGE = 0x4CC, SM_MYPAGE_CALLBACK = 0x548;
}  // namespace

namespace medal_list_back {

// the medal ids of the filtered view, the show type for the sort key, then the list rebuilt from the top
uint32_t sorted(uint32_t self, uint32_t /*filtered*/) {
    const uint32_t scene = ld32(self + 4);
    Frame f;
    const uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    for (uint32_t i = 0; i < (ld32(scene + F_FILTERED + 4) - ld32(scene + F_FILTERED)) >> 3; i++) {
        const uint32_t medal = ld32(ld32(scene + F_FILTERED) + i * 8);
        const uint32_t end = ld32(ids + 4);
        if (end == ld32(ids + 8)) {
            call(VectorU64_pushSlow, {ids, medal});
        } else {
            if (end) {
                st32(end, ld32(medal));
                st32(end + 4, ld32(medal + 4));
            }
            st32(ids + 4, ld32(ids + 4) + 8);
        }
    }
    const uint32_t state = f.alloc(8);
    vcall(scene, V_getSortFilterState, {state, scene});
    const uint32_t showType = call(IMedalSortFilter_getShowType, {scene + F_SORTER, state, 1});
    sharedRelease(state);
    const uint32_t list = ld32(scene + F_LIST);
    call(MedalList_setMedalShowType, {list, showType});
    call(IScrollProtocol_setIndex, {ld32(scene + F_LIST), 0});
    call(MedalList_setMedalIDs, {ld32(scene + F_LIST), ids});
    call(VectorU64_dtor, {ids});
    return 0;
}
RW_FUNCTION(0x73F110, "SceneMedalList::init::$_2 operator() (sort popup callback)", medal_list_back::sorted);

uint32_t sortedClone(uint32_t self) {
    const uint32_t c = call(fn::operator_new, {8});
    if (c) {
        st32(c, VT_MedalListSorted);
        st32(c + 4, ld32(self + 4));
    }
    return c;
}
RW_FUNCTION(0x73F0B8, "SceneMedalList::init::$_2 __clone()", medal_list_back::sortedClone);

// back to MyPage: the manager told so, its MyPage callback cleared, the MyPage APIs prepared, the layer changed
uint32_t deckEditToMyPage(uint32_t) {
    const uint32_t sm = call(fn::SceneManager_getInstance, {});
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    st8(sm + SM_BACK_TO_MYPAGE, 1);
    call(Function_vb_assign, {sm + SM_MYPAGE_CALLBACK, none});
    functionDestroy(none);
    const uint32_t none2 = newEmptyFunction(f);
    call(SceneUtil_callPrepareMypageAPIs, {none2});
    functionDestroy(none2);
    const uint32_t mgr = call(fn::SceneManager_getInstance, {});
    const uint32_t factory = newFunction(f, ld32(GOT_LAYER_FACTORY_VT) + 8, {ld32(GOT_MYPAGE_FACTORY)});
    call(SM_changeSceneLayer, {mgr, factory, 1, 1, 1});
    functionDestroy(factory);
    return 0;
}
RW_FUNCTION(0x674EE0, "SceneDeckEdit::backScene::$_49 operator() (to MyPage)", medal_list_back::deckEditToMyPage);

}  // namespace medal_list_back
