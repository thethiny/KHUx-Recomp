// SceneMyPage lambdas (std::function __func::operator(): r0 is the __func, captures from +4; heap ones that hold
// a std::function keep it at +0x18).
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t M_addChangeMotionEvent = 0x63FA8D;
constexpr uint32_t NODE_removeChild = 0x12C;       // removeChild(child, cleanup)
constexpr uint32_t MOTION_IDLE = 0x1B;

// node hidden and put back at x
void hideAt(uint32_t node, uint32_t x) {
    vcall(node, node::setVisible, {node, 0});
    vcall(node, node::setPositionX, {node, x});
}
// the member panel at self+off hidden and put back at x
void hidePanelAt(uint32_t self, uint32_t off, uint32_t x) {
    uint32_t n = ld32(self + off);
    vcall(n, node::setVisible, {n, 0});
    n = ld32(self + off);
    vcall(n, node::setPositionX, {n, x});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ changeRankingHeader
// $_57: the leader stands idle facing right
void SceneMyPage_header57(uint32_t fo) {
    uint32_t self = ld32(fo + 4);
    call(M_addChangeMotionEvent, {self, ld32(self + 0x2BC), MOTION_IDLE});
    uint32_t avatar = ld32(ld32(self + 0x2BC) + 0x288);
    vcall(avatar, node::setScaleX, {avatar, 0x3F800000u});
}
// $_59 / $_61: the 0x2B4 panel; $_62 / $_64: the 0x2AC panel; $_65: the 0x2B0 panel (hidden, back at x)
void SceneMyPage_header59(uint32_t fo) { hidePanelAt(ld32(fo + 4), 0x2B4, ld32(fo + 8)); }
void SceneMyPage_header61(uint32_t fo) { hidePanelAt(ld32(fo + 4), 0x2B4, ld32(fo + 8)); }
void SceneMyPage_header62(uint32_t fo) { hidePanelAt(ld32(fo + 4), 0x2AC, ld32(fo + 8)); }
void SceneMyPage_header64(uint32_t fo) { hidePanelAt(ld32(fo + 4), 0x2AC, ld32(fo + 8)); }
void SceneMyPage_header65(uint32_t fo) { hidePanelAt(ld32(fo + 4), 0x2B0, ld32(fo + 8)); }
// $_66 / $_67: {node, x}
void SceneMyPage_header66(uint32_t fo) {
    vcall(ld32(fo + 4), node::setVisible, {ld32(fo + 4), 0});
    vcall(ld32(fo + 4), node::setPositionX, {ld32(fo + 4), ld32(fo + 8)});
}
void SceneMyPage_header67(uint32_t fo) {
    vcall(ld32(fo + 4), node::setVisible, {ld32(fo + 4), 0});
    vcall(ld32(fo + 4), node::setPositionX, {ld32(fo + 4), ld32(fo + 8)});
}
// $_60: {self, x, self, done}: the 0x2B8 panel hidden at x, header unlocked, done()
void SceneMyPage_header60(uint32_t fo) {
    uint32_t self = ld32(fo + 8);
    hidePanelAt(self, 0x2B8, ld32(fo + 0xC));
    st8(ld32(fo + 0x10) + 0x2F3, 0);
    call(wl::FunctionVV_call, {fo + 0x18});
}
// $_63 / $_68: {node, x, self, done}
void SceneMyPage_header63(uint32_t fo) {
    vcall(ld32(fo + 8), node::setVisible, {ld32(fo + 8), 0});
    vcall(ld32(fo + 8), node::setPositionX, {ld32(fo + 8), ld32(fo + 0xC)});
    st8(ld32(fo + 0x10) + 0x2F3, 0);
    call(wl::FunctionVV_call, {fo + 0x18});
}
void SceneMyPage_header68(uint32_t fo) {
    vcall(ld32(fo + 8), node::setVisible, {ld32(fo + 8), 0});
    vcall(ld32(fo + 8), node::setPositionX, {ld32(fo + 8), ld32(fo + 0xC)});
    st8(ld32(fo + 0x10) + 0x2F3, 0);
    call(wl::FunctionVV_call, {fo + 0x18});
}
// $_69: the NPC leaves the field layer
void SceneMyPage_header69(uint32_t fo) {
    uint32_t self = ld32(fo + 4);
    uint32_t layer = ld32(self + 0x2A4);
    vcall(layer, NODE_removeChild, {layer, ld32(self + 0x2FC), 1});
    st32(self + 0x2FC, 0);
}

RW_FUNCTION(0x647C1E, "SceneMyPage::changeRankingHeader::$_57::operator()", SceneMyPage_header57);
RW_FUNCTION(0x647ABE, "SceneMyPage::changeRankingHeader::$_59::operator()", SceneMyPage_header59);
RW_FUNCTION(0x6479E0, "SceneMyPage::changeRankingHeader::$_60::operator()", SceneMyPage_header60);
RW_FUNCTION(0x647816, "SceneMyPage::changeRankingHeader::$_61::operator()", SceneMyPage_header61);
RW_FUNCTION(0x647756, "SceneMyPage::changeRankingHeader::$_62::operator()", SceneMyPage_header62);
RW_FUNCTION(0x647680, "SceneMyPage::changeRankingHeader::$_63::operator()", SceneMyPage_header63);
RW_FUNCTION(0x6474B6, "SceneMyPage::changeRankingHeader::$_64::operator()", SceneMyPage_header64);
RW_FUNCTION(0x6473F6, "SceneMyPage::changeRankingHeader::$_65::operator()", SceneMyPage_header65);
RW_FUNCTION(0x64733E, "SceneMyPage::changeRankingHeader::$_66::operator()", SceneMyPage_header66);
RW_FUNCTION(0x647286, "SceneMyPage::changeRankingHeader::$_67::operator()", SceneMyPage_header67);
RW_FUNCTION(0x6471B0, "SceneMyPage::changeRankingHeader::$_68::operator()", SceneMyPage_header68);
RW_FUNCTION(0x646FEA, "SceneMyPage::changeRankingHeader::$_69::operator()", SceneMyPage_header69);

// ------------------------------------------------------------------------------------------------ changeParty / right buttons
namespace {
constexpr uint32_t M_changeRankingHeader = 0x643C9D, M_changeParty = 0x646E45;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;
constexpr uint32_t Facebook_getInstance = 0x4E5585, Facebook_login = 0x4E5747, Facebook_getCurrentAccessToken = 0x4E57F1;
constexpr uint32_t APIManager_executeUserLinkUpdate = 0x4D41CD, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775, SceneUtil_openOkDialog8Line = 0x45CA3D;
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;
constexpr uint32_t VT_party55_inner = 0xF60D98;
constexpr uint32_t VT_fb_login = 0xF60E1C, VT_fb_linked = 0xF60E48, VT_fb_done = 0xF60E74, VT_fb_out = 0xF60EA0;
constexpr uint32_t VT_fb_error = 0xF60ECC, VT_fb_failed = 0xF60F24;
constexpr int32_t TXT_FB_LINKED = 104900005, TXT_FB_FAILED = 104900015;
constexpr uint32_t TOUCH_ENDED = 2;
constexpr uint32_t BUTTON_setEnabled = 0x29C;
constexpr uint32_t TAB_setState = 0xC;               // slot 3 of the ranking tab buttons

// SceneUtil::openOkDialog{4,8}Line(TextData text `id`, inline lambda {vt, cap}, 0)
void okDialog(uint32_t open, int32_t id, uint32_t vt, uint32_t cap) {
    Frame f;
    uint32_t sp = textData(f, id);
    uint32_t s = newStr(f, textDataStr(sp));
    uint32_t fo = newFunction(f, vt, {cap});
    call(open, {s, fo, 0});
    functionDestroy(fo);
    strFree(s);
    sharedRelease(sp);
}
// the page slides out (IInOutAnimation at +0x288), then F60EA0
void runOutPage(uint32_t self) {
    Frame f;
    uint32_t fo = newFunction(f, VT_fb_out, {});
    call(IInOutAnimation_runOutAnimation, {self + 0x288, fo});
    functionDestroy(fo);
}
}  // namespace

// changeParty $_55 {self, kind}: the ranking kind set, header in (then the inner lambda {self})
void SceneMyPage_party55(uint32_t fo) {
    uint32_t self = ld32(fo + 4);
    st32(self + 0x28C, ld32(fo + 8));
    Frame f;
    uint32_t inner = newFunction(f, VT_party55_inner, {self});
    call(M_changeRankingHeader, {self, 1, inner});
    functionDestroy(inner);
}
// the inner lambda: __clone()
uint32_t SceneMyPage_party55InnerClone(uint32_t fo) {
    uint32_t p = call(fn::operator_new, {8});
    if (p) {
        st32(p, VT_party55_inner);
        st32(p + 4, ld32(fo + 4));
    }
    return p;
}

// initRightButtons $_53 {self, tabA, tabB}: switch the ranking between party (kind 1) and solo (kind 0)
void SceneMyPage_right53(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    uint32_t self = ld32(fo + 4);
    if (!ld8(self + 0x410) || ld8(self + 0x2F2) || ld8(self + 0x2F3) || !ld8(self + 0x3A9)) return;
    call(AudioPlaySystemSEDecide, {});
    uint32_t kind = ld32(self + 0x28C);
    if (kind == 1) {
        vcall(ld32(fo + 8), TAB_setState, {ld32(fo + 8), 2});
        vcall(ld32(fo + 0xC), TAB_setState, {ld32(fo + 0xC), 1});
        call(M_changeParty, {self, 0});
    } else if (kind == 0) {
        vcall(ld32(fo + 8), TAB_setState, {ld32(fo + 8), 1});
        vcall(ld32(fo + 0xC), TAB_setState, {ld32(fo + 0xC), 2});
        call(M_changeParty, {self, 1});
    }
}

// initRightButtons $_52 {self, button}: Facebook login (button disabled meanwhile)
void SceneMyPage_right52(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    uint32_t self = ld32(fo + 4);
    if (!ld8(self + 0x410) || ld8(self + 0x2F2) || ld8(self + 0x2F3)) return;
    vcall(ld32(fo + 8), BUTTON_setEnabled, {ld32(fo + 8), 0});
    call(AudioPlaySystemSEDecide, {});
    uint32_t button = ld32(fo + 8);
    uint32_t mgr = call(Facebook_getInstance, {});
    Frame f;
    uint32_t cb = newFunction(f, VT_fb_login, {self, button});
    call(Facebook_login, {mgr, 0, cb});
    functionDestroy(cb);
}
uint32_t SceneMyPage_right52LoginClone(uint32_t fo) {
    uint32_t p = call(fn::operator_new, {0xC});
    if (p) {
        st32(p, VT_fb_login);
        st32(p + 8, ld32(fo + 8));
        st32(p + 4, ld32(fo + 4));
    }
    return p;
}
void SceneMyPage_right52LoginCloneTo(uint32_t fo, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_fb_login);
    st32(dst + 8, ld32(fo + 8));
    st32(dst + 4, ld32(fo + 4));
}

// the login result (ok, error code, message moved in): link the account (token) or re-enable / tell the failure
void SceneMyPage_right52Login(uint32_t fo, uint32_t okRef, uint32_t codeRef, uint32_t msg) {
    const uint32_t msgData = ld32(msg + 8);
    const uint32_t msgFlag = ld8(msg);
    const uint32_t code = ld32(codeRef);
    const uint32_t ok = ld8(okRef);
    st32(msg, 0);
    st32(msg + 4, 0);
    st32(msg + 8, 0);
    const uint32_t self = ld32(fo + 4);
    if (ok) {
        uint32_t button = ld32(fo + 8);
        call(SceneManager_enableLoadingIndicator, {call(SceneManager_getInstance, {}), 1});
        uint32_t api = call(APIManager_getInstance, {});
        uint32_t mgr = call(Facebook_getInstance, {});
        Frame f;
        uint32_t token = f.alloc(12);
        call(Facebook_getCurrentAccessToken, {token, mgr});
        uint32_t cstr = strIsLong(token) ? ld32(token + 8) : token + 1;
        uint32_t cb = newFunction(f, VT_fb_linked, {button, self});
        call(APIManager_executeUserLinkUpdate, {api, cstr, 1, cb});
        functionDestroy(cb);
        strFree(token);
    } else if (code == 3) {
        vcall(ld32(fo + 8), BUTTON_setEnabled, {ld32(fo + 8), 1});
    } else {
        okDialog(SceneUtil_openOkDialog4Line, TXT_FB_FAILED, VT_fb_failed, self);
    }
    if (msgFlag & 1) call(fn::operator_delete, {msgData});
}
uint32_t SceneMyPage_right52LinkedClone(uint32_t fo) {
    uint32_t p = call(fn::operator_new, {0xC});
    if (p) {
        st32(p, VT_fb_linked);
        st32(p + 8, ld32(fo + 8));
        st32(p + 4, ld32(fo + 4));
    }
    return p;
}
void SceneMyPage_right52LinkedCloneTo(uint32_t fo, uint32_t dst) {
    if (!dst) return;
    st32(dst, VT_fb_linked);
    st32(dst + 8, ld32(fo + 8));
    st32(dst + 4, ld32(fo + 4));
}
// the link result {button, self}: linked -> dialog then the page slides out; failed -> the API error dialog
void SceneMyPage_right52Linked(uint32_t fo, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(SceneManager_enableLoadingIndicator, {call(SceneManager_getInstance, {}), 0});
    if (ok) {
        okDialog(SceneUtil_openOkDialog8Line, TXT_FB_LINKED, VT_fb_done, ld32(fo + 8));
    } else if (ld8(call(APIManager_getInstance, {}) + 0x14)) {
        uint32_t api = call(APIManager_getInstance, {});
        Frame f;
        uint32_t cb = newFunction(f, VT_fb_error, {ld32(fo + 4)});
        call(APIManager_openErrorDialog, {api, cb, 1});
        functionDestroy(cb);
    }
}
uint32_t SceneMyPage_right52DoneClone(uint32_t fo) {
    uint32_t p = call(fn::operator_new, {8});
    if (p) {
        st32(p, VT_fb_done);
        st32(p + 4, ld32(fo + 4));
    }
    return p;
}
// {self}: the page slides out (after the linked / failed dialogs)
void SceneMyPage_right52Done(uint32_t fo) { runOutPage(ld32(fo + 4)); }
void SceneMyPage_right52Failed(uint32_t fo) { runOutPage(ld32(fo + 4)); }

RW_FUNCTION(0x647E94, "SceneMyPage::changeParty::$_55::operator()", SceneMyPage_party55);
RW_FUNCTION(0x647F70, "SceneMyPage::changeParty::$_55 inner lambda __clone()", SceneMyPage_party55InnerClone);
RW_FUNCTION(0x648078, "SceneMyPage::initRightButtons::$_53::operator()", SceneMyPage_right53);
RW_FUNCTION(0x6481A4, "SceneMyPage::initRightButtons::$_52::operator()", SceneMyPage_right52);
RW_FUNCTION(0x6482D0, "SceneMyPage::initRightButtons::$_52 login lambda __clone()", SceneMyPage_right52LoginClone);
RW_FUNCTION(0x648300, "SceneMyPage::initRightButtons::$_52 login lambda __clone(dst)", SceneMyPage_right52LoginCloneTo);
RW_FUNCTION(0x648330, "SceneMyPage::initRightButtons::$_52 login lambda operator()", SceneMyPage_right52Login);
RW_FUNCTION(0x648578, "SceneMyPage::initRightButtons::$_52 link lambda __clone()", SceneMyPage_right52LinkedClone);
RW_FUNCTION(0x6485A8, "SceneMyPage::initRightButtons::$_52 link lambda __clone(dst)", SceneMyPage_right52LinkedCloneTo);
RW_FUNCTION(0x6485D8, "SceneMyPage::initRightButtons::$_52 link lambda operator()", SceneMyPage_right52Linked);
RW_FUNCTION(0x648794, "SceneMyPage::initRightButtons::$_52 linked-dialog lambda __clone()", SceneMyPage_right52DoneClone);
RW_FUNCTION(0x6487EC, "SceneMyPage::initRightButtons::$_52 linked-dialog lambda operator()", SceneMyPage_right52Done);
RW_FUNCTION(0x648CE0, "SceneMyPage::initRightButtons::$_52 failed-dialog lambda operator()", SceneMyPage_right52Failed);

// ------------------------------------------------------------------------------------------------ menu buttons
namespace {
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9, Facebook_inviteFriends = 0x4E5779;
constexpr uint32_t FunctionVB_assign = 0x490109;          // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;
constexpr uint32_t SceneShopTop_callShopAPI = 0x68BB9D, SceneAdventureSelect_callAPI = 0x71D261;
constexpr uint32_t APIManager_executeUserSphereCheck = 0x4CCBC1;
constexpr uint32_t ScenePresentBox_openPresentBoxDialog = 0x6CF9E1;
constexpr uint32_t AppState_getInstance = 0x4E66DD, AppState_getTutorialCloseFunc = 0x4EBA3D;
constexpr uint32_t AppState_execTutorialCloseFunc = 0x4EBA5D;
// GOT slots: the __func<Layer*(*)()> vtable and the createSceneLayer<T> instances
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_SHOP = 0xFD66F8, GOT_CREATE_ADVENTURE = 0xFD669C;
constexpr uint32_t GOT_CREATE_SPHERE_SELECT = 0xFD68A0, GOT_CREATE_DECK_EDIT = 0xFD66B0;
constexpr uint32_t VT_invite_51 = 0xF60F7C, VT_shop_50 = 0xF60FD4, VT_present_49 = 0xF6102C;
constexpr uint32_t VT_adv_48 = 0xF61084, VT_sphere_47 = 0xF610DC, VT_sphere_47b = 0xF61108, VT_deck_46 = 0xF61160;
constexpr int32_t TXT_INVITE_FAILED = 104900018;

// a header / menu button touch that may act: ended, page touchable, not locked, header not moving
bool menuTouch(uint32_t self, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return false;
    return ld8(self + 0x410) && !ld8(self + 0x2F2) && !ld8(self + 0x2F3);
}
// the page slides out, then the inline lambda {vt, caps}
void runOutThen(uint32_t self, uint32_t vt, std::initializer_list<uint32_t> caps) {
    Frame f;
    uint32_t fo = newFunction(f, vt, caps);
    call(IInOutAnimation_runOutAnimation, {self + 0x288, fo});
    functionDestroy(fo);
}
// SceneManager: the back-key flag set and its std::function<void(bool)> at +0x548 emptied
void clearBackCallback() {
    uint32_t sm = call(SceneManager_getInstance, {});
    Frame f;
    uint32_t empty = newEmptyFunction(f);
    st8(sm + 0x4CC, 1);
    call(FunctionVB_assign, {sm + 0x548, empty});
    functionDestroy(empty);
}
// call(fn) with an empty std::function<void(bool)> (args before it in r0..)
void callWithEmpty(uint32_t fnAddr, std::initializer_list<uint32_t> before) {
    Frame f;
    uint32_t empty = newEmptyFunction(f);
    uint32_t a[3] = {0, 0, 0};
    uint32_t n = 0;
    for (uint32_t v : before) a[n++] = v;
    if (n == 0) call(fnAddr, {empty});
    else if (n == 1) call(fnAddr, {a[0], empty});
    else call(fnAddr, {a[0], a[1], empty});
    functionDestroy(empty);
}
// SceneManager::changeSceneLayer(createSceneLayer<T>, true, true, fade 1)
void toSceneLayer(uint32_t gotCreate) {
    uint32_t sm = call(SceneManager_getInstance, {});
    Frame f;
    uint32_t fo = newFunction(f, ld32(GOT_FUNC_LAYER_VT) + 8, {ld32(gotCreate)});
    call(SceneManager_changeSceneLayer, {sm, fo, 1, 1, 1});
    functionDestroy(fo);
}
}  // namespace

// initRightButtons $_51 {self}: invite Facebook friends (when logged in)
void SceneMyPage_right51(uint32_t fo, uint32_t, uint32_t typeRef) {
    uint32_t self = ld32(fo + 4);
    if (!menuTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    if (call(SceneUtil_isLoggedInFacebook, {}) != 1) return;
    uint32_t mgr = call(Facebook_getInstance, {});
    Frame f;
    uint32_t message = f.alloc(12), title = f.alloc(12);
    st8(message, 0);
    st8(message + 1, 0);
    st8(title, 0);
    st8(title + 1, 0);
    uint32_t cb = newFunction(f, VT_invite_51, {});
    call(Facebook_inviteFriends, {mgr, message, title, cb});
    functionDestroy(cb);
    strFree(title);
    strFree(message);
}
// its result (ok, error code, message moved in): a failure other than 2 (cancelled) shows a dialog
void SceneMyPage_right51Result(uint32_t, uint32_t okRef, uint32_t codeRef, uint32_t msg) {
    const uint32_t msgData = ld32(msg + 8);
    const uint32_t msgFlag = ld8(msg);
    const uint32_t code = ld32(codeRef);
    const uint32_t ok = ld8(okRef);
    st32(msg, 0);
    st32(msg + 4, 0);
    st32(msg + 8, 0);
    if (!ok && code != 2) {
        Frame f;
        uint32_t sp = textData(f, TXT_INVITE_FAILED);
        uint32_t s = newStr(f, textDataStr(sp));
        uint32_t empty = newEmptyFunction(f);
        call(SceneUtil_openOkDialog4Line, {s, empty, 0});
        functionDestroy(empty);
        strFree(s);
        sharedRelease(sp);
    }
    if (msgFlag & 1) call(fn::operator_delete, {msgData});
}

// initRightButtons $_50 {self}: to the shop (after the page slides out)
void SceneMyPage_right50(uint32_t fo, uint32_t, uint32_t typeRef) {
    uint32_t self = ld32(fo + 4);
    if (!menuTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    runOutThen(self, VT_shop_50, {});
}
void SceneMyPage_right50Out(uint32_t) {
    clearBackCallback();
    callWithEmpty(SceneShopTop_callShopAPI, {});
    toSceneLayer(GOT_CREATE_SHOP);
}

// initRightButtons $_49 {self}: the present box dialog
void SceneMyPage_right49(uint32_t fo, uint32_t, uint32_t typeRef) {
    uint32_t self = ld32(fo + 4);
    if (!menuTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    Frame f;
    uint32_t cb = newFunction(f, VT_present_49, {self});
    call(ScenePresentBox_openPresentBoxDialog, {cb});
    functionDestroy(cb);
}

// initLeftButtons $_48 {self}: quests (closes the tutorial step 100 when pending), page out, adventure select
void SceneMyPage_left48(uint32_t fo, uint32_t, uint32_t typeRef) {
    uint32_t self = ld32(fo + 4);
    if (!menuTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    {
        Frame f;
        uint32_t closeFn = f.alloc(20);
        call(AppState_getTutorialCloseFunc, {closeFn, call(AppState_getInstance, {}), 100});
        if (ld32(closeFn + 16)) {
            functionDestroy(closeFn);
            call(AppState_execTutorialCloseFunc, {call(AppState_getInstance, {}), 100});
        }
    }
    clearBackCallback();
    callWithEmpty(SceneAdventureSelect_callAPI, {});
    runOutThen(self, VT_adv_48, {});
}
void SceneMyPage_left48Out(uint32_t) {
    st32(call(SceneManager_getInstance, {}) + 0x28C, 9);
    toSceneLayer(GOT_CREATE_ADVENTURE);
    st8(call(SceneManager_getInstance, {}) + 0x30A, 1);
}

// initLeftButtons $_47 {self}: the sphere boards
void SceneMyPage_left47(uint32_t fo, uint32_t, uint32_t typeRef) {
    uint32_t self = ld32(fo + 4);
    if (!menuTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    runOutThen(self, VT_sphere_47, {self});
}
void SceneMyPage_left47Out(uint32_t fo) {
    uint32_t self = ld32(fo + 4);
    clearBackCallback();
    callWithEmpty(APIManager_executeUserSphereCheck, {call(APIManager_getInstance, {}), 3});
    runOutThen(self, VT_sphere_47b, {});
}
void SceneMyPage_left47Scene(uint32_t) {
    st32(call(SceneManager_getInstance, {}) + 0x28C, 1);
    toSceneLayer(GOT_CREATE_SPHERE_SELECT);
}

// initLeftButtons $_46 {self}: the deck edit
void SceneMyPage_left46(uint32_t fo, uint32_t, uint32_t typeRef) {
    uint32_t self = ld32(fo + 4);
    if (!menuTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    runOutThen(self, VT_deck_46, {});
}
void SceneMyPage_left46Out(uint32_t) {
    st8(call(SceneManager_getInstance, {}) + 0x2E0, 0);
    st8(call(SceneManager_getInstance, {}) + 0x2E1, 0);
    toSceneLayer(GOT_CREATE_DECK_EDIT);
}

RW_FUNCTION(0x648E0C, "SceneMyPage::initRightButtons::$_51::operator()", SceneMyPage_right51);
RW_FUNCTION(0x648FAC, "SceneMyPage::initRightButtons::$_51 invite lambda operator()", SceneMyPage_right51Result);
RW_FUNCTION(0x649174, "SceneMyPage::initRightButtons::$_50::operator()", SceneMyPage_right50);
RW_FUNCTION(0x6492C8, "SceneMyPage::initRightButtons::$_50 page-out lambda operator()", SceneMyPage_right50Out);
RW_FUNCTION(0x64947C, "SceneMyPage::initRightButtons::$_49::operator()", SceneMyPage_right49);
RW_FUNCTION(0x649678, "SceneMyPage::initLeftButtons::$_48::operator()", SceneMyPage_left48);
RW_FUNCTION(0x64986C, "SceneMyPage::initLeftButtons::$_48 page-out lambda operator()", SceneMyPage_left48Out);
RW_FUNCTION(0x6499C8, "SceneMyPage::initLeftButtons::$_47::operator()", SceneMyPage_left47);
RW_FUNCTION(0x649B28, "SceneMyPage::initLeftButtons::$_47 page-out lambda operator()", SceneMyPage_left47Out);
RW_FUNCTION(0x649CC0, "SceneMyPage::initLeftButtons::$_47 scene lambda operator()", SceneMyPage_left47Scene);
RW_FUNCTION(0x649E14, "SceneMyPage::initLeftButtons::$_46::operator()", SceneMyPage_left46);
RW_FUNCTION(0x649F68, "SceneMyPage::initLeftButtons::$_46 page-out lambda operator()", SceneMyPage_left46Out);

// ------------------------------------------------------------------------------------------------ banners
namespace {
constexpr uint32_t BaseSlideView_changeNextPage = 0x526245, BaseSlideView_changePreviousPage = 0x5263ED;
constexpr uint32_t SE_PAGE = 0x1E56B9EA;
constexpr uint32_t Audio_getInstance = 0x5BECFD, Audio_getLayer = 0x5BF33D, AudioLayer_playSE = 0x5BF785;
constexpr uint32_t Sprite_create = 0x8A637D;            // (const std::string&)
constexpr uint32_t Image_ctor = 0x8D967D, Image_initWithImageData = 0x8D9815;
constexpr uint32_t Texture2D_ctor = 0x9149FD, Texture2D_initWithImage = 0x91501D, Texture2D_getContentSize = 0x914ACB;
constexpr uint32_t ImageMap_at = 0x6469B1;              // std::map<int, Image*>::operator[](const int&)
constexpr uint32_t NODE_getContentSize = 0xAC, NODE_setContentSize = 0xA8, NODE_addChildNamed = 0xFC;
constexpr uint32_t SPRITE_setTexture = 0x27C;
constexpr uint32_t S_EV_BANNER = 0xDFFE3C, S_BANNER_IMAGE = 0xE03AC6, S_LINK_BUTTON = 0xE03AD2;

void playSE(uint32_t id) {
    Frame f;
    uint32_t layer = f.alloc(8);
    call(Audio_getLayer, {layer, call(Audio_getInstance, {}), 0});
    call(AudioLayer_playSE, {ld32(layer), id, 0, 0});
    sharedRelease(layer);
}
}  // namespace

// initBannerArea $_41 / $_40 {self}: arrow touched (began): next / previous banner page, auto-scroll timer reset
void SceneMyPage_banner41(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 0) return;
    uint32_t self = ld32(fo + 4);
    playSE(SE_PAGE);
    st32(self + 0x3FC, 0);
    call(BaseSlideView_changeNextPage, {ld32(self + 0x400)});
}
void SceneMyPage_banner40(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 0) return;
    uint32_t self = ld32(fo + 4);
    playSE(SE_PAGE);
    st32(self + 0x3FC, 0);
    call(BaseSlideView_changePreviousPage, {ld32(self + 0x400)});
}

// initBannerArea $_37 {view}: one banner page: a widget of the view size with the placeholder banner sprite
// ("bannerImage", at the page's top-right corner) and an invisible link button over it ("linkButton")
uint32_t SceneMyPage_banner37(uint32_t fo) {
    const uint32_t widget = call(Widget_create, {});
    {
        uint32_t view = ld32(fo + 4);
        uint32_t size = vcall(view, NODE_getContentSize, {view});
        vcall(widget, NODE_setContentSize, {widget, size});
    }
    uint32_t sprite;
    {
        Frame f;
        uint32_t s = newStr(f, S_EV_BANNER, 0x20);
        sprite = call(Sprite_create, {s});
        strFree(s);
    }
    {
        uint32_t view = ld32(fo + 4);
        uint32_t w = ld32(vcall(view, NODE_getContentSize, {view}));
        view = ld32(fo + 4);
        uint32_t h = ld32(vcall(view, NODE_getContentSize, {view}) + 4);
        Frame f;
        uint32_t pos = f.alloc(8);
        call(wl::Vec2_ctorXY, {pos, w, h});
        vcall(sprite, 0x4C, {sprite, pos});
        call(wl::Vec2_dtor, {pos});
    }
    {
        Frame f;
        uint32_t name = newStr(f, S_BANNER_IMAGE, 0xB);
        vcall(widget, NODE_addChildNamed, {widget, sprite, 0, name});
        strFree(name);
    }
    uint32_t button;
    {
        Frame f;
        uint32_t empty = f.alloc(12);
        st8(empty, 0);
        st8(empty + 1, 0);
        button = call(ImageButton_create, {empty, 0});
        strFree(empty);
    }
    call(ImageButton_setTargetView, {button, sprite, 0x3F800000u});
    {
        Frame f;
        uint32_t name = newStr(f, S_LINK_BUTTON, 0xA);
        vcall(sprite, NODE_addChildNamed, {sprite, button, 0, name});
        strFree(name);
    }
    return widget;
}

// loadShopBanner $_36 {self, key, sprite}: the downloaded banner (data, size at +4/+8) becomes an Image kept in
// the image map and the sprite's texture, scaled to the view height. Returns false when nothing was decoded.
uint32_t SceneMyPage_banner36(uint32_t fo, uint32_t dataRef) {
    if (!ld32(dataRef)) return 0;
    const uint32_t self = ld32(fo + 4);
    uint32_t image = call(fn::operator_new, {0xC4});
    call(Image_ctor, {image});
    uint32_t data = ld32(dataRef);
    if (call(Image_initWithImageData, {image, ld32(data + 4), ld32(data + 8)}) != 1) {
        call(smain::lib::Ref_release, {image});
        return 0;
    }
    call(smain::lib::Ref_retain, {image});
    st32(call(ImageMap_at, {self + OFF_IMAGES, fo + 8}), image);
    uint32_t tex = call(fn::operator_new, {0x40});
    call(Texture2D_ctor, {tex});
    uint32_t result = 0;
    if (call(Texture2D_initWithImage, {tex, image}) == 1) {
        uint32_t sprite = ld32(fo + 0xC);
        result = 1;
        if (sprite) {
            vcall(sprite, SPRITE_setTexture, {sprite, tex});
            uint32_t view = ld32(self + 0x400);
            float viewH = ldf(vcall(view, NODE_getContentSize, {view}) + 4);
            Frame f;
            uint32_t size = f.alloc(8);
            call(Texture2D_getContentSize, {size, tex});
            float scale = viewH / ldf(size + 4);
            sprite = ld32(fo + 0xC);
            vcall(sprite, 0x40, {sprite, fb(scale)});
            sprite = ld32(fo + 0xC);
            vcall(sprite, SPRITE_setTexture, {sprite, tex});
        }
    }
    call(smain::lib::Ref_release, {tex});
    call(smain::lib::Ref_release, {image});
    return result;
}

namespace {
constexpr uint32_t WidgetFactory_call = 0x54A065;        // std::function<ui::Widget*(int)>::operator()(int)
constexpr uint32_t EvStage_create = 0x782D2D, DrawMedalType_create = 0x77DCE1;
constexpr uint32_t String_assignN = 0x48D811;            // std::string::assign(const char*, size_t)
constexpr uint32_t String_compareC = 0x47CBD5;           // std::string::compare(const char*)
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5;
constexpr uint32_t M_loadShopBanner = 0x6467FD, M_updateExpirationDate = 0x646A31;
constexpr uint32_t VT_banner_touch = 0xF61294, VT_banner_shop = 0xF612C0, VT_banner_event = 0xF612EC;
constexpr uint32_t VT_banner_eventScene = 0xF61318;
constexpr uint32_t S_DOT_NAVI = 0xE0391E, S_DOT_NAVI1 = 0xE03909, S_DOT_NAVI2 = 0xE03ADD;
constexpr uint32_t IMAGEVIEW_loadTexture = 0x278;

// APIManager's banner list (16-byte entries {type, id, Date}) entry `idx`; throws when out of range
uint32_t bannerEntry(uint32_t idx) {
    uint32_t list = ld32(call(APIManager_getInstance, {}) + 0x314);
    uint32_t b = ld32(list + 0xC);
    if (!(uint32_t(int32_t(ld32(list + 0x10) - b) >> 4) > idx)) {
        call(wl::VECTOR_AT_THROW, {list + 0xC});
        return 0;
    }
    return b + idx * 16;
}
// url = the master row string (0x201 bytes at `off`) of the row in the shared_ptr at `sp`
void rowString(uint32_t url, uint32_t sp, uint32_t off) {
    uint32_t sb = ld32(sp);
    if (!sb) return;
    Frame f;
    uint32_t buf = f.alloc(0x204);
    call(SafeBuffer_read, {sb, buf, ld32(sb + 0xC) + off, 0x201, off});
    call(String_assignN, {url, buf, host_strlen(buf)});
}
}  // namespace

// initBannerArea $_38 {self, dots, expireText}: (re)fill banner page `idx` (created by the view factory when
// null): its image (event stage / medal draw banner url, downloaded), its touch, and when it is the shown page
// the page dots and the expiration text
uint32_t SceneMyPage_banner38(uint32_t fo, uint32_t widgetRef, uint32_t idxRef, uint32_t shownRef) {
    const uint32_t shown = ld8(shownRef);
    const uint32_t idx = ld32(idxRef);
    uint32_t widget = ld32(widgetRef);
    const uint32_t self = ld32(fo + 4);
    if (!widget) {
        widget = call(WidgetFactory_call, {ld32(self + 0x400) + 0x500, idx});
        call(smain::lib::Ref_retain, {widget});
    }
    Frame frame;
    const uint32_t url = frame.alloc(12);
    st8(url, 0);
    st8(url + 1, 0);
    {
        uint32_t entry = bannerEntry(idx);
        if (!entry) return 0;
        uint32_t type = ld32(entry), id = ld32(entry + 4);
        Frame f;
        uint32_t row = f.alloc(8);
        if (type == 1) {
            call(EvStage_create, {row, id});
            rowString(url, row, 0x30);
        } else {
            call(DrawMedalType_create, {row, id});
            rowString(url, row, 0x21C);
        }
        sharedRelease(row);
    }
    const uint32_t sprite = findNode(widget, S_BANNER_IMAGE, 0xB);
    {
        uint32_t view = ld32(self + 0x400);
        float viewH = ldf(vcall(view, NODE_getContentSize, {view}) + 4);
        float h = ldf(vcall(sprite, NODE_getContentSize, {sprite}) + 4);
        vcall(sprite, 0x40, {sprite, fb(viewH / h)});
    }
    const uint32_t button = findNode(sprite, S_LINK_BUTTON, 0xA);
    {
        Frame f;
        uint32_t fo2 = f.alloc(20);
        st32(fo2 + 16, 0);
        uint32_t p = call(fn::operator_new, {0x14});
        if (p) {
            st32(p, VT_banner_touch);
            st32(p + 4, self);
            st32(p + 8, shown);
            st32(p + 0xC, idx);
        }
        st32(fo2 + 16, p);
        call(ImageButton_addTouchEventListener, {button, fo2});
        functionDestroy(fo2);
    }
    if (call(String_compareC, {url, S_EMPTY}) != 0)
        call(M_loadShopBanner, {self, sprite, idx, strIsLong(url) ? ld32(url + 8) : url + 1});
    if (shown) {
        const uint32_t dots = self + OFF_INTS;
        if (ld32(dots + 4) != ld32(dots)) {
            uint32_t i = 0;
            do {
                uint32_t container = ld32(fo + 8);
                uint32_t dot;
                {
                    Frame f;
                    uint32_t buf = stringBuffer(f, {S_DOT_NAVI, i + 1});
                    uint32_t name = newStr(f, buf);
                    dot = SceneUtil::searchNodeByName(container, name);
                    strFree(name);
                }
                uint32_t load = ld32(ld32(dot) + IMAGEVIEW_loadTexture);
                Frame f;
                uint32_t tex = newStr(f, idx == i ? S_DOT_NAVI2 : S_DOT_NAVI1, 0x14);
                call(load, {dot, tex, load});
                strFree(tex);
                i++;
            } while (i < vecSize4(dots));
        }
        uint32_t entry = bannerEntry(idx);
        if (!entry) return 0;
        Frame f;
        uint32_t date = f.alloc(8);
        st32(date, ld32(entry + 8));
        st32(date + 4, ld32(entry + 0xC));
        uint32_t left = call(Date_calculateIntervalSinceNow, {date});
        uint32_t text = ld32(fo + 0xC);
        vcall(text, node::setVisible, {text, 1});
        call(SceneUtil_setShadow, {ld32(fo + 0xC)});
        call(M_updateExpirationDate, {self, ld32(fo + 0xC), left});
    }
    strFree(url);
    return widget;
}

// the banner page touch {self, shown, idx}: event stage banners open the adventure select on that event, the
// others the shop (after the page slides out)
void SceneMyPage_banner38Touch(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    uint32_t self = ld32(fo + 4);
    if (!ld8(self + 0x410) || ld8(self + 0x2F2) || ld8(self + 0x2F3) || !ld8(fo + 8)) return;
    call(AudioPlaySystemSEDecide, {});
    uint32_t entry = bannerEntry(ld32(fo + 0xC));
    if (!entry) return;
    if (ld32(entry) == 1) runOutThen(self, VT_banner_event, {0, ld32(fo + 0xC)});
    else runOutThen(self, VT_banner_shop, {});
}
void SceneMyPage_banner38Shop(uint32_t) {
    clearBackCallback();
    callWithEmpty(SceneShopTop_callShopAPI, {});
    toSceneLayer(GOT_CREATE_SHOP);
}
// {-, idx}: the adventure select opens on the banner's event (kind 3 when its row says 1 at +0x28, else 2)
void SceneMyPage_banner38Event(uint32_t fo) {
    uint32_t entry = bannerEntry(ld32(fo + 8));
    if (!entry) return;
    Frame f;
    uint32_t row = f.alloc(8);
    call(EvStage_create, {row, ld32(entry + 4)});
    uint32_t sb = ld32(row);
    uint32_t kind = 2;
    if (sb && sbRead(sb, 0x28, 4) == 1) kind = 3;
    st32(call(SceneManager_getInstance, {}) + 0x28C, kind);
    st32(call(SceneManager_getInstance, {}) + 0x2F0, ld32(entry + 4));
    clearBackCallback();
    callWithEmpty(SceneAdventureSelect_callAPI, {});
    {
        uint32_t sm = call(SceneManager_getInstance, {});
        Frame g;
        uint32_t fo2 = newFunction(g, VT_banner_eventScene, {});
        call(SceneManager_changeSceneLayer, {sm, fo2, 1, 1, 1});
        functionDestroy(fo2);
    }
    sharedRelease(row);
}

// ------------------------------------------------------------------------------------------------ popup / balloons
namespace {
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t FadeTo_create = 0x86B3E1;
constexpr uint32_t FunctionVV_copyCtor = 0x4683CD, FunctionVV_moveCtor = 0x47F905;
constexpr uint32_t ImageDownloader_loadImage = 0x5BD0CB;
constexpr uint32_t VT_popup_closed = 0xF613F4;
constexpr uint32_t S_AVATAR_ID = 0xE03794, S_AVATAR_STATUS = 0xE037FA, S_FB_PANEL = 0xDF8E5A, S_FB_PHOTO = 0xDF8E6E;
}  // namespace

// openLinkageAvatarPopup $_34 {-, popup, parent, shade, -, done}: OK touched: the popup closes (then the inner
// lambda), the shade fades out
void SceneMyPage_popup34(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    call(AudioPlaySystemSEDecide, {});
    const uint32_t popup = ld32(fo + 8);
    {
        Frame f;
        uint32_t lam = f.alloc(0x28);   // {parent, shade, popup, -, std::function copy of done}
        st32(lam, ld32(fo + 0xC));
        st32(lam + 4, ld32(fo + 0x10));
        st32(lam + 8, popup);
        call(FunctionVV_copyCtor, {lam + 0x10, fo + 0x18});
        uint32_t out = f.alloc(20);
        st32(out + 16, 0);
        uint32_t p = call(fn::operator_new, {0x30});
        if (p) {
            st32(p, VT_popup_closed);
            st32(p + 8, ld32(lam));
            st32(p + 0xC, ld32(lam + 4));
            st32(p + 0x10, ld32(lam + 8));
            call(FunctionVV_moveCtor, {p + 0x18, lam + 0x10});
        }
        st32(out + 16, p);
        uint32_t action = call(SceneUtil_createClosePopupAnimation, {popup, out});
        call(wl::Node_runAction, {popup, action});
        functionDestroy(out);
        functionDestroy(lam + 0x10);
    }
    uint32_t shade = ld32(fo + 0x10);
    call(wl::Node_runAction, {shade, call(FadeTo_create, {0x3DCCCCCDu, 0})});
}
// its inner lambda {parent, shade, popup, -, done}: both removed from the parent, then done() when set
void SceneMyPage_popup33Closed(uint32_t fo) {
    uint32_t parent = ld32(fo + 8);
    vcall(parent, NODE_removeChild, {parent, ld32(fo + 0xC), 1});
    parent = ld32(fo + 8);
    vcall(parent, NODE_removeChild, {parent, ld32(fo + 0x10), 1});
    if (ld32(fo + 0x28)) call(wl::FunctionVV_call, {fo + 0x18});
}

// updateFukidashi $_32 {-, id (2 words), self, facebook id}: the profile photo url arrived: load it into the
// member's balloon photo
void SceneMyPage_fukidashi32(uint32_t fo, uint32_t okRef, uint32_t urlRef) {
    if (!ld8(okRef)) return;
    const uint32_t url = ld32(urlRef);
    if (!url) return;
    const uint32_t self = ld32(fo + 0x10);
    Frame f;
    uint32_t buf = stringBuffer(f, {S_AVATAR_ID, ld32(fo + 8), ld32(fo + 0xC)});
    uint32_t name = newStr(f, buf);
    uint32_t layer = ld32(self + 0x2A4);
    uint32_t actor = vcall(layer, 0x104, {layer, name});
    if (actor) {
        uint32_t status = findNode(actor, S_AVATAR_STATUS, 13);
        uint32_t panel = findNode(status, S_FB_PANEL, 19);
        uint32_t photo = findNode(panel, S_FB_PHOTO, 14);
        uint32_t fbId = fo + 0x14;
        call(ImageDownloader_loadImage,
             {ld32(self + OFF_DOWNLOADER), photo, url, strIsLong(fbId) ? ld32(fo + 0x1C) : fbId + 1});
    }
    strFree(name);
}

// ------------------------------------------------------------------------------------------------ changeRankingSort
namespace {
constexpr uint32_t M_updateStyle = 0x6428FD, M_updateFukidashi = 0x642C39, M_moveAvatar = 0x6411CD;
constexpr uint32_t M_setAvatarProfileImageVisible = 0x641EF5;
constexpr uint32_t BuildMoveDone = 0x646291;              // sub_646290: the $_30 std::function
constexpr uint32_t FadeIn_create = 0x86B115;
}  // namespace

// $_29 {self, actor, id(2), startX, column, rowY, 0, self, actor, 1.0, 1, id(2), column, button}: the recycled
// actor faded out shows its new member, fades in and walks (or waits) to its line-up place, then $_30
void SceneMyPage_sort29(uint32_t fo) {
    const uint32_t self = ld32(fo + 8);
    call(M_updateStyle, {self, ld32(fo + 0xC), ld32(fo + 0x10), ld32(fo + 0x14)});
    call(M_updateFukidashi, {self, ld32(fo + 0xC), ld32(fo + 0x10), ld32(fo + 0x14), 1});
    call(M_setAvatarProfileImageVisible, {self, ld32(fo + 0xC), 1});
    {
        uint32_t avatar = ld32(ld32(fo + 0xC) + 0x288);
        call(wl::Node_runAction, {avatar, call(FadeIn_create, {fb(0.3f)})});
    }
    // the $_30 builder arguments: {1.0, 0 -> captured from +0x30.., id, column, button}
    const uint32_t a0 = ld32(fo + 0x30), a1 = ld32(fo + 0x34), a2 = ld32(fo + 0x38), a3 = ld32(fo + 0x3C);
    const uint32_t a4 = ld32(fo + 0x40), a5 = ld32(fo + 0x44);
    if (ld32(self + 0x2C0)) {
        uint32_t actor = ld32(fo + 0xC);
        float x = ldf(fo + 0x18) + float(int32_t(ld32(fo + 0x1C))) * 60.0f;
        float y = ldf(fo + 0x20) + 70.0f;
        Frame f;
        uint32_t pos = f.alloc(8);
        call(wl::Vec2_ctorXY, {pos, fb(x), fb(y)});
        uint32_t out = f.alloc(20);
        call(BuildMoveDone, {out, a1, ld32(fo + 0x28), ld32(fo + 0x2C), a0, a1, a2, a3, a4, a5});
        call(M_moveAvatar, {self, actor, pos, out, fb(0.3f)});
        functionDestroy(out);
        call(wl::Vec2_dtor, {pos});
    } else {
        uint32_t avatar = ld32(ld32(fo + 0xC) + 0x288);
        uint32_t delay = call(wl::DelayTime_create, {fb(0.3f)});
        Frame f;
        uint32_t out = f.alloc(20);
        call(BuildMoveDone, {out, a1, ld32(fo + 0x28), ld32(fo + 0x2C), a0, a1, a2, a3, a4, a5});
        uint32_t cf = call(wl::CallFunc_create, {out});
        call(wl::Node_runAction, {avatar, call(wl::Sequence_create, {delay, cf, 0})});
        functionDestroy(out);
    }
}

// $_30 {self, actor, scaleX, keepStyle(byte), id(2), column, button}: the actor arrived: idle, facing, its style
// refreshed unless kept; the first column re-enables the update button and the scrolling
void SceneMyPage_sort30(uint32_t fo) {
    const uint32_t self = ld32(fo + 8);
    call(M_addChangeMotionEvent, {self, ld32(fo + 0xC), MOTION_IDLE});
    uint32_t avatar = ld32(ld32(fo + 0xC) + 0x288);
    vcall(avatar, node::setScaleX, {avatar, ld32(fo + 0x10)});
    if (!ld8(fo + 0x14)) {
        call(M_updateStyle, {self, ld32(fo + 0xC), ld32(fo + 0x18), ld32(fo + 0x1C)});
        call(M_updateFukidashi, {self, ld32(fo + 0xC), ld32(fo + 0x18), ld32(fo + 0x1C), 1});
    }
    if (ld32(fo + 0x20) == 0) {
        st8(self + 0x3A9, 1);
        uint32_t button = ld32(fo + 0x24);
        vcall(button, BUTTON_setEnabled, {button, 1});
    }
}

// ------------------------------------------------------------------------------------------------ field avatars
namespace {
constexpr uint32_t M_resetAvatarAction = 0x643605, M_runAvatrAction = 0x6412E9;
constexpr uint32_t SceneUtil_openAvatarInfoPopup = 0x46A229;
constexpr uint32_t RankIndex_find = 0x490275;
constexpr uint32_t U64Vector_pushBackSlow = 0x4ECB6D;
constexpr uint32_t Random_getInstance = 0x492055, Random_generateUniformInt = 0x492225;
constexpr uint32_t VT_party_info = 0xF61604;

// index of `actor` in the field slots (self+0x2D4), -1 when absent
int32_t slotOf(uint32_t self, uint32_t actor) {
    const uint32_t a = self + OFF_ACTORS_A;
    int32_t bytes = int32_t(ld32(a + 4) - ld32(a));
    if (bytes < 1) return -1;
    int32_t k = 0;
    do {
        if (ld32(ld32(a) + uint32_t(k) * 4) == actor) return k;
        k++;
    } while (k < (bytes >> 2));
    return -1;
}
}  // namespace

// changeWalkingMode $_27 {self, actor}: the leader restyled with the own id, its photo shown, fades in
void SceneMyPage_walk27(uint32_t fo) {
    const uint32_t self = ld32(fo + 4);
    call(M_updateStyle, {self, ld32(self + 0x2BC), ld32(self + 0x3A0), ld32(self + 0x3A4)});
    call(M_setAvatarProfileImageVisible, {self, ld32(fo + 8), 1});
    uint32_t avatar = ld32(ld32(self + 0x2BC) + 0x288);
    call(wl::Node_runAction, {avatar, call(FadeIn_create, {fb(0.6f)})});
}

// changeWalkingMode $_26 {self}: walking again: every field actor (party / ranking kind 1) or only the leader
// starts its walk
void SceneMyPage_walk26(uint32_t fo) {
    const uint32_t self = ld32(fo + 4);
    st8(self + 0x2F0, 1);
    const uint32_t a = self + OFF_ACTORS_A;
    if (ld8(self + OFF_HAS_PARTY) || ld32(self + 0x28C) == 1) {
        if (int32_t(ld32(a + 4) - ld32(a)) < 1) return;
        int32_t i = 0;
        do {
            uint32_t at = ld32(a) + uint32_t(i) * 4;
            uint32_t actor = ld32(at);
            call(M_resetAvatarAction, {at, actor});
            call(M_runAvatrAction, {self, actor, uint32_t(i)});
            i++;
        } while (i < int32_t(ld32(a + 4) - ld32(a)) >> 2);
        return;
    }
    int32_t k = slotOf(self, ld32(self + 0x2BC));
    call(M_runAvatrAction, {self, ld32(self + 0x2BC), uint32_t(k < 0 ? 0 : k)});
}

// setAvatarProfileImageVisible $_22 {self, imageView, facebook id}: the photo url arrived
void SceneMyPage_profile22(uint32_t fo, uint32_t okRef, uint32_t urlRef) {
    if (!ld8(okRef)) return;
    uint32_t url = ld32(urlRef);
    if (!url) return;
    uint32_t fbId = fo + 0xC;
    call(ImageDownloader_loadImage, {ld32(ld32(fo + 4) + OFF_DOWNLOADER), ld32(fo + 8), url,
                                     strIsLong(fbId) ? ld32(fo + 0x14) : fbId + 1});
}

// createPartyAvatar $_21 {self, actor}: a ranking member touched: its info popup (then the inner lambda {self})
void SceneMyPage_party21(uint32_t fo, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    const uint32_t self = ld32(fo + 4);
    if (!ld8(self + 0x410) || ld8(self + 0x2F2) || ld8(self + 0x2F3) || ld8(self + 0x2F0) || !ld8(self + 0x2F1))
        return;
    uint32_t lo = 0, hi = 0;
    {
        int32_t k = slotOf(self, ld32(fo + 8));
        if (k >= 0) {
            const uint32_t ids = self + OFF_IDS;
            if (!(uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 3) > uint32_t(k))) {
                call(wl::VECTOR_AT_THROW, {ids});
                return;
            }
            uint32_t at = ld32(ids) + uint32_t(k) * 8;
            lo = ld32(at);
            hi = ld32(at + 4);
        }
    }
    const uint32_t rank = ld32(self + 0x2C0);
    if (!rank) return;
    if (!ld8(self + OFF_HAS_PARTY) && ld32(self + 0x28C) != 1) return;
    Frame f;
    uint32_t key = f.alloc(8);
    st32(key, lo);
    st32(key + 4, hi);
    uint32_t node = call(RankIndex_find, {rank + 0x24, key});
    if (!node) return;
    uint32_t idx = ld32(node + 0x10);
    if (idx == 0xFFFFFFFFu) return;
    st8(self + 0x2F2, 1);
    uint32_t rec = f.alloc(0x128);
    st8(rec + 0x11F, ld8(rec + 0x11F) & 0xFD);
    uint32_t off = idx * 0x128;
    call(SafeBuffer_read, {rank, rec, ld32(rank + 0xC) + off, 0x128, off});
    uint32_t done = newFunction(f, VT_party_info, {self});
    uint32_t canClose = newEmptyFunction(f);
    call(SceneUtil_openAvatarInfoPopup, {ld32(rec), ld32(rec + 4), 0, done, canClose});
    functionDestroy(canClose);
    functionDestroy(done);
}

// runAvatrAction $_18 {self, actor, Vec2}: the actor re-enters at the point: as the first ranking member not on
// the field (when any; a random one of them in the original's general form), with its photo shown
void SceneMyPage_action18(uint32_t fo) {
    const uint32_t self = ld32(fo + 4);
    Frame f;
    uint32_t tmp = f.alloc(8);
    uint32_t v = f.alloc(12);   // std::vector<uint64>
    st32(v, 0);
    st32(v + 4, 0);
    st32(v + 8, 0);
    uint32_t sb = ld32(self + 0x2C0);
    if (ld32(sb + 0x20)) {
        uint32_t rec = f.alloc(0x128);
        uint32_t k = 0;
        uint32_t flag = ld32(sb + 0x20);   // the first pass masks the record count (a register reuse)
        while (true) {
            st8(rec + 0x11F, uint8_t(flag & 0xFD));
            uint32_t off = 0x128 * k;
            call(SafeBuffer_read, {sb, rec, ld32(sb + 0xC) + off, 0x128, off});
            uint32_t lo = ld32(rec), hi = ld32(rec + 4);
            st32(tmp, lo);
            st32(tmp + 4, hi);
            const uint32_t ids = self + OFF_IDS;
            uint32_t end = ld32(ids + 4), p = ld32(ids);
            while (p != end) {
                if (ld32(p) == lo && ld32(p + 4) == hi) break;
                p += 8;
            }
            k++;
            if (p == end) {
                uint32_t e = ld32(v + 4);
                if (e == ld32(v + 8)) {
                    call(U64Vector_pushBackSlow, {v, tmp});
                } else {
                    uint32_t n = 0;
                    if (e) {
                        st32(e, lo);
                        st32(e + 4, hi);
                        n = ld32(v + 4);
                    }
                    st32(v + 4, n + 8);
                }
                break;
            }
            sb = ld32(self + 0x2C0);
            if (!(k < ld32(sb + 0x20))) break;
            flag = ld8(rec + 0x11F);
        }
    }
    const uint32_t count = uint32_t(int32_t(ld32(v + 4) - ld32(v)) >> 3);
    if (count) {
        uint32_t r = 0;
        if (int32_t(ld32(v + 4) - ld32(v)) >= 9)
            r = call(Random_generateUniformInt, {call(Random_getInstance, {}), 0, count - 1, 0});
        if (!(uint32_t(int32_t(ld32(v + 4) - ld32(v)) >> 3) > r)) {
            call(wl::VECTOR_AT_THROW, {v});
            return;
        }
        uint32_t at = ld32(v) + r * 8;
        uint32_t lo = ld32(at), hi = ld32(at + 4);
        call(M_updateStyle, {self, ld32(fo + 8), lo, hi});
        int32_t k = slotOf(self, ld32(fo + 8));
        if (k >= 0) {
            uint32_t dst = ld32(self + OFF_IDS) + uint32_t(k) * 8;
            st32(dst, lo);
            st32(dst + 4, hi);
        }
    }
    call(M_setAvatarProfileImageVisible, {self, ld32(fo + 8), 1});
    uint32_t actor = ld32(fo + 8);
    vcall(actor, 0x4C, {actor, fo + 0xC});
    call(U64Vector_baseDtor, {v});
}

// runAvatrAction $_17 {balloon, self, point, balloonIndex}: the comment balloon closes, its point and slot freed
void SceneMyPage_action17(uint32_t fo) {
    const uint32_t self = ld32(fo + 8);
    uint32_t balloon = ld32(fo + 4);
    vcall(balloon, node::setVisible, {balloon, 0});
    uint32_t i = ld32(fo + 0xC);
    uint32_t w = ld32(self + OFF_VEC_328) + ((i >> 3) & 0x1FFFFFFCu);
    st32(w, ld32(w) & ~(1u << (i & 31)));
    uint32_t j = ld32(fo + 0x10);
    uint32_t w2 = ld32(self + OFF_BALOON_SHOWN) + ((j >> 3) & 0x1FFFFFFCu);
    st32(w2, ld32(w2) & ~(1u << (j & 31)));
}

// ------------------------------------------------------------------------------------------------ runInAnimation
namespace {
constexpr uint32_t SceneUtil_openNewLuxPopup = 0x46FDC5, SceneManager_openPartyNewPopup = 0x61FC21;
constexpr uint32_t SceneManager_updateHeaderAvatarIcon = 0x611725;
constexpr uint32_t M_openLinkageAvatarPopup = 0x646429, LinkagePopupDone = 0x64EC91;   // sub_64EC90
constexpr uint32_t SceneRanking_openRankingRewardPopup = 0x6FF349;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31, SceneUtil_CompleteTutorialPhase = 0x472A75;
constexpr uint32_t SceneManager_Tutorial12Exec = 0x61379D, SceneManager_Tutorial18Exec = 0x613BE5;
constexpr uint32_t SceneManager_Tutorial15Exec = 0x613A45, SceneManager_tutorial_menu = 0x613CA1;
constexpr uint32_t SceneManager_isTutorial36 = 0x6134F5, SceneManager_ChainOfTutorialDeckEdit = 0x613B05;
constexpr uint32_t SceneManager_ChainOfTutorialAdventureSelect = 0x6219E1;
constexpr uint32_t VT_in79 = 0xF61B00, VT_in79_inner = 0xF61738, VT_in78 = 0xF61AD4, VT_in70_inner = 0xF61790;
constexpr uint32_t VT_in73 = 0xF6186C, VT_in74 = 0xF618F0, VT_in75 = 0xF61974, VT_in76 = 0xF619F8, VT_in77 = 0xF61A7C;
constexpr uint32_t VT_in74_err = 0xF61898, VT_in75_err = 0xF6191C, VT_in76_err = 0xF619A0, VT_in77_err = 0xF61A24;
constexpr uint32_t VT_in78_err = 0xF61AA8;

// a std::function holding a heap __func {vt, words...} (allocation of 4 + 4 * n bytes)
uint32_t heapFunction(Frame &f, uint32_t size, uint32_t vt, std::initializer_list<uint32_t> words) {
    uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    uint32_t p = call(fn::operator_new, {size});
    if (p) {
        st32(p, vt);
        uint32_t k = 4;
        for (uint32_t w : words) {
            st32(p + k, w);
            k += 4;
        }
    }
    st32(fo + 16, p);
    return fo;
}
// the chained ranking reward popups: loaded -> next popup (heap {vt, 4 words}); failed -> the API error dialog
void rewardChain(uint32_t fo, uint32_t okRef, uint32_t nextVt, uint32_t errVt) {
    const uint32_t ok = ld8(okRef);
    call(SceneManager_enableLoadingIndicator, {call(SceneManager_getInstance, {}), 0});
    Frame f;
    if (ok) {
        uint32_t next = heapFunction(f, 0x14, nextVt, {ld32(fo + 4), ld32(fo + 8), ld32(fo + 0xC), ld32(fo + 0x10)});
        call(SceneRanking_openRankingRewardPopup, {0, next});
        functionDestroy(next);
    } else if (ld8(call(APIManager_getInstance, {}) + 0x14)) {
        uint32_t api = call(APIManager_getInstance, {});
        uint32_t err = newFunction(f, errVt, {});
        call(APIManager_openErrorDialog, {api, err, 1});
        functionDestroy(err);
    }
}
}  // namespace

// runInAnimation $_15 {parent, curtain, self, 5 words}: the curtain removed, the field at its base scale, the
// leader walks; then the new Lux popup ($_79) or the new party popup
void SceneMyPage_in15(uint32_t fo) {
    {
        uint32_t parent = ld32(fo + 4);
        vcall(parent, NODE_removeChild, {parent, ld32(fo + 8), 1});
    }
    const uint32_t self = ld32(fo + 0xC);
    uint32_t panel = ld32(self + 0x298);
    vcall(panel, 0x40, {panel, ld32(self + OFF_SCALE_BASE)});
    uint32_t center = ld32(self + 0x294);
    vcall(center, 0x40, {center, ld32(self + OFF_SCALE_BASE)});
    st8(self + 0x2F9, 1);
    int32_t k = slotOf(self, ld32(self + 0x2BC));
    call(M_runAvatrAction, {self, ld32(self + 0x2BC), uint32_t(k < 0 ? 0 : k)});
    uint32_t api = call(APIManager_getInstance, {});
    Frame f;
    const uint32_t w0 = ld32(fo + 0x10), w1 = ld32(fo + 0x14), w2 = ld32(fo + 0x18), w3 = ld32(fo + 0x1C);
    const uint32_t w4 = ld32(fo + 0x20);
    if (ld32(api + 0x30C) != ld32(api + 0x308)) {
        uint32_t next = heapFunction(f, 0x18, VT_in79, {w0, w1, w2, w3, w4});
        call(SceneUtil_openNewLuxPopup, {0, next});
        functionDestroy(next);
    } else {
        uint32_t sm = call(SceneManager_getInstance, {});
        uint32_t next = heapFunction(f, 0x18, VT_in79_inner, {w0, w1, w2, w3, w4});
        call(SceneManager_openPartyNewPopup, {sm, next});
        functionDestroy(next);
    }
}
// $_79 {5 words}: the new party popup (then its inner lambda)
void SceneMyPage_in79(uint32_t fo) {
    const uint32_t w0 = ld32(fo + 4);
    uint32_t sm = call(SceneManager_getInstance, {});
    Frame f;
    uint32_t next = heapFunction(f, 0x18, VT_in79_inner,
                                 {w0, ld32(fo + 8), ld32(fo + 0xC), ld32(fo + 0x10), ld32(fo + 0x14)});
    call(SceneManager_openPartyNewPopup, {sm, next});
    functionDestroy(next);
}
// $_79 inner {w0, w1..w4}: with linked avatars to show: header icon refreshed and the linkage popup ($_78),
// otherwise straight on (sub_64EC90 on w1..)
void SceneMyPage_in79Inner(uint32_t fo) {
    uint32_t list = ld32(call(APIManager_getInstance, {}) + 0x314);
    if (!ld8(list + 0x1C)) {
        call(LinkagePopupDone, {fo + 8});
        return;
    }
    call(SceneManager_updateHeaderAvatarIcon, {call(SceneManager_getInstance, {}), 0, 0xFFFFFFFFu});
    Frame f;
    uint32_t next = heapFunction(f, 0x14, VT_in78, {ld32(fo + 8), ld32(fo + 0xC), ld32(fo + 0x10), ld32(fo + 0x14)});
    call(M_openLinkageAvatarPopup, {ld32(next + 16), next});   // r0 still holds the new __func (`this` unused)
    functionDestroy(next);
}

// $_70: the tutorial step that follows the field's entrance
void SceneMyPage_in70(uint32_t) {
    const int32_t phase = int32_t(call(SceneUtil_getNexTutorialphase, {}));
    auto chainAdventure = [] {
        call(SceneManager_ChainOfTutorialAdventureSelect, {call(SceneManager_getInstance, {}), 0x2C});
    };
    if (phase <= 0xDB) {
        if (phase != 0xD2) return;
        uint32_t sm = call(SceneManager_getInstance, {});
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(SceneManager_Tutorial12Exec, {sm, empty});
        functionDestroy(empty);
    } else if (phase <= 0x12B) {
        if (phase != 0xDC) return;
        if (call(SceneManager_isTutorial36, {call(SceneManager_getInstance, {})}) == 1) {
            call(SceneManager_ChainOfTutorialDeckEdit, {call(SceneManager_getInstance, {}), 0x24});
        } else {
            Frame f;
            uint32_t next = newFunction(f, VT_in70_inner, {});
            call(SceneUtil_CompleteTutorialPhase, {next, 0, 1});
            functionDestroy(next);
        }
    } else if (phase <= 0x18F) {
        if (phase == 300) chainAdventure();
    } else if (phase <= 0x199) {
        if (phase == 400) chainAdventure();
    } else if (phase <= 0x1F3) {
        if (phase != 0x19A) return;
        uint32_t sm = call(SceneManager_getInstance, {});
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        call(SceneManager_Tutorial18Exec, {sm, empty});
        functionDestroy(empty);
    } else if (phase <= 0x257) {
        if (phase == 500) chainAdventure();
    } else if (phase == 600) {
        chainAdventure();
    } else if (phase == 700) {
        call(SceneManager_Tutorial15Exec, {call(SceneManager_getInstance, {})});
    } else if (phase == 710) {
        call(SceneManager_tutorial_menu, {call(SceneManager_getInstance, {})});
    }
}

// $_74..$_78 inner lambdas (bool loaded): the ranking reward popups chained
void SceneMyPage_in74Inner(uint32_t fo, uint32_t okRef) { rewardChain(fo, okRef, VT_in73, VT_in74_err); }
void SceneMyPage_in75Inner(uint32_t fo, uint32_t okRef) { rewardChain(fo, okRef, VT_in74, VT_in75_err); }
void SceneMyPage_in76Inner(uint32_t fo, uint32_t okRef) { rewardChain(fo, okRef, VT_in75, VT_in76_err); }
void SceneMyPage_in77Inner(uint32_t fo, uint32_t okRef) { rewardChain(fo, okRef, VT_in76, VT_in77_err); }
void SceneMyPage_in78Inner(uint32_t fo, uint32_t okRef) { rewardChain(fo, okRef, VT_in77, VT_in78_err); }

// ------------------------------------------------------------------------------------------------ init
namespace {
constexpr uint32_t Touch_getLocation = 0x900E57;
constexpr uint32_t Director_getInstance = 0x8EE519, Director_getVisibleSize = 0x8F0675;
constexpr uint32_t M_clearMotionEvents = 0x63FA5B, M_updateScrollSortAvatars = 0x63FC49;
constexpr uint32_t M_updateScrollAvatarsMotion = 0x63FB09, M_changeRankingMode = 0x64397D;
constexpr uint32_t M_changeRankingSort = 0x645141;
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595, GOT_CREATE_PARTY_TOP = 0xFD6620;
constexpr uint32_t VT_init1_out = 0xF61E70, VT_init2_out = 0xF61E18, VT_init3_out = 0xF61DC0;
constexpr uint32_t VT_init4_out = 0xF61D68, VT_init5_out = 0xF61D10, VT_init6_out = 0xF61CB8;
constexpr uint32_t MOTION_WALK = 0x1C;

// every line-up actor gets `motion` (the line-up motion remembered at +0x3D0)
void lineupMotion(uint32_t self, uint32_t motion) {
    const uint32_t lineup = self + OFF_ACTORS_B;
    if (int32_t(ld32(lineup + 4) - ld32(lineup)) < 1) return;
    int32_t i = 0;
    do {
        call(M_addChangeMotionEvent, {self, ld32(ld32(lineup) + uint32_t(i) * 4), motion});
        i++;
    } while (i < int32_t(ld32(lineup + 4) - ld32(lineup)) >> 2);
}
// a menu button that leaves the page: touch ended, page touchable, not locked or walking
bool pageButtonTouch(uint32_t self, uint32_t typeRef) {
    return menuTouch(self, typeRef) && !ld8(self + 0x2F0);
}
// to the party top (back callback cleared, party API, scene change), the button disabled
void toPartyTop(uint32_t button) {
    clearBackCallback();
    callWithEmpty(ScenePartyTop_callPartyTopAPI, {});
    toSceneLayer(GOT_CREATE_PARTY_TOP);
    vcall(button, BUTTON_setEnabled, {button, 0});
}
}  // namespace

// init $_11 {self}: line-up touch ended: no drag; when not scrolling, everyone idles
void SceneMyPage_init11(uint32_t fo) {
    const uint32_t self = ld32(fo + 4);
    st32(self + 0x2F4, 0);
    st8(self + 0x3AB, 0);
    if (ldf(self + 0x3AC) == 0.0f) {
        call(M_clearMotionEvents, {self});
        st32(self + 0x3D0, MOTION_IDLE);
        lineupMotion(self, MOTION_IDLE);
    }
}

// init $_10 {self}: line-up touch moved: past 5 % of the screen width it becomes a drag (everyone walks); a
// drag scrolls the line-up by the move (in field scale)
void SceneMyPage_init10(uint32_t fo, uint32_t touchRef) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t self = ld32(fo + 4);
    st32(self + 0x2F4, 0);
    Frame f;
    uint32_t loc = f.alloc(8);
    call(Touch_getLocation, {loc, touch});
    const float delta = (ldf(loc) - ldf(self + 0x384)) / ldf(self + OFF_SCALE_BASE);
    uint32_t size = f.alloc(8);
    call(Director_getVisibleSize, {size, call(Director_getInstance, {})});
    bool scroll = true;
    if (!ld8(self + 0x3AA)) {
        float d = ldf(self + OFF_CAMERA) - ldf(loc);
        float w = ldf(size) * bf(0x3D4CCCCDu);
        float ad = bf(fb(d) & 0x7FFFFFFFu);
        if (w >= ad || w != w || ad != ad) {
            scroll = false;
        } else {
            st8(self + 0x3AA, 1);
            if (ld32(self + 0x3D0) != MOTION_WALK) {
                call(M_clearMotionEvents, {self});
                lineupMotion(self, MOTION_WALK);
                st32(self + 0x3D0, MOTION_WALK);
                if (!ld8(self + 0x3AA)) scroll = false;
            }
        }
    }
    if (scroll) {
        stf(self + 0x3AC, delta + ldf(self + 0x3AC));
        uint32_t moved = call(M_updateScrollSortAvatars, {self, fb(delta)});
        call(M_updateScrollAvatarsMotion, {self, moved, 1});
    }
    st32(self + 0x384, ld32(loc));
    call(wl::Vec2_dtor, {loc});
}

// init $_9 {self}: line-up touch began: remembers the point; claims the touch in the ranking mode
uint32_t SceneMyPage_init9(uint32_t fo, uint32_t touchRef) {
    const uint32_t touch = ld32(touchRef);
    const uint32_t self = ld32(fo + 4);
    if (!ld8(self + 0x3A9)) return 0;
    {
        Frame f;
        uint32_t loc = f.alloc(8);
        call(Touch_getLocation, {loc, touch});
        st32(self + 0x37C, ld32(loc + 4));
        st32(self + OFF_CAMERA, ld32(loc));
        call(wl::Vec2_dtor, {loc});
    }
    st32(self + 0x384, ld32(self + OFF_CAMERA));
    if (ld8(self + 0x2F0)) return 0;
    if (!ld8(self + OFF_HAS_PARTY) && ld32(self + 0x28C) != 1) return 0;
    st32(self + 0x2F4, 0);
    st8(self + 0x3AB, 1);
    st8(self + 0x3AA, 0);
    return 1;
}

// init $_8 {self}: the ranking button
void SceneMyPage_init8(uint32_t fo, uint32_t, uint32_t typeRef) {
    const uint32_t self = ld32(fo + 4);
    if (!menuTouch(self, typeRef)) return;
    call(M_changeRankingMode, {self});
}

// init $_7 {self}: the sort button: next sort mode (0..2) and re-sort
void SceneMyPage_init7(uint32_t fo, uint32_t, uint32_t typeRef) {
    const uint32_t self = ld32(fo + 4);
    if (!pageButtonTouch(self, typeRef)) return;
    if (!ld8(self + OFF_HAS_PARTY) && ld32(self + 0x28C) != 1) return;
    if (!ld8(self + 0x3A9)) return;
    st32(self + 0x3AC, 0);
    call(AudioPlaySystemSEDecide, {});
    uint32_t next = ld8(self + 0x3A8) + 1;
    uint32_t mode = (next & 0xFF) > 2 ? 0 : next;
    st8(self + 0x3A8, uint8_t(mode));
    call(M_changeRankingSort, {self, mode & 0xFF});
}

// init $_6 / $_5 / $_1 {self, button}: party buttons: the page slides out, then the party top ({button})
void SceneMyPage_init6(uint32_t fo, uint32_t, uint32_t typeRef) {
    const uint32_t self = ld32(fo + 4);
    if (!pageButtonTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    runOutThen(self, VT_init6_out, {ld32(fo + 8)});
}
void SceneMyPage_init5(uint32_t fo, uint32_t, uint32_t typeRef) {
    const uint32_t self = ld32(fo + 4);
    if (!pageButtonTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    runOutThen(self, VT_init5_out, {ld32(fo + 8)});
}
void SceneMyPage_init1(uint32_t fo, uint32_t, uint32_t typeRef) {
    const uint32_t self = ld32(fo + 4);
    if (!pageButtonTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    runOutThen(self, VT_init1_out, {ld32(fo + 8)});
}
void SceneMyPage_init6Out(uint32_t fo) { toPartyTop(ld32(fo + 4)); }
void SceneMyPage_init5Out(uint32_t fo) { toPartyTop(ld32(fo + 4)); }
void SceneMyPage_init1Out(uint32_t fo) { toPartyTop(ld32(fo + 4)); }

// init $_4 / $_3 / $_2 {self, button}: the page slides out, then {button, self}
void SceneMyPage_init4(uint32_t fo, uint32_t, uint32_t typeRef) {
    const uint32_t self = ld32(fo + 4);
    if (!pageButtonTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    runOutThen(self, VT_init4_out, {ld32(fo + 8), self});
}
void SceneMyPage_init3(uint32_t fo, uint32_t, uint32_t typeRef) {
    const uint32_t self = ld32(fo + 4);
    if (!pageButtonTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    runOutThen(self, VT_init3_out, {ld32(fo + 8), self});
}
void SceneMyPage_init2(uint32_t fo, uint32_t, uint32_t typeRef) {
    const uint32_t self = ld32(fo + 4);
    if (!pageButtonTouch(self, typeRef)) return;
    call(AudioPlaySystemSEDecide, {});
    runOutThen(self, VT_init2_out, {ld32(fo + 8), self});
}

// sub_6395B4: an (unnamed) touch lambda next to the scene: touch ended -> back to the title scene
void SceneMyPage_toTitle(uint32_t, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != TOUCH_ENDED) return;
    toSceneLayer(0xFD68B4);   // GOT: createSceneLayer<SceneTitle>
}

RW_FUNCTION(0x6395B4, "sub_6395B4 (touch lambda: to SceneTitle)", SceneMyPage_toTitle);
RW_FUNCTION(0x650E0C, "SceneMyPage::init::$_11::operator()", SceneMyPage_init11);
RW_FUNCTION(0x650F10, "SceneMyPage::init::$_10::operator()", SceneMyPage_init10);
RW_FUNCTION(0x651128, "SceneMyPage::init::$_9::operator()", SceneMyPage_init9);
RW_FUNCTION(0x65127C, "SceneMyPage::init::$_8::operator()", SceneMyPage_init8);
RW_FUNCTION(0x651344, "SceneMyPage::init::$_7::operator()", SceneMyPage_init7);
RW_FUNCTION(0x651468, "SceneMyPage::init::$_6::operator()", SceneMyPage_init6);
RW_FUNCTION(0x6515DC, "SceneMyPage::init::$_6 page-out lambda operator()", SceneMyPage_init6Out);
RW_FUNCTION(0x6517B0, "SceneMyPage::init::$_5::operator()", SceneMyPage_init5);
RW_FUNCTION(0x651924, "SceneMyPage::init::$_5 page-out lambda operator()", SceneMyPage_init5Out);
RW_FUNCTION(0x651AF8, "SceneMyPage::init::$_4::operator()", SceneMyPage_init4);
RW_FUNCTION(0x651D2C, "SceneMyPage::init::$_3::operator()", SceneMyPage_init3);
RW_FUNCTION(0x651F60, "SceneMyPage::init::$_2::operator()", SceneMyPage_init2);
RW_FUNCTION(0x652194, "SceneMyPage::init::$_1::operator()", SceneMyPage_init1);
RW_FUNCTION(0x652308, "SceneMyPage::init::$_1 page-out lambda operator()", SceneMyPage_init1Out);
RW_FUNCTION(0x64E8FC, "SceneMyPage::runInAnimation::$_15::operator()", SceneMyPage_in15);
RW_FUNCTION(0x650AB8, "SceneMyPage::runInAnimation::$_79::operator()", SceneMyPage_in79);
RW_FUNCTION(0x64EB68, "SceneMyPage::runInAnimation::$_79 inner lambda operator()", SceneMyPage_in79Inner);
RW_FUNCTION(0x64F5E8, "SceneMyPage::runInAnimation::$_70::operator()", SceneMyPage_in70);
RW_FUNCTION(0x64FABC, "SceneMyPage::runInAnimation::$_74 inner lambda operator()", SceneMyPage_in74Inner);
RW_FUNCTION(0x64FDCC, "SceneMyPage::runInAnimation::$_75 inner lambda operator()", SceneMyPage_in75Inner);
RW_FUNCTION(0x6500DC, "SceneMyPage::runInAnimation::$_76 inner lambda operator()", SceneMyPage_in76Inner);
RW_FUNCTION(0x6503EC, "SceneMyPage::runInAnimation::$_77 inner lambda operator()", SceneMyPage_in77Inner);
RW_FUNCTION(0x6506FC, "SceneMyPage::runInAnimation::$_78 inner lambda operator()", SceneMyPage_in78Inner);
RW_FUNCTION(0x64D5A0, "SceneMyPage::changeWalkingMode::$_27::operator()", SceneMyPage_walk27);
RW_FUNCTION(0x64D682, "SceneMyPage::changeWalkingMode::$_26::operator()", SceneMyPage_walk26);
RW_FUNCTION(0x64DA4C, "SceneMyPage::setAvatarProfileImageVisible::$_22::operator()", SceneMyPage_profile22);
RW_FUNCTION(0x64DB20, "SceneMyPage::createPartyAvatar::$_21::operator()", SceneMyPage_party21);
RW_FUNCTION(0x64E3C4, "SceneMyPage::runAvatrAction::$_18::operator()", SceneMyPage_action18);
RW_FUNCTION(0x64E638, "SceneMyPage::runAvatrAction::$_17::operator()", SceneMyPage_action17);
RW_FUNCTION(0x64C6C8, "SceneMyPage::changeRankingSort::$_29::operator()", SceneMyPage_sort29);
RW_FUNCTION(0x64C988, "SceneMyPage::changeRankingSort::$_30::operator()", SceneMyPage_sort30);
RW_FUNCTION(0x64BD04, "SceneMyPage::openLinkageAvatarPopup::$_34::operator()", SceneMyPage_popup34);
RW_FUNCTION(0x64BFEC, "SceneMyPage::openLinkageAvatarPopup::$_33 inner lambda operator()", SceneMyPage_popup33Closed);
RW_FUNCTION(0x64C1BC, "SceneMyPage::updateFukidashi::$_32::operator()", SceneMyPage_fukidashi32);
RW_FUNCTION(0x64A9E0, "SceneMyPage::initBannerArea::$_38::operator()", SceneMyPage_banner38);
RW_FUNCTION(0x64AFE4, "SceneMyPage::initBannerArea::$_38 touch lambda operator()", SceneMyPage_banner38Touch);
RW_FUNCTION(0x64B1BC, "SceneMyPage::initBannerArea::$_38 shop lambda operator()", SceneMyPage_banner38Shop);
RW_FUNCTION(0x64B378, "SceneMyPage::initBannerArea::$_38 event lambda operator()", SceneMyPage_banner38Event);
RW_FUNCTION(0x64A6D4, "SceneMyPage::initBannerArea::$_41::operator()", SceneMyPage_banner41);
RW_FUNCTION(0x64A804, "SceneMyPage::initBannerArea::$_40::operator()", SceneMyPage_banner40);
RW_FUNCTION(0x64B64C, "SceneMyPage::initBannerArea::$_37::operator()", SceneMyPage_banner37);
RW_FUNCTION(0x64B92C, "SceneMyPage::loadShopBanner::$_36::operator()", SceneMyPage_banner36);
