// master::RaidEnemyAttack: rows of the master table #30.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct RaidEnemyAttack : Row {
    static constexpr uint32_t TABLE = 30;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaba74;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaba9c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe09249;                  // "9c397b06aa2d216496d68ae4b0f4ce8fdca1c212"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer); row size 0xd8
    static constexpr uint32_t RAID_ENEMY_ATTACK_ID = 0x0;   // int32
    static constexpr uint32_t NAME = 0x4;                   // char[65]
    static constexpr uint32_t DISPLAY = 0x48;               // int32
    static constexpr uint32_t CHOISE_ATTRIBUTE = 0x4c;      // int32
    static constexpr uint32_t HIT_COUNT = 0x50;             // int32
    static constexpr uint32_t POWER = 0x54;                 // int32
    static constexpr uint32_t CRITICAL = 0x58;              // int32
    static constexpr uint32_t CRITICAL_POWER = 0x5c;        // int32
    static constexpr uint32_t TARGET = 0x60;                // int32
    static constexpr uint32_t EFFECT = 0x64;                // int32
    static constexpr uint32_t ADD_TARGET = 0x68;            // int32
    static constexpr uint32_t ADD_TURN = 0x6c;              // int32
    static constexpr uint32_t ADD_COUNT = 0x70;             // int32
    static constexpr uint32_t BUF_ATTACK = 0x74;            // int32
    static constexpr uint32_t BUF_DEFENSE = 0x78;           // int32
    static constexpr uint32_t BUF_ATTACK_POWER = 0x7c;      // int32
    static constexpr uint32_t BUF_DEFENSE_POWER = 0x80;     // int32
    static constexpr uint32_t BUF_ATTACK_SPEED = 0x84;      // int32
    static constexpr uint32_t BUF_DEFENSE_SPEED = 0x88;     // int32
    static constexpr uint32_t BUF_ATTACK_MAGIC = 0x8c;      // int32
    static constexpr uint32_t BUF_DEFENSE_MAGIC = 0x90;     // int32
    static constexpr uint32_t RESET_ATTACK = 0x94;          // int32
    static constexpr uint32_t RESET_DEFENSE = 0x98;         // int32
    static constexpr uint32_t RESET_ATTACK_POWER = 0x9c;    // int32
    static constexpr uint32_t RESET_DEFENSE_POWER = 0xa0;   // int32
    static constexpr uint32_t RESET_ATTACK_SPEED = 0xa4;    // int32
    static constexpr uint32_t RESET_DEFENSE_SPEED = 0xa8;   // int32
    static constexpr uint32_t RESET_ATTACK_MAGIC = 0xac;    // int32
    static constexpr uint32_t RESET_DEFENSE_MAGIC = 0xb0;   // int32
    static constexpr uint32_t HEAL = 0xb4;                  // int32
    static constexpr uint32_t HEAL_SHAKE = 0xb8;            // int32
    static constexpr uint32_t POISON = 0xbc;                // int32
    static constexpr uint32_t DEEP_POISON = 0xc0;           // int32
    static constexpr uint32_t SLEEP = 0xc4;                 // int32
    static constexpr uint32_t PARALYSIS = 0xc8;             // int32
    static constexpr uint32_t CURE_POISON = 0xcc;           // int32
    static constexpr uint32_t CURE_SLEEP = 0xd0;            // int32
    static constexpr uint32_t CURE_PARALYSIS = 0xd4;        // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfabac8;      // _ZTVN6master15RaidEnemyAttackE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
