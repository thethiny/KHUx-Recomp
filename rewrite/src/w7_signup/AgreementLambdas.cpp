// SceneAgreement's Facebook take-over chain (the Facebook button: login, then the device UUID moved to the
// Facebook-linked account; "could not log in" / errors; done or failed: back to the title with the responses
// cleared). The button is not shown in this build, so these stay unverified.
#include "../w6_signup/Signup.hpp"
#include "../scene_main/MainCommon.hpp"

using namespace scene;
using namespace signup;

namespace {
constexpr uint32_t APIManager_clearAllResponses = 0x4C3141, APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_executeSystemUuidUpdate = 0x4D4595, APIManager_openErrorDialog = 0x4C3C8D;
constexpr uint32_t FacebookManager_getInstance = 0x4E5585, FacebookManager_login = 0x4E5747;
constexpr uint32_t FacebookManager_getCurrentAccessToken = 0x4E57F1, FacebookManager_logout = 0x4E5751;
constexpr uint32_t SM_changeSceneLayer = 0x612B99, SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t GOT_FUNC_LAYER_VT = 0xFD6074, GOT_CREATE_TITLE = 0xFD68B4;
constexpr uint32_t VT_LoggedIn = 0xF6B328;        // {byte}
constexpr uint32_t VT_UuidMoved = 0xF6B354;       // {byte}
constexpr uint32_t VT_LoginFailed = 0xF6B404;     // {byte}
constexpr uint32_t VT_MovedOk = 0xF6B380;         // {byte}
constexpr uint32_t VT_MoveError = 0xF6B3AC;       // {}
constexpr uint32_t VT_LoggedOut = 0xF6B3D8;       // {}
constexpr uint32_t API_ERROR_SHOWN = 0x14;
constexpr uint32_t FB_CANCELLED = 3;

void okDialog(int32_t id, uint32_t vt) {
    Frame f;
    const uint32_t td = textData(f, id);
    const uint32_t s = newStr(f, textDataStr(td));
    const uint32_t fo = newFunction(f, vt, {0});
    call(SceneUtil_openOkDialog4Line, {s, fo, 0});
    functionDestroy(fo);
    strFree(s);
    sharedRelease(td);
}
}  // namespace

namespace agreement_lambdas {

// back to the title, every stored response dropped
uint32_t toTitle(uint32_t) {
    call(APIManager_clearAllResponses, {call(fn::APIManager_getInstance, {})});
    const uint32_t sm = sceneManager();
    Frame f;
    const uint32_t layerFn = newFunction(f, ld32(GOT_FUNC_LAYER_VT) + 8, {ld32(GOT_CREATE_TITLE)});
    call(SM_changeSceneLayer, {sm, layerFn, 1, 1, 1});
    functionDestroy(layerFn);
    return 0;
}
RW_FUNCTION(0x714190, "SceneAgreement Facebook chain to title operator() (sub_714190)", agreement_lambdas::toTitle);
RW_FUNCTION(0x7148B8, "SceneAgreement Facebook chain to title operator() (sub_7148B8)", agreement_lambdas::toTitle);
RW_FUNCTION(0x714BD0, "SceneAgreement Facebook chain to title operator() (sub_714BD0)", agreement_lambdas::toTitle);

// the Facebook button: log in
uint32_t facebookTouched(uint32_t, uint32_t, uint32_t typeRef) {
    if (ld32(typeRef) != 2) return 0;
    call(AudioPlaySystemSEDecide, {});
    const uint32_t fb = call(FacebookManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_LoggedIn, {0});
    call(FacebookManager_login, {fb, 0, fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x7142D8, "SceneAgreement Facebook button operator() (sub_7142D8)", agreement_lambdas::facebookTouched);

// logged in (bool, error, message): the UUID moved to the token's account; failed (not cancelled): a notice
uint32_t loggedIn(uint32_t, uint32_t okRef, uint32_t errRef, uint32_t msg) {
    const uint8_t tag = ld8(msg);
    const uint32_t data = ld32(msg + 8);
    const uint32_t err = ld32(errRef);
    st32(msg, 0); st32(msg + 4, 0); st32(msg + 8, 0);
    if (ld8(okRef)) {
        call(SM_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t fb = call(FacebookManager_getInstance, {});
        Frame f;
        const uint32_t token = f.alloc(12);
        call(FacebookManager_getCurrentAccessToken, {token, fb});
        const uint32_t fo = newFunction(f, VT_UuidMoved, {0});
        call(APIManager_executeSystemUuidUpdate, {api, strData(token), 1, fo});
        functionDestroy(fo);
        strFree(token);
    } else if (err != FB_CANCELLED) {
        okDialog(104900015, VT_LoginFailed);
    }
    if (tag & 1) call(fn::operator_delete, {data});
    return 0;
}
RW_FUNCTION(0x714414, "SceneAgreement Facebook login answer operator() (sub_714414)", agreement_lambdas::loggedIn);

// the UUID moved: "taken over" (then the title); failed: the error dialog
uint32_t uuidMoved(uint32_t, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    call(SM_enableLoadingIndicator, {sceneManager(), 0});
    if (ok) {
        okDialog(104900001, VT_MovedOk);
    } else if (ld8(call(fn::APIManager_getInstance, {}) + API_ERROR_SHOWN)) {
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t fo = newFunction(f, VT_MoveError, {});
        call(APIManager_openErrorDialog, {api, fo, 1});
        functionDestroy(fo);
    }
    return 0;
}
RW_FUNCTION(0x7146A8, "SceneAgreement Facebook UUID update answer operator()(bool) (sub_7146A8)", agreement_lambdas::uuidMoved);

// the error dialog closed: unless retried, logged out of Facebook
uint32_t moveErrorClosed(uint32_t) {
    if (call(APIManager_needsRetry, {call(fn::APIManager_getInstance, {})})) return 0;
    const uint32_t fb = call(FacebookManager_getInstance, {});
    Frame f;
    const uint32_t fo = newFunction(f, VT_LoggedOut, {});
    call(FacebookManager_logout, {fb, fo});
    functionDestroy(fo);
    return 0;
}
RW_FUNCTION(0x7149F4, "SceneAgreement Facebook UUID error closed operator() (sub_7149F4)", agreement_lambdas::moveErrorClosed);

}  // namespace agreement_lambdas
