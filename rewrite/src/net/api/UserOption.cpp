#include "UserOption.hpp"

#include "Party.hpp"
#include "Raid.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t OPTION_SHARED_PTR_CTOR = 0x4ADD51;     // std::shared_ptr<UserOption>::shared_ptr(UserOption*)
constexpr uint32_t LINK_MAKE_SHARED = 0x4AE4A1;           // std::shared_ptr<UserLink>::make_shared<>()
constexpr uint32_t INFO_PUSH_SLOW = 0x4B1DC1;             // vector<UserLink::Info>::__push_back_slow_path(const&)
constexpr uint32_t INFO_VECTOR_BASE_DTOR = 0x4AE051;      // __vector_base<UserLink::Info>::~__vector_base
constexpr uint32_t MAP_INSERT = 0x4AE09B;                 // unordered_map<PlatformType, Info>::__insert_unique(pair&&)
}  // namespace

uint32_t UserOption::parse(SharedPtr *out, const JsonValue *response) {
    UserOption *self = newObject<UserOption>(3);
    bool ok = false;
    if (response->IsObject()) {
        const JsonValue &option = (*response)["userOption"];
        if (option.IsObject()) {
            static const char *const kKeys[] = {"isNoticeAp", "isNoticeHelp", "isNoticeEvent"};
            ok = true;
            for (uint32_t k = 0; k < 3 && ok; k++) {
                const JsonValue &v = option[kKeys[k]];
                ok = v.IsInt();
                if (ok) rw::store8(reinterpret_cast<uint8_t *>(self) + k, uint8_t(v.GetUint() != 0 ? 1 : 0));
            }
        }
    }
    if (!ok) {
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    return rw::call(OPTION_SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
}
RW_FUNCTION(0x4ADC6C, "hole::network::api::UserOption::parse", UserOption::parse);

uint32_t UserLink::Info::init(Info *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    const JsonValue &type = (*json)["platformType"];
    if (!type.IsUint()) return 0;
    self->platformType = uint8_t(type.GetUint());
    const JsonValue &id = (*json)["linkPlatformId"];
    if (!id.IsString()) return 0;
    self->linkPlatformId.assign(id.GetString());
    return 1;
}
RW_FUNCTION(0x4ADDA0, "hole::network::api::UserLink::Info::init", UserLink::Info::init);

uint32_t UserLink::parse(SharedPtr *out, const JsonValue *response) {
    // original frame 0x70: the pair<PlatformType, Info> at +0x18, the vector<Info> at +0x2c, the made UserLink at
    // +0x38, the Info local at +0x40, the insert result at +0x50
    GuestFrame frame(0x70);
    SharedPtr *made = frame.at<SharedPtr>(0x38);
    StdVector *infos = frame.at<StdVector>(0x2C);
    Info *local = frame.at<Info>(0x40);
    rw::call(LINK_MAKE_SHARED, {frame.addr(0x38)});

    auto fail = [&]() -> uint32_t {
        out->reset();
        if (made->ctrl) raid::releaseShared(made->ctrl);
        return 0;   // stack-protector check
    };
    if (!response->IsObject()) return fail();
    infos->begin = 0;
    infos->end = 0;
    infos->capEnd = 0;
    const JsonValue &list = (*response)["linkInfos"];
    bool ok = list.IsArray();
    if (ok) {
        // 1. the entries into the vector
        uint32_t n = list.Size();
        for (uint32_t k = 0; k < n && ok; k++) {
            const JsonValue &entry = list.At(k);
            if (!entry.IsObject()) { ok = false; break; }
            local->linkPlatformId.clear();
            if (!Info::init(local, &entry)) {
                party::freeString(&local->linkPlatformId);
                ok = false;
                break;
            }
            uint32_t end = infos->end;
            if (end == infos->capEnd) {
                rw::call(INFO_PUSH_SLOW, {rw::guest_addr(infos), rw::guest_addr(local)});
            } else {
                uint32_t base = 0;
                if (end) {
                    Info *d = rw::host_ptr<Info>(end);
                    d->platformType = local->platformType.get();
                    party::copyString(&d->linkPlatformId, &local->linkPlatformId);
                    d->platformType = local->platformType.get();
                    base = infos->end;
                }
                infos->end = base + sizeof(Info);
            }
            party::freeString(&local->linkPlatformId);
        }
    }
    if (!ok) {
        rw::call(INFO_VECTOR_BASE_DTOR, {rw::guest_addr(infos)});
        return fail();
    }
    // 2. every entry into the map, keyed by its platform type (a pair copy, then its destructor)
    uint32_t map = made->ptr;
    uint8_t *pairKey = frame.at<uint8_t>(0x18);
    Info *pairValue = frame.at<Info>(0x1C);
    for (uint32_t e = infos->begin; e != infos->end; e += sizeof(Info)) {
        Info *src = rw::host_ptr<Info>(e);
        uint8_t type = src->platformType.get();
        rw::store8(pairKey, type);
        pairValue->platformType = type;
        if (src->linkPlatformId.isLong()) {
            rw::call(party::string_init, {rw::guest_addr(&pairValue->linkPlatformId), src->linkPlatformId.w2,
                                          src->linkPlatformId.w1});
            type = src->platformType.get();                    // reloaded after the call
        } else {
            pairValue->linkPlatformId.w0 = src->linkPlatformId.w0.get();
            pairValue->linkPlatformId.w1 = src->linkPlatformId.w1.get();
            pairValue->linkPlatformId.w2 = src->linkPlatformId.w2.get();
        }
        pairValue->platformType = type;
        rw::call(MAP_INSERT, {frame.addr(0x50), map, frame.addr(0x18)});
        party::freeString(&pairValue->linkPlatformId);
    }
    rw::call(INFO_VECTOR_BASE_DTOR, {rw::guest_addr(infos)});
    out->ptr = made->ptr.get();
    out->ctrl = made->ctrl.get();
    made->reset();
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4ADE18, "hole::network::api::UserLink::parse", UserLink::parse);

}  // namespace hole::network::api
