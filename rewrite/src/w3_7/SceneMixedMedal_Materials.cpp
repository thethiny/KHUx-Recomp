// SceneMixedMedal: the materials - adding / removing material medals, what they give the base medal (experience,
// cost, burst / attack / defense / cost upgrades, skill overwrite) and the automatic selection.
#include "SceneMixedMedal.hpp"

#include <cmath>

#include "../core/SafeBuffer.hpp"
#include "../scene_social/SocialCommon.hpp"

using namespace social;
using Self = SceneMixedMedal;
namespace F = smm::fn;

namespace {

// game functions not in smm::fn
constexpr Addr DeckMedalView_create = 0x52BA71;            // DeckMedalView::create(bool small)
constexpr Addr DeckMedalView_setData = 0x52BEE5;           // (view, shared_ptr by value, int, int, int, int, int, ...)
constexpr Addr DeckMedalView_changeShowType = 0x52C1ED;
constexpr Addr DeckMedalViewManager_addDeckMedalView = 0x527559;
constexpr Addr vector_deckMedalView_push_slow = 0x5306D5;  // vector<DeckMedalView*>::__push_back_slow_path(const&)
constexpr Addr BattleUtil_calcNeedExp = 0x496C99;          // (const shared_ptr<master::Medal>&, int level)
constexpr Addr MiniMedalScrollView_canSelectMaterialMedal = 0x4FD559;
constexpr Addr sortCandidates = 0x69FE01;                  // std::sort of the candidates (lambda: by guilt group)
constexpr Addr memmove_ = 0x40EF50;
constexpr Addr ISceneAnimation_runOutAnimation = 0x6F24B5;  // (interface at +0x2A4, std::function<void()>*)
constexpr Addr ANCHOR_MIDDLE = 0xFEF5C0;          // cocos2d::Vec2::ANCHOR_MIDDLE
constexpr Addr VT_infoButton_back = 0xF654F4;              // onTapMedalInfoButton: come back to this screen (heap, 0x18)
constexpr Addr S_empty = 0xDF8A1A;                         // ""
constexpr Addr S_Txt_SkillName_Label = 0xDF8C2C;
constexpr Addr S_Icon_Skill = 0xDF8C40;
constexpr Addr S_Txt_EXP = 0xE05018;
constexpr Addr S_Txt_EXP_Label = 0xE05020;
constexpr Addr S_Base_Money1 = 0xE0502E;
constexpr Addr S_Base_Money2 = 0xE0503A;
constexpr Addr S_Txt_Money = 0xDFE7A3;
constexpr Addr S_Txt_Money_Label = 0xDFE7AD;
constexpr Addr S_Button_Mix = 0xDF87D4;
constexpr Addr S_Txt_Mix = 0xDF87DF;
constexpr Addr S_Area_MixCircle = 0xDF8B8A;
constexpr Addr S_Area_Guilt_Icon = 0xDF8B99;
constexpr Addr S_Area_Guilt_Plate = 0xDF8BA9;
constexpr Addr S_Gauge_EXP = 0xE04F51;
constexpr Addr S_ProgressBar_EXP = 0xDF8CDC;
constexpr Addr S_LV = 0xDF8A6E;
constexpr Addr S_LV_Now = 0xDF8A71;
constexpr Addr S_fmt_d = 0xDF86DE;
constexpr Addr S_panelOff = 0xE04FF6;                      // "img/ui/Medal_Syn_Select_Panel.png"
constexpr Addr S_panelOn = 0xE04FD2;                       // "img/ui/Medal_Syn_Select_Panel02.png"
constexpr Addr VT_expGauge = 0xF65CDC;                     // updateExpGauge: calcMedalExp callback (heap, 0x20)

// APIManager members (shared_ptr<SafeBuffer>)
constexpr uint32_t API_userMunny = 0x8C;
constexpr uint32_t API_userData = 0x94;
constexpr uint32_t API_userMedals = 0xD4;
constexpr uint32_t USERDATA_guiltOn = 0x54;                // byte: guilt factors are in play

// UserMedalData fields
constexpr uint32_t UMD_level = 0x08, UMD_nextLevelExp = 0x18, UMD_levelExp = 0x1C, UMD_exp = 0x20;
constexpr uint32_t UMD_upperCost = 0x28, UMD_attackUpper = 0x2C, UMD_defenseUpper = 0x30;
constexpr uint32_t UMD_skillIds = 0x38, UMD_skillCount = 0x48, UMD_burstUpper = 0x50, UMD_guiltFactor = 0x54;
constexpr uint32_t UMD_medal = 0x58;

// master::Medal fields
constexpr uint32_t MEDAL_id = 0x000, MEDAL_type = 0x454, MEDAL_mixGroup = 0x458, MEDAL_mixGroupCount = 0x460;
constexpr uint32_t MEDAL_mixGroups = 0x464, MEDAL_rare = 0x470, MEDAL_guiltGroup = 0x478, MEDAL_maxLevel = 0x47C;
constexpr uint32_t MEDAL_costMax = 0x484, MEDAL_costUpper = 0x488, MEDAL_attackMax = 0x498, MEDAL_defenseMax = 0x4A4;
constexpr uint32_t MEDAL_burstUpIds = 0x4B0, MEDAL_evolveFrom = 0x4B8, MEDAL_baseExp = 0x4C0;
constexpr uint32_t MEDAL_attackUp = 0x4CC, MEDAL_defenseUp = 0x4D0, MEDAL_costUp = 0x4D4;
// master::Skill / master::Burst fields
constexpr uint32_t SKILL_name = 0x0C, SKILL_maxLevel = 0xD4, SKILL_group = 0xE0;
constexpr uint32_t BURST_maxLevel = 0x158;

constexpr uint32_t PUSHED = 20;
constexpr uint32_t FRAME_updateMaterial = 0x69C, HOLE_updateMaterial = 0x648 + 0x2C;
constexpr uint32_t FRAME_addMaterialMedal = 0xE4, HOLE_addMaterialMedal = 0x90 + 0x2C;
constexpr uint32_t FRAME_settingOmakase = 0x134, HOLE_settingOmakase = 0xE0 + 0x2C;
constexpr uint32_t FRAME_calcMixedExp = 0xB4;

constexpr float RATE_SCALE = 10000.0f;

void setShadow(Addr text) { call(F::SceneUtil_setShadow, {text}); }
void release(Addr ctrl) {
    if (ctrl) call(F::shared_release, {ctrl});
}
void addRef(Addr ctrl) {
    if (ctrl) call(F::shared_add, {ctrl});
}
int32_t sb32(Addr sb, uint32_t index) { return rw::host_ptr<SafeBuffer>(sb)->get32(index); }
uint8_t sb8(Addr sb, uint32_t index) {
    const SafeBuffer *s = rw::host_ptr<SafeBuffer>(sb);
    return uint8_t(rw::host_ptr<uint8_t>(s->data.addr)[index] ^ SafeBuffer::mask(s->key, index));
}
// the master row (SafeBuffer) of the UserMedalData held by a shared_ptr at `sp`
Addr rowOf(Addr sp) { return ld32(ld32(sp) + UMD_medal); }
// SafeBuffer::read(sb, dst, data + index, n, index) with guest address arithmetic (index may be negative)
void decode(Addr sb, Addr dst, uint32_t index, uint32_t n) {
    const SafeBuffer *s = rw::host_ptr<SafeBuffer>(sb);
    SafeBuffer::read(s, rw::host_ptr<uint8_t>(dst), rw::host_ptr<uint8_t>(s->data.addr + index), n, index);
}
// a copy of a shared_ptr into `dst` (guest stack): add a reference
Addr copyShared(Addr dst, Addr obj, Addr ctrl) {
    st32(dst, obj);
    st32(dst + 4, ctrl);
    addRef(ctrl);
    return ctrl;
}
uint32_t staleRecordWord(uint32_t frame, uint32_t hole) { return ld32(rw::cpu->r[13] - PUSHED - frame + hole); }
void getMedalData(Addr out, Addr mgr, uint32_t lo, uint32_t hi, uint32_t stale) {
    call(F::MedalDataManager_getMedalData,
         {out, mgr, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, stale, 0, 0, 0, 0, 0, 0});
}
void getSkillData(Addr out, Addr mgr, uint32_t lo, uint32_t hi) {
    call(F::SkillDataManager_getData, {out, mgr, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
}
void setDisableMedalID(Addr list, uint32_t lo, uint32_t hi, uint32_t disable, uint32_t used) {
    call(F::MiniMedalScrollView_setDisableMedalID, {list, 0, lo, hi, disable, used});
}
void show(Addr node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
// node->getParent()->removeChild(node, true)
void removeFromParent(Addr node) {
    Addr parent = vcall(node, node::getParent, {node});
    vcall(parent, node::removeChild, {parent, node, 1});
}
// libc++ vector::erase tail fix-up as compiled: end = end + (~(size-1) & ~(end - size - newEnd))
void shrinkEnd(Addr endField, Addr newEnd, uint32_t size) {
    uint32_t end = ld32(endField);
    if (end != newEnd) st32(endField, end + (~(size - 1) & ~(end - size - newEnd)));
}
// a text with "%d"
void setTextNumber(Addr text, int32_t value) {
    Frame f;
    Addr buf = stringBuffer(f, {S_fmt_d, uint32_t(value)});
    setTextC(text, buf);
}
// text->setString(SceneUtil::getMoneyStr(value))
void setMoney(Addr text, int32_t value) {
    Frame f;
    Addr s = f.alloc(12);
    call(F::SceneUtil_getMoneyStr, {s, 0, uint32_t(value), uint32_t(value >> 31)});
    call(F::Text_setString, {text, s});
    strFree(s);
}
float i2f(int32_t v) { return float(v); }
uint64_t dbits(double d) { uint64_t u; std::memcpy(&u, &d, 8); return u; }
double bitsd(uint64_t u) { double d; std::memcpy(&d, &u, 8); return d; }
// libm calls through the guest (pow's last bit is the library's)
double guestPow(double x, double y) {
    uint64_t a = dbits(x), b = dbits(y);
    return bitsd(rw::call64(F::pow_, {uint32_t(a), uint32_t(a >> 32), uint32_t(b), uint32_t(b >> 32)}));
}
// __fixdfsi: truncate, saturate, NaN -> 0
int32_t d2i(double d) {
    if (d != d) return 0;
    if (d >= 2147483648.0) return INT32_MAX;
    if (d < -2147483648.0) return INT32_MIN;
    return int32_t(d);
}

}  // namespace

// ------------------------------------------------------------------------------------------------ the materials panel
// Recompute everything the materials give the base medal `base`: experience, munny cost, the warnings, the exp
// gauge, the slots, the mix button and the mix circle.
void Self::updateMaterial(SceneMixedMedal *self, const guest::SharedPtr *base) {
    Addr me = rw::guest_addr(self);
    Addr baseSp = rw::guest_addr(base);
    const uint32_t stale = staleRecordWord(FRAME_updateMaterial, HOLE_updateMaterial);
    Frame f;
    Addr misc115 = f.alloc(8);
    call(F::master_MedalMisc_create, {misc115, 115});
    self->mixedExp = 0;
    self->mixCost = 0;
    self->hasRareMaterial = 0;
    self->burstOverGuilt = 0;
    self->burstOverMax = 0;
    int32_t burstUps = 0;
    if (self->materialIds.end != self->materialIds.begin) {
        Addr material = f.alloc(8), baseCopy = f.alloc(8), matCopy = f.alloc(8);
        for (uint32_t i = 0, off = 0;; i++, off += 8) {
            Addr mdm = call(F::MedalDataManager_getInstance, {});
            Addr ids = self->materialIds.begin;
            if (!(uint32_t(int32_t(self->materialIds.end - ids) >> 3) > i)) {
                call(F::vector_out_of_range, {me + 0x310});
                return;
            }
            getMedalData(material, mdm, ld32(ids + off), ld32(ids + off + 4), stale);
            if (sb32(rowOf(material), MEDAL_rare) >= 3) self->hasRareMaterial = 1;
            {
                Addr bctrl = copyShared(baseCopy, ld32(baseSp), ld32(baseSp + 4));
                Addr mctrl = copyShared(matCopy, ld32(material), ld32(material + 4));
                int32_t exp = calcMixedExp(self, rw::host_ptr<guest::SharedPtr>(baseCopy),
                                           rw::host_ptr<guest::SharedPtr>(matCopy));
                self->mixedExp = int32_t(uint32_t(self->mixedExp) + uint32_t(exp));
                release(mctrl);
                release(bctrl);
            }
            {
                Addr bctrl = copyShared(baseCopy, ld32(baseSp), ld32(baseSp + 4));
                Addr mctrl = copyShared(matCopy, ld32(material), ld32(material + 4));
                uint32_t burstUp = isBurstUpMedal(self, rw::host_ptr<guest::SharedPtr>(baseCopy),
                                                  rw::host_ptr<guest::SharedPtr>(matCopy));
                release(mctrl);
                release(bctrl);
                if (burstUp == 1)
                    burstUps = int32_t(uint32_t(burstUps) + ld32(ld32(material) + UMD_burstUpper) + 1);
            }
            release(ld32(material + 4));
            if (!(i + 1 < uint32_t(int32_t(self->materialIds.end - self->materialIds.begin) >> 3))) break;
        }
    }
    {
        // munny: costRate * (materials * misc115 * base level) / 10000, scaled by misc123 at the max level
        int32_t perLevel = sb32(ld32(misc115), 4);
        uint32_t count = uint32_t(int32_t(self->materialIds.end - self->materialIds.begin) >> 3);
        uint32_t level = ld32(ld32(baseSp) + UMD_level);
        uint32_t units = count * (uint32_t(perLevel) * level);
        self->mixCost = f2i((i2f(self->costRate) * float(units)) / RATE_SCALE);
        int32_t baseLevel = int32_t(ld32(ld32(baseSp) + UMD_level));
        if (baseLevel == sb32(rowOf(baseSp), MEDAL_maxLevel)) {
            Addr misc123 = f.alloc(8);
            call(F::master_MedalMisc_create, {misc123, 123});
            float scale = i2f(sb32(ld32(misc123), 4)) / RATE_SCALE;
            self->mixCost = f2i(scale * i2f(self->mixCost));
            release(ld32(misc123 + 4));
        }
    }
    Addr left1 = self->centerUILeft1;
    {
        Addr text = search(left1, S_Txt_EXP, 7);
        setShadow(text);
        setTextId(text, 101300002);
    }
    {
        Addr label = search(self->centerUILeft1, S_Txt_EXP_Label, 13);
        setShadow(label);
        setTextNumber(label, self->mixedExp);
    }
    Addr costLabel;
    {
        Addr money1 = search(self->centerUILeft1, S_Base_Money1, 11);
        titleLabel(money1, S_Txt_Money, 9, 100300002);
        costLabel = search(money1, S_Txt_Money_Label, 15);
        setShadow(costLabel);
        setMoney(costLabel, self->mixCost);
    }
    {
        Addr money2 = search(self->centerUILeft1, S_Base_Money2, 11);
        titleLabel(money2, S_Txt_Money, 9, 100300003);
        Addr owned = search(money2, S_Txt_Money_Label, 15);
        setShadow(owned);
        Addr api = call(F::APIManager_getInstance, {});
        setMoney(owned, sb32(ld32(api + API_userMunny), 0));
    }
    {
        Frame g;
        Addr copy = g.alloc(8);
        Addr ctrl = copyShared(copy, ld32(baseSp), ld32(baseSp + 4));
        updateExpGauge(self, self->centerUILeft1, rw::host_ptr<guest::SharedPtr>(copy), self->mixedExp);
        release(ctrl);
    }
    updateMaterialsPanel(self);
    int32_t cost = self->mixCost;
    Addr api = call(F::APIManager_getInstance, {});
    int32_t munny = sb32(ld32(api + API_userMunny), 0);
    if (cost > munny) call(F::SceneUtil_setWarningColor, {costLabel});
    else call(F::SceneUtil_setStatusChangeColor, {costLabel});
    uint32_t enabled = (cost > munny) ? 0 : 1;
    if (self->materialIds.end == self->materialIds.begin) enabled = 0;
    Addr mixArea = search(self->centerUILeft1, S_Button_Mix, 10);
    Addr mixText = search(mixArea, S_Txt_Mix, 7);
    call(F::SceneUtil_setButtonEnabled, {self->mixButton, mixText, enabled, 0});
    if (enabled == 1) {
        // tutorial: close 117 (the material list) and show 118 on the mix button
        Addr app = call(F::AppState_getInstance, {});
        {
            Frame g;
            Addr closeFn = g.alloc(20);
            call(F::AppState_getTutorialCloseFunc, {closeFn, app, 117});
            Addr p = ld32(closeFn + 16);
            if (p) {
                functionDestroy(closeFn);
                call(F::AppState_execTutorialCloseFunc, {call(F::AppState_getInstance, {}), 117});
            }
        }
        Addr app2 = call(F::AppState_getInstance, {});
        Frame g;
        Addr none = newEmptyFunction(g);
        Addr popup = g.alloc(20);
        call(F::SceneUtil_openTutorialPopup, {popup, 118, none, mixArea, 0, 0, 0});
        Addr old = g.alloc(20);
        call(F::AppState_setTutorialCloseFunc, {old, app2, popup, 118});
        functionDestroy(old);
        functionDestroy(popup);
        functionDestroy(none);
    }
    Addr circle = search(self->centerUILeft1, S_Area_MixCircle, 14);
    Addr guiltIcon = search(self->centerUILeft1, S_Area_Guilt_Icon, 15);
    if (guiltIcon) show(guiltIcon, 0);
    Addr guiltPlate = search(self->centerUILeft1, S_Area_Guilt_Plate, 16);
    if (guiltPlate) show(guiltPlate, 0);
    {
        Addr api2 = call(F::APIManager_getInstance, {});
        uint32_t noGuilt = ld32(ld32(baseSp) + UMD_guiltFactor) == 0 ? 1 : 0;
        Frame g;
        Addr copy = g.alloc(8);
        Addr ctrl = copyShared(copy, self->baseMedal.obj, self->baseMedal.ctrl);
        uint32_t guiltOn = sb8(ld32(api2 + API_userData), USERDATA_guiltOn) != 0 ? 1 : 0;
        call(F::SceneUtil_updateMixCircle,
             {circle, guiltIcon, guiltPlate, copy, guiltOn, uint32_t(burstUps), noGuilt, 0, 0, 0, 0});
        release(ctrl);
    }
    release(ld32(misc115 + 4));
}
RW_FUNCTION(0x698638, "SceneMixedMedal::updateMaterial", SceneMixedMedal::updateMaterial);

// The experience gauge of `widget` after gaining `exp`: the bar now, then BattleUtil::calcMedalExp reports the
// level reached to a lambda (it updates the gauge and "LV" labels).
void Self::updateExpGauge(SceneMixedMedal *self, Addr widget, const guest::SharedPtr *medal, int32_t exp) {
    Addr me = rw::guest_addr(self);
    Addr medalSp = rw::guest_addr(medal);
    Addr gauge = search(widget, S_Gauge_EXP, 9);
    Addr bar = search(gauge, S_ProgressBar_EXP, 15);
    show(bar, 1);
    float ratio = 1.0f;
    {
        Addr data = ld32(medalSp);
        int32_t level = int32_t(ld32(data + UMD_level));
        if (level != sb32(ld32(data + UMD_medal), MEDAL_maxLevel)) {
            data = ld32(medalSp);
            uint32_t levelExp = ld32(data + UMD_levelExp);
            float num = i2f(int32_t(ld32(data + UMD_exp) - levelExp));
            float den = i2f(int32_t(ld32(data + UMD_nextLevelExp) - levelExp));
            float q = num / den;
            ratio = (q < 0.0f) ? 0.0f : q;
            if (ratio > 1.0f) ratio = 1.0f;
        }
    }
    {
        Addr setContentSize = ld32(ld32(bar) + node::setContentSize);
        Addr size = vcall(bar, node::getContentSize, {bar});
        Frame f;
        Addr newSize = f.alloc(8);
        call(F::Size_ctor, {newSize, fbits(ratio * 332.0f), ld32(size + 4)});
        call(setContentSize, {bar, newSize});
    }
    Addr lv = search(self->centerUILeft1, S_LV, 2);
    Addr lvNow = search(lv, S_LV_Now, 6);

    // the medal's server record (72 bytes) for calcMedalExp
    Frame f;
    Addr record = f.alloc(0x48);
    {
        Addr api = call(F::APIManager_getInstance, {});
        Addr list = ld32(api + API_userMedals);
        Addr key = f.alloc(8);
        st32(key, ld32(ld32(medalSp)));
        st32(key + 4, ld32(ld32(medalSp) + 4));
        Addr node = call(F::hash_u64_find, {list + 0x24, key});
        uint32_t off = node ? 72u * ld32(node + 0x10) : uint32_t(-72);
        decode(list, record, off, 0x48);
    }
    Addr obj = ld32(medalSp), ctrl = ld32(medalSp + 4);
    Addr data = obj;
    if (ctrl) {
        call(F::shared_add, {ctrl});
        data = ld32(medalSp);
        call(F::shared_add, {ctrl});
    }
    Addr callback = f.alloc(20);
    st32(callback + 16, 0);
    Addr heap = call(F::operator_new, {0x20});
    Addr leftover = ctrl;   // the reference the lambda did not take
    if (heap) {
        st32(heap, VT_expGauge);
        st32(heap + 4, lvNow);
        st32(heap + 8, obj);
        st32(heap + 0xC, ctrl);
        st32(heap + 0x10, bar);
        st32(heap + 0x14, gauge);
        st32(heap + 0x18, fbits(332.0f));
        st32(heap + 0x1C, me);
        leftover = 0;
    }
    st32(callback + 16, heap);
    call(F::BattleUtil_calcMedalExp, {data + UMD_medal, record, callback, uint32_t(exp)});
    functionDestroy(callback);
    release(leftover);
    release(ctrl);
}
RW_FUNCTION(0x699F68, "SceneMixedMedal::updateExpGauge", SceneMixedMedal::updateExpGauge);

// The smallest and largest guilt factor of the master::GuiltProb rows of `guiltGroup` (range[0], range[1]).
void Self::getGuiltFactorValueRange(int32_t guiltGroup, const uint32_t *range) {
    Addr out = rw::guest_addr(range);
    Frame f;
    Addr ids = f.alloc(12);                                   // std::set<int>
    call(F::master_GuiltProb_getIds, {ids});
    Addr row = f.alloc(8);
    Addr endNode = ids + 4;
    for (Addr node = ld32(ids); node != endNode;) {
        call(F::master_GuiltProb_create, {row, ld32(node + 0x10)});
        uint32_t factor = uint32_t(sb32(ld32(row), 8));
        if (sb32(ld32(row), 4) == guiltGroup) {
            uint32_t lo = ld32(out);
            if (lo == 0 || !(factor > lo)) st32(out, factor);
            uint32_t hi = ld32(out + 4);
            if (hi == 0 || hi < factor) st32(out + 4, factor);
        }
        release(ld32(row + 4));
        // in-order successor
        if (Addr right = ld32(node + 4)) {
            node = right;
            while (Addr left = ld32(node)) node = left;
        } else {
            Addr child;
            do {
                child = node;
                node = ld32(child + 8);
            } while (ld32(node) != child);
        }
    }
    call(F::tree_int_destroy, {ids, ld32(ids + 4)});
}
RW_FUNCTION(0x69A308, "SceneMixedMedal::getGuiltFactorValueRange", SceneMixedMedal::getGuiltFactorValueRange);

// Place the material views on their slots; the skill overwrite option follows the first material's skills.
void Self::updateMaterialsPanel(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    uint32_t enabled = 0;
    if (self->materialViews.end != self->materialViews.begin) {
        uint32_t count;
        for (uint32_t i = 0;; i++) {
            Addr pos = call(F::operator_new_array, {8});
            calcMaterialPosition(self, rw::host_ptr<float>(pos), int32_t(i));
            Addr views = self->materialViews.begin;
            if (!(uint32_t(int32_t(self->materialViews.end - views) >> 2) > i)) {
                call(F::vector_out_of_range, {me + 0x304});
                return;
            }
            Addr view = ld32(views + 4 * i);
            Addr setPosition = ld32(ld32(view) + node::setPosition);
            Addr area = self->materialArea;
            Frame f;
            Addr world = f.alloc(8), local = f.alloc(8);
            call(F::Vec2_ctorXY, {world, ld32(pos), ld32(pos + 4)});
            call(F::Node_convertToNodeSpace, {local, area, world});
            call(setPosition, {view, local});
            call(F::Vec2_dtor, {local});
            call(F::Vec2_dtor, {world});
            call(F::operator_delete_array, {pos});
            count = uint32_t(int32_t(self->materialViews.end - self->materialViews.begin) >> 2);
            if (!(i + 1 < count)) break;
        }
        if (count) {
            // the first material has skills
            Addr first = ld32(self->materialViews.begin);
            Addr ctrl = ld32(first + 0x358), obj = ld32(first + 0x354);
            if (ctrl) {
                call(F::shared_add, {ctrl});
                int32_t skills = int32_t(ld32(obj + UMD_skillCount));
                call(F::shared_release, {ctrl});
                enabled = skills > 0 ? 1 : 0;
            } else {
                enabled = int32_t(ld32(obj + UMD_skillCount)) > 0 ? 1 : 0;
            }
        }
    }
    call(F::SceneUtil_setButtonEnabled, {self->skillMixButton, self->skillMixButtonText, enabled, 0});
    if (enabled == 1) {
        if (self->lastMaterialCount != 0 || self->materialViews.end - self->materialViews.begin != 4)
            updateSkillMixPanel(self);
        else
            setEnabledSkillMix(self, 1);
    } else {
        setEnabledSkillMix(self, 0);
    }
    self->lastMaterialCount = int32_t(self->materialViews.end - self->materialViews.begin) >> 2;
}
RW_FUNCTION(0x69A418, "SceneMixedMedal::updateMaterialsPanel", SceneMixedMedal::updateMaterialsPanel);

// World position of material slot `index` (-1: the next free slot).
void Self::calcMaterialPosition(SceneMixedMedal *self, const float *out, int32_t index) {
    Addr dst = rw::guest_addr(out);
    if (index == -1) index = int32_t(self->materialViews.end - self->materialViews.begin) >> 2;
    float x = i2f(int32_t(uint32_t(110) * uint32_t(index)));
    Addr area = self->materialArea;
    Frame f;
    Addr offset = f.alloc(8), world = f.alloc(8);
    call(F::Vec2_ctorXY, {offset, fbits(x), 0});
    call(F::Node_convertToWorldSpace, {world, area, offset});
    call(F::Vec2_dtor, {offset});
    rw::save<float>(dst, (ldf(world) + 55.0f) + 2.0f);
    rw::save<float>(dst + 4, (ldf(world + 4) + 76.0f) + 6.0f);
    call(F::Vec2_dtor, {world});
}
RW_FUNCTION(0x69A5EC, "SceneMixedMedal::calcMaterialPosition", SceneMixedMedal::calcMaterialPosition);

// ------------------------------------------------------------------------------------------------ skill overwrite
void Self::setEnabledSkillMix(SceneMixedMedal *self, uint32_t enabled) {
    self->skillMixEnabled = uint8_t(enabled);
    vcall(self->baseSkill, node::setVisible, {self->baseSkill, enabled});
    show(self->materialAreaAnim, self->skillMixEnabled);
    call(F::ImageButton_setIsSelected, {self->skillMixButton, self->skillMixEnabled});
    bool off = self->skillMixEnabled == 0;
    Addr end = self->skillBasePanels.end;
    for (Addr it = self->skillBasePanels.begin; it != end; it += 4) {
        Addr panel = ld32(it);
        if (off) withStr(S_panelOff, 0x21, [&](Addr s) { call(F::ImageView_loadTexture, {panel, s, 0}); });
        else withStr(S_panelOn, 0x23, [&](Addr s) { call(F::ImageView_loadTexture, {panel, s, 0}); });
        off = self->skillMixEnabled == 0;
    }
    setTextId(self->skillMixTitle, off ? 101300020 : 101300028);
    updateSkillMixPanel(self);
}
RW_FUNCTION(0x69A6C0, "SceneMixedMedal::setEnabledSkillMix", SceneMixedMedal::setEnabledSkillMix);

// The skill overwrite panel: the first material's skill (name, icon), the success rate (MedalMisc 121..122 per
// skill level, campaign skillRate), which material slots add to it, and the "no effect" marks in the list.
void Self::updateSkillMixPanel(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    self->skillGroup = 0;
    self->hasNoEffectMaterial = 0;
    self->skillWillChange = 0;
    Addr nameLabel = search(self->centerUILeft1, S_Txt_SkillName_Label, 19);
    setShadow(nameLabel);
    Addr icon = search(self->centerUILeft1, S_Icon_Skill, 10);
    Frame f;
    Addr misc121 = f.alloc(8), misc122 = f.alloc(8);
    call(F::master_MedalMisc_create, {misc121, 121});
    float minRate = i2f(sb32(ld32(misc121), 4)) / RATE_SCALE;
    call(F::master_MedalMisc_create, {misc122, 122});
    float maxRate = i2f(sb32(ld32(misc122), 4)) / RATE_SCALE;
    {
        Addr end = self->skillAnimAreas.end;
        for (Addr it = self->skillAnimAreas.begin; it != end; it += 4) show(ld32(it), 0);
    }
    // the hasMixedEffectMedal counters (exp, attack, defense, burst, cost, count)
    Addr counters = f.alloc(24);
    for (uint32_t k = 0; k < 24; k += 4) st32(counters + k, 0);
    const Addr cExp = counters + 0x14, cAttack = counters + 0x10, cDefense = counters + 0xC, cBurst = counters + 8,
               cCost = counters + 4, cCount = counters;
    rw::set<uint32_t>(self, 0x344, 0);                        // skillMixRate = 0
    Addr copy = f.alloc(8), skill = f.alloc(8), skillCopy = f.alloc(8);
    if (self->materialViews.end != self->materialViews.begin) {
        float span = maxRate - minRate;
        int32_t group = 0;
        for (uint32_t i = 0;; i++) {
            Addr view = ld32(self->materialViews.begin + 4 * i);
            Addr ctrl = ld32(view + 0x358), obj = ld32(view + 0x354);
            Addr held = 0;
            if (ctrl) {
                call(F::shared_add, {ctrl});
                st32(copy, obj);
                st32(copy + 4, ctrl);
                call(F::shared_add, {ctrl});
                held = ctrl;
            } else {
                st32(copy, obj);
                st32(copy + 4, 0);
            }
            uint32_t effect = hasMixedEffectMedal(self, rw::host_ptr<guest::SharedPtr>(copy),
                                                  rw::host_ptr<int32_t>(cExp), rw::host_ptr<int32_t>(cAttack),
                                                  rw::host_ptr<int32_t>(cDefense), rw::host_ptr<int32_t>(cBurst),
                                                  rw::host_ptr<int32_t>(cCost), rw::host_ptr<int32_t>(cCount), 1);
            release(held);
            uint8_t enabled = self->skillMixEnabled;
            if (!enabled && int32_t(ld32(obj + UMD_skillCount)) > 0) self->skillWillChange = 1;
            uint32_t matched = 0;
            if (int32_t(ld32(obj + UMD_skillCount)) >= 1) {
                Addr idAddr = obj + UMD_skillIds;
                for (int32_t k = 0;; k++, idAddr += 8) {
                    uint32_t lo = ld32(idAddr), hi = ld32(idAddr + 4);
                    if (lo | hi) {
                        Addr skm = call(F::SkillDataManager_getInstance, {});
                        matched = 0;
                        getSkillData(skill, skm, lo, hi);
                        if (i == 0) {
                            // the first material: its skill is the one shown
                            Addr skillRow = ld32(ld32(skill) + 0x20);
                            group = sb32(skillRow, SKILL_group);
                            self->skillGroup = group;
                            {
                                Frame nf;
                                Addr name = nf.alloc(0x44);
                                decode(skillRow, name, SKILL_name, 0x41);
                                withStr(name, host_strlen(name), [&](Addr s) { call(F::Text_setString, {nameLabel, s}); });
                            }
                            Addr skm2 = call(F::SkillDataManager_getInstance, {});
                            Addr c = copyShared(skillCopy, ld32(skill), ld32(skill + 4));
                            Addr path = call(F::SkillDataManager_getImagePath, {skm2, skillCopy});
                            withStr(path, host_strlen(path), [&](Addr s) { call(F::ImageView_loadTexture, {icon, s, 0}); });
                            release(c);
                        }
                        Addr skillRow = ld32(ld32(skill) + 0x20);
                        if (group == sb32(skillRow, SKILL_group) && self->skillMixEnabled) {
                            // same skill group: adds its level's rate
                            Addr skillData = ld32(skill);
                            float rate = minRate;
                            if (sb32(skillData, 8) >= 2) {
                                int32_t maxLevel = sb32(ld32(skillData + 0x20), SKILL_maxLevel);
                                int32_t level = sb32(skillData, 8);
                                rate = minRate + (span / i2f(int32_t(uint32_t(maxLevel) - 1))) *
                                                     i2f(int32_t(uint32_t(level) - 1));
                            }
                            float campaign = i2f(self->skillRate) / RATE_SCALE;
                            self->skillMixRate = self->skillMixRate + rate * campaign;
                            matched = 1;
                            effect = 1;
                        } else {
                            self->skillWillChange = 1;
                        }
                        release(ld32(skill + 4));
                    }
                    if (!(k + 1 < int32_t(ld32(obj + UMD_skillCount)))) break;
                }
                enabled = self->skillMixEnabled;
            }
            if (i != 0 && enabled) {
                Addr anims = self->skillAnimAreas.begin;
                if (!(uint32_t(int32_t(self->skillAnimAreas.end - anims) >> 2) > i - 1)) {
                    call(F::vector_out_of_range, {me + 0x2D8});
                    return;
                }
                Addr anim = ld32(anims + 4 * (i - 1));
                show(anim, matched & 1);
            }
            if (!(effect & 1)) self->hasNoEffectMaterial = 1;
            release(ctrl);
            if (!(i + 1 < uint32_t(int32_t(self->materialViews.end - self->materialViews.begin) >> 2))) break;
        }
        float rate = self->skillMixRate;
        if (rate > 1.0f) self->skillMixRate = 1.0f;
    }
    Addr rateText = self->successRateText;
    if (self->skillMixEnabled) {
        Frame tf;
        Addr td = textData(tf, 101300021);
        int32_t percent = f2i(std::floor(self->skillMixRate * 100.0f));
        Addr buf = stringBuffer(tf, {textDataStr(td), uint32_t(percent)});
        setTextC(rateText, buf);
        sharedRelease(td);
    } else {
        withStr(S_empty, 0, [&](Addr s) { call(F::Text_setString, {rateText, s}); });
    }
    // the medals of the list that would add nothing
    call(F::MiniMedalScrollView_clearNoEffectMedalIDs, {self->medalList});
    if (self->sortFilter.filtered.end != self->sortFilter.filtered.begin) {
        for (uint32_t i = 0;; i++) {
            Addr entry = self->sortFilter.filtered.begin + 8 * i;
            Addr ctrl = copyShared(copy, ld32(entry), ld32(entry + 4));
            uint32_t effect = hasMixedEffectMedal(self, rw::host_ptr<guest::SharedPtr>(copy),
                                                  rw::host_ptr<int32_t>(cExp), rw::host_ptr<int32_t>(cAttack),
                                                  rw::host_ptr<int32_t>(cDefense), rw::host_ptr<int32_t>(cBurst),
                                                  rw::host_ptr<int32_t>(cCost), rw::host_ptr<int32_t>(cCount), 0);
            release(ctrl);
            if (!effect) {
                Addr data = ld32(self->sortFilter.filtered.begin + 8 * i);
                call(F::MiniMedalScrollView_addNoEffectMedalID, {self->medalList, 0, ld32(data), ld32(data + 4)});
            }
            if (!(i + 1 < uint32_t(int32_t(self->sortFilter.filtered.end - self->sortFilter.filtered.begin) >> 3)))
                break;
        }
    }
    call(F::IScrollProtocol_updateLayout, {self->medalList, 0});
    release(ld32(misc122 + 4));
    release(ld32(misc121 + 4));
}
RW_FUNCTION(0x69A928, "SceneMixedMedal::updateSkillMixPanel", SceneMixedMedal::updateSkillMixPanel);

// Does `material` still add something to the base medal (experience before the max level, a burst / attack /
// defense / cost upgrade with room left, or a skill change)? With `accumulate` the counters take its share and
// the over-limit warnings are raised.
uint32_t Self::hasMixedEffectMedal(SceneMixedMedal *self, const guest::SharedPtr *material, const int32_t *exp,
                                   const int32_t *attack, const int32_t *defense, const int32_t *burst,
                                   const int32_t *cost, const int32_t *count, uint32_t accumulate) {
    Addr me = rw::guest_addr(self);
    Addr matSp = rw::guest_addr(material);
    Addr pExp = rw::guest_addr(exp), pAttack = rw::guest_addr(attack), pDefense = rw::guest_addr(defense);
    Addr pBurst = rw::guest_addr(burst), pCost = rw::guest_addr(cost), pCount = rw::guest_addr(count);
    Frame f;
    Addr baseCopy = f.alloc(8), matCopy = f.alloc(8);
    int32_t mixedExp;
    {
        Addr bctrl = self->baseMedal.ctrl;
        st32(baseCopy, self->baseMedal.obj);
        st32(baseCopy + 4, bctrl);
        addRef(bctrl);
        Addr mctrl = copyShared(matCopy, ld32(matSp), ld32(matSp + 4));
        mixedExp = calcMixedExp(self, rw::host_ptr<guest::SharedPtr>(baseCopy), rw::host_ptr<guest::SharedPtr>(matCopy));
        release(mctrl);
        release(bctrl);
    }
    // the base medal's level after the experience gained so far
    uint32_t levelAfter;
    {
        Addr api = call(F::APIManager_getInstance, {});
        Addr list = ld32(api + API_userMedals);
        Addr key = f.alloc(8);
        Addr base = self->baseMedal.obj;
        st32(key, ld32(base));
        st32(key + 4, ld32(base + 4));
        Addr node = call(F::hash_u64_find, {list + 0x24, key});
        int32_t idx = node ? int32_t(ld32(node + 0x10)) : -1;
        Addr record = f.alloc(0x48);
        uint32_t off = uint32_t(idx) * 72u;
        decode(list, record, off, 0x48);
        Addr none = newEmptyFunction(f);
        levelAfter = call(F::BattleUtil_calcMedalExp, {self->baseMedal.obj + UMD_medal, record, none, ld32(pExp)});
        functionDestroy(none);
    }
    int32_t maxLevel = sb32(rowOf(me + 0x31C), MEDAL_maxLevel);
    if (accumulate) rw::save<int32_t>(pExp, int32_t(ld32(pExp) + uint32_t(mixedExp)));

    uint32_t canRaise = 0, touched = 0;
    if (self->baseBurst.obj) {
        Addr bctrl = copyShared(baseCopy, self->baseMedal.obj, self->baseMedal.ctrl);
        Addr mctrl = copyShared(matCopy, ld32(matSp), ld32(matSp + 4));
        uint32_t burstUp = isBurstUpMedal(self, rw::host_ptr<guest::SharedPtr>(baseCopy),
                                          rw::host_ptr<guest::SharedPtr>(matCopy));
        release(mctrl);
        release(bctrl);
        if (burstUp == 1) {
            int32_t burstMax = sb32(self->baseBurst.obj, BURST_maxLevel);
            Addr api = call(F::APIManager_getInstance, {});
            uint32_t room;
            Addr base = self->baseMedal.obj;
            if (sb8(ld32(api + API_userData), USERDATA_guiltOn) != 0 &&
                sb32(rowOf(me + 0x31C), MEDAL_guiltGroup) != 0) {
                base = self->baseMedal.obj;
                uint32_t guilt = ld32(base + UMD_guiltFactor);
                int32_t used;
                if (guilt != 0) {
                    used = int32_t(ld32(pBurst));
                    room = 0;
                    if (used == 0) {
                        room = guilt < self->guiltRange[1] ? 1 : 0;
                        if (guilt < self->guiltRange[1] && accumulate == 1) {
                            self->burstOverGuilt = 1;
                            used = int32_t(ld32(pBurst));
                        }
                    }
                } else {
                    used = int32_t(ld32(pBurst));
                    room = int32_t(uint32_t(used) + ld32(base + UMD_burstUpper)) < int32_t(uint32_t(burstMax) + 1) ? 1 : 0;
                }
                int32_t after = int32_t(ld32(base + UMD_burstUpper) + uint32_t(used) +
                                        ld32(ld32(matSp) + UMD_burstUpper) + 1);
                if (!(burstMax >= after) && accumulate == 1) self->burstOverMax = 1;
            } else {
                room = int32_t(ld32(pBurst) + ld32(base + UMD_burstUpper)) < burstMax ? 1 : 0;
            }
            canRaise = room ? 1 : 0;
            if (accumulate == 1)
                rw::save<uint32_t>(pBurst, ld32(ld32(matSp) + UMD_burstUpper) + ld32(pBurst) + 1);
            touched = 1;
        }
    }
    Addr baseRow = rowOf(me + 0x31C);
    int32_t attackUp = sb32(rowOf(matSp), MEDAL_attackUp);
    if (attackUp >= 1) {
        int32_t sum = int32_t(ld32(pAttack) + ld32(self->baseMedal.obj + UMD_attackUpper));
        canRaise |= sum < sb32(rowOf(me + 0x31C), MEDAL_attackMax) ? 1 : 0;
        touched = 1;
        if (accumulate == 1) rw::save<uint32_t>(pAttack, ld32(pAttack) + uint32_t(attackUp));
    }
    int32_t defenseUp = sb32(rowOf(matSp), MEDAL_defenseUp);
    if (sb32(rowOf(matSp), MEDAL_defenseUp) >= 1) {
        int32_t sum = int32_t(ld32(pDefense) + ld32(self->baseMedal.obj + UMD_defenseUpper));
        canRaise |= sum < sb32(rowOf(me + 0x31C), MEDAL_defenseMax) ? 1 : 0;
        touched = 1;
        if (accumulate == 1) rw::save<uint32_t>(pDefense, ld32(pDefense) + uint32_t(defenseUp));
    }
    (void)baseRow;
    int32_t costUp = sb32(rowOf(matSp), MEDAL_costUp);
    if (costUp >= 1) {
        int32_t upper = sb32(rowOf(me + 0x31C), MEDAL_costUpper);
        int32_t limit = sb32(rowOf(me + 0x31C), MEDAL_costMax);
        int32_t left = int32_t(uint32_t(limit) - ld32(self->baseMedal.obj + UMD_upperCost) - ld32(pCost));
        touched = 1;
        canRaise |= upper < left ? 1 : 0;
        if (accumulate == 1) rw::save<uint32_t>(pCost, ld32(pCost) + uint32_t(costUp));
    }

    // skills: the base medal's (last) skill id, the material's (last) skill id and whether one of the material's
    // skills is in the overwrite group
    uint32_t previous = ld32(pCount);
    int32_t baseSkill = 0;
    Addr skill = f.alloc(8);
    {
        Addr base = self->baseMedal.obj;
        if (int32_t(ld32(base + UMD_skillCount)) >= 1) {
            for (int32_t k = 0;; k++) {
                Addr idAddr = self->baseMedal.obj + UMD_skillIds + 8 * uint32_t(k);
                uint32_t lo = ld32(idAddr), hi = ld32(idAddr + 4);
                if (lo | hi) {
                    Addr skm = call(F::SkillDataManager_getInstance, {});
                    getSkillData(skill, skm, lo, hi);
                    Addr obj = ld32(skill), ctrl = ld32(skill + 4);
                    st32(skill, 0);
                    st32(skill + 4, 0);
                    baseSkill = sb32(ld32(obj + 0x20), 0);
                    release(ctrl);
                }
                if (!(k + 1 < int32_t(ld32(self->baseMedal.obj + UMD_skillCount)))) break;
            }
        }
    }
    uint32_t result;
    if (int32_t(ld32(ld32(matSp) + UMD_skillCount)) < 1) {
        result = canRaise;
    } else {
        int32_t matSkill = 0;
        uint32_t sameGroup = 0;
        for (int32_t k = 0;; k++) {
            Addr idAddr = ld32(matSp) + UMD_skillIds + 8 * uint32_t(k);
            uint32_t lo = ld32(idAddr), hi = ld32(idAddr + 4);
            if (lo | hi) {
                Addr skm = call(F::SkillDataManager_getInstance, {});
                getSkillData(skill, skm, lo, hi);
                Addr obj = ld32(skill), ctrl = ld32(skill + 4);
                st32(skill, 0);
                st32(skill + 4, 0);
                matSkill = sb32(ld32(obj + 0x20), 0);
                int32_t group = self->skillGroup;
                sameGroup |= group == sb32(ld32(obj + 0x20), SKILL_group) ? 1 : 0;
                release(ctrl);
            }
            if (!(k + 1 < int32_t(ld32(ld32(matSp) + UMD_skillCount)))) break;
        }
        touched = 1;
        bool checkMix;
        if (baseSkill == 0) {
            result = 1;
            checkMix = previous != 0;
        } else {
            result = (matSkill != baseSkill ? 1 : 0) | canRaise;
            checkMix = previous != 0 && matSkill != baseSkill;
        }
        if (checkMix) {
            if (!self->skillMixEnabled) {
                result = canRaise;
            } else {
                float rate = self->skillMixRate;
                if (rate >= 1.0f || rate != rate) result = canRaise;
                else result = canRaise | sameGroup;
            }
        }
    }
    if (accumulate == 1) rw::save<uint32_t>(pCount, ld32(pCount) + 1);
    if (touched == 1 && (result & 1)) return 1;
    if (int32_t(levelAfter) == maxLevel) return 0;
    return sb32(rowOf(matSp), MEDAL_baseExp) > 0 ? 1 : 0;
}
RW_FUNCTION(0x69B174, "SceneMixedMedal::hasMixedEffectMedal", SceneMixedMedal::hasMixedEffectMedal);

void Self::clearMaterial(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    if (self->materialIds.end != self->materialIds.begin) {
        uint32_t i = 0;
        for (;;) {
            Addr views = self->materialViews.begin;
            if (!(uint32_t(int32_t(self->materialViews.end - views) >> 2) > i)) {
                call(F::vector_out_of_range, {me + 0x304});
                return;
            }
            removeFromParent(ld32(views + 4 * i));
            i++;
            if (!(i < uint32_t(int32_t(self->materialIds.end - self->materialIds.begin) >> 3))) break;
        }
        shrinkEnd(me + 0x314, self->materialIds.begin, 8);
    }
    Addr end = self->materialViews.end;
    Addr begin = self->materialViews.begin;
    if (begin != end) {
        for (Addr it = begin; it != end; it += 4) {
            Addr mgr = call(F::DeckMedalViewManager_getInstance, {});
            call(F::DeckMedalViewManager_removeDeckMedalView, {mgr, ld32(it)});
        }
        shrinkEnd(me + 0x308, self->materialViews.begin, 4);
    }
}
RW_FUNCTION(0x69B90C, "SceneMixedMedal::clearMaterial", SceneMixedMedal::clearMaterial);

// Experience a material gives the base medal: MedalMisc 112 curve over the material's level, x MedalMisc 113 when
// both share a fusion group, x MedalMisc 114 when their group lists overlap, x the campaign rate.
int32_t Self::calcMixedExp(SceneMixedMedal *self, const guest::SharedPtr *base, const guest::SharedPtr *material) {
    Addr baseSp = rw::guest_addr(base), matSp = rw::guest_addr(material);
    // the original frame: the group arrays are read past their 3 entries when a medal lists more groups
    Frame fr;
    const Addr sp = fr.allocExact(PUSHED + FRAME_calcMixedExp);
    const Addr misc114 = sp + 0x30, misc113 = sp + 0x38, misc112 = sp + 0x40;
    call(F::master_MedalMisc_create, {misc112, 112});
    call(F::master_MedalMisc_create, {misc113, 113});
    call(F::master_MedalMisc_create, {misc114, 114});
    double matExp = double(sb32(rowOf(matSp), MEDAL_baseExp));
    decode(ld32(misc112), sp + 0xA4, 4, 12);
    double coefficient = double(int32_t(ld32(sp + 0xA8)));
    int32_t level = int32_t(ld32(ld32(matSp) + UMD_level));
    decode(ld32(misc112), sp + 0x98, 4, 12);
    int32_t curveLevel = int32_t(ld32(sp + 0x98));
    decode(ld32(misc112), sp + 0x8C, 4, 12);
    float ratio = i2f(int32_t(uint32_t(level) - 1)) / i2f(int32_t(uint32_t(curveLevel) - 1));
    float exponent = i2f(int32_t(ld32(sp + 0x94))) / RATE_SCALE;
    double value = std::floor(matExp + coefficient * guestPow(double(ratio), double(exponent)));
    int32_t baseGroup = sb32(rowOf(baseSp), MEDAL_mixGroup);
    if (baseGroup == sb32(rowOf(matSp), MEDAL_mixGroup)) {
        decode(ld32(misc113), sp + 0x80, 4, 12);
        value = std::floor(value * double(i2f(int32_t(ld32(sp + 0x80))) / RATE_SCALE));
    }
    bool shared = false;
    for (int32_t i = 0;; i++) {
        int32_t baseCount = sb32(rowOf(baseSp), MEDAL_mixGroupCount);
        if (i >= baseCount) break;
        for (int32_t k = 0;;) {
            int32_t matCount = sb32(rowOf(matSp), MEDAL_mixGroupCount);
            if (k >= matCount) break;
            decode(rowOf(baseSp), sp + 0x74, MEDAL_mixGroups, 12);
            int32_t a = int32_t(ld32(sp + 0x74 + 4 * uint32_t(i)));
            decode(rowOf(matSp), sp + 0x68, MEDAL_mixGroups, 12);
            int32_t b = int32_t(ld32(sp + 0x68 + 4 * uint32_t(k)));
            k++;
            if (a == b) {
                shared = true;
                break;
            }
        }
    }
    if (shared) {
        decode(ld32(misc114), sp + 0x5C, 4, 12);
        value = std::floor(value * double(i2f(int32_t(ld32(sp + 0x5C))) / RATE_SCALE));
    }
    int32_t result = d2i(std::floor(value * double(i2f(self->expRate) / RATE_SCALE)));
    release(ld32(misc114 + 4));
    release(ld32(misc113 + 4));
    release(ld32(misc112 + 4));
    return result;
}
RW_FUNCTION(0x69B9E0, "SceneMixedMedal::calcMixedExp", SceneMixedMedal::calcMixedExp);

// Does `material` raise the burst of `base`: the same medal family (evolution base, or the medal itself), one of
// the base's burst-up medals, or a medal of type 8?
uint32_t Self::isBurstUpMedal(SceneMixedMedal *, const guest::SharedPtr *base, const guest::SharedPtr *material) {
    Addr baseRow = rowOf(rw::guest_addr(base)), matRow = rowOf(rw::guest_addr(material));
    int32_t baseFamily = sb32(baseRow, MEDAL_evolveFrom);
    if (!baseFamily) baseFamily = sb32(baseRow, MEDAL_id);
    int32_t matFamily = sb32(matRow, MEDAL_evolveFrom);
    if (!matFamily) matFamily = sb32(matRow, MEDAL_id);
    if (matFamily == baseFamily) return 1;
    if (matFamily == sb32(baseRow, MEDAL_burstUpIds)) return 1;
    if (matFamily == sb32(baseRow, MEDAL_burstUpIds + 4)) return 1;
    return sb32(matRow, MEDAL_type) == 8 ? 1 : 0;
}
RW_FUNCTION(0x69BD64, "SceneMixedMedal::isBurstUpMedal", SceneMixedMedal::isBurstUpMedal);

// ------------------------------------------------------------------------------------------------ adding / removing
void Self::addMaterialMedal(SceneMixedMedal *self, uint64_t userMedalId) {
    Addr me = rw::guest_addr(self);
    const uint32_t stale = staleRecordWord(FRAME_addMaterialMedal, HOLE_addMaterialMedal);
    uint32_t lo = uint32_t(userMedalId), hi = uint32_t(userMedalId >> 32);
    Frame f;
    Addr id = f.alloc(8);
    st32(id, lo);
    st32(id + 4, hi);
    Addr medal = f.alloc(8);
    getMedalData(medal, call(F::MedalDataManager_getInstance, {}), lo, hi, stale);
    Addr viewSlot = f.alloc(4);
    Addr view = call(DeckMedalView_create, {1});
    st32(viewSlot, view);
    vcall(view, node::setAnchorPoint, {view, ANCHOR_MIDDLE});
    rw::save<uint8_t>(view + 0x389, 1);                       // DeckMedalView::isShowMixInfo
    call(DeckMedalView_changeShowType, {view, ld32(self->medalList + 0x6F4)});
    {
        Addr copy = f.alloc(8);
        Addr ctrl = copyShared(copy, ld32(medal), ld32(medal + 4));
        Addr detail = f.alloc(8);
        st32(detail, 0);
        st32(detail + 4, 0);
        call(DeckMedalView_setData, {view, copy, 0xFFFFFFFFu, 4, 1, 10000, 10000, 0, 0, 0, detail});
        release(ld32(detail + 4));
        release(ctrl);
    }
    vcall(self->materialArea, node::addChild, {self->materialArea, view});
    {
        Addr end = self->materialIds.end;
        if (end != ld32(me + 0x318)) {
            Addr base = 0;
            if (end) {
                st32(end, lo);
                st32(end + 4, hi);
                base = self->materialIds.end;
            }
            self->materialIds.end = base + 8;
        } else {
            call(F::vector_u64_push_slow_copy, {me + 0x310, id});
        }
    }
    {
        Addr end = self->materialViews.end;
        if (end != self->materialViews.cap) {
            Addr base = 0;
            if (end) {
                st32(end, view);
                base = self->materialViews.end;
            }
            self->materialViews.end = base + 4;
        } else {
            call(vector_deckMedalView_push_slow, {me + 0x304, viewSlot});
        }
    }
    call(DeckMedalViewManager_addDeckMedalView, {call(F::DeckMedalViewManager_getInstance, {}), view});
    setDisableMedalID(self->medalList, lo, hi, 1, 1);
    release(ld32(medal + 4));
}
RW_FUNCTION(0x69ED98, "SceneMixedMedal::addMaterialMedal", SceneMixedMedal::addMaterialMedal);

// "Auto select": fill the free slots with skill-less medals of type 2 (or type 1 up to rarity 3), sorted by the
// fusion group, as long as the base medal still needs experience (stops 999 past the max level).
void Self::settingOmakaseMedals(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    const uint32_t stale = staleRecordWord(FRAME_settingOmakase, HOLE_settingOmakase);
    int32_t freeSlots = 5 - (int32_t(self->materialIds.end - self->materialIds.begin) >> 3);
    Frame f;
    Addr medal = f.alloc(8), baseCopy = f.alloc(8), matCopy = f.alloc(8);
    int32_t gained = 0;
    if (self->materialIds.end != self->materialIds.begin) {
        for (uint32_t i = 0, off = 0;; i++, off += 8) {
            Addr mdm = call(F::MedalDataManager_getInstance, {});
            Addr ids = self->materialIds.begin;
            if (!(uint32_t(int32_t(self->materialIds.end - ids) >> 3) > i)) {
                call(F::vector_out_of_range, {me + 0x310});
                return;
            }
            getMedalData(medal, mdm, ld32(ids + off), ld32(ids + off + 4), stale);
            Addr bctrl = copyShared(baseCopy, self->baseMedal.obj, self->baseMedal.ctrl);
            Addr mctrl = copyShared(matCopy, ld32(medal), ld32(medal + 4));
            int32_t exp = calcMixedExp(self, rw::host_ptr<guest::SharedPtr>(baseCopy), rw::host_ptr<guest::SharedPtr>(matCopy));
            gained = int32_t(uint32_t(exp) + uint32_t(gained));
            release(mctrl);
            release(bctrl);
            release(ld32(medal + 4));
            if (!(i + 1 < uint32_t(int32_t(self->materialIds.end - self->materialIds.begin) >> 3))) break;
        }
    }
    Addr candidates = f.alloc(12);                            // std::vector<shared_ptr<UserMedalData>>
    st32(candidates, 0);
    st32(candidates + 4, 0);
    st32(candidates + 8, 0);
    int32_t remaining;
    {
        Addr base = self->baseMedal.obj;
        int32_t maxLevel = sb32(ld32(base + UMD_medal), MEDAL_maxLevel);
        int32_t need = int32_t(call(BattleUtil_calcNeedExp, {base + UMD_medal, uint32_t(maxLevel - 1)}));
        remaining = int32_t(uint32_t(need) - uint32_t(gained) - ld32(self->baseMedal.obj + UMD_exp));
    }
    if (self->sortFilter.filtered.end != self->sortFilter.filtered.begin) {
        Addr entry = f.alloc(8);
        for (uint32_t i = 0;; i++) {
            Addr src = self->sortFilter.filtered.begin + 8 * i;
            Addr obj = ld32(src), ctrl = ld32(src + 4);
            st32(entry, obj);
            st32(entry + 4, ctrl);
            addRef(ctrl);
            int32_t type = sb32(ld32(obj + UMD_medal), MEDAL_type);
            if (!(int32_t(ld32(obj + UMD_skillCount)) > 0)) {
                bool take = false;
                if (type == 2) take = true;
                else if (type == 1) take = !(sb32(ld32(obj + UMD_medal), MEDAL_rare) > 3);
                if (take) {
                    Addr end = ld32(candidates + 4);
                    if (end == ld32(candidates + 8)) {
                        call(F::vector_userMedal_push_slow, {candidates, entry});
                    } else {
                        if (end) {
                            st32(end, obj);
                            st32(end + 4, ctrl);
                            addRef(ctrl);
                        }
                        st32(candidates + 4, ld32(candidates + 4) + 8);
                    }
                }
            }
            release(ctrl);
            if (!(i + 1 < uint32_t(int32_t(self->sortFilter.filtered.end - self->sortFilter.filtered.begin) >> 3)))
                break;
        }
    }
    {
        // sort by the fusion group of the base medal (the comparator captures it)
        Addr comparator = f.alloc(8);
        st32(comparator, uint32_t(sb32(ld32(self->baseMedal.obj + UMD_medal), MEDAL_mixGroup)));
        call(sortCandidates, {ld32(candidates), ld32(candidates + 4), comparator});
    }
    if (int32_t(ld32(candidates + 4) - ld32(candidates)) >= 1) {
        uint32_t count = uint32_t(int32_t(ld32(candidates + 4) - ld32(candidates)) >> 3);
        uint32_t any = 0;
        int32_t added = 0;
        Addr pick = f.alloc(8);
        for (uint32_t i = 0;;) {
            if (remaining < 1) break;
            if (!(freeSlots > int32_t(int8_t(added)))) break;
            if (!(count > i)) {
                call(F::vector_out_of_range, {candidates});
                return;
            }
            Addr src = ld32(candidates) + 8 * i;
            Addr obj = ld32(src), ctrl = ld32(src + 4);
            Addr list;
            if (ctrl) {
                call(F::shared_add, {ctrl});
                list = self->medalList;
                st32(pick, obj);
                st32(pick + 4, ctrl);
                call(F::shared_add, {ctrl});
            } else {
                list = self->medalList;
                st32(pick, obj);
                st32(pick + 4, 0);
            }
            uint32_t can = call(MiniMedalScrollView_canSelectMaterialMedal, {list, pick});
            release(ld32(pick + 4));
            if (can == 1) {
                Addr bctrl = copyShared(baseCopy, self->baseMedal.obj, self->baseMedal.ctrl);
                st32(matCopy, obj);
                st32(matCopy + 4, ctrl);
                addRef(ctrl);
                int32_t exp = calcMixedExp(self, rw::host_ptr<guest::SharedPtr>(baseCopy), rw::host_ptr<guest::SharedPtr>(matCopy));
                release(ctrl);
                release(bctrl);
                int32_t left = int32_t(uint32_t(remaining) - uint32_t(exp));
                if (left >= -999) {
                    addMaterialMedal(self, uint64_t(ld32(obj)) | (uint64_t(ld32(obj + 4)) << 32));
                    setDisableMedalID(self->medalList, ld32(obj), ld32(obj + 4), 1, 0);
                    any = 1;
                    added++;
                    remaining = left;
                }
            }
            release(ctrl);
            count = uint32_t(int32_t(ld32(candidates + 4) - ld32(candidates)) >> 3);
            i++;
            if (!(int32_t(i) < int32_t(count))) break;
        }
        if (any & 1) call(F::IScrollProtocol_updateLayout, {self->medalList, 0});
    }
    call(F::vector_userMedal_dtor, {candidates});
}
RW_FUNCTION(0x69EFB4, "SceneMixedMedal::settingOmakaseMedals", SceneMixedMedal::settingOmakaseMedals);

void Self::removeMaterialMedal(SceneMixedMedal *self, int32_t index) {
    Addr me = rw::guest_addr(self);
    uint32_t idx = uint32_t(index);
    {
        Addr ids = self->materialIds.begin;
        if (!(uint32_t(int32_t(self->materialIds.end - ids) >> 3) > idx)) {
            call(F::vector_out_of_range, {me + 0x310});
            return;
        }
        setDisableMedalID(self->medalList, ld32(ids + 8 * idx), ld32(ids + 8 * idx + 4), 0, 1);
    }
    Addr views = self->materialViews.begin;
    if (!(uint32_t(int32_t(self->materialViews.end - views) >> 2) > idx)) {
        call(F::vector_out_of_range, {me + 0x304});
        return;
    }
    Addr view = ld32(views + 4 * idx);
    call(F::DeckMedalViewManager_removeDeckMedalView, {call(F::DeckMedalViewManager_getInstance, {}), view});
    {
        // materialViews.erase(begin + idx)
        Addr begin = self->materialViews.begin;
        Addr at = begin + 4 * idx;
        uint32_t n = self->materialViews.end - (at + 4);
        call(memmove_, {at, at + 4, n});
        shrinkEnd(me + 0x308, begin + ((n >> 2) + idx) * 4, 4);
    }
    {
        // materialIds.erase(begin + idx)
        Addr begin = self->materialIds.begin;
        Addr at = begin + 8 * idx;
        uint32_t n = self->materialIds.end - (at + 8);
        call(memmove_, {at, at + 8, n});
        shrinkEnd(me + 0x314, begin + ((n >> 3) + idx) * 8, 8);
    }
    removeFromParent(view);
    Frame f;
    Addr copy = f.alloc(8);
    Addr ctrl = copyShared(copy, self->baseMedal.obj, self->baseMedal.ctrl);
    updateMaterial(self, rw::host_ptr<guest::SharedPtr>(copy));
    release(ctrl);
}
RW_FUNCTION(0x69F800, "SceneMixedMedal::removeMaterialMedal", SceneMixedMedal::removeMaterialMedal);

// The medal info button: type 1 opens the medal details screen; coming back restores the previous base medal.
void Self::onTapMedalInfoButton(SceneMixedMedal *self, int32_t type, uint32_t, uint64_t userMedalId) {
    if (type != 1) return;
    Addr me = rw::guest_addr(self);
    Addr mgr = call(F::SceneManager_getInstance, {});
    uint32_t oldHi = ld32(mgr + 0x2A4), oldLo = ld32(mgr + 0x2A0);
    Addr mgr2 = call(F::SceneManager_getInstance, {});
    st32(mgr2 + 0x2A4, uint32_t(userMedalId >> 32));
    st32(mgr2 + 0x2A0, uint32_t(userMedalId));
    finalize(self);
    Frame f;
    Addr done = f.alloc(20);
    st32(done + 16, 0);
    Addr heap = call(F::operator_new, {0x18});
    if (heap) {
        st32(heap, VT_infoButton_back);
        st32(heap + 8, me);
        st32(heap + 0xC, 0);
        st32(heap + 0x10, oldLo);
        st32(heap + 0x14, oldHi);
    }
    st32(done + 16, heap);
    call(ISceneAnimation_runOutAnimation, {me + 0x2A4, done});
    functionDestroy(done);
}
RW_FUNCTION(0x69F70C, "SceneMixedMedal::onTapMedalInfoButton", SceneMixedMedal::onTapMedalInfoButton);
