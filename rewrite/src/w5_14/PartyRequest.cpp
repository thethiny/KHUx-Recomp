// Party > Recruit: ScenePartyRequest. For a user in a party (the "invitation" layout) the member search: the four
// search filter buttons, the user list (a MemberScrollView) and Search by ID (a popup with an edit box); otherwise the
// initial request layout. Also ScenePartyRequest::callUserSearchAPI (the invite search request), the filter button
// builder sub_6D6C44, the list loader sub_6D6A60, backScene's lambda and the ID popup's result / close lambdas.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_init = 0x88A8A1, Layer_dtor = 0x88A88D;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t EaseSineOut_create = 0x860D09;
constexpr uint32_t FadeTo_create = 0x86B3E1;
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Size_ctor = 0x8DF027;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVV_swap = 0x4EE86D;             // std::function<void()>::swap
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionVB_copy = 0x45C84D, FunctionVB_move = 0x47BC69;
constexpr uint32_t FunctionVUll_swap = 0x65F595;           // std::function<void(ull)>::swap
constexpr uint32_t VectorULL_pushBackSlow = 0x4ECB6D, VectorULL_dtor = 0x468499;
constexpr uint32_t VectorInt_pushSlowConst = 0x4D6CC1, VectorInt_dtor = 0x478961;
constexpr uint32_t VectorPartyEx_copy = 0x51EE91, VectorPartyEx_dtor = 0x4B3DD1;
constexpr uint32_t TreeTimezone_insertUnique = 0x54CE39, TreeTimezone_destroy = 0x54CD73;
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetImageView = 0x531631;
constexpr uint32_t ImageButton_setSelectEnabled = 0x5318F1, ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t MemberScrollView_create = 0x5208F9, MemberScrollView_setEnabledTapItem = 0x523555;
constexpr uint32_t MemberScrollView_resetScrollPosition = 0x52355D;
constexpr uint32_t PartyScrollView_resetScrollPosition = 0x51F0C9;
constexpr uint32_t BaseScrollViewULL_setItemIDs = 0x4FD231, BaseScrollViewInt_setItemIDs = 0x478929;
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t SceneUtil_createRightInAnimation = 0x45B8BD, SceneUtil_createRightOutAnimation = 0x45BA3D;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05, SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_backBtnSetPosY = 0x45AFB5;
constexpr uint32_t SceneUtil_textFieldToUiText = 0x45C465, SceneUtil_textFieldToEditBox = 0x45C0A1;
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B, SM_setBackButtonType = 0x613E27, SM_showHeader = 0x6131ED;
constexpr uint32_t SM_changeBackground = 0x6128ED, SM_changeTickerMessage = 0x610B39;
constexpr uint32_t SM_setCanTouchHeader = 0x60E86D, SM_getPopupLayer = 0x613E69;
constexpr uint32_t SM_isOpenedPopup = 0x621519, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SM_openPopup = 0x61524D;                // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t APIManager_executePartyInviteSearch = 0x4D073D;
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595;
constexpr uint32_t Request_ctor = 0x657AD5, Request_runInJoinLayout = 0x6D7191, Request_runInEntryLayout = 0x6D7451;
constexpr uint32_t Request_loadList = 0x6D6A61, Request_searchButton = 0x6D6C45, Request_closePopup = 0x6DEDA5;
constexpr uint32_t GOT_VT_Request = 0xFD68D4, GOT_POPUP_FADE_TIME = 0xFD65C0;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_PARTY_TOP = 0xFD6620;
constexpr uint32_t LUX_RANK_MAP = 0xE860D0;                // int[5]: the request's lux rank for ranks 1..5
}  // namespace g

constexpr uint32_t SIZE = 0x318;
constexpr uint32_t F_ON_ENTER = 0x210, F_ON_EXIT = 0x228;
constexpr uint32_t F_ANIM_A = 0x288, F_INOUT = 0x28C;      // second / third vtables (IInOutAnimation at +0x28C)
constexpr uint32_t F_ROOT = 0x290, F_JOINED = 0x294;
constexpr uint32_t F_SEARCH_LIST = 0x2D0;                  // PartyScrollView (initial layout)
constexpr uint32_t F_MEMBER_LIST = 0x2D4;                  // MemberScrollView (invitation layout)
constexpr uint32_t F_TEXT = 0x2D8;                         // std::string
constexpr uint32_t F_ON_ID_POPUP = 0x2E8;                  // std::function<void()>
constexpr uint32_t F_EDIT_NODE = 0x300, F_EDIT_LABEL = 0x304, F_EDIT_CLOSING = 0x308, F_ID_TEXT = 0x30C;
constexpr uint32_t F_CAN_BACK = 0x310;
constexpr uint32_t VT2_OFFSET = 0x328, VT3_OFFSET = 0x348;
// lambdas (vtable + 8)
constexpr uint32_t VT_OnEnter = 0xF69120, VT_OnExit = 0xF690F4, VT_BackKey = 0xF690C8, VT_BackButton = 0xF6909C;
constexpr uint32_t VT_SearchButton = 0xF68FC0;   // {this, this}
constexpr uint32_t VT_JoinedFilter3 = 0xF68F68, VT_JoinedFilter1 = 0xF68F10, VT_JoinedFilter2 = 0xF68EB8;
constexpr uint32_t VT_JoinedFilter4 = 0xF68E60;
constexpr uint32_t VT_EntryFilter1 = 0xF68E08, VT_EntryFilter2 = 0xF68DB0, VT_EntryFilter3 = 0xF68D58;
constexpr uint32_t VT_EntryFilter4 = 0xF68D00;
constexpr uint32_t VT_FilterTouch = 0xF691A4;    // sub_6D6C44's ImageButton touch {function<void()>} (heap 0x20)
constexpr uint32_t VT_SearchID = 0xF68CA8, VT_MemberTap = 0xF68BA0;   // {this}
constexpr uint32_t VT_Back = 0xF67E64;           // backScene {}: sub_6DD3C8
constexpr uint32_t VT_SearchDone = 0xF69178;     // callUserSearchAPI {function<void(bool)>} (heap 0x20)
constexpr uint32_t VT_IdEdited = 0xF68284;       // heap 0x24 {edit box, this, ok, close, close, shadow, widget, layer}
constexpr uint32_t VT_IdOk = 0xF6822C;           // {this, ok, close}
constexpr uint32_t VT_IdClose = 0xF68200, VT_IdCheck = 0xF681D4;      // {}, {close}
constexpr uint32_t VT_IdResult = 0xF6817C;       // heap 0x1C {this, ok, close, shadow, widget, layer}: sub_6DED2A
constexpr uint32_t VT_IdClosed = 0xF681A8;       // {layer, shadow, widget}: sub_6DEEE2
// strings
constexpr uint32_t S_Empty = 0xDF8A1A, S_Bg = 0xE03AF2;
constexpr uint32_t S_JoinedJson = 0xE05C6D;      // "cocostudio/publish/PartyTop_Invitation_Scene_ver130.json"
constexpr uint32_t S_EntryJson = 0xE05CA6;       // "cocostudio/publish/PartyTop_InitialRequest_Scene_ver130.json"
constexpr uint32_t S_WinBottom = 0xE058DC, S_ButtonBack = 0xE03DBA, S_TxtSearch = 0xE05CE3, S_ButtonSearch = 0xE05CEE;
constexpr uint32_t S_FmtFilterLabel = 0xE06061, S_FmtFilterButton = 0xE06075, S_FmtFilterImage = 0xE06086;
constexpr uint32_t S_But04 = 0xDF93D9;
constexpr uint32_t S_RightUI = 0xDF85F7, S_LeftUI = 0xDF85F0;
constexpr uint32_t S_TxtSearchID = 0xE05CFC, S_ButtonSearchID = 0xE05D09, S_ScrollArea = 0xDF9B59;
constexpr uint32_t S_IdSearchJson = 0xE05F81;    // "cocostudio/publish/PopupNormal_Party_IDSearch.json"
constexpr uint32_t S_2LineLabel = 0xDFE647, S_TxtID = 0xDFE756, S_TextFieldID = 0xE05FB4;
constexpr uint32_t S_FontSoGei = 0xDF9F4D, S_ButtonOK = 0xDF8688, S_ButtonClose = 0xDF8692;
constexpr uint32_t S_TxtOK = 0xDF869F, S_TxtCancel = 0xDF86A6;
constexpr int32_t TXT_TICKER_JOINED = 100620041, TXT_TICKER_ENTRY = 100620038, TXT_SEARCH = 103800028;
constexpr int32_t TXT_SEARCH_ID = 103800032, TXT_ID_WORDING = 103830010, TXT_ID = 103800045;
constexpr int32_t TXT_OK = 100100001, TXT_CANCEL = 100100002;
constexpr uint32_t API_USER_DATA = 0x84;         // shared_ptr<SafeBuffer> (+0x10: in a party)
constexpr uint32_t API_INVITE_USERS = 0x134;     // shared_ptr<SafeBuffer>: 0x128-byte records, u64 user id first
constexpr uint32_t API_APPLICATIONS = 0x204;     // vector<PartyApplicationList::PartyEx> (0x198 bytes each)
constexpr uint32_t PARTY_EX_SIZE = 0x198;
constexpr uint32_t SM_FLAG_4CC = 0x4CC, SM_CALLBACK_548 = 0x548, SM_HEADER_308 = 0x308;
constexpr uint32_t V_init = 0x248, V_deleting = 0x04, V_runInAnimation = 0x310, V_setTouchEnabled = 0x29C;
constexpr uint32_t V_getString = 0x150, V_removeChild = 0x12C;

void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
inline void setTouch(uint32_t node, uint32_t on) { vcall(node, V_setTouchEnabled, {node, on}); }
float fadeTime() { return ldf(ld32(g::GOT_POPUP_FADE_TIME)); }
// node->runAction(EaseSineOut(make(node, 0, 0, done))) for a SceneUtil animation maker
void runEased(uint32_t make, uint32_t node, uint32_t done) {
    const uint32_t action = call(make, {node, 0, 0, done});
    call(g::Node_runAction, {node, call(g::EaseSineOut_create, {action})});
}
void runEasedEmpty(uint32_t make, uint32_t node) {
    Frame f;
    const uint32_t done = newEmptyFunction(f);
    runEased(make, node, done);
    functionDestroy(done);
}
// a label with a text-table text, then its shadow
void textThenShadow(uint32_t text, int32_t id) {
    setTextId(text, id);
    SceneUtil::setShadow(text);
}
}  // namespace

struct ScenePartyRequest {
    static uint32_t create();
    static uint32_t construct(uint32_t self);
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static void loadList(uint32_t closure);
    static uint32_t searchButton(uint32_t closure, uint32_t number, uint32_t textId, uint32_t onTouch);
    static void runInAnimation(uint32_t self);
    static void runInJoinLayout(uint32_t self);
    static void runOutAnimationMain(uint32_t self, uint32_t done);
    static void backScene(uint32_t self);
    static void backDone(uint32_t functor);
    static void callUserSearchAPI(uint32_t levelRange, uint32_t timezone, uint32_t frequency, uint32_t luxRank,
                                  uint32_t done);
    static void openPopUpSearchID(uint32_t self);
    static void idResult(uint32_t functor, uint32_t okPtr);
    static void closeIdPopup(uint32_t closure);
    static void idClosed(uint32_t functor);
};

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t ScenePartyRequest::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Request_ctor, {me});
    if (vcall(me, V_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t ScenePartyRequest::construct(uint32_t s) {
    call(g::Layer_ctor, {s});
    const uint32_t vt = ld32(g::GOT_VT_Request);
    st32(s, vt + 8);
    st32(s + F_ANIM_A, vt + VT2_OFFSET);
    st32(s + F_INOUT, vt + VT3_OFFSET);
    st8(s + F_JOINED, 0);
    st32(s + 0x2B8, 0);
    st32(s + 0x2BC, 0);
    strInit(s + F_TEXT, S_Empty, 0);
    st32(s + F_ON_ID_POPUP + 0x10, 0);
    st32(s + F_EDIT_NODE, 0);
    st32(s + F_EDIT_LABEL, 0);
    st8(s + F_EDIT_CLOSING, 0);
    st32(s + F_ID_TEXT, 0);
    st8(s + F_CAN_BACK, 0);
    return s;
}

uint32_t ScenePartyRequest::destruct(uint32_t s) {
    const uint32_t vt = ld32(g::GOT_VT_Request);
    st32(s, vt + 8);
    st32(s + F_ANIM_A, vt + VT2_OFFSET);
    st32(s + F_INOUT, vt + VT3_OFFSET);
    functionDestroy(s + F_ON_ID_POPUP);
    call(g::Layer_dtor, {s});
    return s;
}

// ------------------------------------------------------------------------------------------------ init
// The layout of a user in a party (search filters: region / level / play time / lux rank texts differ) or not.
uint32_t ScenePartyRequest::init(uint32_t me) {
    if (call(g::Layer_init, {me}) != 1) return 0;
    Frame f;
    for (const auto &[member, vtable] : {std::pair{F_ON_ENTER, VT_OnEnter}, std::pair{F_ON_EXIT, VT_OnExit}}) {
        const uint32_t fo = newFunction(f, vtable, {me});
        call(g::FunctionVV_assign, {me + member, fo});
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
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t userCtl = ld32(api + API_USER_DATA + 4), user = ld32(api + API_USER_DATA);
    addRefIf(userCtl);
    bool joined = false;
    if (user) {
        const uint32_t flag = f.alloc(4);
        rw::call(g::SafeBuffer_read, {user, flag, ld32(user + 0xC) + 0x10, 1, 0x10});
        st8(me + F_JOINED, ld8(flag));
        joined = ld8(flag) != 0;
    } else {
        st8(me + F_JOINED, 0);
    }
    {
        const uint32_t s = joined ? newStr(f, S_JoinedJson, 0x38) : newStr(f, S_EntryJson, 0x3C);
        st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
        strFree(s);
        const uint32_t m = sceneManager();
        const uint32_t td = textData(f, joined ? TXT_TICKER_JOINED : TXT_TICKER_ENTRY);
        const uint32_t t = newStr(f, textDataStr(td));
        call(g::SM_changeTickerMessage, {m, t});
        strFree(t);
        sharedRelease(td);
    }
    vcall(me, node::addChild, {me, ld32(me + F_ROOT)});
    {
        const uint32_t bottom = findNode(ld32(me + F_ROOT), S_WinBottom, 0xA);
        if (bottom) setTouch(bottom, 1);
    }
    st32(me + 0x298, 0);
    st32(me + 0x2A0, 0);
    st32(me + 0x2A8, 0);
    st32(me + 0x2B0, 0);
    st32(me + 0x2BC, 7);
    st32(me + 0x2C0, 0);
    st32(me + 0x2C8, 0);
    vcall(me, V_runInAnimation, {me});
    {
        const uint32_t back = findNode(ld32(me + F_ROOT), S_ButtonBack, 0xB);
        call(g::SceneUtil_backBtnSetPosY, {back});
        addTouch(back, VT_BackButton, {me});
    }
    const uint32_t closure = f.alloc(4);
    st32(closure, me);
    call(g::Request_loadList, {closure});
    textThenShadow(findNode(ld32(me + F_ROOT), S_TxtSearch, 0xA), TXT_SEARCH);
    addTouch(findNode(ld32(me + F_ROOT), S_ButtonSearch, 0xD), VT_SearchButton, {me, me});
    struct Filter {
        uint32_t vt, number;
        int32_t text;
    };
    static const Filter entry[] = {{VT_EntryFilter1, 1, 102400003}, {VT_EntryFilter2, 2, 103800012},
                                   {VT_EntryFilter3, 3, 103800019}, {VT_EntryFilter4, 4, 103800023}};
    static const Filter joinedFilters[] = {{VT_JoinedFilter3, 3, 103800042}, {VT_JoinedFilter1, 1, 102400003},
                                           {VT_JoinedFilter2, 2, 102400001}, {VT_JoinedFilter4, 4, 103800043}};
    for (const Filter &k : ld8(me + F_JOINED) ? joinedFilters : entry) {
        Frame t;
        const uint32_t fo = newFunction(t, k.vt, {me});
        call(g::Request_searchButton, {closure, k.number, uint32_t(k.text), fo});
        functionDestroy(fo);
    }
    releaseIf(userCtl);
    return 1;
}

// The list (closure {this}): in a party, the users to invite (MemberScrollView); else the applications.
void ScenePartyRequest::loadList(uint32_t closure) {
    const uint32_t me = ld32(closure);
    Frame f;
    if (ld8(me + F_JOINED)) {
        const uint32_t ids = f.alloc(12);
        st32(ids, 0);
        st32(ids + 4, 0);
        st32(ids + 8, 0);
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t ctl = ld32(api + API_INVITE_USERS + 4), sb = ld32(api + API_INVITE_USERS);
        addRefIf(ctl);
        if (ld32(sb + 0x20)) {
            const uint32_t rec = f.alloc(0x128);
            for (uint32_t i = 0, at = 0;;) {
                rw::call(g::SafeBuffer_read, {sb, rec, ld32(sb + 0xC) + at, 0x128, at});
                const uint32_t end = ld32(ids + 4);
                if (end == ld32(ids + 8)) {
                    call(g::VectorULL_pushBackSlow, {ids, rec});
                } else {
                    uint32_t n = 0;
                    if (end) {
                        st32(end, ld32(rec));
                        st32(end + 4, ld32(rec + 4));
                        n = ld32(ids + 4);
                    }
                    st32(ids + 4, n + 8);
                }
                at += 0x128;
                if (++i >= ld32(sb + 0x20)) break;
            }
        }
        call(g::MemberScrollView_resetScrollPosition, {ld32(me + F_MEMBER_LIST)});
        call(g::BaseScrollViewULL_setItemIDs, {ld32(me + F_MEMBER_LIST), ids});
        releaseIf(ctl);
        call(g::VectorULL_dtor, {ids});
        return;
    }
    const uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t parties = f.alloc(12);
    call(g::VectorPartyEx_copy, {parties, api + API_APPLICATIONS});
    const uint32_t value = f.alloc(4);
    st32(value, 0);
    if (ld32(parties + 4) != ld32(parties)) {
        for (uint32_t k = 1;; k++) {
            const uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(g::VectorInt_pushSlowConst, {ids, value});
            } else {
                uint32_t n = 0;
                if (end) {
                    st32(end, k - 1);
                    n = ld32(ids + 4);
                }
                st32(ids + 4, n + 4);
            }
            st32(value, k);
            if (!(k < (ld32(parties + 4) - ld32(parties)) / PARTY_EX_SIZE)) break;
        }
    }
    call(g::PartyScrollView_resetScrollPosition, {ld32(me + F_SEARCH_LIST)});
    call(g::BaseScrollViewInt_setItemIDs, {ld32(me + F_SEARCH_LIST), ids});
    call(g::VectorPartyEx_dtor, {parties});
    call(g::VectorInt_dtor, {ids});
}

// A search filter button (closure {this}): its label text, an ImageButton "But04" over Button_Search_<n>, and the
// touch callback `onTouch` wrapped in the button's lambda.
uint32_t ScenePartyRequest::searchButton(uint32_t closure, uint32_t number, uint32_t textId, uint32_t onTouch) {
    const uint32_t me = ld32(closure);
    Frame f;
    {
        const uint32_t root = ld32(me + F_ROOT);
        const uint32_t name = newStr(f, stringBuffer(f, {S_FmtFilterLabel, number}));
        const uint32_t label = SceneUtil::searchNodeByName(root, name);
        strFree(name);
        setTextId(label, int32_t(textId));
        SceneUtil::setShadow(label);
    }
    uint32_t view;
    {
        const uint32_t root = ld32(me + F_ROOT);
        const uint32_t name = newStr(f, stringBuffer(f, {S_FmtFilterButton, number}));
        view = SceneUtil::searchNodeByName(root, name);
        strFree(name);
    }
    uint32_t button;
    {
        const uint32_t name = newStr(f, S_But04, 5);
        button = call(g::ImageButton_create, {name, 1});
        strFree(name);
    }
    call(g::ImageButton_setTargetImageView, {button, view});
    call(g::ImageButton_setSelectEnabled, {button, 1});
    {
        const uint32_t name = newStr(f, stringBuffer(f, {S_FmtFilterImage, number}));
        vcall(view, node::addChildZName, {view, button, 0, name});
        strFree(name);
    }
    const uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, onTouch});
    const uint32_t fo = f.alloc(20);
    heapFunction(fo, VT_FilterTouch, 0x20, 8, fn::function_vv_move, copy);
    call(g::ImageButton_addTouchEventListener, {button, fo});
    functionDestroy(fo);
    functionDestroy(copy);
    return 0;
}

// ------------------------------------------------------------------------------------------------ animations
void ScenePartyRequest::runInAnimation(uint32_t me) {
    call(ld8(me + F_JOINED) ? g::Request_runInJoinLayout : g::Request_runInEntryLayout, {me});
    runEasedEmpty(g::SceneUtil_createRightInAnimation, findNode(ld32(me + F_ROOT), S_RightUI, 7));
    runEasedEmpty(0x45B5C5, findNode(ld32(me + F_ROOT), S_LeftUI, 6));
}

// The invitation layout: Search by ID and the member list (taps report a user id).
void ScenePartyRequest::runInJoinLayout(uint32_t me) {
    textThenShadow(findNode(ld32(me + F_ROOT), S_TxtSearchID, 0xC), TXT_SEARCH_ID);
    addTouch(findNode(ld32(me + F_ROOT), S_ButtonSearchID, 0xF), VT_SearchID, {me});
    const uint32_t area = findNode(ld32(me + F_ROOT), S_ScrollArea, 0xB);
    st32(me + F_MEMBER_LIST, call(g::MemberScrollView_create, {1}));
    call(g::IScrollProtocol_setMaxColumn, {ld32(me + F_MEMBER_LIST), 1});
    call(g::MemberScrollView_setEnabledTapItem, {ld32(me + F_MEMBER_LIST), 0});
    {
        const uint32_t list = ld32(me + F_MEMBER_LIST);
        vcall(list, node::setContentSize, {list, vcall(area, node::getContentSize, {area})});
    }
    {
        Frame f;
        const uint32_t fo = newFunction(f, VT_MemberTap, {me});
        call(g::FunctionVUll_swap, {fo, ld32(me + F_MEMBER_LIST) + 0x668});
        functionDestroy(fo);
    }
    vcall(area, node::addChild, {area, ld32(me + F_MEMBER_LIST)});
}

// Slide out (the layout is built again first, as the original does); `done` runs when the left panel is gone.
void ScenePartyRequest::runOutAnimationMain(uint32_t me, uint32_t done) {
    call(g::SM_setCanTouchHeader, {sceneManager(), 0});
    call(ld8(me + F_JOINED) ? g::Request_runInJoinLayout : g::Request_runInEntryLayout, {me});
    runEasedEmpty(g::SceneUtil_createRightOutAnimation, findNode(ld32(me + F_ROOT), S_RightUI, 7));
    const uint32_t left = findNode(ld32(me + F_ROOT), S_LeftUI, 6);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    runEased(0x45B735, left, copy);
    functionDestroy(copy);
}

// ------------------------------------------------------------------------------------------------ back
void ScenePartyRequest::backScene(uint32_t me) {
    if (!ld8(me + F_CAN_BACK)) return;
    if (call(g::SM_isOpenedPopup, {sceneManager()}) == 1) {
        call(g::SM_closeCurrentPopup, {sceneManager(), 0});
        return;
    }
    st8(me + F_CAN_BACK, 0);
    st8(sceneManager() + SM_HEADER_308, 0);
    Frame f;
    const uint32_t fo = newFunction(f, VT_Back, {});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

// backScene's lambda: refresh the party data and return to the party top.
void ScenePartyRequest::backDone(uint32_t) {
    Frame f;
    {
        const uint32_t m = sceneManager();
        const uint32_t none = newEmptyFunction(f);
        st8(m + SM_FLAG_4CC, 1);
        call(g::FunctionVB_assign, {m + SM_CALLBACK_548, none});
        functionDestroy(none);
    }
    {
        const uint32_t none = newEmptyFunction(f);
        call(g::ScenePartyTop_callPartyTopAPI, {none});
        functionDestroy(none);
    }
    const uint32_t m = sceneManager();
    const uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(g::GOT_CREATE_PARTY_TOP)});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 1, 1});
    functionDestroy(layer);
}

// ------------------------------------------------------------------------------------------------ search
// The invite search request: the time zone filter as a one-element set (none for 0 / over 6), the lux rank
// mapped through the request's table (ranks 1, 2, 4, 5; rank 3 passes as is).
void ScenePartyRequest::callUserSearchAPI(uint32_t levelRange, uint32_t timezone, uint32_t frequency,
                                          uint32_t luxRank, uint32_t done) {
    Frame f;
    const uint32_t set = f.alloc(12);
    st32(set + 4, 0);
    st32(set + 8, 0);
    st32(set, set + 4);
    if (timezone - 1 <= 5) {
        const uint32_t value = f.alloc(4), result = f.alloc(8);
        st32(value, timezone);
        call(g::TreeTimezone_insertUnique, {result, set, value});
    }
    const uint32_t k = luxRank - 1;
    if (k <= 4 && ((0x1Bu >> (k & 0xFF)) & 1)) luxRank = ld32(g::LUX_RANK_MAP + 4 * k);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t copy = f.alloc(20);
    call(g::FunctionVB_copy, {copy, done});
    const uint32_t fo = f.alloc(20);
    heapFunction(fo, VT_SearchDone, 0x20, 8, g::FunctionVB_move, copy);
    rw::call(g::APIManager_executePartyInviteSearch, {api, levelRange, set, frequency, luxRank, fo});
    functionDestroy(fo);
    functionDestroy(copy);
    call(g::TreeTimezone_destroy, {set, ld32(set + 4)});
}

// Search by ID: the ID popup with its edit box; OK searches (its lambda), Close cancels.
void ScenePartyRequest::openPopUpSearchID(uint32_t me) {
    call(g::AudioPlaySystemSEPopUp, {});
    const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
    const uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
    {
        const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(fadeTime()), opacity})});
        vcall(layer, node::addChild, {layer, shadow});
    }
    const uint32_t widget = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_IdSearchJson});
    vcall(shadow, node::addChild, {shadow, widget});
    Frame f;
    {
        const uint32_t anchor = f.alloc(8);
        call(g::Vec2_ctorXY, {anchor, fbits(0.5f), fbits(0.5f)});
        vcall(widget, node::setAnchorPoint, {widget, anchor});
        call(g::Vec2_dtor, {anchor});
    }
    vcall(widget, node::setPosition, {widget, vcall(shadow, node::getPosition, {shadow})});
    textThenShadow(findNode(widget, S_2LineLabel, 0xB), TXT_ID_WORDING);
    titleLabel(widget, S_TxtID, 6, TXT_ID);
    const uint32_t field = findNode(widget, S_TextFieldID, 0xC);
    st32(me + F_ID_TEXT, call(g::SceneUtil_textFieldToUiText, {field, S_Empty, S_FontSoGei}));
    st8(me + F_EDIT_CLOSING, 0);
    uint32_t editBox;
    {
        const uint32_t placeholder = f.alloc(12);
        vcall(field, V_getString, {placeholder, field});
        const uint32_t size = f.alloc(8);
        call(g::Size_ctor, {size, fbits(400.0f), fbits(88.0f)});
        editBox = rw::call(g::SceneUtil_textFieldToEditBox, {me + F_EDIT_NODE, me + F_EDIT_LABEL, field, placeholder,
                                                             S_Empty, S_FontSoGei, size, me + F_ANIM_A,
                                                             me + F_EDIT_CLOSING, 0, 0});
        strFree(placeholder);
    }
    setVisible(editBox, 0);
    const uint32_t ok = findNode(widget, S_ButtonOK, 9);
    const uint32_t close = findNode(widget, S_ButtonClose, 0xC);
    {
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x24});
        if (obj) {
            st32(obj, VT_IdEdited);
            const uint32_t caps[] = {editBox, me, ok, close, close, shadow, widget, layer};
            for (uint32_t k = 0; k < 8; k++) st32(obj + 4 + 4 * k, caps[k]);
        }
        st32(fo + 16, obj);
        call(g::FunctionVV_swap, {fo, me + F_ON_ID_POPUP});
        functionDestroy(fo);
    }
    textThenShadow(findNode(widget, S_TxtOK, 6), TXT_OK);
    addTouch(ok, VT_IdOk, {me, ok, close});
    textThenShadow(findNode(widget, S_TxtCancel, 0xA), TXT_CANCEL);
    addTouch(close, VT_IdClose, {});
    {
        const uint32_t m = sceneManager();
        Frame t;
        const uint32_t check = newFunction(t, VT_IdCheck, {close});
        const uint32_t result = t.alloc(20);
        st32(result + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x1C});
        if (obj) {
            st32(obj, VT_IdResult);
            const uint32_t caps[] = {me, ok, close, shadow, widget, layer};
            for (uint32_t k = 0; k < 6; k++) st32(obj + 4 + 4 * k, caps[k]);
        }
        st32(result + 16, obj);
        rw::call(g::SM_openPopup, {m, check, result, 1});
        functionDestroy(result);
        functionDestroy(check);
    }
    {
        Frame t;
        const uint32_t none = newEmptyFunction(t);
        const uint32_t action = call(g::SceneUtil_createOpenPopupAnimation, {widget, none});
        call(g::Node_runAction, {widget, call(g::EaseSineOut_create, {action})});
        functionDestroy(none);
    }
}

// The ID popup's result {this, ok, close, shadow, widget, layer} (false: cancelled): buttons off, then close.
void ScenePartyRequest::idResult(uint32_t functor, uint32_t okPtr) {
    if (ld8(okPtr)) return;
    const uint32_t me = ld32(functor + 4);
    call(g::AudioPlaySystemSECancel, {});
    st8(me + F_EDIT_CLOSING, 1);
    setTouch(ld32(functor + 8), 0);
    setTouch(ld32(functor + 0xC), 0);
    call(g::Request_closePopup, {functor + 0x10});
}

// Close the ID popup (closure {shadow, widget, layer}): the shadow fades, the eased close animation removes both.
void ScenePartyRequest::closeIdPopup(uint32_t closure) {
    const uint32_t shadow = ld32(closure);
    call(g::Node_runAction, {shadow, call(g::FadeTo_create, {fbits(fadeTime()), 0})});
    const uint32_t widget = ld32(closure + 4);
    Frame f;
    const uint32_t fo = newFunction(f, VT_IdClosed, {ld32(closure + 8), ld32(closure), widget});
    const uint32_t action = call(g::SceneUtil_createClosePopupAnimation, {widget, fo});
    call(g::Node_runAction, {widget, call(g::EaseSineOut_create, {action})});
    functionDestroy(fo);
}

// The close animation ended {layer, shadow, widget}: both removed from the layer.
void ScenePartyRequest::idClosed(uint32_t functor) {
    vcall(ld32(functor + 4), V_removeChild, {ld32(functor + 4), ld32(functor + 8), 1});
    vcall(ld32(functor + 4), V_removeChild, {ld32(functor + 4), ld32(functor + 0xC), 1});
}

RW_FUNCTION(0x657A80, "ScenePartyRequest::create", ScenePartyRequest::create);
RW_FUNCTION(0x657AD4, "ScenePartyRequest::ScenePartyRequest", ScenePartyRequest::construct);
RW_FUNCTION(0x6D6050, "ScenePartyRequest::~ScenePartyRequest", ScenePartyRequest::destruct);
RW_FUNCTION(0x6D6120, "ScenePartyRequest::init", ScenePartyRequest::init);
RW_FUNCTION(0x6D6A60, "ScenePartyRequest::init list loader (sub_6D6A60)", ScenePartyRequest::loadList);
RW_FUNCTION(0x6D6C44, "ScenePartyRequest::init search filter button (sub_6D6C44)", ScenePartyRequest::searchButton);
RW_FUNCTION(0x6D6FC8, "ScenePartyRequest::runInAnimation", ScenePartyRequest::runInAnimation);
RW_FUNCTION(0x6D7190, "ScenePartyRequest::runInJoinLayout", ScenePartyRequest::runInJoinLayout);
RW_FUNCTION(0x6D7584, "ScenePartyRequest::runOutAnimationMain", ScenePartyRequest::runOutAnimationMain);
RW_FUNCTION(0x6DD290, "ScenePartyRequest::backScene", ScenePartyRequest::backScene);
RW_FUNCTION(0x6DD3C8, "ScenePartyRequest::backScene lambda __func::operator() (sub_6DD3C8)", ScenePartyRequest::backDone);
RW_FUNCTION(0x6D5D1C, "ScenePartyRequest::callUserSearchAPI", ScenePartyRequest::callUserSearchAPI);
RW_FUNCTION(0x6DB8D0, "ScenePartyRequest::openPopUpSearchID", ScenePartyRequest::openPopUpSearchID);
RW_FUNCTION(0x6DED2A, "ScenePartyRequest ID popup result __func::operator()(bool) (sub_6DED2A)",
            ScenePartyRequest::idResult);
RW_FUNCTION(0x6DEDA4, "ScenePartyRequest ID popup close (sub_6DEDA4)", ScenePartyRequest::closeIdPopup);
RW_FUNCTION(0x6DEEE2, "ScenePartyRequest ID popup closed __func::operator() (sub_6DEEE2)", ScenePartyRequest::idClosed);
