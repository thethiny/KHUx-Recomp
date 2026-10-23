// RaidBossActor: the raid boss on the raid battle field - a MapNode that owns the RaidBoss LWF character, the
// state of the boss and of each of its parts (RaidParts: HP and the per-level rewards from master::RaidEnemy; up to 6
// parts, index 0 = the boss body, 1..5 = the parts, master RaidEnemy::parts[] lists the parts' RaidEnemy ids, whose
// `weakness` field is their part index), the buffs / bad status of the boss (Property) and of each part
// (PartsProperty::Property), the target markers (BattleMarker per click point of the RaidBoss LWF) and the prizes
// (munny, Lux, coins) that erupt when a part is hit or crushed.
#pragma once

#include "actor/MapNode.hpp"
#include "battleui/RaidBoss.hpp"
#include "system/PartsProperty.hpp"

// RaidParts: one part of the raid boss (SafeBuffer of 0x60 bytes) + the damage flags of the last attack.
struct RaidParts : SafeBuffer {
    rw::Field<uint32_t> attackFlags;   // +32 std::bitset<3> of the last attack (bit 0..2: disp_dmgNum flags)

    // data positions
    static constexpr uint32_t HP = 8;                  // current HP
    static constexpr uint32_t DAMAGE = 12;             // damage dealt by the player so far
    static constexpr uint32_t MAX_HP = 16;
    static constexpr uint32_t TYPE = 20;               // RaidBossHPIndicator::Type of its marker (1..3: attribute)
    static constexpr uint32_t ATTACK = 24;
    static constexpr uint32_t DEFENSE = 28;
    static constexpr uint32_t EXP_ATTACK = 32;
    static constexpr uint32_t EXP_SUPPRESS = 36;
    static constexpr uint32_t MONEY_ATTACK = 40;
    static constexpr uint32_t MONEY_SUPPRESS = 44;
    static constexpr uint32_t LUX_ATTACK = 48;
    static constexpr uint32_t LUX_SUPPRESS = 52;
    static constexpr uint32_t BURST_ATTACK = 56;
    static constexpr uint32_t BURST_SUPPRESS = 60;
    static constexpr uint32_t HP_ATTACK = 64;
    static constexpr uint32_t HP_SUPPRESS = 68;
    static constexpr uint32_t GUILT_SUPPRESS = 72;
    static constexpr uint32_t REWARD_ID = 76;
    static constexpr uint32_t MVP_REWARD_ID = 80;
    static constexpr uint32_t DESTROY_ALL_REWARD_ID = 84;
    static constexpr uint32_t ATTACK_DAMAGE = 88;      // damage of the attack being shown
    static constexpr uint32_t TOTAL_ATTACK_DAMAGE = 92;

    static constexpr uint32_t VTABLE = 0xF82FEC;       // _ZTV9RaidParts + 8
    static constexpr uint32_t SET_MASTER_PARAM = 0x5F1551;   // set_master_param(shared_ptr<RaidEnemy>, level, id, hp)
};
RW_LAYOUT(RaidParts, attackFlags, 32);
RW_SIZE(RaidParts, 36);

// PrizeParam: a SafeBuffer(8): two floats (prize rate multipliers) per part.
struct PrizeParam : SafeBuffer {
    static constexpr uint32_t VTABLE = 0xF96490;       // _ZTV10PrizeParam + 8
    static constexpr uint32_t RATE_A = 0;              // float
    static constexpr uint32_t RATE_B = 4;              // float
};
RW_SIZE(PrizeParam, 32);

struct RaidBossActor : MapNode {
    // The boss's own state: a SafeBuffer (conditions, buffs, the raid end date, the number of parts) implementing
    // IStatusProtocol (+32) and IBuffProtocol (+36).
    struct Property : SafeBuffer {
        rw::Field<uint32_t> statusVtable;   // +32 IStatusProtocol
        rw::Field<uint32_t> buffVtable;     // +36 IBuffProtocol

        static constexpr uint32_t SIZE = 0x33C;          // data bytes
        static constexpr uint32_t CONDITIONS = 0;        // StatusChange[4]
        static constexpr uint32_t BUFFS = 48;            // BuffChange[64]
        static constexpr uint32_t END_DATE = 816;        // Date (8 bytes)
        static constexpr uint32_t PART_COUNT = 824;      // int: the RaidEnemy's validParts + 1
        static constexpr uint32_t VTABLE = 0xF963D8;     // _ZTVN13RaidBossActor8PropertyE + 8
        static constexpr uint32_t STATUS_VTABLE = 0xF96408;
        static constexpr uint32_t BUFF_VTABLE = 0xF96424;

        static void getCondition(const uint8_t *out, const Property *self, int32_t type);
        static void setCondition(Property *self, int32_t type, const uint8_t *change);
        static void addCondition(Property *self, int32_t type, int32_t turns, int32_t count);
        static void getBuff(const uint8_t *out, const Property *self, int32_t slot);
        static void addBuff(Property *self, const uint8_t *change);
        static void setBuff(Property *self, int32_t slot, const uint8_t *change);
        static void deletingDestroy(Property *self);
        static void heal(Property *self, int32_t amount);
        static int32_t getMaxHP(const Property *self);
    };
    RW_SIZE(Property, 40);

    Property property;                     // +648
    parts::Property partProperty[6];       // +688 buffs per part (36 bytes each)
    bt::Vec2 bodyMarkerPos;                // +904 position of the body (part 0) marker
    bt::Vec2 wholeAttackPoint;             // +912 centre of the live parts' markers
    rw::Field<int32_t> targetPart;         // +920 part targeted (-1: all / none)
    rw::Field<int32_t> field_924;          // +924 (-1 from make_marker)
    rw::Field<uint8_t> partHit[6];         // +928 parts hit by the current attack
    const uint8_t pad_3A6[2];
    rw::Field<uint32_t> statusIndicator;   // +936 RaidBossStatusIndicator* (retained)
    rw::Field<uint32_t> fieldNode;         // +940 node the damage numbers / prizes are added to
    rw::Field<uint32_t> statusBits;        // +944 the RaidBoss's FlashNode::statusBits (UpdateStatus)
    RaidParts parts[6];                    // +948
    // +1164: 11 records of 72 bytes holding a std::string at +12 (the constructor zeroes the strings)
    const uint8_t records[792];
    const uint8_t pad_7A4[4];              // +1956
    rw::Field<uint32_t> dropItem;          // +1960 MapPrizes* of the drop item (setDropItem / addDropItem)
    bt::Vector prizeList;                  // +1964 cocos2d::Vector<MapPrize*>
    bt::Vector prizes;                     // +1976 cocos2d::Vector<MapPrizes*>
    PrizeParam prizeParams[6];             // +1988
    bt::SharedPtr skill;                   // +2180 shared_ptr<const master::Skill> (setPrizeParam)
    // std::unordered_map<int, BattleMarker*> markers (part index -> marker): buckets, bucket count, first node,
    // size, max_load_factor; node = next, hash, key (+8), value (+12)
    rw::Field<uint32_t> markerBuckets;     // +2188
    rw::Field<uint32_t> markerBucketCount; // +2192
    rw::Field<uint32_t> markerFirst;       // +2196
    rw::Field<uint32_t> markerCount;       // +2200
    rw::Field<float> markerMaxLoad;        // +2204
    rw::PtrField<RaidBoss> boss;           // +2208
    rw::Field<int32_t> raidEnemyId;        // +2212
    rw::Field<int32_t> level;              // +2216
    rw::Field<uint8_t> field_8AC;          // +2220
    const uint8_t pad_8AD[3];

    static constexpr uint32_t SIZE = 0x8B0;
    static constexpr uint32_t VTABLE = 0xF96108;         // _ZTV13RaidBossActor + 8
    static constexpr uint32_t MARKERS = 2188;

    static RaidBossActor *create(int32_t raidEnemyId, int32_t level);
    static RaidBossActor *construct(RaidBossActor *self);
    static uint32_t init(RaidBossActor *self, int32_t raidEnemyId, int32_t level);
    static RaidBossActor *destruct(RaidBossActor *self);
    static void destructDelete(RaidBossActor *self);

    // damage (RaidBossActor_damage.cpp)
    static uint32_t setEnemyPartDamage(RaidBossActor *self, const uint32_t *entry, uint32_t damage,
                                       const guest::SharedPtr *burst);
    static uint32_t addDamage(RaidBossActor *self, int32_t part, uint32_t damage, const guest::SharedPtr *burst);
    static void updateDamageAll(RaidBossActor *self);
    static uint32_t setEnemyDamage(RaidBossActor *self, const guest::SharedPtr *burst, uint32_t critical);
    static int32_t getCurrentHP(const RaidBossActor *self, int32_t part);
    static int32_t getAttackDamage(const RaidBossActor *self, int32_t part);
    static uint32_t getAttackDamageFlags(const RaidBossActor *self, int32_t part);
    static int32_t getType(const RaidBossActor *self, int32_t part);
    static int32_t getMaxHP(const RaidBossActor *self, int32_t part);
    static void setCurrentHP(const RaidBossActor *self, int32_t part, int32_t hp);
    static void setTotalAttackDamage(const RaidBossActor *self, int32_t part, int32_t damage);
    static int32_t getTotalAttackDamage(const RaidBossActor *self, int32_t part);
    static int32_t getCurrentDamage(const RaidBossActor *self, int32_t part);
    static int32_t getDefense(const RaidBossActor *self, int32_t part);
    static void setCurrentDamage(const RaidBossActor *self, int32_t part, int32_t damage);
    static void setAttackDamage(const RaidBossActor *self, int32_t part, int32_t damage, const uint32_t *flags);
    static int32_t getAttack(const RaidBossActor *self, int32_t part);
    static int32_t getExpAttack(const RaidBossActor *self, int32_t part);
    static int32_t getExpSuppress(const RaidBossActor *self, int32_t part);
    static int32_t getMoneyAttack(const RaidBossActor *self, int32_t part);
    static int32_t getMoneySuppress(const RaidBossActor *self, int32_t part);
    static int32_t getLuxAttack(const RaidBossActor *self, int32_t part);
    static int32_t getLuxSuppress(const RaidBossActor *self, int32_t part);
    static int32_t getBurstAttack(const RaidBossActor *self, int32_t part);
    static int32_t getBurstSuppress(const RaidBossActor *self, int32_t part);
    static int32_t getHpAttack(const RaidBossActor *self, int32_t part);
    static int32_t getHpSuppress(const RaidBossActor *self, int32_t part);
    static int32_t getGuiltSuppress(const RaidBossActor *self, int32_t part);
    static int32_t getRewardId(const RaidBossActor *self, int32_t part);
    static int32_t getMvpRewardId(const RaidBossActor *self, int32_t part);
    static int32_t getDestroyAllRewardId(const RaidBossActor *self, int32_t part);
    static int32_t get_protectPoison(const RaidBossActor *self, const guest::SharedPtr *enemy);
    static int32_t get_protectDeepPoison(const RaidBossActor *self, const guest::SharedPtr *enemy);
    static int32_t get_protectSleep(const RaidBossActor *self, const guest::SharedPtr *enemy);
    static int32_t get_protectParalysis(const RaidBossActor *self, const guest::SharedPtr *enemy);
    static int32_t get_protectConfusion(const RaidBossActor *self, const guest::SharedPtr *enemy);
    static int32_t get_registPoison(const RaidBossActor *self, const guest::SharedPtr *enemy);
    static int32_t get_registDeepPoison(const RaidBossActor *self, const guest::SharedPtr *enemy);
    static int32_t get_registSleep(const RaidBossActor *self, const guest::SharedPtr *enemy);
    static int32_t get_registParalysis(const RaidBossActor *self, const guest::SharedPtr *enemy);
    static int32_t get_registConfusion(const RaidBossActor *self, const guest::SharedPtr *enemy);

    // markers (RaidBossActor_markers.cpp)
    static void updateTargetPosition(RaidBossActor *self);
    static uint32_t getGuage(RaidBossActor *self, int32_t part);
    static void ShowTarget(RaidBossActor *self, uint32_t select);
    static void ReselectTarget(RaidBossActor *self);
    static void SelectedTarget(RaidBossActor *self);
    static void CancelTarget(RaidBossActor *self);
    static void ShowTargetInex(RaidBossActor *self, int32_t part);
    static void HideTargetInex(RaidBossActor *self, int32_t part);
    static void marker_mode_chamge(RaidBossActor *self);
    static void HideTarget(RaidBossActor *self);
    static void HideTargetNoSelected(RaidBossActor *self, int32_t part);
    static int32_t get_active_parts(RaidBossActor *self);
    static void make_marker(RaidBossActor *self, uint32_t scene, uint32_t parent, float scale);
    static void makeWholeAttackPoint(RaidBossActor *self);
    static int32_t getEnemyHp(RaidBossActor *self);
    static void find_marker(const bt::Vec2 *out, RaidBossActor *self, int32_t part);
    static uint32_t createRaidBossStatusField(RaidBossActor *self, uint32_t name, uint32_t hp, uint32_t hpMax,
                                              int32_t level, int32_t type);
    static void setPrizeParam(RaidBossActor *self, int32_t part, float rateA, float rateB,
                              const guest::SharedPtr *skill);

    // stats (RaidBossActor_stats.cpp)
    static void set_strat_hp(RaidBossActor *self);
    static void set_buff_param(RaidBossActor *self, int32_t attack, int32_t defense, int32_t attackPower,
                               int32_t defensePower, int32_t attackSpeed, int32_t defenseSpeed, int32_t attackMagic,
                               int32_t defenseMagic, int32_t turns, int32_t count, int32_t condition0,
                               int32_t condition1, int32_t conditionTurns, int32_t conditionCount);
    static void set_buff_param_parts(RaidBossActor *self, int32_t attack, int32_t defense, int32_t attackPower,
                                     int32_t defensePower, int32_t attackSpeed, int32_t defenseSpeed,
                                     int32_t attackMagic, int32_t defenseMagic, int32_t turns, int32_t count,
                                     int32_t condition0, int32_t condition1, int32_t conditionTurns,
                                     int32_t conditionCount, int32_t part);
    static uint32_t get_api_hp(uint32_t self, int32_t index);
    static int32_t calc_total_real_damage(RaidBossActor *self);
    static int32_t calc_total_attack_damage(RaidBossActor *self);
    static void api_reflect(RaidBossActor *self);
    static float MinHPRateRaidTeam(RaidBossActor *self);
    static int32_t getRemainingTime(const RaidBossActor *self);
    static int32_t getNumberOfParts(const RaidBossActor *self);

    // status (RaidBossActor_status.cpp)
    static void raidAddCondition(RaidBossActor *self, int32_t type, int32_t turns, int32_t count);
    static void elapsePartsTurn(RaidBossActor *self, int32_t part);
    static void elapsePartsCount(RaidBossActor *self, int32_t part);
    static void UpdateStatus(RaidBossActor *self);

    // prizes (RaidBossActor_prizes.cpp)
    static void addCrushingPrizes(RaidBossActor *self, int32_t part);
    static void addCrushingPrizesLux(RaidBossActor *self, int32_t part, int32_t hp);
    static void addAttackPrizes(RaidBossActor *self, int32_t part, int32_t rate);
    static void addAttackPrizesLux(RaidBossActor *self, int32_t part, int32_t damage);
    static void setAdvantageousEffect(RaidBossActor *self, int32_t attribute, int32_t part);
    static void addPrizes(RaidBossActor *self, uint32_t prizes, int32_t part);
    static void setDropItem(RaidBossActor *self, uint32_t prizes);
    static void addDropItem(RaidBossActor *self, int32_t part);
    static void prizesLuxCommon(RaidBossActor *self, int32_t part, int32_t amount, const uint32_t *money,
                                const uint32_t *lux, const uint32_t *exp);
    static void prizesLuxPostCommon(RaidBossActor *self, int32_t part, uint32_t money, uint32_t lux, uint32_t exp,
                                    uint32_t crushed);
};
RW_LAYOUT(RaidBossActor, property, 648);
RW_LAYOUT(RaidBossActor, partProperty, 688);
RW_LAYOUT(RaidBossActor, bodyMarkerPos, 904);
RW_LAYOUT(RaidBossActor, wholeAttackPoint, 912);
RW_LAYOUT(RaidBossActor, targetPart, 920);
RW_LAYOUT(RaidBossActor, partHit, 928);
RW_LAYOUT(RaidBossActor, statusIndicator, 936);
RW_LAYOUT(RaidBossActor, parts, 948);
RW_LAYOUT(RaidBossActor, records, 1164);
RW_LAYOUT(RaidBossActor, dropItem, 1960);
RW_LAYOUT(RaidBossActor, prizeList, 1964);
RW_LAYOUT(RaidBossActor, prizes, 1976);
RW_LAYOUT(RaidBossActor, prizeParams, 1988);
RW_LAYOUT(RaidBossActor, skill, 2180);
RW_LAYOUT(RaidBossActor, markerBuckets, 2188);
RW_LAYOUT(RaidBossActor, markerMaxLoad, 2204);
RW_LAYOUT(RaidBossActor, boss, 2208);
RW_LAYOUT(RaidBossActor, field_8AC, 2220);
RW_SIZE(RaidBossActor, 0x8B0);
