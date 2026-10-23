// RaidBattle: one attack exchange of a raid battle (a cocos2d::Node, 0x330 bytes, scheduled while it runs). The
// player's (or the friend's) avatar runs at the boss, the attack is computed (BattleMisc) and shown part by part,
// the boss's bad statuses and buffs tick, the boss may answer with one of its skills, the camera follows. The
// per-frame state machine is RaidBattle::update, at the end of this file.
#include "RaidBattle.hpp"

#include <cmath>

using namespace raid_battle;

namespace {
// ---- guest functions
constexpr uint32_t Node_ctor = 0x892531;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t aeabi_memset = 0x40E7A0;            // __aeabi_memset(dst, n, c)
constexpr uint32_t Vec2_subtract = 0x8E530F;
constexpr uint32_t Vec2_scale = 0x8E50D5;
constexpr uint32_t Vec2_distance = 0x8E4EF5;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t Director_getVisibleSize = 0x8F0675;
constexpr uint32_t ScaleTo_create = 0x86A8CD;           // (duration, scale)
constexpr uint32_t MoveTo_create = 0x86941D;            // (duration, const Vec2&)
constexpr uint32_t JumpTo_create = 0x869F39;            // (duration, const Vec2&, height, jumps)
constexpr uint32_t EaseSineInOut_create = 0x860E0D;
constexpr uint32_t Spawn_create = 0x8682F5;
constexpr uint32_t Sequence_create = 0x867999;
constexpr uint32_t CallFunc_create = 0x866B39;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_generateUniformInt = 0x492225;   // (random, min, max, state*)
constexpr uint32_t master_Badstatus_create = 0x779525;
constexpr uint32_t master_Burst_create = 0x77A491;
constexpr uint32_t master_RaidEnemy_create = 0x787745;
constexpr uint32_t shared_addRef = 0xC8DBB5;
constexpr uint32_t shared_release = 0xC8DBD1;
constexpr uint32_t StageState_addDamage = 0x4EEB89;
constexpr uint32_t StatusChange_clear = 0x4F1D15;
constexpr uint32_t IBuffProtocol_elapseCountAttackBuff = 0x4F221D;
constexpr uint32_t IBuffProtocol_elapseCountDefenseBuff = 0x4F229D;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;
constexpr uint32_t audio_Layer_playSE = 0x5BF785;
constexpr uint32_t FlashNode_set_bad_status = 0x5C2D61;
constexpr uint32_t FlashNode_reflection_status = 0x5C3171;
constexpr uint32_t Avatar_activeBurst = 0x5C62D5;
constexpr uint32_t Avatar_changeMotion = 0x5C63C5;
constexpr uint32_t MissBoard_disp_miss = 0x5D4171;      // (node, Vec2 by value, float, float)
constexpr uint32_t RaidBoss_changeMotion = 0x5D6815;
constexpr uint32_t MedalDeck_getCurrentMedalInfo = 0x5DFF6B;
constexpr uint32_t RaidBossStatusIndicator_setRaidBossHP = 0x5E9191;
constexpr uint32_t DisplayDamageNumber_disp_dmgNum = 0x5EB45D;
constexpr uint32_t RaidBattle_RaidDamageSub = 0x5F0A5D;
constexpr uint32_t markers_find = 0x5F1251;
constexpr uint32_t RaidBossActor_addDamage = 0x5F2D21;
constexpr uint32_t RaidBossActor_setEnemyDamage = 0x5F2FE1;
constexpr uint32_t RaidBossActor_getCurrentHP = 0x5F2ECD;
constexpr uint32_t RaidBossActor_getMaxHP = 0x5F3B05;
constexpr uint32_t RaidBossActor_makeWholeAttackPoint = 0x5F3B65;
constexpr uint32_t RaidBossActor_SelectedTarget = 0x5F36A9;
constexpr uint32_t RaidBossActor_ShowTargetInex = 0x5F36E9;
constexpr uint32_t RaidBossActor_HideTargetInex = 0x5F3725;
constexpr uint32_t RaidBossActor_HideTargetNoSelected = 0x5F377D;
constexpr uint32_t RaidBossActor_find_marker = 0x5F3D79;
constexpr uint32_t RaidBossActor_MinHPRateRaidTeam = 0x5F4A59;
constexpr uint32_t RaidBossActor_setAttackDamage = 0x5F4D7D;   // (actor, part, damage, const bitset<3>*)
constexpr uint32_t RaidBossActor_Property_addCondition = 0x5F65BD;
constexpr uint32_t RaidBossActor_elapsePartsCount = 0x5F6B39;
constexpr uint32_t RaidBossActor_UpdateStatus = 0x5F6BD9;
constexpr uint32_t RaidDirection_raid_fever = 0x5F7E69;
constexpr uint32_t RaidDirection_to_end = 0x5F93CD;
constexpr uint32_t SceneBattleBase_invokeAttackSkill = 0x5FA859;
constexpr uint32_t SceneRaidBattle_to_slow = 0x608725;
constexpr uint32_t SceneRaidBattle_getCameraPosition = 0x609B45;
constexpr uint32_t SceneRaidBattle_alterCamera = 0x609B4D;
constexpr uint32_t AvatarActor_UpdateStatus = 0x7403A9;
constexpr uint32_t AvatarActor_choiceChainType = 0x740945;
constexpr uint32_t AvatarActor_setChainAttackMotion = 0x740A9D;
constexpr uint32_t BattleMisc_getInstance = 0x7420A9;
constexpr uint32_t BattleMisc_preparePlayerAttackRaid = 0x7441F5;
constexpr uint32_t BattleMisc_prepareRaidAttack = 0x74612D;
constexpr uint32_t BattleMisc_calculatePlayerAttack = 0x747EE5;
constexpr uint32_t StageActorManager_autoChoiceTarget = 0x74FB81;
constexpr uint32_t StageActorManager_SetFriendBurst = 0x751F51;
constexpr uint32_t StageUI_getMedalField = 0x75E65D;
constexpr uint32_t GuiltBurstCutInShader_setShaderConcentrationStart = 0x496215;

constexpr uint32_t GOT_VTABLE = 0xFD6830;               // _ZTV10RaidBattle
constexpr uint32_t VT_GO_BACK_DONE = 0xF5DE38;          // go_back $_: CallFunc {this}
constexpr uint32_t VT_END_SEQUENCE = 0xF5DE0C;          // RaidBossEndSequence $_: CallFunc {this} (unused)
constexpr uint32_t SE_RAID_END = 0x1E56BB52;

// ---- virtual slots
constexpr uint32_t V_DELETE = 0x04;
constexpr uint32_t V_SET_SCALE = 0x40;
constexpr uint32_t V_GET_SCALE = 0x44;
constexpr uint32_t V_SET_POSITION = 0x4C;
constexpr uint32_t V_GET_POSITION = 0x54;
constexpr uint32_t V_INIT = 0x248;

// ---- other classes' fields
constexpr uint32_t SCENE_STAGE_UI = 696;                // SceneRaidBattle: StageUI*
constexpr uint32_t SCENE_ACTOR_MANAGER = 700;           // SceneRaidBattle: StageActorManager*
constexpr uint32_t SCENE_STATE_736 = 736;
constexpr uint32_t SCENE_PLAYER = 768;                  // SceneRaidBattle: the player's AvatarActor (camera reference)
constexpr uint32_t SCENE_TARGET_BYTE = 845;
constexpr uint32_t FIELD_NODE = 628;                    // the object at RaidBattle +648: its damage-number node
constexpr uint32_t ACTOR_AVATAR = 648;                  // AvatarActor: Avatar*
constexpr uint32_t AVATAR_BURST_READY = 0x534;          // Avatar: burst gauge full
constexpr uint32_t SAM_FRIEND_BURST = 100;              // StageActorManager: the friend's burst is on
constexpr uint32_t MEDALFIELD_CHAIN_A = 864, MEDALFIELD_CHAIN_B = 868, MEDALFIELD_CHAIN_C = 880;
constexpr uint32_t BOSS_STATUS = 680;                   // RaidBossActor: IStatusProtocol
constexpr uint32_t BOSS_BUFFS = 684;                    // RaidBossActor: IBuffProtocol
constexpr uint32_t BOSS_PROPERTY = 648;
constexpr uint32_t BOSS_TARGET = 920;
constexpr uint32_t BOSS_TARGET_924 = 924;
constexpr uint32_t BOSS_PART_HIT = 928;
constexpr uint32_t BOSS_INDICATOR = 936;                // RaidBossStatusIndicator*
constexpr uint32_t BOSS_MARKERS = 2188;
constexpr uint32_t BOSS_MARKER_FIRST = 2196;
constexpr uint32_t BOSS_RAIDBOSS = 2208;
constexpr uint32_t BOSS_ENEMY_ID = 2212;
constexpr uint32_t BOSS_SLEPT = 2220;                   // byte: put to sleep by this attack
constexpr uint32_t MEDALINFO_FLAGS = 8;                 // MedalDeck::MedalInfo: a SafeBuffer (data +20)
constexpr uint32_t BATTLEMISC_FLAGS = 0xA0;             // BattleMisc: std::bitset<3> of the last attack
constexpr uint32_t STAGESTATE_STATUS = 32;              // StageState: IStatusProtocol
constexpr uint32_t STAGESTATE_BUFFS = 36;               // StageState: IBuffProtocol
constexpr uint32_t STAGESTATE_PENDING_DAMAGE = 0x4248;
// master data positions
constexpr uint32_t RE_WEAKNESS = 144, RE_VALID_PARTS = 148, RE_PARTS = 152, RE_SKILL_COUNT = 540;
constexpr uint32_t RE_SKILL_TYPE = 560, RE_SKILL_VALUE = 576, RE_SKILL_RATE = 592;
constexpr uint32_t RE_PROTECT_POISON = 608, RE_PROTECT_DEEP_POISON = 612, RE_PROTECT_SLEEP = 616;
constexpr uint32_t RE_PROTECT_PARALYSIS = 620, RE_REGIST_POISON = 628, RE_REGIST_DEEP_POISON = 632;
constexpr uint32_t RE_REGIST_SLEEP = 636, RE_REGIST_PARALYSIS = 640;
constexpr uint32_t MEDAL_BURST_SINGLE = 0x4A8, MEDAL_BURST_ID = 0x4AC;
constexpr uint32_t BURST_TARGET_ALL = 0x160;
constexpr uint32_t SKILL_POISON = 268, SKILL_DEEP_POISON = 272, SKILL_SLEEP = 276, SKILL_PARALYSIS = 280;
constexpr uint32_t SKILL_TURNS = 284, SKILL_COUNT = 288;
constexpr uint32_t BADSTATUS_DAMAGE = 0x54;             // Badstatus: {base, range} (x 10000 of max HP)

uint32_t fb(float f) { return rw::fbits(f); }
uint32_t sbGet(uint32_t sbAddr, uint32_t index) { return uint32_t(rw::host_ptr<SafeBuffer>(sbAddr)->get32(index)); }
uint32_t enemyCreate(uint32_t at, uint32_t id) { call(master_RaidEnemy_create, {at, id}); return ld32(at); }
void release(uint32_t ctrl) { if (ctrl) call(shared_release, {ctrl}); }
uint32_t randomInt(int32_t lo, int32_t hi) {
    uint32_t random = call(Random_getInstance, {});
    return call(Random_generateUniformInt, {random, uint32_t(lo), uint32_t(hi), 0});
}
uint32_t boss(uint32_t self) { return ld32(self + BOSS); }
uint32_t scene_(uint32_t self) { return ld32(self + SCENE); }
uint32_t avatar(uint32_t self) { return ld32(self + AVATAR); }
uint32_t fieldNode(uint32_t self) { return ld32(ld32(self + FIELD_OWNER) + FIELD_NODE); }
uint32_t getPosition(uint32_t node) { return vcall(node, V_GET_POSITION, {node}); }
// RaidBoss motion + the attack point over the live parts
void bossMotion(uint32_t b, uint32_t motion) {
    call(RaidBoss_changeMotion, {ld32(b + BOSS_RAIDBOSS), motion});
    call(RaidBossActor_makeWholeAttackPoint, {b});
}
// The boss's condition `type`: IStatusProtocol slot 0 into `out` (12 bytes); its count
uint32_t bossCondition(uint32_t b, uint32_t out, uint32_t type) {
    uint32_t proto = b + BOSS_STATUS;
    call(ld32(ld32(proto)), {out, proto, type});
    return ld32(out + 8);
}

// start / startFriend: the attacker, the boss, the camera node, the scene, where the attack goes, how it targets
void begin(uint32_t self, uint32_t attacker, uint32_t b, uint32_t camera, uint32_t scene, uint32_t target,
           uint32_t type, uint32_t state) {
    st32(self + AVATAR, attacker);
    st32(self + BOSS, b);
    st32(self + CAMERA_NODE, camera);
    st32(self + SCENE, scene);
    uint32_t tx = ld32(target), ty = ld32(target + 4);
    st32(self + TARGET_POS + 4, ty);
    st32(self + TARGET_POS, tx);
    st32(self + TARGET_TYPE, type);
    uint32_t pos = getPosition(attacker);
    uint32_t px = ld32(pos), py = ld32(pos + 4);
    st32(self + AVATAR_POS + 4, py);
    st32(self + AVATAR_POS, px);
    uint32_t bossPos = getPosition(b);
    Frame f;
    uint32_t d = f.alloc(8);
    st32(d, ld32(self + TARGET_POS));
    st32(d + 4, ld32(self + TARGET_POS + 4));
    call(Vec2_subtract, {d, bossPos});
    st32(self + OFFSET + 4, ld32(d + 4));
    st32(self + OFFSET, ld32(d));
    st32(self + SIDE, std::fabs(ldf(self + OFFSET + 4)) + -150.0f > 0.0f ? 2u : 1u);
    st32(self + STATE, state);
    st32(scene_(self) + SCENE_STATE_736, 14);
    call(Node_scheduleUpdate, {self});
    int32_t t = int32_t(ld32(boss(self) + BOSS_TARGET_924));
    if (t >= 0) call(RaidBossActor_HideTargetNoSelected, {boss(self), uint32_t(t)});
}

// One bad status the attack's skill may inflict: the RaidEnemy's protect flag blocks it, else its resistance
// (x 10000) against a draw of 0..9999; a miss shows "Miss" at the target.
void tryCondition(uint32_t self, uint32_t skill, uint32_t b, uint32_t enemy, uint32_t pos, uint32_t skillField,
                  uint32_t protect, uint32_t regist, uint32_t type, uint32_t badStatus, bool sleep) {
    if (!sbGet(skill, skillField)) return;
    bool hit = false;
    if (!sbGet(enemy, protect)) {
        int32_t chance = int32_t(sbGet(enemy, regist));
        hit = chance > int32_t(randomInt(0, 9999));
    }
    if (hit) {
        uint32_t turns = sbGet(skill, SKILL_TURNS);
        uint32_t count = sbGet(skill, SKILL_COUNT);
        call(RaidBossActor_Property_addCondition, {b + BOSS_PROPERTY, type, turns, count});
        call(FlashNode_set_bad_status, {ld32(b + BOSS_RAIDBOSS), badStatus});
        if (sleep) scene::st8(b + BOSS_SLEPT, 1);
    } else {
        Frame f;
        uint32_t copy = f.alloc(8);
        st32(copy, ld32(pos));
        st32(copy + 4, ld32(pos + 4));
        call(MissBoard_disp_miss, {fieldNode(self), copy, fb(-64.0f), fb(1.0f)});
    }
}

// The attack's damage on `part` (BattleMisc), shown on the boss; the part's marker shown while it was hit.
void partDamage(uint32_t self, uint32_t part, bool marker) {
    uint32_t bm = call(BattleMisc_getInstance, {});
    uint32_t damage = call(BattleMisc_calculatePlayerAttack, {bm, self + TARGET_TYPE, boss(self), part});
    uint32_t flags = call(BattleMisc_getInstance, {}) + BATTLEMISC_FLAGS;
    call(RaidBossActor_setAttackDamage, {boss(self), part, damage, flags});
    call(RaidBossActor_UpdateStatus, {boss(self)});
    if (!marker) return;
    uint32_t b = boss(self);
    if (scene::ld8(b + part + BOSS_PART_HIT)) call(RaidBossActor_ShowTargetInex, {b, part});
    else call(RaidBossActor_HideTargetInex, {b, part});
}

// Poison / deep poison damage at the start of the boss's turn: the Badstatus row's rate (with a random spread) of
// the body's max HP, at least 1; the number shows at the body marker.
uint32_t poisonDamage(uint32_t self, uint32_t marker, uint32_t hp, uint32_t badstatusId) {
    Frame f;
    uint32_t row = f.alloc(8), empty = f.alloc(8), pos = f.alloc(8);
    call(master_Badstatus_create, {row, badstatusId});
    uint32_t base = sbGet(ld32(row), BADSTATUS_DAMAGE);
    int32_t range = int32_t(sbGet(ld32(row), BADSTATUS_DAMAGE + 4));
    uint32_t r = randomInt(-(range / 2), range / 2);
    uint32_t b = boss(self);
    uint32_t v = (r + base) * call(RaidBossActor_getMaxHP, {b, 0});
    uint32_t damage = v >= 10000u ? v / 10000u : 1u;
    b = boss(self);
    if (hp <= damage) {
        bossMotion(b, 3);
        RaidBattle::RaidBossEndSequence(self);
    } else {
        bossMotion(b, 2);
    }
    uint32_t indicator = ld32(boss(self) + BOSS_INDICATOR);
    st32(empty, 0);
    st32(empty + 4, 0);
    uint32_t left = call(RaidBossActor_addDamage, {boss(self), 0, damage, empty});
    call(RaidBossStatusIndicator_setRaidBossHP, {indicator, left});
    release(ld32(empty + 4));
    call(FlashNode_reflection_status, {ld32(boss(self) + BOSS_RAIDBOSS), 1});
    uint32_t node = fieldNode(self);
    uint32_t m = ld32(marker + 12);
    uint32_t p = getPosition(m);
    st32(pos, ld32(p));
    st32(pos + 4, ld32(p + 4));
    call(DisplayDamageNumber_disp_dmgNum, {damage, node, pos, fb(-64.0f), 0, 0, 1, 0, 600, fb(1.0f)});
    release(ld32(row + 4));
    return 1;
}
}  // namespace

uint32_t RaidBattle::construct(uint32_t self) {
    call(Node_ctor, {self});
    st32(self, ld32(GOT_VTABLE) + 8);
    st32(self + SCENE, 0);
    st32(self + AVATAR, 0);
    st32(self + BOSS, 0);
    st32(self + STATE, 0);
    st32(self + OFFSET, 0);
    st32(self + OFFSET + 4, 0);
    st32(self + F_676, 1);
    st32(self + AVATAR_POS, 0);
    st32(self + AVATAR_POS + 4, 0);
    st32(self + F_716, 0);
    st32(self + F_720, 0);
    st32(self + F_724, 0);
    st32(self + F_732, fb(1.0f));
    st32(self + F_736, fb(1.0f));
    st32(self + F_740, 0);
    st32(self + F_748, 0);
    st32(self + TARGET_POS, 0);
    st32(self + TARGET_POS + 4, 0);
    st32(self + SHADER, 0);
    call(aeabi_memset, {self + TARGET_TYPE, 0x21, 0});
    for (uint32_t k = 0; k < 4; k++) {
        st32(self + COOLDOWN + 4 * k, 0);
        scene::st8(self + SKILL_USED + k, 0);
    }
    return self;
}
RW_FUNCTION(0x5EDE44, "RaidBattle::RaidBattle", RaidBattle::construct);

uint32_t RaidBattle::create() {
    uint32_t self = call(scene::fn::operator_new, {SIZE});
    construct(self);
    if (vcall(self, V_INIT, {self}) == 1) {
        call(Ref_autorelease, {self});
        return self;
    }
    vcall(self, V_DELETE, {self});
    return 0;
}
RW_FUNCTION(0x60867C, "RaidBattle::create", RaidBattle::create);

void RaidBattle::start(uint32_t self, uint32_t attacker, uint32_t b, uint32_t camera, uint32_t scene,
                       uint32_t target, uint32_t type) {
    begin(self, attacker, b, camera, scene, target, type, 11);
}
RW_FUNCTION(0x5EDF50, "RaidBattle::start", RaidBattle::start);

void RaidBattle::startFriend(uint32_t self, uint32_t attacker, uint32_t b, uint32_t camera, uint32_t scene,
                             uint32_t target, uint32_t type) {
    begin(self, attacker, b, camera, scene, target, type, 10);
}
RW_FUNCTION(0x5EE0AC, "RaidBattle::startFriend", RaidBattle::startFriend);

// The attack goes for the body (a burst that hits all parts targets nothing), the body marker selected; the
// offset to the body marker is where the avatar runs.
void RaidBattle::set_week_position(uint32_t self) {
    st32(boss(self) + BOSS_TARGET_924, 0);
    if (ld32(self + TARGET_TYPE) == 3) {
        uint32_t field = call(StageUI_getMedalField, {ld32(scene_(self) + SCENE_STAGE_UI)});
        uint32_t info = call(MedalDeck_getCurrentMedalInfo, {field});
        st32(self + MEDAL_INFO, info);
        Frame f;
        uint32_t burst = f.alloc(8);
        call(master_Burst_create, {burst, sbGet(ld32(info), MEDAL_BURST_ID)});
        if (sbGet(ld32(burst), BURST_TARGET_ALL) == 1) st32(boss(self) + BOSS_TARGET, 0);
        release(ld32(burst + 4));
    }
    st32(self + STATE, 13);
    call(RaidBossActor_SelectedTarget, {boss(self)});
    call(RaidBossActor_HideTargetNoSelected, {boss(self), 0});
    scene::st8(scene_(self) + SCENE_TARGET_BYTE, 0);
    Frame f;
    uint32_t marker = f.alloc(8), d = f.alloc(8);
    call(RaidBossActor_find_marker, {marker, boss(self), 0});
    uint32_t bossPos = getPosition(boss(self));
    st32(d, ld32(marker));
    st32(d + 4, ld32(marker + 4));
    call(Vec2_subtract, {d, bossPos});
    st32(self + OFFSET + 4, ld32(d + 4));
    st32(self + OFFSET, ld32(d));
}
RW_FUNCTION(0x5EE21C, "RaidBattle::set_week_position", RaidBattle::set_week_position);

// A chain attack goes on unless the player sleeps; while paralysed, only when the Badstatus 121 rate (x 10000)
// is below a draw of 1..10000.
uint32_t RaidBattle::ChainAttack(uint32_t) {
    Frame f;
    uint32_t out = f.alloc(16), row = f.alloc(8);
    uint32_t state = ld32(call(AppState_getInstance, {}) + 32);
    call(ld32(ld32(state + STAGESTATE_STATUS)), {out, state + STAGESTATE_STATUS, 1});
    if (ld32(out + 8)) return 0;
    state = ld32(call(AppState_getInstance, {}) + 32);
    call(ld32(ld32(state + STAGESTATE_STATUS)), {out, state + STAGESTATE_STATUS, 2});
    if (!ld32(out + 8)) return 1;
    call(master_Badstatus_create, {row, 121});
    int32_t rate = int32_t(sbGet(ld32(row), BADSTATUS_DAMAGE));
    int32_t r = int32_t(randomInt(1, 10000));
    release(ld32(row + 4));
    return rate < r ? 1u : 0u;
}
RW_FUNCTION(0x5EE384, "RaidBattle::ChainAttack", RaidBattle::ChainAttack);

// Pan / zoom to `target` (its y held within half the screen height): the camera, and with `withField` the
// damage-number node too (put on the player first).
void RaidBattle::moveCamera(uint32_t self, float seconds, uint32_t target, float scale, uint32_t withField) {
    Frame f;
    uint32_t visible = f.alloc(8), cam = f.alloc(8), v = f.alloc(8), t = f.alloc(8), d = f.alloc(8);
    call(Director_getVisibleSize, {visible, call(Director_getInstance, {})});
    uint32_t camPos = call(SceneRaidBattle_getCameraPosition, {scene_(self)});
    st32(cam, ld32(camPos));
    st32(cam + 4, ld32(camPos + 4));
    st32(v, ld32(target));
    float h = ldf(visible + 4), ty = ldf(target + 4);
    float lo = h * -0.5f, hi = h * 0.5f;
    st32(v + 4, fb(lo > ty ? lo : hi < ty ? hi : ty));
    st32(t, ld32(v));
    st32(t + 4, ld32(v + 4));
    call(Vec2_scale, {t, fb(scale)});
    st32(d, ld32(cam));
    st32(d + 4, ld32(cam + 4));
    call(Vec2_subtract, {d, t});
    if (withField == 1) {
        uint32_t zoom = call(EaseSineInOut_create, {call(ScaleTo_create, {fb(seconds), fb(scale)})});
        uint32_t move = call(EaseSineInOut_create, {call(MoveTo_create, {fb(seconds), d})});
        uint32_t node = fieldNode(self);
        uint32_t player = ld32(scene_(self) + SCENE_PLAYER);
        vcall(node, V_SET_POSITION, {node, getPosition(player)});
        node = fieldNode(self);
        player = ld32(scene_(self) + SCENE_PLAYER);
        vcall(node, V_SET_SCALE, {node, vcall(player, V_GET_SCALE, {player})});
        call(Node_runAction, {fieldNode(self), call(Spawn_create, {zoom, move, 0})});
    }
    uint32_t zoom = call(EaseSineInOut_create, {call(ScaleTo_create, {fb(seconds), fb(scale)})});
    uint32_t move = call(EaseSineInOut_create, {call(MoveTo_create, {fb(seconds), d})});
    call(SceneRaidBattle_alterCamera, {scene_(self), call(Spawn_create, {zoom, move, 0})});
}
RW_FUNCTION(0x5EF750, "RaidBattle::moveCamera", RaidBattle::moveCamera);

// The attacker strikes: a chain attack, a burst (the friend's, or the player's own with its auto target) or a
// plain attack; then the damage is worked out.
void RaidBattle::avatarAttack(uint32_t self) {
    moveCamera(self, rw::bitsf(0x3D888889), getPosition(avatar(self)), 1.0f, 1);
    st32(self + F_676, 1);
    uint32_t avatarObj = ld32(avatar(self) + ACTOR_AVATAR);
    uint32_t field = call(StageUI_getMedalField, {ld32(scene_(self) + SCENE_STAGE_UI)});
    uint32_t info = call(MedalDeck_getCurrentMedalInfo, {field});
    st32(self + MEDAL_INFO, info);
    uint32_t type = ld32(self + TARGET_TYPE);
    if (type == 1) {
        uint32_t chain = ld32(field + MEDALFIELD_CHAIN_B) + ld32(field + MEDALFIELD_CHAIN_A) - ld32(field + MEDALFIELD_CHAIN_C);
        call(AvatarActor_choiceChainType, {avatar(self), boss(self), chain});
        call(AvatarActor_setChainAttackMotion, {avatar(self)});
    } else if (type == 2 || !ld32(avatarObj + AVATAR_BURST_READY)) {
        call(Avatar_changeMotion, {ld32(avatar(self) + ACTOR_AVATAR), 2});
    } else {
        uint32_t sam = ld32(scene_(self) + SCENE_ACTOR_MANAGER);
        if (scene::ld8(sam + SAM_FRIEND_BURST)) {
            call(StageActorManager_SetFriendBurst, {sam});
        } else {
            uint32_t single = sbGet(ld32(info), MEDAL_BURST_SINGLE);
            uint32_t a = avatar(self);
            uint32_t burst = sbGet(ld32(info), MEDAL_BURST_ID);
            call(Avatar_activeBurst, {ld32(a + ACTOR_AVATAR), burst});
            call(Avatar_changeMotion, {ld32(avatar(self) + ACTOR_AVATAR), 47});
            call(StageActorManager_autoChoiceTarget, {ld32(scene_(self) + SCENE_ACTOR_MANAGER), info, single ? 1u : 0u});
        }
        if (uint32_t shader = ld32(self + SHADER)) call(GuiltBurstCutInShader_setShaderConcentrationStart, {shader});
    }
    resetAttackDamage(self);
}
RW_FUNCTION(0x5EF984, "RaidBattle::avatarAttack", RaidBattle::avatarAttack);

// The damage lands: on the targeted part (or each hit part); a part crushed shows the boss's crush motion and
// stops there. Then the boss takes the attack's damage (a burst's shared_ptr<Burst> and its critical flag), which
// raises the fever.
void RaidBattle::RaidDamage(uint32_t self) {
    int32_t t = int32_t(ld32(boss(self) + BOSS_TARGET));
    int32_t first = 0, end = 6;
    if (t >= 0) {
        scene::st8(boss(self) + uint32_t(t) + BOSS_PART_HIT, 1);
        end = t + 1;
        first = t;
    }
    for (int32_t k = first; k < end; k++) {
        if (!scene::ld8(boss(self) + uint32_t(k) + BOSS_PART_HIT)) continue;
        uint32_t crushed = call(RaidBattle_RaidDamageSub, {self, uint32_t(k)});
        uint32_t b = boss(self);
        if (crushed == 1) { bossMotion(b, 3); break; }
        bossMotion(b, 2);
    }
    Frame f;
    uint32_t arg = f.alloc(8), burst = f.alloc(8);
    uint32_t critical = 0, own = 0;
    bool keep = true;
    if (ld32(self + TARGET_TYPE) == 3) {
        uint32_t info = ld32(self + MEDAL_INFO);
        call(master_Burst_create, {burst, sbGet(ld32(info), MEDAL_BURST_ID)});
        uint32_t obj = ld32(burst), ctrl = ld32(burst + 4);
        st32(burst, 0);
        st32(burst + 4, 0);
        critical = sbGet(info + MEDALINFO_FLAGS, 20) ? 1u : 0u;
        st32(arg, obj);
        st32(arg + 4, ctrl);
        own = ctrl;
        if (ctrl) { call(shared_addRef, {ctrl}); keep = false; }
    } else {
        st32(arg, 0);
        st32(arg + 4, 0);
    }
    uint32_t damage = call(RaidBossActor_setEnemyDamage, {boss(self), arg, critical});
    release(ld32(arg + 4));
    if (damage) {
        st32(self + FEVER, ld32(self + FEVER) + damage);
        call(RaidDirection_raid_fever, {ld32(self + DIRECTION), ld32(self + FEVER)});
    }
    if (!keep) call(shared_release, {own});
}
RW_FUNCTION(0x5EFB64, "RaidBattle::RaidDamage", RaidBattle::RaidDamage);

// After the attack: the boss's condition counts tick (only the targeted part's, or all parts'), its sleep ends
// unless this attack caused it, the defence buffs tick; every marker's shown damage resets; the player's attack
// buffs tick; damage the player took meanwhile is applied.
void RaidBattle::reflectionCondition(uint32_t self) {
    call(RaidBossActor_UpdateStatus, {boss(self)});
    int32_t t = int32_t(ld32(boss(self) + BOSS_TARGET));
    Frame f;
    uint32_t enemy = f.alloc(8), part = f.alloc(8), out = f.alloc(16), zero = f.alloc(8);
    if (t > 0) {
        uint32_t e = enemyCreate(enemy, ld32(boss(self) + BOSS_ENEMY_ID));
        uint32_t pe = enemyCreate(part, sbGet(e, RE_PARTS + 4 * uint32_t(t - 1)));
        call(RaidBossActor_elapsePartsCount, {boss(self), sbGet(pe, RE_WEAKNESS) - 1});
        release(ld32(part + 4));
        release(ld32(enemy + 4));
    } else {
        bool all = t < 0;
        if (all) {
            uint32_t e = enemyCreate(enemy, ld32(boss(self) + BOSS_ENEMY_ID));
            int32_t n = int32_t(sbGet(e, RE_VALID_PARTS));
            for (int32_t k = 0; k < n; k++) {
                uint32_t pe = enemyCreate(part, sbGet(ld32(enemy), RE_PARTS + 4 * uint32_t(k)));
                call(RaidBossActor_elapsePartsCount, {boss(self), sbGet(pe, RE_WEAKNESS) - 1});
                release(ld32(part + 4));
            }
        }
        uint32_t b = boss(self);
        if (!scene::ld8(b + BOSS_SLEPT)) {
            uint32_t proto = b + BOSS_STATUS;
            uint32_t set = ld32(ld32(proto) + 4);
            call(ld32(ld32(proto)), {out, proto, 1});
            uint32_t cleared = call(StatusChange_clear, {out});
            call(set, {proto, 1, cleared});
        }
        call(IBuffProtocol_elapseCountDefenseBuff, {boss(self) + BOSS_BUFFS});
        call(FlashNode_reflection_status, {ld32(boss(self) + BOSS_RAIDBOSS), 1});
        if (all) release(ld32(enemy + 4));
    }
    for (uint32_t node = ld32(boss(self) + BOSS_MARKER_FIRST); node; node = ld32(node))
        call(RaidBossActor_setAttackDamage, {boss(self), ld32(node + 8), 0, 0});
    call(IBuffProtocol_elapseCountAttackBuff, {ld32(call(AppState_getInstance, {}) + 32) + STAGESTATE_BUFFS});
    call(AvatarActor_UpdateStatus, {avatar(self)});
    uint32_t state = ld32(call(AppState_getInstance, {}) + 32);
    if (uint32_t damage = sbGet(state, STAGESTATE_PENDING_DAMAGE)) {
        call(StageState_addDamage, {ld32(call(AppState_getInstance, {}) + 32), damage});
        SafeBuffer *s = rw::host_ptr<SafeBuffer>(ld32(call(AppState_getInstance, {}) + 32));
        st32(zero, 0);
        SafeBuffer::write(s, s->data.get() + STAGESTATE_PENDING_DAMAGE, rw::host_ptr<uint8_t>(zero), 4,
                          STAGESTATE_PENDING_DAMAGE);
    }
    scene::st8(boss(self) + BOSS_SLEPT, 0);
}
RW_FUNCTION(0x5EFD84, "RaidBattle::reflectionCondition", RaidBattle::reflectionCondition);

// The attacker jumps back to its place (the time from the distance, in frames of 1/30 s) and the camera returns.
void RaidBattle::go_back(uint32_t self) {
    call(Avatar_changeMotion, {ld32(avatar(self) + ACTOR_AVATAR), 18});
    Frame f;
    uint32_t home = f.alloc(8), zero = f.alloc(8);
    st32(home, fb(-500.0f));
    st32(home + 4, fb(-480.0f));
    float dist = rw::bitsf(call(Vec2_distance, {home, getPosition(avatar(self))}));
    float seconds = float(scene::f2i(dist / 30.0f)) / 30.0f;
    uint32_t jump = call(JumpTo_create, {fb(seconds), home, fb(400.0f), 1});
    uint32_t done;
    {
        Frame g;
        uint32_t fn = scene::newFunction(g, VT_GO_BACK_DONE, {self});
        done = call(CallFunc_create, {fn});
        scene::functionDestroy(fn);
    }
    call(Node_runAction, {avatar(self), call(Sequence_create, {jump, done, 0})});
    st32(zero, 0);
    st32(zero + 4, 0);
    moveCamera(self, seconds, zero, 0.5f, 0);
    reflectionCondition(self);
}
RW_FUNCTION(0x5F0040, "RaidBattle::go_back", RaidBattle::go_back);

// The attack's skill fires, the attack is prepared and its conditions rolled; then the damage per part: the
// targeted one, or the body and every part.
void RaidBattle::resetAttackDamage(uint32_t self) {
    int32_t t = int32_t(ld32(boss(self) + BOSS_TARGET));
    uint32_t t924 = ld32(boss(self) + BOSS_TARGET_924);
    call(SceneBattleBase_invokeAttackSkill, {scene_(self), self + TARGET_TYPE});
    uint32_t bm = call(BattleMisc_getInstance, {});
    call(BattleMisc_preparePlayerAttackRaid, {bm, self + TARGET_TYPE, boss(self), t924});
    call(AvatarActor_UpdateStatus, {avatar(self)});
    addCondition(self, self + TARGET_TYPE, boss(self));
    if (t >= 0) {
        partDamage(self, uint32_t(t), false);
        return;
    }
    Frame f;
    uint32_t enemy = f.alloc(8), part = f.alloc(8);
    uint32_t e = enemyCreate(enemy, ld32(boss(self) + BOSS_ENEMY_ID));
    int32_t n = int32_t(sbGet(e, RE_VALID_PARTS));
    partDamage(self, sbGet(e, RE_WEAKNESS) - 1, true);
    for (int32_t k = 0; k < n; k++) {
        uint32_t pe = enemyCreate(part, sbGet(ld32(enemy), RE_PARTS + 4 * uint32_t(k)));
        partDamage(self, sbGet(pe, RE_WEAKNESS) - 1, true);
        release(ld32(part + 4));
    }
    release(ld32(enemy + 4));
}
RW_FUNCTION(0x5F01C4, "RaidBattle::resetAttackDamage", RaidBattle::resetAttackDamage);

// The boss's turn: its skills' cool-downs tick, the highest usable skill whose rate wins a draw of 0..9999 is used
// (else skill 0), the attack is prepared and the boss plays that skill's motion.
void RaidBattle::enemyAttack(uint32_t self) {
    st32(self + F_676, 1);
    Frame f;
    uint32_t enemy = f.alloc(8);
    uint32_t e = enemyCreate(enemy, ld32(boss(self) + BOSS_ENEMY_ID));
    int32_t n = int32_t(sbGet(e, RE_SKILL_COUNT));
    float rate = rw::bitsf(call(RaidBossActor_MinHPRateRaidTeam, {boss(self)}));
    st32(self + SKILL, 0);
    if (n >= 1) {
        for (int32_t k = 0; k < n; k++) {
            uint32_t at = self + COOLDOWN + 4 * uint32_t(k);
            if (ld32(at)) st32(at, ld32(at) - 1);
        }
        for (int32_t i = n - 1; i >= 0; i--) {
            if (!use_skill_check(self, i, rate)) continue;
            int32_t chance = int32_t(sbGet(ld32(enemy), RE_SKILL_RATE + 4 * uint32_t(i)));
            if (chance > int32_t(randomInt(0, 9999))) {
                st32(self + SKILL, uint32_t(i));
                break;
            }
        }
    }
    use_skill(self, int32_t(ld32(self + SKILL)));
    uint32_t bm = call(BattleMisc_getInstance, {});
    call(BattleMisc_prepareRaidAttack, {bm, boss(self), avatar(self), ld32(self + SKILL)});
    uint32_t b = boss(self);
    call(RaidBoss_changeMotion, {ld32(b + BOSS_RAIDBOSS), ld32(self + SKILL)});
    call(RaidBossActor_makeWholeAttackPoint, {b});
    call(AvatarActor_UpdateStatus, {avatar(self)});
    call(RaidBossActor_UpdateStatus, {boss(self)});
    release(ld32(enemy + 4));
}
RW_FUNCTION(0x5F0428, "RaidBattle::enemyAttack", RaidBattle::enemyAttack);

// The bad statuses the attack's skill (target +8, a master::Skill row) may put on the boss: deep poison, poison,
// sleep, paralysis; misses show at the targeted marker (else the boss).
void RaidBattle::addCondition(uint32_t self, uint32_t target, uint32_t b) {
    Frame f;
    uint32_t pos = f.alloc(8), key = f.alloc(8), enemy = f.alloc(8);
    st32(pos, 0);
    st32(pos + 4, 0);
    int32_t t = int32_t(ld32(b + BOSS_TARGET));
    st32(key, t >= 0 ? uint32_t(t) : 0u);
    uint32_t node = call(markers_find, {b + BOSS_MARKERS, key});
    uint32_t p = node ? getPosition(ld32(node + 12)) : getPosition(b);
    st32(pos, ld32(p));
    st32(pos + 4, ld32(p + 4));
    uint32_t skill = ld32(target + 8);
    if (!skill) return;
    uint32_t e = enemyCreate(enemy, ld32(b + BOSS_ENEMY_ID));
    tryCondition(self, skill, b, e, pos, SKILL_DEEP_POISON, RE_PROTECT_DEEP_POISON, RE_REGIST_DEEP_POISON, 3, 0, false);
    tryCondition(self, ld32(target + 8), b, ld32(enemy), pos, SKILL_POISON, RE_PROTECT_POISON, RE_REGIST_POISON, 0, 0, false);
    tryCondition(self, ld32(target + 8), b, ld32(enemy), pos, SKILL_SLEEP, RE_PROTECT_SLEEP, RE_REGIST_SLEEP, 1, 1, true);
    tryCondition(self, ld32(target + 8), b, ld32(enemy), pos, SKILL_PARALYSIS, RE_PROTECT_PARALYSIS, RE_REGIST_PARALYSIS, 2, 2, false);
    call(RaidBossActor_UpdateStatus, {b});
    release(ld32(enemy + 4));
}
RW_FUNCTION(0x5F05AC, "RaidBattle::addCondition", RaidBattle::addCondition);

// The boss is beaten: the end flash, slow motion and the finishing sound. (A CallFunc is made and dropped.)
void RaidBattle::RaidBossEndSequence(uint32_t self) {
    {
        Frame g;
        uint32_t fn = scene::newFunction(g, VT_END_SEQUENCE, {self});
        call(CallFunc_create, {fn});
        scene::functionDestroy(fn);
    }
    call(RaidDirection_to_end, {ld32(self + DIRECTION), 0});
    call(SceneRaidBattle_to_slow, {scene_(self)});
    Frame f;
    uint32_t layer = f.alloc(8);
    call(audio_Manager_getLayer, {layer, call(audio_Manager_getInstance, {}), 0});
    call(audio_Layer_playSE, {ld32(layer), SE_RAID_END, 0, 0});
    release(ld32(layer + 4));
}
RW_FUNCTION(0x5F0AA0, "RaidBattle::RaidBossEndSequence", RaidBattle::RaidBossEndSequence);

// Whether skill `i` of the boss can be used: by its type (2 always, 3 when its value / 10000 is above the raid
// team's lowest HP rate, 4 when the body's HP rate is below that, 5 the same but once, 6 when off cool-down).
uint32_t RaidBattle::use_skill_check(uint32_t self, int32_t i, float teamRate) {
    Frame f;
    uint32_t enemy = f.alloc(8);
    uint32_t e = enemyCreate(enemy, ld32(boss(self) + BOSS_ENEMY_ID));
    int32_t type = int32_t(sbGet(e, RE_SKILL_TYPE + 4 * uint32_t(i)));
    float value = float(int32_t(sbGet(ld32(enemy), RE_SKILL_VALUE + 4 * uint32_t(i)))) / 10000.0f;
    uint32_t hp = call(RaidBossActor_getCurrentHP, {boss(self), 0});
    uint32_t max = call(RaidBossActor_getMaxHP, {boss(self), 0});
    float hpRate = float(hp) / float(max);
    uint32_t ok = 0;
    switch (type) {
    case 2: ok = 1; break;
    case 3: ok = value > teamRate; break;
    case 4: ok = hpRate < value; break;
    case 5: ok = scene::ld8(self + uint32_t(i) + SKILL_USED) ? 0 : hpRate < value; break;
    case 6: ok = ld32(self + COOLDOWN + 4 * uint32_t(i)) == 0; break;
    default: break;
    }
    release(ld32(enemy + 4));
    return ok;
}
RW_FUNCTION(0x5F0B8C, "RaidBattle::use_skill_check", RaidBattle::use_skill_check);

// Using skill `i`: a once-only skill is spent, a cool-down skill waits its value / 10000 turns.
void RaidBattle::use_skill(uint32_t self, int32_t i) {
    Frame f;
    uint32_t enemy = f.alloc(8);
    uint32_t e = enemyCreate(enemy, ld32(boss(self) + BOSS_ENEMY_ID));
    int32_t type = int32_t(sbGet(e, RE_SKILL_TYPE + 4 * uint32_t(i)));
    int32_t value = int32_t(sbGet(ld32(enemy), RE_SKILL_VALUE + 4 * uint32_t(i)));
    if (type == 5) scene::st8(self + uint32_t(i) + SKILL_USED, 1);
    else if (type == 6) st32(self + COOLDOWN + 4 * uint32_t(i), uint32_t(value / 10000));
    release(ld32(enemy + 4));
}
RW_FUNCTION(0x5F0D20, "RaidBattle::use_skill", RaidBattle::use_skill);

// Poison (Badstatus 102), else deep poison (104), hurts the boss at the body marker; true when it did.
uint32_t RaidBattle::setRaidEnemyPoisonEffect(uint32_t self) {
    Frame f;
    uint32_t key = f.alloc(8), out = f.alloc(16);
    st32(key, 0);
    uint32_t b = boss(self);
    uint32_t marker = call(markers_find, {b + BOSS_MARKERS, key});
    if (!marker) return 0;
    uint32_t hp = call(RaidBossActor_getCurrentHP, {b, 0});
    if (!hp) return 0;
    if (bossCondition(boss(self), out, 0)) return poisonDamage(self, marker, hp, 102);
    if (!bossCondition(boss(self), out, 3)) return 0;
    return poisonDamage(self, marker, hp, 104);
}
RW_FUNCTION(0x5F0DFC, "RaidBattle::setRaidEnemyPoisonEffect", RaidBattle::setRaidEnemyPoisonEffect);

// ------------------------------------------------------------------------------------------------ update
namespace {
constexpr uint32_t Node_getNumberOfRunningActions = 0x894B4F;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t DelayTime_create = 0x86852D;
constexpr uint32_t EaseOut_create = 0x8606A5;           // (action, rate)
constexpr uint32_t RaidBattle_RaidMotionRecover = 0x5EE461;
constexpr uint32_t FlashNode_isPlaying = 0x5EE499;
constexpr uint32_t FlashNode_knockbackShift = 0x5EFB19;
constexpr uint32_t FlashNode_flagShift = 0x5EFD39;
constexpr uint32_t StageUI_getCarouselDeck = 0x75E661;
constexpr uint32_t CarouselDeck_actionAttack = 0x5D0E95;
constexpr uint32_t MedalDeck_isCurrentBurstSingleSelect = 0x5E046D;
constexpr uint32_t RaidBossActor_get_active_parts = 0x5F37A5;
constexpr uint32_t RaidBossActor_ShowTarget = 0x5F3641;
constexpr uint32_t RaidBossActor_ReselectTarget = 0x5F3689;
constexpr uint32_t RaidBossActor_getAttackDamageFlags = 0x5F3491;
constexpr uint32_t CommonDirection_req_select_target_disp = 0x76543B;
constexpr uint32_t CommonDirection_req_select_target_hide = 0x76547D;
constexpr uint32_t CommonDirection_req_skilPanel_in = 0x765A19;
constexpr uint32_t SceneRaidBattle_SelectRandom = 0x609A71;
constexpr uint32_t SceneRaidBattle_endPlayerTurn = 0x60A7DD;
constexpr uint32_t SceneRaidBattle_endPlayerCount = 0x60A839;
constexpr uint32_t SceneRaidBattle_setPlayerDamage = 0x60A96D;
constexpr uint32_t SceneRaidBattle_getEnemyHp = 0x60AA55;
constexpr uint32_t SceneRaidBattle_advanceBattleSequence = 0x60AA63;
constexpr uint32_t SceneBattleBase_enemy_count_end = 0x5FA73F;
constexpr uint32_t SceneBattleBase_DamageSkill = 0x5FAA41;
constexpr uint32_t BattleMisc_calculateRaidAttack = 0x748669;
constexpr uint32_t master_RaidEnemyAttack_create = 0x789039;
constexpr uint32_t HitEffect_deploy = 0x5D9225;         // (effect, node, const Vec2&, int, float, const function&)
constexpr uint32_t GuiltBurstCutInShader_setShaderEnd = 0x496271;
constexpr uint32_t StageActorManager_isEndFriendSummons = 0x751F27;
constexpr uint32_t RaidDirection_raid_fever_over = 0x5F83C5;
constexpr uint32_t VT_TARGET_SELECTED = 0xF5DE64;       // update $_: CallFunc {this} (2 s after the markers show)
constexpr uint32_t SE_ATTACK_WEAK = 0x1DEBE9F8;         // a hit on a weak point
constexpr uint32_t SE_PLAYER_DOWN = 0x1E56B990;

constexpr uint32_t V_SET_VISIBLE = 0xB0;
constexpr uint32_t V_FLASH_HIT_EFFECT = 0x274;          // FlashNode: the hit effect id of its current motion

constexpr uint32_t SCENE_DIRECTION = 784;               // SceneRaidBattle: CommonDirection* (skill panels, numbers)
constexpr uint32_t CAROUSEL_BUSY_A = 0x3FC, CAROUSEL_BUSY_B = 0x3FA;
constexpr uint32_t SAM_FRIEND = 0x20;                   // StageActorManager: the friend's AvatarActor*
constexpr uint32_t SAM_SUMMON = 0x24;                   // StageActorManager: the summoned friend
constexpr uint32_t FLASH_MOTION = 0x524;                // FlashNode: current motion
constexpr uint32_t FLASH_KNOCKBACK = 0x4DC;             // FlashNode: a knock-back frame came
constexpr uint32_t FLASH_HIT = 0x4F4;                   // FlashNode: a hit frame came
constexpr uint32_t BURST_RUN_DISTANCE = 0xC;            // master::Burst: half the distance kept from the target
constexpr uint32_t BURST_HIT_SIDE = 0x10;               // master::Burst: 1 = the hit effect faces right
constexpr uint32_t BURST_RANDOM_TARGETS = 0x160;
constexpr uint32_t RE_ATTACKS = 0x220;                  // RaidEnemy: RaidEnemyAttack id per skill (4)
constexpr uint32_t STAGESTATE_HP = 8;
constexpr uint32_t STAGESTATE_DAMAGE_SKILL_RATE = 0x4238;
constexpr uint32_t CHAIN_COUNT = 800;                   // RaidBattle: chain attacks left
constexpr uint32_t CHAIN_ON = 804;                      // RaidBattle: uint8
constexpr uint32_t RUNNING = 704;                       // RaidBattle: uint8: the attacker runs at the boss
constexpr float RUN_SECONDS = 0.26666668f;

uint32_t running(uint32_t node) { return call(Node_getNumberOfRunningActions, {node}); }
uint32_t avatarObj(uint32_t self) { return ld32(avatar(self) + ACTOR_AVATAR); }
uint32_t actorManager(uint32_t self) { return ld32(scene_(self) + SCENE_ACTOR_MANAGER); }
uint32_t friendAvatar(uint32_t self) { return ld32(ld32(actorManager(self) + SAM_FRIEND) + ACTOR_AVATAR); }
uint32_t isPlaying(uint32_t flash) { return call(FlashNode_isPlaying, {flash}); }
void motion(uint32_t flash, uint32_t m) { call(Avatar_changeMotion, {flash, m}); }
uint32_t medalField(uint32_t self) { return call(StageUI_getMedalField, {ld32(scene_(self) + SCENE_STAGE_UI)}); }
uint32_t carousel(uint32_t self) { return call(StageUI_getCarouselDeck, {ld32(scene_(self) + SCENE_STAGE_UI)}); }
void setState(uint32_t self, uint32_t s) { st32(self + STATE, s); }
// a value of the current medal's Burst row (the medal info read again from the medal field)
uint32_t burstValue(uint32_t self, uint32_t index, bool keepInfo) {
    uint32_t info = call(MedalDeck_getCurrentMedalInfo, {medalField(self)});
    if (keepInfo) st32(self + MEDAL_INFO, info);
    Frame f;
    uint32_t burst = f.alloc(8);
    call(master_Burst_create, {burst, sbGet(ld32(info), MEDAL_BURST_ID)});
    uint32_t v = sbGet(ld32(burst), index);
    release(ld32(burst + 4));
    return v;
}
// how far before the target the attacker stops: a burst's own distance, else 100 each side; the medal info is
// the stored one (`fresh`: read again from the medal field and stored)
float runDistance(uint32_t self, bool fresh) {
    if (ld32(self + TARGET_TYPE) != 3) return 200.0f;
    uint32_t v;
    if (fresh) {
        v = burstValue(self, BURST_RUN_DISTANCE, true);
    } else {
        Frame f;
        uint32_t burst = f.alloc(8);
        call(master_Burst_create, {burst, sbGet(ld32(ld32(self + MEDAL_INFO)), MEDAL_BURST_ID)});
        v = sbGet(ld32(burst), BURST_RUN_DISTANCE);
        release(ld32(burst + 4));
    }
    float d = float(int32_t(v));
    return d + d;
}
// the boss back to its idle motion once its current one ended; false while it still plays
bool bossIdle(uint32_t self) {
    uint32_t flash = ld32(boss(self) + BOSS_RAIDBOSS);
    if (ld32(flash + FLASH_MOTION) == 4) return true;
    if (isPlaying(flash)) return false;
    bossMotion(boss(self), 4);
    return true;
}
void playSE(uint32_t id) {
    Frame f;
    uint32_t layer = f.alloc(8);
    call(audio_Manager_getLayer, {layer, call(audio_Manager_getInstance, {}), 0});
    call(audio_Layer_playSE, {ld32(layer), id, 0, 0});
    release(ld32(layer + 4));
}
void hitEffect(uint32_t effect, uint32_t node, uint32_t pos, float side) {
    Frame f;
    uint32_t fn = scene::newEmptyFunction(f);
    call(HitEffect_deploy, {effect, node, pos, 300, fb(side), fn});
    scene::functionDestroy(fn);
}

// 7: the attacker stands still: it runs at the boss (far side) or swings in place (near side)
void stateApproach(uint32_t self) {
    if (running(avatar(self))) return;
    st32(self + CHAIN_COUNT, 0);
    uint32_t side = ld32(self + SIDE);
    if (side != 2) {
        if (side != 1) return;
        motion(avatarObj(self), 3);
        setState(self, 8);
        return;
    }
    float dist = runDistance(self, false);
    float bx = ldf(getPosition(boss(self)));
    float offX = ldf(self + OFFSET);
    uint32_t p2 = getPosition(boss(self));
    Frame f;
    uint32_t v = f.alloc(8);
    st32(v, fb(bx + offX - dist));
    st32(v + 4, fb(ldf(p2 + 4) + ldf(self + OFFSET + 4)));
    uint32_t a = avatar(self);
    call(Node_runAction, {a, call(EaseOut_create, {call(MoveTo_create, {fb(RUN_SECONDS), v}), fb(2.0f)})});
    motion(avatarObj(self), 16);
    scene::st8(self + RUNNING, 1);
    setState(self, 9);
    RaidBattle::moveCamera(self, RUN_SECONDS, v, 1.0f, 1);
}

// 11: choosing the target: chain / plain attacks go on; a burst that picks one target shows the markers (or goes
// for the body when no part is left) and waits for the player
void stateSelect(uint32_t self) {
    if (ld32(self + TARGET_TYPE) - 1 <= 1) { setState(self, 13); return; }
    if (scene::ld8(carousel(self) + CAROUSEL_BUSY_A)) return;
    if (call(MedalDeck_isCurrentBurstSingleSelect, {medalField(self)}) != 1) { setState(self, 13); return; }
    if (!call(RaidBossActor_get_active_parts, {boss(self)})) {
        RaidBattle::set_week_position(self);
        call(CommonDirection_req_select_target_hide, {ld32(self + FIELD_OWNER)});
        return;
    }
    call(RaidBossActor_ShowTarget, {boss(self), 1});
    call(RaidBossActor_ReselectTarget, {boss(self)});
    scene::st8(scene_(self) + SCENE_TARGET_BYTE, 1);
    uint32_t done;
    {
        Frame g;
        uint32_t fn = scene::newFunction(g, VT_TARGET_SELECTED, {self});
        done = call(CallFunc_create, {fn});
        scene::functionDestroy(fn);
    }
    uint32_t delay = call(DelayTime_create, {fb(2.0f)});
    call(Node_runAction, {boss(self), call(Sequence_create, {delay, done, 0})});
    setState(self, 12);
    call(CommonDirection_req_select_target_disp, {ld32(self + FIELD_OWNER)});
}

// 13: run at the chosen target
void stateRun(uint32_t self) {
    float offY = ldf(self + OFFSET + 4);
    float ax = ldf(getPosition(avatar(self)));
    float y = offY + -480.0f;
    st32(self + AVATAR_POS + 4, fb(y));
    st32(self + AVATAR_POS, fb(ax));
    Frame f;
    uint32_t v = f.alloc(8);
    st32(v, 0);
    st32(v + 4, 0);
    float dist = runDistance(self, true);
    st32(v + 4, ld32(getPosition(avatar(self)) + 4));
    uint32_t bp = getPosition(boss(self));
    st32(v, fb(ldf(bp) + ldf(self + OFFSET) - dist));
    uint32_t a = avatar(self);
    call(Node_runAction, {a, call(EaseSineInOut_create, {call(MoveTo_create, {fb(RUN_SECONDS), v})})});
    motion(avatarObj(self), 24);
    RaidBattle::moveCamera(self, RUN_SECONDS, v, 1.0f, 1);
    setState(self, 7);
}

// 20: the attack's frames: knock-back frames land the damage, hit frames show the hit effect (and the weak-point
// sound); when the motion ends a chain attack may follow, else the attacker goes back (or the boss is down)
void stateStrike(uint32_t self) {
    call(RaidBattle_RaidMotionRecover, {self});
    uint32_t flash = avatarObj(self);
    if (ld32(flash + FLASH_KNOCKBACK)) {
        call(FlashNode_knockbackShift, {flash});
        if (ld32(self + TARGET_TYPE) == 3 && burstValue(self, BURST_RANDOM_TARGETS, false) == 3)
            call(SceneRaidBattle_SelectRandom, {scene_(self), 1});
        RaidBattle::RaidDamage(self);
        flash = avatarObj(self);
    }
    if (ld32(flash + FLASH_HIT) && call(FlashNode_flagShift, {flash}) == 1) {
        float side = -1.0f;
        if (ld32(self + TARGET_TYPE) == 3) side = burstValue(self, BURST_HIT_SIDE, false) == 1 ? 1.0f : -1.0f;
        flash = avatarObj(self);
        uint32_t effect = vcall(flash, V_FLASH_HIT_EFFECT, {flash});
        uint32_t node = ld32(self + CAMERA_NODE);
        float x = ldf(getPosition(boss(self)));
        float offX = ldf(self + OFFSET);
        uint32_t p2 = getPosition(boss(self));
        Frame f;
        uint32_t v = f.alloc(8);
        st32(v, fb(x + offX));
        st32(v + 4, fb(ldf(p2 + 4) + ldf(self + OFFSET + 4)));
        hitEffect(effect, node, v, side);
        if (ld32(self + TARGET_TYPE) != 3) {
            int32_t first = 0, end = 6;
            if (int32_t(ld32(boss(self) + BOSS_TARGET_924)) >= 0) {
                first = int32_t(ld32(boss(self) + BOSS_TARGET));
                end = first + 1;
            }
            for (int32_t k = first; k < end; k++) {
                uint32_t flags = call(RaidBossActor_getAttackDamageFlags, {boss(self), uint32_t(k)});
                if (scene::ld8(flags) & 2) { playSE(SE_ATTACK_WEAK); break; }
            }
        }
    }
    if (!call(RaidBossActor_getCurrentHP, {boss(self), 0})) {
        call(SceneRaidBattle_endPlayerTurn, {scene_(self)});
        if (uint32_t shader = ld32(self + SHADER)) call(GuiltBurstCutInShader_setShaderEnd, {shader});
        setState(self, 36);
        return;
    }
    if (isPlaying(avatarObj(self))) return;
    if (ld32(self + TARGET_TYPE) == 1) {
        uint32_t chain = self + CHAIN_COUNT;
        if (!ld32(chain) && scene::ld8(self + CHAIN_ON) && !scene::ld8(self + BATTLE_SEQUENCE)) st32(chain, 1);
        if (call(SceneRaidBattle_getEnemyHp, {scene_(self)}) && int32_t(ld32(chain)) >= 1 &&
            RaidBattle::ChainAttack(self) == 1 && call(AvatarActor_setChainAttackMotion, {avatar(self)}) == 1) {
            st32(chain, ld32(chain) - 1);
            scene::st8(self + BATTLE_SEQUENCE, uint8_t(call(SceneRaidBattle_advanceBattleSequence, {scene_(self)})));
            RaidBattle::reflectionCondition(self);
            setState(self, 14);
            return;
        }
    }
    if (uint32_t shader = ld32(self + SHADER)) call(GuiltBurstCutInShader_setShaderEnd, {shader});
    RaidBattle::go_back(self);
    setState(self, 21);
}

// 32: the boss's turn starts: asleep it skips; paralysed (Badstatus 122 rate against a draw of 1..10000, tried
// each frame) it skips; the fever also skips turns (one level each); else it attacks
void stateBossTurn(uint32_t self) {
    if (running(avatar(self))) return;
    Frame f;
    uint32_t out = f.alloc(16), row = f.alloc(8);
    if (bossCondition(boss(self), out, 1)) {
        setState(self, 35);
        uint32_t b = boss(self);
        if (ld32(ld32(b + BOSS_RAIDBOSS) + FLASH_MOTION) != 4) bossMotion(b, 4);
        return;
    }
    if (bossCondition(boss(self), out, 2)) {
        call(master_Badstatus_create, {row, 122});
        int32_t rate = int32_t(sbGet(ld32(row), BADSTATUS_DAMAGE));
        if (rate >= int32_t(randomInt(1, 10000))) {
            setState(self, 35);
            uint32_t b = boss(self);
            if (ld32(ld32(b + BOSS_RAIDBOSS) + FLASH_MOTION) != 4) bossMotion(b, 4);
        }
        release(ld32(row + 4));
        return;
    }
    int32_t fever = int32_t(ld32(self + FEVER));
    if (fever < 1) {
        setState(self, 33);
        RaidBattle::enemyAttack(self);
        return;
    }
    st32(self + FEVER, uint32_t(fever - 1));
    if (fever - 1) call(RaidDirection_raid_fever, {ld32(self + DIRECTION), uint32_t(fever - 1)});
    else call(RaidDirection_raid_fever_over, {ld32(self + DIRECTION)});
    setState(self, 35);
    uint32_t b = boss(self);
    if (ld32(ld32(b + BOSS_RAIDBOSS) + FLASH_MOTION) != 4) bossMotion(b, 4);
}

// 33: the boss's attack frames: knock-back frames hurt the player (BattleMisc; the damage skill scales it),
// hit frames show the hit effect; when its motion ends it idles again
void stateBossStrike(uint32_t self) {
    uint32_t flash = ld32(boss(self) + BOSS_RAIDBOSS);
    if (ld32(flash + FLASH_KNOCKBACK)) {
        call(FlashNode_knockbackShift, {flash});
        Frame f;
        uint32_t enemy = f.alloc(8), attack = f.alloc(8), pos = f.alloc(8);
        uint32_t e = enemyCreate(enemy, ld32(boss(self) + BOSS_ENEMY_ID));
        call(master_RaidEnemyAttack_create, {attack, sbGet(e, RE_ATTACKS + 4 * ld32(self + SKILL))});
        uint32_t bm = call(BattleMisc_getInstance, {});
        uint32_t b = boss(self);
        uint32_t damage = call(BattleMisc_calculateRaidAttack, {bm, b, medalField(self), attack});
        if (uint32_t skill = call(SceneBattleBase_DamageSkill, {scene_(self)})) {
            uint32_t state = ld32(call(AppState_getInstance, {}) + 32);
            int32_t rate = int32_t(sbGet(state, STAGESTATE_DAMAGE_SKILL_RATE));
            uint32_t d = uint32_t(int64_t(rate) * int64_t(int32_t(damage)) / 10000);
            damage = d ? d : 1;
            if (uint32_t dir = ld32(scene_(self) + SCENE_DIRECTION)) call(CommonDirection_req_skilPanel_in, {dir, skill});
        }
        call(SceneRaidBattle_setPlayerDamage, {scene_(self), damage});
        uint32_t node = fieldNode(self);
        uint32_t player = ld32(scene_(self) + SCENE_PLAYER);
        vcall(node, V_SET_POSITION, {node, getPosition(player)});
        node = fieldNode(self);
        player = ld32(scene_(self) + SCENE_PLAYER);
        vcall(node, V_SET_SCALE, {node, vcall(player, V_GET_SCALE, {player})});
        uint32_t bm2 = call(BattleMisc_getInstance, {});
        uint32_t numbers = ld32(ld32(scene_(self) + SCENE_DIRECTION) + FIELD_NODE);
        uint32_t p = getPosition(avatar(self));
        st32(pos, ld32(p));
        st32(pos + 4, ld32(p + 4));
        uint32_t flags = ld32(bm2 + BATTLEMISC_FLAGS);
        call(DisplayDamageNumber_disp_dmgNum, {damage, numbers, pos, fb(70.0f), (flags & 2) >> 1, (flags & 4) >> 2, 0,
                                               flags & 1, 600, fb(2.0f)});
        call(FlashNode_reflection_status, {ld32(boss(self) + BOSS_RAIDBOSS), 0});
        uint32_t hp = sbGet(ld32(call(AppState_getInstance, {}) + 32), STAGESTATE_HP);
        uint32_t af = avatarObj(self);
        if (hp) {
            motion(af, 17);
        } else if (ld32(af + FLASH_MOTION) != 41) {
            motion(af, 41);
            playSE(SE_PLAYER_DOWN);
        }
        release(ld32(attack + 4));
        release(ld32(enemy + 4));
    }
    flash = ld32(boss(self) + BOSS_RAIDBOSS);
    if (ld32(flash + FLASH_HIT) && call(FlashNode_flagShift, {flash}) == 1) {
        flash = ld32(boss(self) + BOSS_RAIDBOSS);
        uint32_t effect = vcall(flash, V_FLASH_HIT_EFFECT, {flash});
        uint32_t node = ld32(self + CAMERA_NODE);
        float x = ldf(getPosition(avatar(self)));
        float y = ldf(getPosition(avatar(self)) + 4) + 70.0f;
        Frame f;
        uint32_t v = f.alloc(8);
        st32(v, fb(x));
        st32(v + 4, fb(y));
        hitEffect(effect, node, v, 1.0f);
    }
    uint32_t af = avatarObj(self);
    if (ld32(af + FLASH_MOTION) == 41 && !isPlaying(af)) motion(avatarObj(self), 42);
    if (!bossIdle(self)) return;
    setState(self, 34);
}
}  // namespace

// The attack's state machine, one step per frame: 7..14 the player's attack (approach, target, run, strike),
// 20 / 21 its frames and the way back, 32..36 the boss's turn, 55..57 a friend summon finishing; other states wait.
void RaidBattle::update(uint32_t self, float) {
    switch (ld32(self + STATE)) {
    case 7: stateApproach(self); break;
    case 9:
        if (running(avatar(self))) return;
        [[fallthrough]];
    case 8:
        avatarAttack(self);
        setState(self, 20);
        break;
    case 10:
        if (call(StageActorManager_isEndFriendSummons, {actorManager(self)}) == 1) setState(self, 11);
        break;
    case 11: stateSelect(self); break;
    case 12: {
        int32_t t = int32_t(ld32(boss(self) + BOSS_TARGET_924));
        if (t < 0) return;
        call(RaidBossActor_HideTargetNoSelected, {boss(self), uint32_t(t)});
        call(CommonDirection_req_select_target_hide, {ld32(self + FIELD_OWNER)});
        setState(self, 13);
        break;
    }
    case 13: stateRun(self); break;
    case 14:
        call(RaidBattle_RaidMotionRecover, {self});
        if (scene::ld8(carousel(self) + CAROUSEL_BUSY_B)) return;
        call(CarouselDeck_actionAttack, {carousel(self), 0});
        st32(self + MEDAL_INFO, call(MedalDeck_getCurrentMedalInfo, {medalField(self)}));
        resetAttackDamage(self);
        setState(self, 20);
        break;
    case 20: stateStrike(self); break;
    case 21: {
        call(RaidBattle_RaidMotionRecover, {self});
        if (running(avatar(self))) return;
        scene::st8(self + RUNNING, 0);
        uint32_t sam = actorManager(self);
        if (!scene::ld8(sam + SAM_FRIEND_BURST)) { setState(self, 57); return; }
        setState(self, 55);
        motion(ld32(ld32(sam + SAM_SUMMON) + ACTOR_AVATAR), 30);
        motion(friendAvatar(self), 45);
        break;
    }
    case 32: stateBossTurn(self); break;
    case 33: stateBossStrike(self); break;
    case 34: {
        uint32_t player = ld32(scene_(self) + SCENE_PLAYER);
        call(Node_runAction, {player, call(EaseSineInOut_create, {call(ScaleTo_create, {0x3EAAAAAB, fb(0.5f)})})});
        setState(self, 35);
        break;
    }
    case 35:
        if (running(ld32(scene_(self) + SCENE_PLAYER))) return;
        setState(self, 0);
        call(SceneBattleBase_enemy_count_end, {scene_(self), boss(self)});
        call(Node_unscheduleUpdate, {self});
        break;
    case 36:
        if (isPlaying(avatarObj(self))) return;
        motion(avatarObj(self), 18);
        go_back(self);
        setState(self, 35);
        break;
    case 55: {
        call(RaidBattle_RaidMotionRecover, {self});
        if (call(StageActorManager_isEndFriendSummons, {actorManager(self)}) != 1) return;
        setState(self, 56);
        uint32_t sam = actorManager(self);
        scene::st8(sam + SAM_FRIEND_BURST, 0);
        uint32_t summon = ld32(sam + SAM_SUMMON);
        vcall(summon, V_SET_VISIBLE, {summon, 0});
        break;
    }
    case 56:
        call(RaidBattle_RaidMotionRecover, {self});
        if (isPlaying(friendAvatar(self))) return;
        motion(friendAvatar(self), 11);
        setState(self, 57);
        break;
    case 57:
        if (!isPlaying(friendAvatar(self))) motion(friendAvatar(self), 11);
        if (!bossIdle(self)) return;
        scene::st8(self + BATTLE_SEQUENCE, uint8_t(call(SceneRaidBattle_advanceBattleSequence, {scene_(self)})));
        if (call(RaidBossActor_getCurrentHP, {boss(self), 0})) {
            if (scene::ld8(self + BATTLE_SEQUENCE)) {
                call(SceneRaidBattle_endPlayerTurn, {scene_(self)});
                setState(self, 60);
            } else {
                call(SceneRaidBattle_endPlayerCount, {scene_(self)});
            }
            st32(self + AVATAR, ld32(actorManager(self) + SAM_FRIEND));
            call(Node_unscheduleUpdate, {self});
        } else {
            call(SceneRaidBattle_endPlayerTurn, {scene_(self)});
            setState(self, 35);
        }
        break;
    default: break;
    }
}
RW_FUNCTION(0x5EE53C, "RaidBattle::update", RaidBattle::update);
