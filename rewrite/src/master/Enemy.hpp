// master::Enemy: rows of the master table #15.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Enemy : Row {
    static constexpr uint32_t TABLE = 15;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab294;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab2bc;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08a55;                  // "a050578fce9813a41fd1073e2408830908aedb42"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfab2e8;                // _ZTVN6master5EnemyE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x780D9D;       // shared_ptr<const Enemy>(Enemy*)
    static constexpr uint32_t SIZE = 0x3e0;

    // data positions (JSON keys)
    static constexpr uint32_t ENEMY_ID = 0;                 // int32
    static constexpr uint32_t NAME = 4;                     // char[65]
    static constexpr uint32_t FLAVOR = 69;                  // char[513]
    static constexpr uint32_t DISPLAY_ID = 584;             // int32
    static constexpr uint32_t SHOW = 588;                   // int32
    static constexpr uint32_t SHOW_SWF = 592;               // int32
    static constexpr uint32_t SHOW_FRAME = 596;             // int32
    static constexpr uint32_t HIDE_FRAME = 600;             // int32
    static constexpr uint32_t MOVE_SPEED = 604;             // int32
    static constexpr uint32_t HIT_RADIUS = 608;             // int32
    static constexpr uint32_t TAP_RADIUS = 612;             // int32
    static constexpr uint32_t WIDTH = 616;                  // int32
    static constexpr uint32_t HEIGHT = 620;                 // int32
    static constexpr uint32_t SP_BATTLE = 624;              // int32
    static constexpr uint32_t KIND = 628;                   // int32
    static constexpr uint32_t ATTRIBUTE = 632;              // int32
    static constexpr uint32_t VALID_GROWTH = 636;           // int32
    static constexpr uint32_t BASE_LV = 640;                // int32[5]
    static constexpr uint32_t HP = 660;                     // int32[5]
    static constexpr uint32_t ATTACK = 680;                 // int32[5]
    static constexpr uint32_t DEFENSE = 700;                // int32[5]
    static constexpr uint32_t EXP = 720;                    // int32[5]
    static constexpr uint32_t MONEY = 740;                  // int32[5]
    static constexpr uint32_t RAID_POINT = 760;             // int32[5]
    static constexpr uint32_t LUX = 780;                    // int32[5]
    static constexpr uint32_t ATTACK_CP = 800;              // int32
    static constexpr uint32_t SUPPRESS_CP = 804;            // int32
    static constexpr uint32_t ATTACK_HP = 808;              // int32
    static constexpr uint32_t SUPPRESS_HP = 812;            // int32
    static constexpr uint32_t SUPPRESS_GUILT = 816;         // int32
    static constexpr uint32_t BATTLE_PATTERN = 820;         // int32
    static constexpr uint32_t DISPLAY_HEIGHT = 824;         // int32
    static constexpr uint32_t RUNAWAY = 828;                // int32
    static constexpr uint32_t NEVER_ATTACK = 832;           // int32
    static constexpr uint32_t VALID_SKILL = 836;            // int32
    static constexpr uint32_t SKILL_ID = 840;               // int32[4]
    static constexpr uint32_t SKILL_REQUIRE = 856;          // int32[4]
    static constexpr uint32_t SKILL_REQUIRE_ARG = 872;      // int32[4]
    static constexpr uint32_t SKILL_ODDS = 888;             // int32[4]
    static constexpr uint32_t PROTECT_POISON = 904;         // int32
    static constexpr uint32_t PROTECT_DEEP_POISON = 908;    // int32
    static constexpr uint32_t PROTECT_SLEEP = 912;          // int32
    static constexpr uint32_t PROTECT_PARALYSIS = 916;      // int32
    static constexpr uint32_t REGIST_POISON = 920;          // int32
    static constexpr uint32_t REGIST_DEEP_POISON = 924;     // int32
    static constexpr uint32_t REGIST_SLEEP = 928;           // int32
    static constexpr uint32_t REGIST_PARALYSIS = 932;       // int32
    static constexpr uint32_t BUF_ATTACK = 936;             // int32
    static constexpr uint32_t BUF_DEFENSE = 940;            // int32
    static constexpr uint32_t BUF_ATTACK_POWER = 944;       // int32
    static constexpr uint32_t BUF_DEFENSE_POWER = 948;      // int32
    static constexpr uint32_t BUF_ATTACK_SPEED = 952;       // int32
    static constexpr uint32_t BUF_DEFENSE_SPEED = 956;      // int32
    static constexpr uint32_t BUF_ATTACK_MAGIC = 960;       // int32
    static constexpr uint32_t BUF_DEFENSE_MAGIC = 964;      // int32
    static constexpr uint32_t BUF_TURN = 968;               // int32
    static constexpr uint32_t BUF_COUNT = 972;              // int32
    static constexpr uint32_t STATUS_AILMENTS = 976;        // int32[2]
    static constexpr uint32_t STATUS_AILMENTS_TURN = 984;   // int32
    static constexpr uint32_t STATUS_AILMENTS_COUNT = 988;  // int32

    // shared_ptr<const Enemy> from one JSON row (null when a field is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
