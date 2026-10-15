// MypageInfo: the home screen's state (action 56: admin texts, open banners, present count, ranking reward flags,
// the background).
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct MypageInfo {
    struct AdminText {
        rw::Field<int32_t> type;              // +0
        rw::Field<int32_t> priority;          // +4
        StdString text;                       // +8

        static uint32_t init(AdminText *self, const JsonValue *json);
    };
    struct OpenBanner {
        rw::Field<int32_t> type;              // +0
        rw::Field<int32_t> id;                // +4
        const uint8_t limitDate[8];                 // +8 Date

        // returns Date::init's result for limitDate once the rest is in (0 before that)
        static uint32_t init(OpenBanner *self, const JsonValue *json);
    };
    // Which ranking rewards are waiting (5 bools).
    struct RankingRewards {
        rw::Field<uint8_t> rankingColosseum;  // +0
        rw::Field<uint8_t> rankingParty;      // +1
        rw::Field<uint8_t> rankingLuxWeekly;  // +2
        rw::Field<uint8_t> rankingLuxMonthly; // +3
        rw::Field<uint8_t> rankingLux;        // +4

        static uint32_t init(RankingRewards *self, const JsonValue *json);
    };

    StdVector adminTexts;                     // +0  std::vector<AdminText> ("adminMypageTexts")
    StdVector openBanners;                    // +12 std::vector<OpenBanner>
    rw::Field<int32_t> userPresentCount;      // +24
    rw::Field<uint8_t> isChangeCoordinate;    // +28
    RankingRewards rankingRewards;            // +29
    const uint8_t pad_34[2];
    rw::Field<uint32_t> backgroundId;         // +36 (unsigned in the JSON; mypageBackground master id)
    rw::Field<uint8_t> isNewAlbumChallenge;   // +40 (unsigned in the JSON)
    const uint8_t pad_41[3];

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_SIZE(MypageInfo::AdminText, 20);
RW_SIZE(MypageInfo::OpenBanner, 16);
RW_LAYOUT(MypageInfo, rankingRewards, 29);
RW_LAYOUT(MypageInfo, backgroundId, 36);
RW_SIZE(MypageInfo, 0x2C);

}  // namespace hole::network::api
