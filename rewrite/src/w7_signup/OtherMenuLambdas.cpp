// Other menu callbacks: the legal popup's User Agreement / License web pages, the serial code answer, the user code (transfer) popup, the
// Facebook login / link / unlink / logout chains (the Facebook ones are unreachable without a Facebook login, so
// they stay unverified) and the scene change back to MyPage.
#include "../w6_signup/Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t function_vb_copy = 0x45C84D, function_vb_move = 0x47BC69, function_vb_call = 0x474139;
constexpr uint32_t function_vb_assign = 0x490109;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_closeCurrentPopup = 0x6216A1;
constexpr uint32_t SM_canFinishScene = 0x6214F5;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775, SceneUtil_openOkDialog8Line = 0x45CA3D;
constexpr uint32_t SceneUtil_openWebViewInfoPopup = 0x47463D;   // (title, url, function<void(Node*)>, post)
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9, SceneUtil_callPrepareMypageAPIs = 0x46F37D;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D, APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_executeUserLinkUpdate = 0x4D41CD, APIManager_executeUserLinkDelete = 0x4D43B1;
constexpr uint32_t FacebookManager_getInstance = 0x4E5585, FacebookManager_getCurrentAccessToken = 0x4E57F1;
constexpr uint32_t FacebookManager_logout = 0x4E5751, FacebookManager_refreshCurrentAccessToken = 0x4E573D;
constexpr uint32_t OtherMenuDialog_updateFacebookButton = 0x59F819, OtherMenuDialog_openUserCodePopup = 0x5A18D9;
constexpr uint32_t OtherMenuDialog_openFacebookLoginLogout = 0x59FA79;
constexpr uint32_t OtherMenu_logoutCallback = 0x5A9A49;       // sub_5A9A48: the logout callback {answer, dialog}
constexpr uint32_t SideMenu_changeScene = 0x5AAB41;           // sub_5AAB40
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_MYPAGE = 0xFD6078;
constexpr uint32_t VT_SerialAccepted = 0xF5BB10;  // {dialog, area, button, button} (heap 0x14)
constexpr uint32_t VT_SerialError = 0xF5BB3C;    // {}
constexpr uint32_t VT_LinkUpdated = 0xF5BCF4, VT_LinkFailed = 0xF5BDA4, VT_LinkDone = 0xF5BD20;   // {pad, answer}
constexpr uint32_t VT_LinkError = 0xF5BD4C, VT_LoggedOut = 0xF5BD78;                           // {pad, answer}
constexpr uint32_t VT_LinkDeleted = 0xF5BDFC;     // {pad, answer, dialog} (heap 0x28)
constexpr uint32_t VT_UnlinkError = 0xF5BE80;     // {pad, answer, dialog} (heap 0x28)
constexpr uint32_t VT_UnlinkDone = 0xF5BE54;      // {pad, answer} (heap 0x20)
constexpr uint32_t VT_UserCodeAgain = 0xF5BED8;   // {dialog}
constexpr uint32_t VT_UserCodeError = 0xF5BF04;   // {}
constexpr uint32_t VT_TokenRefreshed = 0xF5C00C;  // {button, -, dialog}
constexpr uint32_t VT_FacebookAnswer = 0xF5BFE0;  // {button, 0}
constexpr uint32_t V_setTouchEnabled = 0x29C;
constexpr uint32_t API_ERROR_SHOWN = 0x14, API_SYSTEM = 0x1A8, API_USER_CODE_SET = 0x35D, API_USER_CODE_USED = 0x35C;
constexpr uint32_t SYSTEM_AGREEMENT_URL = 0x3C, SYSTEM_LICENSE_URL = 0x48;
constexpr uint32_t DIALOG_BUSY = 0x29C, DIALOG_AREA = 0x280;
constexpr uint32_t SM_RETRY_FLAG = 0x4CC, SM_RETRY_FN = 0x548;
constexpr uint32_t FB_CANCELLED = 3;

void setTouch(uint32_t widget, uint32_t on) { vcall(widget, V_setTouchEnabled, {widget, on}); }

// a heap __func (`size` bytes) at `dst` holding `vt` and a std::function<void(bool)> moved from `src` at +8
uint32_t heapAnswer(uint32_t dst, uint32_t vt, uint32_t size, uint32_t src) {
    const uint32_t obj = heapObject(vt, size);
    if (obj) call(function_vb_move, {obj + 8, src});
    st32(dst + 16, obj);
    return obj;
}

// a by-value std::string argument taken over (libc++ moves it out of the caller's slot); freed by the caller of this
struct MovedString { uint8_t tag; uint32_t data; };
MovedString takeString(uint32_t s) {
    const MovedString m{ld8(s), ld32(s + 8)};
    st32(s, 0);
    st32(s + 4, 0);
    st32(s + 8, 0);
    return m;
}
void freeMoved(const MovedString &m) {
    if (m.tag & 1) call(fn::operator_delete, {m.data});
}

// openOkDialog4Line / 8Line with text `id` and a heap callback holding a copy of `answer`
void okDialogWithAnswer(uint32_t dialog, int32_t id, uint32_t vt, uint32_t answer) {
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, answer});
    const uint32_t td = textData(f, id);
    const uint32_t s = newStr(f, textDataStr(td));
    const uint32_t inner = f.alloc(20);
    functionCopy(inner, copy);
    const uint32_t fo = f.alloc(20);
    heapAnswer(fo, vt, 0x20, inner);
    call(dialog, {s, fo, 0});
    functionDestroy(fo);
    functionDestroy(inner);
    strFree(s);
    sharedRelease(td);
    functionDestroy(copy);
}

// a web page in the info popup: title text `id`, the system data's URL at `urlField`
void webPage(int32_t id, uint32_t urlField) {
    const uint32_t system = ld32(call(fn::APIManager_getInstance, {}) + API_SYSTEM);
    Frame f;
    const uint32_t url = f.alloc(12);
    const uint32_t src = system + urlField;
    if (strIsLong(src)) strInit(url, ld32(src + 8), ld32(src + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(url + k, ld32(src + k));
    const uint32_t td = textData(f, id);
    const uint32_t title = newStr(f, textDataStr(td));
    const uint32_t page = newStr(f, strData(url));
    const uint32_t none = newEmptyFunction(f);
    const uint32_t post = f.alloc(12);
    st8(post, 0);
    st8(post + 1, 0);
    call(SceneUtil_openWebViewInfoPopup, {title, page, none, post});
    strFree(post);
    functionDestroy(none);
    strFree(page);
    strFree(title);
    sharedRelease(td);
    strFree(url);
}
}  // namespace

namespace other_menu_lambdas {

// the serial code sent: accepted - "go to your presents" (closing re-enables the menu); refused - the error dialog,
// and unless the request retries, the menu taking touches again
uint32_t serialAnswer(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    const uint32_t dialog = ld32(functor + 4);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        Frame f;
        const uint32_t td = textData(f, 104000007);
        const uint32_t s = newStr(f, textDataStr(td));
        const uint32_t fo = f.alloc(20);
        const uint32_t obj = heapObject(VT_SerialAccepted, 0x14);
        if (obj) {
            st32(obj + 4, dialog);
            st32(obj + 8, ld32(functor + 8));
            st32(obj + 0xC, ld32(functor + 0xC));
            st32(obj + 0x10, ld32(functor + 0x10));
        }
        st32(fo + 16, obj);
        call(SceneUtil_openOkDialog4Line, {s, fo, 0});
        functionDestroy(fo);
        strFree(s);
        sharedRelease(td);
        return 0;
    }
    if (ld8(call(fn::APIManager_getInstance, {}) + API_ERROR_SHOWN)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_SerialError, {});
        call(APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }
    if (!call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) {
        st8(dialog + DIALOG_BUSY, 0);
        setTouch(ld32(ld32(functor + 8) + DIALOG_AREA), 1);
        setTouch(ld32(functor + 0xC), 1);
        setTouch(ld32(functor + 0x10), 1);
    }
    return 0;
}
RW_FUNCTION(0x5A71C8, "OtherMenuDialog serial code send answer operator()(bool) (sub_5A71C8)", other_menu_lambdas::serialAnswer);

// "go to your presents" closed: the menu takes touches again
uint32_t serialAcceptedClosed(uint32_t functor) {
    st8(ld32(functor + 4) + DIALOG_BUSY, 0);
    setTouch(ld32(ld32(functor + 8) + DIALOG_AREA), 1);
    setTouch(ld32(functor + 0xC), 1);
    setTouch(ld32(functor + 0x10), 1);
    return call(SM_closeCurrentPopup, {sceneManager(), 1});
}
RW_FUNCTION(0x5A745A, "OtherMenuDialog serial code accepted closed operator() (sub_5A745A)", other_menu_lambdas::serialAcceptedClosed);

uint32_t licensePageTouched(uint32_t, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) == 2) webPage(104040003, SYSTEM_LICENSE_URL);
    return 0;
}
RW_FUNCTION(0x5A7BC0, "OtherMenuDialog legal popup License page touch operator() (sub_5A7BC0)", other_menu_lambdas::licensePageTouched);

uint32_t agreementPageTouched(uint32_t, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) == 2) webPage(104040001, SYSTEM_AGREEMENT_URL);
    return 0;
}
RW_FUNCTION(0x5A7E00, "OtherMenuDialog legal popup User Agreement page touch operator() (sub_5A7E00)", other_menu_lambdas::agreementPageTouched);

// Facebook logged in (bool, error, message): link the account to the token; failed (not cancelled): "could not
// log in"; cancelled: the answer with false
uint32_t facebookLoggedIn(uint32_t functor, uint32_t okRef, uint32_t errRef, uint32_t msg) {
    const MovedString m = takeString(msg);
    const uint32_t err = ld32(errRef);
    const uint32_t answer = functor + 8;
    if (ld8(okRef)) {
        call(OtherMenuDialog_updateFacebookButton, {ld32(functor + 0x20)});
        Frame f;
        const uint32_t copy = f.alloc(20);
        call(function_vb_copy, {copy, answer});
        call(SM_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t fb = call(FacebookManager_getInstance, {});
        const uint32_t token = f.alloc(12);
        call(FacebookManager_getCurrentAccessToken, {token, fb});
        const uint32_t inner = f.alloc(20);
        functionCopy(inner, copy);
        const uint32_t fo = f.alloc(20);
        heapAnswer(fo, VT_LinkUpdated, 0x20, inner);
        call(APIManager_executeUserLinkUpdate, {api, strData(token), 1, fo});
        functionDestroy(fo);
        functionDestroy(inner);
        strFree(token);
        functionDestroy(copy);
    } else if (err != FB_CANCELLED) {
        okDialogWithAnswer(SceneUtil_openOkDialog4Line, 104900015, VT_LinkFailed, answer);
    } else if (ld32(functor + 0x18)) {
        call(function_vb_call, {answer, 0});
    }
    freeMoved(m);
    return 0;
}
RW_FUNCTION(0x5A82A8, "OtherMenuDialog Facebook login answer operator() (sub_5A82A8)", other_menu_lambdas::facebookLoggedIn);

// the link sent: "linked" (8 lines) or the error dialog
uint32_t linkUpdated(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    const uint32_t answer = functor + 8;
    if (ok) {
        okDialogWithAnswer(SceneUtil_openOkDialog8Line, 104900005, VT_LinkDone, answer);
    } else if (ld8(call(fn::APIManager_getInstance, {}) + API_ERROR_SHOWN)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t copy = f.alloc(20);
        call(function_vb_copy, {copy, answer});
        const uint32_t fo = f.alloc(20);
        heapAnswer(fo, VT_LinkError, 0x20, copy);
        call(APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
        functionDestroy(copy);
    }
    return 0;
}
RW_FUNCTION(0x5A87DC, "OtherMenuDialog Facebook link answer operator()(bool) (sub_5A87DC)", other_menu_lambdas::linkUpdated);

// the link failed and its error dialog closed: unless retried, logged out of Facebook again
uint32_t linkErrorClosed(uint32_t functor) {
    if (call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) return 0;
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, functor + 8});
    const uint32_t fb = call(FacebookManager_getInstance, {});
    const uint32_t inner = f.alloc(20);
    functionCopy(inner, copy);
    const uint32_t fo = f.alloc(20);
    heapAnswer(fo, VT_LoggedOut, 0x20, inner);
    call(FacebookManager_logout, {fb, fo});
    functionDestroy(fo);
    functionDestroy(inner);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x5A8DA4, "OtherMenuDialog Facebook link error closed operator() (sub_5A8DA4)", other_menu_lambdas::linkErrorClosed);

// "unlink?" answered: yes - the link deleted on the server; no - the answer with false
uint32_t unlinkAnswer(uint32_t functor, uint32_t yesRef) {
    const uint32_t answer = functor + 8;
    if (!ld8(yesRef)) {
        if (ld32(functor + 0x18)) call(function_vb_call, {answer, 0});
        return 0;
    }
    const uint32_t dialog = ld32(functor + 0x20);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, answer});
    call(SM_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t fb = call(FacebookManager_getInstance, {});
    const uint32_t token = f.alloc(12);
    call(FacebookManager_getCurrentAccessToken, {token, fb});
    const uint32_t inner = f.alloc(20);
    functionCopy(inner, copy);
    const uint32_t fo = f.alloc(20);
    const uint32_t obj = heapAnswer(fo, VT_LinkDeleted, 0x28, inner);
    if (obj) st32(obj + 0x20, dialog);
    call(APIManager_executeUserLinkDelete, {api, strData(token), 1, fo});
    functionDestroy(fo);
    functionDestroy(inner);
    strFree(token);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x5A940C, "OtherMenuDialog Facebook unlink answer operator()(bool) (sub_5A940C)", other_menu_lambdas::unlinkAnswer);

// the link deleted: logged out of Facebook; failed: the error dialog
uint32_t linkDeleted(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    const uint32_t dialog = ld32(functor + 0x20);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(function_vb_copy, {copy, functor + 8});
    if (!ok) {
        if (ld8(call(fn::APIManager_getInstance, {}) + API_ERROR_SHOWN)) {
            const uint32_t api = call(fn::APIManager_getInstance, {});
            const uint32_t inner = f.alloc(20);
            functionCopy(inner, copy);
            const uint32_t fo = f.alloc(20);
            const uint32_t obj = heapAnswer(fo, VT_UnlinkError, 0x28, inner);
            if (obj) st32(obj + 0x20, dialog);
            call(APIManager_openErrorDialog, {api, fo, 1});
            functionDestroy(fo);
            functionDestroy(inner);
        }
    } else {
        const uint32_t fb = call(FacebookManager_getInstance, {});
        const uint32_t inner = f.alloc(0x1C);
        functionCopy(inner, copy);
        st32(inner + 0x18, dialog);
        const uint32_t fo = f.alloc(20);
        call(OtherMenu_logoutCallback, {fo, inner});
        call(FacebookManager_logout, {fb, fo});
        functionDestroy(fo);
        functionDestroy(inner);
    }
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x5A9794, "OtherMenuDialog Facebook link deleted operator()(bool) (sub_5A9794)", other_menu_lambdas::linkDeleted);

// logged out (bool, error, message): "unlinked" or the answer with false
uint32_t loggedOut(uint32_t functor, uint32_t okRef, uint32_t, uint32_t msg) {
    const MovedString m = takeString(msg);
    const uint32_t answer = functor + 8;
    if (ld8(okRef)) {
        call(OtherMenuDialog_updateFacebookButton, {ld32(functor + 0x20)});
        okDialogWithAnswer(SceneUtil_openOkDialog4Line, 104900009, VT_UnlinkDone, answer);
    } else if (ld32(functor + 0x18)) {
        call(function_vb_call, {answer, 0});
    }
    freeMoved(m);
    return 0;
}
RW_FUNCTION(0x5A9BEC, "OtherMenuDialog Facebook logged out operator() (sub_5A9BEC)", other_menu_lambdas::loggedOut);

// the unlink error dialog closed: unless retried, logged out anyway
uint32_t unlinkErrorClosed(uint32_t functor) {
    if (call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) return 0;
    const uint32_t fb = call(FacebookManager_getInstance, {});
    Frame f;
    const uint32_t copy = f.alloc(0x1C);
    call(function_vb_copy, {copy, functor + 8});
    st32(copy + 0x18, ld32(functor + 0x20));
    const uint32_t fo = f.alloc(20);
    call(OtherMenu_logoutCallback, {fo, copy});
    call(FacebookManager_logout, {fb, fo});
    functionDestroy(fo);
    functionDestroy(copy);
    return 0;
}
RW_FUNCTION(0x5AA150, "OtherMenuDialog Facebook unlink error closed operator() (sub_5AA150)", other_menu_lambdas::unlinkErrorClosed);

// the user code (data transfer) answer: a code issued - the user code popup; already issued / used - a notice
uint32_t userCodeAnswer(uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    const uint32_t dialog = ld32(functor + 4);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    if (ok) {
        if (ld8(api + API_USER_CODE_SET)) {
            if (ld8(api + API_USER_CODE_USED)) {
                const uint32_t td = textData(f, 104700019);
                const uint32_t s = newStr(f, textDataStr(td));
                const uint32_t none = newEmptyFunction(f);
                call(SceneUtil_openOkDialog4Line, {s, none, 0});
                functionDestroy(none);
                strFree(s);
                sharedRelease(td);
            } else {
                const uint32_t td = textData(f, 104700011);
                const uint32_t s = newStr(f, textDataStr(td));
                const uint32_t fo = newFunction(f, VT_UserCodeAgain, {dialog});
                call(SceneUtil_openOkDialog4Line, {s, fo, 0});
                functionDestroy(fo);
                strFree(s);
                sharedRelease(td);
            }
            return 0;
        }
        const uint32_t none = newEmptyFunction(f);
        call(OtherMenuDialog_openUserCodePopup, {dialog, none});
        functionDestroy(none);
    } else if (ld8(api + API_ERROR_SHOWN)) {
        const uint32_t api2 = call(fn::APIManager_getInstance, {});
        const uint32_t fo = newFunction(f, VT_UserCodeError, {});
        call(APIManager_openErrorDialog, {api2, fo, 1});
        functionDestroy(fo);
    }
    return 0;
}
RW_FUNCTION(0x5AA2D8, "OtherMenuDialog user code answer operator()(bool) (sub_5AA2D8)", other_menu_lambdas::userCodeAnswer);

uint32_t userCodeAgain(uint32_t functor) {
    Frame f;
    const uint32_t none = newEmptyFunction(f);
    call(OtherMenuDialog_openUserCodePopup, {ld32(functor + 4), none});
    functionDestroy(none);
    return 0;
}
RW_FUNCTION(0x5AA5D8, "OtherMenuDialog user code notice closed operator() (sub_5AA5D8)", other_menu_lambdas::userCodeAgain);

// the Facebook button: logged in - the token refreshed first; else the login / logout popup
uint32_t facebookTouched(uint32_t functor, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    const uint32_t dialog = ld32(functor + 0xC);
    call(AudioPlaySystemSEDecide, {});
    setTouch(ld32(functor + 4), 0);
    const uint32_t button = ld32(functor + 4);
    Frame f;
    if (call(SceneUtil_isLoggedInFacebook, {}) == 1) {
        const uint32_t fb = call(FacebookManager_getInstance, {});
        const uint32_t fo = f.alloc(20);
        st32(fo, VT_TokenRefreshed);
        st32(fo + 4, button);
        st32(fo + 0xC, dialog);
        st32(fo + 16, fo);
        call(FacebookManager_refreshCurrentAccessToken, {fb, fo});
        functionDestroy(fo);
    } else {
        const uint32_t fo = newFunction(f, VT_FacebookAnswer, {button, 0});
        call(OtherMenuDialog_openFacebookLoginLogout, {dialog, fo});
        functionDestroy(fo);
    }
    return 0;
}
RW_FUNCTION(0x5AA8C0, "OtherMenuDialog Facebook button touch operator() (sub_5AA8C0)", other_menu_lambdas::facebookTouched);

// a side menu popup closed (lambda {button}): the button back; on OK (when the scene may change) MyPage
uint32_t closedToMyPage(uint32_t lambda, uint32_t ok) {
    setTouch(ld32(lambda), 1);
    if (ok != 1) return 0;
    if (call(SM_canFinishScene, {sceneManager()}) != 1) return 0;
    {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        st8(sm + SM_RETRY_FLAG, 1);
        call(function_vb_assign, {sm + SM_RETRY_FN, none});
        functionDestroy(none);
    }
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(SceneUtil_callPrepareMypageAPIs, {none});
        functionDestroy(none);
    }
    Frame f;
    const uint32_t layerFn = newFunction(f, ld32(GOT_FUNC_LAYER_VT) + 8, {ld32(GOT_CREATE_MYPAGE)});
    call(SideMenu_changeScene, {layerFn});
    functionDestroy(layerFn);
    return 0;
}
RW_FUNCTION(0x5AAA04, "side menu popup closed to MyPage lambda operator()(bool) (sub_5AAA04)", other_menu_lambdas::closedToMyPage);

}  // namespace other_menu_lambdas
