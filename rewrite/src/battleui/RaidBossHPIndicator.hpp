// RaidBossHPIndicator: the HP gauge shown over an enemy on the action map and in raid battles (a cocos2d::Node).
// One gauge is `hpPerBox` HP; an enemy with more HP keeps the rest in "boxes" (small cells next to the gauge,
// up to 4 or 8). The gauge itself is a green LoadingBar (current HP) over a red one that catches up after a hit,
// both driven by AnimationNode children whose scaleX is animated (their std::function sets the bar percent).
// It also shows the buff / debuff icon cycling through the active ones (bit i + 1 of buffFlags = buff i, with
// buffTurns[i] the turn count frame) and, for "coerce" enemies, the coerce mark.
//
// The file also holds the small classes of the same translation unit: AnimationNode (a Node whose setScaleX
// calls a std::function<void(float)>), an anonymous Arc gauge, and the cached cocostudio widgets.
#pragma once

#include "BattleUi.hpp"

struct RaidBossHPIndicator {
    const uint8_t node[0x270];                    // cocos2d::Node
    rw::Field<uint32_t> hp;                 // +0x270 HP currently shown
    rw::Field<uint32_t> hpMax;              // +0x274
    rw::Field<uint32_t> hpPerBox;           // +0x278 HP of one full gauge
    rw::Field<uint32_t> hpTarget;           // +0x27C HP the running gauge animation ends at
    rw::Field<uint32_t> boxTotal;           // +0x280 boxes at full HP
    rw::Field<uint32_t> boxLeft;            // +0x284 boxes still filled
    rw::Field<uint32_t> gauges;             // +0x288 cocos2d::Vector<Node*>: [0] AnimationNode driving the red
    rw::Field<uint32_t> gaugesEnd;          // +0x28C   bar, [1] timer node, [2] AnimationNode driving the green
    rw::Field<uint32_t> gaugesCap;          // +0x290   bar, [3] green bar, [4] red bar, [5] HP number (status)
    rw::Field<uint32_t> boxPanel;           // +0x294 the panel holding the box cells
    rw::Field<uint32_t> boxSlots;           // +0x298 box cells in the panel (4 or 8; 18 for the raid status)
    rw::Field<uint32_t> enemyType;          // +0x29C 0 normal, 1 coerce, 2 boss, 3 status icons only (init)
    rw::Field<uint32_t> type;               // +0x2A0 attribute (type_01..03 visible)
    rw::Field<uint32_t> buffIcon;           // +0x2A4 cocostudio::Armature: the buff / debuff icon
    rw::Field<uint32_t> buffTurnIcon;       // +0x2A8 cocostudio::Armature: its turn count (child of buffIcon)
    rw::Field<uint32_t> coerceMark;         // +0x2AC cocostudio::Armature (coerce enemies)
    rw::Field<uint32_t> coerceEffect;       // +0x2B0 cocostudio::Armature (coerce enemies)
    rw::Field<uint8_t> coerceDirEnd;        // +0x2B4 the coerce direction finished
    rw::Field<uint8_t> boxBroken;           // +0x2B5 a box was used up (the max-HP frame was hidden)
    rw::Field<uint8_t> gaugeResetPending;   // +0x2B6 reset the gauge length when the drop animation ends
    const uint8_t pad_2B7[1];
    rw::Field<uint32_t> dropCount;          // +0x2B8 damage animations started since the last reset
    rw::Field<float> gaugeWidth;            // +0x2BC cocos2d::Size of "Raid_Gauge_Hp" at init
    rw::Field<float> gaugeHeight;           // +0x2C0
    rw::Field<uint32_t> targetArmature;     // +0x2C4 "suitable as target" (attribute advantage) armature
    rw::Field<uint32_t> boxNames;           // +0x2C8 const char*[]: box cell node names
    rw::Field<uint32_t> boxPartNames;       // +0x2CC const char*[4]: the parts of a cell (full, first, second, rest)
    rw::Field<uint32_t> buffFlags;          // +0x2D0 bit i + 1: buff i active
    rw::Field<uint32_t> field_2D4;          // +0x2D4
    rw::Field<uint32_t> buffShown;          // +0x2D8 buff index the icon shows
    rw::Field<int32_t> buffTurns[20];       // +0x2DC turn count frame per buff (20 = none)
    rw::Field<uint32_t> field_32C;          // +0x32C (RaidBossStatusIndicator: the HP number shown, float)

    static constexpr uint32_t SIZE = 0x330;
    static constexpr uint32_t VTABLE = 0xF9445C;   // _ZTV19RaidBossHPIndicator + 8

    // Load (and cache) the cocostudio widget `index` (0..4); clone = 1 returns a copy of the cached one.
    static uint32_t loadWidget(int32_t index, int32_t clone);
    static void firstCall();
    static void finalCall();

    static void destroyDelete(RaidBossHPIndicator *self);
    static RaidBossHPIndicator *create(int32_t enemyType, int32_t type, uint32_t hpMax, uint32_t hpPerBox);
    static RaidBossHPIndicator *construct(RaidBossHPIndicator *self);
    static int32_t init(RaidBossHPIndicator *self, int32_t enemyType, int32_t type, uint32_t hpMax,
                        uint32_t hpPerBox);
    static void NumSync(RaidBossHPIndicator *self, uint32_t node, float scale);
    static uint32_t setStatusIconArmature(RaidBossHPIndicator *self, uint32_t parent);
    static uint32_t getGaugeBoxCount(RaidBossHPIndicator *self, uint32_t hp);
    static void setVisibleAttribute(RaidBossHPIndicator *self, uint32_t node, int32_t type);
    static uint32_t setSuitableAsTargetArmature(RaidBossHPIndicator *self, uint32_t widget);
    static uint32_t removeNodeWithName(RaidBossHPIndicator *self, uint32_t root, uint32_t name);
    static uint32_t setHP(RaidBossHPIndicator *self, uint32_t hp);
    static uint32_t setVisibleNodeWithName(RaidBossHPIndicator *self, uint32_t root, uint32_t name, uint32_t visible);
    static float setGaugeLength(RaidBossHPIndicator *self, uint32_t image, float rate);
    static void setBoxHpGauge(RaidBossHPIndicator *self, uint32_t boxesBefore, uint32_t boxesNow);
    static void iniBoxCountText(RaidBossHPIndicator *self, uint32_t panel, float kerning);
    static void setBoxCountText(RaidBossHPIndicator *self, uint32_t panel);
    static uint32_t setFieldExtendInfo(RaidBossHPIndicator *self, uint32_t coerce, int32_t level, int32_t count,
                                       uint32_t name);
    static uint32_t setTextWithName(RaidBossHPIndicator *self, uint32_t root, uint32_t name, uint32_t text);
    static void switchModeFieldToBattle(RaidBossHPIndicator *self);
    static void setSuitableAsTargetEffect(RaidBossHPIndicator *self, uint32_t on);
    static uint8_t isPlayEndCoeDirection(RaidBossHPIndicator *self);
    static void setHPDownDir(RaidBossHPIndicator *self);
    static void setHPDirEnd(RaidBossHPIndicator *self);
    static void setHPUpDir(RaidBossHPIndicator *self);
    static void setHPDownJustDir(RaidBossHPIndicator *self);
    static void setHPUpJustDir(RaidBossHPIndicator *self);
    static void setHPUpDirEnd(RaidBossHPIndicator *self);
    static void set_buf_debuf(RaidBossHPIndicator *self, int32_t idx, int32_t turns);
    static void Next_Disp(RaidBossHPIndicator *self);
    static void clr_buf_debuf(RaidBossHPIndicator *self, int32_t idx);
    static void clr_buf_debuf_all(RaidBossHPIndicator *self);
    static void set_attack_buff_status(RaidBossHPIndicator *self, int32_t a, int32_t b, int32_t c, int32_t d);
    static void set_defence_buff_status(RaidBossHPIndicator *self, int32_t a, int32_t b, int32_t c, int32_t d);
};
RW_LAYOUT(RaidBossHPIndicator, hp, 0x270);
RW_LAYOUT(RaidBossHPIndicator, gauges, 0x288);
RW_LAYOUT(RaidBossHPIndicator, enemyType, 0x29C);
RW_LAYOUT(RaidBossHPIndicator, coerceDirEnd, 0x2B4);
RW_LAYOUT(RaidBossHPIndicator, dropCount, 0x2B8);
RW_LAYOUT(RaidBossHPIndicator, targetArmature, 0x2C4);
RW_LAYOUT(RaidBossHPIndicator, buffFlags, 0x2D0);
RW_LAYOUT(RaidBossHPIndicator, buffTurns, 0x2DC);
RW_SIZE(RaidBossHPIndicator, 0x330);

// A cocos2d::Node whose setScaleX also calls onScaleX(scaleX): scale actions on it animate whatever the
// function drives (the gauge bars).
struct AnimationNode {
    const uint8_t node[0x270];                    // cocos2d::Node
    const uint8_t onScaleX[16];                   // +0x270 std::function<void(float)> buffer
    rw::Field<uint32_t> onScaleXFn;         // +0x280 its callable (null = empty)
    rw::Field<uint32_t> pad_284;

    static constexpr uint32_t SIZE = 0x288;
    static constexpr uint32_t VTABLE = 0xF94A2C;   // _ZTV13AnimationNode + 8

    static AnimationNode *create();
    static AnimationNode *destroy(AnimationNode *self);
    static void destroyDelete(AnimationNode *self);
    static void setScaleX(AnimationNode *self, float scaleX);
};
RW_LAYOUT(AnimationNode, onScaleXFn, 0x280);
RW_SIZE(AnimationNode, 0x288);

// Helpers shared by the RaidBossHPIndicator / RaidBossStatusIndicator files.
namespace raidhp {

namespace lib {
constexpr uint32_t aeabi_memset = 0x40E7A0;          // __aeabi_memset(dst, n, c)
constexpr uint32_t udivsi3 = 0xCD5DF9;               // __udivsi3 (division by zero handled by the guest)
constexpr uint32_t umodsi3 = 0xCD5F3D;
constexpr uint32_t Node_ctor = 0x892531;
constexpr uint32_t Node_dtor = 0x8927ED;
constexpr uint32_t Node_init = 0x892B69;
constexpr uint32_t Node_create = 0x89372D;
constexpr uint32_t Node_setScaleX = 0x892F39;
constexpr uint32_t Node_stopAllActions = 0x893815;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Size_copy = 0x8DF02D;             // Size::Size(const Size&)
constexpr uint32_t Size_assign = 0x8DF041;           // Size::operator=(const Size&)
constexpr uint32_t SIZE_ZERO = 0x10068C0;            // cocos2d::Size::ZERO
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t Vec2_subtract = 0x8E530F;         // Vec2::subtract(const Vec2&) (in place)
constexpr uint32_t NodeVector_pushBack = 0x5CA3F1;   // cocos2d::Vector<Node*>::pushBack(Node*)
constexpr uint32_t NodeVector_clear = 0x496687;
constexpr uint32_t NodeVector_dtor = 0x496317;
constexpr uint32_t SpriteVector_dtor = 0x5E4761;
constexpr uint32_t FloatFunction_swap = 0x5E40BD;    // std::function<void(float)>::swap
constexpr uint32_t FloatFunction_call = 0x4F8E89;    // std::function<void(float)>::operator()(float)
constexpr uint32_t Widget_getCustomSize = 0x8262D1;  // -> const Size&
constexpr uint32_t Widget_clone = 0x8272B9;
constexpr uint32_t ImageView_getCapInsets = 0x839BAD;  // -> const Rect&
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t LoadingBar_setPercent = 0x83BB05;   // (bar, float)
constexpr uint32_t Label_setAdditionalKerning = 0x88697D;
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t ScaleTo_create = 0x86A8CD;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t Sequence_create = 0x867999;       // (action, ..., nullptr)
constexpr uint32_t CallFunc_create = 0x866B39;       // (const std::function<void()>&)
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;  // (mgr, const string&)
constexpr uint32_t Armature_create = 0x79F3C7;       // (const string&)
constexpr uint32_t FileUtils_getInstance = 0xA3F44D;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5;
constexpr uint32_t GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t StringBuffer_ctor = 0x49140D;     // StringBuffer(this, fmt, ...)
constexpr uint32_t TextData_get = 0x4928AD;          // (shared_ptr out, Type, id)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
}  // namespace lib

// cocos2d::Node / Widget / Armature virtual slots (byte offsets into the vtable)
namespace slot {
constexpr uint32_t deletingDtor = 0x04;
constexpr uint32_t setZ10 = 0x10;              // called with 10 on the box number
constexpr uint32_t setScale = 0x40;
constexpr uint32_t getScale = 0x44;
constexpr uint32_t setScaleXY = 0x48;          // setScale(float, float)
constexpr uint32_t getScaleX = 0x2C, getScaleY = 0x34;
constexpr uint32_t setPosition = 0x4C, getPosition = 0x54;
constexpr uint32_t setVisible = 0xB0;
constexpr uint32_t setRotation = 0xB8;
constexpr uint32_t addChild = 0xF0, addChildZ = 0xF4;
constexpr uint32_t getChildByName = 0x104;
constexpr uint32_t removeFromParent = 0x124;
constexpr uint32_t removeChild = 0x12C;        // (child, cleanup)
constexpr uint32_t removeAllChildrenWithCleanup = 0x13C;
constexpr uint32_t getAnimation = 0x298;       // cocostudio::Armature -> ArmatureAnimation*
constexpr uint32_t widget29C = 0x29C;          // ui::Widget virtual called with 0 on the loaded layouts
constexpr uint32_t setSize = 0x2B0;            // ui::Widget / ImageView: set the content size (const Size&)
constexpr uint32_t getVirtualRenderer = 0x2D8;
constexpr uint32_t isFileExist = 0x58;         // FileUtils
// cocostudio::ArmatureAnimation
constexpr uint32_t anim_play = 0x68;           // play(const string&, durationTo, loop)
constexpr uint32_t anim_playWithIndex = 0x70;  // (index, durationTo, loop)
constexpr uint32_t anim_resume = 0x7C, anim_pause = 0x80;
}  // namespace slot

// string literals of the translation unit
namespace str {
constexpr uint32_t PANEL_4 = 0xE02B57;               // "Panel_4"
constexpr uint32_t PANEL_18 = 0xE02B05;              // "Panel_18"
constexpr uint32_t PANEL_1 = 0xE02A0D;               // "Panel_1"
constexpr uint32_t RAID_GAUGE_HP = 0xE02BC2;         // "Raid_Gauge_Hp"
constexpr uint32_t RAID_GAUGE_HPMAX = 0xE02B5F;      // "Raid_Gauge_HpMax"
constexpr uint32_t RAID_GAUGE_SMALL = 0xE02BB1;      // "Raid_Gauge_Small"
constexpr uint32_t RAID_GAUGE_LV = 0xE02B94;         // "Raid_Gauge_Lv"
constexpr uint32_t BUFF_DEBUFF_ICON = 0xE02BA2;      // "BuffDeBuffIcon"
constexpr uint32_t GAUGE_GREEN_S = 0xE028F9;         // "Gauge_GreenS"
constexpr uint32_t GAUGE_RED_S = 0xE02906;           // "Gauge_RedS"
constexpr uint32_t ACT_MAP_HP_BOX = 0xE02B88;        // "ActMapHpBox"
constexpr uint32_t ACT_MAP_HP_BOX_BG = 0xE02B7A;     // "ActMapHpBoxBg"
constexpr uint32_t RAID_HP_BOX = 0xE02B70;           // "RaidHpBox"
constexpr uint32_t BUFF_ANIM_JSON = 0xE02967;        // "cocostudio/publish/ActMapBuffDeBuffAnim.ExportJson"
constexpr uint32_t BUFF_ANIM = 0xE01DFC;             // "ActMapBuffDeBuffAnim"
constexpr uint32_t TYPE_FMT = 0xE0295D;              // "type_%02d"
constexpr uint32_t ATTRIBUTE_ATK_UP = 0xE02AF6;      // "AttributeAtkUP"
constexpr uint32_t EXPORT_JSON_FMT = 0xE01EED;       // "cocostudio/publish/%s.ExportJson"
constexpr uint32_t ACTION_MAP_ANIMATION = 0xE01C44;  // "ActionMapAnimation"
constexpr uint32_t BOX_NUM = 0xE02AEE;               // "Box_Num"
constexpr uint32_t BOX_NUM_PLUS = 0xE02AE2;          // "Box_NumPlus"
constexpr uint32_t D = 0xDF86DE;                     // "%d"
constexpr uint32_t COERCE_ICON = 0xE02B4C;           // "CoerceIcon"
constexpr uint32_t COERCE_ANIM_JSON = 0xE02911;      // "cocostudio/publish/ActionMapCoerceEnmAnim.ExportJson"
constexpr uint32_t COERCE_ANIM = 0xE02946;           // "ActionMapCoerceEnmAnim"
constexpr uint32_t NAME_LABEL = 0xE02B41;            // "Name_Label"
constexpr uint32_t ENM_NUM_LABEL = 0xE02B34;         // "EnmNum_Label"
constexpr uint32_t LV = 0xDF8A6E;                    // "LV"
constexpr uint32_t LV_NUM_LABEL = 0xE02B28;          // "LV_NumLabel"
constexpr uint32_t BOSS_NAME_PANEL = 0xE02B19;       // "BossName_Panel"
constexpr uint32_t NAME_PANEL = 0xE02B0E;            // "Name_Panel"
constexpr uint32_t WIDGET_JSON_FMT = 0xE02C3C;       // "cocostudio/publish/%s.json"
}  // namespace str

constexpr uint32_t BOX_PART_NAMES = 0xFEF0E0;   // const char*[8]: box cell parts (HpBox_RedS...), then
constexpr uint32_t BOX_NAMES = 0xFEF100;        // const char*[8]: the action-map box cells
constexpr uint32_t TEXT_LV = 100200002;         // TextData id of "Lv"


// ARM unsigned division / remainder (the guest's own helpers when dividing by zero)
inline uint32_t udiv(uint32_t a, uint32_t b) { return b ? a / b : rw::call(lib::udivsi3, {a, b}); }
inline uint32_t umod(uint32_t a, uint32_t b) { return b ? a % b : rw::call(lib::umodsi3, {a, b}); }
// Thumb register shift: 1 << n with n taken from the low byte (0 from 32 on)
inline uint32_t bit(uint32_t n) { n &= 0xFF; return n >= 32 ? 0u : 1u << n; }

// Boxes to show for `hp`: hp / perBox, one less when it divides exactly (the gauge shows the last one).
inline uint32_t boxesFor(uint32_t hp, uint32_t perBox) {
    uint32_t q = udiv(hp, perBox);
    bool exact = umod(hp, perBox) == 0;
    return (q != 0 && exact) ? q - 1 : q;
}
// Small rates still show a sliver of the bar: (0, 0.02) -> 0.02.
inline float atLeastSliver(float rate) { return (rate < 0.02f && rate > 0.0f) ? 0.02f : rate; }

using scene::vcall;
using scene::fbits;
// node->getChildByName(literal)
inline uint32_t childByName(uint32_t node, uint32_t name, uint32_t len) {
    scene::Frame f;
    uint32_t s = scene::newStr(f, name, len);
    uint32_t r = vcall(node, slot::getChildByName, {node, s});
    scene::strFree(s);
    return r;
}
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, slot::setVisible, {node, visible}); }
inline void setPercent(uint32_t bar, float percent) { rw::call(lib::LoadingBar_setPercent, {bar, fbits(percent)}); }
// the Vector<Node*> element k
inline uint32_t gauge(const RaidBossHPIndicator *self, uint32_t k) { return rw::load<uint32_t>(self->gauges + 4 * k); }

// A std::function<void()> holding a one-word lambda (vtable, captured this) on the guest stack: CallFunc
inline uint32_t callFunc(uint32_t vtable, uint32_t capture) {
    scene::Frame f;
    uint32_t fo = scene::newFunction(f, vtable, {capture});
    uint32_t action = rw::call(lib::CallFunc_create, {fo});
    scene::functionDestroy(fo);
    return action;
}
// Give an AnimationNode the one-word lambda (vtable, captured this) as its onScaleX function
inline void setOnScaleX(uint32_t animNode, uint32_t vtable, uint32_t capture) {
    scene::Frame f;
    uint32_t fo = scene::newFunction(f, vtable, {capture});
    rw::call(lib::FloatFunction_swap, {fo, animNode + 0x270});
    scene::functionDestroy(fo);
}

inline uint32_t search(uint32_t root, uint32_t name) { return SceneUtil::searchNodeByName(root, name); }

// A std::string temporary built from a literal (guest address, length), freed when it goes out of scope: keeps
// the original's order of allocation and release when the literal needs a heap block (11 chars and more).
class TempStr {
public:
    TempStr(uint32_t text, uint32_t len) { s_ = scene::newStr(f_, text, len); }
    explicit TempStr(uint32_t text) { s_ = scene::newStr(f_, text); }
    ~TempStr() { scene::strFree(s_); }
    TempStr(const TempStr &) = delete;
    TempStr &operator=(const TempStr &) = delete;
    operator uint32_t() const { return s_; }

private:
    scene::Frame f_;
    uint32_t s_;
};

// TextAtlas::setString(printf("%d", value))
inline void setAtlasNumber(uint32_t atlas, int32_t value) {
    scene::Frame f;
    uint32_t buf = scene::stringBuffer(f, {str::D, uint32_t(value)});
    TempStr s(buf);
    rw::call(lib::TextAtlas_setString, {atlas, s});
}

// Sequence(DelayTime(seconds), action, nullptr) run on node
inline void runAfter(uint32_t node, float seconds, uint32_t action) {
    uint32_t delay = rw::call(lib::DelayTime_create, {fbits(seconds)});
    uint32_t seq = rw::call(lib::Sequence_create, {delay, action, 0});
    rw::call(lib::Node_runAction, {node, seq});
}
inline void stopAllActions(uint32_t node) { rw::call(lib::Node_stopAllActions, {node}); }
inline void setScale(uint32_t node, float scale) { vcall(node, slot::setScale, {node, fbits(scale)}); }

}  // namespace raidhp
