// RaidParts::set_master_param: a raid boss part's stats for the boss level, from its master::RaidEnemy row.
#include "RaidBossActor.hpp"

namespace {

constexpr uint32_t DIVSI3 = 0xCD5E8D;   // __divsi3 (division by zero goes to the guest helper)

// master::RaidEnemy data positions (a SafeBuffer)
namespace re {
constexpr uint32_t TYPE = 0x88;               // the marker attribute type
constexpr uint32_t FIELD_90 = 0x90;           // copied to data position 4
constexpr uint32_t LEVEL_COUNT = 0xAC;        // brackets used (of 5)
constexpr uint32_t LEVELS = 0xB0;             // int[5]: the bracket's start level
// int[5] per bracket from here, 0x14 bytes apart: max HP, attack, defense, then the 11 reward rates
constexpr uint32_t MAX_HP = 0xC4;
constexpr uint32_t ATTACK = 0xD8;
constexpr uint32_t DEFENSE = 0xEC;
constexpr uint32_t EXP_ATTACK = 0x100;        // .. GUILT_SUPPRESS = 0x1C8, the order of RaidParts' fields 32..72
constexpr uint32_t REWARD_ID = 0x1DC;         // reward ids: taken from the bracket, never interpolated
constexpr uint32_t MVP_REWARD_ID = 0x1F0;
constexpr uint32_t DESTROY_ALL_REWARD_ID = 0x204;
constexpr uint32_t ARRAY = 0x14;
}  // namespace re

int32_t sdiv(int32_t a, int32_t b) {
    if (b == 0) return int32_t(rw::call(DIVSI3, {uint32_t(a), 0}));
    if (b == -1) return int32_t(0u - uint32_t(a));
    return a / b;
}

}  // namespace

namespace RaidPartsFns {

// Picks the bracket of `level` (the last one whose start level is not above it) and, when a next bracket exists,
// interpolates max HP, attack, defense and the reward rates linearly towards it (integer arithmetic, like the
// original: (next - this) * (level - start) / (nextStart - start) + this). hp < 0: start at max HP.
// Returns the bracket's start level.
int32_t set_master_param(RaidParts *self, uint32_t masterPtr, int32_t level, int32_t id, int32_t hp) {
    const SafeBuffer *m = rw::host_ptr<SafeBuffer>(rw::load<uint32_t>(masterPtr));   // shared_ptr's object
    auto at = [&](uint32_t array, int32_t k) { return m->get32(array + uint32_t(k) * 4); };

    const int32_t count = m->get32(re::LEVEL_COUNT);
    int32_t lo, hi = -1;                         // the bracket, and the next one (-1: none, no interpolation)
    int32_t startLo = at(re::LEVELS, 0), startHi = -1;
    if (startLo >= level) {
        lo = 0;
        startHi = startLo;
    } else if (count < 2) {
        lo = count - 1;
    } else {
        int32_t k = 1;
        for (; k < count; k++) {
            const int32_t start = at(re::LEVELS, k);
            if (start > level) break;
            startLo = start;
        }
        if (k < count) {
            startHi = at(re::LEVELS, k);
            hi = k;
            lo = k - 1;
        } else {
            startHi = startLo;
            lo = count - 1;
        }
    }

    int32_t maxHp = at(re::MAX_HP, lo);
    int32_t attack = at(re::ATTACK, lo);
    int32_t defense = at(re::DEFENSE, lo);
    int32_t rates[11];
    for (int k = 0; k < 11; k++) rates[k] = at(re::EXP_ATTACK + uint32_t(k) * re::ARRAY, lo);
    const int32_t rewardId = at(re::REWARD_ID, lo);
    const int32_t mvpRewardId = at(re::MVP_REWARD_ID, lo);
    const int32_t destroyAllRewardId = at(re::DESTROY_ALL_REWARD_ID, lo);
    if (hi >= 0) {
        const uint32_t dl = uint32_t(level - startLo), d = uint32_t(startHi - startLo);
        auto lerp = [&](int32_t from, int32_t to) {
            return int32_t(uint32_t(sdiv(int32_t((uint32_t(to) - uint32_t(from)) * dl), int32_t(d))) + uint32_t(from));
        };
        maxHp = lerp(maxHp, at(re::MAX_HP, hi));
        for (int k = 0; k < 11; k++) rates[k] = lerp(rates[k], at(re::EXP_ATTACK + uint32_t(k) * re::ARRAY, hi));
        attack = lerp(attack, at(re::ATTACK, hi));
        defense = lerp(defense, at(re::DEFENSE, hi));
    }

    for (int k = 0; k < 11; k++) self->set32(RaidParts::EXP_ATTACK + uint32_t(k) * 4, rates[k]);
    self->set32(RaidParts::REWARD_ID, rewardId);
    self->set32(RaidParts::MVP_REWARD_ID, mvpRewardId);
    self->set32(RaidParts::DESTROY_ALL_REWARD_ID, destroyAllRewardId);
    self->set32(RaidParts::HP, hp >= 0 ? hp : maxHp);
    self->set32(0, id);
    self->set32(RaidParts::DAMAGE, 0);
    self->set32(4, m->get32(re::FIELD_90));
    self->set32(RaidParts::ATTACK, attack);
    self->set32(RaidParts::MAX_HP, maxHp);
    self->set32(RaidParts::DEFENSE, defense);
    self->set32(RaidParts::TYPE, m->get32(re::TYPE));
    return at(re::LEVELS, lo);
}

}  // namespace RaidPartsFns

RW_FUNCTION(0x5F1550, "RaidParts::set_master_param", RaidPartsFns::set_master_param);
