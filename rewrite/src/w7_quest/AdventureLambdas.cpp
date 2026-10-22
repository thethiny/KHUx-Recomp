// SceneAdventureSelect / AdventurePanel lambdas: the raid boss reward popup after its BGM change, a node swap with a
// callback, the event info panel (title, banner, boss, sub missions), the tutorial phase, the filter button, the info
// swipe, the colosseum / event entry taps and their out-animations, the event's avatar board.
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1, AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SAS_raidBossRewardPopup = 0x726F11, SAS_initEventBanner = 0x720A5D;
constexpr uint32_t SAS_infoOutAnimation = 0x71FA5D, SAS_checkApColosseum = 0x727A75, SAS_checkApEvent = 0x727859;
constexpr uint32_t AdventurePanel_runInfoInAnimation = 0x72ABDD, CampaignAnimationView_show = 0x598D59;
constexpr uint32_t AdvEventBannerScrollView_loadEventBanner = 0x586A21;
constexpr uint32_t AdvStageScrollView_setEventTitle = 0x56EDBD, AdvStageScrollView_setBossInfo = 0x56A679;
constexpr uint32_t AdvStageScrollView_setSubMisstionInfo = 0x56B301, BaseStageScrollView_isScroll = 0x59DC19;
constexpr uint32_t SceneUtil_CompleteTutorialPhase = 0x472A75, IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t SM_setCanTouchHeader = 0x60E86D, SM_changeSceneLayer = 0x612B99;
constexpr uint32_t function_vv_copy = 0x4683CD, function_vv_call = 0x470E69, ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_SPHERE_BOARD_SELECT = 0xFD68A0;
constexpr uint32_t VT_InfoIn = 0xF6C2F8, VT_TutorialDone = 0xF6C3A8;                 // {scene}
constexpr uint32_t VT_ColosseumOut = 0xF6C694, VT_EventOut = 0xF6C6EC;               // {scene}
constexpr uint32_t S_Title = 0xE06D99, S_RaidInfo = 0xDFF8C6, S_SubMission = 0xE070E0, S_EventBanner = 0xDFF691;
constexpr uint32_t S_RightUI = 0xDF85F7, S_ButtonFilter = 0xE055B1;
constexpr uint32_t V_setVisible = 0xB0, V_removeChild = 0x12C, V_setEnabled = 0x298;
// SceneAdventureSelect: +0x288 the in/out animation base, +0x298 the chosen event (-1 none), +0x29E busy,
// +0x2A4 the layout, +0x2DA info swiped, +0x300 the stage list, +0x325 tap allowed, +0x328 the event index,
// +0x32C the event banner list, +0x370 the header node, +0x38C the info panel, +0x394 the info layout,
// +0x3FC entered flag
constexpr uint32_t F_INOUT = 0x288, F_EVENT = 0x298, F_BUSY = 0x29E, F_ROOT = 0x2A4, F_SWIPED = 0x2DA;
constexpr uint32_t F_STAGE_LIST = 0x300, F_TAP_OK = 0x325, F_EVENT_INDEX = 0x328, F_BANNERS = 0x32C;
constexpr uint32_t F_HEADER = 0x370, F_PANEL = 0x38C, F_INFO = 0x394, F_ENTERED = 0x3FC;
// APIManager +0x12C: the events (56 bytes each: +0xC the avatar board, +0x14 the banner name)
constexpr uint32_t API_EVENTS = 0x12C, EVENT_SIZE = 56;
constexpr uint32_t SCM_FILTER_SEEN = 0x30A, SCM_BOARD_FROM = 0x28C, SCM_BOARD_ID = 0xBC;

// the chosen event's entry (the original throws out_of_range past the end)
uint32_t eventEntry(uint32_t scene, uint32_t &base) {
    const uint32_t events = ld32(call(fn::APIManager_getInstance, {}) + API_EVENTS);
    base = ld32(events);
    const uint32_t index = ld32(scene + F_EVENT_INDEX);
    if (((ld32(events + 4) - base) >> 3) * 0xB6DB6DB7u <= index) call(ThrowOutOfRange, {events});
    return index;
}

void toSphereBoard(uint32_t board) {
    st32(sceneManager() + SCM_BOARD_ID, board);
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t layerFn = newFunction(f, ld32(GOT_FUNC_LAYER_VT) + 8, {ld32(GOT_CREATE_SPHERE_BOARD_SELECT)});
    call(SM_changeSceneLayer, {sm, layerFn, 1, 1, 1});
    functionDestroy(layerFn);
}

// the entry tap's out animation (the header hidden first)
void entryOut(uint32_t scene, uint32_t vt) {
    st8(scene + F_TAP_OK, 0);
    call(AudioPlaySystemSEDecide, {});
    call(SM_setCanTouchHeader, {sceneManager(), 0});
    const uint32_t header = ld32(scene + F_HEADER);
    if (header) vcall(header, V_setVisible, {header, 0});
    st8(scene + F_ENTERED, 0);
    Frame f;
    const uint32_t fo = newFunction(f, vt, {scene});
    call(IInOutAnimation_runOutAnimation, {scene + F_INOUT, fo});
    functionDestroy(fo);
}
}  // namespace

namespace adventure_lambdas {

uint32_t raidRewardAfterBgm(uint32_t functor) {
    const uint32_t scene = ld32(functor + 8);
    call(AudioReplaceGameMainBGM, {501000990, 500});
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vv_copy, {copy, functor + 0x10});
    call(SAS_raidBossRewardPopup, {scene, copy});
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x729E88, "SceneAdventureSelect raid boss reward after BGM operator() (sub_729E88)", adventure_lambdas::raidRewardAfterBgm);

uint32_t swapNodes(uint32_t functor) {
    const uint32_t parent = ld32(functor + 8);
    vcall(parent, V_removeChild, {parent, ld32(functor + 0xC), 1});
    vcall(ld32(functor + 0x10), V_setVisible, {ld32(functor + 0x10), 1});
    const uint32_t fn = ld32(functor + 0x28);
    if (!fn) return 0;
    return call(function_vv_call, {functor + 0x18});
}
RW_FUNCTION(0x72A7F0, "AdventurePanel node swap then callback operator() (sub_72A7F0)", adventure_lambdas::swapNodes);

// the event info: title, banner, raid boss and sub missions, then the info panel slides in
uint32_t eventInfo(uint32_t functor) {
    const uint32_t scene = ld32(functor + 4);
    if (ld32(scene + F_EVENT) == 0xFFFFFFFF) return 0;
    const uint32_t title = findNode(ld32(scene + F_INFO), S_Title, 5);
    const uint32_t raidInfo = findNode(ld32(scene + F_INFO), S_RaidInfo, 8);
    const uint32_t subMission = findNode(ld32(scene + F_INFO), S_SubMission, 10);
    const uint32_t banner = findNode(title, S_EventBanner, 12);
    uint32_t base;
    const uint32_t index = eventEntry(scene, base);
    uint32_t list = ld32(scene + F_BANNERS);
    if (!list) {
        call(SAS_initEventBanner, {scene, 0});
        list = ld32(scene + F_BANNERS);
    }
    const uint32_t current = ld32(scene + F_EVENT_INDEX);
    call(AdvEventBannerScrollView_loadEventBanner, {list, banner, current, strData(base + EVENT_SIZE * index + 0x14)});
    call(AdvStageScrollView_setEventTitle, {title, ld32(scene + F_EVENT)});
    call(AdvStageScrollView_setBossInfo, {raidInfo, ld32(scene + F_EVENT), 1});
    call(AdvStageScrollView_setSubMisstionInfo, {subMission, ld32(scene + F_EVENT), 0});
    {
        Frame f;
        const uint32_t fo = newFunction(f, VT_InfoIn, {scene});
        call(AdventurePanel_runInfoInAnimation, {scene + F_PANEL, fo});
        functionDestroy(fo);
    }
    const uint32_t campaign = ld32(functor + 8);
    if (campaign) call(CampaignAnimationView_show, {campaign});
    return 0;
}
RW_FUNCTION(0x72B148, "SceneAdventureSelect event info operator() (sub_72B148)", adventure_lambdas::eventInfo);

uint32_t completeTutorial(uint32_t functor) {
    Frame f;
    const uint32_t fo = newFunction(f, VT_TutorialDone, {ld32(functor + 4)});
    call(SceneUtil_CompleteTutorialPhase, {fo, 1, 1});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x72BB64, "SceneAdventureSelect tutorial phase operator() (sub_72BB64)", adventure_lambdas::completeTutorial);

// the filter button enabled (and, the first time, the filter tutorial flag)
uint32_t enableFilter(uint32_t functor) {
    const uint32_t scene = ld32(functor + 4);
    if (!ld8(sceneManager() + SCM_FILTER_SEEN)) {
        st8(sceneManager() + SCM_FILTER_SEEN, 1);
        st8(scene + F_ENTERED, 1);
    }
    const uint32_t right = findNode(ld32(scene + F_ROOT), S_RightUI, 7);
    const uint32_t filter = findNode(right, S_ButtonFilter, 13);
    vcall(filter, V_setEnabled, {filter, 1});
    return 0;
}
RW_FUNCTION(0x72BC90, "SceneAdventureSelect filter enabled operator() (sub_72BC90)", adventure_lambdas::enableFilter);

// a swipe of more than 50 points on the info closes it (once)
uint32_t infoSwiped(uint32_t functor, uint32_t deltaRef) {
    const uint32_t scene = ld32(functor + 4);
    const uint32_t flag = ld8(scene + F_SWIPED);
    if (flag) return flag;
    const float d = ldf(deltaRef);
    if (!(d > 50.0f)) {
        const uint32_t keep = (d >= -50.0f || d != d) ? 1 : 0;
        if (keep) return keep;
    }
    st8(scene + F_SWIPED, 1);
    return call(SAS_infoOutAnimation, {scene});
}
RW_FUNCTION(0x72C3B4, "SceneAdventureSelect info swipe operator() (sub_72C3B4)", adventure_lambdas::infoSwiped);

uint32_t colosseumTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    const uint32_t type = ld32(typeRef);
    if (type != 2) return type;
    const uint32_t scene = ld32(functor + 4);
    if (ld8(scene + F_BUSY)) return scene;
    const uint32_t list = ld32(functor + 8);
    if (!list || ld32(list + 0xC) == ld32(list + 8) || ld32(scene + F_EVENT) == 0xFFFFFFFF) return scene;
    if (!ld8(scene + F_TAP_OK)) return scene;
    st8(scene + F_TAP_OK, 0);
    return call(SAS_checkApColosseum, {scene});
}
RW_FUNCTION(0x72D114, "SceneAdventureSelect colosseum entry touch operator() (sub_72D114)", adventure_lambdas::colosseumTouched);

uint32_t colosseumBoard(uint32_t functor) {
    const uint32_t scene = ld32(functor + 4);
    if (!ld8(scene + F_BUSY) && ld8(scene + F_TAP_OK)) entryOut(scene, VT_ColosseumOut);
    return 0;
}
RW_FUNCTION(0x72D1F4, "SceneAdventureSelect colosseum board out operator() (sub_72D1F4)", adventure_lambdas::colosseumBoard);

uint32_t eventBoardFromEvent(uint32_t functor) {
    const uint32_t scene = ld32(functor + 4);
    uint32_t base;
    const uint32_t index = eventEntry(scene, base);
    st32(sceneManager() + SCM_BOARD_FROM, 4);
    toSphereBoard(ld32(base + EVENT_SIZE * index + 0xC));
    return 0;
}
RW_FUNCTION(0x72D378, "SceneAdventureSelect event avatar board (from the event) operator() (sub_72D378)",
            adventure_lambdas::eventBoardFromEvent);

uint32_t eventBoardOut(uint32_t functor) {
    const uint32_t scene = ld32(functor + 4);
    if (!ld8(scene + F_BUSY) && ld8(scene + F_TAP_OK)) entryOut(scene, VT_EventOut);
    return 0;
}
RW_FUNCTION(0x72D50C, "SceneAdventureSelect event board out operator() (sub_72D50C)", adventure_lambdas::eventBoardOut);

uint32_t eventBoard(uint32_t functor) {
    const uint32_t scene = ld32(functor + 4);
    uint32_t base;
    const uint32_t index = eventEntry(scene, base);
    toSphereBoard(ld32(base + EVENT_SIZE * index + 0xC));
    return 0;
}
RW_FUNCTION(0x72D690, "SceneAdventureSelect event avatar board operator() (sub_72D690)", adventure_lambdas::eventBoard);

uint32_t eventTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return functor;
    const uint32_t scene = ld32(functor + 4);
    const uint32_t list = ld32(scene + F_STAGE_LIST);
    if (list) {
        const uint32_t scrolling = call(BaseStageScrollView_isScroll, {list});
        if (scrolling) return scrolling;
    }
    const uint32_t busy = ld8(scene + F_BUSY);
    if (busy) return busy;
    if (!ld8(scene + F_TAP_OK)) return scene + F_TAP_OK;
    st8(scene + F_TAP_OK, 0);
    return call(SAS_checkApEvent, {scene});
}
RW_FUNCTION(0x72D818, "SceneAdventureSelect event entry touch operator() (sub_72D818)", adventure_lambdas::eventTouched);

}  // namespace adventure_lambdas
