// ChatManager's messages: Message (copy constructor, init from the chat server's JSON), getMessage (the REST request
// for a page of messages) and its response lambda (sub_4DDB94).
//
// Message (0x4C bytes): +0 message number, std::strings tagName +4, userEndPointUrl +0x10, nickName +0x1C,
// userImageUrl +0x28, messageData +0x34, +0x40 messageStatus, +0x44 createAt (Date).
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t Json_member = 0x4B1BB5;                   // rapidjson GenericValue::operator[](const char*)
constexpr uint32_t Json_addMemberUint = 0x4C54C9;            // AddMember<unsigned>(value, name, v, allocator)
constexpr uint32_t Document_makeShared = 0x4B9345;           // make_shared<GenericDocument>() (sret)
constexpr uint32_t STRTOUMAX = 0x413F90;                     // (import)
constexpr uint32_t STRLEN = 0x40E08C;                        // (import)
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t String_assign = 0x48D811;                 // std::string::assign(const char*, size_t)
constexpr uint32_t Date_init = 0x4924F1;                     // (date, const char*, bool jst)
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5;
constexpr uint32_t ChatManager_updateInterval = 0x4DB225;    // (action words, const Document&)
constexpr uint32_t createHeaders = 0x4DB699;                 // (vector<string> out, bool)
constexpr uint32_t RESTClient_getInstance = 0x497AE9;
constexpr uint32_t RESTClient_request = 0x497FE5;            // (shared_ptr out, client, url, method, headers&&, doc,
                                                             //  callback, cipher, bool, connection, const string&)
constexpr uint32_t MessagesCallback_copy = 0x4DBC57;         // function<void(StatusCode, const vector<Message>&, ...)>
constexpr uint32_t MessagesCallback_call = 0x4DBB29;         // ... ::operator()(code, messages, a, b, c)
constexpr uint32_t MessageVector_dtor = 0x4DBBBD;
constexpr uint32_t MessageVector_pushSlow = 0x4DDF05;        // __push_back_slow_path(const Message&)
constexpr uint32_t StringVector_dtor = 0x47363F;
constexpr uint32_t Message_copy = 0x4DE205;
constexpr uint32_t Message_init = 0x4DABE9;

constexpr uint32_t S_MESSAGE_NUMBER = 0xDFC27C, S_TAG_NAME = 0xDFA96F, S_END_POINT = 0xDFC303, S_NICK_NAME = 0xDFC2FA,
                   S_IMAGE_URL = 0xDFC2ED, S_MESSAGE_DATA = 0xDFC222, S_MESSAGE_STATUS = 0xDFC313,
                   S_CREATE_AT = 0xDFC2E4;
constexpr uint32_t S_ENTRY = 0xDFC2A3, S_START_INDEX = 0xDFC1C8, S_ITEM_PER_PAGE = 0xDFC297,
                   S_TOTAL_RESULTS = 0xDFC28A;
constexpr uint32_t S_URL_FMT = 0xDFC1B5;                     // "%s/chat/message/%d"
constexpr uint32_t S_COUNT = 0xDFB5EE, S_START_MESSAGE_NUMBER = 0xDFC1D3;
constexpr uint32_t S_EMPTY = 0xDF8A1A;

constexpr uint32_t VT_GotMessages = 0xF55810;                // getMessage's response lambda {this, action, callback}

// rapidjson value: data +0 (string pointer / number), size +4, flags +0xC (type in the low byte)
constexpr uint32_t J_FLAGS = 0xC, J_STRING = 0x100000, J_UINT = 0x2000;
constexpr uint32_t T_OBJECT = 3, T_ARRAY = 4;
constexpr uint32_t MESSAGE_SIZE = 0x4C;
constexpr uint32_t API_USER = 0x84;                          // APIManager: the user (+0x7C chat id, +0x80 server URL)

uint32_t member(uint32_t value, uint32_t name) { return call(Json_member, {value, name}); }

bool readUnsigned(uint32_t value, uint32_t &out) {
    const uint32_t flags = ld32(value + J_FLAGS);
    if (flags & J_STRING) {
        out = call(STRTOUMAX, {ld32(value), 0, 10});
        return true;
    }
    if (flags & J_UINT) {
        out = ld32(value);
        return true;
    }
    return false;
}

// a string member: assigned when it is a string, "" when null, false otherwise
bool readString(uint32_t object, uint32_t name, uint32_t dst) {
    const uint32_t value = member(object, name);
    const uint32_t flags = ld32(value + J_FLAGS);
    if (flags & J_STRING) {
        const uint32_t text = ld32(value);
        call(String_assign, {dst, text, call(STRLEN, {text})});
        return true;
    }
    if (flags) return false;
    call(String_assign, {dst, S_EMPTY, 0});
    return true;
}

void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) strInit(dst, ld32(src + 8), ld32(src + 4));
    else for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
}

void destroyMessageStrings(uint32_t message) {
    for (uint32_t o : {0x34u, 0x28u, 0x1Cu, 0x10u, 0x4u}) strFree(message + o);
}
}  // namespace

namespace chat_message {

uint32_t copy(uint32_t self, uint32_t other) {
    st32(self, ld32(other));
    copyString(self + 4, other + 4);
    copyString(self + 0x10, other + 0x10);
    copyString(self + 0x1C, other + 0x1C);
    copyString(self + 0x28, other + 0x28);
    copyString(self + 0x34, other + 0x34);
    for (uint32_t k = 0x40; k < 0x4C; k += 4) st32(self + k, ld32(other + k));
    return self;
}
RW_FUNCTION(0x4DE204, "ChatManager::Message::Message(const Message&)", chat_message::copy);

uint32_t init(uint32_t self, uint32_t json) {
    if (ld32(json + J_FLAGS) != T_OBJECT) return 0;
    uint32_t number;
    if (!readUnsigned(member(json, S_MESSAGE_NUMBER), number)) return 0;
    st32(self, number);
    if (!readString(json, S_TAG_NAME, self + 4)) return 0;
    if (!readString(json, S_END_POINT, self + 0x10)) return 0;
    if (!readString(json, S_NICK_NAME, self + 0x1C)) return 0;
    if (!readString(json, S_IMAGE_URL, self + 0x28)) return 0;
    if (!readString(json, S_MESSAGE_DATA, self + 0x34)) return 0;
    uint32_t status;
    if (!readUnsigned(member(json, S_MESSAGE_STATUS), status)) return 0;
    st32(self + 0x40, status);
    const uint32_t created = member(json, S_CREATE_AT);
    if (!(ld8(created + 0xE) & 0x10)) return 0;
    if (call(Date_init, {self + 0x44, ld32(created), 0}) != 1) return 0;
    st8(self + 0x4B, uint8_t(ld8(self + 0x4B) | 1));
    return 1;
}
RW_FUNCTION(0x4DABE8, "ChatManager::Message::init", chat_message::init);

// Ask the chat server for `count` messages (type 1: from index, 2: from message number). Refused (status 3 to the
// callback) while the update interval since the last request has not passed. 1 when the request went out.
uint32_t getMessage(uint32_t self, uint32_t count, uint32_t type, uint32_t index, uint32_t callback) {
    const int32_t since = int32_t(call(Date_calculateIntervalSinceNow, {self + 0x3C}));
    if (since > 0 || int32_t(ld32(self + 0x44)) > -since) {
        if (ld32(callback + 0x10)) {
            Frame f;
            const uint32_t none = f.alloc(12);
            for (uint32_t k = 0; k < 12; k += 4) st32(none + k, 0);
            call(MessagesCallback_call, {callback, 3, none, 0, 0, 0});
            call(MessageVector_dtor, {none});
        }
        return 0;
    }
    const uint32_t user = ld32(call(fn::APIManager_getInstance, {}) + API_USER);
    Frame f;
    const uint32_t url = stringBuffer(f, {S_URL_FMT, strData(user + 0x80), ld32(user + 0x7C)});
    const uint32_t headers = f.alloc(12);
    call(createHeaders, {headers, 1});
    const uint32_t doc = f.alloc(8);
    call(Document_makeShared, {doc});
    const uint32_t d = ld32(doc);
    const uint32_t allocator = ld32(d + 0x10);
    if (d) {
        st32(d, 0);
        st32(d + 4, 0);
        st32(d + 8, 0);
        st32(d + 0xC, T_OBJECT);
    }
    call(Json_addMemberUint, {d, S_COUNT, count, allocator});
    if (type == 1) call(Json_addMemberUint, {ld32(doc), S_START_INDEX, index, allocator});
    else if (type == 2) call(Json_addMemberUint, {ld32(doc), S_START_MESSAGE_NUMBER, index, allocator});
    const uint32_t client = call(RESTClient_getInstance, {});
    // the lambda {this, action 1, callback}: built on the stack, moved into the heap
    const uint32_t lambda = f.alloc(0x20);
    st32(lambda, self);
    st32(lambda + 4, 1);
    call(MessagesCallback_copy, {lambda + 8, callback});
    const uint32_t fo = f.alloc(20);
    st32(fo + 16, 0);
    const uint32_t heap = call(fn::operator_new, {0x28});
    if (heap) {
        st32(heap, VT_GotMessages);
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
    call(RESTClient_request, {result, client, url, 0, headers, doc, fo, cipher, 0, connection, empty});
    sharedRelease(result);
    strFree(empty);
    sharedRelease(connection);
    sharedRelease(cipher);
    functionDestroy(fo);
    functionDestroy(lambda + 8);
    sharedRelease(doc);
    call(StringVector_dtor, {headers});
    return 1;
}
RW_FUNCTION(0x4DB7E0, "ChatManager::getMessage", chat_message::getMessage);

// getMessage's response: the messages of "entry", startIndex / itemPerPage / totalResults, to the callback
// (status 1 / 2 of the response override; 2 when the JSON is not as expected). On a parse failure the original
// passes leftover register values for the three numbers (stack addresses, JSON flag words); the rewrite passes the
// numbers read so far and 0 for the others.
void gotMessages(uint32_t functor, uint32_t responsePtr) {
    const uint32_t response = ld32(responsePtr);
    const uint32_t doc = response + 8;
    call(ChatManager_updateInterval, {ld32(functor + 8), ld32(functor + 0xC), doc});
    Frame f;
    const uint32_t messages = f.alloc(12);
    for (uint32_t k = 0; k < 12; k += 4) st32(messages + k, 0);
    uint32_t code = 2, start = 0, perPage = 0, total = 0;
    do {
        if (ld32(response + 0x14) != T_OBJECT) break;
        const uint32_t entry = member(doc, S_ENTRY);
        if (ld32(entry + J_FLAGS) != T_ARRAY) break;
        const uint32_t n = ld32(entry + 4);
        bool ok = true;
        for (uint32_t i = 0; i < n; i++) {
            const uint32_t elem = ld32(entry) + 16 * i;
            if (ld32(elem + J_FLAGS) != T_OBJECT) {
                ok = false;
                break;
            }
            const uint32_t message = f.alloc(MESSAGE_SIZE);
            call(aeabi_memset, {message + 4, 0x3C, 0});
            st8(message + 0x4B, uint8_t(ld8(message + 0x4B) & 0xFD));
            if (!call(Message_init, {message, elem})) {
                destroyMessageStrings(message);
                ok = false;
                break;
            }
            const uint32_t end = ld32(messages + 4);
            if (end == ld32(messages + 8)) {
                call(MessageVector_pushSlow, {messages, message});
            } else {
                if (end) call(Message_copy, {end, message});
                st32(messages + 4, ld32(messages + 4) + MESSAGE_SIZE);
            }
            destroyMessageStrings(message);
        }
        if (!ok) break;
        if (!readUnsigned(member(doc, S_START_INDEX), start)) break;
        if (!readUnsigned(member(doc, S_ITEM_PER_PAGE), perPage)) break;
        if (!readUnsigned(member(doc, S_TOTAL_RESULTS), total)) break;
        code = 0;
    } while (false);
    const uint32_t status = ld32(response);
    uint32_t override = 0;
    if (status == 1) override = 1;
    if (status == 2) override = 2;
    if (ld32(functor + 0x20))
        call(MessagesCallback_call, {functor + 0x10, override ? override : code, messages, start, perPage, total});
    call(MessageVector_dtor, {messages});
}
RW_FUNCTION(0x4DDB94, "ChatManager::getMessage::$_N::operator() (sub_4DDB94)", chat_message::gotMessages);

}  // namespace chat_message
