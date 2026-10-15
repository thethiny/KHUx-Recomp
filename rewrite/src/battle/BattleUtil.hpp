// BattleUtil: static helpers for medal stats and experience curves, the equipped keyblade and the friend's
// support medal. Medal growth follows master::MedalMisc curves: value = base + (max - base) *
// ((level - 1) / (maxLevel - 1)) ^ (exponent / 10000).
#pragma once

#include "BattleCommon.hpp"

namespace battle_util {
// master::Medal data positions (SafeBuffer)
constexpr uint32_t MEDAL_ATTRIBUTE = 0x458;     // compared with the keyblade's slot attributes
constexpr uint32_t MEDAL_TYPE = 0x45C;          // compared with the keyblade's slot types
constexpr uint32_t MEDAL_MAX_LEVEL = 0x47C;
constexpr uint32_t MEDAL_EXP_CURVE = 0x480;     // 1..6 -> MedalMisc 106..111
constexpr uint32_t MEDAL_STAT_CURVE = 0x48C;    // 1..5 -> MedalMisc 101..105
constexpr uint32_t MEDAL_ATTACK_MIN = 0x490;
constexpr uint32_t MEDAL_ATTACK_MAX = 0x494;
constexpr uint32_t MEDAL_DEFENSE_MIN = 0x49C;
constexpr uint32_t MEDAL_DEFENSE_MAX = 0x4A0;
// master::Keyblade data positions
constexpr uint32_t KEYBLADE_SLOT_ATTRIBUTE = 0xE8;  // int[5]
constexpr uint32_t KEYBLADE_SLOT_TYPE = 0xFC;       // int[5]
constexpr uint32_t KEYBLADE_SLOT_BONUS = 0x110;     // int[5] multiplier (x10000)
constexpr uint32_t KEYBLADE_SLOT_BONUS_TYPED = 0x124;  // int[5] multiplier when the medal type matches
constexpr uint32_t KEYBLADE_SUPPORT_BONUS = 0x138;  // multiplier for a medal outside the slots
// hole::network::api::UserMedal::Medal
constexpr uint32_t USER_MEDAL_LEVEL = 0x0C;
constexpr uint32_t USER_MEDAL_EXP = 0x10;
constexpr uint32_t USER_MEDAL_ATTACK_BONUS = 0x14;
constexpr uint32_t USER_MEDAL_DEFENSE_BONUS = 0x18;
}  // namespace battle_util

struct BattleUtil {
    // Attack / defense of a user medal (with the keyblade slot multiplier for slot `slot`, -1 = support),
    // reported through onResult(attack, defense).
    static void calcMedalStatus(const bt::SharedPtr *medal, const uint8_t *userMedal, uint32_t onResult,
                                const bt::SharedPtr *keyblade, int32_t slot);
    // Level after adding `exp`; onResult(level, exp, needForLevel-1, needForLevel). Returns the level.
    static int32_t calcMedalExp(const bt::SharedPtr *medal, const uint8_t *userMedal, uint32_t onResult, int32_t exp);
    // Experience needed for `level`.
    static int32_t calcNeedExp(const bt::SharedPtr *medal, int32_t level);
    // Index of the equipped keyblade in the user's keyblade list (0 when none).
    static uint32_t getCurrentKeybladeIndex();
    // The friend's support medal (copied to `medal`) and its skill; 1 when found.
    static uint32_t getSupportMedal(const uint8_t *medal, const uint8_t *skill);
    static int32_t calc_many_lux_level(int32_t a, int32_t b, int32_t c);
};
