// SceneAdventureSelect's event pieces: initEvent (the event / raid side buttons, the quest start button, the event
// limit and description), the rotating event banner texts, the event in-animation, and the medal guide (tutorial 13)
// shown while too few 3-star+ medals of the needed kind are owned. Events need event data from the server (the
// private server sends none yet), so most of this is unverified.
#include "../scene/SceneUi.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetImageView = 0x531631;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t AdventurePanel_createWidget = 0x723955, AdventurePanel_runRightInAnimation = 0x726115;
constexpr uint32_t SAS_initEventSetButton = 0x723DBD, SAS_changeStageInfo = 0x723089;
constexpr uint32_t SAS_updateEventTimeChack = 0x71F8CD, SAS_eventInAnimation = 0x723EBD;
constexpr uint32_t SAS_scrollRunInAnimation = 0x726285, SAS_backButtonRunInAnimation = 0x726015;
constexpr uint32_t SceneSphereBoardSelect_getSphereBoardValidCnt = 0x6BB4FD;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_openTutorialPopup = 0x4684C9;
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5, text_createTimeLeftString = 0x492B75;
constexpr uint32_t Node_stopAllActions = 0x893815, FadeIn_create = 0x86B115, FadeOut_create = 0x86B25D;
constexpr uint32_t DelayTime_create = 0x86852D, CallFunc_create = 0x866B39, Sequence_create = 0x867999;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t SafeBuffer_read = 0x45A269, ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t function_vv_copy = 0x4683CD, function_vv_call = 0x470E69;
constexpr uint32_t VT_QuestStart = 0xF6C718, VT_EventBoard = 0xF6C6C0, VT_RaidBoard = 0xF6C668;   // {this}
constexpr uint32_t VT_BannerNext = 0xF6C744;      // {this}
constexpr uint32_t S_RaidInfo = 0xDFF8C6, S_ButtonQuestStart = 0xE06ED0, S_TxtQuestStart = 0xE06EE2;
constexpr uint32_t S_ImageButtonQuestStart = 0xE06EF1, S_But10 = 0xE06F08, S_EvButtonJson = 0xE06F9D;
constexpr uint32_t S_ButtonEvBoard = 0xE06FCD, S_Txt = 0xDF94D1, S_ButtonEvBoard0 = 0xE06FDD;
constexpr uint32_t S_RaidButtonJson = 0xE06FEF, S_TxtLimit = 0xDF9EED, S_TxtDescription = 0xE00690;
constexpr uint32_t S_TxtWording = 0xDF862F;
constexpr uint32_t V_setVisible = 0xB0, V_addChildZName = 0xFC;
// SceneAdventureSelect
constexpr uint32_t F_MODE = 0x28C, F_PARENT = 0x2A8, F_EVENT_INDEX = 0x328, F_BANNER = 0x344;
constexpr uint32_t F_BANNER_TEXTS = 0x34C, F_BANNER_DATES = 0x358, F_BANNER_INDEX = 0x364, F_BANNER_COUNT = 0x368;
constexpr uint32_t F_LIMIT_SHOWN = 0x348, F_TIMER = 0x36C, F_DESCRIPTION = 0x370, F_INFO = 0x394;
constexpr uint32_t F_EV_BUTTONS = 0x3BC, F_RAID_BUTTONS = 0x3C8;
constexpr uint32_t API_EVENTS = 0x12C, API_USER_MEDAL = 0xD4, EVENT_SIZE = 56;
constexpr uint32_t MODE_EVENT = 3, MODE_RAID = 4;

void labelOf(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}

void hide(uint32_t node) { vcall(node, V_setVisible, {node, 0}); }

// the event / raid side buttons: Button_EV_Board opens the board when it has valid boards, else hidden
void sideButtons(uint32_t self, uint32_t panel, uint32_t json, int32_t label, uint32_t boardType, uint32_t vt,
                 bool hideSecond) {
    call(AdventurePanel_createWidget, {self + panel, ld32(self + F_PARENT), json});
    const uint32_t widget = ld32(self + panel + 8);
    vcall(widget, V_setVisible, {widget, 1});
    const uint32_t board = findNode(ld32(self + panel + 8), S_ButtonEvBoard, 0xF);
    labelOf(board, S_Txt, 3, label);
    if (call(SceneSphereBoardSelect_getSphereBoardValidCnt, {boardType})) {
        Frame f;
        const uint32_t fo = newFunction(f, vt, {self});
        call(SAS_initEventSetButton, {board, fo});
        functionDestroy(fo);
    } else {
        hide(board);
    }
    if (hideSecond) {
        const uint32_t second = findNode(ld32(self + panel + 8), S_ButtonEvBoard0, 0x11);
        if (second) hide(second);
    }
}
}  // namespace

namespace adventure_event {

uint32_t initEvent(uint32_t self) {
    const uint32_t raidInfo = findNode(ld32(self + F_INFO), S_RaidInfo, 8);
    const uint32_t start = findNode(raidInfo, S_ButtonQuestStart, 0x11);
    labelOf(start, S_TxtQuestStart, 0xE, 102900001);
    if (!findNode(start, S_ImageButtonQuestStart, 0x16)) {
        uint32_t button;
        {
            Frame f;
            const uint32_t s = newStr(f, S_But10, 5);
            button = call(ImageButton_create, {s, 0});
            strFree(s);
        }
        call(ImageButton_setTargetImageView, {button, start});
        {
            Frame f;
            const uint32_t s = newStr(f, S_ImageButtonQuestStart, 0x16);
            vcall(start, V_addChildZName, {start, button, 0, s});
            strFree(s);
        }
        Frame f;
        const uint32_t fo = newFunction(f, VT_QuestStart, {self});
        call(ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    if (!ld32(self + F_EV_BUTTONS + 8) && ld32(self + F_MODE) == MODE_EVENT)
        sideButtons(self, F_EV_BUTTONS, S_EvButtonJson, 102920005, 4, VT_EventBoard, true);
    if (!ld32(self + F_RAID_BUTTONS + 8) && ld32(self + F_MODE) == MODE_RAID)
        sideButtons(self, F_RAID_BUTTONS, S_RaidButtonJson, 102920006, 5, VT_RaidBoard, false);
    call(SAS_changeStageInfo, {self});
    const uint32_t events = ld32(call(fn::APIManager_getInstance, {}) + API_EVENTS);
    const uint32_t base = ld32(events);
    const uint32_t index = ld32(self + F_EVENT_INDEX);
    if (((ld32(events + 4) - base) >> 3) * 0xB6DB6DB7u <= index) call(ThrowOutOfRange, {events});
    const uint32_t limit = findNode(ld32(self + F_INFO), S_TxtLimit, 9);
    st32(self + F_TIMER, 0);
    const uint32_t description = ld32(self + F_DESCRIPTION);
    const uint32_t event = base + EVENT_SIZE * index;
    if (ld32(event + 0x24) == 2) {
        labelOf(description, S_TxtDescription, 0xF, 102900023);
        vcall(ld32(self + F_DESCRIPTION), V_setVisible, {ld32(self + F_DESCRIPTION), 1});
        st8(self + F_LIMIT_SHOWN, 0);
        hide(limit);
    } else {
        hide(description);
        st8(self + F_LIMIT_SHOWN, 1);
        vcall(limit, V_setVisible, {limit, 1});   // r1 still holds the 1 just stored (no argument set)
        call(SAS_updateEventTimeChack, {self, fbits(1.0f)});
    }
    Frame f;
    const uint32_t date = f.alloc(8);
    st32(date, (uint32_t(rw::load<uint16_t>(event + 0x2A)) << 16) | rw::load<uint16_t>(event + 0x28));
    st32(date + 4, (uint32_t(rw::load<uint16_t>(event + 0x2E)) << 16) | rw::load<uint16_t>(event + 0x2C));
    if (int32_t(call(Date_calculateIntervalSinceNow, {date})) >= 8640000) hide(limit);
    call(SAS_eventInAnimation, {self});
    return 0;
}
RW_FUNCTION(0x721838, "SceneAdventureSelect::initEvent", adventure_event::initEvent);

// the banner's next text and time left, faded in and out, then the next banner
uint32_t eventBannerDisps(uint32_t self) {
    const uint32_t wording = findNode(ld32(self + F_BANNER), S_TxtWording, 0xB);
    const uint32_t limit = findNode(ld32(self + F_BANNER), S_TxtLimit, 9);
    const uint32_t index = ld32(self + F_BANNER_INDEX);
    const uint32_t texts = ld32(self + F_BANNER_TEXTS);
    if (((ld32(self + F_BANNER_TEXTS + 4) - texts) >> 2) * 0xAAAAAAABu <= index) call(ThrowOutOfRange, {self});
    call(fn::Text_setString, {wording, texts + 12 * index});
    const uint32_t dates = ld32(self + F_BANNER_DATES);
    if (((ld32(self + F_BANNER_DATES + 4) - dates) >> 3) <= ld32(self + F_BANNER_INDEX)) call(ThrowOutOfRange, {self});
    const uint32_t left = call(Date_calculateIntervalSinceNow, {dates + 8 * ld32(self + F_BANNER_INDEX)});
    {
        Frame f;
        const uint32_t s = f.alloc(12);
        call(text_createTimeLeftString, {s, left});
        setTextC(limit, strData(s));
        strFree(s);
    }
    call(Node_stopAllActions, {wording});
    call(Node_stopAllActions, {limit});
    {
        const uint32_t in = call(FadeIn_create, {fbits(1.0f)});
        const uint32_t wait = call(DelayTime_create, {fbits(0.5f)});
        const uint32_t out = call(FadeOut_create, {fbits(1.0f)});
        Frame f;
        const uint32_t fo = newFunction(f, VT_BannerNext, {self});
        const uint32_t next = call(CallFunc_create, {fo});
        call(fn::Node_runAction, {wording, call(Sequence_create, {in, wait, out, next, 0})});
        functionDestroy(fo);
    }
    {
        const uint32_t in = call(FadeIn_create, {fbits(1.0f)});
        const uint32_t wait = call(DelayTime_create, {fbits(0.5f)});
        const uint32_t out = call(FadeOut_create, {fbits(1.0f)});
        call(fn::Node_runAction, {limit, call(Sequence_create, {in, wait, out, 0})});
    }
    uint32_t next = ld32(self + F_BANNER_INDEX) + 1;
    if (int32_t(ld32(self + F_BANNER_COUNT)) <= int32_t(next)) next = 0;
    st32(self + F_BANNER_INDEX, next);
    return 0;
}
RW_FUNCTION(0x723994, "SceneAdventureSelect::eventBannerDisps", adventure_event::eventBannerDisps);

uint32_t eventInAnimation(uint32_t self) {
    if (ld32(self + F_MODE) == MODE_EVENT) call(AdventurePanel_runRightInAnimation, {self + F_EV_BUTTONS});
    if (ld32(self + F_MODE) == MODE_RAID) call(AdventurePanel_runRightInAnimation, {self + F_RAID_BUTTONS});
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SAS_scrollRunInAnimation, {self, none});
        functionDestroy(none);
    }
    call(SAS_backButtonRunInAnimation, {self});
    return 0;
}
RW_FUNCTION(0x723EBC, "SceneAdventureSelect::eventInAnimation", adventure_event::eventInAnimation);

// the medal guide: with more medals than misc 406 and at least misc 405 of them 3-star+ of kind 1, `done` runs
// at once; otherwise tutorial 13 first
uint32_t openTutorial13(uint32_t, uint32_t done) {
    const uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t threshold;
    {
        Frame f;
        const uint32_t row = masterRow(f, fn::Misc_create, 406);
        const uint32_t v = f.alloc(4);
        call(SafeBuffer_read, {ld32(row), v, ld32(ld32(row) + 0xC) + 4, 4, 4});
        threshold = ld32(v);
        sharedRelease(row);
    }
    const uint32_t owned = ld32(ld32(api + API_USER_MEDAL) + 0x20);
    bool guide = threshold <= owned;
    if (!guide) {
        int32_t found = 0;
        for (uint32_t i = 0, off = 0; i < ld32(ld32(api + API_USER_MEDAL) + 0x20); i++, off += 72) {
            const uint32_t mgr = call(MedalDataManager_getInstance, {});
            const uint32_t sb = ld32(api + API_USER_MEDAL);
            Frame f;
            const uint32_t medal = f.alloc(72);
            call(SafeBuffer_read, {sb, medal, ld32(sb + 0xC) + off, 0x48, off});
            const uint32_t data = f.alloc(8);
            call(MedalDataManager_getMedalData, {data, mgr, ld32(medal), ld32(medal + 4), 0, 0,
                                                 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
            const uint32_t row = ld32(ld32(data) + 0x58);
            const uint32_t v = f.alloc(4);
            call(SafeBuffer_read, {row, v, ld32(row + 0xC) + 0x470, 4, 0x470});
            bool stop = false;
            if (int32_t(ld32(v)) >= 3) {
                call(SafeBuffer_read, {row, v, ld32(row + 0xC) + 0x454, 4, 0x454});
                stop = ld32(v) == 1;
                if (stop) found++;
            }
            sharedRelease(data);
            if (stop) break;
        }
        Frame f;
        const uint32_t row = masterRow(f, fn::Misc_create, 405);
        const uint32_t v = f.alloc(4);
        call(SafeBuffer_read, {ld32(row), v, ld32(ld32(row) + 0xC) + 4, 4, 4});
        const int32_t need = int32_t(ld32(v));
        sharedRelease(row);
        if (need > found) {
            if (ld32(done + 0x10)) call(function_vv_call, {done});
            return 0;
        }
    }
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vv_copy, {copy, done});
    const uint32_t popup = f.alloc(20);
    call(SceneUtil_openTutorialPopup, {popup, 13, copy, 0, 0, 0, 0});
    functionDestroy(popup);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x727C2C, "SceneAdventureSelect::openTutorial13", adventure_event::openTutorial13);

}  // namespace adventure_event
