// SceneAdventureSelect: lambda bodies and std::function slots that run in game but were left after batch 2 (batch 3:
// named from their __func vtables' target_type). `functor` is the __func object: vtable at +0, then the captures.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"
#include "../core/SafeBuffer.hpp"

using namespace scene;
using namespace sas;

namespace {

constexpr uint32_t BaseStageScrollView_isScroll = 0x59DC19;
constexpr uint32_t BaseStageScrollView_setEnabledScroll = 0x59D63D;   // (view, bool)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_getTutorialCloseFunc = 0x4EBA3D;          // (std::function<void()>* out, app, id)
constexpr uint32_t AppState_execTutorialCloseFunc = 0x4EBA5D;         // (app, id)
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D;         // (mgr, bool)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;    // (mgr, bool)
constexpr uint32_t SceneManager_updateHeaderRaidAlertsBtn = 0x6131F9;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t APIManager_executeRaidList = 0x4CC009;             // (api, const std::function<void(bool)>&)
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;             // (api, const std::function<void(bool)>&, bool)
constexpr uint32_t API_REQUEST_FAILED = 0x14;                         // APIManager byte: the last request failed
constexpr uint32_t SAS_changeOutAnimation = 0x71F00D;                 // (scene, std::function<void()>)
constexpr uint32_t SAS_changeAdventure = 0x71FC3D;                    // (scene, AdventureType, bool)
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;                   // ui::Widget::setTouchEnabled(bool)

// SceneAdventureSelect fields
constexpr uint32_t ADVENTURE_TYPE = 0x28C;
constexpr uint32_t BUSY_29E = 0x29E;
constexpr uint32_t FLAG_2E4 = 0x2E4;
constexpr uint32_t STAGE_SCROLL_VIEW = 0x300;
constexpr uint32_t TOUCH_READY = 0x325;            // a quest button may be pressed
constexpr uint32_t FLAG_3FC = 0x3FC;
constexpr int32_t ADVENTURE_SELECT = 7;
constexpr uint32_t TOUCH_ENDED = 2;
constexpr uint32_t TUTORIAL_STORY_QUEST = 0x68;

// initAdvSelect $_13 [this, quest1..4] -> {lambda(bool)#1} [this, quest1..4] -> {lambda()#1} [quest1..4, this];
// the retry dialog's callback (no captures)
constexpr uint32_t VT_raidList_done = 0xF6CDCC;
constexpr uint32_t VT_raidList_out = 0xF6CDF8;
constexpr uint32_t VT_raidList_retry = 0xF6CE24;

// callAdventureSelectAPI $_0 [callback]: raid list -> {lambda(bool)#1} [callback]: party -> {lambda(bool)#1}
// [callback]: party member list (each __func: vtable, pad, std::function<void(bool)> at +0x10; 0x28 bytes)
constexpr uint32_t VT_selectApi_party = 0xF6D0E4;
constexpr uint32_t VT_selectApi_members = 0xF6D110;
constexpr uint32_t VT_selectApi_done = 0xF6D13C;      // then raid rewards -> stage -> raid -> the last step
constexpr uint32_t VT_selectApi_stage = 0xF6D168, VT_selectApi_raid = 0xF6D194, VT_selectApi_last = 0xF6D1C0;
constexpr uint32_t APIManager_executeRaidRewardGet151101 = 0x4CC291;
constexpr uint32_t APIManager_executeStageGet160310 = 0x4CADB9;
constexpr uint32_t APIManager_executeRaidGet = 0x4CBEC5;
constexpr uint32_t APIManager_executeStageEventGet = 0x4CAEFD;    // (api, EventCategory, const function<void(bool)>&)
constexpr uint32_t VT_selectApi_event1 = 0xF6D270, VT_selectApi_event2 = 0xF6D244, VT_selectApi_friends = 0xF6D1EC;
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t FacebookManager_getInstance = 0x4E5585;
constexpr uint32_t FacebookManager_getFriends = 0x4E575B;         // (fb, limit, const std::function<...>&)
constexpr uint32_t Misc_create = 0x786D5D;                        // master::Misc::create(shared_ptr out, id)
constexpr uint32_t FunctionVB_call = 0x474139;                    // std::function<void(bool)>::operator()(bool)
constexpr uint32_t FunctionVB_assign = 0x490109;
constexpr uint32_t SM_LOADING_BLOCKED = 0x4CC, SM_ON_FINISH_LOADING = 0x548;
constexpr uint32_t FunctionVB_copy = 0x45C84D;                        // std::function<void(bool)>(const function&)
constexpr uint32_t FunctionVB_move = 0x47BC69;                        // std::function<void(bool)>(function&&)
constexpr uint32_t APIManager_executePartyGet = 0x4C69BD;             // (api, uint64 userId, const function&)
constexpr uint32_t APIManager_executePartyMemberList = 0x4C7AA5;      // (api, bool, const function&, platform)
constexpr uint32_t CALLBACK = 0x10;

// `use(tmp)` with tmp a std::function holding a heap __func {vtable, pad, a copy of the callback at `callback`}
template <class F> void withNextStep(uint32_t callback, uint32_t vtable, F use) {
    Frame f;
    const uint32_t copy = f.alloc(24);
    const uint32_t tmp = f.alloc(24);
    call(FunctionVB_copy, {copy, callback});
    st32(tmp + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, vtable);
        call(FunctionVB_move, {obj + CALLBACK, copy});
    }
    st32(tmp + 16, obj);
    use(tmp);
    functionDestroy(tmp);
    functionDestroy(copy);
}

// the __clone() / destroy_deallocate() slots of those __funcs
uint32_t cloneWithCallback(uint32_t self, uint32_t vtable) {
    const uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, vtable);
        call(FunctionVB_copy, {obj + CALLBACK, self + CALLBACK});
    }
    return obj;
}
void deleteWithCallback(uint32_t self) {
    functionDestroy(self + CALLBACK);
    call(fn::operator_delete, {self});
}

constexpr uint32_t SAS_openTutorial104 = 0x72804D;                    // (scene, std::function<void()>)
constexpr uint32_t SAS_changeStageInfo = 0x723089;
constexpr uint32_t SAS_checkApStory = 0x727289;
constexpr uint32_t AdvStageScrollView_setSelectItem = 0x5708D1;       // (view, index)
constexpr uint32_t SceneUtil_isOverMedalSlot = 0x46A085;              // (int)
constexpr uint32_t SceneUtil_openMedalOverDialog = 0x460251;          // (bool, BackScene, std::function<void(bool)>)
constexpr uint32_t SELECTED_STAGE = 0x298;
constexpr uint32_t SCROLL_INFO_OUT = 0x2DA;
constexpr uint32_t STORY_NODE = 0x388;                                // story.node (Panel at +0x380)
constexpr uint32_t SM_NEXT_STAGE = 0x2C8;                             // SceneManager: the stage picked
constexpr uint32_t SM_MEDAL_CHECK = 0x2F4;                            // SceneManager byte: check the medal box once
constexpr uint32_t TUTORIAL_STORY = 0x69;

// AppState::getTutorialCloseFunc(id): when a tutorial step waits for this button, run its close function
void closeTutorialStep(uint32_t id) {
    Frame f;
    const uint32_t close = f.alloc(24);
    call(AppState_getTutorialCloseFunc, {close, call(AppState_getInstance, {}), id});
    if (ld32(close + 16)) {
        functionDestroy(close);
        call(AppState_execTutorialCloseFunc, {call(AppState_getInstance, {}), id});
    }
}

// raidBossRewardPopup $_55 -> {lambda()#1} [this, done]: tutorial 33 -> {lambda()#1} [this, done]: 34 -> 31
// (each __func: vtable, pad, the scene at +8, std::function<void()> `done` at +0x10; 0x28 bytes)
constexpr uint32_t VT_rewardTutorial31 = 0xF6C0BC;
constexpr uint32_t SAS_openTutorial34 = 0x727F71;                     // (scene, std::function<void()>)
constexpr uint32_t SAS_openTutorial31 = 0x71F651;
constexpr uint32_t SCENE_CAPTURE = 8;

// raidBossAnim $_53 [?, this, done]: tutorial popup 20 -> {lambda()#1} [this, done]: the raid BGM, then the
// reward popup
constexpr uint32_t VT_raidBossAnim_after = 0xF6C140;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;   // (sret function, id, function<void()>, Widget* x3, bool)
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;       // (ResourceID, fade ms)
constexpr uint32_t SAS_raidBossRewardPopup = 0x726F11;       // (scene, std::function<void()>)
constexpr uint32_t BGM_RAID = 0x1DDCAB1E;
constexpr uint32_t TUTORIAL_RAID_BOSS = 0x14;

// the libc++ std::function copy constructor, inlined by the compiler: __clone(dst) for an inline callable,
// __clone() for a heap one
void copyFunctionInline(uint32_t dst, uint32_t src) {
    const uint32_t p = ld32(src + 16);
    if (!p) {
        st32(dst + 16, 0);
    } else if (p == src) {
        st32(dst + 16, dst);
        vcall(p, 0xC, {p, dst});
    } else {
        st32(dst + 16, vcall(p, 0x8, {p}));
    }
}

constexpr uint32_t VT_infoIn_done = 0xF6C248;                // changeStageInfo $_47::{lambda()#1} [this]
constexpr uint32_t AdventurePanel_runInfoInAnimation = 0x72ABDD;   // (panel, std::function<void()>)
constexpr uint32_t ADV_SELECT_PANEL = 0x374;
constexpr uint32_t RAID_NODE = 0x3AC;                        // raid.node (Panel at +0x3A4)
constexpr uint32_t FLAG_324 = 0x324;
constexpr uint32_t API_STAGE_STORY = 0x124;                  // APIManager: StageStory* (vector<Stage::Story> first)
constexpr uint32_t STORY_CURRENT = 0xC;                      // StageStory: the current stage id (0: none)
constexpr uint32_t StoryVector_copy = 0x4B5BC9;              // std::vector<Stage::Story>(const vector&)
constexpr uint32_t StoryVector_dtor = 0x4B58EF;              // __vector_base<Stage::Story>::~__vector_base
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;            // std::vector .at() failure (never returns)
constexpr uint32_t STORY_RECORD_SIZE = 24;

// changeStageInfo $_43 [this, raid data (shared_ptr: +8 object, +0xC control), CampaignAnimationView*]: the raid
// panel's title / boss info, its info panel in -> {lambda()#1} [this]: the boss icon takes touches again
constexpr uint32_t VT_raidInfo = 0xF6C400;
constexpr uint32_t VT_raidInfo_done = 0xF6C42C;
constexpr uint32_t AdvStageScrollView_setRaidTitle = 0x56F10D;     // (title widget, const RaidData&, armature)
constexpr uint32_t AdvStageScrollView_setRaidBossInfo = 0x56C211;  // (info widget, const RaidData&)
constexpr uint32_t CampaignAnimationView_show = 0x598D59;
constexpr uint32_t SHARED_ADD_REF = 0xC8DBB5;                      // shared_ptr control block: add a reference
constexpr uint32_t RAID_PANEL = 0x3A4;
constexpr uint32_t CURSOR_ANIM_AP = 0x3F8;
constexpr uint32_t S_TITLE = 0xE06D99, S_RAID_INFO = 0xDFF8C6, S_RAID_BOSS_ICON = 0xDFD2D3;

// changeStageInfo $_44 [this, CampaignAnimationView*]: the story panel's info -> {lambda()#1} [this]: the tutorial
// chain after it (popup 6, then the step of the next tutorial phase) -> {lambda()#1} [this]: tutorial 105
constexpr uint32_t VT_storyInfo_done = 0xF6C350;
constexpr uint32_t VT_storyTutorial_next = 0xF6C3D4;
constexpr uint32_t VT_storyTutorial_39 = 0xF6C37C;
constexpr uint32_t AdvStageScrollView_setStoryTitle = 0x56E559;      // (title widget, stage id)
constexpr uint32_t AdvStageScrollView_setBossInfo = 0x56A679;        // (info widget, stage id, deck kind)
constexpr uint32_t AdvStageScrollView_setSubMisstionInfo = 0x56B301; // (widget, stage id, const ...*)
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneManager_Tutorial12Exec = 0x61379D;           // (mgr, std::function<void()>)
constexpr uint32_t SceneManager_Tutorial18Exec = 0x613BE5;
constexpr uint32_t SceneManager_Tutorial15Exec = 0x613A45;
constexpr uint32_t SceneManager_ChainOfTutorialSphere = 0x621B31;    // (mgr, tutorial id)
constexpr uint32_t AppState_isClearTutorialPopupIDs = 0x4EB815;      // (app, id)
constexpr uint32_t TutorialMisc_create = 0x777BB5;                   // (shared_ptr* out, id)
constexpr uint32_t SHARED_RELEASE = 0xC8DBD1;
constexpr uint32_t SAS_openTutorial105 = 0x7281A5;
constexpr uint32_t API_USER_DATA = 0x8C;                             // APIManager: shared_ptr<UserData> (SafeBuffer)
constexpr uint32_t STORY_PANEL = 0x380;
constexpr uint32_t SKIP_INFO_29C = 0x29C;                            // byte: skip the next info refresh once
constexpr uint32_t S_SUB_MISSION = 0xE070E0;

// a heap copy of a __func of `size` bytes holding plain words (the __clone() slot)
uint32_t cloneWords(uint32_t self, uint32_t vtable, uint32_t size) {
    const uint32_t obj = call(fn::operator_new, {size});
    if (obj) {
        st32(obj, vtable);
        for (uint32_t k = 4; k < size; k += 4) st32(obj + k, ld32(self + k));
    }
    return obj;
}

}  // namespace

struct SceneAdventureSelectBatch3 {
    // initAdvSelect $_13 [this, quest1..4]: the story quest button. On release in the adventure select (no scroll,
    // not busy): play the decide SE, close tutorial step 0x68 if one is waiting, lock the header and refresh the
    // raid list (for the header's raid alert) before going to the story top.
    static void storyQuestTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typePtr) {
        if (ld32(typePtr) != TOUCH_ENDED) return;
        const uint32_t self = ld32(functor + 4);
        if (int32_t(ld32(self + ADVENTURE_TYPE)) != ADVENTURE_SELECT) return;
        if (ld32(self + STAGE_SCROLL_VIEW) && call(BaseStageScrollView_isScroll, {ld32(self + STAGE_SCROLL_VIEW)}))
            return;
        if (ld8(self + BUSY_29E)) return;
        if (!ld8(self + TOUCH_READY)) return;
        if (const uint32_t view = ld32(self + STAGE_SCROLL_VIEW)) call(BaseStageScrollView_setEnabledScroll, {view, 1});
        st8(self + TOUCH_READY, 0);
        call(AudioPlaySystemSEDecide, {});
        closeTutorialStep(TUTORIAL_STORY_QUEST);
        call(SceneManager_setCanTouchHeader, {call(SceneManager_getInstance, {}), 0});
        st8(self + FLAG_3FC, 1);
        st8(self + FLAG_2E4, 1);
        call(SceneManager_enableLoadingIndicator, {call(SceneManager_getInstance, {}), 1});
        const uint32_t api = call(APIManager_getInstance, {});
        Frame f;
        const uint32_t done = f.alloc(24);
        heapLambda(done, 0x18, VT_raidList_done,
                   {self, ld32(functor + 8), ld32(functor + 0xC), ld32(functor + 0x10), ld32(functor + 0x14)});
        call(APIManager_executeRaidList, {api, done});
        functionDestroy(done);
    }

    // ... the raid list's answer: slide out to the story top, or (failed) the retry dialog
    static void raidListDone(uint32_t functor, uint32_t okPtr) {
        const uint8_t ok = ld8(okPtr);
        const uint32_t self = ld32(functor + 4);
        call(SceneManager_enableLoadingIndicator, {call(SceneManager_getInstance, {}), 0});
        if (ok) {
            Frame f;
            const uint32_t out = f.alloc(24);
            heapLambda(out, 0x18, VT_raidList_out,
                       {ld32(functor + 8), ld32(functor + 0xC), ld32(functor + 0x10), ld32(functor + 0x14), self});
            call(SAS_changeOutAnimation, {self, out});
            functionDestroy(out);
            return;
        }
        if (ld8(call(APIManager_getInstance, {}) + API_REQUEST_FAILED)) {
            if (!call(APIManager_needsRetry, {call(APIManager_getInstance, {})})) {
                call(SAS_changeAdventure, {self, 0, 0});
                call(SceneManager_setCanTouchHeader, {call(SceneManager_getInstance, {}), 1});
            }
            const uint32_t api = call(APIManager_getInstance, {});
            Frame f;
            const uint32_t retry = newFunction(f, VT_raidList_retry, {});
            call(APIManager_openErrorDialog, {api, retry, 1});
            functionDestroy(retry);
        }
        st8(self + TOUCH_READY, 1);
    }
    static uint32_t raidListDoneClone(uint32_t self) { return cloneWords(self, VT_raidList_done, 0x18); }

    // ... after the slide-out: the four quest buttons stop taking touches, the header's raid alert is refreshed,
    // and the story top (type 0) is set up behind the raid list
    static void raidListOut(uint32_t functor) {
        const uint32_t self = ld32(functor + 0x14);
        for (uint32_t k = 4; k <= 0x10; k += 4) {
            const uint32_t quest = ld32(functor + k);
            vcall(quest, V_widgetSetTouchEnabled, {quest, 0});
        }
        call(SceneManager_updateHeaderRaidAlertsBtn, {call(SceneManager_getInstance, {})});
        call(SAS_changeAdventure, {self, 0, 0});
    }
    static uint32_t raidListOutClone(uint32_t self) { return cloneWords(self, VT_raidList_out, 0x18); }

    // callAdventureSelectAPI: the raid list, then the party, then the party member list, each step on the
    // previous one's success; the caller's callback rides along to the last step
    static void selectApiRaidList(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        const uint32_t api = call(APIManager_getInstance, {});
        withNextStep(functor + CALLBACK, VT_selectApi_party,
                     [&](uint32_t next) { call(APIManager_executeRaidList, {api, next}); });
    }
    static void selectApiParty(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        const uint32_t api = call(APIManager_getInstance, {});
        withNextStep(functor + CALLBACK, VT_selectApi_members,
                     [&](uint32_t next) { call(APIManager_executePartyGet, {api, 0, 0, 0, next}); });   // user 0: mine
    }
    static void selectApiMembers(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        const uint32_t api = call(APIManager_getInstance, {});
        withNextStep(functor + CALLBACK, VT_selectApi_done,
                     [&](uint32_t next) { call(APIManager_executePartyMemberList, {api, 1, next, 1}); });
    }
    static void selectApiRaidReward(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        const uint32_t api = call(APIManager_getInstance, {});
        withNextStep(functor + CALLBACK, VT_selectApi_stage,
                     [&](uint32_t next) { call(APIManager_executeRaidRewardGet151101, {api, next}); });
    }
    static void selectApiStage(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        const uint32_t api = call(APIManager_getInstance, {});
        withNextStep(functor + CALLBACK, VT_selectApi_raid,
                     [&](uint32_t next) { call(APIManager_executeStageGet160310, {api, next}); });
    }
    static void selectApiRaid(uint32_t functor, uint32_t okPtr) {
        if (!ld8(okPtr)) return;
        const uint32_t api = call(APIManager_getInstance, {});
        withNextStep(functor + CALLBACK, VT_selectApi_last,
                     [&](uint32_t next) { call(APIManager_executeRaidGet, {api, next}); });
    }
    // ... the last step: event stages for an event adventure, else (on success) the Facebook friends, or the
    // caller's callback straight away when not logged in
    static void selectApiLast(uint32_t functor, uint32_t okPtr) {
        const uint8_t ok = ld8(okPtr);
        Frame f;
        const uint32_t done = f.alloc(24);
        call(FunctionVB_copy, {done, functor + CALLBACK});
        const int32_t adventure = int32_t(ld32(call(SceneManager_getInstance, {}) + 0x28C));
        auto eventStages = [&](uint32_t category, uint32_t vtable) {
            const uint32_t api = call(APIManager_getInstance, {});
            const uint32_t fo = f.alloc(24);
            st32(fo + 16, fo);
            st32(fo, vtable);
            st8(fo + 4, 0);
            call(APIManager_executeStageEventGet, {api, category, fo});
            functionDestroy(fo);
        };
        if (adventure == 3) {
            eventStages(1, VT_selectApi_event1);
        } else if (adventure == 2 || adventure == 4) {
            eventStages(2, VT_selectApi_event2);
        } else if (ok) {
            const uint32_t copy = f.alloc(24);
            copyFunctionInline(copy, done);
            if (call(SceneUtil_isLoggedInFacebook, {}) != 1) {
                if (ld32(done + 16)) call(FunctionVB_call, {done, 1});
            } else {
                const uint32_t copy2 = f.alloc(24);
                copyFunctionInline(copy2, copy);
                {
                    const uint32_t mgr = call(SceneManager_getInstance, {});
                    const uint32_t empty = newEmptyFunction(f);
                    st8(mgr + SM_LOADING_BLOCKED, 1);
                    call(FunctionVB_assign, {mgr + SM_ON_FINISH_LOADING, empty});
                    functionDestroy(empty);
                }
                const uint32_t fb = call(FacebookManager_getInstance, {});
                const uint32_t misc = f.alloc(8);
                call(Misc_create, {misc, 811});
                const int32_t limit = rw::host_ptr<SafeBuffer>(ld32(misc))->get32(4);
                const uint32_t copy3 = f.alloc(24);
                copyFunctionInline(copy3, copy2);
                const uint32_t next = f.alloc(24);
                st32(next + 16, 0);
                const uint32_t obj = call(fn::operator_new, {0x20});
                if (obj) {
                    st32(obj, VT_selectApi_friends);
                    call(FunctionVB_move, {obj + 8, copy3});
                }
                st32(next + 16, obj);
                call(FacebookManager_getFriends, {fb, uint32_t(limit), next});
                functionDestroy(next);
                functionDestroy(copy3);
                if (ld32(misc + 4)) call(SHARED_RELEASE, {ld32(misc + 4)});
                functionDestroy(copy2);
            }
            functionDestroy(copy);
        }
        functionDestroy(done);
    }
    static uint32_t selectApiDoneClone(uint32_t self) { return cloneWithCallback(self, VT_selectApi_done); }
    static uint32_t selectApiStageClone(uint32_t self) { return cloneWithCallback(self, VT_selectApi_stage); }
    static uint32_t selectApiRaidClone(uint32_t self) { return cloneWithCallback(self, VT_selectApi_raid); }
    static uint32_t selectApiLastClone(uint32_t self) { return cloneWithCallback(self, VT_selectApi_last); }
    static uint32_t selectApiPartyClone(uint32_t self) { return cloneWithCallback(self, VT_selectApi_party); }
    static uint32_t selectApiMembersClone(uint32_t self) { return cloneWithCallback(self, VT_selectApi_members); }
    static void selectApiDelete(uint32_t self) { deleteWithCallback(self); }

    // raidBossRewardPopup: after tutorial 33, page 34 (then 31) with `done` passed along
    static void rewardTutorial34(uint32_t functor) {
        const uint32_t self = ld32(functor + SCENE_CAPTURE);
        Frame f;
        const uint32_t copy = f.alloc(24);
        const uint32_t next = f.alloc(24);
        call(fn::function_vv_copy, {copy, functor + CALLBACK});
        st32(next + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x28});
        if (obj) {
            st32(obj, VT_rewardTutorial31);
            st32(obj + SCENE_CAPTURE, self);
            call(fn::function_vv_move, {obj + CALLBACK, copy});
            st32(obj + SCENE_CAPTURE, self);
        }
        st32(next + 16, obj);
        call(SAS_openTutorial34, {self, next});
        functionDestroy(next);
        functionDestroy(copy);
    }
    static void rewardTutorial31(uint32_t functor) {
        const uint32_t self = ld32(functor + SCENE_CAPTURE);
        Frame f;
        const uint32_t copy = f.alloc(24);
        call(fn::function_vv_copy, {copy, functor + CALLBACK});
        call(SAS_openTutorial31, {self, copy});
        functionDestroy(copy);
    }
    static uint32_t rewardTutorial31Clone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x28});
        if (obj) {
            st32(obj, VT_rewardTutorial31);
            st32(obj + SCENE_CAPTURE, ld32(self + SCENE_CAPTURE));
            call(fn::function_vv_copy, {obj + CALLBACK, self + CALLBACK});
            st32(obj + SCENE_CAPTURE, ld32(self + SCENE_CAPTURE));
        }
        return obj;
    }

    // raidBossAnim $_53: the raid boss tutorial popup (20); when it closes, the BGM and the reward popup
    static void raidBossTutorial(uint32_t functor) {
        const uint32_t self = ld32(functor + SCENE_CAPTURE);
        Frame f;
        const uint32_t done = f.alloc(24);
        call(fn::function_vv_copy, {done, functor + CALLBACK});
        const uint32_t copy = f.alloc(24);
        copyFunctionInline(copy, done);
        const uint32_t after = f.alloc(24);
        st32(after + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x28});
        if (obj) {
            st32(obj, VT_raidBossAnim_after);
            st32(obj + SCENE_CAPTURE, self);
            call(fn::function_vv_move, {obj + CALLBACK, copy});
            st32(obj + SCENE_CAPTURE, self);
        }
        st32(after + 16, obj);
        const uint32_t popup = f.alloc(24);                  // the returned std::function (sret)
        call(SceneUtil_openTutorialPopup, {popup, TUTORIAL_RAID_BOSS, after, 0, 0, 0, 0});
        functionDestroy(popup);
        functionDestroy(after);
        functionDestroy(copy);
        functionDestroy(done);
    }
    static void raidBossTutorialClosed(uint32_t functor) {
        const uint32_t self = ld32(functor + SCENE_CAPTURE);
        call(AudioReplaceGameMainBGM, {BGM_RAID, 500});
        Frame f;
        const uint32_t copy = f.alloc(24);
        call(fn::function_vv_copy, {copy, functor + CALLBACK});
        call(SAS_raidBossRewardPopup, {self, copy});
        functionDestroy(copy);
    }

    // changeStageInfo $_47 [this]: with a stage selected, slide its info panel in
    static void stageInfoIn(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        if (int32_t(ld32(self + SELECTED_STAGE)) == -1) return;
        Frame f;
        const uint32_t done = newFunction(f, VT_infoIn_done, {self});
        call(AdventurePanel_runInfoInAnimation, {self + ADV_SELECT_PANEL, done});
        functionDestroy(done);
    }

    // initStory $_20 [this]: the raid button of the story top - pick the story's current stage (or the last one)
    static void storyRaidTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typePtr) {
        if (ld32(typePtr) != TOUCH_ENDED) return;
        const uint32_t self = ld32(functor + 4);
        if (const uint32_t view = ld32(self + STAGE_SCROLL_VIEW))
            if (call(BaseStageScrollView_isScroll, {view})) return;
        if (ld8(self + BUSY_29E)) return;
        const uint32_t raid = ld32(self + RAID_NODE);
        if (!vcall(raid, node::getOpacity, {raid})) return;
        if (!ld8(self + TOUCH_READY)) return;
        st8(self + TOUCH_READY, 0);
        const uint32_t api = call(APIManager_getInstance, {});
        Frame f;
        const uint32_t stories = f.alloc(12);
        call(StoryVector_copy, {stories, ld32(api + API_STAGE_STORY)});
        uint32_t stage = ld32(ld32(api + API_STAGE_STORY) + STORY_CURRENT);
        if (stage == 0) {
            const uint32_t begin = ld32(stories), end = ld32(stories + 4);
            const uint32_t count = uint32_t(int32_t(end - begin) >> 3) * 0xAAAAAAABu;   // (end - begin) / 24
            const uint32_t last = count - 1;
            if (!(last < count)) call(THROW_OUT_OF_RANGE, {stories});
            stage = ld32(begin + last * STORY_RECORD_SIZE);
        }
        st32(self + SELECTED_STAGE, stage);
        call(SAS_checkApStory, {self});
        st8(self + FLAG_324, 1);
        call(StoryVector_dtor, {stories});
    }

    // changeStageInfo $_43: the raid panel for the selected raid
    static void raidInfoShow(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        if (int32_t(ld32(self + SELECTED_STAGE)) == -1) return;
        const uint32_t title = findNode(ld32(self + RAID_NODE), S_TITLE, 5);
        const uint32_t info = findNode(ld32(self + RAID_NODE), S_RAID_INFO, 8);
        call(AdvStageScrollView_setRaidTitle, {title, ld32(functor + 8), ld32(self + CURSOR_ANIM_AP)});
        call(AdvStageScrollView_setRaidBossInfo, {info, ld32(functor + 8)});
        {
            Frame f;
            const uint32_t done = newFunction(f, VT_raidInfo_done, {self});
            call(AdventurePanel_runInfoInAnimation, {self + RAID_PANEL, done});
            functionDestroy(done);
        }
        if (const uint32_t campaign = ld32(functor + 0x10)) call(CampaignAnimationView_show, {campaign});
    }
    static uint32_t raidInfoClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x14});
        if (obj) {
            st32(obj, VT_raidInfo);
            st32(obj + 4, ld32(self + 4));
            st32(obj + 8, ld32(self + 8));
            st32(obj + 0xC, ld32(self + 0xC));
            if (ld32(self + 0xC)) call(SHARED_ADD_REF, {ld32(self + 0xC)});
            st32(obj + 0x10, ld32(self + 0x10));
        }
        return obj;
    }
    static void raidInfoShown(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        st8(self + BUSY_29E, 0);
        const uint32_t icon = findNode(ld32(self + RAID_NODE), S_RAID_BOSS_ICON, 14);
        vcall(icon, V_widgetSetTouchEnabled, {icon, 1});
    }

    // changeStageInfo $_44: the story panel for the selected stage (unless skipped once)
    static void storyInfoShow(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        if (int32_t(ld32(self + SELECTED_STAGE)) == -1) return;
        if (ld8(self + SKIP_INFO_29C)) {
            st8(self + SKIP_INFO_29C, 0);
            return;
        }
        const uint32_t title = findNode(ld32(self + STORY_NODE), S_TITLE, 5);
        const uint32_t info = findNode(ld32(self + STORY_NODE), S_RAID_INFO, 8);
        const uint32_t sub = findNode(ld32(self + STORY_NODE), S_SUB_MISSION, 10);
        call(AdvStageScrollView_setStoryTitle, {title, ld32(self + SELECTED_STAGE)});
        call(AdvStageScrollView_setBossInfo, {info, ld32(self + SELECTED_STAGE), 0});
        call(AdvStageScrollView_setSubMisstionInfo, {sub, ld32(self + SELECTED_STAGE), 0});
        {
            Frame f;
            const uint32_t done = newFunction(f, VT_storyInfo_done, {self});
            call(AdventurePanel_runInfoInAnimation, {self + STORY_PANEL, done});
            functionDestroy(done);
        }
        if (const uint32_t campaign = ld32(functor + 8)) call(CampaignAnimationView_show, {campaign});
    }

    // ... after the panel is in: tutorial popup 6, then the step for the next tutorial phase
    static void storyInfoShown(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        {
            Frame f;
            const uint32_t next = newFunction(f, VT_storyTutorial_next, {self});
            const uint32_t popup = f.alloc(24);
            call(SceneUtil_openTutorialPopup, {popup, 6, next, 0, 0, 0, 0});
            functionDestroy(popup);
            functionDestroy(next);
        }
        auto withNext = [&](uint32_t exec) {
            Frame f;
            const uint32_t next = newFunction(f, VT_storyTutorial_next, {self});
            call(exec, {call(SceneManager_getInstance, {}), next});
            functionDestroy(next);
        };
        if (call(SceneUtil_getNexTutorialphase, {}) == 210) {
            withNext(SceneManager_Tutorial12Exec);
        } else if (call(SceneUtil_getNexTutorialphase, {}) == 410) {
            withNext(SceneManager_Tutorial18Exec);
        } else if (call(SceneUtil_getNexTutorialphase, {}) == 700) {
            call(SceneManager_Tutorial15Exec, {call(SceneManager_getInstance, {})});
        } else if (call(SceneUtil_getNexTutorialphase, {}) == 710) {
            if (!call(AppState_isClearTutorialPopupIDs, {call(AppState_getInstance, {}), 39})) {
                Frame f;
                const uint32_t closed = newFunction(f, VT_storyTutorial_39, {self});
                const uint32_t popup = f.alloc(24);
                call(SceneUtil_openTutorialPopup, {popup, 39, closed, 0, 0, 0, 0});
                functionDestroy(popup);
                functionDestroy(closed);
            }
        } else if (int32_t(call(SceneUtil_getNexTutorialphase, {})) >= 995) {
            // after the tutorial: the sphere board tutorial (16) once the user's value reaches TutorialMisc 8's
            Frame f;
            const uint32_t misc = f.alloc(8);
            call(TutorialMisc_create, {misc, 8});
            if (const uint32_t row = ld32(misc)) {
                const int32_t threshold = rw::host_ptr<SafeBuffer>(row)->get32(0xC);
                const uint32_t api = call(APIManager_getInstance, {});
                const uint32_t ctrl = ld32(api + API_USER_DATA + 4);
                const uint32_t user = ld32(api + API_USER_DATA);
                if (ctrl) call(SHARED_ADD_REF, {ctrl});
                if (user && rw::host_ptr<SafeBuffer>(user)->get32(0x18) >= threshold &&
                    !call(AppState_isClearTutorialPopupIDs, {call(AppState_getInstance, {}), 16}))
                    call(SceneManager_ChainOfTutorialSphere, {call(SceneManager_getInstance, {}), 16});
                if (ctrl) call(SHARED_RELEASE, {ctrl});
            }
            if (ld32(misc + 4)) call(SHARED_RELEASE, {ld32(misc + 4)});
        }
        st8(self + BUSY_29E, 0);
    }

    // ... the tutorial step's follow-up: page 105
    static void storyTutorial105(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SAS_openTutorial105, {self, none});
        functionDestroy(none);
    }

    // initAdvSelect $_17 [this]: the tutorial step 104 (no follow-up)
    static void advSelectTutorial(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SAS_openTutorial104, {self, none});
        functionDestroy(none);
    }

    // init $_9 [this, type]: unlock the header and show the adventure type
    static void initShowAdventure(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        call(SceneManager_setCanTouchHeader, {call(SceneManager_getInstance, {}), 0});
        st8(self + FLAG_3FC, 0);
        call(SAS_changeAdventure, {self, ld32(functor + 8), 1});
    }

    // initAdvScrollView $_39 [this](int index): a stage of the list was picked
    static void stagePicked(uint32_t functor, uint32_t indexPtr) {
        const uint32_t self = ld32(functor + 4);
        const uint32_t index = ld32(indexPtr);
        call(AdvStageScrollView_setSelectItem, {ld32(self + STAGE_SCROLL_VIEW), index});
        if (ld32(self + SELECTED_STAGE) != index) {
            st32(self + SELECTED_STAGE, index);
            call(SAS_changeStageInfo, {self});
        } else if (ld8(self + SCROLL_INFO_OUT)) {
            call(SAS_changeStageInfo, {self});
        }
        const uint32_t type = ld32(self + ADVENTURE_TYPE);
        if (type <= 4 && type != 1) st32(call(SceneManager_getInstance, {}) + SM_NEXT_STAGE, index);
    }

    // changeAdventure $_12 [bool]: once after a quest, the "medal box full" dialog
    static void medalBoxCheck(uint32_t functor) {
        if (!ld8(call(SceneManager_getInstance, {}) + SM_MEDAL_CHECK)) return;
        st8(call(SceneManager_getInstance, {}) + SM_MEDAL_CHECK, 0);
        if (call(SceneUtil_isOverMedalSlot, {0}) != 1 || !ld8(functor + 4)) return;
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SceneUtil_openMedalOverDialog, {0, 5, none});
        functionDestroy(none);
    }

    // initStory $_18 [this]: the story button (released, list not scrolling, panel shown)
    static void storyButtonTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typePtr) {
        if (ld32(typePtr) != TOUCH_ENDED) return;
        const uint32_t self = ld32(functor + 4);
        if (const uint32_t view = ld32(self + STAGE_SCROLL_VIEW))
            if (call(BaseStageScrollView_isScroll, {view})) return;
        if (ld8(self + BUSY_29E)) return;
        const uint32_t story = ld32(self + STORY_NODE);
        if (!vcall(story, node::getOpacity, {story})) return;
        if (!ld8(self + TOUCH_READY)) return;
        st8(self + TOUCH_READY, 0);
        call(SAS_checkApStory, {self});
        closeTutorialStep(TUTORIAL_STORY);
    }
};

using L = SceneAdventureSelectBatch3;
RW_FUNCTION(0x7307D8, "SceneAdventureSelect::initAdvSelect::$_13::operator()", L::storyQuestTouched);
RW_FUNCTION(0x730A48, "SceneAdventureSelect::initAdvSelect::$_13::{lambda(bool)#1}::operator()", L::raidListDone);
RW_FUNCTION(0x7309E0, "SceneAdventureSelect::initAdvSelect::$_13::{lambda(bool)#1} __clone()", L::raidListDoneClone);
RW_FUNCTION(0x730C56, "SceneAdventureSelect::initAdvSelect::$_13::{lambda(bool)#1}::{lambda()#1}::operator()",
            L::raidListOut);
RW_FUNCTION(0x730BF0, "SceneAdventureSelect::initAdvSelect::$_13::{lambda(bool)#1}::{lambda()#1} __clone()",
            L::raidListOutClone);
RW_FUNCTION(0x731CE0, "SceneAdventureSelect::callAdventureSelectAPI::$_0::operator()", L::selectApiRaidList);
RW_FUNCTION(0x731F78, "SceneAdventureSelect::callAdventureSelectAPI::$_0::{lambda(bool)#1}::operator()",
            L::selectApiParty);
RW_FUNCTION(0x73221C, "SceneAdventureSelect::callAdventureSelectAPI::$_0::{lambda(bool)#1}::{lambda(bool)#1}::operator()",
            L::selectApiMembers);
RW_FUNCTION(0x731EA8, "SceneAdventureSelect::callAdventureSelectAPI::$_0::{lambda(bool)#1} __clone()",
            L::selectApiPartyClone);
RW_FUNCTION(0x731F4C, "SceneAdventureSelect::callAdventureSelectAPI::$_0::{lambda(bool)#1} destroy_deallocate()",
            L::selectApiDelete);
RW_FUNCTION(0x73214C, "SceneAdventureSelect::callAdventureSelectAPI::$_0::{lambda(bool)#1}::{lambda(bool)#1} __clone()",
            L::selectApiMembersClone);
RW_FUNCTION(0x7321F0,
            "SceneAdventureSelect::callAdventureSelectAPI::$_0::{lambda(bool)#1}::{lambda(bool)#1} destroy_deallocate()",
            L::selectApiDelete);
RW_FUNCTION(0x72F050, "SceneAdventureSelect::initAdvSelect::$_17::operator()", L::advSelectTutorial);
RW_FUNCTION(0x73123A, "SceneAdventureSelect::init::$_9::operator()", L::initShowAdventure);
RW_FUNCTION(0x72C5E0, "SceneAdventureSelect::initAdvScrollView::$_39::operator()", L::stagePicked);
RW_FUNCTION(0x730DD0, "SceneAdventureSelect::changeAdventure::$_12::operator()", L::medalBoxCheck);
RW_FUNCTION(0x72EEF8, "SceneAdventureSelect::initStory::$_18::operator()", L::storyButtonTouched);
RW_FUNCTION(0x7299AC, "SceneAdventureSelect::raidBossRewardPopup::$_55::{lambda()#1}::operator()", L::rewardTutorial34);
RW_FUNCTION(0x729C58, "SceneAdventureSelect::raidBossRewardPopup::$_55::{lambda()#1}::{lambda()#1}::operator()",
            L::rewardTutorial31);
RW_FUNCTION(0x729B74, "SceneAdventureSelect::raidBossRewardPopup::$_55::{lambda()#1}::{lambda()#1} __clone()",
            L::rewardTutorial31Clone);
RW_FUNCTION(0x729980, "SceneAdventureSelect::raidBossRewardPopup::$_55::{lambda()#1} destroy_deallocate()",
            L::selectApiDelete);
RW_FUNCTION(0x729C2C, "SceneAdventureSelect::raidBossRewardPopup::$_55::{lambda()#1}::{lambda()#1} destroy_deallocate()",
            L::selectApiDelete);
RW_FUNCTION(0x72A0CC, "SceneAdventureSelect::raidBossAnim::$_53::operator()", L::raidBossTutorial);
RW_FUNCTION(0x72A418, "SceneAdventureSelect::raidBossAnim::$_53::{lambda()#1}::operator()", L::raidBossTutorialClosed);
RW_FUNCTION(0x72A3EC, "SceneAdventureSelect::raidBossAnim::$_53::{lambda()#1} destroy_deallocate()", L::selectApiDelete);
RW_FUNCTION(0x72AAF8, "SceneAdventureSelect::changeStageInfo::$_47::operator()", L::stageInfoIn);
RW_FUNCTION(0x72EB4C, "SceneAdventureSelect::initStory::$_20::operator()", L::storyRaidTouched);
RW_FUNCTION(0x72C020, "SceneAdventureSelect::changeStageInfo::$_43::operator()", L::raidInfoShow);
RW_FUNCTION(0x72BF74, "SceneAdventureSelect::changeStageInfo::$_43 __clone()", L::raidInfoClone);
RW_FUNCTION(0x72C250, "SceneAdventureSelect::changeStageInfo::$_43::{lambda()#1}::operator()", L::raidInfoShown);
RW_FUNCTION(0x72B568, "SceneAdventureSelect::changeStageInfo::$_44::operator()", L::storyInfoShow);
RW_FUNCTION(0x72B808, "SceneAdventureSelect::changeStageInfo::$_44::{lambda()#1}::operator()", L::storyInfoShown);
RW_FUNCTION(0x72BE5C, "SceneAdventureSelect::changeStageInfo::$_44::{lambda()#1}::{lambda()#1}::operator()",
            L::storyTutorial105);
RW_FUNCTION(0x7324BC, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 4) operator()", L::selectApiRaidReward);
RW_FUNCTION(0x732754, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 5) operator()", L::selectApiStage);
RW_FUNCTION(0x7329EC, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 6) operator()", L::selectApiRaid);
RW_FUNCTION(0x7323EC, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 4) __clone()", L::selectApiDoneClone);
RW_FUNCTION(0x732684, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 5) __clone()", L::selectApiStageClone);
RW_FUNCTION(0x73291C, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 6) __clone()", L::selectApiRaidClone);
RW_FUNCTION(0x732BB4, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 7) __clone()", L::selectApiLastClone);
RW_FUNCTION(0x732490, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 4) destroy_deallocate()", L::selectApiDelete);
RW_FUNCTION(0x732728, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 5) destroy_deallocate()", L::selectApiDelete);
RW_FUNCTION(0x7329C0, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 6) destroy_deallocate()", L::selectApiDelete);
RW_FUNCTION(0x732C58, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 7) destroy_deallocate()", L::selectApiDelete);
RW_FUNCTION(0x732C84, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 7) operator()", L::selectApiLast);

// storyInAnimation::$_48 operator(): runs the captured std::function<void()> (+8) when one is set (void: the
// function's result is a leftover)
static void storyInAnimationDone(uint32_t functor) {
    if (rw::load<uint32_t>(functor + 0x18)) rw::call(0x470E69, {functor + 8});   // std::function<void()>::operator()
}
RW_FUNCTION(0x72AA54, "SceneAdventureSelect::storyInAnimation::$_48 operator()", storyInAnimationDone);

// ------------------------------------------------------------------------------------------------ the friends step
namespace {
constexpr uint32_t FacebookManager_getUserID = 0x4E57F9;       // (sret string, fb)
constexpr uint32_t APIManager_executeLinkMembersGet = 0x4D3FB1; // (api, const vector<string>& ids, platform, const function<void(bool)>&)
constexpr uint32_t StringVector_pushSlow = 0x484465;           // vector<string>::__push_back_slow_path(string&&)
constexpr uint32_t StringVector_dtor = 0x47363F;
constexpr uint32_t string_compare = 0x47CBD5;                  // std::string::compare(const char*)
constexpr uint32_t S_Empty = 0xDF8A1A;
constexpr uint32_t VT_selectApi_members2 = 0xF6D218;           // the linked members' answer {callback} (heap 0x20)
constexpr uint32_t PLATFORM_FACEBOOK = 1;

// callAdventureSelectAPI's last step ($_0's 7th lambda) {callback}(ok, -, string, friends*): logged in to Facebook ->
// the player's own id added to the friends' ids and their linked members fetched (the callback with the answer);
// not logged in -> the callback(true) at once
void selectApiFriends(uint32_t functor, uint32_t okRef, uint32_t, uint32_t strArg, uint32_t friendsRef) {
    Frame f;
    const uint32_t moved = f.alloc(12);           // the string argument, taken over
    strMove(moved, strArg);
    const uint8_t ok = ld8(okRef);
    uint32_t friends = ld32(friendsRef);
    const uint32_t none = f.alloc(12);
    st32(none, 0);
    st32(none + 4, 0);
    st32(none + 8, 0);
    if (!ok || !friends || ld32(friends + 4) == ld32(friends)) friends = none;
    const uint32_t user = f.alloc(12);
    call(FacebookManager_getUserID, {user, call(FacebookManager_getInstance, {})});
    if (call(string_compare, {user, S_Empty})) {
        const uint32_t id = newStr(f, strIsLong(user) ? ld32(user + 8) : user + 1);
        const uint32_t end = ld32(friends + 4);
        if (end >= ld32(friends + 8)) {
            call(StringVector_pushSlow, {friends, id});
        } else {
            uint32_t n = 0;
            if (end) {
                strMove(end, id);
                n = ld32(friends + 4);
            }
            st32(friends + 4, n + 12);
        }
        strFree(id);
        apiBusyDropPayment();
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t copy = f.alloc(24);
        call(FunctionVB_copy, {copy, functor + 8});
        const uint32_t next = f.alloc(24);
        st32(next + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x20});
        if (obj) {
            st32(obj, VT_selectApi_members2);
            call(FunctionVB_move, {obj + 8, copy});
        }
        st32(next + 16, obj);
        call(APIManager_executeLinkMembersGet, {api, friends, PLATFORM_FACEBOOK, next});
        functionDestroy(next);
        functionDestroy(copy);
    } else {
        apiBusyDropPayment(0);
        if (ld32(functor + 8 + 16)) call(FunctionVB_call, {functor + 8, 1});
    }
    strFree(user);
    call(StringVector_dtor, {none});
    strFree(moved);
}
}  // namespace
RW_FUNCTION(0x7331AC, "SceneAdventureSelect::callAdventureSelectAPI::$_0 (level 7: Facebook friends) operator()", selectApiFriends);
