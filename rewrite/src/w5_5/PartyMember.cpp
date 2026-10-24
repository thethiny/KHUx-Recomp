// Party > Members: ScenePartyMember (the member list of the user's party, its "n/m" count and back to the party
// top) and its MemberScrollView (a BaseScrollView<unsigned long long> of user ids; rows are PartyTop_JoinPanel
// clones with an AvatarIconCache for the avatars). MemberScrollView::changeRow is in MemberScrollView_row.cpp.
// Also ScenePartyTop's member-check button chain (sub_6599B8 / sub_659AE8 / sub_659C60) that opens this screen.
#include "../w3_9/BaseScrollView.hpp"
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;
constexpr uint32_t Layer_ctor = rwlib::cocos2d::Layer::ctor_base, Layer_init = rwlib::cocos2d::Layer::init, Layer_dtor = rwlib::cocos2d::Layer::dtor_base;
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain, Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t MEMCPY = 0x40E164;                      // __aeabi_memcpy(dst, src, n)
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create, Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;                   // Size(float, float)
constexpr uint32_t Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance, GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t ExpressionVector_allocate = 0x487635;   // vector<Avatar::Expression>::allocate(n)
constexpr uint32_t ExpressionVector_destroy = 0x46BE0D;
constexpr uint32_t AvatarIconCache_ctor = 0x5CA741;        // (Texture2D*, const Vec2&, float, float, const vector<Expression>&)
constexpr uint32_t AvatarIconCache_dtor = 0x5CA8FD;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;             // vector::at failure (does not return)
constexpr uint32_t VectorULL_dtor = 0x468499;
constexpr uint32_t VectorULL_pushBackSlow = 0x4ECB6D;      // vector<ull>::__push_back_slow_path(const ull&)
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1;
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9, IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t BaseScrollViewULL_init = 0x4FC969, BaseScrollViewULL_setItemIDs = 0x4FD231;
constexpr uint32_t FunctionBTE_swap = 0x483E4D;            // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionVTE_swap = 0x4F3699;            // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t MemberTouchEnded_assign = 0x5234C1;     // sub_5234C0: function<void(Touch*, Event*)> = $_2
constexpr uint32_t Swap_ull = 0x65F595;                    // std::function<void(ull)>::swap
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create, EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t SceneUtil_setAdditionalKerningForAtlasLabel = 0x466BA9;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;  // (node, const string& name, text id)
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B, SM_setBackButtonType = 0x613E27, SM_showHeader = 0x6131ED;
constexpr uint32_t SM_changeBackground = 0x6128ED, SM_changeTickerMessage = 0x610B39;
constexpr uint32_t SM_setCanTouchHeader = 0x60E86D, SM_isOpenedPopup = 0x621519, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;   // (anim, std::function<void()> by value)
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595;     // (function<void(bool)> by value)
constexpr uint32_t APIManager_executePartyMemberList = 0x4C7AA5; // (api, bool, const function<void(bool)>&, PlatformType)
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;        // (api, const function<void(bool)>&, bool)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t MemberScrollView_setEnabledTapItem = 0x523555;
// GOT: the vtables, the __func vtable of a std::function<Layer*()> holding a function pointer, the scene factories,
// Vec2::ANCHOR_MIDDLE
constexpr uint32_t GOT_VT_MemberScrollView = 0xFD65F0, GOT_VT_BaseScrollView_ull = 0xFD65A4;
constexpr uint32_t GOT_VT_ScenePartyMember = 0xFD68E4;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_PARTY_TOP = 0xFD6620, GOT_CREATE_PARTY_MEMBER = 0xFD68E0;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
}  // namespace g

// ---- MemberScrollView
constexpr uint32_t LIST_SIZE = 0x710;
constexpr uint32_t F_ITEM_W = 0x514, F_ITEM_H = 0x518, F_ITEM_IDS = 0x628;
constexpr uint32_t F_TYPE = 0x6F0;             // MemberScrollViewType (0 members, 1, 2 invitations, 3 applicants)
constexpr uint32_t F_PANEL = 0x6F4;            // the PartyTop_JoinPanel row template (retained)
constexpr uint32_t F_TOUCH = 0x6F8;            // rows take touches
constexpr uint32_t F_ICONS = 0x708;            // AvatarIconCache*
constexpr uint32_t LIST_ON_SELECT = 0x650, LIST_ON_SELECT2 = 0x680;   // std::function<void(ull)>
constexpr uint32_t VT_RowTouchBegan = 0xF56D60;   // registItemTouchEvent $_0 {this, index}
constexpr uint32_t VT_RowTouchMoved = 0xF56D34;   // $_1 {this, index} x 3 (heap 0x1C)
constexpr uint32_t VT_RowLambdaA = 0xF56F44;      // a 0x130-byte capture lambda (heap clone sub_524A00)
constexpr uint32_t VT_RowLambdaB = 0xF56FC8;      // a 0x138-byte capture lambda (heap clone sub_525034)
constexpr uint32_t S_JoinPanelJson = 0xDFDB68;    // "cocostudio/publish/PartyTop_JoinPanel.json"
constexpr uint32_t S_Avatar = 0xDF8F56, S_AvatarLv = 0xDFDB93;   // "AtlasLabel_AvatarLV_Label"
constexpr uint32_t AVATAR_ICON_CACHE_SIZE = 0x60;

// ---- ScenePartyMember
constexpr uint32_t SCENE_SIZE = 0x298;
constexpr uint32_t F_INOUT = 0x288, F_ROOT = 0x28C, F_LIST = 0x290, F_CAN_BACK = 0x294;
constexpr uint32_t VT_OnEnter = 0xF69CA4, VT_OnExit = 0xF69C78, VT_BackKey = 0xF69C4C, VT_BackButton = 0xF69C20;
constexpr uint32_t VT_MemberSelected = 0xF69B70;  // initMemberLayer {this} void(ull)
constexpr uint32_t VT_Back = 0xF69B44;            // backScene {}: sub_6F1B7C
constexpr uint32_t VT_MemberCheckOut = 0xF62524;  // ScenePartyTop member-check button: run-out done {}: sub_659AE8
constexpr uint32_t VT_MemberListDone = 0xF62550;  // executePartyMemberList callback {}: sub_659C60
constexpr uint32_t VT_MemberListError = 0xF6257C; // error dialog callback {}
constexpr uint32_t S_Bg = 0xE03AF2, S_SceneJson = 0xE0622F;   // "cocostudio/publish/PartyTop_MemberCheck_Scene_ver130.json"
constexpr uint32_t S_WinBottom = 0xE058DC, S_ButtonBack = 0xE03DBA, S_ScrollArea = 0xDF9B59, S_RightUI = 0xDF85F7;
constexpr uint32_t S_LeftUI = 0xDF85F0, S_TxtWording = 0xDF862F, S_TxtAdministrator = 0xE06269;
constexpr uint32_t S_FmtCount = 0xDF8F35;        // "%d/%d"
constexpr int32_t TXT_TICKER = 100620042, TXT_WORDING = 103820004;
constexpr uint32_t API_PARTY_MEMBERS = 0x1FC;    // SafeBuffer of StageSupportList::SupportUser (0x128 bytes)
constexpr uint32_t API_PARTY = 0x84;             // the user's party (+0x60: vector of sub-leader ids)
constexpr uint32_t SUPPORT_USER_SIZE = 0x128;
constexpr uint32_t SM_FLAG_308 = 0x308, SM_FLAG_4CC = 0x4CC, SM_CALLBACK_548 = 0x548;

constexpr uint32_t V_deleting = 0x04, V_listInit = 0x350, V_sceneInit = 0x248, V_runInAnimation = 0x300;
constexpr uint32_t V_changeRow = 0x340, V_widgetSetTouchEnabled = 0x29C;
constexpr uint32_t V_getVirtualRenderer = 0x2D8, V_getTexture = 0x280;

inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
uint32_t api() { return call(fn::APIManager_getInstance, {}); }
// sm+0x4CC = 1, sm's header callback cleared
void resetHeaderCallback() {
    Frame f;
    const uint32_t m = sceneManager();
    const uint32_t none = newEmptyFunction(f);
    st8(m + SM_FLAG_4CC, 1);
    call(g::FunctionVB_assign, {m + SM_CALLBACK_548, none});
    functionDestroy(none);
}
void callPartyTopApi() {
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    call(g::ScenePartyTop_callPartyTopAPI, {none});
    functionDestroy(none);
}
// SceneManager::changeSceneLayer(createSceneLayer<T>, true, true, fade)
void changeScene(uint32_t gotFactory) {
    const uint32_t m = sceneManager();
    Frame f;
    const uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(gotFactory)});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 1, 1});
    functionDestroy(layer);
}
// "%d/%d" into the text
void setCountText(uint32_t text, uint32_t a, uint32_t b) {
    Frame f;
    const uint32_t buf = stringBuffer(f, {S_FmtCount, a, b});
    setTextC(text, buf);
}
}  // namespace

struct MemberScrollView {
    static uint32_t create(uint32_t type);
    static uint32_t construct(uint32_t self);
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self, uint32_t type);
    static uint32_t getLayoutPanel(uint32_t self, uint32_t, uint32_t, uint32_t);
    static uint32_t createRow(uint32_t self, uint32_t index, uint32_t arg);
    static uint32_t registItemTouchEvent(uint32_t self, uint32_t widget, uint32_t index);
    static uint32_t cloneLambdaA(uint32_t functor);
    static uint32_t cloneLambdaB(uint32_t functor);
};

struct ScenePartyMember {
    static uint32_t create();
    static void destructDelete(uint32_t self);
    static uint32_t init(uint32_t self);
    static uint32_t initMemberLayer(uint32_t self);
    static uint32_t setSelectCallback(uint32_t dst, uint32_t captures);
    static uint32_t loadMembers(uint32_t captures);
    static void runInAnimation(uint32_t self);
    static void runOutAnimationMain(uint32_t self, uint32_t done);
    static void backScene(uint32_t self);
    static void backDone(uint32_t functor);
    static uint32_t memberCheckTouched(uint32_t functor, uint32_t sender, uint32_t type);
    static void memberCheckOutDone(uint32_t functor);
    static void memberListDone(uint32_t functor, uint32_t ok);
};

// ================================================================================================ MemberScrollView
uint32_t MemberScrollView::create(uint32_t type) {
    const uint32_t me = call(fn::operator_new, {LIST_SIZE});
    construct(me);
    if (vcall(me, V_listInit, {me, type}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t MemberScrollView::construct(uint32_t s) {
    auto *base = rw::host_ptr<BaseScrollView>(s);
    call(g::Layout_ctor, {s});
    BaseScrollView::initScrollDefaults(base);
    base->itemIDs.begin = 0;
    base->itemIDs.end = 0;
    base->itemIDs.cap = 0;
    const uint32_t vt = ld32(g::GOT_VT_MemberScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + 0x364);
    st32(s + 0x350, vt + 0x378);
    for (uint32_t fo = 0x638; fo <= 0x6C8; fo += 0x18) st32(s + fo + 0x10, 0);   // seven empty std::functions
    st8(s + F_TOUCH, 0);
    st8(s + 0x6F9, 0);
    st32(s + 0x6FC, 0);
    st8(s + 0x700, 0);
    st32(s + 0x704, 0);
    st32(s + F_ICONS, 0);
    st32(s + 0x6EC, 0);
    st32(s + 0x6E8, 0);
    st32(s + 0x6E4, 0);
    st32(s + 0x6E0, 0);
    return s;
}

uint32_t MemberScrollView::destruct(uint32_t s) {
    const uint32_t vt = ld32(g::GOT_VT_MemberScrollView);
    st32(s, vt + 8);
    st32(s + 0x280, vt + 0x364);
    st32(s + 0x350, vt + 0x378);
    if (const uint32_t panel = ld32(s + F_PANEL)) vcall(panel, V_deleting, {panel});
    st32(s + F_PANEL, 0);
    if (const uint32_t icons = ld32(s + F_ICONS)) {
        call(g::AvatarIconCache_dtor, {icons});
        call(fn::operator_delete, {icons});
    }
    st32(s + F_ICONS, 0);
    for (uint32_t fo = 0x6C8; fo >= 0x638; fo -= 0x18) functionDestroy(s + fo);
    const uint32_t bvt = ld32(g::GOT_VT_BaseScrollView_ull);
    st32(s, bvt + 8);
    st32(s + 0x280, bvt + 0x360);
    st32(s + 0x350, bvt + 0x374);
    call(g::VectorULL_dtor, {s + F_ITEM_IDS});
    call(g::IScrollProtocol_dtor, {s});
    return s;
}

// Rows 946x150 (the scroll bar on); the row template PartyTop_JoinPanel (hidden) and its avatar icon cache.
uint32_t MemberScrollView::init(uint32_t s, uint32_t type) {
    st32(s + F_TYPE, type);
    stf(s + F_ITEM_W, 946.0f);
    stf(s + F_ITEM_H, 150.0f);
    call(g::IScrollProtocol_setEnabledScrollBar, {s, 1});
    stf(s + 0x570, ldf(s + F_ITEM_W) * 0.5f);
    stf(s + 0x574, -93.0f);
    stf(s + 0x57C, type == 2 ? 163.0f : 103.0f);
    const uint32_t panel = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_JoinPanelJson});
    st32(s + F_PANEL, panel);
    call(g::Ref_retain, {panel});
    vcall(ld32(s + F_PANEL), V_widgetSetTouchEnabled, {ld32(s + F_PANEL), 0});
    const uint32_t avatar = findNode(ld32(s + F_PANEL), S_Avatar, 6);
    const uint32_t renderer = vcall(avatar, V_getVirtualRenderer, {avatar});
    const uint32_t texture = vcall(renderer, V_getTexture, {renderer});
    const uint32_t cache = call(fn::operator_new, {AVATAR_ICON_CACHE_SIZE});
    Frame f;
    const uint32_t pos = f.alloc(8);
    call(g::Vec2_ctorXY, {pos, fbits(-104.0f), fbits(122.0f)});
    // vector<Avatar::Expression>{1}
    const uint32_t expression = f.alloc(4), expressions = f.alloc(12);
    st32(expression, 1);
    st32(expressions, 0);
    st32(expressions + 4, 0);
    st32(expressions + 8, 0);
    call(g::ExpressionVector_allocate, {expressions, 1});
    uint32_t end = ld32(expressions + 4);
    if (end) {
        st32(end, ld32(expression));
        end = ld32(expressions + 4);
    }
    st32(expressions + 4, end + 4);
    call(g::AvatarIconCache_ctor, {cache, texture, pos, fbits(-1.43f), fbits(1.43f), expressions});
    st32(s + F_ICONS, cache);
    call(g::ExpressionVector_destroy, {expressions});
    call(g::Vec2_dtor, {pos});
    return call(g::BaseScrollViewULL_init, {s});
}

// A row: an empty widget of the row size holding a clone of the template.
uint32_t MemberScrollView::getLayoutPanel(uint32_t s, uint32_t, uint32_t, uint32_t) {
    const uint32_t panel = call(g::Widget_create, {});
    const uint32_t row = call(g::Widget_clone, {ld32(s + F_PANEL)});
    vcall(panel, node::setAnchorPoint, {panel, ld32(g::GOT_ANCHOR_MIDDLE)});
    vcall(panel, node::addChild, {panel, row});
    {
        Frame f;
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, ld32(s + F_ITEM_W), ld32(s + F_ITEM_H)});
        vcall(panel, node::setContentSize, {panel, size});
    }
    vcall(panel, node::setPositionX, {panel, fbits(ldf(s + F_ITEM_W) * 0.5f)});
    return panel;
}

uint32_t MemberScrollView::createRow(uint32_t s, uint32_t index, uint32_t arg) {
    const uint32_t count = uint32_t(int32_t(ld32(s + F_ITEM_IDS + 4) - ld32(s + F_ITEM_IDS)) >> 3);
    if (count <= index) {
        call(g::ThrowOutOfRange, {s + F_ITEM_IDS});
        return 0;
    }
    const uint32_t panel = getLayoutPanel(s, 0, 0, 0);
    const uint32_t level = findNode(panel, S_AvatarLv, 0x19);
    call(g::SceneUtil_setAdditionalKerningForAtlasLabel, {level, uint32_t(-21)});
    vcall(s, V_changeRow, {s, panel, index, arg});
    return panel;
}

// Touch listener for a row (registered on the list itself): began / moved / ended / cancelled with (this, index).
uint32_t MemberScrollView::registItemTouchEvent(uint32_t s, uint32_t, uint32_t index) {
    if (!ld8(s + F_TOUCH)) return 0;
    const uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + 0x84);
    const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
    call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    Frame f;
    const uint32_t began = newFunction(f, VT_RowTouchBegan, {s, index});
    call(g::FunctionBTE_swap, {began, listener + 0x50});
    functionDestroy(began);
    const uint32_t captures = f.alloc(16);
    st32(captures, s);
    st32(captures + 4, index);
    st32(captures + 8, s);
    st32(captures + 12, index);
    const uint32_t moved = f.alloc(20);
    st32(moved + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x1C});
    if (obj) {
        st32(obj, VT_RowTouchMoved);
        for (uint32_t w = 4; w < 0x1C; w += 8) {
            st32(obj + w, s);
            st32(obj + w + 4, index);
        }
    }
    st32(moved + 16, obj);
    call(g::FunctionVTE_swap, {moved, listener + 0x68});
    functionDestroy(moved);
    call(g::MemberTouchEnded_assign, {listener + 0x80, captures});
    call(g::MemberTouchEnded_assign, {listener + 0x98, captures});
    call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, s});
    return 0;
}

// __func::__clone() of two row lambdas with large captures (copied whole)
uint32_t MemberScrollView::cloneLambdaA(uint32_t functor) {
    const uint32_t obj = call(fn::operator_new, {0x138});
    if (obj) {
        st32(obj, VT_RowLambdaA);
        call(g::MEMCPY, {obj + 8, functor + 8, 0x130});
    }
    return obj;
}
uint32_t MemberScrollView::cloneLambdaB(uint32_t functor) {
    const uint32_t obj = call(fn::operator_new, {0x140});
    if (obj) {
        st32(obj, VT_RowLambdaB);
        call(g::MEMCPY, {obj + 8, functor + 8, 0x138});
    }
    return obj;
}

// ================================================================================================ ScenePartyMember
uint32_t ScenePartyMember::create() {
    const uint32_t me = call(fn::operator_new, {SCENE_SIZE});
    call(g::Layer_ctor, {me});
    const uint32_t vt = ld32(g::GOT_VT_ScenePartyMember);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    st8(me + F_CAN_BACK, 0);
    if (vcall(me, V_sceneInit, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

void ScenePartyMember::destructDelete(uint32_t me) {
    const uint32_t vt = ld32(g::GOT_VT_ScenePartyMember);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    call(g::Layer_dtor, {me});
    call(fn::operator_delete, {me});
}

// The member check scene; without party data (no party any more) straight back to the party top.
uint32_t ScenePartyMember::init(uint32_t me) {
    if (call(g::Layer_init, {me}) != 1) return 0;
    Frame f;
    {
        uint32_t fo = newFunction(f, VT_OnEnter, {me});
        call(g::FunctionVV_assign, {me + 0x210, fo});
        functionDestroy(fo);
        fo = newFunction(f, VT_OnExit, {me});
        call(g::FunctionVV_assign, {me + 0x228, fo});
        functionDestroy(fo);
    }
    st8(me + F_CAN_BACK, 1);
    {
        const uint32_t m = sceneManager();
        const uint32_t fo = newFunction(f, VT_BackKey, {me});
        call(g::SM_setOnBackKeyCallback, {m, fo});
        functionDestroy(fo);
    }
    call(g::SM_setBackButtonType, {sceneManager(), 2});
    call(g::SM_showHeader, {sceneManager()});
    {
        const uint32_t m = sceneManager();
        const uint32_t s = newStr(f, S_Bg, 0x1A);
        call(g::SM_changeBackground, {m, 0, s});
        strFree(s);
    }
    {
        const uint32_t m = sceneManager();
        const uint32_t td = textData(f, TXT_TICKER);
        const uint32_t s = newStr(f, textDataStr(td));
        call(g::SM_changeTickerMessage, {m, s});
        strFree(s);
        sharedRelease(td);
    }
    {
        const uint32_t s = newStr(f, S_SceneJson, 0x39);
        st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
        strFree(s);
    }
    vcall(me, node::addChild, {me, ld32(me + F_ROOT)});
    vcall(me, V_runInAnimation, {me});
    {
        const uint32_t bottom = findNode(ld32(me + F_ROOT), S_WinBottom, 0xA);
        if (bottom) vcall(bottom, V_widgetSetTouchEnabled, {bottom, 1});
    }
    {
        const uint32_t back = findNode(ld32(me + F_ROOT), S_ButtonBack, 0xB);
        SceneUtil::backBtnSetPosY(back);
        addTouch(back, VT_BackButton, {me});
    }
    if (!ld32(api() + API_PARTY_MEMBERS)) {
        st8(me + F_CAN_BACK, 0);
        resetHeaderCallback();
        callPartyTopApi();
        changeScene(g::GOT_CREATE_PARTY_TOP);
    }
    return 1;
}

// The member list under Scroll_Area, the wording, and the selection callbacks; then the members (loadMembers).
uint32_t ScenePartyMember::initMemberLayer(uint32_t me) {
    const uint32_t area = findNode(ld32(me + F_ROOT), S_ScrollArea, 0xB);
    st32(me + F_LIST, call(0x5208F9, {0}));
    vcall(area, node::addChild, {area, ld32(me + F_LIST)});
    call(g::IScrollProtocol_setMaxColumn, {ld32(me + F_LIST), 1});
    call(g::MemberScrollView_setEnabledTapItem, {ld32(me + F_LIST), 0});
    {
        const uint32_t list = ld32(me + F_LIST);
        vcall(list, node::setContentSize, {list, vcall(area, node::getContentSize, {area})});
    }
    const uint32_t right = findNode(ld32(me + F_ROOT), S_RightUI, 7);
    {
        Frame f;
        const uint32_t s = newStr(f, S_TxtWording, 0xB);
        call(g::SceneUtil_settingTitleLabel, {right, s, uint32_t(TXT_WORDING)});
        strFree(s);
    }
    SceneUtil::setShadow(findNode(right, S_TxtAdministrator, 0x11));
    Frame f;
    const uint32_t captures = f.alloc(8);
    st32(captures, me);
    st32(captures + 4, me);
    setSelectCallback(ld32(me + F_LIST) + LIST_ON_SELECT, captures);
    setSelectCallback(ld32(me + F_LIST) + LIST_ON_SELECT2, captures);
    loadMembers(captures + 4);
    return 0;
}

// sub_6F1208: the list's std::function<void(ull)> = initMemberLayer's lambda {this}
uint32_t ScenePartyMember::setSelectCallback(uint32_t dst, uint32_t captures) {
    Frame f;
    const uint32_t fo = newFunction(f, VT_MemberSelected, {ld32(captures)});
    call(g::Swap_ull, {fo, dst});
    functionDestroy(fo);
    return 0;
}

// sub_6F127C (initMemberLayer's lambda {this}): the party members' user ids into the list and the "n/m" count;
// without member data an empty list, then back to the party top.
uint32_t ScenePartyMember::loadMembers(uint32_t captures) {
    const uint32_t me = ld32(captures);
    const uint32_t a = api();
    const uint32_t right = findNode(ld32(me + F_ROOT), S_RightUI, 7);
    const uint32_t admin = findNode(right, S_TxtAdministrator, 0x11);
    Frame f;
    const uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    const uint32_t members = ld32(a + API_PARTY_MEMBERS);
    if (members) {
        if (ld32(members + 0x20)) {
            const uint32_t record = f.alloc(SUPPORT_USER_SIZE);
            for (uint32_t n = 0, at = 0;; at += SUPPORT_USER_SIZE) {
                const uint32_t sb = ld32(a + API_PARTY_MEMBERS);
                rw::call(g::SafeBuffer_read, {sb, record, ld32(sb + 0xC) + at, SUPPORT_USER_SIZE, at});
                const uint32_t end = ld32(ids + 4);
                if (end == ld32(ids + 8)) {
                    call(g::VectorULL_pushBackSlow, {ids, record});
                } else {
                    uint32_t n8 = 0;
                    if (end) {
                        st32(end, ld32(record));
                        st32(end + 4, ld32(record + 4));
                        n8 = ld32(ids + 4);
                    }
                    st32(ids + 4, n8 + 8);
                }
                if (!(++n < ld32(ld32(a + API_PARTY_MEMBERS) + 0x20))) break;
            }
        }
        call(g::BaseScrollViewULL_setItemIDs, {ld32(me + F_LIST), ids});
        const uint32_t party = ld32(api() + API_PARTY);
        const uint32_t count = uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 3);
        if (party && count) {
            const uint32_t admins = uint32_t(int32_t(ld32(party + 0x64) - ld32(party + 0x60)) >> 3);
            setCountText(admin, admins + 1, count);
        } else {
            setCountText(admin, 1, 1);
        }
    } else {
        setCountText(admin, 1, 1);
        const uint32_t e = ld32(ids + 4), b = ld32(ids);
        if (e != b) st32(ids + 4, e + (~7u & ~(e - 8 - b)));
        call(g::BaseScrollViewULL_setItemIDs, {ld32(me + F_LIST), ids});
        st8(me + F_CAN_BACK, 0);
        resetHeaderCallback();
        callPartyTopApi();
        changeScene(g::GOT_CREATE_PARTY_TOP);
    }
    call(g::VectorULL_dtor, {ids});
    return 0xDFC;
}

// ------------------------------------------------------------------------------------------------ animations
void ScenePartyMember::runInAnimation(uint32_t me) {
    Frame f;
    {
        const uint32_t node = findNode(ld32(me + F_ROOT), S_RightUI, 7);
        const uint32_t none = newEmptyFunction(f);
        call(g::Node_runAction, {node, SceneUtil::createRightInAnimation(node, 0.0f, 0.0f, none)});
        functionDestroy(none);
    }
    {
        const uint32_t node = findNode(ld32(me + F_ROOT), S_LeftUI, 6);
        const uint32_t none = newEmptyFunction(f);
        call(g::Node_runAction, {node, SceneUtil::createLeftInAnimation(node, 0.0f, 0.0f, none)});
        functionDestroy(none);
    }
    initMemberLayer(me);
}

// Slide both panels out; `done` (std::function<void()>) runs when the right panel is gone.
void ScenePartyMember::runOutAnimationMain(uint32_t me, uint32_t done) {
    call(g::SM_setCanTouchHeader, {sceneManager(), 0});
    Frame f;
    {
        const uint32_t node = findNode(ld32(me + F_ROOT), S_RightUI, 7);
        const uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, done});
        call(g::Node_runAction, {node, SceneUtil::createRightOutAnimation(node, 0.0f, 0.0f, copy)});
        functionDestroy(copy);
    }
    {
        const uint32_t node = findNode(ld32(me + F_ROOT), S_LeftUI, 6);
        const uint32_t none = newEmptyFunction(f);
        call(g::Node_runAction, {node, SceneUtil::createLeftOutAnimation(node, 0.0f, 0.0f, none)});
        functionDestroy(none);
    }
}

// ------------------------------------------------------------------------------------------------ back
void ScenePartyMember::backScene(uint32_t me) {
    if (!ld8(me + F_CAN_BACK)) return;
    if (call(g::SM_isOpenedPopup, {sceneManager()}) == 1) {
        call(g::SM_closeCurrentPopup, {sceneManager(), 0});
        return;
    }
    st8(me + F_CAN_BACK, 0);
    st8(sceneManager() + SM_FLAG_308, 0);
    Frame f;
    const uint32_t fo = newFunction(f, VT_Back, {});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

// backScene's lambda: refresh the party top and return there.
void ScenePartyMember::backDone(uint32_t) {
    resetHeaderCallback();
    callPartyTopApi();
    changeScene(g::GOT_CREATE_PARTY_TOP);
}

// ScenePartyTop's member-check button (touch ended): decide sound, slide the party top out.
uint32_t ScenePartyMember::memberCheckTouched(uint32_t functor, uint32_t, uint32_t type) {
    if (ld32(type) != 2) return 0;
    const uint32_t top = ld32(functor + 4);
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_MemberCheckOut, {});
    call(g::IInOutAnimation_runOutAnimation, {top + 0x288, fo});
    functionDestroy(fo);
    return 0;
}

// ...then ask for the member list (loading indicator on).
void ScenePartyMember::memberCheckOutDone(uint32_t) {
    resetHeaderCallback();
    call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t a = api();
    Frame f;
    const uint32_t cb = newFunction(f, VT_MemberListDone, {});
    call(g::APIManager_executePartyMemberList, {a, 0, cb, 1});
    functionDestroy(cb);
}

// ...the member list arrived: the member scene (or the party top without a party); on failure the error dialog.
void ScenePartyMember::memberListDone(uint32_t, uint32_t okPtr) {
    const uint32_t ok = ld8(okPtr);
    call(g::SM_enableLoadingIndicator, {sceneManager(), 0});
    const uint32_t a = api();
    if (ok) {
        const uint32_t members = ld32(a + API_PARTY_MEMBERS);
        if (members) {
            changeScene(g::GOT_CREATE_PARTY_MEMBER);
        } else {
            resetHeaderCallback();
            callPartyTopApi();
            changeScene(g::GOT_CREATE_PARTY_TOP);
        }
    } else if (ld8(a + 0x14)) {
        const uint32_t a2 = api();
        Frame f;
        const uint32_t cb = newFunction(f, VT_MemberListError, {});
        call(g::APIManager_openErrorDialog, {a2, cb, 1});
        functionDestroy(cb);
    }
}

RW_FUNCTION(0x5208F8, "MemberScrollView::create", MemberScrollView::create);
RW_FUNCTION(0x520954, "MemberScrollView::MemberScrollView", MemberScrollView::construct);
RW_FUNCTION(0x520710, "MemberScrollView::~MemberScrollView", MemberScrollView::destruct);
RW_FUNCTION(0x520AF4, "MemberScrollView::init", MemberScrollView::init);
RW_FUNCTION(0x520CF8, "MemberScrollView::getLayoutPanel", MemberScrollView::getLayoutPanel);
RW_FUNCTION(0x520DC4, "MemberScrollView::createRow", MemberScrollView::createRow);
RW_FUNCTION(0x523378, "MemberScrollView::registItemTouchEvent", MemberScrollView::registItemTouchEvent);
RW_FUNCTION(0x524A00, "MemberScrollView row lambda (0x130 captures) __func::__clone() (sub_524A00)",
            MemberScrollView::cloneLambdaA);
RW_FUNCTION(0x525034, "MemberScrollView row lambda (0x138 captures) __func::__clone() (sub_525034)",
            MemberScrollView::cloneLambdaB);
RW_FUNCTION(0x659E6C, "ScenePartyMember::create", ScenePartyMember::create);
RW_FUNCTION(0x6F0AA8, "ScenePartyMember::~ScenePartyMember (deleting)", ScenePartyMember::destructDelete);
RW_FUNCTION(0x6F0B14, "ScenePartyMember::init", ScenePartyMember::init);
RW_FUNCTION(0x6F0FDC, "ScenePartyMember::initMemberLayer", ScenePartyMember::initMemberLayer);
RW_FUNCTION(0x6F1208, "ScenePartyMember::initMemberLayer select callback assign (sub_6F1208)",
            ScenePartyMember::setSelectCallback);
RW_FUNCTION(0x6F127C, "ScenePartyMember::initMemberLayer load members lambda (sub_6F127C)", ScenePartyMember::loadMembers);
RW_FUNCTION(0x6F16E0, "ScenePartyMember::runInAnimation", ScenePartyMember::runInAnimation);
RW_FUNCTION(0x6F188C, "ScenePartyMember::runOutAnimationMain", ScenePartyMember::runOutAnimationMain);
RW_FUNCTION(0x6F1A50, "ScenePartyMember::backScene", ScenePartyMember::backScene);
RW_FUNCTION(0x6F1B7C, "ScenePartyMember::backScene lambda __func::operator() (sub_6F1B7C)", ScenePartyMember::backDone);
RW_FUNCTION(0x6599B8, "ScenePartyTop member-check button lambda __func::operator() (sub_6599B8)",
            ScenePartyMember::memberCheckTouched);
RW_FUNCTION(0x659AE8, "ScenePartyTop member-check run-out lambda __func::operator() (sub_659AE8)",
            ScenePartyMember::memberCheckOutDone);
RW_FUNCTION(0x659C60, "ScenePartyTop member list callback __func::operator() (sub_659C60)", ScenePartyMember::memberListDone);
