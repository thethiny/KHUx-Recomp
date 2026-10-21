// ScenePartyTop: the Party screen (MyPage: Lux emblem > Party). A Layer with IInOutAnimation at +0x288; the scene
// file's root at +0x28C. Members of the party see the join layout (the party card, the member / manage / ranking /
// invitation buttons and the leader's edit buttons); others the entry layout (Create Party / Find Party / Manage,
// PartyTop_Initial.json).
//
// Layout: +0x290 in a party, +0x291 manager, +0x292 party flag, +0x293 has members, +0x294 show the "party full"
// dialog, +0x295 sub-leader, +0x298 std::function<void()> (the party card's tap), +0x2B0 Back allowed.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneManager.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Layer_ctor = 0x88A7D1;
constexpr uint32_t Layer_dtor = 0x88A88D;
constexpr uint32_t Layer_init = 0x88A8A1;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Function_vv_assign = 0x4EB9CD;          // std::function<void()>::operator=(const function&)
constexpr uint32_t Function_vv_swap = 0x4EE86D;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;   // (sm, const string&)
constexpr uint32_t SceneManager_openPartyNewPopup = 0x61FC21;     // (sm, function<void()> by value)
constexpr uint32_t APIManager_executePartyGet = 0x4C69BD;         // (api, ull id, const function<void(bool)>&)
constexpr uint32_t APIManager_executePaymentInfo = 0x4CE74D;
constexpr uint32_t APIManager_executePartyMemberList = 0x4C7AA5;  // (api, bool, const function&, PlatformType)
constexpr uint32_t APIManager_executePartyNoticeUpdate = 0x4CED11;
constexpr uint32_t APIManager_executePartyLogin = 0x4CEE55;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t ScenePartyTop_runOutEntryLayout = 0x655B35;
constexpr uint32_t EaseOut_create = 0x8606A5;               // (action, float rate)
constexpr uint32_t Text_setTextHorizontalAlignment = 0x83A65F;
constexpr uint32_t Avatar_create = 0x5C5883;                // (const Style&, bool)
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;
constexpr uint32_t AvatarUtil_makeStyle = 0x497105;
constexpr uint32_t AvatarMap_find = 0x490275;               // unordered_map<ull, int>::find (const)
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t ImageView_loadTexture = 0x839731;
constexpr uint32_t SceneUtil_callPrepareMypageAPIs = 0x46F37D;
constexpr uint32_t Function_vb_assign = 0x490109;
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t Misc_create = 0x786D5D;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t APIManager_executePartyApplicationList = 0x4CF4BD;   // (api, const function<void(bool)>&)
constexpr uint32_t APIManager_executePartyOfferList = 0x4D0E89;
}  // namespace g

constexpr uint32_t GOT_FUNC_LAYER_FACTORY_VTABLE = 0xFD6074;
constexpr uint32_t GOT_CREATE_MYPAGE = 0xFD6078;
constexpr uint32_t GOT_CREATE_MANAGEMENT = 0xFD68C8;     // createSceneLayer<ScenePartyManagement>

constexpr uint32_t SIZE = 0x2B8;
constexpr uint32_t VTABLE = 0xF99B68, VTABLE2 = 0xF99E78;
constexpr uint32_t ROOT = 0x28C, IN_PARTY = 0x290, MANAGER = 0x291, PARTY_FLAG = 0x292, HAS_MEMBERS = 0x293,
                   FULL_DIALOG = 0x294, SUB_LEADER = 0x295, CARD_TAP = 0x298, CAN_BACK = 0x2B0;

// strings
constexpr uint32_t S_DeckBG = 0xE03AF2, S_PartyTopJson = 0xE03B0D, S_RightUI = 0xDF85F7, S_CenterUI = 0xE02FBA,
                   S_TxtMemberInvitation = 0xE03B33, S_MemberInvitationButton = 0xE03B48,
                   S_TxtMemberCheck = 0xE03B5F, S_MemberCheckButton = 0xE03B6F, S_TxtManagement = 0xE03B81,
                   S_ManagementButton = 0xE03B90, S_TxtRanking = 0xE03BA1, S_RankingButton = 0xE03BAD,
                   S_TxtInvitation = 0xE03BBB, S_InvitationButton = 0xE03BCA, S_RankIcon = 0xDFDA07,
                   S_TxtName = 0xDFDA10, S_TxtLeader = 0xE00052, S_TxtLeaderLabel = 0xE0005D,
                   S_TxtMessageLabel = 0xDFDA19, S_TxtMembers = 0xE0006E, S_TxtMembersLabel = 0xE0007A,
                   S_CountFmt = 0xDF8F35, S_TxtPlayStyle = 0xE0008C, S_TxtPlayStyleLabel = 0xE0009A,
                   S_TxtJoinType = 0xE000AE, S_TxtJoinTypeLabel = 0xE000BB, S_Avatar = 0xDF8F56,
                   S_AvatarNode = 0xDFE76A, S_ButtonJoinEditCenter = 0xE03BDB, S_ButtonJoinEdit = 0xE03BF2,
                   S_ButtonLeaderChange = 0xE03C02, S_TxtJoinEdit = 0xE03C16, S_TxtLeaderChange = 0xE03C23,
                   S_TxtJoinEditCenter = 0xE03C34, S_Empty = 0xDF8A1A;
constexpr uint32_t S_PartyTopInitialJson = 0xE03C48, S_EntryLayout = 0xE03C71, S_TxtWording1 = 0xDFF14C,
                   S_TxtWording2 = 0xDF863B, S_TxtWording3 = 0xE0014B, S_TxtWording4 = 0xE00158,
                   S_TxtPREdit = 0xE03C7E, S_ButtonPREdit = 0xE03C89, S_TxtInitialRequest = 0xE03C97,
                   S_ButtonInitialRequest = 0xE03CAA, S_TxtInitialManagement = 0xE03CC0,
                   S_ButtonInitialManagement = 0xE03CD6;

// texts
constexpr int32_t TXT_TICKER_PARTY = 100620040, TXT_TICKER_NO_PARTY = 100620037, TXT_PARTY_FULL = 103810008;
constexpr int32_t TXT_MEMBER_INVITATION = 103800001, TXT_MEMBER_CHECK = 103800002, TXT_MANAGEMENT = 103800003,
                  TXT_RANKING = 100100020, TXT_INVITATION = 103800004, TXT_LEADER = 103800040,
                  TXT_MEMBERS = 103800023, TXT_PLAY_STYLE = 102400003, TXT_JOIN_TYPE = 103800019,
                  TXT_UNSET = 103800011, TXT_JOIN_EDIT = 103800005, TXT_LEADER_CHANGE = 103800038;
constexpr int32_t TXT_PLAY_STYLES[3] = {102400004, 102400005, 102400006};
constexpr int32_t TXT_ENTRY_CREATE = 103800058, TXT_ENTRY_FIND = 103800059, TXT_ENTRY_MANAGE = 103800060,
                  TXT_CREATE_PARTY = 103810021, TXT_FIND_PARTY = 103810022, TXT_MANAGE = 103810023,
                  TXT_FIRST_JOIN_REWARD = 103810026;
constexpr uint32_t MISC_FIRST_JOIN_JEWELS = 117;
constexpr int32_t TXT_JOIN_TYPES[3] = {103800049, 103800021, 103800020};
constexpr uint32_t PARTY_MAX = 30;

// lambdas (__func vtables + 8)
constexpr uint32_t VT_OnEnter = 0xF626B0, VT_OnExit = 0xF62684, VT_BackKey = 0xF62658, VT_FullClosed = 0xF6262C;
constexpr uint32_t VT_CreateParty = 0xF62130, VT_FindParty = 0xF620D8, VT_Manage = 0xF62028,
                   VT_ManageOut = 0xF62054, VT_ManageApplications = 0xF62080, VT_ManageError = 0xF620AC;
constexpr uint32_t V_SET_NAME = 0x154;                    // Node::setName(const string&)
constexpr uint32_t VT_MemberInvitation = 0xF625A8, VT_MemberCheck = 0xF624F8, VT_Management = 0xF62474,
                   VT_Ranking = 0xF623F0, VT_Invitation = 0xF623C4, VT_CardTap = 0xF622BC, VT_JoinEdit = 0xF62290,
                   VT_LeaderChange = 0xF62238, VT_JoinEditCenter = 0xF621E0, VT_LeaderChangeCenter = 0xF62188;
constexpr uint32_t VT_Back = 0xF61EC8;
constexpr uint32_t VT_GetParty = 0xF626DC;        // callPartyTopAPI $_5 (heap 0x40, function at +0x28)
constexpr uint32_t VT_PaymentInfo = 0xF6278C;     // callPartyTopAPI $_4 (heap 0x48, function at +0x30)
constexpr uint32_t VT_Chain1 = 0xF62708;          // ... (heap 0x38, function at +0x20): party notice update next
constexpr uint32_t VT_Chain2 = 0xF62734;          // ... (heap 0x30, function at +0x18): party login next
constexpr uint32_t VT_Chain3 = 0xF62760;          // ... (heap 0x28, function at +0x10)

// APIManager
constexpr uint32_t API_LINK = 0x44;               // the platform link (+0x34 a std::string: an invitation id)
constexpr uint32_t API_USER = 0x84;               // the user / party record (SafeBuffer + plain fields)
constexpr uint32_t API_PARTY_INFO = 0x1D4;        // set while in a party (control +0x1D8)
constexpr uint32_t API_PARTY = 0x1EC;             // party state (control +0x1F0): +3 flag, members vector +4
constexpr uint32_t API_LEADER = 0x1F4;            // (control +0x1F8): +0 the player leads the party
constexpr uint32_t API_AVATARS = 0x1FC;           // (control +0x200)

constexpr uint32_t V_RUN_IN_ANIMATION = 0x300;
constexpr uint32_t V_WIDGET_SET_TOUCH_ENABLED = 0x29C;

uint32_t addRef(uint32_t ctrl) {
    if (ctrl) call(fn::shared_add, {ctrl});
    return ctrl;
}
void release(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}
void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }

void assignFunction(uint32_t dst, uint32_t vtable, uint32_t me) {
    Frame f;
    const uint32_t cb = newFunction(f, vtable, {me});
    call(g::Function_vv_assign, {dst, cb});
    functionDestroy(cb);
}

void runAnimation(uint32_t node, uint32_t (*make)(uint32_t, float, float, uint32_t), float rate = 0.0f) {
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    uint32_t action = make(node, 0.0f, 0.0f, none);
    if (rate != 0.0f) action = call(g::EaseOut_create, {action, fbits(rate)});
    call(fn::Node_runAction, {node, action});
    functionDestroy(none);
}
uint32_t rightIn(uint32_t n, float a, float b, uint32_t d) { return SceneUtil::createRightInAnimation(n, a, b, d); }
uint32_t leftIn(uint32_t n, float a, float b, uint32_t d) { return SceneUtil::createLeftInAnimation(n, a, b, d); }

// text <- TextData `id`, with a shadow
void label(uint32_t root, uint32_t name, uint32_t len, int32_t id) {
    const uint32_t text = findNode(root, name, len);
    setTextId(text, id);
    SceneUtil::setShadow(text);
}

// the back-to-MyPage lambda body (backScene's): prepare MyPage's requests and change to MyPage
void backToMyPage() {
    Frame f;
    const uint32_t sm = call(fn::SceneManager_getInstance, {});
    const uint32_t none = newEmptyFunction(f);
    st8(sm + 0x4CC, 1);
    call(g::Function_vb_assign, {sm + 0x548, none});
    functionDestroy(none);
    const uint32_t done = newEmptyFunction(f);
    call(g::SceneUtil_callPrepareMypageAPIs, {done});
    functionDestroy(done);
    const uint32_t m = call(fn::SceneManager_getInstance, {});
    const uint32_t layer = newFunction(f, ld32(GOT_FUNC_LAYER_FACTORY_VTABLE) + 8, {ld32(GOT_CREATE_MYPAGE)});
    call(g::SceneManager_changeSceneLayer, {m, layer, 1, 1, 1});
    functionDestroy(layer);
}

}  // namespace

namespace party_top {

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t init(uint32_t me);

uint32_t create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    call(g::Layer_ctor, {me});
    st32(me, VTABLE);
    st32(me + 0x288, VTABLE2);
    st32(me + 0x2A8, 0);
    st8(me + CAN_BACK, 0);
    for (uint32_t k = 0x295; k >= 0x290; k--) st8(me + k, 0);
    if (init(me) == 1) {
        call(g::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}
RW_FUNCTION(0x53E30C, "ScenePartyTop::create", party_top::create);

uint32_t destroy(uint32_t me) {
    st32(me, VTABLE);
    st32(me + 0x288, VTABLE2);
    functionDestroy(me + CARD_TAP);
    call(g::Layer_dtor, {me});
    return me;
}
RW_FUNCTION(0x652A70, "ScenePartyTop::~ScenePartyTop", party_top::destroy);

// The scene: callbacks, header, background, PartyTopScene; the party flags from the user record, the ticker, the
// "new party" popup, the full-party dialog; then runInAnimation.
uint32_t init(uint32_t me) {
    if (call(g::Layer_init, {me}) != 1) return 0;
    assignFunction(me + 0x210, VT_OnEnter, me);
    assignFunction(me + 0x228, VT_OnExit, me);
    {
        const uint32_t sm = call(fn::SceneManager_getInstance, {});
        Frame f;
        const uint32_t cb = newFunction(f, VT_BackKey, {me});
        SceneManager::setOnBackKeyCallback(rw::host_ptr<SceneManager>(sm), cb);
        functionDestroy(cb);
    }
    st8(me + CAN_BACK, 1);
    SceneManager::setBackButtonType(rw::host_ptr<SceneManager>(call(fn::SceneManager_getInstance, {})), 2);
    SceneManager::showHeader(rw::host_ptr<SceneManager>(call(fn::SceneManager_getInstance, {})));
    {
        const uint32_t sm = call(fn::SceneManager_getInstance, {});
        Frame f;
        const uint32_t name = newStr(f, S_DeckBG, 0x1A);
        SceneManager::changeBackground(rw::host_ptr<SceneManager>(sm), 0, name);
        strFree(name);
    }
    {
        Frame f;
        const uint32_t file = newStr(f, S_PartyTopJson, 0x25);
        st32(me + ROOT, SceneUtil::createNodeWithSceneFile(file));
        strFree(file);
    }
    vcall(me, node::addChild, {me, ld32(me + ROOT)});
    uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t userCtrl = addRef(ld32(api + API_USER + 4));
    const uint32_t user = ld32(api + API_USER);
    st8(me + 0x291, 0);
    st8(me + IN_PARTY, 0);
    st8(me + SUB_LEADER, 0);
    bool inParty = false;
    if (user) {
        Frame f;
        st8(me + IN_PARTY, ld8(safeBytes(f, user, 0x10, 1)));
        st8(me + MANAGER, ld8(safeBytes(f, user, 0x11, 1)));
        st8(me + SUB_LEADER, ld8(safeBytes(f, user, 0x12, 1)));
        inParty = ld8(me + IN_PARTY) != 0;
    }
    {
        const uint32_t sm = call(fn::SceneManager_getInstance, {});
        Frame f;
        const uint32_t td = textData(f, inParty ? TXT_TICKER_PARTY : TXT_TICKER_NO_PARTY);
        const uint32_t s = newStr(f, textDataStr(td));
        call(g::SceneManager_changeTickerMessage, {sm, s});
        strFree(s);
        sharedRelease(td);
    }
    api = call(fn::APIManager_getInstance, {});
    const uint32_t partyCtrl = addRef(ld32(api + API_PARTY + 4));
    const uint32_t party = ld32(api + API_PARTY);
    if (party) {
        st8(me + PARTY_FLAG, ld8(party + 3));
        st8(me + HAS_MEMBERS, ld32(party + 8) != ld32(party + 4) ? 1 : 0);
        st8(me + FULL_DIALOG, 0);
    }
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(g::SceneManager_openPartyNewPopup, {call(fn::SceneManager_getInstance, {}), none});
        functionDestroy(none);
    }
    if (ld8(me + FULL_DIALOG)) {
        Frame f;
        const uint32_t td = textData(f, TXT_PARTY_FULL);
        const uint32_t text = newStr(f, stringBuffer(f, {textDataStr(td), 10}));
        const uint32_t cb = newFunction(f, VT_FullClosed, {});
        SceneUtil::openOkDialog4Line(text, cb, 0);
        functionDestroy(cb);
        strFree(text);
        sharedRelease(td);
    }
    vcall(me, V_RUN_IN_ANIMATION, {me});
    release(partyCtrl);
    release(userCtrl);
    return 1;
}
RW_FUNCTION(0x652AFC, "ScenePartyTop::init", party_top::init);

// The party screen's requests: in a party, the party (executePartyGet, id 0), else the payment info; the callback
// copied down the chain of lambdas.
void callPartyTopAPI(uint32_t callback) {
    Frame f;
    const uint32_t f0 = f.alloc(20), f1 = f.alloc(20), f2 = f.alloc(20), f3 = f.alloc(20);
    call(fn::function_vb_copy, {f0, callback});
    functionCopy(f1, f0);
    functionCopy(f2, f1);
    functionCopy(f3, f2);
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t ctrl = addRef(ld32(api + API_PARTY_INFO + 4));
    const uint32_t partyInfo = ld32(api + API_PARTY_INFO);
    const uint32_t api2 = call(fn::APIManager_getInstance, {});
    const uint32_t copy = f.alloc(20);
    functionCopy(copy, partyInfo ? f2 : f3);
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t size = partyInfo ? 0x40u : 0x48u, at = partyInfo ? 0x28u : 0x30u;
    const uint32_t heap = call(fn::operator_new, {size});
    if (heap) {
        st32(heap, partyInfo ? VT_GetParty : VT_PaymentInfo);
        call(fn::function_vb_move, {heap + at, copy});
    }
    st32(fo + 16, heap);
    if (partyInfo) call(g::APIManager_executePartyGet, {api2, 0, 0, fo});
    else call(g::APIManager_executePaymentInfo, {api2, fo});
    functionDestroy(fo);
    functionDestroy(copy);
    release(ctrl);
    functionDestroy(f3);
    functionDestroy(f2);
    functionDestroy(f1);
    functionDestroy(f0);
}
RW_FUNCTION(0x652594, "ScenePartyTop::callPartyTopAPI", party_top::callPartyTopAPI);

// ------------------------------------------------------------------------------------------------ the join layout
// The party member's screen: the right-hand buttons sliding in (member invitation only for managers, invitation
// only with a platform link), the party card (rank icon, name, leader, message, members, play style, join type,
// the leader's avatar), the card tap ($_16) and the leader / manager edit buttons.
void runInJoinLayout(uint32_t me) {
    Frame f;
    const uint32_t origin = f.alloc(8);
    call(fn::Director_getVisibleOrigin, {origin, call(fn::Director_getInstance, {})});
    const uint32_t right = findNode(ld32(me + ROOT), S_RightUI, 7);
    uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t leaderCtrl = addRef(ld32(api + API_LEADER + 4));
    const uint32_t leader = ld32(api + API_LEADER);
    api = call(fn::APIManager_getInstance, {});
    const uint32_t userCtrl = addRef(ld32(api + API_USER + 4));
    const uint32_t user = ld32(api + API_USER);
    const uint32_t canManage = ld8(me + MANAGER) ? 1 : (ld8(me + SUB_LEADER) ? 1 : 0);
    // the buttons on the right
    label(right, S_TxtMemberInvitation, 0x14, TXT_MEMBER_INVITATION);
    uint32_t button = findNode(right, S_MemberInvitationButton, 0x16);
    setVisible(button, canManage);
    runAnimation(button, rightIn);
    addTouch(button, VT_MemberInvitation, {me});
    label(right, S_TxtMemberCheck, 0xF, TXT_MEMBER_CHECK);
    button = findNode(right, S_MemberCheckButton, 0x11);
    runAnimation(button, rightIn);
    addTouch(button, VT_MemberCheck, {me});
    label(right, S_TxtManagement, 0xE, TXT_MANAGEMENT);
    button = findNode(right, S_ManagementButton, 0x10);
    runAnimation(button, rightIn);
    addTouch(button, VT_Management, {me, canManage});
    label(right, S_TxtRanking, 0xB, TXT_RANKING);
    button = findNode(right, S_RankingButton, 0xD);
    runAnimation(button, rightIn);
    addTouch(button, VT_Ranking, {me});
    label(right, S_TxtInvitation, 0xE, TXT_INVITATION);
    button = findNode(right, S_InvitationButton, 0x10);
    call(fn::APIManager_getInstance, {});
    const uint32_t link = ld32(call(fn::APIManager_getInstance, {}) + API_LINK);
    if (link && (ld8(link + 0x34) & 1)) {
        runAnimation(button, rightIn);
        addTouch(button, VT_Invitation, {me});
    } else {
        setVisible(button, 0);
    }
    // the party card
    const uint32_t center = findNode(ld32(me + ROOT), S_CenterUI, 8);
    runAnimation(center, leftIn, 6.0f);
    {
        const uint32_t icon = findNode(center, S_RankIcon, 8);
        Frame h;
        const uint32_t path = h.alloc(12);
        SceneUtil::createPartyRankImagePath(path, int32_t(ld32(user + 0x2C)));
        const uint32_t s = newStr(h, strData(path));
        call(g::ImageView_loadTexture, {icon, s, 0});
        strFree(s);
        strFree(path);
    }
    {
        const uint32_t name = findNode(center, S_TxtName, 8);
        SceneUtil::setShadow(name);
        call(fn::Text_setString, {name, user + 0x30});
    }
    api = call(fn::APIManager_getInstance, {});
    const uint32_t avatarsCtrl = addRef(ld32(api + API_AVATARS + 4));
    const uint32_t avatars = ld32(api + API_AVATARS);
    const uint32_t key = f.alloc(8);
    st32(key, ld32(user + 0x58));
    st32(key + 4, ld32(user + 0x5C));
    const uint32_t node = call(g::AvatarMap_find, {avatars + 0x24, key});
    const int32_t index = node ? int32_t(ld32(node + 0x10)) : -1;
    const uint32_t entry = safeBytes(f, avatars, uint32_t(296 * index), 0x128);
    label(center, S_TxtLeader, 0xA, TXT_LEADER);
    {
        const uint32_t text = findNode(center, S_TxtLeaderLabel, 0x10);
        SceneUtil::setShadow(text);
        setTextC(text, entry + 0xC);
    }
    {
        const uint32_t text = findNode(center, S_TxtMessageLabel, 0x11);
        call(g::Text_setTextHorizontalAlignment, {text, 3});
        call(fn::Text_setString, {text, user + 0x44});
    }
    label(center, S_TxtMembers, 0xB, TXT_MEMBERS);
    {
        const uint32_t text = findNode(center, S_TxtMembersLabel, 0x11);
        SceneUtil::setShadow(text);
        Frame h;
        setTextC(text, stringBuffer(h, {S_CountFmt, ld32(user + 0x50), PARTY_MAX}));
    }
    label(center, S_TxtPlayStyle, 0xD, TXT_PLAY_STYLE);
    const uint32_t styleLabel = findNode(center, S_TxtPlayStyleLabel, 0x13);
    SceneUtil::setShadow(styleLabel);
    const uint32_t styleText = newStr(f, S_Empty, 0);
    {
        const uint32_t style = ld32(user + 0x3C);
        Frame h;
        const uint32_t td = textData(h, style - 1 < 3 ? TXT_PLAY_STYLES[style - 1] : TXT_UNSET);
        const uint32_t s = textDataStr(td);
        call(0x48D811, {styleText, s, host_strlen(s)});
        sharedRelease(td);
    }
    call(fn::Text_setString, {styleLabel, styleText});
    label(center, S_TxtJoinType, 0xC, TXT_JOIN_TYPE);
    const uint32_t joinLabel = findNode(center, S_TxtJoinTypeLabel, 0x12);
    SceneUtil::setShadow(joinLabel);
    const uint32_t joinText = newStr(f, S_Empty, 0);
    {
        const uint32_t join = ld32(user + 0x40);
        Frame h;
        const uint32_t td = textData(h, join - 1 < 3 ? TXT_JOIN_TYPES[join - 1] : TXT_UNSET);
        const uint32_t s = textDataStr(td);
        call(0x48D811, {joinText, s, host_strlen(s)});
        sharedRelease(td);
    }
    call(fn::Text_setString, {joinLabel, joinText});
    // the leader's avatar
    const uint32_t placeholder = findNode(center, S_Avatar, 6);
    setVisible(placeholder, 0);
    const uint32_t style = f.alloc(0x40);
    call(g::AvatarUtil_makeStyle, {style, entry + 0xE8, 0});
    const uint32_t avatar = call(g::Avatar_create, {style, 0});
    call(g::Avatar_changeMotion, {avatar, 27});
    vcall(avatar, 0x48, {avatar, fbits(1.8f), fbits(1.8f)});   // setScale(x, y)
    vcall(avatar, node::setPosition, {avatar, vcall(placeholder, node::getPosition, {placeholder})});
    {
        const uint32_t parent = vcall(placeholder, node::getParent, {placeholder});
        const uint32_t z = vcall(placeholder, 0x18, {placeholder});   // getLocalZOrder
        Frame h;
        const uint32_t tag = newStr(h, S_AvatarNode, 10);
        vcall(parent, node::addChildZName, {parent, avatar, z, tag});
        strFree(tag);
    }
    call(g::Node_unscheduleUpdate, {ld32(avatar + 0x29C)});
    // the card tap
    {
        addRef(userCtrl);
        Frame h;
        const uint32_t cb = h.alloc(20);
        st32(cb, VT_CardTap);
        st32(cb + 4, me);
        st32(cb + 8, user);
        st32(cb + 12, userCtrl);
        st32(cb + 16, cb);
        call(g::Function_vv_swap, {cb, me + CARD_TAP});
        functionDestroy(cb);
    }
    // the edit buttons
    const uint32_t editCenter = findNode(center, S_ButtonJoinEditCenter, 0x16);
    const uint32_t edit = findNode(center, S_ButtonJoinEdit, 0xF);
    const uint32_t leaderChange = findNode(center, S_ButtonLeaderChange, 0x13);
    const bool isLeader = ld8(leader) != 0;
    if (canManage == 1) {
        if (isLeader) {
            setVisible(edit, 1);
            addTouch(edit, VT_JoinEdit, {me});
            titleLabel(edit, S_TxtJoinEdit, 0xC, TXT_JOIN_EDIT);
            setVisible(leaderChange, 1);
            addTouch(leaderChange, VT_LeaderChange, {me});
            titleLabel(leaderChange, S_TxtLeaderChange, 0x10, TXT_LEADER_CHANGE);
            setVisible(editCenter, 0);
        } else {
            setVisible(editCenter, 1);
            addTouch(editCenter, VT_JoinEditCenter, {me});
            titleLabel(editCenter, S_TxtJoinEditCenter, 0x13, TXT_JOIN_EDIT);
            setVisible(edit, 0);
            setVisible(leaderChange, 0);
        }
    } else if (isLeader) {
        setVisible(editCenter, 1);
        addTouch(editCenter, VT_LeaderChangeCenter, {me});
        titleLabel(editCenter, S_TxtJoinEditCenter, 0x13, TXT_LEADER_CHANGE);
        setVisible(edit, 0);
        setVisible(leaderChange, 0);
    } else {
        setVisible(editCenter, 0);
        setVisible(edit, 0);
        setVisible(leaderChange, 0);
    }
    call(g::SafeBuffer_dtor, {style});
    strFree(joinText);
    strFree(styleText);
    release(avatarsCtrl);
    release(userCtrl);
    release(leaderCtrl);
    call(g::Vec2_dtor, {origin});
}
RW_FUNCTION(0x6530AC, "ScenePartyTop::runInJoinLayout", party_top::runInJoinLayout);

void runOutJoinLayout(uint32_t me, uint32_t done);

// Leaving: the header untouchable, then the layout slides out (join or entry), `done` at the end.
void runOutAnimationMain(uint32_t me, uint32_t done) {
    SceneManager::setCanTouchHeader(rw::host_ptr<SceneManager>(call(fn::SceneManager_getInstance, {})), 0);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    if (ld8(me + IN_PARTY)) runOutJoinLayout(me, copy);
    else call(g::ScenePartyTop_runOutEntryLayout, {me, copy});
    functionDestroy(copy);
}
RW_FUNCTION(0x6555C0, "ScenePartyTop::runOutAnimationMain", party_top::runOutAnimationMain);

// The join layout slides out: the right-hand buttons (the first carrying `done`), the card to the left.
void runOutJoinLayout(uint32_t me, uint32_t done) {
    const uint32_t right = findNode(ld32(me + ROOT), S_RightUI, 7);
    const auto out = [&](uint32_t node, bool left, uint32_t callback) {
        const uint32_t action = left ? SceneUtil::createLeftOutAnimation(node, 0.0f, 0.0f, callback)
                                     : SceneUtil::createRightOutAnimation(node, 0.0f, 0.0f, callback);
        call(fn::Node_runAction, {node, action});
    };
    {
        const uint32_t button = findNode(right, S_MemberInvitationButton, 0x16);
        Frame f;
        const uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, done});
        out(button, false, copy);
        functionDestroy(copy);
    }
    for (const auto &b : {std::pair<uint32_t, uint32_t>{S_MemberCheckButton, 0x11}, {S_ManagementButton, 0x10},
                          {S_RankingButton, 0xD}}) {
        const uint32_t button = findNode(right, b.first, b.second);
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        out(button, false, none);
        functionDestroy(none);
    }
    call(fn::APIManager_getInstance, {});
    const uint32_t link = ld32(call(fn::APIManager_getInstance, {}) + API_LINK);
    if (link && (ld8(link + 0x34) & 1)) {
        const uint32_t button = findNode(right, S_InvitationButton, 0x10);
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        out(button, false, none);
        functionDestroy(none);
    }
    const uint32_t center = findNode(ld32(me + ROOT), S_CenterUI, 8);
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    out(center, true, none);
    functionDestroy(none);
}
RW_FUNCTION(0x6556B0, "ScenePartyTop::runOutJoinLayout", party_top::runOutJoinLayout);

// Back (once): the scene runs out, then back to MyPage (the $_ lambda, sub_655DDC).
void backScene(uint32_t me) {
    if (!ld8(me + CAN_BACK)) return;
    st8(me + CAN_BACK, 0);
    Frame f;
    const uint32_t cb = newFunction(f, VT_Back, {});
    call(g::IInOutAnimation_runOutAnimation, {me + 0x288, cb});
    functionDestroy(cb);
}
RW_FUNCTION(0x655CDC, "ScenePartyTop::backScene", party_top::backScene);

void backLambda(uint32_t) { backToMyPage(); }
RW_FUNCTION(0x655DDC, "ScenePartyTop::backScene $_ __func::operator() (sub_655DDC)", party_top::backLambda);

// ------------------------------------------------------------------------------------------------ the request chain
// sub_65A7E8: a lambda {..., function<void(bool)> at +0x18}(int status): on 1, the party member list request with the
// next lambda (heap 0x38, the function at +0x20).
void requestMembers(uint32_t self, uint32_t status) {
    if (status != 1) return;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t tmp = f.alloc(20);
    call(fn::function_vb_copy, {tmp, self + 0x18});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x38});
    if (heap) {
        st32(heap, VT_Chain1);
        call(fn::function_vb_move, {heap + 0x20, tmp});
    }
    st32(fo + 16, heap);
    call(g::APIManager_executePartyMemberList, {api, 1, fo, 1});
    functionDestroy(fo);
    functionDestroy(tmp);
}
RW_FUNCTION(0x65A7E8, "ScenePartyTop::callPartyTopAPI lambda (sub_65A7E8)", party_top::requestMembers);

// A chain lambda {function<void(bool)> at FN} of SIZE bytes: __clone(), destroy_deallocate(), and operator()(bool)
// starting the next request with the next lambda.
template <uint32_t VT, uint32_t SIZE_, uint32_t FN>
struct Chain {
    static uint32_t clone(uint32_t self) {
        const uint32_t p = call(fn::operator_new, {SIZE_});
        if (p) {
            st32(p, VT);
            call(fn::function_vb_copy, {p + FN, self + FN});
        }
        return p;
    }
    static void destroyDeallocate(uint32_t self) {
        functionDestroy(self + FN);
        call(fn::operator_delete, {self});
    }
    template <uint32_t NEXT_VT, uint32_t NEXT_SIZE, uint32_t NEXT_FN, uint32_t API>
    static void next(uint32_t self, uint32_t ok) {
        if (!ld8(ok)) return;
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t tmp = f.alloc(20);
        call(fn::function_vb_copy, {tmp, self + FN});
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t heap = call(fn::operator_new, {NEXT_SIZE});
        if (heap) {
            st32(heap, NEXT_VT);
            call(fn::function_vb_move, {heap + NEXT_FN, tmp});
        }
        st32(fo + 16, heap);
        call(API, {api, fo});
        functionDestroy(fo);
        functionDestroy(tmp);
    }
};
using Chain1 = Chain<VT_Chain1, 0x38, 0x20>;
using Chain2 = Chain<VT_Chain2, 0x30, 0x18>;
using Chain3 = Chain<VT_Chain3, 0x28, 0x10>;

uint32_t chain1Clone(uint32_t self) { return Chain1::clone(self); }
void chain1Free(uint32_t self) { Chain1::destroyDeallocate(self); }
void chain1Call(uint32_t self, uint32_t ok) {
    Chain1::next<VT_Chain2, 0x30, 0x18, g::APIManager_executePartyNoticeUpdate>(self, ok);
}
uint32_t chain2Clone(uint32_t self) { return Chain2::clone(self); }
void chain2Free(uint32_t self) { Chain2::destroyDeallocate(self); }
void chain2Call(uint32_t self, uint32_t ok) {
    Chain2::next<VT_Chain3, 0x28, 0x10, g::APIManager_executePartyLogin>(self, ok);
}
uint32_t chain3Clone(uint32_t self) { return Chain3::clone(self); }
void chain3Free(uint32_t self) { Chain3::destroyDeallocate(self); }
RW_FUNCTION(0x65A988, "ScenePartyTop::callPartyTopAPI chain 1 __func::__clone() (sub_65A988)", party_top::chain1Clone);
RW_FUNCTION(0x65AA2C, "ScenePartyTop::callPartyTopAPI chain 1 __func::destroy_deallocate (sub_65AA2C)", party_top::chain1Free);
RW_FUNCTION(0x65AA58, "ScenePartyTop::callPartyTopAPI chain 1 __func::operator() (sub_65AA58)", party_top::chain1Call);
RW_FUNCTION(0x65AC20, "ScenePartyTop::callPartyTopAPI chain 2 __func::__clone() (sub_65AC20)", party_top::chain2Clone);
RW_FUNCTION(0x65ACC4, "ScenePartyTop::callPartyTopAPI chain 2 __func::destroy_deallocate (sub_65ACC4)", party_top::chain2Free);
RW_FUNCTION(0x65ACF0, "ScenePartyTop::callPartyTopAPI chain 2 __func::operator() (sub_65ACF0)", party_top::chain2Call);
RW_FUNCTION(0x65AEB8, "ScenePartyTop::callPartyTopAPI chain 3 __func::__clone() (sub_65AEB8)", party_top::chain3Clone);
RW_FUNCTION(0x65AF5C, "ScenePartyTop::callPartyTopAPI chain 3 __func::destroy_deallocate (sub_65AF5C)", party_top::chain3Free);


// ------------------------------------------------------------------------------------------------ the entry layout
// No party yet: PartyTop_Initial.json as "_entryLayout" over the hidden RightUI / CenterUI - Create Party, Find
// Party and Manage (requests / invitations), the first-join reward line (misc 117 Jewels; hidden once received).
void runInEntryLayout(uint32_t me) {
    setVisible(findNode(ld32(me + ROOT), S_RightUI, 7), 0);
    setVisible(findNode(ld32(me + ROOT), S_CenterUI, 8), 0);
    const uint32_t layout =
        call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PartyTopInitialJson});
    {
        Frame f;
        const uint32_t name = newStr(f, S_EntryLayout, 0xC);
        vcall(layout, V_SET_NAME, {layout, name});
        strFree(name);
    }
    vcall(ld32(me + ROOT), node::addChild, {ld32(me + ROOT), layout});
    titleLabel(layout, S_TxtWording1, 0xC, TXT_ENTRY_CREATE);
    label(layout, S_TxtPREdit, 0xA, TXT_CREATE_PARTY);
    addTouch(findNode(layout, S_ButtonPREdit, 0xD), VT_CreateParty, {me});
    titleLabel(layout, S_TxtWording2, 0xC, TXT_ENTRY_FIND);
    label(layout, S_TxtInitialRequest, 0x12, TXT_FIND_PARTY);
    addTouch(findNode(layout, S_ButtonInitialRequest, 0x15), VT_FindParty, {me});
    titleLabel(layout, S_TxtWording3, 0xC, TXT_ENTRY_MANAGE);
    label(layout, S_TxtInitialManagement, 0x15, TXT_MANAGE);
    addTouch(findNode(layout, S_ButtonInitialManagement, 0x18), VT_Manage, {me});
    const uint32_t reward = findNode(layout, S_TxtWording4, 0xC);
    SceneUtil::setShadow(reward);
    {
        Frame f;
        const uint32_t fmt = textData(f, TXT_FIRST_JOIN_REWARD);
        const uint32_t row = f.alloc(8);
        call(g::Misc_create, {row, MISC_FIRST_JOIN_JEWELS});
        const uint32_t sb = ld32(row);
        const uint32_t jewels = f.alloc(4);
        call(g::SafeBuffer_read, {sb, jewels, ld32(sb + 0xC) + 4, 4, 4});
        setTextC(reward, stringBuffer(f, {textDataStr(fmt), ld32(jewels)}));
        sharedRelease(row);
        sharedRelease(fmt);
    }
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t userCtrl = addRef(ld32(api + API_USER + 4));
    const uint32_t user = ld32(api + API_USER);
    {
        Frame f;
        const uint32_t received = f.alloc(4);
        call(g::SafeBuffer_read, {user, received + 3, ld32(user + 0xC) + 0x13, 1, 0x13});
        setVisible(reward, ld8(received + 3) == 0 ? 1 : 0);
    }
    runAnimation(layout, leftIn);
    release(userCtrl);
}
RW_FUNCTION(0x654C34, "ScenePartyTop::runInEntryLayout", party_top::runInEntryLayout);

// The entry layout slides out to the left, then `done`.
void runOutEntryLayout(uint32_t me, uint32_t done) {
    const uint32_t layout = findNode(ld32(me + ROOT), S_EntryLayout, 0xC);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(fn::function_vv_copy, {copy, done});
    call(fn::Node_runAction, {layout, SceneUtil::createLeftOutAnimation(layout, 0.0f, 0.0f, copy)});
    functionDestroy(copy);
}
RW_FUNCTION(0x655B34, "ScenePartyTop::runOutEntryLayout", party_top::runOutEntryLayout);

// runInEntryLayout $_23 {scene}: Manage - the scene runs out, then the request list
void manageTouch(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return;
    const uint32_t me = ld32(functor + 4);
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t then = newFunction(f, VT_ManageOut, {});
    call(g::IInOutAnimation_runOutAnimation, {me + 0x288, then});
    functionDestroy(then);
}
RW_FUNCTION(0x65722C, "ScenePartyTop::runInEntryLayout::$_23 operator() (Manage)", party_top::manageTouch);
// its lambda(): /party/application/list
void manageOut(uint32_t) {
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t cb = newFunction(f, VT_ManageApplications, {});
    call(g::APIManager_executePartyApplicationList, {api, cb});
    functionDestroy(cb);
}
RW_FUNCTION(0x65735C, "ScenePartyTop::runInEntryLayout::$_23::lambda() operator()", party_top::manageOut);
// that lambda's lambda(bool ok): the offer list, then ScenePartyManagement; or the error dialog
void manageApplications(uint32_t, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    Frame f;
    if (ok) {
        const uint32_t sm = sceneManager();
        const uint32_t none = newEmptyFunction(f);
        st8(sm + 0x4CC, 1);
        call(g::Function_vb_assign, {sm + 0x548, none});
        functionDestroy(none);
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t none2 = newEmptyFunction(f);
        call(g::APIManager_executePartyOfferList, {api, none2});
        functionDestroy(none2);
        const uint32_t sm2 = sceneManager();
        const uint32_t layer = newFunction(f, ld32(GOT_FUNC_LAYER_FACTORY_VTABLE) + 8, {ld32(GOT_CREATE_MANAGEMENT)});
        rw::call(g::SceneManager_changeSceneLayer, {sm2, layer, 1, 1, 1});
        functionDestroy(layer);
        return;
    }
    if (!ld8(call(fn::APIManager_getInstance, {}) + 0x14)) return;
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t cb = newFunction(f, VT_ManageError, {});
    call(g::APIManager_openErrorDialog, {api, cb, 1});
    functionDestroy(cb);
}
RW_FUNCTION(0x657478, "ScenePartyTop::runInEntryLayout::$_23::lambda()::lambda(bool) operator()",
            party_top::manageApplications);
}  // namespace party_top
