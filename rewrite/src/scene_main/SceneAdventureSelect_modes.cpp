// SceneAdventureSelect: the adventure screen's mode buttons (initAdvSelect $_14, $_15, $_16) and their request
// chains. A touch (the screen idle on the story list, nothing scrolling) locks the screen and loads what the mode
// needs: $_14 / $_15 the stage events (types 2 / 1) then the raid list, $_16 the raid list alone; then the screen
// runs out, the four mode buttons go off and the adventure type changes. A failed request unlocks the screen (back
// to the story when the error does not retry) and shows the API error.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t SAS_changeAdventure = 0x71FC3D;           // (scene, AdventureType, bool)
constexpr uint32_t SAS_changeOutAnimation = 0x71F00D;        // (scene, std::function<void()>)
constexpr uint32_t BaseStageScrollView_isScroll = 0x59DC19, BaseStageScrollView_setEnabledScroll = 0x59D63D;
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D, SceneManager_updateHeaderRaidAlertsBtn = 0x6131F9;
constexpr uint32_t APIManager_executeStageEventGet = 0x4CAEFD;   // (api, type, const function<void(bool)>&)
constexpr uint32_t APIManager_executeRaidList = 0x4CC009;        // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_needsRetry = 0x4C3C63, APIManager_openErrorDialog = 0x4C3C8D;
// SceneAdventureSelect
constexpr uint32_t F_MODE = 0x28C;            // 7: the story list
constexpr uint32_t F_BUSY_29E = 0x29E;
constexpr uint32_t F_SCROLL = 0x300;          // BaseStageScrollView*
constexpr uint32_t F_CAN_TAP = 0x325;
constexpr uint32_t F_LOADING = 0x3FC;
constexpr uint32_t MODE_STORY = 7;
constexpr uint32_t SM_ADVENTURE_MODE = 0x28C; // SceneManager: the adventure mode to show
constexpr uint32_t API_SHOW_ERROR = 0x14;
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C;
constexpr uint32_t TOUCH_ENDED = 2;
constexpr int32_t NO_EVENT = -1;

// the request's answer on the heap {vt, this, the four mode buttons}
uint32_t answer(Frame &f, uint32_t vt, uint32_t me, uint32_t buttons) {
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t c0 = ld32(buttons), c1 = ld32(buttons + 4), c2 = ld32(buttons + 8), c3 = ld32(buttons + 12);
    const uint32_t h = call(fn::operator_new, {0x18});
    if (h) {
        st32(h, vt);
        st32(h + 4, me);
        st32(h + 8, c0);
        st32(h + 0xC, c1);
        st32(h + 0x10, c2);
        st32(h + 0x14, c3);
    }
    st32(fo + 16, h);
    return fo;
}
// a failed request: unlocked (back to adventure `back` unless it retries), the API error, taps on again
void failed(uint32_t me, uint32_t errVt, uint32_t back = 0) {
    if (ld8(call(fn::APIManager_getInstance, {}) + API_SHOW_ERROR)) {
        if (!call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) {
            call(SAS_changeAdventure, {me, back, 0});
            call(SceneManager_setCanTouchHeader, {sceneManager(), 1});
        }
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, errVt, {});
        call(APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }
    st8(me + F_CAN_TAP, 1);
}

// the button {this, enabled, the four mode buttons}(sender, type)
void buttonTouched(uint32_t f, uint32_t typeRef, uint32_t nextVt, int32_t eventType) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (ld32(me + F_MODE) != MODE_STORY || !ld8(f + 8)) return;
    const uint32_t scroll = ld32(me + F_SCROLL);
    if (scroll && call(BaseStageScrollView_isScroll, {scroll})) return;
    if (ld8(me + F_BUSY_29E) || !ld8(me + F_CAN_TAP)) return;
    st8(me + F_CAN_TAP, 0);
    if (ld32(me + F_SCROLL)) call(BaseStageScrollView_setEnabledScroll, {ld32(me + F_SCROLL), 1});
    call(fn::AudioPlaySystemSEDecide, {});
    call(SceneManager_setCanTouchHeader, {sceneManager(), 0});
    st8(me + F_LOADING, 1);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame fr;
    const uint32_t fo = answer(fr, nextVt, me, f + 0xC);
    if (eventType == NO_EVENT) call(APIManager_executeRaidList, {api, fo});
    else call(APIManager_executeStageEventGet, {api, uint32_t(eventType), fo});
    functionDestroy(fo);
}
// the events arrived {this, buttons}(ok): the raid list next
void eventsLoaded(uint32_t f, uint32_t okRef, uint32_t nextVt, uint32_t errVt) {
    const uint8_t ok = ld8(okRef);
    const uint32_t me = ld32(f + 4);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        failed(me, errVt);
        return;
    }
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame fr;
    const uint32_t fo = answer(fr, nextVt, me, f + 8);
    call(APIManager_executeRaidList, {api, fo});
    functionDestroy(fo);
}
// the raid list arrived {this, buttons}(ok): the screen runs out (then slidOut {buttons, this})
void raidListLoaded(uint32_t f, uint32_t okRef, uint32_t nextVt, uint32_t errVt) {
    const uint8_t ok = ld8(okRef);
    const uint32_t me = ld32(f + 4);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        failed(me, errVt);
        return;
    }
    Frame fr;
    const uint32_t fo = fr.alloc(20);
    st32(fo + 16, 0);
    const uint32_t c0 = ld32(f + 8), c1 = ld32(f + 0xC), c2 = ld32(f + 0x10), c3 = ld32(f + 0x14);
    const uint32_t h = call(fn::operator_new, {0x18});
    if (h) {
        st32(h, nextVt);
        st32(h + 4, c0);
        st32(h + 8, c1);
        st32(h + 0xC, c2);
        st32(h + 0x10, c3);
        st32(h + 0x14, me);
    }
    st32(fo + 16, h);
    call(SAS_changeOutAnimation, {me, fo});
    functionDestroy(fo);
}
// run out {the four buttons, this}: the buttons off, the SceneManager's adventure mode (< 0: kept), the header's
// raid alert, the adventure type
void slidOut(uint32_t f, int32_t smMode, bool raidAlerts, uint32_t adventure) {
    const uint32_t me = ld32(f + 0x14);
    for (uint32_t k = 4; k <= 0x10; k += 4) vcall(ld32(f + k), V_SET_TOUCH_ENABLED, {ld32(f + k), 0});
    if (smMode >= 0) st32(sceneManager() + SM_ADVENTURE_MODE, uint32_t(smMode));
    if (raidAlerts) call(SceneManager_updateHeaderRaidAlertsBtn, {sceneManager()});
    call(SAS_changeAdventure, {me, adventure, 0});
}

// $_14: events of type 2, raid list, run out -> mode 2, adventure 1
void touch14(uint32_t f, uint32_t, uint32_t t) { buttonTouched(f, t, 0xF6CCC4, 2); }
void events14(uint32_t f, uint32_t ok) { eventsLoaded(f, ok, 0xF6CCF0, 0xF6CD74); }
void raids14(uint32_t f, uint32_t ok) { raidListLoaded(f, ok, 0xF6CD1C, 0xF6CD48); }
void out14(uint32_t f) { slidOut(f, 2, true, 1); }
// $_15: events of type 1, raid list, run out -> mode 3, adventure 1
void touch15(uint32_t f, uint32_t, uint32_t t) { buttonTouched(f, t, 0xF6CBBC, 1); }
void events15(uint32_t f, uint32_t ok) { eventsLoaded(f, ok, 0xF6CBE8, 0xF6CC6C); }
void raids15(uint32_t f, uint32_t ok) { raidListLoaded(f, ok, 0xF6CC14, 0xF6CC40); }
void out15(uint32_t f) { slidOut(f, 3, false, 1); }
// $_16: raid list, run out -> adventure 5
void touch16(uint32_t f, uint32_t, uint32_t t) { buttonTouched(f, t, 0xF6CB0C, NO_EVENT); }
void raids16(uint32_t f, uint32_t ok) { raidListLoaded(f, ok, 0xF6CB38, 0xF6CB64); }
void out16(uint32_t f) { slidOut(f, -1, true, 5); }
}  // namespace

RW_FUNCTION(0x72FF7C, "SceneAdventureSelect::initAdvSelect $_14::operator()", touch14);
RW_FUNCTION(0x730188, "SceneAdventureSelect::initAdvSelect $_14 events loaded::operator()(bool)", events14);
RW_FUNCTION(0x7303B0, "SceneAdventureSelect::initAdvSelect $_14 raid list loaded::operator()(bool)", raids14);
RW_FUNCTION(0x7305BE, "SceneAdventureSelect::initAdvSelect $_14 ran out::operator()", out14);
RW_FUNCTION(0x72F720, "SceneAdventureSelect::initAdvSelect $_15::operator()", touch15);
RW_FUNCTION(0x72F92C, "SceneAdventureSelect::initAdvSelect $_15 events loaded::operator()(bool)", events15);
RW_FUNCTION(0x72FB54, "SceneAdventureSelect::initAdvSelect $_15 raid list loaded::operator()(bool)", raids15);
RW_FUNCTION(0x72FD62, "SceneAdventureSelect::initAdvSelect $_15 ran out::operator()", out15);
RW_FUNCTION(0x72F184, "SceneAdventureSelect::initAdvSelect $_16::operator()", touch16);
RW_FUNCTION(0x72F384, "SceneAdventureSelect::initAdvSelect $_16 raid list loaded::operator()(bool)", raids16);
RW_FUNCTION(0x72F592, "SceneAdventureSelect::initAdvSelect $_16 ran out::operator()", out16);

// ------------------------------------------------------------------------------------------------ init's controls
namespace {
constexpr uint32_t F_FILTER_LABEL = 0x290;    // ui::Text: the filter button's label
constexpr uint32_t F_ARROW_MOVING = 0x2D8, F_ARROW_LEFT = 0x2D9, F_ARROW_TIME = 0x2DC, F_ARROW_2E0 = 0x2E0;
constexpr uint32_t BaseStageScrollView_migrationLeft = 0x59DA99, BaseStageScrollView_migrationRight = 0x59DAB3;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_openAdventureSelectFilterPopup = 0x476E81;   // (function<void(bool)> by value)
constexpr uint32_t SAS_initAdvScrollView = 0x71FF15, SAS_initStory = 0x720271;
constexpr uint32_t AppState_getAdventureSelectFilterState = 0x4EB1A1;     // (sret shared_ptr, app)
constexpr uint32_t VT_FilterClosed = 0xF6CFDC;   // $_5's answer {this}
constexpr uint32_t SE_ARROW = 509000170;
constexpr int32_t TUTORIAL_DONE = 995;
constexpr int32_t TXT_FILTER_ON = 100100012, TXT_FILTER_OFF = 100100035;
constexpr uint32_t TOUCH_BEGAN = 0, TOUCH_CANCELED = 3;

// $_7 / $_6 (the story list's arrows) {this}(sender, type): pressed (after the tutorial) -> the list moves a page
// right / left with the arrow sound; released -> the move ends
void arrowTouched(uint32_t f, uint32_t typeRef, uint32_t migrate, uint8_t left) {
    const uint32_t type = ld32(typeRef);
    const uint32_t me = ld32(f + 4);
    if (type == TOUCH_CANCELED || type == TOUCH_ENDED) {
        st8(me + F_ARROW_MOVING, 0);
    } else if (type == TOUCH_BEGAN) {
        if (int32_t(call(SceneUtil_getNexTutorialphase, {})) < TUTORIAL_DONE) return;
        playLayerSE(SE_ARROW);
        st8(me + F_ARROW_LEFT, left);
        st32(me + F_ARROW_TIME, 0);
        st32(me + F_ARROW_2E0, 0);
        st8(me + F_ARROW_MOVING, uint8_t(call(migrate, {ld32(me + F_SCROLL), 1})));
    }
}
void rightArrow(uint32_t f, uint32_t, uint32_t t) { arrowTouched(f, t, BaseStageScrollView_migrationRight, 0); }
void leftArrow(uint32_t f, uint32_t, uint32_t t) { arrowTouched(f, t, BaseStageScrollView_migrationLeft, 1); }

// $_5 (the filter button) {this}(sender, type): the story filter popup
void filterTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (!ld8(me + F_CAN_TAP)) return;
    st8(me + F_CAN_TAP, 0);
    call(fn::AudioPlaySystemSEDecide, {});
    Frame fr;
    const uint32_t closed = newFunction(fr, VT_FilterClosed, {me});
    call(SceneUtil_openAdventureSelectFilterPopup, {closed});
    functionDestroy(closed);
}
// $_5's answer {this}(changed): changed -> the header back on, the list and the story rebuilt, the button's label
// (filter on / off); taps on again
void filterClosed(uint32_t f, uint32_t changedRef) {
    const uint32_t me = ld32(f + 4);
    if (ld8(changedRef)) {
        call(SceneManager_setCanTouchHeader, {sceneManager(), 1});
        call(SAS_initAdvScrollView, {me, 0});
        call(SAS_initStory, {me});
        Frame fr;
        const uint32_t state = fr.alloc(8);
        call(AppState_getAdventureSelectFilterState, {state, call(fn::AppState_getInstance, {})});
        setTextId(ld32(me + F_FILTER_LABEL), ld8(ld32(state)) ? TXT_FILTER_ON : TXT_FILTER_OFF);
        sharedRelease(state);
    }
    st8(me + F_CAN_TAP, 1);
}

// $_10 {this, adventure type}: the header locked, the screen changes to that adventure
void openAdventure(uint32_t f) {
    const uint32_t me = ld32(f + 4);
    call(SceneManager_setCanTouchHeader, {sceneManager(), 0});
    st8(me + F_LOADING, 0);
    call(SAS_changeAdventure, {me, ld32(f + 8), 1});
}
}  // namespace

RW_FUNCTION(0x731380, "SceneAdventureSelect::init $_7::operator() (right arrow)", rightArrow);
RW_FUNCTION(0x7314E4, "SceneAdventureSelect::init $_6::operator() (left arrow)", leftArrow);
RW_FUNCTION(0x731648, "SceneAdventureSelect::init $_5::operator() (filter)", filterTouched);
RW_FUNCTION(0x731794, "SceneAdventureSelect::init $_5 filter closed::operator()(bool)", filterClosed);
RW_FUNCTION(0x731176, "SceneAdventureSelect::init $_10::operator()", openAdventure);

// ------------------------------------------------------------------------------------------------ the events
namespace {
constexpr uint32_t F_BANNER_INDEX = 0x328, F_BANNERS = 0x32C;   // AdvEventBannerScrollView*
constexpr uint32_t F_RAID_INFO_OPEN = 0x324, F_RAID_ICON = 0x3AC;
constexpr uint32_t MODE_EVENTS = 1;
constexpr uint32_t API_EVENT_BANNERS = 0x12C;   // APIManager: vector<EventBanner>* (56 bytes each; +0x20 the kind)
constexpr uint32_t API_DISCOUNT = 0x44;         // APIManager: the shop discount info (+0x34 bit 2: a Jewel sale)
constexpr uint32_t API_RAID_INFO = 0x148;       // APIManager: RaidBase::Info*
constexpr uint32_t BANNER_SIZE = 56, BANNER_KIND = 0x20;
constexpr uint32_t V_GET_OPACITY = 0x20C;
constexpr uint32_t VT_EventOut = 0xF6C7F4, VT_EventOut1 = 0xF6C820, VT_EventOut2 = 0xF6C84C;   // {this}
constexpr uint32_t VT_DiscountBought = 0xF6C8A4;   // $_23 lambda()#1 {this}
constexpr uint32_t VT_DiscountEvents = 0xF6C8D0, VT_DiscountRaids = 0xF6C8FC, VT_DiscountOut = 0xF6C928;
constexpr uint32_t VT_DiscountEventsErr = 0xF6C980, VT_DiscountRaidsErr = 0xF6C954;
constexpr uint32_t VT_RaidInfoClosed = 0xF6CA5C;   // initStory $_19's answer {this}
constexpr uint32_t VT_ColosseumErr = 0xF6CEA8;
constexpr uint32_t AdvEventBannerScrollView_setSelectItem = 0x587ED9;
constexpr uint32_t BuyMagicStoneDialog_openDiscount = 0x589005;   // (const function<void()>& closed, const function<void()>& bought)
constexpr uint32_t SAS_initColosseum = 0x723F79, SAS_backScene = 0x72834D;
constexpr uint32_t popUpRaidBossInfo = 0x5072F5;               // (const RaidBase::Info&, function<void()> by value)
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t SM_COLOSSEUM_FLAG = 0x30A;

// the Jewel sale popup; bought -> the events and raids again for `owner`
void openDiscount(uint32_t owner) {
    Frame f;
    const uint32_t closed = newEmptyFunction(f);
    const uint32_t bought = newFunction(f, VT_DiscountBought, {owner});
    call(BuyMagicStoneDialog_openDiscount, {closed, bought});
    functionDestroy(bought);
    functionDestroy(closed);
}
// the screen runs out, then the small std::function {vt, this}
void runOut(uint32_t me, uint32_t vt) {
    Frame f;
    const uint32_t fo = newFunction(f, vt, {me});
    call(SAS_changeOutAnimation, {me, fo});
    functionDestroy(fo);
}

// initEventBanner $_24 {this}(index): an event banner picked (the list idle) -> the screen runs out to it
void bannerPicked(uint32_t f, uint32_t indexRef) {
    const uint32_t me = ld32(f + 4);
    if (ld32(me + F_MODE) != MODE_EVENTS) return;
    const uint32_t index = ld32(indexRef);
    const uint32_t scroll = ld32(me + F_SCROLL);
    if (scroll && call(BaseStageScrollView_isScroll, {scroll})) return;
    if (ld8(me + F_BUSY_29E) || !ld8(me + F_LOADING) || !ld8(me + F_CAN_TAP)) return;
    st8(me + F_CAN_TAP, 0);
    call(fn::AudioPlaySystemSEDecide, {});
    call(AdvEventBannerScrollView_setSelectItem, {ld32(me + F_BANNERS), index});
    st32(me + F_BANNER_INDEX, index);
    const uint32_t banners = ld32(call(fn::APIManager_getInstance, {}) + API_EVENT_BANNERS);
    if ((ld32(banners + 4) - ld32(banners)) / BANNER_SIZE <= index) call(ThrowOutOfRange, {});
    const uint32_t kind = ld32(ld32(banners) + BANNER_SIZE * index + BANNER_KIND);
    call(SceneManager_setCanTouchHeader, {sceneManager(), 0});
    st8(me + F_LOADING, 0);
    runOut(me, kind == 1 ? VT_EventOut1 : kind == 2 ? VT_EventOut2 : VT_EventOut);
}
// initEventBanner $_23 {this}: the Jewel sale banner
void discountBanner(uint32_t f) { openDiscount(ld32(f + 4)); }
// initEventBanner $_21 {this, owner}(sender, type): the Jewel sale button (while a sale runs)
void discountButton(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (ld8(me + F_BUSY_29E) || !ld8(me + F_CAN_TAP)) return;
    const uint32_t info = ld32(call(fn::APIManager_getInstance, {}) + API_DISCOUNT);
    if (!info || !(ld8(info + 0x34) & 4)) return;
    call(fn::AudioPlaySystemSEDecide, {});
    openDiscount(ld32(f + 8));
}
// $_23's bought {this}: the screen locked, the events (type 1) again
void discountBought(uint32_t f) {
    const uint32_t me = ld32(f + 4);
    call(SceneManager_setCanTouchHeader, {sceneManager(), 0});
    st8(me + F_LOADING, 1);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame fr;
    const uint32_t fo = newFunction(fr, VT_DiscountEvents, {me});
    call(APIManager_executeStageEventGet, {api, 1, fo});
    functionDestroy(fo);
}
// ... the events arrived {this}(ok): the raid list next (failed: back to the events)
void discountEvents(uint32_t f, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    const uint32_t me = ld32(f + 4);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        failed(me, VT_DiscountEventsErr, MODE_EVENTS);
        return;
    }
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame fr;
    const uint32_t fo = newFunction(fr, VT_DiscountRaids, {me});
    call(APIManager_executeRaidList, {api, fo});
    functionDestroy(fo);
}
// ... the raid list arrived {this}(ok): the screen runs out (failed: back to the events)
void discountRaids(uint32_t f, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    const uint32_t me = ld32(f + 4);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        failed(me, VT_DiscountRaidsErr, MODE_EVENTS);
        return;
    }
    runOut(me, VT_DiscountOut);
}

// initStory $_19 {this}(sender, type): the raid boss icon (shown) -> its info popup
void raidIconTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    if (ld8(me + F_RAID_INFO_OPEN)) return;
    const uint32_t icon = ld32(me + F_RAID_ICON);
    if (!vcall(icon, V_GET_OPACITY, {icon})) return;
    const uint32_t info = ld32(call(fn::APIManager_getInstance, {}) + API_RAID_INFO);
    if (!(ld32(info) | ld32(info + 4))) return;
    st8(me + F_RAID_INFO_OPEN, 1);
    Frame fr;
    const uint32_t closed = newFunction(fr, VT_RaidInfoClosed, {me});
    call(popUpRaidBossInfo, {info, closed});
    functionDestroy(closed);
}

// changeAdventure $_11 {this, adventure}(ok): the Coliseum loaded -> the header back on, the list and the
// Coliseum built; failed -> the API error
void colosseumLoaded(uint32_t f, uint32_t okRef) {
    const uint8_t ok = ld8(okRef);
    const uint32_t me = ld32(f + 4);
    call(fn::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        failed(me, VT_ColosseumErr);
        return;
    }
    call(SceneManager_setCanTouchHeader, {sceneManager(), 1});
    call(SAS_initAdvScrollView, {me, ld32(f + 8)});
    call(SAS_initColosseum, {me});
    st8(me + F_LOADING, ld8(sceneManager() + SM_COLOSSEUM_FLAG));
    st8(me + F_CAN_TAP, 1);
}

// init $_4 {this}(sender, type): Back (the list idle)
void backTouched(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t me = ld32(f + 4);
    const uint32_t scroll = ld32(me + F_SCROLL);
    if (scroll && call(BaseStageScrollView_isScroll, {scroll})) return;
    call(fn::AudioPlaySystemSEDecide, {});
    call(SAS_backScene, {me});
}
}  // namespace

RW_FUNCTION(0x72DBAC, "SceneAdventureSelect::initEventBanner $_24::operator()(unsigned)", bannerPicked);
RW_FUNCTION(0x72E034, "SceneAdventureSelect::initEventBanner $_23::operator()", discountBanner);
RW_FUNCTION(0x72E9AC, "SceneAdventureSelect::initEventBanner $_21::operator()", discountButton);
RW_FUNCTION(0x72E190, "SceneAdventureSelect::initEventBanner $_23 bought::operator()", discountBought);
RW_FUNCTION(0x72E2E4, "SceneAdventureSelect::initEventBanner $_23 events loaded::operator()(bool)", discountEvents);
RW_FUNCTION(0x72E4BC, "SceneAdventureSelect::initEventBanner $_23 raid list loaded::operator()(bool)", discountRaids);
RW_FUNCTION(0x72ECE4, "SceneAdventureSelect::initStory $_19::operator()", raidIconTouched);
RW_FUNCTION(0x730F28, "SceneAdventureSelect::changeAdventure $_11::operator()(bool)", colosseumLoaded);
RW_FUNCTION(0x731952, "SceneAdventureSelect::init $_4::operator() (Back)", backTouched);
