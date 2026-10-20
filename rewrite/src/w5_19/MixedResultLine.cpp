// The fusion result's lines: sub_6A8908(context, kind) builds one MedalInfo_Panel_MixResult row - its title, the old
// value (half opacity) and the arrow, the new value colored by whether it reached its maximum - for the level (0),
// the EXP gained with the success animation (1), attack and defense (2, 3), their bonuses (4, 5), the cost
// reduction (6), the guilt circles old and new (7), the guilt (8) and the new skill (9).
// context: {this, medal (new), the API's +0x190 shared_ptr (the enhance result), ...}; the old medal is this +0x31C.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_setStatusChangeColor = 0x4659F1, SceneUtil_setStatusCountStopColor = 0x465A55;
constexpr uint32_t SceneUtil_updateMixCircle = 0x465C85, SceneUtil_setNormalColor = 0x468669;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t Mixed_getGuiltFactorValueRange = 0x69A309;
constexpr uint32_t SkillDataManager_getInstance = 0x76BA0D, SkillDataManager_getData = 0x76BA59;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5, GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t aeabi_uidiv = 0xCD5DF9;
}  // namespace g

constexpr uint32_t F_BASE = 0x31C;              // SceneMixedMedal: the old base medal
constexpr uint32_t S_PanelJson = 0xE05349;      // "cocostudio/publish/MedalInfo_Panel_MixResult_ver120.json"
constexpr uint32_t S_Text1 = 0xE05382, S_Text2 = 0xE05392, S_NumLabel = 0xDF8A89, S_Num2Label = 0xE053A2;
constexpr uint32_t S_DummyGuilt1 = 0xDFDD6E, S_DummyGuilt2 = 0xE053AD, S_Arrow = 0xE053BB, S_AreaPoint = 0xDF9FB4;
constexpr uint32_t S_SuccessJson = 0xE053C1;    // "cocostudio/publish/MedalMix_Anim_Success.ExportJson" (51)
constexpr uint32_t S_Success = 0xE053F5, S_Wonderful = 0xE05411, S_Great = 0xE0540B;
constexpr uint32_t S_AreaMixCircle = 0xDF8B8A, S_AreaGuiltIcon = 0xDF8B99, S_AreaGuiltPlate = 0xDF8BA9;
constexpr uint32_t S_FmtInt = 0xDF86DE;
constexpr int32_t TXT_TITLE = 101300039;        // + kind
constexpr int32_t TXT_PLUS = 101300049, TXT_COST = 101300050, TXT_GUILT = 101300051, TXT_NO_SKILL = 101300052;
constexpr uint32_t RESULT_KIND = 0x90;          // the enhance result: 3 wonderful, 2 great
constexpr uint32_t V_getAnimation = 0x298, V_play = 0x68;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline void shadow(uint32_t t) { SceneUtil::setShadow(t); }
inline void visible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline void changeColor(uint32_t t) { call(g::SceneUtil_setStatusChangeColor, {t}); }
inline void stopColor(uint32_t t) { call(g::SceneUtil_setStatusCountStopColor, {t}); }
inline void normalColor(uint32_t t) { call(g::SceneUtil_setNormalColor, {t}); }
inline int32_t val(uint32_t medal, uint32_t off) { return int32_t(ld32(medal + off)); }
int32_t master32(uint32_t medal, uint32_t off) {
    Frame f;
    const uint32_t v = f.alloc(4);
    const uint32_t sb = ld32(medal + 0x58);
    rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + off, 4, off});
    return int32_t(ld32(v));
}
// text = StringBuffer(fmt, value) with fmt a TextData entry (released after) or a literal
void setFmtText(uint32_t text, int32_t fmtId, uint32_t value) {
    Frame f;
    const uint32_t td = textData(f, fmtId);
    const uint32_t buf = stringBuffer(f, {textDataStr(td), value});
    setTextC(text, buf);
    sharedRelease(td);
}
void setIntText(uint32_t text, int32_t value) {
    Frame f;
    setTextC(text, stringBuffer(f, {S_FmtInt, uint32_t(value)}));
}
// the old value at half opacity, shown
void showOld(uint32_t num) {
    vcall(num, node::setOpacity, {num, 0x80});
    visible(num, 1);
}
// the guilt circles inside a dummy for a medal: icon and plate hidden, updateMixCircle(owned, 0, 1, 0...)
void guiltCircle(uint32_t dummy, uint32_t api, uint32_t medalObj, uint32_t medalCtl) {
    const uint32_t circle = find(dummy, S_AreaMixCircle, 14);
    const uint32_t icon = find(dummy, S_AreaGuiltIcon, 15);
    if (icon) visible(icon, 0);
    const uint32_t plate = find(dummy, S_AreaGuiltPlate, 16);
    if (plate) visible(plate, 0);
    Frame f;
    const uint32_t copy = f.alloc(8);
    st32(copy, medalObj);
    st32(copy + 4, medalCtl);
    if (medalCtl) call(fn::shared_add, {medalCtl});
    const uint32_t owned = f.alloc(4);
    const uint32_t sb = ld32(api + 0x94);
    rw::call(g::SafeBuffer_read, {sb, owned, ld32(sb + 0xC) + 0x54, 1, 0x54});
    call(g::SceneUtil_updateMixCircle, {circle, icon, plate, copy, ld8(owned) ? 1u : 0u, 0, 1, 0, 0, 0, 0});
    if (ld32(copy + 4)) call(fn::shared_release, {ld32(copy + 4)});
}
}  // namespace

struct MixedResultLine {
    static uint32_t create(uint32_t ctx, uint32_t kind);
};

uint32_t MixedResultLine::create(uint32_t ctx, uint32_t kind) {
    const uint32_t me = ld32(ctx);
    const uint32_t panel =
        call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_PanelJson});
    const uint32_t text1 = find(panel, S_Text1, 15);
    shadow(text1);
    const uint32_t text2 = find(panel, S_Text2, 15);
    shadow(text2);
    visible(text2, 0);
    const uint32_t num = find(panel, S_NumLabel, 9);
    shadow(num);
    visible(num, 0);
    const uint32_t num2 = find(panel, S_Num2Label, 10);
    shadow(num2);
    visible(num2, 0);
    const uint32_t guilt1 = find(panel, S_DummyGuilt1, 13);
    visible(guilt1, 0);
    const uint32_t guilt2 = find(panel, S_DummyGuilt2, 13);
    visible(guilt2, 0);
    const uint32_t arrow = find(panel, S_Arrow, 5);
    visible(arrow, 0);
    const uint32_t newM = ld32(ctx + 4);
    auto oldM = [&] { return ld32(me + F_BASE); };
    switch (kind) {
    case 0: {   // level
        setTextId(text1, TXT_TITLE + 0);
        visible(arrow, 1);
        setIntText(num, val(oldM(), 8));
        showOld(num);
        setIntText(num2, val(newM, 8));
        visible(num2, 1);
        if (master32(newM, 0x47C) > val(newM, 8)) normalColor(num2);
        else stopColor(num2);
        break;
    }
    case 1: {   // EXP and the success animation
        setTextId(text1, TXT_TITLE + 1);
        setFmtText(num2, TXT_PLUS, uint32_t(val(newM, 0x20) - val(oldM(), 0x20)));
        normalColor(num2);
        visible(num2, 1);
        const uint32_t point = find(panel, S_AreaPoint, 10);
        {
            const uint32_t mgr = call(g::ArmatureDataManager_getInstance, {});
            Frame f;
            const uint32_t s = newStr(f, S_SuccessJson, 51);
            call(g::ArmatureDataManager_addArmatureFileInfo, {mgr, s});
            strFree(s);
        }
        const uint32_t resultKind = ld32(ld32(ctx + 0xC) + RESULT_KIND);
        if (resultKind != 3 && resultKind != 2) break;
        uint32_t arm;
        {
            Frame f;
            const uint32_t s = newStr(f, S_Success, 21);
            arm = call(g::Armature_create, {s});
            strFree(s);
        }
        vcall(point, node::addChild, {point, arm});
        const uint32_t anim = vcall(arm, V_getAnimation, {arm});
        Frame f;
        const uint32_t s = resultKind == 3 ? newStr(f, S_Wonderful, 9) : newStr(f, S_Great, 5);
        vcall(anim, V_play, {anim, s, uint32_t(-1), uint32_t(-1)});
        strFree(s);
        break;
    }
    case 2:     // attack
    case 3: {   // defense
        const uint32_t v = kind == 2 ? 0xC : 0x10, plus = kind == 2 ? 0x2C : 0x30;
        setTextId(text1, TXT_TITLE + int32_t(kind));
        visible(arrow, 1);
        setIntText(num, val(oldM(), v) - val(oldM(), plus));
        showOld(num);
        setIntText(num2, val(newM, v) - val(newM, plus));
        normalColor(num2);
        visible(num2, 1);
        break;
    }
    case 4:     // attack bonus
    case 5:     // defense bonus
    case 6: {   // cost reduction
        const uint32_t v = kind == 4 ? 0x2C : kind == 5 ? 0x30 : 0x28;
        const int32_t fmt = kind == 6 ? TXT_COST : TXT_PLUS;
        setTextId(text1, TXT_TITLE + int32_t(kind));
        changeColor(text1);
        visible(arrow, 1);
        setFmtText(num, fmt, uint32_t(val(oldM(), v)));
        showOld(num);
        if (val(oldM(), v) >= 1) changeColor(num);
        else visible(num, 0);
        setFmtText(num2, fmt, uint32_t(val(newM, v)));
        visible(num2, 1);
        if (kind == 6) {
            const int32_t cost = master32(newM, 0x484);
            const int32_t reduced = val(newM, 0x28);
            const int32_t min = master32(newM, 0x488);
            if (cost - reduced > min) changeColor(num2);
            else stopColor(num2);
        } else {
            const int32_t max = master32(newM, kind == 4 ? 0x498 : 0x4A4);
            if (max > val(newM, v)) changeColor(num2);
            else stopColor(num2);
        }
        break;
    }
    case 7: {   // the guilt circles, old and new
        setTextId(text1, TXT_TITLE + 7);
        visible(arrow, 1);
        visible(guilt1, 1);
        visible(guilt2, 1);
        const uint32_t api = call(g::APIManager_getInstance, {});
        guiltCircle(guilt1, api, ld32(me + F_BASE), ld32(me + F_BASE + 4));
        guiltCircle(guilt2, api, newM, ld32(ctx + 8));
        break;
    }
    case 8: {   // guilt
        setTextId(text1, TXT_TITLE + 8);
        visible(arrow, 1);
        setFmtText(num, TXT_GUILT, call(g::aeabi_uidiv, {ld32(oldM() + 0x54), 100}));
        showOld(num);
        if (ld32(oldM() + 0x54)) changeColor(num);
        else visible(num, 0);
        setFmtText(num2, TXT_GUILT, call(g::aeabi_uidiv, {ld32(newM + 0x54), 100}));
        visible(num2, 1);
        Frame f;
        const uint32_t range = f.alloc(8);
        st32(range, 0);
        st32(range + 4, 0);
        call(g::Mixed_getGuiltFactorValueRange, {uint32_t(master32(newM, 0x478)), range});
        if (ld32(range + 4) > ld32(newM + 0x54)) changeColor(num2);
        else stopColor(num2);
        break;
    }
    case 9: {   // the new skill
        setTextId(text1, TXT_TITLE + 9);
        const uint32_t old = oldM();
        bool showSkills;
        if (val(old, 0x48) == 0) {
            showSkills = val(newM, 0x48) > 0;
        } else if (val(old, 0x48) < 1 ||
                   (ld32(old + 0x38) == ld32(newM + 0x38) && ld32(old + 0x3C) == ld32(newM + 0x3C))) {
            showSkills = false;
        } else {
            if (val(newM, 0x48) < 1) break;
            showSkills = true;
        }
        if (!showSkills) {
            setTextId(text2, TXT_NO_SKILL);
            visible(text2, 1);
            break;
        }
        for (int32_t i = 0; i < val(newM, 0x48); ++i) {
            const uint32_t lo = ld32(newM + 0x38 + 8 * i), hi = ld32(newM + 0x3C + 8 * i);
            if (!(lo | hi)) continue;
            const uint32_t sdm = call(g::SkillDataManager_getInstance, {});
            Frame f;
            const uint32_t out = f.alloc(8);
            call(g::SkillDataManager_getData, {out, sdm, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
            const uint32_t data = ld32(out), ctl = ld32(out + 4);
            st32(out, 0);
            st32(out + 4, 0);
            const uint32_t name = f.alloc(0x44);
            const uint32_t sb = ld32(data + 0x20);
            rw::call(g::SafeBuffer_read, {sb, name, ld32(sb + 0xC) + 0xC, 0x41, 0xC});
            setTextC(text2, name);
            visible(text2, 1);
            if (ctl) call(fn::shared_release, {ctl});
        }
        break;
    }
    default:
        break;
    }
    return panel;
}

RW_FUNCTION(0x6A8908, "SceneMixedMedal::openResultPopup result line (sub_6A8908)", MixedResultLine::create);
