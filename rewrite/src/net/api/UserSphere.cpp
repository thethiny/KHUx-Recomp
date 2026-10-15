#include "UserSphere.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t SHARED_PTR_CTOR = 0x4A5ED1;     // std::shared_ptr<UserSphere>::shared_ptr(UserSphere*)
constexpr uint32_t DATE_SINCE_1970 = 0x492745;     // Date::calculateIntervalSince1970() const
constexpr uint32_t VECTOR_INT_PUSH_SLOW = 0x47D709;  // std::vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t VECTOR_INT_COPY = 0x4AF707;     // std::vector<int>::vector(const vector&)
constexpr uint32_t VECTOR_INT_BASE_DTOR = 0x478961;  // std::__vector_base<int>::~__vector_base()
// std::map<int, UserSphere::Sphere> (__tree) members
constexpr uint32_t MAP_INSERT = 0x4AFA79;          // __insert_unique(pair<int, Sphere>&&)
constexpr uint32_t MAP_ERASE = 0x4AF769;           // __erase_unique(const int&)
constexpr uint32_t MAP_DESTROY = 0x4B53BD;         // destroy(node*)

// std::vector<int>::push_back(value) as the parsers inline it: the value is first stored in a guest-stack
// local (the slow path takes its address); the fast path keeps libc++'s placement-new null check.
void pushBackInt(StdVector *vec, uint32_t value, uint32_t local) {
    rw::store32(rw::host_ptr<uint32_t>(local), value);
    uint32_t end = vec->end;
    if (end >= vec->capEnd) {
        rw::call(VECTOR_INT_PUSH_SLOW, {rw::guest_addr(vec), local});
        return;
    }
    uint32_t newEnd = 0;
    if (end != 0) {
        rw::save<uint32_t>(end, value);
        newEnd = vec->end;
    }
    vec->end = newEnd + 4;
}

// copy n words (the original's ldm/stm copies)
void copyWords(uint32_t dst, uint32_t src, uint32_t n) {
    for (uint32_t k = 0; k < n; k++) rw::save<uint32_t>(dst + 4 * k, rw::load<uint32_t>(src + 4 * k));
}

// The pair<int, Sphere> handed to the map's __insert_unique.
struct SpherePair {
    rw::Field<int32_t> key;               // +0 sphereBoardId
    UserSphere::Sphere value;             // +4
};
RW_SIZE(SpherePair, 36);
}  // namespace

uint32_t UserSphere::Sphere::init(Sphere *self, const JsonValue *json) {
    // original frame 0x48: the Date local at +0x24, the push_back value at +0x2c
    GuestFrame frame(0x48);
    constexpr uint32_t LOCAL_DATE = 0x24, LOCAL_VALUE = 0x2C;

    if (!json->IsObject()) return 0;
    const JsonValue &userBoard = (*json)["userSphereBoardId"];
    if (!userBoard.IsInt()) return 0;
    self->userSphereBoardId = userBoard.GetInt();
    const JsonValue &board = (*json)["sphereBoardId"];
    if (!board.IsInt()) return 0;
    self->sphereBoardId = board.GetInt();
    const JsonValue &isBuy = (*json)["isBuy"];
    if (!isBuy.IsInt()) return 0;
    self->isBuy = isBuy.GetUint() != 0 ? 1 : 0;
    const JsonValue &status = (*json)["status"];
    if (!status.IsInt()) return 0;
    self->status = status.GetInt();

    const JsonValue &masu = (*json)["unlockMasuIds"];
    if (!masu.IsArray()) return 0;
    uint32_t n = masu.Size();
    uint32_t k = 0;
    for (; k < n; k++) {
        const JsonValue &id = masu.At(k);
        if (!id.IsInt()) break;
        pushBackInt(&self->unlockMasuIds, id.GetUint(), frame.addr(LOCAL_VALUE));
    }
    if (k != n) return 0;

    uint8_t *dateFlags = frame.at<uint8_t>(LOCAL_DATE + 7);     // Date flags byte: bit 1 cleared before init
    rw::store8(dateFlags, uint8_t(*dateFlags & 0xFD));
    const JsonValue &got = (*json)["getDatetime"];
    if (!got.IsString()) return 0;
    if (rw::call(guest::Date_init, {frame.addr(LOCAL_DATE), got.GetString(), 0}) != 1) return 0;
    self->getDatetime = rw::call(DATE_SINCE_1970, {frame.addr(LOCAL_DATE)});
    return 1;
}
RW_FUNCTION(0x4A5AEC, "hole::network::api::UserSphere::Sphere::init", UserSphere::Sphere::init);

uint32_t UserSphere::parse(SharedPtr *out, const JsonValue *response, bool skipNotCharged) {
    // original frame 0x98: the push_back value at +0x30, the map pair at +0x34, the Sphere local at +0x58,
    // the insert result at +0x78
    GuestFrame frame(0x98);
    constexpr uint32_t LOCAL_VALUE = 0x30, LOCAL_PAIR = 0x34, LOCAL_SPHERE = 0x58, LOCAL_RESULT = 0x78;
    SpherePair *pair = frame.at<SpherePair>(LOCAL_PAIR);
    Sphere *record = frame.at<Sphere>(LOCAL_SPHERE);

    UserSphere *self = nullptr;
    if (response->IsObject()) {
        const JsonValue &root = (*response)["userSphere"];
        if (root.IsObject()) {
            const JsonValue &datas = root["userSphereDatas"];
            if (datas.IsArray()) {
                uint32_t n = datas.Size();
                self = newObject<UserSphere>(sizeof(UserSphere));
                self->mapBegin = rw::guest_addr(self) + 4;    // empty map: begin = end node
                self->mapRoot = 0;
                self->mapSize = 0;
                self->notChargedSphereBoardIds.begin = 0;
                self->notChargedSphereBoardIds.end = 0;
                self->notChargedSphereBoardIds.capEnd = 0;

                uint32_t k = 0;
                for (; k < n; k++) {
                    record->unlockMasuIds.begin = 0;
                    record->unlockMasuIds.end = 0;
                    record->unlockMasuIds.capEnd = 0;
                    if (!Sphere::init(record, &datas.At(k))) {
                        rw::call(VECTOR_INT_BASE_DTOR, {rw::guest_addr(&record->unlockMasuIds)});
                        break;
                    }
                    // pair<int, Sphere>(sphereBoardId, std::move(record))
                    pair->key = record->sphereBoardId;
                    copyWords(rw::guest_addr(&pair->value), rw::guest_addr(record), 4);
                    pair->value.getDatetime = record->getDatetime;
                    pair->value.unlockMasuIds.begin = record->unlockMasuIds.begin;
                    pair->value.unlockMasuIds.end = record->unlockMasuIds.end;
                    pair->value.unlockMasuIds.capEnd = record->unlockMasuIds.capEnd;
                    record->unlockMasuIds.begin = 0;
                    record->unlockMasuIds.end = 0;
                    record->unlockMasuIds.capEnd = 0;
                    rw::call(MAP_INSERT, {frame.addr(LOCAL_RESULT), rw::guest_addr(self), frame.addr(LOCAL_PAIR)});
                    rw::call(VECTOR_INT_BASE_DTOR, {rw::guest_addr(&pair->value.unlockMasuIds)});
                    rw::call(VECTOR_INT_BASE_DTOR, {rw::guest_addr(&record->unlockMasuIds)});
                }

                bool ok = k == n;
                if (ok && !skipNotCharged) {
                    const JsonValue &ids = root["notChargedSphereBoardIds"];
                    ok = ids.IsArray();
                    if (ok) {
                        uint32_t m = ids.Size();
                        uint32_t j = 0;
                        for (; j < m; j++) {
                            const JsonValue &id = ids.At(j);
                            if (!id.IsInt()) break;
                            pushBackInt(&self->notChargedSphereBoardIds, id.GetUint(), frame.addr(LOCAL_VALUE));
                        }
                        ok = j == m;
                    }
                }
                if (!ok) {
                    rw::call(VECTOR_INT_BASE_DTOR, {rw::guest_addr(&self->notChargedSphereBoardIds)});
                    rw::call(MAP_DESTROY, {rw::guest_addr(self), self->mapRoot});
                    guestDelete(rw::guest_addr(self));
                    self = nullptr;
                }
            }
        }
    }
    rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A5CB8, "hole::network::api::UserSphere::parse", UserSphere::parse);

uint32_t UserSphere::setSphere(UserSphere *self, const Sphere *sphere) {
    // original frame 0x48: the map pair at +4 (its vector at +0x1c), the insert result at +0x28
    GuestFrame frame(0x48);
    constexpr uint32_t LOCAL_PAIR = 0x4, LOCAL_RESULT = 0x28;
    SpherePair *pair = frame.at<SpherePair>(LOCAL_PAIR);

    rw::call(MAP_ERASE, {rw::guest_addr(self), rw::guest_addr(&sphere->sphereBoardId)});
    // pair<int, Sphere>(sphere.sphereBoardId, sphere): a copy (the vector is copy-constructed)
    pair->key = sphere->sphereBoardId;
    copyWords(rw::guest_addr(&pair->value), rw::guest_addr(sphere), 4);
    pair->value.getDatetime = sphere->getDatetime;
    rw::call(VECTOR_INT_COPY, {rw::guest_addr(&pair->value.unlockMasuIds), rw::guest_addr(&sphere->unlockMasuIds)});
    rw::call(MAP_INSERT, {frame.addr(LOCAL_RESULT), rw::guest_addr(self), frame.addr(LOCAL_PAIR)});
    rw::call(VECTOR_INT_BASE_DTOR, {rw::guest_addr(&pair->value.unlockMasuIds)});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A5F34, "hole::network::api::UserSphere::setSphere", UserSphere::setSphere);

uint32_t UserSphere::removeSphere(UserSphere *self, int32_t sphereBoardId) {
    // original frame 0x20: the key local at +8
    GuestFrame frame(0x20);
    rw::store32(frame.at<uint32_t>(0x8), uint32_t(sphereBoardId));
    rw::call(MAP_ERASE, {rw::guest_addr(self), frame.addr(0x8)});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4A5FD0, "hole::network::api::UserSphere::removeSphere", UserSphere::removeSphere);

}  // namespace hole::network::api
