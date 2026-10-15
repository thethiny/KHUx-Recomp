// SceneAdventureSelect: switching between the adventure types (story, events, colosseum, adventure select), the
// stage scroll view and the story panel.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

namespace {

// A widget's ImageButton overlay (ImageButton "But10" named `name`) calling lambda `vtable` [self].
void attachStartButton(uint32_t self, uint32_t imageView, uint32_t name, uint32_t nameLen, uint32_t vtable) {
    Frame f;
    uint32_t file = newStr(f, STR_But10, 5);
    uint32_t button = call(lib::ImageButton_create, {file, 0});
    strFree(file);
    call(lib::ImageButton_setTargetImageView, {button, imageView});
    uint32_t key = newStr(f, name, nameLen);
    vcall(imageView, node::addChildZName, {imageView, button, 0, key});
    strFree(key);
    uint32_t cb = newFunction(f, vtable, {self});
    call(lib::ImageButton_addTouchEventListener, {button, cb});
    functionDestroy(cb);
}

// SceneUtil::settingTitleLabel(node, literal name, text id)
void titleText(uint32_t node, uint32_t name, uint32_t len, int32_t textId) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    SceneUtil::settingTitleLabel(node, s, textId);
    strFree(s);
}

// view->swap(member, lambda{vtable, captures}): install a callback, destroy the one it replaces
void swapCallback(uint32_t swapFn, uint32_t member, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    uint32_t fo = newFunction(f, vtable, captures);
    call(swapFn, {fo, member});
    functionDestroy(fo);
}

}  // namespace

// Show adventure `type` (0 story, 1 event list, 2..4 an event, 5 colosseum, 7 adventure select; 6 = story top
// without changing the type). After a raid return (flag_29D) the story is forced. `flag` goes to the story's
// in-animation.
void SceneAdventureSelect::changeAdventure(Addr self, int32_t type, uint32_t flag) {
    Self *s = rw::host_ptr<Self>(self);
    if (s->flag_29D) type = 0;
    s->adventureType = type == 6 ? 0 : type;
    switch (uint32_t(type)) {
    case 0: {
        call(lib::SceneManager_setCanTouchHeader, {sceneManager(), 1});
        initAdvScrollView(self, 0);
        initStory(self);
        uint32_t app = call(fn::AppState_getInstance, {});
        Frame f;
        uint32_t filter = f.alloc(8);
        call(lib::AppState_getAdventureSelectFilterState, {filter, app});
        int32_t textId = ld8(ld32(filter)) ? 100100012 : 100100035;
        uint32_t text = s->txtFilter;
        {
            Frame g;
            uint32_t sp = textData(g, textId);
            setTextC(text, textDataStr(sp));
            sharedRelease(sp);
        }
        s->flag_3FC = ld8(sceneManager() + SM_FILTER_ENABLED);
        sharedRelease(filter);
        s->flag_325 = 1;
        uint32_t cb = f.alloc(20);
        st32(cb, VT_changeAdventure_12);
        st8(cb + 4, uint8_t(flag));
        st32(cb + 16, cb);
        storyInAnimation(self, cb);
        functionDestroy(cb);
        return;
    }
    case 1:
        call(lib::SceneManager_setCanTouchHeader, {sceneManager(), 1});
        initAdvScrollView(self, 1);
        initEventBanner(self, 1);
        s->flag_3FC = ld8(sceneManager() + SM_FILTER_ENABLED);
        break;
    case 2:
    case 3:
    case 4:
        call(lib::SceneManager_setCanTouchHeader, {sceneManager(), 1});
        initAdvScrollView(self, type);
        initEvent(self);
        s->flag_3FC = ld8(sceneManager() + SM_FILTER_ENABLED);
        break;
    case 5: {
        call(lib::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        uint32_t cb = newFunction(f, VT_changeAdventure_11, {self, 5});
        call(lib::APIManager_executeColosseumGet, {api, cb});
        functionDestroy(cb);
        break;
    }
    case 7:
        call(lib::SceneManager_setCanTouchHeader, {sceneManager(), 1});
        initAdvScrollView(self, 7);
        initAdvSelect(self);
        s->flag_3FC = 1;
        break;
    default:
        s->flag_3FC = 1;
        break;
    }
    s->flag_325 = 1;
}
RW_FUNCTION(0x71FC3C, "SceneAdventureSelect::changeAdventure", SceneAdventureSelect::changeAdventure);

// The stage list (AdvStageScrollView in Adv_Panel_Area/Area, a third of the screen high): created once with its
// callbacks, then filled for `type`.
void SceneAdventureSelect::initAdvScrollView(Addr self, int32_t type) {
    Self *s = rw::host_ptr<Self>(self);
    if (s->stageScrollView) {
        if (ld8(sceneManager() + SM_FLAG_2F6)) {
            s->flag_2E4 = 1;
            s->flag_29C = uint8_t(call(lib::AdvStageScrollView_initNewStage, {s->stageScrollView, s->cursor.node}));
            st8(sceneManager() + SM_FLAG_2F6, 0);
        }
    } else {
        s->flag_2E4 = type != 7;
        uint32_t area = findNode(s->advPanelArea, STR_Area, 4);
        s->stageScrollView = call(lib::AdvStageScrollView_create, {});
        uint32_t setContentSize = ld32(ld32(area) + node::setContentSize);
        float width = visibleSize().width;
        float height = ldf(vcall(area, node::getContentSize, {area}) + 4) / 3.0f;
        {
            Frame f;
            uint32_t size = f.alloc(8);
            call(lib::Size_ctor, {size, fbits(width), fbits(height)});
            call(setContentSize, {area, size});
        }
        SceneUtil::leftAdsorptionNode(area);
        uint32_t view = s->stageScrollView;
        uint32_t viewSetContentSize = ld32(ld32(view) + node::setContentSize);
        uint32_t areaSize = vcall(area, node::getContentSize, {area});
        call(viewSetContentSize, {view, areaSize});
        vcall(area, node::addChildZ, {area, s->stageScrollView, 0});
        uint32_t pos = vcall(s->stageScrollView, node::getPosition, {s->stageScrollView});
        s->field_310 = ldf(pos + 4);
        s->field_30C = ldf(pos);
        uint32_t mgr = sceneManager();
        if (type != 7 && ld8(mgr + SM_FLAG_2F6)) {
            s->flag_29C = uint8_t(call(lib::AdvStageScrollView_initNewStage, {s->stageScrollView, s->cursor.node}));
            st8(sceneManager() + SM_FLAG_2F6, 0);
        }
        swapCallback(lib::FunctionVI_swap, s->stageScrollView + VIEW_CALLBACK_640, VT_initAdvScrollView_39, {self});
        swapCallback(lib::FunctionVV_swap, s->stageScrollView + VIEW_CALLBACK_560, VT_initAdvScrollView_40, {self});
        swapCallback(lib::FunctionVI_swap, s->stageScrollView + VIEW_CALLBACK_500, VT_initAdvScrollView_41, {});
        swapCallback(lib::FunctionVF_swap, s->stageScrollView + VIEW_CALLBACK_530, VT_initAdvScrollView_42, {self});
    }
    uint32_t view = s->stageScrollView;
    if (type != 0) {
        call(lib::AdvStageScrollView_resetScrollPosition, {view});
        if (type == 5) setStageScrollViewItemsColosseum(self);
        else if (uint32_t(type - 2) <= 2) setStageScrollViewItemsEvent(self);
        initSelectStage(self, type);
        return;
    }
    uint32_t raidUp = call(lib::AdvStageScrollView_initRaidBoss, {view});
    call(lib::AdvStageScrollView_resetScrollPosition, {s->stageScrollView});
    setStageScrollViewItemsStory(self);
    initSelectStage(self, 0);
    if (raidUp && s->flag_29D) {
        s->flag_29D = 0;
        call(lib::BaseStageScrollView_migrationMaxPosition, {s->stageScrollView});
    }
}
RW_FUNCTION(0x71FF14, "SceneAdventureSelect::initAdvScrollView", SceneAdventureSelect::initAdvScrollView);

// The story panel: ticker message, the quest / raid start buttons, the adventure buttons panel (created once,
// event / colosseum / relief buttons hidden), then the stage info.
void SceneAdventureSelect::initStory(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    if (s->buttonFilter) setVisible(s->buttonFilter, 1);
    {
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t sp = textData(f, 100620049);
        uint32_t msg = newStr(f, textDataStr(sp));
        call(lib::SceneManager_changeTickerMessage, {mgr, msg});
        strFree(msg);
        sharedRelease(sp);
    }
    st32(sceneManager() + SM_ADVENTURE_MODE, 1);

    uint32_t storyInfo = findNode(s->story.node, STR_RaidInfo, 8);
    uint32_t questStart = findNode(storyInfo, STR_Button_QuestStart, 0x11);
    titleText(questStart, STR_Txt_QuestStart, 0xE, 102900001);
    if (!findNode(questStart, STR_ImageButton_QuestStart, 0x16))
        attachStartButton(self, questStart, STR_ImageButton_QuestStart, 0x16, VT_initStory_18);

    uint32_t raidInfo = findNode(s->raid.node, STR_RaidInfo, 8);
    uint32_t raidStart = findNode(raidInfo, STR_Button_RaidStart, 0x10);
    uint32_t bossIcon = findNode(raidInfo, STR_Raid_Boss_Icon, 0xE);
    addTouch(bossIcon, VT_initStory_19, {self});
    titleText(raidStart, STR_Txt_RaidStart, 0xD, 102900002);
    if (!findNode(raidStart, STR_ImageButton_RaidStart, 0x15))
        attachStartButton(self, raidStart, STR_ImageButton_RaidStart, 0x15, VT_initStory_20);

    if (!s->panel_3B0.node) {
        call(lib::AdventurePanel_createWidget, {self + 0x3B0, s->advButton, STR_cocostudio_publish_AdventureTop2_Adv_But});
        uint32_t ev = findNode(s->panel_3B0.node, STR_Button_EV, 9);
        setVisible(ev, 0);
        uint32_t colosseum = findNode(s->panel_3B0.node, STR_Button_Colosseum, 0x10);
        setVisible(colosseum, 0);
        uint32_t relief = findNode(s->panel_3B0.node, STR_Button_Relief, 0xD);
        findNode(relief, STR_Button_Relief_Ico, 0x11);
        setVisible(relief, 0);
    }
    changeStageInfo(self);
}
RW_FUNCTION(0x720270, "SceneAdventureSelect::initStory", SceneAdventureSelect::initStory);
