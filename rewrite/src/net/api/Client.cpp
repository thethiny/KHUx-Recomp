#include "Client.hpp"

namespace hole::network::api {

namespace {
// libc / libc++ / runtime
constexpr uint32_t guard_load = 0xCD8411;
constexpr uint32_t guard_acquire = 0xC98BC5;
constexpr uint32_t guard_release = 0xC98C85;
constexpr uint32_t cxa_atexit = 0x40DCD8;
constexpr uint32_t GOT_Client_D2 = 0xFD6444;
constexpr uint32_t GOT_dso_handle = 0xFD604C;
constexpr uint32_t free_ = 0x40FF64;
constexpr uint32_t string_append = 0x47AC89;          // std::string::append(const char*, size_t)
constexpr uint32_t string_plus = 0x470EAD;            // operator+(const string&, const string&) (sret)
constexpr uint32_t STRING_VECTOR_PUSH_SLOW = 0x484465;  // vector<string>::__push_back_slow_path(string&&)
constexpr uint32_t STRING_VECTOR_BASE_DTOR = 0x47363F;
constexpr uint32_t StringBuffer_ctor = 0x49140D;      // StringBuffer(const char *fmt, ...)
constexpr uint32_t MP_STACK_CTOR = 0x49A659;
constexpr uint32_t MP_STACK_DTOR = 0x49A5FF;
constexpr uint32_t PARSE_STREAM = 0x49BD61;
constexpr uint32_t VALUE_COPY_STRING = 0x4B90F9;      // GenericValue(const char*, allocator&) (copies)
constexpr uint32_t ADD_MEMBER = 0x49A225;             // AddMember(GenericValue &name, GenericValue &value, alloc)
constexpr uint32_t ADD_MEMBER_INT = 0x499801;         // AddMember(const char*, int, alloc)
constexpr uint32_t REMOVE_MEMBER = 0x4B7E25;          // RemoveMember(const char*)
constexpr uint32_t DOC_MAKE_SHARED = 0x4B9345;        // make_shared<GenericDocument>()
constexpr uint32_t DOC_SHARED_PTR_CTOR = 0x4B909D;    // shared_ptr<GenericDocument>(GenericDocument*)
constexpr uint32_t vfs_readData = 0x8DD46D;           // bg::vfs::readData(const char*) -> shared_ptr<Data> (sret)
constexpr uint32_t md5_calculate = 0x4E0A89;
constexpr uint32_t Cipher_ctor = 0x4E0671;            // Cipher::Cipher(const array<uint8_t, 32>&)
constexpr uint32_t Cipher_decrypt = 0x4E06F3;
constexpr uint32_t CIPHER_MAKE_SHARED = 0x4B975D;
constexpr uint32_t base64Decode = 0x905A89;
constexpr uint32_t SafeBuffer_dtor = 0x45A13D;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_getNativeSessionIDString = 0x4EAA31;
constexpr uint32_t AppState_setNativeSessionID = 0x4EAAC5;
constexpr uint32_t AppState_getUUIDString = 0x4EA8E1;
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_getMaxInt = 0x492185;
constexpr uint32_t Random_generateUniformInt = 0x492225;
constexpr uint32_t SEARCH_PRED = 0x49A42B;            // std::__search(..., bool (*&)(char, char))
constexpr uint32_t CHAR_EQUAL_INSENSITIVE = 0x49A419; // the predicate
// RESTClient
constexpr uint32_t RESTClient_getInstance = 0x497AE9;
constexpr uint32_t RESTClient_request = 0x497FE5;
constexpr uint32_t CONNECTION_MAKE_SHARED = 0x49B5E5;
// Client's own
constexpr uint32_t CLIENT_CTOR = 0x4B6FF5;
constexpr uint32_t LOAD_JSON = 0x4B710D;
constexpr uint32_t ISSUE_REQUEST = 0x4B7451;
constexpr uint32_t REQUEST_API = 0x4B7921;
constexpr uint32_t CREATE_HEADERS = 0x4B7C59;
constexpr uint32_t PARSE_RESPONSE = 0x4B7E6D;
constexpr uint32_t RET_PARSE = 0x49D9AD;
constexpr uint32_t RET_MAKE_SHARED = 0x4B1C85;
constexpr uint32_t STATUS_FUNCTION_COPY = 0x4B7329;   // function<StatusCode(...)>::function(const function&)
constexpr uint32_t STATUS_FUNCTION_CALL = 0x4B9141;   // ... ::operator()(code, const shared_ptr<Ret>&, doc)
constexpr uint32_t RESULT_FUNCTION_FROM_BIND = 0x4B9011;   // function<void(Result)>(bind(&Client::X, this, req, _1))
constexpr uint32_t RESULT_FUNCTION_FROM_ACTION_BIND = 0x4B81B5;   // ... (bind(&Client::Y, this, action, _1))
constexpr uint32_t REQUEST_PUSH_BACK = 0x4B735B;
constexpr uint32_t REQUEST_POP_FRONT = 0x4B91C1;
constexpr uint32_t REQUEST_CLEAR = 0x4B9251;
constexpr uint32_t REQUESTS_BASE_DTOR = 0x4B71D5;
constexpr uint32_t GOT_parseResponse = 0xFD644C;      // &Client::parseResponse
constexpr uint32_t GOT_processSystemStatusUpdateCallback = 0xFD6448;
constexpr uint32_t GOT_processLoginTokenCallback = 0xFD6454;
constexpr uint32_t GOT_processNativeSessionCreateCallback = 0xFD6458;

// .rodata strings
constexpr uint32_t KEY_SERVER_CONFIG = 0xDFBA31;      // "json/server_config.json"
constexpr uint32_t KEY_SERVER_API = 0xDFBA49;         // "json/server_api.json"
constexpr uint32_t KEY_SERVER_URL_DOMAIN = 0xDFBB9D;  // "serverURLDomain"
constexpr uint32_t KEY_PREFIX = 0xDFBAC7;             // "systemStatusUpdatePrefix"
constexpr uint32_t KEY_SUFFIX = 0xDFBAED;             // "systemStatusUpdateSuffix"
constexpr uint32_t KEY_STATUS_RESULT = 0xDFBAA4;      // "systemStatusUpdateResult"
constexpr uint32_t KEY_INFO_VERSION = 0xDFBAE0;       // "info/version"
constexpr uint32_t KEY_A = 0xDF959C;                  // "a"
constexpr uint32_t KEY_APP_SIGNATURE = 0xDFBB06;      // "appSignature"
constexpr uint32_t KEY_ACTIONS = 0xDFBB95;            // "actions"
constexpr uint32_t KEY_PATH = 0xDFBB90;               // "path"
constexpr uint32_t KEY_METHOD = 0xDFBB89;             // "method"
constexpr uint32_t FMT_SS = 0xDF8BBA;                 // "%s%s"
constexpr uint32_t KEY_EMPTY = 0xDF8A1A;              // ""
constexpr uint32_t FMT_RETRY = 0xDFBA5E;              // "X-Sqex-Hole-Retry: %d"
constexpr uint32_t KEY_RUV = 0xDFBA74;                // "ruv"
constexpr uint32_t KEY_FORM = 0xDFBB37;               // "Content-Type: application/x-www-form-urlencoded;charset=UTF8"
constexpr uint32_t FMT_NSID = 0xDFBB74;               // "X-Sqex-Hole-Nsid: %s"
constexpr uint32_t KEY_MAINTENANCE = 0xDFBB2B;        // "maintenance"
constexpr uint32_t KEY_VIEW_URL = 0xDFB1AE;           // "viewUrl"
constexpr uint32_t KEY_NATIVE_SESSION_ID = 0xDFBA78;  // "nativeSessionId"
constexpr uint32_t KEY_SHARED_SECURITY_KEY = 0xDFBA88;  // "sharedSecurityKey"
constexpr uint32_t KEY_APP_STATUS = 0xDFBA9A;         // "appStatus"
constexpr uint32_t KEY_MODE = 0xDFBB26;               // "mode"
constexpr uint32_t KEY_CURRENT = 0xDFBB1E;            // "current"
constexpr uint32_t KEY_SERVER = 0xDFBB17;             // "server"
constexpr uint32_t KEY_HTTP = 0xDFBABD;               // "http"
constexpr uint32_t KEY_URL = 0xDFBB13;                // "url"
constexpr uint32_t KEY_NATIVE_TOKEN = 0xDFB9BE;       // "nativeToken"
constexpr uint32_t KEY_UUID = 0xDFBAC2;               // "UUID"
constexpr uint32_t KEY_DEVICE_TYPE = 0xDFB94F;        // "deviceType"

constexpr uint32_t ACTION_LOGIN_TOKEN_OFFSET = 58 * 16;   // "actions"[58]
constexpr uint32_t REQUESTS_PER_BLOCK = 73;
constexpr uint32_t STRING_NAME_FLAGS = 0x100005;      // a const-string GenericValue (kStringFlag | kString)

inline uint32_t R(uint32_t a) { return rw::load<uint32_t>(a); }
inline uint8_t rd8(uint32_t a) { return rw::load<uint8_t>(a); }
inline void W(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }
inline void wr8(uint32_t a, uint8_t v) { rw::store8(rw::host_ptr<uint8_t>(a), v); }
inline uint32_t hostStrlen(uint32_t s) { return uint32_t(std::strlen(rw::host_ptr<char>(s))); }
inline uint32_t addr(const void *p) { return rw::guest_addr(p); }
inline void release(uint32_t ctrl) { if (ctrl) rw::call(guest::shared_release, {ctrl}); }

// rapidjson operator[] with a .rodata key: the member's guest address
uint32_t member(uint32_t value, uint32_t key) { return rw::call(guest::JsonValue_member, {value, key}); }
bool isString(uint32_t value) { return (rd8(value + 14) & 0x10) != 0; }
void assignCString(uint32_t str, uint32_t s) { rw::call(guest::string_assign, {str, s, hostStrlen(s)}); }
void initString(uint32_t str, uint32_t s, uint32_t n) { rw::call(guest::string_init, {str, s, n}); }

// destroy a std::function held in a frame slot (__f_ == the buffer: destroy() in place, else destroy_deallocate())
void destroyLocalFunction(uint32_t buffer, uint32_t f) {
    if (f == buffer) rw::vcall(rw::host_ptr<uint32_t>(buffer), 4, {buffer});
    else if (f) rw::vcall(rw::host_ptr<uint32_t>(f), 5, {f});
}
// vector<string>::emplace_back(std::move(local)) with the fast path inlined
void moveIntoStrings(StdVector *v, uint32_t str) {
    uint32_t end = v->end;
    if (end < v->capEnd) {
        uint32_t base = 0;
        if (end) {
            W(end, R(str));
            W(end + 4, R(str + 4));
            W(end + 8, R(str + 8));
            W(str, 0);
            W(str + 4, 0);
            W(str + 8, 0);
            base = v->end;
        }
        v->end = base + 12;
    } else {
        rw::call(STRING_VECTOR_PUSH_SLOW, {addr(v), str});
    }
}
// std::bind(&Client::callback, this, request, _1) built in a frame slot (0x48 bytes): {memfn, adj, this, pad,
// Request copy (connection, action, params, the callback function, the two flags)}
void buildBound(uint32_t base, uint32_t gotMemfn, uint32_t client, const Client::Request *req) {
    W(base, R(gotMemfn));
    W(base + 4, 0);
    W(base + 8, client);
    W(base + 0x10, req->connection.ptr);
    uint32_t cc = req->connection.ctrl;
    W(base + 0x14, cc);
    if (cc) rw::call(guest::shared_add, {cc});
    uint32_t action = req->action;
    W(base + 0x18, action);
    W(base + 0x1C, req->params.ptr);
    uint32_t pc = req->params.ctrl;
    W(base + 0x20, pc);
    if (pc) {
        rw::call(guest::shared_add, {pc});
        action = req->action;
    }
    W(base + 0x18, action);
    rw::call(STATUS_FUNCTION_COPY, {base + 0x28, addr(req) + 0x18});
    uint16_t flags = uint16_t((uint32_t(req->retry) << 8) | req->cancelable);
    rw::store16(rw::host_ptr<uint16_t>(base + 0x40), flags);
}
void destroyBound(uint32_t base) {
    destroyLocalFunction(base + 0x28, R(base + 0x38));
    release(R(base + 0x20));
    release(R(base + 0x14));
}
uint32_t serverURLChars(const Client *self) { return self->serverURL.data(); }
// the front Request of the queue
uint32_t frontRequest(const Client *self) {
    uint32_t start = self->requests.start;
    return R(self->requests.mapBegin + 4 * (start / REQUESTS_PER_BLOCK)) +
           sizeof(Client::Request) * (start % REQUESTS_PER_BLOCK);
}
uint32_t restRequest(GuestFrame &, std::initializer_list<uint32_t> args) {
    return uint32_t(rw::call_exact(RESTClient_request, args));
}
}  // namespace

// ---------------------------------------------------------------------------------------------- free functions
uint32_t LoadJSON(uint32_t path, uint32_t document) {
    GuestFrame frame(0x38);                   // original frame: the text at +4, the Data at +0x10, the stream at +0x18
    rw::call(vfs_readData, {frame.addr(0x10), path});
    uint32_t data = R(frame.addr(0x10));
    initString(frame.addr(4), R(data + 4), R(data + 8));
    StdString *text = frame.at<StdString>(4);
    uint32_t chars = text->data();
    uint32_t *stream = frame.at<uint32_t>(0x18);
    stream[0] = chars;
    stream[1] = chars;
    rw::call(PARSE_STREAM, {document, frame.addr(0x18)});
    uint32_t error = R(document + 0x28);       // parse result code
    if (text->isLong()) guestDelete(text->w2);
    release(R(frame.addr(0x14)));
    return error ? 0 : 1;
}
RW_FUNCTION(0x4B710C, "hole::network::api::LoadJSON", LoadJSON);

uint32_t createHeaders(StdVector *out, uint32_t form, uint32_t nativeSession) {
    // original frame 0x458: the Nsid line at +0x18, the session id at +0x24, the form line at +0x30, a 1 KB
    // StringBuffer at +0x3c
    GuestFrame frame(0x458);
    out->begin = 0;
    out->end = 0;
    out->capEnd = 0;
    if (form == 1) {
        initString(frame.addr(0x30), KEY_FORM, 0x3C);
        moveIntoStrings(out, frame.addr(0x30));
        if (frame.at<StdString>(0x30)->isLong()) guestDelete(R(frame.addr(0x38)));
    }
    if (nativeSession == 1) {
        uint32_t app = rw::call(AppState_getInstance, {});
        rw::call(AppState_getNativeSessionIDString, {frame.addr(0x24), app});
        StdString *sid = frame.at<StdString>(0x24);
        rw::call(StringBuffer_ctor, {frame.addr(0x3C), FMT_NSID, sid->data()});
        if (sid->isLong()) guestDelete(R(frame.addr(0x2C)));
        initString(frame.addr(0x18), frame.addr(0x3C), hostStrlen(frame.addr(0x3C)));
        moveIntoStrings(out, frame.addr(0x18));
        if (frame.at<StdString>(0x18)->isLong()) guestDelete(R(frame.addr(0x20)));
    }
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4B7C58, "hole::network::api::createHeaders", createHeaders);

// ---------------------------------------------------------------------------------------------- Client
uint32_t Client::getInstance() {
    if (!(rw::call(guard_load, {GUARD, 0, 0}) & 1) && rw::call(guard_acquire, {GUARD})) {
        rw::call(CLIENT_CTOR, {INSTANCE});
        rw::call(cxa_atexit, {R(GOT_Client_D2), INSTANCE, R(GOT_dso_handle)});
        rw::call(guard_release, {GUARD});
    }
    return INSTANCE;
}
// registers an atexit handler: not run twice by the verifier
RW_FUNCTION_NOVERIFY(0x4B6F14, "hole::network::api::Client::getInstance", Client::getInstance);

Client *Client::construct(Client *self) {
    const uint32_t me = addr(self);
    W(me + 0x14, 0);                          // config: null value
    self->cipher.ptr = 0;
    self->cipher.ctrl = 0;
    rw::call(MP_STACK_CTOR, {me + 0x18, 0, 1024});
    W(me + 0x44, 0);                          // api: null value
    W(me + 0x30, 0);                          // config: parse result
    W(me + 0x34, 0);
    rw::call(MP_STACK_CTOR, {me + 0x48, 0, 1024});
    rw::call(guest::memset_, {me + 0x60, 0x2C, 0});   // api's parse result, the queue, serverURL
    rw::call(LOAD_JSON, {KEY_SERVER_CONFIG, me + 8});
    rw::call(LOAD_JSON, {KEY_SERVER_API, me + 0x38});
    uint32_t domain = R(member(me + 8, KEY_SERVER_URL_DOMAIN));
    assignCString(me + 0x80, domain);
    return self;
}
RW_FUNCTION(0x4B6FF4, "hole::network::api::Client::Client", Client::construct);

Client *Client::destroy(Client *self) {
    const uint32_t me = addr(self);
    if (self->serverURL.isLong()) guestDelete(self->serverURL.w2);
    rw::call(REQUESTS_BASE_DTOR, {me + 0x68});
    rw::call(MP_STACK_DTOR, {me + 0x48});
    rw::call(MP_STACK_DTOR, {me + 0x18});
    release(self->cipher.ctrl);
    return self;
}
RW_FUNCTION(0x4B6FAC, "hole::network::api::Client::~Client", Client::destroy);

uint32_t Client::call(SharedPtr *out, Client *self, uint32_t action, uint32_t callback, const SharedPtr *params,
                      uint32_t retry, uint32_t cancelable) {
    GuestFrame frame(0x68);                   // original frame: the Request at +0x10
    rw::call(CONNECTION_MAKE_SHARED, {addr(out)});
    Request *req = frame.at<Request>(0x10);
    req->connection.ptr = out->ptr.get();
    uint32_t cc = out->ctrl;
    req->connection.ctrl = cc;
    if (cc) rw::call(guest::shared_add, {cc});
    req->action = action;
    req->params.ptr = params->ptr.get();
    uint32_t pc = params->ctrl;
    req->params.ctrl = pc;
    if (pc) rw::call(guest::shared_add, {pc});
    rw::call(STATUS_FUNCTION_COPY, {frame.addr(0x28), callback});
    req->cancelable = uint8_t(cancelable);
    req->retry = uint8_t(retry);
    uint32_t pending = self->requests.size;
    rw::call(REQUEST_PUSH_BACK, {addr(self) + 0x68, frame.addr(0x10)});
    if (!pending) rw::call(ISSUE_REQUEST, {addr(self), frame.addr(0x10)});
    destroyLocalFunction(frame.addr(0x28), req->callbackF);
    release(req->params.ctrl);
    release(req->connection.ctrl);
    return 0;   // stack-protector check
}
// issues network requests: not run twice by the verifier
RW_FUNCTION_NOVERIFY(0x4B7200, "hole::network::api::Client::call", Client::call);

uint32_t Client::issueRequest(Client *self, const Request *request) {
    // original frame 0x528: the "" key at +0x40, the null connection / cipher at +0x4c / +0x54, the headers at
    // +0x5c, request's result at +0x68, the document at +0x70, the md5 at +0x78, info/version at +0x84, the
    // signature source at +0x8c, the signature value at +0x98, the bound callback at +0xa8, its std::function at
    // +0xf0, the name value / url StringBuffer at +0x108
    GuestFrame frame(0x528);
    const uint32_t me = addr(self);
    uint32_t action = request->action;
    if (action != 59 && action != 61 && self->cipher.ptr) {
        rw::call(REQUEST_API, {me, addr(request)});
        return 0;   // stack-protector check
    }
    // appSignature = md5(prefix + info/version + "a" + suffix)
    const uint32_t config = me + 8;
    uint32_t prefix = R(member(config, KEY_PREFIX));
    initString(frame.addr(0x8C), prefix, hostStrlen(prefix));
    rw::call(vfs_readData, {frame.addr(0x84), KEY_INFO_VERSION});
    uint32_t version = R(R(frame.addr(0x84)) + 4);
    rw::call(string_append, {frame.addr(0x8C), version, hostStrlen(version)});
    rw::call(string_append, {frame.addr(0x8C), KEY_A, 1});
    uint32_t suffix = R(member(config, KEY_SUFFIX));
    rw::call(string_append, {frame.addr(0x8C), suffix, hostStrlen(suffix)});
    StdString *source = frame.at<StdString>(0x8C);
    rw::call(md5_calculate, {frame.addr(0x78), source->data(), source->size()});

    rw::call(DOC_MAKE_SHARED, {frame.addr(0x70)});
    uint32_t doc = R(frame.addr(0x70));
    uint32_t allocator = R(doc + 0x10);
    uint32_t target = 0;
    if (doc) {                                // SetObject()
        W(doc, 0);
        W(doc + 4, 0);
        W(doc + 8, 0);
        W(doc + 12, 3);
        target = R(frame.addr(0x70));
    }
    StdString *signature = frame.at<StdString>(0x78);
    rw::call(VALUE_COPY_STRING, {frame.addr(0x98), signature->data(), allocator});
    uint32_t *name = frame.at<uint32_t>(0x108);
    name[3] = STRING_NAME_FLAGS;
    name[0] = KEY_APP_SIGNATURE;
    name[1] = 12;
    rw::call(ADD_MEMBER, {target, frame.addr(0x108), frame.addr(0x98), allocator});

    // the system status update request ("actions"[0])
    uint32_t base = serverURLChars(self);
    uint32_t path = R(member(R(member(me + 0x38, KEY_ACTIONS)), KEY_PATH));
    rw::call(StringBuffer_ctor, {frame.addr(0x108), FMT_SS, base, path});
    uint32_t method = R(member(R(member(me + 0x38, KEY_ACTIONS)), KEY_METHOD));
    uint32_t rest = rw::call(RESTClient_getInstance, {});
    rw::call(CREATE_HEADERS, {frame.addr(0x5C), 1, 0});
    buildBound(frame.addr(0xA8), GOT_processSystemStatusUpdateCallback, me, request);
    rw::call(RESULT_FUNCTION_FROM_BIND, {frame.addr(0xF0), frame.addr(0xA8), 0});
    uint32_t *nulls = frame.at<uint32_t>(0x4C);
    nulls[0] = 0;
    nulls[1] = 0;
    nulls[2] = 0;
    nulls[3] = 0;
    initString(frame.addr(0x40), KEY_EMPTY, 0);
    restRequest(frame, {frame.addr(0x68), rest, frame.addr(0x108), method, frame.addr(0x5C), frame.addr(0x70),
                        frame.addr(0xF0), frame.addr(0x54), 0, frame.addr(0x4C), frame.addr(0x40)});

    release(R(frame.addr(0x6C)));
    if (frame.at<StdString>(0x40)->isLong()) guestDelete(R(frame.addr(0x48)));
    release(R(frame.addr(0x50)));
    release(R(frame.addr(0x58)));
    destroyLocalFunction(frame.addr(0xF0), R(frame.addr(0x100)));
    destroyBound(frame.addr(0xA8));
    rw::call(STRING_VECTOR_BASE_DTOR, {frame.addr(0x5C)});
    release(R(frame.addr(0x74)));
    if (signature->isLong()) guestDelete(R(frame.addr(0x80)));
    release(R(frame.addr(0x88)));
    if (source->isLong()) guestDelete(R(frame.addr(0x94)));
    return 0;   // stack-protector check
}
RW_FUNCTION_NOVERIFY(0x4B7450, "hole::network::api::Client::issueRequest", Client::issueRequest);

uint32_t Client::isLoginAction(uint32_t action) { return (action == 61 ? 1u : 0u) | (action == 59 ? 1u : 0u); }
RW_FUNCTION(0x4B7900, "hole::network::api::Client::isLoginAction", Client::isLoginAction);

uint32_t Client::requestAPI(Client *self, const Request *request) {
    // original frame 0x8b8: the "" key at +0x3c, the connection copy at +0x48, request's result at +0x50, the
    // retry line at +0x58, the cipher copy at +0x64, the headers at +0x6c, the callback at +0x78, a 1 KB
    // StringBuffer (retry header) at +0x94, the url StringBuffer at +0x498
    GuestFrame frame(0x8B8);
    const uint32_t me = addr(self);
    uint32_t base = serverURLChars(self);
    uint32_t entry = R(member(me + 0x38, KEY_ACTIONS)) + request->action * 16;
    uint32_t path = R(member(entry, KEY_PATH));
    rw::call(StringBuffer_ctor, {frame.addr(0x498), FMT_SS, base, path});
    entry = R(member(me + 0x38, KEY_ACTIONS)) + request->action * 16;
    uint32_t method = R(member(entry, KEY_METHOD));
    rw::call(CREATE_HEADERS, {frame.addr(0x6C), method == 1 ? 1u : 0u, 1});
    SharedPtr *cipher = frame.at<SharedPtr>(0x64);
    cipher->ptr = self->cipher.ptr.get();
    uint32_t cc = self->cipher.ctrl;
    cipher->ctrl = cc;
    if (cc) rw::call(guest::shared_add, {cc});

    const uint32_t params = addr(request) + 0xC;
    uint32_t action = request->action;
    if (action != 59 && action != 61) {
        rw::call(StringBuffer_ctor, {frame.addr(0x94), FMT_RETRY, request->retry});
        initString(frame.addr(0x58), frame.addr(0x94), hostStrlen(frame.addr(0x94)));
        moveIntoStrings(frame.at<StdVector>(0x6C), frame.addr(0x58));
        if (frame.at<StdString>(0x58)->isLong()) guestDelete(R(frame.addr(0x60)));
        uint32_t p = R(params);
        if (p && !request->retry) {           // a fresh random "ruv" (a retry resends the same one)
            rw::call(REMOVE_MEMBER, {p, KEY_RUV});
            uint32_t doc = R(params);
            uint32_t random = rw::call(Random_getInstance, {});
            uint32_t max = rw::call(Random_getMaxInt, {random});
            uint32_t value = rw::call(Random_generateUniformInt, {random, 0, max, 0});
            rw::call(ADD_MEMBER_INT, {doc, KEY_RUV, value, R(R(params) + 0x10)});
        }
    }
    uint32_t rest = rw::call(RESTClient_getInstance, {});
    rw::call_exact(RESULT_FUNCTION_FROM_ACTION_BIND,
                   {frame.addr(0x78), R(GOT_parseResponse), 0, me, request->action, 0});
    uint8_t cancelable = request->cancelable;
    SharedPtr *conn = frame.at<SharedPtr>(0x48);
    conn->ptr = request->connection.ptr.get();
    uint32_t connCtrl = request->connection.ctrl;
    conn->ctrl = connCtrl;
    if (connCtrl) rw::call(guest::shared_add, {connCtrl});
    initString(frame.addr(0x3C), KEY_EMPTY, 0);
    restRequest(frame, {frame.addr(0x50), rest, frame.addr(0x498), method, frame.addr(0x6C), params,
                        frame.addr(0x78), frame.addr(0x64), cancelable ? 1u : 0u, frame.addr(0x48),
                        frame.addr(0x3C)});

    release(R(frame.addr(0x54)));
    if (frame.at<StdString>(0x3C)->isLong()) guestDelete(R(frame.addr(0x44)));
    release(R(frame.addr(0x4C)));
    destroyLocalFunction(frame.addr(0x78), R(frame.addr(0x88)));
    release(R(frame.addr(0x68)));
    rw::call(STRING_VECTOR_BASE_DTOR, {frame.addr(0x6C)});
    return 0;   // stack-protector check
}
RW_FUNCTION_NOVERIFY(0x4B7920, "hole::network::api::Client::requestAPI", Client::requestAPI);

uint32_t Client::parseResponse(Client *self, uint32_t action, const SharedPtr *result) {
    // original frame 0x88: the failure Ret / document at +0x20 / +0x40, made Rets at +0x28 / +0x30, the Ret at +0x38
    GuestFrame frame(0x88);
    const uint32_t me = addr(self);
    uint32_t res = result->ptr;               // RESTClient::Result {status, http code, document}
    SharedPtr *ret = frame.at<SharedPtr>(0x38);
    ret->reset();
    uint32_t status = R(res);
    uint32_t code = status == 1 ? 1 : 0;
    if (status == 2) code = 2;
    const uint32_t doc = res + 8;
    if (code == 0) {
        code = 2;
        if (R(res + 0x30) == 0 && R(res + 0x14) == JsonValue::kObject) {
            rw::call(RET_PARSE, {frame.addr(0x30), action, doc});
            SharedPtr *parsed = frame.at<SharedPtr>(0x30);
            uint32_t p = parsed->ptr, c = parsed->ctrl;
            parsed->reset();
            ret->ptr = p;
            uint32_t old = ret->ctrl;
            ret->ctrl = c;
            if (old) {
                release(old);
                release(parsed->ctrl);        // (null by now)
            }
            if (!ret->ptr && R(member(doc, KEY_MAINTENANCE) + 12)) {
                // maintenance: a Ret of its own {action, ok 0, maintenance 1, viewUrl}
                rw::call(RET_MAKE_SHARED, {frame.addr(0x28)});
                SharedPtr *made = frame.at<SharedPtr>(0x28);
                p = made->ptr;
                c = made->ctrl;
                made->reset();
                ret->ptr = p;
                old = ret->ctrl;
                ret->ctrl = c;
                release(old);
                W(ret->ptr, action);
                wr8(ret->ptr + 4, 0);
                wr8(ret->ptr + 5, 1);
                uint32_t url = member(doc, KEY_VIEW_URL);
                if (isString(url)) assignCString(ret->ptr + 0x14, R(url));
            }
            if (ret->ptr) {
                code = rd8(ret->ptr + 5) ? 3 : 0;
                if ((R(res + 4) & ~1u) != 200) code = 4;
            }
        }
    }
    uint32_t front = frontRequest(self);
    if (R(front + 0x28)) code = rw::call(STATUS_FUNCTION_CALL, {front + 0x18, code, frame.addr(0x38), doc});
    rw::call(REQUEST_POP_FRONT, {me + 0x68});
    if (code) {
        // fail every queued call with status 5, a null Ret and an empty document; clear the queue
        uint32_t start = self->requests.start;
        uint32_t mapBegin = self->requests.mapBegin, mapEnd = self->requests.mapEnd;
        uint32_t map = mapBegin + 4 * (start / REQUESTS_PER_BLOCK);
        uint32_t p = 0, last = 0;
        if (mapEnd != mapBegin) {
            uint32_t endIndex = start + self->requests.size;
            last = R(mapBegin + 4 * (endIndex / REQUESTS_PER_BLOCK)) +
                   sizeof(Request) * (endIndex % REQUESTS_PER_BLOCK);
            p = R(map) + sizeof(Request) * (start % REQUESTS_PER_BLOCK);
        }
        uint32_t *nullRet = frame.at<uint32_t>(0x20);
        uint32_t *emptyDoc = frame.at<uint32_t>(0x40);
        while (last != p) {
            if (R(p + 0x28)) {
                emptyDoc[3] = 0;
                nullRet[0] = 0;
                nullRet[1] = 0;
                rw::call(MP_STACK_CTOR, {frame.addr(0x50), 0, 1024});
                emptyDoc[10] = 0;
                emptyDoc[11] = 0;
                rw::call(STATUS_FUNCTION_CALL, {p + 0x18, 5, frame.addr(0x20), frame.addr(0x40)});
                rw::call(MP_STACK_DTOR, {frame.addr(0x50)});
                release(nullRet[1]);
            }
            p += sizeof(Request);
            if (p - R(map) == REQUESTS_PER_BLOCK * sizeof(Request)) {
                p = R(map + 4);
                map += 4;
            }
        }
        rw::call(REQUEST_CLEAR, {me + 0x68});
    } else if (self->requests.size) {
        rw::call(ISSUE_REQUEST, {me, frontRequest(self)});
    }
    release(ret->ctrl);
    return 0;   // stack-protector check
}
RW_FUNCTION_NOVERIFY(0x4B7E6C, "hole::network::api::Client::parseResponse", Client::parseResponse);

uint32_t Client::processNativeSessionCreateCallback(Client *self, const Request *request, const SharedPtr *result) {
    GuestFrame frame(0x50);                   // original frame: the Cipher at +0x10, the key bytes at +0x18
    const uint32_t me = addr(self);
    uint32_t res = result->ptr;
    if (R(res) != 0 || R(res + 0x14) != JsonValue::kObject) {
        rw::call(PARSE_RESPONSE, {me, request->action, addr(result)});
        return 0;   // stack-protector check
    }
    const uint32_t doc = res + 8;
    uint32_t id = member(doc, KEY_NATIVE_SESSION_ID);
    uint32_t key = member(doc, KEY_SHARED_SECURITY_KEY);
    if (!isString(id) || !isString(key)) {
        rw::call(PARSE_RESPONSE, {me, request->action, addr(result)});
        return 0;   // stack-protector check
    }
    uint32_t app = rw::call(AppState_getInstance, {});
    rw::call(AppState_setNativeSessionID, {app, R(id)});
    rw::call(guest::memcpy_, {frame.addr(0x18), R(key), 32});
    rw::call(CIPHER_MAKE_SHARED, {frame.addr(0x10), frame.addr(0x18)});
    SharedPtr *made = frame.at<SharedPtr>(0x10);
    uint32_t p = made->ptr, c = made->ctrl;
    made->reset();
    self->cipher.ptr = p;
    uint32_t old = self->cipher.ctrl;
    self->cipher.ctrl = c;
    release(old);
    rw::call(REQUEST_API, {me, addr(request)});
    return 0;   // stack-protector check
}
RW_FUNCTION_NOVERIFY(0x4B8200, "hole::network::api::Client::processNativeSessionCreateCallback",
                     Client::processNativeSessionCreateCallback);

uint32_t Client::processSystemStatusUpdateCallback(Client *self, const Request *request, const SharedPtr *result) {
    // original frame 0x5a0: "" key +0x40, null connection / cipher +0x4c / +0x54, null document +0x5c, headers
    // +0x64, request's result +0x70, decrypted text +0x78, decrypt result +0x84, base64 buffer +0x8c, Cipher
    // +0x90, md5 +0xb4, mode+current +0xc0, key source +0xcc, server / current / mode +0xd8 / +0xe4 / +0xf0,
    // search predicate +0xfc, bound callback +0x100, its std::function +0x148, url StringBuffer +0x160, md5
    // bytes +0x564
    GuestFrame frame(0x5A0);
    const uint32_t me = addr(self);
    auto fallback = [&]() { rw::call(PARSE_RESPONSE, {me, request->action, addr(result)}); };
    uint32_t res = result->ptr;
    if (R(res) != 0 || R(res + 0x14) != JsonValue::kObject) {
        fallback();
        return 0;   // stack-protector check
    }
    const uint32_t doc = res + 8;
    if (R(member(doc, KEY_MAINTENANCE) + 12)) {
        fallback();
        return 0;   // stack-protector check
    }
    StdString *server = frame.at<StdString>(0xD8);
    StdString *current = frame.at<StdString>(0xE4);
    StdString *mode = frame.at<StdString>(0xF0);
    mode->w2 = 0;
    server->w1 = 0;
    server->w2 = 0;
    current->clear();
    mode->w0 = 0;
    mode->w1 = 0;
    server->w0 = 0;

    bool ok = false;
    uint32_t app = member(doc, KEY_APP_STATUS);
    if (R(app + 12) == JsonValue::kObject) {
        uint32_t v = member(app, KEY_MODE);
        if (isString(v)) {
            assignCString(addr(mode), R(v));
            v = member(app, KEY_CURRENT);
            if (isString(v)) {
                assignCString(addr(current), R(v));
                v = member(app, KEY_SERVER);
                if (isString(v)) {
                    assignCString(addr(server), R(v));
                    ok = true;
                }
            }
        }
    }
    if (ok) {
        bool requestToken = true;
        if (server->size() != 0) {
            // "server": base64(Cipher(md5(result key + current + mode)).encrypt(url)); an http... url replaces
            // serverURL
            uint32_t key = R(member(me + 8, KEY_STATUS_RESULT));
            initString(frame.addr(0xCC), key, hostStrlen(key));
            rw::call(string_plus, {frame.addr(0xC0), addr(current), addr(mode)});
            StdString *joined = frame.at<StdString>(0xC0);
            rw::call(string_append, {frame.addr(0xCC), joined->data(), joined->size()});
            if (joined->isLong()) guestDelete(R(frame.addr(0xC8)));
            StdString *source = frame.at<StdString>(0xCC);
            rw::call(md5_calculate, {frame.addr(0xB4), source->data(), source->size()});
            StdString *digest = frame.at<StdString>(0xB4);
            rw::call(guest::memcpy_, {frame.addr(0x564), digest->data(), 32});
            rw::call(Cipher_ctor, {frame.addr(0x90), frame.addr(0x564)});
            uint32_t *raw = frame.at<uint32_t>(0x8C);
            *raw = 0;
            uint32_t len = rw::call(base64Decode, {server->data(), server->size(), frame.addr(0x8C)});
            bool handled = false;
            if (*raw) {
                rw::call(Cipher_decrypt, {frame.addr(0x84), frame.addr(0x90), *raw, len});
                rw::call(free_, {*raw});
                uint32_t plain = R(frame.addr(0x84));
                if (!plain) {
                    handled = true;
                    fallback();
                } else {
                    initString(frame.addr(0x78), R(plain + 4), R(plain + 8));
                    StdString *text = frame.at<StdString>(0x78);
                    uint32_t n = text->size(), chars = text->data();
                    uint32_t at = 0xFFFFFFFFu;
                    if (n >= 4) {
                        *frame.at<uint32_t>(0xFC) = CHAR_EQUAL_INSENSITIVE;
                        uint32_t endChars = chars + n;
                        uint32_t hit = uint32_t(rw::call_exact(
                            SEARCH_PRED, {chars, endChars, KEY_HTTP, KEY_HTTP + 4, frame.addr(0xFC)}));
                        if (hit != endChars) at = hit - chars;
                    }
                    if (at == 0 && text->size() != 0 && me + 0x80 != frame.addr(0x78))
                        rw::call(guest::string_assign, {me + 0x80, text->data(), text->size()});
                    if (text->isLong()) guestDelete(R(frame.addr(0x80)));
                }
                release(R(frame.addr(0x88)));
            }
            requestToken = !handled;
            rw::call(SafeBuffer_dtor, {frame.addr(0x94)});
            if (digest->isLong()) guestDelete(R(frame.addr(0xBC)));
            if (source->isLong()) guestDelete(R(frame.addr(0xD4)));
        }
        if (requestToken) {
            // the login token request ("actions"[58])
            uint32_t base = serverURLChars(self);
            uint32_t path = R(member(R(member(me + 0x38, KEY_ACTIONS)) + ACTION_LOGIN_TOKEN_OFFSET, KEY_PATH));
            rw::call(StringBuffer_ctor, {frame.addr(0x160), FMT_SS, base, path});
            uint32_t method =
                R(member(R(member(me + 0x38, KEY_ACTIONS)) + ACTION_LOGIN_TOKEN_OFFSET, KEY_METHOD));
            uint32_t rest = rw::call(RESTClient_getInstance, {});
            rw::call(CREATE_HEADERS, {frame.addr(0x64), 1, 0});
            uint32_t *nullDoc = frame.at<uint32_t>(0x5C);
            nullDoc[0] = 0;
            nullDoc[1] = 0;
            buildBound(frame.addr(0x100), GOT_processLoginTokenCallback, me, request);
            rw::call(RESULT_FUNCTION_FROM_BIND, {frame.addr(0x148), frame.addr(0x100), 0});
            uint32_t *nulls = frame.at<uint32_t>(0x4C);
            nulls[0] = 0;
            nulls[1] = 0;
            nulls[2] = 0;
            nulls[3] = 0;
            initString(frame.addr(0x40), KEY_EMPTY, 0);
            restRequest(frame, {frame.addr(0x70), rest, frame.addr(0x160), method, frame.addr(0x64),
                                frame.addr(0x5C), frame.addr(0x148), frame.addr(0x54), 0, frame.addr(0x4C),
                                frame.addr(0x40)});
            release(R(frame.addr(0x74)));
            if (frame.at<StdString>(0x40)->isLong()) guestDelete(R(frame.addr(0x48)));
            release(R(frame.addr(0x50)));
            release(R(frame.addr(0x58)));
            destroyLocalFunction(frame.addr(0x148), R(frame.addr(0x158)));
            destroyBound(frame.addr(0x100));
            release(R(frame.addr(0x60)));
            rw::call(STRING_VECTOR_BASE_DTOR, {frame.addr(0x64)});
        }
    } else {
        fallback();
    }
    if (server->isLong()) guestDelete(server->w2);
    if (current->isLong()) guestDelete(current->w2);
    if (mode->isLong()) guestDelete(mode->w2);
    return 0;   // stack-protector check
}
RW_FUNCTION_NOVERIFY(0x4B82F8, "hole::network::api::Client::processSystemStatusUpdateCallback",
                     Client::processSystemStatusUpdateCallback);

uint32_t Client::processLoginTokenCallback(Client *self, const Request *request, const SharedPtr *result) {
    // original frame 0x150: "" key +0x44, null connection / cipher +0x50 / +0x58, headers +0x60, request's result
    // +0x6c, UUID +0x74, the body document +0x80, nativeToken +0x88, url +0x94, token / UUID values +0xa0 / +0xb0,
    // member name +0xc0, bound callback +0xd0, its std::function +0x118
    GuestFrame frame(0x150);
    const uint32_t me = addr(self);
    auto fallback = [&]() { rw::call(PARSE_RESPONSE, {me, request->action, addr(result)}); };
    uint32_t res = result->ptr;
    if (R(res) != 0 || R(res + 0x14) != JsonValue::kObject) {
        fallback();
        return 0;   // stack-protector check
    }
    const uint32_t doc = res + 8;
    if (R(member(doc, KEY_MAINTENANCE) + 12)) {
        fallback();
        return 0;   // stack-protector check
    }
    StdString *url = frame.at<StdString>(0x94);
    url->clear();
    uint32_t u = member(doc, KEY_URL);
    if (!isString(u)) {
        fallback();
    } else {
        assignCString(addr(url), R(u));
        StdString *token = frame.at<StdString>(0x88);
        token->clear();
        uint32_t t = member(doc, KEY_NATIVE_TOKEN);
        if (!isString(t)) {
            fallback();
        } else {
            assignCString(addr(token), R(t));
            // body = {"UUID": ..., "deviceType": 2, "nativeToken": ...}
            uint32_t body = rw::call(guest::operator_new, {0x30});
            W(body + 0xC, 0);
            rw::call(MP_STACK_CTOR, {body + 0x10, 0, 1024});
            W(body + 0x28, 0);
            W(body + 0x2C, 0);
            rw::call(DOC_SHARED_PTR_CTOR, {frame.addr(0x80), body, 0});
            uint32_t bodyDoc = R(frame.addr(0x80));
            uint32_t allocator = R(bodyDoc + 0x10);
            uint32_t target = 0;
            if (bodyDoc) {
                W(bodyDoc, 0);
                W(bodyDoc + 4, 0);
                W(bodyDoc + 8, 0);
                W(bodyDoc + 12, 3);
                target = R(frame.addr(0x80));
            }
            uint32_t app = rw::call(AppState_getInstance, {});
            rw::call(AppState_getUUIDString, {frame.addr(0x74), app});
            StdString *uuid = frame.at<StdString>(0x74);
            rw::call(VALUE_COPY_STRING, {frame.addr(0xB0), uuid->data(), allocator});
            uint32_t *name = frame.at<uint32_t>(0xC0);
            name[3] = STRING_NAME_FLAGS;
            name[0] = KEY_UUID;
            name[1] = 4;
            rw::call(ADD_MEMBER, {target, frame.addr(0xC0), frame.addr(0xB0), allocator});
            if (uuid->isLong()) guestDelete(R(frame.addr(0x7C)));
            rw::call(ADD_MEMBER_INT, {R(frame.addr(0x80)), KEY_DEVICE_TYPE, 2, allocator});
            uint32_t bodyDoc2 = R(frame.addr(0x80));
            rw::call(VALUE_COPY_STRING, {frame.addr(0xA0), token->data(), allocator});
            name[3] = STRING_NAME_FLAGS;
            name[0] = KEY_NATIVE_TOKEN;
            name[1] = 11;
            rw::call(ADD_MEMBER, {bodyDoc2, frame.addr(0xC0), frame.addr(0xA0), allocator});

            uint32_t rest = rw::call(RESTClient_getInstance, {});
            uint32_t urlChars = url->data();
            uint32_t *headers = frame.at<uint32_t>(0x60);
            headers[0] = 0;
            headers[1] = 0;
            headers[2] = 0;
            buildBound(frame.addr(0xD0), GOT_processNativeSessionCreateCallback, me, request);
            rw::call(RESULT_FUNCTION_FROM_BIND, {frame.addr(0x118), frame.addr(0xD0), 0});
            uint32_t *nulls = frame.at<uint32_t>(0x50);
            nulls[0] = 0;
            nulls[1] = 0;
            nulls[2] = 0;
            nulls[3] = 0;
            initString(frame.addr(0x44), KEY_EMPTY, 0);
            restRequest(frame, {frame.addr(0x6C), rest, urlChars, RESTClient::POST, frame.addr(0x60),
                                frame.addr(0x80), frame.addr(0x118), frame.addr(0x58), 0, frame.addr(0x50),
                                frame.addr(0x44)});
            release(R(frame.addr(0x70)));
            if (frame.at<StdString>(0x44)->isLong()) guestDelete(R(frame.addr(0x4C)));
            release(R(frame.addr(0x54)));
            release(R(frame.addr(0x5C)));
            destroyLocalFunction(frame.addr(0x118), R(frame.addr(0x128)));
            destroyBound(frame.addr(0xD0));
            rw::call(STRING_VECTOR_BASE_DTOR, {frame.addr(0x60)});
            release(R(frame.addr(0x84)));
        }
        if (token->isLong()) guestDelete(token->w2);
    }
    if (url->isLong()) guestDelete(url->w2);
    return 0;   // stack-protector check
}
RW_FUNCTION_NOVERIFY(0x4B8ABC, "hole::network::api::Client::processLoginTokenCallback",
                     Client::processLoginTokenCallback);

}  // namespace hole::network::api
