// SceneAdventureSelect: the in / out animations of the panels and the raid result sequence.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

// Slide every panel out (the select / top panels up, the side panels right, the event panels left, the filter
// down, the announce faded), the stage list too; `done` runs when the back button is out.
void SceneAdventureSelect::changeOutAnimation(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    call(lib::Node_stopAllActions, {s->advSelect.node});
    runPanel(lib::AdventurePanel_runSelectOutAnimation, self + 0x374);
    call(lib::Node_stopAllActions, {s->story.node});
    runPanel(lib::AdventurePanel_runTopOutAnimation, self + 0x380);
    call(lib::Node_stopAllActions, {s->event.node});
    runPanel(lib::AdventurePanel_runTopOutAnimation, self + 0x38C);
    call(lib::Node_stopAllActions, {s->colosseum.node});
    runPanel(lib::AdventurePanel_runTopOutAnimation, self + 0x398);
    call(lib::Node_stopAllActions, {s->raid.node});
    runPanel(lib::AdventurePanel_runTopOutAnimation, self + 0x3A4);
    runPanel(lib::AdventurePanel_runRightOutAnimation, self + 0x3B0);
    runPanel(lib::AdventurePanel_runRightOutAnimation, self + 0x3BC);
    runPanel(lib::AdventurePanel_runRightOutAnimation, self + 0x3C8);
    runPanel(lib::AdventurePanel_runRightOutAnimation, self + 0x3D4);
    call(lib::Node_stopAllActions, {s->eventSelect.node});
    runPanel(lib::AdventurePanel_runLeftOutAnimation, self + 0x330);
    call(lib::Node_stopAllActions, {s->eventSelectLeftUI.node});
    runPanel(lib::AdventurePanel_runLeftOutAnimation, self + 0x33C);
    runPanel(lib::AdventurePanel_runFadeOutAnimation, self + 0x2BC);
    runPanel(lib::AdventurePanel_runBottomOutAnimation, self + 0x3E0);
    runPanel(lib::AdventurePanel_runBottomOutAnimation, self + 0x3EC);
    {
        Frame f;
        uint32_t none = newEmptyFunction(f);
        scrollRunOutAnimation(self, none);
        functionDestroy(none);
    }
    callWithCopy(backButtonRunOutAnimation, self, done);
}
RW_FUNCTION(0x71F00C, "SceneAdventureSelect::changeOutAnimation", SceneAdventureSelect::changeOutAnimation);

// The raid boss's state decides what plays before `done`: 1 defeated, 2 escaped, 3 still up (boss animation),
// anything else the reward popup. Then the raid info stays live while a boss with time left is up.
void SceneAdventureSelect::initStageRaidChack(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t ctrl = ld32(api + API_RAID_INFO_CTRL);
    uint32_t info = ld32(api + API_RAID_INFO);
    if (ctrl) call(fn::shared_add, {ctrl});
    int32_t state = int32_t(ld32(info + RAID_INFO_STATE));
    if (state == 1) {
        callWithCopy(raidBossWinAnim, self, done);
    } else if (state == 2) {
        callWithCopy(raidBossEscapeAnim, self, done);
    } else if (state == 3) {
        callWithCopy(raidBossAnim, self, done);
    } else {
        Frame f;
        uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, done});
        uint32_t cb = f.alloc(20);
        heapFunction(cb, VT_initStageRaidChack_52, 0x20, 8, fn::function_vv_move, copy);
        raidBossRewardPopup(self, cb);
        functionDestroy(cb);
        functionDestroy(copy);
    }
    uint8_t active = 0;
    if ((ld32(info) | ld32(info + 4)) != 0 &&
        int32_t(call(lib::Date_calculateIntervalSinceNow, {info + 0x10})) >= 1 && ld32(info + 0x34) != 0)
        active = state == 3;
    s->raidInfoActive = active;
    if (s->flag_29D && ld8(sceneManager() + SM_FLAG_2F6)) st8(sceneManager() + SM_FLAG_2F6, 0);
    if (ctrl) call(fn::shared_release, {ctrl});
}
RW_FUNCTION(0x71F3F4, "SceneAdventureSelect::initStageRaidChack", SceneAdventureSelect::initStageRaidChack);

// Tutorial 31 (the story list) once the story has more than a few stages and the top is shown; else `done`.
void SceneAdventureSelect::openTutorial31(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    uint32_t stories = f.alloc(12);
    call(lib::StoryVector_copy, {stories, ld32(api + API_STORIES)});
    if (s->adventureType == 0 && int32_t(ld32(stories + 4) - ld32(stories)) >= 0x79) {
        uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, done});
        uint32_t result = f.alloc(20);
        SceneUtil::openTutorialPopup(result, 31, copy, 0, 0, 0, 0);
        functionDestroy(result);
        functionDestroy(copy);
    } else if (ld32(done + 16)) {
        call(fn::function_vv_call, {done});
    }
    call(lib::StoryVector_dtor, {stories});
}
RW_FUNCTION(0x71F650, "SceneAdventureSelect::openTutorial31", SceneAdventureSelect::openTutorial31);

// The info panel of the shown AdventureType slides out (the stage list scrolls under it), the announce fades.
void SceneAdventureSelect::infoOutAnimation(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    switch (uint32_t(s->adventureType)) {
    case 0:
    case 6:
        runPanel(lib::AdventurePanel_runInfoOutAnimation, self + 0x380);   // story
        runPanel(lib::AdventurePanel_runInfoOutAnimation, self + 0x3A4);   // raid
        break;
    case 2:
        if (s->deferment) setVisible(s->deferment, 0);
        [[fallthrough]];
    case 3:
    case 4:
        runPanel(lib::AdventurePanel_runInfoOutAnimation, self + 0x38C);   // event
        break;
    case 5:
        if (s->deferment) setVisible(s->deferment, 0);
        runPanel(lib::AdventurePanel_runInfoOutAnimation, self + 0x398);   // colosseum
        break;
    case 7:
        runPanel(lib::AdventurePanel_runInfoOutAnimation, self + 0x374);   // adventure select
        break;
    default:
        break;
    }
    runPanel(lib::AdventurePanel_runFadeOutAnimation, self + 0x2BC);
}
RW_FUNCTION(0x71FA5C, "SceneAdventureSelect::infoOutAnimation", SceneAdventureSelect::infoOutAnimation);

// The story view slides in: back button, the adventure buttons from the right, the filter from below, then the
// stage list; `done` runs when the list is in.
void SceneAdventureSelect::storyInAnimation(Addr self, Addr done) {
    backButtonRunInAnimation(self);
    call(lib::AdventurePanel_runRightInAnimation, {self + 0x3B0});
    call(lib::AdventurePanel_runBottomInAnimation, {self + 0x3E0});
    call(lib::AdventurePanel_runBottomInAnimation, {self + 0x3EC});
    Frame f;
    uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    uint32_t cb = f.alloc(20);
    forwardingLambda(cb, VT_storyInAnimation_48, copy);
    scrollRunInAnimation(self, cb);
    functionDestroy(cb);
    functionDestroy(copy);
}
RW_FUNCTION(0x722F58, "SceneAdventureSelect::storyInAnimation", SceneAdventureSelect::storyInAnimation);

// The stage list, its cursors and arrows slide down; `done` runs when the list is out.
void SceneAdventureSelect::scrollRunOutAnimation(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    if (s->stageScrollView) {
        call(lib::BaseStageScrollView_setEnabledScroll, {s->stageScrollView, 1});
        uint32_t view = s->stageScrollView;
        Frame f;
        uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, done});
        uint32_t cb = f.alloc(20);
        forwardingLambda(cb, VT_scrollRunOutAnimation_58, copy);
        runEased(view, SceneUtil::createBottomOutAnimation(view, 0.0f, 0.0f, cb));
        functionDestroy(cb);
        functionDestroy(copy);
    }
    runPanel(lib::AdventurePanel_runBottomOutAnimation, self + 0x2E8);
    runPanel(lib::AdventurePanel_runBottomOutAnimation, self + 0x2F4);
    runEasedEmpty(s->leftArrow, SceneUtil::createBottomOutAnimation);
    runEasedEmpty(s->rightArrow, SceneUtil::createBottomOutAnimation);
}
RW_FUNCTION(0x725CAC, "SceneAdventureSelect::scrollRunOutAnimation", SceneAdventureSelect::scrollRunOutAnimation);

// The back button slides out to the left; `done` runs at the end.
void SceneAdventureSelect::backButtonRunOutAnimation(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    s->flag_3FD = 0;
    uint32_t back = s->leftUiBack;
    Frame f;
    uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    uint32_t cb = f.alloc(20);
    forwardingLambda(cb, VT_backButtonRunOutAnimation_60, copy);
    runEased(back, SceneUtil::createLeftOutAnimation(back, 0.0f, 0.0f, cb));
    functionDestroy(cb);
    functionDestroy(copy);
}
RW_FUNCTION(0x725EF4, "SceneAdventureSelect::backButtonRunOutAnimation", SceneAdventureSelect::backButtonRunOutAnimation);

// The back button (hidden on the adventure select) slides in from the left; lambda $_59 runs at the end.
void SceneAdventureSelect::backButtonRunInAnimation(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    call(lib::Node_stopAllActions, {s->leftUiBack});
    uint32_t back = s->leftUiBack;
    vcall(back, node::setVisible, {back, s->adventureType != 7 ? 1u : 0u});
    back = s->leftUiBack;
    vcall(back, node::setLocalZOrder, {back, 2});
    back = s->leftUiBack;
    vcall(back, node::setPosition, {back, self + 0x2D0});
    back = s->leftUiBack;
    Frame f;
    uint32_t cb = newFunction(f, VT_backButtonRunInAnimation_59, {self});
    runEased(back, SceneUtil::createLeftInAnimation(back, 0.0f, 0.0f, cb));
    functionDestroy(cb);
}
RW_FUNCTION(0x726014, "SceneAdventureSelect::backButtonRunInAnimation", SceneAdventureSelect::backButtonRunInAnimation);

// The stage list slides up from its saved position (`done` at the end), with the cursors and, when it has
// items, the arrows.
void SceneAdventureSelect::scrollRunInAnimation(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    call(lib::Node_stopAllActions, {s->stageScrollView});
    vcall(s->stageScrollView, node::setPosition, {s->stageScrollView, self + 0x30C});
    uint32_t view = s->stageScrollView;
    Frame f;
    {
        uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, done});
        runEased(view, SceneUtil::createBottomInAnimation(view, 0.0f, 0.0f, copy));
        functionDestroy(copy);
    }
    uint32_t ids = f.alloc(12);
    call(lib::BaseStageScrollView_getItemIDs, {ids, s->stageScrollView});
    uint32_t hasItems = ld32(ids + 4) != ld32(ids) ? 1 : 0;
    call(lib::IntVector_dtor, {ids});
    setVisible(s->cursor.node, hasItems);
    call(lib::AdventurePanel_runBottomInAnimation, {self + 0x2E8});
    setVisible(s->cursorArrow.node, hasItems);
    call(lib::AdventurePanel_runBottomInAnimation, {self + 0x2F4});
    uint32_t ids2 = f.alloc(12);
    call(lib::BaseStageScrollView_getItemIDs, {ids2, s->stageScrollView});
    uint32_t begin = ld32(ids2), end = ld32(ids2 + 4);
    call(lib::IntVector_dtor, {ids2});
    uint32_t scrollable = end != begin ? 1 : 0;
    call(lib::BaseStageScrollView_setEnabledScroll, {s->stageScrollView, scrollable});
    setVisible(s->leftArrow, scrollable);
    call(lib::Node_stopAllActions, {s->leftArrow});
    vcall(s->leftArrow, node::setPosition, {s->leftArrow, self + 0x314});
    runEasedEmpty(s->leftArrow, SceneUtil::createBottomInAnimation);
    setVisible(s->rightArrow, end != begin ? 1 : 0);
    call(lib::Node_stopAllActions, {s->rightArrow});
    vcall(s->rightArrow, node::setPosition, {s->rightArrow, self + 0x31C});
    runEasedEmpty(s->rightArrow, SceneUtil::createBottomInAnimation);
}
RW_FUNCTION(0x726284, "SceneAdventureSelect::scrollRunInAnimation", SceneAdventureSelect::scrollRunInAnimation);

// IInOutAnimation: the side shades slide in.
void SceneAdventureSelect::runInAnimation(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    runEasedEmpty(s->shadeLeft, SceneUtil::createLeftInAnimation);
    runEasedEmpty(s->shadeRight, SceneUtil::createRightInAnimation);
}
RW_FUNCTION(0x728548, "SceneAdventureSelect::runInAnimation", SceneAdventureSelect::runInAnimation);

// IInOutAnimation: the header is locked, the shades and every panel slide out, then `done`.
void SceneAdventureSelect::runOutAnimationMain(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    call(lib::SceneManager_setCanTouchHeader, {sceneManager(), 0});
    runEasedEmpty(s->shadeLeft, SceneUtil::createLeftOutAnimation);
    runEasedEmpty(s->shadeRight, SceneUtil::createRightOutAnimation);
    Frame f;
    uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    uint32_t cb = f.alloc(20);
    forwardingLambda(cb, VT_runOutAnimationMain_68, copy);
    changeOutAnimation(self, cb);
    functionDestroy(cb);
    functionDestroy(copy);
}
RW_FUNCTION(0x72865C, "SceneAdventureSelect::runOutAnimationMain", SceneAdventureSelect::runOutAnimationMain);
