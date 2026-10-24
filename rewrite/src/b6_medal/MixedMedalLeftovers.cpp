// SceneMixedMedal leftovers: openSkillMixPopup (the skill mix confirmation: the base medal's and the first
// material's skill names and the mix chance), openChangeGuiltPopup (the guilt bonus change: the medal's guilt
// percentage, its range, OK / Cancel through SceneManager::openPopup), the guilt bonus animation after a fusion
// (GuiltBonus.lwf with a skip button; then GuiltBonus_result or GuiltBonus_max with one digit per "numN" movie,
// and its enterFrame end), the material list's lambdas (back, reset, added, drop target, cell touch) and the
// std::__insertion_sort_incomplete of the material medals (vector<shared_ptr<UserMedalData>>).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t SM_getPopupLayer = 0x613E69, SM_changeSceneLayer = 0x612B99, SM_openPopup = 0x61524D;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15, SceneUtil_textToCustomTextView = 0x45BF15;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1, SceneUtil_setStatusChangeColor = 0x4659F1;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;
constexpr uint32_t SceneUtil_updateMixCircle = 0x465C85;
constexpr uint32_t CustomTextView_setXml = 0x4F931D;
constexpr uint32_t SceneMixedMedal_getGuiltFactorValueRange = 0x69A309;
constexpr uint32_t SceneMixedMedal_updateBaseMedal = 0x697835, SceneMixedMedal_updateMaterial = 0x698639;
constexpr uint32_t SceneMixedMedal_addMaterialMedal = 0x69ED99, SceneMixedMedal_removeMaterialMedal = 0x69F801;
constexpr uint32_t SceneMixedMedal_calcMaterialPosition = 0x69A5ED;
constexpr uint32_t SceneMixedMedal_onTapMedalInfoButton = 0x69F70D;
constexpr uint32_t GuiltFinish = 0x6A1AD5;                       // the chain's end (sub_6A1AD4)
constexpr uint32_t IMedalSortFilter_getShowType = 0x66527D;
constexpr uint32_t IScrollProtocol_setIndex = 0x4F71A5, IScrollProtocol_updateLayout = 0x4F6CF5;
constexpr uint32_t MiniMedal_setItemIDs = 0x4FD1FD, MiniMedal_changeShowType = 0x4FD531;
constexpr uint32_t MiniMedal_updateDisabledItemIDs = 0x4FD059;
constexpr uint32_t VectorU64_pushSlow = 0x4ECB6D, VectorU64_dtor = 0x468499;
constexpr uint32_t SkillDataManager_getInstance = 0x76BA0D, SkillDataManager_getData = 0x76BA59;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t AudioManager_getInstance = 0x5BECFD, AudioManager_getLayer = 0x5BF33D;
constexpr uint32_t AudioLayer_playSE = 0x5BF785;
constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_removeNewUserMedalID = 0x4EB2C5;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create, Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Node_pause = rwlib::cocos2d::Node::pause;
constexpr uint32_t LWF_ClearAllEventHandlers = rwlib::LWF::LWF::ClearAllEventHandlers;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t HandlerMap_insert_unique = 0x530869, HandlerMap_destroy = 0x530811;
constexpr uint32_t LWF_AddMovieEventHandler = rwlib::LWF::LWF::AddMovieEventHandler;
constexpr uint32_t WidgetVector_copy = 0x6A8799, WidgetVector_dtor = 0x485A8F, WidgetVector_deallocate = 0x6A2E65;
constexpr uint32_t MaterialLess = 0x6A02C5;                      // the material order (comparator, a, b)
constexpr uint32_t Sort3 = 0x6A0425, Sort4 = 0x6A04FB, Sort5 = 0x6A059B;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t FLOORF = 0x415244, MEMCPY = 0x40E164, STRLEN = 0x40E08C;
}  // namespace g

constexpr uint32_t S_GuiltJson = 0xE050E3;      // "cocostudio/publish/PopupNormal_Guilt_ver106.json"
constexpr uint32_t S_Win = 0xE04096, S_Txt_Wording1 = 0xDFF14C, S_Wording1Custom = 0xE05114;
constexpr uint32_t S_Txt_Wording2 = 0xDF863B, S_Wording2Custom = 0xE05128, S_Txt_Wording3 = 0xE0014B;
constexpr uint32_t S_Button_OK = 0xDF8688, S_Button_Close = 0xDF8692, S_Txt_OK = 0xDF869F, S_Txt_Cancel = 0xDF86A6;
constexpr uint32_t S_GuiltBonusLwf = 0xE0516F;  // "lwf/medal/guilt_bonus/GuiltBonus/GuiltBonus.lwf"
constexpr uint32_t S_GuiltResultLwf = 0xE051DD, S_GuiltMaxLwf = 0xE0519F, S_numD = 0xE051D7;
constexpr uint32_t S_enterFrame = 0xDFE0F3;
constexpr int32_t TXT_SKILL_MIX_BOTH = 101300027, TXT_SKILL_MIX = 101300026;
constexpr int32_t TXT_GUILT_NOW = 101300035, TXT_GUILT_RANGE = 101300036, TXT_GUILT_QUESTION = 101300037;
constexpr int32_t TXT_OK = 100100001, TXT_CANCEL = 100100002;
constexpr uint32_t SE_MATERIAL_SET = 509000290;

// __func vtables (+8)
constexpr uint32_t VT_SkillMixResult = 0xF65B24;   // {function<void(bool)>} (0x20)
constexpr uint32_t VT_GuiltOk = 0xF65AF8, VT_GuiltClose = 0xF65ACC, VT_GuiltCanClose = 0xF65AA0;
constexpr uint32_t VT_GuiltResult = 0xF65A48;      // {ok, close, window, layer, shadow, popup, function<void(bool)>}
constexpr uint32_t VT_BackLayer = 0xF65520;
constexpr uint32_t VT_GuiltShow = 0xF656AC;        // the guilt bonus state (0x54), run after the GuiltBonus LWF
constexpr uint32_t VT_GuiltTap = 0xF657B4;         // the skip button {button, lwf node}
constexpr uint32_t VT_GuiltEnter = 0xF65788;       // GuiltBonus "_root" enterFrame {lwf node, function<void()>}
constexpr uint32_t VT_ResultDigit = 0xF65704, VT_MaxDigit = 0xF6575C;    // "numN" load {n, node, lwf node, digit}
constexpr uint32_t VT_ResultEnter = 0xF656D8, VT_MaxEnter = 0xF65730;    // "_root" enterFrame (0x30)

// SceneMixedMedal
constexpr uint32_t F_SORT = 0x288, F_MATERIALS_IDS = 0x298, F_MINI = 0x2BC, F_LAYER_ARG = 0x2F8;
constexpr uint32_t F_MATERIAL_VIEWS = 0x304;       // vector<view*>: the view's UserMedalData at +0x354
constexpr uint32_t F_MATERIALS = 0x310;            // vector<ull> (the material ids)
constexpr uint32_t F_BASE = 0x31C;                 // shared_ptr<UserMedalData> (the base medal)
constexpr uint32_t F_MIX_RATE = 0x344, F_GUILT_SHOW = 0x340, F_DROPPED = 0x34C, F_SCROLL_RESET = 0x3B8;
constexpr uint32_t VIEW_MEDAL = 0x354;
constexpr uint32_t V_GET_SORT_FILTER = 0x308, V_ADD_CHILD_Z = 0xF4, V_IS_VISIBLE = 0xB4;
constexpr uint32_t V_SET_POSITION_X = 0x64, V_GET_POSITION_X = 0x68, V_SET_POSITION_Y = 0x6C;
constexpr uint32_t V_GET_POSITION_Y = 0x70;
constexpr uint32_t LWFNODE_LWF = 0x3E0, MOVIE_TOTAL_FRAMES = 0x98, MOVIE_CURRENT_FRAME = 0x9C;
constexpr uint32_t UM_MASTER = 0x58, UM_SKILLS = 0x38, UM_SKILL_COUNT = 0x48, UM_GUILT = 0x54, M_GUILT = 0x478;

uint32_t api() { return call(fn::APIManager_getInstance, {}); }
void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRef(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
void label(uint32_t root, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(root, name, len);
    call(g::SceneUtil_setShadow, {text, 1});
    setTextId(text, id);
}
void addHandler(uint32_t lwf, uint32_t name, uint32_t pair) {
    Frame f;
    const uint32_t map = f.alloc(12);
    st32(map + 4, 0);
    st32(map + 8, 0);
    st32(map, map + 4);
    call(g::HandlerMap_insert_unique, {map, map + 4, pair});
    call(g::LWF_AddMovieEventHandler, {lwf, name, map});
    call(g::HandlerMap_destroy, {map, ld32(map + 4)});
}
void loadKey(uint32_t pair) {
    st8(pair, 8);
    st8(pair + 1, 'l');
    st8(pair + 2, 'o');
    st8(pair + 3, 'a');
    st8(pair + 4, 'd');
    st8(pair + 5, 0);
}
void enterFrameKey(uint32_t pair) {
    st8(pair, 20);
    call(g::MEMCPY, {pair + 1, S_enterFrame, 10});
    st8(pair + 11, 0);
}
void rootName(uint32_t s) {
    st8(s, 10);
    st8(s + 1, '_');
    st8(s + 2, 'r');
    st8(s + 3, 'o');
    st8(s + 4, 'o');
    st8(s + 5, 't');
    st8(s + 6, 0);
}
uint32_t strChars(uint32_t s) { return (ld8(s) & 1) ? ld32(s + 8) : s + 1; }
// an LWF node centred in `parent` and added over it (z 9999)
uint32_t centredLwf(uint32_t path, uint32_t parent) {
    const uint32_t node = call(g::AudioLWFNode_create, {path, 0});
    const uint32_t setPos = ld32(ld32(node) + node::setPosition);
    const float w = ldf(vcall(parent, node::getContentSize, {parent}));
    const float h = ldf(vcall(parent, node::getContentSize, {parent}) + 4);
    {
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, fbits(w * 0.5f), fbits(h * 0.5f)});
        call(setPos, {node, pos});
        call(g::Vec2_dtor, {pos});
    }
    vcall(parent, V_ADD_CHILD_Z, {parent, node, 9999});
    return node;
}

// ---------------------------------------------------------------- openSkillMixPopup
// the name (master skill +0xC, 0x41 bytes) of skill `lo:hi` assigned to the std::string `out`; `moveOut` clears
// the fetched shared_ptr before the read (as the first loop does)
void skillName(uint32_t out, uint32_t lo, uint32_t hi, uint32_t data, uint32_t name, bool moveOut) {
    const uint32_t mgr = call(g::SkillDataManager_getInstance, {});
    call(g::SkillDataManager_getData, {data, mgr, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    const uint32_t p = ld32(data), ctl = ld32(data + 4);
    if (moveOut) {
        st32(data, 0);
        st32(data + 4, 0);
    }
    const uint32_t sb = ld32(p + 0x20);
    call(g::SafeBuffer_read, {sb, name, ld32(sb + 0xC) + 0xC, 0x41, 0xC});
    call(fn::string_assign, {out, name, call(g::STRLEN, {name})});
    releaseIf(moveOut ? ctl : ld32(data + 4));
}
uint32_t openSkillMixPopup(uint32_t self, uint32_t done) {
    Frame f;
    const uint32_t cb = f.alloc(20);
    call(fn::function_vb_copy, {cb, done});
    const uint32_t baseName = f.alloc(12), matName = f.alloc(12), msg = f.alloc(12);
    st32(baseName, 0);
    st32(baseName + 4, 0);
    st32(baseName + 8, 0);
    const uint32_t scaled = call(g::FLOORF, {fbits(ldf(self + F_MIX_RATE) * 100.0f)});
    const int32_t percent = int32_t(bitsf(scaled));
    const uint32_t data = f.alloc(8), name = f.alloc(0x44);
    uint32_t hasBase = 0;
    uint32_t base = ld32(self + F_BASE);
    if (base && int32_t(ld32(base + UM_SKILL_COUNT)) >= 1) {
        for (int32_t i = 0; i < int32_t(ld32(ld32(self + F_BASE) + UM_SKILL_COUNT)); i++) {
            const uint32_t id = ld32(self + F_BASE) + UM_SKILLS + 8 * uint32_t(i);
            const uint32_t lo = ld32(id), hi = ld32(id + 4);
            if (!(lo | hi)) continue;
            skillName(baseName, lo, hi, data, name, true);
            hasBase = 1;
        }
    }
    st32(matName, 0);
    st32(matName + 4, 0);
    st32(matName + 8, 0);
    if (ld32(self + F_MATERIAL_VIEWS + 4) == ld32(self + F_MATERIAL_VIEWS)) call(g::ThrowOutOfRange, {});
    const uint32_t view = ld32(ld32(self + F_MATERIAL_VIEWS));
    const uint32_t ctl = ld32(view + VIEW_MEDAL + 4), medal = ld32(view + VIEW_MEDAL);
    addRef(ctl);
    for (int32_t i = 0; i < int32_t(ld32(medal + UM_SKILL_COUNT)); i++) {
        const uint32_t id = medal + UM_SKILLS + 8 * uint32_t(i);
        if (!(ld32(id) | ld32(id + 4))) continue;
        skillName(matName, ld32(id), ld32(id + 4), data, name, false);
    }
    st32(msg, 0);
    st32(msg + 4, 0);
    st32(msg + 8, 0);
    {
        Frame t;
        uint32_t text, buf;
        if (hasBase == 1) {
            text = textData(t, TXT_SKILL_MIX_BOTH);
            buf = stringBuffer(t, {textDataStr(text), strChars(baseName), strChars(matName), uint32_t(percent)});
        } else {
            text = textData(t, TXT_SKILL_MIX);
            buf = stringBuffer(t, {textDataStr(text), strChars(matName), uint32_t(percent)});
        }
        call(fn::string_assign, {msg, buf, call(g::STRLEN, {buf})});
        releaseIf(ld32(text + 4));
    }
    const uint32_t s = f.alloc(12);
    call(fn::string_init, {s, strChars(msg), call(g::STRLEN, {strChars(msg)})});
    const uint32_t lam = f.alloc(20);
    functionCopy(lam, cb);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x20});
    if (h) {
        st32(h, VT_SkillMixResult);
        call(fn::function_vb_move, {h + 8, lam});
    }
    st32(fo + 16, h);
    call(g::SceneUtil_openOkCancelDialog4Line, {s, fo, 0, 0});
    functionDestroy(fo);
    functionDestroy(lam);
    strFree(s);
    strFree(msg);
    releaseIf(ctl);
    strFree(matName);
    strFree(baseName);
    functionDestroy(cb);
    return 0x984;
}

// ---------------------------------------------------------------- openChangeGuiltPopup
uint32_t openChangeGuiltPopup(uint32_t self, uint32_t medal, uint32_t done) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_GuiltJson});
    {
        const uint32_t setX = ld32(ld32(popup) + V_SET_POSITION_X);
        const float x = bitsf(vcall(popup, V_GET_POSITION_X, {popup}));
        call(setX, {popup, fbits(x + bitsf(call(g::SceneUtil_getSideMargin, {})))});
        const uint32_t setY = ld32(ld32(popup) + V_SET_POSITION_Y);
        const float y = bitsf(vcall(popup, V_GET_POSITION_Y, {popup}));
        call(setY, {popup, fbits(y + bitsf(call(g::SceneUtil_getSideHeightMargin, {})))});
    }
    vcall(layer, node::addChild, {layer, popup});
    const uint32_t window = findNode(popup, S_Win, 3);
    auto custom = [&](uint32_t textName, uint32_t customName) {
        const uint32_t text = findNode(window, textName, 12);
        Frame t;
        const uint32_t s = newStr(t, customName, 0x13);
        const uint32_t view = call(g::SceneUtil_textToCustomTextView, {text, s});
        strFree(s);
        return view;
    };
    auto setXml = [&](uint32_t view, uint32_t buf) {
        Frame t;
        const uint32_t s = newStr(t, buf, call(g::STRLEN, {buf}));
        call(g::CustomTextView_setXml, {view, s});
        strFree(s);
    };
    {
        const uint32_t view = custom(S_Txt_Wording1, S_Wording1Custom);
        Frame t;
        const uint32_t text = textData(t, TXT_GUILT_NOW);
        const uint32_t buf = stringBuffer(t, {textDataStr(text), ld32(ld32(medal) + UM_GUILT) / 100});
        setXml(view, buf);
        releaseIf(ld32(text + 4));
    }
    {
        const uint32_t view = custom(S_Txt_Wording2, S_Wording2Custom);
        Frame t;
        const uint32_t range = t.alloc(8), factor = t.alloc(4);
        st32(range, 0);
        st32(range + 4, 0);
        const uint32_t sb = ld32(ld32(medal) + UM_MASTER);
        call(g::SafeBuffer_read, {sb, factor, ld32(sb + 0xC) + M_GUILT, 4, M_GUILT});
        call(g::SceneMixedMedal_getGuiltFactorValueRange, {ld32(factor), range});
        const uint32_t text = textData(t, TXT_GUILT_RANGE);
        const uint32_t buf = stringBuffer(t, {textDataStr(text), ld32(range) / 100, ld32(range + 4) / 100});
        setXml(view, buf);
        releaseIf(ld32(text + 4));
    }
    {
        Frame t;
        const uint32_t s = newStr(t, S_Txt_Wording3, 12);
        const uint32_t text = call(g::SceneUtil_settingTitleLabel, {window, s, uint32_t(TXT_GUILT_QUESTION)});
        strFree(s);
        call(g::SceneUtil_setStatusChangeColor, {text, 1});
    }
    const uint32_t ok = findNode(popup, S_Button_OK, 9);
    const uint32_t close = findNode(popup, S_Button_Close, 12);
    Frame f;
    const uint32_t lam = f.alloc(0x2C);
    st32(lam, ok);
    st32(lam + 4, close);
    st32(lam + 8, window);
    st32(lam + 0xC, layer);
    st32(lam + 0x10, shadow);
    st32(lam + 0x14, popup);
    call(fn::function_vb_copy, {lam + 0x18, done});
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_GuiltOk, {});
        call(g::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
    }
    label(ok, S_Txt_OK, 6, TXT_OK);
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_GuiltClose, {});
        call(g::Widget_addTouchEventListener, {close, fo});
        functionDestroy(fo);
    }
    label(close, S_Txt_Cancel, 10, TXT_CANCEL);
    {
        Frame t;
        const uint32_t none = newEmptyFunction(t);
        call(fn::Node_runAction, {window, call(g::SceneUtil_createOpenPopupAnimation, {window, none})});
        functionDestroy(none);
    }
    {
        const uint32_t sm = sceneManager();
        Frame t;
        const uint32_t check = newFunction(t, VT_GuiltCanClose, {close});
        const uint32_t lam2 = t.alloc(0x2C);
        for (uint32_t k = 0; k < 0x18; k += 4) st32(lam2 + k, ld32(lam + k));
        functionCopy(lam2 + 0x18, lam + 0x18);
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x38});
        if (h) {
            st32(h, VT_GuiltResult);
            for (uint32_t k = 0; k < 0x18; k += 4) st32(h + 8 + k, ld32(lam2 + k));
            call(fn::function_vb_move, {h + 0x20, lam2 + 0x18});
        }
        st32(result + 16, h);
        call(g::SM_openPopup, {sm, check, result, 1});
        functionDestroy(result);
        functionDestroy(lam2 + 0x18);
        functionDestroy(check);
    }
    functionDestroy(lam + 0x18);
    return 0xA0C;
}

// ---------------------------------------------------------------- the material list
// back {-, scene}: the layer to go back to ({argument, 0, 2 words})
uint32_t backLayer(uint32_t self) {
    const uint32_t arg = ld32(ld32(self + 8) + F_LAYER_ARG);
    const uint32_t sm = sceneManager();
    const uint32_t a = ld32(self + 0x10), b = ld32(self + 0x14);
    Frame f;
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x18});
    if (h) {
        st32(h, VT_BackLayer);
        st32(h + 8, arg);
        st32(h + 0xC, 0);
        st32(h + 0x10, a);
        st32(h + 0x14, b);
    }
    st32(fo + 16, h);
    call(g::SM_changeSceneLayer, {sm, fo, 1, 0, 0});
    functionDestroy(fo);
    return 0;
}

// {scene}: the material list again (the medal ids, the first page, the sort's show type, the base medal and the
// materials)
uint32_t materialListReset(uint32_t self) {
    const uint32_t me = ld32(self + 4);
    Frame f;
    const uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    if (ld32(me + F_MATERIALS_IDS + 4) != ld32(me + F_MATERIALS_IDS)) {
        uint32_t i = 0;
        do {
            const uint32_t m = ld32(ld32(me + F_MATERIALS_IDS) + 8 * i);
            const uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(g::VectorU64_pushSlow, {ids, m});
            } else {
                if (end) {
                    st32(end, ld32(m));
                    st32(end + 4, ld32(m + 4));
                }
                st32(ids + 4, (end ? ld32(ids + 4) : 0) + 8);
            }
            i++;
        } while (i < (ld32(me + F_MATERIALS_IDS + 4) - ld32(me + F_MATERIALS_IDS)) >> 3);
    }
    st32(me + F_SCROLL_RESET, 0);
    call(g::IScrollProtocol_setIndex, {ld32(me + F_MINI), 0});
    call(g::MiniMedal_setItemIDs, {ld32(me + F_MINI), ids});
    const uint32_t state = f.alloc(8);
    vcall(me, V_GET_SORT_FILTER, {state, me});
    const uint32_t showType = call(g::IMedalSortFilter_getShowType, {me + F_SORT, state, 0});
    releaseIf(ld32(state + 4));
    call(g::MiniMedal_changeShowType, {ld32(me + F_MINI), showType});
    const uint32_t base = ld32(me + F_BASE);
    call(g::SceneMixedMedal_updateBaseMedal, {me, F_BASE, ld32(base), ld32(base + 4)});
    const uint32_t copy = f.alloc(8);
    st32(copy, ld32(me + F_BASE));
    const uint32_t ctl = ld32(me + F_BASE + 4);
    st32(copy + 4, ctl);
    addRef(ctl);
    call(g::SceneMixedMedal_updateMaterial, {me, copy});
    releaseIf(ctl);
    call(g::VectorU64_dtor, {ids});
    return 0;
}

// {scene}(u64 id): a material medal set: the sound, no longer new, added, the materials redrawn
uint32_t materialAdded(uint32_t self, uint32_t, uint32_t idp) {
    const uint32_t lo = ld32(idp), hi = ld32(idp + 4);
    const uint32_t me = ld32(self + 4);
    Frame f;
    const uint32_t layer = f.alloc(8), copy = f.alloc(8);
    call(g::AudioManager_getLayer, {layer, call(g::AudioManager_getInstance, {}), 10000});
    call(g::AudioLayer_playSE, {ld32(layer), SE_MATERIAL_SET, 0, 0});
    releaseIf(ld32(layer + 4));
    call(g::AppState_removeNewUserMedalID, {call(g::AppState_getInstance, {}), 0, lo, hi});
    call(g::SceneMixedMedal_addMaterialMedal, {me, 0, lo, hi});
    st32(copy, ld32(me + F_BASE));
    const uint32_t ctl = ld32(me + F_BASE + 4);
    st32(copy + 4, ctl);
    addRef(ctl);
    call(g::SceneMixedMedal_updateMaterial, {me, copy});
    releaseIf(ctl);
    return 0;
}

// {scene}(u64, Vec2 point, float* pos): the drop target: the next material slot (at most 5), -1 when full
uint32_t dropTarget(uint32_t self, uint32_t, uint32_t point, uint32_t posp) {
    Frame f;
    const uint32_t p = f.alloc(8);
    call(g::Vec2_copy, {p, point});
    const uint32_t me = ld32(self + 4);
    const uint32_t n = (ld32(me + F_MATERIALS + 4) - ld32(me + F_MATERIALS)) >> 3;
    uint32_t result = 0xFFFFFFFF;
    if (n <= 4) {
        call(g::SceneMixedMedal_calcMaterialPosition, {me, ld32(posp), 0xFFFFFFFF});
        result = n;
    }
    call(g::Vec2_dtor, {p});
    return result;
}

// a material cell {-, scene, -, id lo, id hi}(type, bool): 1 -> its info; 2 / 3 -> drop it from the materials
// when it was dropped there, then the list redrawn
uint32_t materialCellTouch(uint32_t self, uint32_t typep, uint32_t flagp) {
    const uint32_t me = ld32(self + 8);
    const uint32_t type = ld32(typep);
    if (type - 2 >= 2) {
        if (type != 1) return self;
        Frame f;
        const uint32_t args = f.alloc(8);
        st32(args, ld32(self + 0x10));
        st32(args + 4, ld32(self + 0x14));
        return rw::call_exact(g::SceneMixedMedal_onTapMedalInfoButton, {me, 1, 0, type - 2});
    }
    if (ld8(flagp) && ld8(me + F_DROPPED)) {
        const uint32_t begin = ld32(me + F_MATERIALS);
        const int32_t bytes = int32_t(ld32(me + F_MATERIALS + 4) - begin);
        const int32_t n = bytes >> 3;
        if (bytes >= 1) {
            const uint32_t lo = ld32(self + 0x10), hi = ld32(self + 0x14);
            for (int32_t i = 0; i < n; i++) {
                const uint32_t e = begin + 8 * uint32_t(i);
                if (ld32(e) == lo && ld32(e + 4) == hi) {
                    call(g::SceneMixedMedal_removeMaterialMedal, {me, uint32_t(i)});
                    st8(me + F_DROPPED, 0);
                    break;
                }
            }
        }
    }
    call(g::MiniMedal_updateDisabledItemIDs, {ld32(me + F_MINI)});
    return call(g::IScrollProtocol_updateLayout, {ld32(me + F_MINI), 0});
}

// std::__insertion_sort_incomplete over shared_ptr<UserMedalData> (moves: the words taken, the source cleared)
uint32_t less(uint32_t comp, uint32_t a, uint32_t b) { return call(g::MaterialLess, {comp, a, b}); }
uint32_t insertionSortIncomplete(uint32_t first, uint32_t last, uint32_t comp) {
    switch ((last - first) >> 3) {
    case 0:
    case 1: return 1;
    case 2:
        if (less(comp, last - 8, first) == 1) {
            const uint32_t a = ld32(first), b = ld32(first + 4);
            st32(first, ld32(last - 8));
            st32(last - 8, a);
            st32(first + 4, ld32(last - 4));
            st32(last - 4, b);
        }
        return 1;
    case 3: call(g::Sort3, {first, first + 8, last - 8, comp}); return 1;
    case 4: call(g::Sort4, {first, first + 8, first + 16, last - 8, comp}); return 1;
    case 5: {
        Frame f;
        const uint32_t args = f.alloc(8);
        st32(args, last - 8);
        st32(args + 4, comp);
        rw::call_exact(g::Sort5, {first, first + 8, first + 16, first + 24});
        return 1;
    }
    default: break;
    }
    uint32_t j = first + 16;
    call(g::Sort3, {first, first + 8, j, comp});
    if (first + 24 == last) return 1;
    Frame f;
    const uint32_t t = f.alloc(8);
    uint32_t count = 0;
    for (uint32_t i = first + 24;;) {
        if (less(comp, i, j) == 1) {
            st32(t, ld32(i));
            st32(t + 4, ld32(i + 4));
            st32(i, 0);
            st32(i + 4, 0);
            uint32_t hole = i, k = j;
            for (;;) {
                const uint32_t v0 = ld32(k), v1 = ld32(k + 4);
                st32(k, 0);
                st32(k + 4, 0);
                st32(hole, v0);
                const uint32_t old = ld32(hole + 4);
                st32(hole + 4, v1);
                releaseIf(old);
                if (first == k) break;
                hole = k;
                if (!less(comp, t, k - 8)) break;
                k -= 8;
            }
            st32(k, ld32(t));
            const uint32_t old = ld32(k + 4);
            st32(k + 4, ld32(t + 4));
            st32(t, 0);
            st32(t + 4, 0);
            releaseIf(old);
            if (++count == 8) return i + 8 == last ? 1 : 0;
        }
        j = i;
        i += 8;
        if (i == last) break;
    }
    return 1;
}

// ---------------------------------------------------------------- the guilt bonus animation
// The state lambda (0x50 bytes after the vtable): +0 3 words, +0xC shared_ptr A, +0x14 shared_ptr B (user data),
// +0x1C word (the guilt shown before), +0x20 node, +0x24 lwf node, +0x28 word, +0x2C Vector<Widget*>, +0x38 4 words,
// +0x48 2 words.
void guiltCopy(uint32_t dst, uint32_t src) {
    for (uint32_t k = 0; k < 0xC; k += 4) st32(dst + k, ld32(src + k));
    st32(dst + 0xC, ld32(src + 0xC));
    st32(dst + 0x10, ld32(src + 0x10));
    addRef(ld32(dst + 0x10));
    st32(dst + 0x14, ld32(src + 0x14));
    st32(dst + 0x18, ld32(src + 0x18));
    addRef(ld32(dst + 0x18));
    for (uint32_t k = 0x1C; k < 0x28; k += 4) st32(dst + k, ld32(src + k));
    st32(dst + 0x28, ld32(src + 0x28));
    call(g::WidgetVector_copy, {dst + 0x2C, src + 0x2C});
    for (uint32_t k = 0x38; k < 0x50; k += 4) st32(dst + k, ld32(src + k));
}
// move a Vector<Widget*>: the destination empty (deallocate), then the source's three words taken
void vectorMove(uint32_t dst, uint32_t src) {
    st32(dst, 0);
    st32(dst + 4, 0);
    st32(dst + 8, 0);
    call(g::WidgetVector_deallocate, {dst});
    st32(dst, ld32(src));
    st32(dst + 4, ld32(src + 4));
    st32(dst + 8, ld32(src + 8));
    st32(src, 0);
    st32(src + 4, 0);
    st32(src + 8, 0);
}

// the fusion's guilt bonus (closure {-, scene, word, Vector +0xC, 4 words +0x18, 2 words +0x28, node +0x30,
// 3 words +0x34, shared_ptr +0x40, shared_ptr +0x48, word +0x50}): GuiltBonus.lwf over the node with a skip
// button, whose end shows the bonus; without a bonus straight to the end
uint32_t guiltBonus(uint32_t self) {
    if (!ld8(ld32(self + 4) + F_GUILT_SHOW)) {
        call(g::GuiltFinish, {self + 8});
        return 0;
    }
    const uint32_t node = ld32(self + 0x30);
    const uint32_t lwfNode = call(g::AudioLWFNode_create, {S_GuiltBonusLwf, 0});
    {
        const uint32_t setPos = ld32(ld32(lwfNode) + node::setPosition);
        const float w = ldf(vcall(node, node::getContentSize, {node}));
        const float h = ldf(vcall(node, node::getContentSize, {node}) + 4);
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, fbits(w * 0.5f), fbits(h * 0.5f)});
        call(setPos, {lwfNode, pos});
        call(g::Vec2_dtor, {pos});
    }
    vcall(ld32(self + 0x30), V_ADD_CHILD_Z, {ld32(self + 0x30), lwfNode, 9999});
    Frame f;
    const uint32_t l1 = f.alloc(0x50), l2 = f.alloc(0x50);
    // l1 from the closure
    for (uint32_t k = 0; k < 0xC; k += 4) st32(l1 + k, ld32(self + 0x34 + k));
    st32(l1 + 0xC, ld32(self + 0x40));
    st32(l1 + 0x10, ld32(self + 0x44));
    addRef(ld32(l1 + 0x10));
    st32(l1 + 0x14, ld32(self + 0x48));
    st32(l1 + 0x18, ld32(self + 0x4C));
    addRef(ld32(l1 + 0x18));
    st32(l1 + 0x1C, ld32(self + 0x50));
    st32(l1 + 0x20, ld32(self + 0x30));
    st32(l1 + 0x24, lwfNode);
    st32(l1 + 0x28, ld32(self + 8));
    call(g::WidgetVector_copy, {l1 + 0x2C, self + 0xC});
    for (uint32_t k = 0; k < 0x10; k += 4) st32(l1 + 0x38 + k, ld32(self + 0x18 + k));
    st32(l1 + 0x48, ld32(self + 0x28));
    st32(l1 + 0x4C, ld32(self + 0x2C));
    guiltCopy(l2, l1);
    // the state into a heap __func: A moved, B copied, the Vector moved
    const uint32_t show = f.alloc(20);
    st32(show + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x54});
    if (h) {
        st32(h, VT_GuiltShow);
        for (uint32_t k = 0; k < 0xC; k += 4) st32(h + 4 + k, ld32(l2 + k));
        st32(h + 0x10, ld32(l2 + 0xC));
        st32(h + 0x14, ld32(l2 + 0x10));
        st32(l2 + 0xC, 0);
        st32(l2 + 0x10, 0);
        st32(h + 0x18, ld32(l2 + 0x14));
        st32(h + 0x1C, ld32(l2 + 0x18));
        addRef(ld32(h + 0x1C));
        for (uint32_t k = 0; k < 0xC; k += 4) st32(h + 0x20 + k, ld32(l2 + 0x1C + k));
        st32(h + 0x2C, ld32(l2 + 0x28));
        vectorMove(h + 0x30, l2 + 0x2C);
        for (uint32_t k = 0; k < 0x18; k += 4) st32(h + 0x3C + k, ld32(l2 + 0x38 + k));
    }
    st32(show + 16, h);
    // the skip button
    uint32_t button;
    {
        Frame t;
        const uint32_t s = t.alloc(12);
        st8(s, 0);
        st8(s + 1, 0);
        button = call(g::ImageButton_create, {s, 1});
        strFree(s);
    }
    call(g::ImageButton_setTargetView, {button, node, fbits(1.0f)});
    vcall(node, node::addChild, {node, button});
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_GuiltTap, {button, lwfNode});
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    // "_root" enterFrame {lwf node, the state}
    {
        const uint32_t lwf = ld32(lwfNode + LWFNODE_LWF);
        Frame t;
        const uint32_t name = t.alloc(12);
        rootName(name);
        const uint32_t l3 = t.alloc(0x18);
        st32(l3, lwfNode);
        functionCopy(l3 + 8, show);
        const uint32_t pair = t.alloc(40);
        enterFrameKey(pair);
        const uint32_t l4 = t.alloc(0x18);
        st32(l4, lwfNode);
        call(fn::function_vv_move, {l4 + 8, l3 + 8});
        st32(l4, lwfNode);
        const uint32_t fo = pair + 16;
        st32(fo + 16, 0);
        const uint32_t e = call(fn::operator_new, {0x28});
        if (e) {
            st32(e, VT_GuiltEnter);
            st32(e + 8, ld32(l4));
            call(fn::function_vv_move, {e + 0x10, l4 + 8});
            st32(e + 8, ld32(l4));
        }
        st32(fo + 16, e);
        functionDestroy(l4 + 8);
        addHandler(lwf, name, pair);
        functionDestroy(fo);
        strFree(pair);
        functionDestroy(l3 + 8);
        strFree(name);
    }
    functionDestroy(show);
    call(g::WidgetVector_dtor, {l2 + 0x2C});
    releaseIf(ld32(l2 + 0x18));
    releaseIf(ld32(l2 + 0x10));
    call(g::WidgetVector_dtor, {l1 + 0x2C});
    releaseIf(ld32(l1 + 0x18));
    releaseIf(ld32(l1 + 0x10));
    return 0;
}

// the guilt bonus state (vtable, then the 0x50 bytes above): the mix circle updated; the new guilt shown digit by
// digit in GuiltBonus_result (or GuiltBonus_max when it did not change), whose end finishes the chain
uint32_t guiltShow(uint32_t self) {
    {
        Frame f;
        const uint32_t a = ld32(self + 0xC), b = ld32(self + 8), c = ld32(self + 4);
        const uint32_t copy = f.alloc(8), flag = f.alloc(4);
        st32(copy, ld32(self + 0x10));
        st32(copy + 4, ld32(self + 0x14));
        addRef(ld32(copy + 4));
        const uint32_t sb = ld32(self + 0x18);
        call(g::SafeBuffer_read, {sb, flag, ld32(sb + 0xC) + 0x54, 1, 0x54});
        const uint32_t args = f.alloc(0x1C);
        st32(args, ld8(flag) ? 1 : 0);
        st32(args + 4, 0);
        st32(args + 8, 1);
        st32(args + 0xC, 0);
        st32(args + 0x10, 1);
        st32(args + 0x14, 0);
        st32(args + 0x18, 0);
        rw::call_exact(g::SceneUtil_updateMixCircle, {c, b, a, copy});
        releaseIf(ld32(copy + 4));
    }
    const uint32_t guilt = ld32(ld32(self + 0x10) + UM_GUILT);
    const uint32_t digits = guilt > 9999 ? 3 : 2;
    uint32_t value = guilt / 100;
    const bool max = ld32(self + 0x20) == value;
    const uint32_t lwfNode = centredLwf(max ? S_GuiltMaxLwf : S_GuiltResultLwf, ld32(self + 0x24));
    const uint32_t digitVt = max ? VT_MaxDigit : VT_ResultDigit;
    for (uint32_t i = 0; i < digits; i++) {
        const uint32_t lwf = ld32(lwfNode + LWFNODE_LWF);
        Frame f;
        const uint32_t buf = stringBuffer(f, {S_numD, i + 1});
        const uint32_t name = f.alloc(12);
        call(fn::string_init, {name, buf, call(g::STRLEN, {buf})});
        const uint32_t owner = ld32(self + 0x28);
        const uint32_t pair = f.alloc(40);
        loadKey(pair);
        const uint32_t fo = pair + 16;
        st32(fo + 16, 0);
        const uint32_t digit = value % 10;
        const uint32_t h = call(fn::operator_new, {0x14});
        if (h) {
            st32(h, digitVt);
            st32(h + 4, i);
            st32(h + 8, owner);
            st32(h + 0xC, lwfNode);
            st32(h + 0x10, digit);
        }
        st32(fo + 16, h);
        addHandler(lwf, name, pair);
        functionDestroy(fo);
        strFree(pair);
        strFree(name);
        value /= 10;
    }
    {
        const uint32_t lwf = ld32(lwfNode + LWFNODE_LWF);
        Frame f;
        const uint32_t name = f.alloc(12);
        rootName(name);
        // L {lwf node, word, Vector copy, 4 words, 2 words}; L2 its move; the heap __func from L2
        const uint32_t l = f.alloc(0x2C), l2 = f.alloc(0x2C);
        st32(l, lwfNode);
        st32(l + 4, ld32(self + 0x2C));
        call(g::WidgetVector_copy, {l + 8, self + 0x30});
        for (uint32_t k = 0; k < 0x18; k += 4) st32(l + 0x14 + k, ld32(self + 0x3C + k));
        const uint32_t pair = f.alloc(40);
        enterFrameKey(pair);
        st32(l2, ld32(l));
        st32(l2 + 4, ld32(l + 4));
        vectorMove(l2 + 8, l + 8);
        for (uint32_t k = 0; k < 0x18; k += 4) st32(l2 + 0x14 + k, ld32(l + 0x14 + k));
        st32(l2, ld32(l));
        const uint32_t fo = pair + 16;
        st32(fo + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x30});
        if (h) {
            st32(h, max ? VT_MaxEnter : VT_ResultEnter);
            st32(h + 4, ld32(l2));
            st32(h + 8, ld32(l2 + 4));
            vectorMove(h + 0xC, l2 + 8);
            for (uint32_t k = 0; k < 0x18; k += 4) st32(h + 0x18 + k, ld32(l2 + 0x14 + k));
            st32(h + 4, ld32(l2));
        }
        st32(fo + 16, h);
        call(g::WidgetVector_dtor, {l2 + 8});
        addHandler(lwf, name, pair);
        functionDestroy(fo);
        strFree(pair);
        call(g::WidgetVector_dtor, {l + 8});
        strFree(name);
    }
    return 0xA5C;
}

// the result's "_root" enterFrame {lwf node, the end's state}(Movie*): at the last frame (when shown) stop and
// hide it, finish the chain and drop the LWF's handlers
uint32_t guiltResultEnd(uint32_t self, uint32_t moviePtr) {
    const uint32_t movie = ld32(moviePtr);
    const uint32_t node = ld32(self + 4);
    const uint32_t visible = vcall(node, V_IS_VISIBLE, {node});
    if (visible != 1) return visible;
    const uint32_t total = ld32(movie + MOVIE_TOTAL_FRAMES);
    if (ld32(movie + MOVIE_CURRENT_FRAME) != total) return total;
    call(g::Node_pause, {ld32(self + 4)});
    vcall(ld32(self + 4), node::setVisible, {ld32(self + 4), 0});
    call(g::GuiltFinish, {self + 8});
    return call(g::LWF_ClearAllEventHandlers, {ld32(ld32(self + 4) + LWFNODE_LWF)});
}
}  // namespace

RW_FUNCTION(0x69CC54, "SceneMixedMedal::openSkillMixPopup(function<void(bool)>)", openSkillMixPopup);
RW_FUNCTION(0x69D1AC, "SceneMixedMedal::openChangeGuiltPopup(shared_ptr<UserMedalData>, function<void(bool)>)",
            openChangeGuiltPopup);
RW_FUNCTION(0x69FA18, "SceneMixedMedal back lambda (sub_69FA18)", backLayer);
RW_FUNCTION(0x6A0664, "std::__insertion_sort_incomplete (SceneMixedMedal materials) (sub_6A0664)", insertionSortIncomplete);
RW_FUNCTION(0x6A090C, "SceneMixedMedal material list reset (sub_6A090C)", materialListReset);
RW_FUNCTION(0x6A0B14, "SceneMixedMedal material added (sub_6A0B14)", materialAdded);
RW_FUNCTION(0x6A0C90, "SceneMixedMedal material drop target (sub_6A0C90)", dropTarget);
RW_FUNCTION(0x6A1136, "SceneMixedMedal material cell touch (sub_6A1136)", materialCellTouch);
RW_FUNCTION(0x6A1418, "SceneMixedMedal guilt bonus animation (sub_6A1418)", guiltBonus);
RW_FUNCTION(0x6A20D8, "SceneMixedMedal guilt bonus result (sub_6A20D8)", guiltShow);
RW_FUNCTION(0x6A2DE4, "SceneMixedMedal guilt result enterFrame end (sub_6A2DE4)", guiltResultEnd);
