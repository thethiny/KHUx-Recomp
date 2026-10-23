#include "StageState.hpp"

#include <algorithm>

namespace {
constexpr uint32_t make_shared_StageState = 0x4F0B6D;  // std::shared_ptr<StageState>::make_shared<>()
constexpr uint32_t SafeBuffer_zerofill = 0x45A605;      // SafeBuffer::zerofill()
constexpr uint32_t tree_destroy = 0x4F0B41;             // std::__tree<unsigned>::destroy(node)
constexpr uint32_t tree_insert_unique = 0x4F0A3D;       // std::__tree<unsigned>::__insert_unique(const unsigned&)
constexpr uint32_t tree_find = 0x4F09F9;                // std::__tree<unsigned>::find(const unsigned&)
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t master_Enemy_create = 0x77F7DD;      // master::Enemy::create(int) (sret shared_ptr)
constexpr uint32_t master_Keyblade_create = 0x783F9D;   // master::Keyblade::create(int)
constexpr uint32_t master_Badstatus_create = 0x779525;  // master::Badstatus::create(int)
constexpr uint32_t IBuffProtocol_getBlankBuff = 0x4F2781;
constexpr uint32_t IBuffProtocol_buff_clear_all = 0x4F2881;
constexpr uint32_t IBuffProtocol_buff_add = 0x4F256F;   // (value, turns, param, BuffType, BuffTarget)
constexpr uint32_t IStatusProtocol_addCondition_common = 0x4F1D21;
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_generateUniformInt = 0x492225;  // (min, max, uint* state)
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;
constexpr uint32_t FlashNode_reflection_status = 0x5C3171;
constexpr uint32_t Vec2_copy = 0x8E4C03;                // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t Vec2_dtor = 0x8E4C17;                // cocos2d::Vec2::~Vec2()
constexpr uint32_t DisplayDamageNumber_disp_dmgNum = 0x5EB45D;
constexpr uint32_t StageState_vtable = 0xF7FDA0;        // _ZTV10StageState

constexpr uint32_t APIMANAGER_USER_STATUS = 140;  // APIManager + 140: user status SafeBuffer (max HP at 56)
constexpr uint32_t APPSTATE_STAGE_STATE = 32;     // AppState + 32: StageState*

// The player's max HP from the user status (APIManager)
int32_t userMaxHp() {
    uint32_t api = rw::call(APIManager_getInstance, {});
    return bt::sbGet32(rw::load<uint32_t>(api + APIMANAGER_USER_STATUS), 56);
}

// value = min(old + amount, cap) with an unsigned compare (the original's bhi)
uint32_t addCapped(StageState *self, uint32_t index, uint32_t amount, uint32_t cap) {
    uint32_t v = uint32_t(self->get32(index)) + amount;
    if (v > cap) v = cap;
    self->set32(index, int32_t(v));
    return v;
}
}  // namespace

void StageState::readBlock(uint32_t index, void *out, uint32_t n) const {
    uint8_t *o = static_cast<uint8_t *>(out);
    for (uint32_t k = 0; k < n; k++) o[k] = uint8_t(data[index + k] ^ mask(key, index + k));
}

void StageState::writeBlock(uint32_t index, const void *in, uint32_t n) {
    SafeBuffer::write(this, data.get() + index, static_cast<const uint8_t *>(in), n, index);
}

StageState *StageState::current() {
    return rw::host_ptr<StageState>(rw::load<uint32_t>(rw::call(AppState_getInstance, {}) + APPSTATE_STAGE_STATE));
}

// ------------------------------------------------------------------------------------------- life cycle
void StageState::create(bt::SharedPtr *out, uint32_t playable) {
    rw::call(make_shared_StageState, {rw::guest_addr(out)});
    StageState *s = rw::host_ptr<StageState>(out->obj);
    rw::call(SafeBuffer_zerofill, {rw::guest_addr(s)});
    uint8_t flag = uint8_t(playable);    // strb: the low byte of the argument
    s->writeBlock(PLAYABLE, &flag, 1);
    s->set32(HP, s->maxHp());
    s->set32(FIELD_4238, 10000);
    s->set32(FIELD_4250, 1);
    s->set32(CP_MAX, 150000);
    // the lists start empty (-1); each is re-read through `out` like the original
    uint8_t empty[8000];
    std::memset(empty, 0xFF, sizeof empty);
    s = rw::host_ptr<StageState>(out->obj);
    s->writeBlock(MATERIALS, empty, 8000);
    s = rw::host_ptr<StageState>(out->obj);
    s->writeBlock(DROP_ITEMS, empty, 8000);
    s = rw::host_ptr<StageState>(out->obj);
    s->writeBlock(TREASURES, empty, 800);
    s->writeBlock(NPC_DONE, empty, 80);
    s->writeBlock(CLEAR_MISSIONS, empty, 12);
    s->field_28 = 0;
    s->field_2C = 0;
    s->field_30 = 0;
    s->field_34 = 0;
    // exterminated teams: clear()
    rw::call(tree_destroy, {rw::guest_addr(&s->teamsBegin), s->teamsRoot});
    s->teamsBegin = rw::guest_addr(&s->teamsRoot);
    s->teamsRoot = 0;
    s->teamsSize = 0;
}
RW_FUNCTION(0x4EE968, "StageState::create", StageState::create);

StageState *StageState::destroy(StageState *self) {
    self->vtable = StageState_vtable + 8;
    self->statusVtable = StageState_vtable + 0x38;
    self->buffVtable = StageState_vtable + 0x54;
    rw::call(tree_destroy, {rw::guest_addr(&self->teamsBegin), self->teamsRoot});
    rw::call(bt::fn::safebuffer_dtor, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0x4F089C, "StageState::~StageState", StageState::destroy);

void StageState::deletingDestroy(StageState *self) {
    destroy(self);
    rw::call(bt::fn::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x4F08E4, "StageState::~StageState (deleting)", StageState::deletingDestroy);

int32_t StageState::getMaxHP(const StageState *) { return userMaxHp(); }
RW_FUNCTION(0x4F0930, "StageState::getMaxHP", StageState::getMaxHP);
// the same through the IStatus base at +32 (the thunk's own copy of the body: no this adjustment needed)
static int32_t getMaxHPThunk(uint32_t) { return userMaxHp(); }
RW_FUNCTION(0x4F0994, "non-virtual thunk to StageState::getMaxHP (+32)", getMaxHPThunk);

// ------------------------------------------------------------------------------------------- HP / gauge / points
int32_t StageState::addDamage(StageState *self, int32_t damage) {
    int32_t hp = int32_t(uint32_t(self->get32(HP)) - uint32_t(damage));     // ARM wraps
    int32_t max = self->maxHp();
    hp = std::min(max, std::max(hp, 0));
    self->set32(HP, hp);
    return hp;
}
RW_FUNCTION(0x4EEB88, "StageState::addDamage", StageState::addDamage);

int32_t StageState::heal(StageState *self, int32_t amount) {
    return addDamage(self, int32_t(0u - uint32_t(amount)));
}
RW_FUNCTION(0x4F0988, "StageState::heal", StageState::heal);

int32_t StageState::addCP(StageState *self, int32_t amount) {
    int32_t cp = std::max(int32_t(uint32_t(self->get32(CP)) + uint32_t(amount)), 0);
    cp = std::min(self->get32(CP_MAX), cp);
    self->set32(CP, cp);
    return cp;
}
RW_FUNCTION(0x4EEC28, "StageState::addCP", StageState::addCP);

void StageState::setCpMax(StageState *self, int32_t keybladeId) {
    bt::GuestFrame frame(8);
    auto *keyblade = frame.ptr<bt::SharedPtr>();
    rw::call(master_Keyblade_create, {frame.addr(), uint32_t(keybladeId)});
    int32_t burst = bt::sbGet32(keyblade->obj, 224);
    self->set32(CP_MAX, int32_t(uint32_t(burst) * 10000u));
    bt::releaseShared(keyblade->ctrl);
}
RW_FUNCTION(0x4EF858, "StageState::setCpMax", StageState::setCpMax);

int32_t StageState::getCpMax(StageState *self) { return self->get32(CP_MAX); }
RW_FUNCTION(0x4EF8F4, "StageState::getCpMax", StageState::getCpMax);

uint32_t StageState::addLUX(StageState *self, uint32_t amount) { return addCapped(self, LUX, amount, 9999999); }
RW_FUNCTION(0x4EECD8, "StageState::addLUX", StageState::addLUX);

uint32_t StageState::addRaidpoint(StageState *self, uint32_t amount) {
    return addCapped(self, RAID_POINT, amount, 9999999);
}
RW_FUNCTION(0x4EED64, "StageState::addRaidpoint", StageState::addRaidpoint);

uint32_t StageState::addEXP(StageState *self, uint32_t amount) { return addCapped(self, EXP, amount, 999999999); }
RW_FUNCTION(0x4EEDF0, "StageState::addEXP", StageState::addEXP);

uint32_t StageState::addMoney(StageState *self, uint32_t amount) {
    return addCapped(self, MONEY, amount, 999999999);
}
RW_FUNCTION(0x4EEE7C, "StageState::addMoney", StageState::addMoney);

// Count a defeated enemy by its master kind (1, 2, 3), then the sub-mission 04 counter for its id.
int32_t StageState::addDefeatedEnemy(StageState *self, int32_t enemyId) {
    bt::GuestFrame frame(8);
    auto *enemy = frame.ptr<bt::SharedPtr>();
    rw::call(master_Enemy_create, {frame.addr(), uint32_t(enemyId)});
    int32_t kind = bt::sbGet32(enemy->obj, 628);
    int32_t count = 0;
    uint32_t index = kind == 1 ? DEFEATED_KIND1 : kind == 2 ? DEFEATED_KIND2 : kind == 3 ? DEFEATED_KIND3 : 0;
    if (index) {
        count = int32_t(uint32_t(self->get32(index)) + 1);
        self->set32(index, count);
    }
    cmpSubMission04(self, enemyId);
    bt::releaseShared(enemy->ctrl);
    return count;
}
RW_FUNCTION(0x4EEF08, "StageState::addDefeatedEnemy", StageState::addDefeatedEnemy);

uint32_t StageState::checkMaxAttack(StageState *self, uint32_t attack) {
    uint32_t best = uint32_t(self->get32(MAX_ATTACK));
    if (best >= attack) return best;
    self->set32(MAX_ATTACK, int32_t(attack));
    return attack;
}
RW_FUNCTION(0x4EF0C8, "StageState::checkMaxAttack", StageState::checkMaxAttack);

// ------------------------------------------------------------------------------------------- collected items
// Materials: append (id, count) in the first free slot; no duplicate check. Returns 1 when stored.
uint32_t StageState::addMaterial(StageState *self, uint32_t id, uint32_t count) {
    uint32_t list[2000];
    self->readBlock(MATERIALS, list, 8000);
    for (uint32_t k = 0; k < 1000; k++) {
        if (list[2 * k] != 0xFFFFFFFFu) continue;
        list[2 * k] = id;
        list[2 * k + 1] = count;
        self->writeBlock(MATERIALS, list, 8000);
        cmpSubMission19(self, int32_t(id));
        return 1;
    }
    return 0;
}
RW_FUNCTION(0x4EF140, "StageState::addMaterial", StageState::addMaterial);

// Append (id, value) to a list of `slots` pairs unless the id is already in it. Returns 1 when stored.
static uint32_t addUniquePair(StageState *self, uint32_t index, uint32_t slots, uint32_t id, uint32_t value) {
    uint32_t list[2000];
    self->readBlock(index, list, slots * 8);
    for (uint32_t k = 0; k < slots; k++) {
        if (list[2 * k] == id) return 0;
        if (list[2 * k] != 0xFFFFFFFFu) continue;
        list[2 * k] = id;
        list[2 * k + 1] = value;
        self->writeBlock(index, list, slots * 8);
        return 1;
    }
    return 0;
}

uint32_t StageState::addEnemyDropItem(StageState *self, uint32_t id, uint32_t count) {
    return addUniquePair(self, DROP_ITEMS, 1000, id, count);
}
RW_FUNCTION(0x4EF288, "StageState::addEnemyDropItem", StageState::addEnemyDropItem);

uint32_t StageState::addTreasure(StageState *self, uint32_t id, uint32_t value) {
    return addUniquePair(self, TREASURES, 100, id, value);
}
RW_FUNCTION(0x4EF340, "StageState::addTreasure", StageState::addTreasure);

uint32_t StageState::addActionAlreadyNPC(StageState *self, uint32_t npcId) {
    uint32_t ids[20];
    self->readBlock(NPC_DONE, ids, sizeof ids);
    for (uint32_t k = 0; k < 20; k++) {
        if (ids[k] == npcId) return 0;
        if (ids[k] != 0xFFFFFFFFu) continue;
        ids[k] = npcId;
        self->writeBlock(NPC_DONE, ids, sizeof ids);
        return 1;
    }
    return 0;
}
RW_FUNCTION(0x4EF3F0, "StageState::addActionAlreadyNPC", StageState::addActionAlreadyNPC);

uint32_t StageState::addClearMission(StageState *self, uint32_t missionId) {
    uint32_t ids[3];
    self->readBlock(CLEAR_MISSIONS, ids, sizeof ids);
    for (uint32_t k = 0; k < 3; k++) {
        if (ids[k] != 0xFFFFFFFFu) continue;
        ids[k] = missionId;
        self->writeBlock(CLEAR_MISSIONS, ids, sizeof ids);
        return 1;
    }
    return 0;
}
RW_FUNCTION(0x4EF494, "StageState::addClearMission", StageState::addClearMission);

// The original compares the COUNT (second word) with the id (game bug, kept).
uint32_t StageState::hasMaterial(StageState *self, uint32_t id) {
    uint32_t list[2000];
    self->readBlock(MATERIALS, list, 8000);
    for (uint32_t k = 0; k < 1000; k++) {
        if (list[2 * k] == 0xFFFFFFFFu) return 0;
        if (list[2 * k + 1] == id) return 1;
    }
    return 0;
}
RW_FUNCTION(0x4EF52C, "StageState::hasMaterial", StageState::hasMaterial);

static uint32_t hasPairId(StageState *self, uint32_t index, uint32_t slots, uint32_t id) {
    uint32_t list[2000];
    self->readBlock(index, list, slots * 8);
    for (uint32_t k = 0; k < slots; k++) {
        if (list[2 * k] == 0xFFFFFFFFu) return 0;
        if (list[2 * k] == id) return 1;
    }
    return 0;
}

uint32_t StageState::hasEnemyDropItem(StageState *self, uint32_t id) { return hasPairId(self, DROP_ITEMS, 1000, id); }
RW_FUNCTION(0x4EF5B8, "StageState::hasEnemyDropItem", StageState::hasEnemyDropItem);

uint32_t StageState::hasTreasure(StageState *self, uint32_t id) { return hasPairId(self, TREASURES, 100, id); }
RW_FUNCTION(0x4EF648, "StageState::hasTreasure", StageState::hasTreasure);

uint32_t StageState::hasActionAlreadyNPC(StageState *self, uint32_t npcId) {
    uint32_t ids[20];
    self->readBlock(NPC_DONE, ids, sizeof ids);
    for (uint32_t k = 0; k < 20; k++) {
        if (ids[k] == 0xFFFFFFFFu) return 0;
        if (ids[k] == npcId) return 1;
    }
    return 0;
}
RW_FUNCTION(0x4EF6D0, "StageState::hasActionAlreadyNPC", StageState::hasActionAlreadyNPC);

// Number of runs of equal counts (second words) before the first -1 count; the first run never matches -1.
int32_t StageState::getHasMaterialCount(StageState *self) {
    uint32_t list[2000];
    self->readBlock(MATERIALS, list, 8000);
    int32_t n = 0;
    uint32_t prev = 0xFFFFFFFFu;
    for (uint32_t k = 0; k < 1000; k++) {
        uint32_t v = list[2 * k + 1];
        if (v == 0xFFFFFFFFu) break;
        if (v != prev) n++;
        prev = v;
    }
    return n;
}
RW_FUNCTION(0x4EF74C, "StageState::getHasMaterialCount", StageState::getHasMaterialCount);

int32_t StageState::getHasTreasureCount(StageState *self) {
    uint32_t list[200];
    self->readBlock(TREASURES, list, 800);
    int32_t n = 0;
    while (n < 100 && list[2 * n] != 0xFFFFFFFFu) n++;
    return n;
}
RW_FUNCTION(0x4EF7E0, "StageState::getHasTreasureCount", StageState::getHasTreasureCount);

void StageState::addExterminationTeamID(StageState *self, uint32_t teamId) {
    bt::GuestFrame frame(16);
    frame.setWord(8, teamId);
    // __insert_unique(sret pair<iterator,bool>, tree, const unsigned&)
    rw::call(tree_insert_unique, {frame.addr(0), rw::guest_addr(&self->teamsBegin), frame.addr(8)});
}
RW_FUNCTION(0x4EF944, "StageState::addExterminationTeamID", StageState::addExterminationTeamID);

uint32_t StageState::hasExterminationTeamID(StageState *self, uint32_t teamId) {
    bt::GuestFrame frame(8);
    frame.setWord(0, teamId);
    uint32_t it = rw::call(tree_find, {rw::guest_addr(&self->teamsBegin), frame.addr(0)});
    return it != rw::guest_addr(&self->teamsRoot) ? 1 : 0;
}
RW_FUNCTION(0x4EF994, "StageState::hasExterminationTeamID", StageState::hasExterminationTeamID);

// ------------------------------------------------------------------------------------------- sub-missions
// Each sub-mission block is 3 x (key, count). get/set index the block with an unchecked slot (0..2 in practice).
namespace {
struct SubMission {
    int32_t key, count;
};

int32_t subGet(StageState *self, uint32_t index, int32_t slot) {
    SubMission m[3];
    self->readBlock(index, m, sizeof m);
    return uint32_t(slot) < 3 ? m[slot].count : 0;
}

void subSet(StageState *self, uint32_t index, int32_t slot, int32_t key) {
    SubMission m[3];
    self->readBlock(index, m, sizeof m);
    if (uint32_t(slot) < 3) m[slot] = {key, 0};
    self->writeBlock(index, m, sizeof m);
}

// Count one more for every slot whose key matches (the whole block is written per match).
void subCount(StageState *self, uint32_t index, int32_t key) {
    SubMission m[3];
    self->readBlock(index, m, sizeof m);
    for (auto &e : m) {
        if (e.key != key) continue;
        e.count = int32_t(uint32_t(e.count) + 1);
        self->writeBlock(index, m, sizeof m);
    }
}
}  // namespace

int32_t StageState::getSubMission04(StageState *self, int32_t slot) { return subGet(self, SUB_MISSION04, slot); }
RW_FUNCTION(0x4EF9F4, "StageState::getSubMission04", StageState::getSubMission04);
void StageState::setSubMission04(StageState *self, int32_t slot, int32_t key) { subSet(self, SUB_MISSION04, slot, key); }
RW_FUNCTION(0x4EFA54, "StageState::setSubMission04", StageState::setSubMission04);
void StageState::cmpSubMission04(StageState *self, int32_t key) { subCount(self, SUB_MISSION04, key); }
RW_FUNCTION(0x4EF038, "StageState::cmpSubMission04", StageState::cmpSubMission04);

int32_t StageState::getSubMission12(StageState *self, int32_t slot) { return subGet(self, SUB_MISSION12, slot); }
RW_FUNCTION(0x4EFAE4, "StageState::getSubMission12", StageState::getSubMission12);
void StageState::setSubMission12(StageState *self, int32_t slot, int32_t key) { subSet(self, SUB_MISSION12, slot, key); }
RW_FUNCTION(0x4EFB44, "StageState::setSubMission12", StageState::setSubMission12);
void StageState::cmpSubMission12(StageState *self, int32_t key) { subCount(self, SUB_MISSION12, key); }
RW_FUNCTION(0x4EFBD4, "StageState::cmpSubMission12", StageState::cmpSubMission12);

int32_t StageState::getSubMission14(StageState *self, int32_t slot) { return subGet(self, SUB_MISSION14, slot); }
RW_FUNCTION(0x4EFC64, "StageState::getSubMission14", StageState::getSubMission14);
void StageState::setSubMission14(StageState *self, int32_t slot, int32_t key) { subSet(self, SUB_MISSION14, slot, key); }
RW_FUNCTION(0x4EFCC4, "StageState::setSubMission14", StageState::setSubMission14);
void StageState::cmpSubMission14(StageState *self, int32_t key) { subCount(self, SUB_MISSION14, key); }
RW_FUNCTION(0x4EFD54, "StageState::cmpSubMission14", StageState::cmpSubMission14);

int32_t StageState::getSubMission15(StageState *self, int32_t slot) { return subGet(self, SUB_MISSION15, slot); }
RW_FUNCTION(0x4EFDE4, "StageState::getSubMission15", StageState::getSubMission15);
void StageState::setSubMission15(StageState *self, int32_t slot, int32_t key) { subSet(self, SUB_MISSION15, slot, key); }
RW_FUNCTION(0x4EFE44, "StageState::setSubMission15", StageState::setSubMission15);

// Keep the highest value per matching key (signed compare).
void StageState::cmpSubMission15(StageState *self, int32_t key, int32_t value) {
    SubMission m[3];
    self->readBlock(SUB_MISSION15, m, sizeof m);
    for (auto &e : m) {
        if (e.key != key || e.count >= value) continue;
        e.count = value;
        self->writeBlock(SUB_MISSION15, m, sizeof m);
    }
}
RW_FUNCTION(0x4EFED4, "StageState::cmpSubMission15", StageState::cmpSubMission15);

int32_t StageState::getSubMission19(StageState *self, int32_t slot) { return subGet(self, SUB_MISSION19, slot); }
RW_FUNCTION(0x4EFF70, "StageState::getSubMission19", StageState::getSubMission19);
void StageState::setSubMission19(StageState *self, int32_t slot, int32_t key) { subSet(self, SUB_MISSION19, slot, key); }
RW_FUNCTION(0x4EFFD0, "StageState::setSubMission19", StageState::setSubMission19);
void StageState::cmpSubMission19(StageState *self, int32_t key) { subCount(self, SUB_MISSION19, key); }
RW_FUNCTION(0x4EF1F8, "StageState::cmpSubMission19", StageState::cmpSubMission19);

// ------------------------------------------------------------------------------------------- buffs / conditions
int32_t StageState::getBadStatusCount(StageState *self, int32_t type) {
    if (type > 3) return 0;
    int32_t counts[4];
    self->readBlock(BAD_STATUS_COUNT, counts, sizeof counts);
    return type >= 0 ? counts[type] : 0;
}
RW_FUNCTION(0x4F0060, "StageState::getBadStatusCount", StageState::getBadStatusCount);

void StageState::addBadStatusCount(StageState *self, int32_t type) {
    if (type > 3) return;
    int32_t counts[4];
    self->readBlock(BAD_STATUS_COUNT, counts, sizeof counts);
    if (type >= 0) counts[type] = int32_t(uint32_t(counts[type]) + 1);
    self->writeBlock(BAD_STATUS_COUNT, counts, sizeof counts);
}
RW_FUNCTION(0x4F00C8, "StageState::addBadStatusCount", StageState::addBadStatusCount);

// BuffChange (12 bytes) of slot `slot`, returned through out (sret).
void StageState::getBuff(const uint8_t *out, const StageState *self, int32_t slot) {
    rw::store32(out, 0);
    rw::store32(out + 4, 0);
    rw::store32(out + 8, 0);
    uint32_t index = uint32_t(slot) * 12u + BUFFS;
    SafeBuffer::read(self, out, self->data.get() + index, 12, index);
}
RW_FUNCTION(0x4F0154, "StageState::getBuff", StageState::getBuff);

void StageState::addBuff(StageState *self, const uint8_t *change) {
    uint32_t slot = rw::call(IBuffProtocol_getBlankBuff, {rw::guest_addr(&self->buffVtable)});
    uint32_t index = slot * 12u + BUFFS;
    SafeBuffer::write(self, self->data.get() + index, change, 12, index);
}
RW_FUNCTION(0x4F01C4, "StageState::addBuff", StageState::addBuff);

void StageState::setBuff(StageState *self, int32_t slot, const uint8_t *change) {
    uint32_t index = uint32_t(slot) * 12u + BUFFS;
    SafeBuffer::write(self, self->data.get() + index, change, 12, index);
}
RW_FUNCTION(0x4F0238, "StageState::setBuff", StageState::setBuff);
// the IBuffProtocol base's (+0x24) view
void thunkSetBuff(uint32_t buffs, int32_t slot, const uint8_t *change) {
    StageState::setBuff(const_cast<StageState *>(rw::host_ptr<StageState>(buffs - 0x24)), slot, change);
}
RW_FUNCTION(0x4F0258, "thunk to StageState::setBuff", thunkSetBuff);
// the IStatusProtocol base's (+0x20) view (void: setCondition returns nothing)
void thunkSetCondition(uint32_t status, uint32_t type, uint32_t condition) {
    rw::call(0x4F03A9, {status - 0x20, type, condition});   // StageState::setCondition
}
RW_FUNCTION(0x4F03EC, "thunk to StageState::setCondition", thunkSetCondition);

int32_t StageState::getBuffChange(const StageState *self, int32_t type, int32_t target) {
    return self->get32((uint32_t(type) * 4u + uint32_t(target)) * 4u + BUFF_CHANGES);
}
RW_FUNCTION(0x4F0280, "StageState::getBuffChange", StageState::getBuffChange);

void StageState::setBuffChange(StageState *self, int32_t type, int32_t target, int32_t value) {
    self->set32((uint32_t(type) * 4u + uint32_t(target)) * 4u + BUFF_CHANGES, value);
}
RW_FUNCTION(0x4F02DC, "StageState::setBuffChange", StageState::setBuffChange);

// StatusChange (12 bytes) of a condition, returned through out (sret).
void StageState::getCondition(const uint8_t *out, const StageState *self, int32_t type) {
    rw::store32(out, 0);
    rw::store32(out + 4, 0);
    rw::store32(out + 8, 0);
    uint32_t index = uint32_t(type) * 12u + CONDITIONS;
    SafeBuffer::read(self, out, self->data.get() + index, 12, index);
}
RW_FUNCTION(0x4F0338, "StageState::getCondition", StageState::getCondition);

// Store a condition; an active one (word 2 set) counts as one more bad status on the live stage state.
void StageState::setCondition(StageState *self, int32_t type, const uint8_t *change) {
    uint32_t index = uint32_t(type) * 12u + CONDITIONS;
    SafeBuffer::write(self, self->data.get() + index, change, 12, index);
    if (rw::get<uint32_t>(change, 8) != 0) addBadStatusCount(current(), type);
}
RW_FUNCTION(0x4F03A8, "StageState::setCondition", StageState::setCondition);

void StageState::addCondition(StageState *self, int32_t type, int32_t a, int32_t b) {
    rw::call(IStatusProtocol_addCondition_common,
             {rw::guest_addr(&self->statusVtable), uint32_t(type), uint32_t(a), uint32_t(b)});
    addBadStatusCount(current(), type);
}
RW_FUNCTION(0x4F03F6, "StageState::addCondition", StageState::addCondition);

// BuffInfoDispParam by value (14 words): p0..p7 = buff values for (BuffType, BuffTarget) (0,0) (1,0) (0,1) (1,1)
// (0,2) (1,2) (0,3) (1,3); p8/p9 = buff turns / param; p10/p11 = status ids (-1 = none) applied through the
// virtual at vtable + 16 with p12/p13.
void StageState::set_buff_param(StageState *self, uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4,
                                uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10,
                                uint32_t p11, uint32_t p12, uint32_t p13) {
    uint32_t buffs = rw::guest_addr(&self->buffVtable);
    rw::call(IBuffProtocol_buff_clear_all, {buffs});
    if (p8 + p9 != 0) {
        const uint32_t values[8] = {p0, p1, p2, p3, p4, p5, p6, p7};
        for (uint32_t k = 0; k < 8; k++) {
            if (values[k]) rw::call(IBuffProtocol_buff_add, {buffs, values[k], p8, p9, k & 1, k >> 1});
        }
    }
    if (p12 + p13 != 0) {
        if (p10 != 0xFFFFFFFFu) rw::vcall(self, VSLOT_STATUS_APPLY, {rw::guest_addr(self), p10, p12, p13});
        if (p11 != 0xFFFFFFFFu) rw::vcall(self, VSLOT_STATUS_APPLY, {rw::guest_addr(self), p11, p12, p13});
    }
}
RW_FUNCTION(0x4F042C, "StageState::set_buff_param", StageState::set_buff_param);

// Poison tick for the playable actor: when the player is alive and poisoned (condition 0, master Badstatus 101)
// or else under condition 3 (Badstatus 103), deal (base +- range/2 random) * maxHP / 10000 damage (at least 1
// when the product is within +-9999), play the damage motion and show the number. Returns 1 when it hit.
uint32_t StageState::setPlayableActorPoisonEffect(StageState *self, const uint8_t *actor, bool) {
    bt::GuestFrame frame(32);
    const uint32_t condition = frame.addr(0);    // StatusChange (sret, 12 bytes)
    auto *status = frame.ptr<bt::SharedPtr>(16); // master::Badstatus
    const uint32_t pos = frame.addr(24);         // cocos2d::Vec2

    if (current()->get32(HP) == 0) return 0;
    const uint32_t statusBase = rw::guest_addr(&self->statusVtable);
    rw::vcall(&self->statusVtable, 0, {condition, statusBase, 0});      // getCondition(poison)
    uint32_t badstatusId = 101;
    if (frame.word(8) == 0) {
        rw::vcall(&self->statusVtable, 0, {condition, statusBase, 3});
        if (frame.word(8) == 0) return 0;
        badstatusId = 103;
    }
    rw::call(master_Badstatus_create, {frame.addr(16), badstatusId});
    const SafeBuffer *row = rw::host_ptr<SafeBuffer>(status->obj);
    int32_t base = row->get32(84);
    int32_t range = row->get32(88);
    uint32_t random = rw::call(Random_getInstance, {});
    int32_t half = range / 2;
    int32_t roll = int32_t(rw::call(Random_generateUniformInt, {random, uint32_t(-half), uint32_t(half), 0}));
    uint32_t product = (uint32_t(roll) + uint32_t(base)) * uint32_t(userMaxHp());
    int32_t damage = 1;
    if (product + 9999u >= 0x4E1Fu) damage = int32_t(product) / 10000;
    addDamage(current(), damage);

    uint32_t avatar = rw::get<uint32_t>(actor, 0x288);
    rw::call(Avatar_changeMotion, {avatar, 17});
    rw::call(FlashNode_reflection_status, {rw::get<uint32_t>(actor, 0x288), 1});
    uint32_t node = rw::load<uint32_t>(rw::get<uint32_t>(actor, 0x290) + 0x274);
    uint32_t actorPos = rw::vcall(actor, 21, {rw::guest_addr(actor)});    // getPosition
    rw::call(Vec2_copy, {pos, actorPos});
    // disp_dmgNum(damage, node, Vec2, 70.0f, false, false, true, false, 400, 1.0f)
    rw::call(DisplayDamageNumber_disp_dmgNum,
             {uint32_t(damage), node, pos, 0x428C0000u, 0, 0, 1, 0, 400, 0x3F800000u});
    rw::call(Vec2_dtor, {pos});
    bt::releaseShared(status->ctrl);
    return 1;
}
RW_FUNCTION(0x4F05C8, "StageState::setPlayableActorPoisonEffect", StageState::setPlayableActorPoisonEffect);
