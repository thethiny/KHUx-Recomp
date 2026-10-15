// SceneAdventureSelect::init: loads AdventureTop2Scene_ver130.json, finds every panel and button, hooks the touch
// callbacks, picks the AdventureType the scene opens on (from how SceneManager says it was entered) and slides
// everything out, ready for the first runInAnimation.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

namespace {

// An ImageButton over the first child of `arrow` (the arrow's touch area), calling lambda `vtable` [self].
void attachArrowButton(uint32_t self, uint32_t arrow, uint32_t vtable) {
    uint32_t children = vcall(arrow, node::getChildren, {arrow});
    uint32_t target = ld32(ld32(children));
    Frame f;
    uint32_t file = newStr(f, STR_EMPTY, 0);
    uint32_t button = call(lib::ImageButton_create, {file, 1});
    strFree(file);
    call(lib::ImageButton_setTargetView, {button, target, fbits(1.0f)});
    uint32_t anchor = f.alloc(8);
    call(lib::Vec2_ctorXY, {anchor, fbits(-0.5f), fbits(-0.5f)});
    call(lib::ImageButton_setTargetAnchorPoint, {button, anchor});
    call(lib::Vec2_dtor, {anchor});
    vcall(target, node::addChild, {target, button});
    uint32_t cb = newFunction(f, vtable, {self});
    call(lib::ImageButton_addTouchEventListener, {button, cb});
    functionDestroy(cb);
}

// panel.node: transparent and shown (the in-animations fade it in)
void hideForFadeIn(uint32_t panelNode) {
    setOpacity(panelNode, 0);
    setVisible(panelNode, 1);
}

// Armature::create("Cursor_Anim_QuestStart")
uint32_t questStartArmature() {
    Frame f;
    uint32_t name = newStr(f, STR_Cursor_Anim_QuestStart, 0x16);
    uint32_t armature = call(lib::Armature_create, {name});
    strFree(name);
    return armature;
}

void addArmatureFileInfo(uint32_t file, uint32_t len) {
    uint32_t mgr = call(lib::ArmatureDataManager_getInstance, {});
    Frame f;
    uint32_t s = newStr(f, file, len);
    call(lib::ArmatureDataManager_addArmatureFileInfo, {mgr, s});
    strFree(s);
}

}  // namespace

uint32_t SceneAdventureSelect::init(Addr self) {
    Self *s = rw::host_ptr<Self>(self);
    s->flag_29F = ld8(sceneManager() + SM_FLAG_2F4);
    if (call(lib::Layer_init, {self}) != 1) return 0;

    // onEnter / onExit callbacks and the back key
    assignFunction(self + NODE_ON_ENTER, VT_init_1, {self});
    assignFunction(self + NODE_ON_EXIT, VT_init_2, {self});
    {
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t cb = newFunction(f, VT_init_3, {self});
        call(lib::SceneManager_setOnBackKeyCallback, {mgr, cb});
        functionDestroy(cb);
    }
    call(lib::SceneManager_setBackButtonType, {sceneManager(), 2});
    call(lib::SceneManager_showHeader, {sceneManager()});
    {
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t bg = newStr(f, STR_img_ui_Equip_DeckBG_01_png, 0x1A);
        call(lib::SceneManager_changeBackground, {mgr, 0, bg});
        strFree(bg);
    }
    {
        Frame f;
        uint32_t file = newStr(f, STR_cocostudio_publish_AdventureTop2Scene_ve, 0x31);
        s->root = SceneUtil::createNodeWithSceneFile(file);
        strFree(file);
    }
    vcall(self, node::addChild, {self, s->root});
    {
        uint32_t mgr = sceneManager();
        Frame f;
        uint32_t none = newEmptyFunction(f);
        call(lib::SceneManager_hideTitleHeader, {mgr, none});
        functionDestroy(none);
    }
    st8(sceneManager() + SM_FLAG_2A8, 0);

    searchWidget(self + 0x2BC, s->root, STR_Announce);
    setOpacity(s->announce.node, 0);
    setVisible(s->announce.node, 0);
    uint32_t leftUI = findNode(s->root, STR_LeftUI, 6);
    setVisible(leftUI, 1);
    uint32_t rightUI = findNode(s->root, STR_RightUI, 7);
    setVisible(rightUI, 1);
    s->buttonFilter = findNode(s->root, STR_RightUI_Button_Filter, 0x15);
    if (s->buttonFilter) setVisible(s->buttonFilter, 0);

    // the back button's panel (its position kept for the in / out animations)
    s->leftUiBack = findNode(s->root, STR_LeftUI_Buck, 0xB);
    uint32_t pos = vcall(s->leftUiBack, node::getPosition, {s->leftUiBack});
    s->leftUiBackY = ldf(pos + 4);
    s->leftUiBackX = ldf(pos);
    setVisible(s->leftUiBack, 0);
    uint32_t back = findNode(s->leftUiBack, STR_Button_Back, 0xB);
    SceneUtil::backBtnSetPosY(back);
    addTouch(back, VT_init_4, {self});

    // the stage filter button
    searchWidget(self + 0x3E0, rightUI, STR_Button_Filter);
    searchWidget(self + 0x3EC, rightUI, STR_Txt_Filter);
    uint32_t filterButton = findNode(rightUI, STR_Button_Filter, 0xD);
    s->txtFilter = findNode(rightUI, STR_Txt_Filter, 0xA);
    SceneUtil::setShadow(s->txtFilter);
    setTextId(s->txtFilter, 100100035);
    addTouch(filterButton, VT_init_5, {self});
    int32_t phase = SceneUtil::getNexTutorialphase();
    uint32_t filterUnlocked = sceneManager() + SM_FILTER_ENABLED;
    if (phase > 994) {
        if (!ld8(filterUnlocked)) st8(sceneManager() + SM_FILTER_ENABLED, 1);
        vcall(filterButton, vslot::widgetSetEnabled, {filterButton, 1});
    } else {
        st8(filterUnlocked, 0);
        vcall(filterButton, vslot::widgetSetEnabled, {filterButton, 0});
    }

    // cursors and the stage list arrows
    searchWidget(self + 0x2E8, s->root, STR_Cursor);
    setVisible(s->cursor.node, 0);
    searchWidget(self + 0x2F4, s->root, STR_Cursor_Arrow);
    setVisible(s->cursorArrow.node, 0);
    s->leftArrow = findNode(leftUI, STR_Left_arrow, 0xA);
    pos = vcall(s->leftArrow, node::getPosition, {s->leftArrow});
    s->leftArrowY = ldf(pos + 4);
    s->leftArrowX = ldf(pos);
    setVisible(s->leftArrow, 0);
    s->rightArrow = findNode(rightUI, STR_Right_arrow, 0xB);
    pos = vcall(s->rightArrow, node::getPosition, {s->rightArrow});
    s->rightArrowY = ldf(pos + 4);
    s->rightArrowX = ldf(pos);
    setVisible(s->rightArrow, 0);
    attachArrowButton(self, s->leftArrow, VT_init_6);
    attachArrowButton(self, s->rightArrow, VT_init_7);

    s->shadeLeft = findNode(s->root, STR_Shade_Left, 0xA);
    SceneUtil::leftAdsorptionNode(s->shadeLeft);
    s->shadeRight = findNode(s->root, STR_Shade_Right, 0xB);
    SceneUtil::rightAdsorptionNode(s->shadeRight);

    // the adventure buttons, pinned to the right edge
    s->advButton = findNode(s->root, STR_Adv_Button, 0xA);
    setVisible(s->advButton, 1);
    vcall(s->advButton, node::removeAllChildren, {s->advButton});
    SceneUtil::rightAdsorptionNode(s->advButton);
    {
        uint32_t button = s->advButton;
        float x = bitsf(vcall(button, node::getPositionX, {button}));
        float margin = SceneUtil::getSideMargin();
        vcall(button, node::setPositionX, {button, fbits(x + margin * -0.5f)});
        button = s->advButton;
        float y = bitsf(vcall(button, node::getPositionY, {button}));
        float hmargin = SceneUtil::getSideHeightMargin();
        vcall(button, node::setPositionY, {button, fbits(y + hmargin)});
    }
    s->advPanelArea = findNode(s->root, STR_Adv_Panel_Area, 0xE);
    setVisible(s->advPanelArea, 1);
    s->deferment = findNode(s->root, STR_deferment, 9);
    setVisible(s->deferment, 0);

    searchWidget(self + 0x374, s->root, STR_Adv_Select);
    hideForFadeIn(s->advSelect.node);
    uint32_t quest1 = findNode(s->advSelect.node, STR_QuestBut1, 9);
    uint32_t quest2 = findNode(s->advSelect.node, STR_QuestBut2, 9);
    uint32_t quest3 = findNode(s->advSelect.node, STR_QuestBut3, 9);
    uint32_t quest4 = findNode(s->advSelect.node, STR_QuestBut4, 9);
    vcall(quest1, vslot::widgetSetTouchEnabled, {quest1, 0});
    vcall(quest2, vslot::widgetSetTouchEnabled, {quest2, 0});
    vcall(quest3, vslot::widgetSetTouchEnabled, {quest3, 0});
    vcall(quest4, vslot::widgetSetTouchEnabled, {quest4, 0});
    searchWidget(self + 0x380, s->root, STR_Story);
    hideForFadeIn(s->story.node);
    searchWidget(self + 0x38C, s->root, STR_Event);
    hideForFadeIn(s->event.node);
    searchWidget(self + 0x398, s->root, STR_Colosseum);
    hideForFadeIn(s->colosseum.node);
    searchWidget(self + 0x3A4, s->root, STR_Raid);
    hideForFadeIn(s->raid.node);
    searchWidget(self + 0x330, s->root, STR_Event_Select);
    hideForFadeIn(s->eventSelect.node);
    searchWidget(self + 0x33C, s->root, STR_Event_Select_LeftUI);
    hideForFadeIn(s->eventSelectLeftUI.node);

    // the AdventureType to open on
    int32_t type = 0;
    int32_t mode = int32_t(ld32(sceneManager() + SM_ADVENTURE_MODE));
    if (uint32_t(mode - 2) < 3) {                      // entered for an event
        bool lookUp = true;
        if (ld32(sceneManager() + SM_EVENT_ID) != 0) {
            uint32_t events = ld32(call(fn::APIManager_getInstance, {}) + API_STAGE_EVENTS);
            uint32_t begin = ld32(events);
            bool found = false;
            if (ld32(events + 4) != begin) {
                uint32_t k = 0, off = 0xC;
                for (;;) {
                    uint32_t id = ld32(begin + off);
                    if (id == ld32(sceneManager() + SM_EVENT_ID)) { found = true; break; }
                    begin = ld32(events);
                    uint32_t n = eventCount(events);
                    off += EVENT_SIZE;
                    k++;
                    if (k >= n) break;
                }
                if (found) s->eventIndex = int32_t(k);
            }
            if (!found) {                              // the event is gone: open the event list
                st32(sceneManager() + SM_EVENT_ID, 0);
                type = 1;
                lookUp = false;
            }
        }
        if (lookUp) {
            uint32_t events = ld32(call(fn::APIManager_getInstance, {}) + API_STAGE_EVENTS);
            uint32_t index = uint32_t(s->eventIndex);
            if (eventCount(events) <= index) call(lib::THROW_OUT_OF_RANGE, {events});
            int32_t kind = int32_t(ld32(ld32(events) + index * EVENT_SIZE + 0x20));
            type = kind == 2 ? 4 : (kind == 1 ? 3 : 2);
            if (!ld8(sceneManager() + SM_FLAG_2F6)) st8(sceneManager() + SM_FLAG_2F6, 0);
        }
    } else if (mode == 5) {
        type = 5;
    } else if (mode == 9) {
        type = 7;
    }

    // the AP cursor in the raid title, the quest start effects on the story / event / raid / colosseum panels
    uint32_t title = findNode(s->raid.node, STR_Title, 5);
    uint32_t areaAP = findNode(title, STR_Area_AP, 7);
    addArmatureFileInfo(STR_cocostudio_publish_Cursor_Anim_AP_Export, 0x2C);
    {
        Frame f;
        uint32_t name = newStr(f, STR_Cursor_Anim_AP, 0xE);
        s->cursorAnimAP = call(lib::Armature_create, {name});
        strFree(name);
    }
    vcall(areaAP, node::addChild, {areaAP, s->cursorAnimAP});
    addArmatureFileInfo(STR_cocostudio_publish_Cursor_Anim_QuestStar, 0x34);
    uint32_t storyInfo = findNode(s->story.node, STR_RaidInfo, 8);
    uint32_t eventInfo = findNode(s->event.node, STR_RaidInfo, 8);
    uint32_t raidInfo = findNode(s->raid.node, STR_RaidInfo, 8);
    uint32_t colosseumInfo = findNode(s->colosseum.node, STR_ColosseumInfo, 0xD);
    uint32_t effects[4];
    for (uint32_t &e : effects) e = questStartArmature();
    uint32_t points[4];
    points[0] = findNode(storyInfo, STR_Area_Point, 0xA);
    points[1] = findNode(eventInfo, STR_Area_Point, 0xA);
    points[2] = findNode(raidInfo, STR_Area_Point, 0xA);
    points[3] = findNode(colosseumInfo, STR_Area_Point, 0xA);
    for (uint32_t e : effects) {
        uint32_t animation = vcall(e, vslot::armatureGetAnimation, {e});
        Frame f;
        uint32_t name = newStr(f, STR_Animation1, 0xA);
        vcall(animation, vslot::animationPlay, {animation, name, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(name);
    }
    for (int k = 0; k < 4; k++) {
        Frame f;
        uint32_t name = newStr(f, STR_ButtonEffect, 0xC);
        vcall(points[k], node::addChildZName, {points[k], effects[k], 0, name});
        strFree(name);
    }

    // everything out (instantly), then the raid result / tutorial 31 before the first in-animation
    {
        Frame f;
        uint32_t cb = newFunction(f, VT_init_8, {});
        changeOutAnimation(self, cb);
        functionDestroy(cb);
    }
    {
        Frame f;
        if (SceneUtil::getNexTutorialphase() == 995) {
            uint32_t cb = newFunction(f, VT_init_9, {self, uint32_t(type)});
            initStageRaidChack(self, cb);
            functionDestroy(cb);
        } else {
            uint32_t cb = newFunction(f, VT_init_10, {self, uint32_t(type)});
            openTutorial31(self, cb);
            functionDestroy(cb);
        }
    }
    vcall(self, vslot::runInAnimation, {self});
    return 1;
}
RW_FUNCTION(0x71D4E4, "SceneAdventureSelect::init", SceneAdventureSelect::init);
