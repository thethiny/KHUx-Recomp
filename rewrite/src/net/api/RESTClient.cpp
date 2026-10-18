#include "RESTClient.hpp"

namespace hole::network {

using api::GuestFrame;
using api::JsonValue;
using api::SharedPtr;
using api::StdString;
using api::StdVector;
namespace g = api::guest;

namespace {
// libc / libc++ / cocos2d / curl
constexpr uint32_t guard_load = 0xCD8411;         // atomic load of a static-init guard
constexpr uint32_t guard_acquire = 0xC98BC5;      // __cxa_guard_acquire
constexpr uint32_t guard_release = 0xC98C85;      // __cxa_guard_release
constexpr uint32_t cxa_atexit = 0x40DCD8;         // __cxa_atexit (import)
constexpr uint32_t GOT_RESTClient_D2 = 0xFD613C;
constexpr uint32_t GOT_dso_handle = 0xFD604C;
constexpr uint32_t malloc_ = 0x40FF58;
constexpr uint32_t free_ = 0x40FF64;
constexpr uint32_t pthread_create_ = 0x410900;
constexpr uint32_t throw_system_error = 0xC97965;
constexpr uint32_t thread_detach = 0xC97ADD;
constexpr uint32_t thread_dtor = 0xC97A3D;
constexpr uint32_t add_weak = 0xC8DBC3;
constexpr uint32_t release_weak = 0xC8DC13;
constexpr uint32_t mutex_dtor = 0xC8DDFD;
constexpr uint32_t curl_easy_init = 0x95E778;     // ARM
constexpr uint32_t curl_easy_cleanup = 0x95EAC4;  // ARM
constexpr uint32_t curl_easy_escape = 0x9547CC;   // ARM
constexpr uint32_t curl_easy_unescape = 0x954C78; // ARM
constexpr uint32_t curl_free = 0x954CD8;          // ARM
constexpr uint32_t HttpClient_getInstance = 0x7E3FCD;
constexpr uint32_t HttpClient_send = 0x7E4409;
constexpr uint32_t HttpClient_enableCookies = 0x7E4045;
constexpr uint32_t HttpClient_clearCookies = 0x7E41D1;
constexpr uint32_t Ref_ctor = 0x8FD025;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t GOT_HttpRequest_vtable = 0xFD6140;   // + 8
constexpr uint32_t STACK_PUSH_CHAR = 0x49BBE5;    // rapidjson::internal::Stack<CrtAllocator>::Push<char>(n)
constexpr uint32_t STACK_DTOR = 0x49D84B;         // Stack<CrtAllocator>::~Stack
constexpr uint32_t MP_STACK_CTOR = 0x49A659;      // Stack<MemoryPoolAllocator>::Stack(allocator*, capacity)
constexpr uint32_t MP_STACK_DTOR = 0x49A5FF;
constexpr uint32_t VALUE_ACCEPT = 0x498479;       // GenericValue::Accept(Writer<GenericStringBuffer>&)
constexpr uint32_t PARSE_STREAM = 0x49BD61;       // GenericDocument::ParseStream<0>(StringStream&)
constexpr uint32_t StringBuffer_ctor = 0x49140D;  // StringBuffer::StringBuffer(const char *fmt, ...)
constexpr uint32_t KEY_PERCENT_D = 0xDF86DE;      // "%d"
constexpr uint32_t KEY_V_EQUALS = 0xDFA2B7;       // "v="
constexpr uint32_t KEY_THREAD_FAILED = 0xDFA2BA;  // "thread constructor failed"
constexpr uint32_t string_append = 0x47AC89;      // std::string::append(const char*, size_t)
constexpr uint32_t Cipher_encrypt = 0x4E06D9;     // -> shared_ptr<{?, data, size}> (sret)
constexpr uint32_t Cipher_decrypt = 0x4E06F3;
constexpr uint32_t base64Encode = 0x905B2D;
constexpr uint32_t base64Decode = 0x905A89;
// hole::network::RESTClient's own template instances
constexpr uint32_t RESULT_FUNCTION_ASSIGN = 0x497DA1;    // function<void(const shared_ptr<Result>&)>::operator=
constexpr uint32_t RESULT_FUNCTION_CALL = 0x49911D;      // ... ::operator()
constexpr uint32_t HTTP_FUNCTION_ASSIGN = 0x49D419;      // function<void(HttpClient*, HttpResponse*)>::operator=
constexpr uint32_t PROGRESS_FUNCTION_ASSIGN = 0x49D279;  // function<int(double, double, double, double)>::operator=
constexpr uint32_t GOT_ON_RESPOND_BINDER_VTABLE = 0xFD6144;   // + 8: __func<bind(&onRespond, this, _1, _2)>
constexpr uint32_t GOT_ON_RESPOND = 0xFD6148;            // &RESTClient::onRespond
constexpr uint32_t PROGRESS_BINDER_VTABLE = 0xF5562C;    // __func<lambda(weak_ptr<Connection>)>
constexpr uint32_t CHAR_VECTOR_ASSIGN = 0x49D725;        // vector<char>::assign(const char*, const char*)
constexpr uint32_t STRING_VECTOR_ASSIGN = 0x49D101;      // vector<string>::assign(string*, string*)
constexpr uint32_t CONNECTION_MAKE_SHARED = 0x49B5E5;
constexpr uint32_t CONNECTION_INIT = 0x497B81;
constexpr uint32_t CONNECTION_SET_KEY = 0x498A1D;        // the connection's key string = const string&
constexpr uint32_t RESULT_MAKE_SHARED = 0x49A511;
constexpr uint32_t QUEUE_PUSH_BACK = 0x498A49;           // deque<shared_ptr<Connection>>::push_back(const&)
constexpr uint32_t QUEUE_POP_FRONT = 0x499161;
constexpr uint32_t QUEUE_ERASE = 0x498B89;               // deque::erase(const_iterator{map, ptr}) (sret)
constexpr uint32_t QUEUE_TO_VECTOR = 0x498F6D;           // vector<shared_ptr<Connection>>(deque iterator range)
constexpr uint32_t CONNECTION_VECTOR_BASE_DTOR = 0x498FDF;
constexpr uint32_t QUEUE_BASE_DTOR = 0x497F65;
constexpr uint32_t RESULTS_BASE_DTOR = 0x497F39;
constexpr uint32_t APPEND_QUERY = 0x498689;
constexpr uint32_t CANCEL = 0x498AB9;
constexpr uint32_t SEARCH_INSENSITIVE = 0x49A35D;        // std::__search with the case-folding predicate
constexpr uint32_t RESPONSE_THREAD = 0x4991BD;           // the decode worker's entry

constexpr uint32_t BLOCK_MASK = 0x1FFFFFC;               // (index >> 7) & mask = the block's map offset
constexpr uint32_t BLOCK_SIZE = 512;                     // shared_ptrs per deque block

inline uint32_t R(uint32_t a) { return rw::load<uint32_t>(a); }
inline uint8_t rd8(uint32_t a) { return rw::load<uint8_t>(a); }
inline void W(uint32_t a, uint32_t v) { rw::save<uint32_t>(a, v); }
inline void wr8(uint32_t a, uint8_t v) { rw::store8(rw::host_ptr<uint8_t>(a), v); }
inline uint32_t hostStrlen(uint32_t s) { return uint32_t(std::strlen(rw::host_ptr<char>(s))); }

// rapidjson::internal::Stack<CrtAllocator>::Push<char>(1) = c
void pushChar(uint32_t stack, uint8_t c) { wr8(rw::call(STACK_PUSH_CHAR, {stack, 1}), c); }
void pushAll(uint32_t stack, uint32_t s) {
    for (uint8_t c; (c = rd8(s)) != 0; s++) pushChar(stack, c);
}
// Stack<CrtAllocator> in a frame slot (the constructor, inlined): {allocator, ownAllocator, stack, top, end, cap}
void initCharStack(uint32_t *st) {
    st[3] = 0;
    st[2] = 0;
    st[1] = 0;
    st[0] = 0;
    st[5] = 256;
    uint32_t allocator = rw::call(g::operator_new, {1});
    st[0] = allocator;
    st[1] = allocator;
    uint32_t chars = rw::call(malloc_, {256});
    st[2] = chars;
    st[3] = chars;
    st[4] = chars + 256;
}
// Push<char>(1) = 0; --top: the stack's characters as a C string
uint32_t finishString(uint32_t stack, uint32_t *st) {
    pushChar(stack, 0);
    st[3] = st[3] - 1;
    return st[2];
}
// destroy a std::function held in a frame slot (__f_ == the buffer: destroy() in place, else destroy_deallocate())
void destroyLocalFunction(uint32_t buffer, uint32_t f) {
    if (f == buffer) rw::vcall(rw::host_ptr<uint32_t>(buffer), 4, {buffer});
    else if (f) rw::vcall(rw::host_ptr<uint32_t>(f), 5, {f});
}
uint32_t sendNow(uint32_t request) {
    uint32_t client = rw::call(HttpClient_getInstance, {});
    return rw::call(HttpClient_send, {client, request});
}
}  // namespace

// ---------------------------------------------------------------------------------------------- Connection
uint32_t RESTClient::Connection::init(Connection *self, uint32_t url, uint32_t method, uint32_t query,
                                      uint32_t callback, const SharedPtr *cipher) {
    GuestFrame frame(0x48);                   // original frame: the GET url Stack at +0x18
    const uint32_t me = rw::guest_addr(self);
    rw::call(RESULT_FUNCTION_ASSIGN, {me + 8, callback});
    uint32_t cp = cipher->ptr, cc = cipher->ctrl;
    if (cc) rw::call(g::shared_add, {cc});
    self->cipher.ptr = cp;
    uint32_t old = self->cipher.ctrl;
    self->cipher.ctrl = cc;
    if (old) rw::call(g::shared_release, {old});

    // new cocos2d::network::HttpRequest (its constructor, inlined)
    uint32_t req = rw::call(g::operator_new, {0x88});
    rw::call(Ref_ctor, {req});
    W(req, R(GOT_HttpRequest_vtable) + 8);
    W(req + 0x64, 0);
    W(req + 0x68, 0);
    W(req + 0x6C, 0);
    rw::call(g::memset_, {req + 0x18, 0x24, 0});
    W(req + 0x14, 4);                         // type: unknown
    wr8(req + 0x19, 0);
    wr8(req + 0x18, 0);
    wr8(req + 0x31, 0);
    wr8(req + 0x30, 0);
    W(req + 0x3C, 0);
    W(req + 0x44, 0);
    W(req + 0x40, 0);
    W(req + 0x58, 0);
    W(req + 0x60, 0);
    W(req + 0x80, 0);
    self->request = req;

    if (method == GET) {
        W(req + 0x14, 0);
        uint32_t *st = frame.at<uint32_t>(0x18);
        const uint32_t stack = frame.addr(0x18);
        initCharStack(st);
        pushAll(stack, url);
        if (rd8(query)) {
            pushChar(stack, '?');
            pushAll(stack, query);
        }
        uint32_t r = self->request;
        uint32_t s = finishString(stack, st);
        rw::call(g::string_assign, {r + 0x18, s, hostStrlen(s)});    // url
        rw::call(STACK_DTOR, {stack});
        return 0;   // stack-protector check
    }
    if (method <= 3) W(req + 0x14, method);
    rw::call(g::string_assign, {req + 0x18, url, hostStrlen(url)});
    uint32_t r = self->request;
    rw::call(CHAR_VECTOR_ASSIGN, {r + 0x24, query, query + hostStrlen(query)});   // body
    return 0;   // stack-protector check
}
RW_FUNCTION(0x497B80, "hole::network::RESTClient::Connection::init", RESTClient::Connection::init);

RESTClient::Connection *RESTClient::Connection::destroy(Connection *self) {
    if (self->request) rw::call(Ref_release, {self->request});
    if (self->response) rw::call(Ref_release, {self->response});
    if (self->key.isLong()) api::guestDelete(self->key.w2);
    if (self->cipher.ctrl) rw::call(g::shared_release, {self->cipher.ctrl});
    destroyLocalFunction(rw::guest_addr(self) + 8, self->callbackF);
    return self;
}
RW_FUNCTION(0x497E10, "hole::network::RESTClient::Connection::~Connection", RESTClient::Connection::destroy);

uint32_t RESTClient::Connection::start(Connection *self) { return sendNow(self->request); }
// queues the request on the network thread: not run twice by the verifier
RW_FUNCTION_NOVERIFY(0x497EAA, "hole::network::RESTClient::Connection::start", RESTClient::Connection::start);

// ---------------------------------------------------------------------------------------------- RESTClient
uint32_t RESTClient::getInstance() {
    if (!(rw::call(guard_load, {GUARD, 0, 0}) & 1) && rw::call(guard_acquire, {GUARD})) {
        rw::call(0x497EBD, {INSTANCE});       // RESTClient::RESTClient
        rw::call(cxa_atexit, {R(GOT_RESTClient_D2), INSTANCE, R(GOT_dso_handle)});
        rw::call(guard_release, {GUARD});
    }
    return INSTANCE;
}
// registers an atexit handler and makes the HttpClient singleton: not run twice by the verifier
RW_FUNCTION_NOVERIFY(0x497AE8, "hole::network::RESTClient::getInstance", RESTClient::getInstance);

RESTClient *RESTClient::construct(RESTClient *self) {
    const uint32_t me = rw::guest_addr(self);
    rw::call(g::memset_, {me + 4, 0x18, 0});
    self->mutex = 0;
    rw::call(g::memset_, {me + 0x20, 0x18, 0});
    self->curl = rw::call(curl_easy_init, {});
    uint32_t client = rw::call(HttpClient_getInstance, {});
    rw::call(HttpClient_enableCookies, {client, 0});
    client = rw::call(HttpClient_getInstance, {});
    rw::call(HttpClient_clearCookies, {client});
    return self;
}
RW_FUNCTION_NOVERIFY(0x497EBC, "hole::network::RESTClient::RESTClient", RESTClient::construct);

RESTClient *RESTClient::destroy(RESTClient *self) {
    const uint32_t me = rw::guest_addr(self);
    rw::call(curl_easy_cleanup, {self->curl});
    rw::call(RESULTS_BASE_DTOR, {me + 0x20});
    rw::call(mutex_dtor, {me + 0x1C});
    rw::call(QUEUE_BASE_DTOR, {me + 4});
    return self;
}
RW_FUNCTION(0x497F90, "hole::network::RESTClient::~RESTClient", RESTClient::destroy);

uint32_t RESTClient::request(SharedPtr *out, RESTClient *self, uint32_t url, uint32_t method, uint32_t headers,
                             const SharedPtr *document, uint32_t callback, const SharedPtr *cipher,
                             uint32_t cancelable, SharedPtr *connection, uint32_t key) {
    // original frame 0x100: the made Connection at +0x40, the query std::string at +0x48, a Writer at +0x54 (plain
    // body) or +0x78 (encrypted), Cipher::encrypt's result at +0x70, the body Stack at +0x94, base64 at +0xac,
    // the two bound std::functions at +0xb0 and +0xc8
    GuestFrame frame(0x100);
    const uint32_t me = rw::guest_addr(self);
    uint32_t *b64 = frame.at<uint32_t>(0xAC);
    *b64 = 0;
    uint32_t cipherPtr = cipher->ptr;
    uint32_t *st = frame.at<uint32_t>(0x94);
    const uint32_t stack = frame.addr(0x94);
    initCharStack(st);

    uint32_t query = 0;
    bool freeEscaped = false;
    bool proceed = true;
    if (!document->ptr) {
        query = finishString(stack, st);
    } else if (!cipherPtr) {
        if (method != GET) {
            // the document as JSON text
            *frame.at<uint32_t>(0x54) = stack;
            rw::call(MP_STACK_CTOR, {frame.addr(0x58), 0, 256});
            rw::call(VALUE_ACCEPT, {document->ptr, frame.addr(0x54)});
            query = finishString(stack, st);
            rw::call(MP_STACK_DTOR, {frame.addr(0x58)});
        } else {
            // name=value&name=value
            const JsonValue *doc = rw::host_ptr<JsonValue>(document->ptr);
            if (doc->Size()) {
                bool first = true;
                for (uint32_t m = doc->w0;;) {
                    if (!first) pushChar(stack, '&');
                    rw::call(APPEND_QUERY, {self->curl, m, stack, 1});
                    first = false;
                    m += 32;
                    doc = rw::host_ptr<JsonValue>(document->ptr);
                    if (m == doc->w0 + 32 * doc->w1) break;
                }
            }
            query = finishString(stack, st);
        }
    } else {
        // the document as JSON text, encrypted, base64, url-escaped
        *frame.at<uint32_t>(0x78) = stack;
        uint32_t writerStack = rw::call(MP_STACK_CTOR, {frame.addr(0x7C), 0, 256});
        rw::call(VALUE_ACCEPT, {document->ptr, frame.addr(0x78)});
        uint32_t c = cipher->ptr;
        freeEscaped = true;
        pushChar(stack, 0);
        st[3] = st[3] - 1;
        rw::call(Cipher_encrypt, {frame.addr(0x70), c, st[2], st[3] - st[2]});
        proceed = false;
        SharedPtr *enc = frame.at<SharedPtr>(0x70);
        if (enc->ptr) {
            *b64 = 0;
            rw::call(base64Encode, {R(enc->ptr + 4), R(enc->ptr + 8), frame.addr(0xAC)});
            if (*b64) {
                uint32_t len = hostStrlen(*b64);
                query = rw::call(curl_easy_escape, {self->curl, *b64, len});
                proceed = true;
            }
        }
        if (enc->ctrl) rw::call(g::shared_release, {enc->ctrl});
        rw::call(MP_STACK_DTOR, {writerStack});
    }

    if (proceed && query) {
        StdString *qs = frame.at<StdString>(0x48);
        qs->clear();
        if (rd8(query)) {
            if (cipherPtr) rw::call(g::string_assign, {frame.addr(0x48), KEY_V_EQUALS, 2});
            rw::call(string_append, {frame.addr(0x48), query, hostStrlen(query)});
        }
        if (!connection->ptr) {
            rw::call(CONNECTION_MAKE_SHARED, {frame.addr(0x40)});
            SharedPtr *made = frame.at<SharedPtr>(0x40);
            uint32_t p = made->ptr, c = made->ctrl;
            made->reset();
            connection->ptr = p;
            uint32_t old = connection->ctrl;
            connection->ctrl = c;
            if (old) rw::call(g::shared_release, {old});
        }
        uint32_t conn = connection->ptr;
        rw::call_exact(CONNECTION_INIT, {conn, url, method, qs->data(), callback, rw::guest_addr(cipher)});
        wr8(connection->ptr + 33, uint8_t(cancelable));

        // request->setResponseCallback(std::bind(&RESTClient::onRespond, this, _1, _2))
        uint32_t req = R(connection->ptr);
        uint32_t *bound = frame.at<uint32_t>(0xC8);
        bound[4] = frame.addr(0xC8);
        bound[0] = R(GOT_ON_RESPOND_BINDER_VTABLE) + 8;
        bound[1] = R(GOT_ON_RESPOND);
        bound[2] = 0;
        bound[3] = me;
        rw::call(HTTP_FUNCTION_ASSIGN, {req + 0x48, frame.addr(0xC8)});
        destroyLocalFunction(frame.addr(0xC8), bound[4]);

        // request's progress callback: a lambda holding a weak_ptr to the connection
        uint32_t cp = connection->ptr, cc = connection->ctrl;
        uint32_t req2;
        if (cc) {
            rw::call(add_weak, {cc});
            req2 = R(connection->ptr);
            rw::call(add_weak, {cc});
        } else {
            req2 = R(cp);
        }
        uint32_t *progress = frame.at<uint32_t>(0xB0);
        progress[4] = frame.addr(0xB0);
        progress[0] = PROGRESS_BINDER_VTABLE;
        progress[1] = cp;
        progress[2] = cc;
        rw::call(PROGRESS_FUNCTION_ASSIGN, {req2 + 0x70, frame.addr(0xB0)});
        destroyLocalFunction(frame.addr(0xB0), progress[4]);

        uint32_t headerVector = R(connection->ptr) + 0x64;
        if (headerVector != headers) rw::call(STRING_VECTOR_ASSIGN, {headerVector, R(headers), R(headers + 4)});
        rw::call(CONNECTION_SET_KEY, {connection->ptr + 0x34, key});
        rw::call(QUEUE_PUSH_BACK, {me + 4, rw::guest_addr(connection)});
        if (self->queue.size == 1) sendNow(R(connection->ptr));
        if (freeEscaped) rw::call(curl_free, {query});
        if (cc) rw::call(release_weak, {cc});
        if (qs->isLong()) api::guestDelete(qs->w2);
    }
    rw::call(free_, {*b64});
    out->ptr = connection->ptr.get();
    out->ctrl = connection->ctrl.get();
    connection->reset();
    rw::call(STACK_DTOR, {stack});
    return 0;   // stack-protector check
}
// sends the request: not run twice by the verifier
RW_FUNCTION_NOVERIFY(0x497FE4, "hole::network::RESTClient::request", RESTClient::request);

uint32_t RESTClient::onRespond(RESTClient *self, uint32_t, uint32_t response) {
    GuestFrame frame(0x48);                   // original frame: the thread at +0x24, the Result at +0x28
    const uint32_t me = rw::guest_addr(self);
    uint32_t start = self->queue.start;
    uint32_t block = R(self->queue.mapBegin + ((start >> 7) & BLOCK_MASK));
    rw::call(RESULT_MAKE_SHARED, {frame.addr(0x28)});
    SharedPtr *result = frame.at<SharedPtr>(0x28);
    W(result->ptr, 0);
    W(result->ptr + 4, R(response + 0x34));   // the HTTP response code
    uint32_t front = block + 8 * (start & (BLOCK_SIZE - 1));
    if (rd8(response + 0x18)) {                // succeeded: decode on a worker thread
        rw::call(Ref_retain, {response});
        uint32_t rp = result->ptr, rc = result->ctrl;
        if (rc) rw::call(g::shared_add, {rc});
        uint32_t cp = R(front), cc = R(front + 4);
        if (cc) rw::call(g::shared_add, {cc});
        uint32_t job = rw::call(g::operator_new, {0x18});
        W(job, rp);
        W(job + 4, rc);
        W(job + 8, response);
        W(job + 12, cp);
        W(job + 16, cc);
        W(job + 20, me);
        uint32_t err = rw::call(pthread_create_, {frame.addr(0x24), 0, RESPONSE_THREAD, job});
        if (err) {
            rw::call(throw_system_error, {err, KEY_THREAD_FAILED});   // does not return
        } else {
            rw::call(thread_detach, {frame.addr(0x24)});
            rw::call(thread_dtor, {frame.addr(0x24)});
        }
    } else {
        uint32_t conn = R(front);
        if (!rd8(conn + 0x20)) {               // not cancelled: callback(result) with status 1 (failed)
            W(result->ptr, 1);
            rw::call(RESULT_FUNCTION_CALL, {conn + 8, frame.addr(0x28)});
        }
    }
    rw::call(QUEUE_POP_FRONT, {me + 4});
    if (self->queue.size) {
        uint32_t s = self->queue.start;
        uint32_t next = R(R(self->queue.mapBegin + ((s >> 7) & BLOCK_MASK)) + 8 * (s & (BLOCK_SIZE - 1)));
        sendNow(R(next));
    }
    if (result->ctrl) rw::call(g::shared_release, {result->ctrl});
    return 0;   // stack-protector check
}
// starts a thread and sends the next request: not run twice by the verifier
RW_FUNCTION_NOVERIFY(0x498818, "hole::network::RESTClient::onRespond", RESTClient::onRespond);

uint32_t RESTClient::cancel(RESTClient *self, const SharedPtr *connection) {
    GuestFrame frame(0x38);                   // original frame: erase's result at +0x18
    uint32_t conn = connection->ptr;
    if (rd8(conn + 0x20)) return 0;            // stack-protector check
    uint32_t start = self->queue.start;
    uint32_t mapBegin = self->queue.mapBegin, mapEnd = self->queue.mapEnd;
    uint32_t map = mapBegin + ((start >> 7) & BLOCK_MASK);
    uint32_t first = mapEnd == mapBegin ? 0 : R(map) + 8 * (start & (BLOCK_SIZE - 1));
    uint32_t endIndex = start + self->queue.size;
    uint32_t p = first;
    for (;;) {
        uint32_t last = mapEnd == mapBegin ? 0 : R(mapBegin + 4 * (endIndex >> 9)) + 8 * (endIndex & (BLOCK_SIZE - 1));
        if (p == last) break;
        if (R(p) == conn) {
            // the front one is in flight: only marked
            if (p != first) rw::call(QUEUE_ERASE, {frame.addr(0x18), rw::guest_addr(self) + 4, map, p});
            break;
        }
        p += 8;
        if (p - R(map) == 8 * BLOCK_SIZE) {
            p = R(map + 4);
            map += 4;
        }
    }
    wr8(conn + 0x20, 1);
    return 0;   // stack-protector check
}
RW_FUNCTION(0x498AB8, "hole::network::RESTClient::cancel", RESTClient::cancel);

uint32_t RESTClient::cancelAll(RESTClient *self) {
    GuestFrame frame(0x38);                   // original frame: the shared_ptr copy at +0xc, the vector at +0x14
    uint32_t start = self->queue.start;
    uint32_t mapBegin = self->queue.mapBegin, mapEnd = self->queue.mapEnd;
    uint32_t map = mapBegin + ((start >> 7) & BLOCK_MASK);
    uint32_t endIndex = start + self->queue.size;
    uint32_t endMap = mapBegin + ((endIndex >> 7) & BLOCK_MASK);
    uint32_t first = 0, last = 0;
    if (mapEnd != mapBegin) {
        last = R(endMap) + 8 * (endIndex & (BLOCK_SIZE - 1));
        first = R(map) + 8 * (start & (BLOCK_SIZE - 1));
    }
    rw::call_exact(QUEUE_TO_VECTOR, {frame.addr(0x14), map, first, endMap, last});
    StdVector *copy = frame.at<StdVector>(0x14);
    SharedPtr *arg = frame.at<SharedPtr>(0xC);
    const uint32_t me = rw::guest_addr(self);
    for (uint32_t e = copy->begin, end = copy->end; e != end; e += 8) {
        uint32_t conn = R(e);
        if (!rd8(conn + 0x21)) continue;
        arg->ptr = conn;
        uint32_t cc = R(e + 4);
        arg->ctrl = cc;
        if (cc) {
            rw::call(g::shared_add, {cc});
            rw::call(CANCEL, {me, frame.addr(0xC)});
            rw::call(g::shared_release, {cc});
        } else {
            rw::call(CANCEL, {me, frame.addr(0xC)});
        }
    }
    rw::call(CONNECTION_VECTOR_BASE_DTOR, {frame.addr(0x14)});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x498E8C, "hole::network::RESTClient::cancelAll", RESTClient::cancelAll);

uint32_t RESTClient::decodeEncodedJSONData(const RESTClient *self, const StdVector *data, uint32_t document,
                                           const SharedPtr *cipher) {
    // original frame 0x48: the text at +0xc, the plain data at +0x18, base64's buffer at +0x20, the unescaped
    // length at +0x24, the StringStream at +0x28
    GuestFrame frame(0x48);
    uint32_t begin = data->begin;
    uint32_t unescaped = rw::call(curl_easy_unescape, {self->curl, begin, data->end - begin, frame.addr(0x24)});
    if (!unescaped) return 0;                 // stack-protector check
    uint32_t *raw = frame.at<uint32_t>(0x20);
    *raw = 0;
    uint32_t len = rw::call(base64Decode, {unescaped, *frame.at<uint32_t>(0x24), frame.addr(0x20)});
    if (*raw) {
        rw::call(Cipher_decrypt, {frame.addr(0x18), cipher->ptr, *raw, len});
        SharedPtr *plain = frame.at<SharedPtr>(0x18);
        if (plain->ptr) {
            rw::call(g::string_init, {frame.addr(0xC), R(plain->ptr + 4), R(plain->ptr + 8)});
            StdString *text = frame.at<StdString>(0xC);
            uint32_t chars = text->data();
            uint32_t *streamSlot = frame.at<uint32_t>(0x28);
            streamSlot[0] = chars;            // src_
            streamSlot[1] = chars;            // head_
            rw::call(PARSE_STREAM, {document, frame.addr(0x28)});
            if (text->isLong()) api::guestDelete(text->w2);
        }
        rw::call(free_, {*raw});
        if (plain->ctrl) rw::call(g::shared_release, {plain->ctrl});
    }
    rw::call(curl_free, {unescaped});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x499020, "hole::network::RESTClient::decodeEncodedJSONData", RESTClient::decodeEncodedJSONData);

// ---------------------------------------------------------------------------------------------- free functions
uint32_t AppendQuery(uint32_t curl, uint32_t member, uint32_t stack, uint32_t escape) {
    GuestFrame frame(0x430);                  // original frame: a 1 KB StringBuffer at +0x14
    auto pushEscaped = [&](uint32_t s) {
        uint32_t e = rw::call(curl_easy_escape, {curl, s, hostStrlen(s)});
        pushAll(stack, e);
        rw::call(curl_free, {e});
    };
    uint32_t name = R(member);
    if (escape == 1) pushEscaped(name);
    else pushAll(stack, name);
    pushChar(stack, '=');
    uint32_t flags = R(member + 16 + 12);
    if (flags & JsonValue::kStringFlag) {
        uint32_t value = R(member + 16);
        if (escape == 1) pushEscaped(value);
        else pushAll(stack, value);
    } else if (flags & JsonValue::kIntFlag) {
        uint32_t text = rw::call(StringBuffer_ctor, {frame.addr(0x14), KEY_PERCENT_D, R(member + 16)});
        uint8_t c = rd8(text);
        for (uint32_t s = frame.addr(0x15); c; c = rd8(s++)) pushChar(stack, c);
    }
    return 0;   // stack-protector check
}
RW_FUNCTION(0x498688, "hole::network::AppendQuery", AppendQuery);

uint32_t FindSubstringInsensitive(const StdString *haystack, const StdString *needle) {
    GuestFrame frame(0x28);                   // original frame: the (empty) predicate at +0xc
    uint32_t first = haystack->data(), last = first + haystack->size();
    uint32_t sub = needle->data(), subLast = sub + needle->size();
    uint32_t hit = uint32_t(rw::call_exact(SEARCH_INSENSITIVE, {first, last, sub, subLast, frame.addr(0xC)}));
    return hit == last ? 0xFFFFFFFFu : hit - first;
}
RW_FUNCTION(0x49977C, "hole::network::FindSubstringInsensitive", FindSubstringInsensitive);

// ---------------------------------------------------------------------------------------------- the decode worker
namespace {
constexpr uint32_t THREAD_LOCAL_DATA = 0xC97C4D;      // std::__thread_local_data()
constexpr uint32_t THREAD_STRUCT_CTOR = 0xC97ED9;     // std::__thread_struct::__thread_struct
constexpr uint32_t THREAD_SPECIFIC_RESET = 0x499749;  // __thread_specific_ptr<__thread_struct>::reset
constexpr uint32_t STRING_INIT_RANGE = 0x49A4A7;      // std::string::__init(char* first, char* last)
constexpr uint32_t MD5_CALCULATE = 0x4E0A89;          // md5::calculate (sret std::string, data, size)
constexpr uint32_t MEMCMP = 0x40EF2C, MEMCPY = 0x40E164, ATOI = 0x410A14;   // imports
constexpr uint32_t SEARCH_CASEFOLD = 0x49A42B;        // std::__search<bool(*&)(char, char)>
constexpr uint32_t CASEFOLD_EQUAL = 0x49A419;         // sub_49A418: the case-insensitive char comparison
constexpr uint32_t FIND_SUBSTRING_INSENSITIVE = 0x49977D;
constexpr uint32_t ADD_MEMBER_INT = 0x499801;         // GenericValue::AddMember(name, int, allocator)
constexpr uint32_t STD_MUTEX_LOCK = 0xC8DE15, STD_MUTEX_UNLOCK = 0xC8DE59;
constexpr uint32_t RESULTS_PUSH_BACK = 0x499875;      // deque<pair<shared_ptr<Connection>, shared_ptr<Result>>>
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t Scheduler_performFunctionInCocosThread = 0x8FF883;
constexpr uint32_t VT_DELIVER_RESULTS = 0xF55600;     // the cocos-thread lambda [client]
constexpr uint32_t S_APPLICATION_JSON = 0xDFA2D4, S_TEXT_JAVASCRIPT = 0xDFA2E5;
constexpr uint32_t S_ENCODED_JSON = 0xDFA2F5, S_OCTET_STREAM = 0xDFA30E;
constexpr uint32_t S_INTERVAL_HEADER = 0xDFA327;      // "Request-Interval-Order:" (23 chars)
constexpr uint32_t S_INTERVAL_MEMBER = 0xDFA33F;      // "__request_interval_order__"
constexpr uint32_t INTERVAL_HEADER_LENGTH = 23;
// HttpResponse: the body vector<char> +0x1C, the headers vector<char> +0x28. Result: status +0, document +8 (its
// type flags +0xC, allocator +0x10).
constexpr uint32_t RESPONSE_BODY = 0x1C, RESPONSE_HEADERS = 0x28;
constexpr uint32_t RESULT_DOCUMENT = 8;
constexpr uint32_t JSON_OBJECT = 3;

const StdString &str(uint32_t addr) { return *rw::host_ptr<StdString>(addr); }

// the headers contain `needle` (n chars), case-insensitively (the predicate stored at `predicate` and passed by
// reference)
bool headersContain(uint32_t headers, uint32_t needle, uint32_t n, uint32_t predicate) {
    const uint32_t size = str(headers).size(), first = str(headers).data();
    if (size < n) return false;
    W(predicate, CASEFOLD_EQUAL);
    const uint32_t last = first + size;
    const uint32_t hit = uint32_t(rw::call_exact(SEARCH_CASEFOLD, {first, last, needle, needle + n, predicate}));
    return hit != last && hit - first != 0xFFFFFFFFu;
}
}  // namespace

// sub_4991BC: the decode worker's std::thread entry. job (new, 0x18 bytes) = {result, its control block, the
// HttpResponse, connection, its control block, the client}: check the body's MD5 against the connection's key
// (when it has one), parse it (JSON) or decrypt it (encoded JSON, when the connection has a cipher) into the
// result's document (else status 2), add the server's Request-Interval-Order, then queue {connection, result} for
// the cocos thread.
uint32_t ResponseThread(uint32_t job) {
    GuestFrame frame(0x9C);
    rw::call(THREAD_SPECIFIC_RESET, {rw::call(THREAD_LOCAL_DATA, {}),
                                     [] {
                                         const uint32_t t = rw::call(g::operator_new, {4});
                                         rw::call(THREAD_STRUCT_CTOR, {t});
                                         return t;
                                     }()});
    const uint32_t client = R(job + 20), result = R(job), document = result + RESULT_DOCUMENT;
    const uint32_t response = R(job + 8), connection = R(job + 12);
    const uint32_t headers = frame.addr(0x60), predicate = frame.addr(0x74);
    rw::call(STRING_INIT_RANGE, {headers, R(response + RESPONSE_HEADERS), R(response + RESPONSE_HEADERS + 4)});
    const uint32_t body = response + RESPONSE_BODY;
    const uint32_t key = connection + 0x34;

    bool intact = true;
    if (str(key).size() != 0) {
        const uint32_t md5 = frame.addr(0x54);
        rw::call(MD5_CALCULATE, {md5, R(body), R(body + 4) - R(body)});
        bool differs = true;
        const uint32_t n = str(key).size();
        if (n == str(md5).size()) {
            if (str(key).isLong()) {
                differs = rw::call(MEMCMP, {str(key).data(), str(md5).data(), n}) != 0;
            } else {
                differs = false;
                for (uint32_t k = 0; k < n; k++)
                    if (rd8(str(key).data() + k) != rd8(str(md5).data() + k)) {
                        differs = true;
                        break;
                    }
            }
        }
        if (str(md5).isLong()) rw::call(g::operator_delete, {str(md5).w2.get()});
        intact = !differs;
    }
    if (intact) {
        if (headersContain(headers, S_APPLICATION_JSON, 16, predicate) ||
            headersContain(headers, S_TEXT_JAVASCRIPT, 15, predicate)) {
            const uint32_t text = frame.addr(0x48), stream = frame.addr(0x6C);
            rw::call(g::string_init, {text, R(body), R(body + 4) - R(body)});
            W(stream, str(text).data());
            W(stream + 4, str(text).data());
            rw::call(PARSE_STREAM, {document, stream});
            if (str(text).isLong()) rw::call(g::operator_delete, {str(text).w2.get()});
        } else if (R(connection + 0x2C) != 0 && (headersContain(headers, S_ENCODED_JSON, 24, predicate) ||
                                                 headersContain(headers, S_OCTET_STREAM, 24, predicate))) {
            rw::call(0x499021, {client, body, document, connection + 0x2C});   // decodeEncodedJSONData
        } else {
            W(result, 2);
        }
    }

    // "Request-Interval-Order: N" -> document["__request_interval_order__"] = N
    const uint32_t needle = frame.addr(0x3C);
    const uint32_t chars = rw::call(g::operator_new, {0x20});
    W(needle, 0x21);
    W(needle + 4, INTERVAL_HEADER_LENGTH);
    W(needle + 8, chars);
    rw::call(MEMCPY, {chars, S_INTERVAL_HEADER, INTERVAL_HEADER_LENGTH});
    rw::store8(rw::host_ptr<uint8_t>(chars + INTERVAL_HEADER_LENGTH), 0);
    const int32_t at = int32_t(rw::call(FIND_SUBSTRING_INSENSITIVE, {headers, needle}));
    if (at >= 0) {
        const uint32_t interval = rw::call(ATOI, {str(headers).data() + uint32_t(at) + INTERVAL_HEADER_LENGTH});
        const uint32_t allocator = R(document + 0x10);
        if (R(document + 0xC) != JSON_OBJECT) {
            W(document + 0xC, JSON_OBJECT);
            W(document, 0);
            W(document + 4, 0);
            W(document + 8, 0);
        }
        rw::call(ADD_MEMBER_INT, {document, S_INTERVAL_MEMBER, interval, allocator});
    }

    rw::call(Ref_release, {response});
    rw::call(STD_MUTEX_LOCK, {client + 0x1C});
    const uint32_t pair = frame.addr(0x2C);
    W(pair, connection);
    W(pair + 4, R(job + 16));
    if (R(pair + 4)) rw::call(g::shared_add, {R(pair + 4)});
    W(pair + 8, result);
    W(pair + 0xC, R(job + 4));
    if (R(pair + 0xC)) rw::call(g::shared_add, {R(pair + 0xC)});
    rw::call(RESULTS_PUSH_BACK, {client + 0x20, pair});
    if (R(pair + 0xC)) rw::call(g::shared_release, {R(pair + 0xC)});
    if (R(pair + 4)) rw::call(g::shared_release, {R(pair + 4)});
    const uint32_t scheduler = R(rw::call(Director_getInstance, {}) + 0x60);
    const uint32_t deliver = frame.addr(0x78);
    W(deliver + 16, deliver);
    W(deliver, VT_DELIVER_RESULTS);
    W(deliver + 4, client);
    rw::call(Scheduler_performFunctionInCocosThread, {scheduler, deliver});
    if (const uint32_t f = R(deliver + 16)) rw::vcall(rw::host_ptr<uint32_t>(f), f == deliver ? 4 : 5, {f});
    rw::call(STD_MUTEX_UNLOCK, {client + 0x1C});
    if (str(needle).isLong()) rw::call(g::operator_delete, {str(needle).w2.get()});
    if (str(headers).isLong()) rw::call(g::operator_delete, {str(headers).w2.get()});
    if (R(job + 16)) rw::call(g::shared_release, {R(job + 16)});
    if (R(job + 4)) rw::call(g::shared_release, {R(job + 4)});
    rw::call(g::operator_delete, {job});
    return 0;
}
// a thread entry: the verifier never shadow-runs it (the host started it), its callees are verified on their own
RW_FUNCTION(0x4991BC, "hole::network::RESTClient (sub_4991BC): the decode worker thread", ResponseThread);

}  // namespace hole::network
