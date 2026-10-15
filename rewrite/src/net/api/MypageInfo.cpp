#include "MypageInfo.hpp"

#include "Party.hpp"

namespace hole::network::api {

namespace {
constexpr uint32_t SHARED_PTR_CTOR = 0x4ADC09;            // std::shared_ptr<MypageInfo>::shared_ptr(MypageInfo*)
constexpr uint32_t TEXT_PUSH_SLOW = 0x4B2159;             // vector<AdminText>::__push_back_slow_path(const&)
constexpr uint32_t TEXT_VECTOR_BASE_DTOR = 0x4B1FC9;
constexpr uint32_t BANNER_PUSH_SLOW = 0x4B2015;           // vector<OpenBanner>::__push_back_slow_path(const&)
constexpr uint32_t BANNER_VECTOR_BASE_DTOR = 0x4B1F9B;

bool readInt(const JsonValue &obj, const char *key, rw::Field<int32_t> &field) {
    const JsonValue &v = obj[key];
    if (!v.IsInt()) return false;
    field = v.GetInt();
    return true;
}
}  // namespace

uint32_t MypageInfo::AdminText::init(AdminText *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    if (!readInt(*json, "type", self->type)) return 0;
    if (!readInt(*json, "priority", self->priority)) return 0;
    const JsonValue &text = (*json)["text"];
    if (!text.IsString()) return 0;
    self->text.assign(text.GetString());
    return 1;
}
RW_FUNCTION(0x4AD6C4, "hole::network::api::MypageInfo::AdminText::init", MypageInfo::AdminText::init);

uint32_t MypageInfo::OpenBanner::init(OpenBanner *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    if (!readInt(*json, "type", self->type)) return 0;
    if (!readInt(*json, "id", self->id)) return 0;
    const JsonValue &limit = (*json)["limitDate"];
    if (!limit.IsString()) return 0;
    return rw::call(guest::Date_init, {rw::guest_addr(self->limitDate), limit.GetString(), 0});
}
RW_FUNCTION(0x4AD764, "hole::network::api::MypageInfo::OpenBanner::init", MypageInfo::OpenBanner::init);

uint32_t MypageInfo::RankingRewards::init(RankingRewards *self, const JsonValue *json) {
    if (!json->IsObject()) return 0;
    static const char *const kKeys[] = {"rankingColosseum", "rankingParty", "rankingLuxWeekly", "rankingLuxMonthly",
                                        "rankingLux"};
    for (uint32_t k = 0; k < 5; k++) {
        const JsonValue &v = (*json)[kKeys[k]];
        if (!v.IsInt()) return 0;
        rw::store8(reinterpret_cast<uint8_t *>(self) + k, uint8_t(v.GetUint() != 0 ? 1 : 0));
    }
    return 1;
}
RW_FUNCTION(0x4AD7F8, "hole::network::api::MypageInfo::RankingRewards::init", MypageInfo::RankingRewards::init);

uint32_t MypageInfo::parse(SharedPtr *out, const JsonValue *response) {
    GuestFrame frame(0x58);                   // original frame; the AdminText / OpenBanner local at +0x28
    MypageInfo *self = newObject<MypageInfo>(sizeof(MypageInfo));
    rw::call(guest::memset_, {rw::guest_addr(self), 0x18, 0});
    auto parseAll = [&]() -> bool {
        if (!response->IsObject()) return false;
        const JsonValue &texts = (*response)["adminMypageTexts"];
        if (!texts.IsArray()) return false;
        uint32_t n = texts.Size();
        AdminText *text = frame.at<AdminText>(0x28);
        for (uint32_t k = 0; k < n; k++) {
            const JsonValue &entry = texts.At(k);
            if (!entry.IsObject()) return false;
            text->text.clear();
            if (!AdminText::init(text, &entry)) {
                party::freeString(&text->text);
                return false;
            }
            uint32_t end = self->adminTexts.end;
            if (end == self->adminTexts.capEnd) {
                rw::call(TEXT_PUSH_SLOW, {rw::guest_addr(&self->adminTexts), rw::guest_addr(text)});
            } else {
                if (end) {
                    party::copyWords(end, rw::guest_addr(text), 2);
                    party::copyString(&rw::host_ptr<AdminText>(end)->text, &text->text);
                }
                self->adminTexts.end = self->adminTexts.end + sizeof(AdminText);
            }
            party::freeString(&text->text);
        }
        const JsonValue &banners = (*response)["openBanners"];
        if (!banners.IsArray()) return false;
        uint32_t m = banners.Size();
        OpenBanner *banner = frame.at<OpenBanner>(0x28);
        for (uint32_t k = 0; k < m; k++) {
            const JsonValue &entry = banners.At(k);
            if (!entry.IsObject()) return false;
            rw::store8(banner->limitDate + 7, uint8_t(banner->limitDate[7] & 0xFD));
            if (OpenBanner::init(banner, &entry) != 1) return false;
            uint32_t end = self->openBanners.end;
            if (end == self->openBanners.capEnd) {
                rw::call(BANNER_PUSH_SLOW, {rw::guest_addr(&self->openBanners), rw::guest_addr(banner)});
            } else {
                uint32_t base = 0;
                if (end) {
                    party::copyWords(end, rw::guest_addr(banner), 4);
                    base = self->openBanners.end;
                }
                self->openBanners.end = base + sizeof(OpenBanner);
            }
        }
        if (!readInt(*response, "userPresentCount", self->userPresentCount)) return false;
        const JsonValue &coordinate = (*response)["isChangeCoordinate"];
        if (!coordinate.IsInt()) return false;
        self->isChangeCoordinate = uint8_t(coordinate.GetUint() != 0 ? 1 : 0);
        const JsonValue &rewards = (*response)["rankingRewards"];
        if (!rewards.IsObject() || RankingRewards::init(&self->rankingRewards, &rewards) != 1) return false;
        const JsonValue &background = (*response)["backgroundId"];
        if (!background.IsUint()) return false;
        self->backgroundId = background.GetUint();
        const JsonValue &album = (*response)["isNewAlbumChallenge"];
        if (!album.IsUint()) return false;
        self->isNewAlbumChallenge = uint8_t(album.GetUint() != 0 ? 1 : 0);
        return true;
    };
    if (!parseAll()) {
        rw::call(BANNER_VECTOR_BASE_DTOR, {rw::guest_addr(&self->openBanners)});
        rw::call(TEXT_VECTOR_BASE_DTOR, {rw::guest_addr(&self->adminTexts)});
        guestDelete(rw::guest_addr(self));
        self = nullptr;
    }
    rw::call(SHARED_PTR_CTOR, {rw::guest_addr(out), rw::guest_addr(self), 0});
    return 0;   // stack-protector check
}
RW_FUNCTION(0x4AD8F8, "hole::network::api::MypageInfo::parse", MypageInfo::parse);

}  // namespace hole::network::api
