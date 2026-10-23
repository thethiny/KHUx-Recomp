// RaidBossActor: bad status rolls, the per-part buff turn / count clocks and the status display (HP indicators,
// the boss's status effects).
#include "RaidBossActor_common.hpp"

using namespace rba;

namespace {
constexpr uint32_t STATUS_PROTOCOL = 680;          // RaidBossActor: Property's IStatusProtocol (+648 + 32)
constexpr uint32_t BUFF_PROTOCOL = 684;            // RaidBossActor: Property's IBuffProtocol (+648 + 36)
constexpr uint32_t BUFF_RECORDS = 64;              // BuffChange records of a parts::Property (12 bytes each)
constexpr uint32_t BUFF_RECORD_SIZE = 12;
constexpr uint32_t RANDOM_ONE = 0x3F800000;        // 1.0f

// A condition of the boss: IStatusProtocol vtable slot 0 (getCondition) into the 12-byte `out`; its count (+8).
int32_t conditionCount(uint32_t a, uint32_t out, int32_t type) {
    uint32_t proto = a + STATUS_PROTOCOL;
    rw::call(ld32(ld32(proto)), {out, proto, uint32_t(type)});
    return ldi(out + 8);
}
// The marker of a part (0 when none); `key` is the caller's int.
uint32_t markerOf(RaidBossActor *self, uint32_t key, int32_t part) {
    rw::save<int32_t>(key, part);
    uint32_t node = findMarkerNode(self, key);
    return node ? ld32(node + 12) : 0;
}
// A marker's RaidBossHPIndicator (the original reads it through a missing marker too: guest address 632)
uint32_t indicatorOf(uint32_t marker) { return ld32(marker + MARKER_HP_INDICATOR); }

// One bad status of the boss on the body marker's indicator and on the boss: shown while it has a count left.
void showCondition(RaidBossActor *self, uint32_t out, int32_t type, uint32_t buffIdx, uint32_t bit, uint32_t update) {
    uint32_t a = addr(self);
    int32_t count = conditionCount(a, out, type);
    uint32_t indicator = indicatorOf(markerOf(self, out, 0));
    if (count) {
        if (indicator) rw::call(fn::RaidBossHPIndicator_set_buf_debuf, {indicator, buffIdx, 21});
        if (!(ld8(a + 944) & bit)) rw::call(update, {self->boss.addr, 1});
    } else {
        if (indicator) rw::call(fn::RaidBossHPIndicator_clr_buf_debuf, {indicator, buffIdx});
        rw::call(update, {self->boss.addr, 0});
    }
}

// A part's buff clocks: every BuffChange record decoded, stepped with `elapse` and written back when it changed.
void elapseParts(RaidBossActor *self, int32_t part, uint32_t elapse) {
    SafeBuffer *buffs = sb(addr(self) + 688 + 36u * uint32_t(part));
    Frame f(16);
    uint32_t rec = f.addr(0);
    for (uint32_t k = 0, at = 0; k < BUFF_RECORDS; k++, at += BUFF_RECORD_SIZE) {
        rw::call(fn::aeabi_memset, {rec, BUFF_RECORD_SIZE, 0});
        SafeBuffer::read(buffs, rw::host_ptr<uint8_t>(rec), buffs->data.get() + at, BUFF_RECORD_SIZE, at);
        if (rw::call(elapse, {rec}) == 1)
            SafeBuffer::write(buffs, buffs->data.get() + at, rw::host_ptr<uint8_t>(rec), BUFF_RECORD_SIZE, at);
    }
}
}  // namespace

// A bad status on the boss, if it is not immune: the RaidEnemy's protect flag for the type blocks it, else its
// resistance (x 10000) is the chance; types past 3 always land (a random number is drawn either way).
void RaidBossActor::raidAddCondition(RaidBossActor *self, int32_t type, int32_t turns, int32_t count) {
    static constexpr uint32_t PROTECT[4] = {RE_PROTECT_POISON, RE_PROTECT_SLEEP, RE_PROTECT_PARALYSIS,
                                            RE_PROTECT_DEEP_POISON};
    static constexpr uint32_t REGIST[4] = {RE_REGIST_POISON, RE_REGIST_SLEEP, RE_REGIST_PARALYSIS,
                                           RE_REGIST_DEEP_POISON};
    uint32_t a = addr(self);
    Frame f(8);
    uint32_t enemy = f.addr(0);
    rw::call(fn::master_RaidEnemy_create, {enemy, uint32_t(self->raidEnemyId.get())});
    float rate = 1.0f;
    bool roll = true;
    if (uint32_t(type) <= 3) {
        if (get32(ld32(enemy), PROTECT[type])) roll = false;
        else rate = float(get32(ld32(enemy), REGIST[type])) / 10000.0f;
    }
    if (roll) {
        uint32_t random = rw::call(fn::Random_getInstance, {});
        float r = rw::bitsf(rw::call(fn::Random_generateUniformFloat, {random, 0, RANDOM_ONE, 0}));
        if (!(rate < r))
            rw::call(fn::IStatusProtocol_addCondition_common, {a + STATUS_PROTOCOL, uint32_t(type), uint32_t(turns), uint32_t(count)});
    }
    releaseShared(ld32(enemy + 4));
}
RW_FUNCTION(0x5F67C4, "RaidBossActor::raidAddCondition", RaidBossActor::raidAddCondition);

void RaidBossActor::elapsePartsTurn(RaidBossActor *self, int32_t part) { elapseParts(self, part, fn::BuffChange_elapseTurn); }
RW_FUNCTION(0x5F6A98, "RaidBossActor::elapsePartsTurn", RaidBossActor::elapsePartsTurn);

void RaidBossActor::elapsePartsCount(RaidBossActor *self, int32_t part) { elapseParts(self, part, fn::BuffChange_elapseCount); }
RW_FUNCTION(0x5F6B38, "RaidBossActor::elapsePartsCount", RaidBossActor::elapsePartsCount);

// The boss's state on screen: its bad statuses (body marker's indicator + the boss's effects; no status at all
// clears the boss's bad-status display), its attack / defence buff levels on the body marker, and each part's
// defence buff levels on that part's marker.
void RaidBossActor::UpdateStatus(RaidBossActor *self) {
    uint32_t a = addr(self);
    uint32_t boss = self->boss.addr;
    self->statusBits = ld32(boss + FLASHNODE_STATUS_BITS);
    Frame f(0x38);
    uint32_t out = f.addr(0x10);                       // the condition (12 bytes); its first word is the marker key
    showCondition(self, out, 0, 16, 1, fn::FlashNode_UpdateStatusPoison);
    showCondition(self, out, 3, 19, 8, fn::FlashNode_UpdateStatusDeepPoison);
    showCondition(self, out, 1, 17, 2, fn::FlashNode_UpdateStatusSleep);
    showCondition(self, out, 2, 18, 4, fn::FlashNode_UpdateStatusParalysis);
    if (!ld32(boss + FLASHNODE_STATUS_BITS)) rw::call(fn::FlashNode_set_bad_status, {boss, 4});

    uint32_t buffs = a + BUFF_PROTOCOL;
    uint32_t level[2][4];
    for (uint32_t type = 0; type < 2; type++)
        for (uint32_t target = 0; target < 4; target++)
            level[type][target] = rw::call(fn::IBuffProtocol_buff_level, {buffs, type, target});
    if (uint32_t ind = indicatorOf(markerOf(self, out, 0)))
        rw::call(fn::RaidBossHPIndicator_set_attack_buff_status, {ind, level[0][0], level[0][1], level[0][2], level[0][3]});
    if (uint32_t ind = indicatorOf(markerOf(self, out, 0)))
        rw::call(fn::RaidBossHPIndicator_set_defence_buff_status, {ind, level[1][0], level[1][1], level[1][2], level[1][3]});
    if (uint32_t ind = indicatorOf(markerOf(self, out, 0))) rw::call(fn::RaidBossHPIndicator_Next_Disp, {ind});

    uint32_t enemy = f.addr(0x08);
    rw::call(fn::master_RaidEnemy_create, {enemy, uint32_t(self->raidEnemyId.get())});
    int32_t validParts = get32(ld32(enemy), RE_VALID_PARTS);
    for (int32_t k = 0; k < validParts; k++) {
        uint32_t ids = f.addr(0x24);                   // the part ids, decoded each time (5 ints)
        SafeBuffer *e = sb(ld32(enemy));
        SafeBuffer::read(e, rw::host_ptr<uint8_t>(ids), e->data.get() + RE_PARTS, 20, RE_PARTS);
        uint32_t pe = f.addr(0);
        rw::call(fn::master_RaidEnemy_create, {pe, ld32(ids + 4u * uint32_t(k))});
        int32_t w = get32(ld32(pe), RE_WEAKNESS);
        uint32_t partBuffs = a + BUFF_PROTOCOL + 36u * uint32_t(w);
        uint32_t d[4];
        for (uint32_t target = 0; target < 4; target++) d[target] = rw::call(fn::IBuffProtocol_buff_level, {partBuffs, 1, target});
        if (markerOf(self, out, w - 1)) {
            if (uint32_t ind = indicatorOf(markerOf(self, out, w - 1)))
                rw::call(fn::RaidBossHPIndicator_set_defence_buff_status, {ind, d[0], d[1], d[2], d[3]});
            if (uint32_t ind = indicatorOf(markerOf(self, out, w - 1))) rw::call(fn::RaidBossHPIndicator_Next_Disp, {ind});
        }
        releaseShared(ld32(pe + 4));
    }
    releaseShared(ld32(enemy + 4));
}
RW_FUNCTION(0x5F6BD8, "RaidBossActor::UpdateStatus", RaidBossActor::UpdateStatus);
