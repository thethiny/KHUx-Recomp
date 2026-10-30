// SceneMixedMedal's own lambdas that a fusion runs: the Level Up button ($_6: enough Munny, then the caution popup
// for rare materials / skill / guilt, the skill mix popup, or straight to sub_6A7FDC: the guilt change popup or
// runMixed), the caution's answer, Optimize ($_5), Sell Medals ($_7: finalize, slide out, SceneSaleMedal), the
// new-mark clear ($_3), the material panel's touch ($_9), the EXP gauge animation step ($_12), and a few clones.
#include "../scene/SceneUi.hpp"

#include <cmath>

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;
constexpr uint32_t SceneUtil_setStatusChangeColor = 0x4659F1, SceneUtil_setStatusCountStopColor = 0x465A55;
constexpr uint32_t SceneUtil_numberToAtlasString = 0x466C41, SceneUtil_setNormalColor = 0x468669;
constexpr uint32_t SceneUtil_clearMedalNewMark = 0x46C4B5;
constexpr uint32_t FunctionVB_move = 0x47BC69, FunctionVB_call = 0x474139;
constexpr uint32_t FunctionVV_copy = 0x4683CD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_getTutorialCloseFunc = 0x4EBA3D;
constexpr uint32_t AppState_execTutorialCloseFunc = 0x4EBA5D;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;
constexpr uint32_t SaleMedal_create = 0x48A0ED, SaleMedal_setBackScene = 0x65C7C9, SaleMedal_setBack2Scene = 0x65C951;
constexpr uint32_t Mixed_updateMaterial = 0x698639, Mixed_openRareWarnPopup = 0x69BE85;
constexpr uint32_t Mixed_openSkillMixPopup = 0x69CC55, Mixed_openChangeGuiltPopup = 0x69D1AD;
constexpr uint32_t Mixed_runMixed = 0x69DB89, Mixed_settingOmakaseMedals = 0x69EFB5, Mixed_finalize = 0x69F56D;
constexpr uint32_t Mixed_mixGo = 0x6A7FDD;          // sub_6A7FDC({button, this, this}, ok)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t Rect_ctor = 0x8DF18B, Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t Node_convertToNodeSpace = 0x89599D, Touch_getLocation = 0x900E57;
constexpr uint32_t Size_ctor = 0x8DF027, Vec2_dtor = 0x8E4C17;
constexpr uint32_t aeabi_fixsfsi = 0xCD7240, aeabi_idiv = 0xCD5E8D;
constexpr uint32_t memcpy = 0x40E164;
}  // namespace g

// SceneMixedMedal
constexpr uint32_t F_INOUT = 0x2A4, F_SORT_BUTTON = 0x2F4, F_FROM = 0x2F8, F_PANEL = 0x300;
constexpr uint32_t F_MATERIAL_NODES = 0x304;   // vector<Node*>
constexpr uint32_t F_MATERIALS = 0x310, F_BASE = 0x31C, F_COST = 0x338;
constexpr uint32_t F_WARN_RARE = 0x33C, F_WARN_SKILL = 0x33D, F_WARN_GUILT = 0x33E, F_GUILT_CHANGE = 0x33F;
constexpr uint32_t F_BUSY = 0x348, F_TOUCH_LOCK = 0x349, F_PRESSED = 0x34A, F_PRESSED2 = 0x34B, F_PRESS_TIME = 0x350;
constexpr uint32_t F_PRESS_X = 0x354, F_PRESS_Y = 0x358, F_PRESS_INDEX = 0x364, F_PRESS_NODE = 0x368;
constexpr uint32_t F_SKILL_MIX = 0x380, F_CAN_MIX = 0x38C;
constexpr uint32_t IMAGEBUTTON_SELECTABLE = 0x272;
constexpr uint32_t VT_SellOut = 0xF65E68;       // init $_7's run-out callback {this}
constexpr uint32_t VT_SaleScene = 0xF65E94;     // function<Layer*()> {origin}
constexpr uint32_t VT_RunOutCopy = 0xF65E10;    // runOutAnimationMain $_8 {function<void()>} (heap 0x20)
constexpr uint32_t VT_MaterialTouchA = 0xF65D08, VT_MaterialTouchB = 0xF65DE4;   // initMaterialPanel $_11, $_9
constexpr uint32_t VT_RareAnswer = 0xF65EEC;    // $_6's caution answer {button, this, button, this, this}
constexpr uint32_t VT_SkillMixAnswer = 0xF65F44; // {button, this, this}
constexpr uint32_t VT_GuiltAnswer = 0xF65F18;    // {this}
constexpr uint32_t VT_LevelUpTouch = 0xF65EC0;   // init $_6 {this, button}
constexpr uint32_t S_ProgressBarRed = 0xE05324;  // "ProgressBar_RED"
constexpr int32_t TXT_NO_MONEY = 100300007;
constexpr uint32_t TUTORIAL_MIX = 118;
constexpr uint32_t V_setTouchEnabledW = 0x29C, V_setContentSize = 0xA8;

inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
// the {vt, a, b, c} std::function shapes and the button-selectable flags
void unlockButtons(uint32_t button, uint32_t me) {
    st8(button + IMAGEBUTTON_SELECTABLE, 1);
    st8(ld32(me + F_SORT_BUTTON) + IMAGEBUTTON_SELECTABLE, 1);
}
void openSkillMix(uint32_t me, uint32_t button, uint32_t a, uint32_t b) {
    Frame f;
    const uint32_t cb = newFunction(f, VT_SkillMixAnswer, {button, a, b});
    call(g::Mixed_openSkillMixPopup, {me, cb});
    functionDestroy(cb);
}
// AppState::getTutorialCloseFunc(118) returned as a std::function: destroyed at once; true when it was set
bool takeTutorialClose() {
    Frame f;
    const uint32_t out = f.alloc(20);
    call(g::AppState_getTutorialCloseFunc, {out, call(g::AppState_getInstance, {}), TUTORIAL_MIX});
    const uint32_t p = ld32(out + 16);
    if (p == out) vcall(p, 0x10, {p});
    else if (p) vcall(p, 0x14, {p});
    return p != 0;
}
}  // namespace

struct MixedSceneLambdas {
    static void levelUpTouch(uint32_t functor, uint32_t refRef, uint32_t typeRef);
    static void cloneLevelUpTouch(uint32_t functor, uint32_t dst);
    static uint32_t cloneRareAnswer(uint32_t functor);
    static void rareAnswer(uint32_t functor, uint32_t okRef);
    static void mixGo(uint32_t ctx, uint32_t ok);
    static void optimizeTouch(uint32_t functor, uint32_t refRef, uint32_t typeRef);
    static void clearNewMark(uint32_t functor, uint32_t fnRef);
    static void sellTouch(uint32_t functor, uint32_t refRef, uint32_t typeRef);
    static void sellOutDone(uint32_t functor);
    static uint32_t saleScene(uint32_t functor);
    static uint32_t cloneRunOutCopy(uint32_t functor);
    static void destroyRunOutCopy(uint32_t functor);
    static void cloneMaterialTouchA(uint32_t functor, uint32_t dst);
    static void cloneMaterialTouchB(uint32_t functor, uint32_t dst);
    static uint32_t materialTouchBegan(uint32_t functor, uint32_t touchRef, uint32_t);
    static void expGaugeStep(uint32_t functor, uint32_t levelRef, uint32_t expRef, uint32_t baseRef, uint32_t nextRef);
    static void materialTouchMoved(uint32_t functor, uint32_t touchRef, uint32_t);
    static void materialTouchEnded(uint32_t functor, uint32_t touchRef, uint32_t);
    static void materialDropped(uint32_t functor);
    static void materialInfoAnswer(uint32_t functor, uint32_t typeRef, uint32_t);
    static void baseMedalTouch(uint32_t functor, uint32_t, uint32_t typeRef);
    static void baseInfoAnswer(uint32_t functor, uint32_t typeRef, uint32_t);
    static void skillMixToggle(uint32_t functor, uint32_t, uint32_t typeRef);
    static void guiltAnswer(uint32_t functor, uint32_t okRef);
};

// init $_6 (Level Up) {this, button}: once a material is chosen and nothing is running - enough Munny (else the OK
// dialog), both buttons locked, then the caution popup when a material is rare / skills / guilt change (unless the
// Level Up tutorial is waiting), else the skill mix popup or straight on
void MixedSceneLambdas::levelUpTouch(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t me = ld32(f + 4);
    if (ld8(me + F_BUSY) || ld32(me + F_MATERIALS + 4) == ld32(me + F_MATERIALS) || !ld8(me + F_CAN_MIX)) return;
    call(g::AudioPlaySystemSEDecide, {});
    Frame fr;
    {
        const uint32_t sb = ld32(call(g::APIManager_getInstance, {}) + 0x8C);
        const uint32_t money = fr.alloc(4);
        rw::call(g::SafeBuffer_read, {sb, money, ld32(sb + 0xC), 4, 0});
        if (int32_t(ld32(money)) < int32_t(ld32(me + F_COST))) {
            const uint32_t td = textData(fr, TXT_NO_MONEY);
            const uint32_t s = newStr(fr, textDataStr(td));
            const uint32_t none = newEmptyFunction(fr);
            call(g::SceneUtil_openOkDialog4Line, {s, none, 0});
            functionDestroy(none);
            strFree(s);
            sharedRelease(td);
            return;
        }
    }
    const uint32_t button = ld32(f + 8);
    st8(button + IMAGEBUTTON_SELECTABLE, 0);
    st8(ld32(me + F_SORT_BUTTON) + IMAGEBUTTON_SELECTABLE, 0);
    const uint32_t ctx = fr.alloc(20);   // {button, this, button, this, this}
    st32(ctx, button);
    st32(ctx + 4, me);
    st32(ctx + 8, button);
    st32(ctx + 0xC, me);
    st32(ctx + 0x10, me);
    const bool tutorial = takeTutorialClose();
    if (!tutorial && (ld8(me + F_WARN_RARE) || ld8(me + F_WARN_SKILL) || ld8(me + F_WARN_GUILT))) {
        const uint32_t ctl = ld32(me + F_BASE + 4);
        if (ctl) call(fn::shared_add, {ctl});
        const uint32_t cb = fr.alloc(20);
        st32(cb + 16, 0);
        const uint32_t heap = call(fn::operator_new, {0x18});
        if (heap) {
            st32(heap, VT_RareAnswer);
            st32(heap + 4, button);
            st32(heap + 8, me);
            st32(heap + 0xC, button);
            st32(heap + 0x10, me);
            st32(heap + 0x14, me);
        }
        st32(cb + 16, heap);
        call(g::Mixed_openRareWarnPopup, {me, button, cb});
        functionDestroy(cb);
        release(ctl);
        return;
    }
    if (ld8(me + F_SKILL_MIX)) {
        openSkillMix(me, button, me, me);
        return;
    }
    call(g::Mixed_mixGo, {ctx + 8, 1});
}

void MixedSceneLambdas::cloneLevelUpTouch(uint32_t f, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_LevelUpTouch);
    st32(dst + 8, ld32(f + 8));
    st32(dst + 4, ld32(f + 4));
}

uint32_t MixedSceneLambdas::cloneRareAnswer(uint32_t f) {
    const uint32_t c = call(fn::operator_new, {0x18});
    if (c) {
        st32(c, VT_RareAnswer);
        for (uint32_t k = 4; k < 0x18; k += 4) st32(c + k, ld32(f + k));
    }
    return c;
}

// the caution's answer {button, this, button, this, this}: OK goes on (skill mix popup or sub_6A7FDC), Cancel
// unlocks the buttons
void MixedSceneLambdas::rareAnswer(uint32_t f, uint32_t okRef) {
    const uint32_t me = ld32(f + 8);
    if (!ld8(okRef)) {
        unlockButtons(ld32(f + 4), me);
        return;
    }
    if (ld8(me + F_SKILL_MIX)) {
        openSkillMix(me, ld32(f + 0xC), ld32(f + 0x10), ld32(f + 0x14));
        return;
    }
    call(g::Mixed_mixGo, {f + 0xC, 1});
}

// sub_6A7FDC({button, this, this}, ok): not ok unlocks the buttons; a guilt change asks first (the guilt popup),
// else the Level Up tutorial's close is run and the fusion starts
void MixedSceneLambdas::mixGo(uint32_t ctx, uint32_t ok) {
    const uint32_t me = ld32(ctx + 4);
    if (!ok) {
        unlockButtons(ld32(ctx), me);
        return;
    }
    if (!ld8(me + F_GUILT_CHANGE)) {
        const uint32_t scene = ld32(ctx + 8);
        if (takeTutorialClose()) call(g::AppState_execTutorialCloseFunc, {call(g::AppState_getInstance, {}), TUTORIAL_MIX});
        call(g::Mixed_runMixed, {scene});
        return;
    }
    Frame f;
    const uint32_t medal = f.alloc(8);
    st32(medal, ld32(me + F_BASE));
    const uint32_t ctl = ld32(me + F_BASE + 4);
    st32(medal + 4, ctl);
    if (ctl) call(fn::shared_add, {ctl});
    const uint32_t scene = ld32(ctx + 8);
    const uint32_t cb = newFunction(f, VT_GuiltAnswer, {scene});
    call(g::Mixed_openChangeGuiltPopup, {scene, medal, cb});
    functionDestroy(cb);
    release(ctl);
}

// init $_5 (Optimize) {this}: pick the materials, then show the base medal's material state
void MixedSceneLambdas::optimizeTouch(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t me = ld32(f + 4);
    call(g::AudioPlaySystemSEDecide, {});
    call(g::Mixed_settingOmakaseMedals, {me});
    Frame fr;
    const uint32_t medal = fr.alloc(8);
    st32(medal, ld32(me + F_BASE));
    const uint32_t ctl = ld32(me + F_BASE + 4);
    st32(medal + 4, ctl);
    if (ctl) call(fn::shared_add, {ctl});
    call(g::Mixed_updateMaterial, {me, medal});
    release(ctl);
}

// init $_3 (function<void(function<void(bool)>)>): the callback (moved in), the medals' new marks cleared, then
// the callback gets true
void MixedSceneLambdas::clearNewMark(uint32_t, uint32_t fnRef) {
    Frame f;
    const uint32_t cb = f.alloc(20);
    call(g::FunctionVB_move, {cb, fnRef});
    call(g::SceneUtil_clearMedalNewMark, {});
    call(g::FunctionVB_call, {cb, 1});
    functionDestroy(cb);
}

// init $_7 (Sell Medals) {this}: the list stops, the screen slides out, then SceneSaleMedal
void MixedSceneLambdas::sellTouch(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t me = ld32(f + 4);
    call(g::AudioPlaySystemSEDecide, {});
    call(g::Mixed_finalize, {me});
    Frame fr;
    const uint32_t done = newFunction(fr, VT_SellOut, {me});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, done});
    functionDestroy(done);
}

void MixedSceneLambdas::sellOutDone(uint32_t f) {
    const uint32_t from = ld32(ld32(f + 4) + F_FROM);
    const uint32_t sm = sceneManager();
    Frame fr;
    const uint32_t layer = newFunction(fr, VT_SaleScene, {from});
    rw::call(g::SM_changeSceneLayer, {sm, layer, 1, 0, 0});
    functionDestroy(layer);
}

uint32_t MixedSceneLambdas::saleScene(uint32_t f) {
    const uint32_t scene = call(g::SaleMedal_create, {});
    call(g::SaleMedal_setBackScene, {scene, 1});
    call(g::SaleMedal_setBack2Scene, {scene, ld32(f + 4)});
    return scene;
}

uint32_t MixedSceneLambdas::cloneRunOutCopy(uint32_t f) {
    const uint32_t c = call(fn::operator_new, {0x20});
    if (c) {
        st32(c, VT_RunOutCopy);
        call(g::FunctionVV_copy, {c + 8, f + 8});
    }
    return c;
}

void MixedSceneLambdas::destroyRunOutCopy(uint32_t f) {
    functionDestroy(f + 8);
    call(fn::operator_delete, {f});
}

void MixedSceneLambdas::cloneMaterialTouchA(uint32_t f, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_MaterialTouchA);
    st32(dst + 8, ld32(f + 8));
    st32(dst + 4, ld32(f + 4));
}

void MixedSceneLambdas::cloneMaterialTouchB(uint32_t f, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_MaterialTouchB);
    st32(dst + 8, ld32(f + 8));
    st32(dst + 4, ld32(f + 4));
}

// initMaterialPanel $_9 (touch began) {this, cell width}: a press on one of the chosen materials
uint32_t MixedSceneLambdas::materialTouchBegan(uint32_t f, uint32_t touchRef, uint32_t) {
    const uint32_t me = ld32(f + 4);
    if (ld8(me + F_TOUCH_LOCK)) return 0;
    const uint32_t panel = ld32(me + F_PANEL);
    const uint32_t w = ld32(vcall(panel, node::getContentSize, {panel}));
    const uint32_t h = ld32(vcall(panel, node::getContentSize, {panel}) + 4);
    Frame fr;
    const uint32_t rect = fr.alloc(16), loc = fr.alloc(8), local = fr.alloc(8);
    rw::call(g::Rect_ctor, {rect, 0, 0, w, h});
    const uint32_t node = ld32(me + F_PANEL);
    call(g::Touch_getLocation, {loc, ld32(touchRef)});
    call(g::Node_convertToNodeSpace, {local, node, loc});
    call(g::Vec2_dtor, {loc});
    uint32_t pressed = 0;
    if (call(g::Rect_containsPoint, {rect, local}) == 1) {
        const int32_t index = int32_t(call(g::aeabi_idiv, {call(g::aeabi_fixsfsi, {ld32(local)}), ld32(f + 8)}));
        if (index >= 0) {
            const uint32_t nodes = ld32(me + F_MATERIAL_NODES);
            if (uint32_t(int32_t(ld32(me + F_MATERIAL_NODES + 4) - nodes) >> 2) > uint32_t(index)) {
                const uint32_t x = ld32(local);
                st32(me + F_PRESS_Y, ld32(local + 4));
                st32(me + F_PRESS_X, x);
                st32(me + F_PRESS_INDEX, uint32_t(index));
                st32(me + F_PRESS_NODE, ld32(nodes + 4 * index));
                pressed = 1;
                st8(me + F_PRESSED2, 1);
                st32(me + F_PRESS_TIME, 0);
                st8(me + F_PRESSED, 1);
            }
        }
    }
    call(g::Vec2_dtor, {local});
    call(g::Vec2_dtor, {rect});
    return pressed;
}

// updateExpGauge $_12 (one step of the EXP count-up) {level text, medal, ?, Lv max mark, gauge panel, gauge width,
// this}: the level number and its color, the red bar's length, and the EXP left to the next level
void MixedSceneLambdas::expGaugeStep(uint32_t f, uint32_t levelRef, uint32_t expRef, uint32_t baseRef,
                                     uint32_t nextRef) {
    const int32_t next = int32_t(ld32(nextRef)), base = int32_t(ld32(baseRef)), exp = int32_t(ld32(expRef));
    const int32_t level = int32_t(ld32(levelRef));
    const uint32_t me = ld32(f + 0x1C);
    const uint32_t text = ld32(f + 4);
    {
        Frame fr;
        setTextC(text, stringBuffer(fr, {0xDF86DE, uint32_t(level)}));
    }
    const uint32_t medal = ld32(f + 8);
    auto maxLevel = [&] {
        Frame fr;
        const uint32_t v = fr.alloc(4);
        const uint32_t sb = ld32(medal + 0x58);
        rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + 0x47C, 4, 0x47C});
        return int32_t(ld32(v));
    };
    const uint32_t mark = ld32(f + 0x10);
    if (maxLevel() == level) {
        call(g::SceneUtil_setStatusCountStopColor, {ld32(f + 4)});
        if (int32_t(ld32(medal + 8)) < level) vcall(mark, node::setVisible, {mark, 0});
    } else if (int32_t(ld32(medal + 8)) >= level) {
        call(g::SceneUtil_setNormalColor, {ld32(f + 4)});
    } else {
        vcall(mark, node::setVisible, {mark, 0});
        call(g::SceneUtil_setStatusChangeColor, {ld32(f + 4)});
    }
    uint32_t bar;
    {
        Frame fr;
        const uint32_t s = fr.alloc(12);
        const uint32_t p = call(fn::operator_new, {0x10});
        st32(s, 0x11);
        st32(s + 4, 15);
        st32(s + 8, p);
        call(g::memcpy, {p, S_ProgressBarRed, 15});
        st8(p + 15, 0);
        bar = SceneUtil::searchNodeByName(ld32(f + 0x14), s);
        strFree(s);
    }
    float ratio = 1.0f;
    if (maxLevel() != level) {
        const float frac = float(exp - base) / float(next - base);
        ratio = frac < 0.0f ? 0.0f : frac;
        if (!(ratio <= 1.0f || std::isnan(ratio))) ratio = 1.0f;
    }
    {
        const float width = ldf(f + 0x18);
        const uint32_t height = ld32(vcall(bar, node::getContentSize, {bar}) + 4);
        Frame fr;
        const uint32_t size = fr.alloc(8);
        call(g::Size_ctor, {size, fbits(ratio * width), height});
        vcall(bar, V_setContentSize, {bar, size});
    }
    vcall(bar, node::setVisible, {bar, ratio > 0.0f ? 1u : 0u});
    const int32_t max = maxLevel();
    const uint32_t atlas = ld32(me + 0x2F0);
    const int32_t left = max != level ? next - exp : 0;
    Frame fr;
    const uint32_t s = fr.alloc(12);
    call(g::SceneUtil_numberToAtlasString, {s, uint32_t(left)});
    call(g::TextAtlas_setString, {atlas, s});
    strFree(s);
}

// ------------------------------------------------------------------------------------------------ the material panel
namespace {
namespace m {
constexpr uint32_t F_LIST = 0x2BC;              // MiniMedalScrollView* (the medal list)
constexpr uint32_t F_INFO_FROM_MATERIAL = 0x34C;  // the info popup was opened from a material slot
constexpr uint32_t F_DRAG_X = 0x35C, F_DRAG_Y = 0x360;   // the dragged copy's position (popup layer space)
constexpr uint32_t F_DRAG = 0x36C;              // the dragged copy (ui::Widget::clone of the pressed material)
constexpr uint32_t F_LIST_RECT = 0x370;         // cocos2d::Rect: the medal list in world space
constexpr uint32_t GOT_MOVE_X_DISTANCE = 0xFD692C;   // GOT: &MiniMedalScrollView::MOVE_X_DISTANCE
constexpr uint32_t VT_MaterialInfoAnswer = 0xF65D8C;   // $_11's info buttons {this, 0, id lo, id hi} (heap 0x18)
constexpr uint32_t VT_MaterialInfoClosed = 0xF65D60;   // $_11's info closed {this}
constexpr uint32_t VT_MaterialDropped = 0xF65D34;      // $_11's drop end {this, dropped on the list}
constexpr uint32_t VT_BaseInfoAnswer = 0xF65CB0;       // updateBaseMedal $_13's info buttons {this, 0, lo, hi} (0x18)
constexpr uint32_t VT_BaseInfoClosed = 0xF65C84;       // its info closed {this}
constexpr uint32_t SE_MATERIAL_REMOVED = 509000310;
constexpr uint32_t SE_LAYER = 10000;
constexpr float DRAG_OFFSET_X = -20.0f, DRAG_OFFSET_Y = 10.0f;
constexpr float START_DRAG = 10.0f, SCREEN_WIDTH = 960.0f;
constexpr uint32_t DRAG_OPACITY = 200;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t Mixed_removeMaterialMedal = 0x69F801, Mixed_setEnabledSkillMix = 0x69A6C1;
constexpr uint32_t Mixed_onTapMedalInfoButton = 0x69F70D;   // (this, type, -, -, u64 id on the stack)
constexpr uint32_t MiniMedal_updateDisabledItemIDs = 0x4FD059, IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t Node_convertToWorldSpace = rwlib::cocos2d::Node::convertToWorldSpace;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_ctor0 = rwlib::cocos2d::Vec2::ctor_base__v;
constexpr uint32_t Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t FadeOut_create = rwlib::cocos2d::FadeOut::create, ScaleTo_create = rwlib::cocos2d::ScaleTo::create__ff;
constexpr uint32_t Spawn_create = rwlib::cocos2d::Spawn::create, MoveTo_create = rwlib::cocos2d::MoveTo::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create, Sequence_create = rwlib::cocos2d::Sequence::create;

// SceneMixedMedal::onTapMedalInfoButton(this, type, 0, r3, id): the u64 id goes on the stack
void tapInfoButton(uint32_t me, uint32_t type, uint32_t r3, uint32_t lo, uint32_t hi) {
    Frame f;
    const uint32_t args = f.alloc(8);
    st32(args, lo);
    st32(args + 4, hi);
    rw::call_exact(Mixed_onTapMedalInfoButton, {me, type, 0, r3});
}
bool movedPast(float limit, float dx, float dy) { return limit <= std::fabs(dy) || limit <= std::fabs(dx); }
}  // namespace m
}  // namespace

// initMaterialPanel $_10 (touch moved) {this}: the dragged copy follows the touch; a press moving START_DRAG
// (in 960-wide screen units) starts the drag (a half transparent copy of the material on the popup layer, the
// material hidden); while dragging, a move of MiniMedalScrollView::MOVE_X_DISTANCE ends the tap
void MixedSceneLambdas::materialTouchMoved(uint32_t f, uint32_t touchRef, uint32_t) {
    const uint32_t me = ld32(f + 4);
    const uint32_t panel = ld32(me + F_PANEL);
    Frame fr;
    const uint32_t loc = fr.alloc(8), local = fr.alloc(8);
    call(g::Touch_getLocation, {loc, ld32(touchRef)});
    call(g::Node_convertToNodeSpace, {local, panel, loc});
    call(g::Vec2_dtor, {loc});
    const float dx = ldf(local) - ldf(me + F_PRESS_X);
    const float dy = ldf(local + 4) - ldf(me + F_PRESS_Y);
    const uint32_t drag = ld32(me + m::F_DRAG);
    if (drag) {
        const uint32_t pos = fr.alloc(8);
        call(m::Vec2_ctor, {pos, fbits(dx + ldf(me + m::F_DRAG_X)), fbits(dy + ldf(me + m::F_DRAG_Y))});
        vcall(drag, node::setPosition, {drag, pos});
        call(g::Vec2_dtor, {pos});
    }
    if (ld8(me + F_PRESSED2)) {
        if (ld8(me + F_TOUCH_LOCK)) {
            const float moveX = ldf(ld32(m::GOT_MOVE_X_DISTANCE));
            if (m::movedPast(visibleSize().width / m::SCREEN_WIDTH * moveX, dx, dy)) st8(me + F_PRESSED, 0);
        } else if (m::movedPast(visibleSize().width / m::SCREEN_WIDTH * m::START_DRAG, dx, dy)) {
            if (ld32(me + m::F_DRAG)) {
                const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
                vcall(layer, node::removeChild, {layer, ld32(me + m::F_DRAG), 1});
                st32(me + m::F_DRAG, 0);
            }
            st32(me + F_PRESS_X, ld32(local));
            st32(me + F_PRESS_Y, ld32(local + 4));
            const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
            const uint32_t node = ld32(me + F_PRESS_NODE);
            const uint32_t world = fr.alloc(8), inLayer = fr.alloc(8), pos = fr.alloc(8);
            call(m::Node_convertToWorldSpace, {world, ld32(me + F_PANEL), vcall(node, node::getPosition, {node})});
            call(g::Node_convertToNodeSpace, {inLayer, layer, world});
            call(g::Vec2_dtor, {world});
            call(m::Vec2_ctor, {pos, fbits(ldf(inLayer) + m::DRAG_OFFSET_X), fbits(ldf(inLayer + 4) + m::DRAG_OFFSET_Y)});
            st32(me + m::F_DRAG_X, ld32(pos));
            st32(me + m::F_DRAG_Y, ld32(pos + 4));
            call(g::Vec2_dtor, {pos});
            const uint32_t copy = call(m::Widget_clone, {ld32(me + F_PRESS_NODE)});
            st32(me + m::F_DRAG, copy);
            vcall(copy, node::setOpacity, {copy, m::DRAG_OPACITY});
            vcall(ld32(me + m::F_DRAG), node::setPosition, {ld32(me + m::F_DRAG), me + m::F_DRAG_X});
            vcall(layer, node::addChild, {layer, ld32(me + m::F_DRAG)});
            vcall(ld32(me + F_PRESS_NODE), node::setVisible, {ld32(me + F_PRESS_NODE), 0});
            st8(me + F_TOUCH_LOCK, 1);
            call(g::Vec2_dtor, {inLayer});
        }
    }
    call(g::Vec2_dtor, {local});
}

// initMaterialPanel $_11 (touch ended / cancelled) {this, slot width}: a tap on a material (not dragged) opens its
// info; a drag ends with the copy shrinking away over the medal list (dropped: the material is removed) or moving
// back to its slot, then the drop end
void MixedSceneLambdas::materialTouchEnded(uint32_t f, uint32_t touchRef, uint32_t) {
    const uint32_t me = ld32(f + 4);
    const uint32_t touch = ld32(touchRef);
    const uint32_t panel = ld32(me + F_PANEL);
    Frame fr;
    const uint32_t loc = fr.alloc(8), local = fr.alloc(8);
    call(g::Touch_getLocation, {loc, touch});
    call(g::Node_convertToNodeSpace, {local, panel, loc});
    call(g::Vec2_dtor, {loc});
    if (ld8(me + F_PRESSED2)) {
        st8(me + F_PRESSED2, 0);
        if (ld8(me + F_PRESSED) && ld8(me + F_CAN_MIX)) {
            st8(me + F_CAN_MIX, 0);
            call(fn::AudioPlaySystemSEDecide, {});
            const uint32_t ids = ld32(me + F_MATERIALS), index = ld32(me + F_PRESS_INDEX);
            if ((ld32(me + F_MATERIALS + 4) - ids) >> 3 <= index) call(m::ThrowOutOfRange, {});
            const uint32_t lo = ld32(ids + 8 * index), hi = ld32(ids + 8 * index + 4);
            st8(me + m::F_INFO_FROM_MATERIAL, 1);
            const uint32_t medal = fr.alloc(8);
            medalDataById(medal, lo, hi);
            openUserMedalInfo(medal, me, lo, hi, m::VT_MaterialInfoAnswer, m::VT_MaterialInfoClosed, 1);
            release(ld32(medal + 4));
        }
    }
    if (ld8(me + F_TOUCH_LOCK) && !ld8(me + F_BUSY)) {
        const uint32_t target = fr.alloc(8), at = fr.alloc(8);
        call(m::Vec2_ctor0, {target});
        call(g::Touch_getLocation, {at, touch});
        const uint32_t onList = call(g::Rect_containsPoint, {me + m::F_LIST_RECT, at});
        call(g::Vec2_dtor, {at});
        uint32_t action;
        if (onList == 1) {
            const uint32_t list = ld32(me + m::F_LIST);
            const float w = ldf(vcall(list, node::getContentSize, {list}));
            const float h = ldf(vcall(ld32(me + m::F_LIST), node::getContentSize, {ld32(me + m::F_LIST)}) + 4);
            const float slot = float(int32_t(ld32(f + 8)));
            const uint32_t size = fr.alloc(8), centre = fr.alloc(8), world = fr.alloc(8);
            call(g::Size_ctor, {size, fbits((w - slot) * 0.5f), fbits((h - slot) * 0.5f)});
            call(m::Vec2_ctor, {centre, ld32(size), ld32(size + 4)});
            call(m::Node_convertToWorldSpace, {world, list, centre});
            st32(target, ld32(world));
            st32(target + 4, ld32(world + 4));
            call(g::Vec2_dtor, {world});
            call(g::Vec2_dtor, {centre});
            const uint32_t fade = call(m::FadeOut_create, {fbits(0.15f)});
            action = call(m::Spawn_create, {fade, call(m::ScaleTo_create, {fbits(0.15f), fbits(1.2f)}), 0});
        } else {
            st32(target, ld32(me + m::F_DRAG_X));
            st32(target + 4, ld32(me + m::F_DRAG_Y));
            action = call(m::MoveTo_create, {fbits(0.2f), target});
        }
        st8(me + F_BUSY, 1);
        const uint32_t drag = ld32(me + m::F_DRAG);
        const uint32_t done = newFunction(fr, m::VT_MaterialDropped, {me, onList});
        call(fn::Node_runAction, {drag, call(m::Sequence_create, {action, call(m::CallFunc_create, {done}), 0})});
        functionDestroy(done);
        call(g::Vec2_dtor, {target});
    }
    call(g::Vec2_dtor, {local});
}

// $_11's drop end {this, dropped}: the copy removed, the material shown again; dropped on the list: the material
// is taken out (its sound); the drag state cleared
void MixedSceneLambdas::materialDropped(uint32_t f) {
    const uint32_t me = ld32(f + 4);
    const uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    vcall(layer, node::removeChild, {layer, ld32(me + m::F_DRAG), 1});
    st32(me + m::F_DRAG, 0);
    vcall(ld32(me + F_PRESS_NODE), node::setVisible, {ld32(me + F_PRESS_NODE), 1});
    if (ld8(f + 8)) {
        call(m::Mixed_removeMaterialMedal, {me, ld32(me + F_PRESS_INDEX)});
        playLayerSE(m::SE_MATERIAL_REMOVED, m::SE_LAYER);
    }
    st32(me + F_PRESS_NODE, 0);
    st8(me + F_TOUCH_LOCK, 0);
    st8(me + F_BUSY, 0);
}

// $_11's info buttons {-, this, -, id}(type, bool): 2 / 3 (the popup's own buttons) -> the material taken out when
// the popup came from its slot, the list redrawn; 1 -> the medal's detail
void MixedSceneLambdas::materialInfoAnswer(uint32_t f, uint32_t typeRef, uint32_t) {
    const uint32_t me = ld32(f + 8);
    const uint32_t type = ld32(typeRef);
    if (type - 2 < 2) {
        if (ld8(me + m::F_INFO_FROM_MATERIAL)) call(m::Mixed_removeMaterialMedal, {me, ld32(me + F_PRESS_INDEX)});
        call(m::MiniMedal_updateDisabledItemIDs, {ld32(me + m::F_LIST)});
        // the original leaves updateDisabledItemIDs' stack canary in r1: never 1, so not deferred
        call(m::IScrollProtocol_updateLayout, {ld32(me + m::F_LIST), 0});
        st8(me + m::F_INFO_FROM_MATERIAL, 0);
    } else if (type == 1) {
        m::tapInfoButton(me, 1, 0, ld32(f + 0x10), ld32(f + 0x14));
    }
}

// updateBaseMedal $_13 {this}(sender, type): the base medal tapped -> its info popup
void MixedSceneLambdas::baseMedalTouch(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t me = ld32(f + 4);
    if (!ld8(me + F_CAN_MIX)) return;
    st8(me + F_CAN_MIX, 0);
    call(fn::AudioPlaySystemSEDecide, {});
    const uint32_t base = ld32(me + F_BASE);
    openUserMedalInfo(me + F_BASE, me, ld32(base), ld32(base + 4), m::VT_BaseInfoAnswer, m::VT_BaseInfoClosed, 1);
}

// $_13's info buttons {-, this, -, id}(type, bool): SceneMixedMedal::onTapMedalInfoButton
void MixedSceneLambdas::baseInfoAnswer(uint32_t f, uint32_t typeRef, uint32_t) {
    m::tapInfoButton(ld32(f + 8), ld32(typeRef), ld32(f + 8), ld32(f + 0x10), ld32(f + 0x14));
}

// initSkillMixPanel $_14 {this}(sender, type): the skill overwrite switch toggled
void MixedSceneLambdas::skillMixToggle(uint32_t f, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t me = ld32(f + 4);
    call(fn::AudioPlaySystemSEDecide, {});
    call(m::Mixed_setEnabledSkillMix, {me, ld8(me + F_SKILL_MIX) == 0 ? 1u : 0u});
}

// init $_6's guilt change answer {this}(ok): OK -> the Level Up tutorial step closed (when waiting), then the
// fusion
void MixedSceneLambdas::guiltAnswer(uint32_t f, uint32_t okRef) {
    if (!ld8(okRef)) return;
    const uint32_t me = ld32(f + 4);
    execTutorialCloseIfSet(TUTORIAL_MIX);
    call(g::Mixed_runMixed, {me});
}

RW_FUNCTION(0x6A7ACC, "SceneMixedMedal::init $_6 (Level Up) __func::operator() (sub_6A7ACC)", MixedSceneLambdas::levelUpTouch);
RW_FUNCTION(0x6A7A9C, "SceneMixedMedal::init $_6 __func::__clone(p) (sub_6A7A9C)", MixedSceneLambdas::cloneLevelUpTouch);
RW_FUNCTION(0x6A7E5C, "SceneMixedMedal::init $_6 caution answer __func::__clone() (sub_6A7E5C)", MixedSceneLambdas::cloneRareAnswer);
RW_FUNCTION(0x6A7EC4, "SceneMixedMedal::init $_6 caution answer __func::operator() (sub_6A7EC4)", MixedSceneLambdas::rareAnswer);
RW_FUNCTION(0x6A7FDC, "SceneMixedMedal::init $_6 go on (sub_6A7FDC)", MixedSceneLambdas::mixGo);
RW_FUNCTION(0x6A8338, "SceneMixedMedal::init $_5 (Optimize) __func::operator() (sub_6A8338)", MixedSceneLambdas::optimizeTouch);
RW_FUNCTION(0x6A8504, "SceneMixedMedal::init $_3 __func::operator() (sub_6A8504)", MixedSceneLambdas::clearNewMark);
RW_FUNCTION(0x6A7788, "SceneMixedMedal::init $_7 (Sell Medals) __func::operator() (sub_6A7788)", MixedSceneLambdas::sellTouch);
RW_FUNCTION(0x6A78D0, "SceneMixedMedal::init $_7 run-out __func::operator() (sub_6A78D0)", MixedSceneLambdas::sellOutDone);
RW_FUNCTION(0x6A7A0A, "SceneMixedMedal::init $_7 sale scene __func::operator() (sub_6A7A0A)", MixedSceneLambdas::saleScene);
RW_FUNCTION(0x6A7618, "SceneMixedMedal::runOutAnimationMain $_8 __func::__clone() (sub_6A7618)", MixedSceneLambdas::cloneRunOutCopy);
RW_FUNCTION(0x6A76BC, "SceneMixedMedal::runOutAnimationMain $_8 __func::destroy_deallocate (sub_6A76BC)", MixedSceneLambdas::destroyRunOutCopy);
RW_FUNCTION(0x6A67BC, "SceneMixedMedal::initMaterialPanel $_11 __func::__clone(p) (sub_6A67BC)", MixedSceneLambdas::cloneMaterialTouchA);
RW_FUNCTION(0x6A73E8, "SceneMixedMedal::initMaterialPanel $_9 __func::__clone(p) (sub_6A73E8)", MixedSceneLambdas::cloneMaterialTouchB);
RW_FUNCTION(0x6A7418, "SceneMixedMedal::initMaterialPanel $_9 __func::operator() (sub_6A7418)", MixedSceneLambdas::materialTouchBegan);
RW_FUNCTION(0x6A6454, "SceneMixedMedal::updateExpGauge $_12 __func::operator() (sub_6A6454)", MixedSceneLambdas::expGaugeStep);
RW_FUNCTION(0x6A7060, "SceneMixedMedal::initMaterialPanel $_10 __func::operator() (touch moved)", MixedSceneLambdas::materialTouchMoved);
RW_FUNCTION(0x6A67EC, "SceneMixedMedal::initMaterialPanel $_11 __func::operator() (touch ended)", MixedSceneLambdas::materialTouchEnded);
RW_FUNCTION(0x6A6D38, "SceneMixedMedal::initMaterialPanel $_11 drop end __func::operator()", MixedSceneLambdas::materialDropped);
RW_FUNCTION(0x6A6F66, "SceneMixedMedal::initMaterialPanel $_11 info buttons __func::operator()", MixedSceneLambdas::materialInfoAnswer);
RW_FUNCTION(0x6A5FF4, "SceneMixedMedal::updateBaseMedal $_13 __func::operator()", MixedSceneLambdas::baseMedalTouch);
RW_FUNCTION(0x6A62E2, "SceneMixedMedal::updateBaseMedal $_13 info buttons __func::operator()", MixedSceneLambdas::baseInfoAnswer);
RW_FUNCTION(0x6A5F36, "SceneMixedMedal::initSkillMixPanel $_14 __func::operator()", MixedSceneLambdas::skillMixToggle);
RW_FUNCTION(0x6A8178, "SceneMixedMedal::init $_6 guilt answer __func::operator()(bool)", MixedSceneLambdas::guiltAnswer);
