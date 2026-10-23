// Batch #8: ChatManager's requests to the chat server - update (the read mark), createMessage, deleteMessage, block,
// unblock, report - and their answer lambdas ($_16..$_21: the action's interval updated, the status to the
// callback), plus ChatManager::Block (init from the chat server's JSON, copy constructor).
//
// Each request is refused (status 3 to the callback, 0 returned) while its action's interval since the last request
// has not passed (a Date + interval pair per action in ChatManager); otherwise a REST request goes out with an
// answer lambda {this, action, callback} and 1 is returned.
//
// Block (0x44 bytes): std::strings tagName +0, userEndPointUrl +0xC, nickName +0x18, userImageUrl +0x24,
// messageData +0x30, createAt +0x3C (Date).
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t Json_member = 0x4B1BB5;               // GenericValue::operator[](const char*)
constexpr uint32_t Json_addMemberUint = 0x4C54C9;        // AddMember<unsigned>(value, name, v, allocator)
constexpr uint32_t Json_addMember = 0x49A225;            // AddMember(value, GenericValue& name, GenericValue& v, alloc)
constexpr uint32_t Json_stringValue = 0x4B90F9;          // GenericValue(const char*, Allocator&) (a copied string)
constexpr uint32_t Document_makeShared = 0x4B9345;
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t String_assign = 0x48D811;
constexpr uint32_t Date_init = 0x4924F1, Date_calculateIntervalSinceNow = 0x4922D5;
constexpr uint32_t ChatManager_updateInterval = 0x4DB225;
constexpr uint32_t createHeaders = 0x4DB699;
constexpr uint32_t RESTClient_getInstance = 0x497AE9, RESTClient_request = 0x497FE5;
constexpr uint32_t StatusCallback_call = 0x4DBF6D, StatusCallback_copy = 0x4DBFED;
constexpr uint32_t StringVector_dtor = 0x47363F;
}  // namespace g

constexpr uint32_t S_EMPTY = 0xDF8A1A;
constexpr uint32_t S_URL_READ = 0xDFC1E6;        // "%s/user/chat/%d"
constexpr uint32_t S_URL_MESSAGE = 0xDFC20C;     // "%s/chat/message/%d/%d"
constexpr uint32_t S_URL_BLOCK = 0xDFC246;       // "%s/user/block"
constexpr uint32_t S_URL_UNBLOCK = 0xDFC254;     // "%s/user/block/%s"
constexpr uint32_t S_URL_REPORT = 0xDFC265;      // "%s/chat/message/report"
constexpr uint32_t S_READ_NUMBER = 0xDFC1F6, S_MESSAGE_DATA = 0xDFC222, S_LOCALHOST = 0xDFC23C;
constexpr uint32_t S_USER_IP = 0xDFC22E, S_TAG_NAME = 0xDFA96F, S_CHAT_ID = 0xDFB0E2, S_MESSAGE_NUMBER = 0xDFC27C;
constexpr uint32_t S_TYPE = 0xDFA938, S_COMMENT = 0xDFB036;
constexpr uint32_t S_END_POINT = 0xDFC303, S_NICK_NAME = 0xDFC2FA, S_IMAGE_URL = 0xDFC2ED, S_CREATE_AT = 0xDFC2E4;

// the actions: the answer lambda's vtable, the HTTP method, the interval pair in ChatManager
struct Action {
    uint32_t id, vtable, method, date;
};
constexpr Action UPDATE{2, 0xF557E4, 2, 0x48}, CREATE{3, 0xF557B8, 1, 0x54}, REMOVE{4, 0xF5578C, 3, 0x60};
constexpr Action BLOCK{5, 0xF55760, 1, 0x6C}, UNBLOCK{6, 0xF55734, 3, 0x78}, REPORT{7, 0xF55708, 1, 0x84};

constexpr uint32_t J_FLAGS = 0xC, J_STRING = 0x100000, T_OBJECT = 3;
constexpr uint32_t J_CONST_STRING = 0x100005;    // a GenericValue naming a member (const string)
constexpr uint32_t API_SERVER = 0x7C;            // APIManager: the server settings (+0x2C the API URL)
constexpr uint32_t API_USER = 0x84;              // APIManager: the user (+0x7C chat id, +0x80 chat server URL)

// refused while the action's interval since its last request has not passed
bool waiting(uint32_t self, const Action &a) {
    const int32_t since = int32_t(call(g::Date_calculateIntervalSinceNow, {self + a.date}));
    return since > 0 || int32_t(ld32(self + a.date + 8)) > -since;
}
uint32_t refuse(uint32_t callback) {
    if (ld32(callback + 0x10)) call(g::StatusCallback_call, {callback, 3});
    return 0;
}

uint32_t chatUser() { return ld32(call(fn::APIManager_getInstance, {}) + API_USER); }
uint32_t serverUrl() { return strData(ld32(call(fn::APIManager_getInstance, {}) + API_SERVER) + 0x2C); }

// a fresh JSON object document (shared_ptr at `doc`); its allocator
uint32_t newDocument(uint32_t doc) {
    call(g::Document_makeShared, {doc});
    const uint32_t d = ld32(doc);
    const uint32_t allocator = ld32(d + 0x10);
    if (d) {
        st32(d, 0);
        st32(d + 4, 0);
        st32(d + 8, 0);
        st32(d + 0xC, T_OBJECT);
    }
    return allocator;
}

// object[name] = copy of the C string `text`
void addString(uint32_t object, uint32_t name, uint32_t nameLength, uint32_t text, uint32_t allocator) {
    Frame f;
    const uint32_t value = f.alloc(16), key = f.alloc(16);
    call(g::Json_stringValue, {value, text, allocator});
    st32(key, name);
    st32(key + 4, nameLength);
    st32(key + 0xC, J_CONST_STRING);
    call(g::Json_addMember, {object, key, value, allocator});
}

// the REST request with the answer lambda {this, action, callback}; the document's shared_ptr at `doc`
void send(uint32_t self, const Action &a, uint32_t callback, uint32_t url, uint32_t headers, uint32_t doc) {
    Frame f;
    const uint32_t client = call(g::RESTClient_getInstance, {});
    const uint32_t lambda = f.alloc(0x20);
    st32(lambda, self);
    st32(lambda + 4, a.id);
    call(g::StatusCallback_copy, {lambda + 8, callback});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, a.vtable);
        st32(heap + 8, ld32(lambda));
        st32(heap + 0xC, ld32(lambda + 4));
        const uint32_t p = ld32(lambda + 8 + 0x10);
        if (!p) {
            st32(heap + 0x20, 0);
        } else if (p == lambda + 8) {
            st32(heap + 0x20, heap + 0x10);
            vcall(p, 0x0C, {p, heap + 0x10});
        } else {
            st32(heap + 0x20, vcall(p, 0x08, {p}));
        }
    }
    st32(fo + 16, heap);
    const uint32_t cipher = f.alloc(8), connection = f.alloc(8), result = f.alloc(8);
    st32(cipher, 0);
    st32(cipher + 4, 0);
    st32(connection, 0);
    st32(connection + 4, 0);
    const uint32_t empty = newStr(f, S_EMPTY, 0);
    call(g::RESTClient_request, {result, client, url, a.method, headers, doc, fo, cipher, 0, connection, empty});
    sharedRelease(result);
    strFree(empty);
    sharedRelease(connection);
    sharedRelease(cipher);
    functionDestroy(fo);
    functionDestroy(lambda + 8);
    sharedRelease(doc);
    call(g::StringVector_dtor, {headers});
}

// update(read message number): PUT /user/chat/<chat id> {"chatReadMessageNumber"}
uint32_t update(uint32_t self, uint32_t number, uint32_t callback) {
    if (waiting(self, UPDATE)) return refuse(callback);
    const uint32_t server = serverUrl();
    const uint32_t user = chatUser();
    Frame f;
    const uint32_t url = stringBuffer(f, {S_URL_READ, server, ld32(user + 0x7C)});
    const uint32_t headers = f.alloc(12);
    call(g::createHeaders, {headers, 1});
    const uint32_t doc = f.alloc(8);
    const uint32_t allocator = newDocument(doc);
    call(g::Json_addMemberUint, {ld32(doc), S_READ_NUMBER, number, allocator});
    send(self, UPDATE, callback, url, headers, doc);
    return 1;
}

// createMessage(text): POST /chat/message/<chat id>/0 {"messageData", "userIpAddress": "127.0.0.1"}
uint32_t createMessage(uint32_t self, uint32_t text, uint32_t callback) {
    if (waiting(self, CREATE)) return refuse(callback);
    const uint32_t user = chatUser();
    Frame f;
    const uint32_t url = stringBuffer(f, {S_URL_MESSAGE, strData(user + 0x80), ld32(user + 0x7C), 0});
    const uint32_t headers = f.alloc(12);
    call(g::createHeaders, {headers, 1});
    const uint32_t doc = f.alloc(8);
    const uint32_t allocator = newDocument(doc);
    addString(ld32(doc), S_MESSAGE_DATA, 11, text, allocator);
    addString(ld32(doc), S_USER_IP, 13, S_LOCALHOST, allocator);
    send(self, CREATE, callback, url, headers, doc);
    return 1;
}

// deleteMessage(number): DELETE /chat/message/<chat id>/<number>
uint32_t deleteMessage(uint32_t self, uint32_t number, uint32_t callback) {
    if (waiting(self, REMOVE)) return refuse(callback);
    const uint32_t user = chatUser();
    Frame f;
    const uint32_t url = stringBuffer(f, {S_URL_MESSAGE, strData(user + 0x80), ld32(user + 0x7C), number});
    const uint32_t headers = f.alloc(12);
    call(g::createHeaders, {headers, 1});
    const uint32_t doc = f.alloc(8);
    newDocument(doc);
    send(self, REMOVE, callback, url, headers, doc);
    return 1;
}

// block(tagName): POST /user/block {"tagName"}
uint32_t block(uint32_t self, uint32_t tag, uint32_t callback) {
    if (waiting(self, BLOCK)) return refuse(callback);
    Frame f;
    const uint32_t url = stringBuffer(f, {S_URL_BLOCK, serverUrl()});
    const uint32_t headers = f.alloc(12);
    call(g::createHeaders, {headers, 1});
    const uint32_t doc = f.alloc(8);
    const uint32_t allocator = newDocument(doc);
    addString(ld32(doc), S_TAG_NAME, 7, tag, allocator);
    send(self, BLOCK, callback, url, headers, doc);
    return 1;
}

// unblock(tagName): DELETE /user/block/<tagName>
uint32_t unblock(uint32_t self, uint32_t tag, uint32_t callback) {
    if (waiting(self, UNBLOCK)) return refuse(callback);
    Frame f;
    const uint32_t url = stringBuffer(f, {S_URL_UNBLOCK, serverUrl(), tag});
    const uint32_t headers = f.alloc(12);
    call(g::createHeaders, {headers, 1});
    const uint32_t doc = f.alloc(8);
    newDocument(doc);
    send(self, UNBLOCK, callback, url, headers, doc);
    return 1;
}

// report(message number, type, comment): POST /chat/message/report {"chatId", "messageNumber", "type", "comment"}
uint32_t report(uint32_t self, uint32_t number, uint32_t type, uint32_t comment, uint32_t callback) {
    if (waiting(self, REPORT)) return refuse(callback);
    const uint32_t user = chatUser();
    Frame f;
    const uint32_t url = stringBuffer(f, {S_URL_REPORT, strData(user + 0x80)});
    const uint32_t headers = f.alloc(12);
    call(g::createHeaders, {headers, 1});
    const uint32_t doc = f.alloc(8);
    const uint32_t allocator = newDocument(doc);
    call(g::Json_addMemberUint, {ld32(doc), S_CHAT_ID, ld32(user + 0x7C), allocator});
    call(g::Json_addMemberUint, {ld32(doc), S_MESSAGE_NUMBER, number, allocator});
    call(g::Json_addMemberUint, {ld32(doc), S_TYPE, type, allocator});
    addString(ld32(doc), S_COMMENT, 7, comment, allocator);
    send(self, REPORT, callback, url, headers, doc);
    return 1;
}

// an answer {this, action, callback}(result): the interval updated from the response, then the status (1 / 2 from
// the result, 0 for an HTTP 2xx) to the callback
uint32_t answered(uint32_t functor, uint32_t resultPtr) {
    const uint32_t result = ld32(resultPtr);
    call(g::ChatManager_updateInterval, {ld32(functor + 8), ld32(functor + 0xC), result + 8});
    if (!ld32(functor + 0x20)) return 0;
    const uint32_t status = ld32(result);
    uint32_t code = 0;
    if (status == 1) code = 1;
    if (status == 2) code = 2;
    if (ld32(result + 4) - 200 < 100) code = 0;
    return call(g::StatusCallback_call, {functor + 0x10, code});
}

// a string member: assigned when it is a string, "" when null, false otherwise
bool readString(uint32_t object, uint32_t name, uint32_t dst) {
    const uint32_t value = call(g::Json_member, {object, name});
    const uint32_t flags = ld32(value + J_FLAGS);
    if (flags & J_STRING) {
        const uint32_t text = ld32(value);
        call(g::String_assign, {dst, text, call(g::STRLEN, {text})});
        return true;
    }
    if (flags) return false;
    call(g::String_assign, {dst, S_EMPTY, 0});
    return true;
}

uint32_t blockInit(uint32_t self, uint32_t json) {
    if (ld32(json + J_FLAGS) != T_OBJECT) return 0;
    if (!readString(json, S_TAG_NAME, self)) return 0;
    if (!readString(json, S_END_POINT, self + 0xC)) return 0;
    if (!readString(json, S_NICK_NAME, self + 0x18)) return 0;
    if (!readString(json, S_IMAGE_URL, self + 0x24)) return 0;
    if (!readString(json, S_MESSAGE_DATA, self + 0x30)) return 0;
    const uint32_t created = call(g::Json_member, {json, S_CREATE_AT});
    if (!(ld8(created + 0xE) & 0x10)) return 0;
    if (call(g::Date_init, {self + 0x3C, ld32(created), 0}) != 1) return 0;
    st8(self + 0x43, uint8_t(ld8(self + 0x43) | 1));
    return 1;
}

void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) strInit(dst, ld32(src + 8), ld32(src + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
}
uint32_t blockCopy(uint32_t self, uint32_t other) {
    for (uint32_t o = 0; o < 0x3C; o += 0xC) copyString(self + o, other + o);
    for (uint32_t k = 0x3C; k < 0x44; k += 2) rw::save<uint16_t>(self + k, rw::load<uint16_t>(other + k));
    return self;
}
}  // namespace

RW_FUNCTION(0x4DBC88, "ChatManager::update", update);
RW_FUNCTION(0x4DC020, "ChatManager::createMessage", createMessage);
RW_FUNCTION(0x4DC364, "ChatManager::deleteMessage", deleteMessage);
RW_FUNCTION(0x4DC610, "ChatManager::block", block);
RW_FUNCTION(0x4DC900, "ChatManager::unblock", unblock);
RW_FUNCTION(0x4DCBA8, "ChatManager::report", report);
RW_FUNCTION(0x4DD03C, "ChatManager::report $_21 operator() (sub_4DD03C)", answered);
RW_FUNCTION(0x4DD220, "ChatManager::unblock $_20 operator() (sub_4DD220)", answered);
RW_FUNCTION(0x4DD404, "ChatManager::block $_19 operator() (sub_4DD404)", answered);
RW_FUNCTION(0x4DD5E8, "ChatManager::deleteMessage $_18 operator() (sub_4DD5E8)", answered);
RW_FUNCTION(0x4DD7CC, "ChatManager::createMessage $_17 operator() (sub_4DD7CC)", answered);
RW_FUNCTION(0x4DD9B0, "ChatManager::update $_16 operator() (sub_4DD9B0)", answered);
RW_FUNCTION(0x4DAE70, "ChatManager::Block::init", blockInit);
RW_FUNCTION(0x4DEB5E, "ChatManager::Block::Block(const Block&)", blockCopy);
