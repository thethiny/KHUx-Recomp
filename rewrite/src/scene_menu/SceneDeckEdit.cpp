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
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base, Layer_dtor = rwlib::cocos2d::Layer::dtor_base, Layer_init = rwlib::cocos2d::Layer::init;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t MEMSET = 0x40E7A0;                       // __aeabi_memset(dest, n, c) (PLT)
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__v, Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
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
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t BaseSlideView_create = 0x534E79, BaseSlideView_setIndex = 0x526141;
constexpr uint32_t BaseSlideView_setItemIDs = 0x5260AD, BaseSlideView_getItemIDs = 0x525EA5;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_setTargetAnchorPoint = 0x5318CF, ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
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

// ------------------------------------------------------------------------------------------------ init's lambdas
namespace {
namespace lam {
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;      // (id, fade ms)
constexpr uint32_t AudioManager_getInstance = 0x5BECFD, AudioManager_load = 0x5BEEF5;   // (manager, ResourceID)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t AppState_getTutorialCloseFunc = 0x4EBA3D;   // (function out, app, popup id)
constexpr uint32_t AppState_execTutorialCloseFunc = 0x4EBA5D;  // (app, popup id)
constexpr uint32_t SceneDeckEdit_openMedalSelectPanel = 0x6652C1;
constexpr uint32_t SceneDeckEdit_updateDeckView = 0x667539;    // (scene, page, index, bool current)
constexpr uint32_t SceneDeckEdit_runUpdateKeybladeEvent = 0x668045;
constexpr uint32_t SceneManager_isOpenedPopup = 0x621519;
constexpr uint32_t FunctionBool_call = 0x51022D;               // std::function<bool()>::operator()
constexpr uint32_t FunctionWidgetInt_call = 0x54A065;          // std::function<Widget*(int)>::operator()(int)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5; // (mixin, function<void()>)
constexpr uint32_t SceneUtil_clearMedalNewMark = 0x46C4B5;
constexpr uint32_t SceneMixedMedal_callPrepareAPI = 0x69F475;  // (function<void(bool)> by value)
constexpr uint32_t BaseSlideView_changeNextPage = 0x526245, BaseSlideView_changePreviousPage = 0x5263ED;
constexpr uint32_t BGM_DECK_EDIT = 501000920, BGM_FADE = 500, SE_PAGE = 509000170, SE_MEDAL_SET = 509000050;
constexpr uint32_t TUTORIAL_MEDAL = 110, TUTORIAL_KEYBLADE = 113;
constexpr uint32_t SLIDE_MOVING = 0x5C0, SLIDE_DRAGGING = 0x5C1;   // BaseSlideView: uint8 each
constexpr uint32_t INOUT = 0x288;                              // SceneDeckEdit: its IInOutAnimation
constexpr uint32_t NEW_MARKS = 0x37C;                          // uint8: clear the medals' "new" marks on leaving
constexpr uint32_t MIX_PREPARE = 0x3A0;                        // uint8: the mixed medal scene needs its prepare API
constexpr uint32_t SM_LIST_SCROLL = 0x2A0, SM_LIST_INDEX = 0x2A4;   // SceneManager: the medal list's saved position
constexpr uint32_t GOT_CREATE_MEDAL_LIST = 0xFD60BC;           // GOT: createSceneLayer<SceneMedalList>
constexpr uint32_t VT_ListLeave = 0xF63DB8;                    // $_5's lambda() (no captures)
constexpr uint32_t VT_FinishDone = 0xF63C84;                   // $_11's lambda(bool) (heap 0x28)
constexpr uint32_t S_ScrollJson = 0xE0448B, S_CenterUIKB = 0xE044B8;   // "cocostudio/publish/DeckEditScene_Scroll.json"

// the deck page can take a touch: ready, the slide view not moving or dragged
bool pageIdle(uint32_t me) {
    if (!ld8(me + READY)) return false;
    const uint32_t slide = ld32(me + SLIDE);
    return !ld8(slide + SLIDE_MOVING) && !ld8(slide + SLIDE_DRAGGING);
}

// a tutorial popup `id` waiting to be closed: its close callback taken (and dropped), then closed through AppState
void closeTutorialPopup(uint32_t id) {
    Frame f;
    const uint32_t fn = f.alloc(20);
    call(AppState_getTutorialCloseFunc, {fn, call(fn::AppState_getInstance, {}), id});
    const bool waiting = ld32(fn + 16) != 0;
    functionDestroy(fn);
    if (waiting) call(AppState_execTutorialCloseFunc, {call(fn::AppState_getInstance, {}), id});
}
}  // namespace lam

// $_0 onEnter {scene}: the update schedule, the deck BGM, the page / medal sounds loaded
void onEnter(uint32_t self) {
    call(lam::Node_scheduleUpdate, {ld32(self + 4)});
    call(lam::AudioReplaceGameMainBGM, {lam::BGM_DECK_EDIT, lam::BGM_FADE});
    call(lam::AudioManager_load, {call(lam::AudioManager_getInstance, {}), lam::SE_PAGE});
    call(lam::AudioManager_load, {call(lam::AudioManager_getInstance, {}), lam::SE_MEDAL_SET});
}

// $_3 Button_Medal {scene}(sender, type): ended on an idle page -> the medal tutorial popup closed, the medal panel
void medalTouched(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return;
    const uint32_t me = ld32(self + 4);
    if (!lam::pageIdle(me)) return;
    call(lam::AudioPlaySystemSEDecide, {});
    lam::closeTutorialPopup(lam::TUTORIAL_MEDAL);
    call(lam::SceneDeckEdit_openMedalSelectPanel, {me});
}

// $_4 Button_KB {scene}(sender, type): ended on an idle page -> the keyblade tutorial popup closed, the keyblade
// panel
void keybladeTouched(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return;
    const uint32_t me = ld32(self + 4);
    if (!lam::pageIdle(me)) return;
    lam::closeTutorialPopup(lam::TUTORIAL_KEYBLADE);
    call(lam::AudioPlaySystemSEDecide, {});
    call(g::SceneDeckEdit_openKeyBladePanel, {me, 0});
}

// $_5 Button_List {scene}(sender, type): ended on an idle page with no popup, when the scene may finish -> the
// buttons off, the scene animates out, then the medal list ($_5's lambda)
void listTouched(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return;
    const uint32_t me = ld32(self + 4);
    if (!lam::pageIdle(me)) return;
    if (call(lam::SceneManager_isOpenedPopup, {smgr()})) return;
    call(lam::AudioPlaySystemSEDecide, {});
    if (call(lam::FunctionBool_call, {me + PREPARE_FINISH}) != 1) return;
    st8(me + READY, 0);
    for (uint32_t b : {BUTTON_MEDAL, BUTTON_LIST, BUTTON_KB, BUTTON_SORT})
        vcall(ld32(me + b), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + b), 0});
    Frame f;
    const uint32_t then = newFunction(f, lam::VT_ListLeave, {});
    call(lam::IInOutAnimation_runOutAnimation, {me + lam::INOUT, then});
    functionDestroy(then);
}
// $_5's lambda(): the medal list's saved position reset, then the medal list scene
void listLeave() {
    const uint32_t sm = smgr();
    st32(sm + lam::SM_LIST_INDEX, 0);
    st32(sm + lam::SM_LIST_SCROLL, 0);
    changeSceneLayerTo(lam::GOT_CREATE_MEDAL_LIST, 1, 0, 0);
}

// $_6 / $_7 the arrows {scene}(sender, type): touch began -> the page sound, the slide view turns
template <uint32_t Turn> void arrowTouched(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 0) return;
    const uint32_t me = ld32(self + 4);
    playLayerSE(lam::SE_PAGE);
    call(Turn, {ld32(me + SLIDE)});
}

// $_8 the slide view's page factory (int): DeckEditScene_Scroll.json with the revolver, keyblade and top panels'
// Panel_1 not touchable
uint32_t makePage(uint32_t /*self*/, uint32_t /*index*/) {
    Frame f;
    const uint32_t file = newStr(f, lam::S_ScrollJson, 44);
    const uint32_t page = SceneUtil::createNodeWithSceneFile(file);
    strFree(file);
    for (const auto &[name, len] : {std::pair{S_CenterUIRevolver, 17u}, std::pair{lam::S_CenterUIKB, 11u},
                                    std::pair{S_CenterUITop, 12u}}) {
        const uint32_t panel = findNode(findNode(page, name, len), S_Panel1, 7);
        vcall(panel, V_WIDGET_SET_TOUCH_ENABLED, {panel, 0});
    }
    return page;
}

// $_9 the slide view's page updater {scene}(page, index, current): a missing page from the factory, then the deck
// view of keyblade `index` on it
uint32_t updatePage(uint32_t self, uint32_t pagep, uint32_t indexp, uint32_t currentp) {
    const uint32_t me = ld32(self + 4);
    const uint32_t current = ld8(currentp);
    const uint32_t index = ld32(indexp);
    uint32_t page = ld32(pagep);
    if (!page) {
        page = call(lam::FunctionWidgetInt_call, {ld32(me + SLIDE) + SLIDE_MAKE_PAGE, index});
        call(rwlib::cocos2d::Ref::retain, {page});
    }
    call(lam::SceneDeckEdit_updateDeckView, {me, page, index, current ? 1u : 0u});
    return page;
}

// $_11 the scene's finish callback {scene}(function<void(bool)> done): the medals' new marks cleared when wanted, the
// finish callback dropped, then the changed keyblades sent (runUpdateKeybladeEvent) with $_11's lambda(bool)
// {done, scene} as its result
void finish(uint32_t self, uint32_t donep) {
    Frame f;
    const uint32_t done = f.alloc(20);
    call(g::Function_vb_move, {done, donep});
    const uint32_t me = ld32(self + 4);
    if (ld8(me + lam::NEW_MARKS)) call(lam::SceneUtil_clearMedalNewMark, {});
    const uint32_t sm = smgr();
    {
        const uint32_t none = newEmptyFunction(f);
        call(g::SceneManager_setFinishSceneCallback, {sm, none});
        functionDestroy(none);
    }
    const uint32_t copy = f.alloc(20);
    functionCopy(copy, done);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, lam::VT_FinishDone);
        call(g::Function_vb_move, {obj + 8, copy});
        st32(obj + 0x20, me);
    }
    st32(fo + 16, obj);
    call(lam::SceneDeckEdit_runUpdateKeybladeEvent, {me, fo, 0});
    functionDestroy(fo);
    functionDestroy(copy);
    functionDestroy(done);
}
// $_11's lambda(bool) {-, done, scene}(sent): failed -> done(false); sent -> the back key's callback emptied, then
// the mixed medal scene's prepare API first when it needs one (done goes along), else done(true)
void finishDone(uint32_t self, uint32_t sentp) {
    if (!ld8(sentp)) {
        call(g::Function_vb_call, {self + 8, 0});
        return;
    }
    const uint32_t me = ld32(self + 0x20);
    const uint32_t sm = smgr();
    st8(sm + SM_BACK_FLAG, 1);
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(g::Function_vb_assign, {sm + SM_BACK_CALLBACK, none});
        functionDestroy(none);
    }
    if (!ld8(me + lam::MIX_PREPARE)) {
        call(g::Function_vb_call, {self + 8, 1});
        return;
    }
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(g::Function_vb_copy, {copy, self + 8});
    call(lam::SceneMixedMedal_callPrepareAPI, {copy});
    functionDestroy(copy);
}
}  // namespace

RW_FUNCTION(0x67CA0C, "SceneDeckEdit::init::$_0 operator() (onEnter)", onEnter);
RW_FUNCTION(0x67C794, "SceneDeckEdit::init::$_3 operator() (Button_Medal)", medalTouched);
RW_FUNCTION(0x67C654, "SceneDeckEdit::init::$_4 operator() (Button_KB)", keybladeTouched);
RW_FUNCTION(0x67C334, "SceneDeckEdit::init::$_5 operator() (Button_List)", listTouched);
RW_FUNCTION(0x67C4FC, "SceneDeckEdit::init::$_5::lambda() operator() (to the medal list)", listLeave);
RW_FUNCTION(0x67C20C, "SceneDeckEdit::init::$_6 operator() (left arrow)", arrowTouched<lam::BaseSlideView_changePreviousPage>);
RW_FUNCTION(0x67C0E4, "SceneDeckEdit::init::$_7 operator() (right arrow)", arrowTouched<lam::BaseSlideView_changeNextPage>);
RW_FUNCTION(0x67BD70, "SceneDeckEdit::init::$_8 operator() (page factory)", makePage);
RW_FUNCTION(0x67BC9A, "SceneDeckEdit::init::$_9 operator() (page updater)", updatePage);
RW_FUNCTION(0x67B748, "SceneDeckEdit::init::$_11 operator() (finish)", finish);
RW_FUNCTION(0x67BA7C, "SceneDeckEdit::init::$_11::lambda(bool) operator() (keyblades sent)", finishDone);

// ------------------------------------------------------------------------------------------------ the tutorial and
// panel lambdas
namespace {
namespace lam2 {
constexpr uint32_t BaseSlideView_updateLayout = 0x525EB5, BaseSlideView_setSlideEnabled = 0x526111;
constexpr uint32_t TutorialMisc_create = 0x777BB5;            // master::TutorialMisc::create(shared_ptr out, id)
constexpr uint32_t AppState_isClearTutorialPopupIDs = 0x4EB815;   // (app, popup id) -> bool
constexpr uint32_t AppState_setTutorialCloseFunc = 0x4EB8D9;  // (function out, app, function close, popup id)
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;    // (function out, id, function onClosed, Node* target, 3 x 0)
constexpr uint32_t SceneDeckEdit_setKeybladeArrowVisible = 0x663DD9;
constexpr uint32_t SceneDeckEdit_onTapMedalInfoButton = 0x667DC1; // (scene, type, bool, -, u64 user medal id)
constexpr uint32_t DeckRevolverView_getTouchMedalIndex = 0x52D605;     // (view, Vec2, bool) -> int
constexpr uint32_t DeckRevolverView_getTouchMedalPosition = 0x52D7B9;  // (Vec2 out, view, Vec2)
constexpr uint32_t Vec2_copy = 0x8E4C03, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SafeBuffer_read = 0x45A269;                // (sb, dst, src, len, offset)
constexpr uint32_t TUTORIAL_MISC_KEYBLADES = 5;                // TutorialMisc 5 +0xC: the keyblades a deck tutorial wants
constexpr uint32_t TUTORIAL_MEDAL_DRAG = 111, TUTORIAL_REVOLVER = 115;
constexpr uint32_t SM_KEYBLADE_PANEL = 0x2E0;                 // SceneManager: uint8, the keyblade panel is open
constexpr uint32_t API_KEYBLADES = 0xEC;                      // APIManager: shared_ptr<SafeBuffer> user keyblades
constexpr uint32_t REV_SLOT_NODES = 0x32C;                    // DeckRevolverView: vector<Node*> the medal slots
constexpr uint32_t VT_RevolverTutorial = 0xF63BD4;            // $_13's lambda()#2 {scene}
constexpr uint32_t VT_FewKeyblades = 0xF63C00;                // $_13's lambda()#1 (no captures)
constexpr uint32_t VT_PanelClosed = 0xF639C4;                 // openKeyBladePanel::$_98's lambda() {scene, view, scale}

// SceneUtil::openTutorialPopup(id, no callback, target) and its close function handed to AppState; true when
// AppState kept one (the popup is up)
bool tutorialPopupOn(uint32_t id, uint32_t target) {
    const uint32_t app = call(fn::AppState_getInstance, {});
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    const uint32_t close = f.alloc(20);
    call(SceneUtil_openTutorialPopup, {close, id, none, target, 0, 0, 0});
    const uint32_t kept = f.alloc(20);
    call(AppState_setTutorialCloseFunc, {kept, app, close, id});
    const bool up = ld32(kept + 16) != 0;
    functionDestroy(kept);
    functionDestroy(close);
    functionDestroy(none);
    return up;
}
bool tutorialCleared(uint32_t id) {
    return call(AppState_isClearTutorialPopupIDs, {call(fn::AppState_getInstance, {}), id}) == 1;
}
}  // namespace lam2

// runInAnimation::$_13 {scene}: the slide view laid out; with fewer keyblades than the deck tutorial needs (TutorialMisc
// 5) its popup 22; else popup 28 (then the revolver tutorial) and the medal / keyblade tutorials (110 on
// Button_Medal while 14 is not cleared, 113 on Button_KB once 18 is), each locking the other button
void slideIn(uint32_t self) {
    const uint32_t me = ld32(self + 4);
    call(lam2::BaseSlideView_updateLayout, {ld32(me + SLIDE), 1});
    Frame f;
    const uint32_t row = f.alloc(8);
    call(lam2::TutorialMisc_create, {row, lam2::TUTORIAL_MISC_KEYBLADES});
    uint32_t wanted = 0;
    if (ld32(row)) {
        const uint32_t v = f.alloc(4);
        call(lam2::SafeBuffer_read, {ld32(row), v, ld32(ld32(row) + 0xC) + 0xC, 4, 0xC});
        wanted = ld32(v);
    }
    const uint32_t a = api();
    const uint32_t ctrl = ld32(a + lam2::API_KEYBLADES + 4), keyblades = ld32(a + lam2::API_KEYBLADES);
    addRef(ctrl);
    if (wanted > ld32(keyblades + 0x20)) {
        {
            Frame t;
            const uint32_t then = newFunction(t, lam2::VT_RevolverTutorial, {me});
            const uint32_t close = t.alloc(20);
            call(lam2::SceneUtil_openTutorialPopup, {close, 28, then, 0, 0, 0, 0});
            functionDestroy(close);
            functionDestroy(then);
        }
        if (!lam2::tutorialCleared(14) && lam2::tutorialPopupOn(110, ld32(me + BUTTON_MEDAL)))
            vcall(ld32(me + BUTTON_KB), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + BUTTON_KB), 0});
        if (lam2::tutorialCleared(18) && lam2::tutorialPopupOn(113, ld32(me + BUTTON_KB)))
            vcall(ld32(me + BUTTON_MEDAL), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + BUTTON_MEDAL), 0});
    } else {
        Frame t;
        const uint32_t then = newFunction(t, lam2::VT_FewKeyblades, {});
        const uint32_t close = t.alloc(20);
        call(lam2::SceneUtil_openTutorialPopup, {close, 22, then, 0, 0, 0, 0});
        functionDestroy(close);
        functionDestroy(then);
    }
    release(ctrl);
    release(ld32(row + 4));
}

// $_13's lambda()#2 {scene}: once tutorial 28 is cleared, popup 115 on the revolver's first medal slot; up -> both
// buttons locked
void revolverTutorial(uint32_t self) {
    const uint32_t me = ld32(self + 4);
    if (!lam2::tutorialCleared(28)) return;
    const uint32_t revolver = ld32(me + REVOLVER);
    const uint32_t slots = revolver + lam2::REV_SLOT_NODES;
    if (ld32(slots + 4) == ld32(slots)) call(g::VECTOR_AT_THROW, {});
    if (!lam2::tutorialPopupOn(lam2::TUTORIAL_REVOLVER, ld32(ld32(slots)))) return;
    vcall(ld32(me + BUTTON_KB), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + BUTTON_KB), 0});
    vcall(ld32(me + BUTTON_MEDAL), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + BUTTON_MEDAL), 0});
}

// initKeyBladePanel::$_16 / initMedalSelectPanel::$_19 the panels' close buttons {scene}(sender, type): ended (the
// keyblade panel: on an idle page) -> the current popup closed
void keybladePanelClose(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2 || !lam::pageIdle(ld32(self + 4))) return;
    call(g::SceneManager_closeCurrentPopup, {smgr(), 0});
}
void medalPanelClose(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2 || !ld8(ld32(self + 4) + READY)) return;
    call(g::SceneManager_closeCurrentPopup, {smgr(), 0});
}

// openKeyBladePanel::$_18 {scene, view, button, button, scale}(closed by OK): closed otherwise -> (when the scene may
// finish) the cancel sound and the panel flag off; the arrows shown, three widgets off, the panel shrinks and
// $_98's lambda restores the scene
void keybladePanelClosed(uint32_t self, uint32_t byOkp) {
    if (ld8(byOkp)) return;
    const uint32_t me = ld32(self + 4);
    if (ld8(me + CAN_FINISH)) {
        call(lam2::AudioPlaySystemSECancel, {});
        st8(smgr() + lam2::SM_KEYBLADE_PANEL, 0);
    }
    call(lam2::SceneDeckEdit_setKeybladeArrowVisible, {me, 1});
    for (uint32_t k : {8u, 0xCu, 0x10u}) vcall(ld32(self + k), V_WIDGET_SET_TOUCH_ENABLED, {ld32(self + k), 0});
    const uint32_t view = ld32(self + 8);
    Frame f;
    const uint32_t done = newFunction(f, lam2::VT_PanelClosed, {me, view, ld32(self + 0x14)});
    call(fn::Node_runAction, {view, SceneUtil::createClosePopupAnimation(view, done)});
    functionDestroy(done);
}
// openKeyBladePanel::$_98's lambda() {scene, view, scale}: the strength popup hidden, the view's scale back, the
// slide view and the medal / keyblade buttons on again
void keybladePanelRestored(uint32_t self) {
    const uint32_t me = ld32(self + 4);
    setVisible(ld32(me + KBSTRENGTH), 0);
    const uint32_t view = ld32(self + 8);
    vcall(view, node::setScale, {view, ld32(self + 0xC)});
    call(lam2::BaseSlideView_setSlideEnabled, {ld32(me + SLIDE), 1});
    vcall(ld32(me + BUTTON_MEDAL), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + BUTTON_MEDAL), 1});
    vcall(ld32(me + BUTTON_KB), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + BUTTON_KB), 1});
}

// initMedalSelectPanel::$_20's lambda(MedalInfoButtonType, bool) {-, scene, -, user medal id}: the medal info popup's
// button -> onTapMedalInfoButton(type, false, id)
void medalInfoButton(uint32_t self, uint32_t typep, uint32_t) {
    const uint32_t me = ld32(self + 8);
    call(lam2::SceneDeckEdit_onTapMedalInfoButton, {me, ld32(typep), 0, me, ld32(self + 0x10), ld32(self + 0x14)});
}

// initMedalSelectPanel::$_21 {scene}(user medal id, touch, float* position out): the revolver's medal slot under the
// touch (its position out when there is one); -1 while the drag tutorial (111) waits and the slot is not the first
uint32_t medalDragTarget(uint32_t self, uint32_t, uint32_t touchp, uint32_t outp) {
    Frame f;
    const uint32_t touch = f.alloc(8);
    call(lam2::Vec2_copy, {touch, touchp});
    const uint32_t out = ld32(outp);
    const uint32_t me = ld32(self + 4);
    const uint32_t revolver = ld32(me + REVOLVER);
    const uint32_t tmp = f.alloc(8);
    call(lam2::Vec2_copy, {tmp, touch});
    const int32_t index = int32_t(call(lam2::DeckRevolverView_getTouchMedalIndex, {revolver, tmp, 1}));
    call(lam2::Vec2_dtor, {tmp});
    if (index >= 0) {
        const uint32_t rev = ld32(me + REVOLVER);
        const uint32_t at = f.alloc(8), pos = f.alloc(8);
        call(lam2::Vec2_copy, {at, touch});
        call(lam2::DeckRevolverView_getTouchMedalPosition, {pos, rev, at});
        call(lam2::Vec2_dtor, {at});
        st32(out, ld32(pos));
        st32(out + 4, ld32(pos + 4));
        call(lam2::Vec2_dtor, {pos});
    }
    const uint32_t closeFn = f.alloc(20);
    call(lam::AppState_getTutorialCloseFunc, {closeFn, call(fn::AppState_getInstance, {}), lam2::TUTORIAL_MEDAL_DRAG});
    const bool waiting = ld32(closeFn + 16) != 0;
    functionDestroy(closeFn);
    call(lam2::Vec2_dtor, {touch});
    return (waiting && index != 0) ? 0xFFFFFFFFu : uint32_t(index);
}
}  // namespace

RW_FUNCTION(0x67B038, "SceneDeckEdit::runInAnimation::$_13 operator() (slid in)", slideIn);
RW_FUNCTION(0x67B430, "SceneDeckEdit::runInAnimation::$_13::lambda()#2 operator() (revolver tutorial)", revolverTutorial);
RW_FUNCTION(0x679D54, "SceneDeckEdit::initKeyBladePanel::$_16 operator() (Close_Button)", keybladePanelClose);
RW_FUNCTION(0x679954, "SceneDeckEdit::initMedalSelectPanel::$_19 operator() (Close_Button)", medalPanelClose);
RW_FUNCTION(0x679A1C, "SceneDeckEdit::openKeyBladePanel::$_18 operator() (panel closed)", keybladePanelClosed);
RW_FUNCTION(0x679BCA, "SceneDeckEdit::openKeyBladePanel::$_98::lambda() operator() (panel gone)", keybladePanelRestored);
RW_FUNCTION(0x679896, "SceneDeckEdit::initMedalSelectPanel::$_20::lambda(MedalInfoButtonType, bool) operator()", medalInfoButton);
RW_FUNCTION(0x679310, "SceneDeckEdit::initMedalSelectPanel::$_21 operator() (drag target)", medalDragTarget);

// ------------------------------------------------------------------------------------------------ the medal panel
namespace {
namespace lam3 {
constexpr uint32_t AppState_removeNewUserMedalID = 0x4EB2C5;   // (app, -, u64 user medal id)
constexpr uint32_t DeckRevolverView_setMedalData = 0x52D981;   // (view, slot, shared_ptr<UserMedalData>, bool, bool)
constexpr uint32_t SceneDeckEdit_updateDeckStauts = 0x66707D;  // (-, page, shared_ptr<UserKeyBladeData>, medals)
constexpr uint32_t SceneDeckEdit_updateDeckData = 0x667EF9;
constexpr uint32_t SceneUtil_openMedalInfoPopup = 0x466EF5;    // (data, bool, bool, bool, buttons fn, 0, closed fn, 0)
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t REV_KEYBLADE = 0x2B8;                       // DeckRevolverView: shared_ptr<UserKeyBladeData>
constexpr uint32_t API_USER = 0x94;                            // APIManager: shared_ptr<SafeBuffer> user detail
constexpr uint32_t VT_InfoButton = 0xF63940;                   // $_20's lambda(MedalInfoButtonType, bool) (heap 0x18)
}  // namespace lam3

// initMedalSelectPanel::$_22 {scene}(slot, user medal id): the medal's "new" mark dropped, the medal set on the
// revolver's slot, the deck status refreshed; slot 0 during the drag tutorial (111) -> the deck data updated and the
// tutorial popup closed
void medalSet(uint32_t self, uint32_t slotp, uint32_t idp) {
    const uint32_t slot = ld32(slotp);
    const uint32_t lo = ld32(idp), hi = ld32(idp + 4);
    const uint32_t me = ld32(self + 4);
    call(lam3::AppState_removeNewUserMedalID, {call(fn::AppState_getInstance, {}), 0, lo, hi});
    Frame f;
    const uint32_t data = medalData(f, call(g::MedalDataManager_getInstance, {}), lo, hi);
    const uint32_t revolver = ld32(me + REVOLVER);
    const uint32_t copy = f.alloc(8);
    st32(copy, ld32(data));
    st32(copy + 4, ld32(data + 4));
    addRef(ld32(copy + 4));
    call(lam3::DeckRevolverView_setMedalData, {revolver, slot, copy, 1, 0});
    release(ld32(copy + 4));
    const uint32_t page = ld32(me + TOP);
    const uint32_t rev = ld32(me + REVOLVER);
    const uint32_t keyblade = f.alloc(8);
    const uint32_t ctrl = ld32(rev + lam3::REV_KEYBLADE + 4);
    st32(keyblade, ld32(rev + lam3::REV_KEYBLADE));
    st32(keyblade + 4, ctrl);
    addRef(ctrl);
    const uint32_t rev2 = ld32(me + REVOLVER);
    call(lam3::SceneDeckEdit_updateDeckStauts, {rev2, page, keyblade, rev2 + REV_MEDALS});
    release(ctrl);
    if (!slot) {
        const uint32_t closeFn = f.alloc(20);
        call(lam::AppState_getTutorialCloseFunc, {closeFn, call(fn::AppState_getInstance, {}), lam2::TUTORIAL_MEDAL_DRAG});
        if (ld32(closeFn + 16)) {
            functionDestroy(closeFn);
            call(lam3::SceneDeckEdit_updateDeckData, {me});
            call(lam::AppState_execTutorialCloseFunc, {call(fn::AppState_getInstance, {}), lam2::TUTORIAL_MEDAL_DRAG});
        }
    }
    release(ld32(data + 4));
}

// initMedalSelectPanel::$_20 {scene}(user medal id, -): unless the drag tutorial (111) waits, the medal's info popup
// (its buttons through a heap lambda {scene, 0, id}; the info shows the deck's medal state from the user byte 0x54)
void medalSelected(uint32_t self, uint32_t idp, uint32_t) {
    const uint32_t lo = ld32(idp), hi = ld32(idp + 4);
    const uint32_t me = ld32(self + 4);
    {
        Frame f;
        const uint32_t closeFn = f.alloc(20);
        call(lam::AppState_getTutorialCloseFunc, {closeFn, call(fn::AppState_getInstance, {}), lam2::TUTORIAL_MEDAL_DRAG});
        if (ld32(closeFn + 16)) {
            functionDestroy(closeFn);
            return;
        }
    }
    call(lam::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t data = medalData(f, call(g::MedalDataManager_getInstance, {}), lo, hi);
    const uint32_t a = api();
    const uint32_t copy = f.alloc(8);
    st32(copy, ld32(data));
    st32(copy + 4, ld32(data + 4));
    addRef(ld32(copy + 4));
    const uint32_t flag = f.alloc(4);
    const uint32_t user = ld32(a + lam3::API_USER);
    call(lam3::SafeBuffer_read, {user, flag, ld32(user + 0xC) + 0x54, 1, 0x54});
    const uint32_t shown = ld8(flag);
    const uint32_t buttons = f.alloc(20);
    st32(buttons + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x18});
    if (obj) {
        st32(obj, lam3::VT_InfoButton);
        st32(obj + 8, me);
        st32(obj + 0xC, 0);
        st32(obj + 0x10, lo);
        st32(obj + 0x14, hi);
    }
    st32(buttons + 16, obj);
    const uint32_t none = newEmptyFunction(f);
    call(lam3::SceneUtil_openMedalInfoPopup, {copy, shown ? 1u : 0u, 1, 1, buttons, 0, none, 0});
    functionDestroy(none);
    functionDestroy(buttons);
    release(ld32(copy + 4));
    release(ld32(data + 4));
}
}  // namespace

RW_FUNCTION(0x678FD8, "SceneDeckEdit::initMedalSelectPanel::$_22 operator() (medal set)", medalSet);
RW_FUNCTION(0x6795D8, "SceneDeckEdit::initMedalSelectPanel::$_20 operator() (medal selected)", medalSelected);

// ------------------------------------------------------------------------------------------------ keyblade strengthening
// initKeyBladePanel::$_15 (Power Up) and its lambdas. The captured keyblade record (UserKeyblade::Keyblade, 0x58
// bytes) travels from lambda to lambda; some copies carry a word the original never writes (a block's leading
// padding, a lambda's trailing padding): those functions keep the original frame so the stale bytes are the same.
namespace {
namespace kbx {
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;   // (message, function<void(bool)>, int, int)
constexpr uint32_t SceneUtil_CompleteTutorialPhase = 0x472A75;     // (function<void()>, int, bool)
constexpr uint32_t APIManager_executeKeybladeEnhance = 0x4C9F75;   // (api, -, u64 user keyblade id, function<void(bool)>)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;          // (api, function<void()>, bool)
constexpr uint32_t SceneDeckEdit_openKeyBladePopup = 0x665585;     // (scene, keyblade id, shared_ptr<UserKeyBladeData>)
constexpr uint32_t SceneDeckEdit_playKeybladeEvolve = 0x669115;    // (-, previous, keyblade, function<void()>)
constexpr uint32_t ENHANCE_RESULT = 0x67AA21;                      // sub_67AA20 (the result popup, below)
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t STRENGTH_ID = 0x39C;        // SceneDeckEdit: the strengthened keyblade's master id (row +0)
constexpr uint32_t ROW = 0x20;                 // UserKeyBladeData: its master::Keyblade row (SafeBuffer*)
constexpr uint32_t ROW_EVOLVES = 0xCC;         // master::Keyblade: the keyblade it evolves into
constexpr uint32_t API_SHOW_ERROR = 0x14;
constexpr int32_t TXT_CONFIRM = 101900004;     // "Strengthen %s?"
constexpr uint32_t TUTORIAL_STRENGTHEN = 114;
constexpr uint32_t VT_Confirmed = 0xF63A74;    // $_15's lambda(bool) (heap 0xD0)
constexpr uint32_t VT_Enhanced = 0xF63AA0;     // $_15's lambda(bool)#1 (heap 0x70)
constexpr uint32_t VT_TutorialDone = 0xF63B50; // $_15's lambda(bool)#2's lambda() (heap 0xC8)
constexpr uint32_t VT_SlidOut = 0xF63ACC;      // $_15's lambda(bool)#1's lambda() (heap 0x70)
constexpr uint32_t VT_Evolved = 0xF63AF8;      // its lambda() (heap 0x70)
constexpr uint32_t VT_EnhanceError = 0xF63B24;

void copyBytes(uint32_t dst, uint32_t src, uint32_t n) { call(g::MEMCPY, {dst, src, n}); }

// getKeyBladeData(user keyblade id, master id, Keyblade{}) into out: the zeroed struct (built at buf, its padding as
// it was) copied to the stack arguments of the original frame at sp
void keybladeDataAt(uint32_t sp, uint32_t buf, uint32_t out, uint32_t manager, uint32_t lo, uint32_t hi, uint32_t id) {
    st32(buf + 0x50, 0);
    call(g::MEMSET, {buf, 0x14, 0});
    call(g::MEMSET, {buf + 0x18, 0x35, 0});
    for (uint32_t k = 0; k < KEYBLADE_SIZE; k += 4) st32(sp + 8 + k, ld32(buf + k));
    rw::call_exact(g::KeyBladeDataManager_getKeyBladeData, {out, manager, lo, hi, id});
}
}  // namespace kbx

// $_15 Power Up {scene, button}(sender, type): ended on an idle page -> the button and Button_List off; the shown
// keyblade's data (its master id kept at +0x39C) and the "Strengthen <name>?" confirmation, answered by a heap
// lambda {keyblade, scene, keyblade, button, scene, button, scene}
void powerUpTouched(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 2) return;
    const uint32_t me = ld32(self + 4);
    if (!lam::pageIdle(me)) return;
    Frame frame;
    const uint32_t sp = frame.allocExact(0x750);
    vcall(ld32(self + 8), V_WIDGET_SET_TOUCH_ENABLED, {ld32(self + 8), 0});
    vcall(ld32(me + BUTTON_LIST), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + BUTTON_LIST), 0});
    const uint32_t slide = ld32(me + SLIDE);
    const uint32_t ids = ld32(slide + SLIDE_IDS), index = ld32(slide + SLIDE_INDEX);
    if (uint32_t(int32_t(ld32(slide + SLIDE_IDS + 4) - ids) >> 2) <= index) call(g::VECTOR_AT_THROW, {slide + SLIDE_IDS});
    const uint32_t which = ld32(ids + 4 * index);
    if (keybladeCount(me) <= which) call(g::VECTOR_AT_THROW, {me + KEYBLADES});
    const uint32_t keyblade = ld32(me + KEYBLADES) + KEYBLADE_SIZE * which;
    const uint32_t data = sp + 0x9C;
    kbx::keybladeDataAt(sp, sp + 0x620, data, call(g::KeyBladeDataManager_getInstance, {}), ld32(keyblade),
                        ld32(keyblade + 4), 0);
    const uint32_t row = ld32(ld32(data) + kbx::ROW);
    call(kbx::SafeBuffer_read, {row, sp + 0xA4, ld32(row + 0xC), 4, 0});
    st32(me + kbx::STRENGTH_ID, ld32(sp + 0xA4));
    kbx::copyBytes(sp + 0x5C8, keyblade, KEYBLADE_SIZE);      // after the block's unwritten first word
    kbx::copyBytes(sp + 0x568, keyblade, KEYBLADE_SIZE);
    kbx::copyBytes(sp + 0x50C, sp + 0x5C4, 0x5C);
    const uint32_t td = sp + 0x94;
    call(fn::TextData_get, {td, 0, uint32_t(kbx::TXT_CONFIRM)});
    const uint32_t format = textDataStr(td);
    const uint32_t row2 = ld32(ld32(data) + kbx::ROW);
    call(kbx::SafeBuffer_read, {row2, sp + 0xC4, ld32(row2 + 0xC) + 4, 0x41, 4});
    call(fn::StringBuffer_ctor, {sp + 0x108, format, sp + 0xC4});
    sharedRelease(td);
    const uint32_t message = sp + 0x88;
    strInitC(message, sp + 0x108);
    kbx::copyBytes(sp + 0x6D8, sp + 0x568, KEYBLADE_SIZE);
    kbx::copyBytes(sp + 0x67C, sp + 0x50C, 0x5C);
    const uint32_t fo = sp + 0xA8;
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0xD0});
    if (obj) {
        const uint32_t button = ld32(self + 8);
        st32(obj, kbx::VT_Confirmed);
        kbx::copyBytes(obj + 8, sp + 0x6D8, KEYBLADE_SIZE);
        st32(obj + 0x60, me);
        kbx::copyBytes(obj + 0x64, sp + 0x67C, 0x5C);
        st32(obj + 0xC0, button);
        st32(obj + 0xC4, me);
        st32(obj + 0xC8, button);
        st32(obj + 0xCC, me);
    }
    st32(fo + 16, obj);
    call(kbx::SceneUtil_openOkCancelDialog4Line, {message, fo, 0, 0});
    functionDestroy(fo);
    strFree(message);
    release(ld32(data + 4));
}

// the confirmation {keyblade, scene, -, keyblade, button, scene, button, scene}(yes): no -> the buttons on again;
// yes -> the loading indicator on, then (during the strengthen tutorial, 114) the tutorial phase completed first
// with a heap lambda {keyblade, scene, -, keyblade, button, scene}; else /keyblade/enhance for the keyblade
void powerUpConfirmed(uint32_t self, uint32_t yesp) {
    if (!ld8(yesp)) {
        const uint32_t me = ld32(self + 0xCC);
        vcall(ld32(self + 0xC8), V_WIDGET_SET_TOUCH_ENABLED, {ld32(self + 0xC8), 1});
        vcall(ld32(me + BUTTON_LIST), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + BUTTON_LIST), 1});
        return;
    }
    call(lam::AudioPlaySystemSEDecide, {});
    call(g::SceneManager_enableLoadingIndicator, {smgr(), 1});
    Frame f;
    const uint32_t closeFn = f.alloc(20);
    call(lam::AppState_getTutorialCloseFunc, {closeFn, call(fn::AppState_getInstance, {}), kbx::TUTORIAL_STRENGTHEN});
    if (!ld32(closeFn + 16)) {
        const uint32_t a = api();
        const uint32_t lo = ld32(self + 8), hi = ld32(self + 0xC);
        const uint32_t copy = f.alloc(0x68);
        kbx::copyBytes(copy, self + 0x60, 0x68);
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x70});
        if (obj) {
            st32(obj, kbx::VT_Enhanced);
            kbx::copyBytes(obj + 8, copy, 0x68);
        }
        st32(fo + 16, obj);
        call(kbx::APIManager_executeKeybladeEnhance, {a, 0, lo, hi, fo});
        functionDestroy(fo);
        return;
    }
    functionDestroy(closeFn);
    const uint32_t copy = f.alloc(0xC0);
    kbx::copyBytes(copy, self + 8, KEYBLADE_SIZE);
    kbx::copyBytes(copy + KEYBLADE_SIZE, self + 0x60, 0x68);
    const uint32_t copy2 = f.alloc(0xC0);
    kbx::copyBytes(copy2, copy, 0xC0);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0xC8});
    if (obj) {
        st32(obj, kbx::VT_TutorialDone);
        kbx::copyBytes(obj + 8, copy2, 0xC0);
    }
    st32(fo + 16, obj);
    call(kbx::SceneUtil_CompleteTutorialPhase, {fo, 0, 1});
    functionDestroy(fo);
    call(lam::AppState_execTutorialCloseFunc, {call(fn::AppState_getInstance, {}), kbx::TUTORIAL_STRENGTHEN});
}

// the tutorial phase done {keyblade, scene, -, keyblade, button, scene}: /keyblade/enhance for the keyblade
void tutorialPhaseDone(uint32_t self) {
    const uint32_t a = api();
    const uint32_t lo = ld32(self + 8), hi = ld32(self + 0xC);
    Frame f;
    const uint32_t copy = f.alloc(0x68);
    kbx::copyBytes(copy, self + 0x60, 0x68);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x70});
    if (obj) {
        st32(obj, kbx::VT_Enhanced);
        kbx::copyBytes(obj + 8, copy, 0x68);
    }
    st32(fo + 16, obj);
    call(kbx::APIManager_executeKeybladeEnhance, {a, 0, lo, hi, fo});
    functionDestroy(fo);
}

// /keyblade/enhance's result {scene, -, keyblade, button, scene}(ok): the loading indicator off; ok -> the scene
// animates out, then a heap lambda {scene, -, keyblade, button, scene}; else the API error dialog when wanted
void enhanced(uint32_t self, uint32_t okp) {
    const uint32_t ok = ld8(okp);
    const uint32_t me = ld32(self + 8);
    call(g::SceneManager_enableLoadingIndicator, {smgr(), 0});
    if (!ok) {
        if (!ld8(api() + kbx::API_SHOW_ERROR)) return;
        const uint32_t a = api();
        Frame f;
        const uint32_t cb = newFunction(f, kbx::VT_EnhanceError, {});
        call(kbx::APIManager_openErrorDialog, {a, cb, 1});
        functionDestroy(cb);
        return;
    }
    Frame frame;
    const uint32_t sp = frame.allocExact(0x120);
    kbx::copyBytes(sp + 0x20, self + 0x10, KEYBLADE_SIZE);   // after the block's unwritten first word
    const uint32_t button = ld32(self + 0x68), scene = ld32(self + 0x6C);
    kbx::copyBytes(sp + 0xA8, sp + 0x1C, 0x5C);
    const uint32_t fo = sp + 0x78;
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x70});
    if (obj) {
        st32(obj, kbx::VT_SlidOut);
        st32(obj + 8, me);
        kbx::copyBytes(obj + 0xC, sp + 0xA8, 0x5C);
        st32(obj + 0x68, button);
        st32(obj + 0x6C, scene);
    }
    st32(fo + 16, obj);
    call(lam::IInOutAnimation_runOutAnimation, {me + lam::INOUT, fo});
    functionDestroy(fo);
}

// the scene animated out {scene, -, keyblade, button, scene}: the header touchable; the keyblade's data before
// (by the master id kept at +0x39C) and after; the same evolution target -> the result popup at once (sub_67AA20),
// else the evolution animation first, then the popup
void enhancedSlidOut(uint32_t self) {
    const uint32_t me = ld32(self + 8);
    Frame frame;
    const uint32_t sp = frame.allocExact(0x260);
    call(kbx::SceneManager_setCanTouchHeader, {smgr(), 1});
    const uint32_t before = sp + 0x98, after = sp + 0x90;
    const uint32_t manager = call(g::KeyBladeDataManager_getInstance, {});
    kbx::keybladeDataAt(sp, sp + 0x180, before, manager, 0, 0, ld32(me + kbx::STRENGTH_ID));
    const uint32_t manager2 = call(g::KeyBladeDataManager_getInstance, {});
    kbx::keybladeDataAt(sp, sp + 0x128, after, manager2, ld32(self + 0x10), ld32(self + 0x14), 0);
    const uint32_t result = sp + 0xC0;        // the result lambda {keyblade, scene, button, scene} (0x68)
    kbx::copyBytes(result, self + 0x10, KEYBLADE_SIZE);
    st32(result + 0x58, me);
    st32(result + 0x5C, ld32(self + 0x68));
    st32(result + 0x60, ld32(self + 0x6C));
    const uint32_t rowBefore = ld32(ld32(before) + kbx::ROW);
    call(kbx::SafeBuffer_read, {rowBefore, sp + 0xA0, ld32(rowBefore + 0xC) + kbx::ROW_EVOLVES, 4, kbx::ROW_EVOLVES});
    const uint32_t rowAfter = ld32(ld32(after) + kbx::ROW);
    call(kbx::SafeBuffer_read, {rowAfter, sp + 0xA4, ld32(rowAfter + 0xC) + kbx::ROW_EVOLVES, 4, kbx::ROW_EVOLVES});
    if (ld32(sp + 0xA0) == ld32(sp + 0xA4)) {
        call(kbx::ENHANCE_RESULT, {result});
    } else {
        const uint32_t prev = sp + 0x88, next = sp + 0x80;
        st32(prev, ld32(before));
        const uint32_t prevCtrl = ld32(before + 4);
        st32(prev + 4, prevCtrl);
        addRef(prevCtrl);
        st32(next, ld32(after));
        const uint32_t nextCtrl = ld32(after + 4);
        st32(next + 4, nextCtrl);
        addRef(nextCtrl);
        kbx::copyBytes(sp + 0x1D8, result, 0x68);
        const uint32_t fo = sp + 0xA8;
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x70});
        if (obj) {
            st32(obj, kbx::VT_Evolved);
            kbx::copyBytes(obj + 8, sp + 0x1D8, 0x68);
        }
        st32(fo + 16, obj);
        // playKeybladeEvolve does not use its first argument
        call(kbx::SceneDeckEdit_playKeybladeEvolve, {0, prev, next, fo});
        functionDestroy(fo);
        release(nextCtrl);
        release(prevCtrl);
    }
    release(ld32(after + 4));
    release(ld32(before + 4));
}

// the result popup {keyblade, scene, button, scene} (also the evolution's end, through its __func): the keyblade's
// strengthen result popup, the buttons on again and Button_List off
void enhanceResult(uint32_t result) {
    const uint32_t me = ld32(result + 0x58);
    const uint32_t manager = call(g::KeyBladeDataManager_getInstance, {});
    Frame f;
    const uint32_t data = keybladeData(f, manager, ld32(result), ld32(result + 4));
    const uint32_t id = ld32(me + kbx::STRENGTH_ID);
    const uint32_t copy = f.alloc(8);
    st32(copy, ld32(data));
    const uint32_t ctrl = ld32(data + 4);
    st32(copy + 4, ctrl);
    addRef(ctrl);
    call(kbx::SceneDeckEdit_openKeyBladePopup, {me, id, copy});
    release(ctrl);
    const uint32_t button = ld32(result + 0x5C), scene = ld32(result + 0x60);
    vcall(button, V_WIDGET_SET_TOUCH_ENABLED, {button, 1});
    vcall(ld32(scene + BUTTON_LIST), V_WIDGET_SET_TOUCH_ENABLED, {ld32(scene + BUTTON_LIST), 1});
    vcall(ld32(me + BUTTON_LIST), V_WIDGET_SET_TOUCH_ENABLED, {ld32(me + BUTTON_LIST), 0});
    release(ld32(data + 4));
}
}  // namespace

RW_FUNCTION(0x679E30, "SceneDeckEdit::initKeyBladePanel::$_15 operator() (Power Up)", powerUpTouched);
RW_FUNCTION(0x67A25C, "SceneDeckEdit::initKeyBladePanel::$_15::lambda(bool)#2 operator() (confirmed)", powerUpConfirmed);
RW_FUNCTION(0x67AD24, "SceneDeckEdit::initKeyBladePanel::$_15::lambda(bool)#2::lambda() operator() (tutorial phase done)", tutorialPhaseDone);
RW_FUNCTION(0x67A52C, "SceneDeckEdit::initKeyBladePanel::$_15::lambda(bool)#1 operator() (enhanced)", enhanced);
RW_FUNCTION(0x67A74C, "SceneDeckEdit::initKeyBladePanel::$_15::lambda(bool)#1::lambda() operator() (slid out)", enhancedSlidOut);
RW_FUNCTION(0x67AA20, "SceneDeckEdit::initKeyBladePanel::$_15 result popup (sub_67AA20)", enhanceResult);
