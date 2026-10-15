// NativeToken: the native session token (action "native session create": {"nativeToken": "..."}).
// The response object is just the token string.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct NativeToken {
    StdString token;               // +0

    // response["nativeToken"] -> *out (an empty pointer when the response is not an object; a null object
    // pointer with a control block is never made: a non-string token gives shared_ptr(nullptr))
    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(NativeToken, 12);

}  // namespace hole::network::api
