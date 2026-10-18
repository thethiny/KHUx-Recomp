#include "EnemyActor.hpp"

#include "system/BuffStatus.hpp"
#include "battle/MapPrizes.hpp"

namespace {
namespace g {
// game
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t StageState_addLUX = 0x4EECD9;
constexpr uint32_t StageState_addMoney = 0x4EEE7D;
constexpr uint32_t StageState_addRaidpoint = 0x4EED65;
constexpr uint32_t StageState_addEXP = 0x4EEDF1;
constexpr uint32_t StageState_addDefeatedEnemy = 0x4EEF09;
constexpr uint32_t StageState_checkMaxAttack = 0x4EF0C9;
constexpr uint32_t StageState_addEnemyDropItem = 0x4EF289;
constexpr uint32_t master_BattleMisc_create = 0x779AD1;   // (sret shared_ptr, id)
constexpr uint32_t master_Misc_create = 0x786D5D;
constexpr uint32_t master_EnemyAttack_create = 0x780F51;
constexpr uint32_t MapPrizes_Info_ctor = 0x5DB935;        // MapPrizes::Info::Info(Vec2, FromGeneration)
constexpr uint32_t MapPrizes_create = 0x5DE8AD;           // (const Info*, Node*)
constexpr uint32_t MapPrizes_eruption = 0x5DEAE1;         // (prizes, Vec2& from, Vec2& to)
constexpr uint32_t VectorPrizes_pushBack = 0x5DB985;      // cocos2d::Vector<MapPrizes*>::pushBack
constexpr uint32_t VectorPrizes_clear = 0x5DB615;
constexpr uint32_t VectorPrizes_dtor = 0x5DB649;
constexpr uint32_t Enemy_alterMotion = 0x5CC06D;
constexpr uint32_t Enemy_getBattleShift = 0x5CC7C5;
constexpr uint32_t Enemy_preloadResources = 0x5CC689;
constexpr uint32_t FlashNode_reflection_status = 0x5C3171;
constexpr uint32_t FlashNode_set_bad_status = 0x5C2D61;
constexpr uint32_t FlashNode_UpdateStatusDeepPoison = 0x5C3BCD;
constexpr uint32_t FlashNode_UpdateStatusPoison = 0x5C3C55;
constexpr uint32_t FlashNode_UpdateStatusSleep = 0x5C3CD9;
constexpr uint32_t FlashNode_UpdateStatusParalysis = 0x5C3D61;
constexpr uint32_t ShowEffect_show = 0x5D8ADD;
constexpr uint32_t ShowEffect_hide = 0x5D8AF1;
constexpr uint32_t ShowEffect_preloadResources = 0x5D8B89;
constexpr uint32_t Board_create = 0x5E5165;               // RaidBossHPIndicator::create(Enemy_Type, Type, hp, color)
constexpr uint32_t Board_setFieldExtendInfo = 0x5E6F91;   // (board, bool, int, int, const char*)
constexpr uint32_t Board_setHP = 0x5E6305;
constexpr uint32_t Board_set_buf_debuf = 0x5E7D91;
constexpr uint32_t Board_clr_buf_debuf = 0x5E7F41;
constexpr uint32_t Board_Next_Disp = 0x5E7DB9;
constexpr uint32_t Board_set_attack_buff_status = 0x5E7F89;
constexpr uint32_t Board_set_defence_buff_status = 0x5E8189;
constexpr uint32_t Board_switchModeFieldToBattle = 0x5E7729;
constexpr uint32_t Board_setSuitableAsTargetEffect = 0x5E7881;
constexpr uint32_t IStatus_addCondition_common = 0x4F1D21;
constexpr uint32_t IBuff_buff_add = 0x4F256F;
constexpr uint32_t IBuff_buff_level = 0x4F23CD;
constexpr uint32_t IBuff_getBlankBuff = 0x4F2781;
constexpr uint32_t Random_getInstance = 0x492055;
constexpr uint32_t Random_generateUniformInt = 0x492225;     // (rng, min, max, state*)
constexpr uint32_t Random_generateUniformFloat = 0x492261;   // (rng, float min, float max, state*)
constexpr uint32_t BattleMisc_getInstance = 0x7420A9;
constexpr uint32_t BattleMisc_prepareEnemyAttack = 0x7454A1;
constexpr uint32_t AvatarActor_UpdateStatus = 0x7403A9;
constexpr uint32_t StringBuffer_ctor = 0x49140D;             // StringBuffer(const char* fmt, ...)
constexpr uint32_t audio_getInstance = 0x5BECFD;
constexpr uint32_t audio_load = 0x5BEEF5;
constexpr uint32_t audio_getLayer = 0x5BF33D;
constexpr uint32_t audio_playSE = 0x5BF785;
// libc++
constexpr uint32_t fnTouchBool_swap = 0x483E4D;   // std::function<bool(Touch*, Event*)>::swap
constexpr uint32_t fnTouchVoid_swap = 0x4F3699;   // std::function<void(Touch*, Event*)>::swap
constexpr uint32_t fnFloat2_call = 0x740C71;      // std::function<bool(float, float)>::operator()
constexpr uint32_t fnSelect_assign = 0x74D911;    // std::function<void(EnemyActor*, bool)>::operator=
constexpr uint32_t powf_ = 0x41ABFC;              // powf (import)
// cocos2d
constexpr uint32_t Rect_ctor4 = 0x8DF18B;         // Rect(float x, float y, float w, float h)
constexpr uint32_t Rect_assign = 0x8DF1C1;
constexpr uint32_t Rect_getMidX = 0x8DF259;
constexpr uint32_t Rect_getMidY = 0x8DF289;
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Vec2_add = 0x8E4D2D;
constexpr uint32_t Vec2_subtract = 0x8E530F;
constexpr uint32_t Vec2_set2 = 0x8E4BDF;          // Vec2::set(const Vec2& from, const Vec2& to)
constexpr uint32_t Vec2_distance = 0x8E4EF5;
constexpr uint32_t Vec2_normalize = 0x8E503D;
constexpr uint32_t Node_convertToWorldSpace = 0x895A39;
constexpr uint32_t Node_setVisible = 0x893341;
constexpr uint32_t Node_setScaleX = 0x892F39;
constexpr uint32_t Node_unscheduleUpdate = 0x894CB3;
constexpr uint32_t Node_scheduleUpdate = 0x894BA7;
constexpr uint32_t Listener_create = 0x8FACA5;    // EventListenerTouchOneByOne::create()
constexpr uint32_t Listener_setSwallowTouches = 0x8FAC99;
constexpr uint32_t Dispatcher_addSceneGraph = 0x8F3649;
constexpr uint32_t MapNode_pause = 0x5DA57D;
constexpr uint32_t MapNode_resume = 0x5DA597;
constexpr uint32_t MapNode_dtor = 0x5DA38D;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Node_removeAllChildrenWithCleanup = 0x894369;
constexpr uint32_t tapActionNames = 0xF6DD9C;     // const char*[4] marker animation per TapAction
constexpr uint32_t str_TapMarker_Select = 0xE07353; // "TapMarker_Select"
// the touch lambdas of setupTouchRect (each captures this)
constexpr uint32_t touchBeganLambda = 0xF6DE38;
constexpr uint32_t touchMovedLambda = 0xF6DE0C;
constexpr uint32_t touchEndedLambda = 0xF6DDE0;
constexpr uint32_t touchCancelledLambda = 0xF6DDB4;
}  // namespace g

// cocos2d / cocostudio virtual slots used here
constexpr uint32_t SLOT_SET_SCALE_X = 10;
constexpr uint32_t SLOT_SET_POSITION = 19;
constexpr uint32_t SLOT_GET_POSITION = 21;
constexpr uint32_t SLOT_SET_VISIBLE = 44;
constexpr uint32_t SLOT_ADD_CHILD_Z = 61;
constexpr uint32_t SLOT_SET_POSITION_Y = 27;
constexpr uint32_t SLOT_GET_EVENT_DISPATCHER = 106;
constexpr uint32_t SLOT_UPDATE = 113;
constexpr uint32_t SLOT_ARMATURE_GET_ANIMATION = 166;
constexpr uint32_t SLOT_ANIM_STOP = 3;
constexpr uint32_t SLOT_ANIM_PLAY = 26;
constexpr uint32_t SLOT_ANIM_GOTO_AND_PLAY = 31;
constexpr uint32_t SLOT_GET_SCALE_X = 11;
constexpr uint32_t SLOT_REMOVE_ALL_CHILDREN_CLEANUP = 79;

constexpr uint32_t FLASH_STATUS_BITS = 1300;     // FlashNode::statusBits
constexpr uint32_t FLASH_MOTION = 1312;          // Enemy::motion
constexpr uint32_t FLASH_CENTER_Y = 1332;        // Enemy::centerPoint.y
constexpr uint32_t FLASH_EFFECT_Y_PARALYSIS = 660;
constexpr uint32_t FLASH_EFFECT_Y = 664;
constexpr uint32_t FLASH_LWF_NODE = 668;
constexpr uint32_t FLASH_EFFECT_ARMATURE = 1296;
constexpr uint32_t PRIZE_INFO_ITEMS = 8;         // MapPrizes::Info: (kind, count) pairs after the Vec2
constexpr uint32_t PRIZE_EXP = 9, PRIZE_MUNNY_LUX = 8, PRIZE_LUX = 11, PRIZE_MONEY = 4;

using actor::ld;
using actor::ldf;
using actor::ldi;
using actor::ld8;

uint32_t ga(const void *p) { return rw::guest_addr(p); }
uint32_t vc(uint32_t obj, uint32_t slot, std::initializer_list<uint32_t> args) { return actor::vcallAt(obj, slot, args); }
float vcf(uint32_t obj, uint32_t slot, std::initializer_list<uint32_t> args) { return rw::bitsf(vc(obj, slot, args)); }
uint32_t fb(float f) { return rw::fbits(f); }
float bf(uint32_t u) { return rw::bitsf(u); }

// A 32-bit value at data position `index` of the SafeBuffer at guest address sb.
int32_t sb32(uint32_t sb, uint32_t index) { return rw::host_ptr<SafeBuffer>(sb)->get32(index); }

// master::X::create(id) through an sret shared_ptr; the record and its control block.
struct MasterRef {
    uint32_t obj, ctrl;
    int32_t at(uint32_t index) const { return sb32(obj, index); }
    void release() const { bt::releaseShared(ctrl); }
};
MasterRef masterCreate(uint32_t fn, int32_t id) {
    bt::GuestFrame f(8);
    rw::call(fn, {f.addr(), uint32_t(id)});
    return {f.word(0), f.word(4)};
}
// A master value read and the record released right away: (float)value / 10000
float masterRate(uint32_t fn, int32_t id) {
    MasterRef m = masterCreate(fn, id);
    float v = float(m.at(4)) / 10000.0f;
    m.release();
    return v;
}

uint32_t stageState() { return ld(rw::call(g::AppState_getInstance, {}) + 32); }

// The original's round-up of a non-negative float: (unsigned)v, plus one when it was truncated.
uint32_t ceilU(float v) {
    uint32_t u = bt::f2uiz(v);
    return float(u) < v ? u + 1 : u;
}

// The Enemy character: alterMotion(motion), then setPosition(battle shift) (Enemy virtual slot 19).
void alterMotionShifted(uint32_t flash, uint32_t motion) {
    rw::call(g::Enemy_alterMotion, {flash, motion});
    uint32_t fn = ld(ld(flash) + 4 * SLOT_SET_POSITION);
    uint32_t shift = rw::call(g::Enemy_getBattleShift, {flash});
    rw::call(fn, {flash, shift});
}
// alterMotion(motion) and setPosition(Vec2::ZERO)
void alterMotionZero(uint32_t flash, uint32_t motion) {
    rw::call(g::Enemy_alterMotion, {flash, motion});
    vc(flash, SLOT_SET_POSITION, {flash, actor_fn::vec2_ZERO});
}

// The libc++ ~function() of a std::function at f (callable pointer at +16).
void destroyFunction(uint32_t f) {
    uint32_t p = ld(f + 16);
    if (p == f) vc(p, 4, {p});
    else if (p) vc(p, 5, {p});
}
// Swap a lambda capturing `self` (an inline std::function on the guest stack) into dst with `swap`, then destroy
// what the temporary holds afterwards.
void swapInLambda(uint32_t swap, uint32_t dst, uint32_t vtable, uint32_t self) {
    bt::GuestFrame f(24);
    uint32_t fo = f.addr();
    rw::save<uint32_t>(fo + 16, fo);
    rw::save<uint32_t>(fo, vtable);
    rw::save<uint32_t>(fo + 4, self);
    rw::call(swap, {fo, dst});
    destroyFunction(fo);
}

// Copy a Vec2 to a fresh guest temporary
uint32_t vec2Copy(const bt::GuestFrame &f, uint32_t off, uint32_t src) {
    rw::call(g::Vec2_copy, {f.addr(off), src});
    return f.addr(off);
}
}  // namespace

// ------------------------------------------------------------------------------------------- life cycle
EnemyActor *EnemyActor::create(uint32_t info, uint32_t fieldNode) {
    uint32_t obj = rw::call(bt::fn::operator_new, {SIZE});
    auto *self = rw::host_ptr<EnemyActor>(obj);
    construct(self, info);
    rw::call(bt::fn::ref_autorelease, {obj});
    self->fieldNode = fieldNode;
    return self;
}
RW_FUNCTION(0x74AB78, "EnemyActor::create", EnemyActor::create);

namespace {
namespace ctor {
constexpr uint32_t master_Enemy_create = 0x77F7DD;
constexpr uint32_t Enemy_create = 0x5CBE25;
constexpr uint32_t ShowEffect_create = 0x5D84A5;
constexpr uint32_t SafeBuffer_ctor = 0x459F21;       // SafeBuffer(size, registerForUpdate)
constexpr uint32_t SafeBuffer_zerofill = 0x45A605;
constexpr uint32_t MapNode_ctor = 0x5DA2D5;
constexpr uint32_t Size_ctor = 0x8DF01F;
constexpr uint32_t Vec2_ctor = 0x8E4B95;
constexpr uint32_t Rect_ctor = 0x8DF171;
constexpr uint32_t Node_create = 0x89372D;
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t memset_ = 0x40E7A0;               // __aeabi_memset(dst, n, c)
constexpr uint32_t masterId2ConditionType = 0x4F1FC1;
constexpr uint32_t pushBackHitEffectID = 0x74A26D;   // FlashNode::pushBackHitEffectID
constexpr uint32_t fnInt_swap = 0x5428E1;            // std::function<void(int)>::swap
constexpr uint32_t adm_getInstance = 0x7AC9B5;
constexpr uint32_t adm_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t setMovementEventCallFunc = 0x7A3C69;
constexpr uint32_t str_exportJson = 0xE01EED;        // "cocostudio/publish/%s.ExportJson"
constexpr uint32_t str_ActMapTapAnim = 0xE01F70;     // "ActMapTapAnim"
constexpr uint32_t showLambda1 = 0xF6DEBC;           // ShowEffect +648 callback (captures this, param)
constexpr uint32_t showLambda2 = 0xF6DE90;           // ShowEffect +672 callback
constexpr uint32_t markerMovementLambda = 0xF6DE64;  // tap marker movement event (captures this)
constexpr uint32_t SHOW_CALLBACK1 = 648, SHOW_CALLBACK2 = 672;
constexpr uint32_t SLOT_SET_TAG = 83, SLOT_4 = 4, SLOT_SET_POSITION_X = 25, SLOT_SET_POSITION_XY = 23;
constexpr uint32_t SLOT_ADD_CHILD = 60;
}  // namespace ctor

// The master::Enemy columns interpolated by level: 8 int[5] tables (level, then HP and the prize values).
constexpr uint32_t LEVEL_TABLES = 8;
}  // namespace

EnemyActor *EnemyActor::construct(EnemyActor *self, uint32_t info) {
    uint32_t s = self->self();
    rw::call(ctor::MapNode_ctor, {s});
    self->vtable = VTABLE;
    self->field_28D = 0;
    self->inBattle = 0;
    self->symbolSet = 0;
    self->boardHidden = 0;
    self->isBoss = 0;
    self->field_288 = 0;
    self->selectWait = 0;
    self->teamId = -1;
    self->teamIndex = -1;
    self->showState = 0;
    self->state = NONE;
    self->fieldNode = 0;
    self->statusBits = 0;
    rw::call(ctor::SafeBuffer_ctor, {ga(&self->property), 0x368, 1});
    self->property.vtable = PROPERTY_VTABLE;
    self->statusVtable = STATUS_VTABLE;
    self->buffVtable = BUFF_VTABLE;
    self->field_2D0 = 0;
    self->field_2D4 = 0;
    self->masterEnemy = 0;
    self->masterEnemyCtrl = 0;
    rw::call(ctor::Size_ctor, {ga(&self->size)});
    self->symbolKind = 1;
    self->moveType = 1;
    rw::call(ctor::Vec2_ctor, {ga(&self->homePosition)});
    rw::call(ctor::Vec2_ctor, {ga(&self->moveDir)});
    self->flash = 0;
    self->showEffect = 0;
    self->deadType = 0;
    rw::set<uint16_t>(self, 788, 0);
    self->moveTimer = 0.0f;
    self->wanderDir = 0;
    rw::call(ctor::Rect_ctor, {ga(&self->touchOrigin)});
    for (uint32_t off : {872u, 896u, 920u, 944u}) rw::set<uint32_t>(self, off, 0);   // the std::functions are empty
    self->tapMarker = 0;
    self->selector = 0;
    self->hpBoard = 0;
    rw::call(g::Vec2_copy, {ga(&self->hpBoardPos), actor_fn::vec2_ZERO});
    self->field_3CC = 0;
    rw::call(ctor::SafeBuffer_ctor, {ga(&self->prizeParam), 8, 1});
    self->prizeParam.vtable = PRIZE_PARAM_VTABLE;
    rw::call(ctor::memset_, {s + 1008, 0x20, 0});
    rw::call(ctor::SafeBuffer_zerofill, {ga(&self->property)});

    // the master record: the info's variant id, else its enemy id
    int32_t id = rw::load<int32_t>(info + 4);
    int32_t level = rw::load<int32_t>(info + 8);
    if (!id) id = rw::load<int32_t>(info);
    {
        bt::GuestFrame f(8);
        rw::call(ctor::master_Enemy_create, {f.addr(), uint32_t(id)});
        uint32_t obj = f.word(0), ctrl = f.word(4);
        f.setWord(0, 0);
        f.setWord(4, 0);
        self->masterEnemy = obj;
        uint32_t old = self->masterEnemyCtrl;
        self->masterEnemyCtrl = ctrl;
        if (old) {
            rw::call(bt::fn::release_shared, {old});
            if (f.word(4)) rw::call(bt::fn::release_shared, {f.word(4)});
        }
    }
    vc(s, ctor::SLOT_SET_TAG, {s, 2});
    self->overlay = rw::call(ctor::Node_create, {});
    rw::call(ctor::Ref_retain, {self->overlay});
    vc(self->overlay, ctor::SLOT_4, {self->overlay, 100});
    vc(self->overlay, SLOT_SET_VISIBLE, {self->overlay, 0});

    // the per-level values: find the level's segment in column 0, interpolate the other columns linearly
    SafeBuffer *m = rw::host_ptr<SafeBuffer>(self->masterEnemy);
    int32_t count = m->get32(M_LEVEL_COUNT);
    bt::GuestFrame tf(LEVEL_TABLES * 20 + 32);   // like the original's frame, column 0 last (highest)
    auto column = [&](uint32_t c) { return tf.addr((LEVEL_TABLES - 1 - c) * 20); };
    for (uint32_t c = 0; c < LEVEL_TABLES; c++) {
        uint32_t index = M_LEVEL_TABLE + 20 * c;
        SafeBuffer::read(m, rw::host_ptr<uint8_t>(column(c)), m->data.get() + index, 20, index);
    }
    uint32_t k = 0;
    int32_t hi;
    for (int32_t i = 1;; i++) {
        hi = ldi(column(0) + 4 + k);
        k += 4;
        if (i >= count || hi >= level) break;
    }
    int32_t lo = ldi(column(0) + k - 4);
    float t = float(int32_t(uint32_t(level) - uint32_t(lo))) / float(int32_t(uint32_t(hi) - uint32_t(lo)));
    for (uint32_t c = 1; c < LEVEL_TABLES; c++) {
        int32_t a = ldi(column(c) + k - 4);
        int32_t b = ldi(column(c) + k);
        int32_t v = bt::f2iz(float(a) + t * float(int32_t(uint32_t(b) - uint32_t(a))));
        self->property.set32(4 + 4 * c, v);   // HP, then the prize values at 12..32
    }
    self->property.set32(P_HP_MIN, 0);
    self->property.set32(P_HP_MAX, self->prop32(P_HP));
    SafeBuffer::write(&self->property, self->property.data.get() + P_DROP_ID, rw::host_ptr<uint8_t>(info + 16), 4, P_DROP_ID);
    SafeBuffer::write(&self->property, self->property.data.get() + P_DROP_COUNT, rw::host_ptr<uint8_t>(info + 20), 4, P_DROP_COUNT);
    SafeBuffer::write(&self->property, self->property.data.get() + P_TEAM, rw::host_ptr<uint8_t>(info + 12), 4, P_TEAM);
    uint8_t zero = 0;
    SafeBuffer::write(&self->property, self->property.data.get() + P_DEFEATED, &zero, 1, P_DEFEATED);

    // the character, centred on a holder node
    self->flash = rw::call(ctor::Enemy_create, {uint32_t(self->master32(M_CHARACTER))});
    vc(self->flash, SLOT_SET_VISIBLE, {self->flash, 0});
    uint32_t holder = rw::call(ctor::Node_create, {});
    vc(holder, ctor::SLOT_SET_POSITION_X, {holder, ld(self->flash + 1328) ^ 0x80000000u});
    vc(holder, ctor::SLOT_ADD_CHILD, {holder, self->flash});
    vc(s, ctor::SLOT_ADD_CHILD, {s, holder});
    self->showState = 0;

    // the hit effect of every attack slot (while bit k of the attack mask is set)
    uint32_t mask = uint32_t(self->master32(M_ATTACK_MASK));
    {
        bt::GuestFrame af(16);
        SafeBuffer::read(m, rw::host_ptr<uint8_t>(af.addr()), m->data.get() + M_ATTACKS, 16, M_ATTACKS);
        for (uint32_t k2 = 0; mask & (1u << k2); k2++) {
            bt::GuestFrame sp(8);
            rw::call(g::master_EnemyAttack_create, {sp.addr(), af.word(4 * k2)});
            uint32_t flash = self->flash;
            int32_t effect = sb32(sp.word(0), 76);
            rw::call(ctor::pushBackHitEffectID, {flash, uint32_t(effect)});
            bt::releaseShared(sp.word(4));
        }
    }
    // initial buffs and bad statuses
    int32_t bp[10];
    for (uint32_t k2 = 0; k2 < 10; k2++) bp[k2] = self->master32(936 + 4 * k2);
    uint8_t flag = 0;
    int32_t cond1 = self->master32(976);
    int32_t cond2 = self->master32(980);
    int32_t type1 = -1, type2 = -1;
    if (cond1) {
        guest::Frame ff(8);
        type1 = int32_t(rw::call(ctor::masterId2ConditionType, {s + 712, uint32_t(cond1), ff.addr()}));
    }
    if (cond2) {
        guest::Frame ff(8);
        type2 = int32_t(rw::call(ctor::masterId2ConditionType, {s + 712, uint32_t(cond2), ff.addr()}));
    }
    (void)flag;
    int32_t condTurns = self->master32(984);
    int32_t condCount = self->master32(988);
    set_buff_param(self, bp[0], bp[1], bp[2], bp[3], bp[4], bp[5], bp[6], bp[7], bp[8], bp[9], type1, type2, condTurns,
                   condCount);
    int32_t kind = self->master32(M_ATTACK_TYPE);
    if (kind == 3) self->attackType = 2;
    else if (kind == 2) self->attackType = 1;
    else if (kind == 1) self->attackType = 0;
    int32_t param1 = self->master32(M_SHOW_PARAM1);
    int32_t param2 = self->master32(M_SHOW_PARAM2);
    int32_t show = self->master32(M_SHOW_EFFECT);
    if (self->attackType == 2) {
        vc(self->flash, SLOT_SET_VISIBLE, {self->flash, 1});
    } else if (show) {
        uint32_t effect = rw::call(ctor::ShowEffect_create, {uint32_t(show)});
        self->showEffect = effect;
        auto install = [&](uint32_t vtable, int32_t param, uint32_t dst) {
            bt::GuestFrame lf(24);
            uint32_t fo = lf.addr();
            rw::save<uint32_t>(fo + 16, fo);
            rw::save<uint32_t>(fo, vtable);
            rw::save<uint32_t>(fo + 8, uint32_t(param));
            rw::save<uint32_t>(fo + 4, s);
            rw::call(ctor::fnInt_swap, {fo, dst});
            destroyFunction(fo);
        };
        install(ctor::showLambda1, param1, effect + ctor::SHOW_CALLBACK1);
        install(ctor::showLambda2, param2, self->showEffect + ctor::SHOW_CALLBACK2);
        uint32_t se = self->showEffect;
        if (self->attackType == 1) vc(se, ctor::SLOT_SET_POSITION_XY, {se, 0, 0});
        else vc(se, SLOT_SET_POSITION_Y, {se, ld(self->flash + FLASH_CENTER_Y)});
        se = self->showEffect;
        uint32_t setScaleX = ld(ld(se) + 4 * SLOT_SET_SCALE_X);
        uint32_t sx = vc(s, SLOT_GET_SCALE_X, {s});
        rw::call(setScaleX, {se, sx});
        vc(s, ctor::SLOT_ADD_CHILD, {s, self->showEffect});
    }
    self->speed = float(self->master32(M_SPEED)) * 0.5f;
    self->touchSize = float(self->master32(M_TOUCH_SIZE));
    self->encountRange = float(self->master32(M_ENCOUNT_RANGE));
    self->size.x = float(self->master32(M_WIDTH));
    self->size.y = float(self->master32(M_HEIGHT));
    self->mapPosition.y = rw::load<float>(info + 28);
    self->mapPosition.x = rw::load<float>(info + 24);
    self->homePosition.x = rw::load<float>(info + 24);
    self->homePosition.y = rw::load<float>(info + 28);
    float touch = self->touchSize;
    setupTouchRect(self, touch + touch, touch + touch);

    // the tap marker and the target selector (ActMapTapAnim armatures)
    {
        uint32_t adm = rw::call(ctor::adm_getInstance, {});
        guest::Frame f(1056 + 16);
        uint32_t sb = f.addr(16), str = f.addr(0);
        rw::call(g::StringBuffer_ctor, {sb, ctor::str_exportJson, ctor::str_ActMapTapAnim});
        uint32_t len = rw::call(actor_fn::strlen_, {sb});
        actor::makeString(str, sb, len);
        rw::call(ctor::adm_addArmatureFileInfo, {adm, str});
        actor::freeString(str);
    }
    {
        bt::GuestFrame f(16);
        actor::makeString(f.addr(), ctor::str_ActMapTapAnim, 13);
        uint32_t a = rw::call(ctor::Armature_create, {f.addr()});
        self->tapMarker = a;
        if (rw::load<uint8_t>(f.addr()) & 1) {
            rw::call(bt::fn::operator_delete, {f.word(8)});
            a = self->tapMarker;
        }
        if (a) {
            uint32_t anim = vc(a, SLOT_ARMATURE_GET_ANIMATION, {a});
            bt::GuestFrame lf(24);
            uint32_t fo = lf.addr();
            rw::save<uint32_t>(fo + 16, fo);
            rw::save<uint32_t>(fo, ctor::markerMovementLambda);
            rw::save<uint32_t>(fo + 4, s);
            rw::call(ctor::setMovementEventCallFunc, {anim, fo});
            destroyFunction(fo);
            vc(self->tapMarker, SLOT_SET_POSITION_Y, {self->tapMarker, ld(self->flash + FLASH_CENTER_Y)});
            vc(self->overlay, SLOT_ADD_CHILD_Z, {self->overlay, self->tapMarker, 1});
            uint32_t an = vc(self->tapMarker, SLOT_ARMATURE_GET_ANIMATION, {self->tapMarker});
            vc(an, SLOT_ANIM_STOP, {an});
            vc(self->tapMarker, SLOT_SET_VISIBLE, {self->tapMarker, 0});
        }
    }
    {
        bt::GuestFrame f(16);
        actor::makeString(f.addr(), ctor::str_ActMapTapAnim, 13);
        uint32_t a = rw::call(ctor::Armature_create, {f.addr()});
        self->selector = a;
        if (rw::load<uint8_t>(f.addr()) & 1) {
            rw::call(bt::fn::operator_delete, {f.word(8)});
            a = self->selector;
        }
        vc(self->tapMarker, SLOT_ADD_CHILD_Z, {self->tapMarker, a, 1});
        vc(self->selector, SLOT_SET_VISIBLE, {self->selector, 0});
        uint32_t an = vc(self->selector, SLOT_ARMATURE_GET_ANIMATION, {self->selector});
        vc(an, SLOT_ANIM_STOP, {an});
        vc(self->selector, SLOT_SET_VISIBLE, {self->selector, 0});
    }
    {
        bt::GuestFrame f(8);
        f.setWord(0, 0);
        f.setWord(4, 0);
        setPrizeParam(self, 1.0f, 1.0f, rw::frame_ptr<bt::SharedPtr>(f.addr()));
    }
    self->state = MAP;
    for (uint32_t k2 = 0; k2 < 4; k2++) {
        self->cooldown[k2] = 0;
        self->used[k2] = 0;
    }
    return self;
}
RW_FUNCTION(0x749180, "EnemyActor::EnemyActor(EnemyActorInfo*)", EnemyActor::construct);

EnemyActor *EnemyActor::destruct(EnemyActor *self) {
    uint32_t s = self->self();
    self->vtable = VTABLE;
    rw::call(g::Node_removeAllChildrenWithCleanup, {s, 1});
    vc(self->overlay, SLOT_REMOVE_ALL_CHILDREN_CLEANUP, {self->overlay, 1});
    if (self->overlay) rw::call(g::Ref_release, {self->overlay});
    rw::call(g::VectorPrizes_dtor, {ga(&self->prizes)});
    bt::releaseShared(self->skillCtrl);
    SafeBuffer::destruct(&self->prizeParam);
    rw::call(g::Vec2_dtor, {ga(&self->hpBoardPos)});
    destroyFunction(ga(self->canMoveTo));
    destroyFunction(ga(self->onSelect));
    destroyFunction(ga(self->onTapped));
    destroyFunction(ga(self->onTouch));
    rw::call(g::Vec2_dtor, {ga(&self->touchOrigin)});
    rw::call(g::Vec2_dtor, {ga(&self->moveDir)});
    rw::call(g::Vec2_dtor, {ga(&self->homePosition)});
    bt::releaseShared(self->masterEnemyCtrl);
    SafeBuffer::destruct(&self->property);
    rw::call(g::MapNode_dtor, {s});
    return self;
}
RW_FUNCTION(0x74A920, "EnemyActor::~EnemyActor", EnemyActor::destruct);

void EnemyActor::destructDelete(EnemyActor *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {self->self()});
}
RW_FUNCTION(0x74AB6C, "EnemyActor::~EnemyActor (deleting)", EnemyActor::destructDelete);

// ------------------------------------------------------------------------------------------- setup
void EnemyActor::set_buff_param(EnemyActor *self, int32_t atk0, int32_t def0, int32_t atk1, int32_t def1,
                                int32_t atk2, int32_t def2, int32_t atk3, int32_t def3, int32_t turns,
                                int32_t count, int32_t condition1, int32_t condition2, int32_t condTurns,
                                int32_t condCount) {
    uint32_t s = self->self();
    if (uint32_t(turns) + uint32_t(count)) {
        uint32_t buffs = s + 716;
        auto add = [&](int32_t value, uint32_t type, uint32_t target) {
            rw::call(g::IBuff_buff_add, {buffs, uint32_t(value), uint32_t(turns), uint32_t(count), type, target});
        };
        add(atk0, 0, 0);
        add(def0, 1, 0);
        add(atk1, 0, 1);
        add(def1, 1, 1);
        add(atk2, 0, 2);
        add(def2, 1, 2);
        add(atk3, 0, 3);
        add(def3, 1, 3);
    }
    if (uint32_t(condTurns) + uint32_t(condCount)) {
        if (condition1 != -1)
            rw::call(g::IStatus_addCondition_common, {s + 712, uint32_t(condition1), uint32_t(condTurns), uint32_t(condCount)});
        if (condition2 != -1)
            rw::call(g::IStatus_addCondition_common, {s + 712, uint32_t(condition2), uint32_t(condTurns), uint32_t(condCount)});
    }
    rw::call(g::FlashNode_reflection_status, {self->flash, 1});
}
RW_FUNCTION(0x74A2DC, "EnemyActor::set_buff_param", EnemyActor::set_buff_param);

EnemyActor *EnemyActor::initMapPosition(EnemyActor *self, const bt::Vec2 *pos) {
    self->mapPosition.x = pos->x.get();
    self->mapPosition.y = pos->y.get();
    self->homePosition.x = pos->x.get();
    self->homePosition.y = pos->y.get();
    return self;
}
RW_FUNCTION(0x74A442, "EnemyActor::initMapPosition", EnemyActor::initMapPosition);

void EnemyActor::setupTouchRect(EnemyActor *self, float width, float height) {
    uint32_t s = self->self();
    {
        bt::GuestFrame f(16);
        float x = 0.0f - width * 0.5f;
        float y = ldf(self->flash + FLASH_CENTER_Y) - height * 0.5f;
        rw::call(g::Rect_ctor4, {f.addr(), fb(x), fb(y), fb(width), fb(height)});
        rw::call(g::Rect_assign, {ga(&self->touchOrigin), f.addr()});
        rw::call(g::Vec2_dtor, {f.addr()});
    }
    uint32_t listener = rw::call(g::Listener_create, {});
    rw::call(g::Listener_setSwallowTouches, {listener, 0});
    swapInLambda(g::fnTouchBool_swap, listener + 0x50, g::touchBeganLambda, s);
    swapInLambda(g::fnTouchVoid_swap, listener + 0x68, g::touchMovedLambda, s);
    swapInLambda(g::fnTouchVoid_swap, listener + 0x80, g::touchEndedLambda, s);
    swapInLambda(g::fnTouchVoid_swap, listener + 0x98, g::touchCancelledLambda, s);
    uint32_t dispatcher = vc(s, SLOT_GET_EVENT_DISPATCHER, {s});
    rw::call(g::Dispatcher_addSceneGraph, {dispatcher, listener, s});
}
RW_FUNCTION(0x74A468, "EnemyActor::setupTouchRect", EnemyActor::setupTouchRect);

// The tap marker plays the animation of `action` (0..3); any other value stops and hides it.
void EnemyActor::actionMarker(EnemyActor *self, uint32_t action) {
    uint32_t anim = vc(self->tapMarker, SLOT_ARMATURE_GET_ANIMATION, {self->tapMarker});
    if (action > 3) {
        vc(anim, SLOT_ANIM_STOP, {anim});
        vc(self->tapMarker, SLOT_SET_VISIBLE, {self->tapMarker, 0});
        return;
    }
    uint32_t name = ld(g::tapActionNames + 4 * action);
    uint32_t play = ld(ld(anim) + 4 * SLOT_ANIM_PLAY);
    bt::GuestFrame f(16);
    uint32_t len = rw::call(actor_fn::strlen_, {name});
    actor::makeString(f.addr(), name, len);
    rw::call(play, {anim, f.addr(), 0xFFFFFFFFu, 0xFFFFFFFFu});
    actor::freeString(f.addr());
    uint32_t a2 = vc(self->tapMarker, SLOT_ARMATURE_GET_ANIMATION, {self->tapMarker});
    vc(a2, SLOT_ANIM_GOTO_AND_PLAY, {a2, 0});
    vc(self->tapMarker, SLOT_SET_VISIBLE, {self->tapMarker, 1});
}
RW_FUNCTION(0x74A65C, "EnemyActor::actionMarker", EnemyActor::actionMarker);

void EnemyActor::actionSelector(EnemyActor *self, uint32_t on) {
    uint32_t anim = vc(self->selector, SLOT_ARMATURE_GET_ANIMATION, {self->selector});
    if ((on & 0xFF) == 1) {
        uint32_t play = ld(ld(anim) + 4 * SLOT_ANIM_PLAY);
        bt::GuestFrame f(16);
        actor::makeString(f.addr(), g::str_TapMarker_Select, 16);
        rw::call(play, {anim, f.addr(), 0xFFFFFFFFu, 0xFFFFFFFFu});
        actor::freeString(f.addr());
        uint32_t a2 = vc(self->selector, SLOT_ARMATURE_GET_ANIMATION, {self->selector});
        vc(a2, SLOT_ANIM_GOTO_AND_PLAY, {a2, 0});
        vc(self->selector, SLOT_SET_VISIBLE, {self->selector, 1});
    } else {
        vc(anim, SLOT_ANIM_STOP, {anim});
        vc(self->selector, SLOT_SET_VISIBLE, {self->selector, 0});
    }
}
RW_FUNCTION(0x74A774, "EnemyActor::actionSelector", EnemyActor::actionSelector);

void EnemyActor::setPrizeParam(EnemyActor *self, float attackRate, float crushRate, bt::SharedPtr *skill) {
    uint32_t a = fb(attackRate), c = fb(crushRate);
    SafeBuffer::write(&self->prizeParam, self->prizeParam.data.get(), reinterpret_cast<const uint8_t *>(&a), 4, 0);
    SafeBuffer::write(&self->prizeParam, self->prizeParam.data.get() + 4, reinterpret_cast<const uint8_t *>(&c), 4, 4);
    uint32_t obj = skill->obj, ctrl = skill->ctrl;
    if (ctrl) guest::addRef(ctrl);
    self->skill = obj;
    uint32_t old = self->skillCtrl;
    self->skillCtrl = ctrl;
    bt::releaseShared(old);
}
RW_FUNCTION(0x74A874, "EnemyActor::setPrizeParam", EnemyActor::setPrizeParam);

void EnemyActor::setSymbolInfo(EnemyActor *self, int32_t kind, int32_t moveType, uint32_t boss, uint32_t big) {
    self->symbolKind = kind;
    self->moveType = moveType;
    self->isBoss = uint8_t(boss);
    self->symbolSet = 1;
    if ((boss & 0xFF) == 1 && (big & 0xFF) == 1) {
        // a big symbol: max HP x BattleMisc 1100 / 10000 becomes the HP floor (data position 0)
        float rate = masterRate(g::master_BattleMisc_create, 1100);
        int32_t maxHp = self->prop32(P_HP_MAX);
        int32_t v = bt::f2iz(rate * float(maxHp));
        self->property.set32(P_HP_MIN, v);
    }
    self->state = SYMBOL;
}
RW_FUNCTION(0x74ABC8, "EnemyActor::setSymbolInfo", EnemyActor::setSymbolInfo);

EnemyActor *EnemyActor::setTeamInfo(EnemyActor *self, int32_t team, int32_t index) {
    self->teamId = team;
    self->teamIndex = index;
    return self;
}
RW_FUNCTION(0x74ACDC, "EnemyActor::setTeamInfo", EnemyActor::setTeamInfo);

void EnemyActor::setStatusBoard(EnemyActor *self, int32_t a, int32_t b) {
    int32_t gauges = self->master32(M_HP_GAUGES);
    uint8_t boss = self->isBoss;
    int32_t maxHp = self->prop32(P_HP_MAX);
    int32_t type = gauges - 1;
    if (boss) {
        MasterRef m = masterCreate(g::master_Misc_create, 202);
        int32_t color = m.at(4);
        self->hpBoard = rw::call(g::Board_create, {2, uint32_t(type), uint32_t(maxHp), uint32_t(color)});
        m.release();
    } else {
        MasterRef m = masterCreate(g::master_Misc_create, 201);
        int32_t color = m.at(4);
        self->hpBoard = rw::call(g::Board_create, {1, uint32_t(type), uint32_t(maxHp), uint32_t(color)});
        m.release();
    }
    int32_t boardY = self->master32(M_HP_BOARD_POS);
    float y;
    if (self->hpBoard) {
        int32_t kind = self->symbolKind;
        // the name (char[65]) as a StringBuffer format, like the original
        guest::Frame f(0x48 + 1060);
        uint32_t name = f.addr(0), sbuf = f.addr(0x48);
        SafeBuffer *m = rw::host_ptr<SafeBuffer>(self->masterEnemy);
        SafeBuffer::read(m, rw::host_ptr<uint8_t>(name), m->data.get() + M_NAME, 0x41, M_NAME);
        rw::call(g::StringBuffer_ctor, {sbuf, name});
        uint32_t flag = (kind == 2) | (kind == 4);
        rw::call(g::Board_setFieldExtendInfo, {self->hpBoard, flag, uint32_t(a), uint32_t(b), sbuf});
        vc(self->hpBoard, SLOT_SET_VISIBLE, {self->hpBoard, 0});
        y = float(boardY);
        vc(self->hpBoard, SLOT_SET_POSITION_Y, {self->hpBoard, fb(y)});
        uint32_t pos = vc(self->hpBoard, SLOT_GET_POSITION, {self->hpBoard});
        uint32_t px = ld(pos), py = ld(pos + 4);
        self->hpBoardPos.y = bf(py);
        self->hpBoardPos.x = bf(px);
        vc(self->overlay, SLOT_ADD_CHILD_Z, {self->overlay, self->hpBoard, 10});
    } else {
        y = float(boardY);
    }
    uint32_t flash = self->flash;
    rw::save<uint32_t>(flash + FLASH_EFFECT_Y_PARALYSIS, ld(flash + FLASH_CENTER_Y));
    rw::save<float>(self->flash + FLASH_EFFECT_Y, y);
}
RW_FUNCTION(0x74ACEC, "EnemyActor::setStatusBoard", EnemyActor::setStatusBoard);

// ------------------------------------------------------------------------------------------- attacks
// Whether attack slot `slot` may be used now (hpRate: the player's HP ratio for kind 3).
bool EnemyActor::use_skill_check(EnemyActor *self, int32_t slot, float hpRate) {
    SafeBuffer *m = rw::host_ptr<SafeBuffer>(self->masterEnemy);
    int32_t kind = m->get32(M_SKILL_KIND + 4u * uint32_t(slot));
    int32_t param = m->get32(M_SKILL_PARAM + 4u * uint32_t(slot));
    int32_t hp = self->prop32(P_HP);
    int32_t maxHp = self->prop32(P_HP_MAX);
    float rate = float(param) / 10000.0f;
    float ratio = float(hp) / float(maxHp);
    switch (kind) {
    case 2: return true;                         // always
    case 3: return rate > hpRate;                // the player's HP is low
    case 4: return ratio < rate;                 // own HP is low
    case 5: return self->used[slot] ? false : ratio < rate;   // once, own HP is low
    case 6: return self->cooldown[slot] == 0;    // every n turns
    default: return false;
    }
}
RW_FUNCTION(0x74AF40, "EnemyActor::use_skill_check", EnemyActor::use_skill_check);

void EnemyActor::use_skill(EnemyActor *self, int32_t slot) {
    SafeBuffer *m = rw::host_ptr<SafeBuffer>(self->masterEnemy);
    int32_t kind = m->get32(M_SKILL_KIND + 4u * uint32_t(slot));
    int32_t param = m->get32(M_SKILL_PARAM + 4u * uint32_t(slot));
    if (kind == 6) self->cooldown[slot] = param / 10000;
    else if (kind == 5) self->used[slot] = 1;
}
RW_FUNCTION(0x74B114, "EnemyActor::use_skill", EnemyActor::use_skill);

int32_t EnemyActor::select_enemy_attack(EnemyActor *self, float hpRate, uint32_t manager) {
    uint32_t s = self->self();
    int32_t count = self->master32(M_ATTACK_MASK);
    self->attackSlot = 0;
    if (count >= 1) {
        // every slot's cooldown runs down
        for (int32_t k = 0; k < count; k++) {
            uint32_t a = s + 796 + 4u * uint32_t(k);
            uint32_t v = ld(a);
            if (v) rw::save<uint32_t>(a, v - 1);
        }
        // the highest usable slot that wins its chance roll; slot 0 otherwise
        for (int32_t k = count - 1;; k--) {
            if (use_skill_check(self, k, hpRate)) {
                int32_t rate = self->master32(M_SKILL_RATE + 4u * uint32_t(k));
                uint32_t rng = rw::call(g::Random_getInstance, {});
                int32_t roll = int32_t(rw::call(g::Random_generateUniformInt, {rng, 0, 9999, 0}));
                if (rate > roll) {
                    self->attackSlot = k;
                    break;
                }
            }
            if (k + 1 <= 1) break;
        }
    }
    use_skill(self, self->attackSlot);
    int32_t kind;
    {
        bt::GuestFrame f(8);
        getEnemyAttackMaster(rw::frame_ptr<bt::SharedPtr>(f.addr()), self);
        kind = sb32(f.word(0), 72);
        bt::releaseShared(f.word(4));
    }
    alterMotionShifted(self->flash, kind == 2 ? 4 : 3);
    uint32_t misc = rw::call(g::BattleMisc_getInstance, {});
    rw::call(g::BattleMisc_prepareEnemyAttack, {misc, s, ld(manager + 0x20), manager});
    rw::call(g::AvatarActor_UpdateStatus, {ld(manager + 0x20)});
    UpdateStatus(self);
    return self->attackSlot;
}
RW_FUNCTION(0x74B1D0, "EnemyActor::select_enemy_attack", EnemyActor::select_enemy_attack);

void EnemyActor::getEnemyAttackMaster(bt::SharedPtr *out, const EnemyActor *self) {
    int32_t id = self->master32(M_ATTACKS + 4u * uint32_t(self->attackSlot.get()));
    rw::call(g::master_EnemyAttack_create, {ga(out), uint32_t(id)});
}
RW_FUNCTION(0x74B378, "EnemyActor::getEnemyAttackMaster", EnemyActor::getEnemyAttackMaster);

void EnemyActor::changeMotion(EnemyActor *self, uint32_t motion) {
    rw::call(g::Enemy_alterMotion, {self->flash, motion});
    uint32_t flash = self->flash;
    uint32_t fn = ld(ld(flash) + 4 * SLOT_SET_POSITION);
    // wait, bwait and dead stand at the origin; the other motions at the battle shift
    uint32_t pos = (motion <= 6 && ((1u << motion) & 0x45)) ? actor_fn::vec2_ZERO
                                                             : rw::call(g::Enemy_getBattleShift, {flash});
    rw::call(fn, {flash, pos});
}
RW_FUNCTION(0x74B3F0, "EnemyActor::changeMotion", EnemyActor::changeMotion);

// Shows the bad statuses and buff levels on the character and the HP board.
void EnemyActor::UpdateStatus(EnemyActor *self) {
    uint32_t s = self->self();
    uint32_t flash = self->flash;
    self->statusBits = ld(flash + FLASH_STATUS_BITS);
    uint32_t status = s + 712;
    bt::GuestFrame f(16);
    uint32_t out = f.addr();
    auto condition = [&](uint32_t type) {
        rw::call(ld(ld(status)), {out, status, type});    // getCondition (sret)
        return ld(out + 8);
    };
    struct Row { uint32_t type, icon, bit, update; };
    const Row rows[4] = {{0, 16, 1, g::FlashNode_UpdateStatusPoison},
                         {3, 19, 8, g::FlashNode_UpdateStatusDeepPoison},
                         {1, 17, 2, g::FlashNode_UpdateStatusSleep},
                         {2, 18, 4, g::FlashNode_UpdateStatusParalysis}};
    for (const Row &r : rows) {
        uint32_t id = condition(r.type);
        uint32_t board = self->hpBoard;
        if (id) {
            rw::call(g::Board_set_buf_debuf, {board, r.icon, 21});
            if (ld8(s + 664) & r.bit) continue;
            rw::call(r.update, {flash, 1});
        } else {
            rw::call(g::Board_clr_buf_debuf, {board, r.icon});
            rw::call(r.update, {flash, 0});
        }
    }
    if (!ld(flash + FLASH_STATUS_BITS)) rw::call(g::FlashNode_set_bad_status, {flash, 4});
    uint32_t buffs = s + 716;
    uint32_t lv[8];
    for (uint32_t k = 0; k < 8; k++) lv[k] = rw::call(g::IBuff_buff_level, {buffs, k >> 2, k & 3});
    rw::call(g::Board_set_attack_buff_status, {self->hpBoard, lv[0], lv[1], lv[2], lv[3]});
    rw::call(g::Board_set_defence_buff_status, {self->hpBoard, lv[4], lv[5], lv[6], lv[7]});
    rw::call(g::Board_Next_Disp, {self->hpBoard});
}
RW_FUNCTION(0x74B440, "EnemyActor::UpdateStatus", EnemyActor::UpdateStatus);

int32_t EnemyActor::getHitEffectID(EnemyActor *self) {
    bt::GuestFrame f(8);
    getEnemyAttackMaster(rw::frame_ptr<bt::SharedPtr>(f.addr()), self);
    int32_t id = sb32(f.word(0), 76);
    bt::releaseShared(f.word(4));
    return id;
}
RW_FUNCTION(0x74B67C, "EnemyActor::getHitEffectID", EnemyActor::getHitEffectID);

// 1.0 for attacks that always hit from the right (direction 1), else the actor's x scale (its facing).
float EnemyActor::getHitEffectDirection(EnemyActor *self) {
    bt::GuestFrame f(8);
    getEnemyAttackMaster(rw::frame_ptr<bt::SharedPtr>(f.addr()), self);
    int32_t dir = sb32(f.word(0), 80);
    uint32_t r = 0x3F800000u;
    if (dir != 1) r = vc(self->self(), SLOT_GET_SCALE_X, {self->self()});
    bt::releaseShared(f.word(4));
    return bf(r);
}
RW_FUNCTION(0x74B6FC, "EnemyActor::getHitEffectDirection", EnemyActor::getHitEffectDirection);

// ------------------------------------------------------------------------------------------- damage, prizes
bool EnemyActor::addDamage(EnemyActor *self, uint32_t damage, const bt::SharedPtr *burst) {
    uint32_t s = self->self();
    int32_t hp = self->prop32(P_HP);
    int32_t floor = self->prop32(P_HP_MIN);
    int32_t left = int32_t(uint32_t(hp) - damage);
    if (floor >= left) left = floor;
    self->property.set32(P_HP, left);
    rw::call(g::StageState_checkMaxAttack, {stageState(), damage});
    int32_t dealt = int32_t(uint32_t(hp) - uint32_t(left));
    bool killed = false;
    uint8_t counted = 0;
    if (left == floor) SafeBuffer::read(&self->property, &counted, self->property.data.get() + P_DEFEATED, 1, P_DEFEATED);
    if (left == floor && !counted) {
        uint8_t one = 1;
        SafeBuffer::write(&self->property, self->property.data.get() + P_DEFEATED, &one, 1, P_DEFEATED);
        uint32_t st = stageState();
        rw::call(g::StageState_addDefeatedEnemy, {st, uint32_t(self->master32(M_ID))});
        st = stageState();
        rw::call(g::StageState_addRaidpoint, {st, uint32_t(self->prop32(P_RAID_POINT))});
        st = stageState();
        rw::call(g::StageState_addEXP, {st, uint32_t(self->prop32(P_EXP))});
        st = stageState();
        int32_t count = self->prop32(P_DROP_COUNT);
        int32_t id = self->prop32(P_DROP_ID);
        rw::call(g::StageState_addEnemyDropItem, {st, uint32_t(count), uint32_t(id)});
        addCrushingPrizes(self, int32_t(damage));
        addDropItem(self);
        if (self->state <= 3) self->state = DYING;
        killed = true;
    }
    actionMarker(self, left > floor ? 0 : 4);
    rw::call(g::Board_setHP, {self->hpBoard, bt::f2uiz(float(left))});
    int32_t hits = 1;
    if (burst->obj) hits = sb32(burst->obj, 356);
    addAttackPrizes(self, dealt, hits);
    (void)s;
    return killed;
}
RW_FUNCTION(0x74B794, "EnemyActor::addDamage", EnemyActor::addDamage);

namespace {
// Lux goes straight into the stage state: the original adds it twice, to the counter at data position 17856 and
// through StageState::addLUX.
void addLuxToStage(uint32_t lux) {
    uint32_t st = stageState();
    int32_t old = sb32(stageState(), 17856);
    rw::host_ptr<SafeBuffer>(st)->set32(17856, int32_t(uint32_t(old) + lux));
    rw::call(g::StageState_addLUX, {stageState(), lux});
}
// MapPrizes::Info(Vec2::ZERO, FromGeneration 1) at `info` (the Vec2 argument copied to `tmp`)
MapPrizes::Info *newPrizeInfo(uint32_t info, uint32_t tmp) {
    rw::call(g::Vec2_copy, {tmp, actor_fn::vec2_ZERO});
    rw::call(g::MapPrizes_Info_ctor, {info, tmp, 1});
    rw::call(g::Vec2_dtor, {tmp});
    return rw::frame_ptr<MapPrizes::Info>(info);
}
// The player's max HP (APIManager user status, data position 56)
int32_t userMaxHp() { return sb32(ld(rw::call(g::APIManager_getInstance, {}) + 140), 56); }
}  // namespace

// The prizes of the killing blow: EXP and munny-Lux piles scaled by the player's max HP, and Lux by the damage
// (a power curve from BattleMisc 1000..1002) and the finishing medal's skill rate.
void EnemyActor::addCrushingPrizes(EnemyActor *self, int32_t damage) {
    float expRate = float(self->master32(M_CRUSH_EXP)) / 10000.0f;
    float luxRate = float(self->master32(M_CRUSH_LUX)) / 10000.0f;
    float lux = float(self->prop32(P_LUX));
    float skillRate = 1.0f;
    if (self->skill) skillRate = float(sb32(self->skill, 300)) / 10000.0f;
    MasterRef m0 = masterCreate(g::master_BattleMisc_create, 1000);
    float scale = float(m0.at(4));
    m0.release();
    MasterRef m1 = masterCreate(g::master_BattleMisc_create, 1001);
    int32_t pivot = m1.at(4);
    m1.release();
    MasterRef m2 = masterCreate(g::master_BattleMisc_create, 1002);
    float exponent = float(m2.at(4)) / 10000.0f;
    m2.release();
    float curve = rw::callf(g::powf_, {fb(float(damage) / float(pivot)), fb(exponent)});
    uint32_t luxAmount = ceilU(skillRate * ((lux * scale) * curve));
    addLuxToStage(luxAmount);
    bt::GuestFrame f(8 + 80);
    MapPrizes::Info *info = newPrizeInfo(f.addr(8), f.addr(0));
    uint32_t exp = ceilU(expRate * float(userMaxHp()));
    bool any = false;
    if (exp) {
        info->items[0].type = PRIZE_EXP;
        info->items[0].count = int32_t(exp);
        any = true;
    }
    uint32_t munny = ceilU(luxRate * 10000.0f);
    if (munny) {
        info->items[1].type = PRIZE_MUNNY_LUX;
        info->items[1].count = int32_t(munny);
        any = true;
    }
    if (luxAmount) {
        info->items[2].type = PRIZE_LUX;
        info->items[2].count = int32_t(luxAmount);
        info->luxStyle = 1;
    }
    if (luxAmount || any) {
        uint32_t prizes = rw::call(g::MapPrizes_create, {f.addr(8), self->fieldNode});
        if (prizes) {
            rw::call(g::VectorPrizes_pushBack, {ga(&self->prizes), prizes});
            self->crushPrize = prizes;
        }
    }
    rw::call(g::Vec2_dtor, {f.addr(8)});
}
RW_FUNCTION(0x74BA1C, "EnemyActor::addCrushingPrizes", EnemyActor::addCrushingPrizes);

// The prizes of a hit: like the crushing ones, per damage dealt and spread over the burst's hit count.
void EnemyActor::addAttackPrizes(EnemyActor *self, int32_t damage, int32_t hits) {
    float expRate = float(self->master32(M_ATTACK_EXP)) / 10000.0f;
    float munnyRate = float(self->master32(M_ATTACK_LUX)) / 10000.0f;
    int32_t luxBase = self->prop32(P_LUX);
    int32_t maxHp = self->prop32(P_HP_MAX);
    float lux = float(luxBase) * (float(damage) / float(maxHp));
    MasterRef m301 = masterCreate(g::master_BattleMisc_create, 301);
    float capExp = float(m301.at(4)) / 10000.0f;
    m301.release();
    MasterRef m401 = masterCreate(g::master_BattleMisc_create, 401);
    float capMunny = float(m401.at(4)) / 10000.0f;
    m401.release();
    MasterRef m302 = masterCreate(g::master_BattleMisc_create, 302);
    float bonusExp = float(m302.at(4)) / 10000.0f;
    m302.release();
    MasterRef m402 = masterCreate(g::master_BattleMisc_create, 402);
    float bonusMunny = float(m402.at(4)) / 10000.0f;
    m402.release();
    // the prize rates set by setPrizeParam (floats)
    float attackRate = bf(uint32_t(self->prizeParam.get32(0)));
    float attackRate2 = bf(uint32_t(self->prizeParam.get32(0)));
    float crushRate = bf(uint32_t(self->prizeParam.get32(4)));
    if (crushRate <= 1.0f) {
        bonusMunny = 1.0f;
        bonusExp = 1.0f;
    }
    if (!(capExp < attackRate)) capExp = attackRate;
    if (!(capMunny < attackRate2)) capMunny = attackRate2;
    float skillExp = 1.0f, skillMunny = 1.0f, skillLux = 1.0f;
    if (self->skill) {
        int32_t a = sb32(self->skill, 292);
        int32_t b = sb32(self->skill, 296);
        int32_t c = sb32(self->skill, 300);
        skillExp = float(a) / 10000.0f;
        skillMunny = float(b) / 10000.0f;
        skillLux = float(c) / 10000.0f;
    }
    uint32_t luxAmount = ceilU(lux * skillLux);
    addLuxToStage(luxAmount);
    bt::GuestFrame f(8 + 80);
    MapPrizes::Info *info = newPrizeInfo(f.addr(8), f.addr(0));
    float hitsF = float(hits);
    bool any = false;
    float expF = skillExp * (capExp * (bonusExp * (expRate * float(userMaxHp()))));
    uint32_t exp = ceilU(expF / hitsF);
    if (exp) {
        info->items[0].type = PRIZE_EXP;
        info->items[0].count = int32_t(exp);
        any = true;
    }
    uint32_t munny = ceilU((((munnyRate * 10000.0f) * bonusMunny) * capMunny) * skillMunny / hitsF);
    if (munny) {
        info->items[1].type = PRIZE_MUNNY_LUX;
        info->items[1].count = int32_t(munny);
        any = true;
    }
    uint32_t prizes = 0;
    if (luxAmount) {
        info->items[2].type = PRIZE_LUX;
        info->items[2].count = int32_t(luxAmount);
        prizes = rw::call(g::MapPrizes_create, {f.addr(8), self->fieldNode});
    } else if (any) {
        prizes = rw::call(g::MapPrizes_create, {f.addr(8), self->fieldNode});
    }
    addPrizes(self, prizes, 0);
    rw::call(g::Vec2_dtor, {f.addr(8)});
}
RW_FUNCTION(0x74BE78, "EnemyActor::addAttackPrizes", EnemyActor::addAttackPrizes);

void EnemyActor::addDropItem(EnemyActor *self) {
    if (!self->dropItem) return;
    uint32_t s = self->self();
    uint32_t pos = vc(s, SLOT_GET_POSITION, {s});
    bt::GuestFrame f(8);
    vec2Copy(f, 0, pos);
    rw::call(g::MapPrizes_eruption, {self->dropItem, ga(&self->mapPosition), f.addr()});
    self->dropItem = 0;
    rw::call(g::Vec2_dtor, {f.addr()});
}
RW_FUNCTION(0x74BDD4, "EnemyActor::addDropItem", EnemyActor::addDropItem);

bool EnemyActor::setState(EnemyActor *self, int32_t s) {
    if (self->state < s) {
        self->state = s;
        return true;
    }
    return false;
}
RW_FUNCTION(0x74BE64, "EnemyActor::setState", EnemyActor::setState);

void EnemyActor::preloadResources(EnemyActor *self) {
    rw::call(g::Enemy_preloadResources, {self->flash});
    if (self->showEffect) rw::call(g::ShowEffect_preloadResources, {self->showEffect});
    if (!self->attackType) {
        rw::call(g::audio_load, {rw::call(g::audio_getInstance, {}), 509000090});
        rw::call(g::audio_load, {rw::call(g::audio_getInstance, {}), 509000100});
    }
    rw::call(g::audio_load, {rw::call(g::audio_getInstance, {}), 509000110});
}
RW_FUNCTION(0x74C41C, "EnemyActor::preloadResources", EnemyActor::preloadResources);

void EnemyActor::deploy(EnemyActor *self, uint32_t show) {
    if (self->showEffect) {
        if (show == 1) {
            self->showState = 1;
            rw::call(g::ShowEffect_show, {self->showEffect});
        } else {
            self->showState = 3;
            rw::call(g::ShowEffect_hide, {self->showEffect});
        }
    }
    // the board shows only for a set symbol that is not kind 5 (Hex-Rays drops this condition)
    if (self->hpBoard && !self->boardHidden) {
        const bool visible = show == 1 && self->symbolSet != 0 && self->symbolKind != 5;
        vc(self->hpBoard, SLOT_SET_VISIBLE, {self->hpBoard, visible ? 1u : 0u});
    }
}
RW_FUNCTION(0x74C470, "EnemyActor::deploy", EnemyActor::deploy);

bool EnemyActor::setEncountAction(EnemyActor *self, int32_t team) {
    if (self->prop32(P_TEAM) != team) return false;
    deploy(self, 1);
    self->inBattle = 1;
    UpdateStatus(self);
    if (self->hpBoard && !self->boardHidden) {
        rw::call(g::Board_switchModeFieldToBattle, {self->hpBoard});
        vc(self->hpBoard, SLOT_SET_VISIBLE, {self->hpBoard, 1});
    }
    if (self->state <= 2) {
        self->state = ENCOUNT;
        alterMotionShifted(self->flash, 1);
        rw::call(g::FlashNode_reflection_status, {self->flash, 1});
    }
    return true;
}
RW_FUNCTION(0x74C4E4, "EnemyActor::setEncountAction", EnemyActor::setEncountAction);

void EnemyActor::setDeadAction(EnemyActor *self) {
    uint32_t s = self->self();
    self->inBattle = 0;
    if (self->hpBoard) vc(self->hpBoard, SLOT_SET_VISIBLE, {self->hpBoard, 0});
    if (self->crushPrize) {
        bt::GuestFrame f(16);
        uint32_t from = vec2Copy(f, 8, ga(&self->mapPosition));
        rw::save<float>(from + 4, ldf(from + 4) - ldf(self->flash + FLASH_CENTER_Y));
        uint32_t pos = vc(s, SLOT_GET_POSITION, {s});
        uint32_t to = vec2Copy(f, 0, pos);
        rw::call(g::MapPrizes_eruption, {self->crushPrize, from, to});
        self->crushPrize = 0;
        rw::call(g::Vec2_dtor, {to});
        rw::call(g::Vec2_dtor, {from});
    }
    if (self->deadType == 2) {
        setEndedAction(self);
        return;
    }
    {
        bt::GuestFrame f(8);
        uint32_t mgr = rw::call(g::audio_getInstance, {});
        rw::call(g::audio_getLayer, {f.addr(), mgr, 0});
        rw::call(g::audio_playSE, {f.word(0), 509000110, 0, 0});
        bt::releaseShared(f.word(4));
    }
    alterMotionZero(self->flash, 6);
    if (self->state <= 4) self->state = DEAD;
}
RW_FUNCTION(0x74C5E0, "EnemyActor::setDeadAction", EnemyActor::setDeadAction);

void EnemyActor::setEndedAction(EnemyActor *self) {
    if (self->state > 5) return;
    self->state = ENDED;
    if (self->deadType == 2) alterMotionShifted(self->flash, 1);
    else if (self->deadType == 0) vc(self->self(), SLOT_SET_VISIBLE, {self->self(), 0});
    rw::call(g::VectorPrizes_clear, {ga(&self->prizes)});
}
RW_FUNCTION(0x74C744, "EnemyActor::setEndedAction", EnemyActor::setEndedAction);

// The touch rect in world space: the node's origin converted, plus the rect origin.
void EnemyActor::getScreenTouchRect(const uint8_t *out, EnemyActor *self) {
    guest::Frame f(16);
    uint32_t p = f.addr();
    rw::call(g::Node_convertToWorldSpace, {p, self->self(), actor_fn::vec2_ZERO});
    rw::call(g::Vec2_add, {p, ga(&self->touchOrigin)});
    rw::call(g::Rect_ctor4, {ga(out), ld(p), ld(p + 4), fb(self->touchExtent.x), fb(self->touchExtent.y)});
    rw::call(g::Vec2_dtor, {p});
}
RW_FUNCTION(0x74C7A8, "EnemyActor::getScreenTouchRect", EnemyActor::getScreenTouchRect);

void EnemyActor::getMapPosotionTouchRect(const uint8_t *out, EnemyActor *self) {
    float mx = self->mapPosition.x;
    uint32_t rect = ga(&self->touchOrigin);
    float x = mx + rw::callf(g::Rect_getMidX, {rect});
    float my = self->mapPosition.y;
    float y = my - rw::callf(g::Rect_getMidY, {rect});
    rw::call(g::Rect_ctor4, {ga(out), fb(x), fb(y), fb(self->touchExtent.x), fb(self->touchExtent.y)});
}
RW_FUNCTION(0x74C848, "EnemyActor::getMapPosotionTouchRect", EnemyActor::getMapPosotionTouchRect);

// ------------------------------------------------------------------------------------------- movement
EnemyActor *EnemyActor::iniApproachMove(EnemyActor *self) {
    self->moveTimer = 0.0f;
    return self;
}
RW_FUNCTION(0x74C8B2, "EnemyActor::iniApproachMove", EnemyActor::iniApproachMove);

bool EnemyActor::setApproachMove(EnemyActor *self, const bt::Vec2 *dir, uint32_t force) {
    if (self->master32(M_APPROACH) == 3 || isDied(self)) return false;
    uint32_t status = self->self() + 712;
    {
        bt::GuestFrame f(16);
        rw::call(ld(ld(status)), {f.addr(), status, 0});   // getCondition(0) (sret)
        if (f.word(8)) return false;
    }
    float t = self->moveTimer;
    if (!(t <= 0.0f) && (force & 0xFF) != 1) return false;
    uint32_t rng = rw::call(g::Random_getInstance, {});
    int32_t r = int32_t(rw::call(g::Random_generateUniformInt, {rng, 0, 3, 0}));
    self->moveTimer = float(r) * 0.1f + 0.5f;
    self->moveDir.x = dir->x.get();
    self->moveDir.y = dir->y.get();
    rw::call(g::Vec2_normalize, {ga(&self->moveDir)});
    uint32_t flash = self->flash;
    if (ld(flash + FLASH_MOTION) != 2) alterMotionZero(flash, 2);
    return true;
}
RW_FUNCTION(0x74C8BC, "EnemyActor::setApproachMove", EnemyActor::setApproachMove);

namespace {
// canMoveTo(x, y): the field's walkability check (std::function<bool(float, float)> at +928)
bool callCanMoveTo(EnemyActor *self, uint32_t x, uint32_t y) {
    return (rw::call(g::fnFloat2_call, {ga(self->canMoveTo), x, y}) & 0xFF) != 0;
}
// One step along moveDir at the move speed from `from` (a Vec2 temporary copy of the map position).
void stepFrom(EnemyActor *self, uint32_t step) {
    float speed = self->speed;
    rw::save<float>(step, ldf(step) + self->moveDir.x * speed);
    rw::save<float>(step + 4, ldf(step + 4) + self->moveDir.y * speed);
}
}  // namespace

// Chase the player (target) while the enemy is in sight: steps along moveDir, a new direction (or a pause) every
// 0.5..0.8 s; returns whether it moved this frame.
bool EnemyActor::exeApproachMove(EnemyActor *self, float dt, const bt::Vec2 *target, uint32_t force) {
    if (self->master32(M_APPROACH) == 3) return false;
    if (isDied(self)) return false;
    uint32_t s = self->self();
    uint32_t status = s + 712;
    {
        bt::GuestFrame f(16);
        rw::call(ld(ld(status)), {f.addr(), status, 1});   // getCondition(1) (sret)
        if (f.word(8)) return false;
    }
    uint32_t map = ga(&self->mapPosition);
    float dist = rw::callf(g::Vec2_distance, {map, ga(target)});
    float mapX = self->mapPosition.x;
    float homeX = self->homePosition.x;
    MasterRef m120 = masterCreate(g::master_BattleMisc_create, 120);
    bool near = dist < 240.0f;
    bool away = true;   // left the home area
    if (!(std::fabs(homeX - mapX) > float(m120.at(4)))) {
        float dy = std::fabs(self->homePosition.y - self->mapPosition.y);
        MasterRef m121 = masterCreate(g::master_BattleMisc_create, 121);
        away = dy > float(m121.at(4));
        m121.release();
    }
    m120.release();
    if (away) {
        uint32_t home = ga(&self->homePosition);
        float back = rw::callf(g::Vec2_distance, {home, map});
        float toTarget = rw::callf(g::Vec2_distance, {home, ga(target)});
        near = (back < toTarget) | (dist < 240.0f);
    }
    bool moved = false;
    float t = self->moveTimer;
    if (t > 0.0f || std::isnan(t)) {
        if ((force & 0xFF) || !near) {
            bt::GuestFrame f(16);
            uint32_t from = vec2Copy(f, 8, map);
            uint32_t step = vec2Copy(f, 0, from);
            stepFrom(self, step);
            bool ok = callCanMoveTo(self, ld(step), ld(step + 4));
            bool set = ok;
            if (!ok) {
                bool blocked = !callCanMoveTo(self, ld(from), ld(from + 4));
                set = (force & 0xFF) || blocked;
            }
            if (set) {
                self->mapPosition.x = ldf(step);
                self->mapPosition.y = ldf(step + 4);
                moved = true;
            }
            rw::call(g::Vec2_dtor, {step});
            rw::call(g::Vec2_dtor, {from});
            t = self->moveTimer;
        }
        self->moveTimer = t - dt;
    } else {
        uint32_t rng = rw::call(g::Random_getInstance, {});
        int32_t r = int32_t(rw::call(g::Random_generateUniformInt, {rng, 0, 3, 0}));
        self->moveTimer = float(r) * 0.1f + 0.5f;
        rng = rw::call(g::Random_getInstance, {});
        int32_t r2 = int32_t(rw::call(g::Random_generateUniformInt, {rng, 0, 3, 0}));
        if (near | (r2 < 1)) {
            // stop and stand
            self->moveDir.x = ldf(actor_fn::vec2_ZERO);
            self->moveDir.y = ldf(actor_fn::vec2_ZERO + 4);
            if (ld(self->flash + FLASH_MOTION) >= 2) alterMotionShifted(self->flash, 1);
        } else {
            rw::call(g::Vec2_set2, {ga(&self->moveDir), map, ga(target)});
            rw::call(g::Vec2_normalize, {ga(&self->moveDir)});
            if (ld(self->flash + FLASH_MOTION) != 2) alterMotionZero(self->flash, 2);
        }
    }
    if (!self->fixedFacing) {
        float sx = self->mapPosition.x < target->x ? -1.0f : 1.0f;
        vc(s, SLOT_SET_SCALE_X, {s, fb(sx)});
    }
    return moved;
}
RW_FUNCTION(0x74C9DC, "EnemyActor::exeApproachMove", EnemyActor::exeApproachMove);

// A symbol enemy wandering on the map: walks to a random point around its home (the 9 directions of the table at
// 0x1003F70), then pauses 1.2..2 s.
void EnemyActor::moveWandering(EnemyActor *self, float dt) {
    if (self->state != SYMBOL || self->moveType == 1) return;
    uint32_t s = self->self();
    float t = self->moveTimer;
    if (t > 0.0f || std::isnan(t)) {
        uint32_t map = ga(&self->mapPosition);
        bt::GuestFrame f(16);
        uint32_t from = vec2Copy(f, 8, map);
        uint32_t step = vec2Copy(f, 0, from);
        stepFrom(self, step);
        if (callCanMoveTo(self, ld(step), ld(step + 4)) || !callCanMoveTo(self, ld(from), ld(from + 4))) {
            self->mapPosition.x = ldf(step);
            self->mapPosition.y = ldf(step + 4);
        }
        self->moveTimer = self->moveTimer - dt;
        rw::call(g::Vec2_dtor, {step});
        rw::call(g::Vec2_dtor, {from});
        return;
    }
    uint32_t rng = rw::call(g::Random_getInstance, {});
    int32_t r = int32_t(rw::call(g::Random_generateUniformInt, {rng, 0, 8, 0}));
    int32_t dir = self->wanderDir;
    self->moveDir.y = ldf(actor_fn::vec2_ZERO + 4);
    self->moveDir.x = ldf(actor_fn::vec2_ZERO);
    int32_t next = int32_t(uint32_t(dir) + uint32_t(r)) % 9;
    if (next == dir) {
        // pause
        rng = rw::call(g::Random_getInstance, {});
        self->moveTimer = rw::callf(g::Random_generateUniformFloat, {rng, fb(1.2f), fb(2.0f), 0});
        if (ld(self->flash + FLASH_MOTION)) alterMotionZero(self->flash, 0);
        return;
    }
    self->wanderDir = next;
    constexpr uint32_t DIRECTIONS = 0x1003F70;   // float pairs; entry k at +28 + 8k
    bt::GuestFrame f(16);
    uint32_t goal = vec2Copy(f, 8, ga(&self->homePosition));
    int32_t type = self->moveType;
    if (type == 2 || type == 4) {
        rng = rw::call(g::Random_getInstance, {});
        int32_t dx = int32_t(rw::call(g::Random_generateUniformInt, {rng, 0, 100, 0}));
        float x = float(dx) + ldf(goal);
        rw::save<float>(goal, x + ldf(DIRECTIONS + 8u * uint32_t(self->wanderDir.get()) + 28) * 100.0f);
        type = self->moveType;
    }
    if (uint32_t(type - 3) <= 1) {
        rng = rw::call(g::Random_getInstance, {});
        int32_t dy = int32_t(rw::call(g::Random_generateUniformInt, {rng, 0, 80, 0}));
        float y = float(dy) + ldf(goal + 4);
        rw::save<float>(goal + 4, y + ldf(DIRECTIONS + 8u * uint32_t(self->wanderDir.get()) + 32) * 80.0f);
    }
    uint32_t map = ga(&self->mapPosition);
    float dist = rw::callf(g::Vec2_distance, {goal, map});
    self->moveTimer = dist / self->speed / 60.0f;
    uint32_t d = vec2Copy(f, 0, goal);
    rw::call(g::Vec2_subtract, {d, map});
    self->moveDir.x = ldf(d);
    self->moveDir.y = ldf(d + 4);
    rw::call(g::Vec2_dtor, {d});
    rw::call(g::Vec2_normalize, {ga(&self->moveDir)});
    float x = self->moveDir.x;
    if (!(x == 0.0f)) vc(s, SLOT_SET_SCALE_X, {s, fb(x > 0.0f ? -1.0f : 1.0f)});
    if (ld(self->flash + FLASH_MOTION) != 2) alterMotionZero(self->flash, 2);
    rw::call(g::Vec2_dtor, {goal});
}
RW_FUNCTION(0x74CE70, "EnemyActor::moveWandering", EnemyActor::moveWandering);

void EnemyActor::lookAt(EnemyActor *self, const bt::Vec2 *target) {
    if (self->fixedFacing) return;
    float sx = self->mapPosition.x < target->x ? -1.0f : 1.0f;
    vc(self->self(), SLOT_SET_SCALE_X, {self->self(), fb(sx)});
}
RW_FUNCTION(0x74CE34, "EnemyActor::lookAt", EnemyActor::lookAt);

void EnemyActor::update(EnemyActor *self, float) {
    uint32_t o = self->overlay;
    if (!o) return;
    uint32_t fn = ld(ld(o) + 4 * SLOT_SET_POSITION);
    uint32_t pos = vc(self->self(), SLOT_GET_POSITION, {self->self()});
    rw::call(fn, {o, pos});
}
RW_FUNCTION(0x74D210, "EnemyActor::update", EnemyActor::update);

void EnemyActor::updateGauge(EnemyActor *self, float dt) {
    if (self->hpBoard) vc(self->hpBoard, SLOT_UPDATE, {self->hpBoard, fb(dt)});
}
RW_FUNCTION(0x74D234, "EnemyActor::updateGauge", EnemyActor::updateGauge);

void EnemyActor::setVisible(EnemyActor *self, uint32_t visible) {
    uint32_t s = self->self();
    rw::call(g::Node_setVisible, {s, visible});
    vc(self->overlay, SLOT_SET_VISIBLE, {self->overlay, visible});
    uint32_t o = self->overlay;
    uint32_t fn = ld(ld(o) + 4 * SLOT_SET_POSITION);
    uint32_t pos = vc(s, SLOT_GET_POSITION, {s});
    rw::call(fn, {o, pos});
}
RW_FUNCTION(0x74D24C, "EnemyActor::setVisible", EnemyActor::setVisible);

void EnemyActor::setScaleX(EnemyActor *self, float sx) {
    rw::call(g::Node_setScaleX, {self->self(), fb(sx)});
    uint32_t effect = ld(self->flash + FLASH_EFFECT_ARMATURE);
    vc(effect, SLOT_SET_SCALE_X, {effect, fb(sx)});
}
RW_FUNCTION(0x74D28C, "EnemyActor::setScaleX", EnemyActor::setScaleX);

// The HP board highlights an enemy weak against the attribute (1 beats 2, 2 beats 3, 3 beats 1).
void EnemyActor::setAdvantageousEffect(EnemyActor *self, int32_t attribute) {
    if (!self->hpBoard) return;
    int32_t own = self->master32(M_ATTRIBUTE);
    uint32_t on = 1;
    if ((attribute != 1 || own != 2) && (attribute != 2 || own != 3)) on = attribute == 3 && own == 1;
    rw::call(g::Board_setSuitableAsTargetEffect, {self->hpBoard, on});
}
RW_FUNCTION(0x74D2B4, "EnemyActor::setAdvantageousEffect", EnemyActor::setAdvantageousEffect);

void EnemyActor::addPrizes(EnemyActor *self, uint32_t prizes, uint32_t onDeath) {
    if (!prizes) return;
    rw::call(g::VectorPrizes_pushBack, {ga(&self->prizes), prizes});
    if ((onDeath & 0xFF) == 1) {
        self->crushPrize = prizes;
        return;
    }
    uint32_t s = self->self();
    uint32_t pos = vc(s, SLOT_GET_POSITION, {s});
    bt::GuestFrame f(8);
    vec2Copy(f, 0, pos);
    rw::call(g::MapPrizes_eruption, {prizes, ga(&self->mapPosition), f.addr()});
    rw::call(g::Vec2_dtor, {f.addr()});
}
RW_FUNCTION(0x74D348, "EnemyActor::addPrizes", EnemyActor::addPrizes);

void EnemyActor::setDropItem(EnemyActor *self, uint32_t prizes) {
    self->dropItem = prizes;
    if (prizes) rw::call(g::VectorPrizes_pushBack, {ga(&self->prizes), prizes});
}
RW_FUNCTION(0x74D3F0, "EnemyActor::setDropItem", EnemyActor::setDropItem);

void EnemyActor::addCrushingMoneyPrizes(EnemyActor *self, int32_t kind) {
    float money = float(self->prop32(P_MONEY));
    if (kind == 2) money = money * masterRate(g::master_BattleMisc_create, 903);
    else if (kind == 1) money = money * masterRate(g::master_BattleMisc_create, 902);
    uint32_t amount = ceilU(money);
    uint32_t prizes = 0;
    if (amount) {
        bt::GuestFrame f(8 + 80);
        uint32_t zero = vec2Copy(f, 0, actor_fn::vec2_ZERO);
        uint32_t info = f.addr(8);
        rw::call(g::MapPrizes_Info_ctor, {info, zero, 1});
        rw::call(g::Vec2_dtor, {zero});
        auto *inf = rw::frame_ptr<MapPrizes::Info>(info);
        inf->items[0].type = PRIZE_MONEY;
        inf->items[0].count = int32_t(amount);
        prizes = rw::call(g::MapPrizes_create, {info, self->fieldNode});
        rw::call(g::StageState_addMoney, {stageState(), amount});
        rw::call(g::Vec2_dtor, {info});
    }
    addPrizes(self, prizes, 0);
}
RW_FUNCTION(0x74D408, "EnemyActor::addCrushingMoneyPrizes", EnemyActor::addCrushingMoneyPrizes);

bool EnemyActor::isEncount(EnemyActor *self, const bt::Vec2 *pos, float range) {
    bt::GuestFrame f(8);
    uint32_t d = vec2Copy(f, 0, ga(pos));
    rw::call(g::Vec2_subtract, {d, ga(&self->mapPosition)});
    rw::save<float>(d + 4, ldf(0x1003F70) * ldf(d + 4));
    float dist = rw::callf(g::Vec2_distance, {d, actor_fn::vec2_ZERO});
    float reach = self->encountRange;
    rw::call(g::Vec2_dtor, {d});
    return dist < reach + range;
}
RW_FUNCTION(0x74D5C4, "EnemyActor::isEncount", EnemyActor::isEncount);

bool EnemyActor::isMarking(EnemyActor *self) {
    int32_t hp = self->prop32(P_HP);
    int32_t floor = self->prop32(P_HP_MIN);
    uint8_t on = self->inBattle ? 1 : 0;
    return ((hp > floor) & on) != 0;
}
RW_FUNCTION(0x74D68C, "EnemyActor::isMarking", EnemyActor::isMarking);

bool EnemyActor::isDied(const EnemyActor *self) {
    int32_t hp = self->prop32(P_HP);
    return hp == self->prop32(P_HP_MIN);
}
RW_FUNCTION(0x6008EC, "EnemyActor::isDied", EnemyActor::isDied);

// ------------------------------------------------------------------------------------------- target selection
void EnemyActor::setupSelectWait(EnemyActor *self, uint32_t callback) {
    rw::call(g::fnSelect_assign, {ga(self->onSelect), callback});
    self->selectWait = 1;
    actionMarker(self, 0);
    actionSelector(self, 1);
}
RW_FUNCTION(0x74D8DC, "EnemyActor::setupSelectWait", EnemyActor::setupSelectWait);

void EnemyActor::startSelectWait(EnemyActor *self) {
    actionMarker(self, 3);
    actionSelector(self, 1);
}
RW_FUNCTION(0x74D980, "EnemyActor::startSelectWait", EnemyActor::startSelectWait);

EnemyActor *EnemyActor::ceaseSelectWait(EnemyActor *self) {
    self->selectWait = 0;
    return self;
}
RW_FUNCTION(0x74D998, "EnemyActor::ceaseSelectWait", EnemyActor::ceaseSelectWait);

void EnemyActor::endedSelectWait(EnemyActor *self) {
    destroyFunction(ga(self->onSelect));
    rw::set<uint32_t>(self, 920, 0);
    self->selectWait = 0;
    uint32_t a = vc(self->tapMarker, SLOT_ARMATURE_GET_ANIMATION, {self->tapMarker});
    vc(a, SLOT_ANIM_STOP, {a});
    vc(self->tapMarker, SLOT_SET_VISIBLE, {self->tapMarker, 0});
    uint32_t b = vc(self->selector, SLOT_ARMATURE_GET_ANIMATION, {self->selector});
    vc(b, SLOT_ANIM_STOP, {b});
    vc(self->selector, SLOT_SET_VISIBLE, {self->selector, 0});
}
RW_FUNCTION(0x74D9A4, "EnemyActor::endedSelectWait", EnemyActor::endedSelectWait);

bool EnemyActor::isSetSelectWait(EnemyActor *self) { return rw::get<uint32_t>(self, 920) != 0; }
RW_FUNCTION(0x74DA24, "EnemyActor::isSetSelectWait", EnemyActor::isSetSelectWait);

void EnemyActor::pause(EnemyActor *self) {
    uint32_t flash = self->flash;
    if (flash) {
        uint32_t lwf = ld(flash + FLASH_LWF_NODE);
        if (lwf) rw::call(g::Node_unscheduleUpdate, {lwf});
    }
    rw::call(g::MapNode_pause, {self->self()});
}
RW_FUNCTION(0x74DA36, "EnemyActor::pause", EnemyActor::pause);

void EnemyActor::resume(EnemyActor *self) {
    rw::call(g::MapNode_resume, {self->self()});
    uint32_t flash = self->flash;
    if (flash) {
        uint32_t lwf = ld(flash + FLASH_LWF_NODE);
        if (lwf) rw::call(g::Node_scheduleUpdate, {lwf});
    }
}
RW_FUNCTION(0x74DA5E, "EnemyActor::resume", EnemyActor::resume);

// ------------------------------------------------------------------------------------------- Property
namespace enemy_actor {
void Property::getCondition(const uint8_t *out, const Property *self, int32_t type) {
    rw::store32(out, 0);
    rw::store32(out + 4, 0);
    rw::store32(out + 8, 0);
    uint32_t index = 12u * uint32_t(type) + EnemyActor::P_CONDITIONS;
    SafeBuffer::read(self, out, self->data.get() + index, 12, index);
}
RW_FUNCTION(0x74D738, "EnemyActor::Property::getCondition", Property::getCondition);

void Property::setCondition(Property *self, int32_t type, const uint8_t *change) {
    uint32_t index = 12u * uint32_t(type) + EnemyActor::P_CONDITIONS;
    SafeBuffer::write(self, self->data.get() + index, change, 12, index);
}
RW_FUNCTION(0x74D79A, "EnemyActor::Property::setCondition", Property::setCondition);

void Property::addCondition(Property *self, int32_t type, int32_t turns, int32_t count) {
    rw::call(g::IStatus_addCondition_common, {rw::guest_addr(self) + 32, uint32_t(type), uint32_t(turns), uint32_t(count)});
}
RW_FUNCTION(0x74ABBE, "EnemyActor::Property::addCondition", Property::addCondition);

void Property::getBuff(const uint8_t *out, const Property *self, int32_t slot) {
    rw::store32(out, 0);
    rw::store32(out + 4, 0);
    rw::store32(out + 8, 0);
    uint32_t index = 12u * uint32_t(slot) + EnemyActor::P_BUFFS;
    SafeBuffer::read(self, out, self->data.get() + index, 12, index);
}
RW_FUNCTION(0x74D7DC, "EnemyActor::Property::getBuff", Property::getBuff);

void Property::addBuff(Property *self, const uint8_t *change) {
    uint32_t slot = rw::call(g::IBuff_getBlankBuff, {rw::guest_addr(self) + 36});
    uint32_t index = 12u * slot + EnemyActor::P_BUFFS;
    SafeBuffer::write(self, self->data.get() + index, change, 12, index);
}
RW_FUNCTION(0x74D83E, "EnemyActor::Property::addBuff", Property::addBuff);

void Property::setBuff(Property *self, int32_t slot, const uint8_t *change) {
    uint32_t index = 12u * uint32_t(slot) + EnemyActor::P_BUFFS;
    SafeBuffer::write(self, self->data.get() + index, change, 12, index);
}
RW_FUNCTION(0x74D8A2, "EnemyActor::Property::setBuff", Property::setBuff);

int32_t Property::getMaxHP(const Property *self) { return self->get32(EnemyActor::P_HP_MAX); }
RW_FUNCTION(0x74B0C8, "EnemyActor::Property::getMaxHP", Property::getMaxHP);

// non-virtual thunks: the IStatusProtocol (+0x20) / IBuffProtocol (+0x24) bases' views of the same Property
void thunkGetCondition(const uint8_t *out, uint32_t status, int32_t type) {
    Property::getCondition(out, rw::host_ptr<Property>(status - 0x20), type);
}
RW_FUNCTION(0x74D766, "thunk to EnemyActor::Property::getCondition", thunkGetCondition);
void thunkGetBuff(const uint8_t *out, uint32_t buffs, int32_t slot) {
    Property::getBuff(out, rw::host_ptr<Property>(buffs - 0x24), slot);
}
RW_FUNCTION(0x74D80A, "thunk to EnemyActor::Property::getBuff", thunkGetBuff);
int32_t thunkGetMaxHP(uint32_t status) { return Property::getMaxHP(rw::host_ptr<Property>(status - 0x20)); }
RW_FUNCTION(0x74DB1C, "thunk to EnemyActor::Property::getMaxHP", thunkGetMaxHP);

void Property::heal(Property *self, int32_t amount) {
    int32_t hp = self->get32(EnemyActor::P_HP);
    int32_t max = int32_t(rw::vcall(self, 8, {rw::guest_addr(self)}));   // virtual getMaxHP
    int32_t v = int32_t(uint32_t(hp) + uint32_t(amount));
    if (v >= max) v = max;
    self->set32(EnemyActor::P_HP, v);
}
RW_FUNCTION(0x74DA90, "EnemyActor::Property::heal", Property::heal);

void Property::destructDelete(Property *self) {
    SafeBuffer::destruct(self);
    rw::call(bt::fn::operator_delete, {rw::guest_addr(self)});
}
RW_FUNCTION(0x74DA82, "EnemyActor::Property::~Property (deleting)", Property::destructDelete);
}  // namespace enemy_actor

// ------------------------------------------------------------------------------------------- touch lambdas
// The bodies of setupTouchRect's listener lambdas (std::function operator(): lambda, Touch*&&, Event*&&; the lambda
// captures `this` at +4).
namespace enemy_actor {
constexpr uint32_t Touch_getLocation = 0x900E57;
constexpr uint32_t Touch_getPreviousLocation = 0x900E79;
constexpr uint32_t Node_convertToNodeSpace = 0x89599D;
constexpr uint32_t Rect_containsPoint = 0x8DF2A9;
constexpr uint32_t fnTapped_call = 0x74DEDD;   // std::function<bool(EnemyActor*)>::operator()
constexpr uint32_t SLOT_IS_VISIBLE = 45;

// touch cancelled: a pending tap is dropped; a shown marker is reset and the selector hidden
void onTouchCancelled(uint32_t lambda, const uint32_t *, const uint32_t *) {
    auto *self = rw::host_ptr<EnemyActor>(ld(lambda + 4));
    if (!self->touching) return;
    self->touching = 0;
    if (vc(self->tapMarker, SLOT_IS_VISIBLE, {self->tapMarker}) != 1) return;
    EnemyActor::actionMarker(self, 0);
    uint32_t anim = vc(self->selector, SLOT_ARMATURE_GET_ANIMATION, {self->selector});
    vc(anim, SLOT_ANIM_STOP, {anim});
    vc(self->selector, SLOT_SET_VISIBLE, {self->selector, 0});
}
RW_FUNCTION(0x74DD06, "EnemyActor::setupTouchRect touch cancelled lambda", onTouchCancelled);

// touch ended: a tap released inside the touch rect calls onTapped(this)
void onTouchEnded(uint32_t lambda, const uint32_t *touchRef, const uint32_t *) {
    auto *self = rw::host_ptr<EnemyActor>(ld(lambda + 4));
    if (!self->touching) return;
    uint32_t touch = *touchRef;
    self->touching = 0;
    bt::GuestFrame f(16);
    uint32_t loc = f.addr(0), p = f.addr(8);
    rw::call(Touch_getLocation, {loc, touch});
    rw::call(Node_convertToNodeSpace, {p, self->self(), loc});
    rw::call(g::Vec2_dtor, {loc});
    if ((rw::call(Rect_containsPoint, {ga(&self->touchOrigin), p}) & 0xFF) == 1 && rw::get<uint32_t>(self, 896))
        rw::call(fnTapped_call, {ga(self->onTapped), self->self()});
    rw::call(g::Vec2_dtor, {p});
}
RW_FUNCTION(0x74DDF4, "EnemyActor::setupTouchRect touch ended lambda", onTouchEnded);

// touch moved: both locations are tested against the touch rect (the results are unused)
void onTouchMoved(uint32_t lambda, const uint32_t *touchRef, const uint32_t *) {
    auto *self = rw::host_ptr<EnemyActor>(ld(lambda + 4));
    if (!self->touching) return;
    uint32_t touch = *touchRef;
    bt::GuestFrame f(32);
    uint32_t prev = f.addr(0), p1 = f.addr(8), loc = f.addr(16), p2 = f.addr(24);
    rw::call(Touch_getPreviousLocation, {prev, touch});
    rw::call(Node_convertToNodeSpace, {p1, self->self(), prev});
    rw::call(g::Vec2_dtor, {prev});
    rw::call(Touch_getLocation, {loc, touch});
    rw::call(Node_convertToNodeSpace, {p2, self->self(), loc});
    rw::call(g::Vec2_dtor, {loc});
    uint32_t rect = ga(&self->touchOrigin);
    if ((rw::call(Rect_containsPoint, {rect, p1}) & 0xFF) == 1) rw::call(Rect_containsPoint, {rect, p2});
    rw::call(g::Vec2_dtor, {p2});
    rw::call(g::Vec2_dtor, {p1});
}
RW_FUNCTION(0x74DFBC, "EnemyActor::setupTouchRect touch moved lambda", onTouchMoved);
}  // namespace enemy_actor
