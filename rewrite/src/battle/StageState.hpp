// StageState: the running quest's player state (HP, special gauge CP, Lux, points, collected items, sub-mission
// counters, buffs and bad statuses), kept in a SafeBuffer so it cannot be edited in memory. Values sit at fixed
// data positions. The object also implements IStatusProtocol (vtable at +32) and IBuffProtocol (vtable at +36),
// and keeps the set of exterminated enemy teams in a std::set<unsigned> at +56.
// The live instance is AppState::getInstance()->stageState (AppState + 32).
#pragma once

#include "BattleCommon.hpp"

struct StageState : SafeBuffer {
    rw::Field<uint32_t> statusVtable;      // +32 IStatusProtocol base (vtable slot 0: getCondition, sret)
    rw::Field<uint32_t> buffVtable;        // +36 IBuffProtocol base
    rw::Field<uint32_t> field_28;          // +40 cleared by create
    rw::Field<uint32_t> field_2C;          // +44
    rw::Field<uint32_t> field_30;          // +48
    rw::Field<uint32_t> field_34;          // +52
    // std::set<unsigned> exterminated enemy team ids (libc++ __tree: begin node, root (= end node's left), size)
    rw::Field<uint32_t> teamsBegin;        // +56
    rw::Field<uint32_t> teamsRoot;         // +60 (its address is the end node)
    rw::Field<uint32_t> teamsSize;         // +64

    // ---- data positions (bytes into the SafeBuffer data)
    static constexpr uint32_t PLAYABLE = 0;          // 1 byte: create(bool)
    static constexpr uint32_t HP = 8;                // current HP
    static constexpr uint32_t CP = 12;               // special attack gauge (10000 per gauge point)
    static constexpr uint32_t LUX = 16;              // Lux collected (max 9,999,999)
    static constexpr uint32_t RAID_POINT = 20;       // raid points (max 9,999,999)
    static constexpr uint32_t EXP = 24;              // experience (max 999,999,999)
    static constexpr uint32_t MONEY = 28;            // munny (max 999,999,999)
    static constexpr uint32_t DEFEATED_KIND1 = 32;   // defeated enemies per master::Enemy kind (1, 2, 3)
    static constexpr uint32_t DEFEATED_KIND2 = 36;
    static constexpr uint32_t DEFEATED_KIND3 = 40;
    static constexpr uint32_t MAX_ATTACK = 44;       // highest single attack
    static constexpr uint32_t CP_MAX = 52;           // gauge maximum (the keyblade's burst * 10000)
    static constexpr uint32_t MATERIALS = 56;        // 1000 x (materialId, count), unused = -1
    static constexpr uint32_t DROP_ITEMS = 8056;     // 1000 x (itemId, count), unused = -1
    static constexpr uint32_t TREASURES = 16056;     // 100 x (treasureId, value), unused = -1
    static constexpr uint32_t NPC_DONE = 16856;      // 20 NPC ids already talked to, unused = -1
    static constexpr uint32_t CLEAR_MISSIONS = 16936;  // 3 cleared mission ids, unused = -1
    static constexpr uint32_t FIELD_4238 = 16952;    // set to 10000 by create
    static constexpr uint32_t FIELD_4250 = 16976;    // set to 1 by create
    static constexpr uint32_t BUFFS = 17004;         // BuffChange[] (12 bytes each)
    static constexpr uint32_t BUFF_CHANGES = 17772;  // int[BuffType * 4 + BuffTarget]
    static constexpr uint32_t CONDITIONS = 17804;    // StatusChange[ConditionType] (12 bytes each)
    static constexpr uint32_t SUB_MISSION04 = 17860; // sub-mission counters: 3 x (key, count)
    static constexpr uint32_t SUB_MISSION12 = 17884;
    static constexpr uint32_t SUB_MISSION14 = 17908;
    static constexpr uint32_t SUB_MISSION15 = 17932;
    static constexpr uint32_t SUB_MISSION19 = 17956;
    static constexpr uint32_t BAD_STATUS_COUNT = 17980;  // int[4] per ConditionType

    static constexpr uint32_t VSLOT_MAX_HP = 8;
    static constexpr uint32_t VSLOT_STATUS_APPLY = 4;    // virtual at vtable + 16: (id, value, turns)

    int32_t maxHp() const { return int32_t(rw::vcall(this, VSLOT_MAX_HP, {rw::guest_addr(this)})); }

    // Decode n data bytes at `index` into a host buffer (SafeBuffer::read without the stack stores).
    void readBlock(uint32_t index, void *out, uint32_t n) const;
    // Encode n bytes from a host buffer into the data at `index` (SafeBuffer::write).
    void writeBlock(uint32_t index, const void *in, uint32_t n);

    // AppState::getInstance()->stageState
    static StageState *current();

    static void create(bt::SharedPtr *out, uint32_t playable);
    static StageState *destroy(StageState *self);
    static void deletingDestroy(StageState *self);
    static int32_t getMaxHP(const StageState *self);

    // Apply damage (negative = healing); HP stays within [0, max]. Returns the new HP.
    static int32_t addDamage(StageState *self, int32_t damage);
    static int32_t heal(StageState *self, int32_t amount);
    // Add to the special gauge; it stays within [0, CP_MAX]. Returns the new value.
    static int32_t addCP(StageState *self, int32_t amount);
    static void setCpMax(StageState *self, int32_t keybladeId);
    static int32_t getCpMax(StageState *self);
    static uint32_t addLUX(StageState *self, uint32_t amount);
    static uint32_t addRaidpoint(StageState *self, uint32_t amount);
    static uint32_t addEXP(StageState *self, uint32_t amount);
    static uint32_t addMoney(StageState *self, uint32_t amount);
    static int32_t addDefeatedEnemy(StageState *self, int32_t enemyId);
    static uint32_t checkMaxAttack(StageState *self, uint32_t attack);

    static uint32_t addMaterial(StageState *self, uint32_t id, uint32_t count);
    static uint32_t addEnemyDropItem(StageState *self, uint32_t id, uint32_t count);
    static uint32_t addTreasure(StageState *self, uint32_t id, uint32_t value);
    static uint32_t addActionAlreadyNPC(StageState *self, uint32_t npcId);
    static uint32_t addClearMission(StageState *self, uint32_t missionId);
    static uint32_t hasMaterial(StageState *self, uint32_t id);
    static uint32_t hasEnemyDropItem(StageState *self, uint32_t id);
    static uint32_t hasTreasure(StageState *self, uint32_t id);
    static uint32_t hasActionAlreadyNPC(StageState *self, uint32_t npcId);
    static int32_t getHasMaterialCount(StageState *self);
    static int32_t getHasTreasureCount(StageState *self);
    static void addExterminationTeamID(StageState *self, uint32_t teamId);
    static uint32_t hasExterminationTeamID(StageState *self, uint32_t teamId);

    // sub-mission counters
    static int32_t getSubMission04(StageState *self, int32_t slot);
    static void setSubMission04(StageState *self, int32_t slot, int32_t key);
    static void cmpSubMission04(StageState *self, int32_t key);
    static int32_t getSubMission12(StageState *self, int32_t slot);
    static void setSubMission12(StageState *self, int32_t slot, int32_t key);
    static void cmpSubMission12(StageState *self, int32_t key);
    static int32_t getSubMission14(StageState *self, int32_t slot);
    static void setSubMission14(StageState *self, int32_t slot, int32_t key);
    static void cmpSubMission14(StageState *self, int32_t key);
    static int32_t getSubMission15(StageState *self, int32_t slot);
    static void setSubMission15(StageState *self, int32_t slot, int32_t key);
    static void cmpSubMission15(StageState *self, int32_t key, int32_t value);
    static int32_t getSubMission19(StageState *self, int32_t slot);
    static void setSubMission19(StageState *self, int32_t slot, int32_t key);
    static void cmpSubMission19(StageState *self, int32_t key);

    // buffs / conditions
    static int32_t getBadStatusCount(StageState *self, int32_t type);
    static void addBadStatusCount(StageState *self, int32_t type);
    static void getBuff(const uint8_t *out, const StageState *self, int32_t slot);
    static void addBuff(StageState *self, const uint8_t *change);
    static void setBuff(StageState *self, int32_t slot, const uint8_t *change);
    static int32_t getBuffChange(const StageState *self, int32_t type, int32_t target);
    static void setBuffChange(StageState *self, int32_t type, int32_t target, int32_t value);
    static void getCondition(const uint8_t *out, const StageState *self, int32_t type);
    static void setCondition(StageState *self, int32_t type, const uint8_t *change);
    static void addCondition(StageState *self, int32_t type, int32_t a, int32_t b);
    static void set_buff_param(StageState *self, uint32_t p0, uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4,
                               uint32_t p5, uint32_t p6, uint32_t p7, uint32_t p8, uint32_t p9, uint32_t p10,
                               uint32_t p11, uint32_t p12, uint32_t p13);
    static uint32_t setPlayableActorPoisonEffect(StageState *self, const uint8_t *actor, bool unused);
};
RW_LAYOUT(StageState, statusVtable, 32);
RW_LAYOUT(StageState, field_28, 40);
RW_LAYOUT(StageState, teamsBegin, 56);
