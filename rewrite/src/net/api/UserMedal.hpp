// UserMedal, UserSkill, UserKeyblade: the player's medals, skills and keyblades (record lists: SafeBuffer
// records indexed by their user id, see UserRecordList.hpp), and UserStone (jewels).
#pragma once

#include "UserRecordList.hpp"

namespace hole::network::api {

struct UserMedal : UserRecordList {
    static constexpr uint32_t VTABLE = 0xF7F148;

    // One owned medal (72 bytes; record k at data position 72 * k).
    struct Medal {
        rw::Field<uint64_t> userMedalId;          // +0
        rw::Field<int32_t> medalId;               // +8 master medal id
        rw::Field<int32_t> level;                 // +12
        rw::Field<int32_t> exp;                   // +16
        rw::Field<int32_t> attackUpperNumber;     // +20
        rw::Field<int32_t> defenseUpperNumber;    // +24
        rw::Field<int32_t> burstUpperNumber;      // +28
        rw::Field<int32_t> lock;                  // +32
        rw::Field<int32_t> upperCost;             // +36
        rw::Field<uint32_t> guiltFactor;          // +40 (an unsigned in the JSON)
        const uint8_t pad_44[4];
        rw::Field<int64_t> userSkillIds[2];       // +48 (ints, sign-extended; the array is not bounds-checked)
        rw::Field<uint32_t> skillCount;           // +64 min(count, 2)
        rw::Field<int32_t> getDatetime;           // +68 seconds since 1970

        static uint32_t init(Medal *self, const JsonValue *json);
    };

    static uint32_t parse(SharedPtr *out, const JsonValue *response);          // response["userMedals"]
    static uint32_t parseSimplex(SharedPtr *out, const JsonValue *response);   // response["userMedal"]: one medal
    static uint32_t create(SharedPtr *out, const StdVector *medals);
    static UserMedal *destroy(UserMedal *self);                                // D2
    static uint32_t destroyDelete(UserMedal *self);                            // D0
};
RW_LAYOUT(UserMedal::Medal, guiltFactor, 40);
RW_LAYOUT(UserMedal::Medal, userSkillIds, 48);
RW_LAYOUT(UserMedal::Medal, getDatetime, 68);
RW_SIZE(UserMedal::Medal, 72);
RW_SIZE(UserMedal, 56);

struct UserSkill : UserRecordList {
    static constexpr uint32_t VTABLE = 0xF7F0DC;

    // One owned skill (40 bytes).
    struct Skill {
        rw::Field<uint64_t> userSkillId;          // +0
        rw::Field<int32_t> skillId;               // +8
        rw::Field<int32_t> level;                 // +12
        rw::Field<int32_t> exp;                   // +16
        const uint8_t pad_20[4];
        rw::Field<uint64_t> userMedalId;          // +24 the medal it is attached to
        rw::Field<int32_t> getDatetime;           // +32 seconds since 1970
        const uint8_t pad_36[4];

        static uint32_t init(Skill *self, const JsonValue *json);
    };

    static uint32_t parse(SharedPtr *out, const JsonValue *response);          // response["userSkills"]
    static uint32_t create(SharedPtr *out, const StdVector *skills);
    static UserSkill *destroy(UserSkill *self);                                // D2
    static uint32_t destroyDelete(UserSkill *self);                            // D0
};
RW_LAYOUT(UserSkill::Skill, userMedalId, 24);
RW_SIZE(UserSkill::Skill, 40);
RW_SIZE(UserSkill, 56);

struct UserKeyblade : UserRecordList {
    static constexpr uint32_t VTABLE = 0xF7F004;

    // One owned keyblade (88 bytes).
    struct Keyblade {
        rw::Field<uint64_t> userKeybladeId;       // +0
        rw::Field<int32_t> category;              // +8
        rw::Field<int32_t> keybladeId;            // +12
        rw::Field<uint32_t> deckCount;            // +16 min(count, 5)
        const uint8_t pad_20[4];
        rw::Field<uint64_t> deckMedals[5];        // +24 userMedalIds (the array is not bounds-checked)
        rw::Field<int32_t> burst;                 // +64
        rw::Field<int32_t> totalAttack;           // +68
        rw::Field<int32_t> totalDefense;          // +72
        rw::Field<uint8_t> isFavorite;            // +76
        const uint8_t pad_77[3];
        rw::Field<int32_t> getDatetime;           // +80 seconds since 1970
        const uint8_t pad_84[4];

        static uint32_t init(Keyblade *self, const JsonValue *json);
    };

    static uint32_t parse(SharedPtr *out, const JsonValue *response);          // response["userKeyblades"]
    static uint32_t create(SharedPtr *out, const StdVector *keyblades);
    static UserKeyblade *destroy(UserKeyblade *self);                          // D2
    static uint32_t destroyDelete(UserKeyblade *self);                         // D0
};
RW_LAYOUT(UserKeyblade::Keyblade, deckMedals, 24);
RW_LAYOUT(UserKeyblade::Keyblade, isFavorite, 76);
RW_SIZE(UserKeyblade::Keyblade, 88);
RW_SIZE(UserKeyblade, 56);

// Jewels: {"userStone": {"freeStone", "payStone"}} in a SafeBuffer of 8 bytes.
struct UserStone : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF7F51C;
    static constexpr uint32_t FREE_STONE = 0;
    static constexpr uint32_t PAY_STONE = 4;

    static uint32_t parse(SharedPtr *out, const JsonValue *response);
    static uint32_t destroyDelete(UserStone *self);                            // D0
};
RW_SIZE(UserStone, 32);

}  // namespace hole::network::api
