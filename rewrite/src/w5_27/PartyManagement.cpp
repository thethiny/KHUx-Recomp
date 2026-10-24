// Party > Manage (ScenePartyManagement), the leader's two lists: Invitations sent (runInJoinLayout $_11: "Cancel
// Invitation" -> /party/invite/cancel 95) and Requests received (runInJoinLayout $_13: "Delete" -> /party/applicant/
// reject 87; $_12: "Approve" -> /party/applicant/agree), and Leave Party. A row's touch copies the row's 56-byte
// record into a confirm lambda (OK -> the request lambda -> the result dialog -> the OK lambda that drops the row
// from the list). The confirm dialogs: openPopUpCheck (a plain
// OK / Cancel dialog) and openPopUpApproveCheck (the applicant's message on a party popup; under-13 players see a
// fixed text instead). Also init's $_9 (the tab button) and MemberScrollView::changeRow $_4 / $_6 (row buttons).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9, AudioPlaySystemSECancel = 0x5C0D81;
constexpr uint32_t SceneManager_enableLoadingIndicator = 0x60D169;   // (manager, bool)
constexpr uint32_t SceneManager_openNoTapScreen = 0x61FB25;          // (manager, float seconds)
constexpr uint32_t SceneManager_getPopupLayer = 0x613E69;
constexpr uint32_t SceneManager_openPopup = 0x61524D;                // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t SceneUtil_openOkCancelDialog4Line = 0x45C951;     // (const string&, function<void(bool)>, a, b)
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;           // (const string&, function<void(bool)>, a)
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;   // (node, const function<void()>&)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t APIManager_openErrorDialog = 0x4C3C8D;            // (api, const function<void(bool)>&, bool)
constexpr uint32_t APIManager_executePartyInviteCancel = 0x4D0CDD;   // (api, -, uint64 offer, const function&)
constexpr uint32_t APIManager_executePartyApplicantReject = 0x4CFA9D;
constexpr uint32_t FunctionVoidSwap = 0x4EE86D;                      // std::function<void()>::swap
constexpr uint32_t String_assignN = 0x48D811;                        // std::string::assign(const char*, size_t)
constexpr uint32_t StringInit = 0x47CD93;                            // std::string(const char*, size_t)
constexpr uint32_t TextData_get = 0x4928AD;                          // (shared_ptr out, type, id)
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t U64Vector_pushBackSlow = 0x4ECB6D;                // vector<unsigned long long>::__push_back_slow_path
constexpr uint32_t U64Vector_dtor = 0x468499;
constexpr uint32_t ULLScrollView_setItemIDs = 0x4FD231;              // BaseScrollView<unsigned long long>::setItemIDs
constexpr uint32_t ScenePartyManagement_changeLayout = 0x6E74CD;     // sub_6E74CC(&scene, int)
constexpr uint32_t ScenePartyManagement_openPopUpCheck = 0x6E8C05;
constexpr uint32_t ScenePartyManagement_openPopUpApproveCheck = 0x6E8D75;
constexpr uint32_t ApplicantLambda_build = 0x6ED3FD;                 // sub_6ED3FC(function out, lambda&&)
constexpr uint32_t OfferLambda_build = 0x6EABE5;                     // sub_6EABE4 (the same for a received offer)
constexpr uint32_t AcceptLambda_build = 0x6EBA51;                    // sub_6EBA50 (the accept row's confirm)
constexpr uint32_t APIManager_executePartyApplicationCancel = 0x4CF601;
constexpr uint32_t APIManager_executePartyOfferReject = 0x4D1179;
constexpr uint32_t APIManager_executePartyOfferAgree = 0x4D0FCD;     // (api, -, uint64 offer, const function&)
constexpr uint32_t APIManager_executePartyApplicantAgree = 0x4CF8F1;
constexpr uint32_t APIManager_executePartyLeave = 0x4CEF99;          // (api, const function<void(bool)>&)
constexpr uint32_t SceneManager_updateHeader = 0x611169;
constexpr uint32_t ApproveLambda_build = 0x6EE2A9;                   // sub_6EE2A8 (the approve row's confirm)
constexpr uint32_t IntVector_pushBackSlow = 0x4D6CC1, IntVector_dtor = 0x478961;
constexpr uint32_t IntScrollView_setItemIDs = 0x478929;              // BaseScrollView<int>::setItemIDs
constexpr uint32_t IInOutAnimation_runOutAnimation = 0x6F24B5;       // (this, function<void()>)
constexpr uint32_t FunctionVB_assign = 0x490109;                     // std::function<void(bool)>::operator=(function&&)
constexpr uint32_t ScenePartyTop_callPartyTopAPI = 0x652595;         // (function<void(bool)> by value)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;         // (sm, function<Layer*()>, bool, bool, bool)
constexpr uint32_t ClosePopup = 0x6E9C91;                            // sub_6E9C90({shadow, view, layer})
}  // namespace g

constexpr uint32_t S_Json = 0xDFE5DF, S_2Line = 0xDFE647, S_TxtComment = 0xDF9EF7, S_TxtCommentLabel = 0xDFE1CC,
                   S_ButtonClose = 0xDF8692, S_TxtOK = 0xDF869F, S_ButtonOK = 0xDF8688, S_TxtCancel = 0xDF86A6;
constexpr int32_t TXT_APPROVE = 103800044, TXT_OK = 100100001, TXT_CANCEL = 100100002;
constexpr int32_t TXT_INVITE_CANCEL_ASK = 103850002, TXT_INVITE_CANCELLED = 103850003;
constexpr int32_t TXT_REQUEST_DELETE_ASK = 103850009, TXT_REQUEST_DELETED = 103850010, TXT_U13 = 103810025;
constexpr int32_t TXT_APPLICATION_WITHDRAW_ASK = 103810009, TXT_APPLICATION_WITHDRAWN = 103810010;
constexpr int32_t TXT_OFFER_DELETE_ASK = 103810016, TXT_OFFER_DELETED = 103810017, TXT_OFFER_U13 = 103830014;
constexpr int32_t TXT_OFFER_ACCEPT_ASK = 103810011;
constexpr uint32_t G_SHADOW_FADE = 0xFD65C0;          // GOT: the shadow fade-in time (float)
constexpr uint32_t SM_POPUP_DELAY = 0x300;            // SceneManager: float, the popup open animation time
constexpr uint32_t API_OFFLINE = 0x14;
constexpr uint32_t SC_INVITES = 0x29C, SC_REQUESTS = 0x2A0;   // ScenePartyManagement: the join layout's lists
constexpr uint32_t SC_APPLICATIONS = 0x294, SC_OFFERS = 0x298;  // the entry layout's lists
constexpr uint32_t SC_ON_APPROVE = 0x2A8;             // std::function<void()> the approve popup's pending action
constexpr uint32_t LIST_IDS = 0x628;                  // BaseScrollView<uint64>: item ids
constexpr uint32_t RECORD = 0x38;                     // an offer / application row record (vector element)
constexpr uint32_t V_IS_VISIBLE = 0xB4, V_SET_TOUCH_ENABLED = 0x29C, V_GET_OPACITY = 0x20C, V_SET_OPACITY = 0x214;
constexpr uint32_t V_REMOVE_CHILD = 0x12C;
constexpr uint32_t TOUCH_ENDED = 2;

constexpr uint32_t VT_CheckClosed = 0xF69330;         // openPopUpCheck's dialog callback {ok fn, cancel fn} (0x38)
constexpr uint32_t VT_ApproveAction = 0xF69304;       // {shadow, view, layer, action} (0x30), parked at +0x2A8
constexpr uint32_t VT_ApproveOkTouch = 0xF692D8, VT_ApproveCloseTouch = 0xF692AC, VT_ApproveCanClose = 0xF69280;
constexpr uint32_t VT_ApproveClosed = 0xF691FC;       // openPopup's result {ok, close, scene, shadow, view, layer, cancel}
constexpr uint32_t VT_ApproveNext = 0xF69254;         // the delayed action {scene}
constexpr uint32_t VT_RemovePopup = 0xF69228;         // {layer, shadow, view}

// one of the two lists
struct List {
    uint32_t view;                                    // the list view in the scene
    uint32_t vtConfirm, vtRequest, vtResult, vtError; // the 0x50 confirm lambda, its request / result / error lambdas
    uint32_t request;
    int32_t done;
    bool intIds;                                      // the entry layout's lists hold int ids (the join layout's uint64)
};
constexpr List INVITES{SC_INVITES, 0xF697D4, 0xF69800, 0xF6982C, 0xF69858, g::APIManager_executePartyInviteCancel,
                       TXT_INVITE_CANCELLED, false};
constexpr List REQUESTS{SC_REQUESTS, 0xF6961C, 0xF69648, 0xF69674, 0xF696A0, g::APIManager_executePartyApplicantReject,
                        TXT_REQUEST_DELETED, false};
// runInEntryLayout (no party yet): the requests sent (withdraw -> /party/application/cancel 84) and the invitations
// received (Delete -> /party/offer/reject 98; Accept -> /party/offer/agree 97, PartyEntry below)
constexpr List APPLICATIONS{SC_APPLICATIONS, 0xF69540, 0xF6956C, 0xF69598, 0xF695C4,
                            g::APIManager_executePartyApplicationCancel, TXT_APPLICATION_WITHDRAWN, true};
constexpr List OFFERS{SC_OFFERS, 0xF69388, 0xF693B4, 0xF693E0, 0xF6940C, g::APIManager_executePartyOfferReject,
                      TXT_OFFER_DELETED, true};
// runInJoinLayout $_12: "Approve" a request received (/party/applicant/agree; the confirm built by sub_6EE2A8)
constexpr int32_t TXT_REQUEST_APPROVE_ASK = 103850004, TXT_REQUEST_APPROVED = 103850008;
constexpr List APPROVALS{SC_REQUESTS, 0, 0xF69724, 0xF69750, 0xF6977C, g::APIManager_executePartyApplicantAgree,
                         TXT_REQUEST_APPROVED, false};

// a row's touch: the confirm dialog of a list (OK / Cancel: openPopUpCheck; the party popup: openPopUpApproveCheck)
struct RowSpec {
    uint32_t view;
    bool intArg;                                      // the row id is an int (the high word its sign)
    int32_t ask, u13;
    uint32_t vtConfirm, build;                        // the confirm lambda; its std::function builder (approve rows)
};
constexpr RowSpec INVITE_ROW{SC_INVITES, false, TXT_INVITE_CANCEL_ASK, 0, 0xF697D4, 0};
constexpr RowSpec APPLICATION_ROW{SC_APPLICATIONS, true, TXT_APPLICATION_WITHDRAW_ASK, 0, 0xF69540, 0};
constexpr RowSpec REQUEST_ROW{SC_REQUESTS, false, TXT_REQUEST_DELETE_ASK, TXT_U13, 0xF6961C, g::ApplicantLambda_build};
constexpr RowSpec OFFER_ROW{SC_OFFERS, true, TXT_OFFER_DELETE_ASK, TXT_OFFER_U13, 0xF69388, g::OfferLambda_build};
constexpr RowSpec APPROVE_ROW{SC_REQUESTS, false, TXT_REQUEST_APPROVE_ASK, TXT_U13, 0, g::ApproveLambda_build};

// TextData `id` into the shared_ptr at `td` and its text into the std::string at `text`
void textInto(uint32_t td, uint32_t text, int32_t id) {
    call(g::TextData_get, {td, 0, uint32_t(id)});
    const uint32_t c = ld32(ld32(td) + 4);
    call(g::StringInit, {text, c, call(g::STRLEN, {c})});
}

// std::string copy constructor as compiled inline
void stringCopy(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) {
        call(g::StringInit, {dst, ld32(src + 8), ld32(src + 4)});
    } else {
        for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
    }
}

// The row lambda {scene, -, row id (uint64), record: 4 words, string, 4 words, 3 words} (0x48 bytes) copied:
// `to` from `from` (the second word is never set: the caller's stale stack word travels along).
void copyRowLambda(uint32_t to, uint32_t from) {
    for (uint32_t k = 0; k < 0x20; k += 4) st32(to + k, ld32(from + k));
    stringCopy(to + 0x20, from + 0x20);
    for (uint32_t k = 0x2C; k < 0x48; k += 4) st32(to + k, ld32(from + k));
}
// the record (0x38) into a row lambda (at +0x10)
void recordIntoLambda(uint32_t l, uint32_t record) {
    for (uint32_t k = 0; k < 0x10; k += 4) st32(l + 0x10 + k, ld32(record + k));
    stringCopy(l + 0x20, record + 0x10);
    for (uint32_t k = 0x1C; k < 0x38; k += 4) st32(l + 0x10 + k, ld32(record + k));
}
// a row lambda moved into its heap __func (0x50: the lambda at +8, its string moved)
uint32_t heapRowLambda(uint32_t vt, uint32_t l) {
    const uint32_t obj = call(fn::operator_new, {0x50});
    if (obj) {
        st32(obj, vt);
        for (uint32_t k = 0; k < 0x20; k += 4) st32(obj + 8 + k, ld32(l + k));
        for (uint32_t k = 0; k < 12; k += 4) st32(obj + 0x28 + k, ld32(l + 0x20 + k));
        st32(l + 0x20, 0);
        st32(l + 0x24, 0);
        st32(l + 0x28, 0);
        for (uint32_t k = 0x2C; k < 0x48; k += 4) st32(obj + 8 + k, ld32(l + k));
    }
    return obj;
}

// the row's record, after the list view's visibility check; 0 when the list is not shown
uint32_t rowRecord(uint32_t functor, uint32_t view, uint32_t index) {
    const uint32_t list = ld32(ld32(functor + 4) + view);
    if (vcall(list, V_IS_VISIBLE, {list}) != 1) return 0;
    const uint32_t begin = ld32(functor + 8);
    if (((ld32(functor + 0xC) - begin) >> 3) * 0xB6DB6DB7u <= index) call(g::ThrowOutOfRange, {functor + 8});
    return begin + RECORD * index;
}

struct PartyManagement {
    // runInJoinLayout $_11 {scene, vector<Offer>}(uint64 row) / runInEntryLayout $_14 (int row): "Cancel
    // Invitation" / "Withdraw Request" - the OK / Cancel dialog
    static void simpleRowTouch(const RowSpec &r, uint32_t functor, uint32_t rowRef) {
        guest::Frame f(0x160);                         // the original frame: the lambda's second word is stale
        const uint32_t index = ld32(rowRef), hi = r.intArg ? uint32_t(int32_t(index) >> 31) : ld32(rowRef + 4);
        const uint32_t record = rowRecord(functor, r.view, index);
        if (!record) return;
        const uint32_t t1 = f.addr(0xD8), l1 = f.addr(0x90), l2 = f.addr(0x30);
        for (uint32_t k = 0; k < 0x10; k += 4) st32(t1 + k, ld32(record + k));
        stringCopy(t1 + 0x10, record + 0x10);
        for (uint32_t k = 0x1C; k < 0x38; k += 4) st32(t1 + k, ld32(record + k));
        st32(l1, ld32(functor + 4));
        st32(l1 + 0xC, hi);
        st32(l1 + 8, index);
        recordIntoLambda(l1, t1);
        const uint32_t td = f.addr(0x7C), text = f.addr(0x84);
        textInto(td, text, r.ask);
        copyRowLambda(l2, l1);
        const uint32_t ok = f.addr(0x128), cancel = f.addr(0x110);
        st32(ok + 16, 0);
        st32(ok + 16, heapRowLambda(r.vtConfirm, l2));
        st32(cancel + 16, 0);
        call(g::ScenePartyManagement_openPopUpCheck, {0, text, ok, cancel});
        functionDestroy(cancel);
        functionDestroy(ok);
        strFree(l2 + 0x20);
        strFree(text);
        sharedRelease(td);
        strFree(l1 + 0x20);
        strFree(t1 + 0x10);
    }

    // runInJoinLayout $_13 {scene, vector<Application>, -, -, payment info}(uint64 row) / runInEntryLayout $_16
    // (int row): "Delete" - the approve popup with the record's message (under 13: a fixed text)
    static void approveRowTouch(const RowSpec &r, uint32_t functor, uint32_t rowRef) {
        guest::Frame f(0x220);
        const uint32_t index = ld32(rowRef), hi = r.intArg ? uint32_t(int32_t(index) >> 31) : ld32(rowRef + 4);
        const uint32_t scene = ld32(functor + 4);
        const uint32_t record = rowRecord(functor, r.view, index);
        if (!record) return;
        const uint32_t t1 = f.addr(0x160), l1 = f.addr(0x118);
        for (uint32_t k = 0; k < 0x10; k += 4) st32(t1 + k, ld32(record + k));
        stringCopy(t1 + 0x10, record + 0x10);
        for (uint32_t k = 0x1C; k < 0x38; k += 4) st32(t1 + k, ld32(record + k));
        st32(l1, scene);
        st32(l1 + 8, index);
        st32(l1 + 0xC, hi);
        recordIntoLambda(l1, t1);
        const uint32_t payment = ld32(functor + 0x14);
        bool child = false;
        if (payment) {
            const uint32_t age = f.addr(0x19C);
            call(g::SafeBuffer_read, {payment, age, ld32(payment + 0xC), 4, 0});
            child = int32_t(ld32(age)) <= 12;
        }
        if (child) {
            const uint32_t td1 = f.addr(0x104), text1 = f.addr(0x10C), td2 = f.addr(0xF0), text2 = f.addr(0xF8);
            textInto(td1, text1, r.ask);
            textInto(td2, text2, r.u13);
            const uint32_t l3 = f.addr(0xA8);
            copyRowLambda(l3, l1);
            const uint32_t ok = f.addr(0x1B8), cancel = f.addr(0x1A0);
            call(r.build, {ok, l3});
            st32(cancel + 16, 0);
            call(g::ScenePartyManagement_openPopUpApproveCheck, {scene, text1, text2, ok, cancel});
            functionDestroy(cancel);
            functionDestroy(ok);
            strFree(l3 + 0x20);
            strFree(text2);
            sharedRelease(td2);
            strFree(text1);
            sharedRelease(td1);
        } else {
            const uint32_t td = f.addr(0x94), text = f.addr(0x9C);
            textInto(td, text, r.ask);
            const uint32_t l3 = f.addr(0x48);
            copyRowLambda(l3, l1);
            const uint32_t ok = f.addr(0x1B8), cancel = f.addr(0x1A0);
            call(r.build, {ok, l3});
            st32(cancel + 16, 0);
            call(g::ScenePartyManagement_openPopUpApproveCheck, {scene, text, t1 + 0x10, ok, cancel});
            functionDestroy(cancel);
            functionDestroy(ok);
            strFree(l3 + 0x20);
            strFree(text);
            sharedRelease(td);
        }
        strFree(l1 + 0x20);
        strFree(t1 + 0x10);
    }
    // sub_6ED3FC / sub_6EABE4: the confirm std::function of an approve row (heap 0x50, the lambda moved in)
    static uint32_t buildConfirm(uint32_t vt, uint32_t out, uint32_t lambda) {
        st32(out + 16, 0);
        st32(out + 16, heapRowLambda(vt, lambda));
        return out;
    }
    static uint32_t buildRequestConfirm(uint32_t out, uint32_t l) { return buildConfirm(REQUESTS.vtConfirm, out, l); }
    static uint32_t buildOfferConfirm(uint32_t out, uint32_t l) { return buildConfirm(OFFERS.vtConfirm, out, l); }
    static void inviteRowTouch(uint32_t f, uint32_t row) { simpleRowTouch(INVITE_ROW, f, row); }
    static void applicationRowTouch(uint32_t f, uint32_t row) { simpleRowTouch(APPLICATION_ROW, f, row); }
    static void requestRowTouch(uint32_t f, uint32_t row) { approveRowTouch(REQUEST_ROW, f, row); }
    static void offerRowTouch(uint32_t f, uint32_t row) { approveRowTouch(OFFER_ROW, f, row); }

    // the confirm lambda's __clone() (0x50: the row lambda copied)
    static uint32_t cloneConfirm(const List &l, uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x50});
        if (obj) {
            st32(obj, l.vtConfirm);
            copyRowLambda(obj + 8, self + 8);
        }
        return obj;
    }
    // operator(): the request with a result lambda {scene, 0, row id}
    static void confirmed(const List &l, uint32_t functor) {
        const uint32_t index = ld32(functor + 0x10), hi = ld32(functor + 0x14), scene = ld32(functor + 8);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t lo = ld32(functor + 0x48), idHi = ld32(functor + 0x4C);
        Frame f;
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x18});
        if (obj) {
            st32(obj, l.vtRequest);
            st32(obj + 8, scene);
            st32(obj + 0xC, 0);
            st32(obj + 0x10, index);
            st32(obj + 0x14, hi);
        }
        st32(fo + 16, obj);
        call(l.request, {api, 0, lo, idHi, fo});
        functionDestroy(fo);
    }
    // the request's lambda(bool ok) {scene, 0, row id}: the result dialog (its OK drops the row), or the error
    static void requestDone(const List &l, uint32_t functor, uint32_t okRef) {
        const uint32_t ok = ld8(okRef), scene = ld32(functor + 8);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        Frame f;
        if (ok) {
            const uint32_t td = textData(f, l.done);
            const uint32_t text = newStr(f, textDataStr(td));
            const uint32_t index = ld32(functor + 0x10), hi = ld32(functor + 0x14);
            const uint32_t fo = f.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x18});
            if (obj) {
                st32(obj, l.vtResult);
                st32(obj + 8, scene);
                st32(obj + 0xC, 0);
                st32(obj + 0x10, index);
                st32(obj + 0x14, hi);
            }
            st32(fo + 16, obj);
            call(g::SceneUtil_openOkDialog4Line, {text, fo, 0});
            functionDestroy(fo);
            strFree(text);
            sharedRelease(td);
            return;
        }
        if (!ld8(call(fn::APIManager_getInstance, {}) + API_OFFLINE)) return;
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t cb = newFunction(f, l.vtError, {});
        call(g::APIManager_openErrorDialog, {api, cb, 1});
        functionDestroy(cb);
    }
    // the result dialog's lambda(bool) {scene, 0, row id}: the list without that row
    static void dropRow(const List &l, uint32_t functor, uint32_t okRef) {
        if (!ld8(okRef)) return;
        Frame f;
        const uint32_t ids = f.alloc(12);
        st32(ids, 0);
        st32(ids + 4, 0);
        st32(ids + 8, 0);
        const uint32_t listRef = ld32(functor + 8) + l.view;
        const uint32_t lo = ld32(functor + 0x10), hi = ld32(functor + 0x14);
        uint32_t list = ld32(listRef);
        const uint32_t size = l.intIds ? 4 : 8;
        for (uint32_t k = 0; k < ((ld32(list + LIST_IDS + 4) - ld32(list + LIST_IDS)) / size); k++) {
            const uint32_t id = ld32(list + LIST_IDS) + size * k;
            const uint32_t idHi = l.intIds ? uint32_t(int32_t(ld32(id)) >> 31) : ld32(id + 4);
            if (ld32(id) != lo || idHi != hi) {
                const uint32_t end = ld32(ids + 4);
                if (end == ld32(ids + 8)) {
                    call(l.intIds ? g::IntVector_pushBackSlow : g::U64Vector_pushBackSlow, {ids, id});
                } else {
                    uint32_t next = 0;
                    if (end) {
                        st32(end, ld32(id));
                        if (!l.intIds) st32(end + 4, ld32(id + 4));
                        next = ld32(ids + 4);
                    }
                    st32(ids + 4, next + size);
                }
            }
            list = ld32(listRef);
        }
        call(l.intIds ? g::IntScrollView_setItemIDs : g::ULLScrollView_setItemIDs, {list, ids});
        call(l.intIds ? g::IntVector_dtor : g::U64Vector_dtor, {ids});
    }

    static uint32_t inviteClone(uint32_t s) { return cloneConfirm(INVITES, s); }
    static uint32_t requestClone(uint32_t s) { return cloneConfirm(REQUESTS, s); }
    static void inviteConfirmed(uint32_t f) { confirmed(INVITES, f); }
    static void requestConfirmed(uint32_t f) { confirmed(REQUESTS, f); }
    static void inviteDone(uint32_t f, uint32_t ok) { requestDone(INVITES, f, ok); }
    static void requestDoneR(uint32_t f, uint32_t ok) { requestDone(REQUESTS, f, ok); }
    static void inviteDrop(uint32_t f, uint32_t ok) { dropRow(INVITES, f, ok); }
    static void requestDrop(uint32_t f, uint32_t ok) { dropRow(REQUESTS, f, ok); }
    static uint32_t applicationClone(uint32_t s) { return cloneConfirm(APPLICATIONS, s); }
    static uint32_t offerClone(uint32_t s) { return cloneConfirm(OFFERS, s); }
    static void applicationConfirmed(uint32_t f) { confirmed(APPLICATIONS, f); }
    static void offerConfirmed(uint32_t f) { confirmed(OFFERS, f); }
    static void applicationDone(uint32_t f, uint32_t ok) { requestDone(APPLICATIONS, f, ok); }
    static void offerDone(uint32_t f, uint32_t ok) { requestDone(OFFERS, f, ok); }
    static void applicationDrop(uint32_t f, uint32_t ok) { dropRow(APPLICATIONS, f, ok); }
    static void offerDrop(uint32_t f, uint32_t ok) { dropRow(OFFERS, f, ok); }
    static void approveRowTouchR(uint32_t f, uint32_t row) { approveRowTouch(APPROVE_ROW, f, row); }
    static void approveConfirmed(uint32_t f) { confirmed(APPROVALS, f); }
    static void approveDone(uint32_t f, uint32_t ok) { requestDone(APPROVALS, f, ok); }
    static void approveDrop(uint32_t f, uint32_t ok) { dropRow(APPROVALS, f, ok); }

    // ScenePartyManagement::openPopUpCheck(text, ok, cancel): an OK / Cancel dialog running one of the two
    static void openPopUpCheck(uint32_t /*self*/, uint32_t text, uint32_t ok, uint32_t cancel) {
        Frame f;
        const uint32_t a = f.alloc(20), b = f.alloc(20);
        call(fn::function_vv_copy, {a, ok});
        call(fn::function_vv_copy, {b, cancel});
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x38});
        if (obj) {
            st32(obj, VT_CheckClosed);
            call(fn::function_vv_move, {obj + 8, a});
            call(fn::function_vv_move, {obj + 0x20, b});
        }
        st32(fo + 16, obj);
        call(g::SceneUtil_openOkCancelDialog4Line, {text, fo, 0, 0});
        functionDestroy(fo);
        functionDestroy(b);
        functionDestroy(a);
    }
    // its dialog callback's __clone() (sub_6EA338)
    static uint32_t checkClosedClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x38});
        if (obj) {
            st32(obj, VT_CheckClosed);
            call(fn::function_vv_copy, {obj + 8, self + 8});
            call(fn::function_vv_copy, {obj + 0x20, self + 0x20});
        }
        return obj;
    }

    // ScenePartyManagement::openPopUpApproveCheck(text, message, action, cancel): a party popup (OK / Cancel)
    static void openPopUpApproveCheck(uint32_t scene, uint32_t text, uint32_t message, uint32_t action,
                                      uint32_t cancel) {
        call(fn::AudioPlaySystemSEPopUp, {});
        const uint32_t layer = call(g::SceneManager_getPopupLayer, {sceneManager()});
        const uint32_t shadow = SceneUtil::createShadowSprite();
        const uint32_t opacity = vcall(shadow, V_GET_OPACITY, {shadow});
        vcall(shadow, V_SET_OPACITY, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {ld32(ld32(G_SHADOW_FADE)), opacity})});
        vcall(layer, node::addChild, {layer, shadow});
        Frame f;
        const uint32_t json = f.alloc(12);
        st32(json, 0);
        st32(json + 4, 0);
        st32(json + 8, 0);
        call(g::String_assignN, {json, S_Json, 0x31});
        const uint32_t reader = call(fn::GUIReader_getInstance, {});
        const uint32_t view = call(fn::GUIReader_widgetFromJsonFile, {reader, strData(json)});
        vcall(shadow, node::addChild, {shadow, view});
        {
            const uint32_t anchor = f.alloc(8);
            call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
            vcall(view, node::setAnchorPoint, {view, anchor});
            call(g::Vec2_dtor, {anchor});
        }
        vcall(view, node::setPosition, {view, vcall(shadow, node::getPosition, {shadow})});
        const uint32_t line = findNode(view, S_2Line, 0xB);
        SceneUtil::setShadow(line);
        const uint32_t comment = findNode(view, S_TxtComment, 0xB);
        SceneUtil::setShadow(comment);
        setTextC(line, strData(text));
        setTextId(comment, TXT_APPROVE);
        const uint32_t label = findNode(view, S_TxtCommentLabel, 0x11);
        SceneUtil::setShadow(label);
        setTextC(label, strData(message));
        {
            // the pending action {shadow, view, layer, action}, parked in the scene
            const uint32_t a = f.alloc(20), b = f.alloc(20);
            call(fn::function_vv_copy, {a, action});
            call(fn::function_vv_move, {b, a});
            const uint32_t fo = f.alloc(20);
            st32(fo + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x30});
            if (obj) {
                st32(obj, VT_ApproveAction);
                st32(obj + 8, shadow);
                st32(obj + 0xC, view);
                st32(obj + 0x10, layer);
                call(fn::function_vv_move, {obj + 0x18, b});
                st32(obj + 8, shadow);
                st32(obj + 0xC, view);
                st32(obj + 0x10, layer);
            }
            st32(fo + 16, obj);
            call(g::FunctionVoidSwap, {fo, scene + SC_ON_APPROVE});
            functionDestroy(fo);
            functionDestroy(b);
            functionDestroy(a);
        }
        const uint32_t close = findNode(view, S_ButtonClose, 0xC);
        {
            const uint32_t t = findNode(view, S_TxtOK, 6);
            setTextId(t, TXT_OK);
            SceneUtil::setShadow(t);
        }
        const uint32_t okButton = findNode(view, S_ButtonOK, 9);
        addTouch(okButton, VT_ApproveOkTouch, {});
        {
            const uint32_t t = findNode(view, S_TxtCancel, 0xA);
            setTextId(t, TXT_CANCEL);
            SceneUtil::setShadow(t);
        }
        addTouch(close, VT_ApproveCloseTouch, {});
        // the popup's result {ok, close, scene, shadow, view, layer, cancel}
        const uint32_t c = f.alloc(20);
        call(fn::function_vv_copy, {c, cancel});
        {
            const uint32_t sm = sceneManager();
            Frame t;
            const uint32_t canClose = newFunction(t, VT_ApproveCanClose, {close});
            const uint32_t copy = t.alloc(20);
            functionCopy(copy, c);
            const uint32_t closed = t.alloc(20);
            st32(closed + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x38});
            if (obj) {
                st32(obj, VT_ApproveClosed);
                const uint32_t caps[6] = {okButton, close, scene, shadow, view, layer};
                for (uint32_t k = 0; k < 6; k++) st32(obj + 8 + 4 * k, caps[k]);
                call(fn::function_vv_move, {obj + 0x20, copy});
            }
            st32(closed + 16, obj);
            call(g::SceneManager_openPopup, {sm, canClose, closed, 1});
            functionDestroy(closed);
            functionDestroy(copy);
            functionDestroy(canClose);
        }
        {
            Frame t;
            const uint32_t none = newEmptyFunction(t);
            const uint32_t anim = SceneUtil::createOpenPopupAnimation(view, none);
            call(fn::Node_runAction, {view, call(g::EaseSineOut_create, {anim})});
            functionDestroy(none);
        }
        functionDestroy(c);
        strFree(json);
    }
    // its popup result (bool ok) {ok, close, scene, shadow, view, layer, cancel}: the buttons off, then the parked
    // action after the popup delay, or the close animation and `cancel`
    static void approveClosed(uint32_t functor, uint32_t okRef) {
        const uint32_t ok = ld8(okRef), scene = ld32(functor + 0x10);
        const uint32_t okButton = ld32(functor + 8), close = ld32(functor + 0xC);
        vcall(okButton, V_SET_TOUCH_ENABLED, {okButton, 0});
        vcall(close, V_SET_TOUCH_ENABLED, {close, 0});
        if (ok) {
            call(g::AudioPlaySystemSEDecide, {});
            call(g::SceneManager_openNoTapScreen, {sceneManager(), ld32(sceneManager() + SM_POPUP_DELAY)});
            const uint32_t wait = call(fn::DelayTime_create, {ld32(sceneManager() + SM_POPUP_DELAY)});
            Frame f;
            const uint32_t then = newFunction(f, VT_ApproveNext, {scene});
            const uint32_t done = call(fn::CallFunc_create, {then});
            call(fn::Node_runAction, {scene, call(fn::Sequence_create, {wait, done, 0})});
            functionDestroy(then);
            return;
        }
        call(g::AudioPlaySystemSECancel, {});
        call(g::ClosePopup, {functor + 0x14});
        if (ld32(functor + 0x30)) call(fn::function_vv_call, {functor + 0x20});
    }
    // sub_6E9C90 {shadow, view, layer}: the shadow fades out, the eased close animation then removes both
    static void closePopup(uint32_t closure) {
        const uint32_t shadow = ld32(closure), view = ld32(closure + 4), layer = ld32(closure + 8);
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {ld32(ld32(G_SHADOW_FADE)), 0})});
        Frame f;
        const uint32_t removeFn = newFunction(f, VT_RemovePopup, {layer, shadow, view});
        const uint32_t anim = call(g::SceneUtil_createClosePopupAnimation, {view, removeFn});
        call(fn::Node_runAction, {view, call(g::EaseSineOut_create, {anim})});
        functionDestroy(removeFn);
    }
    // that animation's end {layer, shadow, view}
    static void removePopup(uint32_t functor) {
        const uint32_t layer = ld32(functor + 4);
        vcall(layer, V_REMOVE_CHILD, {layer, ld32(functor + 8), 1});
        vcall(layer, V_REMOVE_CHILD, {layer, ld32(functor + 0xC), 1});
    }

    // init $_9 {scene}: the tab button
    static void tabTouch(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        call(g::AudioPlaySystemSEDecide, {});
        call(g::ScenePartyManagement_changeLayout, {functor + 4, 0});
    }

    // MemberScrollView::changeRow $_4 / $_6 {+8 button, +0xC view, +0x10 user id}: a row button - remembers the
    // member and runs the view's callback (+0x6A8 / +0x6D8) after the popup delay
    static void rowButton(uint32_t functor, uint32_t typeRef, uint32_t callbackSlot, uint32_t vtThen) {
        if (ld32(typeRef) != TOUCH_ENDED) return;
        const uint32_t view = ld32(functor + 0xC), button = ld32(functor + 8);
        if (vcall(button, V_IS_VISIBLE, {button}) != 1 || vcall(view, V_IS_VISIBLE, {view}) != 1) return;
        call(g::AudioPlaySystemSEDecide, {});
        if (!ld32(view + callbackSlot)) return;
        const uint32_t lo = ld32(functor + 0x10), hi = ld32(functor + 0x14);
        st32(view + 0x6E4, hi);
        st32(view + 0x6E0, lo);
        call(g::SceneManager_openNoTapScreen, {sceneManager(), ld32(sceneManager() + SM_POPUP_DELAY)});
        const uint32_t wait = call(fn::DelayTime_create, {ld32(sceneManager() + SM_POPUP_DELAY)});
        Frame f;
        const uint32_t then = newFunction(f, vtThen, {view});
        const uint32_t done = call(fn::CallFunc_create, {then});
        call(fn::Node_runAction, {view, call(fn::Sequence_create, {wait, done, 0})});
        functionDestroy(then);
    }
    static void rowButton4(uint32_t f, uint32_t, uint32_t t) { rowButton(f, t, 0x6A8, 0xF56E68); }
    static void rowButton6(uint32_t f, uint32_t, uint32_t t) { rowButton(f, t, 0x6D8, 0xF56DB8); }
};

// runInEntryLayout $_15 (int row): "Accept" an invitation. Its lambda is {scene, -, record words 0-3, string,
// 7 words} (0x40; the second word is never set and never moved on), heap 0x48 without that word.
constexpr uint32_t VT_AcceptConfirm = 0xF69464, VT_AcceptRequest = 0xF69490, VT_AcceptLeft = 0xF694BC,
                   VT_AcceptError = 0xF694E8;
constexpr uint32_t SC_IN_OUT = 0x288;                 // ScenePartyManagement: IInOutAnimation base
constexpr uint32_t SM_PARTY_RELOAD = 0x4CC;           // SceneManager: byte, party data to reload
constexpr uint32_t SM_ON_PARTY = 0x548;               // SceneManager: std::function<void(bool)>
constexpr uint32_t G_PARTY_TOP_LAYER = 0xFD6620;      // GOT: createSceneLayer<ScenePartyTop>
constexpr uint32_t G_FN_LAYER_VT = 0xFD6074;          // GOT: the __func<Layer*(*)()> vtable (_ZTV)

void copyAcceptLambda(uint32_t to, uint32_t from, bool second) {
    st32(to, ld32(from));
    if (second) st32(to + 4, ld32(from + 4));
    for (uint32_t k = 8; k < 0x18; k += 4) st32(to + k, ld32(from + k));
    stringCopy(to + 0x18, from + 0x18);
    for (uint32_t k = 0x24; k < 0x40; k += 4) st32(to + k, ld32(from + k));
}

struct PartyEntry {
    static void acceptRowTouch(uint32_t functor, uint32_t rowRef) {
        guest::Frame f(0x208);
        const uint32_t index = ld32(rowRef);
        const uint32_t scene = ld32(functor + 4);
        const uint32_t record = rowRecord(functor, SC_OFFERS, index);
        if (!record) return;
        const uint32_t t1 = f.addr(0x14C), l1 = f.addr(0x10C);
        for (uint32_t k = 0; k < 0x10; k += 4) st32(t1 + k, ld32(record + k));
        stringCopy(t1 + 0x10, record + 0x10);
        for (uint32_t k = 0x1C; k < 0x38; k += 4) st32(t1 + k, ld32(record + k));
        st32(l1, scene);
        for (uint32_t k = 0; k < 0x10; k += 4) st32(l1 + 8 + k, ld32(t1 + k));
        stringCopy(l1 + 0x18, t1 + 0x10);
        for (uint32_t k = 0x1C; k < 0x38; k += 4) st32(l1 + 0x24 - 0x1C + k, ld32(t1 + k));
        const uint32_t payment = ld32(functor + 0x14);
        bool child = false;
        if (payment) {
            const uint32_t age = f.addr(0x188);
            call(g::SafeBuffer_read, {payment, age, ld32(payment + 0xC), 4, 0});
            child = int32_t(ld32(age)) <= 12;
        }
        if (child) {
            const uint32_t td1 = f.addr(0xF8), text1 = f.addr(0x100), td2 = f.addr(0xE4), text2 = f.addr(0xEC);
            textInto(td1, text1, TXT_OFFER_ACCEPT_ASK);
            textInto(td2, text2, TXT_OFFER_U13);
            const uint32_t l3 = f.addr(0xA4);
            copyAcceptLambda(l3, l1, false);
            const uint32_t ok = f.addr(0x1D4), cancel = f.addr(0x1BC);
            call(g::AcceptLambda_build, {ok, l3});
            st32(cancel + 16, 0);
            call(g::ScenePartyManagement_openPopUpApproveCheck, {scene, text1, text2, ok, cancel});
            functionDestroy(cancel);
            functionDestroy(ok);
            strFree(l3 + 0x18);
            strFree(text2);
            sharedRelease(td2);
            strFree(text1);
            sharedRelease(td1);
        } else {
            const uint32_t td = f.addr(0x90), text = f.addr(0x98);
            textInto(td, text, TXT_OFFER_ACCEPT_ASK);
            const uint32_t l3 = f.addr(0x4C);
            copyAcceptLambda(l3, l1, false);
            const uint32_t ok = f.addr(0x1A4), cancel = f.addr(0x18C);
            call(g::AcceptLambda_build, {ok, l3});
            st32(cancel + 16, 0);
            call(g::ScenePartyManagement_openPopUpApproveCheck, {scene, text, t1 + 0x10, ok, cancel});
            functionDestroy(cancel);
            functionDestroy(ok);
            strFree(l3 + 0x18);
            strFree(text);
            sharedRelease(td);
        }
        strFree(l1 + 0x18);
        strFree(t1 + 0x10);
    }
    // sub_6EBA50: the confirm std::function (heap 0x48: the lambda moved in, its second word left out)
    static uint32_t buildAccept(uint32_t out, uint32_t l) {
        st32(out + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x48});
        if (obj) {
            st32(obj, VT_AcceptConfirm);
            const uint32_t scene = ld32(l);
            for (uint32_t k = 0; k < 0x10; k += 4) st32(obj + 0x10 + k, ld32(l + 8 + k));
            for (uint32_t k = 0; k < 12; k += 4) st32(obj + 0x20 + k, ld32(l + 0x18 + k));
            st32(l + 0x18, 0);
            st32(l + 0x1C, 0);
            st32(l + 0x20, 0);
            for (uint32_t k = 0; k < 0x1C; k += 4) st32(obj + 0x2C + k, ld32(l + 0x24 + k));
            st32(obj + 8, scene);
        }
        st32(out + 16, obj);
        return out;
    }
    static uint32_t acceptClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x48});
        if (obj) {
            st32(obj, VT_AcceptConfirm);
            st32(obj + 8, ld32(self + 8));
            for (uint32_t k = 0x10; k < 0x20; k += 4) st32(obj + k, ld32(self + k));
            stringCopy(obj + 0x20, self + 0x20);
            for (uint32_t k = 0x2C; k < 0x48; k += 4) st32(obj + k, ld32(self + k));
            st32(obj + 8, ld32(self + 8));
        }
        return obj;
    }
    // operator(): /party/offer/agree with {scene}
    static void acceptConfirmed(uint32_t functor) {
        const uint32_t scene = ld32(functor + 8);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t lo = ld32(functor + 0x40), hi = ld32(functor + 0x44);
        Frame f;
        const uint32_t cb = newFunction(f, VT_AcceptRequest, {scene});
        call(g::APIManager_executePartyOfferAgree, {api, cb, lo, hi, cb});
        functionDestroy(cb);
    }
    // its lambda(bool ok) {scene}: the scene leaves (then the party top), or the error dialog
    static void acceptDone(uint32_t functor, uint32_t okRef) {
        const uint32_t ok = ld8(okRef), scene = ld32(functor + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        Frame f;
        if (ok) {
            const uint32_t then = newFunction(f, VT_AcceptLeft, {});
            call(g::IInOutAnimation_runOutAnimation, {scene + SC_IN_OUT, then});
            functionDestroy(then);
            return;
        }
        if (!ld8(call(fn::APIManager_getInstance, {}) + API_OFFLINE)) return;
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t cb = newFunction(f, VT_AcceptError, {});
        call(g::APIManager_openErrorDialog, {api, cb, 1});
        functionDestroy(cb);
    }
    // the out animation's end: reload the party and show the party top
    static void acceptLeft(uint32_t) {
        const uint32_t sm = sceneManager();
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        st8(sm + SM_PARTY_RELOAD, 1);
        call(g::FunctionVB_assign, {sm + SM_ON_PARTY, none});
        functionDestroy(none);
        const uint32_t none2 = newEmptyFunction(f);
        call(g::ScenePartyTop_callPartyTopAPI, {none2});
        functionDestroy(none2);
        const uint32_t sm2 = sceneManager();
        const uint32_t layer = newFunction(f, ld32(G_FN_LAYER_VT) + 8, {ld32(G_PARTY_TOP_LAYER)});
        rw::call(g::SceneManager_changeSceneLayer, {sm2, layer, 1, 1, 1});
        functionDestroy(layer);
    }
};

// The leader's "Leave Party" (runInJoinLayout): the button -> after the popup delay the OK / Cancel dialog ->
// /party/leave -> "left" (OK: the header updated, the scene leaves to the party top) or the error dialog.
constexpr uint32_t VT_LeaveAsk = 0xF698B0, VT_LeaveConfirm = 0xF698DC, VT_LeaveRequest = 0xF69908;
constexpr uint32_t VT_LeaveLeft = 0xF69934, VT_LeaveOut = 0xF69960, VT_LeaveError = 0xF6998C;
constexpr int32_t TXT_LEAVE_ASK = 103850011, TXT_LEFT = 103850013;
constexpr uint32_t SM_LEAVING = 0x309;                // SceneManager: byte, the party is being left

struct PartyLeave {
    // the button {scene}(sender, type)
    static uint32_t touched(uint32_t functor, uint32_t, uint32_t typeRef) {
        if (ld32(typeRef) != TOUCH_ENDED) return 0;
        const uint32_t scene = ld32(functor + 4);
        call(g::AudioPlaySystemSEDecide, {});
        call(g::SceneManager_openNoTapScreen, {sceneManager(), ld32(sceneManager() + SM_POPUP_DELAY)});
        const uint32_t wait = call(fn::DelayTime_create, {ld32(sceneManager() + SM_POPUP_DELAY)});
        Frame f;
        const uint32_t then = newFunction(f, VT_LeaveAsk, {scene});
        const uint32_t done = call(fn::CallFunc_create, {then});
        call(fn::Node_runAction, {scene, call(fn::Sequence_create, {wait, done, 0})});
        functionDestroy(then);
        return 0;
    }
    // the delayed {scene}: the OK / Cancel dialog
    static uint32_t ask(uint32_t functor) {
        const uint32_t scene = ld32(functor + 4);
        Frame f;
        const uint32_t td = f.alloc(8), text = f.alloc(12);
        textInto(td, text, TXT_LEAVE_ASK);
        const uint32_t ok = newFunction(f, VT_LeaveConfirm, {scene});
        const uint32_t cancel = f.alloc(20);
        st32(cancel + 16, 0);
        call(g::ScenePartyManagement_openPopUpCheck, {VT_LeaveConfirm, text, ok, cancel});
        functionDestroy(cancel);
        functionDestroy(ok);
        strFree(text);
        sharedRelease(td);
        return 0;
    }
    // OK {scene}: the request
    static uint32_t confirmed(uint32_t functor) {
        const uint32_t scene = ld32(functor + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        Frame f;
        const uint32_t cb = newFunction(f, VT_LeaveRequest, {scene});
        call(g::APIManager_executePartyLeave, {api, cb});
        functionDestroy(cb);
        return 0;
    }
    // its answer (bool ok) {scene}
    static uint32_t done(uint32_t functor, uint32_t okRef) {
        const uint32_t ok = ld8(okRef), scene = ld32(functor + 4);
        call(g::SceneManager_enableLoadingIndicator, {sceneManager(), 0});
        st8(sceneManager() + SM_LEAVING, 1);
        Frame f;
        if (ok) {
            const uint32_t td = textData(f, TXT_LEFT);
            const uint32_t text = newStr(f, textDataStr(td));
            const uint32_t cb = newFunction(f, VT_LeaveLeft, {scene});
            call(g::SceneUtil_openOkDialog4Line, {text, cb, 0});
            functionDestroy(cb);
            strFree(text);
            sharedRelease(td);
            return 0;
        }
        if (!ld8(call(fn::APIManager_getInstance, {}) + API_OFFLINE)) return 0;
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t cb = newFunction(f, VT_LeaveError, {});
        call(g::APIManager_openErrorDialog, {api, cb, 1});
        functionDestroy(cb);
        return 0;
    }
    // the "left" dialog closed (bool ok) {scene}: the header, then the scene leaves (to the party top)
    static uint32_t left(uint32_t functor, uint32_t okRef) {
        if (!ld8(okRef)) return 0;
        const uint32_t scene = ld32(functor + 4);
        st8(sceneManager() + SM_LEAVING, 0);
        call(g::SceneManager_updateHeader, {sceneManager()});
        Frame f;
        const uint32_t then = newFunction(f, VT_LeaveOut, {});
        call(g::IInOutAnimation_runOutAnimation, {scene + SC_IN_OUT, then});
        functionDestroy(then);
        return 0;
    }
};
}  // namespace

RW_FUNCTION(0x6EEB74, "ScenePartyManagement::runInJoinLayout::$_11 operator()(uint64) (cancel invitation)",
            PartyManagement::inviteRowTouch);
RW_FUNCTION(0x6EEF74, "ScenePartyManagement::runInJoinLayout::$_11::lambda() __clone()", PartyManagement::inviteClone);
RW_FUNCTION(0x6EF0C4, "ScenePartyManagement::runInJoinLayout::$_11::lambda() operator()", PartyManagement::inviteConfirmed);
RW_FUNCTION(0x6EF25C, "ScenePartyManagement::runInJoinLayout::$_11::lambda()::lambda(bool) operator()",
            PartyManagement::inviteDone);
RW_FUNCTION(0x6EF4AC, "ScenePartyManagement::runInJoinLayout::$_11 result dialog lambda(bool) operator()",
            PartyManagement::inviteDrop);
RW_FUNCTION(0x6ECE74, "ScenePartyManagement::runInJoinLayout::$_13 operator()(uint64) (delete request)",
            PartyManagement::requestRowTouch);
RW_FUNCTION(0x6ED3FC, "ScenePartyManagement::runInJoinLayout::$_13 confirm std::function (sub_6ED3FC)",
            PartyManagement::buildRequestConfirm);
RW_FUNCTION(0x6ED4D4, "ScenePartyManagement::runInJoinLayout::$_13::lambda() __clone()", PartyManagement::requestClone);
RW_FUNCTION(0x6ED624, "ScenePartyManagement::runInJoinLayout::$_13::lambda() operator()", PartyManagement::requestConfirmed);
RW_FUNCTION(0x6ED7BC, "ScenePartyManagement::runInJoinLayout::$_13::lambda()::lambda(bool) operator()",
            PartyManagement::requestDoneR);
RW_FUNCTION(0x6EDA0C, "ScenePartyManagement::runInJoinLayout::$_13 result dialog lambda(bool) operator()",
            PartyManagement::requestDrop);
RW_FUNCTION(0x6E8C04, "ScenePartyManagement::openPopUpCheck", PartyManagement::openPopUpCheck);
RW_FUNCTION(0x6EA338, "ScenePartyManagement::openPopUpCheck lambda __clone()", PartyManagement::checkClosedClone);
RW_FUNCTION(0x6E8D74, "ScenePartyManagement::openPopUpApproveCheck", PartyManagement::openPopUpApproveCheck);
RW_FUNCTION(0x6E9B18, "ScenePartyManagement::openPopUpApproveCheck popup result operator()", PartyManagement::approveClosed);
RW_FUNCTION(0x6E9C90, "ScenePartyManagement::openPopUpApproveCheck close (sub_6E9C90)", PartyManagement::closePopup);
RW_FUNCTION(0x6E9DCE, "ScenePartyManagement::openPopUpApproveCheck remove lambda operator()", PartyManagement::removePopup);
RW_FUNCTION(0x6F010A, "ScenePartyManagement::init::$_9 operator() (tab button)", PartyManagement::tabTouch);
RW_FUNCTION(0x524250, "MemberScrollView::changeRow::$_4 operator()", PartyManagement::rowButton4);
RW_FUNCTION(0x523CE8, "MemberScrollView::changeRow::$_6 operator()", PartyManagement::rowButton6);
RW_FUNCTION(0x6EC230, "ScenePartyManagement::runInEntryLayout::$_14 operator()(int) (withdraw request)",
            PartyManagement::applicationRowTouch);
RW_FUNCTION(0x6EC62C, "ScenePartyManagement::runInEntryLayout::$_14::lambda() __clone()", PartyManagement::applicationClone);
RW_FUNCTION(0x6EC77C, "ScenePartyManagement::runInEntryLayout::$_14::lambda() operator()",
            PartyManagement::applicationConfirmed);
RW_FUNCTION(0x6EC914, "ScenePartyManagement::runInEntryLayout::$_14::lambda()::lambda(bool) operator()",
            PartyManagement::applicationDone);
RW_FUNCTION(0x6ECB64, "ScenePartyManagement::runInEntryLayout::$_14 result dialog lambda(bool) operator()",
            PartyManagement::applicationDrop);
RW_FUNCTION(0x6EA680, "ScenePartyManagement::runInEntryLayout::$_16 operator()(int) (delete invitation)",
            PartyManagement::offerRowTouch);
RW_FUNCTION(0x6EABE4, "ScenePartyManagement::runInEntryLayout::$_16 confirm std::function (sub_6EABE4)",
            PartyManagement::buildOfferConfirm);
RW_FUNCTION(0x6EACBC, "ScenePartyManagement::runInEntryLayout::$_16::lambda() __clone()", PartyManagement::offerClone);
RW_FUNCTION(0x6EAE0C, "ScenePartyManagement::runInEntryLayout::$_16::lambda() operator()", PartyManagement::offerConfirmed);
RW_FUNCTION(0x6EAFA4, "ScenePartyManagement::runInEntryLayout::$_16::lambda()::lambda(bool) operator()",
            PartyManagement::offerDone);
RW_FUNCTION(0x6EB1F4, "ScenePartyManagement::runInEntryLayout::$_16 result dialog lambda(bool) operator()",
            PartyManagement::offerDrop);
RW_FUNCTION(0x6EB504, "ScenePartyManagement::runInEntryLayout::$_15 operator()(int) (accept invitation)",
            PartyEntry::acceptRowTouch);
RW_FUNCTION(0x6EBA50, "ScenePartyManagement::runInEntryLayout::$_15 confirm std::function (sub_6EBA50)",
            PartyEntry::buildAccept);
RW_FUNCTION(0x6EBB2C, "ScenePartyManagement::runInEntryLayout::$_15::lambda() __clone()", PartyEntry::acceptClone);
RW_FUNCTION(0x6EBC74, "ScenePartyManagement::runInEntryLayout::$_15::lambda() operator()", PartyEntry::acceptConfirmed);
RW_FUNCTION(0x6EBDC0, "ScenePartyManagement::runInEntryLayout::$_15::lambda()::lambda(bool) operator()",
            PartyEntry::acceptDone);
RW_FUNCTION(0x6EBF40, "ScenePartyManagement::runInEntryLayout::$_15 out-animation end operator()", PartyEntry::acceptLeft);
RW_FUNCTION(0x6EDD20, "ScenePartyManagement::runInJoinLayout::$_12 operator()(uint64) (approve request)",
            PartyManagement::approveRowTouchR);
RW_FUNCTION(0x6EE4D0, "ScenePartyManagement::runInJoinLayout::$_12::lambda() operator()", PartyManagement::approveConfirmed);
RW_FUNCTION(0x6EE668, "ScenePartyManagement::runInJoinLayout::$_12::lambda()::lambda(bool) operator()",
            PartyManagement::approveDone);
RW_FUNCTION(0x6EE8B8, "ScenePartyManagement::runInJoinLayout::$_12 result dialog lambda(bool) operator()",
            PartyManagement::approveDrop);
RW_FUNCTION(0x6EF6B4, "ScenePartyManagement leave button operator() (sub_6EF6B4)", PartyLeave::touched);
RW_FUNCTION(0x6EF83C, "ScenePartyManagement leave confirm dialog (sub_6EF83C)", PartyLeave::ask);
RW_FUNCTION(0x6EFA08, "ScenePartyManagement leave request (sub_6EFA08)", PartyLeave::confirmed);
RW_FUNCTION(0x6EFB40, "ScenePartyManagement leave answer (sub_6EFB40)", PartyLeave::done);
RW_FUNCTION(0x6EFD58, "ScenePartyManagement left dialog closed (sub_6EFD58)", PartyLeave::left);
RW_FUNCTION(0x6EFE9C, "ScenePartyManagement leave out-animation end (sub_6EFE9C)", PartyEntry::acceptLeft);
