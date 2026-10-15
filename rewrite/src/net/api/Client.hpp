// hole::network::api::Client: the game's API client. Calls are queued (a deque of Requests) and issued one at a
// time. Before the first call (and for the login actions 59 / 61) it runs the session handshake:
//   system status update (action 0 path, appSignature = md5(prefix + info/version + "a" + suffix)) -> optional
//   server URL switch (the "server" field, decrypted with md5(result key + current + mode)) -> login token
//   (action 58, POST {UUID, deviceType 2, nativeToken} to the returned url) -> native session (sets the
//   session id and the shared Cipher) -> the queued call (requestAPI).
// Responses go to parseResponse, which builds the Ret, runs the call's callback and issues the next request
// (or, when the callback returns non-zero, fails every queued call with status 5 and clears the queue).
#pragma once

#include "RESTClient.hpp"

namespace hole::network::api {

struct Client {
    // A queued call (56 bytes; 73 per deque block).
    struct Request {
        SharedPtr connection;                 // +0  shared_ptr<RESTClient::Connection>
        rw::Field<uint32_t> action;           // +8
        SharedPtr params;                     // +12 shared_ptr<GenericDocument>
        const uint8_t pad_14[4];
        const uint8_t callback[16];                 // +24 std::function<StatusCode(StatusCode, const shared_ptr<Ret>&,
        rw::Field<uint32_t> callbackF;        // +40   const GenericDocument&)> (its __f_)
        const uint8_t pad_2c[4];
        rw::Field<uint8_t> cancelable;        // +48
        rw::Field<uint8_t> retry;             // +49 retry count (X-Sqex-Hole-Retry; a retry keeps "ruv")
        const uint8_t pad_32[6];
    };

    SharedPtr cipher;                         // +0   shared_ptr<Cipher> (made by the native session)
    const uint8_t config[0x30];                     // +8   GenericDocument: json/server_config.json
    const uint8_t api[0x30];                        // +56  GenericDocument: json/server_api.json ("actions"[id])
    ConnectionQueue requests;                 // +104 std::deque<Request>
    StdString serverURL;                      // +128 serverURLDomain (replaced by a status update's "server")

    static constexpr uint32_t GUARD = 0x1003550;
    static constexpr uint32_t INSTANCE = 0x10034C4;

    static uint32_t getInstance();
    static Client *construct(Client *self);   // C2
    static Client *destroy(Client *self);     // D2
    // queue a call (issued at once when the queue was empty); *out = its connection
    static uint32_t call(SharedPtr *out, Client *self, uint32_t action, uint32_t callback, const SharedPtr *params,
                         uint32_t retry, uint32_t cancelable);
    static uint32_t issueRequest(Client *self, const Request *request);
    static uint32_t isLoginAction(uint32_t action);
    static uint32_t requestAPI(Client *self, const Request *request);
    static uint32_t parseResponse(Client *self, uint32_t action, const SharedPtr *result);
    static uint32_t processNativeSessionCreateCallback(Client *self, const Request *request, const SharedPtr *result);
    static uint32_t processSystemStatusUpdateCallback(Client *self, const Request *request, const SharedPtr *result);
    static uint32_t processLoginTokenCallback(Client *self, const Request *request, const SharedPtr *result);
};
RW_LAYOUT(Client::Request, callbackF, 40);
RW_LAYOUT(Client::Request, cancelable, 48);
RW_SIZE(Client::Request, 56);
RW_LAYOUT(Client, requests, 0x68);
RW_LAYOUT(Client, serverURL, 0x80);
RW_SIZE(Client, 0x8C);

// read a JSON file from the vfs into *document; 1 when it parsed
uint32_t LoadJSON(uint32_t path, uint32_t document);
// the request headers: the form Content-Type when form == 1, X-Sqex-Hole-Nsid when nativeSession == 1
uint32_t createHeaders(StdVector *out, uint32_t form, uint32_t nativeSession);

}  // namespace hole::network::api
