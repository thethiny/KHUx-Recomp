// RaidBossActor: the prizes that erupt from the boss (coins / burst coins when a part is hit or crushed, munny, Lux
// and EXP by the share of HP taken) and the drop item; the attribute advantage effect on the HP gauges.
#include "RaidBossActor_common.hpp"

using namespace rba;

namespace {
constexpr uint32_t FROM_ENEMY = 1;                  // MapPrizes::FromGeneration
constexpr int32_t ITEM_MUNNY = 4, ITEM_BURST_COIN = 8, ITEM_COIN = 9, ITEM_LUX = 11;
constexpr uint32_t INFO_ITEMS = 8, ITEM_SIZE = 16, ITEM_COUNT = 8;   // MapPrizes::Info layout
constexpr uint32_t INFO_LUX_STYLE = 76;

// MapPrizes::Info(Vec2::ZERO, FromGeneration enemy) at `info`
void makeInfo(uint32_t info, uint32_t vecTemp) {
    rw::call(fn::Vec2_copy, {vecTemp, fn::Vec2_ZERO});
    rw::call(fn::MapPrizes_Info_ctor, {info, vecTemp, FROM_ENEMY});
    rw::call(fn::Vec2_dtor, {vecTemp});
}
void setItem(uint32_t info, uint32_t slot, int32_t type, uint32_t count) {
    rw::save<int32_t>(info + INFO_ITEMS + ITEM_SIZE * slot, type);
    rw::save<uint32_t>(info + INFO_ITEMS + ITEM_SIZE * slot + ITEM_COUNT, count);
}
// MapPrizes::create(info, field node) and its eruption from the part's marker
void erupt(RaidBossActor *self, uint32_t info, int32_t part) {
    uint32_t prizes = rw::call(fn::MapPrizes_create, {info, self->fieldNode});
    rw::call(self_fn::addPrizes, {addr(self), prizes, uint32_t(part)});
}
float rate10000(int32_t v) { return float(v) / 10000.0f; }
float urate10000(uint32_t v) { return float(v) / 10000.0f; }
// the player's max HP (user status data +56)
int32_t userMaxHp() {
    uint32_t api = rw::call(fn::APIManager_getInstance, {});
    return get32(ld32(api + APIMANAGER_USER_STATUS), 56);
}
// master::BattleMisc(id) value / 10000
float battleMiscRate(uint32_t at, uint32_t id) {
    rw::call(fn::master_BattleMisc_create, {at, id});
    float r = rate10000(get32(ld32(at), 4));
    releaseShared(ld32(at + 4));
    return r;
}
}  // namespace

// A crushed part: coins by the player's max HP x hpSuppress, burst coins by burstSuppress.
void RaidBossActor::addCrushingPrizes(RaidBossActor *self, int32_t part) {
    Frame f(0x80);
    uint32_t p = partAddr(self, part);
    uint32_t hpSuppress = getu(p, RaidParts::HP_SUPPRESS);
    uint32_t burstSuppress = getu(p, RaidParts::BURST_SUPPRESS);
    float hpRate = urate10000(hpSuppress);
    float burstRate = urate10000(burstSuppress);
    // (reads the skill's +300 into a local the Info then overwrites)
    uint32_t info = f.addr(0x28);
    makeInfo(info, f.addr(0x1C));
    float coins = hpRate * float(userMaxHp());
    uint32_t coinCount = ceilU(coins);
    bool any = false;
    if (coinCount) {
        setItem(info, 0, ITEM_COIN, coinCount);
        any = true;
    }
    uint32_t burstCount = ceilU(burstRate * 10000.0f);
    if (burstCount) {
        setItem(info, 1, ITEM_BURST_COIN, burstCount);
        erupt(self, info, part);
    } else if (any) {
        erupt(self, info, part);
    }
    rw::call(fn::Vec2_dtor, {info});
}
RW_FUNCTION(0x5F57C8, "RaidBossActor::addCrushingPrizes", RaidBossActor::addCrushingPrizes);

// A crushed part: munny / Lux / EXP by the share of HP, at least the part's suppress values.
void RaidBossActor::addCrushingPrizesLux(RaidBossActor *self, int32_t part, int32_t hp) {
    Frame f(0x40);
    uint32_t money = f.addr(0x30), lux = f.addr(0x2C), exp = f.addr(0x28);
    rw::call(self_fn::prizesLuxCommon, {addr(self), uint32_t(part), uint32_t(hp), money, lux, exp});
    uint32_t p = partAddr(self, part);
    uint32_t luxSuppress = getu(p, RaidParts::LUX_SUPPRESS);
    uint32_t moneySuppress = getu(p, RaidParts::MONEY_SUPPRESS);
    uint32_t expSuppress = getu(p, RaidParts::EXP_SUPPRESS);
    uint32_t l = ld32(lux), m = ld32(money), e = ld32(exp);
    if (l < luxSuppress) { rw::save<uint32_t>(lux, luxSuppress); l = luxSuppress; }
    if (m < moneySuppress) { rw::save<uint32_t>(money, moneySuppress); m = moneySuppress; }
    if (e < expSuppress) { rw::save<uint32_t>(exp, expSuppress); e = expSuppress; }
    rw::call(self_fn::prizesLuxPostCommon, {addr(self), uint32_t(part), m, l, e, 1});
}
RW_FUNCTION(0x5F59A4, "RaidBossActor::addCrushingPrizesLux", RaidBossActor::addCrushingPrizesLux);

// A hit part (not crushed): coins by the player's max HP x hpAttack, burst coins by burstAttack, each scaled by the
// battle constants (BattleMisc 301 / 302 for coins, 401 / 402 for burst coins; 302 and 402 only apply when the
// part's prize rate B is above 1, 301 / 401 are capped by its rate A), the skill bonuses and divided by `rate`.
void RaidBossActor::addAttackPrizes(RaidBossActor *self, int32_t part, int32_t rate) {
    Frame f(0xF0);
    uint32_t p = partAddr(self, part);
    uint32_t hpAttack = getu(p, RaidParts::HP_ATTACK);
    uint32_t burstAttack = getu(p, RaidParts::BURST_ATTACK);
    uint32_t misc = f.addr(0x70);
    rw::call(fn::master_BattleMisc_create, {misc, 301});
    float hpRate = urate10000(hpAttack);
    float burstRate = urate10000(burstAttack);
    float c301 = rate10000(get32(ld32(misc), 4));
    releaseShared(ld32(misc + 4));
    float c401 = battleMiscRate(f.addr(0x68), 401);
    float c302 = battleMiscRate(f.addr(0x60), 302);
    float c402 = battleMiscRate(f.addr(0x58), 402);

    uint32_t param = addr(self) + 1988 + 32u * uint32_t(part);
    float rateA = getf(param, PrizeParam::RATE_A);
    float rateA2 = getf(param, PrizeParam::RATE_A);
    float rateB = getf(param, PrizeParam::RATE_B);
    if (!(rateB > 1.0f) && !(rateB != rateB)) {   // !fcmpgt && !unordered
        c402 = 1.0f;
        c302 = 1.0f;
    }
    if (!(c301 < rateA)) c301 = rateA;
    if (!(c401 < rateA2)) c401 = rateA2;
    float skillCoin = 1.0f, skillBurst = 1.0f;
    if (self->skill.obj) {
        uint32_t s = self->skill.obj;
        int32_t a = get32(s, SKILL_RATE_292);
        int32_t b = get32(self->skill.obj, SKILL_RATE_296);
        skillCoin = rate10000(a);
        skillBurst = rate10000(b);
    }
    uint32_t info = f.addr(0x90);
    makeInfo(info, f.addr(0x50));
    float maxHp = float(userMaxHp());
    float divisor = float(rate);
    float coins = skillCoin * (c301 * (c302 * (hpRate * maxHp)));
    uint32_t coinCount = ceilU(coins / divisor);
    bool any = false;
    if (coinCount) {
        setItem(info, 0, ITEM_COIN, coinCount);
        any = true;
    }
    float burst = ((c401 * ((burstRate * 10000.0f) * c402)) * skillBurst) / divisor;
    uint32_t burstCount = ceilU(burst);
    if (burstCount) {
        setItem(info, 1, ITEM_BURST_COIN, burstCount);
        erupt(self, info, part);
    } else if (any) {
        erupt(self, info, part);
    }
    rw::call(fn::Vec2_dtor, {info});
}
RW_FUNCTION(0x5F5AA4, "RaidBossActor::addAttackPrizes", RaidBossActor::addAttackPrizes);

// A hit part: munny / Lux / EXP by the share of HP the damage took.
void RaidBossActor::addAttackPrizesLux(RaidBossActor *self, int32_t part, int32_t damage) {
    Frame f(0x20);
    uint32_t money = f.addr(0x14), lux = f.addr(0x10), exp = f.addr(0xC);
    rw::call(self_fn::prizesLuxCommon, {addr(self), uint32_t(part), uint32_t(damage), money, lux, exp});
    rw::call(self_fn::prizesLuxPostCommon, {addr(self), uint32_t(part), ld32(money), ld32(lux), ld32(exp), 0});
}
RW_FUNCTION(0x5F5F10, "RaidBossActor::addAttackPrizesLux", RaidBossActor::addAttackPrizesLux);

// Light up the gauge of a part (the body: the status indicator) when the attack's attribute beats the part's type:
// 1 beats 2, 2 beats 3, 3 beats 1.
void RaidBossActor::setAdvantageousEffect(RaidBossActor *self, int32_t attribute, int32_t part) {
    Frame f(0x18);
    int32_t type = partGet(self, part, RaidParts::TYPE);
    uint32_t indicator;
    if (part) {
        uint32_t key = f.addr(0xC);
        rw::save<int32_t>(key, part);
        uint32_t node = findMarkerNode(self, key);
        if (!node || !ld32(node + 12)) return;
        rw::save<int32_t>(key, part);
        node = findMarkerNode(self, key);
        uint32_t marker = node ? ld32(node + 12) : 0;
        indicator = ld32(marker + MARKER_HP_INDICATOR);
    } else {
        indicator = self->statusIndicator;
        if (!indicator) return;
    }
    uint32_t suitable = 1;
    if (!((attribute == 1 && type == 2) || (attribute == 2 && type == 3)))
        suitable = (attribute == 3 ? 1u : 0u) & (type == 1 ? uint32_t(type) : 0u);
    rw::call(fn::RaidBossHPIndicator_setSuitableAsTargetEffect, {indicator, suitable});
}
RW_FUNCTION(0x5F5F90, "RaidBossActor::setAdvantageousEffect", RaidBossActor::setAdvantageousEffect);

// Keep the prizes and let them erupt from the boss (x jittered by -8..8) towards the part's marker.
void RaidBossActor::addPrizes(RaidBossActor *self, uint32_t prizes, int32_t part) {
    uint32_t a = addr(self);
    Frame f(0x20);
    rw::call(fn::VectorMapPrizes_pushBack, {a + 1976, prizes});
    uint32_t from = f.addr(0x10), to = f.addr(8);
    rw::call(fn::Vec2_copy, {from, a + 624});
    uint32_t random = rw::call(fn::Random_getInstance, {});
    int32_t jitter = int32_t(rw::call(fn::Random_generateUniformInt, {random, uint32_t(-2), 2, 0}));
    rw::save<float>(from, float(int32_t(uint32_t(jitter) << 2)) + rw::load<float>(from));
    rw::call(self_fn::find_marker, {to, a, uint32_t(part)});
    rw::call(fn::MapPrizes_eruption, {prizes, from, to});
    rw::call(fn::Vec2_dtor, {to});
    rw::call(fn::Vec2_dtor, {from});
}
RW_FUNCTION(0x5F6070, "RaidBossActor::addPrizes", RaidBossActor::addPrizes);

void RaidBossActor::setDropItem(RaidBossActor *self, uint32_t prizes) {
    self->dropItem = prizes;
    if (prizes) rw::call(fn::VectorMapPrizes_pushBack, {addr(self) + 1976, prizes});
}
RW_FUNCTION(0x5F6134, "RaidBossActor::setDropItem", RaidBossActor::setDropItem);

// The drop item erupts from the boss towards the part's marker (once).
void RaidBossActor::addDropItem(RaidBossActor *self, int32_t part) {
    uint32_t a = addr(self);
    if (!self->dropItem) return;
    Frame f(0x18);
    uint32_t to = f.addr(8);
    rw::call(self_fn::find_marker, {to, a, uint32_t(part)});
    rw::call(fn::MapPrizes_eruption, {self->dropItem, a + 624, to});
    self->dropItem = 0;
    rw::call(fn::Vec2_dtor, {to});
}
RW_FUNCTION(0x5F614C, "RaidBossActor::addDropItem", RaidBossActor::addDropItem);

// Munny, Lux and EXP for `amount` HP of a part: the part's money / Lux / EXP attack values x amount / max HP; munny
// and Lux rounded up after the skill's bonuses (+304 munny, +300 Lux), EXP truncated.
void RaidBossActor::prizesLuxCommon(RaidBossActor *self, int32_t part, int32_t amount, const uint32_t *money,
                                    const uint32_t *lux, const uint32_t *exp) {
    uint32_t p = partAddr(self, part);
    uint32_t maxHp = getu(p, RaidParts::MAX_HP);
    uint32_t luxAttack = getu(p, RaidParts::LUX_ATTACK);
    uint32_t moneyAttack = getu(p, RaidParts::MONEY_ATTACK);
    uint32_t expAttack = getu(p, RaidParts::EXP_ATTACK);
    float share = float(amount) / float(maxHp);
    float luxF = share * float(luxAttack);
    float moneyF = share * float(moneyAttack);
    float expF = share * float(expAttack);
    float luxBonus = 1.0f, moneyBonus = 1.0f;
    if (self->skill.obj) {
        int32_t a = get32(self->skill.obj, SKILL_RATE_300);
        int32_t b = get32(self->skill.obj, SKILL_RATE_304);
        luxBonus = rate10000(a);
        moneyBonus = rate10000(b);
    }
    rw::store32(lux, ceilU(luxF * luxBonus));
    rw::store32(money, ceilU(moneyF * moneyBonus));
    rw::store32(exp, f2u(expF));
}
RW_FUNCTION(0x5F61D8, "RaidBossActor::prizesLuxCommon", RaidBossActor::prizesLuxCommon);

// Erupt the munny / Lux (a crushed part's Lux uses the big Lux number) and add them and the EXP to the stage
// state (the raid's Lux total too).
void RaidBossActor::prizesLuxPostCommon(RaidBossActor *self, int32_t part, uint32_t money, uint32_t lux, uint32_t exp,
                                        uint32_t crushed) {
    Frame f(0x80);
    uint32_t info = f.addr(0x28);
    makeInfo(info, f.addr(0x1C));
    bool any = false;
    if (lux) {
        setItem(info, 2, ITEM_LUX, lux);
        any = true;
        if (crushed == 1) rw::save<uint32_t>(info + INFO_LUX_STYLE, 1);
    }
    if (money) {
        setItem(info, 3, ITEM_MUNNY, money);
        erupt(self, info, part);
    } else if (any) {
        erupt(self, info, part);
    }
    uint32_t stage = ld32(rw::call(fn::AppState_getInstance, {}) + APPSTATE_STAGE_STATE);
    uint32_t raidLux = getu(ld32(rw::call(fn::AppState_getInstance, {}) + APPSTATE_STAGE_STATE), STAGESTATE_RAID_LUX);
    set32(stage, STAGESTATE_RAID_LUX, int32_t(raidLux + lux));
    rw::call(fn::StageState_addLUX, {ld32(rw::call(fn::AppState_getInstance, {}) + APPSTATE_STAGE_STATE), lux});
    rw::call(fn::StageState_addMoney, {ld32(rw::call(fn::AppState_getInstance, {}) + APPSTATE_STAGE_STATE), money});
    rw::call(fn::StageState_addEXP, {ld32(rw::call(fn::AppState_getInstance, {}) + APPSTATE_STAGE_STATE), exp});
    rw::call(fn::Vec2_dtor, {info});
}
RW_FUNCTION(0x5F63E8, "RaidBossActor::prizesLuxPostCommon", RaidBossActor::prizesLuxPostCommon);
