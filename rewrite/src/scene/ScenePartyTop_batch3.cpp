// ScenePartyTop: the chain of checks run when the party top (My Page) opens (batch 3; unnamed helpers, attributed by
// position): the ranking rewards of each kind, the discounted magic stone offer, then two tutorial popups.
// Each step gets the chain state `captures` (the scene, then three words passed along) and either asks the server
// (the next step as the callback) or goes straight to the next step.
#include "SceneUi.hpp"

using namespace scene;

namespace {

constexpr uint32_t APIManager_executeRankingRewardCheck = 0x4D32C5;   // (api, RankingRewardType, const function&)
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SceneUtil_isBenefits = 0x4733C5;
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;   // (sret function, id, function<void()>, Widget* x3, bool)
constexpr uint32_t BuyMagicStoneDialog_openDiscount = 0x589005;   // (const function<void()>&, const function<void()>&)
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_setPopBenefitStone = 0x4EBFD7;   // (app, bool)
constexpr uint32_t AppState_setTutorialCloseFunc = 0x4EB8D9; // (sret function, app, std::function<void()>, id)
constexpr uint32_t TutorialMisc_create = 0x777BB5;           // (shared_ptr* out, id)
constexpr uint32_t API_RANKING_FLAGS = 0x314;                // APIManager: an object, bytes +0x1D..+0x21: rewards to check
constexpr uint32_t APP_BENEFIT_STONE = 0xA8;                 // AppState byte: offer the discounted stones
constexpr uint32_t PT_TUTORIAL_SHOWN = 0x2F8, PT_MEMBERS = 0x2D4, PT_ROOT = 0x290, PT_TUTORIAL_100 = 0x410;
constexpr uint32_t S_LEFT_UI = 0xDF85F0, S_BUTTON_WORLD = 0xE03A11;

// the steps' entry points and their callbacks' __func vtables ([4 words])
constexpr uint32_t STEP_2 = 0x64ED8D, STEP_3 = 0x64EE89, STEP_4 = 0x64EF85, STEP_5 = 0x64F081, STEP_6 = 0x64F17D;
constexpr uint32_t STEP_7 = 0x64F29D, STEP_8 = 0x64F3D1;
constexpr uint32_t VT_STEP_1 = 0xF61A50, VT_STEP_2 = 0xF619CC, VT_STEP_3 = 0xF61948, VT_STEP_4 = 0xF618C4,
                   VT_STEP_5 = 0xF61840;
constexpr uint32_t VT_discountDone = 0xF61814;   // [3 words]
constexpr uint32_t VT_discountClosed = 0xF617E8; // [scene]
constexpr uint32_t VT_tutorial32 = 0xF617BC;     // [2 words]
constexpr uint32_t VT_tutorial100 = 0xF61764;    // []

// a ranking reward check: when the reward flag is set ask the server (the callback carries the 4 words), else go on
void rankingStep(uint32_t captures, uint32_t flag, uint32_t type, uint32_t vtable, uint32_t next) {
    if (!ld8(ld32(call(fn::APIManager_getInstance, {}) + API_RANKING_FLAGS) + flag)) {
        call(next, {captures});
        return;
    }
    const uint32_t w0 = ld32(captures), w1 = ld32(captures + 4), w2 = ld32(captures + 8), w3 = ld32(captures + 0xC);
    call(SceneManager_enableLoadingIndicator, {call(fn::SceneManager_getInstance, {}), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t done = f.alloc(24);
    st32(done + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x14});
    if (obj) {
        st32(obj, vtable);
        st32(obj + 4, w0);
        st32(obj + 8, w1);
        st32(obj + 0xC, w2);
        st32(obj + 0x10, w3);
    }
    st32(done + 16, obj);
    call(APIManager_executeRankingRewardCheck, {api, type, done});
    functionDestroy(done);
}

}  // namespace

struct ScenePartyTopBatch3 {
    static void step1(uint32_t c) { rankingStep(c, 0x20, 1, VT_STEP_1, STEP_2); }
    static void step2(uint32_t c) { rankingStep(c, 0x1F, 1, VT_STEP_2, STEP_3); }
    static void step3(uint32_t c) { rankingStep(c, 0x21, 4, VT_STEP_3, STEP_4); }
    static void step4(uint32_t c) { rankingStep(c, 0x1E, 2, VT_STEP_4, STEP_5); }
    static void step5(uint32_t c) { rankingStep(c, 0x1D, 3, VT_STEP_5, STEP_6); }

    // the discounted magic stone offer, once (when no benefit is running)
    static void stepDiscount(uint32_t captures) {
        const uint32_t scene = ld32(captures);
        if (call(SceneUtil_isBenefits, {}) || !ld8(call(AppState_getInstance, {}) + APP_BENEFIT_STONE)) {
            call(STEP_7, {captures + 4});
            return;
        }
        Frame f;
        const uint32_t done = newFunction(f, VT_discountDone,
                                          {ld32(captures + 4), ld32(captures + 8), ld32(captures + 0xC)});
        const uint32_t closed = newFunction(f, VT_discountClosed, {scene});
        call(BuyMagicStoneDialog_openDiscount, {done, closed});
        functionDestroy(closed);
        functionDestroy(done);
        call(AppState_setPopBenefitStone, {call(AppState_getInstance, {}), 0});
    }

    // tutorial popup 32 once the party has enough members (TutorialMisc 1, else 32)
    static void stepTutorial32(uint32_t captures) {
        const uint32_t scene = ld32(captures);
        Frame f;
        const uint32_t misc = f.alloc(8);
        call(TutorialMisc_create, {misc, 1});
        int32_t needed = 0x20;
        if (ld32(misc)) needed = safe32(ld32(misc), 0xC);
        const int32_t members = int32_t(ld32(scene + PT_MEMBERS + 4) - ld32(scene + PT_MEMBERS)) >> 2;
        if (ld8(scene + PT_TUTORIAL_SHOWN) && members >= needed) {
            const uint32_t closed = newFunction(f, VT_tutorial32, {ld32(captures + 4), ld32(captures + 8)});
            const uint32_t popup = f.alloc(24);
            call(SceneUtil_openTutorialPopup, {popup, 0x20, closed, 0, 0, 0, 0});
            functionDestroy(popup);
            functionDestroy(closed);
        } else {
            call(STEP_8, {captures + 4});
        }
        if (ld32(misc + 4)) call(fn::shared_release, {ld32(misc + 4)});
    }

    // tutorial popup 100 on the quest button, registered as the tutorial close function
    static void stepTutorial100(uint32_t captures) {
        const uint32_t scene = ld32(captures);
        const uint32_t leftUi = findNode(ld32(scene + PT_ROOT), S_LEFT_UI, 6);
        const uint32_t button = findNode(leftUi, S_BUTTON_WORLD, 12);
        const uint32_t app = call(AppState_getInstance, {});
        Frame f;
        const uint32_t closed = newFunction(f, VT_tutorial100, {});
        const uint32_t popup = f.alloc(24);
        call(SceneUtil_openTutorialPopup, {popup, 100, closed, button, 0, 0, 0});
        const uint32_t previous = f.alloc(24);
        call(AppState_setTutorialCloseFunc, {previous, app, popup, 100});
        functionDestroy(previous);
        functionDestroy(popup);
        functionDestroy(closed);
        st8(scene + PT_TUTORIAL_100, 1);
        call(SceneManager_setCanTouchHeader, {call(fn::SceneManager_getInstance, {}), 1});
    }
};

using L = ScenePartyTopBatch3;
RW_FUNCTION(0x64EC90, "ScenePartyTop (sub_64EC90): ranking reward check 1", L::step1);
RW_FUNCTION(0x64ED8C, "ScenePartyTop (sub_64ED8C): ranking reward check 2", L::step2);
RW_FUNCTION(0x64EE88, "ScenePartyTop (sub_64EE88): ranking reward check 3", L::step3);
RW_FUNCTION(0x64EF84, "ScenePartyTop (sub_64EF84): ranking reward check 4", L::step4);
RW_FUNCTION(0x64F080, "ScenePartyTop (sub_64F080): ranking reward check 5", L::step5);
RW_FUNCTION(0x64F17C, "ScenePartyTop (sub_64F17C): the discounted stones", L::stepDiscount);
RW_FUNCTION(0x64F29C, "ScenePartyTop (sub_64F29C): tutorial popup 32", L::stepTutorial32);
RW_FUNCTION(0x64F3D0, "ScenePartyTop (sub_64F3D0): tutorial popup 100", L::stepTutorial100);
