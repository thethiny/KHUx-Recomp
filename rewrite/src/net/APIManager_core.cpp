// APIManager: construction, the singleton, connection state, the reply handler and error handling.
#include "APIManager.hpp"

#include <cstring>

using namespace apim;

namespace {

constexpr uint32_t SAFEBUFFER_CTOR = 0x459F21;         // SafeBuffer::SafeBuffer(unsigned size, bool)
constexpr uint32_t SAFEBUFFER_DTOR = 0x45A13D;         // SafeBuffer::~SafeBuffer
constexpr uint32_t VECTOR_INT_DTOR = 0x478961;         // std::__vector_base<int>::~__vector_base
constexpr uint32_t VECTOR_PARTYEX_DTOR = 0x4B3DD1;     // std::__vector_base<PartyApplicationList::PartyEx>::~
constexpr uint32_t VECTOR_RAIDBASE_DTOR = 0x4BA87D;    // std::__vector_base<RaidBase>::~
constexpr uint32_t STAGE_REQUEST_VTABLE_GOT = 0xFD6480;// GOT: &vtable for APIManager::StageRequestData
constexpr uint32_t DTOR_GOT = 0xFD6484;                // GOT: &APIManager::~APIManager
constexpr uint32_t DSO_HANDLE_GOT = 0xFD604C;          // GOT: &__dso_handle
constexpr uint32_t GUARD_LOAD = 0xCD8411;              // atomic load of the static guard byte
constexpr uint32_t GUARD_ACQUIRE = 0xC98BC5;           // __cxa_guard_acquire
constexpr uint32_t GUARD_RELEASE = 0xC98C85;           // __cxa_guard_release
constexpr uint32_t CXA_ATEXIT = 0x40DCD8;              // __cxa_atexit (import)
constexpr uint32_t SYSTEM_NEED_URL_PARSE = 0x4A6659;   // hole::network::api::SystemNeedUrl::parse
constexpr uint32_t SET_REFERENCE_DATE = 0x4922AD;      // hole::time::setReferenceDate(const Date&)
constexpr uint32_t APPSTATE_NEEDS_UPDATE_APP = 0x4EAE91;
constexpr uint32_t APPSTATE_NEEDS_UPDATE_DATA = 0x4EAE5D;
constexpr uint32_t CLIENT_IS_LOGIN_ACTION = 0x4B7901;  // Client::isLoginAction(Action)
constexpr uint32_t VERIFY_RESPONSE = 0x4BB4C5;         // APIManager::verifyResponse (not rewritten: 13 KB)
constexpr uint32_t TEXTDATA_GET = 0x4928AD;            // TextData::get(Type, int) -> shared_ptr<entry>
constexpr uint32_t STRING_ASSIGN_CHARS = 0x48D811;     // std::string::assign(const char*, size_t)
constexpr uint32_t STRING_ASSIGN = 0x498A1D;           // std::string::operator=(const std::string&)

// own methods (called through the dispatcher)
constexpr uint32_t ABORT = 0x4BAE85;
constexpr uint32_t FINISH_CONNECTION = 0x4BAEB1;
constexpr uint32_t HANDLE_ERROR = 0x4C28A1;
constexpr uint32_t CALLBACK_LOGIN = 0x4C2A1D;
constexpr uint32_t CALLBACK_USER_CHAT_GET = 0x4C2B2B;
constexpr uint32_t CALLBACK_USER_SHOP_GET = 0x4C2B83;
constexpr uint32_t CALLBACK_USER_SUPPORT_GET = 0x4C2C31;
constexpr uint32_t CALLBACK_PARTY_GET = 0x4C2B57;
constexpr uint32_t EXECUTE_USER_START = 0x4C61C5;
constexpr uint32_t EXECUTE_USER_CHAT_GET = 0x4C6871;
constexpr uint32_t EXECUTE_USER_SHOP_GET = 0x4C6CD5;
constexpr uint32_t EXECUTE_USER_MEDAL_GET = 0x4C7115;
constexpr uint32_t EXECUTE_USER_AVATAR_ALL_GET = 0x4C7555;
constexpr uint32_t EXECUTE_USER_MATERIAL_GET = 0x4C7335;
constexpr uint32_t EXECUTE_USER_SKILL_GET = 0x4C7225;
constexpr uint32_t EXECUTE_USER_KEYBLADE_GET = 0x4C7445;
constexpr uint32_t EXECUTE_USER_LINK_GET = 0x4C7885;
constexpr uint32_t EXECUTE_USER_AVATAR_PARTS_GET = 0x4C7665;
constexpr uint32_t EXECUTE_USER_TITLE_GET = 0x4C7775;
constexpr uint32_t EXECUTE_USER_SPHERE_GET = 0x4C7005;
constexpr uint32_t EXECUTE_TUTORIAL_STATUS_GET = 0x4C6EF5;
constexpr uint32_t EXECUTE_USER_SUPPORT_GET = 0x4C7995;

// Ret (the reply header) fields
constexpr uint32_t RET_ACTION = 0;           // Action
constexpr uint32_t RET_HAS_HEADER = 4;       // u8: the reply carried a header (time, version...)
constexpr uint32_t RET_MAINTENANCE = 6;      // u8
constexpr uint32_t RET_SESSION_EXPIRED = 7;  // u8: relogin required
constexpr uint32_t RET_MESSAGE = 8;          // std::string: server error message
constexpr uint32_t RET_APP_VERSION = 0x20;   // std::string: required app version
constexpr uint32_t RET_SERVER_DATE = 0x38;   // Date

void zeroBytes(uint32_t a, uint32_t n) {
    for (uint32_t k = 0; k < n; k += 4) rw::store32(rw::host_ptr<uint8_t>(a + k), 0);
}

uint32_t guestStrlen(uint32_t s) { return uint32_t(std::strlen(rw::host_ptr<const char>(s))); }

}  // namespace

// ---------------------------------------------------------------------------------------------- lifetime
APIManager *APIManager::construct(APIManager *self) {
    Frame F(0x168);
    self->state = 0;
    self->connecting = 0;
    self->allowWhileBusy = 0;
    self->status = 0;
    self->callback.f = 0;
    self->params.ptr = 0;
    self->params.ctrl = 0;
    self->loginChain = 0;
    self->failed = 0;
    self->errorMessage.data = 0;
    self->errorMessage.size = 0;
    self->errorMessage.cap = 0;
    zeroBytes(addr(self) + 0x44, 0x204);           // the response shared_ptrs, vectors and strings
    zeroBytes(addr(self) + 0x24C, 0xF0);
    uint32_t request = F.call(SAFEBUFFER_CTOR, {addr(&self->stageRequest), 0x18, 1});
    rw::save<uint32_t>(request, load32(STAGE_REQUEST_VTABLE_GOT) + 8);
    self->string_360.data = 0;
    self->string_360.size = 0;
    self->string_360.cap = 0;
    return self;
}
RW_FUNCTION(0x4BA10C, "APIManager::APIManager", APIManager::construct);

APIManager *APIManager::getInstance() {
    Frame F(16);
    if ((F.call(GUARD_LOAD, {INSTANCE_GUARD, 0, 0}) & 1) == 0 && F.call(GUARD_ACQUIRE, {INSTANCE_GUARD}) != 0) {
        F.call(0x4BA10D, {INSTANCE});                                  // APIManager::APIManager
        F.call(CXA_ATEXIT, {load32(DTOR_GOT), INSTANCE, load32(DSO_HANDLE_GOT)});
        F.call(GUARD_RELEASE, {INSTANCE_GUARD});
    }
    return rw::host_ptr<APIManager>(INSTANCE);
}
RW_FUNCTION_NOVERIFY(0x4BA8C8, "APIManager::getInstance", APIManager::getInstance);

APIManager *APIManager::destruct(APIManager *self) {
    Frame F(0x48);
    auto deleteLongString = [&](const StdString &s) {
        if (load8(addr(&s)) & 1) F.call(OPERATOR_DELETE, {s.data});
    };
    auto release = [&](const StdSharedPtr &sp) { releaseIfSet(F, sp.ctrl); };
    deleteLongString(self->string_360);
    F.call(SAFEBUFFER_DTOR, {addr(&self->stageRequest)});
    for (int k = 4; k >= 0; k--) release(self->resp_314[k]);
    F.call(VECTOR_INT_DTOR, {addr(&self->ints_308)});
    F.call(VECTOR_INT_DTOR, {addr(&self->ints_2fc)});
    for (int k = 15; k >= 0; k--) release(self->resp_27c[k]);
    F.call(VECTOR_INT_DTOR, {addr(&self->ints_270)});
    release(self->resp_268);
    release(self->colosseumStart);
    F.call(VECTOR_INT_DTOR, {addr(&self->ints_254)});
    release(self->resp_24c);
    for (int k = 6; k >= 0; k--) release(self->resp_210[k]);
    F.call(VECTOR_PARTYEX_DTOR, {addr(&self->partyApplications)});
    for (int k = 5; k >= 0; k--) release(self->resp_1d4[k]);
    deleteLongString(self->string_1c8);
    for (uint32_t a = addr(&self->resp_1b0[2]); a >= addr(&self->resp_160[0]); a -= 8) releaseIfSet(F, load32(a + 4));
    F.call(VECTOR_RAIDBASE_DTOR, {addr(&self->raids)});
    for (uint32_t a = addr(&self->resp_144[1]); a >= addr(&self->ret); a -= 8) releaseIfSet(F, load32(a + 4));
    destroyFunction(F, addr(&self->callback));
    release(self->params);
    deleteLongString(self->errorMessage);
    return self;
}
RW_FUNCTION(0x4BA960, "APIManager::~APIManager", APIManager::destruct);

// ---------------------------------------------------------------------------------------------- connection
void APIManager::abort(APIManager *self) {
    Frame F(16);
    self->failed = 0;
    self->connecting = 0;
    destroyFunction(F, addr(&self->callback));
    self->callback.f = 0;
}
RW_FUNCTION(0x4BAE84, "APIManager::abort", APIManager::abort);

// The request ended: run the caller's callback with the result. On success the stored callback is dropped
// first (the call runs on a copy, which may start the next request and store a new callback).
void APIManager::finishConnection(APIManager *self, uint32_t ok) {
    Frame F(0x48);
    const uint32_t copy = F.at(0x10);              // std::function<void(bool)> on the stack
    self->connecting = 0;
    F.call(FN_BOOL_COPY, {copy, addr(&self->callback)});
    if (ok == 1) {
        destroyFunction(F, addr(&self->callback));
        self->callback.f = 0;
    }
    if (load32(copy + 16) != 0) {
        F.call(FN_BOOL_CALL, {copy, ok});
        destroyFunction(F, copy);
    }
}
RW_FUNCTION_NOVERIFY(0x4BAEB0, "APIManager::finishConnection", APIManager::finishConnection);

// The Client's reply for `action`: keep the reply header, check maintenance / required app and data updates,
// verify and parse the body (verifyResponse), then either report the error or continue the request chain.
// Returns the status code.
int32_t APIManager::onRespond(APIManager *self, int32_t action, int32_t status, uint32_t retRef, uint32_t doc) {
    Frame F(0x38);
    const uint32_t tmp = F.at(0x18);               // shared_ptr<SystemNeedUrl> on the stack

    assignShared(F, self->ret, load32(retRef), load32(retRef + 4));
    self->status = status;
    if (action == 59) {                            // system/need_url: keep the parsed URLs
        F.call(SYSTEM_NEED_URL_PARSE, {tmp, doc});
        uint32_t parsed = load32(tmp);
        if (parsed) assignShared(F, self->systemNeedUrl, parsed, load32(tmp + 4));
        releaseIfSet(F, load32(tmp + 4));
    }

    uint32_t result = uint32_t(status);
    uint32_t header = 0;                           // the Ret when it carried a header
    uint32_t ret = load32(retRef);
    bool checkState = true;
    if (ret) {
        if (load8(ret + RET_HAS_HEADER)) {
            F.call(SET_REFERENCE_DATE, {load32(retRef) + RET_SERVER_DATE});
            int32_t stop = 0;
            if (load8(load32(retRef) + RET_MAINTENANCE)) {
                stop = 2;
            } else {
                uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});
                uint32_t r = load32(retRef);
                if (F.call(APPSTATE_NEEDS_UPDATE_APP, {app, cstr(r + RET_APP_VERSION)}) == 1) stop = 3;
            }
            if (stop) {
                self->state = stop;
                F.call(ABORT, {addr(self)});
                return status;
            }
            if (status == 0) {
                result = 0;
                if (F.call(VERIFY_RESPONSE, {addr(self), uint32_t(action), doc}) == 0) {
                    result = 2;
                    self->status = 2;
                }
            }
        }
        ret = load32(retRef);
        if (ret) {
            if (!load8(ret + RET_HAS_HEADER)) {
                self->state = 0;
                checkState = false;
            } else {
                header = ret;
            }
        }
    }
    if (checkState) {
        bool update = true;
        if (self->state == 5) {
            // session expired: only a login reply clears it
            update = header != 0 && F.call(CLIENT_IS_LOGIN_ACTION, {load32(header)}) == 1;
            if (update) header = load32(retRef);
        }
        if (update) {
            int32_t next = 0;
            if (header && F.call(CLIENT_IS_LOGIN_ACTION, {load32(header)}) == 0 &&
                load8(load32(retRef) + RET_SESSION_EXPIRED)) {
                next = 5;
            } else if (uint32_t(action - 25) >= 3) {   // not one of the master/resource download actions
                uint32_t app = F.call(APPSTATE_GET_INSTANCE, {});
                if (F.call(APPSTATE_NEEDS_UPDATE_DATA, {app}) == 1) next = 4;
            }
            self->state = next;
        }
    }

    if (result != 0) {
        self->failed = 1;
        F.call(HANDLE_ERROR, {addr(self), result, retRef});
        F.call(FINISH_CONNECTION, {addr(self), 0});
        return int32_t(result);
    }

    const uint32_t me = addr(self), cb = addr(&self->callback);
    switch (action) {
    case 1:                                        // login -> user/start chain
        self->loginChain = 1;
        F.call(EXECUTE_USER_START, {me, cb});
        break;
    case 2: F.call(EXECUTE_USER_CHAT_GET, {me, cb}); break;
    case 5: F.call(CALLBACK_USER_CHAT_GET, {me}); break;
    case 6: F.call(EXECUTE_USER_SHOP_GET, {me, cb}); break;
    case 7: F.call(CALLBACK_USER_SHOP_GET, {me}); break;
    case 9: F.call(EXECUTE_USER_MEDAL_GET, {me, cb}); break;
    case 10: F.call(EXECUTE_USER_AVATAR_ALL_GET, {me, cb}); break;
    case 11: F.call(EXECUTE_USER_MATERIAL_GET, {me, cb}); break;
    case 13: F.call(EXECUTE_USER_SKILL_GET, {me, cb}); break;
    case 14: F.call(EXECUTE_USER_KEYBLADE_GET, {me, cb}); break;
    case 16: F.call(EXECUTE_USER_LINK_GET, {me, cb}); break;
    case 17: F.call(CALLBACK_USER_SUPPORT_GET, {me}); break;
    case 20: F.call(EXECUTE_USER_AVATAR_PARTS_GET, {me, cb}); break;
    case 21: F.call(EXECUTE_USER_TITLE_GET, {me, cb}); break;
    case 59: F.call(CALLBACK_LOGIN, {me, 0}); break;
    case 64: F.call(EXECUTE_USER_SPHERE_GET, {me, cb}); break;
    case 78: F.call(CALLBACK_PARTY_GET, {me}); break;
    case 137: F.call(EXECUTE_TUTORIAL_STATUS_GET, {me, cb}); break;
    case 140: F.call(EXECUTE_USER_SUPPORT_GET, {me, cb}); break;
    case 12:
    case 58:
        break;                                     // no follow-up and no callback
    default:
        if (uint32_t(action - 1) <= 0x8F) F.call(FINISH_CONNECTION, {me, 1});
        break;
    }
    return 0;
}
RW_FUNCTION_NOVERIFY(0x4BAF7C, "APIManager::onRespond", APIManager::onRespond);

// The error dialog text for a failed request: a fixed TextData message per status, or the server's message.
void APIManager::handleError(APIManager *self, int32_t status, uint32_t retRef) {
    Frame F(0x40);
    auto assignText = [&](uint32_t slot, uint32_t textId) {
        F.call(TEXTDATA_GET, {F.at(slot), 0, textId});
        uint32_t text = load32(load32(F.at(slot)) + 4);
        F.call(STRING_ASSIGN_CHARS, {addr(&self->errorMessage), text, guestStrlen(text)});
        releaseIfSet(F, load32(F.at(slot) + 4));
    };
    switch (status) {
    case 1: assignText(0x20, 105000001); break;    // connection failed
    case 2: assignText(0x18, 105000002); break;    // invalid response
    case 3: assignText(0x08, 109020001); break;    // timeout
    case 4: {                                      // server error: its own message when it sent one
        uint32_t ret = load32(retRef);
        if (ret && strSize(ret + RET_MESSAGE) != 0) {
            F.call(STRING_ASSIGN, {addr(&self->errorMessage), ret + RET_MESSAGE});
        } else {
            assignText(0x10, 105000002);
        }
        break;
    }
    default:
        break;
    }
}
RW_FUNCTION(0x4C28A0, "APIManager::handleError", APIManager::handleError);

bool APIManager::isBusy(const APIManager *self) { return uint32_t(self->state - 1) < 3; }
RW_FUNCTION(0x4C3C50, "APIManager::isBusy", APIManager::isBusy);

bool APIManager::needsRetry(const APIManager *self) {
    if (self->retryable) return true;
    return self->status != 4 && self->status != 0;
}
RW_FUNCTION(0x4C3C62, "APIManager::needsRetry", APIManager::needsRetry);

// APIManager::StageRequestData (a SafeBuffer) deleting destructor
void APIManager::StageRequestData_deletingDestructor(uint32_t self) {
    Frame F(8);
    uint32_t p = F.call(SAFEBUFFER_DTOR, {self});
    F.call(OPERATOR_DELETE, {p});
}
RW_FUNCTION(0x4D7564, "APIManager::StageRequestData::~StageRequestData", APIManager::StageRequestData_deletingDestructor);

// Give every cached user-data response its own copy (make_shared<T>(T&)), so the scenes can keep the old ones.
void APIManager::reallocate(APIManager *self) {
    Frame F(0x90);
    struct Copy {
        StdSharedPtr *member;
        uint32_t makeShared;   // std::make_shared<T>(T&)
        uint32_t slot;
    };
    const Copy copies[] = {
        {&self->login, 0x4D6BC9, 0x70},           {&self->user, 0x4D6AD1, 0x68},
        {&self->userPoint, 0x4D69D9, 0x60},       {&self->userDetail, 0x4D68E1, 0x58},
        {&self->stageResumption, 0x4D67E9, 0x50}, {&self->userStone, 0x4D66F1, 0x48},
        {&self->userAvatarAll, 0x4D64F9, 0x40},   {&self->userAvatarParts, 0x4D6441, 0x38},
        {&self->userMedal, 0x4D6389, 0x30},       {&self->userSkill, 0x4D62D1, 0x28},
        {&self->userMaterial, 0x4D6219, 0x20},    {&self->userKeyblade, 0x4D5FB5, 0x18},
        {&self->userAlbum, 0x4D5EB5, 0x10},       {&self->stageStart, 0x4D5CF9, 0x08},
    };
    for (const Copy &c : copies) {
        uint32_t obj = c.member->ptr;
        if (!obj) continue;
        F.call(c.makeShared, {F.at(c.slot), obj});
        uint32_t ptr = F.word(c.slot), ctrl = F.word(c.slot + 4);
        F.setWord(c.slot, 0);
        F.setWord(c.slot + 4, 0);
        c.member->ptr = ptr;
        uint32_t old = c.member->ctrl;
        c.member->ctrl = ctrl;
        releaseIfSet(F, old);
    }
}
RW_FUNCTION(0x4C399C, "APIManager::reallocate", APIManager::reallocate);

// ---------------------------------------------------------------------------------------------- reset
namespace {
// std::vector<int>::clear as inlined: end = begin (computed as end + ~((end - 4 - begin) | 3))
void clearIntVector(StdVector &v) {
    uint32_t end = v.end, begin = v.begin;
    if (end != begin) v.end = end + (~3u & ~(end - 4 - begin));
}
// std::string::clear
void clearString(StdString &s) {
    uint32_t a = addr(&s);
    if (load8(a) & 1) {
        rw::store8(rw::host_ptr<uint8_t>(s.data), 0);
        s.size = 0;
    } else {
        rw::store8(rw::host_ptr<uint8_t>(a + 1), 0);
        rw::store8(rw::host_ptr<uint8_t>(a), 0);
    }
}
}  // namespace

// Drop every cached response (logout / account change). A few members are left alone by the original: the
// shared_ptrs at +0x188 and +0x228, the word at +0x248 and StageRequestData.
void APIManager::clearAllResponses(APIManager *self) {
    Frame F(0xA8);
    for (uint32_t a = addr(&self->ret); a <= addr(&self->resp_144[1]); a += 8)
        resetShared(F, *rw::host_ptr<StdSharedPtr>(a));

    // raids: destroy the RaidBase elements (240 bytes, a std::string at +0xB0) from the back
    uint32_t end = self->raids.end;
    const uint32_t raidsBegin = self->raids.begin;
    while (end != raidsBegin) {
        uint32_t next = end - 0xF0;
        self->raids.end = next;
        if (load8(end - 0x40) & 1) {
            F.call(OPERATOR_DELETE, {load32(end - 0x38)});
            next = self->raids.end;
        }
        end = next;
    }

    for (uint32_t a = addr(&self->resp_160[0]); a <= addr(&self->resp_1b0[2]); a += 8)
        if (a != addr(&self->resp_160[5])) resetShared(F, *rw::host_ptr<StdSharedPtr>(a));   // +0x188 is kept
    clearString(self->string_1c8);
    for (int k = 0; k <= 5; k++) resetShared(F, self->resp_1d4[k]);

    // party applications: PartyEx (408 bytes: strings at +0x10, +0x24, +0x60, vector<u64> at +0x40)
    const uint32_t partyBegin = self->partyApplications.begin;
    for (uint32_t e = self->partyApplications.end; e != partyBegin; e = self->partyApplications.end) {
        self->partyApplications.end = e - 0x198;
        if (load8(e - 0x138) & 1) F.call(OPERATOR_DELETE, {load32(e - 0x130)});
        F.call(0x468499, {e - 0x158});                          // std::__vector_base<unsigned long long>::~
        if (load8(e - 0x174) & 1) F.call(OPERATOR_DELETE, {load32(e - 0x16C)});
        if (load8(e - 0x188) & 1) F.call(OPERATOR_DELETE, {load32(e - 0x180)});
    }

    for (int k = 0; k <= 6; k++)
        if (k != 3) resetShared(F, self->resp_210[k]);       // +0x228 is kept
    resetShared(F, self->resp_24c);
    clearIntVector(self->ints_254);
    resetShared(F, self->colosseumStart);
    resetShared(F, self->resp_268);
    clearIntVector(self->ints_270);
    for (int k = 0; k <= 15; k++) resetShared(F, self->resp_27c[k]);
    clearIntVector(self->ints_2fc);
    clearIntVector(self->ints_308);
    for (int k = 0; k <= 4; k++) resetShared(F, self->resp_314[k]);
    clearString(self->string_360);
}
RW_FUNCTION(0x4C3140, "APIManager::clearAllResponses", APIManager::clearAllResponses);
