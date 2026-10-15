#include "NativeToken.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t SHARED_PTR_CTOR = 0x49DF79;   // std::shared_ptr<NativeToken>::shared_ptr(NativeToken*)
}

uint32_t NativeToken::parse(SharedPtr *out, const JsonValue *response) {
    if (!response->IsObject()) {
        out->reset();
        return 0;
    }
    NativeToken *self = newObject<NativeToken>(sizeof(NativeToken));
    self->token.clear();
    const JsonValue &token = (*response)["nativeToken"];
    if (token.IsString()) {
        self->token.assign(token.GetString());
    } else {
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x49DEF0, "hole::network::api::NativeToken::parse", NativeToken::parse);

}  // namespace hole::network::api
