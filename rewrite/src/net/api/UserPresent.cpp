#include "UserPresent.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t SHARED_PTR_CTOR = 0x4A6441;       // std::shared_ptr<UserPresent>::shared_ptr(UserPresent*)
constexpr uint32_t MAP_INSERT = 0x4AF539;            // map<ull, Present>::__insert_unique(pair<ull, Present>&&)
constexpr uint32_t MAP_FIND = 0x4AF445;              // map<ull, Present>::find(const ull&)
constexpr uint32_t MAP_DESTROY = 0x4B5381;           // map<ull, Present>::__tree::destroy(node)
constexpr uint32_t STRING_SHRINK = 0x48D705;         // std::string::reserve(0) (shrink after clear)

bool readInt(const JsonValue &obj, const char *key, rw::Field<int32_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    field = v.GetInt();
    return true;
}
// ~std::string of a temporary: free a long string's buffer
void freeString(const StdString *s) {
    if (s->isLong()) guestDelete(s->w2);
}
}  // namespace

uint32_t UserPresent::Present::init(Present *self, const JsonValue *json) {
    GuestFrame frame(0x38);                      // original frame; the comment temporary at +0x14
    if (!json->IsObject()) return 0;
    const JsonValue &id = (*json)["userPresentId"];
    if (!id.IsUint64()) return 0;
    self->userPresentId = id.GetUint64();
    if (!readInt(*json, "resourceType", self->resourceType)) return 0;
    if (!readInt(*json, "resourceId", self->resourceId)) return 0;
    if (!readInt(*json, "number", self->number)) return 0;
    const JsonValue &limit = (*json)["limitDatetime"];
    if (!limit.IsString()) return 0;
    if (rw::call(guest::Date_init, {rw::guest_addr(self->limitDatetime), limit.GetString(), 0}) != 1) return 0;
    const JsonValue &comment = (*json)["comment"];
    if (!comment.IsString()) return 0;

    // self->comment = std::string(text): a temporary, then the inlined move assignment (clear, shrink, steal)
    StdString *tmp = frame.at<StdString>(0x14);
    uint32_t text = comment.GetString();
    rw::call(guest::string_init, {frame.addr(0x14), text, uint32_t(std::strlen(rw::host_ptr<char>(text)))});
    StdString *dst = &self->comment;
    if (dst->isLong()) {
        rw::store8(rw::host_ptr<uint8_t>(dst->w2), 0);
        dst->w1 = 0;
    } else {
        rw::store8(reinterpret_cast<uint8_t *>(dst) + 1, 0);
        rw::store8(reinterpret_cast<uint8_t *>(dst), 0);
    }
    rw::call(STRING_SHRINK, {rw::guest_addr(dst), 0});
    dst->w0 = tmp->w0.get();
    dst->w1 = tmp->w1.get();
    dst->w2 = tmp->w2.get();

    const JsonValue &created = (*json)["created"];
    if (!created.IsString()) return 0;
    return rw::call(guest::Date_init, {rw::guest_addr(self->created), created.GetString(), 0});
}
RW_FUNCTION(0x4A6014, "hole::network::api::UserPresent::Present::init", UserPresent::Present::init);

uint32_t UserPresent::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0xe0: the pair<ull, Present> at +0x40, the Present local at +0x80, the insert result at +0xbc
    GuestFrame frame(0xE0);
    constexpr uint32_t PAIR = 0x40, LOCAL = 0x80, RESULT = 0xBC;
    Present *local = frame.at<Present>(LOCAL);
    Present *pairValue = frame.at<Present>(PAIR + 8);

    UserPresent *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &box = (*response)["userPresent"];
        if (box.IsObject()) {
            const JsonValue &list = box["userPresentDatas"];
            if (list.IsArray()) {
                uint32_t n = list.Size();
                self = newObject<UserPresent>(sizeof(UserPresent));
                self->beginNode = rw::guest_addr(self) + 4;   // empty map: begin = end node
                self->root = 0;
                self->size = 0;
                uint32_t k = 0;
                for (; k < n; k++) {
                    // the two Date flag bytes get bit 1 cleared; on the first pass the registers still hold 0 and
                    // the pair's address (sp + 0x48), whose low byte becomes the created flags (original quirk)
                    const uint8_t *limitFlags = local->limitDatetime + 7;
                    const uint8_t *createdFlags = local->created + 7;
                    uint8_t limitOld = k == 0 ? 0 : *limitFlags;
                    uint8_t createdOld = k == 0 ? uint8_t(frame.addr(PAIR + 8)) : *createdFlags;
                    rw::store8(limitFlags, uint8_t(limitOld & 0xFD));
                    local->comment.clear();
                    rw::store8(createdFlags, uint8_t(createdOld & 0xFD));
                    local->received = 0;
                    if (!Present::init(local, &list.At(k))) {
                        freeString(&local->comment);
                        break;
                    }
                    // pair(userPresentId, std::move(present))
                    rw::store32(frame.at<uint32_t>(PAIR), local->userPresentId.raw & 0xFFFFFFFFu);
                    rw::store32(frame.at<uint32_t>(PAIR + 4), uint32_t(local->userPresentId.raw >> 32));
                    for (uint32_t w = 0; w < 28; w += 4)
                        rw::store32(reinterpret_cast<uint8_t *>(pairValue) + w, rw::get<uint32_t>(local, w));
                    pairValue->comment.w0 = local->comment.w0.get();
                    pairValue->comment.w1 = local->comment.w1.get();
                    pairValue->comment.w2 = local->comment.w2.get();
                    local->comment.clear();
                    rw::store32(pairValue->created, rw::get<uint32_t>(local->created, 0));
                    rw::store32(pairValue->created + 4, rw::get<uint32_t>(local->created, 4));
                    pairValue->received = local->received.get();
                    rw::call(MAP_INSERT, {frame.addr(RESULT), rw::guest_addr(self), frame.addr(PAIR)});
                    freeString(&pairValue->comment);
                    freeString(&local->comment);
                }
                bool ok = k == n;
                if (ok) {
                    const JsonValue &count = box["presentCount"];
                    ok = count.IsInt();
                    if (ok) {
                        self->presentCount = count.GetInt();
                        const JsonValue &max = box["presentCountMax"];
                        ok = max.IsInt();
                        if (ok) self->presentCountMax = max.GetInt();
                    }
                }
                if (!ok) {
                    rw::call(MAP_DESTROY, {rw::guest_addr(self), self->root});
                    guestDelete(rw::guest_addr(self));
                    self = nullptr;
                }
            }
        }
    }
    rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A61E4, "hole::network::api::UserPresent::parse", UserPresent::parse);

uint32_t UserPresent::setReceived(UserPresent *self, uint64_t userPresentId) {
    GuestFrame frame(0x28);                      // original frame; the id local at +8
    rw::store32(frame.at<uint32_t>(8), uint32_t(userPresentId));
    rw::store32(frame.at<uint32_t>(12), uint32_t(userPresentId >> 32));
    uint32_t node = rw::call(MAP_FIND, {rw::guest_addr(self), frame.addr(8)});
    if (node == rw::guest_addr(self) + 4) return 0;          // end(): not found
    rw::save<uint8_t>(node + 0x48, 1);                        // node value (+24) .received (+48)
    return 1;
}
RW_FUNCTION(0x4A649C, "hole::network::api::UserPresent::setReceived", UserPresent::setReceived);

}  // namespace hole::network::api
