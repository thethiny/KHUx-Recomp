// SceneDeckEdit: the Equipment screen (MyPage > Equipment). The keyblades slide in a BaseSlideView (one page per
// keyblade: its image and a DeckRevolverView of its medals); the medal list (MiniMedalScrollView, sorted by the
// IMedalSortFilter mix-in) and the keyblade strengthening panel open as popups. Changed keyblades are sent with
// executeUserKeybladeUpdate when leaving.
//
// Layout: Layer; +0x288 IInOutAnimation, +0x28C IMedalSortFilter (medals +0x290, filtered +0x29C); +0x2A8 the scene
// root; +0x2AC..0x2E3 the saved positions of the parts below (Vec2 each); +0x2E4 CenterUI_Top, +0x2E8
// CenterUI_Under, +0x2EC LeftUI, +0x2F0 RightUI, +0x2F4 KBCount, +0x2F8 Pop_KBStrength, +0x2FC Pop_MedalForm;
// +0x310 Button_Medal, +0x314 Button_List, +0x318 Button_KB, +0x31C Button_Sort; +0x320 bool[] keyblade changed;
// +0x324 vector<UserKeyblade::Keyblade> (0x58 bytes, +0x10 medal count, +0x18 medal ids, +0x4C equipped);
// +0x37D open the keyblade panel, +0x37E ready; +0x380 the shown DeckRevolverView; +0x384 the slide's position;
// +0x38C the BaseSlideView, +0x390 the MiniMedalScrollView, +0x394 the equipped keyblade, +0x398 the "n/m" text;
// +0x3A1 can finish; +0x3A8 std::function<bool()> prepare-finish; +0x3C0 Back allowed.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneManager.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_dtor = 0x88A88D, Layer_init = 0x88A8A1;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t MEMSET = 0x40E7A0;                       // __aeabi_memset(dest, n, c) (PLT)
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t Vec2_ctor = 0x8E4B95, Vec2_ctorXY = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t WidgetVector_dtor = 0x485A8F;            // cocos2d::Vector<ui::Widget*>::~Vector
constexpr uint32_t KeybladeVector_baseDtor = 0x4C50FF;
constexpr uint32_t KeybladeVector_pushSlow = 0x4D4FCD;
constexpr uint32_t MedalVector_baseDtor = 0x46B119;
constexpr uint32_t MedalVector_pushSlow = 0x487A39;
constexpr uint32_t IntVector_baseDtor = 0x478961, IntVector_copy = 0x4AF707, IntVector_pushSlow = 0x4D6CC1;
constexpr uint32_t VECTOR_AT_THROW = 0x47CCE1;
constexpr uint32_t Function_vv_assign = 0x4EB9CD, Function_vb_assign = 0x490109;
constexpr uint32_t Function_vv_copy = 0x4683CD, Function_vv_move = 0x47F905;
constexpr uint32_t Function_vb_copy = 0x45C84D, Function_vb_move = 0x47BC69, Function_vb_call = 0x474139;
constexpr uint32_t Function_bv_copy = 0x46A3B5;
constexpr uint32_t Swap_vv = 0x4EE86D, Swap_bv = 0x47D0F1, Swap_widgetInt = 0x54B1C5, Swap_widgetWidget = 0x547159;
constexpr uint32_t Swap_vUllInt = 0x67947D, Swap_iUllVec2 = 0x6791B1, Swap_vIntUll = 0x678E79;
constexpr uint32_t Swap_medalInfo = 0x4876D9, Swap_vUll = 0x65F595;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t SceneManager_setFinishSceneCallback = 0x613E3F;
constexpr uint32_t SceneManager_setPrepareFinishSceneCallback = 0x613E31;
constexpr uint32_t SceneManager_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneUtil_calcDeckStatus = 0x469CB5;     // (shared_ptr kb, medals&, int* out[3], medals*)
constexpr uint32_t SceneUtil_setNormalColor = 0x468669, SceneUtil_setWarningColor = 0x4686A9;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t BaseSlideView_create = 0x534E79, BaseSlideView_setIndex = 0x526141;
constexpr uint32_t BaseSlideView_setItemIDs = 0x5260AD, BaseSlideView_getItemIDs = 0x525EA5;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_setTargetAnchorPoint = 0x5318CF, ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69, MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t KeyBladeDataManager_getInstance = 0x76B60D, KeyBladeDataManager_getKeyBladeData = 0x76B659;
constexpr uint32_t KeyBladeDataManager_getKeyBladeImagePath = 0x76B809;
constexpr uint32_t IMedalSortFilter_runSortFilter = 0x66204D, IMedalSortFilter_registSortButton = 0x664DA5;
constexpr uint32_t SceneDeckEdit_openMedalSelectPanelMain = 0x662611, SceneDeckEdit_openKeyBladePanel = 0x662E2D;
constexpr uint32_t MiniMedalScrollView_create = 0x664D51, MiniMedalScrollView_setIsShowMixInfo = 0x4FD545;
constexpr uint32_t MiniMedalScrollView_changeShowType = 0x4FD531;
constexpr uint32_t DeckRevolverView_create = 0x52C471, DeckRevolverView_setKeyBladeData = 0x52CC21;
constexpr uint32_t DeckRevolverView_setMedalDatas = 0x52D135;
constexpr uint32_t DeckRevolverView_setOpenMedalInfoPopupEnabled = 0x52D5A5;
constexpr uint32_t DeckRevolverView_setTargetMedalList = 0x52D5FD;
constexpr uint32_t APIManager_executeUserKeybladeUpdate = 0x4C8ED5;
constexpr uint32_t AppState_getDeckMedalSortFilterState = 0x4EB095;
}  // namespace g

constexpr uint32_t SIZE = 0x3C8;
constexpr uint32_t VTABLE = 0xF9A808, VTABLE_INOUT = 0xF9AB1C, VTABLE_SORTFILTER = 0xF9AB34;
constexpr uint32_t SORT_FILTER = 0x28C, MEDALS = 0x290, ROOT = 0x2A8;
constexpr uint32_t POS_TOP = 0x2AC, POS_UNDER = 0x2B4, POS_LEFT = 0x2BC, POS_RIGHT = 0x2C4, POS_KBCOUNT = 0x2CC,
                   POS_KBSTRENGTH = 0x2D4, POS_MEDALFORM = 0x2DC;
constexpr uint32_t TOP = 0x2E4, UNDER = 0x2E8, LEFT = 0x2EC, RIGHT = 0x2F0, KBCOUNT = 0x2F4, KBSTRENGTH = 0x2F8,
                   MEDALFORM = 0x2FC;
constexpr uint32_t BUTTON_MEDAL = 0x310, BUTTON_LIST = 0x314, BUTTON_KB = 0x318, BUTTON_SORT = 0x31C;
constexpr uint32_t CHANGED = 0x320, KEYBLADES = 0x324;
constexpr uint32_t OPEN_KB_PANEL = 0x37D, READY = 0x37E, REVOLVER = 0x380, POS_SLIDE = 0x384, SLIDE = 0x38C,
                   SCROLL = 0x390, EQUIPPED = 0x394, COUNT_TEXT = 0x398, CAN_FINISH = 0x3A1, PREPARE_FINISH = 0x3A8,
                   CAN_BACK = 0x3C0;
constexpr uint32_t KEYBLADE_SIZE = 0x58, KB_MEDAL_COUNT = 0x10, KB_MEDAL_IDS = 0x18, KB_EQUIPPED = 0x4C;
constexpr uint32_t KEYBLADES_DIV = 0xBA2E8BA3u;   // the compiler's divide-by-0x58 (after >> 3)

// BaseSlideView fields
constexpr uint32_t SLIDE_MAKE_PAGE = 0x500, SLIDE_UPDATE_PAGE = 0x518, SLIDE_ON_CHANGE = 0x530, SLIDE_IDS = 0x578,
                   SLIDE_INDEX = 0x5AC;
// MiniMedalScrollView fields
constexpr uint32_t SCROLL_ON_SELECT = 0x638, SCROLL_ON_LONG_TAP = 0x650, SCROLL_ON_DRAG = 0x668,
                   SCROLL_SHOW_MIX = 0x6F8;
// DeckRevolverView fields
constexpr uint32_t REV_ON_MEDAL_INFO = 0x270, REV_ON_REMOVE = 0x288, REV_ON_CHANGE = 0x2A0, REV_KB_DATA = 0x2B8,
                   REV_MEDALS = 0x2C0, REV_ENABLED = 0x314, REV_EDITABLE = 0x328, REV_SLOTS = 0x32C,
                   SLOT_EDITABLE = 0x3B8;
// APIManager
constexpr uint32_t API_USER_DETAIL = 0x94, API_USER_MEDALS = 0xD4, API_USER_KEYBLADES = 0xEC;
constexpr uint32_t USER_COST_LIMIT = 0x1C;
// SceneManager
constexpr uint32_t SM_KEYBLADE_ID = 0x2B0, SM_OPEN_KB_PANEL = 0x2E0, SM_OPEN_MEDAL_PANEL = 0x2E1,
                   SM_BACK_FLAG = 0x4CC, SM_BACK_CALLBACK = 0x548;

// strings
constexpr uint32_t S_DeckBG = 0xE03AF2, S_SceneJson = 0xE03E49, S_KBMaterialJson = 0xE03F09,
                   S_TxtKBCountLabel = 0xE03E76, S_CenterUITop = 0xE03E88, S_CenterUIUnder = 0xE03E95,
                   S_Panel1 = 0xE02A0D, S_LeftUI = 0xDF85F0, S_RightUI = 0xDF85F7, S_KBCount = 0xE03EA4,
                   S_PopKBStrength = 0xE03EAC, S_PopMedalForm = 0xE03EBB, S_ButtonMedal = 0xE03EC9,
                   S_TxtMedal = 0xE03ED6, S_ButtonKB = 0xE03EE0, S_TxtKB = 0xE03EEA, S_ButtonList = 0xE00643,
                   S_TxtList = 0xE00658, S_TxtPower = 0xDF8EDC, S_TxtDefence = 0xDF8EF8, S_TxtCost = 0xDF9C29,
                   S_LeftArrow = 0xDFF5DA, S_Empty = 0xDF8A1A, S_WinUnder = 0xE03EF1, S_AreaMaterial = 0xE03EFB,
                   S_MaterialFmt = 0xE03F3C, S_MaterialPanel = 0xE03F47, S_MaterialNameTxt = 0xE03F56,
                   S_MaterialMaxTxt = 0xE03F68, S_MaterialSlashTxt = 0xE03F79, S_MaterialNowTxt = 0xE03F8C,
                   S_PowerUpButton = 0xE03F9D, S_TxtPowerUp = 0xE03FAC, S_CloseButton = 0xDF8D2F,
                   S_ButtonSort = 0xE03DC6, S_ScrollArea = 0xDF9B59, S_TabMainPanel = 0xE04165,
                   S_TxtPowerLabel = 0xDF8ECC, S_IntFmt = 0xDF86DE, S_TxtDefenceLabel = 0xDF8EE6,
                   S_TxtCostLabel = 0xE04032, S_TxtSlashLabel = 0xE04041, S_TxtCostMaxLabel = 0xE04051,
                   S_KB = 0xDFEF39, S_CenterUIRevolver = 0xE04172, S_AreaRevolver = 0xDF8F48,
                   S_RevolverView = 0xDF8F3B, S_CountFmt = 0xDF8F35, S_TxtKBNameLabel = 0xDF8F04;

// texts
constexpr int32_t TXT_TICKER = 100620001, TXT_MEDAL = 101200014, TXT_KB = 101200015, TXT_LIST = 101200027,
                  TXT_POWER = 100200007, TXT_DEFENCE = 100200008, TXT_COST = 100200011,
                  TXT_MATERIAL_NAME = 101200040, TXT_MATERIAL_MAX = 101200041, TXT_MATERIAL_NOW = 101200042,
                  TXT_POWER_UP = 101900001, TXT_COST_OVER = 101200022;

// lambdas (__func vtables + 8)
constexpr uint32_t VT_OnEnter = 0xF63E94, VT_OnExit = 0xF63E68, VT_BackKey = 0xF63E3C, VT_MedalTouched = 0xF63E10,
                   VT_KBTouched = 0xF63DE4, VT_ListTouched = 0xF63D8C, VT_LeftArrow = 0xF63D60,
                   VT_RightArrow = 0xF63D34, VT_MakePage = 0xF63D08, VT_UpdatePage = 0xF63CDC,
                   VT_PageChanged = 0xF63CB0, VT_Finish = 0xF63C58, VT_PrepareFinish = 0xF63C2C;
constexpr uint32_t VT_PowerUp = 0xF63A48, VT_KBPanelClose = 0xF63A1C;
constexpr uint32_t VT_MedalPanelClose = 0xF6396C, VT_MedalSelected = 0xF63914, VT_MedalDragged = 0xF638E8,
                   VT_MedalLongTap = 0xF638BC, VT_SortChanged = 0xF63890;
constexpr uint32_t VT_SlideIn = 0xF63BA8, VT_UnderOut = 0xF63B7C;
constexpr uint32_t VT_MedalInfo = 0xF636D8, VT_MedalRemove = 0xF636AC, VT_DeckChanged = 0xF63680;
constexpr uint32_t VT_KeybladeUpdated = 0xF63470, VT_KeybladeUpdatedTutorial = 0xF6349C;

constexpr int32_t PHASE_DECK_TUTORIAL = 220, PHASE_MEDAL_TUTORIAL = 410, PHASE_TUTORIAL_END = 994;

// node vtable slots
constexpr uint32_t V_SET_POSITION = 0x4C, V_GET_SCALE = 0x44, V_SET_POSITION_Y = 0x6C, V_SET_CONTENT_SIZE = 0xA8,
                   V_GET_CONTENT_SIZE = 0xAC, V_IS_VISIBLE = 0xB4, V_GET_CHILDREN = 0x10C,
                   V_WIDGET_SET_ENABLED = 0x298, V_WIDGET_SET_TOUCH_ENABLED = 0x29C, V_RUN_IN_ANIMATION = 0x300,
                   V_GET_SORT_FILTER = 0x308;
// DeckMedalView::ShowType per sort key (IMedalSortFilter's table)
constexpr uint32_t SHOW_TYPE_TABLE = 0xE6E270, SHOW_TYPE_KEYS = 0x29F, SHOW_TYPE_DEFAULT = 4;

void addRef(uint32_t ctrl) {
    if (ctrl) call(fn::shared_add, {ctrl});
}
void release(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}
void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }
uint32_t api() { return call(fn::APIManager_getInstance, {}); }
uint32_t smgr() { return call(fn::SceneManager_getInstance, {}); }
uint32_t keybladeCount(uint32_t me) {
    return uint32_t(int32_t(ld32(me + KEYBLADES + 4) - ld32(me + KEYBLADES)) >> 3) * KEYBLADES_DIV;
}

// node = find(root, name); keep it at `field` and its position at `pos`
uint32_t findPart(uint32_t me, uint32_t root, uint32_t name, uint32_t len, uint32_t field, uint32_t pos) {
    st32(me + field, findNode(root, name, len));
    const uint32_t node = ld32(me + field);
    const uint32_t p = vcall(node, node::getPosition, {node});
    const uint32_t x = ld32(p), y = ld32(p + 4);
    st32(me + pos + 4, y);
    st32(me + pos, x);
    return node;
}
// text <- TextData `id`, with a shadow (shadow first)
void shadowText(uint32_t text, int32_t id) {
    SceneUtil::setShadow(text);
    setTextId(text, id);
}
// text <- printf("%d", v), with a shadow
void shadowInt(uint32_t text, int32_t v) {
    SceneUtil::setShadow(text);
    Frame f;
    setTextC(text, stringBuffer(f, {S_IntFmt, uint32_t(v)}));
}

// a std::function on the stack holding lambda `vtable` {captures}, swapped into `dst` (`swap` = that type's swap)
void swapInto(uint32_t dst, uint32_t swap, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    Frame f;
    const uint32_t cb = newFunction(f, vtable, captures);
    call(swap, {cb, dst});
    functionDestroy(cb);
}

// node->runAction(EaseSineOut(make(node, done)))
void runEased(uint32_t node, bool right, bool in, uint32_t done) {
    uint32_t action;
    if (in) action = right ? SceneUtil::createRightInAnimation(node, 0.0f, 0.0f, done)
                           : SceneUtil::createLeftInAnimation(node, 0.0f, 0.0f, done);
    else action = right ? SceneUtil::createRightOutAnimation(node, 0.0f, 0.0f, done)
                        : SceneUtil::createLeftOutAnimation(node, 0.0f, 0.0f, done);
    call(fn::Node_runAction, {node, call(g::EaseSineOut_create, {action})});
}
void runEasedNoCallback(uint32_t node, bool right, bool in) {
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    runEased(node, right, in, none);
    functionDestroy(none);
}

// KeyBladeDataManager::getKeyBladeData(out, manager, id, 0, UserKeyblade::Keyblade{}): the keyblade in a stack
// shared_ptr. The Keyblade argument's uncleared bytes (0x14..0x17, 0x4D..0x4F, 0x54..0x57) are passed as 0.
uint32_t keybladeData(Frame &f, uint32_t manager, uint32_t idLo, uint32_t idHi) {
    const uint32_t out = f.alloc(8);
    call(g::KeyBladeDataManager_getKeyBladeData, {out, manager, idLo, idHi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    return out;
}
// MedalDataManager::getMedalData(out, manager, id, 0, UserMedal::Medal{}) (bytes 0x2C..0x2F passed as 0)
uint32_t medalData(Frame &f, uint32_t manager, uint32_t idLo, uint32_t idHi) {
    const uint32_t out = f.alloc(8);
    call(g::MedalDataManager_getMedalData, {out, manager, idLo, idHi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0});
    return out;
}
// vector<shared_ptr<T>>::push_back(const shared_ptr&)
void pushShared(uint32_t vec, uint32_t sp) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(g::MedalVector_pushSlow, {vec, sp});
        return;
    }
    if (end) {
        st32(end, ld32(sp));
        st32(end + 4, ld32(sp + 4));
        addRef(ld32(sp + 4));
    }
    st32(vec + 4, ld32(vec + 4) + 8);
}
// the medals of keyblade record `kb` (count +0x10, ids +0x18) into the stack vector `medals`
void loadDeckMedals(uint32_t kb, uint32_t medals) {
    for (uint32_t n = 0; n < ld32(kb + KB_MEDAL_COUNT); n++) {
        const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
        const uint32_t id = kb + KB_MEDAL_IDS + 8 * n;
        Frame f;
        const uint32_t medal = medalData(f, mdm, ld32(id), ld32(id + 4));
        pushShared(medals, medal);
        release(ld32(medal + 4));
    }
}
// vector<UserKeyblade::Keyblade>::push_back(const Keyblade&)
void pushKeyblade(uint32_t vec, uint32_t kb) {
    const uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(g::KeybladeVector_pushSlow, {vec, kb});
        return;
    }
    if (end) call(g::MEMCPY, {end, kb, KEYBLADE_SIZE});
    st32(vec + 4, end + KEYBLADE_SIZE);
}
int32_t costLimit() { return safe32(ld32(api() + API_USER_DETAIL), USER_COST_LIMIT); }
}  // namespace

namespace deck_edit {

uint32_t construct(uint32_t me);

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    construct(me);
    if (vcall(me, 0x248, {me}) == 1) {   // init
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}
RW_FUNCTION(0x57AFD4, "SceneDeckEdit::create", deck_edit::create);

uint32_t construct(uint32_t me) {
    call(g::Layer_ctor, {me});
    call(g::MEMSET, {me + MEDALS, 0x18, 0});
    st32(me, VTABLE);
    st32(me + 0x288, VTABLE_INOUT);
    st32(me + SORT_FILTER, VTABLE_SORTFILTER);
    for (uint32_t p = POS_TOP; p <= POS_MEDALFORM; p += 8) call(g::Vec2_ctor, {me + p});
    call(g::MEMSET, {me + TOP, 0x78, 0});
    for (uint32_t p : {0x35Cu, 0x364u, 0x36Cu}) call(g::Vec2_ctor, {me + p});
    st8(me + 0x374, 0);
    st8(me + 0x375, 0);
    st32(me + 0x378, 0);
    st8(me + 0x37C, 0);
    st8(me + OPEN_KB_PANEL, 1);
    st8(me + READY, 0);
    st32(me + REVOLVER, 0);
    call(g::Vec2_ctor, {me + POS_SLIDE});
    call(g::MEMSET, {me + SLIDE, 0x15, 0});
    st8(me + CAN_FINISH, 1);
    st32(me + PREPARE_FINISH + 16, 0);
    st8(me + CAN_BACK, 0);
    return me;
}
RW_FUNCTION(0x57B028, "SceneDeckEdit::SceneDeckEdit", deck_edit::construct);

uint32_t destroy(uint32_t me) {
    st32(me, VTABLE);
    st32(me + 0x288, VTABLE_INOUT);
    st32(me + SORT_FILTER, VTABLE_SORTFILTER);
    if (ld32(me + CHANGED)) {
        call(guest::OPERATOR_DELETE_ARRAY, {ld32(me + CHANGED)});
        st32(me + CHANGED, 0);
    }
    functionDestroy(me + PREPARE_FINISH);
    for (uint32_t p : {POS_SLIDE, 0x36Cu, 0x364u, 0x35Cu}) call(g::Vec2_dtor, {me + p});
    release(ld32(me + 0x350));
    call(g::WidgetVector_dtor, {me + 0x340});
    call(g::WidgetVector_dtor, {me + 0x334});
    call(g::KeybladeVector_baseDtor, {me + KEYBLADES});
    for (uint32_t p = POS_MEDALFORM; p >= POS_TOP; p -= 8) call(g::Vec2_dtor, {me + p});
    st32(me + SORT_FILTER, 0xF8B298);   // ~IMedalSortFilter
    call(g::MedalVector_baseDtor, {me + 0x29C});
    call(g::MedalVector_baseDtor, {me + MEDALS});
    call(g::Layer_dtor, {me});
    return me;
}
RW_FUNCTION(0x65FFD8, "SceneDeckEdit::~SceneDeckEdit", deck_edit::destroy);

// IMedalSortFilter's state: the deck medal sort / filter state (in the deck tutorial: sorted by key 4, no filter)
void getSortFilter(uint32_t out) {
    call(g::AppState_getDeckMedalSortFilterState, {out, call(fn::AppState_getInstance, {})});
    if (SceneUtil::getNexTutorialphase() == PHASE_DECK_TUTORIAL) {
        st8(ld32(out) + 1, 4);
        st8(ld32(out) + 2, 0);
    }
}
RW_FUNCTION(0x669874, "SceneDeckEdit::getSortFilter", deck_edit::getSortFilter);

void initKeyBladePanel(uint32_t me);
void initMedalSelectPanel(uint32_t me);
uint32_t checkPrepareFinishScene(uint32_t me);

// ------------------------------------------------------------------------------------------------ init
// The scene: callbacks, background, header, ticker, DeckEditScene; the parts and their positions; the two popups;
// the Medal / KB / List buttons; the user's medals (sorted) and keyblades; the keyblade slide (one page per
// keyblade, arrows when there are two or more); runInAnimation, the finish callbacks; then the panel to open.
uint32_t init(uint32_t me) {
    if (call(g::Layer_init, {me}) != 1) return 0;
    {
        Frame f;
        const uint32_t cb = newFunction(f, VT_OnEnter, {me});
        call(g::Function_vv_assign, {me + 0x210, cb});
        functionDestroy(cb);
    }
    {
        Frame f;
        const uint32_t cb = newFunction(f, VT_OnExit, {me});
        call(g::Function_vv_assign, {me + 0x228, cb});
        functionDestroy(cb);
    }
    {
        const uint32_t sm = smgr();
        Frame f;
        const uint32_t cb = newFunction(f, VT_BackKey, {me});
        SceneManager::setOnBackKeyCallback(rw::host_ptr<SceneManager>(sm), cb);
        functionDestroy(cb);
    }
    st8(me + CAN_BACK, 1);
    {
        const uint32_t sm = smgr();
        Frame f;
        const uint32_t name = newStr(f, S_DeckBG, 0x1A);
        SceneManager::changeBackground(rw::host_ptr<SceneManager>(sm), 0, name);
        strFree(name);
    }
    SceneManager::setBackButtonType(rw::host_ptr<SceneManager>(smgr()), 0);
    SceneManager::showHeader(rw::host_ptr<SceneManager>(smgr()));
    {
        const uint32_t sm = smgr();
        Frame f;
        const uint32_t td = textData(f, TXT_TICKER);
        const uint32_t s = newStr(f, textDataStr(td));
        call(g::SceneManager_changeTickerMessage, {sm, s});
        strFree(s);
        sharedRelease(td);
    }
    {
        Frame f;
        const uint32_t file = newStr(f, S_SceneJson, 0x2C);
        st32(me + ROOT, SceneUtil::createNodeWithSceneFile(file));
        strFree(file);
    }
    st32(me + COUNT_TEXT, findNode(ld32(me + ROOT), S_TxtKBCountLabel, 0x11));
    SceneUtil::setShadow(ld32(me + COUNT_TEXT));
    vcall(me, node::addChild, {me, ld32(me + ROOT)});
    // the parts
    findPart(me, ld32(me + ROOT), S_CenterUITop, 0xC, TOP, POS_TOP);
    const uint32_t under = findPart(me, ld32(me + ROOT), S_CenterUIUnder, 0xE, UNDER, POS_UNDER);
    {
        const uint32_t panel = findNode(under, S_Panel1, 7);
        vcall(panel, V_WIDGET_SET_TOUCH_ENABLED, {panel, 0});
    }
    findPart(me, ld32(me + ROOT), S_LeftUI, 6, LEFT, POS_LEFT);
    findPart(me, ld32(me + ROOT), S_RightUI, 7, RIGHT, POS_RIGHT);
    findPart(me, ld32(me + ROOT), S_KBCount, 7, KBCOUNT, POS_KBCOUNT);
    findPart(me, ld32(me + ROOT), S_PopKBStrength, 0xE, KBSTRENGTH, POS_KBSTRENGTH);
    initKeyBladePanel(me);
    findPart(me, ld32(me + ROOT), S_PopMedalForm, 0xD, MEDALFORM, POS_MEDALFORM);
    initMedalSelectPanel(me);
    // the buttons
    st32(me + BUTTON_MEDAL, findNode(ld32(me + ROOT), S_ButtonMedal, 0xC));
    addTouch(ld32(me + BUTTON_MEDAL), VT_MedalTouched, {me});
    shadowText(findNode(ld32(me + BUTTON_MEDAL), S_TxtMedal, 9), TXT_MEDAL);
    st32(me + BUTTON_KB, findNode(ld32(me + ROOT), S_ButtonKB, 9));
    addTouch(ld32(me + BUTTON_KB), VT_KBTouched, {me});
    shadowText(findNode(ld32(me + BUTTON_KB), S_TxtKB, 6), TXT_KB);
    st32(me + BUTTON_LIST, findNode(ld32(me + LEFT), S_ButtonList, 0xB));
    addTouch(ld32(me + BUTTON_LIST), VT_ListTouched, {me});
    {
        const int32_t phase = SceneUtil::getNexTutorialphase();
        const uint32_t list = ld32(me + BUTTON_LIST);
        vcall(list, V_WIDGET_SET_ENABLED, {list, phase > PHASE_TUTORIAL_END ? 1u : 0u});
    }
    shadowText(findNode(ld32(me + BUTTON_LIST), S_TxtList, 8), TXT_LIST);
    titleLabel(ld32(me + TOP), S_TxtPower, 9, TXT_POWER);
    titleLabel(ld32(me + TOP), S_TxtDefence, 0xB, TXT_DEFENCE);
    titleLabel(ld32(me + TOP), S_TxtCost, 8, TXT_COST);
    // the user's medals, sorted
    while (ld32(me + MEDALS + 4) != ld32(me + MEDALS)) {
        const uint32_t last = ld32(me + MEDALS + 4) - 8;
        st32(me + MEDALS + 4, last);
        release(ld32(last + 4));
    }
    {
        const uint32_t a = api();
        for (uint32_t n = 0, at = 0; n < ld32(ld32(a + API_USER_MEDALS) + 0x20); n++, at += 0x48) {
            const uint32_t mdm = call(g::MedalDataManager_getInstance, {});
            Frame f;
            const uint32_t rec = safeBytes(f, ld32(a + API_USER_MEDALS), at, 0x48);
            const uint32_t medal = medalData(f, mdm, ld32(rec), ld32(rec + 4));
            pushShared(me + MEDALS, medal);
            release(ld32(medal + 4));
        }
    }
    call(g::IMedalSortFilter_runSortFilter, {me + SORT_FILTER, 0});
    // the user's keyblades (the equipped one remembered)
    {
        const uint32_t a = api();
        for (uint32_t n = 0, at = 0; n < ld32(ld32(a + API_USER_KEYBLADES) + 0x20); n++, at += KEYBLADE_SIZE) {
            Frame f;
            const uint32_t rec = safeBytes(f, ld32(a + API_USER_KEYBLADES), at, KEYBLADE_SIZE);
            pushKeyblade(me + KEYBLADES, rec);
            if (ld8(rec + KB_EQUIPPED)) st32(me + EQUIPPED, n);
        }
    }
    // the keyblade slide
    st32(me + SLIDE, call(g::BaseSlideView_create, {}));
    {
        const uint32_t slide = ld32(me + SLIDE);
        const uint32_t p = vcall(slide, node::getPosition, {slide});
        const uint32_t x = ld32(p), y = ld32(p + 4);
        st32(me + POS_SLIDE + 4, y);
        st32(me + POS_SLIDE, x);
        Frame f;
        const uint32_t size = f.alloc(8);
        call(fn::Director_getVisibleSize, {size, call(fn::Director_getInstance, {})});
        vcall(slide, V_SET_CONTENT_SIZE, {slide, size});
    }
    Frame frame;
    const uint32_t ids = frame.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    if (ld32(me + KEYBLADES + 4) != ld32(me + KEYBLADES)) {
        for (uint32_t n = 1;; n++) {
            const uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                Frame f;
                const uint32_t v = f.alloc(4);
                st32(v, n - 1);
                call(g::IntVector_pushSlow, {ids, v});
            } else {
                if (end) st32(end, n - 1);
                st32(ids + 4, end + 4);
            }
            if (!(n < keybladeCount(me))) break;
        }
    }
    if (uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 2) < 2) {
        setVisible(findNode(ld32(me + LEFT), S_LeftArrow, 0xA), 0);
        setVisible(ld32(me + RIGHT), 0);
    } else {
        // the arrows: an invisible ImageButton over each side's first child
        const auto arrow = [&](uint32_t side, uint32_t target, uint32_t vtable) {
            setVisible(target, 1);
            const uint32_t child = ld32(ld32(vcall(target, V_GET_CHILDREN, {target})));
            Frame f;
            const uint32_t empty = newStr(f, S_Empty, 0);
            const uint32_t button = call(g::ImageButton_create, {empty, 0});
            strFree(empty);
            call(g::ImageButton_setTargetView, {button, child, fbits(1.0f)});
            const uint32_t anchor = f.alloc(8);
            call(g::Vec2_ctorXY, {anchor, fbits(-0.5f), fbits(-0.5f)});
            call(g::ImageButton_setTargetAnchorPoint, {button, anchor});
            call(g::Vec2_dtor, {anchor});
            vcall(side, node::addChild, {side, button});
            const uint32_t cb = newFunction(f, vtable, {me});
            call(g::ImageButton_addTouchEventListener, {button, cb});
            functionDestroy(cb);
        };
        const uint32_t left = findNode(ld32(me + LEFT), S_LeftArrow, 0xA);
        arrow(ld32(me + LEFT), left, VT_LeftArrow);
        arrow(ld32(me + RIGHT), ld32(me + RIGHT), VT_RightArrow);
    }
    swapInto(ld32(me + SLIDE) + SLIDE_MAKE_PAGE, g::Swap_widgetInt, VT_MakePage, {});
    swapInto(ld32(me + SLIDE) + SLIDE_UPDATE_PAGE, g::Swap_widgetWidget, VT_UpdatePage, {me});
    call(g::BaseSlideView_setIndex, {ld32(me + SLIDE), ld32(me + EQUIPPED)});
    {
        const uint32_t slide = ld32(me + SLIDE);
        Frame f;
        const uint32_t copy = f.alloc(12);
        call(g::IntVector_copy, {copy, ids});
        call(g::BaseSlideView_setItemIDs, {slide, copy, 0});
        call(g::IntVector_baseDtor, {copy});
    }
    st8(me + OPEN_KB_PANEL, ld8(smgr() + SM_OPEN_KB_PANEL));
    swapInto(ld32(me + SLIDE) + SLIDE_ON_CHANGE, g::Swap_vv, VT_PageChanged, {me});
    vcall(me, node::addChild, {me, ld32(me + SLIDE)});
    st8(me + READY, 1);
    vcall(me, V_RUN_IN_ANIMATION, {me});
    {
        const uint32_t sm = smgr();
        Frame f;
        const uint32_t cb = newFunction(f, VT_Finish, {me});
        call(g::SceneManager_setFinishSceneCallback, {sm, cb});
        functionDestroy(cb);
    }
    {
        // the changed flags, one per keyblade
        const uint32_t n = ld32(me + KEYBLADES + 4) == ld32(me + KEYBLADES) ? 0xFFFFFFFFu : keybladeCount(me);
        const uint32_t changed = call(guest::OPERATOR_NEW_ARRAY, {n});
        st8(changed, 0);
        call(g::MEMSET, {changed + 1, n - 1, 0});
        st32(me + CHANGED, changed);
    }
    if (SceneUtil::getNexTutorialphase() == PHASE_MEDAL_TUTORIAL) st8(smgr() + SM_OPEN_MEDAL_PANEL, 0);
    if (ld8(smgr() + SM_OPEN_MEDAL_PANEL)) call(g::SceneDeckEdit_openMedalSelectPanelMain, {me});
    else if (ld8(me + OPEN_KB_PANEL)) call(g::SceneDeckEdit_openKeyBladePanel, {me, 1});
    swapInto(me + PREPARE_FINISH, g::Swap_bv, VT_PrepareFinish, {me});
    {
        const uint32_t sm = smgr();
        Frame f;
        const uint32_t copy = f.alloc(20);
        call(g::Function_bv_copy, {copy, me + PREPARE_FINISH});
        call(g::SceneManager_setPrepareFinishSceneCallback, {sm, copy});
        functionDestroy(copy);
    }
    if (SceneUtil::getNexTutorialphase() <= PHASE_TUTORIAL_END) checkPrepareFinishScene(me);
    call(g::IntVector_baseDtor, {ids});
    return 1;
}
RW_FUNCTION(0x66016C, "SceneDeckEdit::init", deck_edit::init);

// The keyblade strengthening popup (hidden): four material rows from DeckEdit_KBMaterial stacked from the top of
// Area_Material, the material panel's titles, Power Up and Close.
void initKeyBladePanel(uint32_t me) {
    const uint32_t pop = ld32(me + KBSTRENGTH);
    setVisible(pop, 0);
    {
        const uint32_t win = findNode(ld32(me + KBSTRENGTH), S_WinUnder, 9);
        vcall(win, V_GET_SCALE, {win});
    }
    const uint32_t area = findNode(ld32(me + KBSTRENGTH), S_AreaMaterial, 0xD);
    for (int32_t n = 1; n <= 4; n++) {
        const uint32_t row = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}),
                                                                     S_KBMaterialJson});
        const float areaHeight = ldf(vcall(area, V_GET_CONTENT_SIZE, {area}) + 4);
        const float rowHeight = ldf(vcall(row, V_GET_CONTENT_SIZE, {row}) + 4);
        vcall(row, V_SET_POSITION_Y, {row, fbits(areaHeight - float(n) * rowHeight)});
        Frame f;
        const uint32_t name = newStr(f, stringBuffer(f, {S_MaterialFmt, uint32_t(n)}));
        vcall(area, node::addChildZName, {area, row, 0, name});
        strFree(name);
    }
    const uint32_t panel = findNode(ld32(me + KBSTRENGTH), S_MaterialPanel, 0xE);
    titleLabel(panel, S_MaterialNameTxt, 0x11, TXT_MATERIAL_NAME);
    titleLabel(panel, S_MaterialMaxTxt, 0x10, TXT_MATERIAL_MAX);
    SceneUtil::setShadow(findNode(panel, S_MaterialSlashTxt, 0x12));
    titleLabel(panel, S_MaterialNowTxt, 0x10, TXT_MATERIAL_NOW);
    const uint32_t powerUp = findNode(ld32(me + KBSTRENGTH), S_PowerUpButton, 0xE);
    addTouch(powerUp, VT_PowerUp, {me, powerUp});
    shadowText(findNode(powerUp, S_TxtPowerUp, 0xB), TXT_POWER_UP);
    addTouch(findNode(ld32(me + KBSTRENGTH), S_CloseButton, 0xC), VT_KBPanelClose, {me});
}
RW_FUNCTION(0x661658, "SceneDeckEdit::initKeyBladePanel", deck_edit::initKeyBladePanel);

// The medal list popup (hidden): Sort button, Close, the MiniMedalScrollView in Scroll_Area with its select /
// drag / long-tap callbacks, the sort button's callback, and the list's show type from the sort key.
void initMedalSelectPanel(uint32_t me) {
    setVisible(ld32(me + MEDALFORM), 0);
    const uint32_t win = findNode(ld32(me + MEDALFORM), S_WinUnder, 9);
    vcall(win, V_GET_SCALE, {win});
    st32(me + BUTTON_SORT, findNode(win, S_ButtonSort, 0xB));
    {
        const uint32_t close = findNode(ld32(me + MEDALFORM), S_CloseButton, 0xC);
        setVisible(close, 1);
        addTouch(close, VT_MedalPanelClose, {me});
    }
    const uint32_t area = findNode(win, S_ScrollArea, 0xB);
    st32(me + SCROLL, call(g::MiniMedalScrollView_create, {}));
    {
        const uint32_t scroll = ld32(me + SCROLL);
        vcall(scroll, V_SET_CONTENT_SIZE, {scroll, vcall(area, V_GET_CONTENT_SIZE, {area})});
    }
    st8(ld32(me + SCROLL) + SCROLL_SHOW_MIX, 1);
    call(g::MiniMedalScrollView_setIsShowMixInfo, {ld32(me + SCROLL), 1});
    swapInto(ld32(me + SCROLL) + SCROLL_ON_SELECT, g::Swap_vUllInt, VT_MedalSelected, {me});
    swapInto(ld32(me + SCROLL) + SCROLL_ON_DRAG, g::Swap_iUllVec2, VT_MedalDragged, {me});
    swapInto(ld32(me + SCROLL) + SCROLL_ON_LONG_TAP, g::Swap_vIntUll, VT_MedalLongTap, {me});
    vcall(area, node::addChild, {area, ld32(me + SCROLL)});
    {
        Frame f;
        const uint32_t cb = newFunction(f, VT_SortChanged, {me});
        call(g::IMedalSortFilter_registSortButton, {me + SORT_FILTER, win, 0, cb});
        functionDestroy(cb);
    }
    uint32_t showType = SHOW_TYPE_DEFAULT;
    {
        Frame f;
        const uint32_t state = f.alloc(8);
        vcall(me, V_GET_SORT_FILTER, {state, me});
        const uint32_t k = uint32_t(ld8(ld32(state) + 1)) - 1;
        if (k <= 9 && ((SHOW_TYPE_KEYS >> (k & 0xFFFF)) & 1)) showType = ld32(SHOW_TYPE_TABLE + 4 * k);
        release(ld32(state + 4));
    }
    call(g::MiniMedalScrollView_changeShowType, {ld32(me + SCROLL), showType});
}
RW_FUNCTION(0x661C6C, "SceneDeckEdit::initMedalSelectPanel", deck_edit::initMedalSelectPanel);

// ------------------------------------------------------------------------------------------------ finishing
// Leaving is allowed when no edited keyblade's deck costs more than the player's limit: else a dialog names the
// first such keyblade. Allowed: the open popups are closed.
uint32_t checkPrepareFinishScene(uint32_t me) {
    call(0x667EF9, {me});   // updateDeckData
    const uint32_t a = api();
    const uint32_t userCtrl = ld32(a + API_USER_DETAIL + 4);
    addRef(userCtrl);
    const int32_t limit = safe32(ld32(a + API_USER_DETAIL), USER_COST_LIMIT);
    uint32_t result = 1;
    for (uint32_t n = 0; ld32(me + KEYBLADES + 4) != ld32(me + KEYBLADES) && n < keybladeCount(me); n++) {
        const uint32_t kdm = call(g::KeyBladeDataManager_getInstance, {});
        const uint32_t kb = ld32(me + KEYBLADES) + KEYBLADE_SIZE * n;
        Frame f;
        const uint32_t kbData = keybladeData(f, kdm, ld32(kb), ld32(kb + 4));
        const uint32_t medals = f.alloc(12);
        st32(medals, 0);
        st32(medals + 4, 0);
        st32(medals + 8, 0);
        loadDeckMedals(kb, medals);
        const uint32_t status = call(guest::OPERATOR_NEW_ARRAY, {0xC});
        const uint32_t copy = f.alloc(8);
        st32(copy, ld32(kbData));
        st32(copy + 4, ld32(kbData + 4));
        addRef(ld32(copy + 4));
        call(g::SceneUtil_calcDeckStatus, {copy, medals, status, 0});
        release(ld32(copy + 4));
        const int32_t cost = int32_t(ld32(status + 8));
        call(guest::OPERATOR_DELETE_ARRAY, {status});
        if (limit < cost) {
            const uint32_t td = textData(f, TXT_COST_OVER);
            const uint32_t name = safeBytes(f, ld32(ld32(kbData) + 0x20), 4, 0x41);
            const uint32_t text = newStr(f, stringBuffer(f, {textDataStr(td), name}));
            const uint32_t none = newEmptyFunction(f);
            SceneUtil::openOkDialog4Line(text, none, 0);
            functionDestroy(none);
            strFree(text);
            sharedRelease(td);
            call(g::MedalVector_baseDtor, {medals});
            release(ld32(kbData + 4));
            result = 0;
            break;
        }
        call(g::MedalVector_baseDtor, {medals});
        release(ld32(kbData + 4));
    }
    if (result) {
        st8(me + CAN_FINISH, 0);
        const uint32_t form = ld32(me + MEDALFORM);
        if (vcall(form, V_IS_VISIBLE, {form}) == 1) call(g::SceneManager_closeCurrentPopup, {smgr(), 0});
        const uint32_t strength = ld32(me + KBSTRENGTH);
        if (vcall(strength, V_IS_VISIBLE, {strength}) == 1)
            call(g::SceneManager_closeCurrentPopup, {smgr(), 0});
        st8(me + CAN_FINISH, 1);
    }
    release(userCtrl);
    return result;
}
RW_FUNCTION(0x66321C, "SceneDeckEdit::checkPrepareFinishScene", deck_edit::checkPrepareFinishScene);

// ------------------------------------------------------------------------------------------------ animations
// Every part back at its saved position, sliding in from its side (the slide view calls $_13 when in).
void runInAnimation(uint32_t me) {
    const auto in = [&](uint32_t part, uint32_t pos, bool right) {
        const uint32_t node = ld32(me + part);
        vcall(node, V_SET_POSITION, {node, me + pos});
        runEasedNoCallback(ld32(me + part), right, true);
    };
    in(TOP, POS_TOP, false);
    {
        const uint32_t panel = findNode(ld32(me + UNDER), S_Panel1, 7);
        vcall(panel, V_WIDGET_SET_TOUCH_ENABLED, {panel, 0});
    }
    in(UNDER, POS_UNDER, true);
    in(LEFT, POS_LEFT, false);
    in(RIGHT, POS_RIGHT, true);
    in(KBCOUNT, POS_KBCOUNT, true);
    {
        const uint32_t slide = ld32(me + SLIDE);
        vcall(slide, V_SET_POSITION, {slide, me + POS_SLIDE});
        Frame f;
        const uint32_t cb = newFunction(f, VT_SlideIn, {me});
        runEased(ld32(me + SLIDE), false, true, cb);
        functionDestroy(cb);
    }
    in(KBSTRENGTH, POS_KBSTRENGTH, true);
    in(MEDALFORM, POS_MEDALFORM, true);
}
RW_FUNCTION(0x66360C, "SceneDeckEdit::runInAnimation", deck_edit::runInAnimation);

// Every part slides out to its side; `done` runs when CenterUI_Under is out ($_14).
void runOutAnimationMain(uint32_t me, uint32_t done) {
    runEasedNoCallback(ld32(me + TOP), false, false);
    {
        const uint32_t under = ld32(me + UNDER);
        Frame f;
        const uint32_t copy = f.alloc(20);
        call(g::Function_vv_copy, {copy, done});
        const uint32_t cb = f.alloc(20);
        heapFunction(cb, VT_UnderOut, 0x20, 8, g::Function_vv_move, copy);
        runEased(under, true, false, cb);
        functionDestroy(cb);
        functionDestroy(copy);
    }
    runEasedNoCallback(ld32(me + LEFT), false, false);
    runEasedNoCallback(ld32(me + RIGHT), true, false);
    runEasedNoCallback(ld32(me + KBCOUNT), true, false);
    runEasedNoCallback(ld32(me + SLIDE), false, false);
    runEasedNoCallback(ld32(me + KBSTRENGTH), true, false);
    runEasedNoCallback(ld32(me + MEDALFORM), true, false);
}
RW_FUNCTION(0x663A28, "SceneDeckEdit::runOutAnimationMain", deck_edit::runOutAnimationMain);

// ------------------------------------------------------------------------------------------------ the deck
// The deck's power / defence / cost (red when over the player's limit) and the limit, in the page's TabMainPanel.
void updateDeckStauts(uint32_t, uint32_t page, uint32_t keyblade, uint32_t medals) {
    const uint32_t status = call(guest::OPERATOR_NEW_ARRAY, {0xC});
    {
        Frame f;
        const uint32_t copy = f.alloc(8);
        st32(copy, ld32(keyblade));
        st32(copy + 4, ld32(keyblade + 4));
        addRef(ld32(copy + 4));
        call(g::SceneUtil_calcDeckStatus, {copy, medals, status, 0});
        release(ld32(copy + 4));
    }
    const int32_t power = int32_t(ld32(status)), defence = int32_t(ld32(status + 4)),
                  cost = int32_t(ld32(status + 8));
    call(guest::OPERATOR_DELETE_ARRAY, {status});
    const uint32_t tab = findNode(page, S_TabMainPanel, 0xC);
    shadowInt(findNode(tab, S_TxtPowerLabel, 0xF), power);
    shadowInt(findNode(tab, S_TxtDefenceLabel, 0x11), defence);
    const uint32_t a = api();
    const uint32_t costText = findNode(tab, S_TxtCostLabel, 0xE);
    shadowInt(costText, cost);
    if (safe32(ld32(a + API_USER_DETAIL), USER_COST_LIMIT) >= cost) call(g::SceneUtil_setNormalColor, {costText});
    else call(g::SceneUtil_setWarningColor, {costText});
    SceneUtil::setShadow(findNode(tab, S_TxtSlashLabel, 0xF));
    const uint32_t maxText = findNode(tab, S_TxtCostMaxLabel, 0x11);
    SceneUtil::setShadow(maxText);
    Frame f;
    setTextC(maxText, stringBuffer(f, {S_IntFmt, uint32_t(safe32(ld32(a + API_USER_DETAIL), USER_COST_LIMIT))}));
}
RW_FUNCTION(0x66707C, "SceneDeckEdit::updateDeckStauts", deck_edit::updateDeckStauts);

// Keyblade `index`'s page: its image and its DeckRevolverView (created the first time with the medal callbacks);
// `current`: it is the shown keyblade (the "n/m" count, its name, the deck status).
void updateDeckView(uint32_t me, uint32_t page, uint32_t index, uint32_t current) {
    if (!(keybladeCount(me) > index)) {
        call(g::VECTOR_AT_THROW, {me + KEYBLADES});
        return;
    }
    Frame frame;
    const uint32_t medals = frame.alloc(12);
    st32(medals, 0);
    st32(medals + 4, 0);
    st32(medals + 8, 0);
    const uint32_t kb = ld32(me + KEYBLADES) + KEYBLADE_SIZE * index;
    loadDeckMedals(kb, medals);
    const uint32_t kdm = call(g::KeyBladeDataManager_getInstance, {});
    const uint32_t kbData = keybladeData(frame, kdm, ld32(kb), ld32(kb + 4));
    {
        const uint32_t image = findNode(page, S_KB, 2);
        const uint32_t manager = call(g::KeyBladeDataManager_getInstance, {});
        Frame f;
        const uint32_t copy = f.alloc(8);
        st32(copy, ld32(kbData));
        st32(copy + 4, ld32(kbData + 4));
        addRef(ld32(copy + 4));
        const uint32_t path = call(g::KeyBladeDataManager_getKeyBladeImagePath, {manager, copy});
        const uint32_t s = newStr(f, path);
        call(g::ImageView_loadTexture, {image, s, 0});
        strFree(s);
        release(ld32(copy + 4));
    }
    const uint32_t center = findNode(page, S_CenterUIRevolver, 0x11);
    const uint32_t area = findNode(center, S_AreaRevolver, 0xD);
    uint32_t revolver = findNode(area, S_RevolverView, 0xC);
    if (revolver) {
        Frame f;
        const uint32_t copy = f.alloc(8);
        st32(copy, ld32(kbData));
        st32(copy + 4, ld32(kbData + 4));
        addRef(ld32(copy + 4));
        call(g::DeckRevolverView_setKeyBladeData, {revolver, copy});
        release(ld32(copy + 4));
        call(g::DeckRevolverView_setMedalDatas, {revolver, medals});
    } else {
        {
            Frame f;
            const uint32_t copy = f.alloc(8), user = f.alloc(8);
            st32(copy, ld32(kbData));
            st32(copy + 4, ld32(kbData + 4));
            addRef(ld32(copy + 4));
            st32(user, 0);
            st32(user + 4, 0);
            revolver = call(g::DeckRevolverView_create, {copy, user});
            release(ld32(user + 4));
            release(ld32(copy + 4));
        }
        st8(revolver + REV_EDITABLE, 1);
        for (uint32_t s = ld32(revolver + REV_SLOTS); s != ld32(revolver + REV_SLOTS + 4); s += 4)
            st8(ld32(s) + SLOT_EDITABLE, 1);
        call(g::DeckRevolverView_setMedalDatas, {revolver, medals});
        call(g::DeckRevolverView_setOpenMedalInfoPopupEnabled, {revolver, 1});
        st8(revolver + REV_ENABLED, 1);
        call(g::DeckRevolverView_setTargetMedalList, {revolver, findNode(ld32(me + MEDALFORM), S_WinUnder, 9)});
        swapInto(revolver + REV_ON_MEDAL_INFO, g::Swap_medalInfo, VT_MedalInfo, {me});
        swapInto(revolver + REV_ON_REMOVE, g::Swap_vUll, VT_MedalRemove, {me});
        swapInto(revolver + REV_ON_CHANGE, g::Swap_vv, VT_DeckChanged, {me, revolver});
        {
            const float w = ldf(vcall(revolver, V_GET_CONTENT_SIZE, {revolver}));
            const float h = ldf(vcall(revolver, V_GET_CONTENT_SIZE, {revolver}) + 4);
            Frame f;
            const uint32_t pos = f.alloc(8);
            call(g::Vec2_ctorXY, {pos, fbits(w * 0.5f), fbits(h * 0.5f)});
            vcall(revolver, V_SET_POSITION, {revolver, pos});
            call(g::Vec2_dtor, {pos});
            const uint32_t name = newStr(f, S_RevolverView, 0xC);
            vcall(area, node::addChildZName, {area, revolver, 0, name});
            strFree(name);
        }
    }
    if (current == 1) {
        st32(me + REVOLVER, revolver);
        {
            const uint32_t idLo = ld32(kb), idHi = ld32(kb + 4);
            const uint32_t sm = smgr();
            st32(sm + SM_KEYBLADE_ID + 4, idHi);
            st32(sm + SM_KEYBLADE_ID, idLo);
        }
        {
            const uint32_t text = ld32(me + COUNT_TEXT);
            const uint32_t at = ld32(ld32(me + SLIDE) + SLIDE_INDEX);
            Frame f;
            const uint32_t pageIds = f.alloc(12);
            call(g::BaseSlideView_getItemIDs, {pageIds, ld32(me + SLIDE)});
            const uint32_t count = uint32_t(int32_t(ld32(pageIds + 4) - ld32(pageIds)) >> 2);
            setTextC(text, stringBuffer(f, {S_CountFmt, at + 1, count}));
            call(g::IntVector_baseDtor, {pageIds});
        }
        {
            const uint32_t name = findNode(ld32(me + TOP), S_TxtKBNameLabel, 0x10);
            SceneUtil::setShadow(name);
            const uint32_t rev = ld32(me + REVOLVER);
            const uint32_t ctrl = ld32(rev + REV_KB_DATA + 4);
            const uint32_t data = ld32(rev + REV_KB_DATA);
            addRef(ctrl);
            Frame f;
            setTextC(name, safeBytes(f, ld32(data + 0x20), 4, 0x41));
            release(ctrl);
        }
        {
            const uint32_t rev = ld32(me + REVOLVER);
            Frame f;
            const uint32_t copy = f.alloc(8);
            const uint32_t top = ld32(me + TOP);
            st32(copy, ld32(rev + REV_KB_DATA));
            const uint32_t ctrl = ld32(rev + REV_KB_DATA + 4);
            st32(copy + 4, ctrl);
            addRef(ctrl);
            updateDeckStauts(rev, top, copy, ld32(me + REVOLVER) + REV_MEDALS);
            release(ctrl);
        }
    }
    release(ld32(kbData + 4));
    call(g::MedalVector_baseDtor, {medals});
}
RW_FUNCTION(0x667538, "SceneDeckEdit::updateDeckView", deck_edit::updateDeckView);

// The shown keyblade's record takes the medals of its DeckRevolverView; a changed medal id marks it changed.
void updateDeckData(uint32_t me) {
    const uint32_t a = api();
    const uint32_t slide = ld32(me + SLIDE);
    const uint32_t ids = ld32(slide + SLIDE_IDS);
    const uint32_t at = ld32(slide + SLIDE_INDEX);
    if (!(uint32_t(int32_t(ld32(slide + SLIDE_IDS + 4) - ids) >> 2) > at)) {
        call(g::VECTOR_AT_THROW, {slide + SLIDE_IDS});
        return;
    }
    const uint32_t index = ld32(ids + 4 * at);
    Frame f;
    const uint32_t old = safeBytes(f, ld32(a + API_USER_KEYBLADES), KEYBLADE_SIZE * index, KEYBLADE_SIZE);
    const uint32_t rev = ld32(me + REVOLVER);
    const uint32_t medals = rev + REV_MEDALS;
    st32(ld32(me + KEYBLADES) + KEYBLADE_SIZE * index + KB_MEDAL_COUNT,
         uint32_t(int32_t(ld32(medals + 4) - ld32(medals)) >> 3));
    for (uint32_t n = 0; n < ld32(ld32(me + KEYBLADES) + KEYBLADE_SIZE * index + KB_MEDAL_COUNT); n++) {
        if (!(uint32_t(int32_t(ld32(medals + 4) - ld32(medals)) >> 3) > n)) {
            call(g::VECTOR_AT_THROW, {medals});
            return;
        }
        const uint32_t medal = ld32(ld32(medals) + 8 * n);
        const uint32_t dst = ld32(me + KEYBLADES) + KEYBLADE_SIZE * index + KB_MEDAL_IDS + 8 * n;
        st32(dst, ld32(medal));
        st32(dst + 4, ld32(medal + 4));
        const uint32_t now = ld32(me + KEYBLADES) + KEYBLADE_SIZE * index + KB_MEDAL_IDS + 8 * n;
        const uint32_t was = old + KB_MEDAL_IDS + 8 * n;
        if (ld32(now) != ld32(was) || ld32(now + 4) != ld32(was + 4)) st8(ld32(me + CHANGED) + index, 1);
    }
}
RW_FUNCTION(0x667EF8, "SceneDeckEdit::updateDeckData", deck_edit::updateDeckData);

// Send the changed keyblades (the shown one becomes the equipped one) with executeUserKeybladeUpdate; `callback`
// gets the result. Nothing changed: the callback runs at once. `loading`: with the loading indicator (else the
// header's back is blocked). In the deck tutorial (phase 220) the callback is wrapped in three nested lambdas.
void runUpdateKeybladeEvent(uint32_t me, uint32_t callback, uint32_t loading) {
    const uint32_t slide = ld32(me + SLIDE);
    if (!(uint32_t(int32_t(ld32(slide + SLIDE_IDS + 4) - ld32(slide + SLIDE_IDS)) >> 2) >
          ld32(slide + SLIDE_INDEX))) {
        call(g::VECTOR_AT_THROW, {slide + SLIDE_IDS});
        return;
    }
    const uint32_t shown = ld32(ld32(slide + SLIDE_IDS) + 4 * ld32(slide + SLIDE_INDEX));
    if (shown != ld32(me + EQUIPPED)) {
        st8(ld32(me + CHANGED) + shown, 1);
        if (!(keybladeCount(me) > shown)) {
            call(g::VECTOR_AT_THROW, {me + KEYBLADES});
            return;
        }
        st8(ld32(me + KEYBLADES) + KEYBLADE_SIZE * shown + KB_EQUIPPED, 1);
        st8(ld32(me + CHANGED) + ld32(me + EQUIPPED), 1);
        if (!(keybladeCount(me) > ld32(me + EQUIPPED))) {
            call(g::VECTOR_AT_THROW, {me + KEYBLADES});
            return;
        }
        st8(ld32(me + KEYBLADES) + KEYBLADE_SIZE * ld32(me + EQUIPPED) + KB_EQUIPPED, 0);
    }
    Frame frame;
    const uint32_t changed = frame.alloc(12);
    st32(changed, 0);
    st32(changed + 4, 0);
    st32(changed + 8, 0);
    for (uint32_t n = 0; ld32(me + KEYBLADES + 4) != ld32(me + KEYBLADES) && n < keybladeCount(me); n++) {
        if (!ld8(ld32(me + CHANGED) + n)) continue;
        const uint32_t kb = ld32(me + KEYBLADES) + KEYBLADE_SIZE * n;
        st8(kb + KB_EQUIPPED, n == ld32(ld32(me + SLIDE) + SLIDE_INDEX) ? 1 : 0);
        pushKeyblade(changed, kb);
    }
    // {loading, callback}
    const uint32_t outer = frame.alloc(0x20);
    st8(outer, loading);
    const uint32_t cb = outer + 8;
    call(g::Function_vb_copy, {cb, callback});
    if (ld32(changed + 4) == ld32(changed)) {
        call(g::SceneManager_enableLoadingIndicator, {smgr(), 0});
        call(g::Function_vb_call, {cb, 1});
    } else {
        if (loading == 1) {
            call(g::SceneManager_enableLoadingIndicator, {smgr(), 1});
        } else {
            const uint32_t sm = smgr();
            Frame f;
            const uint32_t none = newEmptyFunction(f);
            st8(sm + SM_BACK_FLAG, 1);
            call(g::Function_vb_assign, {sm + SM_BACK_CALLBACK, none});
            functionDestroy(none);
        }
        if (SceneUtil::getNexTutorialphase() != PHASE_DECK_TUTORIAL) {
            const uint32_t a = api();
            Frame f;
            const uint32_t copy = f.alloc(20);
            functionCopy(copy, cb);
            const uint32_t fo = f.alloc(20);
            const uint32_t heap = heapFunction(fo, VT_KeybladeUpdated, 0x28, 0x10, g::Function_vb_move, copy);
            if (heap) st8(heap + 8, loading);
            call(g::APIManager_executeUserKeybladeUpdate, {a, changed, fo});
            functionDestroy(fo);
            functionDestroy(copy);
        } else {
            // the nested lambdas' copies of the callback, in the original's order
            Frame f;
            uint32_t c[10];
            for (uint32_t k = 1; k <= 9; k++) c[k] = f.alloc(20);
            functionCopy(c[1], cb);
            functionCopy(c[2], cb);
            functionCopy(c[3], c[1]);
            functionCopy(c[4], cb);
            functionCopy(c[5], c[2]);
            functionCopy(c[6], c[3]);
            const uint32_t a = api();
            functionCopy(c[7], c[4]);
            functionCopy(c[8], c[5]);
            functionCopy(c[9], c[6]);
            const uint32_t fo = f.alloc(20);
            st32(fo + 16, 0);
            const uint32_t heap = call(fn::operator_new, {0x80});
            if (heap) {
                st32(heap, VT_KeybladeUpdatedTutorial);
                for (uint32_t k = 0; k < 3; k++) {
                    st8(heap + 8 + 0x28 * k, loading);
                    st8(heap + 0x10 + 0x28 * k, loading);
                    call(g::Function_vb_move, {heap + 0x18 + 0x28 * k, c[7 + k]});
                }
            }
            st32(fo + 16, heap);
            call(g::APIManager_executeUserKeybladeUpdate, {a, changed, fo});
            functionDestroy(fo);
            for (uint32_t k = 9; k >= 1; k--) functionDestroy(c[k]);
        }
    }
    functionDestroy(cb);
    call(g::KeybladeVector_baseDtor, {changed});
}
RW_FUNCTION(0x668044, "SceneDeckEdit::runUpdateKeybladeEvent", deck_edit::runUpdateKeybladeEvent);

// updateDeckView $_ (the DeckRevolverView's change callback {this, revolver}) __func::__clone(dst)
void cloneDeckChanged(uint32_t self, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_DeckChanged);
    const uint32_t me = ld32(self + 4), revolver = ld32(self + 8);
    st32(dst + 8, revolver);
    st32(dst + 4, me);
}
RW_FUNCTION(0x677E00, "SceneDeckEdit::updateDeckView $_ __func::__clone(dst) (sub_677E00)", deck_edit::cloneDeckChanged);

}  // namespace deck_edit
