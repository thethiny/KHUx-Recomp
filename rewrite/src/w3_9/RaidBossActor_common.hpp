// Shared by the RaidBossActor rewrite files: the guest functions they call and small helpers.
#pragma once

#include "RaidBossActor.hpp"

namespace rba {

using guest::Frame;

// ------------------------------------------------------------------------------------------------ guest functions
namespace fn {
// libc / libc++ / cocos2d
constexpr uint32_t aeabi_memset = 0x40E7A0;         // __aeabi_memset(dst, n, c)
constexpr uint32_t memcpy_ = 0x40E164;              // memcpy (import)
constexpr uint32_t strlen_ = 0x40E08C;              // strlen (import)
constexpr uint32_t string_init = 0x47CD93;          // std::string::__init(const char*, size_t)
constexpr uint32_t string_eq_cstr = 0x47CBD5;       // sub_47CBD4: std::string::compare(const char*) (0 = equal)
constexpr uint32_t Vec2_ctor = 0x8E4B95;            // cocos2d::Vec2::Vec2()
constexpr uint32_t Vec2_ctor_ff = 0x8E4B9D;         // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t Vec2_copy = 0x8E4C03;            // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Vec2_add = 0x8E4D2D;             // Vec2::add(const Vec2&)
constexpr uint32_t Vec2_scale = 0x8E50D5;           // Vec2::scale(float)
constexpr uint32_t Vec2_ZERO = 0x1006950;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t Director_getInstance = 0x8EE519;
constexpr uint32_t Director_getVisibleSize = 0x8F0675;   // (Size* out, Director*)
constexpr uint32_t VectorMapPrizes_dtor = 0x5DB649;      // cocos2d::Vector<MapPrizes*>::~Vector
constexpr uint32_t VectorMapPrizes_pushBack = 0x5DB985;  // cocos2d::Vector<MapPrizes*>::pushBack
constexpr uint32_t VectorMapPrize_dtor = 0x5DD8ED;       // cocos2d::Vector<MapPrize*>::~Vector
// std::unordered_map<int, BattleMarker*>
constexpr uint32_t markers_dtor = 0x5F7AA1;
constexpr uint32_t markers_find = 0x5F1251;              // find(const int&) -> node or 0
constexpr uint32_t markers_insert = 0x5F7115;            // __insert_unique(pair<int, BattleMarker*>&&) (sret)
// std::unordered_map<std::string, cocos2d::Vec2>
constexpr uint32_t clickMap_copy = 0x5F33A1;             // unordered_map(const unordered_map&)
constexpr uint32_t clickMap_dtor = 0x5D8485;
constexpr uint32_t clickMap_find = 0x5D78F9;             // find(const std::string&)
constexpr uint32_t markerFunction_swap = 0x5F74E9;       // std::function<void(BattleMarker*)>::swap
constexpr uint32_t bitset3_fill_false = 0x5F7A23;        // std::__fill_n_false<__bitset<1,3>>(__bit_iterator, n)
constexpr uint32_t PlayerVector_copy = 0x5F4A05;         // vector<RaidStart::Player>(const vector&)
constexpr uint32_t PlayerVector_dtor = 0x4B572D;         // __vector_base<RaidStart::Player>::~__vector_base
// game
constexpr uint32_t SafeBuffer_construct = 0x459F21;      // SafeBuffer(size, registerForUpdate)
constexpr uint32_t SafeBuffer_destruct = 0x45A13D;
constexpr uint32_t SafeBuffer_zerofill = 0x45A605;
constexpr uint32_t StringBuffer_ctor = 0x49140D;         // StringBuffer(this, fmt, ...)
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5;
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_generateUniformInt = 0x492225;    // (random, min, max, uint* state)
constexpr uint32_t Random_generateUniformFloat = 0x492261;  // (random, float min, float max, uint* state)
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t StageState_addLUX = 0x4EECD9;
constexpr uint32_t StageState_addEXP = 0x4EEDF1;
constexpr uint32_t StageState_addMoney = 0x4EEE7D;
constexpr uint32_t StageState_checkMaxAttack = 0x4EF0C9;
constexpr uint32_t IStatusProtocol_addCondition_common = 0x4F1D21;
constexpr uint32_t IStatusProtocol_masterId2ConditionType = 0x4F1FC1;  // (protocol, id, bool* flag)
constexpr uint32_t BuffChange_elapseTurn = 0x4F207B;
constexpr uint32_t BuffChange_elapseCount = 0x4F2099;
constexpr uint32_t IBuffProtocol_buff_level = 0x4F23CD;     // (protocol, type, target)
constexpr uint32_t IBuffProtocol_buff_add = 0x4F256F;       // (protocol, value, turns, count, type, target)
constexpr uint32_t IBuffProtocol_getBlankBuff = 0x4F2781;
constexpr uint32_t IBuffProtocol_buff_clear = 0x4F27E9;     // (protocol, type, target)
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;      // (ResourceID, int fadeMs)
constexpr uint32_t FlashNode_set_bad_status = 0x5C2D61;
constexpr uint32_t FlashNode_reflection_status = 0x5C3171;
constexpr uint32_t FlashNode_UpdateStatusDeepPoison = 0x5C3BCD;
constexpr uint32_t FlashNode_UpdateStatusPoison = 0x5C3C55;
constexpr uint32_t FlashNode_UpdateStatusSleep = 0x5C3CD9;
constexpr uint32_t FlashNode_UpdateStatusParalysis = 0x5C3D61;
constexpr uint32_t BattleMarker_create = 0x5D553D;          // (RaidBossHPIndicator::Type, hp)
constexpr uint32_t BattleMarker_set_scene = 0x5D59F5;
constexpr uint32_t BattleMarker_show_wait = 0x5D5A27;
constexpr uint32_t BattleMarker_show_select = 0x5D5A9B;
constexpr uint32_t BattleMarker_disp_select_string = 0x5D5B13;
constexpr uint32_t BattleMarker_show_selected = 0x5D5B29;
constexpr uint32_t BattleMarker_show_selected_random = 0x5D5C55;
constexpr uint32_t BattleMarker_show = 0x5D5CBD;            // (marker, float x, float y, Color)
constexpr uint32_t BattleMarker_hide = 0x5D5D3D;
constexpr uint32_t BattleMarker_setHP = 0x5D5D79;
constexpr uint32_t RaidBoss_create = 0x5D6685;
constexpr uint32_t RaidBoss_getClickPoints = 0x5D7385;
constexpr uint32_t MapPrizes_Info_ctor = 0x5DB935;          // Info(this, Vec2 by value (pointer), FromGeneration)
constexpr uint32_t MapPrizes_create = 0x5DE8AD;             // (const Info*, Node* parent)
constexpr uint32_t MapPrizes_eruption = 0x5DEAE1;           // eruption(Vec2& position, Vec2& luxPosition)
constexpr uint32_t MapNode_construct = 0x5DA2D5;
constexpr uint32_t MapNode_destruct = 0x5DA38D;
constexpr uint32_t RaidBossHPIndicator_setSuitableAsTargetEffect = 0x5E7881;
constexpr uint32_t RaidBossHPIndicator_set_buf_debuf = 0x5E7D91;
constexpr uint32_t RaidBossHPIndicator_Next_Disp = 0x5E7DB9;
constexpr uint32_t RaidBossHPIndicator_clr_buf_debuf = 0x5E7F41;
constexpr uint32_t RaidBossHPIndicator_set_attack_buff_status = 0x5E7F89;
constexpr uint32_t RaidBossHPIndicator_set_defence_buff_status = 0x5E8189;
constexpr uint32_t RaidBossStatusIndicator_create = 0x5E83AD;  // (name, hp, hpMax, level, type, hpPerBox)
constexpr uint32_t RaidBossStatusIndicator_setRaidBossHP = 0x5E9191;
constexpr uint32_t DisplayDamageNumber_disp_dmgNum = 0x5EB45D;
constexpr uint32_t master_BattleMisc_create = 0x779AD1;
constexpr uint32_t master_Misc_create = 0x786D5D;
constexpr uint32_t master_RaidEnemy_create = 0x787745;
}  // namespace fn

// RaidBossActor member functions, as their callers call them (Thumb addresses)
namespace self_fn {
constexpr uint32_t construct = 0x5F1F21;
constexpr uint32_t init = 0x5F21CD;
constexpr uint32_t destruct = 0x5F2A1D;
constexpr uint32_t setEnemyPartDamage = 0x5F2BD1;
constexpr uint32_t addDamage = 0x5F2D21;
constexpr uint32_t find_marker = 0x5F3D79;
constexpr uint32_t setPrizeParam = 0x5F3EF9;
constexpr uint32_t set_strat_hp = 0x5F400D;
constexpr uint32_t set_buff_param = 0x5F4261;
constexpr uint32_t set_buff_param_parts = 0x5F43B3;
constexpr uint32_t get_api_hp = 0x5F44BD;
constexpr uint32_t addCrushingPrizes = 0x5F57C9;
constexpr uint32_t addCrushingPrizesLux = 0x5F59A5;
constexpr uint32_t addAttackPrizes = 0x5F5AA5;
constexpr uint32_t addAttackPrizesLux = 0x5F5F11;
constexpr uint32_t addPrizes = 0x5F6071;
constexpr uint32_t prizesLuxCommon = 0x5F61D9;
constexpr uint32_t prizesLuxPostCommon = 0x5F63E9;
constexpr uint32_t raidAddCondition = 0x5F67C5;
}  // namespace self_fn

// ------------------------------------------------------------------------------------------------ layout facts
constexpr uint32_t APPSTATE_STAGE_STATE = 32;        // AppState: shared_ptr<StageState> (object, control block)
constexpr uint32_t APIMANAGER_USER_STATUS = 140;     // APIManager: SafeBuffer* of the user status (max HP at 56)
constexpr uint32_t APIMANAGER_RAID_START = 376;      // APIManager: shared_ptr<RaidStart> (object, control block)
constexpr uint32_t RAIDSTART_PLAYERS = 32;           // RaidStart: std::vector<RaidStart::Player>
constexpr uint32_t STAGESTATE_RAID_44 = 0x2C;        // StageState data positions zeroed by init
constexpr uint32_t STAGESTATE_RAID_48 = 0x30;
constexpr uint32_t STAGESTATE_RAID_LUX = 0x45C0;     // StageState data: Lux gained in the raid
constexpr uint32_t MARKER_HP_INDICATOR = 632;        // BattleMarker: RaidBossHPIndicator*
constexpr uint32_t MARKER_RING_ONLY = 648;           // BattleMarker: hide() hides only the ring
constexpr uint32_t MARKER_ON_TAP = 656;              // BattleMarker: std::function<void(BattleMarker*)>
constexpr uint32_t BOSS_POINT_ENABLED = 1364;        // RaidBoss: uint8 per part
constexpr uint32_t BOSS_SCALE = 1312;                // RaidBoss: float
constexpr uint32_t FLASHNODE_STATUS_BITS = 1300;     // FlashNode: BadStatusBit set
// master::RaidEnemy data positions
constexpr uint32_t RE_NAME = 4, RE_WEAKNESS = 144, RE_VALID_PARTS = 148, RE_PARTS = 152, RE_DRAW_HEIGHT = 536;
constexpr uint32_t RE_PROTECT_POISON = 608, RE_PROTECT_DEEP_POISON = 612, RE_PROTECT_SLEEP = 616;
constexpr uint32_t RE_PROTECT_PARALYSIS = 620, RE_PROTECT_CONFUSION = 624;
constexpr uint32_t RE_REGIST_POISON = 628, RE_REGIST_DEEP_POISON = 632, RE_REGIST_SLEEP = 636;
constexpr uint32_t RE_REGIST_PARALYSIS = 640, RE_REGIST_CONFUSION = 644;
constexpr uint32_t RE_BUF_ATTACK = 648, RE_BUF_DEFENSE = 652, RE_BUF_ATTACK_POWER = 656, RE_BUF_DEFENSE_POWER = 660;
constexpr uint32_t RE_BUF_ATTACK_SPEED = 664, RE_BUF_DEFENSE_SPEED = 668, RE_BUF_ATTACK_MAGIC = 672;
constexpr uint32_t RE_BUF_DEFENSE_MAGIC = 676, RE_BUF_TURN = 680, RE_BUF_COUNT = 684;
constexpr uint32_t RE_STATUS_AILMENTS = 688, RE_STATUS_AILMENTS_TURN = 696, RE_STATUS_AILMENTS_COUNT = 700;
constexpr uint32_t RE_BGM_RAID = 704;
// master::Skill data positions read for the prize rates (x 10000)
constexpr uint32_t SKILL_RATE_292 = 292, SKILL_RATE_296 = 296, SKILL_RATE_300 = 300, SKILL_RATE_304 = 304;

// ------------------------------------------------------------------------------------------------ helpers
inline uint32_t ld32(uint32_t a) { return rw::load<uint32_t>(a); }
inline int32_t ldi(uint32_t a) { return rw::load<int32_t>(a); }
inline uint8_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }
inline uint32_t addr(const void *p) { return rw::guest_addr(p); }
inline SafeBuffer *sb(uint32_t a) { return rw::host_ptr<SafeBuffer>(a); }
// a 32-bit value of a SafeBuffer-backed object (what the original reads with SafeBuffer::read into a local)
inline int32_t get32(uint32_t sbAddr, uint32_t index) { return sb(sbAddr)->get32(index); }
inline uint32_t getu(uint32_t sbAddr, uint32_t index) { return uint32_t(sb(sbAddr)->get32(index)); }
inline float getf(uint32_t sbAddr, uint32_t index) { return rw::bitsf(getu(sbAddr, index)); }
inline void set32(uint32_t sbAddr, uint32_t index, int32_t v) { sb(sbAddr)->set32(index, v); }
inline void releaseShared(uint32_t ctrl) { if (ctrl) guest::release(ctrl); }
inline void addRef(uint32_t ctrl) { if (ctrl) guest::addRef(ctrl); }

inline uint32_t partAddr(const RaidBossActor *self, int32_t part) {
    return addr(self) + 948 + 36u * uint32_t(part);
}
inline int32_t partGet(const RaidBossActor *self, int32_t part, uint32_t index) {
    return get32(partAddr(self, part), index);
}
inline void partSet(const RaidBossActor *self, int32_t part, uint32_t index, int32_t v) {
    set32(partAddr(self, part), index, v);
}
// the marker of a part (0 when none): markers.find(part)
inline uint32_t findMarkerNode(const RaidBossActor *self, uint32_t keyAddr) {
    return rw::call(fn::markers_find, {addr(self) + RaidBossActor::MARKERS, keyAddr});
}

// libgcc soft-float conversions as the original uses them
inline uint32_t f2u(float f) {   // __fixunssfsi
    if (f != f || f < 1.0f) return 0;   // NaN, negative, below 1
    if (f >= 4294967296.0f) return 0xFFFFFFFFu;
    return uint32_t(f);
}
// ceil to unsigned the way the original does it: u = (unsigned)f; u + 1 when (float)u < f (__aeabi_fcmplt)
inline uint32_t ceilU(float f) {
    uint32_t u = f2u(f);
    return float(u) < f ? u + 1 : u;
}

}  // namespace rba
