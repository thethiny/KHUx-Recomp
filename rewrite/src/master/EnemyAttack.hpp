// master::EnemyAttack: rows of the master table #14.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct EnemyAttack : Row {
    static constexpr uint32_t TABLE = 14;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfab324;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfab34c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe08b61;                  // "ca81a36f9331bda61ed7f2a6ef1ce21627d2ad95"

    // the object parse() builds
    static constexpr uint32_t VTABLE = 0xfab378;                // _ZTVN6master11EnemyAttackE + 8
    static constexpr uint32_t SHARED_PTR_CTOR = 0x781C25;       // shared_ptr<const EnemyAttack>(EnemyAttack*)
    static constexpr uint32_t SIZE = 0xe4;

    // data positions (JSON keys)
    static constexpr uint32_t ENEMY_ATTACK_ID = 0;          // int32
    static constexpr uint32_t NAME = 4;                     // char[65]
    static constexpr uint32_t ATTACK_MOTION = 72;           // int32
    static constexpr uint32_t HIT_EFFECT = 76;              // int32
    static constexpr uint32_t EFFECT_DIRECTION_FIX = 80;    // int32
    static constexpr uint32_t DISPLAY = 84;                 // int32
    static constexpr uint32_t CHOISE_ATTRIBUTE = 88;        // int32
    static constexpr uint32_t HIT_COUNT = 92;               // int32
    static constexpr uint32_t POWER = 96;                   // int32
    static constexpr uint32_t CRITICAL = 100;               // int32
    static constexpr uint32_t CRITICAL_POWER = 104;         // int32
    static constexpr uint32_t TARGET = 108;                 // int32
    static constexpr uint32_t EFFECT = 112;                 // int32
    static constexpr uint32_t ADD_TARGET = 116;             // int32
    static constexpr uint32_t ADD_TURN = 120;               // int32
    static constexpr uint32_t ADD_COUNT = 124;              // int32
    static constexpr uint32_t BUF_ATTACK = 128;             // int32
    static constexpr uint32_t BUF_DEFENSE = 132;            // int32
    static constexpr uint32_t BUF_ATTACK_POWER = 136;       // int32
    static constexpr uint32_t BUF_DEFENSE_POWER = 140;      // int32
    static constexpr uint32_t BUF_ATTACK_SPEED = 144;       // int32
    static constexpr uint32_t BUF_DEFENSE_SPEED = 148;      // int32
    static constexpr uint32_t BUF_ATTACK_MAGIC = 152;       // int32
    static constexpr uint32_t BUF_DEFENSE_MAGIC = 156;      // int32
    static constexpr uint32_t RESET_ATTACK = 160;           // int32
    static constexpr uint32_t RESET_DEFENSE = 164;          // int32
    static constexpr uint32_t RESET_ATTACK_POWER = 168;     // int32
    static constexpr uint32_t RESET_DEFENSE_POWER = 172;    // int32
    static constexpr uint32_t RESET_ATTACK_SPEED = 176;     // int32
    static constexpr uint32_t RESET_DEFENSE_SPEED = 180;    // int32
    static constexpr uint32_t RESET_ATTACK_MAGIC = 184;     // int32
    static constexpr uint32_t RESET_DEFENSE_MAGIC = 188;    // int32
    static constexpr uint32_t HEAL = 192;                   // int32
    static constexpr uint32_t HEAL_SHAKE = 196;             // int32
    static constexpr uint32_t POISON = 200;                 // int32
    static constexpr uint32_t DEEP_POISON = 204;            // int32
    static constexpr uint32_t SLEEP = 208;                  // int32
    static constexpr uint32_t PARALYSIS = 212;              // int32
    static constexpr uint32_t CURE_POISON = 216;            // int32
    static constexpr uint32_t CURE_SLEEP = 220;             // int32
    static constexpr uint32_t CURE_PARALYSIS = 224;         // int32

    // shared_ptr<const EnemyAttack> from one JSON row (null when a field is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
