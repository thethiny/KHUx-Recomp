#include "RaidBossHPIndicator.hpp"

using namespace scene;
using namespace raidhp;

namespace {
constexpr uint32_t WIDGET_CACHE = 0x1003D00;     // cocos2d::ui::Widget* [5]: the loaded layouts (firstCall)
constexpr uint32_t WIDGET_NAMES = 0xF5DCAC;      // const char* [5]: their cocostudio names
constexpr uint32_t WIDGET_COUNT = 5;

// __func vtables (+8) of the lambdas
constexpr uint32_t VT_ANIM_RED = 0xF5DC88;       // init()::$_0  scaleX -> red bar percent
constexpr uint32_t VT_ANIM_GREEN = 0xF5DC5C;     // init()::$_1  scaleX -> green bar percent
constexpr uint32_t VT_SWITCH_MODE = 0xF5DC30;    // switchModeFieldToBattle()::$_2

constexpr uint32_t BUFF_NONE = 20;               // buffTurns value of an inactive buff

// armature = Armature::create(name)
uint32_t createArmature(uint32_t name, uint32_t len) {
    TempStr s(name, len);
    return rw::call(lib::Armature_create, {s});
}
void addArmatureFile(uint32_t mgr, uint32_t path) {
    TempStr s(path);
    rw::call(lib::ArmatureDataManager_addArmatureFileInfo, {mgr, s});
}
}  // namespace

// ------------------------------------------------------------------------------------------- widget cache
uint32_t RaidBossHPIndicator::loadWidget(int32_t index, int32_t clone) {
    uint32_t cached = rw::load<uint32_t>(WIDGET_CACHE + 4 * uint32_t(index));
    if (cached) return clone == 1 ? rw::call(lib::Widget_clone, {cached}) : cached;
    Frame f;
    uint32_t path = stringBuffer(f, {str::WIDGET_JSON_FMT, rw::load<uint32_t>(WIDGET_NAMES + 4 * uint32_t(index))});
    uint32_t files = rw::call(lib::FileUtils_getInstance, {});
    uint32_t isFileExist = rw::load<uint32_t>(rw::load<uint32_t>(files) + slot::isFileExist);
    uint32_t exists;
    {
        TempStr s(path);
        exists = rw::call(isFileExist, {files, s});
    }
    if (exists != 1) return 0;
    return rw::call(lib::GUIReader_widgetFromJsonFile, {rw::call(lib::GUIReader_getInstance, {}), path});
}
RW_FUNCTION(0x5E4FB4, "RaidBossHPIndicator loadWidget (sub_5E4FB4)", RaidBossHPIndicator::loadWidget);

void RaidBossHPIndicator::firstCall() {
    for (uint32_t k = 0; k < WIDGET_COUNT; k++) {
        uint32_t w = loadWidget(int32_t(k), 0);
        rw::save<uint32_t>(WIDGET_CACHE + 4 * k, w);
        if (w) rw::call(lib::Ref_retain, {w});
    }
}
RW_FUNCTION(0x5E4F80, "RaidBossHPIndicator::firstCall", RaidBossHPIndicator::firstCall);

void RaidBossHPIndicator::finalCall() {
    for (uint32_t k = 0; k < WIDGET_COUNT; k++) {
        uint32_t w = rw::load<uint32_t>(WIDGET_CACHE + 4 * k);
        if (w) {
            rw::call(lib::Ref_release, {w});
            rw::save<uint32_t>(WIDGET_CACHE + 4 * k, 0);
        }
    }
}
RW_FUNCTION(0x5E50B8, "RaidBossHPIndicator::finalCall", RaidBossHPIndicator::finalCall);

// ------------------------------------------------------------------------------------------- life cycle
void RaidBossHPIndicator::destroyDelete(RaidBossHPIndicator *self) {
    rw::call(0x5E50ED, {rw::guest_addr(self)});   // ~RaidBossHPIndicator (the folded RaidBossStatusIndicator D2)
    rw::call(guest::OPERATOR_DELETE, {rw::guest_addr(self)});
}
RW_FUNCTION(0x5E5158, "RaidBossHPIndicator::~RaidBossHPIndicator (deleting)", RaidBossHPIndicator::destroyDelete);

RaidBossHPIndicator *RaidBossHPIndicator::create(int32_t enemyType, int32_t type, uint32_t hpMax, uint32_t hpPerBox) {
    auto *self = rw::host_ptr<RaidBossHPIndicator>(guest::newObject(SIZE));
    construct(self);
    if (init(self, enemyType, type, hpMax, hpPerBox) == 1) {
        rw::call(lib::Ref_autorelease, {rw::guest_addr(self)});
        return self;
    }
    vcall(rw::guest_addr(self), slot::deletingDtor, {rw::guest_addr(self)});
    return nullptr;
}
RW_FUNCTION(0x5E5164, "RaidBossHPIndicator::create", RaidBossHPIndicator::create);

RaidBossHPIndicator *RaidBossHPIndicator::construct(RaidBossHPIndicator *self) {
    uint32_t me = rw::guest_addr(self);
    rw::call(lib::Node_ctor, {me});
    rw::set<uint32_t>(self, 0, VTABLE);
    rw::call(lib::aeabi_memset, {me + 0x270, 0x2C, 0});    // hp .. boxSlots
    self->enemyType = 4;
    self->dropCount = 0;
    rw::call(lib::aeabi_memset, {me + 0x2A0, 0x17, 0});    // type .. gaugeResetPending
    rw::call(lib::Size_copy, {me + 0x2BC, lib::SIZE_ZERO});
    self->buffShown = 0;
    self->buffFlags = 0;
    self->boxPartNames = 0;
    self->boxNames = 0;
    self->targetArmature = 0;
    return self;
}
RW_FUNCTION(0x5E51D4, "RaidBossHPIndicator::RaidBossHPIndicator", RaidBossHPIndicator::construct);

int32_t RaidBossHPIndicator::init(RaidBossHPIndicator *self, int32_t enemyType, int32_t type, uint32_t hpMax,
                                  uint32_t hpPerBox) {
    uint32_t me = rw::guest_addr(self);
    self->enemyType = uint32_t(enemyType);
    self->type = uint32_t(type);
    if (enemyType == 3) {               // status icons only
        setStatusIconArmature(self, me);
        return 1;
    }
    if (type == 3) return 0;
    uint32_t layout = loadWidget(enemyType == 2 ? 4 : 3, 1);
    vcall(me, slot::addChild, {me, layout});
    vcall(layout, slot::widget29C, {layout, 0});
    self->hpPerBox = hpPerBox;
    uint32_t boxes = boxesFor(hpMax, hpPerBox);
    self->boxLeft = boxes;
    self->boxTotal = boxes;
    self->boxNames = BOX_NAMES;
    self->boxPartNames = BOX_PART_NAMES;
    {   // the layout is anchored at its centre: move it up-left by half its width and its full height
        uint32_t size = rw::call(lib::Widget_getCustomSize, {layout});
        uint32_t setPosition = rw::load<uint32_t>(rw::load<uint32_t>(layout) + slot::setPosition);
        float x = -(ldf(size) * 0.5f);
        float y = -ldf(size + 4);
        Frame f;
        rw::call(setPosition, {layout, newVec2(f, x, y)});
    }
    uint32_t gaugeImage;
    {
        TempStr s(str::RAID_GAUGE_HP, 13);
        gaugeImage = search(layout, s);
    }
    rw::call(lib::Size_assign, {me + 0x2BC, rw::call(lib::Widget_getCustomSize, {gaugeImage})});
    {
        TempStr s(str::RAID_GAUGE_SMALL, 16);
        setVisibleAttribute(self, search(layout, s), type);
    }
    self->targetArmature = setSuitableAsTargetArmature(self, layout);
    setStatusIconArmature(self, layout);
    removeNodeWithName(self, layout, str::BUFF_DEBUFF_ICON);

    // gauges: [0] red bar animator, [1] timer node, [2] green bar animator
    uint32_t anim = rw::guest_addr(AnimationNode::create());
    setOnScaleX(anim, VT_ANIM_RED, me);
    vcall(me, slot::addChild, {me, anim});
    rw::call(lib::NodeVector_pushBack, {me + 0x288, anim});
    anim = rw::guest_addr(AnimationNode::create());
    vcall(me, slot::addChild, {me, anim});
    rw::call(lib::NodeVector_pushBack, {me + 0x288, anim});
    anim = rw::guest_addr(AnimationNode::create());
    setOnScaleX(anim, VT_ANIM_GREEN, me);
    vcall(me, slot::addChild, {me, anim});
    rw::call(lib::NodeVector_pushBack, {me + 0x288, anim});

    uint32_t green = findNode(layout, str::GAUGE_GREEN_S, 12);
    uint32_t red = findNode(layout, str::GAUGE_RED_S, 10);
    uint32_t level = findNode(layout, str::RAID_GAUGE_LV, 13);
    uint32_t hpBox, hpBoxBg;
    {
        TempStr s(str::ACT_MAP_HP_BOX, 11);
        hpBox = search(layout, s);
    }
    {
        TempStr s(str::ACT_MAP_HP_BOX_BG, 13);
        hpBoxBg = search(layout, s);
    }
    uint32_t raidBox = findNode(layout, str::RAID_HP_BOX, 9);

    uint32_t kind = self->enemyType;
    if (kind - 1 < 2) {                 // coerce / boss: the action-map box panel
        vcall(layout, slot::removeChild, {layout, raidBox, 1});
        self->boxPanel = hpBox;
        self->boxSlots = 4;
    } else if (kind == 0) {             // normal enemy: the raid box panel, no level
        vcall(layout, slot::removeChild, {layout, level, 1});
        vcall(layout, slot::removeChild, {layout, hpBox, 1});
        self->boxPanel = raidBox;
        self->boxSlots = 8;
    }
    rw::call(lib::NodeVector_pushBack, {me + 0x288, green});   // [3]
    rw::call(lib::NodeVector_pushBack, {me + 0x288, red});     // [4]
    self->hpMax = hpMax;
    self->hp = 0;
    setHP(self, hpMax);
    if (self->hpPerBox < hpMax) {
        // more than one gauge: hide the cells beyond the boxes in use
        setVisible(self->boxPanel, 1);
        for (uint32_t k = self->boxTotal; k < self->boxSlots; k++) {
            uint32_t name = rw::load<uint32_t>(BOX_NAMES + 4 * k);
            setVisibleNodeWithName(self, self->boxPanel, name, 0);
            if (hpBoxBg) setVisibleNodeWithName(self, hpBoxBg, name, 0);
        }
        if (umod(self->hpMax, self->hpPerBox) == 0) removeNodeWithName(self, layout, str::RAID_GAUGE_HPMAX);
    } else {
        vcall(layout, slot::removeChild, {layout, hpBoxBg, 1});
        vcall(layout, slot::removeChild, {layout, self->boxPanel, 1});
        self->boxPanel = 0;
        removeNodeWithName(self, layout, str::RAID_GAUGE_HPMAX);
    }
    uint32_t perBox = self->hpPerBox;
    uint32_t rem = umod(self->hpMax, perBox);
    float rate = float(rem) / float(perBox);
    if (rem == 0) rate = 1.0f;
    uint32_t g0 = gauge(self, 0);
    vcall(g0, slot::setScale, {g0, fbits(rate)});
    setPercent(gauge(self, 4), rate * 100.0f);
    setGaugeLength(self, gaugeImage, rate);
    setBoxHpGauge(self, self->boxLeft, self->boxLeft);
    iniBoxCountText(self, childByName(me, str::PANEL_4, 7), -4.0f);
    setBoxCountText(self, childByName(me, str::PANEL_4, 7));
    return 1;
}
RW_FUNCTION(0x5E527C, "RaidBossHPIndicator::init", RaidBossHPIndicator::init);

// ------------------------------------------------------------------------------------------- building blocks
void RaidBossHPIndicator::NumSync(RaidBossHPIndicator *, uint32_t node, float scale) {
    uint32_t delay = rw::call(lib::DelayTime_create, {fbits(0.5f)});
    uint32_t ease = rw::call(lib::EaseSineOut_create, {rw::call(lib::ScaleTo_create, {fbits(0.5f), fbits(scale)})});
    uint32_t seq = rw::call(lib::Sequence_create, {delay, ease, 0});
    rw::call(lib::Node_runAction, {node, seq});
}
RW_FUNCTION(0x5E5AD4, "RaidBossHPIndicator::NumSync", RaidBossHPIndicator::NumSync);

uint32_t RaidBossHPIndicator::setStatusIconArmature(RaidBossHPIndicator *self, uint32_t parent) {
    if (!parent) return 0;
    for (uint32_t k = 0; k < 20; k++) self->buffTurns[k] = BUFF_NONE;
    addArmatureFile(rw::call(lib::ArmatureDataManager_getInstance, {}), str::BUFF_ANIM_JSON);
    uint32_t layout = loadWidget(self->enemyType == 2 ? 4 : 3, 0);
    uint32_t icon;
    {
        TempStr s(str::BUFF_DEBUFF_ICON, 14);
        icon = search(layout, s);
    }
    if (icon) {
        self->buffIcon = createArmature(str::BUFF_ANIM, 20);
        uint32_t a = self->buffIcon;
        uint32_t setPosition = rw::load<uint32_t>(rw::load<uint32_t>(a) + slot::setPosition);
        rw::call(setPosition, {a, vcall(icon, slot::getPosition, {icon})});
        setVisible(self->buffIcon, 0);
        self->buffTurnIcon = createArmature(str::BUFF_ANIM, 20);
        uint32_t b = self->buffTurnIcon;
        {
            Frame f;
            vcall(b, slot::setPosition, {b, newVec2(f, 0.0f, 0.0f)});
        }
        vcall(self->buffIcon, slot::addChildZ, {self->buffIcon, self->buffTurnIcon, 100});
        vcall(parent, slot::addChild, {parent, self->buffIcon});
    }
    if (self->enemyType == 3) {
        // centre the icon horizontally: position - (width / 2, height) of the layout
        uint32_t size = rw::call(lib::Widget_getCustomSize, {layout});
        Frame f;
        uint32_t half = newVec2(f, ldf(size) * 0.5f, ldf(size + 4));
        uint32_t pos = f.alloc(8);
        rw::call(lib::Vec2_copy, {pos, vcall(self->buffIcon, slot::getPosition, {self->buffIcon})});
        uint32_t a = self->buffIcon;
        uint32_t setPosition = rw::load<uint32_t>(rw::load<uint32_t>(a) + slot::setPosition);
        uint32_t moved = f.alloc(8);
        rw::call(lib::Vec2_copy, {moved, pos});
        rw::call(lib::Vec2_subtract, {moved, half});
        rw::call(setPosition, {a, moved});
    }
    return 0;
}
RW_FUNCTION(0x5E5B20, "RaidBossHPIndicator::setStatusIconArmature", RaidBossHPIndicator::setStatusIconArmature);

uint32_t RaidBossHPIndicator::getGaugeBoxCount(RaidBossHPIndicator *self, uint32_t hp) {
    return boxesFor(hp, self->hpPerBox);
}
RW_FUNCTION(0x5E5E1C, "RaidBossHPIndicator::getGaugeBoxCount", RaidBossHPIndicator::getGaugeBoxCount);

void RaidBossHPIndicator::setVisibleAttribute(RaidBossHPIndicator *, uint32_t node, int32_t type) {
    if (!node) return;
    for (int32_t k = 0; k < 3; k++) {
        uint32_t found;
        {
            Frame f;
            uint32_t name = stringBuffer(f, {str::TYPE_FMT, uint32_t(k + 1)});
            TempStr s(name);
            found = search(node, s);
        }
        if (found) setVisible(found, type == k ? 1 : 0);
    }
}
RW_FUNCTION(0x5E5E68, "RaidBossHPIndicator::setVisibleAttribute", RaidBossHPIndicator::setVisibleAttribute);

uint32_t RaidBossHPIndicator::setSuitableAsTargetArmature(RaidBossHPIndicator *, uint32_t widget) {
    if (!widget) return 0;
    uint32_t marker;
    {
        TempStr s(str::ATTRIBUTE_ATK_UP, 14);
        marker = search(widget, s);
    }
    if (!marker) return 0;
    uint32_t mgr = rw::call(lib::ArmatureDataManager_getInstance, {});
    {
        Frame f;
        uint32_t path = stringBuffer(f, {str::EXPORT_JSON_FMT, str::ACTION_MAP_ANIMATION});
        addArmatureFile(mgr, path);
    }
    uint32_t a = createArmature(str::ACTION_MAP_ANIMATION, 18);
    {
        uint32_t anim = vcall(a, slot::getAnimation, {a});
        uint32_t play = rw::load<uint32_t>(rw::load<uint32_t>(anim) + slot::anim_play);
        TempStr s(str::ATTRIBUTE_ATK_UP, 14);
        rw::call(play, {anim, s, 0xFFFFFFFFu, 0xFFFFFFFFu});
    }
    uint32_t anim = vcall(a, slot::getAnimation, {a});
    vcall(anim, slot::anim_pause, {anim, 0});
    setVisible(a, 0);
    uint32_t setPosition = rw::load<uint32_t>(rw::load<uint32_t>(a) + slot::setPosition);
    rw::call(setPosition, {a, vcall(marker, slot::getPosition, {marker})});
    uint32_t setScaleXY = rw::load<uint32_t>(rw::load<uint32_t>(a) + slot::setScaleXY);
    uint32_t sx = vcall(marker, slot::getScaleX, {marker});
    uint32_t sy = vcall(marker, slot::getScaleY, {marker});
    rw::call(setScaleXY, {a, sx, sy});
    vcall(widget, slot::addChildZ, {widget, a, 1});
    vcall(marker, slot::removeFromParent, {marker});
    return a;
}
RW_FUNCTION(0x5E5F60, "RaidBossHPIndicator::setSuitableAsTargetArmature",
            RaidBossHPIndicator::setSuitableAsTargetArmature);

uint32_t RaidBossHPIndicator::removeNodeWithName(RaidBossHPIndicator *, uint32_t root, uint32_t name) {
    uint32_t node;
    {
        TempStr s(name);
        node = search(root, s);
    }
    if (!node) return 0;
    vcall(node, slot::removeFromParent, {node});
    return 1;
}
RW_FUNCTION(0x5E61E0, "RaidBossHPIndicator::removeNodeWithName", RaidBossHPIndicator::removeNodeWithName);

uint32_t RaidBossHPIndicator::setVisibleNodeWithName(RaidBossHPIndicator *, uint32_t root, uint32_t name,
                                                     uint32_t visible) {
    uint32_t node;
    {
        TempStr s(name);
        node = search(root, s);
    }
    if (!node) return 0;
    setVisible(node, visible);
    return 1;
}
RW_FUNCTION(0x5E69B4, "RaidBossHPIndicator::setVisibleNodeWithName", RaidBossHPIndicator::setVisibleNodeWithName);

float RaidBossHPIndicator::setGaugeLength(RaidBossHPIndicator *self, uint32_t image, float rate) {
    if (!(rate < 1.0f)) rate = 1.0f;
    if (!(rate > 0.0f)) rate = 0.0f;
    if (image) {
        // width = both cap insets + rate * the stretchable part of the original width
        float capX = ldf(rw::call(lib::ImageView_getCapInsets, {image}));
        Frame f;
        uint32_t size = f.alloc(8);
        rw::call(lib::Size_copy, {size, rw::guest_addr(self) + 0x2BC});
        float caps = capX + capX;
        float stretch = rate * (ldf(size) - caps);
        stf(size, caps + stretch);
        vcall(image, slot::setSize, {image, size});
    }
    return rate;
}
RW_FUNCTION(0x5E6A68, "RaidBossHPIndicator::setGaugeLength", RaidBossHPIndicator::setGaugeLength);

void RaidBossHPIndicator::setBoxHpGauge(RaidBossHPIndicator *self, uint32_t boxesBefore, uint32_t boxesNow) {
    uint32_t total = self->boxTotal;
    uint32_t slots = self->boxSlots;
    uint32_t cells = total < slots ? total : slots;
    int32_t threeRounds = int32_t(slots * 3);
    uint32_t base = boxesBefore - umod(boxesBefore, slots);
    for (uint32_t k = 0; k < cells; k++) {
        uint32_t cell;
        {
            TempStr s(rw::load<uint32_t>(self->boxNames + 4 * k));
            cell = search(self->boxPanel, s);
        }
        // the box this cell shows (cells wrap around after `slots` boxes)
        uint32_t box = base + k;
        uint32_t perRound = self->boxSlots;
        if (box >= boxesBefore) box -= perRound;
        bool inUse = box < boxesBefore;
        bool full = box >= boxesNow && inUse && int32_t(box) < threeRounds;
        setVisibleNodeWithName(self, cell, rw::load<uint32_t>(self->boxPartNames + 0), full);
        bool spent = box >= boxesBefore || box < boxesNow || int32_t(box) >= threeRounds;
        uint32_t round = udiv(box, perRound);
        setVisibleNodeWithName(self, cell, rw::load<uint32_t>(self->boxPartNames + 4), round == 0 && spent && inUse);
        setVisibleNodeWithName(self, cell, rw::load<uint32_t>(self->boxPartNames + 8), round == 1 && spent && inUse);
        setVisibleNodeWithName(self, cell, rw::load<uint32_t>(self->boxPartNames + 12), round > 1 && spent && inUse);
    }
}
RW_FUNCTION(0x5E6B40, "RaidBossHPIndicator::setBoxHpGauge", RaidBossHPIndicator::setBoxHpGauge);

void RaidBossHPIndicator::iniBoxCountText(RaidBossHPIndicator *, uint32_t panel, float kerning) {
    uint32_t num = childByName(panel, str::BOX_NUM, 7);
    uint32_t label = vcall(num, slot::getVirtualRenderer, {num});
    rw::call(lib::Label_setAdditionalKerning, {label, fbits(kerning)});
    vcall(num, slot::setZ10, {num, 10});
}
RW_FUNCTION(0x5E6D24, "RaidBossHPIndicator::iniBoxCountText", RaidBossHPIndicator::iniBoxCountText);

void RaidBossHPIndicator::setBoxCountText(RaidBossHPIndicator *self, uint32_t panel) {
    uint32_t plus = childByName(panel, str::BOX_NUM_PLUS, 11);
    uint32_t num = childByName(panel, str::BOX_NUM, 7);
    // boxes beyond the cells shown: "+N"
    int32_t extra = int32_t(self->boxLeft - self->boxSlots);
    if (plus) setVisible(plus, extra > 0 ? 1 : 0);
    if (num) {
        setVisible(num, extra > 0 ? 1 : 0);
        if (extra >= 1) setAtlasNumber(num, extra);
    }
}
RW_FUNCTION(0x5E6DDC, "RaidBossHPIndicator::setBoxCountText", RaidBossHPIndicator::setBoxCountText);

uint32_t RaidBossHPIndicator::setTextWithName(RaidBossHPIndicator *, uint32_t root, uint32_t name, uint32_t text) {
    uint32_t label;
    {
        TempStr s(name);
        label = search(root, s);
    }
    if (!label) return 0;
    rw::call(lib::SceneUtil_setShadow, {label});
    TempStr s(text);
    rw::call(lib::Text_setString, {label, s});
    return 1;
}
RW_FUNCTION(0x5E762C, "RaidBossHPIndicator::setTextWithName", RaidBossHPIndicator::setTextWithName);

// ------------------------------------------------------------------------------------------- field info
uint32_t RaidBossHPIndicator::setFieldExtendInfo(RaidBossHPIndicator *self, uint32_t coerce, int32_t level,
                                                 int32_t count, uint32_t name) {
    constexpr uint32_t RESULT = 0x8D4;
    uint32_t me = rw::guest_addr(self);
    if (self->enemyType - 1 > 1) return RESULT;   // coerce and boss enemies only
    uint32_t panel = childByName(me, str::PANEL_4, 7);
    setVisible(panel, 0);
    uint32_t panelSize = rw::call(lib::Widget_getCustomSize, {panel});
    {
        float x = -(ldf(panelSize) * 0.5f);
        uint32_t setPosition = rw::load<uint32_t>(rw::load<uint32_t>(panel) + slot::setPosition);
        Frame f;
        rw::call(setPosition, {panel, newVec2(f, x, 0.0f)});
    }
    uint32_t info = loadWidget(self->enemyType == 2 ? 1 : 0, 1);
    vcall(me, slot::addChild, {me, info});
    vcall(info, slot::widget29C, {info, 0});
    removeNodeWithName(self, info, str::ATTRIBUTE_ATK_UP);
    {
        TempStr s(str::RAID_GAUGE_SMALL, 16);
        setVisibleAttribute(self, search(info, s), int32_t(self->type));
    }
    if (self->enemyType == 1) {
        uint32_t icon = findNode(info, str::COERCE_ICON, 10);
        if (coerce == 1) {
            addArmatureFile(rw::call(lib::ArmatureDataManager_getInstance, {}), str::COERCE_ANIM_JSON);
            self->coerceMark = createArmature(str::COERCE_ANIM, 22);
            self->coerceEffect = createArmature(str::COERCE_ANIM, 22);
            uint32_t anim = vcall(self->coerceMark, slot::getAnimation, {self->coerceMark});
            vcall(anim, slot::anim_playWithIndex, {anim, 0, 0xFFFFFFFFu, 0xFFFFFFFFu});
            anim = vcall(self->coerceEffect, slot::getAnimation, {self->coerceEffect});
            vcall(anim, slot::anim_playWithIndex, {anim, 1, 0xFFFFFFFFu, 0xFFFFFFFFu});
            for (uint32_t a : {uint32_t(self->coerceMark), uint32_t(self->coerceEffect)}) {
                uint32_t setPosition = rw::load<uint32_t>(rw::load<uint32_t>(a) + slot::setPosition);
                rw::call(setPosition, {a, vcall(icon, slot::getPosition, {icon})});
            }
            setVisible(self->coerceEffect, 0);
            vcall(panel, slot::addChild, {panel, self->coerceMark});
            vcall(panel, slot::addChild, {panel, self->coerceEffect});
        }
        vcall(icon, slot::removeFromParent, {icon});
    }
    setTextWithName(self, info, str::NAME_LABEL, name);
    Frame f;
    uint32_t countText = stringBuffer(f, {str::D, uint32_t(count)});
    setTextWithName(self, info, str::ENM_NUM_LABEL, countText);
    uint32_t lv = textData(f, int32_t(TEXT_LV));
    uint32_t levelText = stringBuffer(f, {str::D, uint32_t(level)});
    setTextWithName(self, info, str::LV, textDataStr(lv));
    setTextWithName(self, info, str::LV_NUM_LABEL, levelText);
    setTextWithName(self, panel, str::LV, textDataStr(lv));
    setTextWithName(self, panel, str::LV_NUM_LABEL, levelText);

    // stretch the name plate to the name (plus its cap insets) when the name is wider
    uint32_t plate, label;
    if (self->enemyType == 2) {
        TempStr s(str::BOSS_NAME_PANEL, 14);
        plate = search(info, s);
    } else {
        plate = findNode(info, str::NAME_PANEL, 10);
    }
    label = findNode(info, str::NAME_LABEL, 10);
    uint32_t caps = rw::call(lib::ImageView_getCapInsets, {plate});
    uint32_t size = f.alloc(8);
    rw::call(lib::Size_copy, {size, rw::call(lib::Widget_getCustomSize, {label})});
    uint32_t pos = f.alloc(8);
    rw::call(lib::Vec2_copy, {pos, vcall(label, slot::getPosition, {label})});
    st32(pos, ld32(caps));                                   // x = left cap inset
    vcall(label, slot::setPosition, {label, pos});
    float capX = ldf(caps);
    stf(size, ldf(size) + (capX + capX));
    st32(size + 4, ld32(rw::call(lib::Widget_getCustomSize, {plate}) + 4));
    float width = ldf(size);
    float plateWidth = ldf(rw::call(lib::Widget_getCustomSize, {plate}));
    if (!(width <= plateWidth || width != width || plateWidth != plateWidth)) vcall(plate, slot::setSize, {plate, size});
    uint32_t infoPos = f.alloc(8);
    rw::call(lib::Vec2_copy, {infoPos, vcall(info, slot::getPosition, {info})});
    stf(infoPos, ldf(panelSize) * -0.5f);
    vcall(info, slot::setPosition, {info, infoPos});
    sharedRelease(lv);
    return RESULT;
}
RW_FUNCTION(0x5E6F90, "RaidBossHPIndicator::setFieldExtendInfo", RaidBossHPIndicator::setFieldExtendInfo);

void RaidBossHPIndicator::switchModeFieldToBattle(RaidBossHPIndicator *self) {
    uint32_t me = rw::guest_addr(self);
    if (self->enemyType - 1 > 1) return;
    setVisibleNodeWithName(self, me, str::PANEL_4, 1);
    setVisibleNodeWithName(self, me, str::PANEL_18, 0);
    if (self->coerceMark) setVisible(self->coerceMark, 0);
    if (!self->coerceEffect) return;
    setVisible(self->coerceEffect, 1);
    uint32_t anim = vcall(self->coerceEffect, slot::getAnimation, {self->coerceEffect});
    vcall(anim, slot::anim_playWithIndex, {anim, 1, 0xFFFFFFFFu, 0xFFFFFFFFu});
    uint32_t delay = rw::call(lib::DelayTime_create, {0x3FAAAAABu});   // 4/3 s
    uint32_t effect = self->coerceEffect;
    Frame f;
    uint32_t fo = newFunction(f, VT_SWITCH_MODE, {me});
    uint32_t done = rw::call(lib::CallFunc_create, {fo});
    uint32_t seq = rw::call(lib::Sequence_create, {delay, done, 0});
    rw::call(lib::Node_runAction, {effect, seq});
    functionDestroy(fo);
}
RW_FUNCTION(0x5E7728, "RaidBossHPIndicator::switchModeFieldToBattle", RaidBossHPIndicator::switchModeFieldToBattle);

void RaidBossHPIndicator::setSuitableAsTargetEffect(RaidBossHPIndicator *self, uint32_t on) {
    uint32_t a = self->targetArmature;
    if (!a) return;
    uint32_t anim = vcall(a, slot::getAnimation, {a});
    vcall(anim, on == 1 ? slot::anim_resume : slot::anim_pause, {anim, 0});
    setVisible(self->targetArmature, on);
}
RW_FUNCTION(0x5E7880, "RaidBossHPIndicator::setSuitableAsTargetEffect", RaidBossHPIndicator::setSuitableAsTargetEffect);

uint8_t RaidBossHPIndicator::isPlayEndCoeDirection(RaidBossHPIndicator *self) { return self->coerceDirEnd; }
RW_FUNCTION(0x5E78C2, "RaidBossHPIndicator::isPlayEndCoeDirection", RaidBossHPIndicator::isPlayEndCoeDirection);
