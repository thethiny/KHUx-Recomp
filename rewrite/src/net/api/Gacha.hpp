// Gacha responses: the gacha (medal draw) list and one draw's result.
#pragma once

#include "Common.hpp"

namespace hole::network::api {

struct GachaList {
    struct Gacha {
        rw::Field<uint8_t> validAdd;          // +0
        const uint8_t pad_1[3];
        rw::Field<int32_t> drawMedalTypeId;   // +4
        rw::Field<int32_t> drawType;          // +8
        rw::Field<int32_t> sortId;            // +12
        rw::Field<int32_t> payType;           // +16
        rw::Field<int32_t> price;             // +20
        rw::Field<int32_t> lotCount;          // +24
        rw::Field<uint32_t> isDayLimited;     // +28 (unsigned in the JSON)
        rw::Field<uint32_t> userLimitedCount; // +32 (unsigned in the JSON)
        StdString name;                       // +36
        StdString description;                // +48
        StdString bannerUrl;                  // +60

        static uint32_t init(Gacha *self, const JsonValue *json);
        static Gacha *copyConstruct(Gacha *self, const Gacha *other);   // Gacha(const Gacha&)
    };

    StdVector gachas;                         // +0 std::vector<Gacha> ("gachaList")

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(GachaList::Gacha, name, 36);
RW_LAYOUT(GachaList::Gacha, bannerUrl, 60);
RW_SIZE(GachaList::Gacha, 72);

struct GachaDraw {
    SharedPtr medals;                         // +0  UserMedal
    SharedPtr skills;                         // +8  UserSkill
    SharedPtr materials;                      // +16 UserMaterial
    rw::Field<int32_t> addType;               // +24 gachaBonus.addType
    StdVector userMedalIds;                   // +28 std::vector<unsigned long long> (gachaBonus.userMedalIds)
    StdVector firstGetUserMedalIds;           // +40 std::vector<unsigned long long>

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
};
RW_LAYOUT(GachaDraw, userMedalIds, 28);
RW_SIZE(GachaDraw, 0x34);

}  // namespace hole::network::api
