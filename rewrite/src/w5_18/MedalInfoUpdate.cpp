// SceneUtil::updateMedalInfo: fills a MedalInfoScene layout (the info popup, the Level Up / Evolve result) from a
// UserMedalData - number (or its base medal's), name, keyblade type, level, attack / defense with their bonuses,
// cost, the burst (special attack: name, cost, multiplier text by its kind, target icons, the guilt circle) or the
// no-burst text, the large image with the partner effect, rarity stars, the skills (name, ticker, level, trigger
// chance, progress), the EXP counter and bar, and the flavor text. A previous medal (the Level Up result) marks
// raised values; the 72-byte UserMedal row of a base medal colors values above it.
#include "../scene/SceneUi.hpp"

#include <cmath>
#include <cstring>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_updateMedalRarePanel = 0x4619F5;
constexpr uint32_t SceneUtil_getMedalMaxRare = 0x462049;
constexpr uint32_t SceneUtil_setShadowText = 0x4658D5;          // sub_4658D4(text, TextData id)
constexpr uint32_t SceneUtil_setParameterUpEffect = 0x465995;
constexpr uint32_t SceneUtil_runTicker = 0x465AB9;
constexpr uint32_t SceneUtil_updateMixCircle = 0x465C85;
constexpr uint32_t SceneUtil_showSupportMedalEffect = 0x46695D, SceneUtil_hideSupportMedalEffect = 0x466B09;
constexpr uint32_t SceneUtil_setAdditionalKerningForAtlasLabel = 0x466BA9;
constexpr uint32_t SceneUtil_numberToAtlasString = 0x466C41;
constexpr uint32_t HashULLInt_find = 0x490275;                   // unordered_map<ull, int>::find(const ull&)
constexpr uint32_t TextData_get = 0x4928AD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t MedalDataManager_getMedalLargeImagePath = 0x76B1E5;
constexpr uint32_t SkillDataManager_getInstance = 0x76BA0D, SkillDataManager_getData = 0x76BA59;
constexpr uint32_t SkillDataManager_getImagePath = 0x76BE8D;
constexpr uint32_t Burst_create = 0x77A491;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t LoadingBar_setPercent = 0x83BB05;
constexpr uint32_t Size_ctor = 0x8DF027;
constexpr uint32_t Color3B_ctor = 0x9062E7;
constexpr uint32_t aeabi_idiv = 0xCD5E8D;
}  // namespace g

constexpr uint32_t S_Number = 0xDF8A28, S_NumberLabel = 0xDF8A2F, S_NameLabel = 0xDF8A3C;
constexpr uint32_t S_TxtKeyBladeType = 0xDF8A46, S_KeyBladeType = 0xDF8A58, S_Plate0 = 0xDF8A66;
constexpr uint32_t S_KeyBladeType01 = 0xDF88C6, S_KeyBladeType02 = 0xDF88EE, S_KeyBladeType03 = 0xDF8916;
constexpr uint32_t S_LV = 0xDF8A6E, S_LVNow = 0xDF8A71, S_Slash = 0xDF8A78, S_LVMax = 0xDF8A7E;
constexpr uint32_t S_ATk = 0xDF8A85, S_NumLabel = 0xDF8A89, S_Plus = 0xDF8A93, S_PlusNumLabel = 0xDF8A98;
constexpr uint32_t S_DEF = 0xDF8AA6, S_Cost = 0xDF8AAA, S_Minus = 0xDF8AAF, S_MinusNumLabel = 0xDF8AB5;
constexpr uint32_t S_Burst = 0xDF8AC4, S_BurstNameLabel = 0xDF8ACA, S_Ability = 0xDF8ADA;
constexpr uint32_t S_AbilityTypeLabel = 0xDF8AE2, S_Multiply = 0xDF8AF5, S_Times = 0xDF8AFE;   // "\xC3\x97" (2)
constexpr uint32_t S_MultiplyNumLabel = 0xDF8B01, S_PlusSign = 0xDF8B13, S_AreaTickerAbility = 0xDF8B15;
constexpr uint32_t S_TxtTickerAbility = 0xDF8B29, S_BurstCost = 0xDF8B3C, S_AttackSingleIcon = 0xDF8B46;
constexpr uint32_t S_AttackAllIcon = 0xDF8B59, S_AttackRandomIcon = 0xDF8B69, S_TxtNoAbility = 0xDF8B7C;
constexpr uint32_t S_AreaMixCircle = 0xDF8B8A, S_AreaGuiltIcon = 0xDF8B99, S_AreaGuiltPlate = 0xDF8BA9;
constexpr uint32_t S_FmtSS = 0xDF8BBA, S_DetailPlate1 = 0xDF8BBF, S_DetailPlate2 = 0xDF8BD3;
constexpr uint32_t S_DetailPlate3 = 0xDF8BE7, S_DetailTicker1 = 0xDF8BFB, S_Skill = 0xDF8C10;
constexpr uint32_t S_DummyMedal = 0xDF8C16, S_StarArea = 0xDF8C22, S_TxtSkillNameLabel = 0xDF8C2C;
constexpr uint32_t S_IconSkill = 0xDF8C40, S_ProgressBarSkill = 0xDF8C4B, S_LVSkill = 0xDF8C5D;
constexpr uint32_t S_SlashSign = 0xDF8C66, S_Trigger = 0xDF8C68, S_AreaTickerSkill = 0xDF8C70;
constexpr uint32_t S_TxtTickerSkill = 0xDF8C82, S_TxtNoSkill = 0xDF8C93, S_FmtPercent = 0xDF8C9F;
constexpr uint32_t S_DetailPlate4 = 0xDF8CA4, S_DetailTicker2 = 0xDF8CB8, S_AtlasLabelEXP = 0xDF8CCD;
constexpr uint32_t S_ProgressBarEXP = 0xDF8CDC, S_TxtFlavor = 0xDF8CEC, S_FmtInt = 0xDF86DE;

// UserMedalData words (the medal's current values) and its master row (SafeBuffer at +0x58)
constexpr uint32_t M_LEVEL = 0x8, M_ATK = 0xC, M_DEF = 0x10, M_EXP_NEXT = 0x18, M_EXP_BASE = 0x1C, M_EXP = 0x20;
constexpr uint32_t M_COST_MINUS = 0x28, M_ATK_PLUS = 0x2C, M_DEF_PLUS = 0x30, M_SKILLS = 0x38, M_SKILL_COUNT = 0x48;
constexpr uint32_t M_BURST_LEVEL = 0x50, M_SB = 0x58;
// master::Burst row
constexpr uint32_t B_NAME = 0x14, B_DESC = 0x55, B_KIND = 0x160, B_COST = 0x168, B_POWER = 0x174, B_POWERS = 0x178;
constexpr uint32_t B_POWER2 = 0x18C, B_POWERS2 = 0x190, B_TYPE = 0x1BC, B_DOUBLE = 0x1C0;
// UserSkillData (a SafeBuffer: level +8, exp +0x10, next +0x14, base +0x18) and its master row at +0x20
constexpr uint32_t SK_MASTER = 0x20;
constexpr uint32_t V_setTouchEnabledW = 0x29C, V_setClippingEnabled = 0x31C, V_setContentSize = 0xA8;

struct Ctx {
    Frame &f;
    uint32_t medal;
    uint32_t sb(uint32_t m) const { return ld32(m + M_SB); }
    // SafeBuffer::read(sb, dst, data + off, len, off) into a fresh frame buffer
    uint32_t read(uint32_t buf, uint32_t off, uint32_t len) {
        const uint32_t dst = f.alloc((len + 3) & ~3u);
        rw::call(g::SafeBuffer_read, {buf, dst, ld32(buf + 0xC) + off, len, off});
        return dst;
    }
    int32_t read32(uint32_t buf, uint32_t off) { return int32_t(ld32(read(buf, off, 4))); }
    int32_t mread(uint32_t off) { return read32(sb(medal), off); }
    int32_t val(uint32_t off) const { return int32_t(ld32(medal + off)); }
};
inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void shadow(uint32_t text) { call(g::SceneUtil_setShadow, {text}); }
inline void shadowText(uint32_t text, int32_t id) { call(g::SceneUtil_setShadowText, {text, uint32_t(id)}); }
inline void effect(uint32_t text) { call(g::SceneUtil_setParameterUpEffect, {text}); }
inline void visible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
void color(uint32_t text, uint32_t r, uint32_t g_, uint32_t b) {
    Frame f;
    const uint32_t c = f.alloc(4);
    call(g::Color3B_ctor, {c, r, g_, b});
    vcall(text, node::setColor, {text, c});
}
void setInt(uint32_t text, const char *fmt, uint32_t fmtAddr, int32_t v) {
    (void)fmt;
    Frame f;
    const uint32_t buf = stringBuffer(f, {fmtAddr, uint32_t(v)});
    setTextC(text, buf);
}
// a text's ticker: clip the area, the text, then run it across the area's width
void ticker(uint32_t area, uint32_t text) {
    const uint32_t width = ld32(vcall(area, node::getContentSize, {area}));
    const float x = bitsf(vcall(text, node::getPositionX, {text}));
    const float w = ldf(vcall(area, node::getContentSize, {area}));
    const uint32_t x2 = vcall(text, node::getPositionX, {text});
    call(g::SceneUtil_runTicker, {text, width, fbits(x + w), x2});
}
// the burst value for the medal's burst level: a 5-entry table (level 1..5), or the base value at level 0
int32_t burstValue(Ctx &c, uint32_t burst, uint32_t single, uint32_t table, int32_t level) {
    if (level) return int32_t(ld32(c.read(burst, table, 0x14) + 4 * level - 4));
    return c.read32(burst, single);
}
uint32_t dbits(double d, int hi) {
    uint64_t u;
    std::memcpy(&u, &d, 8);
    return hi ? uint32_t(u >> 32) : uint32_t(u);
}
}  // namespace

struct MedalInfoView {
    static void update(uint32_t root, uint32_t medalRef, uint32_t mixable, uint32_t skills, uint32_t prevRef,
                       uint32_t skillUp);
};

void MedalInfoView::update(uint32_t root, uint32_t medalRef, uint32_t mixable, uint32_t skills, uint32_t prevRef,
                           uint32_t skillUp) {
    Frame f;
    const uint32_t medal = ld32(medalRef);
    Ctx c{f, medal};
    const uint32_t prev = ld32(prevRef);

    // ---------------------------------------------------------------- number (a base medal's when it has one)
    const uint32_t number = find(root, S_Number, 6);
    shadowText(number, 103600003);
    const int32_t baseId = c.mread(0x4B8);
    uint32_t base = 0, baseCtl = 0;
    if (baseId) {
        const uint32_t rows = ld32(call(g::APIManager_getInstance, {}) + 0xD4);
        const uint32_t key = f.alloc(8);
        st32(key, ld32(medal));
        st32(key + 4, ld32(medal + 4));
        const uint32_t node = call(g::HashULLInt_find, {rows + 0x24, key});
        if (node) {
            const int32_t index = int32_t(ld32(node + 0x10));
            if (index >= 0) {
                const uint32_t row = c.read(rows, uint32_t(72 * index), 0x48);
                const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
                const uint32_t out = f.alloc(8);
                call(g::MedalDataManager_getMedalData,
                     {out, mdm, 0, 0, uint32_t(baseId), 0, ld32(row), ld32(row + 4), ld32(row + 8), ld32(row + 12),
                      ld32(row + 16), ld32(row + 20), ld32(row + 24), ld32(row + 28), ld32(row + 32), ld32(row + 36),
                      ld32(row + 40), ld32(row + 44), ld32(row + 48), ld32(row + 52), ld32(row + 56), ld32(row + 60),
                      ld32(row + 64), ld32(row + 68)});
                base = ld32(out);
                baseCtl = ld32(out + 4);
                st32(out, 0);
                st32(out + 4, 0);
            }
        }
    }
    {
        const uint32_t label = find(number, S_NumberLabel, 12);
        shadow(label);
        const int32_t n = base ? c.read32(c.sb(base), 0xC) : c.mread(0xC);
        setInt(label, "%d", S_FmtInt, n);
    }
    {
        const uint32_t name = find(root, S_NameLabel, 9);
        shadow(name);
        setTextC(name, c.read(c.sb(medal), 0x10, 0x41));
    }
    // ---------------------------------------------------------------- keyblade type
    {
        const uint32_t typeText = find(root, S_TxtKeyBladeType, 17);
        shadow(typeText);
        const uint32_t typeImage = find(root, S_KeyBladeType, 13);
        const uint32_t plate = find(root, S_Plate0, 7);
        const int32_t type = c.mread(0x458);
        const uint32_t pngs[3] = {S_KeyBladeType01, S_KeyBladeType02, S_KeyBladeType03};
        if (type >= 1 && type <= 3) {
            Frame g2;
            const uint32_t s = newStr(g2, pngs[type - 1], 39);
            call(g::ImageView_loadTexture, {typeImage, s, 0});
            strFree(s);
            shadowText(typeText, 100250000 + type);
        } else if (type == 4) {
            visible(plate, 0);
            visible(typeImage, 0);
            visible(typeText, 0);
        }
    }
    // ---------------------------------------------------------------- level
    {
        const uint32_t lv = find(root, S_LV, 2);
        shadowText(lv, 100200002);
        const uint32_t now = find(lv, S_LVNow, 6);
        shadow(now);
        setInt(now, "%d", S_FmtInt, c.val(M_LEVEL));
        if (prev && int32_t(ld32(prev + M_LEVEL)) < c.val(M_LEVEL)) effect(now);
        shadow(find(lv, S_Slash, 5));
        const uint32_t max = find(lv, S_LVMax, 6);
        shadow(max);
        setInt(max, "%d", S_FmtInt, c.mread(0x47C));
    }
    // ---------------------------------------------------------------- attack / defense with their bonuses
    auto stat = [&](uint32_t nodeName, uint32_t nodeLen, int32_t titleId, uint32_t valueOff, uint32_t plusOff,
                    uint32_t plusMaxOff) {
        const uint32_t n = find(root, nodeName, nodeLen);
        shadowText(n, titleId);
        const uint32_t num = find(n, S_NumLabel, 9);
        shadow(num);
        const int32_t shown = c.val(valueOff) - c.val(plusOff);
        setInt(num, "%d", S_FmtInt, shown);
        if (prev && int32_t(ld32(prev + valueOff)) - int32_t(ld32(prev + plusOff)) < shown) effect(num);
        if (base && int32_t(ld32(base + valueOff)) < c.val(valueOff)) color(num, 0xFF, 0xEA, 0);
        const uint32_t plus = find(n, S_Plus, 4);
        shadow(plus);
        const uint32_t plusNum = find(n, S_PlusNumLabel, 13);
        shadow(plusNum);
        setInt(plusNum, "%d", S_FmtInt, c.val(plusOff));
        if (c.val(plusOff) <= 0) {
            visible(plus, 0);
            visible(plusNum, 0);
        } else {
            const bool full = c.mread(plusMaxOff) <= c.val(plusOff);
            color(plus, 0xFF, full ? 0x84 : 0xEA, 0);
            color(plusNum, 0xFF, full ? 0x84 : 0xEA, 0);
            if (prev && int32_t(ld32(prev + plusOff)) < c.val(plusOff)) effect(plusNum);
        }
    };
    stat(S_ATk, 3, 100200007, M_ATK, M_ATK_PLUS, 0x498);
    stat(S_DEF, 3, 100200008, M_DEF, M_DEF_PLUS, 0x4A4);
    // ---------------------------------------------------------------- cost
    {
        const uint32_t cost = find(root, S_Cost, 4);
        shadowText(cost, 100200011);
        const int32_t value = c.mread(0x484);
        const uint32_t num = find(cost, S_NumLabel, 9);
        shadow(num);
        setInt(num, "%d", S_FmtInt, value);
        if (prev) {
            const int32_t before = c.read32(c.sb(prev), 0x484);
            if (before < c.mread(0x484)) effect(num);
        }
        if (base && value < c.read32(c.sb(base), 0x484)) color(num, 0xFF, 0xEA, 0);
        const uint32_t minus = find(cost, S_Minus, 5);
        shadow(minus);
        const uint32_t minusNum = find(cost, S_MinusNumLabel, 14);
        shadow(minusNum);
        setInt(minusNum, "%d", S_FmtInt, c.val(M_COST_MINUS));
        const int32_t m = c.val(M_COST_MINUS);
        if (m <= 0) {
            visible(minus, 0);
            visible(minusNum, 0);
        } else {
            const bool full = value - m <= c.mread(0x488);
            color(minus, 0xFF, full ? 0x84 : 0xEA, 0);
            color(minusNum, 0xFF, full ? 0x84 : 0xEA, 0);
            if (prev && int32_t(ld32(prev + M_COST_MINUS)) < c.val(M_COST_MINUS)) effect(minusNum);
        }
    }
    // ---------------------------------------------------------------- burst (special attack)
    const uint32_t burstTitle = find(root, S_Burst, 5);
    shadowText(burstTitle, 100400001);
    const uint32_t burstName = find(root, S_BurstNameLabel, 15);
    shadow(burstName);
    const uint32_t ability = find(root, S_Ability, 7);
    shadowText(ability, 100400005);
    const uint32_t abilityType = find(ability, S_AbilityTypeLabel, 18);
    const uint32_t multiply = find(ability, S_Multiply, 8);
    shadow(multiply);
    {
        Frame g2;
        const uint32_t s = newStr(g2, S_Times, 2);
        call(fn::Text_setString, {multiply, s});
        strFree(s);
    }
    const uint32_t multiplyNum = find(ability, S_MultiplyNumLabel, 17);
    shadow(multiplyNum);
    const uint32_t plus = find(ability, S_Plus, 4);
    shadow(plus);
    {
        Frame g2;
        const uint32_t s = newStr(g2, S_PlusSign, 1);
        call(fn::Text_setString, {plus, s});
        strFree(s);
    }
    const uint32_t plusNum = find(ability, S_PlusNumLabel, 13);
    shadow(plusNum);
    const uint32_t abilityTicker = find(root, S_AreaTickerAbility, 19);
    const uint32_t abilityTickerText = find(abilityTicker, S_TxtTickerAbility, 18);
    shadow(abilityTickerText);
    const uint32_t burstCost = find(root, S_BurstCost, 9);
    shadowText(burstCost, 100400007);
    const uint32_t burstCostNum = find(burstCost, S_MultiplyNumLabel, 17);
    shadow(burstCostNum);
    const uint32_t iconSingle = find(root, S_AttackSingleIcon, 18);
    visible(iconSingle, 0);
    const uint32_t iconAll = find(root, S_AttackAllIcon, 15);
    visible(iconAll, 0);
    const uint32_t iconRandom = find(root, S_AttackRandomIcon, 18);
    visible(iconRandom, 0);
    const uint32_t noAbility = find(root, S_TxtNoAbility, 13);
    visible(noAbility, 0);
    const uint32_t mixCircle = find(root, S_AreaMixCircle, 14);
    visible(mixCircle, 0);
    visible(multiply, 0);
    visible(plus, 0);
    visible(plusNum, 0);
    const uint32_t guiltIcon = find(root, S_AreaGuiltIcon, 15);
    if (guiltIcon) visible(guiltIcon, 0);
    const uint32_t guiltPlate = find(root, S_AreaGuiltPlate, 16);
    if (guiltPlate) visible(guiltPlate, 0);
    if (c.mread(0x4A8) == 1) {
        const int32_t burstId = c.mread(0x4AC);
        const uint32_t burstPtr = f.alloc(8);
        call(g::Burst_create, {burstPtr, uint32_t(burstId)});
        if (prev) {
            const int32_t before = c.read32(c.sb(prev), 0x4AC);
            if (before != c.mread(0x4AC)) effect(burstName);
        }
        const uint32_t burst = ld32(burstPtr);
        const uint32_t desc = c.read(burst, B_DESC, 0x101);
        setTextC(burstName, c.read(burst, B_NAME, 0x41));
        setInt(burstCostNum, "%d", S_FmtInt, c.read32(burst, B_COST));
        vcall(abilityTicker, V_setClippingEnabled, {abilityTicker, 1});
        setTextC(abilityTickerText, desc);
        ticker(abilityTicker, abilityTickerText);
        const int32_t level = c.val(M_BURST_LEVEL);
        if (c.read32(burst, B_TYPE) == 2) {
            const int32_t v = burstValue(c, burst, B_POWER, B_POWERS, level);
            shadowText(abilityType, 101200032);
            setInt(multiplyNum, "%d", S_FmtInt, int32_t(call(g::aeabi_idiv, {uint32_t(v), 10000})));
        } else {
            const int32_t v = burstValue(c, burst, B_POWER, B_POWERS, level);
            shadowText(abilityType, 101200031);
            if (c.read32(burst, B_DOUBLE) == 1) {
                Frame g2;
                const uint32_t td = textData(g2, 101200033);
                const double d = double(float(v) / 10000.0f);
                const uint32_t buf = stringBuffer(g2, {textDataStr(td), dbits(d, 0), dbits(d, 1)});
                setTextC(multiplyNum, buf);
                sharedRelease(td);
            } else {
                const int32_t v2 = burstValue(c, burst, B_POWER2, B_POWERS2, level);
                Frame g2;
                const uint32_t td1 = textData(g2, 101200033);
                const uint32_t td2 = textData(g2, 101200035);
                const uint32_t fmt = stringBuffer(g2, {S_FmtSS, textDataStr(td1), textDataStr(td2)});
                const double d1 = double(float(v) / 10000.0f), d2 = double(float(v2) / 10000.0f);
                const uint32_t buf = stringBuffer(g2, {fmt, dbits(d1, 0), dbits(d1, 1), dbits(d2, 0), dbits(d2, 1)});
                setTextC(multiplyNum, buf);
                sharedRelease(td2);
                sharedRelease(td1);
            }
        }
        if (prev && int32_t(ld32(prev + M_BURST_LEVEL)) < c.val(M_BURST_LEVEL)) effect(multiplyNum);
        const int32_t kind = c.read32(burst, B_KIND);
        if (kind == 3) visible(iconRandom, 1);
        else if (kind == 2) visible(iconAll, 1);
        else if (kind == 1) visible(iconSingle, 1);
        visible(burstName, 1);
        visible(abilityType, 1);
        visible(multiplyNum, 1);
        visible(abilityTickerText, 1);
        visible(burstCostNum, 1);
        visible(mixCircle, 1);
        const uint32_t copy = f.alloc(8);
        st32(copy, medal);
        st32(copy + 4, ld32(medalRef + 4));
        const uint32_t ctl = ld32(copy + 4);
        if (ctl) call(fn::shared_add, {ctl});
        call(g::SceneUtil_updateMixCircle, {mixCircle, guiltIcon, guiltPlate, copy, mixable, 0, 1, 0, 0, 0, 0});
        if (ctl) call(fn::shared_release, {ctl});
        if (ld32(burstPtr + 4)) call(fn::shared_release, {ld32(burstPtr + 4)});
    } else {
        shadowText(burstTitle, 101200028);
        visible(noAbility, 1);
        shadow(noAbility);
        setTextC(noAbility, c.read(c.sb(medal), 0x252, 0x201));
        for (auto [name, len] : {std::pair<uint32_t, uint32_t>{S_DetailPlate1, 19}, {S_DetailPlate2, 19},
                                 {S_DetailPlate3, 19}, {S_DetailTicker1, 20}}) {
            Frame g2;
            const uint32_t s = newStr(g2, name, len);
            const uint32_t n = SceneUtil::searchNodeByName(root, s);
            visible(n, 0);
            strFree(s);
        }
        for (uint32_t n : {ability, burstName, abilityType, multiply, multiplyNum, plus, plusNum, abilityTicker,
                           burstCost, burstCostNum})
            visible(n, 0);
    }
    // ---------------------------------------------------------------- image, partner, rarity
    shadow(find(root, S_Skill, 5));
    const uint32_t image = find(root, S_DummyMedal, 11);
    {
        const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
        const uint32_t copy = f.alloc(8);
        st32(copy, medal);
        st32(copy + 4, ld32(medalRef + 4));
        if (ld32(copy + 4)) call(fn::shared_add, {ld32(copy + 4)});
        const uint32_t path = call(g::MedalDataManager_getMedalLargeImagePath, {mdm, copy});
        {
            Frame g2;
            const uint32_t s = newStr(g2, path);
            call(g::ImageView_loadTexture, {image, s, 0});
            strFree(s);
        }
        if (ld32(copy + 4)) call(fn::shared_release, {ld32(copy + 4)});
    }
    const uint32_t api = call(g::APIManager_getInstance, {});
    const uint32_t partnerCtl = ld32(api + 0x118);
    const uint32_t partnerSb = ld32(api + 0x114);
    if (partnerCtl) call(fn::shared_add, {partnerCtl});
    {
        const uint32_t lo = ld32(medal), hi = ld32(medal + 4);
        const uint32_t partner = c.read(partnerSb, 0x58, 8);
        if (((ld32(partner) ^ lo) | (ld32(partner + 4) ^ hi)) == 0)
            call(g::SceneUtil_showSupportMedalEffect, {image, 0});
        else
            call(g::SceneUtil_hideSupportMedalEffect, {image, 0});
    }
    uint32_t maxRare;
    {
        const uint32_t copy = f.alloc(8);
        st32(copy, medal);
        st32(copy + 4, ld32(medalRef + 4));
        const uint32_t ctl = ld32(copy + 4);
        if (ctl) call(fn::shared_add, {ctl});
        maxRare = call(g::SceneUtil_getMedalMaxRare, {copy});
        if (ctl) call(fn::shared_release, {ctl});
    }
    {
        Frame g2;
        const uint32_t s = newStr(g2, S_StarArea, 9);
        const uint32_t stars = SceneUtil::searchNodeByName(root, s);
        const int32_t rare = c.mread(0x470);
        call(g::SceneUtil_updateMedalRarePanel, {stars, uint32_t(rare), 1, fbits(1.0f), 0, maxRare, 0});
        strFree(s);
    }
    // ---------------------------------------------------------------- skills
    shadowText(find(root, S_Skill, 5), 100450001);
    const uint32_t skillName = find(root, S_TxtSkillNameLabel, 19);
    shadow(skillName);
    if (skillUp == 1) effect(skillName);
    const uint32_t skillIcon = find(root, S_IconSkill, 10);
    const uint32_t skillBar = find(root, S_ProgressBarSkill, 17);
    const uint32_t skillLv = find(root, S_LVSkill, 8);
    shadowText(skillLv, 100200002);
    const uint32_t skillLvNow = find(skillLv, S_LVNow, 6);
    shadow(skillLvNow);
    {
        const uint32_t slash = find(skillLv, S_Slash, 5);
        shadow(slash);
        Frame g2;
        const uint32_t s = newStr(g2, S_SlashSign, 1);
        call(fn::Text_setString, {slash, s});
        strFree(s);
    }
    const uint32_t skillLvMax = find(skillLv, S_LVMax, 6);
    shadow(skillLvMax);
    const uint32_t trigger = find(root, S_Trigger, 7);
    shadowText(trigger, 100450002);
    const uint32_t triggerNum = find(trigger, S_MultiplyNumLabel, 17);
    shadow(triggerNum);
    const uint32_t skillTicker = find(root, S_AreaTickerSkill, 17);
    const uint32_t skillTickerText = find(root, S_TxtTickerSkill, 16);
    shadow(skillTickerText);
    const uint32_t noSkill = find(root, S_TxtNoSkill, 11);
    shadowText(noSkill, 101200020);
    visible(noSkill, 0);
    int32_t count = c.val(M_SKILL_COUNT);
    for (int32_t i = 0; i < count; i = i + 1, count = c.val(M_SKILL_COUNT)) {
        const uint32_t lo = ld32(medal + M_SKILLS + 8 * i), hi = ld32(medal + M_SKILLS + 8 * i + 4);
        if (!(lo | hi)) continue;
        const uint32_t sdOut = f.alloc(8);
        uint32_t data = 0, ctl = 0;
        bool fallback = false;
        if (skills) {
            uint32_t e = ld32(skills), end = ld32(skills + 4);
            for (; e != end; e += 0x28)
                if (ld32(e) == lo && ld32(e + 4) == hi) break;
            if (e == end) {
                fallback = true;
            } else {
                const uint32_t sdm = call(g::SkillDataManager_getInstance, {});
                call(g::SkillDataManager_getData,
                     {sdOut, sdm, 0, 0, lo, hi, ld32(e + 8), ld32(e + 0xC), ld32(e + 0x10), ld32(e + 0x14),
                      ld32(e + 0x18), ld32(e + 0x1C), ld32(e + 0x20), ld32(e + 0x24), 0});
                data = ld32(sdOut);
                ctl = ld32(sdOut + 4);
                st32(sdOut, 0);
                st32(sdOut + 4, 0);
                if (!data) fallback = true;
            }
            if (fallback) {
                const uint32_t sdm = call(g::SkillDataManager_getInstance, {});
                const uint32_t out2 = f.alloc(8);
                call(g::SkillDataManager_getData, {out2, sdm, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
                data = ld32(out2);
                const uint32_t ctl2 = ld32(out2 + 4);
                st32(out2, 0);
                st32(out2 + 4, 0);
                if (ctl) call(fn::shared_release, {ctl});
                ctl = ctl2;
            }
        } else {
            const uint32_t sdm = call(g::SkillDataManager_getInstance, {});
            call(g::SkillDataManager_getData, {sdOut, sdm, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
            data = ld32(sdOut);
            ctl = ld32(sdOut + 4);
            st32(sdOut, 0);
            st32(sdOut + 4, 0);
        }
        const uint32_t master = ld32(data + SK_MASTER);
        setTextC(skillName, c.read(master, 0xC, 0x41));
        setTextC(skillTickerText, c.read(master, 0x4D, 0x81));
        {
            const uint32_t sdm = call(g::SkillDataManager_getInstance, {});
            const uint32_t copy = f.alloc(8);
            st32(copy, data);
            st32(copy + 4, ctl);
            if (ctl) call(fn::shared_add, {ctl});
            const uint32_t path = call(g::SkillDataManager_getImagePath, {sdm, copy});
            {
                Frame g2;
                const uint32_t s = newStr(g2, path);
                call(g::ImageView_loadTexture, {skillIcon, s, 0});
                strFree(s);
            }
            if (ld32(copy + 4)) call(fn::shared_release, {ld32(copy + 4)});
        }
        setInt(skillLvNow, "%d", S_FmtInt, c.read32(data, 8));
        setInt(skillLvMax, "%d", S_FmtInt, c.read32(master, 0xD4));
        const int32_t level = c.read32(data, 8);
        if (level == c.read32(master, 0xD4)) {
            call(g::LoadingBar_setPercent, {skillBar, fbits(100.0f)});
        } else {
            const int32_t exp = c.read32(data, 0x10);
            const float num = float(exp - c.read32(data, 0x18));
            const int32_t next = c.read32(data, 0x14);
            const float den = float(next - c.read32(data, 0x18));
            call(g::LoadingBar_setPercent, {skillBar, fbits((num / den) * 100.0f)});
        }
        const int32_t chance = c.read32(master, 0xE8);
        const int32_t perLevel = c.read32(master, 0xEC);
        setInt(triggerNum, "%d%%", S_FmtPercent,
               int32_t(call(g::aeabi_idiv, {uint32_t(perLevel * (level - 1) + chance), 100})));
        vcall(skillTicker, V_setClippingEnabled, {skillTicker, 1});
        ticker(skillTicker, skillTickerText);
        if (ctl) call(fn::shared_release, {ctl});
    }
    if (count == 0) {
        visible(noSkill, 1);
        for (auto [name, len] : {std::pair<uint32_t, uint32_t>{S_DetailPlate3, 19}, {S_DetailPlate4, 19},
                                 {S_DetailTicker2, 20}}) {
            Frame g2;
            const uint32_t s = newStr(g2, name, len);
            const uint32_t n = SceneUtil::searchNodeByName(root, s);
            visible(n, 0);
            strFree(s);
        }
        for (uint32_t n : {skillIcon, skillBar, skillName, skillLv, trigger, skillTicker}) visible(n, 0);
    }
    // ---------------------------------------------------------------- EXP
    int32_t expLeft = 0;
    {
        const int32_t level = c.val(M_LEVEL);
        if (level != c.mread(0x47C)) expLeft = c.val(M_EXP_NEXT) - c.val(M_EXP);
    }
    {
        const uint32_t atlas = find(root, S_AtlasLabelEXP, 14);
        call(g::SceneUtil_setAdditionalKerningForAtlasLabel, {atlas, uint32_t(-5)});
        Frame g2;
        const uint32_t s = g2.alloc(12);
        call(g::SceneUtil_numberToAtlasString, {s, uint32_t(expLeft)});
        call(g::TextAtlas_setString, {atlas, s});
        strFree(s);
    }
    {
        const uint32_t bar = find(root, S_ProgressBarEXP, 15);
        float ratio = 1.0f;
        const int32_t level = c.val(M_LEVEL);
        if (level != c.mread(0x47C)) {
            const int32_t b = c.val(M_EXP_BASE);
            const float frac = float(c.val(M_EXP) - b) / float(c.val(M_EXP_NEXT) - b);
            ratio = frac < 0.0f ? 0.0f : frac;
            if (!(ratio <= 1.0f || std::isnan(ratio))) ratio = 1.0f;
        }
        const uint32_t size = ld32(vcall(bar, node::getContentSize, {bar}) + 4);
        Frame g2;
        const uint32_t sz = g2.alloc(8);
        call(g::Size_ctor, {sz, fbits(ratio * 100.0f), size});
        vcall(bar, V_setContentSize, {bar, sz});
    }
    {
        const uint32_t flavor = find(root, S_TxtFlavor, 10);
        if (flavor) {
            setTextC(flavor, c.read(c.sb(medal), 0x51, 0x201));
            shadow(flavor);
        }
    }
    if (partnerCtl) call(fn::shared_release, {partnerCtl});
    if (baseCtl) call(fn::shared_release, {baseCtl});
}

RW_FUNCTION(0x462194, "SceneUtil::updateMedalInfo", MedalInfoView::update);
