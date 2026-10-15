// EnemyActor: an enemy standing on the quest map and fighting in battle. A MapNode that owns the LWF character
// (Enemy, a FlashNode), the optional "show" effect (ShowEffect), the HP board above it (RaidBossHPIndicator), the
// tap marker / target selector armatures, and the prizes it drops (MapPrizes). Its battle values (HP, prizes, buffs
// and bad statuses) are kept in Property, a SafeBuffer that implements IStatusProtocol and IBuffProtocol; the
// static data comes from the master::Enemy record (masterEnemy).
#pragma once

#include "actor/MapNode.hpp"

struct EnemyActor : MapNode {
    // Battle life cycle (state): grows only (setState)
    enum State : int32_t { NONE = 0, MAP = 1, SYMBOL = 2, ENCOUNT = 3, DYING = 4, DEAD = 5, ENDED = 6 };

    // ---- data positions in Property (bytes into the SafeBuffer data, 0x368 bytes)
    static constexpr uint32_t P_HP_MIN = 0;          // HP floor (0): dead when HP reaches it
    static constexpr uint32_t P_HP_MAX = 4;
    static constexpr uint32_t P_HP = 8;
    static constexpr uint32_t P_ATTACK = 12;         // the level-scaled master values (7 of them, +12..+36)
    static constexpr uint32_t P_EXP = 20;
    static constexpr uint32_t P_MONEY = 24;
    static constexpr uint32_t P_RAID_POINT = 28;
    static constexpr uint32_t P_LUX = 32;
    static constexpr uint32_t P_DROP_ID = 36;        // EnemyActorInfo +16
    static constexpr uint32_t P_DROP_COUNT = 40;     // EnemyActorInfo +20
    static constexpr uint32_t P_TEAM = 44;           // EnemyActorInfo +12: encount team id
    static constexpr uint32_t P_CONDITIONS = 52;     // StatusChange[4]
    static constexpr uint32_t P_BUFFS = 100;         // BuffChange[64]
    static constexpr uint32_t P_DEFEATED = 868;      // 1 byte: the kill was counted

    // ---- master::Enemy data positions
    static constexpr uint32_t M_ID = 0;
    static constexpr uint32_t M_NAME = 4;            // char[65]
    static constexpr uint32_t M_CHARACTER = 584;     // LWF character id (Enemy::create)
    static constexpr uint32_t M_ATTACK_TYPE = 588;   // 1..3 -> attackType 0..2
    static constexpr uint32_t M_SHOW_EFFECT = 592;
    static constexpr uint32_t M_SHOW_PARAM1 = 596;
    static constexpr uint32_t M_SHOW_PARAM2 = 600;
    static constexpr uint32_t M_SPEED = 604;
    static constexpr uint32_t M_ENCOUNT_RANGE = 608;
    static constexpr uint32_t M_TOUCH_SIZE = 612;
    static constexpr uint32_t M_WIDTH = 616;
    static constexpr uint32_t M_HEIGHT = 620;
    static constexpr uint32_t M_HP_BOARD_POS = 824;
    static constexpr uint32_t M_ATTRIBUTE = 632;     // 1..3 (setAdvantageousEffect)
    static constexpr uint32_t M_HP_GAUGES = 632;     // also the HP board's gauge count
    static constexpr uint32_t M_LEVEL_COUNT = 636;
    static constexpr uint32_t M_LEVEL_TABLE = 640;   // 8 x int[5]: per-level values (level, hp, attack, ...)
    static constexpr uint32_t M_CRUSH_LUX = 804;
    static constexpr uint32_t M_ATTACK_LUX = 800;
    static constexpr uint32_t M_ATTACK_EXP = 808;
    static constexpr uint32_t M_CRUSH_EXP = 812;
    static constexpr uint32_t M_APPROACH = 820;      // 3 = never approaches
    static constexpr uint32_t M_ATTACK_MASK = 836;   // bit k: attack slot k exists
    static constexpr uint32_t M_ATTACKS = 840;       // int[4] master::EnemyAttack ids
    static constexpr uint32_t M_SKILL_KIND = 856;    // int[4] use condition per slot
    static constexpr uint32_t M_SKILL_PARAM = 872;   // int[4]
    static constexpr uint32_t M_SKILL_RATE = 888;    // int[4] chance (x/10000)

    rw::Field<uint8_t> field_288;          // +648
    rw::Field<uint8_t> isBoss;             // +649 symbol flag: boss board / x1100 HP
    rw::Field<uint8_t> boardHidden;        // +650 the HP board is not shown on the map
    rw::Field<uint8_t> symbolSet;          // +651
    rw::Field<uint8_t> inBattle;           // +652 encountered and alive
    rw::Field<uint8_t> field_28D;          // +653
    rw::Field<uint8_t> selectWait;         // +654 waiting to be chosen as a target
    const uint8_t pad_28F[1];              // +655
    rw::Field<int32_t> teamId;             // +656
    rw::Field<int32_t> teamIndex;          // +660
    rw::Field<uint32_t> statusBits;        // +664 the character's BadStatusBit set at the last UpdateStatus
    rw::Field<uint32_t> fieldNode;         // +668 cocos2d::Node* the prizes are added to
    rw::Field<int32_t> state;              // +672 State
    rw::Field<int32_t> showState;          // +676 1 shown, 3 hidden
    SafeBuffer property;                   // +680 EnemyActor::Property (vtable 0xFA8170)
    rw::Field<uint32_t> statusVtable;      // +712 IStatusProtocol base
    rw::Field<uint32_t> buffVtable;        // +716 IBuffProtocol base
    rw::Field<uint8_t> field_2D0;          // +720
    const uint8_t pad_2D1[3];
    rw::Field<uint32_t> field_2D4;         // +724
    rw::Field<uint32_t> masterEnemy;       // +728 shared_ptr<master::Enemy>
    rw::Field<uint32_t> masterEnemyCtrl;   // +732
    rw::Field<float> speed;                // +736 map move speed (master speed / 2)
    rw::Field<float> touchSize;            // +740
    rw::Field<float> encountRange;         // +744
    bt::Vec2 size;                         // +748 cocos2d::Size
    rw::Field<int32_t> symbolKind;         // +756
    rw::Field<int32_t> moveType;           // +760 1 = stays
    bt::Vec2 homePosition;                 // +764
    bt::Vec2 moveDir;                      // +772
    rw::Field<int32_t> wanderDir;          // +780 0..8 (direction table)
    rw::Field<float> moveTimer;            // +784
    rw::Field<uint8_t> field_310;          // +788
    rw::Field<uint8_t> fixedFacing;        // +789 never turns around
    const uint8_t pad_316[2];
    rw::Field<int32_t> attackSlot;         // +792 the attack chosen by select_enemy_attack
    rw::Field<int32_t> cooldown[4];        // +796 turns before slot k can be used again
    rw::Field<uint8_t> used[4];            // +812 once-only slot k was used
    rw::Field<uint32_t> flash;             // +816 Enemy*
    rw::Field<uint32_t> showEffect;        // +820 ShowEffect*
    rw::Field<int32_t> attackType;         // +824 0..2 (master value - 1); 2 = always visible
    rw::Field<int32_t> deadType;           // +828 0 hide when ended, 2 keep standing
    bt::Vec2 touchOrigin;                  // +832 cocos2d::Rect touch rect (origin, size)
    bt::Vec2 touchExtent;                  // +840
    rw::Field<uint8_t> touching;           // +848 a touch began inside
    const uint8_t pad_351[7];
    const uint8_t onTouch[24];             // +856 std::function
    const uint8_t onTapped[24];            // +880 std::function<bool(EnemyActor*)>
    const uint8_t onSelect[24];            // +904 std::function<void(EnemyActor*, bool)>
    const uint8_t canMoveTo[24];           // +928 std::function<bool(float, float)>
    rw::Field<uint32_t> tapMarker;         // +952 cocostudio::Armature* ActMapTapAnim
    rw::Field<uint32_t> selector;          // +956 cocostudio::Armature* ActMapTapAnim (target selector)
    rw::Field<uint32_t> hpBoard;           // +960 RaidBossHPIndicator*
    bt::Vec2 hpBoardPos;                   // +964
    rw::Field<uint8_t> field_3CC;          // +972
    const uint8_t pad_3CD[3];
    SafeBuffer prizeParam;                 // +976 (vtable 0xF96490): +0 attack prize rate, +4 crush rate (floats)
    rw::Field<uint32_t> skill;             // +1008 shared_ptr<const master::Skill> of the finishing medal
    rw::Field<uint32_t> skillCtrl;         // +1012
    rw::Field<uint32_t> dropItem;          // +1016 MapPrizes* erupting when killed
    rw::Field<uint32_t> crushPrize;        // +1020 MapPrizes* erupting when the death animation ends
    bt::Vector prizes;                     // +1024 cocos2d::Vector<MapPrizes*>
    rw::Field<uint32_t> overlay;           // +1036 cocos2d::Node* (z 100) for markers and the HP board

    static constexpr uint32_t VTABLE = 0xFA7EF8;
    static constexpr uint32_t PROPERTY_VTABLE = 0xFA8170;
    static constexpr uint32_t STATUS_VTABLE = 0xFA81A0;
    static constexpr uint32_t BUFF_VTABLE = 0xFA81BC;
    static constexpr uint32_t PRIZE_PARAM_VTABLE = 0xF96490;
    static constexpr uint32_t SIZE = 0x410;

    // virtual slots of EnemyActor (cocos2d::Node + MapNode)
    static constexpr uint32_t VSLOT_GET_POSITION = 21;

    uint32_t self() const { return rw::guest_addr(this); }
    int32_t prop32(uint32_t index) const { return property.get32(index); }
    int32_t master32(uint32_t index) const { return rw::host_ptr<SafeBuffer>(masterEnemy)->get32(index); }

    static EnemyActor *construct(EnemyActor *self, uint32_t info);
    static EnemyActor *destruct(EnemyActor *self);
    static void destructDelete(EnemyActor *self);
    static EnemyActor *create(uint32_t info, uint32_t fieldNode);
    static void set_buff_param(EnemyActor *self, int32_t atk0, int32_t def0, int32_t atk1, int32_t def1, int32_t atk2,
                               int32_t def2, int32_t atk3, int32_t def3, int32_t turns, int32_t count,
                               int32_t condition1, int32_t condition2, int32_t condTurns, int32_t condCount);
    static EnemyActor *initMapPosition(EnemyActor *self, const bt::Vec2 *pos);
    static void setupTouchRect(EnemyActor *self, float width, float height);
    static void actionMarker(EnemyActor *self, uint32_t action);
    static void actionSelector(EnemyActor *self, uint32_t on);
    static void setPrizeParam(EnemyActor *self, float attackRate, float crushRate, bt::SharedPtr *skill);
    static void setSymbolInfo(EnemyActor *self, int32_t kind, int32_t moveType, uint32_t boss, uint32_t big);
    static EnemyActor *setTeamInfo(EnemyActor *self, int32_t team, int32_t index);
    static void setStatusBoard(EnemyActor *self, int32_t a, int32_t b);
    static bool use_skill_check(EnemyActor *self, int32_t slot, float hpRate);
    static void use_skill(EnemyActor *self, int32_t slot);
    static int32_t select_enemy_attack(EnemyActor *self, float hpRate, uint32_t actorManager);
    static void getEnemyAttackMaster(bt::SharedPtr *out, const EnemyActor *self);
    static void changeMotion(EnemyActor *self, uint32_t motion);
    static void UpdateStatus(EnemyActor *self);
    static int32_t getHitEffectID(EnemyActor *self);
    static float getHitEffectDirection(EnemyActor *self);
    static bool addDamage(EnemyActor *self, uint32_t damage, const bt::SharedPtr *burst);
    static void addCrushingPrizes(EnemyActor *self, int32_t damage);
    static void addDropItem(EnemyActor *self);
    static bool setState(EnemyActor *self, int32_t s);
    static void addAttackPrizes(EnemyActor *self, int32_t damage, int32_t hits);
    static void preloadResources(EnemyActor *self);
    static void deploy(EnemyActor *self, uint32_t show);
    static bool setEncountAction(EnemyActor *self, int32_t team);
    static void setDeadAction(EnemyActor *self);
    static void setEndedAction(EnemyActor *self);
    static void getScreenTouchRect(const uint8_t *out, EnemyActor *self);
    static void getMapPosotionTouchRect(const uint8_t *out, EnemyActor *self);
    static EnemyActor *iniApproachMove(EnemyActor *self);
    static bool setApproachMove(EnemyActor *self, const bt::Vec2 *dir, uint32_t force);
    static bool exeApproachMove(EnemyActor *self, float dt, const bt::Vec2 *target, uint32_t force);
    static void lookAt(EnemyActor *self, const bt::Vec2 *target);
    static void moveWandering(EnemyActor *self, float dt);
    static void update(EnemyActor *self, float dt);
    static void updateGauge(EnemyActor *self, float dt);
    static void setVisible(EnemyActor *self, uint32_t visible);
    static void setScaleX(EnemyActor *self, float sx);
    static void setAdvantageousEffect(EnemyActor *self, int32_t attribute);
    static void addPrizes(EnemyActor *self, uint32_t prizes, uint32_t onDeath);
    static void setDropItem(EnemyActor *self, uint32_t prizes);
    static void addCrushingMoneyPrizes(EnemyActor *self, int32_t kind);
    static bool isEncount(EnemyActor *self, const bt::Vec2 *pos, float range);
    static bool isMarking(EnemyActor *self);
    static bool isDied(const EnemyActor *self);
    static void setupSelectWait(EnemyActor *self, uint32_t callback);
    static void startSelectWait(EnemyActor *self);
    static EnemyActor *ceaseSelectWait(EnemyActor *self);
    static void endedSelectWait(EnemyActor *self);
    static bool isSetSelectWait(EnemyActor *self);
    static void pause(EnemyActor *self);
    static void resume(EnemyActor *self);
};
RW_LAYOUT(EnemyActor, isBoss, 649);
RW_LAYOUT(EnemyActor, teamId, 656);
RW_LAYOUT(EnemyActor, state, 672);
RW_LAYOUT(EnemyActor, property, 680);
RW_LAYOUT(EnemyActor, statusVtable, 712);
RW_LAYOUT(EnemyActor, masterEnemy, 728);
RW_LAYOUT(EnemyActor, size, 748);
RW_LAYOUT(EnemyActor, homePosition, 764);
RW_LAYOUT(EnemyActor, moveTimer, 784);
RW_LAYOUT(EnemyActor, attackSlot, 792);
RW_LAYOUT(EnemyActor, used, 812);
RW_LAYOUT(EnemyActor, flash, 816);
RW_LAYOUT(EnemyActor, touchOrigin, 832);
RW_LAYOUT(EnemyActor, touching, 848);
RW_LAYOUT(EnemyActor, onTouch, 856);
RW_LAYOUT(EnemyActor, canMoveTo, 928);
RW_LAYOUT(EnemyActor, tapMarker, 952);
RW_LAYOUT(EnemyActor, hpBoardPos, 964);
RW_LAYOUT(EnemyActor, prizeParam, 976);
RW_LAYOUT(EnemyActor, skill, 1008);
RW_LAYOUT(EnemyActor, prizes, 1024);
RW_LAYOUT(EnemyActor, overlay, 1036);
RW_SIZE(EnemyActor, 0x410);

// EnemyActor::Property: the SafeBuffer part (+0), IStatusProtocol (+32) and IBuffProtocol (+36) bases.
namespace enemy_actor {
struct Property : SafeBuffer {
    rw::Field<uint32_t> statusVtable;   // +32
    rw::Field<uint32_t> buffVtable;     // +36

    static void getCondition(const uint8_t *out, const Property *self, int32_t type);
    static void setCondition(Property *self, int32_t type, const uint8_t *change);
    static void addCondition(Property *self, int32_t type, int32_t turns, int32_t count);
    static void getBuff(const uint8_t *out, const Property *self, int32_t slot);
    static void addBuff(Property *self, const uint8_t *change);
    static void setBuff(Property *self, int32_t slot, const uint8_t *change);
    static int32_t getMaxHP(const Property *self);
    static void heal(Property *self, int32_t amount);
    static void destructDelete(Property *self);
};
RW_LAYOUT(Property, buffVtable, 36);
}  // namespace enemy_actor
