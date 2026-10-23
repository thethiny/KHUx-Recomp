// SceneEvolveMedal leftovers: createScene, isDeckCostOverAfterEvolve, openEvolvePopup (the evolved medal's info:
// MedalInfoScene_ver106 with its buttons hidden and a screen-wide button that closes it), openRareWarnPopup (the
// rare material caution: one panel per kind of rare material, OK / Cancel through SceneManager::openPopup) and
// the screen's lambdas: the Evolve button's checks ($_ {3 flags, the material ids, 2 words, this}), the evolve
// request and its answer (the card_evolution_result LWF with one "drowMedalNNN" load handler per material, shown
// in a FlashMovieView), the popups' buttons and results, the back button, the info popup's reopen and the medal
// image load handler.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Scene_create = 0x8A5F37, SceneEvolveMedal_create = 0x65E1F9;
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09, AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SM_getPopupLayer = 0x613E69, SM_getPopupLayer2 = 0x613E71;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_changeSceneLayer = 0x612B99;
constexpr uint32_t SM_openPopup = 0x61524D, SM_updateHeader = 0x611169;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69, SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_updateMedalInfo = 0x462195;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;        // (const string&, const function<void()>&, bool)
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;  // (const string&, const function<void(bool)>&, ...)
constexpr uint32_t FadeTo_create = 0x86B3E1;
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t AEABI_MEMSET = 0x40E7A0, MEMCPY = 0x40E164, STRLEN = 0x40E08C;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t ArrayU64x5_at = 0x6B00A5;                      // std::array<ull,5>::at
constexpr uint32_t VectorSChar_pushSlow = 0x6A5CF5;
constexpr uint32_t UnorderedSetULL_copy = 0x4740A9, UnorderedSetULL_dtor = 0x482619;
constexpr uint32_t EvolveRequestLambda = 0x6B2499;               // builds the evolve request lambda from {set, 2 words}
constexpr uint32_t SceneEvolveMedal_isDeckCostOverAfterEvolve = 0x6AFE01;
constexpr uint32_t SceneEvolveMedal_openRareWarnPopup = 0x6B0119, SceneEvolveMedal_openEvolvePopup = 0x6AF549;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_executeUserMedalEvolve = 0x4CA331;  // (api, -, u64 base, set&, function<void(bool)>)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t DelayTime_create = 0x86852D, CallFunc_create = 0x866B39, Sequence_create = 0x867999;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5, FlashMovieView_create = 0x5682A1;
constexpr uint32_t HandlerMap_insert_unique = 0x530869, HandlerMap_destroy = 0x530811;
constexpr uint32_t LWF_AddMovieEventHandler = 0x808645;
constexpr uint32_t function_vb_call = 0x474139, function_vv_assign = 0x4EB9CD;
constexpr uint32_t GOT_VEC2_ZERO = 0xFD605C, GOT_LINE_BREAK_TAG = 0xFD6930;
}  // namespace g

// strings
constexpr uint32_t S_MedalInfoScene = 0xDF8CF7;   // "cocostudio/publish/MedalInfoScene_ver106.json"
constexpr uint32_t S_MedalInfo = 0xDF8D25, S_TapJson = 0xDFFCF2, S_TapAnimation = 0xDFFB2B;
constexpr uint32_t S_Animation1 = 0xDFD0AF, S_Empty = 0xDF8A1A, S_CenterButton = 0xDFFB40;
constexpr uint32_t S_Favorite = 0xDF8D3C, S_Support = 0xDF8D71, S_Close_Button = 0xDF8D2F;
constexpr uint32_t S_Button_Mix = 0xDF87D4, S_Button_LimitCut = 0xDF8D54, S_Button_OK = 0xDF8688;
constexpr uint32_t S_RareJson = 0xE05046;         // "cocostudio/publish/PopupNormal_MedalMix_RareCaution.json"
constexpr uint32_t S_RarePanelJson = 0xE0509E;    // ".._RareCaution_Panel.json"
constexpr uint32_t S_Window03 = 0xE0507F, S_Button_Close = 0xDF8692, S_Txt_Wording1 = 0xDFF14C;
constexpr uint32_t S_Txt_Wording2 = 0xDF863B, S_Txt_Wording3 = 0xE0014B, S_Star1 = 0xE050DD;
constexpr uint32_t S_Alert_Area = 0xE05093, S_Txt_OK = 0xDF869F, S_Txt_Cancel = 0xDF86A6;
constexpr uint32_t S_EvolutionLwf = 0xE054F1;     // "lwf/medal/compose/evolution/card_evolution_result_%02d/...lwf"
constexpr uint32_t S_drowMedal001 = 0xE052EA, S_drowMedalN = 0xE04B36;
constexpr uint32_t S_MedalLwf = 0xE052F7;         // "mixture/medal/compose/%d/Medal_L/Medal_L.lwf"
constexpr int32_t TXT_EVOLVE_CONFIRM = 101400003, TXT_COST_OVER = 101200030;
constexpr int32_t TXT_NOT_ENOUGH = 101400004, TXT_LEVEL = 101400005, TXT_MUNNY = 100300007;
constexpr int32_t TXT_RARE_1 = 101300032, TXT_RARE_2 = 101300029, TXT_RARE_3 = 101300030;
constexpr int32_t TXT_OK = 100100001, TXT_CANCEL = 100100002;

// __func vtables (+8)
constexpr uint32_t VT_InfoClose = 0xF661AC;       // openEvolvePopup's button {node, layer, shadow, this}
constexpr uint32_t VT_RareOk = 0xF66128, VT_RareClose = 0xF660FC, VT_RareCanClose = 0xF660D0;
constexpr uint32_t VT_RareResult = 0xF66078;      // {ok, close, window, layer, shadow, popup, function<void(bool)>}
constexpr uint32_t VT_RareClosed = 0xF660A4;      // the close animation's end {layer, shadow, popup}
constexpr uint32_t VT_InfoClosed = 0xF661D8;      // openEvolvePopup's close animation end (0x18)
constexpr uint32_t VT_BackOut = 0xF6625C;         // the back button's run-out {this}
constexpr uint32_t VT_BackLayer = 0xF66288;       // backScene's layer function (0x18)
constexpr uint32_t VT_EvolveAnswer = 0xF6630C;    // the evolve request's answer {2 words}
constexpr uint32_t VT_EvolveRetry = 0xF663E8;
constexpr uint32_t VT_ResultLoad = 0xF663BC;      // "drowMedal001" load {lwf node, this}
constexpr uint32_t VT_MaterialLoad = 0xF66390;    // "drowMedalNNN" load {id lo, id hi, lwf node} (0x18)
constexpr uint32_t VT_ResultEnd = 0xF66338;       // the FlashMovieView's end {layer, view, arg, lwf node}
constexpr uint32_t VT_ReopenInfo = 0xF66364;      // CallFunc {3 words}

// SceneEvolveMedal
constexpr uint32_t F_INOUT = 0x288, F_MEDAL = 0x2A0, F_IDS_A = 0x2A8, F_FROM = 0x2CC, F_DETAIL = 0x2D8;
constexpr uint32_t F_RARE = 0x2E0;                // 3 flags: rare materials of kind 1..3
// SceneManager
constexpr uint32_t SM_SELECTED_MEDAL = 0x2A0;
// UserMedalData / master::Medal
constexpr uint32_t UM_MASTER = 0x58, M_COST = 0x484, M_EVOLVE_TO = 0x4DC, M_IMAGE = 4;
constexpr uint32_t V_SET_TOUCH_ENABLED = 0x29C, V_ADD_CHILD_Z = 0xF4, V_ADD_CHILD_Z_NAME = 0xFC;
constexpr uint32_t V_GET_ANIMATION = 0x298, V_PLAY = 0x68, V_ATTACH_LWF = 0x2F0, V_REMOVE_CHILD = 0x12C;
constexpr uint32_t V_SET_POSITION_X = 0x64, V_GET_POSITION_X = 0x68, V_SET_POSITION_Y = 0x6C;
constexpr uint32_t V_GET_POSITION_Y = 0x70;
constexpr uint32_t LWFNODE_LWF = 0x3E0;
constexpr uint32_t FLASH_END = 0x278, FLASH_END_SET = 0x274;

uint32_t api() { return call(fn::APIManager_getInstance, {}); }
void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void hide(uint32_t root, uint32_t name, uint32_t len) {
    const uint32_t n = findNode(root, name, len);
    vcall(n, node::setVisible, {n, 0});
}
// a user medal record's medal data: getMedalData(out, mgr, id, medalId, an empty UserMedal::Medal by value).
// The record's word at +0x2C is never set by the original (stale stack); 0 here.
void medalData(uint32_t out, uint32_t mgr, uint32_t lo, uint32_t hi, uint32_t medalId) {
    Frame f;
    const uint32_t record = f.alloc(0x48);
    call(g::AEABI_MEMSET, {record, 0x2C, 0});
    call(g::AEABI_MEMSET, {record + 0x30, 0x18, 0});
    const uint32_t args = f.alloc(0x50);
    st32(args, medalId);
    for (uint32_t k = 0; k < 0x48; k += 4) st32(args + 8 + k, ld32(record + k));
    rw::call_exact(g::MedalDataManager_getMedalData, {out, mgr, lo, hi});
}
uint32_t readU32(uint32_t sb, uint32_t offset, uint32_t out) {
    call(g::SafeBuffer_read, {sb, out, ld32(sb + 0xC) + offset, 4, offset});
    return ld32(out);
}
// a label: setShadow, then a TextData text
void label(uint32_t root, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(root, name, len);
    call(g::SceneUtil_setShadow, {text, 1});
    setTextId(text, id);
}
// the std::string at `pair` = "load" (short, built in place)
void loadKey(uint32_t pair) {
    st8(pair, 8);
    st8(pair + 1, 'l');
    st8(pair + 2, 'o');
    st8(pair + 3, 'a');
    st8(pair + 4, 'd');
    st8(pair + 5, 0);
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

uint32_t createScene() {
    const uint32_t scene = call(g::Scene_create, {});
    const uint32_t layer = call(g::SceneEvolveMedal_create, {});
    vcall(scene, node::addChild, {scene, layer});
    return scene;
}

// Would evolving the selected medal push a deck that holds it over the player's cost limit? For each deck that
// holds it: (the deck's cost) - (its cost) + (the evolved medal's cost) > limit.
uint32_t isDeckCostOverAfterEvolve(uint32_t self) {
    Frame f;
    const uint32_t tmp = f.alloc(4), deck = f.alloc(0x58), medal = f.alloc(8), evolved = f.alloc(8);
    const uint32_t user = ld32(api() + 0x94);
    const int32_t limit = int32_t(readU32(user, 0x1C, tmp));
    const uint32_t decksAt = api() + 0xEC;
    uint32_t result = 0;
    if (!ld32(ld32(decksAt) + 0x20)) return 0;
    uint32_t i = 0;
    do {
        const uint32_t decks = ld32(decksAt);
        call(g::SafeBuffer_read, {decks, deck, ld32(decks + 0xC) + 0x58 * i, 0x58, 0x58 * i});
        if (ld32(deck + 0x10)) {
            uint32_t found = 0;
            int32_t sum = 0;
            uint32_t j = 0;
            do {
                const uint32_t id = call(g::ArrayU64x5_at, {deck + 0x18, j});
                const uint32_t lo = ld32(id), hi = ld32(id + 4);
                medalData(medal, call(g::MedalDataManager_getInstance, {}), lo, hi, 0);
                const uint32_t m = ld32(medal);
                const int32_t cost = int32_t(readU32(ld32(m + UM_MASTER), M_COST, tmp));
                const uint32_t target = ld32(self + F_MEDAL);
                if (ld32(m) == ld32(target) && ld32(m + 4) == ld32(target + 4)) found = 1;
                sum = sum + cost - int32_t(ld32(m + 0x28));
                releaseIf(ld32(medal + 4));
                j++;
            } while (j < ld32(deck + 0x10));
            if (found & 1) {
                const uint32_t mgr = call(g::MedalDataManager_getInstance, {});
                const uint32_t target = ld32(self + F_MEDAL);
                const uint32_t hi = ld32(target + 4), lo = ld32(target);
                const uint32_t to = readU32(ld32(target + UM_MASTER), M_EVOLVE_TO, tmp);
                medalData(evolved, mgr, lo, hi, to);
                const int32_t newCost = int32_t(readU32(ld32(ld32(evolved) + UM_MASTER), M_COST, tmp));
                const int32_t oldCost = int32_t(readU32(ld32(ld32(self + F_MEDAL) + UM_MASTER), M_COST, tmp));
                const int32_t total = newCost + sum - oldCost;
                result |= limit < total ? 1 : 0;
                releaseIf(ld32(evolved + 4));
                if (limit < total) return result & 1;
            }
        }
        i++;
    } while (i < ld32(ld32(decksAt) + 0x20));
    return result & 1;
}

// the medal info popup for the evolved medal (popup layer 1)
uint32_t openEvolvePopup(uint32_t self, uint32_t medal) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    uint32_t node;
    {
        Frame f;
        const uint32_t s = newStr(f, S_MedalInfoScene, 0x2D);
        node = call(g::SceneUtil_createNodeWithSceneFile, {s});
        strFree(s);
    }
    vcall(layer, V_ADD_CHILD_Z, {layer, node, 0});
    const uint32_t info = findNode(findNode(node, S_MedalInfo, 9), S_MedalInfo, 9);
    vcall(info, node::setAnchorPoint, {info, ld32(g::GOT_VEC2_ZERO)});
    {
        const uint32_t a = api();
        Frame f;
        const uint32_t copy = f.alloc(8), mine = f.alloc(8), flag = f.alloc(4);
        st32(copy, ld32(medal));
        st32(copy + 4, ld32(medal + 4));
        if (ld32(copy + 4)) call(fn::shared_add, {ld32(copy + 4)});
        const uint32_t user = ld32(a + 0x94);
        call(g::SafeBuffer_read, {user, flag, ld32(user + 0xC) + 0x54, 1, 0x54});
        st32(mine, ld32(self + F_MEDAL));
        st32(mine + 4, ld32(self + F_MEDAL + 4));
        if (ld32(mine + 4)) call(fn::shared_add, {ld32(mine + 4)});
        const uint32_t args = f.alloc(8);
        st32(args, mine);
        st32(args + 4, 0);
        rw::call_exact(g::SceneUtil_updateMedalInfo, {node, copy, ld8(flag) ? 1u : 0u, 0});
        releaseIf(ld32(mine + 4));
        releaseIf(ld32(copy + 4));
    }
    {
        const uint32_t armatures = call(g::ArmatureDataManager_getInstance, {});
        Frame f;
        const uint32_t s = newStr(f, S_TapJson, 0x32);
        call(g::ArmatureDataManager_addArmatureFileInfo, {armatures, s});
        strFree(s);
    }
    uint32_t armature;
    {
        Frame f;
        const uint32_t s = newStr(f, S_TapAnimation, 0x14);
        armature = call(g::Armature_create, {s});
        strFree(s);
    }
    {
        const uint32_t setPos = ld32(ld32(armature) + node::setPosition);
        const float w = ldf(vcall(node, node::getContentSize, {node}));
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, fbits(w * 0.5f), fbits(30.0f)});
        call(setPos, {armature, pos});
        call(g::Vec2_dtor, {pos});
    }
    vcall(node, V_ADD_CHILD_Z, {node, armature, 9999});
    {
        const uint32_t animation = vcall(armature, V_GET_ANIMATION, {armature});
        Frame f;
        const uint32_t s = newStr(f, S_Animation1, 10);
        vcall(animation, V_PLAY, {animation, s, 0xFFFFFFFF, 0xFFFFFFFF});
        strFree(s);
    }
    uint32_t button;
    {
        Frame f;
        const uint32_t s = newStr(f, S_Empty, 0);
        button = call(g::ImageButton_create, {s, 1});
        strFree(s);
    }
    call(g::ImageButton_setTargetView, {button, node, fbits(1.0f)});
    {
        Frame f;
        const uint32_t s = newStr(f, S_CenterButton, 12);
        vcall(node, V_ADD_CHILD_Z_NAME, {node, button, 0, s});
        strFree(s);
    }
    {
        Frame f;
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x14});
        if (h) {
            st32(h, VT_InfoClose);
            st32(h + 4, node);
            st32(h + 8, layer);
            st32(h + 0xC, shadow);
            st32(h + 0x10, self);
        }
        st32(fo + 16, h);
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    hide(node, S_Favorite, 0x11);
    hide(node, S_Support, 0x10);
    hide(node, S_Close_Button, 12);
    hide(node, S_Button_Mix, 10);
    hide(node, S_Button_LimitCut, 15);
    hide(node, S_Button_OK, 9);
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(fn::Node_runAction, {node, call(g::SceneUtil_createOpenPopupAnimation, {node, none})});
        functionDestroy(none);
    }
    return 0;
}

// the rare material caution; `done` gets OK / Cancel
uint32_t openRareWarnPopup(uint32_t self, uint32_t done) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_RareJson});
    {
        const uint32_t setX = ld32(ld32(popup) + V_SET_POSITION_X);
        const float x = bitsf(vcall(popup, V_GET_POSITION_X, {popup}));
        call(setX, {popup, fbits(x + bitsf(call(g::SceneUtil_getSideMargin, {})))});
        const uint32_t setY = ld32(ld32(popup) + V_SET_POSITION_Y);
        const float y = bitsf(vcall(popup, V_GET_POSITION_Y, {popup}));
        call(setY, {popup, fbits(y + bitsf(call(g::SceneUtil_getSideHeightMargin, {})))});
    }
    vcall(layer, node::addChild, {layer, popup});
    const uint32_t window = findNode(popup, S_Window03, 0x13);
    const uint32_t ok = findNode(popup, S_Button_OK, 9);
    const uint32_t close = findNode(popup, S_Button_Close, 12);
    Frame f;
    // the result lambda's captures {ok, close, window, layer, shadow, popup} and a copy of done
    const uint32_t lam = f.alloc(0x2C);
    st32(lam, ok);
    st32(lam + 4, close);
    st32(lam + 8, window);
    st32(lam + 0xC, layer);
    st32(lam + 0x10, shadow);
    st32(lam + 0x14, popup);
    call(fn::function_vb_copy, {lam + 0x18, done});
    label(popup, S_Txt_Wording3, 12, TXT_EVOLVE_CONFIRM);
    // the kinds of rare material (vector<signed char>)
    const uint32_t kinds = f.alloc(12), kind = f.alloc(4);
    st32(kinds, 0);
    st32(kinds + 4, 0);
    st32(kinds + 8, 0);
    for (uint32_t k = 0; k < 3; k++) {
        if (!ld8(self + F_RARE + k)) continue;
        st8(kind + 3 - k, uint8_t(k + 1));
        const uint32_t end = ld32(kinds + 4);
        if (k == 0 || end >= ld32(kinds + 8)) {
            call(g::VectorSChar_pushSlow, {kinds, kind + 3 - k});
        } else {
            if (end) st8(end, uint8_t(k + 1));
            st32(kinds + 4, (end ? ld32(kinds + 4) : 0) + 1);
        }
    }
    const uint32_t area = findNode(popup, S_Alert_Area, 10);
    for (uint32_t i = 0; i < ld32(kinds + 4) - ld32(kinds); i++) {
        const uint32_t k = ld8(ld32(kinds) + i);
        const uint32_t panel =
            call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_RarePanelJson});
        hide(panel, S_Txt_Wording1, 12);
        hide(panel, S_Star1, 5);
        hide(panel, S_Txt_Wording2, 12);
        const uint32_t text = findNode(panel, S_Txt_Wording3, 12);
        vcall(text, node::setVisible, {text, 0});
        if (k >= 1 && k <= 3) {
            vcall(text, node::setVisible, {text, 1});
            call(g::SceneUtil_setShadow, {text, 1});
            setTextId(text, k == 1 ? TXT_RARE_1 : k == 2 ? TXT_RARE_2 : TXT_RARE_3);
        }
        const float h = ldf(vcall(panel, node::getContentSize, {panel}) + 4);
        const float total = float(ld32(kinds + 4) - ld32(kinds)) * ldf(vcall(panel, node::getContentSize, {panel}) + 4);
        const uint32_t setY = ld32(ld32(panel) + V_SET_POSITION_Y);
        const float areaH = ldf(vcall(area, node::getContentSize, {area}) + 4);
        const float mid = total + (areaH - total) * 0.5f;
        call(setY, {panel, fbits(mid - float(int32_t(i + 1)) * h)});
        vcall(area, node::addChild, {area, panel});
    }
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_RareOk, {});
        call(g::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
    }
    label(ok, S_Txt_OK, 6, TXT_OK);
    {
        Frame t;
        const uint32_t fo = newFunction(t, VT_RareClose, {});
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
        const uint32_t check = newFunction(t, VT_RareCanClose, {close});
        const uint32_t lam2 = t.alloc(0x2C);
        for (uint32_t k = 0; k < 0x18; k += 4) st32(lam2 + k, ld32(lam + k));
        functionCopy(lam2 + 0x18, lam + 0x18);
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x38});
        if (h) {
            st32(h, VT_RareResult);
            for (uint32_t k = 0; k < 0x18; k += 4) st32(h + 8 + k, ld32(lam2 + k));
            call(fn::function_vb_move, {h + 0x20, lam2 + 0x18});
        }
        st32(result + 16, h);
        call(g::SM_openPopup, {sm, check, result, 1});
        functionDestroy(result);
        functionDestroy(lam2 + 0x18);
        functionDestroy(check);
    }
    if (ld32(kinds)) {
        if (ld32(kinds + 4) != ld32(kinds)) st32(kinds + 4, ld32(kinds));
        call(fn::operator_delete, {ld32(kinds)});
    }
    functionDestroy(lam + 0x18);
    return 0;
}

// ---------------------------------------------------------------- lambdas
// openRareWarnPopup's result {vt, -, ok, close, window, layer, shadow, popup, function<void(bool)>}(bool)
uint32_t rareResult(uint32_t self, uint32_t okp) {
    const uint32_t ok = ld8(okp);
    vcall(ld32(self + 8), V_SET_TOUCH_ENABLED, {ld32(self + 8), 0});
    vcall(ld32(self + 0xC), V_SET_TOUCH_ENABLED, {ld32(self + 0xC), 0});
    call(ok ? g::AudioPlaySystemSEDecide : g::AudioPlaySystemSECancel, {});
    const uint32_t window = ld32(self + 0x10);
    {
        Frame f;
        const uint32_t fo = newFunction(f, VT_RareClosed, {ld32(self + 0x14), ld32(self + 0x18), ld32(self + 0x1C)});
        call(fn::Node_runAction, {window, call(g::SceneUtil_createClosePopupAnimation, {window, fo})});
        functionDestroy(fo);
    }
    const uint32_t shadow = ld32(self + 0x18);
    call(fn::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), 0})});
    if (ld32(self + 0x30)) call(g::function_vb_call, {self + 0x20, ok ? 1u : 0u});
    return 0;
}

// openEvolvePopup's button {node, layer, shadow, this}(sender, type): ended -> close it
uint32_t infoClose(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    call(g::AudioPlaySystemSEDecide, {});
    const uint32_t node = ld32(self + 4), layer = ld32(self + 8), shadow = ld32(self + 0xC);
    const uint32_t scene = ld32(self + 0x10);
    Frame f;
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x18});
    if (h) {
        st32(h, VT_InfoClosed);
        st32(h + 4, layer);
        st32(h + 8, shadow);
        st32(h + 0xC, node);
        st32(h + 0x10, 1);
        st32(h + 0x14, scene);
    }
    st32(fo + 16, h);
    call(fn::Node_runAction, {node, call(g::SceneUtil_createClosePopupAnimation, {node, fo})});
    functionDestroy(fo);
    call(fn::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(0.1f), 0})});
    return 0;
}

// the back button {this}(sender, type): ended -> run out, then backScene
uint32_t backButton(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    const uint32_t me = ld32(self + 4);
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_BackOut, {me});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
    return 0;
}

// backScene's {this}: the layer to go back to ({from, 0, detail argument})
uint32_t backScene(uint32_t self) {
    const uint32_t me = ld32(self + 4);
    const uint32_t hi = ld32(me + F_DETAIL + 4), lo = ld32(me + F_DETAIL), from = ld32(me + F_FROM);
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t h = call(fn::operator_new, {0x18});
    if (h) {
        st32(h, VT_BackLayer);
        st32(h + 8, from);
        st32(h + 0xC, 0);
        st32(h + 0x10, lo);
        st32(h + 0x14, hi);
    }
    st32(fo + 16, h);
    call(g::SM_changeSceneLayer, {sm, fo, 1, 0, 0});
    functionDestroy(fo);
    return 0;
}

// the Evolve button {3 flags (enough level, munny, materials), unordered_set<ull> ids, 2 words, this}
uint32_t evolveButton(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return 0;
    const uint32_t me = ld32(self + 0x24);
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    if (ld8(self + 4) && ld8(self + 5) && ld8(self + 6)) {
        const uint32_t ids = f.alloc(0x1C);   // the set and the 2 words
        call(g::UnorderedSetULL_copy, {ids, self + 8});
        st32(ids + 0x14, ld32(self + 0x1C));
        st32(ids + 0x18, ld32(self + 0x20));
        // confirm with a dialog (or the rare warning) whose OK sends the request
        auto confirm = [&](int32_t textId) {
            Frame t;
            const uint32_t text = textData(t, textId);
            const uint32_t s = newStr(t, textDataStr(text));
            const uint32_t copy = t.alloc(0x1C);
            call(g::UnorderedSetULL_copy, {copy, ids});
            st32(copy + 0x14, ld32(ids + 0x14));
            st32(copy + 0x18, ld32(ids + 0x18));
            const uint32_t fo = t.alloc(20);
            call(g::EvolveRequestLambda, {fo, copy});
            call(g::SceneUtil_openOkCancelDialog4Line, {s, fo, 0, 0});
            functionDestroy(fo);
            call(g::UnorderedSetULL_dtor, {copy});
            strFree(s);
            releaseIf(ld32(text + 4));
        };
        if (call(g::SceneEvolveMedal_isDeckCostOverAfterEvolve, {me}) == 1) {
            confirm(TXT_COST_OVER);
        } else if (ld8(me + F_RARE) || ld8(me + F_RARE + 1) || ld8(me + F_RARE + 2)) {
            Frame t;
            const uint32_t copy = t.alloc(0x1C);
            call(g::UnorderedSetULL_copy, {copy, ids});
            st32(copy + 0x14, ld32(ids + 0x14));
            st32(copy + 0x18, ld32(ids + 0x18));
            const uint32_t fo = t.alloc(20);
            call(g::EvolveRequestLambda, {fo, copy});
            call(g::SceneEvolveMedal_openRareWarnPopup, {me, fo});
            functionDestroy(fo);
            call(g::UnorderedSetULL_dtor, {copy});
        } else {
            confirm(TXT_EVOLVE_CONFIRM);
        }
        call(g::UnorderedSetULL_dtor, {ids});
        return 0;
    }
    // what is missing, one line each
    const uint32_t msg = f.alloc(12);
    st32(msg, 0);
    st32(msg + 4, 0);
    st32(msg + 8, 0);
    auto append = [&](uint32_t text) { call(fn::string_append, {msg, text, call(g::STRLEN, {text})}); };
    auto appendText = [&](int32_t id) {
        Frame t;
        const uint32_t text = textData(t, id);
        append(textDataStr(text));
        releaseIf(ld32(text + 4));
    };
    bool first = true;
    if (!ld8(self + 4)) {
        appendText(TXT_NOT_ENOUGH);
        first = false;
    }
    if (!ld8(self + 5)) {
        if (!first) append(ld32(ld32(g::GOT_LINE_BREAK_TAG)));
        first = false;
        appendText(TXT_LEVEL);
    }
    if (!ld8(self + 6)) {
        if (!first) append(ld32(ld32(g::GOT_LINE_BREAK_TAG)));
        appendText(TXT_MUNNY);
    }
    {
        Frame t;
        const uint32_t none = newEmptyFunction(t);
        call(g::SceneUtil_openOkDialog4Line, {msg, none, 0});
        functionDestroy(none);
    }
    strFree(msg);
    return 0;
}

// the evolve request {set (+4), 2 words (+0x18)}(bool ok): send it with the selected medal
uint32_t evolveRequest(uint32_t self, uint32_t okp) {
    if (!ld8(okp)) return 0;
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t a = api();
    const uint32_t sm = sceneManager();
    const uint32_t hi = ld32(sm + SM_SELECTED_MEDAL + 4), lo = ld32(sm + SM_SELECTED_MEDAL);
    Frame f;
    const uint32_t fo = newFunction(f, VT_EvolveAnswer, {ld32(self + 0x18), ld32(self + 0x1C)});
    const uint32_t args = f.alloc(8);
    st32(args, self + 4);
    st32(args + 4, fo);
    rw::call_exact(g::APIManager_executeUserMedalEvolve, {a, args, lo, hi});
    functionDestroy(fo);
    return 0;
}

// the evolve answer {this, arg}(bool ok): the evolution result LWF (one card per material) in a FlashMovieView
uint32_t evolveAnswer(uint32_t self, uint32_t okp) {
    const uint32_t ok = ld8(okp);
    const uint32_t me = ld32(self + 4);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    if (!ok) {
        if (ld8(api() + 0x14)) {
            const uint32_t a = api();
            Frame f;
            const uint32_t fo = newFunction(f, VT_EvolveRetry, {});
            call(g::APIManager_openErrorDialog, {a, fo, 1});
            functionDestroy(fo);
        }
        return 0x934;
    }
    uint32_t lwfNode;
    {
        Frame f;
        const uint32_t n = (ld32(me + F_IDS_A + 4) - ld32(me + F_IDS_A)) >> 3;
        const uint32_t path = stringBuffer(f, {S_EvolutionLwf, n, n});
        lwfNode = call(g::AudioLWFNode_create, {path, 0});
    }
    {
        const uint32_t setPos = ld32(ld32(lwfNode) + node::setPosition);
        const uint32_t h = ld32(vcall(me, node::getContentSize, {me}) + 4);
        Frame f;
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctor, {pos, 0, h});
        call(setPos, {lwfNode, pos});
        call(g::Vec2_dtor, {pos});
    }
    {
        // "drowMedal001": the result card itself
        const uint32_t lwf = ld32(lwfNode + LWFNODE_LWF);
        Frame f;
        const uint32_t name = f.alloc(12);
        const uint32_t text = call(fn::operator_new, {0x10});
        st32(name, 0x11);
        st32(name + 4, 12);
        st32(name + 8, text);
        call(g::MEMCPY, {text, S_drowMedal001, 12});
        st8(text + 12, 0);
        const uint32_t pair = f.alloc(40);
        loadKey(pair);
        const uint32_t fo = pair + 16;
        st32(fo, VT_ResultLoad);
        st32(fo + 4, lwfNode);
        st32(fo + 8, me);
        st32(fo + 16, fo);
        addHandler(lwf, name, pair);
        functionDestroy(fo);
        strFree(pair);
        strFree(name);
    }
    if (ld32(me + F_IDS_A + 4) != ld32(me + F_IDS_A)) {
        uint32_t i = 0;
        do {
            const uint32_t e = ld32(me + F_IDS_A) + 8 * i;
            const uint32_t lo = ld32(e), hi = ld32(e + 4);
            const uint32_t lwf = ld32(lwfNode + LWFNODE_LWF);
            Frame f;
            const uint32_t buf = stringBuffer(f, {S_drowMedalN, i + 2});
            const uint32_t name = f.alloc(12);
            call(fn::string_init, {name, buf, call(g::STRLEN, {buf})});
            const uint32_t pair = f.alloc(40);
            loadKey(pair);
            const uint32_t fo = pair + 16;
            st32(fo + 16, 0);
            const uint32_t h = call(fn::operator_new, {0x18});
            if (h) {
                st32(h, VT_MaterialLoad);
                st32(h + 8, lo);
                st32(h + 0xC, hi);
                st32(h + 0x10, lwfNode);
            }
            st32(fo + 16, h);
            addHandler(lwf, name, pair);
            functionDestroy(fo);
            strFree(pair);
            strFree(name);
            i++;
        } while (i < (ld32(me + F_IDS_A + 4) - ld32(me + F_IDS_A)) >> 3);
    }
    const uint32_t layer = call(g::SM_getPopupLayer2, {sceneManager()});
    const uint32_t view = call(g::FlashMovieView_create, {lwfNode, 0});
    vcall(layer, node::addChild, {layer, view});
    const uint32_t arg = ld32(self + 8);
    {
        Frame f;
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t h = call(fn::operator_new, {0x14});
        if (h) {
            st32(h, VT_ResultEnd);
            st32(h + 4, layer);
            st32(h + 8, view);
            st32(h + 0xC, arg);
            st32(h + 0x10, lwfNode);
        }
        st32(fo + 16, h);
        call(g::function_vv_assign, {view + FLASH_END, fo});
        functionDestroy(fo);
    }
    st8(view + FLASH_END_SET, 1);
    return 0x934;
}

// {3 words, node}: after one frame (the animation interval), the CallFunc {3 words}
uint32_t delayedReopen(uint32_t self) {
    const uint32_t a = ld32(self + 4), b = ld32(self + 8), c = ld32(self + 0xC);
    const uint32_t d = call(g::Director_getInstance, {});
    double interval;
    std::memcpy(&interval, rw::host_ptr<uint8_t>(d + 0xA8), 8);
    const uint32_t node = ld32(self + 0x10);
    const uint32_t delay = call(g::DelayTime_create, {fbits(float(interval))});
    Frame f;
    const uint32_t fo = newFunction(f, VT_ReopenInfo, {a, b, c});
    const uint32_t cf = call(g::CallFunc_create, {fo});
    call(fn::Node_runAction, {node, call(g::Sequence_create, {delay, cf, 0})});
    functionDestroy(fo);
    return 0;
}

// {parent, child, scene}: remove the child, refresh the header and reopen the info popup on the selected medal
uint32_t reopenInfo(uint32_t self) {
    vcall(ld32(self + 4), V_REMOVE_CHILD, {ld32(self + 4), ld32(self + 8), 1});
    const uint32_t scene = ld32(self + 0xC);
    call(g::SM_updateHeader, {sceneManager()});
    const uint32_t mgr = call(g::MedalDataManager_getInstance, {});
    const uint32_t sm = sceneManager();
    const uint32_t hi = ld32(sm + SM_SELECTED_MEDAL + 4), lo = ld32(sm + SM_SELECTED_MEDAL);
    Frame f;
    const uint32_t medal = f.alloc(8), copy = f.alloc(8);
    medalData(medal, mgr, lo, hi, 0);
    st32(copy, ld32(medal));
    const uint32_t ctl = ld32(medal + 4);
    st32(copy + 4, ctl);
    if (ctl) call(fn::shared_add, {ctl});
    call(g::SceneEvolveMedal_openEvolvePopup, {scene, copy});
    releaseIf(ctl);
    releaseIf(ld32(medal + 4));
    return 0;
}

// "drowMedalNNN" load {id lo, id hi, lwf node}(Movie*): the material medal's Medal_L LWF
uint32_t materialLoad(uint32_t self, uint32_t moviePtr) {
    const uint32_t movie = ld32(moviePtr);
    const uint32_t mgr = call(g::MedalDataManager_getInstance, {});
    Frame f;
    const uint32_t medal = f.alloc(8), image = f.alloc(4), ret = f.alloc(8);
    medalData(medal, mgr, 0, 0, ld32(self + 8));
    const uint32_t node = ld32(self + 0x10);
    const uint32_t attach = ld32(ld32(node) + V_ATTACH_LWF);
    const uint32_t id = readU32(ld32(ld32(medal) + UM_MASTER), M_IMAGE, image);
    const uint32_t buf = stringBuffer(f, {S_MedalLwf, id});
    const uint32_t none = newEmptyFunction(f);
    call(attach, {ret, node, buf, movie, S_Empty, none});
    releaseIf(ld32(ret + 4));
    functionDestroy(none);
    releaseIf(ld32(medal + 4));
    return 0;
}
}  // namespace

RW_FUNCTION(0x6AA300, "SceneEvolveMedal::createScene()", createScene);
RW_FUNCTION(0x6AFE00, "SceneEvolveMedal::isDeckCostOverAfterEvolve()", isDeckCostOverAfterEvolve);
RW_FUNCTION(0x6AF548, "SceneEvolveMedal::openEvolvePopup(shared_ptr<UserMedalData>)", openEvolvePopup);
RW_FUNCTION(0x6B0118, "SceneEvolveMedal::openRareWarnPopup(function<void(bool)>)", openRareWarnPopup);
RW_FUNCTION(0x6B0F1C, "SceneEvolveMedal::openRareWarnPopup result (sub_6B0F1C)", rareResult);
RW_FUNCTION(0x6B1738, "SceneEvolveMedal::openEvolvePopup close button (sub_6B1738)", infoClose);
RW_FUNCTION(0x6B1B38, "SceneEvolveMedal back button (sub_6B1B38)", backButton);
RW_FUNCTION(0x6B1C78, "SceneEvolveMedal::backScene lambda (sub_6B1C78)", backScene);
RW_FUNCTION(0x6B1FA4, "SceneEvolveMedal Evolve button (sub_6B1FA4)", evolveButton);
RW_FUNCTION(0x6B25E4, "SceneEvolveMedal evolve request (sub_6B25E4)", evolveRequest);
RW_FUNCTION(0x6B2768, "SceneEvolveMedal evolve answer (sub_6B2768)", evolveAnswer);
RW_FUNCTION(0x6B2D8C, "SceneEvolveMedal delayed info popup (sub_6B2D8C)", delayedReopen);
RW_FUNCTION(0x6B2F10, "SceneEvolveMedal reopen info popup (sub_6B2F10)", reopenInfo);
RW_FUNCTION(0x6B30C8, "SceneEvolveMedal material medal load (sub_6B30C8)", materialLoad);
