#include "BattleMisc.hpp"

#include <cmath>

namespace {

// ------------------------------------------------------------------------------------------ guest functions
constexpr uint32_t BattleMisc_destruct = 0x742141;          // BattleMisc::~BattleMisc (atexit)
constexpr uint32_t master_BattleMisc_create = 0x779AD1;    // master::BattleMisc::create(int) (sret shared_ptr)
constexpr uint32_t master_Buff_getBuffIds = 0x779F01;      // master::Buff::getBuffIds() (sret std::set<int>)
constexpr uint32_t master_Buff_create = 0x779F1D;          // master::Buff::create(int)
constexpr uint32_t master_Burst_create = 0x77A491;         // master::Burst::create(int)
constexpr uint32_t master_EnemyAttack_create = 0x780F51;   // master::EnemyAttack::create(int)
constexpr uint32_t set_int_destroy = 0x48D9B5;             // std::__tree<int>::destroy(node)
constexpr uint32_t map_insert_unique = 0x4B18D1;           // __hash_table<int,int>::__insert_unique(pair<int,int>&&) (sret)
constexpr uint32_t map_find = 0x494251;                    // __hash_table<int,int>::find(const int&) -> node
constexpr uint32_t map_destruct = 0x4942D3;                // __hash_table<int,int>::~__hash_table()
constexpr uint32_t bitset3_fill_n_false = 0x5F7A23;        // std::__fill_n_false<__bitset<1,3>>(__bit_iterator, n)
constexpr uint32_t IBuffProtocol_buff_level = 0x4F23CD;    // IBuffProtocol::buff_level(BuffType, BuffTarget)
constexpr uint32_t StageState_getBuffChange = 0x4F0281;    // StageState::getBuffChange(BuffType, BuffTarget)
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_generateUniformFloat = 0x492261; // Random::generateUniformFloat(min, max, unsigned* state)
constexpr uint32_t EnemyActor_setPrizeParam = 0x74A875;    // EnemyActor::setPrizeParam(float, float, shared_ptr<const master::Skill>)
constexpr uint32_t RaidBossActor_getDefense = 0x5F4CBD;    // RaidBossActor::getDefense(int) const
constexpr uint32_t RaidBossActor_getAttack = 0x5F4DFD;     // RaidBossActor::getAttack(int) const
constexpr uint32_t RaidBossActor_getType = 0x5F3AA5;       // RaidBossActor::getType(int) const (attribute)
constexpr uint32_t RaidBossActor_setPrizeParam = 0x5F3EF9; // RaidBossActor::setPrizeParam(int, float, float, shared_ptr<const master::Skill>)
constexpr uint32_t MedalDeck_getCurrentDefense = 0x5DFFFD; // MedalDeck::getCurrentDefense()

// ------------------------------------------------------------------------------------------ globals
constexpr uint32_t BuffChange_minLevel = 0xFEF030;   // BuffChange::minLevel: lowest master::Buff level
constexpr uint32_t BuffChange_maxLevel = 0xFEF034;   // BuffChange::maxLevel: highest
constexpr uint32_t BUFF_KIND_TYPE = 0xE9D7C0;        // int[8]: BuffType of buff kind 1..8 (0,1,0,1,...)
constexpr uint32_t BUFF_KIND_TARGET = 0xE9D7E0;      // int[8]: BuffTarget of buff kind 1..8 (0,0,1,1,2,2,3,3)

// ------------------------------------------------------------------------------------------ data positions
// master::BattleMisc rows: the value at data 4. IDs the formulas read:
constexpr int32_t BM_PLAYER_POWER_SINGLE = 100;   // medal power of a single attack (/10000)
constexpr int32_t BM_PLAYER_POWER_ALL = 101;      // medal power of a whole-team attack (/10000)
constexpr int32_t BM_WEAK = 102;                  // matching coefficient, attribute advantage (/10000)
constexpr int32_t BM_NEUTRAL = 103;               // matching coefficient, same attribute (/10000)
constexpr int32_t BM_RESIST = 104;                // matching coefficient, attribute disadvantage (/10000)
constexpr int32_t BM_PLAYER_CRITICAL_RATE = 105;  // critical rate of medal attacks (/10000)
constexpr int32_t BM_CRITICAL = 106;              // critical coefficient (/10000)
constexpr int32_t BM_PLAYER_BASE = 107;           // base of the player's attack (added as is)
constexpr int32_t BM_ENEMY_BASE = 108;            // base of the enemies' attack (added as is)
constexpr int32_t BM_PLAYER_ATTACK_RATE = 109;    // weight of the medal's attack (/10000)
constexpr int32_t BM_ENEMY_ATTACK_RATE = 110;     // weight of the enemy's attack (/10000)
constexpr int32_t BM_PLAYER_DEFENSE_RATE = 111;   // weight of the enemy's defense in player attacks (/10000)
constexpr int32_t BM_ENEMY_DEFENSE_RATE = 112;    // weight of the player's defense in enemy attacks (/10000)
constexpr int32_t BM_SPREAD = 113;                // random spread of every attack (/10000)

// master::Buff
constexpr uint32_t BUFF_LEVEL = 4;
constexpr uint32_t BUFF_KIND = 0x50;              // 1..8: (BuffType, BuffTarget) through the two tables above
constexpr uint32_t BUFF_VALUE = 0x54;             // correction, 10000 = 100%
// master::Medal (MedalInfo word 0) and the medal's own state (MedalInfo + 8: SafeBuffer)
constexpr uint32_t MEDAL_ATTRIBUTE = 0x458;
constexpr uint32_t MEDAL_HAS_BURST = 0x4A8;
constexpr uint32_t MEDAL_BURST_ID = 0x4AC;
constexpr uint32_t MEDALSTATE_ATTACK = 0;
constexpr uint32_t MEDALSTATE_BURST_LEVEL = 0x10; // 1..5 picks a per-level value of the burst; 0 = the base value
constexpr uint32_t MEDALSTATE_BURST_BONUS = 0x14; // bonus (10000 = +100%)
// master::Burst
constexpr uint32_t BURST_ATTRIBUTE = 0x15C;       // 2..5 -> attribute 1..4 (else the medal's)
constexpr uint32_t BURST_DIVISOR = 0x164;
constexpr uint32_t BURST_CRITICAL_RATE = 0x16C;
constexpr uint32_t BURST_POWER = 0x1A4;           // + [5] per level at 0x1A8
constexpr uint32_t BURST_POWER_LV = 0x1A8;
constexpr uint32_t BURST_KIND = 0x1BC;            // 2 = fixed damage
constexpr uint32_t BURST_SPECIAL = 0x1C0;         // special effect kind 3..22 (see calculatePlayerAttack_sub)
constexpr uint32_t BURST_SPECIAL_A = 0x1C4;       // + [5] per level at 0x1CC
constexpr uint32_t BURST_SPECIAL_A_LV = 0x1CC;
constexpr uint32_t BURST_SPECIAL_B = 0x1C8;       // + [5] per level at 0x1E0
constexpr uint32_t BURST_SPECIAL_B_LV = 0x1E0;
// master::Skill
constexpr uint32_t SKILL_COEFFICIENT = 0xF4;
// master::Enemy
constexpr uint32_t ENEMY_ATTRIBUTE = 0x278;
constexpr uint32_t ENEMY_ATTACK_IDS = 0x348;      // int[4]
// master::EnemyAttack / master::RaidEnemyAttack
constexpr uint32_t EATTACK_ATTRIBUTE = 0x58;      // 1 = the enemy's own, 2..4 -> 1..3, else none (4)
constexpr uint32_t EATTACK_DIVISOR = 0x5C;
constexpr uint32_t EATTACK_POWER = 0x60;
constexpr uint32_t EATTACK_CRITICAL_RATE = 0x64;
constexpr uint32_t EATTACK_CRITICAL = 0x68;
constexpr uint32_t RATTACK_ATTRIBUTE = 0x4C;
constexpr uint32_t RATTACK_DIVISOR = 0x50;
constexpr uint32_t RATTACK_POWER = 0x54;
constexpr uint32_t RATTACK_CRITICAL_RATE = 0x58;
constexpr uint32_t RATTACK_CRITICAL = 0x5C;

// EnemyActor
constexpr uint32_t ENEMY_STATUS = 680;            // SafeBuffer: attack at 0xC, defense at 0x10
constexpr uint32_t ENEMY_STATUS_PROTOCOL = 712;   // IStatusProtocol (getCondition: vtable slot 0, sret)
constexpr uint32_t ENEMY_BUFF_PROTOCOL = 716;     // IBuffProtocol
constexpr uint32_t ENEMY_MASTER = 728;            // shared_ptr<const master::Enemy>
constexpr uint32_t ENEMY_ATTACK_INDEX = 792;      // index into the row's attack ids
constexpr uint32_t ENEMYSTATUS_ATTACK = 0xC;
constexpr uint32_t ENEMYSTATUS_DEFENSE = 0x10;
// RaidBossActor
constexpr uint32_t RAID_STATUS_PROTOCOL = 680;
constexpr uint32_t RAID_BUFF_PROTOCOL = 684;      // the boss; part idx: 720 + 36 * idx
constexpr uint32_t RAID_PART_BUFF_PROTOCOL = 720;
constexpr uint32_t RAID_PART_SIZE = 36;
// AttackTarget (StageActorManager::AttackTarget)
constexpr uint32_t TARGET_TYPE = 0;               // 1 single, 2 whole team, 3 burst
constexpr uint32_t TARGET_MEDAL = 4;              // const MedalDeck::MedalInfo*
constexpr uint32_t TARGET_SKILL = 8;              // shared_ptr<const master::Skill>
// StageState (AppState + 32) / user status (APIManager + 140)
constexpr uint32_t APPSTATE_STAGE_STATE = 32;
constexpr uint32_t STAGESTATE_BUFF_PROTOCOL = 36;
constexpr uint32_t STAGESTATE_HP = 8;
constexpr uint32_t STAGESTATE_BURST_HP = 0x4248;  // HP left after an HP-consuming burst
constexpr uint32_t STAGESTATE_4250 = 0x4250;
constexpr uint32_t STAGESTATE_425C = 0x425C;
constexpr uint32_t APIMANAGER_USER_STATUS = 140;
constexpr uint32_t USERSTATUS_MAX_HP = 56;

constexpr uint32_t FLOAT_ONE = 0x3F800000;

// ------------------------------------------------------------------------------------------ helpers
// libgcc __fixsfsi: truncate toward zero, saturating, NaN -> 0
int32_t f2iz(float f) {
    if (std::isnan(f)) return 0;
    if (f >= 2147483648.0f) return INT32_MAX;
    if (f <= -2147483648.0f) return INT32_MIN;
    return int32_t(f);
}
float i2f(int32_t v) { return float(v); }    // __floatsisf
float u2f(uint32_t v) { return float(v); }   // __floatunsisf

const SafeBuffer *sb(uint32_t addr) { return rw::host_ptr<const SafeBuffer>(addr); }
SafeBuffer *sbw(uint32_t addr) { return rw::host_ptr<SafeBuffer>(addr); }
guest::SharedPtr *sp(uint32_t addr) { return rw::host_ptr<guest::SharedPtr>(addr); }
void releaseCtrl(uint32_t ctrl) { if (ctrl) guest::release(ctrl); }

// A master::BattleMisc value: create the row into the stack slot, read data 4, release.
int32_t miscValue(uint32_t slot, int32_t id) {
    rw::call(master_BattleMisc_create, {slot, uint32_t(id)});
    int32_t v = sb(sp(slot)->obj)->get32(4);
    releaseCtrl(sp(slot)->ctrl);
    return v;
}

// Decode n bytes of a SafeBuffer into guest stack memory (the original's SafeBuffer::read into its frame).
void readTo(uint32_t buffer, uint32_t dst, uint32_t index, uint32_t n) {
    const SafeBuffer *b = sb(buffer);
    SafeBuffer::read(b, rw::host_ptr<uint8_t>(dst), b->data.get() + index, n, index);
}

uint32_t stageState() { return rw::load<uint32_t>(rw::call(AppState_getInstance, {}) + APPSTATE_STAGE_STATE); }
int32_t userMaxHp() {
    uint32_t api = rw::call(APIManager_getInstance, {});
    return sb(rw::load<uint32_t>(api + APIMANAGER_USER_STATUS))->get32(USERSTATUS_MAX_HP);
}

// Random::getInstance()->generateUniformFloat(min, max, nullptr)
float randomUniform(float min, float max) {
    uint32_t random = rw::call(Random_getInstance, {});
    return rw::callf(Random_generateUniformFloat, {random, rw::fbits(min), rw::fbits(max), 0});
}

// Clear the result flags: std::fill_n(bitset_iterator{&flags, 0}, 3, false) (the iterator lives in the frame).
void clearFlags(BattleMisc *self, uint32_t iterSlot) {
    rw::save<uint32_t>(iterSlot, rw::guest_addr(&self->flags));
    rw::save<uint32_t>(iterSlot + 4, 0);
    rw::call(bitset3_fill_n_false, {iterSlot, 3});
}

// The correction of the buff level an IBuffProtocol reports for (type, target); `slot` = 16 bytes of frame:
// +0 the Buff row shared_ptr, +8 the level (find's key).
float buffCorrection(BattleMisc *self, int32_t type, int32_t target, uint32_t protocol, uint32_t slot) {
    int32_t level = int32_t(rw::call(IBuffProtocol_buff_level, {protocol, uint32_t(type), uint32_t(target)}));
    rw::save<int32_t>(slot + 8, level);
    uint32_t map = rw::guest_addr(self) + 80u * uint32_t(type) + 20u * uint32_t(target);
    uint32_t node = rw::call(map_find, {map, slot + 8});
    if (!node) return 0.0f;
    rw::call(master_Buff_create, {slot, rw::load<uint32_t>(node + 12)});
    float value = i2f(sb(sp(slot)->obj)->get32(BUFF_VALUE)) / 10000.0f;
    releaseCtrl(sp(slot)->ctrl);
    return value;
}

bool isAttribute(int32_t attr) { return attr != 4 && uint32_t(attr - 1) <= 2; }

// A medal's attribute for an attack: a burst (attack type 3, the medal has one) may override it.
// slot: 8 bytes for the Burst row, value: 4 bytes of frame for the read.
int32_t attackAttribute(const uint8_t *target, uint32_t burstSlot, uint32_t valueSlot) {
    uint32_t medal = rw::load<uint32_t>(rw::get<uint32_t>(target, TARGET_MEDAL));
    readTo(medal, valueSlot, MEDAL_ATTRIBUTE, 4);
    int32_t attr = rw::load<int32_t>(valueSlot);
    readTo(medal, valueSlot, MEDAL_HAS_BURST, 4);
    if (rw::load<int32_t>(valueSlot) == 0 || rw::get<int32_t>(target, TARGET_TYPE) != 3) return attr;
    readTo(medal, valueSlot, MEDAL_BURST_ID, 4);
    rw::call(master_Burst_create, {burstSlot, rw::load<uint32_t>(valueSlot)});
    int32_t burstAttr = sb(sp(burstSlot)->obj)->get32(BURST_ATTRIBUTE);
    int32_t result = uint32_t(burstAttr - 2) < 4 ? burstAttr - 1 : attr;
    releaseCtrl(sp(burstSlot)->ctrl);
    return result;
}

// IStatusProtocol::getCondition(type) (virtual slot 0, sret StatusChange of 12 bytes): its value at +8.
int32_t condition(uint32_t protocol, uint32_t out, int32_t type) {
    rw::vcall(rw::host_ptr<uint8_t>(protocol), 0, {out, protocol, uint32_t(type)});
    return rw::load<int32_t>(out + 8);
}

}  // namespace

// ------------------------------------------------------------------------------------------ life cycle
BattleMisc *BattleMisc::getInstance() {
    if (guest::staticInitBegin(INSTANCE_GUARD)) {
        construct(rw::host_ptr<BattleMisc>(INSTANCE));
        rw::call(guest::CXA_ATEXIT, {BattleMisc_destruct, INSTANCE, guest::DSO_HANDLE});
        guest::staticInitDone(INSTANCE_GUARD);
    }
    return rw::host_ptr<BattleMisc>(INSTANCE);
}
RW_FUNCTION(0x7420A8, "BattleMisc::getInstance", BattleMisc::getInstance);

BattleMisc *BattleMisc::construct(BattleMisc *self) {
    for (auto &row : self->buffLevels) {
        for (IntMap &m : row) {
            m.buckets = 0;
            m.bucketCount = 0;
            m.first = 0;
            m.size = 0;
            m.maxLoadFactor = 1.0f;
        }
    }
    self->flags = 0;

    guest::Frame f(0x68);                     // the original frame (pushes + locals)
    uint32_t ids = f.addr(0x34);              // std::set<int>: begin node, root (= end node's left), size
    uint32_t endNode = f.addr(0x38);
    uint32_t row = f.addr(0x2C);              // shared_ptr<const master::Buff>
    uint32_t pair = f.addr(0x24);             // pair<int, int>(level, id)
    rw::call(master_Buff_getBuffIds, {ids});

    // the (type, target) of the previous row stays when a row's kind is out of range
    int32_t type = 1, target = 1;
    int32_t minLevel = 0x7FFFFFFF, maxLevel = INT32_MIN;
    for (uint32_t node = rw::load<uint32_t>(ids); node != endNode;) {
        int32_t id = rw::load<int32_t>(node + 16);
        rw::call(master_Buff_create, {row, uint32_t(id)});
        const SafeBuffer *buff = sb(sp(row)->obj);
        int32_t kind = buff->get32(BUFF_KIND);
        if (uint32_t(kind - 1) < 8) {
            target = rw::load<int32_t>(BUFF_KIND_TARGET + 4u * uint32_t(kind - 1));
            type = rw::load<int32_t>(BUFF_KIND_TYPE + 4u * uint32_t(kind - 1));
        }
        int32_t level = buff->get32(BUFF_LEVEL);
        rw::save<int32_t>(pair, level);
        rw::save<int32_t>(pair + 4, id);
        if (maxLevel < level) maxLevel = level;
        if (minLevel > level) minLevel = level;
        uint32_t map = rw::guest_addr(self) + 80u * uint32_t(type) + 20u * uint32_t(target);
        rw::call(map_insert_unique, {f.addr(0x48), map, pair});
        releaseCtrl(sp(row)->ctrl);
        // ++iterator (tree successor)
        uint32_t right = rw::load<uint32_t>(node + 4);
        if (right) {
            node = right;
            while (uint32_t left = rw::load<uint32_t>(node)) node = left;
        } else {
            uint32_t child;
            do {
                child = node;
                node = rw::load<uint32_t>(child + 8);
            } while (rw::load<uint32_t>(node) != child);
        }
    }
    rw::call(set_int_destroy, {ids, rw::load<uint32_t>(endNode)});
    rw::save<int32_t>(BuffChange_minLevel, minLevel);
    rw::save<int32_t>(BuffChange_maxLevel, maxLevel);
    return self;
}
RW_FUNCTION(0x742164, "BattleMisc::BattleMisc", BattleMisc::construct);

BattleMisc *BattleMisc::destruct(BattleMisc *self) {
    for (int k = 7; k >= 0; k--) rw::call(map_destruct, {rw::guest_addr(&self->buffLevels[k / 4][k % 4])});
    return self;
}
RW_FUNCTION(0x742140, "BattleMisc::~BattleMisc", BattleMisc::destruct);

// ------------------------------------------------------------------------------------------ buff kinds
// Quirk: for a kind outside 1..8 the original returns whatever its caller had in r4 (never set).
uint32_t BattleMisc::get_buff_kinds_type(BattleMisc *, int32_t buffId) {
    uint32_t result = rw::cpu->r[4];
    guest::Frame f(0x28);
    rw::call(master_Buff_create, {f.addr(8), uint32_t(buffId)});
    int32_t kind = sb(sp(f.addr(8))->obj)->get32(BUFF_KIND);
    if (uint32_t(kind - 1) < 8) result = rw::load<uint32_t>(BUFF_KIND_TYPE + 4u * uint32_t(kind - 1));
    releaseCtrl(sp(f.addr(8))->ctrl);
    return result;
}
RW_FUNCTION(0x742340, "BattleMisc::get_buff_kinds_type", BattleMisc::get_buff_kinds_type);

uint32_t BattleMisc::get_buff_kinds_target(BattleMisc *, int32_t buffId) {
    uint32_t result = rw::cpu->r[4];
    guest::Frame f(0x28);
    rw::call(master_Buff_create, {f.addr(8), uint32_t(buffId)});
    int32_t kind = sb(sp(f.addr(8))->obj)->get32(BUFF_KIND);
    if (uint32_t(kind - 1) < 8) result = rw::load<uint32_t>(BUFF_KIND_TARGET + 4u * uint32_t(kind - 1));
    releaseCtrl(sp(f.addr(8))->ctrl);
    return result;
}
RW_FUNCTION(0x7423D8, "BattleMisc::get_buff_kinds_target", BattleMisc::get_buff_kinds_target);

int32_t BattleMisc::get_buff_kinds(BattleMisc *, int32_t buffId) {
    guest::Frame f(0x28);
    rw::call(master_Buff_create, {f.addr(8), uint32_t(buffId)});
    int32_t kind = sb(sp(f.addr(8))->obj)->get32(BUFF_KIND);
    if (uint32_t(kind - 1) >= 8) kind = 0;
    releaseCtrl(sp(f.addr(8))->ctrl);
    return kind;
}
RW_FUNCTION(0x742470, "BattleMisc::get_buff_kinds", BattleMisc::get_buff_kinds);

// ------------------------------------------------------------------------------------------ buff corrections
float BattleMisc::getPlayerBuffCorrection(BattleMisc *self, int32_t type, int32_t target) {
    guest::Frame f(0x30);
    uint32_t protocol = stageState() + STAGESTATE_BUFF_PROTOCOL;
    return buffCorrection(self, type, target, protocol, f.addr(8));
}
RW_FUNCTION(0x7424F8, "BattleMisc::getPlayerBuffCorrection", BattleMisc::getPlayerBuffCorrection);

float BattleMisc::getEnemyBuffCorrection(BattleMisc *self, int32_t type, int32_t target, const uint8_t *enemy) {
    guest::Frame f(0x30);
    return buffCorrection(self, type, target, rw::guest_addr(enemy) + ENEMY_BUFF_PROTOCOL, f.addr(8));
}
RW_FUNCTION(0x7425C4, "BattleMisc::getEnemyBuffCorrection", BattleMisc::getEnemyBuffCorrection);

float BattleMisc::getRaidBuffCorrection(BattleMisc *self, int32_t type, int32_t target, const uint8_t *raid,
                                        int32_t idx) {
    guest::Frame f(0x30);
    uint32_t protocol = idx ? rw::guest_addr(raid) + RAID_PART_SIZE * uint32_t(idx) + RAID_PART_BUFF_PROTOCOL
                            : rw::guest_addr(raid) + RAID_BUFF_PROTOCOL;
    return buffCorrection(self, type, target, protocol, f.addr(8));
}
RW_FUNCTION(0x742688, "BattleMisc::getRaidBuffCorrection", BattleMisc::getRaidBuffCorrection);

float BattleMisc::getPlayerBuffCorrectionAttack(BattleMisc *self, int32_t attr) {
    return isAttribute(attr) ? getPlayerBuffCorrection(self, 0, attr) : 0.0f;
}
RW_FUNCTION(0x7489CC, "BattleMisc::getPlayerBuffCorrectionAttack", BattleMisc::getPlayerBuffCorrectionAttack);

float BattleMisc::getPlayerBuffCorrectionDefense(BattleMisc *self, int32_t attr) {
    return isAttribute(attr) ? getPlayerBuffCorrection(self, 1, attr) : 0.0f;
}
RW_FUNCTION(0x748BF8, "BattleMisc::getPlayerBuffCorrectionDefense", BattleMisc::getPlayerBuffCorrectionDefense);

float BattleMisc::getEnemyBuffCorrectionAttack(BattleMisc *self, int32_t attr, const uint8_t *enemy) {
    return isAttribute(attr) ? getEnemyBuffCorrection(self, 0, attr, enemy) : 0.0f;
}
RW_FUNCTION(0x748428, "BattleMisc::getEnemyBuffCorrectionAttack", BattleMisc::getEnemyBuffCorrectionAttack);

float BattleMisc::getEnemyBuffCorrectionDefense(BattleMisc *self, int32_t attr, const uint8_t *enemy) {
    return isAttribute(attr) ? getEnemyBuffCorrection(self, 1, attr, enemy) : 0.0f;
}
RW_FUNCTION(0x747420, "BattleMisc::getEnemyBuffCorrectionDefense", BattleMisc::getEnemyBuffCorrectionDefense);

float BattleMisc::getRaidBuffCorrectionAttack(BattleMisc *self, int32_t attr, const uint8_t *raid) {
    return isAttribute(attr) ? getRaidBuffCorrection(self, 0, attr, raid, 0) : 0.0f;
}
RW_FUNCTION(0x748868, "BattleMisc::getRaidBuffCorrectionAttack", BattleMisc::getRaidBuffCorrectionAttack);

float BattleMisc::getRaidBuffCorrectionDefense(BattleMisc *self, int32_t attr, const uint8_t *raid, int32_t idx) {
    return isAttribute(attr) ? getRaidBuffCorrection(self, 1, attr, raid, idx) : 0.0f;
}
RW_FUNCTION(0x748150, "BattleMisc::getRaidBuffCorrectionDefense", BattleMisc::getRaidBuffCorrectionDefense);

// ------------------------------------------------------------------------------------------ coefficients
// Attribute matching (1..3 cycle; 4 = none): (attacker + 3 - defender) % 3 == 2 weak, 1 resist, 0 neutral.
float BattleMisc::calcMatchingCoefficient(BattleMisc *self, int32_t attackerAttr, int32_t defenderAttr) {
    if (attackerAttr == 4 || defenderAttr == 4) return 1.0f;
    guest::Frame f(0x48);
    int32_t m = int32_t(uint32_t(attackerAttr) + 3u - uint32_t(defenderAttr)) % 3;   // __modsi3
    int32_t value;
    if (m == 2) {
        self->flags = self->flags | FLAG_WEAK;
        value = miscValue(f.addr(0x0C), BM_WEAK);
    } else if (m == 1) {
        self->flags = self->flags | FLAG_RESIST;
        value = miscValue(f.addr(0x14), BM_RESIST);
    } else if (m == 0) {
        value = miscValue(f.addr(0x1C), BM_NEUTRAL);
    } else {
        return 1.0f;
    }
    return i2f(value) / 10000.0f;
}
RW_FUNCTION(0x748898, "BattleMisc::calcMatchingCoefficient", BattleMisc::calcMatchingCoefficient);

// A critical hit when random[0, 1] <= rate: the BattleMisc 106 coefficient, else 1.
float BattleMisc::get_criticalCoefficient(BattleMisc *self, float rate) {
    float coefficient = 1.0f;
    float roll = randomUniform(0.0f, 1.0f);
    if (!(roll > rate || std::isnan(roll) || std::isnan(rate))) {
        guest::Frame f(0x38);
        coefficient = i2f(miscValue(f.addr(0x14), BM_CRITICAL)) / 10000.0f;
        self->flags = self->flags | FLAG_CRITICAL;
    }
    return coefficient;
}
RW_FUNCTION(0x7489F0, "BattleMisc::get_criticalCoefficient", BattleMisc::get_criticalCoefficient);

int32_t BattleMisc::normalizeAttack(BattleMisc *, float base, float multiplier, int32_t divisor) {
    float spread;
    {
        guest::Frame f(0x40);
        spread = i2f(miscValue(f.addr(0x1C), BM_SPREAD)) / 10000.0f;
    }
    float roll = randomUniform(1.0f - spread, spread + 1.0f);
    float attack = roll * ((base * multiplier) / i2f(divisor));
    int32_t value = f2iz(attack);
    return (attack > 0.0f || std::isnan(attack)) ? value : 1;
}
RW_FUNCTION(0x748AC4, "BattleMisc::normalizeAttack", BattleMisc::normalizeAttack);

// ------------------------------------------------------------------------------------------ enemy attacks
// base = BM108 + attack * (1 + attackBuff) * BM110 - defense * (1 + player defense buff) * BM112;
// multiplier = power * critical * (1 + attrAttackBuff + player attribute defense buff).
int32_t BattleMisc::calculateEnemyAttack_sub(BattleMisc *self, int32_t attr, int32_t defense, int32_t attack,
                                             float attackBuff, float power, float critical, int32_t divisor,
                                             float attrAttackBuff) {
    guest::Frame f(0x80);
    clearFlags(self, f.addr(0x58));
    int32_t enemyBase = miscValue(f.addr(0x50), BM_ENEMY_BASE);
    float attackFactor = attackBuff + 1.0f;
    int32_t attackRate = miscValue(f.addr(0x48), BM_ENEMY_ATTACK_RATE);
    float defenseBuff = getPlayerBuffCorrection(self, 1, 0);
    int32_t defenseRate = miscValue(f.addr(0x40), BM_ENEMY_DEFENSE_RATE);
    float defenseFactor = defenseBuff + 1.0f;
    float attackWeight = i2f(attackRate) / 10000.0f;
    float defenseWeight = i2f(defenseRate) / 10000.0f;

    float base = i2f(enemyBase) + (i2f(attack) * attackFactor) * attackWeight;
    base = base - (i2f(defense) * defenseFactor) * defenseWeight;
    float attrDefenseBuff = 0.0f;
    if (isAttribute(attr)) attrDefenseBuff = getPlayerBuffCorrection(self, 1, attr);
    float multiplier = (power * critical) * ((attrAttackBuff + 1.0f) + attrDefenseBuff);
    return normalizeAttack(self, base, multiplier, divisor);
}
RW_FUNCTION(0x748450, "BattleMisc::calculateEnemyAttack_sub", BattleMisc::calculateEnemyAttack_sub);

int32_t BattleMisc::calculateEnemyAttack(BattleMisc *self, const uint8_t *enemy, int32_t defense) {
    guest::Frame f(0x90);
    float attackBuff = getEnemyBuffCorrection(self, 0, 0, enemy);
    const SafeBuffer *status = reinterpret_cast<const SafeBuffer *>(enemy + ENEMY_STATUS);
    int32_t attack = status->get32(ENEMYSTATUS_ATTACK);

    // the enemy's current attack: master::Enemy attack ids[attack index] (array copied to the frame)
    uint32_t masterObj = rw::get<uint32_t>(enemy, ENEMY_MASTER);
    uint32_t masterCtrl = rw::get<uint32_t>(enemy, ENEMY_MASTER + 4);
    if (masterCtrl) guest::addRef(masterCtrl);
    readTo(masterObj, f.addr(0x68), ENEMY_ATTACK_IDS, 16);
    uint32_t attackSlot = f.addr(0x44);
    int32_t index = rw::get<int32_t>(enemy, ENEMY_ATTACK_INDEX);
    rw::call(master_EnemyAttack_create, {attackSlot, rw::load<uint32_t>(f.addr(0x68) + 4u * uint32_t(index))});
    releaseCtrl(masterCtrl);
    const SafeBuffer *row = sb(sp(attackSlot)->obj);

    float power = i2f(row->get32(EATTACK_POWER)) / 10000.0f;
    float criticalRate = i2f(row->get32(EATTACK_CRITICAL_RATE)) / 10000.0f;
    float roll = randomUniform(0.0f, 1.0f);
    float critical = 1.0f;
    if (!(criticalRate < roll || std::isnan(criticalRate) || std::isnan(roll))) {
        int32_t v = row->get32(EATTACK_CRITICAL);
        self->flags = self->flags | FLAG_CRITICAL;
        critical = i2f(v) / 10000.0f;
    }
    int32_t divisor = row->get32(EATTACK_DIVISOR);
    int32_t kind = row->get32(EATTACK_ATTRIBUTE);
    int32_t attr;
    float attrBuff = 0.0f;
    switch (kind) {
    case 1: {   // the enemy's own attribute
        uint32_t obj = rw::get<uint32_t>(enemy, ENEMY_MASTER);
        uint32_t ctrl = rw::get<uint32_t>(enemy, ENEMY_MASTER + 4);
        if (ctrl) guest::addRef(ctrl);
        attr = sb(obj)->get32(ENEMY_ATTRIBUTE);
        releaseCtrl(ctrl);
        break;
    }
    case 2: attr = 1; break;
    case 3: attr = 2; break;
    case 4: attr = 3; break;
    default: attr = 4; break;
    }
    if (kind >= 1 && kind <= 4 && isAttribute(attr)) attrBuff = getEnemyBuffCorrection(self, 0, attr, enemy);
    int32_t result = calculateEnemyAttack_sub(self, attr, defense, attack, attackBuff, power, critical, divisor,
                                              attrBuff);
    releaseCtrl(sp(attackSlot)->ctrl);
    return result;
}
RW_FUNCTION(0x748184, "BattleMisc::calculateEnemyAttack", BattleMisc::calculateEnemyAttack);

int32_t BattleMisc::calculateRaidAttack(BattleMisc *self, const uint8_t *raid, const uint8_t *medalDeck,
                                        guest::SharedPtr *attack) {
    int32_t defense = int32_t(rw::call(MedalDeck_getCurrentDefense, {rw::guest_addr(medalDeck)}));
    const SafeBuffer *row = sb(attack->obj);
    int32_t powerValue = row->get32(RATTACK_POWER);
    float criticalRate = i2f(row->get32(RATTACK_CRITICAL_RATE)) / 10000.0f;
    float roll = randomUniform(0.0f, 1.0f);
    float power = i2f(powerValue) / 10000.0f;
    float critical = 1.0f;
    if (!(criticalRate < roll || std::isnan(criticalRate) || std::isnan(roll))) {
        int32_t v = sb(attack->obj)->get32(RATTACK_CRITICAL);
        self->flags = self->flags | FLAG_CRITICAL;
        critical = i2f(v) / 10000.0f;
    }
    int32_t divisor = sb(attack->obj)->get32(RATTACK_DIVISOR);
    int32_t raidAttack = int32_t(rw::call(RaidBossActor_getAttack, {rw::guest_addr(raid), 0}));
    float attackBuff = getRaidBuffCorrection(self, 0, 0, raid, 0);
    int32_t kind = sb(attack->obj)->get32(RATTACK_ATTRIBUTE);
    int32_t attr;
    switch (kind) {
    case 1: attr = int32_t(rw::call(RaidBossActor_getType, {rw::guest_addr(raid), 0})); break;
    case 2: attr = 1; break;
    case 3: attr = 2; break;
    case 4: attr = 3; break;
    default: attr = 4; break;
    }
    float attrBuff = 0.0f;
    if (kind >= 1 && kind <= 4 && isAttribute(attr)) attrBuff = getRaidBuffCorrection(self, 0, attr, raid, 0);
    return calculateEnemyAttack_sub(self, attr, defense, raidAttack, attackBuff, power, critical, divisor, attrBuff);
}
RW_FUNCTION(0x748668, "BattleMisc::calculateRaidAttack", BattleMisc::calculateRaidAttack);

// ------------------------------------------------------------------------------------------ player attacks
int32_t BattleMisc::calculatePlayerAttack_sub(BattleMisc *self, const uint8_t *target, int32_t defense, float defenseBuff,
                                              uint32_t enemyCondition0or3, uint32_t enemyCondition1,
                                              uint32_t enemyCondition2, int32_t attr, int32_t enemyAttr,
                                              float attrDefenseBuff, uint32_t outCoefficient,
                                              uint32_t outMatching) {
    guest::Frame f(0x168);   // the original frame: the per-level arrays are indexed without a bound check
    clearFlags(self, f.addr(0x90));
    int32_t type = rw::get<int32_t>(target, TARGET_TYPE);
    uint32_t medalInfo = rw::get<uint32_t>(target, TARGET_MEDAL);

    float multiplier;        // power of the attack
    float criticalRate;
    int32_t divisor;
    if (type == 3) {
        // burst
        readTo(rw::load<uint32_t>(medalInfo), f.addr(0x90), MEDAL_BURST_ID, 4);
        uint32_t burstSlot = f.addr(0x68);
        rw::call(master_Burst_create, {burstSlot, rw::load<uint32_t>(f.addr(0x90))});
        const SafeBuffer *medalState = sb(medalInfo + 8);
        int32_t level = medalState->get32(MEDALSTATE_BURST_LEVEL);
        uint32_t bonus = uint32_t(medalState->get32(MEDALSTATE_BURST_BONUS));
        uint32_t burst = sp(burstSlot)->obj;
        // a per-level value (level 1..5; the array is decoded into the frame) or the base value (level 0)
        auto byLevel = [&](uint32_t base, uint32_t perLevel, uint32_t arraySlot) {
            if (!level) return sb(burst)->get32(base);
            readTo(burst, f.addr(arraySlot), perLevel, 20);
            return rw::load<int32_t>(f.addr(arraySlot) + 4u * uint32_t(level) - 4u);
        };
        int32_t kind = sb(burst)->get32(BURST_KIND);
        if (kind == 2) {
            // fixed damage: ceil(value * (1 + bonus)) without the formula
            float value = i2f(byLevel(BURST_SPECIAL_A, BURST_SPECIAL_A_LV, 0x100)) / 10000.0f;
            if (bonus) value = std::ceil((u2f(bonus + 10000u) * value) / 10000.0f);
            int32_t damage = f2iz(value);
            releaseCtrl(sp(burstSlot)->ctrl);
            return damage;
        }
        float a = i2f(byLevel(BURST_SPECIAL_A, BURST_SPECIAL_A_LV, 0x114)) / 10000.0f;
        float b = i2f(byLevel(BURST_SPECIAL_B, BURST_SPECIAL_B_LV, 0x128)) / 10000.0f;
        multiplier = i2f(byLevel(BURST_POWER, BURST_POWER_LV, 0x13C)) / 10000.0f;
        if (bonus) multiplier = multiplier * (u2f(bonus + 10000u) / 10000.0f);

        int32_t special = sb(burst)->get32(BURST_SPECIAL);
        switch (special) {
        case 3:    // stronger at low HP: power * b * (a + (1 - hp / max))
        case 4: {  // stronger at high HP: power * b * (a + hp / max)
            float hp = u2f(uint32_t(sb(stageState())->get32(STAGESTATE_HP)));
            float ratio = hp / i2f(userMaxHp());
            if (special == 3) ratio = 1.0f - ratio;
            multiplier = multiplier * (b * (a + ratio));
            break;
        }
        case 5: {  // consumes HP: down to a * max (or to 1 HP, with b scaled down)
            int32_t hp = sb(stageState())->get32(STAGESTATE_HP);
            int32_t max = userMaxHp();
            float ratio = i2f(hp) / i2f(max);
            int32_t left;
            if (ratio >= a || std::isnan(ratio) || std::isnan(a)) {
                left = f2iz(a * i2f(max));
            } else {
                b = b * (ratio / a);
                left = hp - 1;
            }
            float m = multiplier + b;
            sbw(stageState())->set32(STAGESTATE_BURST_HP, left);
            multiplier = m;
            break;
        }
        case 6:
        case 7: {  // grows with a counter n: power + a * n * n / (b + n * n)
            uint32_t n = uint32_t(sb(stageState())->get32(special == 6 ? STAGESTATE_4250 : STAGESTATE_425C));
            float nf = u2f(n);
            float num = nf * (a * nf);
            multiplier = multiplier + num / (b + u2f(n * n));
            break;
        }
        case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: {
            // + a per point of the player's buff change of (type, target)
            int32_t k = special - 8;
            int32_t change = int32_t(rw::call(StageState_getBuffChange, {stageState(), uint32_t(k / 4), uint32_t(k % 4)}));
            multiplier = multiplier + a * i2f(change);
            break;
        }
        case 16: case 17: case 18: case 19: {
            // + a per point of the attack and defense buff changes of one target
            uint32_t t = uint32_t(special - 16);
            int32_t attackChange = int32_t(rw::call(StageState_getBuffChange, {stageState(), 0, t}));
            int32_t defenseChange = int32_t(rw::call(StageState_getBuffChange, {stageState(), 1, t}));
            multiplier = multiplier + a * i2f(int32_t(uint32_t(defenseChange) + uint32_t(attackChange)));
            break;
        }
        case 20: case 21: case 22: {
            // + a when the enemy has the condition
            uint32_t flag = special == 20 ? enemyCondition0or3 : special == 21 ? enemyCondition1 : enemyCondition2;
            if (flag == 1) multiplier = a + multiplier;
            break;
        }
        default:
            break;
        }
        criticalRate = i2f(sb(burst)->get32(BURST_CRITICAL_RATE)) / 10000.0f;
        divisor = sb(burst)->get32(BURST_DIVISOR);
        releaseCtrl(sp(burstSlot)->ctrl);
    } else if (type == 2 || type == 1) {
        multiplier = i2f(miscValue(type == 2 ? f.addr(0x78) : f.addr(0x88),
                                   type == 2 ? BM_PLAYER_POWER_ALL : BM_PLAYER_POWER_SINGLE)) / 10000.0f;
        criticalRate = i2f(miscValue(type == 2 ? f.addr(0x70) : f.addr(0x80), BM_PLAYER_CRITICAL_RATE)) / 10000.0f;
        divisor = 1;
    } else {
        multiplier = 0.0f;
        criticalRate = 0.0f;
        divisor = type;
    }

    // base = BM107 + medal attack * (1 + attackBuff) * BM109 - defense * (1 + defenseBuff) * BM111
    int32_t playerBase = miscValue(f.addr(0x60), BM_PLAYER_BASE);
    float medalAttack = 0.0f;
    if (medalInfo) medalAttack = i2f(sb(medalInfo + 8)->get32(MEDALSTATE_ATTACK));
    float attrAttackBuff = 0.0f;
    float attackBuff = getPlayerBuffCorrection(self, 0, 0);
    int32_t attackRate = miscValue(f.addr(0x58), BM_PLAYER_ATTACK_RATE);
    float attackFactor = attackBuff + 1.0f;
    int32_t defenseRate = miscValue(f.addr(0x50), BM_PLAYER_DEFENSE_RATE);
    float defenseFactor = defenseBuff + 1.0f;
    float attackWeight = i2f(attackRate) / 10000.0f;
    float defenseWeight = i2f(defenseRate) / 10000.0f;
    float base = i2f(playerBase) + (medalAttack * attackFactor) * attackWeight;
    base = base - (i2f(defense) * defenseFactor) * defenseWeight;

    float matching = calcMatchingCoefficient(self, attr, enemyAttr);
    if (isAttribute(attr)) attrAttackBuff = getPlayerBuffCorrection(self, 0, attr);
    float skillCoefficient = 1.0f;
    uint32_t skill = rw::get<uint32_t>(target, TARGET_SKILL);
    if (skill) skillCoefficient = i2f(sb(skill)->get32(SKILL_COEFFICIENT)) / 10000.0f;
    float power = multiplier * get_criticalCoefficient(self, criticalRate);
    float coefficient = skillCoefficient * ((((matching + attrAttackBuff) + attrDefenseBuff)) * power);
    rw::save<float>(outCoefficient, coefficient);
    rw::save<float>(outMatching, matching);
    return normalizeAttack(self, base, coefficient, divisor);
}
RW_FUNCTION(0x747448, "BattleMisc::calculatePlayerAttack_sub", BattleMisc::calculatePlayerAttack_sub);

int32_t BattleMisc::calculatePlayerAttack(BattleMisc *self, const uint8_t *target, const uint8_t *enemy) {
    guest::Frame f(0x98);
    int32_t defense = reinterpret_cast<const SafeBuffer *>(enemy + ENEMY_STATUS)->get32(ENEMYSTATUS_DEFENSE);
    float defenseBuff = getEnemyBuffCorrection(self, 1, 0, enemy);
    uint32_t protocol = rw::guest_addr(enemy) + ENEMY_STATUS_PROTOCOL;
    uint32_t out = f.addr(0x6C);
    uint32_t condition0or3 = 1;
    if (condition(protocol, out, 0) == 0) condition0or3 = condition(protocol, out, 3) != 0;
    int32_t condition1 = condition(protocol, out, 1);
    int32_t condition2 = condition(protocol, out, 2);

    uint32_t masterObj = rw::get<uint32_t>(enemy, ENEMY_MASTER);
    uint32_t masterCtrl = rw::get<uint32_t>(enemy, ENEMY_MASTER + 4);
    if (masterCtrl) guest::addRef(masterCtrl);
    int32_t enemyAttr = sb(masterObj)->get32(ENEMY_ATTRIBUTE);
    releaseCtrl(masterCtrl);

    int32_t attr = attackAttribute(target, f.addr(0x64), f.addr(0x6C));
    float attrBuff = isAttribute(attr) ? getEnemyBuffCorrection(self, 1, attr, enemy) : 0.0f;
    int32_t damage = calculatePlayerAttack_sub(self, target, defense, defenseBuff, condition0or3, condition1 != 0,
                                               condition2 != 0, attr, enemyAttr, attrBuff, f.addr(0x60),
                                               f.addr(0x5C));
    // the prize parameters take the skill shared_ptr by value (copy in the frame, released after the call)
    float coefficient = rw::load<float>(f.addr(0x60));
    float matching = rw::load<float>(f.addr(0x5C));
    uint32_t skill = f.addr(0x54);
    rw::save<uint32_t>(skill, rw::get<uint32_t>(target, TARGET_SKILL));
    uint32_t ctrl = rw::get<uint32_t>(target, TARGET_SKILL + 4);
    rw::save<uint32_t>(skill + 4, ctrl);
    if (ctrl) guest::addRef(ctrl);
    rw::call(EnemyActor_setPrizeParam, {rw::guest_addr(enemy), rw::fbits(coefficient), rw::fbits(matching), skill});
    releaseCtrl(rw::load<uint32_t>(skill + 4));
    return damage;
}
RW_FUNCTION(0x74718C, "BattleMisc::calculatePlayerAttack", BattleMisc::calculatePlayerAttack);

int32_t BattleMisc::calculatePlayerAttackRaid(BattleMisc *self, const uint8_t *target, const uint8_t *raid, int32_t idx) {
    guest::Frame f(0xA0);
    uint32_t raidAddr = rw::guest_addr(raid);
    int32_t defense = int32_t(rw::call(RaidBossActor_getDefense, {raidAddr, uint32_t(idx)}));
    float defenseBuff = getRaidBuffCorrection(self, 1, 0, raid, idx);
    uint32_t protocol = raidAddr + RAID_STATUS_PROTOCOL;
    uint32_t out = f.addr(0x78);
    uint32_t condition0or3 = 1;
    if (condition(protocol, out, 0) == 0) condition0or3 = condition(protocol, out, 3) != 0;
    int32_t condition1 = condition(protocol, out, 1);
    int32_t condition2 = condition(protocol, out, 2);
    int32_t raidAttr = int32_t(rw::call(RaidBossActor_getType, {raidAddr, uint32_t(idx)}));

    int32_t attr = attackAttribute(target, f.addr(0x70), f.addr(0x78));
    float attrBuff = isAttribute(attr) ? getRaidBuffCorrection(self, 1, attr, raid, idx) : 0.0f;
    int32_t damage = calculatePlayerAttack_sub(self, target, defense, defenseBuff, condition0or3, condition1 != 0,
                                               condition2 != 0, attr, raidAttr, attrBuff, f.addr(0x6C),
                                               f.addr(0x68));
    float coefficient = rw::load<float>(f.addr(0x6C));
    float matching = rw::load<float>(f.addr(0x68));
    uint32_t skill = f.addr(0x60);
    rw::save<uint32_t>(skill, rw::get<uint32_t>(target, TARGET_SKILL));
    uint32_t ctrl = rw::get<uint32_t>(target, TARGET_SKILL + 4);
    rw::save<uint32_t>(skill + 4, ctrl);
    if (ctrl) guest::addRef(ctrl);
    rw::call(RaidBossActor_setPrizeParam,
             {raidAddr, uint32_t(idx), rw::fbits(coefficient), rw::fbits(matching), skill});
    releaseCtrl(rw::load<uint32_t>(skill + 4));
    return damage;
}
RW_FUNCTION(0x747EE4, "BattleMisc::calculatePlayerAttack (raid)", BattleMisc::calculatePlayerAttackRaid);

// ------------------------------------------------------------------------------------------ attack side effects
namespace {

constexpr uint32_t IBuffProtocol_buff_add = 0x4F256F;        // IBuffProtocol::buff_add(value, turns, param, BuffType, BuffTarget)
constexpr uint32_t IBuffProtocol_buff_clear = 0x4F27E9;      // IBuffProtocol::buff_clear(BuffType, BuffTarget)
constexpr uint32_t BuffChange_check_type_target = 0x4F20C1;  // BuffChange::check_type_target(BuffType, BuffTarget)
constexpr uint32_t EnemyProperty_getBuff = 0x74D7DD;         // EnemyActor::Property::getBuff(int) const (sret BuffChange)
constexpr uint32_t EnemyActor_isDied = 0x6008ED;             // EnemyActor::isDied() const
constexpr uint32_t IStatusProtocol_curePoison = 0x74030D;    // IStatusProtocol::curePoison()
constexpr uint32_t StatusChange_clear = 0x4F1D15;           // StatusChange::clear() -> this
constexpr uint32_t RaidBossHPIndicator_setHP = 0x5E6305;    // RaidBossHPIndicator::setHP(unsigned)
constexpr uint32_t Random_generateUniformInt = 0x492225;     // Random::generateUniformInt(min, max, unsigned* state)
constexpr uint32_t buffVector_pushBackSlow = 0x748C71;       // vector<IBuffProtocol*>::__push_back_slow_path(T&&)
constexpr uint32_t buffVector_destruct = 0x748C1B;           // __vector_base<IBuffProtocol*>::~__vector_base()
constexpr uint32_t statusVector_pushBackSlow = 0x748DAD;     // vector<IStatusProtocol*>::__push_back_slow_path(T&&)
constexpr uint32_t statusVector_destruct = 0x748C45;         // __vector_base<IStatusProtocol*>::~__vector_base()
constexpr uint32_t preparePlayerAttackEnemy_tail = 0x74344D; // sub_74344C(const AttackTarget&): the rest of the burst

constexpr uint32_t TARGET_ENEMY = 16;              // AttackTarget: EnemyActor* (single attacks / bursts)
constexpr uint32_t MANAGER_TEAM = 76;              // StageActorManager: std::vector<EnemyActor*> the encountered team
constexpr uint32_t ENEMY_HP_INDICATOR = 960;       // EnemyActor: RaidBossHPIndicator* (HP bar)
constexpr uint32_t ENEMYSTATUS_MAX_HP = 4;         // EnemyActor status (Property) data positions
constexpr uint32_t ENEMYSTATUS_HP = 8;
constexpr uint32_t STAGESTATE_STATUS_PROTOCOL = 32;
constexpr uint32_t STAGESTATE_BURST_CHAIN = 0x425C;  // bursts in a row (reset by a medal attack)
constexpr uint32_t AVATAR_FLAG_788 = 788;          // AvatarActor: byte set when an enemy attack lands condition 1

// master::Burst: turns / param of the buffs it gives; per (BuffType t, BuffTarget g) at + 0x18 * g + 0xC * t:
// add to the targeted enemy (0x200), to the whole team (0x204); clear on the targeted enemy (0x260), on the team
// (0x264); copy the enemy's buffs of target 0 (0x2BC) / of the attribute targets (0x2C0) to the player.
constexpr uint32_t BURST_BUFF_TURNS = 0x1F4;
constexpr uint32_t BURST_BUFF_PARAM = 0x1F8;
constexpr uint32_t BURST_ADD_ENEMY = 0x200;
constexpr uint32_t BURST_ADD_TEAM = 0x204;
constexpr uint32_t BURST_CLEAR_ENEMY = 0x260;
constexpr uint32_t BURST_CLEAR_TEAM = 0x264;
constexpr uint32_t BURST_STEAL_ALL = 0x2BC;
constexpr uint32_t BURST_STEAL_ATTRIBUTE = 0x2C0;
uint32_t burstBuffField(uint32_t base, int32_t type, int32_t target) {
    return base + 0x18u * uint32_t(target) + 0xCu * uint32_t(type);
}

// master::EnemyAttack: who the side effects hit (2 the player, 3 the attacker itself, 4 the living team, 5 the
// living team member with the lowest HP ratio), chances (1..10000) of conditions 0, 3, 1, 2, heal (ratio of the
// max HP + random[0, spread]), cures, and buffs per (BuffType t, BuffTarget g): add at 0x80 + 8 * g + 4 * t,
// clear at 0xA0 + 8 * g + 4 * t, with turns / param at 0x78 / 0x7C.
constexpr uint32_t EATTACK_TARGET = 0x74;
constexpr uint32_t EATTACK_BUFF_TURNS = 0x78;
constexpr uint32_t EATTACK_BUFF_PARAM = 0x7C;
constexpr uint32_t EATTACK_BUFF_ADD = 0x80;
constexpr uint32_t EATTACK_BUFF_CLEAR = 0xA0;
constexpr uint32_t EATTACK_HEAL = 0xC0;
constexpr uint32_t EATTACK_HEAL_SPREAD = 0xC4;
constexpr uint32_t EATTACK_CONDITION0 = 0xC8;
constexpr uint32_t EATTACK_CONDITION3 = 0xCC;
constexpr uint32_t EATTACK_CONDITION1 = 0xD0;
constexpr uint32_t EATTACK_CONDITION2 = 0xD4;
constexpr uint32_t EATTACK_CURE_POISON = 0xD8;
constexpr uint32_t EATTACK_CURE1 = 0xDC;
constexpr uint32_t EATTACK_CURE2 = 0xE0;

// libgcc __fixunssfsi: truncate, negative -> 0, saturating; NaN -> 0
uint32_t f2uiz(float f) {
    if (std::isnan(f) || f <= 0.0f) return 0;
    if (f >= 4294967296.0f) return UINT32_MAX;
    return uint32_t(f);
}

void buffAdd(uint32_t protocol, int32_t value, int32_t turns, int32_t param, int32_t type, int32_t target) {
    rw::call(IBuffProtocol_buff_add,
             {protocol, uint32_t(value), uint32_t(turns), uint32_t(param), uint32_t(type), uint32_t(target)});
}
void buffClear(uint32_t protocol, int32_t type, int32_t target) {
    rw::call(IBuffProtocol_buff_clear, {protocol, uint32_t(type), uint32_t(target)});
}

// The inlined vector<T*>::push_back(value): the value is first stored in its frame slot (the slow path takes it
// by reference).
void pushBack(uint32_t vec, uint32_t slot, uint32_t value, uint32_t slowPath) {
    rw::save<uint32_t>(slot, value);
    uint32_t end = rw::load<uint32_t>(vec + 4), cap = rw::load<uint32_t>(vec + 8);
    if (end < cap) {
        if (end) rw::save<uint32_t>(end, value);
        rw::save<uint32_t>(vec + 4, end + 4);
    } else {
        rw::call(slowPath, {vec, slot});
    }
}
void clearVector(uint32_t vec) {
    rw::save<uint32_t>(vec, 0);
    rw::save<uint32_t>(vec + 4, 0);
    rw::save<uint32_t>(vec + 8, 0);
}
// fn(element) for every element of a vector<T*> in guest memory
template <class F> void forEach(uint32_t vec, F &&fn) {
    uint32_t end = rw::load<uint32_t>(vec + 4);
    for (uint32_t it = rw::load<uint32_t>(vec); it != end; it += 4) fn(rw::load<uint32_t>(it));
}

// Copy the enemy's buffs of (type, target) to the player: clear the player's, then add every one of the enemy's
// 64 buff slots that matches (StageState virtual slot 6: add a BuffChange). `slot` = the BuffChange in the frame.
void stealBuffs(uint32_t enemyProperty, int32_t type, int32_t target, uint32_t slot) {
    buffClear(stageState() + STAGESTATE_BUFF_PROTOCOL, type, target);
    for (int32_t k = 0; k < 64; k++) {
        rw::call(EnemyProperty_getBuff, {slot, enemyProperty, uint32_t(k)});
        if (rw::call(BuffChange_check_type_target, {slot, uint32_t(type), uint32_t(target)}) == 1) {
            uint32_t state = stageState();
            rw::vcall(rw::host_ptr<uint8_t>(state), 6, {state, slot});
        }
    }
}

// Condition arguments as the original passes them: (turns, param) when turns != 0, else (param, 0).
void normalizeCondition(int32_t &a, int32_t &b) {
    if (a == 0) {
        a = b;
        b = 0;
    }
}

int32_t randomPercent() {   // Random::getInstance()->generateUniformInt(1, 10000, nullptr)
    uint32_t random = rw::call(Random_getInstance, {});
    return int32_t(rw::call(Random_generateUniformInt, {random, 1, 10000, 0}));
}

// IStatusProtocol: setCondition(type, getCondition(type).clear()) (virtual slots 0 and 1)
void clearCondition(uint32_t protocol, int32_t type, uint32_t slot) {
    uint32_t vtable = rw::load<uint32_t>(protocol);
    uint32_t getCondition = rw::load<uint32_t>(vtable), setCondition = rw::load<uint32_t>(vtable + 4);
    rw::call(getCondition, {slot, protocol, uint32_t(type)});
    uint32_t change = rw::call(StatusChange_clear, {slot});
    rw::call(setCondition, {protocol, uint32_t(type), change});
}

// RaidBossHPIndicator::setHP(HP) of an enemy's HP bar (through float like the original)
void showHp(uint32_t enemy) {
    uint32_t indicator = rw::load<uint32_t>(enemy + ENEMY_HP_INDICATOR);
    int32_t hp = sb(enemy + ENEMY_STATUS)->get32(ENEMYSTATUS_HP);
    rw::call(RaidBossHPIndicator_setHP, {indicator, f2uiz(i2f(hp))});
}

}  // namespace

int32_t BattleMisc::preparePlayerAttackEnemy(BattleMisc *, const uint8_t *target, const uint8_t *actorManager) {
    guest::Frame f(0x168);
    int32_t type = rw::get<int32_t>(target, TARGET_TYPE);
    if (type != 3 && uint32_t(type) <= 2) {
        // a medal attack ends the burst chain
        sbw(stageState())->set32(STAGESTATE_BURST_CHAIN, 0);
        return 0;
    }
    // type 3 (and anything above 2) is a burst; only 3 counts in the chain
    if (type == 3) {
        SafeBuffer *state = sbw(stageState());
        uint32_t chain = uint32_t(sb(stageState())->get32(STAGESTATE_BURST_CHAIN));
        state->set32(STAGESTATE_BURST_CHAIN, int32_t(chain + 1));
    }
    uint32_t medal = rw::load<uint32_t>(rw::get<uint32_t>(target, TARGET_MEDAL));
    uint32_t burstSlot = f.addr(0xB4);
    rw::call(master_Burst_create, {burstSlot, uint32_t(sb(medal)->get32(MEDAL_BURST_ID))});
    const SafeBuffer *burst = sb(sp(burstSlot)->obj);
    int32_t turns = burst->get32(BURST_BUFF_TURNS);
    int32_t param = burst->get32(BURST_BUFF_PARAM);
    uint32_t enemy = rw::get<uint32_t>(target, TARGET_ENEMY);
    uint32_t enemyBuffs = enemy + ENEMY_BUFF_PROTOCOL;
    uint32_t enemyProperty = enemy + ENEMY_STATUS;

    // the targeted enemy
    for (int32_t t = 0; t < 2; t++)
        for (int32_t g = 0; g < 4; g++)
            if (int32_t v = burst->get32(burstBuffField(BURST_ADD_ENEMY, t, g))) buffAdd(enemyBuffs, v, turns, param, t, g);
    for (int32_t t = 0; t < 2; t++)
        for (int32_t g = 0; g < 4; g++)
            if (burst->get32(burstBuffField(BURST_CLEAR_ENEMY, t, g))) buffClear(enemyBuffs, t, g);

    // the whole team: vector<IBuffProtocol*> of every member (dead ones too)
    uint32_t team = f.addr(0xA8);
    clearVector(team);
    uint32_t end = rw::get<uint32_t>(actorManager, MANAGER_TEAM + 4);
    for (uint32_t it = rw::get<uint32_t>(actorManager, MANAGER_TEAM); it != end; it += 4)
        pushBack(team, f.addr(0xA4), rw::load<uint32_t>(it) + ENEMY_BUFF_PROTOCOL, buffVector_pushBackSlow);
    for (int32_t t = 0; t < 2; t++)
        for (int32_t g = 0; g < 4; g++)
            if (int32_t v = burst->get32(burstBuffField(BURST_ADD_TEAM, t, g)))
                forEach(team, [&](uint32_t p) { buffAdd(p, v, turns, param, t, g); });
    for (int32_t t = 0; t < 2; t++)
        for (int32_t g = 0; g < 4; g++)
            if (burst->get32(burstBuffField(BURST_CLEAR_TEAM, t, g))) forEach(team, [&](uint32_t p) { buffClear(p, t, g); });

    // take over the targeted enemy's buffs
    if (burst->get32(BURST_STEAL_ALL)) {
        stealBuffs(enemyProperty, 0, 0, f.addr(0x98));
        stealBuffs(enemyProperty, 1, 0, f.addr(0x8C));
    }
    if (burst->get32(BURST_STEAL_ATTRIBUTE)) {
        stealBuffs(enemyProperty, 0, 1, f.addr(0x80));
        stealBuffs(enemyProperty, 0, 2, f.addr(0x74));
        stealBuffs(enemyProperty, 0, 3, f.addr(0x68));
        stealBuffs(enemyProperty, 1, 1, f.addr(0x5C));
        stealBuffs(enemyProperty, 1, 2, f.addr(0x50));
        stealBuffs(enemyProperty, 1, 3, f.addr(0x44));
    }
    rw::call(buffVector_destruct, {team});
    releaseCtrl(sp(burstSlot)->ctrl);
    return int32_t(rw::call(preparePlayerAttackEnemy_tail, {rw::guest_addr(target)}));
}
RW_FUNCTION(0x742764, "BattleMisc::preparePlayerAttackEnemy", BattleMisc::preparePlayerAttackEnemy);

void BattleMisc::prepareEnemyAttack(BattleMisc *, const uint8_t *enemy, const uint8_t *avatar, const uint8_t *actorManager) {
    guest::Frame f(0x148);
    uint32_t enemyAddr = rw::guest_addr(enemy);
    // the enemy's current attack (master::Enemy attack ids[index], copied to the frame)
    uint32_t masterCtrl = rw::get<uint32_t>(enemy, ENEMY_MASTER + 4);
    uint32_t masterObj = rw::get<uint32_t>(enemy, ENEMY_MASTER);
    int32_t index = rw::get<int32_t>(enemy, ENEMY_ATTACK_INDEX);
    if (masterCtrl) guest::addRef(masterCtrl);
    readTo(masterObj, f.addr(0x120), ENEMY_ATTACK_IDS, 16);
    uint32_t attackSlot = f.addr(0x7C);
    rw::call(master_EnemyAttack_create, {attackSlot, rw::load<uint32_t>(f.addr(0x120) + 4u * uint32_t(index))});
    releaseCtrl(masterCtrl);

    uint32_t chosen = 0;              // kind 5: the member with the lowest HP ratio
    uint32_t buffs = f.addr(0x64);    // vector<IBuffProtocol*>
    uint32_t statuses = f.addr(0x70); // vector<IStatusProtocol*>
    clearVector(buffs);
    clearVector(statuses);
    const SafeBuffer *row = sb(sp(attackSlot)->obj);
    int32_t kind = row->get32(EATTACK_TARGET);
    if (uint32_t(kind - 2) <= 3) {
        switch (kind) {
        case 2: {   // the player
            uint32_t state = stageState();
            pushBack(statuses, f.addr(0x60), state ? state + STAGESTATE_STATUS_PROTOCOL : 0, statusVector_pushBackSlow);
            state = stageState();
            pushBack(buffs, f.addr(0x5C), state ? state + STAGESTATE_BUFF_PROTOCOL : 0, buffVector_pushBackSlow);
            break;
        }
        case 3:     // the attacker itself
            pushBack(statuses, f.addr(0x58), enemyAddr + ENEMY_STATUS_PROTOCOL, statusVector_pushBackSlow);
            pushBack(buffs, f.addr(0x54), enemyAddr + ENEMY_BUFF_PROTOCOL, buffVector_pushBackSlow);
            break;
        case 4: {   // every living member of the team
            uint32_t end = rw::get<uint32_t>(actorManager, MANAGER_TEAM + 4);
            for (uint32_t it = rw::get<uint32_t>(actorManager, MANAGER_TEAM); it != end; it += 4) {
                if (rw::call(EnemyActor_isDied, {rw::load<uint32_t>(it)})) continue;
                pushBack(statuses, f.addr(0x50), rw::load<uint32_t>(it) + ENEMY_STATUS_PROTOCOL,
                         statusVector_pushBackSlow);
                pushBack(buffs, f.addr(0x4C), rw::load<uint32_t>(it) + ENEMY_BUFF_PROTOCOL, buffVector_pushBackSlow);
            }
            break;
        }
        default: {  // 5: the living member with the lowest HP ratio
            uint32_t end = rw::get<uint32_t>(actorManager, MANAGER_TEAM + 4);
            uint32_t it = rw::get<uint32_t>(actorManager, MANAGER_TEAM);
            if (it == end) break;
            float best = 3.40282347e+38f;   // FLT_MAX
            uint32_t lowest = 0;
            for (; it != end; it += 4) {
                uint32_t e = rw::load<uint32_t>(it);
                if (rw::call(EnemyActor_isDied, {e})) continue;
                const SafeBuffer *status = sb(e + ENEMY_STATUS);
                float ratio = i2f(status->get32(ENEMYSTATUS_HP)) / i2f(status->get32(ENEMYSTATUS_MAX_HP));
                if (!(best <= ratio || std::isnan(best) || std::isnan(ratio))) {
                    lowest = e;
                    best = ratio;
                }
            }
            if (lowest) {
                pushBack(statuses, f.addr(0x48), lowest + ENEMY_STATUS_PROTOCOL, statusVector_pushBackSlow);
                pushBack(buffs, f.addr(0x44), lowest + ENEMY_BUFF_PROTOCOL, buffVector_pushBackSlow);
                chosen = lowest;
            }
            break;
        }
        }

        int32_t turns = row->get32(EATTACK_BUFF_TURNS);
        int32_t param = row->get32(EATTACK_BUFF_PARAM);
        if (rw::load<uint32_t>(statuses) != rw::load<uint32_t>(statuses + 4)) {
            // conditions, each by chance: IStatusProtocol virtual slot 2 = add condition (type, turns, param)
            int32_t condTurns = turns, condParam = param;
            normalizeCondition(condTurns, condParam);
            forEach(statuses, [&](uint32_t p) {
                const uint8_t *ph = rw::host_ptr<uint8_t>(p);
                int32_t a = turns, b = param;
                int32_t chance = row->get32(EATTACK_CONDITION0);
                if (chance >= randomPercent()) {
                    rw::vcall(ph, 2, {p, 0, uint32_t(condTurns), uint32_t(condParam)});
                    a = condTurns;
                    b = condParam;
                }
                chance = row->get32(EATTACK_CONDITION3);
                if (chance >= randomPercent()) {
                    normalizeCondition(a, b);
                    rw::vcall(ph, 2, {p, 3, uint32_t(a), uint32_t(b)});
                }
                chance = row->get32(EATTACK_CONDITION1);
                if (chance >= randomPercent()) {
                    rw::vcall(ph, 2, {p, 1, uint32_t(a), uint32_t(b)});
                    rw::set<uint8_t>(avatar, AVATAR_FLAG_788, 1);
                }
                chance = row->get32(EATTACK_CONDITION2);
                if (chance >= randomPercent()) rw::vcall(ph, 2, {p, 2, uint32_t(a), uint32_t(b)});
            });

            // heal: (ratio + random[0, spread]) * max HP (virtual slots 3 = max HP, 4 = heal)
            float ratio = i2f(row->get32(EATTACK_HEAL)) / 10000.0f;
            float spread = i2f(row->get32(EATTACK_HEAL_SPREAD)) / 10000.0f;
            if (ratio > 0.0f || !(spread <= 0.0f || std::isnan(spread))) {
                forEach(statuses, [&](uint32_t p) {
                    float roll = randomUniform(0.0f, spread);
                    uint32_t vtable = rw::load<uint32_t>(p);
                    uint32_t maxHpFn = rw::load<uint32_t>(vtable + 12), healFn = rw::load<uint32_t>(vtable + 16);
                    int32_t maxHp = int32_t(rw::call(maxHpFn, {p}));
                    rw::call(healFn, {p, uint32_t(f2iz((ratio + roll) * i2f(maxHp)))});
                });
                // refresh the HP bars of the healed enemies
                if (kind == 5) {
                    if (chosen) showHp(chosen);
                } else if (kind == 4) {
                    uint32_t end = rw::get<uint32_t>(actorManager, MANAGER_TEAM + 4);
                    for (uint32_t it = rw::get<uint32_t>(actorManager, MANAGER_TEAM); it != end; it += 4)
                        showHp(rw::load<uint32_t>(it));
                } else if (kind == 3) {
                    showHp(enemyAddr);
                }
            }
            // cures
            if (row->get32(EATTACK_CURE_POISON))
                forEach(statuses, [&](uint32_t p) { rw::call(IStatusProtocol_curePoison, {p}); });
            if (row->get32(EATTACK_CURE1)) forEach(statuses, [&](uint32_t p) { clearCondition(p, 1, f.addr(0x114)); });
            if (row->get32(EATTACK_CURE2)) forEach(statuses, [&](uint32_t p) { clearCondition(p, 2, f.addr(0x100)); });
        }
        // buffs
        if (rw::load<uint32_t>(buffs) != rw::load<uint32_t>(buffs + 4)) {
            for (int32_t t = 0; t < 2; t++)
                for (int32_t g = 0; g < 4; g++)
                    if (int32_t v = row->get32(EATTACK_BUFF_ADD + 8u * uint32_t(g) + 4u * uint32_t(t)))
                        forEach(buffs, [&](uint32_t p) { buffAdd(p, v, turns, param, t, g); });
            for (int32_t t = 0; t < 2; t++)
                for (int32_t g = 0; g < 4; g++)
                    if (row->get32(EATTACK_BUFF_CLEAR + 8u * uint32_t(g) + 4u * uint32_t(t)))
                        forEach(buffs, [&](uint32_t p) { buffClear(p, t, g); });
        }
    }
    rw::call(buffVector_destruct, {buffs});
    rw::call(statusVector_destruct, {statuses});
    releaseCtrl(sp(attackSlot)->ctrl);
}
RW_FUNCTION(0x7454A0, "BattleMisc::prepareEnemyAttack", BattleMisc::prepareEnemyAttack);
