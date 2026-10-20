// Party > Manage: ScenePartyManagement (a leader's join settings: the "join" layout with the invitation / applicant
// lists, or the initial layout for a party without members yet), the left/right tab switch between its two lists,
// and the API chain that opens it from the party top (applicants, then invitations).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1, Layer_init = 0x88A8A1, Layer_dtor = 0x88A88D;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Sprite_create = 0x8A64DD;
constexpr uint32_t Color3B_ctor = 0x9062E7;
constexpr uint32_t Rect_ctor = 0x8DF18B;                   // Rect(x, y, w, h)
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t EventListenerTouchOneByOne_create = 0x8FACA5, EventListenerTouchOneByOne_setSwallowTouches = 0x8FAC99;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = 0x8F3649;
constexpr uint32_t FunctionBTE_swap = 0x483E4D;            // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;           // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionVB_assign = 0x490109;           // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t FunctionVB_copy = 0x45C84D;             // std::function<void(bool)>(const function&)
constexpr uint32_t FunctionVB_move = 0x47BC69;             // std::function<void(bool)>(function&&)
constexpr uint32_t Swap_ull = 0x65F595;                    // std::function<void(ull)>::swap
constexpr uint32_t ImageButton_create = 0x531239, ImageButton_setTargetImageView = 0x531631;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageButton_setSelectEnabled = 0x5318F1, ImageButton_setIsSelected = 0x5318FD;
constexpr uint32_t IScrollProtocol_updateLayout = 0x4F6CF5, IScrollProtocol_setVisibleWidgets = 0x4F711D;
constexpr uint32_t IScrollProtocol_setMaxColumn = 0x4F719D;
constexpr uint32_t PartyScrollView_resetScrollPosition = 0x51F0C9;
constexpr uint32_t MemberScrollView_create = 0x5208F9, MemberScrollView_setEnabledTapItem = 0x523555;
constexpr uint32_t MemberScrollView_resetScrollPosition = 0x52355D;
constexpr uint32_t OfferVector_copy = 0x5232D1, OfferVector_dtor = 0x4B3E5D;          // vector<PartyInviteList::Offer>
constexpr uint32_t ApplicantVector_copy = 0x523325, ApplicantVector_dtor = 0x4B4453;  // vector<PartyApplication::Applicant>
constexpr uint32_t VectorULL_dtor = 0x468499;
constexpr uint32_t VectorULL_pushBackSlowMove = 0x4B33F9;  // vector<ull>::__push_back_slow_path(ull&&)
constexpr uint32_t BaseScrollViewULL_setItemIDs = 0x4FD231;
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1; // (node, const string& name, text id)
constexpr uint32_t SM_setOnBackKeyCallback = 0x613E5B, SM_setBackButtonType = 0x613E27, SM_showHeader = 0x6131ED;
constexpr uint32_t SM_changeBackground = 0x6128ED, SM_changeTickerMessage = 0x610B39, SM_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SM_changeSceneLayer = 0x612B99;         // (sm, const function<Layer*()>&, bool, bool, FadeType)
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;   // (anim, std::function<void()> by value)
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595;     // (function<void(bool)> by value)
constexpr uint32_t APIManager_executePartyApplicantList = 0x4CF7AD;   // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_executePartyInviteList = 0x4D0B99;      // (api, const function<void(bool)>&)
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t runOutEntryLayout = 0x6E8AF9;           // ScenePartyManagement::runOutEntryLayout(function<void()>)
constexpr uint32_t ApplicantLambda_vector = 0x4B4453;
// GOT: the class vtable, the __func vtable of a std::function<Layer*()> holding a function pointer, the factories
constexpr uint32_t GOT_VTABLE = 0xFD68CC, GOT_FUNC_LAYER_VT = 0xFD6074;
constexpr uint32_t GOT_CREATE_MANAGEMENT = 0xFD68C8, GOT_CREATE_PARTY_TOP = 0xFD6620;
}  // namespace g

constexpr uint32_t SIZE = 0x2C8;
// fields
constexpr uint32_t F_INOUT = 0x288, F_ROOT = 0x28C;
constexpr uint32_t F_JOIN = 0x290;           // the party takes join requests (the join layout)
constexpr uint32_t F_HAS_LISTS = 0x291;      // invitations / applicants are shown (the join layout's lists)
constexpr uint32_t F_LEFT_TAB = 0x292;       // the left tab is selected
constexpr uint32_t F_LIST_A = 0x294, F_LIST_B = 0x298;           // PartyScrollView (entry layout)
constexpr uint32_t F_INVITES = 0x29C, F_APPLICANTS = 0x2A0;      // MemberScrollView types 2 and 3
constexpr uint32_t F_CALLBACK = 0x2A8;       // std::function (callable at +0x2B8)
constexpr uint32_t F_CAN_BACK = 0x2C0;
// MemberScrollView members (std::function<void(ull)>)
constexpr uint32_t LIST_ON_INVITE = 0x698, LIST_ON_APPLICANT = 0x6B0, LIST_ON_APPLICANT2 = 0x6C8;

// __func vtables (+8) of the lambdas
constexpr uint32_t VT_OnEnter = 0xF69AC0, VT_OnExit = 0xF69A94, VT_BackKey = 0xF69A68, VT_BackButton = 0xF69A3C;
constexpr uint32_t VT_SwallowTouch = 0xF69A10;   // init {sprite}: the shade sprite swallows touches
constexpr uint32_t VT_LeftTab = 0xF699E4, VT_RightTab = 0xF699B8;   // init {this, left, right, panel} (heap 0x14)
constexpr uint32_t VT_OutButton = 0xF69884;      // runInJoinLayout {this}
constexpr uint32_t VT_InviteSelected = 0xF697A8; // {this, vector<Offer>} (heap 0x14)
constexpr uint32_t VT_ApplicantSelected = 0xF696CC, VT_ApplicantSelected2 = 0xF695F0;   // {this, vector<Applicant>, shared_ptr} (0x1C)
constexpr uint32_t VT_Back = 0xF691D0;           // backScene {}: sub_6E9850
constexpr uint32_t VT_ApplicantsDone = 0xF69AEC; // callAPIs {function<void(bool)>} (heap 0x20): sub_6F0748
constexpr uint32_t VT_InvitesDone = 0xF69B18;    // sub_6F0748's {function<void(bool)>} (heap 0x20)
constexpr uint32_t VT_ManageOut = 0xF624A0;      // ScenePartyTop management button {bool}: sub_659630
constexpr uint32_t VT_ManageHeader = 0xF624CC;   // sub_659630's header callback {}

// strings
constexpr uint32_t S_Bg = 0xE03AF2;
constexpr uint32_t S_JoinScene = 0xE0609C, S_InitialScene = 0xE06140;
constexpr uint32_t S_ButtonInvitationRequest = 0xE060D9, S_ButtonJoinApproval = 0xE060F2;
constexpr uint32_t S_TxtInvitationRequest = 0xE06106, S_TxtJoinApproval = 0xE0611C, S_TxtConpaneBottom = 0xE0612D;
constexpr uint32_t S_ButtonJoinRequest = 0xE06180, S_ButtonInvitationApproval = 0xE06193;
constexpr uint32_t S_TxtJoinRequest = 0xE061AD, S_TxtInvitationApproval = 0xE061BD, S_DummyConpaneBottom = 0xE061D4;
constexpr uint32_t S_WinBottom = 0xE058DC, S_ButtonBack = 0xE03DBA, S_BgShadeBottom = 0xE061E9, S_But04 = 0xDF93D9;
constexpr uint32_t S_ImageButtonLeft = 0xE061F9, S_ImageButtonRight = 0xE0620A;
constexpr uint32_t S_RightUI = 0xDF85F7, S_LeftUI = 0xDF85F0, S_TxtOut = 0xE0621C, S_ButtonOut = 0xE06224;
constexpr uint32_t S_TxtDescription = 0xE00690, S_ScrollArea = 0xDF9B59;

constexpr int32_t TXT_INVITATION_REQUEST = 103800035, TXT_JOIN_APPROVAL = 103800036, TXT_TICKER_JOIN = 100620043;
constexpr int32_t TXT_JOIN_BOTTOM = 103850016;
constexpr int32_t TXT_JOIN_REQUEST = 103800054, TXT_INVITATION_APPROVAL = 103800055, TXT_TICKER_INITIAL = 100620039;
constexpr int32_t TXT_INITIAL_BOTTOM = 103810008, TXT_OUT = 103800034, TXT_DESCRIPTION = 103850015;

constexpr uint32_t API_PARTY = 0x84;          // the user's party SafeBuffer: 0x10 join flag, 0x11 / 0x12 list flags
constexpr uint32_t API_INVITES = 0x230;       // shared_ptr<PartyInviteList> (offers vector first)
constexpr uint32_t API_APPLICANTS = 0x220;    // shared_ptr<PartyApplication> (applicants first, shared_ptr at +0xC)
constexpr uint32_t API_LAMBDA_PTR = 0x1D4;    // a shared_ptr captured by the applicant lambdas
constexpr uint32_t API_HOLD = 0x1F8;          // a shared_ptr's control kept alive during runInJoinLayout
constexpr uint32_t SM_FLAG_308 = 0x308, SM_FLAG_4CC = 0x4CC, SM_CALLBACK_548 = 0x548;
constexpr uint32_t OFFER_SIZE = 0x38;         // PartyInviteList::Offer and PartyApplication::Applicant
constexpr uint32_t V_deleting = 0x04, V_init = 0x248, V_runInAnimation = 0x300, V_widgetSetTouchEnabled = 0x29C;
constexpr uint32_t V_setOpacity = 0x214, V_setTextureRect = 0x284;

uint32_t api() { return call(fn::APIManager_getInstance, {}); }
inline void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
void addRefIf(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
void changeScene(uint32_t m, uint32_t gotFactory) {
    Frame f;
    const uint32_t layer = newFunction(f, ld32(g::GOT_FUNC_LAYER_VT) + 8, {ld32(gotFactory)});
    rw::call(g::SM_changeSceneLayer, {m, layer, 1, 1, 1});
    functionDestroy(layer);
}
// a SafeBuffer byte
uint32_t sbByte(uint32_t sb, uint32_t offset) {
    Frame f;
    const uint32_t b = f.alloc(4);
    rw::call(g::SafeBuffer_read, {sb, b, ld32(sb + 0xC) + offset, 1, offset});
    return ld8(b);
}
void setTextFromId(uint32_t text, int32_t id) {
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t s = newStr(f, textDataStr(td));
    call(g::Text_setString, {text, s});
    strFree(s);
    sharedRelease(td);
}
// text = TextData(id) used as a format with two ints
void setTextFormat(uint32_t text, int32_t id, uint32_t a, uint32_t b) {
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t buf = stringBuffer(f, {textDataStr(td), a, b});
    setTextC(text, buf);
    sharedRelease(td);
}
void titleLabelOf(uint32_t node, uint32_t name, uint32_t len, int32_t id) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    call(g::SceneUtil_settingTitleLabel, {node, s, uint32_t(id)});
    strFree(s);
}
// vector<ull>{0, 1, ..., n-1} of the entries of a vector of 0x38-byte records
void indexIds(uint32_t ids, uint32_t records) {
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    if (ld32(records + 4) == ld32(records)) return;
    Frame f;
    const uint32_t tmp = f.alloc(8);
    for (uint32_t i = 0;;) {
        st32(tmp, i);
        st32(tmp + 4, uint32_t(int32_t(i) >> 31));
        const uint32_t end = ld32(ids + 4);
        if (end >= ld32(ids + 8)) {
            call(g::VectorULL_pushBackSlowMove, {ids, tmp});
        } else {
            uint32_t n = 0;
            if (end) {
                st32(end, i);
                st32(end + 4, uint32_t(int32_t(i) >> 31));
                n = ld32(ids + 4);
            }
            st32(ids + 4, n + 8);
        }
        i++;
        const uint32_t count = uint32_t(int32_t(ld32(records + 4) - ld32(records)) >> 3) * 0xB6DB6DB7u;
        if (i >= count) break;
    }
}
}  // namespace

struct ScenePartyManagement {
    static uint32_t create();
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static void runInJoinLayout(uint32_t self);
    static void runOutAnimationMain(uint32_t self, uint32_t done);
    static void runOutJoinLayout(uint32_t self, uint32_t done);
    static void backScene(uint32_t self);
    static void backDone(uint32_t functor);
    static void selectTab(uint32_t captures, uint32_t left);
    static void callAPIs(uint32_t done);
    static void applicantsDone(uint32_t functor, uint32_t okPtr);
    static void destroyApplicantLambda(uint32_t functor);
    static void manageTouched(uint32_t functor, uint32_t sender, uint32_t type);
    static void manageOutDone(uint32_t functor);
};

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t ScenePartyManagement::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    const uint32_t vt = ld32(g::GOT_VTABLE);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    st8(me + F_JOIN, 0);
    st8(me + F_HAS_LISTS, 0);
    st8(me + F_LEFT_TAB, 1);
    st32(me + F_CALLBACK + 0x10, 0);
    st8(me + F_CAN_BACK, 0);
    st32(me + F_APPLICANTS, 0);
    st32(me + F_INVITES, 0);
    st32(me + F_LIST_B, 0);
    st32(me + F_LIST_A, 0);
    if (vcall(me, V_init, {me}) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, V_deleting, {me});
    return 0;
}

uint32_t ScenePartyManagement::destruct(uint32_t me) {
    const uint32_t vt = ld32(g::GOT_VTABLE);
    st32(me, vt + 8);
    st32(me + F_INOUT, vt + 0x318);
    functionDestroy(me + F_CALLBACK);
    call(g::Layer_dtor, {me});
    return me;
}

// The join layout (the party takes join requests: tabs for invitations / approvals) or the initial layout; a
// transparent touch-swallowing sprite over the shade, and the two tab buttons.
uint32_t ScenePartyManagement::init(uint32_t me) {
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
    const uint32_t a = api();
    const uint32_t partyCtl = ld32(a + API_PARTY + 4), party = ld32(a + API_PARTY);
    addRefIf(partyCtl);
    st8(me + F_HAS_LISTS, 0);
    st8(me + F_JOIN, 0);
    if (party) {
        st8(me + F_JOIN, sbByte(party, 0x10));
        uint32_t lists = 1;
        if (!sbByte(party, 0x11)) lists = sbByte(party, 0x12) ? 1 : 0;
        st8(me + F_HAS_LISTS, lists);
    }
    uint32_t leftButton, rightButton, bottomText;
    if (ld8(me + F_JOIN)) {
        {
            const uint32_t s = newStr(f, S_JoinScene, 0x3C);
            st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
            strFree(s);
        }
        leftButton = findNode(ld32(me + F_ROOT), S_ButtonInvitationRequest, 0x18);
        rightButton = findNode(ld32(me + F_ROOT), S_ButtonJoinApproval, 0x13);
        titleLabelOf(leftButton, S_TxtInvitationRequest, 0x15, TXT_INVITATION_REQUEST);
        titleLabelOf(rightButton, S_TxtJoinApproval, 0x10, TXT_JOIN_APPROVAL);
        {
            const uint32_t m = sceneManager();
            const uint32_t td = textData(f, TXT_TICKER_JOIN);
            const uint32_t s = newStr(f, textDataStr(td));
            call(g::SM_changeTickerMessage, {m, s});
            strFree(s);
            sharedRelease(td);
        }
        bottomText = findNode(ld32(me + F_ROOT), S_TxtConpaneBottom, 0x12);
        setTextFormat(bottomText, TXT_JOIN_BOTTOM, 30, 72);
    } else {
        {
            const uint32_t s = newStr(f, S_InitialScene, 0x3F);
            st32(me + F_ROOT, SceneUtil::createNodeWithSceneFile(s));
            strFree(s);
        }
        leftButton = findNode(ld32(me + F_ROOT), S_ButtonJoinRequest, 0x12);
        rightButton = findNode(ld32(me + F_ROOT), S_ButtonInvitationApproval, 0x19);
        titleLabelOf(leftButton, S_TxtJoinRequest, 0xF, TXT_JOIN_REQUEST);
        titleLabelOf(rightButton, S_TxtInvitationApproval, 0x16, TXT_INVITATION_APPROVAL);
        {
            const uint32_t m = sceneManager();
            const uint32_t td = textData(f, TXT_TICKER_INITIAL);
            const uint32_t s = newStr(f, textDataStr(td));
            call(g::SM_changeTickerMessage, {m, s});
            strFree(s);
            sharedRelease(td);
        }
        const uint32_t dummy = findNode(ld32(me + F_ROOT), S_DummyConpaneBottom, 0x14);
        bottomText = findNode(dummy, S_TxtConpaneBottom, 0x12);
        setTextFormat(bottomText, TXT_INITIAL_BOTTOM, 5, 72);
    }
    SceneUtil::setShadow(bottomText);
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
    // a black, transparent sprite over the bottom pane that swallows touches
    const uint32_t sprite = call(g::Sprite_create, {});
    {
        const uint32_t black = f.alloc(4);
        call(g::Color3B_ctor, {black, 0, 0, 0});
        vcall(sprite, node::setColor, {sprite, black});
    }
    vcall(sprite, V_setOpacity, {sprite, 0});
    const uint32_t origin = f.alloc(8), size = f.alloc(8);
    call(fn::Director_getVisibleOrigin, {origin, director()});
    call(fn::Director_getVisibleSize, {size, director()});
    {
        const uint32_t rect = f.alloc(16);
        call(g::Rect_ctor, {rect, fbits(480.0f), fbits(489.0f), fbits(ldf(origin) + ldf(size)), fbits(101.0f)});
        vcall(sprite, V_setTextureRect, {sprite, rect});
        call(g::Vec2_dtor, {rect});
    }
    {
        const uint32_t pos = f.alloc(8);
        call(g::Vec2_ctorXY, {pos, 0, 0});
        vcall(sprite, node::setPosition, {sprite, pos});
        call(g::Vec2_dtor, {pos});
    }
    {
        const uint32_t dispatcher = ld32(director() + 0x84);
        const uint32_t listener = call(g::EventListenerTouchOneByOne_create, {});
        call(g::EventListenerTouchOneByOne_setSwallowTouches, {listener, 1});
        const uint32_t fo = newFunction(f, VT_SwallowTouch, {sprite});
        call(g::FunctionBTE_swap, {fo, listener + 0x50});
        functionDestroy(fo);
        call(g::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, sprite});
    }
    vcall(leftButton, node::addChildZ, {leftButton, sprite, 2});
    const uint32_t panel = findNode(ld32(me + F_ROOT), S_DummyConpaneBottom, 0x14);
    {
        const uint32_t shade = findNode(ld32(me + F_ROOT), S_BgShadeBottom, 0xF);
        vcall(shade, V_widgetSetTouchEnabled, {shade, 1});
    }
    const auto tabButton = [&](uint32_t target, uint32_t name, uint32_t len) {
        uint32_t button;
        {
            Frame b;
            const uint32_t s = newStr(b, S_But04, 5);
            button = call(g::ImageButton_create, {s, 0});
            strFree(s);
        }
        {
            Frame b;
            const uint32_t s = newStr(b, name, len);
            vcall(target, node::addChildZName, {target, button, 10, s});
            strFree(s);
        }
        call(g::ImageButton_setTargetImageView, {button, target});
        return button;
    };
    const uint32_t left = tabButton(leftButton, S_ImageButtonLeft, 0x10);
    const uint32_t right = tabButton(rightButton, S_ImageButtonRight, 0x11);
    const uint32_t captures = f.alloc(16);
    st32(captures, me);
    st32(captures + 4, left);
    st32(captures + 8, right);
    st32(captures + 12, panel);
    st8(me + F_LEFT_TAB, 0);
    selectTab(captures, 1);
    for (const auto &[button, vt] : {std::pair{left, VT_LeftTab}, std::pair{right, VT_RightTab}}) {
        Frame b;
        const uint32_t fo = b.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x14});
        if (obj) {
            st32(obj, vt);
            st32(obj + 4, me);
            st32(obj + 8, left);
            st32(obj + 12, right);
            st32(obj + 16, panel);
        }
        st32(fo + 16, obj);
        call(g::ImageButton_addTouchEventListener, {button, fo});
        functionDestroy(fo);
    }
    call(g::Vec2_dtor, {origin});
    releaseIf(partyCtl);
    return 1;
}

// The join layout slides in: the leave button, the description, and (with lists) the invitation and applicant
// lists with their selection callbacks.
void ScenePartyManagement::runInJoinLayout(uint32_t me) {
    Frame f;
    {
        const uint32_t node = findNode(ld32(me + F_ROOT), S_RightUI, 7);
        const uint32_t none = newEmptyFunction(f);
        call(g::Node_runAction, {node, SceneUtil::createRightInAnimation(node, 0.0f, 0.0f, none)});
        functionDestroy(none);
    }
    const uint32_t a = api();
    const uint32_t holdCtl = ld32(a + API_LAMBDA_PTR + 4), holdPtr = ld32(a + API_LAMBDA_PTR);
    addRefIf(holdCtl);
    const uint32_t hold2Ctl = ld32(api() + API_HOLD);
    addRefIf(hold2Ctl);
    api();
    {
        const uint32_t text = findNode(ld32(me + F_ROOT), S_TxtOut, 7);
        setTextFromId(text, TXT_OUT);
        SceneUtil::setShadow(text);
    }
    addTouch(findNode(ld32(me + F_ROOT), S_ButtonOut, 0xA), VT_OutButton, {me});
    {
        const uint32_t node = findNode(ld32(me + F_ROOT), S_LeftUI, 6);
        const uint32_t none = newEmptyFunction(f);
        call(g::Node_runAction, {node, SceneUtil::createLeftInAnimation(node, 0.0f, 0.0f, none)});
        functionDestroy(none);
    }
    {
        const uint32_t text = findNode(ld32(me + F_ROOT), S_TxtDescription, 0xF);
        setVisible(text, ld8(me + F_HAS_LISTS) ? 0 : 1);
        SceneUtil::setShadow(text);
        setTextFromId(text, TXT_DESCRIPTION);
    }
    if (ld8(me + F_HAS_LISTS)) {
        const uint32_t area = findNode(ld32(me + F_ROOT), S_ScrollArea, 0xB);
        const auto addList = [&](uint32_t field, uint32_t type) {
            st32(me + field, call(g::MemberScrollView_create, {type}));
            vcall(area, node::addChild, {area, ld32(me + field)});
            call(g::IScrollProtocol_setMaxColumn, {ld32(me + field), 1});
            call(g::MemberScrollView_setEnabledTapItem, {ld32(me + field), 0});
            const uint32_t view = ld32(me + field);
            vcall(view, node::setContentSize, {view, vcall(area, node::getContentSize, {area})});
        };
        // the invitations
        addList(F_INVITES, 2);
        const uint32_t a2 = api();
        const uint32_t invitesCtl = ld32(a2 + API_INVITES + 4), invites = ld32(a2 + API_INVITES);
        addRefIf(invitesCtl);
        const uint32_t offers = f.alloc(12);
        call(g::OfferVector_copy, {offers, invites});
        {
            const uint32_t view = ld32(me + F_INVITES);
            const uint32_t copy = f.alloc(12);
            call(g::OfferVector_copy, {copy, offers});
            const uint32_t fo = f.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x14});
            if (obj) {
                st32(obj, VT_InviteSelected);
                st32(obj + 8, ld32(copy));
                st32(obj + 12, ld32(copy + 4));
                st32(obj + 16, ld32(copy + 8));
                st32(obj + 4, me);
            }
            st32(copy, 0);
            st32(copy + 4, 0);
            st32(copy + 8, 0);
            st32(fo + 16, obj);
            call(g::Swap_ull, {fo, view + LIST_ON_INVITE});
            functionDestroy(fo);
            call(g::OfferVector_dtor, {copy});
        }
        const uint32_t inviteIds = f.alloc(12);
        indexIds(inviteIds, offers);
        call(g::BaseScrollViewULL_setItemIDs, {ld32(me + F_INVITES), inviteIds});
        // the applicants
        addList(F_APPLICANTS, 3);
        const uint32_t a3 = api();
        const uint32_t appCtl = ld32(a3 + API_APPLICANTS + 4), app = ld32(a3 + API_APPLICANTS);
        addRefIf(appCtl);
        const uint32_t applicants = f.alloc(12);
        call(g::ApplicantVector_copy, {applicants, app});
        const uint32_t innerCtl = ld32(app + 0x10);
        addRefIf(innerCtl);
        for (const auto &[member, vt] : {std::pair{LIST_ON_APPLICANT, VT_ApplicantSelected},
                                          std::pair{LIST_ON_APPLICANT2, VT_ApplicantSelected2}}) {
            const uint32_t view = ld32(me + F_APPLICANTS);
            Frame l;
            const uint32_t copy = l.alloc(12);
            call(g::ApplicantVector_copy, {copy, applicants});
            addRefIf(holdCtl);
            const uint32_t fo = l.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x1C});
            if (obj) {
                st32(obj, vt);
                st32(obj + 8, ld32(copy));
                st32(obj + 12, ld32(copy + 4));
                st32(obj + 16, ld32(copy + 8));
                st32(obj + 20, holdPtr);
                st32(obj + 24, holdCtl);
                st32(obj + 4, me);
            }
            st32(copy, 0);
            st32(copy + 4, 0);
            st32(copy + 8, 0);
            st32(fo + 16, obj);
            call(g::Swap_ull, {fo, view + member});
            functionDestroy(fo);
            call(g::ApplicantVector_dtor, {copy});
        }
        const uint32_t appIds = f.alloc(12);
        indexIds(appIds, applicants);
        call(g::BaseScrollViewULL_setItemIDs, {ld32(me + F_APPLICANTS), appIds});
        call(g::VectorULL_dtor, {appIds});
        releaseIf(innerCtl);
        call(g::ApplicantVector_dtor, {applicants});
        releaseIf(appCtl);
        call(g::VectorULL_dtor, {inviteIds});
        call(g::OfferVector_dtor, {offers});
        releaseIf(invitesCtl);
    }
    releaseIf(hold2Ctl);
    releaseIf(holdCtl);
}

// ------------------------------------------------------------------------------------------------ out / back
void ScenePartyManagement::runOutAnimationMain(uint32_t me, uint32_t done) {
    call(g::SM_setCanTouchHeader, {sceneManager(), 0});
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    if (ld8(me + F_JOIN)) runOutJoinLayout(me, copy);
    else call(g::runOutEntryLayout, {me, copy});
    functionDestroy(copy);
}

void ScenePartyManagement::runOutJoinLayout(uint32_t me, uint32_t done) {
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

void ScenePartyManagement::backScene(uint32_t me) {
    if (!ld8(me + F_CAN_BACK)) return;
    st8(me + F_CAN_BACK, 0);
    st8(sceneManager() + SM_FLAG_308, 0);
    Frame f;
    const uint32_t fo = newFunction(f, VT_Back, {});
    call(g::IInOutAnimation_runOutAnimation, {me + F_INOUT, fo});
    functionDestroy(fo);
}

// backScene's lambda: refresh the party top and return there.
void ScenePartyManagement::backDone(uint32_t) {
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
    changeScene(sceneManager(), g::GOT_CREATE_PARTY_TOP);
}

// The tab switch (captures {this, left button, right button, bottom panel}): select the left (true) or right tab and
// show its list.
void ScenePartyManagement::selectTab(uint32_t captures, uint32_t left) {
    const uint32_t me = ld32(captures);
    const uint32_t flag = me + F_LEFT_TAB;
    if (ld8(flag) == (left & 0xFF)) return;
    st8(flag, left);
    call(g::ImageButton_setSelectEnabled, {ld32(captures + 4), 1});
    call(g::ImageButton_setSelectEnabled, {ld32(captures + 8), 1});
    call(g::ImageButton_setIsSelected, {ld32(captures + 4), ld8(flag)});
    call(g::ImageButton_setIsSelected, {ld32(captures + 8), ld8(flag) ? 0u : 1u});
    setVisible(ld32(captures + 12), left);
    if (ld8(me + F_JOIN)) {
        if (!ld8(me + F_HAS_LISTS)) return;
        if (const uint32_t v = ld32(me + F_APPLICANTS)) call(g::MemberScrollView_resetScrollPosition, {v});
        if (const uint32_t v = ld32(me + F_INVITES)) call(g::MemberScrollView_resetScrollPosition, {v});
        setVisible(ld32(me + F_APPLICANTS), ld8(flag) ? 0 : 1);
        call(g::IScrollProtocol_setVisibleWidgets, {ld32(me + F_APPLICANTS), ld8(flag) ? 0u : 1u});
        if (!ld8(flag)) call(g::IScrollProtocol_updateLayout, {ld32(me + F_APPLICANTS), 0});
        setVisible(ld32(me + F_INVITES), ld8(flag));
        call(g::IScrollProtocol_setVisibleWidgets, {ld32(me + F_INVITES), ld8(flag)});
        if (ld8(flag)) call(g::IScrollProtocol_updateLayout, {ld32(me + F_INVITES), 0});
    } else {
        if (const uint32_t v = ld32(me + F_LIST_A)) call(g::PartyScrollView_resetScrollPosition, {v});
        if (const uint32_t v = ld32(me + F_LIST_B)) call(g::PartyScrollView_resetScrollPosition, {v});
        setVisible(ld32(me + F_LIST_A), ld8(flag));
        call(g::IScrollProtocol_setVisibleWidgets, {ld32(me + F_LIST_A), ld8(flag)});
        uint32_t other = 1;
        if (ld8(flag)) {
            call(g::IScrollProtocol_updateLayout, {ld32(me + F_LIST_A), 0});
            other = ld8(flag) ? 0 : 1;
        }
        setVisible(ld32(me + F_LIST_B), other);
        call(g::IScrollProtocol_setVisibleWidgets, {ld32(me + F_LIST_B), ld8(flag) ? 0u : 1u});
        if (!ld8(flag)) call(g::IScrollProtocol_updateLayout, {ld32(me + F_LIST_B), 0});
    }
}

// callAPIs(done): the applicant list, then (sub_6F0748) the invitation list, then done.
void ScenePartyManagement::callAPIs(uint32_t done) {
    Frame f;
    const uint32_t a = f.alloc(20), b = f.alloc(20), c = f.alloc(20), d = f.alloc(20);
    call(g::FunctionVB_copy, {a, done});
    functionCopy(b, a);
    const uint32_t ap = api();
    functionCopy(c, b);
    heapFunction(d, VT_ApplicantsDone, 0x20, 8, g::FunctionVB_move, c);
    call(g::APIManager_executePartyApplicantList, {ap, d});
    functionDestroy(d);
    functionDestroy(c);
    functionDestroy(b);
    functionDestroy(a);
}

// ...the applicants arrived: ask for the invitations (the callback {done} again).
void ScenePartyManagement::applicantsDone(uint32_t functor, uint32_t okPtr) {
    if (!ld8(okPtr)) return;
    const uint32_t a = api();
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(g::FunctionVB_copy, {copy, functor + 8});
    const uint32_t fo = f.alloc(20);
    heapFunction(fo, VT_InvitesDone, 0x20, 8, g::FunctionVB_move, copy);
    call(g::APIManager_executePartyInviteList, {a, fo});
    functionDestroy(fo);
    functionDestroy(copy);
}

// __func::destroy_deallocate of an applicant lambda {this, vector<Applicant>, shared_ptr}
void ScenePartyManagement::destroyApplicantLambda(uint32_t functor) {
    releaseIf(ld32(functor + 0x18));
    call(g::ApplicantLambda_vector, {functor + 8});
    call(fn::operator_delete, {functor});
}

// ScenePartyTop's management button (touch ended): decide sound, slide the party top out {join flag}.
void ScenePartyManagement::manageTouched(uint32_t functor, uint32_t, uint32_t type) {
    if (ld32(type) != 2) return;
    const uint32_t top = ld32(functor + 4);
    call(g::AudioPlaySystemSEDecide, {});
    const uint32_t flag = ld8(functor + 8);
    Frame f;
    const uint32_t fo = newFunction(f, VT_ManageOut, {});
    st8(fo + 4, flag);
    call(g::IInOutAnimation_runOutAnimation, {top + 0x288, fo});
    functionDestroy(fo);
}

// ...then the management scene: with the join flag after both lists are fetched, else at once.
void ScenePartyManagement::manageOutDone(uint32_t functor) {
    const uint32_t join = ld8(functor + 4);
    const uint32_t m = sceneManager();
    if (join) {
        Frame f;
        {
            const uint32_t fo = newFunction(f, VT_ManageHeader, {});
            st8(m + SM_FLAG_4CC, 1);
            call(g::FunctionVB_assign, {m + SM_CALLBACK_548, fo});
            functionDestroy(fo);
        }
        {
            const uint32_t none = newEmptyFunction(f);
            callAPIs(none);
            functionDestroy(none);
        }
        changeScene(sceneManager(), g::GOT_CREATE_MANAGEMENT);
    } else {
        changeScene(m, g::GOT_CREATE_MANAGEMENT);
    }
}

RW_FUNCTION(0x657634, "ScenePartyManagement::create", ScenePartyManagement::create);
RW_FUNCTION(0x6E6578, "ScenePartyManagement::~ScenePartyManagement", ScenePartyManagement::destruct);
RW_FUNCTION(0x6E6604, "ScenePartyManagement::init", ScenePartyManagement::init);
RW_FUNCTION(0x6E767C, "ScenePartyManagement::runInJoinLayout", ScenePartyManagement::runInJoinLayout);
RW_FUNCTION(0x6E8860, "ScenePartyManagement::runOutAnimationMain", ScenePartyManagement::runOutAnimationMain);
RW_FUNCTION(0x6E8950, "ScenePartyManagement::runOutJoinLayout", ScenePartyManagement::runOutJoinLayout);
RW_FUNCTION(0x6E9734, "ScenePartyManagement::backScene", ScenePartyManagement::backScene);
RW_FUNCTION(0x6E9850, "ScenePartyManagement::backScene lambda __func::operator() (sub_6E9850)", ScenePartyManagement::backDone);
RW_FUNCTION(0x6E74CC, "ScenePartyManagement::init tab switch lambda (sub_6E74CC)", ScenePartyManagement::selectTab);
RW_FUNCTION(0x6E6384, "ScenePartyManagement::callAPIs", ScenePartyManagement::callAPIs);
RW_FUNCTION(0x6F0748, "ScenePartyManagement::callAPIs applicants lambda __func::operator() (sub_6F0748)",
            ScenePartyManagement::applicantsDone);
RW_FUNCTION(0x6EDCFC, "ScenePartyManagement applicant lambda __func::destroy_deallocate() (sub_6EDCFC)",
            ScenePartyManagement::destroyApplicantLambda);
RW_FUNCTION(0x6594E8, "ScenePartyTop management button lambda __func::operator() (sub_6594E8)",
            ScenePartyManagement::manageTouched);
RW_FUNCTION(0x659630, "ScenePartyTop management run-out lambda __func::operator() (sub_659630)",
            ScenePartyManagement::manageOutDone);
