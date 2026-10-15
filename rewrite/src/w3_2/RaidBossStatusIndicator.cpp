// RaidBossStatusIndicator: the raid boss's large HP gauge (cocostudio "RaidHpBox_Large": name, level, the HP
// number, up to 18 box cells). A RaidBossHPIndicator whose HP number counts along with the gauge: numberNode is a
// plain Node whose scale is animated from the old HP to the new one and printed by update() every frame.
// gauges (cocos2d::Vector<Node*> at +0x288): [0] AnimationNode -> red bar, [1] timer node, [2] AnimationNode ->
// green bar, [3] green bar "Gauge_GreenL", [4] red bar "Gauge_RedL", [5] TextAtlas "Enemy_HP_Num".
#include "../battleui/RaidBossStatusIndicator.hpp"

using namespace scene;
using namespace raidhp;

namespace {
constexpr uint32_t RESULT_SET_HP = 0xCF4;       // r0 at setRaidBossHP's return: its frame size, left by the epilogue

// __func vtables (+8) of the lambdas, each capturing `this`
constexpr uint32_t VT_HP_HEAL_DONE = 0xF5DAFC;  // setRaidBossHP::$_9
constexpr uint32_t VT_HP_DROP_DONE = 0xF5DB28;  // setRaidBossHP::$_8
constexpr uint32_t VT_ANIM_GREEN = 0xF5DB54;    // init::$_7: scaleX -> green bar percent
constexpr uint32_t VT_ANIM_RED = 0xF5DB80;      // init::$_6: scaleX -> red bar percent

constexpr uint32_t BOX_SLOTS = 18;
constexpr uint32_t RAID_BOX_NAMES = 0xFEF120;       // const char*[18]: the raid box cells
constexpr uint32_t RAID_BOX_PART_NAMES = 0xFEF0F0;  // const char*[4]: their parts
constexpr uint32_t TEXT_LV_ID = 100200002;          // TextData: "Lv"

constexpr uint32_t STR_RAID_HP_BOX_LARGE = 0xE029E8;   // "RaidHpBox_Large"
constexpr uint32_t STR_ENEMY_NAME = 0xE0299A;          // "Enemy_Name"
constexpr uint32_t STR_GAUGE_GREEN_L = 0xE029A5;       // "Gauge_GreenL"
constexpr uint32_t STR_GAUGE_RED_L = 0xE029B2;         // "Gauge_RedL"
constexpr uint32_t STR_ENEMY_HP_NUM = 0xE029BD;        // "Enemy_HP_Num"
constexpr uint32_t STR_ENEMY_HP_RED_NUM = 0xE029CF;    // "Enemy_HPRed_Num"
constexpr uint32_t STR_LV_LABEL = 0xE029DF;            // "LV_Label"
constexpr uint32_t STR_GAUGE_HPMAX = 0xE029F8;         // "Gauge_HpMax"
constexpr uint32_t STR_GAUGE_HP = 0xE02A04;            // "Gauge_Hp"
constexpr uint32_t STR_FLOAT0 = 0xE029CA;              // "%.0f"

constexpr uint32_t Text_setStringFn = 0x83A2DD;        // ui::Text::setString(const string&)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;     // (ui::Text*)
constexpr uint32_t AnimationNode_create = 0x5E6295;
constexpr uint32_t RaidBossHPIndicator_setVisibleAttribute = 0x5E5E69;
constexpr uint32_t RaidBossHPIndicator_setSuitableAsTargetArmature = 0x5E5F61;
constexpr uint32_t RaidBossHPIndicator_setBoxHpGauge = 0x5E6B41;
constexpr uint32_t RaidBossHPIndicator_iniBoxCountText = 0x5E6D25;
constexpr uint32_t RaidBossHPIndicator_setBoxCountText = 0x5E6DDD;
constexpr uint32_t RaidBossHPIndicator_loadWidget = 0x5E4FB5;
constexpr uint32_t ImageView_getCapInsets = 0x839BAD;
constexpr uint32_t Widget_getCustomSize = 0x8262D1;
constexpr uint32_t Size_copy = 0x8DF02D;
constexpr uint32_t Node_create = 0x89372D;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_stopAllActions = 0x893815;
constexpr uint32_t Label_setAdditionalKerning = 0x88697D;
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t NodeVector_clear = 0x496687;
constexpr uint32_t NodeVector_dtor = 0x496317;
constexpr uint32_t V_REMOVE_ALL_CHILDREN_WITH_CLEANUP = 0x13C;
constexpr uint32_t V_GET_VIRTUAL_RENDERER = 0x2D8;
constexpr uint32_t V_SET_SIZE = 0x2B0;
constexpr uint32_t V_SET_SCALE = 0x40, V_GET_SCALE = 0x44;

using Self = RaidBossStatusIndicator;
uint32_t addr(const Self *s) { return rw::guest_addr(s); }
uint32_t g(const Self *s, uint32_t k) { return rw::load<uint32_t>(s->gauges + 4 * k); }

// the fill of the box gauge for `hp`: a full gauge when hp is a non-zero multiple of the box size
float boxRate(uint32_t hp, uint32_t perBox) {
    uint32_t rem = umod(hp, perBox);
    float rate = float(rem) / float(perBox);
    return (hp < perBox || rem != 0) ? rate : 1.0f;
}

// TextAtlas::setString(printf("%.0f", (double)(float)value))
void setAtlasFloat(uint32_t atlas, uint32_t value) {
    double d = float(value);
    uint64_t b;
    std::memcpy(&b, &d, 8);
    Frame f;
    uint32_t buf = stringBuffer(f, {STR_FLOAT0, uint32_t(b), uint32_t(b >> 32)});
    TempStr s(buf);
    rw::call(TextAtlas_setString, {atlas, s});
}
// TextAtlas::setString(printf("%d", value))
void setAtlasInt(uint32_t atlas, uint32_t value) {
    Frame f;
    uint32_t buf = stringBuffer(f, {str::D, value});
    TempStr s(buf);
    rw::call(TextAtlas_setString, {atlas, s});
}

// A short std::string built in place on the stack (as the lambdas do: no __init call)
class InlineStr {
public:
    explicit InlineStr(const char *text) {
        uint32_t n = uint32_t(std::strlen(text));
        s_ = f_.alloc(12);
        rw::store8(rw::frame_ptr<uint8_t>(s_), uint8_t(n << 1));
        for (uint32_t k = 0; k < n; k++) rw::store8(rw::frame_ptr<uint8_t>(s_ + 1 + k), uint8_t(text[k]));
        rw::store8(rw::frame_ptr<uint8_t>(s_ + 1 + n), 0);
    }
    operator uint32_t() const { return s_; }

private:
    Frame f_;
    uint32_t s_;
};

// The broken-box reset: "Gauge_Hp" gets the size of the template layout's "Gauge_Hp" again
void resetGaugeSize(uint32_t me, uint32_t gaugeHp, uint32_t templGaugeHp) {
    if (!gaugeHp) return;
    Frame f;
    uint32_t size = f.alloc(8);
    rw::call(Size_copy, {size, rw::call(Widget_getCustomSize, {templGaugeHp})});
    vcall(gaugeHp, V_SET_SIZE, {gaugeHp, size});
    (void)me;
}

// "Gauge_Hp" shortened to `rate` of the template width (keeping its cap insets)
void setGaugeImageLength(uint32_t image, uint32_t templ, float rate) {
    uint32_t insets = rw::call(ImageView_getCapInsets, {image});
    Frame f;
    uint32_t size = f.alloc(8);
    rw::call(Size_copy, {size, rw::call(Widget_getCustomSize, {templ})});
    float w = rate * (ldf(size) + ldf(insets) * -2.0f);
    rw::save<float>(size, w);
    float ix = ldf(insets);
    rw::save<float>(size, w + (ix + ix));
    vcall(image, V_SET_SIZE, {image, size});
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
// The destructor body shared (folded) with RaidBossHPIndicator: it stores the base vtable.
RaidBossHPIndicator *RaidBossStatusIndicator::destroy(RaidBossHPIndicator *self) {
    uint32_t me = rw::guest_addr(self);
    rw::set<uint32_t>(self, 0, RaidBossHPIndicator::VTABLE);
    rw::call(NodeVector_clear, {me + 0x288});
    vcall(me, V_REMOVE_ALL_CHILDREN_WITH_CLEANUP, {me, 1});
    rw::call(NodeVector_dtor, {me + 0x288});
    rw::call(lib::Node_dtor, {me});
    return self;
}
RW_FUNCTION(0x5E50EC, "RaidBossStatusIndicator::~RaidBossStatusIndicator (folded with ~RaidBossHPIndicator)",
            RaidBossStatusIndicator::destroy);

void RaidBossStatusIndicator::destroyDelete(RaidBossStatusIndicator *self) {
    destroy(self);
    guest::deleteObject(addr(self));
}
RW_FUNCTION(0x5E8F54, "RaidBossStatusIndicator::~RaidBossStatusIndicator (deleting)", RaidBossStatusIndicator::destroyDelete);

RaidBossStatusIndicator *RaidBossStatusIndicator::create(uint32_t name, uint32_t hp, uint32_t hpMax, int32_t level,
                                                         int32_t type, uint32_t hpPerBox) {
    uint32_t me = guest::newObject(SIZE);
    rw::call(0x5E51D5, {me});   // RaidBossHPIndicator::RaidBossHPIndicator
    auto *self = rw::host_ptr<RaidBossStatusIndicator>(me);
    rw::set<uint32_t>(self, 0, VTABLE);
    self->field_32C = 0;
    self->field_330 = 0;
    self->numberNode = 0;
    init(self, name, hp, hpMax, level, type, hpPerBox);
    rw::call(lib::Ref_autorelease, {me});
    return self;
}
RW_FUNCTION(0x5E83AC, "RaidBossStatusIndicator::create", RaidBossStatusIndicator::create);

int32_t RaidBossStatusIndicator::init(RaidBossStatusIndicator *self, uint32_t name, uint32_t hp, uint32_t hpMax,
                                      int32_t level, int32_t type, uint32_t hpPerBox) {
    uint32_t me = addr(self);
    uint32_t layout = rw::call(RaidBossHPIndicator_loadWidget, {2, 1});
    vcall(me, slot::addChild, {me, layout});
    vcall(layout, slot::widget29C, {layout, 0});
    {
        TempStr s(STR_RAID_HP_BOX_LARGE, 15);
        self->boxPanel = search(layout, s);
    }
    self->boxSlots = BOX_SLOTS;
    self->boxNames = RAID_BOX_NAMES;
    self->boxPartNames = RAID_BOX_PART_NAMES;
    // gauges [0] red animator, [1] timer, [2] green animator
    uint32_t anim = rw::call(AnimationNode_create, {});
    setOnScaleX(anim, VT_ANIM_RED, me);
    vcall(me, slot::addChild, {me, anim});
    rw::call(lib::NodeVector_pushBack, {me + 0x288, anim});
    anim = rw::call(AnimationNode_create, {});
    vcall(me, slot::addChild, {me, anim});
    rw::call(lib::NodeVector_pushBack, {me + 0x288, anim});
    anim = rw::call(AnimationNode_create, {});
    setOnScaleX(anim, VT_ANIM_GREEN, me);
    vcall(me, slot::addChild, {me, anim});
    rw::call(lib::NodeVector_pushBack, {me + 0x288, anim});
    // boxes
    self->hpPerBox = hpPerBox;
    uint32_t total = udiv(hpMax, hpPerBox);
    self->boxTotal = total;
    self->boxLeft = boxesFor(hp, hpPerBox);
    if (umod(hpMax, hpPerBox) == 0 && total != 0) self->boxTotal = total - 1;
    // name
    uint32_t text;
    {
        TempStr s(STR_ENEMY_NAME, 10);
        text = search(layout, s);
    }
    rw::call(SceneUtil_setShadow, {text});
    {
        TempStr s(name);
        rw::call(Text_setStringFn, {text, s});
    }
    // gauges [3] green bar, [4] red bar, [5] HP number
    uint32_t node;
    {
        TempStr s(STR_GAUGE_GREEN_L, 12);
        node = search(layout, s);
    }
    rw::call(lib::NodeVector_pushBack, {me + 0x288, node});
    {
        TempStr s(STR_GAUGE_RED_L, 10);
        node = search(layout, s);
    }
    setVisible(node, 1);
    rw::call(lib::NodeVector_pushBack, {me + 0x288, node});
    uint32_t number;
    {
        TempStr s(STR_ENEMY_HP_NUM, 12);
        number = search(layout, s);
    }
    uint32_t label = vcall(number, V_GET_VIRTUAL_RENDERER, {number});
    rw::call(Label_setAdditionalKerning, {label, fbits(-8.0f)});
    setAtlasFloat(number, hpMax);
    rw::call(lib::NodeVector_pushBack, {me + 0x288, number});
    uint32_t redNumber;
    {
        TempStr s(STR_ENEMY_HP_RED_NUM, 15);
        redNumber = search(layout, s);
    }
    // the base functions ignore `this`: the original passes whatever r0 held
    uint32_t r0 = vcall(redNumber, slot::setVisible, {redNumber, 0});
    uint32_t armature = rw::call(RaidBossHPIndicator_setSuitableAsTargetArmature, {r0, layout});
    self->targetArmature = armature;
    rw::call(RaidBossHPIndicator_setVisibleAttribute, {armature, layout, uint32_t(type)});
    // level: "LV_Label" = level, "LV" = TextData "Lv"
    {
        TempStr s(STR_LV_LABEL, 8);
        text = search(layout, s);
    }
    rw::call(SceneUtil_setShadow, {text});
    {
        Frame f;
        uint32_t buf = stringBuffer(f, {str::D, uint32_t(level)});
        TempStr s(buf);
        rw::call(Text_setStringFn, {text, s});
    }
    {
        TempStr s(str::LV, 2);
        text = search(layout, s);
    }
    rw::call(SceneUtil_setShadow, {text});
    {
        Frame f;
        uint32_t lv = f.alloc(8);
        rw::call(lib::TextData_get, {lv, 0, TEXT_LV_ID});
        uint32_t chars = rw::load<uint32_t>(rw::load<uint32_t>(lv) + 4);
        {
            TempStr s(chars);
            rw::call(Text_setStringFn, {text, s});
        }
        uint32_t ctrl = rw::load<uint32_t>(lv + 4);
        if (ctrl) guest::release(ctrl);
    }
    self->hpMax = hpMax;
    self->hp = 0;
    setRaidBossHP(self, hpMax);
    uint32_t panel;
    {
        TempStr s(STR_RAID_HP_BOX_LARGE, 15);
        panel = search(layout, s);
    }
    if (self->hpPerBox < hpMax) {
        // hide the cells past the boxes this boss has
        for (uint32_t k = self->boxTotal; int32_t(k) <= 17; k++) {
            uint32_t cell;
            {
                TempStr s(rw::load<uint32_t>(RAID_BOX_NAMES + 4 * k));
                cell = search(panel, s);
            }
            setVisible(cell, 0);
        }
        if (umod(hpMax, self->hpPerBox) == 0) {
            uint32_t frame;
            {
                TempStr s(STR_GAUGE_HPMAX, 11);
                frame = search(layout, s);
            }
            setVisible(frame, 0);
        }
    } else {
        setVisible(panel, 0);
        uint32_t frame;
        {
            TempStr s(STR_GAUGE_HPMAX, 11);
            frame = search(layout, s);
        }
        setVisible(frame, 0);
    }
    // the gauge shows the last box's share
    uint32_t per = self->hpPerBox;
    uint32_t rem = umod(self->hpMax, per);
    float rate = float(rem) / float(per);
    if (rem == 0) rate = 1.0f;
    setScale(g(self, 0), rate);
    setPercent(g(self, 4), rate * 100.0f);
    uint32_t image;
    {
        TempStr s(STR_GAUGE_HP, 8);
        image = search(layout, s);
    }
    if (image) setGaugeImageLength(image, image, rate);
    rw::call(RaidBossHPIndicator_setBoxHpGauge, {me, self->boxLeft, self->boxLeft});
    {
        TempStr s(str::PANEL_1, 7);
        uint32_t panel1 = search(me, s);   // r0 still holds the panel: the base function ignores `this`
        rw::call(RaidBossHPIndicator_iniBoxCountText, {panel1, panel1, fbits(-6.0f)});
    }
    {
        TempStr s(str::PANEL_1, 7);
        rw::call(RaidBossHPIndicator_setBoxCountText, {me, search(me, s)});
    }
    // the counting HP number
    uint32_t numberNode = rw::call(Node_create, {});
    self->numberNode = numberNode;
    vcall(me, slot::addChild, {me, numberNode});
    uint32_t nn = self->numberNode;
    uint32_t setScaleFn = rw::load<uint32_t>(rw::load<uint32_t>(nn) + V_SET_SCALE);
    rw::call(setScaleFn, {nn, fbits(float(self->hpMax))});
    uint32_t shown = self->hpMax;
    rw::set<float>(self, 0x32C, float(shown));
    self->numberTarget = shown;
    rw::call(Node_scheduleUpdate, {me});
    return 1;
}
RW_FUNCTION(0x5E843C, "RaidBossStatusIndicator::init", RaidBossStatusIndicator::init);

// ------------------------------------------------------------------------------------------- number
// numberNode jumps to the shown HP and scales to the target; the gauge animator `node` scales to `rate`
// (both after half a second when a target was set)
void RaidBossStatusIndicator::NumSync(RaidBossStatusIndicator *self, uint32_t node, float rate) {
    uint32_t nn = self->numberNode;
    uint32_t setScaleFn = rw::load<uint32_t>(rw::load<uint32_t>(nn) + V_SET_SCALE);
    rw::call(setScaleFn, {nn, fbits(float(self->hp))});
    if (self->numberTarget) {
        uint32_t delay = rw::call(lib::DelayTime_create, {fbits(0.5f)});
        uint32_t ease = rw::call(lib::EaseSineOut_create, {rw::call(lib::ScaleTo_create, {fbits(0.5f), fbits(rate)})});
        rw::call(lib::Node_runAction, {node, rw::call(lib::Sequence_create, {delay, ease, 0})});
        rw::call(Node_stopAllActions, {self->numberNode});
        uint32_t target = self->numberNode;
        delay = rw::call(lib::DelayTime_create, {fbits(0.5f)});
        float to = float(self->numberTarget);
        ease = rw::call(lib::EaseSineOut_create, {rw::call(lib::ScaleTo_create, {fbits(0.5f), fbits(to)})});
        rw::call(lib::Node_runAction, {target, rw::call(lib::Sequence_create, {delay, ease, 0})});
    } else {
        uint32_t ease = rw::call(lib::EaseSineOut_create, {rw::call(lib::ScaleTo_create, {fbits(0.5f), fbits(rate)})});
        rw::call(lib::Node_runAction, {node, rw::call(lib::Sequence_create, {ease, 0})});
        rw::call(Node_stopAllActions, {self->numberNode});
        float to = float(self->numberTarget);
        uint32_t target = self->numberNode;
        ease = rw::call(lib::EaseSineOut_create, {rw::call(lib::ScaleTo_create, {fbits(0.5f), fbits(to)})});
        rw::call(lib::Node_runAction, {target, rw::call(lib::Sequence_create, {ease, 0})});
    }
    rw::set<float>(self, 0x32C, float(self->numberTarget));
}
RW_FUNCTION(0x5E8F60, "RaidBossStatusIndicator::NumSync", RaidBossStatusIndicator::NumSync);

// print the counting number: numberNode's scale while it differs from the last target, else the HP
void RaidBossStatusIndicator::update(RaidBossStatusIndicator *self, float) {
    uint32_t atlas = g(self, 5);
    if (!atlas || !self->numberNode) return;
    uint32_t value = self->hp;
    float scale = rw::callf(rw::load<uint32_t>(rw::load<uint32_t>(self->numberNode) + V_GET_SCALE), {self->numberNode});
    if (!(rw::get<float>(self, 0x32C) == scale)) value = uint32_t(f2i(scale));
    setAtlasInt(atlas, value);
}
RW_FUNCTION(0x5E9088, "RaidBossStatusIndicator::update", RaidBossStatusIndicator::update);

// ------------------------------------------------------------------------------------------- setRaidBossHP
uint32_t RaidBossStatusIndicator::setRaidBossHP(RaidBossStatusIndicator *self, uint32_t newHp) {
    uint32_t me = addr(self);
    uint32_t cur = self->hp;
    if (cur == newHp) return RESULT_SET_HP;
    uint32_t atlas = g(self, 5);
    if (atlas) setAtlasInt(atlas, cur);
    self->numberTarget = newHp;

    uint32_t per = self->hpPerBox;
    float fper = float(per);
    cur = self->hp;
    uint32_t rem = umod(cur, per);
    float fromRate = float(rem) / fper;
    bool partial = cur == 0 || rem != 0;
    if (!partial) fromRate = 1.0f;
    float fromShown = atLeastSliver(fromRate);
    uint32_t remNew = umod(newHp, per);
    float toRate = float(remNew) / fper;
    if (newHp != 0 && remNew == 0) toRate = 1.0f;
    float target = atLeastSliver(toRate);

    uint32_t greenBar = g(self, 3);
    uint32_t redBar = g(self, 4);
    uint32_t base = 0;
    if (cur >= per) {
        base = cur - rem;
        if (!partial) base -= per;
    }
    uint32_t top = per + base;

    if (cur > newHp && base >= newHp && int32_t(base) >= 1) {
        // the damage empties this gauge: break a box
        target = 0.0f;
        setPercent(greenBar, 0.0f);
        self->hpTarget = base;
        uint32_t maxFrame;
        {
            TempStr s(str::RAID_GAUGE_HPMAX, 16);
            maxFrame = search(me, s);
        }
        if (maxFrame) setVisible(maxFrame, 0);
        self->boxBroken = 1;
        uint32_t perNow = self->hpPerBox;
        if (self->hpMax > perNow) {
            uint32_t boxes = boxesFor(newHp, perNow);
            rw::call(RaidBossHPIndicator_setBoxHpGauge, {me, self->boxLeft, boxes});
            {
                TempStr s(str::PANEL_1, 7);
                rw::call(RaidBossHPIndicator_setBoxCountText, {me, search(me, s)});
            }
            self->boxLeft = boxes;
            target = 0.0f;
        }
    } else if (cur == 0 || top > newHp || cur >= newHp || self->hpMax == newHp) {
        setPercent(greenBar, target * 100.0f);
        self->hpTarget = newHp;
    } else {
        // healing past this gauge: fill it first
        self->hpTarget = top;
        target = 1.0f;
    }

    if (self->hp > newHp) {
        // damage: the red bar follows after a second
        uint32_t done = callFunc(VT_HP_DROP_DONE, me);
        if (self->gaugeResetPending) {
            if (self->boxBroken) {
                uint32_t templ = rw::call(RaidBossHPIndicator_loadWidget, {2, 0});
                uint32_t gaugeHp;
                {
                    TempStr s(STR_GAUGE_HP, 8);
                    gaugeHp = search(me, s);
                }
                uint32_t templGauge;
                {
                    TempStr s(STR_GAUGE_HP, 8);
                    templGauge = search(templ, s);
                }
                resetGaugeSize(me, gaugeHp, templGauge);
                self->boxBroken = 0;
                self->gaugeResetPending = 0;
                self->dropCount = 0;
            }
        } else if (self->boxBroken) {
            self->gaugeResetPending = 1;
        }
        if (self->dropCount == 0) setPercent(redBar, fromShown * 100.0f);
        uint32_t redAnim = g(self, 0);
        stopAllActions(redAnim);
        if (self->dropCount == 0) setScale(redAnim, fromShown);
        NumSync(self, redAnim, target);
        self->dropCount += 1;
        uint32_t timer = g(self, 1);
        stopAllActions(timer);
        runAfter(timer, 1.0f, done);
    } else {
        int32_t bytes = int32_t(self->gaugesEnd - self->gauges);
        if (self->hp < newHp) {
            if (bytes >= 21) {
                setAtlasInt(g(self, 5), newHp);
            } else {
                // healing: the green bar fills after a second
                uint32_t done = callFunc(VT_HP_HEAL_DONE, me);
                uint32_t greenAnim = g(self, 2);
                stopAllActions(greenAnim);
                setScale(greenAnim, fromShown);
                setPercent(greenBar, fromShown * 100.0f);
                NumSync(self, greenAnim, target);
                uint32_t timer = g(self, 1);
                stopAllActions(timer);
                runAfter(timer, 1.0f, done);
            }
        } else if (bytes >= 21) {
            setAtlasInt(g(self, 5), newHp);
        }
    }
    self->hp = newHp;
    return RESULT_SET_HP;
}
RW_FUNCTION(0x5E9190, "RaidBossStatusIndicator::setRaidBossHP", RaidBossStatusIndicator::setRaidBossHP);

// ------------------------------------------------------------------------------------------- direction steps
void RaidBossStatusIndicator::setRaidBossHPDownDir(RaidBossStatusIndicator *self) {
    uint32_t greenBar = g(self, 3), redBar = g(self, 4), redAnim = g(self, 0);
    setPercent(redBar, 100.0f);
    stopAllActions(redAnim);
    uint32_t cur = self->hp;
    float shown = atLeastSliver(boxRate(cur, self->hpPerBox));
    setPercent(greenBar, shown * 100.0f);
    uint32_t hp = self->hp;
    self->hpTarget = hp;
    if (hp != 0) {
        setScale(redAnim, 1.0f);
        NumSync(self, redAnim, shown);
        setRaidBossHPDirEnd(self);
    } else {
        self->boxLeft = 0;
        setPercent(greenBar, 0.0f);
        setPercent(redBar, 0.0f);
        setRaidBossHPDirEnd(self);
    }
    setAtlasFloat(g(self, 5), self->hp);
}
RW_FUNCTION(0x5E995C, "RaidBossStatusIndicator::setRaidBossHPDownDir", RaidBossStatusIndicator::setRaidBossHPDownDir);

void RaidBossStatusIndicator::setRaidBossHPDirEnd(RaidBossStatusIndicator *self) {
    uint32_t me = addr(self);
    self->dropCount = 0;
    if (self->hpMax > self->hpPerBox) {
        rw::call(RaidBossHPIndicator_setBoxHpGauge, {me, self->boxLeft, self->boxLeft});
        TempStr s(str::PANEL_1, 7);
        rw::call(RaidBossHPIndicator_setBoxCountText, {me, search(me, s)});
    }
}
RW_FUNCTION(0x5E9BE4, "RaidBossStatusIndicator::setRaidBossHPDirEnd", RaidBossStatusIndicator::setRaidBossHPDirEnd);

void RaidBossStatusIndicator::setRaidHPUpDir(RaidBossStatusIndicator *self) {
    uint32_t me = addr(self);
    uint32_t greenAnim = g(self, 2), greenBar = g(self, 3);
    setPercent(g(self, 4), 0.0f);
    stopAllActions(greenAnim);
    float shown = atLeastSliver(boxRate(self->hp, self->hpPerBox));
    if (self->boxLeft == self->boxTotal) {
        // back in the top box: the max-HP frame again and the gauge at the last box's length
        uint32_t maxFrame;
        {
            TempStr s(str::RAID_GAUGE_HPMAX, 16);
            maxFrame = search(me, s);
        }
        if (maxFrame) setVisible(maxFrame, 1);
        uint32_t hpMax = self->hpMax;
        uint32_t per = self->hpPerBox;
        uint32_t templ = rw::call(RaidBossHPIndicator_loadWidget, {2, 0});
        uint32_t templGauge;
        float rate;
        {
            TempStr s(STR_GAUGE_HP, 8);
            uint32_t rem = umod(hpMax, per);
            float r = float(rem) / float(per);
            rate = rem ? r : 1.0f;
            templGauge = search(templ, s);
        }
        uint32_t image;
        {
            TempStr s(STR_GAUGE_HP, 8);
            image = search(me, s);
        }
        if (image) setGaugeImageLength(image, templGauge, rate);
    }
    self->hpTarget = self->hp;
    setPercent(greenBar, 2.0f);
    setScale(greenAnim, 0.0f);
    NumSync(self, greenAnim, shown);
}
RW_FUNCTION(0x5E9CAC, "RaidBossStatusIndicator::setRaidHPUpDir", RaidBossStatusIndicator::setRaidHPUpDir);

void RaidBossStatusIndicator::setRaidBossHPDownJustDir(RaidBossStatusIndicator *self) {
    uint32_t redAnim = g(self, 0), greenBar = g(self, 3);
    setPercent(g(self, 4), 100.0f);
    setPercent(greenBar, 100.0f);
    stopAllActions(redAnim);
    setRaidBossHPDirEnd(self);
    uint32_t atlas = g(self, 5);
    setAtlasFloat(atlas, self->hp);
}
RW_FUNCTION(0x5E9FB0, "RaidBossStatusIndicator::setRaidBossHPDownJustDir",
            RaidBossStatusIndicator::setRaidBossHPDownJustDir);

void RaidBossStatusIndicator::setRaidHPUpJustDir(RaidBossStatusIndicator *self) {
    uint32_t redAnim = g(self, 0), greenBar = g(self, 3);
    setPercent(g(self, 4), 0.0f);
    setPercent(greenBar, 100.0f);
    stopAllActions(redAnim);
    self->dropCount = 0;
}
RW_FUNCTION(0x5EA0C0, "RaidBossStatusIndicator::setRaidHPUpJustDir", RaidBossStatusIndicator::setRaidHPUpJustDir);

void RaidBossStatusIndicator::setRaidHPUpDirEnd(RaidBossStatusIndicator *self) {
    uint32_t per = self->hpPerBox;
    float rate = float(umod(self->hp, per)) / float(per);
    setPercent(g(self, 4), rate * 100.0f);
}
RW_FUNCTION(0x5EA100, "RaidBossStatusIndicator::setRaidHPUpDirEnd", RaidBossStatusIndicator::setRaidHPUpDirEnd);

// ------------------------------------------------------------------------------------------- lambdas
namespace RaidBossStatusIndicator_w3 {
Self *owner(uint32_t lambda) { return rw::host_ptr<Self>(rw::load<uint32_t>(lambda + 4)); }

// setRaidBossHP::$_9 (heal finished): refresh the boxes when a box filled up, then the next step
void healDone(uint32_t lambda) {
    Self *self = owner(lambda);
    uint32_t me = addr(self);
    uint32_t per = self->hpPerBox, target = self->hpTarget, hp = self->hp;
    bool refresh = hp > target;
    if (!refresh) {
        uint32_t rem = umod(hp, per);
        refresh = hp == target && hp != 0 && rem == 0;
    }
    if (refresh && self->hpMax > per) {
        uint32_t boxes = boxesFor(hp, per);
        rw::call(RaidBossHPIndicator_setBoxHpGauge, {me, self->boxLeft, boxes});
        {
            InlineStr s("Panel_1");
            rw::call(RaidBossHPIndicator_setBoxCountText, {me, search(me, s)});
        }
        self->boxLeft = boxes;
        target = self->hpTarget;
        hp = self->hp;
    }
    if (hp > target) {
        RaidBossStatusIndicator::setRaidHPUpDir(self);
        return;
    }
    per = self->hpPerBox;
    uint32_t rem = umod(hp, per);
    if (hp == target && hp != 0 && rem == 0) {
        RaidBossStatusIndicator::setRaidHPUpJustDir(self);
        return;
    }
    uint32_t gauges = self->gauges;
    float rate = float(rem) / float(per);
    setPercent(rw::load<uint32_t>(gauges + 0x10), rate * 100.0f);
    setAtlasFloat(g(self, 5), self->hp);
}

// setRaidBossHP::$_8 (damage finished): reset a broken gauge, then the next step
void dropDone(uint32_t lambda) {
    Self *self = owner(lambda);
    uint32_t me = addr(self);
    if (self->boxBroken && self->gaugeResetPending) {
        uint32_t templ = rw::call(RaidBossHPIndicator_loadWidget, {2, 0});
        uint32_t gaugeHp;
        {
            InlineStr s("Gauge_Hp");
            gaugeHp = search(me, s);
        }
        uint32_t templGauge;
        {
            InlineStr s("Gauge_Hp");
            templGauge = search(templ, s);
        }
        resetGaugeSize(me, gaugeHp, templGauge);
        self->boxBroken = 0;
        self->gaugeResetPending = 0;
        self->dropCount = 0;
    }
    uint32_t target = self->hpTarget, hp = self->hp;
    if (hp < target) {
        RaidBossStatusIndicator::setRaidBossHPDownDir(self);
        return;
    }
    uint32_t rem = umod(hp, self->hpPerBox);
    if (hp == target && hp != 0 && rem == 0) {
        RaidBossStatusIndicator::setRaidBossHPDownJustDir(self);
        return;
    }
    RaidBossStatusIndicator::setRaidBossHPDirEnd(self);
    setAtlasFloat(g(self, 5), self->hp);
}

// init::$_7 / $_6 (AnimationNode scaleX): green / red bar percent = scale * 100
void greenScaleX(uint32_t lambda, const float *scale) { setPercent(g(owner(lambda), 3), *scale * 100.0f); }
void redScaleX(uint32_t lambda, const float *scale) { setPercent(g(owner(lambda), 4), *scale * 100.0f); }
}  // namespace RaidBossStatusIndicator_w3

RW_FUNCTION(0x5EA1B4, "RaidBossStatusIndicator::setRaidBossHP::$_9 operator()", RaidBossStatusIndicator_w3::healDone);
RW_FUNCTION(0x5EA49C, "RaidBossStatusIndicator::setRaidBossHP::$_8 operator()", RaidBossStatusIndicator_w3::dropDone);
RW_FUNCTION(0x5EA788, "RaidBossStatusIndicator::init::$_7 operator()", RaidBossStatusIndicator_w3::greenScaleX);
RW_FUNCTION(0x5EA840, "RaidBossStatusIndicator::init::$_6 operator()", RaidBossStatusIndicator_w3::redScaleX);
