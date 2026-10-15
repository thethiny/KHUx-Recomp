// hole::network::RESTClient: the HTTP layer under api::Client. Requests are queued (a deque of Connections) and
// sent one at a time through cocos2d's HttpClient; a successful response is decoded on a worker thread, a failed
// one calls the connection's callback with status 1 right away. Request bodies are JSON: sent plain, as a query
// string (GET: url-escaped name=value pairs), or encrypted ("v=" + url-escaped base64 of Cipher::encrypt).
#pragma once

#include "Common.hpp"

namespace hole::network {

// std::deque<std::shared_ptr<Connection>> (libc++: a block map of 512 shared_ptrs per block)
struct ConnectionQueue {
    rw::Field<uint32_t> mapFirst;             // +0  __split_buffer<pointer>
    rw::Field<uint32_t> mapBegin;             // +4
    rw::Field<uint32_t> mapEnd;               // +8
    rw::Field<uint32_t> mapCap;               // +12
    rw::Field<uint32_t> start;                // +16 index of the front element
    rw::Field<uint32_t> size;                 // +20
};
RW_SIZE(ConnectionQueue, 24);

struct RESTClient {
    enum Method : uint32_t { GET = 0, POST = 1, PUT = 2, DELETE_ = 3 };

    // One request in flight or queued (made with make_shared).
    struct Connection {
        rw::Field<uint32_t> request;          // +0  cocos2d::network::HttpRequest*
        const uint8_t pad_4[4];
        const uint8_t callback[16];                 // +8  std::function<void(const shared_ptr<Result>&)> buffer
        rw::Field<uint32_t> callbackF;        // +24 its __f_
        const uint8_t pad_1c[4];
        rw::Field<uint8_t> cancelled;         // +32
        rw::Field<uint8_t> cancelable;        // +33 (cancelAll cancels only these)
        const uint8_t pad_22[10];
        api::SharedPtr cipher;                // +44 shared_ptr<Cipher>
        api::StdString key;                   // +52
        rw::Field<uint32_t> response;         // +64 cocos2d::Ref*

        // set up the HttpRequest (new, 0x88 bytes): GET builds "url?query" (query only when not empty), the other
        // methods keep the url and put the query in the request body
        static uint32_t init(Connection *self, uint32_t url, uint32_t method, uint32_t query, uint32_t callback,
                             const api::SharedPtr *cipher);
        static Connection *destroy(Connection *self);   // D2
        static uint32_t start(Connection *self);        // HttpClient::send(request)
    };

    rw::Field<uint32_t> curl;                 // +0  CURL* (escape / unescape only)
    ConnectionQueue queue;                    // +4
    rw::Field<uint32_t> mutex;                // +28
    const uint8_t results[24];                      // +32 std::deque<pair<shared_ptr<Connection>, shared_ptr<Result>>>

    static constexpr uint32_t GUARD = 0x1003470;     // the singleton's guard
    static constexpr uint32_t INSTANCE = 0x1003474;  // the singleton

    static uint32_t getInstance();
    static RESTClient *construct(RESTClient *self);  // C2
    static RESTClient *destroy(RESTClient *self);    // D2
    // build and queue a request (sent at once when the queue was empty); *out = the connection (moved out of
    // *connection, which is made when empty)
    static uint32_t request(api::SharedPtr *out, RESTClient *self, uint32_t url, uint32_t method, uint32_t headers,
                            const api::SharedPtr *document, uint32_t callback, const api::SharedPtr *cipher,
                            uint32_t cancelable, api::SharedPtr *connection, uint32_t key);
    static uint32_t onRespond(RESTClient *self, uint32_t client, uint32_t response);
    // mark a connection cancelled, dropping it from the queue unless it is the one in flight
    static uint32_t cancel(RESTClient *self, const api::SharedPtr *connection);
    static uint32_t cancelAll(RESTClient *self);
    // url-unescape, base64-decode and decrypt a response body, then parse it into *document
    static uint32_t decodeEncodedJSONData(const RESTClient *self, const api::StdVector *data, uint32_t document,
                                          const api::SharedPtr *cipher);
};
RW_LAYOUT(RESTClient::Connection, cancelled, 32);
RW_LAYOUT(RESTClient::Connection, cipher, 44);
RW_LAYOUT(RESTClient::Connection, response, 64);
RW_SIZE(RESTClient, 0x38);

// "name=value" of one JSON member onto a rapidjson string stack (the name and a string value url-escaped when
// escape == 1; an int value printed with %d; other values give "name=")
uint32_t AppendQuery(uint32_t curl, uint32_t member, uint32_t stack, uint32_t escape);
// index of the first case-insensitive match of *needle in *haystack, or -1
uint32_t FindSubstringInsensitive(const api::StdString *haystack, const api::StdString *needle);

}  // namespace hole::network
