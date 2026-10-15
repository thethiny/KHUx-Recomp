// master::Burst: rows of the master table #6.
#pragma once

#include "MasterTable.hpp"

namespace master {

struct Burst : Row {
    static constexpr uint32_t TABLE = 6;
    static constexpr uint32_t CREATE_BLOCK_VTABLE = 0xfaae14;   // __shared_ptr_emplace<create(int)::impl> + 8
    static constexpr uint32_t ROW_VTABLE = 0xfaae3c;            // create(int)::impl + 8
    static constexpr uint32_t HASH = 0xe0845f;                  // "fc5daf97d0263b59d93f95bfd060bcab60f4e86f"

    // data positions of the row's fields (JSON key -> position in the SafeBuffer)
    static constexpr uint32_t BURST_ID = 0;                 // int32
    static constexpr uint32_t MOTION_ID = 4;                // int32
    static constexpr uint32_t EFFECT_ID = 8;                // int32
    static constexpr uint32_t DISTANCE = 12;                // int32
    static constexpr uint32_t EFFECT_DIRECTION_FIX = 16;    // int32
    static constexpr uint32_t NAME = 20;                    // char[65]
    static constexpr uint32_t DESCRIPTION = 85;             // char[257]
    static constexpr uint32_t MAX_ENHANCE_COUNT = 344;      // int32
    static constexpr uint32_t ATTRIBUTE = 348;              // int32
    static constexpr uint32_t TARGET = 352;                 // int32
    static constexpr uint32_t HIT_COUNT = 356;              // int32
    static constexpr uint32_t GAUGE = 360;                  // int32
    static constexpr uint32_t CRITICAL = 364;               // int32
    static constexpr uint32_t CRITICAL_POWER = 368;         // int32
    static constexpr uint32_t DISPLAY_POWER = 372;          // int32
    static constexpr uint32_t DISPLAY_POWER_PER_ENFORCE = 376; // int32[5]
    static constexpr uint32_t DISPLAY_MAX_POWER = 396;      // int32
    static constexpr uint32_t DISPLAY_MAX_POWER_PER_ENFORCE = 400; // int32[5]
    static constexpr uint32_t BASE_POWER = 420;             // int32
    static constexpr uint32_t BASE_POWER_PER_ENFORCE = 424; // int32[5]
    static constexpr uint32_t DAMAGE_TYPE = 444;            // int32
    static constexpr uint32_t SPECIAL_POWER_TYPE = 448;     // int32
    static constexpr uint32_t SP1 = 452;                    // int32
    static constexpr uint32_t SP2 = 456;                    // int32
    static constexpr uint32_t SP1_PER_ENFORCE = 460;        // int32[5]
    static constexpr uint32_t SP2_PER_ENFORCE = 480;        // int32[5]
    static constexpr uint32_t ADD_TURN = 500;               // int32
    static constexpr uint32_t ADD_COUNT = 504;              // int32
    static constexpr uint32_t BUF_ATTACK = 508;             // int32
    static constexpr uint32_t DEBUF_ATTACK = 512;           // int32
    static constexpr uint32_t DEBUF_ATTACK_ALL = 516;       // int32
    static constexpr uint32_t BUF_DEFENSE = 520;            // int32
    static constexpr uint32_t DEBUF_DEFENSE = 524;          // int32
    static constexpr uint32_t DEBUF_DEFENSE_ALL = 528;      // int32
    static constexpr uint32_t BUF_ATTACK_POWER = 532;       // int32
    static constexpr uint32_t DEBUF_ATTACK_POWER = 536;     // int32
    static constexpr uint32_t DEBUF_ATTACK_POWER_ALL = 540; // int32
    static constexpr uint32_t BUF_DEFENSE_POWER = 544;      // int32
    static constexpr uint32_t DEBUF_DEFENSE_POWER = 548;    // int32
    static constexpr uint32_t DEBUF_DEFENSE_POWER_ALL = 552; // int32
    static constexpr uint32_t BUF_ATTACK_SPEED = 556;       // int32
    static constexpr uint32_t DEBUF_ATTACK_SPEED = 560;     // int32
    static constexpr uint32_t DEBUF_ATTACK_SPEED_ALL = 564; // int32
    static constexpr uint32_t BUF_DEFENSE_SPEED = 568;      // int32
    static constexpr uint32_t DEBUF_DEFENSE_SPEED = 572;    // int32
    static constexpr uint32_t DEBUF_DEFENSE_SPEED_ALL = 576; // int32
    static constexpr uint32_t BUF_ATTACK_MAGIC = 580;       // int32
    static constexpr uint32_t DEBUF_ATTACK_MAGIC = 584;     // int32
    static constexpr uint32_t DEBUF_ATTACK_MAGIC_ALL = 588; // int32
    static constexpr uint32_t BUF_DEFENSE_MAGIC = 592;      // int32
    static constexpr uint32_t DEBUF_DEFENSE_MAGIC = 596;    // int32
    static constexpr uint32_t DEBUF_DEFENSE_MAGIC_ALL = 600; // int32
    static constexpr uint32_t RESET_BUF_ATTTACK = 604;      // int32
    static constexpr uint32_t RESET_DEBUF_ATTACK = 608;     // int32
    static constexpr uint32_t RESET_DEBUF_ATTACK_ALL = 612; // int32
    static constexpr uint32_t RESET_BUF_DEFENSE = 616;      // int32
    static constexpr uint32_t RESET_DEBUF_DEFENSE = 620;    // int32
    static constexpr uint32_t RESET_DEBUF_DEFENSE_ALL = 624; // int32
    static constexpr uint32_t RESET_BUF_ATTTACK_POWER = 628; // int32
    static constexpr uint32_t RESET_DEBUF_ATTACK_POWER = 632; // int32
    static constexpr uint32_t RESET_DEBUF_ATTACK_POWER_ALL = 636; // int32
    static constexpr uint32_t RESET_BUF_DEFENSE_POWER = 640; // int32
    static constexpr uint32_t RESET_DEBUF_DEFENSE_POWER = 644; // int32
    static constexpr uint32_t RESET_DEBUF_DEFENSE_POWER_ALL = 648; // int32
    static constexpr uint32_t RESET_BUF_ATTTACK_SPEED = 652; // int32
    static constexpr uint32_t RESET_DEBUF_ATTACK_SPEED = 656; // int32
    static constexpr uint32_t RESET_DEBUF_ATTACK_SPEED_ALL = 660; // int32
    static constexpr uint32_t RESET_BUF_DEFENSE_SPEED = 664; // int32
    static constexpr uint32_t RESET_DEBUF_DEFENSE_SPEED = 668; // int32
    static constexpr uint32_t RESET_DEBUF_DEFENSE_SPEED_ALL = 672; // int32
    static constexpr uint32_t RESET_BUF_ATTTACK_MAGIC = 676; // int32
    static constexpr uint32_t RESET_DEBUF_ATTACK_MAGIC = 680; // int32
    static constexpr uint32_t RESET_DEBUF_ATTACK_MAGIC_ALL = 684; // int32
    static constexpr uint32_t RESET_BUF_DEFENSE_MAGIC = 688; // int32
    static constexpr uint32_t RESET_DEBUF_DEFENSE_MAGIC = 692; // int32
    static constexpr uint32_t RESET_DEBUF_DEFENSE_MAGIC_ALL = 696; // int32
    static constexpr uint32_t COPY_BUF_BASE = 700;          // int32
    static constexpr uint32_t COPY_BUF_ATTRIBUTE = 704;     // int32
    static constexpr uint32_t HEAL = 708;                   // int32
    static constexpr uint32_t HEAL_SHAKE = 712;             // int32
    static constexpr uint32_t CURE_POISON = 716;            // int32
    static constexpr uint32_t CURE_SLEEP = 720;             // int32
    static constexpr uint32_t CURE_DEEP_POISON = 724;       // int32
    static constexpr uint32_t CURE_PARALYSIS = 728;         // int32

    static constexpr uint32_t PARSE_VTABLE = 0xfaae68;          // _ZTVN6master5BurstE + 8 (a parsed row)
    // parse(json): a new row from one JSON object of the table (empty when a member is missing or mistyped)
    static void parse(guest::SharedPtr *out, uint32_t json);
};

}  // namespace master
