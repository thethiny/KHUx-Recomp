// StageUI: life cycle, the HUD parts it creates, the heading banners and the score counters.
#include "StageUI.hpp"

using namespace scene;

namespace {
// cocos2d / cocostudio / library
constexpr uint32_t Ref_ctor = rwlib::cocos2d::Ref::ctor_base;
constexpr uint32_t Ref_dtor = rwlib::cocos2d::Ref::dtor_base;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v;                  // Vec2::Vec2(): (0, 0)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t Widget_getCustomSize = rwlib::cocos2d::ui::Widget::getCustomSize;      // -> const Size&
constexpr uint32_t ActionObject_stop = rwlib::cocostudio::ActionObject::stop;
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t ScaleTo_create = rwlib::cocos2d::ScaleTo::create__ff;            // (float duration, float scale)
constexpr uint32_t EaseBackOut_create = rwlib::cocos2d::EaseBackOut::create;
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t LWFNode_create = rwlib::cocos2d::LWFNode::create;            // (const char* path, void*)
constexpr uint32_t AppGetGrameFrameHeight = 0x799D21;
constexpr uint32_t LWFUtil_preloadResources = 0x491361;  // (const std::string&)
// game
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_load = 0x5BEEF5;        // (manager, ResourceID)
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;    // (shared_ptr<Layer> out, manager, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;        // (layer, ResourceID, int, int)
constexpr uint32_t MedalDeck_create = 0x75E8D5;
constexpr uint32_t MedalDeck_getCurrentDefense = 0x5DFFFD;
constexpr uint32_t CarouselDeck_create = 0x75E949;
constexpr uint32_t PlayerStatusIndicator_create = 0x5E143D;  // (a, b, const Avatar::Style&)
constexpr uint32_t PlayerStatusIndicator_setHP = 0x5E3119;
constexpr uint32_t PlayerStatusIndicator_setCP = 0x5E34D9;
constexpr uint32_t PlayerStatusIndicator_setDef = 0x5E3B99;
constexpr uint32_t LWFNodeGroup_create = 0x5D4DD1;       // (const char** begin, const char** end, int)
constexpr uint32_t LWFNodeGroup_preloadResources = 0x5D521D;
constexpr uint32_t LWFNodeGroup_change = 0x5D520D;       // (group, Heading)
constexpr uint32_t LuxBoard_create = 0x75F4F9;
constexpr uint32_t LuxBoard_play = 0x5D3A45;             // (board, bool)

// cocos2d::ui::Widget virtual slots (byte offsets)
constexpr uint32_t Widget_setTouchEnabled = 0x29C;
constexpr uint32_t Node_cleanupAndRemove = 0x124;        // removeFromParent

// sounds
constexpr uint32_t SE_LUX = 502000110;
constexpr uint32_t SE_MANNY = 502000060;
constexpr uint32_t SE_ITEM = 502000070;

// string literals
constexpr uint32_t STR_LuxNode = 0xE074A1;
constexpr uint32_t STR_Icon_RareItem = 0xE074A9;
constexpr uint32_t STR_Icon_Item = 0xE074B7;
constexpr uint32_t STR_ActionFieldJson = 0xE074C1;       // cocostudio/publish/ActionMap_ActionField.json
constexpr uint32_t STR_StatusFieldJson = 0xE074EF;       // cocostudio/publish/ActionMap_StatusField.json
constexpr uint32_t STR_ScoreField_RareItem = 0xE0751D;
constexpr uint32_t STR_ScoreField_Item = 0xE07531;
constexpr uint32_t STR_ScoreField_Manny = 0xE07541;
constexpr uint32_t STR_ScoreField_Lux = 0xE07552;
constexpr uint32_t STR_Item_Label = 0xE07561;
constexpr uint32_t STR_RareItem_Label = 0xE0756C;
constexpr uint32_t STR_Manny_Label = 0xE0757B;
constexpr uint32_t STR_Lux_Label = 0xE07587;
constexpr uint32_t STR_Zero = 0xE01C01;                  // "0"
constexpr uint32_t STR_AttributeJson = 0xE07591;         // cocostudio/publish/ActionMap_AttributeCompatibility.json
constexpr uint32_t STR_LuxNodeJson = 0xE075CA;           // cocostudio/publish/ActionMap_LuxNode.json
constexpr uint32_t STR_HeadingLwfFmt = 0xE07686;         // "lwf/heading/%s/%s.lwf"
constexpr uint32_t STR_PercentD = 0xDF86DE;              // "%d"

// The heading LWF files: LWFNodeGroup takes the 7 names at HEADING_FILES; HEADING_EXTRAS lists the 2 extra
// banners (name, vertical offset factor) loaded as separate LWF nodes.
constexpr uint32_t HEADING_FILES = 0xFEF1E0;             // const char*[7] ("lwf/heading/battle_start"...)
constexpr uint32_t HEADING_FILE_COUNT = 7;
constexpr uint32_t HEADING_EXTRAS = 0xF6E2E8;            // {const char* name, float offset}[2]
constexpr uint32_t HEADING_EXTRA_COUNT = 2;

// LWFNodeGroup fields
constexpr uint32_t GROUP_LAST_SHOWN = 636;               // int: last heading shown (-1 none)
constexpr uint32_t GROUP_CURRENT = 648;                  // LWFNode* playing

constexpr uint32_t APPSTATE_STAGE_STATE = 32;            // AppState + 32: StageState*

uint32_t appState() { return call(fn::AppState_getInstance, {}); }

// widget = GUIReader::widgetFromJsonFile(json), retained
uint32_t loadWidget(uint32_t json) {
    uint32_t reader = call(GUIReader_getInstance, {});
    uint32_t w = call(GUIReader_widgetFromJsonFile, {reader, json});
    call(Ref_retain, {w});
    return w;
}

void setPosition(uint32_t node, float x, float y) {
    Frame f;
    vcall(node, node::setPosition, {node, newVec2(f, x, y)});
}

Size customSize(uint32_t widget) {
    uint32_t s = call(Widget_getCustomSize, {widget});
    return Size{ldf(s), ldf(s + 4)};
}

// Scale pop of a score counter: 1.1, 0.95, 1.0 with ease-back-out.
uint32_t shake(uint32_t node) {
    call(Node_stopAllActions, {node});
    uint32_t a = call(EaseBackOut_create, {call(ScaleTo_create, {scene::fbits(0.0833333358f), scene::fbits(1.1f)})});
    uint32_t b = call(EaseBackOut_create, {call(ScaleTo_create, {scene::fbits(0.05f), scene::fbits(0.95f)})});
    uint32_t c = call(EaseBackOut_create, {call(ScaleTo_create, {scene::fbits(0.0333333351f), scene::fbits(1.0f)})});
    uint32_t seq = call(Sequence_create, {a, b, c, 0});
    return call(Node_runAction, {node, seq});
}

// audio::Manager::getInstance()->getLayer(0)->playSE(id)
void playSE(uint32_t id) {
    Frame f;
    uint32_t layer = f.alloc(8);
    call(audio_Manager_getLayer, {layer, call(audio_Manager_getInstance, {}), 0});
    call(audio_Layer_playSE, {ld32(layer), id, 0, 0});
    sharedRelease(layer);
}

// The count shown by one score counter: when it changed, print it, pop the counter and play its sound.
void updateCounter(StageUI *self, uint32_t label, rw::Field<int32_t> &shown, int32_t value,
                   uint32_t (*shakeFn)(StageUI *), uint32_t se) {
    if (!label || shown == value) return;
    shown = value;
    {
        Frame f;
        uint32_t buf = stringBuffer(f, {STR_PercentD, uint32_t(value)});
        uint32_t s = newStr(f, buf);
        call(fn::Text_setString, {label, s});
        strFree(s);
    }
    shakeFn(self);
    playSE(se);
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
StageUI *StageUI::create() {
    StageUI *ui = rw::host_ptr<StageUI>(call(fn::operator_new, {SIZE}));
    construct(ui);
    uint32_t a = rw::guest_addr(ui);
    if (vcall(a, 0x08, {a}) == 1) {          // init()
        call(Ref_autorelease, {a});
        return ui;
    }
    vcall(a, 0x04, {a});                      // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x5FA212, "StageUI::create", StageUI::create);

StageUI *StageUI::construct(StageUI *self) {
    uint32_t a = rw::guest_addr(self);
    call(Ref_ctor, {a});
    self->vtable = VTABLE;
    for (uint32_t off = 20; off < 104; off += 4) rw::set<uint32_t>(self, off, 0);   // memset(+20, 0, 0x54)
    call(Vec2_ctor, {a + 104});
    self->battleTurn = 0;
    rw::set<uint32_t>(self, 136, 0);          // tutorialCallback: empty std::function
    self->tutorialWidget = 0;
    return self;
}
RW_FUNCTION(0x5FB824, "StageUI::StageUI", StageUI::construct);

StageUI *StageUI::destroy(StageUI *self) {
    uint32_t a = rw::guest_addr(self);
    self->vtable = VTABLE;
    // release the retained HUD parts (+20..+44) and the heading group
    const uint32_t owned[] = {20, 24, 28, 32, 36, 40, 44, 100};
    for (uint32_t off : owned) {
        uint32_t p = rw::get<uint32_t>(self, off);
        if (p) call(Ref_release, {p});
    }
    deleteTutorialWidget(self);
    functionDestroy(a + 120);                 // ~std::function tutorialCallback
    call(Vec2_dtor, {a + 104});
    call(Ref_dtor, {a});
    return self;
}
RW_FUNCTION(0x75E4F4, "StageUI::~StageUI", StageUI::destroy);

void StageUI::destroyDelete(StageUI *self) {
    destroy(self);
    call(fn::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x75E620, "StageUI::~StageUI (deleting)", StageUI::destroyDelete);

// Preload the score counter sounds.
uint32_t StageUI::init(StageUI *) {
    call(audio_Manager_load, {call(audio_Manager_getInstance, {}), SE_LUX});
    call(audio_Manager_load, {call(audio_Manager_getInstance, {}), SE_MANNY});
    call(audio_Manager_load, {call(audio_Manager_getInstance, {}), SE_ITEM});
    return 1;
}
RW_FUNCTION(0x75E62C, "StageUI::init", StageUI::init);

// Remove the tutorial overlay: stop its cocostudio animation, take it off the scene and release it.
uint32_t StageUI::deleteTutorialWidget(StageUI *self) {
    uint32_t w = self->tutorialWidget;
    if (w) {
        uint32_t action = ld32(w + 0x534);    // TutorialWidget: cocostudio::ActionObject*
        if (action) {
            call(ActionObject_stop, {action});
            w = self->tutorialWidget;
        }
        vcall(w, Node_cleanupAndRemove, {w});
        if (self->tutorialWidget) call(Ref_release, {self->tutorialWidget});
    }
    self->tutorialWidget = 0;
    return 0;
}
RW_FUNCTION(0x75E5E0, "StageUI::deleteTutorialWidget", StageUI::deleteTutorialWidget);

// ------------------------------------------------------------------------------------------- getters
uint32_t StageUI::getMedalField(const StageUI *self) { return self->medalField; }
RW_FUNCTION(0x75E65C, "StageUI::getMedalField", StageUI::getMedalField);
uint32_t StageUI::getCarouselDeck(const StageUI *self) { return self->carouselDeck; }
RW_FUNCTION(0x75E660, "StageUI::getCarouselDeck", StageUI::getCarouselDeck);
uint32_t StageUI::getActionField(const StageUI *self) { return self->actionField; }
RW_FUNCTION(0x75E664, "StageUI::getActionField", StageUI::getActionField);
uint32_t StageUI::getStatusField(const StageUI *self) { return self->statusField; }
RW_FUNCTION(0x75E668, "StageUI::getStatusField", StageUI::getStatusField);
uint32_t StageUI::getAttributeField(const StageUI *self) { return self->attributeField; }
RW_FUNCTION(0x75E66C, "StageUI::getAttributeField", StageUI::getAttributeField);
uint32_t StageUI::getPlayerStatusField(const StageUI *self) { return self->playerStatusField; }
RW_FUNCTION(0x75E670, "StageUI::getPlayerStatusField", StageUI::getPlayerStatusField);
uint32_t StageUI::getLuxTargetField(const StageUI *self) { return self->luxTargetField; }
RW_FUNCTION(0x75E674, "StageUI::getLuxTargetField", StageUI::getLuxTargetField);

// Where collected Lux flies to: the "LuxNode" of the Lux target widget.
uint32_t StageUI::getLuxTarget(const StageUI *self) { return findNode(self->luxTargetField, STR_LuxNode, 7); }
RW_FUNCTION(0x75E678, "StageUI::getLuxTarget", StageUI::getLuxTarget);

// Where collected treasures fly to: the item icons of the status field (0 without a status field).
uint32_t StageUI::getRareTreasureTarget(const StageUI *self) {
    uint32_t field = self->statusField;
    return field ? findNode(field, STR_Icon_RareItem, 13) : 0;
}
RW_FUNCTION(0x75E708, "StageUI::getRareTreasureTarget", StageUI::getRareTreasureTarget);

uint32_t StageUI::getCommonTreasureTarget(const StageUI *self) {
    uint32_t field = self->statusField;
    return field ? findNode(field, STR_Icon_Item, 9) : 0;
}
RW_FUNCTION(0x75E7A8, "StageUI::getCommonTreasureTarget", StageUI::getCommonTreasureTarget);

// ------------------------------------------------------------------------------------------- HUD parts
uint32_t StageUI::createMedalField(StageUI *self) {
    uint32_t deck = call(MedalDeck_create, {});
    call(Ref_retain, {deck});
    setPosition(deck, 0.0f, 468.0f);
    self->medalField = deck;
    return deck;
}
RW_FUNCTION(0x75E848, "StageUI::createMedalField", StageUI::createMedalField);

uint32_t StageUI::createCarouselDeck(StageUI *self) {
    uint32_t deck = self->carouselDeck;
    if (!deck) {
        deck = call(CarouselDeck_create, {});
        call(Ref_retain, {deck});
        self->carouselDeck = deck;
    }
    return deck;
}
RW_FUNCTION(0x75E928, "StageUI::createCarouselDeck", StageUI::createCarouselDeck);

uint32_t StageUI::createActionField(StageUI *self) {
    uint32_t w = loadWidget(STR_ActionFieldJson);
    setPosition(w, 0.0f, 0.0f);
    vcall(w, Widget_setTouchEnabled, {w, 0});
    self->actionField = w;
    return w;
}
RW_FUNCTION(0x75E99C, "StageUI::createActionField", StageUI::createActionField);

// The score counters (top right): rare items, items, munny and Lux, all starting at "0".
uint32_t StageUI::createStatusField(StageUI *self, uint32_t) {
    uint32_t w = loadWidget(STR_StatusFieldJson);
    self->rareItemScore = findNode(w, STR_ScoreField_RareItem, 19);
    self->itemScore = findNode(w, STR_ScoreField_Item, 15);
    self->mannyScore = findNode(w, STR_ScoreField_Manny, 16);
    self->luxScore = findNode(w, STR_ScoreField_Lux, 14);
    self->itemLabel = findNode(w, STR_Item_Label, 10);
    self->rareItemLabel = findNode(w, STR_RareItem_Label, 14);
    self->mannyLabel = findNode(w, STR_Manny_Label, 11);
    self->luxLabel = findNode(w, STR_Lux_Label, 9);
    const uint32_t labels[] = {self->itemLabel, self->rareItemLabel, self->mannyLabel, self->luxLabel};
    for (uint32_t label : labels) {
        Frame f;
        uint32_t s = newStr(f, STR_Zero, 1);
        call(fn::Text_setString, {label, s});
        strFree(s);
    }
    SceneUtil::setShadow(self->itemLabel);
    SceneUtil::setShadow(self->rareItemLabel);
    SceneUtil::setShadow(self->mannyLabel);
    SceneUtil::setShadow(self->luxLabel);
    Size visible = visibleSize();
    Size size = customSize(w);
    setPosition(w, visible.width - size.width, visible.height - size.height);   // top right corner
    vcall(w, Widget_setTouchEnabled, {w, 0});
    vcall(w, node::setVisible, {w, 0});
    self->statusField = w;
    return w;
}
RW_FUNCTION(0x75EA44, "StageUI::createStatusField", StageUI::createStatusField);

uint32_t StageUI::createAttributeField(StageUI *self) {
    uint32_t w = loadWidget(STR_AttributeJson);
    Size visible = visibleSize();
    Size size = customSize(w);
    setPosition(w, visible.width - size.width, 0.0f);   // bottom right corner
    vcall(w, Widget_setTouchEnabled, {w, 0});
    vcall(w, node::setVisible, {w, 0});
    self->attributeField = w;
    return w;
}
RW_FUNCTION(0x75EF24, "StageUI::createAttributeField", StageUI::createAttributeField);

uint32_t StageUI::createPlayerStatusField(StageUI *self, uint32_t a, uint32_t b, uint32_t style) {
    uint32_t indicator = call(PlayerStatusIndicator_create, {a, b, style});
    call(Ref_retain, {indicator});
    float height = bitsf(call(AppGetGrameFrameHeight, {}));
    setPosition(indicator, -8.0f, height + -152.0f);
    vcall(indicator, node::setVisible, {indicator, 0});
    self->playerStatusField = indicator;
    return indicator;
}
RW_FUNCTION(0x75F000, "StageUI::createPlayerStatusField", StageUI::createPlayerStatusField);

uint32_t StageUI::createLuxTargetField(StageUI *self) {
    uint32_t w = loadWidget(STR_LuxNodeJson);
    Size visible = visibleSize();
    Size size = customSize(w);
    setPosition(w, visible.width - size.width, visible.height - size.height);
    vcall(w, Widget_setTouchEnabled, {w, 0});
    vcall(w, node::setVisible, {w, 0});
    self->luxTargetField = w;
    return w;
}
RW_FUNCTION(0x75F0B4, "StageUI::createLuxTargetField", StageUI::createLuxTargetField);

uint32_t StageUI::shake_RareItem(StageUI *self) { return shake(self->rareItemScore); }
RW_FUNCTION(0x75F1A4, "StageUI::shake_RareItem", StageUI::shake_RareItem);
uint32_t StageUI::shake_Item(StageUI *self) { return shake(self->itemScore); }
RW_FUNCTION(0x75F210, "StageUI::shake_Item", StageUI::shake_Item);
uint32_t StageUI::shake_Manny(StageUI *self) { return shake(self->mannyScore); }
RW_FUNCTION(0x75F27C, "StageUI::shake_Manny", StageUI::shake_Manny);
uint32_t StageUI::shake_Lux(StageUI *self) { return shake(self->luxScore); }
RW_FUNCTION(0x75F2E8, "StageUI::shake_Lux", StageUI::shake_Lux);

// ------------------------------------------------------------------------------------------- headings
// The heading banners: an LWFNodeGroup over the 7 heading files, centred on the visible area, with the Lux
// board behind it; the 2 extra banners are preloaded.
uint32_t StageUI::createHeading(StageUI *self) {
    uint32_t group = call(LWFNodeGroup_create, {HEADING_FILES, HEADING_FILES + 4 * HEADING_FILE_COUNT, 0});
    call(Ref_retain, {group});
    Vec2 origin = visibleOrigin();
    Size visible = visibleSize();
    self->headingX = origin.x + visible.width * 0.5f;
    self->headingY = origin.y + visible.height * 0.5f;
    uint32_t a = rw::guest_addr(self);
    vcall(group, node::setPosition, {group, a + 104});
    call(LWFNodeGroup_preloadResources, {group});
    for (uint32_t k = 0; k < HEADING_EXTRA_COUNT; k++) {
        uint32_t name = ld32(HEADING_EXTRAS + 8 * k);
        Frame f;
        uint32_t path = stringBuffer(f, {STR_HeadingLwfFmt, name, name});
        uint32_t s = newStr(f, path);
        call(LWFUtil_preloadResources, {s});
        strFree(s);
    }
    uint32_t board = call(LuxBoard_create, {});
    self->luxBoard = board;
    vcall(group, node::addChildZ, {group, board, uint32_t(-10)});
    self->heading = group;
    return group;
}
RW_FUNCTION(0x75F354, "StageUI::createHeading", StageUI::createHeading);

// Show a heading banner: 2 / 3 sit 37 points higher and hide the Lux board, 4 shows the Lux board, 1 (re)adds
// the 2 extra banners (offset by the frame height margin). Returns 0 (the stack check's result).
void StageUI::showHeading(StageUI *self, int32_t heading) {
    uint32_t group = self->heading;
    if (!group) return;
    if (uint32_t(heading - 2) < 2) {
        vcall(group, node::setPositionY, {group, scene::fbits(self->headingY + 37.0f)});
        call(LuxBoard_play, {self->luxBoard, 0});
    } else if (heading == 4) {
        vcall(group, node::setPositionY, {group, scene::fbits(self->headingY)});
        call(LuxBoard_play, {self->luxBoard, 1});
    } else if (heading == 1) {
        vcall(group, node::setPositionY, {group, scene::fbits(self->headingY)});
        float margin = (bitsf(call(AppGetGrameFrameHeight, {})) + -640.0f) * 0.5f;
        for (uint32_t k = 0; k < HEADING_EXTRA_COUNT; k++) {
            uint32_t entry = HEADING_EXTRAS + 8 * k;
            Frame f;
            uint32_t path = stringBuffer(f, {STR_HeadingLwfFmt, ld32(entry), ld32(entry)});
            uint32_t lwf = call(LWFNode_create, {path, 0});
            uint32_t s = newStr(f, ld32(entry));
            vcall(lwf, node::setName, {lwf, s});
            strFree(s);
            vcall(lwf, node::setPositionY, {lwf, scene::fbits(margin * ldf(entry + 4))});
            uint32_t g = self->heading;
            vcall(g, node::addChild, {g, lwf});
        }
    } else {
        vcall(group, node::setPositionY, {group, scene::fbits(self->headingY)});
    }
    call(LWFNodeGroup_change, {self->heading, uint32_t(heading)});
}

namespace {
// showHeading returns r0 = 0 (the stack check's subtraction) on every path
uint32_t showHeadingEntry(StageUI *self, int32_t heading) {
    StageUI::showHeading(self, heading);
    return 0;
}
}  // namespace
RW_FUNCTION(0x75F54C, "StageUI::showHeading", showHeadingEntry);

// Once the group has played its last banner (state 1), drop the 2 extra banner nodes.
void StageUI::garbageCollectHeading(StageUI *self) {
    uint32_t group = self->heading;
    if (!group || ld32(group + GROUP_LAST_SHOWN) != 1) return;
    for (uint32_t k = 0;; ) {
        uint32_t getChildByName = ld32(ld32(group) + node::getChildByName);
        uint32_t name = ld32(HEADING_EXTRAS + 8 * k);
        uint32_t child;
        {
            Frame f;
            uint32_t s = newStr(f, name);
            child = call(getChildByName, {group, s});
            strFree(s);
        }
        if (child) vcall(child, Node_cleanupAndRemove, {child});
        if (++k == HEADING_EXTRA_COUNT) break;
        group = self->heading;
    }
}

namespace {
uint32_t garbageCollectHeadingEntry(StageUI *self) {
    StageUI::garbageCollectHeading(self);
    return 0;
}
}  // namespace
RW_FUNCTION(0x75F708, "StageUI::garbageCollectHeading", garbageCollectHeadingEntry);

int32_t StageUI::getLastShowHeading(const StageUI *self) {
    uint32_t group = self->heading;
    if (!group) return 7;
    int32_t last = int32_t(ld32(group + GROUP_LAST_SHOWN));
    return last >= 0 ? last : 7;
}
RW_FUNCTION(0x75F7F8, "StageUI::getLastShowHeading", StageUI::getLastShowHeading);

// The current banner's LWF is still playing (lwf->rootMovie... flag at +0xAD).
uint32_t StageUI::isPlayingHeading(const StageUI *self) {
    uint32_t group = self->heading;
    if (!group) return 0;
    uint32_t node = ld32(group + GROUP_CURRENT);
    if (!node) return 0;
    uint32_t lwf = ld32(node + 0x3E0);
    return ld8(ld32(lwf + 0x18) + 0xAD) != 0 ? 1 : 0;
}
RW_FUNCTION(0x75F814, "StageUI::isPlayingHeading", StageUI::isPlayingHeading);

// ------------------------------------------------------------------------------------------- score display
// Refresh the four score counters from the live stage state (raw counters at StageState +40..+52).
void StageUI::updateDispUI(StageUI *self) {
    uint32_t app = appState();
    uint32_t state = ld32(app + APPSTATE_STAGE_STATE);
    uint32_t ctrl = ld32(app + APPSTATE_STAGE_STATE + 4);    // keep the shared_ptr alive
    if (ctrl) call(fn::shared_add, {ctrl});
    updateCounter(self, self->itemLabel, self->shownItems, int32_t(ld32(state + 48)), shake_Item, SE_ITEM);
    updateCounter(self, self->rareItemLabel, self->shownRareItems, int32_t(ld32(state + 52)), shake_RareItem,
                  SE_ITEM);
    updateCounter(self, self->mannyLabel, self->shownManny, int32_t(ld32(state + 44)), shake_Manny, SE_MANNY);
    updateCounter(self, self->luxLabel, self->shownLux, int32_t(ld32(state + 40)), shake_Lux, SE_LUX);
    if (ctrl) call(fn::shared_release, {ctrl});
}
RW_FUNCTION(0x75F840, "StageUI::updateDispUI", StageUI::updateDispUI);

// The player status indicator starts from the stage state's HP and gauge and the medal deck's defence.
void StageUI::set_start_avatar_parameter(StageUI *self) {
    uint32_t indicator = self->playerStatusField;
    uint32_t state = ld32(appState() + APPSTATE_STAGE_STATE);
    call(PlayerStatusIndicator_setHP, {indicator, uint32_t(safe32(state, 8))});
    indicator = self->playerStatusField;
    state = ld32(appState() + APPSTATE_STAGE_STATE);
    call(PlayerStatusIndicator_setCP, {indicator, uint32_t(safe32(state, 12))});
    indicator = self->playerStatusField;
    uint32_t def = call(MedalDeck_getCurrentDefense, {self->medalField});
    call(PlayerStatusIndicator_setDef, {indicator, def});
}

namespace {
uint32_t setStartAvatarParameterEntry(StageUI *self) {
    StageUI::set_start_avatar_parameter(self);
    return 0;
}
}  // namespace
RW_FUNCTION(0x760E24, "StageUI::set_start_avatar_parameter", setStartAvatarParameterEntry);

// Heading 5 = player turn, 6 = enemy turn.
void StageUI::setBattleTurn(StageUI *self, uint32_t playerTurn) { showHeading(self, playerTurn ? 5 : 6); }

namespace {
uint32_t setBattleTurnEntry(StageUI *self, uint32_t playerTurn) {
    StageUI::setBattleTurn(self, playerTurn);
    return 0;
}
}  // namespace
RW_FUNCTION(0x761726, "StageUI::setBattleTurn", setBattleTurnEntry);

// ------------------------------------------------------------------------------------------- continue dialog
namespace {
constexpr uint32_t VT_CONTINUE_CHOICE = 0xF6E434;   // openContinueDialog $_?: {onClose, StageUI*, onClose}
constexpr uint32_t VT_CONTINUE_CLOSED = 0xF6E53C;   // openContinueDialog $_?: {onClose} (no continue offered)
constexpr uint32_t FUNCTION_VV_COPY = 0x4683CD;     // std::function<void()>::function(const function&)
constexpr uint32_t SceneUtil_openContinuPopup = 0x45DDE1, SceneUtil_openOkDialog4Line = 0x45C775;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t API_CONTINUE_STATE = 0x64;       // APIManager: the continue offer (its byte +0x10: unavailable)
constexpr int32_t TEXT_NO_CONTINUE = 103100050;

// libc++'s std::function copy constructor, inline: the callable cloned into dst
void copyFunction(uint32_t dst, uint32_t src) {
    const uint32_t p = ld32(src + 16);
    if (!p) {
        st32(dst + 16, 0);
    } else if (p == src) {
        st32(dst + 16, dst);
        vcall(p, 0x0C, {p, dst});   // __clone(dst)
    } else {
        st32(dst + 16, vcall(p, 0x08, {p}));   // __clone()
    }
}
}  // namespace

// The "continue?" popup after a lost quest (onClose runs when it is closed); when no continue can be offered,
// an OK dialog with the "cannot continue" text instead.
void openContinueDialog(uint32_t self, uint32_t onClose) {
    Frame f;
    // the popup's lambda: {onClose, this, onClose}
    const uint32_t callback = f.alloc(24), lambda = f.alloc(0x38);
    call(FUNCTION_VV_COPY, {callback, onClose});
    copyFunction(lambda, callback);
    st32(lambda + 0x18, self);
    call(FUNCTION_VV_COPY, {lambda + 0x20, onClose});
    const uint32_t offer = ld32(call(APIManager_getInstance, {}) + API_CONTINUE_STATE);
    if (!offer || ld8(offer + 0x10)) {
        const uint32_t moved = f.alloc(0x38);
        copyFunction(moved, lambda);
        st32(moved + 0x18, ld32(lambda + 0x18));
        copyFunction(moved + 0x20, lambda + 0x20);
        st32(moved + 0x18, ld32(lambda + 0x18));
        const uint32_t choice = f.alloc(20);
        st32(choice + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x40});
        if (heap) {
            st32(heap, VT_CONTINUE_CHOICE);
            copyFunction(heap + 8, moved);
            st32(heap + 0x20, ld32(moved + 0x18));
            copyFunction(heap + 0x28, moved + 0x20);
            st32(heap + 0x20, ld32(moved + 0x18));
        }
        st32(choice + 16, heap);
        call(SceneUtil_openContinuPopup, {choice});
        functionDestroy(choice);
        functionDestroy(moved + 0x20);
        functionDestroy(moved);
    } else {
        const uint32_t text = f.alloc(8), message = f.alloc(12);
        call(fn::TextData_get, {text, 0, uint32_t(TEXT_NO_CONTINUE)});
        strInitC(message, ld32(ld32(text) + 4));
        const uint32_t closed = f.alloc(24);
        copyFunction(closed, callback);
        const uint32_t done = f.alloc(20);
        st32(done + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x20});
        if (heap) {
            st32(heap, VT_CONTINUE_CLOSED);
            copyFunction(heap + 8, closed);
        }
        st32(done + 16, heap);
        call(SceneUtil_openOkDialog4Line, {message, done, 0});
        functionDestroy(done);
        functionDestroy(closed);
        strFree(message);
        sharedRelease(text);
    }
    functionDestroy(lambda + 0x20);
    functionDestroy(lambda);
    functionDestroy(callback);
}
RW_FUNCTION(0x75FDB8, "StageUI::openContinueDialog", openContinueDialog);
