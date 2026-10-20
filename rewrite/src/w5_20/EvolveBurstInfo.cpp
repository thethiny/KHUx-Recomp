// SceneEvolveMedal::initBurstInfo: the Evolve screen's burst (special attack) panel for the evolved medal - its
// titles, the burst name, the ability type with the multiplier text by its kind, the cost, the target icon, the
// description ticker and the guilt circle. When the medal has a burst it compares it with the current medal's (the
// second shared_ptr): the type and multiplier, cost and ticker come from that medal's burst, and with `mark` set the
// values that change are colored.
#include "../scene/SceneUi.hpp"

#include <cstring>
#include <tuple>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setStatusChangeColor = 0x4659F1;
constexpr uint32_t SceneUtil_runTicker = 0x465AB9;
constexpr uint32_t SceneUtil_setWarningColor = 0x4686A9;
constexpr uint32_t SceneUtil_updateMixCircle = 0x465C85;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t Burst_create = 0x77A491;
constexpr uint32_t aeabi_idiv = 0xCD5E8D;
}  // namespace g

constexpr uint32_t S_Empty = 0xDF8A1A, S_FmtInt = 0xDF86DE, S_FmtSS = 0xDF8BBA;
constexpr uint32_t S_Burst = 0xDF8AC4, S_BurstNameLabel = 0xDF8ACA, S_Ability = 0xDF8ADA;
constexpr uint32_t S_AbilityTypeLabel = 0xDF8AE2, S_Multiply = 0xDF8AF5, S_MultiplyNumLabel = 0xDF8B01;
constexpr uint32_t S_AreaTickerAbility = 0xDF8B15, S_TxtTickerAbility = 0xDF8B29, S_BurstCost = 0xDF8B3C;
constexpr uint32_t S_AttackSingleIcon = 0xDF8B46, S_AttackAllIcon = 0xDF8B59, S_AttackRandomIcon = 0xDF8B69;
constexpr uint32_t S_AreaMixCircle = 0xDF8B8A, S_AreaGuiltIcon = 0xDF8B99, S_AreaGuiltPlate = 0xDF8BA9;
constexpr uint32_t S_AbilityType2Label = 0xE0548B, S_MultiplyNum2Label = 0xE0549F;
constexpr uint32_t S_FmtS = 0xDF8821, S_FmtFrac = 0xDF8F35, S_BaseMedalInfo1 = 0xE0547B, S_Number = 0xDF8A28;
constexpr uint32_t S_NumberLabel = 0xDF8A2F, S_NameLabel = 0xDF8A3C, S_LV = 0xDF8A6E, S_NumLabel = 0xDF8A89;
constexpr uint32_t S_Num2Label = 0xE053A2, S_Cost = 0xDF8AAA, S_ATk = 0xDF8A85, S_DEF = 0xDF8AA6;

// UserMedalData: level / attack / defense / cost-down words, burst level word, master row SafeBuffer (has burst +0x4A8, burst id +0x4AC)
constexpr uint32_t M_LEVEL = 0x8, M_ATK = 0xC, M_DEF = 0x10, M_COST_MINUS = 0x28, M_MAX_LEVEL = 0x47C, M_COST = 0x484;
constexpr uint32_t M_BURST_LEVEL = 0x50, M_SB = 0x58, M_HAS_BURST = 0x4A8, M_BURST_ID = 0x4AC;
// master::Burst row
constexpr uint32_t B_NAME = 0x14, B_DESC = 0x55, B_KIND = 0x160, B_COST = 0x168, B_POWER = 0x174, B_POWERS = 0x178;
constexpr uint32_t B_POWER2 = 0x18C, B_POWERS2 = 0x190, B_TYPE = 0x1BC, B_DOUBLE = 0x1C0;
constexpr uint32_t V_setClippingEnabled = 0x31C;

struct Ctx {
    Frame &f;
    uint32_t read(uint32_t buf, uint32_t off, uint32_t len) {
        const uint32_t dst = f.alloc((len + 3) & ~3u);
        rw::call(g::SafeBuffer_read, {buf, dst, ld32(buf + 0xC) + off, len, off});
        return dst;
    }
    int32_t read32(uint32_t buf, uint32_t off) { return int32_t(ld32(read(buf, off, 4))); }
    int32_t medal32(uint32_t ref, uint32_t off) { return read32(ld32(ld32(ref) + M_SB), off); }
};
inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void shadow(uint32_t text) { call(g::SceneUtil_setShadow, {text}); }
inline void visible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline void changed(uint32_t text) { call(g::SceneUtil_setStatusChangeColor, {text}); }
void setInt(uint32_t text, int32_t v) {
    Frame f;
    setTextC(text, stringBuffer(f, {S_FmtInt, uint32_t(v)}));
}
void setEmpty(uint32_t text) {
    Frame f;
    const uint32_t s = newStr(f, S_Empty, 0);
    call(fn::Text_setString, {text, s});
    strFree(s);
}
uint32_t dbits(double d, int hi) {
    uint64_t u;
    std::memcpy(&u, &d, 8);
    return hi ? uint32_t(u >> 32) : uint32_t(u);
}
// the burst value for a burst level: a 5-entry table (level 1..5), or the base value at level 0
int32_t burstValue(Ctx &c, uint32_t burst, uint32_t single, uint32_t table, int32_t level) {
    if (level) return int32_t(ld32(c.read(burst, table, 0x14) + 4 * level - 4));
    return c.read32(burst, single);
}
// the ability type and multiplier texts of `burst` at `level`; the one-value check reads `doubleOf`'s flag (the
// first medal's burst in both uses, as the game does). Returns the multiplier.
int32_t abilityTexts(Ctx &c, uint32_t burst, uint32_t doubleOf, int32_t level, uint32_t typeText, uint32_t numText) {
    int32_t v;
    if (c.read32(burst, B_TYPE) == 2) {
        v = burstValue(c, burst, B_POWER, B_POWERS, level);
        setTextId(typeText, 101200032);
        setInt(numText, int32_t(call(g::aeabi_idiv, {uint32_t(v), 10000})));
        return v;
    }
    v = burstValue(c, burst, B_POWER, B_POWERS, level);
    setTextId(typeText, 101200031);
    Frame g2;
    if (c.read32(doubleOf, B_DOUBLE) == 1) {
        const uint32_t td = textData(g2, 101200033);
        const double d = double(float(v) / 10000.0f);
        setTextC(numText, stringBuffer(g2, {textDataStr(td), dbits(d, 0), dbits(d, 1)}));
        sharedRelease(td);
    } else {
        const int32_t v2 = burstValue(c, burst, B_POWER2, B_POWERS2, level);
        const uint32_t td1 = textData(g2, 101200033);
        const uint32_t td2 = textData(g2, 101200035);
        const uint32_t fmt = stringBuffer(g2, {S_FmtSS, textDataStr(td1), textDataStr(td2)});
        const double d1 = double(float(v) / 10000.0f), d2 = double(float(v2) / 10000.0f);
        setTextC(numText, stringBuffer(g2, {fmt, dbits(d1, 0), dbits(d1, 1), dbits(d2, 0), dbits(d2, 1)}));
        sharedRelease(td2);
        sharedRelease(td1);
    }
    return v;
}
}  // namespace

struct EvolveBurstInfo {
    static void init(uint32_t root, uint32_t medalRef, uint32_t currentRef, uint32_t mark);
    static void layer(uint32_t root, uint32_t medalRef, uint32_t currentRef, uint32_t mark);
};

// SceneEvolveMedal::initEvolutLayer: the Evolve info panel - number, name, then level, cost, attack and defense as
// "current" (Num_Label) and "evolved" (Num2_Label, colored as a change) pairs, then the burst panel
void EvolveBurstInfo::layer(uint32_t root, uint32_t medalRef, uint32_t currentRef, uint32_t mark) {
    Frame f;
    Ctx c{f};
    const uint32_t medal = ld32(medalRef), cur = ld32(currentRef);
    const uint32_t info = find(root, S_BaseMedalInfo1, 15);
    const uint32_t number = find(info, S_Number, 6);
    shadow(number);
    setTextId(number, 103600003);
    const uint32_t numberLabel = find(info, S_NumberLabel, 12);
    shadow(numberLabel);
    setInt(numberLabel, c.medal32(medalRef, 0xC));
    const uint32_t name = find(info, S_NameLabel, 9);
    shadow(name);
    {
        Frame g2;
        const uint32_t raw = c.read(ld32(medal + M_SB), 0x10, 0x41);
        setTextC(name, stringBuffer(g2, {S_FmtS, raw}));
    }
    auto fraction = [&](uint32_t text, uint32_t m, uint32_t ref) {
        Frame g2;
        const uint32_t level = ld32(m + M_LEVEL);
        setTextC(text, stringBuffer(g2, {S_FmtFrac, level, uint32_t(c.medal32(ref, M_MAX_LEVEL))}));
    };
    const uint32_t lv = find(info, S_LV, 2);
    shadow(lv);
    setTextId(lv, 100200002);
    const uint32_t lvNow = find(lv, S_NumLabel, 9);
    shadow(lvNow);
    fraction(lvNow, cur, currentRef);
    {
        const int32_t level = int32_t(ld32(cur + M_LEVEL));
        if (level < c.medal32(currentRef, M_MAX_LEVEL)) call(g::SceneUtil_setWarningColor, {lvNow});
    }
    const uint32_t lvNew = find(lv, S_Num2Label, 10);
    shadow(lvNew);
    fraction(lvNew, medal, medalRef);
    changed(lvNew);

    const uint32_t costTitle = find(info, S_Cost, 4);
    shadow(costTitle);
    setTextId(costTitle, 100200011);
    const uint32_t costNow = find(costTitle, S_NumLabel, 9);
    shadow(costNow);
    setInt(costNow, c.medal32(currentRef, M_COST) - int32_t(ld32(cur + M_COST_MINUS)));
    const uint32_t costNew = find(costTitle, S_Num2Label, 10);
    shadow(costNew);
    setInt(costNew, c.medal32(medalRef, M_COST) - int32_t(ld32(medal + M_COST_MINUS)));
    changed(costNew);

    for (auto [title, len, id, off] : {std::tuple<uint32_t, uint32_t, int32_t, uint32_t>{S_ATk, 3, 100200007, M_ATK},
                                       {S_DEF, 3, 100200008, M_DEF}}) {
        const uint32_t t = find(info, title, len);
        shadow(t);
        setTextId(t, id);
        const uint32_t now = find(t, S_NumLabel, 9);
        shadow(now);
        setInt(now, int32_t(ld32(cur + off)));
        const uint32_t next = find(t, S_Num2Label, 10);
        shadow(next);
        setInt(next, int32_t(ld32(medal + off)));
        changed(next);
    }

    const uint32_t a = f.alloc(8), b = f.alloc(8);
    st32(a, medal);
    st32(a + 4, ld32(medalRef + 4));
    if (ld32(a + 4)) call(fn::shared_add, {ld32(a + 4)});
    st32(b, cur);
    st32(b + 4, ld32(currentRef + 4));
    if (ld32(b + 4)) call(fn::shared_add, {ld32(b + 4)});
    call(0x6ADB01, {root, a, b, mark});
    if (ld32(b + 4)) call(fn::shared_release, {ld32(b + 4)});
    if (ld32(a + 4)) call(fn::shared_release, {ld32(a + 4)});
}

void EvolveBurstInfo::init(uint32_t root, uint32_t medalRef, uint32_t currentRef, uint32_t mark) {
    Frame f;
    Ctx c{f};

    const uint32_t burstTitle = find(root, S_Burst, 5);
    shadow(burstTitle);
    setTextId(burstTitle, 100400001);
    const uint32_t ability = find(root, S_Ability, 7);
    shadow(ability);
    setTextId(ability, 100400005);
    const uint32_t typeText = find(ability, S_AbilityType2Label, 19);
    shadow(typeText);
    setEmpty(typeText);
    const uint32_t numText = find(ability, S_MultiplyNum2Label, 18);
    shadow(numText);
    const uint32_t burstCost = find(root, S_BurstCost, 9);
    shadow(burstCost);
    setTextId(burstCost, 100400007);
    const uint32_t costText = find(burstCost, S_MultiplyNum2Label, 18);
    shadow(costText);
    setEmpty(costText);
    const uint32_t iconSingle = find(root, S_AttackSingleIcon, 18);
    visible(iconSingle, 0);
    const uint32_t iconAll = find(root, S_AttackAllIcon, 15);
    visible(iconAll, 0);
    const uint32_t iconRandom = find(root, S_AttackRandomIcon, 18);
    visible(iconRandom, 0);
    const uint32_t burstName = find(root, S_BurstNameLabel, 15);
    shadow(burstName);
    setEmpty(burstName);
    if (c.medal32(medalRef, M_HAS_BURST) != 1) return;

    const uint32_t burstPtr = f.alloc(8);
    call(g::Burst_create, {burstPtr, uint32_t(c.medal32(medalRef, M_BURST_ID))});
    const uint32_t burst = ld32(burstPtr);
    const int32_t value = abilityTexts(c, burst, burst, int32_t(ld32(ld32(medalRef) + M_BURST_LEVEL)), typeText,
                                       numText);
    const int32_t cost = c.read32(burst, B_COST);
    const uint32_t tickerArea = find(root, S_AreaTickerAbility, 19);
    const uint32_t tickerText = find(tickerArea, S_TxtTickerAbility, 18);
    shadow(tickerText);

    // the current medal's burst: its texts replace the ones above, the change flags compare the two
    bool burstChanged = true, valueChanged = true, costChanged = true;
    if (c.medal32(currentRef, M_HAS_BURST) == 1) {
        const uint32_t curPtr = f.alloc(8);
        call(g::Burst_create, {curPtr, uint32_t(c.medal32(currentRef, M_BURST_ID))});
        const uint32_t cur = ld32(curPtr);
        const uint32_t curType = find(ability, S_AbilityTypeLabel, 18);
        shadow(curType);
        shadow(find(ability, S_Multiply, 8));
        const uint32_t curNum = find(ability, S_MultiplyNumLabel, 17);
        shadow(curNum);
        const int32_t curCost = c.read32(cur, B_COST);
        const int32_t curValue =
            abilityTexts(c, cur, burst, int32_t(ld32(ld32(currentRef) + M_BURST_LEVEL)), curType, curNum);
        const int32_t curId = c.medal32(currentRef, M_BURST_ID);
        const int32_t id = c.medal32(medalRef, M_BURST_ID);
        costChanged = curCost != cost;
        valueChanged = curValue != value;
        if (curId == id) burstChanged = false;
        const uint32_t curCostText = find(burstCost, S_MultiplyNumLabel, 17);
        shadow(curCostText);
        setInt(curCostText, curCost);
        const uint32_t desc = c.read(burst, B_DESC, 0x101);
        vcall(tickerArea, V_setClippingEnabled, {tickerArea, 1});
        setTextC(tickerText, desc);
        const uint32_t width = ld32(vcall(tickerArea, node::getContentSize, {tickerArea}));
        const float x = bitsf(vcall(tickerText, node::getPositionX, {tickerText}));
        const float w = ldf(vcall(tickerArea, node::getContentSize, {tickerArea}));
        const uint32_t x2 = vcall(tickerText, node::getPositionX, {tickerText});
        call(g::SceneUtil_runTicker, {tickerText, width, fbits(x + w), x2});
        if (ld32(curPtr + 4)) call(fn::shared_release, {ld32(curPtr + 4)});
    }
    const int32_t kind = c.read32(burst, B_KIND);
    if (kind == 3) visible(iconRandom, 1);
    else if (kind == 2) visible(iconAll, 1);
    else if (kind == 1) visible(iconSingle, 1);
    setTextC(burstName, c.read(burst, B_NAME, 0x41));
    setInt(costText, cost);
    bool differs = c.medal32(currentRef, M_HAS_BURST) != 1;
    if (mark == 1 && !differs) {
        const int32_t curId = c.medal32(currentRef, M_BURST_ID);
        differs = curId != c.medal32(medalRef, M_BURST_ID);
    }
    if (mark == 1 && differs) {
        if (burstChanged) {
            changed(burstName);
            changed(tickerText);
        }
        if (valueChanged) {
            changed(typeText);
            changed(numText);
        }
        if (costChanged) changed(costText);
    }
    const uint32_t mixCircle = find(root, S_AreaMixCircle, 14);
    const uint32_t guiltIcon = find(root, S_AreaGuiltIcon, 15);
    if (guiltIcon) visible(guiltIcon, 0);
    const uint32_t guiltPlate = find(root, S_AreaGuiltPlate, 16);
    if (guiltPlate) visible(guiltPlate, 0);
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t copy = f.alloc(8);
    st32(copy, ld32(medalRef));
    st32(copy + 4, ld32(medalRef + 4));
    const uint32_t ctl = ld32(copy + 4);
    if (ctl) call(fn::shared_add, {ctl});
    const uint32_t mixable = ld8(c.read(ld32(api + 0x94), 0x54, 1)) != 0;
    call(g::SceneUtil_updateMixCircle, {mixCircle, guiltIcon, guiltPlate, copy, mixable, 0, 1, 0, 0, 0, 0});
    if (ctl) call(fn::shared_release, {ctl});
    if (ld32(burstPtr + 4)) call(fn::shared_release, {ld32(burstPtr + 4)});
}

RW_FUNCTION(0x6ADB00, "SceneEvolveMedal::initBurstInfo", EvolveBurstInfo::init);
RW_FUNCTION(0x6AC518, "SceneEvolveMedal::initEvolutLayer", EvolveBurstInfo::layer);
