#include "StageActorManager.hpp"

namespace {
namespace g {
// cocos2d / libc++ / runtime
constexpr uint32_t ref_ctor = 0x8FD025;               // cocos2d::Ref::Ref()
constexpr uint32_t aeabi_memset = 0x40E7A0;           // __aeabi_memset(dst, n, c)
constexpr uint32_t add_shared = 0xC8DBB5;             // std::__shared_weak_count::__add_shared()
constexpr uint32_t enemyVec_clear = 0x74ED1D;         // cocos2d::Vector<EnemyActor*>::clear()
constexpr uint32_t enemyVec_pushBack = 0x74F0F5;      // cocos2d::Vector<EnemyActor*>::pushBack(EnemyActor*)
constexpr uint32_t enemyVec_dtor = 0x74EAF7;          // cocos2d::Vector<EnemyActor*>::~Vector()
constexpr uint32_t mapNodeVec_dtor = 0x5DA621;        // cocos2d::Vector<MapNode*>::~Vector()
constexpr uint32_t vectorBase_dtor = 0x74EACD;        // std::__vector_base<EnemyActor*>::~__vector_base()
constexpr uint32_t push_back_slow = 0x7531E1;         // std::vector<EnemyActor*>::__push_back_slow_path(EnemyActor* const&)
constexpr uint32_t prizesVec_copy = 0x60A4ED;         // cocos2d::Vector<MapPrizes*>::Vector(const Vector&)
constexpr uint32_t prizesVec_dtor = 0x5DB649;         // cocos2d::Vector<MapPrizes*>::~Vector()
constexpr uint32_t prizesVec_erase = 0x60A53F;        // cocos2d::Vector<MapPrizes*>::eraseObject(MapPrizes*, bool)
constexpr uint32_t fnBoolFF_assign = 0x607AC1;        // std::function<bool(float,float)>::operator=(const function&)
constexpr uint32_t fnBoolEnemy_assign = 0x7535CD;     // std::function<bool(EnemyActor*)>::operator=(const function&)
constexpr uint32_t fnBool_call = 0x51022D;            // std::function<bool()>::operator()() const
constexpr uint32_t fnVoid_call = 0x470E69;            // std::function<void()>::operator()() const
constexpr uint32_t fnVoidBool_call = 0x474139;        // std::function<void(bool)>::operator()(bool) const
constexpr uint32_t base_dtor = 0x74E759;              // BaseActorManager::~BaseActorManager()
constexpr uint32_t vec2_ctor = 0x8E4B9D;              // cocos2d::Vec2::Vec2(float, float)
constexpr uint32_t vec2_copy = 0x8E4C03;              // cocos2d::Vec2::Vec2(const Vec2&)
constexpr uint32_t vec2_dtor = 0x8E4C17;              // cocos2d::Vec2::~Vec2()
constexpr uint32_t vec2_add = 0x8E4D2D;               // cocos2d::Vec2::add(const Vec2&)
constexpr uint32_t vec2_scale = 0x8E50D5;             // cocos2d::Vec2::scale(float)
constexpr uint32_t vec2_subtract = 0x8E530F;          // cocos2d::Vec2::subtract(const Vec2&)
constexpr uint32_t vec2_ZERO = 0x1006950;             // cocos2d::Vec2::ZERO
constexpr uint32_t node_runAction = 0x894AED;         // cocos2d::Node::runAction(Action*)
constexpr uint32_t callFunc_create = 0x866B39;        // cocos2d::CallFunc::create(const std::function<void()>&)
constexpr uint32_t moveBy_create = 0x86911D;          // cocos2d::MoveBy::create(float, const Vec2&)
constexpr uint32_t moveTo_create = 0x86941D;          // cocos2d::MoveTo::create(float, const Vec2&)
constexpr uint32_t jumpBy_create = 0x869B79;          // cocos2d::JumpBy::create(float, const Vec2&, float, int)
constexpr uint32_t easeOut_create = 0x8606A5;         // cocos2d::EaseOut::create(ActionInterval*, float)
constexpr uint32_t easeSineOut_create = 0x860D09;     // cocos2d::EaseSineOut::create(ActionInterval*)
constexpr uint32_t delayTime_create = 0x86852D;       // cocos2d::DelayTime::create(float)
constexpr uint32_t sequence_create = 0x867999;        // cocos2d::Sequence::create(FiniteTimeAction*, ...)
// game singletons / state
constexpr uint32_t appState_getInstance = 0x4E66DD;   // AppState::getInstance()
constexpr uint32_t stageState_hasExtermination = 0x4EF995;  // StageState::hasExterminationTeamID(unsigned)
constexpr uint32_t stageState_addDamage = 0x4EEB89;   // StageState::addDamage(int)
constexpr uint32_t battleMisc_getInstance = 0x7420A9; // BattleMisc::getInstance()
constexpr uint32_t battleMisc_enemyAttack = 0x748185; // BattleMisc::calculateEnemyAttack(const EnemyActor*, int)
constexpr uint32_t battleMisc_playerAttack = 0x74718D;  // BattleMisc::calculatePlayerAttack(const AttackTarget&, EnemyActor*)
constexpr uint32_t random_getInstance = 0x492055;     // Random::getInstance()
constexpr uint32_t random_uniformInt = 0x492225;      // Random::generateUniformInt(int, int, unsigned*)
constexpr uint32_t random_uniformFloat = 0x492261;    // Random::generateUniformFloat(float, float, unsigned*)
constexpr uint32_t audio_getInstance = 0x5BECFD;      // audio::Manager::getInstance()
constexpr uint32_t audio_load = 0x5BEEF5;             // audio::Manager::load(ResourceID)
constexpr uint32_t audio_getLayer = 0x5BF33D;         // audio::Manager::getLayer(int) (sret shared_ptr<Layer>)
constexpr uint32_t audio_playSE = 0x5BF785;           // audio::Layer::playSE(ResourceID, int, int)
constexpr uint32_t burst_create = 0x77A491;           // master::Burst::create(int) (sret shared_ptr)
constexpr uint32_t raidEnemy_create = 0x787745;       // master::RaidEnemy::create(int) (sret shared_ptr)
constexpr uint32_t medalDeck_currentInfo = 0x5DFF6B;  // MedalDeck::getCurrentMedalInfo()
constexpr uint32_t medalDeck_currentDefense = 0x5DFFFD;  // MedalDeck::getCurrentDefense()
constexpr uint32_t statusChange_clear = 0x4F1D15;     // StatusChange::clear()
constexpr uint32_t buff_elapseDefense = 0x4F229D;     // IBuffProtocol::elapseCountDefenseBuff()
constexpr uint32_t buff_elapseAttack = 0x4F221D;      // IBuffProtocol::elapseCountAttackBuff()
// stage
constexpr uint32_t sm_getEnemyTeamInfo = 0x75C22D;    // StageManager::getEnemyTeamInfo(int)
constexpr uint32_t sm_isInsideTeamEncountArea = 0x75C261;  // StageManager::isInsideTeamEncountArea(const Vec2&, int)
constexpr uint32_t field_autoDirection = 0x75563D;    // StageFieldManager::getAutoDirectionFromField(float)
constexpr uint32_t field_addMapNode = 0x755509;       // StageFieldManager::addMapNodeToField(MapNode*)
constexpr uint32_t field_removeMapNode = 0x7558E5;    // StageFieldManager::removeMapNodeToField(MapNode*)
constexpr uint32_t field_localToMap = 0x75651D;       // StageFieldManager::getLocalPosToMapPos(const Vec2&) (sret)
constexpr uint32_t stageUtil_dropPrizes = 0x763C1D;   // StageUtil::getEnemyDropPrizes(MapPrizes::Items*, int, int, PlayStyle)
constexpr uint32_t prizesInfo_ctor = 0x5DB935;        // MapPrizes::Info::Info(Vec2, FromGeneration)
constexpr uint32_t prizes_create = 0x5DE8AD;          // MapPrizes::create(const Info*, cocos2d::Node*)
constexpr uint32_t prizes_isEruption = 0x5DEB59;      // MapPrizes::isEruption()
constexpr uint32_t scene_enemyCountEnd = 0x5FA709;    // SceneBattleBase::enemy_count_end(EnemyActor*)
constexpr uint32_t scene_damageSkill = 0x5FAA41;      // SceneBattleBase::DamageSkill()
constexpr uint32_t direction_skillPanelIn = 0x765A19; // CommonDirection::req_skilPanel_in(stage::skill::id)
constexpr uint32_t dispDamageNumber = 0x5EB45D;       // DisplayDamageNumber::disp_dmgNum(int, Node*, Vec2, float, bool, bool, bool, bool, int, float)
constexpr uint32_t missBoard_disp = 0x5D4171;         // MissBoard::disp_miss(Node*, Vec2, float, float)
constexpr uint32_t hpIndicator_setBufDebuf = 0x5E7D91;  // RaidBossHPIndicator::set_buf_debuf(buff_idx, int)
constexpr uint32_t hitEffect_deploy = 0x5D9225;       // HitEffect::deploy(int, Node*, const Vec2&, int, float, const std::function<void()>&)
constexpr uint32_t cutIn_concentrationStart = 0x496215;  // GuiltBurstCutInShader::setShaderConcentrationStart()
constexpr uint32_t cutIn_end = 0x496271;              // GuiltBurstCutInShader::setShaderEnd()
constexpr uint32_t raidBoss_setAdvantage = 0x5F5F91;  // RaidBossActor::setAdvantageousEffect(int, int)
constexpr uint32_t deque_pushFront = 0x741CA9;        // std::deque<FlashNode::ActFlag>::push_front(const ActFlag&)
// MapNode / FlashNode / Avatar
constexpr uint32_t mapNode_ctor = 0x5DA2D5;           // MapNode::MapNode()
constexpr uint32_t mapNode_updatePosition = 0x5DA461; // MapNode::updateMapPosition()
constexpr uint32_t mapNode_lockZ = 0x5DA575;          // MapNode::lockMapPosZ(float)
constexpr uint32_t mapNode_unlockZ = 0x5DA381;        // MapNode::unlockMapPosZ()
constexpr uint32_t mapNode_move = 0x5DA415;           // MapNode::move(cocos2d::Action*)
constexpr uint32_t flash_flagShift = 0x5EFD39;        // FlashNode::flagShift()
constexpr uint32_t flash_knockbackShift = 0x5EFB19;   // FlashNode::knockbackShift()
constexpr uint32_t flash_setBadStatus = 0x5C2D61;     // FlashNode::set_bad_status(int)
constexpr uint32_t flash_reflectionStatus = 0x5C3171; // FlashNode::reflection_status(bool)
constexpr uint32_t flash_isPlaying = 0x5EE499;        // FlashNode::isPlaying() const
constexpr uint32_t avatar_expansionAction = 0x5C81ED; // Avatar::createExpansionAction()
constexpr uint32_t avatar_changeMotion = 0x5C63C5;    // Avatar::changeMotion(Motion)
constexpr uint32_t avatar_activeBurst = 0x5C62D5;     // Avatar::activeBurst(int)
constexpr uint32_t avatarActor_lookAt = 0x741651;     // AvatarActor::lookAt(const Vec2&)
constexpr uint32_t avatarActor_finishBonus = 0x741189;  // AvatarActor::getShortTermFinishBonus(unsigned)
constexpr uint32_t avatarActor_updateStatus = 0x7403A9; // AvatarActor::UpdateStatus()
constexpr uint32_t avatarActor_takeActionFlag = 0x740689;  // AvatarActor::takeActionFlag(FlashNode::ActFlag)
constexpr uint32_t avatarActor_pause = 0x741699;      // AvatarActor::pause()
constexpr uint32_t avatarActor_resume = 0x7416C1;     // AvatarActor::resume()
// EnemyActor
constexpr uint32_t enemy_create = 0x74AB79;           // EnemyActor::create(EnemyActorInfo*, cocos2d::Node*)
constexpr uint32_t enemy_setSymbolInfo = 0x74ABC9;    // EnemyActor::setSymbolInfo(int, int, bool, bool)
constexpr uint32_t enemy_setTeamInfo = 0x74ACDD;      // EnemyActor::setTeamInfo(int, int)
constexpr uint32_t enemy_setStatusBoard = 0x74ACED;   // EnemyActor::setStatusBoard(int, int)
constexpr uint32_t enemy_preloadResources = 0x74C41D; // EnemyActor::preloadResources()
constexpr uint32_t enemy_setDropItem = 0x74D3F1;      // EnemyActor::setDropItem(MapPrizes*)
constexpr uint32_t enemy_deploy = 0x74C471;           // EnemyActor::deploy(bool)
constexpr uint32_t enemy_setEncountAction = 0x74C4E5; // EnemyActor::setEncountAction(int)
constexpr uint32_t enemy_lookAt = 0x74CE35;           // EnemyActor::lookAt(const Vec2&)
constexpr uint32_t enemy_isEncount = 0x74D5C5;        // EnemyActor::isEncount(const Vec2&, float)
constexpr uint32_t enemy_isDied = 0x6008ED;           // EnemyActor::isDied() const
constexpr uint32_t enemy_actionMarker = 0x74A65D;     // EnemyActor::actionMarker(TapAction)
constexpr uint32_t enemy_setAdvantage = 0x74D2B5;     // EnemyActor::setAdvantageousEffect(int)
constexpr uint32_t enemy_crushingMoney = 0x74D409;    // EnemyActor::addCrushingMoneyPrizes(int)
constexpr uint32_t enemy_updateGauge = 0x74D235;      // EnemyActor::updateGauge(float)
constexpr uint32_t enemy_moveWandering = 0x74CE71;    // EnemyActor::moveWandering(float)
constexpr uint32_t enemy_hitEffectID = 0x74B67D;      // EnemyActor::getHitEffectID()
constexpr uint32_t enemy_hitEffectDirection = 0x74B6FD;  // EnemyActor::getHitEffectDirection()
constexpr uint32_t enemy_setDeadAction = 0x74C5E1;    // EnemyActor::setDeadAction()
constexpr uint32_t enemy_setEndedAction = 0x74C745;   // EnemyActor::setEndedAction()
constexpr uint32_t enemy_changeMotion = 0x74B3F1;     // EnemyActor::changeMotion(Enemy::Motion)
constexpr uint32_t enemy_addDamage = 0x74B795;        // EnemyActor::addDamage(int, std::shared_ptr<const master::Burst>)
constexpr uint32_t enemy_isMarking = 0x74D68D;        // EnemyActor::isMarking()
constexpr uint32_t enemy_updateStatus = 0x74B441;     // EnemyActor::UpdateStatus()
constexpr uint32_t enemy_selectAttack = 0x74B1D1;     // EnemyActor::select_enemy_attack(float, StageActorManager*)
constexpr uint32_t enemy_pause = 0x74DA37;            // EnemyActor::pause()
constexpr uint32_t enemy_resume = 0x74DA5F;           // EnemyActor::resume()
constexpr uint32_t enemy_addCondition = 0x74ABBF;     // EnemyActor::Property::addCondition(ConditionType, int, int)
// data
constexpr uint32_t advantageTable = 0xE9DD00;         // int[]: [attr] = attribute it beats, [attr + 5] = second choice
// std::function in-place functor vtables (lambdas)
constexpr uint32_t fn_enemyFF = 0xF6E0A0;             // setupEnemyActorPlatoon lambda bool(float, float) (manager)
constexpr uint32_t fn_enemyB = 0xF6E074;              // setupEnemyActorPlatoon lambda bool(EnemyActor*) #1 (manager)
constexpr uint32_t fn_enemyC = 0xF6E048;              // setupEnemyActorPlatoon lambda bool(EnemyActor*) #2 (manager)
constexpr uint32_t fn_hitEffect = 0xF6E01C;           // deployHitEffect lambda (node, manager)
constexpr uint32_t fn_jumpUp = 0xF6DFC4;              // doJumpUpAction lambda (avatar)
constexpr uint32_t fn_jumpFall1 = 0xF6DF98;           // doJumpFallAction lambda #1 (avatar)
constexpr uint32_t fn_jumpFall2 = 0xF6DF6C;           // doJumpFallAction lambda #2 (avatar)
constexpr uint32_t fn_approach = 0xF6DFF0;            // doJumpApprpachAction lambda (avatar, dir, target)
constexpr uint32_t fn_allyAttack = 0xF6DF40;          // startAllyAttack lambda (manager, ally, dt)
constexpr uint32_t fn_disengage1 = 0xF6DF14;          // doDisengageAttackEnd lambda #1 (avatar)
constexpr uint32_t fn_disengage2 = 0xF6DEE8;          // doDisengageAttackEnd lambda #2 (avatar)
}  // namespace g

// EnemyActor fields (byte offsets into the guest object; the class is another module)
namespace en {
constexpr uint32_t POSITION = 624;        // cocos2d::Vec2 map position (MapNode)
constexpr uint32_t CONDITION_LOCK = 648;  // uint8: a condition (type 1) is holding it; cleared by reflectionCondition
constexpr uint32_t ALLY = 650;            // uint8: allied enemy (team flag 0x20)
constexpr uint32_t LEADER = 651;          // uint8: the team's leader symbol (avatar looks at it)
constexpr uint32_t ACTIVE = 652;          // uint8: in the battle, targetable
constexpr uint32_t KILLABLE = 653;        // uint8: may play its death now
constexpr uint32_t TEAM = 656;            // int: team index
constexpr uint32_t STATE = 672;           // int: 2 = symbol on the map, 3 = fighting
constexpr uint32_t DEPLOY_STATE = 676;    // int: symbol deployment (0 hidden, 2 shown)
constexpr uint32_t STATUS = 680;          // SafeBuffer: 8 = HP, 44 = attack order, 48 = pending damage
constexpr uint32_t BADSTATUS = 712;       // property holder (word 0 -> {getter, setter})
constexpr uint32_t BUFF = 716;            // IBuffProtocol
constexpr uint32_t RANDOM_MARK = 720;     // uint8: picked by SelectRandom / random burst
constexpr uint32_t DAMAGE_FLAGS = 724;    // BattleMisc +160 after the damage computation (bit 0..2)
constexpr uint32_t PARAM = 728;           // std::shared_ptr<SafeBuffer> enemy row: 0x270 jump kind, 0x278 attribute,
constexpr uint32_t PARAM_CTRL = 732;      //   0x334 knockback kind, 0x388.. immunities, 0x398.. resist rates
constexpr uint32_t SYMBOL_KIND = 756;     // int: symbol kind (2, 4 approach; 3, 4 hidden until near; 5 tap only)
constexpr uint32_t FLASH = 816;           // FlashNode*
constexpr uint32_t HP_INDICATOR = 960;    // RaidBossHPIndicator*
constexpr uint32_t PRIZES = 1024;         // cocos2d::Vector<MapPrizes*> dropped prizes
}  // namespace en

// AvatarActor fields
namespace av {
constexpr uint32_t POSITION = 624;        // cocos2d::Vec2
constexpr uint32_t AVATAR = 648;          // Avatar* (a FlashNode)
constexpr uint32_t COVERED = 661;         // uint8
constexpr uint32_t KNOCKBACK_ON = 664;    // int: knockback enabled
constexpr uint32_t KNOCKBACK = 676;       // float: knockback distance factor
constexpr uint32_t NO_STATUS_UPDATE = 788;  // uint8
}  // namespace av

// FlashNode (Avatar / enemy flash) fields
namespace fl {
constexpr uint32_t KNOCKBACK_FLAGS = 0x4DC;   // pending knockback flags
constexpr uint32_t ACT_FLAGS = 0x4E0;         // std::deque<ActFlag>
constexpr uint32_t SHIFT_FLAGS = 0x4F4;       // pending action flags (flagShift)
constexpr uint32_t BAD_STATUS = 0x514;
constexpr uint32_t ENEMY_ACTION = 0x520;      // enemy action kind
constexpr uint32_t MOTION = 0x524;            // avatar motion (47 = burst)
constexpr uint32_t HIT_SCALE = 0x52C;         // float
constexpr uint32_t HEIGHT = 0x534;            // float
constexpr uint32_t EXPANSION = 0x558;         // uint8
}  // namespace fl

using AttackTarget = StageActorManager::AttackTarget;

uint32_t ld(uint32_t a) { return rw::load<uint32_t>(a); }
int32_t ldi(uint32_t a) { return rw::load<int32_t>(a); }
uint8_t ld8(uint32_t a) { return rw::load<uint8_t>(a); }
float ldf(uint32_t a) { return rw::load<float>(a); }
SafeBuffer *sb(uint32_t a) { return rw::host_ptr<SafeBuffer>(a); }
uint32_t vc(uint32_t obj, uint32_t slot, std::initializer_list<uint32_t> args) {
    return rw::vcall(rw::host_ptr<void>(obj), slot, args);
}
void addShared(uint32_t ctrl) {
    if (ctrl) rw::call(g::add_shared, {ctrl});
}
uint32_t isDied(uint32_t e) { return rw::call(g::enemy_isDied, {e}); }                // EnemyActor::isDied
void actionMarker(uint32_t e, uint32_t tap) { rw::call(g::enemy_actionMarker, {e, tap}); }  // EnemyActor::actionMarker
// The StageState SafeBuffer (AppState::getInstance() + 32).
uint32_t stageState() { return ld(rw::call(g::appState_getInstance, {}) + 32); }
// A 32-bit field of the medal's SafeBuffer (MedalInfo word 0).
int32_t medalGet(uint32_t medalInfo, uint32_t index) { return sb(ld(medalInfo))->get32(index); }
// A 32-bit field of the enemy's master row, read while holding a reference (the inlined shared_ptr copy).
int32_t enemyParam(uint32_t e, uint32_t index) {
    uint32_t ctrl = ld(e + en::PARAM_CTRL);
    uint32_t row = ld(e + en::PARAM);
    addShared(ctrl);
    int32_t v = sb(row)->get32(index);
    bt::releaseShared(ctrl);
    return v;
}
// master::Burst of the medal (its id at 0x4AC), into the sret shared_ptr at frame + off.
void createBurst(const bt::GuestFrame &f, uint32_t off, uint32_t medalInfo) {
    int32_t id = medalGet(medalInfo, 0x4AC);
    rw::call(g::burst_create, {f.addr(off), uint32_t(id)});   // master::Burst::create
}
// AttackTarget copy assignment from the guest object at src.
void assignTarget(AttackTarget *dst, uint32_t src) {
    uint32_t type = ld(src), medal = ld(src + 4), obj = ld(src + 8), ctrl = ld(src + 12);
    dst->medalInfo = medal;
    dst->type = int32_t(type);
    addShared(ctrl);
    dst->skill.obj = obj;
    uint32_t old = dst->skill.ctrl;
    dst->skill.ctrl = ctrl;
    bt::releaseShared(old);
    uint32_t enemy = ld(src + 16), f14 = ld(src + 20);
    dst->field_14 = f14;
    dst->enemy = enemy;
}
// std::vector<T*>::clear() inlined: end = begin (computed the way the compiler did).
void clearPtrVector(bt::Vector &v) {
    uint32_t b = v.begin, e = v.end;
    if (e != b) v.end = e + (~(e - 4 - b) & ~3u);
}
// The inlined std::vector<EnemyActor*>::push_back.
void pushBackPtr(uint32_t vec, uint32_t e, uint32_t tmp) {
    bt::Vector *v = rw::host_ptr<bt::Vector>(vec);
    uint32_t end = v->end;
    if (end != v->cap) {
        uint32_t nb = 0;
        if (end) {
            rw::store32(rw::host_ptr<void>(end), e);
            nb = v->end;
        }
        v->end = nb + 4;
    } else {
        rw::save<uint32_t>(tmp, e);
        rw::call(g::push_back_slow, {vec, tmp});              // __push_back_slow_path
    }
}
// A std::function holding a small lambda, in place at frame + off (callable pointer at +16 -> the buffer).
void makeFunction(const bt::GuestFrame &f, uint32_t off, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    f.setWord(off + 16, f.addr(off));
    f.setWord(off, vtable);
    uint32_t k = 4;
    for (uint32_t c : captures) {
        f.setWord(off + k, c);
        k += 4;
    }
}
// std::function destructor: in-place callable -> destroy() (slot 4), heap callable -> destroy_deallocate() (slot 5).
void destroyFunctionAt(uint32_t fn) {
    uint32_t f = ld(fn + 16);
    if (f == fn) vc(fn, 4, {fn});
    else if (f) vc(f, 5, {f});
}
void destroyFunction(const bt::GuestFrame &f, uint32_t off) { destroyFunctionAt(f.addr(off)); }
// An AttackTarget built on the guest stack.
AttackTarget *stackTarget(const bt::GuestFrame &f, uint32_t off, uint32_t type, uint32_t medalInfo, uint32_t enemy) {
    f.setWord(off, type);
    f.setWord(off + 4, medalInfo);
    f.setWord(off + 8, 0);
    f.setWord(off + 12, 0);
    f.setWord(off + 16, enemy);
    f.setWord(off + 20, 0);
    return f.ptr<AttackTarget>(off);
}
// Hit sound of an attack (heavy hit when the damage flags have bit 1).
void playHitSE(const bt::GuestFrame &f, uint32_t off, uint32_t flags) {
    uint32_t id = (flags & 2) ? 502000120u : 502000090u;
    uint32_t mgr = rw::call(g::audio_getInstance, {});           // audio::Manager::getInstance
    rw::call(g::audio_getLayer, {f.addr(off), mgr, 0});          // audio::Manager::getLayer (sret)
    rw::call(g::audio_playSE, {f.word(off), id, 0, 0});          // audio::Layer::playSE
    bt::releaseShared(f.word(off + 4));
}
// Clear a bad status property: holder's getter (sret StatusChange, holder, 1), StatusChange::clear, setter.
void clearStatus(const bt::GuestFrame &f, uint32_t off, uint32_t holder) {
    uint32_t p = ld(holder);
    uint32_t getter = ld(p), setter = ld(p + 4);
    rw::call(getter, {f.addr(off), holder, 1});
    uint32_t r = rw::call(g::statusChange_clear, {f.addr(off)});  // StatusChange::clear
    rw::call(setter, {holder, 1, r});
}
uint32_t firstLeader(const bt::Vector &team) {
    for (uint32_t it = team.begin, end = team.end; it != end; it += 4) {
        uint32_t e = ld(it);
        if (ld8(e + en::LEADER)) return e;
    }
    return 0;
}
}  // namespace

// ============================================================================================ lifetime
StageActorManager::AttackTarget *StageActorManager::AttackTarget::destruct(AttackTarget *self) {
    bt::releaseShared(self->skill.ctrl);
    return self;
}
RW_FUNCTION(0x74E988, "StageActorManager::AttackTarget::~AttackTarget", StageActorManager::AttackTarget::destruct);

StageActorManager *StageActorManager::create() {
    auto *self = rw::host_ptr<StageActorManager>(rw::call(bt::fn::operator_new, {0x1B0}));
    construct(self);
    if (rw::vcall(self, 2, {rw::arg(self)}) == 1) {           // init()
        rw::call(bt::fn::ref_autorelease, {rw::arg(self)});   // cocos2d::Ref::autorelease
        return self;
    }
    rw::vcall(self, 1, {rw::arg(self)});                       // deleting destructor
    return nullptr;
}
RW_FUNCTION(0x5FA1C2, "StageActorManager::create", StageActorManager::create);

StageActorManager *StageActorManager::construct(StageActorManager *self) {
    uint32_t s = rw::arg(self);
    rw::call(g::ref_ctor, {s});                               // cocos2d::Ref::Ref
    self->playStyle = 0;
    rw::call(g::aeabi_memset, {s + 20, 0x18, 0});             // avatars, player, friend, medal deck
    self->vtable = VTABLE;
    rw::call(g::aeabi_memset, {s + 52, 0x31, 0});             // the enemy / node vectors, flag +100
    self->crushed.begin = 0;
    self->crushed.end = 0;
    self->crushed.cap = 0;
    self->field = 0;
    self->lastEncountTeam = -1;
    self->alliesJoin = 0;
    self->encountKind = 0;
    self->tappedSymbol = 0;
    self->leaderSymbol = 0;
    self->direction = 0;
    self->scene = 0;
    for (uint32_t k = 0; k < 6; k++) rw::call(g::aeabi_memset, {s + 160 + 24 * k, 0x18, 0});
    self->targetCapacity = 6;
    self->targetCount = 0;
    self->targetExecuted = 0;
    self->cutInShader = 0;
    // currentTarget(NullTarget)
    uint32_t n = NULL_TARGET;
    self->currentTarget.medalInfo = ld(n + 4);
    self->currentTarget.type = ldi(n);
    self->currentTarget.skill.obj = ld(n + 8);
    uint32_t ctrl = ld(n + 12);
    self->currentTarget.skill.ctrl = ctrl;
    addShared(ctrl);
    uint32_t enemy = ld(n + 16), f14 = ld(n + 20);
    self->currentTarget.field_14 = f14;
    self->currentTarget.enemy = enemy;
    self->canTapEnemy.f = 0;
    self->onTapEnemy.f = 0;
    self->coveringFireState = 0;
    self->coveringFireType = 0;
    self->onEnemyAttack.f = 0;
    self->jumpTarget = 0;
    rw::save<uint32_t>(s + 428, 0);                           // jumpOffset = 0.0f
    return self;
}
RW_FUNCTION(0x74E9A0, "StageActorManager::StageActorManager", StageActorManager::construct);

StageActorManager *StageActorManager::destruct(StageActorManager *self) {
    uint32_t s = rw::arg(self);
    self->vtable = VTABLE;
    rw::call(g::enemyVec_clear, {s + 52});                    // cocos2d::Vector<EnemyActor*>::clear
    rw::call(g::enemyVec_clear, {s + 64});
    rw::call(g::enemyVec_clear, {s + 76});
    destroyFunctionAt(s + 400);                               // ~std::function onEnemyAttack
    destroyFunctionAt(s + 368);                               // ~std::function onTapEnemy
    destroyFunctionAt(s + 344);                               // ~std::function canTapEnemy
    bt::releaseShared(self->currentTarget.skill.ctrl);        // ~AttackTarget currentTarget
    for (int k = 5; k >= 0; k--) bt::releaseShared(self->targets[k].skill.ctrl);  // ~AttackTarget targets[]
    rw::call(g::vectorBase_dtor, {s + 104});                  // ~std::vector crushed
    rw::call(g::mapNodeVec_dtor, {s + 88});                   // cocos2d::Vector<MapNode*>::~Vector
    rw::call(g::enemyVec_dtor, {s + 76});                     // cocos2d::Vector<EnemyActor*>::~Vector
    rw::call(g::enemyVec_dtor, {s + 64});
    rw::call(g::enemyVec_dtor, {s + 52});
    rw::call(g::base_dtor, {s});                              // BaseActorManager::~BaseActorManager
    return self;
}
RW_FUNCTION(0x74EB24, "StageActorManager::~StageActorManager", StageActorManager::destruct);

void StageActorManager::destructDelete(StageActorManager *self) {
    destruct(self);
    rw::call(bt::fn::operator_delete, {rw::arg(self)});       // operator delete
}
RW_FUNCTION(0x74ED50, "StageActorManager::~StageActorManager(deleting)", StageActorManager::destructDelete);

// Preload the two hit sounds.
uint32_t StageActorManager::init(StageActorManager *) {
    uint32_t mgr = rw::call(g::audio_getInstance, {});        // audio::Manager::getInstance
    rw::call(g::audio_load, {mgr, 502000090});                // audio::Manager::load
    mgr = rw::call(g::audio_getInstance, {});
    rw::call(g::audio_load, {mgr, 502000120});
    return 1;
}
RW_FUNCTION(0x74ED5C, "StageActorManager::init", StageActorManager::init);

// ============================================================================================ field map
// Create the enemy actors of every team still alive on this stage: the first member of a team is its symbol
// on the map (symbol info, facing, leader flag), allied teams (flag 0x20) go to the allies list, every enemy
// gets its callbacks, its status board and the prizes it drops.
void StageActorManager::setupEnemyActorPlatoon(StageActorManager *self, uint32_t fm, bt::Vector *teams) {
    uint32_t s = rw::arg(self);
    rw::call(g::enemyVec_clear, {s + 52});                    // cocos2d::Vector<EnemyActor*>::clear
    rw::call(g::enemyVec_clear, {s + 64});
    rw::call(g::enemyVec_clear, {s + 76});
    self->leaderSymbol = 0;
    if (teams->end == teams->begin) return;
    // +0 Vec2, +8 MapPrizes::Info (80 bytes, items at +16), +0x58 / +0x70 / +0x88 std::function temporaries
    bt::GuestFrame f(0xA0);
    uint32_t ti = 0;
    do {
        uint32_t info = ld(teams->begin + 4 * ti);             // MapEnemyTeamInfo*
        uint32_t members = ld(info), count = ld(info + 4);
        uint32_t app = rw::call(g::appState_getInstance, {});  // AppState::getInstance
        if (rw::call(g::stageState_hasExtermination, {ld(app + 32), ld(info + 16)}) == 0 && count != 0) {
            uint32_t m = members;                             // EnemyActorInfo[] (32 bytes each)
            uint32_t k = 0;
            do {
                uint32_t e = rw::call(g::enemy_create, {m, ld(ld(s + 152) + 628)});  // EnemyActor::create
                if (k == 0) {
                    rw::store8(rw::host_ptr<uint8_t>(e + 789), ld8(info + 30) & 1);
                    uint32_t flags = ld(info + 8);
                    rw::call(g::enemy_setSymbolInfo,          // EnemyActor::setSymbolInfo
                             {e, ld(info + 20), ld(info + 24), flags & 1, (flags & 0x40) >> 6});
                    uint32_t setDirection = ld(ld(e) + 40);
                    uint16_t raw = rw::load<uint16_t>(info + 28);
                    uint32_t dir;
                    if (raw) dir = bt::bits(float(int16_t(raw)));
                    else dir = rw::call(g::field_autoDirection, {fm, ld(members + 24)});  // getAutoDirectionFromField
                    rw::call(setDirection, {e, dir});
                    rw::save<uint32_t>(e + 828, (ld(info + 8) >> 2) & 3);
                    if (ld8(info + 8) & 1) self->leaderSymbol = e;
                }
                rw::call(g::enemy_setTeamInfo, {e, ti, k});   // EnemyActor::setTeamInfo
                rw::call(g::enemy_setStatusBoard, {e, ld(m + 8), count});  // EnemyActor::setStatusBoard
                makeFunction(f, 0x58, g::fn_enemyFF, {s});
                rw::call(g::fnBoolFF_assign, {e + 928, f.addr(0x58)});    // std::function<bool(float,float)>::operator=
                destroyFunction(f, 0x58);
                makeFunction(f, 0x70, g::fn_enemyB, {s});
                rw::call(g::fnBoolEnemy_assign, {e + 856, f.addr(0x70)}); // std::function<bool(EnemyActor*)>::operator=
                destroyFunction(f, 0x70);
                makeFunction(f, 0x88, g::fn_enemyC, {s});
                rw::call(g::fnBoolEnemy_assign, {e + 880, f.addr(0x88)});
                destroyFunction(f, 0x88);
                rw::call(g::enemy_preloadResources, {e});    // EnemyActor::preloadResources
                uint32_t list;
                if (ld8(info + 8) & 0x20) {
                    rw::store8(rw::host_ptr<uint8_t>(e + en::ALLY), 1);
                    list = s + 64;
                } else {
                    list = s + 52;
                }
                rw::call(g::enemyVec_pushBack, {list, e});    // cocos2d::Vector<EnemyActor*>::pushBack
                rw::call(g::field_addMapNode, {fm, e});       // StageFieldManager::addMapNodeToField
                uint32_t layer = ld(fm + 20);
                vc(layer, 60, {layer, ld(e + 0x40C)});
                rw::call(g::vec2_copy, {f.addr(0), g::vec2_ZERO});        // Vec2(Vec2::ZERO)
                rw::call(g::prizesInfo_ctor, {f.addr(8), f.addr(0), 1});  // MapPrizes::Info(Vec2, 1)
                rw::call(g::vec2_dtor, {f.addr(0)});
                if (rw::call(g::stageUtil_dropPrizes, {f.addr(16), ld(m + 20), ld(m + 16), 0}) == 1) {
                    uint32_t prizes = rw::call(g::prizes_create, {f.addr(8), ld(ld(s + 152) + 628)});  // MapPrizes::create
                    rw::call(g::enemy_setDropItem, {e, prizes});  // EnemyActor::setDropItem
                }
                rw::call(g::vec2_dtor, {f.addr(8)});          // ~Info (its Vec2)
                m += 32;
                k++;
            } while (k < count);
        }
        ti++;
    } while (ti < uint32_t(int32_t(teams->end - teams->begin) >> 2));
}
RW_FUNCTION(0x74ED80, "StageActorManager::setupEnemyActorPlatoon", StageActorManager::setupEnemyActorPlatoon);

// Show every symbol enemy waiting on the map (state 2).
void StageActorManager::showWanderingSymbolEnemy(StageActorManager *self) {
    for (uint32_t it = self->mapEnemies.begin, end = self->mapEnemies.end; it != end; it += 4) {
        uint32_t e = ld(it);
        if (ld(e + en::STATE) == 2) rw::call(g::enemy_deploy, {e, 1});  // EnemyActor::deploy
    }
    for (uint32_t it = self->allies.begin, end = self->allies.end; it != end; it += 4) {
        uint32_t e = ld(it);
        if (ld(e + en::STATE) == 2) rw::call(g::enemy_deploy, {e, 1});
    }
}
RW_FUNCTION(0x74F15C, "StageActorManager::showWanderingSymbolEnemy", StageActorManager::showWanderingSymbolEnemy);

// Which team the avatar runs into: a tapped symbol, a symbol within its encounter range, or the team's
// encounter area. The team members go to the battle list (counted per attribute) and face the avatar;
// teams with flag 1 call the allies in. Hidden symbols (kinds 3, 4) appear / vanish with the distance.
bool StageActorManager::checkEncount(StageActorManager *self, uint32_t sm, uint32_t avatar) {
    uint32_t s = rw::arg(self);
    uint32_t pos = avatar + av::POSITION;
    int32_t team = -1;
    uint32_t tapped = self->tappedSymbol;
    if (tapped) {
        self->encountKind = ldi(tapped + en::SYMBOL_KIND);
        team = ldi(tapped + en::TEAM);
        self->tappedSymbol = 0;
    } else {
        for (uint32_t it = self->mapEnemies.begin, end = self->mapEnemies.end; it != end; it += 4) {
            uint32_t e = ld(it);
            if (ld(e + en::STATE) != 2) continue;
            int32_t kind = ldi(e + en::SYMBOL_KIND);
            uint32_t range;
            if (kind == 2) range = ld(sm + 356);
            else if (kind == 5) continue;
            else if (kind == 4) range = ld(sm + 356);
            else range = 0;
            if (rw::call(g::enemy_isEncount, {e, pos, range}) == 1) {   // EnemyActor::isEncount
                self->encountKind = kind;
                team = ldi(e + en::TEAM);
                break;
            }
            if (rw::call(g::sm_isInsideTeamEncountArea, {sm, pos, ld(e + en::TEAM)}) == 1) {
                self->encountKind = kind;
                team = ldi(e + en::TEAM);
                break;
            }
            if (uint32_t(kind - 3) <= 1) {
                uint32_t r = rw::call(g::enemy_isEncount, {e, pos, ld(sm + 360)});
                uint32_t st = ld(e + en::DEPLOY_STATE);
                if (r == 1) {
                    if (st == 0) rw::call(g::enemy_deploy, {e, 1});      // EnemyActor::deploy
                } else if (st == 2) {
                    rw::call(g::enemy_deploy, {e, 0});
                }
            }
        }
    }
    if (team >= 0) {
        rw::call(g::enemyVec_clear, {s + 76});                // cocos2d::Vector<EnemyActor*>::clear
        self->attrCount[0] = 0;
        self->attrCount[1] = 0;
        self->attrCount[2] = 0;
        uint32_t lead = 0;
        for (uint32_t it = self->mapEnemies.begin, end = self->mapEnemies.end; it != end; it += 4) {
            uint32_t e = ld(it);
            if (ldi(e + en::TEAM) != team) continue;
            rw::call(g::enemy_setEncountAction, {e, 1});      // EnemyActor::setEncountAction
            rw::call(g::enemy_lookAt, {e, pos});              // EnemyActor::lookAt
            rw::call(g::enemyVec_pushBack, {s + 76, e});      // cocos2d::Vector<EnemyActor*>::pushBack
            int32_t attr = enemyParam(e, 0x278);
            if (attr == 1) self->attrCount[0] += 1;
            else if (attr == 2) self->attrCount[1] += 1;
            else if (attr == 3) self->attrCount[2] += 1;
            if (ld8(e + en::LEADER)) {
                rw::call(g::avatarActor_lookAt, {avatar, e + en::POSITION});  // AvatarActor::lookAt
                lead = e;
            }
        }
        uint32_t info = rw::call(g::sm_getEnemyTeamInfo, {sm, uint32_t(team)});  // StageManager::getEnemyTeamInfo
        if (ld8(info + 8) & 1) {
            self->alliesJoin = 1;
            for (uint32_t it = self->allies.begin, end = self->allies.end; it != end; it += 4) {
                uint32_t e = ld(it);
                rw::call(g::enemy_setEncountAction, {e, 1});
                if (lead) rw::call(g::enemy_lookAt, {e, lead + en::POSITION});
            }
        }
    }
    self->lastEncountTeam = team;
    return team != -1;
}
RW_FUNCTION(0x74F1A4, "StageActorManager::checkEncount", StageActorManager::checkEncount);

// Start the battle actions of the encountered team: the lowest attack order of the active members becomes
// everyone's order; fails when more members are fighting already than the team allows.
uint32_t StageActorManager::checkEncountOrder(StageActorManager *self, uint32_t sm) {
    int32_t teamIndex = self->lastEncountTeam;
    if (teamIndex < 0) return 0;
    uint32_t info = rw::call(g::sm_getEnemyTeamInfo, {sm, uint32_t(teamIndex)});  // StageManager::getEnemyTeamInfo
    if (!info) return 0;
    int32_t order = 0x7FFFFFFF;
    uint32_t it = self->team.begin, end = self->team.end;
    if (it != end) {
        uint32_t fighting = 0;
        for (; it != end; it += 4) {
            uint32_t e = ld(it);
            int32_t st = ldi(e + en::STATE);
            if (st == 3) {
                fighting++;
            } else if (uint32_t(st - 1) <= 1) {
                SafeBuffer *status = sb(e + en::STATUS);
                if (status->get32(44) < order) order = status->get32(44);
            }
        }
        if (fighting > ld(info + 12)) return 0;
    }
    for (it = self->team.begin, end = self->team.end; it != end; it += 4) {
        uint32_t e = ld(it);
        if (ldi(e + en::STATE) <= 2) rw::call(g::enemy_setEncountAction, {e, uint32_t(order)});
    }
    return 1;
}
RW_FUNCTION(0x74F474, "StageActorManager::checkEncountOrder", StageActorManager::checkEncountOrder);

uint32_t StageActorManager::getLastEncountTeamUniqueID(StageActorManager *self, uint32_t sm) {
    int32_t t = self->lastEncountTeam;
    if (t == -1) return 0;
    return ld(rw::call(g::sm_getEnemyTeamInfo, {sm, uint32_t(t)}) + 16);  // StageManager::getEnemyTeamInfo
}
RW_FUNCTION(0x74F594, "StageActorManager::getLastEncountTeamUniqueID", StageActorManager::getLastEncountTeamUniqueID);

uint32_t StageActorManager::getLastEncountTeamInfo(StageActorManager *self, uint32_t sm) {
    int32_t t = self->lastEncountTeam;
    if (t == -1) return 0;
    return ld(rw::call(g::sm_getEnemyTeamInfo, {sm, uint32_t(t)}) + 8);
}
RW_FUNCTION(0x74F5B0, "StageActorManager::getLastEncountTeamInfo", StageActorManager::getLastEncountTeamInfo);

// The leader of the encountered team (0 = none).
uint32_t StageActorManager::getCurrentEncountSymbolEnemyActor(StageActorManager *self) {
    return firstLeader(self->team);
}
RW_FUNCTION(0x74F5CC, "StageActorManager::getCurrentEncountSymbolEnemyActor",
            StageActorManager::getCurrentEncountSymbolEnemyActor);

// ============================================================================================ attack queue
void StageActorManager::clearCurrentTarget(StageActorManager *self) {
    for (uint32_t k = 0; k < 6; k++) assignTarget(&self->targets[k], NULL_TARGET);
    self->targetCount = 0;
    self->targetExecuted = 0;
    clearPtrVector(self->crushed);
}
RW_FUNCTION(0x74F5F4, "StageActorManager::clearCurrentTarget", StageActorManager::clearCurrentTarget);

// Queue an attack on the whole team (once per turn).
uint32_t StageActorManager::entryWholeAttack(StageActorManager *self, uint32_t medalInfo) {
    if (self->attackEntered) return 0;
    self->attackEntered = 1;
    bt::GuestFrame f(24);
    return entryCurrentTarget(self, stackTarget(f, 0, 2, medalInfo, 0));
}
RW_FUNCTION(0x74F684, "StageActorManager::entryWholeAttack", StageActorManager::entryWholeAttack);

// Append an attack to the queue (a full, fully executed queue starts over) and mark the targets.
uint32_t StageActorManager::entryCurrentTarget(StageActorManager *self, const AttackTarget *target) {
    uint32_t s = rw::arg(self);
    int32_t cap = self->targetCapacity;
    int32_t count;
    if (cap <= self->targetExecuted) {
        for (uint32_t k = 0; k < 6; k++) assignTarget(&self->targets[k], NULL_TARGET);
        self->targetCount = 0;
        self->targetExecuted = 0;
        cap = self->targetCapacity;
        count = 0;
    } else {
        count = self->targetCount;
    }
    if (count >= cap) return 0;
    assignTarget(rw::host_ptr<AttackTarget>(s + 160 + uint32_t(count) * 24), rw::arg(target));
    self->targetCount += 1;
    if (self->playStyle == 2) return 1;
    int32_t type = target->type;
    if (type == 2) {
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
            uint32_t e = ld(it);
            if (ld8(e + en::ACTIVE) && isDied(e) == 0) actionMarker(e, 2);
        }
    } else if (type == 1) {
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
            uint32_t e = ld(it);
            if (ld8(e + en::ACTIVE)) actionMarker(e, 4);
        }
        actionMarker(target->enemy, 2);
    }
    return 1;
}
RW_FUNCTION(0x74F6F4, "StageActorManager::entryCurrentTarget", StageActorManager::entryCurrentTarget);

uint32_t StageActorManager::hasAliveEnemyInTeam(StageActorManager *self) {
    for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4)
        if (isDied(ld(it)) == 0) return 1;
    return 0;
}
RW_FUNCTION(0x74F870, "StageActorManager::hasAliveEnemyInTeam", StageActorManager::hasAliveEnemyInTeam);

uint32_t StageActorManager::isAnnihilationMapEnemy(StageActorManager *self) {
    for (uint32_t it = self->mapEnemies.begin, end = self->mapEnemies.end; it != end; it += 4)
        if (isDied(ld(it)) == 0) return 0;
    return 1;
}
RW_FUNCTION(0x74F89A, "StageActorManager::isAnnihilationMapEnemy", StageActorManager::isAnnihilationMapEnemy);

uint32_t StageActorManager::hasActiveAlly(StageActorManager *self) {
    for (uint32_t it = self->allies.begin, end = self->allies.end; it != end; it += 4)
        if (ld8(ld(it) + en::ACTIVE)) return 1;
    return 0;
}
RW_FUNCTION(0x74F8C4, "StageActorManager::hasActiveAlly", StageActorManager::hasActiveAlly);

// Take the next queued attack into currentTarget (single attacks on an enemy that died are skipped);
// NullTarget when the queue is done.
StageActorManager::AttackTarget *StageActorManager::executeAttackCurrentTarget(StageActorManager *self) {
    uint32_t s = rw::arg(self);
    AttackTarget *cur = &self->currentTarget;
    assignTarget(cur, NULL_TARGET);
    int32_t idx = self->targetExecuted;
    while (idx < self->targetCount) {
        assignTarget(cur, s + 160 + uint32_t(idx) * 24);
        idx = self->targetExecuted + 1;
        self->targetExecuted = idx;
        if (cur->type != 1) return cur;
        uint32_t e = cur->enemy;
        if (e) {
            if (isDied(e) != 1) return cur;
            idx = self->targetExecuted;
        }
    }
    assignTarget(cur, NULL_TARGET);
    return cur;
}
RW_FUNCTION(0x74F8EC, "StageActorManager::executeAttackCurrentTarget", StageActorManager::executeAttackCurrentTarget);

// The living active team member of this attribute (0 = any) with the lowest HP.
uint32_t StageActorManager::choiceTargetAttributeWithLowerHp(StageActorManager *self, int32_t attribute) {
    uint32_t best = 0;
    for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
        uint32_t e = ld(it);
        if (!ld8(e + en::ACTIVE) || isDied(e) != 0) continue;
        int32_t attr = enemyParam(e, 0x278);
        if (attr != attribute && attribute != 0) continue;
        if (best) {
            int32_t hp = sb(e + en::STATUS)->get32(8);
            int32_t bestHp = sb(best + en::STATUS)->get32(8);
            if (!(hp < bestHp)) e = best;
        }
        best = e;
    }
    return best;
}
RW_FUNCTION(0x74FA3C, "StageActorManager::choiceTargetAttributeWithLowerHp",
            StageActorManager::choiceTargetAttributeWithLowerHp);

// Auto attack: in a raid the boss; otherwise the weakest enemy the medal's attribute beats, then the same
// attribute, the second advantage, any.
void StageActorManager::autoChoiceTarget(StageActorManager *self, uint32_t medalInfo, uint32_t burst) {
    if (self->attackEntered) return;
    bt::GuestFrame f(24);
    if (self->playStyle == 2) {
        entryCurrentTarget(self, stackTarget(f, 0, burst == 1 ? 3 : 1, medalInfo, 0));
        return;
    }
    int32_t attr = medalGet(medalInfo, 0x458);
    uint32_t slot = g::advantageTable + uint32_t(attr) * 4;
    uint32_t e = choiceTargetAttributeWithLowerHp(self, ldi(slot));
    if (!e) e = choiceTargetAttributeWithLowerHp(self, attr);
    if (!e) e = choiceTargetAttributeWithLowerHp(self, ldi(slot + 0x14));
    if (!e) e = choiceTargetAttributeWithLowerHp(self, 0);
    if (!e) return;
    self->attackEntered = 1;
    entryCurrentTarget(self, stackTarget(f, 0, 1, medalInfo, e));
}
RW_FUNCTION(0x74FB80, "StageActorManager::autoChoiceTarget", StageActorManager::autoChoiceTarget);

// Map symbols switch their display mode (virtual slot 44 with !mode).
void StageActorManager::changeMode(StageActorManager *self, uint32_t mode) {
    uint32_t m = mode ^ 1;
    for (uint32_t it = self->mapEnemies.begin, end = self->mapEnemies.end; it != end; it += 4) {
        uint32_t e = ld(it);
        if (ld(e + en::STATE) == 2) vc(e, 44, {e, m});
    }
}
RW_FUNCTION(0x74FCBC, "StageActorManager::changeMode", StageActorManager::changeMode);

// Show which enemies the current medal's attribute is strong against (raid: the boss and its parts).
void StageActorManager::attackReadyAction(StageActorManager *self, uint32_t boss) {
    uint32_t info = rw::call(g::medalDeck_currentInfo, {self->medalDeck});  // MedalDeck::getCurrentMedalInfo
    int32_t attr = medalGet(info, 0x458);
    if (self->playStyle == 2) {
        rw::call(g::raidBoss_setAdvantage, {boss, uint32_t(attr), 0});  // RaidBossActor::setAdvantageousEffect
        bt::GuestFrame f(16);                                 // +0 boss row, +8 part row (shared_ptr)
        rw::call(g::raidEnemy_create, {f.addr(0), ld(boss + 0x8A4)});  // master::RaidEnemy::create
        int32_t parts = sb(f.word(0))->get32(0x94);
        (void)sb(f.word(0))->get32(0x90);
        // the original copies 5 part ids (0x98..0xAB) to its frame; more than 5 parts would read past them
        for (int32_t k = 0; k < parts; k++) {
            int32_t id = sb(f.word(0))->get32(0x98 + 4 * uint32_t(k));
            rw::call(g::raidEnemy_create, {f.addr(8), uint32_t(id)});
            int32_t part = sb(f.word(8))->get32(0x90);
            rw::call(g::raidBoss_setAdvantage, {boss, uint32_t(attr), uint32_t(part - 1)});
            bt::releaseShared(f.word(12));
        }
        bt::releaseShared(f.word(4));
    } else {
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4)
            rw::call(g::enemy_setAdvantage, {ld(it), uint32_t(attr)});  // EnemyActor::setAdvantageousEffect
    }
}
RW_FUNCTION(0x74FCEC, "StageActorManager::attackReadyAction", StageActorManager::attackReadyAction);

// End of the player's turn: reset the markers, drop the money prizes of the enemies killed this turn.
void StageActorManager::turnEndAction(StageActorManager *self, uint32_t money, uint32_t keepMarkers,
                                      uint32_t noMarker) {
    if (keepMarkers == 0) {
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4)
            if (noMarker == 0) actionMarker(ld(it), 4);
    }
    uint32_t it = self->crushed.begin, end = self->crushed.end;
    if (it != end) {
        uint32_t m = keepMarkers ? money : 0;
        for (; it != end; it += 4) rw::call(g::enemy_crushingMoney, {ld(it), m});  // EnemyActor::addCrushingMoneyPrizes
        clearPtrVector(self->crushed);
    }
    rw::store8(rw::host_ptr<uint8_t>(self->player + av::NO_STATUS_UPDATE), 0);
}
RW_FUNCTION(0x74FE50, "StageActorManager::turnEndAction", StageActorManager::turnEndAction);

void StageActorManager::enebleChoiceMarker(StageActorManager *self) {
    for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
        uint32_t e = ld(it);
        actionMarker(e, ld(e + en::STATE) == 3 ? 0 : 4);
    }
}
RW_FUNCTION(0x74FED8, "StageActorManager::enebleChoiceMarker", StageActorManager::enebleChoiceMarker);

uint32_t StageActorManager::battleFinishAction(StageActorManager *self, uint32_t bonus) {
    uint32_t node = rw::call(g::avatarActor_finishBonus, {self->player, bonus});  // AvatarActor::getShortTermFinishBonus
    if (node) rw::call(g::field_addMapNode, {self->field, node});  // StageFieldManager::addMapNodeToField
    return node;
}
RW_FUNCTION(0x74FF00, "StageActorManager::battleFinishAction", StageActorManager::battleFinishAction);

// ============================================================================================ per frame
void StageActorManager::update(StageActorManager *self, float dt) {
    uint32_t lead = firstLeader(self->team);
    updateAvatar(self, self->player, &self->currentTarget);
    uint32_t fr = self->friendAvatar;
    if (fr && vc(fr, 45, {fr}) == 1) updateAvatar(self, self->friendAvatar, &self->currentTarget);
    for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
        uint32_t e = ld(it);
        rw::call(g::mapNode_updatePosition, {e});             // MapNode::updateMapPosition
        rw::call(g::enemy_updateGauge, {e, bt::bits(dt)});    // EnemyActor::updateGauge
        updateEnemy(self, e, self->player);
    }
    bt::GuestFrame f(16);                                     // copy of a cocos2d::Vector<MapPrizes*>
    for (uint32_t it = self->mapEnemies.begin, end = self->mapEnemies.end; it != end; it += 4) {
        uint32_t e = ld(it);
        vc(e, 113, {e, bt::bits(dt)});
        rw::call(g::enemy_moveWandering, {e, bt::bits(dt)});  // EnemyActor::moveWandering
        uint32_t prizes = e + en::PRIZES;
        rw::call(g::prizesVec_copy, {f.addr(0), prizes});     // cocos2d::Vector<MapPrizes*>(const Vector&)
        for (uint32_t p = f.word(0), pend = f.word(4); p != pend; p += 4) {
            uint32_t pr = ld(p);
            if (rw::call(g::prizes_isEruption, {pr}) == 1) {  // MapPrizes::isEruption
                float z = ldf(pr + 628) + 10.0f;
                rw::call(g::mapNode_lockZ, {pr, bt::bits(z)});              // MapNode::lockMapPosZ
                rw::call(g::field_addMapNode, {self->field, pr});           // StageFieldManager::addMapNodeToField
                rw::call(g::prizesVec_erase, {prizes, pr, 0});              // cocos2d::Vector<MapPrizes*>::eraseObject
            }
        }
        rw::call(g::prizesVec_dtor, {f.addr(0)});
    }
    for (uint32_t it = self->allies.begin, end = self->allies.end; it != end; it += 4) {
        uint32_t e = ld(it);
        if (lead && ld8(e + en::ACTIVE)) updateEnemyEnemy(self, e, lead);
    }
}
RW_FUNCTION(0x74FF24, "StageActorManager::update", StageActorManager::update);

// The avatar's animation flags: hit (effects and sounds on the targets), kill flags, motion end; then the
// knockback frame: the damage of the attack lands (single target or the marked enemies).
void StageActorManager::updateAvatar(StageActorManager *self, uint32_t avatar, const AttackTarget *target) {
    // +0 std::vector targets, +0x10 Vec2 attack position, +0x18 float scale, +0x20 shared_ptr Burst,
    // +0x28 shared_ptr audio layer, +0x30 shared_ptr temporary
    bt::GuestFrame f(0x38);
    uint32_t fn = ld(avatar + av::AVATAR);
    if (ld8(fn + fl::EXPANSION)) {
        uint32_t node = ld(self->field + 20);
        uint32_t act = rw::call(g::avatar_expansionAction, {fn});  // Avatar::createExpansionAction
        rw::call(g::node_runAction, {node, act});             // cocos2d::Node::runAction
    }
    if (ld(fn + fl::SHIFT_FLAGS)) {
        uint32_t shift = rw::call(g::flash_flagShift, {fn});  // FlashNode::flagShift
        switch (shift) {
        case 1: {
            if (target->type == 0) break;
            uint32_t speed = vc(avatar, 11, {avatar});        // float
            if (target->type == 2) {
                getEncountTargets(f.ptr<bt::Vector>(0), self, target, 1);
                for (uint32_t it = f.word(0), end = f.word(4); it != end; it += 4) {
                    uint32_t id = vc(fn, 157, {fn});
                    uint32_t e = ld(it);
                    deployHitEffect(self, int32_t(id), e + en::POSITION, ldf(ld(e + en::FLASH) + fl::HIT_SCALE),
                                    bt::fromBits(speed));
                    if (target->type != 3) playHitSE(f, 0x28, ld(e + en::DAMAGE_FLAGS));
                }
            } else {
                uint32_t scale;
                if (target->type == 3) {
                    createBurst(f, 0x20, target->medalInfo);
                    SafeBuffer *burst = sb(f.word(0x20));
                    (void)burst->get32(0x160);
                    int32_t v = burst->get32(0x10);
                    scale = v == 1 ? 0x3F800000u : speed;
                    bt::releaseShared(f.word(0x24));
                } else {
                    scale = speed;
                }
                getEncountTargets(f.ptr<bt::Vector>(0), self, target, 1);
                computeAttackPosition(f.ptr<bt::Vec2>(0x10), self, target, f.ptr<float>(0x18));
                uint32_t id = vc(fn, 157, {fn});
                deployHitEffect(self, int32_t(id), f.addr(0x10), bt::fromBits(f.word(0x18)), bt::fromBits(scale));
                if (target->type != 3) playHitSE(f, 0x28, ld(target->enemy + en::DAMAGE_FLAGS));
                rw::call(g::vec2_dtor, {f.addr(0x10)});
            }
            if (ldi(fn + fl::MOTION) == 47) rw::call(g::mapNode_lockZ, {avatar, 0x7F7FFFFF});  // MapNode::lockMapPosZ(FLT_MAX)
            rw::call(g::vectorBase_dtor, {f.addr(0)});
            break;
        }
        case 2:                                               // 2: the targets may die now, 3: not yet
        case 3: {
            uint8_t v = shift == 2 ? 1 : 0;
            getEncountTargets(f.ptr<bt::Vector>(0), self, target, 0);
            for (uint32_t it = f.word(0), end = f.word(4); it != end; it += 4)
                rw::store8(rw::host_ptr<uint8_t>(ld(it) + en::KILLABLE), v);
            rw::call(g::vectorBase_dtor, {f.addr(0)});
            break;
        }
        case 4: {
            int32_t m = ldi(fn + fl::MOTION);
            bool idle;
            if (m > 44) idle = m == 45 || m == 47;
            else if (m > 30) idle = uint32_t(m - 31) < 8;
            else if (uint32_t(m) < 3) idle = true;
            else if (m == 7) break;
            else idle = m == 17;
            if (idle) {
                rw::call(g::avatar_changeMotion, {fn, 11});   // Avatar::changeMotion
                if (m == 47) rw::call(g::mapNode_unlockZ, {avatar});  // MapNode::unlockMapPosZ
            } else {
                rw::call(g::avatar_changeMotion, {fn, 27});
            }
            break;
        }
        default:
            break;
        }
    }
    if (!ld(fn + fl::KNOCKBACK_FLAGS)) return;
    int32_t flag = sb(target->medalInfo + 8)->get32(0x14);     // MedalInfo +8: the medal's own SafeBuffer
    uint32_t bobj = 0, bctrl = 0;
    if (target->type == 3) {
        createBurst(f, 0x20, target->medalInfo);
        bobj = f.word(0x20);
        bctrl = f.word(0x24);
    }
    rw::call(g::flash_knockbackShift, {fn});                  // FlashNode::knockbackShift
    if (isSingle(self, target)) {
        uint32_t e = target->enemy;
        f.setWord(0x30, bobj);
        f.setWord(0x34, bctrl);
        addShared(bctrl);
        EnemyDamage(self, e, f.ptr<bt::SharedPtr>(0x30), flag != 0);
        bt::releaseShared(bctrl);
        if (target->type == 1) {
            int32_t kind = sb(ld(target->enemy + en::PARAM))->get32(0x334);
            uint32_t pushed = kind == 3 ? avatar : uint32_t(target->enemy);
            uint32_t factor = 0;
            if (ld(avatar + av::KNOCKBACK_ON)) factor = ld(avatar + av::KNOCKBACK);
            uint32_t dir = vc(pushed, 11, {pushed});
            float push = bt::fromBits(factor) * bt::fromBits(dir);
            rw::save<float>(pushed + 624, push + ldf(pushed + 624));
        }
    } else {
        if (isRandom(self, target)) SelectRandom(self);
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
            uint32_t e = ld(it);
            if (!ld8(e + en::RANDOM_MARK)) continue;
            f.setWord(0x30, bobj);
            f.setWord(0x34, bctrl);
            addShared(bctrl);
            EnemyDamage(self, e, f.ptr<bt::SharedPtr>(0x30), flag != 0);
            bt::releaseShared(bctrl);
        }
    }
    bt::releaseShared(bctrl);
}
RW_FUNCTION(0x7500E8, "StageActorManager::updateAvatar", StageActorManager::updateAvatar);

// An enemy's animation flags against the avatar (hit effect, the avatar's cover flag, motion ends, death),
// then its attack frame: the counter attack lands (damage after the deck's defense and the damage skill,
// damage number, HP, status).
void StageActorManager::updateEnemy(StageActorManager *self, uint32_t e, uint32_t avatar) {
    uint32_t s = rw::arg(self);
    bt::GuestFrame f(0x20);                                   // +0 ActFlag, +8 Vec2, +0x10 StatusChange
    uint32_t fn = ld(e + en::FLASH);
    if (ld(fn + fl::SHIFT_FLAGS)) {
        switch (rw::call(g::flash_flagShift, {fn})) {         // FlashNode::flagShift
        case 1: {
            uint32_t id = rw::call(g::enemy_hitEffectID, {e});            // EnemyActor::getHitEffectID
            uint32_t pos = vc(avatar, 21, {avatar});
            uint32_t dir = rw::call(g::enemy_hitEffectDirection, {e});    // EnemyActor::getHitEffectDirection
            deployHitEffect(self, int32_t(id), pos, 70.0f, bt::fromBits(dir));
            break;
        }
        case 2:
            rw::store8(rw::host_ptr<uint8_t>(avatar + av::COVERED), 1);
            break;
        case 3:
            rw::store8(rw::host_ptr<uint8_t>(avatar + av::COVERED), 0);
            break;
        case 4: {
            int32_t m = ldi(fn + fl::ENEMY_ACTION);
            if (uint32_t(m - 3) <= 1) {
                rw::call(g::scene_enemyCountEnd, {self->scene, e});      // SceneBattleBase::enemy_count_end
                rw::call(g::enemy_changeMotion, {e, 1});                 // EnemyActor::changeMotion
            } else if (m == 6) {
                rw::call(g::enemy_setEndedAction, {e});                  // EnemyActor::setEndedAction
            } else if (m == 5) {
                if (isDied(e) == 0) rw::call(g::enemy_changeMotion, {e, 1});
            } else {
                rw::call(g::enemy_changeMotion, {e, 0});
            }
            break;
        }
        case 5:
            if (ld8(e + en::ACTIVE) && isDied(e) == 1) {
                if (ld8(e + en::KILLABLE)) {
                    rw::call(g::enemy_setDeadAction, {e});               // EnemyActor::setDeadAction
                    rw::call(g::flash_setBadStatus, {ld(e + en::FLASH), 4});  // FlashNode::set_bad_status
                } else {
                    f.setWord(0, 5);                                     // retry the death flag later
                    rw::call(g::deque_pushFront, {fn + fl::ACT_FLAGS, f.addr(0)});
                }
            }
            break;
        default:
            break;
        }
    }
    if (!ld(fn + fl::KNOCKBACK_FLAGS)) return;
    rw::call(g::flash_knockbackShift, {fn});                  // FlashNode::knockbackShift
    int32_t motion = ldi(ld(avatar + av::AVATAR) + fl::MOTION);
    if (motion > 30) {
        uint32_t d = uint32_t(motion - 31);
        if (d <= 16 && ((1u << d) & 0x140FFu)) return;        // the avatar is in a motion that cannot be hit
    } else if (uint32_t(motion) < 3) {
        return;
    }
    if (sb(stageState())->get32(8) == 0) return;              // no HP left
    uint32_t setDirection = ld(ld(avatar) + 40);
    uint32_t dir = vc(e, 11, {e});
    rw::call(setDirection, {avatar, dir ^ 0x80000000u});      // face the attacker
    rw::call(g::avatar_changeMotion, {ld(avatar + av::AVATAR), 17});  // Avatar::changeMotion (damage)
    rw::call(g::fnVoidBool_call, {s + 400, 0});               // onEnemyAttack(false)
    uint32_t ss = stageState();
    int32_t hits = sb(stageState())->get32(0x4268);
    sb(ss)->set32(0x4268, int32_t(uint32_t(hits) + 1));       // counter attacks taken
    uint32_t bm = rw::call(g::battleMisc_getInstance, {});    // BattleMisc::getInstance
    int32_t def = int32_t(rw::call(g::medalDeck_currentDefense, {self->medalDeck}));  // MedalDeck::getCurrentDefense
    int32_t dmg = int32_t(rw::call(g::battleMisc_enemyAttack, {bm, e, uint32_t(bt::f2iz(float(def)))}));
    uint32_t skill = rw::call(g::scene_damageSkill, {self->scene});  // SceneBattleBase::DamageSkill
    if (skill) {
        int32_t rate = sb(stageState())->get32(0x4238);
        int32_t q = int32_t(int64_t(rate) * int64_t(dmg) / 10000);
        dmg = q ? q : 1;
        if (self->direction) rw::call(g::direction_skillPanelIn, {self->direction, skill});  // CommonDirection::req_skilPanel_in
    }
    rw::call(g::flash_reflectionStatus, {ld(avatar + av::AVATAR), 0});  // FlashNode::reflection_status
    uint32_t bm2 = rw::call(g::battleMisc_getInstance, {});
    uint32_t node = ld(self->direction + 628);
    uint32_t pos = vc(avatar, 21, {avatar});
    rw::call(g::vec2_copy, {f.addr(8), pos});
    uint32_t flags = ld(bm2 + 160);
    rw::call(g::dispDamageNumber, {uint32_t(dmg), node, f.addr(8), 0x428C0000u, (flags & 2) >> 1, (flags & 4) >> 2, 0,
                                   flags & 1, 400, 0x3F800000u});  // DisplayDamageNumber::disp_dmgNum
    rw::call(g::vec2_dtor, {f.addr(8)});
    (void)sb(stageState())->get32(8);
    rw::call(g::stageState_addDamage, {stageState(), uint32_t(dmg)});  // StageState::addDamage
    if (ld8(avatar + av::NO_STATUS_UPDATE)) return;
    uint32_t app = rw::call(g::appState_getInstance, {});
    uint32_t state = ld(app + 32), ctrl = ld(app + 36);
    addShared(ctrl);
    if (state) clearStatus(f, 0x10, state + 32);
    rw::call(g::avatarActor_updateStatus, {avatar});          // AvatarActor::UpdateStatus
    uint32_t afn = ld(avatar + av::AVATAR);
    if (ld(afn + fl::BAD_STATUS) == 0) rw::call(g::flash_setBadStatus, {afn, 4});
    bt::releaseShared(ctrl);
}
RW_FUNCTION(0x7506E8, "StageActorManager::updateEnemy(EnemyActor*, AvatarActor*)", StageActorManager::updateEnemy);

// Same for an allied enemy attacking an enemy (the target takes the attack, no counter numbers).
void StageActorManager::updateEnemyEnemy(StageActorManager *self, uint32_t e, uint32_t target) {
    bt::GuestFrame f(0x10);                                   // +0 ActFlag, +8 shared_ptr<Burst>()
    uint32_t flash = e + en::FLASH;
    uint32_t fn = ld(flash);
    if (ld(fn + fl::SHIFT_FLAGS)) {
        switch (rw::call(g::flash_flagShift, {fn})) {         // FlashNode::flagShift
        case 1: {
            uint32_t id = rw::call(g::enemy_hitEffectID, {e});
            uint32_t pos = vc(target, 21, {target});
            uint32_t dir = rw::call(g::enemy_hitEffectDirection, {e});
            deployHitEffect(self, int32_t(id), pos, 70.0f, bt::fromBits(dir));
            break;
        }
        case 2:
            rw::store8(rw::host_ptr<uint8_t>(target + en::KILLABLE), 1);
            break;
        case 3:
            rw::store8(rw::host_ptr<uint8_t>(target + en::KILLABLE), 0);
            break;
        case 4: {
            int32_t m = ldi(ld(flash) + fl::ENEMY_ACTION);
            if (uint32_t(m - 3) <= 1) {
                rw::call(g::enemy_changeMotion, {e, 1});
            } else if (m == 6) {
                rw::call(g::enemy_setEndedAction, {e});
            } else if (m == 5) {
                if (isDied(e) == 0) rw::call(g::enemy_changeMotion, {e, 1});
            } else {
                rw::call(g::enemy_changeMotion, {e, 0});
            }
            break;
        }
        case 5:
            if (ld8(e + en::ACTIVE) && isDied(e) == 1) {
                if (ld8(e + en::KILLABLE)) {
                    rw::call(g::enemy_setDeadAction, {e});
                    rw::call(g::flash_setBadStatus, {ld(flash), 4});
                } else {
                    f.setWord(0, 5);
                    rw::call(g::deque_pushFront, {ld(flash) + fl::ACT_FLAGS, f.addr(0)});
                }
            }
            break;
        default:
            break;
        }
    }
    uint32_t fn2 = ld(flash);
    if (!ld(fn2 + fl::KNOCKBACK_FLAGS)) return;
    rw::call(g::flash_knockbackShift, {fn2});
    uint32_t bm = rw::call(g::battleMisc_getInstance, {});
    int32_t dmg = int32_t(rw::call(g::battleMisc_enemyAttack, {bm, e, 0}));  // BattleMisc::calculateEnemyAttack
    sb(target + en::STATUS)->set32(48, dmg);
    f.setWord(8, 0);
    f.setWord(12, 0);
    EnemyDamage(self, target, f.ptr<bt::SharedPtr>(8), 0);
}
RW_FUNCTION(0x750B0C, "StageActorManager::updateEnemy(EnemyActor*, EnemyActor*)", StageActorManager::updateEnemyEnemy);

// A hit effect on a fresh map node at pos (screen position, raised by offsetY).
void StageActorManager::deployHitEffect(StageActorManager *self, int32_t id, uint32_t pos, float offsetY, float dir) {
    uint32_t s = rw::arg(self);
    bt::GuestFrame f(0x28);                                   // +0 map pos, +8 Vec2, +0x10 std::function
    uint32_t node = rw::call(bt::fn::operator_new, {0x288});
    rw::call(g::mapNode_ctor, {node});                        // MapNode::MapNode
    vc(node, 83, {node, 11});
    rw::call(g::mapNode_lockZ, {node, 0x4F000000});           // MapNode::lockMapPosZ(2^31)
    rw::call(g::field_localToMap, {f.addr(0), self->field, pos});  // StageFieldManager::getLocalPosToMapPos
    float y = bt::fromBits(f.word(4)) - offsetY;
    rw::call(g::vec2_ctor, {f.addr(8), f.word(0), bt::bits(y)});
    rw::save<uint32_t>(node + 628, f.word(12));
    rw::save<uint32_t>(node + 624, f.word(8));
    rw::call(g::vec2_dtor, {f.addr(8)});
    rw::call(g::field_addMapNode, {self->field, node});       // StageFieldManager::addMapNodeToField
    makeFunction(f, 0x10, g::fn_hitEffect, {node, s});
    rw::call(g::hitEffect_deploy, {uint32_t(id), node, g::vec2_ZERO, 300, bt::bits(dir), f.addr(0x10)});  // HitEffect::deploy
    destroyFunction(f, 0x10);
    rw::call(bt::fn::ref_autorelease, {node});                // cocos2d::Ref::autorelease
    rw::call(g::vec2_dtor, {f.addr(0)});
}
RW_FUNCTION(0x750CB0, "StageActorManager::deployHitEffect", StageActorManager::deployHitEffect);

// The enemies an attack hits: single -> its enemy; whole / burst -> the active team members (living ones
// when aliveOnly == 1); otherwise none.
void StageActorManager::getEncountTargets(bt::Vector *out, StageActorManager *self, const AttackTarget *target,
                                          uint32_t aliveOnly) {
    bt::GuestFrame f(0x18);                                   // +0 vector, +0x0c element, +0x10 shared_ptr<Burst>
    int32_t type = target->type;
    if (type == 1) {
        f.setWord(0, 0);
        f.setWord(4, 0);
        f.setWord(8, 0);
        rw::call(g::push_back_slow, {f.addr(0), rw::arg(target) + 16});
    } else if (type == 2 || type == 3) {
        if (type == 3) {
            createBurst(f, 0x10, target->medalInfo);
            (void)sb(f.word(0x10))->get32(0x160);
            bt::releaseShared(f.word(0x14));
        }
        f.setWord(0, 0);
        f.setWord(4, 0);
        f.setWord(8, 0);
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
            uint32_t e = ld(it);
            f.setWord(0x0C, e);
            if (!ld8(e + en::ACTIVE)) continue;
            if (aliveOnly == 1 && isDied(e) != 0) continue;
            pushBackPtr(f.addr(0), e, f.addr(0x0C));
        }
    } else {
        out->begin = 0;
        out->end = 0;
        out->cap = 0;
        return;
    }
    out->end = 0;
    out->cap = 0;
    out->begin = f.word(0);
    out->end = f.word(4);
    out->cap = f.word(8);
    f.setWord(0, 0);
    f.setWord(4, 0);
    f.setWord(8, 0);
    rw::call(g::vectorBase_dtor, {f.addr(0)});                // ~vector (moved from)
}
RW_FUNCTION(0x750E08, "StageActorManager::getEncountTargets", StageActorManager::getEncountTargets);

// Where an attack lands: its enemy, or the centre of the active team; *scale = their mean hit scale.
void StageActorManager::computeAttackPosition(bt::Vec2 *out, const StageActorManager *self,
                                              const AttackTarget *target, const float *scale) {
    bt::GuestFrame f(8);
    rw::call(g::vec2_ctor, {f.addr(0), 0, 0});
    int32_t n = 0;
    float sum = 0.0f;
    uint32_t te = target->enemy;
    if (te) {
        uint32_t p = vc(te, 21, {te});
        uint32_t px = ld(p), py = ld(p + 4);
        f.setWord(4, py);
        f.setWord(0, px);
        sum = ldf(ld(target->enemy + en::FLASH) + fl::HIT_SCALE);
        n = 1;
    } else {
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
            uint32_t e = ld(it);
            if (!ld8(e + en::ACTIVE)) continue;
            n++;
            uint32_t p = vc(e, 21, {e});
            rw::call(g::vec2_add, {f.addr(0), p});            // cocos2d::Vec2::add
            sum = sum + ldf(ld(e + en::FLASH) + fl::HIT_SCALE);
        }
        if (n == 0) {
            rw::call(g::vec2_copy, {rw::arg(out), g::vec2_ZERO});
            rw::call(g::vec2_dtor, {f.addr(0)});
            return;
        }
    }
    float inv = 1.0f / float(n);
    rw::call(g::vec2_scale, {f.addr(0), bt::bits(inv)});     // cocos2d::Vec2::scale
    if (scale) rw::store32(scale, bt::bits(sum * inv));
    rw::call(g::vec2_copy, {rw::arg(out), f.addr(0)});
    rw::call(g::vec2_dtor, {f.addr(0)});
}
RW_FUNCTION(0x750F74, "StageActorManager::computeAttackPosition", StageActorManager::computeAttackPosition);

// One target: single attacks, and bursts of kind 1.
bool StageActorManager::isSingle(StageActorManager *, const AttackTarget *target) {
    int32_t type = target->type;
    if (type == 2) return false;
    if (type != 3) return true;
    bt::GuestFrame f(8);
    createBurst(f, 0, target->medalInfo);
    int32_t kind = sb(f.word(0))->get32(0x160);
    bt::releaseShared(f.word(4));
    return kind == 1;
}
RW_FUNCTION(0x7510B8, "StageActorManager::isSingle", StageActorManager::isSingle);

// Apply an enemy's pending damage: HP (killed enemies go to the crushed list), the damage number, the hurt motion.
void StageActorManager::EnemyDamage(StageActorManager *self, uint32_t e, bt::SharedPtr *burst, uint32_t bursted) {
    SafeBuffer *status = sb(e + en::STATUS);
    if (status->get32(48) == 0) return;
    if (isDied(e) != 0) return;
    int32_t dmg = status->get32(48);
    bt::GuestFrame f(0x18);                                   // +0 shared_ptr copy, +8 element, +0x10 Vec2
    uint32_t ctrl = burst->ctrl;
    f.setWord(0, burst->obj);
    f.setWord(4, ctrl);
    addShared(ctrl);
    uint32_t killed = rw::call(g::enemy_addDamage, {e, uint32_t(dmg), f.addr(0)});  // EnemyActor::addDamage
    bt::releaseShared(f.word(4));
    if (killed == 1) pushBackPtr(rw::arg(&self->crushed), e, f.addr(8));
    int32_t shown = status->get32(48);
    uint32_t node = ld(self->direction + 628);
    uint32_t pos = vc(e, 21, {e});
    rw::call(g::vec2_copy, {f.addr(0x10), pos});
    uint32_t numberScale = bursted ? 0x3F99999Au : 0x3F800000u;   // 1.2 / 1.0
    uint32_t hitScale = ld(ld(e + en::FLASH) + fl::HIT_SCALE);
    uint32_t flags = ld(e + en::DAMAGE_FLAGS);
    rw::call(g::dispDamageNumber, {uint32_t(shown), node, f.addr(0x10), hitScale, (flags & 2) >> 1, (flags & 4) >> 2, 0,
                                   flags & 1, 400, numberScale});  // DisplayDamageNumber::disp_dmgNum
    rw::call(g::vec2_dtor, {f.addr(0x10)});
    rw::call(g::flash_reflectionStatus, {ld(e + en::FLASH), 0});  // FlashNode::reflection_status
    rw::call(g::enemy_changeMotion, {e, 5});                  // EnemyActor::changeMotion (hurt)
}
RW_FUNCTION(0x751174, "StageActorManager::EnemyDamage", StageActorManager::EnemyDamage);

// Bursts of kind 3 hit random enemies.
bool StageActorManager::isRandom(StageActorManager *, const AttackTarget *target) {
    if (target->type != 3) return false;
    bt::GuestFrame f(8);
    createBurst(f, 0, target->medalInfo);
    int32_t kind = sb(f.word(0))->get32(0x160);
    bt::releaseShared(f.word(4));
    return kind == 3;
}
RW_FUNCTION(0x751348, "StageActorManager::isRandom", StageActorManager::isRandom);

// Mark one random markable enemy of the team (50% each in order, else the first).
void StageActorManager::SelectRandom(StageActorManager *self) {
    uint32_t it = self->team.begin, end = self->team.end;
    if (it == end) return;
    for (; it != end; it += 4) {
        uint32_t e = ld(it);
        actionMarker(e, 4);
        rw::store8(rw::host_ptr<uint8_t>(e + en::RANDOM_MARK), 0);
    }
    it = self->team.begin;
    end = self->team.end;
    if (it == end) return;
    for (; it != end; it += 4) {
        uint32_t e = ld(it);
        if (rw::call(g::enemy_isMarking, {e}) != 1) continue;  // EnemyActor::isMarking
        uint32_t rnd = rw::call(g::random_getInstance, {});
        float r = bt::fromBits(rw::call(g::random_uniformFloat, {rnd, 0, 0x3F800000u, 0}));  // [0, 1)
        if (!(r < 0.5f)) {
            actionMarker(e, 1);
            rw::store8(rw::host_ptr<uint8_t>(e + en::RANDOM_MARK), 1);
            return;
        }
    }
    for (it = self->team.begin, end = self->team.end; it != end; it += 4) {
        uint32_t e = ld(it);
        if (rw::call(g::enemy_isMarking, {e}) == 1) {
            actionMarker(e, 1);
            rw::store8(rw::host_ptr<uint8_t>(e + en::RANDOM_MARK), 1);
            return;
        }
    }
}
RW_FUNCTION(0x7513F4, "StageActorManager::SelectRandom", StageActorManager::SelectRandom);

// Compute the player's damage into each target's pending damage (conditions first) and mark the targets.
void StageActorManager::storeDamage(StageActorManager *self, uint32_t, const AttackTarget *target) {
    uint32_t t = rw::arg(target);
    if (isSingle(self, target)) {
        uint32_t e = target->enemy;
        addCondition(self, target, e);
        uint32_t bm = rw::call(g::battleMisc_getInstance, {});
        int32_t dmg = int32_t(rw::call(g::battleMisc_playerAttack, {bm, t, e}));  // BattleMisc::calculatePlayerAttack
        sb(e + en::STATUS)->set32(48, dmg);
        uint32_t bm2 = rw::call(g::battleMisc_getInstance, {});
        rw::save<uint32_t>(e + en::DAMAGE_FLAGS, ld(bm2 + 160));
    } else {
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
            uint32_t e = ld(it);
            if (ld8(e + en::ACTIVE)) {
                addCondition(self, target, e);
                uint32_t bm = rw::call(g::battleMisc_getInstance, {});
                int32_t dmg = int32_t(rw::call(g::battleMisc_playerAttack, {bm, t, e}));
                sb(e + en::STATUS)->set32(48, dmg);
                uint32_t bm2 = rw::call(g::battleMisc_getInstance, {});
                rw::save<uint32_t>(e + en::DAMAGE_FLAGS, ld(bm2 + 160));
            }
            uint8_t mark;
            if (isRandom(self, target)) {
                actionMarker(e, 4);
                mark = 0;
            } else {
                mark = 1;
                actionMarker(e, 1);
            }
            rw::store8(rw::host_ptr<uint8_t>(e + en::RANDOM_MARK), mark);
        }
    }
    for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4)
        rw::call(g::enemy_updateStatus, {ld(it)});            // EnemyActor::UpdateStatus
}
RW_FUNCTION(0x7514A0, "StageActorManager::storeDamage", StageActorManager::storeDamage);

// The bad status conditions of the attack's skill: each one the enemy is not immune to and fails to resist
// (rate out of 10000) is applied; otherwise "miss".
void StageActorManager::addCondition(StageActorManager *self, const AttackTarget *target, uint32_t e) {
    if (!target->skill.obj) return;
    uint32_t ctrl = ld(e + en::PARAM_CTRL);
    uint32_t row = ld(e + en::PARAM);
    addShared(ctrl);
    struct Cond {
        uint32_t flag, immune, resist, type, badStatus, buff;
        bool lock;
    };
    static const Cond conds[4] = {
        {0x110, 0x38C, 0x39C, 3, 0, 19, false},
        {0x10C, 0x388, 0x398, 0, 0, 16, false},
        {0x114, 0x390, 0x3A0, 1, 1, 17, true},
        {0x118, 0x394, 0x3A4, 2, 2, 18, false},
    };
    bt::GuestFrame f(8);
    for (const Cond &c : conds) {
        if (sb(target->skill.obj)->get32(c.flag) == 0) continue;
        bool hit = false;
        if (sb(row)->get32(c.immune) == 0) {
            int32_t resist = sb(row)->get32(c.resist);
            uint32_t rnd = rw::call(g::random_getInstance, {});
            int32_t roll = int32_t(rw::call(g::random_uniformInt, {rnd, 0, 9999, 0}));  // Random::generateUniformInt
            hit = resist > roll;
        }
        if (hit) {
            int32_t p1 = sb(target->skill.obj)->get32(0x11C);
            int32_t p2 = sb(target->skill.obj)->get32(0x120);
            rw::call(g::enemy_addCondition, {e + en::STATUS, c.type, uint32_t(p1), uint32_t(p2)});  // EnemyActor::Property::addCondition
            rw::call(g::flash_setBadStatus, {ld(e + en::FLASH), c.badStatus});
            rw::call(g::hpIndicator_setBufDebuf, {ld(e + en::HP_INDICATOR), c.buff, 21});  // RaidBossHPIndicator::set_buf_debuf
            if (c.lock) rw::store8(rw::host_ptr<uint8_t>(e + en::CONDITION_LOCK), 1);
        } else {
            uint32_t node = ld(self->direction + 628);
            uint32_t pos = vc(e, 21, {e});
            rw::call(g::vec2_copy, {f.addr(0), pos});
            rw::call(g::missBoard_disp, {node, f.addr(0), ld(ld(e + en::FLASH) + fl::HIT_SCALE), 0x3F800000u});  // MissBoard::disp_miss
            rw::call(g::vec2_dtor, {f.addr(0)});
        }
    }
    bt::releaseShared(ctrl);
}
RW_FUNCTION(0x751624, "StageActorManager::addCondition", StageActorManager::addCondition);

// After the counter attack: targets that took damage lose the type-1 condition and their pending damage,
// buffs count down; the player's attack buffs count down and the reflected damage hits the player.
void StageActorManager::reflectionCondition(StageActorManager *self, uint32_t avatar, const AttackTarget *target) {
    bt::GuestFrame f(16);                                     // StatusChange
    if (isSingle(self, target)) {
        uint32_t e = target->enemy;
        if (e && sb(e + en::STATUS)->get32(48) != 0) {
            e = target->enemy;
            if (!ld8(e + en::CONDITION_LOCK)) {
                clearStatus(f, 0, e + en::BADSTATUS);
                e = target->enemy;
            }
            rw::store8(rw::host_ptr<uint8_t>(e + en::CONDITION_LOCK), 0);
            sb(target->enemy + en::STATUS)->set32(48, 0);
            rw::call(g::buff_elapseDefense, {target->enemy + en::BUFF});  // IBuffProtocol::elapseCountDefenseBuff
            rw::call(g::enemy_updateStatus, {target->enemy});           // EnemyActor::UpdateStatus
            uint32_t fn = ld(target->enemy + en::FLASH);
            if (ld(fn + fl::BAD_STATUS) == 0) rw::call(g::flash_setBadStatus, {fn, 4});
        }
    } else {
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
            uint32_t e = ld(it);
            if (sb(e + en::STATUS)->get32(48) == 0) continue;
            if (!ld8(e + en::CONDITION_LOCK)) clearStatus(f, 0, e + en::BADSTATUS);
            rw::store8(rw::host_ptr<uint8_t>(e + en::CONDITION_LOCK), 0);
            sb(e + en::STATUS)->set32(48, 0);
            rw::call(g::buff_elapseDefense, {e + en::BUFF});
            rw::call(g::enemy_updateStatus, {e});
            uint32_t fn = ld(e + en::FLASH);
            if (ld(fn + fl::BAD_STATUS) == 0) rw::call(g::flash_setBadStatus, {fn, 4});
        }
    }
    rw::call(g::buff_elapseAttack, {stageState() + 36});      // IBuffProtocol::elapseCountAttackBuff (StageState)
    rw::call(g::avatarActor_updateStatus, {avatar});          // AvatarActor::UpdateStatus
    int32_t reflected = sb(stageState())->get32(0x4248);
    if (reflected) {
        rw::call(g::stageState_addDamage, {stageState(), uint32_t(reflected)});  // StageState::addDamage
        sb(stageState())->set32(0x4248, 0);
    }
}
RW_FUNCTION(0x751B28, "StageActorManager::reflectionCondition", StageActorManager::reflectionCondition);

// ============================================================================================ touch
bool StageActorManager::onTapBeganEnemyActor(StageActorManager *self, uint32_t e) {
    uint32_t s = rw::arg(self);
    if (ldi(ld(self->player + av::AVATAR) + fl::MOTION) == 47) return false;  // bursting
    int32_t st = ldi(e + en::STATE);
    if (st == 3) {
        if (self->canTapEnemy.f != 0 && rw::call(g::fnBool_call, {s + 344}) != 1) return false;
        if (self->attackEntered) return false;
        if (self->onTapEnemy.f == 0) return false;
        rw::call(g::fnVoid_call, {s + 368});                  // onTapEnemy()
        return true;
    }
    if (st == 2) return ldi(e + en::SYMBOL_KIND) == 5;
    return false;
}
RW_FUNCTION(0x751D60, "StageActorManager::onTapBeganEnemyActor", StageActorManager::onTapBeganEnemyActor);

// Tap released on an enemy: in battle, attack it with the medal (once per turn); on the map, a tap-only symbol
// starts the encounter.
uint32_t StageActorManager::onTapEndedEnemyActor(StageActorManager *self, uint32_t medalInfo, uint32_t e) {
    uint32_t s = rw::arg(self);
    if (ldi(ld(self->player + av::AVATAR) + fl::MOTION) == 47) return 0;
    int32_t st = ldi(e + en::STATE);
    if (st == 3) {
        if (self->canTapEnemy.f != 0 && rw::call(g::fnBool_call, {s + 344}) != 1) return 0;
        if (self->attackEntered) return 0;
        self->attackEntered = 1;
        bt::GuestFrame f(24);
        return entryCurrentTarget(self, stackTarget(f, 0, 1, medalInfo, e));
    }
    if (st == 2 && ldi(e + en::SYMBOL_KIND) == 5) {
        if (self->tappedSymbol == 0) self->tappedSymbol = e;
    }
    return 0;
}
RW_FUNCTION(0x751DDC, "StageActorManager::onTapEndedEnemyActor", StageActorManager::onTapEndedEnemyActor);

// ============================================================================================ friend
// The friend appears at the player's position and summons.
void StageActorManager::startFriendCoveringFire(StageActorManager *self) {
    uint32_t pl = self->player;
    uint32_t dir = vc(pl, 11, {pl});
    uint32_t x = ld(pl + av::POSITION), y = ld(pl + av::POSITION + 4);
    uint32_t fr = self->friendAvatar;
    rw::save<uint32_t>(fr + av::POSITION + 4, y);
    rw::save<uint32_t>(fr + av::POSITION, x);
    fr = self->friendAvatar;
    vc(fr, 10, {fr, dir});
    fr = self->friendAvatar;
    vc(fr, 44, {fr, 1});
    rw::call(g::avatar_changeMotion, {ld(self->friendAvatar + av::AVATAR), 29});  // Avatar::changeMotion
    float z = ldf(pl + av::POSITION + 4) + 1.0f;
    rw::call(g::mapNode_lockZ, {self->friendAvatar, bt::bits(z)});              // MapNode::lockMapPosZ
    self->friendCoveringFire = 1;
    rw::call(g::avatar_changeMotion, {ld(self->player + av::AVATAR), 44});
}
RW_FUNCTION(0x751EB0, "StageActorManager::startFriendCoveringFire", StageActorManager::startFriendCoveringFire);

uint32_t StageActorManager::isEndFriendSummons(StageActorManager *self) {
    if (rw::call(g::flash_isPlaying, {ld(self->friendAvatar + av::AVATAR)}) != 0) return 0;  // FlashNode::isPlaying
    rw::call(g::avatar_changeMotion, {ld(self->friendAvatar + av::AVATAR), 11});
    return 1;
}
RW_FUNCTION(0x751F26, "StageActorManager::isEndFriendSummons", StageActorManager::isEndFriendSummons);

// The friend fires the current medal's burst and the attack is chosen automatically.
void StageActorManager::SetFriendBurst(StageActorManager *self) {
    uint32_t info = rw::call(g::medalDeck_currentInfo, {self->medalDeck});  // MedalDeck::getCurrentMedalInfo
    uint32_t fr = self->friendAvatar;
    int32_t burst = medalGet(info, 0x4AC);
    rw::call(g::avatar_activeBurst, {ld(fr + av::AVATAR), uint32_t(burst)});  // Avatar::activeBurst
    rw::call(g::avatar_changeMotion, {ld(self->friendAvatar + av::AVATAR), 47});
    int32_t v = medalGet(info, 0x4A8);
    autoChoiceTarget(self, info, v != 0);
}
RW_FUNCTION(0x751F50, "StageActorManager::SetFriendBurst", StageActorManager::SetFriendBurst);

void StageActorManager::initFriendCoveringFireAction(StageActorManager *self, int32_t type) {
    self->coveringFireState = 1;
    self->coveringFireType = type;
}
RW_FUNCTION(0x75200C, "StageActorManager::initFriendCoveringFireAction", StageActorManager::initFriendCoveringFireAction);

// One step of the friend's covering fire (appear, [jump up], burst, [fall], leave); 1 = done.
uint32_t StageActorManager::execFriendCoveringFireAction(StageActorManager *self, uint32_t player, uint32_t fr) {
    uint32_t frAvatar = fr + av::AVATAR;
    uint32_t fn = ld(frAvatar);
    int32_t state = self->coveringFireState;
    if (uint32_t(state - 1) > 5) return 1;
    int32_t motion = ldi(fn + fl::MOTION);
    bool burst = false;
    switch (state) {
    case 1: {
        rw::call(g::field_addMapNode, {self->field, fr});     // StageFieldManager::addMapNodeToField
        uint32_t x = ld(player + av::POSITION), y = ld(player + av::POSITION + 4);
        rw::save<uint32_t>(fr + av::POSITION + 4, y);
        rw::save<uint32_t>(fr + av::POSITION, x);
        uint32_t setDirection = ld(ld(fr) + 40);
        uint32_t dir = vc(player, 11, {player});
        rw::call(setDirection, {fr, dir});
        vc(fr, 44, {fr, 1});
        float z = ldf(fr + av::POSITION + 4) + 1.0f;
        rw::call(g::mapNode_lockZ, {fr, bt::bits(z)});        // MapNode::lockMapPosZ
        rw::call(g::avatar_changeMotion, {ld(frAvatar), 29}); // Avatar::changeMotion
        rw::call(g::avatar_changeMotion, {ld(player + av::AVATAR), 44});
        self->coveringFireState = 2;
        return 0;
    }
    case 2:
        if ((motion & ~0x10) != 11) return 0;
        if (self->coveringFireType) {
            doJumpUpAction(self, fr);
            self->coveringFireState = 3;
            return 0;
        }
        burst = true;
        break;
    case 3:
        if (rw::call(g::mapNode_updatePosition, {fr}) != 0) return 0;  // MapNode::updateMapPosition (still moving)
        burst = true;
        break;
    case 4:
        if ((motion & ~0x10) != 11) return 0;
        if (self->coveringFireType) {
            doJumpFallAction(self, fr);
            self->coveringFireState = 5;
        } else {
            rw::call(g::avatar_changeMotion, {fn, 30});
            rw::call(g::avatar_changeMotion, {ld(player + av::AVATAR), 45});
            self->coveringFireState = 6;
        }
        if (self->cutInShader) rw::call(g::cutIn_end, {self->cutInShader});  // GuiltBurstCutInShader::setShaderEnd
        return 0;
    case 5:
        if (rw::call(g::mapNode_updatePosition, {fr}) != 0) return 0;
        rw::call(g::avatar_changeMotion, {ld(frAvatar), 30});
        rw::call(g::avatar_changeMotion, {ld(player + av::AVATAR), 45});
        self->coveringFireState = 6;
        return 0;
    case 6:
        if (rw::call(g::avatarActor_takeActionFlag, {fr, 4}) != 1) return 0;  // AvatarActor::takeActionFlag
        rw::call(g::field_removeMapNode, {self->field, fr});  // StageFieldManager::removeMapNodeToField
        self->coveringFireState = 0;
        self->coveringFireType = 0;
        return 1;
    }
    if (burst) {
        uint32_t info = rw::call(g::medalDeck_currentInfo, {self->medalDeck});  // MedalDeck::getCurrentMedalInfo
        int32_t id = medalGet(info, 0x4AC);
        rw::call(g::avatar_activeBurst, {ld(frAvatar), uint32_t(id)});  // Avatar::activeBurst
        rw::call(g::avatar_changeMotion, {ld(frAvatar), 47});
        if (self->cutInShader) rw::call(g::cutIn_concentrationStart, {self->cutInShader});
        self->coveringFireState = 4;
    }
    return 0;
}
RW_FUNCTION(0x75201C, "StageActorManager::execFriendCoveringFireAction",
                     StageActorManager::execFriendCoveringFireAction);

// Jump up to the height of the jump target.
void StageActorManager::doJumpUpAction(StageActorManager *self, uint32_t avatar) {
    bt::GuestFrame f(0x20);                                   // +0 Vec2, +8 std::function
    uint32_t h = ld(ld(self->jumpTarget + en::FLASH) + fl::HEIGHT);
    rw::call(g::vec2_ctor, {f.addr(0), 0, h});
    makeFunction(f, 8, g::fn_jumpUp, {avatar});
    uint32_t cf = rw::call(g::callFunc_create, {f.addr(8)});  // cocos2d::CallFunc::create
    uint32_t mv = rw::call(g::moveBy_create, {0x3E800000u, f.addr(0)});  // cocos2d::MoveBy::create(0.25)
    uint32_t ease = rw::call(g::easeOut_create, {mv, 0x40000000u});      // cocos2d::EaseOut::create(2.0)
    uint32_t seq = rw::call(g::sequence_create, {cf, ease, 0});         // cocos2d::Sequence::create
    destroyFunction(f, 8);
    rw::call(g::mapNode_move, {avatar, seq});                 // MapNode::move
    rw::call(g::vec2_dtor, {f.addr(0)});
}
RW_FUNCTION(0x7522A4, "StageActorManager::doJumpUpAction", StageActorManager::doJumpUpAction);

// Jump back down (200 units backwards).
void StageActorManager::doJumpFallAction(StageActorManager *self, uint32_t avatar) {
    bt::GuestFrame f(0x38);                                   // +0 Vec2, +8 / +0x20 std::function
    uint32_t tgt = self->jumpTarget;
    uint32_t dir = vc(avatar, 11, {avatar});
    float x = bt::fromBits(dir) * 200.0f;
    uint32_t h = ld(ld(tgt + en::FLASH) + fl::HEIGHT) ^ 0x80000000u;
    rw::call(g::vec2_ctor, {f.addr(0), bt::bits(x), h});
    makeFunction(f, 8, g::fn_jumpFall1, {avatar});
    uint32_t cf1 = rw::call(g::callFunc_create, {f.addr(8)});
    uint32_t jump = rw::call(g::jumpBy_create, {0x3F19999Au, f.addr(0), 0x43480000u, 1});  // JumpBy(0.6, v, 200, 1)
    makeFunction(f, 0x20, g::fn_jumpFall2, {avatar});
    uint32_t cf2 = rw::call(g::callFunc_create, {f.addr(0x20)});
    uint32_t seq = rw::call(g::sequence_create, {cf1, jump, cf2, 0});
    destroyFunction(f, 0x20);
    destroyFunction(f, 8);
    rw::call(g::mapNode_move, {avatar, seq});
    rw::call(g::vec2_dtor, {f.addr(0)});
}
RW_FUNCTION(0x75239C, "StageActorManager::doJumpFallAction", StageActorManager::doJumpFallAction);

bool StageActorManager::isFriendCoveringFireCameraTargetPlayer(StageActorManager *self) {
    if (self->coveringFireType != 1) return true;
    return uint32_t(self->coveringFireState - 3) > 2;
}
RW_FUNCTION(0x752514, "StageActorManager::isFriendCoveringFireCameraTargetPlayer",
            StageActorManager::isFriendCoveringFireCameraTargetPlayer);

// ============================================================================================ jump attacks
// An enemy the attack can only reach by jumping (enemy row 0x270 == 1): the attack's own enemy, else the
// first fighting team member.
uint32_t StageActorManager::findNeedJumpAttackEnemy(StageActorManager *self, const AttackTarget *target) {
    if (target->enemy) {
        if (enemyParam(target->enemy, 0x270) == 1 && target->enemy) return target->enemy;
    }
    for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
        uint32_t e = ld(it);
        if (ld(e + en::STATE) != 3) continue;
        if (enemyParam(e, 0x270) == 1) return e;
    }
    return 0;
}
RW_FUNCTION(0x752538, "StageActorManager::findNeedJumpAttackEnemy", StageActorManager::findNeedJumpAttackEnemy);

bool StageActorManager::isJumpAttackAction(StageActorManager *self, const AttackTarget *target, float offset) {
    uint32_t e = findNeedJumpAttackEnemy(self, target);
    self->jumpTarget = e;
    self->jumpOffset = offset;
    return e != 0;
}
RW_FUNCTION(0x75266C, "StageActorManager::isJumpAttackAction", StageActorManager::isJumpAttackAction);

// Move next to the jump target (keeping jumpOffset on the near side).
void StageActorManager::doJumpApprpachAction(StageActorManager *self, uint32_t avatar) {
    bt::GuestFrame f(0x20);                                   // +0 Vec2, +8 std::function (4 words)
    uint32_t tgt = self->jumpTarget;
    uint32_t tp = vc(tgt, 21, {tgt});
    uint32_t ap = vc(avatar, 21, {avatar});
    uint32_t v = rw::call(g::vec2_copy, {f.addr(0), tp});
    rw::call(g::vec2_subtract, {v, ap});                      // cocos2d::Vec2::subtract
    float x = bt::fromBits(f.word(0));
    float dir = x < 0.0f ? 1.0f : -1.0f;
    float off = self->jumpOffset * dir;
    f.setWord(0, bt::bits(x + off));
    makeFunction(f, 8, g::fn_approach, {avatar, bt::bits(dir), tgt});
    uint32_t cf = rw::call(g::callFunc_create, {f.addr(8)});
    uint32_t mv = rw::call(g::moveTo_create, {0x3E2AAAABu, f.addr(0)});  // cocos2d::MoveTo::create(1/6)
    uint32_t ease = rw::call(g::easeSineOut_create, {mv});
    uint32_t seq = rw::call(g::sequence_create, {cf, ease, 0});
    destroyFunction(f, 8);
    rw::call(g::mapNode_move, {avatar, seq});
    rw::call(g::vec2_dtor, {f.addr(0)});
}
RW_FUNCTION(0x752698, "StageActorManager::doJumpApprpachAction", StageActorManager::doJumpApprpachAction);

// ============================================================================================ allies
// Every ally attacks after a random delay of 2..18 frames.
void StageActorManager::startAllyAttack(StageActorManager *self, float dt) {
    uint32_t s = rw::arg(self);
    bt::GuestFrame f(0x18);                                   // std::function (3 captures)
    for (uint32_t it = self->allies.begin, end = self->allies.end; it != end; it += 4) {
        uint32_t e = ld(it);
        uint32_t rnd = rw::call(g::random_getInstance, {});
        int32_t frames = int32_t(rw::call(g::random_uniformInt, {rnd, 2, 18, 0}));
        float delay = float(frames) / 60.0f;
        uint32_t dl = rw::call(g::delayTime_create, {bt::bits(delay)});  // cocos2d::DelayTime::create
        makeFunction(f, 0, g::fn_allyAttack, {s, e, bt::bits(dt)});
        uint32_t cf = rw::call(g::callFunc_create, {f.addr(0)});
        uint32_t seq = rw::call(g::sequence_create, {dl, cf, 0});
        destroyFunction(f, 0);
        rw::call(g::mapNode_move, {e, seq});                  // MapNode::move
    }
}
RW_FUNCTION(0x7527F8, "StageActorManager::startAllyAttack", StageActorManager::startAllyAttack);

// All allies stopped moving and are idle (action kind 0 or 1).
bool StageActorManager::isEndAllyAttack(StageActorManager *self) {
    uint32_t b = self->allies.begin, end = self->allies.end;
    int32_t done = 0;
    if (b != end) {
        for (uint32_t it = b; it != end; it += 4) {
            uint32_t e = ld(it);
            if (rw::call(g::mapNode_updatePosition, {e}) == 0 && ld(ld(e + en::FLASH) + fl::ENEMY_ACTION) < 2) done++;
        }
        b = self->allies.begin;
        end = self->allies.end;
    }
    return (int32_t(end - b) >> 2) <= done;
}
RW_FUNCTION(0x752904, "StageActorManager::isEndAllyAttack", StageActorManager::isEndAllyAttack);

void StageActorManager::doAttackAllyActor(StageActorManager *self, uint32_t e, float dt) {
    uint32_t lead = firstLeader(self->team);
    if (lead && isDied(lead) == 0)
        rw::call(g::enemy_selectAttack, {e, bt::bits(dt), rw::arg(self)});  // EnemyActor::select_enemy_attack
}
RW_FUNCTION(0x75296C, "StageActorManager::doAttackAllyActor", StageActorManager::doAttackAllyActor);

// After an attack on an enemy that pushes back (enemy row 0x270 == 2) within 200 units: jump away.
uint32_t StageActorManager::doDisengageAttackEnd(StageActorManager *self, uint32_t avatar, const AttackTarget *target) {
    uint32_t e = 0;
    if (target->type == 2) {
        for (uint32_t it = self->team.begin, end = self->team.end; it != end; it += 4) {
            uint32_t m = ld(it);
            if (ld(m + en::STATE) != 3) continue;
            if (enemyParam(m, 0x270) == 2) {
                e = m;
                break;
            }
        }
        if (!e) return 0;
    } else if (target->type == 1) {
        if (!target->enemy) return 0;
        if (enemyParam(target->enemy, 0x270) != 2) return 0;
        e = target->enemy;
        if (!e) return 0;
    } else {
        return 0;
    }
    float gap = 200.0f - std::fabs(ldf(e + en::POSITION) - ldf(avatar + av::POSITION));
    if (!(gap > 0.0f)) return 0;
    bt::GuestFrame f(0x38);                                   // +0 Vec2, +8 / +0x20 std::function
    float s1 = gap + 1.0f;
    uint32_t dir = vc(avatar, 11, {avatar});
    float x = s1 * bt::fromBits(dir);
    rw::call(g::vec2_ctor, {f.addr(0), bt::bits(x), 0});
    makeFunction(f, 0x20, g::fn_disengage1, {avatar});
    uint32_t cf1 = rw::call(g::callFunc_create, {f.addr(0x20)});
    float q = gap / 200.0f;
    float dur = q * 0.3f;
    uint32_t jump = rw::call(g::jumpBy_create, {bt::bits(dur), f.addr(0), 0x41A00000u, 1});  // JumpBy(dur, v, 20, 1)
    makeFunction(f, 8, g::fn_disengage2, {avatar});
    uint32_t cf2 = rw::call(g::callFunc_create, {f.addr(8)});
    uint32_t seq = rw::call(g::sequence_create, {cf1, jump, cf2, 0});
    destroyFunction(f, 8);
    destroyFunction(f, 0x20);
    rw::call(g::mapNode_move, {avatar, seq});
    rw::call(g::vec2_dtor, {f.addr(0)});
    return 1;
}
RW_FUNCTION(0x7529B8, "StageActorManager::doDisengageAttackEnd", StageActorManager::doDisengageAttackEnd);

void StageActorManager::pauseToAllActor(StageActorManager *self) {
    for (uint32_t it = self->avatars.begin, end = self->avatars.end; it != end; it += 4)
        rw::call(g::avatarActor_pause, {ld(it)});             // AvatarActor::pause
    for (uint32_t it = self->mapEnemies.begin, end = self->mapEnemies.end; it != end; it += 4)
        rw::call(g::enemy_pause, {ld(it)});                   // EnemyActor::pause
    for (uint32_t it = self->allies.begin, end = self->allies.end; it != end; it += 4)
        rw::call(g::enemy_pause, {ld(it)});
}
RW_FUNCTION(0x752C8C, "StageActorManager::pauseToAllActor", StageActorManager::pauseToAllActor);

void StageActorManager::resumeToAllActor(StageActorManager *self) {
    for (uint32_t it = self->avatars.begin, end = self->avatars.end; it != end; it += 4)
        rw::call(g::avatarActor_resume, {ld(it)});            // AvatarActor::resume
    for (uint32_t it = self->mapEnemies.begin, end = self->mapEnemies.end; it != end; it += 4)
        rw::call(g::enemy_resume, {ld(it)});                  // EnemyActor::resume
    for (uint32_t it = self->allies.begin, end = self->allies.end; it != end; it += 4)
        rw::call(g::enemy_resume, {ld(it)});
}
RW_FUNCTION(0x752CD0, "StageActorManager::resumeToAllActor", StageActorManager::resumeToAllActor);
