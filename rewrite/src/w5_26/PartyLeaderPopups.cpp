// Party leader actions on another member's Player Information page: AvatarInfoDialog::openPopUpLeaderChange /
// SubLeaderAppoint / SubLeaderDismiss (an OK / Cancel dialog), their OK lambdas ($_47 / $_49 / $_51: the request,
// /party/leader/change 88, /party/admin/appoint 89, /party/admin/dismiss 90) and the request callbacks ($_46 / $_48 /
// $_50: the result dialog, or the error dialog). Also the member list's row: MemberScrollView::changeRow $_3 (the
// profile button) and ScenePartyMember::initMemberLayer $_4 (opens the profile) with its two refresh lambdas.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t FunctionVB_copy = 0x45C84D, FunctionVB_move = 0x47BC69, FunctionVB_call = 0x474139;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;   // (manager, bool)
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;          // (manager, float seconds)
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;     // (const string&, function<void(bool)>, a, b)
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;           // (const string&, function<void(bool)>, a)
constexpr uint32_t SceneUtil_openAvatarInfoPopup = 0x46A229;         // (id lo, id hi, bool, fn<void(bool)>, fn<bool()>)
constexpr uint32_t APIManager_needsRetry = 0x4C3C63;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;            // (api, const function<void(bool)>&, bool)
constexpr uint32_t APIManager_executePartyLeaderChange = 0x4D008D;   // (api, -, uint64 user, const function&)
constexpr uint32_t APIManager_executePartyAdminAppoint = 0x4D0239;
constexpr uint32_t APIManager_executePartyAdminDismiss = 0x4D03E5;
constexpr uint32_t APIManager_executePartyMemberList = 0x4C7AA5;     // (api, 0, const function&, bool)
constexpr uint32_t MemberIdMap_find = 0x490275;                      // unordered_map<uint64, int>::find(map, &key)
constexpr uint32_t SafeBuffer_read = 0x45A269;                       // (sb, dst, src, len, offset)
constexpr uint32_t ScenePartyMember_refreshMembers = 0x6F127D;       // sub_6F127C(&scene)
}  // namespace g

constexpr uint32_t D_USER_ID = 0x2C0;          // AvatarInfoDialog: the player shown (uint64)
constexpr uint32_t D_VIEWER_LEADER = 0x2C9;    // the viewer leads the party (leader change needs it)
constexpr uint32_t D_SUB_LEADER = 0x2CA;       // the player shown is a sub leader
constexpr uint32_t SM_PARTY_CHANGED = 0x309;   // SceneManager: the party changed (refresh on return)
constexpr uint32_t SM_POPUP_DELAY = 0x300;     // SceneManager: float, the popup open animation time
constexpr uint32_t API_OFFLINE = 0x14;         // APIManager: a request failed (byte)
constexpr uint32_t API_MEMBERS = 0x1FC;        // shared_ptr<SafeBuffer> the member list (0x128-byte records, +0x200)
constexpr uint32_t MEMBER = 0x128;
constexpr uint32_t V_IS_VISIBLE = 0xB4;
constexpr uint32_t TOUCH_ENDED = 2;

// one of the three popups
struct Action {
    int32_t ask, done;          // texts: the question, the success message
    uint32_t vtRequest;         // the OK lambda (heap: 0x40 with one dialog word, 0x48 with two)
    uint32_t size;
    uint32_t vtResult;          // the request's callback (heap 0x20 / 0x28)
    uint32_t vtError;           // the error dialog's callback (inline)
    uint32_t request;           // APIManager::execute...
    int32_t subLeader;          // the sub-leader flag the success sets (-1: none)
};
constexpr Action LEADER{103840007, 103840008, 0xF57C80, 0x40, 0xF57CAC, 0xF57CD8, g::APIManager_executePartyLeaderChange, -1};
constexpr Action APPOINT{103840001, 103840002, 0xF57BFC, 0x48, 0xF57C28, 0xF57C54, g::APIManager_executePartyAdminAppoint, 1};
constexpr Action DISMISS{103840003, 103840004, 0xF57B78, 0x48, 0xF57BA4, 0xF57BD0, g::APIManager_executePartyAdminDismiss, 0};
// the OK lambda's layout: +8 dialog, +0x10 the popup's callback copy; 0x40: +0x28 callback; 0x48: +0x28 dialog,
// +0x30 callback (the cancel / failure path)
constexpr uint32_t fnB(uint32_t size) { return size == 0x40 ? 0x28 : 0x30; }

void openPopup(const Action &a, uint32_t dialog, uint32_t onClosed) {
    call(g::AudioPlaySystemSEDecide, {});
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(g::FunctionVB_copy, {copy, onClosed});
    if (a.vtRequest != LEADER.vtRequest || ld8(dialog + D_VIEWER_LEADER)) {
        const uint32_t td = textData(f, a.ask);
        const uint32_t text = newStr(f, textDataStr(td));
        const uint32_t lambdaFn = f.alloc(20);           // the lambda's captured copy
        functionCopy(lambdaFn, copy);
        const uint32_t copyB = f.alloc(20);
        call(g::FunctionVB_copy, {copyB, onClosed});
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {a.size});
        if (obj) {
            st32(obj, a.vtRequest);
            st32(obj + 8, dialog);
            call(g::FunctionVB_move, {obj + 0x10, lambdaFn});
            if (a.size == 0x48) st32(obj + 0x28, dialog);
            st32(obj + 8, dialog);
            call(g::FunctionVB_move, {obj + fnB(a.size), copyB});
            st32(obj + 8, dialog);
        }
        st32(fo + 16, obj);
        call(g::SceneUtil_openOkCancelDialog4Line, {text, fo, 0, 0});
        functionDestroy(fo);
        functionDestroy(copyB);
        functionDestroy(lambdaFn);
        strFree(text);
        sharedRelease(td);
    }
    functionDestroy(copy);
}

// the OK lambda: cancelled -> the popup's callback(false); else the request with a result lambda
void request(const Action &a, uint32_t functor, uint32_t okRef) {
    if (!ld8(okRef)) {
        call(g::FunctionVB_call, {functor + fnB(a.size), 0});
        return;
    }
    const uint32_t dialog = ld32(functor + 8);
    call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    const uint32_t hi = ld32(dialog + D_USER_ID + 4), lo = ld32(dialog + D_USER_ID);
    Frame f;
    const uint32_t copy = f.alloc(20);
    call(g::FunctionVB_copy, {copy, functor + 0x10});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {a.size == 0x40 ? 0x20u : 0x28u});
    if (obj) {
        st32(obj, a.vtResult);
        call(g::FunctionVB_move, {obj + 8, copy});
        if (a.size == 0x48) st32(obj + 0x20, ld32(functor + 0x28));
    }
    st32(fo + 16, obj);
    call(a.request, {api, 0, lo, hi, fo});
    functionDestroy(fo);
    functionDestroy(copy);
}

// the request's callback {+8 the popup's callback, +0x20 dialog (appoint / dismiss)}
void result(const Action &a, uint32_t functor, uint32_t okRef) {
    const uint32_t ok = ld8(okRef);
    const uint32_t dialog = a.subLeader >= 0 ? ld32(functor + 0x20) : 0;
    call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
    if (a.subLeader < 0) st8(sceneManager() + SM_PARTY_CHANGED, 1);
    if (ok) {
        if (a.subLeader >= 0) {
            st8(dialog + D_SUB_LEADER, uint32_t(a.subLeader));
            st8(sceneManager() + SM_PARTY_CHANGED, 1);
        }
        Frame f;
        const uint32_t td = textData(f, a.done);
        const uint32_t text = newStr(f, textDataStr(td));
        const uint32_t copy = f.alloc(20);
        call(g::FunctionVB_copy, {copy, functor + 8});
        call(g::SceneUtil_openOkDialog4Line, {text, copy, 0});
        functionDestroy(copy);
        strFree(text);
        sharedRelease(td);
        return;
    }
    if (a.subLeader >= 0) st8(sceneManager() + SM_PARTY_CHANGED, 1);
    if (!ld8(call(fn::APIManager_getInstance, {}) + API_OFFLINE)) return;
    if (!call(g::APIManager_needsRetry, {call(fn::APIManager_getInstance, {})}) && ld32(functor + 8 + 16))
        call(g::FunctionVB_call, {functor + 8, 0});
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t cb = newFunction(f, a.vtError, {});
    call(g::APIManager_openErrorDialog, {api, cb, 1});
    functionDestroy(cb);
}

// the OK lambda's __clone() / destroy_deallocate()
uint32_t cloneRequest(const Action &a, uint32_t self) {
    const uint32_t obj = call(fn::operator_new, {a.size});
    if (obj) {
        st32(obj, a.vtRequest);
        st32(obj + 8, ld32(self + 8));
        call(g::FunctionVB_copy, {obj + 0x10, self + 0x10});
        if (a.size == 0x48) st32(obj + 0x28, ld32(self + 0x28));
        st32(obj + 8, ld32(self + 8));
        call(g::FunctionVB_copy, {obj + fnB(a.size), self + fnB(a.size)});
        st32(obj + 8, ld32(self + 8));
    }
    return obj;
}
void deleteRequest(const Action &a, uint32_t self) {
    functionDestroy(self + fnB(a.size));
    functionDestroy(self + 0x10);
    call(fn::operator_delete, {self});
}

struct PartyLeaderPopups {
    static void leaderChange(uint32_t d, uint32_t cb) { openPopup(LEADER, d, cb); }
    static void appoint(uint32_t d, uint32_t cb) { openPopup(APPOINT, d, cb); }
    static void dismiss(uint32_t d, uint32_t cb) { openPopup(DISMISS, d, cb); }
    static void leaderRequest(uint32_t f, uint32_t ok) { request(LEADER, f, ok); }
    static void appointRequest(uint32_t f, uint32_t ok) { request(APPOINT, f, ok); }
    static void dismissRequest(uint32_t f, uint32_t ok) { request(DISMISS, f, ok); }
    static void leaderResult(uint32_t f, uint32_t ok) { result(LEADER, f, ok); }
    static void appointResult(uint32_t f, uint32_t ok) { result(APPOINT, f, ok); }
    static void dismissResult(uint32_t f, uint32_t ok) { result(DISMISS, f, ok); }
    static uint32_t leaderClone(uint32_t s) { return cloneRequest(LEADER, s); }
    static uint32_t appointClone(uint32_t s) { return cloneRequest(APPOINT, s); }
    static uint32_t dismissClone(uint32_t s) { return cloneRequest(DISMISS, s); }
    static void leaderDelete(uint32_t s) { deleteRequest(LEADER, s); }
    static void appointDelete(uint32_t s) { deleteRequest(APPOINT, s); }
    static void dismissDelete(uint32_t s) { deleteRequest(DISMISS, s); }

    // MemberScrollView::changeRow $_3 {+8 button, +0xC view, +0x10 user id}: the profile button - remembers the
    // member and runs the view's callback after the popup delay
    static void memberProfileTouch(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        const uint32_t view = ld32(functor + 0xC), button = ld32(functor + 8);
        if (vcall(button, V_IS_VISIBLE, {button}) != 1 || vcall(view, V_IS_VISIBLE, {view}) != 1) return;
        call(g::AudioPlaySystemSEDecide, {});
        if (!ld32(view + 0x690)) return;
        const uint32_t lo = ld32(functor + 0x10), hi = ld32(functor + 0x14);
        st32(view + 0x6E4, hi);
        st32(view + 0x6E0, lo);
        call(g::SceneManager_openNoTapScreen, {sceneManager(), ld32(sceneManager() + SM_POPUP_DELAY)});
        const uint32_t wait = call(fn::DelayTime_create, {ld32(sceneManager() + SM_POPUP_DELAY)});
        Frame f;
        const uint32_t then = newFunction(f, 0xF56EC0, {view});
        const uint32_t done = call(fn::CallFunc_create, {then});
        call(fn::Node_runAction, {view, call(fn::Sequence_create, {wait, done, 0})});
        functionDestroy(then);
    }

    // ScenePartyMember::initMemberLayer $_4 {scene}(const uint64& user): that member's profile
    static void openMemberProfile(uint32_t functor, uint32_t userRef) {
        const uint32_t lo = ld32(userRef), hi = ld32(userRef + 4);
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t count = ld32(api + API_MEMBERS + 4), members = ld32(api + API_MEMBERS);
        if (count) call(fn::shared_add, {count});
        if (members) {
            Frame f;
            const uint32_t record = f.alloc(MEMBER);
            st32(record, lo);
            st32(record + 4, hi);
            const uint32_t node = call(g::MemberIdMap_find, {members + 0x24, record});
            const uint32_t off = node ? MEMBER * ld32(node + 0x10) : uint32_t(-int32_t(MEMBER));
            st8(record + 0x11F, 0);
            call(g::SafeBuffer_read, {members, record, ld32(members + 0xC) + off, MEMBER, off});
            const uint32_t closed = newFunction(f, 0xF69B9C, {ld32(functor + 4)});
            const uint32_t none = newEmptyFunction(f);
            call(g::SceneUtil_openAvatarInfoPopup, {ld32(record), ld32(record + 4), 1, closed, none});
            functionDestroy(none);
            functionDestroy(closed);
        }
        if (count) call(fn::shared_release, {count});
    }
    // its lambda(bool changed) {scene}: reload the member list
    static void memberProfileClosed(uint32_t functor, uint32_t okRef) {
        if (!ld8(okRef)) return;
        const uint32_t scene = ld32(functor + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t cb = newFunction(f, 0xF69BC8, {scene});
        call(g::APIManager_executePartyMemberList, {api, 0, cb, 1});
        functionDestroy(cb);
    }
    // that lambda's lambda(bool ok) {scene}: the list again, or the error dialog
    static void memberListReloaded(uint32_t functor, uint32_t okRef) {
        const uint32_t ok = ld8(okRef);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        if (ok) {
            call(g::ScenePartyMember_refreshMembers, {functor + 4});
            return;
        }
        if (!ld8(call(fn::APIManager_getInstance, {}) + API_OFFLINE)) return;
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t cb = newFunction(f, 0xF69BF4, {});
        call(g::APIManager_openErrorDialog, {api, cb, 1});
        functionDestroy(cb);
    }
};
}  // namespace

RW_FUNCTION(0x539CCC, "AvatarInfoDialog::openPopUpLeaderChange(function<void(bool)>)", PartyLeaderPopups::leaderChange);
RW_FUNCTION(0x539F34, "AvatarInfoDialog::openPopUpSubLeaderAppoint(function<void(bool)>)", PartyLeaderPopups::appoint);
RW_FUNCTION(0x53A18C, "AvatarInfoDialog::openPopUpSubLeaderDismiss(function<void(bool)>)", PartyLeaderPopups::dismiss);
RW_FUNCTION(0x541E7C, "AvatarInfoDialog::openPopUpLeaderChange::$_47 __clone()", PartyLeaderPopups::leaderClone);
RW_FUNCTION(0x541FD4, "AvatarInfoDialog::openPopUpLeaderChange::$_47 destroy_deallocate()", PartyLeaderPopups::leaderDelete);
RW_FUNCTION(0x54201C, "AvatarInfoDialog::openPopUpLeaderChange::$_47 operator()", PartyLeaderPopups::leaderRequest);
RW_FUNCTION(0x5422E0, "AvatarInfoDialog::openPopUpLeaderChange::$_46 operator()", PartyLeaderPopups::leaderResult);
RW_FUNCTION(0x5416A8, "AvatarInfoDialog::openPopUpSubLeaderAppoint::$_49 __clone()", PartyLeaderPopups::appointClone);
RW_FUNCTION(0x541808, "AvatarInfoDialog::openPopUpSubLeaderAppoint::$_49 destroy_deallocate()", PartyLeaderPopups::appointDelete);
RW_FUNCTION(0x541850, "AvatarInfoDialog::openPopUpSubLeaderAppoint::$_49 operator()", PartyLeaderPopups::appointRequest);
RW_FUNCTION(0x541B2C, "AvatarInfoDialog::openPopUpSubLeaderAppoint::$_48 operator()", PartyLeaderPopups::appointResult);
RW_FUNCTION(0x540ED0, "AvatarInfoDialog::openPopUpSubLeaderDismiss::$_51 __clone()", PartyLeaderPopups::dismissClone);
RW_FUNCTION(0x541030, "AvatarInfoDialog::openPopUpSubLeaderDismiss::$_51 destroy_deallocate()", PartyLeaderPopups::dismissDelete);
RW_FUNCTION(0x541078, "AvatarInfoDialog::openPopUpSubLeaderDismiss::$_51 operator()", PartyLeaderPopups::dismissRequest);
RW_FUNCTION(0x541354, "AvatarInfoDialog::openPopUpSubLeaderDismiss::$_50 operator()", PartyLeaderPopups::dismissResult);
RW_FUNCTION(0x524504, "MemberScrollView::changeRow::$_3 operator() (profile button)", PartyLeaderPopups::memberProfileTouch);
RW_FUNCTION(0x6F1D30, "ScenePartyMember::initMemberLayer::$_4 operator()(unsigned long long)", PartyLeaderPopups::openMemberProfile);
RW_FUNCTION(0x6F1F54, "ScenePartyMember::initMemberLayer::$_4::lambda(bool) operator()", PartyLeaderPopups::memberProfileClosed);
RW_FUNCTION(0x6F20A4, "ScenePartyMember::initMemberLayer::$_4::lambda(bool)::lambda(bool) operator()",
            PartyLeaderPopups::memberListReloaded);
